/********************************************************
 
  ˵����������ģ�����MPU6050��OLED,TIM2��Ϊ��ʱ��ʹ��ʱ��Ƭ��
	TIM3��4·PWM�����������������������ʹ��PC15��ΪLED��
	ʹ��PE1��PE2��PE3��PE4���ⲿ�жϵİ������롣
	
	���ߣ���ͷ��ѧ����ϵmax�Ŷ� 

********************************************************/
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "math.h"
#include "led.h"   //LED:PC15
#include "TIM4.h" //PB6��PB7Ϊ������ͨ��
#include "TIM2.h"  //��ʱ��2��Ϊʱ��Ƭ
#include "pwm.h" //��ʱ��3��PWM�����������������������PB0,PB1,PA6,PA7
//#include "OLED_I2C.h"  //SCL:PB10 SDA:PB11
#include "delay_1.h"
#include "key.h" //������PE1,PE2,PE3,PE4
#include "ADC.h"  //AD1��ͨ��1 PC0
#include "DMA.h"

#define N 10

float Kp = 160;
float Kd = 5;
float Ki = 1.2;
float Kp_b = 0;        //������Kp
float Ki_b = 0;        //������Kd 

int   Go = 0.0;
float err = 0;

float jifen_shangxian = 1000;  //�����޷�
float Ju_Li_Max = 10000;  //�����޷�

// ADC1ת���ĵ�ѹֵͨ��MDA��ʽ����SRAM
extern __IO uint16_t ADC_ConvertedValue;

// �ֲ����������ڱ���ת�������ĵ�ѹֵ 	 
float ADC_ConvertedValueLocal,Angle,Angle_err,Angle_Last,Angle_jifen;  

extern uint8_t SendBuff[SIZE];

int PWM_OUT = 0,PWM_WEIYI ,PWM_ANGLE;
u8 time = 2; //��ʱ��2���
int Time_EN = 500;
int Time_Go = 0;
int Time_D = 10;
u8 Key_Flag = 0;//��������1234

int Count_TIM4 ; //������ȡֵ
int Wei_Yi;       //����������
int Wei_Yi_Last = 0;//��һ�ε�λ����ֵ
int Speed; //������ٶ�
int Ju_Li = 0;//�ٶȵĻ���

int Wei_Yi_flag=0;
int ZhouQi = 0;

 u8 i = 0,j = 0;	 
float Lvbo[N] = {0};
int Mode1_Flag = 0,Mode4_Flag = 0;

