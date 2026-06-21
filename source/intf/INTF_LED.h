

#ifndef _INTF_LED_H_
#define _INTF_LED_H_

/* ========================================================================== */

/* ========================================================================== */
#include "dh_type.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

    /* ========================================================================== */

    /* ========================================================================== */

#define FUNC_LEDDISP_TYPE 1

/* LED0 Port/Pin definition */
#define LED_GREEN_PORT    (PortD)
#define LED_GREEN_PIN     (Pin02)

/* LED1 Port/Pin definition */
#define LED_RED_PORT      (PortB)
#define LED_RED_PIN       (Pin03)

#define GREEN_LED_ON      PORT_SetBits(LED_GREEN_PORT, LED_GREEN_PIN)
#define GREEN_LED_OFF     PORT_ResetBits(LED_GREEN_PORT, LED_GREEN_PIN)
#define GREEN_LED_OFF_    PORT_ResetBits(LED_GREEN_PORT, LED_GREEN_PIN)

#define RED_LED_ON        PORT_SetBits(LED_RED_PORT, LED_RED_PIN)
#define RED_LED_OFF       PORT_ResetBits(LED_RED_PORT, LED_RED_PIN)
#define RED_LED_OFF_      PORT_ResetBits(LED_RED_PORT, LED_RED_PIN)

#define LED_FM_ON         GREEN_LED_ON
#define LED_FM_OFF        GREEN_LED_OFF

#define LED_DMR_ON        RED_LED_ON
#define LED_DMR_OFF       RED_LED_OFF

#define LED_HP_ON         RED_LED_ON
#define LED_HP_OFF        RED_LED_OFF
#define LED_LP_ON         GREEN_LED_ON
#define LED_LP_OFF        GREEN_LED_OFF

    /* ========================================================================== */

    /* ========================================================================== */
    enum
    {
        E_LED_STAT_IDLE,
        E_LED_STAT_KeyOpt,
        E_LED_STAT_Signal,
        E_LED_STAT_Rec,
        E_LED_STAT_Ptt
    };

    /* ========================================================================== */

    /* ========================================================================== */
    DH_VOID LED_INTF_Init(DH_VOID);
    DH_VOID LED_INTF_SetBrightness(DH_U32 u32Ration);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
