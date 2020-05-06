#ifndef __MPU6050_H
#define __MPU6050_H	






#include "stm32f10x.h"
#include "IIC.h"

void InitMPU6050(void);

void MPU6050_Read(void);

extern float ACCEL_XOUT ; // ���ٶ�X��ԭʼ����
extern float ACCEL_YOUT ; // ���ٶ�Y��ԭʼ����
extern float ACCEL_ZOUT ; // ���ٶ�Z��ԭʼ����
extern float GYRO_XOUT ; // ������X��ԭʼ����
extern float GYRO_YOUT; // ������Y��ԭʼ����
extern float GYRO_ZOUT; // ������Z��ԭʼ����



#endif

