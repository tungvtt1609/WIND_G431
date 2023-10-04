################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../app/porting/timer/timer.c \
../app/porting/timer/timer_pwm.c 

OBJS += \
./app/porting/timer/timer.o \
./app/porting/timer/timer_pwm.o 

C_DEPS += \
./app/porting/timer/timer.d \
./app/porting/timer/timer_pwm.d 


# Each subdirectory must supply rules for building sources it contributes
app/porting/timer/%.o app/porting/timer/%.su app/porting/timer/%.cyclo: ../app/porting/timer/%.c app/porting/timer/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -DUSE_FULL_LL_DRIVER '-DPI=3.14' -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"D:/2.WORK/VMC/Wind turbine/wind_g431/Wind" -I"D:/2.WORK/VMC/Wind turbine/wind_g431/Wind/app/porting" -I"D:/2.WORK/VMC/Wind turbine/wind_g431/Wind/app" -I"D:/2.WORK/VMC/Wind turbine/wind_g431/Wind/app/src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-app-2f-porting-2f-timer

clean-app-2f-porting-2f-timer:
	-$(RM) ./app/porting/timer/timer.cyclo ./app/porting/timer/timer.d ./app/porting/timer/timer.o ./app/porting/timer/timer.su ./app/porting/timer/timer_pwm.cyclo ./app/porting/timer/timer_pwm.d ./app/porting/timer/timer_pwm.o ./app/porting/timer/timer_pwm.su

.PHONY: clean-app-2f-porting-2f-timer

