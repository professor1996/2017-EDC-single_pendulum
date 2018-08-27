#include "IMU.h"
#include "mpu6050.h"
#include "math.h"

ANGLE Q_ANGLE;
ANGLE Origin_ANGLE;

//把数转化为角速度(角度每秒)
float number_to_dps(s16 number)
{
	float temp;
	temp = (float)number*Gyro_Gain;
	return temp;
}

//把数转化为角速度(弧度每秒)
float number_to_dps1(s16 number)
{
	float temp;
	temp = (float)number*Gyro_GainR;
	return temp;
}

//把数转化为加速度
float number_to_g(s16 number)
{
	float temp;
	temp = (float)number*Acc_Gain;
	return temp;
}

//使用加速度计数据计算欧拉角
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

//加速度计 角度 加权平均滤波
float roll_data[4];
float pitch_data[4];
void ACC_Angle_Filter(float roll_in,float pitch_in,float *roll_out,float *pitch_out)
{
	u8 i;
	
	//入队开始
	for(i=0;i<3;i++)
	{
		roll_data[3-i] = roll_data[3-i-1];
		pitch_data[3-i] = pitch_data[3-i-1];
	}
	roll_data[0] = roll_in;
	pitch_data[0] = pitch_in;
	//入队完成
	
	//加权平均滤波
	*roll_out = (65*roll_data[0]+20*roll_data[1]+10*roll_data[2]+5*roll_data[3])/100;
	*pitch_out = (65*pitch_out[0]+20*pitch_out[1]+10*pitch_out[2]+5*pitch_out[3])/100;
	
}



//数据融合计算欧拉角
//输入：角速度(度/秒)，加速度原始数据
//输出：Q_ANGLE
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
	//角速度
	//向前运动
	if(GYRO_XOUT<32768) GYRO_XOUT=-(GYRO_XOUT/16.4);//范围为1000deg/s时，换算关系：16.4 LSB/(deg/s)
	//向后运动
	if(GYRO_XOUT>32768) GYRO_XOUT=+(65535-GYRO_XOUT)/16.4;
	
	if(GYRO_XOUT >= 100 || GYRO_XOUT <= -100)
	{
		GYRO_XOUT = 0;
	}
	GYRO_X = number_to_dps(GYRO_XOUT);
//	GYRO_X = Kalman_Filter(GYRO_X  , 1 , 0.01);
	//向前运动
	if(GYRO_YOUT<32768) GYRO_YOUT=-(GYRO_YOUT/16.4);//范围为1000deg/s时，换算关系：16.4 LSB/(deg/s)
	//向后运动
	if(GYRO_YOUT>32768) GYRO_YOUT=+(65535-GYRO_YOUT)/16.4;
	
	if(GYRO_YOUT >= 100 || GYRO_YOUT <= -100)
	{
		GYRO_YOUT = 0;
	}
	GYRO_Y = number_to_dps(GYRO_YOUT);
//	GYRO_Y = Kalman_Filter(GYRO_Y  , 1 , 0.01);
	//向前运动
	if(GYRO_ZOUT<32768) GYRO_ZOUT=-(GYRO_ZOUT/16.4);//范围为1000deg/s时，换算关系：16.4 LSB/(deg/s)
	//向后运动
	if(GYRO_ZOUT>32768) GYRO_ZOUT=+(65535-GYRO_ZOUT)/16.4;
	
	if(GYRO_ZOUT >= 100 || GYRO_ZOUT <= -100)
	{
		GYRO_ZOUT = 0;
	}
	GYRO_Z = number_to_dps(GYRO_ZOUT);
//  GYRO_Z = Kalman_Filter(GYRO_Z  , 1 , 0.01);
	
	Get_Accel_Angle(ACCEL_XOUT,ACCEL_YOUT,ACCEL_ZOUT,&roll_a,&pitch_a);//得到加速度计的欧拉角度
	
	Q_ANGLE.PITCH = pitch_a;
	Q_ANGLE.ROLL = roll_a;
	
//	Kalman_Filter_X(Q_ANGLE.PITCH,GYRO_X);  //卡尔曼滤波计算X倾角
//	Kalman_Filter_Y(Q_ANGLE.ROLL,GYRO_Y);  //卡尔曼滤波计算Y倾角
	Q_ANGLE.PITCH = Kalman_Filter(Q_ANGLE.PITCH , 1 , 0.001);
	Q_ANGLE.ROLL  = Kalman_Filter(Q_ANGLE.ROLL  , 1 , 0.001);
}


