#include "app_config.h"
#include "BLL_PC.h"
#include "app_menu.h"
#include "intf_usart.h"

#define PC_RX_MAX 400

DH_U8 PCRxBuf[PC_RX_MAX];

#if 0
OS_TCB TaskPCTCB;
CPU_STK TaskPCStk[TASKPC_STK_SIZE];
#endif
//DH_BOOL GloVarFlag.PcWriteReadFlg = FALSE;

void PC_BLL_PcMenuOperPro(DH_U8 *RecDataBuf);

DH_VOID PC_BLL_Init(DH_VOID)
{
    //OSTaskCreate(APP_UartTask,(DH_VOID *)0,&UartTaskStk[UARTTASK_STK_SIZE-1],UARTTASK_PRIO);
    //dhtask_create(0,USART_TASK,0);

    /* create a task queue */
    //uart_task_queue = _taskq_create(MQX_TASK_QUEUE_FIFO);
    //if (uart_task_queue == NULL) {
    //_task_block();
    //}
    //task_id_uart = _task_get_id_from_name("usart");
    //	OS_ERR err;

    //	OSMutexCreate(&mutex_RW, "rwMutex", &err);

    //	memset(TaskPCStk, 0x00, sizeof(CPU_STK) * TASKPC_STK_SIZE);
    //
    //	OSTaskCreate(&TaskPCTCB,\
//			"pc",\
//			(OS_TASK_PTR)PC_BLL_Proc,\
//			0, \
//			TASKPC_PRIO, \
//			&TaskPCStk[0],
    //			0, \
//			TASKPC_STK_SIZE, \
//			0, 0, 0, \
//			OS_OPT_TASK_NONE | OS_OPT_TASK_STK_CHK,\
//			&err);
}

DH_VOID Before_PC_Readwrite(DH_VOID)
{
    //	OS_ERR err;

    //	GloVarFlag.PcWriteReadFlg = TRUE;

    //	GPIO_DRV_IntDisable(MAP_PTT_PIN);

    //	GPIO_DRV_IntDisable(RDA_CDCSS_PIN);
}

DH_VOID PC_BLL_ReSet(DH_VOID)
{
    //	OS_ERR err;

    //	GloVarFlag.PcWriteReadFlg = FALSE;
}

