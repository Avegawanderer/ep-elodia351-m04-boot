//DRV_Audio.c
#include "app_config.h"
#include "include.h"
#include "DRV_Audio.h"

#define USE_NVDS_RU

#define RDASpiLock()  
#define RDASpiUnlock()  

#define DelayXUs Delay_us
#define DelayXMs Delay_ms
#define OSTimeDly Delay_ms
#define freq_thrOpt(a,b,c,d,e,f,g,h, KEYVOICEVOLUME_MAX) 
#define	fun_freq_end(a) 


STR_AUDIO Str_Audio;/*用于报号*/
/*功能函数是用于语音报号的各种情况，写出来放在这边进行调用*/
AUDIO_MENUSETINIT Audio_MenuSetinit;

/*
*
*/
void (*PVoiceFunction[2][36])(void)={
{
0,/*0*/
	#if 0
*InterohoneOpenVoice,/*1*/
*InterohoneCloseVoice,/*2*/
*ChanleNumberVoice,/*3*/
*BSChanleNumberVoice,/*4*/
*LookInfmVoice,/*5*/
*UnProgrammedChanleNumberVoice,/*6*/
*UnProgrammedChanleZoneVoice,/*7*/
*ZoneVoice,/*8*/
*CouloStatVoice,/*9*/
*LowCouloVoice,/*10*/
*PowerVoice,/*11*/
*PCtoMCUStartVoice,/*12*/
*PCtoMCUEndVoice,/*13*/
*ScanStartVoice,/*14*/
*ScanEndVoice,/*15*/
*LockVoice,/*16*/
*UnLockVoice,/*17*/
*RedAlarmVoice,/*18*/
*RedAlarmStpVoice,/*19*/
*GeHuVoice,/*20*/
*GeHuEndVoice,/*21*/
*ZuHuEndVoice,/*22*/
*ZuHuVoice,/*23*/
*QuanHuVoice,/*24*/
*TxTimeOutVoice,/*25*/
*BsTxTimeOutVoice,/*26*/
*EncrptOpenVoice,/*27*/
*EncrptCloseVoice,/*28*/
*SKOpenVoice,/*29*/
*SKCloseVoice,/*30*/
*VolumeGrade,/*31*/
*SQGrade,/*32*/
*ScanCallInStart,/*33*/
*ScanMode,/*34*/
*NumberVoice,/*3*/
#endif
},
/////////////////////////////////////////
{
0,/*0*/
*RU_InterohoneOpenVoice,/*1*/
*RU_InterohoneCloseVoice,/*2*/
*RU_ChanleNumberVoice,/*3*/
*RU_BSChanleNumberVoice,/*4*/
*RU_LookInfmVoice,/*5*/
*audio_unprogram,//*UnProgrammedChanleNumberVoice,/*6*/
*audio_unprogram,//*UnProgrammedChanleZoneVoice,/*7*/
*RU_ZoneVoice,/*8*/
*RU_CouloStatVoice,/*9*/
*RU_LowCouloVoice,/*10*/
*RU_PowerVoice,/*11*/
*RU_PCtoMCUStartVoice,/*12*///-------------------
*RU_PCtoMCUEndVoice,/*13*///--------------------
*RU_ScanStartVoice,/*14*/
*RU_ScanEndVoice,/*15*/
*RU_LockVoice,/*16*///--------------------
*RU_UnLockVoice,/*17*///--------------------
*RU_RedAlarmVoice,/*18*/
*RU_RedAlarmStpVoice,/*19*/
*RU_GeHuVoice,/*20*/
*RU_GeHuEndVoice,/*21*/
*RU_ZuHuEndVoice,/*22*/
*RU_ZuHuVoice,/*23*/
*RU_QuanHuVoice,/*24*/
*RU_TxTimeOutVoice,/*25*/
*RU_BsTxTimeOutVoice,/*26*/
*RU_EncrptOpenVoice,/*27*/
*RU_EncrptCloseVoice,/*28*/
*RU_SKOpenVoice,/*29*/
*RU_SKCloseVoice,/*30*/
*RU_VolumeGrade,/*31*///--------------------
*RU_SQGrade,/*32*/
*RU_ScanCallInStart,/*33*/
*RU_ScanMode,/*34*/
*RU_NumberVoice,/*3*/
}
};








const uint16 ru_pcm_len[]={
  5434   ,
  7942   ,
  7942   ,
  10030  ,
  6270   ,
  7524   ,
  6688   ,
  8778   ,
  8778   ,
  10030  ,
  
  10868  ,
  11000  ,
  11286  ,
  14630  ,
  10030  ,
  13376  ,
  10030  ,
  7942   ,
  7524   ,
  15466  ,
  
  7106   ,
  9612   ,
  10868  ,
  9612   ,
  7942   ,
  13376  ,
  16302  ,
  8778   ,
  9612   ,
  11286  ,
  
  7942   ,
  7106   ,
  12540  ,
  10030  ,
  17554  ,
  11286  ,
  10868  ,
  8360   ,
  8778   ,
  16302  ,
  
  11704  ,
  18808  ,
  9612   ,
  10868  ,
  10030  ,
  15048  ,
  6688   ,
  9196   ,
  10030  ,
};

#define PCM_CAP_RATE 16 //8000 //8K
/*获取报号时间*/
uint16 RU_getVoicePlayTime(uint16 index)
{
  return ru_pcm_len[index]/PCM_CAP_RATE+1;
}
uint8 sendbytes = 0;
uint8 sendbytesCal = 0;
uint16 sendbytesTime = 0;
uint8 ManyByte[16]={0xf1,0x00,0x01,0xf4,0x0A,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0xf3,0x19};

void SendManyByte_Add (uint8 data)
{
  if(data==0xD4) return;
  else if(data==0xD5) return;//data = 0xFE ;
  else if(data>=0xD0 && data<=0xD3) data=0xE0+(data-0xD0)*2;
  else if(data>=0x94 && data < (0x94+49))	{data-=0x94;	}
//  else return;
  
   sendbytesCal += data;
   ManyByte[sendbytes++] = data;
   ManyByte[sendbytes] = 0xf3;
   ManyByte[sendbytes+1] = sendbytesCal;
   
   if(data<49) sendbytesTime += (RU_getVoicePlayTime(data));
}

void SendManyByte_Reset (void)
{
  sendbytes = 0;
  memset(ManyByte,0,sizeof(ManyByte));
  sendbytesCal = 0;
  sendbytesTime = 300;
  SendManyByte_Add(0xf1);
}


void	 SendManyByte_TwoLine (void)
{
      SendManyByte_Add(0xfe); 
        
        
	uint8 i,j,err,data;
	
        RDASpiLock();
	//VOICE_DATA_HIGH(); 
	VOICE_DATA_LOW(); 
	VOICE_CLK_LOW();
	 DelayXMs(5);
         for(j=0;j< sendbytes+2; j++)
	{
          data = ManyByte[j];
          for(i=0;i<8;i++)
          {
            VOICE_CLK_LOW();
            DelayXUs(300);

            if(data&0x01)//低位在前  SendByte&0x80(高位在前)
              VOICE_DATA_HIGH();
            else
              VOICE_DATA_LOW();

            DelayXUs(300);
            VOICE_CLK_HIGH();
            DelayXUs(600);
            data = data>>1;
          }
        }
        
        VOICE_DATA_HIGH();
        VOICE_CLK_HIGH();
        
        RDASpiUnlock();

}


void	WriteI2CM6265Audio(uint8 data)
{
	uint8 i,err;
        

#if 1//def USE_NVDS_RU
	if(data==0xD4) return;
	else if(data==0xD5) return;//data = 0xFE ;
	else if(data>=0xD0 && data<=0xD3) data=0xE0+(data-0xD0)*2;
	else if(data>=0x94 && data < (0x94+49))	{data-=0x94;	}
        else return;
        
        RDASpiLock();
	//VOICE_DATA_HIGH(); 
	VOICE_DATA_LOW(); 
	VOICE_CLK_LOW();
	 DelayXMs(5);
         
	for(i=0;i<8;i++)
	{
          VOICE_CLK_LOW();
          DelayXUs(300);

          if(data&0x01)//低位在前  SendByte&0x80(高位在前)
            VOICE_DATA_HIGH();
          else
            VOICE_DATA_LOW();

          DelayXUs(300);
          VOICE_CLK_HIGH();
          DelayXUs(600);
          data = data>>1;
	}
        
        VOICE_DATA_HIGH();
        VOICE_CLK_HIGH();
        
        RDASpiUnlock();
        
        //if(data<49) OSTimeDly (RU_getVoicePlayTime(data)+500);
        //if(data>=0x93 && data < (0x93+0x30)) OSTimeDly (RU_getVoicePlayTime(data-0x93));
        return;
#else

        
	RDASpiLock();
	VOICE_DATA_LOW(); 
	VOICE_CLK_HIGH();
	 DelayXMs(6);
	VOICE_CLK_LOW();
	for(i=0;i<8;i++)
	{
		if((data<<i)&0x80)
		{
			VOICE_DATA_HIGH();
		}
		else
		{
			VOICE_DATA_LOW();
		}
		DelayXMs(1);
		VOICE_CLK_HIGH();
		DelayXMs(1);
		VOICE_CLK_LOW();
	}
	VOICE_CLK_LOW();
	VOICE_DATA_LOW();
	RDASpiUnlock();
#endif
}
/*
报号IC测试
68 21 49 01 00 11 18 00 FF
*/
void audio_test(uint8 cnt)
{
	//while(1)
//	{
//		DRV_LoudspeakerSwitch(ENABLE);
//		WriteI2CM6265Audio(0xD4);	//	开声音和数据手册上的相反
//		OSTimeDly(10);
//		WriteI2CM6265Audio(cnt);
//		OSTimeDly(1800);
//		WriteI2CM6265Audio(0xD5);	
//		OSTimeDly(10);
//		DRV_LoudspeakerSwitch(DISABLE);
//	}
}

