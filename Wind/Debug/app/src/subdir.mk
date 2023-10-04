################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../app/src/app_main.c \
../app/src/control_dcdc.c \
../app/src/control_dumpload.c \
../app/src/error_check.c \
../app/src/interface.c \
../app/src/state_machine.c \
../app/src/test_sensor.c \
../app/src/wind_control.c \
../app/src/wind_mppt.c 

OBJS += \
./app/src/app_main.o \
./app/src/control_dcdc.o \
./app/src/control_dumpload.o \
./app/src/error_check.o \
./app/src/interface.o \
./app/src/state_machine.o \
./app/src/test_sensor.o \
./app/src/wind_control.o \
./app/src/wind_mppt.o 

C_DEPS += \
./app/src/app_main.d \
./app/src/control_dcdc.d \
./app/src/control_dumpload.d \
./app/src/error_check.d \
./app/src/interface.d \
./app/src/state_machine.d \
./app/src/test_sensor.d \
./app/src/wind_control.d \
./app/src/wind_mppt.d 


# Each subdirectory must supply rules for building sources it contributes
app/src/%.o app/src/%.su app/src/%.cyclo: ../app/src/%.c app/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -DUSE_FULL_LL_DRIVER '-DPI=3.14' -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"D:/7.GITHUB/WIND_G431/Wind" -I"D:/7.GITHUB/WIND_G431/Wind/app/porting" -I"D:/7.GITHUB/WIND_G431/Wind/app" -I"D:/7.GITHUB/WIND_G431/Wind/app/src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-app-2f-src

clean-app-2f-src:
	-$(RM) ./app/src/app_main.cyclo ./app/src/app_main.d ./app/src/app_main.o ./app/src/app_main.su ./app/src/control_dcdc.cyclo ./app/src/control_dcdc.d ./app/src/control_dcdc.o ./app/src/control_dcdc.su ./app/src/control_dumpload.cyclo ./app/src/control_dumpload.d ./app/src/control_dumpload.o ./app/src/control_dumpload.su ./app/src/error_check.cyclo ./app/src/error_check.d ./app/src/error_check.o ./app/src/error_check.su ./app/src/interface.cyclo ./app/src/interface.d ./app/src/interface.o ./app/src/interface.su ./app/src/state_machine.cyclo ./app/src/state_machine.d ./app/src/state_machine.o ./app/src/state_machine.su ./app/src/test_sensor.cyclo ./app/src/test_sensor.d ./app/src/test_sensor.o ./app/src/test_sensor.su ./app/src/wind_control.cyclo ./app/src/wind_control.d ./app/src/wind_control.o ./app/src/wind_control.su ./app/src/wind_mppt.cyclo ./app/src/wind_mppt.d ./app/src/wind_mppt.o ./app/src/wind_mppt.su

.PHONY: clean-app-2f-src

