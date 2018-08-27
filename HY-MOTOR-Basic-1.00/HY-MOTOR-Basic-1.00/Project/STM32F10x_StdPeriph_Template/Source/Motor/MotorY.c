/**
  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
  ******************************************************************************
  * @file    Project\STM32F10x_StdPeriph_Template\Source\Motor\motor.c 
  * @author  CCW Application Team
  * @version V3.5.0
  * @date    2013-10-26
  * @brief   Main program body
  ******************************************************************************
  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
**/

#include "main.h"

//-----------main and motor------------------------------------------------------------------
u16 MotorYSpeedPos 						   	           = 0;
s32 CounterMotorYStepPostionCurrent           	 = StartPostion; //�������1��ǰλ�ü�����
u16 MotorYStep_StartStop                     = 256;	//����ٶ� MotorSpeed

//-----------------------------------------------------------------------------------
//static uint32 Count_Motor1_Postion_Current         = 0;
/*****************************************************************************************/
static u16 step_startstop 				   	      = 0;	//��ͣ����1 256΢�� ��ͣ����2 512΢��
//static u16 SmallStepCurrent 				 	    = SMALL_STEP;       //��ǰ�������ϸ�ֲ����Ĵ��� 127  64  32  16  8  4  2  1�� //�������ϸ�ֲ��� ��ʼ����
static u8  LineFlag						              = YES;
//----------------------------Motor-1---------------------------------------------------------------
static s16 MotorPositionPointer             = 0;  
static u8  Motor1_Stop_Flag                 = 0;
static u8  Motor1_dir                       = 1;  
static u8  Motor1WillBackFlag               = 0;
static u8  Motor1_dir_Current               = 1;
static u8  Motor1_speed_postion_dir         = 1;
static s32 Motor1_postion_Current           = StartPostion;
static s32 Motor1_postion_user              = StartPostion;
static s32 Motor1_dynamic_postion_gap       = 0;           
static s32 Motor1_user_postion_gap          = 0;         
//-----------------------------------------------------------------------------------------------


/*****************************************************************************
*		s32	CheckValChange 	   two times
*****************************************************************************/
static u8 CheckValChange1(s32 Val)
{
    static s32 tempa1    = 0;
    static s32 tempb1    = 0;
    static u8  i         = 0;
	static u8  m		 = 0;

//-------------------------------------------------------------------
    i++;
    switch (i)
    {
    case 1:
        tempa1 = Val;
        break;
    case 2:
        i = 0;      //CLR 0
        tempb1 = Val;
        break;
    }
   //-------------------------------
    if (tempa1 != tempb1)
		m = YES;
	else
		m = NO;

		return m;
//-------------------------------------------------------------------
}

/*******************************************************************/
static u8 CheckValChange2(s32 Val)
{
    static s32 tempa1    = 0;
    static s32 tempb1    = 0;
    static u8  i         = 0;
	static u8  m		 = 0;

//-------------------------------------------------------------------
    i++;
    switch (i)
    {
    case 1:
        tempa1 = Val;
        break;
    case 2:
        i = 0;      //CLR 0
        tempb1 = Val;
        break;
    }
   //-------------------------------
    if (tempa1 != tempb1)
		m = YES;
	else
		m = NO;

		return m;
//-------------------------------------------------------------------
}

/*******************************************************************/
static u8 CheckValChange3(s32 Val)
{
    static s32 tempa1    = 0;
    static s32 tempb1    = 0;
    static u8  i         = 0;
	static u8  m		 = 0;

//-------------------------------------------------------------------
    i++;
    switch (i)
    {
    case 1:
        tempa1 = Val;
        break;
    case 2:
        i = 0;      //CLR 0
        tempb1 = Val;
        break;
    }
   //-------------------------------
    if (tempa1 != tempb1)
		m = YES;
	else
		m = NO;

		return m;
//-------------------------------------------------------------------
}

