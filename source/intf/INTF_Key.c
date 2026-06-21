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

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

volatile static uint8 g_CredFlg = CREDFLG_DIS;

/* uncomment this line if wants to print information to Terminal I/O window */
//#define  __PRINT_TO_TERMINAL

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

STR_KEY Str_Key = {
    .CurStat = 0,
    .LastStat = 0,
};
extern void (*PkeyHnd[2][ENM_Key_Max])();

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

#if 0
/**
 *******************************************************************************
 ** \brief ExtInt12 as key row 0 callback function
 **
 ** \param  None
 **
 ** \retval None
 **
 ******************************************************************************/
void KeyRow0_Callback(void)
{
    if (Set == EXINT_IrqFlgGet(ExtiCh12))
    {
    //    LED0_TOGGLE();

#ifdef __PRINT_TO_TERMINAL
        printk("Key row 0, col %d is pressed.\n", KEYSCAN_GetColIdx());
#endif
        /* clear int request flag */
        EXINT_IrqFlgClr(ExtiCh12);
    }
}

/**
 *******************************************************************************
 ** \brief ExtInt13 as key row 1 callback function
 **
 ** \param  None
 **
 ** \retval None
 **
 ******************************************************************************/
void KeyRow1_Callback(void)
{
    if (Set == EXINT_IrqFlgGet(ExtiCh13))
    {
       // LED1_TOGGLE();

#ifdef __PRINT_TO_TERMINAL
        printk("Key row 1, col %d is pressed.\n", KEYSCAN_GetColIdx());
#endif
        /* clear int request flag */
        EXINT_IrqFlgClr(ExtiCh13);
    }
}

/**
 *******************************************************************************
 ** \brief ExtInt14 as key row 2 callback function
 **
 ** \param  None
 **
 ** \retval None
 **
 ******************************************************************************/
void KeyRow2_Callback(void)
{
    if (Set == EXINT_IrqFlgGet(ExtiCh14))
    {
   //     LED2_TOGGLE();

#ifdef __PRINT_TO_TERMINAL
        printk("Key row 2, col %d is pressed.\n", KEYSCAN_GetColIdx());
#endif
        /* clear int request flag */
        EXINT_IrqFlgClr(ExtiCh14);
    }
}

/**
 *******************************************************************************
 ** \brief Key row 0 init function
 **
 ** \param  None
 **
 ** \retval None
 **
 ******************************************************************************/
void KeyRow0_Init(void)
{
    stc_exint_config_t stcExtiConfig;
    stc_irq_regi_conf_t stcIrqRegiConf;
    stc_port_init_t stcPortInit;

    /* configuration structure initialization */
    MEM_ZERO_STRUCT(stcExtiConfig);
    MEM_ZERO_STRUCT(stcIrqRegiConf);
    MEM_ZERO_STRUCT(stcPortInit);

    /**************************************************************************/
    /* External Int Ch.05 for keyin0                                          */
    /**************************************************************************/
    stcExtiConfig.enExitCh = ExtiCh05;

    /* Filter setting */
    stcExtiConfig.enFilterEn = Enable;
    stcExtiConfig.enFltClk = Pclk3Div8;
    /* falling edge for keyscan function */
    stcExtiConfig.enExtiLvl = ExIntFallingEdge;
    EXINT_Init(&stcExtiConfig);

    /* Set PD12 as External Int Ch.5 input */
    MEM_ZERO_STRUCT(stcPortInit);
    stcPortInit.enExInt = Enable;
    PORT_Init(KEYIN_PORT, KEYIN0_PIN, &stcPortInit);

    /* Select External Int Ch.5 */
    stcIrqRegiConf.enIntSrc = INT_PORT_EIRQ5;

    /* Register External Int to Vect.No.000 */
    stcIrqRegiConf.enIRQn = Int000_IRQn;

    /* Callback function */
    stcIrqRegiConf.pfnCallback = &KeyRow0_Callback;

    /* Registration IRQ */
    enIrqRegistration(&stcIrqRegiConf);

    /* Clear pending */
    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);

    /* Set priority */
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_15);

    /* Enable NVIC */
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);
}

