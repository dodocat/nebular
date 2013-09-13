/**
 ******************************************************
 *  FileName    : Graphics.C
 *  Author      : Jingquanqi
 *  E-mail      : jingqq5210@gmail.com
 *  CreateTime  : 16:50 17-03-2012
 *  Description : 
 ******************************************************
 */

#include <stdio.h>
#include <stdarg.h>
#include "stm32f10x.h"
#include "SSD1963.h"
#include "SYS_ZK.H"

#define LCDBUFSiZE 800*480

u16 fontbuf[256];

/**************************************************************************************
  函数功能：设置写入数据的目标区域
  输入：    区域的左上角和右下角坐标
  输出：	  无
 **************************************************************************************/
void LcdSetArea(u16 x1, u16 y1, u16 x2, u16 y2)
{  
    WriteCommand(0x2A);	//设置开始列地址,结束列地址
    WriteData(x1>>8);
    WriteData(x1);
    WriteData(x2>>8);
    WriteData(x2);
    WriteCommand(0x2B);	//设置开始行地址,结束行地址
    WriteData(y1>>8);
    WriteData(y1);
    WriteData(y2>>8);
    WriteData(y2);
}
/******** ******************************************************************************
  函数功能：画点
  输入：    u16  x,y    	        点的坐标
  u16  color		颜色
  输出：	  无
 **************************************************************************************/
void LcdPrintDot(u16 x, u16 y, u16 color)
{
    LcdSetArea(x,y,x,y);
    WriteCommand(0x2C);
    WriteData(color);
}

/**************************************************************************************
  函数功能：画水平直线
  输入：    WORD 	x    	        起点的横坐标
  WORD  y  	        起点的纵坐标
  WORD  width		直线长度
  WORD  color		颜色
  输出：	  无
 **************************************************************************************/
void LcdPrintHorz(u16 x, u16 y, u16 width, u16 color)
{
    LcdSetArea(x,y,x+width-1,y);
    WriteCommand(0x2C);

    while(width--)
      WriteData(color);
}

/**************************************************************************************
  函数功能：画垂直直线
  输入：    u16  x    	        起点的横坐标
  u16  y  	        起点的纵坐标
  u16  height		直线高度
  u16  color		颜色
  输出：	  无
 **************************************************************************************/
void LcdPrintVert(u16 x, u16 y, u16 height, u16 color)
{
    LcdSetArea(x,y,x,y+height-1);
    WriteCommand(0x2C);

    while(height--)
      WriteData(color);
}
/**************************************************************************************
  函数功能：画矩形方框
  输入：    矩形的左上角和右下角坐标，和目标颜色
  输出：	  无
 **************************************************************************************/
