#include "stm32f10x.h"

#include "common.h"

void DelayMs(u32 nMs)
{
	u32 i;
	
	for(; nMs !=0; nMs--)
	{
		i = 10301;
		while(i--);
	}
}

void Delay_Us(u32 time)  //ÑÓÊ±º¯Êý
{ 
	uint16_t i,j;
	for(i=0;i<time;i++)
  		for(j=0;j<9;j++);
}


