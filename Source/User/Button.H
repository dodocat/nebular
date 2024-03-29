
#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "stm32f10x.h"




struct BUTTONDEFINE
{
	u8 id;         		               		// ��Ŧ���º��id����ֵ
	u16 x1, y1;  		        	       	// ��Ŧ���Ͻ�����
	u16 x2, y2;				       	// ��Ŧ���½�����
	u8 left;					// �ַ�������߿����
	u8 top;						// �ַ������ϱ߿����
	u8 types;                                    	// ��ť����
	u8 enable;					// ��ť����
	char *str;			       		// ��Ŧ����ʾ����
};
extern void DisplayButtonUp(u16 x1,u16 y1,u16 x2,u16 y2);
extern void DisplayButtonDown(u16 x1,u16 y1,u16 x2,u16 y2);
extern void DispButton(struct BUTTONDEFINE *buttons);
extern void DisplayButtonDownColor(u16 x1,u16 y1,u16 x2,u16 y2,u16 color);
#endif
