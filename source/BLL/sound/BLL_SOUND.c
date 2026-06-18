/*******************************************************************************
 * Copyright (C) 2011-2013 ZheJiang Dahua Technology CO.,LTD.
 *
 * 文件名 : BLL_SOUND.c
 *
 * 作  者 : Li_endi(24539)<li_endi@dahuatech.com>
 * 版  本 : V1.0.0
 * 日  期 : 2015年5月27日 Create
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
/*                             头文件区                                                                                              */
/* ========================================================================== */
#include "app_config.h"

#include "include.h"
/* ========================================================================== */
/*                           宏和类型定义区                                                                                      */
/* ========================================================================== */

#define USE_Sem_SoundFlg 0
/* 上锁宏定义 */
#define CODECLOCK() // dhsem_wait(&CodecLock, 0)
#define CODECUNLOCK()// dhsem_post(&CodecLock)


//#define SoundMutexLock()   OSMutexPend(&Sound_Mutex,0,OS_OPT_PEND_BLOCKING,&ts,&err)
//#define SoundMutexUnLock() OSMutexPost(&Sound_Mutex,OS_OPT_POST_NONE,&err)

///* 按键提示音大小的最大和最小值 */
//#define KEYVOICEVOLUME_MAX 13
//#define KEYVOICEVOLUME_MIN 1

/* ========================================================================== */
/*                          数据结构定义区                                                                                         */
/* ========================================================================== */


/* 提示音状态值 */
DH_U8 SoundStat;
DH_U8 SoundStatExu;           /* 提示音大小偏移值 */
DH_U8 g_SoundLevel = 100;
static DH_U8 CodecStat;       /*编解码的状态值*/

//CPU_STK TaskSoundStk[TASKSOUND_STK_SIZE];
//OS_TCB TaskSoundTCB={0};

//OS_SEM CodecLock;
//OS_SEM Sem_SoundFlg;

