#ifndef __INDKEY_H
#define __INDKEY_H

#include "stm32f10x.h"

#define KEY_PRESSED 0  //�������������費ͬ����
#define KEY_UNPRESS	1  //�������������費ͬ����

void Key_IO_Init(void);
void KeyScan(void);

#endif
