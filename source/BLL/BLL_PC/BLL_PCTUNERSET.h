/*****************************************************************************
 *   文件名:PcTunerSet.h
 *   文件描述:none；
 *   创建人：yufeng	, 2012/11/18
 ******************************************************************************/
#ifndef _BLL_PCTUNERSET_
#define	_BLL_PCTUNERSET_

#include "app_config.h"
#include "BLL_PCTUNER_FREQ.h"

#define TUNER_CMD_READ			0x01
#define TUNER_CMD_WRITE			0x02

#if 1
#define TUNERALLWRITE			0x01
#define TUNERALLREAD			0x02

#define	FREQCHEXKSET			0x10	/*收发频率校准*/
#define	HIGHPOWERSET			0x11	/*高功率设置*/
#define	LOWPOWERSET			    0x12	/*低功率设置*/
#define DMODBALANCESET		    0x13	/*数字调制频偏设置*/
#define	MDEVNARROWSET		    0x14	/*模拟调制频偏设置(窄带)*/
#define	MDEVWIDESET			    0x15	/*模拟调制频偏设置(宽带)*/
#define	MTPLDPLNARROWSET	    0x16	/*TPL/DPL调制频偏设置(窄带)*/
#define	MTPLDPLWIDESET		    0x17	/*TPL/DPL调制频偏设置(宽带)*/
#define	APCADJUST 				0x18	/*APC时间*/
#define	RDA1846PASET			0x1A	/*1846S射频功率输出分段设置*/
#define	TVSET					0x20	/*电调设置*/
#define	RSSITEST				0x21	/*RSSI值查询*/
#define	ERRNUMRATTEST		    0x22	/*误码率测试*/
#define	VOLUMESET				0x23	/*音量设置*/
#define	SQLEVEL1OPENSET		    0x24	/*SQ（1级）开启设置*/
#define	SQLEVEL1CLOSESET		0x25	/*SQ（1级）关闭设置*/
#define	SQLEVEL9OPENSET		    0x26	/*SQ（9级）开启设置*/
#define	SQLEVEL9CLOSESET		0x27	/*SQ（9级）关闭设置*/
#define	CPUFREQADJUST 		    0x28	/*CPU频率校准*/
#define	RSSILEVELSET		    0x30	/*基带解调的模数RSSI中频解调的模拟RSSI*/
#define	DIGITALRSSILEVELSET		0x31	/*中频解调的数字RSSI*/

//V
#define	V_FREQCHEXKSET			0x90	/*收发频率校准*/
#define	V_HIGHPOWERSET			0x91	/*高功率设置*/
#define	V_LOWPOWERSET			0x92	/*低功率设置*/
#define V_DMODBALANCESET		0x93	/*数字调制频偏设置*/
#define	V_MDEVNARROWSET			0x94	/*模拟调制频偏设置(窄带)*/
#define	V_MDEVWIDESET			0x95	/*模拟调制频偏设置(宽带)*/
#define	V_MTPLDPLNARROWSET		0x96	/*TPL/DPL调制频偏设置(窄带)*/
#define	V_MTPLDPLWIDESET		0x97	/*TPL/DPL调制频偏设置(宽带)*/
#define	V_APCADJUST 			0x98	/*APC时间*/
#define	V_RDA1846PASET			0x9A	/*1846S射频功率输出分段设置*/
#define	V_TVSET					0xA0	/*电调设置*/
#define	V_RSSITEST				0xA1	/*RSSI值查询*/
#define	V_ERRNUMRATTEST			0xA2	/*误码率测试*/
#define	V_VOLUMESET				0xA3	/*音量设置*/
#define	V_SQLEVELSET			0xA4	/*SQ（1级）开启设置*/
#define	V_SQLEVEL1CLOSESET		0xA5	/*SQ（1级）关闭设置*/
#define	V_SQLEVEL9OPENSET		0xA6	/*SQ（9级）开启设置*/
#define	V_SQLEVEL9CLOSESET		0xA7	/*SQ（9级）关闭设置*/
#define	V_CPUFREQADJUST 		0xA8	/*CPU频率校准*/
#define	V_RSSILEVELSET			0xB0	/*基带解调的模数RSSI中频解调的模拟RSSI*/
#define	V_DIGITALRSSILEVELSET	0xB1    /*中频解调的数字RSSI*/

