#include "PID.h"

float SetPoint_x ;
float SetPoint_y ;

float Proportion_x;
float Integral_x;
float Derivative_x;
float Proportion_y;
float Integral_y;
float Derivative_y;


float PID_M1_PosLocCalc(float NextPoint)
{
  float  iError_x,dError_x;
  float SumError_x ;
  float LastError_x ;
	
	iError_x = SetPoint_x - NextPoint;        // ƫ��
	SumError_x += iError_x;				    // ����
	
	if(SumError_x > 2700.0)					//�����޷�2300
		SumError_x = 2700.0;
	else if(SumError_x < -2700.0)
		SumError_x = -2700.0;	
	
	dError_x = iError_x - LastError_x; 			// ��ǰ΢��
	LastError_x = iError_x;
	
	return(float)(  Proportion_x * iError_x           	// ������
          		    + Integral_x   * SumError_x 		// ������
          		    + Derivative_x * dError_x);
}

 
float PID_M2_PosLocCalc(float NextPoint)
{
  float  iError_y,dError_y;
  float SumError_y ;
  float LastError_y ;
	
	iError_y = SetPoint_y - NextPoint;        // ƫ��
	SumError_y += iError_y;
	
	if(SumError_y > 2700.0)					//�����޷�
		SumError_y = 2700.0;
	else if(SumError_y < -2700.0)
		SumError_y = -2700.0;
	
	dError_y = iError_y - LastError_y; 			// ��ǰ΢��
	LastError_y = iError_y;
	
	return(float)(  Proportion_y * iError_y           	// ������
          		    + Integral_y   * SumError_y 		// ������
          		    + Derivative_y * dError_y);
}


//float PID_M1_PosLocCalc(float NextPoint)
//{
//	float pwm;
//  float  iError_x;
//	
//	iError_x = SetPoint_x - NextPoint;        // ƫ��
//	pwm += Proportion_x * iError_x ;
//	
//	return(float) pwm ;
//}

// 
//float PID_M2_PosLocCalc(float NextPoint)
//{
//  float  iError_y,dError_y;
//  float SumError_y ;
//  float LastError_y ;
//	
//	iError_y = SetPoint_y - NextPoint;        // ƫ��
//	SumError_y += iError_y;
//	
//	if(SumError_y > 2300.0)					//�����޷�
//		SumError_y = 2300.0;
//	else if(SumError_y < -2300.0)
//		SumError_y = -2300.0;
//	
//	dError_y = iError_y - LastError_y; 			// ��ǰ΢��
//	LastError_y = iError_y;
//	
//	return(float)(  Proportion_y * iError_y           	// ������
//          		    + Integral_y   * SumError_y 		// ������
//          		    + Derivative_y * dError_y);
//}
















