#include "Kalman_Filter.h"
#include "pwm.h"


extern float SetPoint_x ;
extern float SetPoint_y ;

extern float Proportion_x;
extern float Integral_x;
extern float Derivative_x;
extern float Proportion_y;
extern float Integral_y;
extern float Derivative_y;


float PID_M1_PosLocCalc(float NextPoint);
float PID_M2_PosLocCalc(float NextPoint);
