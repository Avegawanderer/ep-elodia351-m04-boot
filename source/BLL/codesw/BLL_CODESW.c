/*************0******************************************************************
 * Copyright (C) 2011-2013 ZheJiang Dahua Technology CO.,LTD.
 *
 * 文件名 : BLL_CODESW.c
 *
 * 作  者 : 123(ggg)<gg>
 * 版  本 : V1.0.0
 * 日  期 : 2015年4月10日 Create
 *
 * 描  述 : 
 *
 *       1. 硬件说明
 *          无。
 *
 *       2. 程序结构说明。
 *          无。
 *
 *       3. 使用说明。
 *          无。
 *
 *       4. 局限性说明。
 *          无。
 *
 *       5. 其他说明。
 *          无。
 *
 * 修订历史: 
 * 日    期: 
 * 修订版本:
 * 作    者:
 * 修订备注:
 *******************************************************************************/



/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */

#include "app_config.h"
 

#include "include.h"
/* ========================================================================== */
/*                           宏和类型定义区                                   */
/* ========================================================================== */

FUNC_INFO_S Func_Str;

static CHN_INFO_S  Chn_Str;

static DH_U8 g_u8ChnAttr;

/* ========================================================================== */
/*                          数据结构定义区                                    */
/* ========================================================================== */


/* ========================================================================== */
/*                          函数声明区                                        */
/* ========================================================================== */
DH_VOID APP_IdleTask_Process(void *p_arg);
DH_VOID CHN_BLL_Get_Subvoice(DH_VOID);

DH_U8 CHN_BLL_GetSubvoiceIndex(DH_U16 u16RxSubVoice);
/***************************************************************************
* 函数名：APP_CodeSwSave_Process
* 描  述：频道中断处理任务
* 输  入：u32Data:系统定义的参数
* 输  出：无
* 返回值：无
***************************************************************************/
DH_VOID APP_IdleTask_Process(void *p_arg)
{
	DH_U32 lu32Signal;

	Delay_ms(1000);
	while(1)
	{

		
	}
}


/***************************************************************************
* 函数名：CODESW_BLL_Process
* 描  述：频道中断处理任务
* 输  入：u32Data:系统定义的参数
* 输  出：无
* 返回值：无
***************************************************************************/
DH_VOID CODESW_BLL_Process(void *p_arg)
{ 
     DH_U8 data;
     DH_U32 u32err;
     DH_U32 lu32signal;

	if(GloVarFlag.KeyFlag||GloVarFlag.VoxFlg||GloVarFlag.OneKeyCall)//正在发射的过程中不允许响应.
	{		
	}		
	else if(ScanStatGet()==SCAN_ON )//
	{//在扫描时不允许切换.
	}
	else if(GloVarFlag.KeyFlag)//正在PTT按键发射的时候旋转无效.
	{
	}	
	else
	{
	//	Delay_ms(10);//滤波
		data = CODESW_INTF_GetRealStat();

		/* 判断是否由为相同信道 */
		if(data != CODESW_INTF_GetStatus() )
		{
			CODESW_INTF_SaveStatus(data);

			INTF_VoxChnChangeStop();

			MenuVarFlg.g_ChecksignalFlg = 1;

			if(GloVarFlag.FmRecStat==1)
			{
				GloVarFlag.FmMontFlg = 0; ////NEW
//				HRC_BLL_CloseAudio();
//				TMR_APP_TmrHndStop(TMRHND_CALLREF);
//				TIMER_DRV_Disable(TIM4);
//				TIMER_DRV_Disable(TIM5);
//				GPIO_DRV_IntDisable(RDA_CDCSS_PIN);
//				FM_INTF_ParamInit();
			}

			/* 防止频繁刷新导致信道数据获取错误 */

			if(1/*CODESW_INTF_ChnGetStat()*/)
			{
				CHN_BLL_Set( data+1);				
				
				if(CHN_BLL_GetAttr()==CHNSTAT_NONE)
				{
					SOUND_BLL_SoundStart(SOUNDSTAT_Alarm);
				}
				else
				{
				#if FUNC_MENU_DISP!=MENU_DISP_LCD
					#if USE_CHN_CODESW==CHN_CODESW_16MAX
						if(MenuVarFlg.g_bZoneChnChgFlg == 2)
						{
							SOUND_BLL_SoundExe(SOUNDSTAT_Voice, SOUNDSTAT_ZoneVoice);
						}
						else
						{
							SOUND_BLL_SoundExe(SOUNDSTAT_Voice, SOUNDSTAT_ChanleNumberVoice);
						}
					#else
//						printk("MenuVarFlg.g_bZoneChnChgFlg %d\r\n",MenuVarFlg.g_bZoneChnChgFlg);
						if(MenuVarFlg.g_bZoneChnChgFlg == 2)
						{
							SOUND_BLL_SoundStartOpt(SOUNDSTAT_Voice, SOUNDSTAT_ZoneVoice);
						}
						else
						{
							SOUND_BLL_SoundStartOpt(SOUNDSTAT_Voice, SOUNDSTAT_ChanleNumberVoice);
						}
					#endif
				#endif
				}
				MenuVarFlg.g_bZoneChnChgFlg = 0;
				//TMR_APP_TmrHndStart(TMRHND_CODESWSAVE);
			}

			RED_LED_OFF;

			GREEN_LED_OFF;
			
		}
	}
}



