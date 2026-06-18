/*******************************************************************************
 * Copyright (C) 2011-2013 ZheJiang Dahua Technology CO.,LTD.
 *
 * 文件名 : BLL_SOUND.h
 *
 * 作  者 : Li_endi(24539)<li_endi@dahuatech.com>
 * 版  本 : V1.0.0
 * 日  期 : 2015年5月27日 Create
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

#ifndef	BLL_SOUND_H_
#define	BLL_SOUND_H_

/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */

#include "app_config.h"


#define VOICE_CFG_SILENT 0
#define VOICE_CFG_TONE   1
#define VOICE_CFG_VOICE  2

/* ========================================================================== */
/*                           宏和类型定义区                                   */
/* ========================================================================== */


/* ========================================================================== */
/*                          数据结构定义区                                    */
/* ========================================================================== */

/*声码器状态定义*/
typedef enum{
	ALLCLOSE=0,		/* 编解码全关*/
	ENCODEOPEN,	    /* 编码*/
	DECODEOPEN,	    /* 解码*/
	ALLOPEN,		/* 编解码全开*/
}ENUM_CODEC_STAT;


/*声音大小*/
typedef enum{
	SOUND_LVL_KEY = 0,		/* 按键音大小*/
	SOUND_LVL_PWR,	        /* 电量大小*/
	SOUND_LVL_MSG,	        /* 短信大小*/
	SOUND_LVL_SINGLE,		/* 个呼大小*/
	SOUND_LVL_GROUP,		/* 组呼大小*/
	SOUND_LVL_CALLEND,		/* 呼叫结束提示音大小*/
}ENUM_SOUND_LVL;

typedef enum{
	SOUNDSTAT_1 = 1,
	SOUNDSTAT_2,
	SOUNDSTAT_3,
	SOUNDSTAT_4,
	SOUNDSTAT_5,
	/* 短信提示音为此 */
	SOUNDSTAT_6,
	SOUNDSTAT_7,
	SOUNDSTAT_8,
	SOUNDSTAT_9,
	SOUNDSTAT_10,

	SOUNDSTAT_PCM,
	SOUNDSTAT_PowerOn,
	SOUNDSTAT_KeySound,
	SOUNDSTAT_Edited,
	SOUNDSTAT_Alarm,		/* 呼叫超时报警声 */
	SOUNDSTAT_Alarm_Dudu,		/* 呼叫超时报警声 */
	SOUNDSTAT_AlarmD1,
	SOUNDSTAT_AlarmD2,
	SOUNDSTAT_Voice,
	SOUNDSTAT_SendVoice,
	SOUNDSTAT_KeyLongSound,
	SOUNDSTAT_SendRefuseVoice,
	SOUNDSTAT_COLOU,		/* 电量报警声 */

}ENUM_SOUNDSTAT;


typedef enum{
SOUNDSTAT_InterohoneOpenVoice = 1,
SOUNDSTAT_InterohoneCloseVoice,
SOUNDSTAT_ChanleNumberVoice,
SOUNDSTAT_BSChanleNumberVoice,
SOUNDSTAT_LookInfmVoice,
SOUNDSTAT_UnProgrammedChanleNumberVoice,
SOUNDSTAT_UnProgrammedChanleZoneVoice,
SOUNDSTAT_ZoneVoice,
SOUNDSTAT_CouloStatVoice,
SOUNDSTAT_LowCouloVoice,
SOUNDSTAT_PowerVoice,
SOUNDSTAT_PCtoMCUStartVoice,
SOUNDSTAT_PCtoMCUEndVoice,
SOUNDSTAT_ScanStartVoice,
SOUNDSTAT_ScanEndVoice,
SOUNDSTAT_LockVoice,		
SOUNDSTAT_UnLockVoice,
SOUNDSTAT_RedAlarmVoice,
SOUNDSTAT_RedAlarmStpVoice,
SOUNDSTAT_GeHuVoice,
SOUNDSTAT_GeHuEndVoice,
SOUNDSTAT_ZuHuEndVoice,
SOUNDSTAT_ZuHuVoice,
SOUNDSTAT_QuanHuVoice,
SOUNDSTAT_TxTimeOutVoice,
SOUNDSTAT_BsTxTimeOutVoice,		
SOUNDSTAT_EncrptOpenVoice,
SOUNDSTAT_EncrptCloseVoice,
SOUNDSTAT_SKOpenVoice,
SOUNDSTAT_SKCloseVoice,
SOUNDSTAT_VolumeGradeVoice,
SOUNDSTAT_SQGradeVoice,
SOUNDSTAT_ScanCallInStart,/*扫到信道报*/
SOUNDSTAT_ScanMode,/*扫描模式*/
SOUNDSTAT_NumberVoice,


}ENUM_SOUNDSTATAUDIO;

