#include "app_config.h"
#include "include.h"


//STR_CODESW g_scanpointchnpll;	          /* 扫描指定信道的状态值 */
//uint8 scanbuf[16];					                 /* 扫描列表缓存 */
//uint8 g_band25;					                    /* 25K带宽 */
uint8 Scanflag=0;/*扫到信号的标志位，停留时间的标志位*/
uint8 g_remainflag= 0;			               /* 停留时间标志位 */
uint8 g_presentchn;				                 /* 当前频道 ，0是区域1是信道号*/
//uint8 g_soundflag = 0;
//uint16 ScanListValue=0;//扫描信道的个数.
//uint8 PreScanListValue=0;//备用的扫描信道的个数.
STR_SCAN ScanList;				                  /* 扫描列表结构体 */
//STR_EDIT_CH EditCHanal;



uint8 ListChnSw=SCAN_OFF;	/* 开启的话,on */
static uint8 ListChnSwLast=SCAN_OFF;/* 此值不管是否开启扫描模式，均为off */
static uint8 ListChnStat;

void Scan_SaveFreq(void);
void Scan(void);

void ScanChangeFreq(void);
/*****************************************************************************
 *   函数名: ScanCtrlSet
 *   功能描述: 扫描控制设置
 *   参数定义:	
 *	 返回值:
 *   创建人：
******************************************************************************/
void ScanCtrlSet(uint8 stat)
{
	if(SCAN_ON==stat)
	{	
//		g_Str_CodeSwbackup=GetPllStat();//备份PLL数据
//		if(g_fmonitorflag)
//		{//处于永久监听状态下.
//			INTF_FmMontStop();
//			monitorstop_Flag=1;
//		} 
		TmrHndStart(TMRHND_SCAN);
	}
	else if(SCAN_OFF==stat)
	{
		if(ScanList.ScanMode==SCAN_MODE_FreqChange)
		{
			Scan_SaveFreq();
		}
//		if(monitorstop_Flag)
//		{//处于永久监听状态下.
//			GloVarFlag.FmMontFlg=0;
//			if(g_fmonitorflag==1)
//			{
//				SetRx_OnArrowBand();
//				INTF_FmMontStart();
//			}
//			else
//			{
//				g_fmonitorflag=0;
//				INTF_FmMontStop();
//			}
//			monitorstop_Flag=0;
//		}  
		TmrHndStop(TMRHND_SCAN);
	}

	ListChnSw = stat;
}

/*****************************************************************************
 *   函数名: ScanCtrlSet
 *   功能描述: 扫描控制设置
 *   参数定义:	
 *	 返回值:
 *   创建人：
******************************************************************************/
uint8 ScanCtrlGet(void)
{
  return ListChnSw;
}

/*****************************************************************************
 *   函数名: ScanLastCtrlSet
 *   功能描述: 扫描控制设置
 *   参数定义:	
 *	 返回值:
 *   创建人：
******************************************************************************/
void ScanLastCtrlSet(uint8 stat)
{
  
 ListChnSwLast = stat;
}

/*****************************************************************************
 *   函数名: ScanLastCtrlGet
 *   功能描述: 扫描控制设置
 *   参数定义:	
 *	 返回值:
 *   创建人：
******************************************************************************/
uint8 ScanLastCtrlGet(void)
{
  return ListChnSwLast;
}

/*****************************************************************************
 *   函数名: ScanStatSet
 *   功能描述: 扫描控制设置
 *   参数定义:	
 *	 返回值:
 *   创建人：
******************************************************************************/
void ScanStatSet(uint8 stat)
{
   ListChnStat = stat;
}

/*****************************************************************************
 *   函数名: ScanStatGet
 *   功能描述: 扫描控制设置
 *   参数定义:	
 *	 返回值:
 *   创建人：
******************************************************************************/
uint8 ScanStatGet(void)
{
  return ListChnStat;
}

