

#ifndef _BLL_CODESW_H_
#define _BLL_CODESW_H_

/* ========================================================================== */

/* ========================================================================== */
#include "dh_type.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

    /* ========================================================================== */

    /* ========================================================================== */
    //extern OS_SEM Chn_Sem;
    //extern OS_SEM ChnDisp_Sem;

    extern DH_U8 g_DFMRecStat;

#define CHN_INTF_LOCK
#define CHN_INTF_UNLOCK    // dhsem_post(&Chn_Sem)

#define AREA_MAX        1
#define CHN_MAX         16

#define SUBAUDIO_TX     1
#define SUBAUDIO_RX     2

#define FMFUNC_BUSY_ON  0
#define FMFUNC_BUSY_OFF 1

#define BW_12_5K        0
#define BW_20K          2
#define BW_25K          1

    /* ========================================================================== */

    /* ========================================================================== */

    //00    25    00   40    00   25  00   41   9E   02   13  28  E6  BF  FF  1F

    //
    //

    //1	TOT	272	0-40	15s

    //0.5 s=1
    //1 s= 2
    //1.5 s= 3
    //2 s=4
    //3 s=5

    //5700Hz
    //6 1000 Hz
    //71400 Hz
    //8 1450 Hz
    //92100 Hz

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

    typedef struct
    {

        DH_U32 u32RxFreq;

        DH_U32 u32TxFreq;

        DH_U16 u16RxSubVoice;
        DH_U16 u16TxSubVoice;

        struct
        {
            DH_U8 rever : 4;

            DH_U8 busyflag : 1;
            DH_U8 rever5 : 1;

            DH_U8 scanflg : 1;
            DH_U8 rever7 : 1;
        };

        struct
        {
            DH_U8 reverx : 4;
            DH_U8 band : 1;
            DH_U8 power : 1;
            DH_U8 reverx7 : 1;
        };

        DH_U8 u8RxSubVoiceIndex;
        DH_U8 u8TxSubVoiceIndex;
    } CHN_INFO_S;

    typedef struct
    {
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
    } FUNC_INFO_S;

    extern FUNC_INFO_S Func_Str;

    /* ========================================================================== */

    /* ========================================================================== */
    DH_U8 CODESW_BLL_Init(DH_VOID);
    DH_VOID CODESW_BLL_Process(void *p_arg);
    DH_VOID CHN_BLL_Set(DH_U8 u8Chn);
    DH_VOID CHN_BLL_Get(DH_U16 u16Area, DH_U8 u8Chn, CHN_INFO_S *str);
    DH_VOID CHN_BLL_InfoSet(CHN_INFO_S *pstr, DH_U8 *buf);

    CHN_INFO_S CHN_BLL_GetStr(DH_VOID);
    DH_VOID CHN_BLL_SetStr(CHN_INFO_S str);
    DH_VOID CHN_BLL_SetDFMAttr(DH_U8 stat);
    DH_VOID CHN_BLL_SetFM_Subvoice(DH_U8 rxtype, DH_U8 txtype, DH_U8 rxSub, DH_U8 txSub);

    DH_VOID CHN_BLL_SetAttr(DH_U8 stat);
    DH_VOID CHN_BLL_SetChnName(DH_U8 *s);
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

    DH_U8 CHN_BLL_GetAttr(DH_VOID);
    DH_U8 CHN_BLL_GetDFMAttr(DH_VOID);
    DH_U8 *CHN_BLL_GetChnName(DH_VOID);
    //DMRCHN_INFO_S   CHN_BLL_GetDMRChnInfo(DH_VOID);
    //DMRRXCHN_INFO_S CHN_BLL_GetDMRRxChnInfo(DH_VOID);
    //DMRTXCHN_INFO_S CHN_BLL_GetDMRTxChnInfo(DH_VOID);
    //FMCHN_INFO_S    CHN_BLL_GetFMChnInfo(DH_VOID);
    //FMRXCHN_INFO_S  CHN_BLL_GetFMRxChnInfo(DH_VOID);
    //FMTXCHN_INFO_S  CHN_BLL_GetFMTxChnInfo(DH_VOID);
    DH_U32 *CHN_BLL_GetGroupNumList(DH_VOID);
    DH_U8 *CHN_BLL_GetPassWord(DH_VOID);
    DH_U32 CHN_BLL_GetLinkMan(DH_VOID);
    DH_U8 CHN_BLL_GetLinkManAttr(DH_VOID);
    DH_U8 CHN_BLL_GetLinkManIndex(DH_VOID);

    DH_U32 CHN_BLL_GetLocalID(DH_VOID);

    DH_U8 CHN_BLL_GetDefaultSTAT(DH_U16 u16Area, DH_U8 u8Chn);

    DH_VOID CHN_BLL_SetDFMRecStat(DH_U8 stat, DH_U8 recstat);

    DH_BOOL CHN_BLL_KeyMsgFilter(DH_U8 Msg);

    DH_U8 CHN_BLL_GetFMBandWide(DH_VOID);

    CHN_INFO_S *CHN_BLL_GetChnInfoP(DH_VOID);

    DH_U8 CHN_BLL_GetFMRxSubVoiceType(DH_VOID);
    DH_U8 CHN_BLL_GetFMTxSubVoiceType(DH_VOID);

    DH_VOID CHN_BLL_DFMTxCallSet(DH_VOID);

    DH_VOID CHN_BLL_SetCallManIndex(DH_U8 ind);
    DH_U8 CHN_BLL_GetCallManIndex(DH_VOID);
    DH_VOID CHN_BLL_GetLMName(DH_U8 *namebuf);

    DH_VOID CHN_BLL_GetLoFreq(DH_U32 *u32Freq);

    DH_VOID CHN_BLL_SetCurZoneChn(DH_U8 zone, DH_U8 chn);
    DH_U8 CHN_BLL_GetCurZoneNum(DH_VOID);
    DH_U8 CHN_BLL_GetCurChnNum(DH_VOID);

    uint8 CHN_BLL_PowerGet(void);

    void CHN_BLL_PowerSet(uint8 power);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
