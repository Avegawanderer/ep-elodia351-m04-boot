/*******************************************************************************
 * Copyright (C) 2011-2013 ZheJiang Dahua Technology CO.,LTD.
 *
 * 文件名 : APP_MENU.h
 *
 * 作  者 : Li_endi(24539)<li_endi@dahuatech.com>
 * 版  本 : V1.0.0
 * 日  期 : 2015年5月15日 Create
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

#ifndef _APP_MENU_H
#define	_APP_MENU_H

/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */

/* ========================================================================== */
/*                           宏和类型定义区                                   */
/* ========================================================================== */

/*菜单*/
#if FUNC_MENU_DISP == MENU_DISP_NONE
	#define MENU_STAT 0
#else
	#define MENU_STAT 1
#endif


#define TASK_FLAG_VOICE      (1<<0)	/* 音量定时器任务 */
#define TASK_FLAG_100MS      (1<<1)	/* 刷新通话界面定时器任务 */
#define TASK_FLAG_VOX        (1<<2)	/* 延时执行定时器任务 */
#define TASK_FLAG_1S         (1<<3)	/* 长按键执行任务 */
#define TASK_FLAG_SLEEP      (1<<4)	/* 长按键执行任务 */
#define TASK_FLAG_KEYSCAN      (1<<5)
#define TASK_FLAG_FMREC      (1<<6)


#define LcdWriteLock()
#define		LcdWriteUnlock()


#define 		MenuWriteLock()		
#define 		MenuWriteUnlock()

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
#define Menu_RecMes_Index	70	/* 收到短信快捷键 */
#define Menu_MisCall_Index	77	/* 未接来电快捷键 */
#define Menu_Vol_Index		84	/* 音量显示所在层数 */
#define Menu_Call_Index		91	/* 呼叫层 */
#define Menu_PwdLock_Index	98	/* 加密锁定 */
#define Menu_Tran_Index	    105	/* 数据传输 */

//不初始化Flash
#define FlashMode_NoAction    0
//系统系统负责初始化
#define FlashMode_AutoInit    1
//手动初始化
#define FlashMode_ManualInit  2
//重绘画面
#define FlashMode_ReFlash     3
//返回当前菜单
#define SelfMenuIndex         g_FatherIndex[g_Layer-1]

#if FUNC_MENU_DISP == MENU_DISP_NONE
	#define DispMax 		      1     /* 一个屏幕最多能显示的菜单条数(文字型的)*/
	#define DispMax_ICO 	      1     /* 一个屏幕最多能显示的菜单条数 (图片型的)*/
#elif FUNC_MENU_DISP == MENU_DISP_LED
	#define DispMax 		      1     /* 一个屏幕最多能显示的菜单条数(文字型的)*/
	#define DispMax_ICO 	      1     /* 一个屏幕最多能显示的菜单条数 (图片型的)*/
#else
	#if USE_LCD_DISP_SEL == USE_DISP_LCD19264
		#define DispMax 		  1     /* 一个屏幕最多能显示的菜单条数(文字型的)*/
		#define DispMax_ICO 	  8     /* 一个屏幕最多能显示的菜单条数 (图片型的)*/
	#elif USE_LCD_DISP_SEL == USE_DISP_LCD_ST7735
		#define DispMax 		  4     /* 一个屏幕最多能显示的菜单条数(文字型的)*/
		#define DispMax_ICO 	  8     /* 一个屏幕最多能显示的菜单条数 (图片型的)*/
	#endif
#endif

#if(USE_LCD_DISP_SEL==USE_DISP_LCD_ST7735)
#define ContentSize           3		/* 显示(短信)最多需要的几屏 */
#else
#define ContentSize           11		/* 显示(短信)最多需要的几屏 */
#endif

#define NoteUnRead            0		//未读标志
#define NoteReaded            1		//已读标志

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
#define BARTYPELINKMANLIST 11	//联系人列表 
#define BARTYPEDYNAMIC 12		/* 动态显示的菜单 */
#define BARTYPEFMRADIO   13       //收音机
#define BARTYPEFMLIST    14       //收音机已存频点
#define BARTYPESUBAUDIO 15       //亚音频编码选择
#define BARTYPEDTMF         16      //DTMF 码值输入
#define BARTYPE1750Hz         17      //扫描操作显示
#define BARTYPESCANCHLIST   18      //扫描信道列表显示
#define BARTYPEFREQINPUT 19     //频率输入(模拟)
#define BARTYPE547Hz        20       //产生54.7Hz的尾音消除

