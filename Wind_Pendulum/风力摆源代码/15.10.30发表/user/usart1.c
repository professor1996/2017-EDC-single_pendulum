/*---------------------------------------------------------
	      ����1���ó���
	˵��: �������ó�����Ҫ����stdarg.h����ʹ��
	      �ܶ���ӿ�PA10->RXD  PA9->TXD,USART1
		  ʹ��printf��ʱ���\r\n��������س���
		  �����Ҫʹ�ô����ж������misc.c�ļ�
		  ���Ұ���misc.hͷ�ļ�,����NVIC�ж�Դ
		  ��stm32f10x_it.c��д�жϷ������
    ʱ��: 2013��7��24�� BoX��д 
---------------------------------------------------------*/
#include "stm32f10x.h"
#include "usart1.h"
#include "misc.h"   //�����Ҫ���ж��������ͷ�ļ�
#include <stdarg.h>

void USART1_Config(void)
{
 	GPIO_InitTypeDef IO_Init;
	USART_InitTypeDef USART_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA , ENABLE);
	NVIC_Config(); //�õ������ж�,�����жϲ���
	//A9�����������
	IO_Init.GPIO_Pin = GPIO_Pin_9;
	IO_Init.GPIO_Mode = GPIO_Mode_AF_PP;
	IO_Init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&IO_Init);
	//A10��������
	IO_Init.GPIO_Pin = GPIO_Pin_10;
	IO_Init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&IO_Init);
	//���ô���1
	USART_InitStructure.USART_BaudRate = 115200;  //���ò�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//��λ����λ
	USART_InitStructure.USART_StopBits =  USART_StopBits_1;		//1λֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;			//��У��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ����
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	  //���͡�����ģʽ
	USART_Init(USART1,&USART_InitStructure);  //д������
	USART_Cmd(USART1,ENABLE);				  //ʹ�ܴ���
	USART_ITConfig(USART1,USART_IT_RXNE,DISABLE); //ʹ�ܽ����ж�
}

void NVIC_Config()
{
	/* ���Ҫ���ö���ж������Ĵ���ֻ�ֿܷ���������,����һ������ */
	NVIC_InitTypeDef NVICInit;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);	//��ռ��Ӧ���ȼ���1
	//���ڽ����ж�
	/* �ж�����stm32f10x.h�ļ���188�п����ҵ������ж����б�,������Ҫѡ�� */
	NVICInit.NVIC_IRQChannel = USART1_IRQn;	
	NVICInit.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�2
    NVICInit.NVIC_IRQChannelSubPriority = 1;  		 //��Ӧ���ȼ�1
	NVICInit.NVIC_IRQChannelCmd = DISABLE;	         //ʹ���ж�
	NVIC_Init(&NVICInit);                            //д������
}

/* ʵ����printf�������ض������� */
int fputc(int ch,FILE *f)
{
 	USART_SendData(USART1, (unsigned char) ch);	//����stm32f10x_usart.c�еĺ���
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) != SET);  //�ȴ��������
	return ch;
}
/*---------------------------------------------------------
	��������: �Զ��������printf���ܺ���
	��������: ��Ҫ�������ݵĴ��ں�USARTx
			  ��Ҫ��ӡ������data
	���ز���: ��
---------------------------------------------------------*/
void USART1_printf(USART_TypeDef* USARTx, uint8_t *Data,...)
{
	const char *s;
	int d;   
	char buf[16];
	
	va_list ap;
	va_start(ap, Data);
	
	while ( *Data != 0)     // �ж��Ƿ񵽴��ַ���������
	{				                          
		if ( *Data == 0x5c )  //'\'
		{									  
			switch ( *++Data )
			{
				case 'r':							          //�س���
					USART_SendData(USARTx, 0x0d);
					Data ++;
				break;
				
				case 'n':							          //���з�
					USART_SendData(USARTx, 0x0a);	
					Data ++;
				break;
				
				default:
					Data ++;
				break;
			}			 
		}
		else if ( *Data == '%')
		{									  //
			switch ( *++Data )
			{				
				case 's':										  //�ַ���
					s = va_arg(ap, const char *);
					for ( ; *s; s++) 
					{
						USART_SendData(USARTx,*s);
						while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
					}
					Data++;
				break;
			
				case 'd':										//ʮ����
					d = va_arg(ap, int);
					itoa(d, buf, 10);
					for (s = buf; *s; s++) 
					{
						USART_SendData(USARTx,*s);
						while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
					}
					Data++;
				break;
			    default:  Data++;
				break;
			}		 
		} 
		else USART_SendData(USARTx, *Data++); //���û����ת���ַ����������
		while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
	}
}
/*--------------------------------------------------
	��������: ��������ת�����ַ�������
	��������: ��Ҫת����������value
	          ת������ַ��� string
			  radix = 10
	��������: ��
---------------------------------------------------*/
static char *itoa(int value, char *string, int radix)
{
	int     i, d;
	int     flag = 0;
	char    *ptr = string;
	/* This implementation only works for decimal numbers. */
	if (radix != 10)
	{
	    *ptr = 0;
	    return string;
	}
	if (!value)
	{
	    *ptr++ = 0x30;
	    *ptr = 0;
	    return string;
	}
	/* if this is a negative value insert the minus sign. */
	if (value < 0)
	{
	    *ptr++ = '-';
	    /* Make the value positive. */
	    value *= -1;
	}
	for (i = 10000; i > 0; i /= 10)	//��ÿ������ÿһλ�ֱ��ʽ�����ַ�
	{
	    d = value / i;
	    if (d || flag)
	    {
	        *ptr++ = (char)(d + 0x30);
	        value -= (d * i);
	        flag = 1;
	    }
	}
	/* Null terminate the string. */
	*ptr = 0;
	return string;
}

void USART1_IRQHandler(void)
{
   if(USART_GetITStatus(USART1,USART_IT_RXNE) == SET)  //����н����ж�
   {
		USART_ClearFlag(USART1,USART_IT_RXNE);  //����ж�λ
   }	
}

