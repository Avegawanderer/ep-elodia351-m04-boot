/*****************************************************************************
 *   文件名:CallDisp.c
 *	 文件描述: 菜单显示函数集
 *   创建人：陶林，2010/11/20
******************************************************************************/

#include "app_config.h"

#include "include.h"

CALLSTR CallStr = {0};

/* 按键已释放的时间变量 */
uint8 CallTimeout_flag;/*超时标志位*/
uint8 g_overtimeforbid;/*超时禁发标志*/

const uint8 g_calltimegrp[] = {0,1,3,10};


extern uint8 g_KeyMenuFunc;

DH_VOID FM_INTF_AlterToneFreq(int tonefreq)
{

}

DH_VOID FM_INTF_GenerateTone(int tonefreq)
{

}
DH_VOID FM_INTF_StopTone(void)
{

}
/*****************************************************************************
 *   函数名: CallDispInit
 *   功能描述:  初始化通话界面显示
 *   参数定义: 
 *	返回值: 
 *   创建人：陶林，2010/11/22
******************************************************************************/
void CallDispInit(void)
{


}

void INTF_FmRecvEn(void)
{
	RF_BLL_EnableRx();   
	FM_APP_SetRxSubVoice();
	SPK_INTF_CloseTone();
}
/*****************************************************************************
 *   函数名: CallRefHnd
 *   功能描述: 刷新通话界面执行函数
 *   参数定义:		
 *	返回值:
 *   创建人：陶林，2010/10/22
******************************************************************************/
void CallRefHnd(void)
{
	SlpModeExitSM();
	
	do
	{
		/* 通话时间过长 */
		if (CallStr.N_100Ms >= g_calltimegrp[Func_Str.tot]*60*15&&Func_Str.tot)//by liendi
		{
			if (GloVarFlag.KeyFlag)
			{
				if(g_overtimeforbid)
				{	
					Sound_Alarm();
				}
				else
				{
					CallTimeout_flag = 1;
					g_overtimeforbid = 1;

					FM_APP_SetTail();

					INTF_FmRecvEn();
					if(GloVarFlag.RadioPauseFlag)
					{
						RadioNPauseTimer=0;
						TmrHndStart(TMRHND_SYJ);
					}
					GREEN_LED_OFF;
					#if 0
					if(g_Layer==1)
					{
						//LCD_BLL_LcdClearArea_BKImgae(59,21,5,84,LCD_White);/*显示信号强度图标*/
						MENU_APP_DQT_Display(RX_FLAG,StrMenuSet.MainBand);
						BandTwo_DisplayArrow();
					
					}
					#if CALLDISP_EN
					//g_FpgaTx=0;//防止再次刷新桌面层.
					DispMenuTop();
					#endif
				//	CloneSuccessSound();
					#endif

				}
			}
			else if (GloVarFlag.VoxFlg)
			{
				INTF_VoxCallTimeOutStop();
			}
			break;
		}

		CallStr.N_100Ms += 5;							
	
	}while(0);		
}


uint8 FmRecBusyChk(void)
{
	if( GloVarFlag.FmRecStat)
	{
		if(CHN_BLL_GetBusyFlag()==1)//载波禁止
		{
			return 1;
		}
		else if(CHN_BLL_GetBusyFlag()==2)
		{
		//	if(g_Str_ChnStat.R_S_SubType.b_type_r==E_CTD_SQ)	return 1;
		}
	}
	else
	{
		if(CHN_BLL_GetBusyFlag())
		{
			if(g_msenden )	return 1;//有载波，未接收则禁止
		}
	}
	return 0;
}


extern uint8 g_sendlimit;//
uint8 DisableTxState(void)
{
	if(FmRecBusyChk())
	{//繁忙开关打开了并且处于接收状态.
		//此处应该加入无屏机繁忙发射失败提示	
		return TRUE;
	}
	if(g_sendlimit)        /* 错误电量时禁止发射 */
	{
		if(g_sendlimit==2)
		{
		}
		else
		{
		}
		return TRUE;
		
	}
	if(GloVarFlag.PcWriteReadFlg)        /*写频时禁止发射 */
	{
		return TRUE;
	}

	return FALSE;
}

/*****************************************************************************
 *   函数名: CallOutStart
 *   功能描述:CallOut开始
 *   参数定义:
 *	返回值:
 *   创建人：陶林，2011/03/03
******************************************************************************/
void CallOutStart(void)
{		
	if(TRUE==DisableTxState())	
	{
		RED_LED_ON;
//		if(GloVarFlag.CallToneFlg)
//		{
			Sound_Alarm();
//		}
//		else
//		{
//			DRV_ToneVoiceStart(500);
//			while  (GloVarFlag.KeyFlag && DRV_RdaSqStat()){
//			//	Sound_Alarm();
//				osDelay(100);
//			}	
//			DRV_ToneVoiceEnd();
//			DRV_KeyAudioSwitch(DISABLE);
//		}
	    RED_LED_OFF;
	    GloVarFlag.FmRecStat = 0;
		//g_FpgaTx=0;
		if(!GloVarFlag.KeyFlag)		return;
	}
	TmrHndStop(TMRHND_KEY_FUNC);

	if(ScanCtrlGet()==SCAN_ON)
	{
		ScanCtrlSet(SCAN_ONOF);
		RED_LED_OFF;
		Scanflag=0;
	}

	if(GloVarFlag.FmRecStat)//有信号接收状态,喇叭已经打开.
	{		
//		SQ_Check_Enable=1;//不再信号查询
		GloVarFlag.FmRecStat = 0;
//	    INTF_FmAfOutMute();
		SPK_INTF_Close();
	}
	else
	{
	
	}

	
	{
		g_FpgaTx=1;
		RF_BLL_EnableTx();   
		FM_APP_SetTxSubVoice();
//		if(StrMenuSet.Call_BOTEOT&0x01 && !GloVarFlag.CallToneFlg)
//		{
//			FM_INTF_GenerateTone( 700);
//			osDelay(250);
//			FM_INTF_StopTone();
//		}
	}
	TmrHndStart(TMRHND_CALLREF);
}


void Call_ToneTxStart(int freq)
{
	
	printk("%s %d %d\r\n",__FUNCTION__,__LINE__,freq);

	CallOutStart();
	if(g_FpgaTx)
	{
		Tone_SetFreq(freq);//open tone pwm
		SPK_INTF_OpenTone();//open tone pwm

	}
}
void Call_ToneTxStop(void)
{
printk("%s %d\r\n",__FUNCTION__,__LINE__);
	KeyPutOpt(KEY_CRED, Key_CallUp);
}






