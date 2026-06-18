/******************************************************************************  
 Copyright (C), 2014-2020, ZheJiang Dahua Technology CO.,LTD. 
 ******************************************************************************  
 File Name     : dh_drv_adc.c  
 Version       : Initial Draft  
 Author        : yu_feng1<yu_feng1@dahuatech.com>  
 Created       : 2014.3.17  
 Last Modified :  
 Description   : define the Mcu of AD code  
 Function List :  
 DHstory       :  
 1.Date        : 2014/3/17    
 Author      : 24538   
 Modification: Create
 ******************************************************************************/

#include "app_config.h"

#include "APP_CSBK.H"
#include "APP_Record.h"

#include "APP_MENU.h"
#include "APP_MENU_FONT.h"
#include "APP_MENU_TMRHND.h"
#include "APP_MENU_MENUSAVE.h"
#include "APP_MENU_DISPLAY.H"
#include "APP_MENU_FONT_ENUM.h"
#include "APP_MENU_MESSAGE.H"

#include "BLL_PC.h"
#include "BLL_CLK.h"
#include "BLL_LCD.h"
#include "BLL_PCIAP.h"
#include "BLL_SLEEP.h"
#include "BLL_PCINFMSET.h"
#include "BLL_PCTUNERSET.h"
#include "BLL_PCTUNER_FREQ.h"

#include "DRV_UART.h"
#include "DRV_GPIO.h"

#include "CCL_LCD_DISPLAY.h"

#include "INTF_PCLINK.h"
#include "INTF_FLASH.h"
#include "INTF_KEYS.h"
#include "INTF_GPS.h"

#include "HRC_OS.h"

#include "SysCfgMac.h"

#include "sys_inc.h"
#include "version.h"

#define FlashBuf2Lock() 
#define FlashBuf2UnLock() 


#define FUNC_RWALL_EN   1 //是否支持除写频数据以外的读写操作
#define FUNC_PRO_IAP_EN 1 //是否支持在程序中更新IAP


#define XIEPINADDR (40960)

DH_U32 BLL_IAP_Write(DH_VOID);
DH_VOID BLL_PCIAP_PCSetDataTime(DH_VOID);

enum
{
	FLASH_CMD_ERASE = 1,
	FLASH_CMD_WRITE,
	FLASH_CMD_READ,
	FLASH_CMD_READRCD
};

extern const DH_CHAR fwver[];
const struct VERSION_Struct
{
	char FW_ver[18]; //加两个空格符是为了上位机不会丢掉W或者R
	char FR_ver[18]; //加两个空格符是为了上位机不会丢掉W或者R
//	char m[12];
	char date[18];
	char time[10];
} VERSION ={
	"  Write_2M_",
	"  Read_2M_",
//	""
	__DATE__,
	__TIME__,
};

DH_U32 FlashAddr = 0;
DH_U32 Write_ReadLong = 0;

const char* const devinfo[]=
{	
" ",
"+DR-6R",////1
};
const char* const devfreq[]=
{	
" ",
"+136-174MHZ+",
"+240-250MHZ+",
"+350-400MHZ+",
"+400-480MHZ+",
"+400-470MHZ+",
"+430-480MHZ+",
"+410-440MHZ+",
"+430-450MHZ+",
"+136-480MHZ+",
"+150-170MHZ+",
"+140-174MHZ+",
"+450-520MHZ+",
"+30-32MHZ+",
"+66-68MHZ+",
"+66-88MHZ+",
"+200-300MHZ+",
};

extern DH_U8 g_SecFlashType;
extern DH_U32 g_NandFlashBlock_Size;
extern const int * g_ImageIndex[];
extern OS_SEM Sem_MenuTopLock;


DH_U32 BLL_IAP_RcdRead(DH_VOID);


DH_BOOL CheckPassWord(DH_U8 RWFlag, DH_U8* recbuf) //0=读1=写
{
	//19是因为数据帧头第20个字节开始就是密码.
	DH_U8 i = 0;

	if (FLASH_CMD_WRITE == RWFlag)
	{
		if (MENU_APP_GetMenuSetStat(BARTYPESETFLG_Write_PWD) == WRITESWITCH_ON)
		{
			for (i = 0; i < UART_RECV_PWD_LEN; i++)
			{
				if (StrMenuSet.Write_Password[i]
						!= recbuf[UART_RECV_HEAD_LEN + i])
				{

					LCD_CCL_DispMsgBox(Mes_wrong,"writepwd err");
					PCLINK_INTF_TransmitFrame("pwd",3);
					PCLINK_INTF_TransmitFrame(StrMenuSet.Write_Password,6);
					return FALSE;
				}
			}
		}
		return TRUE;
	}
	else if (FLASH_CMD_READ == RWFlag)
	{
		if (MENU_APP_GetMenuSetStat(BARTYPESETFLG_Read_PWD) == READSWITCH_ON)
		{
			for (i = 0; i < UART_RECV_PWD_LEN; i++)
			{
				if (StrMenuSet.Read_Password[i]
				 != recbuf[UART_RECV_HEAD_LEN + UART_RECV_PWD_LEN + i])
				{
					LCD_CCL_DispMsgBox(Mes_wrong,"Readpwd err");
					PCLINK_INTF_TransmitFrame("pwd",3);
					PCLINK_INTF_TransmitFrame(StrMenuSet.Read_Password,6);
					return FALSE;
				}
			}
		}
		return TRUE;
	}
	return TRUE;
}

DH_VOID GetFlashAddrNPkg(DH_U8 *key)
{
	DH_U8 i = 0;
	FlashAddr = 0;/*防止多次升级而叠加在一起*/
	Write_ReadLong = 0;
	for (i = 11; i < 15; i++)
	{
		FlashAddr += key[i];
		if (i != 14)
			FlashAddr <<= 8;
	}
	for (; i < 19; i++)
	{
		Write_ReadLong += key[i];
		if (i != 18)
			Write_ReadLong <<= 8;
	}
	//Write_ReadLong ++;
}

DH_VOID MoveValue(DH_U32 CheckSum, DH_U8* CheckSumToPc)
{
	CheckSumToPc[0] = CheckSum >> 24;
	CheckSumToPc[1] = CheckSum >> 16;
	CheckSumToPc[2] = CheckSum >> 8;
	CheckSumToPc[3] = CheckSum;
}

DH_U8 Option_Command(DH_U8 *key)
{
	DH_U8 i = 0;
	DH_U8 str_Erase[6] =
	{ 0 };
	DH_U8 str_RWrite[12] =
	{ 0 };
	for (i = 0; i < 5; i++)
	{
		str_Erase[i] = (DH_U8) key[i];
		str_RWrite[i] = (DH_U8) key[i];
	}
	for (; i < 11; i++)
	{
		str_RWrite[i] = (DH_U8) key[i];
	}
	if (strcmp((char*)str_Erase, "Erase") == 0)
	{
		return FLASH_CMD_ERASE;
	}
	else if (strcmp((char*)str_RWrite, "Flash Write") == 0)
	{
		return FLASH_CMD_WRITE;
	}
	else if (strcmp((char*)str_RWrite, "Flash Read ") == 0)
	{
		return FLASH_CMD_READ;
	}
	else if (strcmp((char*)str_RWrite, "Record Read") == 0)
	{
		return FLASH_CMD_READRCD;
	}
	return 0;
}

