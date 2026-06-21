#ifndef _APP_SCAN_H
#define _APP_SCAN_H

#define MENUSETSCANMODE_TO          0
#define MENUSETSCANMODE_SO          1
#define MENUSETSCANMODE_SE          2

#define SCAN_TIMEMODE_REMAIN_TIME   10
#define SCAN_REMAIN_TIME_MAX        17
#define SCAN_SUB_REMAIN_TIME        10

#define SCAN_MODE_ChannelList       1
#define SCAN_MODE_FreqChange        2

#define SCAN_ON                     1
#define SCAN_OFF                    2
#define SCAN_ONOF                   3

#define SCAN_VALID_LISTNUM          E2PROM_SCANBASEADDR
#define SCAN_VALID_LISTNUMH         SCAN_VALID_LISTNUM + 0
#define SCAN_VALID_LISTNUML         SCAN_VALID_LISTNUM + 1
#define SCAN_scanIndex_FLAG_ADDR    SCAN_VALID_LISTNUM + 2
#define SCAN_scanIndex_CH_ADDR      SCAN_VALID_LISTNUM + 3
#define SCAN_scanIndex_CH_ITEM      SCAN_VALID_LISTNUM + 4

#define SCAN_FREQODE_SAVED_BASEADDR SCAN_VALID_LISTNUM + 10

#define SCAN_ONECH_LEN              2
#define SCAN_LIST_scanIndex_FLAG    40
#define SCAN_LIST_scanIndex_CH      41
#define SCAN_scanIndex_FLAG         0xA5
#define SCAN_LIST_VALID_FLAG        0XAA

#define SCAN_LIST_BASE              (SCAN_VALID_LISTNUM + 20)
#define SCAN_LIST_LEN               (512)

extern uint8 g_presentchn;
extern uint8 g_remainflag;
extern uint8 Scanflag;
//extern uint16 ScanListValue;
extern uint8 PreScanListValue;

typedef struct
{
    uint8 ScanMode;
    uint8 scanIndex;
    uint8 priFlag;
    uint8 ctcdmode;
    uint8 remaintime;
    uint8 sendmode;
    uint8 callback;
} STR_SCAN;

void ScanDispTopExit(void);

void ListChnSet(uint8 CCnt);
void ChnListScan(void);

void ScanStart(void);
void ScanStop(void);
uint8 ScanCtrlGet(void);
void ScanCtrlSet(uint8 stat);

#endif
