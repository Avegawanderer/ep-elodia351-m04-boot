
/*****************************************************************************
 *   文件名:Rda1846App.h
 *	 文件描述:none；
 *   创建人：陶林，2009/12/31
******************************************************************************/
#ifndef _RDA1846_APP_H_
#define	_RDA1846_APP_H_

#include "dh_type.h"



#define NEW_DECODE 1

typedef struct{
	volatile DH_U8 Step;
	DH_U16 CtcCnt1;
	DH_U16 CtcCnt2;
	DH_U16 CtcCnt3;
	DH_U16 CtcCnt4;
	DH_U16 CtcCnt5;
	DH_U16 CtcCnt6;
}CTC_STR;

#define RDA_RX_CTCSS_OFF_DELAY		3000 // 9000//

extern DH_U8 SubCodeFlg;
extern CTC_STR Ctc_Str;
//extern DH_U16 g_sql_h,g_sql_l;
//extern DH_U16 SQLIntensBuf[10];
/*by xiao niu*/
extern DH_U8 g_msenden;		/*模拟接收使能*/
extern DH_U8 g_SqChkFlg;
//extern DH_U8 g_msendlimit;	/*模拟发送限时*/
//extern DH_U8 CloseCntCtcss;

//extern DH_U16 g_sqrssi;
extern DH_U8 CheckctdcsFlg;
extern DH_U8 sq_Tail;
extern DH_U8 analog_on,analog_off;
extern const DH_U16 CtcssTones[];



DH_VOID FM_APP_ICInit(DH_VOID);
DH_U8 BitCountExt(DH_U8 n);
DH_U8 FM_APP_CTCSSCodeMatching(DH_U16 Cnt1, DH_U16 Cnt2);
DH_VOID FM_APP_RecTask(DH_VOID* pdata);
DH_VOID sortI(DH_U16 *cal_ad,unsigned int n);
DH_VOID FM_APP_StartReceiveCtdcss(DH_VOID);
DH_VOID FM_APP_EndReceiveCtdcss(DH_VOID );
DH_VOID FM_APP_FmVoiceStart(DH_VOID);
DH_VOID FM_APP_FmVoiceStop(DH_VOID);
DH_VOID FM_APP_ForceFmVoiceStop(DH_VOID);
DH_VOID FM_APP_CTCSSDecode(DH_U8 Step,DH_U32);
DH_VOID FM_APP_CTCSS_Int(DH_VOID);
DH_VOID FM_APP_CTCSSOnOff(DH_U8 on,DH_U8 off);
DH_VOID FM_APP_FmMontStart(DH_VOID);
DH_VOID FM_APP_FmMontStop(DH_VOID);
DH_VOID FM_INTF_CTCSSInit(DH_U8 Attr);
DH_VOID FM_INTF_ParamInit(DH_VOID);
DH_VOID FM_APP_TaskInit(DH_VOID);

DH_VOID FM_INTF_VolSet(DH_VOID);
DH_U8 FM_APP_GetSqStatus(void);
DH_VOID FM_INTF_31136SetWN(DH_U8 wn);

DH_VOID FM_APP_OpenAudio(DH_VOID);
DH_VOID FM_APP_CloseAudio(DH_VOID);

DH_VOID FM_APP_IC_GpioSet(DH_U8 gpio,DH_U8 stat);
DH_VOID FM_APP_SetRxInit(DH_U32 freq);
DH_VOID FM_APP_IC_PowerDown(DH_VOID);
DH_VOID FM_APP_IC_PowerOn(DH_VOID);
DH_VOID FM_APP_OpenCtcDcsTimer(DH_VOID);
DH_VOID FM_APP_StopCtcDcsTimer(DH_VOID);

DH_VOID FM_APP_ExitSleep(DH_VOID);

DH_VOID FM_APP_SetTail(DH_VOID);
DH_VOID FM_APP_SetTxSubVoice(DH_VOID);

DH_VOID FM_APP_SetRxSubVoice(DH_VOID);



#endif
