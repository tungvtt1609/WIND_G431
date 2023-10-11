#include "EC200_mqtt_application.h"

mqtt_connectServer_state_e mqtt_connectServer_state = MQTT_IDLE;

/* Extern variables */
extern uint8_t EC200_Command_Buffer[RECEIVE_SIZE];
extern ec200_simStart_state_e ec200_simStart_state;
/* Extern MQTT application */
extern MQTT_received_data_type_t MQTT_received_data_type;
extern uint8_t MQTT_Response_Server[COMMAND_SIZE];
/* Timeout variables */
extern bool enable_timeout;
extern uint32_t ec200_timeout;
extern uint8_t reset_sim_countdown;

void MQTT_Trigger_SIM_Restart(void)
{
    /* Reset EC200 module using RESET PIN */
    EC200_RESET();
    ec200_timeout = EC200_RESET_TIMEOUT;
    reset_sim_countdown = EC200_RESET_COUNTDOWN;
    enable_timeout = false;
    mqtt_connectServer_state = MQTT_IDLE;
    ec200_simStart_state = EC200_POWER_OFF;
    EC200_Delayms(1000);
}

/************************************** MQTT-connecting functions ***********************************************/
boolean_3_state_e MQTT_SetKeepAlive_Time(uint16_t keepalive_time, uint8_t client_id)
{
    boolean_3_state_e return_function = _NOT_DEFINE_;
    static uint8_t setKeepalive_time_step = 0;
    uint8_t buff_temp[50];

    /* Send Keep alive command */
    if (setKeepalive_time_step == 0)
    {
        memset(buff_temp, 0, 50);
        sprintf(buff_temp, "AT+QMTCFG=\"keepalive\",%d,%d\r", client_id, keepalive_time);
        EC200_SendCommand(buff_temp);
        setKeepalive_time_step = 1;
    }
    else if (setKeepalive_time_step == 1)
    {
        if (EC200_ReceiveCommand(EC200_Command_Buffer))
        {
            if (EC200_CompareCommand(EC200_Command_Buffer, "\r\nOK\r\n"))
            {
                return_function = _TRUE_;
            }
            else /* Error command */
            {
                return_function = _FALSE_;
            }
            setKeepalive_time_step = 0;
        }
        /* Timeout */
        if (ec200_timeout == 0)
        {
            setKeepalive_time_step = 0;
            MQTT_Trigger_SIM_Restart();
        }
    }
    return return_function;
}

boolean_3_state_e MQTT_Receiving_Mode(uint8_t client_id, uint8_t msg_recv_mode, uint8_t msg_len_enable)
{
    boolean_3_state_e return_function = _NOT_DEFINE_;
    static uint8_t setReceivingMode_step = 0;
    uint8_t buff_temp[50];

    /* Send Keep alive command */
    if (setReceivingMode_step == 0)
    {
        memset(buff_temp, 0, 50);
        sprintf(buff_temp, "AT+QMTCFG=\"recv/mode\",%d,%d,%d\r", client_id, msg_recv_mode, msg_len_enable);
        EC200_SendCommand(buff_temp);
        setReceivingMode_step = 1;
    }
    else if (setReceivingMode_step == 1)
    {
        if (EC200_ReceiveCommand(EC200_Command_Buffer))
        {
            if (EC200_CompareCommand(EC200_Command_Buffer, "\r\nOK\r\n"))
            {
                return_function = _TRUE_;
            }
            else /* Error command */
            {
                return_function = _FALSE_;
            }
            setReceivingMode_step = 0;
        }
        /* Timeout  */
        if (ec200_timeout == 0)
        {
            setReceivingMode_step = 0;
            MQTT_Trigger_SIM_Restart();
        }
    }
    return return_function;
}

