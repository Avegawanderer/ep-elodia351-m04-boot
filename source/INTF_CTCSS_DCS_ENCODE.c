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

/* Timer4 counter */
#define TIMER4_UNIT                     (M4_TMR41)
#define TIMER4_CNT_ZERO_INT_NUM         (INT_TMR41_GUDF)
#define TIMER4_CNT_CYCLE_VAL            (39063u)



/* LED0 Port/Pin definition */
#define  TX_CDCSS_PORT          (PortA)
#define  TX_CDCSS_PIN           (Pin02)

#define TX_CDCSS_H    PORT_SetBits(TX_CDCSS_PORT, TX_CDCSS_PIN)
#define TX_CDCSS_L   PORT_ResetBits(TX_CDCSS_PORT, TX_CDCSS_PIN)


//period = 1000000/134.4Hz = 7440.5us
/*********************************************************************
 * 定时器每1488us发dcs码
 */

static void ZeroMatchIrqCb(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

static uint32 dcsgraycode,dcsgraycode_t;
uint8 dcssendindex;




void CDS_INTF_Init(void)
{
	stc_port_init_t stcPortInit;

	/*initiallize LED port*/
	MEM_ZERO_STRUCT(stcPortInit);

	stcPortInit.enPinMode = Pin_Mode_Out;
	stcPortInit.enExInt = Disable;
	stcPortInit.enPullUp = Enable;

	/* LED0 Port/Pin initialization */
	PORT_Init(TX_CDCSS_PORT, TX_CDCSS_PIN, &stcPortInit);
}



/**
 *******************************************************************************
 ** \brief Zero match interrupt handler
 **
 ******************************************************************************/
static void ZeroMatchIrqCb(void)
{
	(dcsgraycode_t & 0x00000001ul) ? TX_CDCSS_H : TX_CDCSS_L;
	dcsgraycode_t >>= 1;
	dcssendindex ++;

	if(dcssendindex>=23){
		dcsgraycode_t = dcsgraycode;
		dcssendindex = 0;

	}

    /* Clear Timer4-CNT zero interrupt flag */
    TIMER4_CNT_ClearIrqFlag(TIMER4_UNIT, Timer4CntZeroMatchInt);
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
void Timer4_DCS_Encode1488us(void)
{
    stc_irq_regi_conf_t stcIrqRegiCfg;
    stc_timer4_cnt_init_t stcCntInit;

    /* Clear structures */
    MEM_ZERO_STRUCT(stcCntInit);
    MEM_ZERO_STRUCT(stcIrqRegiCfg);

    /* Enable peripheral clock */
    PWC_Fcg2PeriphClockCmd(PWC_FCG2_PERIPH_TIM41, Enable);

    /* Timer4-CNT : Initialize CNT configuration structure */
    stcCntInit.enBufferCmd = Disable;
    stcCntInit.enClk = Timer4CntPclk;
    stcCntInit.enClkDiv = Timer4CntPclkDiv16;    /* Timer4-CNT clock divide */
    stcCntInit.u16Cycle = TIMER4_CNT_CYCLE_VAL;  /* Timer4-CNT cycle */
    stcCntInit.enCntMode = Timer4CntSawtoothWave;
    stcCntInit.enZeroIntCmd = Enable;    /* Enable zero match interrupt */
    stcCntInit.enPeakIntCmd = Disable;   /* Disable peak match interrupt */
    stcCntInit.enZeroIntMsk = Timer4CntIntMask0;
    stcCntInit.enPeakIntMsk = Timer4CntIntMask0;
    TIMER4_CNT_Init(TIMER4_UNIT, &stcCntInit); /* Initialize Timer4-CNT */

    /* Set Timer4-CNT IRQ */
    stcIrqRegiCfg.enIRQn = Int002_IRQn;
    stcIrqRegiCfg.pfnCallback = &ZeroMatchIrqCb;
    stcIrqRegiCfg.enIntSrc = TIMER4_CNT_ZERO_INT_NUM;
    enIrqRegistration(&stcIrqRegiCfg);
    NVIC_SetPriority(stcIrqRegiCfg.enIRQn, DDL_IRQ_PRIORITY_DEFAULT);
    NVIC_ClearPendingIRQ(stcIrqRegiCfg.enIRQn);
    NVIC_EnableIRQ(stcIrqRegiCfg.enIRQn);

    /* Start Timer4-CNT */
    TIMER4_CNT_ClearCountVal(TIMER4_UNIT);
}
 
extern uint8 decode_type;
void DCS_Encode_Start(void)
{
	if (CHN_BLL_GetFMRxSubVoiceType() == FM_SUBVOICETYPE_CDCSS)
	{
		decode_type == DCS_ACTIVE ;
	}
	else
	{
		decode_type == DCS_ACTIVE_N ;
	}

	dcsgraycode = DCS_GetGrayCode(CHN_BLL_GetTxSubvoice());
	dcsgraycode >>=1;
	dcsgraycode_t = dcsgraycode;
	dcssendindex = 0;
	TIMER4_CNT_Start(TIMER4_UNIT);
	
}
void DCS_Encode_Stop(void)
{
	TIMER4_CNT_Stop(TIMER4_UNIT);
	TX_CDCSS_L;
}

#endif

