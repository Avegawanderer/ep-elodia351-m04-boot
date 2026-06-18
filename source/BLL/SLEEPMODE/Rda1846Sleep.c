
#include "app_config.h"

#include "include.h"


#define APP_SYSSLEEP_EN 		1


#define TASKSystemSleep_STK_SIZE 	TASKSystemSleep_STK_SIZE_
#define TASKSystemSleep_PRIO  		TASKSystemSleep_PRIO_

//static OS_STK  TaskSystemSleepStk[TASKSystemSleep_STK_SIZE];
uint32 SleepTimeCouter=0;

//OS_THREAD_DEF_ID(TaskSystemSleep, osscanIndexityNormal, TASKSystemSleep_STK_SIZE);

//extern osThreadId ThreadId_RX_SW_DirectoryTask;

/*预设唤醒与休眠比例1:1*/
#define CSLP_SLEEP_TIME    2
#define CSLP_UNSLEEP_TIME  2
Str_SYSSLP_ Str_SyatemSlp;
/*------------- ---------------公共变量----------------------------------*/
u8 IS_COM_Wake_Up;//唤醒方式
uint8 RDA1846sPdnDown=0;/*1846s   1==已经进入省电模式0==不是*/
/*-----------------------------私有函数---------------------------------*/
void PowerDown_Init(void);
void PowerDown_Start(u8 wakeuptime);
void PowerDown_RTC_Config(void);
void PowerDown_EXTI_Config(void);
void PowerDown_NVIC_Config(void);
void PowerDown_RTC_IT_Handler(void);
void PowerDown_EXTI_IT_Handler(void);
void APP_SleepTaskSw(void);

void TaskSystemSleepInit(void)
{
#if  APP_SYSSLEEP_EN 
///	uint8 err;
	//uint8 C5000SlpMode = GetMenuSetStat(BARTYPESETFLG_POWERM);

	Str_SyatemSlp.CSLP_Mode = SYSSLP_MODE_CHCK;
	Str_SyatemSlp.CSLP_SystemSlpStat = SYSSLP_RDA1846NSLP;
	Str_SyatemSlp.CSLP_SlpCnt = 0;
       Str_SyatemSlp.CSLP_USlpCnt = 0;
	RDA1846sPdnDown=0;
//	ThreadId_TaskSystemSleep = OSTaskCreate(TaskSystemSleep,(void *)0,&TaskSystemSleepStk[TASKSystemSleep_STK_SIZE-1],TASKSystemSleep_PRIO);
//	Assert( err == OS_NO_ERR ,"省电任务创建不成功!");

#endif	
}

void TaskSystemSleepAct(void)
{	
	TaskHndStart(TASK_FLAG_SLEEP);

//	COMMONPost(ThreadId_TaskSystemSleep,SysSleepFlag);
}
extern volatile uint8 codeswStat;
extern uint8 SQ_Cnt;

