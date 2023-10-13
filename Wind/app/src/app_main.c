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
//#include "driver/com/modbus_master.h"
#include "lib/EC200_mqtt_application.h"

modbus_slave_obj* debug_modbus;
//extern modbus_master_obj* debug_master;
serial_obj *serial_test_2;
extern volatile uint32_t buf[8];


static BackgroundCb BackgroundTask[MAX_BACKGOUND_TASK] = {NULL};
static int current_index_background = 0;
static void MainAddBackgroundProcess(BackgroundCb fn);


void AppMainInit(){

//		serial_init();
		serial_test_2 = create_serial(1, 9600, 512, 512);
//		debug_modbus = create_modbus(2, serial_test_2);
//		debug_master = create_modbus_master(serial_test_2);
		initGlobalVariables();
		EC200_Init();
//		MainAddBackgroundProcess(&ModbusBackground);
//		MainAddBackgroundProcess(&ModbusMasterBackground);
		MainAddBackgroundProcess(&UpdateAllVariable);
//		Timer1kHzAddCallback(&Modbus1kHz);
//		Timer1kHzAddCallback(&Modbus1kHz_master);
//		Timer1kHzAddCallback(&TEST);
		Timer1kHzAddCallback(&test_wind);
		Timer1kHzAddCallback(&EC200_Time_Base_1ms);

		Pin_Func_Init();
		Signal_Init();

		Vol_Controller_Init();
		Curr_Controller_Init();
		MPPT_Controller_Init();

//		ControlDcDcBoostInit();
		ControlDcDcBuckBoostInit();
//		ControlDcDcDumpInit();
//		Control_Xylanh_Init();

//
//		TestBoost();
//		TestBuck();
//		Test_PWM_Xylanh();
//		TestDump();
//		Pin_Func_TurnOff(LED1);
//		Pin_Func_TurnOff(LED2);
//		Pin_Func_TurnOff(LED3);
//		Pin_Func_TurnOn(RELAY1);
//		Pin_Func_TurnOn(RELAY2);
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