const DH_S16 gas16CodecBuf[]={
0x0000, 0x0019, 0x0032,  0x004B, 0x0065, 0x007E,  0x0097, 0x00B0, 0x00C9,  0x00E2, 0x00FB,
0x0114, 0x012D, 0x0147,  0x0160, 0x0179, 0x0192,  0x01AB, 0x01C4, 0x01DD,  0x01F6, 0x020F,
0x0228, 0x0242, 0x025B,  0x0274, 0x028D, 0x02A6,  0x02BF, 0x02D8, 0x02F1,  0x030A, 0x0323,
0x033C, 0x0355, 0x036E,  0x0387, 0x03A0, 0x03B9,  0x03D2, 0x03EB, 0x0404,  0x041D, 0x0435,
0x044E, 0x0467, 0x0480,  0x0499, 0x04B2, 0x04CB,  0x04E4, 0x04FC, 0x0515,  0x052E, 0x0547,
0x0560, 0x0578, 0x0591,  0x05AA, 0x05C3, 0x05DB,  0x05F4, 0x060D, 0x0625,  0x063E, 0x0657,
0x066F, 0x0688, 0x06A0,  0x06B9, 0x06D2, 0x06EA,  0x0703, 0x071B, 0x0734,  0x074C, 0x0765,
0x077D, 0x0795, 0x07AE,  0x07C6, 0x07DF, 0x07F7,  0x080F, 0x0828, 0x0840,  0x0858, 0x0870,
0x0889, 0x08A1, 0x08B9,  0x08D1, 0x08E9, 0x0901,  0x091A, 0x0932, 0x094A,  0x0962, 0x097A,
0x0992, 0x09AA, 0x09C2,  0x09DA, 0x09F1, 0x0A09,  0x0A21, 0x0A39, 0x0A51,  0x0A69, 0x0A80,
0x0A98, 0x0AB0, 0x0AC7,  0x0ADF, 0x0AF7, 0x0B0E,  0x0B26, 0x0B3D, 0x0B55,  0x0B6C, 0x0B84,
0x0B9B, 0x0BB3, 0x0BCA,  0x0BE1, 0x0BF9, 0x0C10,  0x0C27, 0x0C3F, 0x0C56,  0x0C6D, 0x0C84,
0x0C9B, 0x0CB2, 0x0CC9,  0x0CE0, 0x0CF7, 0x0D0E,  0x0D25, 0x0D3C, 0x0D53,  0x0D6A, 0x0D81,
0x0D97, 0x0DAE, 0x0DC5,  0x0DDB, 0x0DF2, 0x0E09,  0x0E1F, 0x0E36, 0x0E4C,  0x0E63, 0x0E79,
0x0E90, 0x0EA6, 0x0EBC,  0x0ED3, 0x0EE9, 0x0EFF,  0x0F15, 0x0F2B, 0x0F41,  0x0F58, 0x0F6E,
0x0F84, 0x0F9A, 0x0FAF,  0x0FC5, 0x0FDB, 0x0FF1,  0x1007, 0x101C, 0x1032,  0x1048, 0x105D,
0x1073, 0x1089, 0x109E,  0x10B3, 0x10C9, 0x10DE,  0x10F4, 0x1109, 0x111E,  0x1133, 0x1149,
0x115E, 0x1173, 0x1188,  0x119D, 0x11B2, 0x11C7,  0x11DC, 0x11F0, 0x1205,  0x121A, 0x122F,
0x1243, 0x1258, 0x126C,  0x1281, 0x1295, 0x12AA,  0x12BE, 0x12D3, 0x12E7,  0x12FB, 0x130F,
0x1324, 0x1338, 0x134C,  0x1360, 0x1374, 0x1388,  0x139C, 0x13AF, 0x13C3,  0x13D7, 0x13EB,
0x13FE, 0x1412, 0x1425,  0x1439, 0x144C, 0x1460,  0x1473, 0x1486, 0x149A,  0x14AD, 0x14C0,
0x14D3, 0x14E6, 0x14F9,  0x150C, 0x151F, 0x1532,  0x1545, 0x1557, 0x156A,  0x157D, 0x158F,
0x15A2, 0x15B4, 0x15C7,  0x15D9, 0x15EC, 0x15FE,  0x1610, 0x1622, 0x1634,  0x1646, 0x1658,
0x166A, 0x167C, 0x168E,  0x16A0, 0x16B2, 0x16C3,  0x16D5, 0x16E7, 0x16F8,  0x170A, 0x171B,
0x172C, 0x173E, 0x174F,  0x1760, 0x1771, 0x1782,  0x1793, 0x17A4, 0x17B5,  0x17C6, 0x17D7,
0x17E8, 0x17F8, 0x1809,  0x1819, 0x182A, 0x183A,  0x184B, 0x185B, 0x186B,  0x187B, 0x188C,
0x189C, 0x18AC, 0x18BC,  0x18CC, 0x18DB, 0x18EB,  0x18FB, 0x190B, 0x191A,  0x192A, 0x1939,
0x1949, 0x1958, 0x1967,  0x1977, 0x1986, 0x1995,  0x19A4, 0x19B3, 0x19C2,  0x19D1, 0x19E0,
0x19EE, 0x19FD, 0x1A0C,  0x1A1A, 0x1A29, 0x1A37,  0x1A46, 0x1A54, 0x1A62,  0x1A70, 0x1A7F,
0x1A8D, 0x1A9B, 0x1AA8,  0x1AB6, 0x1AC4, 0x1AD2,  0x1AE0, 0x1AED, 0x1AFB,  0x1B08, 0x1B16,
0x1B23, 0x1B30, 0x1B3D,  0x1B4B, 0x1B58, 0x1B65,  0x1B72, 0x1B7F, 0x1B8B,  0x1B98, 0x1BA5,
0x1BB1, 0x1BBE, 0x1BCA,  0x1BD7, 0x1BE3, 0x1BF0,  0x1BFC, 0x1C08, 0x1C14,  0x1C20, 0x1C2C,
0x1C38, 0x1C44, 0x1C4F,  0x1C5B, 0x1C67, 0x1C72,  0x1C7E, 0x1C89, 0x1C94,  0x1CA0, 0x1CAB,
0x1CB6, 0x1CC1, 0x1CCC,  0x1CD7, 0x1CE2, 0x1CED,  0x1CF7, 0x1D02, 0x1D0C,  0x1D17, 0x1D21,
0x1D2C, 0x1D36, 0x1D40,  0x1D4A, 0x1D55, 0x1D5F,  0x1D68, 0x1D72, 0x1D7C,  0x1D86, 0x1D8F,
0x1D99, 0x1DA3, 0x1DAC,  0x1DB5, 0x1DBF, 0x1DC8,  0x1DD1, 0x1DDA, 0x1DE3,  0x1DEC, 0x1DF5,
0x1DFE, 0x1E06, 0x1E0F,  0x1E18, 0x1E20, 0x1E29,  0x1E31, 0x1E39, 0x1E41,  0x1E4A, 0x1E52,
0x1E5A, 0x1E62, 0x1E69,  0x1E71, 0x1E79, 0x1E81,  0x1E88, 0x1E90, 0x1E97,  0x1E9E, 0x1EA6,
0x1EAD, 0x1EB4, 0x1EBB,  0x1EC2, 0x1EC9, 0x1ED0,  0x1ED6, 0x1EDD, 0x1EE4,  0x1EEA, 0x1EF1,
0x1EF7, 0x1EFD, 0x1F03,  0x1F0A, 0x1F10, 0x1F16,  0x1F1C, 0x1F21, 0x1F27,  0x1F2D, 0x1F32,
0x1F38, 0x1F3D, 0x1F43,  0x1F48, 0x1F4D, 0x1F53,  0x1F58, 0x1F5D, 0x1F62,  0x1F66, 0x1F6B,
0x1F70, 0x1F75, 0x1F79,  0x1F7E, 0x1F82, 0x1F86,  0x1F8B, 0x1F8F, 0x1F93,  0x1F97, 0x1F9B,
0x1F9F, 0x1FA3, 0x1FA6,  0x1FAA, 0x1FAE, 0x1FB1,  0x1FB4, 0x1FB8, 0x1FBB,  0x1FBE, 0x1FC1,
0x1FC4, 0x1FC7, 0x1FCA,  0x1FCD, 0x1FD0, 0x1FD2,  0x1FD5, 0x1FD8, 0x1FDA,  0x1FDC, 0x1FDF,
0x1FE1, 0x1FE3, 0x1FE5,  0x1FE7, 0x1FE9, 0x1FEB,  0x1FEC, 0x1FEE, 0x1FF0,  0x1FF1, 0x1FF3,
0x1FF4, 0x1FF5, 0x1FF6,  0x1FF7, 0x1FF8, 0x1FF9,  0x1FFA, 0x1FFB, 0x1FFC,  0x1FFD, 0x1FFD,
0x1FFE, 0x1FFE, 0x1FFE,  0x1FFF, 0x1FFF, 0x1FFF,  0x1FFF, 0x1FFF, 0x1FFF,  0x1FFF, 0x1FFE,
0x1FFE, 0x1FFE, 0x1FFD,  0x1FFD, 0x1FFC, 0x1FFB,  0x1FFA, 0x1FF9, 0x1FF8,  0x1FF7, 0x1FF6,
0x1FF5, 0x1FF4, 0x1FF3,  0x1FF1, 0x1FF0, 0x1FEE,  0x1FEC, 0x1FEB, 0x1FE9,  0x1FE7, 0x1FE5,
0x1FE3, 0x1FE1, 0x1FDF,  0x1FDC, 0x1FDA, 0x1FD8,  0x1FD5, 0x1FD2, 0x1FD0,  0x1FCD, 0x1FCA,
0x1FC7, 0x1FC4, 0x1FC1,  0x1FBE, 0x1FBB, 0x1FB8,  0x1FB4, 0x1FB1, 0x1FAE,  0x1FAA, 0x1FA6,
0x1FA3, 0x1F9F, 0x1F9B,  0x1F97, 0x1F93, 0x1F8F,  0x1F8B, 0x1F86, 0x1F82,  0x1F7E, 0x1F79,
0x1F75, 0x1F70, 0x1F6B,  0x1F66, 0x1F62, 0x1F5D,  0x1F58, 0x1F53, 0x1F4D,  0x1F48, 0x1F43,
0x1F3D, 0x1F38, 0x1F32,  0x1F2D, 0x1F27, 0x1F21,  0x1F1C, 0x1F16, 0x1F10,  0x1F0A, 0x1F03,
0x1EFD, 0x1EF7, 0x1EF1,  0x1EEA, 0x1EE4, 0x1EDD,  0x1ED6, 0x1ED0, 0x1EC9,  0x1EC2, 0x1EBB,
0x1EB4, 0x1EAD, 0x1EA6,  0x1E9E, 0x1E97, 0x1E90,  0x1E88, 0x1E81, 0x1E79,  0x1E71, 0x1E69,
0x1E62, 0x1E5A, 0x1E52,  0x1E4A, 0x1E41, 0x1E39,  0x1E31, 0x1E29, 0x1E20,  0x1E18, 0x1E0F,
0x1E06, 0x1DFE, 0x1DF5,  0x1DEC, 0x1DE3, 0x1DDA,  0x1DD1, 0x1DC8, 0x1DBF,  0x1DB5, 0x1DAC,
0x1DA3, 0x1D99, 0x1D8F,  0x1D86, 0x1D7C, 0x1D72,  0x1D68, 0x1D5F, 0x1D55,  0x1D4A, 0x1D40,
0x1D36, 0x1D2C, 0x1D21,  0x1D17, 0x1D0C, 0x1D02,  0x1CF7, 0x1CED, 0x1CE2,  0x1CD7, 0x1CCC,
0x1CC1, 0x1CB6, 0x1CAB,  0x1CA0, 0x1C94, 0x1C89,  0x1C7E, 0x1C72, 0x1C67,  0x1C5B, 0x1C4F,
0x1C44, 0x1C38, 0x1C2C,  0x1C20, 0x1C14, 0x1C08,  0x1BFC, 0x1BF0, 0x1BE3,  0x1BD7, 0x1BCA,
0x1BBE, 0x1BB1, 0x1BA5,  0x1B98, 0x1B8B, 0x1B7F,  0x1B72, 0x1B65, 0x1B58,  0x1B4B, 0x1B3D,
0x1B30, 0x1B23, 0x1B16,  0x1B08, 0x1AFB, 0x1AED,  0x1AE0, 0x1AD2, 0x1AC4,  0x1AB6, 0x1AA8,
0x1A9B, 0x1A8D, 0x1A7F,  0x1A70, 0x1A62, 0x1A54,  0x1A46, 0x1A37, 0x1A29,  0x1A1A, 0x1A0C,
0x19FD, 0x19EE, 0x19E0,  0x19D1, 0x19C2, 0x19B3,  0x19A4, 0x1995, 0x1986,  0x1977, 0x1967,
0x1958, 0x1949, 0x1939,  0x192A, 0x191A, 0x190B,  0x18FB, 0x18EB, 0x18DB,  0x18CC, 0x18BC,
0x18AC, 0x189C, 0x188C,  0x187B, 0x186B, 0x185B,  0x184B, 0x183A, 0x182A,  0x1819, 0x1809,
0x17F8, 0x17E8, 0x17D7,  0x17C6, 0x17B5, 0x17A4,  0x1793, 0x1782, 0x1771,  0x1760, 0x174F,
0x173E, 0x172C, 0x171B,  0x170A, 0x16F8, 0x16E7,  0x16D5, 0x16C3, 0x16B2,  0x16A0, 0x168E,
0x167C, 0x166A, 0x1658,  0x1646, 0x1634, 0x1622,  0x1610, 0x15FE, 0x15EC,  0x15D9, 0x15C7,
0x15B4, 0x15A2, 0x158F,  0x157D, 0x156A, 0x1557,  0x1545, 0x1532, 0x151F,  0x150C, 0x14F9,
0x14E6, 0x14D3, 0x14C0,  0x14AD, 0x149A, 0x1486,  0x1473, 0x1460, 0x144C,  0x1439, 0x1425,
0x1412, 0x13FE, 0x13EB,  0x13D7, 0x13C3, 0x13AF,  0x139C, 0x1388, 0x1374,  0x1360, 0x134C,
0x1338, 0x1324, 0x130F,  0x12FB, 0x12E7, 0x12D3,  0x12BE, 0x12AA, 0x1295,  0x1281, 0x126C,
0x1258, 0x1243, 0x122F,  0x121A, 0x1205, 0x11F0,  0x11DC, 0x11C7, 0x11B2,  0x119D, 0x1188,
0x1173, 0x115E, 0x1149,  0x1133, 0x111E, 0x1109,  0x10F4, 0x10DE, 0x10C9,  0x10B3, 0x109E,
0x1089, 0x1073, 0x105D,  0x1048, 0x1032, 0x101C,  0x1007, 0x0FF1, 0x0FDB,  0x0FC5, 0x0FAF,
0x0F9A, 0x0F84, 0x0F6E,  0x0F58, 0x0F41, 0x0F2B,  0x0F15, 0x0EFF, 0x0EE9,  0x0ED3, 0x0EBC,
0x0EA6, 0x0E90, 0x0E79,  0x0E63, 0x0E4C, 0x0E36,  0x0E1F, 0x0E09, 0x0DF2,  0x0DDB, 0x0DC5,
0x0DAE, 0x0D97, 0x0D81,  0x0D6A, 0x0D53, 0x0D3C,  0x0D25, 0x0D0E, 0x0CF7,  0x0CE0, 0x0CC9,
0x0CB2, 0x0C9B, 0x0C84,  0x0C6D, 0x0C56, 0x0C3F,  0x0C27, 0x0C10, 0x0BF9,  0x0BE1, 0x0BCA,
0x0BB3, 0x0B9B, 0x0B84,  0x0B6C, 0x0B55, 0x0B3D,  0x0B26, 0x0B0E, 0x0AF7,  0x0ADF, 0x0AC7,
0x0AB0, 0x0A98, 0x0A80,  0x0A69, 0x0A51, 0x0A39,  0x0A21, 0x0A09, 0x09F1,  0x09DA, 0x09C2,
0x09AA, 0x0992, 0x097A,  0x0962, 0x094A, 0x0932,  0x091A, 0x0901, 0x08E9,  0x08D1, 0x08B9,
0x08A1, 0x0889, 0x0870,  0x0858, 0x0840, 0x0828,  0x080F, 0x07F7, 0x07DF,  0x07C6, 0x07AE,
0x0795, 0x077D, 0x0765,  0x074C, 0x0734, 0x071B,  0x0703, 0x06EA, 0x06D2,  0x06B9, 0x06A0,
0x0688, 0x066F, 0x0657,  0x063E, 0x0625, 0x060D,  0x05F4, 0x05DB, 0x05C3,  0x05AA, 0x0591,
0x0578, 0x0560, 0x0547,  0x052E, 0x0515, 0x04FC,  0x04E4, 0x04CB, 0x04B2,  0x0499, 0x0480,
0x0467, 0x044E, 0x0435,  0x041D, 0x0404, 0x03EB,  0x03D2, 0x03B9, 0x03A0,  0x0387, 0x036E,
0x0355, 0x033C, 0x0323,  0x030A, 0x02F1, 0x02D8,  0x02BF, 0x02A6, 0x028D,  0x0274, 0x025B,
0x0242, 0x0228, 0x020F,  0x01F6, 0x01DD, 0x01C4,  0x01AB, 0x0192, 0x0179,  0x0160, 0x0147,
0x012D, 0x0114, 0x00FB,  0x00E2, 0x00C9, 0x00B0,  0x0097, 0x007E, 0x0065,  0x004B, 0x0032,
0x0019, 0x0000, 0xFFE7,  0xFFCE, 0xFFB5, 0xFF9B,  0xFF82, 0xFF69, 0xFF50,  0xFF37, 0xFF1E,
0xFF05, 0xFEEC, 0xFED3,  0xFEB9, 0xFEA0, 0xFE87,  0xFE6E, 0xFE55, 0xFE3C,  0xFE23, 0xFE0A,
0xFDF1, 0xFDD8, 0xFDBE,  0xFDA5, 0xFD8C, 0xFD73,  0xFD5A, 0xFD41, 0xFD28,  0xFD0F, 0xFCF6,
0xFCDD, 0xFCC4, 0xFCAB,  0xFC92, 0xFC79, 0xFC60,  0xFC47, 0xFC2E, 0xFC15,  0xFBFC, 0xFBE3,
0xFBCB, 0xFBB2, 0xFB99,  0xFB80, 0xFB67, 0xFB4E,  0xFB35, 0xFB1C, 0xFB04,  0xFAEB, 0xFAD2,
0xFAB9, 0xFAA0, 0xFA88,  0xFA6F, 0xFA56, 0xFA3D,  0xFA25, 0xFA0C, 0xF9F3,  0xF9DB, 0xF9C2,
0xF9A9, 0xF991, 0xF978,  0xF960, 0xF947, 0xF92E,  0xF916, 0xF8FD, 0xF8E5,  0xF8CC, 0xF8B4,
0xF89B, 0xF883, 0xF86B,  0xF852, 0xF83A, 0xF821,  0xF809, 0xF7F1, 0xF7D8,  0xF7C0, 0xF7A8,
0xF790, 0xF777, 0xF75F,  0xF747, 0xF72F, 0xF717,  0xF6FF, 0xF6E6, 0xF6CE,  0xF6B6, 0xF69E,
0xF686, 0xF66E, 0xF656,  0xF63E, 0xF626, 0xF60F,  0xF5F7, 0xF5DF, 0xF5C7,  0xF5AF, 0xF597,
0xF580, 0xF568, 0xF550,  0xF539, 0xF521, 0xF509,  0xF4F2, 0xF4DA, 0xF4C3,  0xF4AB, 0xF494,
0xF47C, 0xF465, 0xF44D,  0xF436, 0xF41F, 0xF407,  0xF3F0, 0xF3D9, 0xF3C1,  0xF3AA, 0xF393,
0xF37C, 0xF365, 0xF34E,  0xF337, 0xF320, 0xF309,  0xF2F2, 0xF2DB, 0xF2C4,  0xF2AD, 0xF296,
0xF27F, 0xF269, 0xF252,  0xF23B, 0xF225, 0xF20E,  0xF1F7, 0xF1E1, 0xF1CA,  0xF1B4, 0xF19D,
0xF187, 0xF170, 0xF15A,  0xF144, 0xF12D, 0xF117,  0xF101, 0xF0EB, 0xF0D5,  0xF0BF, 0xF0A8,
0xF092, 0xF07C, 0xF066,  0xF051, 0xF03B, 0xF025,  0xF00F, 0xEFF9, 0xEFE4,  0xEFCE, 0xEFB8,
0xEFA3, 0xEF8D, 0xEF77,  0xEF62, 0xEF4D, 0xEF37,  0xEF22, 0xEF0C, 0xEEF7,  0xEEE2, 0xEECD,
0xEEB7, 0xEEA2, 0xEE8D,  0xEE78, 0xEE63, 0xEE4E,  0xEE39, 0xEE24, 0xEE10,  0xEDFB, 0xEDE6,
0xEDD1, 0xEDBD, 0xEDA8,  0xED94, 0xED7F, 0xED6B,  0xED56, 0xED42, 0xED2D,  0xED19, 0xED05,
0xECF1, 0xECDC, 0xECC8,  0xECB4, 0xECA0, 0xEC8C,  0xEC78, 0xEC64, 0xEC51,  0xEC3D, 0xEC29,
0xEC15, 0xEC02, 0xEBEE,  0xEBDB, 0xEBC7, 0xEBB4,  0xEBA0, 0xEB8D, 0xEB7A,  0xEB66, 0xEB53,
0xEB40, 0xEB2D, 0xEB1A,  0xEB07, 0xEAF4, 0xEAE1,  0xEACE, 0xEABB, 0xEAA9,  0xEA96, 0xEA83,
0xEA71, 0xEA5E, 0xEA4C,  0xEA39, 0xEA27, 0xEA14,  0xEA02, 0xE9F0, 0xE9DE,  0xE9CC, 0xE9BA,
0xE9A8, 0xE996, 0xE984,  0xE972, 0xE960, 0xE94E,  0xE93D, 0xE92B, 0xE919,  0xE908, 0xE8F6,
0xE8E5, 0xE8D4, 0xE8C2,  0xE8B1, 0xE8A0, 0xE88F,  0xE87E, 0xE86D, 0xE85C,  0xE84B, 0xE83A,
0xE829, 0xE818, 0xE808,  0xE7F7, 0xE7E7, 0xE7D6,  0xE7C6, 0xE7B5, 0xE7A5,  0xE795, 0xE785,
0xE774, 0xE764, 0xE754,  0xE744, 0xE734, 0xE725,  0xE715, 0xE705, 0xE6F5,  0xE6E6, 0xE6D6,
0xE6C7, 0xE6B7, 0xE6A8,  0xE699, 0xE689, 0xE67A,  0xE66B, 0xE65C, 0xE64D,  0xE63E, 0xE62F,
0xE620, 0xE612, 0xE603,  0xE5F4, 0xE5E6, 0xE5D7,  0xE5C9, 0xE5BA, 0xE5AC,  0xE59E, 0xE590,
0xE581, 0xE573, 0xE565,  0xE558, 0xE54A, 0xE53C,  0xE52E, 0xE520, 0xE513,  0xE505, 0xE4F8,
0xE4EA, 0xE4DD, 0xE4D0,  0xE4C3, 0xE4B5, 0xE4A8,  0xE49B, 0xE48E, 0xE481,  0xE475, 0xE468,
0xE45B, 0xE44F, 0xE442,  0xE436, 0xE429, 0xE41D,  0xE410, 0xE404, 0xE3F8,  0xE3EC, 0xE3E0,
0xE3D4, 0xE3C8, 0xE3BC,  0xE3B1, 0xE3A5, 0xE399,  0xE38E, 0xE382, 0xE377,  0xE36C, 0xE360,
0xE355, 0xE34A, 0xE33F,  0xE334, 0xE329, 0xE31E,  0xE313, 0xE309, 0xE2FE,  0xE2F4, 0xE2E9,
0xE2DF, 0xE2D4, 0xE2CA,  0xE2C0, 0xE2B6, 0xE2AB,  0xE2A1, 0xE298, 0xE28E,  0xE284, 0xE27A,
0xE271, 0xE267, 0xE25D,  0xE254, 0xE24B, 0xE241,  0xE238, 0xE22F, 0xE226,  0xE21D, 0xE214,
0xE20B, 0xE202, 0xE1FA,  0xE1F1, 0xE1E8, 0xE1E0,  0xE1D7, 0xE1CF, 0xE1C7,  0xE1BF, 0xE1B6,
0xE1AE, 0xE1A6, 0xE19E,  0xE197, 0xE18F, 0xE187,  0xE17F, 0xE178, 0xE170,  0xE169, 0xE162,
0xE15A, 0xE153, 0xE14C,  0xE145, 0xE13E, 0xE137,  0xE130, 0xE12A, 0xE123,  0xE11C, 0xE116,
0xE10F, 0xE109, 0xE103,  0xE0FD, 0xE0F6, 0xE0F0,  0xE0EA, 0xE0E4, 0xE0DF,  0xE0D9, 0xE0D3,
0xE0CE, 0xE0C8, 0xE0C3,  0xE0BD, 0xE0B8, 0xE0B3,  0xE0AD, 0xE0A8, 0xE0A3,  0xE09E, 0xE09A,
0xE095, 0xE090, 0xE08B,  0xE087, 0xE082, 0xE07E,  0xE07A, 0xE075, 0xE071,  0xE06D, 0xE069,
0xE065, 0xE061, 0xE05D,  0xE05A, 0xE056, 0xE052,  0xE04F, 0xE04C, 0xE048,  0xE045, 0xE042,
0xE03F, 0xE03C, 0xE039,  0xE036, 0xE033, 0xE030,  0xE02E, 0xE02B, 0xE028,  0xE026, 0xE024,
0xE021, 0xE01F, 0xE01D,  0xE01B, 0xE019, 0xE017,  0xE015, 0xE014, 0xE012,  0xE010, 0xE00F,
0xE00D, 0xE00C, 0xE00B,  0xE00A, 0xE009, 0xE008,  0xE007, 0xE006, 0xE005,  0xE004, 0xE003,
0xE003, 0xE002, 0xE002,  0xE002, 0xE001, 0xE001,  0xE001, 0xE001, 0xE001,  0xE001, 0xE001,
0xE002, 0xE002, 0xE002,  0xE003, 0xE003, 0xE004,  0xE005, 0xE006, 0xE007,  0xE008, 0xE009,
0xE00A, 0xE00B, 0xE00C,  0xE00D, 0xE00F, 0xE010,  0xE012, 0xE014, 0xE015,  0xE017, 0xE019,
0xE01B, 0xE01D, 0xE01F,  0xE021, 0xE024, 0xE026,  0xE028, 0xE02B, 0xE02E,  0xE030, 0xE033,
0xE036, 0xE039, 0xE03C,  0xE03F, 0xE042, 0xE045,  0xE048, 0xE04C, 0xE04F,  0xE052, 0xE056,
0xE05A, 0xE05D, 0xE061,  0xE065, 0xE069, 0xE06D,  0xE071, 0xE075, 0xE07A,  0xE07E, 0xE082,
0xE087, 0xE08B, 0xE090,  0xE095, 0xE09A, 0xE09E,  0xE0A3, 0xE0A8, 0xE0AD,  0xE0B3, 0xE0B8,
0xE0BD, 0xE0C3, 0xE0C8,  0xE0CE, 0xE0D3, 0xE0D9,  0xE0DF, 0xE0E4, 0xE0EA,  0xE0F0, 0xE0F6,
0xE0FD, 0xE103, 0xE109,  0xE10F, 0xE116, 0xE11C,  0xE123, 0xE12A, 0xE130,  0xE137, 0xE13E,
0xE145, 0xE14C, 0xE153,  0xE15A, 0xE162, 0xE169,  0xE170, 0xE178, 0xE17F,  0xE187, 0xE18F,
0xE197, 0xE19E, 0xE1A6,  0xE1AE, 0xE1B6, 0xE1BF,  0xE1C7, 0xE1CF, 0xE1D7,  0xE1E0, 0xE1E8,
0xE1F1, 0xE1FA, 0xE202,  0xE20B, 0xE214, 0xE21D,  0xE226, 0xE22F, 0xE238,  0xE241, 0xE24B,
0xE254, 0xE25D, 0xE267,  0xE271, 0xE27A, 0xE284,  0xE28E, 0xE298, 0xE2A1,  0xE2AB, 0xE2B6,
0xE2C0, 0xE2CA, 0xE2D4,  0xE2DF, 0xE2E9, 0xE2F4,  0xE2FE, 0xE309, 0xE313,  0xE31E, 0xE329,
0xE334, 0xE33F, 0xE34A,  0xE355, 0xE360, 0xE36C,  0xE377, 0xE382, 0xE38E,  0xE399, 0xE3A5,
0xE3B1, 0xE3BC, 0xE3C8,  0xE3D4, 0xE3E0, 0xE3EC,  0xE3F8, 0xE404, 0xE410,  0xE41D, 0xE429,
0xE436, 0xE442, 0xE44F,  0xE45B, 0xE468, 0xE475,  0xE481, 0xE48E, 0xE49B,  0xE4A8, 0xE4B5,
0xE4C3, 0xE4D0, 0xE4DD,  0xE4EA, 0xE4F8, 0xE505,  0xE513, 0xE520, 0xE52E,  0xE53C, 0xE54A,
0xE558, 0xE565, 0xE573,  0xE581, 0xE590, 0xE59E,  0xE5AC, 0xE5BA, 0xE5C9,  0xE5D7, 0xE5E6,
0xE5F4, 0xE603, 0xE612,  0xE620, 0xE62F, 0xE63E,  0xE64D, 0xE65C, 0xE66B,  0xE67A, 0xE689,
0xE699, 0xE6A8, 0xE6B7,  0xE6C7, 0xE6D6, 0xE6E6,  0xE6F5, 0xE705, 0xE715,  0xE725, 0xE734,
0xE744, 0xE754, 0xE764,  0xE774, 0xE785, 0xE795,  0xE7A5, 0xE7B5, 0xE7C6,  0xE7D6, 0xE7E7,
0xE7F7, 0xE808, 0xE818,  0xE829, 0xE83A, 0xE84B,  0xE85C, 0xE86D, 0xE87E,  0xE88F, 0xE8A0,
0xE8B1, 0xE8C2, 0xE8D4,  0xE8E5, 0xE8F6, 0xE908,  0xE919, 0xE92B, 0xE93D,  0xE94E, 0xE960,
0xE972, 0xE984, 0xE996,  0xE9A8, 0xE9BA, 0xE9CC,  0xE9DE, 0xE9F0, 0xEA02,  0xEA14, 0xEA27,
0xEA39, 0xEA4C, 0xEA5E,  0xEA71, 0xEA83, 0xEA96,  0xEAA9, 0xEABB, 0xEACE,  0xEAE1, 0xEAF4,
0xEB07, 0xEB1A, 0xEB2D,  0xEB40, 0xEB53, 0xEB66,  0xEB7A, 0xEB8D, 0xEBA0,  0xEBB4, 0xEBC7,
0xEBDB, 0xEBEE, 0xEC02,  0xEC15, 0xEC29, 0xEC3D,  0xEC51, 0xEC64, 0xEC78,  0xEC8C, 0xECA0,
0xECB4, 0xECC8, 0xECDC,  0xECF1, 0xED05, 0xED19,  0xED2D, 0xED42, 0xED56,  0xED6B, 0xED7F,
0xED94, 0xEDA8, 0xEDBD,  0xEDD1, 0xEDE6, 0xEDFB,  0xEE10, 0xEE24, 0xEE39,  0xEE4E, 0xEE63,
0xEE78, 0xEE8D, 0xEEA2,  0xEEB7, 0xEECD, 0xEEE2,  0xEEF7, 0xEF0C, 0xEF22,  0xEF37, 0xEF4D,
0xEF62, 0xEF77, 0xEF8D,  0xEFA3, 0xEFB8, 0xEFCE,  0xEFE4, 0xEFF9, 0xF00F,  0xF025, 0xF03B,
0xF051, 0xF066, 0xF07C,  0xF092, 0xF0A8, 0xF0BF,  0xF0D5, 0xF0EB, 0xF101,  0xF117, 0xF12D,
0xF144, 0xF15A, 0xF170,  0xF187, 0xF19D, 0xF1B4,  0xF1CA, 0xF1E1, 0xF1F7,  0xF20E, 0xF225,
0xF23B, 0xF252, 0xF269,  0xF27F, 0xF296, 0xF2AD,  0xF2C4, 0xF2DB, 0xF2F2,  0xF309, 0xF320,
0xF337, 0xF34E, 0xF365,  0xF37C, 0xF393, 0xF3AA,  0xF3C1, 0xF3D9, 0xF3F0,  0xF407, 0xF41F,
0xF436, 0xF44D, 0xF465,  0xF47C, 0xF494, 0xF4AB,  0xF4C3, 0xF4DA, 0xF4F2,  0xF509, 0xF521,
0xF539, 0xF550, 0xF568,  0xF580, 0xF597, 0xF5AF,  0xF5C7, 0xF5DF, 0xF5F7,  0xF60F, 0xF626,
0xF63E, 0xF656, 0xF66E,  0xF686, 0xF69E, 0xF6B6,  0xF6CE, 0xF6E6, 0xF6FF,  0xF717, 0xF72F,
0xF747, 0xF75F, 0xF777,  0xF790, 0xF7A8, 0xF7C0,  0xF7D8, 0xF7F1, 0xF809,  0xF821, 0xF83A,
0xF852, 0xF86B, 0xF883,  0xF89B, 0xF8B4, 0xF8CC,  0xF8E5, 0xF8FD, 0xF916,  0xF92E, 0xF947,
0xF960, 0xF978, 0xF991,  0xF9A9, 0xF9C2, 0xF9DB,  0xF9F3, 0xFA0C, 0xFA25,  0xFA3D, 0xFA56,
0xFA6F, 0xFA88, 0xFAA0,  0xFAB9, 0xFAD2, 0xFAEB,  0xFB04, 0xFB1C, 0xFB35,  0xFB4E, 0xFB67,
0xFB80, 0xFB99, 0xFBB2,  0xFBCB, 0xFBE3, 0xFBFC,  0xFC15, 0xFC2E, 0xFC47,  0xFC60, 0xFC79,
0xFC92, 0xFCAB, 0xFCC4,  0xFCDD, 0xFCF6, 0xFD0F,  0xFD28, 0xFD41, 0xFD5A,  0xFD73, 0xFD8C,
0xFDA5, 0xFDBE, 0xFDD8,  0xFDF1, 0xFE0A, 0xFE23,  0xFE3C, 0xFE55, 0xFE6E,  0xFE87, 0xFEA0,
0xFEB9, 0xFED3, 0xFEEC,  0xFF05, 0xFF1E, 0xFF37,  0xFF50, 0xFF69, 0xFF82,  0xFF9B, 0xFFB5,
0xFFCE, 0xFFE7 };

