/*******************************************************************************
 * Copyright (C) 2011-2013 ZheJiang Dahua Technology CO.,LTD.
 *
 * 文件名 : BLL_RF.c
 *
 * 作  者 : 123(ggg)<gg>
 * 版  本 : V1.0.0
 * 日  期 : 2015年4月14日 Create
 *
 * 描  述 : 
 *
 *       1. 硬件说明
 *          无。
 *
 *       2. 程序结构说明。
 *          无。
 *
 *       3. 使用说明。
 *          无。
 *
 *       4. 局限性说明。
 *          无。
 *
 *       5. 其他说明。
 *          无。
 *
 * 修订历史: 
 * 日    期: 
 * 修订版本:
 * 作    者:
 * 修订备注:
 *******************************************************************************/

/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include "APP_Config.h"
#include "include.h"


/* ========================================================================== */
/*                           宏和类型定义区                                   */
/* ========================================================================== */



typedef struct _strlo{
	DH_U8 dir;
	DH_U32 lofreq;
} STR_LO;

static STR_LO str_lo;
static PLL_INFO_S Pll_Str;

DH_U8 g_FpgaTx;

#if 0
OS_SEM Sem_ApcTiming;

OS_TCB TaskRFTCB;
CPU_STK TaskRfStk[TASKRF_STK_SIZE];
extern DH_U16 g_u16TimCnt_us ;
#endif
/* ========================================================================== */
/*                          数据结构定义区                                    */
/* ========================================================================== */

#if(FREQ_SCOPE_UV == FREQ_SCOPE_U)
const DH_U32 g_u32FreqSectorList[10] = {
		FREQ_U_0,FREQ_U_1,FREQ_U_2,FREQ_U_3,FREQ_U_4,
		FREQ_U_5,FREQ_U_6,FREQ_U_7,FREQ_U_8,FREQ_U_9,
};
const DH_U32 g_u32FreqIntSectorList[10] = {
		FREQ_U_Z0,FREQ_U_Z1,FREQ_U_Z2,FREQ_U_Z3,FREQ_U_Z4,
		FREQ_U_Z5,FREQ_U_Z6,FREQ_U_Z7,FREQ_U_Z8,
};
#else
const DH_U32 g_u32FreqSectorList[10] = {
		FREQ_V_0,FREQ_V_1,FREQ_V_2,FREQ_V_3,FREQ_V_4,
		FREQ_V_5,FREQ_V_6,FREQ_V_7,FREQ_V_8,FREQ_V_9,
};
const DH_U32 g_u32FreqIntSectorList[9] = {
		FREQ_V_Z0,FREQ_V_Z1,FREQ_V_Z2,FREQ_V_Z3,FREQ_V_Z4,
		FREQ_V_Z5,FREQ_V_Z6,FREQ_V_Z7,FREQ_V_Z8,
};
#endif


/* 步进 */
const DH_U16 const APCVALUE[64] =
{
	0x0000, 0x0001, 0x0003, 0x0006, 0x000a, 0x0010, 0x0017, 0x001f, 0x0028,0x0033, 0x003e, 0x004b, 0x0059, 0x0068, 0x0078, 0x0089,
	0x009a, 0x00ad, 0x00c1, 0x00d5, 0x00ea, 0x0100, 0x0116, 0x012d, 0x0145,0x015d, 0x0175, 0x018e, 0x01a7, 0x01c0, 0x01d9, 0x01f3,
	0x020d, 0x0228, 0x0244, 0x0260, 0x027c, 0x0299, 0x02b6, 0x02d4, 0x02fe,0x0311, 0x0331, 0x0351, 0x0371, 0x0392, 0x03b3, 0x03d5,
	0x03f7, 0x041a, 0x043d, 0x0461, 0x0486, 0x04ab, 0x04d1, 0x04f7, 0x051d,0x0544, 0x056c, 0x0594, 0x05bd, 0x05e6, 0x0616, 0x0647,
};


/* ========================================================================== */
/*                          函数声明区                                        */
/* ========================================================================== */


static void RF_BLL_ApcDlyUS(DH_U32 DlyTime);

