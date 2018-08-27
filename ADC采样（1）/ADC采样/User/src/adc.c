#include "adc.h"

int adc_flag;
extern float VolValue;
extern float VolValue_Local;
extern int a;

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

float Get_Angular_Speed(void)   
{   
	float Speed;
	Speed =(VolValue-VolValue_Local)*72/(0.0002*a);
	return Speed;    //返回最近一次ADC的数据  
}



