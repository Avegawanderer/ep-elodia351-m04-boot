/**************************************************************************************************
  Filename:       CTCSS_DCS_DECODE.c
  Revised:        $Date: 2010-08-05 $
  Revision:       $ $
  Author:         cykang

  Description:    1846解码专用 -- 因为1846将正弦波自动整成方波，
									并在检测到尾音时自动发持续高（低）电平

  Change History:

**********************************************************************/


/*********************************************************************
 * INCLUDES
 */
#include "app_config.h"
 

#include "include.h"

//#define CTCSSEND 0
/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

#if (FM_DECODE_SEL==NEW_DECODE_TYEP_ADC)

extern DH_U8 FlashBuf[];


const uint8 CdcssErrorBitChangeTable[] =
{
    8,	/* 0 */
    4,	/* 1 */
    2,	/* 2 */
    1,	/* 3 */
    0	/* 4 */
};




const uint8 CtcssCharacterValueTable[] =
{
    0x59, /*  */
    0x59, /* 1x Hz  */
    0x59, /* 2x Hz  */
    0x59, /* 3x Hz  */
    0x59, /* 4x Hz  */
    0x59, /* 5x Hz  */
    0x43, /* 6x HZ  */
    0x43, /* 7x HZ  */
    0x43, /* 8x HZ  */
    0x43, /* 9x HZ  */

    0x53, /* 10x HZ */
    0x53, /* 11x HZ */
    0x53, /* 12x HZ */
    0x53, /* 13x HZ */
    0x53, /* 14x HZ */
    0x43, /* 15x HZ */
    0x43, /* 16x HZ */
    0x43, /* 17x HZ */
    0x43, /* 18x HZ */
    0x43, /* 19x HZ */

    0x42, /* 20x HZ */
    0x32, /* 21x HZ */
    0x32, /* 22x HZ */
    0x32, /* 23x HZ */
    0x32, /* 24x HZ */
    0x32, /* 25x HZ */
    0x59, /* 26x HZ */
    0x59, /* 27x HZ */
    0x59, /* 28x HZ */
    0x59, /* 29x HZ */
    0x59, /* 30x HZ */
    0x59  /* 31x HZ */
};

/*------------------------------------------------------------------------------------
	0000	0001	0010	0011	0100	0101	0110	0111
	1000	1001	1010	1011	1100	1101	1110	1111
------------------------------------------------------------------------------------*/
const uint8 Digit1InAccTable[] =
{


    0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,		/* 00-0fh */
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,		/* 10-1fh */
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,		/* 20-2fh */
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,		/* 30-3fh */
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,		/* 40-4fh */
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,		/* 50-5fh */
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,		/* 60-6fh */
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,		/* 70-7fh */

    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,		/* 80-8fh */
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,		/* 90-9fh */
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,		/* a0-afh */
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,		/* b0-bfh */
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,		/* c0-cfh */
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,		/* d0-dfh */
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,		/* e0-efh */
    4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8			/* f0-ffh */
};



const uint8 BCHL4TAB[]={
0xA0,
0xD0,
0x40,
0xA0,
0xD0,
0xC0,
0x60,
0x30,
0x30,
0xB0,
0xF0,
0x50
};

const uint8 BCHU7TAB[]={
0x63,
0x31,
0x7B,
0x3D,
0x1E,
0x6C,
0x36,
0x1B,
0x6E,
0x54,
0x49,
0x47
};

const DH_U16 CtcssValueTableADC[]=
{/*解码频率的倒数关系*/
#if FUNC_FM_CTCSS_38_51
#else
	/*62.5*/625,
	/*67*/670,
	/*69.3*/693,
	/*71.9*/719,
	/*74.4*/744,
	/*77*/770,
	/*79.7*/797,
	/*82.5*/825,
	/*85.4*/854,
	/*88.5*/885,
	/*91.5*/915,
	/*94.8*/948,
	/*97.4*/974,
	/*100*/1000,
	/*103.5*/1035,
	/*107.2*/1072,
	/*110.9*/1109,
	/*114.8*/1148,
	/*118.8*/1188,
	/*123*/1230,
	/*127.3*/1273,
	/*131.8*/1318,
	/*136.5*/1365,
	/*141.3*/1413,
	/*146.2*/1462,
	/*151.4*/1514,
	/*156.7*/1567,
	/*159.8*/1598,
	/*162.2*/1622,
	/*165.5*/1655,
	/*167.9*/1679,
	/*171.3*/1713,
	/*173.8*/1738,
	/*177.3*/1773,
	/*179.9*/1799,
	/*183.5*/1835,
	/*186.2*/1862,
	/*189.9*/1899,
	/*192.8*/1928,
	/*196.6*/1966,
	/*199.5*/1995,
	/*203.5*/2035,
	/*206.5*/2065,
	/*210.7*/2107,
	/*218.1*/2181,
	/*225.7*/2257,
	/*229.1*/2291,
	/*233.6*/2336,
	/*241.8*/2418,
	/*250.3*/2503,
	/*254.1*/2541,
#endif
};
const DH_U16 CtcssDeviationOnBuf[51]=
{/*开启阀值偏移量*/
#if FUNC_FM_CTCSS_38_51
	11,/*67*/
	11,/*71.9*/
	12,/*74.4*/
	12,/*77*/
	12,/*79.7*/
	13,/*82.5*/
	13,/*85.4*/
	14,/*88.5*/
	13,/*91.5*/
	13,/*94.8*/
	12,/*97.4*/
	13,/*100*/
	16,/*103.5*/
	16,/*107.2*/
	16,/*110.9*/
	17,/*114.8*/
	17,/*118.8*/
	18,/*123*/
	19,/*127.3*/
	19,/*131.8*/
	20,/*136.5*/
	21,/*141.3*/
	21,/*146.2*/
	22,/*151.4*/
	15,/*156.7*/
	10,/*162.2*/
	13,/*167.9*/
	10,/*173.8*/
	14,/*179.9*/
	14,/*186.2*/
	10,/*192.8*/
	11,/*203.5*/
	19,/*210.7*/
	21,/*218.1*/
	18,/*225.7*/
	20,/*233.6*/
	29,/*241.8*/
	9 ,/*250.3*/

	10,/*69.3*/
	15,/*62.5*/
	12,/*159.8*/
	10,/*165.5*/
	13,/*171.3*/
	12,/*177.3*/
	11,/*183.5*/
	14,/*189.9*/
	9 ,/*196.6*/
	18,/*199.5*/
	12,/*206.5*/
	16,/*229.1*/
	12,/*254.1*/
#else
15,/*62.5*/
11,/*67*/
10,/*69.3*/
11,/*71.9*/
12,/*74.4*/
12,/*77*/
12,/*79.7*/
13,/*82.5*/
13,/*85.4*/
14,/*88.5*/
13,/*91.5*/
13,/*94.8*/
12,/*97.4*/
13,/*100*/
16,/*103.5*/
16,/*107.2*/
16,/*110.9*/
17,/*114.8*/
17,/*118.8*/
18,/*123*/
19,/*127.3*/
19,/*131.8*/
20,/*136.5*/
21,/*141.3*/
21,/*146.2*/
22,/*151.4*/
15,/*156.7*/
12,/*159.8*/
10,/*162.2*/
10,/*165.5*/
13,/*167.9*/
13,/*171.3*/
10,/*173.8*/
12,/*177.3*/
14,/*179.9*/
11,/*183.5*/
14,/*186.2*/
14,/*189.9*/
10,/*192.8*/
9,/*196.6*/
18,/*199.5*/
11,/*203.5*/
12,/*206.5*/
19,/*210.7*/
21,/*218.1*/
18,/*225.7*/
16,/*229.1*/
20,/*233.6*/
29,/*241.8*/
9,/*250.3*/
12,/*254.1*/


#endif
};
/*新增到108个*/
const DH_U16 DcsCode_table_NormalADC[CDCSS_NUM]=
{
0X080F,/*017add*/
0x0813,/*023*/
0x0815,/*025*/
0x0816,/*026*/
0x0819,/*031*/
0x081a,/*032*/
0x081e,/*036add*/
0x0823,/*043*/
0x0827,/*047*/
0x0828,/*050add*/
0x0829,/*051*/
0x082b,/*053add*/
0x082c,/*054*/
0x0835,/*065*/
0x0839,/*071*/
0x083a,/*072*/
0x083b,/*073*/
0x083c,/*074*/
0x084c,/*114*/
0x084d,/*115*/
0x084E,/*116*/
0x0852,/*122add*/
0x0855,/*125*/
0x0859,/*131*/
0x085A,/*132*/
0x085c,/*134*/
0x0863,/*143*/
0x0865,/*145add*/
0x086a,/*152*/
0x086d,/*155*/
0x086e,/*156*/
0x0872,/*162*/
0x0875,/*165*/
0x087A,/*172*/
0x087C,/*174*/
0x0885,/*205*/
0x088a,/*212add*/
0x0893,/*223*/
0x0895,/*225add*/
0x0896,/*226*/
0x08a3,/*243*/
0x08a4,/*244*/
0x08a5,/*245*/
0x08a6,/*246add*/
0x08a9,/*251*/
0x08aa,/*252add*/
0x08ad,/*255add*/
0x08b1,/*261*/
0x08b3,/*263*/
0x08b5,/*265*/
0x08b6,/*266add*/
0x08b9,/*271*/
0x08bc,/*274add*/
0x08c6,/*306*/
0x08c9,/*311*/
0x08cd,/*315*/
0x08d5,/*325add*/
0x08d9,/*331*/
0x08da,/*332add*/
0x08e3,/*343*/
0x08e6,/*346*/
0x08e9,/*351*/
0x08ee,/*356add*/
0x08f4,/*364*/
0x08f5,/*365*/
0x08f9,/*371*/
0x0909,/*411*/
0x090a,/*412*/
0x090b,/*413*/
0x0913,/*423*/
0x0919,/*431*/
0x091A,/*432*/
0x0925,/*445*/
0x0926,/*446add*/
0x092a,/*452add*/
0x092c,/*454add*/
0x092d,/*455add*/
0x0932,/*462add*/
0x0934,/*464*/
0x0935,/*465*/
0x0936,/*466*/
0x0943,/*503*/
0x0946,/*506*/
0x094e,/*516*/
0x0953,/*523add*/
0x0956,/*526add*/
0x095a,/*532*/
0x0966,/*546*/
0x0975,/*565*/
0x0986,/*606*/
0x098a,/*612*/
0x0994,/*624*/
0x0997,/*627*/
0x0999,/*631*/
0x099a,/*632*/
0x09a5, ///*645add20151130zsy*/
0x09a6,/*646add*/
0x09ac,/*654*/
0x09b2,/*662*/
0x09b4,/*664*/
0x09c3,/*703*/
0x09ca,/*712*/
0x09d3,/*723*/
0x09d9,/*731*/
0x09da,/*732*/
0x09dc,/*734*/
0x09e3,/*743*/
0x09ec, /*754*/
};

