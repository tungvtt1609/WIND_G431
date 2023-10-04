/*
 * adc.c
 *
 *  Created on: 9 Mar 2023
 *      Author: Thanh Tung
 */

#include "adc.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#include "stm32g4xx_hal.h"

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
//extern ADC_HandleTypeDef hadc3;
//extern ADC_HandleTypeDef hadc4;
//extern ADC_HandleTypeDef hadc5;

typedef struct adc_ch_rank_t {
	ADC_CH_e ch;
	uint32_t rank;
}adc_ch_rank_t;

typedef struct adc_id_inf_t {
	ADC_ID_e id;
	ADC_HandleTypeDef* hadc;
	uint16_t sample_num;
	adc_ch_rank_t ch_rank[ADC_CH_MAX];
}adc_id_inf_t;

static adc_id_inf_t adc_id_inf[2] = {
	{ADC_ID_1, &hadc1, 1, {{ ADC_CH_1, RANK_1}, { ADC_CH_2, RANK_2}, {ADC_CH_3}, {ADC_CH_4}, {ADC_CH_5, RANK_3}, {ADC_CH_6}, {ADC_CH_7}, {ADC_CH_8},{ADC_CH_9},{ADC_CH_10},{ADC_CH_11},{ADC_CH_12},{ADC_CH_13},{ADC_CH_14},{ADC_CH_15,RANK_4},{ADC_CH_16},{ADC_CH_17} }},
	{ADC_ID_2, &hadc2, 1, {{ ADC_CH_1}, { ADC_CH_2}, {ADC_CH_3, RANK_1}, {ADC_CH_4, RANK_2}, {ADC_CH_5}, {ADC_CH_6}, {ADC_CH_7}, {ADC_CH_8},{ADC_CH_9},{ADC_CH_10},{ADC_CH_11},{ADC_CH_12},{ADC_CH_13, RANK_3},{ADC_CH_14},{ADC_CH_15},{ADC_CH_16},{ADC_CH_17,RANK_4} }},
};

adc_obj_t* ADC_DMA_CREATE(ADC_ID_e id){
	uint16_t num_adc_val = 0;
	ADC_HandleTypeDef* hadc = adc_id_inf[id].hadc;

	adc_obj_t* adc_obj = malloc(sizeof(adc_obj_t));
	memset(adc_obj, 0, sizeof(adc_obj_t));

	if(NULL == adc_obj){
		free(adc_obj);
		return NULL;
	}

	adc_obj->adc_id = id;
	adc_obj->sample_num = adc_id_inf[id].sample_num;
	adc_obj->ch_num = hadc->Init.NbrOfConversion;
	num_adc_val = adc_obj->sample_num * adc_obj->ch_num;

	HAL_ADCEx_Calibration_Start(hadc, ADC_SINGLE_ENDED);
	HAL_ADC_Start_DMA(hadc, (uint32_t *)&adc_obj->val, num_adc_val);
	return adc_obj;
}
uint16_t Get_ADC_VAL_CH(struct adc_obj_t* adc_obj, ADC_CH_e ch){
	uint32_t sum = 0;
	uint16_t i = 0, val = 0;
	for(i = 0; i < adc_obj->sample_num; i++){
		sum += adc_obj->val[i * adc_obj->ch_num + adc_id_inf[adc_obj->adc_id].ch_rank[ch].rank];
	}
	val = sum / adc_obj->sample_num;
	return val;
}
