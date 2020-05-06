#ifndef __Motor_GPIO_H
#define __Motor_GPIO_H	






#include "stm32f10x.h"

void Motor_GPIO_Init(void);                      //Direction 's IO      

void Motor_PWM_Run(int give_pulse,int max_pulse);
void Motor_Free (void);
void Motor_Break (void);


#endif








