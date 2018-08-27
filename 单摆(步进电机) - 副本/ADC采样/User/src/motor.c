#include "motor.h"

#define abs(x) ((x)>0? (x):(-(x)))

int motor_run_flag;
int i=0;

void Motor_GPIO_Init(void)                      //Direction 's IO      
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_SetBits(GPIOA , GPIO_Pin_1); 
}



float Hz  = 5000;
float Motor_Rate = 0;

void Motor_Run(void)
{

	  TIM2->ARR  = Hz ;
		TIM2->CCR1 = Hz / 2;
}





