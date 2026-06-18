/*****************************************************************************
 *   文件名:menu.h
 *	 文件描述:none；
 *   创建人：陶林，2009/11/4
******************************************************************************/
#ifndef _MENU_H
#define	_MENU_H

/*菜单*/
#if FUNC_MENU_DISP == MENU_DISP_NONE
	#define MENU_STAT 0
#else
	#define MENU_STAT 1
#endif

#define Jump2Menu(MenuTopIndex,FlashMode_AutoInit) 


#define TASK_FLAG_VOICE      (1<<0)	/* 音量定时器任务 */
#define TASK_FLAG_100MS      (1<<1)	/* 刷新通话界面定时器任务 */
#define TASK_FLAG_VOX        (1<<2)	/* 延时执行定时器任务 */
#define TASK_FLAG_1S         (1<<3)	/* 长按键执行任务 */
#define TASK_FLAG_SLEEP      (1<<4)	/* 长按键执行任务 */
#define TASK_FLAG_KEYSCAN      (1<<5)
#define TASK_FLAG_FMREC      (1<<6)


#define LcdWriteLock()
#define		LcdWriteUnlock()



#define INTF_RFLock()     
#define INTF_RFUnlock()      


#define HRC_BLL_CloseAudio()    SPK_INTF_Close() 
#define HRC_BLL_OpenAudio()      SPK_INTF_Open() 

typedef enum{
	FM_SUBVOICETYPE_CARRIER = 1,
	FM_SUBVOICETYPE_CTCSS,
	FM_SUBVOICETYPE_CDCSS,
	FM_SUBVOICETYPE_CDCSSR,
} FM_SUBVOICETYPE;


#define  MenuTopLock()  //OSSemPend(Sem_MenuTopLock,0,&err)
#define  MenuTopUnLock()  //OSSemPost(Sem_MenuTopLock);

#define  MenuWriteLock()   
#define  MenuWriteUnlock()   


#define MenuSize 1          //菜单深度最大值

//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//如下定义从KeyTab中得到
//调整KeyTab时必须更新如下的定义
//否则菜单链接出错!
#define MenuTopIndex		0
#define Menu_1_Index		7
#define Menu_2_Index		14
#define Menu_3_Index		21
#define Menu_4_Index		28
#define Menu_5_Index		35
#define Menu_6_Index		42
#define Menu_7_Index		49
#define Menu_8_Index		56
#define Menu_9_Index		63

#define Menu_Vol_Index	70	/* 音量显示所在层数 */
#define Menu_Freq_Index       77    /*频率输入显示层*/

//不初始化Flash
#define FlashMode_NoAction     0
//系统系统负责初始化
#define FlashMode_AutoInit     1
//手动初始化
#define FlashMode_ManualInit   2
//重绘画面
#define FlashMode_ReFlash      3

//返回当前菜单
#define SelfMenuIndex g_FatherIndex[g_Layer-1]


//Bar型菜单的数据结构

struct Menu
{
    uint8 **Text;     //指向显示内容数组的指针
    uint8 TextNum;         //项个数
    uint8 BarType_Type;
};

//#define DispMax 5		/* 一个屏幕最多能显示的菜单条数 */
#define DispMax1 4
#define ContentSize 3		/* 显示(短信)最多需要的几屏 */

#define NoteUnRead 0		//未读标志
#define NoteReaded 1		//已读标志

