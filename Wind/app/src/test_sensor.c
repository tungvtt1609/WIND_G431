/*
 * test_sensor.c
 *
 *  Created on: Oct 4, 2023
 *      Author: TungVT
 */

#include "test_sensor.h"
#include "driver/control/pin_func.h"

extern UART_HandleTypeDef huart3;

uint8_t read_wind_speed[8]  = {0x02, 0x03, 0x00, 0x00, 0x00, 0x01, 0x84, 0x39};
uint8_t read_air_level[8]	= {0x02, 0x03, 0x00, 0x01, 0x00, 0x01, 0xD5, 0x9F};

volatile uint32_t read_index;
volatile uint8_t request_size = sizeof(read_wind_speed);
uint8_t response_buffer[30];
//typedef struct dat_wind_speed {
//	float wind_speed;
//};
//typedef struct air_level {
//	float air_;
//};

float wind_speed = 0;
float air_ = 0;

void speed_info(void){
	HAL_UART_Transmit(&huart3, read_wind_speed, request_size, 10);
	HAL_UART_Receive(&huart3, response_buffer, sizeof(response_buffer), 10);

}


void response_process(uint8_t *response, uint8_t response_size){
	if(response_size >= 5){
		uint8_t device_addr = response[0];
		uint8_t	func_addr	= response[1];
		if(device_addr == 0x02 && func_addr == 0x03){
			read_index = response[2] << 8 | response[3];
		}
		else{
			Pin_Func_TurnOff(LED2);
		}
	}
}
