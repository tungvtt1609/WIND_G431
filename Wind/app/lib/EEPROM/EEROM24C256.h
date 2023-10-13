#ifndef EEROM_H
#define EEROM_H

#include <stdint.h>
#include <string.h>
//#include "SysTick_Delay.h"
#include "softI2C.h"

#define EEPROM_ADDR 0x50 //0101 0000
#define PAGE_SIZE  64
#define PAGE_NUMBER 512
#define WORD_NUMBER_BYTE 6 // because 9 bits for WORD_NUMBER_PAGE ,6 bits for WORD_NUMBER_BYTE

void I2C_Mem_Write(uint8_t Device_Addr,uint16_t Mem_Addr,uint8_t *Data,uint16_t Length);
void I2C_Mem_Read(uint8_t Device_Addr,uint16_t Mem_Addr,uint8_t *Data,uint16_t Length);
uint16_t Data_Seperate(uint16_t offset,uint16_t size);
void EEPROM_Write(uint16_t Page,uint16_t Offset,uint8_t *data,uint16_t Size);
void EEPROM_Read(uint16_t Page,uint16_t Offset,uint8_t *data,uint16_t Size);
void EEPROM_Errase(uint16_t Page,uint16_t Offset,uint16_t Size);

#endif