//0-静音
//1-提示音
//2-语音播报
DH_U8 gVoiceSetting[33+1]={2};
DH_U8 g_ToneVoiceVolume;

static DH_U16  soundDac[32];

extern DH_U16 g_u16TimCnt_us;
extern DH_U8 g_Coulo;         /* 电量大小控制 */
extern DH_U8 g_PowerForbidden ;
/* ========================================================================== */
/*                          函数声明区                                        */
/* ========================================================================== */


void SOUND_BLL_VoiceInit(void)
{
#if 0
	uint8 buf[27];
	FLASH_INTF_ReadStr( BA_TONE,buf,27);

	g_ToneVoiceVolume = buf[22]&KEYVOICE_BITS;
	if (g_ToneVoiceVolume > KEYVOICEVOLUME_MAX)
	{
		g_ToneVoiceVolume = KEYVOICEVOLUME_MAX;
	}
	else if (g_ToneVoiceVolume < KEYVOICEVOLUME_MIN)
	{
		g_ToneVoiceVolume = KEYVOICEVOLUME_MIN;
	}
//printk("Tone%d\r\n",g_ToneVoiceVolume);
	gVoiceSetting[SOUNDSTAT_InterohoneOpenVoice ] = buf[0];/*开机*/
	gVoiceSetting[SOUNDSTAT_InterohoneCloseVoice] = buf[1];/*关机*/
	gVoiceSetting[SOUNDSTAT_ChnNumberVoice      ] = buf[2];/*信道切换*/
	gVoiceSetting[SOUNDSTAT_CouloStatVoice      ] = buf[3];/*电量*/
	gVoiceSetting[SOUNDSTAT_LowCouloVoice       ] = buf[4];/*低电*/

	gVoiceSetting[SOUNDSTAT_PowerVoice          ] = buf[5];/*功率*/
	gVoiceSetting[SOUNDSTAT_SQGradeVoice        ] = buf[6];/*SQ*/
	gVoiceSetting[SOUNDSTAT_ScanStartVoice      ] = buf[7];/*扫描开始*/
	gVoiceSetting[SOUNDSTAT_ScanEndVoice        ] = buf[7];/*扫描结束*/
	gVoiceSetting[SOUNDSTAT_LookInfmVoice       ] = buf[8];/*信息查询*/

	gVoiceSetting[SOUNDSTAT_LockVoice           ] = buf[9];/*上锁*/
	gVoiceSetting[SOUNDSTAT_UnLockVoice         ] = buf[10];/*解锁*/
	gVoiceSetting[SOUNDSTAT_RedAlarmVoice       ] = buf[11];/*报警开*/
	gVoiceSetting[SOUNDSTAT_RedAlarmStpVoice    ] = buf[11];/*报警关*/

	gVoiceSetting[SOUNDSTAT_Handup    ]           = buf[9];/*挂起*/

	gVoiceSetting[SOUNDSTAT_GeHuVoice           ] = buf[12];/*个呼开*/
	gVoiceSetting[SOUNDSTAT_GeHuEndVoice        ] = buf[12];/*个呼结束*/
	gVoiceSetting[SOUNDSTAT_ZuHuVoice           ] = buf[13];/*组呼开*/
	gVoiceSetting[SOUNDSTAT_ZuHuEndVoice        ] = buf[13];/*组呼结束*/

	gVoiceSetting[SOUNDSTAT_QuanHuVoice         ] = buf[14];/*全呼*/

	gVoiceSetting[SOUNDSTAT_TxTimeOutVoice      ] = buf[15];/*发射TOT超时*/
	gVoiceSetting[SOUNDSTAT_BsTxTimeOutVoice    ] = buf[16];/*中继超时*/
	gVoiceSetting[SOUNDSTAT_EncrptOpenVoice     ] = buf[17];/*加密开*/
	gVoiceSetting[SOUNDSTAT_EncrptCloseVoice    ] = buf[18];/*加密关*/
	gVoiceSetting[SOUNDSTAT_SKOpenVoice         ] = buf[19];/*声控开*/
	gVoiceSetting[SOUNDSTAT_SKCloseVoice        ] = buf[20];/*声控关*/

	gVoiceSetting[SOUNDSTAT_ZoneVoice           ] = buf[21];/*区域切换*/
	gVoiceSetting[SOUNDSTAT_ScanCallInStart     ] = buf[23];/*扫描接收提示*/

	gVoiceSetting[SOUNDSTAT_Monitor             ] = buf[24];/**/
	gVoiceSetting[SOUNDSTAT_Monitor_A1          ] = buf[24];/**/
	gVoiceSetting[SOUNDSTAT_Monitor_A2          ] = buf[24];/**/


	gVoiceSetting[SOUNDSTAT_VolumeGradeVoice    ]       = 2;/*音量*/
	gVoiceSetting[SOUNDSTAT_VolumeGradeVoice2   ]       = 2;/*音量*/

	gVoiceSetting[SOUNDSTAT_ScanMode            ]       = 2;/*扫描模式*/

	gVoiceSetting[SOUNDSTAT_Revive              ]       = 2;/**/
	gVoiceSetting[SOUNDSTAT_Kill                ]       = 2;/**/
	gVoiceSetting[SOUNDSTAT_Detection           ]       = 2;/**/
	gVoiceSetting[SOUNDSTAT_Prompt              ]       = 2;/**/


	gVoiceSetting[SOUNDSTAT_SetOn               ]       = 2;/**/
	gVoiceSetting[SOUNDSTAT_SetOff              ]       = 2;/**/

	gVoiceSetting[SOUNDSTAT_RoamConnected       ]       = 2;/**/

	gVoiceSetting[SOUNDSTAT_BSChnNumberVoice    ]       = 2;/**/
	gVoiceSetting[SOUNDSTAT_UnProgrammedChnNumberVoice] = 2;/**/
	gVoiceSetting[SOUNDSTAT_UnProgrammedChnZoneVoice]   = 2;/**/
	gVoiceSetting[SOUNDSTAT_PCtoMCUStartVoice   ]       = 2;/**/
	gVoiceSetting[SOUNDSTAT_PCtoMCUEndVoice     ]       = 2;/**/
	gVoiceSetting[SOUNDSTAT_PwrOff              ]       = 2;/**/
#endif
}



/*******************************************************************************
 * 函数名  : SOUND_BLL_SoundInit
 * 描  述  : 声码器初始化 
 * 输  入  :  参数        描述
 *         :   DH_VOID         请输入DH_VOID的描述
 * 输  出  : 无
 * 返回值  : DH_SUCCESS: 成功
 *           DH_FAILURE: 失败
 *******************************************************************************/
DH_VOID SOUND_BLL_SoundInit(DH_VOID)
{
	//OS_ERR err;
	
	//if (dhsem_create(&CodecLock,1) != DH_OK()) {	}
//	if (dhsem_create(&Sem_SoundFlg,1) != DH_OK()) {	}

//	CKSetIsrVector(interrupt_sound_isr, 27);

    /*  */
//    OSMutexCreate(&Sound_Mutex, "sndmtx",  &err) ;

	SOUND_BLL_VoiceInit();

//	memset(TaskSoundStk, 0x00, sizeof(CPU_STK) * TASKSOUND_STK_SIZE);
//	
//	OSTaskCreate(&TaskSoundTCB,\
//		  "sound",\
//		  (OS_TASK_PTR)SOUND_BLL_SoundTask,\
//		  0,\
//		  TASKSOUND_PRIO,\
//		  &TaskSoundStk[0],\
//		  0,\
//		  TASKSOUND_STK_SIZE,\
//		  0,0,0,\
//		  OS_OPT_TASK_NONE | OS_OPT_TASK_STK_CHK,\
//		  &err);
}

///***************************************************************
//* 函数名: interrupt_sound_isr
//* 描  述: 侧音
//* 输  入: 操作系统定义的参数
//* 输  出: 无
//* 返回值: 提供给内核的状态
//***************************************************************/
//DH_VOID STATIC_CODE_SECTION interrupt_sound_isr(DH_VOID)
//{
//	OS_ERR err;
//	OSSemPost(&Sem_SoundFlg, OS_OPT_POST_1, &err);
//}




/*******************************************************************************
 * 函数名  : SOUND_BLL_SoundTask
 * 描  述  : 声音任务 
 * 输  入  :  参数        描述
 *         :   initial_data         请输入initial_data的描述
 * 输  出  : 无
 * 返回值  : DH_SUCCESS: 成功
 *           DH_FAILURE: 失败
 *******************************************************************************/
DH_VOID SOUND_BLL_SoundTask(void *p_arg)
{
	DH_U32 lu32Signal;
//	OS_ERR err;
	while(TaskHndGet(TASK_FLAG_VOICE))
	{
		TaskHndStop(TASK_FLAG_VOICE);
       // lu32Signal = dhevent_wait(&lwevent_int_grp, SOUND_PLAY_EV, FALSE, 0, &err);

      ///  if(lu32Signal & SOUND_PLAY_EV)/*提示音*/
        {
      //      dhevent_clear(&lwevent_int_grp, SOUND_PLAY_EV);

			MenuVarFlg.bSoundChangedFlg = FALSE;

		//	if(CSBK_APP_PadlockStatGet())
			{
				SOUND_BLL_SoundExe(SoundStat,SoundStatExu);
			}
        }
	}  
}

/*******************************************************************************
 * 函数名  : SOUND_BLL_SoundStart
 * 描  述  : 
 * 输  入  :  参数        描述
 *         :   Stat         请输入Stat的描述
 * 输  出  : 无
 * 返回值  : DH_SUCCESS: 成功
 *           DH_FAILURE: 失败
 *******************************************************************************/
DH_VOID SOUND_BLL_SoundStart(DH_U8 Stat)
{
//	DH_U8 ProfStat = MENU_APP_GetMenuSetStat(BARTYPESETFLG_Profile);
//	OS_ERR err;
//	if(Stat == 0) {
//		SoundStatExu = 0;
//	}
//	else if (ProfStat == GENERALFLAG || ProfStat == VOIEC_VIBRATINGFLAG)
	{
		SoundStat = Stat;
		SoundStatExu = 0;
//		MenuVarFlg.bSoundChangedFlg = TRUE;
		//OSTimeDlyResume(&TaskSoundTCB,&err);
		SOUND_BLL_SoundExe(SoundStat,SoundStatExu);

//		dhevent_set(&lwevent_int_grp, SOUND_PLAY_EV);
	}
}


/*******************************************************************************
 * 函数名  : SOUND_BLL_SoundLvlSet
 * 描  述  :
 * 输  入  :  参数        描述
 *         :   Stat         请输入Stat的描述
 *         :   Exu          请输入Exu的描述
 * 输  出  : 无
 * 返回值  : DH_SUCCESS: 成功
 *           DH_FAILURE: 失败
 *******************************************************************************/
DH_VOID SOUND_BLL_SoundLvlSet(DH_U8 volume)
{
	g_SoundLevel = volume * 5;
}

/*******************************************************************************
 * 函数名  : SOUND_BLL_SoundStartOpt
 * 描  述  : 
 * 输  入  :  参数        描述
 *         :   Stat         请输入Stat的描述
 *         :   Exu          请输入Exu的描述
 * 输  出  : 无
 * 返回值  : DH_SUCCESS: 成功
 *           DH_FAILURE: 失败
 *******************************************************************************/
