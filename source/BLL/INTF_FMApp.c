/*****************************************************************************
 *   文件名:Rda1847App.c
 *  文件描述:
 *   创建人：陶林，2010/08/12
 ******************************************************************************/

#include <math.h>
#include "app_config.h"

#include "include.h"


#if 1
#define TASKRda1846Irq_STK_SIZE TASK_FMREC_STK_SIZE_

#define Checkdelay 10
#define CheckCnt 5

#define CDCSS_DECODE_SOFTWARE 0 /* CDCSS软件解码 */
#define CDCSS_DECODE_HARDWARE 1 /* CDCSS硬件解码 */
#define CDCSS_DECODE_TYPE CDCSS_DECODE_SOFTWARE

/*CTCSS亚音频62.5KHz~254.1KHz*/
//fp32 const CtcssDec_table[51]={
//62.5,67.0,69.3,71.9,74.4,77.0,79.7,82.5,85.4,88.5,91.5,94.8,97.4,100.0,
//103.5,107.2,110.9,114.8,118.8, 123.0,127.3,131.8,136.5,141.3,146.2,151.4,
//156.7,159.8,162.2,165.5,167.9,171.3,173.8,177.3,179.9,183.5,186.2,189.9,
//192.8,196.6,199.5,203.5,206.5,210.7,218.1,225.7,229.1,233.6,241.8,250.3,254.1,
//};
#if !NEW_DECODE
const DH_U16 CtcssValueTable[51][2]=
{/*解码频率的倒数关系*/
	{/*62.5*/1103,1044},
	{/*67*/1022,980},
	{/*69.3*/973,950},
	{/*71.9*/942,914},
	{/*74.4*/909,883},
	{/*77*/879,853},
	{/*79.7*/848,823},
	{/*82.5*/820,797},
	{/*85.4*/792,768},
	{/*88.5*/763,743},
	{/*91.5*/740,717},
	{/*94.8*/716,693},
	{/*97.4*/692,676},
	{/*100*/674,656},
	{/*103.5*/654,634},
	{/*107.2*/631,612},
	{/*110.9*/610,592},
	{/*114.8*/590,572},
	{/*118.8*/1066,1035},
	{/*123*/1030,1001},
	{/*127.3*/995,968},
	{/*131.8*/961,934},
	{/*136.5*/928,902},
	{/*141.3*/896,873},
	{/*146.2*/867,843},
	{/*151.4*/838,814},
	{/*156.7*/810,793},
	{/*159.8*/786,778},
	{/*162.2*/773,767},
	{/*165.5*/760,753},
	{/*167.9*/746,740},
	{/*171.3*/734,728},
	{/*173.8*/722,715},
	{/*177.3*/708,702},
	{/*179.9*/697,691},
	{/*183.5*/684,678},
	{/*186.2*/674,667},
	{/*189.9*/663,655},
	{/*192.8*/652,645},
	{/*196.6*/639,632},
	{/*199.5*/629,622},
	{/*203.5*/618,612},
	{/*206.5*/609,601},
	{/*210.7*/599,587},
	{/*218.1*/580,565},
	{/*225.7*/561,551},
	{/*229.1*/549,541},
	{/*233.6*/537,528},
	{/*241.8*/525,511},
	{/*250.3*/507,496},
	{/*254.1*/495,489},
};
#endif

volatile DH_U8 SubDcsCode = 0;
DH_U8 AttrTail; /*是载波和亚音频的区别，1表示是载波2为模拟亚音频*/
DH_U8 CtcCnt = 0;        /*是否在模拟亚音频模式下解码正确，*/
DH_U8 CtcTail = 0;       /*0为没有收到尾音，1为收到尾音*/

DH_U8 CtcMatchFlg = 0;
DH_U8 TailMatchFlg = 0;
DH_U16 CtcMatchCnt = 0;
DH_U8 Scan_RecFlag;

DH_U8 CompareFlg = 0x7e;
DH_U8 sq_Tail = 0;       /*尾音消除标志位0表示没有收到尾音，1为收到尾音信号*/
DH_U8 sq_PhaseTail = 0;  /*尾音消除相位标志位0表示没有收到尾音，1为收到尾音信号*/
/*DH_U8 g_sq_cnt;*/
DH_U8 SubCodeFlg;
DH_U8 g_msenden;         /*模拟接收使能*/
DH_U8 g_SqChkFlg = 0;         /*50ms模拟载波检测标志*/
/*DH_U8 g_msendlimit;	模拟发送限时*/
//DH_U8 TimeDelay = 0;     /*接收到尾音后延时多久才再次打开喇叭的延时时间*100ms*/
DH_U8 g_DecodeRight = 0; /*解码正确的次数*/
DH_U8 analog_on = 0, analog_off = 0;
/*DH_U8 zsytst=1;*/
DH_U8 OpenSqFlg = 0;
DH_U8 CloseSqFlg = 0;
DH_U8 CheckctdcsFlg = 0;
DH_U8 SubjsCode = 0;
DH_U8 DcsCodemd = 0;

DH_U16 g_CTCBufMaxNum;
DH_U32 CtcMatchCnt1 = 0;

CTC_STR Ctc_Str;
SQL_STATUS_S gstSql;


#if FM_DECODE_SEL ==  FM_DECODE_SOFTWARE
static DH_U16 freqCtcBuf[26]={0};
static DH_U8  CurItem = 0;
static DH_S32 SumFreq = 0;
#endif
DH_U16 g_CurSignalVal;
extern DH_U8 g_RecChn;

#if FUNC_FM_CTCSS_38_51 //38 stanadard tones and then extended tones
/*CTCSS亚音频62.5KHz~254.1KHz*/
const DH_U16 CtcssTones[] =
{
	0x1a2c/*67*/,     0x1c16/*71.9*/,   0x1d10/*74.4*/,
	0x1e14/*77*/,     0x1f22/*79.7*/,   0x203a/*82.5*/,
	0x215c/*85.4*/,	  0x2292/*88.5*/,	0x23be/*91.5*/,
	0x2508/*94.8*/,   0x260c/*97.4*/,   0x2710/*100*/,
	0x286e/*103.5*/,  0x29e0/*107.2*/,  0x2b52/*110.9*/,
	0x2cd8/*114.8*/,  0x2e68/*118.8*/,  0x300c/*123.0*/,
	0x31ba/*127.3*/,  0x337c/*131.8*/,  0x3552/*136.5*/,
	0x3732/*141.3*/,  0x391c/*146.2*/,	0x3b24/*151.4*/,
	0x3d36/*156.7*/,  0x3f5c/*162.2*/, 	0x4196/*167.9*/,
	0x43e4/*173.8*/,  0x4646/*179.9*/,	0x48bc/*186.2*/,
	0x4b50/*192.8*/,  0x4f7e/*203.5*/,  0x524e/*210.7*/,
	0x5532/*218.1*/,  0x582a/*225.7*/,  0x5b40/*233.6*/,
	0x5e74/*241.8*/,  0x61c6/*250.3*/,
	0x1b12/*69.3*/,
	0x186a/*62.5*/,   0x3e6c/*159.8*/,
	0x40a6/*165.5*/,  0x42ea/*171.3*/,  0x454b/*177.3*/,
	0x47ae/*183.5*/,  0x4a2e/*189.9*/,  0x4ccc/*196.6*/,
	0x4dee/*199.5*/,  0x50aa/*206.5*/,  0x597e/*229.1*/,
	0x6342/*254.1*/,
};

