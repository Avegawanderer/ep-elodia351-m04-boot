/*****************************************************************************
 *   文件名:keyTime.c
 *	 文件描述:按键定时器函数集
 *   创建人：陶林，2011/08/06
******************************************************************************/
#include "app_config.h"


#include "include.h" 


//OS_TMR* PTMR_Key;
//OS_TMR* PTMRURGENT_Key;
uint8 PrmrKeyOpt;		/* 暂时未使用 */
//extern  OS_TMR* PTMR_InputCursor; // 光标闪烁函数的TMR

STR_KEYT Str_KeyT;

/* 标记当前功能按键的模式 */
//static uint8 g_KeyFncStat = KEYT_NONE;

extern uint8 ListChnSw;
uint8 KeyLongTimeNum;

uint8 g_monitorflag = 1,g_fmonitorflag = 0;	/*监听标志位*/


#define KEYTIMENUMBER 1106


/* Private function prototypes -----------------------------------------------*/
///static void KeyTimeLongHnd(void const *argument);
/*Timer definition*/
//osTimerDef(TimerKeyLong, KeyTimeLongHnd);



void KeyRadioScanStart(void);

void Key_RadioSW(void);

void KeyUpDownLong(void);

//顺序为1的长按,1的短按,2 的长按....6 的短按.
/*****************************************************************************
 *   函数名: KeyTimeInit
 *   功能描述:
 *   参数定义:
 *	返回值:
 *   创建人：陶林，2011/08/12
******************************************************************************/
void KeyTimeInit(void)
{
//	uint8 err;
	//uint8 KeyT = E2PROMReadByte(PCBA_LONGKEYTIME) ;
	//uint8 LongKeyT =5 * (KeyT&PCEA_LONGKEYBITS) + 5;
	//uint8 UrgentShortKeyT = (KeyT&PCEA_URGENTSHORTKEYBITS)>>4;
//	uint8 KeyHnd;
	KeyLongTimeNum = 60;//LongKeyT;

	Str_KeyT.P1LHnd = KeyMonitor;
	Str_KeyT.OKLHnd = KeyLock;
	Str_KeyT.BackLHnd = 0;//SetSingleDidplayMode;

	KEY_APP_KeyTimeInit();
}

/*****************************************************************************
 *   函数名: KeyTimeStart
 *   功能描述: 按下按键时调用此函数
 *   参数定义:
 *	返回值:
 *   创建人：陶林，2011/08/12
******************************************************************************/
void KeyTimeStart(uint8 Flg)
{
	//uint8 err;
	//OS_TMR* PTmr = (OS_TMR*)PTMR_Key;

	if((GloVarFlag.KeyFlag==1))
		return;
	Str_KeyT.KeyType = Flg;
	Str_KeyT.KeyLSFlg = KEY_FLG_NONE;

//	if(Flg==Key_Back)
//	{//切换到快捷设置的状态只要1秒钟就够.
//		PTMR_Key->OSTmrPeriod =OS_TMR_CFG_TICKS_PER_SEC;
//		PTMR_Key->OSTmrDly =OS_TMR_CFG_TICKS_PER_SEC;
//	}
//	else
//	{
//		PTMR_Key->OSTmrPeriod =KeyLongTimeNum*OS_TMR_CFG_TICKS_PER_SEC/10;
//		PTMR_Key->OSTmrDly=KeyLongTimeNum*OS_TMR_CFG_TICKS_PER_SEC/10;
//	}

	TmrHndStart(TMRHND_KEYLONG);

//	OSTmrStart(osKeyLongTimerId, 100*KeyLongTimeNum);
//	Assert( err == OS_NO_ERR ,"PTMR_Key_TOT_Start_Fail");

	
}

/*****************************************************************************
 *   函数名: KeyTimeStart
 *   功能描述: 按下按键时调用此函数
 *   参数定义:
 *	返回值:
 *   创建人：陶林，2011/08/12
******************************************************************************/
//void KeyUrgentTimeStart(uint8 Flg)
//{
//#if 0
//	uint8 err;
//	if(GloVarFlag.KeyFlag==1)
//		return;
//	Str_KeyT.KeyType = Flg;
//	Str_KeyT.KeyLSFlg = KEY_FLG_NONE;
//
//	OSTmrStart(PTMRURGENT_Key, &err);
//	Assert( err == OS_NO_ERR ,"超时处理启动不成功!");
//	#endif
//}
/*****************************************************************************
 *   函数名: KeyTimeLongHnd
 *   功能描述: 长按键定时时间到执行此函数
 *   参数定义:
 *	返回值:
 *   创建人：陶林，2011/08/12
******************************************************************************/
void KeyTimeLongHnd(void )
{
	if (Str_KeyT.KeyLSFlg == KEY_FLG_NONE)
	{
		Str_KeyT.KeyLSFlg = KEY_FLG_LONG;

		if (Str_KeyT.KeyType == Key_P1)
		{			
			Str_KeyT.KeyLongHnd = Str_KeyT.P1LHnd;
		}
        /*按键声*/
		TmrHndStart (TMRHND_KEYLONG);
	}

}

