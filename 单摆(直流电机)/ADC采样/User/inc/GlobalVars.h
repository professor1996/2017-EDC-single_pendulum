#ifndef __GlobalVars_H
#define __GlobalVars_H	



#include <stm32f10x.h>
#include "PID.h"





extern PidTypeDef PidDataSpeed_Left;
extern PidTypeDef PidDataSpeed_Right;
extern PidTypeDef PidDataStation;

void PID_InitALL(void);





#endif
