################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../app/src/Frequency.c \
../app/src/app_main.c \
../app/src/control_dcdc.c \
../app/src/control_dumpload.c \
../app/src/control_xylanh.c \
../app/src/error_check.c \
../app/src/interface.c \
../app/src/state_machine.c \
../app/src/wind_control.c \
../app/src/wind_mppt.c 

OBJS += \
./app/src/Frequency.o \
./app/src/app_main.o \
./app/src/control_dcdc.o \
./app/src/control_dumpload.o \
./app/src/control_xylanh.o \
./app/src/error_check.o \
./app/src/interface.o \
./app/src/state_machine.o \
./app/src/wind_control.o \
./app/src/wind_mppt.o 

C_DEPS += \
./app/src/Frequency.d \
./app/src/app_main.d \
./app/src/control_dcdc.d \
./app/src/control_dumpload.d \
./app/src/control_xylanh.d \
./app/src/error_check.d \
./app/src/interface.d \
./app/src/state_machine.d \
./app/src/wind_control.d \
./app/src/wind_mppt.d 


# Each subdirectory must supply rules for building sources it contributes
app/src/%.o app/src/%.su app/src/%.cyclo: ../app/src/%.c app/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -DUSE_FULL_LL_DRIVER '-DPI=3.14' -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"G:/Duchm20/1.Wind_Turbine_Mini/3.Source/Wind_Turbine_1kW/wind_turbine_1kw/Wind" -I"G:/Duchm20/1.Wind_Turbine_Mini/3.Source/Wind_Turbine_1kW/wind_turbine_1kw/Wind/app/porting" -I"G:/Duchm20/1.Wind_Turbine_Mini/3.Source/Wind_Turbine_1kW/wind_turbine_1kw/Wind/app" -I"G:/Duchm20/1.Wind_Turbine_Mini/3.Source/Wind_Turbine_1kW/wind_turbine_1kw/Wind/app/src" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-app-2f-src

clean-app-2f-src:
	-$(RM) ./app/src/Frequency.cyclo ./app/src/Frequency.d ./app/src/Frequency.o ./app/src/Frequency.su ./app/src/app_main.cyclo ./app/src/app_main.d ./app/src/app_main.o ./app/src/app_main.su ./app/src/control_dcdc.cyclo ./app/src/control_dcdc.d ./app/src/control_dcdc.o ./app/src/control_dcdc.su ./app/src/control_dumpload.cyclo ./app/src/control_dumpload.d ./app/src/control_dumpload.o ./app/src/control_dumpload.su ./app/src/control_xylanh.cyclo ./app/src/control_xylanh.d ./app/src/control_xylanh.o ./app/src/control_xylanh.su ./app/src/error_check.cyclo ./app/src/error_check.d ./app/src/error_check.o ./app/src/error_check.su ./app/src/interface.cyclo ./app/src/interface.d ./app/src/interface.o ./app/src/interface.su ./app/src/state_machine.cyclo ./app/src/state_machine.d ./app/src/state_machine.o ./app/src/state_machine.su ./app/src/wind_control.cyclo ./app/src/wind_control.d ./app/src/wind_control.o ./app/src/wind_control.su ./app/src/wind_mppt.cyclo ./app/src/wind_mppt.d ./app/src/wind_mppt.o ./app/src/wind_mppt.su

.PHONY: clean-app-2f-src

