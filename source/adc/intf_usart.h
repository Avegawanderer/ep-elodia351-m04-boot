/*****************************************************************************
 *   文件名:INTF_usart.h
 *	 文件描述:none；
 *   创建人：陶林，2011/06/02
******************************************************************************/
#ifndef _INTF_USART_H
#define	_INTF_USART_H
 
 

void usart_Send(uint8_t *buf,uint16_t len);

DH_U32 usart_Recv(DH_U8 *pu8Buf, DH_U32 u32Len, DH_U32 u32TimeOut);

#endif
 