/*****************************************************************************
 *   函数名: DRV_AudioSetInit
 *   功能描述: 上位机选择何种声音进行初使化
 *   参数定义: 
 *	返回值:
 *   创建人：liuzhiheng 2015 /4/3
******************************************************************************/
void DRV_AudioSetInit(void)
{
	uint8 Audio_MenuSet[23]={2};
	uint8 i=0;
	  
	#if 0
	#if LCD_DEF_EN
		for(i=0;i<22;i++)
		{	
			Audio_MenuSet[i]=1;
		}
		Audio_MenuSet[9]=E2PROMReadByte(BA_VOICESELECT+9);
	#else
		E2PROMReadPage(BA_VOICESELECT,Audio_MenuSet,23);
	#endif

	Str_Audio.VoiceVolume=Audio_MenuSet[22]&KEYVOICE_BITS;
	if (Str_Audio.VoiceVolume > KEYVOICEVOLUME_MAX)
	{
		Str_Audio.VoiceVolume= KEYVOICEVOLUME_MAX;
	}
	else if (Str_Audio.VoiceVolume < KEYVOICEVOLUME_MIN)
	{
		Str_Audio.VoiceVolume= KEYVOICEVOLUME_MIN;
	}



	Audio_MenuSetinit.InterohoneOpenVoice=Audio_MenuSet[0];/*开机*/
	if(Audio_MenuSetinit.InterohoneOpenVoice>3)
	{
		Audio_MenuSetinit.InterohoneOpenVoice=2;
	}
	Audio_MenuSetinit.InterohoneCloseVoice=Audio_MenuSet[1];
	if(Audio_MenuSetinit.InterohoneCloseVoice>3)/*关机*/
	{
		Audio_MenuSetinit.InterohoneCloseVoice=2;
	}
	Audio_MenuSetinit.ChanleNumberVoice=Audio_MenuSet[2];/*信道切换*/
	if(Audio_MenuSetinit.ChanleNumberVoice>3)
	{
		Audio_MenuSetinit.ChanleNumberVoice=2;
	}
	Audio_MenuSetinit.CouloStatVoice=Audio_MenuSet[3];/*电量查询*/
	if(Audio_MenuSetinit.CouloStatVoice>3)
	{
		Audio_MenuSetinit.CouloStatVoice=2;
	}
	Audio_MenuSetinit.LowCouloVoice=Audio_MenuSet[4];/*低电量提示*/
	if(Audio_MenuSetinit.LowCouloVoice>3)
	{
		Audio_MenuSetinit.LowCouloVoice=2;
	}
	Audio_MenuSetinit.PowerVoice=Audio_MenuSet[5];/*功率设置*/
	if(Audio_MenuSetinit.PowerVoice>3)
	{
		Audio_MenuSetinit.PowerVoice=2;
	}
	Audio_MenuSetinit.SQGradeVoice=Audio_MenuSet[6];/*静噪级别的调整*/
	if(Audio_MenuSetinit.SQGradeVoice>3)
	{
		Audio_MenuSetinit.SQGradeVoice=2;
	}
	Audio_MenuSetinit.ScanStartVoice=Audio_MenuSet[7];/*扫描的开始与结束*/
	Audio_MenuSetinit.ScanEndVoice=Audio_MenuSet[7];
	if(Audio_MenuSetinit.ScanStartVoice>3)
	{
		Audio_MenuSetinit.ScanStartVoice=2;
		Audio_MenuSetinit.ScanEndVoice=2;
	}
		Audio_MenuSetinit.ScanCallInStart=2;/*扫描到信道*/
	Audio_MenuSetinit.ScanMode=2;/*扫描模式*/
	Audio_MenuSetinit.LookInfmVoice=Audio_MenuSet[8];/*查找信息*/
	if(Audio_MenuSetinit.LookInfmVoice>3)
	{
		Audio_MenuSetinit.LookInfmVoice=2;
	}
	Audio_MenuSetinit.LockVoice=Audio_MenuSet[9];/*加锁*/
	if(Audio_MenuSetinit.LockVoice>3)
	{
		Audio_MenuSetinit.LockVoice=1;
	}
	Audio_MenuSetinit.UnLockVoice=Audio_MenuSet[10];/*解锁*/
	if(Audio_MenuSetinit.UnLockVoice>3)
	{
		Audio_MenuSetinit.UnLockVoice=2;
	}
	Audio_MenuSetinit.RedAlarmVoice=Audio_MenuSet[11];/*报警*/
	if(Audio_MenuSetinit.RedAlarmVoice>3)
	{
		Audio_MenuSetinit.RedAlarmVoice=2;
	}
	Audio_MenuSetinit.GeHuVoice=Audio_MenuSet[12];/*个呼开始*/
	if(Audio_MenuSetinit.GeHuVoice>3)
	{
		Audio_MenuSetinit.GeHuVoice=2;
	}
	Audio_MenuSetinit.GeHuEndVoice=Audio_MenuSet[12];/*个呼结束*/
	if(Audio_MenuSetinit.GeHuEndVoice>3)
	{
		Audio_MenuSetinit.GeHuEndVoice=2;
	}
	Audio_MenuSetinit.ZuHuVoice=Audio_MenuSet[13];/*组呼开始*/
	if(Audio_MenuSetinit.ZuHuVoice>3)
	{
		Audio_MenuSetinit.ZuHuVoice=2;
	}
	Audio_MenuSetinit.ZuHuEndVoice=Audio_MenuSet[13];/*组呼结束*/
	if(Audio_MenuSetinit.ZuHuEndVoice>3)
	{
		Audio_MenuSetinit.ZuHuEndVoice=2;
	}
	Audio_MenuSetinit.QuanHuVoice=Audio_MenuSet[14];/*全呼开始*/
	if(Audio_MenuSetinit.QuanHuVoice>3)
	{
		Audio_MenuSetinit.QuanHuVoice=2;
	}
	Audio_MenuSetinit.TxTimeOutVoice=Audio_MenuSet[15];/*呼叫超时TOT*/
	if(Audio_MenuSetinit.TxTimeOutVoice>3)
	{
		Audio_MenuSetinit.TxTimeOutVoice=2;
	}
	Audio_MenuSetinit.BsTxTimeOutVoice=Audio_MenuSet[16];/*BS激活超时*/
	if(Audio_MenuSetinit.BsTxTimeOutVoice>3)
	{
		Audio_MenuSetinit.BsTxTimeOutVoice=2;
	}
	Audio_MenuSetinit.EncrptOpenVoice=Audio_MenuSet[17];/*加密开*/
	if(Audio_MenuSetinit.EncrptOpenVoice>3)
	{
		Audio_MenuSetinit.EncrptOpenVoice=2;
	}
	Audio_MenuSetinit.EncrptCloseVoice=Audio_MenuSet[18];/*加密关*/
	if(Audio_MenuSetinit.EncrptCloseVoice>3)
	{
		Audio_MenuSetinit.EncrptCloseVoice=2;
	}
	Audio_MenuSetinit.SKOpenVoice=Audio_MenuSet[19];/*声控开*/
	if(Audio_MenuSetinit.SKOpenVoice>3)
	{
		Audio_MenuSetinit.SKOpenVoice=2;
	}
	Audio_MenuSetinit.SKCloseVoice=Audio_MenuSet[20];/*声控关*/
	if(Audio_MenuSetinit.SKCloseVoice>3)
	{
		Audio_MenuSetinit.SKCloseVoice=2;
	}

	Audio_MenuSetinit.ZoneVoice=Audio_MenuSet[21];/*区域设置*/
	if(Audio_MenuSetinit.ZoneVoice>3)
	{
		Audio_MenuSetinit.ZoneVoice=2;
	}

	
	Audio_MenuSetinit.VolumeGradeVoice=2;/*音量等级*/


	Sem_AudioLock = OSSemCreate(1);
	Assert(Sem_AudioLock, "Sem_AudioLock_C_FAIL");
	#endif
}
/*
无屏机提示音开关
*/
void AudioConfig(void)
{
#if 0
	if (!(CodecStatBack&DECODEOPEN))
	{
		DRV_KeyAudioSwitch(ENABLE);		
	}
	HRC5000_OpenMusic(ENABLE);		
	DRV_Tim7En(999);
	HRC5000Send_cmd(HRC_R0X37, 0xc0);
	OSTimeDlyHMSM(0,0,0,50);
#endif
}
/*****************************************************************************
 *   函数名: AudioVoiceOutSelect
 *   功能描述: 语音报号芯片的输出所有情况
 *   参数定义: 
 *	返回值:
 *   创建人：liuzhiheng 2015 /4/3
******************************************************************************/
void DRV_AudioVoiceSelct(uint8 stat)
{
	uint8 cmd,err=0;
        err=err;
	switch(stat)
	{
		case SOUNDSTAT_InterohoneOpenVoice :
				switch(Audio_MenuSetinit.InterohoneOpenVoice)
				{
					case 0:break;
					case 1:/*由C6000提供的语音输出提示音*/
							AudioConfig();
							freq_thrOpt(1000,380,0,0,0,0,0,0, KEYVOICEVOLUME_MAX - Str_Audio.VoiceVolume);
							fun_freq_end(50);
							break;
					case 2:/*报号芯片的输出提示音*/
							SPK_INTF_Open();
							AudioVoiceOutSelect(stat);
							break;
					default:break;
				}
			break;
		case SOUNDSTAT_InterohoneCloseVoice:
				switch(Audio_MenuSetinit.InterohoneCloseVoice)
				{
					case 0:break;
					case 1:/*由C6000提供的语音输出提示音*/
							AudioConfig();
							freq_thrOpt(1000,380,0,0,0,0,0,0, KEYVOICEVOLUME_MAX - Str_Audio.VoiceVolume);
							fun_freq_end(50);
							break;
					case 2:/*报号芯片的输出提示音*/
							SPK_INTF_Open();
							AudioVoiceOutSelect(stat);
							break;
					default:break;
				}
			break;
		case SOUNDSTAT_NumberVoice:
				switch(Audio_MenuSetinit.ChanleNumberVoice)
				{
					case 0:break;
					case 1:/*由C6000提供的语音输出提示音*/
							AudioConfig();
							freq_thrOpt(1000,380,0,0,0,0,0,0, KEYVOICEVOLUME_MAX - Str_Audio.VoiceVolume);
							fun_freq_end(50);
							break;
					case 2:/*报号芯片的输出提示音*/
							SPK_INTF_Open();
							AudioVoiceOutSelect(stat);
							break;
					default:break;
				}

			break;
		case SOUNDSTAT_ChanleNumberVoice:
				switch(Audio_MenuSetinit.ChanleNumberVoice)
				{
					case 0:break;
					case 1:/*由C6000提供的语音输出提示音*/
							AudioConfig();
							freq_thrOpt(1000,380,0,0,0,0,0,0, KEYVOICEVOLUME_MAX - Str_Audio.VoiceVolume);
							fun_freq_end(50);
							break;
					case 2:/*报号芯片的输出提示音*/
							SPK_INTF_Open();
							AudioVoiceOutSelect(stat);
							break;
					default:break;
				}

			break;
		case SOUNDSTAT_BSChanleNumberVoice:
				switch(Audio_MenuSetinit.ChanleNumberVoice)
				{
					case 0:break;
					case 1:/*由C6000提供的语音输出提示音*/
							AudioConfig();
							freq_thrOpt(1000,380,0,0,0,0,0,0, KEYVOICEVOLUME_MAX - Str_Audio.VoiceVolume);
							fun_freq_end(50);
							break;
					case 2:/*报号芯片的输出提示音*/
							SPK_INTF_Open();
							AudioVoiceOutSelect(stat);
							break;
					default:break;
				}
			break;
		case SOUNDSTAT_LookInfmVoice:
				switch(Audio_MenuSetinit.LookInfmVoice)
				{
					case 0:break;
					case 1:/*由C6000提供的语音输出提示音*/
							AudioConfig();
							freq_thrOpt(1000,380,0,0,0,0,0,0, KEYVOICEVOLUME_MAX - Str_Audio.VoiceVolume);
							fun_freq_end(50);
							break;
					case 2:/*报号芯片的输出提示音*/
							SPK_INTF_Open();
							AudioVoiceOutSelect(stat);
							break;
					default:break;
				}

			break;

		case SOUNDSTAT_ZoneVoice:
				switch(Audio_MenuSetinit.ZoneVoice)
				{
					case 0:;break;
					case 1:/*由C6000提供的语音输出提示音*/
							AudioConfig();
							freq_thrOpt(1000,380,0,0,0,0,0,0, KEYVOICEVOLUME_MAX - Str_Audio.VoiceVolume);
							fun_freq_end(50);
							break;
					case 2:/*报号芯片的输出提示音*/
							SPK_INTF_Open();
							AudioVoiceOutSelect(stat);
							break;
					default:break;
				}

			break;
		case SOUNDSTAT_CouloStatVoice:
				switch(Audio_MenuSetinit.CouloStatVoice)
				{
					case 0:break;
					case 1:/*由C6000提供的语音输出提示音*/
						{
//							u8 i=0;
//							AudioConfig();
//							freq_thrOpt(1000,380,0,0,0,0,0,0, KEYVOICEVOLUME_MAX - Str_Audio.VoiceVolume);
//							for(i=0;i<Str_Audio.Colour;i++)
//							{
//								OSTimeDlyHMSM(0,0,0,150);
//								freq_thrOpt(1000,380,0,0,0,0,0,0, KEYVOICEVOLUME_MAX - Str_Audio.VoiceVolume);
//							}
//							fun_freq_end(50);
						}
							break;
					case 2:/*报号芯片的输出提示音*/
							SPK_INTF_Open();
							AudioVoiceOutSelect(stat);
							break;
					default:break;
				}

			break;
		case SOUNDSTAT_LowCouloVoice:
				switch(Audio_MenuSetinit.LowCouloVoice)
				{
					case 0:break;
					case 1:/*由C6000提供的语音输出提示音*/
							AudioConfig();
							//freq_thrOpt(1000,380,0,0,0,0,0,0, KEYVOICEVOLUME_MAX - Str_Audio.VoiceVolume);
							freq_thrOpt(7200,440,0,0,0,0,0,0,KEYVOICEVOLUME_MAX - Str_Audio.VoiceVolume);
							fun_freq_end(50);
							break;
					case 2:/*报号芯片的输出提示音*/
							SPK_INTF_Open();
							AudioVoiceOutSelect(stat);
							break;
					default:break;
				}

			break;
		case SOUNDSTAT_PowerVoice:
				switch(Audio_MenuSetinit.PowerVoice)
				{
					case 0:break;
					case 1:/*由C6000提供的语音输出提示音*/
//							AudioConfig();
//							freq_thrOpt(1000,380,0,0,0,0,0,0, KEYVOICEVOLUME_MAX - Str_Audio.VoiceVolume);
//							if(ChnPowerGet())
//							{
//								OSTimeDlyHMSM(0,0,0,150);
//								freq_thrOpt(1000,380,0,0,0,0,0,0, KEYVOICEVOLUME_MAX - Str_Audio.VoiceVolume);
//							}
//							fun_freq_end(50);
							break;
					case 2:/*报号芯片的输出提示音*/
							SPK_INTF_Open();
							AudioVoiceOutSelect(stat);
							break;
					default:break;
				}

			break;
		case SOUNDSTAT_PCtoMCUStartVoice:
				switch(Audio_MenuSetinit.PCtoMCUStartVoice)
				{
					case 0:break;
					case 1:/*由C6000提供的语音输出提示音*/
							AudioConfig();
							freq_thrOpt(1000,380,0,0,0,0,0,0, KEYVOICEVOLUME_MAX - Str_Audio.VoiceVolume);
							fun_freq_end(50);
							break;
					case 2:/*报号芯片的输出提示音*/
							SPK_INTF_Open();
							AudioVoiceOutSelect(stat);
							break;
					default:break;
				}

			break;
		case SOUNDSTAT_PCtoMCUEndVoice:
				switch(Audio_MenuSetinit.PCtoMCUEndVoice)
				{
					case 0:break;
					case 1:/*由C6000提供的语音输出提示音*/
							AudioConfig();
							freq_thrOpt(1000,380,0,0,0,0,0,0, KEYVOICEVOLUME_MAX - Str_Audio.VoiceVolume);
							fun_freq_end(50);
							break;
					case 2:/*报号芯片的输出提示音*/
							SPK_INTF_Open();
							AudioVoiceOutSelect(stat);
							break;
					default:break;
				}

			break;
		case SOUNDSTAT_ScanStartVoice:
				switch(Audio_MenuSetinit.ScanStartVoice)
				{
					case 0:break;
					case 1:/*由C6000提供的语音输出提示音*/
							AudioConfig();
							freq_thrOpt(1000,380,0,0,0,0,0,0, KEYVOICEVOLUME_MAX - Str_Audio.VoiceVolume);
							fun_freq_end(50);
							break;
					case 2:/*报号芯片的输出提示音*/
							SPK_INTF_Open();
							AudioVoiceOutSelect(stat);
							break;
					default:break;
				}

			break;
		case SOUNDSTAT_ScanEndVoice:
				switch(Audio_MenuSetinit.ScanEndVoice)
				{
					case 0:break;
					case 1:/*由C6000提供的语音输出提示音*/
							AudioConfig();
							freq_thrOpt(1000,380,0,0,0,0,0,0, KEYVOICEVOLUME_MAX - Str_Audio.VoiceVolume);
							fun_freq_end(50);
							break;
					case 2:/*报号芯片的输出提示音*/
							SPK_INTF_Open();
							AudioVoiceOutSelect(stat);
							break;
					default:break;
				}

			break;
		case SOUNDSTAT_LockVoice:
				switch(Audio_MenuSetinit.LockVoice)
				{
					case 0:break;
					case 1:/*由C6000提供的语音输出提示音*/
							AudioConfig();
							freq_thrOpt(1000,380,0,0,0,0,0,0, KEYVOICEVOLUME_MAX - Str_Audio.VoiceVolume);
							fun_freq_end(50);
							break;
					case 2:/*报号芯片的输出提示音*/
							SPK_INTF_Open();
							AudioVoiceOutSelect(stat);
							break;
					default:break;
				}

			break;
		case SOUNDSTAT_UnLockVoice:
				switch(Audio_MenuSetinit.UnLockVoice)
				{
					case 0:break;
					case 1:/*由C6000提供的语音输出提示音*/
							AudioConfig();
							freq_thrOpt(1000,380,0,0,0,0,0,0, KEYVOICEVOLUME_MAX - Str_Audio.VoiceVolume);
							fun_freq_end(50);
							break;
					case 2:/*报号芯片的输出提示音*/
							SPK_INTF_Open();
							AudioVoiceOutSelect(stat);
							break;
					default:break;
				}

			break;
		case SOUNDSTAT_RedAlarmVoice:
				switch(Audio_MenuSetinit.RedAlarmVoice)
				{
					case 0:break;
					case 1:/*由C6000提供的语音输出提示音*/
							AudioConfig();
							freq_thrOpt(1000,380,0,0,0,0,0,0, KEYVOICEVOLUME_MAX - Str_Audio.VoiceVolume);
							fun_freq_end(50);
							break;
					case 2:/*报号芯片的输出提示音*/
							SPK_INTF_Open();
							AudioVoiceOutSelect(stat);
							break;
					default:break;
				}

			break;
		case SOUNDSTAT_RedAlarmStpVoice:
				switch(Audio_MenuSetinit.RedAlarmVoice)
				{
					case 0:
					case 1:/*由C6000提供的语音输出提示音*/
							AudioConfig();
							freq_thrOpt(1000,380,0,0,0,0,0,0, KEYVOICEVOLUME_MAX - Str_Audio.VoiceVolume);
							fun_freq_end(50);
							break;
					case 2:/*报号芯片的输出提示音*/
							SPK_INTF_Open();
							AudioVoiceOutSelect(stat);
							break;
					default:break;
				}

			break;
		case SOUNDSTAT_GeHuVoice:
				switch(Audio_MenuSetinit.GeHuVoice)
				{
					case 0:break;
					case 1:/*由C6000提供的语音输出提示音*/
							AudioConfig();
							freq_thrOpt(1000,380,0,0,0,0,0,0, KEYVOICEVOLUME_MAX - Str_Audio.VoiceVolume);
							fun_freq_end(50);
							break;
					case 2:/*报号芯片的输出提示音*/
							SPK_INTF_Open();
							AudioVoiceOutSelect(stat);
							break;
					default:break;
				}

			break;
		case SOUNDSTAT_GeHuEndVoice:
				switch(Audio_MenuSetinit.GeHuEndVoice)
				{
					case 0:break;
					case 1:/*由C6000提供的语音输出提示音*/
							AudioConfig();
							freq_thrOpt(1000,380,0,0,0,0,0,0, KEYVOICEVOLUME_MAX - Str_Audio.VoiceVolume);
							fun_freq_end(50);
							break;
					case 2:/*报号芯片的输出提示音*/
							SPK_INTF_Open();
							AudioVoiceOutSelect(stat);
							break;
					default:break;
				}

			break;
			case SOUNDSTAT_ZuHuVoice:
				switch(Audio_MenuSetinit.ZuHuVoice)
				{
					case 0:break;
					case 1:/*由C6000提供的语音输出提示音*/
							AudioConfig();
							freq_thrOpt(1000,380,0,0,0,0,0,0, KEYVOICEVOLUME_MAX - Str_Audio.VoiceVolume);
							fun_freq_end(50);
							break;
					case 2:/*报号芯片的输出提示音*/
							SPK_INTF_Open();
							AudioVoiceOutSelect(stat);
							break;
					default:break;
				}

			break;

		case SOUNDSTAT_ZuHuEndVoice:
				switch(Audio_MenuSetinit.ZuHuEndVoice)
				{
					case 0:break;
					case 1:/*由C6000提供的语音输出提示音*/
							AudioConfig();
							freq_thrOpt(1000,380,0,0,0,0,0,0, KEYVOICEVOLUME_MAX - Str_Audio.VoiceVolume);
							fun_freq_end(50);
							break;
					case 2:/*报号芯片的输出提示音*/
							SPK_INTF_Open();
							AudioVoiceOutSelect(stat);
							break;
					default:break;
				}

			break;
		case SOUNDSTAT_QuanHuVoice:
				switch(Audio_MenuSetinit.QuanHuVoice)
				{
					case 0:break;
					case 1:/*由C6000提供的语音输出提示音*/
							AudioConfig();
							freq_thrOpt(1000,380,0,0,0,0,0,0, KEYVOICEVOLUME_MAX - Str_Audio.VoiceVolume);
							fun_freq_end(50);
							break;
					case 2:/*报号芯片的输出提示音*/
							SPK_INTF_Open();
							AudioVoiceOutSelect(stat);
							break;
					default:break;
				}

			break;
		case SOUNDSTAT_TxTimeOutVoice:
				switch(Audio_MenuSetinit.TxTimeOutVoice)
				{
					case 0:break;
					case 1:/*由C6000提供的语音输出提示音*/
							AudioConfig();
							freq_thrOpt(1000,380,0,0,0,0,0,0, KEYVOICEVOLUME_MAX - Str_Audio.VoiceVolume);
							fun_freq_end(50);
							break;
					case 2:/*报号芯片的输出提示音*/
							SPK_INTF_Open();
							AudioVoiceOutSelect(stat);
							break;
					default:break;
				}

			break;
		case SOUNDSTAT_BsTxTimeOutVoice:
				switch(Audio_MenuSetinit.BsTxTimeOutVoice)
				{
					case 0:break;
					case 1:/*由C6000提供的语音输出提示音*/
							AudioConfig();
							freq_thrOpt(1000,380,0,0,0,0,0,0, KEYVOICEVOLUME_MAX - Str_Audio.VoiceVolume);
							fun_freq_end(50);
							break;
					case 2:/*报号芯片的输出提示音*/
							SPK_INTF_Open();
							AudioVoiceOutSelect(stat);
							break;
					default:break;
				}

			break;
		case SOUNDSTAT_EncrptOpenVoice:
				switch(Audio_MenuSetinit.EncrptOpenVoice)
				{
					case 0:break;
					case 1:/*由C6000提供的语音输出提示音*/
							AudioConfig();
							freq_thrOpt(1000,380,0,0,0,0,0,0, KEYVOICEVOLUME_MAX - Str_Audio.VoiceVolume);
							fun_freq_end(50);
							break;
					case 2:/*报号芯片的输出提示音*/
							SPK_INTF_Open();
							AudioVoiceOutSelect(stat);
							break;
					default:break;
				}

			break;
		case SOUNDSTAT_EncrptCloseVoice:
				switch(Audio_MenuSetinit.EncrptCloseVoice)
				{
					case 0:break;
					case 1:/*由C6000提供的语音输出提示音*/
							AudioConfig();
							freq_thrOpt(1000,380,0,0,0,0,0,0, KEYVOICEVOLUME_MAX - Str_Audio.VoiceVolume);
							fun_freq_end(50);
							break;
					case 2:/*报号芯片的输出提示音*/
							SPK_INTF_Open();
							AudioVoiceOutSelect(stat);
							break;
					default:break;
				}

			break;
		case SOUNDSTAT_SKOpenVoice:
				switch(Audio_MenuSetinit.SKOpenVoice)
				{
					case 0:break;
					case 1:/*由C6000提供的语音输出提示音*/
							AudioConfig();
							freq_thrOpt(1000,380,0,0,0,0,0,0, KEYVOICEVOLUME_MAX - Str_Audio.VoiceVolume);
							fun_freq_end(50);
							break;
					case 2:/*报号芯片的输出提示音*/
							SPK_INTF_Open();
							AudioVoiceOutSelect(stat);
							break;
					default:break;
				}

			break;
		case SOUNDSTAT_SKCloseVoice:
				switch(Audio_MenuSetinit.SKCloseVoice)
				{
					case 0:break;
					case 1:/*由C6000提供的语音输出提示音*/
							AudioConfig();
							freq_thrOpt(1000,380,0,0,0,0,0,0, KEYVOICEVOLUME_MAX - Str_Audio.VoiceVolume);
							fun_freq_end(50);
							break;
					case 2:/*报号芯片的输出提示音*/
							SPK_INTF_Open();
							AudioVoiceOutSelect(stat);
							break;
					default:break;
				}

			break;
		case SOUNDSTAT_VolumeGradeVoice:
				switch(Audio_MenuSetinit.VolumeGradeVoice)
				{
					case 0:break;
					case 1:/*由C6000提供的语音输出提示音*/
							AudioConfig();
							freq_thrOpt(1000,380,0,0,0,0,0,0, KEYVOICEVOLUME_MAX - Str_Audio.VoiceVolume);
							fun_freq_end(50);
							break;
					case 2:/*报号芯片的输出提示音*/
							SPK_INTF_Open();
							AudioVoiceOutSelect(stat);
							break;
					default:break;
				}

			break;
		case SOUNDSTAT_SQGradeVoice:
				switch(Audio_MenuSetinit.SQGradeVoice)
				{
					case 0:break;
					case 1:/*由C6000提供的语音输出提示音*/
							AudioConfig();
							freq_thrOpt(1000,380,0,0,0,0,0,0, KEYVOICEVOLUME_MAX - Str_Audio.VoiceVolume);
							fun_freq_end(50);
							break;
					case 2:/*报号芯片的输出提示音*/
							SPK_INTF_Open();
							AudioVoiceOutSelect(stat);
							break;
					default:break;
				}

			break;
		case SOUNDSTAT_ScanCallInStart:
				switch(Audio_MenuSetinit.ScanCallInStart)
				{
					case 0:break;
					case 1:/*由C6000提供的语音输出提示音*/
							AudioConfig();
							freq_thrOpt(1000,380,0,0,0,0,0,0, KEYVOICEVOLUME_MAX - Str_Audio.VoiceVolume);
							fun_freq_end(50);
							break;
					case 2:/*报号芯片的输出提示音*/
							SPK_INTF_Open();
							AudioVoiceOutSelect(stat);
							break;
					default:break;
				}

			break;
		case SOUNDSTAT_ScanMode:
				switch(Audio_MenuSetinit.ScanMode)
				{
					case 0:break;
					case 1:/*由C6000提供的语音输出提示音*/
							AudioConfig();
							freq_thrOpt(1000,380,0,0,0,0,0,0, KEYVOICEVOLUME_MAX - Str_Audio.VoiceVolume);
							fun_freq_end(50);
							break;
					case 2:/*报号芯片的输出提示音*/
							SPK_INTF_Open();
							AudioVoiceOutSelect(stat);
							break;
					default:break;
				}

			break;
		case SOUNDSTAT_UnProgrammedChanleNumberVoice:
			SPK_INTF_Open();
			AudioVoiceOutSelect(stat);
			break;
		case SOUNDSTAT_UnProgrammedChanleZoneVoice:
			SPK_INTF_Open();
			AudioVoiceOutSelect(stat);
			break;
		default :break;
	}
}
//
void AudioVoiceOutSelect(uint8 stat)
{
	#if (LANGUAGE_SEL==LANGUAGE_RU)  
          #ifdef USE_NVDS_RU      
            if(stat == SOUNDSTAT_InterohoneOpenVoice)
                (*PVoiceFunction[1][stat])();
            else
            {
                SendManyByte_Reset();
                (*PVoiceFunction[1][stat])();/////1
                SendManyByte_TwoLine();
                OSTimeDly (sendbytesTime+100);
              //  OSTimeDly (sendbytesTime>1000?sendbytesTime-1000:sendbytesTime);
            }
          #else
                (*PVoiceFunction[1][stat])();/////1
          #endif
	#else
		(*PVoiceFunction[0][stat])();
	#endif
	 
#if 0
	switch(stat)
	{
		case SOUNDSTAT_InterohoneOpenVoice :
			InterohoneOpenVoice();
			break;
		case SOUNDSTAT_InterohoneCloseVoice:
			InterohoneCloseVoice();
			break;
		case SOUNDSTAT_ChanleNumberVoice:
			ChanleNumberVoice(Str_Audio.Attr,Str_Audio.Ccnt,Str_Audio.Zone);
			break;
		case SOUNDSTAT_BSChanleNumberVoice:
			BSChanleNumberVoice(Str_Audio.Attr,Str_Audio.Ccnt,Str_Audio.Zone);
			break;
		case SOUNDSTAT_LookInfmVoice:
			LookInfmVoice(Str_Audio.Attr,Str_Audio.Ccnt,Str_Audio.Zone,Str_Audio.BsModeSelect,Str_Audio.LMAttr);
			break;
		case SOUNDSTAT_UnProgrammedChanleNumberVoice:
			UnProgrammedChanleNumberVoice(Str_Audio.Ccnt,Str_Audio.Zone);
			break;
		case SOUNDSTAT_UnProgrammedChanleZoneVoice:
			UnProgrammedChanleZoneVoice(Str_Audio.Ccnt,Str_Audio.Zone);
			break;
		case SOUNDSTAT_ZoneVoice:
			ZoneVoice(Str_Audio.Zone,Str_Audio.Attr,Str_Audio.Ccnt,Str_Audio.BsModeSelect);
			break;
		case SOUNDSTAT_CouloStatVoice:
			CouloStatVoice(Str_Audio.Colour);
			break;
		case SOUNDSTAT_LowCouloVoice:
			LowCouloVoice();
			break;
		case SOUNDSTAT_PowerVoice:
			PowerVoice(ChnPowerGet());
			break;
		case SOUNDSTAT_PCtoMCUStartVoice:
			PCtoMCUStartVoice();
			break;
		case SOUNDSTAT_PCtoMCUEndVoice:
			PCtoMCUEndVoice();
			break;
		case SOUNDSTAT_ScanStartVoice:
			ScanStartVoice();
			break;
		case SOUNDSTAT_ScanEndVoice:
			ScanEndVoice();
			break;
		case SOUNDSTAT_LockVoice:
			LockVoice();
			break;
		case SOUNDSTAT_UnLockVoice:
			UnLockVoice();
			break;
		case SOUNDSTAT_RedAlarmVoice:
			RedAlarmVoice();
			break;
		case SOUNDSTAT_RedAlarmStpVoice:
			RedAlarmStpVoice();
			break;
		case SOUNDSTAT_GeHuVoice:
			GeHuVoice();
			break;
		case SOUNDSTAT_GeHuEndVoice:
			GeHuEndVoice();
			break;
		case SOUNDSTAT_ZuHuEndVoice:
			ZuHuEndVoice();
			break;

		case SOUNDSTAT_ZuHuVoice:
			ZuHuVoice();
			break;
		case SOUNDSTAT_QuanHuVoice:
			QuanHuVoice();
			break;
		case SOUNDSTAT_TxTimeOutVoice:
			TxTimeOutVoice();
			break;
		case SOUNDSTAT_BsTxTimeOutVoice:
			BsTxTimeOutVoice();
			break;
		case SOUNDSTAT_EncrptOpenVoice:
			EncrptOpenVoice();
			break;
		case SOUNDSTAT_EncrptCloseVoice:
			EncrptCloseVoice();
			break;
		case SOUNDSTAT_SKOpenVoice:
			SKOpenVoice();
			break;
		case SOUNDSTAT_SKCloseVoice:
			SKCloseVoice();
			break;
		case SOUNDSTAT_VolumeGradeVoice:
			VolumeGrade(vol);
			break;
		case SOUNDSTAT_SQGradeVoice:
			SQGrade(GetMenuSetStat(BARTYPESETFLG_Squelch));
			break;
		case SOUNDSTAT_ScanCallInStart:
			ScanCallInStart(Str_Audio.Attr,Str_Audio.Ccnt);
			break;
		case SOUNDSTAT_ScanMode:
			ScanMode(StrMMenuSet.ScanMode+1);
			break;
		default :break;
	}
#endif
}

