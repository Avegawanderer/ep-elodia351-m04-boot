/**
  ******************************************************************************
  * @file    STM32F4xx_IAP/src/menu.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    10-October-2011
  * @brief   This file provides the software which contains the main menu routine.
  *          The main menu gives the options of:
  *             - downloading a new binary file, 
  *             - uploading internal flash memory,
  *             - executing the binary file already loaded 
  *             - disabling the write protection of the Flash sectors where the 
  *               user loads his binary file.
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

/** @addtogroup STM32F4xx_IAP
  * @{
  */

/* Includes ------------------------------------------------------------------*/
 
#include "app_config.h"
#include "include.h"
#include "IAP.h"

//#include "ymodem.h"

//#include <stdio.h>
//#include <stdlib.h>

#define FW_VER				"V1.08BZSR"
#define FLASH_PROGRAM       1
#define FLASH_WRITE         2
#define FLASH_READ          3
#define FLASH_READPROGRAM   4


#define PCWDB_WR_ADDR	 	 (10*FLASH_SECTOR_SIZE)



#define IAP_SendString printk








/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

DH_U32 FlashAddr;
DH_U32 Write_ReadLong;
DH_U32 JumpAddress;
extern DH_U8 au8FlashBuf[DH_COMM_PAGE_4K];
extern DH_U8 g_SecFlashType;
extern DH_U16 g_TC58_Page_Size;
extern DH_U32 g_NandFlashBlock_Size;
DH_U8 tab_1024[1024] =
{
    0
};
DH_U8 tab_packet[PACKET_PC_SIZE] =
{
    0
};
//DH_U8 FileName[FILE_NAME_LENGTH];
DH_U8 lau8FlashBuf[DH_COMM_PAGE_4K];



/* Private function prototypes -----------------------------------------------*/
static DH_U32 PC_BLL_Serial_Proc(void);
static void SerialUpload(void);
int32  FLASH_ReadNTran(DH_U8 stat);
DH_U32 BLL_PCIAP_Serial(DH_U8 stat);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Download a file via serial port
  * @param  None
  * @retval None
  */
DH_U32    Get_Command(DH_U8 *key)
{
	DH_U8 head=0;
	while(1)
	{
		if (5 ==  PCLINK_INTF_ReceiveFrame(key,5,IAP_COM_WAIT_TIME)) 
		{
			/*读剩下的14字节*/
			if (14 ==  PCLINK_INTF_ReceiveFrame(key+5,14,IAP_COM_WAIT_TIME));/*读写flash*/
			return 1;
		}
	}
}
void    GetHead(DH_U8 *key)
{
  /* Waiting for user input */
  while (1)
  {
   	if (Get_Command(key) )
	{
//		UARTRcvClr();
		break;
	}
  }
}

uint8    Option_Command(uint8 *key)
{

	DH_U8 i=0;
	DH_U8 str_Erase[6]={0};
	DH_U8 str_RWrite[12]={0};
	for(i=0;i<5;i++)
	{
		str_Erase[i]=(DH_U8)key[i];
		str_RWrite[i]=(DH_U8)key[i];
	}
	for(;i<11;i++)
	{
		str_RWrite[i]=(DH_U8)key[i];
	}
	if(strcmp(str_Erase,"Erase")==0)
		{return FLASH_PROGRAM;}
	if(strcmp(str_RWrite,"Flash Write")==0)
		{return FLASH_WRITE;}
	if(strcmp(str_RWrite,"Flash Read ")==0)
		{return FLASH_READ;}
	return 0;
}
/*
*获取第一次读写的地址和数据包的大小
*/
void    GetAddrAndLong(uint8 *key)
{
	uint8 i=0;
	FlashAddr=0;/*防止多次升级而叠加在一起*/
	Write_ReadLong=0;
	for(i=11;i<15;i++)
	{
		FlashAddr+=key[i];
		if(i!=14)
		FlashAddr<<=8;
	}
	for(;i<19;i++)
	{
		Write_ReadLong+=key[i];
		if(i!=18)
		Write_ReadLong<<=8;
	}
//	Write_ReadLong ++;
}


void    MoveValue(uint32 CheckSum,uint8* CheckSumToPc)
{
	CheckSumToPc[0]=CheckSum>>24;
	CheckSumToPc[1]=CheckSum>>16;
	CheckSumToPc[2]=CheckSum>>8;
	CheckSumToPc[3]=CheckSum;
}