DH_VOID BLL_IAP_SendDeviveInf(DH_U8 opt)
{
#if 0
	DH_U8 buf[13] = {'+','1','3','6','-','1','7','4','M','H','Z','+','\0'};
	DH_U8 i;
	buf[1] = FREQ_SCOPE[1];
	buf[2] = FREQ_SCOPE[2];
	buf[3] = FREQ_SCOPE[3];
	buf[4] = FREQ_SCOPE[4];
	buf[5] = FREQ_SCOPE[5];
	buf[6] = FREQ_SCOPE[6];
	buf[7] = FREQ_SCOPE[7];

	if (opt == FLASH_CMD_WRITE)
		PCLINK_INTF_TransmitFrame(VERSION.FW_ver, sizeof(VERSION.FW_ver));
	else if (opt == FLASH_CMD_READ) //printk(VERSION.FR_ver);
		PCLINK_INTF_TransmitFrame(VERSION.FR_ver, sizeof(VERSION.FR_ver));
	PCLINK_INTF_TransmitFrame(VERSION.m, sizeof(VERSION.m));
	PCLINK_INTF_TransmitFrame(VERSION.date, sizeof(VERSION.date));
	PCLINK_INTF_TransmitFrame(VERSION.time, sizeof(VERSION.time));

	for(i=0;i<strlen((char *)DEV_NAME);i++)
	{
		buf[i+1] = DEV_NAME[i];
	}
	buf[i+1] = '\0';
	PCLINK_INTF_TransmitFrame((DH_U8 *) buf,strlen((char *) buf));
/*	PCLINK_INTF_TransmitFrame((DH_U8 *) devinfo[DEV_TYPE_SEL],
			strlen((char *) devinfo[DEV_TYPE_SEL]));
	PCLINK_INTF_TransmitFrame((DH_U8 *) devfreq[FREQ_SCOPE_SET],
			strlen((char *) devfreq[FREQ_SCOPE_SET]));*/
	buf[1] = FREQ_SCOPE[1];
	buf[2] = FREQ_SCOPE[2];
	buf[3] = FREQ_SCOPE[3];
	buf[4] = FREQ_SCOPE[4];
	buf[5] = FREQ_SCOPE[5];
	buf[6] = FREQ_SCOPE[6];
	buf[7] = FREQ_SCOPE[7];
	buf[8] = 'M';
	buf[9] = 'H';
	buf[10] = 'Z';
	buf[11] = '+';
	buf[12] = '\0';
	PCLINK_INTF_TransmitFrame((DH_U8 *) buf,strlen((char *) buf));
	PCLINK_INTF_TransmitFrame(StrMenuSet.Write_Password, 6);
	PCLINK_INTF_TransmitFrame(StrMenuSet.Read_Password, 6);
#else
	if (opt == FLASH_CMD_WRITE)
		PCLINK_INTF_TransmitFrame((uint8 *)VERSION.FW_ver, sizeof(VERSION.FW_ver));
	else if (opt == FLASH_CMD_READ) //printk(VERSION.FR_ver);
		PCLINK_INTF_TransmitFrame((uint8 *)VERSION.FR_ver, sizeof(VERSION.FR_ver));
	else if (opt == FLASH_CMD_READRCD) //printk(VERSION.FR_ver);
	{
		char* c = "Record Read";
		Write_ReadLong = APP_RecordReadStart(FlashAddr);
		PCLINK_INTF_TransmitFrame((DH_U8*)c, strlen(c));
		return;
	}
	PCLINK_INTF_TransmitFrame((uint8 *)ver_str,strlen(ver_str));
	PCLINK_INTF_TransmitFrame((uint8 *)VERSION.date,sizeof(VERSION.date));
	PCLINK_INTF_TransmitFrame((uint8 *)VERSION.time,sizeof(VERSION.time));

#if DEV_TYPE_SEL==DEV_TYPE_DM855_EN || DEV_TYPE_SEL==DEV_TYPE_DM850_EN
	if(GloVarFlag.KeyPadType)
	{
		PCLINK_INTF_TransmitFrame((uint8 *)devinfo[DEV_TYPE_DM850_EN],strlen((char *)devinfo[DEV_TYPE_DM850_EN]));
	}
	else
	{
		PCLINK_INTF_TransmitFrame((uint8 *)devinfo[DEV_TYPE_DM855_EN],strlen((char *)devinfo[DEV_TYPE_DM855_EN]));
	}
#else
	PCLINK_INTF_TransmitFrame((uint8 *)devinfo[DEV_TYPE_SEL],strlen((char *)devinfo[DEV_TYPE_SEL]));
#endif

	PCLINK_INTF_TransmitFrame((uint8 *)devfreq[FREQ_SCOPE_SET],strlen((char *)devfreq[FREQ_SCOPE_SET]));
	PCLINK_INTF_TransmitFrame(StrMenuSet.Write_Password, 6);
	PCLINK_INTF_TransmitFrame(StrMenuSet.Read_Password, 6);

//	getversionstr();

#endif
}

//发送设备信息
DH_VOID Pc_get_ver_date(DH_U8 *SendBuf)
{
	strcpy((char*)(SendBuf + 48), ver_str);
	strcpy((char*)(SendBuf + 48 + strlen(ver_str)), VERSION.date);
	strcpy((char*)(SendBuf + 80), FREQ_SCOPE);
}

