/*
 * control_dcdc.h
 *
 *  Created on: Apr 12, 2023
 *      Author: TungVT
 */

#ifndef SRC_CONTROL_DCDC_H_
#define SRC_CONTROL_DCDC_H_

void ControlDcDcBuckInit(void);
void ControlDcDcBoostInit(void);
void ControlDcDcDumpInit(void);


//void DcDc_ControlerTestBoost(int chanel, float vbus_ref);

void DcDc_ControllerMppt(void);
void DCDC_BuckBoostMppt(void);

void DC_StopPWM_Boost(void);
void DC_StartPWM_Boost(void);

void DC_StopPWM_Buck(void);
void DC_StartPWM_Buck(void);
void TestBoost(void);
void TestBuck(void);
void Test_Buck_Boost(float i);

#endif /* SRC_CONTROL_DCDC_H_ */
