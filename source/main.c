
#include "app_config.h"
#include "IAP.h"
#include "include.h"

static uint32_t JumpAddr;
static func_ptr_t JumpToApp;

uint8 buff[1024];

DH_U32 g_SysTaskFlag = 0;

void TaskHndStart(u32 Flg)
{
    g_SysTaskFlag |= Flg;
}

void TaskHndStop(u32 Flg)
{
    g_SysTaskFlag &= ~Flg;
}

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
    SysTick->CTRL = 0UL;
    SysTick->LOAD = 0UL;
    SysTick->VAL = 0UL;
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
    if ((u32StackTop > SRAM_BASE) && (u32StackTop <= (SRAM_BASE + SRAM_SIZE)))
    {
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
    if ((APP_UPGRADE_FLAG != *(__IO uint32_t *)APP_UPGRADE_FLAG_ADDR))
    {
        if ((APP_EXIST_FLAG == *(__IO uint32_t *)APP_EXIST_FLAG_ADDR))
        {
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
    if (KeyStatChk() == Key_P4)    //!IAP_CheckApp() ||
    {
        SystemClockConfig(); /* Configuring a new system clock if you need. */

        USART_INIT(); /* Config UART for printing. Baud rate 115200. */

        LED_INTF_Init();
        RED_LED_ON;
        AT24Cxx_Init();
        /* Control Menu */
        printk("\r\n============= Bootloader Menu =============\r\n");
        printk(" 1: Download execute program to the Flash \r\n");
        printk(" 2: Upload execute program from the Flash \r\n");
        printk(" 3: Jump to the application \r\n");

        for (;;)
        {
            RED_LED_ON;
            if (1 == PCLINK_INTF_ReceiveFrame(au8FlashBuf, 1, 0))
            {
                if (au8FlashBuf[0] == 0x68)
                {
                }
                else if ((au8FlashBuf[0] == 'F' || au8FlashBuf[0] == 'E'))    ////flash read write
                {
                    reclen = PCLINK_INTF_ReceiveFrame(au8FlashBuf + 1, UART_RECV_HEAD_LEN - 1, 200);
                    if (reclen >= UART_RECV_HEAD_LEN - 1)
                    {
                        if (BLL_IAP_Flash(au8FlashBuf))
                            ;
                    }
                }
                else
                {
                }
            }
        }
    }
    if (LL_OK != IAP_JumpToApp(IAP_APP_ADDR))
    {
        printk("\r\nJump to app failed \r\n");
    }
}
