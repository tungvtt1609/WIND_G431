/*
 * signal.c
 *
 *  Created on: Apr 12, 2023
 *      Author: TungVT
 */

#include "signal.h"


#define ADC_FULLSCALE_VALUE		4095

static adc_obj_t* s_signal_obj[ADC_ID_MAX];

float g_vin;
float g_vout;
float g_iload;
float g_idumpload;
float g_temp1;
float g_temp2;
float g_windspeed;
float g_freq;

const uint16_t TempADC_Table[121]={
		216, 227, 240, 252, 266, 279, 294, 309,
		324, 340, 357, 375, 393, 411, 431, 451, 472, 493, 515, 538, 562, 586,
		611, 637, 664, 691, 719, 748, 778, 808, 839, 871, 903, 937, 971, 1005,
		1040, 1076, 1113, 1150, 1187, 1225, 1264, 1303, 1342, 1383, 1423, 1463,
		1504, 1546, 1587, 1629, 1670, 1712, 1754, 1796, 1838, 1880, 1922, 1964,
		2006, 2047, 2088, 2129, 2170, 2210, 2250, 2290, 2329, 2368, 2407, 2445,
		2482, 2519, 2556, 2592, 2627, 2662, 2696, 2730, 2763, 2795, 2828, 2859,
		2889, 2920, 2949, 2978, 3006, 3034, 3061, 3088, 3113, 3139, 3163, 3188,
		3211, 3234, 3257, 3279, 3300, 3321, 3341, 3361, 3380, 3399, 3417, 3435,
		3453, 3469, 3486, 3502, 3518, 3533, 3548, 3562, 3576, 3590, 3603, 3616,
		3628


};



adc_signal_type_t g_signal_value[SIGNAL_MAX] = {
		{VIN,						ADC_ID_2,		ADC_CH_3, 		0,	0,		&g_vin,				0,						201.3},	//offset 63.1327,				 gain 194.59
		{VOUT,						ADC_ID_1,		ADC_CH_1,		0,	0,		&g_vout,		  	-20.0146,				102.3},		//		-27.3,						89.9
		{ILOAD,						ADC_ID_2,		ADC_CH_17,		0,	0,		&g_iload,			0, 						64.7058},		//		388.883, 					56.86
		{IDUMP,						ADC_ID_2,		ADC_CH_13,		0,	0,		&g_idumpload,		0,						64.7058},
		{TEMP1,						ADC_ID_2,		ADC_CH_4,		0,	0,		&g_temp1,			0,						0			},
		{TEMP2,						ADC_ID_1,		ADC_CH_2,		0,	0,		&g_temp2,			0,						0			}
//		{WIND_SPEED,				ADC_ID_1,		ADC_CH_15,		0,	0,		&g_windspeed,		0,						0			},
//		{FREQ,						ADC_ID_1,		ADC_CH_5,			0,		&g_freq,			0,						0			}

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

//void Get_All_Signal(void){
//	int i = 0;
//	for(i = 0; i < SIGNAL_MAX; i++){
//		g_signal_value[i].val_control = (float)(Get_ADC_VAL_CH(s_signal_obj[g_signal_value[i].adc_id], g_signal_value[i].adc_ch) - g_signal_value[i].offset)/ADC_FULLSCALE_VALUE;
//		*g_signal_value[i].real_value = g_signal_value[i].val_control*g_signal_value[i].gain;
//	}
//}

void Signal_Update(signal_type_e signal){
	g_signal_value[signal].val_control = (float)(Get_ADC_VAL_CH(s_signal_obj[g_signal_value[signal].adc_id], g_signal_value[signal].adc_ch) - g_signal_value[signal].offset)/ADC_FULLSCALE_VALUE;
	*g_signal_value[signal].real_value = g_signal_value[signal].val_control*g_signal_value[signal].gain;
}

void ADC_Temp_Update(signal_type_e signal){
	g_signal_value[signal].ADC_Value = (uint16_t)(Get_ADC_VAL_CH(s_signal_obj[g_signal_value[signal].adc_id],g_signal_value[signal].adc_ch));
	if(g_signal_value[signal].ADC_Value < ADC_TEMP_VALUE__20_DEGREE){
		*g_signal_value[signal].real_value = -20;
	}
	else if(g_signal_value[signal].ADC_Value > ADC_TEMP_VALUE_100_DEGREE){
		*g_signal_value[signal].real_value = 100;
	}
	else {
		uint16_t i = 0;
		for (i= 0; i < TEMP_MAX_VALUE;i++){
			if (g_signal_value[signal].ADC_Value > TempADC_Table[i] && g_signal_value[signal].ADC_Value <= TempADC_Table[i+1]){
				*g_signal_value[signal].real_value = (i - 20) + 1;
			}
		}
	}
}

void Temp_Callback(void){
//	ADC_Temp_Update(TEMP1);
	ADC_Temp_Update(TEMP2);
}