const DH_U16 CtcssTonesRangeL[] =
{
	6500,  7070,  7330,
	7580,  7850,  8120,  8410,  8710,
	9010,  9330,  9620,  9880,  10190,
	10550, 10920, 11300, 11700, 12110,
	12530, 12970, 13430, 13910, 14390,
	14900, 15420, 16110,
	16680, 17270, 17880,
	18500, 19150,
	20170, 20880, 21510,
	22270, 23160, 23830, 24680,
	6830,	6050, 15840, 16400,16980,17570,18190, 18820,19490,19820, 20520,22760,25240,
};
const DH_U16 CtcssTonesRangeH[] =
{
	6800,  7300,  7560,
	7820,  8100,  8380,  8680,  8990,
	9300,  9600,  9860,  10160, 10520,
	10890, 11270, 11660, 12080, 12500,
	12940, 13400, 13870, 14360, 14860,
	15390, 15810, 16370,
	16940, 17540, 18150,
	18790, 19450,
	20480, 21370, 22110,
	22720, 23710, 24530, 25200,
	7050,  6400,  16090, 16660, 17240,17840,18470,19120,19790,20130,  20840,23110,25610,
};
#else
/*CTCSS亚音频62.5KHz~254.1KHz*/
const DH_U16 CtcssTones[] =
{
	0x186a, 0x1a2c, 0x1b12, 0x1c16, 0x1d10,
	0x1e14, 0x1f22, 0x203a, 0x215c,	0x2292,
	0x23be, 0x2508, 0x260c, 0x2710, 0x286e,
	0x29e0, 0x2b52, 0x2cd8,	0x2e68, 0x300c,
	0x31ba, 0x337c, 0x3552, 0x3732, 0x391c,
	0x3b24, 0x3d36,	0x3e6c, 0x3f5c, 0x40a6,
	0x4196, 0x42ea, 0x43e4, 0x4542, 0x4646,
	0x47ae,	0x48bc, 0x4a2e, 0x4b50, 0x4ccc,
	0x4dee, 0x4f7e, 0x50aa, 0x524e, 0x5532,
	0x582a, 0x597e, 0x5b40, 0x5e74, 0x61c6,
	0x6342
};

const DH_U16 CtcssTonesRangeL[] =
{
	6050,  6500,  6830,  7070,  7330,
	7580,  7850,  8120,  8410,  8710,
	9010,  9330,  9620,  9880,  10190,
	10550, 10920, 11300, 11700, 12110,
	12530, 12970, 13430, 13910, 14390,
	14900, 15420, 15840, 16110, 16400,
	16680, 16980, 17270, 17570, 17880,
	18190, 18500, 18820, 19150,	19490,
	19820, 20170, 20520, 20880, 21510,
	22270, 22760, 23160, 23830, 24680,
	25240
};
const DH_U16 CtcssTonesRangeH[] =
{
	6400,  6800,  7050,  7300,  7560,
	7820,  8100,  8380,  8680,  8990,
	9300,  9600,  9860,  10160, 10520,
	10890, 11270, 11660, 12080, 12500,
	12940, 13400, 13870, 14360, 14860,
	15390, 15810, 16090, 16370, 16660,
	16940, 17240, 17540, 17840, 18150,
	18470, 18790, 19120, 19450,	19790,
	20130, 20480, 20840, 21370, 22110,
	22720, 23110, 23710, 24530, 25200,
	25610
};
#endif
extern DH_U8 g_FMIC_Sel;
DH_U8 g_SqStatus = 0;
DH_U32 timer_dec;
DH_VOID FM_APP_CtcDcsTimerChk(DH_VOID);

void FM_APP_SQGetFrmE2(void);
extern void ctcssPhaseDecode(DH_U16 perid );
void ctcssPhaseProc(DH_VOID );
DH_VOID FM_APP_GetSqArray(DH_VOID);
DH_VOID HRC_BLL_SetSqLevel(DH_VOID);

void FM_INTF_VolSet(DH_VOID);

//void start_ctcdcs_decode(uint8 flg, uint16 xx){}

DH_U8 FM_APP_GetSqStatus(void)
{
	if(Func_Str.sqlevel==0 \
	|| (GloVarFlag.FmMontFlg == 1)\
	|| (GloVarFlag.FmMontCFlg == 1))
	{
		g_SqStatus = 1;
	}
	else
	{
#if FM_RF_SEL==FM_RF_AF
		g_SqStatus = RFIC_INTF_GetSqStatus();
#elif FM_RF_SEL==FM_RF_IF
	#if SQ_DET_SEL==SQ_DET_31136

		DH_U16 sql = ADC_INTF_GetValue(E_ADC_CHN_RSSI);

		if(sql <= gstSql.u16StartNoiseLevel)
		{
			g_SqStatus = 1;
		}
		else
		{
			if(GloVarFlag.FmRecStat)
			{
				if(sql <= gstSql.u16StopNoiseLevel)
				{
					g_SqStatus = 1;
				}
				else
				{
					g_SqStatus = 0;
				}
			}
			else
			{
				g_SqStatus = 0;
			}
		} 
	#else

	#endif
#endif
	}
	return g_SqStatus;
}

extern DH_U8 g_FmTxEndCnt;



/*****************************************************************************
 *   函数名: FM_APP_ICInit
 *   功能描述:  1846 or 4818芯片初始化
 *   参数定义:
 *	返回值:
 *   创建人：陶林，2010/11/15
 *****************************************************/
DH_VOID FM_APP_ICInit(DH_VOID)
{
	IC15E03_INTF_GPIOInit();

//    RFIC_INTF_INITIAL();


#if REC_CHN_SEL==FM_RF_IF_AF
    RF_INTF_LO_INITIAL();
#endif
}
/*****************************************************************************
 *   函数名: TaskRda1847Irq
 *   功能描述:  音频任务函数
 *   参数定义:
 *	返回值:
 *   创建人：陶林，2010/11/15
 ******************************************************************************/