#define BARTYPENONE 0
#define BARTYPENOTELIST 1		//短信列表
#define BARTYPEMENU 2			//一般菜单项
#define BARTYPENOTECONTENT 3	//内容显示
#define BARTYPESET  4			//设置项
#define BARTYPESTR  5			//状态信息显示
#define BARTYPEHANDLE 6		//表示确定或取消的操作；
#define BARTYPEINPUTNUM 8		//拨号菜单
#define BARTYPECALLLOG 9		//通话记录
#define BARTYPEEDIT 10			//输入法编辑
#define BARTYPE_LINKMANLIST 11	//联系人列表 
#define BARTYPE_DYNAMIC 12		/* 动态显示的菜单 */
#define BARTYPEFMRADIO   13       //收音机
#define BARTYPE_FMLIST    14       //收音机已存频点
#define BARTYPESUBAUDIO 15       //亚音频编码选择
#define BARTYPEDTMF         16      //DTMF 码值输入
#define BARTYPE1750Hz         17      //扫描操作显示
#define BARTYPE_SCANCHLIST   18      //扫描信道列表显示
#define BARTYPE_FREQINPUT 19     //频率输入(模拟)
#define BARTYPE547Hz        20       //产生54.7Hz的尾音消除
#define BARTYPE_SCANLIST 22      //扫描列表
#define BARTYPE_ADDSCANCHLIST 23 //添加扫描信道列表
#define BARTYPE_NAME 25//信道名称
#define BARTYPE_INPUTCHANNELNUM 26 //手动输入切换信道。

//全局变量的定义

typedef struct{
        uint8 KeyStateIndex; //当前状态索引号
        uint8 KeyUpState; //按下"向上"键时转向的状态索引号
        uint8 KeyDownState; //按下"向下"键时转向的状态索引号
        uint8 KeyLeftState; //按下"向左"键时转向的状态索引号
        uint8 KeyRightState;  //按下"向右"键时转向的状态索引号        
        uint8 KeyOkState;  //按下"确认"键时转向的状态索引号
        uint8 KeyBackState;  //按下"返回"键时转向的状态索引号
        void (*CurrentOperate)(); //当前状态应该执行的功能操作
} KbdTabStruct;

/* 动态菜单条的最大数目 */
#define MENU_ITEMMAX 31
//#define MENU_ITEMMAX 215

/* 当前菜单状态的结构体变量 */
typedef struct{
uint16 SltItemNum;									/* 当前菜单下需要显示的菜单条总数 */
uint8 SltItemBuf[MENU_ITEMMAX];					/* 被选择的需要显示的菜单条 */
uint8 SltIntemBuf_i;								/* 被选中菜单条存储数组的移位 */
uint8 SltItemBufBackUp[MenuSize][MENU_ITEMMAX];	/* 保存被选择菜单项 */
uint8 SltItemBufBackUp_i;
uint8 SltItemNumBackUp[MenuSize];
uint8 SltItemNumBackUp_i;
}STR_MENUSTAT;

extern STR_MENUSTAT Str_MenuStat;

extern uint8 vol;
extern uint8 flag_of_on;
extern uint8 g_Flash;               //跳转进入子菜单 刷新标志  >1刷新
extern volatile uint16 g_KeyFuncIndex;        //索引
extern uint16 g_FatherIndex[]; //父菜单备份数组
extern uint8 g_Layer ;               //菜单层数
extern uint8 g_ItemBackup[];      //当前菜单项备份
extern uint8 g_ItemBackup_i ;          //
extern uint8 g_WhereBackup[];     //反显位置备份
extern uint8 g_WhereBackup_i ;         //
extern uint8 g_Item;            //当前Bar项  范围:[0,g_ItemNum-1]  包括0 包括ItemNum-1  共包括ItemNum个取值
extern uint8 g_ItemNum;         //Bar项数
extern uint8 g_ItemNumLast;	//保存上次的g_ItemNum
extern uint8 g_BarType;
extern uint8 **g_DispItem; //项显示内容指针
extern uint8 **g_DispItemLast;

