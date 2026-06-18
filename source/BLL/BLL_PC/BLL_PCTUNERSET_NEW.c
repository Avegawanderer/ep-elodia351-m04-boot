/*****************************************************************************
 *   文件名:PcTunerSet.c
 *   文件描述:射频调测软件
 *   创建人：yufeng	, 2012/11/18
 ******************************************************************************/
#include "APP_config.h"
#include "include.h"

GLOVAR_FLAG GloVarFlag={0};
STR_MENU_SET StrMenuSet={
	.ScanMode = 1,
	.PowerMTime = 15,	

	};
ALL_FLAG AllFlag;

extern SQL_STATUS_S gstSql;




#if 0
#else

#define PCTUNERCNTMAX		  4
#define PUMP 	              31		/*磅电流*/
#define DIV 		          2			/*预分频*/
//#define TASKRF_PRIOPCTUNER  TASK_HRC5000_RF_PRIOR_

static DH_U8 ResCnt = 0;
DH_U8 FreqCheckBuf[6] =
{ 0 };
DH_U8 ErrNumbTestFlag = 0;
DH_U8 Sq_flag = 0; /*是否处于调试状态设定SQ在不同等级下的th_l,th_h*/
DH_U8 PcTunerFlg; /*0为无设置发射频率的调试，1为需要发射射频*/
DH_U8 g_PcTunerFeqSelect = 0; /*频率的设置防止最后一段读取调试数据时出错*/

/* 当前菜单状态的结构体变量 */
typedef struct
{
	DH_U16 FreqChexkse    :1;    /*0为正处于接收状态，1为正处于发射状态*/
	DH_U16 HighPowerset   :1;    /*0为正处于接收状态，1为正处于发射状态*/
	DH_U16 LowPowerset    :1;    /*0为正处于接收状态，1为正处于发射状态*/
	DH_U16 Dmodbalanceset :1;    /*0为正处于接收状态，1为正处于发射状态*/
	DH_U16 RF_OUTset      :1;    /*0为正处于接收状态，1为正处于发射状态*/
	DH_U16 FreqBuf        :4;    /*调试频率段位*/
	DH_U16 Reverse        :7;
} STR_PCTUNERTXSTAT;
STR_PCTUNERTXSTAT Str_PCctunertxstat;



extern DH_U16 SignalBuf[];
extern const DH_U32 g_u32FreqSectorList[10];
extern DH_U16 g_SqLevelArr[10];


