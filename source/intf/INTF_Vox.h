

#ifndef _INTF_VOX_H
#define _INTF_VOX_H

#define VOXCRT_ON  1
#define VOXCRT_OFF 0
#define VOX_I_MAX  250
#define VOX_I_MIN  100

extern uint8 voxbusyen;
extern uint16 VoxCallOutTimeOut;

void INTF_VoxInit(void);
void INTF_VoxCheck(void);
void INTF_VoxCallTimeOutStop(void);
void INTF_VoxKey(void);
void YuyinSK(void);
void CloseYuyinSK(void);
void YuyinSKChk(void);
void INTF_VoxChnChangeStop(void);

void SKDisp(void);

#endif
