/*****************************************************************************
 *   文件名:PcTunerSet.c
 *   文件描述:射频调测软件
 *   创建人：yufeng	, 2012/11/18
******************************************************************************/
#include "config.h"

#include "INTF_RF.h"
#include "BLL_RF.h"
//#include "BLL_HRC.h"

#if 0 //HRC5000

#if 1
#define PCTUNERCNTMAX		4
static DH_U8 ResCnt=0;

DH_U8 FreqCheckBuf[6]={0};
DH_U8 ErrNumbTestFlag;

/* 调测频点宏定义 */
#define FREQ403_175	403175000
#define FREQ414_175	414175000
#define FREQ425_175	425175000
#define FREQ436_625	436625000
#define FREQ447_775	447775000
#define FREQ458_775	458775000
#define FREQ469_825	469825000

DH_U32 gTestFREQList[7]={
403175000,414175000,425175000,436625000,447775000,
458775000,469825000,
};

DH_VOID PC_BLL_PcAfcAutCreate(DH_U8* Buf, DH_U8* Ampbias, DH_U16* Afc)
{
    //DH_U8 Buf[6];				/*|T(0-1)|T+10(2-3)|OffSet(4-5)|*/
    DH_F32 VoltageRef;

    //FlashReadPage(TUNER_TXRXADDR+1,Buf,6);
    //OffSet = (Buf[5]<<8)+Buf[4];

    *Ampbias = (Buf[1]<<8)+Buf[0];
    VoltageRef = 1.228+ (*Ampbias)*0.422/128;
    *Afc = (DH_U16)(VoltageRef*1023/3.3+0.5);
} 

/*******************************************************************************
* 函数名: PC_BLL_PcTunerRecData
* 描  述: 接收PC调测数据
* 输  入: *RecBuf:接收数据首地址，HeadLen:接收数据长度
* 输  出: 无
* 返回值: 无
*******************************************************************************/
DH_VOID PC_BLL_PcTunerRecData(DH_U8* RecBuf,DH_U32 HeadLen)
{	
	DH_U8 err;
	DH_U16 RecLen;
	static DH_U8 flg = 1;

	err=PC_BLL_TunerGetPcFrm(RecBuf, &RecLen, UART_RX_CON_WAITT, HeadLen);

	if((RecBuf[2]>=0x10 && RecBuf[2]<=0x13) || (RecBuf[2]>=0x20 && RecBuf[2]<=0x23))
	{
		//StrCode.Attr=PLLSTAT_D;
		//StrCode.GroupList = 0x00;
		//StrCode.AreaLM = 0x81;
		//StrCode.AreaRelay= 0x00;
		//SaveCodeSwStat(StrCode);
		if(RecBuf[2]>=0x10 && RecBuf[2]<=0x13) 
		{
			*WORK_MODE= 0x73;
		}
	}
	else
	{
		//StrCode.Attr=PLLSTAT_M;
		//RdaWrite(0x58, 0xbc7d);
		//if(RecBuf[2]==0x15 || RecBuf[2]==0x17)  /* 宽带 */
		//{
		//	StrCode.AreaRelay = 0x84;
		//}
		//else                                    /* 窄带 */
		//{
		//	StrCode.AreaRelay = 0x04;
		//}
		if(RecBuf[2]>=0x14 && RecBuf[2]<=0x17) 
		{
			*WORK_MODE= 0x80 ;
		}
		
		//SaveCodeSwStat(StrCode);
		//ChnPllSet (StrCode);
	}

	switch(RecBuf[2])
	{
		case TUNERALLWRITE:
			PC_BLL_PcToMcuTuner(RecBuf, RecLen, err);
			break;
		case TUNERALLREAD:
			PC_BLL_McuToPcTuner(RecBuf, RecLen, err);
			break;
		case FREQCHEXKSET:
			PC_BLL_PcFreqCheckSet(RecBuf, RecLen, err);
			break;
		case HIGHPOWERSET:
			PC_BLL_PcHighPowerSet(RecBuf, RecLen, err);
			break;
		case LOWPOWERSET:
			PC_BLL_PcLowPowerSet(RecBuf, RecLen, err);
			break;
		case DMODBALANCESET:
			PC_BLL_PcDModBalanceSet(RecBuf, RecLen, err);
			break;
    case MDEVNARROWSET:
      PC_BLL_PcMDevNarrowSet(RecBuf, RecLen, err);
      break;
    case MDEVWIDESET:
      PC_BLL_PcMDevWideSet(RecBuf, RecLen, err);
      break;
    case MTPLDPLNARROWSET:
      PC_BLL_PcMTPLDPLNarrowSet(RecBuf, RecLen, err);
      break;
    case MTPLDPLWIDESET:
      PC_BLL_PcMTPLDPLWideSet(RecBuf, RecLen, err);
      break;
		case TVSET:
			PC_BLL_PcTvSet(RecBuf, RecLen, err);
			break;
		case RSSITEST:
			PC_BLL_PcReadRssi(RecBuf, RecLen, err);
			break;
		//case ERRNUMRATTEST:
		//	PcReadErrNumRat(RecBuf, RecLen, err);
		//	break;
		case VOLUMESET:
			PC_BLL_PcVolumSet(RecBuf, RecLen, err);
			break;
        case SQLEVEL1OPENSET:
            PC_BLL_PcSQLevel1OpenSet(RecBuf, RecLen, err);
            break;
        case SQLEVEL1CLOSESET:
            PC_BLL_PcSQLevel1CloseSet(RecBuf, RecLen, err);
            break;
        case SQLEVEL9OPENSET:
            PC_BLL_PcSQLevel9OpenSet(RecBuf, RecLen, err);
            break;
        case SQLEVEL9CLOSESET:
            PC_BLL_PcSQLevel9CloseSet(RecBuf, RecLen, err);
            break;
        case CPUFREQADJUST:
            PC_BLL_CPUFreqAdjust(RecBuf, RecLen, err);
            break;
		default:
			break;
	}


	if(RecBuf[2] == RSSITEST || RecBuf[2] == ERRNUMRATTEST)
	{
		if(flg)
		{
			RecBuf[8] = 1;
			PC_BLL_PcReadErrNumRat(RecBuf, RecLen, err);
			flg = 0;
		}
	}
	else
	{
		if(!flg)
		{
			RecBuf[8] = 0;
			PC_BLL_PcReadErrNumRat(RecBuf, RecLen, err);
			flg = 1;
		}
	}
}


