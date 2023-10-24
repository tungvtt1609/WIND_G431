/*
 * wind_mppt.c
 *
 *  Created on: Mar 8, 2023
 *      Author: Thanh Tung
 */

#include "wind_mppt.h"
#include "math.h"
#include "driver/measure/signal.h"
#include "porting/timer/timer_pwm.h"
#include "driver/control/driver_pwm_dc_dc.h"
#include "driver/control/driver_pwm_dumpload.h"
#include "stdbool.h"

bool flag_check_mppt = false;

//static timer_advance_obj_t *s_ptimer_pwm;

void MPPTInit(MPPT_PNO *v){
	v->Iwt = 0;
//	v->Iwt_prev = 0;
	v->Vwt = 0;
	v->Vout = 0;
	v->Vwt_prev = 0;
	v->Delta_V = 0;


//	Cong suat dau vao wind turbine
	v->Power_wind = 0;
	v->Power_wind_prev = 0;
	v->Delta_P = 0;
	v->Delta_Pmin = 5;

//	MPPT tac dong vao Vin
	v->VmppOut = 0;
	v->VmppOut_prev = 0;
	v->StepsizeVol = 3.0;
	v->MaxVolt=0;
	v->MinVolt=0;

//	MPPT tac dong vao Duty Cycle
//	v-> D_ref = 0;
//	v-> D_prev  = 0.5 ;
//	v-> Delta_D = 0.055;
//	v->	D_refMin = 0.55 ;
//	v->	D_refMax = 1.5 ;
//	v->K_factor = 0;

	v->mppt_enable = 1;
	v->mppt_first = 1;
}

void MPPTProcess(MPPT_PNO *v){

		v->Delta_V 		= v->Vwt - v->Vwt_prev;
		v->Power_wind 	= v->Vout * v->Iwt;
		v->Delta_P 		= v->Power_wind - v->Power_wind_prev;
	//	v->K_factor			= fabs(v->Delta_P/v->Delta_V);


	//	if ( v->mppt_enable == 1){
	//	if (v->K_factor	<	0.05)
	//	{
	//		v->D_ref = v->D_prev;
	//	}
	//	else
	//	{
			if (v->Delta_P > 0){
				if(v->Delta_V > 0){

					v->VmppOut = v->VmppOut + v->StepsizeVol;
				}
				else{

					v->VmppOut = v->VmppOut - v->StepsizeVol;
				}
			}
			else //if(v->Delta_P < -v->Delta_Pmin)
			{
				if(v->Delta_V > 0){

					v->VmppOut = v->VmppOut - v->StepsizeVol;
				}
				else{

					v->VmppOut = v->VmppOut + v->StepsizeVol;
				}
			}
//			else{
//				if(v->Delta_V >0){
//					v->VmppOut = v->VmppOut_prev - v->StepsizeVol;
//				}
//				else{
//					v->VmppOut = v->VmppOut_prev + v->StepsizeVol;
//				}
//			}
	//	}


	    if(v->VmppOut < v->MinVolt) v->VmppOut = v->MinVolt;
	    if(v->VmppOut > v->MaxVolt) v->VmppOut = v->MaxVolt;

		v->Power_wind_prev = v->Power_wind;
//		v->Iwt_prev = v->Iwt;
		v->Vwt_prev = v->Vwt;

}

//void MPPTUpdate(MPPT_PNO *mppt, float cur, float vol){
//
//	static float vwt_avg_multiplier = 0.002f;
//	static float iwt_avg_multiplier = 0.002f;
//	mppt ->Vwt = ((vol - mppt->Vwt)*vwt_avg_multiplier) + mppt->Vwt;
//	mppt ->Iwt = ((cur - mppt->Iwt)*iwt_avg_multiplier) + mppt->Iwt;
//}

void MPPTUpdate(MPPT_PNO *mppt, float cur, float vol, float vout){

//	static float vout_avg_multiplier = 0.002f;
//	static float vwt_avg_multiplier = 0.002f;
//	static float iwt_avg_multiplier = 0.002f;
//	mppt ->Vwt = ((vol - mppt->Vwt)*vwt_avg_multiplier) + mppt->Vwt;
//	mppt ->Iwt = ((cur - mppt->Iwt)*iwt_avg_multiplier) + mppt->Iwt;
//	mppt ->Vout= ((vout - mppt->Vout)*vout_avg_multiplier)+mppt->Vout;
	mppt ->Vout = vout;
	mppt ->Vwt = vol;
	mppt ->Iwt = cur;
}
