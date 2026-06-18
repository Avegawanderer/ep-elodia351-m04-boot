/*****************************************************************************
 *   文件名:INTF_Vox.c
 *	 文件描述:
 *   创建人：陶林，2011/06/02
******************************************************************************/

#include "app_config.h"

#include "include.h"

#define VOX_STAT_1 420
uint16 VoxValue_temp;
static uint16 VoxStat_i = VOX_I_MAX;		/* 无效状态的i 值:大于MAX 关闭 */
uint16 VoxCallOutTimeOut=0;		/* 呼出超时计时器,如果呼出超时则在设定时间内不在声控发射，即使达到了对应的AD采样值*/
static uint8 VoxCrtlStat = 0;	/* 控制状态:关/开 */
uint8 voxbusyen;/*繁忙锁定控制开关，既是接收到信号后要不要开声控*/
//static OS_STK  TaskStkYuyin[YUYIN_TASK_STK_SIZE_ ];	
uint16 skdj[]={
	3914 ,
	3547 ,
	3181 ,
	2814 ,
	2447 ,
	2081 ,
	1714 ,
	1348 ,
	981 ,
	600 ,
	0
};


//3610,3220,2830,2200,1710,1200,1000,810,630,450,180,0};/*byliuzhihengyichu ,防止堆栈溢出*/
extern uint16 g_AdcValueVox;

//static void YuyinSKTask(void *pdata);
/*****************************************************************************
 *   函数名: INTF_VoxInit
 *   功能描述:
 *   参数定义:
 *	返回值:
 *   创建人：陶林，2012/02/28
******************************************************************************/
void INTF_VoxInit(void)
{
//	uint8 Stat;
	

	
	
//	lcd_infostr.vox = StrMenuSet.SKOn?:0;
}


const uint8 voxMaxIgrp[] = {
15,//300/20
25,//500/20
50 ,//1000/20
75 ,//1500/20
100 ,//2000/20
150 ,//3000/20
//200 ,//4000/20
//250,//5000/20
};
/*****************************************************************************
 *   函数名: INTF_VoxCheck
 *   功能描述:
 *   参数定义:
 *	返回值:
 *   创建人：陶林，2012/02/28
******************************************************************************/
void INTF_VoxCheck(void)
{
	uint16 VoxStat;
//	STR_CODESW PLLStat= GetPllStat();
	static uint8 FirstFlag=0;
	if(!FirstFlag)
	{
		FirstFlag=1;
		VoxCallOutTimeOut=VOX_I_MIN;
	}
	if (FmRecBusyChk())//正在接收且繁忙开关打开
	{
		VoxCallOutTimeOut=VOX_I_MIN;
	}
	else if (GloVarFlag.KeyFlag==1)//PTT发射按键已经发射了.
	{
		VoxCallOutTimeOut=VOX_I_MIN;
	}
	else if(CHN_BLL_GetAttr()==CHNSTAT_NONE)//未编程
	{
		VoxCallOutTimeOut=VOX_I_MIN;
	}
	else if(MenuVarFlg.VoiceFlg||SPK_INTF_IsOpen())
	{
		VoxCallOutTimeOut=VOX_I_MIN;
	}
	else if (GloVarFlag.KeyFlag==0)
	{
		uint8 SKDJ = Func_Str.voxlevel-1;
		VoxStat = ADC_INTF_GetValue(E_ADC_CHN_VOX_SET);//g_AdcValueVox;//DRV_GetAdc(CHN_ADC_VOX_DET);
		 
		if(VoxStat<(skdj[SKDJ]))	/* 未检测到 声音信号开始计时*/
		{			
			if (VoxStat_i<voxMaxIgrp[Func_Str.voxdelay])//3秒超时未检测到信号退出声控发射.
			{
			 	VoxStat_i++;
			}
		}
		else
		{
			VoxStat_i = 0;
		}
			
		if (VoxCallOutTimeOut)	/* 呼出超时时,n s 内不再呼出 */
		{
			VoxCallOutTimeOut--;
		}
		if (VoxStat_i >= voxMaxIgrp[Func_Str.voxdelay])
		{
			if (VoxCrtlStat == VOXCRT_ON)
			{
				SlpModeExitSM();
				VoxCrtlStat = VOXCRT_OFF;
				GloVarFlag.VoxFlg = 0;
			//	AllFlag.Reflash_TxSignleTBFlag=0;//图标刷新的标志位重新清除掉.
				KeyPutOpt(KEY_CRED, Key_CallUp);
				if(ScanCtrlGet()==SCAN_ONOF)
				{
						ScanCtrlSet(SCAN_ON);
				}
				VoxCallOutTimeOut = 50;
			}
		}
		else if(VoxStat_i==0)
		{
			if ((VoxCrtlStat==VOXCRT_OFF)&&(VoxCallOutTimeOut==0))
			{
				SlpModeExitSM();
				if(SKDJ>6 && ChkHasKeypadPress())
				{
					return;	
				}
				if(FALSE==DisableTxState())//符合发射的条件才能发射.
				{
					VoxCrtlStat = VOXCRT_ON;
					GloVarFlag.VoxFlg = 1;
					if(ScanCtrlGet()==SCAN_ON)
					{
						ScanCtrlSet(SCAN_ONOF);
						//if(TopSet_Str.ScanMode==SCAN_MODE_ChannelList)
						//{
						//	ListChnSet(ALLchannel-1);//当前信道
						//}
					}
					KeyPutOpt(KEY_CRED, Key_Call);//by liendi		
				}
			}
		}
	}
}