/*******************************************************************************
* 函数名: PC_BLL_PcToMcuTuner
* 描  述: PC写入调测数据
* 输  入: *RecBuf:接收数据首地址，HeadLen:接收数据长度
* 输  出: 无
* 返回值: 无
*******************************************************************************/
DH_VOID PC_BLL_PcToMcuTuner(DH_U8* Buf,DH_U16 Len, DH_U8 err)
{
	if(PCINFERR_OK==err)
	{
		FLASH_INTF_WriteStr(g_FlashSaveFlg,PCTUNERADDR, (DH_U8*)(Buf+8), (Len-9));
		PC_BLL_TunerMFToPc (Buf, (DH_U8*)0, 0, TUNERALLWRITE, UFR_RECOK);	/* 发送校验ok包 */
		ResCnt=0;
		dhtime_delay(1000);
		//DelayXMs(1000);
		AppSysReset();		/* 重启 */
		return;
	}
	else if (PCINFERR_TIMEOUT == err)
	{
		PC_BLL_TunerMFToPc (Buf, (DH_U8*)0, 0, TUNERALLWRITE, UFR_RECERR);
	}
	else if (PCINFERR_CHECKERR == err)
	{
		/* 超过最大重传次数，发送停止发送包，并退出 */
		if (++ResCnt >= PCTUNERCNTMAX) 	
		{
			//TunerMFToPc (Buf, (DH_U8*)0, 0, TUNERALLWRITE, UFR_RECSTOP1);
		}
		else
		{
			PC_BLL_TunerMFToPc (Buf, (DH_U8*)0, 0, TUNERALLWRITE, UFR_RECERR);
		}
	}
	else //if (PCINFERR_FORMATERR == err)
	{
		PC_BLL_TunerMFToPc (Buf, (DH_U8*)0, 0, TUNERALLWRITE, UFR_RECERR);
	}		
}

/*******************************************************************************
* 函数名: PC_BLL_McuToPcTuner
* 描  述: PC读取调测数据
* 输  入: *Buf:读取数据首地址，Len:数据长度
* 输  出: 无
* 返回值: 无
*******************************************************************************/
DH_VOID PC_BLL_McuToPcTuner(DH_U8* Buf,DH_U16 Len, DH_U8 err)
{
 	DH_U8 SendBuf[TUNERPARMTLEN];

	if (PCINFERR_OK == err)
	{
		if(++ResCnt<PCTUNERCNTMAX)
		{
			if (Buf[3]==0x00)
			{
				FLASH_INTF_ReadStr(g_FlashSaveFlg,PCTUNERADDR, SendBuf, TUNERPARMTLEN);
				PC_BLL_TunerMFToPc(Buf, SendBuf, TUNERPARMTLEN, TUNERALLREAD ,UFR_READDATA);
			}
			else if (Buf[3]==0x01)
			{
				ResCnt=0;
			}
			else if (Buf[3]==0xff)
			{
				FLASH_INTF_ReadStr(g_FlashSaveFlg,PCTUNERADDR, SendBuf, TUNERPARMTLEN);
				PC_BLL_TunerMFToPc(Buf, SendBuf, TUNERPARMTLEN, TUNERALLREAD, UFR_READDATA);
			}		
		}
		else
		{
			ResCnt=0;
			//TunerMFToPc (Buf, (DH_U8*)0, 0, UFR_RECSTOP1);
		}
	}
	else if (PCINFERR_TIMEOUT==err || PCINFERR_FORMATERR==err ||
		PCINFERR_CHECKERR==err)
	{
		if (++ResCnt > PCTUNERCNTMAX)		/* 超过重传次数 */
		{
			ResCnt=0;
			//TunerMFToPc (Buf, (DH_U8*)0, 0, TUNERALLREAD, UFR_RECSTOP1);
		}
		else
		{
			FLASH_INTF_ReadStr(g_FlashSaveFlg,PCTUNERADDR, SendBuf, TUNERPARMTLEN);
			PC_BLL_TunerMFToPc(Buf, SendBuf, TUNERPARMTLEN, TUNERALLREAD, UFR_READDATA);
		}
	}
}

