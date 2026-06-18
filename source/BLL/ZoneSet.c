#include "app_config.h"
#include "include.h"

uint8_t g_u8ChnListFlag[2];
uint8_t g_u8ScanChnListFlag[2];

LIST_MG_STR str_ChnList ;
LIST_MG_STR str_ScanChnList ;


void getchninfo(CHN_INFO_S  *pchninfo,uint8*buf)
{
	
	
}

void ZoneChnInit(void)
{
	int32 Send_Freq,Rec_Freq;
	uint8_t j,Buf[PCEA_CHN_LEN];

	CHN_INFO_S  chninfo;
	
	str_ChnList.grp = g_u8ChnListFlag;
	str_ChnList.valid = 0;
	str_ChnList.size = ZONE_CHN_MAX;
	str_ScanChnList.grp = g_u8ScanChnListFlag;
	str_ScanChnList.valid = 0;
	str_ScanChnList.size = ZONE_CHN_MAX;
	

	memset(g_u8ChnListFlag,0,sizeof(g_u8ChnListFlag));
	memset(g_u8ScanChnListFlag,0,sizeof(g_u8ScanChnListFlag));	



	for (j=0; j<ZONE_CHN_MAX; j++)
	{	
		FLASH_INTF_ReadStr(PCEA_CHN_LEN*(j), &chninfo, sizeof(CHN_INFO_S));
		//memcpy ((void*)&Send_Freq, (void*)&Buf[PCEA_SENDFREQ], sizeof(int32));
		//memcpy ((void*)&Rec_Freq,  (void*)&Buf[PCEA_RECFREQ], sizeof(int32));
	
		if(    chninfo.u32RxFreq > FREQ_UHF_H || chninfo.u32RxFreq < FREQ_UHF_L
		    || chninfo.u32RxFreq > FREQ_UHF_H || chninfo.u32TxFreq < FREQ_UHF_L)
		{
		}
		else
		{		
			str_ChnList.grp[j/8] |= BITSET(j%8);
			str_ChnList.valid ++;
			getchninfo(&chninfo, Buf);
			if(chninfo.scanflg)
			{
				str_ScanChnList.grp[j/8] |= BITSET(j%8);
				str_ScanChnList.valid ++;
				
			}
		}
	}
}

/*sft 发射频率计算*/
void FreqAll_GetSndFreq(uint32 recfreq)
{


}


 
uint16 CHN_APP_GetBaseAddr(uint8 CCnt)
{
//	if(CCnt==FREQALL_BASECHN)return PCWDBADDR;
//	else if(CCnt==FREQALL_BASECHN+1)return (PCWDBADDR+PCEA_CHN_LEN);
//	else return PECA_CHNUM_BASEADDR+PCEA_CHN_LEN*(CCnt);
}


uint16 CHN_APP_GetFreqBaseAddr(uint8 bandflg)
{
///	return CHN_APP_GetBaseAddr(FREQALL_BASECHN+bandflg);
}

void CHN_APP_GetChnInfo(struct _chninfo_  *pchninfo,uint8 CCnt)
{
//	uint8 buf[PCEA_CHN_LEN];
//
//	E2PROMReadStr(CHN_APP_GetBaseAddr(CCnt), buf, PCEA_CHN_LEN);
//	pchninfo->Numb = CCnt;
//	if(pchninfo->Numb>=FREQALL_BASECHN){buf[0] = CCnt;	}
//	getchninfo(pchninfo, buf);
}


uint8_t CHN_APP_GetChnNum(void)
{
	return str_ChnList.valid;
}

uint8_t CHN_APP_ChnValidChk(uint8_t chn) //0~chnmax
{
	return (str_ChnList.grp[chn/8] & BITSET(chn%8))?1:0;
}

uint8_t CHN_APP_GetNextValidIndex(uint8_t curIndex,uint8_t dir)
{
	return list_GetNextValidIndex(&str_ChnList,curIndex,dir);
}

uint8_t CHN_APP_GetFirstValidChn(void)
{
	uint8_t chn= list_GetFirstValidIndex(&str_ChnList);
	if(chn==0xff)//无有效
	{
		return 0;
	}else{
		return chn;
	}
}

uint8_t CHN_APP_DelChn(uint8_t chn)
{
	return list_DelValidIndex(chn,&str_ChnList);
}

uint8_t CHN_APP_AddChn(uint8_t chn)
{
	return list_AddValidIndex(chn,&str_ChnList);
}

