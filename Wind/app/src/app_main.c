/*
 * app_main.c
 *
 *  Created on: Apr 14, 2023
 *      Author: TungVT
 */

#include "app_main.h"
#include "driver/measure/signal.h"
#include "driver/control/pin_func.h"
#include "control_dcdc.h"
#include "control_dumpload.h"
#include "wind_control.h"
#include "state_machine.h"
#include "interface.h"
#include "porting/serial/serial.h"
#include "porting/timer/timer.h"
#include "driver/com/modbus.h"


modbus_master_obj* debug_modbus;
serial_obj *serial_test_1;
serial_obj *serial_test_2;


static BackgroundCb BackgroundTask[MAX_BACKGOUND_TASK] = {NULL};
static int current_index_background = 0;
static void MainAddBackgroundProcess(BackgroundCb fn);


void AppMainInit(){

		serial_init();
//	  	serial_test_1 = create_serial(1, 115200, 512, 512);
		debug_modbus = create_modbus(118, serial_test_1);
		EC200_Init();
		initGlobalVariables();
		MainAddBackgroundProcess(&ModbusBackground);
		MainAddBackgroundProcess(&UpdateAllVariable);

//		MainAddBackgroundProcess(&TEST);
//		send_data();
//		MainAddBackgroundProcess(&receive_string_data);
		Timer1kHzAddCallback(&Modbus1kHz);

		Pin_Func_Init();
		Signal_Init();
		ControlDcDcBoostInit();
//		ControlDcDcBuckInit();
//		ControlDcDcDumpInit();
		StateInit();


}

void AppMainProcess(){
	int i;
	for(i = 0; i < MAX_BACKGOUND_TASK; i++){
		if(BackgroundTask[i] != NULL){
			BackgroundTask[i]();
		}
	}
}

static void MainAddBackgroundProcess(BackgroundCb fn){
	if(current_index_background < MAX_BACKGOUND_TASK){
		BackgroundTask[current_index_background] = fn;
		current_index_background++;
	}
}