#if 0
/*开机声音*/
void InterohoneOpenVoice(void)
{
/*要开启芯片的声音*/
	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);	//	开声音和数据手册上的相反
	OSTimeDly(10);
	WriteI2CM6265Audio(AUDIO_Welcome);	//	Welcome
	OSTimeDly(650);
	WriteI2CM6265Audio(AUDIO_DMR);//DMR
	OSTimeDly(900);
	WriteI2CM6265Audio(AUDIO_Product);//Preduct
	OSTimeDly(900);
	WriteI2CM6265Audio(0xD5);	//	关声音
	OSTimeDly(10);//需要延时才有效
	AudioWriteUnlock();
/*结束后设静音*/
}


/*关机的声音*/
void InterohoneCloseVoice(void)
{
/*要开启芯片的声音*/
	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD2);	//声音开为中等
	DelayXMs(10);
	WriteI2CM6265Audio(0xD4);	//	开声音和数据手册上的相反
	DelayXMs(10);
	WriteI2CM6265Audio(AUDIO_goodbye);
	DelayXMs(600);
	WriteI2CM6265Audio(0xD5);	//	关声音
	DelayXMs(10);//需要延时才有效
	AudioWriteUnlock();
/*结束后设静音*/
}
/*频道报号*/
void NumberVoice(void)
{
  //(Str_Audio.Attr,Str_Audio.Ccnt,Str_Audio.Zone);
/*要开启芯片的声音*/
	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);
	OSTimeDly(10);
	#if (LANGUAGE_SEL==LANGUAGE_RU)
	WriteI2CM6265Audio(0xaf+Str_Audio.number);
	#else
	WriteI2CM6265Audio(Str_Audio.number);
	#endif
	OSTimeDly(900);
	WriteI2CM6265Audio(0xD5);
	OSTimeDly(10);
	AudioWriteUnlock();
