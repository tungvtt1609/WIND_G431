/*
 * flash_internal.c
 *
 *  Created on: Oct 11, 2023
 *      Author: TungVT
 */

#include "flash_internal.h"

//#define FLASH_USER_START_ADDR   ADDR_FLASH_PAGE_4   /* Start @ of user Flash area */
//#define FLASH_USER_END_ADDR     (ADDR_FLASH_PAGE_63 + FLASH_PAGE_SIZE - 1)   /* End @ of user Flash area */

uint32_t PageError = 0;

static FLASH_EraseInitTypeDef EraseInitStruct;
static uint32_t GetPage(uint32_t Address);
static uint32_t GetBank(uint32_t Address);

void FLASH_Erase_Pages(uint32_t page_start, uint32_t page_num)
{
	HAL_FLASH_Unlock();

	/* Clear OPTVERR bit set on virgin samples */
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);

	/* Fill EraseInit structure*/
	EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.Banks       = 1;
	EraseInitStruct.Page        = page_start;
	EraseInitStruct.NbPages     = page_num;

	HAL_FLASHEx_Erase(&EraseInitStruct, &PageError);
	HAL_FLASH_Lock();
}

void FLASH_Erase_Address(uint32_t addr_start, uint32_t addr_end)
{
	uint32_t FirstPage = 0, NbOfPages = 0, BankNumber = 0;
	HAL_FLASH_Unlock();
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);

	FirstPage = GetPage(addr_start);
	NbOfPages = GetPage(addr_end) - FirstPage + 1;
	BankNumber = GetBank(addr_start);

	/* Fill EraseInit structure*/
	EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.Banks       = BankNumber;
	EraseInitStruct.Page        = FirstPage;
	EraseInitStruct.NbPages     = NbOfPages;

	HAL_FLASHEx_Erase(&EraseInitStruct, &PageError);
	HAL_FLASH_Lock();
}

bool FLASH_Write_8bytes(uint32_t addr, uint8_t* data)
{
	uint64_t data64, datacheck = 0;
	uint8_t* ptr = (uint8_t*)&data64;

	for(uint8_t i = 0; i < 8; i++)
	{
		*(ptr + i) = *(data + i);
	}

	HAL_FLASH_Unlock();
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, addr, data64);
    HAL_FLASH_Lock();

    datacheck = *(__IO uint64_t *)addr;

    if(datacheck == data64) return true;
    else return false;
}

bool FLASH_Write(uint32_t addr, uint8_t* data, uint32_t length)
{
	uint64_t data64 = 0;
	uint8_t* ptr = (uint8_t*)&data64;
	uint32_t addr_point = addr;
	uint8_t count = 0;

	HAL_FLASH_Unlock();
	while(length > 0)
	{
		if(length >= 8)
		{
			for(uint8_t i = 0; i < 8; i++)
			{
				*(ptr + i) = *(data + count * 8 + i);
			}
			HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, addr_point, data64);
			addr_point += 8;
			length -= 8;
			count++;
		}
		else
		{
			for(uint8_t i = 0; i < length; i++)
			{
				*(ptr + i) = *(data + count * 8 + i);
			}

			for(uint8_t i = length; i < 8; i++)
			{
				*(ptr + i) = 0xFF;
			}
			HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, addr_point, data64);
			addr_point += length;
			length -= length;
			count++;
		}
	}

    HAL_FLASH_Lock();
    return true;
}

static uint32_t GetPage(uint32_t Addr)
{
  uint32_t page = 0;
  page = (Addr - FLASH_BASE) / FLASH_PAGE_SIZE;
  return page;
}

static uint32_t GetBank(uint32_t Addr)
{
  return FLASH_BANK_1;
}

