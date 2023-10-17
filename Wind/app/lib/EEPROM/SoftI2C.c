#include "softI2C.h"

uint8_t test_eeprom = 0;

void SCL_OutputMode(void)
{
	GPIO_SetPinOut(SCL_PORT, SCL);
	Write_SCL(1);
}

void SDA_OutputMode(void)
{
	GPIO_SetPinOut(SDA_PORT, SDA);
	Write_SDA(1);
}

void SDA_InputMode(void)
{
	GPIO_SetPinIn(SDA_PORT, SDA, NO_PULL);
}

void Write_SCL(uint8_t Level)
{
	GPIO_WritePin(SCL_PORT, SCL, Level);
}

void Write_SDA(uint8_t Level)
{
	GPIO_WritePin(SDA_PORT, SDA, Level);
}

void I2C_Start(void)
{
	SDA_OutputMode();
	SCL_OutputMode();
	Write_SCL(1);
	Write_SDA(1);
	HAL_Delay(1);
	Write_SDA(0);
	HAL_Delay(1);
	Write_SCL(0);
	HAL_Delay(1);
}

void I2C_Stop(void)
{

	Write_SDA(0);
	HAL_Delay(1);
	Write_SCL(1);
	HAL_Delay(1);
	Write_SDA(1);
	HAL_Delay(1);
}

uint8_t Check_ACK(void)
{
	uint8_t Status_SDA;

	SDA_InputMode(); // Setup InputMode for SDA
	Write_SCL(1);
	HAL_Delay(1);
	// Read ACK
	Status_SDA = GPIO_ReadPin(SDA_PORT, SDA);
	// Reset SDA back to OutputMode
	Write_SCL(0);
	HAL_Delay(1);
	SDA_OutputMode();

	return Status_SDA;
}

void Send_ACK_or_NACK(uint8_t ACK_or_NACK)
{

	Write_SDA(ACK_or_NACK);
	HAL_Delay(1);
	Write_SCL(1);
	HAL_Delay(1);
	Write_SCL(0);
}

uint8_t flag_eeprom;
void I2C_Write(uint8_t dat)
{
	for (int i = 0; i < 8; i++)
	{
		if ((dat & 0x80) == 0x80)
			Write_SDA(1);
		else
			Write_SDA(0);
		HAL_Delay(1);
		Write_SCL(1);
		HAL_Delay(1);
		Write_SCL(0);
		HAL_Delay(1); // could be wrong
		dat <<= 1;
	}

	// ckeck ACK
	flag_eeprom = Check_ACK();

	if (flag_eeprom == ACK) // Received a bit ACK
	{
		test_eeprom++; // Oke
	}

	if (flag_eeprom == NACK)
		ERROR_I2C(); // Received a bit NACK,go to infinite loop
}

// ACK_or_NACK is the parameter that send to slave
// ACK_or_NACK could be ACK or NACK

uint8_t I2C_Read(uint8_t ACK_or_NACK)
{
	uint8_t Receiv_Data = 0x00;
	uint8_t Temp;
	SDA_InputMode();
	for (int i = 0; i < 8; i++)
	{
		Write_SCL(1);
		HAL_Delay(1);
		Temp = GPIO_ReadPin(SDA_PORT, SDA);
		Receiv_Data = Receiv_Data << 1;
		Receiv_Data = Receiv_Data | Temp;
		Write_SCL(0);
		HAL_Delay(1);
	}
	SDA_OutputMode(); // swap to output mode
	Send_ACK_or_NACK(ACK_or_NACK);

	return Receiv_Data;
}

void ERROR_I2C(void)
{
	;
}
