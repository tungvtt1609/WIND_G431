/*
 * driver_pwm_dumpload.c
 *
 *  Created on: Sep 12, 2023
 *      Author: TungVT
 */


#include "driver_pwm_dumpload.h"

void PwmWindDumpLoadInit(timer_advance_obj_t *timeradvanceX){

	if(timeradvanceX->advance_timer_id == TIMER_ADVANCE_ID_3)
		{
			PwmWindDumpLoadDisableOutput(timeradvanceX);
			EnableCntTimer(timeradvanceX);
		}
}

void PwmWindDumpLoadEnableOutput(timer_advance_obj_t *timeradvanceX)
{
	EnableTimerChanel(timeradvanceX);
	EnableCH2Output(timeradvanceX);
}

void PwmWindDumpLoadDisableOutput(timer_advance_obj_t *timeradvanceX)
{
	DisableTimerChanel(timeradvanceX);
	DisableCH2Output(timeradvanceX);

}
