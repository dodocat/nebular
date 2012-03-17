/**************************************************************
** 	启航号开发板
**	功能介绍： 触摸屏驱动代码
**  版本：V1.0  
**	论坛：www.openmcu.com
**	淘宝：http://shop36995246.taobao.com/   
**  技术支持群：121939788 
***************************************************************/ 
/* Includes ------------------------------------------------------------------*/
//#include "usually.h"
//#include "LCD_Drive.h"
#include "Touch.h"
#include "24LC02.h"
#include "graphics.h"
#include "stdlib.h"
#include "math.h"
#include "ssd1963.h"

#include "common.h"

u16 TextColor;
u16 BlackColor;

Pen_Holder Pen_Point;	// 定义笔实体 
void SPI_CS(u8 a)
{
  // PD6 -> TS_nCS
  if (a)
    GPIO_SetBits(GPIOD,GPIO_Pin_6);
  else
    GPIO_ResetBits(GPIOD,GPIO_Pin_6);
}

void SPI_DIN(u8 a)
{
  // PD7 -> TS_DIN
  if (a)
    GPIO_SetBits(GPIOD,GPIO_Pin_7);
  else
    GPIO_ResetBits(GPIOD,GPIO_Pin_7);
}

void SPI_CLK(u8 a)
{
  // PD5 -> TS_CLK
  if (a)
    GPIO_SetBits(GPIOD,GPIO_Pin_5);
  else
    GPIO_ResetBits(GPIOD,GPIO_Pin_5);
}

u8 SPI_DOUT(void)
{
  // PD10 -> TS_DOUT
  return GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_10);
}

void SPI_delay(u16 i)
{
  u16 k;
  for (k=0;k<i;k++);
}

void Touch_Start(void)
{
	SPI_CLK(0);
	SPI_CS(1);
	SPI_DIN(1);
	SPI_CLK(1);
	SPI_CS(0);
}

void Touch_Write(u8 d)
{
	u8 buf, i ;
	
  SPI_CLK(0);
	for( i = 0; i < 8; i++ )
	{
    buf = (d >> (7-i)) & 0x1 ;
    SPI_DIN(buf);
    SPI_CLK(0);
    SPI_CLK(1);
    SPI_CLK(0);
	}
}

u16  Touch_Read(void)
{
	u16 buf ;
	u8 i ;

	buf=0;
	for( i = 0; i < 12; i++ )
	{
		buf = buf << 1 ;
		SPI_CLK(1);
		SPI_CLK(0);			
		if ( SPI_DOUT() )	
		{
			buf = buf + 1 ;
		}
	}
	return( buf ) ;
}

u8  Touch_Busy(void)
{
  // PD8 -> TS_BUSY
  return GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_8);
}

u8  Touch_PenIRQ(void)
{
  // PD9 -> TS_nPENIRQ
  return GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_9);
}

void Touch_Initializtion()
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /*****************************
  **    硬件连接说明          **
  ** STM32         TSC2046    **
  ** PD3    <----> nPENIRQ    ** i
  ** PD4    <----> BUSY       ** i
  ** PC6    <----> DCLK       ** o
  ** PC5    <----> DIN        ** o
  ** PC4    <----> DOUT       ** i
  ** PC7    <----> nCS        ** o
  ******************************/
// PD5 -> TS_CLK  PD6 -> TS_nCS  PD7 -> TS_DIN  PD8 -> TS_BUSY  PD9 -> TS_nPENIRQ  PD10 -> TS_DOUT

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_8 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
}

u16 _AD2X(int adx)
{
  u16 sx=0;
  int r = adx - 280;
  r *= 239;
  sx=r / (3740 - 280);
  if (sx<=0 || sx>240)
    return 0;
  return sx;
}

u16 _AD2Y(int ady)
{
  u16 sy=0;
  int r = ady - 230;
  r *= 319;
  sy=r/(3720 - 230);
  if (sy<=0 || sy>320)
    return 0;
  return sy;
}

u16  Touch_MeasurementX(void)
{
  u8 i;
  u16 p=0;
  for (i=0;i<8;i++)
  {
    p+=Touch_GetPhyX();
    SPI_delay(1000);
  }
  p>>=3;
  
  return ( p );
}

