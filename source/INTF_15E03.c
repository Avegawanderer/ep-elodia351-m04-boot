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
#include "INTF_15E03.h"
#include "include.h"

#define IC15E03_IC_STAT 1
//
//PA1/ADC1_IN1		15E03_CLK	PLL_15E03芯片时钟端口
//PC9		        15E03_DATA	PLL_15E03芯片数据端口
//PA8		        15E03_LE	PLL_15E03芯片选通端口
//PA6/ADC12_IN6		PLL_LD	PLL失锁检测端口	L有效	

#define  MAP_15E03_CLK_PORT        (PortA)
#define  MAP_15E03_CLK_PIN         (Pin01)

#define  MAP_15E03_DATA_PORT        (PortC)
#define  MAP_15E03_DATA_PIN         (Pin09)

#define  MAP_15E03_CS_PORT        (PortA)
#define  MAP_15E03_CS_PIN         (Pin08)

#define  MAP_PLL_LD_PORT        (PortA)
#define  MAP_PLL_LD_PIN         (Pin06)


#define IC15E03_CLK_HIGH()  PORT_SetBits(MAP_15E03_CLK_PORT,MAP_15E03_CLK_PIN )
#define IC15E03_CLK_LOW()   PORT_ResetBits(MAP_15E03_CLK_PORT,MAP_15E03_CLK_PIN )

#define IC15E03_DATA_HIGH() PORT_SetBits(MAP_15E03_DATA_PORT,MAP_15E03_DATA_PIN )
#define IC15E03_DATA_LOW()  PORT_ResetBits(MAP_15E03_DATA_PORT,MAP_15E03_DATA_PIN )

#define IC15E03_CS_HIGH() PORT_SetBits(MAP_15E03_CS_PORT,MAP_15E03_CS_PIN )
#define IC15E03_CS_LOW()  PORT_ResetBits(MAP_15E03_CS_PORT,MAP_15E03_CS_PIN )


/*******************************************************************************
*函数名:15E03IC_INTF_Init

*函数说明:

*参数说明:无

*返回值:无

*创建人:yufeng

*创建日期:2012/01/31
*******************************************************************************/

void IC15E03_INTF_GPIOInit(void)
{
#if IC15E03_IC_STAT
    stc_port_init_t stcPortInit;

    /* configuration structure initialization */
    MEM_ZERO_STRUCT(stcPortInit);

    stcPortInit.enExInt   = Disable;
    stcPortInit.enPullUp  = Enable;
    stcPortInit.enPinMode = Pin_Mode_Out;
    /*  Port/Pin initialization */
    PORT_Init(MAP_15E03_CLK_PORT,MAP_15E03_CLK_PIN, &stcPortInit);
    PORT_Init(MAP_15E03_DATA_PORT,MAP_15E03_DATA_PIN, &stcPortInit);
    PORT_Init(MAP_15E03_CS_PORT,MAP_15E03_CS_PIN, &stcPortInit);
    IC15E03_CS_HIGH();

	
    stcPortInit.enPinMode = Pin_Mode_In;
    PORT_Init(MAP_PLL_LD_PORT,MAP_PLL_LD_PIN, &stcPortInit);
#endif
}

#define Delay20us() Delay_us(20)
static void	SetFreq(unsigned long	int	FreqToSet,uint8 ABandSel)
{
	char	i;
//	uint8	ret;
	
	Delay_us(1);
	IC15E03_DATA_LOW();
	IC15E03_CLK_LOW();
	IC15E03_CS_LOW();
	for(i=19;i>0;i--)
	{
		Delay20us();
		if((FreqToSet>>(i-1))&0x01)
			IC15E03_DATA_HIGH();
		else
			IC15E03_DATA_LOW();
		Delay20us();
		IC15E03_CLK_LOW();
		Delay20us();
		IC15E03_CLK_HIGH();
	}
	Delay20us();

	IC15E03_CS_HIGH();
	Delay20us();
	Delay20us();
	
	IC15E03_CS_LOW();

	Delay20us();
	IC15E03_CLK_LOW();
	IC15E03_DATA_LOW();

	return;
}

void	IC15E03_INTF_SetTxFreq(unsigned	long	int	TxFreq,uint8	ABandSel)
{
printk("settxfreq %d\r\n",TxFreq);
	double	tail;
	unsigned	long	int	Freq,BaseFreq;
	//if(ABandSel)
	{												//A段温补16.8M,中频49.95M
		tail=TxFreq%5000;
		if(tail)
		{
			Freq=TxFreq/6250;
			BaseFreq=0x51501 & (0x1F<<15 | 0x01);			//CS改为1，使用6.0MA
			BaseFreq |= ((13000000/6250)<<1);
		}
		else
		{
			Freq=TxFreq/5000;
			//BaseFreq=0x051A41;		//CS改为1，使用6.0MA 16.8M
			BaseFreq=0x051451;		//CS改为1，使用6.0MA 13M
			BaseFreq=0x51501 & (0x1F<<15 | 0x01);			//CS改为1，使用6.0MA
			BaseFreq |= ((13000000/5000)<<1);
		}
	}
//	else
//	{											//A段温补19.2M,中频38.85M
//		tail=TxFreq%5000;
//		if(tail)
//		{
//			Freq=TxFreq/6250;
//			BaseFreq=0x51801;			//CS改为1，使用6.0MA
//		}
//		else
//		{
//			Freq=TxFreq/5000;
//			BaseFreq=0x51e01;			//CS改为1，使用6.0MA
//		}
//	}
	Freq<<=1;
	SetFreq(BaseFreq,ABandSel);
	SetFreq(Freq,ABandSel);	
}

void IC15E03_INTF_SetRxFreq(unsigned	long	int	RxFreq,uint8	ABandSel,uint8	UBandSel)
{
printk("setrxfreq %d\r\n",RxFreq);

	RxFreq+=FREQ_UPDOWN;
	IC15E03_INTF_SetTxFreq(RxFreq,0);
}

