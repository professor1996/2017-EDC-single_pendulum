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
#include "stm32f10x.h"
#include "usart1.h"					 
#include "delay.h"
#include "led.h"
#include "timer.h"
#include "motor_control.h"
#include "motor_pwm.h"
#include "motor_pid.h"
#include "stdlib.h"
#include "indkey.h"
#include "mpu6050.h"
#include "ahrs.h"
#include "math.h"
#include "usart2.h"
/*------------------------------------------
 				ȫ�ֱ���				
------------------------------------------*/ 
extern uint8_t Q1_Start;
extern uint8_t Q2_Start;
extern uint8_t Q3_Start;
extern uint8_t Q4_Start;
extern uint8_t Q5_Start;
extern uint8_t Q6_Start;
extern uint8_t CurMode; 
/*-----------------------------------------
			    ��ʼ������
------------------------------------------*/ 
void BSP_Init(void)
{
	DelayInit();
	LED_GPIO_Config();
	Key_IO_Init();	
	PID_M1_Init();
	PID_M2_Init();
	M1TypeDef_Init();
	M2TypeDef_Init();	
	PWM_Init();
	TIM1_Config(10000-1,71);  /* TIM1 10ms Inturrupt  */
	TIM5_Config(5000-1,71);   /* TIM5 5ms Inturrupt ������200Hz ���ܸ���*/
	USART1_Config();
	USART1_printf(USART1," ϵͳ�����ɹ�!\r\n");
	USART2_Config();
	Display_Title();
	MPU6050_Init();	          //MPU6050									    
	TIM_Cmd(TIM5,ENABLE);  	  //������ƶ���TIM5���
	TIM_Cmd(TIM1,ENABLE);
}
/*-----------------------------------------
				�� �� ��
------------------------------------------*/ 
int main(void)    
{   
	BSP_Init();
	//�ǶȲ������˶����ƶ���TIM5_IRQHandler()�����
	while(1)  
	{  	
		if(Q1_Start == 1) 
		{
			CurMode = 1;
		}
		else if(Q2_Start == 1)
		{ 
			CurMode = 2;
		}
		else if(Q3_Start == 1)
		{
		 	CurMode = 3;
		}
		else if(Q4_Start == 1)
		{
		 	CurMode = 4;
		}
		else if(Q5_Start == 1)
		{
			CurMode = 5;
		}
		else if(Q6_Start == 1)
		{
			CurMode = 6;
		}
		else
		{
			CurMode = 0;
		}	
	}     
}

	
