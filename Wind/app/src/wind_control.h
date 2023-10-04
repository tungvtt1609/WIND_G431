/*
 * wind_control.h
 *
 *  Created on: 18 Mar 2023
 *      Author: Thanh Tung
 */

#ifndef SRC_WIND_CONTROL_H_
#define SRC_WIND_CONTROL_H_

#include "types.h"
#include "math.h"
#include "wind_mppt.h"

#define VOL_LOOP_DIV 	2
#define CUR_LOOP_DIV	1

typedef struct{
	bool is_plugin;
	bool is_mppt;
	bool boost_state;
	bool is_dumpload;
	float vwt;
	float iwt;
	float power;
	float power_avg;
	float sum_power;
	float sum_power_cycles;
	uint32_t sample_count;
	uint32_t cycles_count;
	uint32_t count_loop_vol;
	MPPT_PNO mppt;
}WIND;

extern WIND g_wind;

void WindControlInit(WIND *wind);
void WindControlMppt(WIND *wind);
void WindControlUpdate(WIND *wind, float wind_cur, float wind_vol);

#endif /* SRC_WIND_CONTROL_H_ */