#define BARTYPE_NONE          0
#define BARTYPE_NOTELIST      1		//短信列表
#define BARTYPE_MENU          2		//一般菜单项
#define BARTYPE_NOTECONTENT   3	    //内容显示
#define BARTYPE_SET           4		//设置项
#define BARTYPE_STR           5		//状态信息显示
#define BARTYPE_HANDLE        6		//表示确定或取消的操作；
#define BARTYPE_INPUTNUM      8		//拨号菜单
#define BARTYPE_CALLLOG       9		//通话记录
#define BARTYPE_EDIT          10	//输入法编辑
#define BARTYPE_LINKMANLIST   11	//联系人列表
#define BARTYPE_DYNAMIC       12	/* 动态显示的菜单 */
#define BARTYPE_RECORD        13	/* 录音列表 */
#define BARTYPE_CANCEL        14	//只显示取消和中间的显示信息
#define BARTYPE_SCANLIST      15	//扫描列表
#define BARTYPE_FMLIST        16
#define BARTYPE_RECGROUPLIST  17
#define BARTYPE_SUBVOICELIST  18
#define BARTYPE_ZONELIST      19
#define BARTYPE_RADIO_SEEK    20
#define BARTYPE_LISTSET       21    //多项设置
#define BARTYPE_FILTERCHNLIST 22    //过滤后的信道列表
#define BARTYPE_GPSINFO       23    //多项设置
#define BARTYPE_GPSRECLIST	  24
#define BARTYPE_GPSUTCLIST	  25    //时区列表
#define BARTYPE_ONLYONESET	  26    //单项设置(实心圆或者空心圆)
#define BARTYPE_ALLCHNLIST    27    //所有信道列表，包括空信道（*表示）
#define BARTYPE_FREQALL_MENU  28    /*全频模式使用*/
#define BARTYPE_ENCRYPTLIST   29    //加密选择列表
#define BARTYPE_SELNUM        30    //数字选择



///////////差频相关的设置//////////////////
#define	SFT_OFF			0//关闭差频.
#define	SFT_Direction_POS	1//正向
#define	SFT_Direction_NEG	2//负向
#define	SFT_RangeMax		7000000//70mhz差频可输入的范围.//按步进可被改变.
#define	SFT_RangeMin		0//注意需要按步进.









/* 动态菜单条的最大数目 */
#if FUNC_MENU_DISP == MENU_DISP_LCD
	#define MENU_ITEMMAX        16
	#define MenuSize            10          //菜单深度最大值
#else
	#define MENU_ITEMMAX        5
	#define MenuSize            5          //菜单深度最大值
#endif

#if(USE_LCD_DISP_SEL==USE_DISP_LCD_ST7735)
#define MENU_MID_ROWS		53/*主屏幕单行显示居中开始行.*/
#else
#define MENU_MID_ROWS		32/*主屏幕单行显示居中开始行.*/
#endif

/* 菜单ok/back显示位置 (列)*/
#define MENU_OK_PST 		5
#if(LANGUAGE_SEL==LANGUAGE_SWEDEN)
#define MENU_BACK_PST 		102
#else
#define MENU_BACK_PST 		110
#endif

#define MENU_SPACE_COLS		3/*工作区间开始列*/
#define MENU_BAR_ROWS		106/*底部条，开始行位置*/
#define MENU_BAR_ASC_ROWS 	108/*底部条字体的开始行*/

#define MENU_BACK1_PST      (LCD_COL_MAX-LCD_ASCII_COL_MAX*6-4) /* 英文back位置 */
#define MENU_BAR_PST        (LCD_COL_MAX-LCD_ASCII_COL_MAX-4)

