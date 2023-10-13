///*
// * modbus_master.c
// *
// *  Created on: Oct 7, 2023
// *      Author: duchm20
// */
//
//#include "modbus_master.h"
//#include "driver/control/pin_func.h"
//#include "stdio.h"
//#include "stdlib.h"
//
//
//typedef struct m_modbus_obj{
//	modbus_master_obj modbus;
//	uint8_t slaveID;
//	uint8_t Fnc;
//	uint16_t start_addr;
//	uint16_t data_length;
//	uint16_t modbus_data[100];
//	uint16_t *data_out;
//	uint8_t rdata[RXSIZE];
//	uint8_t sdata[TXSIZE];
//	int rcount, rlength, tick;
//	int scount, slength;
//	void(*Task)(struct m_modbus_obj*);
//}m_modbus_obj;
//
//
//static int current_index_modbus_dev = 0;
//volatile uint32_t buf[8];
//volatile uint32_t read_index;
//volatile float read_conv;
//static m_modbus_obj* modbus_list[MAX_MODBUS_MASTER];
//modbus_master_obj* debug_master;
//
//static void ModbusResetValue(volatile m_modbus_obj *mb_obj);
//static void ModbusSetReDe(uint32_t level);
//static void SendData(m_modbus_obj *mb_obj);
//static void ReceiveData(m_modbus_obj *mb_obj);
//static void ModbusIdle(m_modbus_obj *mb_obj);
//static void ModbusFnc03CreateDataSend(m_modbus_obj *mb_obj);
//static void ModbusFnc03GetDataReceive(m_modbus_obj *mb_obj);
//static void ModbusFnc04CreateDataSend(m_modbus_obj *mb_obj);
//static void ModbusFnc04GetDataReceive(m_modbus_obj *mb_obj);
//
//static void ModbusResetValue(volatile m_modbus_obj *mb_obj)
//{
//    int i;
//    for(i = 0; i < RXSIZE; i++)
//    {
//        mb_obj->rdata[i] = 0;
//    }
//
//    for(i = 0; i < TXSIZE; i++)
//    {
//        mb_obj->sdata[i] = 0;
//    }
//
//    mb_obj->rcount = 0;
//    mb_obj->scount = 0;
//    mb_obj->tick = 0;
//}
//
//static void ModbusSetReDe(uint32_t level)
//{
//	if(1 == level)
//		Pin_Func_TurnOn(CONTROL_485);
//	else
//		Pin_Func_TurnOff(CONTROL_485);
//}
//
//static void SendData(m_modbus_obj *mb_obj){
//	if(mb_obj->tick >= TXTIMEOUT){
//		mb_obj->Task = &ModbusIdle;
//		mb_obj->scount = 0;
//		mb_obj->tick = 0;
//		mb_obj->modbus.Status = Fail;
//	}
//	else if(mb_obj->tick < TXTIMEOUT && mb_obj->scount == mb_obj->slength){
//		switch(mb_obj->Fnc){
//			case 04:
//				mb_obj->Task = &ReceiveData;
//				mb_obj->modbus.Status = InProcess;
//				mb_obj->tick = 0;
//				mb_obj->rcount = 0;
//				break;
//			case 03:
//
//				mb_obj->Task = &ReceiveData;
//				if(mb_obj->modbus.SetReDe != NULL)
//					mb_obj->modbus.SetReDe(1);
//				mb_obj->modbus.Status = InProcess;
//				mb_obj->tick = 0;
//				mb_obj->rcount = 0;
//				break;
//			default:
//				mb_obj->Task = &ModbusIdle;
//				mb_obj->modbus.Status = Fail;
//				break;
//		}
//	}
//}
//
//static void ReceiveData(m_modbus_obj *mb_obj){
//	if(mb_obj->tick >= RXTIMEOUT){
//		mb_obj->Task = &ModbusIdle;
//		mb_obj->scount = 0;
//		mb_obj->tick = 0;
//		mb_obj->modbus.Status = Fail;
//	}
//	else if(mb_obj->tick < RXTIMEOUT && mb_obj->rcount == mb_obj->rlength){
//		switch(mb_obj->Fnc){
//			case 04:
//				mb_obj->Task = &ModbusFnc04GetDataReceive;
//				mb_obj->modbus.Status = InProcess;
//				break;
//			case 03:
//				mb_obj->Task = &ModbusFnc03GetDataReceive;
//				if(mb_obj->modbus.SetReDe != NULL)
//					mb_obj->modbus.SetReDe(0);
//				mb_obj->modbus.Status = InProcess;
//				break;
//			default:
//				mb_obj->Task = &ModbusIdle;
//				mb_obj->modbus.Status = Fail;
//				break;
//		}
//	}
//}
//
//static void ModbusIdle(m_modbus_obj *mb_obj){
//
//}
//
//void Modbus1kHz_master(void){
//	int index_modbus_dev = 0;
//	for(index_modbus_dev = 0; index_modbus_dev < current_index_modbus_dev; index_modbus_dev++){
//		m_modbus_obj *modbus = modbus_list[index_modbus_dev];
//		modbus->tick++;
//	}
//}
//
//static void ModbusFnc04CreateDataSend(m_modbus_obj *mb_obj){
//	uint16_t crc;
//	mb_obj->sdata[0] = mb_obj->slaveID;
//	mb_obj->sdata[1] = mb_obj->Fnc;
//	mb_obj->sdata[2] = ((mb_obj->start_addr) >> 8) & 0xFF;
//	mb_obj->sdata[3] = (mb_obj->start_addr) & 0xFF;
//	mb_obj->sdata[4] = ((mb_obj->data_length) >> 8) & 0xFF;
//	mb_obj->sdata[5] = (mb_obj->data_length) & 0xFF;
//	crc = calcCRC(mb_obj->sdata, 6);
//	mb_obj->sdata[6] = ((crc) >> 8) & 0xFF;
//	mb_obj->sdata[7] = ((crc)) & 0xFF;
//	if(mb_obj->modbus.SetReDe != NULL)
//		mb_obj->modbus.SetReDe(0);
//	mb_obj->slength = 8;
//	mb_obj->rlength = 5 + mb_obj->data_length * 2;
//	mb_obj->tick  = 0;
//	mb_obj->scount = 0;
//	mb_obj->Task = &SendData;
//}
//
//static void ModbusFnc04GetDataReceive(m_modbus_obj *mb_obj){
//	uint16_t crc;
//	uint16_t i;
//	crc = calcCRC(mb_obj->rdata, mb_obj->rlength - 2);
//	if(mb_obj->slaveID != mb_obj->rdata[0] || mb_obj->Fnc != mb_obj->rdata[1] || (mb_obj->rdata[mb_obj->rcount -2]  != (crc >> 8 & 0xFF)) || (mb_obj->rdata[mb_obj->rcount -1]  != (crc & 0xFF))){
//		mb_obj->Task = &ModbusIdle;
//		mb_obj->modbus.Status = Fail;
//		return;
//	}
//	for(i = 0; i < mb_obj->data_length; i++){
//		*(mb_obj->data_out + i) = ((mb_obj->rdata[2*i + 3] & 0x00FF) << 8) | (mb_obj->rdata[2*i + 4] & 0x00FF);
//	}
//	if(mb_obj->modbus.SetReDe != NULL)
//		mb_obj->modbus.SetReDe(1);
//	mb_obj->Task = &ModbusIdle;
//	mb_obj->modbus.Status = Success;
//
//	return;
//}
//
//static void ModbusFnc03CreateDataSend(m_modbus_obj *mb_obj){
//	uint16_t crc;
//	mb_obj->sdata[0] = 0x02;
//	mb_obj->sdata[1] = 0x03;
//	mb_obj->sdata[2] = 0x00;
//	mb_obj->sdata[3] = 0x00;
//	mb_obj->sdata[4] = 0x00;
//	mb_obj->sdata[5] = 0x01;
//	crc = calcCRC(mb_obj->sdata, 6);
//	mb_obj->sdata[6] = 0x84;
//	mb_obj->sdata[7] = 0x39;
//	if(mb_obj->modbus.SetReDe != NULL)
//		mb_obj->modbus.SetReDe(0);
//	mb_obj->slength = 8;
//	mb_obj->rlength = 5 + mb_obj->data_length * 2;
//	mb_obj->tick  = 0;
//	mb_obj->scount = 0;
//	mb_obj->Task = &SendData;
//}
//
//
//static void ModbusFnc03GetDataReceive(m_modbus_obj *mb_obj)
//{
//	uint16_t crc;
//	uint16_t i;
//
//	crc = calcCRC(mb_obj->rdata, mb_obj->rlength - 2);
//
//	if(mb_obj->slaveID 	!= mb_obj->rdata[0] || mb_obj->Fnc != mb_obj->rdata[1] || (mb_obj->rdata[mb_obj->rcount - 2] != (crc >> 8 & 0xFF)) || (mb_obj->rdata[mb_obj->rcount - 1] != (crc & 0xFF)))
//	{
//        mb_obj->Task = &ModbusIdle;
//        mb_obj->modbus.Status = Fail;
//		return;
//	}
//
//	for(i = 0 ; i < mb_obj->data_length; i++)
//	{
//		*(mb_obj->data_out + i) = ((mb_obj->rdata[2*i + 3] & 0x00FF) << 8) | (mb_obj->rdata[2*i + 4] & 0x00FF);
////		read_index = ((mb_obj->rdata[2*i + 3] & 0x00FF) << 8) | (mb_obj->rdata[2*i + 4] & 0x00FF);
//
//	}
//	read_index = (((mb_obj->rdata[3]) & 0x00FF) << 8) | ((mb_obj->rdata[4]) & 0x00FF);
//	read_conv = (float)read_index/10;
//	if(mb_obj->modbus.SetReDe != NULL)
//		mb_obj->modbus.SetReDe(1);
//    mb_obj->Task = &ModbusIdle;
//    mb_obj->modbus.Status = Success;
//	return;
//}
//
//bool ModbusMasterReadFnc03(modbus_master_obj *obj, uint16_t slave_ID, uint16_t start_addr, uint16_t data_length, void *data){
//	if(obj != null && obj->Status != InProcess && (data_length < DATA_LENGTH_RX_SIZE)){
//		m_modbus_obj *modbus = (m_modbus_obj*)obj;
//		modbus->slaveID = 0x02;
//		modbus->Fnc = 0x03;
//		modbus->start_addr = start_addr;
//		modbus->data_length = data_length;
//		modbus->data_out = data;
//		modbus->Task = &ModbusFnc03CreateDataSend;
//		modbus->modbus.Status = InProcess;
//		return true;
//	}
//	else {
//		return false;
//	}
//}
//
//bool ModbusMasterReadFnc04(modbus_master_obj *obj, uint16_t slave_ID, uint16_t start_addr, uint16_t data_length, void *data){
//	if(obj != null && obj->Status != InProcess && (data_length < DATA_LENGTH_RX_SIZE)){
//		m_modbus_obj *modbus = (m_modbus_obj*)obj;
//		modbus->slaveID = slave_ID;
//		modbus->Fnc = 0x03;
//		modbus->start_addr = start_addr;
//		modbus->data_length = data_length;
//		modbus->data_out = data;
//		modbus->Task = &ModbusFnc04CreateDataSend;
//		modbus->modbus.Status = InProcess;
//		return true;
//	}
//	else {
//		return false;
//	}
//}
//
//
//void TUNG_TEST(void){
//	ModbusMasterReadFnc03(debug_master, 2, 0, 1, buf);
//}
//
//modbus_master_obj* create_modbus_master(serial_obj *serial){
//	if(current_index_modbus_dev >= MAX_MODBUS_MASTER){
//		while(1);
//	}
//	m_modbus_obj *modbus = malloc(sizeof(m_modbus_obj));
//	memset(modbus, 0, sizeof(m_modbus_obj));
//	if(modbus != null){
//		modbus->modbus.id = current_index_modbus_dev;
//		modbus->modbus.serial_port = serial;
//		modbus_list[current_index_modbus_dev] = modbus;
//		ModbusResetValue(modbus);
//		current_index_modbus_dev++;
//		modbus->Task = &ModbusIdle;
//		modbus->modbus.SetReDe = ModbusSetReDe;
//		modbus->modbus.Status = Idle;
//		return &(modbus->modbus);
//	}
//	return null;
//}
//
//
//void ModbusMasterBackground(void){
//	int index_modbus_dev = 0;
//	for(index_modbus_dev = 0; index_modbus_dev < current_index_modbus_dev; index_modbus_dev++){
//		if(modbus_list[index_modbus_dev]->Task != null){
//			modbus_list[index_modbus_dev]->Task(modbus_list[index_modbus_dev]);
//		}
//		m_modbus_obj *modbus = modbus_list[index_modbus_dev];
//		SerialHandle(modbus->modbus.serial_port);
//		while(modbus->modbus.serial_port->length_received > 0 && modbus->Task == (void*)&ReceiveData){
//			if(modbus->rcount >= RXSIZE){
//				modbus->rcount = 0;
//			}
//			SerialReadBuf(modbus->modbus.serial_port, &(modbus->rdata[modbus->rcount]), 1);
//			modbus->rcount++;
//			modbus->tick = 0;
//		}
//		send:
//		if(modbus->scount < modbus->slength && modbus->Task == &SendData){
//			bool ret;
//			ret = SerialSendBuf(modbus->modbus.serial_port, &modbus->sdata[modbus->scount], 1);
//			if(ret == true){
//				modbus->tick = 0;
//				modbus->scount++;
//				goto send;
//			}
//			else{
//				break;
//			}
//		}
//	}
//}
