#ifndef __APP_CONFIG_H
#define __APP_CONFIG_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "SysCfgMac.h"

#define SOFT_RELEASE 1

#if SOFT_RELEASE

#define DEBUG 0

#else
#define DEBUG 1

#endif

    /***********************************************************************/

#define FM_RF_IF             0
#define FM_RF_AF             1
#define FM_RF_IF_AF          2

#define FM_LINK_HRCx000      1
#define FM_LINK_FM_IC        0
#define FM_LINK_CMX138       2

#define FM_DECODE_SOFTWARE   0
#define FM_DECODE_HARDWARE   1
#define NEW_DECODE_TYEP_ADC  2

#define FM_IC_RDA1846S       0
#define FM_IC_W12T           1

#define W12T_FD4818          0
#define W12T_FD6818          1

#define FMIC_RDA1846S        0
#define FMIC_AT1846S         1 /*xin*/

#define SQ_DET_31136         0
#define SQ_DET_HRCx000       1
#define SQ_DET_FMIC          2

#define POWERON_PRESS        0
#define POWERON_ROLLEN       1

#define CHN_CODESW_NONE      0

#define CHN_CODESW_16MAX     1

#define CHN_CODESW_360D      2

#define MENU_DISP_NONE       0
#define MENU_DISP_LED        1
#define MENU_DISP_LCD        2

#define BOARD_WITHOUTKEY     0
#define BOARD_WITHKEY        1
#define BOARD_WITH12KEY      2

#define POWER_SUPPLY_72V     0    //7.2V
#define POWER_SUPPLY_42V     1    //4.2V

#define USE_DISP_LCDNONE     0
#define USE_DISP_LCD19264    1
#define USE_DISP_LCD_ST7735  2
#define USE_DISP_LCD13232    3

#define VOLUME_CTL_TYPE_SOFT 0
#define VOLUME_CTL_TYPE_HARD 1

#if DEV_TYPE_SEL == DEV_TYPE_DRF24_EN
#define DEV_NAME        "DR-F24"

#define LANGUAGE_SEL    LANGUAGE_RU
#define FREQ_SCOPE_SET  FREQ_136_174

#define FM_RF_SEL       FM_RF_IF
#define FM_IC_SEL       0xff

#define FM_IC_SEL_MODEL FMIC_RDA1846S

#if FM_RF_SEL == FM_RF_IF
#define FM_LINK_SEL   FM_LINK_HRCx000

#define SQ_DET_SEL    SQ_DET_31136
#define FM_DECODE_SEL NEW_DECODE_TYEP_ADC

#else
#define FM_LINK_SEL   FM_LINK_FM_IC

#define FM_DECODE_SEL FM_DECODE_SOFTWARE
#endif

#define DMR_RF_SEL             FM_RF_AF

#define REC_CHN_SEL            FM_RF_AF

#define POWER_SUPPLY_SEL       POWER_SUPPLY_42V

#define USE_BOARD_SEL          BOARD_WITHKEY

#define USE_POWERON_SEL        POWERON_PRESS
#define FUNC_USE_PWR_KEY       1

#define USE_SPIN_VOLUME        0

#define USE_CHN_CODESW         CHN_CODESW_16MAX

#define USE_SIMUCODESW         0

#define FUNC_MENU_DISP         MENU_DISP_NONE
#define USE_LCD_DISP_SEL       USE_DISP_LCD19264

#define USE_KEYPAD_ALL         1
#define USE_UART_SIGNL_LINE    0

#define FUNC_LEDLIGHT          0

#define FUNC_DMR_N_FM          0
#define FUNC_RECORD            1
#define FUNC_RECORD_FM         0
#define FUNC_FMCALLDISP        0
#define FUNC_RDA5802           0
#define FUNC_GPS               0
#define FUNC_FREQALL           0
#define FUNC_CHNEDIT           0
#define FUNC_CHNSWQIT          0
#define FUNC_ZONE_ADD          0
#define FUNC_SKIP_EMPTY_CHN    1

#define FUNC_USER_ENCKEY       1

#define FUNC_ZONE_CHN_SEL      1 /*0:16chn-128zone;1:250chn-4zone*/

#define FUNC_FM_SUB_NO_FSREC   0

#define FUNC_FM_SUB_MENUDISP   1

#define FUNC_FM_CTCSS_38_51    0

#define FUNC_RADIO_ID_EDIT_EN  1

#define FUNC_RX_GRP_INCLUDE_LM 0

