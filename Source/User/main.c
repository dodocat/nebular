  /******************************************************************************
  * @file    DIESEL_2_SENSOR\Source/main.c
  * @author  YJW
  * @version V 1.0.0
  * @date    2010.09.1
  * @brief   
  *******************************************************************************/
#include <string.h>
#include "stm32f10x.h"
//#include "bmp2.c"
#include "debug_STM32.h"
#include "fsmc_sram.h"
#include "hw_config.h"
#include "usb_init.h"
#include "usb_lib.h"
#include "SSD1963.h"
#include "Graphics.h"
#include "SYS_ZK.H"
#include "button.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_spi.h"
//#include "bmp.h"
#include "ot.h"
#include "touch.h"
#include "24LC02.H"
#include "a.h"
//#include "luoyong.h"


/////////////////
void Init_LED(void);
void Init_NVIC(void);	  
void Init_KEY(void);
uint8_t KEY_Scan(void);	
extern void Delay_Ms(uint16_t time);  
extern void Delay_Us(uint16_t time); 


////////////////////

u8 SecCnt = 0;
u8 _SEC_  = 0;
u8 mode;
u8 brightness;//亮度
u8 contrast;//对比度
u8 saturation;//饱和度

u8 inbuf[64];
u8 outbuf[64];

struct BUTTONDEFINE  button1[]=
{
{ 1, 184, 100, 256, 172, 13, 28,1,1,"步骤 A"},
{ 2, 327, 100, 399, 172, 13, 28,1,1,"步骤 B"},

{ 3,  15,  35, 100,  65, 12,  7,1,1,"时间设置"},
{ 4,  15,  77, 100, 107, 12,  7,1,1,"气压设置"},
{ 5,  15, 119, 100, 149, 12,  7,1,1,"温度校正"},
{ 6,  15, 161, 100, 191, 12,  7,1,1,"搅拌速度"},
{ 7,  15, 203, 100, 233, 12,  7,1,1,"点火温度"},

{ 0 },
};
struct BUTTONDEFINE button2[]=
{
	{1, 200, 200, 290, 260, 41, 22, 1, 1, "1"},
	{2, 306, 200, 396, 260, 41, 22, 1, 1, "2"},
	{3, 412, 200, 502, 260, 41, 22, 1, 1, "3"},
	{4, 200, 270, 290, 330, 41, 22, 1, 1, "4"},
	{5, 306, 270, 396, 330, 41, 22, 1, 1, "5"},
	{6, 412, 270, 502, 330, 41, 22, 1, 1, "6"},
	{7, 200, 340, 290, 400, 41, 22, 1, 1, "7"},
	{8, 306, 340, 396, 400, 41, 22, 1, 1, "8"},
	{9, 412, 340, 502, 400, 41, 22, 1, 1, "9"},
	 	
	{10,306, 410, 396, 470, 41, 22, 1, 1, "0"},
	{0},
};

void GPIOInit(void);
void ADC_Init_Ex(void);
void Tim3_ETR_Init(void);
void Tim4_Init(void);

void DelayMs(u32 nMs)
{
	u32 i;
	
	for(; nMs !=0; nMs--)
	{
		i = 10301;
		while(i--);
	}
}

void Usb_SendData(u8 len)
{
	//UserToPMABufferCopyENDP1(USB_TX_DATA,len);
	_SetEPTxCount(ENDP1, len);
	_SetEPTxStatus(ENDP1, EP_TX_VALID);
}

