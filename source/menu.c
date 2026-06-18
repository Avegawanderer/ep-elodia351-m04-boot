/*****************************************************************************
 *   文件名:menu.c
 *	 文件描述:  界面接口函数
 *   创建人：陶林，2009/11/4
******************************************************************************/


#include "app_config.h"

#include "include.h"






#define TASKMenu_PRIO TASKMenu_PRIO_
#define TASKMenu_STK_SIZE TASKMenu_STK_SIZE_
//static OS_STK  TaskStkMenu[TASKMenu_STK_SIZE];			// 任务1堆栈

//********************************************************************************************
//与菜单相关的全局变量
//********************************************************************************************


uint8 g_SetItem = 0;


uint8 vol=20;
uint8 g_Flash = 0;              	 //跳转进入子菜单 刷新标志  >1刷新
volatile uint8 g_KeyFuncIndex = 0;        //索引
uint8 g_FatherIndex[MenuSize+1]; //父菜单备份数组
uint8 g_Layer = 0;               	//菜单层数
uint8 g_ItemBackup[MenuSize];      //当前菜单项备份
uint8 g_ItemBackup_i = 0;          //当前菜单项备份数组的i变量

uint8 g_Item;       /* 表示当前菜单层，需要反显的菜单条 */
uint8 g_ItemNum;         			//Bar项数
uint8 g_ItemNumLast;			//保存上次的g_ItemNum
uint8 g_BarType=BARTYPEMENU;	//菜单类型
uint8 **g_DispItem; //项显示内容指针
uint8 g_Where;           //当前反显位置,[0,DispMax-1]  反映反显的Bar在屏幕上的位置
void (*g_KeyFuncPtr)(); //按键功能指针

STR_MENUSTAT Str_MenuStat;

uint8  LastKeyState=0;/*最后一个按键的备份值.*/
//uint8 g_MenuBarType;
extern uint8 DisableCouloScanTimer  ;

uint8 g_KeyMenuFunc = 0;
uint8 SQ_Check_Enable;
uint8 g_Coulo;
DH_U8 g_PowerForbidden ;

//********************************************************************************************
//与设置项相关变量
//********************************************************************************************

uint8 g_BarTypeSetFlag;					//菜单设置项的不同选择
uint32 g_DFRange;

void KeyFunc_MRVF(void);

void KeyNumberDel(void);
//static uint8 g_KeylockFlg;   //待机界面进入功能菜单标志

const KbdTabStruct g_KeyTab[]={
//       |-----------> Index
//       |  Up
//       |  |  Down
//       |  |  |  Ok
//       |  |  |  |  Back   --->功能函数
//       |  |  |  |  |       |
       { 0, 1, 2, 3, 4,5,6,(*DispMenuTop)},// 待机画面
       { 1, 0, 0, 0, 0,0, 0,(*DispMenuTopUp)},
       { 2, 0, 0, 0, 0,0, 0,(*DispMenuTopDown)},
       { 3, 0, 0, 0, 0,0, 0,(*DispMenuTopLeft)},
       { 4, 0, 0, 0, 0,0, 0,(*DispMenuTopRight)},
       { 5, 0, 0, 0, 0,0, 0,(*DispMenuTopOk)},
       { 6, 0, 0, 0, 0,0, 0,(*DispMenuTopBack)},

       { 7, 8, 9, 10, 11,12, 13,(*DispMenu_1_Init)}, //图标层菜单
       { 8, 0, 0, 0, 0,0, 0,(*DispMenu_1_Up)},   //
       { 9, 0, 0, 0, 0,0, 0,(*DispMenu_1_Down)}, //
       { 10, 0, 0, 0, 0,0, 0,(*DispMenu_1_Left)}, //
       { 11, 0, 0, 0, 0,0, 0,(*DispMenu_1_Right)},//
       { 12, 0, 0, 0, 0,0, 0,(*DispMenu_1_Ok)}, //
       { 13, 0, 0, 0, 0,0, 0,(*DispMenu_1_Back)},//

};


extern uint8 codeswStat;
void KeyFuncStart(void);


void MENUInit(void)
{

}

void MENU_SaveSetItem(uint8 item)
{

}


void  TaskMenu (const void *pdata)
{
	pdata = pdata;       
     
	while (1)
	{
		MENULoop();
	} 
}
	 
