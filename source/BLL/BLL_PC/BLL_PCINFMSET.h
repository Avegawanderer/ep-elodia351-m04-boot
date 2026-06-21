

#ifndef _BLL_PCINFMSET_
#define _BLL_PCINFMSET_

#define PCEA_CHN_LEN          16
#define PCEA_PC_SET           272

#define PCWAITTOUTWRITE       (8 * BSP_ALARM_FREQUENCY)
#define PCWAITTOUTREAD        (8 * BSP_ALARM_FREQUENCY)

#define PCCMDTYPE             0x0f

#define PCRESENDMAX           5

#define UF_READ               0
#define UF_WRITE              1

#define UFR_READDATA          0X00
#define UFR_RECOK             0X01
#define UFR_RECERR            0Xff
#define UFR_RECSTOP1          0Xf0
#define UFR_RECSTOP2          0Xf1
#define UFR_BASEINFO          0X02

#define UFW_WRITEDATA         0X00
#define UFW_RECOK             0X01
#define UFW_RECERR            0Xff
#define UFW_REQREAD           0X02
#define UFW_RECSTOP1          0X03
#define UFW_BASEINFO          0X04

#define DATATYPE_DMRINFO      0X00
#define DATATYPE_BASICSET     0X01
#define DATATYPE_AREASAT      0X02
#define DATATYPE_ALARM        0X03
#define DATATYPE_SCAN         0X04
#define DATATYPE_LM           0X05
#define DATATYPE_RECGROUPLIST 0X06
#define DATATYPE_DMRSET       0X07
#define DATATYPE_TWOTONE      0X08
#define DATATYPE_FIVETONE     0X09
#define DATATYPE_PTTID        0X0a /* PTT_ID */

#define PCINFERR_OK           0 /* ok */
#define PCINFERR_TIMEOUT      1
#define PCINFERR_CHECKERR     2
#define PCINFERR_FORMATERR    3

#define MENUSETPOWER_M        1
#define MENUSETPOWER_L        0

typedef struct
{
    DH_U8 Stat;
    DH_U8 Clone;
    DH_U8 Type;
    DH_U8 LastType;
    DH_U8 Cnt;
    DH_U8 ReSRFlg;
} STR_PCINF;

DH_VOID PC_BLL_PcWirteOk(DH_VOID);

DH_VOID PC_BLL_RecPcData(DH_U8 *RecDataBuf, DH_U32 HeadLen);
DH_VOID PC_BLL_PcWirteData(DH_U8 *Buf, DH_U16 Len, DH_U8 err);
DH_VOID PC_BLL_PcReadData(DH_U8 *Buf, DH_U16 Len, DH_U8 err);
DH_VOID PC_BLL_PcReqInfo(DH_U8 *Buf);
DH_VOID PC_BLL_PcMFSend(DH_U8 *DstBuf, DH_U8 Type, DH_U8 Packet,
                        DH_U8 PacketNum, DH_U8 *SourBuf, DH_U16 Len, DH_U8 Opt);
DH_U8 PC_BLL_PcGetFrm(DH_U8 *DstBuf, DH_U16 *PLen, DH_U32 TimeOut,
                      DH_U32 HeadLen);

DH_VOID PC_BLL_VersionInit(DH_VOID);
DH_VOID PC_BLL_PcWCIPGER(DH_U8 *Buf, DH_U16 Len, DH_U8 err);
DH_VOID PC_BLL_PcRCIPGER(DH_U8 *Buf, DH_U16 Len, DH_U8 err);

#endif
