/*
 * gpio.h
 *
 *  Created on: Mar 8, 2023
 *      Author: Thanh Tung
 */

#ifndef PORTING_GPIO_GPIO_H_
#define PORTING_GPIO_GPIO_H_

#include "types.h"
#include "stm32g4xx_hal.h"

typedef enum gpio_pin_id{
	PIN_0 = 0,
	PIN_1,
	PIN_2,
	PIN_3,
	PIN_4,
	PIN_5,
	PIN_6,
	PIN_7,
	PIN_8,
	PIN_9,
	PIN_10,
	PIN_11,
	PIN_12,
	PIN_13,
	PIN_14,
	PIN_15,
	PIN_MAX
}gpio_pin_id;

typedef enum gpio_port_id{
	PORT_A = 0,
	PORT_B,
	PORT_C,
	PORT_D,
	PORT_E,
	PORT_F,
	PORT_G,
	PORT_MAX
}gpio_port_id;

typedef enum gpio_state{
	GPIO_LOW = 0,
	GPIO_HIGH
}gpio_state;

#define PULL_UP   	GPIO_PULLUP
#define PULL_DOWN 	GPIO_PULLDOWN
#define NO_PULL 		GPIO_NOPULL

typedef struct gpio_pin
{
	gpio_pin_id pin_id;
	uint32_t pin;
}gpio_pin;

typedef struct gpio_port
{
	gpio_port_id port_id;
	GPIO_TypeDef *port;
}gpio_port;

extern uint32_t pin_isr;

void GPIO_SetPinOut(gpio_port_id port_id, gpio_pin_id pin_id);
void GPIO_SetPinIn(gpio_port_id port_id, gpio_pin_id pin_id, uint32_t PULLUPDOWN);
void GPIO_WritePin(gpio_port_id port_id, gpio_pin_id pin_id, gpio_state state);
gpio_state GPIO_ReadPin(gpio_port_id port_id, gpio_pin_id pin_id);
void GPIO_TogglePin(gpio_port_id port_id, gpio_pin_id pin_id);
//void EXTI15_10_IRQHandler_Config(gpio_port_id port_id, gpio_pin_id pin_id, uint32_t mode, uint32_t PULLUPDOWN);


#endif /* PORTING_GPIO_GPIO_H_ */
