#ifndef _DEBUG_STM32_
#define _DEBUG_STM32_

#include "stm32f10x.h"

extern void UART1_SendByte (u8 data);
extern void PrintStr(char *buffer);
extern void Printf(char *fmt,...);
extern void PrintBuff(char *buffer,unsigned long len);
extern void UART3_Init(u32 Baud);


#endif
