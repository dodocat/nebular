
#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "stm32f10x.h"




struct BUTTONDEFINE
{
	u8 id;         		               		// 按纽按下后的id返回值
	u16 x1, y1;  		        	       	// 按纽左上角坐标
	u16 x2, y2;				       	// 按纽右下角坐标
	u8 left;					// 字符距离左边框距离
	u8 top;						// 字符距离上边框距离
	u8 types;                                    	// 按钮类型
	u8 enable;					// 按钮允许
	char *str;			       		// 按纽的显示内容
};
extern void DisplayButtonUp(u16 x1,u16 y1,u16 x2,u16 y2);
extern void DisplayButtonDown(u16 x1,u16 y1,u16 x2,u16 y2);
extern void DispButton(struct BUTTONDEFINE *buttons);
extern void DisplayButtonDownColor(u16 x1,u16 y1,u16 x2,u16 y2,u16 color);
#endif
