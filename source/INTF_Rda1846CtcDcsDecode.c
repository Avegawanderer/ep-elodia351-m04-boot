/*****************************************************************************
 *   文件名:INTF_Rda1846CtcDcsDecode.c
 *   文件描述:亚音软件解码
 *   创建人：yufeng	, 2013/11/18
 ******************************************************************************/

#include "app_config.h"
#include "include.h"

#if FM_DECODE_SEL ==  FM_DECODE_SOFTWARE

DH_U8 g_cdsmatch = 0;
/* 低---高 */
/*Sui1 [0 1 1 0 1 1 0 0 1 1 0]*/
const DH_U16 Sui1_ = 0x0336;
/*Sui2 [1 1 0 1 1 0 0 1 1 0 0]*/
const DH_U16 Sui2_ = 0x019b;

extern DH_U8 CheckctdcsFlg;
extern DH_U8 DcsCodemd;
extern volatile DH_U8 SubDcsCode;
extern DH_U8 analog_on;
extern DH_U8 analog_off;
/*原来的83组解码*/ 
//const DH_U16 DcsCode_table_Normal1[83][5]={
//	{0x0013,0x00e0,0x01f6},/*023*/{0x0015},/*025*/{0x0016,0x0176},/*026*/
//	{0x0019,0x00fc,0x01a3},/*031*/{0x001a},/*032*/{0x0023,0x00ed},/*043*/
//	{0x0027,0x00fd,0x01c7},/*047*/{0x0029,0x01f3,0x0150},/*051*/
//	{0x002c,0x0105,0x01bd},/*054*/{0x0035,0x00c1},/*065*/
//	{0x0039,0x0183,0x01cf,0x01e6},/*071*/{0x003a,0x0138,0x01c1},/*072*/
//	{0x003b,0x01a0},/*073*/{0x003c,0x00f0,0x01d1},/*074*/
//	{0x004c,0x00d7,0x018d},/*114*/{0x004d,0x015c,0x01bc},/*115*/
//	{0x0030,0x004e,0x01df},/*116*/{0x0055,0x007b},/*125*/
//	{0x0059,0x017a,0x01c2},/*131*/{0x0060,0x0185,0x019c,0x01cc},/*132*/
//	{0x005c,0x00bb},/*134*/{0x0063,0x00db},/*143*/
//	{0x006a,0x00f6,0x010d},/*152*/{0x006d,0x009b,0x01b0},/*155*/
//	{0x006e,0x014f,0x01f6},/*156*/{0x0072,0x010e,0x016b},/*162*/
//	{0x0075,0x00ec},/*165*/{0x002f,0x007a},/*172*/
//	{0x0062,0x007c,0x00b8},/*174*/{0x005d,0x0085,0x0188},/*205*/
//	{0x0093,0x00e8,0x013d,0x01e8},/*223*/{0x0044,0x0096,0x016f},/*226*/
//	{0x00a3,0x00b7,0x00e2},/*243*/{0x007e,0x00a4,0x010f},/*244*/
//	{0x00a5,0x00f8,0x016c},/*245*/{0x009e,0x00a9,0x01c4,0x01e2},/*251*/
//	{0x0097,0x00b1,0x0177},/*261*/{0x008b,0x00b3,0x01de},/*263*/
//	{0x0079,0x00b5,0x0116},/*265*/{0x00b9,0x0117,0x0148,0x01f2},/*271*/
//	{0x0067,0x00c3,0x00c6,0x01f1},/*306*/{0x00c9,0x00d8,0x012e,0x0171},/*311*/
//	{0x00cd,0x00fa,0x0147},/*315*/{0x00d9,0x00fa,0x0147},/*331*/
//	{0x00d4,0x00e3,0x0178},/*343*/{0x00e6,0x018e,0x019d,0x01d4},/*346*/
//	{0x00e9,0x00eb,0x011d},/*351*/{0x0058,0x00f4,0x01a1},/*364*/
//	{0x0047,0x00f5},/*365*/{0x008f,0x00f9,0x012b,0x0158},/*371*/
//	{0x004f,0x0109,0x01ee},/*411*/{0x0057,0x010a,0x0121,0x01c9},/*412*/
//	{0x005b,0x010b,0x0190},/*413*/{0x009c,0x0113,0x0173,0x0191,0x01cb},/*423*/
//	{0x00b2,0x00ce,0x0119,0x01d8},/*431*/{0x00be,0x00d6,0x011a},/*432*/
//	{0x0092,0x0125,0x012f,0x017d},/*445*/{0x009f,0x0134,0x01a2,0x01fa},/*464*/
//	{0x0056,0x0135,0x01ae},/*465*/{0x0064,0x0136,0x01b6},/*466*/
//	{0x006f,0x00d2,0x0143},/*503*/{0x0094,0x00cb,0x0146,0x017c},/*506*/
//	{0x0037,0x014e,0x01d0},/*516*/{0x0071,0x00e5,0x015a},/*532*/
//	{0x00cf,0x0166,0x018c,0x01e9},/*546*/{0x00c7,0x00f2,0x0175},/*565*/
//	{0x006b,0x0186,0x0198},/*606*/{0x00ac,0x00cc,0x018a,0x01c6},/*612*/
//	{0x003d,0x0141,0x0194},/*624*/{0x001f,0x0170,0x0197},/*627*/
//	{0x0099,0x0144,0x0199,0x019e,0x01e5},/*631*/{0x0053,0x019a,0x1af},/*632*/
//	{0x0073,0x0130,0x0187,0x01ac},/*654*/{0x00f3,0x011e,0x0123,0x0124,0x01b2},/*662*/
//	{0x00e4,0x0139,0x01b4,0x01cd},/*664*/{0x0068,0x00ae,0x01c3},/*703*/
//	{0x005e,0x0142,0x01ca},/*712*/{0x009d,0x01b9,0x0189,0x01d3},/*723*/
//	{0x0127,0x013b,0x013c,0x01d9,0x01e4},/*731*/{0x0074,0x0087,0x01da},/*732*/
//	{0x0036,0x01dc},/*734*/{0x00ca,0x014d,0x01b3,0x01e3},/*743*/
//	{0x003e,0x0083,0x01ec},/*754*/
//};
//const DH_U8 DcsCode_table_Normal_Total1[83]={
//	3,1,2,3,1,2,3,3,3,2,4,3,2,3,3,3,3,2,3,4,2,2,3,3,3,3,2,2,3,3,
//	4,3,3,3,3,4,3,3,3,4,4,4,3,3,3,4,3,3,2,4,3,4,3,5,4,3,4,4,3,3,
//	3,4,3,3,4,3,3,4,3,3,5,3,4,5,4,3,3,4,5,3,2,4,3
//};
/*新增到108个*/
const DH_U16 DcsCode_table_Normal[CDCSS_NUM][5] =
{
{ 0x000f, 0x0080 },/*017add*/
{ 0x0013, 0x00e0, 0x01f6 },/*023*/
{ 0x0015 },/*025*/
{ 0x0016, 0x0176 },/*026*/
{ 0x0019, 0x00fc, 0x01a3 },/*031*/
{ 0x001a },/*032*/
{ 0x001e, 0x005f },/*036add*/
{ 0x0023, 0x00ed },/*043*/
{ 0x0027, 0x00fd, 0x01c7 },/*047*/
{ 0x0028, 0x0077 },/*050add*/

{ 0x0029, 0x01f3, 0x0150 },/*051*/
{ 0x002b },/*053add*/
{ 0x002c, 0x0105, 0x01bd },/*054*/
{ 0x0035, 0x00c1 },/*065*/
{ 0x0039, 0x0183, 0x01cf, 0x01e6 },/*071*/
{ 0x003a, 0x0138, 0x01c1 },/*072*/
{ 0x003b, 0x01a0 },/*073*/
{ 0x003c, 0x00f0, 0x01d1 },/*074*/
{ 0x004c, 0x00d7, 0x018d },/*114*/
{ 0x004d, 0x015c, 0x01bc },/*115*/

{ 0x0030, 0x004e, 0x01df },/*116*/
{ 0x0052, 0x015d },/*122add*/
{ 0x0055, 0x007b },/*125*/
{ 0x0059, 0x017a, 0x01c2 },/*131*/
{ 0x0060, 0x0185, 0x019c, 0x01cc },/*132*/
{ 0x005c, 0x00bb },/*134*/
{ 0x0063, 0x00db },/*143*/
{ 0x0065, 0x0155 },/*145add*/
{ 0x006a, 0x00f6, 0x010d },/*152*/
{ 0x006d, 0x009b, 0x01b0 },/*155*/

{ 0x006e, 0x014f, 0x01f6 },/*156*/
{ 0x0072, 0x010e, 0x016b },/*162*/
{ 0x0075, 0x00ec },/*165*/
{ 0x002f, 0x007a },/*172*/
{ 0x0062, 0x007c, 0x00b8 },/*174*/
{ 0x005d, 0x0085, 0x0188 },/*205*/
{ 0x008a, 0x00ab },/*212add*/
{ 0x0093, 0x00e8, 0x013d, 0x01e8 },/*223*/
{ 0x0095, 0x015e },/*225add*/
{ 0x0044, 0x0096, 0x016f },/*226*/

{ 0x00a3, 0x00b7, 0x00e2 },/*243*/
{ 0x007e, 0x00a4, 0x010f },/*244*/
{ 0x00a5, 0x00f8, 0x016c },/*245*/
{ 0x00a6, 0x0162, 0x01ab },/*246add*/
{ 0x009e, 0x00a9, 0x01c4, 0x01e2 },/*251*/
{ 0x00aa, 0x01b1 },/*252add*/
{ 0x00ad, 0x0115 },/*255add*/
{ 0x0097, 0x00b1, 0x0177 },/*261*/
{ 0x008b, 0x00b3, 0x01de },/*263*/
{ 0x0079, 0x00b5, 0x0116 },/*265*/

{ 0x00b6, 0x01ad },/*266add*/
{ 0x00b9, 0x0117, 0x0148, 0x01f2 },/*271*/
{ 0x00bc, 0x01aa },/*274add*/
{ 0x0067, 0x00c3, 0x00c6, 0x01f1 },/*306*/
{ 0x00c9, 0x00d8, 0x012e, 0x0171 },/*311*/
{ 0x00cd, 0x00fa, 0x0147 },/*315*/
{ 0x00d5, 0x0168, 0x0196 },/*325add*/
{ 0x00d9, 0x00fa, 0x0147 },/*331*/
{ 0x00da, 0x011b, 0x016a },/*332add*/
{ 0x00d4, 0x00e3, 0x0178 },/*343*/

{ 0x00e6, 0x018e, 0x019d, 0x01d4 },/*346*/
{ 0x00e9, 0x00eb, 0x011d },/*351*/
{ 0x00ee, 0x0151 },/*356add*/
{ 0x0058, 0x00f4, 0x01a1 },/*364*/
{ 0x0047, 0x00f5 },/*365*/
{ 0x008f, 0x00f9, 0x012b, 0x0158 },/*371*/
{ 0x004f, 0x0109, 0x01ee },/*411*/
{ 0x0057, 0x010a, 0x0121, 0x01c9 },/*412*/
{ 0x005b, 0x010b, 0x0190 },/*413*/
{ 0x009c, 0x0113, 0x0173, 0x0191, 0x01cb },/*423*/

{ 0x00b2, 0x00ce, 0x0119, 0x01d8 },/*431*/
{ 0x00be, 0x00d6, 0x011a },/*432*/
{ 0x0092, 0x0125, 0x012f, 0x017d },/*445*/
{ 0x0126, 0x0137, 0x0149, 0x01ba },/*446add*/
{ 0x012a, 0x0145, 0x01f5 },/*452add*/
{ 0x012c, 0x0165, 0x014b, 0x0174 },/*454add*/
{ 0x012d, 0x015b, 0x0169 },/*455add*/
{ 0x0132, 0x013a, 0x0193, 0x01d5 },/*462add*/
{ 0x009f, 0x0134, 0x01a2, 0x01fa },/*464*/
{ 0x0056, 0x0135, 0x01ae },/*465*/

{ 0x0064, 0x0136, 0x01b6 },/*466*/
{ 0x006f, 0x00d2, 0x0143 },/*503*/
{ 0x0094, 0x00cb, 0x0146, 0x017c },/*506*/
{ 0x0037, 0x014e, 0x01d0 },/*516*/
{ 0x0153, 0x01a7, 0x01d6 },/*523add*/
{ 0x0156, 0x0172, 0x01a5 },/*526add*/
{ 0x0071, 0x00e5, 0x015a },/*532*/
{ 0x00cf, 0x0166, 0x018c, 0x01e9 },/*546*/
{ 0x00c7, 0x00f2, 0x0175 },/*565*/
{ 0x006b, 0x0186, 0x0198 },/*606*/

{ 0x00ac, 0x00cc, 0x018a, 0x01c6 },/*612*/
{ 0x003d, 0x0141, 0x0194 },/*624*/
{ 0x001f, 0x0170, 0x0197 },/*627*/
{ 0x0099, 0x0144, 0x0199, 0x019e, 0x01e5 },/*631*/
{ 0x0053, 0x019a, 0x1af },/*632*/
{ 0x0156, 0x0172, 0x01a5 }, //{0x01a5,0x01b5},/*645add20151130zsy*/
		{ 0x01a6, 0x01b5 },/*646add*/
		{ 0x0073, 0x0130, 0x0187, 0x01ac },/*654*/
		{ 0x00f3, 0x011e, 0x0123, 0x0124, 0x01b2 },/*662*/
		{ 0x00e4, 0x0139, 0x01b4, 0x01cd },/*664*/

		{ 0x0068, 0x00ae, 0x01c3 },/*703*/
		{ 0x005e, 0x0142, 0x01ca },/*712*/
		{ 0x009d, 0x01b9, 0x0189, 0x01d3 },/*723*/
		{ 0x0127, 0x013b, 0x013c, 0x01d9, 0x01e4 },/*731*/
		{ 0x0074, 0x0087, 0x01da },/*732*/
		{ 0x0036, 0x01dc },/*734*/
		{ 0x00ca, 0x014d, 0x01b3, 0x01e3 },/*743*/
		{ 0x003e, 0x0083, 0x01ec }, /*754*/
};

