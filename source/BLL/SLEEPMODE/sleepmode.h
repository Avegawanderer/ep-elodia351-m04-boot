

#ifndef _APP_SLEEP_MODE_H
#define _APP_SLEEP_MODE_H

#define SLPMODECNT_MAX 10

#define POWERMODE_NONE 0 /*NONE*/
#define POWERMODE_IDLE 1
#define POWERMODE_SLP  2

#define PWMD_NONE      0
#define PWMD_RTC       1
#define PWMD_C5000CHK  2
#define PWMD_CTCD      4

#define SLPSTAT_SLP    1
#define SLPSTAT_NSLP   2

#define PM_LED_EN      1
#define PM_LED_DIS     0

typedef struct
{
    uint8 ModeMStat;
    uint8 ModeChckStat;
    uint8 SlpStat;
    uint8 LedStat;
    uint8 Cnt;
    uint8 LedCnt;
    uint8 SlpModeTime;
    uint8 LedModeTime;

} STR_SLPMODE_;
extern STR_SLPMODE_ Str_SlpMode;

//extern osSemaphoreId Sem_SlpLockId;

#define SEM_SLPLOCK      //OSSemPend(Sem_SlpLockId,0,&err)
#define SEM_SLPUNLOCK    //OSSemPost(Sem_SlpLockId);

void SlpModeInit(void);
void SlpMode1SSet(void);
void SlpModeIdle(void);
void SlpModeExitSM(void);
void SlpModeFRtcExitSM(void);
uint8 SlpModeGetStat(void);
void SlpModeSetStat(uint8 Stat);
void APP_SMModeChckSet(uint8 Flg);
void APP_SMModeChckClr(uint8 Flg);

void WakeUpFromDeepSleepOperation(void);

#endif