/*******************************************************************************
* 函数名: PC_BLL_PcFreqCheckSet
* 描  述: 上位机设置收发频率校准
* 输  入: *Buf:写入数据首地址，Len:数据长度
* 输  出: 无
* 返回值: 无
*******************************************************************************/
DH_VOID PC_BLL_PcFreqCheckSet(DH_U8* Buf, DH_U16 Len, DH_U8 err)
{
	DH_U8 i;
	DH_U8 Signal[36];
	PLL_INFO_S Str;
    DH_U8 Ampbias;
    DH_U16 Afc;
    DH_U32 lu32Reg;

	if(PCINFERR_OK==err)
	{	
		memcpy ((DH_VOID*)FreqCheckBuf, (DH_VOID*)(Buf+9), 6);
		
		if(Buf[15]==0x01)	
		{
          Str= RF_BLL_GetPll();
          
          Str.u32TxFreq = 435000000;
          Str.u32RxFreq = 435000000;
          
        //Rda1846Rx(Str.PllRecValue);		/*480us*/
			
    	if(Buf[8]==0x01)	/*Tx_Amp_Bias*/
    	{
    		Str.u16AmpbiasVal=(Buf[10]<<8)+Buf[9];
    		RF_BLL_SavePll(Str);

			Ampbias = Str.u16AmpbiasVal;

			lu32Reg = *SIG_CENTER;
			lu32Reg &= 0x0000ffff;
			lu32Reg |= (Ampbias << 24);
			lu32Reg |= (Ampbias << 16);
			*SIG_CENTER= lu32Reg;

			lu32Reg = *RF_MODE_BIAS_CTRL;
			lu32Reg &= 0XFFFF0000;
			lu32Reg |= (Str.u8Mod1 << 8) + Str.u8Mod1;
			*RF_MODE_BIAS_CTRL= lu32Reg;

    		for(i=0;i<36;i++)
    		{
    			Signal[i] = 0x5f;
    		}

    		*SYS_SOFT_RSTN= ~0x03;
    		*LAYER2_CONTROL= 0x82;
    		*LAYER2_TXRX_CTRL= 0x81;
    		*WORK_MODE= 0x02;
    		HRC_INTF_WriteTxRam(0x00,(DH_U8*)Signal,36);
    		HRC_INTF_WriteTxRam(0x24,(DH_U8*)Signal,36);
    	}
    	else if(Buf[8]==0x02)	/*Rx_Amp_Bias*/
    	{
            PC_BLL_PcAfcAutCreate(Buf+9,&Ampbias,&Str.u16AfcVal);
            RF_BLL_SavePll(Str);

            *LAYER2_TXRX_CTRL= 0x00;
            *LAYER2_CONTROL= 0x00;
            *WORK_MODE= 0x13;
            *SYS_SOFT_RSTN= ~0x03;

            Afc = Str.u16AfcVal;

            lu32Reg=*RF_MODE_BIAS_CTRL;
            lu32Reg &= 0x0000ffff;
            lu32Reg |= Afc;
            *RF_MODE_BIAS_CTRL= lu32Reg;
    	}

				RF_BLL_EnableTx();
				
		}
		else
		{
           
           Afc = Str.u16AfcVal;
           
           lu32Reg = *RF_MODE_BIAS_CTRL;
           lu32Reg &= 0x0000ffff;
           lu32Reg |= Afc;
           *RF_MODE_BIAS_CTRL= lu32Reg;
           
           RF_BLL_EnableRx();		/*480us*/	
		}
	}
	else
	{
		PC_BLL_TunerMFToPc(Buf, (DH_U8*)0, 0, FREQCHEXKSET, UFR_RECERR);
	}
}

