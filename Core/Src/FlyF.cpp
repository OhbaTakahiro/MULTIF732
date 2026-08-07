#include <bits/stdc++.h>
using namespace std;
#include "state_type.hpp"
#include "wrapper.hpp"
#include "stdio.h"
#include "math.h"
#include <cstring>

// EKF library
#include "attitude_ekf.h"

// PID library
#include "PID.h"
#include "motor_controller.hpp"
#include<cstdio>

// EKF instance
//AttitudeEKF_t* attitude_ekf = nullptr;

// センサーデータ
//float accel_data[3] = {0, 0, 0};  // m/s^2
//float gyro_data[3] = {0, 0, 0};   // deg/s

// タイミング制御
static uint32_t last_update_tick = 0;
static const uint32_t UPDATE_PERIOD_MS = 10;  // 10ms (100Hz)

MotorController* motor1 = nullptr;
MotorController* motor2 = nullptr;
MotorController* motor3 = nullptr;
MotorController* motor4 = nullptr;

void flyf(State* current_state, Context* context, int sbusdata9ch, int sbusdata3ch, int sbusdata1ch, int sbusdata4ch, int sbusdata2ch){
    context->count++;
    printf("Fly %d\n",context->count);
    // 10ms周期で実行
    uint32_t current_tick = HAL_GetTick();
    uint32_t elapsed = current_tick - last_update_tick;

    if(elapsed < UPDATE_PERIOD_MS){

    	return;
    }

    last_update_tick += UPDATE_PERIOD_MS;
    if(current_tick - last_update_tick > UPDATE_PERIOD_MS){

    	last_update_tick = current_tick;
    }

    // ----- センサーデータ取得 -----
    // 使うセンサーのライブラリに合わせてください
    // float accel_data[3] 加速度[m/s^2]
    // float gyro_data[3] 角速度[dgree/s]


    // ----- EKF更新 -----
    float roll_deg = 0.0f;
    float pitch_deg = 0.0f;
    float yaw_deg = 0.0f;
    if(attitude_ekf != nullptr){

    	// ジャイロをdeg/s -> rad/sに変換
    	float gyro_rad[3];
    	gyro_rad[0] = gyro_data[0] * M_PI / 180.0f;
    	gyro_rad[1] = gyro_data[1] * M_PI / 180.0f;
    	gyro_rad[2] = gyro_data[2] * M_PI / 180.0f;

    	// EKF更新
    	if(AttitudeEKF_Update(attitude_ekf, accel_data, gyro_rad)){

    		// オイラー角取得
    		float roll = AttitudeEKF_GetRoll(attitude_ekf);
    		float pitch = AttitudeEKF_GetPitch(attitude_ekf);
    		float yaw = AttitudeEKF_GetYaw(attitude_ekf);

    		// 結果表示（rad -> deg変換）
    		roll_deg = roll * 180.0f / M_PI;
    		pitch_deg = pitch * 180.0f / M_PI;
    		yaw_deg = yaw * 180.0f / M_PI;
   			printf("Roll: %+7.2f  Pitch: %+7.2f  Yaw: %+7.2f [deg]\n", roll_deg, pitch_deg, yaw_deg);
   		}
   	}
    //ここからPID
    //目標値の変換（中央1024を0度とし、傾きを最大30度にする計算）
    float roll_target = (sbusdata3ch - 1024) * 0.05f;
    float pitch_target = (sbusdata1ch - 1024) * 0.05f;
    float yaw_target = (sbusdata4ch - 1024) * 0.05f;
    roll_pid.calc(roll_target, roll_deg);
    pitch_pid.calc(pitch_target, pitch_deg);
    yaw_pid.calc(yaw_target, yaw_deg);
    float roll_u = roll_pid.getData();
    float pitch_u = pitch_pid.getData();
    float yaw_u = yaw_pid.getData();

    //ここからモーターを動かそうと試みる
    // --- 1. 基本となるスロットル量（ベースの推力）をプロポから取得 ---
    // 例として 2ch がスロットルだとします。
    // sbusの範囲が 1024〜1696、あるいは 368〜1696 だとして、0% 〜 100% の範囲に変換します。
    // ここでは簡易的に、ベース推力を throttle (0.0f ~ 100.0f) と定義します。
    float throttle = (sbusdata2ch - 368) * (100.0f / 1328.0f);
    if (throttle < 0.0f) throttle = 0.0f;
    // --- 2. ミキシング計算（ベース推力にPIDの修正量を加減算する） ---
    // ※機体の向きやモーターの回転方向によってプラスマイナスは変わりますが、一般的なX型は以下の通りです
    float m1_speed = throttle + roll_u - pitch_u + yaw_u; // 左前
    float m2_speed = throttle - roll_u - pitch_u - yaw_u; // 右前
    float m3_speed = throttle - roll_u + pitch_u + yaw_u; // 右後
    float m4_speed = throttle + roll_u + pitch_u - yaw_u; // 左後
    // --- 3. 出力値の制限（0% 〜 100% の間に収めるガード処理） ---
    // これをしないと、setSpeedに120%などが渡ってしまいエラーになるか暴走します
    auto clamp = [](float& val) {
        if (val < 0.0f)   val = 0.0f;
        if (val > 100.0f) val = 100.0f;
    };
    clamp(m1_speed);
    clamp(m2_speed);
    clamp(m3_speed);
    clamp(m4_speed);

    // --- 4. モーターへ速度指令を送る ---
    motor1->setSpeed(m1_speed);
    motor2->setSpeed(m2_speed);
    motor3->setSpeed(m3_speed);
    motor4->setSpeed(m4_speed);

    if(sbusdata9ch < 1000){
    	throttle = 0.0f;
    	motor1->stop();
    	motor2->stop();
    	motor3->stop();
    	motor4->stop();
        *current_state = State::Dis;
    }
}
