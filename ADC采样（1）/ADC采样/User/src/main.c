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
		if(motor_run_flag==a)           //a*0.2ms����ˢ��ʱ��
		{

		  PWM_Init(ARR,ARR*0.8);       //ARR�͵�����ٶȵĹ�ϵ��32.4/ARR��10^6��ÿ��=9��10^4/ARRתÿ��
		  VolValue_Local=VolValue;
		  VolValue=3.3 * ADC_GetConversionValue(ADC1) /4095; 	
//		  Angular_Speed = Get_Angular_Speed( );
//			ARR=32400000/Angular_Speed;
		}
		
		
//		if(adc_flag==1)          //��ȡ���ٶ�
//		{
//		 VolValue_Local=VolValue;
//		 VolValue=3.3 * ADC_GetConversionValue(ADC1) /4095; 	
//		 Angular_Speed = Get_Angular_Speed( );
//		 adc_flag=0;
//		}
	}

}
//ͨ�����ٶȻ��нǶȣ������������һ����ߵ�ʣ��ʱ�䣬��ͨ��ʣ��ʱ�����������Ľ��ٶȡ���Ϊ�ǲ���ˢ�����Կ���ʵʱ�ı�����
//�ڼ���ʱ����ȡ�ý��ٶȣ�Ȼ��ѽ��ٶȸ���������ã�


