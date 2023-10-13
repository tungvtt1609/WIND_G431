/*
 * modbus.c
 *
 *  Created on: Sep 12, 2023
 *      Author: TungVT
 */

#include "modbus.h"
#include "driver/control/pin_func.h"
#include "stdlib.h"

#define RXSIZE          100
#define TXSIZE          100
#define RXTIMEOUT       500   //ms
#define TXTIMEOUT       500   //ms

#define MAX_VAR_SHOW        10

typedef struct
{
    uint16_t start_addr;
    void *data;
    uint16_t len;
}DataModBus;

typedef struct m_modbus_obj
{
	modbus_slave_obj modbus;
    uint8_t rdata[RXSIZE];
    uint8_t sdata[TXSIZE];
    int rcount, tick;
    int scount, sSend;
    void(*RcvTask)(struct m_modbus_obj*);
}m_modbus_obj;

static int current_index_list_var = 0;
static DataModBus data_list[MAX_VAR_SHOW];

static int current_index_modbus_dev = 0;
static m_modbus_obj* modbus_list[MAX_MODBUS_DEV];


static void ModbusSendData(m_modbus_obj *mb_obj);
static void ModbusCreateData(m_modbus_obj *mb_obj);
static void ModbusSetData(m_modbus_obj *mb_obj);
static void ModbusResetValue(volatile m_modbus_obj *mb_obj);
static void ModbusGetData(m_modbus_obj *mb_obj);


static void ModbusResetValue(volatile m_modbus_obj *mb_obj)
{
    int i;
    for(i = 0; i < RXSIZE; i++)
    {
        mb_obj->rdata[i] = 0;
    }

    for(i = 0; i < TXSIZE; i++)
    {
        mb_obj->sdata[i] = 0;
    }

    mb_obj->rcount = 0;
    mb_obj->scount = 0;
    mb_obj->tick = 0;
}

static void ModbusGetData(m_modbus_obj *mb_obj) // Get Function
{
    uint16_t crc;
    if(mb_obj->tick >= RXTIMEOUT)
    {
        mb_obj->tick = 0;
        if(mb_obj->rcount >= 8)
        {
            if( mb_obj->rdata[0] == mb_obj->modbus.id)
            {
                switch(mb_obj->rdata[1])
                {
                case 0x03:
                    if(mb_obj->rcount >= 8)
                    {
                        crc = calcCRC(mb_obj->rdata, 6);
                        if((mb_obj->rdata[6] == (crc >> 8 & 0xFF)) && (mb_obj->rdata[7] == (crc & 0xFF)))
                        {
                            mb_obj->RcvTask = &ModbusCreateData;
                            mb_obj->tick = 0;
                        }
                    }
                    break;
                case 0x10:
                    crc = calcCRC(mb_obj->rdata, mb_obj->rcount - 2);
                    if((mb_obj->rdata[mb_obj->rcount - 2] == (crc >> 8 & 0xFF)) && (mb_obj->rdata[mb_obj->rcount - 1] == (crc & 0xFF)))
                    {
                        mb_obj->RcvTask = &ModbusSetData;
                        mb_obj->tick = 0;
                    }
                    break;
                default:
                    mb_obj->tick = 0;
                }
            }
            mb_obj->rcount = 0;
        }
        mb_obj->rcount = 0;
    }
}
static void ModbusSetData(m_modbus_obj *mb_obj)
{
    int i, j, k;
    uint16_t crc;
    uint16_t addrData;
    uint16_t sizeData;
    uint16_t *ptr_data = NULL;
    for(i = 0; i < 6; i++)
    {
        mb_obj->sdata[i] = mb_obj->rdata[i];
    }

    addrData = mb_obj->rdata[2] << 8 | mb_obj->rdata[3];
    sizeData = mb_obj->rdata[4] << 8 | mb_obj->rdata[5];
    if(sizeData > (TXSIZE - 6))
    {
        mb_obj->RcvTask = &ModbusGetData;
        mb_obj->tick = 0;
    }
    for(j = 0; j < MAX_VAR_SHOW; j++)
    {
        if(data_list[j].data != NULL)
        {
            if(addrData >= data_list[j].start_addr && addrData <= data_list[j].start_addr + data_list[j].len)
            {
                ptr_data = (uint16_t *)data_list[j].data;
                addrData = addrData - data_list[j].start_addr;
                break;
            }
        }
    }
    if(ptr_data == NULL)
    {
        mb_obj->RcvTask = &ModbusGetData;
        return;
    }

    k = 7;

    for(j = 0; j < sizeData; j++)
    {
        *((uint16_t*)ptr_data + addrData) = (uint16_t)(((uint16_t)mb_obj->rdata[k] << 8) + (uint16_t)mb_obj->rdata[k + 1]);
        k = k + 2;
        addrData += 1;
    }

    crc = calcCRC(mb_obj->sdata, i);
    mb_obj->sdata[i++] = (crc >> 8) & 0xFF;
    mb_obj->sdata[i++] = crc & 0xFF;

    mb_obj->sSend = i;
    if(mb_obj->modbus.SetReDe != NULL)
    	mb_obj->modbus.SetReDe(0);
    mb_obj->RcvTask = &ModbusSendData;
    mb_obj->tick = 0;
    mb_obj->scount = 0;

}
static void ModbusCreateData(m_modbus_obj *mb_obj)
{
    int i, j;
    uint16_t crc;
    uint16_t addrData;
    uint16_t sizeData;
    uint16_t *ptr_data = NULL;
    for(i = 0; i < 2; i++)
    {
        mb_obj->sdata[i] = mb_obj->rdata[i];
    }

    addrData = mb_obj->rdata[2] << 8 | mb_obj->rdata[3];
    sizeData = mb_obj->rdata[4] << 8 | mb_obj->rdata[5];

    mb_obj->sdata[i++] = sizeData * 2;
    if(sizeData * 2 > (TXSIZE - 6))
    {
        mb_obj->RcvTask = &ModbusGetData;
        return;           // over len buff
    }
    for(j = 0; j < MAX_VAR_SHOW; j++)
    {
        if(data_list[j].data != NULL)
        {
            if(addrData >= data_list[j].start_addr && addrData < data_list[j].start_addr + data_list[j].len)
            {
                ptr_data = (uint16_t *)data_list[j].data;
                addrData = addrData - data_list[j].start_addr;
                break;
            }
        }
    }
    if(ptr_data == NULL)
    {
        mb_obj->RcvTask = &ModbusGetData;
        return;
    }

    for(j = 0; j < sizeData; j++)
    {
        memcpy(&mb_obj->sdata[i], (uint16_t*)ptr_data + addrData, 2);
        uint8_t fw = 0;
        fw = mb_obj->sdata[i];
        mb_obj->sdata[i] = mb_obj->sdata[i + 1];
        mb_obj->sdata[i + 1] = fw;
        i += 2;
        addrData += 1;
    }

    crc = calcCRC(mb_obj->sdata, i);
    mb_obj->sdata[i++] = (crc >> 8) & 0xFF;
    mb_obj->sdata[i++] = crc & 0xFF;
    if(mb_obj->modbus.SetReDe != NULL)
    	mb_obj->modbus.SetReDe(0);
    mb_obj->sSend = i;
    mb_obj->RcvTask = &ModbusSendData;
    mb_obj->tick = 0;
    mb_obj->scount = 0;
}