extern uint8 g_DispMin;         //ItemNum和DispMax中的小者
extern uint8 g_Where;           //当前反显位置,[0,DispMax-1]  反映反显的Bar在屏幕上的位置
extern uint32 g_DFRange;  //差频范围的值
extern uint8 g_Brightness;//屏幕背光亮度
extern uint8 g_Disp1;   //选择显式界面2 
extern uint8* g_ImgIndex;
extern void (*g_KeyFuncPtr)();
extern const KbdTabStruct g_KeyTab[];
extern uint8	g_ImgSel;
extern uint8   DisNum[9];
extern uint8   Freq_flag;
extern uint8   CHName_flag;
extern uint8   g_ChChangeOnA,g_ChChangeOnB;
//extern STR_CODESW g_Str_CodeSwbackup;//开始扫描的时候备份
extern uint8 TopSetFlag;//顶层设置项目的标记位.
extern uint8  LastKeyState;/*最后一个按键的备份值.*/
extern uint8 g_MenuBarType;
#if 0
//段菜单项设置
typedef enum{
BANDVALUESETFLG_SQUELCH=0, 	/* 静噪等级 */
BANDVALUESETFLG_DISPMODE ,	/*  */
BANDVALUESETFLG_FREQMODE ,		/* 参数设置区域设置 */
BANDVALUESETFLG_CHNO ,		/* 参数设置项中继时隙 */
BANDVALUESETFLG_LEDGREENSQFLAG,	/* 日期格式选择 */
BANDVALUESETFLG_ISUBANDSEL,		/* 背光灯选择 */
BANDVALUESETFLG_SQLGRADETABLE,		/* 背光亮度选择 */
BANDVALUESETFLG_SFT,	/* 按键提示音大小 */
BANDVALUESETFLG_STEP,			/* 自动锁选择 */
BANDVALUESETFLG_POWERSET,		/* 参数设置项高低功率 */
BANDVALUESETFLG_VOX,		/* 自编键２选择 */
BANDVALUESETFLG_BUSY,		/*发射时限*/
BANDVALUESETFLG_SCANMODE,	/* 短信提示音选择 */
BANDVALUESETFLG_SCANSEL,	/* 短信提示音选择 */
BANDVALUESETFLG_SCANEXITMODE,
BANDVALUESETFLG_PRINO,
BANDVALUESETFLG_SIGNALING,	/* 电量开关提示音 */
BANDVALUESETFLG_OFFSETFREQ,	/* 个呼提示音 */
BANDVALUESETFLG_RXCTDCS,	/* 个呼提示音 开关*/
BANDVALUESETFLG_TXCTDCS,	/* 组呼提示音 */
BANDVALUESETFLG_RXFREQH2L,	/* 组呼提示音 开关*/
BANDVALUESETFLG_RXFREQL2K,		/* 呼叫结束提示音 */
BANDVALUESETFLG_RXFREQMHK,	/* 按键提示音大小 */
BANDVALUESETFLG_RXFREQHM,	/* 电量提示音大小 */
BANDVALUESETFLG_TXCTCSS,		
BANDVALUESETFLG_RXCTCSS,		/*差频范围设定*/
BANDVALUESETFLG_TXCDCSS,			/*繁忙开关设定*/
BANDVALUESETFLG_RXCDCSS,		/*频率设定*/
BANDVALUESETFLG_DIRECTION,		/**/
BANDVALUESETFLG_TXPOWER,	/*设置收音机扫描模式*/

}e_BandValueSetFlg;
#endif 

