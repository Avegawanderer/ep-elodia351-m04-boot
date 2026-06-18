/*******************************************************************************
 * Copyright (C) 2011-2013 ZheJiang Dahua Technology CO.,LTD.
 *
 * 文件名 : INTF_VOL.c
 *
 * 作  者 : 123(ggg)<gg>
 * 版  本 : V1.0.0
 * 日  期 : 2015年4月7日 Create
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


/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include "app_config.h"
 

#include "include.h"


/* ========================================================================== */
/*                           宏和类型定义区                                   */
/* ========================================================================== */
#define CTCSSPHASE_HARD  0
#define CTCSSPHASE_SOFT  1
#define CTCSSPHASE_TYPE_SEL CTCSSPHASE_SOFT
#define PhaseBufMax 48

/* ========================================================================== */
/*                          数据结构定义区                                    */
/* ========================================================================== */
#if CTCSSPHASE_TYPE_SEL == CTCSSPHASE_SOFT && FUNC_CTCSSPHASETAIL
struct {
	DH_U8 t_i;
	DH_U8 bufnum;
	DH_U16 ctcss_T ;
//	DH_U16 phase1;
	DH_U16 phase120[2];
	DH_U16 phase180[2];
	DH_U16 phase240[2];
	DH_U32 Tgroup[PhaseBufMax];// 存储高低电平计算的亚音周期
} str_CtcssPhase = {
	.t_i = 0,
};
#endif

/* ========================================================================== */
/*                          函数声明区                                        */
/* ========================================================================== */
extern DH_U8 sq_PhaseTail ;
extern DH_U8 sq_Tail ;
extern DH_U8 analog_on;
extern DH_U8 analog_off;
/*******************************************************************************
 * 函数名  : ctcssPhaseInit
 * 描  述  : 读取音量值
 * 输  入  : 无
 * 输  出  : 无
 * 返回值  : 音量值
 *******************************************************************************/
void ctcssPhaseInit(DH_U16 freq)
{
#if CTCSSPHASE_TYPE_SEL == CTCSSPHASE_SOFT && FUNC_CTCSSPHASETAIL
	DH_U8 i,gap;
//	DH_U16 phase1,phase2;
	for(i=0;i<PhaseBufMax;i++)
	{
		str_CtcssPhase.Tgroup[i] = 0;
	}
	str_CtcssPhase.t_i = 0;
	str_CtcssPhase.ctcss_T = 100000000 / freq;

	str_CtcssPhase.bufnum = 4*freq/1000;
	if(str_CtcssPhase.bufnum%4) str_CtcssPhase.bufnum = (str_CtcssPhase.bufnum+3)/4*4;//4倍数

	if(str_CtcssPhase.bufnum > PhaseBufMax)str_CtcssPhase.bufnum = PhaseBufMax;

	//相位判断范围
	gap = freq/130;

//	if(StrMenuSet.CtcTail==1)
//	{
//		phase1 = 1200+gap;
//		phase2 = 1200-gap;
//	}
//	else if(StrMenuSet.CtcTail==2)
//	{
//		phase1 = 1800+gap;
//		phase2 = 1800-gap;
//	}
//	else if(StrMenuSet.CtcTail==3)
//	{
//		phase1 = 2400+gap;
//		phase2 = 2400-gap;
//	}
//	else
//	{
//		phase1 = 360;
//		phase2 = 360;
//	}

	str_CtcssPhase.phase120[0] = (3600-(1200+gap))*str_CtcssPhase.ctcss_T/3600;
	str_CtcssPhase.phase120[1] = (3600-(1200-gap))*str_CtcssPhase.ctcss_T/3600;

	str_CtcssPhase.phase180[0] = (3600-(1800+gap))*str_CtcssPhase.ctcss_T/3600;
	str_CtcssPhase.phase180[1] = (3600-(1800-gap))*str_CtcssPhase.ctcss_T/3600;

	str_CtcssPhase.phase240[0] = (3600-(2400+gap))*str_CtcssPhase.ctcss_T/3600;
	str_CtcssPhase.phase240[1] = (3600-(2400-gap))*str_CtcssPhase.ctcss_T/3600;





//	printk("gap %d %d,%d %d %d %d\r\n",freq,gap,str_CtcssPhase.bufnum,3600-str_CtcssPhase.phase1*3600/str_CtcssPhase.ctcss_T,str_CtcssPhase.phase1, str_CtcssPhase.phase2);
#endif
}