DH_VOID FM_APP_RecTask(DH_VOID* pdata)
{
	pdata = pdata;
	if(TaskHndGet( TASK_FLAG_FMREC))
	{          
		TaskHndStop(TASK_FLAG_FMREC);
	//	dhtime_delay(20);

		if( g_FpgaTx|| MenuVarFlg.VoiceFlg||GloVarFlag.PcWriteReadFlg)
		{
			return;
		}

		CHN_INTF_LOCK;
		if( (CHNSTAT_FM==CHN_BLL_GetAttr())
			&& (GloVarFlag.KeyFlag==0)
//			&& (0 == g_FmTxEndCnt)
		)
		{
			if (FM_APP_GetSqStatus())
			{
				SlpModeExitSM();
			}

			if (!sq_Tail && !sq_PhaseTail)
			{
				if (g_SqStatus) /* 高电平 :接收中... */
				{
					CloseSqFlg = 0;
					
					OpenSqFlg++;	
					FM_APP_StartReceiveCtdcss();
				}
				else
				{
					OpenSqFlg = 0;
					CloseSqFlg ++;
					FM_APP_EndReceiveCtdcss();
				}
			}

			//ctcssPhaseProc();

			if((Func_Str.sqlevel==0 && (CHN_BLL_GetFMRxSubVoiceType() == FM_SUBVOICETYPE_CARRIER && !g_FpgaTx))
			 || GloVarFlag.FmMontFlg || GloVarFlag.FmMontCFlg)
			{
				analog_on = 1;
				analog_off = 0;
			}
			else
			{
			#if(FM_DECODE_SEL==NEW_DECODE_TYEP_ADC)
				//FM_APP_CtcDcsTimerChk();
				/*else if(CHN_BLL_GetFMRxSubVoiceType()==FM_SUBVOICETYPE_CARRIER)
				{
					if(GetCtcDcsDecodeReverseStatus())
					{
						analog_off = 111;
						analog_on = 0;
					}
					else if(g_msenden)
					{
						analog_on = 111;
						analog_off = 0;
					}
					else
					{
						analog_off = 112;
						analog_on = 0;
					}
				}*/
				if(CHN_BLL_GetFMRxSubVoiceType()>=FM_SUBVOICETYPE_CDCSS)
				{
					if(g_msenden || CheckctdcsFlg)//zaibo
					{
						if(0)//GetCtcDcsDecodeStatus()
						{
							analog_on = 111;
							analog_off = 0;
						}
						else
						{
							analog_off = 111;
							analog_on = 0;
						}
					}
					else
					{
						analog_off = 112;
						analog_on = 0;
					}
				}
				else
				{

				}
			#endif
			}


			if(analog_on && !MenuVarFlg.VoiceFlg)
			{
				FM_APP_FmVoiceStart();
				analog_on = 0;
			}
			if (analog_off)
			{
				FM_APP_FmVoiceStop();

			//	 TmrHndStop(TMRHND_3S_CTCSS);
			}
		}
		CHN_INTF_UNLOCK;
	}
}



DH_U8   BitCountExt(DH_U8 n)
{
	DH_U8 count;
	for (count = 0; n; n >>= 1)
	{
		count += n & 1;
	}
	return count;
}
//
DH_VOID sortI(DH_U16 *cal_ad, unsigned int n)
{
	//从小到大排序
	unsigned int i, j, k, t;
	for (i = 0; i < n - 1; i++)
	{
		k = i;
		for (j = i + 1; j < n; j++)
		{
			if (cal_ad[j] < cal_ad[k])
				k = j;
			t = cal_ad[k];
			cal_ad[k] = cal_ad[i];
			cal_ad[i] = t;
		}
	}
}

//
DH_VOID FM_APP_StartReceiveCtdcss(DH_VOID)
{
	if (CHN_BLL_GetFMRxSubVoiceType() == FM_SUBVOICETYPE_CARRIER)/*接收方式为信令*/
	{
		if ((GloVarFlag.FmRecStat == 0))
 		{
			if (OpenSqFlg >= CheckCnt)/*检测到持续的50ms高电平时间则开启喇叭*/
			{
				{
					OpenSqFlg=0;/*清0*/
					g_msenden=1;/*检测到载波，需要亮灯*/
					g_SqChkFlg = 1;
					analog_on = 1;
		//			SCAN_app_LastActiveChnGet();

				#if FM_RF_SEL==FM_RF_AF

					GPIO_DRV_IntEnable(RDA_CDCSS_PIN);/*收到载波才开始解尾音*/
					TIMER_DRV_Enable(TIM4);

				#else
					#if(FM_DECODE_SEL==NEW_DECODE_TYEP_ADC)
					start_ctcdcs_decode(CTCSS_DCS_NO_ACTIVE, 550);
					#endif
				#endif
				}
			}
		}
		else
		{
			OpenSqFlg = 0;/*准备*/
		}
	}
	else if (CHN_BLL_GetFMRxSubVoiceType() == FM_SUBVOICETYPE_CTCSS)/*接收方式为信令*/
	{
		if ((CheckctdcsFlg == 0) && (GloVarFlag.FmMontFlg == 0))/*若是处于监听的状态则不需要去打开检测信令功能*/
		{
			if (OpenSqFlg >= CheckCnt)/*检测到持续的50ms高电平时间则开启喇叭*/
			{
				{
					g_msenden = 1;/*检测到载波，需要亮灯*/g_SqChkFlg = 1;
					OpenSqFlg = 0;/*清0*/
					CheckctdcsFlg = 1;/*检测到了高电平需要开始解码*/
			//		SCAN_APP_ScanGapAdj();
//					SCAN_app_LastActiveChnGet();
				#if FM_RF_SEL==FM_RF_AF
					GPIO_DRV_IntEnable(RDA_CDCSS_PIN);/*收到载波才开始解尾音*/
					TIMER_DRV_Enable(TIM4);
				#else
					#if(FM_DECODE_SEL==NEW_DECODE_TYEP_ADC)
					start_ctcdcs_decode(CTCSS_ACTIVE, CtcssTones[CHN_BLL_GetRxSubvoiceIndex()]/10);
					#endif
				#endif
				}
			}
		}
		else
		{
			OpenSqFlg = 0;/*准备*/
		}
	}
#if CDCSS_DECODE_TYPE == CDCSS_DECODE_SOFTWARE
	else/*接收方式数字亚音频*/
	{
		if ((CheckctdcsFlg == 0) && (GloVarFlag.FmMontFlg == 0))/*&&zsytst 若是处于监听的状态则不需要去打开检测信令功能*/
		{
			if (OpenSqFlg >= CheckCnt)/*检测到持续的50ms高电平时间则开启喇叭*/
			{
				{
					g_msenden = 1;/*检测到载波，需要亮灯*/g_SqChkFlg = 1;
					OpenSqFlg = 0;/*清0*/
					CheckctdcsFlg = 1;/*检测到了高电平需要开始解码*/
//					SCAN_APP_ScanGapAdj();
//					SCAN_app_LastActiveChnGet();
				#if FM_RF_SEL==FM_RF_AF
					TIMER_DRV_Enable(TIM5);
				#else
					#if(FM_DECODE_SEL==NEW_DECODE_TYEP_ADC)
					if (CHN_BLL_GetFMRxSubVoiceType() == FM_SUBVOICETYPE_CDCSS)
					{
						start_ctcdcs_decode(DCS_ACTIVE,CHN_BLL_GetTxSubvoice() );//DcsCode_table_NormalADC[CHN_BLL_GetRxSubvoiceIndex()]0X2813);//023N
					}
					else
					{
						start_ctcdcs_decode(DCS_ACTIVE_N,CHN_BLL_GetTxSubvoice() );//DcsCode_table_NormalADC[CHN_BLL_GetRxSubvoiceIndex()]0X2813);//023N
					}
					#endif
				#endif
				}
			}
		}
		else
		{
			OpenSqFlg = 0;/*准备*/
		}
	}
#endif
}

