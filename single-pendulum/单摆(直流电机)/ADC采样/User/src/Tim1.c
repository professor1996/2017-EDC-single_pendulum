#include "Tim1.h"


void TIM1_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  tim;
	NVIC_InitTypeDef         nvic;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 , ENABLE);

	nvic.NVIC_IRQChannel = TIM1_UP_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0 ;
	nvic.NVIC_IRQChannelSubPriority = 1 ;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);

	tim.TIM_Prescaler = 2 - 1;  //72Mhz/9= 8Mhz  
	tim.TIM_CounterMode = TIM_CounterMode_Up;
	tim.TIM_ClockDivision = TIM_CKD_DIV1;
	tim.TIM_Period = 400 - 1;				//  1ms
	TIM_TimeBaseInit(TIM1 , &tim);
	
  TIM1_Start();
}

void TIM1_Dis(void)
{
	TIM_Cmd(TIM1 , DISABLE) ;
	TIM_ITConfig(TIM1 , TIM_IT_Update , DISABLE) ;
	TIM_ClearFlag(TIM1 , TIM_FLAG_Update) ; 
	
}

void TIM1_Start(void)
{
	TIM_ITConfig(TIM1 , TIM_IT_Update , ENABLE) ;
	TIM_ClearFlag(TIM1 , TIM_FLAG_Update) ; 
	TIM_Cmd(TIM1 , ENABLE) ;
}




void TIM1_UP_IRQHandler()  
{
	
	if(TIM_GetITStatus(TIM1 , TIM_IT_Update) != RESET)
	{

		int Pwm_Cnt = 0;
		TIM_ClearFlag(TIM1, TIM_FLAG_Update);

		Pwm_Cnt = Encoder_Get_CNT();
   	Real_Speed = Pwm_Cnt*750/Encoder_Ratio ;	
		Real_Location += Encoder_Get_CNT() ;
	
		PID_Calc(&PidDataSpeed , Real_Speed , Want_Speed);         //控制左电机转速
		Want_Pwm  = PidDataSpeed.U;
		Motor_PWM_Run(Want_Pwm  , 4000);
	
	}
}


//float Kalman_Filter(float ResrcData,float ProcessNiose_Q,float MeasureNoise_R)
//{
//   float R = MeasureNoise_R;
//   float Q = ProcessNiose_Q;
//   static float x_mid ;//= x_last
//   static float x_now;
//   static float p_mid ;
//   static float p_now;
//   static float kg;        
//   static float x_last,p_last;//原来是在形式参数里的
//   x_mid=x_last;          //x_last=x(k-1|k-1),x_mid=x(k|k-1)
//   p_mid=p_last+Q;        //p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=噪声
//   kg=p_mid/(p_mid+R);    //kg为kalman filter，R为噪声
//   x_now=x_mid+kg*(ResrcData-x_mid);//估计出的最优值
//                
//   p_now=(1-kg)*p_mid;   //最优值对应的covariance       
//   p_last = p_now;       //更新covariance值
//   x_last = x_now;       //更新系统状态值
//   return x_now;                
//}