const DH_U8 DcsCode_table_Normal_Total[CDCSS_NUM] =
{ 2, //61
		3, 1, 2, //62
		3, 1, 2, //63
		2, 3, 2, //64
		3, 1, 3, //65
		2, 4, 3, //66
		2, 3, 3, //67
		3, 3, 2, //68
		2, 3, 4, //69
		2, 2, 2, //70
		3, 3, 3, //71
		3, 2, 2, //72
		3, 3, 2, //73
		4, 2, 3, //74
		3, 3, 3, //75
		3, 4, 2, //76
		2, 3, 3, //77
		3, 2, 4, //78
		2, 4, 4, //79
		3, 3, 3, //80
		3, 3, 4, //81
		3, 2, 3, //82
		2, 4, 3, //83
		4, 3, 5, //84
		4, 3, 4, //85
		4, 3, 4, //86
		3, 4, 4, //87
		3, 3, 3, //88
		4, 3, 3, //89
		3, 3, 4, //90
		3, 3, 4, //91
		3, 3, 5, //92

		3, 3,/*645add20151130zsy*/
		2, 4, //93

		5, 4, 3, //94
		3, 4, 5, //95
		3, 2, 4, 3, //96
		};

typedef struct
{
	DH_U16 b0 :1;
	DH_U16 b1 :1;
	DH_U16 b2 :1;
	DH_U16 b3 :1;
	DH_U16 b4 :1;
	DH_U16 b5 :1;
	DH_U16 b6 :1;
	DH_U16 b7 :1;
	DH_U16 b8 :1;
	DH_U16 b9 :1;
	DH_U16 b10 :1;
	DH_U16 reserve :5;
} GOLAY_11B;

