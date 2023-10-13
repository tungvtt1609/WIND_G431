/*
 * signal.c
 *
 *  Created on: Apr 12, 2023
 *      Author: TungVT
 */

#include "signal.h"

static adc_obj_t* s_signal_obj[ADC_ID_MAX];

float g_vin;
float g_vout;
float g_iload;
float g_idumpload;
float g_temp1;
float g_temp2;
float g_windspeed;
float g_freq;

#define ADC_FULLSCALE_VALUE		4095

adc_signal_type_t g_signal_value[SIGNAL_MAX] = {
		{VIN,						ADC_ID_2,		ADC_CH_3, 			0,		&g_vin,				0,						201.3	},
		{VOUT,						ADC_ID_1,		ADC_CH_1,			0,		&g_vout,		  	0,						102.3}, // 102.3*0.635
		{ILOAD,						ADC_ID_2,		ADC_CH_17,			0,		&g_iload,			151.88, 						64.7058},
		{IDUMP,						ADC_ID_2,		ADC_CH_13,			0,		&g_idumpload,		0,						64.7058},
		{TEMP1,						ADC_ID_2,		ADC_CH_4,			0,		&g_temp1,			0,						0			},
		{TEMP2,						ADC_ID_1,		ADC_CH_2,			0,		&g_temp2,			0,						0			},
		{WIND_SPEED,				ADC_ID_1,		ADC_CH_15,			0,		&g_windspeed,		0,						0			},
		{FREQ,						ADC_ID_1,		ADC_CH_5,			0,		&g_freq,			0,						0			}

};

//static double Convert_ADC_To_Val(uint16_t adc_val){
//	return 1;
//}

void Signal_Init(void){
	uint16_t i = 0;
	for(i = 0; i < ADC_ID_MAX; i++){
		s_signal_obj[i] = ADC_DMA_CREATE(i);
	}
}

void Get_All_Signal(void){
	int i = 0;
	for(i = 0; i < SIGNAL_MAX; i++){
		g_signal_value[i].val_control = (float)(Get_ADC_VAL_CH(s_signal_obj[g_signal_value[i].adc_id], g_signal_value[i].adc_ch) - g_signal_value[i].offset)/ADC_FULLSCALE_VALUE;
		*g_signal_value[i].real_value = g_signal_value[i].val_control*g_signal_value[i].gain;
	}
}

void Signal_Update(signal_type_e signal){
	g_signal_value[signal].val_control = (float)(Get_ADC_VAL_CH(s_signal_obj[g_signal_value[signal].adc_id], g_signal_value[signal].adc_ch) - g_signal_value[signal].offset)/ADC_FULLSCALE_VALUE;
	*g_signal_value[signal].real_value = g_signal_value[signal].val_control*g_signal_value[signal].gain;
}
