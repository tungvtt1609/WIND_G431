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
//#include "driver/measure/Temperature.h"

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


static ListError listError[ID_MAX] = {
		{ID_OK},

		//all state
		{ID_OCDC1,				CODE_OCDC1,					STATE_ALL,				MAX_ILOAD,				TYPE_UP,				TIME_ZERO,						TIME_START_0,				LEVEL_0,				&s_mError[ID_OCDC1],					NULL,			false,			ACTIVE},
		{ID_OTEMP1,				CODE_HTEMP,					STATE_ALL,				MAX_TEMP_BOOST,			TYPE_UP,				TIME_ZERO,						TIME_START_0,				LEVEL_0,				&s_mError[ID_OTEMP1],					NULL,			false,			ACTIVE},
		{ID_OVDC1,				CODE_OVDC1,					STATE_ALL,				MAX_VIN,				TYPE_UP,				TIME_ZERO,						TIME_START_0,				LEVEL_0,				&s_mError[ID_OVDC1],					NULL,			false,			ACTIVE},
		{ID_OVDC2,				CODE_OVDC2,					STATE_ALL, 				MAX_VOUT,				TYPE_UP,				TIME_ZERO,						TIME_START_0,				LEVEL_0,				&s_mError[ID_OVDC2],					NULL,			false,			ACTIVE},

};

void ErrorCheckInit(void){
	for(countList = 0; countList < ID_MAX; countList++){
		listError[countList].time_count 	= 0;
		listError[countList].error_state 	= false;
		listError[countList].active_err		= ACTIVE;
	}
//	listError[ID_OVDC1].active_err = DEACTIVE;
//	listError[ID_OVDC2].active_err = DEACTIVE;
//	listError[ID_OCDC1].active_err = DEACTIVE;
//	listError[ID_OCDC2].active_err = DEACTIVE;
	countList = 0;
}

void ErrorCheck_Running(void){
	UpdateCalc();
	for(countList = 1; countList < ID_MAX; countList++){
		if(listError[countList].state_valid == STATE_ALL){
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
						 	 listError[countList].error_state = true;
					 }
//							 if(countList == ID_UVDC1 || countList == ID_UVDC2)
//							 {
//									 if(countList == ID_UVDC1) g_state_vin 	= false;
//									 if(countList == ID_UVDC2) g_state_vout = false;
//							 }
//							 listError[countList].error_state = true;
//					 }
//				}
//				else{
//					 if(countList == ID_UVDC1 || countList == ID_UVDC2)
//					 {
//							 if(countList == ID_UVDC1) g_state_vin 	= true;
//							 if(countList == ID_UVDC2) g_state_vout = true;
//					 }


				}
				else{
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
	s_mError[ID_OTEMP1]	= g_temp1;
	s_mError[ID_OVDC1]	= g_vin;
	s_mError[ID_OVDC2]	= g_vout;

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

