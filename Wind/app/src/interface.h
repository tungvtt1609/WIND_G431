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

#define EEPROM_UPDATEFIRMWARE_PAGE 0
#define EEPROM_UPDATEFIRMWARE_FLAG_OFFSET 4
#define EEPROM_UPDATEFIRMWARE_CRC32_OFFSET 0

//#define RXSIZE			100
//#define TXSIZE			100

typedef struct{
	float guiVin;
	float guiVout;
	float guiIload;
	float guiIdump;
	float guiRPM;
	float guiPout;
	float guiVmpptOut;
	float guiErrCode;
	float guiStatus;
}ManagerVariable;

typedef enum{
	MONITOR_VAR_ADDR = 0,
	LOG_VAR_LIST_ADDR = 500,
	DEVICE_INFO_ADDR = 1000,
}VAR_MODBUS_ADDR;

typedef union
{
	struct
	{
		uint8_t u0 : 4;
		uint8_t u1 : 4;
		uint8_t u2 : 4;
		uint8_t u3 : 4;
		uint8_t u4 : 4;
		uint8_t u5 : 4;
		uint8_t u6 : 4;
		uint8_t u7 : 4;
	};
	uint32_t all;
} convert_crc32_t;

//void send_data(void);
void read_data(void);
void serial_init();
void initGlobalVariables(void);
void UpdateAllVariable(void);
void Send_Data_To_Broker(void);
void Wait_Data_From_Broker(void);
void Get_DateTime_From_Server(uint32_t timeout_ms);
bool ConverStringCRC32toU32(char *str, uint32_t *pCRC32);
void test_wind(void);
void Power_Cal(void);

#endif /* SRC_INTERFACE_H_ */