/* MENU */
#define LCD_MENU_COMS       16 /* 菜单起始行 */
#define LCD_MENU_COME       LCD_ROW_MAX	/* 菜单结束行 */
#define LCD_MENU_COM_MAX    (LCD_MENU_COME-LCD_MENU_COMS)

#define LCD_MENU_COLS       0	/* 菜单起始列 */
#define LCD_MENU_COLE       LCD_COL_MAX	/* 菜单结束列 */
#define LCD_MENU_COL_MAX    (LCD_MENU_COLE-LCD_MENU_COLS)



/*刷新当前层*/
#define REFLASHMENU()     g_Flash = FlashMode_ReFlash

/* ========================================================================== */
/*                          数据结构定义区                                    */
/* ========================================================================== */

//Bar型菜单的数据结构

struct Menu
{
    DH_U8 **Text;     //指向显示内容数组的指针
    DH_U8 TextNum;         //项个数
    DH_U8 BarType_Type;
};

//全局变量的定义
typedef struct{
	DH_U8 KeyStateIndex; //当前状态索引号
	DH_U8 KeyUpState; //按下"向上"键时转向的状态索引号
	DH_U8 KeyDownState; //按下"向下"键时转向的状态索引号
	DH_U8 KeyLeftState; //按下"向左"键时转向的状态索引号
	DH_U8 KeyRightState;  //按下"向右"键时转向的状态索引号
	DH_U8 KeyOkState;  //按下"确认"键时转向的状态索引号
	DH_U8 KeyBackState;  //按下"返回"键时转向的状态索引号
	DH_VOID (*CurrentOperate)(DH_VOID); //当前状态应该执行的功能操作
} KbdTabStruct;

/* 当前菜单状态的结构体变量 */
typedef struct{
DH_U8 SltItemNum;									/* 当前菜单下需要显示的菜单条总数 */
DH_U8 SltItemBuf[MENU_ITEMMAX];			                    /*全频用到22个*//* 被选择的需要显示的菜单条 */
DH_U8 SltIntemBuf_i;								/* 被选中菜单条存储数组的移位 */
DH_U8 SltItemBufBackUp[MenuSize][MENU_ITEMMAX];	    /* 保存被选择菜单项 */
DH_U8 SltItemBufBackUp_i;
DH_U8 SltItemNumBackUp[MenuSize];
DH_U8 SltItemNumBackUp_i;
}STR_MENUSTAT;

enum{
	E_TopMenu_MENU = 0,
	E_TopMenu_GPS,
	E_TopMenu_LMList,
	E_TopMenu_DMRSUBAUDIO,
	E_TopMenu_ZONE,
	E_TopMenu_FMSUBAUDIO,
};

enum{
	E_MENU_TIP_MENU,
	E_MENU_TIP_ZONE,
	E_MENU_TIP_FUNC,
	E_MENU_TIP_OK,
	E_MENU_TIP_DEL,
	E_MENU_TIP_BACK,
	E_MENU_TIP_SEL,
	E_MENU_TIP_UNLOCK,
	E_MENU_TIP_GPSUNLOCATE,
	E_MENU_TIP_GPSLOCATE,
	E_MENU_TIP_NONE,//清屏背景色
};
typedef struct _GLOVARFLAG_{
	    DH_U32 PcWriteReadFlg  :1;
	    DH_U32 VoxFlg        :1;
	    DH_U32 OneKeyCall    :1;/*GPS 模块存在的标志*/
	    DH_U32 FmMontFlg     :1;	/* 模拟模式监听模式标志位（当前模拟信道进入监听） */
	    DH_U32 FmMontCFlg    :1;	/* 模拟永久模式监听模式标志位（所有模拟信道均进入监听模式） */

	    DH_U32 bSoundPlayFlag  :1;//
	    DH_U32 DispCallFlag  :1;//表示需要显示语音
	    DH_U32 DispVolume    :1;//表示需要显示音量
	    DH_U32 FmRecStat     :1;//表示当前是否处于FM接收状态
	    DH_U32 ASensorScanFlg:1;	/* 1:开启,0:关闭 */

	    DH_U32 KeyFlag       :1;   // 1:表示按键按下,0表示按键释放
	    DH_U32 PcStatFlag    :1;// 1 : 表示处于pc接收数据处理中；
	 
	    DH_U32 UVDebugFlg    :1;

	    DH_U32 RadioFlag     :1;
	    DH_U32 RadioPauseFlag:1;
	    DH_U32 Freq_ExpandEnableFlag:1;
	    DH_U32 CopyFlag       :1;/*GPS 模块存在的标志*/
	    DH_U32 ChannelFiler   :1;/*信道过滤模式，只显示DMR*/
		DH_U32 DataTransferFlag  : 1;/*数据传输模式*/

	    DH_U32 Unused : 11;
}GLOVAR_FLAG;
extern GLOVAR_FLAG GloVarFlag;

