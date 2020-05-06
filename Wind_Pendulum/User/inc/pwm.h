#include "stm32f10x.h"


void TIM3_Init(void);

void PWM_M1_Forward(float pwm1);
void PWM_M1_Backward(float pwm1);
void PWM_M2_Forward(float pwm2);
void PWM_M2_Backward(float pwm2);
void PWM_M3_Forward(float pwm3);
void PWM_M3_Backward(float pwm3);
void PWM_M4_Forward(float pwm4);
void PWM_M4_Backward(float pwm4);


extern float pwm1;
extern float pwm2;
extern float pwm3;
extern float pwm4;

