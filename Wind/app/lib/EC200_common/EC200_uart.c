#include "EC200_uart.h"

uint8_t EC200_DataProcessing_Array[COMMAND_SIZE];

uint8_t MQTT_Response_Command[COMMAND_SIZE];
uint8_t MQTT_Response_Server[COMMAND_SIZE];
uint8_t FTP_Response_Command[COMMAND_SIZE];
uint8_t FTP_Response_Server[COMMAND_SIZE];

current_application_occur_e current_application_occur = MQTT_APPLICATION_OCCUR; /* Default value */
ftp_output_mode_e ftp_output_mode = FTP_COMMAND_MODE;
FTP_received_data_type_t FTP_received_data_type = {false, false};
MQTT_received_data_type_t MQTT_received_data_type = {false, false};
serial_obj *serial_EC200;
/* FTP data count for receiving content from server */
FTP_preprocessing_data_t FTP_preprocessing_data = {0, 0};

/* Extern variables */

/*******************************************************************************
 * Variables
 ******************************************************************************/
EC200_preprocessing_data_t EC200_preprocessing_data = {0};

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
    EC200_DataProcessing_Array[EC200_preprocessing_data.Upcoming_Data_Count] = rx_char;
    EC200_preprocessing_data.Upcoming_Data_Count++;

    if (((EC200_preprocessing_data.Upcoming_Data_Count > 3) && ((rx_char == '\n') || ((strcmp((char *)EC200_DataProcessing_Array, "\r\n> ") == 0)))) || ((current_application_occur == FTP_APPLICATION_OCCUR) && (ftp_output_mode == FTP_DATA_MODE))) /* >3 to avoid the first end of line  */
    {
        /* Exception cases */
        if ((strcmp((char *)EC200_DataProcessing_Array, "ATE0\r\r\n") == 0))
        {
            return;
        }
        if (current_application_occur == MQTT_APPLICATION_OCCUR) /* MQTT occurs UART interrupt resource (Always running if FTP application not occur) */
        {
            /* Receive data from Server */
            if (strstr(EC200_DataProcessing_Array, "+QMTRECV:") != NULL)
            {
                MQTT_received_data_type.Is_Data_From_Server = true;

                memset(MQTT_Response_Server, 0, strlen(MQTT_Response_Server));
                for (int i = 0; i < EC200_preprocessing_data.Upcoming_Data_Count; i++)
                {
                    MQTT_Response_Server[i] = EC200_DataProcessing_Array[i];
                }
            }
            else /* Receive data from Command response */
            {
                MQTT_received_data_type.Is_Data_From_Command = true;

                memset(MQTT_Response_Command, 0, strlen(MQTT_Response_Command));
                for (int i = 0; i < EC200_preprocessing_data.Upcoming_Data_Count; i++)
                {
                    MQTT_Response_Command[i] = EC200_DataProcessing_Array[i];
                }
            }
        }
        else if (current_application_occur == FTP_APPLICATION_OCCUR) /* FTP occurs UART interrupt resource */
        {
            if (strstr(EC200_DataProcessing_Array, "\r\nCONNECT\r\n") != NULL)
            {
                ftp_output_mode = FTP_DATA_MODE;
                FTP_preprocessing_data.Upcoming_ServerData_Count = 0;
                memset(FTP_Response_Server, 0, strlen(FTP_Response_Server));
            }
            else if (ftp_output_mode == FTP_DATA_MODE) /* Receive data from Server */
            {
                FTP_Response_Server[FTP_preprocessing_data.Upcoming_ServerData_Count] = rx_char;
                if (rx_char == '+') /* Significant character to detect end of stream */
                {
                    FTP_preprocessing_data.EndOfStream_Position = FTP_preprocessing_data.Upcoming_ServerData_Count;
                    FTP_received_data_type.Is_Data_From_Server = true;
                }

                FTP_preprocessing_data.Upcoming_ServerData_Count++;
            }
            else if (ftp_output_mode == FTP_COMMAND_MODE) /* Receive data from Command response */
            {
                FTP_received_data_type.Is_Data_From_Command = true;

                memset(FTP_Response_Command, 0, strlen(FTP_Response_Command));
                for (int i = 0; i < EC200_preprocessing_data.Upcoming_Data_Count; i++)
                {
                    FTP_Response_Command[i] = EC200_DataProcessing_Array[i];
                }
            }
        }

        /* Clear unnecessary data in buffer */
        memset(EC200_DataProcessing_Array, 0, strlen(EC200_DataProcessing_Array));
        /* Done of data receiving process */
        EC200_preprocessing_data.Upcoming_Data_Count = 0;
    }
}
