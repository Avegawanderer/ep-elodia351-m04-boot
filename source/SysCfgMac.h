//SysCfgMac.h
#ifndef  __SYS_CONFIG_MAC_H__
#define  __SYS_CONFIG_MAC_H__

#define RELEASE_EN      		1//软件发行开关

///////////////////////机型选择/////////////////////////////////////////////////////////////

#define DEV_TYPE_DRF24_EN		9//C7000//850_7000版本,+19264

#define DEV_TYPE_SEL		    DEV_TYPE_DRF24_EN//机型选择



/////////////////////////////////////////频段设置/////////////////////////////////////////////////////////
#define FREQ_136_174			1
#define FREQ_240_250			2//
#define FREQ_350_400			3
#define FREQ_400_480			4
#define FREQ_400_470			5
#define FREQ_430_480			6
#define FREQ_410_440			7 //DM-6R_RXonly
#define FREQ_430_450			8 //
#define FREQ_150_400			9 //双段收接收
#define FREQ_150_170			10//
#define FREQ_140_174			11//
#define FREQ_450_520			12

#define FREQ_30_32			    13
#define FREQ_66_68			    14
#define FREQ_66_88			    15
//#define FREQ_SCOPE_SET	        FREQ_136_174//频段选择

///////////////////////语言选择/////////////////////////////////////////////////////////////
#define LANGUAGE_EN				0/*中英簡體旧版使用iar編譯器生成的GBK碼*/
#define LANGUAGE_TW				1//中英繁體旧版使用iar編譯器生成的GBK碼
#define LANGUAGE_ZH				2//中英新版
#define LANGUAGE_RU				3//俄语
#define LANGUAGE_KO				4//韩国
#define LANGUAGE_FAN			5//中(繁体)英
#define LANGUAGE_SWEDEN			6//瑞典语(西欧字符集.CP1252)

//#define LANGUAGE_SEL			LANGUAGE_EN        //LCD显示语言选择

/////////////////////////////////////////////////////////////////////////////////////////////////
#define LCD_BK_IMAGE_EN			0/*1=使用背景图片的版本*/
#define LCD_BK_Color_EN			1/*1=使用背景为单颜色的版本*/


///////////////////////显示屏尺寸/////////////////////////////////////////////////////////////
#define LCD_TYPE850				0/*1=使用背景为单颜色的版本*/
#define LCD_TYPE990				1/*1=使用背景为单颜色的版本*/
#define LCD_TYPE22A				2//

#define LCD_TYPE_SEL			0



///////////////////////开机界面选择/////////////////////////////////////////////////////////////
#define LOGO_KYD_EN			    1//
#define LOGO_RFT_EN			    2//(俄语)
#define LOGO_D750_EN	        4
#define LOGO_RBOSS_EN	        5
#define LOGO_CIGUNS_EN	        6
#define LOGO_RF_EN			    7
#define LOGO_STAR_EN	        8
#define LOGO_RFTECH_EN	        9
#define LOGO_NULL_EN	        10/*空白的白块.用于不需要LOGO的情况260X60*/
#define LOGO_PRNTO_EN	        11
#define LOGO_HORA_EN	        12
#define LOGO_NRC_EN	        	13
#define LOGO_RUASC_EN	        14/*俄文的,使用俄文字符作为LOGO*/
#define LOGO_MIKO_EN	        15/*89*64*/
#define LOGO_AWC_EN	        	16/*192*64,带振动*/
#define LOGO_HKDMR_EN	        17/*188×32*/
#define LOGO_TTTT_EN	        18/*58×61,一圆<*/
#define LOGO_POLICE_EN	        19/*64×64<*/
#define LOGO_TEKK_EN	        20/*192×64<*/
#define LOGO_DIR_EN	            21/*Digital Radio192×64<*/
#define LOGO_DELTA_EN	        22/*192*64<*/
#define LOGO_DTS_EN	            23/*192*64<*/
#define LOGO_DATINA_EN	        24/*192*64<*/
#define LOGO_DATINA2_EN	        25/*192*58<*/
#define LOGO_TRITAN_EN	        26/*192*38<*/
#define LOGO_SJR_EN	            27/*84*36*/
#define LOGO_ADVANCE_EN	        28/*192*64<*/
#define LOGO_THIUNX_EN	        29/*192*64<*/
#define LOGO_QCOM_EN	        30/*192*64<*/
#define LOGO_TNIAU_EN	        31/* * <*/

#define LOGO_USERDEF_EN	        100/*192*64<*/

#define LOGO_POWER_ON 	        LOGO_TNIAU_EN





/*
V+主版本号+次版本号+修订版本号+基带类型+语言版本+写频通道+R/T(R是发行版本;T内部测试版本)
主版本号:在修改硬件部件情况下修改；
次版本号:在修改协议，增加功能，情况下修改；
修订版本号:每次解决批量bug进行修改；

004版本外挂Flash存储地址做了调整，增加数据备份和保护；
05版本增加108亚音
07处理扫描问题；
08增加串口写频功能；

*/
#if(DEV_TYPE_SEL==DEV_TYPE_DR350_EN||DEV_TYPE_SEL==DEV_TYPE_DR360_EN)
#define FW_VER				"V100MR"
#else
#define FW_VER				"V100MZSR "
#endif

#endif


