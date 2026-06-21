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
 ** \brief This sample demonstrates UART data receive and transfer by interrupt.
 **
 **   - 2018-11-27  1.0  Hongjh First version for Device Driver Library of USART
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/

#include "app_config.h"
#include "intf_usart.h"
#include "dh_type.h"
/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* USART channel definition */
#define USART_CH       (M4_USART1)

/* USART baudrate definition */
#define USART_BAUDRATE (115200ul)

/* USART RX Port/Pin definition */
#define USART_RX_PORT  (PortA)
#define USART_RX_PIN   (Pin12)
#define USART_RX_FUNC  (Func_Usart1_Rx)

/* USART TX Port/Pin definition */
#define USART_TX_PORT  (PortA)
#define USART_TX_PIN   (Pin11)
#define USART_TX_FUNC  (Func_Usart1_Tx)

/* USART interrupt number  */
#define USART_RI_NUM   (INT_USART1_RI)
#define USART_EI_NUM   (INT_USART1_EI)
#define USART_TI_NUM   (INT_USART1_TI)
#define USART_TCI_NUM  (INT_USART1_TCI)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void UsartRxIrqCallback(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
typedef struct
{
    DH_U8 *UARTTxbuf;
    DH_U8 *UARTRxbuf;
    DH_U16 Txbuf_Max;
    DH_U16 Rxbuf_Max;
    volatile DH_U16 Txbuf_i;
    volatile DH_U16 Txbuf_len;

    volatile DH_U16 Rxbuf_wi;
    volatile DH_U16 Rxbuf_ri;
    volatile DH_U16 Rxbuf_len;

    //	 OS_SEM sem_uarttx;
    //	 OS_SEM sem_uartrx;
} UART_DATA_STR_TDF;

#define UART1_RXBUF_MAX 2048
#define UART1_TXBUF_MAX 128
DH_U8 UART1_RxBuf[UART1_RXBUF_MAX];
DH_U8 UART1_TxBuf[UART1_TXBUF_MAX];
static UART_DATA_STR_TDF uart1_var = {
    UART1_TxBuf,
    UART1_RxBuf,
    UART1_TXBUF_MAX,
    UART1_RXBUF_MAX,
    0,
    0,
    0,
};

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 *******************************************************************************
 ** \brief USART RX irq callback function.
 **
 ** \param [in] None
 **
 ** \retval None
 **
 ******************************************************************************/
static void UsartRxIrqCallback(void)
{
    UART_DATA_STR_TDF *pData = &uart1_var;

    do {
        pData->UARTRxbuf[pData->Rxbuf_wi++] = USART_RecData(USART_CH);
        pData->Rxbuf_wi %= pData->Rxbuf_Max;
        pData->Rxbuf_len++;
        if (pData->Rxbuf_wi == pData->Rxbuf_ri)
        {
            pData->Rxbuf_ri++;
            pData->Rxbuf_ri %= pData->Rxbuf_Max;
            pData->Rxbuf_len = pData->Rxbuf_Max;
        }
    } while (0);

    //  m_u16RxData = USART_RecData(USART_CH);
    USART_FuncCmd(USART_CH, UsartTxAndTxEmptyInt, Enable);
}

/**
 *******************************************************************************
 ** \brief USART RX error irq callback function.
 **
 ** \param [in] None
 **
 ** \retval None
 **
 ******************************************************************************/
static void UsartErrIrqCallback(void)
{
    if (Set == USART_GetStatus(USART_CH, UsartFrameErr))
    {
        USART_ClearStatus(USART_CH, UsartFrameErr);
    }
    else
    {
    }

    if (Set == USART_GetStatus(USART_CH, UsartParityErr))
    {
        USART_ClearStatus(USART_CH, UsartParityErr);
    }
    else
    {
    }

    if (Set == USART_GetStatus(USART_CH, UsartOverrunErr))
    {
        USART_ClearStatus(USART_CH, UsartOverrunErr);
    }
    else
    {
    }
}

/**
 *******************************************************************************
 ** \brief USART TX irq callback function.
 **
 ** \param [in] None
 **
 ** \retval None
 **
 ******************************************************************************/

