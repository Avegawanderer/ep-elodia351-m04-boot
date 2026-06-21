

#ifndef _APP_MENU_H
#define _APP_MENU_H

/* ========================================================================== */

/* ========================================================================== */

/* ========================================================================== */

/* ========================================================================== */

#if FUNC_MENU_DISP == MENU_DISP_NONE
#define MENU_STAT 0
#else
#define MENU_STAT 1
#endif

#define TASK_FLAG_VOICE   (1 << 0)
#define TASK_FLAG_100MS   (1 << 1)
#define TASK_FLAG_VOX     (1 << 2)
#define TASK_FLAG_1S      (1 << 3)
#define TASK_FLAG_SLEEP   (1 << 4)
#define TASK_FLAG_KEYSCAN (1 << 5)
#define TASK_FLAG_FMREC   (1 << 6)

#define LcdWriteLock()
#define LcdWriteUnlock()

#define MenuWriteLock()
#define MenuWriteUnlock()

#define INTF_RFLock()
#define INTF_RFUnlock()

#define HRC_BLL_CloseAudio() SPK_INTF_Close()
#define HRC_BLL_OpenAudio()  SPK_INTF_Open()

typedef enum
{
    FM_SUBVOICETYPE_CARRIER = 1,
    FM_SUBVOICETYPE_CTCSS,
    FM_SUBVOICETYPE_CDCSS,
    FM_SUBVOICETYPE_CDCSSR,
} FM_SUBVOICETYPE;

//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------

#define MenuTopIndex         0
#define Menu_1_Index         7
#define Menu_2_Index         14
#define Menu_3_Index         21
#define Menu_4_Index         28
#define Menu_5_Index         35
#define Menu_6_Index         42
#define Menu_7_Index         49
#define Menu_8_Index         56
#define Menu_9_Index         63
#define Menu_RecMes_Index    70
#define Menu_MisCall_Index   77
#define Menu_Vol_Index       84
#define Menu_Call_Index      91
#define Menu_PwdLock_Index   98
#define Menu_Tran_Index      105

#define FlashMode_NoAction   0

#define FlashMode_AutoInit   1

#define FlashMode_ManualInit 2

#define FlashMode_ReFlash    3

#define SelfMenuIndex        g_FatherIndex[g_Layer - 1]

#if FUNC_MENU_DISP == MENU_DISP_NONE
#define DispMax     1
#define DispMax_ICO 1
#elif FUNC_MENU_DISP == MENU_DISP_LED
#define DispMax     1
#define DispMax_ICO 1
#else
#if USE_LCD_DISP_SEL == USE_DISP_LCD19264
#define DispMax     1
#define DispMax_ICO 8
#elif USE_LCD_DISP_SEL == USE_DISP_LCD_ST7735
#define DispMax     4
#define DispMax_ICO 8
#endif
#endif

#if (USE_LCD_DISP_SEL == USE_DISP_LCD_ST7735)
#define ContentSize 3
#else
#define ContentSize 11
#endif

#define NoteUnRead            0
#define NoteReaded            1

#define BARTYPENONE           0
#define BARTYPENOTELIST       1
#define BARTYPEMENU           2
#define BARTYPENOTECONTENT    3
#define BARTYPESET            4
#define BARTYPESTR            5
#define BARTYPEHANDLE         6
#define BARTYPEINPUTNUM       8
#define BARTYPECALLLOG        9
#define BARTYPEEDIT           10
#define BARTYPELINKMANLIST    11
#define BARTYPEDYNAMIC        12
#define BARTYPEFMRADIO        13
#define BARTYPEFMLIST         14
#define BARTYPESUBAUDIO       15
#define BARTYPEDTMF           16
#define BARTYPE1750Hz         17
#define BARTYPESCANCHLIST     18
#define BARTYPEFREQINPUT      19
#define BARTYPE547Hz          20

