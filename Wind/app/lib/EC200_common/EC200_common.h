#ifndef _EC200_COMMON_H_
#define _EC200_COMMON_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "EC200_uart.h"

#define EC200_RESET_PIN_HIGH() Pin_Func_TurnOn(EC200_RESET_PIN)
#define EC200_RESET_PIN_LOW() Pin_Func_TurnOff(EC200_RESET_PIN)
#define EC200_POWER_PIN_HIGH() Pin_Func_TurnOn(EC200_POWER_PIN)
#define EC200_POWER_PIN_LOW() Pin_Func_TurnOff(EC200_POWER_PIN)
#define RECEIVE_SIZE 400
#define EC200_RESET_COUNTDOWN 3
#define EC200_RESET_TIMEOUT 5000 //5s

typedef enum
{
    EC200_POWER_OFF = 0,
    EC200_POWERED_ON,
    EC200_STARTING_DONE,
    EC200_RESTART
} ec200_simStart_state_e;

typedef enum
{
    _FALSE_ = false,
    _TRUE_ = true,
    _NOT_DEFINE_
} boolean_3_state_e;

void EC200_SendCommand(uint8_t *ec200_cmd);
bool EC200_ReceiveCommand(uint8_t *receiv_command_buffer);
bool EC200_CompareCommand(uint8_t *compared_buffer, char *command);
bool EC200_SIM_Start(void);
void EC200_RESET(void);
void EC200_Delayms(uint32_t mili_sec);
void EC200_Time_Base_1ms(void);
/********************************************* USER FUNCTION ***************************************************/
void EC200_Init(void);

#endif /* _EC200_COMMON_H_ */