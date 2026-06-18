/*****************************************************************************
 *   文件名:PcInfmSet.c
 *	 文件描述:  写频软件接口函数
 *   创建人：陶林，2011/07/29
******************************************************************************/
#include "config.h"

#include "INTF_FLASH.h"
#include "INTF_PCLINK.h"
#include "DRV_GPIO.h"
#include "BLL_PC.h"
#include "BLL_LCD.h"
#include "BLL_PCINFMSET.h"
#include "APP_MENU_FONT.h"
#include "APP_MENU_ZONESET.h"
#include "APP_MENU_LINKMAN.H"
#include "APP_MENU_MSGAPI.h"
#include "APP_MENU_MENUSAVE.h"

#include "HRC_OS.h"

/* 写频/读频发送的数据包的最大包数 */
const DH_U32 PCDATAFRAMELEN[10] =
{
    PCDATAEQIPINFOLEN,
    PCDATABASESETLEN,
    PCDATAAREASETLEN,
    PCDATAEXIALARMLEN,
    PCDATASCANLEN,
    PCDATA_LM_LEN,
    PCDATAREGGROUPLISTLEN,
    PCDATADMRSERVERLEN,
};
const DH_U32 PCWDBFRAMEADDR[10] =
{
    BA_EQIPINFO,
    BA_BASESET,
    BA_AREASET,
    BA_EXIALARM,
    BA_SCAN,
    BA_LM,
    BA_REGGROUPLIST,
    BA_DMRSERVER,
};

#define PCDATAMAXF(cmd) ((PCDATAFRAMELEN[cmd]%UARTDATALEN!=0) ? (PCDATAFRAMELEN[cmd]/UARTDATALEN+1):(PCDATAFRAMELEN[cmd]/UARTDATALEN))

/* 标记当前所处状态 */
STR_PCINF StrPcInf = {PCINF_IDLE};

/*****************************************************************************
 *函数名: PC_BLL_RecPcData
 *描  述: 写频软件接收处理接口函数
 *输  入: *RecDataBuf:接收到的数据首地址，HeadLen:接收到的数据长度
 *输  出: 无
 *返回值: 无
******************************************************************************/
DH_VOID PC_BLL_RecPcData(DH_U8* RecDataBuf, DH_U32 HeadLen)
{
#if 0
	DH_U16 RecDataLen;							/* 接收数据长度 */
	//DH_U16 CheckSum;
	DH_U8 err;
	DH_U32 RxLen;
	
	do
	{
		/* 判断当前所处状态，进行数据接收 */
		if (StrPcInf.Stat == PCINF_IDLE)
		{
			err = PC_BLL_PcGetFrm(RecDataBuf, &RecDataLen, UART_RX_CON_WAITT, HeadLen);
		}
		else if (StrPcInf.Stat == PCINF_WRITE)	/* 写频时获取数据 */
		{
			RxLen = PCLINK_INTF_ReceiveFrame (RecDataBuf, UART_RX_FIRST_I, PCWAITTOUTWRITE);
			if ((RxLen>=UART_RX_FIRST_I) && RecDataBuf[UART_DATAMODE_HEAD]==0x68)
			{
				err = PC_BLL_PcGetFrm(RecDataBuf, &RecDataLen, UART_RX_CON_WAITT, RxLen);
			}
			else
			{
				StrPcInf.Stat = PCINF_IDLE;	
				return;
			}
		}
		else /* 读频时获取数据 */
		{
			RxLen = PCLINK_INTF_ReceiveFrame (RecDataBuf, UART_RX_FIRST_I, PCWAITTOUTREAD);
			if ((RxLen>=UART_RX_FIRST_I) && RecDataBuf[UART_DATAMODE_HEAD]==0x68)
			{
				err = PC_BLL_PcGetFrm(RecDataBuf, &RecDataLen, UART_RX_CON_WAITT, RxLen);
			}
			else
			{
				StrPcInf.Stat = PCINF_IDLE;	
				return;				
			}
		}	
		if (StrPcInf.Stat == PCINF_IDLE)
		{
		    //HRC_BLL_UartPrint(RecDataBuf, 12, 1);
		    if(PCINFERR_OK != err)
		    {
		        printk("err: %d.\r\n", err);
		    }
		    
			if (PCINFERR_OK == err && RecDataBuf[1]==PCCMDTYPE)	/* 格式正确 */
			{
				if(RecDataBuf[2] == 0x02)               /* 克隆 */
				{
					StrPcInf.Stat = PCINF_WRITE;
					StrPcInf.Clone = 1;
				}
				else
				{
					if (RecDataBuf[3]==UFW_WRITEDATA) 	/* 写频 */
					//&& RecDataBuf[8]==0)					/* 包序号为0 */
					{
						StrPcInf.Stat = PCINF_WRITE;
					}
					else if (RecDataBuf[3]==UFW_REQREAD)	/* 读频 */
					{
						StrPcInf.Stat = PCINF_READ;
					}
					else if (RecDataBuf[3]==UFW_BASEINFO)	/* 加密等基本信息 */
					{
						StrPcInf.Stat = PCINF_CIPGER;
					}
					StrPcInf.Clone = 0;
				}
				StrPcInf.Cnt = 0;
				StrPcInf.ReSRFlg = 0;
			}
		}

		//printk("pc state: %d.\r\n",StrPcInf.Stat );

		if (StrPcInf.Stat == PCINF_WRITE)
		{
		    //printk("pc write.\r\n");
			//PC_BLL_PcWirteData(RecDataBuf, RecDataLen, err);
		}
		else if (StrPcInf.Stat == PCINF_READ)
		{
			PC_BLL_PcReadData(RecDataBuf, RecDataLen, err);
		}
		else if (StrPcInf.Stat == PCINF_CIPGER)
		{
		    //printk("pc info.\r\n");
			PC_BLL_PcReqInfo(RecDataBuf);
		}	
	}while(StrPcInf.Stat != PCINF_IDLE);
#endif
}

