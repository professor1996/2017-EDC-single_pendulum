#include "Control.h"

u8 Mode = 0;

float PWM1 , PWM2;
float R = 30.0 ;
float POWER_MAX = 2700 ;


void Mode_0(void)         //��ֹ
{
	MotorPower(1 , 1);
}

void Mode_1(void)          //��Y�����ɰڶ������Ȳ�����50cm
{
	const float priod = 2000.0;  //��������(����)
	static uint32_t MoveTimeCnt = 0;
  int A1 = 200;
	float Normalization = 0.0;
	float Omega = 0.0;
	float set_x = 0.0;
	
	MoveTimeCnt += 5;							 //�22ms����1��
	Normalization = (float)MoveTimeCnt / priod;	 //�Ե������ڹ�һ��
	Omega =   Normalization ;            //2.0*3.14159*Normalization;			 //��2�н��й�һ������
	//A = atan((R/88.0f))*57.2958f;				 //���ݰڷ�����Ƕ�A,88Ϊ�ڸ˾�����泤��cm
	set_x = A1*sin(Omega);                        //�������ǰ�ڽ� 	
		
	SetPoint_x = set_x;			//X����PID��λĿ��ֵ0
	Proportion_x = 5;	
	Integral_x = 2;	 
	Derivative_x = 5;
	
	SetPoint_y = 0 ;		//Y����PID����Ŀ��ֵsin
	Proportion_y = 5;	   //60
	Integral_y = 2;	 
	Derivative_y = 5; 
	
	PWM1 = PID_M1_PosLocCalc(Angle_X);	//Pitch
	PWM2 = PID_M2_PosLocCalc(Angle_Y); //Roll

	
	if(PWM1 > POWER_MAX)  PWM1 =  POWER_MAX;
	if(PWM1 < -POWER_MAX) PWM1 = -POWER_MAX;	
	
	if(PWM2 > POWER_MAX)  PWM2 = POWER_MAX;
	if(PWM2 < -POWER_MAX) PWM2 = -POWER_MAX;		
	
	MotorPower(PWM1 , PWM2);
}

 int A2 = 0;

void Mode_2(void)          //��Y�����ɰڶ������ȿɵ� 30cm-50cm
{
	const float priod = 2000.0;  //��������(����)
	static uint32_t MoveTimeCnt = 0;

	float Normalization = 0.0;
	float Omega = 0.0;
	float set_y = 0.0;
	
	MoveTimeCnt += 5;							 //�22ms����1��
	Normalization = (float)MoveTimeCnt / priod;	 //�Ե������ڹ�һ��
	Omega =   Normalization ;            //2.0*3.14159*Normalization;			 //��2�н��й�һ������
	//A = atan((R/88.0f))*57.2958f;				 //���ݰڷ�����Ƕ�A,88Ϊ�ڸ˾�����泤��cm
	set_y = A2*sin(Omega);                        //�������ǰ�ڽ� 	
	
	SetPoint_x = 0;			//X����PID��λĿ��ֵ0
	Proportion_x = 4;	
	Integral_x = 1.7;	 
	Derivative_x = 5;
	
	SetPoint_y = set_y ;		//Y����PID����Ŀ��ֵsin
	Proportion_y = 4;	   //60
	Integral_y = 1.7;	 
	Derivative_y = 5; 
	
	PWM1 = PID_M1_PosLocCalc(Angle_X);	//Pitch
	PWM2 = PID_M2_PosLocCalc(Angle_Y); //Roll

	
	if(PWM1 > POWER_MAX)  PWM1 =  POWER_MAX;
	if(PWM1 < -POWER_MAX) PWM1 = -POWER_MAX;	
	
	if(PWM2 > POWER_MAX)  PWM2 = POWER_MAX;
	if(PWM2 < -POWER_MAX) PWM2 = -POWER_MAX;		
	
	MotorPower(PWM1 , PWM2);
}

int A3 = 100;
int B3 = 100;