/*无频语音*/
//enum{
//	/*
//	 * 开启对讲机	    Addrs+(0)	语音：2 提示音 ：1
//	 关闭对讲机	    Addrs+1	            语音：2 提示音 ：1
//	 选择频道	    Addrs+2	            语音：2 提示音 ：1
//	 电量查询	    Addrs+3	            语音：2 提示音 ：1
//	 低电量提示	    Addrs+4	            语音：2 提示音 ：1
//	 功率设置	    Addrs+5	            语音：2 提示音 ：1
//	 静噪级别设置	Addrs+6	            语音：2 提示音 ：1
//	 扫描设置	    Addrs+7	            语音：2 提示音 ：1
//	 当前频道状态	Addrs+8	            语音：2 提示音 ：1
//	 加锁	        Addrs+9	            语音：2 提示音 ：1
//	 解频	        Addrs+10	语音：2 提示音 ：1
//	 报警	        Addrs+11	语音：2 提示音 ：1
//	 个呼	        Addrs+12	语音：2 提示音 ：1
//	 组呼	        Addrs+13	语音：2 提示音 ：1
//	 全呼	        Addrs+14	语音：2 提示音 ：1
//	 TOT	    Addrs+15	语音：2 提示音 ：1
//	 中继激活超时	Addrs+16	语音：2 提示音 ：1
//	 加密开	    Addrs+17	语音：2 提示音 ：1
//	 加密关	    Addrs+18	语音：2 提示音 ：1
//	 声控开	    Addrs+19	语音：2 提示音 ：1
//	 声控关	    Addrs+20	语音：2 提示音 ：1
//	 区域切换	    Addrs+21	语音：2 提示音 ：1
//	 提示音音量   	Addrs+22	语音：2 提示音 ：1
//	 报号音音量   	Addrs+23	音量:1-13
//	 保留	        Addrs+(24-127)	语音：2 提示音 ：1	*/
//	SOUNDSTAT_InterohoneOpenVoice = 0,/*开机*/
//	SOUNDSTAT_InterohoneCloseVoice,/*关机*/
//	SOUNDSTAT_ChnNumberVoice,/*信道切换*/
//	SOUNDSTAT_CouloStatVoice,/*电量查询*/
//	SOUNDSTAT_LowCouloVoice,/*低电量提示*/
//
//	SOUNDSTAT_PowerVoice,     /*功率设置*/               /*功率高低*/
//	SOUNDSTAT_SQGradeVoice,   /*静噪级别*/
//	SOUNDSTAT_ScanStartVoice,      /*扫描开*/
//	SOUNDSTAT_ScanEndVoice,    /*扫描关*/
//	SOUNDSTAT_LookInfmVoice,   /*当前频道状态*/
//
//	SOUNDSTAT_LockVoice,
//	SOUNDSTAT_UnLockVoice,
//	SOUNDSTAT_RedAlarmVoice,
//	SOUNDSTAT_RedAlarmStpVoice,
//
//	SOUNDSTAT_Handup,/*呼叫挂起*/
//
//	SOUNDSTAT_GeHuVoice,
//	SOUNDSTAT_GeHuEndVoice,
//	SOUNDSTAT_ZuHuVoice,
//	SOUNDSTAT_ZuHuEndVoice,
//
//	SOUNDSTAT_QuanHuVoice,
//
//	SOUNDSTAT_TxTimeOutVoice,
//	SOUNDSTAT_BsTxTimeOutVoice,
//	SOUNDSTAT_EncrptOpenVoice,
//	SOUNDSTAT_EncrptCloseVoice,
//	SOUNDSTAT_SKOpenVoice,
//	SOUNDSTAT_SKCloseVoice,
//
//	SOUNDSTAT_ZoneVoice,
//	SOUNDSTAT_ScanCallInStart,
//
//	SOUNDSTAT_VolumeGradeVoice,
//	SOUNDSTAT_VolumeGradeVoice2,
//	SOUNDSTAT_ScanMode,
//
//	SOUNDSTAT_Revive, //激活
//	SOUNDSTAT_Monitor,//监听
//	SOUNDSTAT_Kill,//kill
//	SOUNDSTAT_Detection,//chk
//	SOUNDSTAT_Prompt,/*呼叫提示*/
//
//	SOUNDSTAT_Monitor_A1,/*模拟监听1*/
//	SOUNDSTAT_Monitor_A2,/*模拟监听2*/
//
//	SOUNDSTAT_SetOn,/**/
//	SOUNDSTAT_SetOff,/**/
//
//	SOUNDSTAT_RoamConnected,/**/
//
//	SOUNDSTAT_DTMF,/**/
//
//	SOUNDSTAT_BSChnNumberVoice,
//	SOUNDSTAT_UnProgrammedChnNumberVoice,
//	SOUNDSTAT_UnProgrammedChnZoneVoice,
//	SOUNDSTAT_PCtoMCUStartVoice,
//	SOUNDSTAT_PCtoMCUEndVoice,
//	SOUNDSTAT_PwrOff,
//	SOUNDSTAT_VoiceMax,
//};

