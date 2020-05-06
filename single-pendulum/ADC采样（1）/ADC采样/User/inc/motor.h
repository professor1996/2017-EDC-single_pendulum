#ifndef __Motor_GPIO_H
#define __Motor_GPIO_H	






#include "stm32f10x.h"

extern int motor_run_flag;

void Motor_GPIO_Init(void);
void motor_run(void);



#endif