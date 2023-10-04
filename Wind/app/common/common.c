/*
 * common.c
 *
 *  Created on: Sep 12, 2023
 *      Author: TungVT
 */

#include "common.h"
#include <stdio.h>
#include <stdint.h>

uint16_t calcCRC(uint8_t *buf, uint16_t len)
{
    int i, j;
    unsigned int temp, temp2, flag;
    temp = 0xFFFF;
    for (i = 0; i < len; i++)
    {
        temp = temp ^ buf[i];
        for (j = 1; j <= 8; j++)
        {
            flag = temp & 0x0001;
            temp >>=1;
            if (flag)
                temp ^= 0xA001;
        }
    }
    // Reverse byte order.
    temp2 = temp >> 8;
    temp = (temp << 8) | temp2;
    temp &= 0xFFFF;
    // the returned value is already swapped
    // crcLo byte is first & crcHi byte is last
    return temp;
}

int crc16(const unsigned char *ptr, int len)
{
	int crc = 0;
	unsigned char i;
	int index = 0;
	while (len-- != 0)
	{
		for (i = 0x80; i != 0; i /= 2)
		{
			if ((crc & 0x8000) != 0)
			{
				crc *= 2;
				crc ^= 0x1021;
			}
			else
				crc *= 2;
			if ((ptr[index] & i) != 0) crc ^= 0x1021;
		}
		index++;
	}
	return crc & 0x0000ffff;
}

void memcpy_ss(void* dest, void* src, int len)
{
	unsigned char *_dest = dest;
	unsigned char *_src = src;
	int index = 0;
	while(index < len)
	{
		_dest[index] = _src[index];
		index++;
	}
}