#define	MODEIQSET		        0x41	/*IQ两路的幅度和偏置值*/
#define	FMRXDETECTSET		    0x42	/*接收调制频偏的系数*/
#define	FMTXDETECTSET		    0x43	/*发射的调制系数*/

//#define	C5000REGSET			0x48
//#define	P_FLASH_OPT			0x49//外挂Flash操作

#define	RDA1846SREGSET		    0x7f/*1846寄存器值的设置功能*/
#define	AUDIOREGSET			    0x7e//报号测试
#define	C5000REGSET			    0x7d//基带操作
#define	P_FLASH_OPT			    0x7c//外挂Flash操作
#define FLASH_RW			    0XFF

/******************************************************************************/
/****************调测软件读写频数据段格式定义*****************/
/******************************************************************************/
#define TUNERPARMTLEN			198

/*存储起始地址*/
#if (FREQ_SCOPE_UV == FREQ_SCOPE_V)
#define PCTUNERADDR		FLASH_PCTUNER_V_ADDR
#elif (FREQ_SCOPE_UV == FREQ_SCOPE_U)
#define PCTUNERADDR		FLASH_PCTUNER_U_ADDR
#endif

/* 设备信息 */
#define TUNER_EQIPINFO 	PCTUNERADDR	
#define FLASH_PAGE_LEN 32		
#define TUNER_FLASH_LEN (7*FLASH_PAGE_LEN)	


#if 1
#define TUNER_POWER_LEN         (1+8*2)
/*发射选项中高功率设置参数*/
#define TUNER_HPOWERADDR		(TUNER_EQIPINFO)


/*发射选项中低功率设置参数*/
#define TUNER_LPOWERADDR		(TUNER_HPOWERADDR+FLASH_PAGE_LEN)

/* SQ等级 */
#define TUNER_SQ_LEN            (1+8*18)
#define TUNER_SQWDBADDR         (TUNER_LPOWERADDR+FLASH_PAGE_LEN)



#else
#define TUNER_SQUNUM    TUNER_EQIPINFO				/* 序列号 */
#define TUNER_MODEL     (TUNER_EQIPINFO+6)			/* 本机型号 */
#define TUNER_DMRNUM    (TUNER_EQIPINFO+12)		    /* 本机机器码 */
#define TUNER_FREQL     (TUNER_EQIPINFO+20)			/* 频率范围L */
#define TUNER_FREQH     (TUNER_EQIPINFO+24)			/* 频率范围H */
#define TUNER_VERSION   (TUNER_EQIPINFO+28)		    /* 软件版本 */
#define TUNER_CONSDATA  (TUNER_EQIPINFO+34)	        /* 软件日期 */

/*接收选项中电调参数*/
#define TUNER_TVADDR	(PCTUNERADDR+44)

#define TUNER_TVSELECT	TUNER_TVADDR		    /*电调参数选择项*/



#define TUNER_TV403_075	        (TUNER_TVADDR+1)		 /*403.075MHz频点电调参数*/
#define TUNER_TV414_075	        (TUNER_TVADDR+3)		 /*414.075MHz频点电调参数*/
#define TUNER_TV425_075	        (TUNER_TVADDR+5)		 /*425.075MHz频点电调参数*/
#define TUNER_TV436_525	        (TUNER_TVADDR+7)		 /*436.525MHz频点电调参数*/
#define TUNER_TV447_875	        (TUNER_TVADDR+9)		 /*447.875MHz频点电调参数*/
#define TUNER_TV458_875	        (TUNER_TVADDR+11)		 /*458.875MHz频点电调参数*/
#define TUNER_TV469_825	        (TUNER_TVADDR+13)		 /*469.925MHz频点电调参数*/
#define TUNER_TV478_825	        (TUNER_TVADDR+15)		 /*469.925MHz频点电调参数*/
/*接收选项中音量参数*/
#define TUNER_VOLADDR		    (PCTUNERADDR+61)
/*发射选项中收发频率校准参数*/
#define TUNER_TXRXADDR		    (PCTUNERADDR+62)