DH_VOID CHN_BLL_InfoSet(CHN_INFO_S *pstr,DH_U8 *buf)
{
#if 0
	if ((buf[PCEA_AREAOTHER]&PCEA_ATTR_BITS) == PCEA_ATTR_D)
	{
		pstr->u8ChnAttr = CHNSTAT_DMR;
		pstr->u8DFMStat = CHNSTAT_NONE;
	}
	else if ((buf[PCEA_AREAOTHER]&PCEA_ATTR_BITS) == PCEA_ATTR_M)
	{
		pstr->u8ChnAttr = CHNSTAT_FM;
		pstr->u8DFMStat = CHNSTAT_NONE;
	}
	 
//	memcpy ((DH_CHAR*)(pstr->u8ChnName), (DH_CHAR*)&buf[PCEA_CHANNELNAME],PCEA_CHN_NAME_LEN);
//	pstr->u8ChnName[PCEA_CHN_NAME_LEN+1] = '\0';

	pstr->stDChn.u8ScanRaomList      = (buf[PCEA_AREASCAN]&0x3f);


	pstr->stTxDChn.u8Power           = (buf[PCEA_AREAOTHER]&MENUSETPOWER_BITS) ? MENUSETPOWER_M:MENUSETPOWER_L;
	pstr->stTxAChn.u8Power           = pstr->stTxDChn.u8Power;


	/* 0表示12.5K，1表示25K */
	pstr->stAChn.u8ChnSpace         = (buf[PCEA_AREAOTHER]&PCEA_BW_BITS)? BW_25K:BW_12_5K;

	/* 1表示载波，2表示亚音，3表示亚音数码，4表示反向亚音数码 */
	pstr->stRxAChn.u8SubVoiceType   = (buf[PCEA_SUBVOICETYPE]&PCEA_REC_SUBVOICETYPE_BITS)+1;
	pstr->stRxAChn.u16SubVoice      = buf[PCEA_REC_SUBVOICE];

#if FUNC_FM_SUB_NO_FSREC
	pstr->stTxAChn.u8SubVoiceType   = pstr->stRxAChn.u8SubVoiceType;
	pstr->stTxAChn.u16SubVoice      =  pstr->stRxAChn.u16SubVoice;
#else
	/* 1表示载波，2表示亚音，3表示亚音数码，4表示反向亚音数码 */
	pstr->stTxAChn.u8SubVoiceType   = ((buf[PCEA_SUBVOICETYPE]&PCEA_TRAN_SUBVOICETYPE_BITS)>>2)+1;
	pstr->stTxAChn.u16SubVoice      =  buf[PCEA_TRAN_SUBVOICE];
#endif
#if 1
	/* 0表示无，1表示两音，2表示五音 */
	pstr->stAChn.u8SignalType       = 0;//;
	pstr->stAChn.u8Characterlist    = 0;//buf[PCEA_AREACCSLOT]&0x3f;
	pstr->stAChn.u8ScanList         = (buf[PCEA_AREASCAN]&0x3f);
	pstr->stAChn.u8ScanAutoFlg      = 0;//(buf[PCEA_AREASCAN]&PCEA_AUTOSCN_BITS)>>6;

	pstr->stRxAChn.u8SignalSysterm  = 0;//buf[PCEA_ENCRYPT]&0x03;
	/* 0表示载波，1表示亚音，2表示可选信令，3表示亚音与可选信令，4表示亚音或可选信令 */
	pstr->stRxAChn.u8SQLMode        = 0;
	pstr->stRxAChn.u8SQLLevel       = StrMenuSet.SqLevel;//(buf[PCEA_ENCRYPT]&0xc0)>>6;//printk("u8SQLLevel %d",pstr->stRxAChn.u8SQLLevel);
	pstr->stRxAChn.u8AlarmAlert     = buf[PCEA_AREAOTHER]&PCEA_EMERGNCY_ALARM_BITS;
	pstr->stRxAChn.u8AlarmReply     = 0;//(buf[PCEA_ALARM]&0x02)>>1;
	pstr->stRxAChn.u8EmergncyAlert  = buf[PCEA_AREAOTHER]&PCEA_EMERGNCY_CALL_BITS;
	pstr->stRxAChn.u8MatchSubvoice  = 0;

	/* 0表示无，1表示两音，2表示五音 */
	pstr->stTxAChn.u8SignalSysterm  = 0;//(buf[PCEA_AREARLM]&0x18)>>3;
	pstr->stTxAChn.u8AlarmIndex     = buf[PCEA_EXI];         /* */
	/* 0表示允许发送，1表示信道空闲，2表示亚音匹配，3表示亚音不匹配 */
	pstr->stTxAChn.u8TransTactics   = 1+ ((buf[PCEA_AREACCSLOT]&PCEA_POLITE_BITS)>>5);
	pstr->stTxAChn.u16Tot           = 0;
	pstr->stTxAChn.u8OvertimeTrans  = 0;//buf[PCEA_FOBIDCALL];
	pstr->stTxAChn.u8FMDeviation    = 0;
	pstr->stTxAChn.u8SubFMDeviation = 0;
#endif

	memcpy ((void*)&pstr->stRxAChn.u32RxFreq, (void*)&buf[PCEA_RECFREQ], sizeof(DH_U32));
	memcpy ((void*)&pstr->stTxAChn.u32TxFreq, (void*)&buf[PCEA_SENDFREQ], sizeof(DH_U32));

	memcpy ((void*)&pstr->stRxDChn.u32RxFreq, (void*)&buf[PCEA_RECFREQ], sizeof(DH_U32));
	memcpy ((void*)&pstr->stTxDChn.u32TxFreq, (void*)&buf[PCEA_SENDFREQ], sizeof(DH_U32));



    CHN_BLL_SetCurZoneChn(pstr->u8Zone,pstr->u8Chn);
	CHN_BLL_SetAttr(pstr->u8ChnAttr);
	CHN_BLL_SetDFMAttr(pstr->u8DFMStat);
	CHN_BLL_SetChnName(pstr->u8ChnName);

	CHN_BLL_SetDMRChnInfo(pstr->stDChn);
	CHN_BLL_SetDMRRxChnInfo(pstr->stRxDChn);
	CHN_BLL_SetDMRTxChnInfo(pstr->stTxDChn);

	/* 设置信道联系人 */
	CHN_BLL_SetLinkMan(pstr->stTxDChn.u8LinkmanIndex);
	/* 设置组呼列表 */
	CHN_BLL_SetGroupNumList(pstr->stRxDChn.u8GroupList);
	/* 设置加密信息 */
//		CHN_BLL_SetPassWord(pstr->stRxDChn.u8PassWord);

	CHN_BLL_SetFMChnInfo(pstr->stAChn);
	CHN_BLL_SetFMRxChnInfo(pstr->stRxAChn);
	CHN_BLL_SetFMTxChnInfo(pstr->stTxAChn);
	#endif
}