void Scan_SaveFreq(void)
{
//	uint32 Freq = GetPllChnRxFreq(StrMenuSet.MainBand);
//	where_flag=0;	
//	ScanFreqMode_SaveFreq(Freq);//保存频率值和标志位
//	where_flag=1;	
//	ScanFreqMode_SaveFreq(Freq);//保存频率值和标志位

	
}
void Scan_Process(void)
{
//	if(g_SysTaskFlag & systa)
	{
		if(g_Layer==1&&ScanCtrlGet()==SCAN_ON)	
		{
			Scan();
		}
	}
}

void ScanStop(void)
{
	if(ScanCtrlGet()==SCAN_ON)
	{
		ScanCtrlSet(SCAN_OFF);
		if(ScanList.ScanMode==SCAN_MODE_ChannelList)
		{//只有在列表扫描的方式下才要切换回去.	
			CHN_BLL_Set(CODESW_INTF_GetRealStat());						
		}
	}
}



void ScanStart(void)
{
	if(1)//!FreqallStat_Check()列表扫描//开始扫描
	{
		if(Scan_APP_GetChnNum())
		{
			ScanList.ScanMode=SCAN_MODE_ChannelList;
			//SetRx_OnArrowBand();
			
			Scanflag=1;//为了多停留1S
			CHN_BLL_Set(Scan_APP_GetChnIndex(0));//切换到扫描列表第一个的信道上去
			ScanCtrlSet(SCAN_ON);
		}
		else
		{
		//	Display_SetFail_Loop((uint8*)*(StrListEmpty[StrMenuSet.language].Text+0));
		}
		Jump2Menu(MenuTopIndex,FlashMode_AutoInit);
	}
//	else// if(g_Item==1)//扫频模式
//	{
//		ScanList.ScanMode=SCAN_MODE_FreqChange;
////		Display_SetOK_Loop((uint8*)*(StrMenuM_5_6_10_3_3[StrMenuSet.language].Text+0));
//		SetRx_OnArrowBand();
//		
////		uint32 Freq;
//
//		//PLLStat= GetPllStat();
//		//Freq=(uint32)PLLStat.chninfo[StrMenuSet.MainBand].PllValueRec;
////		Freq = GetPllChnInfo_p( StrMenuSet.MainBand)->PllValueRec;
//						
////		ScanFreqMode_SaveFreq(Freq);//保存频率值和标志位
//		ScanCtrlSet(SCAN_ON);
//
//		Jump2Menu(MenuTopIndex,FlashMode_AutoInit);
//	}
}



