/*
 * driver_pwm_dumpload.h
 *
 *  Created on: Sep 12, 2023
 *      Author: TungVT
 */

#ifndef DRIVER_CONTROL_DRIVER_PWM_DUMPLOAD_H_
#define DRIVER_CONTROL_DRIVER_PWM_DUMPLOAD_H_

#include "porting/timer/timer_pwm.h"
#include "stdint.h"




#define PwmWindDumpLoadSetDuty(timeradvanceX, Duty) 									\
				uint32_t u32CompareDumpLoadTemp = 0;													\
				u32CompareDumpLoadTemp = (uint32_t)(Duty*timeradvanceX->TimerPeriod);	\
				SetCompareCH2(timeradvanceX, u32CompareDumpLoadTemp);




void PwmWindDumpLoadInit(timer_advance_obj_t *timeradvanceX);
void PwmWindDumpLoadEnableOutput(timer_advance_obj_t *timeradvanceX);
void PwmWindDumpLoadDisableOutput(timer_advance_obj_t *timeradvanceX);

#endif /* DRIVER_CONTROL_DRIVER_PWM_DUMPLOAD_H_ */
