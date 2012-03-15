

#include "stm32f10x.h"
#include "SSD1963.h"
#include "debug_STM32.h"

unsigned int  HDP=799;
//unsigned int  HT=1000;
//unsigned int  HPS=51;
unsigned int  LPS=46;
unsigned char HPW=DISP_HOR_PULSE_WIDTH;//20;

unsigned int  VDP=479;
//unsigned int  VT=625;
//unsigned int  VPS=22;
unsigned int  FPS=23;
unsigned char VPW=3;

//#define DISP_HOR_RESOLUTION		800
//#define DISP_VER_RESOLUTION		480
//	#define DISP_HOR_PULSE_WIDTH		1
//	#define DISP_HOR_BACK_PORCH		210
//	#define DISP_HOR_FRONT_PORCH		45
////	#define DISP_VER_PULSE_WIDTH		1
//	#define DISP_VER_BACK_PORCH		34	
//	#define DISP_VER_FRONT_PORCH		10
	
	#define HT (DISP_HOR_RESOLUTION+DISP_HOR_PULSE_WIDTH+DISP_HOR_BACK_PORCH+DISP_HOR_FRONT_PORCH)
	#define HPS (DISP_HOR_PULSE_WIDTH+DISP_HOR_BACK_PORCH)
	#define VT (DISP_VER_PULSE_WIDTH+DISP_VER_BACK_PORCH+DISP_VER_FRONT_PORCH+DISP_VER_RESOLUTION)
	#define VPS (DISP_VER_PULSE_WIDTH+DISP_VER_BACK_PORCH)


extern u8 brightness; //亮度
extern u8 contrast;   //对比度
extern u8 saturation; //饱和度
extern void DelayMs(u32 nMs);

u8 s1963gpio;

u8 ReadData(void)
{
  return LCD_DATA_ADD;
}
//-------------------------------------------------------------------------------------------------


