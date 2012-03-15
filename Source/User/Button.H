
#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "stm32f10x.h"


#define   BLACK    0x0000      	//��ɫ
#define   BLUE     0xF800      	//��ɫ
#define   RED      0x001F      	//��ɫ
#define   GRAY0    0xCE9A   	//��ɫ0 3165 00110 001011 00101
#define   GRAY1    0x8410      	//��ɫ1      00000 000000 00000
#define   GRAY2    0x4208      	//��ɫ2  1111111111011111
#define   WHITE    0xFFFF	//��ɫ   110010 110100 110100

#define   YELLOW   0xF800      	//��ɫ07FF


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