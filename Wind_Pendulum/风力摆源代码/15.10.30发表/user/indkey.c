/*-----------------------------------------------
		���������˿����ü�������
	˵��:	�Ѿ����˿����úͰ���ɨ������װ����
	        ���ݳ���ʵ����Ҫ������Ӧ�Ĳ������ɡ�
	ʱ��:	2013��7��22�� BoX��д 
------------------------------------------------*/
#include "indkey.h"
#include "stm32f10x.h"
#include "delay.h"
#include "stdio.h"
#include "mpu6050.h"
#include "ahrs.h"
#include "usart2.h"
/*-----------------------------------------
				ȫ�ֱ���
------------------------------------------*/
uint8_t Item = 0;
uint8_t Q1_Start = 0;
uint8_t Q2_Start = 0;
uint8_t Q3_Start = 0;
uint8_t Q4_Start = 0;
uint8_t Q5_Start = 0;
uint8_t Q6_Start = 0;
uint8_t Q7_Start = 0;

extern MPU6050_AxisTypeDef    Axis;  //MPU6050���ݽṹ��
extern AHRS_EulerAngleTypeDef EulerAngle;
extern char buf[512];	 //Һ�������ݻ�����
extern float R;
extern float angle;
extern uint8_t RoundDir;
extern uint8_t CurMode; 
/*-----------------------------------------
				KEY IO����
------------------------------------------*/
void Key_IO_Init(void)	 //����IO����
{
  GPIO_InitTypeDef IO_Init;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);	
  IO_Init.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;				    
  IO_Init.GPIO_Mode = GPIO_Mode_IPU;	
  GPIO_Init(GPIOA, &IO_Init);
}
/*-----------------------------------------------
	��������:	�����������
	��������:	�˿�����GPIOx,������GPIO_Pin
	������ֵ:	INT8U���� ����ֵ0,1
------------------------------------------------*/
void KeyScan(void)
{	
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4) == KEY_PRESSED) //K1
	{	
		switch(Item)
		{
			case 2:R+=5.0;
				   if(R >= 35.0) R = 35.0;
				   sprintf(buf,"DS16(6,60,'���ó���:%.1f ',10)\r\n",R);
				   GpuSend(buf);	
			       break;  //��2�ʰ���S4���Ӿ���

			case 3:angle+=10.0;
				   if(angle >= 180.0) 
				   	  angle = 180.0;
				   sprintf(buf,"DS16(6,80,'���ýǶ�:%.1f ',10)\r\n",angle);
				   GpuSend(buf);	
			       break;  //��3�ʰ���S4���ӽǶ�;  
			
			case 5:R+=5.0;
				   if(R >= 35.0) R = 35.0;
				   sprintf(buf,"DS16(6,100,'���ð뾶:%.1f ',10)\r\n",R);
				   GpuSend(buf);
				   break;

			case 6:R+=5.0;
				   if(R >= 35.0) R = 35.0;
				   sprintf(buf,"DS16(6,100,'���ð뾶:%.1f ',10)\r\n",R);
				   GpuSend(buf);
				   break;
				   

			default:break;
		}				
	}

	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5) == KEY_PRESSED) //K2
	{
		switch(Item)
		{
			case 2:R-=5.0;
				   if(R <= 15.0) R = 15.0;
				   sprintf(buf,"DS16(6,60,'���ó���:%.1f ',10)\r\n",R);
				   GpuSend(buf);	
			       break;  //��2�ʰ���S4���Ӿ���

			case 3:angle-=10.0;
				   if(angle <= 0.0) 
				   	  angle = 0.0;
				   sprintf(buf,"DS16(6,80,'���ýǶ�:%.1f ',10)\r\n",angle);
				   GpuSend(buf);	
			       break;  //��3�ʰ���S4���ӽǶ�;  
			
			case 5:R-=5.0;
				   if(R <= 15.0) R = 15.0;
				   sprintf(buf,"DS16(6,100,'���ð뾶:%.1f ',10)\r\n",R);
				   GpuSend(buf);
				   break;

			case 6:R-=5.0;
				   if(R <= 15.0) R = 15.0;
				   sprintf(buf,"DS16(6,100,'���ð뾶:%.1f ',10)\r\n",R);
				   GpuSend(buf);
				   break;
				   

			default:break;
		}		
	}
	
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6) == KEY_PRESSED) //K3
	{
		switch(Item)
		{
			case 1:Q1_Start = 1;
				   sprintf(buf,"DS16(6,120,'��ʼ!',10)\r\n");
				   GpuSend(buf);
				   break; 

			case 2:Q2_Start = 1;
				   sprintf(buf,"DS16(6,120,'��ʼ!',10)\r\n");
				   GpuSend(buf);
				   break;

			case 3:Q3_Start = 1;
				   sprintf(buf,"DS16(6,120,'��ʼ!',10)\r\n");
				   GpuSend(buf);
				   break;  

			case 4:Q4_Start = 1;
				   sprintf(buf,"DS16(6,120,'��ʼ!',10)\r\n");
				   GpuSend(buf);break;
				   
			case 5:Q5_Start = 1;
				   RoundDir = !RoundDir;
				   if(RoundDir == 1)
			       	   sprintf(buf,"DS16(6,120,'˳ʱ����ת!',10)\r\n");
				   else
				   	   sprintf(buf,"DS16(6,120,'��ʱ����ת!',10)\r\n");
				   GpuSend(buf);break;
				   
			case 6:Q6_Start = 1;	       
			       sprintf(buf,"DS16(6,120,'��ʼ!',10)\r\n");
				   GpuSend(buf);break;   

			default:break;
		}	
	} 	

	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7) == KEY_PRESSED)  //K4
	{
		Item++;
		if(Item>6)		//����7��
			Item = 0;
		sprintf(buf,"DS16(6,40,'��%d��',10)\r\n",Item);
		GpuSend(buf);
	}
}

