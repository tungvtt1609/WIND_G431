/*
 * timer_pwm.h
 *
 *  Created on: Sep 12, 2023
 *      Author: TungVT
 */

#ifndef PORTING_TIMER_TIMER_PWM_H_
#define PORTING_TIMER_TIMER_PWM_H_


#ifndef   __STATIC_INLINE
	#define __STATIC_INLINE                  static inline
#endif

#include "main.h"
#include "stdlib.h"
#include "stdint.h"

typedef void (*pTIMER_ADVANCE_EVENT_INT_HANDLE)();

extern pTIMER_ADVANCE_EVENT_INT_HANDLE Tim1CCIsr;
extern pTIMER_ADVANCE_EVENT_INT_HANDLE Tim8CCIsr;
extern pTIMER_ADVANCE_EVENT_INT_HANDLE Tim15CCIsr;


#ifndef   TIMER_ADVANCE_TIMEBASE_CLOCK
	#define TIMER_ADVANCE_TIMEBASE_CLOCK 170000000
#endif

typedef enum TIMER_ADVANCE_ID_E {
	TIMER_ADVANCE_ID_1 = 0,		//Tim1
	TIMER_ADVANCE_ID_2,			//Tim8
	TIMER_ADVANCE_ID_3,			//Tim15
	TIMER_ADVANCE_ID_MAX,
}TIMER_ADVANCE_ID_E;

typedef struct timer_advance_obj_t {
	TIMER_ADVANCE_ID_E advance_timer_id;
	uint16_t TimerPeriod;
	TIM_TypeDef* advtimer;
}timer_advance_obj_t;

timer_advance_obj_t* create_advance_timer(TIMER_ADVANCE_ID_E id, uint32_t Frequency);

__STATIC_INLINE void SetCompareCH1(timer_advance_obj_t *advancetimerobjX, uint32_t CompareValue)
{
	WRITE_REG(advancetimerobjX->advtimer->CCR1, CompareValue);
}

__STATIC_INLINE void SetCompareCH1N(timer_advance_obj_t *advancetimerobjX, uint32_t CompareValue)
{
	WRITE_REG(advancetimerobjX->advtimer->CCR1, CompareValue);
}

__STATIC_INLINE void SetCompareCH2(timer_advance_obj_t *advancetimerobjX, uint32_t CompareValue)
{
	WRITE_REG(advancetimerobjX->advtimer->CCR2, CompareValue);
}

__STATIC_INLINE void SetCompareCH2N(timer_advance_obj_t *advancetimerobjX, uint32_t CompareValue)
{
	WRITE_REG(advancetimerobjX->advtimer->CCR2, CompareValue);
}


__STATIC_INLINE void SetCompareCH3(timer_advance_obj_t *advancetimerobjX, uint32_t CompareValue)
{
	WRITE_REG(advancetimerobjX->advtimer->CCR3, CompareValue);
}

__STATIC_INLINE void SetCompareCH3N(timer_advance_obj_t *advancetimerobjX, uint32_t CompareValue)
{
	WRITE_REG(advancetimerobjX->advtimer->CCR3, CompareValue);
}

__STATIC_INLINE void SetCompareCH4(timer_advance_obj_t *advancetimerobjX, uint32_t CompareValue)
{
	WRITE_REG(advancetimerobjX->advtimer->CCR4, CompareValue);
}

__STATIC_INLINE void SetCompareCH4N(timer_advance_obj_t *advancetimerobjX, uint32_t CompareValue)
{
	WRITE_REG(advancetimerobjX->advtimer->CCR4, CompareValue);
}

__STATIC_INLINE void DisableTimerChanel(timer_advance_obj_t *advancetimerobjX)
{
	CLEAR_BIT(advancetimerobjX->advtimer->BDTR, TIM_BDTR_MOE);
}
__STATIC_INLINE void EnableTimerChanel(timer_advance_obj_t *advancetimerobjX)
{
	SET_BIT(advancetimerobjX->advtimer->BDTR, TIM_BDTR_MOE);
}


//=======================================-START_ENABLE_OUTPUT-===============================================================//
__STATIC_INLINE void EnableCH1Output(timer_advance_obj_t *advancetimerobjX)
{
	SET_BIT(advancetimerobjX->advtimer->CCER, LL_TIM_CHANNEL_CH1);
}

