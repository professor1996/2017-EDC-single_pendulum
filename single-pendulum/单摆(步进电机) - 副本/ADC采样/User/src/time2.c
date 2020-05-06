#include "time2.h"
#include "motor.h"
#include "adc.h"


void TIM2_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  tim;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef         nvic;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	nvic.NVIC_IRQChannel = TIM2_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0 ;
	nvic.NVIC_IRQChannelSubPriority = 0 ;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);

	tim.TIM_Prescaler = 72 - 1;    //72000000 / 2 = 36000000
	tim.TIM_CounterMode = TIM_CounterMode_Up;
	tim.TIM_ClockDivision = TIM_CKD_DIV1;
	tim.TIM_Period = 1000	- 1;				//  0.2ms
	TIM_TimeBaseInit(TIM2,&tim);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             //设置TIM2  A0 A1口为PWM输出模式
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0 ;//         
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;	
  TIM_OC1Init(TIM2 , &TIM_OCInitStructure);
  TIM_OC1PreloadConfig(TIM2 , TIM_OCPreload_Enable);
	
//	TIM_OC2Init(TIM2 , &TIM_OCInitStructure);
//  TIM_OC2PreloadConfig(TIM2 , TIM_OCPreload_Enable);
//  TIM_ARRPreloadConfig(TIM2 , ENABLE);   

	TIM_ITConfig(TIM2 , TIM_IT_Update , ENABLE) ;//使能该时钟的中断
	TIM_ClearFlag(TIM2 , TIM_FLAG_Update) ; //清除待处理标志位
	
//	TIM_Cmd(TIM2 , ENABLE) ;//开启时钟
	TIM_Cmd(TIM2 , DISABLE) ;//关闭时钟
//	TIM_ARRPreloadConfig(TIM2, ENABLE);
	
}
 


void TIM2_IRQHandler()                     
{
	if(TIM_GetITStatus(TIM2 , TIM_IT_Update) != RESET)        //这句话很重要，否则会一直陷入中断里面出不来
	{
		TIM_ClearFlag(TIM2, TIM_FLAG_Update);

		if(positive_flag)
		{
		  peri_count++;
		}

		if(negetive_flag)
		{
		  peri_count--;
		}
		
		if((motor_pulse_count - peri_count) <= 5 || (peri_count - motor_pulse_count) <= 5 )
		{
 		  TIM_Cmd(TIM2 , DISABLE);		
//      positive_flag	= 0;
//			negetive_flag = 0;
		}
	
		 if(peri_count == 399)
		{
			peri_count = 0;
		}

                                    		//频率为        1000000 / HZ   
	}                                    //周期为        Hz / 1000000
	                                     //电机转动周期  Hz / 1000000 * 400
}




