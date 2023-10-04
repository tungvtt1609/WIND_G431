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

void TestMPPTInit(MPPT_PNO *v){
	v->Iwt = 0;
	v->Iwt_prev = 0;
	v->Vwt = 0;
	v->Vwt_prev = 0;
	v->Delta_V = 0;
	v->Delta_P = 0;
	v->Stepsize = 0;
	v->K_factor = 1;
	v->Power_wind = 0.1;
	v->Power_wind_prev = 0;
//	v->VmppOut = 0;
//	v->VmppOut_prev = 0;
//	v->VmppOutMax	= 2;
//	v->VmppOutMin = 0.005;
	v-> D_ref = 0 ;
	v-> D_prev  = 0.01 ;
	v-> Delta_D = 0.001;
	v->	D_refMin = 0.005 ;
	v->	D_refMax = 1.5 ;
	v->mppt_enable = 1;
	v->mppt_first = 1;
}

void TestMPPTProcess(MPPT_PNO *v){

//	flag_check_mppt = true;
	v->Delta_V 		= v->Vwt - v->Vwt_prev;
	v->Power_wind 	= v->Vwt * v->Iwt;
	v->Delta_P 		= v->Power_wind - v->Power_wind_prev;
	v->K_factor		= 1;

	if ( v->mppt_enable == 1){
		if (v->Delta_P > 0){
			if(v->Delta_V >0){
				v->D_ref = v->D_prev - v->Delta_D	*	v->K_factor;
			}
			else{
				v->D_ref = v->D_prev + v->Delta_D	*	v->K_factor;
			}
		}
		else{
			if(v->Delta_V >0){
				v->D_ref = v->D_prev + v->Delta_D	*	v->K_factor;
			}
			else{
				v->D_ref = v->D_prev - v->Delta_D	*	v->K_factor;
			}
		}
	}

	if (v->D_ref > v->D_refMax || v->D_ref < v->D_refMin ){
		v->D_ref = v->D_prev;
	}

	v->Power_wind_prev = v->Power_wind;
	v->Vwt_prev = v->Vwt;
	v->D_prev = v->D_ref;

}

void TestMPPTUpdate(MPPT_PNO *mppt, float cur, float vol){

	static float vwt_avg_multiplier = 0.002f;
	static float iwt_avg_multiplier = 0.002f;
	mppt ->Vwt = ((vol - mppt->Vwt)*vwt_avg_multiplier) + mppt->Vwt;
	mppt ->Iwt = ((cur - mppt->Iwt)*iwt_avg_multiplier) + mppt->Iwt;

}
