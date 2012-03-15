

#include "stm32f10x.h"
#include "Graphics.h"
#include "button.h"

/**************************************************************************************
功能描述: 在屏幕显示一凸起的按钮框
输    入: u16 x1,y1,x2,y2 按钮框左上角和右下角坐标
输    出: 无
**************************************************************************************/
void DisplayButtonUp(u16 x1,u16 y1,u16 x2,u16 y2)
{
	LcdPrintHorz(x1,  y1,  x2 - x1+1, GRAY2);
	LcdPrintHorz(x1+1,y1+1,x2 - x1-1, GRAY1);
	LcdPrintVert(x1,  y1,  y2 - y1+1, GRAY2);
	LcdPrintVert(x1+1,y1+1,y2 - y1-1, GRAY1);
	LcdPrintHorz(x1,  y2,  x2 - x1+1, WHITE);
	LcdPrintVert(x2,  y1,  y2 - y1+1, WHITE);
}

/**************************************************************************************
功能描述: 在屏幕显示一凹下的按钮框
输    入: u16 x1,y1,x2,y2 按钮框左上角和右下角坐标
输    出: 无
**************************************************************************************/
void DisplayButtonDown(u16 x1,u16 y1,u16 x2,u16 y2)
{
	LcdPrintHorz(x1,  y1,  x2 - x1+1, WHITE);
	LcdPrintVert(x1,  y1,  y2 - y1+1, WHITE);
	
	LcdPrintHorz(x1+1,y2-1,x2 - x1-1, GRAY1);
	LcdPrintHorz(x1,  y2,  x2 - x1+1, GRAY2);
	LcdPrintVert(x2-1,y1+1,y2 - y1-1, GRAY1);
        LcdPrintVert(x2  ,y1  ,y2 - y1+1, GRAY2);
}

void DisplayButtonDownColor(u16 x1,u16 y1,u16 x2,u16 y2,u16 color)
{
	LcdPrintHorz(x1,  y1,  x2 - x1+1, WHITE);
	LcdPrintVert(x1,  y1,  y2 - y1+1, WHITE);
	
	LcdPrintHorz(x1+1,y2-1,x2 - x1-1, GRAY1);
	LcdPrintHorz(x1,  y2,  x2 - x1+1, GRAY2);
	LcdPrintVert(x2-1,y1+1,y2 - y1-1, GRAY1);
    LcdPrintVert(x2  ,y1  ,y2 - y1+1, GRAY2);
	LcdFillRec(x1+1,y1+1,x2-2,y2-2,color);
}


/**************************************************************************************
功能描述: 显示一组按钮
输    入: struct BUTTONDEFINE *buttons	目标按钮的结构体
输    出: 无
**************************************************************************************/
void DispButton( struct BUTTONDEFINE *buttons )
{
	u16 i=0;
	while(buttons[i].id)
	{
		if(buttons[i].types)
		{
			if(buttons[i].enable)
			{
				LcdFillRec(buttons[i].x1,buttons[i].y1,buttons[i].x2,buttons[i].y2,GRAY0);
				DisplayButtonDown(buttons[i].x1,buttons[i].y1,buttons[i].x2,buttons[i].y2 );
				LcdPrintStr(buttons[i].str,buttons[i].x1+ buttons[i].left,buttons[i].y1+buttons[i].top,BLACK,GRAY0);
			}
			else
			{
				LcdFillRec(buttons[i].x1,buttons[i].y1,buttons[i].x2,buttons[i].y2,GRAY0);
				DisplayButtonDown(buttons[i].x1,buttons[i].y1,buttons[i].x2,buttons[i].y2 );
				LcdPrintStr(buttons[i].str,buttons[i].x1+ buttons[i].left,buttons[i].y1+buttons[i].top,GRAY1,GRAY0);
			}
		}
		i++;
	}
}
