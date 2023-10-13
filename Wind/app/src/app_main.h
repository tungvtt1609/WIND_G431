/*
 * app_main.h
 *
 *  Created on: Apr 14, 2023
 *      Author: TungVT
 */

#ifndef SRC_APP_MAIN_H_
#define SRC_APP_MAIN_H_

#include "driver/com/modbus.h"
#include "lib/EC200_mqtt_application

#define MAX_BACKGOUND_TASK	6
typedef void (*BackgroundCb)(void);

extern modbus_master_obj* debug_modbus;

void AppMainInit();
void AppMainProcess();

#endif /* SRC_APP_MAIN_H_ */