boolean_3_state_e MQTT_Open(uint8_t client_id, char *host_name, uint16_t port)
{
    boolean_3_state_e return_function = _NOT_DEFINE_;
    static uint8_t MQTT_open_step = 0;
    uint8_t buff_temp[50];
    uint8_t buff_temp1[30];

    if (MQTT_open_step == 0)
    {
        memset(buff_temp, 0, 50);
        sprintf(buff_temp, "AT+QMTOPEN=%d,%s,%d\r", client_id, host_name, port);
        EC200_SendCommand(buff_temp);

        MQTT_open_step = 1;
    }

    else if (MQTT_open_step == 1)
    {
        if (EC200_ReceiveCommand(EC200_Command_Buffer))
        {
            /* Case 1: Received full-response */
            memset(buff_temp, 0, 50);
            sprintf(buff_temp, "\r\nOK\r\n\r\n+QMTOPEN: %d,0\r\n", client_id);

            /* Case 2: Received half-response */
            memset(buff_temp1, 0, 30);
            sprintf(buff_temp1, "\r\n+QMTOPEN: %d,0\r\n", client_id);
            if (EC200_CompareCommand(EC200_Command_Buffer, "\r\nOK\r\n"))
            {
                /* Do nothing */
            }
            else if (EC200_CompareCommand(EC200_Command_Buffer, buff_temp) || EC200_CompareCommand(EC200_Command_Buffer, buff_temp1))
            {
                return_function = _TRUE_;
                MQTT_open_step = 0;
            }
            else /* Error command */
            {
                return_function = _FALSE_;
                MQTT_open_step = 0;
            }
        }
        /* Timeout */
        if (ec200_timeout == 0)
        {
            MQTT_open_step = 0;
            MQTT_Trigger_SIM_Restart();
        }
        
    }
    return return_function;
}

/* TODO: Not checked yet */
boolean_3_state_e MQTT_Close(uint8_t client_id)
{
    boolean_3_state_e return_function = _NOT_DEFINE_;
    static uint8_t MQTT_close_step = 0;
    uint8_t buff_temp[50];
    uint8_t buff_temp1[30];

    if (MQTT_close_step == 0)
    {
        memset(buff_temp, 0, 50);
        sprintf(buff_temp, "AT+QMTCLOSE=%d\r", client_id);
        EC200_SendCommand(buff_temp);

        MQTT_close_step = 1;
    }

    else if (MQTT_close_step == 1)
    {
        if (EC200_ReceiveCommand(EC200_Command_Buffer))
        {
            /* Case 1: Received full-response */
            memset(buff_temp, 0, 50);
            sprintf(buff_temp, "\r\nOK\r\n\r\n+QMTCLOSE: %d,0\r\n", client_id);

            /* Case 2: Received half-response */
            memset(buff_temp1, 0, 30);
            sprintf(buff_temp1, "\r\n+QMTCLOSE: %d,0\r\n", client_id);
            if (EC200_CompareCommand(EC200_Command_Buffer, "\r\nOK\r\n"))
            {
                /* Do nothing */
            }
            else if (EC200_CompareCommand(EC200_Command_Buffer, buff_temp) || EC200_CompareCommand(EC200_Command_Buffer, buff_temp1))
            {
                return_function = _TRUE_;
                MQTT_close_step = 0;
            }
            else /* Error command */
            {
                return_function = _FALSE_;
                MQTT_close_step = 0;
            }
        }
        /* Timeout */
        if (ec200_timeout == 0)
        {
            MQTT_close_step = 0;
            MQTT_Trigger_SIM_Restart();
        }
    }
    return return_function;
}

boolean_3_state_e MQTT_Connect(uint8_t client_id, char *client_name, char *username, char *password)
{
    boolean_3_state_e return_function = _NOT_DEFINE_;
    static uint8_t MQTT_connect_step = 0;
    uint8_t buff_temp[200];
    uint8_t buff_temp1[50];
    uint8_t buff_temp2[30];

    if (MQTT_connect_step == 0)
    {
        memset(buff_temp, 0, 200);
        sprintf(buff_temp, "AT+QMTCONN=%d,%s,%s,%s\r", client_id, client_name, username, password);
        EC200_SendCommand(buff_temp);

        MQTT_connect_step = 1;
    }

    else if (MQTT_connect_step == 1)
    {
        if (EC200_ReceiveCommand(EC200_Command_Buffer))
        {
            /* Case 1: Received full response */
            memset(buff_temp1, 0, 50);
            sprintf(buff_temp1, "\r\nOK\r\n\r\n+QMTCONN: %d,0,0\r\n", client_id);
            /* Case 2: Received half response */
            memset(buff_temp2, 0, 30);
            sprintf(buff_temp2, "\r\n+QMTCONN: %d,0,0\r\n", client_id);

            if (EC200_CompareCommand(EC200_Command_Buffer, "\r\nOK\r\n"))
            {
                /* Do nothing */
            }
            else if (EC200_CompareCommand(EC200_Command_Buffer, buff_temp1) || EC200_CompareCommand(EC200_Command_Buffer, buff_temp2))
            {
                return_function = _TRUE_;
                MQTT_connect_step = 0;
            }
            else /* Error command */
            {
                return_function = _FALSE_;
                MQTT_connect_step = 0;
            }
        }
        /* Timeout */
        if (ec200_timeout == 0)
        {
            MQTT_connect_step = 0;
            MQTT_Trigger_SIM_Restart();
        }
    }
    return return_function;
}

