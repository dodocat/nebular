
#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

//-------------------------------------------------------------------------------------
#include "stm32f10x.h"

extern void LcdSetArea(u16 x1, u16 y1, u16 x2, u16 y2);
extern void LcdPrintDot(u16 x, u16 y, u16 color);
extern void LcdPrintHorz(u16 x, u16 y, u16 width, u16 color);
extern void LcdPrintVert(u16 x, u16 y, u16 height, u16 color);
extern void LcdPrintRec(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);
extern void LcdFillRec(u16 x1, u16 y1, u16 x2, u16 y2,u16 color);
extern void LcdClear(u16 color);
extern void LcdPrintEn(u8 code, u16 x, u16 y, u16 color,u16 bcolor);
extern void LcdPrintHz(u16 code,u16 x,u16 y,u16 color,u16 bcolor);
extern void LcdPrintStr(char *str,u16 x,u16 y,u16 color,u16 bcolor);
extern void LcdPrintf(u16 x,u16 y,u16 color,u16 bcolor,char *fmt,...);
extern void LcdPrint16bitBmp(const u8* bmpbit,u16 x,u16 y,u16 widht,u16 height);



#endif