typedef union
{
	DH_U16 golaysInit;
	GOLAY_11B golays;
} GOLAY_STR1;

typedef union
{
	DH_U16 golaysInit;
	GOLAY_11B s;
} GOLAY_STR2;

typedef union
{
	DH_U16 golaysInit;
	GOLAY_11B s1;
} GOLAY_STR3;

typedef struct
{
	DH_U32 b :23;
	DH_U32 reserve :9;
} DCS_23B;

typedef union
{
	DH_U32 DataInInit;
	DCS_23B DataIn;
} DCS_STR1;

typedef union
{
	DH_U32 DataOutInit;
	DCS_23B DataOut;
} DCS_STR2;

typedef union
{
	DH_U32 EInit;
	DCS_23B E;
} DCS_STR3;

DCS_STR1 Dcs_Str1_CodeIn1;
DCS_STR1 Dcs_Str1_CodeIn2;
DCS_STR1 Dcs_Str1_CodeIn3;
DCS_STR1 Dcs_Str1_CodeIn4;
DCS_STR1 Dcs_Str1_CodeIn5;
DCS_STR2 Dcs_Str1_CodeOut1;
DCS_STR2 Dcs_Str1_CodeOut2;
DCS_STR2 Dcs_Str1_CodeOut3;
DCS_STR2 Dcs_Str1_CodeOut4;
DCS_STR2 Dcs_Str1_CodeOut5;

