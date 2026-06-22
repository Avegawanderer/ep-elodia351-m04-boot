#include "app_config.h"

/******************************************************************************
*
 * Local type definitions ('typedef')
 
******************************************************************************/

/******************************************************************************
*
 * Local pre-processor symbols/macros ('#define')
 
******************************************************************************/
#define FLASH_SECTOR62_ADRR     (0x0007C000u)
/******************************************************************************
*
 * Global variable definitions (declared in header file with 'extern')
 
******************************************************************************/

/******************************************************************************
*
 * Local function prototypes ('static')
 
******************************************************************************/
#define EFM_START_ADDR          (0x00000000UL) /*!< Flash start address */

#define EFM_END_ADDR            (0x0007FFFFUL) /*!< Flash end address */
#define EFM_OTP_START_ADDR      (0x03000C00UL) /*!< OTP start address */
#define EFM_OTP_END_ADDR        (0x03000FFBUL) /*!< OTP end address */
#define EFM_OTP_LOCK_ADDR_START (0x03000FC0UL) /*!< OTP lock start address */
#define EFM_OTP_LOCK_ADDR_END   (0x03000FFCUL) /*!< OTP lock end address */

#define SECTOR_SIZE             (0x2000UL)
/******************************************************************************
*
 * Local variable definitions ('static')
 
******************************************************************************/
/* Flash definitions */
#define FLASH_BASE              (EFM_START_ADDR)
#define FLASH_SIZE              (EFM_END_ADDR + 1U)
#define FLASH_SECTOR_SIZE       (SECTOR_SIZE)
#define FLASH_SECTOR_NUM        (64U)

/******************************************************************************
*
 * Function implementation - global ('extern') and local ('static')
 
******************************************************************************/
int32_t EFM_Program(uint32_t u32Addr, uint8_t *pu8Buf, uint32_t u32Len);
//{
//    int32_t i32Ret = LL_OK;
//    uint32_t u32Tmp;
//    uint8_t u8Shift;
//    uint32_t u32LoopWords = u32Len >> 2UL;
//    uint32_t u32RemainBytes = u32Len % 4UL;
//    uint32_t *u32pSource = (uint32_t *)(uint32_t)pu8Buf;
//    uint32_t *u32pDest = (uint32_t *)u32Addr;
//    uint32_t u32LastWord;

//    DDL_ASSERT(IS_EFM_REG_UNLOCK());
//    DDL_ASSERT(IS_EFM_FWMC_UNLOCK());
//    DDL_ASSERT(IS_EFM_ADDR(u32Addr));
//    DDL_ASSERT(IS_EFM_ADDR(u32Addr + u32Len - 1UL));
//    DDL_ASSERT(IS_ADDR_ALIGN_WORD(u32Addr));

//    u8Shift = 0U;

//    /* CLear the error flag. */
//    EFM_ClearStatus(EFM_FLAG_ALL);
//    /* Get CACHE status */
//    u32Tmp = READ_REG32_BIT(CM_EFM->FRMC, EFM_CACHE_ALL);
//    /* Disable CACHE */
//    CLR_REG32_BIT(CM_EFM->FRMC, EFM_CACHE_ALL);

//    /* Set single program mode. */
//    MODIFY_REG32(CM_EFM->FWMC, EFM_FWMC_PEMOD, EFM_MD_PGM_SINGLE);

//    while (u32LoopWords-- > 0UL) {
//        /* program data. */
//        *u32pDest++ = *u32pSource++;
//        /* Wait for ready flag. */
//        if (LL_ERR_TIMEOUT == EFM_WaitFlag(EFM_FLAG_RDY << u8Shift, EFM_PGM_TIMEOUT)) {
//            i32Ret = LL_ERR_NOT_RDY;
//        }
//        /* CLear the operation end flag. */
//        EFM_ClearStatus(EFM_FLAG_OPTEND << u8Shift);
//    }

//    if (0U != u32RemainBytes) {
//        u32LastWord = *u32pSource;
//        u32LastWord |= 0xFFFFFFFFUL << (u32RemainBytes * 8UL);
//        *u32pDest++ = u32LastWord;
//        /* Wait for ready flag. */
//        if (LL_ERR_TIMEOUT == EFM_WaitFlag(EFM_FLAG_RDY << u8Shift, EFM_PGM_TIMEOUT)) {
//            i32Ret = LL_ERR_NOT_RDY;
//        }
//        /* CLear the operation end flag. */
//        EFM_ClearStatus(EFM_FLAG_OPTEND << u8Shift);

//    }

//    /* Reset cache data */
//    /* Recover CACHE function */
//    MODIFY_REG32(CM_EFM->FRMC, EFM_CACHE_ALL, u32Tmp);
//    return i32Ret;
//}

int32_t FLASH_EraseSector(uint32_t u32Addr)
{
    /* Unlock EFM. */
    EFM_Unlock();

    /* Enable flash. */
    EFM_FlashCmd(Enable);
    /* Wait flash ready. */
    while (Set != EFM_GetFlagStatus(EFM_FLAG_RDY))
    {
        ;
    }

    /* Erase sector 62. */
    EFM_SectorErase(u32Addr);

    /* Lock EFM. */
    EFM_Lock();
    return LL_OK;
}

