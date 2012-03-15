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
    0x12,   /* 本描述的长度 */
    0x01,   /* 本描述的类型: Device */
    0x00,   /* bcdUSB compatible with USB2.0 */
    0x02,   // 0x00,0x01 means compatible with USB1.0
    0x00,   /* USB分配的设备类代码,0x01~0xfe为标准设备类,0xff为厂商自定义类型,0表示类型在接口描述符中定义 */
    0x00,   /* usb分配的子类代码,同上,值由USB规定和分配的 */
    0x00,   /* USB分配的设备协议代码 0表示无特定协议 */
    0x40,   /* 最大封包64字节 */
    0x81,   /* idVendor PID_0483 */
    0x19,
    0x12,   /* idProduct VID_5720 */
    0x11,
    0x00,   /* bcdDevice 2.00 以BCD码表示设备发行序号,设备出厂编号*/
    0x02,
    1,      /* 描述厂商字符串的索引 */
    2,      /* 描述产品字符串的索引 */
    3,      /* 描述设备序列号字符串的索引 */
    0x01    /* 配置数目为1,接下来只有1个配置描述符 */
  };

const uint8_t MASS_ConfigDescriptor[MASS_SIZ_CONFIG_DESC] =
  {
    0x09,   /* 配置描述符 长度为9字节 */
    0x02,   /* 本描述的类型: Configuration */
    MASS_SIZ_CONFIG_DESC,  // 本描述的总长度,2个字节表示
    0x00,                  // 9+9+7+7=32 字节
    0x01,   /* bNumInterfaces: 1 interface 接下来只有1个接口描述符 */
    0x01,   /* bConfigurationValue: 接口配置值 */
    0x00,   /* iConfiguration: Index of string descriptor describing the configuration */
    0xC0,   /* bmAttributes: bus powered */
    0x32,   /* MaxPower 100 mA */

    /******************** 接口描述符 ********************/
    0x09,   /* bLength: Interface Descriptor size */
    0x04,   /* bDescriptorType: Interface descriptor type */
    0x00,   /* bInterfaceNumber: Number of Interface 接口数目以0为基值 */
    0x00,   /* bAlternateSetting: Alternate setting 交互设置值 */
    0x02,   /* bNumEndpoints 端点数目为2 */
    0xFF,   /* 接口群组类型: ff=制造商特定 */
    0xFF,   /* 接口次群组类型: ff=制造商特定 */
    0xFF,   /* 接口协议: ff=制造商特定 */
    0x04,   /* iInterface: Index of string descriptor 4个字符串描述符 */

    /******************** 端点1描述符 ********************/
    0x07,   /*Endpoint descriptor length = 7 */
    0x05,   /*Endpoint descriptor type */
    0x81,   /*b.7(1为in,0为out), b.0-b.3为端点号 */
    0x02,   /*Bulk endpoint type 类型: 0 控制；1 等时 2 批量 3 中断 */
    0x40,   /*Maximum packet size (64 bytes) */
    0x00,
    0x00,   /*Polling interval in milliseconds 查询间隔,以ms为单位(不支持BULK模式) */

    /******************** 端点2描述符 ********************/
    0x07,   /*Endpoint descriptor length = 7 */
    0x05,   /*Endpoint descriptor type */
    0x02,   /*Endpoint address (OUT, address 2) 端点地址,bit0为2表示OUT */
    0x02,   /*Bulk endpoint type 类型: 0 控制；1 等时 2 批量 3 中断 */
    0x40,   /*Maximum packet size (64 bytes) */
    0x00,
    0x00     /*Polling interval in milliseconds 查询间隔,以ms为单位(不支持BULK模式) */
  };

// 语言字符串 LangID = 0x0409: U.S. English
const uint8_t MASS_StringLangID[MASS_SIZ_STRING_LANGID] =
  {
    MASS_SIZ_STRING_LANGID,
    0x03,                    /* bDescriptorType = String descriptor */
    0x09,                    // 语言
    0x04                     // 次语言
  };

// 描述厂商字符串
const uint8_t MASS_StringVendor[MASS_SIZ_STRING_VENDOR] =
  {
    MASS_SIZ_STRING_VENDOR,  /* Size of manufaturer string */
    0x03,                    /* bDescriptorType = String descriptor */
    /* Manufacturer: "FAIR DALIAN CHINA" */
    'F', 0, 'A', 0, 'I', 0, 'R', 0, ' ', 0, 'D', 0, 'A', 0, 'L', 0,
    'I', 0, 'A', 0, 'N', 0, ' ', 0, 'C', 0, 'H', 0, 'I', 0, 'N', 0,
    'A', 0
  };

// 描述产品字符串
const uint8_t MASS_StringProduct[MASS_SIZ_STRING_PRODUCT] =
  {
    MASS_SIZ_STRING_PRODUCT,
    0x03,                    /* bDescriptorType = String descriptor */

    /* Product name: "Vehicle Capture" */
    'V', 0, 'e', 0, 'h', 0, 'i', 0, 'c', 0, 'l', 0, 'e', 0, ' ', 0, 'C', 0,
    'a', 0, 'p', 0, 't', 0, 'u', 0, 'r', 0, 'e', 0
  };

// 描述设备序列号字符串
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