//extern OS_SEM CodecLock;
extern DH_U8 gVoiceSetting[];
extern DH_U8 g_ToneVoiceVolume;
/* ========================================================================== */
/*                          函数声明区                                        */
/* ========================================================================== */

DH_VOID SOUND_BLL_SoundInit(DH_VOID);
DH_VOID SOUND_BLL_SoundTask(void *p_arg);
DH_VOID SOUND_BLL_SoundStart(DH_U8 Stat);
DH_VOID SOUND_BLL_SoundStartOpt(DH_U8 Stat, DH_U8 Exu);
DH_VOID SOUND_BLL_SoundExe(DH_U8 Stat,DH_U8 StatExu);
DH_VOID SOUND_bll_freq_thrext (DH_U16 STD_delay, DH_U16 Step_1, DH_U16 Step_2,DH_U16 Step_3,DH_U16 Step_4,DH_U8 Rshift_1,DH_U8 Rshift_2,DH_U8 Rshift_3,DH_U8 Rshift_4,DH_U8 level);
DH_VOID SOUND_bll_freq_thr (DH_U16 STD_delay, DH_U16 Step_1, DH_U16 Step_2,DH_U16 Step_3,DH_U8 Rshift_1,DH_U8 Rshift_2,DH_U8 Rshift_3,DH_U8 Step_amp);
DH_VOID SOUND_BLL_KeyVoice(DH_VOID);
DH_VOID SOUND_bll_freq_I2S(DH_U16 STD_delay, DH_U16 Step_1, DH_U16 Step_2,DH_U16 Step_3,DH_U16 Step_4,DH_U8 Rshift_1,DH_U8 Rshift_2,DH_U8 Rshift_3,DH_U8 Rshift_4);
DH_VOID SOUND_bll_freq_I2S_PlayPCM(DH_U8 *pcm, DH_U32 len);

DH_VOID SOUND_bll_WaitAudio (DH_U16 timeout);
DH_VOID HRC_BLL_PowerOnSound1(DH_VOID);
DH_VOID SOUND_bll_PowerOffSound (DH_VOID);
DH_VOID SOUND_bll_freq_end (DH_VOID);

DH_VOID SOUND_BLL_Tone(DH_U8 stat);
DH_VOID SOUND_BLL_VoiceIC(DH_U8 stat);

DH_VOID SOUND_BLL_Play(DH_U8 Num);
DH_VOID SOUND_BLL_PlayTone(DH_U8 *shift,DH_U8 len);
DH_VOID SOUND_BLL_Audio_CHN_Err(DH_VOID);
DH_VOID SOUND_BLL_Audio_CHN_Chg(DH_U8 opt,DH_U8 dat);
DH_U8 SOUND_BLL_StartCallTonePlay(DH_U8 u8Gi,DH_U8 call);
void Sound_Alarm(void);


#endif