__STATIC_INLINE void EnableCH1NOutput(timer_advance_obj_t *advancetimerobjX)
{
	SET_BIT(advancetimerobjX->advtimer->CCER, LL_TIM_CHANNEL_CH1N);
}


__STATIC_INLINE void EnableCH2Output(timer_advance_obj_t *advancetimerobjX)
{
	SET_BIT(advancetimerobjX->advtimer->CCER, LL_TIM_CHANNEL_CH2);
}

__STATIC_INLINE void EnableCH2NOutput(timer_advance_obj_t *advancetimerobjX)
{
	SET_BIT(advancetimerobjX->advtimer->CCER, LL_TIM_CHANNEL_CH2N);
}

__STATIC_INLINE void EnableCH3NOutput(timer_advance_obj_t *advancetimerobjX)
{
	SET_BIT(advancetimerobjX->advtimer->CCER, LL_TIM_CHANNEL_CH3N);
}

__STATIC_INLINE void EnableCH4Output(timer_advance_obj_t *advancetimerobjX)
{
	SET_BIT(advancetimerobjX->advtimer->CCER, LL_TIM_CHANNEL_CH4);
}

__STATIC_INLINE void EnableCH4NOutput(timer_advance_obj_t *advancetimerobjX)
{
	SET_BIT(advancetimerobjX->advtimer->CCER, LL_TIM_CHANNEL_CH4N);
}


__STATIC_INLINE void EnableCH1AndCH4Outputs(timer_advance_obj_t *advancetimerobjX)
{
	SET_BIT(advancetimerobjX->advtimer->CCER, LL_TIM_CHANNEL_CH1|LL_TIM_CHANNEL_CH4);
}

__STATIC_INLINE void EnableCH1AndCH3NOutputs(timer_advance_obj_t *advancetimerobjX)
{
	SET_BIT(advancetimerobjX->advtimer->CCER, LL_TIM_CHANNEL_CH1|LL_TIM_CHANNEL_CH3N);
}

__STATIC_INLINE void EnableCH1NAndCH2NOutputs(timer_advance_obj_t *advancetimerobjX)
{
	SET_BIT(advancetimerobjX->advtimer->CCER, LL_TIM_CHANNEL_CH1N|LL_TIM_CHANNEL_CH2N);
}

__STATIC_INLINE void EnableAllChanelOutputs(timer_advance_obj_t *advancetimerobjX)
{
	SET_BIT(advancetimerobjX->advtimer->CCER, LL_TIM_CHANNEL_CH1|LL_TIM_CHANNEL_CH1N|LL_TIM_CHANNEL_CH2|LL_TIM_CHANNEL_CH2N|LL_TIM_CHANNEL_CH3
					   |LL_TIM_CHANNEL_CH3N|LL_TIM_CHANNEL_CH4|LL_TIM_CHANNEL_CH4N);
}
//=======================================-END_ENABLE_OUTPUT-===============================================================//

//=======================================-START_DISABLE_OUTPUT-===============================================================//
__STATIC_INLINE void DisableCH1Output(timer_advance_obj_t *advancetimerobjX)
{
	CLEAR_BIT(advancetimerobjX->advtimer->CCER, LL_TIM_CHANNEL_CH1);
}

__STATIC_INLINE void DisableCH1NOutput(timer_advance_obj_t *advancetimerobjX)
{
	CLEAR_BIT(advancetimerobjX->advtimer->CCER, LL_TIM_CHANNEL_CH1N);
}

__STATIC_INLINE void DisableCH2Output(timer_advance_obj_t *advancetimerobjX)
{
	CLEAR_BIT(advancetimerobjX->advtimer->CCER, LL_TIM_CHANNEL_CH2);
}

__STATIC_INLINE void DisableCH2NOutput(timer_advance_obj_t *advancetimerobjX)
{
	CLEAR_BIT(advancetimerobjX->advtimer->CCER, LL_TIM_CHANNEL_CH2N);
}

__STATIC_INLINE void DisableCH3NOutput(timer_advance_obj_t *advancetimerobjX)
{
	CLEAR_BIT(advancetimerobjX->advtimer->CCER, LL_TIM_CHANNEL_CH3N);
}

__STATIC_INLINE void DisableCH4Output(timer_advance_obj_t *advancetimerobjX)
{
	CLEAR_BIT(advancetimerobjX->advtimer->CCER, LL_TIM_CHANNEL_CH4);
}

