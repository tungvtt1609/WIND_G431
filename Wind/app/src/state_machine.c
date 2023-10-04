/*
 * state_machine.c
 *
 *  Created on: Mar 17, 2023
 *      Author: Thanh Tung
 */


#include "porting/timer/timer.h"
#include "driver/control/pin_func.h"
#include "driver/measure/signal.h"
#include "state_machine.h"
#include "wind_control.h"
#include "wind_mppt.h"
#include "config_parameter.h"
#include "control_dcdc.h"
#include "driver/control/driver_pwm_dc_dc.h"
#include "control_dumpload.h"

typedef struct
{
    STATE_MACHINE_ID 		id;
    STATE_MACHINE_ID 		next;
    STATE_MACHINE_ID 		back;
    uint32_t 				time_running;
    FnCheck 				fn;
    bool 					is_always_true;
    ERROR_CODE 				code_fault;
} STATE_MACHINE;

STATE_MACHINE_ID g_current_state = STATE_WIND_OFF;
static STATE_MACHINE_ID prv_state = STATE_WIND_OFF;
ERROR_CODE faultInfo = CODE_OK;

bool g_state_vin = false;
bool g_state_vout = false;
bool g_state_bat = false;

bool IsStartState = true;
static void StateMachineRunning();
//static void Run_Mode_Dumpload();

static FUNCTION_RETURN WindOff(void);
static FUNCTION_RETURN CheckWind(void);
static FUNCTION_RETURN CloseLoop(void);
static FUNCTION_RETURN StateReset(void);
static FUNCTION_RETURN ErrorFunction(void);

STATE_MACHINE state_list[STATE_MAX] =
    {
        // id									next								    back								time_running	   fn						   need true all time      Error_code
        {STATE_WIND_OFF,                        STATE_CHECK_WIND,                       STATE_WIND_OFF,                     100,               &WindOff,                   true,                   CODE_OK},
        {STATE_CHECK_WIND,                      STATE_CLOSE_LOOP,                       STATE_RESET,                        100,               &CheckWind,                 true,                   NoWindInput},
        {STATE_CLOSE_LOOP,                      STATE_CLOSE_LOOP,                       STATE_RESET,                        100,               &CloseLoop,                 true,                   CODE_OK},
        {STATE_RESET,                           STATE_WIND_OFF,                         STATE_ERR,                          100,               &StateReset,                true,                   CODE_ERR},
        {STATE_ERR,                             STATE_ERR,                              STATE_ERR,                          100,               &ErrorFunction,             false,                  CODE_ERR}};

void StateInit(void)
{
    g_current_state = STATE_WIND_OFF;
    Timer1kHzAddCallback(&StateMachineRunning);
//		Timer1kHzAddCallBack(&Run_Mode_Dumpload);
}

static void StateMachineRunning()
{
    static uint32_t tick = 0, loop_check_err = 0;
    static ERROR_CODE protect_ret;
    static FUNCTION_RETURN ret;
    int i;
    tick++;
    loop_check_err++;
    if (loop_check_err >= ERR_LOOP_DIV)
    {
        loop_check_err = 0;
        if (g_current_state != STATE_WIND_OFF && g_current_state != STATE_RESET)
        {
            ErrorCheck_Running();
            protect_ret = getFaultInfo();
            if (protect_ret != CODE_OK && faultInfo == CODE_OK)
            {
                faultInfo = protect_ret;
                g_current_state = STATE_RESET;
                tick = 0;
                prv_state = g_current_state;
                IsStartState = true;
            }
        }
    }

    for (i = 0; i < STATE_MAX; i++)
    {
        if (g_current_state == state_list[i].id)
        {
            if (tick < state_list[i].time_running)
            {
                if (state_list[i].fn != NULL)
                {
                    ret = state_list[i].fn();
                    IsStartState = false;
                    if (state_list[i].is_always_true == true)
                    {
                        if (ret == FUNCTION_FAIL)
                        {
                            g_current_state = state_list[i].back;
                            tick = 0;
                            if (state_list[i].code_fault != CODE_OK)
                                faultInfo = state_list[i].code_fault;
                        }
                    }
                    else
                    {
                        if (ret == FUNCTION_DONE)
                        {
                            g_current_state = state_list[i].next;
                            tick = 0;
                        }
                    }
                }
            }
            else
            {
                if (ret == FUNCTION_DONE)
                {
                    g_current_state = state_list[i].next;
                    tick = 0;
                }
                else
                {
                    g_current_state = state_list[i].back;
                    tick = 0;
                    if (state_list[i].code_fault != CODE_OK)
                        faultInfo = state_list[i].code_fault;
                }
            }
            break;
        }
    }

    if (prv_state != g_current_state)
    {
        tick = 0;
        prv_state = g_current_state;
        IsStartState = true;
    }
}

