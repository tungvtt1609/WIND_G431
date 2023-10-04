/*
 * control_dcdc.c
 *
 *  Created on: Apr 12, 2023
 *      Author: TungVT
 */

#include <driver/control/driver_pwm_dc_dc.h>
#include "control_dcdc.h"

#include "lib/CNTL_2P2Z.h"
#include "math.h"
#include "driver/measure/signal.h"
#include "porting/timer/timer_pwm.h"
#include "wind_control.h"
#include "driver/control/driver_pwm_dumpload.h"
#include "wind_mppt.h"
#include "driver/control/pin_func.h"

#define FREQUENCY_SAMPLE_WIND_BUCK_BOOST			50000


bool flag = false;
float i;
typedef void (*fnCb)();

static fnCb fnControleDC = NULL;

static timer_advance_obj_t *s_ptimer_pwm;

static CNTL_2P2Z_F_C_Coeffs vol_controller_pi_coef[2];
static CNTL_2P2Z_F_C_Vars vol_controller_pi_var[2];

static CNTL_2P2Z_F_C_Coeffs cur_controller_pi_coef[2];
static CNTL_2P2Z_F_C_Vars cur_controller_pi_var[2];


static void DcDcProcess(void);
static uint32_t cnt_timer = 0;


void ControlDcDcBuckInit(void)
{
	s_ptimer_pwm = create_advance_timer(TIMER_ADVANCE_ID_1, FREQUENCY_SAMPLE_WIND_BUCK_BOOST);
	PwmWindBuckInit(s_ptimer_pwm, &DcDcProcess);
}

void ControlDcDcBoostInit(void)
{
	s_ptimer_pwm = create_advance_timer(TIMER_ADVANCE_ID_1, FREQUENCY_SAMPLE_WIND_BUCK_BOOST);
	PwmWindBoostInit(s_ptimer_pwm, &DcDcProcess);
}



static void DcDcProcess(void)
{
	Signal_Update(VIN);
	Signal_Update(ILOAD);
	Signal_Update(VOUT);
//	Signal_Update(IDUMP);
//	Signal_Update(TEMP1);
//	Signal_Update(TEMP2);
//	Signal_Update(WIND_SPEED);
//	Signal_Update(FREQ);
//	static uint32_t count = 0;
//	count++;
//	if(count >= 1000000)
//	{
//		count = 0;
//	}
	WindControlUpdate(&g_wind, g_iload, g_vin);
}


uint32_t count_vol_loop = 0;

//static CNTL_2P2Z_F_C_Coeffs dc_no_mppt_pi_vol_coef;
//static CNTL_2P2Z_F_C_Vars dc_no_mppt_pi_vol_var;
//static CNTL_2P2Z_F_C_Coeffs dc_no_mppt_pi_cur_coef;
//static CNTL_2P2Z_F_C_Vars dc_no_mppt_pi_cur_var;



void DC_StopPWM_Boost(void)
{
	PwmWindBoostDisable(s_ptimer_pwm);
}

void DC_StartPWM_Boost(void)
{
	if (g_wind.is_plugin == true)
	{
		 PwmWindBoostEnable(s_ptimer_pwm);
	}
}

void DC_StopPWM_Buck(void)
{
	PwmWindBuckDisable(s_ptimer_pwm);
}

void DC_StartPWM_Buck(void)
{
//	PwmWindBuckEnable(s_ptimer_pwm);
	if (g_wind.is_plugin == true)
	{
		 PwmWindBuckEnable(s_ptimer_pwm);
	}
}


void TestBoost()
{
	PwmWindBoostSetDuty(s_ptimer_pwm, 0.7);
	PwmWindBoostEnable(s_ptimer_pwm);
}
void TestBuck()
{
	PwmWindBuckSetDuty(s_ptimer_pwm, 0.5);
	PwmWindBuckEnable(s_ptimer_pwm);
}

void Test_Buck_Boost(float i)
{

	if (i >= 0 && i <= 1){

		PwmWindBuckSetDuty(s_ptimer_pwm, i);
		PwmWindBoostSetDuty(s_ptimer_pwm, 0);
	}
	else if (i>1 && i<2) {
		PwmWindBuckSetDuty(s_ptimer_pwm, 1);
		PwmWindBoostSetDuty(s_ptimer_pwm, (i-1));
	}
}


void DcDc_ControllerMppt(void){
	flag = true;
	static float duty;

	TestMPPTUpdate(&g_wind.mppt, g_wind.iwt, g_wind.vwt);
	WindControlMppt(&g_wind);


	duty = g_wind.mppt.D_ref;
	if (duty >= 0 && duty <= 1){
		PwmWindBuckSetDuty(s_ptimer_pwm, duty);
		PwmWindBoostSetDuty(s_ptimer_pwm, 0);
	}
	else{
		PwmWindBuckSetDuty(s_ptimer_pwm, 1);
		PwmWindBoostSetDuty(s_ptimer_pwm, (duty-1));
	}
}
