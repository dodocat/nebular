
#ifndef __SSD1963_H__
#define __SSD1963_H__

#include "stm32f10x.h"


//-----------------------------------------------------------------------------------------------
#define __AT070TN92__

#ifdef	__AT070TN92__
	// ssd1963 d0-d15 -------- STM32F103VCT6 D0-D15
	// ssd1963 #d/c   -------- STM32F103VCT6 A16
	// ssd1963 #cs    -------- STM32F103VCT6 A17
	// ssd1963 #wr    -------- STM32F103VCT6 WR
	// ssd1963 #rd    -------- STM32F103VCT6 RD
	
	#define LCD_COMM_ADD		*((u16 *)0X60000000)
	#define LCD_DATA_ADD		*((u16 *)0X60020000)
	
	#define WriteCommand(cmd)	{LCD_COMM_ADD = cmd;}
	#define WriteData(data)		{LCD_DATA_ADD = data;}
	//#define ReadData()		{return LCD_DATA_ADD;}
	
	#define OpenEL()		{s1963gpio = 0x08|s1963gpio; LCD_COMM_ADD = 0xBA; LCD_DATA_ADD = s1963gpio;}
	#define CloseEL()		{s1963gpio = 0x07&s1963gpio; LCD_COMM_ADD = 0xBA; LCD_DATA_ADD = s1963gpio;}
	#define EnableDITHB()		{s1963gpio = 0x0B&s1963gpio; LCD_COMM_ADD = 0xBA; LCD_DATA_ADD = s1963gpio;}
	#define DisableDITHB()		{s1963gpio = 0x04|s1963gpio; LCD_COMM_ADD = 0xBA; LCD_DATA_ADD = s1963gpio;}
	#define SetS1963GpioOut()	{		 	     LCD_COMM_ADD = 0xBA; LCD_DATA_ADD = s1963gpio;}
	
	#define DISP_HOR_RESOLUTION		800
	#define DISP_VER_RESOLUTION		480
	#define DISP_HOR_PULSE_WIDTH		1
	#define DISP_HOR_BACK_PORCH		210
	#define DISP_HOR_FRONT_PORCH		45
	#define DISP_VER_PULSE_WIDTH		1
	#define DISP_VER_BACK_PORCH		34	
	#define DISP_VER_FRONT_PORCH		10
	
//	#define HT (DISP_HOR_RESOLUTION+DISP_HOR_PULSE_WIDTH+DISP_HOR_BACK_PORCH+DISP_HOR_FRONT_PORCH)
//	#define HPS (DISP_HOR_PULSE_WIDTH+DISP_HOR_BACK_PORCH)
//	#define VT (DISP_VER_PULSE_WIDTH+DISP_VER_BACK_PORCH+DISP_VER_FRONT_PORCH+DISP_VER_RESOLUTION)
	#define VSP (DISP_VER_PULSE_WIDTH+DISP_VER_BACK_PORCH)
	
	#define Up2Down2Left2Right  	0x01
	#define Down2Up2Right2Left	0x02
	#define Up2Down2Right2Left	0x00
	#define Down2Up2Left2Right	0x03
#endif

extern void InitSSD1963(void);
extern void LcdInit(void);
#endif