uint32 BLL_IAP_Read(DH_VOID)
{
#if	FLASH_STORE_SEL == FLASH_STORE_IN_TC58
	DH_U8 flashsel = g_FlashSaveFlg;
	DH_U8 ReadCmd[4] =	{ 'R', 'e', 'a', 'd' };
	DH_U16 RxLen_i = 0, currentPcreadLong = 0, i = 0;
	DH_U32 CheckSum = 0;
	DH_U32 FlashDestination = FlashAddr;

	if (FlashAddr & 0x80000000)
	{
		FlashAddr &= 0x7FFFFFFF;
		flashsel = g_SecFlashType;
	}

	if (FLASH_PCWDB_BUFF_ADDR == FlashAddr)
	{
		FlashAddr = FLASH_PCWDB_ADDR;
	}
	FlashDestination = FlashAddr;
	flashsel = g_SecFlashType;

	FlashBufLock();
	while (1)
	{
		RxLen_i = PCLINK_INTF_ReceiveFrame(FlashBuf, 4, PCWAITTOUTREAD);
		if (RxLen_i == 4)
		{
			for (i = 0; i < 4; i++)
			{
				if (ReadCmd[i] != FlashBuf[i])
				{
					break;
				}
			}
			dhtime_delay(100);
			if (i == 4)/*则是正常的帧数据*/
			{
				FLASH_INTF_ReadStr(flashsel, FlashDestination,FlashBuf, UART_PC_PACKETLEN);

				if (currentPcreadLong== 0
				&& flashsel==g_FlashSaveFlg
				&& FlashDestination==PCWDBADDR)
				{
					Pc_get_ver_date(FlashBuf);
				}
				PCLINK_INTF_TransmitFrame(FlashBuf, UART_PC_PACKETLEN);
				FlashDestination += UART_PC_PACKETLEN;

				for (i = 0; i < UART_PC_PACKETLEN; i++)
				{
					CheckSum += FlashBuf[i];
				}

				currentPcreadLong++;

				if(Write_ReadLong>99)
				{LCD_BLL_printf_ext(50, 120, lcd_g,(DH_U8*) "%d/%d",currentPcreadLong, Write_ReadLong);}
				else
				{LCD_BLL_printf_ext(50, 136, lcd_g, (DH_U8*) "%d/%d",currentPcreadLong, Write_ReadLong);}

				if (currentPcreadLong >= Write_ReadLong)/*读取完成*/
				{
					FlashBufUnLock();
					dhtime_delay(1000);
					return CheckSum;
				}
			}
		}
		else/*超时则退出读频*/
		{
			FlashBuf[4] = 0;
//			LCD_BLL_printf_ext(23,0,lcd_menu,(DH_U8*) "Rec%d,%s ",RxLen_i,FlashBuf);
//			dhtime_delay(2000);
			FlashBufUnLock();
			return 0;
		}
	}
#else
	DH_U8 flashsel = g_FlashSaveFlg;
	DH_U8 ReadCmd[4] =	{ 'R', 'e', 'a', 'd' };
	DH_U16 RxLen_i = 0, currentPcreadLong = 0, i = 0;
	DH_U32 CheckSum = 0;
	DH_U32 FlashDestination = FlashAddr;

	if (FlashAddr & 0x80000000)
	{
		FlashDestination = FlashAddr & 0x7FFFFFFF;
		flashsel = g_SecFlashType;
	}

	FlashBufLock();
	while (1)
	{
		RxLen_i = PCLINK_INTF_ReceiveFrame(FlashBuf, 4, PCWAITTOUTREAD);
		if (RxLen_i == 4)
		{
			for (i = 0; i < 4; i++)
			{
				if (ReadCmd[i] != FlashBuf[i])
				{
					break;
				}
			}

			if (i == 4)/*则是正常的帧数据*/
			{
				FLASH_INTF_ReadStr(flashsel, FlashDestination,FlashBuf, UART_PC_PACKETLEN);

				if (1)
				{
					if (currentPcreadLong== 0
					&& flashsel==0
					&& FlashDestination==PCWDBADDR)
					{
						Pc_get_ver_date(FlashBuf);
					}

					PCLINK_INTF_TransmitFrame(FlashBuf, UART_PC_PACKETLEN);

					FlashDestination += UART_PC_PACKETLEN;

					for (i = 0; i < UART_PC_PACKETLEN; i++)
					{
						CheckSum += FlashBuf[i];
					}

					currentPcreadLong++;

					if(Write_ReadLong>99)
					{LCD_BLL_printf_ext(50, 87, lcd_g, "%d/%d",currentPcreadLong, Write_ReadLong);}
					else
					{LCD_BLL_printf_ext(50, 103, lcd_g, "%d/%d",currentPcreadLong, Write_ReadLong);}

					if (currentPcreadLong >= Write_ReadLong)/*读取完成*/
					{
						FlashBufUnLock();
						dhtime_delay(1000);
						return CheckSum;
					}
				}
				else
				{
					FlashBufUnLock();
					dhtime_delay(1000);
					return 0;
				}
			}
		}
		else/*超时则退出读频*/
		{
			FlashBufUnLock();
			return 0;
		}
	}

#endif
}

DH_VOID BLL_IAP_PcRead(DH_VOID)
{
	DH_U32 CheckSum = 0;

	DH_U8 CheckSumToPc[13] =
	{ 'C', 'h', 'e', 'c', 'k', 's', 'u', 'm', 'R' };

	BLL_IAP_SendDeviveInf(FLASH_CMD_READ);

	CheckSum = BLL_IAP_Read();

	MoveValue(CheckSum, CheckSumToPc + 9);

	PCLINK_INTF_TransmitFrame(CheckSumToPc, 13);
}

/*****************************************************************************
 *   函数名: BLL_IAP_PcWrite
 *   功能描述:
 *   参数定义:
 *	   返回值:
 *   创建人：陶林，2013/06/15
 ******************************************************************************/
DH_VOID BLL_IAP_PcWrite(DH_VOID)
{
#if	FLASH_STORE_SEL == FLASH_STORE_IN_TC58
    DH_U16 checkword = 0x1111;
	DH_U32 CheckSum = 0;
	DH_U8 CheckSumToPc[13] ={ 'C', 'h', 'e', 'c', 'k', 's', 'u', 'm', 'W' };


	UART_DRV_ClearBuf(UART1);

	BLL_IAP_SendDeviveInf(FLASH_CMD_WRITE);

	CheckSum = BLL_IAP_Write();

	if (CheckSum)
	{
		MoveValue(CheckSum, CheckSumToPc + 9);

		PCLINK_INTF_TransmitFrame(CheckSumToPc, 13);

		if (FLASH_PCWDB_BUFF_ADDR == FlashAddr)//写频
		{
			checkword = RECORD_VERIFY_VALUE;

			GPS_OFF;
			/*备份数据*/
		#if FLASH_STORE_SEL == FLASH_STORE_IN_TC58
//			TC58_INTF_BlockErase(FLASH_PCWDB_ADDR);
			FlashBuf2Lock();
			for(CheckSum = 0;CheckSum<FLASH_PCWDB_SIZE/FLASH_SECTOR_SIZE;CheckSum++)
			{
				FLASH_INTF_ReadStr(FLASH_IC_PROG,FLASH_PCWDB_BUFF_ADDR+CheckSum*FLASH_SECTOR_SIZE,FlashBuff,FLASH_SECTOR_SIZE);
				TC58_INTF_WriteStr2(FLASH_PCWDB_ADDR+CheckSum*FLASH_SECTOR_SIZE,FlashBuff,FLASH_SECTOR_SIZE);
			}
			FlashBuf2UnLock();

			TC58_INTF_BlocksBackup(FLASH_PCWDB_ADDR,FLASH_PCWDB_BK_ADDR,FLASH_PCWDB_SIZE);

//			TC58_INTF_BlocksBackup(FLASH_PCWDB_ADDR+g_NandFlashBlock_Size,FLASH_PCWDB_BK_ADDR+g_NandFlashBlock_Size);

		#else
			FlashBufLock();
			for (CheckSum = 0;CheckSum < (FLASH_PCWDB_SIZE / FLASH_SECTOR_SIZE);CheckSum++)
			{
				FLASH_INTF_ReadStr(g_FlashSaveFlg,
						FLASH_PCWDB_ADDR + CheckSum * FLASH_SECTOR_SIZE,
						FlashBuf, FLASH_SECTOR_SIZE);
				FLASH_INTF_WriteStr(g_FlashSaveFlg,
						FLASH_PCWDB_BK_ADDR + CheckSum * FLASH_SECTOR_SIZE,
						FlashBuf, FLASH_SECTOR_SIZE);
			}
			FlashBufUnLock();
			/*for (CheckSum = Write_ReadLong*UART_PC_PACKETLEN;CheckSum < (FLASH_PCWDB_SIZE / FLASH_SECTOR_SIZE);CheckSum++)
			{
				FLASH_INTF_SectorErase(g_FlashSaveFlg,FLASH_PCWDB_ADDR+CheckSum);
				FLASH_INTF_SectorErase(g_FlashSaveFlg,FLASH_PCWDB_BK_ADDR+CheckSum);
			}*/
		#endif
			PC_BLL_PcWirteOk();

			BLL_PCIAP_PCSetDataTime();

			if (CSBK_APP_PadlockStatGet() != PADLOCKDIS)
				CSBK_APP_PadlockStatSet(PADLOCKDIS);

//			FLASH_INTF_WriteStr(g_FlashSaveFlg, FLASH_PCWDB_VALID_ADDR,	&checkword, 2);
  			MENU_APP_MenuSetSetted(BARTYPESETFLG_ZONE,0);
			MENU_APP_MenuSetSetted(BARTYPESETFLG_CurChn,0);
			MENU_APP_MenuSetSetted(BARTYPESETFLG_DisplayMode,E_DispMode_ChnNName);/*回到名称显示模式*/
		}

		dhtime_delay(500);
		AppSysReset(); /* 重启 */
	}
	else
	{
		LCD_CCL_DispMsgBox(Mes_wrong, (DH_U8*)"TimeOut");
	}
#else
	DH_U16 checkword = 0x1111;
	DH_U32 CheckSum = 0;
	DH_U8 CheckSumToPc[13] ={ 'C', 'h', 'e', 'c', 'k', 's', 'u', 'm', 'W' };

	if(  (FLASH_PCWDB_ADDR == FlashAddr
	   || FLASH_UNICODEDOT_ADDR == FlashAddr
	   || FLASH_GBK2UNICODE_ADDR == FlashAddr)
	&& g_FlashSaveFlg == FLASH_IC_TC58)
	{
		for(CheckSum=0;CheckSum<UART_PC_PACKETLEN*Write_ReadLong;)
		{
			TC58_INTF_BlockErase(CheckSum+FlashAddr);
			CheckSum+=g_NandFlashBlock_Size;
		}
	}
	UART_DRV_ClearBuf(UART1);

	BLL_IAP_SendDeviveInf(FLASH_CMD_WRITE);

	CheckSum = BLL_IAP_Write();

	if (CheckSum)
	{
		MoveValue(CheckSum, CheckSumToPc + 9);

		PCLINK_INTF_TransmitFrame(CheckSumToPc, 13);

		if (FLASH_PCWDB_ADDR == FlashAddr)
		{
			checkword = RECORD_VERIFY_VALUE;

			GPS_OFF;

			PC_BLL_PcWirteOk();

			BLL_PCIAP_PCSetDataTime();

			if (CSBK_APP_PadlockStatGet() != PADLOCKDIS)
				CSBK_APP_PadlockStatSet(PADLOCKDIS);

			/*备份数据*/
			#if FLASH_STORE_SEL == FLASH_STORE_IN_TC58
				TC58_INTF_BlocksBackup(FLASH_PCWDB_ADDR/g_NandFlashBlock_Size,FLASH_PCWDB_BK_ADDR/FLASH_PCWDB_BK_ADDR);
			#else
				FlashBufLock();
				for (CheckSum = 0;CheckSum < (FLASH_PCWDB_SIZE / FLASH_SECTOR_SIZE);CheckSum++)
				{
					FLASH_INTF_ReadStr(g_FlashSaveFlg,
							FLASH_PCWDB_ADDR + CheckSum * FLASH_SECTOR_SIZE,
							FlashBuf, FLASH_SECTOR_SIZE);
					FLASH_INTF_WriteStr(g_FlashSaveFlg,
							FLASH_PCWDB_BK_ADDR + CheckSum * FLASH_SECTOR_SIZE,
							FlashBuf, FLASH_SECTOR_SIZE);
				}
				FlashBufUnLock();
				/*for (CheckSum = Write_ReadLong*UART_PC_PACKETLEN;CheckSum < (FLASH_PCWDB_SIZE / FLASH_SECTOR_SIZE);CheckSum++)
				{
					FLASH_INTF_SectorErase(g_FlashSaveFlg,FLASH_PCWDB_ADDR+CheckSum);
					FLASH_INTF_SectorErase(g_FlashSaveFlg,FLASH_PCWDB_BK_ADDR+CheckSum);
				}*/
			#endif
			FLASH_INTF_WriteStr(g_FlashSaveFlg, FLASH_PCWDB_VALID_ADDR,	&checkword, 2);
  			MENU_APP_MenuSetSetted(BARTYPESETFLG_ZONE,0);
			MENU_APP_MenuSetSetted(BARTYPESETFLG_CurChn,0);
			MENU_APP_MenuSetSetted(BARTYPESETFLG_DisplayMode,E_DispMode_ChnNName);/*回到名称显示模式*/
		}

		dhtime_delay(500);
		AppSysReset(); /* 重启 */
	}
#endif
}

