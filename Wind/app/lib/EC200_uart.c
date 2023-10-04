#include "EC200_uart.h"
#include "serial/serial.h"

uint8_t MQTT_DataProcessing_Array[COMMAND_SIZE];

uint8_t MQTT_Response_Command[COMMAND_SIZE];
uint8_t MQTT_Response_Server[COMMAND_SIZE];

/* Extern variables */
extern bool Is_Data_From_Server;
serial_obj *serial_EC200;

/*******************************************************************************
 * Variables
 ******************************************************************************/
EC200_preprocessing_data_t EC200_preprocessing_data = { 0, false};

void EC200_UART_InitPins(void)
{
    ;
}

void EC200_RESET_InitPins(void)
{
    ;
}

void EC200_UART_Init(void)
{
    /* Configure UART Pins */
    EC200_UART_InitPins();
    /* Configure EC200-RESET Pins */
    EC200_RESET_InitPins();
    serial_EC200 = create_serial(0, EC200_UART_BAUDRATE, 64, 64); /* Data size=64 can be modified later */
}

void EC200_UART_SendString(uint8_t *dat)
{
    SendStringUart2(dat);
}

void EC200_UART_Handler(uint8_t rx_char)
{
        /* Buffer save the data to check it's sent from Server or not */
        MQTT_DataProcessing_Array[EC200_preprocessing_data.Upcoming_Data_Count] = rx_char;
        EC200_preprocessing_data.Upcoming_Data_Count++;

        if ((EC200_preprocessing_data.Upcoming_Data_Count > 3) && ((rx_char == '\n') || ((strcmp((char *)MQTT_DataProcessing_Array, "\r\n> ") == 0)))) /* >3 to avoid the first end of line  */
        {
            /* Exception cases */
            if((strcmp((char *)MQTT_DataProcessing_Array, "ATE0\r\r\n") == 0))
            {
                return;
            }

            /* Receive data from Server */
            if (strstr(MQTT_DataProcessing_Array, "+QMTRECV:") != NULL)
            {
                Is_Data_From_Server = true;

                memset(MQTT_Response_Server, 0, strlen(MQTT_Response_Server));

                for (int i = 0; i < EC200_preprocessing_data.Upcoming_Data_Count; i++)
                {
                    MQTT_Response_Server[i] = MQTT_DataProcessing_Array[i];
                }
            }
            else /* Receive data from Command response */
            {
                EC200_preprocessing_data.Is_Receive_Done = true;

                memset(MQTT_Response_Command, 0, strlen(MQTT_Response_Command));

                for (int i = 0; i < EC200_preprocessing_data.Upcoming_Data_Count; i++)
                {
                    MQTT_Response_Command[i] = MQTT_DataProcessing_Array[i];
                }
            }
            /* Clear unnecessary data in buffer */
            memset(MQTT_DataProcessing_Array, 0, strlen(MQTT_DataProcessing_Array));
            /* Done of data receiving process */
            EC200_preprocessing_data.Upcoming_Data_Count = 0;
        }
}

