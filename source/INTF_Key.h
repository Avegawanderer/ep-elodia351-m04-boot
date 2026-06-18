/*****************************************************************************
 *   文件名:key.h
 *	 文件描述:none；
 *   创建人：陶林，2009/10/13
******************************************************************************/
#ifndef _KEY_H
#define	_KEY_H


/* KEYOUT port, pin definition */
#define  KEYOUT_PORT        (PortB)
#define  KEYOUT0_PIN        (Pin07)
//#define  KEYOUT1_PIN        (Pin05)
//#define  KEYOUT2_PIN        (Pin06)

/* KEYIN port, pin definition */
#define  KEYIN_PORT         (PortB)
#define  KEYIN0_PIN         (Pin05)
#define  KEYIN1_PIN         (Pin06)
//#define  KEYIN2_PIN         (Pin07)


#define  KEYPTT_PORT         (PortA)
#define  KEYPTT_PIN          (Pin12)

//PB12		PTT	防水耳机端口独立PTT	L有效	
#define  KEYPTT_E_PORT         (PortB)
#define  KEYPTT_E_PIN          (Pin12)



#define KEY_PRESSING    Reset
#define KEY_RELEASE     Set

#define KEY_CRED 1		/* 可靠按键 */
#define KEY_UNCRED 2	/* 不可靠按键 */

#define CREDFLG_EN 1
#define CREDFLG_DIS 0

/* 按键值不能为0 ，否则无法得到按键值 */

enum{
 Key_None ,
 Key_Ptt ,
 Key_P1 ,
 Key_P2 ,
 Key_P3 ,
 Key_P4 ,
 Key_LongHnd  ,


 Key_Call,
 Key_CallUp ,
	Key_Ref,
	Key_SimuBack,
	ENM_Key_Max
};


typedef enum{
EE_KeyNoHnd=0, //未定义
EE_KeyPower,
EE_KeyLedSw, //2背景灯开关
EE_KeyLock, 
EE_KeyVox, 
EE_KeyZoneChange, //5区域切换
EE_KeyScanSw, 
EE_KeyScanModeChg,
EE_KeyRelay, 
EE_KeyExiOpen,
EE_KeyEncryp, 
EE_KeyLM, //6-11
EE_KeyNote,
EE_KeyDmrAct, 
EE_KeyDmrChck, 
EE_KeyDmrRadioDisale,
EE_KeyListen, //12-16
EE_KeyMonitor,
EE_KeyPermanentMonitor, 
EE_key_DispMode,//EE_key_1750tone,
EE_InformationSearch,//20信息查询
EE_KeyADSW, //21数模切换
EE_KeySQL, 
EE_Key1L, 
EE_Key2L, 
EE_Key3L, 
EE_KeyLight,//26手电筒
EE_Key4L,
EE_Key5L,
EE_Key6L, 
EE_KeyZoneSet, 
EE_KeyUselessChannel, //27-31
EE_KeyASensorScan, 
EE_KeyAdel,//32-33
}ENM_KEY_ZDY;//上位机所有自定义按键的功能列表





/* 按键中断类型 */
#define KEY_INTMODE_PTT 1
#define KEY_INTMODE_USER1 2
#define KEY_INTMODE_USER2 3
#define KEY_INTMODE_DATA 4
#define KEY_INTMODE_INT1 5
#define KEY_INTMODE_INT2 6
#define KEY_INTMODE_INT3 7
#define KEY_INTMODE_INT4 8

/* 按键的状态: */
/* bit0: ptt0,bit1:use1,bit2:use2 */
#define KEYSTAT_PTT 1
#define KEYSTAT_USER1 2
#define KEYSTAT_USER2 4

typedef struct {
uint8 CurStat;	/* 当前按键状态; */
uint8 LastStat;	/* 上一次按键状态 */
uint8 IntMode;	/* 响应中断类型 */
uint32 UartRecTime;	
}STR_KEY;

/* 按键音执行状态 */
#define KEYVOC_HND_Y 1
#define KEYVOC_HND_N 2

typedef struct{
uint8 HndStat;	/* 是否执行标记 */
}STR_KEYVOC;

typedef struct {
uint8 Cnt;	/*延时计数器5最大递减0关闭*/
uint8 Stat;/*键盘背光灯状态,EN/DIS */
}KEYPAD_LIGHT;
#define KEYPAD_LIGHT_ENABLE	1
#define KEYPAD_LIGHT_DISABLE	0
#define KEYPAD_LIGHT_CNT_MAX	5//最大值5秒延时.
extern KEYPAD_LIGHT KeyPadLightSet;
extern STR_KEYVOC Str_KeyVoc;
//监听0x11
//永久监听0X12
//Tone700=0X1C
//Tone1000=0X1D
//Tone1200=0X1E
//Tone1400=0X1F
#define FUN_MONITOR		0X11
#define FUN_PMONITOR	0X12
#define FUN_TONE700		0X1C
#define FUN_TONE1000		0X1D
#define FUN_TONE1200		0X1E
#define FUN_TONE1400		0X1F
//		0x5A==立即执行短按键功能
//		0xAA==立即执行长按键功能
//		其他===不要执行此功能,
#define FUN_LONGKEY		0XAA
#define FUN_SHORTKEY		0X5A
#define FUN_NONE			0


//extern uint8 KeyCurrentStat;
extern STR_KEY Str_Key;
extern uint8 g_ConVolume;
void KeyInit(void);
void KeyIrq(void);
void KeyHandle (void);
uint32 KeyGetOpt(uint32 TimeOut);
void KeyPutOpt(uint8 Flg, uint32 Stat);
void KeyVoice(void);
uint8 KeyStatChk(void);
void KeyIntHnd(uint8 flg);
void KeyVoiceStart(uint8 Stat);

void KeyVoiceEnd(void);
void ConVlumeAdd(void);
void ConVlumeDec(void);
void INTF_KeyPttUartSet(void);
void keypad_pn_setalllow(void);

uint8 GetKeypad(void);

uint8 ChkHasKeypadPress(void);
#endif
