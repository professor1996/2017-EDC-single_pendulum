#include "Control.h"

#define pi 3.141592653
int Encoder_Ratio  = 448 ;   //��դ����Ϊ448��  , ������ٱ�Ϊ  1��30 
float Real_Speed = 0.0;
float Want_Speed = 0;          //500
int  Want_Pwm = 0;
float Real_Location = 0.0;
float Want_Location = 0.0;


void Mode_1(void)     //����һ����������Ϊ2.203s �� 
{
  Want_Speed = 114 ;
	
}


//void Mode_2(void)
//{
//	TIM2->CCR1 = 0;
//}


//void Mode_3(void)
//{
//	TIM2->CCR1 = 0;
//}


//float R = 1.205  ;
//float L = 1.5    ;
//float thelta     ;
//float alfa       ;
//float motor_turn_angle;
//float motor_pulse_count ;
//int peri_count = 1; 
//int positive_flag = 0;
//int negetive_flag = 0;
//float Actual_Angle;

//void Mode_4(void)
//{
//	//���Ƶ���ٶȣ��ȽϿ죩
//	Hz = 1500;
//	Motor_Run();	

//	//ת���Ƕ�
//  thelta = Actual_Angle * pi / 180.0;
//	
//	//����Ҫ���Ǻ���ģ�ͣ�
//	alfa = ( atan( (L - R * sin(thelta)) / (R * (1 - cos(thelta))) ) - thelta ) * 180 / pi ;  
//	motor_turn_angle = 90 - alfa;  //��õ����ʱ��ת���Ƕ�

//	//��ת���Ƕ�������������
//	//�� angle / 0.9�� = ���ת����������ֵ
//	motor_pulse_count = motor_turn_angle / 0.9;
//	motor_pulse_count = (int)motor_pulse_count;
//	
//	if(motor_pulse_count > peri_count )
//	{
//		TIM_Cmd(TIM2 , ENABLE);	
//		GPIO_ResetBits(GPIOA , GPIO_Pin_1);// �����ʱ��ת
//		positive_flag = 1;
//		negetive_flag = 0;
//	}
//	
//	if(motor_pulse_count <  peri_count )
//	{
//		TIM_Cmd(TIM2 , ENABLE);	
//		GPIO_SetBits(GPIOA , GPIO_Pin_1);  // ���˳ʱ��ת
//		negetive_flag = 1;
//		positive_flag = 0;
//	}
//}







