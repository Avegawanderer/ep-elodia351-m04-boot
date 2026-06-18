/*****************************************************************************
 *   文件名:TmrHnd.c
 *	 文件描述:Tmr 执行函数集
 *   创建人：陶林，2011/03/17
******************************************************************************/
#include "app_config.h"
#include "include.h"

//void TmrHndTask(void *pdata);	
#define TMRHND_STK_SIZE TMRHND_STK_SIZE_
#define TASKTMRHND_PRIO TASKTMRHND_PRIO_

/* 定时器计数器 */
typedef struct{
	uint8 keyFtmr;
    uint8 animal;
    uint8 callref;
    uint8 keylongtmr;
    uint8 scantmr;
    uint8 pllchktmr;
    uint8 secondscantmr;
}TMR_STR;
TMR_STR str_Timer_Cnt;


static uint32 TmrHndFlg = 0;	/* 标记需要执行的任务 */

uint8 RadioNPauseTimer=0;/*重开收音机的倒计时计数器.*/

//OS_THREAD_DEF_ID(TmrHndTask,osscanIndexityNormal,  TMRHND_STK_SIZE);

//osTimerId os100MsTimerId;

/* Private function prototypes -----------------------------------------------*/
static void osTimer100msCallback(void const *argument);

/*Timer definition*/
//osTimerDef(Timer100Ms, osTimer100msCallback);



extern uint8 KeyLongTimeNum;
//uint8 timer_animal;

extern uint8 g_KeyMenuFunc;
/*****************************************************************************
 *   函数名: TmrHndInit
 *   功能描述: 定时器处理函数初始化
 *   参数定义:	
 *	返回值:
 *   创建人：陶林，2011/03/17
******************************************************************************/
void TmrHndInit(void)
{
//	ThreadId_TmrHndTask = OSTaskCreate(TmrHndTask, (void *)0, &TmrHndTask_[TMRHND_STK_SIZE-1], TASKTMRHND_PRIO);
	/* Create Timer */
//	os100MsTimerId = osTimerCreate(osTimer(Timer100Ms), osTimerPeriodic, NULL);
}

void osTimer100msCallback(void const *argument)
{	
//	COMMONPost(ThreadId_TmrHndTask,TMRHND_Flag);

}

