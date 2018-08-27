#include "motor.h"
#include "math.h"
#include "IMU.h"



void Mode_1(void);
void Mode_2(void);
void Mode_3(void);
void Mode_4(void);


extern float R  ;
extern float L   ;
extern float thelta     ;
extern float alfa       ;
extern float motor_turn_angle;
extern float motor_pulse_count ;
extern int peri_count ;
extern int positive_flag  ;
extern int negetive_flag  ;
extern float Actual_Angle;

extern int Encoder_Ratio    ;   //光栅条数为448线  , 电机减速比为  1：30 
extern float Real_Speed   ;
extern float Want_Speed ;          //500
extern int  Want_Pwm  ;
extern float Real_Location  ;
extern float Want_Location  ;





