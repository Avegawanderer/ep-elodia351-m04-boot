

#ifndef _BLL_PCTUNERSET_
#define _BLL_PCTUNERSET_

#include "BLL_PCTUNER_FREQ.h"
#include "app_config.h"

#define TUNER_CMD_READ  0x01
#define TUNER_CMD_WRITE 0x02

#if 1
#define TUNERALLWRITE         0x01
#define TUNERALLREAD          0x02

#define FREQCHEXKSET          0x10
#define HIGHPOWERSET          0x11
#define LOWPOWERSET           0x12
#define DMODBALANCESET        0x13
#define MDEVNARROWSET         0x14
#define MDEVWIDESET           0x15
#define MTPLDPLNARROWSET      0x16
#define MTPLDPLWIDESET        0x17
#define APCADJUST             0x18
#define RDA1846PASET          0x1A
#define TVSET                 0x20
#define RSSITEST              0x21
#define ERRNUMRATTEST         0x22
#define VOLUMESET             0x23
#define SQLEVEL1OPENSET       0x24
#define SQLEVEL1CLOSESET      0x25
#define SQLEVEL9OPENSET       0x26
#define SQLEVEL9CLOSESET      0x27
#define CPUFREQADJUST         0x28
#define RSSILEVELSET          0x30
#define DIGITALRSSILEVELSET   0x31

//V
#define V_FREQCHEXKSET        0x90
#define V_HIGHPOWERSET        0x91
#define V_LOWPOWERSET         0x92
#define V_DMODBALANCESET      0x93
#define V_MDEVNARROWSET       0x94
#define V_MDEVWIDESET         0x95
#define V_MTPLDPLNARROWSET    0x96
#define V_MTPLDPLWIDESET      0x97
#define V_APCADJUST           0x98
#define V_RDA1846PASET        0x9A
#define V_TVSET               0xA0
#define V_RSSITEST            0xA1
#define V_ERRNUMRATTEST       0xA2
#define V_VOLUMESET           0xA3
#define V_SQLEVELSET          0xA4
#define V_SQLEVEL1CLOSESET    0xA5
#define V_SQLEVEL9OPENSET     0xA6
#define V_SQLEVEL9CLOSESET    0xA7
#define V_CPUFREQADJUST       0xA8
#define V_RSSILEVELSET        0xB0
#define V_DIGITALRSSILEVELSET 0xB1

#define MODEIQSET             0x41
#define FMRXDETECTSET         0x42
#define FMTXDETECTSET         0x43

//#define	C5000REGSET			0x48

#define RDA1846SREGSET        0x7f
#define AUDIOREGSET           0x7e
#define C5000REGSET           0x7d
#define P_FLASH_OPT           0x7c
#define FLASH_RW              0XFF

/******************************************************************************/

/******************************************************************************/
#define TUNERPARMTLEN         198

#if (FREQ_SCOPE_UV == FREQ_SCOPE_V)
#define PCTUNERADDR FLASH_PCTUNER_V_ADDR
#elif (FREQ_SCOPE_UV == FREQ_SCOPE_U)
#define PCTUNERADDR FLASH_PCTUNER_U_ADDR
#endif

#define TUNER_EQIPINFO  PCTUNERADDR
#define FLASH_PAGE_LEN  32
#define TUNER_FLASH_LEN (7 * FLASH_PAGE_LEN)

#if 1
#define TUNER_POWER_LEN  (1 + 8 * 2)

#define TUNER_HPOWERADDR (TUNER_EQIPINFO)

#define TUNER_LPOWERADDR (TUNER_HPOWERADDR + FLASH_PAGE_LEN)

#define TUNER_SQ_LEN     (1 + 8 * 18)
#define TUNER_SQWDBADDR  (TUNER_LPOWERADDR + FLASH_PAGE_LEN)