/*******************************************************************************
* 函数名: PC_BLL_PcHighPowerSet
* 描  述: 上位机设置高功率
* 输  入: *Buf:写入数据首地址，Len:数据长度
* 输  出: 无
* 返回值: 无
*******************************************************************************/
DH_VOID PC_BLL_PcHighPowerSet(DH_U8* Buf, DH_U16 Len, DH_U8 err)
{
	//DH_U16 SkyBuf[10];
	DH_U8 i;
	DH_U8 Signal[36];
	PLL_INFO_S StrSnd;
    DH_U8 Ampbias;
    DH_U16 Afc;
    DH_U32 lu32Reg;
  
	if(PCINFERR_OK==err)
	{
		StrSnd= RF_BLL_GetPll();

		if((Buf[8]>0)&&(Buf[8]<8))
		{
			
StrSnd.u32TxFreq = gTestFREQList[Buf[8]-1];
			 StrSnd.u32RxFreq = gTestFREQList[Buf[8]-1];

			 StrSnd.u16ApcHighVal=(Buf[2*Buf[8]+8]<<8)+Buf[2*Buf[8]+7];
		}
		else
		{
			StrSnd.u32TxFreq = 0;
			StrSnd.u32RxFreq = 0;
			StrSnd.u16ApcHighVal = 0;
		}
		
    StrSnd.u16ApcVal= StrSnd.u16ApcHighVal;
    RF_BLL_SavePll(StrSnd);
    Ampbias = StrSnd.u16AmpbiasVal;
    Afc = StrSnd.u16AfcVal;
		if(Buf[23]==0x01)
		{
      lu32Reg = *SIG_CENTER;
      lu32Reg &= 0x0000ffff;
      lu32Reg |= (Ampbias << 24);
      lu32Reg |= (Ampbias << 16);
      *SIG_CENTER= lu32Reg;

      *RF_MODE_BIAS_CTRL= lu32Reg;
      lu32Reg &= 0XFFFF0000;
      lu32Reg |= (StrSnd.u8Mod1 << 8) + StrSnd.u8Mod1;
      *RF_MODE_BIAS_CTRL= lu32Reg;

      RF_BLL_EnableTx();

			for(i=0;i<36;i++)
			{
				Signal[i] = 0x5f;
			}

			  *SYS_SOFT_RSTN= ~0x03;
    		*LAYER2_CONTROL= 0x82;
    		*LAYER2_TXRX_CTRL= 0x81;
    		*WORK_MODE= 0x02;
    		HRC_INTF_WriteTxRam(0x00,(DH_U8*)Signal,36);
    		HRC_INTF_WriteTxRam(0x24,(DH_U8*)Signal,36);
		}
		else
		{
        lu32Reg=*RF_MODE_BIAS_CTRL;
        lu32Reg &= 0x0000ffff;
        lu32Reg |= Afc;
        *RF_MODE_BIAS_CTRL= lu32Reg;

        RF_BLL_EnableRx();
		}
	}	
	else
	{
		PC_BLL_TunerMFToPc(Buf, (DH_U8*)0, 0, HIGHPOWERSET, UFR_RECERR);
	}
}

/*******************************************************************************
* 函数名: PC_BLL_PcLowPowerSet
* 描  述: 上位机设置低功率
* 输  入: *Buf:写入数据首地址，Len:数据长度
* 输  出: 无
* 返回值: 无
*******************************************************************************/
DH_VOID PC_BLL_PcLowPowerSet(DH_U8* Buf, DH_U16 Len, DH_U8 err)
{
	DH_U8 i;
	DH_U8 Signal[36];
	PLL_INFO_S StrSnd;
    DH_U8 Ampbias;
    DH_U16 Afc;
    DH_U32 lu32Reg;
  
	if(PCINFERR_OK==err)
	{		
		StrSnd= RF_BLL_GetPll();

		if((Buf[8]>0)&&(Buf[8]<8))
		{
			
StrSnd.u32TxFreq = gTestFREQList[Buf[8]-1];
			 StrSnd.u32RxFreq = gTestFREQList[Buf[8]-1];

			 StrSnd.u16ApcLowVal=(Buf[2*Buf[8]+8]<<8)+Buf[2*Buf[8]+7];
		}
		else
		{
			StrSnd.u32TxFreq = 0;
			StrSnd.u32RxFreq = 0;
			StrSnd.u16ApcLowVal = 0;
		}
		
    StrSnd.u16ApcVal= StrSnd.u16ApcLowVal;
		RF_BLL_SavePll(StrSnd);
    Ampbias = StrSnd.u16AmpbiasVal;
    Afc = StrSnd.u16AfcVal;
      
		if(Buf[23]==0x01)
		{
      lu32Reg=SIG_CENTER;
      lu32Reg &= 0x0000ffff;
      lu32Reg |= (Ampbias << 24);
      lu32Reg |= (Ampbias << 16);
      *SIG_CENTER=lu32Reg;

      lu32Reg=*RF_MODE_BIAS_CTRL;
      lu32Reg &= 0XFFFF0000;
      lu32Reg |= (StrSnd.u8Mod1 << 8) + StrSnd.u8Mod1;
      *RF_MODE_BIAS_CTRL= lu32Reg;

      RF_BLL_EnableTx();
    
			for(i=0;i<36;i++)
			{
				Signal[i] = 0x5f;
			}

			  *SYS_SOFT_RSTN= ~0x03;
    		*LAYER2_CONTROL=0x82;
    		*LAYER2_TXRX_CTRL=0x81;
    		*WORK_MODE=0x02;
    		HRC_INTF_WriteTxRam(0x00,(DH_U8*)Signal,36);
    		HRC_INTF_WriteTxRam(0x24,(DH_U8*)Signal,36);
		}
		else
		{
        lu32Reg=*RF_MODE_BIAS_CTRL;
        lu32Reg &= 0x0000ffff;
        lu32Reg |= Afc;
        *RF_MODE_BIAS_CTRL=lu32Reg;

        RF_BLL_EnableRx();
		}
	}
	else
	{
		PC_BLL_TunerMFToPc(Buf, (DH_U8*)0, 0, LOWPOWERSET, UFR_RECERR);
	}
}

