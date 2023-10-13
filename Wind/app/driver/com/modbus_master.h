///*
// * modbus_master.h
// *
// *  Created on: Oct 7, 2023
// *      Author: duchm20
// */
//
//#ifndef DRIVER_COM_MODBUS_MASTER_H_
//#define DRIVER_COM_MODBUS_MASTER_H_
//
//#include "serial/serial.h"
//#include "types.h"
//
//#define MAX_MODBUS_MASTER		1
//
//#define RXSIZE				30
//#define TXSIZE				30
//#define DATA_LENGTH_RX_SIZE (RXSIZE/2-10)
//#define DATA_LENGTH_TX_SIZE (TXSIZE/2-10)
//#define RXTIMEOUT			500
//#define TXTIMEOUT			500
//
//#define MAX_VAR_SHOW		6
//
//typedef enum {
//	Idle = 0,
//	Success = 1,
//	Fail = 2,
//	InProcess
//}ModbusmasterStatus_t;
//
//typedef struct modbus_master_obj{
//	void (*SetReDe)(uint32_t val);
//	uint16_t id;
//	serial_obj* serial_port;
//	ModbusmasterStatus_t Status;
//}modbus_master_obj;
//
//modbus_master_obj* create_modbus_master(serial_obj *serial);
//
//void Modbus1kHz_master(void);
//void ModbusMasterBackground(void);
//bool ModbusMasterReadFnc03(modbus_master_obj *obj, uint16_t slave_ID, uint16_t start_addr, uint16_t data_length, void *data);
//bool ModbusMasterReadFnc04(modbus_master_obj *obj, uint16_t slave_ID, uint16_t start_addr, uint16_t data_length, void *data);
//void TUNG_TEST(void);
//void TUNG_GET_DATA_PROCESS(void);
//
//#endif /* DRIVER_COM_MODBUS_MASTER_H_ */
