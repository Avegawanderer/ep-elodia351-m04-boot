/*****************************************************************************
 *   文件名:ScanDisp.c
 *   文件描述:循环显示任务源文件 
 *   创建人：陶林，2010/10/19
******************************************************************************/
#include "app_config.h"
 

#include "include.h"

/* 查询显示任务 */
//static void TaskScanDisp(void *pdata);		
void Coulovoice(void);

#define TASKScanDisp_STK_SIZE TASKScanDisp_STK_SIZE_
#define TASKScanDisp_PRIO TASKScanDisp_PRIO_
//static OS_STK  TaskScanDispStk[TASKScanDisp_STK_SIZE];


#define TASKScanFunction_STK_SIZE TASKScanFunction_STK_SIZE_
#define TASKScanFunction_PRIO TASKScanFunction_PRIO_
//static OS_STK  TaskScanFunctionStk[TASKScanFunction_STK_SIZE];



/*电量上一次保存的值*/
uint8 g_sendlimit;		/*发送限时标志位*/
uint8 LowPowerFlg=0;/*1为低电量的标志*/
uint8 DisableCouloScanTimer=0;//禁止扫描电量扫描的倒计时定时器.
uint8 SaveChnTimer=0;//保存信道的功能倒计时定时器


void Scan_Led_Stat(void);
void CouloScan(void);

/*****************************************************************************
 *   函数名: ScanDispInit
 *   功能描述:  查询显示任务初始化
 *   参数定义:	
 *	返回值:
 *   创建人：陶林，2010/10/26
******************************************************************************/
void ScanDispInit(void)
{


}

extern uint8 g_SqStatus;
extern const uint8 DcsCodeParam[];
extern const uint16 CtcssTones[];
extern uint16 g_AdcValueVox;
extern uint16 CurSubvoice;
void  SecondScan ( void )
{

	if(TaskHndGet(TASK_FLAG_1S) )
	{
		TaskHndStop(TASK_FLAG_1S); //只有初始化完毕之后才会进入这些显示函数.
	//printk("s.");
	#if 1
		if(!AllFlag.SysStat_DelOneChannelFlag && !GloVarFlag.CopyFlag)
		{	
			if (GloVarFlag.FmRecStat==0
			&& GloVarFlag.KeyFlag==0
			&& (ScanCtrlGet()==SCAN_OFF)
			&& GloVarFlag.FmMontFlg ==0/*&&!(HAL_GPIO_ReadOutputDataBit(RF2_PORT,RF_TX_EN_PIN))*/)
			{
				SlpMode1SSet();/*省电模式秒扫描*/
			}
			if((GloVarFlag.KeyFlag==0)
				&&(GloVarFlag.VoxFlg==0)
				&&GloVarFlag.FmMontFlg ==0
				&&DisableCouloScanTimer==0
				&&g_FpgaTx==0)
			{
				CouloScan();			//电量显示  
			}
			else if(DisableCouloScanTimer)
			{
				DisableCouloScanTimer--;
			}
			
			if(SlpModeGetStat() != POWERMODE_SLP)
			{//信号强度显示扫描函数
				if((GloVarFlag.KeyFlag==0)&&(GloVarFlag.VoxFlg==0))
				{
					//SignIntensGetSign();
				}
			}
			Rda1846sCheckErr();
			
			Scan_Led_Stat();

			#if 0	
			if(StrMenuSet.ctdcssScan&&g_Layer==2&&Str_MenuStat.SltItemBuf[g_Item]==E_TopFunc_SCN)
			{
				uint8 index, buf[10];
				buf[0]='S';
				buf[1]='C';
				buf[2]='.';
				if(StrMenuSet.ctdcssScan==1)
				{
					if(CheckctdcsFlg)
					{
						index = ctcss_scan();
						if(0xff==index);
						else
						{
							if(index != CurSubvoice)
							{
								start_ctcdcs_decode(CTCSS_ACTIVE, index);
							}
							else if(fCtdcssValid)
							{
							//	g_Str_ChnStat.RxSubVoice = CurSubvoice;
							}

						}
					}
					FM_CTCSS_SubVoiceGetStr(CurSubvoice,buf+3);
				}
				else
				{
					
					if(CheckctdcsFlg)
					{
						index = CheckCDcssScan(StrMenuSet.ctdcssScan);
						if(0xff==index);
						else
						{							
							if(index != CurSubvoice)
							{
								start_ctcdcs_decode(StrMenuSet.ctdcssScan, index);
							}
							else if(fCtdcssValid)
							{
							//	g_Str_ChnStat.RxSubVoice = CurSubvoice;
							}
						}
					}
					FM_CDCSS_SubVoiceGetStr(CurSubvoice,buf+3,StrMenuSet.ctdcssScan==E_CTD_CDCSS?1:0);
					
				}
				Lcd_INTF_DispStr(0, buf, 0);
				TmrHndStart(TMRHND_KEY_FUNC);
			}
			#endif
			#endif
		}

		
		}
} 



