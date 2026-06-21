

#ifndef _CALL_DISP_H
#define _CALL_DISP_H

#define CALLSTR_USED    1
#define CALLSTR_UNUSED  0

#define CALLSTR_CALLOUT 1
#define CALLSTR_CALLIN  2

#ifndef DEF_CALLSTR

#define DEF_CALLSTR

#define DLYSTOPFLG_TRUE  1
#define DLYSTOPFLG_FALSE 0

typedef struct
{

    //	uint8 Mode1_i;
    //	uint8 Mode2_i;

    uint16 N_100Ms;

} CALLSTR;

#endif
//extern OS_TMR* PTMR_RefCall;
extern CALLSTR CallStr;
extern CALLSTR CallStrLast;
extern uint8 CallTimeout_flag;
extern uint8 g_overtimeforbid;
extern uint8 ttp;

void CallDispInit(void);
void CallRefHnd(void);
void CallStop(void *Mode);
void CallOut_UpTimeSet(void);

DH_BOOL GetCallStat(void);
void CallDisp(void);
void CallOutStart(void);

uint8 DisableTxState(void);

uint8 FmRecBusyChk(void);

void CallDisp_FMDisp(void);

DH_VOID FM_INTF_GenerateTone(int tonefreq);

void Call_ToneTx(void);
void Call_ToneTxStart(int freq);
void Call_ToneTxStop(void);

#endif
