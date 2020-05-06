#ifndef __MPU6050_H
#define __MPU6050_H	






#include "stm32f10x.h"
#include "IIC.h"

void InitMPU6050(void);

void MPU6050_Read(void);

extern float ACCEL_XOUT ; // 加速度X轴原始数据
extern float ACCEL_YOUT ; // 加速度Y轴原始数据
extern float ACCEL_ZOUT ; // 加速度Z轴原始数据
extern float GYRO_XOUT ; // 陀螺仪X轴原始数据
extern float GYRO_YOUT; // 陀螺仪Y轴原始数据
extern float GYRO_ZOUT; // 陀螺仪Z轴原始数据



#endif

