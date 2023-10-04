/*
 * modbus.h
 *
 *  Created on: Sep 12, 2023
 *      Author: TungVT
 */

#ifndef DRIVER_COM_MODBUS_H_
#define DRIVER_COM_MODBUS_H_


#include "porting/serial/serial.h"
#include "types.h"




#define MAX_MODBUS_DEV 	2

typedef struct modbus_obj
{
	uint16_t id;
	serial_obj* serial_port;
	void (*SetReDe)(uint32_t val);
}modbus_master_obj;



modbus_master_obj* create_modbus(uint16_t id, serial_obj *serial);
void ModbusAddVariable(modbus_master_obj *obj, uint16_t addr, void *data, uint16_t len);
void Modbus1kHz(void);
void ModbusBackground(void);


#endif /* DRIVER_COM_MODBUS_H_ */