static void ModbusSendData(m_modbus_obj *mb_obj) // Task 1
{
    if(mb_obj->tick >= TXTIMEOUT)
    {
    	if(mb_obj->modbus.SetReDe != NULL)
    		mb_obj->modbus.SetReDe(1);
        mb_obj->RcvTask = &ModbusGetData;
        mb_obj->scount = 0;
        mb_obj->tick = 0;
    }
}

static void ModbusSetReDe(uint32_t level)
{
	if(1 == level)
		Pin_Func_TurnOn(CONTROL_485);
	else
		Pin_Func_TurnOff(CONTROL_485);
}

modbus_slave_obj* create_modbus(uint16_t id, serial_obj *serial)
{
	if(current_index_modbus_dev >= MAX_MODBUS_DEV)
	{
		while(1); // no modbus more than
	}
	m_modbus_obj *modbus = malloc(sizeof(m_modbus_obj));
	memset(modbus, 0, sizeof(m_modbus_obj));
	if(modbus != NULL)
	{
		modbus->modbus.id = id;
		modbus->modbus.serial_port = serial;
		modbus_list[current_index_modbus_dev] = modbus;
		ModbusResetValue(modbus);
		current_index_modbus_dev++;
		modbus->RcvTask = &ModbusGetData;
		modbus->modbus.SetReDe = ModbusSetReDe;
		return &(modbus->modbus);
	}
	return NULL;
}


void Modbus1kHz(void)
{
	int index_modbus_dev = 0;
	for(index_modbus_dev = 0; index_modbus_dev < current_index_modbus_dev; index_modbus_dev++)
	{
		m_modbus_obj *modbus = modbus_list[index_modbus_dev];
		modbus->tick++;
	}
}

void ModbusBackground(void)
{
	int index_modbus_dev = 0;
	for(index_modbus_dev = 0; index_modbus_dev < current_index_modbus_dev; index_modbus_dev++)
	{
        if(modbus_list[index_modbus_dev]->RcvTask != NULL)
        {
        	modbus_list[index_modbus_dev]->RcvTask(modbus_list[index_modbus_dev]);
        }

        m_modbus_obj *modbus = modbus_list[index_modbus_dev];
		SerialHandle(modbus->modbus.serial_port);
		while(modbus->modbus.serial_port->length_received > 0 && modbus->RcvTask == (void*)&ModbusGetData)	 // check if a char has been received
		{
			if(modbus->rcount >= RXSIZE)
			{
				modbus->rcount = 0;
			}
			SerialReadBuf(modbus->modbus.serial_port,
							&(modbus->rdata[modbus->rcount]),
							1);
			modbus->rcount++;
			modbus->tick = 0;
		}
		send:
		if(modbus->scount < modbus->sSend && modbus->RcvTask == (void*)&ModbusSendData)
		{
			bool ret;
			ret = SerialSendBuf(modbus->modbus.serial_port, &modbus->sdata[modbus->scount], 1);
			if(ret == true)
			{
				modbus->tick = 0;
				modbus->scount++;
				goto send;
			}
			else
			{
				break;
			}
		}

	}
}


void ModbusAddVariable(modbus_slave_obj *obj, uint16_t addr, void *data, uint16_t len)
{
    if(current_index_list_var >= MAX_VAR_SHOW) return;
    data_list[current_index_list_var].start_addr = addr;
    data_list[current_index_list_var].data = data;
    data_list[current_index_list_var].len = len;
    current_index_list_var++;
}
