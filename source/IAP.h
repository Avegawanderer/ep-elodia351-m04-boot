/**
  ******************************************************************************
  * @file    STM32F4xx_IAP/inc/menu.h 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    10-October-2011
  * @brief   This file provides all the headers of the menu functions.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __IAP_H
#define __IAP_H

/******************************************************************************/
/*                     Memory Base Address                                    */
/******************************************************************************/
#define EFM_BASE                             (0x00000000UL)   /*!< EFM base address in the alias region */
#define SRAM_BASE                            (0x1FFF8000UL)   /*!< SRAM base address in the alias region */
#define QSPI_BASE                            (0x98000000UL)   /*!< QSPI base address in the alias region */

/* SRAM definitions */
#define SRAM_SIZE                       (0x02F000UL)

/**
 * @brief Function pointer type to void/void function
 */
typedef void (*func_ptr_t)(void);


/* Boot configuration */
#define IAP_BOOT_SIZE                               (0x8000UL)
/* APP configuration */
#define IAP_APP_ADDR                                (EFM_BASE + IAP_BOOT_SIZE)
#define IAP_APP_SIZE                                (FLASH_SIZE - IAP_BOOT_SIZE)

#define APP_EXIST_FLAG_ADDR                         (EFM_BASE + IAP_BOOT_SIZE - 4U)
#define APP_UPGRADE_FLAG_ADDR                       (EFM_BASE + IAP_BOOT_SIZE - 8U)
#define APP_EXIST_FLAG                              (0x67890123UL)
#define APP_UPGRADE_FLAG                            (0xA5B6C7D8UL)


#define Phead		0		//包头
#define Pcmd 		1		//数据包中的命令
#define Popmode		2		//操作方式
#define Popack		3		//设置回答指令
#define Pcksum		4		//校验和位置
#define Plen		6		//数据段长度
#define Pdata		8		//数据段起始

#define Pqu			8		//数据序号
#define Ipdata		9		//IP  数据的起始位置

#define DH_COMM_PAGE_4K		8192	
#define PACKET_PC_SIZE		2048		 

/* Communication timeout */
#define IAP_COM_WAIT_TIME               (2000UL)


#define UART_DATAMODE_HEAD  Phead
#define UART_DATAMODE_MODE  1
#define UART_DATAMODE_LENL  Plen
#define UART_DATAMODE_LENH  7

#define UART_RECV_HEAD_LEN  19
#define UART_RECV_PWD_LEN   6

	/* 类型定义 */
#define UART_MODE_PLLSET    0x20
#define UART_TUNER_TEST	    0x21
#define UART_MODE_UIIMG     0x88
#define UART_RX_FIRST_I    8	   /* UART 接收数据头的大小 */

#define PCINFERR_OK         0		/* ok */
#define PCINFERR_TIMEOUT    1		/* 等待超时 */
#define PCINFERR_CHECKERR   2	    /* 校验出错 */
#define PCINFERR_FORMATERR  3		/* 格式错误 */

#define LL_OK                           (0)   /*!< No error */
#define LL_ERR                          (-1)  /*!< Non-specific error code */
#define LL_ERR_UNINIT                   (-2)  /*!< Module (or part of it) was not initialized properly */
#define LL_ERR_INVD_PARAM               (-3)  /*!< Provided parameter is not valid */
#define LL_ERR_INVD_MD                  (-4)  /*!< Operation not allowed in current mode */
#define LL_ERR_NOT_RDY                  (-5)  /*!< A requested final state is not reached */
#define LL_ERR_BUSY                     (-6)  /*!< A conflicting or requested operation is still in progress */
#define LL_ERR_ADDR_ALIGN               (-7)  /*!< Address alignment does not match */
#define LL_ERR_TIMEOUT                  (-8)  /*!< Time Out error occurred (e.g. I2C arbitration lost, Flash time-out, etc.) */
#define LL_ERR_BUF_EMPTY                (-9)  /*!< Circular buffer can not be read because the buffer is empty */
#define LL_ERR_BUF_FULL                 (-10) /*!< Circular buffer can not be written because the buffer is full */

typedef unsigned char  uint8;                   // 无符号8位整型变量  
typedef signed   char  int8;                    // 有符号8位整型变量  
typedef unsigned short uint16;                  // 无符号16位整型变量 
typedef signed   short int16;                   // 有符号16位整型变量 
typedef unsigned int   uint32;                  // 无符号32位整型变量 
typedef signed   int   int32;                   // 有符号32位整型变量 
typedef float          fp32;                    // 单精度浮点数（32位长度） 
typedef double         fp64;                    // 双精度浮点数（64位长度） 

/*----------------------------------------------*
 * The common data type, will be used in the whole project.*
 *----------------------------------------------*/

typedef unsigned char           DH_U8;
typedef unsigned short          DH_U16;
typedef unsigned int            DH_U32;

