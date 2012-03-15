#ifndef __OT_H
#define __OT_H	
//fr hardware.h
//IO�ڲ����궨��
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO�ڵ�ַӳ��
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
 
//IO�ڲ���,ֻ�Ե�һ��IO��!
//ȷ��n��ֵС��16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //��� 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //���� 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //��� 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //���� 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //��� 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //���� 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //��� 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //���� 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //��� 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //����
//end hd.h

extern uint16_t TextColor , BlackColor;


//LED���Ŷ���
#define LED1 PDout(6)	   	//�����϶�Ӧ��LD1
#define LED2 PDout(12)	   	//�����϶�Ӧ��LD2
#define LED3 PDout(13)	   	//�����϶�Ӧ��LD3

//�������Ŷ���
#define KEY_USER  PCin(13) 	//�����϶�Ӧ�ڰ���USER
#define KEY_S  PEin(0)  	//�����϶�Ӧ�ڰ���JOY-SEN
#define KEY_D  PEin(1)  	//�����϶�Ӧ�ڰ���JOY-DOWN
#define KEY_L  PEin(2)  	//�����϶�Ӧ�ڰ���JOY-LEFT
#define KEY_R  PEin(3)  	//�����϶�Ӧ�ڰ���JOY-RIGHT
#define KEY_U  PEin(4)  	//�����϶�Ӧ�ڰ���JOY-UP

//����һ��ö��
enum  						
{							
	 USER=1,				//1
	 SEN,					//2
	 DOWN,					//3
	 LEFT,					//4
	 RIGHT,					//5
	 UP						//6
};
struct Data_Time
{
	uint8_t hour;
	uint8_t min;
	uint8_t sec;			
	//������������
	uint16_t w_year;
	uint8_t  w_month;
	uint8_t  w_date;
	uint8_t  week;
};

 //24λ�������16λͼ 
//#define RGB888ToRGB565(uint8_t r,uint8_t g,uint8_t b)  (uint32_t)((r&0xF8)<<8 | (g&0xFC)<<3 | (b&0xF8)>>3)	//565

/***********************************************************************************
������ɫ�궨��
***********************************************************************************/
#define   BLACK        0x0000                    // ��ɫ�� 0, 0, 0              
#define   NAVY         0x000F                    // ����ɫ�� 0, 0, 128          
#define   DGREEN       0x03E0                    // ����ɫ�� 0, 128, 0          
#define   DCYAN        0x03EF                    // ����ɫ�� 0, 128, 128        
#define   MAROON       0x7800                    // ���ɫ��128, 0, 0           
#define   PURPLE       0x780F                    // ��ɫ�� 128, 0, 128          
#define   OLIVE        0x7BE0                    // ����̣�128, 128, 0         
#define   LGRAY        0xC618                    // �Ұ�ɫ��192, 192, 192        
#define   DGRAY        0x7BEF                    // ���ɫ��128, 128, 128        
//#define   BLUE         0x001F                    // ��ɫ�� 0, 0, 255            
#define   GREEN        0x07E0                 	 // ��ɫ�� 0, 255, 0             
#define   CYAN         0x07FF                    // ��ɫ�� 0, 255, 255           
//#define   RED          0xF800                    // ��ɫ�� 255, 0, 0             
#define   MAGENTA      0xF81F                    // Ʒ�죺 255, 0, 255          
//#define   YELLOW       0xFFE0                    // ��ɫ�� 255, 255, 0           
#define   WHITE        0xFFFF                    // ��ɫ�� 255, 255, 255         



#endif