/******************************************************************************
*函数名称:扫描接收切换

*函数说明:会保存扫描下对应的信道信息

*参数说明:

*创建人:yufeng

*创建日期:2012/04/11
******************************************************************************/
void ListChnSet(uint8 CCnt)
{
#if 1

#else
	STR_CHNINFO PLLStat = GetPllChnInfo(StrMenuSet.MainBand);
	{
		DRV_RedLedOn();
	}

	INTF_ChnLock();
	/* 切换前做相应处理 */
	/*旋转频道时如果当前在发送则停止发送*/

	if(GloVarFlag.FmRecStat==1)
	{
		if(GloVarFlag.FmMontFlg==1)
		{
			//KeyPermanentMonitor();
		}
		INTF_FmVoiceStop();
		GloVarFlag.FmRecStat=0;
	}

	DH_Drv_ExtiIntSet(EXTI_MODE_CTSDCS,EXTI_DIS);
	CloseCtcssTimeInt();
	CloseCdcssTimeInt();	
	DH_Drv_ExtiIntSet(EXTI_MODE_FM, EXTI_DIS);

	CHN_APP_GetChnInfo(&PLLStat,  CCnt);
	
	AllFlag.NeedConfigRdaVolFlag=1;
	SavePllChnInfo(StrMenuSet.MainBand,&PLLStat);
	if(PLLSTAT_M==PLLStat.Attr)
	{
		if(ScanList.ctcdmode == 0)//不检测亚音
		{
			DH_Drv_ExtiIntSet(EXTI_MODE_CTSDCS,EXTI_DIS);
				CloseCtcssTimeInt();
				CloseCdcssTimeInt();
	
		}
		else if(ScanList.ctcdmode == 1)//非优先信道检测亚音
		{
			if(ScanList.priFlag == g_presentchn)
			{
				DH_Drv_ExtiIntSet(EXTI_MODE_CTSDCS,EXTI_DIS);
				CloseCtcssTimeInt();
				CloseCdcssTimeInt();
			}
		}
		else if(ScanList.ctcdmode == 2)//优先信道检测亚音
		{
			if(ScanList.priFlag != g_presentchn)
			{
				DH_Drv_ExtiIntSet(EXTI_MODE_CTSDCS,EXTI_DIS);
				CloseCtcssTimeInt();
				CloseCdcssTimeInt();
			}
		}
	
	}
	
	if((Check_SQ_Zero()==TRUE&&(PLLStat.R_S_SubType.b_type_r==E_CTD_SQ)))
	{
		/*防止有人在0级的情况下开启扫描*/
		MenuSetSetted(BARTYPESETFLG_Squelch,4);
	}
//	if(StrMenuSet.Scan_Led && StrMenuSet.All_Led)
//	{
		DRV_RedLedOff();
//	}
	
	if(AllFlag.KeyNumberOnFlag==0&&g_Layer==1)
	{
		LcdClearMainBand();

		PLLDispModeX(StrMenuSet.MainBand);	
	}  
	SetRx_OnArrowBand();	
//	SubAudioDispInit();//亚音频显示//速度比秒扫描的显示快。
	INTF_ChnUnlock();
#endif
}



/******************************************************************************
*函数名称:频道扫描功能

*函数说明:

*参数说明:

*创建人:yufeng

*创建日期:2012/04/11
******************************************************************************/
void ChnListScan(void)
{
	static uint8 flg;
	
    if(ScanCtrlGet()==SCAN_ON)
    {
		MenuVarFlg.g_ChecksignalFlg = 1;//在扫描的时候不要进入省电模式.
    	if(g_remainflag != 0)
    	{
            g_remainflag --;
			if(g_remainflag ==0)//不需要再次切换了
			{
				goto SCAN_LIST_CHANGE_LOOP;
				//ListChnSet(ALLchannel-1);//扫描开始前的信道
			}
            return;
        }
SCAN_LIST_CHANGE_LOOP://切换扫描信道
        if(ScanList.scanIndex<CHN_APP_GetChnNum())
       	{
        //	if(ScanList.priFlag == 0xff)        /* 无优先信道 */
        	if(!ScanList.priFlag || (flg==1))        /* 无优先信道 */
        	{
				flg = 0;
				g_presentchn=ScanList.scanIndex;
				ListChnSet(Scan_APP_GetChnIndex(g_presentchn)   );//ScanListTab[g_presentchn]
				ScanLastCtrlSet(SCAN_ON);
				ScanList.scanIndex = getNextIndex(1, ScanList.scanIndex,0 , Scan_APP_GetChnNum()-1);
        	}
        	else                               /* 有优先信道 */
        	{
				flg = 1;				
				g_presentchn = ScanList.priFlag;//ScanList.priFlag;//处理优先信道
			//	lcd_infostr.PRI = 1;
				ListChnSet(0);//第一信道即優先信道	
			//	lcd_infostr.PRI = 0;
            }
    	}
    	else
    	{
			ScanList.scanIndex=0;
			g_presentchn = ScanList.scanIndex;			 
			ListChnSet(CODESW_INTF_GetRealStat());
			ScanLastCtrlSet(SCAN_OFF);
        }
    }
}