/**
 *******************************************************************************
 ** \brief Key row 1 init function
 **
 ** \param  None
 **
 ** \retval None
 **
 ******************************************************************************/
void KeyRow1_Init(void)
{
    stc_exint_config_t stcExtiConfig;
    stc_irq_regi_conf_t stcIrqRegiConf;
    stc_port_init_t stcPortInit;

    /* configuration structure initialization */
    MEM_ZERO_STRUCT(stcExtiConfig);
    MEM_ZERO_STRUCT(stcIrqRegiConf);
    MEM_ZERO_STRUCT(stcPortInit);

    /**************************************************************************/
    /* External Int Ch.6 for keyin1                                          */
    /**************************************************************************/
    stcExtiConfig.enExitCh = ExtiCh06;

    /* Filter setting */
    stcExtiConfig.enFilterEn = Enable;
    stcExtiConfig.enFltClk = Pclk3Div8;
    /* falling edge for keyscan function */
    stcExtiConfig.enExtiLvl = ExIntFallingEdge;
    EXINT_Init(&stcExtiConfig);

    /* Set PD13 as External Int Ch.6 input */
    MEM_ZERO_STRUCT(stcPortInit);
    stcPortInit.enExInt = Enable;
    PORT_Init(KEYIN_PORT, KEYIN1_PIN, &stcPortInit);

    /* Select External Int Ch.6 */
    stcIrqRegiConf.enIntSrc = INT_PORT_EIRQ6;

    /* Register External Int to Vect.No.001 */
    stcIrqRegiConf.enIRQn = Int001_IRQn;

    /* Callback function */
    stcIrqRegiConf.pfnCallback = &KeyRow1_Callback;

    /* Registration IRQ */
    enIrqRegistration(&stcIrqRegiConf);

    /* Clear Pending */
    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);

    /* Set priority */
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_15);

    /* Enable NVIC */
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);
}

#ifdef KEYIN2_PIN
/**
 *******************************************************************************
 ** \brief Key row 2 init function
 **
 ** \param  None
 **
 ** \retval None
 **
 ******************************************************************************/
void KeyRow2_Init(void)
{
    stc_exint_config_t stcExtiConfig;
    stc_irq_regi_conf_t stcIrqRegiConf;
    stc_port_init_t stcPortInit;

    /* configuration structure initialization */
    MEM_ZERO_STRUCT(stcExtiConfig);
    MEM_ZERO_STRUCT(stcIrqRegiConf);
    MEM_ZERO_STRUCT(stcPortInit);

    /**************************************************************************/
    /* External Int Ch.14 for keyin2                                          */
    /**************************************************************************/
    stcExtiConfig.enExitCh = ExtiCh14;

    /* Filter setting */
    stcExtiConfig.enFilterEn = Enable;
    stcExtiConfig.enFltClk = Pclk3Div8;
    /* falling edge for keyscan function */
    stcExtiConfig.enExtiLvl = ExIntFallingEdge;
    EXINT_Init(&stcExtiConfig);

    /* Set PD14 as External Int Ch.14 input */
    MEM_ZERO_STRUCT(stcPortInit);
    stcPortInit.enExInt = Enable;
    PORT_Init(KEYIN_PORT, KEYIN2_PIN, &stcPortInit);

    /* Select External Int Ch.14 */
    stcIrqRegiConf.enIntSrc = INT_PORT_EIRQ14;

    /* Register External Int to Vect.No.002 */
    stcIrqRegiConf.enIRQn = Int002_IRQn;

    /* Callback function */
    stcIrqRegiConf.pfnCallback = &KeyRow2_Callback;

    /* Registration IRQ */
    enIrqRegistration(&stcIrqRegiConf);

    /* Clear Pending */
    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);

    /* Set priority */
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_15);

    /* Enable NVIC */
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);
}
#endif
/**
 *******************************************************************************
 ** \brief Key column init function
 **
 ** \param  None
 **
 ** \retval None
 **
 ******************************************************************************/