uint16 g_AdcValueCoulo;
uint16 g_AdcValueVox;

void idle_AdcChannelScan(void)
{
	if(AllFlag.SysStat_Poweron_OkFlag)
	{
	//	temp=ADC_INTF_GetValue(E_ADC_CHN_VOLATGE);
	}
}

uint8 CouloDisp_index;

/*****************************************************************************
 *	函数名: CouloScan
 *	功能描述:电量显示扫描
 *	参数定义:
 *	返回值:
 *	创建人：陶林，2011/03/31
******************************************************************************/
void CouloScan(void)
{
	uint16 CouloMax = 0;		  /* 电量AD  的最大 值 */
 //   static uint8 flg;
    CouloMax = ADC_INTF_GetValue(E_ADC_CHN_VOLATGE);
    CouloMax=CouloMax*100/57-9;
	if(!GloVarFlag.KeyFlag&&!GloVarFlag.VoxFlg)
	{
		if (CouloMax <= VOL_DIV_V_LOWLIMIT)
		{
			g_sendlimit = 1;/*禁止发射*/	
		}
		else if(CouloMax >=VOL_DIV_V_HIGHLIMIT)
		{
			g_sendlimit = 2;/*禁止发射*/	
		}
	}
		
	if (CouloMax > VOL_DIV_V_LOWLIMIT&&CouloMax <VOL_DIV_V_HIGHLIMIT)
	{
		g_sendlimit = 0;/*允许发射*/
	}
#if LCD_TYPE_SEL
	if (CouloMax > VOL_DIV_V_F)
	{
		LowPowerFlg=0;
		CouloDisp_index = 4;
	}
	else if (CouloMax > VOL_DIV_V_4)
	{
		LowPowerFlg=0;
		CouloDisp_index = 3;
	}
	else if (CouloMax > VOL_DIV_V_3)
	{
		LowPowerFlg=0;
		CouloDisp_index = 2;
	}
	else if (CouloMax > VOL_DIV_V_1)
	{
		LowPowerFlg=0;
		CouloDisp_index = 1;
	}
	else
	{
		LowPowerFlg=1;
		CouloDisp_index = 0;		
	}
#else
	if (CouloMax > VOL_DIV_L_H)
	{
		LowPowerFlg=0;
		CouloDisp_index = 3;
	}
	else if (CouloMax > VOL_DIV_L_M)
	{
		LowPowerFlg=0;
		CouloDisp_index = 2;
	}
	else if (CouloMax > VOL_DIV_V_1)
	{
		LowPowerFlg=0;
		CouloDisp_index = 1;
	}
	else
	{
		LowPowerFlg=1;
		CouloDisp_index = 0;		
	}

#endif
	if(CouloMax<VOL_DIV_V_1)
	{
		Coulovoice();
	}
#if 0
	if( StrMenuSet.All_Led)
	{
        if(flg == 0&&CouloDisp_index<1)
        {
			DRV_GreenLedOn();//HAL_UART0_Transmit("1.",2);
			DRV_RedLedOn();
			MenuVarFlg.g_ChecksignalFlg = 1;//
			
	        flg = 1;
        }
        else if(flg == 1)
        {
			DRV_RedLedOff();
			DRV_GreenLedOff();
			
	        flg = 0;
        }
	}
	CouloStatDisp(CouloDisp_index);
	#endif
}
/*****************************************************************************
 *	函数名: Coulovoice
 *	功能描述:电量提示音
 *	参数定义:
 *	返回值:
 *	创建人：陶林，2011/03/31
******************************************************************************/
void Coulovoice(void)
{	

//	#define CNT_MAX 10
	static uint8 CouloCnt = 0;	
//
//	if (GetMenuSetStat(BARTYPESETFLG_COLOUTONE) == COLOUSW_ON)
//	{
		if (CouloCnt++ > 5)
		{
//			uint8 Stat = GetMenuSetStat(BARTYPESETFLG_POWERVOLUME);
			MenuVarFlg.g_ChecksignalFlg	 = 1;
			CouloCnt = 0;
			SlpModeExitSM();
			
			#if LCD_TYPE_SEL	
 			Sound_Alarm();
 			#else
			SOUND_BLL_SoundStartOpt(SOUNDSTAT_Voice,SOUNDSTAT_LowCouloVoice);
			#endif
		}
//	}
}