uint8_t Scan_APP_GetChnIndex(uint8_t item)
{
	return list_GetValidIndex(item,&str_ScanChnList);
}

uint8_t Scan_APP_GetChnNum(void)
{
	return str_ScanChnList.valid;
}

uint8_t Scan_APP_DelChn(uint8_t chn)
{
	return list_DelValidIndex(chn,&str_ScanChnList);
}

//0可用於優先信道確認
uint8_t Scan_APP_ChnValidChk(uint8_t chn) //0~chnmax
{
	return (str_ScanChnList.grp[chn/8] & BITSET(chn%8))?1:0;
}

#if 0
/*****************************************************************************
 *   函数名: ChnSet
 *   功能描述:code sw 旋钮频道设置
 *   参数定义:	ZCnt为区域号，CCnt为频道号码1-16为数字信道，17-32为模拟信道
 *	返回值:
 *   创建人：陶林，2011/08/05
******************************************************************************/
void ChnSetInit(void)
{
	struct _chninfo_  chninfo;
	uint8 CCntA,i;
	uint8 CCnTemp[2];
//	uint8 flag=StrMenuSet.MainBand;

	INTF_ChnLock();
	
#if(FREQ_TYPE_SEL==FREQ_TYPE_UV)
	for(i=0;i<2;i++)
#else
	for(i=0;i<1;i++)
#endif
	{
		StrMenuSet.MainBand=i;
		if(!CHN_APP_GetChnNum())
		{
		#if LCD_TYPE_SEL == LCD_TYPE_NONE			
			StrMMenuSetAB[i].DispMode = DISPFMMODE_Numb;
		#else
			StrMMenuSetAB[i].DispMode = DISPFMMODE_FreqALL;	
		#endif
		}
		else
		{
		
		#if USE_CHN_CODESW==CHN_CODESW_16MAX
			CCntA = INTF_GetFreqStat();
		#else
			E2PROMReadStr(PCBA_CCNTSAVEA+i*2, CCnTemp, 2);	
			if(CCnTemp[1]==0x5a)
				CCntA=CCnTemp[0];
		
			if(!CHN_APP_ChnValidChk(CCntA))//無效
			{
				CCntA = CHN_APP_GetFirstValidChn();
				CCnTemp[0]=CCntA;
				CCnTemp[1]=0x5a;
				E2PROMWriteStr(PCBA_CCNTSAVEA+i*2, CCnTemp, 2);
			}
		#endif
			INTF_SetCodeSwStat(CCntA);
			INTF_SetFreqStat(CCntA);
		
			PLLStat_Num[i]=CCntA;
		
			CHN_APP_GetChnInfo(&chninfo,CCntA);
		
			SavePllChnInfo(StrMenuSet.MainBand,&chninfo);
		}
		if((StrMMenuSetAB[i].DispMode == DISPFMMODE_FreqALL))
		{
			Freqall_EnterInit();
		}
	}

	StrMenuSet.MainBand=0;
	Set_ReflashRxChannelSwFlagState(1);//刷新一下接收切换开关.

	INTF_ChnUnlock();
}
/*****************************************************************************
 *   函数名: ChnSet
 *   功能描述:code sw 旋钮频道设置
 *   参数定义:	ZCnt: 1~0x10,
 *					CCnt: 1~0x10
 *	返回值:
 *   创建人：陶林，2011/08/05
******************************************************************************/
void ChnSet(uint8 CCnt)
{
	uint8 NeedReflashRx_flag=0;

	STR_CHNINFO chninfo=GetPllChnInfo(StrMenuSet.MainBand);
	INTF_ChnLock();

	StrMMenuSetAB[StrMenuSet.MainBand].ReverseFreqOn = 0;
	
	{
		chninfo.Numb=CCnt;

		INTF_SetRealCode(StrMenuSet.MainBand,CCnt);
		
		INTF_SetCodeSwStat(CCnt);
		INTF_SetFreqStat(CCnt);

		
		DRV_RedLedOff();
		DRV_GreenLedOff();//在切换时灯全部先关闭
		/* 切换前做相应处理 */
		if(GloVarFlag.KeyFlag==1)	/*旋转频道时如果当前在发送则停止发送*/
		{
			GloVarFlag.KeyFlag=0;
			DRV_RedLedOff();
			DRV_GreenLedOff();
			INTF_FmRecvEn();	
		}
		else if(GloVarFlag.FmRecStat==1)
		{
			SQ_Check_Enable=1;
			if(GloVarFlag.FmMontFlg==1)
			{
				GloVarFlag.FmMontFlg=0;
			}
			INTF_FoceFmVoiceStop();
			g_msenden[AllFlag.Rx_BandSelectFlag] = 0;
			GloVarFlag.FmRecStat=0;
		//	osDelay(50);
			NeedReflashRx_flag=1;
		}

		if (DRV_RdaSqStat())	/* 高电平 :接收中... */
		{
			NeedReflashRx_flag=1;
		}
		/*处于声控发射则关闭*/
		if(GloVarFlag.VoxFlg==1)
		{
			INTF_VoxChnChangeStop();
		}
		CloseCtcssTimeInt();
		CloseCdcssTimeInt();	
		
		CHN_APP_GetChnInfo(&chninfo,CCnt);

		SavePllChnInfo(StrMenuSet.MainBand,&chninfo);

		if(g_fmonitorflag&&chninfo.Attr == PLLSTAT_M)
		{
			NeedReflashRx_flag=1;
		}
	}
	
	if(NeedReflashRx_flag)
	{
		Need_ReflashRX_Directory();
	}
	else
	{
		if(StrMenuSet.Displaymode==DISPFMMODE_Single ) SetRx_OnArrowBand();
		Set_ReflashRxChannelSwFlagState(1);
	}//刷新一下接收切换开关.
	INTF_ChnUnlock();
	
}

