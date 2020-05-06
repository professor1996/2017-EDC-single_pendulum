/********************************************************
 
  ËµÃ÷£º±¾¹¤³ÌÄ£°å°üº¬MPU6050£¬OLED,TIM2×÷Îª¶¨Ê±Æ÷Ê¹ÓÃÊ±¼äÆ¬£¬
	TIM3×÷4Â·PWMÊä³ö²¢¼ÆËãÊä³öµÄÂö³å¸öÊı£¬Ê¹ÓÃPC15×÷ÎªLED£¬
	Ê¹ÓÃPE1£¬PE2£¬PE3£¬PE4×öÍâ²¿ÖĞ¶ÏµÄ°´¼üÊäÈë¡£
	
	×÷Õß£ºÉÇÍ·´óÑ§µç×ÓÏµmaxÍÅ¶Ó 

********************************************************/
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "math.h"
#include "led.h"   //LED:PC15
#include "TIM4.h" //PB6ºÍPB7Îª±àÂëÆ÷Í¨µÀ
#include "TIM2.h"  //¶¨Ê±Æ÷2×÷ÎªÊ±¼äÆ¬
#include "pwm.h" //¶¨Ê±Æ÷3×öPWMÊä³ö£¬²¢¼ÆËãÊä³öÂö³å¸öÊı£»PB0,PB1,PA6,PA7
//#include "OLED_I2C.h"  //SCL:PB10 SDA:PB11
#include "delay_1.h"
#include "key.h" //°´¼ü£ºPE1,PE2,PE3,PE4
#include "ADC.h"  //AD1£ºÍ¨µÀ1 PC0
#include "DMA.h"

#define N 10

float Kp = 160;
float Kd = 5;
float Ki = 1.2;
float Kp_b = 0;        //±àÂëÆ÷Kp
float Ki_b = 0;        //±àÂëÆ÷Kd 

int   Go = 0.0;
float err = 0;

float jifen_shangxian = 1000;  //»ı·ÖÏŞ·ù
float Ju_Li_Max = 10000;  //»ı·ÖÏŞ·ù

// ADC1×ª»»µÄµçÑ¹ÖµÍ¨¹ıMDA·½Ê½´«µ½SRAM
extern __IO uint16_t ADC_ConvertedValue;

// ¾Ö²¿±äÁ¿£¬ÓÃÓÚ±£´æ×ª»»¼ÆËãºóµÄµçÑ¹Öµ 	 
float ADC_ConvertedValueLocal,Angle,Angle_err,Angle_Last,Angle_jifen;  

extern uint8_t SendBuff[SIZE];

int PWM_OUT = 0,PWM_WEIYI ,PWM_ANGLE;
u8 time = 2; //¶¨Ê±Æ÷2¼ÆÊ
int Time_EN = 500;
int Time_Go = 0;
int Time_D = 10;
u8 Key_Flag = 0;//°´¼ü·µ»Ø1234

int Count_TIM4 ; //Âö³åÊıÈ¡Öµ
int Wei_Yi;       //¸üĞÂÂö³åÊı
int Wei_Yi_Last = 0;//ÉÏÒ»´ÎµÄÎ»ÒÆÊıÖµ
int Speed; //µç»úµÄËÙ¶È
int Ju_Li = 0;//ËÙ¶ÈµÄ»ı·Ö

int Wei_Yi_flag=0;
int ZhouQi = 0;

 u8 i = 0,j = 0;	 
float Lvbo[N] = {0};
int Mode1_Flag = 0,Mode4_Flag = 0;

/***************DMA´òÓ¡º¯Êı****************/
/****
  shu1:3Î»£¬shu2:5Î»£¬shu3:3Î»£¬shu4:4Î»£¬shu5:9Î»
*****/
void DMA_Send(int start,int Shu1,int Shu2 ,int Shu3,int Shu4,int Shu5)  //start£ºÊı×éµÄÆğÊ¼Î»ÖÃ ¬±shu£º±äÁ¿
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

/*************µç»ú¿ØÖÆ£ºÕı×ª***************/
void Motor_Zheng(int pwm)
{
  TIM_SetCompare1(TIM3,pwm);
	GPIO_ResetBits(GPIOC, GPIO_Pin_4);
}