/*********************************************************************
 * TYPEDEFS
 */
#define BUILD_UINT16(hibyte,lobyte) \
	((uint16)(((lobyte)&(uint16)0x00ff)+(((hibyte)&(uint16)0x00ff)<<8)))
/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */
//uint8   CTCSS_TIME=0xF8;
//DH_BOOL fCtdcssValid;
//DH_BOOL fLastGpioValue;
//uint16 GpioValueCount;
uint16 TailNum;
uint16 ctcssOkNum;/*ctcss比对符合的次数.*/
uint8 decode_type;/*亚音频类型.*/
uint8 Cdcss23BitOkCnt;
uint8 AA;
//uint8 cCtcssCdcssValid_DE;
//uint16 CTCSS_DCS_DECODE;


//DH_BOOL cTX_RX_Flag;

DH_BOOL fOldTiLevel;						/* OLD CTCSS LEVEL 旧CTCSS水平 */
DH_BOOL f8TimesNormalize;
DH_BOOL fCtdcssValid;						/* 1:CTCSS/cdcss match */
DH_BOOL fCompareSubgroupCtcss;
DH_BOOL fCtdcssCmp;
DH_BOOL fCtcssReverseBurst;
DH_BOOL fCdcssRcvMore;
DH_BOOL fCdcssNormalMore;


//uint8 QTDECSTU;					/* QT decode stastu 模拟亚音频解码状态 */
//uint8 TOTALCIR;					/* Total circle counter 总的循环次数 */
uint8 CtcssDeviationOff;
uint8 CtcssDeviationOn;
uint8 FilterLengthValue;
uint8 AudioClosingTimeIncPer10ms;
uint8 TiSampleStorePtr;			/* 类似于AD采样值保存数组的指针的意义 */
uint8 CtcssCorrelationDelayValue;
uint8 CtcssSubgroupOkMinEnteryValue;
uint8 CtcssSubgroupStateWindow;
uint8 CtcssFilterSampleArray[ConstMaxFilterLengthValue];/* 64 */
uint8 CtcssTimeStorePtr;
uint8 CtcssCorrelationCount;
uint8 CtcssBurstHalfMoreCount;
uint8 CtcssBurstHalfLessCount;
uint8 LastCtcssMatchedPeriod;
uint8 SameCorrelationCtcssCount;
uint8 CdcssCycleMinErrorMatch;
uint8 TiCdcssTailTimer;

uint8 CdcssRcv3;
uint8 CdcssRcv2;
uint8 CdcssRcv1;
uint8 CdcssCycleCompareByte3;
uint8 CdcssCycleCompareByte2;
uint8 CdcssCycleCompareByte1;

uint8 DQTCODE;
uint8 DQTCODE1;
uint8 DQTCODE2;
uint8 DQTCODE3;
uint8 IntTemp;
//uint8 Cdcss23BitOkCnt;
uint8 CdcssQuickChangCnt;
uint8 TiNormalTimer;
uint8 DQTBCHU;
uint8 DQTBCHL;

uint16 CurSgnl;						/* CurSgnlHigh CurSgnlLow */
uint32 MULLIER;						/* For step 5.00 or 6.25 */
uint16 TEMPBCD;						/* TEMPBCD2 TEMPBCD1 */
uint16 CtcssEnterMinCount; 			/* CtcssEnterMinHiCount CtcssEnterMinLowCount */
uint16 CtcssExitMinCount;			/* CtcssExitMinHiCount CtcssExitMinLowCount */
uint16 CtcssEnterMaxCount;			/* CtcssEnterMaxHiCount CtcssEnterMaxLowCount */
uint16 CtcssExitMaxCount;			/* CtcssExitMaxHiCount CtcssExitMaxLowCount */
uint16 CtcssCriterionCount;			/* CtcssCriterionHiCount CtcssCriterionLowCount */
uint16 CtcssBurstHalfMoreMaxCount;	/* CtcssBurstHalfMoreMaxHiCount CtcssBurstHalfMoreMaxLowCount */
uint16 CtcssBurstHalfMoreMinCount;	/* CtcssBurstHalfMoreMinHiCount CtcssBurstHalfMoreMinLowCount */
uint16 CtcssBurstHalfLessMaxCount;	/* CtcssBurstHalfLessMaxHiCount CtcssBurstHalfLessMaxLowCount */
uint16 CtcssBurstHalfLessMinCount;	/* CtcssBurstHalfLessMinHiCount CtcssBurstHalfLessMinLowCount */
uint16 TempReg;						/* Temporary register临时寄存器 这边也有可能是8位的变量 要确认一下 */
uint16 PeakToPeakValue;				/* PeakToPeakHiValue PeakToPeakLowValue */
uint16 CtcssSubgroupMaxCount;			/* CtcssSubgroupMaxHiCount CtcssSubgroupMaxLowCount */
uint16 CtcssSubgroupMinCount;			/* CtcssSubgroupMinHiCount CtcssSubgroupMinLowCount */
uint16 CtcssExitUnitMaxCount;			/* CtcssExitUnitMaxHiCount CtcssExitUnitMaxLowCount */
uint16 CtcssExitUnitMinCount;			/* CtcssExitUnitMinHiCount CtcssExitUnitMinLowCount */
uint16 CtcssCorrelationMaxCount;		/* CtcssCorrelationMaxHiCount CtcssCorrelationMaxLowCount */
uint16 CtcssCorrelationMinCount;		/* CtcssCorrelationMinHiCount CtcssCorrelationMinLowCount */
uint16 CtcssPeakTimerArray[ConstCtcssGroupNum];/* 里面的数可能是ConstCtcssGroupNum //CtcssPeakHiTimerArray CtcssPeakLowTimerArray */
uint16 Ti1stFilterSamplesSum;			/* Ti1stFilterSamplesHiSum Ti1stFilterSamplesLowSum */
uint16 TiEndFilterSamplesSum;			/* TiEndFilterSamplesHiSum TiEndFilterSamplesLowSum */
uint16 TiTimer;						/* Timer for Decode CTCSS */
uint16 DifferenceOfSampleValue;		/* DifferenceOfSampleHiValue DifferenceOfSampleLowValue */
uint16 CtcssTotalTime;				/* CtcssTotalHiTime CtcssTotalLowTime */
uint16 QuasiCtcssPeakTime;
uint16 CtcssLastPeakTime;
uint16 CtcssCorrelationExitMinCount;
uint16 CtcssCorrelationExitMaxCount;

int16 TiPeriodMinValue;				/* TiPeriodMinHiValue TiPeriodMinLowValue */
int16 TiPeriodMaxValue;

//uint32 MULLIER_temp;
uint8 XX;
uint8 YY;
//uint8 AA;
uint16 AD;
//uint8 P_AF_PWR;
//uint8 AA_TEMP;
uint8 TIBIAVG = 86;//134;/*中间线直接有关.*/
uint8 TI_DQT_AMP = 3;/*幅值比较直接有关.检波幅值越小，配置值应越小*/
//uint32 DQTRSV;
uint16 DQTORG;
//uint8 TIBIAD4;
//uint8 TIBIAD3;
//uint8 TIBIAD2;
//uint8 TIBIAD1;

uint8 DCS_TITIME;

uint8 DQTORGL;
uint8 DQTORGU;

uint8 DQTRSV1;
uint8 DQTRSV2;
uint8 DQTRSV3;

uint8 CTCSS_ON_OFFSET_DATA = 0;
extern DH_U16 g_CurSignalVal;
/*********************************************************************
 * GLOBAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL FUNCTIONS
 */
static void CTC_LIMIT(void);
static void CALMUL(void);

static void CtcssWeakSignalCompare(void);
static void TackleCtcssValidReverseTimes( uint8 data_temp );
static void CorrelationCtcssCompareOk(void);
static void CheckCtcssTailLoop(void);




uint8 GetCtcDcsDecodeStatus(void)
{
    //	LED_TX=fCtdcssValid;
    return((uint8)fCtdcssValid);
}

uint8 GetCtcDcsDecodeReverseStatus(void)
{
#if FM_RF_SEL==FM_RF_AF
	return 0; /* 尾音 */
#else
	#if(FM_DECODE_SEL==NEW_DECODE_TYEP_ADC)
		return ((uint8) fCtcssReverseBurst); /* 尾音 */
	#endif
#endif
}
/*字符串倒序*/
void ReverseMem(uint8*DataIn,uint8 Len)
{
	uint8 i;
	uint8 ch;
	for(i=0;i<(Len/2);i++)
	{
		ch=DataIn[i];
		DataIn[i]=DataIn[Len-i-1];
		DataIn[Len-i-1]=ch;
	}
}






