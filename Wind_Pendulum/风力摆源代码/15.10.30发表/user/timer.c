/*-------------------------------------------------------------------------------------------
        		   			�����ڿ���ϵͳ(2015-8-12)

 Ӳ��ƽ̨:
 			������: STM32F103VET6 64K RAM 512K ROM
			������: LMD18200T 
		    ��Դ:   DC +12V

 ���ƽ̨:
 			��������: RealView MDK-ARM uVision4.10
			C������ : ARMCC
			ASM������:ARMASM
			������:   ARMLINK
			�ײ�����: ����������������       
 
 ʱ��: 2015��8��12��       
 
 ����: BoX
-------------------------------------------------------------------------------------------*/ 
#include "motor_control.h"
#include "motor_pid.h"
#include "motor_pwm.h"
#include "Timer.h"
#include "stm32f10x.h"
#include "stdio.h"
#include "stdlib.h"
#include "usart1.h"
#include "indkey.h"
#include "ahrs.h"
#include "mpu6050.h"
/*------------------------------------------
 				ȫ�ֱ���				
------------------------------------------*/ 
static uint16_t LED6Cnt = 0;
uint8_t CurMode = 0;

extern M1TypeDef  M1;
extern M2TypeDef  M2;

extern PIDTypdDef M1PID;
extern PIDTypdDef M2PID;

extern MPU6050_AxisTypeDef    Axis;  //MPU6050���ݽṹ��
extern AHRS_EulerAngleTypeDef EulerAngle;

extern uint8_t Item;
extern char buf[512];	 //Һ�������ݻ�����
/*-----------------------------------------------
 ��������: TIM1��ʱ��ΪPID���������ṩ�ȶ��ж�
 ��������: ARR�Ĵ���ֵ0-65535,Ԥ��Ƶֵ0-65535
 �� �� ֵ: TIM1_Config(9999,71)
	       ����Ƶ��1MHz,�ж�Ƶ��100Hz
		   ������ÿ1us��1,�ж�ÿ10ms����һ��		  			  
-----------------------------------------------*/
void TIM1_Config(uint16_t Period,uint16_t Prescaler)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef        NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);	
	
	TIM_TimeBaseStructure.TIM_Prescaler = Prescaler; 			//ʱ��Ԥ��Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���
	TIM_TimeBaseStructure.TIM_Period = Period;			        //�Զ���װֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	    //ʱ�ӷ�Ƶ1
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;			
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM1,TIM_FLAG_Update);
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);  					
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;   //��ռ���ȼ�2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  	    //��Ӧ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	            
	NVIC_Init(&NVIC_InitStructure);	
}
/*-----------------------------------------------
 ��������: TIM5��ʱ��ΪPID���������ṩ�ȶ��ж�
 ��������: ARR�Ĵ���ֵ0-65535,Ԥ��Ƶֵ0-65535
 �� �� ֵ: TIM5_Config(999,71)
	       ����Ƶ��1MHz,�ж�Ƶ��1000Hz
		   ������ÿ1us��1,�ж�ÿ1ms����һ��		  			  
-----------------------------------------------*/
void TIM5_Config(unsigned short int Period,unsigned short int Prescaler)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef        NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);	
	
	TIM_TimeBaseStructure.TIM_Prescaler = Prescaler; 			//ʱ��Ԥ��Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���
	TIM_TimeBaseStructure.TIM_Period = Period;			        //�Զ���װֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	    //ʱ�ӷ�Ƶ1
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;			
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM5,TIM_FLAG_Update);
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);  					
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //��ռ���ȼ�2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  	    //��Ӧ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	            
	NVIC_Init(&NVIC_InitStructure);		
}
/*-----------------------------------------------
 ��������:TIM1�жϷ������
 ����˵��:10ms��һ���ж�
-----------------------------------------------*/
void TIM1_UP_IRQHandler(void)
{		
	LED6Cnt++;

	if(LED6Cnt%10 == 0)	   //100msɨ��һ��
	{	
		printf("% 2.1f % 2.1f\r\n",EulerAngle.Pitch,EulerAngle.Roll);
		KeyScan();
	}
	
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
}
/*-----------------------------------------------
 ��������:TIM5�жϷ������
 ����˵��:ÿ5ms����һ���ж�,������200Hz
 ʵ������ʱ��: 3.93ms
-----------------------------------------------*/
#define H (0.88f)  //����ھ����ĸ߶�(��)
void TIM5_IRQHandler(void)
{  	
	uint8_t i = 0;
	float pitch_temp1 = 0.0;
	float roll_temp1 = 0.0;
	float pitch_temp2 = 0.0;
	float roll_temp2 = 0.0;
	static float pitch_sum = 0.0;
	static float roll_sum = 0.0;
		
	GPIOE->BSRR = GPIO_Pin_3;
	if(TIM_GetITStatus(TIM5,TIM_IT_Update) == SET)
	{			
		for(i=0;i<3;i++)
		{
			Angle_Calculate();		
			
			pitch_temp1 = (atan(Axis.AccX/Axis.AccZ)*57.2958-0.4);   //����Pitch�Ƕ� 0.4Ϊ��̬ƫ���
			roll_temp1  = (atan(Axis.AccY/Axis.AccZ)*57.2958-0.3);   //����Roll�Ƕ�  0.3Ϊ��̬ƫ���
			
			pitch_sum += pitch_temp1;
			roll_sum  += roll_temp1;
		}
		
		pitch_temp1 = pitch_sum / 3.0;	 //ȡ��ƽ��ֵ
		roll_temp1  = roll_sum  / 3.0;	 //ȡ��ƽ��ֵ

		pitch_sum = 0.0;
		roll_sum = 0.0;
		
		EulerAngle.Pitch = Kalman_Filter1(pitch_temp1,Axis.GyroY);       //�������˲���
		EulerAngle.Roll  = Kalman_Filter2(roll_temp1,-Axis.GyroX);       //�������˲���
		
		M1.CurPos = EulerAngle.Pitch; 
		M2.CurPos = EulerAngle.Roll;						   
		
		//�����ٶ�
		M1.CurSpeed = M1.CurPos - M1.PrevPos;
		M1.PrevPos = M1.CurPos;				
		
		M2.CurSpeed = M2.CurPos - M2.PrevPos;
		M2.PrevPos = M2.CurPos;	

		switch(CurMode)	//������Ŀѡ����
		{	
			case 1: Mode_1(); break;
			case 2: Mode_2(); break;
			case 3: Mode_3(); break;
			case 4: Mode_4(); break;
			case 5: Mode_5(); break;
			case 6: Mode_6(); break;
			default:break;
		}
				
		TIM_ClearITPendingBit(TIM5,TIM_IT_Update);		
	}
	GPIOE->BRR = GPIO_Pin_3;					 	
}

