#include <bits/stdc++.h>
using namespace std;
#include "state_type.hpp"
#include "wrapper.hpp"
#include <stdio.h>
#include "tim.h"
#include "motor_controller.hpp"

void initf(State* current_state, Context* context){
	// インスタンスの作成
	motor1 = new MotorController(&htim1, TIM_CHANNEL_1, MotorMode::NORMAL);
	motor2 = new MotorController(&htim1, TIM_CHANNEL_2, MotorMode::NORMAL);
	motor3 = new MotorController(&htim1, TIM_CHANNEL_3, MotorMode::NORMAL);
	motor4 = new MotorController(&htim1, TIM_CHANNEL_4, MotorMode::NORMAL);

	// 初期化の確認
	if (motor1->isInitialized()&&motor2->isInitialized()&&motor3->isInitialized()&&motor4->isInitialized()) {

	    printf("MotorController (NORMAL mode) initialized successfully\n");
	}
	else {

	    printf("MotorController initialization failed\n");
	}

	// パルス幅の範囲を設定（1000us ~ 2000us）
	motor1->setPulseRange(1000, 2000);
	motor2->setPulseRange(1000, 2000);
	motor3->setPulseRange(1000, 2000);
	motor4->setPulseRange(1000, 2000);
	printf("Pulse range set to 1000us ~ 2000us\n");

	// 0%に設定（初期値を送信）
	motor1->setSpeed(0.0f);
	motor2->setSpeed(0.0f);
	motor3->setSpeed(0.0f);
	motor4->setSpeed(0.0f);
	printf("Motor speed: 0 %%\n");
	HAL_Delay(10000);
    context->count++;
    printf("Init %d\n",context->count);
    *current_state = State::Calib;
}