DH_VOID FM_APP_EndReceiveCtdcss(DH_VOID)
{
	uint8 SQkeeptime=0;

//	if(g_CurSignalVal<25)/*-115db*/
//	{
//		SQkeeptime=10;	
//	}
//	else if(g_CurSignalVal<30)/*-110db*/
//	{
//		SQkeeptime=7;	
//	}
//	else
//	{
//		SQkeeptime=5;
//	}


	
	if (CHN_BLL_GetFMRxSubVoiceType() == FM_SUBVOICETYPE_CARRIER)/*接收方式为载波*/
	{
		if (GloVarFlag.FmRecStat )/*打开了喇叭后关闭,或数模同检判断退出模拟状态*/
		{
			if (CloseSqFlg >= SQkeeptime)/*检测到持续的低电平则要去关闭喇叭了*/
			{
				g_msenden = 0;/*载波消失需要灭灯*/g_SqChkFlg = 0;
				CloseSqFlg = 0;
				analog_off = 1;
				sq_Tail = 0;
				#if FM_RF_SEL==FM_RF_AF
					GPIO_DRV_IntDisable(RDA_CDCSS_PIN);
				#else
					#if(FM_DECODE_SEL==NEW_DECODE_TYEP_ADC)
					//TIMER_DRV_Disable(TIM4);
					FM_APP_StopCtcDcsTimer();
					#endif
				#endif
			}
			else
			{
				CloseSqFlg = 0;/*认为是误检测*/
			}
		}
		else
		{
			CloseSqFlg = 0;/*准备*/
		}
	}
	else if (CHN_BLL_GetFMRxSubVoiceType() == FM_SUBVOICETYPE_CTCSS)/*接收方式为信令*/
	{
		if (CheckctdcsFlg)/*已经有开始解码信令*/
		{
			
			if (CloseSqFlg >= SQkeeptime)/*检测到持续的低电平则要去关闭喇叭了*/
			{
				g_msenden = 0;/*载波消失需要灭灯*/g_SqChkFlg = 0;
				CloseSqFlg = 0;
				analog_off = 2;
				CheckctdcsFlg = 0;
				CtcMatchCnt1 = 0;/*满足条件范围内*/
				g_DecodeRight = 0;
				CtcMatchCnt = 0;
				CtcCnt = 0;
				sq_Tail = 0;
				sq_PhaseTail = 0;
				#if FM_RF_SEL==FM_RF_AF				
				GPIO_DRV_IntDisable(RDA_CDCSS_PIN);
				#else
					#if(FM_DECODE_SEL==NEW_DECODE_TYEP_ADC)
					//TIMER_DRV_Disable(TIM4);
					FM_APP_StopCtcDcsTimer();
					#endif
				#endif
			}
			else
			{
				CloseSqFlg = 0;/*认为是误检测*/
			}
		}
		else
		{
			CloseSqFlg = 0;/*准备*/
		}
	}
#if CDCSS_DECODE_TYPE == CDCSS_DECODE_SOFTWARE
	else/*接收方式为数字亚音频*/
	{
		if (CheckctdcsFlg)/*已经有开始解码信令*/
		{
			if (CloseSqFlg >= SQkeeptime)/*检测到持续的低电平则要去关闭喇叭了*/
			{
				g_msenden = 0;/*载波消失需要灭灯*/
				g_SqChkFlg = 0;
				CloseSqFlg = 0;
				analog_off = 3;
				CheckctdcsFlg = 0;
			#if FM_RF_SEL==FM_RF_IF
				#if(FM_DECODE_SEL==NEW_DECODE_TYEP_ADC)
//					TIMER_DRV_Disable(TIM4);
				#endif
			#endif
			}
			else
			{
				CloseSqFlg = 0;/*认为是误检测*/
			}
		}
		else
		{
			CloseSqFlg = 0;/*准备*/
		}
	}
#endif
}

//
DH_VOID FM_APP_FmVoiceStart(DH_VOID)
{
//	printk("-%d-%d-%d.",GloVarFlag.KeyFlag,GloVarFlag.VoxFlg,GloVarFlag.FmRecStat );
	if ((GloVarFlag.KeyFlag == 0 && GloVarFlag.VoxFlg == 0))
	{
		if (GloVarFlag.FmRecStat == 0)
		{
		//	printk("a_on %d.",analog_on);
			SlpModeExitSM();
		//	SCAN_app_LastActiveChnGet();
			
			MenuVarFlg.g_ChecksignalFlg = 1;

			FM_INTF_VolSet();  //模拟信道的音量设置

			HRC_BLL_OpenAudio();
		}
	}
}


//
DH_VOID FM_APP_FmVoiceStop(DH_VOID)
{
	if (CHN_BLL_GetAttr() == CHNSTAT_FM)  //在数字的状态下则需要去关闭喇叭
	{
		if (((CHN_BLL_GetFMRxSubVoiceType() == FM_SUBVOICETYPE_CARRIER)
			&& (Func_Str.sqlevel == 0)) || (GloVarFlag.FmMontFlg == 1)||(GloVarFlag.FmMontCFlg == 1))
		{
			return;
		}
	}
	FM_APP_ForceFmVoiceStop();
	Scan_RecFlag = 0;
	//printk("a_off %d %d.\r\n",analog_off,OS_TS_GET());
	analog_off = 0;
}

/*强制关闭模拟信号*/
void FM_APP_ForceFmVoiceStop(DH_VOID)  //FoceFmVoiceStop
{
	if (GloVarFlag.FmRecStat)
	{
		HRC_BLL_CloseAudio();
		g_msenden = 0;
		Scan_RecFlag = 0;
	}
}



DH_U16   getCtcDataDeal(DH_U16 *buf,DH_U8 len)
{
#if FM_DECODE_SEL ==  FM_DECODE_SOFTWARE
	DH_U16 lmax = 0,lmin = buf[0];
	DH_U8 i;
	SumFreq = 0;
	for(i=0;i<len;i++){
		SumFreq +=  buf[i];
		if(lmax<buf[i])lmax = buf[i];
		if(lmin>buf[i])lmin = buf[i];
	}
	return lmax-lmin;
#else
	return 0;
#endif
}
/*******************************************************************************
 *函数说明:Ctcss解码Cnt1和Cnt2分别为高电平和低电平的检测到的次数

 *函数说明:Ctcss解码

 *返回值:

 *创建人:yufeng

 *创建日期:2013/11/18
 *******************************************************************************/
DH_U8   FM_APP_CTCSSCodeMatching(DH_U16 Cnt1, DH_U16 Cnt2)
{
#if FM_DECODE_SEL ==  FM_DECODE_SOFTWARE
	static DH_U16 tailbuf[8] = {0};
	static DH_U8 taili = 0;
	DH_U16 CTCint = 100000000/(Cnt2 + Cnt1);

	tailbuf[taili++] = CTCint;
	if(taili>=8)taili = 0;

	freqCtcBuf[CurItem++] = CTCint;
	if(CurItem>=g_CTCBufMaxNum)CurItem = 0;


	if (5100 <= CTCint && CTCint <= 5900 && abs(Cnt1 - Cnt2)<5500)//单次超出上下限1Hz过滤
	{
		if(getCtcDataDeal(tailbuf,8) < 1000)
		{
			CTCint = SumFreq>>3;
			if (5300 <= CTCint && 5700 >= CTCint)//判断平均频率是否在范围内
			{
				CtcTail = 1;
				sq_PhaseTail = 0;
				return 1;
			}
		}
	}
	else if (25600 <= CTCint && CTCint <= 26200 && abs(Cnt1 - Cnt2)<25800)//单次超出上下限1Hz过滤
	{/*258HZ*/
		if(getCtcDataDeal(freqCtcBuf,g_CTCBufMaxNum) < 1000)
		{
			CTCint = SumFreq/(g_CTCBufMaxNum);
			if (25700 <= CTCint && 26100 >= CTCint)//判断平均频率是否在范围内
			{
				CtcTail = 1;
				return 1;
			}
		}
	}

	else
	{
		if(AttrTail==2)
		{
			if ((CtcssTonesRangeL[SubDcsCode]-200 <= CTCint)
			 && (CtcssTonesRangeH[SubDcsCode]+200 >= CTCint)
			 && (getCtcDataDeal(freqCtcBuf,g_CTCBufMaxNum) < (400+g_CTCBufMaxNum*100)))//单次超出上下限2Hz过滤
			{

				CTCint = SumFreq/(g_CTCBufMaxNum);//printk("%d\r\n",CTCint);
				if ((CtcssTonesRangeL[SubDcsCode] <= CTCint)
				 && (CtcssTonesRangeH[SubDcsCode] >= CTCint))//判断平均频率是否在范围内
				{
					return 2;
				}
			}
		}
	}
#endif
	return 0;
}
extern DH_U16 g_CurSignalVal;

