################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../app/lib/CNTL_2P2Z.c \
../app/lib/EC200_mqtt_application.c 

OBJS += \
./app/lib/CNTL_2P2Z.o \
./app/lib/EC200_mqtt_application.o 

C_DEPS += \
./app/lib/CNTL_2P2Z.d \
./app/lib/EC200_mqtt_application.d 


# Each subdirectory must supply rules for building sources it contributes
app/lib/%.o app/lib/%.su app/lib/%.cyclo: ../app/lib/%.c app/lib/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -DUSE_FULL_LL_DRIVER '-DPI=3.14' -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"G:/Duchm20/1.Wind_Turbine_Mini/3.Source/Wind_Turbine_1kW/wind_turbine_1kw/Wind" -I"G:/Duchm20/1.Wind_Turbine_Mini/3.Source/Wind_Turbine_1kW/wind_turbine_1kw/Wind/app/porting" -I"G:/Duchm20/1.Wind_Turbine_Mini/3.Source/Wind_Turbine_1kW/wind_turbine_1kw/Wind/app" -I"G:/Duchm20/1.Wind_Turbine_Mini/3.Source/Wind_Turbine_1kW/wind_turbine_1kw/Wind/app/src" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-app-2f-lib

clean-app-2f-lib:
	-$(RM) ./app/lib/CNTL_2P2Z.cyclo ./app/lib/CNTL_2P2Z.d ./app/lib/CNTL_2P2Z.o ./app/lib/CNTL_2P2Z.su ./app/lib/EC200_mqtt_application.cyclo ./app/lib/EC200_mqtt_application.d ./app/lib/EC200_mqtt_application.o ./app/lib/EC200_mqtt_application.su

.PHONY: clean-app-2f-lib