void RF_Gpio_init(void)
{
    stc_port_init_t stcPortInit;

    /* configuration structure initialization */
    MEM_ZERO_STRUCT(stcPortInit);

    stcPortInit.enExInt   = Disable;
    stcPortInit.enPullUp  = Enable;
    stcPortInit.enPinMode = Pin_Mode_Out;


    /*  Port/Pin initialization */
    PORT_Init(RX_POW_EN_PORT, RX_POW_EN_PIN, &stcPortInit);
    PORT_Init(TX_POW_EN_PORT, TX_POW_EN_PIN, &stcPortInit);
    PORT_Init(VCO_POW_EN_PORT, VCO_POW_EN_PIN, &stcPortInit);
    PORT_Init(NW_SET_PORT, NW_SET_PIN, &stcPortInit);

	RF_VCO_POW_EN;
}


void RF_NW_Set(uint8 Band)
{
	if(Band==BW_12_5K) PORT_SetBits(NW_SET_PORT, NW_SET_PIN);
	else 	PORT_ResetBits(NW_SET_PORT, NW_SET_PIN);
}

DH_VOID RF_BLL_CalLoFreq(PLL_INFO_S *pstr)
{
//	DH_U32 freqscope = FREQ_UHF_H-FREQ_UHF_L;
//
//	if(FREQ_UPDOWN < freqscope && (pstr->u32RxFreq-FREQ_UHF_L)<(freqscope>>1))
//	{
//		str_lo.dir = 0;
//		str_lo.lofreq = pstr->u32RxFreq - FREQ_UPDOWN;
//	}
//	else
	{
		str_lo.dir = 1;
		str_lo.lofreq = pstr->u32RxFreq + FREQ_UPDOWN;
	}
}

DH_VOID RF_BLL_GetLoFreq(DH_U32 *u32Freq)
{
//	*u32Freq -= FREQ_UPDOWN;
	*u32Freq = str_lo.lofreq;
}

//+、-
DH_U8 RF_BLL_GetLoDir(DH_VOID)
{
	return str_lo.dir;
}

/***************************************************************************
* 函数名：apc_autocreat
* 描  述：获取高低功率
* 输  入：u32PllVal 频率
* 输  出：高低功率值
* 返回值：无
***************************************************************************/
static POWER_S apc_autocreat(DH_U32 u32PllVal)
{
    DH_U8 i = 0;
    DH_U8 Buf[4]={0}; 
    DH_U8 Buf_[4]={0};
    DH_U16 ApcSmall;
    DH_U16 ApcBig;
    POWER_S Str;
  
    for(i = 2;i < 9;i++)
    {
    	if(u32PllVal <= g_u32FreqSectorList[i]) break;
    }
	
	FLASH_INTF_ReadStr((TUNER_HPOWERADDR+1+(i-2)*2),Buf,4);
	FLASH_INTF_ReadStr((TUNER_LPOWERADDR+1+(i-2)*2),Buf_,4);
	if( i == 9 || u32PllVal <= g_u32FreqSectorList[1])
	{
		Buf[2] = Buf[0];
		Buf[3] = Buf[1];
		Buf_[2] = Buf_[0];
		Buf_[3] = Buf_[1];
	}

    ApcSmall = (Buf[1]<<8)+Buf[0];
    ApcBig =   (Buf[3]<<8)+Buf[2];
    Str.u16H = (DH_U16)(((DH_F64)u32PllVal-g_u32FreqSectorList[i-1])/(g_u32FreqSectorList[i]-g_u32FreqSectorList[i-1])*(ApcBig-ApcSmall)+ApcSmall+0.5);
    ApcSmall=  (Buf_[1]<<8)+Buf_[0];
    ApcBig =   (Buf_[3]<<8)+Buf_[2];
    Str.u16L = (DH_U16)(((DH_F64)u32PllVal-g_u32FreqSectorList[i-1])/(g_u32FreqSectorList[i]-g_u32FreqSectorList[i-1])*(ApcBig-ApcSmall)+ApcSmall+0.5);
    Str.u16M = (Str.u16H+ Str.u16L)/2;	

//    printk("apc %d,%d H%d,L%d",u32PllVal,g_u32FreqSectorList[i],Str.u16H,Str.u16L);
	return Str;
}