#define BARTYPE_NONE          0
#define BARTYPE_NOTELIST      1
#define BARTYPE_MENU          2
#define BARTYPE_NOTECONTENT   3
#define BARTYPE_SET           4
#define BARTYPE_STR           5
#define BARTYPE_HANDLE        6
#define BARTYPE_INPUTNUM      8
#define BARTYPE_CALLLOG       9
#define BARTYPE_EDIT          10
#define BARTYPE_LINKMANLIST   11
#define BARTYPE_DYNAMIC       12
#define BARTYPE_RECORD        13
#define BARTYPE_CANCEL        14
#define BARTYPE_SCANLIST      15
#define BARTYPE_FMLIST        16
#define BARTYPE_RECGROUPLIST  17
#define BARTYPE_SUBVOICELIST  18
#define BARTYPE_ZONELIST      19
#define BARTYPE_RADIO_SEEK    20
#define BARTYPE_LISTSET       21
#define BARTYPE_FILTERCHNLIST 22
#define BARTYPE_GPSINFO       23
#define BARTYPE_GPSRECLIST    24
#define BARTYPE_GPSUTCLIST    25
#define BARTYPE_ONLYONESET    26
#define BARTYPE_ALLCHNLIST    27
#define BARTYPE_FREQALL_MENU  28
#define BARTYPE_ENCRYPTLIST   29
#define BARTYPE_SELNUM        30

#define SFT_OFF               0
#define SFT_Direction_POS     1
#define SFT_Direction_NEG     2
#define SFT_RangeMax          7000000
#define SFT_RangeMin          0

#if FUNC_MENU_DISP == MENU_DISP_LCD
#define MENU_ITEMMAX 16
#define MenuSize     10
#else
#define MENU_ITEMMAX 5
#define MenuSize     5
#endif

#if (USE_LCD_DISP_SEL == USE_DISP_LCD_ST7735)
#define MENU_MID_ROWS 53
#else
#define MENU_MID_ROWS 32
#endif

#define MENU_OK_PST 5
#if (LANGUAGE_SEL == LANGUAGE_SWEDEN)
#define MENU_BACK_PST 102
#else
#define MENU_BACK_PST 110
#endif

#define MENU_SPACE_COLS   3
#define MENU_BAR_ROWS     106
#define MENU_BAR_ASC_ROWS 108

#define MENU_BACK1_PST    (LCD_COL_MAX - LCD_ASCII_COL_MAX * 6 - 4)
#define MENU_BAR_PST      (LCD_COL_MAX - LCD_ASCII_COL_MAX - 4)

/* MENU */
#define LCD_MENU_COMS     16
#define LCD_MENU_COME     LCD_ROW_MAX
#define LCD_MENU_COM_MAX  (LCD_MENU_COME - LCD_MENU_COMS)

#define LCD_MENU_COLS     0
#define LCD_MENU_COLE     LCD_COL_MAX
#define LCD_MENU_COL_MAX  (LCD_MENU_COLE - LCD_MENU_COLS)

#define REFLASHMENU()     g_Flash = FlashMode_ReFlash

/* ========================================================================== */

/* ========================================================================== */

struct Menu
{
    DH_U8 **Text;
    DH_U8 TextNum;
    DH_U8 BarType_Type;
};

typedef struct
{
    DH_U8 KeyStateIndex;
    DH_U8 KeyUpState;
    DH_U8 KeyDownState;
    DH_U8 KeyLeftState;
    DH_U8 KeyRightState;
    DH_U8 KeyOkState;
    DH_U8 KeyBackState;
    DH_VOID(*CurrentOperate)
    (DH_VOID);
} KbdTabStruct;

typedef struct
{
    DH_U8 SltItemNum;
    DH_U8 SltItemBuf[MENU_ITEMMAX];
    DH_U8 SltIntemBuf_i;
    DH_U8 SltItemBufBackUp[MenuSize][MENU_ITEMMAX];
    DH_U8 SltItemBufBackUp_i;
    DH_U8 SltItemNumBackUp[MenuSize];
    DH_U8 SltItemNumBackUp_i;
} STR_MENUSTAT;

