

#ifndef _TMR_HND_H
#define _TMR_HND_H

#define TMRHND_VOICE       1
#define TMRHND_CALLREF     (1 << 1)
#define TMRHND_DLYHND      (1 << 2)
#define TMRHND_KEYLONG     (1 << 3)
#define TMRHND_DLYHNDSTART (1 << 4)
#define TMRHND_SYJ         (1 << 6)
#define TMRHND_FREQ_CHG    (1 << 7)  /*  */
#define TMRHND_KEY_FUNC    (1 << 8)  /*  */
#define TMRHND_TOPSET      (1 << 9)  /*  */
#define TMRHND_ANIMAL      (1 << 10) /*  */
#define TMRHND_SCAN        (1 << 11) /*  */
#define TMRHND_PLL_CHK     (1 << 12) /*  */
#define TMRHND_3S_CTCSS    (1 << 13) /*  */

extern uint8 RadioNPauseTimer;

void TmrHndInit(void);
void TmrHndStart(uint32 Flg);
void TmrHndStop(uint32 Flg);

uint8 TmrHndGet(uint32 Flg);

#endif