#if 0
/***************************************************************************
* 函数名：afc_autocreat
* 描  述：获取接收频偏值
* 输  入：无
* 输  出：接收频偏值
* 返回值：无
***************************************************************************/
static BIAS_S afc_autocreat(DH_VOID)
{
    DH_U8 Buf[2];				/*|T(0-1)|T+10(2-3)|OffSet(4-5)|*/
    DH_F32 VoltageRef;
    BIAS_S Str;
    
    FLASH_INTF_ReadStr(g_FlashSaveFlg,TUNER_TXRXADDR+1,Buf,2);
    
    //Str.u16Tx = (Buf[1]<<8)+Buf[0];
    Str.u16Tx = Buf[0];
    
    VoltageRef = 1.228+ Str.u16Tx*0.422/128;
    
    Str.u16Rx = (DH_U16)(VoltageRef*1023/3.3+0.5);

    return Str;
} 

/***************************************************************************
* 函数名：afc_autocreat
* 描  述：获取发送和接收频偏值
* 输  入：u8Attr 信道属性，
          u32PllVal 发射频率，
* 输  出：获取两点平衡值
* 返回值：无
***************************************************************************/
BALANCE_S modbalance_autcreate(DH_U8 u8Attr, DH_U32 u32PllVal)
{
    DH_U8 Buf[4];
    DH_U8 i;
    BALANCE_S Str;
	for(i = 2;i < 9;i++)
	{
		if(u32PllVal <= g_u32FreqSectorList[i]) break;
	}
//    if(u32PllVal <= Balance_Arr[1])
//    {
//        i = 1;
//    }
//    else if(u32PllVal <= Balance_Arr[2])
//    {
//        i = 2;
//    }
//    else if(u32PllVal <= Balance_Arr[3])
//    {
//        i = 3;
//    }
//    else if(u32PllVal <= Balance_Arr[4])
//    {
//        i = 4;
//    }
//    else if(u32PllVal <= Balance_Arr[5])
//    {
//        i = 5;
//    }
//    else
//    {
//        i = 7;
//    }
//	if(u8Attr == CHNSTAT_DMR)
	{
		FLASH_INTF_ReadStr(g_FlashSaveFlg,(TUNER_DMODBALADDR+1+(i-2)*2),Buf,4);

//	else
//	{
//		FLASH_INTF_ReadStr(g_FlashSaveFlg,(TUNER_MMODBALADDR+1+(i-2)*2),Buf,4);
//	}
		if( i == 9 || u32PllVal <= g_u32FreqSectorList[1])
		{
			Buf[2] = Buf[0];
			Buf[3] = Buf[1];
		}

		Str.u8Mod1 = (DH_U8)(((DH_F64)u32PllVal-g_u32FreqSectorList[i-1])/(g_u32FreqSectorList[i]-g_u32FreqSectorList[i-1])*(Buf[2]-Buf[0])+Buf[0]+0.5);
		Str.u8Mod2 = (DH_U8)(((DH_F64)u32PllVal-g_u32FreqSectorList[i-1])/(g_u32FreqSectorList[i]-g_u32FreqSectorList[i-1])*(Buf[3]-Buf[1])+Buf[1]+0.5);
	}/*else{
		Str.u8Mod1 = 0;
		Str.u8Mod2 = 0;
	}*/
    return Str;	
}


/***************************************************************************
* 函数名：RF_BLL_Process
* 描  述：射频控制任务
* 输  入：无
* 输  出：无
* 返回值：无
***************************************************************************/
DH_VOID  RF_BLL_Process(void *p_arg)
{	
    static CPU_TS ts;
    OS_ERR err;
    while(1)
    {     
        OSTaskSemPend(0, OS_OPT_PEND_BLOCKING, &ts, &err);

        #if 1
        
        if(CHNSTAT_DMR == CHN_BLL_GetAttr())
        {
            if (str_rf_int.TxStat == RF_TX_INT)
            {
                RF_BLL_EnableTx();
                str_rf_int.TxStat = RF_IDLE_INT;

            }
            else
            //else if (str_rf_int.RxStat == RF_RX_INT)
            {
                RF_BLL_EnableRx();
                str_rf_int.RxStat = RF_IDLE_INT;
            } 
        }
	#endif
    }
}

