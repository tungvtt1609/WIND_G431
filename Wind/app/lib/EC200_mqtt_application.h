#ifndef _EC200_APP_H_
#define _EC200_APP_H_

#include "EC200_common/EC200_common.h"
#include "EC200_common/EC200_JSON_parsing.h"
#include "main.h"

/* MQTT parameters */
#define KEEP_ALIVE_TIME 120 /* 120 seconds */
#define CLIENT_ID 2
#define MQTT_BROKER "117.1.17.14"
#define MQTT_PORT 1883
#define MQTT_CLIENT_NAME "abc"
#define MQTT_USER ""
#define MQTT_PASSWORD ""
#define MQTT_MSG_ID 2
#define MQTT_QOS_SUB 0 /* If modify QOS, let's check again fixed response from server */
#define MQTT_QOS_PUB 1 /* If modify QOS, let's check again fixed response from server */
#define MQTT_RETAIN 0

/* MQTT PUB/SUB topics */
#define MQTT_PUB_TOPIC "serial_001"
#define MQTT_SUB_TOPIC "vmc/sub"

typedef enum
{
    MQTT_IDLE = 0,
    MQTT_SENT_KEEPALIVE,
    MQTT_SENT_RECEIVING_MODE,
    MQTT_OPENED,
    MQTT_CONNECTED_DONE,
    MQTT_SUBCRIBED,
    MQTT_RESET
} mqtt_connectServer_state_e;

/* MQTT transferring data types */
typedef enum
{
    NONE_DATA = 0,
    BATTERY_DATA,
    WIND_DATA,
    SYSTEM_DATA
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
    BAT_TEMP_NORMAL = 0,
    BAT_TEMP_HIGH,
    BAT_TEMP_OTP,
    BAT_TEMP_ERROR
} battery_temperature_e;

typedef enum
{
    WIND_OFF = 0,
    CHECK_WIND,
    CLOSE_LOOP,
    RESET,
    ERROR
} system_controller_state_e;

typedef enum
{
    CODE_OK = 0,
    NO_WIND_INPUT,
    NO_BAT,
    CODE_OVDC1,
    CODE_OVDC2,
    CODE_UVDC1,
    CODE_UVDC2,
    CODE_OCDC1,
    CODE_OCDC2
} system_error_code_e;

typedef enum
{
    SYS_TEMP_NORMAL = 0,
    SYS_TEMP_HIGH,
    SYS_TEMP_OTP,
    SYS_TEMP_ERROR
} system_temperature_e;


/* MQTT transferring data structs */

/******* Send data ******/
typedef struct
{
    battery_capacity_e state;
    float power; //(W) 
    float voltage; //(V)
    float current; //(A)
    battery_temperature_e temperature;
}battery_send_data_t;

typedef struct
{
    uint16_t speed; //(RPM)
    float power; //(W) 
    float voltage; //(V)
    float current; //(A)
}wind_send_data_t;

typedef struct
{
    system_controller_state_e controller_state;
    system_error_code_e error_code;
    float total_energy; //(kWh)
    system_temperature_e temperature;
}system_send_data_t;

/******* Received data ******/
typedef struct
{
    uint16_t capacity; //cap(Ah) 
    uint16_t over_voltage; //ov_vol(Vol)
    uint16_t under_voltage; //ud_vol(Vol)

}battery_received_data_t;

typedef struct
{
    uint16_t max_voltage; //max_vol(Vol)
    uint16_t max_current; //max_cur(Ampe)
    char generator_pole[100]; //pole
    uint16_t start_charging_voltage; //chrg_vol(Vol)
    uint16_t max_rotate_speed; //max_spd(rpm)
}wind_received_data_t;

typedef struct
{
    uint16_t over_output_voltage; //ov_out_vol(Vol)
    char address_controller_modbus[100]; //add_ctler_mdbus
}system_received_data_t;


/********************************************* USER FUNCTIONS ***************************************************/
void MQTT_Trigger_SIM_Restart(void);
bool EC200_MQTT_ConnectToServer(void);
bool MQTT_Transmit_Data(void *mqtt_data_struct, mqtt_transferring_data_e transferring_data_type);
mqtt_transferring_data_e MQTT_Receive_Data(battery_received_data_t *battery_data_receive_buffer, wind_received_data_t *wind_data_receive_buffer, system_received_data_t *system_data_receive_buffer);

#endif /* _EC200_APP_H_ */