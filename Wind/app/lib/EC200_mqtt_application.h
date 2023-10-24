#ifndef _EC200_APP_H_
#define _EC200_APP_H_

#include "EC200_common/EC200_common.h"
#include "EC200_common/EC200_JSON_parsing.h"
#include "EEPROM/EEROM24C256.h"
#include "src/interface.h"
#include "main.h"

/* MQTT parameters */
#define KEEP_ALIVE_TIME 120 /* 120 seconds */
#define CLIENT_ID 2
#define MQTT_BROKER "117.1.17.14"
// #define MQTT_BROKER "broker.mqtt.cool"
#define MQTT_PORT 53195
#define MQTT_CLIENT_NAME "abc"
#define MQTT_USER "admin_vmc"
#define MQTT_PASSWORD "admin@123"
#define MQTT_MSG_ID 2
#define MQTT_QOS_SUB 0 /* If modify QOS, let's check again fixed response from server */
#define MQTT_QOS_PUB 1 /* If modify QOS, let's check again fixed response from server */
#define MQTT_RETAIN 0

/* MQTT PUB/SUB topics */
#define MQTT_PUB_TOPIC "serial_002_sub"
#define MQTT_SUB_TOPIC "serial_002_pub"

#define MQTT_SENDING_PERIOD 5000 /* 5s */

typedef enum
{
    MQTT_IDLE = 0,
    MQTT_SENT_KEEPALIVE,
    MQTT_SENT_RECEIVING_MODE,
    MQTT_OPENED,
    MQTT_CONNECTED_DONE,
    MQTT_SUBCRIBED,
    MQTT_DELAY,
    MQTT_RESET
} mqtt_connectServer_state_e;

/* MQTT transferring data types */
typedef enum
{
    NONE_DATA = 0,
    BATTERY_DATA,
    WIND_DATA,
    SYSTEM_DATA,
    UPDATE_FW_DATA
} mqtt_transferring_data_e;

/************************************* Data format to send *****************************************/

typedef enum
{
    BAT_LOW = 0,
    BAT_NORMAL,
    BAT_FULL
} battery_capacity_e;

typedef enum
{
    WIND_OFF = 0,
    CHECK_WIND,
    CLOSE_LOOP,
    SYS_RESET,
    SYS_ERROR
} system_controller_state_e;

typedef enum
{
    SYS_CODE_OK = 0,
    SYS_NO_WIND_INPUT,
    SYS_NO_BAT,
    SYS_CODE_OVDC1,
    SYS_CODE_OVDC2,
    SYS_CODE_UVDC1,
    SYS_CODE_UVDC2,
    SYS_CODE_OCDC1,
    SYS_CODE_OCDC2
} system_error_code_e;

/* MQTT transferring data structs */

/***************** Send data ********************/
typedef struct
{
    battery_capacity_e state;
    uint16_t power;   //(W)
    uint16_t voltage; //(V)
    uint16_t current; //(A)
    //    uint8_t temperature;
} battery_send_data_t;

typedef struct
{
    uint16_t speed;   //(RPM)
    uint16_t power;   //(W)
    uint16_t voltage; //(V)
    //    uint16_t current; //(A)
} wind_send_data_t;

typedef struct
{
    system_controller_state_e controller_state;
    system_error_code_e error_code;
    uint16_t total_energy; //(kWh)
    uint16_t temperature;
    uint16_t rpm_gen;
    //    uint16_t temperature_boost;
} system_send_data_t;

/***************** Received data ********************/
typedef struct
{
    uint16_t serial;
    uint16_t capacity;      // cap(Ah)
    uint16_t over_voltage;  // ov_vol(Vol)
    uint16_t under_voltage; // ud_vol(Vol)

} battery_received_data_t;

typedef struct
{
    uint16_t serial;
    uint16_t max_voltage;            // max_vol(Vol)
    uint16_t max_current;            // max_cur(Ampe)
    char generator_pole[50];         // pole
    uint16_t start_charging_voltage; // chrg_vol(Vol)
    uint16_t max_rotate_speed;       // max_spd(rpm)
} wind_received_data_t;

typedef struct
{
    uint16_t serial;
    uint16_t max_voltage_output;
    uint16_t max_current_output;
    uint16_t over_voltage_output;       // ov_out_vol(Vol)
    uint16_t over_current_output;
    uint16_t over_voltage_input;
    uint16_t over_current_input;
    //char address_controller_modbus[50]; // add_ctler_mdbus
} system_received_data_t;

typedef struct
{
    uint16_t serial;
    char CRC32[4]; // crc32
} updatefw_received_data_t;

typedef struct
{
    uint16_t serial;
    uint16_t year; //year
    uint8_t month; //mon
    uint8_t date; //dat
    uint8_t hour; //hr
    uint8_t minute; //min
    uint8_t second; //sec
} datetime_received_data_t;

/********************************************* USER FUNCTIONS ***************************************************/
void MQTT_Trigger_SIM_Restart(void);
bool EC200_MQTT_ConnectToServer(void);
bool MQTT_Transmit_Data(void *mqtt_data_struct, mqtt_transferring_data_e transferring_data_type);
mqtt_transferring_data_e MQTT_Receive_Data(battery_received_data_t *battery_data_receive_buffer, wind_received_data_t *wind_data_receive_buffer, system_received_data_t *system_data_receive_buffer, updatefw_received_data_t *updatefw_data_receive_buffer);
bool MQTT_ACK_To_Server(void *mqtt_data_struct, mqtt_transferring_data_e transferring_data_type);
bool MQTT_Get_DateTime(datetime_received_data_t *datetime_received_data, uint32_t timeout_ms);

#endif /* _EC200_APP_H_ */
