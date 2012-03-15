/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : usb_desc.c
* Author             : MCD Application Team
* Version            : V3.0.1
* Date               : 04/27/2009
* Description        : Descriptors for Mass Storage Device
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "usb_desc.h"

const uint8_t MASS_DeviceDescriptor[MASS_SIZ_DEVICE_DESC] =
  {
    0x12,   /* �������ĳ��� */
    0x01,   /* ������������: Device */
    0x00,   /* bcdUSB compatible with USB2.0 */
    0x02,   // 0x00,0x01 means compatible with USB1.0
    0x00,   /* USB������豸�����,0x01~0xfeΪ��׼�豸��,0xffΪ�����Զ�������,0��ʾ�����ڽӿ��������ж��� */
    0x00,   /* usb������������,ͬ��,ֵ��USB�涨�ͷ���� */
    0x00,   /* USB������豸Э����� 0��ʾ���ض�Э�� */
    0x40,   /* �����64�ֽ� */
    0x81,   /* idVendor PID_0483 */
    0x19,
    0x12,   /* idProduct VID_5720 */
    0x11,
    0x00,   /* bcdDevice 2.00 ��BCD���ʾ�豸�������,�豸�������*/
    0x02,
    1,      /* ���������ַ��������� */
    2,      /* ������Ʒ�ַ��������� */
    3,      /* �����豸���к��ַ��������� */
    0x01    /* ������ĿΪ1,������ֻ��1������������ */
  };

const uint8_t MASS_ConfigDescriptor[MASS_SIZ_CONFIG_DESC] =
  {
    0x09,   /* ���������� ����Ϊ9�ֽ� */
    0x02,   /* ������������: Configuration */
    MASS_SIZ_CONFIG_DESC,  // ���������ܳ���,2���ֽڱ�ʾ
    0x00,                  // 9+9+7+7=32 �ֽ�
    0x01,   /* bNumInterfaces: 1 interface ������ֻ��1���ӿ������� */
    0x01,   /* bConfigurationValue: �ӿ�����ֵ */
    0x00,   /* iConfiguration: Index of string descriptor describing the configuration */
    0xC0,   /* bmAttributes: bus powered */
    0x32,   /* MaxPower 100 mA */

    /******************** �ӿ������� ********************/
    0x09,   /* bLength: Interface Descriptor size */
    0x04,   /* bDescriptorType: Interface descriptor type */
    0x00,   /* bInterfaceNumber: Number of Interface �ӿ���Ŀ��0Ϊ��ֵ */
    0x00,   /* bAlternateSetting: Alternate setting ��������ֵ */
    0x02,   /* bNumEndpoints �˵���ĿΪ2 */
    0xFF,   /* �ӿ�Ⱥ������: ff=�������ض� */
    0xFF,   /* �ӿڴ�Ⱥ������: ff=�������ض� */
    0xFF,   /* �ӿ�Э��: ff=�������ض� */
    0x04,   /* iInterface: Index of string descriptor 4���ַ��������� */

    /******************** �˵�1������ ********************/
    0x07,   /*Endpoint descriptor length = 7 */
    0x05,   /*Endpoint descriptor type */
    0x81,   /*b.7(1Ϊin,0Ϊout), b.0-b.3Ϊ�˵�� */
    0x02,   /*Bulk endpoint type ����: 0 ���ƣ�1 ��ʱ 2 ���� 3 �ж� */
    0x40,   /*Maximum packet size (64 bytes) */
    0x00,
    0x00,   /*Polling interval in milliseconds ��ѯ���,��msΪ��λ(��֧��BULKģʽ) */

    /******************** �˵�2������ ********************/
    0x07,   /*Endpoint descriptor length = 7 */
    0x05,   /*Endpoint descriptor type */
    0x02,   /*Endpoint address (OUT, address 2) �˵��ַ,bit0Ϊ2��ʾOUT */
    0x02,   /*Bulk endpoint type ����: 0 ���ƣ�1 ��ʱ 2 ���� 3 �ж� */
    0x40,   /*Maximum packet size (64 bytes) */
    0x00,
    0x00     /*Polling interval in milliseconds ��ѯ���,��msΪ��λ(��֧��BULKģʽ) */
  };

// �����ַ��� LangID = 0x0409: U.S. English
const uint8_t MASS_StringLangID[MASS_SIZ_STRING_LANGID] =
  {
    MASS_SIZ_STRING_LANGID,
    0x03,                    /* bDescriptorType = String descriptor */
    0x09,                    // ����
    0x04                     // ������
  };

// ���������ַ���
const uint8_t MASS_StringVendor[MASS_SIZ_STRING_VENDOR] =
  {
    MASS_SIZ_STRING_VENDOR,  /* Size of manufaturer string */
    0x03,                    /* bDescriptorType = String descriptor */
    /* Manufacturer: "FAIR DALIAN CHINA" */
    'F', 0, 'A', 0, 'I', 0, 'R', 0, ' ', 0, 'D', 0, 'A', 0, 'L', 0,
    'I', 0, 'A', 0, 'N', 0, ' ', 0, 'C', 0, 'H', 0, 'I', 0, 'N', 0,
    'A', 0
  };

// ������Ʒ�ַ���
const uint8_t MASS_StringProduct[MASS_SIZ_STRING_PRODUCT] =
  {
    MASS_SIZ_STRING_PRODUCT,
    0x03,                    /* bDescriptorType = String descriptor */

    /* Product name: "Vehicle Capture" */
    'V', 0, 'e', 0, 'h', 0, 'i', 0, 'c', 0, 'l', 0, 'e', 0, ' ', 0, 'C', 0,
    'a', 0, 'p', 0, 't', 0, 'u', 0, 'r', 0, 'e', 0
  };

// �����豸���к��ַ���
uint8_t MASS_StringSerial[MASS_SIZ_STRING_SERIAL] =
  {
    MASS_SIZ_STRING_SERIAL,
    0x03,
    /* Serial number*/
    'Y', 0, 'J', 0, 'W', 0, ' ', 0, '1', 0, '.', 0, '0', 0
  };

const uint8_t MASS_StringInterface[MASS_SIZ_STRING_INTERFACE] =
  {
    MASS_SIZ_STRING_INTERFACE,
    0x03,                    /* bDescriptorType = String descriptor */

    /* Interface 0: "ST Mass" */
    'Y', 0, 'a', 0, 'n', 0, 'g', 0, 'S', 0, 'i', 0, 'r', 0
  };

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
