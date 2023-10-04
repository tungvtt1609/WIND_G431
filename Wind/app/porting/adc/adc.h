/*
 * adc.h
 *
 *  Created on: 9 Mar 2023
 *      Author: Thanh Tung
 */

#ifndef PORTING_ADC_ADC_H_
#define PORTING_ADC_ADC_H_

#include "types.h"

typedef enum ADC_ID_e{
	ADC_ID_1 = 0,
	ADC_ID_2,
	ADC_ID_MAX

}ADC_ID_e;

typedef enum ADC_CH_e{
	ADC_CH_1 = 0,
	ADC_CH_2,
	ADC_CH_3,
	ADC_CH_4,
	ADC_CH_5,
	ADC_CH_6,
	ADC_CH_7,
	ADC_CH_8,
	ADC_CH_9,
	ADC_CH_10,
	ADC_CH_11,
	ADC_CH_12,
	ADC_CH_13,
	ADC_CH_14,
	ADC_CH_15,
	ADC_CH_16,
	ADC_CH_17,
	ADC_CH_MAX
}ADC_CH_e;

typedef enum RANK_e
{
	RANK_1 = 0,
	RANK_2,
	RANK_3,
	RANK_4,
	RANK_5,
	RANK_6,
	RANK_7,
	RANK_8,
	RANK_9,
	RANK_10,
	RANK_11,
	RANK_12,
	RANK_13,
	RANK_14,
	RANK_15,
	RANK_MAX
}RANK_e;

typedef struct adc_obj_t
{
	uint16_t val[4];
	ADC_ID_e adc_id;
	uint16_t sample_num;
	uint16_t ch_num;
}adc_obj_t;

adc_obj_t* ADC_DMA_CREATE(ADC_ID_e id);
uint16_t Get_ADC_VAL_CH(struct adc_obj_t* adc_obj, ADC_CH_e ch);

#endif /* PORTING_ADC_ADC_H_ */
