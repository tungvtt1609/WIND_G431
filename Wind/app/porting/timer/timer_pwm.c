/*
 * timer_pwm.c
 *
 *  Created on: Sep 12, 2023
 *      Author: TungVT
 */

#include "timer_pwm.h"
#include "string.h"

pTIMER_ADVANCE_EVENT_INT_HANDLE Tim1CCIsr 	= NULL;
pTIMER_ADVANCE_EVENT_INT_HANDLE Tim8CCIsr 	= NULL;
pTIMER_ADVANCE_EVENT_INT_HANDLE Tim15CCIsr 	= NULL;

typedef struct timeradvanceTable_t {
	TIMER_ADVANCE_ID_E id;
	TIM_TypeDef* timeradvance;
}timeradvanceTable_t;

static timeradvanceTable_t timeradvanceTable[3] = {
		{TIMER_ADVANCE_ID_1, (TIM_TypeDef*)TIM1},
		{TIMER_ADVANCE_ID_2, (TIM_TypeDef*)TIM8},
		{TIMER_ADVANCE_ID_3, (TIM_TypeDef*)TIM15},
};

timer_advance_obj_t* create_advance_timer(TIMER_ADVANCE_ID_E id, uint32_t Frequency){
	timer_advance_obj_t* timer_advance_obj= malloc(sizeof(timer_advance_obj_t));
	memset(timer_advance_obj, 0 ,sizeof(timer_advance_obj_t));

	if(id >= 0 && id <= 2){
		timer_advance_obj->advtimer 		= timeradvanceTable[id].timeradvance;
		timer_advance_obj->advance_timer_id = (TIMER_ADVANCE_ID_E)id;
	}
	else{
		timer_advance_obj->advtimer 		= NULL;
		timer_advance_obj->advance_timer_id = 0;
	}
	float PeriodTemp = 0;
	if(TIM_GetCounterMode(timer_advance_obj) == 0){
		PeriodTemp = ((float)TIMER_ADVANCE_TIMEBASE_CLOCK/(float)Frequency) + 0.7;
	}
	else{
		PeriodTemp = (float)TIMER_ADVANCE_TIMEBASE_CLOCK/(float)(Frequency*2) + 0.7;
	}
	TIM_SetAutoReload(timer_advance_obj , (uint32_t)PeriodTemp);
	timer_advance_obj->TimerPeriod = (uint32_t)PeriodTemp;
	return timer_advance_obj;
}
