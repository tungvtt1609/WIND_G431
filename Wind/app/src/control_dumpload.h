/*
 * control_dumpload.h
 *
 *  Created on: Sep 12, 2023
 *      Author: TungVT
 */

#ifndef SRC_CONTROL_DUMPLOAD_H_
#define SRC_CONTROL_DUMPLOAD_H_


void ControlDcDcDumpInit(void);

void StartPWM_Dumpload(void);
void Stop_PWM_DumpLoad(void);

void Dumpload_controller(void);
//void TestDump(void);

#endif /* SRC_CONTROL_DUMPLOAD_H_ */
