/*******************************************************************************
 * Copyright (C) 2016, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software is owned and published by:
 * Huada Semiconductor Co., Ltd. ("HDSC").
 *
 * BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND
 * BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
 *
 * This software contains source code for use with HDSC
 * components. This software is licensed by HDSC to be adapted only
 * for use in systems utilizing HDSC components. HDSC shall not be
 * responsible for misuse or illegal use of this software for devices not
 * supported herein. HDSC is providing this software "AS IS" and will
 * not be responsible for issues arising from incorrect user implementation
 * of the software.
 *
 * Disclaimer:
 * HDSC MAKES NO WARRANTY, EXPRESS OR IMPLIED, ARISING BY LAW OR OTHERWISE,
 * REGARDING THE SOFTWARE (INCLUDING ANY ACCOMPANYING WRITTEN MATERIALS),
 * ITS PERFORMANCE OR SUITABILITY FOR YOUR INTENDED USE, INCLUDING,
 * WITHOUT LIMITATION, THE IMPLIED WARRANTY OF MERCHANTABILITY, THE IMPLIED
 * WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE OR USE, AND THE IMPLIED
 * WARRANTY OF NONINFRINGEMENT.
 * HDSC SHALL HAVE NO LIABILITY (WHETHER IN CONTRACT, WARRANTY, TORT,
 * NEGLIGENCE OR OTHERWISE) FOR ANY DAMAGES WHATSOEVER (INCLUDING, WITHOUT
 * LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION,
 * LOSS OF BUSINESS INFORMATION, OR OTHER PECUNIARY LOSS) ARISING FROM USE OR
 * INABILITY TO USE THE SOFTWARE, INCLUDING, WITHOUT LIMITATION, ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL OR CONSEQUENTIAL DAMAGES OR LOSS OF DATA,
 * SAVINGS OR PROFITS,
 * EVEN IF Disclaimer HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * YOU ASSUME ALL RESPONSIBILITIES FOR SELECTION OF THE SOFTWARE TO ACHIEVE YOUR
 * INTENDED RESULTS, AND FOR THE INSTALLATION OF, USE OF, AND RESULTS OBTAINED
 * FROM, THE SOFTWARE.
 *
 * This software may be replicated in part or whole for the licensed use,
 * with the restriction that this Disclaimer and Copyright notice must be
 * included with each copy of this software, whether used in part or whole,
 * at all times.
 */
/******************************************************************************/
/** \file main.c
 **
 ** \brief This example demonstrates how to use the through mode function of
 **        Timer4Pwm.
 **
 **   - 2018-10-30  1.0  Hongjh First version for Device Driver Library of
 **                      Timer4Pwm
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/

#include "app_config.h"
#include "INTF_LED.h"


/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* Timer4 CNT */
#define TIMER4_UNIT                     (M4_TMR42)
#define TIMER4_CNT_CYCLE_VAL            (50000u)        /* Timer4 counter cycle value */

/* Timer4 OCO */
#define TIMER4_OCO_HIGH_CH              (Timer4OcoOuh)  /* only Timer4OcoOuh  Timer4OcoOvh  Timer4OcoOwh */

/* Timer4 PWM */
#define TIMER4_PWM_CH                   (Timer4PwmU)    /* only Timer4PwmU  Timer4PwmV  Timer4PwmW */

/* Define port and pin for Timer4Pwm */
#define TIMER4_PWM_H_PORT               (PortA)         /* TIM4_1_OUH_B:PE9   TIM4_1_OVH_B:PE11   TIM4_1_OWH_B:PE13 */
#define TIMER4_PWM_H_PIN                (Pin00)

/* Timer4 RT interrupt number */
#define TIMER4_RT_INT_NUM               (INT_TMR42_GCMUH)