DH_VOID   FM_APP_CTCSSOnOff(DH_U8 on,DH_U8 off)
{
	CtcMatchCnt = 0;
	CtcCnt=0;
	g_DecodeRight=0;
	analog_on=on;
	analog_off=off;
}

DH_VOID   FM_APP_CTCSS_Int(DH_VOID)
{
#if FM_DECODE_SEL ==  FM_DECODE_SOFTWARE
	DH_U32 perid = (*TIMER4_LOADCOUNT-*TIMER4_CURRENTVALUE);
	*TIMER4_CONTROLREG &= 0xFE;
	*TIMER4_CURRENTVALUE = *TIMER4_LOADCOUNT;
	*TIMER4_CONTROLREG |= 0x01;

	perid = perid/g_u16TimCnt_us;	//us

	if(!perid)
	{
		perid = 1000;
	}
	Ctc_Str.Step++;
	if(Ctc_Str.Step>6)
	{
		Ctc_Str.Step = 1;
	}

	/*软解亚音相位*/
	if(/*StrMenuSet.CtcTail && */CHN_BLL_GetFMRxSubVoiceType() == FM_SUBVOICETYPE_CTCSS /*!sq_PhaseTail&& GloVarFlag.FmRecStat*/ )
	{
		ctcssPhaseDecode(perid);
	}

	FM_APP_CTCSSDecode(Ctc_Str.Step,perid);
#endif
}

/*******************************************************************************
 *函数名:CtcssDecode

 *函数说明:Ctcss解码

 *返回值:

 *创建人:yufeng

 *创建日期:2013/11/18
 *******************************************************************************/
#if 1