/*结束后设静音*/
}
/*频道报号*/
void ChanleNumberVoice(void)
{
  //(Str_Audio.Attr,Str_Audio.Ccnt,Str_Audio.Zone);
/*要开启芯片的声音*/
	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);
	OSTimeDly(10);
	#if RX_ONLY_EN
	#else
	//WriteI2CM6265Audio(Str_Audio.Zone);
	//OSTimeDly(900);
	switch(Str_Audio.Attr)
		{
		case 1:	
			WriteI2CM6265Audio(AUDIO_Analog);
			OSTimeDly(900);
			break;
		case 2:
			WriteI2CM6265Audio(AUDIO_Digital);
			OSTimeDly(900);
			break;
		default :
			break;
		}
	//WriteI2CM6265Audio(AUDIO_Mode);
	//OSTimeDly(1000);
	WriteI2CM6265Audio(AUDIO_Channel);
	OSTimeDly(750);
	#endif
	#if (LANGUAGE_SEL==LANGUAGE_RU)
	WriteI2CM6265Audio(0xaf+Str_Audio.Ccnt);
	#else
	WriteI2CM6265Audio(Str_Audio.Ccnt);
	#endif
	OSTimeDly(900);
	WriteI2CM6265Audio(0xD5);
	OSTimeDly(10);
	AudioWriteUnlock();
/*结束后设静音*/
}
/*中继频道报号*/
void BSChanleNumberVoice(void)
{
//(Str_Audio.Attr,Str_Audio.Ccnt,Str_Audio.Zone)
/*要开启芯片的声音*/
	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);
	OSTimeDly(10);
	//WriteI2CM6265Audio(Str_Audio.Zone);
	//OSTimeDly(900);

	switch(Str_Audio.Attr)
		{
		case 1:	
			WriteI2CM6265Audio(AUDIO_Analog);
			OSTimeDly(900);
			break;
		case 2:
			WriteI2CM6265Audio(AUDIO_Digital);
			OSTimeDly(900);
			break;
		default :
			break;
		}
	//WriteI2CM6265Audio(AUDIO_Repeater);
	//OSTimeDly(1000);
	//WriteI2CM6265Audio(AUDIO_Mode);
	//OSTimeDly(1000);
	WriteI2CM6265Audio(AUDIO_Channel);
	OSTimeDly(750);
	#if (LANGUAGE_SEL==LANGUAGE_RU)
	WriteI2CM6265Audio(0xaf+Str_Audio.Ccnt);
	#else
	WriteI2CM6265Audio(Str_Audio.Ccnt);
	#endif

	OSTimeDly(900);
	WriteI2CM6265Audio(0xD5);
	OSTimeDly(10);
	AudioWriteUnlock();
/*结束后设静音*/
}
/*频道报号*/
void LookInfmVoice(void)
{
//(Str_Audio.Attr,Str_Audio.Ccnt,Str_Audio.Zone,Str_Audio.BsModeSelect,Str_Audio.LMAttr);
/*要开启芯片的声音*/
	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);
	OSTimeDly(10);
	WriteI2CM6265Audio(Str_Audio.Zone);
	OSTimeDly(900);

	switch(Str_Audio.Attr)
		{
		case 1:	
			WriteI2CM6265Audio(AUDIO_Analog);
			OSTimeDly(900);
			break;
		case 2:
			WriteI2CM6265Audio(AUDIO_Digital);
			OSTimeDly(900);
			break;
		default :
			break;
		}
	if(Str_Audio.BsModeSelect)
	{
		WriteI2CM6265Audio(AUDIO_Repeater);
		OSTimeDly(1000);
	}
	WriteI2CM6265Audio(AUDIO_Mode);
	OSTimeDly(1000);

	WriteI2CM6265Audio(AUDIO_Channel);
	OSTimeDly(750);

	#if (LANGUAGE_SEL==LANGUAGE_RU)
	WriteI2CM6265Audio(0xaf+Str_Audio.Ccnt);
	#else
	WriteI2CM6265Audio(Str_Audio.Ccnt);
	#endif
	OSTimeDly(750);
	if(Str_Audio.Attr==2)
	{
		switch(Str_Audio.LMAttr)
			{
			case HRC_DPATCS:
				WriteI2CM6265Audio(AUDIO_PrivateCall);
				OSTimeDly(1100);
				break;
			case HRC_DGROUP:
				WriteI2CM6265Audio(AUDIO_GroupCall);
				OSTimeDly(1100);
				break;
			default :	
				WriteI2CM6265Audio(AUDIO_GeneralCall);
				OSTimeDly(1100);
				break;
			}
	}
	WriteI2CM6265Audio(0xD5);
	OSTimeDly(10);
	AudioWriteUnlock();
/*结束后设静音*/
}

/**未编程的提示**/
void UnProgrammedChanleNumberVoice(void)
{
//(Str_Audio.Ccnt,Str_Audio.Zone);
	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);
	OSTimeDly(10);
	WriteI2CM6265Audio(Str_Audio.Zone);
	OSTimeDly(750);
	WriteI2CM6265Audio(AUDIO_Channel);
	OSTimeDly(850);

	#if (LANGUAGE_SEL==LANGUAGE_RU)
	WriteI2CM6265Audio(0xaf+Str_Audio.Ccnt);
	#else
	WriteI2CM6265Audio(Str_Audio.Ccnt);
	#endif
	OSTimeDly(950);
	WriteI2CM6265Audio(AUDIO_FREQ);
	OSTimeDly(950);
	WriteI2CM6265Audio(AUDIO_One);
	OSTimeDly(800);
	WriteI2CM6265Audio(AUDIO_Prompt);
	OSTimeDly(800);
	WriteI2CM6265Audio(AUDIO_Remote);
	OSTimeDly(950);
	WriteI2CM6265Audio(0xD5);
	OSTimeDly(10);
	AudioWriteUnlock();
/*结束后设静音*/
}
/**未编程的提示只报区域和信道**/
void UnProgrammedChanleZoneVoice(void)
{
//(Str_Audio.Ccnt,Str_Audio.Zone);
	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);
	OSTimeDly(10);
	WriteI2CM6265Audio(Str_Audio.Zone);
	OSTimeDly(900);
	WriteI2CM6265Audio(AUDIO_Channel);
	OSTimeDly(850);

	#if (LANGUAGE_SEL==LANGUAGE_RU)
	WriteI2CM6265Audio(0xaf+Str_Audio.Ccnt);
	#else
	WriteI2CM6265Audio(Str_Audio.Ccnt);
	#endif
	OSTimeDly(900);
	WriteI2CM6265Audio(0xD5);
	OSTimeDly(10);
	AudioWriteUnlock();