void CtcssTimerOutOfTimeSub(void)
{
    fOldTiLevel = 0;
    fCompareSubgroupCtcss = 0;
    fCtdcssValid = 0;
    AudioClosingTimeIncPer10ms = 0;
    CtcssSubgroupStateWindow = 0;
    TiTimer	= 0;
    TiPeriodMinValue = 0;
    YY = 0;
    /* ReClrCtcssStoreTimer1 */
    while( YY < ConstCtcssGroupNum )/* 8 */
    {
        CtcssPeakTimerArray[YY] = 0;/* CtcssPeakTimerArray应该是一个数组，这里是对这个数组清0 */
        YY ++;
    }
}
/*static*/ void cdcss_param_clear(void)
{
	TailNum=0;
    fOldTiLevel = 0;
    f8TimesNormalize = 0;
    fCtdcssValid = 0;
    fCompareSubgroupCtcss = 0;
    fCtdcssCmp = 0;
    fCtcssReverseBurst = 0;
    fCdcssRcvMore = 0;
    fCdcssNormalMore = 0;
    //QTDECSTU =0;
    //TOTALCIR =0;
    CtcssDeviationOff = 0;
    CtcssDeviationOn = 0;
    FilterLengthValue = 0;
    AudioClosingTimeIncPer10ms = 0;
    TiSampleStorePtr = 0;
    CtcssCorrelationDelayValue = 0;
    CtcssSubgroupOkMinEnteryValue = 0;
    CtcssSubgroupStateWindow = 0;
    TiCdcssTailTimer = 0;
    CtcssTimeStorePtr = 0;
    CtcssCorrelationCount = 0;
    CtcssBurstHalfMoreCount = 0;
    CtcssBurstHalfLessCount = 0;
    LastCtcssMatchedPeriod = 0;
    SameCorrelationCtcssCount = 0;
    CdcssCycleMinErrorMatch = 0;
    CdcssRcv3 = 0;
    CdcssRcv2 = 0;
    CdcssRcv1 = 0;
    CdcssCycleCompareByte3 = 0;
    CdcssCycleCompareByte2 = 0;
    CdcssCycleCompareByte1 = 0;
    IntTemp = 0;
    Cdcss23BitOkCnt = 0;
    CdcssQuickChangCnt = 0;
    TiNormalTimer = 0;
    MULLIER = 0;
    TEMPBCD = 0;
    TempReg = 0;
    PeakToPeakValue = 0;
    Ti1stFilterSamplesSum = 0;
    TiEndFilterSamplesSum = 0;
    TiTimer = 0;
    DifferenceOfSampleValue = 0;
    CtcssTotalTime = 0;
    QuasiCtcssPeakTime = 0;
    TiPeriodMinValue = 0;
    TiPeriodMaxValue = 0;
    //	MULLIER_temp =0;
    XX = 0;
    YY = 0;
    AA = 0;
    //AA_TEMP =0;
    //	DQTRSV =0;
    YY = 0;
    /* ReClrCtcssStoreTimer1 */
    while( YY < ConstCtcssGroupNum )/* 8 */
    {
        CtcssPeakTimerArray[YY] = 0;/* CtcssPeakTimerArray应该是一个数组，这里是对这个数组清0 */
        YY ++;
    }
    YY = 0;
    /* ReClrCtcssFilterArea */
    while( YY < ConstMaxFilterLengthValue )/* 64 */
    {
        CtcssFilterSampleArray[YY] = 0; /* CtcssFilterSampleArray应该是一个数组，这里是对这个数组清0 */
        YY ++;
    }
}




/*-------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------
                                          CTCSS
---------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------*/
static void CTC_LIMIT(void)
{
	//FMRXCHN_INFO_S lstrxfm = CHN_BLL_GetFMRxChnInfo();

//		CtcssDeviationOff = (uint8)(CurSgnl / 50);	/*关闭阀值2%*/
//		CtcssDeviationOn = (uint8)(CurSgnl * 3 / 200);	/* (CurSgnlHigh,L)*1.5% */
//		return;


 #if 1
	if(decode_type == CTCSS_DCS_NO_ACTIVE )
	{
		CtcssDeviationOff=40;
		CtcssDeviationOn=20;
	}
	else
	{
		if(g_CurSignalVal>250)/*弱信号-118*/
		{
			CtcssDeviationOff = (uint8)(CurSgnl / 50);	/*关闭阀值2%*/
			CtcssDeviationOn = (uint8)(CurSgnl * 3 / 200);	/* (CurSgnlHigh,L)*1.5% */
			return;
		}
		CtcssDeviationOn=CtcssDeviationOnBuf[CHN_BLL_GetRxSubvoiceIndex()];//
		CtcssDeviationOff=CtcssDeviationOn*1.5;

	}
	#endif

}


static void CALMUL(void)
{
    /*	计算CTCSS参数( Cycle * freq of samplling )
    	f = 7944.8Hz for ctcss/cdcss decode			*/
    /*	Samplling freq 7944.8*10 = 79448 = 13658H
    	sample cycle: 8
    	13658H*8 = 9B2C0H
     */
    MULLIER =0x9DC02;//0x9E058;//0xA0010; //0x9B2C0;
}


static void CtcssWeakSignalCompare(void)	/* Ctcss 弱信号时判断处理 ( 低信噪比 ) */
{
    CtcssBurstHalfMoreCount = 0;
    CtcssBurstHalfLessCount = 0;
    if( (CtcssSubgroupStateWindow & (uint8)0x09) != 0x09 )/* Continous 6 Cycles Ok, Regard it OK! */
    {
        /* Continous6CyclesBad */
        //BEEPIO=0;
        CtcssCorrelationCount = ConstCtcssGroupNum - ConstCorrelationLowerValue; /* = 8 - 2 = 6 //Ctcss Correlation Value Set, enable Correlation Compare */
        CtcssCorrelationExitMinCount = CtcssCorrelationMinCount;/* Set Correlation ExitMin */
        CtcssCorrelationExitMaxCount = CtcssCorrelationMaxCount;/* Set Correlation ExitMax */
        return;
    }
    else
    {
        /* NormalCtcssCompareOk */
        //BEEPIO=1;
        LastCtcssMatchedPeriod = (uint8)(CtcssSubgroupStateWindow & (uint8)0x09);
        SameCorrelationCtcssCount = 0;
        AudioClosingTimeIncPer10ms = 0;
        fCtdcssCmp = 0;
        return;
    }
}


static void TackleCtcssValidReverseTimes( uint8 data_temp )	/* 匹配次数比较 */
{
#if 1
    if( data_temp < ConstCtcssBurstThresholdNum )
    {
        /* SkipCtcssSquelchTail */
        fCtdcssCmp = 0;
        return;
    }
    else
    {
        if( Digit1InAccTable[CtcssSubgroupStateWindow] < ConstCtcssReverseExitOkNum )/* Compare Subgroup State */
        {
            /* SkipCtcssSquelchTail */
            fCtdcssCmp = 0;
            return;
        }
        else
        {
            fCtdcssValid = 0;
            //BEEPIO=0;
            fCtcssReverseBurst = 1; /* CTCSS 尾音标志 = 1 */
            //P_AF_PWR =0;
            analog_off =10;//Rx_ControlVoice(COLSE);
            AudioClosingTimeIncPer10ms = 0;
            CtcssBurstHalfMoreCount = 0;
            CtcssBurstHalfLessCount = 0;
            fCtdcssCmp = 0;
            //GpioValueCount++;
            ProcessCtcssDcsTail();
            ;///osal_start_timerEx( BUSY_ADC_EVENT, 300 );
            return;
        }
    }
#else
    fCtdcssValid = 0;
    //BEEPIO=0;
    fCtcssReverseBurst = 1; /* CTCSS 尾音标志 = 1 */
    /*P_AF_PWR =0;*/
     analog_off =2;//Rx_ControlVoice(COLSE);
    AudioClosingTimeIncPer10ms = 0;
    CtcssBurstHalfMoreCount = 0;
    CtcssBurstHalfLessCount = 0;
    fCtdcssCmp = 0;
    ProcessCtcssDcsTail();
    //osal_start_timerEx( BUSY_ADC_EVENT, 300 );
#endif
}


static void CorrelationCtcssCompareOk(void)
{
    SameCorrelationCtcssCount = 0; /* 累计次数 Clear */
    LastCtcssMatchedPeriod = CtcssCorrelationCount;/* Set Last Ctcss Matched Period value */
    if( ( AudioClosingTimeIncPer10ms - CtcssCorrelationDelayValue ) & 0x80 ) /* Audio Closing Time 延时处理 */
    {
        AudioClosingTimeIncPer10ms = 0;
    }
    else
    {
        AudioClosingTimeIncPer10ms -= CtcssCorrelationDelayValue;
    }
    fCtdcssCmp = 0;
    return;
}


/*-------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------
                                          DCS
---------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------*/
/*
void Calculate_TIBIAVG(uint8 cCtcDcsType)
{
	uint8 TEMPAD=0;
	uint16 tempADsum =0;

	if( (cCtcDcsType == CTCSS_DCS_NO_ACTIVE) || (cCtcDcsType == CTCSS_ACTIVE) )
		return;

	TEMPAD = 127;///HI_UINT16(ADC_Conversion(ADC_QT_LEVEL));	// it is get high 8 bit of adc value  //

	TIBIAD4 = TIBIAD3;
	TIBIAD3 = TIBIAD2;
	TIBIAD2 = TIBIAD1;
	TIBIAD1 = TEMPAD;
	tempADsum = TIBIAD1 +TIBIAD2 + TIBIAD3 + TIBIAD4;
	tempADsum /=4;
	TIBIAVG = (uint8)(tempADsum & (uint8)0xFF);
	TIBIAVG += 5;
}
*/

void CTCSS_DCS_STOP(void)
{/*关闭定时器*/
	//T10CR1_STA = 0;;
//	TIMER_DRV_Disable(TIM4);
	FM_APP_StopCtcDcsTimer();
}



