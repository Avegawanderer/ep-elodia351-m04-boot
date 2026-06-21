

#ifndef _KEY_TIME_H
#define _KEY_TIME_H

#define KEYSELMAX              38

#define KEYTIMEL_MAX           50
#define KEYTIMEL_DEFAULT       20
#define KEYTIMEL_MIN           10

#define KEYURGENTTIMEL_MAX     0x0e
#define KEYURGENTTIMEL_DEFAULT 0x04
#define KEYURGENTTIMEL_MIN     0x01

#define KEY_FLG_LONG           1
#define KEY_FLG_SHORT          2
#define KEY_FLG_NONE           3

typedef struct
{
    void (*P1LHnd)();
    void (*P2LHnd)();
    void (*OKLHnd)();
    void (*BackLHnd)();
    //void(*P4LHnd)();
    //void(*P5LHnd)();
    //void(*P6LHnd)();
    //void(*Key1LHnd)();
    //void(*Key2LHnd)();
    //void(*Key3LHnd)();
    //void(*Key4LHnd)();
    //void(*Key5LHnd)();
    //void(*Key6LHnd)();
    //void(*Key7LHnd)();
    //void(*Key8LHnd)();
    //void(*Key9LHnd)();
    //void(*KeyLBack)();
    void (*P1SHnd)();
    void (*P2SHnd)();
    void (*P3SHnd)();
    void (*P4SHnd)();
    //void(*P3SHnd)();
    //void(*P4SHnd)();
    //void(*P5SHnd)();
    //void(*P6SHnd)();
    //void(*Key1SHnd)();
    //void(*Key2SHnd)();
    //void(*Key3SHnd)();
    //void(*Key4SHnd)();
    //void(*Key5SHnd)();
    //void(*Key6SHnd)();
    //void(*Key7SHnd)();
    //void(*Key8SHnd)();
    //void(*Key9SHnd)();
    //void(*KeySBack)();
    //
    //void(*KeyUpSHnd)();
    //void(*KeyUpLHnd)();
    //void(*KeyDownSHnd)();
    //void(*KeyDownLHnd)();

    uint8 LongKeyT;
    uint8 KeyType;
    uint8 KeyLSFlg;
    void (*KeyLongHnd)();
} STR_KEYT;

extern STR_KEYT Str_KeyT;
extern uint8 g_monitorflag, g_fmonitorflag;
extern uint8 g_channelcalbuf[16];
extern uint8 g_keyencryp;
extern uint8 g_ErrFlag;

#define KEYT_NONE           0
#define KEYT_DmrAct         1
#define KEYT_DmrChck        2
#define KEYT_DmrRadioDisale 3
#define KEYT_Listen         4
#define KEYT_Callclew       5

typedef enum
{
    HANDKeyNoHnd = 0,           ///0
    HANDKeyPower,               ///1
    HANDKeyLedSw,               ///2
    HANDKeyLock,                ///3
    HANDKeyVox,                 ///4
    HANDKeyZoneChange,          ///5
    HANDKeyScanSw,              ///6
    HANDKeyScanModeChg,         ///7
    HANDKeyRelay,               ///8
    HANDKeyExiOpen,             ///9
    HANDKeyEncryp,              ///10
    HANDKeyLM,                  ///11
    HANDKeyNote,                ///12
    HANDKeyDmrAct,              ///13
    HANDKeyDmrChck,             ///14
    HANDKeyDmrRadioDisale,      ///15
    HANDKeyListen,              ///16
    HANDKeyMonitor,             ///17
    HANDKeyPermanentMonitor,    ///18
    HANDkey_DispMode,           //HANDkey_1750tone,///19
    HANDInformationSearch,      ///20
    HANDKeyADSW,                ///21
    HANDKeySQL,                 ///22
    HANDKey1L,                  ///23
    HANDKey2L,                  ///24
    HANDKey3L,                  ///25
    HANDKeyLight,               //26
    HANDKeyGPS_SW,              ///27
    HANDkey_700tone,            ///28
    HANDkey_1000tone,           ///29
    HANDkey_1200tone,           ///30
    HANDkey_1400tone,           ///31
    HANDkey_2100tone,           ///32
    HANDKey4L,                  ///33
    HANDKey5L,                  ///34
    HANDKey6L,                  ///35
    HANDKeyZoneSet,             ///36
    HANDKeyUselessChannel,      //37
    HANDKeyASensorScan,         ///38
    HANDKeyAdel,                ///39

    HANDFreqallKeyLeft_Short,     //40
    HANDFreqallKeyLeft_Long,      //41
    HANDFreqallKeyRight_Short,    //42
    HANDFreqallKeyRight_Long,     //43
    HANDFreqallKeyUp_Short,       //44
    HANDFreqallKeyUp_Long,        //45
    HANDFreqallKeyDown_Short,     //46
    HANDFreqallKeyDown_Long       //47
} ENM_KEYHND;

void KeyTimeInit(void);
void KeyTimeStart(uint8 Flg);
void KeyUrgentTimeStart(uint8 Flg);
void KeyTimeLongHnd(void);
void KeyTimeStop(void);
void KeyUrgentTimeStop(void);
void KeyFncSet(uint8 Stat);
uint8 KeyFncGet(void);
void KeyFncExe(uint8 Stat);
void KeyNoHnd(void);
void KeyLedSw(void);
void KeyDmrAct(void);
void KeyDmrChck(void);
void KeyDmrRadioDisale(void);
void KeyListen(void);
void KeyMonitor(void);
void KeyPermanentMonitor(void);
void KeyPower(void);
void KeyADSW(void);
void KeyEncryp(void);
void KeyLock(void);
void KeyExiOpen(void);
void KeyExiClse(void);
void KeyZoneSet(void);
void KeyZoneChange(void);
void KeyScanSw(void);
void KeyLM(void);
void Key1L(void);
void Key2L(void);
void Key3L(void);
void Key4L(void);
void Key5L(void);
void Key6L(void);
void KeyRelay(void);
void KeySQL(void);
void KeyUselessChannel(void);
void KeyASensorScan(void);
void KeyVox(void);
void KeyAdel(void);
void KeyLight(void);

void KeyScanModeChg(void);
void KeyNote(void);
void key_DispMode(void);    //void key_1750tone(void);
void keyInfoChk(void);
extern void KeyHndD_Ptt(void);
void InformationSearch(void);
uint8 Check_TheEnableKEYs(uint8 flg);
uint8 Check_TheEnableKeyFuction(uint8 flg);

//void KeyGPS_SW();
//void Key_Compr();
void DispModeChange(uint8 newmode);

void key_700tone(void);
void key_1000tone(void);
void key_1200tone(void);
void key_1400tone(void);
void key_2100tone(void);
void FreqallKeyLeft_Short(void);
void FreqallKeyLeft_Long(void);
void FreqallKeyRight_Short(void);
void FreqallKeyRight_Long(void);
void FreqallKeyUp_Short(void);
void FreqallKeyUp_Long(void);
void FreqallKeyDown_Short(void);
void FreqallKeyDown_Long(void);

/////////////////////////////////////////////////////
DH_BOOL KeyFun_Monitor_Down_Check(uint8 num);

#endif
