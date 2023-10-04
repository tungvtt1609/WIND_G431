/*
 * control_dumpload.c
 *
 *  Created on: Sep 12, 2023
 *      Author: TungVT
 */

#include "driver/control/driver_pwm_dumpload.h"
#include "config_parameter.h"
#include "error_check.h"
#include "driver/measure/signal.h"


#define FREQUENCY_SAMPLE_WIND_DUMPLOAD			50000
static timer_advance_obj_t *s_ptimer_pwm;

void ControlDcDcDumpInit(void)
{
	s_ptimer_pwm = create_advance_timer(TIMER_ADVANCE_ID_1, FREQUENCY_SAMPLE_WIND_DUMPLOAD);
	PwmWindDumpLoadInit(s_ptimer_pwm);
}

void Start_PWM_Dumpload(void)
{
	PwmWindDumpLoadEnableOutput(s_ptimer_pwm);
}

void Stop_PWM_Dumpload(void)
{
	PwmWindDumpLoadDisableOutput(s_ptimer_pwm);
}

void Dumpload_controller(void)
{
	if(g_vin > DC_MAX_INPUT)
	{
		PwmWindDumpLoadSetDuty(s_ptimer_pwm, (g_vin - MAX_VIN)/g_vin);
		Start_PWM_Dumpload();
	}
	else
	{
		PwmWindDumpLoadSetDuty(s_ptimer_pwm, 0);
		Stop_PWM_Dumpload();
	}
}
