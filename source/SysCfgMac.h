//SysCfgMac.h
#ifndef __SYS_CONFIG_MAC_H__
#define __SYS_CONFIG_MAC_H__

#define RELEASE_EN        1

#define DEV_TYPE_DRF24_EN 9

#define DEV_TYPE_SEL      DEV_TYPE_DRF24_EN

#define FREQ_136_174      1
#define FREQ_240_250      2    //
#define FREQ_350_400      3
#define FREQ_400_480      4
#define FREQ_400_470      5
#define FREQ_430_480      6
#define FREQ_410_440      7    //DM-6R_RXonly
#define FREQ_430_450      8    //
#define FREQ_150_400      9
#define FREQ_150_170      10    //
#define FREQ_140_174      11    //
#define FREQ_450_520      12

#define FREQ_30_32        13
#define FREQ_66_68        14
#define FREQ_66_88        15

#define LANGUAGE_EN       0

#define LANGUAGE_TW       1
#define LANGUAGE_ZH       2
#define LANGUAGE_RU       3
#define LANGUAGE_KO       4
#define LANGUAGE_FAN      5
#define LANGUAGE_SWEDEN   6

/////////////////////////////////////////////////////////////////////////////////////////////////
#define LCD_BK_IMAGE_EN   0

#define LCD_BK_Color_EN   1

#define LCD_TYPE850       0

#define LCD_TYPE990       1

#define LCD_TYPE22A       2    //

#define LCD_TYPE_SEL      0

#define LOGO_KYD_EN       1    //
#define LOGO_RFT_EN       2
#define LOGO_D750_EN      4
#define LOGO_RBOSS_EN     5
#define LOGO_CIGUNS_EN    6
#define LOGO_RF_EN        7
#define LOGO_STAR_EN      8
#define LOGO_RFTECH_EN    9
#define LOGO_NULL_EN      10

#define LOGO_PRNTO_EN     11
#define LOGO_HORA_EN      12
#define LOGO_NRC_EN       13
#define LOGO_RUASC_EN     14

#define LOGO_MIKO_EN      15 /*89*64*/
#define LOGO_AWC_EN       16

#define LOGO_HKDMR_EN     17 /*188×32*/
#define LOGO_TTTT_EN      18

#define LOGO_POLICE_EN    19 /*64×64<*/
#define LOGO_TEKK_EN      20 /*192×64<*/
#define LOGO_DIR_EN       21 /*Digital Radio192×64<*/
#define LOGO_DELTA_EN     22 /*192*64<*/
#define LOGO_DTS_EN       23 /*192*64<*/
#define LOGO_DATINA_EN    24 /*192*64<*/
#define LOGO_DATINA2_EN   25 /*192*58<*/
#define LOGO_TRITAN_EN    26 /*192*38<*/
#define LOGO_SJR_EN       27 /*84*36*/
#define LOGO_ADVANCE_EN   28 /*192*64<*/
#define LOGO_THIUNX_EN    29 /*192*64<*/
#define LOGO_QCOM_EN      30 /*192*64<*/
#define LOGO_TNIAU_EN     31 /* * <*/

#define LOGO_USERDEF_EN   100 /*192*64<*/

#define LOGO_POWER_ON     LOGO_TNIAU_EN

#if (DEV_TYPE_SEL == DEV_TYPE_DR350_EN || DEV_TYPE_SEL == DEV_TYPE_DR360_EN)
#define FW_VER "V100MR"
#else
#define FW_VER "V100MZSR "
#endif

#endif