void DcsParaClr(void)
{
	Dcs_Str1_CodeIn1.DataInInit = 0;
	Dcs_Str1_CodeIn2.DataInInit = 0;
	Dcs_Str1_CodeIn3.DataInInit = 0;
	Dcs_Str1_CodeIn4.DataInInit = 0;
	Dcs_Str1_CodeIn5.DataInInit = 0;
	Dcs_Str1_CodeOut1.DataOutInit = 0;
	Dcs_Str1_CodeOut2.DataOutInit = 0;
	Dcs_Str1_CodeOut3.DataOutInit = 0;
	Dcs_Str1_CodeOut4.DataOutInit = 0;
	Dcs_Str1_CodeOut5.DataOutInit = 0;
}

DH_U16 GolayBitCount(DH_U16 n)
{
	DH_U16 count;
	for (count = 0; n; n >>= 1)
	{
		count += n & 1;
	}
	return count;
}
DH_U8 GolayBit1Cnt(DH_U32 n)
{
	DH_U8 count;
	for (count = 0; n; n >>= 1)
	{
		count += n & 1;
	}
	return count;
}
/*************************************************************
 *函数名称:BitShift
 *函数功能:倒序
 *作    者:yufeng
 *日    期:2013/11/18
 **************************************************************/
DH_U32 BitShift(DH_U32 buf, DH_U8 len)
{
	DH_U32 n = 0;
	DH_U8 i;

	for (i = 0; i < len; i++)
	{
		n |= ((buf >> i) & 0x01) << (len - i - 1);
	}
	return n;
}