DH_VOID SOUND_BLL_SoundStartOpt(DH_U8 Stat, DH_U8 Exu)
{
	MenuVarFlg.bSoundChangedFlg = TRUE;
#if VOICE_IC_STAT

	if(Stat == SOUNDSTAT_Voice)
	{
		MenuVarFlg.KeySoundIgnoreFlg = 1;
	}
#else/*if(SOUNDSTAT_KeySound != Stat)*/
	if(Stat == SOUNDSTAT_Voice)
	{
		return;
	}
#endif
//	if (MENU_APP_GetMenuSetStat(BARTYPESETFLG_Profile) == GENERALFLAG
//	 || MENU_APP_GetMenuSetStat(BARTYPESETFLG_Profile) == VOIEC_VIBRATINGFLAG)
	{
		SoundStatExu = Exu;
		SoundStat = Stat;
		SOUND_BLL_SoundExe(SoundStat,SoundStatExu);
		//dhevent_set(&lwevent_int_grp, SOUND_PLAY_EV);
	}
}

/*******************************************************************************
 * 函数名  : SOUND_BLL_SoundExe
 * 描  述  : 声音函数 
 * 输  入  :  参数        描述
 *         :   Stat         请输入Stat的描述
 * 输  出  : 无
 * 返回值  : DH_SUCCESS: 成功
 *           DH_FAILURE: 失败
 *******************************************************************************/
DH_VOID SOUND_BLL_SoundExe(DH_U8 Stat,DH_U8 StatExu)
{
//	if(GloVarFlag.RadioFlag ==1 || !CSBK_APP_PadlockStatGet())
//	{
//		return;
//	}
//
//#if !VOICE_IC_STAT//不支持语音播报则退出
//	if(Stat == SOUNDSTAT_Voice)
//	{
//		return;
//	}
//#endif
//
//	if(Stat ==0)
//	{
//		return;
//	}
//
//	if (MENU_APP_GetMenuSetStat(BARTYPESETFLG_Profile) == SILENTFLAG
//	 || MENU_APP_GetMenuSetStat(BARTYPESETFLG_Profile) == VOIEC_VIBRATINGFLAG)
//	{
//		return;
//	}
//
//#if FUNC_MENU_DISP  == MENU_DISP_LCD
//    g_SoundLevel = VOL_BLL_GetVolvalue();
//#else
//    g_SoundLevel = g_ToneVoiceVolume *5;
//#endif

    if(g_SoundLevel == 0 && StatExu ==0 && Stat != SOUNDSTAT_Voice)
    {
    	return;
    }

    CODECLOCK();

    MenuVarFlg.bSoundChangedFlg = FALSE;

    MenuVarFlg.VoiceFlg=1;

    if(CHN_BLL_GetAttr()==CHNSTAT_FM && (Stat != SOUNDSTAT_Voice
		||(gVoiceSetting[StatExu ]==1 && Stat == SOUNDSTAT_Voice)))
    {

    }

    VoxCallOutTimeOut = VOX_I_MAX;

	if(Stat == SOUNDSTAT_Voice)
	{
	#if VOICE_IC_STAT
	
		DRV_AudioVoiceSelct(SoundStatExu);
//		if(gVoiceSetting[StatExu ]==0)//
//		{
//		}
//		else if(gVoiceSetting[StatExu ]==1)
//		{
//		
//		}
//		else
//		{
//			if (!(CodecStat&DECODEOPEN))
//			{
//				SPK_INTF_Open();
//			}
//
//
//			DH_U8 lvollvl = g_ToneVoiceVolume;
//
//			VOICEIC_INTF_SetSoundLvl((lvollvl?(lvollvl-1):0)/4);
//			printk("SOUND_BLL_VoiceIC.");
//			SOUND_BLL_VoiceIC(StatExu);
//		}
	#endif
	}
//	else
//	{
//		if (!(CodecStat&DECODEOPEN) && Stat!=SOUNDSTAT_PowerOn)
//		{
//			SPK_INTF_Open();
//		}
//		*AUDIO_CONTROL |= BITSET(7);
//		DH_U8 gcr = *CODEC_GCR_DACL;
//
//		CODEC_BLL_SetLineOutVol(g_SoundLevel>40?40:g_SoundLevel);
//
//		switch (Stat)
//		{
//			case SOUNDSTAT_1:
//				SOUND_bll_freq_thr(800,100,0,0,0,0,0,0);SOUND_bll_freq_end();
//				break;
//			case SOUNDSTAT_2:
//				SOUND_bll_freq_thr(2000,256,0,0,0,0,0,0);SOUND_bll_freq_end();
//				break;
//			case SOUNDSTAT_3:
//				SOUND_bll_freq_thr(800,160,0,0,0,0,0,0);SOUND_bll_freq_end();
//				break;
//			case SOUNDSTAT_4:
//				SOUND_bll_freq_thr(800,190,0,0,0,0,0,0);SOUND_bll_freq_end();
//				break;
//			case SOUNDSTAT_5:
//				SOUND_bll_freq_thr(800,512,0,0,1,0,0,0);SOUND_bll_freq_end();
//				break;
//			case SOUNDSTAT_6:
//				SOUND_bll_freq_thr(480,384,0,0,2,0,0,0);
//				SOUND_bll_freq_thr(480,256,512,768,0,0,0,0);
//				SOUND_bll_freq_thr(480,384,0,0,2,0,0,0);
//				SOUND_bll_freq_thr(480,256,512,768,0,0,0,0);
//				SOUND_bll_freq_thr(480,384,0,0,2,0,0,0);
//				SOUND_bll_freq_thr(480,256,512,768,0,0,0,0);
//				SOUND_bll_freq_thr(480,384,0,0,2,0,0,0);
//				SOUND_bll_freq_thr(480,256,512,768,0,0,0,0);
//				SOUND_bll_freq_end();
//				break;
//			case SOUNDSTAT_7:
//				SOUND_bll_freq_thr(480,133,0,0,0,0,0,0);			/* 音调2*/
//				SOUND_bll_freq_thr(480,64,0,0,2,0,0,0);
//				SOUND_bll_freq_thr(480,133,0,0,0,0,0,0);
//				SOUND_bll_freq_thr(480,64,0,0,2,0,0,0);
//				SOUND_bll_freq_thr(480,133,0,0,0,0,0,0);
//				SOUND_bll_freq_thr(480,64,0,0,2,0,0,0);
//				SOUND_bll_freq_thr(480,133,0,0,0,0,0,0);
//				SOUND_bll_freq_thr(480,64,0,0,2,0,0,0);
//				SOUND_bll_freq_end();
//				break;
//			case SOUNDSTAT_8:
//				SOUND_bll_freq_thrext(480,136,272,408,544,1,2,2,1,1);		/*音调3*/
//				SOUND_bll_freq_thr(480,256,0,0,1,0,0,0);
//				SOUND_bll_freq_thrext(480,136,272,408,544,1,2,2,1,1);
//				SOUND_bll_freq_thr(480,256,0,0,1,0,0,0);
//				SOUND_bll_freq_thrext(480,136,272,408,544,1,2,2,1,1);
//				SOUND_bll_freq_thr(480,256,0,0,1,0,0,0);
//				SOUND_bll_freq_thrext(480,136,272,408,544,1,2,2,1,1);
//				SOUND_bll_freq_thr(480,256,0,0,1,0,0,0);
//				SOUND_bll_freq_end();
//				break;
//			case SOUNDSTAT_9:
//				SOUND_bll_freq_thr(480,246,492,0,3,3,0,0);			/* 音调4*/
//				SOUND_bll_freq_thr(480,246,492,0,3,3,0,0);
//				SOUND_bll_freq_thr(480,246,492,0,3,3,0,0);
//				SOUND_bll_freq_thr(480,246,492,0,3,3,0,0);
//				SOUND_bll_freq_thr(480,246,492,0,3,3,0,0);
//				SOUND_bll_freq_thr(480,246,492,0,3,3,0,0);
//				SOUND_bll_freq_thr(480,246,492,0,3,3,0,0);
//				SOUND_bll_freq_thr(480,246,492,0,3,3,0,0);
//				SOUND_bll_freq_end();
//				break;
//			case SOUNDSTAT_10:
//				SOUND_bll_freq_thr(480,460,0,0,2,0,0,0);			/* 音调9*/
//				SOUND_bll_freq_thr(480,256,512,0,2,1,0,0);
//				SOUND_bll_freq_thr(480,460,0,0,2,0,0,0);
//				SOUND_bll_freq_thr(480,256,512,0,2,1,0,0);
//				SOUND_bll_freq_thr(480,460,0,0,2,0,0,0);
//				SOUND_bll_freq_thr(480,256,512,0,2,1,0,0);
//				SOUND_bll_freq_thr(480,460,0,0,2,0,0,0);
//				SOUND_bll_freq_thr(480,256,512,0,2,1,0,0);
//				SOUND_bll_freq_end();
//				break;
//
//			case SOUNDSTAT_PCM:
////				SOUND_bll_freq_I2S_PlayPCM(_acopen_music,30*1024);
//				break;
//
//			case SOUNDSTAT_PowerOn:
//				{
//					CPU_SR_ALLOC();
//					CPU_CRITICAL_ENTER();
//					SoundStatExu = VOL_BLL_GetVolLevel();//音量
//					SoundStatExu = SoundStatExu<1?1:SoundStatExu;
//					CODEC_BLL_SetLineOutVol(SoundStatExu*3);
//				//	HRC_BLL_PowerOnSound1();
//					SOUND_bll_freq_I2S(20,64,0,0,0,2,0,0,0);
//					SPK_INTF_Open();
//					SOUND_bll_freq_I2S(20,64,0,0,0,2,0,0,0);
//					SOUND_bll_freq_I2S(20,100,200,0,0,2,2,0,0);
//					SOUND_bll_freq_I2S(20,84,169,253,0,2,2,1,0);
//					SOUND_bll_freq_I2S(21,133,266,399,532,2,2,1,0);
//					SOUND_bll_freq_end();
//					CPU_CRITICAL_EXIT();
//
///////////					SOUND_bll_freq_I2S_Play(0,726*2048);///
//				}
//				break;
//			case SOUNDSTAT_KeySound:
//				{
//					CODEC_BLL_SetLineOutVol(StatExu*3);
//					SOUND_bll_freq_thrext(400,380,0,0,0,0,0,0,0,20);
//					SOUND_bll_freq_end();
//				}
//				break;
//			case SOUNDSTAT_Edited:
//				{
//					CODEC_BLL_SetLineOutVol(StatExu*3);
//					SOUND_bll_freq_thr(800,160,0,0,0,0,0,0);
//					SOUND_bll_freq_end();
//				}
//				break;
//			case SOUNDSTAT_COLOU:
//				{
//					CODEC_BLL_SetLineOutVol(StatExu*3);
//					SOUND_bll_freq_thrext(7200,140,0,0,0,0,0,0,0,20);
//					SOUND_bll_freq_end();
//				}
//				break;
//			case SOUNDSTAT_Alarm:
//				SOUND_bll_freq_thr(7200,140,0,0,0,0,0,0);
//				SOUND_bll_freq_end();
//				break;
//			case SOUNDSTAT_SendRefuseVoice:/*低电量提示音*/
//				VoxCallOutTimeOut=VOX_CallOutTimeOut_MAX;
//				SOUND_bll_freq_thr(7200,140,0,0,0,0,0,0);
//				SOUND_bll_freq_end();
//				break;
//
//			default:
//				SOUND_BLL_Tone(Stat);
//				break;
//		}
//
//		*AUDIO_CONTROL &= BITCLR(7);
//		*CODEC_GCR_DACL = gcr;
//	}
//
//   if ((!(Str_PlayRecd.PlayFlg)
//	 &&(!CALL_APP_CallInStatGet())
//	 &&(!(CodecStat&DECODEOPEN))
//	 &&(!GloVarFlag.RadioFlag)
//	 &&(!GloVarFlag.FmRecStat)
//	 &&(!MenuVarFlg.bSoundChangedFlg)) || CALL_APP_CallOutStatGet() || VOL_BLL_GetVolLevel()==0)
   {
      SPK_INTF_Close();
   }

   if(CHN_BLL_GetAttr()==CHNSTAT_FM)//模拟走模拟芯片（1846或4818则切换音频输出通道（Afout->）
   {
	
   }
//   SoundStatExu = 0;
   MenuVarFlg.VoiceFlg = 0;
   CODECUNLOCK();
}

#if 0
#if 1
/*******************************************************************************
 * 函数名  : SOUND_bll_freq_thrext
 * 描  述  :
 * 输  入  :  参数        描述
 *         :   STD_delay         请输入STD_delay的描述
 *         :   Step_1            请输入Step_1的描述
 *         :   Step_2            请输入Step_2的描述
 *         :   Step_3            请输入Step_3的描述
 *         :   Step_4            请输入Step_4的描述
 *         :   Rshift_1          请输入Rshift_1的描述
 *         :   Rshift_2          请输入Rshift_2的描述
 *         :   Rshift_3          请输入Rshift_3的描述
 *         :   Rshift_4          请输入Rshift_4的描述
 * 输  出  : 无
 * 返回值  : DH_SUCCESS: 成功
 *           DH_FAILURE: 失败
 *******************************************************************************/
DH_VOID SOUND_bll_freq_thrext (DH_U16 STD_delay, DH_U16 Step_1, DH_U16 Step_2,DH_U16 Step_3,DH_U16 Step_4,
		DH_U8 Rshift_1,DH_U8 Rshift_2,DH_U8 Rshift_3,DH_U8 Rshift_4,DH_U8 level)
{
	DH_U16 i;
	DH_U8 j,k;
	DH_S16  DataSum;
	DH_U16 Step_1_;
	DH_U16 Step_2_;
	DH_U16 Step_3_;
	DH_U16 Step_4_;

	STD_delay = STD_delay/32;

	for(i=0;i<STD_delay && !MenuVarFlg.bSoundChangedFlg;i++)
	{
		for(j=0;j<32 && !MenuVarFlg.bSoundChangedFlg;j++)
		{
			Step_1_  =   ((j+32*i)*Step_1)%2048;
			Step_2_  =   ((j+32*i)*Step_2)%2048;
			Step_3_  =   ((j+32*i)*Step_3)%2048;
			Step_4_  =   ((j+32*i)*Step_4)%2048;

            DataSum = (*(gas16CodecBuf + Step_1_)>>Rshift_1) + (*(gas16CodecBuf + Step_2_)>>Rshift_2)
                    + (*(gas16CodecBuf + Step_3_)>>Rshift_3) + (*(gas16CodecBuf + Step_4_)>>Rshift_4);
            DataSum>>=1;
            soundDac[j] = DataSum*level/20;
		}

        for(k = 0;k<32 && !MenuVarFlg.bSoundChangedFlg;k++){
			(*(volatile DH_S8 *)(0x16000980+2*k)) = soundDac[k];
			(*(volatile DH_S8 *)(0x16000981+2*k)) = (soundDac[k]>>8);
		}

        SOUND_bll_WaitAudio(4000);

		 for(k = 0;k<32 && !MenuVarFlg.bSoundChangedFlg;k++){
			(*(volatile DH_S8 *)(0x16000980+2*k)) = soundDac[k];
			(*(volatile DH_S8 *)(0x16000981+2*k)) = (soundDac[k]>>8);
		}
    }
}


/*******************************************************************************
 * 函数名  : SOUND_bll_freq_thr
 * 描  述  : 基于查询方式的提示音写入
 * 输  入  :  参数        描述
 *         :   STD_delay         请输入STD_delay的描述
 *         :   Step_1            请输入Step_1的描述
 *         :   Step_2            请输入Step_2的描述
 *         :   Step_3            请输入Step_3的描述
 *         :   Rshift_1          请输入Rshift_1的描述
 *         :   Rshift_2          请输入Rshift_2的描述
 *         :   Rshift_3          请输入Rshift_3的描述
 *         :   Step_amp          请输入Step_amp的描述
 * 输  出  : 无
 * 返回值  : DH_SUCCESS: 成功
 *           DH_FAILURE: 失败
 *******************************************************************************/
