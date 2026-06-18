/*******************************************************************************
 * Include files
 ******************************************************************************/

#include "app_config.h"

#include "INTF_SPK.h"

#define  SPK_SHTD_PORT         (PortH)
#define  SPK_SHTD_PIN          (Pin02)

//PB2/PVD2EXINP		BT_MIC_SP_SW	使用蓝牙设备音频切换控制	L应用蓝牙		H本机/手咪	
#define  BT_MIC_SP_SW_PORT         (PortB)
#define  BT_MIC_SP_SW_PIN          (Pin02)

//PC8		MUTE	MIC静音控制端口	H有效	
#define  MIC_MUTE_PORT         (PortC)
#define  MIC_MUTE_PIN          (Pin08)

//PA3/ADC1_IN3		AF_TONE_SW	单音/音频输出切换控制端口	H=音频;L=单音	
#define  AF_TONE_SW_PORT         (PortA)
#define  AF_TONE_SW_PIN          (Pin03)


/*******************************************************************************
 * 函数名  : SPK_INTF_Init
 * 描  述  : 喇叭接口初始化
 * 输  入  : 无
 * 输  出  : 无
 * 返回值  : 无
 *******************************************************************************/
DH_VOID SPK_INTF_GPIOInit(DH_VOID)
{	
   stc_port_init_t stcPortInit;

    /* configuration structure initialization */
    MEM_ZERO_STRUCT(stcPortInit);

    stcPortInit.enPinMode = Pin_Mode_Out;
    stcPortInit.enExInt   = Disable;
    stcPortInit.enPullUp  = Enable;

    /*  Port/Pin initialization */
    PORT_Init(SPK_SHTD_PORT, SPK_SHTD_PIN, &stcPortInit);
    PORT_Init(BT_MIC_SP_SW_PORT, BT_MIC_SP_SW_PIN, &stcPortInit);
    PORT_Init(MIC_MUTE_PORT, MIC_MUTE_PIN, &stcPortInit);
    PORT_Init(AF_TONE_SW_PORT, AF_TONE_SW_PIN, &stcPortInit);


	SPK_INTF_CloseTone();
    SPK_INTF_Close();
	SPK_INTF_CloseBTChn();
}
/*******************************************************************************
 * 函数名  : SPK_INTF_Close
 * 描  述  : 关闭喇叭功放
 * 输  入  : 无
 * 输  出  : 无
 * 返回值  : 无
 *******************************************************************************/
DH_VOID SPK_INTF_Close(DH_VOID)
{
    
    PORT_ResetBits(SPK_SHTD_PORT, SPK_SHTD_PIN);
   // printk("spcc..");
}

DH_U8 SPK_INTF_IsOpen(DH_VOID)
{
	return (PORT_GetBit(SPK_SHTD_PORT, SPK_SHTD_PIN));
}

/*******************************************************************************
 * 函数名  : SPK_INTF_Open
 * 描  述  : 打开喇叭功放
 * 输  入  : 无
 * 输  出  : 无
 * 返回值  : 无
 *******************************************************************************/
DH_VOID SPK_INTF_Open(DH_VOID)
{
	//if(!SPK_INTF_IsOpen())
	{
		
		 PORT_SetBits(SPK_SHTD_PORT, SPK_SHTD_PIN);
	}
}

//H Mute
DH_VOID MIC_INTF_Open(DH_VOID)
{
	//if(!SPK_INTF_IsOpen())
	{
		
	PORT_ResetBits(MIC_MUTE_PORT, MIC_MUTE_PIN);
	}
}
DH_VOID MIC_INTF_Mute(DH_VOID)
{
	//if(!SPK_INTF_IsOpen())
	{
		
		PORT_SetBits(MIC_MUTE_PORT, MIC_MUTE_PIN);
	}
}



//单元切换
DH_VOID SPK_INTF_OpenTone(DH_VOID)
{
	 PORT_ResetBits(AF_TONE_SW_PORT, AF_TONE_SW_PIN);
	 MIC_INTF_Mute();
}
DH_VOID SPK_INTF_CloseTone(DH_VOID)
{
	 PORT_SetBits(AF_TONE_SW_PORT, AF_TONE_SW_PIN);
	  MIC_INTF_Open();
}



//bluetooth
DH_VOID SPK_INTF_OpenBTChn(DH_VOID)
{
	PORT_ResetBits(BT_MIC_SP_SW_PORT, BT_MIC_SP_SW_PIN);

}
DH_VOID SPK_INTF_CloseBTChn(DH_VOID)
{
	PORT_SetBits(BT_MIC_SP_SW_PORT, BT_MIC_SP_SW_PIN);

}


