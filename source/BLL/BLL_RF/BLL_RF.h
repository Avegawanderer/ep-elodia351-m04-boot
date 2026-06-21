

#ifndef __BLL_RF_H__
#define __BLL_RF_H__

/* ========================================================================== */

/* ========================================================================== */

#include "dh_type.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

    /* ========================================================================== */

    /* ========================================================================== */
    //extern OS_SEM RfLock_Sem;

#define RX_POW_EN_PORT   (PortC)
#define RX_POW_EN_PIN    (Pin02)

#define VCO_POW_EN_PORT  (PortC)
#define VCO_POW_EN_PIN   (Pin03)

#define TX_POW_EN_PORT   (PortB)
#define TX_POW_EN_PIN    (Pin14)

#define NW_SET_PORT      (PortC)
#define NW_SET_PIN       (Pin04)

#define RF_RX_EN_HIGH_EN PORT_SetBits(RX_POW_EN_PORT, RX_POW_EN_PIN)
#define RF_RX_EN_LOW_DIS PORT_ResetBits(RX_POW_EN_PORT, RX_POW_EN_PIN)

#define RF_TX_EN_HIGH_EN PORT_SetBits(TX_POW_EN_PORT, TX_POW_EN_PIN)
#define RF_TX_EN_LOW_DIS PORT_ResetBits(TX_POW_EN_PORT, TX_POW_EN_PIN)

#define RF_VCO_POW_EN    PORT_SetBits(VCO_POW_EN_PORT, VCO_POW_EN_PIN)
#define RF_VCO_POW_DIS   PORT_ResetBits(VCO_POW_EN_PORT, VCO_POW_EN_PIN)

#define RF_LOCK      //dhsem_wait(&RfLock_Sem, 0)
#define RF_UNLOCK    //dhsem_post(&RfLock_Sem)

#define FREQ_UPDOWN   45000000

#define RF_RSSI_LEVEL 200

    //#define   POWER_HIGH        1
    //#define   POWER_LOW         2

#define CHNSTAT_NONE  0
#define CHNSTAT_FM    1
#define CHNSTAT_DMR   2
#define CHNSTAT_DFM   3

    /* ========================================================================== */

    /* ========================================================================== */
    typedef struct
    {

        DH_U32 u32TxFreq;

        DH_U32 u32RxFreq;

        //DH_U16 u16AfcVal;

        //DH_U16 u16AmpbiasVal;

        DH_U16 u16ApcVal;

        DH_U16 u16ApcLowVal;

        DH_U16 u16ApcHighVal;

        //DH_U8  u8Mod1;

        //DH_U8  u8Mod2;

        DH_U16 u16TvVal;

        //DH_U16 u16APCUpTime;

        //DH_U16 u16APCDownTime;

        //DH_U8 u8RDA1846sPower;
    } PLL_INFO_S;

    typedef struct
    {
        DH_U16 u16H;
        DH_U16 u16M;
        DH_U16 u16L;
    } POWER_S;

    typedef struct
    {
        DH_U16 u16Tx;
        DH_U16 u16Rx;
    } BIAS_S;

    typedef struct
    {
        DH_U8 u8Mod1;
        DH_U8 u8Mod2;
    } BALANCE_S;

    extern const DH_U32 g_u32FreqSectorList[];
    extern const DH_U32 g_u32FreqIntSectorList[];

    /* ========================================================================== */

    /* ========================================================================== */
    DH_VOID RF_BLL_SavePll(PLL_INFO_S str);
    PLL_INFO_S RF_BLL_GetPll(DH_VOID);
    PLL_INFO_S *RF_BLL_GetPllStrP(DH_VOID);
    DH_VOID RF_BLL_Process(void *p_arg);
    DH_U8 RF_BLL_Init(DH_VOID);
    PLL_INFO_S PLL_BLL_GetRfVal(DH_U32 u32TxFreq, DH_U32 u32RxFreq);
    DH_VOID RF_BLL_SavePll(PLL_INFO_S str);
    DH_VOID RF_BLL_RxInit(PLL_INFO_S *str);
    DH_VOID RF_BLL_EnableRx(DH_VOID);
    DH_VOID RF_BLL_EnableTx(DH_VOID);
    DH_VOID RF_BLL_SetPower(DH_U8 u8Flg);
    DH_U8 RF_BLL_GetLimit(DH_VOID);
    DH_VOID RF_BLL_GetLoFreq(DH_U32 *u32Freq);
    DH_U8 RF_BLL_GetLoDir(DH_VOID);
    DH_VOID RF_INTF_APCSet(DH_U16 u16Val);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