/*****************************************************************************
 *   函数名: TaskC500Slp
 *   功能描述:  420ms会进来中断一次
 *   参数定义:	
 *	返回值:
 *  说明:
 *   创建人：陶林，2013/06/07
******************************************************************************/
void TaskSystemSleep(const void *pdata)
{	
#if APP_SYSSLEEP_EN 
//	uint8 err;
//	u8 temp=2;
	//OS_CPU_SR  cpu_sr = 0;
//	static uint8 flash_num=0;

//	osEvent event;

	if(TaskHndGet( TASK_FLAG_SLEEP))
	{          
		TaskHndStop(TASK_FLAG_SLEEP);
		//COMMONPend(SysSleepFlag);

		/*GD的芯片深度睡眠起来之后会死机或者晶振起振不正常*/
		#if  1//(MCU_TYPE_SEL==MCU_TYPE_GD32)
		//event = COMMONPend(SysSleepFlag|SOUND_Flag|CODESW_Flag);
		
			
//		if(event.status == osEventSignal)
		{
//			if(event.value.v&CODESW_Flag)
//			{			
//				if(0x03==codeswStat||0x00==codeswStat)
//				{
//					KeyPutOpt(KEY_UNCRED,Key_Down);
//				}
//				else// if(codeswStat == 0x02||codeswStat == 0x01)
//				{
//					KeyPutOpt(KEY_UNCRED,Key_Up);
//				}	
//				HAL_GPIO_ITConfig(FREQ4_PORT,FREQ4_PIN,ENABLE);
//			}
//			if(event.value.v & SOUND_Flag)
//			{
//				SoundExe(SoundStat);
//			}
//			if(event.value.v & SysSleepFlag)
			{
			
				INTF_RFLock();
				SEM_SLPLOCK;	
				AllFlag.SysStat_SleepFlag=1;
		
				if (Str_SyatemSlp.CSLP_Mode == SYSSLP_MODE_CHCK)
				{		
	        		Str_SyatemSlp.CSLP_USlpCnt ++;

	        		if(Str_SyatemSlp.CSLP_USlpCnt >= Func_Str.PowerSwitch*CSLP_UNSLEEP_TIME)
	        		{
		                	Str_SyatemSlp.CSLP_Mode = SYSSLP_MODE_SLP;
		                	Str_SyatemSlp.CSLP_USlpCnt = 0;
	            	}
					if (Str_SyatemSlp.CSLP_SystemSlpStat == SYSSLP_RDA1846SLP)
					{
						Str_SyatemSlp.CSLP_SystemSlpStat = SYSSLP_RDA1846NSLP;
						APP_RDA1846SlpDis();
						APP_SleepTaskSw();
						
						//SQ_Cnt = 0;
						osDelay(20); 
					}
				}
				else if (Str_SyatemSlp.CSLP_Mode == SYSSLP_MODE_SLP)
				{
					if (SlpModeGetStat() == POWERMODE_SLP)
					{
						if (Str_SyatemSlp.CSLP_SystemSlpStat == SYSSLP_RDA1846NSLP)
						{
							Str_SyatemSlp.CSLP_SystemSlpStat = SYSSLP_RDA1846SLP;

							APP_RDA1846SlpEn();							
						}				
					}
	        		Str_SyatemSlp.CSLP_SlpCnt ++;
	        		if(Str_SyatemSlp.CSLP_SlpCnt >=Func_Str.PowerSwitch*CSLP_SLEEP_TIME)// CSLP_SLEEP_TIME * StrMenuSet.PowerMode)
	        		{
	            			Str_SyatemSlp.CSLP_Mode = SYSSLP_MODE_CHCK;
	            			Str_SyatemSlp.CSLP_SlpCnt = 0;
	        		}
				}
				else
				{
					Str_SyatemSlp.CSLP_Mode = SYSSLP_MODE_CHCK;
				}
				APP_SMModeChckClr(PWMD_C5000CHK);
				
				SEM_SLPUNLOCK;
				INTF_RFUnlock();
			}
		}
		

		#endif
	}
#endif	
}




/*****************************************************************************
 *   函数名: APP_RDA1846SlpEn
 *   功能描述:  1846进入低功耗
 *   参数定义:	
 *	返回值:
 *  说明:
 *   创建人：陶林，2013/06/07
******************************************************************************/
void APP_RDA1846SlpEn(void)
{
#if 1

#else
	Set_ReflashRxChannelSwFlagState(0);
	ChangeTimer=0;//切换接收的定时器加快.
	
	RDA1846sPdnDown=1;/*rda1846s进入了Pdn模式*/
	CloseCtcssTimeInt();
	CloseCdcssTimeInt();	
	DH_Drv_ExtiIntSet(EXTI_MODE_CTSDCS,EXTI_DIS);
	MenuVarFlg.g_ChecksignalFlg = 0;
	DRV_RedLedOff();
	DRV_GreenLedOff();
	RDA_RX_DIS_LOW();


#ifdef RDA1846S_PDN
	GPIO_ResetBits(RDA1846S_PDN_PORT, RDA1846S_PDN);
#else
	#if FM_IC_SEL==FM_IC_W12T
	W12T_INTF_Enter_Sleep();

	#elif FM_IC_SEL==FM_IC_RDA1846S
	RDA1846S_INTF_PowerDown();

	#else
	AUCTUS_AT1161SleepCtrl(AT1161_SLEEP_ON);
	
	#endif
	
#endif
#endif
}

