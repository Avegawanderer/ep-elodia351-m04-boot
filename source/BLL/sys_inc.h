#ifndef __SYS_INC_H
#define __SYS_INC_H

#ifdef __cplusplus
extern "C"
{
#endif

    //#include "dh_type.h"
    //#include "datatype.h"

    uint8_t getSysState(void);
    void setSysState(uint8_t stat);

    uint16_t GetBitsBuf(uint8_t *Buf, uint16_t Len);
    uint8_t ResetBitsBuf(uint16_t Cnt, uint8_t *Buf, uint16_t Len);
    uint8_t SetBitsBuf(uint8_t *Buf, uint16_t Len, uint8_t *err);

    uint8_t list_GetValidIndex(uint8_t item, LIST_MG_STR *str);
    uint8_t list_GetFirstValidIndex(LIST_MG_STR *str);
    uint8_t list_GetValidItem(uint8_t index, LIST_MG_STR *str);
    uint8_t list_GetFirstInValidIndex(LIST_MG_STR *str);
    uint8_t list_GetNextValidIndex(LIST_MG_STR *str, uint8_t curIndex, uint8_t dir);
    DH_BOOL list_AddValidIndex(uint8_t index, LIST_MG_STR *str);
    DH_BOOL list_DelValidIndex(uint8_t index, LIST_MG_STR *str);
    uint16_t getNextIndex(uint8_t upDwn, uint16_t curValue, uint16_t Item_start, uint16_t Item_Max);

    void CheckDispNum0(uint8_t *buf);
    int power(int base, int n);
    uint8_t getcharnum(char *buf, char c);
    uint32_t strtoi_len(uint8_t *buf, uint8_t len);
    void itostr_len(uint8_t *buf, uint32_t num, uint8_t len);
    void itostr(uint8_t *buf, uint32_t num);
    void strAppend(uint8_t *buf, uint8_t *str);
    void strCopy(uint8_t *buf, uint8_t *str);

#endif
    /*********************************************************************************************************
 **                            End Of File
 ********************************************************************************************************/
