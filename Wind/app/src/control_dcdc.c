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
#define Kp_cur  0.00020		//0.01
#define Ki_cur	0.20			//60

#define Kp_vol	0.002	//0.75/4
#define Ki_vol	4		//0.75*2*25

#define Kp_MPPT 0.002		//ripple 7V
#define Ki_MPPT	10


typedef void (*fnCb)();
static fnCb fnControllerDC = NULL;

static timer_advance_obj_t *s_ptimer_pwm;

static CNTL_2P2Z_F_C_Coeffs vol_controller_pi_coef;
static CNTL_2P2Z_F_C_Vars vol_controller_pi_var;

static CNTL_2P2Z_F_C_Coeffs cur_controller_pi_coef;
static CNTL_2P2Z_F_C_Vars cur_controller_pi_var;

static CNTL_2P2Z_F_C_Coeffs MPPT_controller_pi_coef;
static CNTL_2P2Z_F_C_Vars MPPT_controller_pi_var;


static void DcDcProcess(void);
static void DcDc_Buck_Boost_Mppt(void);

void ControlDcDcBuckBoostInit(void)
{
	s_ptimer_pwm = create_advance_timer(TIMER_ADVANCE_ID_1, FREQUENCY_SAMPLE_WIND_BUCK_BOOST);
	PwmWindBuckBoostInit(s_ptimer_pwm, &DcDcProcess);
}

//void ControlDcDcBoostInit(void)
//{
//	s_ptimer_pwm = create_advance_timer(TIMER_ADVANCE_ID_1, FREQUENCY_SAMPLE_WIND_BUCK_BOOST);
//	PwmWindBoostInit(s_ptimer_pwm, &DcDcProcess);
//}


uint32_t mppt_cnt = 0;
static float Vout_Sum = 0, Iload_Sum = 0, Vin_Sum = 0, Mean_Vout = 0 , Mean_Iload = 0, Mean_Vin = 0;
static void DcDcProcess(void)
{
	mppt_cnt++;
	Signal_Update(VIN);
	Signal_Update(ILOAD);
	Signal_Update(VOUT);
//	MPPTUpdate(&g_wind.mppt, g_wind.iwt, g_wind.vwt);
	if(mppt_cnt	<	50000)
	{
		Vin_Sum			+=	*g_signal_value[VIN].real_value;
		Vout_Sum 		+=	*g_signal_value[VOUT].real_value;
		Iload_Sum 		+=	*g_signal_value[ILOAD].real_value;
	}
	else
	{

		Mean_Vin 		= Vin_Sum/50000;
		Mean_Vout 		= Vout_Sum/50000;
		Mean_Iload 		= Iload_Sum/50000;
		WindControlUpdate(&g_wind, Mean_Iload, Mean_Vin,Mean_Vout);
		MPPTUpdate(&g_wind.mppt, g_wind.iwt, g_wind.vwt, g_wind.vout);
		WindControlMppt(&g_wind);
		Vin_Sum			= 0;
		Vout_Sum 		= 0;
		Iload_Sum 		= 0;
		mppt_cnt		= 0;

	}

	if(fnControllerDC != NULL)
	{
		fnControllerDC();
	}
}



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


//void TestBoost()
//{
//	PwmWindBoostSetDuty(s_ptimer_pwm, 0.5);
//	PwmWindBoostEnable(s_ptimer_pwm);
//}
//void TestBuck()
//{
//	PwmWindBuckSetDuty(s_ptimer_pwm, 0.1);
//	PwmWindBuckEnable(s_ptimer_pwm);
//}
//
//
//void Test_Buck_Boost(float i)
//{
//
//	if (i >= 0 && i <= 1){
//
//		PwmWindBuckSetDuty(s_ptimer_pwm, i);
//		PwmWindBoostSetDuty(s_ptimer_pwm, 0);
//	}
//	else if (i>1 && i<2) {
//		PwmWindBuckSetDuty(s_ptimer_pwm, 1);
//		PwmWindBoostSetDuty(s_ptimer_pwm, (i-1));
//	}
//}

