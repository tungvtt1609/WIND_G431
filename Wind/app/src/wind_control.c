/*
 * wind_control.c
 *
 *  Created on: 18 Mar 2023
 *      Author: Thanh Tung
 */

#include "Wind_control.h"
#include "wind_mppt.h"



WIND g_wind;



void WindControlInit(WIND *wind){
	wind->is_plugin 			= true;
//	wind->boost_state 			= false;
//	wind->is_dumpload 			= false;
//	wind->vwt_oc 				= wind->vwt;
	TestMPPTInit(&wind->mppt);
//	wind->mppt.mppt_enable 		= 1;
//	wind->mppt.VmppOut 			= wind->vwt_oc - 5;
//	wind->mppt.MinVolt 			= wind->vwt_oc * 0.5f;
}

void WindControlMppt(WIND *wind){
	TestMPPTProcess(&wind->mppt);

//	if(wind->is_plugin == true){

//		 TestMPPTProcess(&wind->mppt);
//
////		if(fabs(wind->mppt.Vwt - wind->mppt.VmppOut) > 5.0){
////			wind->mppt.VmppOut = wind->mppt.Vwt - 1.0;
////		}
//	}
}

void WindControlUpdate(WIND *wind, float wind_cur, float wind_vol){

	wind->vwt = wind_vol;
	wind->iwt = wind_cur;
}