#define FUNC_SLOT_RXTX         0

#define FUNC_REPEAT            0

#define FUNC_REPEATAB          0

#define FUNC_VIBRATION         1

#define FUNC_ROAM              0 /**/
#define FUNC_DTMF              1
#define FUNC_GYRO              0

#define FUNC_LONEWORKER        0 /**/
#define FUNC_CTCSSPHASETAIL    0 /**/
#endif

#define BSP_ALARM_FREQUENCY    500

#define TASKKEY_STK_SIZE       256 /*used:87*/
#define TASKCODESW_STK_SIZE    384 /*used:220*/
#define TASKPC_STK_SIZE        256 /*used:52*/
#define TASKRF_STK_SIZE        256 /*used:103*/
#define TASKSYS_STK_SIZE       256 /*used:134*/
#define TASKTXSLOT_STK_SIZE    256 /*used:101*/
#define TASKRXSLOT_STK_SIZE    256 /*used:49*/

#define TASKRXCSBK_STK_SIZE    256 /*used:62*/
#define TASKRXVOICE_STK_SIZE   256 /*used:67*/
#define TASKTXMSG_STK_SIZE     256 /*used:122*/
#define TASKRXMSG_STK_SIZE     256 /*used:69*/

#define TASKCPCHAN_STK_SIZE    256
#define TASKENCRYPT_STK_SIZE   256
#define TASKDECRYPT_STK_SIZE   256

#define TASKINIT_STK_SIZE      768 /*used:532, free:236*/
#define TASKTMRHND_STK_SIZE    512 /*used:73*/
#define TASKINPUTMTHD_STK_SIZE 128 /*used:51*/
#define TASKMENUDISP_STK_SIZE  256 /*used:134*/

#define TASKMENU_STK_SIZE      512 /*used:125*/
#define TASKMENUKEY_STK_SIZE   768 /*used:214*/
#define TASKSCANDISP_STK_SIZE  512 /*used:121*/
#define TASKSLP_STK_SIZE       256 /*used:*/
#define TASKCALLPTT_STK_SIZE   256 /*used:62*/
#define TASKCALLRX_STK_SIZE    256 /*used:128*/
#define TASKCSBK_STK_SIZE      256 /*used:56*/
#define TASKMSG_STK_SIZE       512 /*used:326*/
#define TASKSHUTDOWN_STK_SIZE  128

#define TASKVOX_STK_SIZE       340

#define TASKENCODER_STK_SIZE   1480 /*used:984*/
#define TASKDECODER_STK_SIZE   768  /*used:578*/

#define TASKSOUND_STK_SIZE     256

#define TASKTXRECORD_STK_SIZE  256
#define TASKRXRECORD_STK_SIZE  256

#define TASK_FMREC_STK_SIZE_   384

#define TASK_Agc_STK_SIZE_     256
#define TASKIDLE_STK_SIZE      300
#define TASKGPS_STK_SIZE       512
#define TASK_RDA5802_STK_SIZE  256
#define TASKLCDDISP_STK_SIZE   256

    typedef enum
    {

        TASKSHUTDOWN_PRIO = 10,

        TASKMENUDISP_PRIO,
        TASKCODESW_PRIO,
        TASKKEY_PRIO,
        //TASKCALLPTT_PRIO,
        TASKPC_PRIO,

        TASKRF_PRIO,
        TASKTXSLOT_PRIO,
        TASKRXSLOT_PRIO,
        TASKSYS_PRIO,
        TASKDECRYPT_PRIO,

        TASKENCRYPT_PRIO,
        TASKDECODER_PRIO,
        TASKENCODER_PRIO,
        TASKRXCSBK_PRIO,
        TASKRXVOICE_PRIO,

        TASKTXMSG_PRIO,
        TASKRXMSG_PRIO,

        //	TASKSOUND_PRIO,                    //
        TASKCALLRX_PRIO,

        TASKINIT_PRIO,
        TASKCPCHAN_PRIO,
        TASKTMRHND_PRIO,

        TASKINPUTMTHD_PRIO,
        TASKVOX_PRIO,
        //	TASKMENUDISP_PRIO,
        TASKMENUKEY_PRIO,
        TASKMENU_PRIO,

        TASKSCANDISP_PRIO,
        TASKSLP_PRIO,

        TASKCALLPTT_PRIO,
        TASKSOUND_PRIO,
        TASKLCDDISP_PRIO,

        //	TASKCALLRX_PRIO,
        TASKCSBK_PRIO,
        TASKMSG_PRIO,

        TASKFMRec_PRIO_,
        TASKRDA5802_PRIO,
        TASKAgc_PRIO,

        TASKTXRECORD_PRIO,
        TASKRXRECORD_PRIO,

        TASKGPS_PRIO,

        TASKIDLE_PRIO
    } TASK_PRIO_E;

