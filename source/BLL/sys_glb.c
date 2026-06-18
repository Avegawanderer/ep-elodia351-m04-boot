
#include "app_config.h"
#include "sys_inc.h"
#include <string.h>

//int power(int base, int n)
//{
//	int i, p;
//	p = 1;
//	for(i = 1; i <= n; ++i)
//		p = p * base;
//	return p;
//
//}

void int2hex(uint32 value,uint8* buf)
{
	uint8 temp;
	uint8 leng=0,i;
	buf[0]='0';
	buf[1]='\0';
	while(value){
		for(i=leng;i>0;i--)buf[i] = buf[i-1];
		temp = value%16;
		value /= 16;
		if(temp>9)buf[0] = 'a'+(temp-10);
		else buf[0] = temp + '0';
		leng++;
		buf[leng]='\0';
	}
}
uint8* int2Str(uint32  value,uint8* buf)
{
//	uint8 temp;
	uint8 len=0,i;
	buf[0]='0';
	buf[1]='\0';
	while(value){
		for(i=len;i>0;i--)buf[i] = buf[i-1];
		buf[0] = '0'+value%10;
		value /= 10;
		len++;
		buf[len]='\0';
	}
	return buf;
}

uint32_t strtoi_len(uint8_t *buf, uint8_t len)
{
    uint32_t result = 0;
    uint8_t i;
    for(i = 0; i < len; i++)
    {
        result *= 10;
        result += (buf[i] - '0');
    }
    return result;
}

void int2hex_len(uint8* buf,uint32 num,uint8_t len)
{
	uint8_t i,temp;
	for(i = 0; i < len; i++)
	{
		temp = num%16;
		num /= 16;
		if(temp>9)buf[i] = 'a'+(temp-10);
		else buf[i] = temp + '0';
	}
	buf[len]= 0;
	for(i = 0; i < len/2; i++)
	{
		temp = buf[i];
		buf[i]=buf[len-1-i];
		buf[len-1-i] = temp;
	}
}


void itostr_len(uint8_t *buf, uint32_t num, uint8_t len)
{
	uint8_t i,temp;
	for(i = 0; i < len; i++)
	{
		buf[i] = num%10+'0';
		num /= 10;
	}
	buf[len]= 0;
	for(i = 0; i < len/2; i++)
	{
		temp = buf[i];
		buf[i]=buf[len-1-i];
		buf[len-1-i] = temp;
	}
}
void itostr(uint8_t *buf, uint32_t num)
{
	uint32_t numtemp = num;
	uint8_t i,temp;
	uint8_t len = 0;
	while(numtemp){len++;numtemp/=10;}

	if(len==0) len=1;	

	
	numtemp = num;
	for(i = 0; i < len; i++)
	{
		buf[i] = numtemp%10+'0';
		numtemp /= 10;
	}
	buf[len]= 0;
	for(i = 0; i < len/2; i++)
  {
		temp = buf[i];
		buf[i]=buf[len-1-i];
		buf[len-1-i] = temp;
  }
}

void strAppend(uint8_t *buf, uint8_t* str)
{
	while(*buf !=0) buf++;
	while(*str !=0) *buf++ = *str++;
	*buf = 0;
}
void strCopy(uint8_t *buf, uint8_t* str)
{
	while(*str !=0) *buf++ = *str++;
	*buf = 0;
}


void CheckDispNum0(uint8_t *buf)
{
	while(*buf == '0')
	{
		if(*(buf+1)!='.') *buf = ' ';
		buf++;
	}
}

uint16_t getNextIndex(uint8_t upDwn,uint16_t curValue,uint16_t Item_start,uint16_t Item_Max)
{
	uint16_t nextValue = curValue;
	if(upDwn)
	{
		nextValue++;
		if(nextValue>Item_Max)nextValue = Item_start;
	}else{
		if(nextValue<=Item_start)nextValue = Item_Max;
		else  nextValue--;
	}
	return nextValue;
}
/*****************************************************************************
 *   函数名: GetBitsBuf
 *   功能描述:  返回buf 并长度为Len的数组中bit的位数。
 *   参数定义:
 *	 返回值:
 *   创建人：陶林，2013/06/15
******************************************************************************/
//uint16_t GetBitsBuf(uint8_t* Buf, uint16_t Len)
//{
//	uint16_t i, j;
//	uint16_t BitCnt=0;
//
//	for (i=0; i<Len; i++)
//	{
//		for (j=0; j<8; j++)
//		{
//			if(Buf[i]&(1<<j))
//			{
//				BitCnt++;
//			}
//		}
//	}
//	return BitCnt;
//}

/*****************************************************************************
 *   函数名: ResetBitsBuf
 *   功能描述:  将bit cnt对应位置0(从0开始)
 *   参数定义:
 *	 返回值:
 *   创建人：陶林，2013/06/15
******************************************************************************/
//uint8_t ResetBitsBuf(uint16_t Cnt, uint8_t* Buf, uint16_t Len)
//{
//	if (Cnt>= Len*8)
//	{
//		return 1;
//	}
//	Buf[Cnt/8] &= ~(1<<(Cnt%8));
//
//	return 0;
//}

