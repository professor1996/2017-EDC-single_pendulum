#include "MPU6050.h"

float ACCEL_XOUT = 0; // 加速度X轴原始数据
float ACCEL_YOUT = 0; // 加速度Y轴原始数据
float ACCEL_ZOUT = 0; // 加速度Z轴原始数据
float GYRO_XOUT = 0; // 陀螺仪X轴原始数据
float GYRO_YOUT = 0; // 陀螺仪Y轴原始数据
float GYRO_ZOUT = 0; // 陀螺仪Z轴原始数据

void InitMPU6050(void)
{
	Single_WriteI2C(PWR_MGMT_1, 0x00);
	Single_WriteI2C(SMPLRT_DIV, 0x08);   // 04
	Single_WriteI2C(CONFIG, 0x04);         // 02
	Single_WriteI2C(GYRO_CONFIG, 0x18);			//18			
	Single_WriteI2C(ACCEL_CONFIG, 0x10);    //10
}

void MPU6050_Read(void)
{
		ACCEL_XOUT = GetData(ACCEL_XOUT_H);
		ACCEL_YOUT = GetData(ACCEL_YOUT_H);	
		ACCEL_ZOUT = GetData(ACCEL_ZOUT_H);	
		GYRO_XOUT = GetData(GYRO_XOUT_H);	
		GYRO_YOUT = GetData(GYRO_YOUT_H);	
		GYRO_ZOUT = GetData(GYRO_ZOUT_H);
}