static void NotExceedNormalBitCycle( void )
{
	// uint8 ii;
    if( fCtdcssCmp == 0 )
    {
        /* NoCdcssBitCompare */
        if( DCS_TITIME == (ConstCdcss1bitIntCycles / 2)) /* if Timer = 59/2 , Receive the bit */
        {
            /* PrepareReceiveThisCdcssBit */
            /* 这边有一段程序被注释掉 留意下 */
#if 0
            CdcssCycleCompareByte = 0;
            CdcssCycleCompareByte |= CdcssCycleCompareByte3;
            CdcssCycleCompareByte <<= 8;
            CdcssCycleCompareByte |= CdcssCycleCompareByte2;
            CdcssCycleCompareByte <<= 8;
            CdcssCycleCompareByte |= CdcssCycleCompareByte1;
            CdcssCycleCompareByte >>= 1;
            CdcssCycleCompareByte1 = CdcssCycleCompareByte & 0xFe;
            CdcssCycleCompareByte2 = (CdcssCycleCompareByte >> 8) & 0xFF;
            CdcssCycleCompareByte3 = (CdcssCycleCompareByte >> 16) & 0xFF;
            if( fOldTiLevel )
            {
                CdcssCycleCompareByte3 |= 0x80;
            }
            else
            {
                CdcssCycleCompareByte3 &= ~0x80;
            }
#endif
            fCtdcssCmp = 1;	/* Compare flag = 1 */
            CtcssCorrelationCount = 0xFF;
            CdcssCycleMinErrorMatch = 23;
            return;
        }
        else
        {
            if( DCS_TITIME >= ConstCdcss1bitIntCycles )	/* if Timer >= 59 ,	Timer = Timer - 59 */
            {
                /* ExceedCdcss1BitTime */
                DCS_TITIME -= ConstCdcss1bitIntCycles;
                return;
            }
            else
            {
                return;
            }
        }
    }
    else
    {
        /* CompareCdcss23Bit */
        CtcssCorrelationCount ++;
        if( CtcssCorrelationCount == 0 )
        {
            if( fCdcssRcvMore == 0 )
            {
                /* PassRcvCheck */
                if( fCdcssNormalMore )
                {
                    /* NowRcvCatchUpWithNormal */
                    fCdcssNormalMore = 0;
                }
                else
                {
                    fCdcssRcvMore = 1;
                }
                #if 1///2014-12-19 1:36:51
				if( fOldTiLevel == 0 )
				{
					CdcssRcv1 = (uint8)((uint8)(CdcssRcv1 >>1)|(uint8)(CdcssRcv2 <<(8-1)));
					CdcssRcv1 &= (uint8)0xfe;
					CdcssRcv2 = (uint8)((uint8)(CdcssRcv2 >>1)|(uint8)(CdcssRcv3 <<(8-1)));
					CdcssRcv3 >>= 1;
					#if 0
					#pragma asm
					 CLRC 
					 MOV		A,_CdcssRcv3
					 RORC		A
					 MOV		_CdcssRcv3,A
					 MOV		A,_CdcssRcv2
					 RORC		A
					 MOV		_CdcssRcv2,A
					 MOV		A,_CdcssRcv1
					 RORC		A
					 MOV		A,#0FEH
					 AND		A	
					 MOV		_CdcssRcv1,A
				 #pragma endasm	
				 #endif
				}
				else
				{
					CdcssRcv1 = (uint8)((uint8)(CdcssRcv1 >>1)|(uint8)(CdcssRcv2 <<(8-1)));
					CdcssRcv1 &= (uint8)0xfe;
					CdcssRcv2 = (uint8)((uint8)(CdcssRcv2 >>1)|(uint8)(CdcssRcv3 <<(8-1)));
					CdcssRcv3 >>= 1;
					CdcssRcv3 |= (uint8)0x80;
					#if 0
					#pragma asm
					 SETC 
					 MOV		A,_CdcssRcv3
					 RORC		A
					 MOV		_CdcssRcv3,A
					 MOV		A,_CdcssRcv2
					 RORC		A
					 MOV		_CdcssRcv2,A
					 MOV		A,_CdcssRcv1
					 RORC		A
					 MOV		A,#0FEH
					 AND		A	
					 MOV		_CdcssRcv1,A
				 #pragma endasm
				 #endif
				}
       #endif
                CdcssCycleCompareByte1 = CdcssRcv1;
                CdcssCycleCompareByte2 = CdcssRcv2;
                CdcssCycleCompareByte3 = CdcssRcv3;
                return;
            }
            else
            {
                fCtdcssCmp = 0;	/* Discard this Bit.(Too quick)丢弃这个bit */
                return;
            }
        }
        else
        {
            /*StartCompareCdcss23Bit //Start Cdcss compare*/
            if( CtcssCorrelationCount < 24 )
            {
                YY = (uint8)(DQTCODE1 ^ CdcssCycleCompareByte1);
                IntTemp = Digit1InAccTable[YY];
                YY = (uint8)(DQTCODE2 ^ CdcssCycleCompareByte2);
                IntTemp += Digit1InAccTable[YY];
                YY = (uint8)(DQTCODE3 ^ CdcssCycleCompareByte3);
                IntTemp += Digit1InAccTable[YY];	/* [IntTemp] = CDCSS码不匹配的位数 */
                if( IntTemp < CdcssCycleMinErrorMatch )
                {
                    CdcssCycleMinErrorMatch = IntTemp;    /* 保留最小的不匹配数 */
                }
			 #if 1///2014-12-19 1:36:51
				CdcssCycleCompareByte1 |= (uint8)(CdcssCycleCompareByte3 >>(8-1));
				CdcssCycleCompareByte3 = (uint8)((uint8)(CdcssCycleCompareByte3 <<1)|(uint8)(CdcssCycleCompareByte2 >>(8-1)));
				CdcssCycleCompareByte2 = (uint8)((uint8)(CdcssCycleCompareByte2 <<1)|(uint8)(CdcssCycleCompareByte1 >>(8-1)));
				CdcssCycleCompareByte1 <<= 1;
			 #else
			   #pragma asm
				 CLRC
				 MOV		A,_CdcssCycleCompareByte1
				 ROLC		A
				 MOV		_CdcssCycleCompareByte1,A
				 MOV		A,_CdcssCycleCompareByte2
				 ROLC		A
				 MOV		_CdcssCycleCompareByte2,A
				 MOV		A,_CdcssCycleCompareByte3
				 ROLC		A
				 MOV		_CdcssCycleCompareByte3,A
				 BNC 		AAA
				 MOV		A,_CdcssCycleCompareByte1
				 OR			A,#02H
				 MOV		_CdcssCycleCompareByte1,A
			AAA:
			   #pragma endasm
			 #endif
                /* 这边有一段程序被注释掉 留意下 */
			#if 0
                if( DQTCODE3 & 0x80 )
                {
                    DQTCODE1 |= 0x01;
                }
                DQTCODE = DQTCODE3;
                DQTCODE <<= 8;
                DQTCODE |= DQTCODE2;
                DQTCODE <<= 8;
                DQTCODE |= DQTCODE1;
                DQTCODE <<= 1;
                DQTCODE1 = DQTCODE & 0xFF;
                DQTCODE2 = (DQTCODE >> 8) & 0xFF;
                DQTCODE3 = (DQTCODE >> 16) & 0xFF;
		#endif
                return;
            }
            /* Cdcss compare over */
            fCtdcssCmp = 0;
            if( fCtdcssValid == 0 )						/* When Cdcss not Matched */
            {
                if( CdcssCycleMinErrorMatch < 4 )
                {
                    YY = CdcssCycleMinErrorMatch;
                    AA = CdcssErrorBitChangeTable[YY];	/* If Continous 7 Group is OK, Set CDCSS Valid */
                    AA += Cdcss23BitOkCnt;
                    if( AA >= 22)	////////* if [Cdcss23BitOkCnt] >= 22, Set Cdcss Valid */
                    {
                        fCtdcssValid = 1;				/* Cdcss Valid flag = 1 */
                        //BEEPIO=1;
						//analog_on = 2;	// Rx_ControlVoice(OPEN);
                        AudioClosingTimeIncPer10ms = 0;
                        ///Cdcss23BitOkCnt = 0;
                        CdcssQuickChangCnt = 0;
                        //#ifndef DEVICE_DEBUG
                        //SPEAKING_TURN_ON();
                        //MUTE_TURN_ON();
                        //CTCSS_DCS_SCAN_ACTIVE =1;
                        //#endif
                    }
                    else
                    {
                        Cdcss23BitOkCnt = AA;
                    }
                }
                else
                {
                    if( Cdcss23BitOkCnt >= 2 )
                    {
                        Cdcss23BitOkCnt -= 2;
                    }
                    else
                    {
                        Cdcss23BitOkCnt = 0;
                    }
                }
            }
            else
            {
                if( CdcssCycleMinErrorMatch < 5 )	/* When Cdcss Matched */
                {
                    AudioClosingTimeIncPer10ms = 0;
                }
            }
            return;
        }
    }
}



static void TiLevelNoChange(void)
{
    TiNormalTimer ++;
    if( TiNormalTimer < ConstCdcss1bitIntCycles )	/* 59 */
    {
        /* NotExceedNormalBitCycle 没有超过正常位循环 */
        NotExceedNormalBitCycle();
    }
    else
    {
        TiNormalTimer = 0;
        if( fCdcssNormalMore )
        {
            /* NowInsertRcvData1Bit */
            if( fOldTiLevel == 0 )/* Old Ti Level Is Low */
            {
                /* 这边就是用来保存亚音频所传输的数据 刚才检测到的数据根据fOldTiLevel来判断 */
                CdcssRcv1 = (uint8)((uint8)(CdcssRcv1 >> 1) | (uint8)(CdcssRcv2 << (8 - 1)));
                CdcssRcv1 &= (uint8)0xfe;
                CdcssRcv2 = (uint8)((uint8)(CdcssRcv2 >> 1) | (uint8)(CdcssRcv3 << (8 - 1)));
                CdcssRcv3 >>= 1;
            }
            else
            {
                CdcssRcv1 = (uint8)((uint8)(CdcssRcv1 >> 1) | (uint8)(CdcssRcv2 << (8 - 1)));
                CdcssRcv1 &= (uint8)0xfe;
                CdcssRcv2 = (uint8)((uint8)(CdcssRcv2 >> 1) | (uint8)(CdcssRcv3 << (8 - 1)));
                CdcssRcv3 >>= 1;
                CdcssRcv3 |= (uint8)0x80;
            }
            NotExceedNormalBitCycle();
        }
        else
        {
            if( fCdcssRcvMore )
            {
                /* NowNormalCatchUpWithRcv */
                fCdcssRcvMore = 0;
                NotExceedNormalBitCycle();
            }
            else
            {
                fCdcssNormalMore = 1;
                /* NotExceedNormalBitCycle 没有超过正常位循环 */
                NotExceedNormalBitCycle();
            }
        }
    }
}



uint32 DCS_GetGrayCode(uint16 iDecode)
{
	CurSgnl = iDecode;
	/* caj_cdcss_bch_code */
	DQTORGL = (uint8)(CurSgnl & (uint8)0xff);
	DQTORGU = (uint8)((CurSgnl >> 8) & (uint8)0xff);
	DQTORG = BUILD_UINT16( DQTORGU, DQTORGL );
	DQTCODE1 = DQTORGL;
	DQTCODE2 = (uint8)(DQTORGU | (uint8)0x08);
	DQTBCHL = 0;
	DQTBCHU = 0;
	XX = 11;
	while( XX != (uint8)0xFF )
	{
		if( DQTORG & (uint16)0x0001 )
		{
		    DQTBCHL ^= BCHL4TAB[XX];
		    DQTBCHU ^= BCHU7TAB[XX];
		}
		DQTORG >>= 1;
		XX --;
	}
	DQTCODE3 = DQTBCHU;
	DQTCODE2 |= (uint8)(DQTBCHL & (uint8)0xF0);
	if( decode_type == DCS_ACTIVE )
	{
		DQTCODE1 = (uint8)~DQTCODE1;
		DQTCODE2 = (uint8)~DQTCODE2;
		DQTCODE3 = (uint8)~DQTCODE3;
		DQTCODE3 &= (uint8)0x7f;
	}


#if 0
	GloVarFlag.SysPrintfEnableFlag=1;
	printk("CurSgnl=0x%X\r\n",CurSgnl);
	printk("DQTCODE3=0x%X\r\n",DQTCODE3);
	printk("DQTCODE2=0x%X\r\n",DQTCODE2);
	printk("DQTCODE1=0x%X\r\n",DQTCODE1);


	GloVarFlag.SysPrintfEnableFlag=0;
#endif







	DQTCODE3 = (uint8)((uint8)(DQTCODE3 << 1) | (uint8)(DQTCODE2 >> (8 - 1)));
	DQTCODE2 = (uint8)((uint8)(DQTCODE2 << 1) | (uint8)(DQTCODE1 >> (8 - 1)));
	DQTCODE1 <<= 1;		

	return (DQTCODE3<<16) | (DQTCODE2<<8) | DQTCODE1;
}