/*****************************************************************************
 *函数名: PcWirteData
 *描  述: 从pc接收写频数据的处理函数
 *输  入: Buf:接收数据缓存，Len:接收数据长度；
 *输  出: err:错误信息
 *返回值: 无
******************************************************************************/
DH_VOID PC_BLL_PcWirteData(DH_U8* Buf, DH_U16 Len, DH_U8 err)
{  
#if 0
    DH_U8 cmd = Buf[8];

    //printk("err: %d, cmd: %d, len: %d.\r\n", err, cmd, Len);
    
	if (PCINFERR_OK==err || Buf[3]==UFW_WRITEDATA)
	{          	
	    if((cmd==DATATYPE_DMRINFO)&&StrPcInf.Clone)
	    {
	        //MenuCloneRead();
	    }
		if (PCDATAMAXF(cmd)-1 <= Buf[UARTFORMATPACKETNUM])			/* 写频数据接收完成，退出 */
		{  
		    if(cmd == DATATYPE_FIVETONE)    /* 克隆时包号有错误故作了特殊处理(适用于不超过64个字节) */
		    {
		        Buf[UARTFORMATPACKETNUM] = 0;
		    }
			if (Len>UARTFORMATLEN+1)						/* 接收数据 */
			{
				FLASH_INTF_WriteStr(g_FlashSaveFlg,PCWDBFRAMEADDR[cmd]+Buf[UARTFORMATPACKETNUM]*UARTDATALEN, (DH_U8*)(Buf+UARTFORMATBADDR), Len-UARTFORMATLEN-1);
            }			
			PC_BLL_PcMFSend (Buf, 0, 0, 0, (DH_U8*)0, 0, UFR_RECOK);	/* 发送校验ok包 */
      
			if(cmd >= FRAMEPACKETTOTAL-1)
			{
    			/* 进行写频数据处理，重启系统 */
    			if(StrPcInf.Clone)
    			{
    			   //MenuCloneWrite();
    			   StrPcInf.Clone = 0;
    			}

    			PC_BLL_PcWirteOk();

    			return;
			}
		}
		else
		{			
			FLASH_INTF_WriteStr(g_FlashSaveFlg,PCWDBFRAMEADDR[Buf[8]]+Buf[UARTFORMATPACKETNUM]*UARTDATALEN, (DH_U8*)(Buf+UARTFORMATBADDR), UARTDATALEN);
			PC_BLL_PcMFSend (Buf, 0, 0, 0, (DH_U8*)0, 0, UFR_RECOK);	/* 发送校验ok包 */
		}
		StrPcInf.ReSRFlg = 0;
	}
	else if (PCINFERR_TIMEOUT == err)
	{
		PC_BLL_PcMFSend (Buf, 0, 0, 0, (DH_U8*)0, 0, UFR_RECSTOP1);
		StrPcInf.Stat = PCINF_IDLE;
	}
	else if (PCINFERR_CHECKERR == err)
	{
		/* 超过最大重传次数，发送停止发送包，并退出 */
		if (++StrPcInf.ReSRFlg >= PCRESENDMAX) 	
		{
			PC_BLL_PcMFSend (Buf, 0, 0, 0, (DH_U8*)0, 0, UFR_RECSTOP1);
			StrPcInf.Stat = PCINF_IDLE;
		}
		else
		{
			PC_BLL_PcMFSend (Buf, 0, 0, 0, (DH_U8*)0, 0, UFR_RECERR);
		}
	}
	else //if (PCINFERR_FORMATERR == err)
	{
		PC_BLL_PcMFSend (Buf, 0, 0, 0, (DH_U8*)0, 0, UFR_RECSTOP1);
		StrPcInf.Stat = PCINF_IDLE;
	}
#endif
}
/*****************************************************************************
 *	函数名: PC_BLL_PcWirteOk
 *	功能描述: 写频完成处理函数
 *	参数定义:
 *	返回值:
 *	创建人：陶林，2011/08/01
******************************************************************************/
DH_VOID PC_BLL_PcWirteOk(DH_VOID)
{
	DH_U16 row = Str_LCD_Disp.lcd_size_r*5/8-8;
	LCD_BLL_LcdClearArea(row,(LCD_COL_MAX/2-80),24,160,LCD_Orange);
	LCD_BLL_LcdWriteMid (row, lcd_mes, (DH_U8*)*(StrPCwriteData[StrMenuSet.language].Text+0));
	LM_APP_LMConsFPc();
	MSGAPI_APP_ConsComMsgFPc();
	CHN_APP_GetZoneInfo();
	LCD_BLL_LcdClearArea(row,(LCD_COL_MAX/2-80),24,160,LCD_Orange);
	LCD_BLL_LcdWriteMid (row, lcd_mes, (DH_U8*)*(StrPCwriteData[StrMenuSet.language].Text+1));

}
/*****************************************************************************
 *	函数名: PcRDSndData
 *	功能描述: 发送读频数据
 *	参数定义:Buf:接收数据缓存
 *			        :Len:接收数据长度；
 *				  :err:错误信息
 *	返回值:
 *	创建人：陶林，2011/08/01
******************************************************************************/
DH_VOID PC_BLL_PcRDSndData(DH_U8* Buf, DH_U8 Cnt)
{
#if 0
	DH_U8 SendBuf[UARTDATALEN];	
	
	if (Cnt == PCDATAMAXF(StrPcInf.Type)-1)	/* 最后一包数据 */
	{
		DH_U8 Len;	/* 待发送数据长度 */

		Len = PCDATAFRAMELEN[StrPcInf.Type]%UARTDATALEN;
		if (Len==0)
		{
			Len = UARTDATALEN;
		}
		FLASH_INTF_ReadStr(g_FlashSaveFlg,PCWDBFRAMEADDR[StrPcInf.Type]+UARTDATALEN*Cnt, SendBuf, Len);
        dhtime_delay(100);//100ms
		PC_BLL_PcMFSend(Buf, StrPcInf.Type, PCDATAMAXF(StrPcInf.Type), Cnt, SendBuf, Len, UFR_READDATA);
		StrPcInf.Type++;
	}
	else if (Cnt < PCDATAMAXF(StrPcInf.Type))
	{
		FLASH_INTF_ReadStr(g_FlashSaveFlg,PCWDBFRAMEADDR[StrPcInf.Type]+UARTDATALEN*Cnt, SendBuf, UARTDATALEN);
		PC_BLL_PcMFSend(Buf, StrPcInf.Type, PCDATAMAXF(StrPcInf.Type), Cnt, SendBuf, UARTDATALEN, UFR_READDATA);
	}
	else
	{
		StrPcInf.Stat = PCINF_IDLE;
	}
#endif
}