DH_U8 BLL_IAP_Flash(DH_U8* key)
{
	DH_U8 command = Option_Command(key);

	GetAddrAndLong(key);

//	if(FlashAddr == IAP_APP_ADDR)
//	{
//		command = FLASH_READPROGRAM;
//	}
	if(command !=0)
	{
	//	bDispReflesh = 1;

		if( BLL_PCIAP_Serial(command))
		{
			if(command == FLASH_PROGRAM) return 1;
		}
		else{
			IAP_SendString("Flash operation fail!\r\n");
		}
	}
	return 0;
/*	switch (command)
	{
	case FLASH_PROGRAM:
		BLL_IAP_Program();写到M25P16
		break;

	case FLASH_WRITE:
		BLL_IAP_PcWrite();写到M25P16
		break;

	case FLASH_READ:
		BLL_IAP_PcRead();读M25p16

		break;

	default:
		break;
	}*/
}


DH_U32   PC_BLL_Serial_Proc(void)
{
	while(1)
	{
		IAP_SendString("Waiting for the file to be sent ... (press 'a' to abort)\n\r");

		while(1){
			DH_U8 key[20];

			GetHead(key);

			DH_U8 command = Option_Command(key);

			GetAddrAndLong(key);

			if(FlashAddr == IAP_APP_ADDR) command = FLASH_READPROGRAM;

			if(command !=0)
			{
				if( BLL_PCIAP_Serial(command)) {
//					IAP_SendString("Flash Write successful!\r\n");
					if(command == FLASH_PROGRAM) return 1;
				}
				else{
					IAP_SendString("Flash operation fail!\r\n");
					break;
				}
			}
		}
	}
}


DH_VOID SendDeviveInf(DH_U8 stat)
{
	if(FLASH_PROGRAM==stat)
	{
		IAP_SendString("  IAP");
	}
	else if(FLASH_WRITE==stat)
	{
		IAP_SendString("  Write_2M_");//UARTSendStr(FW_VER);UARTSendStr(__DATE__);UARTSendStr(__TIME__);
	}
	else if(FLASH_READ==stat)
	{
		IAP_SendString("  Read_2M_");
	}
	IAP_SendString(FW_VER );
	IAP_SendString(__DATE__);
	IAP_SendString(__TIME__);
}

DH_U32    BLL_PCIAP_Serial(DH_U8 stat)
{
	DH_U32 CheckSum=0;
	DH_U8 CheckSumToPc[13]={'C','h','e','c','k','s','u','m','I'};
	int error = 0;

	SendDeviveInf(stat);

	if(FLASH_PROGRAM==stat)
	{
	//	dhtime_delay(500);
	
	//	uart_clear();
		FLASH_INTF_EraseSectors(IAP_APP_ADDR, Write_ReadLong*PACKET_PC_SIZE);
//    FLASH_INTF_EraseSectors(APP_EXIST_FLAG_ADDR, 0U);
//		CheckSum= 0;
//    FLASH_WriteData(APP_EXIST_FLAG_ADDR, (uint8_t *)&CheckSum, 4U);
		if( error!=0 )
		{
			IAP_SendString("Erase err");
		}else{
			IAP_SendString("Erase ok");
		}
	}

	if (error==0)
	{
		
		if(FLASH_WRITE==stat || FLASH_PROGRAM==stat)
		{
			CheckSum = FLASH_RecNWrite(stat);
		}
		else
		{
			CheckSum = FLASH_ReadNTran(stat);
		}
		if(FLASH_WRITE==stat)
		{
			CheckSumToPc[8] = 'W';
		}else if(FLASH_READ==stat)
		{
			CheckSumToPc[8] = 'R';
		}
		MoveValue(CheckSum,CheckSumToPc+9);
		PCLINK_INTF_TransmitFrame(CheckSumToPc,13);

		if(FLASH_PROGRAM==stat && CheckSum)
		{
//	        FLASH_INTF_EraseSectors(APP_EXIST_FLAG_ADDR, 0U);
//			CheckSum= APP_EXIST_FLAG;
//	        FLASH_WriteData(APP_EXIST_FLAG_ADDR, (uint8_t *)&CheckSum, 4U);
		}
	}
	return CheckSum;
}


/**
  * @brief  Display the Main Menu on HyperTerminal
  * @param  None
  * @retval None
  */
