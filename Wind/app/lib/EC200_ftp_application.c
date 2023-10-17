#include "EC200_ftp_application.h"

ftp_loginServer_state_e ftp_loginServer_state = FTP_LOGIN_IDLE;
ftp_bootloader_processing_t ftp_bootloader_processing = {0x00, 0xFFFFFFFF};

/* Extern variables */
extern uint8_t EC200_Command_Buffer[RECEIVE_SIZE];
extern uint8_t FTP_Response_Server[COMMAND_SIZE];
extern ec200_simStart_state_e ec200_simStart_state;
extern ftp_output_mode_e ftp_output_mode;
extern FTP_received_data_type_t FTP_received_data_type;
extern FTP_preprocessing_data_t FTP_preprocessing_data;
/* Timeout variables */
extern bool enable_timeout;
extern uint32_t ec200_timeout;
extern FLASH_PART_e part_select;
extern uint8_t reset_sim_countdown;
void FTP_Trigger_SIM_Restart(void)
{
    /* Reset EC200 module using RESET PIN */
    EC200_RESET();
    ec200_timeout = EC200_RESET_TIMEOUT;
    reset_sim_countdown = EC200_RESET_COUNTDOWN;
    enable_timeout = false;
    ftp_loginServer_state = FTP_LOGIN_IDLE;
    ec200_simStart_state = EC200_POWER_OFF;
    EC200_Delayms(1000); /* So important */
}

/******************************************** Setup functions ****************************************************/

boolean_3_state_e FTP_Set_TCPIP_Parameters(uint8_t contextID, uint8_t contextType, uint8_t *APN)
{
    boolean_3_state_e return_function = _NOT_DEFINE_;
    static uint8_t setTCPIP_params_step = 0;
    uint8_t buff_temp[50];

    if (setTCPIP_params_step == 0)
    {
        memset(buff_temp, 0, 50);
        sprintf(buff_temp, "AT+QICSGP=%d,%d,%s\r", contextID, contextType, APN);
        EC200_SendCommand(buff_temp);
        setTCPIP_params_step = 1;
    }
    else if (setTCPIP_params_step == 1)
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
            setTCPIP_params_step = 0;
        }
        /* Timeout */
        if (ec200_timeout == 0)
        {
            setTCPIP_params_step = 0;
            ec200_timeout = EC200_RESET_TIMEOUT;
            enable_timeout = false;
            /* TODO: Restart should be added here */
        }
    }
    return return_function;
}

boolean_3_state_e FTP_Activate_PDP(uint8_t contextID)
{
    boolean_3_state_e return_function = _NOT_DEFINE_;
    static uint8_t activate_PDP_step = 0;
    uint8_t buff_temp[20];

    if (activate_PDP_step == 0)
    {
        memset(buff_temp, 0, 20);
        sprintf(buff_temp, "AT+QIACT=%d\r", contextID);
        EC200_SendCommand(buff_temp);
        activate_PDP_step = 1;
    }
    else if (activate_PDP_step == 1)
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
            activate_PDP_step = 0;
        }
        /* Timeout */
        if (ec200_timeout == 0)
        {
            activate_PDP_step = 0;
            ec200_timeout = EC200_RESET_TIMEOUT;
            enable_timeout = false;
            /* TODO: Restart should be added here */
        }
    }
    return return_function;
}

boolean_3_state_e FTP_Deactivate_PDP(uint8_t contextID)
{
    boolean_3_state_e return_function = _NOT_DEFINE_;
    static uint8_t deactivate_PDP_step = 0;
    uint8_t buff_temp[20];

    if (deactivate_PDP_step == 0)
    {
        memset(buff_temp, 0, 20);
        sprintf(buff_temp, "AT+QIDEACT=%d\r", contextID);
        EC200_SendCommand(buff_temp);
        deactivate_PDP_step = 1;
    }
    else if (deactivate_PDP_step == 1)
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
            deactivate_PDP_step = 0;
        }
        /* Timeout */
        if (ec200_timeout == 0)
        {
            deactivate_PDP_step = 0;
            ec200_timeout = EC200_RESET_TIMEOUT;
            enable_timeout = false;
            /* TODO: Restart should be added here */
        }
    }
    return return_function;
}

