################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../app/porting/serial/serial.c 

OBJS += \
./app/porting/serial/serial.o 

C_DEPS += \
./app/porting/serial/serial.d 


# Each subdirectory must supply rules for building sources it contributes
app/porting/serial/%.o app/porting/serial/%.su app/porting/serial/%.cyclo: ../app/porting/serial/%.c app/porting/serial/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -DUSE_FULL_LL_DRIVER '-DPI=3.14' -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"D:/2.WORK/VMC/Wind turbine/wind_g431/Wind" -I"D:/2.WORK/VMC/Wind turbine/wind_g431/Wind/app/porting" -I"D:/2.WORK/VMC/Wind turbine/wind_g431/Wind/app" -I"D:/2.WORK/VMC/Wind turbine/wind_g431/Wind/app/src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-app-2f-porting-2f-serial

clean-app-2f-porting-2f-serial:
	-$(RM) ./app/porting/serial/serial.cyclo ./app/porting/serial/serial.d ./app/porting/serial/serial.o ./app/porting/serial/serial.su

.PHONY: clean-app-2f-porting-2f-serial