void   Main_Menu(void)
{
  DH_U8 key = 0;

  IAP_SendString("\r\n======================================================================");
  IAP_SendString("\r\n=              (C) COPYRIGHT 2017 ZheJiang Dahua Technology CO.,LTD  =");
  IAP_SendString("\r\n=                                                                    =");
  IAP_SendString("\r\n=  HR_C7000 In-Application Programming Application  (Version 1.0.0)  =");
  IAP_SendString("\r\n=                                                                    =");
  IAP_SendString("\r\n=                                           By YUFENG   2017/07/21   =");
  IAP_SendString("\r\n======================================================================");
  IAP_SendString("\r\n\r\n");

  while (1)
  {
    IAP_SendString("\r\n========================= Main Menu ==================================\r\n\n");
    IAP_SendString("  Through UART Download Image To the HR_C7000 CHIP Flash ----------- 1\r\n\n");
	IAP_SendString("  Through USB Download Image To the HR_C7000 CHIP Flash ------------ 2\r\n\n");
    IAP_SendString("  Execute The New Program ------------------------------------------ 3\r\n\n");
    IAP_SendString("======================================================================\r\n\n");

    /* Receive key */
    key = 0x31;//GetKey();

    if (key == 0x31)
    {
		IAP_SendString("\r\n========================== UART boot mode ============================\r\n\n");
		/* Download user application in the Flash */
		if(PC_BLL_Serial_Proc())return;
    }
	
    else if (key == 0x33) /* execute the new program */
    {
		IAP_SendString("\r\n====================== Execute The Program ===========================\r\n\n");
		/* Jump to user application */
		JumpAddress = IAP_APP_ADDR;
		(*(void(*)(void))JumpAddress)();
    }
    else
    {
		IAP_SendString("Invalid Number ! ==> The number should be either 1 or 2\r");
    }
  }
}

uint32  currentPcWriteLong;
int32   FLASH_RecNWrite (DH_U8 stat)
{
	uint32 i=0,  CheckSum = 0, j,writetime = 3;
  	uint32 FlashDestination = 0;
  	uint32 offset  = 0;
	uint16 RxLen_i = 0;
	uint8 erase = 0;
currentPcWriteLong = 0;
	if(stat == FLASH_PROGRAM)//升级程序
  	{
  		FlashDestination = IAP_APP_ADDR;
  		Write_ReadLong++;
  	}
	
	{
		while( 1 )
		{
      RED_LED_ON;
			if (PACKET_PC_SIZE == PCLINK_INTF_ReceiveFrame(&au8FlashBuf[offset], PACKET_PC_SIZE, IAP_COM_WAIT_TIME*25))
			{RED_LED_OFF;
				for(i = offset;i< PACKET_PC_SIZE+offset;i++) {CheckSum = CheckSum + au8FlashBuf[i];}

				currentPcWriteLong++;

				offset = (PACKET_PC_SIZE+offset)%DH_COMM_PAGE_4K;

				if(offset == 0 || currentPcWriteLong >= Write_ReadLong)
				{
					if(stat == FLASH_PROGRAM)
					{
						writetime = 5;
						DH_U8 writestat;
						DH_U8 CheckOK = 0;
						DH_U8 Readtime = 50;
						//while(writetime && CheckOK==0)
						{
							if (FLASH_WriteData(FlashDestination, au8FlashBuf, DH_COMM_PAGE_4K) == LL_OK) {
	                           
	                        }
							else
							{
								Delay_ms(500);
								return 0;
							}
				
//							if(DH_SUCCESS != writestat)
//							{
//								writetime --;
//							}
//							else
//							{
//								Readtime = 5;
//								while(Readtime )
//								{
//									if(DH_SUCCESS == SPI_CAL_ReadFlash(FlashDestination,DH_COMM_PAGE_4K,lau8FlashBuf))
//									{
//										for(j=0;j<DH_COMM_PAGE_4K;j++){
//											if(lau8FlashBuf[j] != au8FlashBuf[j] )
//											{
//												break;
//											}
//										}
//										if(j==DH_COMM_PAGE_4K){
//											CheckOK = 1;
//											Readtime = 0;
//										}
//									}else{
//										Readtime--;
//									}
//								}
//							}
						}
						//if(CheckOK == 0 ) return 0;
					}
					FlashDestination += DH_COMM_PAGE_4K;
				}
				else //单线ICOM在不存储数据时，需延时10ms
				{
					Delay_ms(10);
				}
			

				if(stat == FLASH_PROGRAM)	  PCLINK_INTF_TransmitFrame("kyd",3);
				else if(stat == FLASH_WRITE)  PCLINK_INTF_TransmitFrame("Write",5);

				if(currentPcWriteLong >= Write_ReadLong)/*读取完成*/
				{
					
					Delay_ms(500);
					return CheckSum;
				}else{

				}
			}
			else/*超时退出写频*/
			{RED_LED_OFF;
				PCLINK_INTF_TransmitFrame("TimeOut!", 8);
				Delay_ms(1000);

				return 0;
			}
		}
	}

  	return CheckSum;
}