typedef struct _MENU_GLOVARFLAG__{
	DH_U32 KeySoundIgnoreFlg:         1; /* 按键音*/
	DH_U32 TopMenuNoRefleshFlg:       1; /* 1 切换频道时不对整个屏幕刷屏*/
	DH_U32 g_bZoneChnChgFlg:          2; /* 0：区域不变，1：区域或信道有变化，2：区域有变化*/
	DH_U32 g_bDFMChgFlg :             1;

	DH_U32 KeySideFuncFlg :           1;
	DH_U32 g_MenuDispSkipFlg :        1;
	DH_U32 g_KeyCsbkFlg :             1;
	DH_U32 DispMsgBoxDispFlg :        2;

	DH_U32 g_ChecksignalFlg :         1; ///check if any operation is done in last 1 s
	DH_U32 bAnimalStopFlg:            1;
	DH_U32 bShutDownFlg :             1;
	volatile DH_U32 VoiceFlg:         1; /* 1为喇叭在主动输出声音，0为输出完成防止在开了声控的情况下倒发出去*/
	volatile DH_U32 bSoundChangedFlg: 1;

	DH_U32 itemDispReverse            :1;/*反显*/
	DH_U32 AsciiSizeType              :2;/*ascii码尺寸的类型选择,0=12X24,1=08X16,2=5X8*/
	DH_U32 DislayVolFlag              :1;/*1=是开启音量条显示的状态，0=常规提示性语句*/
	DH_U32 CallSartDisplayFlag        :1;/*1=开始呼叫,需要第一次显示名称或者号码,0=显示完毕了.后续不要一直更新*/

	DH_U32 Unused                    : 12;
}MENU_GLOVARFLAG;
extern MENU_GLOVARFLAG MenuVarFlg;

/* 定义菜单系统的设置项结构体 */
#pragma  pack(1)

typedef struct {

	DH_U8 ScanMode;
	DH_U8 PowerMTime;

}STR_MENU_SET;

extern STR_MENU_SET StrMenuSet;