#define TUNER_FREQCHECKSEL		 TUNER_TXRXADDR			  /*收发选择标志*/
#define TUNER_TX_AMP_BIAS		(TUNER_TXRXADDR+1)	      /*发射频率校准参数*/
#define TUNER_RX_AMP_BIAS		(TUNER_TXRXADDR+3)        /*发射频率校准值+10参数*/
#define TUNER_OFFSET			(TUNER_TXRXADDR+5)	      /*间隔10两次差值参数*/

/*发射选项中高功率设置参数*/
#define TUNER_HPOWERADDR		(PCTUNERADDR+69)

#define TUNER_HPOWERSEL		     TUNER_HPOWERADDR		  /*当前频道号选择参数*/
#define TUNER_HP403_175		    (TUNER_HPOWERSEL+1)	      /*403.175发射功率参数*/
#define TUNER_HP414_175		    (TUNER_HPOWERSEL+3)	      /*414.175发射功率参数*/
#define TUNER_HP425_175		    (TUNER_HPOWERSEL+5)	      /*425.175发射功率参数*/
#define TUNER_HP436_625		    (TUNER_HPOWERSEL+7)	      /*436.625发射功率参数*/
#define TUNER_HP447_775		    (TUNER_HPOWERSEL+9)	      /*447.775发射功率参数*/
#define TUNER_HP458_775		    (TUNER_HPOWERSEL+11)	  /*458.775发射功率参数*/
#define TUNER_HP469_825		    (TUNER_HPOWERSEL+13)	  /*469.825发射功率参数*/
#define TUNER_HP478_825		    (TUNER_HPOWERSEL+15)	  /*469.825发射功率参数*/
/*发射选项中低功率设置参数*/
#define TUNER_LPOWERADDR		(PCTUNERADDR+86)

#define TUNER_LPOWERSEL		     TUNER_LPOWERADDR		  /*当前频道号选择参数*/
#define TUNER_LP403_175		    (TUNER_LPOWERSEL+1)	      /*403.175发射功率参数*/
#define TUNER_LP414_175		    (TUNER_LPOWERSEL+3)	      /*414.175发射功率参数*/
#define TUNER_LP425_175		    (TUNER_LPOWERSEL+5)	      /*425.175发射功率参数*/
#define TUNER_LP436_625		    (TUNER_LPOWERSEL+7)	      /*436.625发射功率参数*/
#define TUNER_LP447_775		    (TUNER_LPOWERSEL+9)	      /*447.775发射功率参数*/
#define TUNER_LP458_775		    (TUNER_LPOWERSEL+11)	  /*458.775发射功率参数*/
#define TUNER_LP469_825		    (TUNER_LPOWERSEL+13)	  /*469.825发射功率参数*/
#define TUNER_LP478_825		    (TUNER_LPOWERSEL+15)	  /*469.825发射功率参数*/
/*发射选项中数字调制频偏参数*/
#define TUNER_DMODBALADDR	    (PCTUNERADDR+103)

#define TUNER_DMODBALSET		TUNER_DMODBALADDR		   /*当前频道号选择参数*/
#define TUNER_DMODBAL403		(TUNER_DMODBALADDR+1)	   /*403MOD1、MOD2参数*/
#define TUNER_DMODBAL410		(TUNER_DMODBALADDR+3)	   /*410MOD1、MOD2参数*/
#define TUNER_DMODBAL424		(TUNER_DMODBALADDR+5)	   /*424MOD1、MOD2参数*/
#define TUNER_DMODBAL436		(TUNER_DMODBALADDR+7)	   /*436MOD1、MOD2参数*/
#define TUNER_DMODBAL445		(TUNER_DMODBALADDR+9)	   /*445MOD1、MOD2参数*/
#define TUNER_DMODBAL459		(TUNER_DMODBALADDR+11)	   /*459.999995MOD1、MOD2参数*/
#define TUNER_DMODBAL460		(TUNER_DMODBALADDR+13)	   /*460MOD1、MOD2参数*/
#define TUNER_DMODBAL470		(TUNER_DMODBALADDR+15)	   /*470MOD1、MOD2参数*/
#define TUNER_SENDSIGNAL		(TUNER_DMODBALADDR+17)	   /*01为+3,02为-3,03为+-3,04为连续语音*/

