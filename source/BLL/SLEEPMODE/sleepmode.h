/*****************************************************************************
 *   文件名:sleepmode.h
 *	 文件描述:none；
 *   创建人：陶林，2010/08/17
******************************************************************************/
#ifndef _APP_SLEEP_MODE_H
#define	_APP_SLEEP_MODE_H

/* 进入睡眠模式时间(s) */
#define SLPMODECNT_MAX 10

#define POWERMODE_NONE 0  /*NONE*/
#define POWERMODE_IDLE 1  
#define POWERMODE_SLP 2 

/* 2 级功耗模式 */
#define PWMD_NONE 0	
#define PWMD_RTC 1	/* rtc 唤醒后模式 */
#define PWMD_C5000CHK 2	/* WUUP (C5000CHK) 唤醒后模式 */
#define PWMD_CTCD 4	/* WUUP (C5000CHK) 唤醒后模式 */

#define SLPSTAT_SLP 1
#define SLPSTAT_NSLP 2

/* lcd 状态设置 */
#define PM_LED_EN 1
#define PM_LED_DIS 0


/*低功耗模块的结构体*/
typedef  struct {
uint8 ModeMStat;	/* 状态 */
uint8 ModeChckStat;	/*2 级功耗模式 状态 */
uint8 SlpStat;
uint8 LedStat;
uint8 Cnt;	/* 延时cnt 进入sleep 模式 */
uint8 LedCnt;	/* 延时cnt 进入sleep 模式 */
uint8 SlpModeTime;
uint8 LedModeTime;

}STR_SLPMODE_;
extern STR_SLPMODE_ Str_SlpMode;

//extern uint8 g_checksignal;//用于省电


//extern osSemaphoreId Sem_SlpLockId;

#define SEM_SLPLOCK       //OSSemPend(Sem_SlpLockId,0,&err)
#define SEM_SLPUNLOCK     //OSSemPost(Sem_SlpLockId);


void SlpModeInit(void) ;
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