/*====================================================================================

 CTCSS/CDCSS信令接收初始化处理

====================================================================================*/
void start_ctcdcs_decode( uint8 cDecodeType, uint16 iDecode )
{
    uint8 FilterLengthValue_temp;
    decode_type = cDecodeType;
//ReverseMem((uint8 *)&iDecode,2);
    //BEEPIO = 0;
    if( decode_type == CTCSS_DCS_NO_ACTIVE )
    {
        iDecode = 556;    //return;
    }
    //cCtcssCdcssValid_DE = cDecodeType;
    //CTCSS_DCS_DECODE = iDecode;
    ;//ITC_SetSoftwarePriority(ITC_IRQ_TIM3_OVF, ITC_PRIORITYLEVEL_2);
    //BEEPIO = 0;
    if( decode_type <= CTCSS_ACTIVE )
    {
        //T10CR1_STA = 0;;
        //TIMER_DRV_Disable(TIM4);
        FM_APP_StopCtcDcsTimer();
        //GpioValueCount =0;
        cdcss_param_clear();
        CurSgnl = iDecode;
        //cTX_RX_Flag =0;
        //QTDECSTU =0;						/* 模拟亚音频解码状态 */
        fOldTiLevel = 0;						/* 旧CTCSS水平 */
        //TOTALCIR = ConstCtcssGroupNum;/* Cycle = 8 */
        /* CALQTRAG */ /* Calculate CTCSS decode counter range */
        /* Calculate Group(8 Unit) Min Enter Value */
        CTC_LIMIT();
        CALMUL();
        TEMPBCD = CurSgnl + CtcssDeviationOn;
        MULLIER /= TEMPBCD;
        CtcssEnterMinCount = (uint16)(MULLIER & (uint16)0xFFFF);
        /* Calculate Group(8 Unit) Min Exit Value */
        CALMUL();
        TEMPBCD = CurSgnl + CtcssDeviationOff;
        MULLIER /= TEMPBCD;
        CtcssExitMinCount = (uint16)(MULLIER & (uint16)0xFFFF);
        /* Calculate Group(8 Unit) Max Enter Value */
        CALMUL();
        TEMPBCD = CurSgnl - CtcssDeviationOn;
        MULLIER /= TEMPBCD;
        CtcssEnterMaxCount = (uint16)(MULLIER & (uint16)0xFFFF);
        /* Calculate Group(8 Unit) Max Exit Value */
        CALMUL();
        TEMPBCD = CurSgnl - CtcssDeviationOff;
        MULLIER /= TEMPBCD;
        CtcssExitMaxCount = (uint16)(MULLIER & (uint16)0xFFFF);
        /* Group(8 Unit) Criterion(标准) Value */
        CALMUL();
        TEMPBCD = CurSgnl;
        MULLIER /= TEMPBCD;
        CtcssCriterionCount = (uint16)(MULLIER & (uint16)0xFFFF);
        CtcssBurstHalfMoreMaxCount = CtcssCriterionCount;
        CtcssBurstHalfLessMinCount = CtcssCriterionCount;




#if 0
	GloVarFlag.SysPrintfEnableFlag=1;
	printk("CurSgnl=%d\r\n",CurSgnl);
	printk("CtcssDeviationOn=%d\r\n",CtcssDeviationOn);
	printk("CtcssDeviationOff=%d\r\n",CtcssDeviationOff);
	printk("CtcssEnterMinCount=%d\r\n",CtcssEnterMinCount);
	printk("CtcssEnterMaxCount=%d\r\n",CtcssEnterMaxCount);
	printk("CtcssExitMinCount=%d\r\n",CtcssExitMinCount);
	printk("CtcssExitMaxCount=%d\r\n",CtcssExitMaxCount);

	
	GloVarFlag.SysPrintfEnableFlag=0;
#endif




		
        /* 1 Unit Criterion(标准) Value */
        CtcssCriterionCount /= 8;
        //TailNum = CtcssCriterionCount;/* cykang 2010-08-05 */
        /* 0.25 Unit Criterion(标准) Value */
        CtcssCriterionCount /= 4;
        //TailNum += CtcssCriterionCount;/* cykang 2010-08-05 */
        /* Half More Criterion(标准) Value */
        /* Half More Min = 8 + 0.25 */
        CtcssBurstHalfMoreMaxCount += CtcssCriterionCount;
        CtcssBurstHalfMoreMinCount = CtcssBurstHalfMoreMaxCount;
        /* Half More Max = 8 + 0.75 */
        CtcssBurstHalfMoreMaxCount += CtcssCriterionCount;
        CtcssBurstHalfMoreMaxCount += CtcssCriterionCount;
        /* Half Less Criterion(标准) Value */
        /* Half Less Max = 8 - 0.25 */
        CtcssBurstHalfLessMinCount -= CtcssCriterionCount;
        CtcssBurstHalfLessMaxCount = CtcssBurstHalfLessMinCount;
        /* Half Less Min = 8 - 0.75 */
        CtcssBurstHalfLessMinCount -= CtcssCriterionCount;
        CtcssBurstHalfLessMinCount -= CtcssCriterionCount;
        /* Calculate Exit Unit Value */
        CtcssExitUnitMaxCount = CtcssExitMaxCount;
        CtcssExitUnitMaxCount /= 8;
        CtcssExitUnitMinCount = CtcssExitMinCount;
        CtcssExitUnitMinCount /= 8;
        /* Calculate Filter Length Value */
        /* SamplFreq 7944.8/67.0 = 118 ; 7944.8/250.3 = 32
         * 118/2 = 59 ; 32/2 = 16
         * 大致范围：16 -- 59							*/
        FilterLengthValue_temp = (uint8)((CtcssExitUnitMaxCount & (uint8)0xFF) / 2);
        if( FilterLengthValue_temp >= ConstMaxFilterLengthValue )
        {
            FilterLengthValue = ConstMaxFilterLengthValue;
        }
        else
        {
            FilterLengthValue = FilterLengthValue_temp;
        }
#if 1
        f8TimesNormalize = 0; /* Not use */
        if( FilterLengthValue < 32 )
        {
            f8TimesNormalize = 1; /* Set Filter2 Average Divisor Flag */
        }
#endif
        /* cykang 2010-08-09 */
        //if( CurSgnl >= 2503 )
        //{
        //	FilterLengthValue --;
        //}
        /* Calculate Ctcss Correlation Delay Value */
        TempReg = CurSgnl / 100; /* 6 - 25 */
        AA = (uint8)(TempReg & (uint8)0x1F);
        if( AA >= 20 )
        {
            CtcssCorrelationDelayValue = ConstCorrelationDelayFor200Hz;/* >= 200Hz, = 4 */
        }
        else if( AA >= 10 )
        {
            CtcssCorrelationDelayValue = ConstCorrelationDelayFor100Hz;/* >= 100Hz, = 5 */
        }
        else
        {
            CtcssCorrelationDelayValue = ConstCorrelationDelayFor67Hz;/* < 100Hz, = 9 */
        }
        /* Calculate Ctcss Sub-group Ok Entery Times */
        YY = (uint8)(TempReg & (uint8)0x1F);
        CtcssSubgroupOkMinEnteryValue = (uint8)((CtcssCharacterValueTable[YY] >> 4) & (uint8)0x0F); /* = 3, 4, 5 */
        /* Calculate Peak To Peak Value to compare */
        YY = (uint8)(CtcssCharacterValueTable[YY] & (uint8)0x0F); /* y = 3(<100Hz), 2(<200Hz), 1(>=200Hz) */
        PeakToPeakValue = 0;
        PeakToPeakValue = FilterLengthValue * YY;
        /* Calculate Sub-group( 3 unit ) Value */
        /* Sub-group Max Value ( 3 unit ) */
        CtcssSubgroupMaxCount = 1;
        CtcssSubgroupMaxCount += CtcssExitUnitMaxCount * ConstSubgroupNum;/* *3 */
        /* Sub-group Min Value ( 3 unit ) */
        CtcssSubgroupMinCount = 0;
        CtcssSubgroupMinCount = CtcssExitUnitMinCount * ConstSubgroupNum;
        /* Calculate Correlation Value */
        CtcssCorrelationMinCount = CtcssExitMinCount;
        CtcssCorrelationMaxCount = CtcssExitMaxCount;
        /* Correlation Min Value ( 6 unit ) */
        CtcssCorrelationMinCount -= CtcssExitUnitMinCount * 2;
        /* Correlation Max Value ( 6 unit ) */
        CtcssCorrelationMaxCount -= CtcssExitUnitMaxCount * 2;
#if 1
        CtcssTimerOutOfTimeSub();
#else
        /* 相关变量初始化 */
        fOldTiLevel = 0;
        fCompareSubgroupCtcss = 0;
        fCtdcssValid = 0;
        AudioClosingTimeIncPer10ms = 0;
        CtcssSubgroupStateWindow = 0;
        TiTimer	= 0;
        TiPeriodMinValue = 0;
        YY = 0;
        /* ReClrCtcssStoreTimer1 */
        while( YY < ConstCtcssGroupNum )/* 8 */
        {
            CtcssPeakTimerArray[YY] = 0;/* CtcssPeakTimerArray应该是一个数组，这里是对这个数组清0 */
            YY ++;
        }
#endif
        YY = 0;
        /* ReClrCtcssFilterArea */
        while( YY < ConstMaxFilterLengthValue )/* 64 */
        {
            CtcssFilterSampleArray[YY] = 0; /* CtcssFilterSampleArray应该是一个数组，这里是对这个数组清0 */
            YY ++;
        }
        ;//TIM3_ITConfig( TIM3_IT_UPDATE, ENABLE );
        ;//TIM3_Cmd(ENABLE);
	#if 0
       T10CR1_STA = 0;
		T10CR1_IE = 0  ;
		T10DR = 250; //CTCSS_TIME;
		T10CR1 = 0xA0;
	#else
		FM_APP_OpenCtcDcsTimer();
//		TIMER_DRV_Enable(TIM4);
	#endif
    }
    else
    {
     #ifdef DCS_EN	
        //T10CR1_STA = 0;;
//        TIMER_DRV_Disable(TIM4);
        FM_APP_StopCtcDcsTimer();
        //cTX_RX_Flag =0;
        //QTDECSTU =0;						/* 模拟亚音频解码状态 */
        fOldTiLevel = 0;						/* 旧CTCSS水平 */
        CurSgnl = iDecode;
        /* caj_cdcss_bch_code */
        DQTORGL = (uint8)(CurSgnl & (uint8)0xff);
        DQTORGU = (uint8)((CurSgnl >> 8) & (uint8)0xff);
        DQTORG = BUILD_UINT16( DQTORGU, DQTORGL );
        DQTCODE1 = DQTORGL;
        DQTCODE2 = (uint8)(DQTORGU | (uint8)0x08);
        DQTBCHL = 0;
        DQTBCHU = 0;
        XX = 11;
        while( XX != (uint8)0xFF )
        {
            if( DQTORG & (uint16)0x0001 )
            {
                DQTBCHL ^= BCHL4TAB[XX];
                DQTBCHU ^= BCHU7TAB[XX];
            }
            DQTORG >>= 1;
            XX --;
        }
        DQTCODE3 = DQTBCHU;
        DQTCODE2 |= (uint8)(DQTBCHL & (uint8)0xF0);
	if( decode_type == DCS_ACTIVE )
        {
            DQTCODE1 = (uint8)~DQTCODE1;
            DQTCODE2 = (uint8)~DQTCODE2;
            DQTCODE3 = (uint8)~DQTCODE3;
            DQTCODE3 &= (uint8)0x7f;
        }


#if 0
	GloVarFlag.SysPrintfEnableFlag=1;
	printk("CurSgnl=0x%X\r\n",CurSgnl);
	printk("DQTCODE3=0x%X\r\n",DQTCODE3);
	printk("DQTCODE2=0x%X\r\n",DQTCODE2);
	printk("DQTCODE1=0x%X\r\n",DQTCODE1);

	
	GloVarFlag.SysPrintfEnableFlag=0;
#endif







        DQTCODE3 = (uint8)((uint8)(DQTCODE3 << 1) | (uint8)(DQTCODE2 >> (8 - 1)));
        DQTCODE2 = (uint8)((uint8)(DQTCODE2 << 1) | (uint8)(DQTCODE1 >> (8 - 1)));
        DQTCODE1 <<= 1;		
        DQTRSV1 = 0;
        DQTRSV2 = 0;
        DQTRSV3 = 0;
        DQTORGU = 0;
        DQTBCHL = 0;
        DQTBCHU = 0;
        DQTBCHL = 0;
        DQTBCHU = 0;
        /* cdcss_param_clear CDCSS接收参数清零处理 ( 地址范围：和CTDCSS接收处理有关的变量都清0 )*/
        cdcss_param_clear();
        fOldTiLevel = 0;
        fCtdcssCmp = 0;
        fCtdcssValid = 0;
        AudioClosingTimeIncPer10ms = 0;
        DCS_TITIME = 0;
        ;//TIM3_TimeBaseInit(TIME3_PRE,TIME3_DATA); //( TIM3_PRESCALER_1, CTCSS_DCS_DECDOE_TIMER );
        ;//TIM3_ITConfig( TIM3_IT_UPDATE, ENABLE );
        ;//TIM3_Cmd(ENABLE);
     #endif   
	 #if 0
    T10CR1_STA = 0;
    T10CR1_IE = 0  ;
    T10DR = 240; //CTCSS_TIME;
    T10CR1 = 0xA0;
	#else
//		TIMER_DRV_Enable(TIM4);
		FM_APP_OpenCtcDcsTimer();
	#endif
    }
}

