/*
 * serial.h
 *
 *  Created on: Sep 12, 2023
 *      Author: TungVT
 */

#ifndef PORTING_SERIAL_SERIAL_H_
#define PORTING_SERIAL_SERIAL_H_

#include "common/common.h"
#include "stdbool.h"
#include "common/fifo.h"
#include "types.h"


#define DEV_SERIAL			2

typedef struct serial_obj
{
	uint16_t port;
    uint32_t baudrate;
    uint32_t tx_size;
    uint32_t rx_size;
    uint16_t length_received;
    uint16_t length_send;
}serial_obj;

serial_obj* create_serial(uint16_t port, uint32_t baudrate, uint16_t tx_size, uint16_t rx_size);
bool SerialSendBuf(struct serial_obj* serial, uint8_t *data, uint16_t len);
uint16_t SerialReadBuf(struct serial_obj* serial, uint8_t *data, uint16_t len);
void SerialHandle(struct serial_obj* serial);
void SendStringUart2(uint8_t *dat);


#endif /* PORTING_SERIAL_SERIAL_H_ */
