
#ifndef _INTF_ADC_H
#define	_INTF_ADC_H

enum{
	E_ADC_CHN_VOLATGE,
	E_ADC_CHN_KEY1,
	E_ADC_CHN_VOX_SET,
	E_ADC_CHN_RSSI,
	E_ADC_CHN_SQ_SET,
	E_ADC_CHN_KEY2
		

};



/**
 *******************************************************************************
 ** \brief  ADC configuration, including clock configuration, initial configuration,
 **         channel configuration and trigger source configuration.
 **
 ** \param  None.
 **
 ** \retval None.
 **
 ******************************************************************************/
void ADC_INTF_Config(void);


void ADC_INTF_Get(void);


uint16 ADC_INTF_GetValue(uint8 chn);



/**
 *******************************************************************************
 ** \brief  Timer configuration, for generating event EVT_TMR02_GCMA every second.
 **
 ** \param  None.
 **
 ** \retval None.
 **
 ******************************************************************************/
void Timer0Config(void);


#endif
/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
