#ifndef __Kalman_Filter_H
#define __Kalman_Filter_H	


#include "MPU6050.h"

extern float Angle_X_Final;
extern float Angle_Y_Final;
extern float Angle_X  ;  
extern float Angle_Y  ;  
extern float Angle_X_dirta ;  
extern float Angle_Y_dirta ;  
extern float Angle_x_temp;  //由加速度计算的x倾斜角度
extern float Angle_y_temp;  //由加速度计算的y倾斜角度

void Angle_Calcu(void);
void Kalman_Filter_X(float Accel,float Gyro);
void Kalman_Filter_Y(float Accel,float Gyro);




#endif