/***************DMA��ӡ����****************/
/****
  shu1:3λ��shu2:5λ��shu3:3λ��shu4:4λ��shu5:9λ
*****/
void DMA_Send(int start,int Shu1,int Shu2 ,int Shu3,int Shu4,int Shu5)  //start���������ʼλ�� ��shu������
{   
	  if(Shu1 < 0)
		{
      Shu1 = -Shu1; 
			SendBuff[start]   = '-';
			SendBuff[start+1] = Shu1/100 + 0x30;
			SendBuff[start+2] = Shu1%100/10 + 0x30;
			SendBuff[start+3] = Shu1%10 + 0x30;
			SendBuff[start+4] = ' ';
    }
		else 
		{
      SendBuff[start]   = '+';
			SendBuff[start+1] = Shu1/100 + 0x30;
			SendBuff[start+2] = Shu1%100/10 + 0x30;
			SendBuff[start+3] = Shu1%10 + 0x30;
			SendBuff[start+4] = ' ';
    }
		if(Shu2<0)
		{
			Shu2 = -Shu2;
			SendBuff[start+5] = '-';
			SendBuff[start+6] = Shu2/10000 + 0x30;
			SendBuff[start+7] = Shu2%10000/1000 + 0x30;
			SendBuff[start+8] = Shu2%1000/100 + 0x30;
			SendBuff[start+9] = Shu2%100/10 + 0x30;
			SendBuff[start+10] = Shu2%10 + 0x30;
			SendBuff[start+11] = ' ';
		}
		else 
		{
			SendBuff[start+5] = '+';
      SendBuff[start+6] = Shu2/10000 + 0x30;
			SendBuff[start+7] = Shu2%10000/1000 + 0x30;
			SendBuff[start+8] = Shu2%1000/100 + 0x30;
			SendBuff[start+9] = Shu2%100/10 + 0x30;
			SendBuff[start+10] = Shu2%10 + 0x30;
			SendBuff[start+11] = ' ';
    }
	  
		if(Shu3 < 0)
		{
      Shu3 = -Shu3; 
			SendBuff[start+12]   = '-';
			SendBuff[start+13] = Shu3/100 + 0x30;
			SendBuff[start+14] = Shu3%100/10 + 0x30;
			SendBuff[start+15] = Shu3%10 + 0x30;
			SendBuff[start+16] = ' ';
    }
		else 
		{
      SendBuff[start+12]   = '+';
			SendBuff[start+13] = Shu3/100 + 0x30;
			SendBuff[start+14] = Shu3%100/10 + 0x30;
			SendBuff[start+15] = Shu3%10 + 0x30;
			SendBuff[start+16] = ' ';
    }
		
		if(Shu4 < 0)
		{
      Shu4 = -Shu4; 
			SendBuff[start+17]   = '-';
			SendBuff[start+18] = Shu4/1000 + 0x30;
			SendBuff[start+19] = Shu4%1000/100 + 0x30;
			SendBuff[start+20] = Shu4%100/10 + 0x30;
			SendBuff[start+21] = Shu4%10 + 0x30;
			SendBuff[start+22] = ' ';
    }
		else 
		{
		  SendBuff[start+17]   = '+';
			SendBuff[start+18] = Shu4/1000 + 0x30;
			SendBuff[start+19] = Shu4%1000/100 + 0x30;
			SendBuff[start+20] = Shu4%100/10 + 0x30;
			SendBuff[start+21] = Shu4%10 + 0x30;
			SendBuff[start+22] = ' ';
    }
		if(Shu5 < 0)
		{
      Shu5 = -Shu5; 
			SendBuff[start+23]   = '-';
			SendBuff[start+24] = Shu5/100000000 + 0x30;
			SendBuff[start+25] = Shu5%100000000/10000000 + 0x30;
			SendBuff[start+26] = Shu5%10000000/1000000 + 0x30;
			SendBuff[start+27] = Shu5%1000000/100000 + 0x30;
			SendBuff[start+28] = Shu5%100000/10000 + 0x30;
			SendBuff[start+29] = Shu5%10000/1000 + 0x30;
			SendBuff[start+30] = Shu5%1000/100 + 0x30;
			SendBuff[start+31] = Shu5%100/10 + 0x30;
			SendBuff[start+32] = Shu5%10 + 0x30;
			SendBuff[start+33] = ' ';
    }
		else 
		{
		  SendBuff[start+23]   = '+';
			SendBuff[start+24] = Shu5/100000000 + 0x30;
			SendBuff[start+25] = Shu5%100000000/10000000 + 0x30;
			SendBuff[start+26] = Shu5%10000000/1000000 + 0x30;
			SendBuff[start+27] = Shu5%1000000/100000 + 0x30;
			SendBuff[start+28] = Shu5%100000/10000 + 0x30;
			SendBuff[start+29] = Shu5%10000/1000 + 0x30;
			SendBuff[start+30] = Shu5%1000/100 + 0x30;
			SendBuff[start+31] = Shu5%100/10 + 0x30;
			SendBuff[start+32] = Shu5%10 + 0x30;
			SendBuff[start+33] = ' ';
    }
		
		SendBuff[start+34]= '\n';
}

/*************������ƣ���ת***************/
void Motor_Zheng(int pwm)
{
  TIM_SetCompare1(TIM3,pwm);
	GPIO_ResetBits(GPIOC, GPIO_Pin_4);
}

