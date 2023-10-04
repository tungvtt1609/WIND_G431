/*
 * gpio.c
 *
 *  Created on: Mar 8, 2023
 *      Author: Thanh Tung
 */

#include "gpio.h"

uint32_t pin_isr = GPIO_PIN_0;

static gpio_pin s_gpio_pin[PIN_MAX] = {
		{PIN_0, GPIO_PIN_0},
		{PIN_1, GPIO_PIN_1},
		{PIN_2, GPIO_PIN_2},
		{PIN_3, GPIO_PIN_3},
		{PIN_4, GPIO_PIN_4},
		{PIN_5, GPIO_PIN_5},
		{PIN_6, GPIO_PIN_6},
		{PIN_7, GPIO_PIN_7},
		{PIN_8, GPIO_PIN_8},
		{PIN_9, GPIO_PIN_9},
		{PIN_10, GPIO_PIN_10},
		{PIN_11, GPIO_PIN_11},
		{PIN_12, GPIO_PIN_12},
		{PIN_13, GPIO_PIN_13},
		{PIN_14, GPIO_PIN_14},
		{PIN_15, GPIO_PIN_15},
};

static gpio_port s_gpio_port[PORT_MAX] = {
		{PORT_A, GPIOA},
		{PORT_B, GPIOB},
		{PORT_C, GPIOC},
		{PORT_D, GPIOD},
		{PORT_E, GPIOE},
		{PORT_F, GPIOF},
		{PORT_G, GPIOG},
};

void GPIO_SetPinOut(gpio_port_id port_id, gpio_pin_id pin_id){
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;   /*!< Specifies the operating mode for the selected pins.
                           	   	   	   	   	   	   	   This parameter can be a value of @ref GPIO_mode */
	GPIO_InitStruct.Pin = s_gpio_pin[pin_id].pin; /*!< Specifies the GPIO pins to be configured.
                           	   	   	   	   	   	   	   This parameter can be any value of @ref GPIO_pins */
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; /*!< Specifies the speed for the selected pins.
                           	   	   	   	   	   	   	   This parameter can be a value of @ref GPIO_speed */
	HAL_GPIO_Init(s_gpio_port[port_id].port, &GPIO_InitStruct);
	return;
}

void GPIO_SetPinIn(gpio_port_id port_id, gpio_pin_id pin_id, uint32_t PULLUPDOWN){
	GPIO_InitTypeDef  GPIO_InitStruct;
	GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;      	/*!< Input Floating Mode                   	*/
	GPIO_InitStruct.Pull  = PULLUPDOWN;				/*!< Input state pull-up or pull-down		*/
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;	/*!< Specifies the speed for the selected pins.
                           	   	   	   	   	   	   	   This parameter can be a value of @ref GPIO_speed */
	GPIO_InitStruct.Pin = s_gpio_pin[pin_id].pin;
	HAL_GPIO_Init(s_gpio_port[port_id].port, &GPIO_InitStruct);
	return;
}

void GPIO_WritePin(gpio_port_id port_id, gpio_pin_id pin_id, gpio_state state){
	HAL_GPIO_WritePin(s_gpio_port[port_id].port, s_gpio_pin[pin_id].pin, state);
}

gpio_state GPIO_ReadPin(gpio_port_id port_id, gpio_pin_id pin_id){
	return HAL_GPIO_ReadPin(s_gpio_port[port_id].port, s_gpio_pin[pin_id].pin);
}

void GPIO_TogglePin(gpio_port_id port_id, gpio_pin_id pin_id){
	HAL_GPIO_TogglePin(s_gpio_port[port_id].port, s_gpio_pin[pin_id].pin);
}

//void EXTI15_10_IRQHandler_Config(){}
