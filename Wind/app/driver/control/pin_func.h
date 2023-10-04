/*
 * pin_func.h
 *
 *  Created on: Apr 12, 2023
 *      Author: TungVT
 */

#ifndef DRIVER_CONTROL_PIN_FUNC_H_
#define DRIVER_CONTROL_PIN_FUNC_H_

#include "gpio/gpio.h"

typedef enum pin_func_id_e
{
    LED1,
	LED2,
	LED3,
	BUZZ,
	CONTROL_485,
    PIN_FUNC_MAX
} pin_func_id_e;

typedef enum pin_dir_e
{
    OUTPUT,
    INPUT_PULLUP,
    INPUT_PULLDOWN
} pin_dir_e;

typedef struct pin_func_t
{
    pin_func_id_e pin_func_id;
    gpio_port_id port_id;
    gpio_pin_id pin_id;
    pin_dir_e dir;
    gpio_state default_level;

    /* data */
} pin_func_t;

void Pin_Func_Init(void);
void Pin_Func_TurnOn(pin_func_id_e id);
void Pin_Func_TurnOff(pin_func_id_e id);
void Pin_Func_Toggle(pin_func_id_e id);
uint8_t Pin_Func_Read(pin_func_id_e id);

#endif /* DRIVER_CONTROL_PIN_FUNC_H_ */
