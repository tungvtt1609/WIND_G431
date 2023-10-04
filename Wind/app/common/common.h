/*
 * common.h
 *
 *  Created on: Sep 12, 2023
 *      Author: TungVT
 */

#ifndef COMMON_COMMON_H_
#define COMMON_COMMON_H_

#include "stdint.h"

uint16_t calcCRC(uint8_t *buf, uint16_t len);
int crc16(const unsigned char *ptr, int len);
void memcpy_ss(void* dest, void* src, int len);

#endif /* COMMON_COMMON_H_ */
