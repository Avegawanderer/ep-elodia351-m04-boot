/**
 *******************************************************************************
 * @file  24cxx.c
 * @brief This midware file provides firmware functions to 24cxx EEPROM.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2022-03-31       CDT             First version
 @endverbatim
 *******************************************************************************
 * Copyright (C) 2022-2023, Xiaohua Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by XHSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 *
 *******************************************************************************
 */

/*******************************************************************************
 * Include files
 ******************************************************************************/
 #include "app_config.h"
#include "24cxx.h"

/**
 * @addtogroup BSP
 * @{
 */

/**
 * @addtogroup Components
 * @{
 */

/**
 * @defgroup 24CXX EEPROM Driver for 24CXX
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
 #define EE_24CXX_WAIT_TIMEOUT           (0x20000UL)

#define EepromLock()
#define EepromUnlock()
/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static uint32_t u32PageSize;
static uint32_t u32Capacity;


#define FLASH_SECTOR_SIZE 32
/*****************************************************************************
 *   函数名: E2PROMWriteStr
 *   功能描述:  向eeprom写数据函数(无页的限制)
 *   参数定义:  address:写地址；
 *				   e2prom_buf:待些内容指针；
 *				   length:待写数据长度。
 *	返回值:无
 *   创建人：陶林，2011/02/24
******************************************************************************/
void FLASH_INTF_WriteStr(uint16 Addr,uint8 *Buf,uint16 Len)
{
//	uint32 AddrTmp = Addr;
//	uint32 LenTmp = Len;
//	uint8* BufTmp = Buf;
	uint8 CurPageLen;
 	uint8 SectorOver;

	EepromLock();

	while (Len)
	{
		if(Addr+Len>8192)while(1);
		SectorOver = Addr%FLASH_SECTOR_SIZE;

		if ((FLASH_SECTOR_SIZE - SectorOver)>Len)
		{
			CurPageLen = Len;
		}
		else
		{
			CurPageLen = FLASH_SECTOR_SIZE - SectorOver;
		}
		AT24Cxx_INTF_WriterPage(Addr, Buf, CurPageLen);
		Delay_ms(10);

		Addr = Addr + CurPageLen;
		Buf = Buf + CurPageLen;
		Len = Len - CurPageLen;
	}

	EepromUnlock();
}


/*****************************************************************************
 *   函数名: E2PROMReadByte
 *   功能描述: 从eeprom的address地址处读出数据
 *   参数定义: address:待读地址；
 *	返回值:
 *   创建人：陶林，2011/07/24
******************************************************************************/
uint8 E2PROMReadByte(uint16 address)	//向eeprom读单个字节
{
//	uint8 err;
	uint8 Data;

	EepromLock();
	
	Data = AT24Cxx_INTF_ReadStr(address &Data, 1);

	//DRV_FlashRead(address, &Data, 1);

	EepromUnlock();
    
	return Data;

}


void FLASH_INTF_ReadStr(uint16 address,uint8 * e2prom_buf,uint16 length)
{
//	uint8 err;

	EepromLock();

	AT24Cxx_INTF_ReadStr(address, e2prom_buf, length);

//	DRV_FlashRead(address, e2prom_buf, length);

	EepromUnlock();
}



/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