u16  Touch_MeasurementY(void)
{
  u8 i;
  u16 p=0;
  for (i=0;i<8;i++)
  {
    p+=Touch_GetPhyY();
    SPI_delay(1000);
  }
  p>>=3;
  
  return ( p );
}

u16  Touch_GetPhyX(void)
{
  if (Touch_PenIRQ()) return 0;
  Touch_Start();
  Touch_Write(0x00);
  Touch_Write(CH_X);
  while(!Touch_Busy());
  return (Touch_Read());
}

u16  Touch_GetPhyY(void)
{
  if (Touch_PenIRQ()) return 0;
  Touch_Start();
  Touch_Write(0x00);
  Touch_Write(CH_Y);
  while(!Touch_Busy());
  return (Touch_Read());
}
/*
u16 Dx(u16 xx)
{
  return (_AD2X(xx));
}
u16 Dy(u16 yy)
{
  return (_AD2Y(yy));
}
*/

//SPI写数据
//向7843写入1byte数据   
void ADS_Write_Byte(uint8_t num)    
{  
	uint8_t count=0;   
	for(count=0;count<8;count++)  
	{ 	  
		if(num&0x80)TDIN=1;  
		else TDIN=0;   
		num<<=1;   
		TCLK=0;//上升沿有效
		TCLK=1; 
	} 			    
} 		 
//SPI读数据 
//从7846/7843/XPT2046/UH7843/UH7846读取adc值	   
uint16_t ADS_Read_AD(uint8_t CMD)	  
{ 	 
	uint8_t i;
	uint8_t count=0; 	  
	uint16_t Num=0; 
	TCLK=0;//先拉低时钟 	 
	TCS=0; //选中ADS7843	 
	ADS_Write_Byte(CMD);//发送命令字
	for(i=200;i>0;i--);
	TCLK=1;//给1个时钟，清除BUSY   	    
	TCLK=0; 	 
	for(count=0;count<16;count++)  
	{ 				  
		Num<<=1; 
		TCLK=0;//下降沿有效  
		TCLK=1;
		if(DOUT)Num++; 		 
	}  	
	Num>>=4;   //只有高12位有效.
	TCS=1;//释放ADS7843	 
	return(Num);   
}
//读取一个坐标值
//连续读取READ_TIMES次数据,对这些数据升序排列,
//然后去掉最低和最高LOST_VAL个数,取平均值 
#define READ_TIMES 15 //读取次数
#define LOST_VAL 5	  //丢弃值
uint16_t ADS_Read_XY(uint8_t xy)
{
	uint16_t i, j;
	uint16_t buf[READ_TIMES];
	uint16_t sum=0;
	uint16_t temp;
	for(i=0;i<READ_TIMES;i++)
	{				 
		buf[i]=ADS_Read_AD(xy);	    
	}				    
	for(i=0;i<READ_TIMES-1; i++)//排序
	{
		for(j=i+1;j<READ_TIMES;j++)
		{
			if(buf[i]>buf[j])//升序排列
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}	  
	sum=0;
	for(i=LOST_VAL;i<READ_TIMES-LOST_VAL;i++)sum+=buf[i];
	temp=sum/(READ_TIMES-2*LOST_VAL);
	return temp;   
} 
//带滤波的坐标读取
//最小值不能少于100.
uint8_t Read_ADS(uint16_t *x,uint16_t *y)
{
	uint16_t xtemp,ytemp;			 	 		  
	xtemp=ADS_Read_XY(CMD_RDX);
	ytemp=ADS_Read_XY(CMD_RDY);	  												   
	if(xtemp<100||ytemp<100)return 0;//读数失败
	*x=xtemp;
	*y=ytemp;
	return 1;//读数成功
}
//2次读取ADS7846,连续读取2次有效的AD值,且这两次的偏差不能超过
//50,满足条件,则认为读数正确,否则读数错误.	   
//该函数能大大提高准确度
#define ERR_RANGE 50 //误差范围 
uint8_t Read_ADS2(uint16_t *x,uint16_t *y) 
{
	uint16_t x1,y1;
 	uint16_t x2,y2;
 	uint8_t flag;    
    flag=Read_ADS(&x1,&y1);   
    if(flag==0)return(0);
    flag=Read_ADS(&x2,&y2);	   
    if(flag==0)return(0);   
    if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE))//前后两次采样在+-50内
    &&((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))
    {
        *x=(x1+x2)/2;
        *y=(y1+y2)/2;
        return 1;
    }else return 0;	  
} 
//读取一次坐标值	
//仅仅读取一次,知道PEN松开才返回!					   
uint8_t Read_TP_Once(void)
{
	uint8_t t=0;	    
	Pen_Int_Set(0);//关闭中断
	Pen_Point.Key_Sta=Key_Up;
	Read_ADS2(&Pen_Point.X,&Pen_Point.Y);
	while(PEN==0&&t<=250)
	{
		t++;
		DelayMs(10);
	};
	Pen_Int_Set(1);//开启中断		 
	if(t>=250)return 0;//按下2.5s 认为无效
	else return 1;	
}