/*发射选项中模拟调制频偏参数(窄带)*/
#define TUNER_MMODBALADDR	    (PCTUNERADDR+121)

#define TUNER_MMODBALSET		TUNER_MMODBALADDR		   /*当前频道号选择参数*/
#define TUNER_MMODBAL403		(TUNER_MMODBALADDR+1)	   /*403MOD1、MOD2参数*/
#define TUNER_MMODBAL410		(TUNER_MMODBALADDR+3)	   /*410MOD1、MOD2参数*/
#define TUNER_MMODBAL424		(TUNER_MMODBALADDR+5)	   /*424MOD1、MOD2参数*/
#define TUNER_MMODBAL436		(TUNER_MMODBALADDR+7)	   /*436MOD1、MOD2参数*/
#define TUNER_MMODBAL445		(TUNER_MMODBALADDR+9)	   /*445MOD1、MOD2参数*/
#define TUNER_MMODBAL459		(TUNER_MMODBALADDR+11)	   /*459.999995MOD1、MOD2参数*/
#define TUNER_MMODBAL460		(TUNER_MMODBALADDR+13)	   /*460MOD1、MOD2参数*/
#define TUNER_MMODBAL470		(TUNER_MMODBALADDR+15)	   /*470MOD1、MOD2参数*/

/*发射选项中模拟调制频偏参数(宽带)*/
#define TUNER_MMODBALWIDEADDR	(PCTUNERADDR+138)

#define TUNER_MMODBALWIDESET	 TUNER_MMODBALWIDEADDR		/*当前频道号选择参数*/
#define TUNER_MMODBALWIDE403	(TUNER_MMODBALWIDEADDR+1)	/*403MOD1、MOD2参数*/
#define TUNER_MMODBALWIDE410	(TUNER_MMODBALWIDEADDR+3)	/*410MOD1、MOD2参数*/
#define TUNER_MMODBALWIDE424	(TUNER_MMODBALWIDEADDR+5)	/*424MOD1、MOD2参数*/
#define TUNER_MMODBALWIDE436	(TUNER_MMODBALWIDEADDR+7)	/*436MOD1、MOD2参数*/
#define TUNER_MMODBALWIDE445	(TUNER_MMODBALWIDEADDR+9)	/*445MOD1、MOD2参数*/
#define TUNER_MMODBALWIDE459	(TUNER_MMODBALWIDEADDR+11)	/*459.999995MOD1、MOD2参数*/
#define TUNER_MMODBALWIDE460	(TUNER_MMODBALWIDEADDR+13)	/*460MOD1、MOD2参数*/
#define TUNER_MMODBALWIDE470	(TUNER_MMODBALWIDEADDR+15)	/*470MOD1、MOD2参数*/

/*TPLDPL(窄带)*/
#define TUNER_TPLDPLNARROWADDR	(PCTUNERADDR+155)

#define TUNER_TPLDPLNARROWSET	 TUNER_TPLDPLNARROWADDR		/*当前频道号选择参数*/
#define TUNER_TPLDPLNARROW403	(TUNER_TPLDPLNARROWADDR+1)	/*403MOD1、MOD2参数*/
#define TUNER_TPLDPLNARROW410	(TUNER_TPLDPLNARROWADDR+2)	/*410MOD1、MOD2参数*/
#define TUNER_TPLDPLNARROW424	(TUNER_TPLDPLNARROWADDR+3)	/*424MOD1、MOD2参数*/
#define TUNER_TPLDPLNARROW436	(TUNER_TPLDPLNARROWADDR+4)	/*436MOD1、MOD2参数*/
#define TUNER_TPLDPLNARROW445	(TUNER_TPLDPLNARROWADDR+5)	/*445MOD1、MOD2参数*/
#define TUNER_TPLDPLNARROW459	(TUNER_TPLDPLNARROWADDR+6)	/*459.999995MOD1、MOD2参数*/
#define TUNER_TPLDPLNARROW460	(TUNER_TPLDPLNARROWADDR+7)	/*460MOD1、MOD2参数*/
#define TUNER_TPLDPLNARROW470	(TUNER_TPLDPLNARROWADDR+8)	/*01为+3,02为-3,03为+-3,04为连续语音*/