#else
#define TUNER_SQUNUM                       TUNER_EQIPINFO
#define TUNER_MODEL                        (TUNER_EQIPINFO + 6)
#define TUNER_DMRNUM                       (TUNER_EQIPINFO + 12)
#define TUNER_FREQL                        (TUNER_EQIPINFO + 20)
#define TUNER_FREQH                        (TUNER_EQIPINFO + 24)
#define TUNER_VERSION                      (TUNER_EQIPINFO + 28)
#define TUNER_CONSDATA                     (TUNER_EQIPINFO + 34)

#define TUNER_TVADDR                       (PCTUNERADDR + 44)

#define TUNER_TVSELECT                     TUNER_TVADDR

#define TUNER_TV403_075                    (TUNER_TVADDR + 1)
#define TUNER_TV414_075                    (TUNER_TVADDR + 3)
#define TUNER_TV425_075                    (TUNER_TVADDR + 5)
#define TUNER_TV436_525                    (TUNER_TVADDR + 7)
#define TUNER_TV447_875                    (TUNER_TVADDR + 9)
#define TUNER_TV458_875                    (TUNER_TVADDR + 11)
#define TUNER_TV469_825                    (TUNER_TVADDR + 13)
#define TUNER_TV478_825                    (TUNER_TVADDR + 15)

#define TUNER_VOLADDR                      (PCTUNERADDR + 61)

#define TUNER_TXRXADDR                     (PCTUNERADDR + 62)

#define TUNER_FREQCHECKSEL                 TUNER_TXRXADDR
#define TUNER_TX_AMP_BIAS                  (TUNER_TXRXADDR + 1)
#define TUNER_RX_AMP_BIAS                  (TUNER_TXRXADDR + 3)
#define TUNER_OFFSET                       (TUNER_TXRXADDR + 5)

#define TUNER_HPOWERADDR                   (PCTUNERADDR + 69)

#define TUNER_HPOWERSEL                    TUNER_HPOWERADDR
#define TUNER_HP403_175                    (TUNER_HPOWERSEL + 1)
#define TUNER_HP414_175                    (TUNER_HPOWERSEL + 3)
#define TUNER_HP425_175                    (TUNER_HPOWERSEL + 5)
#define TUNER_HP436_625                    (TUNER_HPOWERSEL + 7)
#define TUNER_HP447_775                    (TUNER_HPOWERSEL + 9)
#define TUNER_HP458_775                    (TUNER_HPOWERSEL + 11)
#define TUNER_HP469_825                    (TUNER_HPOWERSEL + 13)
#define TUNER_HP478_825                    (TUNER_HPOWERSEL + 15)

#define TUNER_LPOWERADDR                   (PCTUNERADDR + 86)

#define TUNER_LPOWERSEL                    TUNER_LPOWERADDR
#define TUNER_LP403_175                    (TUNER_LPOWERSEL + 1)
#define TUNER_LP414_175                    (TUNER_LPOWERSEL + 3)
#define TUNER_LP425_175                    (TUNER_LPOWERSEL + 5)
#define TUNER_LP436_625                    (TUNER_LPOWERSEL + 7)
#define TUNER_LP447_775                    (TUNER_LPOWERSEL + 9)
#define TUNER_LP458_775                    (TUNER_LPOWERSEL + 11)
#define TUNER_LP469_825                    (TUNER_LPOWERSEL + 13)
#define TUNER_LP478_825                    (TUNER_LPOWERSEL + 15)

#define TUNER_DMODBALADDR                  (PCTUNERADDR + 103)

#define TUNER_DMODBALSET                   TUNER_DMODBALADDR
#define TUNER_DMODBAL403                   (TUNER_DMODBALADDR + 1)
#define TUNER_DMODBAL410                   (TUNER_DMODBALADDR + 3)
#define TUNER_DMODBAL424                   (TUNER_DMODBALADDR + 5)
#define TUNER_DMODBAL436                   (TUNER_DMODBALADDR + 7)
#define TUNER_DMODBAL445                   (TUNER_DMODBALADDR + 9)
#define TUNER_DMODBAL459                   (TUNER_DMODBALADDR + 11)
#define TUNER_DMODBAL460                   (TUNER_DMODBALADDR + 13)
#define TUNER_DMODBAL470                   (TUNER_DMODBALADDR + 15)
#define TUNER_SENDSIGNAL                   (TUNER_DMODBALADDR + 17)

