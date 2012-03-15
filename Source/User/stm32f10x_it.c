/**
  ******************************************************************************
  * @file    Project/Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.1.0
  * @date    06/19/2009
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "usb_lib.h"
#include "SSD1963.h"
#include "Graphics.h"

extern u8 mode;
extern u8 brightness; //亮度
extern u8 contrast;   //对比度
extern u8 saturation; //饱和度
extern u8 inbuf[64];
extern u8 outbuf[64];

u16 sofnum = 0;
u16 image_w;
u16 image_h;
u16 display_x;
u16 display_y;

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}


__IO uint16_t wIstr;		/* ISTR register last read value */
__IO uint8_t bIntPackSOF = 0;	/* SOFs received between 2 consecutive packets */
uint16_t SaveRState;
uint16_t SaveTState;

/*******************************************************************************
* Function Name  : USB_HP_CAN1_TX_IRQHandler
* Description    : This function handles USB High Priority or CAN TX interrupts
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USB_HP_CAN1_TX_IRQHandler(void)
{
	uint32_t wEPVal = 0;
	
	_SetISTR((uint16_t)CLR_CTR);			// clear CTR flag
	EPindex = (uint8_t)(wIstr & ISTR_EP_ID);	// extract highest priority endpoint number
	wEPVal = _GetENDPOINT(EPindex);			// process related endpoint register
	
	if ((wEPVal & EP_CTR_RX) != 0)
	{
		_ClearEP_CTR_RX(EPindex);	// clear int flag
		
		//(*pEpInt_OUT[EPindex-1])();
	}
	else if ((wEPVal & EP_CTR_TX) != 0)
	{
		_ClearEP_CTR_TX(EPindex);	// clear int flag
		
		//(*pEpInt_IN[EPindex-1])();
	}
}

/*******************************************************************************
* Function Name  : USB_LP_CAN1_RX0_IRQHandler
* Description    : This function handles USB Low Priority or CAN RX0 interrupts
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void WriteBuf(void)
{
  u16 *dp;
  dp = (u16*)outbuf;
  
  WriteData(*dp++);
  WriteData(*dp++);
  WriteData(*dp++);
  WriteData(*dp++);
  WriteData(*dp++);
  WriteData(*dp++);
  WriteData(*dp++);
  WriteData(*dp++);
  WriteData(*dp++);
  WriteData(*dp++);
  WriteData(*dp++);
  WriteData(*dp++);
  WriteData(*dp++);
  WriteData(*dp++);
  WriteData(*dp++);
  WriteData(*dp++);
  WriteData(*dp++);
  WriteData(*dp++);
  WriteData(*dp++);
  WriteData(*dp++);
  WriteData(*dp++);
  WriteData(*dp++);
  WriteData(*dp++);
  WriteData(*dp++);
  WriteData(*dp++);
  WriteData(*dp++);
  WriteData(*dp++);
  WriteData(*dp++);
  WriteData(*dp++);
  WriteData(*dp++);
  WriteData(*dp++);
  WriteData(*dp++);
}
void USB_LP_CAN1_RX0_IRQHandler(void)
{
	//u16 rxlen,ip;
	
	wIstr = _GetISTR();		//中断状态寄存器
	if (wIstr & ISTR_CTR)	//正确的传输
	{
		uint32_t wEPVal = 0;
		
		_SetISTR((uint16_t)CLR_CTR);			// clear CTR flag
		EPindex = (uint8_t)(wIstr & ISTR_EP_ID);	// 获取高优先级的端点号
		
		if (EPindex)	//端点1 or 2
		{
			wEPVal = _GetENDPOINT(EPindex);
			
			if (wEPVal & EP_CTR_TX)	//主机IN
			{
				_ClearEP_CTR_TX(EPindex);	// clear int flag
                                inbuf[0] = mode;
                                inbuf[1] = brightness;
                                inbuf[2] = contrast;
                                inbuf[3] = saturation;
				UserToPMABufferCopyENDP1(inbuf, 64);
				_SetEPTxCount(ENDP1, 64);
				_SetEPTxStatus(ENDP1, EP_TX_VALID);
			}
			else if (wEPVal & EP_CTR_RX)	//主机OUT
			{
				_ClearEP_CTR_RX(EPindex);	// clear int flag
                                
                    		//rxlen = GetEPRxCount(ENDP2);
				PMAToUserBufferCopyENDP2(outbuf, 64);
                                _SetEPRxStatus(ENDP2, EP_RX_VALID);
                                if(!sofnum)
                                {
                                	if(outbuf[0] == 0x01)//亮度调节
                                	{
                                    		brightness = outbuf[1];
                                    		contrast   = outbuf[2];
                                    		saturation = outbuf[3];
                                    		WriteCommand(0xBC);
	                            		WriteData(brightness);
                                    		WriteData(contrast);
                                    		WriteData(saturation);
                                    		WriteData(0x01);
                                	}
                                	else if(outbuf[0] == 0x02)//图形下载定义
                                	{
                                                display_x = outbuf[9]<<8|outbuf[8];
                                                display_y = outbuf[11]<<8|outbuf[10];
                                                image_w   = outbuf[13]<<8|outbuf[12];
                                                image_h   = outbuf[15]<<8|outbuf[14];
                                                sofnum    = outbuf[17]<<8|outbuf[16];
                                                
                                                LcdSetArea(display_x,display_y,display_x+image_w-1,display_y+image_h-1);
	                                        WriteCommand(0x2C);
                                	}
                        	}
                        	else//数据
                        	{
                        	        sofnum--;
                                        //for(ip = 0; ip < 64; ip += 8)
                                        //{
           	                          WriteBuf();  
                                           
                                        //}
                        	}
			}
		}
		else	//控制命令
		{
			SaveRState = _GetEPRxStatus(ENDP0);	// save RX & TX status
			SaveTState = _GetEPTxStatus(ENDP0);
			_SetEPRxStatus(ENDP0, EP_RX_NAK);	// set both to NAK
			_SetEPTxStatus(ENDP0, EP_TX_NAK);
			
			wEPVal = _GetENDPOINT(ENDP0);
			if ((wEPVal & EP_CTR_TX))
			{
				_ClearEP_CTR_TX(ENDP0);
				In0_Process();
			}
			else if ((wEPVal & EP_SETUP))
			{
				_ClearEP_CTR_RX(ENDP0);	// SETUP bit kept frozen while CTR_RX = 1
				Setup0_Process();
				
				// 处理自己定义的控制命令
				// 标准的控制命令为8字节,这里只使用其中的5个字节
				//pInformation->USBbRequest;
				//pInformation->USBwValues.bw.bb0;
				//pInformation->USBwValues.bw.bb1;
				//pInformation->USBwIndexs.bw.bb0;
				//pInformation->USBwIndexs.bw.bb1;
				
				switch(pInformation->USBbRequest)
				{
					//自定义的USB控制命令
					case 0x20:
					break;
					
                                        case 0x21:
					break;
					
					//标准的USB控制命令
					default :
					break;
				}
			}
			else if ((wEPVal & EP_CTR_RX))
			{
				_ClearEP_CTR_RX(ENDP0);
				Out0_Process();
			}
			// before terminate set Tx & Rx status
			_SetEPRxStatus(ENDP0, SaveRState);
			_SetEPTxStatus(ENDP0, SaveTState);
			
			return;
		}
	}
	else if (wIstr & ISTR_RESET)	//复位
	{
		_SetISTR((uint16_t)CLR_RESET);
		Device_Property.Reset();
		sofnum = 0; 	//数据帧计数
	}
}

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