/*TPLDPL(宽带)*/
#define TUNER_TPLDPLWIDEADDR	(PCTUNERADDR+164)

#define TUNER_TPLDPLWIDESET		 TUNER_TPLDPLWIDEADDR		/*当前频道号选择参数*/
#define TUNER_TPLDPLWIDE403		(TUNER_TPLDPLWIDEADDR+1)	/*403MOD1、MOD2参数*/
#define TUNER_TPLDPLWIDE410		(TUNER_TPLDPLWIDEADDR+2)	/*410MOD1、MOD2参数*/
#define TUNER_TPLDPLWIDE424		(TUNER_TPLDPLWIDEADDR+3)	/*424MOD1、MOD2参数*/
#define TUNER_TPLDPLWIDE436		(TUNER_TPLDPLWIDEADDR+4)	/*436MOD1、MOD2参数*/
#define TUNER_TPLDPLWIDE445		(TUNER_TPLDPLWIDEADDR+5)	/*445MOD1、MOD2参数*/
#define TUNER_TPLDPLWIDE459		(TUNER_TPLDPLWIDEADDR+6)	/*459.999995MOD1、MOD2参数*/
#define TUNER_TPLDPLWIDE460		(TUNER_TPLDPLWIDEADDR+7)	/*460MOD1、MOD2参数*/
#define TUNER_TPLDPLWIDE470		(TUNER_TPLDPLWIDEADDR+8)	/*01为+3,02为-3,03为+-3,04为连续语音*/

/*CPU频率校准*/
#define TUNER_CPUFREQADJUSTADDR	(PCTUNERADDR+173)	        /*01为+3,02为-3,03为+-3,04为连续语音*/

/*APC时间校准*/
#define TUNER_APCADJUSTADDR		(PCTUNERADDR+174)	        /*APC时间*/
#define TUNER_APCADJUST		     TUNER_APCADJUSTADDR		/*当前频道号选择参数*/
#define TUNER_APC403_175		(TUNER_APCADJUSTADDR+1)	    /*403.175发射功率参数*/
#define TUNER_APC414_175		(TUNER_APCADJUSTADDR+5)	    /*414.175发射功率参数*/
#define TUNER_APC425_175		(TUNER_APCADJUSTADDR+9)	    /*425.175发射功率参数*/
#define TUNER_APC436_625		(TUNER_APCADJUSTADDR+13)	/*436.625发射功率参数*/
#define TUNER_APC447_775		(TUNER_APCADJUSTADDR+17)	/*447.775发射功率参数*/
#define TUNER_APC458_775		(TUNER_APCADJUSTADDR+21)	/*458.775发射功率参数*/
#define TUNER_APC469_825		(TUNER_APCADJUSTADDR+25)	/*469.825发射功率参数*/
#define TUNER_APC478_825		(TUNER_APCADJUSTADDR+29)	/*478.825发射功率参数*/