DH_VOID   FM_APP_CTCSSDecode(DH_U8 Step,DH_U32 freq)
{
#if FM_DECODE_SEL ==  FM_DECODE_SOFTWARE
	DH_U8 err;
	static DH_U8 flg;

	switch (Step)
	{
	case 1:
	#if NEW_DECODE
		Ctc_Str.CtcCnt6 = freq;
	#else
		Ctc_Str.CtcCnt1++;
	#endif
      if(CompareFlg&0x02)
      {
        CompareFlg = (CompareFlg&0xfd)|0x40;
        err = FM_APP_CTCSSCodeMatching(Ctc_Str.CtcCnt5,Ctc_Str.CtcCnt6);
        TailMatchFlg = TailMatchFlg&0xbf;
		CtcMatchFlg  = CtcMatchFlg &0xbf;
        if(err==1){
        	TailMatchFlg |= (1<<6);
        }else if(err==2){
        	CtcMatchFlg  |= (1<<6);
        }
        Ctc_Str.CtcCnt5 = 0;
      }
      break;

	case 2:
	#if NEW_DECODE
		Ctc_Str.CtcCnt1 = freq;
	#else
		Ctc_Str.CtcCnt2++;
	#endif
      if(CompareFlg&0x04)
      {
        CompareFlg = (CompareFlg&0xfb)|0x02;
        err = FM_APP_CTCSSCodeMatching(Ctc_Str.CtcCnt6,Ctc_Str.CtcCnt1);
        
        TailMatchFlg = TailMatchFlg&0xfd;
		CtcMatchFlg  = CtcMatchFlg &0xfd;
		if(err==1){
			TailMatchFlg |= (1<<1);
		}else if(err==2){
			CtcMatchFlg  |= (1<<1);
		}
        Ctc_Str.CtcCnt6 = 0;
      }
      break;

	case 3:
	#if NEW_DECODE
		Ctc_Str.CtcCnt2 = freq;
	#else
		Ctc_Str.CtcCnt3++;
	#endif
      if(CompareFlg&0x08)
      {
        CompareFlg = (CompareFlg&0xf7)|0x04;
        err = FM_APP_CTCSSCodeMatching(Ctc_Str.CtcCnt1,Ctc_Str.CtcCnt2);
       
        TailMatchFlg = TailMatchFlg&0xfb;
		CtcMatchFlg  = CtcMatchFlg &0xfb;
		if(err==1){
			TailMatchFlg |= (1<<2);
		}else if(err==2){
			CtcMatchFlg  |= (1<<2);
		}
        Ctc_Str.CtcCnt1 = 0;
      }
      break;

	case 4:
	#if NEW_DECODE
		Ctc_Str.CtcCnt3 = freq;
	#else
		Ctc_Str.CtcCnt4++;
	#endif
      if(CompareFlg&0x10)
      {
        CompareFlg = (CompareFlg&0xef)|0x08;
        err = FM_APP_CTCSSCodeMatching(Ctc_Str.CtcCnt2,Ctc_Str.CtcCnt3);
        
        TailMatchFlg = TailMatchFlg&0xf7;
		CtcMatchFlg  = CtcMatchFlg &0xf7;
		if(err==1){
			TailMatchFlg |= (1<<3);
		}else if(err==2){
			CtcMatchFlg  |= (1<<3);
		}
        Ctc_Str.CtcCnt2 = 0;
      }
      break;

	case 5:
	#if NEW_DECODE
		Ctc_Str.CtcCnt4 = freq;
	#else
		Ctc_Str.CtcCnt5++;
	#endif
      if(CompareFlg&0x20)
      {
        CompareFlg = (CompareFlg&0xdf)|0x10;
        err = FM_APP_CTCSSCodeMatching(Ctc_Str.CtcCnt3,Ctc_Str.CtcCnt4);
        
        TailMatchFlg = TailMatchFlg&0xef;
		CtcMatchFlg  = CtcMatchFlg &0xef;
		if(err==1){
			TailMatchFlg |= (1<<4);
		}else if(err==2){
			CtcMatchFlg  |= (1<<4);
		}
        Ctc_Str.CtcCnt3 = 0;
      }
      break;

	case 6:
#if NEW_DECODE
		Ctc_Str.CtcCnt5 = freq;
#else
		Ctc_Str.CtcCnt6++;
#endif
      if(CompareFlg&0x40)
      {
        CompareFlg = (CompareFlg&0xbf)|0x20;
        err = FM_APP_CTCSSCodeMatching(Ctc_Str.CtcCnt4,Ctc_Str.CtcCnt5);
        TailMatchFlg = TailMatchFlg&0xdf;
		CtcMatchFlg  = CtcMatchFlg &0xdf;
		if(err==1){
			TailMatchFlg |= (1<<5);
		}else if(err==2){
			CtcMatchFlg  |= (1<<5);
		}
        Ctc_Str.CtcCnt4 = 0;
      }
      break;
    default:
      break;
  }

	if(sq_PhaseTail)
	{
		CtcMatchFlg = 0;
	}

#if NEW_DECODE
	if (freq >= *TIMER4_LOADCOUNT)
#else
	if((Ctc_Str.CtcCnt1>RDA_RX_CTCSS_OFF_DELAY)||(Ctc_Str.CtcCnt2>RDA_RX_CTCSS_OFF_DELAY)||(Ctc_Str.CtcCnt3>RDA_RX_CTCSS_OFF_DELAY)
			||(Ctc_Str.CtcCnt4>RDA_RX_CTCSS_OFF_DELAY)||(Ctc_Str.CtcCnt5>RDA_RX_CTCSS_OFF_DELAY)||(Ctc_Str.CtcCnt6>RDA_RX_CTCSS_OFF_DELAY))
#endif
	{
		if (AttrTail == 2)
		{
			CtcMatchCnt1++;
			if (CtcMatchCnt1 >= 5*20)/*50ms*100=5s*/
			{
				CtcMatchCnt1 = 0;/*为下次做准备*/
				analog_off = 4;	//INTF_FmVoiceStop();
				CtcMatchCnt = 0;
				CtcMatchFlg = 0;
				CtcCnt = 0;
				CtcTail = 0;
			}
	    #if !NEW_DECODE
			if(SubDcsCode<=17)
			{
				if(CtcMatchCnt1>82417)/*经计算是14.56us*82417=1.2S*/
				{
					CtcMatchCnt1=0;/*为下次做准备*/
					analog_off=1;//INTF_FmVoiceStop();
					CtcMatchCnt = 0;
					CtcMatchFlg = 0;
					CtcCnt=0;
					CtcTail=0;
				}
			}
			else
			{
				if(CtcMatchCnt1>164835)/*经计算是7.28us*164835=1.2S*/
				{
					CtcMatchCnt1=0;/*为下次做准备*/
					analog_off=1;//INTF_FmVoiceStop();
					CtcMatchCnt = 0;
					CtcMatchFlg = 0;
					CtcCnt=0;
					CtcTail=0;
				}
			}
        #endif
		}
		if ((AttrTail == 1) || (AttrTail == 2))
		{
			if (sq_Tail)/*没有解码到尾音*/
			{
				sq_Tail = 0;
			}
		}
	}
	else
	{
		if (Step == 2)
		{
			if (flg == 0)
			{
				flg = 1;
			#if 1
				if(!CtcTail&&BitCountExt(CtcMatchFlg)>SubCodeFlg)
				{
					CtcMatchCnt1=0;/*满足条件范围内*/
					g_DecodeRight=0;
					CtcMatchCnt = 0;
					analog_on = 3;//INTF_FmVoiceStart();
					CtcCnt=10;
					 TmrHndStart(TMRHND_3S_CTCSS);
				}
				if(CtcCnt||AttrTail==1)//ctc解到亚音或载波模式
				{
					if(CtcTail)
					{
						if(BitCountExt(TailMatchFlg)>1)
						{
							CtcMatchCnt = 0;
							sq_Tail=1;
							sq_PhaseTail = 0;
							g_DecodeRight=0;
							analog_off=5;//INTF_FmVoiceStop();
							 TmrHndStop(TMRHND_3S_CTCSS);
//							FM_APP_FmVoiceStop();
						}
					}
					else if(sq_Tail)/*优先判断是否为还有尾音*/
					{
						CtcMatchCnt++;
						if(CtcMatchCnt>30)
						{
							CtcMatchCnt = 0;
							CtcCnt=0;/*尾音消失了，才把本次解码到信令标志位清0*/
							sq_Tail=0;
						}
					}
					else if(AttrTail==2)//亚音模式
					{

						if(BitCountExt(CtcMatchFlg)>1)
						{
							 TmrHndStart(TMRHND_3S_CTCSS);
							CtcMatchCnt1=0;
							CtcMatchCnt = 0;
					    }
						else
						{
							if(! TmrHndGetState(TMRHND_3S_CTCSS))
							{
								 TmrHndStart(TMRHND_3S_CTCSS);
							}

						if (g_FMIC_Sel==FM_IC_W12T)
						{
							CtcMatchCnt++;
							if(g_CurSignalVal<155)
							{
								if(CtcMatchCnt>((150/*+SubDcsCode*/)*400/(1091-6.55*g_CurSignalVal)))
								 {
									  CtcMatchCnt = 0;
									  CtcCnt=0;
									  g_DecodeRight=0;
									  analog_off=6;//INTF_FmVoiceStop();
								 }
							}
							else
							{
								if(CtcMatchCnt>(750/*(150+SubDcsCode)*5*/))
								 {
									  CtcMatchCnt = 0;
									  CtcCnt=0;
									  g_DecodeRight=0;SQ_C
									  analog_off=6;//INTF_FmVoiceStop();
								 }
							}
						}
						else if (g_FMIC_Sel==FM_IC_RDA1846S)
						{
							CtcMatchCnt++;
							 if(CtcMatchCnt>((65+SubDcsCode)))
							 {
								  CtcMatchCnt = 0;
								  CtcCnt=0;
								  g_DecodeRight=0;
								  analog_off=6;//INTF_FmVoiceStop();
//								  printk("g_CurSignalVal %d =%d\r\n",g_CurSignalVal,OS_TS_GET()-lt);
							 }
						}
						}
					}
				}
			
			#endif
				CtcMatchFlg = 0;
			}
			CtcTail = 0;
		}

		if (Step == 3)
		{
			flg = 0;
		}
	}
#endif
}
  
    


#endif

#if 1
/*****************************************************************************
 *   函数名: INTF_FmMontStart
 *   功能描述:
 *   参数定义:
 *	返回值:
 *   创建人：陶林，2013/04/23
 ******************************************************************************/
DH_VOID FM_APP_FmMontStart(DH_VOID)
{
	if (GloVarFlag.FmMontFlg == 0)
	{
		GloVarFlag.FmMontFlg = 1;
	//	TIMER_DRV_Disable(TIM4);
	//	TIMER_DRV_Disable(TIM5);
		CheckctdcsFlg = 0;/*重新检测是否有有效信号接入*/
	//	GPIO_DRV_IntDisable(RDA_SQ_PIN);
	//	GPIO_DRV_IntDisable(RDA_CDCSS_PIN);
		FM_APP_FmVoiceStart();
//		if (g_FMIC_Sel == FM_IC_RDA1846S)RDA1846S_INTF_FMZBSet();
	}
}

/*****************************************************************************
 *   函数名: INTF_FmMontStop
 *   功能描述:
 *   参数定义:
 *	返回值:
 *   创建人：陶林，2013/04/23
 ******************************************************************************/
DH_VOID FM_APP_FmMontStop(DH_VOID)
{
	if (GloVarFlag.FmMontFlg == 1)
	{
		CHN_INTF_LOCK;
		GloVarFlag.FmMontCFlg = 0;
		GloVarFlag.FmMontFlg = 0;
		FM_APP_FmVoiceStop();
	//	CALLDISP_APP_LEDCallDisp();
		CHN_INTF_UNLOCK;
	}
}
#endif


