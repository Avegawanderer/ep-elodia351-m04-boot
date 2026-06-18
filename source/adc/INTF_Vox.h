/*****************************************************************************
 *   文件名:INTF_Vox.h
 *	 文件描述:none；
 *   创建人：陶林，2011/06/02
******************************************************************************/
#ifndef _INTF_VOX_H
#define	_INTF_VOX_H


/* 控制状态:关/开 */
#define VOXCRT_ON 1
#define VOXCRT_OFF 0
#define VOX_I_MAX 250//没有信号时的计数关闭拨号的到计数时间5s
#define VOX_I_MIN 100

extern uint8 voxbusyen;
extern uint16 VoxCallOutTimeOut;

void INTF_VoxInit(void);
void INTF_VoxCheck(void);
void INTF_VoxCallTimeOutStop(void);
void INTF_VoxKey(void);
void YuyinSK(void);
void CloseYuyinSK(void);
void YuyinSKChk( void );
void INTF_VoxChnChangeStop(void);

void SKDisp(void);


#endif

