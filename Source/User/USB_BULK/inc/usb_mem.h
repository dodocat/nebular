/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : usb_mem.h
* Author             : MCD Application Team
* Version            : V3.0.1
* Date               : 04/27/2009
* Description        : Utility prototypes functions for memory/PMA transfers
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_MEM_H
#define __USB_MEM_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void UserToPMABufferCopyENDP0(uint8_t *pbUsrBuf, uint16_t wNBytes);
void PMAToUserBufferCopyENDP0(uint8_t *pbUsrBuf, uint16_t wNBytes);
void UserToPMABufferCopyENDP1(uint8_t *pbUsrBuf, uint16_t wNBytes);
void PMAToUserBufferCopyENDP2(uint8_t *pbUsrBuf, uint16_t wNBytes);

/* External variables --------------------------------------------------------*/

#endif  /*__USB_MEM_H*/

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