uint8 Freq_UV(uint32 Freq)
{
	if((Freq<=Get_UFreqMax())&&(Freq>=(Get_UFreqMin())))
	{
		return FREQ_U_FLAG;
	}
//	else if((Freq<=(Get_VFreqMax()))&&(Freq>=(Get_VFreqMin())))
//	{
//
//		return FREQ_V_FLAG;
//	}
	return FREQ_U_FLAG;
}


BOOLEAN Check_Freq_scope(uint32 Freq,uint8 Flag)
{//flag是RX或者TX的区分.0=RX
	if(!Flag)
	{
		StrMMenuSetAB[StrMenuSet.MainBand].RX_UVSelectFlag=Freq_UV(Freq);
	}
	else
	{
		StrMMenuSetAB[StrMenuSet.MainBand].TX_UVSelectFlag=Freq_UV(Freq);
	}
	if((Freq<=Get_UFreqMax())&&(Freq>=(Get_UFreqMin())))
	{
		return TRUE;
	}
//	if((Freq<=(Get_VFreqMax()))&&(Freq>=(Get_VFreqMin())))
//	{
//		if(!Flag)
//		{
//			StrMMenuSetAB[StrMenuSet.MainBand].RX_UVSelectFlag=FREQ_V_FLAG;
//		}
//		else
//		{
//			StrMMenuSetAB[StrMenuSet.MainBand].TX_UVSelectFlag=FREQ_V_FLAG;
//		}
	
//		return TRUE;
//	}
	return FALSE;
}

BOOLEAN Check_SelectBand_Freq_scope(uint32 Freq,uint8 RxTxFlag,uint8 BandFlag)
{//flag是RX或者TX的区分.0=RX
	if((Freq<=Get_UFreqMax())&&(Freq>=(Get_UFreqMin())))
	{
		if(!RxTxFlag)
		{
			StrMMenuSetAB[BandFlag].RX_UVSelectFlag=FREQ_U_FLAG;
		}
		else
		{
			StrMMenuSetAB[BandFlag].TX_UVSelectFlag=FREQ_U_FLAG;
		}
		return TRUE;
	}
//	if((Freq<=(Get_VFreqMax()))&&(Freq>=(Get_VFreqMin())))
//	{
//		if(!RxTxFlag)
//		{
//			StrMMenuSetAB[BandFlag].RX_UVSelectFlag=FREQ_V_FLAG;
//		}
//		else
//		{
//			StrMMenuSetAB[BandFlag].TX_UVSelectFlag=FREQ_V_FLAG;
//		}	
//		return TRUE;
//	}
	return FALSE;
}

//静噪等级分档设置
//void SqLevel_set(STR_CODESW PLLStat)
//{
//	/*使用的是常开下对应的级别StrMenuSet.SqLevelMin*/
//
//	{
//	/* 静噪等级赋值 */
//	StrMenuSet.SqLevel = StrMenuSet.SqLevelMin;
//	}
//
//}




