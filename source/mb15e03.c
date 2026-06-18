#include "app_config.h"
#include "mb15e03.h"

///PA1/ADC1_IN1		15E03_CLK	PLL_15E03芯片时钟端口
//PC9		        15E03_DATA	PLL_15E03芯片数据端口
 //PA8		        15E03_LE	PLL_15E03芯片选通端口
#define  MAP_15E03_CLK_PORT        (PortA)
#define  MAP_15E03_CLK_PIN         (Pin01)

#define  MAP_15E03_DATA_PORT        (PortC)
#define  MAP_15E03_DATA_PIN         (Pin09)

#define  MAP_15E03_CS_PORT        (PortA)
#define  MAP_15E03_CS_PIN         (Pin08)


#define IC15E03_CLK_HIGH()  PORT_SetBits(MAP_15E03_CLK_PORT,MAP_15E03_CLK_PIN )
#define IC15E03_CLK_LOW()   PORT_ResetBits(MAP_15E03_CLK_PORT,MAP_15E03_CLK_PIN )

#define IC15E03_DATA_HIGH() PORT_SetBits(MAP_15E03_DATA_PORT,MAP_15E03_DATA_PIN )
#define IC15E03_DATA_LOW()  PORT_ResetBits(MAP_15E03_DATA_PORT,MAP_15E03_DATA_PIN )

#define IC15E03_CS_HIGH() PORT_SetBits(MAP_15E03_CS_PORT,MAP_15E03_CS_PIN )
#define IC15E03_CS_LOW()  PORT_ResetBits(MAP_15E03_CS_PORT,MAP_15E03_CS_PIN )




#define	PLL_DATAOUT	0X20
#define	PLL_DATAREG	PB_DDR
void	SetFreq(unsigned long	int	FreqToSet)
{
	char	i;
	Bool	ret;
	
	DelayUs(1);
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
/*	
	Delay20us();
	Delay20us();
	if(ABandSel)
	{
		if(PLL_A_LD)
			ret=1;
		else
			ret=0;
	}
	else
	{
		if(PLL_B_LD)
			ret=1;
		else
			ret=0;
	}
	return	ret;
	*/
	return;
}
void SendRxFreq(unsigned	long	int	RxFreq,Bool	ABandSel,Bool	UBandSel)
{
/*
以标称19.2M晶振为准.
*/
	double	tail;
	unsigned	long	int	Freq,BaseFreq;
	//if(ABandSel)
	{
//		if(UBandSel)
//			Freq=RxFreq-49950000;
//		else
			Freq=RxFreq+49950000;
		tail=Freq%5000;
		if(tail)
		{
			Freq/=6250;
			BaseFreq=0x051501;		//CS改为1，使用6.0MA
		}	
		else
		{
			Freq/=5000;
			BaseFreq=0x051A41;		//CS改为1，使用6.0MA
		}
	}
//	else
//	{
//		if(UBandSel)
//			Freq=RxFreq-38850000;
//		else
//			Freq=RxFreq+38850000;
//		tail=Freq%5000;
//		if(tail)
//		{
//			Freq/=6250;
//			BaseFreq=0x051801;		//CS改为1，使用6.0MA
//		}	
//		else
//		{
//			Freq/=5000;
//			BaseFreq=0x051e01;		//CS改为1，使用6.0MA
//		}
//
//	}
	Freq<<=1;
	SetFreq(BaseFreq,ABandSel);
	SetFreq(Freq,ABandSel);
}
void	SendTxFreq(unsigned	long	int	TxFreq,Bool	ABandSel)
{
	double	tail;
	unsigned	long	int	Freq,BaseFreq;
	//if(ABandSel)
	{												//A段温补16.8M,中频49.95M
		tail=TxFreq%5000;
		if(tail)
		{
			Freq=TxFreq/6250;
			BaseFreq=0x51501;			//CS改为1，使用6.0MA
		}
		else
		{
			Freq=TxFreq/5000;
			BaseFreq=0x51A41;			//CS改为1，使用6.0MA
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
