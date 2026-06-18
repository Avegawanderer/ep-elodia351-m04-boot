/*****************************************************************************
 *   文件名:PcInfmSet.h
 *	 文件描述:无；
 *   创建人：陶林，2011/07/29
 ******************************************************************************/
#ifndef _BLL_PCINFMSET_
#define	_BLL_PCINFMSET_

#define PCEA_CHN_LEN 16
#define PCEA_PC_SET 272

/* 等待超时时间定义 */
#define PCWAITTOUTWRITE       (8*BSP_ALARM_FREQUENCY)		/* 写频等待 */
#define PCWAITTOUTREAD        (8*BSP_ALARM_FREQUENCY)	    /* 读频等待 */

/* 指令类型 */
#define PCCMDTYPE             0x0f

/* 最大重传次数 */
#define PCRESENDMAX           5

/* 串口协议格式信息 */
#define UF_READ               0				/* 读模式 */
#define UF_WRITE              1				/* 写模式 */

/* 读操作命令格式(cpu->pc) */
#define UFR_READDATA          0X00			/* 读频数据 */
#define UFR_RECOK             0X01			/* 应答，接收ok */
#define UFR_RECERR            0Xff			/* 应答，接收err，要求重发 */
#define UFR_RECSTOP1          0Xf0			/* 应答，接收err，停止发送超过传输次数 */
#define UFR_RECSTOP2          0Xf1			/* 应答，接收err，停止发送格式不匹配 */
#define UFR_BASEINFO          0X02			/* 提供基本信息 */

/* 写操作命令格式(pc->cpu) */
#define UFW_WRITEDATA         0X00			/* 写频数据 */
#define UFW_RECOK             0X01			/* 应答，接收ok */
#define UFW_RECERR            0Xff			/* 应答，接收err，要求重发 */
#define UFW_REQREAD           0X02			/* 读频命令请求帧 */
#define UFW_RECSTOP1          0X03			/* 异常结束 */
#define UFW_BASEINFO          0X04			/* 获取基本信息，具体内容在数据段中判断 */

/*读写频配置类型*/
#define DATATYPE_DMRINFO      0X00			/* 对讲机信息 */
#define DATATYPE_BASICSET     0X01			/* 基本配置*/
#define DATATYPE_AREASAT      0X02			/* 区域设置*/
#define DATATYPE_ALARM        0X03			/* 报警*/
#define DATATYPE_SCAN         0X04			/* 扫描*/
#define DATATYPE_LM           0X05			/* 联系人*/
#define DATATYPE_RECGROUPLIST 0X06	        /* 接收组列表*/
#define DATATYPE_DMRSET       0X07			/* DMR设置*/
#define DATATYPE_TWOTONE      0X08			/* 两音*/
#define DATATYPE_FIVETONE     0X09			/* 五音*/
#define DATATYPE_PTTID        0X0a			/* PTT_ID */


/* 获取pc写频数据帧时的出错信息 */
#define PCINFERR_OK         0		/* ok */
#define PCINFERR_TIMEOUT    1		/* 等待超时 */
#define PCINFERR_CHECKERR   2	    /* 校验出错 */
#define PCINFERR_FORMATERR  3		/* 格式错误 */


#define MENUSETPOWER_M                  1
#define MENUSETPOWER_L                  0



/* pc 软件的状态信息 */
typedef struct
{
	DH_U8 Stat; /* 当前状态机所处变量 */
	DH_U8 Clone; /* 克隆标志 */
	DH_U8 Type; /* 当前包类型 */
	DH_U8 LastType; /* 上次包类型 */
	DH_U8 Cnt; /* 当前所接收or 发送的包数 */
	DH_U8 ReSRFlg; /* 重传或重复接收次数 */
} STR_PCINF;

DH_VOID PC_BLL_PcWirteOk(DH_VOID);

DH_VOID PC_BLL_RecPcData(DH_U8* RecDataBuf, DH_U32 HeadLen);
DH_VOID PC_BLL_PcWirteData(DH_U8* Buf, DH_U16 Len, DH_U8 err);
DH_VOID PC_BLL_PcReadData(DH_U8* Buf, DH_U16 Len, DH_U8 err);
DH_VOID PC_BLL_PcReqInfo(DH_U8* Buf);
DH_VOID PC_BLL_PcMFSend(DH_U8* DstBuf, DH_U8 Type, DH_U8 Packet,
		DH_U8 PacketNum, DH_U8* SourBuf, DH_U16 Len, DH_U8 Opt);
DH_U8 PC_BLL_PcGetFrm(DH_U8* DstBuf, DH_U16* PLen, DH_U32 TimeOut,
		DH_U32 HeadLen);

DH_VOID PC_BLL_VersionInit(DH_VOID);
DH_VOID PC_BLL_PcWCIPGER(DH_U8* Buf, DH_U16 Len, DH_U8 err);
DH_VOID PC_BLL_PcRCIPGER(DH_U8* Buf, DH_U16 Len, DH_U8 err);

#endif

