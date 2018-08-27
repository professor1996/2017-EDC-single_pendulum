#ifndef __IMU_H_
#define __IMU_H_

#include "stm32f10x.h"
//#include "Kalman_Filter.h"


#define Gyro_Gain 		2000/32767;		//���������̣�+-2000��ÿ��
#define Gyro_GainR 		0.0010653;		//���������̣�+-2000����ÿ��
#define Acc_Gain 			2/32767;			//���ٶȼ����̣�+-2g

#define DT 	0.002f					//ÿ���˲���ʱ��Ƭ
#define FILTER_A  0.9983f //һ�׻����˲����˲�ϵ��
#define FILTER_K  0.45f//���׻����˲����˲�ϵ��

//����ϵͳ��ŷ����
typedef struct
{
	float ROLL;
	float PITCH;
	float YAW;
}ANGLE;

extern ANGLE Q_ANGLE;
extern ANGLE Origin_ANGLE;
extern float GYRO_X  ;
extern float GYRO_Y  ;
extern float GYRO_Z  ;
extern float Origin_GYRO_X ;
extern float Origin_GYRO_Y ;
extern float Origin_GYRO_Z ;

float number_to_dps(s16 number);
float number_to_dps1(s16 number);
float number_to_g(s16 number);

void Get_Accel_Angle(s16 x,s16 y,s16 z,float* roll,float* pitch);//ʹ�ü��ٶȼ����ݼ���ŷ����
void IMUupdate(void);//�����ںϼ���ŷ����

void IMUupdate1(float gx, float gy, float gz, float ax, float ay, float az);//������ ���ٶȼ�  ��Ԫ����̬����
void IMUupdate2(float gx, float gy, float gz, float ax, float ay, float az);//������ ��Ԫ��������̬����

//float Kalman_Filter(float ResrcData,float ProcessNiose_Q,float MeasureNoise_R);





#endif