/*******************************************************************************
* 函数名: PC_BLL_PcDModBalanceSet
* 描  述: 上位机设置数字两点平衡
* 输  入: *Buf:写入数据首地址，Len:数据长度
* 输  出: 无
* 返回值: 无
*******************************************************************************/
DH_VOID PC_BLL_PcDModBalanceSet(DH_U8* Buf, DH_U16 Len, DH_U8 err)
{
	DH_U8 i;
	DH_U8 Signal[36];
	PLL_INFO_S StrSnd;
    DH_U8 Ampbias;
    DH_U16 Afc;

    DH_U32 lu32Reg;
  
	if(PCINFERR_OK==err)
	{		
		StrSnd= RF_BLL_GetPll();

		if((Buf[8]>0)&&(Buf[8]<8))
		{
			
StrSnd.u32TxFreq = gTestFREQList[Buf[8]-1];
			 StrSnd.u32RxFreq = gTestFREQList[Buf[8]-1];

			 StrSnd.u8Mod2 = Buf[2*Buf[8]+7];
			 StrSnd.u8Mod1 = Buf[2*Buf[8]+8];
		}
		else
		{
			StrSnd.u32TxFreq = 0;
			StrSnd.u32RxFreq = 0;
			StrSnd.u8Mod2 = 0;
			StrSnd.u8Mod1 = 0;
		}
		
		RF_INTF_SetRx(StrSnd.u32RxFreq);    /*480us*/ 
	  RF_BLL_SavePll(StrSnd);
    Ampbias = StrSnd.u16AmpbiasVal;
    Afc = StrSnd.u16AfcVal;
      
		if(Buf[26]==0x01)
		{
      lu32Reg = *SIG_CENTER;
      lu32Reg &= 0x0000ffff;
      lu32Reg |= (Ampbias << 24);
      lu32Reg |= (Ampbias << 16);

      *SIG_CENTER = lu32Reg;

      lu32Reg = *RF_MODE_BIAS_CTRL;
      lu32Reg &= 0XFFFF0000;
      lu32Reg |= (StrSnd.u8Mod1 << 8) + StrSnd.u8Mod1;
      *RF_MODE_BIAS_CTRL = lu32Reg;

      RF_BLL_EnableTx();


			if(Buf[25]==0x01)			/*+3*/
			{
				for(i=0;i<36;i++)
				{
					Signal[i] = 0x55;
				}	

				HRC_INTF_WriteTxRam(0x00,(DH_U8*)Signal,36);
    		HRC_INTF_WriteTxRam(0x24,(DH_U8*)Signal,36);
		
			}
			else if(Buf[25]==0x02)		/*-3*/
			{
				for(i=0;i<36;i++)
				{
					Signal[i] = 0xff;
				}		
	
				HRC_INTF_WriteTxRam(0x00,(DH_U8*)Signal,36);
    		HRC_INTF_WriteTxRam(0x24,(DH_U8*)Signal,36);
		
			}
			else if(Buf[25]==0x03)		/*+3-3*/
			{
				for(i=0;i<36;i++)
				{
					Signal[i] = 0x5f;
				}

				HRC_INTF_WriteTxRam(0x00,(DH_U8*)Signal,36);
    		HRC_INTF_WriteTxRam(0x24,(DH_U8*)Signal,36);
		
			}
			else if(Buf[25]==0x04)			/*随机数*/
			{
				static   DH_U8 stochastic[36] = {0x80,0x00,0x1D,0x10,0x16,0x80,0x19,0xE0,0x62,0xE0,
             0x0F,0x81,0xE2,0x10,0x5E,0xD5,0xD7,0xF7,0x7F,0xD7,0x57,0x09,0x40,0x82,0x17,0x20,0x06,
             0x20,0x80,0xD1,0xF5,0x00,0xA0,0x03,0x30,0x01};

				HRC_INTF_WriteTxRam(0x00,(DH_U8*)Signal,36);
    		HRC_INTF_WriteTxRam(0x24,(DH_U8*)Signal,36);
		
			}

			  *SYS_SOFT_RSTN= ~0x03;
    		*LAYER2_CONTROL=0x82;
    		*LAYER2_TXRX_CTRL=0x81;
    		*WORK_MODE=0x02;

		}
		else
		{
        lu32Reg = *RF_MODE_BIAS_CTRL;
        lu32Reg &= 0x0000ffff;
        lu32Reg |= Afc;
        *RF_MODE_BIAS_CTRL = lu32Reg;

        RF_BLL_EnableRx();
		}
	}
	else
	{
		PC_BLL_TunerMFToPc(Buf, (DH_U8*)0, 0, DMODBALANCESET, UFR_RECERR);
	}
}


