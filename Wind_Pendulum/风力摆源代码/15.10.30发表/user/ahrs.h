/*-----------------------------------------------------------------------------------
										STM32F401+MPU6050+AHRS??��������??
	
	?��?��: ����1y???a��y???��????��??��?��?��??o������?Y��?��?��y?Y??DD����o?????3??����-??.
			 
	����??: BoX
	��?��?: V1.0
	??����: STM32F401RE Nucleo Board
	��??��: 2014?��7??6��?
------------------------------------------------------------------------------------*/
#ifndef __AHRS__H__
#define __AHRS__H__

#include "stm32f10x.h"
#include "math.h"
/* include "arm_math.h" */

/*------------------------------------------
						o��?����?��y?��????
------------------------------------------*/
#define invSqrtf( iSq ) (1.0f/sqrtf((float)iSq))
#define squa( Sq )      (((float)Sq)*((float)Sq))
#define toRad( Math_D )	((float)(Math_D)*0.0174532925f)
#define toDeg( Math_R )	((float)(Math_R)*57.2957795f)
	
/* ������500Hz,�����ж϶�ʱ����֤,����̬��Χ���ʱ����ʵ���߲����� */
#define MPU6050_SAMPLERATE_FEQ  	 ((uint16_t)200) // 200Hz
#define MPU6050_SAMPLERATE_PRI  	 ((float)0.005f) // 5ms
#define MPU6050_SAMPLERATE_HALFPRI ((float)0.0025f)   //2.5ms
/*------------------------------------------
							ŷ����
------------------------------------------*/
typedef volatile struct 
{
  float Pitch;
  float Roll;
  float Yaw;
}AHRS_EulerAngleTypeDef;
/*------------------------------------------
							��Ԫ��
------------------------------------------*/
typedef volatile struct 
{
  float q0;
  float q1;
  float q2;
  float q3;
}AHRS_QuaternionTypeDef;
/*------------------------------------------
							��������
------------------------------------------*/
void Euler_ToQuaternion(AHRS_QuaternionTypeDef *pQ, AHRS_EulerAngleTypeDef *pE);
void Quaternion_ToEuler(AHRS_QuaternionTypeDef *pQ, AHRS_EulerAngleTypeDef *pE);
void Quaternion_Normalize(AHRS_QuaternionTypeDef *pQ);
void RungeKutta(AHRS_QuaternionTypeDef *pQ, float GyrX, float GyrY, float GyrZ, float helfTimes);
void AHRS_Init(AHRS_QuaternionTypeDef *pQ, AHRS_EulerAngleTypeDef *pE);
void AHRS_Update(void);
AHRS_QuaternionTypeDef Quaternion_Multiply(AHRS_QuaternionTypeDef NowQ , AHRS_QuaternionTypeDef OldQ);
float Kalman_Filter1(float Accel,float Gyro);
float Kalman_Filter2(float Accel,float Gyro);
#endif

