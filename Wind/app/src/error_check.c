/*
 * error_check.c
 *
 *  Created on: Mar 17, 2023
 *      Author: Thanh Tung
 */

#include "error_check.h"
#include "driver/control/pin_func.h"
#include "driver/measure/signal.h"
#include "porting/gpio/gpio.h"
#include "types.h"
#include "math.h"
#include "state_machine.h"

typedef enum {
	TYPE_UP = 0,
	TYPE_DOWN,
	TYPE_EXTERNAL
}ERROR_TYPE;

typedef enum {
	LEVEL_0 = 0,
	LEVEL_1
}ERROR_LEVEL;

typedef enum {
	ACTIVE = 0,
	DEACTIVE
}ERROR_ACTIVE;

typedef void (*FuncError)();

typedef struct {
	ERROR_ID 						id_err;
	ERROR_CODE 						code_err;
	STATE_MACHINE_ID				state_valid;
	float 							value_limit;
	ERROR_TYPE						type_check;
	uint32_t 						time_limit;
	uint32_t						time_count;
	ERROR_LEVEL						level_err;
	volatile float 					*value_addr;
	FuncError						func_check;
	bool							error_state;
	ERROR_ACTIVE					active_err;
}ListError;

static volatile uint32_t countList = 0;
static volatile float s_mError[ID_MAX];
static void UpdateCalc(void);
//static void FuncVDCCHK();

