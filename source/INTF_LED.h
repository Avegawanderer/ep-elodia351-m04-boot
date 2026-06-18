/*******************************************************************************
 * Copyright (C) 2011-2013 ZheJiang Dahua Technology CO.,LTD.
 *
 * 文件名 : INTF_LED.h
 *
 * 作  者 : 123(ggg)<gg>
 * 版  本 : V1.0.0
 * 日  期 : 2015年5月4日 Create
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

#ifndef _INTF_LED_H_
#define _INTF_LED_H_

/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
 #include "dh_type.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* ========================================================================== */
/*                           宏和类型定义区                                   */
/* ========================================================================== */

#define FUNC_LEDDISP_TYPE 1

/* LED0 Port/Pin definition */
#define  LED_GREEN_PORT          (PortD)
#define  LED_GREEN_PIN           (Pin02)

/* LED1 Port/Pin definition */
#define  LED_RED_PORT          (PortB)
#define  LED_RED_PIN           (Pin03)


#define GREEN_LED_ON    PORT_SetBits(LED_GREEN_PORT, LED_GREEN_PIN)
#define GREEN_LED_OFF   PORT_ResetBits(LED_GREEN_PORT, LED_GREEN_PIN)
#define GREEN_LED_OFF_   PORT_ResetBits(LED_GREEN_PORT, LED_GREEN_PIN)



#define RED_LED_ON      PORT_SetBits(LED_RED_PORT, LED_RED_PIN)
#define RED_LED_OFF      PORT_ResetBits(LED_RED_PORT, LED_RED_PIN)
#define RED_LED_OFF_     PORT_ResetBits(LED_RED_PORT, LED_RED_PIN)



#define LED_FM_ON       GREEN_LED_ON
#define LED_FM_OFF      GREEN_LED_OFF

#define LED_DMR_ON      RED_LED_ON
#define LED_DMR_OFF     RED_LED_OFF

#define LED_HP_ON      RED_LED_ON
#define LED_HP_OFF     RED_LED_OFF
#define LED_LP_ON      GREEN_LED_ON
#define LED_LP_OFF     GREEN_LED_OFF



/* ========================================================================== */
/*                          数据结构定义区                                    */
/* ========================================================================== */
enum{
	E_LED_STAT_IDLE,
	E_LED_STAT_KeyOpt,
	E_LED_STAT_Signal,
	E_LED_STAT_Rec,
	E_LED_STAT_Ptt
};

/* ========================================================================== */
/*                          函数声明区                                        */
/* ========================================================================== */
DH_VOID LED_INTF_Init(DH_VOID);
DH_VOID  LED_INTF_SetBrightness(DH_U32 u32Ration);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

