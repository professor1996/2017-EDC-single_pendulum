#include "motor.h"

#define abs(x) ((x)>0? (x):(-(x)))

int motor_run_flag;
int i=0;

void Motor_GPIO_Init(void)                          //双电机驱动H桥IO口配置
{
	GPIO_InitTypeDef GPIO_InitStructure;	//定义结构体
		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);	//开启外设时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//选择要控制的引脚
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;//选择要控制的引脚
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//选择要控制的引脚0
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//选择要控制的引脚
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB , &GPIO_InitStructure);
}
void motor_run(void)
{
		i++;
		i=i%8;
		motor_run_flag=0;
		switch(i)
		{
			case 0:	GPIO_SetBits(GPIOB,GPIO_Pin_6); 
							GPIO_ResetBits(GPIOB,GPIO_Pin_7); 
							GPIO_ResetBits(GPIOB,GPIO_Pin_8); 
							GPIO_ResetBits(GPIOB,GPIO_Pin_9);
							break;
																						
			case 1:	GPIO_SetBits(GPIOB,GPIO_Pin_7); 
							GPIO_SetBits(GPIOB,GPIO_Pin_6); 
							GPIO_ResetBits(GPIOB,GPIO_Pin_8); 
							GPIO_ResetBits(GPIOB,GPIO_Pin_9);  
							break;
			
			case 2:	GPIO_SetBits(GPIOB,GPIO_Pin_7); 
							GPIO_ResetBits(GPIOB,GPIO_Pin_8); 
							GPIO_ResetBits(GPIOB,GPIO_Pin_6); 
							GPIO_ResetBits(GPIOB,GPIO_Pin_9); 
							break;
			
			case 3:	GPIO_SetBits(GPIOB,GPIO_Pin_8); 
							GPIO_SetBits(GPIOB,GPIO_Pin_7); 
							GPIO_ResetBits(GPIOB,GPIO_Pin_9); 
							GPIO_ResetBits(GPIOB,GPIO_Pin_6);   
							break;
		
			case 4:	GPIO_SetBits(GPIOB,GPIO_Pin_8); 
							GPIO_ResetBits(GPIOB,GPIO_Pin_7); 
							GPIO_ResetBits(GPIOB,GPIO_Pin_6); 
							GPIO_ResetBits(GPIOB,GPIO_Pin_9); 
							break;
																						
			case 5:	GPIO_SetBits(GPIOB,GPIO_Pin_8); 
							GPIO_SetBits(GPIOB,GPIO_Pin_9); 
							GPIO_ResetBits(GPIOB,GPIO_Pin_6); 
							GPIO_ResetBits(GPIOB,GPIO_Pin_7); 
							break;
			
			case 6:	GPIO_SetBits(GPIOB,GPIO_Pin_9); 
							GPIO_ResetBits(GPIOB,GPIO_Pin_7); 
							GPIO_ResetBits(GPIOB,GPIO_Pin_6); 
							GPIO_ResetBits(GPIOB,GPIO_Pin_8);   
							break;
			
			case 7:	GPIO_SetBits(GPIOB,GPIO_Pin_9); 
							GPIO_SetBits(GPIOB,GPIO_Pin_6); 
							GPIO_ResetBits(GPIOB,GPIO_Pin_8); 
							GPIO_ResetBits(GPIOB,GPIO_Pin_7);  
							break;
		}

}