

#ifndef _BLL_PC_
#define _BLL_PC_

//extern DH_VOID *uart_task_queue;
//extern _task_id task_id_uart;

#define Phead                 0
#define Pcmd                  1
#define Popmode               2
#define Popack                3
#define Pcksum                4
#define Plen                  6
#define Pdata                 8

#define Pqu                   8
#define Ipdata                9

#define UART_DATAMODE_HEAD    Phead
#define UART_DATAMODE_MODE    1
#define UART_DATAMODE_LENL    Plen
#define UART_DATAMODE_LENH    7

#define UART_PC_PACKETLEN     2048
#define UART_RECV_HEAD_LEN    19
#define UART_RECV_PWD_LEN     6

#define UART_MODE_PLLSET      0x20
#define UART_TUNER_TEST       0x21
#define UART_MODE_UIIMG       0x88
#define UART_MODE_READRCDLIST 0x58
#define UART_MODE_READMSGCONT 0x59

#if (FREQ_SCOPE_SET > FREQ_136_174)
#define UART_MODE_CMX138SET 0x22
#else
#define UART_MODE_CMX138SET 0x22
#endif

#define UART_RX_CON_WAITT                 (4 * 1000)
#define UART_RX_FIRST_I                   8

#define PCLINK_INTF_ReceiveFrame(a, b, c) usart_Recv(a, b, c)
#define PCLINK_INTF_TransmitFrame(a, b)   usart_Send(a, b)

DH_VOID PC_BLL_Init(DH_VOID);
DH_VOID PC_BLL_Proc(void *p_arg);

DH_VOID PC_BLL_PcCmdPro(DH_U8 *RecDataBuf, DH_U32 HeadLen);
DH_VOID PC_BLL_ReadToPC(DH_U8 command, DH_U8 buf[], DH_U16 len);

DH_U16 PC_BLL_PcCheckSum(DH_U8 *buf, DH_S16 len);
DH_U8 PC_BLL_CheckCkSum(DH_U8 buf[], DH_U16 len);
DH_VOID PC_BLL_MakeCkSum(DH_U8 buf[], DH_U16 len);

DH_VOID PC_BLL_ReadCPUID(DH_U16 len, DH_U8 buf[]);
DH_VOID PC_BLL_SetVocoderKey(DH_U16 len, DH_U8 buf[]);

#endif