/*************µç»ú¿ØÖÆ£º·´×ª**************/
void Motor_Fan(int pwm)
{
  TIM_SetCompare1(TIM3,1000-pwm);
	GPIO_SetBits(GPIOC, GPIO_Pin_4);
}



 int main(void)
 {
//   u8 i = 0,j = 0;	 
	delay_init();	    	 //ÑÓÊ±º¯Êı³õÊ¼»¯	  
	NVIC_Configuration(); 	 //ÉèÖÃNVICÖĞ¶Ï·Ö×é2:2Î»ÇÀÕ¼ÓÅÏÈ¼¶£¬2Î»ÏìÓ¦ÓÅÏÈ¼¶
	uart_init(115200);	 //´®¿Ú³õÊ¼»¯Îª9600
	 
	printf("***********************\r\n");
	USART1_DMA_Config(); 
	LED_GPIO_Config();   //LED³õÊ¼»¯ £ºPC15
  TIM2_Init();  //¶¨Ê±Æ÷2³õÊ¼»¯
	TIM3_PWM_Init();//¶¨Ê±Æ÷3PWMÊä³ö³õÊ¼»¯
	Time4_Config(); //TIM4±àÂëÆ÷Ä£Ê½
	ADC1_Init();  //AD³õÊ¼»¯ PC0 
	/*****************°´¼ü³õÊ¼»¯****************/ 
	KEY_Init();//IO³õÊ¼»¯	
  EXTIX_Init();//Íâ²¿ÖĞ¶Ï³õÊ¼»¯	
  
	printf("111111111111111111111111\r\n");
	
  printf("333333333333333333333333333\r\n");

  USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);  //´®¿Ú1µÄDMAÊ¹ÄÜ

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE); //¿ªÆô¶¨Ê±Æ÷TIM2
	
	TIM_SetCounter(TIM4, 5050);  //TIM4±àÂëÆ÷¼ÆÊı³õÊ¼Öµ
	
	while(1)
	{
		Count_TIM4 = (TIM4 -> CNT) - 5050;   //¶¨Ê±Æ÷4Âö³å¼ÆÊı£¬±àÂëÆ÷ÊäÈë£¬³õÊ¼Îª5500
		
		Wei_Yi = 5000*ZhouQi + Count_TIM4;   //¾ÍËã³öÎ»ÒÆ
		
		
		if((TIM4 -> CNT) >= 10050)
		{
       (TIM4 -> CNT) = 5050;
			 ZhouQi ++;             //×ªµÄÈ¦Êı++
    }
		else if((TIM4 -> CNT) <= 50)
		{
       (TIM4 -> CNT) = 5050;
			  ZhouQi--;                  //×ªµÄÈ¦Êı--
    } 
		
		/********µİÍÆ¾ùÖµÂË²¨********/
		Lvbo[i++] =(ADC_ConvertedValue - 2048)*0.0879 + err; // ¶ÁÈ¡×ª»»µÄADÖµ(int)
		if(i == N)
		{
			i = 0;
		}
		
		for(j = 0;j<N;j++)
		{
			ADC_ConvertedValueLocal = ADC_ConvertedValueLocal + Lvbo[j];
		}
		
		/*****°´¼ü4°´ÏÂºó¸øÆ«ÒÆ½ÇGo,ÆäËû×´Ì¬²»ÒªÆ«ÒÆ*********/
		if(Key_Flag != 4)
		{
		 Angle = ADC_ConvertedValueLocal /10;
		}
		else
		{
       Angle = ADC_ConvertedValueLocal /10+Go;
    }

		ADC_ConvertedValueLocal  = 0;
    
		/**********×´Ì¬0¹Ø±Õµç»ú*******/
		if(Key_Flag ==0)
		{
      	GPIO_ResetBits(GPIOC, GPIO_Pin_5);
    }
		else if(Key_Flag == 1)       //°´¼ü1£¬»ù±¾ÌâµÚÒ»¶şÌâ£¬Íê³ÉÔ²ÖÜÔË¶¯
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
		
		
		else if(Key_Flag == 3)    //°´¼ü3£¬°Ú°Ú¶¯ÖÁ+-35¶È½øÈë×´Ì¬6£¬½øĞĞPIDµ÷½Ú
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

 /*******************ÖĞ¶Ïº¯Êı*******************/
 void TIM2_IRQHandler(void)
{  
		if ( TIM_GetITStatus(TIM2 , TIM_IT_Update) != RESET ) 
		{	
			time--;	
		
			if(time <= 0)
			{
				time = 2;
				if(Key_Flag == 2)   //×´Ì¬2µÄPID¿ØÖÆ
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
				
				if(Key_Flag == 6)   //×´Ì¬6µÄPID¿ØÖÆ£¬ºÍ×´Ì¬2Ò»Ñù£¬¿ÉÒÔÊÊµ±Î¢µ÷
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
				
				
				if(Key_Flag == 4)   //×´Ì¬4£º¸øÒ»¸öÆ«ÒÆ½Çºó½øĞĞÍ¬ÑùµÄPID¿ØÖÆ£¬ÏµÍ³»áÍùÆ«ÒÆ·½Ïò°Ú¶¯
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

			if(Key_Flag == 1)   //°´¼ü1°´ÏÂºó£¬¿ØÖÆ°Ú¶¯µÄÊ±¼ä
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



//Íâ²¿ÖĞ¶Ï0·şÎñ³ÌĞò 
void EXTI1_IRQHandler(void)
{
	delay_ms(10);//Ïû¶¶
	
	if(KEY3==1)	 	 //WK_UP°´¼ü1
	{				 
	 	Key_Flag = 1;
		Time_EN = 500;
		 Mode1_Flag = 0;
	}
	EXTI_ClearITPendingBit(EXTI_Line1); //Çå³ıLINE0ÉÏµÄÖĞ¶Ï±êÖ¾Î»  
}
 

//Íâ²¿ÖĞ¶Ï2·şÎñ³ÌĞò
void EXTI2_IRQHandler(void)
{
	delay_ms(10);//Ïû¶¶
	if(KEY2==0)	  //°´¼üKEY2  
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
	EXTI_ClearITPendingBit(EXTI_Line2);  //Çå³ıLINE2ÉÏµÄÖĞ¶Ï±êÖ¾Î»  
}

//Íâ²¿ÖĞ¶Ï3·şÎñ³ÌĞò
void EXTI3_IRQHandler(void)
{
	delay_ms(10);//Ïû¶¶
	if(KEY1==0)	 //°´¼üKEY3
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
	EXTI_ClearITPendingBit(EXTI_Line3);  //Çå³ıLINE3ÉÏµÄÖĞ¶Ï±êÖ¾Î»  
}


void EXTI4_IRQHandler(void)
{
	delay_ms(10);//Ïû¶¶
	if(KEY0==0)	 //°´¼üKEY4
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
	EXTI_ClearITPendingBit(EXTI_Line4);  //Çå³ıLINE4ÉÏµÄÖĞ¶Ï±êÖ¾Î»  
}