typedef signed char             DH_S8;
typedef short                   DH_S16;
typedef int                     DH_S32;

#ifndef _M_IX86
    typedef unsigned long long  DH_U64;
    typedef long long           DH_S64;
#else
    typedef __int64             DH_U64;
    typedef __int64             DH_S64;
#endif

typedef char                    DH_CHAR;
typedef void                    DH_VOID;


typedef float                   DH_F32;
typedef double                  DH_F64;


extern const int g_Menu1Image[];/*菜单第一层图标地址索引*/
extern const int g_MenuNumImage[16];/*数字图标的地址索引*/
extern const int g_LinkManImage[];/*联系人层图标地址索引*/
extern const int g_MessageImage[];/*短信息层图标地址索引*/
extern const int g_CalllogImage[];/*通话记录层图标地址索引*/
extern const int g_ScanImage[];/*扫描层图标地址索引*/
extern const int g_LocalSetImage[];/*对讲机设置层图标地址索引*/
extern const int g_ParameterSetImage[];/*参数设置层图标地址索引*/
extern const int g_ToneSetImage[];/*提示音设置层图标地址索引*/
extern const int g_AppdixImage[];/*附件层图标地址索引*/
extern const int g_LinkManAttrImage[];/*联系人属性的图标的地址索引*/

/*菜单图标的尺寸大小*/
#define	NUMBER_ICO_HIGH		18
#define	NUMBER_ICO_WIDE		24
#define	NUMBER_ICO_SIZE		864/*菜单图标的字节数18X24X2=864*/

#define NumberIcoBaseADDR		MenuNumberIcoimageBaseADDR/*菜单里面的数字图标*/

#define MenuNumber_1			(NumberIcoBaseADDR+NUMBER_ICO_SIZE*0)
#define MenuNumber_2			(NumberIcoBaseADDR+NUMBER_ICO_SIZE*1)
#define MenuNumber_3			(NumberIcoBaseADDR+NUMBER_ICO_SIZE*2)
#define MenuNumber_4			(NumberIcoBaseADDR+NUMBER_ICO_SIZE*3)
#define MenuNumber_5			(NumberIcoBaseADDR+NUMBER_ICO_SIZE*4)
#define MenuNumber_6			(NumberIcoBaseADDR+NUMBER_ICO_SIZE*5)
#define MenuNumber_7			(NumberIcoBaseADDR+NUMBER_ICO_SIZE*6)
#define MenuNumber_8			(NumberIcoBaseADDR+NUMBER_ICO_SIZE*7)
#define MenuNumber_9			(NumberIcoBaseADDR+NUMBER_ICO_SIZE*8)
#define MenuNumber_10			(NumberIcoBaseADDR+NUMBER_ICO_SIZE*9)
#define MenuNumber_11			(NumberIcoBaseADDR+NUMBER_ICO_SIZE*10)
#define MenuNumber_12			(NumberIcoBaseADDR+NUMBER_ICO_SIZE*11)
#define MenuNumber_13			(NumberIcoBaseADDR+NUMBER_ICO_SIZE*12)
#define MenuNumber_14			(NumberIcoBaseADDR+NUMBER_ICO_SIZE*13)
#define MenuNumber_15			(NumberIcoBaseADDR+NUMBER_ICO_SIZE*14)
#define MenuNumber_16			(NumberIcoBaseADDR+NUMBER_ICO_SIZE*15)

#define LinkManAttr_GroupCall	(LinkManAttrIcoimageBaseADDR+NUMBER_ICO_SIZE*0)/*个呼图标地址*/
#define LinkManAttr_PrivateCall	(LinkManAttrIcoimageBaseADDR+NUMBER_ICO_SIZE*1)/*组呼图标地址*/
#define LinkManAttr_AllCall		(LinkManAttrIcoimageBaseADDR+NUMBER_ICO_SIZE*2)/*全呼图标地址*/
#define LinkManAttr_NoAddrCall	(LinkManAttrIcoimageBaseADDR+NUMBER_ICO_SIZE*3)/*无地址呼图标地址*/
#define LinkManAttr_RawData	(LinkManAttrIcoimageBaseADDR+NUMBER_ICO_SIZE*4)/*RawData*/
#define LinkManAttr_DefineData	(LinkManAttrIcoimageBaseADDR+NUMBER_ICO_SIZE*5)/*DefineData*/
#define LinkManAttr_SPDATA		(LinkManAttrIcoimageBaseADDR+NUMBER_ICO_SIZE*6)/*SPDATA*/


/* Includes ------------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Main_Menu(void);
int32  FLASH_RecNWrite (DH_U8 stat);

#endif  /* __MENU_H */

/*******************(C)COPYRIGHT 2011 STMicroelectronics *****END OF FILE******/
