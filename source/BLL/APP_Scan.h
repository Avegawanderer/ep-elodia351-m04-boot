#ifndef _APP_SCAN_H
#define _APP_SCAN_H



#define MENUSETSCANMODE_TO  0 //时间
#define MENUSETSCANMODE_SO  1//载波
#define MENUSETSCANMODE_SE   2



#define SCAN_TIMEMODE_REMAIN_TIME	10/*时间模式时的乘的倍数.300*17=5100ms*/
#define SCAN_REMAIN_TIME_MAX	17/*有信号保持时间*/
#define SCAN_SUB_REMAIN_TIME	10/*有亚音频接收时保持时间(方便解亚音频)*/



#define SCAN_MODE_ChannelList		1//默认的就是列表扫描.
#define SCAN_MODE_FreqChange		2//扫频模式.



#define SCAN_ON   		1
#define SCAN_OFF  	2
#define SCAN_ONOF 	3


//扫描空间的安排.前10个字节是一些是，扫描的参数和配置.
//每个扫描信道都包含了2个字节,Buf[0]=区域号,Buf[0]=频道号,
#define SCAN_VALID_LISTNUM		E2PROM_SCANBASEADDR	//扫描列表的有效的个数
#define SCAN_VALID_LISTNUMH		SCAN_VALID_LISTNUM+0//是哪个位代表哪个列表有效.
#define SCAN_VALID_LISTNUML		SCAN_VALID_LISTNUM+1
#define SCAN_scanIndex_FLAG_ADDR	SCAN_VALID_LISTNUM+2//有优先信道的标志.存放位置
#define SCAN_scanIndex_CH_ADDR		SCAN_VALID_LISTNUM+3//优先信道号码存放位置.
#define SCAN_scanIndex_CH_ITEM		SCAN_VALID_LISTNUM+4//优先信道的对应序号0-255

#define SCAN_FREQODE_SAVED_BASEADDR	SCAN_VALID_LISTNUM+10//扫频模式的时候保存扫描的频率值4个字节,收发同频.
//大小4个字节,




#define SCAN_ONECH_LEN			2//一个扫描信道的大小2字节,
#define SCAN_LIST_scanIndex_FLAG		40//优先信道有无的标志.(第40个字节.)
#define SCAN_LIST_scanIndex_CH		41//优先信道号码
#define SCAN_scanIndex_FLAG			0xA5//有优先信道的标志.
#define SCAN_LIST_VALID_FLAG		0XAA//扫描信道有效的标记




#define SCAN_LIST_BASE		(SCAN_VALID_LISTNUM+20)
#define SCAN_LIST_LEN		(512)

extern uint8 g_presentchn;
extern uint8 g_remainflag;
extern uint8 Scanflag;/*扫到信号的标志位，停留时间的标志位*/
//extern uint16 ScanListValue;
extern uint8 PreScanListValue;//备用的扫描信道的个数.

typedef struct {
uint8 ScanMode;
uint8 scanIndex;//扫描序号递增或递减
uint8 priFlag;//优先扫描标志，对应信道0的扫描位
uint8 ctcdmode;//检测亚音模式
uint8 remaintime;//by liendi	/*读取上位机扫描停留时间*/
uint8 sendmode;//扫描发射模式
uint8 callback;//扫描回讲
}STR_SCAN;


void ScanDispTopExit(void);


void ListChnSet(uint8 CCnt);
void ChnListScan(void);

void ScanStart(void);
void ScanStop(void);
uint8 ScanCtrlGet(void);
void ScanCtrlSet(uint8 stat);


#endif