/**************************************************************************************
函数功能：初始化SSD1963
输入：    无
输出：	  无
注意：    无
**************************************************************************************/
void InitSSD1963(void)
{
	WriteCommand(0xE2);		//Set PLL with OSC = 10MHz (hardware),	Multiplier N = 35, 250MHz < VCO < 800MHz = OSC*(N+1), VCO = 360MHz
	WriteData(0x23);		//参数1 
	WriteData(0x02);		//参数2 Divider M = 2, PLL = 360/(M+1) = 120MHz
	WriteData(0x54);		//参数3 Validate M and N values
	
	PrintStr("ok: =======23 02 04==========\r\n");
	WriteCommand(0xE3);
	Printf("e30=%02x\r\n",ReadData());
	Printf("e31=%02x\r\n",ReadData());
	Printf("e32=%02x\r\n",ReadData());

 	WriteCommand(0xE0);		// Start PLL command
	WriteData(0x01);		// enable PLL
 	DelayMs(1);
 	
 	WriteCommand(0xE0);		// Start PLL command again
	WriteData(0x03);		// now, use PLL output as system clock	
 	DelayMs(1);
 	
 	PrintStr("ok: ===========04===========\r\n");
 	WriteCommand(0xE4);		// Start PLL command again
	Printf("e4=%02x\r\n",ReadData());// now, use PLL output as system clock	
 	
 	
 	WriteCommand(0x01);		//软复位
 	DelayMs(10);
 	
 	/*****************************************************************
	* 7" TFT model # TY700TFT800480
	*****************************************************************/
	//Set LSHIFT freq, i.e. the DCLK with PLL freq 120MHz set previously
	//Typical DCLK for TY700TFT800480 is 33.3MHz(datasheet), experiment shows 30MHz gives a stable result
	//30MHz = 120MHz*(LCDC_FPR+1)/2^20
	//LCDC_FPR = 262143 (0x3FFFF)
	//Time per line = (DISP_HOR_RESOLUTION+DISP_HOR_PULSE_WIDTH+DISP_HOR_BACK_PORCH+DISP_HOR_FRONT_PORCH)/30 us = 1056/30 = 35.2us
	WriteCommand(0xE6);
	//WriteData(0x03);
	//WriteData(0xff);
	//WriteData(0xff);
	WriteData(0x04);
	WriteData(0x93);
	WriteData(0xE0);
	
	PrintStr("ok: =========03 ff ff============\r\n");
	WriteCommand(0xE7);
	Printf("e70=%02x\r\n",ReadData());
	Printf("e71=%02x\r\n",ReadData());
	Printf("e72=%02x\r\n",ReadData());
	
	//Set LCD mode, varies from individual manufacturer
	WriteCommand(0xB0);
	WriteData(0x10);//0x10				// set 18-bit for 7" panel TY700TFT800480
	WriteData(0x80);				// set TTL mode
	WriteData((DISP_HOR_RESOLUTION-1)>>8); 		// Set LCD panel size(DISP_HOR_RESOLUTION = 800) H
	WriteData(DISP_HOR_RESOLUTION-1);		// Set LCD panel size(DISP_HOR_RESOLUTION = 800) L
	WriteData((DISP_VER_RESOLUTION-1)>>8);		// Set LCD panel size(DISP_VER_RESOLUTION = 480) H
	WriteData(DISP_VER_RESOLUTION-1);		// Set LCD panel size(DISP_VER_RESOLUTION = 480) L
	WriteData(0x00);//0x00				// RGB sequence	
	
	PrintStr("ok: =======10 80 03 1f 01 df 00============\r\n");
	WriteCommand(0xB1);
	Printf("B10=%02x\r\n",ReadData());
	Printf("B11=%02x\r\n",ReadData());
	Printf("B12=%02x\r\n",ReadData());
	Printf("B13=%02x\r\n",ReadData());
	Printf("B14=%02x\r\n",ReadData());
	Printf("B15=%02x\r\n",ReadData());
	Printf("B16=%02x\r\n",ReadData());
	
	//Set horizontal period
	WriteCommand(0xB4);
	WriteData((HT-1)>>8);
	WriteData(HT-1);
	WriteData((HPS-1)>>8);
	WriteData(HPS-1);
	WriteData(DISP_HOR_PULSE_WIDTH-1);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	
	PrintStr("ok======: 04 1f 00 d2 00 00 00 00========\r\n");
	WriteCommand(0xB5);
	Printf("B50=%02x\r\n",ReadData());
	Printf("B51=%02x\r\n",ReadData());
	Printf("B52=%02x\r\n",ReadData());
	Printf("B53=%02x\r\n",ReadData());
	Printf("B54=%02x\r\n",ReadData());
	Printf("B55=%02x\r\n",ReadData());
	Printf("B56=%02x\r\n",ReadData());
	Printf("B57=%02x\r\n",ReadData());
	
	
	
	//Set vertical period
	WriteCommand(0xB6);
	WriteData((VT-1)>>8);
	WriteData(VT-1);
	WriteData((VSP-1)>>8);
	WriteData(VSP-1);
	WriteData(DISP_VER_PULSE_WIDTH-1);
	WriteData(0x00);
	WriteData(0x00);
	
	PrintStr("ok======: 02 0c 00 22 00 00 00========\r\n");
	WriteCommand(0xB7);
	Printf("B70=%02x\r\n",ReadData());
	Printf("B71=%02x\r\n",ReadData());
	Printf("B72=%02x\r\n",ReadData());
	Printf("B73=%02x\r\n",ReadData());
	Printf("B74=%02x\r\n",ReadData());
	Printf("B75=%02x\r\n",ReadData());
	Printf("B76=%02x\r\n",ReadData());
	
	
	WriteCommand(0x3A);	//Set pixel format, i.e. the bpp
	WriteData(0x55); 	//set 16bpp
	
	WriteCommand(0xF0);	//设置SSD1963同CPU接口为16bit Set pixel data interface 
	WriteData(0x03);	//16-bit(565 format) data for 16bpp PIC32MX only
	
	PrintStr("ok======: 03========\r\n");
	WriteCommand(0xF1);
	Printf("F10=%02x\r\n",ReadData());
	
	WriteCommand(0x21);	//进入图形颜色翻转模式
	
        WriteCommand(0xBC);//重要
	WriteData(brightness);//亮度值
        WriteData(contrast);//对比度值
        WriteData(saturation);//饱和度值
        WriteData(0x01);//处理机允许
        
	WriteCommand(0x29);	// Turn on display; show the image on display
	
        //读系统一些状态
        PrintStr("==========get power mode==========\r\n");
	WriteCommand(0x0A);
	Printf("0A0=%02x\r\n",ReadData());
        PrintStr("==========get adder mode==========\r\n");
	WriteCommand(0x0B);
	Printf("0B0=%02x\r\n",ReadData());
        PrintStr("==========Get Pixel Format==========\r\n");
	WriteCommand(0x0C);
	Printf("0B0=%02x\r\n",ReadData());
        PrintStr("==========Get Display mode==========\r\n");
	WriteCommand(0x0D);
	Printf("0B0=%02x\r\n",ReadData());
        PrintStr("==========Get Signal Mode==========\r\n");
	WriteCommand(0x0E);
	Printf("0B0=%02x\r\n",ReadData());
        PrintStr("==========Get Post Proc==========\r\n");
	WriteCommand(0xBD);
	Printf("0B0=%02x\r\n",ReadData());
        Printf("0B0=%02x\r\n",ReadData());
        Printf("0B0=%02x\r\n",ReadData());
        Printf("0B0=%02x\r\n",ReadData());
        
        
        
	WriteCommand(0xB8);	//设置GPIO配置
	WriteData(0x0F);	//GPIO is controlled by host; 4个IO设置成输出
	WriteData(0x01);	//GPIO0 使用正常的IO功能
	
	s1963gpio = Up2Down2Left2Right;//禁止DITHB,及LCD方向设置,关闭背光
	SetS1963GpioOut();	//设置固定的输出值
	
	//OpenEL();		//打开背光，显示图像
}
  
