/*
 * up_firm.c
 *
 *  Created on: Oct 3, 2023
 *      Author: TungVT
 */

#include "up_firm.h"
#include "state_machine.h"
#include "interface.h"
#include "flash_internal/flash_internal.h"
#include "time.h"
#include "config_parameter.h"
#include "control_dcdc.h"
#include "serial/serial.h"

#define ADDR_BOOT					0x08000000

//#define ADDR_FL_START				0x0803E000
//#define ADDR_FL_END				0x0803E7FF


uint32_t fw_version = FWVER;
extern UART_HandleTypeDef huart2;
static uint32_t s_udfw_wind_sts = 0;
static void WIND_UDFW_Start(void);


static up_cmd_t list_up_cmd[ID_UP_MAX] =
{
		{ID_UPDATE_FW,			UP_NONE,			NULL,				&WIND_UDFW_Start},
};

static uint32_t count_time_sts = 0;


void UP_Time1kHz(void){
	static uint32_t count_time_reset = 0;
	count_time_sts++;
	if(s_udfw_wind_sts == 1){
		count_time_reset++;
		if(count_time_reset >= 5000){
			s_udfw_wind_sts = 0;
			count_time_reset = 0;
			NVIC_SystemReset();
		}
	}
}

static void WIND_UDFW_Start(void)
{
	DC_StopPWM_Buck();
	DC_StopPWM_Boost();
	s_udfw_wind_sts = 1;
}

void UP_Process(void){
	uint8_t rxdata[8];

	if(true == HAL_UART_Receive_IT(&huart2, (uint8_t*)&rxdata, 8))
	{

		for(uint8_t i = 0; i < ID_UP_MAX; i++)
		{
			if(list_up_cmd[i].up_sts == UP_NONE)
			{
				list_up_cmd[i].up_func(rxdata);
			}

			i = ID_UP_MAX;
		}
	}

}

