#ifndef __time2_H
#define __time2_H	

#include "stm32f10x.h"
#include "Control.h"



void TIM2_Configuration(void);
//void TIM2_IRQHandler();
void delay_us(u32 nus);
void delay_ms(u16 nms);
void delay_init(u8 SYSCLK);
 




#endif
