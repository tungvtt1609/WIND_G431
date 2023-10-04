/*
 * fifo.c
 *
 *  Created on: Sep 12, 2023
 *      Author: TungVT
 */


#include "fifo.h"
#include "math.h"
#include <stdlib.h>

static uint16_t FIFO_GetAvaiableSpace(fifo* fifo);
static bool FIFO_Put(fifo* fifo, void* byte);
static bool FIFO_Puts(fifo* fifo, void* buffer, uint16_t length);
static bool FIFO_Peak(fifo* fifo, void *data);
static bool FIFO_Peaks(fifo* fifo, void *data, uint16_t len);
static uint16_t FIFO_Get(fifo* fifo, void* byte);
static bool FIFO_Gets(fifo* fifo, void* buffer, uint16_t length);
static bool FIFO_Clear(fifo* fifo);

fifo* create_fifo(uint16_t size, uint16_t element_size)
{
	fifo* ff;
	ff = (fifo*)malloc(sizeof(fifo));
	if(ff != NULL)
	{
		memset(ff, 0, sizeof(fifo));
		ff->size = size;
		ff->size_element = element_size;
		ff->Put = FIFO_Put;
		ff->Puts = FIFO_Puts;
		ff->Peak = FIFO_Peak;
		ff->Peaks = FIFO_Peaks;
		ff->Get = FIFO_Get;
		ff->Gets = FIFO_Gets;
		ff->Clear = FIFO_Clear;
		ff->GetAvaiableSpace = FIFO_GetAvaiableSpace;
		ff->buffer = malloc(size * element_size);
		if(ff->buffer != NULL) return ff;
		else
			free(ff);
	}
	return NULL;
}

uint16_t FIFO_GetAvaiableSpace(fifo* fifo)
{
	if(fifo == NULL)
		return 0;
    return (fifo->size - fifo->data_len);
}

static bool FIFO_Put(fifo* fifo, void* byte)
{
	if(fifo == NULL)
		return false;
    if(fifo->data_len < fifo->size)
    {
      memcpy(&(fifo->buffer[fifo->write_idx*fifo->size_element]), byte, fifo->size_element);
      fifo->write_idx++;
      fifo->data_len++;
      if(fifo->write_idx == fifo->size)
        fifo->write_idx = 0;
      return true;
    }
    return false;
}

bool FIFO_Puts(fifo* fifo, void* buffer, uint16_t length   )
{
    uint16_t i;
    uint8_t* ptr = (uint8_t*)buffer;
	if(fifo == NULL)
		return false;
    if( length <= FIFO_GetAvaiableSpace(fifo))
    {
        for(i = 0; i < length; i++)
        if(FIFO_Put(fifo, ptr + i*fifo->size_element) == 0)
            return false;
    }
    else
        return false;
    return true;
}

bool FIFO_Peak(fifo* fifo, void* data)
{
	if(fifo == NULL)
		return false;
    if(fifo->data_len > 0)       // it empty
    {
    	memcpy(data, &(fifo->buffer[fifo->read_idx*fifo->size_element]), fifo->size_element);
      return true;
    }
    else
        return false;
}

bool FIFO_Peaks(fifo* fifo, void* data, uint16_t len)
{
	uint16_t index;
	uint8_t* ptr_data = (uint8_t*)data;
	if(fifo == NULL)
		return false;
    if(fifo->data_len >= len)       // it empty
    {
    	for(index = 0; index < len; index++)
    		memcpy(ptr_data + index*fifo->size_element, &(fifo->buffer[(fifo->read_idx + index)*fifo->size_element]), fifo->size_element);
      return true;
    }
    else
        return false;
}

uint16_t FIFO_Get(fifo* fifo, void* byte)
{
	if(fifo == NULL)
		return 0;
    if(fifo->data_len > 0)       // it empty
    {
    	memcpy(byte, &(fifo->buffer[fifo->read_idx*fifo->size_element]), fifo->size_element);
      fifo->read_idx++;
      fifo->data_len--;
      if(fifo->read_idx == fifo->size)
        fifo->read_idx = 0;
      if(fifo->data_len == 0) fifo->write_idx = fifo->read_idx;
      return 1;
    }
    return 0;
}

bool FIFO_Gets(fifo* fifo, void* buffer, uint16_t length)
{
	uint16_t i;
	if(fifo == NULL)
		return false;
    if(length <= fifo->data_len)
    {
        for(i = 0; i < length; i++)
          FIFO_Get(fifo, (buffer + i*fifo->size_element));
        return true;
    }
    return false;
}

bool FIFO_Clear(fifo* fifo)
{
	if(fifo == NULL)
		return false;
	fifo->data_len = 0;
	fifo->read_idx = fifo->write_idx;
	return true;
}
