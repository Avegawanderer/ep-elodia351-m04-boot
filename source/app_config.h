#ifndef __APP_CONFIG_H
#define __APP_CONFIG_H

#ifdef __cplusplus
extern "C"
{
#endif

 #include "SysCfgMac.h"

//软件发布
#define SOFT_RELEASE            1

#if SOFT_RELEASE

	#define DEBUG               0     //调试串口输出

#else
	#define DEBUG               1     //调试串口输出

#endif




/***********************************************************************/
/*模拟通道选择，传统orAF*/
#define FM_RF_IF                    0           //模拟信号为中频/*模拟信号走中频后输入7000*/
#define FM_RF_AF                    1           //模拟信号为AF，走模拟芯片
#define FM_RF_IF_AF                 2

/*模拟芯片*/
#define FM_LINK_HRCx000             1           //模拟通道为IF->31136->C7000
#define FM_LINK_FM_IC               0           //模拟通道为FM芯片,RDA1846S or W12T
#define FM_LINK_CMX138              2           //模拟通道为IF->31136->CMX138

/*模拟解码方式选择，软件解码或硬件解码*/
#define FM_DECODE_SOFTWARE          0           //模拟软件解码方式
#define FM_DECODE_HARDWARE          1           //模拟硬件解码方式
#define NEW_DECODE_TYEP_ADC         2           //模拟硬件解码方式

/*模拟芯片*/
#define FM_IC_RDA1846S              0           //模拟芯片为1846
#define FM_IC_W12T                  1           //模拟芯片为W12T

/*4818芯片型号*/
#define W12T_FD4818                 0           //模拟芯片为W12T4818
#define W12T_FD6818                 1           //模拟芯片为W12T6818

/*1846芯片型号*/
#define FMIC_RDA1846S              0
#define FMIC_AT1846S               1             /*xin*/


#define SQ_DET_31136                0
#define SQ_DET_HRCx000              1
#define SQ_DET_FMIC                 2

/*********启动方式选择**********/
#define POWERON_PRESS               0    //按压3S启动
#define POWERON_ROLLEN              1    //旋音量键开关

/*********信道切换方式选择**********/
#define CHN_CODESW_NONE             0    /*不适应旋转信道器件*/
#define CHN_CODESW_16MAX            1    /*16频道旋钮,16格雷码对应16信道*/
#define CHN_CODESW_360D             2    /*360旋钮,旋转触发信道切换或其他*/

/*********显示方式选择**********/
#define MENU_DISP_NONE              0    //无显示
#define MENU_DISP_LED               1    //使用LED点阵显示
#define MENU_DISP_LCD               2    //使用LCD屏显示

/*********电路主板选择**********/
#define BOARD_WITHOUTKEY            0    //外挂按键板的主板
#define BOARD_WITHKEY               1    //自带按键的主板
#define BOARD_WITH12KEY             2    //自带12按键的主板（F24）


/*********供电选择**********/
#define POWER_SUPPLY_72V            0    //7.2V
#define POWER_SUPPLY_42V            1    //4.2V

/*********MENU_DISP_LCD显示器件选择**********/
#define USE_DISP_LCDNONE            0    //使用LED不用lCD屏显示
#define USE_DISP_LCD19264           1    //使用LCD19264屏显示
#define USE_DISP_LCD_ST7735         2    //使用LCDST7735S屏显示
#define USE_DISP_LCD13232           3    //使用LCD13234屏显示

/*********MENU_DISP_LCD显示器件选择**********/
#define VOLUME_CTL_TYPE_SOFT        0    //使用音量检测后用软件控制音量输出大小
#define VOLUME_CTL_TYPE_HARD        1    //使用硬件控制音量

/**********************************无屏机DR320**************************************
 * **************************************************************************/
#if DEV_TYPE_SEL == DEV_TYPE_DRF24_EN
	#define DEV_NAME 			  "DR-F24"

	#define LANGUAGE_SEL			LANGUAGE_RU
	#define FREQ_SCOPE_SET	        FREQ_136_174

	/***************模拟通路****************/
	#define FM_RF_SEL              FM_RF_IF
	#define FM_IC_SEL              0xff

	#define FM_IC_SEL_MODEL        FMIC_RDA1846S

	/*模拟解码方式选择，软件解码或硬件解码*/
	#if FM_RF_SEL == FM_RF_IF
		#define FM_LINK_SEL         FM_LINK_HRCx000      /*模拟通道选择，模拟芯片 或C7000*///模拟通道选择，模拟芯片 或C7000
		#define SQ_DET_SEL          SQ_DET_31136
		#define FM_DECODE_SEL       NEW_DECODE_TYEP_ADC   /*C7000只有硬件解码*/
	#else
		#define FM_LINK_SEL         FM_LINK_FM_IC        /*模拟通道选择，模拟芯片 或C7000*///模拟通道选择，模拟芯片 或C7000
		#define FM_DECODE_SEL       FM_DECODE_SOFTWARE
	#endif

	/***************数字通路****************/
	#define DMR_RF_SEL              FM_RF_AF

	#define REC_CHN_SEL              FM_RF_AF


	/***************电源配置****************/
	#define POWER_SUPPLY_SEL             POWER_SUPPLY_42V

	/***************电路板选择****************/
	#define USE_BOARD_SEL                BOARD_WITHKEY

	/***************开机方式选择****************/
	#define USE_POWERON_SEL              POWERON_PRESS
	#define FUNC_USE_PWR_KEY             1    //使用按压电源开关为指定按键功能

	/*********音量检测方式选择**********/
	#define USE_SPIN_VOLUME              0    //1：使用旋鈕音量檢測，0：使用上下鍵進行音量設置

	/*********编码器选择**********/
	#define USE_CHN_CODESW               CHN_CODESW_16MAX    /*1:频道旋钮功能 ，0:使用上下键切换*/
	#define USE_SIMUCODESW               0                  /*1:上下键模拟旋钮，用于输入法选择 ，0:不模拟*/

	/*********显示LCDorLED和显示器件选择**********/
	#define FUNC_MENU_DISP               MENU_DISP_NONE
	#define USE_LCD_DISP_SEL             USE_DISP_LCD19264 //FUNC_MENU_DISP==MENU_DISP_LED 此项不起作用

	/*********其他硬件选择**********/
	#define USE_KEYPAD_ALL               1    //按键功能 0:6键,1:全键
	#define USE_UART_SIGNL_LINE          0    //1:串口使用单线，0：双线

	/*********其他硬件or功能选择**********/
	#define FUNC_LEDLIGHT                0    /*不使用360旋钮，手电筒功能自定义*/
	#define FUNC_DMR_N_FM                0    //数模同检功能
	#define FUNC_RECORD                  1    //录音功能
	#define FUNC_RECORD_FM               0    //录音功能
	#define FUNC_FMCALLDISP              0    //模拟呼叫显示
	#define FUNC_RDA5802                 0    //收音机功能
	#define FUNC_GPS                     0    //GPS功能
	#define FUNC_FREQALL                 0    //全频模式
	#define FUNC_CHNEDIT                 0    //信道编辑
	#define FUNC_CHNSWQIT                0    //快捷切换到指定信道
	#define FUNC_ZONE_ADD                0    //自建区域
	#define FUNC_SKIP_EMPTY_CHN          1    /*切换信道时跳过空信道*/
	#define FUNC_USER_ENCKEY             1    /*支持用户自定义密匙*/
	#define FUNC_ZONE_CHN_SEL            1    /*0:16chn-128zone;1:250chn-4zone*/

	//一些菜单特殊操作要求
	#define FUNC_FM_SUB_NO_FSREC         0    /*1:模拟亚音设置部分接收发射，设置即同时设置接收和发射*/
	#define FUNC_FM_SUB_MENUDISP         1    /*1:显示所有模拟数字亚音菜单，0：只显示模拟亚音*/
	#define FUNC_FM_CTCSS_38_51          0    /*模拟亚音先显示1-38个标准亚音，再显示扩展亚音*/
	#define FUNC_RADIO_ID_EDIT_EN        1    /*可编辑本机ID*/

	#define FUNC_RX_GRP_INCLUDE_LM       0    /*1:接收组列表自动添加发射默认联系人，0：不自动包括*/
	#define FUNC_SLOT_RXTX               0    /*1:发射接收时隙单独设置；             0:*/
	#define FUNC_REPEAT                  0    /*1:同频中继；             0:*/
	#define FUNC_REPEATAB                0    /*1:UV双段AB中继；             0:*/
	#define FUNC_VIBRATION               1    /*1:振动；             0:*/
	#define FUNC_ROAM                    0    /**/
	#define FUNC_DTMF                    1
	#define FUNC_GYRO                    0    /*陀螺仪*/
	#define FUNC_LONEWORKER              0    /**/
	#define FUNC_CTCSSPHASETAIL          0    /**/
#endif

/*时钟滴答*/
#define BSP_ALARM_FREQUENCY     500

/*任务堆栈大小*/
#define TASKKEY_STK_SIZE        256/*used:87*/
#define TASKCODESW_STK_SIZE     384/*used:220*/
#define TASKPC_STK_SIZE         256/*used:52*/
#define TASKRF_STK_SIZE         256/*used:103*/
#define TASKSYS_STK_SIZE        256/*used:134*/
#define TASKTXSLOT_STK_SIZE     256/*used:101*/
#define TASKRXSLOT_STK_SIZE     256/*used:49*/

#define TASKRXCSBK_STK_SIZE     256/*used:62*/
#define TASKRXVOICE_STK_SIZE    256/*used:67*/
#define TASKTXMSG_STK_SIZE      256/*used:122*/
#define TASKRXMSG_STK_SIZE      256/*used:69*/

#define TASKCPCHAN_STK_SIZE     256
#define TASKENCRYPT_STK_SIZE    256
#define TASKDECRYPT_STK_SIZE    256

#define	TASKINIT_STK_SIZE   	768/*used:532, free:236*/
#define TASKTMRHND_STK_SIZE     512/*used:73*/
#define TASKINPUTMTHD_STK_SIZE  128/*used:51*/
#define TASKMENUDISP_STK_SIZE   256/*used:134*/

#define TASKMENU_STK_SIZE       512/*used:125*/
#define TASKMENUKEY_STK_SIZE    768/*used:214*/
#define TASKSCANDISP_STK_SIZE   512/*used:121*/
#define TASKSLP_STK_SIZE        256/*used:*/
#define TASKCALLPTT_STK_SIZE    256/*used:62*/
#define TASKCALLRX_STK_SIZE     256/*used:128*/
#define TASKCSBK_STK_SIZE       256/*used:56*/
#define TASKMSG_STK_SIZE        512/*used:326*/
#define TASKSHUTDOWN_STK_SIZE   128

#define TASKVOX_STK_SIZE        340

#define TASKENCODER_STK_SIZE    1480/*used:984*/
#define TASKDECODER_STK_SIZE    768/*used:578*/

#define TASKSOUND_STK_SIZE      256

#define TASKTXRECORD_STK_SIZE   256
#define TASKRXRECORD_STK_SIZE   256

#define TASK_FMREC_STK_SIZE_    384

#define TASK_Agc_STK_SIZE_      256
#define TASKIDLE_STK_SIZE       300
#define TASKGPS_STK_SIZE        512
#define TASK_RDA5802_STK_SIZE   256
#define TASKLCDDISP_STK_SIZE    256

/*任务优先级枚举定义，优先级最大为32，
 32为保留优先级，多任务之间可以设置相同优先级*/
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





#if 0// FREQ_RELEASE_EN	//完全放开频率的限制，适合测试使用
	#define FREQ_SCOPE 			"F100-600 "
	#define FREQ_UHF_L			100000000UL
	#define FREQ_UHF_H			600000000UL
	#define FREQ_UHF_E			400000000UL
#elif (FREQ_SCOPE_SET==FREQ_136_174)//使能U段的频率.
	#define FREQ_SCOPE 			"F136-174 "
	#define FREQ_UHF_L			136000000UL
	#define FREQ_UHF_H			174000000UL
	#define FREQ_UHF_E			155000000UL
#elif (FREQ_SCOPE_SET==FREQ_240_250)
	#define FREQ_SCOPE 			"F240-250 "
	#define FREQ_UHF_L			240000000UL
	#define FREQ_UHF_H			250000000UL
	#define FREQ_UHF_E			245000000UL
#elif (FREQ_SCOPE_SET==FREQ_350_400)
	#define FREQ_SCOPE 			"F350-400 "
	#define FREQ_UHF_L			350000000UL
	#define FREQ_UHF_H			400000000UL
	#define FREQ_UHF_E			375000000UL
#elif (FREQ_SCOPE_SET==FREQ_430_480)
	#define FREQ_SCOPE 			"F430-480 "
	#define FREQ_UHF_L			430000000UL
	#define FREQ_UHF_H			480000000UL
	#define FREQ_UHF_E			455000000UL
#elif (FREQ_SCOPE_SET==FREQ_150_170)
	#define FREQ_SCOPE 			"F150-170 "
	#define FREQ_UHF_L			150000000UL
	#define FREQ_UHF_H			170000000UL
	#define FREQ_UHF_E			160000000UL
#elif (FREQ_SCOPE_SET==FREQ_140_174)
	#define FREQ_SCOPE 			"F140-174 "
	#define FREQ_UHF_L			140000000UL
	#define FREQ_UHF_H			174000000UL
	#define FREQ_UHF_E			160000000UL
#elif (FREQ_SCOPE_SET==FREQ_450_520)
	#define FREQ_SCOPE 			"F450-520 "
	#define FREQ_UHF_L			450000000UL
	#define FREQ_UHF_H			520000000UL
	#define FREQ_UHF_E			480000000UL
#elif (FREQ_SCOPE_SET==FREQ_400_470)
	#define FREQ_SCOPE 			"F400-470 "
	#define FREQ_UHF_L			400000000UL
	#define FREQ_UHF_H			470000000UL
	#define FREQ_UHF_E			455550000UL
#elif (FREQ_SCOPE_SET==FREQ_400_480)
	#define FREQ_SCOPE 			"F400-480"
	#define FREQ_UHF_L			400000000UL
	#define FREQ_UHF_H			480000000UL
	#define FREQ_UHF_E			455550000UL
#elif (FREQ_SCOPE_SET==FREQ_30_32)
	#define FREQ_SCOPE 			"F30-32  "
	#define FREQ_UHF_L			30000000UL
	#define FREQ_UHF_H			32000000UL
	#define FREQ_UHF_E			31000000UL
#elif (FREQ_SCOPE_SET==FREQ_66_68)
	#define FREQ_SCOPE 			"F66-68  "
	#define FREQ_UHF_L			66000000UL
	#define FREQ_UHF_H			68000000UL
	#define FREQ_UHF_E			67000000UL
#elif (FREQ_SCOPE_SET==FREQ_66_88)
	#define FREQ_SCOPE 			"F66-88  "
	#define FREQ_UHF_L			66000000UL
	#define FREQ_UHF_H			88000000UL
	#define FREQ_UHF_E			77000000UL
#endif


#define Delay_ms(a) Ddl_Delay1ms(a)
#define Delay_us(a) Ddl_Delay1us(a)
#define osDelay(a) Ddl_Delay1ms(a)



#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

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
