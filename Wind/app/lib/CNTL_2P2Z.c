/*
 * CNTL_2P2Z.c
 *
 *  Created on: Apr 10, 2023
 *      Author: TungVT
 */
#include "CNTL_2P2Z.h"
#include "math.h"

//TODO computeDF22_PRcontrollerCoeff()
void computeDF22_PRcontrollerCoeff(CNTL_2P2Z_F_C_Coeffs *v, float kp, float ki,
		float wo, float fs, float wrc)
{
	float temp1, temp2, wo_adjusted;
    wo_adjusted = 2.0f*fs*tanf(wo/(2.0f*fs));

    temp1 = 4.0f*fs*fs+wo_adjusted*wo_adjusted+4.0f*fs*wrc;
    temp2 = 4.0f*ki*wrc*fs/temp1;
    v->Coeff_B0 = temp2;
    v->Coeff_B1 = 0;
    v->Coeff_B2=-temp2;
    v->Coeff_A1=((-8.0f*fs*fs+2*wo_adjusted*wo_adjusted)/temp1);
    v->Coeff_A2=((temp1-8.0f*fs*wrc)/temp1);
	v->IMin=-10000.0;
	v->Max=10000.0;
	v->Min=-10000.0;
    if(kp!=0)
    {
        v->Coeff_B0+=kp;
        v->Coeff_B1+=kp*v->Coeff_A1;
        v->Coeff_B2+=kp*v->Coeff_A2;
    }
    v->Coeff_A1 = -v->Coeff_A1;
    v->Coeff_A2 = -v->Coeff_A2;
}

//TODO computeDF22_InvertNotchFLTCoeff()
void computeDF22_InvertNotchFLTCoeff(CNTL_2P2Z_F_C_Coeffs *v , float ki,
		float wo, float fs, float wrc)
{
	float temp1, temp2, wo_adjusted;
    wo_adjusted = 2.0f*fs*tanf(wo/(2.0f*fs));

    temp1 = 4.0f*fs*fs+wo_adjusted*wo_adjusted+4.0f*fs*wrc;
    temp2 = 4.0f*ki*wrc*fs/temp1;
    v->Coeff_B0 = temp2;
    v->Coeff_B1 = 0;
    v->Coeff_B2=-temp2;
    v->Coeff_A1=((-8.0f*fs*fs+2*wo_adjusted*wo_adjusted)/temp1);
    v->Coeff_A2=((temp1-8.0f*fs*wrc)/temp1);
    v->Coeff_A1 = -v->Coeff_A1;
    v->Coeff_A2 = -v->Coeff_A2;
}

//TODO computeDF22_PIcontrollerCoeff()
void computeDF22_PIcontrollerCoeff(CNTL_2P2Z_F_C_Coeffs *v, float kp, float ki,
		float fsw)
{
	float temp1;
    temp1 = ki/(2.0f*fsw);

    v->Coeff_B0 = kp + temp1;
    v->Coeff_B1 = temp1 - kp;
    v->Coeff_B2 = 0;
    v->Coeff_A1 = 1.0f;
    v->Coeff_A2 = 0;
}

//TODO computeDF22_LowPassFLTCoeff()
void computeDF22_LowPassFLTCoeff(CNTL_2P2Z_F_C_Coeffs *v, float wc,
		float fsw)
{
	float temp1;
	float Ts = 1/fsw;
    temp1 = (2.0f + wc*Ts);

    v->Coeff_B0= (wc*Ts)/temp1;
    v->Coeff_B1= (wc*Ts)/temp1;
    v->Coeff_B2= 0;
    v->Coeff_A1= (wc*Ts -2)/temp1;
    v->Coeff_A2 = 0;
    v->Coeff_A1 = -v->Coeff_A1;
    v->Coeff_A2 = -v->Coeff_A2;
}

//TODO computeDF_22NotchFltrCoeff
void computeDF22_NotchFltrCoeff(CNTL_2P2Z_F_C_Coeffs *v, float Fs, float notch_freq,
		float c1, float c2)
{
	float temp1;
    float temp2;
    float wn2;
    float Ts;
    Ts = 1.0f/Fs;

    // pre warp the notch frequency
    wn2 = 2.0f*Fs*tanf(notch_freq*PI*Ts);

    temp1= 4.0f*Fs*Fs + 4.0f* wn2 * c2 * Fs + wn2*wn2;
    temp2= 1.0f/ (4.0f*Fs*Fs + 4.0f* wn2 * c1 * Fs + wn2*wn2);

    v->Coeff_B0 = temp1* temp2;
    v->Coeff_B1 = (-8.0f*Fs*Fs + 2.0f* wn2* wn2)* temp2;
    v->Coeff_B2 = (4.0f*Fs*Fs-4*wn2*c2*Fs+wn2*wn2)*temp2;
    v->Coeff_A1 = (-8.0f*Fs*Fs + 2.0f* wn2* wn2)*temp2;
    v->Coeff_A2 = (4.0f*Fs*Fs-4.0f*wn2*c1*Fs+wn2*wn2)*temp2;
    v->Coeff_A1 = -v->Coeff_A1;
    v->Coeff_A2 = -v->Coeff_A2;
}

//TODO computeDF_22NotchFltrCoeff
void computeDF22_NewNotchFltrCoeff(CNTL_2P2Z_F_C_Coeffs *v, float Fs, float notch_freq,
		float wrc)
{
	float temp1;
	float wn2, w0;
	float Ts;
    Ts = 1.0f/Fs;

    // pre warp the notch frequency
    wn2 = 2.0f*Fs*tanf(notch_freq*PI*Ts);
    w0 = wn2;

    temp1 = (4.0f + 4.0f*wrc*Ts + (w0*Ts)*(w0*Ts));
    v->Coeff_B0 = (4.0f + (w0*Ts)*(w0*Ts))/temp1;
    v->Coeff_B1 = (2.0f*(w0*Ts)*(w0*Ts) - 8.0f)/temp1;
    v->Coeff_B2 = (4.0f + (w0*Ts)*(w0*Ts))/temp1;
    v->Coeff_A1 = (2.0f*(w0*Ts)*(w0*Ts) - 8.0f)/temp1;
    v->Coeff_A2 = (4.0f - 4.0f*wrc*Ts + (w0*Ts)*(w0*Ts))/temp1;
    v->Coeff_A1 = -v->Coeff_A1;
    v->Coeff_A2 = -v->Coeff_A2;
}