/*结束后设静音*/
}
/*区域报号*/
void ZoneVoice(void)
{
//(Str_Audio.Zone,Str_Audio.Attr,Str_Audio.Ccnt,Str_Audio.BsModeSelect);
/*要开启芯片的声音*/
	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);
	OSTimeDly(10);
	WriteI2CM6265Audio(Str_Audio.Zone);
	OSTimeDly(900);
	switch(Str_Audio.Attr)
		{
		case 1:	
			WriteI2CM6265Audio(AUDIO_Analog);
			OSTimeDly(900);
			break;
		case 2:
			WriteI2CM6265Audio(AUDIO_Digital);
			OSTimeDly(900);
			break;
		default :
			break;
		}
	if(Str_Audio.BsModeSelect)
	{
		WriteI2CM6265Audio(AUDIO_Repeater);
		OSTimeDly(1000);
	}

	WriteI2CM6265Audio(AUDIO_Mode);
	OSTimeDly(1000);
	WriteI2CM6265Audio(AUDIO_Channel);
	OSTimeDly(750);

	#if (LANGUAGE_SEL==LANGUAGE_RU)
	WriteI2CM6265Audio(0xaf+Str_Audio.Ccnt);
	#else
	WriteI2CM6265Audio(Str_Audio.Ccnt);
	#endif
	OSTimeDly(900);
	WriteI2CM6265Audio(0xD5);	//	关声音
	OSTimeDly(10);//需要延时才有效
	AudioWriteUnlock();

}

/*电量查询高中低*/
void CouloStatVoice(void)
{
//Str_Audio.Colour
/*要开启芯片的声音*/
	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);
	OSTimeDly(10);
	WriteI2CM6265Audio(0x56);
	OSTimeDly(900);
	WriteI2CM6265Audio(0x57);
	OSTimeDly(800);
	switch(Str_Audio.Colour)
		{
		case 0:
				WriteI2CM6265Audio(0x35);
				OSTimeDly(750);
				break;
		case 1:
				WriteI2CM6265Audio(0x34);
				OSTimeDly(750);
				break;
		case 2:
				WriteI2CM6265Audio(0x33);
				OSTimeDly(750);
				break;
		default :  break;
				
		}
	WriteI2CM6265Audio(0xD5);	//	关声音
	OSTimeDly(10);//需要延时才有效
	AudioWriteUnlock();
}

/*低电量提示*/
void LowCouloVoice(void)
{
/*要开启芯片的声音*/
	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);
	OSTimeDly(10);
	WriteI2CM6265Audio(0x56);
	OSTimeDly(900);
	WriteI2CM6265Audio(0x57);
	OSTimeDly(800);
	WriteI2CM6265Audio(0x35);
	OSTimeDly(800);
	WriteI2CM6265Audio(0x5a);
	OSTimeDly(1000);

	WriteI2CM6265Audio(0xD5);	//	关声音
	OSTimeDly(10);//需要延时才有效
	AudioWriteUnlock();
}

/*选择输出的功率*/
void PowerVoice(void)
{
//ChnPowerGet()
/*要开启芯片的声音*/
	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);	//	开声音和数据手册上的相反
	OSTimeDly(10);
	WriteI2CM6265Audio(0x52);
	OSTimeDly(750);
	switch(ChnPowerGet())//只有低和高功率
		{
		case 0:
				WriteI2CM6265Audio(0x35);//LOW
				OSTimeDly(750);
				break;
		case 1:
				WriteI2CM6265Audio(0x33);//HIGH
				OSTimeDly(750);
				break;
		default :  break;
				
		}

	WriteI2CM6265Audio(0xD5);	//	关声音
	OSTimeDly(10);//需要延时才有效
	AudioWriteUnlock();
}
/*开始写频*/
void PCtoMCUStartVoice(void)
{
/*要开启芯片的声音*/
	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);	//	开声音和数据手册上的相反
	OSTimeDly(10);
	WriteI2CM6265Audio(0x3b);
	OSTimeDly(1000);
	WriteI2CM6265Audio(0x3c);
	OSTimeDly(1000);

	WriteI2CM6265Audio(0xD5);	//	关声音
	OSTimeDly(10);//需要延时才有效
	AudioWriteUnlock();
}
/*结束写频*/
void PCtoMCUEndVoice(void)
{
/*要开启芯片的声音*/
	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);	//	开声音和数据手册上的相反
	OSTimeDly(10);
	WriteI2CM6265Audio(0x3b);
	OSTimeDly(1000);
	WriteI2CM6265Audio(0x3D);
	OSTimeDly(1000);

	WriteI2CM6265Audio(0xD5);	//	关声音
	OSTimeDly(10);//需要延时才有效
	AudioWriteUnlock();
}
/*扫描启动*/
void ScanStartVoice(void)
{
/*要开启芯片的声音*/
	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);	//	开声音和数据手册上的相反
	OSTimeDly(10);
	WriteI2CM6265Audio(0x54);
	OSTimeDly(1000);
	WriteI2CM6265Audio(0x63);
	OSTimeDly(1000);

	WriteI2CM6265Audio(0xD5);	//	关声音
	OSTimeDly(10);//需要延时才有效
	AudioWriteUnlock();
}
/*退出扫描*/
void ScanEndVoice(void)
{
/*要开启芯片的声音*/
	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);	//	开声音和数据手册上的相反
	OSTimeDly(10);
	WriteI2CM6265Audio(0x65);
	OSTimeDly(900);
	WriteI2CM6265Audio(0x54);
	OSTimeDly(900);

	WriteI2CM6265Audio(0xD5);	//	关声音
	OSTimeDly(10);//需要延时才有效
	AudioWriteUnlock();
}
/*加锁*/
void LockVoice(void)
{
/*要开启芯片的声音*/
	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);	//	开声音和数据手册上的相反
	OSTimeDly(10);
	WriteI2CM6265Audio(0x5d);
	OSTimeDly(800);

	WriteI2CM6265Audio(0xD5);	//	关声音
	OSTimeDly(10);//需要延时才有效
	AudioWriteUnlock();
}
/*解锁*/
void UnLockVoice(void)
{
/*要开启芯片的声音*/
	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);	//	开声音和数据手册上的相反
	OSTimeDly(10);
	WriteI2CM6265Audio(0x5e);
	OSTimeDly(1200);

	WriteI2CM6265Audio(0xD5);	//	关声音
	OSTimeDly(10);//需要延时才有效
	AudioWriteUnlock();
}
/*报警*/
void RedAlarmVoice(void)
{
/*要开启芯片的声音*/
	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);	//	开声音和数据手册上的相反
	OSTimeDly(10);
	WriteI2CM6265Audio(0x5f);
	OSTimeDly(750);
	WriteI2CM6265Audio(0x4a);
	OSTimeDly(700);
	WriteI2CM6265Audio(0xD5);	//	关声音
	OSTimeDly(10);//需要延时才有效
	AudioWriteUnlock();
/*结束后设静音*/
}
/*报警结束音*/
void RedAlarmStpVoice(void)
{
/*要开启芯片的声音*/
	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);	//	开声音和数据手册上的相反
	OSTimeDly(10);
	WriteI2CM6265Audio(0x5f);
	OSTimeDly(750);
	WriteI2CM6265Audio(0x49);
	OSTimeDly(800);
	WriteI2CM6265Audio(0xD5);	//	关声音
	OSTimeDly(10);//需要延时才有效
	AudioWriteUnlock();
/*结束后设静音*/
}
/*个呼*/
void GeHuVoice(void)
{
/*要开启芯片的声音*/
	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);	//	开声音和数据手册上的相反
	OSTimeDly(10);
	WriteI2CM6265Audio(0x60);
	OSTimeDly(1200);
	WriteI2CM6265Audio(0xD5);	//	关声音
	OSTimeDly(10);//需要延时才有效
	AudioWriteUnlock();
/*结束后设静音*/
}
/*个呼*/
void GeHuEndVoice(void)
{
/*要开启芯片的声音*/
	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);	//	开声音和数据手册上的相反
	OSTimeDly(10);
	WriteI2CM6265Audio(0x60);
	OSTimeDly(1200);
	WriteI2CM6265Audio(0x49);
	OSTimeDly(900);
	WriteI2CM6265Audio(0xD5);	//	关声音
	OSTimeDly(10);//需要延时才有效
	AudioWriteUnlock();
/*结束后设静音*/
}

/*组呼*/
void ZuHuVoice(void)
{
/*要开启芯片的声音*/
	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);	//	开声音和数据手册上的相反
	OSTimeDly(10);
	WriteI2CM6265Audio(0x61);
	OSTimeDly(1200);
	WriteI2CM6265Audio(0xD5);	//	关声音
	OSTimeDly(10);//需要延时才有效
	AudioWriteUnlock();
/*结束后设静音*/
}
/*组呼*/
void ZuHuEndVoice(void)
{
/*要开启芯片的声音*/
	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);	//	开声音和数据手册上的相反
	OSTimeDly(10);
	WriteI2CM6265Audio(0x61);
	OSTimeDly(1200);
	WriteI2CM6265Audio(0x49);
	OSTimeDly(900);
	WriteI2CM6265Audio(0xD5);	//	关声音
	OSTimeDly(10);//需要延时才有效
	AudioWriteUnlock();
/*结束后设静音*/
}

/*全呼*/
void QuanHuVoice(void)
{
/*要开启芯片的声音*/
	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);	//	开声音和数据手册上的相反
	OSTimeDly(10);
	WriteI2CM6265Audio(0x62);
	OSTimeDly(1200);
	WriteI2CM6265Audio(0xD5);	//	关声音
	OSTimeDly(10);//需要延时才有效
	AudioWriteUnlock();
/*结束后设静音*/
}
/*发射超时限发*/
void TxTimeOutVoice(void)
{
/*要开启芯片的声音*/
	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);	//	开声音和数据手册上的相反
	OSTimeDly(10);
	WriteI2CM6265Audio(0x5b);
	OSTimeDly(1000);
	WriteI2CM6265Audio(0x5c);
	OSTimeDly(1000);
	WriteI2CM6265Audio(0xD5);	//	关声音
	OSTimeDly(10);//需要延时才有效
	AudioWriteUnlock();
/*结束后设静音*/
}
/*中继激活后发射超时限发*/
void BsTxTimeOutVoice(void)
{
/*要开启芯片的声音*/
	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);	//	开声音和数据手册上的相反
	OSTimeDly(10);
	WriteI2CM6265Audio(AUDIO_Repeater);
	OSTimeDly(1000);
	WriteI2CM6265Audio(AUDIO_Mode);
	OSTimeDly(1000);
	WriteI2CM6265Audio(0x5b);
	OSTimeDly(1000);
	WriteI2CM6265Audio(0x5c);
	OSTimeDly(1000);
	WriteI2CM6265Audio(0xD5);	//	关声音
	OSTimeDly(10);//需要延时才有效
//	BSOverTime=0;
	AudioWriteUnlock();
/*结束后设静音*/
}
/*加密开*/
void EncrptOpenVoice(void)
{
/*要开启芯片的声音*/
	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);	//	开声音和数据手册上的相反
	OSTimeDly(10);
	WriteI2CM6265Audio(0x55);
	OSTimeDly(950);
	WriteI2CM6265Audio(0x4a);
	OSTimeDly(850);
	WriteI2CM6265Audio(0xD5);	//	关声音
	OSTimeDly(10);//需要延时才有效
	AudioWriteUnlock();
/*结束后设静音*/
}
/*加密关*/
void EncrptCloseVoice(void)
{
/*要开启芯片的声音*/
	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);	//	开声音和数据手册上的相反
	OSTimeDly(10);
	WriteI2CM6265Audio(0x55);
	OSTimeDly(950);
	WriteI2CM6265Audio(0x49);
	OSTimeDly(900);
	WriteI2CM6265Audio(0xD5);	//	关声音
	OSTimeDly(10);//需要延时才有效
	AudioWriteUnlock();
/*结束后设静音*/
}
/*声控开*/
void SKOpenVoice(void)
{
/*要开启芯片的声音*/
	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);	//	开声音和数据手册上的相反
	OSTimeDly(10);
	WriteI2CM6265Audio(0x53);
	OSTimeDly(950);
	WriteI2CM6265Audio(0x4a);
	OSTimeDly(900);
	WriteI2CM6265Audio(0xD5);	//	关声音
	OSTimeDly(10);//需要延时才有效
	AudioWriteUnlock();
	
/*结束后设静音*/
}

/*加密关*/
void SKCloseVoice(void)
{
/*要开启芯片的声音*/
	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);	//	开声音和数据手册上的相反
	OSTimeDly(10);
	WriteI2CM6265Audio(0x53);
	OSTimeDly(950);
	WriteI2CM6265Audio(0x49);
	OSTimeDly(900);
	WriteI2CM6265Audio(0xD5);	//	关声音
	OSTimeDly(10);//需要延时才有效
	AudioWriteUnlock();
/*结束后设静音*/
}
/*
*音量大小的等级
*/
void VolumeGrade(void)
{
//vol
/*要开启芯片的声音*/
	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);	//	开声音和数据手册上的相反
	OSTimeDly(10);
	WriteI2CM6265Audio(0x4c);
	OSTimeDly(950);
	WriteI2CM6265Audio(vol);
	OSTimeDly(900);
	WriteI2CM6265Audio(0xD5);	//	关声音
	OSTimeDly(10);//需要延时才有效
	AudioWriteUnlock();
