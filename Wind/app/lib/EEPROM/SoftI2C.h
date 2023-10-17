#ifndef SOFT_I2C_H
#define SOFT_I2C_H

#include <stdint.h>
#include "porting/gpio/gpio.h"

#define SCL_PORT 	PORT_A
#define SDA_PORT 	PORT_A
#define SCL  		PIN_9
#define SDA 		PIN_8

#define ACK 	0
#define NACK 	1


void SCL_OutputMode(void);
void SDA_OutputMode(void);
void SDA_InputMode(void);
void Write_SCL(uint8_t Level);
void Write_SDA(uint8_t Level);
void I2C_Start(void);
void I2C_Stop(void);
uint8_t Check_ACK(void);
void Send_ACK_or_NACK(uint8_t ACK_or_NACK);
void I2C_Write(uint8_t dat);
uint8_t I2C_Read(uint8_t ACK_or_NACK) ;
void ERROR_I2C(void);



#endif