/*****************************************************************************
 *   函数名: ScanDispTopExit
 *   功能描述: 扫描执行时退出显示信道信息(只针对模拟信道)
 *   参数定义: 
 *	返回值:
 *   创建人：yufeng,2012/11/28
******************************************************************************/
void ScanDispTopExit(void)
{
	//ScanList.scanIndex=0;
//	g_presentchn = ScanList.scanIndex;
//	g_remainflag =0;
//
//	
//	PLLStat_Numb =(ScanListTab[ScanList.scanIndex]+1);
//	if(TopSet_Str.ScanMode==SCAN_MODE_ChannelList)
//	{
//	  	ListChnSet(ScanListTab[g_presentchn]);
//	}
	
	ScanLastCtrlSet(SCAN_OFF);	
}


void ScanFreqMode(void)
{
#if 1
	if(g_remainflag != 0)
	{
		g_remainflag --;
		if(g_remainflag ==0)
		{
			ScanChangeFreq();
		}
		return;
	}
	ScanChangeFreq();
#endif		
}


/*****************************************************************************
 *	函数名: Scan
 *	功能描述:扫描
 *	参数定义:
 *	返回值:
 *	创建人：陶林，2011/03/31
******************************************************************************/
void Scan(void)
{
    if((ScanCtrlGet()==SCAN_ON)&&(g_Layer == 1)//&&(AllFlag.KeyNumberOnFlag==0)
			&&!GloVarFlag.PcWriteReadFlg && !RxBusyCheck())
	{
	 	static uint8 timecnt;

	///	STR_CODESW PLLStat = GetPllStat();
		
		ScanList.remaintime=0;


		if(ScanCtrlGet()==SCAN_ON&&ScanList.ScanMode==SCAN_MODE_ChannelList)//&&g_KeyFuncIndex == MenuTopIndex
		{	//？？？？？？？？？？？？
//			if(Scanflag)
//			{//正在扫描呼叫完毕之后回到最初的信道界面.
//				if(Scanflag==1)
//			 	{
//			 		PLLStat_Numb=(ScanListTab[0]+1);//跳转到第一个扫描信道上去.
//			 		ListChnSet(ScanListTab[0]);
//					ScanList.scanIndex=ScanListTab[0];
//				}
//				Scanflag--;
//				return ;
//			}
			if(StrMenuSet.ScanMode==MENUSETSCANMODE_SO)//载波模式
			{
				if(timecnt >= ScanList.remaintime)
				{
					timecnt = 0;
					if(/*(HAL_GPIO_ReadOutputDataBit(RF2_PORT,RF_TX_EN_PIN))||*/GloVarFlag.FmRecStat)
					{	                			
						g_remainflag =SCAN_REMAIN_TIME_MAX;
		                return ;
		            }
					ChnListScan();
				}
				else
				{
					timecnt++;
				}
			}
//			else if(StrMenuSet.ScanMode==MENUSETSCANMODE_TO)//时间模式
//			{
//				if(timecnt >= ScanList.remaintime)
//				{	
//					timecnt = 0;
//					if(/*(HAL_GPIO_ReadOutputDataBit(RF2_PORT,RF_TX_EN_PIN))||*/GloVarFlag.FmRecStat)
//					{
//						if(g_remainflag ==0)
//						{
//							g_remainflag=(StrMenuSet.ScanTOTime+1)*SCAN_TIMEMODE_REMAIN_TIME;
//							return ;
//						}
//		            }	
//					ChnListScan();
//				}
//				else
//				{
//					timecnt++;
//				}
//			}
			else if(StrMenuSet.ScanMode==MENUSETSCANMODE_SE)//搜索模式
			{
				if(timecnt >= ScanList.remaintime)
				{
					timecnt = 0;
					if(GloVarFlag.FmRecStat)
					{
						ScanCtrlSet(SCAN_OFF);
						ScanLastCtrlSet(SCAN_OFF);
					#if  1//区域切换到扫描到的区域.
						CHN_BLL_Set (CODESW_INTF_GetRealStat());
				//		ScanStatDisp();//扫描到了之后图标要消失掉;
					#endif	
		             	return ;
		            }
					ChnListScan();
				}
				else
				{
					timecnt++;
				}
			}
		}
		else if(ScanCtrlGet()==SCAN_ON&&ScanList.ScanMode==SCAN_MODE_FreqChange)//&&g_KeyFuncIndex == MenuTopIndex
		{	
			MenuVarFlg.g_ChecksignalFlg = 1;//在扫频模式的扫描的时候不要进入省电模式.
			if(StrMenuSet.ScanMode==MENUSETSCANMODE_SO)//载波模式
			{
				if(timecnt >= ScanList.remaintime)
				{
					timecnt = 0;
					if(/*(HAL_GPIO_ReadOutputDataBit(RF2_PORT,RF_TX_EN_PIN))||*/GloVarFlag.FmRecStat)
					{	            
						Scan_SaveFreq();   
						g_remainflag =SCAN_REMAIN_TIME_MAX;
						return ;
		      }
					ScanFreqMode();
				}
				else
				{
					timecnt++;
				}
			}
//			else if(StrMenuSet.ScanMode==MENUSETSCANMODE_TO)//时间模式
//			{
//				if(timecnt >= ScanList.remaintime)
//				{	
//					timecnt = 0;
//					if(/*(HAL_GPIO_ReadOutputDataBit(RF2_PORT,RF_TX_EN_PIN))||*/GloVarFlag.FmRecStat)
//					{
//						if(g_remainflag ==0)
//						{
//							Scan_SaveFreq();  
//							g_remainflag=(StrMenuSet.ScanTOTime+1)*SCAN_TIMEMODE_REMAIN_TIME;
//							return ;
//						}
//		            }	
//					ScanFreqMode();
//				}
//				else
//				{
//					timecnt++;
//				}
//			}
			else if(StrMenuSet.ScanMode==MENUSETSCANMODE_SE)//搜索模式
			{
				if(timecnt >= ScanList.remaintime)
				{
					timecnt = 0;
					if(GloVarFlag.FmRecStat)
					{
						Scan_SaveFreq();  
						ScanCtrlSet(SCAN_OFF);
						ScanLastCtrlSet(SCAN_OFF);
				//		ScanStatDisp();//扫描到了之后图标要消失掉;
	                	return ;
		            }
					ScanFreqMode();
				}
				else
				{
					timecnt++;
				}
			}
		}

        else if(GloVarFlag.KeyFlag!=1&&GloVarFlag.VoxFlg!=1)
        {
            if(/*(HAL_GPIO_ReadOutputDataBit(RF2_PORT,RF_TX_EN_PIN))||*/GloVarFlag.FmRecStat)
            {
                return ;
            }
            else
            {	
	            if(ScanLastCtrlGet()==SCAN_ON)
	            {
					ScanList.scanIndex=0;
					g_remainflag=0;
					g_presentchn = ScanList.scanIndex;
					ListChnSet(CODESW_INTF_GetRealStat());//当前信道
					
					ScanLastCtrlSet(SCAN_OFF);
	            }
            }
        }
	}	
}