void PC_BLL_FlashReadW(DH_U8 * Buf, DH_U8 Len, DH_U8 err);

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
	DH_U16 ChckSum;
	DH_U16 RecLenAll = DstBuf[UART_DATAMODE_LENH] * 0x100
			+ DstBuf[UART_DATAMODE_LENL];
	RecLenAll += (RecLenAll ? 9 : 10);

	if (HeadLen < RecLenAll)
	{
		*PLen = PCLINK_INTF_ReceiveFrame(DstBuf + HeadLen, RecLenAll - HeadLen,
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
//				LCD_BLL_printf_ext(24,0,lcd_menu,"%d.%d",ChckSum,DstBuf[5] * 0x100
//						+ DstBuf[4]);
				return PCINFERR_CHECKERR;
			}
			else
			{
//				LCD_BLL_printf_ext(24,0,lcd_menu,"ChckSumOK");
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
}

/*******************************************************************************
 *函数名:PcTunerRecData
 *函数说明:接收PC调测数据
 *参数说明:
 *返回值:无
 *创建人:yufeng
 *创建日期:2012/11/18
 *******************************************************************************/
void PC_BLL_PcTunerRecData(DH_U8* RecBuf, DH_U32 HeadLen)
{
	DH_U8 err;
	DH_U16 RecLen;
	DH_U8 tempbuf = RecBuf[2];

	err = PC_BLL_TunerGetPcFrm(RecBuf, &RecLen, UART_RX_CON_WAITT / 4, HeadLen);

#if	FREQ_SCOPE_UV == FREQ_SCOPE_V
	if(0x31 >= RecBuf[2] && RecBuf[2] >= 0x10)//U段命令
	{
		return ;
	}
	else if (0x90 <= RecBuf[2] && RecBuf[2] <= 0xB1)//V段命令
	{
		tempbuf -= 0x80;
	}
#else //
	if (0x90 <= RecBuf[2] && RecBuf[2] <= 0xB1)//V段命令
	{
		return;
	}
#endif

	if ((tempbuf >= 0x10 && tempbuf <= 0x13)
			||tempbuf == 0x18
			|| (tempbuf >= 0x20 && tempbuf <= 0x23))
	{
		if ((tempbuf >= 0x10 && tempbuf <= 0x13 ) ||tempbuf == 0x18)
		{
			
		}
	}
	else
	{
		if ((tempbuf >= 0x14 && tempbuf <= 0x17)||tempbuf==SQLEVEL1OPENSET)
		{
			if (CHN_BLL_GetAttr() != CHNSTAT_FM)
			{
			//	CHN_BLL_SetAttr(CHNSTAT_FM);
			//	HRC_BLL_RFRXCHN_SW(CHN_BLL_GetAttr());
				RF_BLL_RxInit(RF_BLL_GetPllStrP());
			//	HRC_BLL_SetDmrFmMode();
			}
//			*WORK_MODE = 0x80;                     //FM 模式
		}
	}

	if (tempbuf == RDA1846SREGSET)                     //1846寄存器设置
	{
	//	PC_BLL_PcOption1846S(RecBuf);
		return;
	}
//	if (tempbuf == C5000REGSET)                     //5000寄存器设置
//	{
//		//PC_BLL_PcOptionC5000S(RecBuf);
//		return;
//	}
	if (tempbuf == P_FLASH_OPT)                     //
	{
		PC_BLL_PcOptionFlash(RecBuf);
		return;
	}





	//if (RecBuf[3] == 2)
	//	GloVarFlag.UVDebugFlg = 1;


	switch (tempbuf)
	{	
	case FLASH_RW:
		PC_BLL_FlashReadW(RecBuf, RecLen, err);
		break;
	case TUNERALLWRITE:
		PC_BLL_PcToMcuTuner(RecBuf, RecLen, err);
		break;
	case TUNERALLREAD:
		PC_BLL_McuToPcTuner(RecBuf, RecLen, err);
		break;
	//case FREQCHEXKSET:
	//	PC_BLL_PcFreqCheckSet(RecBuf, RecLen, err);
	//	break;
	case HIGHPOWERSET:
		PC_BLL_PcHighPowerSet(RecBuf, RecLen, err);
		break;
	case LOWPOWERSET:
		PC_BLL_PcLowPowerSet(RecBuf, RecLen, err);
		break;
//	case DMODBALANCESET:                     //数字调制频偏的设置
//		PC_BLL_PcDModBalanceSet(RecBuf, RecLen, err);
//		break;
//	case MDEVNARROWSET:
//		PC_BLL_PcMDevNarrowSet(RecBuf, RecLen, err);
//		break;
//	case MDEVWIDESET:
//		PC_BLL_PcMDevWideSet(RecBuf, RecLen, err);
//		break;
//	case MTPLDPLNARROWSET:
//		PC_BLL_PcMTPLDPLNarrowSet(RecBuf, RecLen, err);
//		break;
//	case MTPLDPLWIDESET:
//		PC_BLL_PcMTPLDPLWideSet(RecBuf, RecLen, err);
//		break;
//	case TVSET:
//		PC_BLL_PcTvSet(RecBuf, RecLen, err);
//		break;
	case RSSITEST:
		PC_BLL_PcReadRssi(RecBuf, RecLen, err);
		break;
//	case ERRNUMRATTEST:
//		PC_BLL_PcReadErrNumRat(RecBuf, RecLen, err);
//		break;
//	case VOLUMESET:
//		PC_BLL_PcVolumSet(RecBuf, RecLen, err);
//		break;
//	case SQLEVEL1CLOSESET:
//		PC_BLL_PcSQLevel1CloseSet(RecBuf, RecLen, err);
//		break;
//	case SQLEVEL9OPENSET:
//		PC_BLL_PcSQLevel9OpenSet(RecBuf, RecLen, err);
//		break;
//	case SQLEVEL9CLOSESET:
//		PC_BLL_PcSQLevel9CloseSet(RecBuf, RecLen, err);
//		break;
//	case CPUFREQADJUST:
//		PC_BLL_CPUFreqAdjust(RecBuf, RecLen, err);
//		break;
//	case APCADJUST:
//		PC_BLL_ApcAdjustSet(RecBuf, RecLen, err);
//		break;
//		case SQLEVELSET:
	case SQLEVEL1OPENSET:
		PC_BLL_SqAdjustSet(RecBuf, RecLen, err);
		break;
//	case RSSILEVELSET:
//		PC_BLL_RSSIAdjustSet(RecBuf, RecLen, err);
//		break;
//	case DIGITALRSSILEVELSET:
//		PC_BLL_Digital_RSSISet(RecBuf, RecLen, err);
//		break;
//	case RDA1846PASET:
//		PC_BLL_Pc1846PaOutSet(RecBuf, RecLen, err);
//		break;
//	case MODEIQSET:
//		PC_BLL_ModeIQSet(RecBuf, RecLen, err);
//		break;
//	case FMRXDETECTSET:
//		PC_BLL_FMRxDetectset(RecBuf, RecLen, err);
//		break;
//	case FMTXDETECTSET:
//		PC_BLL_FMTxDetectset(RecBuf, RecLen, err);
//		break;
//	case READ_GPS_MSG:
//		PC_BLL_PcOptionGPSmsg(RecBuf);
//		break;
	default:
		break;
	}
#if 0
	if(RecBuf[2] == RSSITEST || RecBuf[2] == ERRNUMRATTEST)
	{
		if(flg)
		{
			RecBuf[8] = 1;
			PcReadErrNumRat(RecBuf, RecLen, err);
			flg = 0;
		}
	}
	else
	{
		if(!flg)
		{
			RecBuf[8] = 0;
			PcReadErrNumRat(RecBuf, RecLen, err);
			flg = 1;
		}
	}
#else
//	if (RecBuf[2] == ERRNUMRATTEST)/*接收 O.153和发射O.153误码率的测试*/
//		PC_BLL_PcReadErrNumRat(RecBuf, RecLen, err);
#endif
}

const DH_U8 tunestr[] = {'T','U','N','E'};
//开机检测调试参数
void PC_BLL_TuneParamChk(DH_VOID)
{
#if 0
	DH_U8 buf1[8];
	DH_U8 buf2[8];
	DH_U32 num1,num2;
	DH_U32 bkaddr = FLASH_PROG_PCTUNER_BK-PARAM_BASE;
	TC58_INTF_ReadStr(FLASH_PCTUNER_U_ADDR,buf1,8);
	FLASH_INTF_ReadStr(FLASH_IC_PROG,bkaddr,buf2,8);
	if(memcmp(buf1,buf2,8)==0 && memcmp(buf1,tunestr,4)==0)
	{
		printk("==\r\n");
	}
	else
	{
		if(memcmp(buf1,tunestr,4)==0)//存在调试参数
		{
			if(memcmp(buf2,tunestr,4)!=0)//bu存在调试参数
			{
				printk("TC58==>PROG\r\n");
				TC58_INTF_ReadStr(FLASH_PCTUNER_U_ADDR,FlashBuff,2048);
				FLASH_INTF_WriteStr(FLASH_IC_PROG,bkaddr,FlashBuff,FLASH_SECTOR_SIZE);
			}
			else//同时存在进行判断并同步
			{
				memcpy(&num1,&buf1[4],4);
				memcpy(&num2,&buf2[4],4);
				if(num1>num2)
				{
					printk("TC58==>PROG\r\n");
					TC58_INTF_ReadStr(FLASH_PCTUNER_U_ADDR,FlashBuff,2048);
					FLASH_INTF_WriteStr(FLASH_IC_PROG,bkaddr,FlashBuff,FLASH_SECTOR_SIZE);
				}
			}
		}
		else//调试参数丢失或未调试
		{
			if(memcmp(buf2,tunestr,4)==0)//存在调试参数备份
			{
				printk("PROG==>TC58\r\n");
				FLASH_INTF_ReadStr(FLASH_IC_PROG,bkaddr,FlashBuff,2048);
				TC58_INTF_SetOptEnable();
				TC58_INTF_BlockErase(FLASH_PCTUNER_U_ADDR);
				TC58_INTF_WriteStr(FLASH_PCTUNER_U_ADDR,FlashBuff,2048);
				TC58_INTF_SetOptDisable();
			}
			else
			{
				FlashBuf2Lock();
				TC58_INTF_ReadStr(FLASH_PCTUNER_U_ADDR,FlashBuff,2048);

				if(memcmp(FlashBuff,tunestr,4)==0)//存在调试参数
				{
					memcpy(&num1,&FlashBuff[4],4);
					num1 += 1;
				}
				else//不存在，次数1
				{
					memcpy(FlashBuff,tunestr,4);
					num1 = 1;
				}
				memcpy(&FlashBuff[4],&num1,4);

				TC58_INTF_SetOptEnable();
				TC58_INTF_BlockErase(FLASH_PCTUNER_U_ADDR);
				TC58_INTF_WriteStr(FLASH_PCTUNER_U_ADDR,FlashBuff,2048);
				TC58_INTF_SetOptDisable();

				FLASH_INTF_WriteStr(FLASH_IC_PROG,bkaddr,FlashBuff,FLASH_SECTOR_SIZE);
				FlashBuf2UnLock();
				printk("No tune\r\n");
			}
		}
	}
	#endif
}


//写入外部TC58flash，为减少写入时间
void PC_BLL_TuneWriteStr(DH_U32 Addr,DH_U8* Buf, DH_U16 Len)
{
	DH_U32 temp;
	#if 0
	if(flg==FLASH_IC_TC58)
	{
		FlashBuf2Lock();
		TC58_INTF_ReadStr(FLASH_PCTUNER_U_ADDR,FlashBuff,2048);
		memcpy(FlashBuff+(Addr-FLASH_PCTUNER_U_ADDR),Buf,Len);

		if(memcmp(FlashBuff,tunestr,4)==0)//存在调试参数
		{
			memcpy(&temp,&FlashBuff[4],4);
			temp += 1;
		}
		else//不存在，次数1
		{
			memcpy(FlashBuff,tunestr,4);
			temp = 1;
		}
		memcpy(&FlashBuff[4],&temp,4);

		TC58_INTF_SetOptEnable();
		TC58_INTF_BlockErase(Addr);
		TC58_INTF_WriteStr(FLASH_PCTUNER_U_ADDR,FlashBuff,2048);
		TC58_INTF_SetOptDisable();
		FlashBuf2UnLock();
	}
	else
	#endif
	{
		FLASH_INTF_WriteStr( Addr, Buf,Len);
	}
}

/*******************************************************************************
 *函数名:PcToMcuTuner
 *函数说明:MCU接收PC调测数据
 *参数说明:
 *返回值:无
 *创建人:yufeng
 *创建日期:2012/11/18
 *******************************************************************************/
void PC_BLL_PcToMcuTuner(DH_U8* Buf, DH_U16 Len, DH_U8 err)
{
	Str_PCctunertxstat.FreqChexkse = 0;
	Str_PCctunertxstat.HighPowerset = 0;
	Str_PCctunertxstat.LowPowerset = 0;
	Str_PCctunertxstat.Dmodbalanceset = 0;
	Str_PCctunertxstat.RF_OUTset = 0;
	Str_PCctunertxstat.FreqBuf = 0;
	if (PCINFERR_OK == err)
	{
		PC_BLL_TuneWriteStr( PCTUNERADDR, (DH_U8*) (Buf + 8),
				(Len - 9));
		PC_BLL_TunerMFToPc(Buf, (DH_U8*) 0, 0, TUNERALLWRITE, UFR_RECOK); /* 发送校验ok包 */
		ResCnt = 0;
		Delay_ms(100);
		GloVarFlag.UVDebugFlg = 1;
	//	AppSysReset(); /* 重启 */
		return;
	}
	else if (PCINFERR_TIMEOUT == err)
	{
		PC_BLL_TunerMFToPc(Buf, (DH_U8*) 0, 0, TUNERALLWRITE, UFR_RECERR);
	}
	else if (PCINFERR_CHECKERR == err)
	{
		/* 超过最大重传次数，发送停止发送包，并退出 */
		if (++ResCnt >= PCTUNERCNTMAX)
		{
			//PC_BLL_TunerMFToPc (Buf, (DH_U8*)0, 0, TUNERALLWRITE, UFR_RECSTOP1);
		}
		else
		{
			PC_BLL_TunerMFToPc(Buf, (DH_U8*) 0, 0, TUNERALLWRITE, UFR_RECERR);
		}
	}
	else //if (PCINFERR_FORMATERR == err)
	{
		PC_BLL_TunerMFToPc(Buf, (DH_U8*) 0, 0, TUNERALLWRITE, UFR_RECERR);
	}
}
/*******************************************************************************
 *函数名:PC_BLL_TunerMFToPc
 *函数说明:MCU组射频调测帧发送给PC
 *参数说明:
 *返回值:无
 *创建人:yufeng
 *创建日期:2012/11/18
 *******************************************************************************/
void PC_BLL_TunerMFToPc(DH_U8* DstBuf, DH_U8* SourBuf, DH_U16 Len, DH_U8 Opt,
		DH_U8 Ack)
{
	DH_U16 CheckSum;

	DstBuf[0] = 0x68;
	DstBuf[1] = UART_TUNER_TEST;
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
 *函数名:PC_BLL_McuToPcTuner
 *函数说明:PC接收MCU调测数据
 *参数说明:
 *返回值:无
 *创建人:yufeng
 *创建日期:2012/11/18
 *******************************************************************************/
void PC_BLL_McuToPcTuner(DH_U8* Buf, DH_U16 Len, DH_U8 err)
{
	Str_PCctunertxstat.FreqChexkse = 0;
	Str_PCctunertxstat.HighPowerset = 0;
	Str_PCctunertxstat.LowPowerset = 0;
	Str_PCctunertxstat.Dmodbalanceset = 0;
	Str_PCctunertxstat.RF_OUTset = 0;
	Str_PCctunertxstat.FreqBuf = 0;
	DH_U8 SendBuf[TUNERPARMTLEN];

	if (PCINFERR_OK == err)
	{
		if (++ResCnt < PCTUNERCNTMAX)
		{
			if (Buf[3] == 0x00)
			{
				FLASH_INTF_ReadStr(  PCTUNERADDR, SendBuf,
						TUNERPARMTLEN);
				PC_BLL_TunerMFToPc(Buf, SendBuf, TUNERPARMTLEN, TUNERALLREAD,
						UFR_READDATA);
			}
			else if (Buf[3] == 0x01)
			{
				ResCnt = 0;
			}
			else if (Buf[3] == 0xff)
			{
				FLASH_INTF_ReadStr(  PCTUNERADDR, SendBuf,
						TUNERPARMTLEN);
				PC_BLL_TunerMFToPc(Buf, SendBuf, TUNERPARMTLEN, TUNERALLREAD,
						UFR_READDATA);
			}
		}
		else
		{
			ResCnt = 0;
			//PC_BLL_TunerMFToPc (Buf, (DH_U8*)0, 0, UFR_RECSTOP1);
		}
	}
	else if (PCINFERR_TIMEOUT == err || PCINFERR_FORMATERR == err
			|| PCINFERR_CHECKERR == err)
	{
		if (++ResCnt > PCTUNERCNTMAX) /* 超过重传次数 */
		{
			ResCnt = 0;
			//PC_BLL_TunerMFToPc (Buf, (DH_U8*)0, 0, TUNERALLREAD, UFR_RECSTOP1);
		}
		else
		{
			FLASH_INTF_ReadStr(  PCTUNERADDR, SendBuf,
					TUNERPARMTLEN);
			PC_BLL_TunerMFToPc(Buf, SendBuf, TUNERPARMTLEN, TUNERALLREAD,
					UFR_READDATA);
		}
	}
}

#if 0
/*******************************************************************************
 *函数名:FreqCheckSet
 *函数说明:上位机收发频率校准
 Addr=8
 |Sel(Addr+0)|Tx(Addr+(1~2))|Rx(Addr+(3~4))|OffSet(Addr+(5~6))|
 *参数说明:
 *返回值:无
 *创建人:yufeng
 *创建日期:2012/11/18
 *******************************************************************************/
void PC_BLL_PcFreqCheckSet(DH_U8* Buf, DH_U16 Len, DH_U8 err)
{
	DH_U8 i;
	DH_U8 Buf_[7];
	DH_U8 Signal[36];
	PLL_INFO_S Str;
	DH_U8 Ampbias;
	DH_U16 Afc;
	//OS_ERR oserr;
	PcTunerFlg = 1;
	//OSTaskSuspend(&TaskRFTCB, &oserr);
	Str_PCctunertxstat.HighPowerset = 0;
	Str_PCctunertxstat.LowPowerset = 0;
	Str_PCctunertxstat.Dmodbalanceset = 0;
	Str_PCctunertxstat.RF_OUTset = 0;
	Str_PCctunertxstat.FreqBuf = 0;

	if (Buf[3] == 1)
	{
		Str_PCctunertxstat.FreqChexkse = 0;
		Str = RF_BLL_GetPll();
		RF_BLL_RxInit(&Str);
		FLASH_INTF_ReadStr(  TUNER_TXRXADDR, Buf_, 7);
		Buf_[0] = 0x01;
		PC_BLL_TunerMFToPc(Buf, Buf_, 17, Buf[2], Buf[3]);///只上传发射
	}
	else if (Buf[3] == 2)
	{
		if (PCINFERR_OK == err)
		{
			memcpy((void*) FreqCheckBuf, (void*) (Buf + 9), 6);
//			FLASH_INTF_WriteStr(  TUNER_TXRXADDR, Buf + 8, 7);
			Str = RF_BLL_GetPll();
			Str.u16ApcLowVal = 1000;/*防止上电直接校准频率导致发射功率太低*/
		#if (FREQ_SCOPE_SET==FREQ_136_174||FREQ_SCOPE_SET==FREQ_150_170)
			Str.u32TxFreq = 155000000;
			Str.u32RxFreq = 155000000;
		#elif (FREQ_SCOPE_SET==FREQ_240_250)
			Str.u32TxFreq = 245000000;
			Str.u32RxFreq = 245000000;
		#elif (FREQ_SCOPE_SET==FREQ_350_400)
			Str.u32TxFreq = 375000000;
			Str.u32RxFreq = 375000000;
		#elif (FREQ_SCOPE_SET==FREQ_30_32)
			Str.u32TxFreq = 31000000;
			Str.u32RxFreq = 31000000;
		#elif (FREQ_SCOPE_SET==FREQ_66_68)
			Str.u32TxFreq = 67000000;
			Str.u32RxFreq = 67000000;
		#elif (FREQ_SCOPE_SET==FREQ_66_88)
			Str.u32TxFreq = 77000000;
			Str.u32RxFreq = 77000000;
		#elif (FREQ_SCOPE_SET==FREQ_200_300)
			Str.u32TxFreq = 255000000;
			Str.u32RxFreq = 255000000;
		#else
//			Str.u32TxFreq = 435000000;
//			Str.u32RxFreq = 435000000;
			Str.u32TxFreq = 430000000;
			Str.u32RxFreq = 430000000;
		#endif
			if (Str_PCctunertxstat.FreqChexkse == 0)
			{
				RF_INTF_SetRx(Str.u32RxFreq); /*Rda1846Rx*//*480us*/
			}

			Str.u16AmpbiasVal = (Buf[10] << 8) | Buf[9];
			Str.u16ApcVal = Str.u16ApcLowVal;
			PC_BLL_PcAfcAutCreate(Buf + 9, &Ampbias, &Str.u16AfcVal);
			RF_BLL_SavePll(Str);
			RF_INTF_SetIQAmpNBias(&Str);
			if (Buf[8] == 0x01) /*Tx_Amp_Bias*/
			{
				for (i = 0; i < 36; i++)
				{
					Signal[i] = 0x5f;
				}
				//HRC_INTF_StartTxRAM(Signal);
			}
			else if (Buf[8] == 0x02) /*Rx_Amp_Bias*/
			{
				*LAYER2_TXRX_CTRL = 0x00;
				*LAYER2_CONTROL = 0x00;
				*WORK_MODE = 0x13;               //二层模式,表示时隙2,对齐模式
				*SYS_SOFT_RSTN = ~0x03;
			}
			if (Str_PCctunertxstat.FreqChexkse == 0)
			{
				Str_PCctunertxstat.FreqChexkse = 1;
				RF_BLL_EnableTx();
			}
			Buf[8] = 0x01;
			RF_INTF_APCSet(Str.u16ApcVal);
			PC_BLL_TunerMFToPc(Buf, (DH_U8*) 0, 0, Buf[2], Buf[3]);
			PC_BLL_TuneWriteStr( TUNER_TXRXADDR,Buf+8,7);
			//FLASH_INTF_WriteStr( TUNER_TXRXADDR,Buf+8,7);


		}
		else
		{
			Afc = Str.u16AfcVal;
			RF_INTF_SetIQAmpNBias(&Str);
			RF_BLL_EnableRx(); /*480us*/
		}
	}
	else
	{               //注意应答的命令号
		PC_BLL_TunerMFToPc(Buf, (DH_U8*) 0, 0, Buf[2], UFR_RECERR);
	}
}

/*******************************************************************************
 *函数名:PcDModBalanceSet
 *函数说明:上位机数字两点平衡设置
 Addr=8
 |Sel(Addr+0)|403(Addr+(1~2))|410(Addr+(3~4))|424(Addr+(5~6))|436(Addr+(7~8))/
 |445(Addr+(9~10))|459(Addr+11~12))|460(Addr+(13~14))|470(Addr+(15~16))|
 *参数说明:
 *返回值:无
 *创建人:yufeng
 *创建日期:2012/11/18
 *******************************************************************************/
void PC_BLL_PcDModBalanceSet(DH_U8* Buf, DH_U16 Len, DH_U8 err)
{
	DH_U8 i;
	DH_U8 Buf_[18];
	DH_U8 Signal[36] =
	{ 0x80, 0x00, 0x1D, 0x10, 0x16, 0x80, 0x19, 0xE0, 0x62, 0xE0, 0x0F, 0x81,
			0xE2, 0x10, 0x5E, 0xD5, 0xD7, 0xF7, 0x7F, 0xD7, 0x57, 0x09, 0x40,
			0x82, 0x17, 0x20, 0x06, 0x20, 0x80, 0xD1, 0xF5, 0x00, 0xA0, 0x03,
			0x30, 0x01 };
	PLL_INFO_S StrSnd;
	DH_U8 Ampbias;
	DH_U16 Afc;
	//OS_ERR oserr;
	PcTunerFlg = 1;
//	//OSTaskSuspend(TASK_HRC5000_RF_PRIOR_);
	//OSTaskSuspend(&TaskRFTCB, &oserr);
	Str_PCctunertxstat.FreqChexkse = 0;
	Str_PCctunertxstat.HighPowerset = 0;
	Str_PCctunertxstat.LowPowerset = 0;
	Str_PCctunertxstat.RF_OUTset = 0;
	if (Buf[3] == 1)
	{
		Str_PCctunertxstat.Dmodbalanceset = 0;
		StrSnd = RF_BLL_GetPll();
		RF_BLL_RxInit(&StrSnd);

		FLASH_INTF_ReadStr(  TUNER_DMODBALADDR, Buf_, 18);
		PC_BLL_TunerMFToPc(Buf, Buf_, 18, Buf[2], Buf[3]);
		//LCD_BLL_printf_ext(6, 5, lcd_mes, "Read DModBalance    ");
	}
	else if (Buf[3] == 2)
	{
		if (PCINFERR_OK == err)
		{
			StrSnd = RF_BLL_GetPll();

			if (Str_PCctunertxstat.FreqBuf != Buf[8])
			{
				Str_PCctunertxstat.FreqBuf = Buf[8];
				Str_PCctunertxstat.Dmodbalanceset = 0;
			}
//			g_PcTunerFeqSelect=1;
//			PC_BLL_Copy8Sectordata();
//			g_PcTunerFeqSelect=0;

			if ((Buf[8] > 0) && (Buf[8] <= 8))
			{
				StrSnd.u32TxFreq = g_u32FreqSectorList[Buf[8]];
				StrSnd.u32RxFreq = g_u32FreqSectorList[Buf[8]];

//				 StrSnd.u8Mod1 = Buf[2*Buf[8]+7];
				StrSnd.u8Mod1 = Buf[2 * Buf[8] + 8];
				StrSnd.u8Mod2 = StrSnd.u8Mod1;
				Buf[2 * 1 + 7] = Buf[2 * 1 + 8];
				Buf[2 * 2 + 7] = Buf[2 * 2 + 8];
				Buf[2 * 3 + 7] = Buf[2 * 3 + 8];
				Buf[2 * 4 + 7] = Buf[2 * 4 + 8];
				Buf[2 * 5 + 7] = Buf[2 * 5 + 8];
				Buf[2 * 6 + 7] = Buf[2 * 6 + 8];
				Buf[2 * 7 + 7] = Buf[2 * 7 + 8];
				Buf[2 * 8 + 7] = Buf[2 * 8 + 8];
			}
			else
			{
				StrSnd.u32TxFreq = 0;
				StrSnd.u32RxFreq = 0;
				StrSnd.u8Mod2 = 0;
				StrSnd.u8Mod1 = 0;
			}

//			FLASH_INTF_WriteStr(  TUNER_DMODBALADDR, Buf + 8, 18);

			if (Str_PCctunertxstat.Dmodbalanceset == 0)
			{
				RF_INTF_SetRx(StrSnd.u32RxFreq);	//Rda1846Rx	480us
			}
			RF_BLL_SavePll(StrSnd);
			Ampbias = /*ampbais_autoset*/(StrSnd.u16AmpbiasVal);//Ampbias = AmpbaisAutSet(StrSnd.Reg02);
			Afc = /*afc_autoset*/(StrSnd.u16AfcVal);//	Afc = AfcAutSet(StrSnd.u16AfcVal);

			if (Buf[26] == 0x10)
			{
				RF_INTF_SetIQAmpNBias(&StrSnd);

				if (Str_PCctunertxstat.Dmodbalanceset == 0)
				{
					Str_PCctunertxstat.Dmodbalanceset = 1;
					StrSnd.u16ApcVal = 1000;
					RF_BLL_EnableTx();
					RF_INTF_APCSet(StrSnd.u16ApcVal);
//					LCD_BLL_printf_ext(6,5,lcd_mes,"Set DModBal   ");
				}

				if (Buf[25] == 0x01) /*+3*/
				{
					for (i = 0; i < 36; i++)
					{
						Signal[i] = 0x55;
					}
//					LCD_BLL_printf_ext(6,101,lcd_mes, "+3 ");
				}
				else if (Buf[25] == 0x02) /*-3*/
				{
//					LCD_BLL_printf_ext(6,101,lcd_mes, "-3 ");
					for (i = 0; i < 36; i++)
					{
						Signal[i] = 0xff;
					}
				}

				else if (Buf[25] == 0x03) /*+3-3*/
				{
//					LCD_BLL_printf_ext(6,101,lcd_mes, "+-3");
					for (i = 0; i < 36; i++)
					{
						Signal[i] = 0x5f;
					}
				}
				else if (Buf[25] == 0x04) /*随机数*/
				{
//					LCD_BLL_printf_ext(6,101,lcd_mes, "Rand");
				}
				//LCD_BLL_printf_ext(6, 5, lcd_mes, "Set I%d Q%d %d",
				//		StrSnd.u8Mod1, StrSnd.u8Mod2, Buf[25]);
				//HRC_INTF_StartTxRAM((DH_U8*) Signal);
			}
			else
			{
				RF_INTF_SetRxAmpbias(Afc);
				RF_BLL_EnableRx();
			}

//			FLASH_INTF_WriteStr( TUNER_DMODBALADDR,Buf+8,18);
			PC_BLL_TunerMFToPc(Buf, (DH_U8*) 0, 0, Buf[2], Buf[3]);
			PC_BLL_TuneWriteStr(  TUNER_DMODBALADDR, Buf + 8, 18);
		}
		else
		{
			PC_BLL_TunerMFToPc(Buf, (DH_U8*) 0, 0, Buf[2], UFR_RECERR);
		}
	}
	else
	{
		PC_BLL_TunerMFToPc(Buf, (DH_U8*) 0, 0, Buf[2], UFR_RECERR);
	}
}

/*******************************************************************************
 *函数名:PC_BLL_PcMDevNarrowSet
 *函数说明:
 Addr=8
 |Sel(Addr+0)|403(Addr+(1~2))|410(Addr+(3~4))|424(Addr+(5~6))|436(Addr+(7~8))/
 |445(Addr+(9~10))|459(Addr+11~12))|460(Addr+(13~14))|470(Addr+(15~16))|
 *参数说明:
 *返回值:无
 *创建人:yufeng
 *创建日期:2012/11/18
 *******************************************************************************/
void PC_BLL_PcMDevNarrowSet(DH_U8* Buf, DH_U16 Len, DH_U8 err)
{
	PLL_INFO_S StrSnd;
	DH_U16 reg59 = 0;

	reg59 = RDA1846S_INTF_ReadReg(0x59);

	if (PCINFERR_OK == err)
	{
		StrSnd = RF_BLL_GetPll();

//		g_PcTunerFeqSelect=1;
//		PC_BLL_Copy8Sectordata();
//		g_PcTunerFeqSelect=0;
		StrSnd.u32TxFreq = StrSnd.u32RxFreq = g_u32FreqSectorList[Buf[8]];

		reg59 &= 0x003f;
		reg59 |= (((Buf[8 + (Buf[8] * 2)] << 8) | Buf[8 + (Buf[8] * 2) - 1])
				<< 6);

		RF_BLL_SavePll(StrSnd);

		/* 设置组呼列表 */
		CHN_BLL_SetGroupNumList(0x01);
		/* 设置信道联系人 */
		CHN_BLL_SetLinkMan(0x01);

//		STR_CODESW StrCode=GetPllStat();
//		StrCode.GroupList = 0x01;
//		StrCode.AreaLM = 0x01;
//		Func_Str.sqlevel = 1;
//		SavePllStat(StrCode);

		if (Buf[25] == 0x01)
		{
			RF_INTF_SetRx(StrSnd.u32RxFreq); /*Rda1846Rx*//*480us*/
			if( g_FMIC_Sel==FM_IC_RDA1846S)RDA1846S_INTF_WriteReg(RDA_REG_59H, reg59);
			RF_BLL_EnableTx();
		}
		else
		{
			RF_BLL_EnableRx();
		}
	}
	else
	{
		PC_BLL_TunerMFToPc(Buf, (DH_U8*) 0, 0, MDEVNARROWSET, UFR_RECERR);
	}
}

/*******************************************************************************
 *函数名:PC_BLL_PcMDevWideSet
 *函数说明:上位机模拟两点平衡设置
 Addr=8
 |Sel(Addr+0)|403(Addr+(1~2))|410(Addr+(3~4))|424(Addr+(5~6))|436(Addr+(7~8))/
 |445(Addr+(9~10))|459(Addr+11~12))|460(Addr+(13~14))|470(Addr+(15~16))|
 *参数说明:
 *返回值:无
 *创建人:yufeng
 *创建日期:2012/11/18
 *******************************************************************************/
void PC_BLL_PcMDevWideSet(DH_U8* Buf, DH_U16 Len, DH_U8 err)
{
	PLL_INFO_S StrSnd;
	DH_U16 reg59 = 0;

	reg59 = RDA1846S_INTF_ReadReg(0x59);

	if (PCINFERR_OK == err)
	{
		StrSnd = RF_BLL_GetPll();

//		g_PcTunerFeqSelect=1;
//		PC_BLL_Copy8Sectordata();strcpy
//		g_PcTunerFeqSelect=0;
		StrSnd.u32TxFreq = StrSnd.u32RxFreq = g_u32FreqSectorList[Buf[8]];

		reg59 &= 0x003f;
		reg59 |= (((Buf[8 + (Buf[8] * 2)] << 8) | Buf[8 + (Buf[8] * 2) - 1])
				<< 6);

		RF_BLL_SavePll(StrSnd);

		/* 设置组呼列表 */
		CHN_BLL_SetGroupNumList(0x01);
		/* 设置信道联系人 */
		CHN_BLL_SetLinkMan(0x01);
//		STR_CODESW StrCode=GetPllStat();
//		StrCode.GroupList = 0x01;
//		StrCode.AreaLM = 0x01;
//		Func_Str.sqlevel = 1;
//		SavePllStat(StrCode);

		if (Buf[25] == 0x01)
		{
			RF_INTF_SetRx(StrSnd.u32RxFreq); /*Rda1846Rx*//*480us*/

			if( g_FMIC_Sel==FM_IC_RDA1846S)RDA1846S_INTF_WriteReg(RDA_REG_59H, reg59);
			RF_BLL_EnableTx();
		}
		else
		{
			RF_BLL_RxInit(&StrSnd);
		}
	}
	else
	{
		PC_BLL_TunerMFToPc(Buf, (DH_U8*) 0, 0, MDEVWIDESET, UFR_RECERR);
	}
}

/*******************************************************************************
 *函数名:PC_BLL_PcMTPLDPLNarrowSet
 *函数说明:上位机模拟两点平衡设置
 Addr=8
 |Sel(Addr+0)|403(Addr+(1~2))|410(Addr+(3~4))|424(Addr+(5~6))|436(Addr+(7~8))/
 |445(Addr+(9~10))|459(Addr+11~12))|460(Addr+(13~14))|470(Addr+(15~16))|
 *参数说明:
 *返回值:无
 *创建人:yufeng
 *创建日期:2012/11/18
 *******************************************************************************/
void PC_BLL_PcMTPLDPLNarrowSet(DH_U8* Buf, DH_U16 Len, DH_U8 err)
{
	PLL_INFO_S StrSnd;
	DH_U16 reg59 = 0;

	reg59 = RDA1846S_INTF_ReadReg(0x59);

	if (PCINFERR_OK == err)
	{
		StrSnd = RF_BLL_GetPll();

//		g_PcTunerFeqSelect=1;
//		PC_BLL_Copy8Sectordata();
//		g_PcTunerFeqSelect=0;
		StrSnd.u32TxFreq = StrSnd.u32RxFreq = g_u32FreqSectorList[Buf[8]];

		reg59 &= 0xffc0;
		reg59 |= Buf[8 + Buf[8]];

		RF_BLL_SavePll(StrSnd);

		/* 设置组呼列表 */
		CHN_BLL_SetGroupNumList(0x02);
		/* 设置信道联系人 */
		CHN_BLL_SetLinkMan(0x02);

		CHN_INFO_S StrCode = CHN_BLL_GetStr();
		StrCode.stRxDChn.u8GroupList = 0x02;
		StrCode.stTxDChn.u8LinkmanIndex = 0x01;
		StrCode.stTxDChn.u8ReceiveTransTactics = 1;
		CHN_BLL_SetStr(StrCode);
//		STR_CODESW StrCode=GetPllStat();
//		StrCode.GroupList = 0x02;
//		StrCode.AreaLM = 0x02;
//		StrCode.AreaRecdissend = 1;	/*0x6342*/
//		StrCode.AreaEncryptmode = 1;
//		SavePllStat(StrCode);

		if (Buf[16] == 0x01)
		{
			RF_INTF_SetRx(StrSnd.u32RxFreq); /*Rda1846Rx*//*480us*/
			if( g_FMIC_Sel==FM_IC_RDA1846S)RDA1846S_INTF_WriteReg(RDA_REG_59H, reg59);
			RF_BLL_EnableTx();
		}
		else
		{
			RF_BLL_RxInit(&StrSnd);
		}
	}
	else
	{
		PC_BLL_TunerMFToPc(Buf, (DH_U8*) 0, 0, MTPLDPLNARROWSET, UFR_RECERR);
	}
}

/*******************************************************************************
 *函数名:PC_BLL_PcMTPLDPLNarrowSet
 *函数说明:上位机模拟两点平衡设置
 Addr=8
 |Sel(Addr+0)|403(Addr+(1~2))|410(Addr+(3~4))|424(Addr+(5~6))|436(Addr+(7~8))/
 |445(Addr+(9~10))|459(Addr+11~12))|460(Addr+(13~14))|470(Addr+(15~16))|
 *参数说明:
 *返回值:无
 *创建人:yufeng
 *创建日期:2012/11/18
 *******************************************************************************/
void PC_BLL_PcMTPLDPLWideSet(DH_U8* Buf, DH_U16 Len, DH_U8 err)
{
	PLL_INFO_S StrSnd;
	DH_U16 reg59 = 0;

	reg59 = RDA1846S_INTF_ReadReg(0x59);

	if (PCINFERR_OK == err)
	{
		StrSnd = RF_BLL_GetPll();

//		g_PcTunerFeqSelect=1;
//		PC_BLL_Copy8Sectordata();
//		g_PcTunerFeqSelect=0;
		StrSnd.u32TxFreq = StrSnd.u32RxFreq = g_u32FreqSectorList[Buf[8]];

		reg59 &= 0xffc0;
		reg59 |= Buf[8 + Buf[8]];

		RF_BLL_SavePll(StrSnd);

		/* 设置组呼列表 */
		CHN_BLL_SetGroupNumList(0x02);
		/* 设置信道联系人 */
		CHN_BLL_SetLinkMan(0x02);

		CHN_INFO_S StrCode = CHN_BLL_GetStr();
		StrCode.stRxDChn.u8GroupList = 0x02;
		StrCode.stTxDChn.u8LinkmanIndex = 0x02;
		StrCode.stTxDChn.u8ReceiveTransTactics = 1;
		CHN_BLL_SetStr(StrCode);

//		STR_CODESW StrCode=GetPllStat();
//		StrCode.GroupList = 0x02;
//		StrCode.AreaLM = 0x02;
//		StrCode.AreaRecdissend = 1;	/*0x6342*/
//		StrCode.AreaEncryptmode = 1;
//		SavePllStat(StrCode);

		if (Buf[16] == 0x01)
		{
			RF_INTF_SetRx(StrSnd.u32RxFreq); /*Rda1846Rx*//*480us*/
			if( g_FMIC_Sel==FM_IC_RDA1846S)RDA1846S_INTF_WriteReg(RDA_REG_59H, reg59);
			RF_BLL_EnableTx();
		}
		else
		{
			RF_BLL_RxInit(&StrSnd);
		}
	}
	else
	{
		PC_BLL_TunerMFToPc(Buf, (DH_U8*) 0, 0, MTPLDPLWIDESET, UFR_RECERR);
	}
}
extern DH_BOOL g_bCommunicating2PC;
/*******************************************************************************
 *函数名:PC_BLL_PcReadErrNumRat
 *函数说明:上位机查询误码率
 *参数说明:
 *返回值:无
 *创建人:yufeng
 *创建日期:2012/11/18
 *******************************************************************************/
void PC_BLL_PcReadErrNumRat(DH_U8*Buf, DH_U16 Len, DH_U8 err)
{
	//OS_ERR oserr;
	Str_PCctunertxstat.FreqChexkse = 0;
	Str_PCctunertxstat.HighPowerset = 0;
	Str_PCctunertxstat.LowPowerset = 0;
	Str_PCctunertxstat.Dmodbalanceset = 0;
	Str_PCctunertxstat.RF_OUTset = 0;
	Str_PCctunertxstat.FreqBuf = 0;
	PcTunerFlg = 0;
//	//OSTaskResume(TASK_HRC5000_RF_PRIOR_);
	//OSTaskResume(&TaskRFTCB, &oserr);
	if (PCINFERR_OK == err)
	{
		if (Buf[8] == 0x01)	//start transmit
		{
			PC_BLL_TunerMFToPc(Buf, (DH_U8*) 0, 0, ERRNUMRATTEST, UFR_BASEINFO);
			ErrNumbTestFlag = 1;
			g_bCommunicating2PC = FALSE;
			printk("发送O.153");
			g_bCommunicating2PC = TRUE;

//			PCLINK_INTF_TransmitFrame("发送O.153", 10);
			HRC_CCL_ChangeChan();
//			LCD_BLL_LcdWriteMid(28, 30, "发送O.153", lcd_h);

		}
		else if (Buf[8] == 0x02)	//stop transmit
		{
			gstChanInfo.u8PhySendForTest = 0;
			PC_BLL_TunerMFToPc(Buf, (DH_U8*) 0, 0, ERRNUMRATTEST, UFR_BASEINFO);
//			LCD_BLL_LcdWriteMid(28, 30, "关闭发送O.153", lcd_h);
			g_bCommunicating2PC = FALSE;
			printk("关闭发送O.153");
			g_bCommunicating2PC = TRUE;
			//PCLINK_INTF_TransmitFrame("关闭发送O.153", 14);
			ErrNumbTestFlag = 0;
			HRC_CCL_ChangeChan();
			RF_BLL_RxInit(RF_BLL_GetPllStrP());
			RED_LED_OFF;
		}
		else if (Buf[8] == 0x03)	//return receive err rate
		{
			PC_BLL_TunerMFToPc(Buf, (DH_U8*) 0, 0, ERRNUMRATTEST, UFR_BASEINFO);
//			HRC_INTF_PHY_SetRecv();
			/*
			 * *LAYER2_CONTROL = 0x00;
			 *LAYER2_TXRX_CTRL= 0x00;
			 *SYS_SOFT_RSTN = ~0x03;
			 *WORK_MODE=0x13;
			 */
			Delay_ms(100);
			g_bCommunicating2PC = FALSE;
			printk("接收误码率测试\r\n");
			g_bCommunicating2PC = TRUE;

			//PCLINK_INTF_TransmitFrame("接收误码率测试", 15);

			DMRRXCHN_INFO_S Str = CHN_BLL_GetDMRRxChnInfo();
			g_ErrFlag = 1;
			Str.u8ErrRatio = 1;
			CHN_BLL_SetDMRRxChnInfo(Str);
			/*频道切换后的协议层状态复位接口*/
			HRC_CCL_ChangeChan();
			ErrNumbTestFlag = 0;
		}
	}
	else
	{
		PC_BLL_TunerMFToPc(Buf, (DH_U8*) 0, 0, ERRNUMRATTEST, UFW_RECERR);
	}
}

/*******************************************************************************
 *函数名:PC_BLL_PcVolumSet
 *函数说明:上位机音量设置
 *参数说明:
 *返回值:无
 *创建人:yufeng
 *创建日期:2012/11/18
 *******************************************************************************/
void PC_BLL_PcVolumSet(DH_U8*Buf, DH_U16 Len, DH_U8 err)
{
	//DH_U8 data;

	//data=Buf[8];
	if (PCINFERR_OK == err)
	{
		PC_BLL_TunerMFToPc(Buf, (DH_U8*) 0, 0, Buf[2], UFW_RECOK);
	}
	else
	{
		PC_BLL_TunerMFToPc(Buf, (DH_U8*) 0, 0, Buf[2], UFW_RECERR);
	}

}

/*******************************************************************************
 *函数名:PC_BLL_PcSQLevel1OpenSet
 *函数说明:静噪等级1开启设置
 *参数说明:
 *返回值:无
 *创建人:yufeng
 *创建日期:2012/11/18
 *******************************************************************************/
void PC_BLL_PcSQLevel1OpenSet(DH_U8*Buf, DH_U16 Len, DH_U8 err)
{
	PLL_INFO_S Str;

	if (PCINFERR_OK == err)
	{
		Str = RF_BLL_GetPll();
//		g_PcTunerFeqSelect=1;
//		PC_BLL_Copy8Sectordata();
//		g_PcTunerFeqSelect=0;
		Str.u32TxFreq = Str.u32RxFreq = g_u32FreqSectorList[Buf[8]];

		DH_U8 th_h_sq = Buf[8 + Buf[8]];

#if  0
		switch(Buf[8])
		{
			case 1: /*403.075*/
			{
				Str.u32TxFreq = FREQ403_175;
				Str.u32RxFreq = FREQ403_175;
				Str.th_h_sq= Buf[9];
			}
			break;
			case 2: /*414.075*/
			{
				Str.u32TxFreq = FREQ414_175;
				Str.u32RxFreq = FREQ414_175;
				Str.th_h_sq=Buf[10];
			}
			break;
			case 3: /*425.075*/
			{
				Str.u32TxFreq = FREQ425_175;
				Str.u32RxFreq = FREQ425_175;
				Str.th_h_sq=Buf[11];
			}
			break;
			case 4: /*436.525*/
			{
				Str.u32TxFreq = FREQ436_625;
				Str.u32RxFreq = FREQ436_625;
				Str.th_h_sq=Buf[12];
			}
			break;
			case 5: /*447.875*/
			{
				Str.u32TxFreq = FREQ447_775;
				Str.u32RxFreq = FREQ447_775;
				Str.th_h_sq=Buf[13];
			}
			break;
			case 6: /*458.875*/
			{
				Str.u32TxFreq = FREQ458_775;
				Str.u32RxFreq = FREQ458_775;
				Str.th_h_sq=Buf[14];
			}
			break;
			case 7: /*469.925*/
			{
				Str.u32TxFreq = FREQ469_825;
				Str.u32RxFreq = FREQ469_825;
				Str.th_h_sq=Buf[15];
			}
			break;
			default :
			break;
		}
#endif

		/* 设置组呼列表 */
		CHN_BLL_SetGroupNumList(0x01);
		/* 设置信道联系人 */
		CHN_BLL_SetLinkMan(0x01);

		CHN_INFO_S StrCode = CHN_BLL_GetStr();
		StrCode.stRxDChn.u8GroupList = 0x01;
		StrCode.stTxDChn.u8LinkmanIndex = 0x01;
		CHN_BLL_SetStr(StrCode);

//		STR_CODESW StrCode=GetPllStat();
//		StrCode.GroupList = 0x01;
//		StrCode.AreaLM = 0x01;
//		SavePllStat(StrCode);

		Sq_flag = 1;
		RF_BLL_SavePll(Str);
		RF_BLL_RxInit(&Str);

		gstSql.u16StartNoiseLevel = th_h_sq;
		gstSql.u16StopNoiseLevel = th_h_sq + 1;
		//	RDA1846S_INTF_SqThrSet(th_h_sq + 1, th_h_sq);
		Sq_flag = 0;
	}
	else
	{
		PC_BLL_TunerMFToPc(Buf, (DH_U8*) 0, 0, SQLEVEL1OPENSET, UFR_RECERR);
	}
}

/*******************************************************************************
 *函数名:PC_BLL_PcSQLevel1CloseSet
 *函数说明:静噪等级1关闭设置
 *参数说明:
 *返回值:无
 *创建人:yufeng
 *创建日期:2012/11/18
 *******************************************************************************/
void PC_BLL_PcSQLevel1CloseSet(DH_U8*Buf, DH_U16 Len, DH_U8 err)
{
#if 0
	PLL_INFO_S Str;

	if(PCINFERR_OK==err)
	{
		Str= RF_BLL_GetPll();

		switch(Buf[8])
		{
			case 1: /*403.075*/
			{
				Str.u32TxFreq = FREQ403_175;
				Str.u32RxFreq = FREQ403_175;
				Str.th_l_sq= Buf[9];
			}
			break;
			case 2: /*414.075*/
			{
				Str.u32TxFreq = FREQ414_175;
				Str.u32RxFreq = FREQ414_175;
				Str.th_l_sq=Buf[10];
			}
			break;
			case 3: /*425.075*/
			{
				Str.u32TxFreq = FREQ425_175;
				Str.u32RxFreq = FREQ425_175;
				Str.th_l_sq=Buf[11];
			}
			break;
			case 4: /*436.525*/
			{
				Str.u32TxFreq = FREQ436_625;
				Str.u32RxFreq = FREQ436_625;
				Str.th_l_sq=Buf[12];
			}
			break;
			case 5: /*447.875*/
			{
				Str.u32TxFreq = FREQ447_775;
				Str.u32RxFreq = FREQ447_775;
				Str.th_l_sq=Buf[13];
			}
			break;
			case 6: /*458.875*/
			{
				Str.u32TxFreq = FREQ458_775;
				Str.u32RxFreq = FREQ458_775;
				Str.th_l_sq=Buf[14];
			}
			break;
			case 7: /*469.925*/
			{
				Str.u32TxFreq = FREQ469_825;
				Str.u32RxFreq = FREQ469_825;
				Str.th_l_sq=Buf[15];
			}
			break;
			default :
			break;
		}

		STR_CODESW StrCode=GetPllStat();
		StrCode.GroupList = 0x01;
		StrCode.AreaLM = 0x01;
		SavePllStat(StrCode);

		Sq_flag = 1;
		RF_BLL_SavePll(Str);

#if HRC7000IC
		RF_BLL_RxInit(Str);
#else
		RF_INTF_APCSet(0);
		RF_INTF_TVSet(StrSnd.u16TvVal);
		RF_TX_EN_LOW_DIS;
		RF_ANT_SW_LOW_RX;

		Afc = AfcAutSet(StrSnd.u16AfcVal);
		HRC5000Send_cmd(0x47,(DH_U8)Afc);
		HRC5000Send_cmd(0x48,(DH_U8)(Afc>>8));

		RF_RX_EN_HIGH_EN;
		RF_INTF_SetRx(StrSnd.u32RxFreq);/*Rda1846Rx*//*480us*/
#endif

		gstSql.u16StartNoiseLevel = th_h_sq;
		gstSql.u16StopNoiseLevel = th_h_sq + 1;
		//	RDA1846S_INTF_SqThrSet(th_h_sq + 1, th_h_sq);
		Sq_flag = 0;
	}
	else
	{
		PC_BLL_TunerMFToPc(Buf, (DH_U8*)0, 0, SQLEVEL1CLOSESET, UFR_RECERR);
	}
#endif
}

/*******************************************************************************
 *函数名:PC_BLL_PcSQLevel9OpenSet
 *函数说明:静噪等级9开启设置
 *参数说明:
 *返回值:无
 *创建人:yufeng
 *创建日期:2012/11/18
 *******************************************************************************/
void PC_BLL_PcSQLevel9OpenSet(DH_U8*Buf, DH_U16 Len, DH_U8 err)
{
	PLL_INFO_S Str;

	if (PCINFERR_OK == err)
	{
		Str = RF_BLL_GetPll();

		Str.u32TxFreq = Str.u32RxFreq = g_u32FreqSectorList[Buf[8]];
		DH_U8 th_h_sq = Buf[8 + Buf[8]];

#if  0
		switch(Buf[8])
		{
			case 1: /*403.075*/
			{
				Str.u32TxFreq = FREQ403_175;
				Str.u32RxFreq = FREQ403_175;
				Str.th_h_sq= Buf[9];
			}
			break;
			case 2: /*414.075*/
			{
				Str.u32TxFreq = FREQ414_175;
				Str.u32RxFreq = FREQ414_175;
				Str.th_h_sq=Buf[10];
			}
			break;
			case 3: /*425.075*/
			{
				Str.u32TxFreq = FREQ425_175;
				Str.u32RxFreq = FREQ425_175;
				Str.th_h_sq=Buf[11];
			}
			break;
			case 4: /*436.525*/
			{
				Str.u32TxFreq = FREQ436_625;
				Str.u32RxFreq = FREQ436_625;
				Str.th_h_sq=Buf[12];
			}
			break;
			case 5: /*447.875*/
			{
				Str.u32TxFreq = FREQ447_775;
				Str.u32RxFreq = FREQ447_775;
				Str.th_h_sq=Buf[13];
			}
			break;
			case 6: /*458.875*/
			{
				Str.u32TxFreq = FREQ458_775;
				Str.u32RxFreq = FREQ458_775;
				Str.th_h_sq=Buf[14];
			}
			break;
			case 7: /*469.925*/
			{
				Str.u32TxFreq = FREQ469_825;
				Str.u32RxFreq = FREQ469_825;
				Str.th_h_sq=Buf[15];
			}
			break;
			default :
			break;
		}
#endif

		/* 设置组呼列表 */
		CHN_BLL_SetGroupNumList(0x01);
		/* 设置信道联系人 */
		CHN_BLL_SetLinkMan(0x01);

		CHN_INFO_S StrCode = CHN_BLL_GetStr();
		StrCode.stRxDChn.u8GroupList = 0x01;
		StrCode.stTxDChn.u8LinkmanIndex = 0x01;
		CHN_BLL_SetStr(StrCode);
		th_h_sq -= 18;
		Sq_flag = 1;
		RF_BLL_SavePll(Str);
		RF_BLL_RxInit(&Str);

		gstSql.u16StartNoiseLevel = th_h_sq;
		gstSql.u16StopNoiseLevel = th_h_sq + 1;
		//	RDA1846S_INTF_SqThrSet(th_h_sq + 1, th_h_sq);
		Sq_flag = 0;
	}
	else
	{
		PC_BLL_TunerMFToPc(Buf, (DH_U8*) 0, 0, SQLEVEL9OPENSET, UFR_RECERR);
	}
}

/*******************************************************************************
 *函数名:PC_BLL_PcSQLevel9CloseSet
 *函数说明:静噪等级9关闭设置
 *参数说明:
 *返回值:无
 *创建人:yufeng
 *创建日期:2012/11/18
 *******************************************************************************/
void PC_BLL_PcSQLevel9CloseSet(DH_U8*Buf, DH_U16 Len, DH_U8 err)
{
#if 0
	PLL_INFO_S Str;

	if(PCINFERR_OK==err)
	{
		Str= RF_BLL_GetPll();

		switch(Buf[8])
		{
			case 1: /*403.075*/
			{
				Str.u32TxFreq = FREQ403_175;
				Str.u32RxFreq = FREQ403_175;
				Str.th_l_sq= Buf[9];
			}
			break;
			case 2: /*414.075*/
			{
				Str.u32TxFreq = FREQ414_175;
				Str.u32RxFreq = FREQ414_175;
				Str.th_l_sq=Buf[10];
			}
			break;
			case 3: /*425.075*/
			{
				Str.u32TxFreq = FREQ425_175;
				Str.u32RxFreq = FREQ425_175;
				Str.th_l_sq=Buf[11];
			}
			break;
			case 4: /*436.525*/
			{
				Str.u32TxFreq = FREQ436_625;
				Str.u32RxFreq = FREQ436_625;
				Str.th_l_sq=Buf[12];
			}
			break;
			case 5: /*447.875*/
			{
				Str.u32TxFreq = FREQ447_775;
				Str.u32RxFreq = FREQ447_775;
				Str.th_l_sq=Buf[13];
			}
			break;
			case 6: /*458.875*/
			{
				Str.u32TxFreq = FREQ458_775;
				Str.u32RxFreq = FREQ458_775;
				Str.th_l_sq=Buf[14];
			}
			break;
			case 7: /*469.925*/
			{
				Str.u32TxFreq = FREQ469_825;
				Str.u32RxFreq = FREQ469_825;
				Str.th_l_sq=Buf[15];
			}
			break;
			default :
			break;
		}

		STR_CODESW StrCode=GetPllStat();
		StrCode.GroupList = 0x01;
		StrCode.AreaLM = 0x01;
		SavePllStat(StrCode);

		Str.th_l_sq -= 18;
		Sq_flag = 1;
		RF_BLL_SavePll(Str);

#if HRC7000IC
		RF_BLL_RxInit(Str);
#else
		RF_INTF_APCSet(0);
		RF_INTF_TVSet(StrSnd.u16TvVal);
		RF_TX_EN_LOW_DIS;
		RF_ANT_SW_LOW_RX;

		Afc = AfcAutSet(StrSnd.u16AfcVal);
		HRC5000Send_cmd(0x47,(DH_U8)Afc);
		HRC5000Send_cmd(0x48,(DH_U8)(Afc>>8));

		RF_RX_EN_HIGH_EN;
		RF_INTF_SetRx(StrSnd.u32RxFreq);/*Rda1846Rx*//*480us*/
#endif

		gstSql.u16StartNoiseLevel = th_h_sq;
		gstSql.u16StopNoiseLevel = th_h_sq + 1;
		//	RDA1846S_INTF_SqThrSet(th_h_sq + 1, th_h_sq);
		Sq_flag = 0;
	}
	else
	{
		PC_BLL_TunerMFToPc(Buf, (DH_U8*)0, 0, SQLEVEL9CLOSESET, UFR_RECERR);
	}
#endif
}

/*******************************************************************************
 *函数名:PC_BLL_CPUFreqAdjust
 *函数说明:上位机设置CPU频率校准
 *参数说明:
 *返回值:无
 *创建人:yufeng
 *创建日期:2012/11/18
 *******************************************************************************/
void PC_BLL_CPUFreqAdjust(DH_U8*Buf, DH_U16 Len, DH_U8 err)
{
#if 0
	PLL_INFO_S Str;

	if(PCINFERR_OK==err)
	{
		Str= RF_BLL_GetPll();
		Str.u32TxFreq = 410.0;
		Str.u32RxFreq = 410.0;

		if(Buf[8])
		{
			adjust_flag = 1;
			AdjustStart = 1;
		}

		STR_CODESW StrCode=GetPllStat();
		StrCode.GroupList = 0x02;
		StrCode.AreaLM = 0x02;
		SavePllStat(StrCode);

		RF_BLL_SavePll(Str);

#if HRC7000IC
		RF_BLL_RxInit(Str);
#else
		RF_INTF_APCSet(0);
		RF_INTF_TVSet(StrSnd.u16TvVal);
		RF_TX_EN_LOW_DIS;
		RF_ANT_SW_LOW_RX;

		Afc = AfcAutSet(StrSnd.u16AfcVal);
		HRC5000Send_cmd(0x47,(DH_U8)Afc);
		HRC5000Send_cmd(0x48,(DH_U8)(Afc>>8));

		RF_RX_EN_HIGH_EN;
		RF_INTF_SetRx(StrSnd.u32RxFreq);/*Rda1846Rx*//*480us*/
#endif

	}
	else
	{
		PC_BLL_TunerMFToPc(Buf, (DH_U8*)0, 0, CPUFREQADJUST, UFR_RECERR);
	}
#endif
}

/*******************************************************************************
 *函数名:APCadjustSet
 *函数说明:上位APC设置
 Addr=8
 |Sel(Addr+0)|403(Addr+(1~2))|414(Addr+(3~4))|425(Addr+(5~6))|436(Addr+(7~8))/
 |447(Addr+(9~10))|458(Addr+11~12))|469(Addr+(13~14))|
 *参数说明:
 *返回值:无
 *创建人:yufeng
 *创建日期:2012/11/18
 *******************************************************************************/
DH_VOID PC_BLL_ApcAdjustSet(DH_U8 * Buf, DH_U8 Len, DH_U8 err)
{
//	//OS_ERR oserr;
	Str_PCctunertxstat.FreqChexkse = 0;
	Str_PCctunertxstat.HighPowerset = 0;
	Str_PCctunertxstat.LowPowerset = 0;
	Str_PCctunertxstat.Dmodbalanceset = 0;
	Str_PCctunertxstat.RF_OUTset = 0;
	Str_PCctunertxstat.FreqBuf = 0;
	PcTunerFlg = 0;
//	//OSTaskResume(TASK_HRC5000_RF_PRIOR_);
//	//OSTaskResume(&TaskRFTCB,&oserr);
	DH_U8 Buf_[33];
	PLL_INFO_S StrSnd;
//	DH_U16 Afc;
	if (Buf[3] == 1)
	{
		StrSnd = RF_BLL_GetPll();
		RF_BLL_RxInit(&StrSnd);

		FLASH_INTF_ReadStr(  TUNER_APCADJUSTADDR, Buf_, 33);
		PC_BLL_TunerMFToPc(Buf, Buf_, 33, Buf[2], UFR_RECOK);
	}
	else if (Buf[3] == 2)
	{
		if (PCINFERR_OK == err)
		{
			DH_U8 i = 0;
			StrSnd = RF_BLL_GetPll();
//			FLASH_INTF_WriteStr(  TUNER_APCADJUSTADDR, Buf + 8,33);
			CHN_BLL_SetAttr(CHNSTAT_DMR);
			StrSnd.u32TxFreq = StrSnd.u32RxFreq = g_u32FreqSectorList[Buf[8]];
			StrSnd = PLL_BLL_GetRfVal(CHN_BLL_GetAttr(), StrSnd.u32TxFreq,
					StrSnd.u32RxFreq);

			i = (Buf[8] * 4);
			StrSnd.u16APCUpTime = Buf[4 * Buf[8] + 5]
					+ Buf[4 * Buf[8] + 6] * 256;
			StrSnd.u16APCDownTime = Buf[4 * Buf[8] + 7]
					+ Buf[4 * Buf[8] + 8] * 256;

			RF_BLL_SavePll(StrSnd);
#if 0
			if(StrSnd.u32RxFreq<RECFREQ460)
			{
				HRC5000Send_cmd(0x01,0xf0);
			}
			else
			{
				HRC5000Send_cmd(0x01,0xb0);
			}
#endif

			RF_BLL_RxInit(&StrSnd);
			*WORK_MODE |= 0x20;//时隙模式
			PC_BLL_TunerMFToPc(Buf, (DH_U8*) 0, 0, Buf[2], UFW_REQREAD);
			PC_BLL_TuneWriteStr(  TUNER_APCADJUSTADDR, Buf + 8,33);

//			FLASH_INTF_WriteStr( TUNER_APCADJUSTADDR,Buf+8,33);

		}
		else
		{
			PC_BLL_TunerMFToPc(Buf, (DH_U8*) 0, 0, Buf[2], UFR_RECERR);
		}
	}
}
/*******************************************************************************
 *函数名:PC_BLL_Digital_RSSISet
 *函数说明:中频解调的数字RSSI设置值
 *参数说明:
 *返回值:无
 *创建人:liuzhiheng
 *创建日期:2012/11/18
 *******************************************************************************/
void PC_BLL_Digital_RSSISet(DH_U8 * Buf, DH_U8 Len, DH_U8 err)
{
//	STR_CODESW StrCode=GetPllStat();
	//OS_ERR oserr;
	PcTunerFlg = 0;
//	//OSTaskResume(TASK_HRC5000_RF_PRIOR_);
	//OSTaskResume(&TaskRFTCB, &oserr);
#if(!RF_MOD_AF_FM)
	/*中频解调数字的的RSSI*/
	PC_BLL_DrvRssiAdjustSet(Buf, Len, err);
#endif
}
/*******************************************************************************
 *函数名:
 *函数说明:
 *参数说明:
 *返回值:无
 *创建人:
 *创建日期:2012\09\08
 *******************************************************************************/
void PC_BLL_ModeIQSet(DH_U8* Buf, DH_U8 Len, DH_U8 err)
{
	DH_U8 Buf_[4];
//	PLL_INFO_S StrSnd;
	if (PCINFERR_OK == err)
	{
		if (Buf[3] == 0x01)
		{
#if 0
			StrSnd= PllValueRecGet();
			DRV_SetDacAPC(0);
			DRV_SetDacTV(StrSnd.u16TvVal);
			Rf5TcLowExt();
			RfAntSwLowExt();
			Rf5RcHighExt();
			SetRda1846SRec((DH_U32)(435*1000000)); /*480us*/
#endif
			FLASH_INTF_ReadStr(  TUNER_MODEIQSET, Buf_, 4);
			PC_BLL_TunerMFToPc(Buf, Buf_, 4, MODEIQSET, UFR_RECOK);
		}
		else if (Buf[3] == 0x02)
		{
			PC_BLL_TuneWriteStr(  TUNER_MODEIQSET, Buf + 8, 4);
//			FLASH_INTF_WriteStr(  TUNER_MODEIQSET, Buf + 8, 4);
			//	HRC6000Send_cmd(TxRegCmd,0x94, Buf+8,2);
			//	HRC6000Send_cmd(TxRegCmd,0x97, Buf+10,2);
			PC_BLL_TunerMFToPc(Buf, (DH_U8*) 0, 0, MODEIQSET, UFW_REQREAD);
#if 0
			/*byliuzhiheng重新调整代码*/
			Rf5RcLowExt();
			Rf5TcHighExt();
			SetRda1846Send((DH_U32)(435*1000000));
			OpenTxFreqSend();
			RfAntSwHighExt();
			DRV_SetDacAPC(2000);
#endif
		}
	}
	else
	{
		PC_BLL_TunerMFToPc(Buf, (DH_U8*) 0, 0, MODEIQSET, UFW_RECERR);
	}
}
/*******************************************************************************
 *函数名:
 *函数说明:
 *参数说明:
 *返回值:无
 *创建人:
 *创建日期:2012\09\08
 *******************************************************************************/
void PC_BLL_FMRxDetectset(DH_U8* Buf, DH_U8 Len, DH_U8 err)
{
	DH_U8 Buf_[3];
//	DH_U8 Bandreg=0;
	if (PCINFERR_OK == err)
	{
		if (Buf[3] == 0x01)
		{
			if (Buf[8] == 0x00)
			{
				FLASH_INTF_ReadStr( 
						TUNER_TUNER_FMRXDETECTSETNARROW, Buf_ + 1, 1);
			}
			else
			{
				FLASH_INTF_ReadStr( 
						TUNER_TUNER_FMRXDETECTSETWIDE, Buf_ + 1, 1);
			}
			Buf_[0] = Buf[8];
			PC_BLL_TunerMFToPc(Buf, Buf_, 2, FMRXDETECTSET, UFR_RECOK);
		}
		else if (Buf[3] == 0x02)
		{

			if (Buf[8] == 0x00)
			{
				*FM_BANDWIDTH &= 0xF0;

				PC_BLL_TuneWriteStr( 
						TUNER_TUNER_FMRXDETECTSETNARROW, Buf + 9, 1);
			}
			else
			{
				*FM_BANDWIDTH |= 0x0F;
				PC_BLL_TuneWriteStr( TUNER_TUNER_FMRXDETECTSETWIDE, Buf + 9, 1);
			}
			*FM_DEV_COEF &= 0xFFFF00FF;
			*FM_DEV_COEF |= *(Buf + 9) << 8; ///FM收端调制频偏系数

			PC_BLL_TunerMFToPc(Buf, (DH_U8*) 0, 0, FMRXDETECTSET, UFW_REQREAD);
		}
	}
	else
	{
		PC_BLL_TunerMFToPc(Buf, (DH_U8*) 0, 0, FMRXDETECTSET, UFW_RECERR);
	}
}
/*******************************************************************************
 *函数名:
 *函数说明:
 *参数说明:
 *返回值:无
 *创建人:
 *创建日期:2012\09\08
 *******************************************************************************/
void PC_BLL_FMTxDetectset(DH_U8* Buf, DH_U8 Len, DH_U8 err)
{
	DH_U8 Buf_[6];
//	DH_U8 Bandreg=0;
	if (PCINFERR_OK == err)
	{
		if (Buf[3] == 0x01)
		{
			if (Buf[8] == 0x00)
			{
				FLASH_INTF_ReadStr( 
						TUNER_TUNER_FMTXDETECTSETNARROW, Buf_ + 1, 3);
			}
			else
			{
				FLASH_INTF_ReadStr( 
						TUNER_TUNER_FMTXDETECTSETWIDE, Buf_ + 1, 3);
			}
			Buf_[0] = Buf[8];
			PC_BLL_TunerMFToPc(Buf, Buf_, 4, FMTXDETECTSET, UFR_RECOK);
		}
		else if (Buf[3] == 0x02)
		{
			if (Buf[8] == 0x00)
			{
				*FM_BANDWIDTH &= 0xF0;

				PC_BLL_TuneWriteStr( 	TUNER_TUNER_FMTXDETECTSETNARROW, Buf + 9, 3);
			}
			else
			{
				*FM_BANDWIDTH |= 0x0F;
				PC_BLL_TuneWriteStr( 
						TUNER_TUNER_FMTXDETECTSETWIDE, Buf + 9, 3);
			}
			*FM_DEV_COEF &= 0xFF00FF00;
			*FM_DEV_COEF |= *(Buf + 9);      ///FM发端调制频偏系数
			*FM_DEV_COEF |= *(Buf + 10) << 16; ///FM限幅调制系数HRC5000Send_cmd(0x3f, *(Buf+10));

			*SUBVOICE_COEF &= 0xFFFF00FF;
			*SUBVOICE_COEF |= *(Buf + 11) << 8; //FM亚音频调制频

			PC_BLL_TunerMFToPc(Buf, (DH_U8*) 0, 0, FMTXDETECTSET, UFW_REQREAD);
		}
	}
	else
	{
		PC_BLL_TunerMFToPc(Buf, (DH_U8*) 0, 0, FMTXDETECTSET, UFW_RECERR);
	}
}

/*
 1846输出功率分段设置,0x0abit[14-11]
 */
extern DH_U8 g_TxPower;
void PC_BLL_Pc1846PaOutSet(DH_U8* Buf, DH_U8 Len, DH_U8 err)
{
	DH_U8 i;
	DH_U8 Buf_[17];
	DH_U8 Signal[36];
	//DH_U8 Ampbias;
//	DH_U16 Afc;

	PLL_INFO_S StrSnd;
	//OS_ERR oserr;
	Str_PCctunertxstat.FreqChexkse = 0;
	Str_PCctunertxstat.HighPowerset = 0;
	Str_PCctunertxstat.LowPowerset = 0;
	Str_PCctunertxstat.Dmodbalanceset = 0;
	PcTunerFlg = 1;
//	//OSTaskSuspend(TASK_HRC5000_RF_PRIOR_);
	//OSTaskSuspend(&TaskRFTCB, &oserr);
	if (Buf[3] == 1)
	{
		Str_PCctunertxstat.RF_OUTset = 0;
		StrSnd = RF_BLL_GetPll();
		RF_BLL_RxInit(&StrSnd);

		FLASH_INTF_ReadStr(  TUNER_TPLDPLNARROWADDR, Buf_, 9);
		PC_BLL_TunerMFToPc(Buf, Buf_, 9, Buf[2], Buf[3]);
	}
	else if (Buf[3] == 2)
	{
		if (PCINFERR_OK == err)
		{
			StrSnd = RF_BLL_GetPll();
			if (Str_PCctunertxstat.FreqBuf != Buf[8])
			{
				Str_PCctunertxstat.FreqBuf = Buf[8];
				Str_PCctunertxstat.RF_OUTset = 0;
			}

			StrSnd.u32TxFreq = StrSnd.u32RxFreq = g_u32FreqSectorList[Buf[8]];
			DH_U8 reg0a = Buf[8 + Buf[8]];

			StrSnd.u16ApcVal = StrSnd.u16ApcLowVal = 1000;
			RF_BLL_SavePll(StrSnd);
			RF_INTF_SetIQAmpNBias(&StrSnd);
			//StrSnd.u16ApcVal = StrSnd.u16ApcLowVal;
//			RF_BLL_SavePll(StrSnd);结束
//			Ampbias = ampbais_autoset(StrSnd.u16AmpbiasVal);//Ampbias = AmpbaisAutSet(StrSnd.Reg02);Ampbias = AmpbaisAutSet(StrSnd.Reg02);
//			Afc = afc_autoset(StrSnd.u16AfcVal);    //Afc = AfcAutSet(StrSnd.u16AfcVal);
			if (1)
			{
				if (g_FMIC_Sel == FM_IC_RDA1846S)
				{
					DH_U16 rf_outset = 0, rf_outset1 = 0;
					rf_outset1 = reg0a;
					rf_outset = RDA1846S_INTF_ReadReg(0x0a);
					rf_outset = (rf_outset & 0x7ff) | (rf_outset1 << 11);/*bit14-11 1000 0111 1111 1111 RF power control */
					RDA1846S_INTF_WriteReg(RDA_REG_0AH, rf_outset);
				}
				else
				{
					g_TxPower = reg0a;
					RFIC_INTF_TxPower(reg0a);
				}

				if (Str_PCctunertxstat.RF_OUTset == 0)
				{
					Str_PCctunertxstat.RF_OUTset = 1;
					RF_BLL_EnableTx();
				}

				for (i = 0; i < 36; i++)
				{
					Signal[i] = 0x5f;
				}
				//HRC_INTF_StartTxRAM((DH_U8*) Signal);
				PC_BLL_TunerMFToPc(Buf, (DH_U8*) 0, 0, Buf[2], Buf[3]);
				PC_BLL_TuneWriteStr(  TUNER_TPLDPLNARROWADDR,Buf + 8, 9);
//				FLASH_INTF_WriteStr( TUNER_TPLDPLNARROWADDR,Buf+8,9);

			}
			else
			{
				RF_BLL_RxInit(&StrSnd);
			}
		}
		else
		{
			PC_BLL_TunerMFToPc(Buf, (DH_U8*) 0, 0, Buf[2], UFR_RECERR);
		}
	}
	else
	{
		PC_BLL_TunerMFToPc(Buf, (DH_U8*) 0, 0, Buf[2], UFR_RECERR);
	}
}

//命令格式:68 21 40 01 xx xx xx xx addr xx
//命令格式:68 21 40 02 xx xx xx xx addr vh vl xx//设置
void PC_BLL_PcOption1846S(DH_U8* RecBuf)
{
	if (2 == RecBuf[3])
	{
		RDA1846S_INTF_SetReg(RecBuf[8], RecBuf[9], RecBuf[10]);
		PC_BLL_TunerMFToPc(RecBuf, (DH_U8*) 0, 0, RDA1846SREGSET, UFR_BASEINFO);
	}
	else if (1 == RecBuf[3])
	{
		DH_U8 SBuf[3];
		DH_U16 tmp;
		tmp = RDA1846S_INTF_ReadReg(RecBuf[8]);
		SBuf[0] = RecBuf[8];
		SBuf[2] = (DH_U8) (tmp);
		SBuf[1] = (DH_U8) (tmp >> 8);
		PC_BLL_TunerMFToPc(RecBuf, SBuf, 3, RDA1846SREGSET, UFR_RECOK);
	}
}

/*******************************************************************************
 * 函数名  : PC_BLL_PcOptionGPSmsg
 * 描  述  :
 * 输  入  : 无
 * 输  出  : 无
 * 返回值  : 无
 *******************************************************************************/
void PC_BLL_PcOptionGPSmsg(DH_U8* Buf)
{
#if 0
	DH_U8 msgbuf[1508];
	DH_U16 len;
	if (1 == Buf[3])
	{
		DH_U8 mID = Buf[8];
		DH_U32 id = 0;
		NoteTypeSelect(NOTETYPE_INBOX);
		id = MSG_APP_MsgGetNumber(NOTETYPE_INBOX, mID);
		if (id)
		{
			MSG_APP_MsgGetContent(NOTETYPE_INBOX, mID);
			g_DispBufMax = strlen((char const*) g_InputBuf) + 1;
//			INPUTMTHD_APP_DisColRowWhereInit(g_DispBufMax-1);
			len = (DH_U32) MSG_APP_MsgRecL(id, g_InputBuf, g_DispBufMax,
					msgbuf);
			PC_BLL_MakeSDSend(len, msgbuf, 0);
		}
		else
			PC_BLL_TunerMFToPc(Buf, (DH_U8*) 0, 0, Buf[2], UFR_RECERR);

	}
	else if (2 == Buf[3])
	{
		MSG_APP_MsgDelete(g_ItemBackup[3] + 1, NOTETYPE_INBOX);
		PC_BLL_TunerMFToPc(Buf, (DH_U8*) 0, 0, Buf[2], UFR_RECOK);
	}
	else if (3 == Buf[3])
	{
		MSG_APP_MsgDeleteAll(NOTETYPE_INBOX);
		PC_BLL_TunerMFToPc(Buf, (DH_U8*) 0, 0, Buf[2], UFR_RECERR);
	}
#endif
}

//读命令格式:68 21 48 01 xx xx xx xx addr xx->68 21 48 01 87 CD 00 00 10 10
//写命令格式:68 21 48 02 xx xx xx xx addr vh xx->68 21 48 02 87 CD 00 00 35 10 3e 3f
void PC_BLL_PcOptionC5000S(DH_U8* RecBuf)
{

}

DH_VOID PC_BLL_PcAfcAutCreate(DH_U8* Buf, DH_U8* Ampbias, DH_U16* Afc)
{
	DH_F32 VoltageRef;

	*Ampbias = (Buf[1] << 8) + Buf[0];
	VoltageRef = 1.228 + (*Ampbias) * 0.422 / 128;
	*Afc = (DH_U16) (VoltageRef * 1023 / 3.3 + 0.5);
}

/*******************************************************************************
 *函数名:PC_BLL_DrvRssiAdjustSet
 *函数说明:上位APC设置
 Addr=8
 |Sel(Addr+0)|403(Addr+(1~2))|414(Addr+(3~4))|425(Addr+(5~6))|436(Addr+(7~8))/
 |447(Addr+(9~10))|458(Addr+11~12))|469(Addr+(13~14))|
 *参数说明:
 *返回值:无
 *创建人:yufeng
 *创建日期:2012/11/18
 *******************************************************************************/
void PC_BLL_DrvRssiAdjustSet(DH_U8 * Buf, DH_U8 Len, DH_U8 err)
{
	Str_PCctunertxstat.FreqChexkse = 0;
	Str_PCctunertxstat.HighPowerset = 0;
	Str_PCctunertxstat.LowPowerset = 0;
	Str_PCctunertxstat.Dmodbalanceset = 0;
	Str_PCctunertxstat.RF_OUTset = 0;
	Str_PCctunertxstat.FreqBuf = 0;

	DH_U8 Buf_[81];
//	DH_U16 Afc;
	DH_U8 RSSIfre = 0;
	DH_U8 i = 0;
	PLL_INFO_S StrSnd = RF_BLL_GetPll();
	if (Buf[3] == 1)
	{
		RF_BLL_RxInit(&StrSnd);

		FLASH_INTF_ReadStr(  TUNER_RSSIBADDR, Buf_, 81);

		PC_BLL_TunerMFToPc(Buf, Buf_, 81, Buf[2], UFR_RECOK);
	}
	else if (Buf[3] == 2)
	{
		if (PCINFERR_OK == err)
		{
			RSSIfre = Buf[8] & 0x0f;

			StrSnd.u32TxFreq = StrSnd.u32RxFreq = g_u32FreqSectorList[RSSIfre];

			if ((RSSIfre < 9) && (RSSIfre > 0))
			{
				for (i = 0; i < 5; i++)
				{
					SignalBuf[i] = Buf[9 + (RSSIfre - 1) * 10 + 2 * i + 1];
					SignalBuf[i] = SignalBuf[i] << 8;
					SignalBuf[i] += Buf[9 + (RSSIfre - 1) * 10 + 2 * i];
				}
			}

			Sq_flag = 1;
			StrSnd = PLL_BLL_GetRfVal(CHN_BLL_GetAttr(), StrSnd.u32TxFreq,
					StrSnd.u32RxFreq);

			RF_BLL_RxInit(&StrSnd);

			Sq_flag = 0;

			PC_BLL_TunerMFToPc(Buf, (DH_U8*)0, 0,Buf[2], UFW_REQREAD);

			PC_BLL_TuneWriteStr(  TUNER_RSSIBADDR, Buf + 8, 81);
			//PC_BLL_TunerMFToPc(Buf,Buf_, 81, RSSILEVELSET, UFR_RECOK);
		}
		else
		{
			PC_BLL_TunerMFToPc(Buf, (DH_U8*)0, 0,Buf[2], UFR_RECERR);
		}
	}
	else if (Buf[3] == 3)/***时时读取当前rssi的值**/
	{
		if (PCINFERR_OK == err)
		{
			Buf_[0] = (DH_U8) (g_CurSignalVal);
			Buf_[1] = (DH_U8) (g_CurSignalVal >> 8);
			PC_BLL_TunerMFToPc(Buf, Buf_, 2, Buf[2], 0x03);

		}
		else
		{
#if RF_MOD_AF_FM
			PC_BLL_TunerMFToPc(Buf, (DH_U8*)0, 0,Buf[2], UFR_RECERR);
#else
			PC_BLL_TunerMFToPc(Buf, (DH_U8*) 0, 0, Buf[2], UFR_RECERR);
#endif
		}
	}
}
/*******************************************************************************
 *函数名:PC_BLL_FmRSSIadjustSet
 *函数说明:上位APC设置
 Addr=8
 |Sel(Addr+0)|403(Addr+(1~2))|414(Addr+(3~4))|425(Addr+(5~6))|436(Addr+(7~8))/
 |447(Addr+(9~10))|458(Addr+11~12))|469(Addr+(13~14))|
 *参数说明:
 *返回值:无
 *创建人:yufeng
 *创建日期:2012/11/18
 *******************************************************************************/
void PC_BLL_FmRssiAdjustSet(DH_U8 * Buf, DH_U16 Len, DH_U8 err)
{
	Str_PCctunertxstat.FreqChexkse = 0;
	Str_PCctunertxstat.HighPowerset = 0;
	Str_PCctunertxstat.LowPowerset = 0;
	Str_PCctunertxstat.Dmodbalanceset = 0;
	Str_PCctunertxstat.RF_OUTset = 0;
	Str_PCctunertxstat.FreqBuf = 0;

	DH_U8 Buf_[81];
	DH_U16 Afc;
	DH_U8 RSSIfre = 0;
	DH_U8 i = 0;
	PLL_INFO_S StrSnd = RF_BLL_GetPll();
	if (Buf[3] == 1)
	{
		Afc = /*afc_autoset*/(StrSnd.u16AfcVal);//Afc = AfcAutSet(StrSnd.u16AfcVal);
		RF_BLL_RxInit(&StrSnd);

		FLASH_INTF_ReadStr(  TUNER_FMRSSIBADDR, Buf_, 81);
		PC_BLL_TunerMFToPc(Buf, Buf_, 81, Buf[2], UFR_RECOK);
	}
	else if (Buf[3] == 2)
	{
		if (PCINFERR_OK == err)
		{
			RSSIfre = Buf[8] & 0x0f;

//			FLASH_INTF_WriteStr(  TUNER_FMRSSIBADDR, Buf + 8, 81);
//			g_PcTunerFeqSelect = 1;
//			PC_BLL_Copy8Sectordata();
//			g_PcTunerFeqSelect = 0;
			StrSnd.u32TxFreq = StrSnd.u32RxFreq = g_u32FreqSectorList[RSSIfre];
			if ((RSSIfre < 9) && (RSSIfre > 0))
			{
				for (i = 0; i < 5; i++)
				{
					SignalBuf[i] = Buf[9 + (RSSIfre - 1) * 10 + 2 * i + 1];
					SignalBuf[i] = SignalBuf[i] << 8;
					SignalBuf[i] += Buf[9 + (RSSIfre - 1) * 10 + 2 * i];
				}
			}

			Sq_flag = 1;
			StrSnd = PLL_BLL_GetRfVal(CHN_BLL_GetAttr(), StrSnd.u32TxFreq,
					StrSnd.u32RxFreq);
			RF_BLL_SavePll(StrSnd);
			RF_BLL_RxInit(&StrSnd);

			Sq_flag = 0;
//			FLASH_INTF_WriteStr( TUNER_FMRSSIBADDR,Buf+8,81);
			PC_BLL_TunerMFToPc(Buf, (DH_U8*) 0, 0, Buf[2], UFW_REQREAD);
			PC_BLL_TuneWriteStr(  TUNER_FMRSSIBADDR, Buf + 8, 81);
		}
		else
		{
			PC_BLL_TunerMFToPc(Buf, (DH_U8*) 0, 0, Buf[2], UFR_RECERR);
		}
	}
	else if (Buf[3] == 3)
	{
		if (PCINFERR_OK == err)
		{
			Buf_[0] = (DH_U8) (g_CurSignalVal);
			Buf_[1] = (DH_U8) (g_CurSignalVal >> 8);

			PC_BLL_TunerMFToPc(Buf, Buf_, 2, Buf[2], 0x03);
		}
		else
		{
			PC_BLL_TunerMFToPc(Buf, (DH_U8*) 0, 0, Buf[2], UFR_RECERR);
		}
	}
}
/*******************************************************************************
 *函数名:PC_BLL_RSSIadjustSet
 *函数说明:上位APC设置
 Addr=8
 |Sel(Addr+0)|403(Addr+(1~2))|414(Addr+(3~4))|425(Addr+(5~6))|436(Addr+(7~8))/
 |447(Addr+(9~10))|458(Addr+11~12))|469(Addr+(13~14))|
 *参数说明:
 *返回值:无
 *创建人:yufeng
 *创建日期:2012/11/18
 *******************************************************************************/
void PC_BLL_RSSIAdjustSet(DH_U8 * Buf, DH_U8 Len, DH_U8 err)
{
	//OS_ERR oserr;
//	STR_CODESW StrCode = GetPllStat();
	PcTunerFlg = 0;
//	//OSTaskResume(TASK_HRC5000_RF_PRIOR_);
	//OSTaskResume(&TaskRFTCB, &oserr);
#if RF_MOD_AF_FM
	if(StrCode.Attr==PLLSTAT_D)
	{	DrvRssiAdjustSet(Buf,Len,err);}
	else
	{
		if(1)
		{	DrvRssiAdjustSet(Buf,Len,err);}
		else
		{	FmRssiAdjustSet(Buf,Len,err);}
	}
#else
	/*中频解调模拟的RSSI*/
	PC_BLL_FmRssiAdjustSet(Buf, Len, err);
#endif
}

/*******************************************************************************
 *函数名:PC_BLL_PcTvSet
 *函数说明:上位机电调设置
 Addr=8
 |Sel(Addr+0)|403(Addr+(1~2))|414(Addr+(3~4))|425(Addr+(5~6))|436(Addr+(7~8))/
 |447(Addr+(9~10))|458(Addr+11~12))|469(Addr+(13~14))|
 *参数说明:
 *返回值:无
 *创建人:yufeng
 *创建日期:2012/11/18
 *******************************************************************************/
void PC_BLL_PcTvSet(DH_U8* Buf, DH_U16 Len, DH_U8 err)
{
	PLL_INFO_S Str;
	DH_U8 Buf_[17];
	DH_U8 Ampbias;
	DH_U16 Afc;
	if (Buf[3] == 1)
	{
		FLASH_INTF_ReadStr(  TUNER_TVADDR, Buf_, 17);
		PC_BLL_TunerMFToPc(Buf, Buf_, 17, Buf[2], UFR_RECOK);
	}
	else if (Buf[3] == 2)
	{
		if (PCINFERR_OK == err)
		{
			Str = RF_BLL_GetPll();

//			FLASH_INTF_WriteStr(  TUNER_TVADDR, Buf + 8, 17);

			if ((Buf[8] > 0) && (Buf[8] <= 8))
			{
				Str.u32TxFreq = g_u32FreqSectorList[Buf[8]];
				Str.u32RxFreq = g_u32FreqSectorList[Buf[8]];
				Str.u16TvVal = (Buf[2 * Buf[8] + 8] << 8) + Buf[2 * Buf[8] + 7];
			}
			else
			{
				Str.u32TxFreq = 0;
				Str.u32RxFreq = 0;
				Str.u16TvVal = 0;
			}

#if(0)
			if(Str.u32RxFreq<RECFREQ460)
			{
				HRC5000Send_cmd(0x01,0xf0);
			}
			else
			{
				HRC5000Send_cmd(0x01,0xb0);
			}
#endif

			RF_BLL_SavePll(Str);

		//	Ampbias = /*ampbais_autoset*/(Str.u16AmpbiasVal);//Ampbias = AmpbaisAutSet(Str.Reg02);

		//	Afc = /*afc_autoset*/(Str.u16AfcVal);

			RF_BLL_RxInit(&Str);

//		    FLASH_INTF_WriteStr( TUNER_TVADDR,Buf+8,17);
			PC_BLL_TunerMFToPc(Buf, (DH_U8*) 0, 0, Buf[2], Buf[3]);
			PC_BLL_TuneWriteStr(  TUNER_TVADDR, Buf + 8, 17);
		}
	}
	else
	{
		PC_BLL_TunerMFToPc(Buf, (DH_U8*) 0, 0, Buf[2], UFR_RECERR);
	}
}

#endif



/*******************************************************************************
 *函数名:PcHighPowerSet
 *函数说明:上位机高功率设置
 Addr=8
 |Sel(Addr+0)|403(Addr+(1~2))|414(Addr+(3~4))|425(Addr+(5~6))|436(Addr+(7~8))/
 |447(Addr+(9~10))|458(Addr+11~12))|469(Addr+(13~14))|
 *参数说明:
 *返回值:无
 *创建人:yufeng
 *创建日期:2012/11/18
 *******************************************************************************/
void PC_BLL_PcHighPowerSet(DH_U8* Buf, DH_U16 Len, DH_U8 err)
{
	DH_U8 i;
	DH_U8 Buf_[17];
	DH_U8 Signal[36];
	PLL_INFO_S StrSnd;
	DH_U8 Ampbias;
	DH_U16 Afc;
	//OS_ERR oserr;
	PcTunerFlg = 1;
	//OSTaskSuspend(&TaskRFTCB, &oserr);
//	//OSTaskSuspend(TASK_HRC5000_RF_PRIOR_);
	Str_PCctunertxstat.FreqChexkse = 0;
	Str_PCctunertxstat.LowPowerset = 0;
	Str_PCctunertxstat.Dmodbalanceset = 0;
	Str_PCctunertxstat.RF_OUTset = 0;
	if (Buf[3] == TUNER_CMD_READ)
	{
		if (Str_PCctunertxstat.HighPowerset == 1)
		{
			Str_PCctunertxstat.HighPowerset = 0;
			StrSnd = RF_BLL_GetPll();
			RF_BLL_RxInit(&StrSnd);
		}
		FLASH_INTF_ReadStr(  TUNER_HPOWERADDR, Buf_, 17);
		PC_BLL_TunerMFToPc(Buf, Buf_, 17, Buf[2], Buf[3]);
	}
	else if (Buf[3] == TUNER_CMD_WRITE)
	{
		if (PCINFERR_OK == err)
		{
			StrSnd = RF_BLL_GetPll();

//			FLASH_INTF_WriteStr(  TUNER_HPOWERADDR, Buf + 8, 17);
			if (Str_PCctunertxstat.FreqBuf != Buf[8])
			{
				Str_PCctunertxstat.FreqBuf = Buf[8];
				Str_PCctunertxstat.HighPowerset = 0;
			}
//			g_PcTunerFeqSelect=1;
//			PC_BLL_Copy8Sectordata();
//			g_PcTunerFeqSelect=0;
			if ((Buf[8] > 0) && (Buf[8] <= 8))
			{
				StrSnd.u32TxFreq = g_u32FreqSectorList[Buf[8]];
				StrSnd.u32RxFreq = g_u32FreqSectorList[Buf[8]];
				StrSnd.u16ApcHighVal = (Buf[8 + (Buf[8] * 2)] << 8)
						+ Buf[8 + (Buf[8] * 2) - 1];
			}
			else
			{
				StrSnd.u32TxFreq = 0;
				StrSnd.u32RxFreq = 0;
				StrSnd.u16ApcHighVal = 0;
			}

			StrSnd.u16ApcVal = StrSnd.u16ApcHighVal;
			RF_BLL_SavePll(StrSnd);
//			Ampbias = /*ampbais_autoset*/(StrSnd.u16AmpbiasVal); //Ampbias = AmpbaisAutSet(StrSnd.Reg02);
//			Afc = /*afc_autoset*/(StrSnd.u16AfcVal); //Afc = AfcAutSet(StrSnd.u16AfcVal);
			if (1)
			{
				RF_INTF_SetIQAmpNBias(&StrSnd);

				if (Str_PCctunertxstat.HighPowerset == 0)
				{
					Str_PCctunertxstat.HighPowerset = 1;
					RF_BLL_EnableTx();
				}
				RF_INTF_APCSet(StrSnd.u16ApcVal);
				for (i = 0; i < 36; i++)
				{
					Signal[i] = 0x5f;
				}
				//HRC_INTF_StartTxRAM((DH_U8*) Signal);
				PC_BLL_TunerMFToPc(Buf, (DH_U8*) 0, 0, Buf[2], Buf[3]);
				PC_BLL_TuneWriteStr(  TUNER_HPOWERADDR, Buf + 8, 17);
//				FLASH_INTF_WriteStr( TUNER_HPOWERADDR,Buf+8,17);

			}
			else
			{
				RF_BLL_RxInit(&StrSnd);
			}
		}
		else
		{
			PC_BLL_TunerMFToPc(Buf, (DH_U8*) 0, 0, Buf[2], UFR_RECERR);
		}
	}
	else
	{
		PC_BLL_TunerMFToPc(Buf, (DH_U8*) 0, 0, Buf[2], UFR_RECERR);
	}
}

/*******************************************************************************
 *函数名:PC_BLL_PcLowPowerSet
 *函数说明:上位机低功率设置
 Addr=8
 |Sel(Addr+0)|403(Addr+(1~2))|414(Addr+(3~4))|425(Addr+(5~6))|436(Addr+(7~8))/
 |447(Addr+(9~10))|458(Addr+11~12))|469(Addr+(13~14))|
 *参数说明:
 *返回值:无
 *创建人:yufeng
 *创建日期:2012/11/18
 *******************************************************************************/
void PC_BLL_PcLowPowerSet(DH_U8* Buf, DH_U16 Len, DH_U8 err)
{
	DH_U8 i;
	DH_U8 Buf_[17];
//	DH_U8 Signal[36];
	PLL_INFO_S StrSnd;
	DH_U8 Ampbias;
	DH_U16 Afc;
	//OS_ERR oserr;
	PcTunerFlg = 1;
//	//OSTaskSuspend(TASK_HRC5000_RF_PRIOR_);
	//OSTaskSuspend(&TaskRFTCB, &oserr);
	Str_PCctunertxstat.FreqChexkse = 0;
	Str_PCctunertxstat.HighPowerset = 0;
	Str_PCctunertxstat.Dmodbalanceset = 0;
	Str_PCctunertxstat.RF_OUTset = 0;
	if (Buf[3] == TUNER_CMD_READ)
	{
		Str_PCctunertxstat.LowPowerset = 0;
		StrSnd = RF_BLL_GetPll();

		RF_BLL_RxInit(&StrSnd);

		FLASH_INTF_ReadStr(  TUNER_LPOWERADDR, Buf_, 17);
		PC_BLL_TunerMFToPc(Buf, Buf_, 17, Buf[2], Buf[3]);
	}
	else if (Buf[3] == TUNER_CMD_WRITE)
	{
		if (PCINFERR_OK == err)
		{
			StrSnd = RF_BLL_GetPll();

//			FLASH_INTF_WriteStr(  TUNER_LPOWERADDR, Buf + 8, 17);
			if (Str_PCctunertxstat.FreqBuf != Buf[8])
			{
				Str_PCctunertxstat.FreqBuf = Buf[8];
				Str_PCctunertxstat.LowPowerset = 0;
			}

			if ((Buf[8] > 0) && (Buf[8] <= 8))
			{
				StrSnd.u32TxFreq = g_u32FreqSectorList[Buf[8]];
				StrSnd.u32RxFreq = g_u32FreqSectorList[Buf[8]];
				StrSnd.u16ApcLowVal = (Buf[2 * Buf[8] + 8] << 8)
						+ Buf[2 * Buf[8] + 7];
			}
			else
			{
				StrSnd.u32TxFreq = 0;
				StrSnd.u32RxFreq = 0;
				StrSnd.u16ApcLowVal = 0;
			}
			StrSnd.u16ApcVal = StrSnd.u16ApcLowVal;
			RF_BLL_SavePll(StrSnd);
			//Ampbias =/* ampbais_autoset*/(StrSnd.u16AmpbiasVal); //Ampbias = AmpbaisAutSet(StrSnd.Reg02);
			//Afc = /*afc_autoset*/(StrSnd.u16AfcVal);
			if (1)
			{
				RF_INTF_SetIQAmpNBias(&StrSnd);

				if (Str_PCctunertxstat.LowPowerset == 0)
				{
					Str_PCctunertxstat.LowPowerset = 1;
					RF_BLL_EnableTx();
				}
				RF_INTF_APCSet(StrSnd.u16ApcVal);

//				for (i = 0; i < 36; i++)
//				{
//					Signal[i] = 0x5f;
//				}
				//HRC_INTF_StartTxRAM((DH_U8*) Signal);
				PC_BLL_TunerMFToPc(Buf, (DH_U8*) 0, 0, Buf[2], Buf[3]);
				PC_BLL_TuneWriteStr(  TUNER_LPOWERADDR, Buf + 8, 17);

				//				FLASH_INTF_WriteStr( TUNER_LPOWERADDR,Buf+8,17);

			}
			else
			{
				RF_BLL_RxInit(&StrSnd);
			}
		}
		else
		{
			PC_BLL_TunerMFToPc(Buf, (DH_U8*) 0, 0, Buf[2], UFR_RECERR);
		}
	}
	else
	{
		PC_BLL_TunerMFToPc(Buf, (DH_U8*) 0, 0, Buf[2], UFR_RECERR);
	}
}

/*******************************************************************************
 *函数名:PC_BLL_PcReadRssi
 *函数说明:Pc读取RSSI值
 *参数说明:
 *返回值:无
 *创建人:yufeng
 *创建日期:2012/11/18
 *******************************************************************************/
void PC_BLL_PcReadRssi(DH_U8* Buf, DH_U16 Len, DH_U8 err)
{
	//OS_ERR oserr;
	Str_PCctunertxstat.FreqChexkse = 0;
	Str_PCctunertxstat.HighPowerset = 0;
	Str_PCctunertxstat.LowPowerset = 0;
	Str_PCctunertxstat.Dmodbalanceset = 0;
	Str_PCctunertxstat.RF_OUTset = 0;
	Str_PCctunertxstat.FreqBuf = 0;
	PcTunerFlg = 0;
//	//OSTaskResume(TASK_HRC5000_RF_PRIOR_);
	//OSTaskResume(&TaskRFTCB, &oserr);
	DH_U16 SignalIntensAdStat1 = 0;
	DH_U8 i;
	DH_U8 SBuf[3];

	if (PCINFERR_OK == err)
	{
		if (Buf[8] == 0x01)
		{
			for (i = 0; i < 3; i++)
			{
				SignalIntensAdStat1 +=
						/**RSSI_BIAS_VALUE;*/RF_INTF_GetSignal();

				Delay_ms(10);
			}
			SignalIntensAdStat1 /= 3;

			SBuf[0] = 0x02;
			SBuf[1] = (DH_U8) (SignalIntensAdStat1);
			SBuf[2] = (DH_U8) (SignalIntensAdStat1 >> 8);
//LCD_BLL_printf_ext(5,5,LCD_Cyan,"RSSI:%03d",SignalIntensAdStat1);
			PC_BLL_TunerMFToPc(Buf, SBuf, 3, Buf[2], 0);
		}
	}
	else
	{
		PC_BLL_TunerMFToPc(Buf, (DH_U8*) 0, 0, Buf[2], err);
	}
}

/*******************************************************************************
 *函数名:PC_BLL_SqAdjustSet
 *函数说明:上位APC设置
 Addr=8
 |Sel(Addr+0)|403(Addr+(1~2))|414(Addr+(3~4))|425(Addr+(5~6))|436(Addr+(7~8))/
 |447(Addr+(9~10))|458(Addr+11~12))|469(Addr+(13~14))|
 *参数说明:
 *返回值:无
 *创建人:yufeng
 *创建日期:2012/11/18
 *******************************************************************************/
void PC_BLL_SqAdjustSet(DH_U8 * Buf, DH_U8 Len, DH_U8 err)
{
//OS_ERR oserr;
	Str_PCctunertxstat.FreqChexkse = 0;
	Str_PCctunertxstat.HighPowerset = 0;
	Str_PCctunertxstat.LowPowerset = 0;
	Str_PCctunertxstat.Dmodbalanceset = 0;
	Str_PCctunertxstat.RF_OUTset = 0;
	Str_PCctunertxstat.FreqBuf = 0;
	PcTunerFlg = 0;
//	OSTaskResume(TASK_HRC5000_RF_PRIOR_);
//	OSTaskResume(&TaskRFTCB, &oserr);
	DH_U8 Sqfre;
	DH_U8 Buf_[145];
	PLL_INFO_S StrSnd = RF_BLL_GetPll();

	if (Buf[3] == 1)
	{
//		RF_BLL_RxInit(StrSnd);
		FLASH_INTF_ReadStr( TUNER_SQWDBADDR, Buf_, 145);
		PC_BLL_TunerMFToPc(Buf, Buf_, 145, Buf[2], UFR_RECOK);
	}
	else if (Buf[3] == 2)
	{
		if (PCINFERR_OK == err)
		{

//			FLASH_INTF_WriteStr(g_FlashSaveFlg, TUNER_SQWDBADDR, Buf + 8, 145);
			Sqfre = Buf[8] & 0x0f;
			Func_Str.sqlevel = Buf[8] >> 4;
			StrSnd.u32TxFreq = StrSnd.u32RxFreq = g_u32FreqSectorList[Sqfre];

			gstSql.u16StartNoiseLevel = (Buf[(8 + ((Buf[8] >> 4) * 2))
					+ (Sqfre - 1) * 18] << 8)
					+ Buf[(8 + ((Buf[8] >> 4) * 2) - 1) + (Sqfre - 1) * 18];
			gstSql.u16StopNoiseLevel = gstSql.u16StartNoiseLevel + 1;

			StrSnd = PLL_BLL_GetRfVal( StrSnd.u32TxFreq,
					StrSnd.u32RxFreq);

			RF_BLL_SavePll(StrSnd);
			RF_BLL_RxInit(&StrSnd);

			int i;
			for (i = 0; i < 9; i++)
			{
				g_SqLevelArr[i] = Buf[9 + (Sqfre - 1) * 18 + 2 * i]
						+ Buf[9 + (Sqfre - 1) * 18 + 2 * i + 1] * 256;
			}
			//memcpy (g_SqLevelArr, &Buf[9+(Sqfre-1)*18], 18);
			gstSql.u16StartNoiseLevel = g_SqLevelArr[Func_Str.sqlevel - 1];

			gstSql.u16StopNoiseLevel = gstSql.u16StartNoiseLevel + 1;

		#if (FM_LINK_SEL == FM_LINK_FM_IC)
			#if (FM_IC_SEL == FM_IC_RDA1846S)
				RDA1846S_INTF_WriteReg(RDA_REG_49H, 0xffff);
				Delay_ms(10);

				RDA1846S_INTF_SetRxInit(StrSnd.u32RxFreq);

				HRC_BLL_SetSubMode(SUBAUDIO_RX);

				RDA1846S_INTF_SqThrSet(gstSql.u16StopNoiseLevel,
						gstSql.u16StartNoiseLevel);
			#else
				HRC_BLL_SetSqLevel();
			#endif
		#else
			//	HRC_BLL_CloseAudio();
			//	HRC_BLL_SetSqLevel();
		#endif

			//LCD_BLL_printf_ext(4, 5, lcd_b, "%03d %03d %03d", g_SqLevelArr[0],
			//		g_SqLevelArr[1], g_SqLevelArr[2]);
//			FLASH_INTF_WriteStr(g_FlashSaveFlg,TUNER_SQWDBADDR,Buf+8,145);
			PC_BLL_TunerMFToPc(Buf, (DH_U8*) 0, 0, Buf[2], UFW_REQREAD);
			PC_BLL_TuneWriteStr( TUNER_SQWDBADDR, Buf + 8, 145);
		}
		else
		{
			PC_BLL_TunerMFToPc(Buf, (DH_U8*) 0, 0, Buf[2], UFR_RECERR);
		}
	}
}


void PC_BLL_FlashReadW(DH_U8 * Buf, DH_U8 Len, DH_U8 err)
{
//	DH_U8 *Buf_ = malloc(300);
	 
	if (Buf[3] == 1)
	{
		FLASH_INTF_ReadStr(FLASH_PCWDB_ADDR , Buf+8, 300);
		PC_BLL_TunerMFToPc(Buf, Buf+8, 300, Buf[2], Buf[3]);
	}
	else if (Buf[3] == 2)
	{
		if (PCINFERR_OK == err)
		{
			FLASH_INTF_WriteStr( FLASH_PCWDB_ADDR, Buf + 8, 300);
			PC_BLL_TunerMFToPc(Buf, (DH_U8*) 0, 0, Buf[2], Buf[3]);
			CODESW_INTF_SaveStatus(0xff);//初始信道号
			FLASH_INTF_ReadStr(FLASH_PCWDB_ADDR+PCEA_PC_SET,&Func_Str,sizeof(FUNC_INFO_S));
		}
		else
		{
			PC_BLL_TunerMFToPc(Buf, (DH_U8*) 0, 0, Buf[2], UFR_RECERR);
		}
	}



}


//读令格式:68 21 49 01 xx xx xx xx len ->68 21 49 01 00 11 18 00 FF
//写令格式:68 21 49 02 xx xx xx xx len data ->68 21 49 02 00 11 18 00 FF 11 12 13 14 15 16 17 18 19 20 21 22 23 24
void PC_BLL_PcOptionFlash(DH_U8* RecBuf)
{
	DH_U32 addr = (RecBuf[4] << 24) + (RecBuf[5] << 16) + (RecBuf[6]<< 8) + RecBuf[7];
	DH_U8 len = RecBuf[8];

	if (2 == RecBuf[3])
	{
		PC_BLL_TuneWriteStr(  addr, RecBuf + 9, len);
		PC_BLL_TunerMFToPc(RecBuf, (DH_U8*) 0, 0, C5000REGSET, UFR_BASEINFO);
	}
	else if (1 == RecBuf[3])
	{
		FLASH_INTF_ReadStr(  addr, RecBuf, len);
		PCLINK_INTF_TransmitFrame(RecBuf, len);
	}
}

#endif
