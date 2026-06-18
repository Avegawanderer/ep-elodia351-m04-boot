/*******************************************************************
  Filename:       CTCSS_DCS_DECODE.h
  Revised:        $Date: 2009-10-18 $
  Revision:       $ $
  Author:         cykang

  Description:

  Change History:

********************************************************************/
#if 1//(FM_DECODE_SEL==NEW_DECODE_TYEP_ADC)

#ifndef 	_INTF_CTCSS_DCS_DECODE_H
#define 	_INTF_CTCSS_DCS_DECODE_H

/*********************************************************************
 * INCLUDES
 */


/*********************************************************************
 * CONSTANTS
 */
#define CTCSS_EN //打开则加入解亚音频  关掉则不加入解码文件
#define DCS_EN //打开则加入解数字亚音频

#define ADC_QT	CHN_ADC_QT_DQT
#define BEEPIO   PIN18	//TX_LED_GPIO()	//LAMP_LED
//#define BEEPIO   KEY_LED
/* CTCSS DCS tail */
#define CTCSS_DCS_NO_TAIL			(0xff)
#define CTCSS_TAIL_50Hz				(0)
#define CTCSS_TAIL_120				(1)
#define CTCSS_TAIL_180				(2)
#define CTCSS_TAIL_240				(3)
#define DCS_TAIL_ACTIVE				(4)

#define CTCSS_TAIL_FREQ				(550)
#define DCS_TAIL_FREQ					(1344)

#define CTCSS_DCS_DELAY_TIME	(200)


#define ConstCtcssGroupNum						(8 )
#define ConstMaxFilterLengthValue			(64)/* 64 */
#define ConstCorrelationLowerValue		(2 ) /* 2 4 //2 */
#define ConstCtcssReverseEnterOkNum		(2 )/* 2 1//4 */
#define ConstCtcssBurstThresholdNum		(4 )
#define ConstCtcssReverseExitOkNum		(3 )/* 3 4 */
#define ConstSubgroupNum							(3 )
#define ConstCorrelationDelayFor200Hz	(4 )
#define ConstCorrelationDelayFor100Hz	(5 )
#define ConstCorrelationDelayFor67Hz	(9 )
#define ConstCorrelationUpperValue		(2 )/* 4//2 */
#define ConstMaxSameCorrelationNum		(15)
#define ConstCdcss1bitIntCycles				(59)/* 59  61 */
#define ConstCdcssTailerNum						(10)/* 11 */

#define CTCSS_DCS_NO_ACTIVE	(0)
#define CTCSS_ACTIVE (1)
#define DCS_ACTIVE (3)
#define DCS_ACTIVE_N (2)

//#define IDCS_ACTIVE  (3)
extern DH_BOOL fCtdcssValid;						/* 1:CTCSS/cdcss match */
extern const DH_U16 CtcssDeviationOnBuf[51];
extern uint8 CTCSS_ON_OFFSET_DATA;
//#define CTCSS_DCS_DECDOE_TIMER				(0X664) //(928)/* 125.9us  4M--503  3.58M--451   */
/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * VARIABLES
 */
extern DH_BOOL fCtdcssValid;
extern DH_BOOL fCtcssReverseBurst;
//extern uint8 AudioClosingTimeIncPer10ms;
extern const DH_U16 DcsCode_table_NormalADC[];
extern const DH_U16 CtcssValueTableADC[];

/*********************************************************************
 * FUNCTIONS
 */
void CTCSS_DCS_STOP(void);
uint8 GetCtcDcsDecodeStatus(void);
uint8 GetCtcDcsDecodeReverseStatus(void);
void start_ctcdcs_decode( uint8 cDecodeType, uint16 iDecode );
void ctcss_cdcss_decode_interrupt(void);
void Calculate_TIBIAVG(uint8 cCtcDcsType);
void TackleCtcssCdcssValidFlagFunc(void);
void ProcessCtcssDcsTail(void);
void cdcss_param_clear(void);
/*********************************************************************
*********************************************************************/
#endif/* CTCSS_DCS_DECODE_H */
#endif
