#include "EC200_common.h"

bool enable_timeout = false;
uint32_t ec200_timeout = EC200_RESET_TIMEOUT;

ec200_simStart_state_e ec200_simStart_state = EC200_POWER_OFF;
uint8_t EC200_Command_Buffer[RECEIVE_SIZE];

/* External variables */
extern uint8_t EC200_DataProcessing_Array[COMMAND_SIZE];
extern current_application_occur_e current_application_occur;
/* MQTT application */
extern uint8_t MQTT_Response_Command[COMMAND_SIZE];
extern MQTT_received_data_type_t MQTT_received_data_type;
/* FTP application */
extern uint8_t FTP_Response_Command[COMMAND_SIZE];
extern FTP_received_data_type_t FTP_received_data_type;

void EC200_Delayms(uint32_t mili_sec) /* Abstract function */
{
    HAL_Delay(mili_sec);
}

void EC200_RESET(void)
{
    /* Turn on the Power */
    EC200_POWER_PIN_LOW();
    EC200_Delayms(1000U);
    /* RESET EC200 */
    EC200_RESET_PIN_HIGH();
    /* Delay 1000 ms */
    EC200_Delayms(1000U);
    EC200_RESET_PIN_LOW();
}

void EC200_Init(void)
{
    EC200_UART_Init();
    /* Reset EC200 module */
    EC200_RESET();
}

/************************** EC200 data processing functions ********************************/
bool EC200_CompareCommand(uint8_t *compared_buffer, char *command)
{
    if (strcmp((char *)compared_buffer, command) == 0)
    {
        return true;
    }
    return false;
}

bool EC200_ReceiveCommand(uint8_t *receiv_command_buffer)
{
    uint8_t index = 0;
    bool return_function = false;

    if (enable_timeout == false)
    {
        /* Reset timeout */
        ec200_timeout = EC200_RESET_TIMEOUT;
        enable_timeout = true;
    }

    if (current_application_occur == MQTT_APPLICATION_OCCUR)
    {
        if (MQTT_received_data_type.Is_Data_From_Command == true)
        {
            // memset(receiv_command_buffer, 0, RECEIVE_SIZE);
            memset(receiv_command_buffer, 0, strlen(receiv_command_buffer));
            while (MQTT_Response_Command[index] != NULL)
            {
                receiv_command_buffer[index] = MQTT_Response_Command[index];
                index++;
            }
            MQTT_received_data_type.Is_Data_From_Command = false;

            enable_timeout = false;
            ec200_timeout = EC200_RESET_TIMEOUT;
            return_function = true;
        }
    }
    else if (current_application_occur == FTP_APPLICATION_OCCUR)
    {
        if (FTP_received_data_type.Is_Data_From_Command == true)
        {
            // memset(receiv_command_buffer, 0, RECEIVE_SIZE);
            memset(receiv_command_buffer, 0, strlen(receiv_command_buffer));
            while (FTP_Response_Command[index] != NULL)
            {
                receiv_command_buffer[index] = FTP_Response_Command[index];
                index++;
            }
            FTP_received_data_type.Is_Data_From_Command = false;

            enable_timeout = false;
            ec200_timeout = EC200_RESET_TIMEOUT;
            return_function = true;
        }
    }

    return return_function;
}

void EC200_SendCommand(uint8_t *ec200_cmd)
{
    EC200_UART_SendString(ec200_cmd);
}

/******************************************* Starting functions ***********************************************/
bool Is_Power_ON(void)
{
    bool return_function = false;
    if (EC200_ReceiveCommand(EC200_Command_Buffer))
    {
        if (EC200_CompareCommand(EC200_Command_Buffer, "\r\nRDY\r\n"))
        {
            return_function = true;
        }
    }
    return return_function;
}

/**
 * Disable corresponding response from EC200.
 */
boolean_3_state_e OffEcho(void)
{
    boolean_3_state_e return_function = _NOT_DEFINE_;
    static uint8_t offEcho_step = 0;

    if (offEcho_step == 0)
    {
        EC200_SendCommand("ATE0\r");
        offEcho_step = 1;
    }
    else if (offEcho_step == 1)
    {
        if (EC200_ReceiveCommand(EC200_Command_Buffer))
        {
            if (EC200_CompareCommand(EC200_Command_Buffer, "ATE0\r\r\nOK\r\n"))
            {
                return_function = _TRUE_;
            }
            else /* Error command */
            {
                return_function = _FALSE_;
            }
            offEcho_step = 0;
        }
        /* TODO: Timeout should be added here */
    }
    return return_function;
}

/**
 * Initialize EC200 module after power-up.
 */
bool EC200_SIM_Start(void)
{
    bool return_function = false;
    boolean_3_state_e check_result = _NOT_DEFINE_;

    switch (ec200_simStart_state)
    {
    /* EC200_POWERED_OFF is the first state if the state rolls back */
    case EC200_POWER_OFF:
        if (Is_Power_ON())
        {
            ec200_simStart_state = EC200_POWERED_ON;
            EC200_Delayms(500); /* Delay after each state */
        }
        break;
    case EC200_POWERED_ON:
        check_result = OffEcho();
        if (check_result == _TRUE_)
        {
            ec200_simStart_state = EC200_STARTING_DONE;
            EC200_Delayms(500); /* Delay after each state */
        }
        break;

    case EC200_STARTING_DONE: /* Starting completed */
        return_function = true;
        break;

    case EC200_RESTART:
        /* TODO */
        break;

    default:
        break;
    }
    return return_function;
}


void EC200_Time_Base_1ms(void)
{
    if(enable_timeout == true)
    {
        if(ec200_timeout > 0)
        {
            ec200_timeout--;
        }
    }
}