/* 菜单设置项 */
typedef enum{
BARTYPESETFLG_TIME = 3,	/* 参数设置项时长 */
BARTYPESETFLG_POWER = 4,	/* 参数设置项高低功率 */
//BARTYPESETFLG_BStime = 5,	/* 参数设置项中继时隙 */
//BARTYPESETFLG_LANGUAGE , /* 语言选择 */
//BARTYPESETFLG_DATAFORM,	/* 日期格式选择 */
BARTYPESETFLG_Lcok,	/* 自动锁选择 */
//BARTYPESETFLG_HandLcok,	/* 手动锁选择 */
BARTYPESETFLG_LedOn,	/* 背光灯选择 */
//BARTYPESETFLG_Profile,	/* 情景模式选择 */
BARTYPESETFLG_KEYVOICE,	/* 按键声开关 */
BARTYPESETFLG_COLOUTONE,	/* 电量开关提示音 */
//BARTYPESETFLG_SINGLECALL,	/* 个呼提示音 */
//BARTYPESETFLG_GROUPCALL,	/* 组呼提示音 */
//BARTYPESETFLG_CALL,	/* 提示音 */
//BARTYPESETFLG_CALLSTOP,	/* 呼叫结束提示音 */
BARTYPESETFLG_KEYVOLUME,	/* 按键提示音大小 */
//BARTYPESETFLG_POWERVOLUME,	/* 电量提示音大小 */
BARTYPESETFLG_Squelch, /* 静噪级别设置*/
BARTYPESETFLG_FStep, /*频率步长设置*/
BARTYPESETFLG_MPower, /*模拟状态下的功率设置*/
BARTYPESETFLG_SFTDirection,/*差频方向*/
BARTYPESETFLG_SFTRange,/*差频范围*/
BARTYPESETFLG_SendTime,/*发射时限*/
//BARTYPESETFLG_Coding, /*编码开关*/
BARTYPESETFLG_Busy, /*繁忙开关*/
BARTYPESETFLG_Band,/*宽窄带*/
BARTYPESETFLG_POWERM,   /* 省电模式开关 */
BARTYPESETFLG_POWERMODE,   /* 省电模式*/
BARTYPESETFLG_POWERGAP,   /* 省电模式*/
BARTYPESETFLG_POWERMTime,   /* 省电模式时长 */
BARTYPESETFLG_Brightness,/*亮度*/
BARTYPESETFLG_FMSavedCH,/*收音机已存频点*/
BARTYPESETFLG_FMCHNum,/*收音机已存信道的总数*/
//BARTYPESETFLG_ENCRYPTION,/*加密设置*/
//BARTYPESETFLG_SCRAMBLERFREQ,/*用户扰频频率*/
//BARTYPESETFLG_USERSCRAMBLERFREQ,/*用户自定义扰频频率*/
BARTYPESETFLG_SCANMODE,/*扫描模式*/
BARTYPESETFLG_SCANTIME,/*扫描时长(时间模式)*/
BARTYPESETFLG_DispMode, /*模拟状态下的显示模式*/
//BARTYPESETFLG_1750Hz,     /*1750信令开关标志*/
//BARTYPESETFLG_RadioSeekMode, /*收音机调频模式*/
BARTYPESETFLG_CallTimeSW,     /*呼叫时长开关*/
//BARTYPESETFLG_SINGLECALLSW,	/* 个呼提示音 开关*/
//BARTYPESETFLG_GROUPCALLSW,	/* 组呼提示音 开关*/
//BARTYPESETFLG_CALLSW,	/* 组呼提示音 开关*/
//BARTYPESETFLG_GPS, /* GPS开关选择 */
///BARTYPESETFLG_GPSSQ, /* GPS时区选择 */
BARTYPESETFLG_SKOn,/* 声控开关选择 */
BARTYPESETFLG_SKDJ,	/* 声控等级选择 */
//BARTYPESETFLG_VOLUME,	/* 电量提示音大小 */
BARTYPESETFLG_POWERON_TONE,   /*开机铃声开关*/

}e_BarTypeSetFlg;

extern uint8 g_BarTypeSetFlag;		//处于哪一种菜单设置项

extern uint8 g_RtcTimeMonth,g_RtcTimeDay,g_RtcTimeHour,g_RtcTimeMin;
extern uint16 g_RtcTimeYear;						//rtc时间设置；
extern uint8  g_RtcTimeDigit;						//表示处于时间设置的哪一位；
/*菜单频率显示位置*/
#define	MENU_ROW_FIRCHTIP_PST	30
#define	MENU_ROW_FIRFREQ_PST	40
#define	MENU_ROW_SECCHTIP_PST	66
#define	MENU_ROW_SECFREQ_PST	76
#define	MENU_CHTIP_FREQ_COL_PST	20		
#define	MENU_CHTIP_CH_COL_PST		100
#define	MENU_ABTIP_COL_PST	5
#define	MENU_ARROWTIP_COL_PST	 14
#define	MENU_M_FREQ_PST	30
#define	MENU_K_FREQ_PST	64
#define	MENU_Z_FREQ_PST	90
 #define	MENU_FUNC_ROW_PST		10