__STATIC_INLINE void DisableCH4NOutput(timer_advance_obj_t *advancetimerobjX)
{
	CLEAR_BIT(advancetimerobjX->advtimer->CCER, LL_TIM_CHANNEL_CH4N);
}

__STATIC_INLINE void DisableCH1AndCH2Outputs(timer_advance_obj_t *advancetimerobjX)
{
	CLEAR_BIT(advancetimerobjX->advtimer->CCER, LL_TIM_CHANNEL_CH1|LL_TIM_CHANNEL_CH2);
}

__STATIC_INLINE void DisableCH1AndCH3NOutputs(timer_advance_obj_t *advancetimerobjX)
{
	CLEAR_BIT(advancetimerobjX->advtimer->CCER, LL_TIM_CHANNEL_CH1|LL_TIM_CHANNEL_CH3N);
}

__STATIC_INLINE void DisableCH1NAndCH2NOutputs(timer_advance_obj_t *advancetimerobjX)
{
	CLEAR_BIT(advancetimerobjX->advtimer->CCER, LL_TIM_CHANNEL_CH1N|LL_TIM_CHANNEL_CH2N);
}

__STATIC_INLINE void DisableAllChanelOutputs(timer_advance_obj_t *advancetimerobjX)
{
	CLEAR_BIT(advancetimerobjX->advtimer->CCER, LL_TIM_CHANNEL_CH1|LL_TIM_CHANNEL_CH1N|LL_TIM_CHANNEL_CH2|LL_TIM_CHANNEL_CH2N|LL_TIM_CHANNEL_CH3
						 |LL_TIM_CHANNEL_CH3N|LL_TIM_CHANNEL_CH4|LL_TIM_CHANNEL_CH4N);
}
//=======================================-END_DISABLE_OUTPUT-===============================================================//

__STATIC_INLINE void EnableCntTimer(timer_advance_obj_t *advancetimerobjX)
{
	SET_BIT(advancetimerobjX->advtimer->CR1, TIM_CR1_CEN);
}

__STATIC_INLINE void TIM_EnableIT_CC3(timer_advance_obj_t *advancetimerobjX)
{
  SET_BIT(advancetimerobjX->advtimer->DIER, TIM_DIER_CC3IE);
}

__STATIC_INLINE void TIM_ClearFlag_CC3(timer_advance_obj_t *advancetimerobjX)
{
  WRITE_REG(advancetimerobjX->advtimer->SR, ~(TIM_SR_CC3IF));
}

__STATIC_INLINE void TIM_EnableIT_BRK(timer_advance_obj_t *advancetimerobjX)
{
  SET_BIT(advancetimerobjX->advtimer->DIER, TIM_DIER_BIE);
}

__STATIC_INLINE void TIM_ClearFlag_BRK(TIM_TypeDef *TIMx)
{
  WRITE_REG(TIMx->SR, ~(TIM_SR_BIF));
}

//__STATIC_INLINE void TIM_ClearFlag_BRK2(timer_advance_obj_t *advancetimerobjX)
//{
//  WRITE_REG(advancetimerobjX->advtimer->SR, ~(TIM_SR_B2IF));
//}

__STATIC_INLINE uint32_t TIM_GetCounterMode(timer_advance_obj_t *advancetimerobjX)
{
  uint32_t counter_mode;

  counter_mode = (uint32_t)(READ_BIT(advancetimerobjX->advtimer->CR1, TIM_CR1_CMS));
  return counter_mode;
}

__STATIC_INLINE void TIM_SetAutoReload(timer_advance_obj_t *advancetimerobjX, uint32_t AutoReload)
{
  WRITE_REG(advancetimerobjX->advtimer->ARR, AutoReload);
}


__STATIC_INLINE uint32_t LL_TIM_GetCounter_Freq(timer_advance_obj_t *advancetimerobjX)
{
  return (uint32_t)(READ_REG(advancetimerobjX->advtimer->CNT));
}

__STATIC_INLINE void LL_TIM_SetCounter_Freq(timer_advance_obj_t *advancetimerobjX, uint32_t Counter)
{
  WRITE_REG(advancetimerobjX->advtimer->CNT, Counter);
}

#endif /* PORTING_TIMER_TIMER_PWM_H_ */