void KeyCol_Init(void)
{
    stc_port_init_t stcPortInit;
    stc_keyscan_config_t stcKeyscanConfig;

    /* configuration structure initialization */
    MEM_ZERO_STRUCT(stcPortInit);

    /* Enable internal pull-up */
    stcPortInit.enPullUp = Enable;
    PORT_Init(KEYOUT_PORT, (KEYOUT0_PIN ), &stcPortInit);

    /* Set corresponding pins to KEYSCAN function */
    PORT_SetFunc(KEYOUT_PORT, (KEYOUT0_PIN ), Func_Key, DISABLE);

    /* Enable KEYSCAN module source clock */
    PWC_Fcg0PeriphClockCmd(PWC_FCG0_PERIPH_KEY, Enable);

    /* output Hiz state 512 clocks*/
    stcKeyscanConfig.enHizCycle = Hiz512;

    /* output Low state 512 clocks */
    stcKeyscanConfig.enLowCycle = Low512;

    /* use HCLK as scan clock */
    stcKeyscanConfig.enKeyscanClk = KeyscanHclk;

    /* KEYOUT 0~3 are selected as column */
    stcKeyscanConfig.enKeyoutSel = Keyout0To1;

    /* KEYIN 5~6 are selected as row */
    stcKeyscanConfig.u16KeyinSel = (Keyin05 | Keyin06);

    KEYSCAN_Init(&stcKeyscanConfig);
}
/**
 *******************************************************************************
 ** \brief  main function for KEYSCAN function
 **
 ** \param  None
 **
 ** \return int32_t Return value, if needed
 **
 ******************************************************************************/
int32_t KEY_INTF_ScanInit(void)
 {
 
	//  Key_Gpio_init();


    /* Key row 0~2 init */
    KeyRow0_Init();
    KeyRow1_Init();
//    KeyRow2_Init();

    /* Key column 0~2 init */
    KeyCol_Init();
}

#else
void Key_Gpio_init(void)
{
    stc_port_init_t stcPortInit;

    /* configuration structure initialization */
    MEM_ZERO_STRUCT(stcPortInit);

    stcPortInit.enPinMode = Pin_Mode_In;
    stcPortInit.enExInt = Disable;
    stcPortInit.enPullUp = Enable;

    /*  Port/Pin initialization */
    PORT_Init(KEYPTT_PORT, KEYPTT_PIN, &stcPortInit);
    PORT_Init(KEYPTT_E_PORT, KEYPTT_E_PIN, &stcPortInit);

    /*  Port/Pin initialization */
    PORT_Init(KEYIN_PORT, KEYIN0_PIN, &stcPortInit);

    /*  Port/Pin initialization */
    PORT_Init(KEYIN_PORT, KEYIN1_PIN, &stcPortInit);

    stcPortInit.enPinMode = Pin_Mode_Out;

    /*  Port/Pin initialization */
    PORT_Init(KEYOUT_PORT, KEYOUT0_PIN, &stcPortInit);
    PORT_ResetBits(KEYOUT_PORT, KEYOUT0_PIN);
}

/**
 *******************************************************************************
 ** \brief  main function for KEYSCAN function
 **
 ** \param  None
 **
 ** \return int32_t Return value, if needed
 **
 ******************************************************************************/
void KEY_INTF_ScanInit(void)
{

    Key_Gpio_init();
}