/*****************************************************************************
 *   函数名: TmrHndTask
 *   功能描述: 定时器处理函数
 *   参数定义:	
 *	返回值:
 *   创建人：陶林，2011/03/17
******************************************************************************/
void TmrHndTask(void const *pdata)
{
//	uint8 err;

	while(TaskHndGet(TASK_FLAG_100MS) )
	{
		TaskHndStop(TASK_FLAG_100MS);
//		COMMONPend(TMRHND_Flag);
		{
			sys_tick_exta();

			if (TmrHndFlg & TMRHND_DLYHND)	/* 延时执行函数 */
			{
//				DlyHndHnd_();
				TmrHndFlg &= ~TMRHND_DLYHND;		
			}
			if (TmrHndFlg & TMRHND_KEYLONG)	/* 长按键执行函数 */
			{
				str_Timer_Cnt.keylongtmr++;
				if(str_Timer_Cnt.keylongtmr>KeyLongTimeNum)
				{
					KeyTimeLongHnd();
					if(Str_KeyT.KeyLongHnd) KeyPutOpt(KEY_CRED, Key_LongHnd);
					TmrHndFlg &= ~TMRHND_KEYLONG;
				}
			}
			if (TmrHndFlg & TMRHND_CALLREF)	/*仅用于通话时长的计数定时器*/
			{			 	
				str_Timer_Cnt.callref++;
				if(str_Timer_Cnt.callref>=5){
					str_Timer_Cnt.callref=0;
					KeyPutOpt(KEY_CRED, Key_Ref);
			}
			//	TmrHndFlg &= ~TMRHND_CALLREF;		
			}
			if (TmrHndFlg & TMRHND_SCAN)	
			{
				str_Timer_Cnt.scantmr++;
				
				if(str_Timer_Cnt.scantmr>=3)
				{
					str_Timer_Cnt.scantmr = 0;
			//		if(ScanCtrlGet()==SCAN_ON)KeyPutOpt(KEY_UNCRED,Key_Scan);
			//		else TmrHndFlg &= ~TMRHND_SCAN;
				}
				
			}
		#if 0// FM_IC_SEL==FM_IC_W12T
//			if (TmrHndFlg & TMRHND_ANIMAL)	
//			{
//				str_Timer_Cnt.animal++;
//				if(str_Timer_Cnt.animal>=5)
//				{
//					MenuWriteLock();
//					if (TmrHndFlg & TMRHND_ANIMAL) LCD_INTF_Blink();					
//					MenuWriteUnlock();
//					str_Timer_Cnt.animal = 0;
//				}
//			}
//			if (TmrHndFlg & TMRHND_KEY_FUNC)	
//			{
//				ChangeTimer = 0;//设置功能时不切换
//				str_Timer_Cnt.keyFtmr++;
//				if(str_Timer_Cnt.keyFtmr>=100)
//				{
//					if(g_Layer==2 
//					|| (g_KeyMenuFunc == E_Setting_Ready)
//					|| (TmrHndFlg & TMRHND_ANIMAL)
//					)
//					{
//						KeyPutOpt(KEY_CRED, Key_SimuBack);
//					}
//					TmrHndFlg &= ~TMRHND_KEY_FUNC;
//				}
//			}
			if (TmrHndFlg & TMRHND_PLL_CHK)	
			{
				str_Timer_Cnt.pllchktmr++;
				if(str_Timer_Cnt.pllchktmr>=2)
				{
					TmrHndFlg &= ~TMRHND_PLL_CHK;
					
					INTF_RFLock();
					W12T_INTF_PllChk();
					INTF_RFUnlock();
				}
			}
			//if (TmrHndFlg & TMRHND_DLYHNDSTART)	/* 启动延时定时器 */
			//{
			//	OSTmrStart(PTMR_DlyHnd, &err);
			//	Assert( err == OS_NO_ERR ,"PTMR_VolChk启动不成功!");
			//	TmrHndFlg &= ~TMRHND_DLYHNDSTART;		
			//}
//			if (TmrHndFlg & TMRHND_SYJ )	/* 收音机重新打开定时器 */
//			{
//				RadioNPauseTimer++;
//				SYJTmrHnd();
//				if(RadioNPauseTimer>100)
//				{
//					RadioNPauseTimer=0;	
//					//SYJHnd();	
//					KeyPutOpt(KEY_CRED, Key_RadioRestart);
//					TmrHndFlg &= ~TMRHND_SYJ ;		
//				}		
//			}
		#endif		
			
			str_Timer_Cnt.secondscantmr++;
			if(str_Timer_Cnt.secondscantmr>=10)
			{
				str_Timer_Cnt.secondscantmr = 0;
				TaskHndStart(TASK_FLAG_1S);
			}
			
	//		FreqInputCursorDisplay();
		}
	}
}

/*****************************************************************************
 *   函数名: TmrHndStart
 *   功能描述: 开启定时器任务
 *   参数定义:	Flg:开启对应的任务
 *	返回值:
 *   创建人：陶林，2011/03/17
******************************************************************************/
void TmrHndStart(uint32 Flg)
{
//	uint8 err;
	
	TmrHndFlg |= Flg;	

	if(Flg==TMRHND_KEY_FUNC)
	{
		str_Timer_Cnt.keyFtmr = 0;
	}
	else if(Flg==TMRHND_KEYLONG)
	{
		str_Timer_Cnt.keylongtmr = 0;
	}
	else if(Flg==TMRHND_SCAN)
	{
		str_Timer_Cnt.scantmr = 0;
	}
	else if(Flg==TMRHND_PLL_CHK)
	{
		str_Timer_Cnt.pllchktmr = 0;
	}
}
void TmrHndStop(uint32 Flg)
{
//	uint8 err;
	
	TmrHndFlg &= ~Flg;	
}

uint8 TmrHndGet(uint32 Flg)
{
//	uint8 err;
	
	return (TmrHndFlg & Flg)?1:0;	
}

