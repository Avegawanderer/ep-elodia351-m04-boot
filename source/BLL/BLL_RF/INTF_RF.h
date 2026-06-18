/*******************************************************************************
 * Copyright (C) 2011-2013 ZheJiang Dahua Technology CO.,LTD.
 *
 * 文件名 : INTF_RF.h
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

#ifndef _INTF_RF_H_
#define _INTF_RF_H_

/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include "dh_type.h"
#include "BLL_RF.h"
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* ========================================================================== */
/*                           宏和类型定义区                                   */
/* ========================================================================== */
#define RF1_APC_CHAN    0
#define RF1_TV_CHAN     1
#define RF2_APC_CHAN    2
#define RF2_TV_CHAN     3

#define RF_APC_CHN DAC_CHN_B
#define RX_APC_CHN DAC_CHN_A
#define RF_BIAS_CHN DAC_CHN_

/* ========================================================================== */
/*                          数据结构定义区                                    */
/* ========================================================================== */

/* ========================================================================== */
/*                          函数声明区                                        */
/* ========================================================================== */
DH_VOID RF_INTF_GPIOInit(DH_VOID);
DH_VOID RF_INTF_DlySMU(DH_U32 u32Secs, DH_U32 u32Ms, DH_U32 u32Us);

DH_VOID RF_INTF_APCChange(DH_U16 u16Target, DH_U16 u16Src);
DH_VOID RF_INTF_APCSet(DH_U16 u16Val);
DH_VOID RF_INTF_TVSet(DH_U16 u16Val);
DH_VOID RF_INTF_BIASSet(DH_U16 u16Val);

DH_VOID RF_INTF_SetTx(DH_U32 u32Freq);
DH_VOID RF_INTF_SetRx(DH_U32 u32Freq);

DH_U16 RF_INTF_GetSignal(DH_VOID);
DH_U8  RF_INTF_GetRssiLevelStat(DH_VOID);

DH_VOID RF_INTF_SetTxAmpbias(DH_U16 Ampbias);
DH_VOID RF_INTF_SetRxAmpbias(DH_U16 Ampbias);
DH_VOID RF_INTF_AdjIQAmp(DH_U16 mod);
DH_VOID RF_INTF_SetTowPointBias(DH_U16 afc);
DH_VOID RF_INTF_SetIQAmpNBias(PLL_INFO_S *str);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