/* SQ等级 */
#define TUNER_SQWDBADDR         (PCTUNERADDR+207)
#define TUNER_SQL403_175         TUNER_SQWDBADDR+1
#define TUNER_SQL414_175        (TUNER_SQWDBADDR+19)
#define TUNER_SQL425_175        (TUNER_SQWDBADDR+37)
#define TUNER_SQL436_625        (TUNER_SQWDBADDR+55)
#define TUNER_SQL447_775        (TUNER_SQWDBADDR+73)
#define TUNER_SQL458_775        (TUNER_SQWDBADDR+91)
#define TUNER_SQL469_825        (TUNER_SQWDBADDR+109)
#define TUNER_SQL478_825        (TUNER_SQWDBADDR+127)
/* 不同频率RSSI值*/
#define TUNER_RSSIBADDR         (PCTUNERADDR+352)
#define RSSI400To410            (TUNER_RSSIBADDR+1)
#define RSSI410To420            (TUNER_RSSIBADDR+11)
#define RSSI420To430            (TUNER_RSSIBADDR+21)
#define RSSI430To440            (TUNER_RSSIBADDR+31)
#define RSSI440To450            (TUNER_RSSIBADDR+41)
#define RSSI450To460            (TUNER_RSSIBADDR+51)
#define RSSI460To470            (TUNER_RSSIBADDR+61)
#define RSSI470To480            (TUNER_RSSIBADDR+71)
/* FM不同频率RSSI值*/
#define TUNER_FMRSSIBADDR        TUNER_RSSIBADDR+81
#define FMRSSI400To410          (TUNER_FMRSSIBADDR+1)
#define FMRSSI410To420          (TUNER_FMRSSIBADDR+11)
#define FMRSSI420To430          (TUNER_FMRSSIBADDR+21)
#define FMRSSI430To440          (TUNER_FMRSSIBADDR+31)
#define FMRSSI440To450          (TUNER_FMRSSIBADDR+41)
#define FMRSSI450To460          (TUNER_FMRSSIBADDR+51)
#define FMRSSI460To470          (TUNER_FMRSSIBADDR+61)
#define FMRSSI470To480          (TUNER_FMRSSIBADDR+71)

/*364开始但是调试数据只有3页354-356=384个字节*/
#define TUNER_MODEIQSET         (TUNER_FMRSSIBADDR+81)                 /*4个字节IQ两路的幅度和偏置值*/

#define TUNER_FMRXDETECTSET     (TUNER_MODEIQSET+4)	               /*2个字节分宽窄带接收调制频偏的系数*/
//#define TUNER_FMRXBANDMODE  TUNER_FMRXDETECTSET /*宽窄带*/
#define TUNER_TUNER_FMRXDETECTSETNARROW      TUNER_FMRXDETECTSET   /*窄带*/
#define TUNER_TUNER_FMRXDETECTSETWIDE       (TUNER_FMRXDETECTSET+1) /*宽带带*/

#define TUNER_FMTXDETECTSET                 (TUNER_FMRXDETECTSET	+2)        /*发射的调制系数等分款窄带6个字节*/
//#define TUNER_FMTXBANDMODE  TUNER_FMTXDETECTSET /*宽窄带*/
#define TUNER_TUNER_FMTXDETECTSETNARROW      TUNER_FMTXDETECTSET
#define TUNER_TUNER_FMTXLIMITSETNARROW      (TUNER_FMTXDETECTSET+1)
#define TUNER_TUNER_FMTXSUBDETECTSETNARROW  (TUNER_FMTXDETECTSET+2)
#define TUNER_TUNER_FMTXDETECTSETWIDE       (TUNER_FMTXDETECTSET+3)
#define TUNER_TUNER_FMTXLIMITSETWIDE        (TUNER_FMTXDETECTSET+4)
#define TUNER_TUNER_FMTXSUBDETECTSETWIDE    (TUNER_FMTXDETECTSET+5 )

#pragma pack(1)
typedef struct
{
	uint8 Flg; /*收发频率校准设置标志*/
	uint16 Txampbias; /*发射频率校准值*/
	uint16 Rxampbias; /*发射频率校准值+10*/
	uint16 Offset; /*偏移量*/
	uint8 Toggle; /*PTT按键,1为发射,0为接收*/
} STR_FRQCHECK;
#pragma pack()

#pragma pack(1)
typedef struct
{
	uint8 Flg; /*当前频道号设置标志*/
	uint16 Hp403175; /*403.175高功率设置值*/
	uint16 Hp414175; /*414.175高功率设置值*/
	uint16 Hp425175; /*425.175高功率设置值*/
	uint16 Hp436625; /*436.625高功率设置值*/
	uint16 Hp447775; /*447.775高功率设置值*/
	uint16 Hp458775; /*458.775高功率设置值*/
	uint16 Hp469825; /*469.825高功率设置值*/
	uint8 Toggle; /*PTT按键,1为发射,0为接收*/
} STR_HPOWER;
#pragma pack()

