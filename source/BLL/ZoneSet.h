/*****************************************************************************
 *   文件名:ZoneSet.h
 *	 文件描述:无；
 *   创建人：liuzhiheng2015\10\13
******************************************************************************/
#ifndef _ZONE_SET_H
#define	_ZONE_SET_H

/* 区域设置的最大数 */
#define ZONE_MAX 1

/* 一个区域的频道的数目*/
#define ZONE_CHN_MAX 16

/* 一个区域的频道号的最大数目 */
#define ZONE_CHNM_MAX 32


#define INVALIDCHN 0xff



struct _chninfo_ {
	uint8 ss;
	};


void ZoneChnInit(void);


uint8 Freq_UV(uint32 Freq);
DH_BOOL Check_SelectBand_Freq_scope(uint32 Freq,uint8 RxTxFlag,uint8 BandFlag);
DH_BOOL Check_Freq_scope(uint32 Freq,uint8 Flag);
DH_BOOL Check_SQ_Zero_Onband(void);
DH_BOOL Check_SQ_Zero(void);
void ChnSetInit(void);
void ChnSet( uint8 CCnt);
uint8 GetZoneNum(void);
//void SqLevel_set(STR_CODESW PLLStat);

void SetChPrio(uint8 ListItem,uint8 CHItem);
uint8 GetChPrio(uint8 ListItem);
void  AddChListInit(void);
void AddCh(uint8 Item);
void DelCh(uint8 Item);
uint8 GetScanChListNumb(uint8 ListNumb);
uint8 GetScanListNumb(void);
void AddChListDisp(uint16 where,uint16 Item);

void ChListDisp(uint8 where,uint8 Item);
uint8 ChnPowerGet(uint8);
void ScanListSet(uint8 Stat);
void ScanDispKeyUp(void);
void ScanDispKeyDown(void);
uint8 GetScanListNumb(void);
uint8 GetScanChListNumb(uint8 ListNumb);
void ChListDisp(uint8 where,uint8 Item);
void AddChListDisp(uint16 where,uint16 Item);

void InitScanListNum(void);
void PowerOnInitPrepareScanListNum(void);
void PowerOnInitScanList(void);

uint16 CHN_APP_GetBaseAddr(uint8 CCnt);
uint16 CHN_APP_GetFreqBaseAddr(uint8 bandflg);
void CHN_APP_GetChnInfo(struct _chninfo_  *pchninfo,uint8 CCnt);


uint8_t CHN_APP_GetScanChnNum(uint8_t zone);

uint8_t CHN_APP_GetChnNum(void);

uint8_t CHN_APP_ChnValidChk(uint8_t chn);

uint8_t CHN_APP_GetNextValidIndex(uint8_t curIndex,uint8_t dir);
uint8_t CHN_APP_GetFirstValidChn(void);

uint8_t CHN_APP_DelChn(uint8_t chn);
uint8_t CHN_APP_AddChn(uint8_t chn);
uint8_t Scan_APP_GetChnIndex(uint8_t item);
uint8_t Scan_APP_GetChnNum(void);

uint8_t Scan_APP_DelChn(uint8_t chn);

//0可用於優先信道確認
uint8_t Scan_APP_ChnValidChk(uint8_t chn);

void CHN_APP_GetChnInfo(struct _chninfo_  *pchninfo,uint8 CCnt);

#endif
