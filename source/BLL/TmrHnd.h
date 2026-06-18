/*****************************************************************************
 *   文件名:TmrHnd.h
 *	 文件描述:none；
 *   创建人：陶林，2011/03/17
******************************************************************************/
#ifndef _TMR_HND_H
#define	_TMR_HND_H


#define TMRHND_VOICE 1	/* 音量定时器任务 */
#define TMRHND_CALLREF (1<<1)	/* 刷新通话时间计数定时器任务 */
#define TMRHND_DLYHND (1<<2)	/* 延时执行定时器任务 */
#define TMRHND_KEYLONG (1<<3)	/* 长按键执行任务 */
#define TMRHND_DLYHNDSTART (1<<4)	/* 启动延时定时器 */
#define TMRHND_SYJ (1<<6)	/* 收音机重新打开定时器 */
#define TMRHND_FREQ_CHG (1<<7)	/*  */
#define TMRHND_KEY_FUNC (1<<8)	/*  */
#define TMRHND_TOPSET (1<<9)	/*  */
#define TMRHND_ANIMAL (1<<10)	/*  */
#define TMRHND_SCAN (1<<11)	/*  */
#define TMRHND_PLL_CHK (1<<12)	/*  */
#define TMRHND_3S_CTCSS (1<<13)	/*  */



extern uint8 RadioNPauseTimer;/*重开收音机的倒计时计数器.*/






void TmrHndInit(void);
void TmrHndStart(uint32 Flg);
void TmrHndStop(uint32 Flg);

uint8 TmrHndGet(uint32 Flg);

#endif
