#ifndef __IAP_H
#define __IAP_H

/******************************************************************************/
/*                     Memory Base Address                                    */
/******************************************************************************/
#define EFM_BASE  (0x00000000UL) /*!< EFM base address in the alias region */
#define SRAM_BASE (0x1FFF8000UL) /*!< SRAM base address in the alias region */
#define QSPI_BASE (0x98000000UL) /*!< QSPI base address in the alias region */

/* SRAM definitions */
#define SRAM_SIZE (0x02F000UL)

/**
 * @brief Function pointer type to void/void function
 */
typedef void (*func_ptr_t)(void);

/* Boot configuration */
#define IAP_BOOT_SIZE         (0x8000UL)
/* APP configuration */
#define IAP_APP_ADDR          (EFM_BASE + IAP_BOOT_SIZE)
#define IAP_APP_SIZE          (FLASH_SIZE - IAP_BOOT_SIZE)

#define APP_EXIST_FLAG_ADDR   (EFM_BASE + IAP_BOOT_SIZE - 4U)
#define APP_UPGRADE_FLAG_ADDR (EFM_BASE + IAP_BOOT_SIZE - 8U)
#define APP_EXIST_FLAG        (0x67890123UL)
#define APP_UPGRADE_FLAG      (0xA5B6C7D8UL)

#define Phead                 0
#define Pcmd                  1
#define Popmode               2
#define Popack                3
#define Pcksum                4
#define Plen                  6
#define Pdata                 8

#define Pqu                   8
#define Ipdata                9

#define DH_COMM_PAGE_4K       8192
#define PACKET_PC_SIZE        2048

/* Communication timeout */
#define IAP_COM_WAIT_TIME     (2000UL)

#define UART_DATAMODE_HEAD    Phead
#define UART_DATAMODE_MODE    1
#define UART_DATAMODE_LENL    Plen
#define UART_DATAMODE_LENH    7

#define UART_RECV_HEAD_LEN    19
#define UART_RECV_PWD_LEN     6

#define UART_MODE_PLLSET      0x20
#define UART_TUNER_TEST       0x21
#define UART_MODE_UIIMG       0x88
#define UART_RX_FIRST_I       8

#define PCINFERR_OK           0 /* ok */
#define PCINFERR_TIMEOUT      1
#define PCINFERR_CHECKERR     2
#define PCINFERR_FORMATERR    3

#define LL_OK                 (0)   /*!< No error */
#define LL_ERR                (-1)  /*!< Non-specific error code */
#define LL_ERR_UNINIT         (-2)  /*!< Module (or part of it) was not initialized properly */
#define LL_ERR_INVD_PARAM     (-3)  /*!< Provided parameter is not valid */
#define LL_ERR_INVD_MD        (-4)  /*!< Operation not allowed in current mode */
#define LL_ERR_NOT_RDY        (-5)  /*!< A requested final state is not reached */
#define LL_ERR_BUSY           (-6)  /*!< A conflicting or requested operation is still in progress */
#define LL_ERR_ADDR_ALIGN     (-7)  /*!< Address alignment does not match */
#define LL_ERR_TIMEOUT        (-8)  /*!< Time Out error occurred (e.g. I2C arbitration lost, Flash time-out, etc.) */
#define LL_ERR_BUF_EMPTY      (-9)  /*!< Circular buffer can not be read because the buffer is empty */
#define LL_ERR_BUF_FULL       (-10) /*!< Circular buffer can not be written because the buffer is full */

typedef unsigned char uint8;
typedef signed char int8;
typedef unsigned short uint16;
typedef signed short int16;
typedef unsigned int uint32;
typedef signed int int32;
typedef float fp32;
typedef double fp64;

/*----------------------------------------------*
 * The common data type, will be used in the whole project.*
 *----------------------------------------------*/

typedef unsigned char DH_U8;
typedef unsigned short DH_U16;
typedef unsigned int DH_U32;

typedef signed char DH_S8;
typedef short DH_S16;
typedef int DH_S32;

#ifndef _M_IX86
typedef unsigned long long DH_U64;
typedef long long DH_S64;
#else
typedef __int64 DH_U64;
typedef __int64 DH_S64;
#endif

