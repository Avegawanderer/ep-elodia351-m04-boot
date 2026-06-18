/*****************************************************************************
 *   文件名:keyHnd.c
 *	 文件描述:按键执行函数
 *   创建人：陶林，2009/10/17
******************************************************************************/
#include "app_config.h"

#include "include.h" 

uint8 Key_User1_Down_Flag=0;//按键被按下的标志位1=被按下过,0=未被按下过.
uint8 Key_User2_Down_Flag=0;//按键被按下的标志位1=被按下过,0=未被按下过.
//uint8 Key_Left_Down_Flag=0;//按键被按下的标志位1=被按下过,0=未被按下过.
//uint8 Key_Right_Down_Flag=0;//按键被按下的标志位1=被按下过,0=未被按下过.
extern uint8 where_flag;
void Key_OnlySoundVoice(void)
{
//	KeyVoiceStart(KEY_FLG_SHORT);
//	//LockKeypadHnd(LOCK_YES);
//	KeyVoiceEnd();
}
//

void KeyHndD_None(void){}
void KeyHndU_None(void){}

void KeyHndD_Ptt(void)		/* S220 */
{


//	FMRadioSeekFlag = 0;

	if(GloVarFlag.Freq_ExpandEnableFlag||GloVarFlag.CopyFlag)
	{
		Key_OnlySoundVoice();
		return;
	}

	if(g_Layer==2) {
		return;
	}

	if(g_overtimeforbid)	{		return;	}
	if(GloVarFlag.KeyFlag) return ;	
	if(CHN_BLL_GetAttr()==CHNSTAT_NONE)	{		return;	}
	
	Delay_ms(10);//防止写频开始时会抖动
	if(PORT_GetBit(KEYPTT_PORT, KEYPTT_PIN)==KEY_RELEASE
	&& PORT_GetBit(KEYPTT_E_PORT, KEYPTT_E_PIN)==KEY_RELEASE)		return;

	
	GloVarFlag.KeyFlag=1;
	//此处制造主动呼叫计数开始的初始值
	CallStr.N_100Ms = 0; 

	KeyPutOpt(KEY_CRED, Key_Call);
}




/*****************************************************************************
 *   函数名: KeyHndD
 *   功能描述: 按下按键
 *   参数定义:
 *	返回值:
 *   创建人：陶林，2009/10/17
******************************************************************************/
void KeyHndD_User1(void)		/* S219 侧边按钮*/
{
	if(KEY_APP_IsLongPressFuncChk(Key_P1))
	{
		KeyPutOpt(KEY_CRED, Key_P1);
	}
}
//
void KeyHndD_User2(void)		/* S221 */
{
	if(KEY_APP_IsLongPressFuncChk(Key_P2))
	{
		KeyPutOpt(KEY_CRED, Key_P2);
	}
}
void KeyHndD_User3(void)	
{
	if(KEY_APP_IsLongPressFuncChk(Key_P3))
	{
		KeyPutOpt(KEY_CRED, Key_P3);
	}
}

void KeyHndD_User4(void)	
{
	if(KEY_APP_IsLongPressFuncChk(Key_P4))
	{
		KeyPutOpt(KEY_CRED, Key_P4);
	}
}

/*****************************************************************************
 *   函数名: KeyHndU
 *   功能描述: 释放按键
 *   参数定义:
 *	返回值:
 *   创建人：陶林，2009/10/17
******************************************************************************/

void KeyHndU_Ptt(void)		/* S220 */
{
#if 0
	if(g_Layer==2) {
		//while(GPIO_ReadInputDataBit(PTT_PORT, PTT_PIN)==RESET);
		if(g_BarType!=BARTYPEMENU){
			g_InputNumWhere = g_InputNumBuf_Max-1;
			KeyPutOpt(KEY_CRED, Key_Ok);
		}
		KeyPutOpt(KEY_CRED, Key_CallclewBack);
	}
	#endif
	if(GloVarFlag.Freq_ExpandEnableFlag)
	{
		return;
	}


	if(PORT_GetBit(KEYPTT_PORT, KEYPTT_PIN)==KEY_PRESSING)		return;

	if(PORT_GetBit(KEYPTT_E_PORT, KEYPTT_E_PIN)==KEY_PRESSING)		return;

	
	if(FmRecBusyChk()&&!GloVarFlag.KeyFlag)
	{//处于繁忙禁发的状态，不要传按键出去.

	}
	else if(GloVarFlag.VoxFlg){}
	else
	{
	//	AllFlag.Reflash_TxSignleTBFlag=0;
		KeyPutOpt(KEY_CRED, Key_CallUp);
	//	CallOut_UpTimeSet();
	}
	TmrHndStop(TMRHND_CALLREF);
	GloVarFlag.KeyFlag=0;
}

void KeyHndU_User1(void)		/* S219 */
{	 
	{
		KeyPutOpt(KEY_CRED, Key_P1);
	}
//	if(!Key_User1_Down_Flag)
//	{
//		return;
//	}
//	Key_User1_Down_Flag=0;
//
//	if((TmrHndGet(TMRHND_KEYLONG) && Str_KeyT.KeyType == Key_P1) )
//	{
////		if(Str_Lock.Stat == LOCK_NO)KeyPutOpt(KEY_CRED, Key_P1);
////		else KeyPutOpt(KEY_CRED, Key_SimuBack);
//	}
//	KeyTimeStop();
}


void KeyHndU_User2(void)		/* S221 */
{
	{
		KeyPutOpt(KEY_CRED, Key_P2);
	}
}

void KeyHndU_User3(void)	
{
	{
		KeyPutOpt(KEY_CRED, Key_P3);
	}
}
void KeyHndU_User4(void)	
{
	{
		KeyPutOpt(KEY_CRED, Key_P4);
	}
}

/************************************************************************
* 说明:功能测试版3
***************************************************************************/
void (*PkeyHnd[2][ENM_Key_Max])() =
{

	{
	
//	#define defkeyfunc(name,num) *KeyHndD_##name,
//			KEYFUNCLIST
//	#undef defkeyfunc

	0,
	*KeyHndD_Ptt, 
	*KeyHndD_User1, 
	*KeyHndD_User2,  
	*KeyHndD_User3, 
	*KeyHndD_User4,
	},
	{
//	#define defkeyfunc(name,num) *KeyHndU_##name,
//			KEYFUNCLIST
//	#undef defkeyfunc
	0,
	*KeyHndU_Ptt, 
	*KeyHndU_User1, 
	*KeyHndU_User2, 
	*KeyHndU_User3, 
	*KeyHndU_User4, 
	}
};