/*****************************************************************************
 *函数名: PC_BLL_PcReadData
 *描  述: 从pc接收读频数据的处理函数
 *输  入: *Buf:接收数据缓存，Len:接收数据长度；
 *输  出: err:错误信息
 *返回值: 无
******************************************************************************/
DH_VOID PC_BLL_PcReadData(DH_U8* Buf, DH_U16 Len, DH_U8 err)
{
	
	/*if (PCINFERR_OK == err)
	{
		if (Buf[3]==UFW_REQREAD)
		{
			StrPcInf.Cnt = 0;
			StrPcInf.Type = 0;
			StrPcInf.LastType = 0;
			
			 读频数据准备
			//LM_APP_LM2Pc();//重启之后，连续人数据会乱，使用待确定
			//MSGAPI_APP_SaveComMsgTPc();

			PC_BLL_PcRDSndData(Buf, StrPcInf.Cnt);
		}
		else if (Buf[3]==UFW_RECOK)
		{
			if(StrPcInf.Type > (FRAMEPACKETTOTAL-1))
			{
				StrPcInf.Stat = PCINF_IDLE;
				return ;
			}
			
			if(StrPcInf.LastType!=StrPcInf.Type)
			{
				StrPcInf.Cnt=0;
				StrPcInf.LastType = StrPcInf.Type;
			}
			else
			{
				StrPcInf.Cnt++;
			}
			
			StrPcInf.ReSRFlg = 0;
			PC_BLL_PcRDSndData(Buf, StrPcInf.Cnt);
		}
		else if (Buf[3]==UFW_RECERR)
		{
			PC_BLL_PcRDSndData(Buf, StrPcInf.Cnt);
		}
		else
		{
			StrPcInf.Stat = PCINF_IDLE;
		}
		
	}
	else if (PCINFERR_TIMEOUT==err || PCINFERR_FORMATERR==err ||
		PCINFERR_CHECKERR==err)
	{
		if (++StrPcInf.ReSRFlg > PCRESENDMAX)	 超过重传次数
		{
			StrPcInf.Stat = PCINF_IDLE;
		}
		else
		{
			PC_BLL_PcRDSndData(Buf, StrPcInf.Cnt);
		}
	}
	else 
	{
		StrPcInf.Stat = PCINF_IDLE;
	}	*/
}