int32   FLASH_ReadNTran(DH_U8 stat)
{
//	DH_U32 CheckSum=0,i=0;
//	DH_U8  ReadCmd[4]={'R','e','a','d'};
//	DH_U8  flashsel=0,RxLen_i=0;
//	DH_U32 FlashDestination = FlashAddr;
//	DH_U32 currentPcreadLong=0;
//
//	if (FlashAddr & 0x80000000)
//	{
//		FlashDestination = FlashAddr & 0x7FFFFFFF;
//		flashsel = 1;
//	}
//
//	if(FlashDestination==PCWDB_WR_ADDR)//读频
//	{
//		FlashDestination=FLASH_PCWDB_ADDR;
//	}
//
//	while( 1 )
//	{
//		RxLen_i = UART_DRV_Recv (au8FlashBuf,4, IAP_COM_WAIT_TIME*25);
//		if( RxLen_i== 4)
//		{
//			for(i=0;i<4;i++)
//			{
//				if(ReadCmd[i]!=au8FlashBuf[i])
//				{
//					break;
//				}
//			}
//			Delay_ms(10);
//			if(i == 4)/*则是正常的帧数据*/
//			{
//			#if FLASH_STORE_SEL == FLASH_STORE_IN_TC58
//				TC58_INTF_ReadStr(FlashDestination,au8FlashBuf,PACKET_PC_SIZE);
//			#else
//				if (flashsel)
//				{
//					if(g_SecFlashType==FLASH_TC58){
//						TC58_INTF_ReadStr(FlashDestination, au8FlashBuf,
//							PACKET_PC_SIZE);
//					}else{
//						FLASH_INTF_ReadStr(FlashDestination, au8FlashBuf,
//													PACKET_PC_SIZE);
//					}
//				}
//				else
//				{
//					FLASH_PG_INTF_ReadStr(FlashDestination,au8FlashBuf,PACKET_PC_SIZE);
//				}
//			#endif
//				if(1)
//				{
//					UART_DRV_Send(au8FlashBuf,PACKET_PC_SIZE);
//
//					FlashDestination += PACKET_PC_SIZE;
//
//					for(i = 0;i < PACKET_PC_SIZE;i++)  {CheckSum += au8FlashBuf[i];}
//
//					currentPcreadLong ++;
//
//					if(currentPcreadLong%2){
//						RED_LED_OFF;
//						GREEN_LED_ON;
//					}
//					else
//					{
//						RED_LED_ON;
//						GREEN_LED_OFF;
//					}
//					LCD_BLL_LcdWriteStr(83,72,int2hex(CheckSum),lcd_menu);
//					LCD_BLL_LcdWriteStr(43,48,int2Str(currentPcreadLong),lcd_menu);
//					if(currentPcreadLong >= Write_ReadLong)/*读取完成*/
//					{
//						LCD_BLL_LcdWriteStr(MENU_BAR_ROWS,0,"Status:Read Done!   ",lcd_bottem);
//						dhtime_delay(500);
//						return CheckSum;
//					}
//				}
//			}
//		}
//		else/*超时则退出读频*/
//		{
//			LCD_BLL_LcdWriteStr(63,56,int2Str(RxLen_i),lcd_menu);
//			LCD_BLL_LcdWriteStr(MENU_BAR_ROWS,0,"Status:ReadTimeOut!",lcd_bottem);
////			PCLINK_INTF_TransmitFrame("TimeOut!", 8);
//			Delay_ms(1000);
//			return  0;
//		}
//	}
}

