################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../app/driver/com/can.c \
../app/driver/com/modbus.c 

OBJS += \
./app/driver/com/can.o \
./app/driver/com/modbus.o 

C_DEPS += \
./app/driver/com/can.d \
./app/driver/com/modbus.d 


# Each subdirectory must supply rules for building sources it contributes
app/driver/com/%.o app/driver/com/%.su app/driver/com/%.cyclo: ../app/driver/com/%.c app/driver/com/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -DUSE_FULL_LL_DRIVER '-DPI=3.14' -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"D:/2.WORK/VMC/Wind turbine/wind_g431/Wind" -I"D:/2.WORK/VMC/Wind turbine/wind_g431/Wind/app/porting" -I"D:/2.WORK/VMC/Wind turbine/wind_g431/Wind/app" -I"D:/2.WORK/VMC/Wind turbine/wind_g431/Wind/app/src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-app-2f-driver-2f-com

clean-app-2f-driver-2f-com:
	-$(RM) ./app/driver/com/can.cyclo ./app/driver/com/can.d ./app/driver/com/can.o ./app/driver/com/can.su ./app/driver/com/modbus.cyclo ./app/driver/com/modbus.d ./app/driver/com/modbus.o ./app/driver/com/modbus.su

.PHONY: clean-app-2f-driver-2f-com