DH_VOID PC_BLL_Proc(void *p_arg)
{
    DH_U32 RxLen;
    DH_U32 RxLen_i;

    //while (1)
    {
        RxLen_i = PCLINK_INTF_ReceiveFrame(PCRxBuf, 1, 0);
        if (RxLen_i == 1)
        {
            if (PCRxBuf[UART_DATAMODE_HEAD] == 0x68)
            {
                Before_PC_Readwrite();

                RxLen = RxLen_i + PCLINK_INTF_ReceiveFrame(PCRxBuf + 1, UART_RX_FIRST_I - 1, 100);

                if (RxLen >= UART_RX_FIRST_I)
                {
                    if (PCRxBuf[UART_DATAMODE_MODE] == UART_MODE_PLLSET)
                    {
                        //PLLRecData(UartAppRxBuf, RxLen);
                    }
                    else if (PCRxBuf[UART_DATAMODE_MODE] == UART_TUNER_TEST)
                    {
                        //printk("PC_BLL_PcTunerRecData.");
                        //	PC_BLL_PcTunerRecData(PCRxBuf, RxLen);
                    }
                    /*else if (PCRxBuf[UART_DATAMODE_MODE] == PCCMDTYPE)
					{
						PC_BLL_RecPcData(PCRxBuf, RxLen);
					}*/
                    else if (PCRxBuf[UART_DATAMODE_MODE] == UART_MODE_UIIMG)
                    {
                        //	PC_BLL_PCIAP_ImgRecData(PCRxBuf, RxLen);
                    }
                    else if (PCRxBuf[UART_DATAMODE_MODE] == UART_MODE_READRCDLIST)
                    {
                        //	PC_BLL_PCIAP_RcdInfoSend(PCRxBuf, RxLen);
                    }
                    else if (PCRxBuf[UART_DATAMODE_MODE] == UART_MODE_READMSGCONT)
                    {
                        //	PC_BLL_PCIAP_MsgContSend(PCRxBuf, RxLen);
                    }
                    else if (PCRxBuf[UART_DATAMODE_MODE] == UART_MODE_CMX138SET)
                    {
                        //	CMX138RecData(PCRxBuf, RxLen);
                    }
                    else
                    {
                        /*PC_BLL_PcCmdPro(PCRxBuf, RxLen);*/
                    }
                }
                PC_BLL_ReSet();
            }
            else if ((PCRxBuf[UART_DATAMODE_HEAD] == 'F'))    ////flash read write
            {
                Before_PC_Readwrite();
                RxLen = RxLen_i;

                RxLen += PCLINK_INTF_ReceiveFrame(PCRxBuf + RxLen_i,
                                                  UART_RECV_HEAD_LEN + 2 * UART_RECV_PWD_LEN - RxLen_i, 100);

                if (RxLen >= UART_RECV_HEAD_LEN)
                {
                    printk("BLL_IAP_Flash.");
                    //BLL_IAP_Flash(PCRxBuf);
                }
                /*else
				{
					if(RxLen>13)RxLen=13;
					PCRxBuf[RxLen] = 0;
					static DH_U8 lt=0;
					if(lt)
					{lt = 0;
						LCD_BLL_printf_ext(23,0,lcd_b,"pcget=%s .",PCRxBuf);
						LCD_BLL_printf_ext(43,0,lcd_b,"pcget=            ");
					}else{lt = 1;
						LCD_BLL_printf_ext(43,0,lcd_b,"pcget=%s .",PCRxBuf);
						LCD_BLL_printf_ext(23,0,lcd_b,"pcget=            ");
					}
				}*/
                PC_BLL_ReSet();
            }
            else
            {
            }
        }
    }
}

void PC_BLL_PcMenuOperPro(DH_U8 *RecDataBuf)
{
    //VOICEIC_INTF_PlayNum(10*(RecDataBuf[1]-'0')+RecDataBuf[2]-'0');

    //	PCLINK_INTF_TransmitFrame(RecDataBuf,3);
    //	switch(RecDataBuf[1])
    //	case '0':
    //		KEY_INTF_PutKeyAssignment(KEY_CRED, Key_0);
    //		break;
    //	case '1':
    //		KEY_INTF_PutKeyAssignment(KEY_CRED, Key_1);
    //		break;
    //	case '2':
    //		KEY_INTF_PutKeyAssignment(KEY_CRED, Key_2);
    //		break;
    //	case '3':
    //		KEY_INTF_PutKeyAssignment(KEY_CRED, Key_3);
    //		break;
    //	case '4':
    //		KEY_INTF_PutKeyAssignment(KEY_CRED, Key_4);
    //		break;
    //	case '5':
    //		KEY_INTF_PutKeyAssignment(KEY_CRED, Key_5);
    //		break;
    //	case '6':
    //		KEY_INTF_PutKeyAssignment(KEY_CRED, Key_6);
    //		break;
    //	case '7':
    //		KEY_INTF_PutKeyAssignment(KEY_CRED, Key_7);
    //		break;
    //	case '8':
    //		KEY_INTF_PutKeyAssignment(KEY_CRED, Key_8);
    //		break;
    //	case '9':
    //		KEY_INTF_PutKeyAssignment(KEY_CRED, Key_9);
    //		break;
    //	case 'O':
    //		KEY_INTF_PutKeyAssignment(KEY_CRED, Key_Ok);
    //		break;
    ////	case 'U':
    ////		KEY_INTF_PutKeyAssignment(KEY_CRED, Key_Up);
    ////		break;
    //	case 'L':
    //		KEY_INTF_PutKeyAssignment(KEY_CRED, Key_Left);
    //		break;
    //	case 'R':
    //		KEY_INTF_PutKeyAssignment(KEY_CRED, Key_Right);
    //		break;
    //	case 'U':
    //		KEY_INTF_PutKeyAssignment(KEY_CRED, Key_Up);
    //		break;
    //	case 'D':
    //		KEY_INTF_PutKeyAssignment(KEY_CRED, Key_Down);
    //		break;
    //	case 'B':
    //		KEY_INTF_PutKeyAssignment(KEY_CRED, Key_Back);///
    //		break;
    //	case 'M':
    //		KEY_INTF_PutKeyAssignment(KEY_CRED, Key_B);///inputmethod mode
    //		break;
    //#if USE_STOKE_KEY
    //	case 'T':///like tab
    //		KEY_INTF_PutKeyAssignment(KEY_CRED, Key_Stoke2);///
    //		break;
    //	case 'S':///like tab
    //		KEY_INTF_PutKeyAssignment(KEY_CRED, Key_Stoke1);///
    //		break;
    //	case 'I':///jump to iap
    //	//	*b = TRUE;///
    //		break;
    //#endif
    //	default:break;
    //	}
    //	RecDataBuf[2] = 0;
    //	printk("KeyMnCmd:%s\r\n",RecDataBuf);
}

