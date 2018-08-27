#include "MPU6050.h"

float ACCEL_XOUT = 0; // ���ٶ�X��ԭʼ����
float ACCEL_YOUT = 0; // ���ٶ�Y��ԭʼ����
float ACCEL_ZOUT = 0; // ���ٶ�Z��ԭʼ����
float GYRO_XOUT = 0; // ������X��ԭʼ����
float GYRO_YOUT = 0; // ������Y��ԭʼ����
float GYRO_ZOUT = 0; // ������Z��ԭʼ����

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
	
//	ACCEL_XOUT = Kalman_Filter(ACCEL_XOUT , 0.1 , 0.001);
//	ACCEL_YOUT = Kalman_Filter(ACCEL_YOUT , 0.1 , 0.001);
//	ACCEL_ZOUT = Kalman_Filter(ACCEL_ZOUT , 0.1 , 0.001);
//	GYRO_XOUT  = Kalman_Filter(GYRO_XOUT  , 0.1 , 0.001);
//	GYRO_YOUT  = Kalman_Filter(GYRO_YOUT  , 0.1 , 0.001);
//	GYRO_ZOUT  = Kalman_Filter(GYRO_ZOUT  , 0.1 , 0.001);
//	ACCEL_XOUT = IIR_I_Filter(sensor.acc.origin.x, InPut_IIR[0], OutPut_IIR[0], b_IIR, IIR_ORDER+1, a_IIR, IIR_ORDER+1);
//	ACCEL_YOUT = IIR_I_Filter(sensor.acc.origin.y, InPut_IIR[1], OutPut_IIR[1], b_IIR, IIR_ORDER+1, a_IIR, IIR_ORDER+1);
//	ACCEL_ZOUT = IIR_I_Filter(sensor.acc.origin.z, InPut_IIR[2], OutPut_IIR[2], b_IIR, IIR_ORDER+1, a_IIR, IIR_ORDER+1);
//	
//	// ������һ�׵�ͨ�˲�
// 	GYRO_XOUT = LPF_1st(x,sensor.gyro.radian.x * Gyro_G,0.386f);	x = sensor.gyro.averag.x;
// 	GYRO_YOUT = LPF_1st(y,sensor.gyro.radian.y * Gyro_G,0.386f);	y = sensor.gyro.averag.y;
// 	GYRO_ZOUT = LPF_1st(z,sensor.gyro.radian.z * Gyro_G ,0.386f);	z = sensor.gyro.averag.z;//
}


/*====================================================================================================*/
/*==============================1`======================================================================*
**���� : KalmanFilter
**���� : �������˲�
**���� :  
**ݔ�� : None
**��ע : None
**====================================================================================================*/
/*====================================================================================================*/
float Kalman_Filter(float ResrcData,float ProcessNiose_Q,float MeasureNoise_R)
{
   float R = MeasureNoise_R;
   float Q = ProcessNiose_Q;
   static float x_mid ;//= x_last
   static float x_now;
   static float p_mid ;
   static float p_now;
   static float kg;        
   static float x_last,p_last;//ԭ��������ʽ�������
   x_mid=x_last;          //x_last=x(k-1|k-1),x_mid=x(k|k-1)
   p_mid=p_last+Q;        //p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=����
   kg=p_mid/(p_mid+R);    //kgΪkalman filter��RΪ����
   x_now=x_mid+kg*(ResrcData-x_mid);//���Ƴ�������ֵ
                
   p_now=(1-kg)*p_mid;   //����ֵ��Ӧ��covariance       
   p_last = p_now;       //����covarianceֵ
   x_last = x_now;       //����ϵͳ״ֵ̬
   return x_now;                
}




