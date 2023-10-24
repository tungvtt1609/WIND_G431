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
#include "control_dcdc.h"
#include "control_dumpload.h"

extern serial_obj *serial_test_1;
volatile ManagerVariable g_guiValue;
MPPT_PNO mppt_pno;
extern WIND g_wind;

extern float Mean_Iload;
extern float Mean_Vout;
extern float Mean_Vin;
extern uint16_t RPM;

int counter_500ms = 0;

static uint16_t time_power_cal = 0;
static float Energy = 0;

extern uint8_t uart_rx[20];
extern uint8_t uart_rx_cnt;
uint32_t wind_speed = 0;
float wind_conv = 0;
uint8_t uart_tx[8] = {0x02, 0x03, 0x00, 0x00, 0x00, 0x01, 0x84, 0x39};
extern UART_HandleTypeDef huart3;

void serial_init()
{
	serial_test_1 = create_serial(1, 9600, 512, 512);
}

// void initGlobalVariables(void)
//{
//	memset((void *)&g_guiValue, 0, sizeof(g_guiValue));
//	ModbusAddVariable(debug_modbus, MONITOR_VAR_ADDR, (void *)&g_guiValue, sizeof(g_guiValue));
// }
/* MQTT sending variables */
battery_send_data_t battery_send_data;
wind_send_data_t wind_send_data;
system_send_data_t system_send_data;
/* MQTT receiving variables */
battery_received_data_t battery_received_data;
wind_received_data_t wind_received_data;
system_received_data_t system_received_data;
updatefw_received_data_t updatefw_received_data;
datetime_received_data_t datetime_received_data;

void UpdateAllVariable(void)
{
	/* Send data to Modbus */
	//	g_guiValue.guiVin = g_signal_value[VIN].val_control * g_signal_value[VIN].gain;
	//	g_guiValue.guiVout = g_signal_value[VOUT].val_control * g_signal_value[VOUT].gain;
	//	g_guiValue.guiIload = g_signal_value[ILOAD].val_control * g_signal_value[ILOAD].gain;
	//	g_guiValue.guiIdump = g_signal_value[IDUMP].val_control * g_signal_value[IDUMP].gain;
	//	g_guiValue.guiRPM = RPM;
	//	g_guiValue.guiPout = g_wind.vout * g_wind.iwt;
	//	g_guiValue.guiVmpptOut = g_wind.mppt.VmppOut; // mppt_pno.VmppOut;
	//	g_guiValue.guiErrCode = Mean_Iload;
	//	g_guiValue.guiStatus = faultInfo; // DUTY_FINAL;

	/* Send data to 4G */
	battery_send_data.voltage = Mean_Vout * 10;
	battery_send_data.current = Mean_Iload * 10;		   // g_guiValue.guiIload*10;
	battery_send_data.power = Mean_Vout * Mean_Iload * 10; // g_guiValue.guiPout*10;

	wind_send_data.voltage = Mean_Vin * 10;
	wind_send_data.power = battery_send_data.power * 1.0526;
	wind_send_data.speed = wind_conv * 10;

	system_send_data.temperature = *g_signal_value[TEMP2].real_value * 10;
	system_send_data.controller_state = g_current_state;
	system_send_data.error_code = faultInfo;
	system_send_data.rpm_gen = RPM * 10;
	system_send_data.total_energy = Energy;
}

void User_Soft_Reset(void)
{
	NVIC_SystemReset();
}

/**
 * @brief
 *
 * @param str
 * @param pCRC32
 * @return true
 * @return false
 */
bool ConverStringCRC32toU32(char *str, uint32_t *pCRC32)
{
	convert_crc32_t crc32;
	uint8_t data[8];

	for (int i = 0; i < 8; i++)
	{
		if ('0' <= str[i] && str[i] <= '9')
			data[i] = str[i] - '0';
		else if ('A' <= str[i] && str[i] <= 'F')
			data[i] = str[i] - 0x37;
		else if ('a' <= str[i] && str[i] <= 'f')
			data[i] = str[i] - 0x57;
		else
			return false;
	}
	crc32.u0 = data[7];
	crc32.u1 = data[6];
	crc32.u2 = data[5];
	crc32.u3 = data[4];
	crc32.u4 = data[3];
	crc32.u5 = data[2];
	crc32.u6 = data[1];
	crc32.u7 = data[0];
	*pCRC32 = crc32.all;

	return true;
}