typedef struct _ALL_FLAG_{
uint32 SysStat_Poweron_OkFlag:1;/*初始化完毕的标志位.*/
uint32 SysStat_SleepFlag:1;/*需要打开*/
uint32 Freq_ALL_SELFlg:1;/*0=普通模式,1=全频模式*/
uint32 ShiftFunIsOnFlag:1;/*开启了第二功能了。//标记着刚才有段按下右键.*/
uint32 KeyNumberOnFlag:1;/*数字按键也按下去的标志.*/
uint32 FreqallStatFlag:2;/* */
uint32 FreqallChangeFlag:1;/*M为单位改变的标志*/
uint32 ReflashnumberFlag:1;/*闪烁的问题，需要重新还原刷掉的数字.*/
uint32 Freqall_SaveChnFlag:1;
uint32 NeedConfigRdaVolFlag:1;/*在设置宽窄带时需要配置音量.*/
uint32 DispMsgIsBusyFlag:1;/*正在显示音量条,不允许显示提示信息和360编码器中断.*/
uint32 Reflash_SignleTBFlag:1;/*接收0==需要刷新信号图标.1=不需要刷新信号图标*/
uint32 Reflash_TxSignleTBFlag:1;/*发射0==需要刷新信号图标.1=不需要刷新信号图标*/
uint32 WakeupFromRTCFlag:1;/*是在RTC唤醒的,0==是其他地方唤醒的.*/
uint32 WakeupFromDeepSleepOpeOkFlag:1;/*深度睡眠唤醒之后的动作执行完毕了.0=没执行需要再次执行.*/
uint32 ScanFreqChangeDirFlag:1;/*扫频模式的频率增减方向:1=频率数值递减   0//频率数值递增*/
uint32 SysStat_EditChNameFlag:1;/*1=处于正在编辑信道号码的状态.*/
uint32 SysStat_CloneHostDeviceFlag:1;/*0=是主机模式.1=从机*/
uint32 SysStat_CloneOKFlag:1;/*0=有线复制成功了,1=反之.*/
uint32 SysStat_DelOneChannelFlag:1;/*1==处于删除1个信道状态.0=不处于删除信道的状态.*/
uint32 SysStat_CpuFreqAdjustOkFlag:1;/*0==已经校准过CPU频率了.1==没校准过的.*/
uint32 Freqall_SaveChnHadProgramFlag:1;/*1=准备保存到的目标信道已经保存有信息了.0=未编程信道*/
uint32 InputFreq_KeyNumberFlag:1;/*1==是正常的输入频率的方向,0=回删/确认/退出等操作.*/
uint32 DisplayMenutopOkFlag:1;/*1==顶层显示已经完成了.*/
uint32  Raio_CloseSeekFlag:1;/*1==正在搜索模式下关闭的标志位.*/
uint32 NeedReflashRxChannelSwFlag:1;/*1==需要执行切换接收的动作,0==不需要.*/
uint32 Rx_BandSelectFlag:1;/*0==接收切换到A段,1==选中B段*/
uint32 MustStopABChangeFlag:1;/*1==必须停止切换直到关机的标志//用于上位机调试时.*/
uint32 NeedABChangeSWFlag:1;/*1=需要AB段接收切换了,0=不需要切换的.*/
uint32 ABChangeTimerStopFlag:1;/*1==定时器已经停止的标志.0=定时器在打开的状态.*/
uint32 NeedCloseVolumeCheck_Flag:1;/*1==需要禁止电量的显示(在360编码器响应任务期间不允许音量显示,否则交错显示.)*/
uint32 ReflashRxBusyFlag:1;/*1==正在忙于切换,0=不处于切换的过程中.*/
}ALL_FLAG;
extern ALL_FLAG AllFlag;


typedef struct{
	/* 时间计数器 */
	DH_U8 u8Cnt;
	/* 启动计数器 */
	DH_U8 u8StartCnt;
	/* 关闭计数器 */
	DH_U8 u8StopCnt;
	/* 启动时噪声门限 */
	DH_U16 u16StartNoiseLevel;
	/* 关闭时噪声门限 */
	DH_U16 u16StopNoiseLevel;
}SQL_STATUS_S; 

extern DH_U8 g_FpgaTx;

extern STR_MENUSTAT Str_MenuStat;

//extern OS_SEM Sem_MenuLock;	

extern volatile DH_U8 g_KeyFuncIndex;  //索引
extern DH_U8 g_Flash;                  //跳转进入子菜单 刷新标志  >1刷新
extern DH_U8 g_FatherIndex[];          //父菜单备份数组
extern DH_U8 g_Layer ;                 //菜单层数
extern DH_U8 g_ItemBackup[];           //当前菜单项备份
extern DH_U8 g_ItemBackup_i ;          //
extern DH_U8 g_WhereBackup[];          //反显位置备份
extern DH_U8 g_WhereBackup_i ;         //
extern DH_U8 g_Item;                   //当前Bar项  范围:[0,g_ItemNum-1]  包括0 包括ItemNum-1  共包括ItemNum个取值
extern DH_U8 g_ItemNum;                //Bar项数
extern DH_U8 g_ItemNumLast;	           //保存上次的g_ItemNum
extern DH_U8 g_BarType;
extern DH_U8 g_BarTypeLast;
extern DH_U8 **g_DispItem;             //项显示内容指针
extern DH_U8 g_DispMin;                //ItemNum和DispMax中的小者
extern DH_U8 g_Where;                  //当前反显位置,[0,DispMax-1]  反映反显的Bar在屏幕上的位置
extern DH_U8 g_IconType;               //图标类型
extern DH_U8 g_LMType;                 //图标类型
extern DH_U8 g_MenuBarType;