void MENULoop(void)
{
	uint32 Msg;		 //表示所得按键值

	Msg = KeyGetOpt(0);		//等待按键值	

	if(Msg == 0) return;

	printk("key%d\r\n",Msg);
	LastKeyState=Msg;

	if (CHN_BLL_GetAttr( ) == CHNSTAT_NONE)	//无效操作
	{
		Sound_Alarm();
		return;
	}
	
	if (Msg < Key_Call)		/* 一般菜单项 */
	{
		if (g_BarType == BARTYPENONE)	//无效操作
		{
			Sound_Alarm();
			return;
 		}
  		
		MenuWriteLock();
		if((GloVarFlag.VoxFlg||KEY_PRESSING == PORT_GetBit(KEYPTT_PORT, KEYPTT_PIN)||KEY_PRESSING == PORT_GetBit(KEYPTT_E_PORT, KEYPTT_E_PIN)) && g_Layer==1)
		{
			MenuWriteUnlock();
			return;
		}

		SlpModeExitSM();

		if(ScanCtrlGet()==SCAN_ON)
		{
		#if 0	
			if(Msg==Key_Up||Msg==Key_Down ){}
			else
			{
				ScanStop();
				MenuWriteUnlock();
				//KeyUnLockCheckLoop(Msg);
				return;
			}
		#else			
			ScanStop();
			SOUND_BLL_SoundStartOpt(SOUNDSTAT_Voice,SOUNDSTAT_ScanEndVoice);			
			MenuWriteUnlock();
			return;
		#endif
		}
 		
		switch (Msg)
		{
			case Key_P1:	(*Str_KeyT.P1SHnd)();break;		//
			case Key_P2:	(*Str_KeyT.P2SHnd)();break;		//
			case Key_P3:	(*Str_KeyT.P3SHnd)();break;		//			
			case Key_P4:	(*Str_KeyT.P4SHnd)();break;		//
	
			case Key_LongHnd:
				if(Str_KeyT.KeyLongHnd) (*Str_KeyT.KeyLongHnd)();				
				break;
			default: 			//按键错误的处理
				break;
		}					
	}//这边是一个SWITCH语句
	else		
	{
		MenuWriteLock();
			
		switch (Msg)
		{
			case Key_Call:
	        {
	        	if(GloVarFlag.Freq_ExpandEnableFlag) break;
				SlpModeExitSM();
				
	        	if(GloVarFlag.VoxFlg||KEY_PRESSING == PORT_GetBit(KEYPTT_PORT, KEYPTT_PIN)
					||KEY_PRESSING == PORT_GetBit(KEYPTT_E_PORT, KEYPTT_E_PIN))
				{
					CallOutStart();
				}				
	        }break;
			case Key_CallUp:		
			 { /*//发射结束过程:
					 //1.关闭全部发射提示.
					 //2.给对方的提示音100MS
					 //3.尾音消除250MS
					 //4.给自己的提示音100MS
					 //5.转到接收以及其他.
			 */
				if(KEY_PRESSING == PORT_GetBit(KEYPTT_PORT, KEYPTT_PIN)
					||KEY_PRESSING == PORT_GetBit(KEYPTT_E_PORT, KEYPTT_E_PIN))	 
				{
					break;
				}
				if(!g_FpgaTx)//声控关闭的是定时传递按键过来的.
					break;
 					
				SQ_Check_Enable=1;//禁止掉1846SQ的查询.
				AllFlag.Reflash_SignleTBFlag=0;
				if(CHN_BLL_GetAttr()==CHNSTAT_FM)
				{
 					RED_LED_OFF;
 					
//					if(StrMenuSet.Call_BOTEOT&0x02 && !GloVarFlag.CallToneFlg )
//					{
//						FM_INTF_GenerateTone( 700);
//						osDelay(250);
//						FM_INTF_StopTone();
//					}

						
					TmrHndStop(TMRHND_CALLREF);

					FM_APP_SetTail();
		
					INTF_FmRecvEn();
					
			
					if(g_overtimeforbid )//发射如果超时了.
					{
						g_overtimeforbid =0;
					}
  
					if(ScanCtrlGet()==SCAN_ONOF)
					{//发射前若有正在扫描,完毕之后需要还原扫描状态.
						ScanCtrlSet(SCAN_ON);
						//保持3秒之后回到初始的状态.
						g_remainflag =SCAN_REMAIN_TIME_MAX;
					}
					
					
				}
				g_FpgaTx=0;
				DisableCouloScanTimer = 3;
				
				SQ_Check_Enable=0;//禁止掉1846SQ的查询.
			}break;
			case Key_Ref:
				if (g_FpgaTx==1)	/* 通话层 */
				{         
					 CallRefHnd();							
				}
				else
				{
					if(g_Layer!=1)
					{ 
						g_KeyFuncPtr = g_KeyTab[g_KeyFuncIndex].CurrentOperate;
						(*g_KeyFuncPtr)();	
					}
				}
				break;
				case Key_SimuBack:	/* 退出菜单层 */    
				break;
        
		default:
			break;
		}

	}
	MenuWriteUnlock();
}