boolean_3_state_e FTP_Configure_PDP(uint8_t contextID, uint8_t *username, uint8_t *password, uint8_t filetype, uint8_t transmode, uint16_t resptimeout)
{
    boolean_3_state_e return_function = _NOT_DEFINE_;
    static uint8_t configure_PDP_step = 0;
    uint8_t buff_temp[50];

    if (configure_PDP_step == 0)
    {
        memset(buff_temp, 0, 50);
        sprintf(buff_temp, "AT+QFTPCFG=\"contextid\",%d\r", contextID);
        EC200_SendCommand(buff_temp);
        configure_PDP_step = 1;
    }
    else if (configure_PDP_step == 1)
    {
        if (EC200_ReceiveCommand(EC200_Command_Buffer))
        {
            if (EC200_CompareCommand(EC200_Command_Buffer, "\r\nOK\r\n"))
            {
                configure_PDP_step = 2;
            }
            else /* Error command */
            {
                return_function = _FALSE_;
                configure_PDP_step = 0;
            }
        }
        /* Timeout */
        if (ec200_timeout == 0)
        {
            configure_PDP_step = 0;
            ec200_timeout = EC200_RESET_TIMEOUT;
            enable_timeout = false;
            /* TODO: Restart should be added here */
        }
    }
    else if (configure_PDP_step == 2)
    {
        memset(buff_temp, 0, 50);
        sprintf(buff_temp, "AT+QFTPCFG=\"account\",%s,%s\r", username, password);
        EC200_SendCommand(buff_temp);
        configure_PDP_step = 3;
    }
    else if (configure_PDP_step == 3)
    {
        if (EC200_ReceiveCommand(EC200_Command_Buffer))
        {
            if (EC200_CompareCommand(EC200_Command_Buffer, "\r\nOK\r\n"))
            {
                configure_PDP_step = 4;
            }
            else /* Error command */
            {
                return_function = _FALSE_;
                configure_PDP_step = 0;
            }
        }
        /* Timeout */
        if (ec200_timeout == 0)
        {
            configure_PDP_step = 0;
            ec200_timeout = EC200_RESET_TIMEOUT;
            enable_timeout = false;
            /* TODO: Restart should be added here */
        }
    }
    else if (configure_PDP_step == 4)
    {
        memset(buff_temp, 0, 50);
        sprintf(buff_temp, "AT+QFTPCFG=\"filetype\",%d\r", filetype);
        EC200_SendCommand(buff_temp);
        configure_PDP_step = 5;
    }
    else if (configure_PDP_step == 5)
    {
        if (EC200_ReceiveCommand(EC200_Command_Buffer))
        {
            if (EC200_CompareCommand(EC200_Command_Buffer, "\r\nOK\r\n"))
            {
                configure_PDP_step = 6;
            }
            else /* Error command */
            {
                return_function = _FALSE_;
                configure_PDP_step = 0;
            }
        }
        /* Timeout */
        if (ec200_timeout == 0)
        {
            configure_PDP_step = 0;
            ec200_timeout = EC200_RESET_TIMEOUT;
            enable_timeout = false;
            /* TODO: Restart should be added here */
        }
    }
    else if (configure_PDP_step == 6)
    {
        memset(buff_temp, 0, 50);
        sprintf(buff_temp, "AT+QFTPCFG=\"transmode\",%d\r", transmode);
        EC200_SendCommand(buff_temp);
        configure_PDP_step = 7;
    }
    else if (configure_PDP_step == 7)
    {
        if (EC200_ReceiveCommand(EC200_Command_Buffer))
        {
            if (EC200_CompareCommand(EC200_Command_Buffer, "\r\nOK\r\n"))
            {
                configure_PDP_step = 8;
            }
            else /* Error command */
            {
                return_function = _FALSE_;
                configure_PDP_step = 0;
            }
        }
        /* Timeout */
        if (ec200_timeout == 0)
        {
            configure_PDP_step = 0;
            ec200_timeout = EC200_RESET_TIMEOUT;
            enable_timeout = false;
            /* TODO: Restart should be added here */
        }
    }
    else if (configure_PDP_step == 8)
    {
        memset(buff_temp, 0, 50);
        sprintf(buff_temp, "AT+QFTPCFG=\"rsptimeout\",%d\r", resptimeout);
        EC200_SendCommand(buff_temp);
        configure_PDP_step = 9;
    }
    else if (configure_PDP_step == 9)
    {
        if (EC200_ReceiveCommand(EC200_Command_Buffer))
        {
            if (EC200_CompareCommand(EC200_Command_Buffer, "\r\nOK\r\n"))
            {
                return_function = _TRUE_;
                configure_PDP_step = 0;
            }
            else /* Error command */
            {
                return_function = _FALSE_;
                configure_PDP_step = 0;
            }
        }
        /* Timeout */
        if (ec200_timeout == 0)
        {
            configure_PDP_step = 0;
            ec200_timeout = EC200_RESET_TIMEOUT;
            enable_timeout = false;
            /* TODO: Restart should be added here */
        }
    }

    return return_function;
}

