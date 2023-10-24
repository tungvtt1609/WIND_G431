/*
 * app_main.h
 *
 *  Created on: Apr 14, 2023
 *      Author: TungVT
 */

#ifndef SRC_APP_MAIN_H_
#define SRC_APP_MAIN_H_

#include "driver/com/modbus.h"

#define MAX_BACKGOUND_TASK	7
typedef void (*BackgroundCb)(void);

//extern modbus_slave_obj* debug_modbus;

void AppMainInit();
void AppMainProcess();

#endif /* SRC_APP_MAIN_H_ */