//----------------------------Motor1WaitProgram--------------------------------------------------------------
static void Motor1WaitProgram(void)
{
    if (Motor1_postion_user  > Motor1_postion_Current)  
    {
        Motor1_user_postion_gap   = Motor1_postion_user - Motor1_postion_Current;  
        if (Motor1_dir_Current == 1)  //MotorYSpeedPos
        {
            Motor1WillBackFlag = 0;
        }
        else
        {
            Motor1WillBackFlag = 1;
        }
        if (Motor1_Stop_Flag) Motor1_speed_postion_dir = YES;
    }
    //--------------------------------------------------------------------------------------------------------------------------
    if (Motor1_postion_user  < Motor1_postion_Current)  
    {
        Motor1_user_postion_gap   = Motor1_postion_Current  - Motor1_postion_user;  
        if (Motor1_dir_Current == 0)
        {
            Motor1WillBackFlag = 0;
        }
        else
        {
            Motor1WillBackFlag = 1;
        }
        if (Motor1_Stop_Flag) Motor1_speed_postion_dir = NO;
    }
//-----------------------------------------------------------------------------------
    if (Motor1_user_postion_gap <= 8*MaxStep)
    {
        step_startstop = Motor1_user_postion_gap/MaxStep / 4;
    }
    else if ((8*MaxStep < Motor1_user_postion_gap) && (Motor1_user_postion_gap < MotorYStep_StartStop*2*MaxStep)) //<=512*2
    {   // only two state
        step_startstop = Motor1_user_postion_gap/MaxStep / 2;
    }
    else
    {
        step_startstop = MotorYStep_StartStop;
    }
}








//==============================================================================================================
//----------------------ϸ����ѡ��128-8����-------------------------------------------------

//-------------------------------------------------------------------------------------------------------------------------
/*****************************************************************************************
*
*				 ���������������λ�ã������ٶȺ��� 
*       
*****************************************************************************************/
static void Motor1DynamicPostionGap(void)             // LOOP IN TIME0 48us
{
    
//------------------------------------------------------------------------
    if (!MotorYSpeedPos)
    {
        Motor1_Stop_Flag = 1;
    }
    else
    {
        Motor1_Stop_Flag = 0;
    }
    //-------------------------------------------------------------------
    if (Motor1_Stop_Flag)  //�����ǰ����Ѿ�ֹͣ  �ټ��
    {
        Motor1WaitProgram();
    }
//---------------------------------------------------------------------------------------------------------------
    
	if (Motor1_postion_user == Motor1_postion_Current)
    {
	    Motor1_dynamic_postion_gap = 0;

	} 
	//---------------------------------------------------------------------------------
    else if (Motor1_postion_user  > Motor1_postion_Current)  
    {  // ͬ��
        Motor1_dynamic_postion_gap  = Motor1_postion_user - Motor1_postion_Current;   //λ�ò� Postion_gap   һ��λ�ò�Ϊ10�� һȦ200��
		if (Motor1_Stop_Flag) Motor1_speed_postion_dir = YES;
    }
	//-----------------------------------------------------------------------------------
    else if (Motor1_postion_user < Motor1_postion_Current)       
    {
        Motor1_dynamic_postion_gap  = Motor1_postion_Current - Motor1_postion_user;    //����λ�ò�ֵ
		if (Motor1_Stop_Flag)  Motor1_speed_postion_dir = NO;
    }
//-----------------------------------------------------------------------------------------------------------------
}

//----------------------------------------------------------------------


/*****************************************************************************************
*
*				 ��������������� 
*       
*      Motor1_drive(Motor1_speed_postion_dir,20,Motor1_dynamic_postion_gap/100);
*****************************************************************************************/
static void Motor1_drive(u8 dir,u16 SpeedPostion)    //STEP  �����������  200�� ΪһȦ
{
    static u16 SpeedVal =0;

	SpeedVal = SpeedCurveTimerVal[SpeedPostion];
	
    //----------------------------------------------------------------------
    Motor1_dir      = dir;
    TIM1CCR4_Val	= 65535 - SpeedVal;  //u16
    //T0Tcnt0Val      = speed;   //�ı䶨ʱ����ֵ �ı�ÿ��΢����ʱ��  //test?
}