/***************************************************************************
* 函数名：CHN_BLL_Set
* 描  述：设置区域信道信息
* 输  入：u16Area-区域，u8Chn-信道
* 输  出：无
* 返回值：无
***************************************************************************/
DH_VOID CHN_BLL_Set( DH_U8 u8Chn)
{
    DH_U8 buf[PCEA_CHN_LEN];
    CHN_INFO_S str = {0};
    PLL_INFO_S p_str;

	
	FLASH_INTF_ReadStr( FLASH_PCWDB_ADDR+PCEA_CHN_LEN*(u8Chn-1), (DH_U8*) &str, sizeof(CHN_INFO_S));

	//str.u32TxFreq = str.u32RxFreq=156125000;
	//Chn_Str.u16TxSubVoice = 670;

	Chn_Str = str;

	Chn_Str.u8RxSubVoiceIndex =	CHN_BLL_GetSubvoiceIndex(Chn_Str.u16RxSubVoice);
	Chn_Str.u8TxSubVoiceIndex =	CHN_BLL_GetSubvoiceIndex(Chn_Str.u16TxSubVoice);
//	CHN_BLL_InfoSet(&str,buf);

	if(    str.u32RxFreq > FREQ_UHF_H || str.u32RxFreq < FREQ_UHF_L
		|| str.u32RxFreq > FREQ_UHF_H || str.u32TxFreq < FREQ_UHF_L)
	{
		g_u8ChnAttr = CHNSTAT_NONE;
	//	CHN_BLL_SetAttr(str.u8ChnAttr);
		HRC_BLL_CloseAudio();
		return;
		
	}
	g_u8ChnAttr = CHNSTAT_FM;

	p_str = PLL_BLL_GetRfVal(str.u32TxFreq,str.u32RxFreq);

	CODESW_INTF_UpdateSqLevel(); //更新信噪等级数组

	RF_BLL_RxInit(&p_str);
}

