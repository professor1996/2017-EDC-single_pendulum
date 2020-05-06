#include "IIC.h"

u8 mpu6050_buffer[14];				


void IIC_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;// ?? CLK ? SDA
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD ;   //????
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_1 | GPIO_Pin_0);
}


void SDA_IOOUT(void)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 ;			
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_OD;		// ????
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOA, &GPIO_InitStructure);				 	    
}

void SDA_IOIN(void)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 ;			
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN_FLOATING;  // ????
	GPIO_Init(GPIOA, &GPIO_InitStructure);				     			
}

static void I2C_delay(void)
{
  volatile int i = 1;
    while (i)
        i--;
}

void I2C_Start(void)
{
	SCL_L;					
	I2C_delay();
	SDA_H;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SDA_L;
	I2C_delay();  
} 

void I2C_Stop(void)
{
	SCL_L;					
	I2C_delay();
	SDA_L;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SDA_H;
	I2C_delay();
}

unsigned char I2C_SlaveAck(void)
{
	SCL_L;				
	I2C_delay();
	SDA_H;
	
	SDA_IOIN();					
	I2C_delay();
	SCL_H;
	
	I2C_delay();
	
	if(SDA_D)
	{
		SDA_IOOUT();			
		SCL_L;
		return ERROR;			
	}
	SCL_L;
	SDA_IOOUT();					
	I2C_delay();
	return SUCCESS;						  
}

void I2C_WriteByte(unsigned char a) 
{
	unsigned char i; 
	for(i=0; i<8; i++)
	{	  	
		SCL_L;
		if((a&0x80)!=0) SDA_H;
		else SDA_L;
		a <<= 1;
		I2C_delay();
		SCL_H;
		I2C_delay();
	}   
	SCL_L;  

	if(I2C_SlaveAck()==ERROR)		//?????ACK???
	{
		return ;
	}
}

unsigned char I2C_ReadByte(void)
{
	unsigned char a =0;
	unsigned char i;
	SDA_IOIN();					//SDA????	
	for(i=0; i<8; i++)
	{
 		a <<= 1;  
		SCL_H;
		I2C_delay();
		if(SDA_D==1) a |= 0x01;
    	I2C_delay();
		SCL_L;
	  	I2C_delay();   
	}
	return a;
}

void Single_WriteI2C(unsigned char Regs_Addr,unsigned char Regs_Data) 
{  
   I2C_Start();
   I2C_WriteByte(SlaveAddress); 		
   I2C_WriteByte(Regs_Addr);    		
   I2C_WriteByte(Regs_Data);    		
   I2C_Stop();                  		
}

unsigned char Single_ReadI2C(unsigned char Regs_Addr) 
{
   unsigned char ret;
   
   I2C_Start();
   
   I2C_WriteByte(SlaveAddress); 		
   I2C_WriteByte(Regs_Addr);    	
   
   I2C_Start();
   I2C_WriteByte(SlaveAddress+1);  
   ret=I2C_ReadByte();						

   I2C_SlaveAck();
   I2C_Stop();                  		
   
   return ret;
} 


int GetData(u8 REG_Address)
{
	u8 H,L;
	H=Single_ReadI2C(REG_Address);
	L=Single_ReadI2C(REG_Address+1);
	return (H<<8)+L;   //????
}