/*****************************************************************************************
*
*				 ����������ݵ�ǰλ�� ���� �ٶ� �ﵽָ��λ�� �������� 
*                                
*****************************************************************************************/
static void Motor1PostionControlSpeed(void)          // LOOP IN T0
{

//---------------------------------------------------------------------------------------
	if (CheckValChange3(Motor1_dynamic_postion_gap))
	{
	  	if (((Motor1_user_postion_gap-step_startstop*MaxStep) < Motor1_dynamic_postion_gap) && (Motor1_dynamic_postion_gap <= Motor1_user_postion_gap))
            LineFlag = 1;  //1 ����  0 ����  2 ���� (ֹͣ)  
	}

    Motor1_drive(Motor1_speed_postion_dir,MotorYSpeedPos);

	//MicroStep and MaxStep  Motor1_postion_Current	  Motor1_dynamic_postion_gap
	//-------- CheckValChange --------
    if (CheckValChange2(Motor1_dynamic_postion_gap/MaxStep))
    {
        if (Motor1WillBackFlag==0)
        {   //-----------------------------------------------------------------
            if ((0 < Motor1_dynamic_postion_gap) && (Motor1_dynamic_postion_gap <= MotorYSpeedPos*MaxStep))  //loop 20*256/x (256-32) 20Ϊ20��
            {                
                if (MotorYSpeedPos)
                {
                    MotorYSpeedPos--;  //�����ƶ�
                    LineFlag = 0;
                }
            }//-----------------------------------------------------------------
            else if ((step_startstop*MaxStep < Motor1_dynamic_postion_gap) && (Motor1_dynamic_postion_gap <= (Motor1_user_postion_gap-step_startstop*MaxStep)))
            {
				LineFlag = 2;
            }//-----------------------------------------------------------------
            else if (((Motor1_user_postion_gap-step_startstop*MaxStep) < Motor1_dynamic_postion_gap) && (Motor1_dynamic_postion_gap <= Motor1_user_postion_gap))
            {
                if (MotorYSpeedPos<step_startstop)
                {
					          MotorYSpeedPos++;  //��������
                    LineFlag = 1;
                }
            }//-----------------------------------------------------------------
            else if (Motor1_dynamic_postion_gap > Motor1_user_postion_gap)
            {
                Motor1WillBackFlag = 1;  //�����ƶ�
            }
        }
        //----------------------------------------------------------------------------------------
        else if ((Motor1WillBackFlag==1))//|(Motor1MakeStopFlag==1)) //stop
        {   
            if (MotorYSpeedPos)
            {
                MotorYSpeedPos--;
                LineFlag = 0;
            }
            else
            {
                Motor1WillBackFlag = 0;
            }
        }
    }

//----------------------------------------------------------------------------------------------------------------------------------------------
}