#define TUNER_MMODBALADDR                  (PCTUNERADDR + 121)

#define TUNER_MMODBALSET                   TUNER_MMODBALADDR
#define TUNER_MMODBAL403                   (TUNER_MMODBALADDR + 1)
#define TUNER_MMODBAL410                   (TUNER_MMODBALADDR + 3)
#define TUNER_MMODBAL424                   (TUNER_MMODBALADDR + 5)
#define TUNER_MMODBAL436                   (TUNER_MMODBALADDR + 7)
#define TUNER_MMODBAL445                   (TUNER_MMODBALADDR + 9)
#define TUNER_MMODBAL459                   (TUNER_MMODBALADDR + 11)
#define TUNER_MMODBAL460                   (TUNER_MMODBALADDR + 13)
#define TUNER_MMODBAL470                   (TUNER_MMODBALADDR + 15)

#define TUNER_MMODBALWIDEADDR              (PCTUNERADDR + 138)

#define TUNER_MMODBALWIDESET               TUNER_MMODBALWIDEADDR
#define TUNER_MMODBALWIDE403               (TUNER_MMODBALWIDEADDR + 1)
#define TUNER_MMODBALWIDE410               (TUNER_MMODBALWIDEADDR + 3)
#define TUNER_MMODBALWIDE424               (TUNER_MMODBALWIDEADDR + 5)
#define TUNER_MMODBALWIDE436               (TUNER_MMODBALWIDEADDR + 7)
#define TUNER_MMODBALWIDE445               (TUNER_MMODBALWIDEADDR + 9)
#define TUNER_MMODBALWIDE459               (TUNER_MMODBALWIDEADDR + 11)
#define TUNER_MMODBALWIDE460               (TUNER_MMODBALWIDEADDR + 13)
#define TUNER_MMODBALWIDE470               (TUNER_MMODBALWIDEADDR + 15)

#define TUNER_TPLDPLNARROWADDR             (PCTUNERADDR + 155)

#define TUNER_TPLDPLNARROWSET              TUNER_TPLDPLNARROWADDR
#define TUNER_TPLDPLNARROW403              (TUNER_TPLDPLNARROWADDR + 1)
#define TUNER_TPLDPLNARROW410              (TUNER_TPLDPLNARROWADDR + 2)
#define TUNER_TPLDPLNARROW424              (TUNER_TPLDPLNARROWADDR + 3)
#define TUNER_TPLDPLNARROW436              (TUNER_TPLDPLNARROWADDR + 4)
#define TUNER_TPLDPLNARROW445              (TUNER_TPLDPLNARROWADDR + 5)
#define TUNER_TPLDPLNARROW459              (TUNER_TPLDPLNARROWADDR + 6)
#define TUNER_TPLDPLNARROW460              (TUNER_TPLDPLNARROWADDR + 7)
#define TUNER_TPLDPLNARROW470              (TUNER_TPLDPLNARROWADDR + 8)

#define TUNER_TPLDPLWIDEADDR               (PCTUNERADDR + 164)

#define TUNER_TPLDPLWIDESET                TUNER_TPLDPLWIDEADDR
#define TUNER_TPLDPLWIDE403                (TUNER_TPLDPLWIDEADDR + 1)
#define TUNER_TPLDPLWIDE410                (TUNER_TPLDPLWIDEADDR + 2)
#define TUNER_TPLDPLWIDE424                (TUNER_TPLDPLWIDEADDR + 3)
#define TUNER_TPLDPLWIDE436                (TUNER_TPLDPLWIDEADDR + 4)
#define TUNER_TPLDPLWIDE445                (TUNER_TPLDPLWIDEADDR + 5)
#define TUNER_TPLDPLWIDE459                (TUNER_TPLDPLWIDEADDR + 6)
#define TUNER_TPLDPLWIDE460                (TUNER_TPLDPLWIDEADDR + 7)
#define TUNER_TPLDPLWIDE470                (TUNER_TPLDPLWIDEADDR + 8)

