/******************************************************************************  
 Copyright (C), 2014-2020, ZheJiang Dahua Technology CO.,LTD.
 ******************************************************************************
 File Name     : dh_drv_adc.c
 Version       : Initial Draft
 Author        : yu_feng1<yu_feng1@dahuatech.com>
 Created       : 2014.3.17
 Last Modified :
 Description   : define the Mcu of AD code
 Function List :
 DHstory       :
 1.Date        : 2014/3/17
 Author      : 24538
 Modification: Create
 ******************************************************************************/

#include "app_config.h"

#include "BLL_PC.h"
#include "app_menu.h"

/* uart 接收缓冲区 */
#define PC_RX_MAX 400

DH_U8 PCRxBuf[PC_RX_MAX];

#if 0
OS_TCB TaskPCTCB;
CPU_STK TaskPCStk[TASKPC_STK_SIZE];
#endif
//DH_BOOL GloVarFlag.PcWriteReadFlg = FALSE;




void PC_BLL_PcMenuOperPro(DH_U8* RecDataBuf);

/**************************************************************************
 * 函数名: PC_BLL_Init
 * 描  述: PC数据业务层的初始化
 * 输  入: 无
 * 输  出: 无
 * 返回值: 无
 **************************************************************************/
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
//	SLP_BLL_SlpModeExitSM(1);/*退出睡眠模式*/

//	GloVarFlag.PcWriteReadFlg = TRUE;

//	GPIO_DRV_IntDisable(MAP_PTT_PIN);

//	GPIO_DRV_IntDisable(RDA_CDCSS_PIN);

//	MSET_APP_LockKpKeyClear();/*清除锁键*/

//	GPS_INTF_PauseRec();    //暂停gps接收

//	if(SCAN_APP_ScanCtrlGet()==SCAN_ON)	SCAN_APP_ScanCtrlSet(SCAN_SUSPEND);/*关闭扫描*/

}

DH_VOID PC_BLL_ReSet(DH_VOID)
{
//	OS_ERR err;

//	GloVarFlag.PcWriteReadFlg = FALSE;

}

/**************************************************************************
 * 函数名: PC_BLL_Proc
 * 描  述: 上位机通信数据的处理任务
 * 输  入: 任务形参，无具体应用
 * 输  出: 无
 * 返回值: 无
 **************************************************************************/
