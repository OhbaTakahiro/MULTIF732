#pragma once
#include "ICM42688P_HAL_I2C.h"
#include "ICM42688P.h"

class ICM42688P_HAL_I2C;

enum class State{//単語に数字の意味を持たせるやつ
    Start = 1,
    Init,
    Calib,
	Prearm,
    Arm,
    Fly,
    Dis,
};

struct Context{
	//ICM42688P_HAL_I2C icm(&hi2c1, 0b1101001);
    int count;
};
