//DRV_Audio.h
#ifndef _DRV_AUDIO_H
#define	_DRV_AUDIO_H

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


typedef struct {
uint8 Attr;/*模数属性*/
uint8 Ccnt;/*频道号*/
uint8 Zone;/*区域报号*/
uint8 BsModeSelect;/*0为直通模式1为中继模式*/
uint8 Colour;/*电量的状态*/
uint8 UnProgrammed;/*编程的状态0已编程1未编程*/
uint8 LMAttr;/*联系人的属性，个呼....*/
uint8 ZoneSet;/*1为自定义按键区域设置标志位*/
uint8 VoiceVolume;/*提示音的大小*/
uint8 chg_chl_flg;//信道改变报号标志位
uint8 number;//报数
}STR_AUDIO;

extern STR_AUDIO Str_Audio;/*用于语音报号*/

/*
typedef enum
{
AUDIO_One     			,//0X00      1              0X4A                                                 
AUDIO_Two     			,//0X01      2              0X4B                                                 
AUDIO_Three    			,//0X02      3              0X4C                                                 
AUDIO_Four     			,//0X03      4              0X4D                                                 
AUDIO_Five     			,//0X04      5              0X4E                                                 
AUDIO_Six     			,//0X05      6              0X4F                                                 
AUDIO_Seven    			,//0X06      7              0X50                                                 
AUDIO_Eight    			,//0X07      8              0X51                                                 
AUDIO_Nine     			,//0X08      9              0X52                                                 
AUDIO_Ten     			,//0X09     10              0X53                                                 
AUDIO_Eleven    		,//0X0A     11              0X54                                                 
AUDIO_Twelve    		,//0X0B     12              0X55                                                 
AUDIO_Thirteen   		,//0X0C     13              0X56                                                 
AUDIO_Fourteen   		,//0X0D     14              0X57                                                 
AUDIO_Fifteen   		,//0X0E     15              0X58                                                 
AUDIO_Sixteen   		,//0X0F     16              0X59                                                 
AUDIO_Seventeen  		,//0X10     17              0X5A                                                 
AUDIO_Eighteen   		,//0X11     18              0X5B                                                 
AUDIO_Nineteen   		,//0X12     19              0X5C                                                 
AUDIO_Twenty    		,//0X13     20              0X5D                                                 
AUDIO_thirty    		,//0X14     30              0X5E                                                 
AUDIO_forty    			,//0X15     40              0X5F                                                 
AUDIO_fifty    			,//0X16     50              0X60                                                 
AUDIO_sixty    			,//0X17     60              0X61                                                 
AUDIO_seventy   		,//0X18     70              0X62   
AUDIO_eighty    		,//0X19     80             0X63                                                  
AUDIO_ninety    		,//0X1A     90             0X64                                                  
AUDIO_hundred    		,//0X1B     100            0X65                                                  
AUDIO_High      		,//0X1C     高             0X66                                                 
AUDIO_Middle     		,//0X1D     中             0X67                                                 
AUDIO_Low      			,//0X1E     低             0X68                                                 
AUDIO_Product    		,//0X1F    产品            0X69                                                
AUDIO_Squelch    		,//0X20    静噪            0X6A                                                
AUDIO_Zone     			,//0X21    区域            0X6B                                                
AUDIO_Prompt    		,//0X22    提示            0X6C                                                
AUDIO_Revive    		,//0X23    激活            0X6D                                                
AUDIO_Remote    		,//0X24    远程            0X6E                                                
AUDIO_Detection   	,//0X25    检测            0X6F                                                
AUDIO_Kill     			,//0X26    谣毙            0X70                                                
AUDIO_VFO      			,//0X27    全频            0X71                                                
AUDIO_User_defined 	,//0X28   自定义           0X72                                               
AUDIO_DPMR      		,//0X29    DPMR            0X73                                                  
AUDIO_DMR      			,//0X2A     DMR            0X74                                                  
AUDIO_Analog     		,//0X2B    模拟            0X75                                                
AUDIO_Mixed     		,//0X2C    混合            0X76                                                
AUDIO_Digital    		,//0X2D    数字            0X77                                                
AUDIO_Mode      		,//0X2E    模式            0X78                                                
AUDIO_Repeater   		,//0X2F    中继            0X79                                                
AUDIO_Time_Slot   	,//0X30    时隙            0X7A                                                
AUDIO_Off      			,//0X31     关             0X7B   
AUDIO_On      			,//0X32     开              0X7C                                                                                                    
AUDIO_Priority   		,//0X33    优先             0X7D                                                     
AUDIO_Volume    		,//0X34    音量             0X7E                                                     
AUDIO_Channel    		,//0X35    频道             0X7F                                                     
AUDIO_Call     			,//0X36    呼叫             0X80                                                     
AUDIO_TalkAround  	,//0X37    脱网             0X81                                                     
AUDIO_Bluetooth   	,//0X38    蓝牙             0X82                                                     
AUDIO_Power     		,//0X39    电源             0X83                                                     
AUDIO_VOX      			,//0X3A    声控             0X84                                                     
AUDIO_Scan     			,//0X3B    扫描             0X85                                                     
AUDIO_Encryption  	,//0X3C    加密             0X86                                                     
AUDIO_GPS      			,//0X3D     GPS             0X87                                                      
AUDIO_BatteryLevel ,//0X3E    电量             0X88                                                  
AUDIO_goodbye    		,//0X3F    再见             0X89                                                  
AUDIO_Welcome    		,//0X40  欢迎使用           0X8A                                              
AUDIO_Plscharge  	,//0X41   请充电            0X8B                                                   
AUDIO_timeout   		,//0X42    超时             0X8C                     
AUDIO_Lock     			,//0X43    加锁             0X8D                                                     
AUDIO_Unlock    		,//0X44    解锁             0X8E                                                     
AUDIO_Alarm     		,//0X45    报警             0X8F                                                     
AUDIO_PrivateCall  ,//0X46    个呼             0X90                                                     
AUDIO_GroupCall  	,//0X47    组呼             0X91                                                     
AUDIO_GeneralCall 	,//0X48    全呼             0X92                                                     
AUDIO_Monitor    		,//0X49    监听             0X93    
                                                 
}AUDIO_NUMBER;
*/      
#define AUDIO_OPEN			0xD4
#define AUDIO_CLOSE			0xD5


