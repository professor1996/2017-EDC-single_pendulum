#include "pwm.h"
#include "time2.h"


void PWM_Init(int arr,int ccr)
{ 
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	GPIO_InitTypeDef GPIO_InitStructure;	
	TIM_OCInitTypeDef  TIM_OCInitStructure;//
	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);
	
  TIM_TimeBaseStructure.TIM_Prescaler = 0;  //Ô¤·ÖÆµÏµÊý	
	TIM_TimeBaseStructure.TIM_Period = arr; 
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;//TIM_CKD_DIV1;	  
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;        
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
  TIM_OCInitStructure.TIM_Pulse = ccr  ;//         
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High ;  
  TIM_OC3Init(TIM3, &TIM_OCInitStructure);
  TIM_OC3PreloadConfig(TIM3 , TIM_OCPreload_Enable);
  TIM_ARRPreloadConfig(TIM3 , ENABLE);   
	TIM_CtrlPWMOutputs(TIM3, ENABLE);
	TIM_Cmd(TIM3 , ENABLE);	
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
  GPIO_Init(GPIOB, &GPIO_InitStructure);
		
  motor_run_flag=0;
}