static FUNCTION_RETURN WindOff(void)
{
    if (IsStartState == true)
    {
        ErrorCheckInit();
        faultInfo = CODE_OK;
    }
    return FUNCTION_DONE;
}

static FUNCTION_RETURN CheckWind(void)
{
    if (g_wind.vwt > DC_MIN_INPUT && g_wind.vwt < DC_MAX_INPUT)
    {
        WindControlInit(&g_wind);
    }

    if (g_wind.is_plugin == true)
    {
        return FUNCTION_DONE;
    }
    else
    {
        if(g_wind.vwt > DC_MAX_INPUT)
        {
            faultInfo = CODE_OVDC1;
            Pin_Func_TurnOff(LED1);
        }
        else if(g_wind.vwt <= DC_MIN_INPUT)
        {
            faultInfo = NoWindInput;
            Pin_Func_TurnOff(LED1);
            Pin_Func_TurnOff(LED2);
            Pin_Func_TurnOff(LED3);
        }
    }
		if(g_vout > BAT_MIN_INPUT && g_vout < BAT_MAX_INPUT )
		{
				g_state_bat = true;
		}
		else
		{
				g_state_bat = false;
		}
		if(g_state_bat == true)
		{
				return FUNCTION_DONE;
		}
		else
		{
				faultInfo = NoBatInput;
		}
    return FUNCTION_FAIL;
}

static FUNCTION_RETURN CloseLoop(void)
{
    static uint32_t count_wind = 0;
    if(g_wind.vwt > DC_MIN_INPUT && g_wind.vwt < DC_MAX_INPUT && g_state_bat == true)
    {
        g_wind.is_plugin = true;
        faultInfo = CODE_OK;
    }
    else{
        g_wind.is_plugin = false;
    }

    if(g_wind.is_plugin == true)
    {
        if(g_vout < MAX_VOUT && g_iload < MAX_ILOAD)
        {
            faultInfo = CODE_OK;
            DcDc_ControllerMppt();
			DC_StartPWM_Boost();
			DC_StartPWM_Buck();
        }
        else
        {
            if(g_vout > MAX_VOUT)
            {
                faultInfo = CODE_OVDC2;
                DC_StopPWM_Buck();
                DC_StopPWM_Boost();
                Pin_Func_TurnOn(LED2);
            }
            else if(g_iload > MAX_ILOAD)
            {
                faultInfo = CODE_OCDC1;
                DC_StopPWM_Buck();
                DC_StopPWM_Boost();
                Pin_Func_TurnOn(LED3);
            }
            return FUNCTION_FAIL;
        }
    }
    else
    {
        if(g_wind.vwt > DC_MAX_INPUT)
        {
			count_wind++;
			if(count_wind > 300)
			{
				faultInfo = CODE_OVDC1;
				DC_StopPWM_Buck();
				DC_StopPWM_Boost();
				Pin_Func_TurnOn(LED1);
			}
			else
			{
				Dumpload_controller();
			}
        }
        else if(g_wind.vwt <= DC_MIN_INPUT)
        {
            faultInfo = NoWindInput;
            DC_StopPWM_Buck();
            DC_StopPWM_Boost();
            Pin_Func_TurnOn(LED1);
            Pin_Func_TurnOn(LED2);
            Pin_Func_TurnOn(LED3);
        }
		else if(g_wind.iwt > MAX_ILOAD)
		{
			faultInfo = CODE_OCDC1;
		}
        return FUNCTION_FAIL;
    }
    return FUNCTION_DONE;
}

static FUNCTION_RETURN ErrorFunction(void)
{
    if (IsStartState == true)
    {
        DC_StopPWM_Buck();
        DC_StopPWM_Boost();
    }
    if (faultInfo == CODE_OVDC1)
    {
    	Pin_Func_TurnOn(LED1);
    }
    if (faultInfo == CODE_OVDC2)
    {
    	Pin_Func_TurnOn(LED2);
    }
    if (faultInfo == CODE_OCDC1)
    {
    	Pin_Func_TurnOn(LED3);
    }
    return FUNCTION_FAIL;
}

static FUNCTION_RETURN StateReset(void)
{
    if (IsStartState == true)
    {
        DC_StopPWM_Buck();
        DC_StopPWM_Boost();
    }
    return FUNCTION_DONE;
}

//static void Run_Mode_Dumpload()
//{
//		Dumpload_controller();
//}