uint32_t sending_period = MQTT_SENDING_PERIOD; // 10s
void Send_Data_To_Broker(void)
{
	bool is_failed = false;
	Pin_Func_TurnOff(LED1);
	if (sending_period == 0)
	{
		sending_period = MQTT_SENDING_PERIOD;
		if (MQTT_Transmit_Data((void *)&battery_send_data, BATTERY_DATA) == false)
		{
			is_failed = true;
		}
		else if (MQTT_Transmit_Data((void *)&wind_send_data, WIND_DATA) == false)
		{
			is_failed = true;
		}
		else if (MQTT_Transmit_Data((void *)&system_send_data, SYSTEM_DATA) == false)
		{
			is_failed = true;
		}

		if (is_failed == true)
		{
			Pin_Func_TurnOn(LED1);
			MQTT_Trigger_SIM_Restart();
		}
	}
}

void Wait_Data_From_Broker(void)
{
	mqtt_transferring_data_e received_datatype;
	bool is_failed = false;

	received_datatype = MQTT_Receive_Data(&battery_received_data, &wind_received_data, &system_received_data, &updatefw_received_data);
	if (received_datatype != NONE_DATA)
	{
		switch (received_datatype)
		{
		case BATTERY_DATA:

			/* ACK data to broker */
			if (MQTT_ACK_To_Server((void *)&battery_received_data, BATTERY_DATA))
			{
				;
			}
			else
			{
				is_failed = true;
			}

			break;

		case WIND_DATA:

			/* ACK data to broker */
			if (MQTT_ACK_To_Server((void *)&wind_received_data, WIND_DATA))
			{
				;
			}
			else
			{
				is_failed = true;
			}

			break;

		case SYSTEM_DATA:

			/* ACK data to broker */
			if (MQTT_ACK_To_Server((void *)&system_received_data, SYSTEM_DATA))
			{
				;
			}
			else
			{
				is_failed = true;
			}

			break;

		case UPDATE_FW_DATA:

			/* ACK data to broker */
			if (MQTT_ACK_To_Server((void *)&updatefw_received_data, UPDATE_FW_DATA))
			{
				if ((updatefw_received_data.CRC32[0] != 0xFF) | (updatefw_received_data.CRC32[1] != 0xFF) | (updatefw_received_data.CRC32[2] != 0xFF) | (updatefw_received_data.CRC32[3] != 0xFF))
				{
					/* Write flag to EEPROM */
					uint8_t updatefw_flag = 1;
					EEPROM_Write(EEPROM_UPDATEFIRMWARE_PAGE, EEPROM_UPDATEFIRMWARE_FLAG_OFFSET, &updatefw_flag, 1);
					/* Write CRC32 to EEPROM */
					EEPROM_Write(EEPROM_UPDATEFIRMWARE_PAGE, EEPROM_UPDATEFIRMWARE_CRC32_OFFSET, (uint8_t *)updatefw_received_data.CRC32, 4);

					/* Turn off DC converters before entering bootloader */
					DC_StopPWM_Boost();
					DC_StopPWM_Buck();
					Stop_PWM_DumpLoad();

					/* Jump to bootloader */
					User_Soft_Reset();
				}
			}
			else
			{
				is_failed = true;
			}

			break;

		default:
			break;
		}

		if (is_failed == true)
		{
			MQTT_Trigger_SIM_Restart();
		}
	}
}

/**
 * This is a blocking function, timeout_ms is maximum time of the execution.
 */
void Get_DateTime_From_Server(uint32_t timeout_ms)
{
	MQTT_Get_DateTime(&datetime_received_data, timeout_ms);
}

static uint8_t flag_read = 0;
void test_wind(void)
{
	counter_500ms++;
	if (counter_500ms >= 1000)
	{
		counter_500ms = 0;
		if (flag_read == 0)
		{
			flag_read = 1;
			uart_rx_cnt = 0;
			Pin_Func_TurnOff(CONTROL_485);
			HAL_UART_Transmit(&huart3, uart_tx, 8, 1);
			Pin_Func_TurnOn(CONTROL_485);
		}
		else
		{
			flag_read = 0;
			if (uart_rx_cnt == 7)
			{
				wind_speed = ((uart_rx[3] << 8) | uart_rx[4]);
				wind_conv = (float)(wind_speed) / 10;
			}
		}
	}
}

void Power_Cal(void)
{

	time_power_cal++;
	if (time_power_cal > 1000)
	{
		Energy += battery_send_data.power * 0.001 / 3600;
		time_power_cal = 0;
	}
}
