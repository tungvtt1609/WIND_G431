/*
 * wind_mppt.h
 *
 *  Created on: Mar 8, 2023
 *      Author: Thanh Tung
 */

#ifndef SRC_WIND_MPPT_H_
#define SRC_WIND_MPPT_H_

#include "math.h"
#include "types.h"

typedef struct {
	float Iwt;
	float Iwt_prev;
	float Vwt;
	float Vwt_prev;
	float Vout;
	float Delta_V;
	float Delta_P;
	float Delta_Pmin;
	float StepsizeVol;
	float K_factor;
	float Power_wind;
	float Power_wind_prev;
	float VmppOut;
	float VmppOut_prev ;
	float MaxVolt;
	float MinVolt;
//	float D_ref;
//	float D_prev;
//	float Delta_D;
//	float D_refMax;
//	float D_refMin;
	uint16_t mppt_enable;
	uint16_t mppt_first;
} MPPT_PNO;

void MPPTInit(MPPT_PNO *v);
void MPPTProcess(MPPT_PNO *v);
void MPPTUpdate(MPPT_PNO *mppt, float cur, float vol, float vout);

#endif /* SRC_WIND_MPPT_H_ */
