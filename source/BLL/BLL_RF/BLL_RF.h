/*******************************************************************************
 * Copyright (C) 2011-2013 ZheJiang Dahua Technology CO.,LTD.
 *
 * 文件名 : BLL_RF.h
 *
 * 作  者 : 123(ggg)<gg>
 * 版  本 : V1.0.0
 * 日  期 : 2015年4月14日 Create
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

#ifndef __BLL_RF_H__
#define __BLL_RF_H__


/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */

#include "dh_type.h"

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */


/* ========================================================================== */
/*                           宏和类型定义区                                   */
/* ========================================================================== */
//extern OS_SEM RfLock_Sem;

//10	PC2/ADC1_IN12		RX_POW_EN	接收电源控制端口	H有效	
#define  RX_POW_EN_PORT         (PortC)
#define  RX_POW_EN_PIN          (Pin02)

//11	PC3/ADC1_IN13		VCO_POW_EN	PLL电源控制端口	H有效	
#define  VCO_POW_EN_PORT        (PortC)
#define  VCO_POW_EN_PIN         (Pin03)

//35	PB14		        TX_POW_EN	发射电源控制端口	H有效	
#define  TX_POW_EN_PORT         (PortB)
#define  TX_POW_EN_PIN          (Pin14)

//PC4/ADC1_IN14		N/W_SET	宽/窄帯工作模式控制端口	L=W;H=N	
#define  NW_SET_PORT           (PortC)
#define  NW_SET_PIN            (Pin04)


#define  RF_RX_EN_HIGH_EN PORT_SetBits(RX_POW_EN_PORT, RX_POW_EN_PIN)
#define  RF_RX_EN_LOW_DIS PORT_ResetBits(RX_POW_EN_PORT, RX_POW_EN_PIN)


#define  RF_TX_EN_HIGH_EN PORT_SetBits(TX_POW_EN_PORT, TX_POW_EN_PIN)
#define  RF_TX_EN_LOW_DIS PORT_ResetBits(TX_POW_EN_PORT, TX_POW_EN_PIN)



#define  RF_VCO_POW_EN PORT_SetBits(VCO_POW_EN_PORT, VCO_POW_EN_PIN)
#define  RF_VCO_POW_DIS PORT_ResetBits(VCO_POW_EN_PORT, VCO_POW_EN_PIN)


#define RF_LOCK      //dhsem_wait(&RfLock_Sem, 0)
#define RF_UNLOCK    //dhsem_post(&RfLock_Sem)

#define FREQ_UPDOWN	45000000


/* RSSI门限宏定义 */

 #define RF_RSSI_LEVEL           200
	

/* 高低功率定义 */
//#define   POWER_HIGH        1
//#define   POWER_LOW         2

/* 信道宏定义 */
#define   CHNSTAT_NONE      0
#define   CHNSTAT_FM        1
#define   CHNSTAT_DMR       2
#define   CHNSTAT_DFM       3

/* ========================================================================== */
/*                          数据结构定义区                                    */
/* ========================================================================== */
typedef struct {
/* 发射频率 */
DH_U32 u32TxFreq;
/* 接收频率 */
DH_U32 u32RxFreq;
///* 接收频率校准 */
//DH_U16 u16AfcVal;
///* 发射频率校准 */
//DH_U16 u16AmpbiasVal;
/* 功率值 */
DH_U16 u16ApcVal;
/* 低功率 */
DH_U16 u16ApcLowVal;
/* 高功率 */
DH_U16 u16ApcHighVal;
/* I路调制值 */
//DH_U8  u8Mod1;
/* Q路调制值 */
//DH_U8  u8Mod2;
/* 电调值 */
DH_U16 u16TvVal;
///* APC上升沿时间 */
//DH_U16 u16APCUpTime;
///* APC下降沿时间 */
//DH_U16 u16APCDownTime;
///* 1846输出功率 */
//DH_U8 u8RDA1846sPower;
}PLL_INFO_S;

typedef struct {
DH_U16 u16H;
DH_U16 u16M;
DH_U16 u16L;
}POWER_S;

typedef struct {
DH_U16 u16Tx;
DH_U16 u16Rx;
}BIAS_S;

typedef struct {
DH_U8 u8Mod1;
DH_U8 u8Mod2;
}BALANCE_S;



extern const DH_U32 g_u32FreqSectorList[] ;
extern const DH_U32 g_u32FreqIntSectorList[];

/* ========================================================================== */
/*                          函数声明区                                        */
/* ========================================================================== */
DH_VOID RF_BLL_SavePll(PLL_INFO_S str);
PLL_INFO_S RF_BLL_GetPll(DH_VOID);
PLL_INFO_S* RF_BLL_GetPllStrP(DH_VOID);
DH_VOID RF_BLL_Process(void *p_arg);
DH_U8 RF_BLL_Init(DH_VOID);
PLL_INFO_S PLL_BLL_GetRfVal( DH_U32 u32TxFreq, DH_U32 u32RxFreq);
DH_VOID RF_BLL_SavePll(PLL_INFO_S str);
DH_VOID RF_BLL_RxInit(PLL_INFO_S *str);
DH_VOID RF_BLL_EnableRx(DH_VOID);
DH_VOID RF_BLL_EnableTx(DH_VOID);
DH_VOID RF_BLL_SetPower(DH_U8 u8Flg);
DH_U8 RF_BLL_GetLimit(DH_VOID);
DH_VOID RF_BLL_GetLoFreq(DH_U32 *u32Freq);
DH_U8 RF_BLL_GetLoDir(DH_VOID);
	DH_VOID  RF_INTF_APCSet(DH_U16 u16Val);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif

