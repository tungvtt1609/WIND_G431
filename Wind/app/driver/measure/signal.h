/*
 * signal.h
 *
 *  Created on: Apr 12, 2023
 *      Author: TungVT
 */

#ifndef DRIVER_MEASURE_SIGNAL_H_
#define DRIVER_MEASURE_SIGNAL_H_

#include "porting/adc/adc.h"
#define FULLSCALE		4095


typedef enum signal_type_e {
	VIN = 0,
	VOUT,
	ILOAD,
	IDUMP,
	TEMP1,
	TEMP2,
	WIND_SPEED,
	FREQ,
	SIGNAL_MAX
}signal_type_e;

typedef struct adc_signal_type_t {
	signal_type_e signal_type;
	ADC_ID_e adc_id;
	ADC_CH_e adc_ch;
	float val_control;
	float *real_value;
	float offset;
	float gain;
}adc_signal_type_t;

extern float g_vin;
extern float g_vout;
extern float g_iload;
extern float g_idumpload;
extern float g_temp1;
extern float g_temp2;
extern float g_windspeed;
extern float g_freq;

extern adc_signal_type_t g_signal_value[];

void Signal_Init(void);
void Signal_Update(signal_type_e signal);
void Get_All_Signal(void);
double Get_Vin_Signal_Val(void);
double Get_Iload_Signal_Val(void);

#endif /* DRIVER_MEASURE_SIGNAL_H_ */