typedef char DH_CHAR;
typedef void DH_VOID;

typedef float DH_F32;
typedef double DH_F64;

extern const int g_Menu1Image[];
extern const int g_MenuNumImage[16];
extern const int g_LinkManImage[];
extern const int g_MessageImage[];
extern const int g_CalllogImage[];
extern const int g_ScanImage[];
extern const int g_LocalSetImage[];
extern const int g_ParameterSetImage[];
extern const int g_ToneSetImage[];
extern const int g_AppdixImage[];
extern const int g_LinkManAttrImage[];

#define NUMBER_ICO_HIGH         18
#define NUMBER_ICO_WIDE         24
#define NUMBER_ICO_SIZE         864

#define NumberIcoBaseADDR       MenuNumberIcoimageBaseADDR

#define MenuNumber_1            (NumberIcoBaseADDR + NUMBER_ICO_SIZE * 0)
#define MenuNumber_2            (NumberIcoBaseADDR + NUMBER_ICO_SIZE * 1)
#define MenuNumber_3            (NumberIcoBaseADDR + NUMBER_ICO_SIZE * 2)
#define MenuNumber_4            (NumberIcoBaseADDR + NUMBER_ICO_SIZE * 3)
#define MenuNumber_5            (NumberIcoBaseADDR + NUMBER_ICO_SIZE * 4)
#define MenuNumber_6            (NumberIcoBaseADDR + NUMBER_ICO_SIZE * 5)
#define MenuNumber_7            (NumberIcoBaseADDR + NUMBER_ICO_SIZE * 6)
#define MenuNumber_8            (NumberIcoBaseADDR + NUMBER_ICO_SIZE * 7)
#define MenuNumber_9            (NumberIcoBaseADDR + NUMBER_ICO_SIZE * 8)
#define MenuNumber_10           (NumberIcoBaseADDR + NUMBER_ICO_SIZE * 9)
#define MenuNumber_11           (NumberIcoBaseADDR + NUMBER_ICO_SIZE * 10)
#define MenuNumber_12           (NumberIcoBaseADDR + NUMBER_ICO_SIZE * 11)
#define MenuNumber_13           (NumberIcoBaseADDR + NUMBER_ICO_SIZE * 12)
#define MenuNumber_14           (NumberIcoBaseADDR + NUMBER_ICO_SIZE * 13)
#define MenuNumber_15           (NumberIcoBaseADDR + NUMBER_ICO_SIZE * 14)
#define MenuNumber_16           (NumberIcoBaseADDR + NUMBER_ICO_SIZE * 15)

#define LinkManAttr_GroupCall   (LinkManAttrIcoimageBaseADDR + NUMBER_ICO_SIZE * 0)
#define LinkManAttr_PrivateCall (LinkManAttrIcoimageBaseADDR + NUMBER_ICO_SIZE * 1)
#define LinkManAttr_AllCall     (LinkManAttrIcoimageBaseADDR + NUMBER_ICO_SIZE * 2)
#define LinkManAttr_NoAddrCall  (LinkManAttrIcoimageBaseADDR + NUMBER_ICO_SIZE * 3)
#define LinkManAttr_RawData     (LinkManAttrIcoimageBaseADDR + NUMBER_ICO_SIZE * 4) /*RawData*/
#define LinkManAttr_DefineData  (LinkManAttrIcoimageBaseADDR + NUMBER_ICO_SIZE * 5) /*DefineData*/
#define LinkManAttr_SPDATA      (LinkManAttrIcoimageBaseADDR + NUMBER_ICO_SIZE * 6) /*SPDATA*/

/* Includes ------------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
DH_U8 BLL_IAP_Flash(DH_U8 *key);
int32_t FLASH_INTF_EraseSectors(uint32_t u32Addr, uint32_t u32Size);
int32_t FLASH_WriteData(uint32_t u32Addr, uint8_t *pu8Buff, uint32_t u32Len);
int32_t IAP_JumpToApp(uint32_t u32Addr);
void Main_Menu(void);
int32 FLASH_RecNWrite(DH_U8 stat);

#endif /* __MENU_H */

/*******************(C)COPYRIGHT 2011 STMicroelectronics *****END OF FILE******/