/*************������ƣ���ת**************/
void Motor_Fan(int pwm)
{
  TIM_SetCompare1(TIM3,1000-pwm);
	GPIO_SetBits(GPIOC, GPIO_Pin_4);
}



 int main(void)
 {
//   u8 i = 0,j = 0;	 
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 //���ڳ�ʼ��Ϊ9600
	 
	printf("***********************\r\n");
	USART1_DMA_Config(); 
	LED_GPIO_Config();   //LED��ʼ�� ��PC15
  TIM2_Init();  //��ʱ��2��ʼ��
	TIM3_PWM_Init();//��ʱ��3PWM�����ʼ��
	Time4_Config(); //TIM4������ģʽ
	ADC1_Init();  //AD��ʼ�� PC0 
	/*****************������ʼ��****************/ 
	KEY_Init();//IO��ʼ��	
  EXTIX_Init();//�ⲿ�жϳ�ʼ��	
  
	printf("111111111111111111111111\r\n");
	
  printf("333333333333333333333333333\r\n");

  USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);  //����1��DMAʹ��

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE); //������ʱ��TIM2
	
	TIM_SetCounter(TIM4, 5050);  //TIM4������������ʼֵ
	
	while(1)
	{
		Count_TIM4 = (TIM4 -> CNT) - 5050;   //��ʱ��4������������������룬��ʼΪ5500
		
		Wei_Yi = 5000*ZhouQi + Count_TIM4;   //�����λ��
		
		
		if((TIM4 -> CNT) >= 10050)
		{
       (TIM4 -> CNT) = 5050;
			 ZhouQi ++;             //ת��Ȧ��++
    }
		else if((TIM4 -> CNT) <= 50)
		{
       (TIM4 -> CNT) = 5050;
			  ZhouQi--;                  //ת��Ȧ��--
    } 
		
		/********���ƾ�ֵ�˲�********/
		Lvbo[i++] =(ADC_ConvertedValue - 2048)*0.0879 + err; // ��ȡת����ADֵ(int)
		if(i == N)
		{
			i = 0;
		}
		
		for(j = 0;j<N;j++)
		{
			ADC_ConvertedValueLocal = ADC_ConvertedValueLocal + Lvbo[j];
		}
		
		/*****����4���º��ƫ�ƽ�Go,����״̬��Ҫƫ��*********/
		if(Key_Flag != 4)
		{
		 Angle = ADC_ConvertedValueLocal /10;
		}
		else
		{
       Angle = ADC_ConvertedValueLocal /10+Go;
    }

		ADC_ConvertedValueLocal  = 0;
    
		/**********״̬0�رյ��*******/
		if(Key_Flag ==0)
		{
      	GPIO_ResetBits(GPIOC, GPIO_Pin_5);
    }
		else if(Key_Flag == 1)       //����1���������һ���⣬���Բ���˶�
		{   
			  GPIO_SetBits(GPIOC, GPIO_Pin_5);
				switch(Mode1_Flag)
				{
					case 0:{Motor_Zheng(800);}break;
					case 1:{Motor_Fan(900);}break;
					case 2:{Motor_Zheng(950);}break;
					case 3:{Motor_Fan(950);}break;
					case 4:{Motor_Zheng(800);}break;
					case 5:{Motor_Zheng(0);Key_Flag =0;}break;
				}
	  }
		
		
		else if(Key_Flag == 3)    //����3���ڰڶ���+-35�Ƚ���״̬6������PID����
		{
        GPIO_SetBits(GPIOC, GPIO_Pin_5);
				switch(Mode1_Flag)
				{
					case 0:{Motor_Zheng(800);}break;
					case 1:{Motor_Fan(900);}break;
					case 2:{Motor_Zheng(180);}break; //200
					case 3:{
						       Motor_Zheng(0);
						       if(Angle>-35&& Angle<35)
                   {
                    Key_Flag  = 6;
                   } 
									 else 
									 {
                    Key_Flag =0;
                   }
                  }break;
				}
    }
	  LED1_TOGGLE	;
 		DMA_Send(0,Angle,Count_TIM4,Angle_err,Speed,Ju_Li);//Angle_jifen
		LED1_TOGGLE	;

	}
 }

 /*******************�жϺ���*******************/
 void TIM2_IRQHandler(void)
{  
		if ( TIM_GetITStatus(TIM2 , TIM_IT_Update) != RESET ) 
		{	
			time--;	
		
			if(time <= 0)
			{
				time = 2;
				if(Key_Flag == 2)   //״̬2��PID����
				{
							Angle_err = Angle-Angle_Last;
						
							Angle_Last = Angle;
							
							Speed = Wei_Yi - Wei_Yi_Last;
							
							Wei_Yi_Last = Wei_Yi;
							
							Ju_Li += Speed;
							if(Ju_Li > Ju_Li_Max)
							{
                 Ju_Li = Ju_Li_Max ;
              }
							if(Ju_Li < -Ju_Li_Max)
							{
                 Ju_Li = -Ju_Li_Max ;
              }
					
							
						PWM_WEIYI = -Speed*Kp_b -Ju_Li*Ki_b ;				
 					  
						PWM_ANGLE = (Angle)*Kp + Angle_err*Kd;
						
						PWM_OUT = PWM_ANGLE + PWM_WEIYI;
 					
 					  if(PWM_OUT < 0)
						{
							if(PWM_OUT<=-1000)
							{
								PWM_OUT=-1000;
							}
							Motor_Zheng(-PWM_OUT);
						}
						else
						{
							if(PWM_OUT>=1000)
							{
								PWM_OUT=1000;
							}
							 Motor_Fan(PWM_OUT);
						}
						if(Angle >= 45 || Angle <=-45)
						{
						  GPIO_ResetBits(GPIOC, GPIO_Pin_5);
              Motor_Fan(0);
							Key_Flag = 0;
							PWM_OUT = 0;
							Angle_jifen=0;
						  Wei_Yi_flag=0;
            }
			  }
				
				if(Key_Flag == 6)   //״̬6��PID���ƣ���״̬2һ���������ʵ�΢��
				{
						Angle_err = Angle-Angle_Last;
						
							Angle_Last = Angle;
							
							Speed = Wei_Yi - Wei_Yi_Last;
							
							Wei_Yi_Last = Wei_Yi;
							
							Ju_Li += Speed;
							if(Ju_Li > Ju_Li_Max)
							{
                 Ju_Li = Ju_Li_Max ;
              }
							if(Ju_Li < -Ju_Li_Max)
							{
                 Ju_Li = -Ju_Li_Max ;
              }
					
							
						PWM_WEIYI = -Speed*Kp_b -Ju_Li*Ki_b ;				
 					  
						PWM_ANGLE = (Angle)*Kp + Angle_err*Kd;
						
						PWM_OUT = PWM_ANGLE + PWM_WEIYI;
 					
 					  if(PWM_OUT < 0)
						{
							if(PWM_OUT<=-1000)
							{
								PWM_OUT=-1000;
							}
							Motor_Zheng(-PWM_OUT);
						}
						else
						{
							if(PWM_OUT>=1000)
							{
								PWM_OUT=1000;
							}
							 Motor_Fan(PWM_OUT);
						}
						if(Angle >= 45 || Angle <=-45)
						{
						  GPIO_ResetBits(GPIOC, GPIO_Pin_5);
              Motor_Fan(0);
							Key_Flag = 0;
							PWM_OUT = 0;
							Angle_jifen=0;
						  Wei_Yi_flag=0;
            }
			  }
				
				
				if(Key_Flag == 4)   //״̬4����һ��ƫ�ƽǺ����ͬ����PID���ƣ�ϵͳ����ƫ�Ʒ���ڶ�
				{
           Angle_err = Angle-Angle_Last;
						
							Angle_Last = Angle;
							
							Speed = Wei_Yi - Wei_Yi_Last;
							
							Wei_Yi_Last = Wei_Yi;
							
							Ju_Li += Speed;
							if(Ju_Li > Ju_Li_Max)
							{
                 Ju_Li = Ju_Li_Max ;
              }
							if(Ju_Li < -Ju_Li_Max)
							{
                 Ju_Li = -Ju_Li_Max ;
              }
					
							
						PWM_WEIYI = -Speed*Kp_b -Ju_Li*Ki_b ;				
 					  
						PWM_ANGLE = (Angle)*Kp + Angle_err*Kd;
						
						PWM_OUT = PWM_ANGLE + PWM_WEIYI;
 					
 					  if(PWM_OUT < 0)
						{
							if(PWM_OUT<=-1000)
							{
								PWM_OUT=-1000;
							}
							Motor_Zheng(-PWM_OUT);
						}
						else
						{
							if(PWM_OUT>=1000)
							{
								PWM_OUT=1000;
							}
							 Motor_Fan(PWM_OUT);
						}
						if(Angle >= 45 || Angle <=-45)
						{
						  GPIO_ResetBits(GPIOC, GPIO_Pin_5);
              Motor_Fan(0);
							Key_Flag = 0;
							PWM_OUT = 0;
							Angle_jifen=0;
						  Wei_Yi_flag=0;
            }
			  }
				
			}		

			if(Key_Flag == 1)   //����1���º󣬿��ưڶ���ʱ��
			{
         Time_EN --;
				if(Time_EN <=0 && Mode1_Flag <5)
			 {
         Time_EN =500;
				 Mode1_Flag ++;
				 if(Mode1_Flag >= 5)
				 {
            Mode1_Flag = 5;
         }
       }
      }
			
		  if(Key_Flag == 3)
			{
        Time_EN --;
			 if(Time_EN <=0 && Mode1_Flag <3)
			 {
         Time_EN =500;
				 Mode1_Flag ++;
				 if(Mode1_Flag >= 3)
				 {
            Mode1_Flag = 3;
         }
       }
      }
			
			if(Key_Flag == 4)
			{
        Time_Go--;
				if(Time_Go <= 0)
				{
					 Time_Go = 0;
					 Go = 0;
					 Key_Flag = 2;
        }
      }
			TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);  
		}		 	
}