uint8 ChkHasKeypadPress(void)
{
    if (KEY_PRESSING == PORT_GetBit(KEYPTT_PORT, KEYPTT_PIN))
    {
        return 1;
    }
    if (KEY_PRESSING == PORT_GetBit(KEYPTT_E_PORT, KEYPTT_E_PIN))
    {
        return 1;
    }

#if 1
    //uint8 j;
    //for(j=0;j<2;j++)
    {
        if (KEY_PRESSING == PORT_GetBit(KEYIN_PORT, KEYIN0_PIN))
        {
            return 1;
        }
        if (KEY_PRESSING == PORT_GetBit(KEYIN_PORT, KEYIN1_PIN))
        {
            return 1;
        }
    }
#endif
    return 0;
}

uint8 GetKeypad(void)
{
    uint8 Stat = 0;
#if 0
	uint8 i,j;
	for(j=0;j<4;j++)
	{
		if (KEY_PRESSING == GPIO_ReadInputDataBit((GPIO_TypeDef* )KEYPAD_INT_GPIOGRP[j], KEYPAD_INT_PINGRP[j]))
		{
			KeyChkLock();
			keypad_pn_setallhigh();
			if (KEY_PRESSING == GPIO_ReadInputDataBit((GPIO_TypeDef* )KEYPAD_INT_GPIOGRP[j], KEYPAD_INT_PINGRP[j]))
			{
				Stat =	keypad_arrP[j];
			}
			else
			{
				for(i = 0;i < KEY_ARRNUM;i++)
				{
					keypad_pn_setlow(i);//asm("NOP");asm("NOP");
					//Delay_100ns();
					if (KEY_PRESSING == GPIO_ReadInputDataBit((GPIO_TypeDef* )KEYPAD_INT_GPIOGRP[j], KEYPAD_INT_PINGRP[j]))
					{
						Stat =	keypad_arr[j][i];	break;
					}
					keypad_pn_sethigh(i);
				}
			}
			keypad_pn_setalllow();
			KeyChkUnlock();
		}
	}
#endif
    return Stat;
}

uint8 KeyStatChk(void)
{
    uint8 Stat = 0;
    //if(!AllFlag.SysStat_Poweron_OkFlag) return Stat;

    //	{
    //		Delay_ms(10);
    //		if  (Reset == PORT_GetBit(KEYPTT_PORT, KEYPTT_PIN))
    //		{
    //			if(usart_getFirstData()==0x68)
    //			{
    //				Stat=0;
    //			}
    //			else if(GloVarFlag.PcWriteReadFlg==0)
    //			{
    //				Stat = Key_Ptt;
    //			}
    //			else
    //			{
    //			//	DH_Drv_ExtiIntSet(EXTI_MODE_PTT, EXTI_DIS);
    //				Stat=0;
    //			}
    //		}
    //
    //		return Stat;
    //	}

    //	{
    //		Delay_ms(10);
    //		if  (Reset == PORT_GetBit(KEYPTT_E_PORT, KEYPTT_E_PIN))
    //		{
    //			if(GloVarFlag.PcWriteReadFlg==0)
    //			{
    //				Stat = Key_Ptt;
    //				//printk("PTTE_.");
    //			}
    //			else
    //			{
    //			//	DH_Drv_ExtiIntSet(EXTI_MODE_PTT, EXTI_DIS);
    //				Stat=0;
    //			}
    //		}
    //
    //		return Stat;
    //	}

    //
    //	if(GloVarFlag.KeyFlag==1)//

    //		Stat = 0;
    //		return Stat;
    //	}

    if (Reset == PORT_GetBit(KEYIN_PORT, KEYIN0_PIN))
    {

        Delay_ms(5);
        if (Reset == PORT_GetBit(KEYIN_PORT, KEYIN0_PIN))
        {

            PORT_SetBits(KEYOUT_PORT, KEYOUT0_PIN);
            if (Reset == PORT_GetBit(KEYIN_PORT, KEYIN0_PIN))
            {
                Stat = Key_P1;
            }
            else
            {
                //printk("key2.");
                Stat = Key_P2;
            }
            PORT_ResetBits(KEYOUT_PORT, KEYOUT0_PIN);
        }
    }

    if (Reset == PORT_GetBit(KEYIN_PORT, KEYIN1_PIN))
    {
        Delay_ms(5);
        if (Reset == PORT_GetBit(KEYIN_PORT, KEYIN1_PIN))
        {
            PORT_SetBits(KEYOUT_PORT, KEYOUT0_PIN);
            if (Reset == PORT_GetBit(KEYIN_PORT, KEYIN1_PIN))
            {
                //printk("key3.");
                Stat = Key_P3;
            }
            else
            {
                //printk("key4.");
                Stat = Key_P4;
            }
            PORT_ResetBits(KEYOUT_PORT, KEYOUT0_PIN);
        }
    }

    if (Str_Key.LastStat == Key_Ptt)
    {
        Stat = 0;
    }
    return Stat;
}

