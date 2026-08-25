#include "state_type.hpp"
#include "wrapper.hpp"
#include <stdio.h>

void calibf(State* current_state, Context* context){
    context->count++;
    // 機体を水平かつ静止させた状態で実行する。
    if (icm.Calibration(200) != 0) {
        printf("IMU calibration failed\n");
        *current_state = State::Dis;
        return;
    }
    printf("IMU calibration complete\n");
    *current_state = State::Prearm;
}
