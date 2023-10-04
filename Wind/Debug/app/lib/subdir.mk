################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../app/lib/CNTL_2P2Z.c \
../app/lib/EC200_JSON_parsing.c \
../app/lib/EC200_application.c \
../app/lib/EC200_uart.c 

OBJS += \
./app/lib/CNTL_2P2Z.o \
./app/lib/EC200_JSON_parsing.o \
./app/lib/EC200_application.o \
./app/lib/EC200_uart.o 

C_DEPS += \
./app/lib/CNTL_2P2Z.d \
./app/lib/EC200_JSON_parsing.d \
./app/lib/EC200_application.d \
./app/lib/EC200_uart.d 


# Each subdirectory must supply rules for building sources it contributes
app/lib/%.o app/lib/%.su app/lib/%.cyclo: ../app/lib/%.c app/lib/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -DUSE_FULL_LL_DRIVER '-DPI=3.14' -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"D:/7.GITHUB/WIND_G431/Wind" -I"D:/7.GITHUB/WIND_G431/Wind/app/porting" -I"D:/7.GITHUB/WIND_G431/Wind/app" -I"D:/7.GITHUB/WIND_G431/Wind/app/src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-app-2f-lib

clean-app-2f-lib:
	-$(RM) ./app/lib/CNTL_2P2Z.cyclo ./app/lib/CNTL_2P2Z.d ./app/lib/CNTL_2P2Z.o ./app/lib/CNTL_2P2Z.su ./app/lib/EC200_JSON_parsing.cyclo ./app/lib/EC200_JSON_parsing.d ./app/lib/EC200_JSON_parsing.o ./app/lib/EC200_JSON_parsing.su ./app/lib/EC200_application.cyclo ./app/lib/EC200_application.d ./app/lib/EC200_application.o ./app/lib/EC200_application.su ./app/lib/EC200_uart.cyclo ./app/lib/EC200_uart.d ./app/lib/EC200_uart.o ./app/lib/EC200_uart.su

.PHONY: clean-app-2f-lib