/**
 *******************************************************************************
 ** \brief USART TX complete irq callback function.
 **
 ** \param [in] None
 **
 ** \retval None
 **
 ******************************************************************************/

/**
 *******************************************************************************
 ** \brief  Main function of project
 **
 ** \param  None
 **
 ** \retval int32_t return value, if needed
 **
 ******************************************************************************/
int32_t USART_INIT(void)
{
    en_result_t enRet = Ok;
    stc_irq_regi_conf_t stcIrqRegiCfg;
    uint32_t u32Fcg1Periph = PWC_FCG1_PERIPH_USART1;
    const stc_usart_uart_init_t stcInitCfg = {
        UsartIntClkCkNoOutput,
        UsartClkDiv_1,
        UsartDataBits8,
        UsartDataLsbFirst,
        UsartOneStopBit,
        UsartParityNone,
        UsartSampleBit8,
        UsartStartBitFallEdge,
        UsartRtsEnable,
    };

    /* Enable peripheral clock */
    PWC_Fcg1PeriphClockCmd(u32Fcg1Periph, Enable);

    /* Initialize USART IO */
    PORT_SetFunc(USART_RX_PORT, USART_RX_PIN, USART_RX_FUNC, Disable);
    PORT_SetFunc(USART_TX_PORT, USART_TX_PIN, USART_TX_FUNC, Disable);

    /* Initialize UART */
    enRet = USART_UART_Init(USART_CH, &stcInitCfg);
    if (enRet != Ok)
    {
        while (1)
        {
        }
    }
    else
    {
    }

    /* Set baudrate */
    enRet = USART_SetBaudrate(USART_CH, USART_BAUDRATE);
    if (enRet != Ok)
    {
        while (1)
        {
        }
    }
    else
    {
    }
#if 0
    /*Enable RX && TX function*/
    USART_FuncCmd(USART_CH, UsartRx, Enable);
    USART_FuncCmd(USART_CH, UsartTx, Enable);
#else
    /* Set USART RX IRQ */
    stcIrqRegiCfg.enIRQn = Int000_IRQn;
    stcIrqRegiCfg.pfnCallback = &UsartRxIrqCallback;
    stcIrqRegiCfg.enIntSrc = USART_RI_NUM;
    enIrqRegistration(&stcIrqRegiCfg);
    NVIC_SetPriority(stcIrqRegiCfg.enIRQn, DDL_IRQ_PRIORITY_DEFAULT);
    NVIC_ClearPendingIRQ(stcIrqRegiCfg.enIRQn);
    NVIC_EnableIRQ(stcIrqRegiCfg.enIRQn);

    /* Set USART RX error IRQ */
    stcIrqRegiCfg.enIRQn = Int001_IRQn;
    stcIrqRegiCfg.pfnCallback = &UsartErrIrqCallback;
    stcIrqRegiCfg.enIntSrc = USART_EI_NUM;
    enIrqRegistration(&stcIrqRegiCfg);
    NVIC_SetPriority(stcIrqRegiCfg.enIRQn, DDL_IRQ_PRIORITY_DEFAULT);
    NVIC_ClearPendingIRQ(stcIrqRegiCfg.enIRQn);
    NVIC_EnableIRQ(stcIrqRegiCfg.enIRQn);

    /*Enable RX && RX interupt function*/
    USART_FuncCmd(USART_CH, UsartRx, Enable);
    USART_FuncCmd(USART_CH, UsartRxInt, Enable);
    USART_FuncCmd(USART_CH, UsartTx, Enable);
#endif

#if 0
		
		
		
		
    /* Set USART TX IRQ */
    stcIrqRegiCfg.enIRQn = Int002_IRQn;
    stcIrqRegiCfg.pfnCallback = &UsartTxIrqCallback;
    stcIrqRegiCfg.enIntSrc = USART_TI_NUM;
    enIrqRegistration(&stcIrqRegiCfg);
    NVIC_SetPriority(stcIrqRegiCfg.enIRQn, DDL_IRQ_PRIORITY_DEFAULT);
    NVIC_ClearPendingIRQ(stcIrqRegiCfg.enIRQn);
    NVIC_EnableIRQ(stcIrqRegiCfg.enIRQn);

    /* Set USART TX complete IRQ */
    stcIrqRegiCfg.enIRQn = Int003_IRQn;
    stcIrqRegiCfg.pfnCallback = &UsartTxCmpltIrqCallback;
    stcIrqRegiCfg.enIntSrc = USART_TCI_NUM;
    enIrqRegistration(&stcIrqRegiCfg);
    NVIC_SetPriority(stcIrqRegiCfg.enIRQn, DDL_IRQ_PRIORITY_DEFAULT);
    NVIC_ClearPendingIRQ(stcIrqRegiCfg.enIRQn);
    NVIC_EnableIRQ(stcIrqRegiCfg.enIRQn);
#endif
    usart_Send((const uint8_t *)"test.", 5);

    //   while (1)
    {
    }
    return (int32_t)enRet;
}

