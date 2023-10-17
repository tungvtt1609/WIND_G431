#ifndef _EC200_FTP_APP_H_
#define _EC200_FTP_APP_H_

#include "EC200_common/EC200_common.h"
#include "driver/BL_memory.h"
#include "src/update_firmware.h"

#define FTP_CONTEXT_ID 1
#define FTP_CONTEXT_TYPE 1
#define FTP_APN "v-internet" /* Sim Viettel */
#define FTP_HOSTNAME "117.1.17.14"
#define FTP_PORT 2121
#define FTP_USERNAME "emsftp"
#define FTP_PASSWORD "ems@admin"
#define FTP_FILE_TYPE 0 /* 0 is binary, 1 is ASCII */

#define BYTES_BLOCK 480 /* Must be a number that % 8 */

typedef enum
{
    FTP_LOGIN_IDLE = 0,
    FTP_SET_TCPIP_DONE,
    FTP_ACTIVATED_PDP,
    FTP_CONFIGURED,
    FTP_LOGIN_SERVER_DONE,
    FTP_RESET

} ftp_loginServer_state_e;

typedef struct
{
    uint8_t update_firmware_flag;
    uint32_t CRC32;
} ftp_bootloader_processing_t;

/* User functions */
boolean_3_state_e FTP_List_Content_Directory(uint8_t *dir);
bool EC200_FTP_ConnectToServer(void);
bool Update_Firmware(uint8_t *filename);

#endif /* _EC200_FTP_APP_H_ */