#define	MENU_FUNC_COL_PST		60
#define	MENU_TIPCHAR_COL_PST	30


/* 菜单ok/back显示位置 */
/* 菜单ok/back显示位置 (列)*/
#define MENU_OK_PST 		5
#define MENU_BACK_PST 		110

#define MENU_MID_ROWS		53/*主屏幕单行显示居中开始行.*/
#define MENU_SPACE_COLS		3/*工作区间开始列*/
#define MENU_BAR_ROWS		110/*底部条，开始行位置*/
#define MENU_BAR_ASC_ROWS 	110/*底部条字体的开始行*/

#define MENU_BACK1_PST 	129 /* 英文back位置 */
#define MENU_BAR_PST 	152
#define MENU_BK_PST 		16


#define ATTR_ROW_A	14
#define ATTR_ROW_B	68


enum{
MENUBAR_TYPE_ICO=0,/*菜单显示的类型(图标型)*/
MENUBAR_TYPE_STR,/*菜单显示的类型(文字型)*/

};

//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------

#define DIR_ADD	0//频率数值递增
#define DIR_DEC    	1//频率数值递减

//extern osSemaphoreId Sem_MenuLock;

#define  MenuLock()  //OSSemPend(Sem_MenuLock,0,&err)
#define  MenuUnLock() // OSSemPost(Sem_MenuLock);

/*刷新当前层*/
#define REFLASHMENU() g_Flash = FlashMode_ReFlash
void MENUInit(void);
void MENULoop(void);
void MenuDispInit(void);
void DispMenuTop(void);
void DispMenuTopUp(void);
void DispMenuTopDown(void);
void DispMenuTopLeft(void);
void DispMenuTopRight(void);
void DispMenuTopOk(void);
void DispMenuTopBack(void);
void DispMenuCallRef(void);

void MenuValInit(void);
void Jump2Menu(uint8,uint8);


void DispMenu_1_Init(void);
void DispMenu_1_Up(void);
void DispMenu_1_Down(void);
void DispMenu_1_Left(void);
void DispMenu_1_Right(void);
void DispMenu_1_Ok(void);
void DispMenu_1_Back(void);

void DispMenu_2_Init(void);
void DispMenu_2_Up(void);
void DispMenu_2_Down(void);
void DispMenu_2_Left(void);
void DispMenu_2_Right(void);
void DispMenu_2_Ok(void);
void DispMenu_2_Back(void);

void PUSH(void);
void POP(void);

void KeyUp(void);
void KeyDown(void);
void KeyRight(void);
void KeyLeft(void);
void KeyNumber(uint8 key_digit);
void KeyA(void);
void KeyDel(void);
void KeyMode(void);
void DispBar(void);
void RadioSwitch(void);
void MENUDISP_APP_Rowone_Cue(void);


void sysParamInit(void);


uint32 getInputNumberFreq(void );
DH_BOOL checkInputFreq(uint32     freq,uint32 min, uint32 max);

void KeyFuncQuickEnter(uint8 Msg);


void TaskHndStart(u32 Flg);
void TaskHndStop(u32 Flg);
u32 TaskHndGet(u32 Flg);




#if 0
//进入菜单层保存上层变量并清零菜单条数；
#define DispMenuInitSave() {    \
                            g_ItemNumLast = g_ItemNum;	\
                            g_ItemNum = 0;\
                            if (g_Flash == FlashMode_AutoInit)	\
                            {\
                                    PUSH();\
                                    g_Item = 0;\
                                    g_Where = 1;\
                             }\
                        }
#else
	//进入菜单层保存上层变量并清零菜单条数；
inline void DispMenuInitSave(void)
{    			
      
	if (g_Flash == FlashMode_AutoInit)
	{		
    
		PUSH();
		g_Item = 0;
		g_Where = 1;
		
		
		 g_ItemNumLast = Str_MenuStat.SltItemNum;	
		 Str_MenuStat.SltItemNum = 0;
		
		
	}
	g_BarType =BARTYPEMENU;//菜单类型
}

#endif


#endif
