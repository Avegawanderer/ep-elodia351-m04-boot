

#ifndef _INTF_USART_H
#define _INTF_USART_H

int32_t USART_INIT(void);
void COM_DeInit(void);
void usart_Send(const uint8_t *buf, uint16_t len);
DH_U8 usart_getFirstData(void);

DH_U32 usart_Recv(DH_U8 *pu8Buf, DH_U32 u32Len, DH_U32 u32TimeOut);

#endif
