/*
 * fifo.h
 *
 *  Created on: Sep 12, 2023
 *      Author: TungVT
 */

#ifndef COMMON_FIFO_H_
#define COMMON_FIFO_H_

#include <string.h>
#include "stdint.h"
#include "stdbool.h"


typedef struct fifo
{

    uint16_t read_idx;
    uint16_t write_idx;
    uint8_t *buffer;
    uint16_t data_len;
    uint16_t size;
    uint16_t size_element;
    bool (*Put)(struct fifo* fifo, void* byte);
    bool (*Puts)(struct fifo* fifo, void* buffer, uint16_t length);
    bool (*Peak)(struct fifo* fifo, void *data);
    bool (*Peaks)(struct fifo* fifo, void *data, uint16_t len);
    uint16_t (*Get)(struct fifo* fifo, void* byte);
    bool (*Gets)(struct fifo* fifo, void* buffer, uint16_t length);
    bool (*Clear)(struct fifo* fifo);
    uint16_t (*GetAvaiableSpace)(struct fifo* fifo);
}fifo;

fifo* create_fifo(uint16_t size, uint16_t element_size);

#endif /* COMMON_FIFO_H_ */