DH_VOID FM_INTF_CTCSSInit(DH_U8 Attr)
{
//	cdcss_param_clear();
	AttrTail = Attr;
	g_DecodeRight = 0;
	CtcMatchFlg = 0;
	TailMatchFlg = 0;
	CtcMatchCnt = 0;
	CompareFlg = 0x7e;

	g_CTCBufMaxNum = (CtcssTones[SubDcsCode]+50)/1000;
	if(g_CTCBufMaxNum>25)g_CTCBufMaxNum = 25;
	if(AttrTail==1) g_CTCBufMaxNum = 5;
}
DH_VOID FM_INTF_ParamInit(DH_VOID)
{
//	cdcss_param_clear();
	g_msenden = 0;/*载波消失需要灭灯*/
	analog_on = 0;
	sq_Tail = 0;
	CloseSqFlg = 0;
	analog_off = 0;
	CheckctdcsFlg = 0;
	CtcMatchCnt1 = 0;/*满足条件范围内*/
	g_DecodeRight = 0;
	CtcMatchCnt = 0;
	CtcCnt = 0;
	AttrTail = 0;
}


void FM_INTF_VolSet(DH_VOID)
{
	if(CHN_BLL_GetAttr()==CHNSTAT_FM)
	{

	//	RFIC_INTF_SetVol();
	
	}
}


DH_VOID FM_APP_SetRxInit(DH_U32 freq)
{
//	RFIC_INTF_SetRxInit(freq);
//    if (g_RecChn == FM_RF_IF_AF)
//    {
//	RF_INTF_LO_SetRxInit(freq);
//	}
	FM_APP_GetSqArray();

	HRC_BLL_SetSqLevel();
}

DH_VOID FM_APP_IC_PowerDown(DH_VOID)
{
//	RFIC_INTF_Enter_Sleep();
//	if (g_RecChn == FM_RF_IF_AF)
//	{
//	RF_INTF_LO_Enter_Sleep();
//	}
}

DH_VOID FM_APP_IC_PowerOn(DH_VOID)
{
//	RFIC_INTF_Exit_Sleep();
//	if (g_RecChn == FM_RF_IF_AF)
//	{
//	RF_INTF_LO_Exit_Sleep();
//	}
}

DH_VOID FM_APP_OpenCtcDcsTimer(DH_VOID)
{
//	if((*TIMER4_CONTROLREG & 0x01)==0)
//	{//printk("f3:%d\r\n",OS_TS_GET());
//		TIMER_DRV_Enable(TIM4);
//	}
}

DH_VOID FM_APP_StopCtcDcsTimer(DH_VOID)
{
//	TIMER_DRV_Disable(TIM4);
}


DH_VOID FM_APP_CtcDcsTimerChk(DH_VOID)
{
#if NEW_DECODE_TYEP_ADC == FM_DECODE_SEL
	if(g_msenden)//如果载波存在
	{
		FM_APP_OpenCtcDcsTimer();
	}
	else
	{
		if(CHN_BLL_GetFMRxSubVoiceType()>=FM_SUBVOICETYPE_CDCSS)
		{
			FM_APP_OpenCtcDcsTimer();
		}
	}
#endif
}
	/*****************************************************************************
	 *	 函数名: RdaSubTxTailElimSet
	 *	 功能描述:在从发切换到收时调用，用于去尾音
	 *	 参数定义:
	 *	返回值:
	 *	 创建人：陶林，2011/11/14
	******************************************************************************/
DH_VOID FM_APP_SetTail(DH_VOID)
{
	if(CHN_BLL_GetFMTxSubVoiceType()<FM_SUBVOICETYPE_CDCSS)
	{	
		DCS_Encode_Stop();//stop cdcss
		CTCSS_INTF_Set(1344);
	}
	else
	{
		CTCSS_INTF_Set(550);
	}
	Delay_ms(300);
	CTCSS_INTF_Stop();

}
DH_VOID FM_APP_SetTxSubVoice(DH_VOID)
{
	if(CHN_BLL_GetFMTxSubVoiceType()==FM_SUBVOICETYPE_CARRIER)
	{
		CTCSS_INTF_Stop();
	}
	else if(CHN_BLL_GetFMTxSubVoiceType()==FM_SUBVOICETYPE_CTCSS)
	{
		CTCSS_INTF_Set(CHN_BLL_GetTxSubvoice());
	}
	else
	{//数字亚音		 
		DCS_Encode_Start();
	}
}

DH_VOID FM_APP_SetRxSubVoice(DH_VOID)
{
	DCS_Encode_Stop();

	CTCSS_INTF_Stop();
//send stop数字亚音

} 

extern STR_SLPMODE_ Str_SlpMode;
DH_VOID FM_APP_ExitSleep(DH_VOID)
{
#if FM_RF_IF==FM_RF_SEL
	if (FM_APP_GetSqStatus())
	{
		Str_SlpMode.SlpStat = SLPSTAT_NSLP;
		SlpModeFRtcExitSM();
		Str_SlpMode.ModeMStat = POWERMODE_IDLE;
	#if NEW_DECODE_TYEP_ADC == FM_DECODE_SEL
		if(CHN_BLL_GetFMRxSubVoiceType()>=FM_SUBVOICETYPE_CDCSS)
		{
			cdcss_param_clear();
			FM_APP_OpenCtcDcsTimer();
		}
	#endif
	}
	else
	{
	//	SLP_app_C5000SlpEnter();
	}
#else
	//SLP_app_C5000SlpEnter();
#endif
}

DH_VOID FM_APP_TaskInit(DH_VOID)
{

}

uint16 SQLIntensBuf[2][11]={
	{2500,1850,1750,1600,1400,1193,1005,834,619,500,0},{0}
};

DH_U16 g_SqLevelArr[10];

DH_VOID FM_APP_GetSqArray(DH_VOID)
{
	static uint8 freqindex = 0xff;
	uint8 i, k;
	DH_U8 bufL[36];
	DH_U32 freq;
	freq = RF_BLL_GetPll().u32RxFreq;

#if (FREQ_SCOPE_SET==FREQ_400_480)
	for (k = 1; k < 8; k++)
	{
		if (freq / 10000000 < g_u32FreqSectorList[k + 1] / 10000000)break;
	}
#elif (FREQ_SCOPE_SET==FREQ_30_32\
	|| FREQ_SCOPE_SET==FREQ_66_68)
	for (k = 1; k < 3; k++)
	{
		if (freq  < g_u32FreqIntSectorList[ k ] )break;
	}
#elif FREQ_SCOPE_SET==FREQ_66_88
	for (k = 1; k < 6; k++)
	{
		if (freq  < g_u32FreqIntSectorList[ k ] )break;
	}
#else
	for (k = 1; k < 8; k++)
	{
		if (freq  < g_u32FreqIntSectorList[ k ] )break;
	}
#endif

	if(freqindex==k) return;
	freqindex=k;

	FLASH_INTF_ReadStr(TUNER_SQWDBADDR + 1 + 18 * (k - 1), bufL, 18);
	for (i = 0; i < 9; i++)
	{
		g_SqLevelArr[i] = bufL[2 * i + 1] * 256 + bufL[i * 2];
	}
}

