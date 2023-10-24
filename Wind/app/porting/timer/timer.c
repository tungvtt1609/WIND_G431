/*
 * timer.c
 *
 *  Created on: 16 Mar 2023
 *      Author: Thanh Tung
 */

#include "timer.h"
#include "stdbool.h"
#include "stddef.h"
#include "lib/EC200_mqtt_application.h"

#define MAX_TIMER_SERVICE					7



static TimerfuncCb fnFunctionTimer1kHzISR[MAX_TIMER_SERVICE] = {NULL};


static int current_index_callback_1k_fn = 0;

bool Timer1kHzAddCallback(TimerfuncCb fn)
{
	if(current_index_callback_1k_fn < MAX_TIMER_SERVICE)
	{
		fnFunctionTimer1kHzISR[current_index_callback_1k_fn] = fn;
		current_index_callback_1k_fn++;
		return true;
	}
	return false;
}

void TimerTick1kHz()
{
	int i = 0;
	for (i=0; i < current_index_callback_1k_fn; i++)
	{
		if (fnFunctionTimer1kHzISR[i] != NULL)
		{
			fnFunctionTimer1kHzISR[i]();
		}
	}
}
