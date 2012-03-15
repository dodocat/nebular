
#include <stdio.h>
#include <stdarg.h>
#include "stm32f10x.h"
#include "SSD1963.h"
#include "SYS_ZK.H"

#define LCDBUFSIZE	800*480

extern void DelayMs(u32 nMs);
extern const struct __ASC_ZK sysEnzk[];
extern const struct __HZK_ZK sysHzzk[];



u16 fontbuf[256];

/**************************************************************************************
�������ܣ�����д�����ݵ�Ŀ������
���룺    ��������ϽǺ����½�����
�����	  ��
**************************************************************************************/
void LcdSetArea(u16 x1, u16 y1, u16 x2, u16 y2)
{
	WriteCommand(0x2A);	//���ÿ�ʼ�е�ַ,�����е�ַ
	WriteData(x1>>8);
	WriteData(x1);
	WriteData(x2>>8);
	WriteData(x2);
	WriteCommand(0x2B);	//���ÿ�ʼ�е�ַ,�����е�ַ
	WriteData(y1>>8);
	WriteData(y1);
	WriteData(y2>>8);
	WriteData(y2);
}
/**************************************************************************************
�������ܣ�����
���룺    u16  x,y    	        �������
	  u16  color		��ɫ
�����	  ��
**************************************************************************************/
void LcdPrintDot(u16 x, u16 y, u16 color)
{
	LcdSetArea(x,y,x,y);
	WriteCommand(0x2C);
	WriteData(color);
}

/**************************************************************************************
�������ܣ���ˮƽֱ��
���룺    WORD 	x    	        ���ĺ�����
	  WORD  y  	        ����������
	  WORD  width		ֱ�߳���
	  WORD  color		��ɫ
�����	  ��
**************************************************************************************/
void LcdPrintHorz(u16 x, u16 y, u16 width, u16 color)
{
	LcdSetArea(x,y,x+width-1,y);
	WriteCommand(0x2C);
	
	while(width--)
		WriteData(color);
}