/*****************************************************************************
 *函数名: PC_BLL_PcReqInfo
 *描  述: 向pc写密码和型号信息
 *输  入: Buf:接收数据缓存
 *输  出: 无
 *返回值: 无
******************************************************************************/
DH_VOID PC_BLL_PcReqInfo(DH_U8* Buf)
{
#if 0
	DH_U16 Len=0;
	DH_U8 PassWordSwitch = FLASH_INTF_ReadByte(g_FlashSaveFlg,PCBA_WRITECIPHERSW);
    DH_U8 BaseInfoBuf[PCWR_DATALEN+2*PCWR_CIPHERLEN+2] = {0};

    FLASH_INTF_ReadStr(g_FlashSaveFlg,PCWDBADDR+PCWR_BASEEXC, BaseInfoBuf+1, PCWR_DATALEN);

    if(PassWordSwitch&0xE0)
    {
        /* 第一次写频时防止弹出写频密码 */
        PassWordSwitch = 0;   
    }
    
	if (Buf[UARTFORMATBADDR - 1] == UARTCNTFORMAT_WRITE)
	{
		FLASH_INTF_ReadStr(g_FlashSaveFlg,PCBA_WRITECIPHER, BaseInfoBuf+1+PCWR_DATALEN+PCWR_CIPHERLEN, PCWR_CIPHERLEN);
        BaseInfoBuf[0] = UARTCNTFORMAT_WRITE;
		BaseInfoBuf[67] = PassWordSwitch & 0x01;
		Len = PCWR_DATALEN+2*PCWR_CIPHERLEN+2;
	}
	else if (Buf[UARTFORMATBADDR - 1] == UARTCNTFORMAT_READ)
	{	
		FLASH_INTF_ReadStr(g_FlashSaveFlg,PCBA_READCIPHER, BaseInfoBuf+1+PCWR_DATALEN+PCWR_CIPHERLEN, PCWR_CIPHERLEN);
        BaseInfoBuf[0] = UARTCNTFORMAT_READ;
		BaseInfoBuf[67] = PassWordSwitch & 0x02;
		Len = PCWR_DATALEN+2*PCWR_CIPHERLEN+2;
	}
	else
	{
		Len = PCWR_DATALEN;
	}

    PC_BLL_PcMFSend (Buf, 0, 0, 0, (DH_U8*)BaseInfoBuf, Len, UFR_BASEINFO);

    //HRC_BLL_UartPrint(BaseInfoBuf, Len, 1);
    
	StrPcInf.Stat = PCINF_IDLE;		/* 只要进入本函数，则读写频密码信息就结束 */
	return;
#endif
}

