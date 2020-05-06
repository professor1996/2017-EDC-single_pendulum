#include "Timer.h"


void TIM2_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  tim;
	NVIC_InitTypeDef         nvic;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);

	nvic.NVIC_IRQChannel = TIM2_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0 ;
	nvic.NVIC_IRQChannelSubPriority = 0 ;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);

	tim.TIM_Prescaler = 9 - 1;  //72Khz/9= 8Khz  
	tim.TIM_CounterMode = TIM_CounterMode_Up;
	tim.TIM_ClockDivision = TIM_CKD_DIV1;
	tim.TIM_Period = 400 - 1;				//  2ms
	TIM_TimeBaseInit(TIM2,&tim);
	
	TIM2_Start();
}

void TIM2_Dis()
{
	TIM_Cmd(TIM2 , DISABLE) ;
	TIM_ITConfig(TIM2 , TIM_IT_Update , DISABLE) ;
	TIM_ClearFlag(TIM2 , TIM_FLAG_Update) ; 
	
}

void TIM2_Start()
{
	TIM_Cmd(TIM2 , ENABLE) ;
	TIM_ITConfig(TIM2 , TIM_IT_Update , ENABLE) ;
	TIM_ClearFlag(TIM2 , TIM_FLAG_Update) ; 
}


void TIM2_IRQHandler()  
{
	
	if(TIM_GetITStatus(TIM2 , TIM_IT_Update) != RESET)
	{

		TIM_ClearFlag(TIM2, TIM_FLAG_Update);
		
		Angle_Calcu();
		Angle_X = Angle_x_temp - Angle_X_dirta;
		Angle_Y = Angle_y_temp - Angle_Y_dirta;

	  switch(Mode)
		{
			case 0: Mode_0(); break;
			case 1: Mode_1(); break;
			case 2: Mode_2(); break;
			case 3: Mode_3(); break;
			case 4: Mode_4(); break;
			case 5: Mode_5(); break;
			case 6: Mode_6(); break;
			default:break;
		}


	}
}