/***************************************************************************
* 函数名：tv_autocreat
* 描  述：获取接收频偏值
* 输  入：无
* 输  出：接收频偏值
* 返回值：无
***************************************************************************/
static DH_U16 tv_autocreat(DH_U32 u32PllVal)
{
    DH_U8 Buf[4];    
    DH_U8 i=0;
    DH_U16 TvValue=0;
    DH_U16 TvSmall=0;
    DH_U16 TvBig=0;
    
	for(i = 2;i < 9;i++)
	{
		if(u32PllVal <= g_u32FreqSectorList[i]) break;
	}
	
//	FLASH_INTF_ReadStr((TUNER_TVADDR+1+(i-2)*2),Buf,4);
	if( i == 9 || u32PllVal <= g_u32FreqSectorList[1])
	{
		Buf[2] = Buf[0];
		Buf[3] = Buf[1];
	}

    TvSmall=(Buf[1]<<8)+Buf[0];
    TvBig=(Buf[3]<<8)+Buf[2];
    TvValue=(DH_U16)((DH_F64)(TvBig-TvSmall)/(g_u32FreqSectorList[i]-g_u32FreqSectorList[i-1])*(u32PllVal-g_u32FreqSectorList[i-1])+TvSmall+0.5);

    return TvValue;
}

/***************************************************************************
* 函数名：afc_autocreat
* 描  述：获取发送和接收频偏值
* 输  入：u8Attr 信道属性，
          u32PllVal 发射频率，
* 输  出：获取两点平衡值
* 返回值：无
***************************************************************************/
DH_VOID apcdlytime_autcreate(DH_U8 u8Attr, DH_U32 u32PllVal,DH_U16* u16ApcUp,DH_U16* u16ApcDown)
{

    DH_U8 Buf[8];
    DH_U8 i;
	DH_S16 upl,uph,downl,downh;
	DH_F64 freqoff;
	for(i = 2;i < 9;i++)
	{
		if(u32PllVal <= g_u32FreqSectorList[i]) break;
	}

//	FLASH_INTF_ReadStr(g_FlashSaveFlg,(TUNER_APCADJUSTADDR+1+(i-2)*4),Buf,8);

    if( i == 9 || u32PllVal <= g_u32FreqSectorList[1])
    {
        Buf[4] = Buf[0];
        Buf[5] = Buf[1];
        Buf[6] = Buf[2];
		Buf[7] = Buf[3];
    }
    upl = Buf[0]+Buf[1]*256+0.5;
    uph = Buf[4]+Buf[5]*256+0.5;
    downl = Buf[2]+Buf[3]*256+0.5;
    downh = Buf[6]+Buf[7]*256+0.5;
    freqoff = (DH_F64)(u32PllVal-g_u32FreqSectorList[i-1])/(g_u32FreqSectorList[i]-g_u32FreqSectorList[i-1]);

    *u16ApcUp =   (freqoff*(DH_S16)(uph-upl)+upl);
    *u16ApcDown = (freqoff*(DH_S16)(downh-downl)+downl);
    if(*u16ApcUp>2000)*u16ApcUp = 0;
    if(*u16ApcDown>2000)*u16ApcDown = 0;
}

#endif
/***************************************************************************
* 函数名：PLL_BLL_GetRfVal
* 描  述：获取射频参数
* 输  入：u8Attr 信道属性，
          u32TxFreq 发射频率，
          u32RxFreq 接收频率
* 输  出：射频参数结构体
* 返回值：无
***************************************************************************/
PLL_INFO_S PLL_BLL_GetRfVal(DH_U32 u32TxFreq, DH_U32 u32RxFreq)
{
    PLL_INFO_S p_str;
    POWER_S    po_str;
 //   BIAS_S     bi_str;
//    BALANCE_S  ba_str;

    po_str = apc_autocreat(u32TxFreq);
    
//   bi_str = afc_autocreat();

 //   ba_str = modbalance_autcreate(u8Attr,u32TxFreq);

    p_str.u32TxFreq = u32TxFreq;
    p_str.u32RxFreq = u32RxFreq;
    p_str.u16ApcHighVal = po_str.u16H;
    p_str.u16ApcLowVal  = po_str.u16L;
    p_str.u16ApcVal  = p_str.u16ApcLowVal;
//    p_str.u16AmpbiasVal = ampbais_autoset(bi_str.u16Tx);
//    p_str.u16AfcVal = afc_autoset(bi_str.u16Rx);
  //  p_str.u8Mod1= ba_str.u8Mod1;
 //   p_str.u8Mod2= ba_str.u8Mod2;
	p_str.u16TvVal = 0;//tv_autocreat(u32RxFreq);
//	p_str.u8RDA1846sPower = FLASH_INTF_ReadByte(g_FlashSaveFlg,TUNER_TPLDPLNARROWADDR);
//	apcdlytime_autcreate(u32TxFreq,&p_str.u16APCUpTime,&p_str.u16APCDownTime);


	RF_BLL_CalLoFreq(&p_str);

	if (CHN_BLL_PowerGet())           //NEW
	{
		p_str.u16ApcVal = p_str.u16ApcHighVal;
	}
	else
	{
		p_str.u16ApcVal = p_str.u16ApcHighVal;
	}
    RF_BLL_SavePll(p_str);
    

    return p_str;
}