//////////////////////////////////////////////////
//与LCD部分有关的函数  
//画一个触摸点
//用来校准用的
void Drow_Touch_Point(uint16_t x,uint16_t y)
{
	u16 colorT = BLUE;
	LcdPrintHorz(x-12,y,25,colorT);//横线
	LcdPrintVert(x,y-12,25,colorT);//竖线
	LcdPrintDot(x+1,y+1,colorT);
	LcdPrintDot(x-1,y+1,colorT);
	LcdPrintDot(x+1,y-1,colorT);
	LcdPrintDot(x-1,y-1,colorT);
	LcdPrintCircle(x,y,6,colorT);//画中心圈
	LcdPrintDot(x,y,colorT);
}	  
//画一个大点
//2*2的点			   
void Draw_Big_Point(uint16_t x,uint16_t y)
{	
	u16 colorT = BLUE;    
	LcdPrintDot(x,y,colorT);//中心点 
	LcdPrintDot(x+1,y,colorT);
	LcdPrintDot(x,y+1,colorT);
	LcdPrintDot(x+1,y+1,colorT);
	LcdFillRec(x,y,x+5,y+5,colorT);	 	  	
}
//////////////////////////////////////////////////

//转换结果
//根据触摸屏的校准参数来决定转换后的结果,保存在X0,Y0中
void Convert_Pos(void)
{		 	  
	if(Read_ADS2(&Pen_Point.X,&Pen_Point.Y))
	{
		Pen_Point.X0=Pen_Point.xfac*Pen_Point.X+Pen_Point.xoff;
		Pen_Point.Y0=Pen_Point.yfac*Pen_Point.Y+Pen_Point.yoff;  
	}
}
//触摸屏校准代码
//得到四个校准参数
void Touch_Adjust(void)
{								 
	uint16_t pos_temp[4][2];//坐标缓存值
	uint8_t  cnt=0;	
	uint16_t d1,d2;
	uint32_t tem1,tem2;
	float fac; 	   
	cnt=0;				
	TextColor=PURPLE;
	BlackColor =BLACK;
	Drow_Touch_Point(20,20);//画点1 
	Pen_Point.Key_Sta=Key_Up;//消除触发信号 
	Pen_Point.xfac=0;//xfac用来标记是否校准过,所以校准之前必须清掉!以免错误	

	while(1)
	{
		if(Pen_Point.Key_Sta==Key_Down)//按键按下了
		{
			if(Read_TP_Once())//得到单次按键值
			{  								   
				pos_temp[cnt][0]=Pen_Point.X;
				pos_temp[cnt][1]=Pen_Point.Y;
				cnt++;
			}			 
			switch(cnt)
			{			   
				case 1:
					LcdClear(BlackColor);//清屏 
					Drow_Touch_Point(780,20);//画点2
					break;
				case 2:
					LcdClear(BlackColor);//清屏 
					Drow_Touch_Point(20,460);//画点3
					break;
				case 3:
					LcdClear(BlackColor);//清屏 
					Drow_Touch_Point(780,460);//画点4
					break;
				case 4:	 	//全部四个点已经得到
	    		    		//对边相等
					tem1=abs(pos_temp[0][0]-pos_temp[1][0]);//x1-x2
					tem2=abs(pos_temp[0][1]-pos_temp[1][1]);//y1-y2
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrt(tem1+tem2);//得到1,2的距离
					
					tem1=abs(pos_temp[2][0]-pos_temp[3][0]);//x3-x4
					tem2=abs(pos_temp[2][1]-pos_temp[3][1]);//y3-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrt(tem1+tem2);//得到3,4的距离
					fac=(float)d1/d2;
					if(fac<0.95||fac>1.05||d1==0||d2==0)//不合格
					{
						cnt=0;
						LcdClear(BlackColor);//清屏 
						Drow_Touch_Point(20,20);
						continue;
					} 
					tem1=abs(pos_temp[0][0]-pos_temp[2][0]);//x1-x3
					tem2=abs(pos_temp[0][1]-pos_temp[2][1]);//y1-y3
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrt(tem1+tem2);//得到1,3的距离
					
					tem1=abs(pos_temp[1][0]-pos_temp[3][0]);//x2-x4
					tem2=abs(pos_temp[1][1]-pos_temp[3][1]);//y2-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrt(tem1+tem2);//得到2,4的距离
					fac=(float)d1/d2;
					if(fac<0.95||fac>1.05)//不合格
					{
						cnt=0;
						LcdClear(BlackColor);//清屏 
						Drow_Touch_Point(20,20);
						continue;
					}//正确了
								   
					//对角线相等
					tem1=abs(pos_temp[1][0]-pos_temp[2][0]);//x1-x3
					tem2=abs(pos_temp[1][1]-pos_temp[2][1]);//y1-y3
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrt(tem1+tem2);//得到1,4的距离
	
					tem1=abs(pos_temp[0][0]-pos_temp[3][0]);//x2-x4
					tem2=abs(pos_temp[0][1]-pos_temp[3][1]);//y2-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrt(tem1+tem2);//得到2,3的距离
					fac=(float)d1/d2;
					if(fac<0.95||fac>1.05)//不合格
					{
						cnt=0;
						LcdClear(BlackColor);//清屏 
						Drow_Touch_Point(20,20);
						continue;
					}//正确了
					 

					//计算结果
					/*
					Pen_Point.xfac=(float)200/(pos_temp[1][0]-pos_temp[0][0]);//得到xfac		 
					Pen_Point.xoff=(240-Pen_Point.xfac*(pos_temp[1][0]+pos_temp[0][0]))/2;//得到xoff
						  
					Pen_Point.yfac=(float)280/(pos_temp[2][1]-pos_temp[0][1]);//得到yfac
					Pen_Point.yoff=(320-Pen_Point.yfac*(pos_temp[2][1]+pos_temp[0][1]))/2;//得到yoff  	*/

					Pen_Point.xfac=(float)760/(pos_temp[1][0]-pos_temp[0][0]);//得到xfac		 
					Pen_Point.xoff=(800-Pen_Point.xfac*(pos_temp[1][0]+pos_temp[0][0]))/2;//得到xoff
						  
					Pen_Point.yfac=(float)440/(pos_temp[2][1]-pos_temp[0][1]);//得到yfac
					Pen_Point.yoff=(480-Pen_Point.yfac*(pos_temp[2][1]+pos_temp[0][1]))/2;//得到yoff  	

					TextColor=DGREEN;
					LcdClear(BLACK);//清屏
					LcdPrintStr("Touch Screen Adjust OK!",35,110,WHITE,BLACK);//校正完成
					DelayMs(1200);
					LcdClear(BLACK);//清屏   
					return;//校正完成				 
			}
		}
	} 
}
void Init_TOUCH()
{
	EXTI_InitTypeDef EXTI_InitStructure;

	Touch_Configuration();

 	Read_ADS(&Pen_Point.X,&Pen_Point.Y);//第一次读取初始化			 

  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource5);

  	EXTI_InitStructure.EXTI_Line = EXTI_Line5;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);

	#ifdef ADJ_SAVE_ENABLE	  
	if(Get_Adjdata())return;//已经校准
	else			   //未校准?
	{ 										    
		LcdClear(BLACK);//清屏
	    Touch_Adjust();  //屏幕校准 
		Save_Adjdata();	 
	}			
	#else	
	LcdClear(WHITE);//清屏
    Touch_Adjust();  //屏幕校准,带自动保存			   
