/*
 * driver_pwm_dumpload.c
 *
 *  Created on: Sep 12, 2023
 *      Author: TungVT
 */


#include "driver_pwm_dumpload.h"

void PwmWindDumpLoadInit(timer_advance_obj_t *timeradvanceX){
//	if(timeradvanceX->advance_timer_id == TIMER_ADVANCE_ID_1){
//		Tim1CCIsr = WindDumpLoadCtrlFn;
//	}
//	else if(timeradvanceX->advance_timer_id == TIMER_ADVANCE_ID_2){
//		Tim4CCIsr = WindDumpLoadCtrlFn;
//	}

	PwmWindDumpLoadDisableOutput(timeradvanceX);
	SetCompareCH2(timeradvanceX, 0);
	EnableCntTimer(timeradvanceX);
//	TIM_ClearFlag_CC2(timeradvanceX);
//	TIM_EnableIT_CC2(timeradvanceX);

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
