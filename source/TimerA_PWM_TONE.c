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

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/* TIMERA unit and clock definition */
#define TIMERA_UNIT2                    (M4_TMRA2)
#define TIMERA_UNIT2_CLOCK              (PWC_FCG2_PERIPH_TIMA2)
#define TIMERA_UNIT2_OVERFLOW_INT       (INT_TMRA2_OVF)

/* TIMERA channel 1 Port/Pin definition */
#define TIMERA_UNIT2_CH1                (TimeraCh1)
#define TIMERA_UNIT2_CH1_PORT           (PortA)
#define TIMERA_UNIT2_CH1_PIN            (Pin00)
#define TIMERA_UNIT2_CH1_FUNC           (Func_Tima0)

/* TIMERA channel 3 Port/Pin definition */
#define TIMERA_UNIT2_CH3                (TimeraCh3)
#define TIMERA_UNIT2_CH3_PORT           (PortB)
#define TIMERA_UNIT2_CH3_PIN            (Pin10)
#define TIMERA_UNIT2_CH3_FUNC           (Func_Tima0)

void TIMERA_SetCompare(M4_TMRA_TypeDef *TIMERAx, en_timera_channel_t enChannel,uint16 duty)
{
    stc_timera_compare_init_t stcTimerCompareInit;
    MEM_ZERO_STRUCT(stcTimerCompareInit);

    /* Configuration timera unit 2 compare structure */
    stcTimerCompareInit.u16CompareVal = duty;
    stcTimerCompareInit.enCompareMatchOutput = TimeraCompareMatchOutputReverse;
    stcTimerCompareInit.enPeriodMatchOutput = TimeraPeriodMatchOutputKeep;
    stcTimerCompareInit.enSpecifyOutput = TimeraSpecifyOutputInvalid;
    stcTimerCompareInit.enCacheEn = Disable;
    stcTimerCompareInit.enTriangularTroughTransEn = Disable;
    stcTimerCompareInit.enTriangularCrestTransEn = Disable;
    stcTimerCompareInit.u16CompareCacheVal = stcTimerCompareInit.u16CompareVal;

    /* Configure channel  */
    stcTimerCompareInit.enStartCountOutput = TimeraCountStartOutputLow;
    stcTimerCompareInit.enStopCountOutput = TimeraCountStartOutputLow;
    TIMERA_CompareInit(TIMERAx, enChannel, &stcTimerCompareInit);
}



void Tone_SetFreq(uint16 freq)
{
//=pclk1/div/2/freq
	freq = 10500000/freq;

	TIMERA_SetPeriodValue(TIMERA_UNIT2, freq);
	TIMERA_SetCompare(TIMERA_UNIT2, TIMERA_UNIT2_CH3,freq/2);//占空比
	TIMERA_Cmd(TIMERA_UNIT2,Enable);
}

void Tone_SendStop(void)
{
	TIMERA_Cmd(TIMERA_UNIT2,Disable);
}

/**
 *******************************************************************************
 ** \brief Configure Timera peripheral function
 **
 ** \param [in] None
 **
 ** \retval None
 **
 ******************************************************************************/
void Timera_Config_Tone(void)
{
    stc_timera_base_init_t stcTimeraInit;
    stc_irq_regi_conf_t stcIrqRegiConf;
 //   stc_timera_hw_startup_cofig_t stcTimeraHwConfig;
    stc_port_init_t stcPortInit;

    /* configuration structure initialization */
    MEM_ZERO_STRUCT(stcTimeraInit);
    MEM_ZERO_STRUCT(stcIrqRegiConf);
//    MEM_ZERO_STRUCT(stcTimeraHwConfig);
    MEM_ZERO_STRUCT(stcPortInit);

    /* Configuration peripheral clock */
    PWC_Fcg2PeriphClockCmd(TIMERA_UNIT2_CLOCK, Enable);
 //   PWC_Fcg0PeriphClockCmd(PWC_FCG0_PERIPH_PTDIS, Enable);

    /* Configuration TIMERA compare pin */
    PORT_SetFunc(TIMERA_UNIT2_CH3_PORT, TIMERA_UNIT2_CH3_PIN, TIMERA_UNIT2_CH3_FUNC, Disable);

    /* Configuration timera unit 2 base structure */
    stcTimeraInit.enClkDiv = TimeraPclkDiv4;//freqckl = 84M/TimeraPclkDiv4
    stcTimeraInit.enCntMode = TimeraCountModeSawtoothWave;//TimeraCountModeTriangularWave;
    stcTimeraInit.enCntDir = TimeraCountDirUp;
    stcTimeraInit.enSyncStartupEn = Disable;
    stcTimeraInit.u16PeriodVal = 60500;        //freq: 100Hz = freqckl/freq/2
    TIMERA_BaseInit(TIMERA_UNIT2, &stcTimeraInit);
   
	TIMERA_SetCompare(TIMERA_UNIT2, TIMERA_UNIT2_CH3,stcTimeraInit.u16PeriodVal/2);
    TIMERA_CompareCmd(TIMERA_UNIT2, TIMERA_UNIT2_CH3, Enable);

    /* Enable period count interrupt */
 //   TIMERA_IrqCmd(TIMERA_UNIT2, TimeraIrqOverflow, Enable);
    /* Interrupt of timera unit 2 */
//    stcIrqRegiConf.enIntSrc = TIMERA_UNIT2_OVERFLOW_INT;
//    stcIrqRegiConf.enIRQn = Int006_IRQn;
//    stcIrqRegiConf.pfnCallback = &TimeraUnit1_IrqCallback;
//    enIrqRegistration(&stcIrqRegiConf);
//    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
//    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_15);
//    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);

//	TIMERA_Cmd(TIMERA_UNIT2,Enable);
//	uint16 u16DutyCycle = TIMERA_GetCompareValue(TIMERA_UNIT2, TIMERA_UNIT2_CH3);

    /* Configure timera unit 2 startup */
//    stcTimeraHwConfig.enSpecifyEventStartupEn = Enable;
//    stcTimeraHwConfig.enTrigFallingStartupEn = Disable;
//    stcTimeraHwConfig.enTrigRisingStartupEn = Disable;
//    TIMERA_HwStartupConfig(TIMERA_UNIT2, &stcTimeraHwConfig);

    /* Set external Int Ch.4 trigger timera startup */
  //  stcPortInit.enExInt = Enable;
 //   PORT_Init(KEY1_PORT, KEY1_PIN, &stcPortInit);
  //  TIMERA_SetCountTriggerSrc(KEY1_TRIGGER_EVENT);
}