#if 0
#define FREQ_SCOPE "F100-600 "
#define FREQ_UHF_L 100000000UL
#define FREQ_UHF_H 600000000UL
#define FREQ_UHF_E 400000000UL
#elif (FREQ_SCOPE_SET == FREQ_136_174)
#define FREQ_SCOPE "F136-174 "
#define FREQ_UHF_L 136000000UL
#define FREQ_UHF_H 174000000UL
#define FREQ_UHF_E 155000000UL
#elif (FREQ_SCOPE_SET == FREQ_240_250)
#define FREQ_SCOPE "F240-250 "
#define FREQ_UHF_L 240000000UL
#define FREQ_UHF_H 250000000UL
#define FREQ_UHF_E 245000000UL
#elif (FREQ_SCOPE_SET == FREQ_350_400)
#define FREQ_SCOPE "F350-400 "
#define FREQ_UHF_L 350000000UL
#define FREQ_UHF_H 400000000UL
#define FREQ_UHF_E 375000000UL
#elif (FREQ_SCOPE_SET == FREQ_430_480)
#define FREQ_SCOPE "F430-480 "
#define FREQ_UHF_L 430000000UL
#define FREQ_UHF_H 480000000UL
#define FREQ_UHF_E 455000000UL
#elif (FREQ_SCOPE_SET == FREQ_150_170)
#define FREQ_SCOPE "F150-170 "
#define FREQ_UHF_L 150000000UL
#define FREQ_UHF_H 170000000UL
#define FREQ_UHF_E 160000000UL
#elif (FREQ_SCOPE_SET == FREQ_140_174)
#define FREQ_SCOPE "F140-174 "
#define FREQ_UHF_L 140000000UL
#define FREQ_UHF_H 174000000UL
#define FREQ_UHF_E 160000000UL
#elif (FREQ_SCOPE_SET == FREQ_450_520)
#define FREQ_SCOPE "F450-520 "
#define FREQ_UHF_L 450000000UL
#define FREQ_UHF_H 520000000UL
#define FREQ_UHF_E 480000000UL
#elif (FREQ_SCOPE_SET == FREQ_400_470)
#define FREQ_SCOPE "F400-470 "
#define FREQ_UHF_L 400000000UL
#define FREQ_UHF_H 470000000UL
#define FREQ_UHF_E 455550000UL
#elif (FREQ_SCOPE_SET == FREQ_400_480)
#define FREQ_SCOPE "F400-480"
#define FREQ_UHF_L 400000000UL
#define FREQ_UHF_H 480000000UL
#define FREQ_UHF_E 455550000UL
#elif (FREQ_SCOPE_SET == FREQ_30_32)
#define FREQ_SCOPE "F30-32  "
#define FREQ_UHF_L 30000000UL
#define FREQ_UHF_H 32000000UL
#define FREQ_UHF_E 31000000UL
#elif (FREQ_SCOPE_SET == FREQ_66_68)
#define FREQ_SCOPE "F66-68  "
#define FREQ_UHF_L 66000000UL
#define FREQ_UHF_H 68000000UL
#define FREQ_UHF_E 67000000UL
#elif (FREQ_SCOPE_SET == FREQ_66_88)
#define FREQ_SCOPE "F66-88  "
#define FREQ_UHF_L 66000000UL
#define FREQ_UHF_H 88000000UL
#define FREQ_UHF_E 77000000UL
#endif

#define Delay_ms(a) Ddl_Delay1ms(a)
#define Delay_us(a) Ddl_Delay1us(a)
#define osDelay(a)  Ddl_Delay1ms(a)

#include <ctype.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dh_type.h"

#include "hc32_ddl.h"

#include "lib_print.h"
#include "sys_inc.h"

#include "hc32_ll_def.h"

    extern DH_U32 gu32sysclk;
    extern DH_U32 gu32ahbclk;
    extern DH_U32 gu32apbclk;
    extern DH_U32 gu32socclk;

#ifdef __cplusplus
}
#endif

#endif
/*********************************************************************************************************
 **                            End Of File
 ********************************************************************************************************/
