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
#include "control_xylanh.h"
#include "wind_control.h"
#include "state_machine.h"
#include "interface.h"
#include "porting/serial/serial.h"
#include "porting/timer/timer.h"
#include "driver/com/modbus.h"
#include "lib/EC200_mqtt_application.h"
#include "Frequency.h"
//#include "driver/measure/Temperature.h"

//modbus_slave_obj* debug_modbus;
serial_obj *serial_test_2;
extern volatile uint32_t buf[8];


static BackgroundCb BackgroundTask[MAX_BACKGOUND_TASK] = {NULL};
static int current_index_background = 0;
static void MainAddBackgroundProcess(BackgroundCb fn);


void AppMainInit(){

//		serial_test_2 = create_serial(1, 9600, 512, 512);
//		debug_modbus = create_modbus(2, serial_test_2);
//		MainAddBackgroundProcess(&ModbusBackground);
//		Timer1kHzAddCallback(&Modbus1kHz);
//		initGlobalVariables();

		EC200_Init();

		MainAddBackgroundProcess(&UpdateAllVariable);
		Timer1kHzAddCallback(&test_wind);
		Timer1kHzAddCallback(&EC200_Time_Base_1ms);

		Pin_Func_Init();
		Signal_Init();

		Vol_Controller_Init();
		Curr_Controller_Init();
		MPPT_Controller_Init();


		ControlDcDcBuckBoostInit();
		ControlDcDcDumpInit();
		Timer1kHzAddCallback(&Cal_Freq);
		Timer1kHzAddCallback(&Temp_Callback);
		Timer1kHzAddCallback(&Dumpload_controller);
		Timer1kHzAddCallback(&Power_Cal);
		Control_Xylanh_Init();

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
