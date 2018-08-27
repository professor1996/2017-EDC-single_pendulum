#include "adc.h"

int adc_flag;
float VolValue = 0.00;

void ADC_Configuration(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_ADC1  , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE ;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE ;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right ;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1 , &ADC_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB , &GPIO_InitStructure);

	ADC_RegularChannelConfig(ADC1 , ADC_Channel_9 , 1 , ADC_SampleTime_55Cycles5);
	ADC_Cmd(ADC1 , ENABLE);
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));
	ADC_SoftwareStartConvCmd(ADC1 , ENABLE);
	
}


float Get_Adc(void)   
{   
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);  //使能ADC1的转换功能  
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));//判断ADC转换结束标志位 等待转换结束  
	return ADC_GetConversionValue(ADC1);    //返回最近一次ADC的数据  
}


float Angle;
void Read_Vol(void)
{
	 VolValue = 3.3 * ADC_GetConversionValue(ADC1) /4096; 
   Angle = VolValue * 69;
	 Filter();
//	 Angle = (int)Angle;
//	 VolValue = VolValue * 100;
//	 VolValue = (int)VolValue;
//	 filter();
}


#define A 0.05

float value;
float new_value;

//限幅滤波法
void Filter(void)
{ 
	new_value = Angle;
	
	if( (new_value - value > A) || (value - new_value > A) )
	{
		Angle = value;
	}
	else
	{
	  Angle = new_value;
	}
	
	value = new_value;

}

	
	
	