DH_U32 BLL_IAP_Write(DH_VOID)
{
#if	FLASH_STORE_SEL == FLASH_STORE_IN_TC58
	DH_U8 stat,erase = 0;
	DH_U16 i = 0, RxLen_i = 0, currentPcWriteLong = 0;
	DH_U32 CheckSum = 0;
	DH_U32 FlashDestination = FlashAddr;

	FlashBuf2Lock();

	if (0x80000000 == (FlashDestination & 0x80000000)) //片外第二个 flash存储
	{
		FlashDestination &= 0x7FFFFFFF;

		/*if( ( FLASH_UNICODEDOT_ADDR == FlashAddr || FLASH_ICON_BASE_ADDR == FlashAddr)
		&& g_FlashSaveFlg == FLASH_IC_TC58)
		{
			for(CheckSum=0;CheckSum<UART_PC_PACKETLEN*Write_ReadLong;)
			{
				TC58_INTF_BlockErase(CheckSum+FlashAddr);
				CheckSum+=g_NandFlashBlock_Size;
			}
			erase = 1;
			CheckSum = 0;
		}*/

		while (1)
		{
			RxLen_i = PCLINK_INTF_ReceiveFrame(FlashBuff, UART_PC_PACKETLEN,PCWAITTOUTWRITE);
			if (RxLen_i >= UART_PC_PACKETLEN)
			{
				for (i = 0; i < UART_PC_PACKETLEN; i++)
					CheckSum += FlashBuff[i];

				if(g_SecFlashType==FLASH_IC_TC58)
				{
					if(erase){
						stat = TC58_INTF_WriteStr(FlashDestination, FlashBuff,	UART_PC_PACKETLEN);
					}else{
						stat = TC58_INTF_WriteStr2(FlashDestination, FlashBuff,	UART_PC_PACKETLEN);
					}
				}else{
					FLASH_INTF_WriteStr(g_SecFlashType,FlashDestination, FlashBuff,	UART_PC_PACKETLEN);
				}

				if(Write_ReadLong>99)
				{LCD_BLL_printf_ext(50, 120, lcd_g, (DH_U8*) "%d/%d",currentPcWriteLong, Write_ReadLong);}
				else
				{LCD_BLL_printf_ext(50, 136, lcd_g, (DH_U8*) "%d/%d",currentPcWriteLong, Write_ReadLong);}
				if (stat != DH_SUCCESS)
				{
//					LCD_CCL_DispMsgBox(Mes_wrong, (DH_U8*) "TC58 WriteErr");
//					LCD_BLL_printf_ext(81, 19, lcd_b, (DH_U8*) "tc58 %d", stat);
					CheckSum = 0;
					break;
				}

				FlashDestination = FlashDestination + UART_PC_PACKETLEN;

				currentPcWriteLong++;

				if (currentPcWriteLong >= Write_ReadLong)
				{
					break;
				}else{
					PCLINK_INTF_TransmitFrame((DH_U8*)"Write", 5);
				}
			}

			else/*超时退出写频*/
			{
				CheckSum = 0;
				break;
			}
		}
	}
	else if(FlashAddr==FLASH_PCWDB_BUFF_ADDR)//FLASH_IC_PROG
	{
		DH_U32 rec_Offset = FlashAddr % FLASH_SECTOR_SIZE;

		if(FlashAddr>(1024+256)*1024)
		{
			FlashBuf2UnLock();
			LCD_CCL_DispMsgBox(Mes_wrong, (DH_U8*)"OverSpace");
			dhtime_delay(800);
			return 0;
		}
		////读写首地址不是一页 的首地址，则先把这一页前面的数据读入缓存
		if (rec_Offset)
		{
			FlashDestination = FlashAddr - rec_Offset;
			FLASH_INTF_ReadStr(FLASH_IC_PROG, FlashDestination, FlashBuff,
					rec_Offset);
		}

		while (1)
		{
			RxLen_i = PCLINK_INTF_ReceiveFrame(FlashBuff + rec_Offset,
					UART_PC_PACKETLEN, PCWAITTOUTWRITE);

			if (RxLen_i >= UART_PC_PACKETLEN)
			{
				for (i = 0; i < UART_PC_PACKETLEN; i++)
				{
					CheckSum += FlashBuff[rec_Offset + i];
				}

				currentPcWriteLong++;

/*				if (g_FlashSaveFlg == FLASH_IC_TC58)
				{
					stat = TC58_INTF_WriteStr(FlashDestination, FlashBuff,
							UART_PC_PACKETLEN);
					LCD_BLL_printf_ext(50, 87, lcd_g, "%d/%d",
							currentPcWriteLong, Write_ReadLong);
					if (stat != DH_SUCCESS)
					{
						LCD_CCL_DispMsgBox(Mes_wrong, "TC58 WriteErr");
						LCD_BLL_printf_ext(81, 19, lcd_b, "tc58 %d", stat);
						CheckSum = 0;
						break;
					}
					FlashDestination = FlashDestination + UART_PC_PACKETLEN;
				}
				else*/
				{
					/* 更新计算下一次接收缓存偏置  */
					rec_Offset = (rec_Offset + UART_PC_PACKETLEN)
							% FLASH_SECTOR_SIZE;

					if (currentPcWriteLong >= Write_ReadLong || rec_Offset == 0)
					{
						if (rec_Offset)/* 如果接收到最后一包，但数据不够一页，则读取这页剩下的数据进入缓存  */
						{
							FLASH_INTF_ReadStr(FLASH_IC_PROG,
									FlashDestination + rec_Offset,
									FlashBuff + rec_Offset,
									FLASH_SECTOR_SIZE - rec_Offset);
						}
						{
							FLASH_INTF_WriteSector(FLASH_IC_PROG, FlashDestination,
								FlashBuff, FLASH_SECTOR_SIZE);
						}
						FlashDestination = FlashDestination + FLASH_SECTOR_SIZE;
					}
				}
				if(Write_ReadLong>99)
				{LCD_BLL_printf_ext(50, 120, lcd_g, (DH_U8*)"%d/%d",currentPcWriteLong, Write_ReadLong);}
				else
				{LCD_BLL_printf_ext(50, 136, lcd_g, (DH_U8*)"%d/%d",currentPcWriteLong, Write_ReadLong);}

				if (currentPcWriteLong >= Write_ReadLong)
				{
					break;
				}else{
					PCLINK_INTF_TransmitFrame((DH_U8*)"Write", 5);
				}
			}
			else/*超时退出写频*/
			{
				LCD_CCL_DispMsgBox(Mes_wrong, (DH_U8*)"TimeOut");
				CheckSum = 0;
				break;
			}
		}
	}
	else
	{
	#if FUNC_PRO_IAP_EN
		if(FlashAddr == 0x3001000)//写IAP
		{
			FlashDestination = 0x0001000;
			DH_U32 rec_Offset = FlashDestination % FLASH_SECTOR_SIZE;

			////读写首地址不是一页 的首地址，则先把这一页前面的数据读入缓存
			if (rec_Offset)
			{
				FlashDestination = FlashDestination - rec_Offset;
				FLASH_PG_INTF_ReadPGSector(FlashDestination, FlashBuff,
						rec_Offset);
			}

			while (1)
			{
				RxLen_i = PCLINK_INTF_ReceiveFrame(FlashBuff + rec_Offset,
						UART_PC_PACKETLEN, PCWAITTOUTWRITE);

				if (RxLen_i >= UART_PC_PACKETLEN)
				{
					for (i = 0; i < UART_PC_PACKETLEN; i++)
					{
						CheckSum += FlashBuff[rec_Offset + i];
					}

					currentPcWriteLong++;

					/* 更新计算下一次接收缓存偏置  */
					rec_Offset = (rec_Offset + UART_PC_PACKETLEN)
							% FLASH_SECTOR_SIZE;

					if (currentPcWriteLong >= Write_ReadLong || rec_Offset == 0)
					{
						if (rec_Offset)/* 如果接收到最后一包，但数据不够一页，则读取这页剩下的数据进入缓存  */
						{
							FLASH_PG_INTF_ReadPGSector(	FlashDestination + rec_Offset,
								FlashBuff + rec_Offset,FLASH_SECTOR_SIZE - rec_Offset);
						}
						FLASH_PG_INTF_WritePGSector(FlashDestination,
							FlashBuff, FLASH_SECTOR_SIZE);

						FlashDestination = FlashDestination + FLASH_SECTOR_SIZE;
					}

					if(Write_ReadLong>99)
					{LCD_BLL_printf_ext(50, 120, lcd_g, (DH_U8*)"%d/%d",currentPcWriteLong, Write_ReadLong);}
					else
					{LCD_BLL_printf_ext(50, 136, lcd_g, (DH_U8*)"%d/%d",currentPcWriteLong, Write_ReadLong);}

					PCLINK_INTF_TransmitFrame((DH_U8*)"Write", 5);

					if (currentPcWriteLong >= Write_ReadLong)
					{
						break;
					}
				}
				else/*超时退出写频*/
				{
					LCD_CCL_DispMsgBox(Mes_wrong, (DH_U8*)"TimeOut");
					CheckSum = 0;
					break;
				}
			}

		}
	#endif
	}
	FlashBuf2UnLock();
	dhtime_delay(800);
	return CheckSum;
#else
	DH_U8 stat;
	DH_U16 i = 0, RxLen_i = 0, currentPcWriteLong = 0;
	DH_U32 CheckSum = 0;
	DH_U32 FlashDestination = FlashAddr;

	FlashBuf2Lock();

	if (0x80000000 == (FlashDestination & 0x80000000)) //片外第二个 flash存储
	{
		FlashDestination &= 0x7FFFFFFF;
		while (1)
		{
			RxLen_i = PCLINK_INTF_ReceiveFrame(FlashBuff, UART_PC_PACKETLEN,
			PCWAITTOUTWRITE);

			if (RxLen_i >= UART_PC_PACKETLEN)
			{
				for (i = 0; i < UART_PC_PACKETLEN; i++)
					CheckSum += FlashBuff[i];

				if(g_SecFlashType==FLASH_IC_TC58)
				{
					stat = TC58_INTF_WriteStr2(FlashDestination, FlashBuff,	UART_PC_PACKETLEN);
				}else{
					FLASH_INTF_WriteStr(g_SecFlashType,FlashDestination, FlashBuff,	UART_PC_PACKETLEN);
				}

				if(Write_ReadLong>99)
				{LCD_BLL_printf_ext(50, 87, lcd_g, "%d/%d",currentPcWriteLong, Write_ReadLong);}
				else
				{LCD_BLL_printf_ext(50, 103, lcd_g, "%d/%d",currentPcWriteLong, Write_ReadLong);}
				if (stat != DH_SUCCESS)
				{
					LCD_CCL_DispMsgBox(Mes_wrong, "TC58 WriteErr");
					LCD_BLL_printf_ext(81, 19, lcd_b, "tc58 %d", stat);
					CheckSum = 0;
					break;
				}

				FlashDestination = FlashDestination + UART_PC_PACKETLEN;

				currentPcWriteLong++;

				PCLINK_INTF_TransmitFrame((DH_U8*)"Write", 5);

				if (currentPcWriteLong >= Write_ReadLong)
				{
					break;
				}
			}

			else/*超时退出写频*/
			{
				LCD_CCL_DispMsgBox(Mes_wrong, "TimeOut");
				CheckSum = 0;
				break;
			}
		}
	}
	else
	{
		DH_U32 rec_Offset = FlashAddr % FLASH_SECTOR_SIZE;

		////读写首地址不是一页 的首地址，则先把这一页前面的数据读入缓存
		if (rec_Offset)
		{
			FlashDestination = FlashAddr - rec_Offset;
			FLASH_INTF_ReadStr(g_FlashSaveFlg, FlashDestination, FlashBuff,
					rec_Offset);
		}

		while (1)
		{
			RxLen_i = PCLINK_INTF_ReceiveFrame(FlashBuff + rec_Offset,
					UART_PC_PACKETLEN, PCWAITTOUTWRITE);

			if (RxLen_i >= UART_PC_PACKETLEN)
			{
				for (i = 0; i < UART_PC_PACKETLEN; i++)
				{
					CheckSum += FlashBuff[rec_Offset + i];
				}

				currentPcWriteLong++;

				if (g_FlashSaveFlg == FLASH_IC_TC58)
				{
					stat = TC58_INTF_WriteStr(FlashDestination, FlashBuff,
							UART_PC_PACKETLEN);
					LCD_BLL_printf_ext(50, 87, lcd_g, "%d/%d",
							currentPcWriteLong, Write_ReadLong);
					if (stat != DH_SUCCESS)
					{
						LCD_CCL_DispMsgBox(Mes_wrong, "TC58 WriteErr");
						LCD_BLL_printf_ext(81, 19, lcd_b, "tc58 %d", stat);
						CheckSum = 0;
						break;
					}
					FlashDestination = FlashDestination + UART_PC_PACKETLEN;
				}
				else
				{
					/* 更新计算下一次接收缓存偏置  */
					rec_Offset = (rec_Offset + UART_PC_PACKETLEN)
							% FLASH_SECTOR_SIZE;

					if (currentPcWriteLong >= Write_ReadLong || rec_Offset == 0)
					{
						if (rec_Offset)/* 如果接收到最后一包，但数据不够一页，则读取这页剩下的数据进入缓存  */
						{
							FLASH_INTF_ReadStr(g_FlashSaveFlg,
									FlashDestination + rec_Offset,
									FlashBuff + rec_Offset,
									FLASH_SECTOR_SIZE - rec_Offset);
						}
						if(FlashAddr == 0x3001000)//写IAP
						{
							FLASH_PG_INTF_WritePGSector(FlashDestination,
									FlashBuff, FLASH_SECTOR_SIZE);
						}else{
							FLASH_INTF_WriteSector(g_FlashSaveFlg, FlashDestination,
								FlashBuff, FLASH_SECTOR_SIZE);
						}
						FlashDestination = FlashDestination + FLASH_SECTOR_SIZE;
					}
				}
				if(Write_ReadLong>99)
				{LCD_BLL_printf_ext(50, 87, lcd_g, "%d/%d",currentPcWriteLong, Write_ReadLong);}
				else
				{LCD_BLL_printf_ext(50, 103, lcd_g, "%d/%d",currentPcWriteLong, Write_ReadLong);}

				PCLINK_INTF_TransmitFrame((DH_U8*)"Write", 5);

				if (currentPcWriteLong >= Write_ReadLong)
				{
					break;
				}
			}
			else/*超时退出写频*/
			{
				LCD_CCL_DispMsgBox(Mes_wrong, "TimeOut");
				CheckSum = 0;
				break;
			}
		}
	}
	FlashBuf2UnLock();
	dhtime_delay(800);
	return CheckSum;
#endif
}