#define Kp 10.0f                        // proportional gain governs rate of convergence to accelerometer/magnetometer
#define Ki 0.008f                       // integral gain governs rate of convergence of gyroscope biases
#define halfT 0.001f                   // half the sample period采样周期的一半

float q0 = 1, q1 = 0, q2 = 0, q3 = 0;    // quaternion elements representing the estimated orientation
float exInt = 0, eyInt = 0, ezInt = 0;    // scaled integral error
void IMUupdate1(float gx, float gy, float gz, float ax, float ay, float az)
{

  float norm;
//  float hx, hy, hz, bx, bz;
  float vx, vy, vz;// wx, wy, wz;
  float ex, ey, ez;

  // 先把这些用得到的值算好
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
	
  norm = sqrt(ax*ax + ay*ay + az*az);       //acc数据归一化
  ax = ax /norm;
  ay = ay / norm;
  az = az / norm;
	
  // estimated direction of gravity and flux (v and w)              估计重力方向和流量/变迁
  vx = 2*(q1q3 - q0q2);												//四元素中xyz的表示
  vy = 2*(q0q1 + q2q3);
  vz = q0q0 - q1q1 - q2q2 + q3q3 ;

  // error is sum of cross product between reference direction of fields and direction measured by sensors
  ex = (ay*vz - az*vy) ;                           					 //向量外积在相减得到差分就是误差
  ey = (az*vx - ax*vz) ;
  ez = (ax*vy - ay*vx) ;

  exInt = exInt + ex * Ki;								  //对误差进行积分
  eyInt = eyInt + ey * Ki;
  ezInt = ezInt + ez * Ki;

  // adjusted gyroscope measurements
  gx = gx + Kp*ex + exInt;					   							//将误差PI后补偿到陀螺仪，即补偿零点漂移
  gy = gy + Kp*ey + eyInt;
  //gz = gz + Kp*ez+ ezInt;				   							//这里的gz由于没有观测者进行矫正会产生漂移，表现出来的就是积分自增或自减
	
	
	
  //四元素的微分方程
  q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
  q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
  q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
  q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;

  // 正常化四元数
  norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
  q0 = q0 / norm;
  q1 = q1 / norm;
  q2 = q2 / norm;
  q3 = q3 / norm;
	
	//四元数转化成欧拉角
  //Q_ANGLE.YAW =  0.002 * MPU6050_GYRO_LAST.Z*Gyro_Gain;  // yaw GYRO_I.Z;  atan2(2 * q1 * q2 + 2 * q0 * q3, -2 * q2*q2 - 2 * q3* q3+1)* 57.3;
	Q_ANGLE.YAW = atan2(2 * q1 * q2 + 2 * q0 * q3, -2 * q2*q2 - 2 * q3* q3+1)* 57.3;//yaw
	Q_ANGLE.PITCH = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3; // pitch
  Q_ANGLE.ROLL = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; // roll
}

void IMUupdate2(float gx, float gy, float gz, float ax, float ay, float az)
{
	float norm;
	float cr, cp, cy, sr, sp, sy, cpcy, spsy;
	
	//欧拉角转四元数
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
	
	
	
	
	//四元素的微分方程
  q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
  q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
  q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
  q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;

  // 正常化四元数
  norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
  q0 = q0 / norm;
  q1 = q1 / norm;
  q2 = q2 / norm;
  q3 = q3 / norm;
	
	//四元数转化成欧拉角
  //Q_ANGLE.YAW =  0.002 * MPU6050_GYRO_LAST.Z*Gyro_Gain;  // yaw GYRO_I.Z;  atan2(2 * q1 * q2 + 2 * q0 * q3, -2 * q2*q2 - 2 * q3* q3+1)* 57.3;
	Q_ANGLE.YAW = atan2(2 * q1 * q2 + 2 * q0 * q3, -2 * q2*q2 - 2 * q3* q3+1)* 57.3;//yaw
	Q_ANGLE.PITCH = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3; // pitch
  Q_ANGLE.ROLL = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; // roll
	
}