/***************************************************************************
* 函数名：RF_BLL_SavePll
* 描  述：保存锁相环参数
* 输  入：str 射频参数结构体
* 输  出：无
* 返回值：无
***************************************************************************/
DH_VOID RF_BLL_SavePll(PLL_INFO_S str)
{
    Pll_Str = str;
}

/***************************************************************************
* 函数名：RF_BLL_GetPll
* 描  述：返回锁相环参数
* 输  入：无
* 输  出：PLL_INFO_S
* 返回值：无
***************************************************************************/
PLL_INFO_S   RF_BLL_GetPll(DH_VOID)
{
    return Pll_Str;
}

/***************************************************************************
* 函数名：RF_BLL_GetPll
* 描  述：返回锁相环参数
* 输  入：无
* 输  出：PLL_INFO_S
* 返回值：无
***************************************************************************/
PLL_INFO_S*   RF_BLL_GetPllStrP(DH_VOID)
{
    return &Pll_Str;
}
/***************************************************************************
* 函数名：RF_BLL_Init
* 描  述：射频控制任务初始化
* 输  入：无
* 输  出：无
* 返回值：无
***************************************************************************/
DH_U8 RF_BLL_Init(DH_VOID)
{
#if 0
    /* 创建信道互斥信号量 */
    //if (dhsem_create(&RfLock_Sem,0) != DH_OK()) 
    //{
    //     return 1;
    //}

    /* 创建信道互斥信号量 */
    if (dhsem_create(&Sem_ApcTiming,0) != DH_OK()) {
		printk("Creating Sem_ApcTiming failed: \r\n");
	}


    
    /* 需创建射频任务 */
    //dhtask_create(0, u16TaskID, 0);

    OS_ERR err;

    memset(TaskRfStk, 0x00, sizeof(CPU_STK)*TASKRF_STK_SIZE);
    
    OSTaskCreate(&TaskRFTCB,\
                "rfint",\
                (OS_TASK_PTR)RF_BLL_Process,\
                0,\
                TASKRF_PRIO,\
                &TaskRfStk[0],\
                0,\
                TASKRF_STK_SIZE,\
                0,0,0,\
                OS_OPT_TASK_NONE | OS_OPT_TASK_STK_CHK,\
                &err);
    #endif
    return 0;
}

extern DH_U8 g_FMIC_Sel ;
/***************************************************************************
* 函数名：RF_BLL_RxInit
* 描  述：信道射频接收初始化
* 输  入：str 射频结构体参数
* 输  出：无
* 返回值：无
***************************************************************************/
DH_VOID RF_BLL_RxInit(PLL_INFO_S *pStr)
{
#if 1
    RF_INTF_APCSet(0);
    RF_TX_EN_LOW_DIS;
  //  RF_ANT_SW_LOW_RX;
    
 

	RF_INTF_TVSet(pStr->u16TvVal);

    FM_APP_SetRxInit(pStr->u32RxFreq);	
	RF_NW_Set(CHN_BLL_GetFMBandWide());
	RF_BLL_EnableRx();
	FM_APP_SetRxSubVoice();

    RF_RX_EN_HIGH_EN;

	if (CHN_BLL_PowerGet())           //NEW
  	{
  		RF_BLL_SetPower(MENUSETPOWER_M);
  	}
  	else
  	{
  		RF_BLL_SetPower(MENUSETPOWER_L);
  	}
	#endif
}

