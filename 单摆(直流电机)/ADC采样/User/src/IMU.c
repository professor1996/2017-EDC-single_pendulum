#include "IMU.h"
#include "mpu6050.h"
#include "math.h"

ANGLE Q_ANGLE;
ANGLE Origin_ANGLE;

//����ת��Ϊ���ٶ�(�Ƕ�ÿ��)
float number_to_dps(s16 number)
{
	float temp;
	temp = (float)number*Gyro_Gain;
	return temp;
}

//����ת��Ϊ���ٶ�(����ÿ��)
float number_to_dps1(s16 number)
{
	float temp;
	temp = (float)number*Gyro_GainR;
	return temp;
}

//����ת��Ϊ���ٶ�
float number_to_g(s16 number)
{
	float temp;
	temp = (float)number*Acc_Gain;
	return temp;
}

//ʹ�ü��ٶȼ����ݼ���ŷ����
void Get_Accel_Angle(s16 x,s16 y,s16 z,float* roll,float* pitch)
{

	float temp;
	
	temp = sqrtf((float)(y*y+z*z));
	temp = atan2((float)x,temp)*(180/3.14159265);
	*pitch = temp;
	
	temp = sqrtf((float)(x*x+z*z));
	temp = atan2((float)y,temp)*(180/3.14159265);
	*roll = temp;

}

//���ٶȼ� �Ƕ� ��Ȩƽ���˲�
float roll_data[4];
float pitch_data[4];
void ACC_Angle_Filter(float roll_in,float pitch_in,float *roll_out,float *pitch_out)
{
	u8 i;
	
	//��ӿ�ʼ
	for(i=0;i<3;i++)
	{
		roll_data[3-i] = roll_data[3-i-1];
		pitch_data[3-i] = pitch_data[3-i-1];
	}
	roll_data[0] = roll_in;
	pitch_data[0] = pitch_in;
	//������
	
	//��Ȩƽ���˲�
	*roll_out = (65*roll_data[0]+20*roll_data[1]+10*roll_data[2]+5*roll_data[3])/100;
	*pitch_out = (65*pitch_out[0]+20*pitch_out[1]+10*pitch_out[2]+5*pitch_out[3])/100;
	
}



//�����ںϼ���ŷ����
//���룺���ٶ�(��/��)�����ٶ�ԭʼ����
//�����Q_ANGLE
float GYRO_X ;
float GYRO_Y ;
float GYRO_Z ;
float Origin_GYRO_X ;
float Origin_GYRO_Y ;
float Origin_GYRO_Z ;

void IMUupdate(void)
{
	static float x1=0,y1=0,z1=0,x2=0,y2=0,z2=0;
	static float roll_a=0,pitch_a=0;
	
	MPU6050_Read();
	//���ٶ�
	//��ǰ�˶�
	if(GYRO_XOUT<32768) GYRO_XOUT=-(GYRO_XOUT/16.4);//��ΧΪ1000deg/sʱ�������ϵ��16.4 LSB/(deg/s)
	//����˶�
	if(GYRO_XOUT>32768) GYRO_XOUT=+(65535-GYRO_XOUT)/16.4;
	
	if(GYRO_XOUT >= 100 || GYRO_XOUT <= -100)
	{
		GYRO_XOUT = 0;
	}
	GYRO_X = number_to_dps(GYRO_XOUT);
//	GYRO_X = Kalman_Filter(GYRO_X  , 1 , 0.01);
	//��ǰ�˶�
	if(GYRO_YOUT<32768) GYRO_YOUT=-(GYRO_YOUT/16.4);//��ΧΪ1000deg/sʱ�������ϵ��16.4 LSB/(deg/s)
	//����˶�
	if(GYRO_YOUT>32768) GYRO_YOUT=+(65535-GYRO_YOUT)/16.4;
	
	if(GYRO_YOUT >= 100 || GYRO_YOUT <= -100)
	{
		GYRO_YOUT = 0;
	}
	GYRO_Y = number_to_dps(GYRO_YOUT);
//	GYRO_Y = Kalman_Filter(GYRO_Y  , 1 , 0.01);
	//��ǰ�˶�
	if(GYRO_ZOUT<32768) GYRO_ZOUT=-(GYRO_ZOUT/16.4);//��ΧΪ1000deg/sʱ�������ϵ��16.4 LSB/(deg/s)
	//����˶�
	if(GYRO_ZOUT>32768) GYRO_ZOUT=+(65535-GYRO_ZOUT)/16.4;
	
	if(GYRO_ZOUT >= 100 || GYRO_ZOUT <= -100)
	{
		GYRO_ZOUT = 0;
	}
	GYRO_Z = number_to_dps(GYRO_ZOUT);
//  GYRO_Z = Kalman_Filter(GYRO_Z  , 1 , 0.01);
	
	Get_Accel_Angle(ACCEL_XOUT,ACCEL_YOUT,ACCEL_ZOUT,&roll_a,&pitch_a);//�õ����ٶȼƵ�ŷ���Ƕ�
	
	Q_ANGLE.PITCH = pitch_a;
	Q_ANGLE.ROLL = roll_a;
	
//	Kalman_Filter_X(Q_ANGLE.PITCH,GYRO_X);  //�������˲�����X���
//	Kalman_Filter_Y(Q_ANGLE.ROLL,GYRO_Y);  //�������˲�����Y���
	Q_ANGLE.PITCH = Kalman_Filter(Q_ANGLE.PITCH , 1 , 0.001);
	Q_ANGLE.ROLL  = Kalman_Filter(Q_ANGLE.ROLL  , 1 , 0.001);
}