/**
 * @brief  Erase flash sector.
 * @param  u32Addr                      Flash address
 * @param  u32Size                      Firmware size (0: current address sector)
 * @retval An en_result_t enumeration value:
 *           - LL_OK: Erase succeeded
 *           - LL_ERR: Erase timeout
 *           - LL_ERR_INVD_PARAM: The parameters is invalid.
 */
int32_t FLASH_INTF_EraseSectors(uint32_t u32Addr, uint32_t u32Size)
{
    (void)u32Addr;
    (void)u32Size;
    return LL_OK;
}

/**
 * @brief  Write data to flash.
 * @param  u32Addr                      Flash address
 * @param  pu8Buff                      Pointer to the buffer to be written
 * @param  u32Len                       Buffer length
 * @retval int32_t:
 *           - LL_OK: Program successful.
 *           - LL_ERR_INVD_PARAM: The parameters is invalid.
 *           - LL_ERR_NOT_RDY: EFM if not ready.
 *           - LL_ERR_ADDR_ALIGN: Address alignment error
 */
int32_t FLASH_WriteData(uint32_t u32Addr, uint8_t *pu8Buff, uint32_t u32Len)
{
    if ((pu8Buff == NULL) || (u32Len == 0U) || ((u32Addr + u32Len) > (FLASH_BASE + FLASH_SIZE)))
    {
        return LL_ERR_INVD_PARAM;
    }
    if (0UL != (u32Addr % 4U))
    {
        return LL_ERR_ADDR_ALIGN;
    }
    int32_t reg = EFM_Program(u32Addr, pu8Buff, u32Len);
    return reg;
}

/**
 * @brief  Read data from flash.
 * @param  u32Addr                      Flash address
 * @param  pu8Buff                      Pointer to the buffer to be reading
 * @param  u32Len                       Buffer length
 * @retval int32_t:
 *           - LL_OK: Read data succeeded
 *           - LL_ERR_INVD_PARAM: The parameters is invalid
 *           - LL_ERR_ADDR_ALIGN: Address alignment error
 */
int32_t FLASH_ReadData(uint32_t u32Addr, uint8_t *pu8Buff, uint32_t u32Len)
{
    uint32_t i;
    uint32_t u32WordLength, u8ByteRemain;
    uint32_t *pu32ReadBuff;
    __IO uint32_t *pu32FlashAddr;
    uint8_t *pu8Byte;
    __IO uint8_t *pu8FlashAddr;

    if ((pu8Buff == NULL) || (u32Len == 0U) || ((u32Addr + u32Len) > (FLASH_BASE + FLASH_SIZE)))
    {
        return LL_ERR_INVD_PARAM;
    }
    if (0UL != (u32Addr % 4U))
    {
        return LL_ERR_ADDR_ALIGN;
    }

    pu32ReadBuff = (uint32_t *)(uint32_t)pu8Buff;
    pu32FlashAddr = (uint32_t *)u32Addr;
    u32WordLength = u32Len / 4U;
    u8ByteRemain = u32Len % 4U;
    /* Read data */
    for (i = 0UL; i < u32WordLength; i++)
    {
        *(pu32ReadBuff++) = *(pu32FlashAddr++);
    }
    if (0UL != u8ByteRemain)
    {
        pu8Byte = (uint8_t *)pu32ReadBuff;
        pu8FlashAddr = (uint8_t *)pu32FlashAddr;
        for (i = 0UL; i < u8ByteRemain; i++)
        {
            *(pu8Byte++) = *(pu8FlashAddr++);
        }
    }

    return LL_OK;
}

/**
 
*******************************************************************************
 ** \brief  Main function of template project
 **
 ** \param  None
 **
 ** \retval int32_t return value, if needed
 **
 
******************************************************************************/
int32_t EFM_Program(uint32_t u32Addr, uint8_t *pu8Buf, uint32_t u32Len)
{

    int32_t i32Ret = LL_OK;
    //    uint32_t u32Tmp;
    //    uint8_t u8Shift;
    uint32_t u32LoopWords = u32Len >> 2UL;
    uint32_t u32RemainBytes = u32Len % 4UL;
    uint32_t *u32pSource = (uint32_t *)(uint32_t)pu8Buf;
    uint32_t u32pDest = u32Addr;
    uint32_t u32LastWord;

    /* Unlock EFM. */
    EFM_Unlock();

    /* Enable flash. */
    EFM_FlashCmd(Enable);
    /* Wait flash ready. */
    while (Set != EFM_GetFlagStatus(EFM_FLAG_RDY))
    {
        ;
    }

    /* Erase sector 62. */
    EFM_SectorErase(u32pDest);

    //   u32Addr = FLASH_SECTOR62_ADRR;
    while (u32LoopWords-- > 0UL)
    {
        /* program data. */
        i32Ret = EFM_SingleProgram(u32pDest, *u32pSource);
        u32pDest += 4;
        u32pSource++;
    }

    if (0U != u32RemainBytes)
    {
        u32LastWord = *u32pSource;
        u32LastWord |= 0xFFFFFFFFUL << (u32RemainBytes * 8UL);
        i32Ret = EFM_SingleProgram(u32pDest, u32LastWord);
    }
    //    for(i = 0u; i < 10u; i++)
    //    {
    //        EFM_SingleProgram(u32Addr,pu8Buf+i);
    //        u32Addr += 4u;
    //    }

    /* Lock EFM. */
    EFM_Lock();
    Delay_ms(5);
    return i32Ret;
}