uint8 INTF_KeyStat(void)
{
    return Str_Key.CurStat;
}

void KEY_INTF_KeyScan(void)
{
    //	uint8 err;

    //DH_Drv_ExtiIntSet(EXTI_MODE_KEY, EXTI_EN);
    //DH_Drv_ExtiIntSet(EXTI_MODE_PTT, EXTI_EN);

    if (1)    //TaskHndGet(TASK_FLAG_KEYSCAN)
    {

        (void)KeyStatChk();
    }
}

typedef struct
{
    DH_U8 *buf;
    DH_U16 bufnum;
    DH_U16 buf_wi;
    DH_U16 buf_ri;
} Str_fifo_T;
static uint8 keygrp[5];
Str_fifo_T Str_Fifo_Key = {
    .buf = keygrp,
    .bufnum = sizeof(keygrp),
    .buf_ri = 0,
    .buf_wi = 0,
};

uint32 KeyGetOpt(uint32 TimeOut)
{
    uint32 Stat = 0;
#if 1
    if (Str_Fifo_Key.buf_ri != Str_Fifo_Key.buf_wi)
    {
        Stat = Str_Fifo_Key.buf[Str_Fifo_Key.buf_ri++];
        Str_Fifo_Key.buf_ri %= Str_Fifo_Key.bufnum;
    }
#else
    osEvent event;

    g_CredFlg = CREDFLG_EN;

    event = osMessageGet(OsMsg_KeyStatId, TimeOut);
    if (event.status == osEventMessage)
    {
        Stat = event.value.v;
    }
    else
    {
        Stat = 0;
    }
    //Stat = (uint32)OSQPend(Q_KeyStat, TimeOut, &err);

    g_CredFlg = CREDFLG_DIS;
#endif
    return Stat;
}

void KeyPutOpt(uint8 Flg, uint32 Stat)
{
#if 1
    if (KEY_CRED == Flg)
    {
        Str_Fifo_Key.buf[Str_Fifo_Key.buf_wi++] = Stat;
    }
    else
    {
        if (g_CredFlg == CREDFLG_EN)
        {
            Str_Fifo_Key.buf[Str_Fifo_Key.buf_wi++] = Stat;
        }
    }
    Str_Fifo_Key.buf_wi %= Str_Fifo_Key.bufnum;
#else

    UINT32 KeyStat = Stat;

    if (Stat < Key_Call)
    {
        if (FMRadioSeekFlag)
        {
            if (Stat == Key_Up || Stat == Key_Down)
            {
                rideoseek = Stat == Key_Up ? 1 : 0;
            }
            else
            {
                FMRadioSeekFlag = 0;
            }
            return;
        }
        GloVarFlag.bSoundPlayFlag = 0;
    }

    if (KEY_CRED == Flg)
    {
        //OSQPost(Q_KeyStat, KeyStat);
        osMessagePut(OsMsg_KeyStatId, KeyStat, 0);
    }
    else
    {
        if (g_CredFlg == CREDFLG_EN)
        {
            //OSQPost(Q_KeyStat, KeyStat);
            osMessagePut(OsMsg_KeyStatId, KeyStat, 0);
        }
    }

#endif
}

#endif

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