extern DH_U16 g_setDispTemp;               //设置显示调节时的临时变量




enum{
MENUBAR_TYPE_ICO=0,/*菜单显示的类型(图标型)*/
MENUBAR_TYPE_STR,/*菜单显示的类型(文字型)*/

};
/* 菜单设置项 */
typedef enum{
	
	BARTYPESETFLG_RoamConnectTime,//
}e_BARTYPE_SETFlg;

extern DH_U8  g_BarTypeSetFlag;		//处于哪一种菜单设置项
extern DH_U8  g_RtcTimeMonth,g_RtcTimeDay,g_RtcTimeHour,g_RtcTimeMin;
extern DH_U16 g_RtcTimeYear;						//rtc时间设置；
extern DH_U8  g_RtcTimeDigit;						//表示处于时间设置的哪一位；

/* ========================================================================== */
/*                          函数声明区                                        */
/* ========================================================================== */
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


DH_VOID KEY_APP_TaskMenu(DH_U32 initial_data);	
DH_VOID MENU_APP_TaskMenu(DH_U32 initial_data);	
DH_VOID MENU_app_Menu_Init(DH_VOID);
DH_VOID MENU_APP_KeyLoop(DH_VOID);
DH_VOID MENU_APP_MENULoop(DH_VOID);
DH_VOID MENU_APP_MenuDispInit(DH_VOID);
DH_VOID MENU_APP_SimuBack(DH_VOID );

DH_VOID MENU_APP_ReFlash(DH_VOID);

DH_VOID MENU_app_Menu_Top_Init(DH_VOID);
DH_VOID MENU_app_Menu_Top_Up(DH_VOID);
DH_VOID MENU_app_Menu_Top_Down(DH_VOID);
DH_VOID MENU_app_Menu_Top_Left(DH_VOID);
DH_VOID MENU_app_Menu_Top_Right(DH_VOID);
DH_VOID MENU_app_Menu_Top_Ok(DH_VOID);
DH_VOID MENU_app_Menu_Top_Back(DH_VOID);

DH_VOID MENU_app_DispMenuCallRef(DH_VOID);
DH_VOID MENU_APP_MenuValInit(DH_VOID);
DH_VOID MENU_APP_Jump2Menu(DH_U8,DH_U8);

DH_VOID MENU_app_Menu_1_Init(DH_VOID);
DH_VOID MENU_app_Menu_1_Up(DH_VOID);
DH_VOID MENU_app_Menu_1_Down(DH_VOID);
DH_VOID MENU_app_Menu_1_Left(DH_VOID);
DH_VOID MENU_app_Menu_1_Right(DH_VOID);
DH_VOID MENU_app_Menu_1_Ok(DH_VOID);
DH_VOID MENU_app_Menu_1_Back(DH_VOID);

DH_VOID MENU_app_Menu_2_Init(DH_VOID);
DH_VOID MENU_app_Menu_2_Up(DH_VOID);
DH_VOID MENU_app_Menu_2_Down(DH_VOID);
DH_VOID MENU_app_Menu_2_Left(DH_VOID);
DH_VOID MENU_app_Menu_2_Right(DH_VOID);
DH_VOID MENU_app_Menu_2_Ok(DH_VOID);
DH_VOID MENU_app_Menu_2_Back(DH_VOID);

DH_VOID MENU_app_Menu_3_Init(DH_VOID);
DH_VOID MENU_app_Menu_3_Up(DH_VOID);
DH_VOID MENU_app_Menu_3_Down(DH_VOID);
DH_VOID MENU_app_Menu_3_Left(DH_VOID);
DH_VOID MENU_app_Menu_3_Right(DH_VOID);
DH_VOID MENU_app_Menu_3_Ok(DH_VOID);
DH_VOID MENU_app_Menu_3_Back(DH_VOID);