int main(void)
{
/*		///////////////////////////////////////////
		uint8_t Key;
		int i;
		////////////////////////////////////////////////
*/
        brightness = 0x40;//亮度
        contrast   = 0x70;//对比度
        saturation = 0x40;//饱和度
        memset(outbuf,0,64);
        

	SystemInit();
	//GPIOInit();
/*	 
	 //chumo改版加入
	Init_NVIC();				//中断向量表注册函数 	
		Init_LED();					//LED初始化
	Init_KEY();					//按键初始化
		Init_IIC();					//24LC02初始化
	Init_TOUCH();				// 触摸屏配置

	//////////
 */
	UART3_Init(115200);
	PrintStr("=========12345========\r\n");
	
	//ADC_Init_Ex();
	//Tim3_ETR_Init();
	Tim4_Init();
	//CAN1_Init(250);
	
	USB_Init();
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
  
  	/* Configure FSMC Bank1 NOR/SRAM3 */
  	//FSMC_SRAM_Init();
  	//DelayMs(300);
  	//InitSSD1963();
	LcdInit();
  	
	LcdClear(GRAY0);
	DispButton(button2);
	DisplayButtonUp(2,  2,400,25);   	
	LcdPrintStr("版权所有(C)mmmmmmmmmmmmmmmmmmmm",4,4,BLUE,GRAY0);
	DisplayButtonUp(402,2,478,25);
	LcdPrintf(410,4,BLACK,GRAY0,"%02d:%02d:%02d",12,12,12);
	DisplayButtonDownColor(200, 410, 290, 470, RED);
	DisplayButtonDownColor(412, 410, 502, 470, GREEN);
	LcdPrintStr("删除",230,432,WHITE,RED);
	LcdPrintStr("确认",442,432,BLACK,GREEN);
	LcdFillRec(200,100,502,190,DGREEN);
	DisplayButtonUp(190,90,512,475);
	DelayMs(1000);
	LcdFillRec(190,90,512,475,RED);
	DelayMs(1000);
	LcdFillRec(190,90,512,475,GRAY0);
	DispButton(button2);
	DisplayButtonDownColor(200, 410, 290, 470, RED);
	DisplayButtonDownColor(412, 410, 502, 470, GREEN);
	LcdPrintStr("删除",230,432,WHITE,RED);
	LcdPrintStr("确认",442,432,BLACK,GREEN);
	LcdFillRec(200,100,502,190,DGREEN);
	DisplayButtonUp(190,90,512,475);


	LcdPrint16bitBmp(gImage_a, 600, 90, 40, 92);

	//LcdPrint16bitBmp(gImage, 16, 16, 490, 368 );
	while(1)
	{
	LED1=!LED1;
	DelayMs(50);
/*	 ////////////////////////////////////////
		Key=KEY_Scan();
		if(Pen_Point.Key_Sta==Key_Down)//触摸屏被按下
		{
			Pen_Int_Set(0);				//关闭中断
			do
			{
				Convert_Pos();
				Pen_Point.Key_Sta=Key_Up;
				if(Pen_Point.X0>216&&Pen_Point.Y0<48)LcdClear(RED);//清除
				else 
				{
					Draw_Big_Point(Pen_Point.X0,Pen_Point.Y0);//画图	    
					GPIOC->ODR|=1<<5;    //PC5 上拉	   
				}
			}while(PEN==0);//如果PEN一直有效,则一直执行
			Pen_Int_Set(1);//开启中断
		}
		else Delay_Ms(1);
		if(Key==USER)//USER按下,则执行校准程序
		{
			LcdClear(BlackColor);//清屏
		    Touch_Adjust();  //屏幕校准 
			Save_Adjdata();	 
			LcdClear(RED);
		} 
		i++;
		if(i==200)
		{
			i=0;
			LED1=~LED1;
		}
	}
	 ///////////////////////////////////

 */
//                if(_SEC_)
//		{
//			_SEC_ = 0;
			
//			GPIO_WriteBit(GPIOC, GPIO_Pin_6, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_6)));

//                        LcdPrintf(300,460,BLACK,GRAY0,"%03d",i);
                        //i += 10;
//			SetSensors();
			
//			for(u8 i=1; i<=16; i++)
//				Printf("No.%d=%d,",i,GetSensorVoltage(i));
//			PrintStr("\r\n");
//		}
//		UART1_SendByte(0x55);
//		DelayMs(1);
	}
}

void GPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//GPIO_Mode_IPU;
	//开启全部IO时钟信号
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | 
				RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, 
				ENABLE );
	
	//设置所有IO为推挽输出
	//GPIOA->CRL  = 0x33333333;
	//GPIOA->CRH  = 0x33333333;
	//GPIOB->CRL  = 0x33333333;
	//GPIOB->CRH  = 0x33333333;
	//GPIOC->CRL  = 0x33333333;
	//GPIOC->CRH  = 0x33333333;
	//GPIOD->CRL  = 0x33333333;
	//GPIOD->CRH  = 0x33333333;
	//GPIOE->CRL  = 0x33333333;
	//GPIOE->CRH  = 0x33333333;
	
	//PB5~9,PD4~7,PE0~6 (16个电磁阀控制)
	//GPIOA->BSRR = 0x0000FFFF;
	//GPIOB->BSRR = 0x03E0FC1F;
	//GPIOC->BSRR = 0x0000FFFF;
	//GPIOD->BSRR = 0x00F0FF0F;
	//GPIOE->BSRR = 0x007FFF80;
	
	

	//79(PC13)IO2IN,81(PD0)IO3IN,84(PD3)IO4IN配置为上拉输入
	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	//GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_3;
	//GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/**************************************************************************************************************