BOOLEAN Check_SQ_Zero(void)
{
	STR_CODESW PLLStat=GetPllStat();
	if(StrMenuSet.Displaymode==DISPFMMODE_Single)
	{
		return (PLLStat.chninfo[StrMenuSet.MainBand].Attr==PLLSTAT_M 
				&& !StrMMenuSetAB[StrMenuSet.MainBand].Squelch);
	}
	return 0;
//	else
//	{
//		return (PLLStat.chninfo[BAND_A_FLAG].Attr==PLLSTAT_M && !StrMMenuSetAB[BAND_A_FLAG].Squelch)
//			 ||(PLLStat.chninfo[BAND_B_FLAG].Attr==PLLSTAT_M && !StrMMenuSetAB[BAND_B_FLAG].Squelch);
//
//	}
}


BOOLEAN Check_SQ_Zero_Onband(void)
{
	//uint8 type = g_Str_ChnStat.R_S_SubType.b_type_r;
	return (GetPllAttr(AllFlag.Rx_BandSelectFlag)==PLLSTAT_M 
			&& !StrMMenuSetAB[AllFlag.Rx_BandSelectFlag].Squelch
			&& (g_Str_ChnStat.R_S_SubType.b_type_r==E_CTD_SQ));


//	if(StrMenuSet.Displaymode==DISPFMMODE_Single)
//	{
//		return (GetPllAttr(StrMenuSet.MainBand)==PLLSTAT_M 
//				&& !StrMMenuSetAB[StrMenuSet.MainBand].Squelch
//				&& (GetPllChnRxSubType(StrMenuSet.MainBand)==E_CTD_SQ));
//	}
//	else
//	{
//		return (GetPllAttr(0)==PLLSTAT_M 
//				&& !StrMMenuSetAB[0].Squelch 
//				&& GetPllChnRxSubType(0)==E_CTD_SQ)
//			 ||(GetPllAttr(1)==PLLSTAT_M 
//				 && !StrMMenuSetAB[1].Squelch 
//				 && GetPllChnRxSubType(1)==E_CTD_SQ);
//	}
}


#if 0
/*****************************************************************************
 *   函数名: ScanDispKeyUp
 *   功能描述: 扫描功能显示列表按向上键操作
 *   参数定义: 
 *   返回值:
 *   创建人：
******************************************************************************/
void ScanDispKeyUp(void)
{    
	  if(g_Item==0)
	  {
	      g_Where=g_DispMin;
	      g_Item=Str_MenuStat.SltItemNum-1;
	  }
	  else
	  {
	     g_Where--;
	     g_Item--;
	  
	     if(g_Where<1)
	     { 
	       g_Where = 1;
	     }
	  }
}
/*****************************************************************************
 *   函数名: ScanDispKeyDown
 *   功能描述: 扫描功能显示列表按向下键操作
 *   参数定义: 
 *   返回值:
 *   创建人：
******************************************************************************/
void ScanDispKeyDown(void)
{
	  g_Where++;
	  g_Item++;
	  if(g_Item==Str_MenuStat.SltItemNum)
	  {
	      g_Where=1;
	      g_Item=0;
	  }
	  else
	  {
	     if(g_Where>g_DispMin)
	     { 
	       g_Where = g_DispMin;
	     }
	  }
  
}
/*****************************************************************************
 *   函数名: GetScanListNumb
 *   功能描述:  信道列表任务初始化
 *   参数定义:	
 *	返回值:
 *   创建人：陶林，2010/10/26
******************************************************************************/
//uint8 GetScanListNumb(void)
//{
//  return 0;
//}

