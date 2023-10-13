/*
 * driver_pwm_dumpload.c
 *
 *  Created on: Sep 12, 2023
 *      Author: TungVT
 */


#include "driver_pwm_dumpload.h"

void PwmWindDumpLoadInit(timer_advance_obj_t *timeradvanceX, pTIMER_ADVANCE_EVENT_INT_HANDLE DumpCtrlFn){
	if(timeradvanceX->advance_timer_id == TIMER_ADVANCE_ID_1)
	{
		Tim1CCIsr = DumpCtrlFn;
	}
	else if(timeradvanceX->advance_timer_id == TIMER_ADVANCE_ID_2)
	{
		Tim8CCIsr = DumpCtrlFn;
		SetCompareCH1(timeradvanceX,0);
	}
	else if(timeradvanceX->advance_timer_id == TIMER_ADVANCE_ID_3)
	{
		Tim15CCIsr = DumpCtrlFn;
	}
	PwmWindDumpLoadDisableOutput(timeradvanceX);

//	PwmWindBuckDisable(timeradvanceX);
//	SetCompareCH3(timeradvanceX, 0);
	EnableCntTimer(timeradvanceX);
//	TIM_ClearFlag_CC3(timeradvanceX);
//	TIM_EnableIT_CC3(timeradvanceX);

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
