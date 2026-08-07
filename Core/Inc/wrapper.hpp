#ifndef INC_WRAPPER_HPP_
#define INC_WRAPPER_HPP_

#ifdef __cplusplus
extern "C" {
#endif

void init(void);
void loop(void);

#ifdef __cplusplus
};

// これより4行はGeminiにやってもらった
#include "attitude_ekf.h"
extern AttitudeEKF_t* attitude_ekf;
extern float accel_data[3];
extern float gyro_data[3];

class MotorController;

extern MotorController* motor1;
extern MotorController* motor2;
extern MotorController* motor3;
extern MotorController* motor4;

#endif

#endif /* INC_WRAPPER_HPP_ */
