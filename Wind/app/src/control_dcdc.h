/*
 * control_dcdc.h
 *
 *  Created on: Apr 12, 2023
 *      Author: TungVT
 */

#ifndef SRC_CONTROL_DCDC_H_
#define SRC_CONTROL_DCDC_H_

void ControlDcDcBuckBoostInit(void);
//void ControlDcDcBoostInit(void);

void DcDc_MPPT_Controller(void);

//void DcDc_ControllerMppt(void);
void DCDC_BuckBoostMppt(void);

void DC_StopPWM_Boost(void);
void DC_StartPWM_Boost(void);

void DC_StopPWM_Buck(void);
void DC_StartPWM_Buck(void);

//void TestBoost(void);
//void TestBuck(void);
//void Test_Buck_Boost(float i);

void Curr_Controller_Init(void);
void Vol_Controller_Init(void);
void MPPT_Controller_Init(void);

#endif /* SRC_CONTROL_DCDC_H_ */
