/*
 * interface.c
 *
 *  Created on: Sep 12, 2023
 *      Author: TungVT
 */

#include "interface.h"
#include "stdbool.h"
#include "driver/com/modbus.h"
#include "app_main.h"
#include "state_machine.h"
#include "string.h"
#include "serial/serial.h"

extern serial_obj *serial_test_1;
volatile ManagerVariable g_guiValue;
MPPT_PNO mppt_pno;

void serial_init(){
	serial_test_1 = create_serial(1, 9600, 512, 512);
}

void initGlobalVariables(void){
	memset((void*)&g_guiValue, 0, sizeof(g_guiValue));
	ModbusAddVariable(debug_modbus, MONITOR_VAR_ADDR, (void*)&g_guiValue, sizeof(g_guiValue));
}

void UpdateAllVariable(void){
	g_guiValue.guiVin 	= g_signal_value[VIN].val_control 	* g_signal_value[VIN].gain;
	g_guiValue.guiVout	= g_signal_value[VOUT].val_control 	* g_signal_value[VOUT].gain;
	g_guiValue.guiIload	= g_signal_value[ILOAD].val_control * g_signal_value[ILOAD].gain;
	g_guiValue.guiIdump	= g_signal_value[IDUMP].val_control * g_signal_value[IDUMP].gain;
	g_guiValue.guiPwind = g_guiValue.guiVin * g_guiValue.guiIload;
	g_guiValue.guiDref 	= mppt_pno.D_ref;
	g_guiValue.guiErrCode = faultInfo;
	g_guiValue.guiStatus = g_current_state;

//	g_guiValue.guiVin 	= 5;
//	g_guiValue.guiVout	= 4;
//	g_guiValue.guiIload	= 3;
//	g_guiValue.guiIdump = 22;
//	g_guiValue.guiPwind = 2;
//	g_guiValue.guiDref 	= 1;
//	g_guiValue.guiErrCode = 6;
//	g_guiValue.guiStatus = 7;
}

