/*
 * Frequency.c
 *
 *  Created on: Oct 11, 2023
 *      Author: duchm20
 */

#include "Frequency.h"
#define 	Pole  4


float Freq = 0;
float Freq_fil =0;
uint16_t RPM = 0;

void Cal_Freq(void){
	if (Time_Cap > 500 ){
		Freq = 50000.0/(float)Time_Cap;
	}
	if (Freq <= 0.25){
		Freq = 0;
	}
	Freq_fil +=0.001*(Freq-Freq_fil);
	RPM = 60*Freq_fil/Pole;
}
