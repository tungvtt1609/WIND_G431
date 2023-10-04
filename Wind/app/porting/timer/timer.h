/*
 * timer.h
 *
 *  Created on: 16 Mar 2023
 *      Author: Thanh Tung
 */

#ifndef PORTING_TIMER_TIMER_H_
#define PORTING_TIMER_TIMER_H_

#include "types.h"
#include "stdbool.h"

typedef void (*TimerfuncCb)(void);
extern long long g_system_tick;

void TimerTick1kHz();
bool Timer1kHzAddCallback(TimerfuncCb fn);



#endif /* PORTING_TIMER_TIMER_H_ */
