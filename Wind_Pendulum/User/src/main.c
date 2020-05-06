#include "stm32f10x.h"
#include "gpio_motor.h"
#include "pwm.h"
#include "IIC.h"
#include "MPU6050.h"
#include "Kalman_Filter.h"
#include "Delay.h"
#include "PID.h"
#include "Timer.h"

int a =1;
int main(void)
{
	
	TIM3_Init();
	gpio_motor1_init();
	gpio_motor2_init();
	gpio_motor3_init();
	gpio_motor4_init();
	
	IIC_Init();
	InitMPU6050();

	delay_ms(1000);
	Angle_X_dirta = Angle_x_temp;
	Angle_Y_dirta = Angle_y_temp;
	
	TIM2_Configuration();
	while(1)
	{
	
    a = 1;

		
		//Mode_1();
	}
	
}



