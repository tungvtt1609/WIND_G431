#ifndef _EC200_APP_H_
#define _EC200_APP_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "EC200_uart.h"
#include "EC200_JSON_parsing.h"

#define EC200_RESET_PIN_HIGH()   Pin_Func_TurnOn(EC200_RESET_PIN)
#define EC200_RESET_PIN_LOW()   Pin_Func_TurnOff(EC200_RESET_PIN)
#define EC200_POWER_PIN_HIGH()   Pin_Func_TurnOn(EC200_POWER_PIN)
#define EC200_POWER_PIN_LOW()   Pin_Func_TurnOff(EC200_POWER_PIN)

#define RECEIVE_SIZE 400
#define EC200_RESET_COUNTDOWN 5

/* MQTT parameters */
#define KEEP_ALIVE_TIME 120 /* 120 seconds */
#define CLIENT_ID 2
#define MQTT_BROKER "broker.mqtt.cool"
#define MQTT_PORT 1883
#define MQTT_CLIENT_NAME "abc"
#define MQTT_USER ""
#define MQTT_PASSWORD ""
#define MQTT_MSG_ID 2
#define MQTT_QOS_SUB 0 /* If modify QOS, let's check again fixed response from server */
#define MQTT_QOS_PUB 1 /* If modify QOS, let's check again fixed response from server */
#define MQTT_RETAIN 0

/* MQTT PUB/SUB topics */
#define MQTT_PUB_TOPIC "vmc/pub"
#define MQTT_SUB_TOPIC "vmc/sub"

typedef enum
{
    _FALSE_ = false,
    _TRUE_ = true,
    _NOT_DEFINE_
} boolean_3_state_e;

typedef enum
{
    EC200_POWER_OFF = 0,
    EC200_POWERED_ON,
    EC200_STARTING_DONE,
    EC200_RESTART
} ec200_simStart_state_e;

typedef enum
{
    MQTT_IDLE = 0,
    MQTT_SENT_KEEPALIVE,
    MQTT_SENT_RECEIVING_MODE,
    MQTT_OPENED,
    MQTT_CONNECTED_DONE,
    MQTT_SUBCRIBE,
    MQTT_RESET
} ec200_simConnectServer_state_e;

/* MQTT transferring data types */
typedef enum
{
    NONE_DATA = 0,
    BATTERY_DATA,
    WIND_DATA,
    SYSTEM_DATA
} mqtt_transferring_data_e;

/* MQTT transferring data structs */
typedef struct
{
    uint16_t capacity; //cap(Ah) 
    uint16_t over_voltage; //ov_vol(Vol)
    uint16_t under_voltage; //ud_vol(Vol)

}battery_data_t;

typedef struct
{
    uint16_t max_voltage; //max_vol(Vol)
    uint16_t max_current; //max_cur(Ampe)
    char generator_pole[100]; //pole
    uint16_t start_charging_voltage; //chrg_vol(Vol)
    uint16_t max_rotate_speed; //max_spd(rpm)
}wind_data_t;

typedef struct
{
    uint16_t over_output_voltage; //ov_out_vol(Vol)
    char address_controller_modbus[100]; //add_ctler_mdbus
}system_data_t;


/********************************************* USER FUNCTION ***************************************************/
void EC200_Init(void);
bool EC200_MQTT_ConnectToServer(void);
bool MQTT_Transmit_Data(void *mqtt_data_struct, mqtt_transferring_data_e transferring_data_type);
mqtt_transferring_data_e MQTT_Receive_Data(battery_data_t *battery_data_receive_buffer, wind_data_t *wind_data_receive_buffer, system_data_t *system_data_receive_buffer);

#endif /* _EC200_APP_H_ */