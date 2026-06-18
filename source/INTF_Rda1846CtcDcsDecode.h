/*****************************************************************************
 *   文件名:INTF_Rda1846CtcDcsDecode.h
 *   文件描述:亚音软件解码
 *   创建人：yufeng	, 2013/11/18
******************************************************************************/
#ifndef _RDA1846_CTCDCSDECODE_H_
#define	_RDA1846_CTCDCSDECODE_H_


#define CDCSS_NUM 108

extern const unsigned short DcsCode_table_Normal[CDCSS_NUM][5];//83
extern const unsigned char DcsCode_table_Normal_Total[CDCSS_NUM];//83

#define RDA_RX_DCS_OFF_DELAY	500//  900//

void DcsParaClr(void);
void RecevieCdcss23bit(void);

#endif