//DH_U16   PC_BLL_PcCheckSum(DH_U8 *buf, DH_S16 len)
//{
//#if 1
//    DH_U32 sum=0;
//    
//    while(len >1)
//    {
//        sum += 0xFFFF & (*buf<<8|*(buf+1));
//        buf+=2;
//        len-=2;
//    }
//    
//    if (len)
//    {
//        sum += (0xFF & *buf)<<8;
//    }
//    
//    while (sum>>16)
//    {
//        sum = (sum & 0xFFFF)+(sum >> 16);
//    }
//    
//    return( (DH_U16) sum ^ 0xFFFF);
//#else
//    return 0;
//#endif
//}

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

	PCLINK_INTF_TransmitFrame(DstBuf, 9 + Len);

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
//#define TUNERALLWRITE 1
//	if (PCINFERR_OK==err)
//	{
//		DH_U32 addr = g_ImageIndex[Buf[3]-1][Buf[2]-1];
//		Len -= 9;
//		if((Buf[3] >= 1  && Buf[3] <= 9  && Len == MENU_ICO_SIZE)
//		|| (Buf[3] >= 10 && Buf[3] <= 11 && Len == NUMBER_ICO_SIZE))
//		{
//		if(g_SecFlashType == FLASH_TC58)
//		{
//			TC58_INTF_WriteStr2(addr, (DH_U8*) (Buf + 8), Len);
//		}else{
//			FLASH_INTF_WriteStr(addr, (DH_U8*) (Buf + 8), Len);
//		}
//			PC_BLL_McuToPc(Buf, (DH_U8*) 0, 0, Buf[2], Buf[3],
//				UART_MODE_UIIMG); /* 发送校验ok包 */
//		}
//		else
//		{
//			PC_BLL_McuToPc(Buf, (DH_U8*) 0, 0, TUNERALLWRITE, UFR_RECERR - 4,
//							UART_MODE_UIIMG); /* 发送校验ok包 */
//		}
//		return;
//	}
//	else if (PCINFERR_TIMEOUT == err)
//	{
//		PC_BLL_McuToPc(Buf, (DH_U8*) 0, 0, TUNERALLWRITE, UFR_RECERR - 3,
//				UART_MODE_UIIMG);
//	}
//	else if (PCINFERR_CHECKERR == err)
//	{
//		PC_BLL_McuToPc(Buf, (DH_U8*) 0, 0, TUNERALLWRITE, UFR_RECERR - 1,
//					UART_MODE_UIIMG);
//	}
//	else
//	{
//		PC_BLL_McuToPc(Buf, (DH_U8*) 0, 0, TUNERALLWRITE, UFR_RECERR - 2,
//				UART_MODE_UIIMG);
//	}
}
/*******************************************************************************
 * 函数名: PC_BLL_TunerGetPcFrm
 * 描  述: 获取一帧pc 数据帧
 * 输  入: *DstBuf:数据缓存首地址，PLen:数据帧长度指针	(返回值为ok时有效)
 *         TimeOut:等待时间(为0表示一直等待，OS_TICKS_PER_SEC为1s)，HeadLen:读取的长度
 * 输  出: 无
 * 返回值: 读取结果
 *******************************************************************************/
DH_U8 PC_BLL_TunerGetPcFrm(DH_U8* DstBuf, DH_U16* PLen, DH_U32 TimeOut,
		DH_U32 HeadLen)
{
#if 0
	DH_U16 ChckSum;
	DH_U16 RecLenAll = DstBuf[UART_DATAMODE_LENH] * 0x100
			+ DstBuf[UART_DATAMODE_LENL];
	RecLenAll += (RecLenAll ? 9 : 10);

	if (HeadLen < RecLenAll)
	{
		*PLen = UART_DRV_Recv(DstBuf + HeadLen, RecLenAll - HeadLen,
				TimeOut); /* 获取数据 */
		*PLen += HeadLen;
	}
	if (*PLen) /* 获取到数据 */
	{
		//printk(" %x %x \r\n",DstBuf[0],DstBuf[*PLen-1]);
		if (/**PLen>=9 && */DstBuf[0] == 0x68 && DstBuf[*PLen - 1] == 0x10)
		{
			ChckSum = PC_BLL_PcCheckSum(DstBuf, *PLen);
			if (ChckSum )
			{
				return PCINFERR_CHECKERR;
			}
			else
			{
				return PCINFERR_OK;
			}
		}
		else /* 格式错误 */
		{
			return PCINFERR_FORMATERR;
		}
	}
	else /* 等待超时 */
	{
		return PCINFERR_TIMEOUT;
	}
	#endif
}


/**
  * @}
  */

/*******************(C)COPYRIGHT 2011 STMicroelectronics *****END OF FILE******/