/***************************************************************************
* 函数名：RF_BLL_EnableRx
* 描  述：射频配置为接收模式
* 输  入：无
* 输  出：无
* 返回值：无
***************************************************************************/
DH_VOID   RF_BLL_EnableRx(DH_VOID)
{
    #if 1
    PLL_INFO_S str = RF_BLL_GetPll();

 //   RF_BLL_ApcDlyUS(str.u16APCDownTime);
    /*在I2C通信上，耗时，暂关闭，用固定的值*/
    RF_INTF_APCChange(0,str.u16ApcVal);

 //   RF_ANT_SW_LOW_RX;
    
    RF_TX_EN_LOW_DIS;
    
    Delay_us(300);

    RF_INTF_SetRx(str.u32RxFreq);

    Delay_us(300);
    
    RF_RX_EN_HIGH_EN;

    Delay_us(200);

//    RDA1846S_INTF_SetSubAudioOpen();

    #endif
}

/***************************************************************************
* 函数名：RF_BLL_EnalbeTx
* 描  述：射频配置为发射模式
* 输  入：无
* 输  出：无
* 返回值：无
***************************************************************************/
DH_VOID   RF_BLL_EnableTx(DH_VOID)
{
#if 1
    PLL_INFO_S str = RF_BLL_GetPll();
    
 //   RDA1846S_INTF_SetSubAudioClose();

    RF_RX_EN_LOW_DIS;

    Delay_us(50);

    RF_INTF_SetTx(str.u32TxFreq);

 //   RF_BLL_ApcDlyUS(str.u16APCUpTime);

    RF_TX_EN_HIGH_EN;

    Delay_us(200);

//    RF_ANT_SW_HIGH_TX;

    /*在I2C通信上，耗时，暂关闭，用固定的值*/
    RF_INTF_APCChange(str.u16ApcVal,0);
	#endif
}

/***************************************************************************
* 函数名：RF_BLL_SetPower
* 描  述：高低功率切换
* 输  入：u8Flg 高低功率选择标志
* 输  出：无
* 返回值：无
***************************************************************************/
DH_VOID RF_BLL_SetPower(DH_U8 u8Flg)
{
    if(u8Flg == MENUSETPOWER_M)
    {
        Pll_Str.u16ApcVal = Pll_Str.u16ApcHighVal;
    }
    else
    {
        Pll_Str.u16ApcVal = Pll_Str.u16ApcLowVal;
    }
}

/***************************************************************************
* 函数名：RF_BLL_GetLimit
* 描  述：发射权限获取
* 输  入：无
* 输  出：0:可以发射，非0不能发射
* 返回值：无
***************************************************************************/
DH_U8 RF_BLL_GetLimit(DH_VOID)
{

#if 0
    DH_U8 attr;
    DMRTXCHN_INFO_S d_str;
//    FMTXCHN_INFO_S a_str;
//    SIGNAL_U str;
    DH_U8 i;
    DH_U16 SignalValue=0;
    
    attr = CHN_BLL_GetAttr();
if(attr == CHNSTAT_FM)
    {
    	return DH_TRUE;

    }
		#else
 //   ADC_DRV_GetChnValue(CHN_ADC_RSSI);
    #endif

    return 0;
}

void   RF_BLL_ApcDlyUS(DH_U32 DlyTime)
{
	Delay_ms(DlyTime);


}





uint8 RxBusyCheck(void)//(显示呼叫图标的情况下)判断是否处于接受起来的忙状态
{//1==正处于接收状态.
#if 0//CALLDISP_EN
	if(GloVarFlag.FmRecStat&&GloVarFlag.FmMontFlg)//监听状态下//不允许图标显示.
	{
		if(Signal_stat)
		{return TRUE;}
		else
		{return FALSE;}
	}
	if(GloVarFlag.FmRecStat&&(Check_SQ_Zero_Onband()&&Signal_stat))//静噪为0但是有信号.
	{
		return TRUE;
	}
	if(GloVarFlag.FmRecStat&&!Check_SQ_Zero_Onband())//静噪大于0且已经接起来的情况下.
	{
		return TRUE;
	}	
	return FALSE;
#else
	return FALSE;
#endif
}