/******************************************* Server interaction functions *****************************************************/

boolean_3_state_e FTP_Login_Server(uint8_t *hostname, uint16_t port)
{
    boolean_3_state_e return_function = _NOT_DEFINE_;
    static uint8_t login_server_step = 0;
    uint8_t buff_temp[50];

    if (login_server_step == 0)
    {
        memset(buff_temp, 0, 50);
        sprintf(buff_temp, "AT+QFTPOPEN=%s,%d\r", hostname, port);
        EC200_SendCommand(buff_temp);
        login_server_step = 1;
    }
    else if (login_server_step == 1)
    {
        if (EC200_ReceiveCommand(EC200_Command_Buffer))
        {
            /* Case 1: Received full-response */

            /* Case 2: Received half-response */

            if (EC200_CompareCommand(EC200_Command_Buffer, "\r\nOK\r\n"))
            {
                /* Do nothing */
            }
            else if (EC200_CompareCommand(EC200_Command_Buffer, "\r\nOK\r\n\r\n+QFTPOPEN: 0,0\r\n") || EC200_CompareCommand(EC200_Command_Buffer, "\r\n+QFTPOPEN: 0,0\r\n"))
            {
                return_function = _TRUE_;
                login_server_step = 0;
            }
            else /* Error command */
            {
                return_function = _FALSE_;
                login_server_step = 0;
            }
        }
        /* Timeout */
        if (ec200_timeout == 0)
        {
            login_server_step = 0;
            ec200_timeout = EC200_RESET_TIMEOUT;
            enable_timeout = false;
            /* TODO: Restart should be added here */
        }
    }
    return return_function;
}

boolean_3_state_e FTP_Logout_Server(void)
{
    boolean_3_state_e return_function = _NOT_DEFINE_;
    static uint8_t logout_server_step = 0;

    if (logout_server_step == 0)
    {
        EC200_SendCommand("AT+QFTPCLOSE\r");
        logout_server_step = 1;
    }
    else if (logout_server_step == 1)
    {
        if (EC200_ReceiveCommand(EC200_Command_Buffer))
        {
            /* Case 1: Received full-response */

            /* Case 2: Received half-response */

            if (EC200_CompareCommand(EC200_Command_Buffer, "\r\nOK\r\n"))
            {
                /* Do nothing */
            }
            else if (EC200_CompareCommand(EC200_Command_Buffer, "\r\nOK\r\n\r\n+QFTPCLOSE: 0,0\r\n") || EC200_CompareCommand(EC200_Command_Buffer, "\r\n+QFTPCLOSE: 0,0\r\n"))
            {
                return_function = _TRUE_;
                logout_server_step = 0;
            }
            else /* Error command */
            {
                return_function = _FALSE_;
                logout_server_step = 0;
            }
        }
        /* Timeout */
        if (ec200_timeout == 0)
        {
            logout_server_step = 0;
            ec200_timeout = EC200_RESET_TIMEOUT;
            enable_timeout = false;
            /* TODO: Restart should be added here */
        }
    }
    return return_function;
}

