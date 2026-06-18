/*****************************************************************************
 *   文件名:sleepmode.c
 *   文件描述:睡眠模式模块的函数集
 *   创建人：陶林，2010/08/17
******************************************************************************/


#include "app_config.h"

#include "include.h"


STR_SLPMODE_ Str_SlpMode = 
{
POWERMODE_NONE, 
PWMD_NONE, 
SLPSTAT_NSLP, 
PM_LED_EN, 
SLPMODECNT_MAX*2,
SLPMODECNT_MAX
};

//osSemaphoreId Sem_SlpLockId;
//osSemaphoreDef(name_Sem_SlpLock);


#define INTF_LcdLightSet(a)

#define DEF_SLP_MODE 1
extern uint8 LcdLightSet_flag;
/*****************************************************************************
 *   函数名: SlpModeInit
 *   功能描述: 低功耗模式初始化
 *   参数定义:
 *	返回值:
 *   创建人：陶林，2012/01/17
******************************************************************************/
void SlpModeInit(void) 
{
#if DEF_SLP_MODE
	Str_SlpMode.LedModeTime = 4;
	
//	Sem_SlpLockId=OSSemCreate(name_Sem_SlpLock,1);
#endif
}

/*****************************************************************************
 *   函数名: SlpMode1SSet
 *   功能描述: 1s 设置
 *   参数定义:
 *	返回值:
 *   创建人：陶林，2012/01/17
******************************************************************************/
void SlpMode1SSet(void)
{
#if DEF_SLP_MODE
	if(MenuVarFlg.g_ChecksignalFlg == 1)
	{
		Str_SlpMode.LedCnt = Str_SlpMode.LedModeTime;
		Str_SlpMode.Cnt = StrMenuSet.PowerMTime;
		MenuVarFlg.g_ChecksignalFlg = 0;
	}

	{
		if (Str_SlpMode.Cnt&&Func_Str.PowerSwitch&&!GloVarFlag.RadioFlag)
		{
			Str_SlpMode.Cnt--;
		}
//		if (Str_SlpMode.LedCnt&&StrMenuSet.LedOn==MENUSETLED_Auto)
//		{
//			Str_SlpMode.LedCnt--;
//		}
		SlpModeIdle();
//		if(KeyPadLightSet.Stat==KEYPAD_LIGHT_ENABLE&&KeyPadLightSet.Cnt)
//		{
//			KeyPadLightSet.Cnt--;
//			if(!KeyPadLightSet.Cnt)
//			{
//				KeyPadLightSet.Stat=KEYPAD_LIGHT_DISABLE;
//				KeyPad_Light_OFF();//关闭键盘背光灯.
//			}
//		}

	}
#endif	
}

/*****************************************************************************
 *   函数名: SlpModeIdle
 *   功能描述: idle任务调用
 *   参数定义:
 *	返回值:
 *   创建人：陶林，2012/01/17
******************************************************************************/
void SlpModeIdle(void)
{
#if DEF_SLP_MODE

	SEM_SLPLOCK;

	if (Str_SlpMode.ModeMStat != POWERMODE_SLP)
	{
		if (Str_SlpMode.Cnt == 0 && Func_Str.PowerSwitch) 
		{		
//			AllFlag.ABChangeTimerStopFlag=1;//停止切换定时器
//			if(StrMenuSet.LedOn == MENUSETLED_Auto)
//			{//只有在自动的时候才会关闭液晶背光灯.
//				BeforeSleepCloseTime2();
//				ClearAllShiftFunction();//准备清空掉闪烁定时器
//				if(AllFlag.KeyNumberOnFlag==1)//快捷设置的界面需要还原到桌面.
//				{
//					AllFlag.KeyNumberOnFlag=0;
//					KeyPutOpt(KEY_CRED,Key_CallclewBack); 
//				}
//				SleepTimeCouter=0;
//			}
			Str_SlpMode.ModeMStat = POWERMODE_SLP;		
		}
	}
	
	if (Str_SlpMode.ModeMStat == POWERMODE_IDLE)
	{		
		INTF_EnterSleepMode();
	}	
	else if (Str_SlpMode.ModeMStat == POWERMODE_SLP)
	{
		if (Str_SlpMode.SlpStat == SLPSTAT_NSLP)
		{			
			Str_SlpMode.SlpStat = SLPSTAT_SLP;
		}
		if (Str_SlpMode.ModeChckStat == PWMD_NONE)	/* 不在rtc 唤醒和c5000 check 模式 */
		{
			INTF_EnterSleepMode();		
		}
	}
	SEM_SLPUNLOCK;
	
	if(Str_SlpMode.LedCnt==0)
	{
		{
		//	DRV_GreenLedOff();
		//	DRV_RedLedOff();			
			INTF_LcdLightSet(FALSE);
		}	
	}
#endif
}

