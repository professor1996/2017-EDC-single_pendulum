#ifndef __time2_H
#define __time2_H	

#include "stm32f10x.h"


void TIM2_Configuration(void);
void TIM2_IRQHandler();
void delay_us(u32 nus);
//void delay_ms(u16 t);
void delay_ms(unsigned int t);
void delay_init(u8 SYSCLK);
extern int motor_run_flag;




#endif