/*****************************************************************************
 *   函数名: GetScanChListNumb
 *   功能描述:  扫描信道列表任务初始化
 *   参数定义:	
 *	返回值:
 *   创建人：陶林，2010/10/26
******************************************************************************/
uint8 GetScanChListNumb(uint8 ListNumb)
{
	return 1;
}
/*****************************************************************************
 *   函数名: ChListDisp
 *   功能描述:  信道列表显示
 *   参数定义:	
 *	返回值:
 *   创建人：陶林，2010/10/26
******************************************************************************/
void ChListDisp(uint8 where,uint8 Item)
{
	uint8 i;//,BufName[11]
	uint8 Where_i,Item_i;
	uint8 Buf[PCEA_SCANAREANAMENUM+1];
   	uint8 row=0;
	if(Str_MenuStat.SltItemNum>0)
   	{
		Where_i=1;
		Item_i =Item-(where-Where_i);
		for(Where_i=1;Where_i<g_DispMin+1;Where_i++)
		{

			sprintf(Buf,(uint8 *)*(ScanChannelListName[StrMenuSet.language].Text+0),ScanListTab[Item_i]+1);
			Buf[PCEA_SCANAREANAMENUM]='\0';
			row = LCD_Menu_LcdRowCount(Where_i)+1;
			if(where==Where_i)
			{				
				
				LCD_Menu_PrintfBar(1,Where_i, Buf, lcd_g);
				/*判断该信道是否显示扫描优先信道标志*/
                          	if((ScanListTab[Item_i] +1)== (GetChPrio(Item_i)))
				{
					CallTBFlag.NotUseBKimageFlag=1;
					//LCD_BLL_LcdWriteImage_BKImgae(row,9,(uint8*)Scan_Prio_disp,16,lcd_g);
					CallTBFlag.NotUseBKimageFlag=0;
				}
			}
			else
			{

				LCD_Menu_PrintfBar(0,Where_i, Buf, lcd_menu);
				if((ScanListTab[Item_i] +1) == (GetChPrio(Item_i)))
				{
					CallTBFlag.NotUseBKimageFlag=1;
					//LCD_BLL_LcdWriteImage_BKImgae(row,9,(uint8*)Scan_Prio_disp,16,lcd_blcak);
					CallTBFlag.NotUseBKimageFlag=0;
				}
			}

			Item_i++;      
		}
	}
	else
	{
		LcdWriteMid(54,lcd_blcak, (uint8*)*StrListEmpty[StrMenuSet.language].Text);
	}

}


/*****************************************************************************
 *   函数名: SetChPrio
 *   功能描述:  设置扫描信道优先级
 *   参数定义:	CHNumb 为0时表示取消该优先信道
 *	返回值:
 *   创建人：陶林，2010/10/26
******************************************************************************/
void SetChPrio(uint8 ListItem,uint8 CHItem)//入口都是Item型的,
{
	uint8 chnum=0;
	uint8 TempBuf[3]={0};
        
        chnum=ScanListTab[CHItem];
	if(CHItem==0xff)//取消优先级的命令
	{
		TempBuf[0]=0;
		TempBuf[1]=0;
		TempBuf[2]=0;
		ScanList.priFlag=0xff;//取消优先级
		E2PROMWriteStr(SCAN_scanIndex_FLAG_ADDR, TempBuf, 3);
	}
	else if(CHItem<0xff)//设定优先级的命令
	{

		TempBuf[0]=SCAN_scanIndex_FLAG;
		TempBuf[1]=chnum+1;//保证大于0
		TempBuf[2]=CHItem;
		ScanList.priFlag=CHItem;//不是信道号码而是Item选中,

		E2PROMWriteStr(SCAN_scanIndex_FLAG_ADDR, TempBuf, 3);
	}

}


/*****************************************************************************
 *   函数名: GetChPrio
 *   功能描述:  获取当前所选扫描列表中的优先扫描信道
 *   参数定义:	
 *	返回值:
 *   创建人：陶林，2010/10/26
******************************************************************************/
uint8 GetChPrio(uint8 ListItem)//返回的是信道号码.
{
	uint8 TempBuf[3]={0};
	uint8 chnum=ScanListTab[ListItem]+1;//获取到信道号码
	E2PROMReadStr(SCAN_scanIndex_FLAG_ADDR, TempBuf, 3);
	if(TempBuf[0]==SCAN_scanIndex_FLAG&&TempBuf[1]==chnum)
	{
		//if(ListItem==TempBuf[2])
		//{	ScanList.priFlag=TempBuf[2];}
		//else
		//{
		//	ScanList.priFlag=0xff;//取消优先级
		//}
		return TempBuf[1];
	}
	return 0;//只要返回是0就没有优先信道的存在.
}