void LcdWriteRam (uint16_t data)
{
	LCD_DATA_ADD = data;
}
void LcdWriteCom (uint16_t com)
{
	LCD_COMM_ADD = com;
}

/*****************************************************************************
** 函数名称: FSMC_GPIO_Configuration
** 功能描述: FSMC引脚配置
** 参数描述：无
** 作  　者: Dream
** 日　  期: 2011年5月15日
*****************************************************************************/
void FSMC_GPIO_Configuration()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO
							,ENABLE);	/* 使能各个端口时钟，重要！！！*/ 	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 |
                                 GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | 
								 GPIO_Pin_11 | GPIO_Pin_14 | GPIO_Pin_15;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_Init(GPIOD, &GPIO_InitStructure);

  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | 
                                GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
                                GPIO_Pin_15;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_Init(GPIOE, &GPIO_InitStructure);
}
/*****************************************************************************
** 函数名称: FSMC_LCD_Init
** 功能描述: FSMC配置使用FSMC的Bank1 NOR/PSRAM
** 参数描述：无
** 作  　者: Dream
** 日　  期: 2011年5月15日
*****************************************************************************/
void FSMC_LCD_Init()
{
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef  p;

  	/* Configure FSMC Bank1 NOR/PSRAM */

  	p.FSMC_AddressSetupTime = 0x02;
  	p.FSMC_AddressHoldTime = 0x00;
  	p.FSMC_DataSetupTime = 0x05;
  	p.FSMC_BusTurnAroundDuration = 0x00;
  	p.FSMC_CLKDivision = 0x00;
  	p.FSMC_DataLatency = 0x00;
  	p.FSMC_AccessMode = FSMC_AccessMode_B;

  	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
  	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
  	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;
  	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
  	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
  	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
  	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
  	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
  	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
  	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
  	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
  	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
  	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
  	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;	  
 
  	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 

  	/* Enable FSMC Bank1_SRAM Bank */
  	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);  
}
/*****************************************************************************
** 函数名称: LCD_Init
** 功能描述: LCD初始化
** 参数描述：无
** 作  　者: Dream
** 日　  期: 2011年5月15日
*****************************************************************************/
void LcdInit(void)
{
	FSMC_GPIO_Configuration();
	FSMC_LCD_Init();


	LcdWriteCom(0x00E2);	//PLL multiplier, set PLL clock to 120M
	LcdWriteRam(0x001D);	    //N=0x36 for 6.5M, 0x23 for 10M crystal
	LcdWriteRam(0x0002);
	LcdWriteRam(0x0004);
	
	LcdWriteCom(0x00E0);  // PLL enable
	LcdWriteRam(0x0001);
	DelayMs(5);
	LcdWriteCom(0x00E0);
	LcdWriteRam(0x0003);
	DelayMs(5);
	LcdWriteCom(0x0001);  // software reset
	DelayMs(5);
	LcdWriteCom(0x00E6);	//PLL setting for PCLK, depends on resolution
	LcdWriteRam(0x0004);	//0x0003
	LcdWriteRam(0x0093);	//0x0033
	LcdWriteRam(0x00e0);	//0x0033

	LcdWriteCom(0x00B0);	//LCD SPECIFICATION
	LcdWriteRam(0x0000);
	LcdWriteRam(0x0000);
	LcdWriteRam((HDP>>8)&0X00FF);  //Set HDP
	LcdWriteRam(HDP&0X00FF);
    LcdWriteRam((VDP>>8)&0X00FF);  //Set VDP
	LcdWriteRam(VDP&0X00FF);
    LcdWriteRam(0x0000);

	LcdWriteCom(0x00B4);	//HSYNC
	LcdWriteRam((HT>>8)&0X00FF);  //Set HT
	LcdWriteRam(HT&0X00FF);
	LcdWriteRam((HPS>>8)&0X00FF);  //Set HPS
	LcdWriteRam(HPS&0X00FF);
	LcdWriteRam(HPW);			   //Set HPW
	LcdWriteRam((LPS>>8)&0X00FF);  //Set HPS
	LcdWriteRam(LPS&0X00FF);
	LcdWriteRam(0x0000);

	LcdWriteCom(0x00B6);	//VSYNC
	LcdWriteRam((VT>>8)&0X00FF);   //Set VT
	LcdWriteRam(VT&0X00FF);
	LcdWriteRam((VPS>>8)&0X00FF);  //Set VPS
	LcdWriteRam(VPS&0X00FF);
	LcdWriteRam(VPW);			   //Set VPW
	LcdWriteRam((FPS>>8)&0X00FF);  //Set FPS
	LcdWriteRam(FPS&0X00FF);

	LcdWriteCom(0x00BA);
	LcdWriteRam(0x0005);    //GPIO[3:0] out 1

	LcdWriteCom(0x00B8);
	LcdWriteRam(0x0007);    //GPIO3=input, GPIO[2:0]=output
	LcdWriteRam(0x0001);    //GPIO0 normal

	LcdWriteCom(0x0036); //rotation
	LcdWriteRam(0x0000);
//	LcdWriteRam(0x0060);
	//============================================

	//============================================
//	LcdWriteCom(0x003A); //rotation
//	LcdWriteRam(0x0050);

//		LcdWriteCom(0x0026); //rotation
//	LcdWriteRam(0x0003);
//		LcdWriteCom(0x0030); //rotation
//	LcdWriteRam(0x0000);

	//=============================================
	LcdWriteCom(0x00F0); //pixel data interface
	LcdWriteRam(0x0003);

	DelayMs(5);

	//LcdClear(BLUE);
	
	LcdWriteCom(0x0026); //display on
	LcdWriteRam(0x0001);

	LcdWriteCom(0x0029); //display on

/*	LcdWriteCom(0x00BE); //set PWM for B/L
	LcdWriteRam(0x0006);
	//LcdWriteRam(0x0008);
	LcdWriteRam(0x0080);
	//LcdWriteRam(0x00f0);	 
	
	LcdWriteRam(0x0001);
	LcdWriteRam(0x00f0);
	LcdWriteRam(0x0000);
	LcdWriteRam(0x0000);  */

	LcdWriteCom(0x00d0);//设置动态背光控制配置 
	LcdWriteRam(0x000d);	

	LcdWriteCom(0x0029); //display on	 
}



