#include "DataInit.h"



PidTypeDef PidDataSpeed ;
PidTypeDef PidDataSpeed_Right;
PidTypeDef PidDataStation;



void PID_InitALL(void)                //初始化位置环，速度环参数
{
	
	PID_Init(&PidDataSpeed);
 	PID_SetParam(&PidDataSpeed , 10.0 , 0.05 , 0 , 4000 , 4000 , 0 , 0 , 0 , 4500);		             // P I D Beta Outbeta Dif_prior Kaff Kvff Full_beta

//	PID_Init(&PidDataStation);
// 	PID_SetParam(&PidDataStation , 0.1 , 0.05 , 0 , 4000 , 100 , 0 , 0 , 0 , 4500);		             // P I D Beta Outbeta Dif_prior Kaff Kvff Full_beta

}


