/*
 * driver_pwm_xylanh.c
 *
 *  Created on: Sep 26, 2023
 *      Author: user
 */

#include "driver_pwm_xylanh.h"

void PwmXylanhInit(timer_advance_obj_t *timeradvanceX)
{
	if(timeradvanceX->advance_timer_id == TIMER_ADVANCE_ID_2)
		{
			PwmWindDumpLoadDisableOutput(timeradvanceX);
			EnableCntTimer(timeradvanceX);
		}
}

void PwmXylanh3EnableOutput(timer_advance_obj_t *timeradvanceX)
{
	EnableTimerChanel(timeradvanceX);
	EnableCH3NOutput(timeradvanceX);
}

void PwmXylanh4EnableOutput(timer_advance_obj_t *timeradvanceX)
{
	EnableTimerChanel(timeradvanceX);
	EnableCH1Output(timeradvanceX);
}

void PwmXylanh3DisableOutput(timer_advance_obj_t *timeradvanceX)
{
	DisableTimerChanel(timeradvanceX);
	DisableCH3NOutput(timeradvanceX);
}



void PwmXylanh4DisableOutput(timer_advance_obj_t *timeradvanceX)
{
	DisableTimerChanel(timeradvanceX);
	DisableCH1Output(timeradvanceX);
}

void PwmXylanhDisableAllOutput(timer_advance_obj_t *timeradvanceX)
{
	DisableTimerChanel(timeradvanceX);
	DisableCH1AndCH3NOutputs(timeradvanceX);
}

void PwmXylanhEnableAllOutput(timer_advance_obj_t *timeradvanceX)
{
	EnableTimerChanel(timeradvanceX);
	EnableCH1AndCH3NOutputs(timeradvanceX);
}
