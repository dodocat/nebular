
#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

//-------------------------------------------------------------------------------------
#include "stm32f10x.h"

#define   BLACK    0x0000      	//黑色
#define   BLUE     0xF800      	//蓝色
#define   RED      0x001F      	//红色
#define   GRAY0    0xCE9A   	//灰色0 3165 00110 001011 00101
#define   GRAY1    0x8410      	//灰色1      00000 000000 00000
#define   GRAY2    0x4208      	//灰色2  1111111111011111
#define   WHITE    0xFFFF		//白色   110010 110100 110100
#define   YELLOW   0xF800      	//黄色07FF
#define   BLACK        0x0000                    // 黑色： 0, 0, 0              
#define   NAVY         0x000F                    // 深蓝色： 0, 0, 128          
#define   DGREEN       0x03E0                    // 深绿色： 0, 128, 0          
#define   DCYAN        0x03EF                    // 深青色： 0, 128, 128        
#define   MAROON       0x7800                    // 深红色：128, 0, 0           
#define   PURPLE       0x780F                    // 紫色： 128, 0, 128          
#define   OLIVE        0x7BE0                    // 橄榄绿：128, 128, 0         
#define   LGRAY        0xC618                    // 灰白色：192, 192, 192        
#define   DGRAY        0x7BEF                    // 深灰色：128, 128, 128        
//#define   BLUE         0x001F                    // 蓝色： 0, 0, 255            
#define   GREEN        0x07E0                 	 // 绿色： 0, 255, 0             
#define   CYAN         0x07FF                    // 青色： 0, 255, 255           
//#define   RED          0xF800                    // 红色： 255, 0, 0             
#define   MAGENTA      0xF81F                    // 品红： 255, 0, 255          
//#define   YELLOW       0xFFE0                    // 黄色： 255, 255, 0           
#define   WHITE        0xFFFF                    // 白色： 255, 255, 255         

extern const struct __ASC_ZK sysEnzk[];
extern const struct __HZK_ZK sysHzzk[];

extern void LcdSetArea(u16 x1, u16 y1, u16 x2, u16 y2);
extern void LcdPrintDot(u16 x, u16 y, u16 color);
extern void LcdPrintLine (u16 x1, u16 y1, u16 x2, u16 y2, u16 color);
extern void LcdPrintHorz(u16 x, u16 y, u16 width, u16 color);
extern void LcdPrintVert(u16 x, u16 y, u16 height, u16 color);
extern void LcdPrintCircle(uint16_t x0,uint16_t y0,uint16_t r, u16 color);

extern void LcdPrintRec(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);
extern void LcdFillRec(u16 x1, u16 y1, u16 x2, u16 y2,u16 color);
extern void LcdClear(u16 color);
extern void LcdPrintEn(u8 code, u16 x, u16 y, u16 color,u16 bcolor);
extern void LcdPrintHz(u16 code,u16 x,u16 y,u16 color,u16 bcolor);
extern void LcdPrintStr(char *str,u16 x,u16 y,u16 color,u16 bcolor);
extern void LcdPrintf(u16 x,u16 y,u16 color,u16 bcolor,char *fmt,...);
extern void LcdPrint16bitBmp(const u8* bmpbit,u16 x,u16 y,u16 widht,u16 height);



#endif