//uint16 TictcTimer = 0;
DH_U16 cdindex = 0;
//extern  DH_U8 adcindex;
/*==============================================================================================

 CTCSS/CDCSS decode func ( 7944.8=125.8684926 )		解码功能

==============================================================================================*/
void ctcss_cdcss_decode_interrupt(void)
{
    //LED_TX=~LED_TX;
    //decode_type=1;

	static DH_U8 TailTimes=0;
	TailTimes++;
	if(TailTimes>64)//8ms++=8000/124
	{
		TailTimes=0;
		TackleCtcssCdcssValidFlagFunc();
	}

    if( decode_type <= CTCSS_ACTIVE )
    {
        TiTimer ++;
        TiSampleStorePtr ++;
        if( TiSampleStorePtr >= FilterLengthValue )
        {
            TiSampleStorePtr = 0;
        }
        /* cykang 2010-08-05 */
        /* ctcss的尾音和DCS的尾音都会被1846给解成0电平 */
#if 0
        if( fCtdcssValid )
        {
            //BEEPIO=1;
            if( GpioValueCount >= TailNum )
            {
                TackleCtcssValidReverseTimes( CtcssBurstHalfLessCount );
            }
        }
        else
            ;//BEEPIO=0;
#endif
        AD = 1;//ADC_DRV_GetChnValueDQT(ADC_QT);///
//        AD = ADC_DRV_GetChnValue(adcindex)>>2;///
	///GloVarFlag.SysPrintfEnableFlag=1;
	//printk("DQT_ADC:%d\r\n",AD);
	//GloVarFlag.SysPrintfEnableFlag=0;
	
        //LAMP_LED = ~LAMP_LED;
        Ti1stFilterSamplesSum -= CtcssFilterSampleArray[TiSampleStorePtr];
        /* AD 是否可以再上一次中断进来时开启，下一次进来再读取 这样子做比较好 */
        CtcssFilterSampleArray[TiSampleStorePtr] = AD;
        Ti1stFilterSamplesSum += CtcssFilterSampleArray[TiSampleStorePtr];
        TiEndFilterSamplesSum = Ti1stFilterSamplesSum;

//        		uint8 *pbuf = (uint8 *)FlashBuf;
//        		if(cdindex<9000) pbuf[cdindex++]=AD;
//
//        		if(cdindex==9000)
//        		{
//        			cdindex =9100;
//        		}

        if( fCtdcssCmp == 0 )
        {
            /* CtcssPeakDetect */
            if( fOldTiLevel == 0 )
            {
                /* FindRisingEdge */
                DifferenceOfSampleValue = TiEndFilterSamplesSum - TiPeriodMinValue;
                if( TiEndFilterSamplesSum < TiPeriodMinValue )
                {
                    /* SaveQuasiBottomValue */
                    TiPeriodMinValue = TiEndFilterSamplesSum;/* Regard the Current Filted TI is Min */
                    /* QuasiExitCtcssDetectInterrupt2 */
                    return;
                }
                else
                {
                    if( DifferenceOfSampleValue < PeakToPeakValue )
                    {
                        /* QuasiExitCtcssDetectInterrupt2 */
                        return;
                    }
                    else
                    {
                        fOldTiLevel = 1; /* New Rising Curve Arrives */

                     	//RED_LED_ON;
                        fCompareSubgroupCtcss = 1; /* fCompareSubgroupCtcss flag = 1 */
                        TiPeriodMaxValue = TiEndFilterSamplesSum; /* Save Peak Value & Time ( Maybe ) */
                        QuasiCtcssPeakTime = TiTimer;
                        /* QuasiExitCtcssDetectInterrupt */
                        return;
                    }
                }
            }
            else if( fCompareSubgroupCtcss == 0 )
            {
                /* FindFallingEdge */
                DifferenceOfSampleValue	= TiPeriodMaxValue - TiEndFilterSamplesSum;
                /* TiPeriodMax < EndFilterSamples , Save New TiPeriodMax */
                if( TiPeriodMaxValue < TiEndFilterSamplesSum )
                {
                    /* SaveQuasiMaxTime */
                    TiPeriodMaxValue = TiEndFilterSamplesSum; /* Save Peak Value & Time ( Maybe ) */
                    QuasiCtcssPeakTime = TiTimer;
                    /* QuasiExitCtcssDetectInterrupt */
                    return;
                }
                else
                {
                    /* TiPeriodMax - EndFilterSamples < PeakToPeak, Exit */
                    if( DifferenceOfSampleValue < PeakToPeakValue )
                    {
                        /* QuasiExitCtcssDetectInterrupt2 */
                        return;
                    }
                    else
                    {
                        fOldTiLevel = 0; /* New Falling Curve Arrives */
                        //RED_LED_OFF;
                        /* Save One Group Time ( Peak To Peak * Groups ) */
                        CtcssTotalTime = QuasiCtcssPeakTime - CtcssPeakTimerArray[CtcssTimeStorePtr]; /* 8个周期 */
                        CtcssPeakTimerArray[CtcssTimeStorePtr] = QuasiCtcssPeakTime; /* Store This Peak Time */
                        CtcssLastPeakTime = QuasiCtcssPeakTime; /* Store For Sub-group Ctcss Compare */
                        fCtdcssCmp = 1; /* Ctcss Compare Flag = 1 */
                        CtcssCorrelationCount = 0;
                        TiPeriodMinValue = TiEndFilterSamplesSum;
                        /* QuasiExitCtcssDetectInterrupt2 */
                        return;
                    }
                }
            }
            else
            {

                /* CompareSubgroupCtcss */ /* Judge Subgroup Ctcss Period, In The End Of TI Low Level */
                fCompareSubgroupCtcss = 0;
                YY = (uint8)((CtcssTimeStorePtr - ConstSubgroupNum) & (uint8)0x07);/* = 3, Move Pointer to ConstSubgroupNum Before //&0x07 -- Assure Inside Area */
                CtcssTotalTime = CtcssLastPeakTime - CtcssPeakTimerArray[YY];/* Calculate Sub-Period //Subgroup Period */
                if( (CtcssTotalTime >= CtcssSubgroupMinCount) && (CtcssTotalTime <= CtcssSubgroupMaxCount) )
                {
                    CtcssSubgroupStateWindow *= 2;
                    CtcssSubgroupStateWindow ++;
                    //RED_LED_ON;
                }
                else
                {
                    CtcssSubgroupStateWindow *= 2; /* Save This Half Group CTCSS state */
                    //RED_LED_OFF;
                }
                CtcssTimeStorePtr ++;
                CtcssTimeStorePtr &= (uint8)0x07;/* Point to Next Position */


                return;
            }
        }
        else
        {
		#if 1
//            CheckCtcssTailLoop();

            /* CalculateCtcssArray */ /* Compare Ctcss Period */
            if( fCtcssReverseBurst )
            {
                fCtdcssCmp = 0;
                return;
            }
            if( fCtdcssValid )
            {
                /* CtcssMatched */ /* 信令打开时处理 */

				//CheckCtcssTailLoop();
				if(CtcssTotalTime>1242||CtcssTotalTime<1114)/*52--58HZ*/
				{
					TailNum=0;
				}
		
                if( CtcssCorrelationCount < ( ConstCtcssGroupNum - ConstCorrelationLowerValue ) )
                {
                    /* CtcssRxTailCompare */ /* Ctcss 尾音判断处理 */
                    /* CtcssTotalTime >= MoreMin ? ( 8.25周期 ) */
                    if( CtcssTotalTime < CtcssBurstHalfMoreMinCount )
                    {
                        /* MaybeLessReverseBurst */
                        /* CtcssTotalTime >= LessMin ? ( 7.25周期 ) */
                        if( CtcssTotalTime < CtcssBurstHalfLessMinCount )
                        {
						#if 1	/* cykang 2010-08-05 */
                            /* CtcssWeakSignalCompare */
                            CtcssWeakSignalCompare();
						#else
                            /* DisgardThisValidReverse */
                            CtcssBurstHalfMoreCount = 0;
                            CtcssBurstHalfLessCount = 0;
                            fCtdcssCmp = 0;
                            return;
						#endif
                        }
                        else
                        {
                            /* CtcssTotalTime <= LessMax ? ( 7.75周期 ) */
                            if( CtcssTotalTime > CtcssBurstHalfLessMaxCount )
                            {
#if 1	/* cykang 2010-08-05 */
                                /* CtcssWeakSignalCompare */
                                CtcssWeakSignalCompare();
#else
                                /* DisgardThisValidReverse */
                                CtcssBurstHalfMoreCount = 0;
                                CtcssBurstHalfLessCount = 0;
                                fCtdcssCmp = 0;
                                return;
#endif
                            }
                            else
                            {
                            	CheckCtcssTailLoop();	
                                if( Digit1InAccTable[CtcssSubgroupStateWindow] < ConstCtcssReverseEnterOkNum)/* Compare Subgroup State */
                                {
                                    /* DisgardThisValidReverse */
                                    CtcssBurstHalfMoreCount = 0;
                                    CtcssBurstHalfLessCount = 0;
                                    fCtdcssCmp = 0;
                                    return;
                                }
                                else
                                {
#if 1	/* cykang 2010-08-05 */
                                    CtcssBurstHalfMoreCount = 0;
                                    CtcssBurstHalfLessCount ++;/* 匹配次数累计 */
                                    /* TackleCtcssValidReverseTimes */
                                    TackleCtcssValidReverseTimes( CtcssBurstHalfLessCount );
#else
                                    /* DisgardThisValidReverse */
                                    CtcssBurstHalfMoreCount = 0;
                                    CtcssBurstHalfLessCount = 0;
                                    fCtdcssCmp = 0;
                                    return;
#endif
                                }
                            }
                        }
                    }
                    else
                    {
                        /* CtcssTotalTime <= MoreMax ? ( 8.75周期 ) */
                        if( CtcssTotalTime > CtcssBurstHalfMoreMaxCount )
                        {
#if 1	/* cykang 2010-08-05 */
                            /* CtcssWeakSignalCompare */
                            CtcssWeakSignalCompare();
#else
                            /* DisgardThisValidReverse */
                            CtcssBurstHalfMoreCount = 0;
                            CtcssBurstHalfLessCount = 0;
                            fCtdcssCmp = 0;
                            return;
#endif
                        }
                        else
                        {
                        	CheckCtcssTailLoop();	
                            if( Digit1InAccTable[CtcssSubgroupStateWindow] < ConstCtcssReverseEnterOkNum )
                            {
                                /* DisgardThisValidReverse */
                                CtcssBurstHalfMoreCount = 0;
                                CtcssBurstHalfLessCount = 0;
                                fCtdcssCmp = 0;
                                return;
                            }
                            else
                            {
                                /*CtcssBurstHalfMoreCount =1; 匹配次数累计 */
                                CtcssBurstHalfLessCount = 0;
                                CtcssBurstHalfMoreCount ++;
                                /* TackleCtcssValidReverseTimes */
                                TackleCtcssValidReverseTimes( CtcssBurstHalfMoreCount );
                            }
                        }
                    }
                }
                else
                {
#if 1	/* cykang 2010-08-05 */
					CheckCtcssTailLoop();
                    /* GroupCtcssCorrelationCompare */ /* Ctcss 低信噪比 or 信令关断时处理 */
                    if( CtcssTotalTime < CtcssCorrelationExitMinCount )
                    {
                        /* CtcssCorrelationCompareExit */
                        fCtdcssCmp = 0;
                        return;
                    }
                    if( CtcssCorrelationExitMaxCount < CtcssTotalTime )
                    {
                        /* PrepareNextCtcssCorrelationCompare */
                        CtcssCorrelationCount ++;
                        if( CtcssCorrelationCount >= ( ConstCtcssGroupNum + ConstCorrelationUpperValue + 1 ) )/* 8 + 4 + 1 */
                        {
                            /* CtcssCorrelationCompareExit */
                            fCtdcssCmp = 0;
                            return;
                        }
                        else
                        {
                            CtcssCorrelationExitMinCount += CtcssExitUnitMinCount;
                            CtcssCorrelationExitMaxCount += CtcssExitUnitMaxCount;
                            /* ExitCtcssDetectInterrupt */
                            return;
                        }
                    }
                    else
                    {
                        if( CtcssCorrelationCount == ConstCtcssGroupNum )
                        {
                            /* NormalCtcssCompareOk */ /* 等于8周期时, Regard Ctcss OK ! */
                            LastCtcssMatchedPeriod = CtcssCorrelationCount;/* Set Last Ctcss Matched Period value */
                            SameCorrelationCtcssCount = 0;
                            AudioClosingTimeIncPer10ms = 0;
                            fCtdcssCmp = 0;
                            return;
                        }
                        else
                        {
                            if( Digit1InAccTable[CtcssSubgroupStateWindow] >= CtcssSubgroupOkMinEnteryValue )/* Compare Subgroup State //= 3, 4, 5 */
                            {
                                /* CorrelationCtcssCompareOk */
                                CorrelationCtcssCompareOk();
                            }
                            else
                            {
                                if( CtcssCorrelationCount != LastCtcssMatchedPeriod )
                                {
                                    /* CorrelationCtcssCompareOk */ /* 不等于时，可能是随机噪音干扰 */
                                    CorrelationCtcssCompareOk();
                                }
                                else
                                {
                                    if( SameCorrelationCtcssCount >= ConstMaxSameCorrelationNum )/* = Last Matched Period > 15 , 信令变化或消失, exit */
                                    {
                                        /* CtcssCorrelationCompareExit */
                                        fCtdcssCmp = 0;
                                        return;
                                    }
                                    else
                                    {
                                        SameCorrelationCtcssCount ++;/* 等于时次数累计 */
                                        /* IsTheSameCtcssCorrelation */
                                        if( ( AudioClosingTimeIncPer10ms - CtcssCorrelationDelayValue ) & 0x80 ) /* Audio Closing Time 延时处理 */
                                        {
                                            AudioClosingTimeIncPer10ms = 0;
                                        }
                                        else
                                        {
                                            AudioClosingTimeIncPer10ms -= CtcssCorrelationDelayValue;
                                        }
                                        fCtdcssCmp = 0;
                                        return;
                                    }
                                }
                            }
                        }
                    }
				#endif			
                }
            }
            else
		#endif
            {
                /* CtcssNotMatched */ /* 信令未打开时处理 */
                fCtdcssCmp = 0;
		//printk("CtcssTotalTime=%d\r\n",CtcssTotalTime);
				if( decode_type == CTCSS_DCS_NO_ACTIVE )
				{/*载波模式下不符合尾音范围时不能执行到此*/
					AudioClosingTimeIncPer10ms = 0;
					if(CtcssTotalTime<1242&&CtcssTotalTime>1114)/*52--58HZ*/
					{
						TailNum++;
						ctcssOkNum=0;
						if(TailNum>2)
						{
							TailNum=0;
							sq_Tail=1;
							analog_off =11;//Rx_ControlVoice(COLSE);
						}
					}
					else
					{
						TailNum=0;
						ctcssOkNum++;
						if(sq_Tail)
						{
							if(ctcssOkNum>30)
							{
								ctcssOkNum=0;
								sq_Tail=0;
							}
						}
					}
					return;
				}
				else
				{
					if( CtcssTotalTime < CtcssEnterMinCount )
					{
						ctcssOkNum=0;
						/* CtcssNotMatchedExit */
						return;
					}
					if( CtcssTotalTime > CtcssEnterMaxCount )
					{
						ctcssOkNum=0;
						/* CtcssNotMatchedExit */
						return;
					}
					if( ((CtcssSubgroupStateWindow & 0x49) != 0x49) && ((CtcssSubgroupStateWindow & 0x92) != 0x92) )
					{
						ctcssOkNum=0;
						/* CtcssNotMatchedExit */
						return;
					}
					else
					{
			
						/* SetCtcssMatchedFlag */
						fCtdcssValid = 1; /* Ctcss Valid flag = 1 , 信令匹配 */
						//BEEPIO = 1;
						analog_on = 3;//Rx_ControlVoice(OPEN);
						//printk("a_on %d.",analog_on);
						SameCorrelationCtcssCount = 0;
						LastCtcssMatchedPeriod = 0;
						AudioClosingTimeIncPer10ms = 0;
						//GpioValueCount=0;
				
						return;
					}
				}
            }
        }
    }
    else
    {
    #ifdef DCS_EN	
        DCS_TITIME ++;
        if( TiCdcssTailTimer < 0xff )
        {
            TiCdcssTailTimer ++;
        }
        TiSampleStorePtr ++;
        if( TiSampleStorePtr >= 16 )	/* Filter Length = 16 , Mod(16) */
        {
            TiSampleStorePtr = 0;
        }
        //BEEPIO=~BEEPIO;
        AD = 1;//ADC_DRV_GetChnValueDQT(ADC_QT);


		//GloVarFlag.SysPrintfEnableFlag=1;
		//printk("DQT_ADC:%d\r\n",AD);
		//GloVarFlag.SysPrintfEnableFlag=0;
		
        /* cykang 2010-08-05 */
        /* ctcss的尾音和DCS的尾音都会被1846给解成0电平 */
#if 0//1
        if( fCtdcssValid )
        {
            if( (AD && fLastGpioValue) || ( (AD == 0) && (fLastGpioValue == 0) ) )
            {
                GpioValueCount ++;
                if( GpioValueCount >= 590 ) /* 134.4采样点位59个 10个134.4 */
                {
                    fCtdcssValid = 0;
                    //BEEPIO = 0;
                    fCtcssReverseBurst = 1;	/* CDCSS 尾音标志 = 1//CTCSS突然翻转 */
                    CdcssQuickChangCnt = 0;
                    ProcessCtcssDcsTail();
                    osal_start_timerEx( BUSY_ADC_EVENT, 300 );
                }
            }
            else
            {
                if( AD )
                {
                    fLastGpioValue = 1;
                }
                else
                {
                    fLastGpioValue = 0;
                }
                GpioValueCount = 0;
            }
        }
#endif
        Ti1stFilterSamplesSum -= CtcssFilterSampleArray[TiSampleStorePtr];
        Ti1stFilterSamplesSum += AD;
        CtcssFilterSampleArray[TiSampleStorePtr] = AD;	/* Add to array */
        AA = (uint8)(Ti1stFilterSamplesSum / 16);


//		uint8 *pbuf = (uint8 *)FlashBuf;
//		if(cdindex<11000) pbuf[cdindex++]=AA;
//
//		if(cdindex==11000)
//		{
//			uint16 i;
//			for(i=0;i<cdindex;i++)printk("%d\r\n",pbuf[i]);
//			printk("\r\n",pbuf[i]);
//			cdindex =12000;
//		}
		if (fOldTiLevel == 0)
		{
			/* Old Ti Level Is Low, find rising edge */
			//RED_LED_ON;
			if (AA < CtcssFilterSampleArray[TiSampleStorePtr])/* 如果最近的平均值 < 上一次的平均值 那么 */
			{
				/* TiLevelNoChange */
				CtcssFilterSampleArray[TiSampleStorePtr] = AA;
				TiLevelNoChange();
			}
			else
			{
				if ((AA - CtcssFilterSampleArray[TiSampleStorePtr]) < TI_DQT_AMP)
				{
					/* TiLevelNoChange */
					CtcssFilterSampleArray[TiSampleStorePtr] = AA;
					TiLevelNoChange();
				}
				else
				{
					/* 也有可能是5 反正意思是Const Cdcss Min Peak To Peak Value //AA -AA_TEMP >= 6 */
					/* AA -AA_TEMP >= 6相当于（最近采样的那个点的采样值-数组里被替换掉的那个点的采样值）/16 >=6 */
					/* 经过这一条指令后，符合条件的就是上升沿，以及第一次进入采样且刚好处在下降沿采样值高的点两种情况 */
					CtcssFilterSampleArray[TiSampleStorePtr] = AA;
					if (AA >= TIBIAVG) /* 通过最近16个点的采样和是否达到这个值，就可以排除第一次进入采样且刚好处在下降沿采样值高的点这种情况 */
					{
						/* CdcssRisingEdgeArrives */
						fOldTiLevel = 1; /* rising edge flag = 1	 */
						/* CDCSS尾音判断 */
					#if 1///0
                        if( (TiCdcssTailTimer <= (ConstCdcss1bitIntCycles * 2 / 3))
							&& (TiCdcssTailTimer >= (ConstCdcss1bitIntCycles / 3)))
						{
							if (fCtdcssValid)
							{
								CdcssQuickChangCnt++; /* Tail Counter ++ */
								if (CdcssQuickChangCnt >= ConstCdcssTailerNum) /* Tail Counter >= 10, is tail signal */
								{
                                    fCtdcssValid = 0;
									//BEEPIO = 0;
									fCtcssReverseBurst = 1; /* CDCSS 尾音标志 = 1//CTCSS突然翻转 */
									//P_AF_PWR = 0;						/* Audio power off */
									//analog_off = 12;// Rx_ControlVoice(COLSE);
									CdcssQuickChangCnt = 0;
									ProcessCtcssDcsTail();
									//osal_start_timerEx( BUSY_ADC_EVENT, 300 );
								}
								else
								{
									TiLevelNoChange();
								}
							}
							else
							{
								TiLevelNoChange();
							}
						}
					#endif
						else
						{
							CdcssQuickChangCnt = 0; /* Tail Counter Clear */
							TiLevelNoChange();
						}
					}
					else
					{
						TiLevelNoChange();
					}
				}
			}
		}
		else
		{
			/* OldTiLevelHi //Old Ti Level Is High, find falling edge */
			//RED_LED_OFF;
			if (AA > (0xFF - TI_DQT_AMP)) /* Ti(16 before) - Ti(now) >= 5 ?   6 -- Const Cdcss Min Peak To Peak Value */
			{
				/* TiLevelNoChange */
				CtcssFilterSampleArray[TiSampleStorePtr] = AA;
				TiLevelNoChange();
			}
			else
			{
				if ((AA + TI_DQT_AMP) >= CtcssFilterSampleArray[TiSampleStorePtr])
				{
					/* TiLevelNoChange */
					CtcssFilterSampleArray[TiSampleStorePtr] = AA;
					TiLevelNoChange();
				}
				else/* C = 0, Falling Edge Arrives //AA_TEMP - AA > 6 */
				{
					CtcssFilterSampleArray[TiSampleStorePtr] = AA;
					if (AA >= TIBIAVG) /* If Average Ti < TIBIAVG, Is Turely falling Edge */
					{
						/* TiLevelNoChange */
						TiLevelNoChange();
					}
					else
					{
						/* CdcssFallingEdgeArrives */
						TiCdcssTailTimer = 0;
						fOldTiLevel = 0; /* falling edge flag = 1 */
						DCS_TITIME = 0;
						/* TiLevelNoChange */
						TiLevelNoChange();
					}
				}
			}
		}
#endif
	}
}

