

#ifndef _INTF_CODESW_H_
#define _INTF_CODESW_H_

/* ========================================================================== */

/* ========================================================================== */
#include "dh_type.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

    /* ========================================================================== */

    /* ========================================================================== */

    /* ========================================================================== */

    /* ========================================================================== */

    /* ========================================================================== */

    /* ========================================================================== */
    DH_VOID CODESW_INTF_GPIOInit(DH_VOID);
    DH_U8 CODESW_INTF_GetRealStat(DH_VOID);
    DH_VOID CODESW_INTF_SaveStatus(DH_U8 u8Data);
    DH_U8 CODESW_INTF_GetStatus(DH_VOID);
    DH_VOID CODESW_INTF_UpdateSqLevel(DH_VOID);
    DH_U8 CODESW_INTF_GetNextCode(DH_U8 adddec);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