/***************************************************************************
* 函数名：CHN_BLL_Get
* 描  述：信道信息获取
* 输  入：u16Area-区域，u8Chn-信道，str-信道结构体
* 输  出：信道结构体
* 返回值：无
***************************************************************************/
DH_VOID CHN_BLL_Get(DH_U16 u16Area, DH_U8 u8Chn, CHN_INFO_S *pstr)
{
    DH_U8 buf[PCEA_CHN_LEN];
#if 0
    if((u16Area < 1||u16Area > AREA_MAX)||(u8Chn < 1||u8Chn > CHN_MAX))
    {
        pstr->u8ChnAttr= CHNSTAT_NONE;
        
        return ;
    }

    FLASH_INTF_ReadStr( PCBA_AREASET+PCEA_ZONE_LEN*(u16Area-1)+\
    		PCEA_CHNBASEADDR+PCEA_CHN_LEN*(u8Chn-1), buf, PCEA_CHN_LEN);

    pstr->u8Chn = u8Chn-1;
    pstr->u8Zone = u16Area-1;
    CHN_BLL_InfoSet(pstr,buf);

    CODESW_INTF_UpdateSqLevel();
#endif
}

/***************************************************************************
* 函数名：CHN_BLL_SetAttr
* 描  述：设置当前信道属性
* 输  入：stat:属性值
* 输  出：无
* 返回值：无
***************************************************************************/
DH_VOID CHN_BLL_SetAttr(DH_U8 stat)
{
    g_u8ChnAttr = stat;
}

/***************************************************************************
* 函数名：CHN_BLL_SetDFMAttr
* 描  述：设置当前信道属性
* 输  入：stat:属性值
* 输  出：无
* 返回值：无
***************************************************************************/
DH_VOID CHN_BLL_SetDFMAttr(DH_U8 stat)
{
//    Chn_Str.u8DFMStat = stat;
}