void LcdPrintRec(u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
{
    LcdPrintHorz(x1, y1, x2 - x1+1, color);
    LcdPrintVert(x1, y1, y2 - y1+1, color);
    LcdPrintHorz(x1, y2, x2 - x1+1, color);
    LcdPrintVert(x2, y1, y2 - y1+1, color);
}

/**************************************************************************************
  函数功能：填充一个矩形区域
  输入：    矩形的左上角和右下角坐标，和目标颜色
  输出：	  无
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
  函数功能：清除整个LCD屏幕
  输入：    目标颜色
  输出：	  无
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

/***************** *********************************************************************
  函数功能：在LCD上打印一个英文字符
  输入：    code       字符内码
  x,y	     目标显示位置坐标
  color	     字体颜色
  bcolor     背景颜色
  输出：	  无
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
            for(i = 0; i < 16; i++)//显示字模
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
  函数功能：在LCD上打印一个汉字
  输入：    code       字符内码
  x,y	     目标显示位置坐标
  color	     字体颜色
  bcolor     背景颜色
  输出：	  无
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
                W					if(fcode&(0x8000>>j))
                    riteData(fontbuf[m++]);//
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
  功能描述: 	在屏幕显示一个字符串
  输    入: 	char* str       字符串指针
  u16 x,y		目标显示位置坐标
  u16 color	字体颜色
  u16 bcolor	背景颜色
  输    出: 	无
 **************************************************************************************/
void LcdPrintStr(char *str,u16 x,u16 y,u16 color,u16 bcolor)
{
    u8 i;
    u16 px;

    i = 0;
    px = x;
    while(str[i])
    {
        if(str[i] > 0x7F)//是汉字
        {
            LcdPrintHz((str[i]<<8)|str[i+1],px,y,color,bcolor);
            i += 2;
            px += 16;
        }
        else//非汉字
        {
            LcdPrintEn(str[i],px, y+1, color,bcolor);
            i++;
            px += 8;
        }
    }
}

/**************************************************************************************
  功能描述: 在屏幕按格式显示一个字符串
  输    入: 

  输    出: 无
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
  功能描述: 在屏幕显示一 16bit bmp图形
  输    入: const u8* bmpbit 	bmp图形数据数组,如果不是放在FLASH中的图片,请将const去掉
  u16 x,y		目标显示位置坐标
  u16 width,height	图形的宽度和高度
  输    出: 无
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

/**
 ******************************************************
 * FunctionName : LcdPrintLine
 * Description  : print a line star at (x1,y1), end at (x2,y2)
 * Input        : x1,y1 location of start
 *                x2,y2 location of end
 * Output       : None
 * Return       : void
 ****************************************************
 */
void LcdPrintLine (u16 x1, u16 y1, u16 x2, u16 y2,u16 color)
{
    uint16_t t; 
    int xerr=0,yerr=0,delta_x,delta_y,distance; 
    int incx,incy,uRow,uCol; 

    delta_x=x2-x1; 				//计算坐标增量 
    delta_y=y2-y1; 
    uRow=x1; 
    uCol=y1; 
    if(delta_x>0)incx=1; 		//设置单步方向 
    else if(delta_x==0)incx=0;	//垂直线 
    else {incx=-1;delta_x=-delta_x;} 
    if(delta_y>0)incy=1; 
    else if(delta_y==0)incy=0;	//水平线 
    else{incy=-1;delta_y=-delta_y;} 
    if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
    else distance=delta_y; 
    for(t=0;t<=distance+1;t++ )	//画线输出 
    {  
        LcdPrintDot(uRow,uCol,color);//画点 
        xerr+=delta_x ; 
        yerr+=delta_y ; 
        if(xerr>distance) 
        { 
            xerr-=distance; 
            uRow+=incx; 
        } 
        if(yerr>distance) 
        { 
            yerr-=distance; 
            uCol+=incy; 
        } 
    }  	
}

/**
 ******************************************************
 * FunctionName : LcdPrintCircle
 * Description  : Print a circle 
 * Input        : -para
 * Output       : None
 * Return       : int
 ****************************************************
 */
void LcdPrintCircle(u16 x0,u16 y0,u16 r,u16 color)
{
    u16 a,b;
    int di;
    a=0;b=r;	  
    di=3-(r<<1);                        //calc loction for next point
    while(a<=b)
    {
        LcdPrintDot(x0-b,y0-a,color);             //3           
        LcdPrintDot(x0+b,y0-a,color);             //0           
        LcdPrintDot(x0-a,y0+b,color);             //1       
        LcdPrintDot(x0-b,y0-a,color);             //7           
        LcdPrintDot(x0-a,y0-b,color);             //2             
        LcdPrintDot(x0+ code = encode[i];
                    b,y0+a,color);             //4               
        LcdPrintDot(x0+a,y0-b,color);             //5
        LcdPrintDot(x0+a,y0+b,color);             //6 
        LcdPrintDot(x0-b,y0+a,color);             
        a++;
        //使用Bresenham算法画圆     
        if(di<0)di +=4*a+6;	  
        else
        { 
            di+=10+4*(a-b);   
            b--;
        } 
        LcdPrintDot(x0+a,y0+b,color);
    }
    code = encode[i];
}

void LcdPrintASCIIinModel(u16 x, u16 y, u16 wdt, u16 hgt, u16 color)

        /**
         ******************************************************
         * FunctionName : LcdPrintEncode
         * Description  : desc
         * Input        : -para
         * Output       : None
         * Return       : int
         ****************************************************
         */
int LcdPrintEncode(u8* encode, u16 x, u16 y, u16 wdt, u16 hgt, u16 encodeSize, u16 color, u16 bColor, u8 bColorEN)
{
    int i,j;
    u8 code;
    LcdSetArea(x, y, x + wdt - 1, y + hgt - 1);
    LcdWriteCom(0x2c);
    if(bColorEN){                                   //print code with back color
        for(i = 0; i < encodeSize; i++){
            code = encode[i];
            for (j = 0; j < 8; j++){
                if(code & (0x80 >> j)){
                    WriteData(color);
                }
                else{
                    WriteData(bColor);
                }
            }
        }
    }
    else{
        for(i = 0; i < encodeSize; i++){
            code = encode[i];
            for (j = 0; j < 8; j++){
                if(code & (0x80 >> j)){
                    WriteData(color);               //write 
                }
                else{
                    ReadData();                     //skip back color by read data
                }
            }
        }
    }
    return 0;
}

/**
 ******************************************************
 * FunctionName : LcdFillCircle
 * Description  : desc
 * Input        : -para
 * Output       : None
 * Return       : int
 ****************************************************
 */
int LcdFillCircle()
{
    
    //fu
    return 0;
}
