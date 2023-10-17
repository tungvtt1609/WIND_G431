/*
 * up_firm.h
 *
 *  Created on: Oct 3, 2023
 *      Author: TungVT
 */

#ifndef SRC_UP_FIRM_H_
#define SRC_UP_FIRM_H_

#include "stdio.h"
#include "stdint.h"

void UP_Process(void);
void UP_Time1kHz(void);

typedef enum ID_UP_e
{
	ID_UPDATE_FW,
	ID_READ_DATA,
	ID_READ_SETTING,
	ID_READ_FWVER,
	ID_READ_END,
	ID_UP_MAX
}ID_UP_e;

typedef enum UP_STS_e
{
	UP_NONE,
	UP_GET,
	UP_SET,
	U_READ_DATA,
	UP_READ_SET,
}UP_STS_e;

typedef void (*UP_Function)();
typedef struct up_cmd_t
{
	ID_UP_e id;
	uint32_t up_cmd;
	UP_STS_e up_sts;
    volatile float *value_addr;
    UP_Function up_func;
}up_cmd_t;
#endif /* SRC_UP_FIRM_H_ */