DH_VOID SOUND_bll_freq_thr (DH_U16 STD_delay, DH_U16 Step_1, DH_U16 Step_2,DH_U16 Step_3,DH_U8 Rshift_1,DH_U8 Rshift_2,DH_U8 Rshift_3,DH_U8 Step_amp)
{
	DH_U16 i;
	DH_U8  j,k;
	DH_S16 DataSum;
	DH_U16 Step_1_;
	DH_U16 Step_2_;
	DH_U16 Step_3_;
	DH_U16 Step_amp_;
	STD_delay = STD_delay/32;

	for(i=0;i<STD_delay && !MenuVarFlg.bSoundChangedFlg;i++)
	{
		for(j=0;j<32 && !MenuVarFlg.bSoundChangedFlg;j++)
        {
            Step_1_  =   ((j+32*i)*Step_1)%2048;
            Step_2_  =   ((j+32*i)*Step_2)%2048;
            Step_3_  =   ((j+32*i)*Step_3)%2048;
            Step_amp_ =  ((j+32*i)*Step_amp)%2048;

            if(Step_amp == 0)
            {
                DataSum = (*(gas16CodecBuf + Step_1_)>>Rshift_1)
                		+ (*(gas16CodecBuf + Step_2_)>>Rshift_2)
                        + (*(gas16CodecBuf + Step_3_)>>Rshift_3);
            }
            else
            {
                DataSum = (((*(gas16CodecBuf + Step_1_)>>Rshift_1)
                		  + (*(gas16CodecBuf + Step_2_)>>Rshift_2)
                          + (*(gas16CodecBuf + Step_3_)>>Rshift_3)) * (8-(*(gas16CodecBuf + Step_amp_)>>10)))>>4;
            }
            DataSum>>=1;
            soundDac[j] = DataSum;
        }

	    SOUND_bll_WaitAudio(4000);


		for(k = 0;k<32 && !MenuVarFlg.bSoundChangedFlg;k++){
			(*(volatile DH_S8 *)(0x16000980+2*k)) = soundDac[k];
			(*(volatile DH_S8 *)(0x16000981+2*k)) = (soundDac[k]>>8);
		}
	}
}

#else

/*******************************************************************************
 * 函数名  : SOUND_bll_freq_thrext
 * 描  述  : 
 * 输  入  :  参数        描述
 *         :   STD_delay         请输入STD_delay的描述
 *         :   Step_1            请输入Step_1的描述
 *         :   Step_2            请输入Step_2的描述
 *         :   Step_3            请输入Step_3的描述
 *         :   Step_4            请输入Step_4的描述
 *         :   Rshift_1          请输入Rshift_1的描述
 *         :   Rshift_2          请输入Rshift_2的描述
 *         :   Rshift_3          请输入Rshift_3的描述
 *         :   Rshift_4          请输入Rshift_4的描述
 * 输  出  : 无
 * 返回值  : DH_SUCCESS: 成功
 *           DH_FAILURE: 失败
 *******************************************************************************/
DH_VOID SOUND_bll_freq_thrext (DH_U16 STD_delay, DH_U16 Step_1, DH_U16 Step_2,DH_U16 Step_3,DH_U16 Step_4,
		DH_U8 Rshift_1,DH_U8 Rshift_2,DH_U8 Rshift_3,DH_U8 Rshift_4,DH_U8 level)
{
	DH_U16 i;
	DH_U8 j,k;
	DH_S16  DataSum;
	DH_U16 Step_1_;
	DH_U16 Step_2_;
	DH_U16 Step_3_; 	  
	DH_U16 Step_4_;  
//	DH_U16 amp = level;
//	DH_U16 tail = STD_delay*0.5;
	static DH_U16  dac[32];
	STD_delay = STD_delay/32;//printk("vol %d\r\n",level);
	TIMER_DRV_Enable(TIM1);

	for(i=0;i<STD_delay && !MenuVarFlg.bSoundChangedFlg;i++)
	{
		for(j=0;j<32 && !MenuVarFlg.bSoundChangedFlg;j++)
		{
			Step_1_  =   ((j+32*i)*Step_1)%2048;
			Step_2_  =   ((j+32*i)*Step_2)%2048;
			Step_3_  =   ((j+32*i)*Step_3)%2048;
			Step_4_  =   ((j+32*i)*Step_4)%2048;
//            Step_1_  = (i*Step_1)%2048;
//            Step_2_  = (i*Step_2)%2048;
//            Step_3_  = (i*Step_3)%2048;
//            Step_4_  = (i*Step_4)%2048;

            DataSum = (*(gas16CodecBuf + Step_1_)>>Rshift_1) + (*(gas16CodecBuf + Step_2_)>>Rshift_2)
                    + (*(gas16CodecBuf + Step_3_)>>Rshift_3) + (*(gas16CodecBuf + Step_4_)>>Rshift_4);
            DataSum>>=1;
            dac[j] = DataSum/**level/20*/;
		}

        for(k = 0;k<32 && !MenuVarFlg.bSoundChangedFlg;k++){
			(*(volatile DH_S8 *)(0x16000980+2*k)) = dac[k];
			(*(volatile DH_S8 *)(0x16000981+2*k)) = (dac[k]>>8);
		}
        /*一直等到空为止*/

		dhsem_wait(&Sem_SoundFlg, 0);
        while(((*AUDIO_CONTROL) & BITSET(31)) && !MenuVarFlg.bSoundChangedFlg){

		 for(k = 0;k<32 && !MenuVarFlg.bSoundChangedFlg;k++){
			(*(volatile DH_S8 *)(0x16000980+2*k)) = dac[k];
			(*(volatile DH_S8 *)(0x16000981+2*k)) = (dac[k]>>8);
		}
		/*一直等到空为止*/

		dhsem_wait(&Sem_SoundFlg, 0);
		while(((*AUDIO_CONTROL) & BITSET(31)) && !MenuVarFlg.bSoundChangedFlg){

		}
#if USE_Sem_SoundFlg
        	TIMER_DRV_Enable(TIM1);
			dhsem_wait(&Sem_SoundFlg, 0);
#endif
        }
    }
	TIMER_DRV_Disable(TIM1);
}

/*******************************************************************************
 * 函数名  : SOUND_bll_freq_thr
 * 描  述  : 基于查询方式的提示音写入
 * 输  入  :  参数        描述
 *         :   STD_delay         请输入STD_delay的描述
 *         :   Step_1            请输入Step_1的描述
 *         :   Step_2            请输入Step_2的描述
 *         :   Step_3            请输入Step_3的描述
 *         :   Rshift_1          请输入Rshift_1的描述
 *         :   Rshift_2          请输入Rshift_2的描述
 *         :   Rshift_3          请输入Rshift_3的描述
 *         :   Step_amp          请输入Step_amp的描述
 * 输  出  : 无
 * 返回值  : DH_SUCCESS: 成功
 *           DH_FAILURE: 失败
 *******************************************************************************/
DH_VOID SOUND_bll_freq_thr (DH_U16 STD_delay, DH_U16 Step_1, DH_U16 Step_2,DH_U16 Step_3,DH_U8 Rshift_1,DH_U8 Rshift_2,DH_U8 Rshift_3,DH_U8 Step_amp)
{
	DH_U16 i;
	DH_U8  j,k;
	DH_S16 DataSum;
	DH_U16 Step_1_;
	DH_U16 Step_2_;
	DH_U16 Step_3_; 	  
	DH_U16 Step_amp_;
	static DH_U16  dac[32];

	STD_delay = STD_delay/32;
	TIMER_DRV_Enable(TIM1);
	for(i=0;i<STD_delay && !MenuVarFlg.bSoundChangedFlg;i++)
	{          
		for(j=0;j<32 && !MenuVarFlg.bSoundChangedFlg;j++)
        {        
            Step_1_  =   ((j+32*i)*Step_1)%2048;
            Step_2_  =   ((j+32*i)*Step_2)%2048;
            Step_3_  =   ((j+32*i)*Step_3)%2048;
            Step_amp_ =  ((j+32*i)*Step_amp)%2048;

            if(Step_amp == 0)
            {
                DataSum = (*(gas16CodecBuf + Step_1_)>>Rshift_1)
                		+ (*(gas16CodecBuf + Step_2_)>>Rshift_2)
                        + (*(gas16CodecBuf + Step_3_)>>Rshift_3);
            }
            else
            {
                DataSum = (((*(gas16CodecBuf + Step_1_)>>Rshift_1)
                		  + (*(gas16CodecBuf + Step_2_)>>Rshift_2)
                          + (*(gas16CodecBuf + Step_3_)>>Rshift_3)) * (8-(*(gas16CodecBuf + Step_amp_)>>10)))>>4;
            }
            DataSum>>=1;
            dac[j] = DataSum;
        }
		dhsem_wait(&Sem_SoundFlg, 0);
		/*一直等到空为止*/
		while(((*AUDIO_CONTROL) & BITSET(31)) && !MenuVarFlg.bSoundChangedFlg){
#if USE_Sem_SoundFlg
			if(g_FpgaTx){}
			else
			{
				TIMER_DRV_Enable(TIM1);
				dhsem_wait(&Sem_SoundFlg, 0);
			}
#endif
        }
		for(k = 0;k<32 && !MenuVarFlg.bSoundChangedFlg;k++){
			(*(volatile DH_S8 *)(0x16000980+2*k)) = dac[k];
			(*(volatile DH_S8 *)(0x16000981+2*k)) = (dac[k]>>8);
		}
	}TIMER_DRV_Disable(TIM1);
}
#endif
/*******************************************************************************
 * 函数名  : SOUND_bll_freq_end
 * 描  述  : 基于查询方式的提示音写入
 * 输  入  :  参数        描述
 *         :   STD_delay         请输入STD_delay的描述
 *         :   Step_1            请输入Step_1的描述
 *         :   Step_2            请输入Step_2的描述
 *         :   Step_3            请输入Step_3的描述
 *         :   Rshift_1          请输入Rshift_1的描述
 *         :   Rshift_2          请输入Rshift_2的描述
 *         :   Rshift_3          请输入Rshift_3的描述
 *         :   Step_amp          请输入Step_amp的描述
 * 输  出  : 无
 * 返回值  : DH_SUCCESS: 成功
 *           DH_FAILURE: 失败
 *******************************************************************************/
DH_VOID SOUND_bll_freq_end (DH_VOID)
{
	DH_U16 i;
	DH_U8  j,k;

	for(j=0;j<32 ;j++)
	{
		soundDac[j] = 0;
	}
	for(i=0;i<50 && !MenuVarFlg.bSoundChangedFlg;)
	{
		/*一直等到空为止*/
		SOUND_bll_WaitAudio(4000);
		for(k = 0;k<32 && !MenuVarFlg.bSoundChangedFlg;k++){
			(*(volatile DH_S8 *)(0x16000980+2*k)) = soundDac[k];
			(*(volatile DH_S8 *)(0x16000981+2*k)) = (soundDac[k]>>8);
		}
		i+=16;
	}
	SOUND_bll_WaitAudio(5000);
}

DH_VOID SOUND_bll_WaitAudio (DH_U16 timeout)//us
{
	DH_U16 cnt = timeout*2;
	while(((*AUDIO_CONTROL) & BITSET(31)) && !MenuVarFlg.bSoundChangedFlg && cnt)
	{
		Delay_500ns();
		cnt--;
	}
}


DH_VOID SOUND_bll_PowerOffSound (DH_VOID)//us
{
#if FUNC_MENU_DISP == MENU_DISP_LCD
#elif FUNC_MENU_DISP == MENU_DISP_LED
	/*if(gVoiceSetting[SOUNDSTAT_InterohoneCloseVoice]==1)
	{
		SOUND_BLL_SoundExe(SOUNDSTAT_Voice,SOUNDSTAT_InterohoneCloseVoice);
	}
	else*/
	{
		if(TaskSoundTCB.NamePtr)//任务已创建
		SOUND_BLL_SoundStartOpt(SOUNDSTAT_Voice,SOUNDSTAT_InterohoneCloseVoice);
	}
#else
	if(TaskSoundTCB.NamePtr)//任务已创建
	SOUND_BLL_SoundExe(SOUNDSTAT_Voice,SOUNDSTAT_InterohoneCloseVoice);
#endif
}

DH_VOID HRC_BLL_PowerOnSound1(DH_VOID)
{
	if(!CSBK_APP_PadlockStatGet()) return;
#if LCD_DISPLAY_STAT
    g_SoundLevel = VOL_BLL_GetVolvalue();
    if(CHN_BLL_GetAttr()==CHNSTAT_FM )
    {
	#if FM_LINK_SEL==FM_LINK_HRCx000
    	//关闭FM播放通路输出;
		*AUDIO_CONTROL &= ~(1<<6);
		/* 关闭模拟发送接收时钟 */
//		*CLK_MGR_REG0x2c &= (BITCLR(6) | BITCLR(7));
	#elif FM_LINK_SEL==FM_LINK_FM_IC
		#if FM_IC_SEL==FM_IC_W12T
		GPIO_DRV_Reset(MAP_MIC_AF_SW_PIN);
		if(GloVarFlag.FmRecStat == 1)
		{
			W12T_INTF_Mute();//printk("soundmute1 ");
		}
		#elif FM_IC_SEL==FM_IC_RDA1846S
		GPIO_DRV_Reset(MAP_MIC_AF_SW_PIN);
		if(GloVarFlag.FmRecStat == 1)
		{
			RDA1846S_INTF_FmAfOutMute();//printk("soundmute ");sql%d
		}
		#endif
	#endif
    }

	CODEC_BLL_SetLineOutVol(g_SoundLevel>40?40:g_SoundLevel);

	SoundStatExu = VOL_BLL_GetVol();//音量
	SoundStatExu = SoundStatExu<1?1:SoundStatExu;
	CODEC_BLL_SetLineOutVol(SoundStatExu*3);

	*AUDIO_CONTROL |= BITSET(7);
	DH_U8 gcr = *CODEC_GCR_DACL;
	//Delay_ms(1000);
	
	CPU_SR_ALLOC();
	CPU_CRITICAL_ENTER();
	SOUND_bll_freq_I2S(20,64,0,0,0,2,0,0,0);
	SPK_INTF_Open();
	SOUND_bll_freq_I2S(20,64,0,0,0,2,0,0,0);
	SOUND_bll_freq_I2S(20,100,200,0,0,2,2,0,0);
	SOUND_bll_freq_I2S(20,84,169,253,0,2,2,1,0);
	SOUND_bll_freq_I2S(21,133,266,399,532,2,2,1,0);
	CPU_CRITICAL_EXIT();

	 if ((!(Str_PlayRecd.PlayFlg)
	 &&(!CALL_APP_CallInStatGet())
	 &&(!(CodecStat&DECODEOPEN))
	 &&(!GloVarFlag.RadioFlag)
	 &&(!GloVarFlag.FmRecStat)
	 &&(!MenuVarFlg.bSoundChangedFlg)) || CALL_APP_CallOutStatGet() || VOL_BLL_GetVolLevel()==0)
	{
		SPK_INTF_Close();
	}
	*AUDIO_CONTROL &= BITCLR(7);
	*CODEC_GCR_DACL = gcr;

	

	if(CHN_BLL_GetAttr()==CHNSTAT_FM)//模拟走模拟芯片（1846或4818则切换音频输出通道（Afout->）
	{
		#if FM_LINK_SEL==FM_LINK_FM_IC
		GPIO_DRV_Set(MAP_MIC_AF_SW_PIN);
		#else
		/* 开启模拟发送接收时钟 */
		//		*CLK_MGR_REG0x2c |= BITSET(6)|BITSET(7);
		//	   GPIO_DRV_Reset(MAP_MIC_AF_SW_PIN);
		#endif
	}

#else
	if(gVoiceSetting[SOUNDSTAT_InterohoneOpenVoice ]==1)
	{
		SOUND_BLL_SoundExe(SOUNDSTAT_PowerOn,1);
	}
	else if(gVoiceSetting[SOUNDSTAT_InterohoneOpenVoice ]==2)
	{
		 SOUND_BLL_SoundStartOpt(SOUNDSTAT_Voice,SOUNDSTAT_InterohoneOpenVoice);
		 /*if(CHN_BLL_GetAttr()==CHNSTAT_NONE)
		 {
			SOUND_BLL_SoundStart(SOUNDSTAT_Alarm);
		 }
		 else
		 {
	 		if(gVoiceSetting[SOUNDSTAT_ZoneVoice ]==2)
			{
				SOUND_BLL_SoundExe(SOUNDSTAT_Voice,SOUNDSTAT_ZoneVoice);
			}
		}*/
	}
#endif
}
/*******************************************************************************
 * 函数名  : SOUND_bll_freq_I2S
 * 描  述  : 
 * 输  入  :  参数        描述
 *         :   STD_delay         1表示10ms
 *         :   Step_1            请输入Step_1的描述
 *         :   Step_2            请输入Step_2的描述
 *         :   Step_3            请输入Step_3的描述
 *         :   Step_4            请输入Step_4的描述
 *         :   Rshift_1          请输入Rshift_1的描述
 *         :   Rshift_2          请输入Rshift_2的描述
 *         :   Rshift_3          请输入Rshift_3的描述
 *         :   Rshift_4          请输入Rshift_4的描述
 * 输  出  : 无
 * 返回值  : DH_SUCCESS: 成功
 *           DH_FAILURE: 失败
 *******************************************************************************/
