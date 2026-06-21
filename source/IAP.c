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
#include "IAP.h"
#include "include.h"

//#include "ymodem.h"

//#include <stdio.h>
//#include <stdlib.h>

#define IAP_FW_VER        "V1.08BZSR"
#define FLASH_PROGRAM     1
#define FLASH_WRITE       2
#define FLASH_READ        3
#define FLASH_READPROGRAM 4

#define PCWDB_WR_ADDR     (10 * FLASH_SECTOR_SIZE)

#define IAP_SendString    printk

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
        0};
DH_U8 tab_packet[PACKET_PC_SIZE] =
    {
        0};
//DH_U8 FileName[FILE_NAME_LENGTH];
DH_U8 lau8FlashBuf[DH_COMM_PAGE_4K];

/* Private function prototypes -----------------------------------------------*/
static DH_U32 PC_BLL_Serial_Proc(void);
int32 FLASH_ReadNTran(DH_U8 stat);
DH_U32 BLL_PCIAP_Serial(DH_U8 stat);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Download a file via serial port
  * @param  None
  * @retval None
  */
DH_U32 Get_Command(DH_U8 *key)
{
    while (1)
    {
        if (5 == PCLINK_INTF_ReceiveFrame(key, 5, IAP_COM_WAIT_TIME))
        {

            if (14 == PCLINK_INTF_ReceiveFrame(key + 5, 14, IAP_COM_WAIT_TIME))
                ;
            return 1;
        }
    }
}
void GetHead(DH_U8 *key)
{
    /* Waiting for user input */
    while (1)
    {
        if (Get_Command(key))
        {
            //		UARTRcvClr();
            break;
        }
    }
}

uint8 Option_Command(uint8 *key)
{

    DH_U8 i = 0;
    char str_Erase[6] = {0};
    char str_RWrite[12] = {0};
    for (i = 0; i < 5; i++)
    {
        str_Erase[i] = (char)key[i];
        str_RWrite[i] = (char)key[i];
    }
    for (; i < 11; i++)
    {
        str_RWrite[i] = (char)key[i];
    }
    if (strcmp(str_Erase, "Erase") == 0)
    {
        return FLASH_PROGRAM;
    }
    if (strcmp(str_RWrite, "Flash Write") == 0)
    {
        return FLASH_WRITE;
    }
    if (strcmp(str_RWrite, "Flash Read ") == 0)
    {
        return FLASH_READ;
    }
    return 0;
}

void GetAddrAndLong(uint8 *key)
{
    uint8 i = 0;
    FlashAddr = 0;
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
    //	Write_ReadLong ++;
}

void MoveValue(uint32 CheckSum, uint8 *CheckSumToPc)
{
    CheckSumToPc[0] = CheckSum >> 24;
    CheckSumToPc[1] = CheckSum >> 16;
    CheckSumToPc[2] = CheckSum >> 8;
    CheckSumToPc[3] = CheckSum;
}

DH_U8 BLL_IAP_Flash(DH_U8 *key)
{
    DH_U8 command = Option_Command(key);

    GetAddrAndLong(key);

    //	if(FlashAddr == IAP_APP_ADDR)
    //	{
    //		command = FLASH_READPROGRAM;
    //	}
    if (command != 0)
    {
        //	bDispReflesh = 1;

        if (BLL_PCIAP_Serial(command))
        {
            if (command == FLASH_PROGRAM)
                return 1;
        }
        else
        {
            IAP_SendString("Flash operation fail!\r\n");
        }
    }
    return 0;
}

DH_U32 PC_BLL_Serial_Proc(void)
{
    while (1)
    {
        IAP_SendString("Waiting for the file to be sent ... (press 'a' to abort)\n\r");

        while (1)
        {
            DH_U8 key[20];

            GetHead(key);

            DH_U8 command = Option_Command(key);

            GetAddrAndLong(key);

            if (FlashAddr == IAP_APP_ADDR)
                command = FLASH_READPROGRAM;

            if (command != 0)
            {
                if (BLL_PCIAP_Serial(command))
                {
                    //					IAP_SendString("Flash Write successful!\r\n");
                    if (command == FLASH_PROGRAM)
                        return 1;
                }
                else
                {
                    IAP_SendString("Flash operation fail!\r\n");
                    break;
                }
            }
        }
    }
}

DH_VOID SendDeviveInf(DH_U8 stat)
{
    if (FLASH_PROGRAM == stat)
    {
        IAP_SendString("  IAP");
    }
    else if (FLASH_WRITE == stat)
    {
        IAP_SendString("  Write_2M_");    //UARTSendStr(FW_VER);UARTSendStr(__DATE__);UARTSendStr(__TIME__);
    }
    else if (FLASH_READ == stat)
    {
        IAP_SendString("  Read_2M_");
    }
    IAP_SendString(IAP_FW_VER);
    IAP_SendString(__DATE__);
    IAP_SendString(__TIME__);
}

