#include "wrapper.hpp"
#include "main.h"
#include "state_type.hpp"
#include <bits/stdc++.h>
#include "string"
using namespace std;
#include "usart.h" // UARTの設定を読み込む
#include "stdio.h"
// 使用したい通信プロトコルのヘッダーファイルをインクルード
#include "ICM42688P_HAL_I2C.h"

#include "FuncList.hpp"

#include "math.h"
#include <cstring>

// EKF library
#include "attitude_ekf.h"

// EKF instance
AttitudeEKF_t* attitude_ekf = nullptr;

// センサーデータ
float accel_data[3] = {0, 0, 0};  // m/s^2
float gyro_data[3] = {0, 0, 0};   // deg/s

State current_state = State::Start;
Context context;
uint8_t  ReceiveBuffer[25];
uint16_t SBUSData[10] = {};

ICM42688P_HAL_I2C icm(&hi2c1, 0b1101001);

int sbusdata3ch ,sbusdata9ch, sbusdata1ch, sbusdata4ch, sbusdata2ch;

void SBUS_decode();

// --- マイクロ秒(μs)タイマー機能 ---
void DWT_Init(void) {
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

uint32_t micros(void) {
    return DWT->CYCCNT / (SystemCoreClock / 1000000);
}
// ----------------------------------

void init(){

//起動時に1度だけ実行される
	context.count = 0;
	//割り込みの開始
	HAL_UART_Receive_DMA(&huart2, ReceiveBuffer, 25);
	printf("program start\n");

	// DWTマイクロ秒タイマーの起動
	DWT_Init();

	// 通信チェック
	if(icm.Connection()){
		printf("ICM42688p Not Found\n");
		while(1){
			;
		}
	}
	printf("found\n");
	// センサーの設定
	icm.AccelConfig(icm.ACCEL_Mode::LowNoize, icm.ACCEL_SCALE::SCALE02g, icm.ACCEL_ODR::ODR01000hz, icm.ACCEL_DLPF::ODR40);
	icm.GyroConfig(icm.GYRO_MODE::LowNoize, icm.GYRO_SCALE::Dps0250, icm.GYRO_ODR::ODR01000hz, icm.GYRO_DLPF::ODR40);
	//ここから姿勢推定
	printf("=== System Start ===\n");

	// ----- EKF初期化 -----
	printf("[EKF] Initializing...\n");
	attitude_ekf = new AttitudeEKF_t();

	if (!AttitudeEKF_Init(attitude_ekf, SS_DT)) {

		printf("[EKF] ERROR: Initialization failed\n");
		while(1){
            HAL_Delay(1000);
        }
	}
    else {

		printf("[EKF] Initialized\n");
	}

	printf("=== Initialization Complete ===\n\n");
}

void loop(){
	static uint32_t last_us = 0;
	const uint32_t PERIOD_US = 2500; // 400Hz = 2.5ms = 2500us

	uint32_t now = micros();

	// 2500μs (2.5ms) 経過していない場合は何もせず抜ける
	if (now - last_us < PERIOD_US) {
		return;
	}
	last_us = now;

	// ==========================================
	//  以下が 400Hz (2.5ms周期) で実行される処理
	// ==========================================

	sbusdata9ch = SBUSData[8];
	sbusdata3ch = SBUSData[2];
	sbusdata1ch = SBUSData[0];
	sbusdata4ch = SBUSData[3];
	sbusdata2ch = SBUSData[1];

	switch(current_state){
	    case State::Start:
	    	startf(&current_state, &context);
	        break;
	    case State::Init:
	        initf(&current_state, &context);
	        break;
	    case State::Calib:
	        calibf(&current_state, &context);
            break;
	    case State::Prearm:
	    	prearmf(&current_state, &context, sbusdata9ch);
	    	break;
        case State::Arm:
            armf(&current_state, &context, sbusdata9ch);
            break;
        case State::Fly:
            flyf(&current_state, &context, sbusdata9ch, sbusdata3ch, sbusdata1ch, sbusdata4ch, sbusdata2ch);
            break;
        case State::Dis:
            disf(&current_state, &context);
            break;
	}

	icm.GetData(accel_data, gyro_data);

	// デバッグ用の送信・printf は処理が重いため100ループに1回(4Hz)だけ実行
	static uint32_t debug_counter = 0;
	debug_counter++;
	if(debug_counter >= 100){
		debug_counter = 0;

		std::string str;
		for(int i = 0; i < 6; i++){
			str = std::to_string(SBUSData[i]) + " ";
			HAL_UART_Transmit(&huart2, (uint8_t *)str.c_str(),str.length(),100);
		}
		str = "\n";
		HAL_UART_Transmit(&huart2, (uint8_t *)str.c_str(),str.length(),100);

		printf("Accel[m/s^2]: %+4.4f %+4.4f %+4.4f\n", accel_data[0], accel_data[1], accel_data[2]);
		printf("Gyro[deg/s]:  %+4.4f %+4.4f %+4.4f\n", gyro_data[0], gyro_data[1], gyro_data[2]);
	}
}

//データを受信したら呼び出される
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {

    //データがSBUSの形式であるか確認
	if(ReceiveBuffer[0] == 0x0F && ReceiveBuffer[24] == 0x00){

		SBUS_decode();//SUBSデータの分解
    }

    //受信の再開
    HAL_UART_Receive_DMA(&huart2, ReceiveBuffer, 25);
}

//SBUSからデータを取り出すプログラム
void SBUS_decode(){

    SBUSData[0]  = (ReceiveBuffer[1]        | ReceiveBuffer[2] << 8)   & 0x07FF;
    SBUSData[1]  = (ReceiveBuffer[2] >> 3   | ReceiveBuffer[3] << 5)   & 0x07FF;
    SBUSData[2]  = (ReceiveBuffer[3] >> 6   | ReceiveBuffer[4] << 2    | ReceiveBuffer[5] << 10) & 0x07FF;
    SBUSData[3]  = (ReceiveBuffer[5] >> 1   | ReceiveBuffer[6] << 7)   & 0x07FF;
    SBUSData[4]  = (ReceiveBuffer[6] >> 4   | ReceiveBuffer[7] << 4)   & 0x07FF;
    SBUSData[5]  = (ReceiveBuffer[7] >> 7   | ReceiveBuffer[8] << 1    | ReceiveBuffer[9] << 9) & 0x07FF;
    SBUSData[6]  = (ReceiveBuffer[9] >> 2   | ReceiveBuffer[10] << 6)  & 0x07FF;
    SBUSData[7]  = (ReceiveBuffer[10] >> 5  | ReceiveBuffer[11] << 3)  & 0x07FF;
    SBUSData[8]  = (ReceiveBuffer[12]       | ReceiveBuffer[13] << 8)  & 0x07FF;
    SBUSData[9]  = (ReceiveBuffer[13] >> 3  | ReceiveBuffer[14] << 5)  & 0x07FF;
}