DH_VOID SOUND_bll_freq_I2S(DH_U16 STD_delay, DH_U16 Step_1, DH_U16 Step_2,DH_U16 Step_3,DH_U16 Step_4,DH_U8 Rshift_1,DH_U8 Rshift_2,DH_U8 Rshift_3,DH_U8 Rshift_4)
{
	DH_S16 las16DacBuf[80];

	DH_S16 ls16Sum;
	DH_U16 lu16Step1;
	DH_U16 lu16Step2;
	DH_U16 lu16Step3;
	DH_U16 lu16Step4;
	DH_U8 i,j;
	for(i=0;i<STD_delay;i++)
	{
		for(j=0;j<80;j++)
		{
			lu16Step1  = ((j+80*i)*Step_1)%2048;
			lu16Step2  = ((j+80*i)*Step_2)%2048;
			lu16Step3  = ((j+80*i)*Step_3)%2048;
			lu16Step4  = ((j+80*i)*Step_4)%2048;
			ls16Sum = (*(gas16CodecBuf + lu16Step1)>>Rshift_1) + (*(gas16CodecBuf + lu16Step2)>>Rshift_2)
					+  (*(gas16CodecBuf + lu16Step3)>>Rshift_3)+  (*(gas16CodecBuf + lu16Step4)>>Rshift_4);
			ls16Sum>>=1;

			las16DacBuf[j] = ls16Sum/2;
			SOUND_bll_WaitAudio(4000);
//			while((*AUDIO_CONTROL) & BITSET(31));
//			memcpy(((volatile DH_S16 *)0x16000980),&las16DacBuf[j],2);
			memcpy(((void *)0x16000980),(void *)&las16DacBuf[j],2);
		}
	}
    /*DH_S16 Dac_Buf[80];
    DH_S16 DataSum;
    DH_U16 Step_1_;
    DH_U16 Step_2_;
    DH_U16 Step_3_; 	  
    DH_U16 Step_4_;  
    DH_U8 i;
    DH_U8 j,k;

    for(i=0;i<STD_delay;i++)
    {
        for(j=0;j<80;j++)  
        {
            Step_1_  = ((j+80*i)*Step_1)%2048;
            Step_2_  = ((j+80*i)*Step_2)%2048;
            Step_3_  = ((j+80*i)*Step_3)%2048;	
            Step_4_  = ((j+80*i)*Step_4)%2048;	

            DataSum = (*(gas16CodecBuf + Step_1_)>>Rshift_1) + (*(gas16CodecBuf + Step_2_)>>Rshift_2)
                    +  (*(gas16CodecBuf + Step_3_)>>Rshift_3)+  (*(gas16CodecBuf + Step_4_)>>Rshift_4);   
            DataSum>>=1;

            Dac_Buf[j] = DataSum/2;
        }

		for(k = 0;k<80 ;k++){
			(*(volatile DH_S8 *)(0x180000A0+2*k)) = Dac_Buf[k];
			(*(volatile DH_S8 *)(0x180000A1+2*k)) = (Dac_Buf[k]>>8);
		}
		Delay_ms(10);
		SPK_INTF_Open();
//		dhsem_wait(&Sem_SoundFlg, 0);
    }*/
}

DH_VOID SOUND_bll_freq_I2S_Play(DH_U32 *addr, DH_U32 len)
{
	DH_U32 addrtemp=42;
	DH_U32 LenTmp=len;
	DH_U16 CurPageLen;
	FlashBufLock();

	while(LenTmp){

		if (LenTmp>3200)
		{
			CurPageLen = 3200;
		}
		else
		{
			CurPageLen = LenTmp;
		}


		TC58_INTF_ReadStr(addrtemp,FlashBuf,CurPageLen);
		SOUND_bll_freq_I2S_PlayPCM(FlashBuf,CurPageLen);
		addrtemp += CurPageLen;
		LenTmp -= CurPageLen;
	}

	FlashBufUnLock();
}

DH_VOID SOUND_bll_freq_I2S_PlayPCM(DH_U8 *pcm, DH_U32 len)
{
    DH_U16 i;
//    DH_U8 k;
//	DH_U16 data;
    for(i=0;i<len/128 && !MenuVarFlg.bSoundChangedFlg;i++)
    {
    	SOUND_bll_WaitAudio(4000);
    	/*for(k = 0;k<32 ;k++){
//    				data=pcm[64*i+2*k]<<8|pcm[64*i+2*k+1];
    				(*(volatile DH_S8 *)(0x16000980+2*k)) = pcm[128*i+4*k];
    				(*(volatile DH_S8 *)(0x16000981+2*k)) = pcm[128*i+4*k+1];
//    				printk(" %02x %02x",pcm[64*i+2*k],pcm[64*i+2*k+1]);
    			}*/
		while((*AUDIO_CONTROL) & BITSET(31));
		/*for(k = 0;k<32 ;k++){
			data=pcm[64*i+2*k]<<8|pcm[64*i+2*k+1];
			(*(volatile DH_S8 *)(0x16000980+2*k)) = pcm[64*i+2*k+1];
			(*(volatile DH_S8 *)(0x16000981+2*k)) = pcm[64*i+2*k+2];
			printk(" %02x %02x",pcm[64*i+2*k],pcm[64*i+2*k+1]);
		}*/
    }
}

/*******************************************************************************
 * 函数名  : SOUND_BLL_KeyVoice
 * 描  述  : 
 * 输  入  :  参数        描述
 *         :   DH_VOID         请输入DH_VOID的描述
 * 输  出  : 无
 * 返回值  : DH_SUCCESS: 成功
 *           DH_FAILURE: 失败
 *******************************************************************************/
DH_VOID SOUND_BLL_KeyVoice(DH_VOID)
{
	if(g_Layer ==4 &&  Str_MenuItemBackUp(1)  == E_ToneSet/*提示音设置*/
		&&(  Str_MenuItemBackUp(2) ==EE_Tone_MsgTones/*短信提示音*/
			|| Str_MenuItemBackUp(2) ==EE_Tone_CallAlert))/*呼叫提示音*/
	{
		
		if( Str_MenuItemBackUp(2)  == EE_Tone_MsgTones)
		{
			MenuVarFlg.KeySoundIgnoreFlg = TRUE;
			MenuVarFlg.bSoundChangedFlg = TRUE;
			SOUND_BLL_SoundStart(g_Item?(SOUNDSTAT_5 + g_Item):0);
		}
		else if( Str_MenuItemBackUp(2)  == EE_Tone_CallAlert)
		{
			MenuVarFlg.KeySoundIgnoreFlg = TRUE;
			MenuVarFlg.bSoundChangedFlg = TRUE;
			SOUND_BLL_SoundStart(g_Item);
		}
	}
	else if(g_Layer ==5 &&  Str_MenuItemBackUp(1)  ==E_ToneSet/*提示音设置*/
		&& (( Str_MenuItemBackUp(2)  == E_Tone_PrivateCall
		||  Str_MenuItemBackUp(2)  == E_Tone_GroupCall)
		&&  Str_MenuItemBackUp(3)  == E_Tone_Select))
	{
		MenuVarFlg.KeySoundIgnoreFlg = TRUE;
		MenuVarFlg.bSoundChangedFlg = TRUE;
		if(g_Item == 0) return;
		{
			SOUND_BLL_SoundStart(g_Item);
		}
	}
}

/*****************************************************************************
 *   函数名: AudioVoiceOutSelect
 *   功能描述: 语音报号芯片的输出所有情况
 *   参数定义:
 *	返回值:
 *   创建人：liuzhiheng 2015 /4/3
******************************************************************************/
DH_VOID SOUND_BLL_Tone(DH_U8 stat)
{
	switch(stat)
	{
		case SOUNDSTAT_InterohoneOpenVoice :
		case SOUNDSTAT_ChnNumberVoice:
		case SOUNDSTAT_BSChnNumberVoice:
		case SOUNDSTAT_LookInfmVoice:
		case SOUNDSTAT_PCtoMCUStartVoice:
		case SOUNDSTAT_PCtoMCUEndVoice:
		case SOUNDSTAT_ZoneVoice:
		case SOUNDSTAT_ScanStartVoice:
		case SOUNDSTAT_ScanEndVoice:
		case SOUNDSTAT_LockVoice:
		case SOUNDSTAT_UnLockVoice:
		case SOUNDSTAT_RedAlarmVoice:
		case SOUNDSTAT_RedAlarmStpVoice:
		case SOUNDSTAT_GeHuVoice:
		case SOUNDSTAT_GeHuEndVoice:
		case SOUNDSTAT_ZuHuVoice:
		case SOUNDSTAT_ZuHuEndVoice:
		case SOUNDSTAT_QuanHuVoice:
		case SOUNDSTAT_TxTimeOutVoice:
		case SOUNDSTAT_BsTxTimeOutVoice:
		case SOUNDSTAT_EncrptOpenVoice:
		case SOUNDSTAT_EncrptCloseVoice:
		case SOUNDSTAT_SKOpenVoice:
		case SOUNDSTAT_SKCloseVoice:
		case SOUNDSTAT_VolumeGradeVoice:
		case SOUNDSTAT_SQGradeVoice:
		case SOUNDSTAT_ScanCallInStart:
		case SOUNDSTAT_ScanMode:
		case SOUNDSTAT_CouloStatVoice:
		case SOUNDSTAT_PowerVoice:
		case SOUNDSTAT_PwrOff:

			SOUND_bll_freq_thr(1000,380,0,0,0,0,0,0);
			break;


		case SOUNDSTAT_InterohoneCloseVoice:

			VoxCallOutTimeOut=VOX_I_MAX;
			CPU_SR_ALLOC();
			CPU_CRITICAL_ENTER();
			SOUND_bll_freq_I2S(30,74,10,0,0,2,0,0,0);
			SOUND_bll_freq_I2S(30,110,210,0,0,2,2,0,0);
			SOUND_bll_freq_I2S(30,94,179,263,0,2,2,1,0);
			SOUND_bll_freq_I2S(30,54,159,203,0,2,2,1,0);
			CPU_CRITICAL_EXIT();

			/*SOUND_bll_freq_thr(1000,380,0,0,0,0,0,0);
			//for(i=0;i<Str_Audio.Colour;i++)
			{
				Delay_ms(150);
				SOUND_bll_freq_thr(1000,380,0,0,0,0,0,0);
			}*/
			break;

		case SOUNDSTAT_LowCouloVoice:
			SOUND_bll_freq_thr(7200,440,0,0,0,0,0,0);
			break;
		default :
			SOUND_bll_freq_thr(800,512,0,0,1,0,0,0);
			break;
	}
}


DH_VOID SOUND_BLL_VICNum(DH_U16 num,DH_U8 *buf,DH_U8 *len)
{
//	if(num>999) return;
	if(num/100)
	{
		if(StrMenuSet.language == 0)
		{
			if(num/100>1)
			{
				buf[(*len)++] = num/100 + VIC_0;
			}else{
				buf[(*len)++] = VIC_Hundred;
			}
		}else{
			buf[(*len)++] = num/100 + VIC_0;
			buf[(*len)++] = VIC_Hundred;
		}

		num %= 100;
	}

	if(num<=20)
	{
		buf[(*len)++] = num + VIC_0;
	}
	else
	{
		buf[(*len)++] = num/10 + VIC_Twenty - 2;
		if(num%10)
		{
			buf[(*len)++] = num%10 + VIC_0;
		}
	}
}