/*****************************************************************************
 *   函数名: KeyTimeStop
 *   功能描述: 松开按键时调用此函数
 *   参数定义:
 *	返回值:
 *   创建人：陶林，2011/08/12
******************************************************************************/
void KeyTimeStop(void)
{

	if (Str_KeyT.KeyLSFlg == KEY_FLG_NONE)
	{
		Str_KeyT.KeyLSFlg = KEY_FLG_SHORT;

		TmrHndStop(TMRHND_KEYLONG);
		Str_KeyT.KeyType = 0;
	}
}
/*****************************************************************************
 *   函数名: KeyUrgentTimeStop
 *   功能描述: 松开按键时调用此函数
 *   参数定义:
 *	返回值:
 *   创建人：陶林，2011/08/12
******************************************************************************/
void KeyUrgentTimeStop(void)
{
#if 0
	uint8 err;

	if (Str_KeyT.KeyLSFlg == KEY_FLG_NONE)
	{
		Str_KeyT.KeyLSFlg = KEY_FLG_SHORT;

		OSTmrStop(PTMRURGENT_Key, OS_TMR_OPT_NONE, 0, &err);

		if (Str_KeyT.KeyType >= Key_P1)
		{
			Str_KeyT.P1SHnd = KeyP[10];
		}
		else if (Str_KeyT.KeyType == Key_P2)
		{
			Str_KeyT.P2SHnd = KeyP[10];
		}
		else if (Str_KeyT.KeyType == Key_P3)
		{
			Str_KeyT.P3SHnd = KeyP[10];
		}
		else if (Str_KeyT.KeyType == Key_P4)
		{
			Str_KeyT.P4SHnd = KeyP[10];
		}
		else if (Str_KeyT.KeyType == Key_P5)
		{
			Str_KeyT.P5SHnd = KeyP[10];
		}
		else if (Str_KeyT.KeyType == Key_P6)
		{
			Str_KeyT.P6SHnd = KeyP[10];
		}
		TmrHndStart (TMRHND_KEYLONG);
        /*按键声*/
		//KeyVoiceStart(KEY_FLG_LONG);
	}
	#endif
}


/* 无操作 */
void KeyNoHnd(void)
{
	Sound_Alarm();	
}

/* 背景灯开关1 */
void KeyLedSw(void)
{
//  INT8U err;
#if 0
	uint8 Flg = GetMenuSetStat (BARTYPESETFLG_LedOn);

	if (Flg != MENUSETLED_OFF)
	{
		MenuSetSetted(BARTYPESETFLG_LedOn, MENUSETLED_OFF);
	}
	else
	{
		MenuSetSetted(BARTYPESETFLG_LedOn, MENUSETLED_ON);
	}
#elif 0
	uint8 Flg = GetMenuSetStat (BARTYPESETFLG_LedOn);

	if(Flg==MENUSETLED_ON)
	{
		Flg=MENUSETLED_OFF;
		MenuSetSetted(BARTYPESETFLG_LedOn, MENUSETLED_OFF);
	}
	else if (Flg == MENUSETLED_OFF)
	{
		Flg=MENUSETLED_Auto;
		MenuSetSetted(BARTYPESETFLG_LedOn, MENUSETLED_Auto);
	}
	else
	{
		Flg=MENUSETLED_ON;
		MenuSetSetted(BARTYPESETFLG_LedOn, MENUSETLED_ON);
	}
	MenuWriteLock();
	Display_SetOK_Loop((uint8*)*(StrLightMode[StrMenuSet.language].Text+Flg));
	MenuWriteUnlock();	
	KeyPutOpt(KEY_CRED, Key_SimuBack);
#endif
}

extern uint8 g_KeyMenuFunc;
/* 键盘锁 */
void KeyLock(void)
{
#if (0)
	if(g_KeyMenuFunc == E_Setting_Ready) return;

	//uint8 Flg = GetMenuSetStat (BARTYPESETFLG_Lcok);
	if (Str_Lock.Stat == LOCK_YES)
	{
		LockKpForceClear();
		//MenuSetSetted(BARTYPESETFLG_Lcok, MENUSETLOCK_OFF);
		
	}
	else
	{
		LockKpForceLock();
		//MenuSetSetted(BARTYPESETFLG_Lcok, MENUSETLOCK_ON);
	}
	KeyPutOpt(KEY_CRED, Key_SimuBack);
//	KeyPutOpt(KEY_CRED, Key_SimuBack);
#endif//无屏机时不需要锁键盘这个项目
}