#define AUDIO_Copy			0X3A//   复制     0X6D                                        
#define AUDIO_Write    		0X3b//  写频         0X6E  

#define AUDIO_Start    			0X3c//   开始     0X6E                                               
#define AUDIO_Stop    			0X3d//   完成        0X6E                                              
#define AUDIO_AGP    			0X3e//    A组        0X6E                                               
#define AUDIO_BGP    			0X3f//    B组            0X6E                                               
#define AUDIO_CGP    			0X40//    C组          0X6E                                                 
#define AUDIO_Master   			0X4E//    主动          0X80      
#define AUDIO_BATT   			0X4E//   电池
#define AUDIO_TX  			0X5B//   发射

#if 1
#define AUDIO_EN				0
#define AUDIO_One     			0X00//      1              0X4A      

#define AUDIO_High      		0X33//     高             0X66                                                 
#define AUDIO_Middle     		0X34//     中             0X67                                                 
#define AUDIO_Low      		0X35//     低             0X68                                                 
#define AUDIO_Product    		0X36//    产品            0X69                                                
#define AUDIO_Squelch    		0X37//    静噪            0X6A                                                
#define AUDIO_Zone     		0X38//    区域            0X6B                                                
#define AUDIO_Prompt    		0X39//    提示            0X6C    

#define AUDIO_Revive    		0X3A//    激活            0X6D                                                      
#define AUDIO_Remote    		0X3b//    远程            0X6E                                                                                 
#define AUDIO_Detection   		0X25//    检测            0X6F                                                
#define AUDIO_Kill     			0X26//    谣毙            0X70                                                
#define AUDIO_VFO      		0X27//    全频            0X71                                                
#define AUDIO_Userdefined 	0X28//   自定义           0X72  

#define AUDIO_DPMR      		0X41//    DPMR            0X73                                                  
#define AUDIO_DMR      		0X42//     DMR            0X74                                                  
#define AUDIO_Analog     		0X43//    模拟            0X75                                                
#define AUDIO_Mixed     		0X44//    混合            0X76                                                
#define AUDIO_Digital    		0X45//    数字            0X77                                                
#define AUDIO_Mode      		0X46//    模式            0X78                                                
#define AUDIO_Repeater   		0X47//    中继            0X79                                                
#define AUDIO_TimeSlot   		0X48//    时隙            0X7A                                                
#define AUDIO_Off      			0X49//     关             0X7B   
#define AUDIO_On      			0X4A//     开              0X7C                                                                                                    
#define AUDIO_Priority   		0X4B//    优先             0X7D                                                     
#define AUDIO_Volume    		0X4C//    音量             0X7E                                                     
#define AUDIO_Channel    		0X4D//    频道             0X7F                                                     
#define AUDIO_Call     			0X4F//    呼叫             0X80  

