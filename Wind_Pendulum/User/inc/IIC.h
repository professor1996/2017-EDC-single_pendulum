#ifndef __IIC_H
#define __IIC_H	




#include "stm32f10x.h"



/*
 **********************************************************
 *
 * ??MPU6050????
 *
 **********************************************************
 */

//����mpu6050�ڲ���ַ
#define	SMPLRT_DIV		0x19	                     //�����ǲ�����,����ֵ:0x07(125Hz)
#define	CONFIG			  0x1A	                     //��ͨ�˲�Ƶ��,����ֵ:0x06(5Hz)
#define	GYRO_CONFIG		0x1B	                     //�������Լ켰������Χ,����ֵ:0x18(���Լ�,2000deg/s)
#define	ACCEL_CONFIG	0x1C	                     //���ټ��Լ�,������Χ����ͨ�˲�Ƶ��,����ֵ:0x01(���Լ�,2G,5Hz)
#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D                       //0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40
#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42
#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48
#define	PWR_MGMT_1		0x6B	                      //��Դ����,����ֵ:0x00(��������)
#define	WHO_AM_I			0x75	                      //IIC��ַ�Ĵ���(Ĭ����ֵ0x68,ֻ��)
#define	SlaveAddress	0xD0	                      //IICд��ʱ�ĵ�ַ�ֽ�����,+1Ϊ��ȡ

/*
 **********************************************************
 *
 * ��غ궨��
 *
 **********************************************************
 */
//#define ERROR			0
//#define OK				1

#define	SDA_D	  GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5)	
#define	SDA_H   GPIO_SetBits(GPIOA, GPIO_Pin_5)	
#define	SDA_L   GPIO_ResetBits(GPIOA, GPIO_Pin_5)	
						
#define	SCL_H   GPIO_SetBits(GPIOA, GPIO_Pin_4)	
#define	SCL_L   GPIO_ResetBits(GPIOA, GPIO_Pin_4)	 


void IIC_Init(void);
void SDA_IOOUT(void);								//SDA???????
void SDA_IOIN(void);								//SDA???????
void I2C_Start(void);									//????
void I2C_Stop(void);									//????

unsigned char I2C_SlaveAck(void);						//????
void I2C_WriteByte(unsigned char a);					//????
unsigned char I2C_ReadByte(void);						//????

void Single_WriteI2C(unsigned char Regs_Addr,unsigned char Regs_Data); //????
unsigned char Single_ReadI2C(unsigned char Regs_Addr);	//????

int GetData(u8 REG_Address);

extern u8 mpu6050_buffer[14];

#endif