#endif				   
}
void Touch_Configuration()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE );  //重要！！

	//下面是SPI相关GPIO初始化  	Configure PB0 pin: TP_CS pin 
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_5 | GPIO_Pin_13 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //通用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //通用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	//下面是SPI相关GPIO初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  	//上拉输入
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	//Configure PC4 pin: TP_INT pin 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 	//上拉输入
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	TP_CS = MP3_CS = F_CS = TCS	= 1; 				//禁止SPI2上的其他外设
}
//PEN中断设置	 
void Pen_Int_Set(uint8_t en)
{
	if(en)EXTI->IMR|=1<<5;   //开启line5上的中断	  	
	else EXTI->IMR&=~(1<<5); //关闭line5上的中断	   
}
//中断,检测到PEN脚的一个下降沿.
//置位Pen_Point.Key_Sta为按下状态
//中断线4线上的中断检测
void EXTI9_5_IRQHandler()
{
  if(EXTI_GetITStatus(EXTI_Line5) != RESET)
	{
		Pen_Point.Key_Sta=Key_Down;//按键按下  		  				 

    	EXTI_ClearITPendingBit(EXTI_Line5);
	}
}
#ifdef ADJ_SAVE_ENABLE
#define SAVE_ADDR_BASE 40
/*****************************************************************************
** 函数名称: Save_Adjdata
** 功能描述: 保存校准参数到EEPROAM中的 地址40
				此部分涉及到使用外部EEPROM,如果没有外部EEPROM,屏蔽此部分即可
				保存在EEPROM里面的地址区间基址,占用13个字节(RANGE:SAVE_ADDR_BASE~SAVE_ADDR_BASE+12)
** 作  　者: Dream
** 日　  期: 2010年12月06日
*****************************************************************************/
void Save_Adjdata(void)
{
	s32 temp;			 
	//保存校正结果!		   							  
	temp=Pen_Point.xfac*100000000;//保存x校正因素      
    AT24CXX_WriteLenByte(SAVE_ADDR_BASE,temp,4);   
	temp=Pen_Point.yfac*100000000;//保存y校正因素    
    AT24CXX_WriteLenByte(SAVE_ADDR_BASE+4,temp,4);
	//保存x偏移量
    AT24CXX_WriteLenByte(SAVE_ADDR_BASE+8,Pen_Point.xoff,2);		    
	//保存y偏移量
	AT24CXX_WriteLenByte(SAVE_ADDR_BASE+10,Pen_Point.yoff,2);	
			     							 
	temp=AT24CXX_ReadOneByte(SAVE_ADDR_BASE+12);
//	temp&=0XF0;
	temp =0Xaa;//标记校准过了
	AT24CXX_WriteOneByte(SAVE_ADDR_BASE+12,temp);			 
}
/*****************************************************************************
** 函数名称: Get_Adjdata
** 功能描述: 得到保存在EEPROM里面的校准值
				返回值：1，成功获取数据
						 0，获取失败，要重新校准
** 作  　者: Dream
** 日　  期: 2010年12月06日
*****************************************************************************/       
uint8_t Get_Adjdata(void)
{	
	s32 tempfac;
	tempfac=AT24CXX_ReadOneByte(52);//第五十二字节的第四位用来标记是否校准过！ 		 
	if((tempfac)==0xaa)//触摸屏已经校准过了			   
	{    												 
		tempfac=AT24CXX_ReadLenByte(40,4);		   
		Pen_Point.xfac=(float)tempfac/100000000;//得到x校准参数
		tempfac=AT24CXX_ReadLenByte(44,4);			          
		Pen_Point.yfac=(float)tempfac/100000000;//得到y校准参数
	    //得到x偏移量
		tempfac=AT24CXX_ReadLenByte(48,2);			   	  
		Pen_Point.xoff=tempfac;					 
	    //得到y偏移量
		tempfac=AT24CXX_ReadLenByte(50,2);				 	  
		Pen_Point.yoff=tempfac;					 
		return 1;	 
	}
	return 0;
}
#endif