static ListError listError[ID_MAX] = {
		{ID_OK},

		//all state
		{ID_OCDC1,				CODE_OCDC1,					STATE_MAX,				MAX_ILOAD,				TYPE_UP,				TIME_ZERO,						TIME_START_0,				LEVEL_0,				&s_mError[ID_OCDC1],					NULL,			false,			ACTIVE},
		{ID_OCDC2,				CODE_OCDC2,					STATE_MAX,				MAX_IDUMP,				TYPE_UP,				TIME_ZERO,						TIME_START_0,				LEVEL_0,				&s_mError[ID_OCDC2],					NULL,			false,			ACTIVE},
		{ID_OVDC1,				CODE_OVDC1,					STATE_MAX,				MAX_VIN,				TYPE_UP,				TIME_ZERO,						TIME_START_0,				LEVEL_0,				&s_mError[ID_OVDC1],					NULL,			false,			ACTIVE},
		{ID_OVDC2,				CODE_OVDC2,					STATE_MAX, 				MAX_VOUT,				TYPE_UP,				TIME_ZERO,						TIME_START_0,				LEVEL_0,				&s_mError[ID_OVDC2],					NULL,			false,			ACTIVE},
//		{ID_UVDC1,				CODE_UVDC1,					STATE_MAX,				MIN_VIN,				TYPE_DOWN,			TIME_VDC_WARNING,			TIME_START_0,				LEVEL_0,				&s_mError[ID_UVDC1],					NULL,			false,			ACTIVE},
		{ID_UVDC2,				CODE_UVDC2,					STATE_MAX,				MIN_VOUT,				TYPE_DOWN,				TIME_VDC_WARNING, 				TIME_START_0,				LEVEL_0,				&s_mError[ID_UVDC2],					NULL,			false,			ACTIVE},

//		{ID_OVAC_BEGIN,		CODE_OVAC_BEGIN,		STATE_CHECK_ERR,	MAX_VAC_WRN,		TYPE_UP,				TIME_ZERO,						TIME_START_0,				LEVEL_0,				&s_mError[ID_OVAC_BEGIN],			NULL,			false,			ACTIVE},
//		{ID_OVAC_RUNNING,	CODE_OVAC_RUNNING,	STATE_ALL,				MAX_VAC,				TYPE_UP,				TIME_ZERO,						TIME_START_0, 			LEVEL_0,				&s_mError[ID_OVAC_RUNNING],		NULL,			false,			ACTIVE},
//		{ID_OVAC_WARNING,	CODE_OVAC_WARNING,	STATE_ALL,				MAX_VAC_WRN,		TYPE_UP,				TIME_ZERO,						TIME_START_0,				LEVEL_0,				&s_mError[ID_OVAC_WARNING],		NULL,			false,			ACTIVE},

//		{ID_UVAC_BEGIN,		CODE_UVAC_BEGIN,		STATE_CHECK_ERR,	MIN_VAC_WRN,		TYPE_DOWN,			TIME_ZERO,						TIME_START_0,				LEVEL_0,				&s_mError[ID_UVAC_BEGIN],			NULL,			false,			ACTIVE},
//		{ID_UVAC_RUNNING,	CODE_UVAC_RUNNING,	STATE_ALL,				MIN_VAC,				TYPE_DOWN,			TIME_ZERO,						TIME_START_0, 			LEVEL_0,				&s_mError[ID_UVAC_RUNNING],		NULL,			false,			ACTIVE},
//		{ID_UVAC_WARNING,	CODE_UVAC_WARNING,	STATE_ALL,				MIN_VAC_WRN,		TYPE_DOWN,			TIME_ZERO,						TIME_START_0,				LEVEL_0,				&s_mError[ID_UVAC_WARNING],		NULL,			false,			ACTIVE},

//		{ID_OF_BEGIN,			CODE_OF_BEGIN,			STATE_CHECK_ERR,	MAX_FAC_WRN,		TYPE_UP,				TIME_ZERO,						TIME_START_0,				LEVEL_0,				&s_mError[ID_OF_BEGIN],				NULL,			false,			ACTIVE},
//		{ID_OF_RUNNING,		CODE_OF_RUNNING,		STATE_ALL,				MAX_FAC,				TYPE_UP,				TIME_ZERO,						TIME_START_0,				LEVEL_0,				&s_mError[ID_OF_RUNNING],			NULL,			false,			ACTIVE},
//		{ID_OF_WARNING,		CODE_OF_WARNING,		STATE_ALL,				MAX_FAC_WRN,		TYPE_UP,				TIME_ZERO,						TIME_START_0,				LEVEL_0,				&s_mError[ID_OF_WARNING],			NULL,			false,			ACTIVE},

//		{ID_UF_BEGIN,			CODE_UF_BEGIN,			STATE_CHECK_ERR,	MIN_FAC_WRN,		TYPE_DOWN,			TIME_ZERO,						TIME_START_0,				LEVEL_0,				&s_mError[ID_UF_BEGIN],				NULL,			false,			ACTIVE},
//		{ID_UF_RUNNING,		CODE_UF_RUNNING,		STATE_ALL,				MIN_FAC,				TYPE_DOWN,			TIME_ZERO,						TIME_START_0,				LEVEL_0,				&s_mError[ID_UF_RUNNING],			NULL,			false,			ACTIVE},
//		{ID_UF_WARNING,		CODE_UF_WARNING,		STATE_ALL,				MIN_FAC_WRN,		TYPE_DOWN,			TIME_ZERO,						TIME_START_0,				LEVEL_0,				&s_mError[ID_UF_WARNING],			NULL,			false,			ACTIVE},

//		{ID_OPIN,					CODE_OPIN,					STATE_ALL,				MAX_PIN,				TYPE_UP,				TIME_ZERO,						TIME_START_0,				LEVEL_0,				&s_mError[ID_OPIN],						NULL,			false,			ACTIVE},
//		{ID_OPOUT,				CODE_OPOUT,					STATE_ALL,				MAX_POUT,				TYPE_UP,				TIME_ZERO,						TIME_START_0,				LEVEL_0,				&s_mError[ID_OPOUT],					NULL,			false,			ACTIVE},

//		{ID_BOOST_HTEMP, 	CODE_BOOST_HTEMP,		STATE_ALL,				HIGH_TEMP1,			TYPE_UP,				TIME_ZERO,						TIME_START_0,				LEVEL_0,				&s_mError[ID_BOOST_HTEMP],		NULL,			false,			ACTIVE},
//		{ID_BOOST_LTEMP,	CODE_BOOST_LTEMP,		STATE_ALL,				LOW_TEMP1,			TYPE_DOWN,			TIME_ZERO,						TIME_START_0,				LEVEL_0,				&s_mError[ID_BOOST_LTEMP],		NULL,			false,			ACTIVE},

//		{ID_AQ_HTEMP,			CODE_AQ_HTEMP,			STATE_ALL,				HIGH_TEMP2,			TYPE_DOWN,			TIME_ZERO,						TIME_START_0,				LEVEL_0,				&s_mError[ID_AQ_HTEMP],				NULL,			false,			ACTIVE},
//		{ID_AQ_LTEMP,			CODE_AQ_LTEMP,			STATE_ALL,				LOW_TEMP2,			TYPE_DOWN,			TIME_ZERO,						TIME_START_0,				LEVEL_0,				&s_mError[ID_AQ_LTEMP],				NULL,			false,			ACTIVE},

//		{ID_OVBAT_CHAR,		CODE_OVBAT_CHAR,		STATE_ALL,				1,							TYPE_UP,				0,										0,									0,							&s_mError[ID_OVBAT_CHAR],			NULL,			false,			ACTIVE},
//		{ID_UVBAT_CHAR,		CODE_UVBAT_CHAR,		STATE_ALL,				1,							TYPE_UP,				0,										0,									0,							&s_mError[ID_UVBAT_CHAR],			NULL,			false,			ACTIVE},
//		{ID_OIBAT_CHAR,		CODE_OIBAT_CHAR,		STATE_ALL,				1,							TYPE_UP,				0,										0,									0,							&s_mError[ID_OIBAT_CHAR],			NULL,			false,			ACTIVE},
//		{ID_UIBAT_CHAR,		CODE_UIBAT_CHAR,		STATE_ALL,				1,							TYPE_UP,				0,										0,									0,							&s_mError[ID_UIBAT_CHAR],			NULL,			false,			ACTIVE},
//		{ID_OVBAT_DCHAR,	CODE_OVBAT_DCHAR,		STATE_ALL,				1,							TYPE_UP,				0,										0,									0,							&s_mError[ID_OVBAT_DCHAR],		NULL,			false,			ACTIVE},
//		{ID_UVBAT_DCHAR,	CODE_UVBAT_CHAR,		STATE_ALL,				1,							TYPE_UP,				0,										0,									0,							&s_mError[ID_UVBAT_DCHAR],		NULL,			false,			ACTIVE},
//		{ID_OIBAT_DCHAR,	CODE_OIBAT_CHAR,		STATE_ALL,				1,							TYPE_UP,				0,										0,									0,							&s_mError[ID_OIBAT_DCHAR],		NULL,			false,			ACTIVE},
//		{ID_UIBAT_DCHAR,	CODE_UIBAT_CHAR,		STATE_ALL,				1,							TYPE_UP,				0,										0,									0,							&s_mError[ID_UIBAT_DCHAR],		NULL,			false,			ACTIVE},

};