DH_U32 BLL_PCIAP_Serial(DH_U8 stat)
{
    DH_U32 CheckSum = 0;
    DH_U8 CheckSumToPc[13] = {'C', 'h', 'e', 'c', 'k', 's', 'u', 'm', 'I'};
    int error = 0;

    SendDeviveInf(stat);

    if (FLASH_PROGRAM == stat)
    {
        //	dhtime_delay(500);

        //	uart_clear();
        FLASH_INTF_EraseSectors(IAP_APP_ADDR, Write_ReadLong * PACKET_PC_SIZE);
        //    FLASH_INTF_EraseSectors(APP_EXIST_FLAG_ADDR, 0U);
        //		CheckSum= 0;
        //    FLASH_WriteData(APP_EXIST_FLAG_ADDR, (uint8_t *)&CheckSum, 4U);
        if (error != 0)
        {
            IAP_SendString("Erase err");
        }
        else
        {
            IAP_SendString("Erase ok");
        }
    }

    if (error == 0)
    {

        if (FLASH_WRITE == stat || FLASH_PROGRAM == stat)
        {
            CheckSum = FLASH_RecNWrite(stat);
        }
        else
        {
            CheckSum = FLASH_ReadNTran(stat);
        }
        if (FLASH_WRITE == stat)
        {
            CheckSumToPc[8] = 'W';
        }
        else if (FLASH_READ == stat)
        {
            CheckSumToPc[8] = 'R';
        }
        MoveValue(CheckSum, CheckSumToPc + 9);
        PCLINK_INTF_TransmitFrame(CheckSumToPc, 13);

        if (FLASH_PROGRAM == stat && CheckSum)
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
void Main_Menu(void)
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
        key = 0x31;    //GetKey();

        if (key == 0x31)
        {
            IAP_SendString("\r\n========================== UART boot mode ============================\r\n\n");
            /* Download user application in the Flash */
            if (PC_BLL_Serial_Proc())
                return;
        }

        else if (key == 0x33) /* execute the new program */
        {
            IAP_SendString("\r\n====================== Execute The Program ===========================\r\n\n");
            /* Jump to user application */
            JumpAddress = IAP_APP_ADDR;
            (*(void (*)(void))JumpAddress)();
        }
        else
        {
            IAP_SendString("Invalid Number ! ==> The number should be either 1 or 2\r");
        }
    }
}

uint32 currentPcWriteLong;
int32 FLASH_RecNWrite(DH_U8 stat)
{
    uint32 i = 0, CheckSum = 0;
    uint32 FlashDestination = 0;
    uint32 offset = 0;
    currentPcWriteLong = 0;
    if (stat == FLASH_PROGRAM)
    {
        FlashDestination = IAP_APP_ADDR;
        Write_ReadLong++;
    }

    {
        while (1)
        {
            RED_LED_ON;
            if (PACKET_PC_SIZE == PCLINK_INTF_ReceiveFrame(&au8FlashBuf[offset], PACKET_PC_SIZE, IAP_COM_WAIT_TIME * 25))
            {
                RED_LED_OFF;
                for (i = offset; i < PACKET_PC_SIZE + offset; i++) { CheckSum = CheckSum + au8FlashBuf[i]; }

                currentPcWriteLong++;

                offset = (PACKET_PC_SIZE + offset) % DH_COMM_PAGE_4K;

                if (offset == 0 || currentPcWriteLong >= Write_ReadLong)
                {
                    if (stat == FLASH_PROGRAM)
                    {
                        //while(writetime && CheckOK==0)
                        {
                            if (FLASH_WriteData(FlashDestination, au8FlashBuf, DH_COMM_PAGE_4K) == LL_OK)
                            {
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
                else
                {
                    Delay_ms(10);
                }

                if (stat == FLASH_PROGRAM)
                    PCLINK_INTF_TransmitFrame("kyd", 3);
                else if (stat == FLASH_WRITE)
                    PCLINK_INTF_TransmitFrame("Write", 5);

                if (currentPcWriteLong >= Write_ReadLong)
                {

                    Delay_ms(500);
                    return CheckSum;
                }
                else
                {
                }
            }
            else
            {
                RED_LED_OFF;
                PCLINK_INTF_TransmitFrame("TimeOut!", 8);
                Delay_ms(1000);

                return 0;
            }
        }
    }
}

int32 FLASH_ReadNTran(DH_U8 stat)
{
    (void)stat;
    return 0;
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

DH_VOID PC_BLL_McuToPc(DH_U8 *DstBuf, DH_U8 *SourBuf, DH_U16 Len, DH_U8 Opt,
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

    if (Len)
    {
        memcpy((void *)(DstBuf + 8), (void *)SourBuf, Len);
    }

    CheckSum = PC_BLL_PcCheckSum(DstBuf, 9 + Len);

    DstBuf[4] = CheckSum >> 8;
    DstBuf[5] = CheckSum;

    PCLINK_INTF_TransmitFrame(DstBuf, 9 + Len);

    return;
}

DH_VOID PC_BLL_PcToTC58Img(DH_U8 *Buf, DH_U16 Len, DH_U8 err)
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

    //		}
    //		else
    //		{
    //			PC_BLL_McuToPc(Buf, (DH_U8*) 0, 0, TUNERALLWRITE, UFR_RECERR - 4,

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

DH_U8 PC_BLL_TunerGetPcFrm(DH_U8 *DstBuf, DH_U16 *PLen, DH_U32 TimeOut,
                           DH_U32 HeadLen)
{
    (void)DstBuf;
    (void)PLen;
    (void)TimeOut;
    (void)HeadLen;
    return PCINFERR_TIMEOUT;
}

/**
  * @}
  */

/*******************(C)COPYRIGHT 2011 STMicroelectronics *****END OF FILE******/