/***************************************************************************
* 函数名：CHN_BLL_SetDFMRecStat
* 描  述：设置当前信道属性
* 输  入：stat:属性值
* 输  出：无
* 返回值：无
***************************************************************************/
DH_VOID CHN_BLL_SetDFMRecStat(DH_U8 stat,DH_U8 recstat)
{
#if FUNC_DMR_N_FM
	if(CHN_BLL_GetDFMAttr())
	{
		if( CHN_BLL_GetAttr() != stat)
		{
			if (CALL_APP_CallInStatGet())
			{
				g_FpgaRx = 0;
				CallStr.UsedFlg = CALLSTR_UNUSED;
				TMR_APP_TmrHndStop(TMRHND_CALLREF);
				CALLDISP_APP_SetCallDisp1(CALLSTR_CALLOUT, CD1_CALLEND);
				CALLDISP_APP_CallStopTimeOut(BSP_ALARM_FREQUENCY, CALLSTR_CALLOUT);
				MenuVarFlg.TopMenuNoRefleshFlg = 0;
			}
			HRC_BLL_CloseAudio();
			CHN_BLL_SetAttr(stat);
			CODESW_INTF_UpdateSqLevel(); //更新信噪等级数组
			RF_BLL_RxInit(RF_BLL_GetPllStrP());
			MenuVarFlg.g_bDFMChgFlg = 1;

//			SCAN_APP_ScanGapSet();

			CHN_BLL_SetModeInit();
//			dhsem_post(&ChnDisp_Sem);
//			Delay_ms(10);
		}
		OpenSqFlg = 0;
		g_DFMRecStat = recstat;
	}
#endif
}
/***************************************************************************
* 函数名：CHN_BLL_SetChnName
* 描  述：设置信道名
* 输  入：*s: 字符串
* 输  出：无
* 返回值：无
***************************************************************************/
DH_VOID CHN_BLL_SetChnName(DH_U8* s)
{
//    if(s == NULL)
//    {
//      return ;
//    }
//    
//    memcpy((void*)Chn_Str.u8ChnName,(void*)s,sizeof(Chn_Str.u8ChnName));
}


DH_VOID CHN_BLL_SetCurZoneChn(DH_U8 zone,DH_U8 chn)
{
//	Chn_Str.u8Chn = chn;
//	Chn_Str.u8Zone = zone;
}
DH_U8 CHN_BLL_GetCurZoneNum(DH_VOID)
{
	return  0;//Chn_Str.u8Zone;
}

DH_U8 CHN_BLL_GetCurChnNum(DH_VOID)
{
	return 0;//Chn_Str.u8Chn;
}

uint8 CHN_BLL_PowerGet(void)
{
	return Chn_Str.power;
}

void CHN_BLL_PowerSet(uint8 power)
{
printk("%s %d\r\n",__FUNCTION__,__LINE__);
	Chn_Str.power = power;
	FLASH_INTF_WriteStr( FLASH_PCWDB_ADDR+PCEA_CHN_LEN*CODESW_INTF_GetStatus(), (DH_U8*) &Chn_Str, sizeof(CHN_INFO_S));
}





DH_U8 CHN_BLL_GetBusyFlag(DH_VOID)
{
	return Chn_Str.busyflag;//Chn_Str.u8Chn;
}
/***************************************************************************
* 函数名：CHN_BLL_GetAttr
* 描  述： 获取当前信道属性
* 输  入：无
* 输  出：无
* 返回值：无
***************************************************************************/
DH_U8 CHN_BLL_GetAttr(DH_VOID)
{
    return  g_u8ChnAttr;
}

/***************************************************************************
* 函数名：CHN_BLL_GetChnName
* 描  述： 获取信道名
* 输  入：无
* 输  出：无
* 返回值：信道名字符串
***************************************************************************/
DH_U8* CHN_BLL_GetChnName(DH_VOID)
{
//	Chn_Str.u8ChnName[10]='\0';
 //   return Chn_Str.u8ChnName;
}