/*****************************************************************************
 *	函数名: APP_1SCome
 *	功能描述:表示1s 时间到;
 *	参数定义:
 *	返回值:
 *    说明:被1s中断函数调用;
 *	创建人：陶林，2012/01/11
******************************************************************************/
	



/*****************************************************************************
 *   函数名:SKDisp
 *   功能描述:是否开启了声控开关
 *   参数定义:
 *	返回值:
 *   创建人：陶林，2011/08/15
******************************************************************************/
void SKDisp(void)
{

}


/*
*在待机层界面显示的10个图标，还有一个输入法没有显示
*/
void DispTuBiaoInit(void)
{

}



void Rda1846sCheckErr(void)
{

}

void DisplayCallinTb_Check(void)
{
}


void MENU_APP_SignalLine_Display(u8 Rxtx_flag,u8 num)
{
	 
}

void MENU_APP_DQT_Display(u8 rxtx_flag,u8 BandFlag)
{
	
}

void MENU_APP_WorkstateFlag_Display(u8 BandFlag,u8 flag)
{

}



//呼叫[主动/被动]状态下的桌面非选中段的显示,
//

void MENU_APP_SFT_Direction_Display(u8 BandFlag)
{
	
}
void MENU_APP_Rev_Display(u8 BandFlag)//倒频图标.
{
}


void TopFunCloseScanLoop(void)/*桌面层的秒扫描任务自动归0*/
{
	
}



void Scan_Led_Stat(void)
{
#if !LCD_TYPE_SEL	
	static uint8 idleled = 0;	
	if(g_SqStatus||0==Func_Str.sqlevel||GloVarFlag.FmMontFlg||GloVarFlag.FmRecStat)
	{
		GREEN_LED_ON;idleled = 0;	
	}
  	else  if (GloVarFlag.KeyFlag==1||GloVarFlag.VoxFlg==1||g_FpgaTx)//Stat<1||
	{
		RED_LED_ON;idleled = 0;	
		GREEN_LED_OFF;
	}
	else if(ScanCtrlGet()==SCAN_ON){}
	else
	{
		idleled ++;	
		idleled %= 3;
		if(idleled == 0	)
		{
			GREEN_LED_ON;
			if( CHN_BLL_GetAttr()==CHNSTAT_FM)	{	RED_LED_OFF;	}	// 
			else{ RED_LED_ON;};
		}
		else
		{
			GREEN_LED_OFF;
			RED_LED_OFF;
		}
	}
#endif
}