/*****************************************************************************
 *   函数名: MenuDispInit
 *   功能描述:  菜单显示初始化
 *   参数定义:	
 *	返回值:
 *   创建人：陶林，2011/02/17
******************************************************************************/
void MenuDispInit(void)
{	

}

/*
*****************************************************************************
* DispMenu_1_Init - 顶层菜单层
* DESCRIPTION: -
* 共有6个函数

* @Return :
*
*****************************************************************************
*/
void DispMenuTop(void)				//待机界面
{

	MenuValInit();
	g_BarType = BARTYPEMENU;//菜单类型

	AllFlag.FreqallChangeFlag = 0;
	AllFlag.Freqall_SaveChnFlag = 0;
	
	TmrHndStop(TMRHND_KEY_FUNC);

	{
		
		

	}
	g_Flash = FlashMode_NoAction;
	AllFlag.DisplayMenutopOkFlag=1;

}


void DispMenuTopUp(void)
{
	
	if(g_Layer==1&&ScanCtrlGet()==SCAN_ON)
	{//在全频扫描的模式下
        AllFlag.ScanFreqChangeDirFlag=1;
        g_KeyFuncIndex=MenuTopIndex;
	}
	
	Jump2Menu(MenuTopIndex, FlashMode_NoAction);

}
void DispMenuTopDown(void)
{

	if(g_Layer==1&&ScanCtrlGet()==SCAN_ON)
	{//在全频扫描的模式下
		AllFlag.ScanFreqChangeDirFlag=0;
		g_KeyFuncIndex=MenuTopIndex;
	}
	
	else//信道模式切換信道
	{
		
	}
	Jump2Menu(MenuTopIndex, FlashMode_NoAction);

}
void DispMenuTopLeft(void)
{
//	Jump2Menu(MenuTopIndex, FlashMode_NoAction);	
}
void DispMenuTopRight(void)
{

//	Jump2Menu(MenuTopIndex, FlashMode_NoAction);

}

void DispMenuTopOk(void)
{

	Jump2Menu(SelfMenuIndex, FlashMode_NoAction);	
}

void DispMenuTopBack(void)
{
	Jump2Menu(SelfMenuIndex, FlashMode_NoAction);
}

/*****************************************************************************
 *   函数名: DispMenuCallRef
 *   功能描述:  刷新当前菜单
 *   参数定义: 
 *	返回值: 
 *   创建人：陶林，2010/11/29
******************************************************************************/
void DispMenuCallRef(void)
{
}

/*
*****************************************************************************
* DispMenu_1_Init - 图标层菜单界面
* DESCRIPTION: -
* 共有7个函数
 DispMenu_1_Init    DispMenu_1_Up   DispMenu_1_Down
 DispMenu_1_Left    DispMenu_1_Right
* @Return :
*
*****************************************************************************
*/
void DispMenu_1_Init(void)
{
 #if 0
	DispMenuInitSave();

	{
		g_ItemNum = StrMenu_top[StrMenuSet.language].TextNum;//多少数据项
		g_DispItem = StrMenu_top[StrMenuSet.language].Text;
		g_BarType = StrMenu_top[StrMenuSet.language].BarType_Type;
	
		if (g_Flash == FlashMode_AutoInit)
		{
			g_Item = g_SetItem;

			{
			
			}
		}
		TmrHndStart(TMRHND_KEY_FUNC);
	
	}
#endif
}