void ErrorCheckInit(void){
	for(countList = 0; countList < ID_MAX; countList++){
		listError[countList].time_count 	= 0;
		listError[countList].error_state 	= false;
		listError[countList].active_err		= ACTIVE;
	}
	listError[ID_OVDC1].active_err = DEACTIVE;
	listError[ID_OVDC2].active_err = DEACTIVE;
	listError[ID_OCDC1].active_err = DEACTIVE;
	listError[ID_OCDC2].active_err = DEACTIVE;
	countList = 0;
}

void ErrorCheck_Running(void){
	UpdateCalc();
	for(countList = 1; countList < ID_MAX; countList++){
		if(listError[countList].state_valid == STATE_MAX){
			if(listError[countList].func_check != NULL){
				listError[countList].func_check();
			}
			else
				goto run_all;
		}
		else{
			if(listError[countList].state_valid == g_current_state){
				if(listError[countList].func_check != NULL){
					listError[countList].func_check();
				}
				else
					goto run_all;
			}
		}
		continue;
		run_all:
		switch(listError[countList].type_check){
			case TYPE_DOWN:
				if(fabs(*listError[countList].value_addr) <= listError[countList].value_limit){
					if(listError[countList].time_count < listError[countList].time_limit)
					 {
							 listError[countList].time_count++;
					 }
					 else
					 {
							 if(countList == ID_UVDC1 || countList == ID_UVDC2)
							 {
									 if(countList == ID_UVDC1) g_state_vin 	= false;
									 if(countList == ID_UVDC2) g_state_vout = false;
							 }
							 listError[countList].error_state = true;
					 }
				}
				else{
					 if(countList == ID_UVDC1 || countList == ID_UVDC2)
					 {
							 if(countList == ID_UVDC1) g_state_vin 	= true;
							 if(countList == ID_UVDC2) g_state_vout = true;
					 }
					 listError[countList].time_count = 0;
				}
				break;

			case TYPE_UP:
				if(fabs(*listError[countList].value_addr) >= listError[countList].value_limit)
				 {
						 if(listError[countList].time_count < listError[countList].time_limit)
						 {
								 listError[countList].time_count++;
						 }
						 else
						 {
								 listError[countList].error_state = true;
						 }
				 }
				 else
				 {
						 listError[countList].time_count = 0;
				 }
				 break;

		 default:
				 break;
		}
	}
}

static void UpdateCalc(void){
	s_mError[ID_OCDC1]	= g_iload;
	s_mError[ID_OCDC2]	= g_idumpload;
	s_mError[ID_OVDC1]	= g_vin;
//	s_mError[ID_UVDC1]	= g_vin;
	s_mError[ID_OVDC2]	= g_vout;
//	s_mError[ID_UVDC2]	= g_vout;

}

ERROR_CODE getFaultInfo(){
	int i;
	for(i = 0; i < ID_MAX; i++){
		if(listError[i].error_state == true && listError[i].active_err == ACTIVE){
			if(listError[i].code_err != CODE_OK){
				return listError[i].code_err;
			}
		}
	}
	return CODE_OK;
}

void SetErr(ERROR_ID id){
	listError[id].error_state = true;
}

