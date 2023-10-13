/*
 * driver_pwm_xylanh.h
 *
 *  Created on: Sep 26, 2023
 *      Author: user
 */

#ifndef DRIVER_CONTROL_DRIVER_PWM_XYLANH_H_
#define DRIVER_CONTROL_DRIVER_PWM_XYLANH_H_

#include "porting/timer/timer_pwm.h"


#define PwmXylanh3SetDuty(timeradvanceX, Duty) 														\
			uint32_t u32CompareXylanh3Temp = 0;																		\
			u32CompareXylanh3Temp = (uint32_t)(Duty*timeradvanceX->TimerPeriod);	\
			SetCompareCH3N(timeradvanceX, u32CompareXylanh3Temp);

#define PwmXylanh4SetDuty(timeradvanceX, Duty) 														\
			uint32_t u32CompareXylanh4Temp = 0;																		\
			u32CompareXylanh4Temp = (uint32_t)(Duty*timeradvanceX->TimerPeriod);	\
			SetCompareCH1(timeradvanceX, u32CompareXylanh4Temp);

void PwmXylanhInit(timer_advance_obj_t *timeradvanceX,pTIMER_ADVANCE_EVENT_INT_HANDLE XylanhCtrlFn);

void PwmXylanh3EnableOutput(timer_advance_obj_t *timeradvanceX);
void PwmXylanh3DisableOutput(timer_advance_obj_t *timeradvanceX);

void PwmXylanh4EnableOutput(timer_advance_obj_t *timeradvanceX);
void PwmXylanh4DisableOutput(timer_advance_obj_t *timeradvanceX);

void PwmXylanhEnableAllOutput(timer_advance_obj_t *timeradvanceX);
void PwmXylanhDisableAllOutput(timer_advance_obj_t *timeradvanceX);

#endif /* DRIVER_CONTROL_DRIVER_PWM_XYLANH_H_ */
