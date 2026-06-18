/*******************************************************************************
 * Copyright (C) 2011-2013 ZheJiang Dahua Technology CO.,LTD.
 *
 * 文件名 : BLL_RF.c
 *
 * 作  者 : 123(ggg)<gg>
 * 版  本 : V1.0.0
 * 日  期 : 2015年4月14日 Create
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

#include "app_config.h"
 

#include "include.h"


#include "app_key.h"



/* ========================================================================== */
/*                           宏和类型定义区                                   */
/* ========================================================================== */

#define KEYTIMENUMBER 265//702  /// old 5202

#define KEYINDEXNUMOFPRESS 7//702  /// old 5202
/* ========================================================================== */
/*                          数据结构定义区                                    */
/* ========================================================================== */

/* ========================================================================== */
/*                          函数声明区                                                                                                 */
/* ========================================================================== */

void KEY_app_KeyNoHnd(void)
{
//	UARTSend("Noh.", 4);
printk("%s %d\r\n",__FUNCTION__,__LINE__);
	Sound_Alarm();	

}

void KEY_app_KeyPowerSet(void)
{
	DH_U8 power = CHN_BLL_PowerGet();	
	if(power==MENUSETPOWER_M)
	{
		CHN_BLL_PowerSet(power);
	}
	SOUND_BLL_SoundStartOpt(SOUNDSTAT_Voice,SOUNDSTAT_PowerVoice);
}

void KEY_app_KeyMonitor(void)
{
printk("%s %d\r\n",__FUNCTION__,__LINE__);
	KeyMonitor();
}

void KEY_app_KeyMonitor2(void)
{
printk("%s %d\r\n",__FUNCTION__,__LINE__);
	KeyMonitor();
}

void KEY_app_KeyScanSw(void)
{
printk("%s %d\r\n",__FUNCTION__,__LINE__);
	SOUND_BLL_SoundStartOpt(SOUNDSTAT_Voice,SOUNDSTAT_ScanStartVoice);

	ScanStart();
//	ScanStop();             

}



void KEY_app_KeyAlarm(void)
{
	
	printk("%s %d\r\n",__FUNCTION__,__LINE__);

}




#define KEY_TONE_TIMEE		6
#define TONE_DELAY_TIME	400

void ana_tx_tone(u16 freq)
{
	if(CHNSTAT_FM == CHN_BLL_GetAttr())
	{
		if( g_FpgaTx==0)
		{
			Call_ToneTxStart(freq );
		 
		}
		else
		{
			Call_ToneTxStop();
		}
	}
	else
	{	
		 
	}
}
//发送700_Tone
void KEY_app_700tone(void)
{	
	ana_tx_tone(700);
}
//发送1000_Tone
void KEY_app_1000tone(void)
{
	ana_tx_tone(1000);

}
//发送1200_Tone
void KEY_app_1200tone(void)
{
	ana_tx_tone(1200);
}
//发送1400_Tone
void KEY_app_1400tone(void)
{
	ana_tx_tone(1400);
}
void KEY_app_1450tone(void)
{
	ana_tx_tone(1450);
}
//发送2100_Tone
void KEY_app_2100tone(void)
{
	ana_tx_tone(2100);
}

void KEY_app_2tone(void)
{
printk("%s %d\r\n",__FUNCTION__,__LINE__);
	 
}


/* 操作函数指针 */
KEY_FPTR KeyP[] =
{
	KEY_app_KeyNoHnd,                                
	KEY_app_KeyMonitor,
	KEY_app_KeyMonitor2,
	KEY_app_KeyScanSw,   
	KEY_app_KeyAlarm,                                
                                                    
	KEY_app_700tone,
	KEY_app_1000tone,
	KEY_app_1400tone,
	KEY_app_1450tone,
	KEY_app_2100tone,                           //
	KEY_app_KeyPowerSet,//  
	KEY_app_2tone
};

/*******************************************************************************
 * 函数名  : KeyTimeInit
 * 描  述  :
 * 输  入  :  参数        描述
 *         :   DH_VOID         请输入DH_VOID的描述
 * 输  出  : 无
 * 返回值  : DH_SUCCESS: 成功
 *           DH_FAILURE: 失败
 *******************************************************************************/
DH_VOID KEY_APP_KeyTimeInit(DH_VOID)
{
#define PF_KEYFUNC_MAX (sizeof(KeyP)/sizeof(KEY_FPTR))
	Str_KeyT.P1SHnd = KeyP[0];
	Str_KeyT.P2SHnd = KeyP[0];
	Str_KeyT.P3SHnd = KeyP[0];
	Str_KeyT.P4SHnd = KeyP[0];
	if(Func_Str.pf1>PF_KEYFUNC_MAX)Func_Str.pf1 = 0;
	if(Func_Str.pf2>PF_KEYFUNC_MAX)Func_Str.pf2 = 0;
	if(Func_Str.pf3>PF_KEYFUNC_MAX)Func_Str.pf3 = 0;
	Str_KeyT.P1SHnd =  KeyP[Func_Str.pf1];
	Str_KeyT.P2SHnd =  KeyP[Func_Str.pf2];
	Str_KeyT.P3SHnd =  KeyP[Func_Str.pf3];
}


DH_U8 KEY_APP_IsLongPressFuncChk(DH_U8 key)
{
	if(key == Key_P1) return (Str_KeyT.P1SHnd == KEY_app_KeyMonitor);
	else if(key == Key_P2) return (Str_KeyT.P2SHnd == KEY_app_KeyMonitor);
	else if(key == Key_P3) return (Str_KeyT.P3SHnd == KEY_app_KeyMonitor);
	return 0;
}
