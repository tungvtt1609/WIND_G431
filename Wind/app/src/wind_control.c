/*
 * wind_control.c
 *
 *  Created on: 18 Mar 2023
 *      Author: Thanh Tung
 */

#include "Wind_control.h"
#include "wind_mppt.h"

#include "stdbool.h"


WIND g_wind;
bool Soft_Start = true;

//unsigned long Duc_Le=0;
void WindControlInit(WIND *wind){
	wind->is_plugin 			= true;
//	wind->boost_state 			= false;
//	wind->is_dumpload 			= false;
	wind->vwt_oc 				= wind->vwt;
	MPPTInit(&wind->mppt);
	wind->mppt.mppt_enable 		= 1;
	wind->mppt.VmppOut 			= 150;
	wind->mppt.MaxVolt			= 150;
	wind->mppt.MinVolt 			= 0;
}
float Power_Tip =0;
void WindControlMppt(WIND *wind){
//	MPPTProcess(&wind->mppt);
	if(wind->is_plugin == true){
		Power_Tip = g_wind.vout * g_wind.iwt;
		if(Power_Tip > Power_Tip_Min){
			Soft_Start = false;
		}
		else if(Power_Tip < 30){
			Soft_Start = true;
		}


		if (Soft_Start == true){
			g_wind.mppt.VmppOut -= 10;
			if (g_wind.mppt.VmppOut < 10 ){
				g_wind.mppt.VmppOut = 150;
			}
		}
		else{
			MPPTProcess(&wind->mppt);
		}
	}
}

//void WindControlUpdate(WIND *wind, float wind_cur, float wind_vol){
//
//	wind->vwt = wind_vol;
//	wind->iwt = wind_cur;
//}
void WindControlUpdate(WIND *wind, float wind_cur, float wind_vol, float Vout)
{
	wind->vwt = wind_vol;
	wind->iwt = wind_cur;
	wind->vout = Vout;
}
