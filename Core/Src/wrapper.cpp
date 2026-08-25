#include "wrapper.hpp"
#include "main.h"
#include "state_type.hpp"
#include "usart.h" // UARTの設定を読み込む
#include "stdio.h"
// 使用したい通信プロトコルのヘッダーファイルをインクルード
#include "ICM42688P_HAL_I2C.h"

#include "FuncList.hpp"

#include "math.h"
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
volatile uint16_t SBUSData[10] = {};
volatile uint32_t last_sbus_tick = 0;
volatile bool sbus_failsafe = true;

ICM42688P_HAL_I2C icm(&hi2c1, 0b1101001);

int sbusdata3ch, sbusdata9ch, sbusdata1ch, sbusdata4ch, sbusdata2ch, sbusdata5ch;

void SBUS_decode();

// --- マイクロ秒(μs)タイマー機能 ---
void DWT_Init(void) {
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
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
	HAL_UART_Receive_DMA(&huart5, ReceiveBuffer, 25);
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
	if (icm.AccelConfig(icm.ACCEL_Mode::LowNoize, icm.ACCEL_SCALE::SCALE02g,
                        icm.ACCEL_ODR::ODR01000hz, icm.ACCEL_DLPF::ODR40) != 0 ||
	    icm.GyroConfig(icm.GYRO_MODE::LowNoize, icm.GYRO_SCALE::Dps0250,
	                   icm.GYRO_ODR::ODR01000hz, icm.GYRO_DLPF::ODR40) != 0) {
		printf("ICM42688p configuration failed\n");
		while (1) { }
	}
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

	uint16_t sbus_snapshot[10];
	const uint32_t primask = __get_PRIMASK();
	__disable_irq();
	for (uint8_t i = 0; i < 10; ++i) {
		sbus_snapshot[i] = SBUSData[i];
	}
	const bool sbus_ok = !sbus_failsafe && (HAL_GetTick() - last_sbus_tick <= 100U);
	if (primask == 0U) {
		__enable_irq();
	}

	sbusdata9ch = sbus_ok ? sbus_snapshot[8] : 0;
	sbusdata3ch = sbus_snapshot[2];
	sbusdata1ch = sbus_snapshot[0];
	sbusdata4ch = sbus_snapshot[3];
	sbusdata2ch = sbus_snapshot[1];
	sbusdata5ch = sbus_ok ? sbus_snapshot[4] : 500;
	updateServoFromSbus(static_cast<uint16_t>(sbusdata5ch));

	// センサー値は EKF/PID より先に取得する。通信失敗時は飛行を継続しない。
	if (icm.GetData(accel_data, gyro_data) != 0) {
		if (current_state == State::Fly) {
			stopAllMotors();
			current_state = State::Dis;
		}
		return;
	}

	if (!sbus_ok && current_state == State::Fly) {
		stopAllMotors();
		current_state = State::Dis;
	}

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

}

//データを受信したら呼び出される
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance != UART5) {
		return;
	}

    //データがSBUSの形式であるか確認
	if(ReceiveBuffer[0] == 0x0F && ReceiveBuffer[24] == 0x00){
		// bit2: frame lost, bit3: failsafe。異常フレームはただちに安全側へ。
		sbus_failsafe = (ReceiveBuffer[23] & 0x0CU) != 0U;
		if (!sbus_failsafe) {
			SBUS_decode();
			last_sbus_tick = HAL_GetTick();
		}
    }

    //受信の再開
    HAL_UART_Receive_DMA(&huart5, ReceiveBuffer, 25);
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
