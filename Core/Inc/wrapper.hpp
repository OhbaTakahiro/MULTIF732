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
#include "ICM42688P_HAL_I2C.h"
extern AttitudeEKF_t* attitude_ekf;
extern float accel_data[3];
extern float gyro_data[3];
extern ICM42688P_HAL_I2C icm;

class MotorController;
class ServoController;

extern MotorController* motor1;
extern MotorController* motor2;
extern MotorController* motor3;
extern MotorController* motor4;
extern ServoController* servo1;
void stopAllMotors();
void updateServoFromSbus(uint16_t sbus_value);

#endif

#endif /* INC_WRAPPER_HPP_ */