void DispMenu_1_Up(void)
{
 #if 0
//	if(AllFlag.KeyNumberOnFlag)//开启了快捷设置.
//	{
//		TopMenuSetKeyUp();
//               // g_KeyFuncIndex=Menu_1_Index;
//		 Jump2Menu(SelfMenuIndex, FlashMode_NoAction);
//	}
//	else
	{
		//KeyUp();
		if (g_BarType == BARTYPEMENU)
		{
			TopFunc_INTF_Alterate(Key_Up);
		}
		else if (g_BarType == BARTYPESET || g_BarType == BARTYPEEDIT)
		{
			TopFunc_INTF_Operation(Key_Up);
		}
		else 
		{
		}
			
		Jump2Menu(SelfMenuIndex, FlashMode_NoAction);	
	}
#endif
}

void DispMenu_1_Down(void)
{
 #if 0
	if (g_BarType == BARTYPEMENU)
	{
		TopFunc_INTF_Alterate(Key_Down);
	}
	else if (g_BarType == BARTYPESET || g_BarType == BARTYPEEDIT)
	{
		TopFunc_INTF_Operation(Key_Down);
	}
	else 
	{
	}
		
#endif
	Jump2Menu(SelfMenuIndex, FlashMode_NoAction);	
	
}
void DispMenu_1_Left(void)
{
 //  Jump2Menu(SelfMenuIndex, FlashMode_NoAction);	
}
void DispMenu_1_Right(void)
{
//	Jump2Menu(MenuTopIndex, FlashMode_AutoInit);
}

void DispMenu_1_Back(void)
{
 #if 0
	//Jump2Menu(MenuTopIndex, FlashMode_ReFlash);
	if (g_BarType == BARTYPEMENU)
	{
		MENU_SaveSetItem(g_Item);
		POP();
	}
	else
	{
		if (g_BarType == BARTYPESET)
		{
			TopFunc_INTF_Operation(Key_Ok);
	 		g_BarType = BARTYPEMENU;
			TopFunc_INTF_Enter();
		}
		else if (g_BarType == BARTYPEEDIT)
		{
			if(g_InputNumWhere==0)
			{
				g_BarType = BARTYPEMENU;
				TopFunc_INTF_Enter();
			}
			else
			{
				g_InputNumWhere--;
			//	LCD_INTF_StartBlink(E_BLINK_EDIT);
			}	
		}
		else if (g_BarType == BARTYPEINPUTNUM)
		{
			g_BarType = BARTYPESET;
		}
		TopFunc_INTF_Disp();	
	  	Jump2Menu(SelfMenuIndex, FlashMode_NoAction);
	}
#endif
	
}
void DispMenu_1_Ok(void)
{
 #if 0
//	if(AllFlag.KeyNumberOnFlag)
//	{
//		TopMenuSetKeyOK();
//		 Jump2Menu(SelfMenuIndex, FlashMode_NoAction);	
//		return;
//	}
//	g_DispItemLast=g_DispItem;
//	g_KeylockFlg = 0;//到时若需要该在第二层就锁键只需改变位置和back处
//	Jump2Menu(Menu_2_Index, FlashMode_AutoInit);
	
	MENU_SaveSetItem(g_Item);

//	if(Str_MenuStat.SltItemBuf[g_Item]==E_TopFunc_OFFSET\
//	|| Str_MenuStat.SltItemBuf[g_Item]==E_TopFunc_SFT)
//	{
//
//	}
//	else
	{
		if (g_BarType == BARTYPEMENU)
		{
			if(Str_MenuStat.SltItemBuf[g_Item]==E_TopFunc_PON)
			{
				g_BarType = BARTYPEEDIT;
			}
			else if(FreqallStat_Check())
			{
				 if(Str_MenuStat.SltItemBuf[g_Item]==E_TopFunc_MNAME\
				  ||Str_MenuStat.SltItemBuf[g_Item]==E_TopFunc_LOUF)
				 {
				 }
				 else
				 {
					 g_BarType = BARTYPESET;
				 }
			}
			else
			{
				if(Str_MenuStat.SltItemBuf[g_Item]==E_TopFunc_MNAME)
				{
					g_BarType = BARTYPEEDIT;
				}
				else if(Str_MenuStat.SltItemBuf[g_Item]==E_TopFunc_OFFSET\
				 	  ||Str_MenuStat.SltItemBuf[g_Item]==E_TopFunc_SFT)
				{

				}
				else 
				{
					g_BarType = BARTYPESET;
				}
			}
		}
		else if ((g_BarType == BARTYPESET))
		{
			TopFunc_INTF_Operation(Key_Ok);
			if(Str_MenuStat.SltItemBuf[g_Item]!=E_TopFunc_SCN)	g_BarType = BARTYPEMENU;
		}
		else if ((g_BarType == BARTYPEEDIT))
		{
			TopFunc_INTF_Operation(Key_Ok);
		}
		else 
		{
			TopFunc_INTF_Operation(Key_Ok);
			g_BarType = BARTYPESET;
		}
		TopFunc_INTF_Disp();
		if(g_BarType == BARTYPEMENU)	 LCD_INTF_StartBlink(E_BLINK_ITEM);
		else if(g_BarType == BARTYPEEDIT)LCD_INTF_StartBlink(E_BLINK_EDIT);
		else LCD_INTF_StartBlink(E_BLINK_ITEM);//LCD_INTF_StopBlink();
	}
#endif
	Jump2Menu(SelfMenuIndex, FlashMode_NoAction);
}