#define AUDIO_TalkAround  	0X50//    脱网             0X81                                                     
#define AUDIO_Bluetooth   		0X51//    蓝牙             0X82                                                     
#define AUDIO_Power     		0X52//    电源             0X83                                                     
#define AUDIO_VOX      		0X53//    声控             0X84                                                     
#define AUDIO_Scan     		0X54//    扫描             0X85                                                     
#define AUDIO_Encryption  	0X55//    加密             0X86     

#define AUDIO_GPS      		0X3D//     GPS             0X87                                                      
#define AUDIO_BatteryLevel 	0X57//    电量             0X88   

#define AUDIO_goodbye    		0X58//    再见             0X89                                                  
#define AUDIO_Welcome    	0X59//  欢迎使用           0X8A                                              
#define AUDIO_Plscharge  		0X5A//   请充电            0X8B                                                   
#define AUDIO_timeout   		0X5C//    超时             0X8C                     
#define AUDIO_Lock     		0X5D//    加锁             0X8D                                                     
#define AUDIO_Unlock    		0X5E//    解锁             0X8E                                                     
#define AUDIO_Alarm     		0X5F//    报警             0X8F                                                     
#define AUDIO_PrivateCall  	0X60//    个呼             0X90                                                     
#define AUDIO_GroupCall  		0X61//    组呼             0X91                                                     
#define AUDIO_GeneralCall 	0X62//    全呼             0X92         
#define AUDIO_RUN		 	0X63//启动
#define AUDIO_CANCEL	 	0X64//取消
#define AUDIO_EXIT			0X65//退出
#define AUDIO_PUT			0X66//按
#define AUDIO_ADD		 	0X67//+
#define AUDIO_DEC		 	0X68//-
#define AUDIO_KEY			0X69//键
#define AUDIO_UP 			0X6A//上
#define AUDIO_DW 			0X6B//下
#define AUDIO_INPUT 			0X6C//输入	
#define AUDIO_CODE 			0X6D//编码
#define AUDIO_PPT			0X6E//PPT
#define AUDIO_FREQ 			0X6F//频率

#define AUDIO_Monitor    		0X70//    监听             0X93    