/*****************************************************************************************
*      ��ǰ�������״̬�Ĵ���   ����  �ٶ�  �Ƕ� ����Ϣ��¼
*	      Motor_state        0x01   ==> ����   0x00   ==> ����       		0   1
*  1111 1111 1111 1111    1111 1111 1111 1111         4*8 = 32
*                                              ����Ϊ 00-07λ               0 - 1
*				  		�ٶ�Ϊ 08-15λ         0X0000-0x0000 ff00                0-255
*						λ��Ϊ 16-23λ  ��ʾ  0X00 00 00 00 -  0X00 FF 00 00     0-255
*****************************************************************************************/
static void Motor1PointerControl(void)        //100us*32 == 3.2ms     LOOP in Time0
{
	static u16 iStop = 0;

	//----------------------------------------------------------------------DmxFinedataUser
    if ((Motor1_postion_Current == Motor1_postion_user)&&(!MotorYSpeedPos))//&&(DmxFinedataUser==CounterFinedataCurrent))
    { 
        Motor1_user_postion_gap    = 0;
        Motor1_dynamic_postion_gap = 0;

		LineFlag = 2;//YES;		

        //------------------------------------------------------------       
        if (iStop > 100) //655us/2*500
        {
            TIM4->CCR1 =          (SinData[MotorPositionPointer])>>2;
            TIM4->CCR2 = (CyclePWM-SinData[MotorPositionPointer])>>2;
            TIM4->CCR3 =          (CosData[MotorPositionPointer])>>2;
            TIM4->CCR4 = (CyclePWM-CosData[MotorPositionPointer])>>2;
            //LED_FLASH(1);
        }
        else
        {
            iStop++;
        }
        //----------------STOP AND AUTO TURN BACK---------------------
		        //Motor1_StopAutoBACK();
        //------------------------------------------------------------
    }
    else   //---(Motor1_postion_Current != Motor1_postion_user)---
    {
        //Motor1_Arrive_Flag = 0;   //���ֹͣ �û��趨λ��δ���� ���
		//------------------------------------------------------------
      iStop = 0;   
        
	    TIM4->CCR1 =          SinData[MotorPositionPointer];
      TIM4->CCR2 = CyclePWM-SinData[MotorPositionPointer];
      TIM4->CCR3 =          CosData[MotorPositionPointer];
      TIM4->CCR4 = CyclePWM-CosData[MotorPositionPointer];


		//---------����ת����-----------------------------------------
        if (Motor1_dir == 1)            //�û������Ʒ��� Ϊ����ʱ  
            Motor1_dir_Current     = 1;
        else                           //�û������Ʒ��� Ϊ����ʱ
            Motor1_dir_Current     = 0;
		//------------------------------------------------------------ 
        
      if (Motor1_dir_Current == YES)   //���Ʋ������ ������ת
      {
          //������ֹͣת�� ����״̬
        
            MotorPositionPointer += 8;
            CounterMotorYStepPostionCurrent += 1;

          //--------------------------------------------------          
	     if (MotorPositionPointer >= 512)  MotorPositionPointer = 0; 
     }
     else                //���Ʋ������ ������ת
     {                          
				
            MotorPositionPointer -= 8;
            CounterMotorYStepPostionCurrent -= 1;
	
          //--------------------------------------------------
          if (MotorPositionPointer <= 0)  MotorPositionPointer = 512; 
     }
  } 

//---------------------------------------------------------------------------------------------------------------
	Motor1_postion_Current = CounterMotorYStepPostionCurrent;
    //��ǰ״̬���  ������״̬�Ĵ��� ���뵱ǰ״̬	    
//---------------------------------------------------------------------------------------------------------------
}

/*****************************************************************************************
*
*				 �������  λ��  ���ƺ��� 
*                         void Motor1_postion_control(u16 Motor1_postion)   0-255
*****************************************************************************************/
static void Motor1_postion_control(s32 postion)   // һ��λ�� ==  ��
{

    if (CheckValChange1(postion))
    {
        //-------------------------------------------------------------------------------------------------
        Motor1_postion_user      = postion;     //�û��趨λ��  ȫ�ֱ���
        Motor1WaitProgram();
    }//-----------------------------------------------------------------------------------------------------
}

/********************************************************************
*
*          Motor1DmxDataControl
*
*********************************************************************/
void MotorYDmxDataControl(u8 Dmxdata,u8 Finedata)
{    //0-255 ==== 0-9R*200 =1800  1800/255 = 7.05     540/360=1.5 *���==> 1.5*6=9
    static s32 i=0;
	static u8 RatioA  = 8;//50;   //8  9
	static u8 RatioB  = 1;//255;   //1

	i = (s32)((Dmxdata*MaxStep*RatioA/RatioB)+Finedata+StartPostion);

        //---------ÿ������һ��1.8--------------------------------------------------	
	//i = (Dmxdata*RatioA/RatioB)+(Finedata)/MaxStep+StartPostion;  //+100 6	 Step
	//DmxFinedataUser = (Finedata)%MaxStep;	
						  //Micro-Step
	//---------ÿ��С��һ��1.8--------------------------------------------------
	//i = Dmxdata*RatioA/RatioB + StartPostion;
    //DmxFinedataUser = (u8)(Dmxdata*RatioA*MaxStep/RatioB) % MaxStep;

    //i = 20*Dmxdata;
    Motor1_postion_control(i);
}

/*****************************************************************************************
*
*				 ������� �������� 
*                                     
*****************************************************************************************/
void MotorYDriveInTimer(void)
{
    Motor1PointerControl();
    Motor1DynamicPostionGap();
    Motor1PostionControlSpeed(); 
}













/***************************************************************************************************************
*                                             END
***************************************************************************************************************/