/*****************************************************************************
 *   函数名: AddChListInit
 *   功能描述:  添加扫描信道初始化
 *   参数定义:	
 *	返回值:
 *   创建人：陶林，2010/10/26
******************************************************************************/
//uint8 AddCH[1024];
//uint8 AddChList[256];
void  AddChListInit(void)
{
#if  0
   uint8 Buf[2];
   uint8 i,j,k,flag;
   uint16 Numb=0;

   for(j=0;j<3;j++)
    {
		for(i=16;i<32;i++)
		{
			if(i>15)
			{
				E2PROMReadStr(PCBA_AREASETEXT+PCEA_ZONE_LEN*j+PCEA_CHNBASEADDR+PCEA_CHN_LEN*(i-16), Buf, 1);
			}
			else
			{
				E2PROMReadStr(PCBA_AREASET+PCEA_ZONE_LEN*j+PCEA_CHNBASEADDR+PCEA_CHN_LEN*i, Buf, 1);
			}
                
			if(Buf[0])
			{
				for(k=0;k<16;k++)
				{
					E2PROMReadStr(PCBA_SCAN+PCEA_SCANLISTLEN*ListNumb+PCEA_SCANAREANAMENUM+PCEA_SCANSET+PCEA_SCANLIST_LEN*k, Buf, 2);
					if(Buf[0]==j+1&&Buf[1]==i+1)
					{
                                       		flag=1;
					     	break;
					}
					else
					{
                                        		flag=0;
					}
				}
				if(flag==0)
				{
                               		AddCH[2*Numb]=j+1;
				    	AddCH[2*Numb+1]=i+1;
				    	Numb++;
				}
				
			}

		}
    }
      return Numb;

#endif

}
/*****************************************************************************
 *   函数名: AddCh
 *   功能描述:  添加信道
 *   参数定义:	
 *	返回值:
 *   创建人：陶林，2010/10/26
******************************************************************************/
void AddCh(uint8 Item)//序号
{
#if 0
	uint8 Numb,i;
	uint8 chnum=0;
	uint8 BufSet[2]={0,0};
	uint8 TempBuf[512]={0,0};
	uint8 Tempdata=0;
	chnum=AddChList[Item];//信道号码
	if(ScanListValue==0)
	{//列表为空的情况下.
	          BufSet[0]=SCAN_LIST_VALID_FLAG;
	          BufSet[1]=chnum;
	          E2PROMWriteStr(SCAN_LIST_BASE,BufSet,2);
	}
	else
	{
		for(i=0;i<ScanListValue;i++)
		{//寻找插入增加的信道的位置
			if(ScanListTab[i]>chnum)
			{  
                            break;
			}
		}
		ScanListValue++;
	        //E2PROMReadStr(SCAN_LIST_BASE,TempBuf,512);//测试使用      

	       {
	          Numb=ScanListValue-(i);//该位置之后的所有有效的数据后移
	          Numb*=2;//字节数.

	          E2PROMReadStr(SCAN_LIST_BASE+i*SCAN_ONECH_LEN,TempBuf,Numb);
	          E2PROMWriteStr(SCAN_LIST_BASE+i*SCAN_ONECH_LEN+2,TempBuf,Numb);//后移
	          //移动完毕需要在插入的位置上加入新的有效数据
	          BufSet[0]=SCAN_LIST_VALID_FLAG;
	          BufSet[1]=chnum;
	          E2PROMWriteStr(SCAN_LIST_BASE+(i)*SCAN_ONECH_LEN,BufSet,2);
	        }
	}
	
	//E2PROMReadStr(SCAN_LIST_BASE,TempBuf,512);//测试使用
	
	Tempdata=E2PROMReadByte(PECA_CHNUM_BASEADDR+PCEA_AREARELAY+PCEA_CHN_LEN*(chnum));//
	Tempdata |=0x01;
	E2PROMWriteByte(PECA_CHNUM_BASEADDR+PCEA_AREARELAY+PCEA_CHN_LEN*(chnum),Tempdata);//打开了该信道的扫描使能.
	
	PowerOnInitScanList();//重新初始化了扫描的列表.
	#endif
}
/*****************************************************************************
 *   函数名: DelCh
 *   功能描述:  删除信道
 *   参数定义:	
 *	返回值:
 *   创建人：陶林，2010/10/26
******************************************************************************/
void DelCh(uint8 Item)
{
#if 0
	uint8 Numb;
	uint8 chnum=0;
	uint8 Bufclear[2]={0,0};
	uint8 TempBuf[512]={0,0};
	uint8 Tempdata=0;
	chnum=ScanListTab[Item];
	
	/*判断该删除信道是否为优先信道*/
	//E2PROMWriteStr(SCAN_LIST_BASE+Item*SCAN_ONECH_LEN,Bufclear,2);
        //E2PROMReadStr(SCAN_LIST_BASE,TempBuf,512);//测试使用
	//chnum=TempBuf[Item*2+1];//信道号码
	
		
	Numb=ScanListValue-(Item+1);//该位置之后的所有有效的数据前移.
	Numb*=2;//字节数.
	if(Numb>0)
        {
          E2PROMReadStr(SCAN_LIST_BASE+Item*SCAN_ONECH_LEN+2,TempBuf,Numb);
          E2PROMWriteStr(SCAN_LIST_BASE+Item*SCAN_ONECH_LEN,TempBuf,Numb);
          //移动完毕需要清除最后的位置上的数据.
        }
        E2PROMWriteStr(SCAN_LIST_BASE+(ScanListValue-1)*SCAN_ONECH_LEN,Bufclear,2);
	
	Tempdata=E2PROMReadByte(PECA_CHNUM_BASEADDR+PCEA_AREARELAY+PCEA_CHN_LEN*(chnum));//
	Tempdata &=0xfe;
	E2PROMWriteByte(PECA_CHNUM_BASEADDR+PCEA_AREARELAY+PCEA_CHN_LEN*(chnum),Tempdata);//清除掉了该信道的扫描使能.
	
	PowerOnInitScanList();//重新初始化了扫描的列表.
#endif
}