DH_VOID PC_BLL_Proc(void *p_arg)
{
	DH_U32 RxLen;
	DH_U32 RxLen_i;

	//while (1)
	{
		RxLen_i = PCLINK_INTF_ReceiveFrame(PCRxBuf, 1, 0);
		if (RxLen_i == 1)
		{
			if ( PCRxBuf[UART_DATAMODE_HEAD] == 0x68) // 接收到数据 ////c5000 read write
			{
				Before_PC_Readwrite();


				RxLen = RxLen_i + PCLINK_INTF_ReceiveFrame(PCRxBuf + 1, UART_RX_FIRST_I - 1,100);

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
					else if (PCRxBuf[UART_DATAMODE_MODE] == UART_MODE_READRCDLIST)/*读录音信息*/
					{
					//	PC_BLL_PCIAP_RcdInfoSend(PCRxBuf, RxLen);
					}
					else if (PCRxBuf[UART_DATAMODE_MODE] == UART_MODE_READMSGCONT)/*读msg内容*/
					{
					//	PC_BLL_PCIAP_MsgContSend(PCRxBuf, RxLen);
					}
					else if(PCRxBuf[UART_DATAMODE_MODE]==UART_MODE_CMX138SET)
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
			else if ((PCRxBuf[UART_DATAMODE_HEAD] == 'F')) ////flash read write
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

/**************************************************************************
 * 函数名: PC_BLL_PcMenuOperPro
 * 描  述: 接收到上位机命令的解析处理
 * 输  入: *RecDataBuf:接收到的数据内容，HeadLen: 数据长度
 * 输  出: 无
 * 返回值: 无
 **************************************************************************/
void PC_BLL_PcMenuOperPro(DH_U8* RecDataBuf)
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

/**************************************************************************
 * 函数名: PC_BLL_ReadToPC
 * 描  述: 组返回给PC的应答帧
 * 输  入: command:帧命令，buf[]:接收的帧内容地址，len:帧长度
 * 输  出: 无
 * 返回值: 无
 **************************************************************************/
void PC_BLL_ReadToPC(DH_U8 command, DH_U8 buf[], DH_U16 len)
{
	buf[Phead] = 0x68;
	buf[Pcmd] = command;
	buf[Popmode] = 0x00;
	buf[Popack] = 0x00;
	buf[Plen] = len >> 8;		//数据位长度
	buf[Plen + 1] = len;
	buf[Phead + 8 + len] = 0x10;
	PC_BLL_MakeCkSum(buf, 9 + len);
	PCLINK_INTF_TransmitFrame(buf, 9 + len);
}

/*******************************************************************************
 * 函数名  : PC_BLL_ReadCPUID
 * 描  述  : 读取CPU的ID值
 * 输  入  : len:接收帧数据的长度，buf[]:数据内容数组地址
 * 输  出  : 无
 * 返回值  : 无
 *******************************************************************************/
void PC_BLL_ReadCPUID(DH_U16 len, DH_U8 buf[])
{
#if 0
	dword cpuid[4];
	//if((CheckCkSum(len/*+ACKlen*/, buf))||((buf[Pcksum]||buf[Pcksum+1])==0))
	{
		//获取CPU ID
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

	//g_Command = buf[Pcmd];          //保存上次命令用于收发短信
#endif
}

/*******************************************************************************
 * 函数名  : PC_BLL_SetVocoderKey
 * 描  述  : 设置声码器秘钥
 * 输  入  : 输入的形参无使用
 * 输  出  : 无
 * 返回值  : 无
 *******************************************************************************/
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
			/*把秘钥值写入指定Flash地址*/
//            FLASH_INTF_WriteStr (PCBA_ENCKEYID, (DH_U8*)lu8buf, 32);
#else
			/*读Flash中IFR功能时，先关中断，读完后再开中断*/
			_lpm_set_operation_mode(LPM_OPERATION_MODE_RUN);
			_int_disable();

			IFR_INTF_ReadKey(lau8key, 32);

			if(memcmp(lau8key, lu8buf, 32) != 0)
			{
				IFR_INTF_WriteKey(lu8buf, 32);
			}

			_int_enable();
			_lpm_set_operation_mode(LPM_OPERATION_MODE_HSRUN);
#endif

			/*返回成功应答包*/
			PC_BLL_ReadToPC(3, lu8buf, 0);
		}
	}
}

/**************************************************************************
 * 函数名: PC_BLL_PcCheckSum
 * 描  述: 生成校验信息的算法
 * 输  入: *buf:待生成校验信息的数组首地址, len:待生成校验信息的长度
 * 输  出: 无
 * 返回值: 生成校验码
 **************************************************************************/
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

	return ((DH_U16) sum ^ 0xFFFF);

}

/**************************************************************************
 * 函数名: PC_BLL_MakeCkSum
 * 描  述: 生成PC通信的校验信息
 * 输  入: buf[]:待生成校验信息的数组首地址, len:待生成校验信息的长度
 * 输  出: buf[]:待生成校验信息的数组首地址,
 * 返回值: 无
 **************************************************************************/
void PC_BLL_MakeCkSum(DH_U8 buf[], DH_U16 len)
{
	DH_U16 sum = 0;
	buf[Pcksum] = 0;
	buf[Pcksum + 1] = 0;
	sum = PC_BLL_PcCheckSum(buf, len);
	buf[Pcksum] = ((sum >> 8) & 0xff);
	buf[Pcksum + 1] = (sum & 0xff);
}

/**************************************************************************
 * 函数名: PC_BLL_CheckCkSum
 * 描  述: 检查校验信息是否正确
 * 输  入: buf[]:待校验信息的数组首地址, len:待检查校验信息的长度
 * 输  出: 无
 * 返回值: 校验结果是否正确，0:错误，1:正确
 **************************************************************************/
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

/**************************************************************************
 * 函数名: PC_BLL_MakeSDSend
 * 描  述:
 * 输  入: buf[]:待校验信息的数组首地址, len:待检查校验信息的长度
 * 输  出: 无
 * 返回值: 校验结果是否正确，0:错误，1:正确
 **************************************************************************/
void PC_BLL_MakeSDSend(uint16 len, uint8 ShortData[], uint8 ipflag)
{
	uint16 i;
	uint16 cksum;
	static uint8 UARTSendBUF[340];

	for (i = len; i > 0; i--)
	{
		UARTSendBUF[i + 10 - 1] = ShortData[i - 1];
	}

	UARTSendBUF[Phead] = 0x68;		//包头
	UARTSendBUF[Pcmd] = 0x0d;			//命令
	UARTSendBUF[Popmode] = 0x00;			//PC读写方式
	UARTSendBUF[Popack] = 3;
	UARTSendBUF[Plen] = (((len + 2) >> 8) & 0xff);		//数据长度
	UARTSendBUF[Plen + 1] = ((len + 2) & 0xff);
	UARTSendBUF[Pcksum] = 0; 			//cheksum
	UARTSendBUF[Pcksum + 1] = 0;
	UARTSendBUF[Pqu] = (1 << 3);			//表示单包
	UARTSendBUF[Ipdata] = (ipflag << 4); //是否是IP数据
	if (ipflag)
	{
		UARTSendBUF[Ipdata] |= (1 << 3);
	}
	UARTSendBUF[Pdata + len + 2] = 0x10;
	cksum = PC_BLL_PcCheckSum(UARTSendBUF, len + 11);		//校验和  长度为包长加2个扩展字节
	UARTSendBUF[Pcksum] = ((cksum >> 8) & 0xff); 			//cheksum
	UARTSendBUF[Pcksum + 1] = (cksum & 0xff);
	PCLINK_INTF_TransmitFrame(UARTSendBUF, len + 11);
}