/*******************************************************************************
* 函数名: PC_BLL_PcMDevNarrowSet
* 描  述: 上位机设置模拟两点平衡
* 输  入: *Buf:写入数据首地址，Len:数据长度
* 输  出: 无
* 返回值: 无
*******************************************************************************/
DH_VOID PC_BLL_PcMDevNarrowSet(DH_U8* Buf, DH_U16 Len, DH_U8 err)
{
	if(PCINFERR_OK==err)
	{
	}
	else
	{
		PC_BLL_TunerMFToPc(Buf, (DH_U8*)0, 0, MDEVNARROWSET, UFR_RECERR);
	}
}

/*******************************************************************************
* 函数名: PC_BLL_PcMDevWideSet
* 描  述: 上位机设置模拟调制频偏(宽带)
* 输  入: *Buf:写入数据首地址，Len:数据长度
* 输  出: 无
* 返回值: 无
*******************************************************************************/
DH_VOID PC_BLL_PcMDevWideSet(DH_U8* Buf, DH_U16 Len, DH_U8 err)
{
	if(PCINFERR_OK==err)
	{
	}
	else
	{
		PC_BLL_TunerMFToPc(Buf, (DH_U8*)0, 0, MDEVWIDESET, UFR_RECERR);
	}
}

/*******************************************************************************
* 函数名: PC_BLL_PcMTPLDPLNarrowSet
* 描  述: 上位机设置模拟亚音调制频偏(窄带)
* 输  入: *Buf:写入数据首地址，Len:数据长度
* 输  出: 无
* 返回值: 无
*******************************************************************************/
DH_VOID PC_BLL_PcMTPLDPLNarrowSet(DH_U8* Buf, DH_U16 Len, DH_U8 err)
{
	if(PCINFERR_OK==err)
	{

	}
	else
	{
		PC_BLL_TunerMFToPc(Buf, (DH_U8*)0, 0, MTPLDPLNARROWSET, UFR_RECERR);
	}
}

/*******************************************************************************
* 函数名: PC_BLL_PcMTPLDPLWideSet
* 描  述: 上位机设置模拟亚音调制频偏(宽带)
* 输  入: *Buf:写入数据首地址，Len:数据长度
* 输  出: 无
* 返回值: 无
*******************************************************************************/
DH_VOID PC_BLL_PcMTPLDPLWideSet(DH_U8* Buf, DH_U16 Len, DH_U8 err)
{

	if(PCINFERR_OK==err)
	{

	}
	else
	{
		PC_BLL_TunerMFToPc(Buf, (DH_U8*)0, 0, MTPLDPLWIDESET, UFR_RECERR);
	}
}

/*******************************************************************************
* 函数名: PC_BLL_PcTvSet
* 描  述: 上位机设置电调
* 输  入: *Buf:写入数据首地址，Len:数据长度
* 输  出: 无
* 返回值: 无
*******************************************************************************/
DH_VOID PC_BLL_PcTvSet(DH_U8* Buf, DH_U16 Len, DH_U8 err)
{
	PLL_INFO_S Str;
	DH_U8 Ampbias;
	DH_U16 Afc;
  DH_U32 lu32Reg;
		
	if(PCINFERR_OK==err)
  {
		Str= RF_BLL_GetPll();

		if((Buf[8]>0)&&(Buf[8]<8))
		{

Str.u32TxFreq = gTestFREQList[Buf[8]-1];
			 Str.u32RxFreq = gTestFREQList[Buf[8]-1];

			 Str.u16TvVal=(Buf[2*Buf[8]+8]<<8)+Buf[2*Buf[8]+7];
		}
		else
		{
			Str.u32TxFreq = 0;
			Str.u32RxFreq = 0;
			Str.u16TvVal = 0;
		}
			
      RF_BLL_SavePll(Str);
      Afc = Str.u16AfcVal;

      RF_INTF_TVSet(Str.u16TvVal);

      lu32Reg = *SIG_CENTER;
      lu32Reg &= 0x0000ffff;
      lu32Reg |= ((Afc&0xff) << 8);
      lu32Reg |= (Afc&0xff);
      *SIG_CENTER= lu32Reg;

      RF_BLL_EnableRx();
	}
	else
	{
		PC_BLL_TunerMFToPc(Buf, (DH_U8*)0, 0, TVSET, UFR_RECERR);
	}
}

