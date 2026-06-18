/*****************************************************************************
 *   文件名:sleepmode.c
 *   文件描述:睡眠模式模块的函数集
 *   创建人：陶林，2010/08/17
******************************************************************************/

#include "app_config.h"
#include "include.h"

#define DEF_INTFSLEEP 	0//睡眠的使能位

// Cortex System Control register address
#define SCB_SysCtrl              ((u32)0xE000ED10)
// SLEEPDEEP bit mask
#define SysCtrl_SLEEPDEEP_Set    ((u32)0x00000004)

/*****************************************************************************
 *   函数名: INTF_SleepModeInit
 *   功能描述: 低功耗模式初始化
 *   参数定义:	
 *	返回值:
 *   创建人：陶林，2012/01/13
******************************************************************************/
void INTF_SleepModeInit(void)
{

}

/*****************************************************************************
 *   函数名: INTF_EnterSleepMode
 *   功能描述: 进入低功耗模式
 *   参数定义:	
 *	返回值:
 *   创建人：陶林，2012/01/13
******************************************************************************/
void INTF_EnterSleepMode(void)
{
#if DEF_INTFSLEEP
	__WFI();
#endif
}

/*****************************************************************************
 *   函数名: INTF_EnterDsleepMode
 *   功能描述: 进入深度低功耗模式
 *   参数定义:	
 *	返回值:
 *   创建人：陶林，2012/01/13
******************************************************************************/
void INTF_EnterDsleepMode(void)
{
#if DEF_INTFSLEEP
	//INTF_LcdLightSet(FALSE);
	/* Request to enter STOP mode with regulator in low power mode*/
    PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
#endif
}

/*****************************************************************************
 *   函数名: INTF_ExitDSlpMode
 *   功能描述: 退出深度低功耗模式
 *   参数定义:	
 *	返回值:
 *   创建人：陶林，2012/01/13
******************************************************************************/
void INTF_ExitDSlpMode(void)
{
#if DEF_INTFSLEEP

    // Clear SLEEPDEEP bit of Cortex System Control Register
    *(vu32 *)  SCB_SysCtrl &= ~SysCtrl_SLEEPDEEP_Set; 

	/* Configures system clock after wake-up from STOP: enable HSE, PLL and select 
       PLL as system clock source (HSE and PLL are disabled in STOP mode) */
    //SYSCLKConfig_STOP();
	
#endif
}

/*******************************************************************************
* Function Name  : SYSCLKConfig_STOP
* Description    : Configures system clock after wake-up from STOP: enable HSE, PLL
*                  and select PLL as system clock source.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SYSCLKConfig_STOP(void)
{
#if DEF_INTFSLEEP
	ErrorStatus HSEStartUpStatus;
	
  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if(HSEStartUpStatus == SUCCESS)
  {
    /* Enable PLL */ 
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while(RCC_GetSYSCLKSource() != 0x08)
    {
    }
  }
  #endif
}

