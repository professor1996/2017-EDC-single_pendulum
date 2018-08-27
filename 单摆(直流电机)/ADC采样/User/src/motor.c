#include "motor.h"

#define abs(x) ((x)>0? (x):(-(x)))

int motor_run_flag;
int i=0;

void Motor_GPIO_Init(void)                      //Direction 's IO      
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_SetBits(GPIOA , GPIO_Pin_3); 
	GPIO_SetBits(GPIOA , GPIO_Pin_4); 
}




void Motor_PWM_Run(int give_pulse,int max_pulse)
{
    max_pulse = abs(max_pulse);
    
    if(give_pulse < 0)
		{
			
			GPIO_ResetBits(GPIOA,GPIO_Pin_3);   // AO1
			GPIO_SetBits(GPIOA,GPIO_Pin_4);        // AO2
			
			if(give_pulse < (-max_pulse))
			{
				give_pulse = (-max_pulse);
			}
			
    }
		
    else 
    {
					
			GPIO_SetBits(GPIOA,GPIO_Pin_3); // AO1
			GPIO_ResetBits(GPIOA,GPIO_Pin_4);  // AO2
			
			if(give_pulse > max_pulse)
			{
				give_pulse = max_pulse;
			}
			
    }												  
		
    TIM2->CCR1 = abs(give_pulse);

}

void Motor_Free (void)
{
	TIM2->CCR1 = 0;
	GPIO_ResetBits(GPIOA,GPIO_Pin_3);
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);		
}

void Motor_Break (void)
{
	TIM2->CCR1 = 4000;
	GPIO_ResetBits(GPIOA,GPIO_Pin_3);
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);
}




