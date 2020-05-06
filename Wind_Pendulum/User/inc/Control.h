#include "stm32f10x.h"
#include "pwm.h"
#include "PID.h"
#include "math.h"
#include "Kalman_Filter.h"



extern u8 Mode;

void Mode_0(void);
void Mode_1(void);
void Mode_2(void); 
void Mode_3(void);
void Mode_4(void);
void Mode_5(void);
void Mode_6(void);


void MotorPower(float pwm1 , float pwm2);
