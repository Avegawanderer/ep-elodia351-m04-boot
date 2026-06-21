

#ifndef BLL_SOUND_H_
#define BLL_SOUND_H_

/* ========================================================================== */

/* ========================================================================== */

#include "app_config.h"

#define VOICE_CFG_SILENT 0
#define VOICE_CFG_TONE   1
#define VOICE_CFG_VOICE  2

/* ========================================================================== */

/* ========================================================================== */

/* ========================================================================== */

/* ========================================================================== */

typedef enum
{
    ALLCLOSE = 0,
    ENCODEOPEN,
    DECODEOPEN,
    ALLOPEN,
} ENUM_CODEC_STAT;

typedef enum
{
    SOUND_LVL_KEY = 0,
    SOUND_LVL_PWR,
    SOUND_LVL_MSG,
    SOUND_LVL_SINGLE,
    SOUND_LVL_GROUP,
    SOUND_LVL_CALLEND,
} ENUM_SOUND_LVL;

typedef enum
{
    SOUNDSTAT_1 = 1,
    SOUNDSTAT_2,
    SOUNDSTAT_3,
    SOUNDSTAT_4,
    SOUNDSTAT_5,

    SOUNDSTAT_6,
    SOUNDSTAT_7,
    SOUNDSTAT_8,
    SOUNDSTAT_9,
    SOUNDSTAT_10,

    SOUNDSTAT_PCM,
    SOUNDSTAT_PowerOn,
    SOUNDSTAT_KeySound,
    SOUNDSTAT_Edited,
    SOUNDSTAT_Alarm,
    SOUNDSTAT_Alarm_Dudu,
    SOUNDSTAT_AlarmD1,
    SOUNDSTAT_AlarmD2,
    SOUNDSTAT_Voice,
    SOUNDSTAT_SendVoice,
    SOUNDSTAT_KeyLongSound,
    SOUNDSTAT_SendRefuseVoice,
    SOUNDSTAT_COLOU,

} ENUM_SOUNDSTAT;

typedef enum
{
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
    SOUNDSTAT_ScanCallInStart,
    SOUNDSTAT_ScanMode,
    SOUNDSTAT_NumberVoice,

} ENUM_SOUNDSTATAUDIO;

//enum{
//	/*

//

//
//	SOUNDSTAT_LockVoice,
//	SOUNDSTAT_UnLockVoice,
//	SOUNDSTAT_RedAlarmVoice,
//	SOUNDSTAT_RedAlarmStpVoice,
//

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

//	SOUNDSTAT_Kill,//kill
//	SOUNDSTAT_Detection,//chk

//

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

/* ========================================================================== */

DH_VOID SOUND_BLL_SoundInit(DH_VOID);
DH_VOID SOUND_BLL_SoundTask(void *p_arg);
DH_VOID SOUND_BLL_SoundStart(DH_U8 Stat);
DH_VOID SOUND_BLL_SoundStartOpt(DH_U8 Stat, DH_U8 Exu);
DH_VOID SOUND_BLL_SoundExe(DH_U8 Stat, DH_U8 StatExu);
DH_VOID SOUND_bll_freq_thrext(DH_U16 STD_delay, DH_U16 Step_1, DH_U16 Step_2, DH_U16 Step_3, DH_U16 Step_4, DH_U8 Rshift_1, DH_U8 Rshift_2, DH_U8 Rshift_3, DH_U8 Rshift_4, DH_U8 level);
DH_VOID SOUND_bll_freq_thr(DH_U16 STD_delay, DH_U16 Step_1, DH_U16 Step_2, DH_U16 Step_3, DH_U8 Rshift_1, DH_U8 Rshift_2, DH_U8 Rshift_3, DH_U8 Step_amp);
DH_VOID SOUND_BLL_KeyVoice(DH_VOID);
DH_VOID SOUND_bll_freq_I2S(DH_U16 STD_delay, DH_U16 Step_1, DH_U16 Step_2, DH_U16 Step_3, DH_U16 Step_4, DH_U8 Rshift_1, DH_U8 Rshift_2, DH_U8 Rshift_3, DH_U8 Rshift_4);
DH_VOID SOUND_bll_freq_I2S_PlayPCM(DH_U8 *pcm, DH_U32 len);

DH_VOID SOUND_bll_WaitAudio(DH_U16 timeout);
DH_VOID HRC_BLL_PowerOnSound1(DH_VOID);
DH_VOID SOUND_bll_PowerOffSound(DH_VOID);
DH_VOID SOUND_bll_freq_end(DH_VOID);

DH_VOID SOUND_BLL_Tone(DH_U8 stat);
DH_VOID SOUND_BLL_VoiceIC(DH_U8 stat);

DH_VOID SOUND_BLL_Play(DH_U8 Num);
DH_VOID SOUND_BLL_PlayTone(DH_U8 *shift, DH_U8 len);
DH_VOID SOUND_BLL_Audio_CHN_Err(DH_VOID);
DH_VOID SOUND_BLL_Audio_CHN_Chg(DH_U8 opt, DH_U8 dat);
DH_U8 SOUND_BLL_StartCallTonePlay(DH_U8 u8Gi, DH_U8 call);
void Sound_Alarm(void);

#endif
