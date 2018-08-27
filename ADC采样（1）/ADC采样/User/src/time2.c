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
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             //����TIM2  A0 A1��ΪPWM���ģʽ
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


	TIM_ITConfig(TIM2 , TIM_IT_Update , ENABLE) ;//ʹ�ܸ�ʱ�ӵ��ж�
	TIM_ClearFlag(TIM2 , TIM_FLAG_Update) ; //����������־λ
	
	TIM_Cmd(TIM2 , ENABLE) ;//����ʱ��
}
 
//������cnt��ϵͳʱ��Ϊint����Ԥ��Ƶ����ʱ�����������������������arr��ֵ����������´�0��ʼ����

void TIM2_IRQHandler()                     
{
	if(TIM_GetITStatus(TIM2 , TIM_IT_Update) != RESET)        //��仰����Ҫ�������һֱ�����ж����������
	{
		TIM_ClearFlag(TIM2, TIM_FLAG_Update);
		motor_run_flag++;
		adc_flag=1;
	}
}




static u8 fac_us=0; //us��ʱ������
static u16 fac_ms=0; //ms��ʱ������
 
////////////////////////////////////////
//��ʼ���ӳٺ���
//SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8
//SYSCLK:ϵͳʱ�� ��72
////////////////////////////////////////
void delay_init(u8 SYSCLK)
{
	SysTick->CTRL&=0xfffffffb;//bit2���,ѡ���ⲿʱ�� HCLK/8
	fac_us=SYSCLK/8;
	fac_ms=(u16)fac_us*1000;
}
////////////////////////////////////////



//��ʱnms
//ע��nms�ķ�Χ
//SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK��λΪHz,nms��λΪms
//��72M������,nms<=1864
////////////////////////////////////////
//void delay_ms(u16 nms)
//{
//	u32 temp;
//	SysTick->LOAD=(u32)nms*fac_ms;//ʱ�����(SysTick->LOADΪ24bit)
//	SysTick->VAL =0x00; //��ռ�����
//	SysTick->CTRL=0x01 ; //��ʼ����
//	do
//		{
//		 
//		temp=SysTick->CTRL;
//		}
//	while(temp&0x01&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��
//	SysTick->CTRL=0x00; //�رռ�����
//	SysTick->VAL =0X00; //��ռ�����
//}



////////////////////////////////////////////
//��ʱnus
//nusΪҪ��ʱ��us��.
/////////////////////////////////////////////

void delay_us(u32 nus)
{
	u32 temp;
	SysTick->LOAD=nus*fac_us; //ʱ�����
	SysTick->VAL=0x00; //��ռ�����
	SysTick->CTRL=0x01 ; //��ʼ����
	do
		{
		temp=SysTick->CTRL;
		}
	while(temp&0x01&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��
	SysTick->CTRL=0x00; //�رռ�����
	SysTick->VAL =0X00; //��ռ�����
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