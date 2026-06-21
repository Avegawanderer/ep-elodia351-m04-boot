

#ifndef _SCAN_DISP_H
#define _SCAN_DISP_H
extern uint16 LCDyanshi;
extern uint8 g_sendlimit;
extern uint8 DispDlyTimes;
extern uint8 DisableCouloScanTimer;
extern uint8 SaveChnTimer;

#if 0
#define VOL_7_1_DIV3 2125

#define VOL_6_9_DIV3 2070

#define VOL_6_7_DIV3 2010

#define VOL_6_5_DIV3 1940

#define VOL_6_4_DIV3 1905

#define VOL_6_3_DIV3 1870

#define VOL_6_0_DIV3 1861
#define VOL_5_8_DIV3 1620
#else

#define VOL_DIV_V_HIGHLIMIT 900
#define VOL_DIV_V_F         740

#define VOL_DIV_V_5         740

#define VOL_DIV_V_4         740

#define VOL_DIV_V_3         680

#define VOL_DIV_V_2         670

#define VOL_DIV_V_1         640

#define VOL_DIV_L_H         760    //> H

#define VOL_DIV_L_M         700    //> M

#define VOL_DIV_V_LOWLIMIT  570
#endif

/*

#define VOL_7_3_DIV3 2210
#define VOL_7_2_DIV3 2195
#define VOL_7_1_DIV3 2160
#define VOL_7_0_DIV3 2140
#define VOL_6_8_DIV3 2120
#define VOL_6_5_DIV3 2074
#define VOL_6_0_DIV3 1990
*/

#define ASENSOR_ON                1
#define ASENSOR_OFF               0

#define INFODISPFLG_EDIT          1

#define SCAN_TIMEMODE_REMAIN_TIME 10

#define SCAN_REMAIN_TIME_MAX      17

#define SCAN_SUB_REMAIN_TIME      10

void ScanDispInit(void);
void CouloScanInit(void);
void CouloScan(void);
void ASensorScanInit(void);
uint8 ASensorScanGet(void);
void ASensorScan(void);
void APP_1SCome(void);
void APP_TimeDisp(void);
void APP_DataDisp(void);
void APP_LockDisp(void);

void PCDispInit(void);
void PCDispTest(void);
void PYFlgDisp(uint8 Flg);
void ScanStatDisp(void);
uint8 LOWorHighPowerScan(void);
void SKDisp(void);
void Scan(void);
void DispTuBiaoInit(void);
void ScanFreqMode(void);
void ScanChangeFreq(void);
void ScanFreqMode_SaveFreq(uint32 Freq);
uint32 ScanFreqMode_GetFreq(void);
void Freqall_InputCursorDisp(void);
void Rda1846sCheckErr(void);
void BandSelectDiaplay(void);
void DisplayCallinTb_Check(void);
void Scan_SaveFreq(void);

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

void MENU_app_BottomTipDisp(u8 okPst, u8 backPst, u8 lcd_color);
void MENU_APP_SignalLine_Display(u8 Rxtx_flag, u8 num);
void MENU_APP_DQT_Display(u8 rxtx_flag, u8 BandFlag);
void MENU_APP_WorkstateFlag_Display(u8 BandFlag, u8 flag);
void MENU_APP_Volume_Display(void);
void MENU_APP_nCalldestop_Display(u8 Rxtx_flag, u8 BandFlag);
void MENU_APP_SFT_Direction_Display(u8 BandFlag);
void LcdWriteReverseFreqImage_Display(u8 BandFlag);
u8 GetFMBandWide(u8 BandFlag);
void TopFunCloseScanLoop(void);

uint8 GetChnDelLock(uint8 BandFlag);
void MENU_APP_Rev_Display(u8 BandFlag);

void Scan_Process(void);
void SecondScan(void);

#endif