/* TODO: Not checked yet */
boolean_3_state_e MQTT_Disconnect(uint8_t client_id)
{
    boolean_3_state_e return_function = _NOT_DEFINE_;
    static uint8_t MQTT_disconnect_step = 0;
    uint8_t buff_temp[30];
    uint8_t buff_temp1[50];
    uint8_t buff_temp2[30];

    if (MQTT_disconnect_step == 0)
    {
        memset(buff_temp, 0, 30);
        sprintf(buff_temp, "AT+QMTDISC=%d\r", client_id);
        EC200_SendCommand(buff_temp);

        MQTT_disconnect_step = 1;
    }

    else if (MQTT_disconnect_step == 1)
    {
        if (EC200_ReceiveCommand(EC200_Command_Buffer))
        {
            /* Case 1: Received full response */
            memset(buff_temp1, 0, 50);
            sprintf(buff_temp1, "\r\nOK\r\n\r\n+QMTDISC: %d,0\r\n", client_id);
            /* Case 2: Received half response */
            memset(buff_temp2, 0, 30);
            sprintf(buff_temp2, "\r\n+QMTDISC: %d,0\r\n", client_id);

            if (EC200_CompareCommand(EC200_Command_Buffer, "\r\nOK\r\n"))
            {
                /* Do nothing */
            }
            else if (EC200_CompareCommand(EC200_Command_Buffer, buff_temp1) || EC200_CompareCommand(EC200_Command_Buffer, buff_temp2))
            {
                return_function = _TRUE_;
                MQTT_disconnect_step = 0;
            }
            else /* Error command */
            {
                return_function = _FALSE_;
                MQTT_disconnect_step = 0;
            }
        }
        /* Timeout */
        if (ec200_timeout == 0)
        {
            MQTT_disconnect_step = 0;
            MQTT_Trigger_SIM_Restart();
        }
    }
    return return_function;
}

/**
 * Publish data to particular topic.
 */
