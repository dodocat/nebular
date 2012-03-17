#ifndef __TOUCH_H
#define __TOUCH_H	

#include "stm32f10x.h"
/* ����״̬	*/ 
#define Key_Down 0x01
#define Key_Up   0x00 
/* �ʸ˽ṹ�� */
typedef struct 
{
	uint16_t X0;//ԭʼ����
	uint16_t Y0;
	uint16_t X; //����/�ݴ�����
	uint16_t Y;						   	    
	uint8_t  Key_Sta;//�ʵ�״̬			  
	//������У׼����
	float xfac;
	float yfac;
	short xoff;
	short yoff;
}Pen_Holder;

extern Pen_Holder Pen_Point;

/*������оƬ������������	*/   
#define PEN  PCin(5)   	//PC5 INT
#define DOUT PBin(14)   //PB14  MISO
#define TDIN PBout(15)  //PB15  MOSI
#define TCLK PBout(13)  //PB13  SCLK
#define TCS  PBout(0)  	//PB0  CS 
#define F_CS PBout(1)  	//PB0  CS 
#define MP3_CS PBout(5)  	//PB0  CS 
#define TP_CS  PCout(1)  	//PB0  CS 
/* ADS7843/7846/UH7843/7846/XPT2046/TSC2046 ָ� */
#define CMD_RDY 0x90  //0B10010000���ò�ַ�ʽ��X����
#define CMD_RDX	0xD0  //0B11010000���ò�ַ�ʽ��Y����  
#define CH_X 0xd0
#define CH_Y 0x90
#define ADJ_SAVE_ENABLE	    

  
void Init_TOUCH(void);
void Touch_Adjust(void);
void Convert_Pos(void);
void Pen_Int_Set(uint8_t en);
void Touch_Configuration(void);
void ADS_Write_Byte(uint8_t num);
uint16_t ADS_Read_AD(uint8_t CMD);
uint16_t ADS_Read_XY(uint8_t xy);
uint8_t Read_TP_Once(void);
uint8_t Read_ADS2(uint16_t *x,uint16_t *y);
uint8_t Read_ADS(uint16_t *x,uint16_t *y);

void Touch_Initializtion(void);
u16  Touch_GetPhyX(void);
u16  Touch_GetPhyY(void);
u16  Touch_MeasurementX(void);
u16  Touch_MeasurementY(void);


void Draw_Big_Point(uint16_t x,uint16_t y);
void Drow_Touch_Point(uint16_t x,uint16_t y);

void Save_Adjdata(void);
uint8_t Get_Adjdata(void);
uint8_t Is_In_Area(uint16_t x,uint16_t y,uint16_t xlen,uint16_t ylen);

#endif