/*****************************************************************************
 *   函数名: SlpModeExitSM
 *   功能描述:退出深度睡眠
 *   参数定义:
 *	返回值:
 *   创建人：陶林，2012/01/17
******************************************************************************/
void SlpModeExitSM(void)
{
#if DEF_SLP_MODE

	SEM_SLPLOCK;

	if (Str_SlpMode.ModeMStat == POWERMODE_SLP)
	{		
		Str_SlpMode.ModeMStat = POWERMODE_IDLE;
		Str_SlpMode.SlpStat = SLPSTAT_NSLP;
		if(RDA1846sPdnDown)	APP_RDA1846SlpDis();

	}


	IS_COM_Wake_Up=1;
	if(AllFlag.SysStat_SleepFlag==1)
	{
		AllFlag.SysStat_SleepFlag=0;
		//if(StrMenuSet.LedOn == MENUSETLED_Auto)
		{
			WakeUpFromDeepSleepOperation();
			INTF_LcdLightSet(TRUE);
		}
		//ChangeTimer=0;//切换接收的定时器清空.
		//Set_ReflashRxChannelSwFlagState(1);
	}
	if(RDA1846sPdnDown	||
		(Str_SyatemSlp.CSLP_SystemSlpStat == SYSSLP_RDA1846SLP&&
		Str_SyatemSlp.CSLP_Mode == SYSSLP_MODE_SLP))
	{
		{
			Str_SyatemSlp.CSLP_Mode = SYSSLP_MODE_CHCK;
			Str_SyatemSlp.CSLP_SystemSlpStat = SYSSLP_RDA1846SLP;
			Str_SyatemSlp.CSLP_SlpCnt = 0;
			TaskSystemSleepAct();
		}
		INTF_LcdLightSet(TRUE);
	}

	Str_SlpMode.LedStat=PM_LED_EN;
	Str_SlpMode.LedCnt = Str_SlpMode.LedModeTime;
	INTF_LcdLightSet(TRUE);
	
	if (Str_SlpMode.ModeMStat == POWERMODE_SLP)
	{
		SlpModeFRtcExitSM();
	}
	Str_SlpMode.Cnt = StrMenuSet.PowerMTime;
//		INTF_LcdLightSet(TRUE);
	MenuVarFlg.g_ChecksignalFlg = 0;


//	LcdLightSet_flag = 0;


	SEM_SLPUNLOCK;

	
#endif		
}
/*****************************************************************************
 *   函数名: SlpModeExitSM
 *   功能描述:退出深度睡眠
 *   参数定义:
 *	返回值:
 *   创建人：陶林，2012/01/17
******************************************************************************/
void SlpModeFRtcExitSM(void)
{
#if DEF_SLP_MODE
	if (Str_SlpMode.SlpStat == SLPSTAT_SLP)
	{
		Str_SlpMode.SlpStat = SLPSTAT_NSLP;
		INTF_ExitDSlpMode();
		//INTF_LcdLightSet(TRUE);		
	}
#endif	
}

/*****************************************************************************
 *   函数名: SlpModeGetStat
 *   功能描述:获取模式
 *   参数定义:
 *	返回值:
 *   创建人：陶林，2012/01/17
******************************************************************************/
uint8 SlpModeGetStat(void)
{
	return Str_SlpMode.ModeMStat;
}
 void SlpModeSetStat(uint8 Stat)
{
	 Str_SlpMode.ModeMStat=Stat;
}
/*****************************************************************************
 *   函数名: APP_SMModeChckSet
 *   功能描述:2级功耗模式设置
 *   参数定义:
 *	返回值:
 *   创建人：陶林，2012/01/17
******************************************************************************/
void APP_SMModeChckSet(uint8 Flg)
{
	Str_SlpMode.ModeChckStat |= Flg;
}

/*****************************************************************************
 *   函数名: APP_SMModeChckClr
 *   功能描述:2级功耗模式清除
 *   参数定义:
 *	返回值:
 *   创建人：陶林，2012/01/17
******************************************************************************/
void APP_SMModeChckClr(uint8 Flg)
{
	Str_SlpMode.ModeChckStat &= ~Flg;
}


void WakeUpFromDeepSleepOperation(void)
{/*GD的芯片深度睡眠起来之后会死机或者晶振起振不正常*/
#if 0//(MCU_TYPE_SEL==MCU_TYPE_STM32)
	BSP_Init();//系统时钟重新初始化
	SlpModeSetStat(POWERMODE_IDLE);
	AfterWakeupOpenTime2();//LCD背光灯重新打开了.
	Str_SlpMode.LedCnt = Str_SlpMode.LedModeTime;
	Str_SlpMode.Cnt = StrMenuSet.PowerMTime;
	RDA1846sPdnDown=0;//背光灯自动模式下唤醒.重新配置了时钟
	Re_Enable_RTCclockAfterWakeUp();//RTC时钟重新定义了
	//不用时间的时候屏蔽掉ReFlashDateandTimeAfterDeepSleep();//重新更新时间了
	AllFlag.WakeupFromDeepSleepOpeOkFlag=1;
#else
	SlpModeSetStat(POWERMODE_IDLE);
	Str_SlpMode.LedCnt = Str_SlpMode.LedModeTime;
	Str_SlpMode.Cnt = StrMenuSet.PowerMTime;
	AllFlag.WakeupFromDeepSleepOpeOkFlag=1;

#endif
}