/*******************************************************************************
* 函数名: PC_BLL_PcReadRssi
* 描  述: Pc读取RSSI值
* 输  入: *Buf:读取数据首地址，Len:数据长度
* 输  出: 无
* 返回值: 无
*******************************************************************************/
DH_VOID PC_BLL_PcReadRssi(DH_U8* Buf, DH_U16 Len, DH_U8 err)
{
	DH_U16 SignalIntensAdStat = 0;	/*从AD得到的AD值*/
	//DH_U8 i;
	//DH_U16 SignalValue;
	DH_U8 SBuf[3];

	if(PCINFERR_OK==err)
	{
		if(Buf[8]==0x01)
		{
			//for (i = 0; i < 3; i++)
			{
				//SignalIntensAdStat = Rssi_NoiseGet()>>8;

				//SignalIntensAdStat += SignalValue;
				//OSTimeDlyHMSM(0, 0, 0, 10);
			}
			//SignalIntensAdStat=(DH_U16)(SignalIntensAdStat/3);
			SBuf[0]=0x02;
			SBuf[1]=(DH_U8)(SignalIntensAdStat);
			SBuf[2]=(DH_U8)(SignalIntensAdStat>>8);
			
			PC_BLL_TunerMFToPc(Buf, SBuf, 3, RSSITEST, 0x00);
		}
	}
	else
	{
		PC_BLL_TunerMFToPc(Buf, (DH_U8*)0, 0, RSSITEST, UFW_RECERR);
	}
}

/*******************************************************************************
* 函数名: PC_BLL_PcReadErrNumRat
* 描  述: 上位机查询误码率
* 输  入: *Buf:读取数据首地址，Len:数据长度
* 输  出: 无
* 返回值: 无
*******************************************************************************/
DH_VOID PC_BLL_PcReadErrNumRat(DH_U8*Buf, DH_U16 Len, DH_U8 err)
{
	if(PCINFERR_OK==err)
	{
		if(Buf[8]==0x01)
		{
       *SYS_SOFT_RSTN= ~0x03;
       *LAYER2_CONTROL=0x40;
       *WORK_MODE= 0x22;
       *LAYER2_TXRX_CTRL= 0xc1;
       ErrNumbTestFlag=1;
		}
		else
		{
			ErrNumbTestFlag=0;

      *LAYER2_CONTROL = 0x00;
      *LAYER2_TXRX_CTRL= 0x00;
      *SYS_SOFT_RSTN = ~0x03;
      *WORK_MODE=0x13;
		}
	}
	else
	{
		PC_BLL_TunerMFToPc(Buf, (DH_U8*)0, 0, ERRNUMRATTEST, UFW_RECERR);
	}
}

/*******************************************************************************
* 函数名: PC_BLL_PcVolumSet
* 描  述: 上位机设置音量
* 输  入: *Buf:读取数据首地址，Len:数据长度
* 输  出: 无
* 返回值: 无
*******************************************************************************/
DH_VOID PC_BLL_PcVolumSet(DH_U8*Buf, DH_U16 Len, DH_U8 err)
{
	//DH_U8 data;
	//data=Buf[8];
	
	if(PCINFERR_OK==err)
	{
		PC_BLL_TunerMFToPc(Buf, (DH_U8*)0, 0, VOLUMESET, UFW_RECOK);
	}
	else
	{
		PC_BLL_TunerMFToPc(Buf, (DH_U8*)0, 0, VOLUMESET, UFW_RECERR);
	}
}

/*******************************************************************************
* 函数名: PC_BLL_PcSQLevel1OpenSet
* 描  述: 上位机设置静噪等级1开启
* 输  入: *Buf:写入数据首地址，Len:数据长度
* 输  出: 无
* 返回值: 无
*******************************************************************************/
DH_VOID PC_BLL_PcSQLevel1OpenSet(DH_U8*Buf, DH_U16 Len, DH_U8 err)
{
	if(PCINFERR_OK==err)
	{
	}
	else
	{
		PC_BLL_TunerMFToPc(Buf, (DH_U8*)0, 0, SQLEVEL1OPENSET, UFR_RECERR);
	}
}

/*******************************************************************************
* 函数名: PC_BLL_PcSQLevel1CloseSet
* 描  述: 上位机设置静噪等级1关闭
* 输  入: *Buf:写入数据首地址，Len:数据长度
* 输  出: 无
* 返回值: 无
*******************************************************************************/
DH_VOID PC_BLL_PcSQLevel1CloseSet(DH_U8*Buf, DH_U16 Len, DH_U8 err)
{
	if(PCINFERR_OK==err)
	{
	}
	else
	{
		PC_BLL_TunerMFToPc(Buf, (DH_U8*)0, 0, SQLEVEL1CLOSESET, UFR_RECERR);
	}
}