void PC_BLL_ReadToPC(DH_U8 command, DH_U8 buf[], DH_U16 len)
{
    buf[Phead] = 0x68;
    buf[Pcmd] = command;
    buf[Popmode] = 0x00;
    buf[Popack] = 0x00;
    buf[Plen] = len >> 8;
    buf[Plen + 1] = len;
    buf[Phead + 8 + len] = 0x10;
    PC_BLL_MakeCkSum(buf, 9 + len);
    PCLINK_INTF_TransmitFrame(buf, 9 + len);
}

void PC_BLL_ReadCPUID(DH_U16 len, DH_U8 buf[])
{
#if 0
	dword cpuid[4];
	//if((CheckCkSum(len/*+ACKlen*/, buf))||((buf[Pcksum]||buf[Pcksum+1])==0))
	{

		cpuid[0] = SIM_UIDH;
		cpuid[1] = SIM_UIDMH;
		cpuid[2] = SIM_UIDML;
		cpuid[3] = SIM_UIDL;

		buf[8] = (cpuid[0] >> 24) & 0xff;
		buf[9] = (cpuid[0] >> 16) & 0xff;
		buf[10] = (cpuid[0] >> 8) & 0xff;
		buf[11] = cpuid[0] & 0xff;
		buf[12] = (cpuid[1] >> 24) & 0xff;
		buf[13] = (cpuid[1] >> 16) & 0xff;
		buf[14] = (cpuid[1] >> 8) & 0xff;
		buf[15] = cpuid[1] & 0xff;
		buf[16] = (cpuid[2] >> 24) & 0xff;
		buf[17] = (cpuid[2] >> 16) & 0xff;
		buf[18] = (cpuid[2] >> 8) & 0xff;
		buf[19] = cpuid[2] & 0xff;
		buf[20] = (cpuid[3] >> 24) & 0xff;
		buf[21] = (cpuid[3] >> 16) & 0xff;
		buf[22] = (cpuid[3] >> 8) & 0xff;
		buf[23] = cpuid[3] & 0xff;

		ReadToPC(1,buf,16);
	}
	//else
	{
		//AckToPC(buf[Pcmd],ChkError,Ackbuf,pc_read);
	}

#endif
}