void Vol_Controller_Init(void){
	CNTL_2P2Z_F_C_COEFF_INIT(vol_controller_pi_coef);
	vol_controller_pi_coef.Max 		= 1.5;
	vol_controller_pi_coef.IMin 	= 	0;
	vol_controller_pi_coef.Min		=	0;
	computeDF22_PIcontrollerCoeff(&vol_controller_pi_coef,	Kp_vol,	Ki_vol,	FREQUENCY_SAMPLE_WIND_BUCK_BOOST);
	CNTL_2P2Z_F_C_VAR_INIT(vol_controller_pi_var);

}
void Curr_Controller_Init(void){

	CNTL_2P2Z_F_C_COEFF_INIT(cur_controller_pi_coef);
	cur_controller_pi_coef.Max		= 1.5;
	cur_controller_pi_coef.IMin		= 0;
	cur_controller_pi_coef.Min		= 0;
	computeDF22_PIcontrollerCoeff(&cur_controller_pi_coef,Kp_cur ,Ki_cur, FREQUENCY_SAMPLE_WIND_BUCK_BOOST );
	CNTL_2P2Z_F_C_VAR_INIT(cur_controller_pi_var);
}
void MPPT_Controller_Init(void){

	CNTL_2P2Z_F_C_COEFF_INIT(MPPT_controller_pi_coef);
	MPPT_controller_pi_coef.Max			= 1.5;
	MPPT_controller_pi_coef.IMin		= 0;
	MPPT_controller_pi_coef.Min			= 0;
	computeDF22_PIcontrollerCoeff(&MPPT_controller_pi_coef, Kp_MPPT ,Ki_MPPT, FREQUENCY_SAMPLE_WIND_BUCK_BOOST );
	CNTL_2P2Z_F_C_VAR_INIT(MPPT_controller_pi_var);
}


void DcDc_MPPT_Controller(void){

	fnControllerDC = &DcDc_Buck_Boost_Mppt;
}
float DUTY_FINAL ;
static void DcDc_Buck_Boost_Mppt(void){
	static float duty_mppt;
//	static float DUTY_FINAL ;
	static float duty_cur;
	static float duty_vol;
	if (g_wind.is_plugin == true){

				/* Iout controller */
		cur_controller_pi_var.Ref = 22;
		cur_controller_pi_var.Fdbk = *g_signal_value[ILOAD].real_value;
		CNTL_2P2Z_F_C(cur_controller_pi_coef, cur_controller_pi_var);

				/* Vout controller */
		vol_controller_pi_var.Ref 	= 54;
		vol_controller_pi_var.Fdbk 	= *g_signal_value[VOUT].real_value ;
		CNTL_2P2Z_F_C(vol_controller_pi_coef, vol_controller_pi_var);

				/* MPPT controller */
		MPPT_controller_pi_var.Ref 		= 	*g_signal_value[VIN].real_value;
		MPPT_controller_pi_var.Fdbk 	= 	g_wind.mppt.VmppOut;		//30;
		CNTL_2P2Z_F_C(MPPT_controller_pi_coef, MPPT_controller_pi_var);

		duty_vol = vol_controller_pi_var.Out;
		duty_cur = cur_controller_pi_var.Out;
		duty_mppt = MPPT_controller_pi_var.Out;

		if (duty_vol < duty_cur && duty_vol < duty_mppt){
//		if (duty_vol < duty_mppt){
			DUTY_FINAL = duty_vol;
		}
		else if(duty_cur < duty_vol && duty_cur < duty_mppt)
		{
			DUTY_FINAL = duty_cur; //duty_mppt;
		}
		else{
			DUTY_FINAL = duty_mppt;
		}

//		DUTY_FINAL = duty_mppt;
		if (DUTY_FINAL >= 0 && DUTY_FINAL <= 1){
			PwmWindBuckSetDuty(s_ptimer_pwm,DUTY_FINAL);
			PwmWindBoostSetDuty(s_ptimer_pwm,0);
		}
		else{
			PwmWindBuckSetDuty(s_ptimer_pwm,1);
			PwmWindBoostSetDuty(s_ptimer_pwm,(DUTY_FINAL-1));

		}
	}
}

