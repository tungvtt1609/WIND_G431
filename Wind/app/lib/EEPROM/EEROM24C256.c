#include "EEROM24C256.h"

void I2C_Mem_Write(uint8_t Device_Addr, uint16_t Mem_Addr, uint8_t *Data, uint16_t Length)
{
	I2C_Start();
	I2C_Write((Device_Addr << 1));		 // Write Device Address and function as Write
	I2C_Write((uint8_t)(Mem_Addr >> 8)); // Write to location of memory
	I2C_Write((uint8_t)(Mem_Addr));
	for (int i = 0; i < Length; i++)
	{
		I2C_Write(Data[i]);
	}
	I2C_Stop();
}

void I2C_Mem_Read(uint8_t Device_Addr, uint16_t Mem_Addr, uint8_t *Data, uint16_t Length)
{
	I2C_Start();
	I2C_Write((Device_Addr << 1));		 // Writing Device Address and function as Write
	I2C_Write((uint8_t)(Mem_Addr >> 8)); // Write to location of memory
	I2C_Write((uint8_t)(Mem_Addr));
	I2C_Start();
	I2C_Write((Device_Addr << 1) | 0x01); // Writing Device Address and function as Read

	for (int i = 0; i < (Length - 1); i++)
	{
		Data[i] = I2C_Read(ACK);
	}
	Data[Length - 1] = I2C_Read(NACK); // Reading last byte
	I2C_Stop();
}

uint16_t Data_Seperate(uint16_t offset, uint16_t size)
{
	if ((offset + size) > PAGE_SIZE)
	{
		return (PAGE_SIZE - offset);
	}

	else
	{
		return size;
	}
}

void EEPROM_Write(uint16_t Page, uint16_t Offset, uint8_t *data, uint16_t Size)
{
	// calculate the start page and the end page
	uint16_t startPage = Page;
	uint16_t endPage = Page + ((Offset + Size) / PAGE_SIZE); // Exam : Size = 50, Offset = 20 => 2 Page needed

	// number of pages to be written
	uint16_t numofpages = (endPage - startPage) + 1;
	uint16_t pos = 0;

	// write the data
	for (int i = 0; i < numofpages; i++)
	{
		uint16_t MemAddress = startPage << WORD_NUMBER_BYTE | Offset; // Exam: We are using page 3rd, write to byte 30th => (3<<6)|30
		uint16_t bytesremaining = Data_Seperate(Offset, Size);		  // calculate the remaining bytes to be written

		I2C_Mem_Write(EEPROM_ADDR, MemAddress, &data[pos], bytesremaining);
		startPage += 1;				  // increment the page, so that a new page address can be selected for further write
		Offset = 0;					  // since we will be writing to a new page, so offset will be 0
		Size = Size - bytesremaining; // The rest of size bytes
		pos += bytesremaining;		  // Update possision
		HAL_Delay(20);				  // Write cycle delay (20ms)
	}
}

void EEPROM_Read(uint16_t Page, uint16_t Offset, uint8_t *data, uint16_t Size)
{
	// calculate the start page and the end page
	uint16_t startPage = Page;
	uint16_t endPage = Page + ((Offset + Size) / PAGE_SIZE); // Exam : Size = 50, Offset = 20 => 2 Page needed

	// number of pages to be written
	uint16_t numofpages = (endPage - startPage) + 1;
	uint16_t pos = 0;

	// write the data
	for (int i = 0; i < numofpages; i++)
	{
		uint16_t MemAddress = startPage << WORD_NUMBER_BYTE | Offset; // Exam: We are using page 3rd, write to byte 30th => (3<<6)|30
		uint16_t bytesremaining = Data_Seperate(Offset, Size);		  // calculate the remaining bytes to be written

		I2C_Mem_Read(EEPROM_ADDR, MemAddress, &data[pos], bytesremaining);
		startPage += 1;				  // increment the page, so that a new page address can be selected for further write
		Offset = 0;					  // since we will be writing to a new page, so offset will be 0
		Size = Size - bytesremaining; // The rest of size bytes
		pos += bytesremaining;		  // Update possision
		HAL_Delay(20);				  // Write cycle delay (20ms)
	}
}

void EEPROM_Errase(uint16_t Page, uint16_t Offset, uint16_t Size)
{
	uint8_t Errase[Size];
	memset(Errase, 0xff, Size);

	// calculate the start page and the end page
	uint16_t startPage = Page;
	uint16_t endPage = Page + ((Offset + Size) / PAGE_SIZE); // Exam : Size = 50, Offset = 20 => 2 Page needed

	// number of pages to be written
	uint16_t numofpages = (endPage - startPage) + 1;
	uint16_t pos = 0;

	// write the data
	for (int i = 0; i < numofpages; i++)
	{
		uint16_t MemAddress = startPage << WORD_NUMBER_BYTE | Offset; // Exam: We are using page 3rd, write to byte 30th => (3<<6)|30
		uint16_t bytesremaining = Data_Seperate(Offset, Size);		  // calculate the remaining bytes to be written

		I2C_Mem_Write(EEPROM_ADDR, MemAddress, &Errase[pos], bytesremaining);
		startPage += 1;				  // increment the page, so that a new page address can be selected for further write
		Offset = 0;					  // since we will be writing to a new page, so offset will be 0
		Size = Size - bytesremaining; // The rest of size bytes
		pos += bytesremaining;		  // Update possision
		HAL_Delay(10);				  // Write cycle delay (5ms)
	}
}