DH_VOID MENU_app_Menu_4_Init(DH_VOID);
DH_VOID MENU_app_Menu_4_Up(DH_VOID);
DH_VOID MENU_app_Menu_4_Down(DH_VOID);
DH_VOID MENU_app_Menu_4_Left(DH_VOID);
DH_VOID MENU_app_Menu_4_Right(DH_VOID);
DH_VOID MENU_app_Menu_4_Ok(DH_VOID);
DH_VOID MENU_app_Menu_4_Back(DH_VOID);

DH_VOID MENU_app_Menu_5_Init(DH_VOID);
DH_VOID MENU_app_Menu_5_Up(DH_VOID);
DH_VOID MENU_app_Menu_5_Down(DH_VOID);
DH_VOID MENU_app_Menu_5_Left(DH_VOID);
DH_VOID MENU_app_Menu_5_Right(DH_VOID);
DH_VOID MENU_app_Menu_5_Ok(DH_VOID);
DH_VOID MENU_app_Menu_5_Back(DH_VOID);

DH_VOID MENU_app_Menu_6_Init(DH_VOID);
DH_VOID MENU_app_Menu_6_Up(DH_VOID);
DH_VOID MENU_app_Menu_6_Down(DH_VOID);
DH_VOID MENU_app_Menu_6_Left(DH_VOID);
DH_VOID MENU_app_Menu_6_Right(DH_VOID);
DH_VOID MENU_app_Menu_6_Ok(DH_VOID);
DH_VOID MENU_app_Menu_6_Back(DH_VOID);

DH_VOID MENU_app_Menu_7_Init(DH_VOID);
DH_VOID MENU_app_Menu_7_Up(DH_VOID);
DH_VOID MENU_app_Menu_7_Down(DH_VOID);
DH_VOID MENU_app_Menu_7_Left(DH_VOID);
DH_VOID MENU_app_Menu_7_Right(DH_VOID);
DH_VOID MENU_app_Menu_7_Ok(DH_VOID);
DH_VOID MENU_app_Menu_7_Back(DH_VOID);

DH_VOID MENU_app_Menu_8_Init(DH_VOID);
DH_VOID MENU_app_Menu_8_Up(DH_VOID);
DH_VOID MENU_app_Menu_8_Down(DH_VOID);
DH_VOID MENU_app_Menu_8_Left(DH_VOID);
DH_VOID MENU_app_Menu_8_Right(DH_VOID);
DH_VOID MENU_app_Menu_8_Ok(DH_VOID);
DH_VOID MENU_app_Menu_8_Back(DH_VOID);

DH_VOID MENU_app_Menu_9_Init(DH_VOID);
DH_VOID MENU_app_Menu_9_Up(DH_VOID);
DH_VOID MENU_app_Menu_9_Down(DH_VOID);
DH_VOID MENU_app_Menu_9_Left(DH_VOID);
DH_VOID MENU_app_Menu_9_Right(DH_VOID);
DH_VOID MENU_app_Menu_9_Ok(DH_VOID);
DH_VOID MENU_app_Menu_9_Back(DH_VOID);

DH_VOID MENU_app_Menu_HotKey_Init(DH_VOID);
DH_VOID MENU_app_Menu_HotKey_Up(DH_VOID);
DH_VOID MENU_app_Menu_HotKey_Down(DH_VOID);
DH_VOID MENU_app_Menu_HotKey_Left(DH_VOID);
DH_VOID MENU_app_Menu_HotKey_Right(DH_VOID);
DH_VOID MENU_app_Menu_HotKey_Ok(DH_VOID);
DH_VOID MENU_app_Menu_HotKey_Back(DH_VOID);
DH_VOID MENU_app_MesRecLIn(DH_U8* MsgBuf, DH_U16 Len, DH_U8 Stat);

DH_VOID MENU_app_Menu_MisCall_Init(DH_VOID);
DH_VOID MENU_app_Menu_MisCall_Up(DH_VOID);
DH_VOID MENU_app_Menu_MisCall_Down(DH_VOID);
DH_VOID MENU_app_Menu_MisCall_Left(DH_VOID);
DH_VOID MENU_app_Menu_MisCall_Right(DH_VOID);
DH_VOID MENU_app_Menu_MisCall_Ok(DH_VOID);
DH_VOID MENU_app_Menu_MisCall_Back(DH_VOID);
DH_VOID MENU_APP_MissedCallIn(DH_VOID);

