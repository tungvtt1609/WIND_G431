/*
 * state_machine.h
 *
 *  Created on: Mar 17, 2023
 *      Author: Thanh Tung
 */

#ifndef SRC_STATE_MACHINE_H_
#define SRC_STATE_MACHINE_H_

#include "stdbool.h"
#include "stdint.h"
#include "error_check.h"

typedef enum {
	FUNCTION_NONE = 0,
	FUNCTION_RUNNING,
	FUNCTION_DONE,
	FUNCTION_FAIL
}FUNCTION_RETURN;

typedef FUNCTION_RETURN (*FnCheck)();

typedef enum {
	STATE_WIND_OFF 			= 0,
//	STATE_CHECK_ERR			,
	STATE_CHECK_WIND		,
//	STATE_CHECK_BOOST		,
	STATE_CLOSE_LOOP		,
	STATE_RESET				,
	STATE_ERR				,
//	STATE_ALL				,
	STATE_MAX
}STATE_MACHINE_ID;

extern STATE_MACHINE_ID 	g_current_state;
extern bool g_state_vin;
extern bool g_state_vout;
extern ERROR_CODE faultInfo;

void StateInit(void);

#endif /* SRC_STATE_MACHINE_H_ */
