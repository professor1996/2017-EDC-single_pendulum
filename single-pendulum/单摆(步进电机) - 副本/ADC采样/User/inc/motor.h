#ifndef __Motor_GPIO_H
#define __Motor_GPIO_H	






#include "stm32f10x.h"

extern int motor_run_flag;
extern float Motor_Rate  ;
extern float Hz  ;
void Motor_GPIO_Init(void);
void Motor_Run(void);


#endif








