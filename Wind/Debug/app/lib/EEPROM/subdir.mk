################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../app/lib/EEPROM/EEROM24C256.c \
../app/lib/EEPROM/SoftI2C.c 

OBJS += \
./app/lib/EEPROM/EEROM24C256.o \
./app/lib/EEPROM/SoftI2C.o 

C_DEPS += \
./app/lib/EEPROM/EEROM24C256.d \
./app/lib/EEPROM/SoftI2C.d 


# Each subdirectory must supply rules for building sources it contributes
app/lib/EEPROM/%.o app/lib/EEPROM/%.su app/lib/EEPROM/%.cyclo: ../app/lib/EEPROM/%.c app/lib/EEPROM/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -DUSE_FULL_LL_DRIVER '-DPI=3.14' -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"G:/Duchm20/1.Wind_Turbine_Mini/3.Source/Wind_Turbine_1kW/wind_turbine_1kw/Wind" -I"G:/Duchm20/1.Wind_Turbine_Mini/3.Source/Wind_Turbine_1kW/wind_turbine_1kw/Wind/app/porting" -I"G:/Duchm20/1.Wind_Turbine_Mini/3.Source/Wind_Turbine_1kW/wind_turbine_1kw/Wind/app" -I"G:/Duchm20/1.Wind_Turbine_Mini/3.Source/Wind_Turbine_1kW/wind_turbine_1kw/Wind/app/src" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-app-2f-lib-2f-EEPROM

clean-app-2f-lib-2f-EEPROM:
	-$(RM) ./app/lib/EEPROM/EEROM24C256.cyclo ./app/lib/EEPROM/EEROM24C256.d ./app/lib/EEPROM/EEROM24C256.o ./app/lib/EEPROM/EEROM24C256.su ./app/lib/EEPROM/SoftI2C.cyclo ./app/lib/EEPROM/SoftI2C.d ./app/lib/EEPROM/SoftI2C.o ./app/lib/EEPROM/SoftI2C.su

.PHONY: clean-app-2f-lib-2f-EEPROM