extern uint8 g_KeyMenuFunc;
void KeyUpDownLong(void)
{
	#if  0
	uint8 stat,err;
	if(GloVarFlag.KeyFlag==0)  //当没有ptt 键按下时执行按键操作
	{
		KeyVoiceEnd();
		while(1){
			stat=KeyStatChk(&err);
			
//		  if(stat==ENM_Key_Up&&Key_Up==Str_KeyT.KeyType )
//			{
//				g_KeyFuncIndex = g_KeyTab[g_KeyFuncIndex].KeyUpState;
//			}
//			else if(stat==ENM_Key_Down&&Key_Down==Str_KeyT.KeyType )
//			{
//				g_KeyFuncIndex = g_KeyTab[g_KeyFuncIndex].KeyDownState;
//			}
//			else break;	

			if(GloVarFlag.RadioFlag && !GloVarFlag.RadioPauseFlag)
			{
				Radio_KeyEvent(Str_KeyT.KeyType);
			}
			else if(g_KeyMenuFunc == E_Setting_Ready && g_Layer==1)
			{
				KeyFuncQuickEnter(Str_KeyT.KeyType);
			}
			else
			{
				g_KeyFuncPtr = g_KeyTab[g_KeyFuncIndex].CurrentOperate;
				(*g_KeyFuncPtr)();//执行当前按键的操作
			
				if( g_Flash)
				{
					MenuSBChck();
					g_KeyFuncPtr = g_KeyTab[g_KeyFuncIndex].CurrentOperate;
					(*g_KeyFuncPtr)();					//执行当前按键的操作
					g_Flash = 0;
				}
			}
			osDelay(100);
			if(g_Layer==2 || TmrHndGet(TMRHND_KEY_FUNC)) TmrHndStart(TMRHND_KEY_FUNC);
		}
	}
	#endif
}

/* 模拟监听 */
void KeyMonitor(void)
{
#if 1
	if(CHN_BLL_GetAttr()!=CHNSTAT_NONE)
	{
		if(!GloVarFlag.FmMontFlg)
		{
			GloVarFlag.FmMontFlg=1;
			if(GloVarFlag.FmRecStat)//处于正在接收的状态下
			{
			}
			else
			{
				 
			}
			//INTF_FmMontStart();
		}
		else
		{
			GloVarFlag.FmMontFlg=0;
			//INTF_FmMontStop();
		}
                
	}
	else
	{
		 
	}
	#endif
}


/* 扫描开关 */
void KeyScanSw(void)
{
#if 0
	STR_CODESW PLLStat;
	AllFlag.DispMsgIsBusyFlag=1;//静止掉360编码器的响应.
	#if CALLDISP_EN
	if(RxBusyCheck()||GloVarFlag.FmMontFlg||Check_SQ_Zero_Onband())//不允许开启扫描功能.
	{
		Display_SetFail_Loop((uint8*)*(SetFail[StrMenuSet.language].Text+1));
		DispMenuTop();
	}
	else
        #endif
	{
		if(ScanCtrlGet()==SCAN_ON)
		{//
			ScanCtrlSet(SCAN_OFF);
			Display_SetOK_Loop((uint8*)*(StrMenuM_5_6_10_3_3[StrMenuSet.language].Text+1));
			if(!FreqallStat_Check())
			{
				if(StrMenuSet.MainBand)//B段
				{
					CHN_BLL_Set(PLLStat_NumB);
				}
				else//A段
				{
					CHN_BLL_Set(PLLStat_NumA);
				} 			
			}
		}
		else
		{
			if(FreqallStat_Check())
			{
				TopSet_Str.ScanMode=SCAN_MODE_FreqChange;
				Display_SetOK_Loop((uint8*)*(StrMenuM_5_6_10_3_3[StrMenuSet.language].Text+0));
				uint32 Freq;

				PLLStat= GetPllStat();
				Freq=(uint32)(PLLStat.chninfo[StrMenuSet.MainBand].PllValueRec);
				
				ScanFreqMode_SaveFreq(Freq);//保存频率值和标志位
				ScanCtrlSet(SCAN_ON);
			}
			else
			{
				if(Scan_APP_GetChnNum()==0&&!FreqallStat_Check())
				{
					Display_SetFail_Loop((uint8*)*(StrListEmpty[StrMenuSet.language].Text+0));
				}
				else
				{
					TopSet_Str.ScanMode=SCAN_MODE_ChannelList;
					Display_SetOK_Loop((uint8*)*(StrMenuM_5_6_10_3_3[StrMenuSet.language].Text+0));
					Scanflag=1;//为了多停留1S
	 				CHN_BLL_Set(Scan_APP_GetChnIndex(0));//切换到扫描列表第一个的信道上去
					ScanCtrlSet(SCAN_ON);
				}
			}
		}
		DispMenuTop();
	}
	AllFlag.DispMsgIsBusyFlag=0;//重新打开360编码器的动作.
#endif
}

void Key_RadioSW(void)
{

}
/*手电筒的开关*/
void KeyLight(void)
{
#if 0	
	static uint8 flg=1;
	if(flg)
	{
		flg=0;
		//DRV_FlashlightOn();
		//DRV_PwrClr();
		//D_FmAfOutMute();
		//analog_on=1;
	}
	else
	{
		flg=1;
		//DRV_FlashlightOff();
		//D_FmAfOutOpen();  
		//analog_off=1;

	}
#endif
}



