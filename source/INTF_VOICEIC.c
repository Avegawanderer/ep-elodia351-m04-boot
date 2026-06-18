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

#include "app_config.h"
#include "INTF_VOICEIC.h"


//PA4/ADC12_IN4		AUDIO_CK	语音报号芯片时钟端口
#define  MAP_VOICE_CLK_PORT        (PortA)
#define  MAP_VOICE_CLK_PIN         (Pin04)

//PA5/ADC12_IN5		AUDIO_DA	语音报号芯片数据端口
#define  MAP_VOICE_DATA_PORT        (PortA)
#define  MAP_VOICE_DATA_PIN         (Pin05)


 #if VOICE_IC_STAT
	#define VOICE_CLK_HIGH()  PORT_SetBits(MAP_VOICE_CLK_PORT,MAP_VOICE_CLK_PIN )
	#define VOICE_CLK_LOW()   PORT_ResetBits(MAP_VOICE_CLK_PORT,MAP_VOICE_CLK_PIN )

	#define VOICE_DATA_HIGH() PORT_SetBits(MAP_VOICE_DATA_PORT,MAP_VOICE_DATA_PIN )
	#define VOICE_DATA_LOW()  PORT_ResetBits(MAP_VOICE_DATA_PORT,MAP_VOICE_DATA_PIN )
#endif

static const DH_U8 grp_VIC_RU_VOICE[]=
{
#define VICDEF(name,cn,en,ru, ruaddr) ruaddr,
	VIC_STRDEF
#undef VICDEF
};

const DH_U8 gVoiceIC_DelayTbl[][3]=
{
#if VOICE_IC_STAT
#define VICDEF(name,cn,en,ru, ruaddr) {cn,en,ru},
	VIC_STRDEF
#undef VICDEF
#endif
};

/*******************************************************************************
*函数名:VOICEIC_INTF_Init

*函数说明:

*参数说明:无

*返回值:无

*创建人:yufeng

*创建日期:2012/01/31
*******************************************************************************/

void VOICEIC_INTF_GPIOInit(void)
{
#if VOICE_IC_STAT
    stc_port_init_t stcPortInit;

    /* configuration structure initialization */
    MEM_ZERO_STRUCT(stcPortInit);

    stcPortInit.enExInt   = Disable;
    stcPortInit.enPullUp  = Enable;
    stcPortInit.enPinMode = Pin_Mode_Out;
    /*  Port/Pin initialization */
    PORT_Init(MAP_VOICE_CLK_PORT,MAP_VOICE_CLK_PIN, &stcPortInit);
    PORT_Init(MAP_VOICE_DATA_PORT,MAP_VOICE_DATA_PIN, &stcPortInit);
    
    VOICE_CLK_LOW();
    VOICE_DATA_LOW();
#endif
}


/*******************************************************************************
*函数名:VOICEIC_INTF_Write

*函数说明:

*参数说明:无

*返回值:无

*创建人:yufeng

*创建日期:2012/01/31
*******************************************************************************/
void VOICEIC_INTF_Write(DH_U8 Addr)
{
#if VOICE_IC_STAT
    DH_U8 i;
    
    VOICE_CLK_HIGH();
    Delay_ms(7);
    VOICE_CLK_LOW();
    
    for(i=0;i<8;i++)
    {
	  if(Addr&(1<<(7-i)))
      {
        VOICE_DATA_HIGH();
      }
      else
      {
        VOICE_DATA_LOW();
      }
      Delay_ms(1);
      VOICE_CLK_HIGH();
      Delay_ms(1);
      VOICE_CLK_LOW();
    }
    
    VOICE_DATA_LOW();
#endif
}


DH_VOID VOICEIC_INTF_PlayTone(DH_U8 *shift,DH_U8 len)
{
#if	VOICE_IC_STAT

	DH_U8 i,j,lang,addr = 0;

//	if(StrMenuSet.language == 0)
//	{
//		lang = VOICEIC_LANGUAGE1;
//	}
//	else if(StrMenuSet.language == 1)
	{
		lang = VOICEIC_LANGUAGE2;
	}
	//else
	{
//		printk("Wrong Language\r\n");
//		return;
	}

	addr = (lang == VOICEIC_LANG_CN )?VOICEIC_BASEADDR1:((lang == VOICEIC_LANG_EN )?VOICEIC_BASEADDR2:VOICEIC_BASEADDR3);

//	VOICEIC_INTF_SetSoundLvl(2);
	VOICEIC_INTF_Write(0xD4);
	 Delay_ms(10);
	//Delay_ms(10);
//	printk("VOICEIC_INTF_PlayTone,%d\r\n",MenuVarFlg.bSoundChangedFlg);
	for(i=0;i<len;i++)
    {
		if(/*gVoiceIC_DelayTbl[shift[i]][lang] == 0
			|| */shift[i] > VIC_MAX
			)//|| MenuVarFlg.bSoundChangedFlg
		{
			continue;//无此发音
		}

		if(lang == VOICEIC_LANG_RU)//俄文发音地址不与中文和英文一一对应，单独查表
		{
			VOICEIC_INTF_Write(grp_VIC_RU_VOICE[shift[i]]);
		}
		else if(shift[i])
		{
			VOICEIC_INTF_Write(addr+shift[i]-1);
			printk("-%d-%d.\r\n",addr+shift[i]-1,gVoiceIC_DelayTbl[shift[i]][lang]);
		}
		for(j=0;j<gVoiceIC_DelayTbl[shift[i]][lang];j++)
		{
			Delay_ms(10);
		}
		//if(MenuVarFlg.bSoundChangedFlg) break;
    }

//	VOICEIC_INTF_Write(0xD5);
//	Delay_ms(10);
#endif
}


void VOICEIC_INTF_SetSoundLvl(DH_U8 lvl)
{
#if VOICE_IC_STAT
	static DH_U8 lastlvl = 0xff;

	if(lvl>3) lvl=3;
	if(lastlvl!=lvl)
	{
//		CODECLOCK();
		VOICEIC_INTF_Write(0xD4);
		Delay_ms(20);
		VOICEIC_INTF_Write(0xD0+lvl);printk("icSond%d\r\n",lvl);
		Delay_ms(10);
//		CODECUNLOCK();
	}
	lastlvl=lvl;
#endif
}