void PC_BLL_SetVocoderKey(DH_U16 len, DH_U8 buf[])
{
    DH_U8 lu8buf[33];
    //	DH_U8 lau8key[32];
    DH_U8 lu8rxlen;
    lu8rxlen = PCLINK_INTF_ReceiveFrame(lu8buf, 33, UART_RX_CON_WAITT);
    if (lu8rxlen >= 33)
    {
        if (0x10 == lu8buf[32])
        {
#if 1

//            FLASH_INTF_WriteStr (PCBA_ENCKEYID, (DH_U8*)lu8buf, 32);
#else

            _lpm_set_operation_mode(LPM_OPERATION_MODE_RUN);
            _int_disable();

            IFR_INTF_ReadKey(lau8key, 32);

            if (memcmp(lau8key, lu8buf, 32) != 0)
            {
                IFR_INTF_WriteKey(lu8buf, 32);
            }

            _int_enable();
            _lpm_set_operation_mode(LPM_OPERATION_MODE_HSRUN);
#endif

            PC_BLL_ReadToPC(3, lu8buf, 0);
        }
    }
}

DH_U16 PC_BLL_PcCheckSum(DH_U8 *buf, DH_S16 len)
{
    DH_U32 sum = 0;

    while (len > 1)
    {
        sum += 0xFFFF & (*buf << 8 | *(buf + 1));
        buf += 2;
        len -= 2;
    }

    if (len)
    {
        sum += (0xFF & *buf) << 8;
    }

    while (sum >> 16)
    {
        sum = (sum & 0xFFFF) + (sum >> 16);
    }

    return ((DH_U16)sum ^ 0xFFFF);
}

void PC_BLL_MakeCkSum(DH_U8 buf[], DH_U16 len)
{
    DH_U16 sum = 0;
    buf[Pcksum] = 0;
    buf[Pcksum + 1] = 0;
    sum = PC_BLL_PcCheckSum(buf, len);
    buf[Pcksum] = ((sum >> 8) & 0xff);
    buf[Pcksum + 1] = (sum & 0xff);
}

DH_U8 PC_BLL_CheckCkSum(DH_U8 buf[], DH_U16 len)
{
    DH_U16 sum, cksum;
    sum = buf[Pcksum];
    buf[Pcksum] = 0;
    sum = (sum << 8);
    sum = sum + buf[Pcksum + 1];
    buf[Pcksum + 1] = 0;
    cksum = PC_BLL_PcCheckSum(buf, len);
    buf[Pcksum + 1] = (sum & 0xff);
    buf[Pcksum] = (sum >> 8) & 0xff;
    if (sum == cksum)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void PC_BLL_MakeSDSend(uint16 len, uint8 ShortData[], uint8 ipflag)
{
    uint16 i;
    uint16 cksum;
    static uint8 UARTSendBUF[340];

    for (i = len; i > 0; i--)
    {
        UARTSendBUF[i + 10 - 1] = ShortData[i - 1];
    }

    UARTSendBUF[Phead] = 0x68;
    UARTSendBUF[Pcmd] = 0x0d;
    UARTSendBUF[Popmode] = 0x00;
    UARTSendBUF[Popack] = 3;
    UARTSendBUF[Plen] = (((len + 2) >> 8) & 0xff);
    UARTSendBUF[Plen + 1] = ((len + 2) & 0xff);
    UARTSendBUF[Pcksum] = 0;    //cheksum
    UARTSendBUF[Pcksum + 1] = 0;
    UARTSendBUF[Pqu] = (1 << 3);
    UARTSendBUF[Ipdata] = (ipflag << 4);
    if (ipflag)
    {
        UARTSendBUF[Ipdata] |= (1 << 3);
    }
    UARTSendBUF[Pdata + len + 2] = 0x10;
    cksum = PC_BLL_PcCheckSum(UARTSendBUF, len + 11);
    UARTSendBUF[Pcksum] = ((cksum >> 8) & 0xff);    //cheksum
    UARTSendBUF[Pcksum + 1] = (cksum & 0xff);
    PCLINK_INTF_TransmitFrame(UARTSendBUF, len + 11);
}