函数描述 : SysTick初始化,8分频,分频后每计1个数为0.111us
***************************************************************************************************************/
void SysTick_Init(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//计数器时钟源 == AHB(HCLK)/8 = 72MHz/8 = 9MHz
	
	SysTick->LOAD = 0xFFFFFE;				//装载计数器初始值
	SysTick->VAL   =  0;					//重新装载
	SysTick->CTRL &= ~(SYSTICK_TICKINT<<1);			//禁止中断
}

/*********************************************************************************************************
功能 ：设置AD寄存器,采样16路传感器信号
       26(PA3_IN3),25(PA2_IN2),24(PA1_IN1),23(PA0_IN0),18(PC3_IN13),17(PC2_IN12),16(PC1_IN11),15(PC0_IN10),
       29(PA4_IN4),30(PA5_IN5),31(PA6_IN6),32(PA7_IN7),33(PC4_IN14),34(PC5_IN15),35(PB0_IN8) ,36(PB1_IN9)
       以上依次为传感器1~16
**********************************************************************************************************/
void ADC_Init_Ex(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef	ADC_InitStructure;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | 
				      GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | 
				      GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	//ADCLK时钟超过14M会使AD精度显著下降
	//12M采样频率,采样周期为1.5个周期,则用时为(1.5+12.5)*83.3333 = 1.167us
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
  	//开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	//配置ADC1工作模式
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;			//工作在单独模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;				//单通道
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;			//单次
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//无外部触发
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;			//数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 16;				//多通道转换的通道数
	ADC_Init(ADC1, &ADC_InitStructure);
	
	ADC_Cmd(ADC1, ENABLE);				//使能AD
	
	ADC_ResetCalibration(ADC1);			//Enable ADC1 reset calibaration register
	while(ADC_GetResetCalibrationStatus(ADC1));	//Check the end of ADC1 reset calibration register
	
	ADC_StartCalibration(ADC1);			//Start ADC1 calibaration
	while(ADC_GetCalibrationStatus(ADC1));		//Check the end of ADC1 calibration
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//Start ADC1 Software Conversion
	
	ADC1->CR2 |= 0x01;				//唤醒AD
}

/*********************************************************************************************************
功能 ：TIM3初始化,83(PD2),对外部计数,计算出泵流量,每秒1次
**********************************************************************************************************/
void Tim3_ETR_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	//引脚初始化

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
  	
	//Time base configuration
	TIM_TimeBaseStructure.TIM_Period = 65535;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	TIM3->CNT = 0;
	TIM_SelectInputTrigger(TIM3,TIM_TS_ETRF);//选择触发源
	TIM_ETRClockMode2Config(TIM3,TIM_ExtTRGPSC_OFF,TIM_ExtTRGPolarity_NonInverted,0x00);
	
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
}

/*********************************************************************************************************
功能 ：TIM4用做5ms中断
**********************************************************************************************************/
void Tim4_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = 60000;
	TIM_TimeBaseStructure.TIM_Prescaler = 5;	//分频后 72/(5+1) = 12MHz 每计1个数为0.08333us
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	

  	//Enable the TIM6 gloabal Interrupt
  	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
  	
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);	//清除更新标志位
	TIM_ARRPreloadConfig(TIM4, DISABLE);	//预装载寄存器的内容被立即传送到影子寄存器
	
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM4, ENABLE);
}
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		
		SecCnt++;
		if(SecCnt >= 200)
		{
			SecCnt = 0;
			_SEC_ = 1;
			
			//GetPumpFlowRate();
		}
	}
}