/*******************************************************************************
* 函数名: PC_BLL_PcSQLevel9OpenSet
* 描  述: 上位机设置静噪等级9开启
* 输  入: *Buf:写入数据首地址，Len:数据长度
* 输  出: 无
* 返回值: 无
*******************************************************************************/
DH_VOID PC_BLL_PcSQLevel9OpenSet(DH_U8*Buf, DH_U16 Len, DH_U8 err)
{
	if(PCINFERR_OK==err)
	{

	}
	else
	{
		PC_BLL_TunerMFToPc(Buf, (DH_U8*)0, 0, SQLEVEL9OPENSET, UFR_RECERR);
	}
}

/*******************************************************************************
* 函数名: PC_BLL_PcSQLevel9CloseSet
* 描  述: 上位机设置静噪等级9关闭
* 输  入: *Buf:写入数据首地址，Len:数据长度
* 输  出: 无
* 返回值: 无
*******************************************************************************/
DH_VOID PC_BLL_PcSQLevel9CloseSet(DH_U8*Buf, DH_U16 Len, DH_U8 err)
{

}

/*******************************************************************************
* 函数名: PC_BLL_CPUFreqAdjust
* 描  述: 上位机设置CPU频率校准
* 输  入: *Buf:写入数据首地址，Len:数据长度
* 输  出: 无
* 返回值: 无
*******************************************************************************/
DH_VOID PC_BLL_CPUFreqAdjust(DH_U8*Buf, DH_U16 Len, DH_U8 err)
{
	if(PCINFERR_OK==err)
	{

	}
	else
	{
		PC_BLL_TunerMFToPc(Buf, (DH_U8*)0, 0, CPUFREQADJUST, UFR_RECERR);
	}
}

/*******************************************************************************
* 函数名: PC_BLL_TunerMFToPc
* 描  述: MCU组射频调测帧发送给PC
* 输  入: *DstBuf:结果数据缓存首地址，*SourBuf:源数据缓存首地址，Len:数据长度
*         Opt:选项信息，Ack:应答信息
* 输  出: 无
* 返回值: 无
*******************************************************************************/
DH_VOID PC_BLL_TunerMFToPc(DH_U8* DstBuf, DH_U8* SourBuf, DH_U16 Len, DH_U8 Opt, DH_U8 Ack)
{
	DH_U16 CheckSum;
	
	DstBuf[0] = 0x68;
	DstBuf[1] = UART_TUNER_TEST;
	DstBuf[2] = Opt;
	DstBuf[3] = Ack;
	
	DstBuf[4] = DstBuf[5] = 0;			/* check sum */
	
	DstBuf[6] = Len%256;
	DstBuf[7] = Len/256;

	DstBuf[8+Len] = 0x10;

	/* 存储数据 */
	if (Len)
	{
		memcpy ((DH_VOID*)(DstBuf+8), (DH_VOID*)SourBuf, Len);
	}
	
	/* 填充校验位 */
	CheckSum = PC_BLL_PcCheckSum(DstBuf, 9+Len);

	DstBuf[4] = CheckSum>>8;
	DstBuf[5] = CheckSum;	

	PCLINK_INTF_TransmitFrame(DstBuf, 9+Len);

	return;
	
}

/*******************************************************************************
* 函数名: PC_BLL_TunerGetPcFrm
* 描  述: 获取一帧pc 数据帧
* 输  入: *DstBuf:数据缓存首地址，PLen:数据帧长度指针	(返回值为ok时有效)
*         TimeOut:等待时间(为0表示一直等待，OS_TICKS_PER_SEC为1s)，HeadLen:读取的长度
* 输  出: 无
* 返回值: 读取结果
*******************************************************************************/
DH_U8 PC_BLL_TunerGetPcFrm( DH_U8* DstBuf, DH_U16* PLen, DH_U32 TimeOut, DH_U32 HeadLen)
{
	DH_U16 ChckSum;
	DH_U16 RecLenAll = DstBuf[UART_DATAMODE_LENH]*0x100+DstBuf[UART_DATAMODE_LENL]+9;
#if 1	
	if (HeadLen<RecLenAll)
	{
		*PLen = PCLINK_INTF_ReceiveFrame (DstBuf+HeadLen, RecLenAll-HeadLen, TimeOut);		/* 获取数据 */
		*PLen += HeadLen;
	}

	if (*PLen)			/* 获取到数据 */
	{
		if (*PLen>=UARTFORMATLEN && DstBuf[0]==0x68 && DstBuf[*PLen-1]==0x10)
		{
			ChckSum = PC_BLL_PcCheckSum(DstBuf, *PLen);
			if (ChckSum)
			{
				return PCINFERR_CHECKERR;
			}
			else
			{
				return PCINFERR_OK;
			}
		}
		else			/* 格式错误 */	
		{
			return PCINFERR_FORMATERR;
		}
	}
	else				/* 等待超时 */
	{
		return PCINFERR_TIMEOUT;
	}
#else
return PCINFERR_OK;
#endif
}
#endif

#endif

