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

// #define COMMAND_SIZE 1024
#define COMMAND_SIZE 600

/* PE2 is configured as POWER_PIN */
#define EC200_RESET_PIN RESET_4G
#define EC200_POWER_PIN POWER_4G

/* EC200-UART Baudrate */
#define EC200_UART_BAUDRATE 115200U

/* Struct to contain all of variables for pre-processing EC200's data from UART */
typedef struct
{
    uint16_t Upcoming_Data_Count;
}EC200_preprocessing_data_t;

typedef struct
{
    volatile bool Is_Data_From_Server;
    volatile bool Is_Data_From_Command;
}MQTT_received_data_type_t;

typedef struct
{
    volatile bool Is_Data_From_Server;
    volatile bool Is_Data_From_Command;
}FTP_received_data_type_t;

typedef struct
{
    uint16_t Upcoming_ServerData_Count;
    uint16_t EndOfStream_Position;
}FTP_preprocessing_data_t;

typedef enum
{
    MQTT_APPLICATION_OCCUR = 0, /* Default running if FTP application not occur */
    FTP_APPLICATION_OCCUR
} current_application_occur_e;

/**
 * FTP output mode to seperate upcoming data
*/
typedef enum
{
    FTP_COMMAND_MODE = 0,
    FTP_DATA_MODE
}ftp_output_mode_e;

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