/*******************************************************************************
 *函数名:GolayDecode_

 *函数说明:格雷译码

 *返回值:

 *创建人:yufeng

 *创建日期:2013/11/18
 *******************************************************************************/
DH_U32 GolayDecode_(DCS_STR1 Dcs_Str1, DH_U8 len)
{
	DH_U8 i;
	DH_U8 j;
	GOLAY_STR1 Golay_Str1;
	GOLAY_STR2 Golay_Str2;
	GOLAY_STR3 Golay_Str3;
	DCS_STR2 Dcs_Str2;
	DCS_STR3 Dcs_Str3;

	Dcs_Str2.DataOutInit = 0;
	Dcs_Str3.EInit = 0;
	Golay_Str1.golaysInit = 0;
	Golay_Str2.golaysInit = 0;
	Golay_Str3.golaysInit = 0;
	if (DcsCodemd)
		Dcs_Str1.DataInInit = ~Dcs_Str1.DataInInit;
	for (i = 0; i < len; i++)
	{
		for (j = 0; j < len; j++)
		{
			Golay_Str2.s.b10 = (Dcs_Str1.DataInInit >> (22 - j))
					^ Golay_Str1.golays.b0;
			Golay_Str2.s.b9 = Golay_Str1.golays.b10;
			Golay_Str2.s.b8 = Golay_Str1.golays.b9 ^ Golay_Str1.golays.b0;
			Golay_Str2.s.b7 = Golay_Str1.golays.b8;
			Golay_Str2.s.b6 = Golay_Str1.golays.b7 ^ Golay_Str1.golays.b0;
			Golay_Str2.s.b5 = Golay_Str1.golays.b6 ^ Golay_Str1.golays.b0;
			Golay_Str2.s.b4 = Golay_Str1.golays.b5 ^ Golay_Str1.golays.b0;
			Golay_Str2.s.b3 = Golay_Str1.golays.b4;
			Golay_Str2.s.b2 = Golay_Str1.golays.b3;
			Golay_Str2.s.b1 = Golay_Str1.golays.b2;
			Golay_Str2.s.b0 = Golay_Str1.golays.b1 ^ Golay_Str1.golays.b0;

			Golay_Str1.golaysInit = Golay_Str2.golaysInit;
		}
		Golay_Str3.s1.b0 = Golay_Str1.golays.b10;
		Golay_Str3.s1.b1 = Golay_Str1.golays.b9;
		Golay_Str3.s1.b2 = Golay_Str1.golays.b8;
		Golay_Str3.s1.b3 = Golay_Str1.golays.b7;
		Golay_Str3.s1.b4 = Golay_Str1.golays.b6;
		Golay_Str3.s1.b5 = Golay_Str1.golays.b5;
		Golay_Str3.s1.b6 = Golay_Str1.golays.b4;
		Golay_Str3.s1.b7 = Golay_Str1.golays.b3;
		Golay_Str3.s1.b8 = Golay_Str1.golays.b2;
		Golay_Str3.s1.b9 = Golay_Str1.golays.b1;
		Golay_Str3.s1.b10 = Golay_Str1.golays.b0;

		Golay_Str1.golaysInit = 0;

		if (GolayBitCount(Golay_Str3.golaysInit) <= 3)
		{
			Dcs_Str3.EInit = Golay_Str3.golaysInit;
			Dcs_Str2.DataOutInit = Dcs_Str1.DataInInit ^ Dcs_Str3.EInit;

			//Dcs_Str2.DataOutInit = BitShift(Dcs_Str2.DataOutInit,23);
			break;
		}
		else if (GolayBitCount(Golay_Str3.golaysInit ^ Sui1_) <= 2)
		{
			Dcs_Str3.EInit = ((Golay_Str3.golaysInit ^ Sui1_) << 12) | (1 << 6);
			Dcs_Str2.DataOutInit = Dcs_Str1.DataInInit ^ Dcs_Str3.EInit;

			//Dcs_Str2.DataOutInit = BitShift(Dcs_Str2.DataOutInit,23);
			break;
		}
		else if (GolayBitCount(Golay_Str3.golaysInit ^ Sui2_) <= 2)
		{
			Dcs_Str3.EInit = ((Golay_Str3.golaysInit ^ Sui2_) << 12) | (1 << 5);
			Dcs_Str2.DataOutInit = Dcs_Str1.DataInInit ^ Dcs_Str3.EInit;

			//Dcs_Str2.DataOutInit = BitShift(Dcs_Str2.DataOutInit,23);
			break;
		}
		else
		{
			Dcs_Str1.DataInInit = BitShift(Dcs_Str1.DataInInit, 23);
		}
	}
	return Dcs_Str2.DataOutInit;
}

