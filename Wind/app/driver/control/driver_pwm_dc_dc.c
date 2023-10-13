/*
 * driver_pwm_boost.c
 *
 *  Created on: Mar 8, 2023
 *      Author: Thanh Tung
 */


#include <driver/control/driver_pwm_dc_dc.h>

void PwmWindBuckBoostInit(timer_advance_obj_t *timeradvanceX, pTIMER_ADVANCE_EVENT_INT_HANDLE WindBuckCtrlFn){
	if(timeradvanceX->advance_timer_id == TIMER_ADVANCE_ID_1)
	{
		Tim1CCIsr = WindBuckCtrlFn;
		SetCompareCH4(timeradvanceX,0);
//		SetCompareCH1N(timeradvanceX,0);
//		SetCompareCH2N(timeradvanceX,0);
	}
	else if(timeradvanceX->advance_timer_id == TIMER_ADVANCE_ID_2)
	{
		Tim8CCIsr = WindBuckCtrlFn;

	}
	else if (timeradvanceX->advance_timer_id == TIMER_ADVANCE_ID_3)
	{
		Tim15CCIsr = WindBuckCtrlFn;
	}
	PwmWindBuckBoostDisableAllOutput(timeradvanceX);
//	PwmWindBuckDisable(timeradvanceX);
	SetCompareCH3(timeradvanceX, 0);
	EnableCntTimer(timeradvanceX);
	TIM_ClearFlag_CC3(timeradvanceX);
	TIM_EnableIT_CC3(timeradvanceX);

}

//void PwmWindBoostInit(timer_advance_obj_t *timeradvanceX, pTIMER_ADVANCE_EVENT_INT_HANDLE WindBoostCtrlFn){
//	if(timeradvanceX->advance_timer_id == TIMER_ADVANCE_ID_1){
//		Tim1CCIsr = WindBoostCtrlFn;
//		SetCompareCH4(timeradvanceX, 0);
//	}
//	else if(timeradvanceX->advance_timer_id == TIMER_ADVANCE_ID_2){
//		Tim15CCIsr = WindBoostCtrlFn;
//	}
//	PwmWindBuckBoostDisableAllOutput(timeradvanceX);
////	PwmWindBoostDisable(timeradvanceX);
//	SetCompareCH3(timeradvanceX, 0);
//	EnableCntTimer(timeradvanceX);
//	TIM_ClearFlag_CC3(timeradvanceX);
//	TIM_EnableIT_CC3(timeradvanceX);
//
//}
//

void PwmWindBuckBoostEnableAllOutput(timer_advance_obj_t *timeradvanceX){
	EnableTimerChanel(timeradvanceX);
	EnableCH1NAndCH2NOutputs(timeradvanceX);
}

void PwmWindBuckBoostDisableAllOutput(timer_advance_obj_t *timeradvanceX){
	DisableTimerChanel(timeradvanceX);
	DisableCH1NAndCH2NOutputs(timeradvanceX);
}



void PwmWindBuckEnable(timer_advance_obj_t *timeradvanceX){
	EnableTimerChanel(timeradvanceX);
	EnableCH2NOutput(timeradvanceX);
}

void PwmWindBoostEnable(timer_advance_obj_t *timeradvanceX){
	EnableTimerChanel(timeradvanceX);
	EnableCH1NOutput(timeradvanceX);
}



void PwmWindBuckDisable(timer_advance_obj_t *timeradvanceX)
{
	DisableCH2NOutput(timeradvanceX);
}
void PwmWindBoostDisable(timer_advance_obj_t *timeradvanceX)
{
	DisableCH1NOutput(timeradvanceX);
}

//void PwmWindBuckSetDuty(timer_advance_obj_t *timeradvanceX, float Duty) {
//
//	uint32_t u32CompareBuckTemp = 0;
//				u32CompareBuckTemp = (uint32_t)(Duty*timeradvanceX->TimerPeriod);
//				SetCompareCH1(timeradvanceX, u32CompareBuckTemp);
//
//}

//void PwmWindBoostSetDuty(timer_advance_obj_t *timeradvanceX, float Duty) 	{
//				uint32_t u32CompareBoostTemp = 0;
//				u32CompareBoostTemp = (uint32_t)(Duty*timeradvanceX->TimerPeriod);
//				SetCompareCH4(timeradvanceX, u32CompareBoostTemp);
//				}
