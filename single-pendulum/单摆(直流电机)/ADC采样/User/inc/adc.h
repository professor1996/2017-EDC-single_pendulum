#ifndef __ADC_H
#define __ADC_H

#include "stm32f10x.h"

extern int adc_flag;
extern float VolValue ;
extern float Angle;
	
void ADC_Configuration(void);
float Get_Adc(void);
void Read_Vol(void);
void Filter(void);


#endif




