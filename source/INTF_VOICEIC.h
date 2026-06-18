/*******************************************************************************
 * Copyright (C) 2011-2013 ZheJiang Dahua Technology CO.,LTD.
 *
 * 文件名 : BLL_RF.c
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
#ifndef _INTF_VOICEIC_H_
#define _INTF_VOICEIC_H_

#include  "dh_type.h"

#if 0//FUNC_MENU_DISP == MENU_DISP_LCD
	#define VOICE_IC_STAT 0
#else
	#define VOICE_IC_STAT 1
#endif

#define VOICEIC_LANG_CN   0
#define VOICEIC_LANG_EN   1
#define VOICEIC_LANG_RU   2

#define VOICEIC_BASEADDR1 0x00
#define VOICEIC_BASEADDR2 0x4A
#define VOICEIC_BASEADDR3 0x94

#if LANGUAGE_SEL == LANGUAGE_EN
	#define VOICEIC_LANGUAGE1 VOICEIC_LANG_CN
	#define VOICEIC_LANGUAGE2 VOICEIC_LANG_EN
#elif LANGUAGE_SEL == LANGUAGE_RU
	#define VOICEIC_LANGUAGE1 VOICEIC_LANG_EN
	#define VOICEIC_LANGUAGE2 VOICEIC_LANG_RU
#endif

/****
 * 0：语言1地址
 * 1：语言1发音时长/10 ms
 * 2：语言2地址
 * 3：语言2发音时长/10 ms
 *****/
struct _voiceic{
	DH_U8 voice[3];
};