/*******************************************************************************
 *函数名:PC_BLL_McuToPc
 *函数说明:
 *参数说明:
 *返回值:无
 *创建人:yufeng
 *创建日期:2012/11/18
 *******************************************************************************/
DH_VOID PC_BLL_McuToPc(DH_U8* DstBuf, DH_U8* SourBuf, DH_U16 Len, DH_U8 Opt,
		DH_U8 Ack, DH_U8 type)
{
	DH_U16 CheckSum;

	DstBuf[0] = 0x68;
	DstBuf[1] = type;
	DstBuf[2] = Opt;
	DstBuf[3] = Ack;

	DstBuf[4] = DstBuf[5] = 0; /* check sum */

	DstBuf[6] = Len % 256;
	DstBuf[7] = Len / 256;

	DstBuf[8 + Len] = 0x10;

	/* 存储数据 */
	if (Len)
	{
		memcpy((void*) (DstBuf + 8), (void*) SourBuf, Len);
	}

	/* 填充校验位 */
	CheckSum = PC_BLL_PcCheckSum(DstBuf, 9 + Len);

	DstBuf[4] = CheckSum >> 8;
	DstBuf[5] = CheckSum;

	PCLINK_INTF_TransmitFrame((DH_U8*)DstBuf, 9 + Len);

	return;
}