boolean_3_state_e MQTT_PubTopic(uint8_t client_id, uint8_t msgid, uint8_t qos, uint8_t retain, uint8_t *topic,
                                uint16_t send_datalength, uint8_t *data)
{
    boolean_3_state_e return_function = _NOT_DEFINE_;
    static uint8_t MQTT_pubTopic_step = 0;
    uint8_t buff_temp[400];
    uint8_t buff_temp1[50];
    uint8_t buff_temp2[30];

    if (MQTT_pubTopic_step == 0)
    {
        memset(buff_temp, 0, 400);
        sprintf(buff_temp, "AT+QMTPUBEX=%d,%d,%d,%d,%s,%d\r", client_id, msgid, qos, retain, topic, send_datalength);
        EC200_SendCommand(buff_temp);

        MQTT_pubTopic_step = 1;
    }

    else if (MQTT_pubTopic_step == 1)
    {
        if (EC200_ReceiveCommand(EC200_Command_Buffer))
        {
            if (EC200_CompareCommand(EC200_Command_Buffer, "\r\n> "))
            {
                MQTT_pubTopic_step = 2;
            }
            else /* Error command */
            {
                return_function = _FALSE_;
                MQTT_pubTopic_step = 0;
            }
        }
        /* Timeout  */
        if (ec200_timeout == 0)
        {
            MQTT_pubTopic_step = 0;
            /* Return _FALSE_ to break out of while() */
            return_function = _FALSE_;
            MQTT_Trigger_SIM_Restart();
        }
    }

    else if (MQTT_pubTopic_step == 2)
    {
        EC200_SendCommand(data);

        MQTT_pubTopic_step = 3;
    }

    else if (MQTT_pubTopic_step == 3)
    {
        if (EC200_ReceiveCommand(EC200_Command_Buffer))
        {
            /* Case 1: Received full response */
            memset(buff_temp1, 0, 50);
            sprintf(buff_temp1, "\r\nOK\r\n\r\n+QMTPUBEX: %d,%d,0\r\n", client_id, msgid);
            /* Case 2: Received half response */
            memset(buff_temp2, 0, 30);
            sprintf(buff_temp2, "\r\n+QMTPUBEX: %d,%d,0\r\n", client_id, msgid);

            if (EC200_CompareCommand(EC200_Command_Buffer, "\r\nOK\r\n"))
            {
                /* Do nothing */
            }
            else if (EC200_CompareCommand(EC200_Command_Buffer, buff_temp1) || EC200_CompareCommand(EC200_Command_Buffer, buff_temp2))
            {
                return_function = _TRUE_;
                MQTT_pubTopic_step = 0;
            }
            else /* Error command */
            {
                return_function = _FALSE_;
                MQTT_pubTopic_step = 0;
            }
        }
        /* Timeout */
        if (ec200_timeout == 0)
        {
            MQTT_pubTopic_step = 0;
            /* Return _FALSE_ to break out of while() */
            return_function = _FALSE_;
            MQTT_Trigger_SIM_Restart();
        }
    }
    return return_function;
}

boolean_3_state_e MQTT_SubTopic(uint8_t client_id, uint8_t msgid, char *topic, uint8_t qos)
{
    boolean_3_state_e return_function = _NOT_DEFINE_;
    static uint8_t MQTT_subTopic_step = 0;
    char buff_temp[400];
    char buff_temp1[50];
    char buff_temp2[30];

    if (MQTT_subTopic_step == 0)
    {
        memset(buff_temp, 0, 400);
        sprintf(buff_temp, "AT+QMTSUB=%d,%d,%s,%d\r", client_id, msgid, topic, qos);
        EC200_SendCommand(buff_temp);

        MQTT_subTopic_step = 1;
    }

    else if (MQTT_subTopic_step == 1)
    {
        if (EC200_ReceiveCommand(EC200_Command_Buffer))
        {
            /* Case 1: Received full response */
            memset(buff_temp1, 0, 50);
            sprintf(buff_temp1, "\r\nOK\r\n\r\n+QMTSUB: %d,%d,0,0\r\n", client_id, msgid);
            /* Case 2: Received half response */
            memset(buff_temp2, 0, 30);
            sprintf(buff_temp2, "\r\n+QMTSUB: %d,%d,0,0\r\n", client_id, msgid);

            if (EC200_CompareCommand(EC200_Command_Buffer, "\r\nOK\r\n"))
            {
                /* Do nothing */
            }
            else if ((strstr((char *)EC200_Command_Buffer, buff_temp1) != NULL) || ((strstr((char *)EC200_Command_Buffer, buff_temp2) != NULL)))
            {
                return_function = _TRUE_;
                MQTT_subTopic_step = 0;
            }
            else /* Error command */
            {
                return_function = _FALSE_;
                MQTT_subTopic_step = 0;
            }
        }
        /* Timeout */
        if (ec200_timeout == 0)
        {
            MQTT_subTopic_step = 0;
            MQTT_Trigger_SIM_Restart();
        }
    }

    return return_function;
}

/******************************************** Application sequence functions **********************************************/
/**
 * MQTT client connects to Server.
 */