/***************************************************************************
* 函数名：CHN_BLL_GetChnInfoP
* 描  述： 获取数字信道信息
* 输  入：无
* 输  出：无
* 返回值：数字信道结构体
***************************************************************************/
CHN_INFO_S* CHN_BLL_GetChnInfoP(DH_VOID)
{
    return &Chn_Str;
}

DH_U8 CHN_BLL_GetFMBandWide(DH_VOID)
{
	if(CHN_BLL_GetAttr()==CHNSTAT_FM)
	{
		return Chn_Str.band;
	}
	else
	{
		return BW_12_5K;
	}
}
uint16 CHN_BLL_GetTxSubvoice(DH_VOID)
{
	return Chn_Str.u16TxSubVoice & 0xFFF;

}
uint16 CHN_BLL_GetRxSubvoice(DH_VOID)
{
	return Chn_Str.u16RxSubVoice & 0xFFF;

}
uint8 CHN_BLL_GetTxSubvoiceIndex(DH_VOID)
{
	return Chn_Str.u8TxSubVoiceIndex;

}
uint8 CHN_BLL_GetRxSubvoiceIndex(DH_VOID)
{
	return Chn_Str.u8RxSubVoiceIndex;

}

DH_U8 CHN_BLL_GetSubvoiceIndex(DH_U16 u16RxSubVoice)
{
	DH_U8 u8RxSubvoiceIndex;

	if(!u16RxSubVoice) u8RxSubvoiceIndex = 0;
	else if(u16RxSubVoice < 10000)
	{
		for(u8RxSubvoiceIndex = 0 ;u8RxSubvoiceIndex<51;u8RxSubvoiceIndex++)
		{
			if(CtcssTones[u8RxSubvoiceIndex] == u16RxSubVoice) break;
		}
	}
	else 
	{
		uint16 u16RxSubVoice = u16RxSubVoice &0x0FFF;
		if(u16RxSubVoice < 40000)
		{

		}
		else 
		{


		}
	}
	return u8RxSubvoiceIndex;
}

DH_U8 CHN_BLL_GetFMRxSubVoiceType(DH_VOID)
{
	if(!Chn_Str.u16RxSubVoice) return FM_SUBVOICETYPE_CARRIER;
	else if(Chn_Str.u16RxSubVoice < 10000) return FM_SUBVOICETYPE_CTCSS;
	else if(Chn_Str.u16RxSubVoice < 40000) return FM_SUBVOICETYPE_CDCSS;
	else  return FM_SUBVOICETYPE_CDCSSR;
}

DH_U8 CHN_BLL_GetFMTxSubVoiceType(DH_VOID)
{
	if(!Chn_Str.u16TxSubVoice) return FM_SUBVOICETYPE_CARRIER;
	else if(Chn_Str.u16TxSubVoice < 10000) return FM_SUBVOICETYPE_CTCSS;
	else if(Chn_Str.u16TxSubVoice < 40000) return FM_SUBVOICETYPE_CDCSS;
	else  return FM_SUBVOICETYPE_CDCSSR;
}

