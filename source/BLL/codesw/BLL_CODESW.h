/*******************************************************************************
 * Copyright (C) 2011-2013 ZheJiang Dahua Technology CO.,LTD.
 *
 * 文件名 : BLL_CODESW.h
 *
 * 作  者 : 123(ggg)<gg>
 * 版  本 : V1.0.0
 * 日  期 : 2015年4月10日 Create
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



#ifndef _BLL_CODESW_H_
#define _BLL_CODESW_H_


/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include "dh_type.h" 



#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */


/* ========================================================================== */
/*                           宏和类型定义区                                   */
/* ========================================================================== */
//extern OS_SEM Chn_Sem;
//extern OS_SEM ChnDisp_Sem;

extern DH_U8 g_DFMRecStat;

#define CHN_INTF_LOCK      
#define CHN_INTF_UNLOCK   // dhsem_post(&Chn_Sem)

#define AREA_MAX           1
#define CHN_MAX            16

#define  SUBAUDIO_TX		1
#define  SUBAUDIO_RX		2

#define FMFUNC_BUSY_ON      0
#define FMFUNC_BUSY_OFF     1

#define BW_12_5K            0
#define BW_20K              2
#define BW_25K              1

/* ========================================================================== */
/*                          数据结构定义区                                    */
/* ========================================================================== */
//信道数据格式：
//00    25    00   40    00   25  00   41   9E   02   13  28  E6  BF  FF  1F
//项目	地址		描述
//接收频点	0-3	00    25    00   40	400.02500
//发射频点	4-7	00   25  00   41	410.02500
//接收哑音	8-9	9E   02	67.0 HZ   670=0X29E
//发射哑音	10-11	13   28	023N
//步骤1：oct（23）=19
//步骤2：10240(正向数字亚音频)+19=10259
//步骤3：10259 =OX2813
//43008 (反向向数字亚音频)
//繁忙禁发	12		bit4 = 0关，bit4= 1开
//扫描	12		Bit6 = 0关，bit6= 1开
//宽窄带	13	bit4	1=宽带  0=窄带
//高低功率		bit5 bit6	10=高  01=中00=低
//			
//
//可选功能：
//序号	项目	地址	取值范围	步进
//1	TOT	272	0-40	15s
//2	静噪等级	273	0-9	1s
//3	Voice	274	0-1	0关1开
//4	空	275		
//5	VOX等级	276	0-16	0关 1-16级
//6	VOX延时	277	0-5	0.3s= 0
//0.5 s=1
//1 s= 2
//1.5 s= 3
//2 s=4
//3 s=5
//7	省电模式	278	0-1	0关1开
//8	TONE延时	279	0-9	1-10S
//9	PF1	280	0-11	0关
//1瞬时监听
//2监听
//3扫描
//4报警
//5700Hz
//6 1000 Hz
//71400 Hz
//8 1450 Hz
//92100 Hz
//10高低功率
//11 2音
//10	PF2	281		
//11	PF3	282		
//12	TONE1	283	0-5
//	1200Hz=0
//1600 Hz= 1
//1700 Hz=2
//1800 Hz=3
//2200 Hz=4
//2300 Hz=5
//13	TONE2	284		
//14	TOT TONE	285	0-10	0关 1-10s
//15	TONE	286	0-1	0关1开



/* 信道结构体 */
typedef struct{
	/* 接收频率 */
	DH_U32 u32RxFreq ; 
		/* 发射频率 */
	DH_U32 u32TxFreq ;
	
	DH_U16 u16RxSubVoice;
	DH_U16 u16TxSubVoice;
	
	struct{
		DH_U8 rever : 4;
		/* 繁忙禁发*/
		DH_U8 busyflag : 1;
		DH_U8 rever5   : 1;
		/* 扫描*/ 
		DH_U8 scanflg : 1; 
		DH_U8 rever7   : 1;
	};


	struct{
		DH_U8 reverx : 4;		
		DH_U8 band : 1;
		DH_U8 power   : 1; 
		DH_U8 reverx7   : 1;
	};	
	
	DH_U8 u8RxSubVoiceIndex;
	DH_U8 u8TxSubVoiceIndex;
}CHN_INFO_S;


/* 功能结构体 */
typedef struct{
	DH_U8 tot;
	DH_U8 sqlevel;
	DH_U8 Voice;
	DH_U8 rever;
	DH_U8 voxlevel;
	DH_U8 voxdelay;
	DH_U8 PowerSwitch;
	DH_U8 tonedelay;
	DH_U8 pf1;
	DH_U8 pf2;
	DH_U8 pf3;
	DH_U8 tone1;
	DH_U8 tone2;
	DH_U8 tot_tone;
	DH_U8 tonesw;	
}FUNC_INFO_S;

extern FUNC_INFO_S Func_Str;

