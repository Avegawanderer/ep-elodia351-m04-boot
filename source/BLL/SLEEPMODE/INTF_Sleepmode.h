/*****************************************************************************
 *   文件名:sleepmode.h
 *	 文件描述:none；
 *   创建人：陶林，2010/08/17
******************************************************************************/
#ifndef _INTF_SLEEP_MODE_H
#define	_INTF_SLEEP_MODE_H

void INTF_SleepModeInit(void);
void INTF_EnterSleepMode(void);
void INTF_EnterDsleepMode(void);
void INTF_ExitDSlpMode(void);
void SYSCLKConfig_STOP(void);

#endif
