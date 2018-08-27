#include "stm32f10x.h"
#include "main.h"
#include "stdio.h"
#include "motor.h"
#include "adc.h"
#include "time2.h"
#include "Tim1.h"
#include "Usart3.h"
#include "VisualScope.h"
#include "MPU6050.h"
#include "IIC.h"
#include "IMU.h"
#include "Control.h"

int b = 1;

float origin_Angle = 0;

int main(void)
{	
	SystemInit();
	IIC_Init();
	InitMPU6050();
  ADC_Configuration();
	Motor_GPIO_Init();  
	TIM2_Configuration();
	TIM1_Configuration();
	Usart3_init();
	Angle_init();

	
	while(1)
	{	
    VS4Channal_Send( Q_ANGLE.PITCH , 0 , 0 , 0);
		Read_Vol();
		Actual_Angle = Angle - origin_Angle;
//		
//		if(b == 1)
//		{
//			GPIO_SetBits(GPIOA , GPIO_Pin_1);  // 顺时针
//		}
//		else
//		{
//			GPIO_ResetBits(GPIOA , GPIO_Pin_1);// 逆时针
//		}
		
//		Mode_1();
//    Mode_2();
//		Mode_3();
    Mode_4();

				

		
		
		
		
		
		
//		if(peri_count == 400)
//		{
// 		  TIM_Cmd(TIM2 , DISABLE);		
//			peri_count = 0;
//		}
//		if(peri_count)
//		{
//			TIM_Cmd(TIM2 , ENABLE) ;
//		}

		
	}

}
//通过角速度还有角度，推算出到达下一个最高点剩余时间，再通过剩余时间来推算电机的角速度。因为是不断刷新所以可以实时改变周期
//在极短时间内取得角速度，然后把角速度给电机（采用）`

//		if(peri_count == 400)
//		{
// 		  TIM_Cmd(TIM2 , DISABLE) ;//开启时钟
////				TIM_Cmd(TIM2 , ENABLE) ;//开启时钟
//			peri_count = 0;
//		}
//		if(peri_count)
//		{
//			TIM_Cmd(TIM2 , ENABLE) ;
//		}


void Angle_init(void)
{
	int i ;
	for(i=0;i<2000;i++)
	{
		Read_Vol();
	}

	origin_Angle = Angle;
}

void Read_Datainit(void)
{
		IMUupdate();
		Q_ANGLE.ROLL  =  Q_ANGLE.ROLL  - Origin_ANGLE.ROLL;
		Q_ANGLE.PITCH =  Q_ANGLE.PITCH - Origin_ANGLE.PITCH;
		GYRO_X = GYRO_X - Origin_GYRO_X;
		GYRO_Y = GYRO_Y - Origin_GYRO_Y;
		GYRO_Z = GYRO_Z - Origin_GYRO_Z;
	
}






