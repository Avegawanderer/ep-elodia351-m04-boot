

#ifndef BLL_CLK_H
#define BLL_CLK_H

/* ========================================================================== */

/* ========================================================================== */

#include "dh_type.h"

/* ========================================================================== */

/* ========================================================================== */
#define PLL_48M          1
#define PLL_72M          2
#define PLL_84M          3
#define PLL_96M          4
#define PLL_112M         5
#define PLL_120M         6
#define PLL_144M         7
#define PLL_168M         8
#define PLL_192M         9

#define PLL_CLK          PLL_168M
#define PLL_CLK_FLASH_RW PLL_84M

/* ========================================================================== */

/* ===========================================================================*/

extern stc_clk_sysclk_cfg_t m_stcSysclkCfg;

void SystemClockConfig(void);

void TaskHndStart(u32 Flg);

void TaskHndStop(u32 Flg);

u32 TaskHndGet(u32 Flg);

#endif
