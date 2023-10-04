/*
 * interface.h
 *
 *  Created on: Sep 12, 2023
 *      Author: TungVT
 */

#ifndef SRC_INTERFACE_H_
#define SRC_INTERFACE_H_

#include "string.h"
#include "driver/control/pin_func.h"
#include "driver/measure/signal.h"
#include "wind_mppt.h"
#include "porting/serial/serial.h"

//#define RXSIZE			100
//#define TXSIZE			100

typedef struct{
	float guiVin;
	float guiVout;
	float guiIload;
	float guiIdump;
	float guiPwind;
	float guiDref;
	float guiErrCode;
	float guiStatus;
}ManagerVariable;

typedef enum{
	MONITOR_VAR_ADDR = 0,
	LOG_VAR_LIST_ADDR = 500,
	DEVICE_INFO_ADDR = 1000,
}VAR_MODBUS_ADDR;



void send_data(void);
void read_data(void);
void serial_init();
void initGlobalVariables(void);
void UpdateAllVariable(void);
void INF_Process(void);
void INF_Time1kHz(void);
void TEST(void);

#endif /* SRC_INTERFACE_H_ */
