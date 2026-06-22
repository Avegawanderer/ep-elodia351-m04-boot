#ifndef __24CXX_H__
#define __24CXX_H__

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

    /*******************************************************************************
 * Include files
 ******************************************************************************/
    //#include "hc32_ll_def.h"

    /**
 * @addtogroup BSP
 * @{
 */

    /**
 * @addtogroup Components
 * @{
 */

    /**
 * @addtogroup 24CXX
 * @{
 */

    /*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

    /**
 * @brief 24CXX low layer structure definition
 */
    typedef struct
    {
        /* Properties */
        uint32_t u32PageSize;
        uint32_t u32Capacity;
        /* Methods */
        void (*Delay)(uint32_t);
        int32_t (*Init)(void);
        void (*DeInit)(void);
        int32_t (*WritePage)(uint16_t u16Addr, const uint8_t *pu8Buf, uint32_t u32Len);
        int32_t (*Read)(uint16_t u16Addr, uint8_t *pu8Buf, uint32_t u32Len);
        int32_t (*GetStatus)(void);
    } stc_24cxx_ll_t;

    /*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/

    /*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

    /*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/
    /**
 * @addtogroup 24CXX_Global_Functions 24CXX Global Functions
 * @{
 */
    void AT24Cxx_Init(void);
    void AT24Cxx_INTF_WriterPage(uint16 Addr, uint8 *Buf, uint16 Len);
    void AT24Cxx_INTF_ReadStr(uint16 Addr, uint8 *Buf, uint16 Len);
    void FLASH_INTF_WriteStr(uint16 Addr, uint8 *Buf, uint16 Len);
    uint8 E2PROMReadByte(uint16 address);
    void FLASH_INTF_ReadStr(uint16 address, uint8 *e2prom_buf, uint16 length);

    int32_t EE_24CXX_Init(const stc_24cxx_ll_t *pstc24cxxLL);
    int32_t EE_24CXX_DeInit(const stc_24cxx_ll_t *pstc24cxxLL);
    int32_t EE_24CXX_Read(const stc_24cxx_ll_t *pstc24cxxLL, uint16_t u16Addr, uint8_t *pu8Buf, uint32_t u32Len);
    int32_t EE_24CXX_Write(const stc_24cxx_ll_t *pstc24cxxLL, uint16_t u16Addr, const uint8_t *pu8Buf, uint32_t u32Len);
    int32_t EE_24CXX_WaitIdle(const stc_24cxx_ll_t *pstc24cxxLL);
    /**
 * @}
 */

    /**
 * @}
 */

    /**
 * @}
 */

    /**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __24CXX_H__ */