/*结束后设静音*/

}
void SQGrade(void)
{

/*要开启芯片的声音*/
	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);	//	开声音和数据手册上的相反
	OSTimeDly(10);
	WriteI2CM6265Audio(AUDIO_Squelch);
	OSTimeDly(950);
	WriteI2CM6265Audio(GetMenuSetStat(BARTYPESETFLG_Squelch));
	OSTimeDly(900);
	WriteI2CM6265Audio(0xD5);	//	关声音
	OSTimeDly(10);//需要延时才有效
	AudioWriteUnlock();
/*结束后设静音*/


}
void ScanCallInStart(void)
{
//(Str_Audio.Attr,Str_Audio.Ccnt)
/*要开启芯片的声音*/
	STR_CODESW PLLStat=GetPllStat();
	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);	//	开声音和数据手册上的相反
	OSTimeDly(10);
	switch(Str_Audio.Attr)
		{
		case 1:	
			WriteI2CM6265Audio(AUDIO_Analog);
			OSTimeDly(800);
			break;
		case 2:
			WriteI2CM6265Audio(AUDIO_Digital);
			OSTimeDly(800);
			break;
		default :
			break;
		}
	WriteI2CM6265Audio(AUDIO_Channel);
	OSTimeDly(750);

	#if (LANGUAGE_SEL==LANGUAGE_RU)
	WriteI2CM6265Audio(0xaf+Str_Audio.Ccnt);
	#else
	WriteI2CM6265Audio(Str_Audio.Ccnt);
	#endif
	OSTimeDly(750);
	WriteI2CM6265Audio(0xD5);	//	关声音
	OSTimeDly(10);//需要延时才有效
	AudioWriteUnlock();
/*结束后设静音*/


}
void ScanMode(void)
{
/*要开启芯片的声音*/
 	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);	//	开声音和数据手册上的相反
	OSTimeDly(10);
	WriteI2CM6265Audio(0x54);
	OSTimeDly(900);
	WriteI2CM6265Audio(AUDIO_Mode);
	OSTimeDly(800);
	WriteI2CM6265Audio(StrMMenuSet.ScanMode+1);
	OSTimeDly(750);
	WriteI2CM6265Audio(0xD5);	//	关声音
	OSTimeDly(10);//需要延时才有效
	AudioWriteUnlock();
/*结束后设静音*/


}
//播报未编程提示音
void audio_unprogram()
{
 // OSTimeDlyHMSM(0,0,0,80);
#if(!LCD_DEF_EN)
	SoundStartOpt(SOUNDSTAT_AudioVoice, SOUNDSTAT_UnProgrammedChanleZoneVoice);
#endif
	SoundStart(SOUNDSTAT_Alarm);

#if(FREQ_SCOPE_SET==FREQ_150_400)
//INTF_ChnUnlock();
//OSTimeDlyHMSM(0,0,1,660);
#endif
}
//
void Set_VolGrade(uint8 Stat)
{
#if(!LCD_DEF_EN)
	if(Stat<4)/*****语音报号音量by liuzhiheng*****/
		{WriteI2CM6265Audio(0xD0);}
	else if(Stat<9)
		{WriteI2CM6265Audio(0xD1);}
	else if(Stat<14)
		{WriteI2CM6265Audio(0xD2);}
	else
		{WriteI2CM6265Audio(0xD3);}
#endif
}
/*
无屏机信道切换声音
*/
void audio_ch_change(u8 opt,u8 dat)
{

#if(!LCD_DEF_EN)//如果是无屏机
	if(opt)
	{
         	if(1==Audio_MenuSetinit.ChanleNumberVoice) ;//信道音为提示音时，不报音
		 else
		if(!Str_Audio.UnProgrammed)
		{
			if(Get_ch_freq())/*中继同频选择初始化*/
			{
			Str_Audio.BsModeSelect=0;
			SoundStartOpt(SOUNDSTAT_AudioVoice,SOUNDSTAT_ChanleNumberVoice);
			}
			else
			{
			Str_Audio.BsModeSelect=1;
			SoundStartOpt(SOUNDSTAT_AudioVoice,SOUNDSTAT_BSChanleNumberVoice);
			}
		}
	}
	else
	{
		if(!Str_Audio.UnProgrammed)
		{
			if(!Str_Audio.ZoneSet)
			{
				if(Get_ch_freq())/*中继同频选择初始化*/
				{
					Str_Audio.BsModeSelect=0;
					if (CodeSwChngStat() && dat)
					{
						SoundStartOpt(SOUNDSTAT_AudioVoice,SOUNDSTAT_ChanleNumberVoice);
						Str_Audio.chg_chl_flg=1;
					}
				}
				else
				{
					Str_Audio.BsModeSelect=1;
					if (CodeSwChngStat() && dat)
					{
						SoundStartOpt(SOUNDSTAT_AudioVoice,SOUNDSTAT_BSChanleNumberVoice);
						Str_Audio.chg_chl_flg=1;
					}
				}
			}
			else
			{
				if(Get_ch_freq())/*中继同频选择初始化*/
				{
					Str_Audio.BsModeSelect=0;
				}
				else
				{
					Str_Audio.BsModeSelect=1;
				}
				if (CodeSwChngStat() && dat)
				{
					SoundStartOpt(SOUNDSTAT_AudioVoice, SOUNDSTAT_ZoneVoice);
					Str_Audio.chg_chl_flg=1;
				}
				Str_Audio.ZoneSet=0;
			}
		}	
	}
#else
	Str_Audio.chg_chl_flg=0;
#endif 

}
//呼叫指示音
void CallOutVioce(u8 type,u8 opt)
{
	uint8 SoudReg;
	
	switch(type)
	{
		case HRC_DPATCS:
		{
			if (opt)
			{
				#if(!LCD_DEF_EN)
				switch(Audio_MenuSetinit.GeHuVoice)
				{
					case 0:break;
					case 1:/*由C6000提供的语音输出提示音*/
							SoudReg = GetMenuSetStat(BARTYPESETFLG_SINGLECALL);
							SoundExe(SoudReg);			
							break;
					case 2:/*报号芯片的输出提示音*/
							SPK_INTF_Open();
							AudioVoiceOutSelect(SOUNDSTAT_GeHuVoice);
							DRV_AudioSwitch(DISABLE);
							break;
					default:break;
				}
				#else
				SoudReg = GetMenuSetStat(BARTYPESETFLG_SINGLECALL);
				SoundExe(SoudReg);	
				#endif 
			}	
		}break;
		case HRC_DGROUP:
		{
			if (opt)
			{
				#if(!LCD_DEF_EN)
				switch(Audio_MenuSetinit.ZuHuVoice)
				{
					case 0:break;
					case 1:/*由C6000提供的语音输出提示音*/
							SoudReg = GetMenuSetStat(BARTYPESETFLG_GROUPCALL);
							SoundExe(SoudReg);			
							break;
					case 2:/*报号芯片的输出提示音*/
							SPK_INTF_Open();
							AudioVoiceOutSelect(SOUNDSTAT_ZuHuVoice);
							DRV_AudioSwitch(DISABLE);
							break;
					default:break;
				}
				#else
				SoudReg = GetMenuSetStat(BARTYPESETFLG_GROUPCALL);
				SoundExe(SoudReg);	
				#endif	
			}
		}break;
		case HRC_DALLCALL:
		{
			#if(!LCD_DEF_EN)
			switch(Audio_MenuSetinit.QuanHuVoice)
			{
				case 0:break;
				case 1:/*由C6000提供的语音输出提示音*/
						SoudReg = GetMenuSetStat(BARTYPESETFLG_GROUPCALL);
						SoundExe(SoudReg);			
						break;
				case 2:/*报号芯片的输出提示音*/
						SPK_INTF_Open();
						AudioVoiceOutSelect(SOUNDSTAT_QuanHuVoice);
						DRV_AudioSwitch(DISABLE);
						break;
				default:break;
			}
			#endif

		}break;
		
	}
}
//
void TxTimeOut_Voice(u8 opt)
{
	#if(!LCD_DEF_EN)
	if(opt)
		SoundStartOpt(SOUNDSTAT_AudioVoice,SOUNDSTAT_TxTimeOutVoice);
	else
		SoundStartOpt(SOUNDSTAT_AudioVoice, SOUNDSTAT_BsTxTimeOutVoice);/*语音输出报号超时*/
	#else
	if(GetMenuSetStat(BARTYPESETFLG_Profile) != 1)
		SoundStart(SOUNDSTAT_Alarm);
	#endif
}
//













#endif


//播报未编程提示音
void audio_unprogram()
{
 // OSTimeDlyHMSM(0,0,0,80);
#if(!LCD_DEF_EN)
	//SoundStartOpt(SOUNDSTAT_AudioVoice, SOUNDSTAT_UnProgrammedChanleZoneVoice);
#endif
	//SoundStart(SOUNDSTAT_Alarm);

#if(FREQ_SCOPE_SET==FREQ_150_400)
//INTF_ChnUnlock();
//OSTimeDlyHMSM(0,0,1,660);
#endif
}



/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////俄语版本的报号修改//////////////////////////////////////

/*开机声音*/
void RU_InterohoneOpenVoice(void)
{//俄语版本的开机音只好使用5000音了,

//	if(vol==0)
//	{

//		HRC5000VolSet(1);
//	}
//	else
//	{
//		HRC5000VolSet(vol);
//	}
//	VOCODER_I2S_TxChan_Set(1);  
//	DRV_KeyAudioSwitch(ENABLE);
//	HR_Decoder_Open(); 
//	VoxCallOutTimeOut=VOX_I_MAX;
//	freq_I2S(20,64,0,0,0,2,0,0,0);
//	freq_I2S(20,100,200,0,0,2,2,0,0);
//	freq_I2S(20,84,169,253,0,2,2,1,0);
//	freq_I2S(21,133,266,399,532,2,2,1,0);  

//                        
//	HR_Decoder_Close();
//	DRV_KeyAudioSwitch(DISABLE);  
//	VOCODER_I2S_TxChan_Set(0); //此处喇叭没关闭的话,会有"波"的一声.
	//HRC5000VolSet(vol); //应该屏蔽掉       
	//OSTimeDly(500);
}


/*关机的声音*/
void RU_InterohoneCloseVoice(void)
{
	//PowerShutDownVoice();
//	if(PowerOpenVoiceDown)
//	{
//		HRC5000VolSet(12);
//		DRV_KeyAudioSwitch(ENABLE);
//		VOCODER_I2S_TxChan_Set(1);  
//		HR_Decoder_Open(); 
//		VoxCallOutTimeOut=VOX_I_MAX;
//		freq_I2S(30,74,10,0,0,2,0,0,0);
//		freq_I2S(30,110,210,0,0,2,2,0,0);
//		freq_I2S(30,94,179,263,0,2,2,1,0);
//		freq_I2S(30,54,159,203,0,2,2,1,0);

//		HR_Decoder_Close();
//		VOCODER_I2S_TxChan_Set(0); 	
//		DRV_KeyAudioSwitch(DISABLE);
//	}
}

/*频道报号*/
void RU_NumberVoice(void)
{
  //(Str_Audio.Attr,Str_Audio.Ccnt,Str_Audio.Zone);
/*要开启芯片的声音*/
  
#ifdef USE_NVDS_RU
	uint8 err;
    AudioWriteLock();
    SendManyByte_Add(0X93+Str_Audio.number);
    AudioWriteUnlock();
#else
  
	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);
	OSTimeDly(10);
	WriteI2CM6265Audio(0X93+Str_Audio.number);
	OSTimeDly(900);
	WriteI2CM6265Audio(0xD5);
	OSTimeDly(10);
	AudioWriteUnlock();
#endif
/*结束后设静音*/
}

/*频道报号*/
void RU_ChanleNumberVoice(void)
{
  //(Str_Audio.Attr,Str_Audio.Ccnt,Str_Audio.Zone);
/*要开启芯片的声音*/    
  
#ifdef USE_NVDS_RU
	uint8 err;
      AudioWriteLock();	
        switch(Str_Audio.Attr)
        {
        case 1:	
                SendManyByte_Add(0xA9);//模拟
                break;
        case 2:
                SendManyByte_Add(0xAB);
                break;
        default :
                break;
        }
	SendManyByte_Add(0xB2);//信道
	SendManyByte_Add(0X93+Str_Audio.Ccnt);
        
    AudioWriteUnlock();
    
    
    #else
    
    
	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);
	OSTimeDly(10);
	//WriteI2CM6265Audio(0X93+Str_Audio.Zone);
	//OSTimeDly(900);
	switch(Str_Audio.Attr)
		{
		case 1:	
			WriteI2CM6265Audio(0xA9);//模拟
			OSTimeDly(900);
			break;
		case 2:
			WriteI2CM6265Audio(0xAB);//数字
			OSTimeDly(900);
			break;
		default :
			break;
		}
	//WriteI2CM6265Audio(AUDIO_Mode);
	//OSTimeDly(1000);
	WriteI2CM6265Audio(0xB2);//信道
	OSTimeDly(700);
	WriteI2CM6265Audio(0X93+Str_Audio.Ccnt);
	OSTimeDly(900);
	WriteI2CM6265Audio(0xD5);
	OSTimeDly(10);
	AudioWriteUnlock();