/*******************************************************************************
 *函数名:PcToMcuTuner
 *函数说明:MCU接收PC调测数据
 *参数说明:
 *返回值:无
 *创建人:yufeng
 *创建日期:2012/11/18
 *******************************************************************************/
DH_VOID PC_BLL_PcToTC58Img(DH_U8* Buf, DH_U16 Len, DH_U8 err)
{
	if (PCINFERR_OK==err)
	{
		DH_U32 addr = g_ImageIndex[Buf[3]-1][Buf[2]-1];

		Len -= 9;

		if((Buf[3] >= 1  && Buf[3] <= 9  && Len == MENU_ICO_SIZE)
		 ||(Buf[3] >= 10 && Buf[3] <= 18 && Len == MENU_BIGICO_SIZE)
		 ||(Buf[3] >= 19 && Buf[3] <= 20 && Len == NUMBER_ICO_SIZE))
		{
		#if (FLASH_ICON_STORE_SEL == FLASH_STORE_IN_TC58 )
			if(g_SecFlashType==FLASH_IC_TC58)
			{
				TC58_INTF_WriteStr2(addr, (DH_U8*) (Buf + 8), Len);
			}else{
				FLASH_INTF_WriteStr(g_SecFlashType,addr, (DH_U8*) (Buf + 8), Len);
			}
		#elif (FLASH_ICON_STORE_SEL == FLASH_STORE_IN_FLASH2 )
			FLASH_INTF_WriteStr(g_FlashSaveFlg,addr, (DH_U8*) (Buf + 8), Len);
		#endif
			PC_BLL_McuToPc(Buf, (DH_U8*) 0, 0, Buf[2], Buf[3],UART_MODE_UIIMG); /* 发送校验ok包 */
		}
		else
		{
			PC_BLL_McuToPc(Buf, (DH_U8*) 0, 0, TUNERALLWRITE, UFR_RECERR ,UART_MODE_UIIMG); /* 发送校验ok包 */
		}
		return;
	}
	else if (PCINFERR_TIMEOUT == err)
	{
		PC_BLL_McuToPc(Buf, (DH_U8*) 0, 0, TUNERALLWRITE, UFR_RECERR ,UART_MODE_UIIMG);
	}
	else if (PCINFERR_CHECKERR == err)
	{
		PC_BLL_McuToPc(Buf, (DH_U8*) 0, 0, TUNERALLWRITE, UFR_RECERR ,UART_MODE_UIIMG);
	}
	else
	{
		PC_BLL_McuToPc(Buf, (DH_U8*) 0, 0, TUNERALLWRITE, UFR_RECERR ,UART_MODE_UIIMG);
	}
}