extern uint8 g_SqValue;
void APP_RDA1846SlpDis(void)
{
#if 1


#else
	
#ifdef RDA1846S_PDN
	GPIO_SetBits(RDA1846S_PDN_PORT, RDA1846S_PDN);
#else
	#if FM_IC_SEL==FM_IC_W12T
	W12T_INTF_Exit_Sleep();
	#elif FM_IC_SEL==FM_IC_RDA1846S

	RDA1846S_INTF_PowerOn();

	#else
	AUCTUS_AT1161SleepCtrl(AT1161_SLEEP_OFF);
		PLLSetRx();
	
	#endif
#endif

#if 0
	RDA_RX_EN_HIGH();
	DelayXMs(10);
	stat=RdaRead(RDA_REG_30H);	
	RdaWrite(RDA_REG_30H,stat|(1<<5));
	Set_ReflashRxChannelSwFlagState(0);
#else
	Set_ReflashRxChannelSwFlagState(0);
	RDA_RX_EN_HIGH();
	ChangeTimer=0;//切换接收的定时器加快.
	osDelay(10);
	g_SqValue = DRV_Adc_GetAdcQ(CHN_ADC_RF_SQL);
	RDA1846sPdnDown=0;/*rda1846s退出了Pdn模式*/
#endif

#endif
}


void APP_SleepTaskSw(void)
{

}



/*****************************************************************************
 *   功能描述:  
 *   参数定义:	
 *	返回值:TRUE:无信号被检测到,FALSE:检测到信号
 *  说明:
 *   创建人：陶林，2013/06/07
******************************************************************************/
DH_BOOL APP_RDA1846SlpStatGet(void)
{
#if 0
	uint8 data;
	//uint16 Rssi;
    
	HRC5000Receive_cmd(0x51,&data);
			
	if(data&0x03)     /*已同步*/
	{
		Str_SyatemSlp.CSLP_Mode = C5000SLP_MODE_RUN;
		return FALSE;
	}
	else              /*未同步*/
	{
		//Rssi = Rssi_NoiseGet();
		if(CallInStatGet()||CallOutStatGet()//||GloVarFlag.FmRecStat//||DRV_RdaSqStat()
			)//||(RdaRead(0x30)&0x0040)\  //by liendi
		{
			return FALSE;
		}
	}  
#endif	
	return TRUE;
}
void RTC_WKUP_IRQHandler(void)
{

}



/*****************************************************************************
 函 数 名  : PowerDown_Init
 功能描述  : 省电功能初始化
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 修改历史      :
  1.日    期   : 2013年11月29日 星期五
    作    者   : 陈夏冰
    修改内容   : 新生成函数

*****************************************************************************/
void PowerDown_Init(void)
{	
#if 1

#else
	CPU_IntDis();
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP|RCC_APB1Periph_PWR,ENABLE);
	PWR_BackupAccessCmd(ENABLE);		//允许RTC和bkp 控制电源
	//BKP_DeInit();	//不用BKP
	PowerDown_EXTI_Config();			
	PowerDown_NVIC_Config();
	PowerDown_RTC_Config();
	IS_COM_Wake_Up=0;
	BSP_IntVectSet(BSP_INT_ID_RTCAlarm, PowerDown_RTC_IT_Handler);	
	//会导致PTT按键无发射
	//BSP_IntVectSet(BSP_INT_ID_EXTI15_10, PowerDown_EXTI_IT_Handler);	
	RTC_ITConfig(RTC_IT_SEC, DISABLE);
	CPU_IntEn();
#endif
}