#define TUNER_CPUFREQADJUSTADDR            (PCTUNERADDR + 173)

#define TUNER_APCADJUSTADDR                (PCTUNERADDR + 174)
#define TUNER_APCADJUST                    TUNER_APCADJUSTADDR
#define TUNER_APC403_175                   (TUNER_APCADJUSTADDR + 1)
#define TUNER_APC414_175                   (TUNER_APCADJUSTADDR + 5)
#define TUNER_APC425_175                   (TUNER_APCADJUSTADDR + 9)
#define TUNER_APC436_625                   (TUNER_APCADJUSTADDR + 13)
#define TUNER_APC447_775                   (TUNER_APCADJUSTADDR + 17)
#define TUNER_APC458_775                   (TUNER_APCADJUSTADDR + 21)
#define TUNER_APC469_825                   (TUNER_APCADJUSTADDR + 25)
#define TUNER_APC478_825                   (TUNER_APCADJUSTADDR + 29)

#define TUNER_SQWDBADDR                    (PCTUNERADDR + 207)
#define TUNER_SQL403_175                   TUNER_SQWDBADDR + 1
#define TUNER_SQL414_175                   (TUNER_SQWDBADDR + 19)
#define TUNER_SQL425_175                   (TUNER_SQWDBADDR + 37)
#define TUNER_SQL436_625                   (TUNER_SQWDBADDR + 55)
#define TUNER_SQL447_775                   (TUNER_SQWDBADDR + 73)
#define TUNER_SQL458_775                   (TUNER_SQWDBADDR + 91)
#define TUNER_SQL469_825                   (TUNER_SQWDBADDR + 109)
#define TUNER_SQL478_825                   (TUNER_SQWDBADDR + 127)

#define TUNER_RSSIBADDR                    (PCTUNERADDR + 352)
#define RSSI400To410                       (TUNER_RSSIBADDR + 1)
#define RSSI410To420                       (TUNER_RSSIBADDR + 11)
#define RSSI420To430                       (TUNER_RSSIBADDR + 21)
#define RSSI430To440                       (TUNER_RSSIBADDR + 31)
#define RSSI440To450                       (TUNER_RSSIBADDR + 41)
#define RSSI450To460                       (TUNER_RSSIBADDR + 51)
#define RSSI460To470                       (TUNER_RSSIBADDR + 61)
#define RSSI470To480                       (TUNER_RSSIBADDR + 71)

#define TUNER_FMRSSIBADDR                  TUNER_RSSIBADDR + 81
#define FMRSSI400To410                     (TUNER_FMRSSIBADDR + 1)
#define FMRSSI410To420                     (TUNER_FMRSSIBADDR + 11)
#define FMRSSI420To430                     (TUNER_FMRSSIBADDR + 21)
#define FMRSSI430To440                     (TUNER_FMRSSIBADDR + 31)
#define FMRSSI440To450                     (TUNER_FMRSSIBADDR + 41)
#define FMRSSI450To460                     (TUNER_FMRSSIBADDR + 51)
#define FMRSSI460To470                     (TUNER_FMRSSIBADDR + 61)
#define FMRSSI470To480                     (TUNER_FMRSSIBADDR + 71)

#define TUNER_MODEIQSET                    (TUNER_FMRSSIBADDR + 81)

#define TUNER_FMRXDETECTSET                (TUNER_MODEIQSET + 4)

#define TUNER_TUNER_FMRXDETECTSETNARROW    TUNER_FMRXDETECTSET
#define TUNER_TUNER_FMRXDETECTSETWIDE      (TUNER_FMRXDETECTSET + 1)