/*             enum       ,*/
#define VIC_STRDEF  \
	    VICDEF(VIC_0,              30,  30,  0 ,    0xff    )   \
		VICDEF(VIC_One,            40,  50,  80,    0x94    )   \
		VICDEF(VIC_2,              40,  40,  80,    0x95    )   \
		VICDEF(VIC_3,              40,  70,  80,    0x96    )   \
		VICDEF(VIC_4,              50,  50,  80,    0x97    )   \
		VICDEF(VIC_5,              50,  60,  80,    0x98    )   \
		VICDEF(VIC_6,              70,  70,  80,    0x99    )   \
		VICDEF(VIC_7,              50,  70,  80,    0x9A    )   \
		VICDEF(VIC_8,              50,  50,  80,    0x9B    )   \
		VICDEF(VIC_9,              50,  50,  80,    0x9C    )   \
		VICDEF(VIC_10,             50,  50,  80,    0x9D    )   \
		VICDEF(VIC_11,             100,  80,  80,   0x9E	)   \
		VICDEF(VIC_12,             100,  80,  80,   0x9F	)   \
		VICDEF(VIC_13,             100,  80,  80,   0xA0	)   \
		VICDEF(VIC_14,             100,  80,  80,   0xA1	)   \
		VICDEF(VIC_15,             100,  80,  80,   0xA2	)   \
		VICDEF(VIC_SixTeen  ,      100,  80,  80,   0xA3	)        \
		VICDEF(VIC_SevenTeen,      100,  80,  0,    0xFF    )         \
		VICDEF(VIC_Eighteen,       100,  80,  0,    0xFF    )      \
		VICDEF(VIC_Nineteen,       100,  80,  0,    0xFF    )       \
		VICDEF(VIC_Twenty,         100,  80,  0,    0xFF    )       \
		VICDEF(VIC_Thirty,         100,  60,  0,    0xFF    )       \
		VICDEF(VIC_Forty,          100,  60,  0,    0xFF    )       \
		VICDEF(VIC_Fifty,          100,  80,  0,    0xFF    )       \
		VICDEF(VIC_Sixty,          100,  80,  0,    0xFF    )       \
		VICDEF(VIC_Seventy,        100,  80,  0,    0xFF    )       \
		VICDEF(VIC_Eightty,        100,  60,  0,    0xFF    )       \
		VICDEF(VIC_Ninety,         100,  60,  0,    0xFF    )       \
		VICDEF(VIC_Hundred,        70,  60,  0,  	0xFF	)       \
		VICDEF(VIC_High,           50,  50,  80,    0xA4	)	       \
		VICDEF(VIC_Middle,         50,  60,  80,    0xA5	)	       \
		VICDEF(VIC_Low,            50,  50,  50 ,   0xA6	)       \
		VICDEF(VIC_Product,        80,  60,  0  ,   0xFF	)       \
		VICDEF(VIC_Squelch,        100,  70,  100,  0xA7	)       \
		VICDEF(VIC_Zone,           70,  50,  80,    0xA8	)	        \
		VICDEF(VIC_Prompt,         100,  50,  0,    0xFF	)	       \
		VICDEF(VIC_Revive,         100,  50,  0,    0xFF	)	       \
		VICDEF(VIC_Remote,         120,  50,  0,    0xFF	)	       \
		VICDEF(VIC_Detection,      100,  50,  0,    0xFF	)	       \
		VICDEF(VIC_Kill,           100,  50,  0,    0xFF	)	       \
		VICDEF(VIC_VFO,            150,  50,  0,    0xFF	)	       \
		VICDEF(VIC_User_defined,   150,  150,  0,   0xFF	)     \
		VICDEF(VIC_DPMR,           150,  150,  0,   0xFF	)        \
		VICDEF(VIC_DMR,            100,  100,  0,   0xFF	)        \
		VICDEF(VIC_Analog,         60,  80,  80,    0xA9	)	        \
		VICDEF(VIC_Mixed,          50,  50,  80,    0xAA	)	       \
		VICDEF(VIC_Digital,        60,  80,  80,    0xAB	)	       \
		VICDEF(VIC_Mode,           50,  50,  80,    0xAC	)	       \
		VICDEF(VIC_Repeater,       100,  100,  80,  0xAD	)       \
		VICDEF(VIC_Time_Slot,      70,  50,  80,    0xAE	)	       \
		VICDEF(VIC_Off,            50,  50,  80,    0xAF	)	       \
		VICDEF(VIC_On,             50,  50,  80,    0xB0	)	       \
		VICDEF(VIC_Priority,       50,  50,  80,    0xB1	)	       \
		VICDEF(VIC_Volume,         50,  50,  0,  	0xFF	)       \
		VICDEF(VIC_Channel,        50,  50,  80,    0xB2	)	       \
		VICDEF(VIC_Call,           50,  50,  80,    0xB3	)	     \
		VICDEF(VIC_Talk_Around,    50,  100,  80 ,  0xB4	)      \
		VICDEF(VIC_Bluetooth,      50,  50,  0 ,  	0xFF	)       \
		VICDEF(VIC_Power,          50,  50,  80 ,   0xB5	)	       \
		VICDEF(VIC_VOX,            50,  50,  80 ,   0xB6	)	       \
		VICDEF(VIC_Scan,           50,  50,  80 ,   0xB7	)	       \
		VICDEF(VIC_Encryption,     50,  100,  80 ,  0xB8	)       \
		VICDEF(VIC_GPS,            50,  50,  0 ,  	0xFF	)       \
		VICDEF(VIC_Battery_Level,  50,  150,  0 ,   0xFF	)	   \
		VICDEF(VIC_goodbye,        70,  70,  0 ,  	0xFF	)       \
		VICDEF(VIC_Welcome,        140,  50,  0 ,   0xFF	)	       \
        VICDEF(VIC_Pls_charge,     120,  150,  0 ,  0xFF	)   \
        VICDEF(VIC_Time_out,       70,  50,  80 ,   0xB9	)	      \
        VICDEF(VIC_Lock,           50,  50,  0 ,  	0xFF	)   \
        VICDEF(VIC_Unlock,         50,  50,  0 ,  	0xFF	)        \
        VICDEF(VIC_Alarm,          50,  50,  80 ,   0xBA	)	        \
        VICDEF(VIC_Private_Call,   90,  150,  150,  0xBB	)     \
        VICDEF(VIC_Group_Call,     90,  150,  100,  0xBC    )      \
        VICDEF(VIC_General_Call,   90,  150,  100,  0xBD    )    \
        VICDEF(VIC_Monitor,        50,  50,  0 ,  	0xFF	)      \
        VICDEF(VIC_Active,         0,  0,  100 ,  	0xBE	)      \
        VICDEF(VIC_Battery,        0,  0,  80 ,  	0xBF	)        \
        VICDEF(VIC_Cancel,         0,  0,  80 ,  	0xC0	)        \
        VICDEF(VIC_LowPower,       0,  0,  80 ,  	0xC1	)        \
        VICDEF(VIC_Quit,           0,  0,  80 ,  	0xC2	)        \
        VICDEF(VIC_Start,          0,  0,  80 ,  	0xC3	)        \
        VICDEF(VIC_Transmit,       0,  0,  80 ,  	0xC4	)        \

/* VOICEIC_list */
enum{
#define VICDEF(name,  cn,  en,  ru,   ruaddr) name,
	VIC_STRDEF
#undef VICDEF
	VIC_MAX
};




void VOICEIC_INTF_GPIOInit(void);
void VOICEIC_INTF_Write(DH_U8 Addr);
void VOICEIC_INTF_Play(DH_U8 Num);
void VOICEIC_INTF_PlayTone(DH_U8 *shift,  DH_U8 len);
void VOICEIC_INTF_SetSoundLvl(DH_U8 lvl);
#endif


