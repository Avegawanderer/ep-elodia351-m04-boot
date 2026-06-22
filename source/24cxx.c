#include "app_config.h"
#include "24cxx.h"

/**
 * @addtogroup BSP
 * @{
 */

/**
 * @addtogroup Components
 * @{
 */

/**
 * @defgroup 24CXX EEPROM Driver for 24CXX
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define EE_24CXX_WAIT_TIMEOUT (0x20000UL)

#define EepromLock()
#define EepromUnlock()
/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

#define FLASH_SECTOR_SIZE 32

void FLASH_INTF_WriteStr(uint16 Addr, uint8 *Buf, uint16 Len)
{
    //	uint32 AddrTmp = Addr;
    //	uint32 LenTmp = Len;
    //	uint8* BufTmp = Buf;
    uint8 CurPageLen;
    uint8 SectorOver;

    EepromLock();

    while (Len)
    {
        if (Addr + Len > 8192)
            while (1)
                ;
        SectorOver = Addr % FLASH_SECTOR_SIZE;

        if ((FLASH_SECTOR_SIZE - SectorOver) > Len)
        {
            CurPageLen = Len;
        }
        else
        {
            CurPageLen = FLASH_SECTOR_SIZE - SectorOver;
        }
        AT24Cxx_INTF_WriterPage(Addr, Buf, CurPageLen);
        Delay_ms(10);

        Addr = Addr + CurPageLen;
        Buf = Buf + CurPageLen;
        Len = Len - CurPageLen;
    }

    EepromUnlock();
}

uint8 E2PROMReadByte(uint16 address)
{
    //	uint8 err;
    uint8 Data;

    EepromLock();

    AT24Cxx_INTF_ReadStr(address, &Data, 1);

    //DRV_FlashRead(address, &Data, 1);

    EepromUnlock();

    return Data;
}

void FLASH_INTF_ReadStr(uint16 address, uint8 *e2prom_buf, uint16 length)
{
    //	uint8 err;

    EepromLock();

    AT24Cxx_INTF_ReadStr(address, e2prom_buf, length);

    //	DRV_FlashRead(address, e2prom_buf, length);

    EepromUnlock();
}