void TackleCtcssCdcssValidFlagFunc(void)
{
	AudioClosingTimeIncPer10ms++;	//8MS加一次，做为亚音延时

	#if 1
	if( decode_type == CTCSS_DCS_NO_ACTIVE )
	{
		if(sq_Tail)
		{
			if (AudioClosingTimeIncPer10ms >= 10)//
			{/*55HZ尾音时不会累计到这个值*/
				/*载波模式下不符合尾音范围时能执行到此*/
				AudioClosingTimeIncPer10ms = 0;
				sq_Tail=0;
			}
		}
	}
	#endif
	
	if (fCtcssReverseBurst == 1)
	{
		if (AudioClosingTimeIncPer10ms >= 20) //ConstCtcssSquelchTime	; CTCSS/CDCSS Valid Delay Time Used Up ?
		{
			AudioClosingTimeIncPer10ms = 0;
			fCtcssReverseBurst = 0;
			CtcssTimerOutOfTimeSub();
		}
	}
	else
	{
		if (fCtdcssValid == 1)
		{
			if (decode_type <= CTCSS_ACTIVE)
			{
				if (AudioClosingTimeIncPer10ms >= 200) // 200) //ConstCtcssAudioClosingTime
				{
					CtcssTimerOutOfTimeSub();
					AudioClosingTimeIncPer10ms = 0;
					fCtcssReverseBurst = 0;
					//P_AF_PWR = 0;						/* Audio power off */
					analog_off = 13; //Rx_ControlVoice(COLSE);
					//BEEPIO = 0;
					CdcssQuickChangCnt = 0;
				}
			}
			else
			{
				//BEEPIO=1;
				if (AudioClosingTimeIncPer10ms >50/* 150*/) //ConstCdcssAudioClosingTime
				{
					AudioClosingTimeIncPer10ms = 0;
					CdcssQuickChangCnt = 0;
					DQTRSV3 = 0;
					DQTRSV2 = 0;
					DQTRSV1 = 0;
					fCtdcssValid = 0;
					fCtcssReverseBurst = 1; /* CDCSS 尾音标志 = 1//CTCSS突然翻转 */
					//P_AF_PWR = 0;						/* Audio power off */
					analog_off = 14; //Rx_ControlVoice(COLSE);
					//BEEPIO=0;
					CdcssQuickChangCnt = 0;
					//ProcessCtcssDcsTail();
					///osal_start_timerEx( BUSY_ADC_EVENT, 300 );
				}
			}
		}
		else
		{
			;                    //BEEPIO = 0;
		}
	}
}