#define Kp 10.0f                        // proportional gain governs rate of convergence to accelerometer/magnetometer
#define Ki 0.008f                       // integral gain governs rate of convergence of gyroscope biases
#define halfT 0.001f                   // half the sample period�������ڵ�һ��

float q0 = 1, q1 = 0, q2 = 0, q3 = 0;    // quaternion elements representing the estimated orientation
float exInt = 0, eyInt = 0, ezInt = 0;    // scaled integral error
void IMUupdate1(float gx, float gy, float gz, float ax, float ay, float az)
{

  float norm;
//  float hx, hy, hz, bx, bz;
  float vx, vy, vz;// wx, wy, wz;
  float ex, ey, ez;

  // �Ȱ���Щ�õõ���ֵ���
  float q0q0 = q0*q0;
  float q0q1 = q0*q1;
  float q0q2 = q0*q2;
  //float q0q3 = q0*q3;
  float q1q1 = q1*q1;
  //float q1q2 = q1*q2;
  float q1q3 = q1*q3;
  float q2q2 = q2*q2;
  float q2q3 = q2*q3;
  float q3q3 = q3*q3;
	
	if(ax*ay*az==0)
 		return;
	
  norm = sqrt(ax*ax + ay*ay + az*az);       //acc���ݹ�һ��
  ax = ax /norm;
  ay = ay / norm;
  az = az / norm;
	
  // estimated direction of gravity and flux (v and w)              �����������������/��Ǩ
  vx = 2*(q1q3 - q0q2);												//��Ԫ����xyz�ı�ʾ
  vy = 2*(q0q1 + q2q3);
  vz = q0q0 - q1q1 - q2q2 + q3q3 ;

  // error is sum of cross product between reference direction of fields and direction measured by sensors
  ex = (ay*vz - az*vy) ;                           					 //�������������õ���־������
  ey = (az*vx - ax*vz) ;
  ez = (ax*vy - ay*vx) ;

  exInt = exInt + ex * Ki;								  //�������л���
  eyInt = eyInt + ey * Ki;
  ezInt = ezInt + ez * Ki;

  // adjusted gyroscope measurements
  gx = gx + Kp*ex + exInt;					   							//�����PI�󲹳��������ǣ����������Ư��
  gy = gy + Kp*ey + eyInt;
  //gz = gz + Kp*ez+ ezInt;				   							//�����gz����û�й۲��߽��н��������Ư�ƣ����ֳ����ľ��ǻ����������Լ�
	
	
	
  //��Ԫ�ص�΢�ַ���
  q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
  q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
  q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
  q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;

  // ��������Ԫ��
  norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
  q0 = q0 / norm;
  q1 = q1 / norm;
  q2 = q2 / norm;
  q3 = q3 / norm;
	
	//��Ԫ��ת����ŷ����
  //Q_ANGLE.YAW =  0.002 * MPU6050_GYRO_LAST.Z*Gyro_Gain;  // yaw GYRO_I.Z;  atan2(2 * q1 * q2 + 2 * q0 * q3, -2 * q2*q2 - 2 * q3* q3+1)* 57.3;
	Q_ANGLE.YAW = atan2(2 * q1 * q2 + 2 * q0 * q3, -2 * q2*q2 - 2 * q3* q3+1)* 57.3;//yaw
	Q_ANGLE.PITCH = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3; // pitch
  Q_ANGLE.ROLL = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; // roll
}

void IMUupdate2(float gx, float gy, float gz, float ax, float ay, float az)
{
	float norm;
	float cr, cp, cy, sr, sp, sy, cpcy, spsy;
	
	//ŷ����ת��Ԫ��
	cr = cos(Q_ANGLE.ROLL/114.6f);
	cp = cos(Q_ANGLE.PITCH/114.6f);
	cy = cos(Q_ANGLE.YAW/114.6f);
	
	sr = sin(Q_ANGLE.ROLL/114.6f);
	sp = sin(Q_ANGLE.PITCH/114.6f);
	sy = sin(Q_ANGLE.YAW/114.6f);
	
	cpcy = cp * cy;
	spsy = sp * sy;
	
	q0 = cr * cpcy + sr * spsy;
	q1 = sr * cpcy - cr * spsy;
	q2 = cr * sp * cy + sr * cp * sy;
	q3 = cr * cp * sy - sr * sp * cy;
	
	
	
	
	//��Ԫ�ص�΢�ַ���
  q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
  q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
  q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
  q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;

  // ��������Ԫ��
  norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
  q0 = q0 / norm;
  q1 = q1 / norm;
  q2 = q2 / norm;
  q3 = q3 / norm;
	
	//��Ԫ��ת����ŷ����
  //Q_ANGLE.YAW =  0.002 * MPU6050_GYRO_LAST.Z*Gyro_Gain;  // yaw GYRO_I.Z;  atan2(2 * q1 * q2 + 2 * q0 * q3, -2 * q2*q2 - 2 * q3* q3+1)* 57.3;
	Q_ANGLE.YAW = atan2(2 * q1 * q2 + 2 * q0 * q3, -2 * q2*q2 - 2 * q3* q3+1)* 57.3;//yaw
	Q_ANGLE.PITCH = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3; // pitch
  Q_ANGLE.ROLL = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; // roll
	
}