void Mode_3(void)               //���趨�ڶ��Ƕ�0-360�� �� ������20cm
{
	const float priod = 2000.0;  //��������(����)
	static uint32_t MoveTimeCnt = 0;

	float Normalization = 0.0;
	float Omega = 0.0;
	float set_x = 0.0;
	float set_y = 0.0;
	
	MoveTimeCnt += 2;							 //�22ms����1��
	Normalization = (float)MoveTimeCnt / priod;	 //�Ե������ڹ�һ��
	Omega =   Normalization ;            //2.0*3.14159*Normalization;			 //��2�н��й�һ������
	//A = atan((R/88.0f))*57.2958f;				 //���ݰڷ�����Ƕ�A,88Ϊ�ڸ˾�����泤��cm
	set_x = A3*sin(Omega);                        //�������ǰ�ڽ� 	
	set_y = B3*sin(Omega);                        //�������ǰ�ڽ�
	
	SetPoint_x = set_x;			//X����PID��λĿ��ֵ0
	Proportion_x = 4;	
	Integral_x = 1.7;	 
	Derivative_x = 5;
	
	SetPoint_y = set_y ;		//Y����PID����Ŀ��ֵsin
	Proportion_y = 4;	   //60
	Integral_y = 1.7;	 
	Derivative_y = 5; 
	
	PWM1 = PID_M1_PosLocCalc(Angle_X);	//Pitch
	PWM2 = PID_M2_PosLocCalc(Angle_Y); //Roll

	
	if(PWM1 > POWER_MAX)  PWM1 =  POWER_MAX;
	if(PWM1 < -POWER_MAX) PWM1 = -POWER_MAX;	
	
	if(PWM2 > POWER_MAX)  PWM2 = POWER_MAX;
	if(PWM2 < -POWER_MAX) PWM2 = -POWER_MAX;		
	
	MotorPower(PWM1 , PWM2);
}

void Mode_4(void)         // ����һ���Ƕȷ��£�5s�ڻָ�ԭλ�ã�0��0��
{
	if(Angle_X<45.0 && Angle_Y<45.0)	//С��45�ȲŽ����ƶ�
	{		
		SetPoint_x = 0;			//X����PID��λĿ��ֵ0
		Proportion_x = 20;	
		Integral_x = 0;	 
		Derivative_x = 70;

		SetPoint_y = 0 ;		//Y����PID����Ŀ��ֵsin
		Proportion_y = 20;	   //60
		Integral_y = 0;	 
		Derivative_y = 70; 
			
	  PWM1 = PID_M1_PosLocCalc(Angle_X);	//Pitch
	  PWM2 = PID_M2_PosLocCalc(Angle_Y); //Roll
		
	if(PWM1 > POWER_MAX)  PWM1 =  POWER_MAX;
	if(PWM1 < -POWER_MAX) PWM1 = -POWER_MAX;	
	
	if(PWM2 > POWER_MAX)  PWM2 = POWER_MAX;
	if(PWM2 < -POWER_MAX) PWM2 = -POWER_MAX;		
	
	}
	else	
	{
	 	PWM1 = 1;
		PWM2 = 1;	
	}
	
	MotorPower(PWM1 , PWM2);
}

int A5 = 110;

void Mode_5(void)         //��Բ�� �뾶��15cm-45cm�ڿɵ�
{
		const float priod = 2000.0;  //��������(����)
	static uint32_t MoveTimeCnt = 0;

	float Normalization = 0.0;
	float Omega = 0.0;
	float set_x = 0.0;
	float set_y = 0.0;
	
	MoveTimeCnt += 2;							 //�22ms����1��
	Normalization = (float)MoveTimeCnt / priod;	 //�Ե������ڹ�һ��
	Omega =   Normalization ;            //2.0*3.14159*Normalization;			 //��2�н��й�һ������
	//A = atan((R/88.0f))*57.2958f;				 //���ݰڷ�����Ƕ�A,88Ϊ�ڸ˾�����泤��cm
	set_x = A5*sin(Omega);                        //�������ǰ�ڽ� 	
	set_y = A5*sin(Omega + 3.14159 / 2);                        //�������ǰ�ڽ�
	
	SetPoint_x = set_x;			//X����PID��λĿ��ֵ0
	Proportion_x = 4;	
	Integral_x = 1.7;	 
	Derivative_x = 5;
	
	SetPoint_y = set_y ;		//Y����PID����Ŀ��ֵsin
	Proportion_y = 4;	   //60
	Integral_y = 1.7;	 
	Derivative_y = 5; 
	
	PWM1 = PID_M1_PosLocCalc(Angle_X);	//Pitch
	PWM2 = PID_M2_PosLocCalc(Angle_Y); //Roll

	
	if(PWM1 > POWER_MAX)  PWM1 =  POWER_MAX;
	if(PWM1 < -POWER_MAX) PWM1 = -POWER_MAX;	
	
	if(PWM2 > POWER_MAX)  PWM2 = POWER_MAX;
	if(PWM2 < -POWER_MAX) PWM2 = -POWER_MAX;		
	
	MotorPower(PWM1 , PWM2);
}

void Mode_6(void)
{
	
}






void MotorPower(float pwm1 , float pwm2)
{
	
	if(pwm1 < 0)
	{
	 	PWM_M2_Forward(-pwm1);
		PWM_M3_Forward(0);
	}
	else if(pwm1 > 0)
	{
	 	PWM_M2_Forward(0);
		PWM_M3_Forward(pwm1);	
	}

	if(pwm2 < 0)
	{
	 	PWM_M4_Forward(-pwm2);
		PWM_M1_Forward(0);
	}
	else if(pwm2 > 0)
	{
	 	PWM_M4_Forward(0);
		PWM_M1_Forward(pwm2);	
	} 	
}