DH_VOID CHN_BLL_SetFM_Subvoice(DH_U8 rxtype,DH_U8 txtype,DH_U8 rxSub,DH_U8 txSub)
{
#if 0
	DH_U8 buf[3];
    FLASH_INTF_ReadStr( PCBA_AREASET+PCEA_ZONE_LEN*(MENU_APP_GetMenuSetStat(BARTYPESETFLG_ZONE))+PCEA_CHNBASEADDR+PCEA_SUBVOICETYPE+PCEA_CHN_LEN*(CODESW_INTF_GetStatus()),buf,3);
   	if(rxtype <= FM_SUBVOICETYPE_CDCSSR){
   		buf[0] &= ~PCEA_REC_SUBVOICETYPE_BITS;//0xFC;
		buf[0] |= (rxtype-1);
		Chn_Str.stRxAChn.u8SubVoiceType = rxtype;
   	}
   	if(txtype <= FM_SUBVOICETYPE_CDCSSR){
		buf[0] &= ~PCEA_TRAN_SUBVOICETYPE_BITS;//0xF3;
		buf[0] |= ((txtype-1)<<2);
		Chn_Str.stTxAChn.u8SubVoiceType = txtype;
	}
   	if(rxSub != 0xff && rxtype!=0xff){
   		Chn_Str.stRxAChn.u16SubVoice = rxSub;
   		buf[1] = rxSub;
   	}
   	if(txSub != 0xff && txtype!=0xff){
		Chn_Str.stTxAChn.u16SubVoice = txSub;
		buf[2] = txSub;
	}
    FLASH_INTF_WriteStr( (PCBA_AREASET+PCEA_ZONE_LEN*(MENU_APP_GetMenuSetStat(BARTYPESETFLG_ZONE))+PCEA_CHNBASEADDR+PCEA_SUBVOICETYPE+PCEA_CHN_LEN*(CODESW_INTF_GetStatus())), buf,3);
#endif
}

DH_BOOL CHN_BLL_KeyMsgFilter(DH_U8 Msg)
{
#if 0
#if USE_CHN_CODESW==CHN_CODESW_360D
	if((Msg == Key_CoderDown) ||(Msg == Key_CoderUp))
	{
		if(CHN_BLL_GetAttr() != CHNSTAT_NONE && \
		  (MSET_APP_LockKeypadStat()==LOCK_NO || MSET_APP_LockKeypadStat()==LOCK_OPENED))
		{
			return TRUE;
		}
	}
#endif
	if(MSET_APP_LockKeypadStat()==LOCK_NO || MSET_APP_LockKeypadStat()==LOCK_OPENED)
	{
		if(CHN_BLL_GetAttr() != CHNSTAT_NONE )
		{
			return TRUE;
		}
		else if( Msg==Key_Ok||Msg==Key_Up||Msg==Key_Down||Msg==Key_LongHnd||Msg==Key_Back)
		{
			return TRUE;
		}
	}
#if USE_KEYPAD_ALL
	else if ((MSET_APP_LockKeypadStat()==LOCK_YES  && Msg==Key_Ok))// menu  ok + *
	{
		return TRUE;//
	}
#endif
	else if  (MSET_APP_LockKeypadStat()==LOCK_OPEN && Msg==Key_A)
	{
		return TRUE;
	}

	else if((MSET_APP_LockKeypadStat()==LOCK_YES || MSET_APP_LockKeypadStat()==LOCK_OPEN))
	{
		if(g_KeyFuncIndex != MenuTopIndex)
		{
			return TRUE;
		}
		if(/*(Msg == Key_P3) ||*/(Msg == Key_P1) || (Msg == Key_P2)||Msg == Key_Call||Msg == Key_CallUp )
		{
			if((Msg == Key_P3) ||(Msg == Key_P1) || (Msg == Key_P2)){
				MenuVarFlg.KeySideFuncFlg = 1;
			}
			return TRUE;
		}
		else if(Msg == Key_LongHnd &&\
			(   Str_Key.pFunKeyLongHnd == Str_KeyT.P1LHnd ||\
				Str_Key.pFunKeyLongHnd == Str_KeyT.P2LHnd))
		{
			MenuVarFlg.KeySideFuncFlg = 1;
			return TRUE;
		}
	#if !USE_KEYPAD_ALL
		else if(Msg == Key_LongHnd && Str_Key.pFunKeyLongHnd == Str_KeyT.pKeyLHnd[Key_Ok])
		{
			MenuVarFlg.KeySideFuncFlg = 1;
			return TRUE;
		}
	#endif
		else if(Str_Csbk.CallClewVoice == 1  && (Msg==Key_Ok||Msg==Key_Back))
		{
			CSBK_APP_CallClewClose();
		}
		else
		{
			MSET_APP_LockKeypadHnd(LOCK_WARN);
		}
	}
#endif
	return FALSE;
}