const uint16 SinaValueGrp[]={
2      ,
21     ,
115    ,
284    ,
526    ,
843    ,
1232   ,
1694   ,
2226   ,
2827   ,
3496   ,
4232   ,
5032   ,
5894   ,
6817   ,
7798   ,
8834   ,
9923   ,
11062  ,
12249  ,
13479  ,
14752  ,
16062  ,
17407  ,
18784  ,
20188  ,
21618  ,
23068  ,
24535  ,
26016  ,
27507  ,
29005  ,
30504  ,
32003  ,
33496  ,
34981  ,
36454  ,
37910  ,
39346  ,
40759  ,
42145  ,
43501  ,
44823  ,
46108  ,
47353  ,
48555  ,
49710  ,
50815  ,
51869  ,
52868  ,
53810  ,
54692  ,
55513  ,
56270  ,
56961  ,
57585  ,
58140  ,
58625  ,
59038  ,
59378  ,
59645  ,
59838  ,
59956  ,
60000  ,
59968  ,
59862  ,
59681  ,
59425  ,
59097  ,
58695  ,
58222  ,
57678  ,
57065  ,
56385  ,
55638  ,
54827  ,
53955  ,
53022  ,
52032  ,
50987  ,
49889  ,
48742  ,
47548  ,
46309  ,
45031  ,
43714  ,
42364  ,
40982  ,
39573  ,
38140  ,
36687  ,
35217  ,
33734  ,
32241  ,
30743  ,
29243  ,
27745  ,
26253  ,
24770  ,
23300  ,
21847  ,
20414  ,
19006  ,
17624  ,
16274  ,
14958  ,
13679  ,
12442  ,
11248  ,
10101  ,
9004   ,
7959   ,
6969   ,
6037   ,
5165   ,
4355   ,
3609   ,
2929   ,
2317   ,
1774   ,
1301   ,
900    ,
572    ,
317    ,
137    ,
31     ,
0      ,
};
uint8 pulseperPeriod;
uint8 sinaindex;
uint16_t u16Cycle;
uint16_t u16DuteGrp[100];

void CTCSS_INTF_Set(uint16 ctcssfreq)
{
	if(ctcssfreq==0){
	//	TIMER4_CNT_Stop(TIMER4_UNIT);
	}
	else
	{
		//5KHz左右的PWM波，亚音频倍数，
		pulseperPeriod = 54000/ctcssfreq;//1周期内N个PWM
		//获取各个pwm波的占空比，预先计算，减少中断计算时间

		#if 1		
		u16Cycle = 105000000*4/ctcssfreq/pulseperPeriod;//ctcssfreq x10倍，（62.5x10 = 625）

		TIMER4_CNT_SetCycleVal(TIMER4_UNIT,  u16Cycle);
		uint8 i;
		for(i = 0;i<pulseperPeriod;i++)
		{
			u16DuteGrp[i] = SinaValueGrp[i*126/pulseperPeriod]+2500;
			u16DuteGrp[i] = u16DuteGrp[i]*u16Cycle/65535;
		}
		#else
		uint16_t u16Cycle = 105000000/4/ctcssfreq;//ctcssfreq x10倍，（62.5x10 = 625）

		TIMER4_CNT_SetCycleVal(TIMER4_UNIT,  u16Cycle);
		TIMER4_OCO_WriteOccr(TIMER4_UNIT, TIMER4_OCO_HIGH_CH, u16Cycle/2);
		#endif	
		TIMER4_CNT_Start(TIMER4_UNIT);
	}

}

/**
 *******************************************************************************
 ** \brief pwm couple channel reload timer interrupt handler
 **
 ******************************************************************************/
static void PwmCoupleChIrqCallback(void)
{
	TIMER4_OCO_WriteOccr(TIMER4_UNIT, TIMER4_OCO_HIGH_CH,u16DuteGrp[0]);

//PORT_Toggle(LED_GREEN_PORT, LED_GREEN_PIN);

    TIMER4_OCO_ClearIrqFlag(TIMER4_UNIT, TIMER4_OCO_HIGH_CH);

	sinaindex++;

    sinaindex %= pulseperPeriod;
}

/**
 *******************************************************************************
 ** \brief  Main function of project
 **
 ** \param  None
 **
 ** \retval int32_t return value, if needed
 **
 ******************************************************************************/