boolean_3_state_e FTP_Enter_Directory(uint8_t *dir)
{
    boolean_3_state_e return_function = _NOT_DEFINE_;
    static uint8_t dir_step = 0;
    uint8_t buff_temp[50];

    if (dir_step == 0)
    {
        memset(buff_temp, 0, 50);
        sprintf(buff_temp, "AT+QFTPCWD=%s\r", dir);
        EC200_SendCommand(buff_temp);
        dir_step = 1;
    }
    else if (dir_step == 1)
    {
        if (EC200_ReceiveCommand(EC200_Command_Buffer))
        {
            /* Case 1: Received full-response */

            /* Case 2: Received half-response */

            if (EC200_CompareCommand(EC200_Command_Buffer, "\r\nOK\r\n"))
            {
                /* Do nothing */
            }
            else if (EC200_CompareCommand(EC200_Command_Buffer, "\r\nOK\r\n\r\n+QFTPCWD: 0,0\r\n") || EC200_CompareCommand(EC200_Command_Buffer, "\r\n+QFTPCWD: 0,0\r\n"))
            {
                return_function = _TRUE_;
                dir_step = 0;
            }
            else /* Error command */
            {
                return_function = _FALSE_;
                dir_step = 0;
            }
        }
        /* Timeout */
        if (ec200_timeout == 0)
        {
            dir_step = 0;
            ec200_timeout = EC200_RESET_TIMEOUT;
            enable_timeout = false;
            /* TODO: Restart should be added here */
        }
    }
    return return_function;
}

boolean_3_state_e FTP_List_Content_Directory(uint8_t *dir)
{
    boolean_3_state_e return_function = _NOT_DEFINE_;
    static uint8_t list_dir_step = 0;
    uint8_t buff_temp[50];

    if (list_dir_step == 0)
    {
        memset(buff_temp, 0, 50);
        sprintf(buff_temp, "AT+QFTPLIST=%s\r", dir);
        EC200_SendCommand(buff_temp);
        list_dir_step = 1;
    }
    else if (list_dir_step == 1)
    {
        if (FTP_received_data_type.Is_Data_From_Server == true)
        {
            if (strstr(FTP_Response_Server, "+QFTPLIST: 0") != NULL)
            {
                /* Delay to get entire of received data frame */
                EC200_Delayms(5);
                FTP_received_data_type.Is_Data_From_Server = false;
                ftp_output_mode = FTP_COMMAND_MODE;

                list_dir_step = 0;
                return_function = _TRUE_;
            }
        }
    }

    return return_function;
}

boolean_3_state_e FTP_List_Filename_Directory(uint8_t *dir)
{
    boolean_3_state_e return_function = _NOT_DEFINE_;
    static uint8_t list_filename_step = 0;
    uint8_t buff_temp[50];

    if (list_filename_step == 0)
    {
        memset(buff_temp, 0, 50);
        sprintf(buff_temp, "AT+QFTPNLST=%s\r", dir);
        EC200_SendCommand(buff_temp);
        list_filename_step = 1;
    }
    else if (list_filename_step == 1) /* Get data from server */
    {
        if (FTP_received_data_type.Is_Data_From_Server == true)
        {
            if (strstr(FTP_Response_Server, "+QFTPNLST: 0") != NULL)
            {
                /* Delay to get entire of received data frame */
                EC200_Delayms(5);
                FTP_received_data_type.Is_Data_From_Server = false;
                ftp_output_mode = FTP_COMMAND_MODE;

                list_filename_step = 0;
                return_function = _TRUE_;
            }
        }
    }
    return return_function;
}

/**
 * Get the content of a file.
 * @download_len: not greater than 480 bytes
 * @return: the length of data field
 */