bool EC200_MQTT_ConnectToServer(void)
{
    bool return_function = false;
    boolean_3_state_e check_result = _NOT_DEFINE_;

    if (EC200_SIM_Start()) /* If EC200 has been successfully started */
    {
        switch (mqtt_connectServer_state)
        {
        /* MQTT_IDLE is the first state if the state rolls back */
        case MQTT_IDLE:
            check_result = MQTT_SetKeepAlive_Time(KEEP_ALIVE_TIME, CLIENT_ID);
            if (check_result == _TRUE_)
            {
                mqtt_connectServer_state = MQTT_SENT_KEEPALIVE;
                reset_sim_countdown = EC200_RESET_COUNTDOWN;
                EC200_Delayms(500); /* Delay after each state */
            }
            else if (check_result == _FALSE_)
            {
                reset_sim_countdown--;
                if (reset_sim_countdown == 0)
                {
                    mqtt_connectServer_state = MQTT_RESET;
                }
            }
            break;

        case MQTT_SENT_KEEPALIVE:
            check_result = MQTT_Receiving_Mode(CLIENT_ID, 0, 0);
            if (check_result == _TRUE_)
            {
                mqtt_connectServer_state = MQTT_SENT_RECEIVING_MODE;
                reset_sim_countdown = EC200_RESET_COUNTDOWN;
                EC200_Delayms(500); /* Delay after each state */
            }
            else if (check_result == _FALSE_)
            {
                reset_sim_countdown--;
                if (reset_sim_countdown == 0)
                {
                    mqtt_connectServer_state = MQTT_RESET;
                }
            }

            break;

        case MQTT_SENT_RECEIVING_MODE:
            check_result = MQTT_Open(CLIENT_ID, MQTT_BROKER, MQTT_PORT);
            if (check_result == _TRUE_)
            {
                mqtt_connectServer_state = MQTT_OPENED;
                reset_sim_countdown = EC200_RESET_COUNTDOWN;
                EC200_Delayms(500); /* Delay after each state */
            }
            else if (check_result == _FALSE_)
            {
                reset_sim_countdown--;
                if (reset_sim_countdown == 0)
                {
                    mqtt_connectServer_state = MQTT_RESET;
                }
            }
            break;

        case MQTT_OPENED:
            check_result = MQTT_Connect(CLIENT_ID, MQTT_CLIENT_NAME, MQTT_USER, MQTT_PASSWORD);
            if (check_result == _TRUE_)
            {
                mqtt_connectServer_state = MQTT_CONNECTED_DONE;
                reset_sim_countdown = EC200_RESET_COUNTDOWN;
                EC200_Delayms(500); /* Delay after each state */
            }
            else if (check_result == _FALSE_)
            {
                reset_sim_countdown--;
                if (reset_sim_countdown == 0)
                {
                    mqtt_connectServer_state = MQTT_RESET;
                }
            }
            break;

        case MQTT_CONNECTED_DONE:
            check_result = MQTT_SubTopic(CLIENT_ID, MQTT_MSG_ID, MQTT_SUB_TOPIC, MQTT_QOS_SUB);
            if (check_result == _TRUE_)
            {
                mqtt_connectServer_state = MQTT_SUBCRIBED;
                reset_sim_countdown = EC200_RESET_COUNTDOWN;
                EC200_Delayms(500); /* Delay after each state */
            }
            else if (check_result == _FALSE_)
            {
                reset_sim_countdown--;
                if (reset_sim_countdown == 0)
                {
                    mqtt_connectServer_state = MQTT_RESET;
                }
            }

            break;

        case MQTT_SUBCRIBED:
            return_function = true;

            break;

        case MQTT_RESET:
            MQTT_Trigger_SIM_Restart();

            break;

        default:
            break;
        }
    }

    return return_function;
}

/*************************************** Data transfer functions **************************************/

/**
 * Send data to server, this function's argument can be passed by the different data types.
 */
