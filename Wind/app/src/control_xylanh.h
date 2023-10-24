/*
 * control_xylanh.h
 *
 *  Created on: Sep 26, 2023
 *      Author: user
 */

#ifndef SRC_CONTROL_XYLANH_H_
#define SRC_CONTROL_XYLANH_H_


void Control_Xylanh_Init(void);

//void Start_PWM_Xylanh_3(void);
//void Stop_PWM_Xylanh_3(void);
//
//void Start_PWM_Xylanh_4(void);
//void Stop_PWM_Xylanh_4(void);

//void Test_PWM_Xylanh(void);
void Xylanh_Return(void);
void Stop_Return(void);

void Xylanh_Forward(void);
void Stop_Forward(void);

void stop_all_direct(void);
#endif /* SRC_CONTROL_XYLANH_H_ */
