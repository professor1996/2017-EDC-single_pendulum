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
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_ADC1  , ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	nvic.NVIC_IRQChannel = TIM2_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0 ;
	nvic.NVIC_IRQChannelSubPriority = 0 ;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);

	tim.TIM_Prescaler = 2 - 1;    
	tim.TIM_CounterMode = TIM_CounterMode_Up;
	tim.TIM_ClockDivision = TIM_CKD_DIV1;
	tim.TIM_Period = 36000	- 1;				//  0.2ms
	TIM_TimeBaseInit(TIM2,&tim);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             //设置TIM2  A0 A1口为PWM输出模式
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 5000 ;//         
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;	
	
  TIM_OC1Init(TIM2 , &TIM_OCInitStructure);
  TIM_OC1PreloadConfig(TIM2 , TIM_OCPreload_Enable);
	
	TIM_OC2Init(TIM2 , &TIM_OCInitStructure);
  TIM_OC2PreloadConfig(TIM2 , TIM_OCPreload_Enable);
  TIM_ARRPreloadConfig(TIM2 , ENABLE);   


	TIM_ITConfig(TIM2 , TIM_IT_Update , ENABLE) ;//使能该时钟的中断
	TIM_ClearFlag(TIM2 , TIM_FLAG_Update) ; //清除待处理标志位
	
	TIM_Cmd(TIM2 , ENABLE) ;//开启时钟
}
 
//计数器cnt（系统时钟为int）由预分频器的时钟输出驱动，计数器计数到arr的值后溢出，重新从0开始计数

void TIM2_IRQHandler()                     
{
	if(TIM_GetITStatus(TIM2 , TIM_IT_Update) != RESET)        //这句话很重要，否则会一直陷入中断里面出不来
	{
		TIM_ClearFlag(TIM2, TIM_FLAG_Update);
		motor_run_flag++;
		adc_flag=1;
	}
}




static u8 fac_us=0; //us延时倍乘数
static u16 fac_ms=0; //ms延时倍乘数
 
////////////////////////////////////////
//初始化延迟函数
//SYSTICK的时钟固定为HCLK时钟的1/8
//SYSCLK:系统时钟 如72
////////////////////////////////////////
void delay_init(u8 SYSCLK)
{
	SysTick->CTRL&=0xfffffffb;//bit2清空,选择外部时钟 HCLK/8
	fac_us=SYSCLK/8;
	fac_ms=(u16)fac_us*1000;
}
////////////////////////////////////////



//延时nms
//注意nms的范围
//SysTick->LOAD为24位寄存器,所以,最大延时为:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK单位为Hz,nms单位为ms
//对72M条件下,nms<=1864
////////////////////////////////////////
//void delay_ms(u16 nms)
//{
//	u32 temp;
//	SysTick->LOAD=(u32)nms*fac_ms;//时间加载(SysTick->LOAD为24bit)
//	SysTick->VAL =0x00; //清空计数器
//	SysTick->CTRL=0x01 ; //开始倒数
//	do
//		{
//		 
//		temp=SysTick->CTRL;
//		}
//	while(temp&0x01&&!(temp&(1<<16)));//等待时间到达
//	SysTick->CTRL=0x00; //关闭计数器
//	SysTick->VAL =0X00; //清空计数器
//}



////////////////////////////////////////////
//延时nus
//nus为要延时的us数.
/////////////////////////////////////////////

void delay_us(u32 nus)
{
	u32 temp;
	SysTick->LOAD=nus*fac_us; //时间加载
	SysTick->VAL=0x00; //清空计数器
	SysTick->CTRL=0x01 ; //开始倒数
	do
		{
		temp=SysTick->CTRL;
		}
	while(temp&0x01&&!(temp&(1<<16)));//等待时间到达
	SysTick->CTRL=0x00; //关闭计数器
	SysTick->VAL =0X00; //清空计数器
}

void delay_ms(unsigned int t)
{
	int i;
	for(i=0;i<t;i++)
	{
		int a=10300; //at 72MHz 10300 is ok
 		while(a--);

	}
}