bool MQTT_Transmit_Data(void *mqtt_data_struct, mqtt_transferring_data_e transferring_data_type)
{
    bool return_function = false;
    boolean_3_state_e check_result = _NOT_DEFINE_;

    if (mqtt_data_struct != NULL)
    {
        switch (transferring_data_type)
        {
        case BATTERY_DATA:
        {
            battery_send_data_t *battery_data_temp = NULL;
            uint8_t battery_data_buffer[200];
            memset(battery_data_buffer, 0, 200);

            battery_data_temp = (battery_send_data_t *)mqtt_data_struct;
            sprintf(battery_data_buffer, "{\"serial\":\"001\",\"type\":\"battery\",\"batt_status\":\"%d\",\"batt_power\":\"%.2f\",\"batt_volt\":\"%.2f\",\"batt_curr\":\"%.2f\",\"batt_temp\":\"%d\"}", battery_data_temp->state, battery_data_temp->power, battery_data_temp->voltage, battery_data_temp->current, battery_data_temp->temperature);

            while (check_result == _NOT_DEFINE_) /* _NOT_DEFINE_ means that is in progress */
            {
                check_result = MQTT_PubTopic(CLIENT_ID, MQTT_MSG_ID, MQTT_QOS_PUB, MQTT_RETAIN, MQTT_PUB_TOPIC,
                                             strlen(battery_data_buffer), battery_data_buffer);
            }

            if (check_result == _TRUE_)
            {
                return_function = true;
            }

            break;
        }

        case WIND_DATA:
        {
            wind_send_data_t *wind_data_temp = NULL;
            uint8_t wind_data_buffer[200];
            memset(wind_data_buffer, 0, 200);

            wind_data_temp = (wind_send_data_t *)mqtt_data_struct;
            sprintf(wind_data_buffer, "{\"serial\":\"001\",\"type\":\"wind\",\"wind_rpm\":\"%d\",\"wind_power\":\"%.2f\",\"wind_volt\":\"%.2f\",\"wind_curr\":\"%.2f\"}", wind_data_temp->speed, wind_data_temp->power, wind_data_temp->voltage, wind_data_temp->current);

            while (check_result == _NOT_DEFINE_) /* _NOT_DEFINE_ means that is in progress */
            {
                check_result = MQTT_PubTopic(CLIENT_ID, MQTT_MSG_ID, MQTT_QOS_PUB, MQTT_RETAIN, MQTT_PUB_TOPIC,
                                             strlen(wind_data_buffer), wind_data_buffer);
            }

            if (check_result == _TRUE_)
            {
                return_function = true;
            }

            break;
        }

        case SYSTEM_DATA:
        {
            system_send_data_t *system_data_temp = NULL;
            uint8_t system_data_buffer[200];
            memset(system_data_buffer, 0, 200);

            system_data_temp = (system_send_data_t *)mqtt_data_struct;
            sprintf(system_data_buffer, "{\"serial\":\"001\",\"type\":\"system\",\"sys_status\":\"%d\",\"sys_err_code\":\"%d\",\"sys_total_energy\":\"%.2f\",\"sys_mosfet_temp\":\"%d\"}", system_data_temp->controller_state, system_data_temp->error_code, system_data_temp->total_energy, system_data_temp->temperature);

            while (check_result == _NOT_DEFINE_) /* _NOT_DEFINE_ means that is in progress */
            {
                check_result = MQTT_PubTopic(CLIENT_ID, MQTT_MSG_ID, MQTT_QOS_PUB, MQTT_RETAIN, MQTT_PUB_TOPIC,
                                             strlen(system_data_buffer), system_data_buffer);
            }

            if (check_result == _TRUE_)
            {
                return_function = true;
            }

            break;
        }

        case NONE_DATA:
            break;

        default:
            break;
        }
    }
    return return_function;
}

/* Get data field inside MQTT received message from server */
bool MQTT_Get_DataField(uint8_t *received_sub_buffer, uint8_t *dataField_buffer)
{
    bool return_function = false;

    uint8_t comma_count, j;
    memset(dataField_buffer, 0, 260);
    comma_count = 0;
    j = 0;
    for (uint8_t i = 0; i < (strlen((char *)received_sub_buffer) - 3); i++) /* minus 3 because i want to clear '"' at the end of line */
    {
        if (return_function == true)
        {
            dataField_buffer[j] = (char)received_sub_buffer[i];
            j++;
        }
        if (received_sub_buffer[i] == '\"')
        {
            comma_count++;
            if (comma_count == 3)
            {
                return_function = true;
            }
        }
    }
    return return_function;
}

/**
 * Get the received data if the data is available, data will be saved to a dedicated type buffer.
 * @return: Return the type of data.
 */