void COM_DeInit(void)
{
    USART_DeInit(USART_CH);

    /* Disable USART clock */
    PWC_Fcg1PeriphClockCmd(PWC_FCG1_PERIPH_USART1, Disable);

    /* Configure USART RX/TX pin */
    PORT_SetFunc(USART_RX_PORT, USART_RX_PIN, Func_Gpio, Disable);
    PORT_SetFunc(USART_TX_PORT, USART_TX_PIN, Func_Gpio, Disable);
}

void usart_Send(const uint8_t *buf, uint16_t len)
{
    while (len--)
    {
        while (Reset == USART_GetStatus(USART_CH, UsartTxEmpty)) /* Warit Tx data register empty */
        {
        }

        USART_SendData(USART_CH, *buf);
        buf++;
    }
}

DH_U8 usart_getFirstData(void)
{
    UART_DATA_STR_TDF *pUart = &uart1_var;
    if (pUart->Rxbuf_ri != pUart->Rxbuf_wi)
    {
        return pUart->UARTRxbuf[pUart->Rxbuf_ri];
    }
    return 0;
}

DH_U32 usart_Recv(DH_U8 *pu8Buf, DH_U32 u32Len, DH_U32 u32TimeOut)
{
#if 0
	 DH_U32 i = 0;   

    while (i < u32Len)
    {
        while ((Set == USART_GetStatus(USART_CH, UsartRxNoEmpty))  && (i < u32Len))
        {	
            pu8Buf[i++] =  USART_RecData(USART_CH);
        }
        
        if (i >= u32Len)
        {
            return i;
        }
        else
        {
          //  if(dhsem_wait(&pUart->sem_uartrx, u32TimeOut) != DH_OK())
          	if(u32TimeOut)
            {
               while( (Set != USART_GetStatus(USART_CH, UsartRxNoEmpty)) && u32TimeOut--) 
            	{
								Ddl_Delay1ms(1);
            	}
							if(u32TimeOut==0) break;
            }
						else
						{
						//	break;
						}
        }
    }
#else
    DH_U32 i = 0;
    UART_DATA_STR_TDF *pUart = &uart1_var;

    while (i < u32Len)
    {
        while (pUart->Rxbuf_ri != pUart->Rxbuf_wi && (i < u32Len))
        {
            pu8Buf[i++] = pUart->UARTRxbuf[pUart->Rxbuf_ri++];
            pUart->Rxbuf_ri %= pUart->Rxbuf_Max /*RXBUF_MAX*/;
            //    pUart->Rxbuf_len--;
        }

        if (i >= u32Len)
        {
            return i;
        }
        else
        {
            //  if(dhsem_wait(&pUart->sem_uartrx, u32TimeOut) != DH_OK())
            if (u32TimeOut)
            {
                while (pUart->Rxbuf_ri == pUart->Rxbuf_wi && u32TimeOut--)
                {
                    Ddl_Delay1ms(1);
                }
                if (u32TimeOut == 0)
                    break;
            }
            else
            {
                break;
            }
        }
    }
#endif
    return i;
}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