#define TUNER_FMTXDETECTSET                (TUNER_FMRXDETECTSET + 2)

#define TUNER_TUNER_FMTXDETECTSETNARROW    TUNER_FMTXDETECTSET
#define TUNER_TUNER_FMTXLIMITSETNARROW     (TUNER_FMTXDETECTSET + 1)
#define TUNER_TUNER_FMTXSUBDETECTSETNARROW (TUNER_FMTXDETECTSET + 2)
#define TUNER_TUNER_FMTXDETECTSETWIDE      (TUNER_FMTXDETECTSET + 3)
#define TUNER_TUNER_FMTXLIMITSETWIDE       (TUNER_FMTXDETECTSET + 4)
#define TUNER_TUNER_FMTXSUBDETECTSETWIDE   (TUNER_FMTXDETECTSET + 5)

#pragma pack(1)
typedef struct
{
    uint8 Flg;
    uint16 Txampbias;
    uint16 Rxampbias;
    uint16 Offset;
    uint8 Toggle;
} STR_FRQCHECK;
#pragma pack()

#pragma pack(1)
typedef struct
{
    uint8 Flg;
    uint16 Hp403175;
    uint16 Hp414175;
    uint16 Hp425175;
    uint16 Hp436625;
    uint16 Hp447775;
    uint16 Hp458775;
    uint16 Hp469825;
    uint8 Toggle;
} STR_HPOWER;
#pragma pack()

#pragma pack(1)
typedef struct
{
    uint8 Flg;
    uint16 Lp403175;
    uint16 Lp414175;
    uint16 Lp425175;
    uint16 Lp436625;
    uint16 Lp447775;
    uint16 Lp458775;
    uint16 Lp469825;
    uint8 Toggle;
} STR_LPOWER;
#pragma pack()

#pragma pack(1)
typedef struct
{
    uint8 Flg;
    uint16 BAL403;
    uint16 BAL410;
    uint16 BAL424;
    uint16 BAL436;
    uint16 BAL445;
    uint16 BAL459;
    uint16 BAL460;
    uint16 BAL470;
    uint8 Toggle;
} STR_DMODBAL;
#pragma pack()

#pragma pack(1)
typedef struct
{
    uint8 Flg;
    uint16 BAL403;
    uint16 BAL410;
    uint16 BAL424;
    uint16 BAL436;
    uint16 BAL445;
    uint16 BAL459;
    uint16 BAL460;
    uint16 BAL470;
    uint8 Toggle;
} STR_MMODBAL;
#pragma pack()

#pragma pack(1)
typedef struct
{
    uint8 Flg;
    uint16 BAL403075;
    uint16 BAL414075;
    uint16 BAL425075;
    uint16 BAL436525;
    uint16 BAL447825;
    uint16 BAL458875;
    uint16 BAL469925;
    uint8 Toggle;
} STR_TV;
#pragma pack()
#endif
extern uint8 Sq_flag;
extern uint8 PcTunerFlg;
extern uint8 g_PcTunerFeqSelect;

