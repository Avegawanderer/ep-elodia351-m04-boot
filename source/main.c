/*******************************************************************************
 * Copyright (C) 2016, Huada Semiconductor Co.,Ltd All rights reserved.
 *
 * This software is owned and published by:
 * Huada Semiconductor Co.,Ltd ("HDSC").
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
 ** \brief The example for KEYSCAN function demonstration
 **
 **   - 2018-10-24  1.0  Zhangxl First version for sample of keyscan module.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/

#include "app_config.h"
#include "include.h"
#include "IAP.h"

static uint32_t JumpAddr;
static func_ptr_t JumpToApp;

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/
uint8 buff[1024];


DH_U32 g_SysTaskFlag = 0; 

/*****************************************************************************
 *   函数名: TaskHndStart
 * 开启目标任务的执行,进程,
******************************************************************************/
void TaskHndStart(u32 Flg)
{
	g_SysTaskFlag |= Flg;	

}
/*****************************************************************************
 *   函数名: TaskHndStop
 * 关闭目标任务的执行进程.
******************************************************************************/
void TaskHndStop(u32 Flg)
{
	g_SysTaskFlag &= ~Flg;	
}

/*****************************************************************************
 *   函数名: TaskHndGet()
 * 开启目标任务的执行,进程,
******************************************************************************/
u32 TaskHndGet(u32 Flg)
{
	return (g_SysTaskFlag & Flg);	
}

/**
 * @brief  Systick De-Initialize.
 * @param  None
 * @retval None
 */
void SysTick_DeInit(void)
{
    SysTick->CTRL  = 0UL;
    SysTick->LOAD  = 0UL;
    SysTick->VAL   = 0UL;
}
/**
 * @brief  IAP clock De-Initialize.
 * @param  None
 * @retval None
 */
void IAP_CLK_DeInit(void)
{
//	CLK_SetSysClockSrc(CLK_SYSCLK_SRC_MRC);
//	/* Switch driver ability */
//	(void)PWC_HighPerformanceToHighSpeed();
//	/* Set bus clk div. */
//	CLK_SetClockDiv(CLK_BUS_CLK_ALL, (CLK_HCLK_DIV1 | CLK_EXCLK_DIV1 | CLK_PCLK0_DIV1 |
//																		CLK_PCLK1_DIV1 | CLK_PCLK2_DIV1 | CLK_PCLK3_DIV1 | CLK_PCLK4_DIV1));
//	CLK_PLLCmd(DISABLE);
//	CLK_XtalCmd(DISABLE);
//	/* sram init include read/write wait cycle setting */
//	SRAM_SetWaitCycle(SRAM_SRAM_ALL, SRAM_WAIT_CYCLE0, SRAM_WAIT_CYCLE0);
//	SRAM_SetWaitCycle(SRAM_SRAMH, SRAM_WAIT_CYCLE0, SRAM_WAIT_CYCLE0);
//	/* 0 cycles */
//	GPIO_SetReadWaitCycle(GPIO_RD_WAIT0);
//	/* flash read wait cycle setting */
//	EFM_SetWaitCycle(EFM_WAIT_CYCLE0);
}


/**
 * @brief  IAP peripheral de-initialize.
 * @param  None
 * @retval None
 */
void IAP_PeriphDeinit(void)
{
    /* De-Init Peripheral */
		COM_DeInit();
//		GPIO_DeInit();
		SysTick_DeInit();
		IAP_CLK_DeInit();
		/* Peripheral registers write protected */
	//	EFM_FWMC_Cmd(DISABLE);
	//	LL_PERIPH_WP(EXAMPLE_PERIPH_WP);	
}

/**
 * @brief  Jump from boot to app function.
 * @param  [in] u32Addr                 APP address
 * @retval LL_ERR                       APP address error
 */
int32_t IAP_JumpToApp(uint32_t u32Addr)
{
    uint32_t u32StackTop = *((__IO uint32_t *)u32Addr);

    /* Check stack top pointer. */
    if ((u32StackTop > SRAM_BASE) && (u32StackTop <= (SRAM_BASE + SRAM_SIZE))) {
        IAP_PeriphDeinit();
        /* Jump to user application */
        JumpAddr = *(__IO uint32_t *)(u32Addr + 4U);
        JumpToApp = (func_ptr_t)JumpAddr;
        /* Initialize user application's Stack Pointer */
        __set_MSP(u32StackTop);
        JumpToApp();
    }

    return LL_ERR;
}

/**
 * @brief  IAP check app.
 * @param  None
 * @retval None
 */
uint8 IAP_CheckApp(void)
{
    if ((APP_UPGRADE_FLAG != *(__IO uint32_t *)APP_UPGRADE_FLAG_ADDR)) {
        if ((APP_EXIST_FLAG == *(__IO uint32_t *)APP_EXIST_FLAG_ADDR)) {
          return 1;
        }
    }
	return 0;
}

uint8_t au8FlashBuf[8192];
/**
 *******************************************************************************
 ** \brief  Main function.
 **
 ** \param  None.
 **
 ** \retval int32_t return value, if needed.
 **
 ******************************************************************************/
int32_t main(void)
{ 
	uint8 reclen;

	KEY_INTF_ScanInit();

	/* Check app validity */
	if( KeyStatChk()==Key_P4)//!IAP_CheckApp() ||
	{
		SystemClockConfig(); /* Configuring a new system clock if you need. */
		
		USART_INIT(); /* Config UART for printing. Baud rate 115200. */

		LED_INTF_Init();
RED_LED_ON;
		AT24Cxx_Init();
		/* Control Menu */
		printk((uint8_t *)"\r\n============= Bootloader Menu =============\r\n");
		printk((uint8_t *)" 1: Download execute program to the Flash \r\n");
		printk((uint8_t *)" 2: Upload execute program from the Flash \r\n");
		printk((uint8_t *)" 3: Jump to the application \r\n");
			
		for (;;) {
      RED_LED_ON;
				if (1 == PCLINK_INTF_ReceiveFrame(au8FlashBuf, 1, 0)) 
				{
					if ( au8FlashBuf[0] == 0x68) // 接收到数据 ////c5000 read write
					{
						
					}
					else if ((au8FlashBuf[0] == 'F'||au8FlashBuf[0] == 'E')) ////flash read write
					{
						reclen =  PCLINK_INTF_ReceiveFrame(au8FlashBuf + 1,	UART_RECV_HEAD_LEN - 1, 200);
						if (reclen >=UART_RECV_HEAD_LEN - 1)				
						{
							if(BLL_IAP_Flash(au8FlashBuf))	;
						}
					}
					else
					{

					}
				}
		}
	}	
	if (LL_OK != IAP_JumpToApp(IAP_APP_ADDR)) {
		printk((uint8_t *)"\r\nJump to app failed \r\n");
	}
}


/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