DH_VOID SOUND_BLL_VoiceIC(DH_U8 stat)
{
	DH_U8 len = 0,voiceAddr[14] = {0};

	switch(stat)
	{
		case SOUNDSTAT_InterohoneOpenVoice :/**/
			voiceAddr[len++] = VIC_Welcome;
			voiceAddr[len++] = VIC_DMR;
			voiceAddr[len++] = VIC_Digital;
			voiceAddr[len++] = VIC_Product;
			break;

		case SOUNDSTAT_InterohoneCloseVoice:
			voiceAddr[len++] = VIC_goodbye;
			break;

		case SOUNDSTAT_ChnNumberVoice:         /*频道报号*/
//			if(CHN_BLL_GetDFMAttr())
//			{
//				voiceAddr[len++] = CHN_BLL_GetDFMAttr()==CHNSTAT_DMR?VIC_Digital:VIC_Analog;
//				voiceAddr[len++] = CHN_BLL_GetDFMAttr()==CHNSTAT_DMR?VIC_Analog:VIC_Digital;
//			}
//			else if(CHN_BLL_GetAttr())
//			{
//				voiceAddr[len++] = CHN_BLL_GetAttr()==CHNSTAT_DMR?VIC_Digital:VIC_Analog;
//			}
//			else
//			{
//				voiceAddr[len++] = VIC_Channel;
//				SOUND_BLL_VICNum(CODESW_INTF_GetStatus()+1,voiceAddr,&len);
//				voiceAddr[len++] = VIC_Off;
//				break;
//			}
			SOUND_BLL_VICNum(CODESW_INTF_GetStatus()+1,voiceAddr,&len);

			break;

		case SOUNDSTAT_BSChnNumberVoice:/*中继频道报号*/
			voiceAddr[len++] = CHN_BLL_GetDMRChnInfo().u8TalkAround?VIC_Talk_Around:VIC_Repeater;
			voiceAddr[len++] = VIC_On;
			break;

//		case SOUNDSTAT_LookInfmVoice:/*频道报号:区域xxx*/
//			voiceAddr[len++] = VIC_Zone;
//			SOUND_BLL_VICNum(MENU_APP_GetMenuSetStat(BARTYPESETFLG_ZONE)+1,voiceAddr,&len);
//			if(CHN_BLL_GetDFMAttr())
//			{
//				voiceAddr[len++] = CHN_BLL_GetDFMAttr()==CHNSTAT_DMR?VIC_Digital:VIC_Analog;
//				voiceAddr[len++] = CHN_BLL_GetDFMAttr()==CHNSTAT_DMR?VIC_Analog:VIC_Digital;
//			}
//			else if(CHN_BLL_GetAttr())
//			{
//				voiceAddr[len++] = CHN_BLL_GetAttr()==CHNSTAT_DMR?VIC_Digital:VIC_Analog;
//			}
//			else
//			{
//				voiceAddr[len++] = VIC_Channel;
//				SOUND_BLL_VICNum(CODESW_INTF_GetStatus()+1,voiceAddr,&len);
//				voiceAddr[len++] = VIC_Off;
//				break;
//			}

//			SOUND_BLL_VICNum(CODESW_INTF_GetStatus()+1,voiceAddr,&len);

//			/*if(CHN_BLL_GetAttr()==CHNSTAT_DMR)
//			{
//				if(MENU_APP_GetMenuSetStat(BARTYPESETFLG_BStime)==1)//中继
//					voiceAddr[len++] = VIC_Repeater;
//				else
//					voiceAddr[len++] = VIC_Talk_Around;
//				if(DPATCS==CALL_APP_CallInTypeGet())	  voiceAddr[len++] = VIC_Private_Call;
//				else if(DGROUP==CALL_APP_CallInTypeGet()) voiceAddr[len++] = VIC_Group_Call;
//				else voiceAddr[len++] = VIC_General_Call;
//			}*/
//			voiceAddr[len++] = VIC_Battery_Level;
//			voiceAddr[len++] = g_Coulo==0?VIC_High:(g_Coulo==3?VIC_Low:VIC_Middle);
//			break;

//		case SOUNDSTAT_ZoneVoice:
//			voiceAddr[len++] = VIC_Zone;
//			SOUND_BLL_VICNum(MENU_APP_GetMenuSetStat(BARTYPESETFLG_ZONE)+1,voiceAddr,&len);
//			//voiceAddr[len++] = VIC_0;
//			if(CHN_BLL_GetDFMAttr())
//			{
//				voiceAddr[len++] = CHN_BLL_GetDFMAttr()==CHNSTAT_DMR?VIC_Digital:VIC_Analog;
//				voiceAddr[len++] = CHN_BLL_GetDFMAttr()==CHNSTAT_DMR?VIC_Analog:VIC_Digital;
//			}
//			else if(CHN_BLL_GetAttr())
//			{
//				voiceAddr[len++] = CHN_BLL_GetAttr()==CHNSTAT_DMR?VIC_Digital:VIC_Analog;
//			}
//			else
//			{
//				voiceAddr[len++] = VIC_Channel;
//				SOUND_BLL_VICNum(CODESW_INTF_GetStatus()+1,voiceAddr,&len);
//				voiceAddr[len++] = VIC_Off;
//				break;
//			}
//			SOUND_BLL_VICNum(CODESW_INTF_GetStatus()+1,voiceAddr,&len);
//			break;

		case SOUNDSTAT_CouloStatVoice:/*电量等级：电量xxx*/
			voiceAddr[len++] = VIC_Battery_Level;
			voiceAddr[len++] = g_Coulo==0?VIC_High:(g_Coulo==3?VIC_Low:VIC_Middle);
			break;

		case SOUNDSTAT_LowCouloVoice:/*低电量提示：电量低，请充电*/
			voiceAddr[len++] = VIC_Battery_Level;
			if(g_PowerForbidden==1)
			{
				voiceAddr[len++] = VIC_Low;
				voiceAddr[len++] = VIC_Pls_charge;
			}
			else
			{
				voiceAddr[len++] = VIC_High;
			}
			break;

		case SOUNDSTAT_PowerVoice:   /*功率选择：功率高/低*/
			voiceAddr[len++] = CHN_BLL_PowerGet() == MENUSETPOWER_M ? VIC_High : VIC_Low;
			break;

		case SOUNDSTAT_PwrOff:
			voiceAddr[len++] = VIC_Power;
			voiceAddr[len++] = VIC_Off;
			break;

		case SOUNDSTAT_PCtoMCUStartVoice:
			voiceAddr[len++] = 0;
			voiceAddr[len++] = 0;
			break;

		case SOUNDSTAT_PCtoMCUEndVoice:
			voiceAddr[len++] = 0;
			voiceAddr[len++] = 0;
			break;

		case SOUNDSTAT_ScanStartVoice:
			voiceAddr[len++] = VIC_Scan;
			voiceAddr[len++] = VIC_On;
			break;

		case SOUNDSTAT_ScanEndVoice:
			voiceAddr[len++] = VIC_Scan;
			voiceAddr[len++] = VIC_Off;
			break;

		case SOUNDSTAT_LockVoice:
			voiceAddr[len++] = VIC_Lock;
			break;
		case SOUNDSTAT_UnLockVoice:
			voiceAddr[len++] = VIC_Unlock;
			break;

		case SOUNDSTAT_RedAlarmVoice:
			voiceAddr[len++] = VIC_Alarm;
			voiceAddr[len++] = VIC_On;
			break;

		case SOUNDSTAT_RedAlarmStpVoice:
			voiceAddr[len++] = VIC_Alarm;
			voiceAddr[len++] = VIC_Off;
			break;

		case SOUNDSTAT_GeHuVoice:
			voiceAddr[len++] = VIC_Private_Call;
			break;

		case SOUNDSTAT_GeHuEndVoice:
			voiceAddr[len++] = VIC_Private_Call;
			voiceAddr[len++] = VIC_Off;
			break;

		case SOUNDSTAT_ZuHuVoice:
			voiceAddr[len++] = VIC_Group_Call;
			break;

		case SOUNDSTAT_ZuHuEndVoice:
			voiceAddr[len++] = VIC_Group_Call;
			voiceAddr[len++] = VIC_Off;
			break;

		case SOUNDSTAT_QuanHuVoice:
			voiceAddr[len++] = VIC_General_Call;
			break;

		case SOUNDSTAT_TxTimeOutVoice:
			voiceAddr[len++] = VIC_Call;
			voiceAddr[len++] = VIC_Time_out;
			break;

		case SOUNDSTAT_BsTxTimeOutVoice:/*中继激活超时*/
			voiceAddr[len++] = VIC_Repeater;
			voiceAddr[len++] = VIC_Revive;
			voiceAddr[len++] = VIC_Time_out;
			break;

		case SOUNDSTAT_EncrptOpenVoice:/*加密开*/
			voiceAddr[len++] = VIC_Encryption;
			voiceAddr[len++] = VIC_On;
			break;

		case SOUNDSTAT_EncrptCloseVoice:/*加密关*/

			voiceAddr[len++] = VIC_Encryption;
			voiceAddr[len++] = VIC_Off;
			break;

		case SOUNDSTAT_SKOpenVoice:    /*声控开*/
			voiceAddr[len++] = VIC_VOX;
			voiceAddr[len++] = VIC_On;
			break;

		case SOUNDSTAT_SKCloseVoice:   /*声控关*/
			voiceAddr[len++] = VIC_VOX;
			voiceAddr[len++] = VIC_Off;
			break;

		case SOUNDSTAT_VolumeGradeVoice: /*音量xxx*/
			voiceAddr[len++] = VIC_Volume;
			voiceAddr[len++] = VOL_BLL_GetVolLevel()==0?VIC_Off:VOL_BLL_GetVolLevel();
			break;
		case SOUNDSTAT_VolumeGradeVoice2: /*音量xxx*/
			voiceAddr[len++] = VOL_BLL_GetVolLevel()==0?VIC_Off:VOL_BLL_GetVolLevel();
			break;

		case SOUNDSTAT_SQGradeVoice:    /*静噪级别选择：静噪xxx*/
			voiceAddr[len++] = VIC_Squelch;
			voiceAddr[len++] = MENU_APP_GetMenuSetStat(BARTYPESETFLG_SqLevel);
			if(voiceAddr[len-1]==0) voiceAddr[len-1] = VIC_On;
			break;

		case SOUNDSTAT_ScanCallInStart:
			voiceAddr[len++] = CHN_BLL_GetAttr()==CHNSTAT_DMR?VIC_Digital:VIC_Analog;
			voiceAddr[len++] = VIC_Channel;
			SOUND_BLL_VICNum(CODESW_INTF_GetStatus()+1,voiceAddr,&len);
			break;

		case SOUNDSTAT_ScanMode:
			voiceAddr[len++] = VIC_Scan;
			voiceAddr[len++] = VIC_Mode;
			voiceAddr[len++] = MENU_APP_GetMenuSetStat(BARTYPESETFLG_SCANMODE)+1;
			break;

		case SOUNDSTAT_UnProgrammedChnNumberVoice:
			voiceAddr[len++] = VIC_Zone;
			SOUND_BLL_VICNum(MENU_APP_GetMenuSetStat(BARTYPESETFLG_ZONE)+1,voiceAddr,&len);
			SOUND_BLL_VICNum(CODESW_INTF_GetStatus()+1,voiceAddr,&len);

			break;

		case SOUNDSTAT_UnProgrammedChnZoneVoice:
			voiceAddr[len++] = VIC_Zone;
			SOUND_BLL_VICNum(MENU_APP_GetMenuSetStat(BARTYPESETFLG_ZONE)+1,voiceAddr,&len);
			SOUND_BLL_VICNum(CODESW_INTF_GetStatus()+1,voiceAddr,&len);
			break;

		case SOUNDSTAT_Revive:
			voiceAddr[len++] = VIC_Remote;
			voiceAddr[len++] = VIC_Revive;
			break;

		case SOUNDSTAT_Kill:
			voiceAddr[len++] = VIC_Remote;
			voiceAddr[len++] = VIC_Kill;
			break;

		case SOUNDSTAT_Monitor:
			voiceAddr[len++] = VIC_Remote;
			voiceAddr[len++] = VIC_Monitor;
			voiceAddr[len++] = VIC_On;
			break;

		case SOUNDSTAT_Detection:
			voiceAddr[len++] = VIC_Remote;
			voiceAddr[len++] = VIC_Detection;
			break;

		case SOUNDSTAT_Monitor_A1:/*模拟监听1*/
			voiceAddr[len++] = VIC_Monitor;
			voiceAddr[len++] = VIC_One;
			voiceAddr[len++] = GloVarFlag.FmMontFlg?VIC_On:VIC_Off;
			break;
		case SOUNDSTAT_Monitor_A2:/*模拟监听2（永久监听）*/
			voiceAddr[len++] = VIC_Monitor;
			voiceAddr[len++] = VIC_One+1;
			voiceAddr[len++] = GloVarFlag.FmMontCFlg?VIC_On:VIC_Off;
			break;

		case SOUNDSTAT_Prompt:
			voiceAddr[len++] = VIC_Call;
			voiceAddr[len++] = VIC_Prompt;
			break;

		case SOUNDSTAT_SetOn:
			voiceAddr[len++] = VIC_On;
			break;
		case SOUNDSTAT_SetOff:
			voiceAddr[len++] = VIC_Off;
			break;
		case SOUNDSTAT_RoamConnected:
			voiceAddr[len++] = VIC_Repeater;
			voiceAddr[len++] = VIC_Revive;
			break;
		default :break;
	}
	VOICEIC_INTF_PlayTone(voiceAddr,len);
}



//播报未编程提示音
DH_VOID SOUND_BLL_Audio_CHN_Err(DH_VOID)
{
#if(!LCD_DISPLAY_STAT)
	SOUND_BLL_SoundStartOpt(SOUNDSTAT_Voice, SOUNDSTAT_UnProgrammedChnZoneVoice);
#endif
	SOUND_BLL_SoundStart(SOUNDSTAT_Alarm);
}


DH_VOID SOUND_BLL_StopCurTone(DH_VOID)
{
	MenuVarFlg.bSoundChangedFlg = 1;
}

DH_U8 SOUND_BLL_StartCallTonePlay(DH_U8 u8Gi,DH_U8 call)
{
	DH_U8 ProfStat = MENU_APP_GetMenuSetStat(BARTYPESETFLG_Profile);
	if (ProfStat == GENERALFLAG || ProfStat == VOIEC_VIBRATINGFLAG)
	{
	#if FUNC_MENU_DISP == MENU_DISP_LCD
		if (u8Gi== DGROUP)
		{
			if((CALLSTR_CALLIN == call  && MENU_APP_GetMenuSetStat(g_FpgaRx?BARTYPESETFLG_GroupCallIn:BARTYPESETFLG_GroupCallInStop))
			 ||(CALLSTR_CALLOUT == call && MENU_APP_GetMenuSetStat(g_FpgaTx?BARTYPESETFLG_GroupCallOut:BARTYPESETFLG_GroupCallOutStop)))
			{
				SOUND_BLL_SoundExe(MENU_APP_GetMenuSetStat(BARTYPESETFLG_GroupCallTone),0);
				return 1;
			}
		}
		else
		{
			if((CALLSTR_CALLIN == call  && MENU_APP_GetMenuSetStat(g_FpgaRx?BARTYPESETFLG_SingleCallIN:BARTYPESETFLG_SingleCallINStop))
			 ||(CALLSTR_CALLOUT == call && MENU_APP_GetMenuSetStat(g_FpgaTx?BARTYPESETFLG_SingleCallOut:BARTYPESETFLG_SingleCallOutStop)))
			{
				SOUND_BLL_SoundExe(MENU_APP_GetMenuSetStat(BARTYPESETFLG_SingleCallTone),0);
				return 1;
			}
		}
	#else
		if(CALLSTR_CALLOUT == call)
		{
			if(SCAN_APP_ScanCtrlGet() == 3) //SCAN_SUSPEND
			{}
			else
			{
				if(g_FpgaTx)
				{
					SOUND_BLL_StopCurTone();

					if (u8Gi== DGROUP)
					{
						SOUND_BLL_SoundExe(SOUNDSTAT_Voice,SOUNDSTAT_ZuHuVoice);
					}
					else if(DALLCALL==u8Gi)
					{
						SOUND_BLL_SoundExe(SOUNDSTAT_Voice,SOUNDSTAT_QuanHuVoice);
					}
					else
					{
						SOUND_BLL_SoundExe(SOUNDSTAT_Voice,SOUNDSTAT_GeHuVoice);
					}
				}
				else
				{
					if (u8Gi== DGROUP)
					{
						if(gVoiceSetting[SOUNDSTAT_ZuHuEndVoice]==1)
						{
							SOUND_BLL_SoundExe(SOUNDSTAT_Voice,SOUNDSTAT_ZuHuEndVoice);
						}
					}
					else if(DALLCALL==u8Gi)
					{
						if(gVoiceSetting[SOUNDSTAT_QuanHuVoice]==1)
						{
							SOUND_BLL_SoundExe(SOUNDSTAT_Voice,SOUNDSTAT_QuanHuVoice);
						}
					}
					else
					{
						if(gVoiceSetting[SOUNDSTAT_ZuHuEndVoice]==1)
						{
							SOUND_BLL_SoundExe(SOUNDSTAT_Voice,SOUNDSTAT_GeHuEndVoice);
						}
					}
				}
			}
			return 1;
		}
		else if(CALLSTR_CALLIN == call && g_FpgaRx==0)
		{
			SPK_INTF_Close();
		}
	#endif
	}

	return 0;
}
#endif

/*****************************************************************************
 *   函数名: AudioVoiceOutSelect
 *   功能描述: 语音报号芯片的输出所有情况
 *   参数定义:
 *	返回值:
 *   创建人：liuzhiheng 2015 /4/3
******************************************************************************/
DH_VOID SOUND_BLL_Tone(DH_U8 stat)
{
	#if 0
	switch(stat)
	{
		case SOUNDSTAT_InterohoneOpenVoice :
		case SOUNDSTAT_ChnNumberVoice:
		case SOUNDSTAT_BSChnNumberVoice:
		case SOUNDSTAT_LookInfmVoice:
		case SOUNDSTAT_PCtoMCUStartVoice:
		case SOUNDSTAT_PCtoMCUEndVoice:
		case SOUNDSTAT_ZoneVoice:
		case SOUNDSTAT_ScanStartVoice:
		case SOUNDSTAT_ScanEndVoice:
		case SOUNDSTAT_LockVoice:
		case SOUNDSTAT_UnLockVoice:
		case SOUNDSTAT_RedAlarmVoice:
		case SOUNDSTAT_RedAlarmStpVoice:
		case SOUNDSTAT_GeHuVoice:
		case SOUNDSTAT_GeHuEndVoice:
		case SOUNDSTAT_ZuHuVoice:
		case SOUNDSTAT_ZuHuEndVoice:
		case SOUNDSTAT_QuanHuVoice:
		case SOUNDSTAT_TxTimeOutVoice:
		case SOUNDSTAT_BsTxTimeOutVoice:
		case SOUNDSTAT_EncrptOpenVoice:
		case SOUNDSTAT_EncrptCloseVoice:
		case SOUNDSTAT_SKOpenVoice:
		case SOUNDSTAT_SKCloseVoice:
		case SOUNDSTAT_VolumeGradeVoice:
		case SOUNDSTAT_SQGradeVoice:
		case SOUNDSTAT_ScanCallInStart:
		case SOUNDSTAT_ScanMode:
		case SOUNDSTAT_CouloStatVoice:
		case SOUNDSTAT_PowerVoice:
		case SOUNDSTAT_PwrOff:

		//	SOUND_bll_freq_thr(1000,380,0,0,0,0,0,0);
			break;


		case SOUNDSTAT_InterohoneCloseVoice:

			VoxCallOutTimeOut=VOX_I_MAX;
			
			break;

		case SOUNDSTAT_LowCouloVoice:
		//	SOUND_bll_freq_thr(7200,440,0,0,0,0,0,0);
			break;
		default :
		//	SOUND_bll_freq_thr(800,512,0,0,1,0,0,0);
			break;
	}
	#endif
}