/**************************************************************************************
�������ܣ�����ֱֱ��
���룺    u16  x    	        ���ĺ�����
	  u16  y  	        ����������
	  u16  height		ֱ�߸߶�
	  u16  color		��ɫ
�����	  ��
**************************************************************************************/
void LcdPrintVert(u16 x, u16 y, u16 height, u16 color)
{
	LcdSetArea(x,y,x,y+height-1);
	WriteCommand(0x2C);
	
        while(height--)
		WriteData(color);
}
/**************************************************************************************
�������ܣ������η���
���룺    ���ε����ϽǺ����½����꣬��Ŀ����ɫ
�����	  ��
**************************************************************************************/
void LcdPrintRec(u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
{
	LcdPrintHorz(x1, y1, x2 - x1+1, color);
	LcdPrintVert(x1, y1, y2 - y1+1, color);
	LcdPrintHorz(x1, y2, x2 - x1+1, color);
	LcdPrintVert(x2, y1, y2 - y1+1, color);
}

/**************************************************************************************
�������ܣ����һ����������
���룺    ���ε����ϽǺ����½����꣬��Ŀ����ɫ
�����	  ��
**************************************************************************************/
void LcdFillRec(u16 x1, u16 y1, u16 x2, u16 y2,u16 color)
{
	register u32 size;
	
	LcdSetArea(x1,y1,x2,y2);
	WriteCommand(0x2C);
	
	size = (u32)(y2-y1+1)*(x2-x1+1);
	while(size--)
		WriteData(color);
}

/**************************************************************************************
�������ܣ��������LCD��Ļ
���룺    Ŀ����ɫ
�����	  ��
**************************************************************************************/
void LcdClear(u16 color)
{
	register u32 size;

	LcdSetArea(0,0,799,479);
	WriteCommand(0x2C);
	
	size = LCDBUFSIZE;
	while(size--)
		WriteData(color);
}

/**************************************************************************************
�������ܣ���LCD�ϴ�ӡһ��Ӣ���ַ�
���룺    code       �ַ�����
          x,y	     Ŀ����ʾλ������
	  color	     ������ɫ
	  bcolor     ������ɫ
�����	  ��
**************************************************************************************/
void LcdPrintEn(u8 code, u16 x, u16 y, u16 color,u16 bcolor)
{
	u8 ip,i,j,m;

	ip = 0;
	m=0;
	do
	{
		if(sysEnzk[ip].code == code)
		{
			for(i = 0; i < 16; i++)//��ʾ��ģ
			{
				for(j=0;j<8;j++)
				{
					if(sysEnzk[ip].ENCODE[i]&(0x80>>j))
				      		fontbuf[m++] = color;
					else
				      		fontbuf[m++] = bcolor;
				}
			}
			LcdSetArea(x,y,x+7,y+15);
			WriteCommand(0x2C);
			m=0;
			for(i=0;i<16;i++)
			{
				WriteData(fontbuf[m++]);//
				WriteData(fontbuf[m++]);//
				WriteData(fontbuf[m++]);//
				WriteData(fontbuf[m++]);//
				WriteData(fontbuf[m++]);//
				WriteData(fontbuf[m++]);//
				WriteData(fontbuf[m++]);//
				WriteData(fontbuf[m++]);//8
			}
			return;
		}
		ip++;
	}
	while(sysEnzk[ip].code);
}

/**************************************************************************************
�������ܣ���LCD�ϴ�ӡһ������
���룺    code       �ַ�����
          x,y	     Ŀ����ʾλ������
	  color	     ������ɫ
	  bcolor     ������ɫ
�����	  ��
**************************************************************************************/
void LcdPrintHz(u16 code,u16 x,u16 y,u16 color,u16 bcolor)
{
	u8 ip,i,j;
	u16 m,fcode;
	
	ip = 0;
	m = 0;
	do
	{
		if(sysHzzk[ip].code == code)
		{
			for(i=0;i<32;i=i+2)
			{
				fcode = sysHzzk[ip].HZCODE[i]<<8|sysHzzk[ip].HZCODE[i+1];
				for(j=0;j<16;j++)
				{
					if(fcode&(0x8000>>j))
						fontbuf[m++] = color;
					else
						fontbuf[m++] = bcolor;  
				}
			}
			LcdSetArea(x,y,x+15,y+15);
			WriteCommand(0x2C);
			m = 0;
			for(i=0;i<16;i++)
			{
				WriteData(fontbuf[m++]);//
				WriteData(fontbuf[m++]);//
				WriteData(fontbuf[m++]);//
				WriteData(fontbuf[m++]);//
				WriteData(fontbuf[m++]);//
				WriteData(fontbuf[m++]);//
				WriteData(fontbuf[m++]);//
				WriteData(fontbuf[m++]);//8
				WriteData(fontbuf[m++]);//
				WriteData(fontbuf[m++]);//
				WriteData(fontbuf[m++]);//
				WriteData(fontbuf[m++]);//
				WriteData(fontbuf[m++]);//
				WriteData(fontbuf[m++]);//
				WriteData(fontbuf[m++]);//
				WriteData(fontbuf[m++]);//16
			}
			return;
		}
		ip++;
	}while(sysHzzk[ip].code);
}

/**************************************************************************************
��������: 	����Ļ��ʾһ���ַ���
��    ��: 	char* str       �ַ���ָ��
                u16 x,y		Ŀ����ʾλ������
		u16 color	������ɫ
		u16 bcolor	������ɫ
��    ��: 	��
**************************************************************************************/
void LcdPrintStr(char *str,u16 x,u16 y,u16 color,u16 bcolor)
{
	u8 i;
	u16 px;
	
	i = 0;
	px = x;
	while(str[i])
	{
		if(str[i] > 0x7F)//�Ǻ���
		{
			LcdPrintHz((str[i]<<8)|str[i+1],px,y,color,bcolor);
			i += 2;
			px += 16;
		}
		else//�Ǻ���
		{
			LcdPrintEn(str[i],px, y+1, color,bcolor);
			i++;
			px += 8;
		}
	}
}

/**************************************************************************************
��������: ����Ļ����ʽ��ʾһ���ַ���
��    ��: 

��    ��: ��
**************************************************************************************/
void LcdPrintf(u16 x,u16 y,u16 color,u16 bcolor,char *fmt,...)
{
    va_list ap;
    char str[64];

    va_start(ap,fmt);
    vsprintf(str,fmt,ap);
    LcdPrintStr(str,x,y,color,bcolor);
    va_end(ap);
}

/**************************************************************************************
��������: ����Ļ��ʾһ 16bit bmpͼ��
��    ��: const u8* bmpbit 	bmpͼ����������,������Ƿ���FLASH�е�ͼƬ,�뽫constȥ��
          u16 x,y		Ŀ����ʾλ������
	  u16 width,height	ͼ�εĿ��Ⱥ͸߶�
��    ��: ��
**************************************************************************************/
void LcdPrint16bitBmp(const u8* bmpbit,u16 x,u16 y,u16 widht,u16 height)
{
      	u32 size,ip = 0;
      	
	LcdSetArea(x,y,x+widht-1,y+height-1);
	WriteCommand(0x2C);
	
	size = widht*height*2;
      	for(ip = 0; ip < size; ip += 2)
           	WriteData(bmpbit[ip]<<8|bmpbit[ip+1]);
}
//

void LcdPrintASCIIinModel(u16 x, u16 y, u16 width, u16 height, )