#pragma pack(1)
typedef struct
{
	uint8 Flg; /*当前频道号设置标志*/
	uint16 Lp403175; /*403.175低功率设置值*/
	uint16 Lp414175; /*414.175低功率设置值*/
	uint16 Lp425175; /*425.175低功率设置值*/
	uint16 Lp436625; /*436.625低功率设置值*/
	uint16 Lp447775; /*447.775低功率设置值*/
	uint16 Lp458775; /*458.775低功率设置值*/
	uint16 Lp469825; /*469.825低功率设置值*/
	uint8 Toggle; /*PTT按键,1为发射,0为接收*/
} STR_LPOWER;
#pragma pack()

#pragma pack(1)
typedef struct
{
	uint8 Flg; /*当前频道号设置标志*/
	uint16 BAL403; /*403低8位为MOD1,高8位MOD2幅度值*/
	uint16 BAL410; /*410低8位为MOD1,高8位MOD2幅度值*/
	uint16 BAL424; /*424低8位为MOD1,高8位MOD2幅度值*/
	uint16 BAL436; /*436低8位为MOD1,高8位MOD2幅度值*/
	uint16 BAL445; /*445低8位为MOD1,高8位MOD2幅度值*/
	uint16 BAL459; /*459低8位为MOD1,高8位MOD2幅度值*/
	uint16 BAL460; /*460低8位为MOD1,高8位MOD2幅度值*/
	uint16 BAL470; /*470低8位为MOD1,高8位MOD2幅度值*/
	uint8 Toggle; /*PTT按键,1为发射,0为接收*/
} STR_DMODBAL;
#pragma pack()

#pragma pack(1)
typedef struct
{
	uint8 Flg; /*当前频道号设置标志*/
	uint16 BAL403; /*403低8位为MOD1,高8位MOD2幅度值*/
	uint16 BAL410; /*410低8位为MOD1,高8位MOD2幅度值*/
	uint16 BAL424; /*424低8位为MOD1,高8位MOD2幅度值*/
	uint16 BAL436; /*436低8位为MOD1,高8位MOD2幅度值*/
	uint16 BAL445; /*445低8位为MOD1,高8位MOD2幅度值*/
	uint16 BAL459; /*459低8位为MOD1,高8位MOD2幅度值*/
	uint16 BAL460; /*460低8位为MOD1,高8位MOD2幅度值*/
	uint16 BAL470; /*470低8位为MOD1,高8位MOD2幅度值*/
	uint8 Toggle; /*PTT按键,1为发射,0为接收*/
} STR_MMODBAL;
#pragma pack()

#pragma pack(1)
typedef struct
{
	uint8 Flg; /*当前频道号设置标志*/
	uint16 BAL403075; /*403.075接收灵敏度校准值*/
	uint16 BAL414075; /*414.075接收灵敏度校准值*/
	uint16 BAL425075; /*425.075接收灵敏度校准值*/
	uint16 BAL436525; /*436.525接收灵敏度校准值*/
	uint16 BAL447825; /*447.825接收灵敏度校准值*/
	uint16 BAL458875; /*458.875接收灵敏度校准值*/
	uint16 BAL469925; /*469.925接收灵敏度校准值*/
	uint8 Toggle; /*PTT按键,1为发射,0为接收*/
} STR_TV;
#pragma pack()
#endif
extern uint8 Sq_flag;
extern uint8 PcTunerFlg;
extern uint8 g_PcTunerFeqSelect;/*1为处于配置调试的状态0非调试的状态*/


