/*
 * pin_func.c
 *
 *  Created on: Apr 12, 2023
 *      Author: TungVT
 */

#include "driver/control/pin_func.h"

pin_func_t pin_func_list[PIN_FUNC_MAX] = {
		{LED1	, 				PORT_C, 		PIN_13, 		OUTPUT, 		GPIO_HIGH		},
		{LED2	, 				PORT_C, 		PIN_14, 		OUTPUT, 		GPIO_HIGH		},
		{LED3	, 				PORT_C, 		PIN_15, 		OUTPUT, 		GPIO_HIGH		},
		{BUZZ	, 				PORT_B, 		PIN_7, 			OUTPUT, 		GPIO_LOW		},
		{CONTROL_485, 			PORT_B, 		PIN_12, 		OUTPUT, 		GPIO_HIGH		},
		{RELAY1, 				PORT_B, 		PIN_1, 			OUTPUT, 		GPIO_LOW		},
		{RELAY2, 				PORT_B, 		PIN_2, 			OUTPUT, 		GPIO_LOW		},
		{RESET_4G, 				PORT_B, 		PIN_13, 		OUTPUT, 		GPIO_LOW		},
		{POWER_4G, 				PORT_B, 		PIN_9, 			OUTPUT, 		GPIO_LOW		},
};

void Pin_Func_Init(void)
{
    pin_func_id_e id = 0;
    for (id = 0; id < PIN_FUNC_MAX; id++)
    {
        if (pin_func_list[id].port_id != PORT_MAX)
        {
            if (pin_func_list[id].dir == OUTPUT)
            {
                GPIO_SetPinOut(pin_func_list[id].port_id, pin_func_list[id].pin_id);
                GPIO_WritePin(pin_func_list[id].port_id, pin_func_list[id].pin_id, pin_func_list[id].default_level);
            }
            else if (pin_func_list[id].dir == INPUT_PULLDOWN)
            {
                GPIO_SetPinIn(pin_func_list[id].port_id, pin_func_list[id].pin_id, PULL_DOWN);
            }
            else
                GPIO_SetPinIn(pin_func_list[id].port_id, pin_func_list[id].pin_id, PULL_UP);
        }
    }
}

void Pin_Func_TurnOn(pin_func_id_e id)
{
    if (pin_func_list[id].port_id < PORT_MAX)
    {
        GPIO_WritePin(pin_func_list[id].port_id, pin_func_list[id].pin_id, GPIO_HIGH);
    }
}

void Pin_Func_TurnOff(pin_func_id_e id)
{
    if (pin_func_list[id].port_id < PORT_MAX)
    {
        GPIO_WritePin(pin_func_list[id].port_id, pin_func_list[id].pin_id, GPIO_LOW);
    }
}

void Pin_Func_Toggle(pin_func_id_e id)
{
    if (pin_func_list[id].port_id < PORT_MAX)
    {
        GPIO_TogglePin(pin_func_list[id].port_id, pin_func_list[id].pin_id);
    }
}

uint8_t Pin_Func_Read(pin_func_id_e id)
{
    if (pin_func_list[id].port_id < PORT_MAX)
    {
        return GPIO_ReadPin(pin_func_list[id].port_id, pin_func_list[id].pin_id);

    }
    return 0;
}
