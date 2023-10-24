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
#include "porting/timer/timer_pwm.h"
#include "Frequency.h"
#include "control_dcdc.h"
#include "control_dumpload.h"


#define FREQUENCY_SAMPLE_WIND_DUMPLOAD			50000
static timer_advance_obj_t *s_ptimer_pwm;


void ControlDcDcDumpInit(void)
{
	s_ptimer_pwm = create_advance_timer(TIMER_ADVANCE_ID_3, FREQUENCY_SAMPLE_WIND_DUMPLOAD);
	PwmWindDumpLoadInit(s_ptimer_pwm);
}

void Start_PWM_Dumpload(void)
{
	PwmWindDumpLoadEnableOutput(s_ptimer_pwm);
}

void Stop_PWM_DumpLoad(void)
{
	PwmWindDumpLoadDisableOutput(s_ptimer_pwm);
}

void Dumpload_controller(void)
{
	if((150.0 <= g_vin) && (g_vin <= 155.0) )
	{
		PwmWindDumpLoadSetDuty(s_ptimer_pwm, 0.25);
		Start_PWM_Dumpload();
	}
	else if ((155.0 < g_vin) && (g_vin <= 160.0) ){
		PwmWindDumpLoadSetDuty(s_ptimer_pwm, 0.5);
		Start_PWM_Dumpload();
	}
	else if ((160.0 < g_vin) && (g_vin <= 165.0) ){
		PwmWindDumpLoadSetDuty(s_ptimer_pwm, 0.75);
		Start_PWM_Dumpload();
	}
	else{
		PwmWindDumpLoadSetDuty(s_ptimer_pwm, 1);
		Start_PWM_Dumpload();
	}

}
//void TestDump()
//{
//	PwmWindDumpLoadSetDuty(s_ptimer_pwm, 0.5);
//	PwmWindDumpLoadEnableOutput(s_ptimer_pwm);
//}

