/*
 * control_xylanh.c
 *
 *  Created on: Sep 26, 2023
 *      Author: user
 */

#include "control_xylanh.h"
#include "driver/control/driver_pwm_xylanh.h"
#include "porting/timer/timer_pwm.h"
#include "driver/measure/signal.h"

#define FREQUENCY_SAMPLE_XYLANH			50000
static timer_advance_obj_t *s_ptimer_pwm;

static void ControlXylanhISR(void);
void Control_Xylanh_Init()
{
	s_ptimer_pwm = create_advance_timer(TIMER_ADVANCE_ID_2, FREQUENCY_SAMPLE_XYLANH);
	PwmXylanhInit(s_ptimer_pwm,&ControlXylanhISR);
}

void Start_PWM_Xylanh_3()
{
	PwmXylanh3EnableOutput(s_ptimer_pwm);
}
void Stop_PWM_Xylanh_3()
{
	PwmXylanh3DisableOutput(s_ptimer_pwm);
}

void Start_PWM_Xylanh_4()
{
	PwmXylanh4EnableOutput(s_ptimer_pwm);
}

void Stop_PWM_Xylanh_4()
{
	PwmXylanh4DisableOutput(s_ptimer_pwm);
}

static void ControlXylanhISR(void)
{
	Signal_Update(WIND_SPEED);
}

void Test_PWM_Xylanh()
{

	PwmXylanh3SetDuty(s_ptimer_pwm, 0);
	PwmXylanh4SetDuty(s_ptimer_pwm,1);
	PwmXylanh3EnableOutput(s_ptimer_pwm);
	PwmXylanh4EnableOutput(s_ptimer_pwm);
}
