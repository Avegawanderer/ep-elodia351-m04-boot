
#include "app_config.h"



//PA10/USBFS_ID		ES_SET	接收/发射音频降噪开/关	H=开;L=关	
//PA15/TDI		MCU-ES_SW	降噪模组电源开/关控制端口	L=开;H=关	
//PC10		   MCU_ES_SET	接收或发射音频降噪选择	L=TX;H=RX		只能二选一	
//PC11		   MCU_IG_SCL	MCU与降噪模组通讯时钟端口			预留	
//PC12		  MCU_IG_SDA	MCU与降噪模组通讯数据端口			预留	

#define  MCU_ES_SET_PORT        (PortC)
#define  MCU_ES_SET_PIN         (Pin10)

#define  MCU_ES_SW_PORT        (PortA)
#define  MCU_ES_SW_PIN         (Pin15)

#define  ES_SET_ONFOFF_PORT        (PortA)
#define  ES_SET_ONFOFF_PIN         (Pin10)

DH_VOID DeNoice_INTF_GPIOInit(DH_VOID)
{
	stc_port_init_t stcPortInit;
	
	/* configuration structure initialization */
	MEM_ZERO_STRUCT(stcPortInit);

	stcPortInit.enExInt	= Disable;
	stcPortInit.enPullUp	= Enable;
	stcPortInit.enPinMode = Pin_Mode_Out;
	/*  Port/Pin initialization */
	PORT_Init(MCU_ES_SET_PORT,MCU_ES_SET_PIN, &stcPortInit);
	PORT_Init(MCU_ES_SW_PORT,MCU_ES_SW_PIN, &stcPortInit);
	PORT_Init(ES_SET_ONFOFF_PORT,ES_SET_ONFOFF_PIN, &stcPortInit);

	PORT_SetBits(MCU_ES_SET_PORT,MCU_ES_SET_PIN);
	PORT_ResetBits(MCU_ES_SW_PORT,MCU_ES_SW_PIN);
	PORT_ResetBits(ES_SET_ONFOFF_PORT,ES_SET_ONFOFF_PIN);


//	MenuVarFlg.denoisesw = FLASH_INTF_ReadByte(g_FlashSaveFlg, Flash_MENUSET_INDEX(FlashMENUSET_DeNoiceSW));
//	if (MenuVarFlg.denoisesw != 1)
	{
//		MenuVarFlg.denoisesw = 0;
	}
}


//on 0
//off 1

DH_VOID DeNoice_INTF_Sw(DH_VOID)
{
	 PORT_ResetBits(MCU_ES_SW_PORT,MCU_ES_SW_PIN);
}

DH_U8 DeNoice_INTF_GetMicDeNoiceSw(void)
{
	//return GPIO_DRV_GetOutputStat(MCU_ES_SET);
	return PORT_GetBit(MCU_ES_SET_PORT,MCU_ES_SET_PIN);
}
//mic 0
//voice 1
DH_VOID DeNoice_INTF_VoiceMicDeNoiceSw(DH_U8 stat)
{
	if(stat)
	{
		PORT_SetBits( MCU_ES_SET_PORT,MCU_ES_SET_PIN);
	}
	else
	{
		PORT_ResetBits( MCU_ES_SET_PORT,MCU_ES_SET_PIN);
	}
}


//on 0
//off 1
DH_VOID DeNoice_INTF_DeNoiceOnOff(DH_VOID)
{
//	if(MenuVarFlg.denoisesw)PORT_SetBits(ES_SET_ONFOFF_PORT,ES_SET_ONFOFF_PIN);
//	else PORT_ResetBits(ES_SET_ONFOFF_PORT,ES_SET_ONFOFF_PIN);
}



#if 0

DH_U8 gImage_denoice[] = {
//	0x00,0xC0,0xF0,0x78,0xFC,0x1C,0xFE,0x06,0xFE,0x7E,0xFE,0xFC,0xFC,0xF8,0xF0,0xC0,0x00,0x03,0x0F,0x1C,0x3F,0x30,0x7F,0x40,0x7F,0x7C,0x7F,0x3E,0x3F,0x1E,0x0F,0x03,
	0x00,0xC0,0xF0,0x38,0xFC,0x0C,0xFE,0x02,0xFE,0x3E,0xFC,0x7C,0xF8,0x70,0xC0,0x00,0x00,0x03,0x0F,0x1E,0x3F,0x38,0x7F,0x60,0x7F,0x7E,0x3F,0x3F,0x1F,0x0F,0x03,0x00
};
DH_VOID DeNoice_INTF_DeNoiceDispIcon(DH_VOID)
{
	if( MenuVarFlg.denoisesw)
	{
		LCD_BLL_LcdWriteImage_BKImage(SCANDISP_app_GetTopBarStrRow(),pFirICO_Col->FIRICO_DeNoice,gImage_denoice,16,lcd_color_TopRow);///
	 }
	else
	{
		LCD_BLL_LcdWriteImage_BKImage(SCANDISP_app_GetTopBarStrRow(),pFirICO_Col->FIRICO_DeNoice,(DH_U8 *)blank_disp,13,lcd_color_TopRow);///
	}
}
#endif
