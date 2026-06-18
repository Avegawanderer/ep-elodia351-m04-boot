#ifndef _MB15E03_H_
#define _MB15E03_H_
void	SendRxFreq(unsigned	long	int	RxFreq,Bool	ABandSel,Bool	UBandSel);
void	SendTxFreq(unsigned	long	int	TxFreq,Bool	ABandSel);
void	SetFreq(unsigned	long int	FreqToSet,Bool	ABandSel);
#endif