/* ========================================================================== */
/*                          函数声明区                                        */
/* ========================================================================== */
DH_U8   CODESW_BLL_Init(DH_VOID);
DH_VOID CODESW_BLL_Process(void *p_arg);
DH_VOID CHN_BLL_Set( DH_U8 u8Chn);
DH_VOID CHN_BLL_Get(DH_U16 u16Area, DH_U8 u8Chn, CHN_INFO_S *str);
DH_VOID CHN_BLL_InfoSet(CHN_INFO_S *pstr,DH_U8 *buf);

CHN_INFO_S CHN_BLL_GetStr(DH_VOID);
DH_VOID CHN_BLL_SetStr(CHN_INFO_S str);
DH_VOID CHN_BLL_SetDFMAttr(DH_U8 stat);
DH_VOID CHN_BLL_SetFM_Subvoice(DH_U8 rxtype,DH_U8 txtype,DH_U8 rxSub,DH_U8 txSub);

DH_VOID CHN_BLL_SetAttr(DH_U8 stat);
DH_VOID CHN_BLL_SetChnName(DH_U8* s);
//DH_VOID CHN_BLL_SetDMRChnInfo(DMRCHN_INFO_S str);
//DH_VOID CHN_BLL_SetDMRRxChnInfo(DMRRXCHN_INFO_S str);
//DH_VOID CHN_BLL_SetDMRTxChnInfo(DMRTXCHN_INFO_S str);
//DH_VOID CHN_BLL_SetFMChnInfo(FMCHN_INFO_S str);
//DH_VOID CHN_BLL_SetFMRxChnInfo(FMRXCHN_INFO_S str);
//DH_VOID CHN_BLL_SetFMTxChnInfo(FMTXCHN_INFO_S str);
DH_VOID CHN_BLL_SetGroupNumList(DH_U8 u8Num);
DH_VOID CHN_BLL_SetPassWord(DH_VOID);
DH_VOID CHN_BLL_SetLinkMan(DH_U8 u8Stat);
DH_VOID CHN_BLL_SetLocalID(DH_VOID);
DH_VOID CHN_BLL_ModifyLocalID(DH_U32 id);
DH_VOID CHN_BLL_SetEncryptReg(DH_VOID);

DH_VOID CHN_BLL_SetFMChnSpace(DH_U8 space);

DH_U8   CHN_BLL_GetAttr(DH_VOID);
DH_U8 CHN_BLL_GetDFMAttr(DH_VOID);
DH_U8*  CHN_BLL_GetChnName(DH_VOID);
//DMRCHN_INFO_S   CHN_BLL_GetDMRChnInfo(DH_VOID);
//DMRRXCHN_INFO_S CHN_BLL_GetDMRRxChnInfo(DH_VOID);
//DMRTXCHN_INFO_S CHN_BLL_GetDMRTxChnInfo(DH_VOID);
//FMCHN_INFO_S    CHN_BLL_GetFMChnInfo(DH_VOID);
//FMRXCHN_INFO_S  CHN_BLL_GetFMRxChnInfo(DH_VOID);
//FMTXCHN_INFO_S  CHN_BLL_GetFMTxChnInfo(DH_VOID);
DH_U32* CHN_BLL_GetGroupNumList(DH_VOID);
DH_U8*  CHN_BLL_GetPassWord(DH_VOID);
DH_U32  CHN_BLL_GetLinkMan(DH_VOID);
DH_U8  CHN_BLL_GetLinkManAttr(DH_VOID);
DH_U8  CHN_BLL_GetLinkManIndex(DH_VOID);

DH_U32  CHN_BLL_GetLocalID(DH_VOID);

DH_U8 CHN_BLL_GetDefaultSTAT(DH_U16 u16Area,DH_U8 u8Chn);

DH_VOID CHN_BLL_SetDFMRecStat(DH_U8 stat,DH_U8 recstat);

DH_BOOL CHN_BLL_KeyMsgFilter(DH_U8 Msg);

DH_U8 CHN_BLL_GetFMBandWide(DH_VOID);

CHN_INFO_S* CHN_BLL_GetChnInfoP(DH_VOID);

DH_U8 CHN_BLL_GetFMRxSubVoiceType(DH_VOID);
DH_U8 CHN_BLL_GetFMTxSubVoiceType(DH_VOID);

DH_VOID CHN_BLL_DFMTxCallSet(DH_VOID);

DH_VOID CHN_BLL_SetCallManIndex(DH_U8 ind);
DH_U8 CHN_BLL_GetCallManIndex(DH_VOID);
DH_VOID CHN_BLL_GetLMName(DH_U8 *namebuf);


DH_VOID CHN_BLL_GetLoFreq(DH_U32 *u32Freq);

DH_VOID CHN_BLL_SetCurZoneChn(DH_U8 zone,DH_U8 chn);
DH_U8 CHN_BLL_GetCurZoneNum(DH_VOID);
DH_U8 CHN_BLL_GetCurChnNum(DH_VOID);


uint8 CHN_BLL_PowerGet(void);

void CHN_BLL_PowerSet(uint8 power);



#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif

