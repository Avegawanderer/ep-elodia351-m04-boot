/*******************************************************************************
 * Copyright (C) 2011-2013 ZheJiang Dahua Technology CO.,LTD.
 *
 * 文件名 : INTF_CODESW.c
 *
 * 作  者 : 123(ggg)<gg>
 * 版  本 : V1.0.0
 * 日  期 : 2015年4月7日 Create
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

#include "app_config.h"

#include "INTF_SPK.h"






/* ========================================================================== */
/*                           宏和类型定义区                                   */
/* ========================================================================== */



#define  MAP_FREQ1_PORT        (PortB)
#define  MAP_FREQ1_PIN         (Pin04)


#define  MAP_FREQ23_PORT        (PortH)
#define  MAP_FREQ2_PIN         (Pin01)
#define  MAP_FREQ3_PIN         (Pin00)

#define  MAP_FREQ4_PORT        (PortC)
#define  MAP_FREQ4_PIN         (Pin13)

/* ========================================================================== */
/*                          数据结构定义区                                    */
/* ========================================================================== */

#if USE_CHN_CODESW

const DH_U8 gu8StatCode[16] =
{
//		Chn f 0
//		Chn e 1
//		Chn a 2
//		Chn b 3
//		Chn 3 4
//		Chn 2 5
//		Chn 6 6
//		Chn 7 7
//		Chn 5 8
//		Chn 4 9
//		Chn 0 10
//		Chn 1 11
//		Chn 9 12
//		Chn 8 13
//		Chn c 14
//		Chn d 15

		10, //		Chn 0
		11,//		Chn 1
		5,//		Chn 2
		4, //		Chn 3
		9, //		Chn 4
		8,//		Chn 5
		6, //		Chn 6
		7,//		Chn 7
		13, //		Chn 8
		12,//		Chn 9
		2, //		Chn a
		3, //		Chn b
		14,//		Chn c
		15,//		Chn d
		1, //		Chn e
		0,//		Chn f
};
#endif

static DH_U8 gu8CodeSw = 0xff;
DH_U8 g_u8CodeSwCur = 1;

/* ========================================================================== */
/*                          函数声明区                                        */
/* ========================================================================== */

/*********************************************************************
 * 函数名: CODESW_INTF_Init
 * 描  述: 编码频道旋钮初始化
 * 输  入: 无
 * 输  出: 无
 * 返回值: 无
 *********************************************************************/
DH_VOID CODESW_INTF_GPIOInit(DH_VOID)
{
#if USE_CHN_CODESW
	stc_port_init_t stcPortInit;
	
	/* configuration structure initialization */
	MEM_ZERO_STRUCT(stcPortInit);

	stcPortInit.enPinMode = Pin_Mode_In;
	stcPortInit.enExInt   = Disable;
	stcPortInit.enPullUp  = Disable;

	/*	Port/Pin initialization */
	PORT_Init(MAP_FREQ1_PORT, MAP_FREQ1_PIN, &stcPortInit);

	/*	Port/Pin initialization */
	PORT_Init(MAP_FREQ23_PORT, MAP_FREQ2_PIN, &stcPortInit);
	PORT_Init(MAP_FREQ23_PORT, MAP_FREQ3_PIN, &stcPortInit);


	PORT_Init(MAP_FREQ4_PORT, MAP_FREQ4_PIN, &stcPortInit);

#else
	g_u8CodeSwCur = MENU_APP_GetMenuSetStat(BARTYPESETFLG_CurChn);

#endif
	
}

/*********************************************************************
 * 函数名: CODESW_INTF_GetRealStat
 * 描  述: 获取信道编码值
 * 输  入: 无
 * 输  出: 无
 * 返回值: 频道编码值
 *********************************************************************/
DH_U8 CODESW_INTF_GetRealStat(DH_VOID)
{
#if USE_CHN_CODESW
	DH_U8 Stat = 0;
	/*因为上升沿和下降沿不能同时产生中断，需要更新调整*/
	if(PORT_GetBit(MAP_FREQ1_PORT, MAP_FREQ1_PIN))
	{
		Stat |= 1;
	}
	Stat <<= 1;
	if(PORT_GetBit(MAP_FREQ23_PORT, MAP_FREQ2_PIN))
	{
		Stat |= 1;
	}
	Stat <<= 1;
	if(PORT_GetBit(MAP_FREQ23_PORT, MAP_FREQ3_PIN))
	{
		Stat |= 1;
	}
	Stat <<= 1;
	if(PORT_GetBit(MAP_FREQ4_PORT, MAP_FREQ4_PIN))
	{
		Stat |= 1;
	}
	g_u8CodeSwCur = gu8StatCode[Stat];
	return  gu8StatCode[Stat];
#else
	return g_u8CodeSwCur;
#endif
}
DH_U8 CODESW_INTF_GetCurZone(DH_VOID)
{
	//return MENU_APP_GetMenuSetStat(BARTYPESETFLG_ZONE);
}

DH_VOID CODESW_INTF_SetCurZone(DH_U8 zone)
{
	//MENU_APP_MenuSetSetted(BARTYPESETFLG_ZONE,zone);
}


DH_U8 CODESW_INTF_GetNextCode(DH_U8 adddec)
{
//#if FUNC_SKIP_EMPTY_CHN
//	DH_U8 cnt = 0,swcode = CODESW_INTF_GetRealStat();
//	do {
//		swcode = getNextIndex(adddec,swcode,0,CHN_MAX-1);
//		cnt ++;
//		if(cnt>CHN_MAX) return CODESW_INTF_GetRealStat();
//	} while (!CHN_APP_ChnValidChk(MENU_APP_GetMenuSetStat(BARTYPESETFLG_ZONE),swcode));
//
//	return swcode;
//#else
//	return getNextIndex(adddec,g_u8CodeSwCur,0,CHN_MAX-1);
//#endif
}
/*********************************************************************
 * 函数名: CODESW_INTF_SaveStatus
 * 描  述: 保存信道编码值
 * 输  入: 频道编码值
 * 输  出: 无
 * 返回值: 无
 *********************************************************************/
DH_VOID CODESW_INTF_SaveStatus(DH_U8 u8Data)
{
	gu8CodeSw = u8Data;
}

/*********************************************************************
 * 函数名: CODESW_INTF_GetStatus
 * 描  述: 返回信道编码值
 * 输  入: 无
 * 输  出: 无
 * 返回值: 频道编码值
 *********************************************************************/
DH_U8 CODESW_INTF_GetStatus(DH_VOID)
{
	return gu8CodeSw;
}

DH_VOID CODESW_INTF_UpdateSqLevel(DH_VOID)
{
//	int i, k;
//	DH_U8 bufL[36];
//	DH_U32 freq;
//	freq = RF_BLL_GetPll().u32RxFreq;
//
//#if (FREQ_SCOPE_SET==FREQ_400_480 || FREQ_SCOPE_SET==FREQ_200_300)
//	for (k = 1; k < 8; k++)
//	{
//		if (freq / 10000000 < g_u32FreqSectorList[k + 1] / 10000000)break;
//	}
//#elif (FREQ_SCOPE_SET==FREQ_30_32\
//	|| FREQ_SCOPE_SET==FREQ_66_68)
//	for (k = 1; k < 3; k++)
//	{
//		if (freq  < g_u32FreqIntSectorList[ k ] )break;
//	}
//#elif FREQ_SCOPE_SET==FREQ_66_88
//	for (k = 1; k < 6; k++)
//	{
//		if (freq  < g_u32FreqIntSectorList[ k ] )break;
//	}
//#else
//	for (k = 1; k < 8; k++)
//	{
//		if (freq  < g_u32FreqIntSectorList[ k ] )break;
//	}
//#endif
//
//	FLASH_INTF_ReadStr(g_FlashSaveFlg,TUNER_SQWDBADDR + 1 + 18 * (k - 1), bufL, 18);
//	for (i = 0; i < 9; i++)
//	{
//		g_SqLevelArr[i] = bufL[2 * i + 1] * 256 + bufL[i * 2];
//	}
//	HRC_BLL_GetSqLevel();
}
