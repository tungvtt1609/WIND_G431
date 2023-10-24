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

void WindControlInit(WIND *wind){
	wind->is_plugin 			= true;
//	wind->boost_state 			= false;
//	wind->is_dumpload 			= false;
	wind->vwt_oc 				= wind->vwt;
	MPPTInit(&wind->mppt);
	wind->mppt.mppt_enable 		= 1;
	wind->mppt.VmppOut 			= 150;
	wind->mppt.MaxVolt			= 150;
	wind->mppt.MinVolt 			= 10;
}

float Power_Tip =0;
uint16_t SoftStart_Cnt = 0;
uint16_t SoftStart_Cnt1 = 0;
void WindControlMppt(WIND *wind){
	if(wind->is_plugin == true){
		Power_Tip = g_wind.vout * g_wind.iwt;
		if(Power_Tip > Power_Tip_Min){
			Soft_Start = false;
		}
		else if(Power_Tip < 10){
			SoftStart_Cnt ++;
			if(SoftStart_Cnt > 10){
				Soft_Start = true;
				SoftStart_Cnt = 0;
			}
		}

		if (Soft_Start == true){
			g_wind.mppt.VmppOut -= 10;
			if (g_wind.mppt.VmppOut < 10 ){
				SoftStart_Cnt1++;
				g_wind.mppt.VmppOut = 10;
				if (SoftStart_Cnt1>10){
					g_wind.mppt.VmppOut = 150;
					SoftStart_Cnt1 =0 ;
				}

			}
		}
		else{
			MPPTProcess(&wind->mppt);
		}
	}
}

void WindControlUpdate(WIND *wind, float wind_cur, float wind_vol, float Vout)
{
	wind->vwt = wind_vol;
	wind->iwt = wind_cur;
	wind->vout = Vout;
}