void FM_APP_SQGetFrmE2(void)
{
	uint16 th_l_sq,th_h_sq = g_SqLevelArr[Func_Str.sqlevel];//[StrMenuOnA.Squelch];

	if(Func_Str.sqlevel>2)
		th_l_sq = g_SqLevelArr[Func_Str.sqlevel-2];		/* Shut Level(-?dBm) */
	else if(Func_Str.sqlevel==2)
		th_l_sq = g_SqLevelArr[Func_Str.sqlevel-1]+100;
	else if(Func_Str.sqlevel==1)
		th_l_sq = th_h_sq+300;

	th_h_sq +=  SQLIntensBuf[1][0];
	th_l_sq +=  SQLIntensBuf[1][0];

	if(CHN_BLL_GetFMBandWide()==BW_12_5K)
	{	
		th_h_sq+=400;
		th_l_sq+=400;
	}
	if(th_h_sq>500)
	{
		th_h_sq -= 500;
		th_l_sq -= 500;
	}
	else
	{
		th_l_sq -= th_h_sq;
		th_h_sq = 1;
	}
	if(th_h_sq<100){
		th_h_sq = 50;
		th_l_sq = 150;
	}

	gstSql.u16StartNoiseLevel = th_h_sq ;
	gstSql.u16StopNoiseLevel = th_l_sq ;

}


#if (FM_RF_SEL == FM_RF_IF)//sq detected in c7000
	#if SQ_DET_SEL==SQ_DET_31136
		const DH_U16 g_SqLevelRange[]={
				440,378,370,359,343,329,312,
				290,265,240,211,184,160,138 ,113 ,91 ,73 ,55,39,24,9};
		const DH_U8  g_SqLevelGap  []={
				40, 8 ,11 ,16 ,14 ,17 ,
				22 ,25 ,25 ,29 ,27 ,24 ,22 ,25 ,22 ,18 ,18,16,15,15 };

		#define SQ_RANGE_ADJ_MAX 126
		#define SQ_RANGE_ADJ_MIN 107
	#endif
#endif


extern DH_U16 SignalBuf[];

DH_U8 HRC_BLL_GetSqSignalndex(DH_U16 sqvalue)
{
	if(sqvalue>SQ_RANGE_ADJ_MAX) return 0;
	else if(sqvalue<SQ_RANGE_ADJ_MIN) return SQ_RANGE_ADJ_MAX - SQ_RANGE_ADJ_MIN +1;
	else return SQ_RANGE_ADJ_MAX+1-sqvalue;
}

DH_U8 HRC_BLL_GetSqSignalLevel(DH_U16 sqvalue)
{
#if FM_RF_SEL==FM_RF_IF
	DH_U8 i;
//	DH_U16 temp;
//	for(i=0;i<6;i++)
//	{
//		temp = g_SqLevelRange[HRC_BLL_GetSqSignalndex(SignalBuf[i])];

//		if(sqvalue > temp)
//		{
//			break;
//		}
//	}
	return (i>5 ? 5 : i);
#endif
}

DH_VOID HRC_BLL_GetSqLevel(DH_VOID)
{
	DH_U8 lSq = Func_Str.sqlevel;

#if (FM_RF_SEL == FM_RF_IF)//sq detected in c7000
	#if SQ_DET_SEL==SQ_DET_31136
		DH_U8 sqval = g_SqLevelArr[lSq - 1];

		sqval = HRC_BLL_GetSqSignalndex(sqval);

		if(lSq == 0)
		{
			gstSql.u16StartNoiseLevel = g_SqLevelRange[0] ;
		}
		else
		{
			gstSql.u16StartNoiseLevel = g_SqLevelRange[sqval] ;
		}

		gstSql.u16StopNoiseLevel = gstSql.u16StartNoiseLevel+g_SqLevelGap[sqval];
	#endif
#else
	if (lSq == 0)
	{
	#if (FM_LINK_SEL == FM_LINK_FM_IC)
		gstSql.u16StartNoiseLevel = 136;
	#else
		gstSql.u16StartNoiseLevel = 65535;
	#endif
	}
	else if (lSq <= 9)
	{
		gstSql.u16StartNoiseLevel = g_SqLevelArr[lSq - 1];
	}
	else
	{
		gstSql.u16StartNoiseLevel = g_SqLevelArr[0];
	}

	gstSql.u16StopNoiseLevel = gstSql.u16StartNoiseLevel + 1;
#endif
}
/***************************************************************************
 * 函数名：HRC_BLL_SetSqLevel
 * 描  述：设置C7000 FM SQL 门限值,与*FM_SQL值比较
 * 输  入：无
 * 输  出：无
 * 返回值：无
 ***************************************************************************/
DH_VOID HRC_BLL_SetSqLevel(DH_VOID)
{
	DH_U8 lSq = Func_Str.sqlevel;
	HRC_BLL_GetSqLevel();

	if (CHN_BLL_GetAttr() == CHNSTAT_FM) /*FM模式*/
	{
	#if SQ_DET_SEL==SQ_DET_31136
			DH_U8 sqval = g_SqLevelArr[lSq - 1];
			if(sqval>=126) sqval = 1;
			else if(sqval<110)sqval = 16;
			else sqval = 126-sqval;

			if(lSq == 0)
			{
				gstSql.u16StartNoiseLevel = g_SqLevelRange[0] ;
			}
			else
			{
				gstSql.u16StartNoiseLevel = g_SqLevelRange[sqval] ;
			}

			gstSql.u16StopNoiseLevel = gstSql.u16StartNoiseLevel+g_SqLevelGap[sqval];
		
	#elif (FM_LINK_SEL == FM_LINK_FM_IC)
		#if FM_IC_SEL==FM_IC_RDA1846S
			RDA1846S_INTF_WriteReg(RDA_REG_49H, 0xffff);
			FM_APP_ForceFmVoiceStop();
			dhtime_delay(30);
			RDA1846S_INTF_SetRxInit(RF_BLL_GetPll().u32RxFreq);
			HRC_BLL_SetSubMode(SUBAUDIO_RX);


//			RDA1846S_INTF_WriteReg(RDA_REG_49H, 0xffff);
//			RDA1846S_INTF_SetRxInit(RF_BLL_GetPll().u32RxFreq);
//
//			HRC_BLL_SetSubMode(SUBAUDIO_RX);
//			RDA1846S_INTF_SqThrSet(gstSql.u16StopNoiseLevel, gstSql.u16StartNoiseLevel);
//			FM_APP_ForceFmVoiceStop();

			if (g_DFMRecStat == CHNSTAT_FM && CHN_BLL_GetDFMAttr())
			{
				sq_Tail = 0;
				CHN_BLL_SetDFMRecStat(CHNSTAT_DMR, CHNSTAT_NONE);
			}
		#elif FM_IC_SEL==FM_IC_W12T
			DH_U8 lSq;
			if(gstSql.u16StartNoiseLevel>130)lSq = 0;
			else lSq = 130-gstSql.u16StartNoiseLevel;
/*			W12T_INTF_Set_SQ_Level(gstSql.u16StartNoiseLevel-20);
			dhtime_delay(30);*/
//			W12T_INTF_Set_SQ_Level(0);

			W12T_INTF_Set_SQ_Level(gstSql.u16StartNoiseLevel);//dhtime_delay(30);

			FM_APP_ForceFmVoiceStop();

			if (g_DFMRecStat == CHNSTAT_FM && CHN_BLL_GetDFMAttr())
			{
				sq_Tail = 0;
				CHN_BLL_SetDFMRecStat(CHNSTAT_DMR, CHNSTAT_NONE);
			}
		#endif
	#endif
	}
	
}


#endif
