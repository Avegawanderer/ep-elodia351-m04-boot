#ifndef _RDA1846SLEEP_H
#define _RDA1846SLEEP_H

#define SYSSLP_MODE_SLP    1
#define SYSSLP_MODE_CHCK   2
#define SYSSLP_MODE_RUN    3

#define SYSSLP_RDA1846SLP  1
#define SYSSLP_RDA1846NSLP 2

#define SYSSLP_CMD_OPEN    1
#define SYSSLP_CMD_CLOSE   0

typedef struct _STR_SYSTEMSLP_
{
    uint8 CSLP_Mode;
    uint8 CSLP_SystemSlpStat;
    uint8 CSLP_Cmd;
    uint8 CSLP_SlpCnt;
    uint8 CSLP_USlpCnt;
} Str_SYSSLP_;

extern Str_SYSSLP_ Str_SyatemSlp;
extern uint8 g_checksignal;
extern u8 IS_COM_Wake_Up;
extern uint8 RDA1846sPdnDown;
extern uint8 SleepBackupSQA;
extern uint8 SleepBackupSQB;

//extern osThreadId  ThreadId_TaskSystemSleep;

void TaskSystemSleepInit(void);

void TaskSystemSleepAct(void);

void APP_RDA1846SlpEn(void);
void APP_RDA1846SlpDis(void);
DH_BOOL APP_RDA1846SlpStatGet(void);
void Re_Enable_RTCclockAfterWakeUp(void);
extern uint32 SleepTimeCouter;

void sys_tick_exta(void);

#endif
