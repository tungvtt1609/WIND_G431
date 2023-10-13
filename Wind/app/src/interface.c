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
#include "wind_control.h"
#include "lib/EC200_mqtt_application.h"

extern serial_obj *serial_test_1;
extern UART_HandleTypeDef huart3;
volatile ManagerVariable g_guiValue;
MPPT_PNO mppt_pno;
extern WIND g_wind;
extern float DUTY_FINAL;

int counter_500ms = 0;

extern uint8_t uart_rx[20];
extern uint8_t uart_rx_cnt;
uint32_t wind_speed  = 0;
float wind_conv = 0;
uint8_t uart_tx[8]={0x02, 0x03, 0x00, 0x00,0x00,0x01,0x84,0x39};


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
	g_guiValue.guiPout	=	g_wind.vout*g_wind.iwt;
	g_guiValue.guiVmpptOut 	=g_wind.mppt.VmppOut; //mppt_pno.VmppOut;
	g_guiValue.guiErrCode = faultInfo;
}

static uint8_t flag_read = 0;
void test_wind(void){
	counter_500ms++;
	if(counter_500ms >= 1000){
		counter_500ms = 0;
		if (flag_read ==0) {
			flag_read = 1;
			uart_rx_cnt = 0;
			Pin_Func_TurnOff(CONTROL_485);
			HAL_UART_Transmit(&huart3, uart_tx, 8, 1);
			Pin_Func_TurnOn(CONTROL_485);
		}
		else{
			flag_read = 0;
			if(uart_rx_cnt == 7){
			wind_speed = (uart_rx[3]<<8|uart_rx[4]);
			wind_conv =(float)(wind_speed)/10;
			}
		}

	}
}

void TEST(void){
	battery_send_data_t battery_send_data={BAT_NORMAL,100,50,10,25};
	if(EC200_MQTT_ConnectToServer())
	{
		if(MQTT_Transmit_Data(&battery_send_data, BATTERY_DATA))
		{
			Pin_Func_TurnOff(LED1);
		}
		else
		{
			MQTT_Trigger_SIM_Restart();
		}

	}
}