enum
{
    E_TopMenu_MENU = 0,
    E_TopMenu_GPS,
    E_TopMenu_LMList,
    E_TopMenu_DMRSUBAUDIO,
    E_TopMenu_ZONE,
    E_TopMenu_FMSUBAUDIO,
};

enum
{
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
    E_MENU_TIP_NONE,
};
typedef struct _GLOVARFLAG_
{
    DH_U32 PcWriteReadFlg : 1;
    DH_U32 VoxFlg : 1;
    DH_U32 OneKeyCall : 1;
    DH_U32 FmMontFlg : 1;
    DH_U32 FmMontCFlg : 1;

    DH_U32 bSoundPlayFlag : 1;    //
    DH_U32 DispCallFlag : 1;
    DH_U32 DispVolume : 1;
    DH_U32 FmRecStat : 1;
    DH_U32 ASensorScanFlg : 1;

    DH_U32 KeyFlag : 1;
    DH_U32 PcStatFlag : 1;

    DH_U32 UVDebugFlg : 1;

    DH_U32 RadioFlag : 1;
    DH_U32 RadioPauseFlag : 1;
    DH_U32 Freq_ExpandEnableFlag : 1;
    DH_U32 CopyFlag : 1;
    DH_U32 ChannelFiler : 1;
    DH_U32 DataTransferFlag : 1;

    DH_U32 Unused : 11;
} GLOVAR_FLAG;
extern GLOVAR_FLAG GloVarFlag;

typedef struct _MENU_GLOVARFLAG__
{
    DH_U32 KeySoundIgnoreFlg : 1;
    DH_U32 TopMenuNoRefleshFlg : 1;
    DH_U32 g_bZoneChnChgFlg : 2;
    DH_U32 g_bDFMChgFlg : 1;

    DH_U32 KeySideFuncFlg : 1;
    DH_U32 g_MenuDispSkipFlg : 1;
    DH_U32 g_KeyCsbkFlg : 1;
    DH_U32 DispMsgBoxDispFlg : 2;

    DH_U32 g_ChecksignalFlg : 1;    ///check if any operation is done in last 1 s
    DH_U32 bAnimalStopFlg : 1;
    DH_U32 bShutDownFlg : 1;
    volatile DH_U32 VoiceFlg : 1;
    volatile DH_U32 bSoundChangedFlg : 1;

    DH_U32 itemDispReverse : 1;
    DH_U32 AsciiSizeType : 2;
    DH_U32 DislayVolFlag : 1;
    DH_U32 CallSartDisplayFlag : 1;

    DH_U32 Unused : 12;
} MENU_GLOVARFLAG;
extern MENU_GLOVARFLAG MenuVarFlg;

#pragma pack(1)

typedef struct
{

    DH_U8 ScanMode;
    DH_U8 PowerMTime;

} STR_MENU_SET;

extern STR_MENU_SET StrMenuSet;

typedef struct _ALL_FLAG_
{
    uint32 SysStat_Poweron_OkFlag : 1;
    uint32 SysStat_SleepFlag : 1;
    uint32 Freq_ALL_SELFlg : 1;
    uint32 ShiftFunIsOnFlag : 1;
    uint32 KeyNumberOnFlag : 1;
    uint32 FreqallStatFlag : 2; /* */
    uint32 FreqallChangeFlag : 1;
    uint32 ReflashnumberFlag : 1;
    uint32 Freqall_SaveChnFlag : 1;
    uint32 NeedConfigRdaVolFlag : 1;
    uint32 DispMsgIsBusyFlag : 1;
    uint32 Reflash_SignleTBFlag : 1;
    uint32 Reflash_TxSignleTBFlag : 1;
    uint32 WakeupFromRTCFlag : 1;
    uint32 WakeupFromDeepSleepOpeOkFlag : 1;
    uint32 ScanFreqChangeDirFlag : 1;
    uint32 SysStat_EditChNameFlag : 1;
    uint32 SysStat_CloneHostDeviceFlag : 1;
    uint32 SysStat_CloneOKFlag : 1;
    uint32 SysStat_DelOneChannelFlag : 1;
    uint32 SysStat_CpuFreqAdjustOkFlag : 1;
    uint32 Freqall_SaveChnHadProgramFlag : 1;
    uint32 InputFreq_KeyNumberFlag : 1;
    uint32 DisplayMenutopOkFlag : 1;
    uint32 Raio_CloseSeekFlag : 1;
    uint32 NeedReflashRxChannelSwFlag : 1;
    uint32 Rx_BandSelectFlag : 1;
    uint32 MustStopABChangeFlag : 1;
    uint32 NeedABChangeSWFlag : 1;
    uint32 ABChangeTimerStopFlag : 1;
    uint32 NeedCloseVolumeCheck_Flag : 1;
    uint32 ReflashRxBusyFlag : 1;
} ALL_FLAG;
extern ALL_FLAG AllFlag;