/*
*****************************************************************************
* MenuValInit - 初始化菜单系统
* DESCRIPTION: -
* 该函数必须在进入菜单前被调用   初始化全局变量
* @Para void:
* Return :
*
*****************************************************************************
*/
void MenuValInit(void)
{
//	g_WhereBackup_i = 0;
	g_ItemBackup_i = 0;
	Str_MenuStat.SltItemBufBackUp_i = 0;
	Str_MenuStat.SltItemNumBackUp_i = 0;
	g_Layer = 0;
	g_BarType = BARTYPEMENU;
	g_KeyFuncIndex = MenuTopIndex;
	g_FatherIndex[g_Layer++]  = MenuTopIndex;  //push index  待机界面作为起点
	g_Item=0;
}

//跳转到指定菜单
void Jump2Menu(uint8 Index,uint8 FlashMode)
{
	g_KeyFuncIndex = Index;                 //jump to Menu index

	if (FlashMode)
	{
		g_Flash = FlashMode;
	}
}

//-----------------------
//菜单备份函数
//备份当前菜单的各个参数
void PUSH(void)
{
//	g_WhereBackup[g_WhereBackup_i++] = g_Where;      //备份Where--->高亮显示的位置
	g_ItemBackup[g_ItemBackup_i++] = g_Item;         //备份Item--->当前项的位置
	g_FatherIndex[g_Layer++] = g_KeyFuncIndex;      //备份KeyFuncIndex--->状态机的状态
    memcpy (&Str_MenuStat.SltItemBufBackUp[Str_MenuStat.SltItemBufBackUp_i][0], Str_MenuStat.SltItemBuf, MENU_ITEMMAX);

	Str_MenuStat.SltItemBufBackUp_i++;
	Str_MenuStat.SltItemNumBackUp[Str_MenuStat.SltItemNumBackUp_i++] = Str_MenuStat.SltItemNum;
}

//菜单还原函数
void POP(void)
{
	if (g_Layer>1)
	{
		g_Layer--;      //先弹出一层,因为使用的是++,因此当前Layer指向下一个数组位置
		g_KeyFuncIndex = g_FatherIndex[g_Layer-1];     //恢复KeyFuncIndex
//		g_Where = g_WhereBackup[--g_WhereBackup_i];    //恢复高亮显示位置Where
		g_Item = g_ItemBackup[--g_ItemBackup_i];       //恢复项位置Item
		Str_MenuStat.SltItemBufBackUp_i--;
		memcpy (Str_MenuStat.SltItemBuf, &Str_MenuStat.SltItemBufBackUp[Str_MenuStat.SltItemBufBackUp_i][0], MENU_ITEMMAX);
	//	g_DispItem=g_DispItemLast;
		Str_MenuStat.SltItemNum = Str_MenuStat.SltItemNumBackUp[--Str_MenuStat.SltItemNumBackUp_i];
	}
	g_Flash = FlashMode_ReFlash;                     //子项返回,请求刷新
}



//g_InputNumBuf 转频率后检测输入的频率是否处于范围内
DH_BOOL checkInputFreq(uint32     freq,uint32 min, uint32 max)
{
//	uint32 frp = 0;
//	uint8 i, buf[10];
//	if(min)
//	{
//		itostr_len(buf,min,9);//	sprintf((char*)buf,"%09d",min);
//		
//		for(i=0 ; i<g_InputNumBuf_i ; i++)
//		{
//			frp=frp*10 + (buf[i]-'0');
//		}
//		for( ; i<9 ; i++)
//		{
//			frp=frp*10;
//		}
//	}

//	if(freq>=frp && freq<=max)	return TRUE;
	return DH_FALSE;
}


