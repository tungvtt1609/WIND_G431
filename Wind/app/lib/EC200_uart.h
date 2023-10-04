#ifndef _EC200_UART_H_
#define _EC200_UART_H_
#include <string.h>
#include <stdint.h>

#include "serial/serial.h"
#include "interface.h"
#include "driver/control/pin_func.h"

#if defined(__cplusplus)
extern "C"
{
#endif /* __cplusplus */

#define COMMAND_SIZE 1024

/* PE2 is configured as POWER_PIN */
#define EC200_RESET_PIN RESET_4G
#define EC200_POWER_PIN POWER_4G

/* EC200-UART Baudrate */
#define EC200_UART_BAUDRATE 115200U

/* Struct to contain all of variables for pre-processing EC200's data from UART */
typedef struct
{
    uint16_t Upcoming_Data_Count;
    volatile bool Is_Receive_Done;
}EC200_preprocessing_data_t;

/***********************************************************************************************************************
 * Initialization functions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * User functions
 **********************************************************************************************************************/
void EC200_UART_Init(void);
void EC200_UART_SendString(uint8_t *dat);
void EC200_UART_Handler(uint8_t rx_char);

#if defined(__cplusplus)
}
#endif

#endif /* _EC200_UART_H_ */