/*结束后设静音*/
#endif
}
/*中继频道报号*/
void RU_BSChanleNumberVoice(void)
{
//(Str_Audio.Attr,Str_Audio.Ccnt,Str_Audio.Zone)
/*要开启芯片的声音*/
#ifdef USE_NVDS_RU
      uint8 err;
	AudioWriteLock();

	switch(Str_Audio.Attr)
        {
        case 1:	
                SendManyByte_Add(0xA9);//模拟
                break;
        case 2:
                SendManyByte_Add(0xAB);//数字
                break;
        default :
                break;
        }
	SendManyByte_Add(0xB2);
	SendManyByte_Add(0X93+Str_Audio.Ccnt);
	AudioWriteUnlock();
  
  #else
  
  
	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);
	OSTimeDly(10);
	//WriteI2CM6265Audio(0X93+Str_Audio.Zone);
	//OSTimeDly(900);

	switch(Str_Audio.Attr)
		{
		case 1:	
			WriteI2CM6265Audio(0xA9);//模拟
			OSTimeDly(900);
			break;
		case 2:
			WriteI2CM6265Audio(0xAB);//数字
			OSTimeDly(900);
			break;
		default :
			break;
		}
	//WriteI2CM6265Audio(AUDIO_Repeater);
	//OSTimeDly(1000);
	//WriteI2CM6265Audio(AUDIO_Mode);
	//OSTimeDly(1000);
	WriteI2CM6265Audio(0xB2);
	OSTimeDly(750);
	WriteI2CM6265Audio(0X93+Str_Audio.Ccnt);
	OSTimeDly(900);
	WriteI2CM6265Audio(0xD5);
	OSTimeDly(10);
	AudioWriteUnlock();
/*结束后设静音*/
#endif
}
/*信息查询*/
void RU_LookInfmVoice(void)
{
//(Str_Audio.Attr,Str_Audio.Ccnt,Str_Audio.Zone,Str_Audio.BsModeSelect,Str_Audio.LMAttr);
/*要开启芯片的声音*/
  
//#ifdef USE_NVDS_RU
//  uint8 err;
//	AudioWriteLock();
//	SendManyByte_Add(0X93+Str_Audio.Zone);

//	switch(Str_Audio.Attr)
//        {
//        case 1:	
//                SendManyByte_Add(0xA9);//模拟
//                break;
//        case 2:
//                SendManyByte_Add(0xAB);//数字
//                break;
//        default :
//                break;
//        }
//	if(Str_Audio.BsModeSelect)
//	{
//		SendManyByte_Add(0xAD);//中继
//	}
//	SendManyByte_Add(0xAC);//模式
//	SendManyByte_Add(0xB2);//信道
//	SendManyByte_Add(0X93+Str_Audio.Ccnt);
//	if(Str_Audio.Attr==2)
//	{
//		switch(Str_Audio.LMAttr)
//			{
//			case HRC_DPATCS:
//				SendManyByte_Add(0xBB);//个呼
//				break;
//			case HRC_DGROUP:
//				SendManyByte_Add(0xBC);//组呼
//				break;
//			default :	
//				SendManyByte_Add(0xBD);//全呼
//				break;
//			}
//	}
//	AudioWriteUnlock();
//  #else
//  
//  
//	uint8 err;
//	AudioWriteLock();
//	WriteI2CM6265Audio(0xD4);
//	OSTimeDly(10);
//	WriteI2CM6265Audio(0X93+Str_Audio.Zone);
//	OSTimeDly(900);

//	switch(Str_Audio.Attr)
//		{
//		case 1:	
//			WriteI2CM6265Audio(0xA9);//模拟
//			OSTimeDly(900);
//			break;
//		case 2:
//			WriteI2CM6265Audio(0xAB);//数字
//			OSTimeDly(900);
//			break;
//		default :
//			break;
//		}
//	if(Str_Audio.BsModeSelect)
//	{
//		WriteI2CM6265Audio(0xAD);//中继
//		OSTimeDly(1000);
//	}
//	WriteI2CM6265Audio(0xAC);//模式
//	OSTimeDly(500);

//	WriteI2CM6265Audio(0xB2);//信道
//	OSTimeDly(750);
//	WriteI2CM6265Audio(0X93+Str_Audio.Ccnt);
//	OSTimeDly(750);
//	if(Str_Audio.Attr==2)
//	{
//		switch(Str_Audio.LMAttr)
//			{
//			case HRC_DPATCS:
//				WriteI2CM6265Audio(0xBB);//个呼
//				OSTimeDly(1100);
//				break;
//			case HRC_DGROUP:
//				WriteI2CM6265Audio(0xBC);//组呼
//				OSTimeDly(1100);
//				break;
//			default :	
//				WriteI2CM6265Audio(0xBD);//全呼
//				OSTimeDly(1100);
//				break;
//			}
//	}
//	WriteI2CM6265Audio(0xD5);
//	OSTimeDly(10);
//	AudioWriteUnlock();
//#endif
/*结束后设静音*/
}
/*区域报号*/
void RU_ZoneVoice(void)
{
//(Str_Audio.Zone,Str_Audio.Attr,Str_Audio.Ccnt,Str_Audio.BsModeSelect);
/*要开启芯片的声音*/
#ifdef USE_NVDS_RU
  uint8 err;
	AudioWriteLock();
	SendManyByte_Add(0X93+Str_Audio.Zone);
	switch(Str_Audio.Attr)
		{
		case 1:	
			SendManyByte_Add(0xA9);//模拟
			break;
		case 2:
			SendManyByte_Add(0xAB);//数字
			break;
		default :
			break;
		}
	if(Str_Audio.BsModeSelect)
	{
		SendManyByte_Add(0xAD);//中继
	}
	SendManyByte_Add(0xAC);//模式
	SendManyByte_Add(0xB2);//信道
	SendManyByte_Add(0X93+Str_Audio.Ccnt);
	AudioWriteUnlock();
  
  #else
  
	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);
	OSTimeDly(10);
	WriteI2CM6265Audio(0X93+Str_Audio.Zone);
	OSTimeDly(900);
	switch(Str_Audio.Attr)
		{
		case 1:	
			WriteI2CM6265Audio(0xA9);//模拟
			OSTimeDly(900);
			break;
		case 2:
			WriteI2CM6265Audio(0xAB);//数字
			OSTimeDly(900);
			break;
		default :
			break;
		}
	if(Str_Audio.BsModeSelect)
	{
		WriteI2CM6265Audio(0xAD);//中继
		OSTimeDly(1000);
	}

	WriteI2CM6265Audio(0xAC);//模式
	OSTimeDly(500);
	WriteI2CM6265Audio(0xB2);//信道
	OSTimeDly(750);
	WriteI2CM6265Audio(0X93+Str_Audio.Ccnt);
	OSTimeDly(900);
	WriteI2CM6265Audio(0xD5);	//	关声音
	OSTimeDly(10);//需要延时才有效
	AudioWriteUnlock();
#endif

}
/*电量查询高中低*/
void RU_CouloStatVoice(void)
{
//Str_Audio.Colour
/*要开启芯片的声音*/
#ifdef USE_NVDS_RU
	uint8 err;
	AudioWriteLock();
	SendManyByte_Add(0xBF);//电池
	switch(Str_Audio.Colour)
        {
        case 0:
                        SendManyByte_Add(0xA6);//低
                        break;
        case 1:
                        SendManyByte_Add(0xA5);//中
                        break;
        case 2:
                        SendManyByte_Add(0xA4);//高
                        break;
        default :  break;
                        
        }
	AudioWriteUnlock();
  #else
  
	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);
	OSTimeDly(10);
	WriteI2CM6265Audio(0xBF);//电池
	OSTimeDly(900);
	//WriteI2CM6265Audio(0x57);//电量
	//OSTimeDly(800);
	switch(Str_Audio.Colour)
		{
		case 0:
				WriteI2CM6265Audio(0xA6);//低
				OSTimeDly(750);
				break;
		case 1:
				WriteI2CM6265Audio(0xA5);//中
				OSTimeDly(750);
				break;
		case 2:
				WriteI2CM6265Audio(0xA4);//高
				OSTimeDly(750);
				break;
		default :  break;
				
		}
	WriteI2CM6265Audio(0xD5);	//	关声音
	OSTimeDly(10);//需要延时才有效
	AudioWriteUnlock();
#endif
}
/*低电量提示*/
void RU_LowCouloVoice(void)
{
/*要开启芯片的声音*/
#ifdef USE_NVDS_RU
	uint8 err;
	AudioWriteLock();
	SendManyByte_Add(0xBF);//电池
	SendManyByte_Add(0xA6);//低
	AudioWriteUnlock();
  
  #else
	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);
	OSTimeDly(10);
	WriteI2CM6265Audio(0xBF);//电池
	OSTimeDly(900);
	WriteI2CM6265Audio(0xA6);//低
	OSTimeDly(750);
	WriteI2CM6265Audio(0xD5);	//	关声音
	OSTimeDly(10);//需要延时才有效
	AudioWriteUnlock();
#endif
}
/*选择输出的功率*/
void RU_PowerVoice(void)
{
//ChnPowerGet()
/*要开启芯片的声音*/
  
#ifdef USE_NVDS_RU
  uint8 err;
	AudioWriteLock();
	SendManyByte_Add(0xB5);//功率
	switch(CHN_BLL_PowerGet())//只有低和高功率
        {
        case 0:                    SendManyByte_Add(0xA6);//低                       break;
        case 1:                    SendManyByte_Add(0xA4);//高                       break;
        default :  break;		
        }
	AudioWriteUnlock();
  #else
	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);	//	开声音和数据手册上的相反
	OSTimeDly(10);
	WriteI2CM6265Audio(0xB5);//功率
	OSTimeDly(750);
	switch(ChnPowerGet())//只有低和高功率
		{
		case 0:
				WriteI2CM6265Audio(0xA6);//低
				OSTimeDly(750);
				break;
		case 1:
				WriteI2CM6265Audio(0xA4);//高
				OSTimeDly(750);
				break;
		default :  break;		
		}

	WriteI2CM6265Audio(0xD5);	//	关声音
	OSTimeDly(10);//需要延时才有效
	AudioWriteUnlock();
#endif
}
/*开始写频*/
void RU_PCtoMCUStartVoice(void)
{

}
/*结束写频*/
void RU_PCtoMCUEndVoice(void)
{

}
/*扫描启动*/
void RU_ScanStartVoice(void)
{
/*要开启芯片的声音*/
#ifdef USE_NVDS_RU
	uint8 err;
	AudioWriteLock();
	SendManyByte_Add(0xB7);//扫描
	SendManyByte_Add(0xC3);//开始
	AudioWriteUnlock();
  #else
	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);	//	开声音和数据手册上的相反
	OSTimeDly(10);
	WriteI2CM6265Audio(0xB7);//扫描
	OSTimeDly(1000);
	WriteI2CM6265Audio(0xC3);//开始
	OSTimeDly(1000);
	WriteI2CM6265Audio(0xD5);	//	关声音
	OSTimeDly(10);//需要延时才有效
	AudioWriteUnlock();
#endif
}
/*退出扫描*/
void RU_ScanEndVoice(void)
{
/*要开启芯片的声音*/
  
#ifdef USE_NVDS_RU
	uint8 err;
	AudioWriteLock();
	SendManyByte_Add(0xB7);//扫描
	SendManyByte_Add(0xC2);//退出
	AudioWriteUnlock();
    #else
	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);	//	开声音和数据手册上的相反
	OSTimeDly(10);
	WriteI2CM6265Audio(0xB7);//扫描
	OSTimeDly(900);
	WriteI2CM6265Audio(0xC2);//退出
	OSTimeDly(900);
	WriteI2CM6265Audio(0xD5);	//	关声音
	OSTimeDly(10);//需要延时才有效
	AudioWriteUnlock();
#endif
}
/*加锁*/
void RU_LockVoice(void)
{

}
/*解锁*/
void RU_UnLockVoice(void)
{

}
/*报警*/
void RU_RedAlarmVoice(void)
{
/*要开启芯片的声音*/
#ifdef USE_NVDS_RU
	uint8 err;
	AudioWriteLock();
	SendManyByte_Add(0xBA);//报警
	SendManyByte_Add(0xB0);//开
	AudioWriteUnlock();
    #else
	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);	//	开声音和数据手册上的相反
	OSTimeDly(10);
	WriteI2CM6265Audio(0xBA);//报警
	OSTimeDly(750);
	WriteI2CM6265Audio(0xB0);//开
	OSTimeDly(700);
	WriteI2CM6265Audio(0xD5);	//	关声音
	OSTimeDly(10);//需要延时才有效
	AudioWriteUnlock();
