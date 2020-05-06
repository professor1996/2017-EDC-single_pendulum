#ifndef __DMA_H
#define	__DMA_H

#include "sys.h"
#include "delay.h"
#include "usart.h"

#define SIZE 35

#define USART1_DR_Base  0x40013804		// 0x40013800 + 0x04 = 0x40013804

void USART1_DMA_Config(void);  //DMA≥ı ºªØ

#endif