DH_VOID PC_BLL_PcTunerRecData(DH_U8 *RecBuf, DH_U32 HeadLen);
DH_VOID PC_BLL_PcToMcuTuner(DH_U8 *Buf, DH_U16 Len, DH_U8 err);
DH_VOID PC_BLL_McuToPcTuner(DH_U8 *Buf, DH_U16 Len, DH_U8 err);
DH_VOID PC_BLL_PcFreqCheckSet(DH_U8 *Buf, DH_U16 Len, DH_U8 err);
DH_VOID PC_BLL_PcHighPowerSet(DH_U8 *Buf, DH_U16 Len, DH_U8 err);
DH_VOID PC_BLL_PcLowPowerSet(DH_U8 *Buf, DH_U16 Len, DH_U8 err);
DH_VOID PC_BLL_PcDModBalanceSet(DH_U8 *Buf, DH_U16 Len, DH_U8 err);
DH_VOID PC_BLL_PcMDevNarrowSet(DH_U8 *Buf, DH_U16 Len, DH_U8 err);
DH_VOID PC_BLL_PcMDevWideSet(DH_U8 *Buf, DH_U16 Len, DH_U8 err);
DH_VOID PC_BLL_PcMTPLDPLNarrowSet(DH_U8 *Buf, DH_U16 Len, DH_U8 err);
DH_VOID PC_BLL_PcMTPLDPLWideSet(DH_U8 *Buf, DH_U16 Len, DH_U8 err);
DH_VOID PC_BLL_PcTvSet(DH_U8 *Buf, DH_U16 Len, DH_U8 err);
DH_VOID PC_BLL_PcReadRssi(DH_U8 *Buf, DH_U16 Len, DH_U8 err);
DH_VOID PC_BLL_PcReadErrNumRat(DH_U8 *Buf, DH_U16 Len, DH_U8 err);
DH_VOID PC_BLL_PcVolumSet(DH_U8 *Buf, DH_U16 Len, DH_U8 err);
DH_VOID PC_BLL_PcSQLevel1OpenSet(DH_U8 *Buf, DH_U16 Len, DH_U8 err);
DH_VOID PC_BLL_PcSQLevel1CloseSet(DH_U8 *Buf, DH_U16 Len, DH_U8 err);
DH_VOID PC_BLL_PcSQLevel9OpenSet(DH_U8 *Buf, DH_U16 Len, DH_U8 err);
DH_VOID PC_BLL_PcSQLevel9CloseSet(DH_U8 *Buf, DH_U16 Len, DH_U8 err);
DH_VOID PC_BLL_CPUFreqAdjust(DH_U8 *Buf, DH_U16 Len, DH_U8 err);
DH_VOID PC_BLL_TunerMFToPc(DH_U8 *DstBuf, DH_U8 *SourBuf, DH_U16 Len, DH_U8 Opt,
                           DH_U8 Ack);
DH_U8 PC_BLL_TunerGetPcFrm(DH_U8 *DstBuf, DH_U16 *PLen, DH_U32 TimeOut,
                           DH_U32 HeadLen);
DH_VOID PC_BLL_ApcAdjustSet(DH_U8 *Buf, DH_U8 Len, DH_U8 err);
DH_VOID PC_BLL_SqAdjustSet(DH_U8 *Buf, DH_U8 Len, DH_U8 err);
DH_VOID PC_BLL_DrvRssiAdjustSet(DH_U8 *Buf, DH_U8 Len, DH_U8 err);
DH_VOID PC_BLL_FmRSSIAdjustSet(DH_U8 *Buf, DH_U8 Len, DH_U8 err);
DH_VOID PC_BLL_RSSIAdjustSet(DH_U8 *Buf, DH_U8 Len, DH_U8 err);
DH_VOID PC_BLL_Digital_RSSISet(DH_U8 *Buf, DH_U8 Len, DH_U8 err);
DH_VOID PC_BLL_Pc1846PaOutSet(DH_U8 *Buf, DH_U8 Len, DH_U8 err);

DH_VOID PC_BLL_PcOption1846S(DH_U8 *RecBuf);
DH_VOID PC_BLL_PcOptionC5000S(DH_U8 *RecBuf);
DH_VOID PC_BLL_PcOptionFlash(DH_U8 *RecBuf);

DH_VOID PC_BLL_ModeIQSet(DH_U8 *Buf, DH_U8 Len, DH_U8 err);
DH_VOID PC_BLL_FMRxDetectset(DH_U8 *Buf, DH_U8 Len, DH_U8 err);
DH_VOID PC_BLL_FMTxDetectset(DH_U8 *Buf, DH_U8 Len, DH_U8 err);
DH_VOID PC_BLL_PcOptionGPSmsg(DH_U8 *Buf);

DH_VOID PC_BLL_PcAfcAutCreate(DH_U8 *Buf, DH_U8 *Ampbias, DH_U16 *Afc);
#endif

#endif
