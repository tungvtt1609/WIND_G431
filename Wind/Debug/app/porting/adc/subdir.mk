################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../app/porting/adc/adc.c 

OBJS += \
./app/porting/adc/adc.o 

C_DEPS += \
./app/porting/adc/adc.d 


# Each subdirectory must supply rules for building sources it contributes
app/porting/adc/%.o app/porting/adc/%.su app/porting/adc/%.cyclo: ../app/porting/adc/%.c app/porting/adc/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -DUSE_FULL_LL_DRIVER '-DPI=3.14' -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"D:/7.GITHUB/WIND_G431/Wind" -I"D:/7.GITHUB/WIND_G431/Wind/app/porting" -I"D:/7.GITHUB/WIND_G431/Wind/app" -I"D:/7.GITHUB/WIND_G431/Wind/app/src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-app-2f-porting-2f-adc

clean-app-2f-porting-2f-adc:
	-$(RM) ./app/porting/adc/adc.cyclo ./app/porting/adc/adc.d ./app/porting/adc/adc.o ./app/porting/adc/adc.su

.PHONY: clean-app-2f-porting-2f-adc