void KeyNumberDel(void)
{
	
}



void KeyFuncStart(void)
{
	
}

void KeyFuncQuickEnter(uint8 Msg)
{
	
}


void KeyNumber(uint8 KeyDigit)
{
#if 0
	if(CallTBFlag.DispMsgBoxDispFlg)/*正在显示框子,返回*/
		return;

	if(GloVarFlag.Freq_ExpandEnableFlag)
	{
		#if 0
		if(g_BarType!=BARTYPEEDIT)
		{
			//InputNumEditInit(BARTYPEEDIT,6);
			
			memset(g_InputNumBuf,'-',6);
			
			TopFunc_EditInit();			
			g_InputNumBuf[6] = 0;
			g_InputNumBuf_i  = 6;//PCEA_CHN_NAME_LEN;
			g_InputNumBuf_Max = 6;//PCEA_CHN_NAME_LEN;
		}
		InputNumNumber(KeyDigit - Key_0 + '0' );
		
		if(g_InputNumBuf_i==g_InputNumBuf_Max)
		{	
			Key_PasswordInputDone();
		}
		#else
			if(g_BarType!=BARTYPEEDIT)
			{
				InputNumEditInit(BARTYPEEDIT,6);
				memset(g_InputNumBuf,'-',6);
			}
			g_InputNumBuf[g_InputNumWhere] = KeyDigit - Key_0 + '0' ;
		#endif
		Expand_InputDisp();
	}
	else if(g_Layer==2&&(g_KeyFuncIndex ==Menu_1_Index))
	{
		if((g_BarType==BARTYPEMENU))//选项
		{
		}
		else//其他设置或输入状态
		{
			if(TopFunc_INTF_GetCurSetItem()==E_TopFunc_OFFSET && FreqallStat_Check())//
			{
				if(g_BarType==BARTYPESET)
				{
					InputNumEditInit(BARTYPEINPUTNUM,6);//xx.0000
				}
				
				InputNumNumber(KeyDigit - Key_0 + '0' );
				u32 freq = getInputNumberFreq();
				if(!checkInputFreq(freq,0,SFT_RangeMax/100))
				{
					BackspaceNumber();//回退-个数字字符
					//输入错误字符声音
					if(g_InputNumBuf_i == 0) {
						g_BarType=BARTYPESET;
						return;
					}
				}
				else
				{	
					if(g_InputNumBuf_i==g_InputNumBuf_Max)
					{
						g_BarType=BARTYPESET;		
						SFTSet_getInputFreq();
						InputFreqOkSound();
					}
					TopFunc_INTF_Disp();
				}
			}
			else if(TopFunc_INTF_GetCurSetItem()==E_TopFunc_MNAME || TopFunc_INTF_GetCurSetItem()==E_TopFunc_PON)//
			{
				g_InputNumBuf[g_InputNumWhere] = KeyDigit - Key_0 + '0' ;				
			}
		}
	}
	else if(g_Layer==1)
	{	
		if((FreqallStat_Check()))
		{
			Freqall_KeyOperate(KeyDigit);
		}
		else//信道模式
		{
			if(g_BarType==BARTYPEMENU)//常规
			{
				InputNumEditInit(BARTYPE_INPUTCHANNELNUM,3);	//xxx				
				InputChannelNumInit(PLLStat_Num[StrMenuSet.MainBand]);
				ChannelNumInput(KeyDigit - Key_0 + '0' );
				
				ChannelNumInputDisp(StrMenuSet.MainBand);
			}
			else if(g_BarType==BARTYPE_INPUTCHANNELNUM)
			{
				ChannelNumInput(KeyDigit - Key_0 + '0' );
				
				ChannelNumInputDisp(StrMenuSet.MainBand);
				if(g_InputNumBuf_i == g_InputNumBuf_Max)
				{//手动输入信道号码已经完成了
					HandleChangeInputChnum();
					g_InputNumBuf_i =0; 
					g_KeyFuncIndex=0;
					g_BarType=BARTYPEMENU;
				//DispMenuTop();
				}
			}
		}

	}
#endif
	return;

}


