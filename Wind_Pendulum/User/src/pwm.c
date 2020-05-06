#include "pwm.h"

float pwm1 = 0 ;
float pwm2 = 0 ;
float pwm3 = 0 ;
float pwm4 = 0 ;

void TIM3_Init(void)
{
	TIM_TimeBaseInitTypeDef  tim;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
  GPIO_Init(GPIOB, &GPIO_InitStructure);


	tim.TIM_Prescaler = 24 - 1;  //72Khz/9= 8Khz  
	tim.TIM_CounterMode = TIM_CounterMode_Up;
	tim.TIM_ClockDivision = TIM_CKD_DIV1;
	tim.TIM_Period = 6000 - 1;				//  2ms
	TIM_TimeBaseInit(TIM3,&tim);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             //设置TIM2  A0 A1口为PWM输出模式
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0 ;//         
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;	
	
	TIM_OC1Init(TIM3 , &TIM_OCInitStructure);
  TIM_OC1PreloadConfig(TIM3 , TIM_OCPreload_Enable);
	
	TIM_OC2Init(TIM3 , &TIM_OCInitStructure);
  TIM_OC2PreloadConfig(TIM3 , TIM_OCPreload_Enable);
	
  TIM_OC3Init(TIM3 , &TIM_OCInitStructure);
  TIM_OC3PreloadConfig(TIM3 , TIM_OCPreload_Enable);
	
	TIM_OC4Init(TIM3 , &TIM_OCInitStructure);
  TIM_OC4PreloadConfig(TIM3 , TIM_OCPreload_Enable);
  TIM_ARRPreloadConfig(TIM3 , ENABLE);   
	
	TIM_Cmd(TIM3 , ENABLE) ;
	

}

void PWM_M1_Forward(float pwm1)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	GPIO_SetBits(GPIOA,GPIO_Pin_5);
	TIM3->CCR3 =  pwm1;
}
void PWM_M1_Backward(float pwm1)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);
	GPIO_SetBits(GPIOA,GPIO_Pin_4);
	TIM3->CCR3 =  pwm1;
}


void PWM_M2_Forward(float pwm2)
{
	GPIO_ResetBits(GPIOE,GPIO_Pin_8);
	GPIO_SetBits(GPIOE,GPIO_Pin_7);
	TIM3->CCR4 =  pwm2;
}
void PWM_M2_Backward(float pwm2)
{
	GPIO_ResetBits(GPIOE,GPIO_Pin_7);
	GPIO_SetBits(GPIOE,GPIO_Pin_8);
	TIM3->CCR4 =  pwm2;
}

void PWM_M3_Forward(float pwm3)
{
	GPIO_ResetBits(GPIOC,GPIO_Pin_4);
	GPIO_SetBits(GPIOC,GPIO_Pin_5);
	TIM3->CCR1 =  pwm3;
}
void PWM_M3_Backward(float pwm3)
{
	GPIO_ResetBits(GPIOC,GPIO_Pin_5);
	GPIO_SetBits(GPIOC,GPIO_Pin_4);
	TIM3->CCR1 =  pwm3;
}

void PWM_M4_Forward(float pwm4)
{
	GPIO_ResetBits(GPIOC,GPIO_Pin_3);
	GPIO_SetBits(GPIOC,GPIO_Pin_2);
	TIM3->CCR2 =  pwm4;
}
void PWM_M4_Backward(float pwm4)
{
	GPIO_ResetBits(GPIOC,GPIO_Pin_2);
	GPIO_SetBits(GPIOC,GPIO_Pin_3);
	TIM3->CCR2 =  pwm4;
}



