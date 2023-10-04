/*
 * error_check.h
 *
 *  Created on: Mar 17, 2023
 *      Author: Thanh Tung
 */

#ifndef SRC_ERROR_CHECK_H_
#define SRC_ERROR_CHECK_H_

#include "types.h"
#include "stdlib.h"
//#include "state_machine.h"
#include "stdlib.h"
#include "driver/control/pin_func.h"



#define ERR_LOOP_DIV				5

#define MAX_ILOAD					20
#define MAX_IDUMP					2
#define MAX_PIN 					1200
#define MAX_POUT 					1200
#define MAX_VIN						48
#define MAX_VOUT					50
#define MIN_VIN						0
#define MIN_VOUT					0
#define MIN_VAC						110
#define MAX_VAC						264
#define MIN_VAC_WRN 				187
#define MAX_VAC_WRN					242
#define MIN_FAC						0
#define MAX_FAC						0
#define MIN_FAC_WRN					0
#define MAX_FAC_WRN					0
#define MIN_ROTOR					0
#define MAX_ROTOR					0
#define HIGH_TEMP1					0
#define HIGH_TEMP2					0
#define LOW_TEMP1					0
#define LOW_TEMP2					0

#define TIME_ZERO					0
#define TIME_1_S					(1000/ERR_LOOP_DIV)
#define TIME_VDC_WARNING			(1000/ERR_LOOP_DIV)
#define TIME_FAC_WARNING			(1800000/ERR_LOOP_DIV)
#define TIME_TEMP_CHK     			(1000/ERR_LOOP_DIV)
#define TIME_START_0				(0/ERR_LOOP_DIV)

typedef enum {
	ID_OK = 0,
	ID_OVDC1,		//V_in
	ID_OVDC2,		//V_out
	ID_UVDC1,		//V_in
	ID_UVDC2,		//V_out
	ID_OCDC1,		//I_load
	ID_OCDC2,		//I_dump
//	ID_OPIN,
//	ID_OPOUT,
//	ID_OVAC_BEGIN,
//	ID_OVAC_RUNNING,
//	ID_OVAC_WARNING,
//	ID_UVAC_BEGIN,
//	ID_UVAC_RUNNING,
//	ID_UVAC_WARNING,
//	ID_OVBAT_CHAR,
//	ID_UVBAT_CHAR,
//	ID_OIBAT_CHAR,
//	ID_UIBAT_CHAR,
//	ID_OVBAT_DCHAR,
//	ID_UVBAT_DCHAR,
//	ID_OIBAT_DCHAR,
//	ID_UIBAT_DCHAR,
//	ID_BOOST_LTEMP,
//	ID_BOOST_HTEMP,
//	ID_AQ_LTEMP,
//	ID_AQ_HTEMP,
//	ID_OF_BEGIN,
//	ID_OF_RUNNING,
//	ID_OF_WARNING,
//	ID_UF_BEGIN,
//	ID_UF_RUNNING,
//	ID_UF_WARNING,
	ID_MAX
}ERROR_ID;

typedef enum {
	CODE_OK = 0,
	BoostFault,
	NoWindInput,
	NoBatInput,
	CODE_BOOST_LTEMP,
	CODE_BOOST_HTEMP,
	CODE_AQ_LTEMP,
	CODE_AQ_HTEMP,
	CODE_OVDC1,		//V_in
	CODE_OVDC2,		//V_out
	CODE_UVDC1,		//V_in
	CODE_UVDC2,		//V_out
	CODE_OCDC1,		//I_load
	CODE_OCDC2,		//I_load
	CODE_OPIN,
	CODE_OPOUT,
	CODE_OVAC_BEGIN,
	CODE_OVAC_RUNNING,
	CODE_OVAC_WARNING,
	CODE_UVAC_BEGIN,
	CODE_UVAC_RUNNING,
	CODE_UVAC_WARNING,
	CODE_OVBAT_CHAR,
	CODE_UVBAT_CHAR,
	CODE_OIBAT_CHAR,
	CODE_UIBAT_CHAR,
	CODE_OVBAT_DCHAR,
	CODE_UVBAT_DCHAR,
	CODE_OIBAT_DCHAR,
	CODE_UIBAT_DCHAR,
	CODE_OF_BEGIN,
	CODE_OF_RUNNING,
	CODE_OF_WARNING,
	CODE_UF_BEGIN,
	CODE_UF_RUNNING,
	CODE_UF_WARNING,
	CODE_ERR = 99
}ERROR_CODE;

void SetErr(ERROR_ID id);
void SetErr(ERROR_ID id);
void ErrorCheckInit(void);
void ErrorCheck_Running(void);
ERROR_CODE getFaultInfo();

#endif /* SRC_ERROR_CHECK_H_ */