DH_VOID MENU_app_Menu_Vol_Ref(DH_VOID);
DH_VOID MENU_app_Menu_Vol_Init(DH_VOID);
DH_VOID MENU_app_Menu_Vol_Up(DH_VOID);
DH_VOID MENU_app_Menu_Vol_Down(DH_VOID);
DH_VOID MENU_app_Menu_Vol_Left(DH_VOID);
DH_VOID MENU_app_Menu_Vol_Right(DH_VOID);
DH_VOID MENU_app_Menu_Vol_Ok(DH_VOID);
DH_VOID MENU_app_Menu_Vol_Back(DH_VOID);

DH_VOID MENU_app_Menu_Call_Init(DH_VOID);
DH_VOID MENU_app_Menu_Call_Up(DH_VOID);
DH_VOID MENU_app_Menu_Call_Down(DH_VOID);
DH_VOID MENU_app_Menu_Call_Left(DH_VOID);
DH_VOID MENU_app_Menu_Call_Right(DH_VOID);
DH_VOID MENU_app_Menu_Call_Ok(DH_VOID);
DH_VOID MENU_app_Menu_Call_Back(DH_VOID);


DH_VOID MENU_app_Menu_Pwd_Init(DH_VOID);
DH_VOID MENU_app_Menu_Pwd_Up(DH_VOID);
DH_VOID MENU_app_Menu_Pwd_Down(DH_VOID);
DH_VOID MENU_app_Menu_Pwd_Left(DH_VOID);
DH_VOID MENU_app_Menu_Pwd_Right(DH_VOID);
DH_VOID MENU_app_Menu_Pwd_Ok(DH_VOID);
DH_VOID MENU_app_Menu_Pwd_Back(DH_VOID);


DH_VOID MENU_APP_PUSH(DH_VOID);
DH_VOID MENU_APP_POP(DH_VOID);

DH_VOID MENU_app_KeyUp(DH_VOID);
DH_VOID MENU_app_KeyDown(DH_VOID);
DH_VOID MENU_app_KeyRight(DH_VOID);
DH_VOID MENU_app_KeyLeft(DH_VOID);
DH_VOID MENU_app_KeyNumber(DH_U8 key_digit);
DH_VOID MENU_app_KeyA(DH_VOID);
DH_VOID MENU_app_KeyDel(DH_VOID);
DH_VOID MENU_app_KeyMode(DH_VOID);

DH_VOID MENU_app_KeyHandUp(DH_VOID);

DH_U8   MENU_app_GetMenu(DH_VOID);
DH_VOID MENU_app_SetMenu(DH_U8 u8type);
DH_U8   MENU_app_GetKey(DH_VOID);
DH_VOID MENU_app_SetKey(DH_U8 u8type);

DH_VOID MENU_APP_TxChk(DH_VOID);

#if 0
//进入菜单层保存上层变量并清零菜单条数；
inline DH_VOID MENU_app_DispMenuInitSave(DH_VOID)
{    			
	if (g_Flash == FlashMode_AutoInit)
	{		
		MENU_APP_PUSH();
		g_Item = 0;
		g_Where = 1;
		g_ItemNumLast = Str_MenuStat.SltItemNum;	
		Str_MenuStat.SltItemNum = 0;
	}
}
#else
DH_VOID MENU_app_DispMenuInitSave(DH_VOID);
DH_VOID MENU_APP_ZoneChnChg(DH_U8 zone,DH_U8 chn,DH_U8 b);
DH_VOID MENU_app_360CoderKeyUp(DH_VOID);
DH_VOID MENU_app_360CoderKeyDown(DH_VOID);
DH_VOID MENU_app_Jump2ContactsList(DH_VOID);
DH_VOID MENU_app_Jump2MenuPath(DH_U8 *path, DH_U8 layer);
DH_VOID MENU_app_TopMenuZoneChange(DH_U8 updown);
DH_VOID MENU_app_BottomTipDisp(DH_U8 okPst,DH_U8 backPst,DH_U8 lcd_color);
#endif

#endif
