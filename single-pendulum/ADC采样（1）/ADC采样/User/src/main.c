#include "stm32f10x.h"
#include "stdio.h"
#include "motor.h"
#include "adc.h"
#include "time2.h"
#include "pwm.h"

float VolValue = 0.00;
float VolValue_Local=0.0;
int a=20;
int j=0;
void PWM_Init(int arr,int ccr);
float ARR,CCR_Val;
float Angular_Speed;
float	ARR=20000;

int main(void)
{
	delay_ms(100);
	SystemInit( );
	delay_ms(100);
  ADC_Configuration( );
	Motor_GPIO_Init( );  
	TIM2_Configuration( );
	PWM_Init(ARR,ARR*0.8); 
	while(1)
	{
		if(motor_run_flag==a)           //a*0.2ms等于刷新时间
		{

		  PWM_Init(ARR,ARR*0.8);       //ARR和电机角速度的关系：32.4/ARR×10^6度每秒=9×10^4/ARR转每秒
		  VolValue_Local=VolValue;
		  VolValue=3.3 * ADC_GetConversionValue(ADC1) /4095; 	
//		  Angular_Speed = Get_Angular_Speed( );
//			ARR=32400000/Angular_Speed;
		}
		
		
//		if(adc_flag==1)          //获取角速度
//		{
//		 VolValue_Local=VolValue;
//		 VolValue=3.3 * ADC_GetConversionValue(ADC1) /4095; 	
//		 Angular_Speed = Get_Angular_Speed( );
//		 adc_flag=0;
//		}
	}

}
//通过角速度还有角度，推算出到达下一个最高点剩余时间，再通过剩余时间来推算电机的角速度。因为是不断刷新所以可以实时改变周期
//在极短时间内取得角速度，然后把角速度给电机（采用）