#else
#define AUDIO_EN			0X4A
#define AUDIO_One     			0X00//      1              0X4A                                                 
#define AUDIO_Two     			0X01//      2              0X4B                                                 
#define AUDIO_Three    			0X02//      3              0X4C                                                 
#define AUDIO_Four     			0X03//      4              0X4D                                                 
#define AUDIO_Five     			0X04//      5              0X4E                                                 
#define AUDIO_Six     			0X05//      6              0X4F                                                 
#define AUDIO_Seven    			0X06//      7              0X50                                                 
#define AUDIO_Eight    			0X07//      8              0X51                                                 
#define AUDIO_Nine     			0X08//      9              0X52                                                 
#define AUDIO_Ten     			0X09//     10              0X53                                                 
#define AUDIO_Eleven    		0X0A//     11              0X54                                                 
#define AUDIO_Twelve    		0X0B//     12              0X55                                                 
#define AUDIO_Thirteen   		0X0C//     13              0X56                                                 
#define AUDIO_Fourteen   		0X0D//     14              0X57                                                 
#define AUDIO_Fifteen   		0X0E//     15              0X58                                                 
#define AUDIO_Sixteen   		0X0F//     16              0X59                                                 
#define AUDIO_Seventeen  		0X10//     17              0X5A                                                 
#define AUDIO_Eighteen   		0X11//     18              0X5B                                                 
#define AUDIO_Nineteen   		0X12//     19              0X5C                                                 
#define AUDIO_Twenty    		0X13//     20              0X5D                                                 
#define AUDIO_thirty    		0X14//     30              0X5E                                                 
#define AUDIO_forty    			0X15//     40              0X5F                                                 
#define AUDIO_fifty    			0X16//     50              0X60                                                 
#define AUDIO_sixty    			0X17//     60              0X61                                                 
#define AUDIO_seventy   		0X18//     70              0X62   
#define AUDIO_eighty    		0X19//     80             0X63                                                  
#define AUDIO_ninety    		0X1A//     90             0X64                                                  
#define AUDIO_hundred    		0X1B//     100            0X65                                                  
#define AUDIO_High      		0X1C//     高             0X66                                                 
#define AUDIO_Middle     		0X1D//     中             0X67                                                 
#define AUDIO_Low      			0X1E//     低             0X68                                                 
#define AUDIO_Product    		0X1F//    产品            0X69                                                
#define AUDIO_Squelch    		0X20//    静噪            0X6A                                                
#define AUDIO_Zone     			0X21//    区域            0X6B                                                
#define AUDIO_Prompt    		0X22//    提示            0X6C                                                
#define AUDIO_Revive    		0X23//    激活            0X6D                                                
#define AUDIO_Remote    		0X24//    远程            0X6E                                                
#define AUDIO_Detection   	0X25//    检测            0X6F                                                
#define AUDIO_Kill     			0X26//    谣毙            0X70                                                
#define AUDIO_VFO      			0X27//    全频            0X71                                                
#define AUDIO_Userdefined 	0X28//   自定义           0X72                                               
#define AUDIO_DPMR      		0X29//    DPMR            0X73                                                  
#define AUDIO_DMR      			0X2A//     DMR            0X74                                                  
#define AUDIO_Analog     		0X2B//    模拟            0X75                                                
#define AUDIO_Mixed     		0X2C//    混合            0X76                                                
#define AUDIO_Digital    		0X2D//    数字            0X77                                                
#define AUDIO_Mode      		0X2E//    模式            0X78                                                
#define AUDIO_Repeater   		0X2F//    中继            0X79                                                
#define AUDIO_TimeSlot   	0X30//    时隙            0X7A                                                
#define AUDIO_Off      			0X31//     关             0X7B   
#define AUDIO_On      			0X32//     开              0X7C                                                                                                    
#define AUDIO_Priority   		0X33//    优先             0X7D                                                     
#define AUDIO_Volume    		0X34//    音量             0X7E                                                     
#define AUDIO_Channel    		0X35//    频道             0X7F                                                     
#define AUDIO_Call     			0X36//    呼叫             0X80                                                     
#define AUDIO_TalkAround  	0X37//    脱网             0X81                                                     
#define AUDIO_Bluetooth   	0X38//    蓝牙             0X82                                                     
#define AUDIO_Power     		0X39//    电源             0X83                                                     
#define AUDIO_VOX      			0X3A//    声控             0X84                                                     
#define AUDIO_Scan     			0X3B//    扫描             0X85                                                     
#define AUDIO_Encryption  	0X3C//    加密             0X86                                                     
#define AUDIO_GPS      			0X3D//     GPS             0X87                                                      
#define AUDIO_BatteryLevel 0X3E//    电量             0X88                                                  
#define AUDIO_goodbye    		0X3F//    再见             0X89                                                  
#define AUDIO_Welcome    		0X40//  欢迎使用           0X8A                                              
#define AUDIO_Plscharge  	0X41//   请充电            0X8B                                                   
#define AUDIO_timeout   		0X42//    超时             0X8C                     
#define AUDIO_Lock     			0X43//    加锁             0X8D                                                     
#define AUDIO_Unlock    		0X44//    解锁             0X8E                                                     
#define AUDIO_Alarm     		0X45//    报警             0X8F                                                     
#define AUDIO_PrivateCall  0X46//    个呼             0X90                                                     
#define AUDIO_GroupCall  	0X47//    组呼             0X91                                                     
#define AUDIO_GeneralCall 	0X48//    全呼             0X92                                                     
#define AUDIO_Monitor    		0X49//    监听             0X93    
#endif