/*****************************************************************************
 *   函数名: INTF_VoxCallTimeOutStop
 *   功能描述:
 *   参数定义:
 *	返回值:
 *   创建人：陶林，2012/02/28
******************************************************************************/
void INTF_VoxCallTimeOutStop(void)
{
	VoxCallOutTimeOut = VOX_I_MIN;
	SlpModeExitSM();
	VoxCrtlStat = VOXCRT_OFF;
	GloVarFlag.VoxFlg = 0;
	KeyPutOpt(KEY_CRED, Key_CallUp);
}
/*****************************************************************************
 *   函数名: INTF_VoxChnChangeStop
 *   功能描述:信道切换时如果开了声控则需要关闭
 *   参数定义:
 *	返回值:
 *   创建人：
******************************************************************************/
void INTF_VoxChnChangeStop(void)
{
	VoxCallOutTimeOut = VOX_I_MAX;
	VoxCrtlStat = VOXCRT_OFF;
	GloVarFlag.VoxFlg = 0;
}
//
//void INTF_VoxKey(void)
//{
//	if (GetMenuSetStat(BARTYPESETFLG_SKOn) == MENUSETSK_OFF)
//	{
//		MenuSetSetted(BARTYPESETFLG_SKOn, MENUSETSK_ON);
//		YuyinSK();
//		StrMenuSet.SKOn=2;
//	}
//	else
//	{
//		CloseYuyinSK();
//		KeyPutOpt(KEY_CRED, Key_CallUp);
//	}
//
//	SKDisp();//声控
//}


//osThreadId ThreadId_vox;
//static void YuyinSKTask(void const *argument);
//osThreadDef(YuyinSKTask, osscanIndexityNormal, 0, YUYIN_TASK_STK_SIZE_);

void YuyinSK(void)
{       
	/* Create the first Thread */
//	ThreadId_vox = osThreadCreate(osThread(YuyinSKTask), NULL);

 //   INT8U err;
 //   err = OSTaskCreate(YuyinSKTask, (void *)0, &TaskStkYuyin[YUYIN_TASK_STK_SIZE_-1 ],YUYINUARTTASK_PRIO_);
 //   Assert(err == OS_NO_ERR, "YuyinSKTask_C_FAIL");
//	DRV_OpenMIC();
}
/*****************************************************************************
 *   函数名: CloseYuyinSK
 *   功能描述:关闭语音声控的功能
 *   参数定义:
 *	返回值:
 *   创建人：
******************************************************************************/
void CloseYuyinSK(void)
{       
	VoxCrtlStat = VOXCRT_OFF;
	GloVarFlag.VoxFlg = 0;
}

//void YuyinSKTask(const void *pdata)
//{       	
//	pdata = pdata;	
//
//	while(1)
//	{             	        		
//		osDelay(20);
//		if(AllFlag.SysStat_Poweron_OkFlag && StrMenuSet.SKOn == VOXCRT_ON)
//		{
//			INTF_VoxCheck();
//		}
//	}
//}

void YuyinSKTask( void )
{    //   	StrMenuSet.SKOn = VOXCRT_ON;
	if(TaskHndGet( TASK_FLAG_VOX))
	{          
		TaskHndStop(TASK_FLAG_VOX);
		if(Func_Str.voxlevel)
		{		
			INTF_VoxCheck();
		}else{
		
		}
	}
}