void printperiod(void)
{
//		DH_U8 i;
//		for(i=0;i<str_CtcssPhase.bufnum;i++)
//		{
//			DH_U8 index = (str_CtcssPhase.t_i+i)%str_CtcssPhase.bufnum;
//			printk("%d %d %d\r\n",index,
//					str_CtcssPhase.Tgroup[index],
//					i>=str_CtcssPhase.bufnum/2?str_CtcssPhase.bufnum:3600-((str_CtcssPhase.Tgroup[(str_CtcssPhase.t_i+str_CtcssPhase.bufnum/2+i)%str_CtcssPhase.bufnum]-str_CtcssPhase.Tgroup[(str_CtcssPhase.t_i+i)
//					 %str_CtcssPhase.bufnum])%str_CtcssPhase.ctcss_T)*3600/str_CtcssPhase.ctcss_T);
//		}
//		printk("%d \r\n",str_CtcssPhase.ctcss_T);
//		printk("\r\n");


}

/*软解亚音相位*/
void   ctcssPhaseDecode(DH_U16 perid )
{
#if CTCSSPHASE_TYPE_SEL == CTCSSPHASE_SOFT && FUNC_CTCSSPHASETAIL
	DH_U8 i,phasematch[3] = {0};
	static DH_U8 phasecnt = 0;
	DH_U16 phase ;
	for(i=1;i<str_CtcssPhase.bufnum;i++)
	{
		str_CtcssPhase.Tgroup[(str_CtcssPhase.t_i+i)%str_CtcssPhase.bufnum] -= str_CtcssPhase.Tgroup[str_CtcssPhase.t_i];
	}
	str_CtcssPhase.Tgroup[str_CtcssPhase.t_i] = str_CtcssPhase.Tgroup[(str_CtcssPhase.t_i+str_CtcssPhase.bufnum-1)%str_CtcssPhase.bufnum]+perid;
	str_CtcssPhase.t_i++;
	str_CtcssPhase.t_i %= str_CtcssPhase.bufnum;

	if(GloVarFlag.FmRecStat)
	{
		if(phasecnt<str_CtcssPhase.bufnum/2)
		{
			phasecnt ++;
			return;
		}
		for(i=0;i<str_CtcssPhase.bufnum>>1;i++)
		{
	//		DH_U16 phase = 3600-((str_CtcssPhase.Tgroup[(str_CtcssPhase.t_i+(str_CtcssPhase.bufnum>>1)+i)%str_CtcssPhase.bufnum]-
	//						str_CtcssPhase.Tgroup[(str_CtcssPhase.t_i+i)
	//					    %str_CtcssPhase.bufnum])%str_CtcssPhase.ctcss_T)*3600/str_CtcssPhase.ctcss_T;
			phase = (str_CtcssPhase.Tgroup[(str_CtcssPhase.t_i+i+(str_CtcssPhase.bufnum>>1))%str_CtcssPhase.bufnum]-\
							str_CtcssPhase.Tgroup[(str_CtcssPhase.t_i+i) % str_CtcssPhase.bufnum])%str_CtcssPhase.ctcss_T;

			if(phase > str_CtcssPhase.phase120[0] && phase < str_CtcssPhase.phase120[1])
			{
				phasematch[0]++;
			}
			else if(phase > str_CtcssPhase.phase180[0] && phase < str_CtcssPhase.phase180[1])
			{
				phasematch[1]++;
			}else if(phase > str_CtcssPhase.phase240[0] && phase < str_CtcssPhase.phase240[1])
			{
				phasematch[2]++;
			}
		}
		phase = str_CtcssPhase.bufnum>>3;
		if(!sq_PhaseTail && !sq_Tail && (phasematch[0]>=phase || phasematch[1]>=phase || phasematch[2]>=phase))
		{
			sq_PhaseTail = 13;
		}
	}
	else
	{
		phasecnt  = 0;
	}
#endif
}

extern DH_U8 g_SqStatus;
void ctcssPhaseProc(DH_VOID )
{
#if FUNC_CTCSSPHASETAIL
#if CTCSSPHASE_TYPE_SEL == CTCSSPHASE_HARD
	if((GloVarFlag.FmRecStat || sq_PhaseTail)&&CHN_BLL_GetFMRxSubVoiceType() == FM_SUBVOICETYPE_CTCSS)
	{
		if(RDA1846S_INTF_GetRxCTSTail())
		{
			analog_on = 0;
			analog_off = 8;
			sq_PhaseTail = 10;
			//printperiod();
		}
		else
		{
			if(FM_APP_GetSqStatus()==0) sq_PhaseTail = 0;
			if(sq_PhaseTail)
			{
				sq_PhaseTail --;
			}
		}
	}
#else
	if((GloVarFlag.FmRecStat || sq_PhaseTail)&&CHN_BLL_GetFMRxSubVoiceType() == FM_SUBVOICETYPE_CTCSS)
	{
		if(g_SqStatus==0) sq_PhaseTail = 0;
		if(sq_PhaseTail)
		{
			analog_on = 0;
			analog_off = 10;
			sq_PhaseTail --;
		}
	}
#endif
#endif
}