/*****************************************************************************
 *   函数名: ResetBitsBuf
 *   功能描述:  查找到一个bit=0的位将其置一，并返回其所在bit位，从0开始。
 *   参数定义:
 *	 返回值:
 *   创建人：陶林，2013/06/15
******************************************************************************/
//uint8_t SetBitsBuf(uint8_t* Buf, uint16_t Len, uint8_t* err)
//{
//	uint16_t i, j;
//
//	for (i=0; i<Len; i++)
//	{
//		for (j=0; j<8; j++)
//		{
//			if ((Buf[i]&(1<<j))==0)
//			{
//				Buf[i] |= (1<<j);
//				*err = 0;
//				return (i*8+j);
//			}
//		}
//	}
//	*err = 1;
//	return Len*8;
//}


uint8_t getcharnum(char *buf,char c)
{
	uint8_t num = 0;
	while(*buf != '\0')
	{
		if(*buf == c) num++;
		buf++;
	}
	return num;
}
/*******************************************************************************
 * 函数名  : list_GetValidIndex
 * 描  述  :  获取第item有效位所对应的实际区域号
 * 输  入  :  参数 index: 1~
 *
 * 输  出  :  无
 * 返回值  : 有效区域的序号 0~
 *******************************************************************************/
uint8_t list_GetValidIndex(uint8_t item,LIST_MG_STR* str)
{
	uint8_t i;
	uint8_t y = 0;
	for(i=0;i<str->size;i++){
		if(str->grp[i/8] & BITSET(i%8))
		{
			if(y == item) return i;
			y++;
		}
	}
	return 0xff;
}

/*获取下一个有效序号*/
uint8_t list_GetNextValidIndex(LIST_MG_STR* str,uint8_t curIndex,uint8_t dir)
{
	if(str->valid==0) return 0;
	else if(str->valid>1)
	{
		do {
			curIndex = getNextIndex(dir,curIndex,0,str->size-1);
		} while (!(str->grp[curIndex/8] & BITSET(curIndex%8)));
	}
	return curIndex;
}


uint8_t list_GetFirstValidIndex(LIST_MG_STR* str)
{
	uint8_t i;
	for(i=0;i<str->size;i++){
		if(str->grp[i/8] & BITSET(i%8))
		{
			return i;
		}
	}
	return 0xff;
}
/*******************************************************************************
 * 函数名  : list_GetValidItem
 * 描  述  :  获取列表中第index(此区域有效)前有多少有效区域
 * 输  入  :  参数 index: 1~
 *
 * 输  出  :  无
 * 返回值  : 有效区域的个数
 *******************************************************************************/
uint8_t list_GetValidItem(uint8_t index,LIST_MG_STR* str)
{
	uint8_t i;
	uint8_t y = 0;
	for(i=0;i<str->size;i++){
		if(str->grp[i/8] & BITSET(i%8))
		{
		    if(i==index) return y;
			y++;
		}
	}
	return 0;
}

uint8_t list_GetFirstInValidIndex(LIST_MG_STR* str)
{
	uint8_t i;
	for(i=0;i<str->size;i++){
		if((str->grp[i/8] & BITSET(i%8))==0)
		{
			return i;
		}
	}
	return 0xFF;
}


DH_BOOL list_AddValidIndex(uint8_t index,LIST_MG_STR* str)
{
	if(index < str->size && !(str->grp[index/8] & BITSET(index%8)))
	{
		str->grp[index/8] |= BITSET(index%8);
		str->valid ++;
		return TRUE;
	}
	return FALSE;
}

DH_BOOL list_DelValidIndex(uint8_t index,LIST_MG_STR* str)
{
	if(index < str->size && (str->grp[index/8] & BITSET(index%8)))
	{
		str->grp[index/8] &= BITCLR(index%8);
		str->valid --;
		return TRUE;
	}
	return FALSE;
}

//type: 0 mid;1 left 2 right
//void adjustdispAlian(uint8_t *namebuf,uint8_t displen,uint8_t type)
//{
//	uint8_t len = strlen((const char*)namebuf);
//	uint8_t leftstart = 0;
//
//	//printk("%s %d\r\n",namebuf,len);
//	if(displen<len) len = displen;
//
//	uint8_t tempbuf[len];
//	memcpy((void*)tempbuf,(void*)namebuf,len);
//	memset((void*)namebuf,' ',displen);
//	if(type == 0)//mid
//	{
//		leftstart = (displen - len)>>1;
//	}
//	else if(type == 1)//left
//	{
//		leftstart = 0;
//	}
//	else if(type == 2)
//	{
//		leftstart = displen - len;
//	}
//	memcpy(&namebuf[leftstart],tempbuf,len);
//
//	namebuf[displen] = '\0';
//}
/*********************************************************************************************************
 **                            End Of File
 ********************************************************************************************************/