/*****************************************************************************
 函 数 名  : PowerDown_EXTI_Config
 功能描述  : EXTI初始化
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年11月29日 星期五
    作    者   : 陈夏冰
    修改内容   : 新生成函数

*****************************************************************************/
void PowerDown_EXTI_Config(void)
{
#if 1

#else
	EXTI_InitTypeDef EXTI_InitStructure;

	//RTC 做中断唤醒
	EXTI_ClearITPendingBit(EXTI_Line17);
	EXTI_InitStructure.EXTI_Line=EXTI_Line17;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising;	//上升触发 
	EXTI_Init(&EXTI_InitStructure);

	//UART 接收做中断
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource10);
	EXTI_InitStructure.EXTI_Line=EXTI_Line10;	
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising_Falling;	//上升下降触发 
	EXTI_Init(&EXTI_InitStructure);
#endif	
}

/*****************************************************************************
 函 数 名  : PowerDown_NVIC_Config
 功能描述  : NVIC
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年11月29日 星期五
    作    者   : 陈夏冰
    修改内容   : 新生成函数
*****************************************************************************/
void PowerDown_NVIC_Config(void)
{
#if 1

#else
	NVIC_InitTypeDef NVIC_InitStructure;

	//RTC 做中断唤醒
	NVIC_InitStructure.NVIC_IRQChannel = RTCAlarm_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionscanIndexity = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubscanIndexity= 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);		

	#if 0
	//UART 接收做中断
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionscanIndexity = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubscanIndexity= 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
	#endif
#endif	
}

/*****************************************************************************
 函 数 名  : PowerDown_RTC_Config
 功能描述  : RTC初始化
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年11月29日 星期五
    作    者   : 陈夏冰
    修改内容   : 新生成函数

*****************************************************************************/
void PowerDown_RTC_Config(void)
{
#if 1

#else	
	//使能内部低速时钟作为RTC 时钟
	RCC_LSICmd(ENABLE);	
	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
	RCC_RTCCLKCmd(ENABLE);
	RTC_WaitForSynchro();
	RTC_WaitForLastTask();
	
	RTC_ITConfig(RTC_IT_ALR, ENABLE);
	RTC_WaitForLastTask();
	//40khz 的LSI  预分频值为39999  实现 秒级定时
	// 设为2000 的精度为50ms 不精确
	RTC_SetPrescaler(1999);
	RTC_WaitForLastTask();	
#endif
}

/*****************************************************************************
 函 数 名  : PowerDown_Start
 功能描述  : 进入睡眠
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年11月29日 星期五
    作    者   : 陈夏冰
    修改内容   : 新生成函数
*****************************************************************************/
void PowerDown_Start(u8 wakeuptime)
{
#if 1

#else
	uint32 i=0;
#if OS_CRITICAL_METHOD == 3/* Allocate storage for CPU status register           */
	OS_CPU_SR  cpu_sr = 0;
#endif
	OS_ENTER_CRITICAL();
	//防止RTC 用作second 影响闹钟	
	RTC_ClearFlag(RTC_FLAG_SEC);
	//没有用作计时且需要等50ms去掉
	//while(RTC_GetFlagStatus(RTC_FLAG_SEC)==RESET);
	//设置闹钟时间M*50ms
	RTC_SetAlarm(RTC_GetCounter()+wakeuptime-1);
	RTC_WaitForLastTask();	
	OS_EXIT_CRITICAL();
	//进入睡眠stop模式 WFI

	OSSchedLock();
	PWR_EnterSTOPMode(PWR_Regulator_LowPower,PWR_STOPEntry_WFI);

	for(i=0;i<100;i++);//延时一阵.
	
	//PWR_EnterSTANDBYMode();
	//唤醒后必须重新初始化时钟
        if(AllFlag.SysStat_SleepFlag)//还处于睡眠模式才需要
	{//如果已经被其他的方式唤醒过了就不要了,
		CPU_IntDis();
		BSP_Init();
		CPU_IntEn();
        }
	else
	{//此处把时钟的分频系数还原回去.
		if(AllFlag.WakeupFromDeepSleepOpeOkFlag==0)
		{
			WakeUpFromDeepSleepOperation();
		}
	}
	OSSchedUnlock();
#endif
}

