################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Src/ArmF.cpp \
../Core/Src/CalibF.cpp \
../Core/Src/DisF.cpp \
../Core/Src/FlyF.cpp \
../Core/Src/ICM42688P.cpp \
../Core/Src/ICM42688P_HAL_I2C.cpp \
../Core/Src/InitF.cpp \
../Core/Src/PID.cpp \
../Core/Src/PrearmF.cpp \
../Core/Src/StartF.cpp \
../Core/Src/attitude_ekf.cpp \
../Core/Src/ekf.cpp \
../Core/Src/matrix.cpp \
../Core/Src/motor_controller.cpp \
../Core/Src/pwm_controller.cpp \
../Core/Src/servo_controller.cpp \
../Core/Src/wrapper.cpp 

C_SRCS += \
../Core/Src/adc.c \
../Core/Src/gpio.c \
../Core/Src/i2c.c \
../Core/Src/main.c \
../Core/Src/spi.c \
../Core/Src/stm32f7xx_hal_msp.c \
../Core/Src/stm32f7xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f7xx.c \
../Core/Src/tim.c \
../Core/Src/usart.c 

C_DEPS += \
./Core/Src/adc.d \
./Core/Src/gpio.d \
./Core/Src/i2c.d \
./Core/Src/main.d \
./Core/Src/spi.d \
./Core/Src/stm32f7xx_hal_msp.d \
./Core/Src/stm32f7xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f7xx.d \
./Core/Src/tim.d \
./Core/Src/usart.d 

OBJS += \
./Core/Src/ArmF.o \
./Core/Src/CalibF.o \
./Core/Src/DisF.o \
./Core/Src/FlyF.o \
./Core/Src/ICM42688P.o \
./Core/Src/ICM42688P_HAL_I2C.o \
./Core/Src/InitF.o \
./Core/Src/PID.o \
./Core/Src/PrearmF.o \
./Core/Src/StartF.o \
./Core/Src/adc.o \
./Core/Src/attitude_ekf.o \
./Core/Src/ekf.o \
./Core/Src/gpio.o \
./Core/Src/i2c.o \
./Core/Src/main.o \
./Core/Src/matrix.o \
./Core/Src/motor_controller.o \
./Core/Src/pwm_controller.o \
./Core/Src/servo_controller.o \
./Core/Src/spi.o \
./Core/Src/stm32f7xx_hal_msp.o \
./Core/Src/stm32f7xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f7xx.o \
./Core/Src/tim.o \
./Core/Src/usart.o \
./Core/Src/wrapper.o 

CPP_DEPS += \
./Core/Src/ArmF.d \
./Core/Src/CalibF.d \
./Core/Src/DisF.d \
./Core/Src/FlyF.d \
./Core/Src/ICM42688P.d \
./Core/Src/ICM42688P_HAL_I2C.d \
./Core/Src/InitF.d \
./Core/Src/PID.d \
./Core/Src/PrearmF.d \
./Core/Src/StartF.d \
./Core/Src/attitude_ekf.d \
./Core/Src/ekf.d \
./Core/Src/matrix.d \
./Core/Src/motor_controller.d \
./Core/Src/pwm_controller.d \
./Core/Src/servo_controller.d \
./Core/Src/wrapper.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.cpp Core/Src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F732xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F732xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/ArmF.cyclo ./Core/Src/ArmF.d ./Core/Src/ArmF.o ./Core/Src/ArmF.su ./Core/Src/CalibF.cyclo ./Core/Src/CalibF.d ./Core/Src/CalibF.o ./Core/Src/CalibF.su ./Core/Src/DisF.cyclo ./Core/Src/DisF.d ./Core/Src/DisF.o ./Core/Src/DisF.su ./Core/Src/FlyF.cyclo ./Core/Src/FlyF.d ./Core/Src/FlyF.o ./Core/Src/FlyF.su ./Core/Src/ICM42688P.cyclo ./Core/Src/ICM42688P.d ./Core/Src/ICM42688P.o ./Core/Src/ICM42688P.su ./Core/Src/ICM42688P_HAL_I2C.cyclo ./Core/Src/ICM42688P_HAL_I2C.d ./Core/Src/ICM42688P_HAL_I2C.o ./Core/Src/ICM42688P_HAL_I2C.su ./Core/Src/InitF.cyclo ./Core/Src/InitF.d ./Core/Src/InitF.o ./Core/Src/InitF.su ./Core/Src/PID.cyclo ./Core/Src/PID.d ./Core/Src/PID.o ./Core/Src/PID.su ./Core/Src/PrearmF.cyclo ./Core/Src/PrearmF.d ./Core/Src/PrearmF.o ./Core/Src/PrearmF.su ./Core/Src/StartF.cyclo ./Core/Src/StartF.d ./Core/Src/StartF.o ./Core/Src/StartF.su ./Core/Src/adc.cyclo ./Core/Src/adc.d ./Core/Src/adc.o ./Core/Src/adc.su ./Core/Src/attitude_ekf.cyclo ./Core/Src/attitude_ekf.d ./Core/Src/attitude_ekf.o ./Core/Src/attitude_ekf.su ./Core/Src/ekf.cyclo ./Core/Src/ekf.d ./Core/Src/ekf.o ./Core/Src/ekf.su ./Core/Src/gpio.cyclo ./Core/Src/gpio.d ./Core/Src/gpio.o ./Core/Src/gpio.su ./Core/Src/i2c.cyclo ./Core/Src/i2c.d ./Core/Src/i2c.o ./Core/Src/i2c.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/matrix.cyclo ./Core/Src/matrix.d ./Core/Src/matrix.o ./Core/Src/matrix.su ./Core/Src/motor_controller.cyclo ./Core/Src/motor_controller.d ./Core/Src/motor_controller.o ./Core/Src/motor_controller.su ./Core/Src/pwm_controller.cyclo ./Core/Src/pwm_controller.d ./Core/Src/pwm_controller.o ./Core/Src/pwm_controller.su ./Core/Src/servo_controller.cyclo ./Core/Src/servo_controller.d ./Core/Src/servo_controller.o ./Core/Src/servo_controller.su ./Core/Src/spi.cyclo ./Core/Src/spi.d ./Core/Src/spi.o ./Core/Src/spi.su ./Core/Src/stm32f7xx_hal_msp.cyclo ./Core/Src/stm32f7xx_hal_msp.d ./Core/Src/stm32f7xx_hal_msp.o ./Core/Src/stm32f7xx_hal_msp.su ./Core/Src/stm32f7xx_it.cyclo ./Core/Src/stm32f7xx_it.d ./Core/Src/stm32f7xx_it.o ./Core/Src/stm32f7xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f7xx.cyclo ./Core/Src/system_stm32f7xx.d ./Core/Src/system_stm32f7xx.o ./Core/Src/system_stm32f7xx.su ./Core/Src/tim.cyclo ./Core/Src/tim.d ./Core/Src/tim.o ./Core/Src/tim.su ./Core/Src/usart.cyclo ./Core/Src/usart.d ./Core/Src/usart.o ./Core/Src/usart.su ./Core/Src/wrapper.cyclo ./Core/Src/wrapper.d ./Core/Src/wrapper.o ./Core/Src/wrapper.su

.PHONY: clean-Core-2f-Src