/*****************************************************************************
 *   函数名: AddChListDisp
 *   功能描述:  添加扫描信道列表显示
 *   参数定义:	
 *	返回值:
 *   创建人：陶林，2010/10/26
******************************************************************************/
void AddChListDisp(uint16 where,uint16 Item)
{
#if 0
	uint16 i;
	uint16 Where_i,Item_i;
	u8 buf[PCEA_SCANAREANAMENUM+1];

	if(Str_MenuStat.SltItemNum>0)
   	{
		Where_i=1;
		Item_i =Item-(where-Where_i);
		for(Where_i=1;Where_i<g_DispMin+1;Where_i++)
		{
			sprintf(buf,(uint8 *)*(ScanChannelListName[StrMenuSet.language].Text+0),AddChList[Item_i]+1);
			buf[PCEA_SCANAREANAMENUM]='\0';
			if(where==Where_i)
			{	
				LCD_Menu_PrintfBar(1,Where_i, buf, lcd_g);
                        }
			else
			{
				LCD_Menu_PrintfBar(0,Where_i, buf, lcd_menu);
			}
			 
			 Item_i++;      
		}
   	}
 #endif
}
///////////////////////////////////////////////////////////////////////////////////
//写频完毕执行一次,恢复出厂设置执行一次.
//功能:使用写频的信道信息来初始化建立扫描的列表.
/////////////////////////////////////////////////////////////////////////////////////
void InitScanListNum(void)
{
#if  0
	uint8 Buf[1];
	uint16  Numb=0;
	uint8  i;
	uint8 TempBuf[10]={0};
	uint8 Databuf[512]={0};
	ScanListValue=0;
	
#if 1
	for(i=0;i<255;i++)//只有一个列表的形式.
	{
		Buf[0]=E2PROMReadByte(PECA_CHNUM_BASEADDR+PCEA_AREARELAY+PCEA_CHN_LEN*(i));
		if(Buf[0]&0X01)
		{
			Databuf[Numb]=SCAN_LIST_VALID_FLAG;//有效标记
			Databuf[Numb+1]=i;//有效的信道号.
			Numb+=2;
			ScanListValue++;
		}
	}
	E2PROMWriteStr(SCAN_LIST_BASE,Databuf,512);
	TempBuf[0]=ScanListValue;
	E2PROMWriteStr(SCAN_VALID_LISTNUM,TempBuf,1);
#endif
#endif

#if 0
	uint8 Buf[11];
	uint16  Numb=0;
	uint32 Freq=0;
	uint8  i;
	uint8 TempBuf;
	uint8 Databuf[512]={0};
	ScanListValue=0;

	for(i=0;i<255;i++)//只有一个列表的形式.
	{
		Buf[0]=E2PROMReadByte(PECA_CHNUM_BASEADDR+PCEA_CHN_LEN*(i));
		if(Buf[0])/*信道有效*/
		{
			E2PROMReadStr(PECA_CHNUM_BASEADDR+PCEA_AREARELAY+PCEA_CHN_LEN*(i),Buf,11);
			memcpy ((void*)&Freq, (void*)&Buf[1], 4);
			if(Check_Freq_scope(Freq,TX_FLAG))
			{
				memcpy ((void*)&Freq, (void*)&Buf[7], 4);
				if(Check_Freq_scope(Freq,RX_FLAG))
				{
					if(Buf[0]&0X01)
					{
						Databuf[Numb]=SCAN_LIST_VALID_FLAG;//有效标记
						Databuf[Numb+1]=i;//有效的信道号.
						Numb+=2;
						ScanListValue++;
					}
				}
			}
		}
	}
	E2PROMWriteStr(SCAN_LIST_BASE,Databuf,512);
	TempBuf=ScanListValue;
	E2PROMWriteByte(SCAN_VALID_LISTNUM,TempBuf);
#endif	
}


