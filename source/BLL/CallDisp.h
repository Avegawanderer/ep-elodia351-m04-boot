/*****************************************************************************
 *   文件名:CallDisp.h
 *	 文件描述:none；
 *   创建人：陶林，2010/11/20
******************************************************************************/
#ifndef _CALL_DISP_H
#define	_CALL_DISP_H


/* 此结构体是否有效 */
#define CALLSTR_USED 1
#define CALLSTR_UNUSED 0

/* 呼入或呼出 */
#define CALLSTR_CALLOUT 1
#define CALLSTR_CALLIN 2

#ifndef DEF_CALLSTR

#define DEF_CALLSTR

/* 标志位: 已经调用延时执行stop，但还未执行stop 函数 */
#define DLYSTOPFLG_TRUE 1
#define DLYSTOPFLG_FALSE 0



/* 通话界面显示的结构体  */
typedef struct {
//	uint8 UsedFlg;	/* 此结构体是否有效 */
//	uint8 Mode;		/* 呼出0r 呼入 */
//	uint8 Mode1_i;
//	uint8 Mode2_i;
//	uint8 DlyStopFlg;	/* 标志位: DLYSTOPFLG_TRUE : 已经调用延时执行stop，但还未执行stop 函数 */
	uint16 N_100Ms;	/* 通话时间n(100ms) */
//	uint32 Num;		/* 对方号码 */
	//uint8 NumBuf[LM_FORMAT_NAME_LEN];	/* 存储通话显示的姓名或号码的缓存 */
//	TIMESTR CTime;	/* 开始通话时间 */
//	uint8 HadNetFlag;//是否已经接通的标志,1=已接通,2=未接
}CALLSTR;

#endif
//extern OS_TMR* PTMR_RefCall; 
extern CALLSTR CallStr;
extern CALLSTR CallStrLast;	/* CallStr 的保存值 */
extern uint8 CallTimeout_flag;
extern uint8 g_overtimeforbid;
extern uint8 ttp;/*可以防一下通话界面一直关不掉后通过旋转信道来清除*/


void CallDispInit(void);
void CallRefHnd(void);
void CallStop(void* Mode);
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