void ScanFreqMode_SaveFreq(uint32 freq)
{
//	uint32 addr = CHN_APP_GetFreqBaseAddr(StrMenuSet.MainBand);
//	E2PROMWriteStr(addr+PCEA_RECFREQ,(uint8*) &freq,4);
//	E2PROMWriteStr(addr+PCEA_SENDFREQ,(uint8*) &freq,4);
}

//uint32 ScanFreqMode_GetFreq(void )
//{//注意:现在的保存方式还是之前的数模混合的方式
////	STR_CODESW PLLStat = GetPllStat();
//	STR_CHNINFO chninfo = GetPllChnInfo(StrMenuSet.MainBand);

//	uint32  Freq ,addr= StrMenuSet.MainBand?FREQALL_SAVE_START_ADDRB:FREQALL_SAVE_START_ADDRA;

//	E2PROMReadStr(addr+PCEA_RECFREQ,(uint8*)&Freq,4);
//	chninfo.PllValueRec=Freq;//(fp64)(Freq/1000000.0);//跟上扫描的节奏.	

//	SavePllChnInfo(StrMenuSet.MainBand,&chninfo);
//	Set_ReflashRxChannelSwFlagState(1);
//	return Freq;
//}
//


void ScanChangeFreq(void)
{
#if 0
//	STR_CODESW PLLStat=GetPllStat();
	STR_CHNINFO chninfo = GetPllChnInfo(StrMenuSet.MainBand);

//	uint32 Freq;
//	uint8 NumBuf[10];
	uint8 bandflg = StrMenuSet.MainBand;
	
	
	uint32 step =FreqStep_Tab[StrMMenuSetAB[bandflg].FStep];

	chninfo.PllValueRec = FreqAll_ChangeRecFreq(chninfo.PllValueRec,step,AllFlag.ScanFreqChangeDirFlag);
	chninfo.PllValueSnd = SFTSet_ChangeFreq(chninfo.PllValueRec);//(fp64)(FreqTx/1000000.0);/*发射频率*/

    SavePllChnInfo(bandflg,&chninfo);

//	if((Freq<=(Get_UFreqMax()))&&(Freq>=(Get_UFreqMin())))
//	{
//		if(AllFlag.ScanFreqChangeDirFlag==DIR_ADD)
//		{
//			if(Freq<=(Get_UFreqMax()))
//			{	
//				Freq+=(FreqStep_Tab[StrMMenuSetAB[bandflg].FStep]*10);
//				if(Freq>=(Get_UFreqMax()))
//				{	Freq=Get_UFreqMin();}
//			}
//		}
//		else
//		{
//			if(Freq>=(Get_UFreqMin()))
//			{	
//				Freq -=(FreqStep_Tab[StrMMenuSetAB[bandflg].FStep]*10);
//				if(Freq<(Get_UFreqMin()))
//				{	Freq=Get_UFreqMax()-(FreqStep_Tab[StrMMenuSetAB[bandflg].FStep]*10);}
//			}
//		}
//	}
//	else if((Freq<=(Get_VFreqMax()))&&(Freq>=(Get_VFreqMin())))
//	{
//		if(AllFlag.ScanFreqChangeDirFlag==DIR_ADD)
//		{
//			if(Freq<=(Get_VFreqMax()))
//			{	
//				Freq+=(FreqStep_Tab[StrMMenuSetAB[bandflg].FStep]*10);
//				if(Freq>(Get_VFreqMax()))
//				{	Freq=Get_VFreqMin();}
//			}
//		}
//		else
//		{
//			if(Freq>=(Get_VFreqMin()))
//			{	
//				Freq -=(FreqStep_Tab[StrMMenuSetAB[bandflg].FStep]*10);
//				if(Freq<=(Get_VFreqMin()))
//				{	Freq=Get_VFreqMax();}
//			}
//		}
//	}
//	else//出错处理.
//	{
//		Freq=455550000;
//	}
//	chninfo.PllValueRec = Freq;//(fp64)(Freq/1000000.0);		
//	chninfo.PllValueSnd = SFTSet_ChangeFreq(Freq);//(fp64)(FreqTx/1000000.0);/*发射频率*/

	//where_flag=0;	
	//ScanFreqMode_SaveFreq(Freq);//保存频率值和标志位
	//where_flag=1;
	//ScanFreqMode_SaveFreq(FreqTx);//保存频率值和标志位


	//Freq=Freq/10;
	if( StrMenuSet.All_Led)
	{
	//	DRV_RedLedOn();
	}
	
	SetRx_OnArrowBand();
	 	
//	LcdDispFreqTran(NumBuf,Freq);
//	AllFlag.DispBigAscFlag=1;
//	LCD_Menu_LcdWriteBigStr_BKImgae(ATTR_ROW_A+18,28,NumBuf,lcd_g);
	PLLDispModeX( bandflg);


	if(StrMenuSet.All_Led)
	{
	//	DRV_RedLedOff();
	}
	
#endif 
}