typedef struct
{

    DH_U8 u8Cnt;

    DH_U8 u8StartCnt;

    DH_U8 u8StopCnt;

    DH_U16 u16StartNoiseLevel;

    DH_U16 u16StopNoiseLevel;
} SQL_STATUS_S;

extern DH_U8 g_FpgaTx;

extern STR_MENUSTAT Str_MenuStat;

//extern OS_SEM Sem_MenuLock;

extern volatile DH_U8 g_KeyFuncIndex;
extern DH_U8 g_Flash;
extern DH_U8 g_FatherIndex[];
extern DH_U8 g_Layer;
extern DH_U8 g_ItemBackup[];
extern DH_U8 g_ItemBackup_i;    //
extern DH_U8 g_WhereBackup[];
extern DH_U8 g_WhereBackup_i;    //
extern DH_U8 g_Item;
extern DH_U8 g_ItemNum;
extern DH_U8 g_ItemNumLast;
extern DH_U8 g_BarType;
extern DH_U8 g_BarTypeLast;
extern DH_U8 **g_DispItem;
extern DH_U8 g_DispMin;
extern DH_U8 g_Where;
extern DH_U8 g_IconType;
extern DH_U8 g_LMType;
extern DH_U8 g_MenuBarType;

extern DH_U16 g_setDispTemp;

enum
{
    MENUBAR_TYPE_ICO = 0,
    MENUBAR_TYPE_STR,

};

typedef enum
{

    BARTYPESETFLG_RoamConnectTime,    //
} e_BARTYPE_SETFlg;

extern DH_U8 g_BarTypeSetFlag;
extern DH_U8 g_RtcTimeMonth, g_RtcTimeDay, g_RtcTimeHour, g_RtcTimeMin;
extern DH_U16 g_RtcTimeYear;
extern DH_U8 g_RtcTimeDigit;

/* ========================================================================== */

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
void Jump2Menu(uint8, uint8);

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
DH_VOID MENU_APP_SimuBack(DH_VOID);

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
DH_VOID MENU_APP_Jump2Menu(DH_U8, DH_U8);

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
DH_VOID MENU_app_MesRecLIn(DH_U8 *MsgBuf, DH_U16 Len, DH_U8 Stat);

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

DH_U8 MENU_app_GetMenu(DH_VOID);
DH_VOID MENU_app_SetMenu(DH_U8 u8type);
DH_U8 MENU_app_GetKey(DH_VOID);
DH_VOID MENU_app_SetKey(DH_U8 u8type);

DH_VOID MENU_APP_TxChk(DH_VOID);

#if 0

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
DH_VOID MENU_APP_ZoneChnChg(DH_U8 zone, DH_U8 chn, DH_U8 b);
DH_VOID MENU_app_360CoderKeyUp(DH_VOID);
DH_VOID MENU_app_360CoderKeyDown(DH_VOID);
DH_VOID MENU_app_Jump2ContactsList(DH_VOID);
DH_VOID MENU_app_Jump2MenuPath(DH_U8 *path, DH_U8 layer);
DH_VOID MENU_app_TopMenuZoneChange(DH_U8 updown);
DH_VOID MENU_app_BottomTipDisp(DH_U8 okPst, DH_U8 backPst, DH_U8 lcd_color);
#endif

#endif
