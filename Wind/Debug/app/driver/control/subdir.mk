################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../app/driver/control/driver_pwm_dc_dc.c \
../app/driver/control/driver_pwm_dumpload.c \
../app/driver/control/pin_func.c 

OBJS += \
./app/driver/control/driver_pwm_dc_dc.o \
./app/driver/control/driver_pwm_dumpload.o \
./app/driver/control/pin_func.o 

C_DEPS += \
./app/driver/control/driver_pwm_dc_dc.d \
./app/driver/control/driver_pwm_dumpload.d \
./app/driver/control/pin_func.d 


# Each subdirectory must supply rules for building sources it contributes
app/driver/control/%.o app/driver/control/%.su app/driver/control/%.cyclo: ../app/driver/control/%.c app/driver/control/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -DUSE_FULL_LL_DRIVER '-DPI=3.14' -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"D:/2.WORK/VMC/Wind turbine/wind_g431/Wind" -I"D:/2.WORK/VMC/Wind turbine/wind_g431/Wind/app/porting" -I"D:/2.WORK/VMC/Wind turbine/wind_g431/Wind/app" -I"D:/2.WORK/VMC/Wind turbine/wind_g431/Wind/app/src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-app-2f-driver-2f-control

clean-app-2f-driver-2f-control:
	-$(RM) ./app/driver/control/driver_pwm_dc_dc.cyclo ./app/driver/control/driver_pwm_dc_dc.d ./app/driver/control/driver_pwm_dc_dc.o ./app/driver/control/driver_pwm_dc_dc.su ./app/driver/control/driver_pwm_dumpload.cyclo ./app/driver/control/driver_pwm_dumpload.d ./app/driver/control/driver_pwm_dumpload.o ./app/driver/control/driver_pwm_dumpload.su ./app/driver/control/pin_func.cyclo ./app/driver/control/pin_func.d ./app/driver/control/pin_func.o ./app/driver/control/pin_func.su

.PHONY: clean-app-2f-driver-2f-control

