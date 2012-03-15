
#include "debug_STM32.h"


#include <stdio.h>
#include <stdarg.h>

void UART3_Init(u32 Baud)
{
  	USART_InitTypeDef    USART_InitStructure;
  	GPIO_InitTypeDef     GPIO_InitStructure;
  	
  	USART_InitStructure.USART_BaudRate = Baud;
  	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  	USART_InitStructure.USART_StopBits = USART_StopBits_1;
  	USART_InitStructure.USART_Parity = USART_Parity_No;
  	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  	
  	//Enable  clock
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
  	
  	//Configure USART Tx as alternate function push-pull
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);
  	  
  	//Configure USART Rx as input floating
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);
  	
  	USART_Init(USART3, &USART_InitStructure);
  	USART_Cmd(USART3, ENABLE);
}

void UART3_SendByte (u8 data)
{
	USART_SendData(USART3, data);
	while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
}

void PrintStr(char *buffer)
{
	while(*buffer != '\0')
	{
		UART3_SendByte(*buffer++);
	}
}

void Printf(char *fmt,...)
{
    va_list ap;
    char string[256];
    
    va_start(ap,fmt);
    vsprintf(string,fmt,ap);
    PrintStr(string);
    va_end(ap);
}

void PrintBuff(char *buffer,unsigned long len)
{
	while(len--)
	{
		UART3_SendByte(*buffer++);
	}
}
