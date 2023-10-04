/*
 * driver_pwm_boost.h
 *
 *  Created on: Mar 8, 2023
 *      Author: Thanh Tung
 */

#ifndef DRIVER_CONTROL_DRIVER_PWM_DC_DC_H_
#define DRIVER_CONTROL_DRIVER_PWM_DC_DC_H_

#include "timer/timer_pwm.h"


#define PwmWindBuckSetDuty(timeradvanceX, Duty) 														\
				uint32_t u32CompareBuckTemp = 0;																		\
				u32CompareBuckTemp = (uint32_t)(Duty*timeradvanceX->TimerPeriod);	\
				SetCompareCH2(timeradvanceX, u32CompareBuckTemp/2);								\
				SetCompareCH1(timeradvanceX, u32CompareBuckTemp);

#define PwmWindBoostSetDuty(timeradvanceX, Duty) 														\
				uint32_t u32CompareBoostTemp = 0;																		\
				u32CompareBoostTemp = (uint32_t)(Duty*timeradvanceX->TimerPeriod);	\
				SetCompareCH2(timeradvanceX, u32CompareBoostTemp/2);								\
				SetCompareCH4(timeradvanceX, u32CompareBoostTemp);


void PwmWindBuckInit(timer_advance_obj_t *timeradvanceX, pTIMER_ADVANCE_EVENT_INT_HANDLE WindBuckCtrlFn);			//Init PWM Buck
void PwmWindBoostInit(timer_advance_obj_t *timeradvanceX, pTIMER_ADVANCE_EVENT_INT_HANDLE WindBoostCtrlFn);			//Init PWM Boost

void PwmWindBuckBoostEnableAllOutput(timer_advance_obj_t *timeradvanceX);																						//Enable Buck-Boost
void PwmWindBuckBoostDisableAllOutput(timer_advance_obj_t *timeradvanceX);																					//Disable Buck-Boost

void PwmWindBuckEnable(timer_advance_obj_t *timeradvanceX);																													//Enable Buck
void PwmWindBuckDisable(timer_advance_obj_t *timeradvanceX);																													//Disable Buck

void PwmWindBoostEnable(timer_advance_obj_t *timeradvanceX);																													//Enable Boost
void PwmWindBoostDisable(timer_advance_obj_t *timeradvanceX);

#endif /* DRIVER_CONTROL_DRIVER_PWM_DC_DC_H_ */