uint16_t FTP_Get_Content_InFile(uint8_t *filename, uint8_t *local_name, uint32_t start_position, uint16_t download_len)
{
    uint16_t return_function = 0;
    static uint8_t get_file_step = 0;
    uint8_t buff_temp[50];

    if (get_file_step == 0)
    {
        memset(buff_temp, 0, 50);
        sprintf(buff_temp, "AT+QFTPGET=%s,%s,%d,%d\r", filename, local_name, start_position, download_len);
        EC200_SendCommand(buff_temp);
        get_file_step = 1;
    }
    else if (get_file_step == 1)
    {
        if (FTP_received_data_type.Is_Data_From_Server == true)
        {
            if (strstr(&FTP_Response_Server[FTP_preprocessing_data.EndOfStream_Position], "+QFTPGET: 0") != NULL)
            {
                /* Delay to get entire received data frame */
                EC200_Delayms(5);
                get_file_step = 0;
                if (strstr(&FTP_Response_Server[FTP_preprocessing_data.EndOfStream_Position], "+QFTPGET: 0,0") != NULL) /* File empty */
                {
                    return_function = 550; /* Assume return value = 550 that represents for end of file if the file size % 480 = 0 */
                }
                else
                {
                    /* Return must be a value <= 480 */
                    return_function = (FTP_preprocessing_data.EndOfStream_Position - 8);
                }
            }
            if (strstr(&FTP_Response_Server[FTP_preprocessing_data.EndOfStream_Position], "+QFTPGET: 627,550") != NULL) /* Assume the end of the file is reached and the file size % 480 = 0 */
            {
                get_file_step = 0;
                /* Return a value > 480*/
                return_function = 550; /* Assume return value = 550 that represents for end of file if the file size % 480 = 0 */
            }
        }
    }
    return return_function;
}

/************************************************************* Sequence functions *************************************************************/
bool EC200_FTP_ConnectToServer(void)
{
    bool return_function = false;
    boolean_3_state_e check_result = _NOT_DEFINE_;
    if (EC200_SIM_Start()) /* If EC200 has been successfully started */
    {
        switch (ftp_loginServer_state)
        {
        case FTP_LOGIN_IDLE:
            check_result = FTP_Set_TCPIP_Parameters(FTP_CONTEXT_ID, FTP_CONTEXT_TYPE, FTP_APN);
            if (check_result == _TRUE_)
            {
                ftp_loginServer_state = FTP_SET_TCPIP_DONE;
                reset_sim_countdown = EC200_RESET_COUNTDOWN;
                EC200_Delayms(500); /* Delay after each state */
            }
            else if (check_result == _FALSE_)
            {
                reset_sim_countdown--;
                if (reset_sim_countdown == 0)
                {
                    ftp_loginServer_state = FTP_RESET;
                }
            }

            break;

        case FTP_SET_TCPIP_DONE:
            check_result = FTP_Activate_PDP(FTP_CONTEXT_ID);
            if (check_result == _TRUE_)
            {
                ftp_loginServer_state = FTP_ACTIVATED_PDP;
                reset_sim_countdown = EC200_RESET_COUNTDOWN;
                EC200_Delayms(500); /* Delay after each state */
            }
            else if (check_result == _FALSE_)
            {
                reset_sim_countdown--;
                if (reset_sim_countdown == 0)
                {
                    ftp_loginServer_state = FTP_RESET;
                }
            }

            break;

        case FTP_ACTIVATED_PDP:
            check_result = FTP_Configure_PDP(FTP_CONTEXT_ID, FTP_USERNAME, FTP_PASSWORD, FTP_FILE_TYPE, 1, 90);
            if (check_result == _TRUE_)
            {
                ftp_loginServer_state = FTP_CONFIGURED;
                reset_sim_countdown = EC200_RESET_COUNTDOWN;
                EC200_Delayms(500); /* Delay after each state */
            }
            else if (check_result == _FALSE_)
            {
                reset_sim_countdown--;
                if (reset_sim_countdown == 0)
                {
                    ftp_loginServer_state = FTP_RESET;
                }
            }

            break;

        case FTP_CONFIGURED:
            check_result = FTP_Login_Server(FTP_HOSTNAME, FTP_PORT);
            if (check_result == _TRUE_)
            {
                ftp_loginServer_state = FTP_LOGIN_SERVER_DONE;
                reset_sim_countdown = EC200_RESET_COUNTDOWN;
                EC200_Delayms(500); /* Delay after each state */
            }
            else if (check_result == _FALSE_)
            {
                reset_sim_countdown--;
                if (reset_sim_countdown == 0)
                {
                    ftp_loginServer_state = FTP_RESET;
                }
            }

            break;

        case FTP_LOGIN_SERVER_DONE:
            return_function = true;

            break;

        case FTP_RESET:
            FTP_Trigger_SIM_Restart();
            break;

        default:
            break;
        }
    }

    return return_function;
}