//�ⲿ�ж�0������� 
void EXTI1_IRQHandler(void)
{
	delay_ms(10);//����
	
	if(KEY3==1)	 	 //WK_UP����1
	{				 
	 	Key_Flag = 1;
		Time_EN = 500;
		 Mode1_Flag = 0;
	}
	EXTI_ClearITPendingBit(EXTI_Line1); //���LINE0�ϵ��жϱ�־λ  
}
 

//�ⲿ�ж�2�������
void EXTI2_IRQHandler(void)
{
	delay_ms(10);//����
	if(KEY2==0)	  //����KEY2  
	{
    Kp = 160;//200
    Kd = 1.5; //10
		Kp_b = 0.2;//5 0.475
		Ki_b = 2;
		Ju_Li_Max=200;
		Speed=0;
		Key_Flag = 2;
		GPIO_SetBits(GPIOC, GPIO_Pin_5);
		Ju_Li = 0;
  }		 
	EXTI_ClearITPendingBit(EXTI_Line2);  //���LINE2�ϵ��жϱ�־λ  
}

//�ⲿ�ж�3�������
void EXTI3_IRQHandler(void)
{
	delay_ms(10);//����
	if(KEY1==0)	 //����KEY3
	{				 
	  Kp = 160;//200
    Kd = 1.5; //10
		Kp_b = 0.2;//5 0.475
		Ki_b = 2;
		Ju_Li_Max=200;
		Speed=0;
		GPIO_SetBits(GPIOC, GPIO_Pin_5);
	  Key_Flag = 3;
		Time_EN = 500;
		Mode1_Flag = 0;
		Ju_Li = 0;
	}		 
	EXTI_ClearITPendingBit(EXTI_Line3);  //���LINE3�ϵ��жϱ�־λ  
}


void EXTI4_IRQHandler(void)
{
	delay_ms(10);//����
	if(KEY0==0)	 //����KEY4
	{
		Kp = 160;//200
    Kd = 1.5; //10
		Kp_b = 0.2;//5 0.475
		Ki_b = 2;
		Ju_Li_Max=200;
		Speed=0;
		Key_Flag = 4;
		GPIO_SetBits(GPIOC, GPIO_Pin_5);
		Ju_Li = 0;
		Time_Go = 3500;
		Go = 3.0;
	}		 
	EXTI_ClearITPendingBit(EXTI_Line4);  //���LINE4�ϵ��жϱ�־λ  
}