DH_VOID SOUND_BLL_VICNum(DH_U16 num,DH_U8 *buf,DH_U8 *len)
{
//	if(num>999) return;
	if(num/100)
	{
		if( 0)
		{
			if(num/100>1)
			{
				buf[(*len)++] = num/100 + VIC_0;
			}else{
				buf[(*len)++] = VIC_Hundred;
			}
		}else{
			buf[(*len)++] = num/100 + VIC_0;
			buf[(*len)++] = VIC_Hundred;
		}

		num %= 100;
	}

	if(num<=20)
	{
		buf[(*len)++] = num + VIC_0;
	}
	else
	{
		buf[(*len)++] = num/10 + VIC_Twenty - 2;
		if(num%10)
		{
			buf[(*len)++] = num%10 + VIC_0;
		}
	}
}

DH_VOID SOUND_BLL_VoiceIC(DH_U8 stat)
{
	DH_U8 len = 0,voiceAddr[14] = {0};
#if 0
	switch(stat)
	{
		case SOUNDSTAT_InterohoneOpenVoice :/**/
			voiceAddr[len++] = VIC_Welcome;
			voiceAddr[len++] = VIC_DMR;
			voiceAddr[len++] = VIC_Digital;
			voiceAddr[len++] = VIC_Product;
			break;

		case SOUNDSTAT_InterohoneCloseVoice:
			voiceAddr[len++] = VIC_goodbye;
			break;

		case SOUNDSTAT_ChnNumberVoice:         /*频道报号*/
//			if(CHN_BLL_GetDFMAttr())
//			{
//				voiceAddr[len++] = CHN_BLL_GetDFMAttr()==CHNSTAT_DMR?VIC_Digital:VIC_Analog;
//				voiceAddr[len++] = CHN_BLL_GetDFMAttr()==CHNSTAT_DMR?VIC_Analog:VIC_Digital;
//			}
//			else if(CHN_BLL_GetAttr())
//			{
//				voiceAddr[len++] = CHN_BLL_GetAttr()==CHNSTAT_DMR?VIC_Digital:VIC_Analog;
//			}
//			else
//			{
//				voiceAddr[len++] = VIC_Channel;
//				SOUND_BLL_VICNum(CODESW_INTF_GetStatus()+1,voiceAddr,&len);
//				voiceAddr[len++] = VIC_Off;
//				break;
//			}
			SOUND_BLL_VICNum(CODESW_INTF_GetStatus()+1,voiceAddr,&len);

			break;

//		case SOUNDSTAT_BSChnNumberVoice:/*中继频道报号*/
//			voiceAddr[len++] = CHN_BLL_GetDMRChnInfo().u8TalkAround?VIC_Talk_Around:VIC_Repeater;
//			voiceAddr[len++] = VIC_On;
//			break;
//
//		case SOUNDSTAT_LookInfmVoice:/*频道报号:区域xxx*/
//			voiceAddr[len++] = VIC_Zone;
//			SOUND_BLL_VICNum(MENU_APP_GetMenuSetStat(BARTYPESETFLG_ZONE)+1,voiceAddr,&len);
//			if(CHN_BLL_GetDFMAttr())
//			{
//				voiceAddr[len++] = CHN_BLL_GetDFMAttr()==CHNSTAT_DMR?VIC_Digital:VIC_Analog;
//				voiceAddr[len++] = CHN_BLL_GetDFMAttr()==CHNSTAT_DMR?VIC_Analog:VIC_Digital;
//			}
//			else if(CHN_BLL_GetAttr())
//			{
//				voiceAddr[len++] = CHN_BLL_GetAttr()==CHNSTAT_DMR?VIC_Digital:VIC_Analog;
//			}
//			else
//			{
//				voiceAddr[len++] = VIC_Channel;
//				SOUND_BLL_VICNum(CODESW_INTF_GetStatus()+1,voiceAddr,&len);
//				voiceAddr[len++] = VIC_Off;
//				break;
//			}
//
//			SOUND_BLL_VICNum(CODESW_INTF_GetStatus()+1,voiceAddr,&len);
//
//			/*if(CHN_BLL_GetAttr()==CHNSTAT_DMR)
//			{
//				if(MENU_APP_GetMenuSetStat(BARTYPESETFLG_BStime)==1)//中继
//					voiceAddr[len++] = VIC_Repeater;
//				else
//					voiceAddr[len++] = VIC_Talk_Around;
//				if(DPATCS==CALL_APP_CallInTypeGet())	  voiceAddr[len++] = VIC_Private_Call;
//				else if(DGROUP==CALL_APP_CallInTypeGet()) voiceAddr[len++] = VIC_Group_Call;
//				else voiceAddr[len++] = VIC_General_Call;
//			}*/
//			voiceAddr[len++] = VIC_Battery_Level;
//			voiceAddr[len++] = g_Coulo==0?VIC_High:(g_Coulo==3?VIC_Low:VIC_Middle);
//			break;
//
//		case SOUNDSTAT_ZoneVoice:
//			voiceAddr[len++] = VIC_Zone;
////			voiceAddr[len++] = VIC_Digital;
////			voiceAddr[len++] = VIC_Zone;
////			voiceAddr[len++] = VIC_Analog;
//			SOUND_BLL_VICNum(MENU_APP_GetMenuSetStat(BARTYPESETFLG_ZONE)+1,voiceAddr,&len);
//			//voiceAddr[len++] = VIC_0;
//			if(CHN_BLL_GetDFMAttr())
//			{
//				voiceAddr[len++] = CHN_BLL_GetDFMAttr()==CHNSTAT_DMR?VIC_Digital:VIC_Analog;
//				voiceAddr[len++] = CHN_BLL_GetDFMAttr()==CHNSTAT_DMR?VIC_Analog:VIC_Digital;
//			}
//			else if(CHN_BLL_GetAttr())
//			{
//				voiceAddr[len++] = CHN_BLL_GetAttr()==CHNSTAT_DMR?VIC_Digital:VIC_Analog;
//			}
//			else
//			{
//				voiceAddr[len++] = VIC_Channel;
//				SOUND_BLL_VICNum(CODESW_INTF_GetStatus()+1,voiceAddr,&len);
//				voiceAddr[len++] = VIC_Off;
//				break;
//			}
//			SOUND_BLL_VICNum(CODESW_INTF_GetStatus()+1,voiceAddr,&len);
//			break;

		case SOUNDSTAT_CouloStatVoice:/*电量等级：电量xxx*/
			voiceAddr[len++] = VIC_Battery_Level;
			voiceAddr[len++] = g_Coulo==0?VIC_High:(g_Coulo==3?VIC_Low:VIC_Middle);
			break;

		case SOUNDSTAT_LowCouloVoice:/*低电量提示：电量低，请充电*/
			voiceAddr[len++] = VIC_Battery_Level;
			if(g_PowerForbidden==1)
			{
				voiceAddr[len++] = VIC_Low;
				voiceAddr[len++] = VIC_Pls_charge;
			}
			else
			{
				voiceAddr[len++] = VIC_High;
			}
			break;

		case SOUNDSTAT_PowerVoice:   /*功率选择：功率高/低*/
			voiceAddr[len++] = CHN_BLL_PowerGet() == MENUSETPOWER_M ? VIC_High : VIC_Low;
			break;

		case SOUNDSTAT_PwrOff:
			voiceAddr[len++] = VIC_Power;
			voiceAddr[len++] = VIC_Off;
			break;

//		case SOUNDSTAT_PCtoMCUStartVoice:
//			voiceAddr[len++] = 0;
//			voiceAddr[len++] = 0;
//			break;
//
//		case SOUNDSTAT_PCtoMCUEndVoice:
//			voiceAddr[len++] = 0;
//			voiceAddr[len++] = 0;
//			break;

		case SOUNDSTAT_ScanStartVoice:
			voiceAddr[len++] = VIC_Scan;
			voiceAddr[len++] = VIC_On;
			break;

		case SOUNDSTAT_ScanEndVoice:
			voiceAddr[len++] = VIC_Scan;
			voiceAddr[len++] = VIC_Off;
			break;

		case SOUNDSTAT_LockVoice:
			voiceAddr[len++] = VIC_Lock;
			break;
		case SOUNDSTAT_UnLockVoice:
			voiceAddr[len++] = VIC_Unlock;
			break;

//		case SOUNDSTAT_RedAlarmVoice:
//			voiceAddr[len++] = VIC_Alarm;
//			voiceAddr[len++] = VIC_On;
//			break;
//
//		case SOUNDSTAT_RedAlarmStpVoice:
//			voiceAddr[len++] = VIC_Alarm;
//			voiceAddr[len++] = VIC_Off;
//			break;
//
//		case SOUNDSTAT_GeHuVoice:
//			voiceAddr[len++] = VIC_Private_Call;
//			break;
//
//		case SOUNDSTAT_GeHuEndVoice:
//			voiceAddr[len++] = VIC_Private_Call;
//			voiceAddr[len++] = VIC_Off;
//			break;
//
//		case SOUNDSTAT_ZuHuVoice:
//			voiceAddr[len++] = VIC_Group_Call;
//			break;
//
//		case SOUNDSTAT_ZuHuEndVoice:
//			voiceAddr[len++] = VIC_Group_Call;
//			voiceAddr[len++] = VIC_Off;
//			break;
//
//		case SOUNDSTAT_QuanHuVoice:
//			voiceAddr[len++] = VIC_General_Call;
//			break;
//
//		case SOUNDSTAT_TxTimeOutVoice:
//			voiceAddr[len++] = VIC_Call;
//			voiceAddr[len++] = VIC_Time_out;
//			break;
//
//		case SOUNDSTAT_BsTxTimeOutVoice:/*中继激活超时*/
//			voiceAddr[len++] = VIC_Repeater;
//			voiceAddr[len++] = VIC_Revive;
//			voiceAddr[len++] = VIC_Time_out;
//			break;
//
//		case SOUNDSTAT_EncrptOpenVoice:/*加密开*/
//			voiceAddr[len++] = VIC_Encryption;
//			voiceAddr[len++] = VIC_On;
//			break;
//
//		case SOUNDSTAT_EncrptCloseVoice:/*加密关*/
//
//			voiceAddr[len++] = VIC_Encryption;
//			voiceAddr[len++] = VIC_Off;
//			break;

		case SOUNDSTAT_SKOpenVoice:    /*声控开*/
			voiceAddr[len++] = VIC_VOX;
			voiceAddr[len++] = VIC_On;
			break;

		case SOUNDSTAT_SKCloseVoice:   /*声控关*/
			voiceAddr[len++] = VIC_VOX;
			voiceAddr[len++] = VIC_Off;
			break;

//		case SOUNDSTAT_VolumeGradeVoice: /*音量xxx*/
//			voiceAddr[len++] = VIC_Volume;
//			voiceAddr[len++] = VOL_BLL_GetVolLevel()==0?VIC_Off:VOL_BLL_GetVolLevel();
//			break;
//		case SOUNDSTAT_VolumeGradeVoice2: /*音量xxx*/
//			voiceAddr[len++] = VOL_BLL_GetVolLevel()==0?VIC_Off:VOL_BLL_GetVolLevel();
//			break;

		case SOUNDSTAT_SQGradeVoice:    /*静噪级别选择：静噪xxx*/
			voiceAddr[len++] = VIC_Squelch;
			voiceAddr[len++] = Func_Str.sqlevel;
			if(voiceAddr[len-1]==0) voiceAddr[len-1] = VIC_On;
			break;

		case SOUNDSTAT_ScanCallInStart:
			voiceAddr[len++] = CHN_BLL_GetAttr()==CHNSTAT_DMR?VIC_Digital:VIC_Analog;
			voiceAddr[len++] = VIC_Channel;
			SOUND_BLL_VICNum(CODESW_INTF_GetStatus()+1,voiceAddr,&len);
			break;

//		case SOUNDSTAT_ScanMode:
//			voiceAddr[len++] = VIC_Scan;
//			voiceAddr[len++] = VIC_Mode;
//			voiceAddr[len++] = MENU_APP_GetMenuSetStat(BARTYPESETFLG_SCANMODE)+1;
//			break;

//		case SOUNDSTAT_UnProgrammedChnNumberVoice:
//			voiceAddr[len++] = VIC_Zone;
//			SOUND_BLL_VICNum(MENU_APP_GetMenuSetStat(BARTYPESETFLG_ZONE)+1,voiceAddr,&len);
//			SOUND_BLL_VICNum(CODESW_INTF_GetStatus()+1,voiceAddr,&len);

//			break;

//		case SOUNDSTAT_UnProgrammedChnZoneVoice:
//			voiceAddr[len++] = VIC_Zone;
//			SOUND_BLL_VICNum(MENU_APP_GetMenuSetStat(BARTYPESETFLG_ZONE)+1,voiceAddr,&len);
//			SOUND_BLL_VICNum(CODESW_INTF_GetStatus()+1,voiceAddr,&len);
//			break;

//		case SOUNDSTAT_Revive:
//			voiceAddr[len++] = VIC_Remote;
//			voiceAddr[len++] = VIC_Revive;
//			break;
//
//		case SOUNDSTAT_Kill:
//			voiceAddr[len++] = VIC_Remote;
//			voiceAddr[len++] = VIC_Kill;
//			break;
//
//		case SOUNDSTAT_Monitor:
//			voiceAddr[len++] = VIC_Remote;
//			voiceAddr[len++] = VIC_Monitor;
//			voiceAddr[len++] = VIC_On;
//			break;
//
//		case SOUNDSTAT_Detection:
//			voiceAddr[len++] = VIC_Remote;
//			voiceAddr[len++] = VIC_Detection;
//			break;

		case SOUNDSTAT_Monitor_A1:/*模拟监听1*/
			voiceAddr[len++] = VIC_Monitor;
			voiceAddr[len++] = VIC_One;
			voiceAddr[len++] = GloVarFlag.FmMontFlg?VIC_On:VIC_Off;
			break;
		case SOUNDSTAT_Monitor_A2:/*模拟监听2（永久监听）*/
			voiceAddr[len++] = VIC_Monitor;
			voiceAddr[len++] = VIC_One+1;
			voiceAddr[len++] = GloVarFlag.FmMontCFlg?VIC_On:VIC_Off;
			break;

		case SOUNDSTAT_Prompt:
			voiceAddr[len++] = VIC_Call;
			voiceAddr[len++] = VIC_Prompt;
			break;

		case SOUNDSTAT_SetOn:
			voiceAddr[len++] = VIC_On;
			break;
		case SOUNDSTAT_SetOff:
			voiceAddr[len++] = VIC_Off;
			break;
//		case SOUNDSTAT_RoamConnected:
//			voiceAddr[len++] = VIC_Repeater;
//			voiceAddr[len++] = VIC_Revive;
//			break;
		default :break;
	}
	VOICEIC_INTF_PlayTone(voiceAddr,len);
	#endif
}



//播报未编程提示音
DH_VOID SOUND_BLL_Audio_CHN_Err(DH_VOID)
{
#if(!LCD_DISPLAY_STAT)
	SOUND_BLL_SoundStartOpt(SOUNDSTAT_Voice, SOUNDSTAT_UnProgrammedChanleNumberVoice);
#endif
	SOUND_BLL_SoundStart(SOUNDSTAT_Alarm);
}


DH_VOID SOUND_BLL_StopCurTone(DH_VOID)
{
	MenuVarFlg.bSoundChangedFlg = 1;
}

void Sound_Alarm(void)
{
//	Str_KeyVoc.HndStat = KEYVOC_HND_Y;	
	SOUND_BLL_SoundStart(SOUNDSTAT_Alarm);
}

//