void ProcessCtcssDcsTail(void)
{
    //CTCSS_DCS_STOP();
    ///BUSY_RX_ING_FLAG =0;
    ///BUSY_RX_DELAY_FLAG =0;
    ///BUSY_RX_CTCDCS_ACTIVE_FLAG =0;
    ///DeviceListen(DISABLE);
}
static void CheckCtcssTailLoop(void)
{
	if(CtcssTotalTime<1242&&CtcssTotalTime>1114)/*52--58HZ*/
	{
		TailNum++;
		if(TailNum>5)
		{
			TailNum=0;
		        fCtdcssValid = 0;
			fCtcssReverseBurst = 1; /* CTCSS 尾音标志 = 1 */
			analog_off =15;//Rx_ControlVoice(COLSE);
			AudioClosingTimeIncPer10ms = 0;
			CtcssBurstHalfMoreCount = 0;
			CtcssBurstHalfLessCount = 0;
			fCtdcssCmp = 0;
		}
	}
	else
	{
		TailNum=0;
	}
}
#endif

//void pppp()
//{
//	DH_U8 i;
//	printk("deco: ");
//	for(i=0;i<ConstCtcssGroupNum;i++)
//	{
//		printk("%d ",CtcssPeakTimerArray[i]);
//	}
//	printk("\r\n");
//}