#if  0
/* TIMERA unit and clock definition */
#define TIMERA_UNIT1                    (M4_TMRA1)
#define TIMERA_UNIT1_CLOCK              (PWC_FCG2_PERIPH_TIMA1)
#define TIMERA_UNIT1_OVERFLOW_INT       (INT_TMRA1_OVF)

/* TIMERA channel 1 Port/Pin definition */
#define TIMERA_UNIT1_CH2                (TimeraCh2)
#define TIMERA_UNIT1_CH2_PORT           (PortA)
#define TIMERA_UNIT1_CH2_PIN            (Pin09)
#define TIMERA_UNIT1_CH2_FUNC           (Func_Tima0)



void APC_SetDuty(uint16 percent)
{
	uint16 period = TIMERA_GetPeriodValue(TIMERA_UNIT1);
	if(percent == 0)TIMERA_Cmd(TIMERA_UNIT1,Disable);
	else 
	{
	//	TIMERA_Cmd(TIMERA_UNIT1,Disable);	
		period = percent * period /100;
	
		TIMERA_SetCacheValue(TIMERA_UNIT1, TIMERA_UNIT1_CH2,period);
	//	TIMERA_SetCompare(TIMERA_UNIT1, TIMERA_UNIT1_CH2,period);//占空比
		TIMERA_Cmd(TIMERA_UNIT1,Enable);
	}
}
void Timera_Config_Apc(void)
{
    stc_timera_base_init_t stcTimeraInit;
    stc_irq_regi_conf_t stcIrqRegiConf;
    stc_timera_hw_startup_cofig_t stcTimeraHwConfig;
    stc_port_init_t stcPortInit;

    /* configuration structure initialization */
    MEM_ZERO_STRUCT(stcTimeraInit);
    MEM_ZERO_STRUCT(stcIrqRegiConf);
    MEM_ZERO_STRUCT(stcTimeraHwConfig);
    MEM_ZERO_STRUCT(stcPortInit);

    /* Configuration peripheral clock */
    PWC_Fcg2PeriphClockCmd(TIMERA_UNIT1_CLOCK, Enable);
    PWC_Fcg0PeriphClockCmd(PWC_FCG0_PERIPH_PTDIS, Enable);

    /* Configuration TIMERA compare pin */
    PORT_SetFunc(TIMERA_UNIT1_CH2_PORT, TIMERA_UNIT1_CH2_PIN, TIMERA_UNIT1_CH2_FUNC, Disable);

    /* Configuration timera unit 2 base structure */
    stcTimeraInit.enClkDiv = TimeraPclkDiv4;//freqckl = 84M/TimeraPclkDiv4
    stcTimeraInit.enCntMode = TimeraCountModeSawtoothWave;//TimeraCountModeTriangularWave;
    stcTimeraInit.enCntDir = TimeraCountDirUp;
    stcTimeraInit.enSyncStartupEn = Disable;
    stcTimeraInit.u16PeriodVal = 60500;        //freq: 100Hz = freqckl/freq/2
    TIMERA_BaseInit(TIMERA_UNIT1, &stcTimeraInit);
   
	TIMERA_SetCompare(TIMERA_UNIT1, TIMERA_UNIT1_CH2,stcTimeraInit.u16PeriodVal/4);
    TIMERA_CompareCmd(TIMERA_UNIT1, TIMERA_UNIT1_CH2, Enable);


	TIMERA_Cmd(TIMERA_UNIT1,Enable);

	TIMERA_SetPeriodValue(TIMERA_UNIT1,30000);

	
}
#endif

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
