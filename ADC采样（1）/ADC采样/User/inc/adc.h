#ifndef __ADC_H
#define __ADC_H

#include "stm32f10x.h"

extern int adc_flag;

void ADC_Configuration(void);
float Get_Adc(void);
float Get_Angular_Speed(void);

#endif