/*结束后设静音*/
#endif
}
/*报警结束音*/
void RU_RedAlarmStpVoice(void)
{
/*要开启芯片的声音*/
#ifdef USE_NVDS_RU
	uint8 err;
	AudioWriteLock();
	SendManyByte_Add(0xBA);//报警
	SendManyByte_Add(0xAF);//开
	AudioWriteUnlock();
    #else
	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);	//	开声音和数据手册上的相反
	OSTimeDly(10);
	WriteI2CM6265Audio(0xBA);//报警
	OSTimeDly(750);
	WriteI2CM6265Audio(0xAF);//开
	OSTimeDly(700);
	WriteI2CM6265Audio(0xD5);	//	关声音
	OSTimeDly(10);//需要延时才有效
	AudioWriteUnlock();
/*结束后设静音*/
#endif
}
/*个呼*/
void RU_GeHuVoice(void)
{
/*要开启芯片的声音*/
#ifdef USE_NVDS_RU
	uint8 err;
	AudioWriteLock();
	SendManyByte_Add(0xBB);//个呼
	AudioWriteUnlock();
    #else
	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);	//	开声音和数据手册上的相反
	OSTimeDly(10);
	WriteI2CM6265Audio(0xBB);//个呼 
	OSTimeDly(1600);
	WriteI2CM6265Audio(0xD5);	//	关声音
	OSTimeDly(10);//需要延时才有效
	AudioWriteUnlock();
/*结束后设静音*/
#endif
}
/*个呼*/
void RU_GeHuEndVoice(void)
{
/*要开启芯片的声音*/
#ifdef USE_NVDS_RU
	uint8 err;
	AudioWriteLock();
	SendManyByte_Add(0xBB);//个呼
	SendManyByte_Add(0xAF);//关
	AudioWriteUnlock();
    #else
	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);	//	开声音和数据手册上的相反
	OSTimeDly(10);
	WriteI2CM6265Audio(0xBB);//个呼
	OSTimeDly(1200);
	WriteI2CM6265Audio(0xAF);//关
	OSTimeDly(900);
	WriteI2CM6265Audio(0xD5);	//	关声音
	OSTimeDly(10);//需要延时才有效
	AudioWriteUnlock();
/*结束后设静音*/
#endif
}

/*组呼*/
void RU_ZuHuVoice(void)
{
/*要开启芯片的声音*/
#ifdef USE_NVDS_RU
	uint8 err;
	AudioWriteLock();
	SendManyByte_Add(0xBC);//组呼
	AudioWriteUnlock();
    #else
	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);	//	开声音和数据手册上的相反
	OSTimeDly(10);
	WriteI2CM6265Audio(0xBC);//组呼
	OSTimeDly(1200);
	WriteI2CM6265Audio(0xD5);	//	关声音
	OSTimeDly(10);//需要延时才有效
	AudioWriteUnlock();
/*结束后设静音*/
#endif
}
/*组呼*/
void RU_ZuHuEndVoice(void)
{
/*要开启芯片的声音*/
#ifdef USE_NVDS_RU
	uint8 err;
	AudioWriteLock();
	SendManyByte_Add(0xBC);//组呼
	SendManyByte_Add(0xAF);//关
	AudioWriteUnlock();
    #else
	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);	//	开声音和数据手册上的相反
	OSTimeDly(10);
	WriteI2CM6265Audio(0xBC);//组呼
	OSTimeDly(1200);
	WriteI2CM6265Audio(0xAF);//关
	OSTimeDly(900);
	WriteI2CM6265Audio(0xD5);	//	关声音
	OSTimeDly(10);//需要延时才有效
	AudioWriteUnlock();
/*结束后设静音*/
#endif
}

/*全呼*/
void RU_QuanHuVoice(void)
{
/*要开启芯片的声音*/
#ifdef USE_NVDS_RU
	uint8 err;
	AudioWriteLock();
	SendManyByte_Add(0xBD);//全呼
	AudioWriteUnlock();
    #else 
	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);	//	开声音和数据手册上的相反
	OSTimeDly(10);
	WriteI2CM6265Audio(0xBD);//全呼
	OSTimeDly(1200);
	WriteI2CM6265Audio(0xD5);	//	关声音
	OSTimeDly(10);//需要延时才有效
	AudioWriteUnlock();
/*结束后设静音*/
#endif
}
/*发射超时限发*/
void RU_TxTimeOutVoice(void)
{
/*要开启芯片的声音*/
#ifdef USE_NVDS_RU
	uint8 err;
	AudioWriteLock();
        SendManyByte_Add(0xC4);//发射
	SendManyByte_Add(0xB9);//超时
	AudioWriteUnlock();
    #else
	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);	//	开声音和数据手册上的相反
	OSTimeDly(10);
	WriteI2CM6265Audio(0xC4);//发射
	OSTimeDly(1000);
	WriteI2CM6265Audio(0xB9);//超时
	OSTimeDly(1000);
	WriteI2CM6265Audio(0xD5);	//	关声音
	OSTimeDly(10);//需要延时才有效
	AudioWriteUnlock();
/*结束后设静音*/
#endif
}
/*中继激活后发射超时限发*/
void RU_BsTxTimeOutVoice(void)
{
/*要开启芯片的声音*/
#ifdef USE_NVDS_RU
	uint8 err;
	AudioWriteLock();
        SendManyByte_Add(0xAD);//中继
	SendManyByte_Add(0xBE);//主动呼叫
	SendManyByte_Add(0xB9);//超时
	AudioWriteUnlock();
    #else
	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);	//	开声音和数据手册上的相反
	OSTimeDly(10);
	WriteI2CM6265Audio(0xAD);//中继
	OSTimeDly(1000);
	WriteI2CM6265Audio(0xBE);//主动呼叫
	OSTimeDly(1000);
	WriteI2CM6265Audio(0xB9);//超时
	OSTimeDly(1000);
	WriteI2CM6265Audio(0xD5);	//	关声音
	OSTimeDly(10);//需要延时才有效
//	BSOverTime=0;
	AudioWriteUnlock();
/*结束后设静音*/
#endif
}
/*加密开*/
void RU_EncrptOpenVoice(void)
{
/*要开启芯片的声音*/
#ifdef USE_NVDS_RU
	uint8 err;
	AudioWriteLock();
	SendManyByte_Add(0xB8);//加密
	SendManyByte_Add(0xB0);//开
	AudioWriteUnlock();
    #else
	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);	//	开声音和数据手册上的相反
	OSTimeDly(10);
	WriteI2CM6265Audio(0xB8);//加密
	OSTimeDly(950);
	WriteI2CM6265Audio(0xB0);//开
	OSTimeDly(850);
	WriteI2CM6265Audio(0xD5);	//	关声音
	OSTimeDly(10);//需要延时才有效
	AudioWriteUnlock();
/*结束后设静音*/
#endif
}
/*加密关*/
void RU_EncrptCloseVoice(void)
{
/*要开启芯片的声音*/
#ifdef USE_NVDS_RU
	uint8 err;
	AudioWriteLock();
	SendManyByte_Add(0xB8);//加密
	SendManyByte_Add(0xAF);//关
	AudioWriteUnlock();
    #else
	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);	//	开声音和数据手册上的相反
	OSTimeDly(10);
	WriteI2CM6265Audio(0xB8);//加密
	OSTimeDly(950);
	WriteI2CM6265Audio(0xAF);//关
	OSTimeDly(850);;
	WriteI2CM6265Audio(0xD5);	//	关声音
	OSTimeDly(10);//需要延时才有效
	AudioWriteUnlock();
/*结束后设静音*/
#endif
}
/*声控开*/
void RU_SKOpenVoice(void)
{
/*要开启芯片的声音*/
#ifdef USE_NVDS_RU
	uint8 err;
	AudioWriteLock();
	SendManyByte_Add(0xB6);//声控
	SendManyByte_Add(0xB0);//开
	AudioWriteUnlock();
    #else
	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);	//	开声音和数据手册上的相反
	OSTimeDly(10);
	WriteI2CM6265Audio(0xB6);//声控
	OSTimeDly(950);
	WriteI2CM6265Audio(0xB0);//开
	OSTimeDly(900);
	WriteI2CM6265Audio(0xD5);	//	关声音
	OSTimeDly(10);//需要延时才有效
	AudioWriteUnlock();
	
/*结束后设静音*/
#endif
}

/*声控关*/
void RU_SKCloseVoice(void)
{
/*要开启芯片的声音*/
#ifdef USE_NVDS_RU
	uint8 err;
	AudioWriteLock();
	SendManyByte_Add(0xB6);//声控
	SendManyByte_Add(0xAF);//关
	AudioWriteUnlock();
    #else
	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);	//	开声音和数据手册上的相反
	OSTimeDly(10);
	WriteI2CM6265Audio(0xB6);//声控
	OSTimeDly(950);
	WriteI2CM6265Audio(0xAF);//关
	OSTimeDly(900);
	WriteI2CM6265Audio(0xD5);	//	关声音
	OSTimeDly(10);//需要延时才有效
	AudioWriteUnlock();
/*结束后设静音*/
#endif
}
/*
*音量大小的等级
*/
void RU_VolumeGrade(void)
{
}
/*静燥等级*/
void RU_SQGrade(void)
{

/*要开启芯片的声音*/
#ifdef USE_NVDS_RU
	uint8 err;
	AudioWriteLock();
	SendManyByte_Add(0xA7);
	if( Func_Str.sqlevel>0)
	{
		SendManyByte_Add(0x93+ Func_Str.sqlevel);//
	}
	else
	{
		SendManyByte_Add(0xB0);//开
	}
	AudioWriteUnlock();
    #else
	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);	//	开声音和数据手册上的相反
	OSTimeDly(10);
	WriteI2CM6265Audio(0xA7);
	OSTimeDly(950);
	if(GetMenuSetStat(BARTYPESETFLG_Squelch)>0)
	{
		WriteI2CM6265Audio(0x93+GetMenuSetStat(BARTYPESETFLG_Squelch));//
		OSTimeDly(900);
	}
	else
	{
		WriteI2CM6265Audio(0xB0);//开
		OSTimeDly(900);
	}
	WriteI2CM6265Audio(0xD5);	//	关声音
	OSTimeDly(10);//需要延时才有效
	AudioWriteUnlock();
/*结束后设静音*/
#endif
}
void RU_ScanCallInStart(void)
{
//(Str_Audio.Attr,Str_Audio.Ccnt)
/*要开启芯片的声音*/
#ifdef USE_NVDS_RU
//    STR_CODESW PLLStat=GetPllStat();
//	uint8 err;
//	AudioWriteLock();
//	switch(Str_Audio.Attr)
//        {
//        case 1:	
//                SendManyByte_Add(0xA9);//模拟
//                break;
//        case 2:
//                SendManyByte_Add(0xAB);//数字
//        default :
//                break;
//        }
//	SendManyByte_Add(0xB2);//信道
//	SendManyByte_Add(0x93+Str_Audio.Ccnt);
//	AudioWriteUnlock();
//    #else
//	STR_CODESW PLLStat=GetPllStat();
//	uint8 err;
//	AudioWriteLock();
//	WriteI2CM6265Audio(0xD4);	//	开声音和数据手册上的相反
//	OSTimeDly(10);
//	switch(Str_Audio.Attr)
//		{
//		case 1:	
//			WriteI2CM6265Audio(0xA9);//模拟
//			OSTimeDly(900);
//			break;
//		case 2:
//			WriteI2CM6265Audio(0xAB);//数字
//			OSTimeDly(900);
//		default :
//			break;
//		}
//	WriteI2CM6265Audio(0xB2);//信道
//	OSTimeDly(750);
//	WriteI2CM6265Audio(0x93+Str_Audio.Ccnt);
//	OSTimeDly(750);
//	WriteI2CM6265Audio(0xD5);	//	关声音
//	OSTimeDly(10);//需要延时才有效
//	AudioWriteUnlock();
///*结束后设静音*/


#endif
}
void RU_ScanMode(void)
{
/*要开启芯片的声音*/
#ifdef USE_NVDS_RU
 	uint8 err;
	AudioWriteLock();
	SendManyByte_Add(0xB7);//扫描
	SendManyByte_Add(0xAC);//模式
//	SendManyByte_Add(0X93+StrMMenuSet.ScanMode+1);
	AudioWriteUnlock();
    #else
 	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);	//	开声音和数据手册上的相反
	OSTimeDly(10);
	WriteI2CM6265Audio(0xB7);//扫描
	OSTimeDly(900);
	WriteI2CM6265Audio(0xAC);//模式
	OSTimeDly(500);
	WriteI2CM6265Audio(0X93+StrMMenuSet.ScanMode+1);
	OSTimeDly(750);
	WriteI2CM6265Audio(0xD5);	//	关声音
	OSTimeDly(10);//需要延时才有效
	AudioWriteUnlock();
/*结束后设静音*/

#endif
}

/*报号调试*/
void AudioVoiceTst(uint8 Ccnt)
{
  //(Str_Audio.Attr,Str_Audio.Ccnt,Str_Audio.Zone);
/*要开启芯片的声音*/
  
	uint8 err;
	AudioWriteLock();
	WriteI2CM6265Audio(0xD4);
	OSTimeDly(10);

	WriteI2CM6265Audio(Ccnt);
	OSTimeDly(1500);
	WriteI2CM6265Audio(0xD5);
	OSTimeDly(10);
	AudioWriteUnlock();
/*结束后设静音*/

}