/*******************************************************************************
 *函数名:PC_BLL_PCIAP_ImgRecData
 *函数说明:接收PC图片
 *参数说明:
 *返回值:无
 *创建人:yufeng
 *创建日期:2012/11/18
 *******************************************************************************/
DH_VOID PC_BLL_PCIAP_ImgRecData(DH_U8* RecBuf, DH_U32 HeadLen)
{
	DH_U8 err;
	DH_U16 RecLen;

	FlashBuf2Lock();
	memcpy(FlashBuff, RecBuf, UART_RX_FIRST_I);
	err = PC_BLL_TunerGetPcFrm(FlashBuff, &RecLen, UART_RX_CON_WAITT / 4,
			HeadLen);

	PC_BLL_PcToTC58Img(FlashBuff, RecLen, err);
	FlashBuf2UnLock();
}

/*****************************************************************************
 *   函数名: BLL_IAP_Flash
 *   功能描述:  BLL_IAP_Flash
 *   参数定义:
 *	 返回值:
 *   创建人：陶林，2013/06/15
 ******************************************************************************/
DH_VOID BLL_IAP_Flash(DH_U8* key)
{
	DH_U8 command = Option_Command(key);

	GetFlashAddrNPkg(key);

	switch (command)
	{
	case FLASH_CMD_ERASE:
		if(GPIO_DRV_GetInputStat(KEYPAD_INT1_PIN)==KEY_PRESSING)
		{
			/*DH_U32 CheckSum = 0;
			DH_U8 CheckSumToPc[13] ={ 'C', 'h', 'e', 'c', 'k', 's', 'u', 'm', 'I' };
			GPIO_DRV_IntDisable(MAP_PTT_PIN);

			PCLINK_INTF_TransmitFrame((DH_U8*)"  IAP",5);
			PCLINK_INTF_TransmitFrame((DH_U8*)VERSION.m, sizeof(VERSION.m));
			PCLINK_INTF_TransmitFrame((DH_U8*)VERSION.date, sizeof(VERSION.date));
			PCLINK_INTF_TransmitFrame((DH_U8*)VERSION.time, sizeof(VERSION.time));
			UART_DRV_ClearBuf(UART1);
			FlashAddr = 0x3001000;
			Write_ReadLong++;
			CheckSum = BLL_IAP_Write();
			MoveValue(CheckSum,CheckSumToPc+9);
			PCLINK_INTF_TransmitFrame(CheckSumToPc,13);
			dhtime_delay(1000);
			AppSysReset();*/
		}
		break;

	case FLASH_CMD_WRITE:
	#if !FUNC_RWALL_EN
		if(FlashAddr!=FLASH_PCWDB_BUFF_ADDR)
		{
					LCD_CCL_DispMsgBox(Mes_ok, (DH_U8*)*(StrMenuCNRun[StrMenuSet.language].Text));
		}
		else
	#endif
		{
			if (CheckPassWord(FLASH_CMD_WRITE, key))
			{
				MenuTopLock();
				TMR_APP_TmrHndStop(TMRHND_CALLREF);
				LCD_CCL_DispMsgBox(Mes_ok, (DH_U8*)*(StrPCwriteData[StrMenuSet.language].Text+6));
				BLL_IAP_PcWrite();/*写到M25P16*/
				MenuTopUnLock();
			}
			else
			{
				LCD_CCL_DispMsgBox(Mes_wrong, (DH_U8*)*(StrPCwriteData[StrMenuSet.language].Text+4));
			}
		}
		MENU_app_Menu_Top_Init();
		break;

	case FLASH_CMD_READ:
	#if !FUNC_RWALL_EN
		if(FlashAddr==FLASH_PCWDB_BUFF_ADDR)
		{
			LCD_CCL_DispMsgBox(Mes_ok, (DH_U8*)*(StrMenuCNRun[StrMenuSet.language].Text));
		}
		else
	#endif
		{
			if (CheckPassWord(FLASH_CMD_READ, key))
			{
				MenuTopLock();
				TMR_APP_TmrHndStop(TMRHND_CALLREF);
				LCD_CCL_DispMsgBox(Mes_ok, (DH_U8*)*(StrPCwriteData[StrMenuSet.language].Text+7));
				BLL_IAP_PcRead();/*读M25p16*/
				MenuTopUnLock();
			}
			else
			{
				LCD_CCL_DispMsgBox(Mes_wrong, (DH_U8*)*(StrPCwriteData[StrMenuSet.language].Text+5));
			}
		}
		SLP_BLL_SlpModeExitSM(1);
		MENU_app_Menu_Top_Init();
		break;
	case FLASH_CMD_READRCD://读取录音内容

	#if FUNC_MENU_DISP == MENU_DISP_LCD
		LCD_CCL_DispMsgBox(Mes_ok, (DH_U8*)*(StrPCwriteData[StrMenuSet.language].Text+7));
	#endif
		BLL_IAP_PcReadRecord();

		MENU_app_Menu_Top_Init();
		break;
	default:
		break;
	}
}