void Timer4_CTCSS_INIT(void)
{
    stc_timer4_cnt_init_t stcCntInit;
    stc_timer4_oco_init_t stcOcoInit;
    stc_timer4_pwm_init_t stcPwmInit;
    stc_oco_low_ch_compare_mode_t stcLowChCmpMode;
    stc_oco_high_ch_compare_mode_t stcHighChCmpMode;
    uint16_t u8OcoOccrVal = 100/2u;
//    en_timer4_oco_ch_t enOcoLowCh = (en_timer4_oco_ch_t)(TIMER4_OCO_HIGH_CH + 1);

    /* Clear structures */
    MEM_ZERO_STRUCT(stcCntInit);
    MEM_ZERO_STRUCT(stcOcoInit);
    MEM_ZERO_STRUCT(stcLowChCmpMode);
    MEM_ZERO_STRUCT(stcHighChCmpMode);
    MEM_ZERO_STRUCT(stcPwmInit);

    /* Enable peripheral clock */
    PWC_Fcg2PeriphClockCmd( PWC_FCG2_PERIPH_TIM42 , Enable);

    /* Timer4 CNT : Initialize CNT configuration structure */
    stcCntInit.enBufferCmd = Disable;
    stcCntInit.enClk = Timer4CntPclk;
    stcCntInit.enClkDiv = Timer4CntPclkDiv1;  /* CNT clock divide */
    stcCntInit.u16Cycle = 100;
    stcCntInit.enCntMode = Timer4CntSawtoothWave;
    stcCntInit.enZeroIntCmd = Disable;
    stcCntInit.enPeakIntCmd = Disable;
    stcCntInit.enZeroIntMsk = Timer4CntIntMask0;
    stcCntInit.enPeakIntMsk = Timer4CntIntMask0;
    TIMER4_CNT_Init(TIMER4_UNIT, &stcCntInit); /* Initialize CNT */

    /* Timer4 OCO : Initialize OCO configuration structure */
    stcOcoInit.enOccrBufMode = OccrBufDisable;
    stcOcoInit.enOcmrBufMode = OcmrBufDisable;
    stcOcoInit.enPortLevel = OcPortLevelLow;
    stcOcoInit.enOcoIntCmd = Enable;
    TIMER4_OCO_Init(TIMER4_UNIT, TIMER4_OCO_HIGH_CH, &stcOcoInit); /* Initialize OCO high channel */
 //  TIMER4_OCO_Init(TIMER4_UNIT, enOcoLowCh, &stcOcoInit);         /* Initialize OCO low channel */

    if (!(TIMER4_OCO_HIGH_CH % 2))
    {
        /* ocmr[15:0] = 0x0FFF */
        stcHighChCmpMode.enCntZeroMatchOpState = OcoOpOutputReverse;
        stcHighChCmpMode.enCntZeroNotMatchOpState = OcoOpOutputHold;
        stcHighChCmpMode.enCntUpCntMatchOpState = OcoOpOutputReverse;
        stcHighChCmpMode.enCntPeakMatchOpState = OcoOpOutputReverse;
        stcHighChCmpMode.enCntPeakNotMatchOpState = OcoOpOutputHold;
        stcHighChCmpMode.enCntDownCntMatchOpState = OcoOpOutputReverse;

        stcHighChCmpMode.enCntZeroMatchOcfState = OcoOcfSet;
        stcHighChCmpMode.enCntUpCntMatchOcfState = OcoOcfSet;
        stcHighChCmpMode.enCntPeakMatchOcfState = OcoOcfSet;
        stcHighChCmpMode.enCntDownCntMatchOcfState = OcoOcfSet;

        stcHighChCmpMode.enMatchConditionExtendCmd = Disable;

        TIMER4_OCO_SetHighChCompareMode(TIMER4_UNIT, TIMER4_OCO_HIGH_CH, &stcHighChCmpMode);  /* Set OCO high channel compare mode */
    }

    /*************Timer4 OCO ocmr1[31:0] = 0x0FF0 0FFF*****************************/
//    if (enOcoLowCh % 2)
//    {
//        /* OCMR[31:0] Ox 0FF0 0FFF    0000 1111 1111 0000   0000 1111 1111 1111 */
//        stcLowChCmpMode.enCntZeroLowMatchHighMatchLowChOpState = OcoOpOutputReverse;         /* bit[27:26]  11 */
//        stcLowChCmpMode.enCntZeroLowMatchHighNotMatchLowChOpState = OcoOpOutputReverse;      /* bit[11:10]  11 */
//        stcLowChCmpMode.enCntZeroLowNotMatchHighMatchLowChOpState = OcoOpOutputHold;         /* bit[31:30]  00 */
//        stcLowChCmpMode.enCntZeroLowNotMatchHighNotMatchLowChOpState = OcoOpOutputHold;      /* bit[15:14]  00 */
//
//        stcLowChCmpMode.enCntUpCntLowMatchHighMatchLowChOpState = OcoOpOutputReverse;        /* bit[25:24]  11 */
//        stcLowChCmpMode.enCntUpCntLowMatchHighNotMatchLowChOpState = OcoOpOutputReverse;     /* bit[9:8]    11 */
//        stcLowChCmpMode.enCntUpCntLowNotMatchHighMatchLowChOpState = OcoOpOutputHold;        /* bit[19:18]  00 */
//
//        stcLowChCmpMode.enCntPeakLowMatchHighMatchLowChOpState = OcoOpOutputReverse;         /* bit[23:22]  11 */
//        stcLowChCmpMode.enCntPeakLowMatchHighNotMatchLowChOpState = OcoOpOutputReverse;      /* bit[7:6]    11 */
//        stcLowChCmpMode.enCntPeakLowNotMatchHighMatchLowChOpState = OcoOpOutputHold;         /* bit[29:28]  00 */
//        stcLowChCmpMode.enCntPeakLowNotMatchHighNotMatchLowChOpState = OcoOpOutputHold;      /* bit[13:12]  00 */
//
//        stcLowChCmpMode.enCntDownLowMatchHighMatchLowChOpState = OcoOpOutputReverse;         /* bit[21:20]  11 */
//        stcLowChCmpMode.enCntDownLowMatchHighNotMatchLowChOpState = OcoOpOutputReverse;      /* bit[5:4]    11 */
//        stcLowChCmpMode.enCntDownLowNotMatchHighMatchLowChOpState = OcoOpOutputHold;         /* bit[17:16]  00 */
//
//        stcLowChCmpMode.enCntZeroMatchOcfState = OcoOcfSet;    /* bit[3] 1 */
//        stcLowChCmpMode.enCntUpCntMatchOcfState = OcoOcfSet;   /* bit[2] 1 */
//        stcLowChCmpMode.enCntPeakMatchOcfState = OcoOcfSet;    /* bit[1] 1 */
//        stcLowChCmpMode.enCntDownCntMatchOcfState = OcoOcfSet; /* bit[0] 1 */
//
//        TIMER4_OCO_SetLowChCompareMode(TIMER4_UNIT, enOcoLowCh, &stcLowChCmpMode);  /* Set OCO low channel compare mode */
//    }

    /* Set OCO compare value */
    TIMER4_OCO_WriteOccr(TIMER4_UNIT, TIMER4_OCO_HIGH_CH, u8OcoOccrVal);

    /* Enable OCO */
    TIMER4_OCO_OutputCompareCmd(TIMER4_UNIT, TIMER4_OCO_HIGH_CH, Enable);

    /* Initialize PWM I/O */
    PORT_SetFunc(TIMER4_PWM_H_PORT, TIMER4_PWM_H_PIN, Func_Tim4, Disable);

    /* Timer4 PWM: Initialize PWM configuration structure */
    stcPwmInit.enRtIntMaskCmd = Enable;
    stcPwmInit.enClkDiv = PwmPlckDiv1;
    stcPwmInit.enOutputState = PwmHHoldPwmLReverse;
    stcPwmInit.enMode = PwmThroughMode;
    TIMER4_PWM_Init(TIMER4_UNIT, TIMER4_PWM_CH, &stcPwmInit); /* Initialize timer4 pwm */



    stc_irq_regi_conf_t stcIrqRegiCfg;
    MEM_ZERO_STRUCT(stcIrqRegiCfg);

    /* Set Timer4 PWM RT IRQ */
    MEM_ZERO_STRUCT(stcIrqRegiCfg);
    stcIrqRegiCfg.enIRQn = Int003_IRQn;
    stcIrqRegiCfg.pfnCallback = &PwmCoupleChIrqCallback;
    stcIrqRegiCfg.enIntSrc = TIMER4_RT_INT_NUM;
    enIrqRegistration(&stcIrqRegiCfg);
    NVIC_SetPriority(stcIrqRegiCfg.enIRQn, DDL_IRQ_PRIORITY_DEFAULT);
    NVIC_ClearPendingIRQ(stcIrqRegiCfg.enIRQn);
    NVIC_EnableIRQ(stcIrqRegiCfg.enIRQn);





	
    /* Clear && Start CNT */
    TIMER4_CNT_ClearCountVal(TIMER4_UNIT);
   // TIMER4_CNT_Start(TIMER4_UNIT);

	 CTCSS_INTF_Set(625);
	 CTCSS_INTF_Set(1133);
	 CTCSS_INTF_Set(2540);
}


void  CTCSS_INTF_Stop(void)
{
//	TIMER4_CNT_Stop(TIMER4_UNIT);
}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