typedef struct{
uint8 InterohoneOpenVoice;/*对应操作时报何种声音0:静音1:提示音2:语音*/
uint8 InterohoneCloseVoice;
uint8 ChanleNumberVoice;
uint8 BSChanleNumberVoice;

uint8 CouloStatVoice;
uint8 LowCouloVoice;
uint8 PowerVoice;
uint8 PCtoMCUStartVoice;
uint8 PCtoMCUEndVoice;
uint8 LookInfmVoice;
uint8 LockVoice;		
uint8 UnLockVoice;
uint8 RedAlarmVoice;
uint8 GeHuVoice;
uint8 GeHuEndVoice;
uint8 ZuHuVoice;
uint8 ZuHuEndVoice;
uint8 QuanHuVoice;
uint8 TxTimeOutVoice;
uint8 BsTxTimeOutVoice;		
uint8 EncrptOpenVoice;
uint8 EncrptCloseVoice;
uint8 SKOpenVoice;
uint8 SKCloseVoice;
uint8 ZoneVoice;
uint8 VolumeGradeVoice;
uint8 SQGradeVoice;
uint8 ScanCallInStart;/*扫到信道报*/
uint8 ScanMode;/*扫描模式*/
uint8 ScanStartVoice;
uint8 ScanEndVoice;
uint8 UnProgrammedChanleNumberVoice;
}AUDIO_MENUSETINIT;
extern AUDIO_MENUSETINIT Audio_MenuSetinit;
extern void (*PVoiceFunction[2][36])(void);

//extern OS_EVENT *Sem_AudioLock;
#define AudioWriteLock() // OSSemPend(Sem_AudioLock,0,&err)
#define AudioWriteUnlock()// OSSemPost(Sem_AudioLock)


void DRV_AudioSetInit(void);
void DRV_AudioVoiceSelct(uint8 stat);
void AudioVoiceOutSelect(uint8 stat);
void AudioVoiceTst(uint8 Ccnt);
void audio_test(uint8 cnt);

void InterohoneOpenVoice(void);
void InterohoneCloseVoice(void);
void ChanleNumberVoice(void);
void NumberVoice(void);
void BSChanleNumberVoice(void);
void LookInfmVoice(void);
void UnProgrammedChanleNumberVoice(void);
void UnProgrammedChanleZoneVoice(void);
void ZoneVoice(void);
void CouloStatVoice(void);
void LowCouloVoice(void);
void PowerVoice(void);
void PCtoMCUStartVoice(void);
void PCtoMCUEndVoice(void);
void ScanStartVoice(void);
void ScanEndVoice(void);
void LockVoice(void);
void UnLockVoice(void);
void RedAlarmVoice(void);
void RedAlarmStpVoice(void);
void GeHuVoice(void);
void GeHuEndVoice(void);
void ZuHuVoice(void);
void ZuHuEndVoice(void);
void QuanHuVoice(void);
void TxTimeOutVoice(void);
void BsTxTimeOutVoice(void);
void EncrptOpenVoice(void);
void EncrptCloseVoice(void);
void SKOpenVoice(void);
void SKCloseVoice(void);
void VolumeGrade(void);
void SQGrade(void);
void ScanCallInStart(void);
void ScanMode(void);
void audio_unprogram();
void audio_ch_change(u8,u8);
void CallOutVioce(u8 type,u8 opt);
void TxTimeOut_Voice(u8);

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////俄语版本的报号修改//////////////////////////////////////
void RU_InterohoneOpenVoice(void);
void RU_InterohoneCloseVoice(void);
void RU_ChanleNumberVoice(void);
void RU_NumberVoice(void);
void RU_BSChanleNumberVoice(void);
void RU_LookInfmVoice(void);
void RU_UnProgrammedChanleNumberVoice(void);
void RU_UnProgrammedChanleZoneVoice(void);
void RU_ZoneVoice(void);
void RU_CouloStatVoice(void);
void RU_LowCouloVoice(void);
void RU_PowerVoice(void);
void RU_PCtoMCUStartVoice(void);
void RU_PCtoMCUEndVoice(void);
void RU_ScanStartVoice(void);
void RU_ScanEndVoice(void);
void RU_LockVoice(void);
void RU_UnLockVoice(void);
void RU_RedAlarmVoice(void);
void RU_RedAlarmStpVoice(void);
void RU_GeHuVoice(void);
void RU_GeHuEndVoice(void);
void RU_ZuHuVoice(void);
void RU_ZuHuEndVoice(void);
void RU_QuanHuVoice(void);
void RU_TxTimeOutVoice(void);
void RU_BsTxTimeOutVoice(void);
void RU_EncrptOpenVoice(void);
void RU_EncrptCloseVoice(void);
void RU_SKOpenVoice(void);
void RU_SKCloseVoice(void);
void RU_VolumeGrade(void);
void RU_SQGrade(void);
void RU_ScanCallInStart(void);
void RU_ScanMode(void);
#endif