/*****************************************************************************
 函 数 名  : PowerDown_RTC_IT_Handler
 功能描述  : RTC闹钟中断处理
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年11月29日 星期五
    作    者   : 陈夏冰
    修改内容   : 新生成函数

*****************************************************************************/
void PowerDown_RTC_IT_Handler(void)
{
#if 1

#else
	if(RTC_GetITStatus(RTC_IT_ALR)!=RESET)
	{
		//清除一些flag 以便能再次进入中断
		EXTI_ClearITPendingBit(EXTI_Line17);
		if(PWR_GetFlagStatus(PWR_FLAG_WU)!=RESET)
		{	
			PWR_ClearFlag(PWR_FLAG_WU);
		}
		RTC_WaitForLastTask();
		RTC_ClearITPendingBit(RTC_IT_ALR);
		RTC_WaitForLastTask();
	}
	IS_COM_Wake_Up=0;
	AllFlag.WakeupFromRTCFlag=1;
#endif
}
/*****************************************************************************
 函 数 名  : PowerDown_EXTI_IT_Handler
 功能描述  : UART改为外部中断
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年11月29日 星期五
    作    者   : 陈夏冰
    修改内容   : 新生成函数

*****************************************************************************/
void PowerDown_EXTI_IT_Handler(void)
{
#if 1

#else
	if(EXTI_GetITStatus(EXTI_Line10)!=RESET)
	{	
		//清除标志位
		EXTI_ClearFlag(EXTI_Line10);
		EXTI_ClearITPendingBit(EXTI_Line10);
		IS_COM_Wake_Up=1;
	}
	SlpModeExitSM();
#endif
}














//400ms
void sys_tick_exta(void)
{
//	static u8 cnt=0;
///	u8 err;
	if((GloVarFlag.RadioFlag == 1)||(Scanflag&0x08)||g_FpgaTx||FM_APP_GetSqStatus())
	{
	//	cnt=0;
	}
	else	if (SlpModeGetStat()== POWERMODE_SLP)
	{
		//if(cnt++>100)
		{
		//	cnt=0;

			TaskSystemSleepAct();
		}
	}
	//debug_tim++; 
}



void  Re_Enable_RTCclockAfterWakeUp(void)
{//唤醒之后重新打开秒时钟.
#if  0
	/* Enable PWR and BKP clocks */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

	/* Allow access to BKP Domain */
	PWR_BackupAccessCmd(ENABLE);

	///* Reset Backup Domain */
	//BKP_DeInit();

	/* Enable LSI */
	RCC_LSICmd(ENABLE);
	/* Wait till LSI is ready */
	while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
	{}

	/* Select LSI as RTC Clock Source */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);

	/* Enable RTC Clock */
	RCC_RTCCLKCmd(ENABLE);

	/* Wait for RTC registers synchronization */
	RTC_WaitForSynchro();

	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();

	RTC_ITConfig(RTC_IT_ALR, ENABLE);
	RTC_WaitForLastTask();
	
	/* Set RTC prescaler: set RTC period to 1sec */
	RTC_SetPrescaler(39999); /* RTC period = RTCCLK/RTC_PR = (40KHZ KHz)/(32767+1) */
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();

	INTF_RtcIntEn();









#else




	//使能内部低速时钟作为RTC 时钟
//	RCC_LSICmd(ENABLE);	
//	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
//	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
//	RCC_RTCCLKCmd(ENABLE);
//	RTC_WaitForSynchro();
//	RTC_WaitForLastTask();
//	
//	RTC_ITConfig(RTC_IT_ALR, ENABLE);
//	RTC_WaitForLastTask();
//	RTC_SetPrescaler(39999);
//	RTC_WaitForLastTask();	




//	
//	INTF_RtcIntEn();
	#endif
}