DH_VOID PC_BLL_PcTunerRecData(DH_U8* RecBuf, DH_U32 HeadLen);
DH_VOID PC_BLL_PcToMcuTuner(DH_U8* Buf, DH_U16 Len, DH_U8 err);
DH_VOID PC_BLL_McuToPcTuner(DH_U8* Buf, DH_U16 Len, DH_U8 err);
DH_VOID PC_BLL_PcFreqCheckSet(DH_U8* Buf, DH_U16 Len, DH_U8 err);
DH_VOID PC_BLL_PcHighPowerSet(DH_U8* Buf, DH_U16 Len, DH_U8 err);
DH_VOID PC_BLL_PcLowPowerSet(DH_U8* Buf, DH_U16 Len, DH_U8 err);
DH_VOID PC_BLL_PcDModBalanceSet(DH_U8* Buf, DH_U16 Len, DH_U8 err);
DH_VOID PC_BLL_PcMDevNarrowSet(DH_U8*Buf, DH_U16 Len, DH_U8 err);
DH_VOID PC_BLL_PcMDevWideSet(DH_U8*Buf, DH_U16 Len, DH_U8 err);
DH_VOID PC_BLL_PcMTPLDPLNarrowSet(DH_U8*Buf, DH_U16 Len, DH_U8 err);
DH_VOID PC_BLL_PcMTPLDPLWideSet(DH_U8*Buf, DH_U16 Len, DH_U8 err);
DH_VOID PC_BLL_PcTvSet(DH_U8* Buf, DH_U16 Len, DH_U8 err);
DH_VOID PC_BLL_PcReadRssi(DH_U8* Buf, DH_U16 Len, DH_U8 err);
DH_VOID PC_BLL_PcReadErrNumRat(DH_U8*Buf, DH_U16 Len, DH_U8 err);
DH_VOID PC_BLL_PcVolumSet(DH_U8*Buf, DH_U16 Len, DH_U8 err);
DH_VOID PC_BLL_PcSQLevel1OpenSet(DH_U8*Buf, DH_U16 Len, DH_U8 err);
DH_VOID PC_BLL_PcSQLevel1CloseSet(DH_U8*Buf, DH_U16 Len, DH_U8 err);
DH_VOID PC_BLL_PcSQLevel9OpenSet(DH_U8*Buf, DH_U16 Len, DH_U8 err);
DH_VOID PC_BLL_PcSQLevel9CloseSet(DH_U8*Buf, DH_U16 Len, DH_U8 err);
DH_VOID PC_BLL_CPUFreqAdjust(DH_U8*Buf, DH_U16 Len, DH_U8 err);
DH_VOID PC_BLL_TunerMFToPc(DH_U8* DstBuf, DH_U8* SourBuf, DH_U16 Len, DH_U8 Opt,
		DH_U8 Ack);
DH_U8 PC_BLL_TunerGetPcFrm(DH_U8* DstBuf, DH_U16* PLen, DH_U32 TimeOut,
		DH_U32 HeadLen);
DH_VOID PC_BLL_ApcAdjustSet(DH_U8 * Buf, DH_U8 Len, DH_U8 err);
DH_VOID PC_BLL_SqAdjustSet(DH_U8 * Buf, DH_U8 Len, DH_U8 err);
DH_VOID PC_BLL_DrvRssiAdjustSet(DH_U8 * Buf, DH_U8 Len, DH_U8 err);
DH_VOID PC_BLL_FmRSSIAdjustSet(DH_U8 * Buf, DH_U8 Len, DH_U8 err);
DH_VOID PC_BLL_RSSIAdjustSet(DH_U8 * Buf, DH_U8 Len, DH_U8 err);
DH_VOID PC_BLL_Digital_RSSISet(DH_U8 * Buf, DH_U8 Len, DH_U8 err);
DH_VOID PC_BLL_Pc1846PaOutSet(DH_U8* Buf, DH_U8 Len, DH_U8 err);

DH_VOID PC_BLL_PcOption1846S(DH_U8* RecBuf);
DH_VOID PC_BLL_PcOptionC5000S(DH_U8* RecBuf);
DH_VOID PC_BLL_PcOptionFlash(DH_U8* RecBuf);

DH_VOID PC_BLL_ModeIQSet(DH_U8* Buf, DH_U8 Len, DH_U8 err);
DH_VOID PC_BLL_FMRxDetectset(DH_U8* Buf, DH_U8 Len, DH_U8 err);
DH_VOID PC_BLL_FMTxDetectset(DH_U8* Buf, DH_U8 Len, DH_U8 err);
DH_VOID PC_BLL_PcOptionGPSmsg(DH_U8* Buf);

DH_VOID PC_BLL_PcAfcAutCreate(DH_U8* Buf, DH_U8* Ampbias, DH_U16* Afc);
#endif

#endif