void BLL_IAP_PcReadRecord(void)
{
	DH_U32 CheckSum = 0;

	DH_U8 CheckSumToPc[13] = { 'C', 'h', 'e', 'c', 'k', 's', 'u', 'm', 'R' };

	BLL_IAP_SendDeviveInf(FLASH_CMD_READRCD);

	CheckSum = BLL_IAP_RcdRead();

	MoveValue(CheckSum, CheckSumToPc + 9);

	PCLINK_INTF_TransmitFrame(CheckSumToPc, 13);
}
//////////////////////////////////////////////
//区分:
//yy-m-d格式;    yy/m/d格式;     yy.m.d格式;
//yy-mm-dd格式;  yy/mm/dd格式;   yy.mm.dd格式;
//yyyy-m-d格式;  yyyy/m/d格式;   yyyy.m.d格式;(注意月份 的位数)
//yyyy-mm-dd格式;yyyy/mm/dd格式; yyyy.mm.dd格式;
/////////////////////////////////////////////
DH_VOID BLL_PCIAP_PCSetDataTime(DH_VOID)
{
#if  0
	uint8 DateBuf[16]=
	{	0};
	TIMESTR timer;
	timer=INTF_GetCurTime();

	E2PROMReadPage(PCBA_PROGDATE,DateBuf,16);

	timer.Year=((DateBuf[0]-'0')*1000+(DateBuf[1]-'0')*100+(DateBuf[2]-'0')*10+(DateBuf[3]-'0'));
	time.month=((DateBuf[5]-'0')*10+(DateBuf[6]-'0'));
	time.day=((DateBuf[8]-'0')*10+(DateBuf[9]-'0'));

	INTF_CurTimeSet(timer);
#else
	uint8 DateBuf[16] =
	{ 0 };
	uint8 i = 0;
	DH_U8 seperate = 0;

	TIME_Str time = RTC_DRV_GetTime();

	FLASH_INTF_ReadStr(g_FlashSaveFlg, PCBA_PROGDATE, DateBuf, 16);

	for (i = 0; i < 16; i++)
	{
		if (DateBuf[i] == '/' || DateBuf[i] == '-' || DateBuf[i] == '.')
		{
			seperate = DateBuf[i];
			break;
		}
	}

	if (DateBuf[2] == seperate)
	{ //yy-mm-dd格式///或者yy-m-d格式//或者yy/m/d格式
		time.year = 2000 + ((DateBuf[0] - '0') * 10 + (DateBuf[1] - '0'));
		if (DateBuf[4] == seperate) //肯定是yy-m-d格式
		{
			time.month = (DateBuf[3] - '0');
			if (DateBuf[6] == ' ')
			{
				time.day = (DateBuf[5] - '0');
			}
			else if (DateBuf[7] == ' ')
			{
				time.day = ((DateBuf[5] - '0') * 10 + (DateBuf[6] - '0'));
			}
			else //出错
			{
				return;
			}
		}
		else if (DateBuf[5] == seperate)
		{
			time.month = ((DateBuf[3] - '0') * 10 + (DateBuf[4] - '0'));
			if (DateBuf[7] == ' ') //肯定是yy-m-d格式
			{
				time.day = (DateBuf[6] - '0');
			}
			else if (DateBuf[8] == ' ')
			{
				time.day = ((DateBuf[6] - '0') * 10 + (DateBuf[7] - '0'));
			}
			else //出错
			{
				return;
			}
		}
		else //出错
		{
			return;
		}
	}
	else if (DateBuf[4] == seperate) //
	{ //yyyy-格式
		time.year = ((DateBuf[0] - '0') * 1000 + (DateBuf[1] - '0') * 100
				+ (DateBuf[2] - '0') * 10 + (DateBuf[3] - '0'));
		if (DateBuf[6] == seperate) //yyyy-m-d格式
		{
			time.month = (DateBuf[5] - '0');
			if (DateBuf[8] == ' ')
			{ //yyyy-m-d
				time.day = (DateBuf[7] - '0');
			}
			else if (DateBuf[9] == ' ')
			{ //yyyy-m-d
				time.day = ((DateBuf[7] - '0') * 10 + (DateBuf[8] - '0'));
			}
			else //出错
			{
				return;
			}
		}
		else if (DateBuf[7] == seperate)
		{
			time.month = ((DateBuf[5] - '0') * 10 + (DateBuf[6] - '0'));
			if (DateBuf[9] == ' ')
			{
				time.day = (DateBuf[8] - '0');
			}
			else if (DateBuf[10] == 0x20)
			{
				time.day = ((DateBuf[8] - '0') * 10 + (DateBuf[9] - '0'));
			}
			else //出错
			{
				return;
			}
		}
		else //出错
		{
			return;
		}
	}
	else //出错
	{
		return;
	}
///////////////////////////////////////////
//区分:时间格式,经测试,不需要区分.
///////////////////////////////////////////
	for (i = 3; i < 13; i++) //找到0x20这个头
	{
		if (DateBuf[i] == ' ')
		{
			break;
		}
	}
	if (DateBuf[i + 3] == ':') //冒号
	{
		time.hour = ((DateBuf[i + 1] - '0') * 10 + (DateBuf[i + 2] - '0'));
		time.minute = ((DateBuf[i + 4] - '0') * 10 + (DateBuf[i + 5] - '0'));
	} //出错不再设置时间.
	RTC_DRV_SetTime(time);
#endif
}


/*******************************************************************************
 *函数名:PC_BLL_PCIAP_RcdInfoSend
 *函数说明:发送录音列表
 *参数说明:
 *返回值:无
 *创建人:yufeng
 *创建日期:2012/11/18
 *******************************************************************************/
DH_VOID PC_BLL_PCIAP_RcdInfoSend(DH_U8* Buf, DH_U32 HeadLen)
{
	DH_U8 err;
	DH_U16 RecLen;
	DH_U16 rcdindex;

	err = PC_BLL_TunerGetPcFrm(Buf, &RecLen, UART_RX_CON_WAITT / 4, HeadLen);

	rcdindex = 256*Buf[8]+Buf[9];

	if (PCINFERR_OK == err)
	{
		APP_Record_GetNSendMainInfo(rcdindex);
	}
	else
	{
		PCLINK_INTF_TransmitFrame((DH_U8*)"PCINFERR_ERR.",13);
	}
}

/*******************************************************************************
 *函数名:PC_BLL_PCIAP_MsgContSend
 *函数说明:发送录音列表
 *参数说明:
 *返回值:无
 *创建人:yufeng
 *创建日期:2012/11/18
 *******************************************************************************/
DH_VOID PC_BLL_PCIAP_MsgContSend(DH_U8* Buf, DH_U32 HeadLen)
{
	DH_U8 err;
	DH_U16 RecLen;

	err = PC_BLL_TunerGetPcFrm(Buf, &RecLen, UART_RX_CON_WAITT / 4, HeadLen);

	if (PCINFERR_OK == err)
	{
		struct _msginfostr_ *info =  &MsginfoStr;;
		DH_U8 flg = Buf[8] == 0?NOTETYPE_INBOX:NOTETYPE_OUTBOX;
		DH_U8 msgnum = MSG_APP_MsgGetListNum(flg);

		for(err = 0;err<msgnum;err++)
		{
			MSG_APP_MsgGetContent(flg ,err+1 );

	//		if(msgnum) MSG_app_getMsgInfo(flg ,Buf[9],&info);

			FlashBufLock();
			sprintf((void*)FlashBuf,(void*)
					"#%d#%d#%s#%08d#%s#%04d/%02d/%02d %02d:%02d\r\n",\
					err+1,\
					msgnum,\
					flg==NOTETYPE_INBOX?"In":"Out",\
					info->id,\
					g_InputBuf,\
					0,0,0,0,0
				);
			PCLINK_INTF_TransmitFrame((DH_U8*)FlashBuf, strlen(FlashBuf));

			FlashBufUnLock();
			dhtime_delay(1000);
		}
	}
	else
	{
		PCLINK_INTF_TransmitFrame((DH_U8*)"PCINFERR_ERR.",13);
	}
}
/*******************************************************************************
 *函数名:BLL_IAP_ReadRcd
 *函数说明:上传录音内容
 *参数说明:
 *返回值:无
 *创建人:yufeng
 *创建日期:2012/11/18
 *******************************************************************************/
DH_U32 BLL_IAP_RcdRead(DH_VOID)
{
	DH_U8 ReadCmd[4] =	{ 'R', 'e', 'a', 'd' };
	DH_U16 RxLen_i = 0, currentPcreadLong = 0, i = 0;
	DH_U32 CheckSum = 0;
	DH_U8 err = ERR_NOERR;

	FlashBufLock();
	while (ERR_NOERR == err)
	{
		RxLen_i = PCLINK_INTF_ReceiveFrame(FlashBuf, 4, PCWAITTOUTREAD);
		if (RxLen_i == 4)
		{
			for (i = 0; i < 4; i++)
			{
				if (ReadCmd[i] != FlashBuf[i])
				{
					break;
				}
			}
//			dhtime_delay(100);
			if (i == 4)/*则是正常的帧数据*/
			{
				err = APP_RecordGetCntt(FlashBuf,RECORD_RP_CONT_SIZE);

				PCLINK_INTF_TransmitFrame(FlashBuf, RECORD_RP_CONT_SIZE);


				for (i = 0; i < RECORD_RP_CONT_SIZE; i++)
				{
					CheckSum += FlashBuf[i];
				}

				currentPcreadLong++;

				if(Write_ReadLong>99)
				{LCD_BLL_printf_ext(50, 120, lcd_g,(DH_U8*) "%d/%d",currentPcreadLong, Write_ReadLong);}
				else
				{LCD_BLL_printf_ext(50, 136, lcd_g, (DH_U8*) "%d/%d",currentPcreadLong, Write_ReadLong);}

				if (currentPcreadLong >= Write_ReadLong)/*读取完成*/
				{
					break;
				}
			}
		}
		else/*超时则退出读频*/
		{
			CheckSum = 0;
			break;
		}
	}
	FlashBufUnLock();
	dhtime_delay(500);
	return CheckSum;
}