mqtt_transferring_data_e MQTT_Receive_Data(battery_received_data_t *battery_data_receive_buffer, wind_received_data_t *wind_data_receive_buffer, system_received_data_t *system_data_receive_buffer)
{
    mqtt_transferring_data_e return_function = NONE_DATA;
    uint8_t DataField_Buffer[260]; /* Buffer size is 260 due to maximum received size are 255 bytes */
    uint8_t JSON_value[100];
    if (MQTT_received_data_type.Is_Data_From_Server == true)
    {
        if (MQTT_Get_DataField(MQTT_Response_Server, DataField_Buffer))
        {
            memset((char *)JSON_value, 0, 100);
            if (parse_JSON((char *)DataField_Buffer, "type", (char *)JSON_value))
            {
                if (strcmp(JSON_value, "battery") == 0)
                {
                    memset((char *)JSON_value, 0, strlen((char *)JSON_value));
                    if (parse_JSON((char *)DataField_Buffer, "cap", (char *)JSON_value))
                    {
                        battery_data_receive_buffer->capacity = atoi(JSON_value);
                    }

                    memset((char *)JSON_value, 0, strlen((char *)JSON_value));
                    if (parse_JSON((char *)DataField_Buffer, "ov_vol", (char *)JSON_value))
                    {
                        battery_data_receive_buffer->over_voltage = atoi(JSON_value);
                    }

                    memset((char *)JSON_value, 0, strlen((char *)JSON_value));
                    if (parse_JSON((char *)DataField_Buffer, "ud_vol", (char *)JSON_value))
                    {
                        battery_data_receive_buffer->under_voltage = atoi(JSON_value);
                    }

                    return_function = BATTERY_DATA;
                }
                else if (strcmp(JSON_value, "wind") == 0)
                {
                    memset((char *)JSON_value, 0, strlen((char *)JSON_value));
                    if (parse_JSON((char *)DataField_Buffer, "max_vol", (char *)JSON_value))
                    {
                        wind_data_receive_buffer->max_voltage = atoi(JSON_value);
                    }

                    memset((char *)JSON_value, 0, strlen((char *)JSON_value));
                    if (parse_JSON((char *)DataField_Buffer, "max_cur", (char *)JSON_value))
                    {
                        wind_data_receive_buffer->max_current = atoi(JSON_value);
                    }

                    memset((char *)JSON_value, 0, strlen((char *)JSON_value));
                    memset((char *)(wind_data_receive_buffer->generator_pole), 0, strlen((char *)(wind_data_receive_buffer->generator_pole)));
                    if (parse_JSON((char *)DataField_Buffer, "pole", (char *)JSON_value))
                    {
                        for (int i = 0; i < strlen(JSON_value); i++)
                        {
                            wind_data_receive_buffer->generator_pole[i] = JSON_value[i];
                        }
                    }

                    memset((char *)JSON_value, 0, strlen((char *)JSON_value));
                    if (parse_JSON((char *)DataField_Buffer, "chrg_vol", (char *)JSON_value))
                    {
                        wind_data_receive_buffer->start_charging_voltage = atoi(JSON_value);
                    }

                    memset((char *)JSON_value, 0, strlen((char *)JSON_value));
                    if (parse_JSON((char *)DataField_Buffer, "max_spd", (char *)JSON_value))
                    {
                        wind_data_receive_buffer->max_rotate_speed = atoi(JSON_value);
                    }

                    return_function = WIND_DATA;
                }
                else if (strcmp(JSON_value, "system") == 0)
                {
                    memset((char *)JSON_value, 0, strlen((char *)JSON_value));
                    if (parse_JSON((char *)DataField_Buffer, "ov_out_vol", (char *)JSON_value))
                    {
                        system_data_receive_buffer->over_output_voltage = atoi(JSON_value);
                    }

                    memset((char *)JSON_value, 0, strlen((char *)JSON_value));
                    memset((char *)(system_data_receive_buffer->address_controller_modbus), 0, strlen((char *)(system_data_receive_buffer->address_controller_modbus)));
                    if (parse_JSON((char *)DataField_Buffer, "add_ctler_mdbus", (char *)JSON_value))
                    {
                        for (int i = 0; i < strlen(JSON_value); i++)
                        {
                            system_data_receive_buffer->address_controller_modbus[i] = JSON_value[i];
                        }
                    }

                    return_function = SYSTEM_DATA;
                }
            }
        }
        memset(MQTT_Response_Server, 0, strlen(MQTT_Response_Server));
        MQTT_received_data_type.Is_Data_From_Server = false;
    }
    return return_function;
}