/************************************************************* User functions *************************************************************/
void EC200_Write_To_Memory(uint8_t *buffer, uint32_t addr_offset, uint16_t dataField_len)
{
    BL_MEM_Part_Write(part_select, addr_offset, buffer, dataField_len);
}

void EC200_Read_From_Memory(uint8_t *buffer, uint32_t addr_offset, uint16_t dataField_len)
{
    BL_MEM_Part_Read(part_select, addr_offset, buffer, dataField_len);
}

void EC200_Calculate_CRC32(const uint8_t *data, uint32_t dataSize)
{
    static bool init_crc = false;
    if (init_crc == false)
    {
        crc32cal_Init();
        init_crc = true;
    }

    crc32cal_WriteData(data, dataSize);
}

uint32_t EC200_Get_CRC32(void)
{
    return crc32cal_Get32bitResult();
}

bool EC200_Verify_CRC32(uint32_t CRC32)
{
    bool return_function = false;
    if (ftp_bootloader_processing.CRC32 == CRC32)
    {
        return_function = true;
    }
    return return_function;
}

bool Update_Firmware(uint8_t *filename)
{
    bool return_function = false;
    uint32_t downloaded_bytes = 0;
    uint16_t number_bytes = 0;
    uint16_t blocks = 0;
    /* Read operation*/
    uint8_t read_mem_buff[510];
    uint16_t lastblock_bytes = 0;

    /* Start update firmware */
    memset(read_mem_buff, 0, 510);
    /* Download executed file from Server and write to flash */
    while (1)
    {
        number_bytes = FTP_Get_Content_InFile(filename, "COM:", downloaded_bytes, BYTES_BLOCK);
        if (number_bytes > 0) /* Received data successfully when number bytes greater than 0 */
        {
            if (number_bytes <= BYTES_BLOCK)
            {
                EC200_Write_To_Memory(FTP_Response_Server, downloaded_bytes, number_bytes);
                downloaded_bytes = (downloaded_bytes + number_bytes);
            }

            FTP_received_data_type.Is_Data_From_Server = false;
            ftp_output_mode = FTP_COMMAND_MODE;
            memset(FTP_Response_Server, 0, FTP_preprocessing_data.EndOfStream_Position + 15);
            FTP_preprocessing_data.EndOfStream_Position = 0;

            blocks++;

            /* Detect end of file */
            if (number_bytes < BYTES_BLOCK)
            {
                lastblock_bytes = number_bytes;
                break;
            }
            else if (number_bytes > BYTES_BLOCK)
            {
                lastblock_bytes = 0;
                break;
            }
        }
    }
    /* Read file.bin from flash */
    for (int i = 0; i < blocks; i++)
    {
        if (i == (blocks - 1))
        {
            memset(read_mem_buff, 0, sizeof(read_mem_buff));
            EC200_Read_From_Memory(read_mem_buff, BYTES_BLOCK * i, lastblock_bytes);
            EC200_Calculate_CRC32(read_mem_buff, lastblock_bytes);
        }
        else
        {
            EC200_Read_From_Memory(read_mem_buff, BYTES_BLOCK * i, BYTES_BLOCK);
            EC200_Calculate_CRC32(read_mem_buff, BYTES_BLOCK);
        }
    }
    /* Verify CRC32 */
    if (EC200_Verify_CRC32(EC200_Get_CRC32()))
    {
        return_function = true;
    }

    return return_function;
}
