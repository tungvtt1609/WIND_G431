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
	float Delta_V;
	float Delta_P;
	float Stepsize;
	float K_factor;
	float Power_wind;
	float Power_wind_prev;
	float VmppOut;
	float VmppOut_prev ;
	float VmppOutMax;
	float VmppOutMin;
	float D_ref;
	float D_prev;
	float Delta_D;
	float D_refMax;
	float D_refMin;
	uint16_t mppt_enable;
	uint16_t mppt_first;
} MPPT_PNO;

void TestMPPTInit(MPPT_PNO *v);
void TestMPPTProcess(MPPT_PNO *v);
void TestMPPTUpdate(MPPT_PNO *mppt, float cur, float vol);

#endif /* SRC_WIND_MPPT_H_ */