void CompareCdcss23Bit(DH_U32 buf, DH_U8 len, DH_U8* err)
{
	DH_U8 i;
	DH_U8 j;
	DH_U32 data = buf;
	DH_U32 result;

	*err = FALSE;

	for (i = 0; i < len; i++)
	{
		result = data & 0xe00;
		if (result == 0x800)
		{
			result = data & 0x1ff;
			//if((result==0x013)||(result==0x0e0)||(result==0x1f6))
			for (j = 0; j < DcsCode_table_Normal_Total[SubDcsCode]; j++)
			{
				if (result == DcsCode_table_Normal[SubDcsCode][j])
				{
					*err = TRUE;
					return;
				}
			}
		}

		data <<= 1;
		data = (data & 0x7fffff) | ((buf >> (len - i - 1)) & 0x1);
	}
}

DH_U8 CompareCdcssTail(DH_U32 data1, DH_U32 data2, DH_U32 data3, DH_U32 data4,
		DH_U32 data5)
{
static DH_U8 CdcssTailCnt = 0;
	static DH_U8 CdcssTailErr = 0;

	if (data1 == 0x7fffff)
	{
		if ((data2 == 0x00) || (data3 == 0x00) || (data4 == 0x00)
				|| (data5 == 0x00))
		{
			CdcssTailCnt++;
			CdcssTailErr = 0;
		}
	}
	else if (data1 == 0x00)
	{
		if ((data2 == 0x7fffff) || (data3 == 0x7fffff) || (data4 == 0x7fffff)
				|| (data5 == 0x7fffff))
		{
			CdcssTailCnt++;
			CdcssTailErr = 0;
		}
	}
	else if (data2 == 0x7fffff)
	{
		if ((data3 == 0x00) || (data4 == 0x00) || (data5 == 0x00))
		{
			CdcssTailCnt++;
			CdcssTailErr = 0;
		}
	}
	else if (data2 == 0x00)
	{
		if ((data3 == 0x7fffff) || (data4 == 0x7fffff) || (data5 == 0x7fffff))
		{
			CdcssTailCnt++;
			CdcssTailErr = 0;
		}
	}
	else if (data3 == 0x7fffff)
	{
		if ((data4 == 0x00) || (data5 == 0x00))
		{
			CdcssTailCnt++;
			CdcssTailErr = 0;
		}
	}
	else if (data3 == 0x00)
	{
		if ((data4 == 0x7fffff) || (data5 == 0x7fffff))
		{
			CdcssTailCnt++;
			CdcssTailErr = 0;
		}
	}
	else if (data4 == 0x7fffff)
	{
		if (data5 == 0x00)
		{
			CdcssTailCnt++;
			CdcssTailErr = 0;
		}
	}
	else if (data4 == 0x00)
	{
		if (data5 == 0x7fffff)
		{
			CdcssTailCnt++;
			CdcssTailErr = 0;
		}
	}
	else
	{
		CdcssTailErr++;
		if (CdcssTailErr >40)
		{
			CdcssTailCnt = 0;
			CdcssTailErr = 0;
		}
	}

	if(GolayBit1Cnt((data1&0x7fffff))>20
		||GolayBit1Cnt((data2&0x7fffff))>20
		||GolayBit1Cnt((data3&0x7fffff))>20
		||GolayBit1Cnt((data4&0x7fffff))>20
		||GolayBit1Cnt((data5&0x7fffff))>20
		)
	{
	      CdcssTailCnt=0;
	      CdcssTailErr = 0;
	    return TRUE;
	}


	if (CdcssTailCnt > 20)
	{
		analog_off = 8;
		//dhevent_set(&lwevent_int_grp, FM_INT_ONOFF_EV);
		CdcssTailCnt = 0;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void RecevieCdcssCdcssTailCnt(void)
{
	//printk("CdcssTailCnt %d\r\n", CdcssTailCnt);
}
void RecevieCdcss23bit(void)
{
	static DH_U8 step = 0;
	static DH_U8 OpenFlag = 0;
	static DH_U16 CloseFlag = 0;
	DH_U8 cnt = 0;
	DH_U8 err;
	DH_U32 iod;
	if (!CheckctdcsFlg)/*是否已经开始解数字信令*/
	{
		return;
	}
	//static DH_U8 flg=0;
	//static DH_U32 Dcsdata[5][40];
	//static DH_U8 n[5];

//  iod=(GPIO_ReadInputDataBit(RF1_PORT,RDA_CDCSS_PIN)<<22);
	iod = (GPIO_DRV_GetInputStat(RDA_CDCSS_PIN) << 22);
	if (step == 0)
	{
		step = 1;
		Dcs_Str1_CodeIn1.DataInInit >>= 1;
		Dcs_Str1_CodeIn1.DataInInit |= iod;
		Dcs_Str1_CodeOut1.DataOutInit = GolayDecode_(Dcs_Str1_CodeIn1, 23);
	}
	else if (step == 1)
	{
		step = 2;
		Dcs_Str1_CodeIn2.DataInInit >>= 1;
		Dcs_Str1_CodeIn2.DataInInit |= iod;
		Dcs_Str1_CodeOut2.DataOutInit = GolayDecode_(Dcs_Str1_CodeIn2, 23);
	}
	else if (step == 2)
	{
		step = 3;
		Dcs_Str1_CodeIn3.DataInInit >>= 1;
		Dcs_Str1_CodeIn3.DataInInit |= iod;
		Dcs_Str1_CodeOut3.DataOutInit = GolayDecode_(Dcs_Str1_CodeIn3, 23);
	}
	else if (step == 3)
	{
		step = 4;
		Dcs_Str1_CodeIn4.DataInInit >>= 1;
		Dcs_Str1_CodeIn4.DataInInit |= iod;
		Dcs_Str1_CodeOut4.DataOutInit = GolayDecode_(Dcs_Str1_CodeIn4, 23);
	}
	else if (step == 4)
	{
		step = 0;
		Dcs_Str1_CodeIn5.DataInInit >>= 1;
		Dcs_Str1_CodeIn5.DataInInit |= iod;
		Dcs_Str1_CodeOut5.DataOutInit = GolayDecode_(Dcs_Str1_CodeIn5, 23);
	}

	CompareCdcss23Bit(Dcs_Str1_CodeOut1.DataOutInit, 23, &err);
	if (err == TRUE)
	{
		cnt++;
	}
	CompareCdcss23Bit(Dcs_Str1_CodeOut2.DataOutInit, 23, &err);
	if (err == TRUE)
	{
		cnt++;
	}

	if (cnt < 2)
	{
		CompareCdcss23Bit(Dcs_Str1_CodeOut3.DataOutInit, 23, &err);
		if (err == TRUE)
		{
			cnt++;
		}

		if (cnt < 2)
		{
			CompareCdcss23Bit(Dcs_Str1_CodeOut4.DataOutInit, 23, &err);
			if (err == TRUE)
			{
				cnt++;
			}

			if (cnt < 2)
			{
				CompareCdcss23Bit(Dcs_Str1_CodeOut5.DataOutInit, 23, &err);
				if (err == TRUE)
				{
					cnt++;
				}
			}
		}
	}

	if (cnt > 1)
	{
		cnt = 0;
		OpenFlag++;
		CloseFlag = 0;
	}
	else
	{
		cnt = 0;
		CloseFlag++;
		OpenFlag = 0;
	}

	if (OpenFlag > 23)
	{
		OpenFlag = 0;
		analog_on = 5;
		g_cdsmatch = 1;
	}

	if (CloseFlag > RDA_RX_DCS_OFF_DELAY)  // 200
	{
		CloseFlag = 0;
		analog_off = 7;
	}

	if (CompareCdcssTail(Dcs_Str1_CodeIn1.DataInInit,
			Dcs_Str1_CodeIn2.DataInInit, Dcs_Str1_CodeIn3.DataInInit,
			Dcs_Str1_CodeIn4.DataInInit, Dcs_Str1_CodeIn5.DataInInit))
	{
		analog_off = 8;
	}
}

#endif