/*****************************************************************************
 *   函数名: ScanListSet
 *   功能描述:扫描列表设置
 *   参数定义:Stat:扫描列表值(需要转化为区域号)
 *	返回值:
 *   创建人：陶林，2011/08/22
******************************************************************************/
void ScanListSet(uint8 Stat)//值可取Item
{

}

void PowerOnInitPrepareScanListNum(void)
{//上电准备备用的增加扫描信道
#if 0
#if 0

	uint8 Buf[11]={0};
	uint16  Numb=0;
	uint32 Freq=0;
	uint8  i;
	PreScanListValue=0;
	for(i=0;i<255;i++)//只有一个列表的形式.
	{
		Buf[0]=E2PROMReadByte(PECA_CHNUM_BASEADDR+PCEA_CHANNELNUMBER+PCEA_CHN_LEN*(i));
		Buf[1]=E2PROMReadByte(PECA_CHNUM_BASEADDR+PCEA_AREARELAY+PCEA_CHN_LEN*(i));
		if(Buf[0]!=0&&(Buf[1]&0x01) !=1)
		{//信道有效并且不允许加入
			AddChList[Numb]=i;//有效的信道号.
			Numb++;
		}
		Buf[0]=0;
		Buf[1]=0;
	}
	PreScanListValue=Numb;
#endif
	for(i=0;i<255;i++)//只有一个列表的形式.
	{
		E2PROMReadStr(PECA_CHNUM_BASEADDR+PCEA_AREARELAY+PCEA_CHN_LEN*(i),Buf,11);

		memcpy ((void*)&Freq, (void*)&Buf[1], 4);
		if(Check_Freq_scope(Freq,TX_FLAG))
		{
			memcpy ((void*)&Freq, (void*)&Buf[7], 4);
			if(Check_Freq_scope(Freq,RX_FLAG))
			{
				if((Buf[0]&0x01) !=1)
				{//信道有效并且不允许加入
					AddChList[Numb]=i;//有效的信道号.
					Numb++;
				}
			}
		}
	}
	
#endif
//	PreScanListValue=Numb;


}

void PowerOnInitScanList(void)
{
#if 0
	uint8 TempBuf[512]={0};
	uint8 Num=0;
	uint8 j;
	uint8 flag=0;
	uint16 i=0;

	uint8 BufPrio[2]={0,0};
	E2PROMReadStr(SCAN_scanIndex_FLAG_ADDR,BufPrio, 2);//优先信道的初始化.
	if((BufPrio[0]==SCAN_scanIndex_FLAG))
	{//有优先信道。且优先信道的信道号码符合要求.注意保存的时候有+1
		ScanList.priFlag = BufPrio[1]-1; /* 优先信道号码赋值*/
	}
	else
	{
		ScanList.priFlag = 0xff; /* 无优先信道*/
	}
	
	ScanListValue=0;
	for(i=0;i<255;i++)
	{
		ScanListTab[i]=0;
	}
	//256个信道需要512个字节，分小块才不会容易死机.
	#if 1
	E2PROMReadStr(SCAN_LIST_BASE,TempBuf, 512);
	for(j=0;j<4;j++)
	{
		E2PROMReadStr(SCAN_LIST_BASE+128*j,TempBuf, 128);
		for(i=0;i<128;i+=2)
		{
			if(TempBuf[i]==SCAN_LIST_VALID_FLAG)
			{//扫描的信道有效，且信道号码符合要求.
				ScanListTab[Num]=TempBuf[i+1];
				if(TempBuf[i+1]==ScanList.priFlag)
				{//信道号码相同的.
					if(BufPrio[0]==SCAN_scanIndex_FLAG)
					{	
						flag=1;
						ScanList.priFlag=Num;//把信道号码转化为Item
						E2PROMWriteByte(SCAN_scanIndex_CH_ITEM,ScanList.priFlag);	
					}
				}
                                Num++;
			}
		}
	}
	ScanListValue=Num;
	if(flag==0)
	{
		ScanList.priFlag = 0xff; /* 无优先信道*/
	}
	ScanList.scanIndex=0;

	PowerOnInitPrepareScanListNum();
	#endif
#endif
}

#endif


#endif