/*****************************************************************************
 *函数名: PC_BLL_PcMFSend
 *描  述: 组pc 帧,并发送
 *输  入: DstBuf:目标指针；Type: Packet: PacketNum: 
 *		  SourBuf:源地址，len:SourBuf长度, Opt:设置类型
 *输  出: 无
 *返回值: 无
******************************************************************************/
DH_VOID PC_BLL_PcMFSend(DH_U8* DstBuf, DH_U8 Type, DH_U8 Packet, DH_U8 PacketNum, DH_U8* SourBuf, DH_U16 Len, DH_U8 Opt)
{
	DH_U16 CheckSum;
	
	DstBuf[0] = 0x68;
	DstBuf[1] = PCCMDTYPE;
	DstBuf[2] = UF_READ;
	DstBuf[3] = Opt;
	
	DstBuf[4] = DstBuf[5] = 0;			/* check sum */
	
	DstBuf[6] = Len%256;
	DstBuf[7] = Len/256;

	DstBuf[8] = Type;
	DstBuf[9] = Packet;
	DstBuf[10] = PacketNum;

	if (Len)
	{
		memcpy ((DH_VOID*)(DstBuf+11), (DH_VOID*)SourBuf, Len);
	}

	DstBuf[11+Len] = 0x10;

	/* 填充校验位 */
	CheckSum = PC_BLL_PcCheckSum(DstBuf, 12+Len);

	DstBuf[4] = CheckSum>>8;
	DstBuf[5] = CheckSum;	

	PCLINK_INTF_TransmitFrame(DstBuf, 12+Len);

	return;
}

/*****************************************************************************
 *函数名: PC_BLL_PcGetFrm
 *描  述: 获取一帧pc 数据帧
 *输  入: DstBuf:目标指针；(返回值为ok时有效), PLen:数据帧长度指针(返回值为ok时有效)
 *		  TimeOut:等待时间(为0表示一直等待，OS_TICKS_PER_SEC为1s)
 *输  出: 无
 *返回值: 结果
******************************************************************************/
DH_U8 PC_BLL_PcGetFrm( DH_U8* DstBuf, DH_U16* PLen, DH_U32 TimeOut, DH_U32 HeadLen)
{
	DH_U16 ChckSum;
	DH_U16 RecLenAll = (DstBuf[UART_DATAMODE_LENH]<<8)+DstBuf[UART_DATAMODE_LENL]+12;
#if 1	
	if (HeadLen<RecLenAll)
	{
		*PLen = PCLINK_INTF_ReceiveFrame (DstBuf+HeadLen, RecLenAll-HeadLen, TimeOut);		/* 获取数据 */
		*PLen += HeadLen;
		
	}

	//HRC_BLL_UartPrint(DstBuf, *PLen, 1);

	if (*PLen)			/* 获取到数据 */
	{
		if (*PLen>=UARTFORMATLEN && DstBuf[0]==0x68 && DstBuf[*PLen-1]==0x10)
		{
			ChckSum = PC_BLL_PcCheckSum(DstBuf, *PLen);

			if (ChckSum)
			{
				return PCINFERR_CHECKERR;
			}
			else
			{
				return PCINFERR_OK;
			}
		}
		else			/* 格式错误 */	
		{
		    //printk("head: %x, term:%x, len:%d", DstBuf[0], DstBuf[*PLen-1], *PLen);
			return PCINFERR_FORMATERR;
		}
	}
	else				/* 等待超时 */
	{
		return PCINFERR_TIMEOUT;
	}
#else
  return PCINFERR_OK;
#endif
}