/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称: LED_Init
** 功能描述: LED IO引脚配置
** 参数描述：无
** 作  　者: Dream
** 日　  期: 2011年6月20日
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void Init_LED(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;					//定义一个GPIO结构体变量

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);	
															//使能各个端口时钟，重要！！！	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_12; //配置LED端口挂接到6、12、13端口
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   	//通用输出开漏
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   	//配置端口速度为50M
  	GPIO_Init(GPIOD, &GPIO_InitStructure);				   	//将端口GPIOD进行初始化配置

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 ; 			//配置LED端口挂接到13端口
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   		//复用功能输出开漏
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   	//配置端口速度为50M
  	GPIO_Init(GPIOD, &GPIO_InitStructure);				   	//将端口GPIOD进行初始化配置
}
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称: KEY_Init
** 功能描述: 按键 IO引脚配置
** 参数描述：无
** 作  　者: Dream
** 日　  期: 2011年6月20日
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void Init_KEY()
{
	GPIO_InitTypeDef GPIO_InitStructure;					//定义一个GPIO结构体变量
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC |RCC_APB2Periph_GPIOE,ENABLE);	//使能各个端口时钟，重要！！！

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				//配置按键USER挂接到13端口 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	   		//配置成上拉输入
  	GPIO_Init(GPIOC	,&GPIO_InitStructure);				   	//将端口GPIOC进行初始化配置

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2
	| GPIO_Pin_3 | GPIO_Pin_4;								//配置五项摇杆按键挂接到0、1、2、3、4端口 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	   		//配置成上拉输入
  	GPIO_Init(GPIOE,&GPIO_InitStructure);				   	//将端口GPIOE进行初始化配置
}
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称: KEY_Scan
** 功能描述: 按键扫描程序			
** 参数描述：KEY_Status标志位
				返回的值分别对应按键值
** 作  　者: Dream
** 日　  期: 2011年6月20日
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
uint8_t KEY_Scan(void)
{
	static uint8_t KEY_Status =1; //定义一个按键标志位
	if(KEY_Status&&(KEY_USER==0||KEY_S==0||KEY_D==0||KEY_L==0||KEY_R==0||KEY_U==0))//判断是否有按键按下
	{
		Delay_Ms(10);//去抖动 
		KEY_Status =0;
		if(KEY_USER==0) return 1;	  //按键USER按下返回1
		else if(KEY_S==0) return 2;	  //按键JOY-SEN按下返回1
		else if(KEY_D==0) return 3;	  //按键JOY-DOWN按下返回1
		else if(KEY_L==0) return 4;	  //按键JOY-LEFT按下返回1
		else if(KEY_R==0) return 5;	  //按键JOY-RIGHT按下返回1
		else if(KEY_U==0) return 6;	  //按键JOY-DOWN按下返回1
	}else if(KEY_USER==1&&KEY_S==1&&KEY_D==1&&KEY_L==1&&KEY_R==1&&KEY_U==1)KEY_Status=1;
	return 0;						  //无按键按下返回0
}
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称: NVIC_Configuration
** 功能描述: 系统中断配置
** 参数描述：无
** 作  　者: Dream
** 日　  期: 2011年6月20日
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void Init_NVIC(void)
{ 	
	NVIC_InitTypeDef NVIC_InitStructure;

	#ifdef  VECT_TAB_RAM  									//向量表基地址选择

	  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);  			//将0x20000000地址作为向量表基地址(RAM)
	#else  

	  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0); 		//将0x08000000地址作为向量表基地址(FLASH)  
	#endif
	/*  摄像头中断配置 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置优先级组:先占优先级1位,从优先级3位

	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;	// 配置中断线4
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; // 设置占先优先级为2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  // 设置副优先级为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  // 使能中断线4
	NVIC_Init(&NVIC_InitStructure);							  // 初始化中断线4
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称: Delay_Ms_Ms
** 功能描述: 延时1MS (可通过仿真来判断他的准确度)			
** 参数描述：time (ms) 注意time<65535
** 作  　者: Dream
** 日　  期: 2011年6月20日
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void Delay_Ms(uint16_t time)  //延时函数
{ 
	uint16_t i,j;
	for(i=0;i<time;i++)
  		for(j=0;j<10260;j++);
}
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称: Delay_Ms_Us
** 功能描述: 延时1us (可通过仿真来判断他的准确度)
** 参数描述：time (us) 注意time<65535				 
** 作  　者: Dream
** 日　  期: 2011年6月20日
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void Delay_Us(uint16_t time)  //延时函数
{ 
	uint16_t i,j;
	for(i=0;i<time;i++)
  		for(j=0;j<9;j++);
}
