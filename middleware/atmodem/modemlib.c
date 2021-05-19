/****************************************Copyright (c)**************************************************
**                               	____科技有限公司
**                                     		开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: modemlib.c
**创   建   人: yzy
**最后修改日期: 2007年3月3日
**描        述: 各个模块的驱动函数集
**注        意:
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2007年3月3日
** 描　述: 原始版本
********************************************************************************************************/
#define EXT_MODEMLIB

#include "public.h"
#include "k_types.h"
#include "k_api.h"
//	#include "lib_app.h"
#include "hal.h"
#include <sal_import.h>
#include "atmodem.h"

#include "modemlib.h"



//	/************************************************************************
//	 * @function: LineLen
//	 * @描述: 从缓存中获取第一行数据的长度(包括一个结尾符)
//	 * 
//	 * @参数: 
//	 * @param: buffer 缓存
//	 * @param: len 缓存数据长度
//	 * @param: ebytes 结尾符长度
//	 * 
//	 * @返回: 
//	 * @return: uint8  行长度(包括一个结尾符)
//	 *              0表示无数据或未找到行结束符,1表示仅读到结尾符
//	 * @说明: 
//	 * @作者: yzy (2014/11/13)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
//	 ************************************************************************/
//	uint16 LineLen(uint8* buffer, uint16 len, uint16* ebytes)
//	{
//	    uint8 i;
//	    uint8 getline = false;          //是否已经找到行
//	    uint16 ret = 0;
//	    
//	    for(i = 0; i < len; i++)        //循环判断
//	    {
//	        if((buffer[i] == '\r') || (buffer[i] == '\n'))
//	        {
//	            if(!getline)            //找到第一个行结束符
//	            {
//	                getline = true;
//	                ret = i;
//	            }
//	        }
//	        else
//	        {
//	            if(getline)
//	            {
//	                break;
//	            }
//	        }
//	    }
//	
//	    if(!getline)                    //未找到行
//	    {
//	        return 0;
//	    }
//	    
//	    if(ebytes != __NULL)            //找到行,输出在输入范围内找到的结尾符数量
//	    {
//	        *ebytes = i - ret;
//	    }
//	    
//	    return ret + 1;
//	}
//	
//	
//	
//	/************************************************************************
//	 * @function: GetOneChar
//	 * @描述: 从串口缓存中的当前行中读取一个ASCII字符形式的数字
//	 * 
//	 * @参数: 
//	 * @param: rbyte 存放最后读取的那个字节(256表示串口已无数据可读)
//	 * 
//	 * @返回: 
//	 * @return: uint16  0-0xFFFE 转换好的数字
//	 *                  0xFFFF 没有找到可转换的字符
//	 * @说明: 
//	 * @作者: yzy (2014/11/13)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
//	 ************************************************************************/
//	uint16 GetOneChar(uint16* rbyte)
//	{
//		uint16 byte;							//....
//		uint16 value = 0xFFFF;				    //默认返回非法值
//		bool isbcd = false;						//默认没有找到数字	
//		
//		while(1)
//		{
//			byte = ReadNextByte();				//从串口中读取下一个字节
//	        
//			if(IsBcdChar(byte))					//判断是否是数字
//			{
//				if(!isbcd)						//判断是否是第一数字
//				{
//					value = byte - '0';			//赋值	
//					isbcd = true;
//				}
//				else
//				{
//					value *= 10;				//移位后赋值
//					value += byte - '0';
//				}			
//			}
//			else
//			{
//	//	            if(isbcd && ((byte == 0x0D) || (byte == 0x0A)))//读到行尾,不管有没有扫描到数字,都退出
//	//	            {
//	//	                break;
//	//	            }
//	            
//				if(isbcd)						//已经扫描到过数字了
//				{
//					break;						//退出
//				}
//												//串口已经没数据了退出								
//				if(byte > 255)
//				{
//					byte = 256;
//					break;
//				}
//			}
//		}
//		
//		*rbyte = byte;							//获取当前读取的字节，用以判断当前行是否完结	
//		return value;							//返回最终结果
//	}
//	
//	
//	
//	
//	
//	
//	/************************************************************************
//	 * @function: DecodeRecvDataNone
//	 * @描述: 去除这一行中剩下的数据
//	 * 
//	 * @参数: 
//	 * @param: mrfb 处理后的信息结构(该函数用不到)
//	 * @返回: 
//	 * @说明: 
//	 * @作者: yzy (2014/11/13)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
//	 ************************************************************************/
//	void DecodeRecvDataNone(ModemRecvFeedBack* mrfb)
//	{
//		uint16 byte;						//定义从串口读出的字节
//		
//		while(1)
//		{
//			byte = ReadNextByte();			//读取下一个字节
//			if(byte == 0x0D || byte == 0x0A)//这一行读完则退出
//			{
//				break;
//			}
//			
//			if(byte > 255)					//所有的数据都读完也退出
//			{
//				break;
//			}	
//		}
//	}
//	
//	
//	
//	/************************************************************************
//	 * @function: DecodeRecvDataOneChar
//	 * @描述: 从串口缓存的第一行中解析一个数字,然后去除这一行中剩下的数据
//	 * 
//	 * @参数: 
//	 * @param: mrfb 处理后的信息结构
//	 * @返回: 
//	 * @说明: 
//	 * @作者: yzy (2014/11/13)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
//	 ************************************************************************/
//	void DecodeRecvDataOneChar(ModemRecvFeedBack* mrfb)
//	{	
//		uint16 byte;                 
//											//获取一个字节的信息
//		mrfb->proced = (void*)GetOneChar(&byte);			
//		
//		if((byte != 0x0D) && (byte != 0x0A))//如果还没有到行尾
//		{
//			DecodeRecvDataNone(mrfb);		//去除本行剩余的无效信息
//		}
//	}
//	
//	
//	
//	
//	/************************************************************************
//	 * @function: DecodeRecvDataTwoChar
//	 * @描述: 从串口缓存的第一行中解析二个数字,然后去除这一行中剩下的数据
//	 * 
//	 * @参数: 
//	 * @param: mrfb 处理后的信息结构
//	 * @返回: 
//	 * @说明: 
//	 * @作者: yzy (2014/11/13)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
//	 ************************************************************************/
//	void DecodeRecvDataTwoChar(ModemRecvFeedBack* mrfb)
//	{
//		uint16 byte;
//		Word32 wd;
//		                 
//		wd.byte[0] = GetOneChar(&byte);		//获取第一个数字
//		wd.byte[1] = GetOneChar(&byte);		//获取第二个数字
//		
//		mrfb->proced = (void*)(int)wd.lword;		//获取数据
//		
//		if((byte != 0x0D) && (byte != 0x0A))//去除本行剩余的无效信息
//		{
//			DecodeRecvDataNone(mrfb);		//....
//		}
//	}
//	
//	
//	
//	/************************************************************************
//	 * @function: DecodeAndEnqueuePduMsg
//	 * @描述: 解析PDU短信
//	 * 
//	 * @参数: 
//	 * @param: rbyte 当前行的第一个ASCII码数字
//	 * 
//	 * @返回: 
//	 * @return: bool  true 得到了一个完整的短信帧
//	 *                false 解析失败
//	 * @说明: 
//	 * @作者: yzy (2014/11/13)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
//	 ************************************************************************/
//	bool DecodeAndEnqueuePduMsg(uint8 rbyte)
//	{
//		uint8 byte;
//		uint8 result;
//		uint8 length = 0;
//		int16 uc_i;
//											
//		rbyte = (HexCharToValue(rbyte)<< 4);//转换第一个字节
//											//转换第二个字节，然后去掉短信中心
//		if((byte = HexCharToValue(ReadNextByte())) == 0xFF)
//		{
//			return false;
//		}
//		
//		rbyte+= byte;						//短信中心的长度
//		rbyte++;
	
//		while(rbyte--)						//去除短信中心
//		{
//			(void)ReadNextByte();			
//			(void)ReadNextByte();
//		}
//		
//		rbyte = HexCharToValue(ReadNextByte()) << 4;
//		rbyte += HexCharToValue(ReadNextByte());
//		
//		if(rbyte > 16)						//对方号码不能太长
//		{
//			return false;					//返回错误
//		}
//		
//		(void)ReadNextByte();				//消除两个字节	
//		(void)ReadNextByte();				
//		for(;length < rbyte; length++)		//获取对方短信号码
//		{
//			if(length & 0x01)				//高低字节互换
//			{
//				DevModem->smsRev->number[length-1]=ReadNextByte();    //拷贝字符串
//			}
//			else
//			{
//				DevModem->smsRev->number[length+1]=ReadNextByte();    //拷贝字符串
//			}
//		}
//		if(rbyte & 0x01)
//		{
//			DevModem->smsRev->number[length-1]=ReadNextByte();    //拷贝字符串
//		}
//		
//		DevModem->smsRev->numlength = rbyte;	//目标号码的长度
//		
//		
//		(void)ReadNextByte();				//消除两个字节	
//		(void)ReadNextByte();				
//		
//		rbyte = HexCharToValue(ReadNextByte()) << 4;
//		rbyte += HexCharToValue(ReadNextByte());
//		
//		DevModem->smsRev->fmt = rbyte;     //短信格式  
//		
//		rbyte = 14;
//		
//		while(rbyte--)						//消除14个字节的时间
//		{
//			ReadNextByte();
//		}
//											//获取短信的长度
//		rbyte = HexCharToValue(ReadNextByte()) << 4;
//		rbyte += HexCharToValue(ReadNextByte());	
//		
//		if(rbyte > 140)						//短信长度不能超过140字节
//		{
//			return false;					//返回错误	
//		}
//		DevModem->smsRev->smslength=rbyte; //数据长度
//		                                    //7bit的8bit长度
//		if((DevModem->smsRev->fmt & 0x0F) == 0)
//		{
//		    rbyte = ((rbyte * 7) >> 3) + ((rbyte * 7 & 0x07) ? 1 : 0);
//		}
//		
//		length = 0;	
//		 						
//		while(rbyte--)
//		{
//			byte = ReadNextByte();			//逐字读取		
//			if(!IsHexChar(byte))			//直到非HexChar为止
//			{
//				break;
//			}
//			result = HexCharToValue(byte) << 4;		
//					
//			byte = ReadNextByte();			//读取低四位的形式
//			if(!IsHexChar(byte))			//安全性判断
//			{
//				break;
//			}
//			result += HexCharToValue(byte);	//加上第四位
//											//增加一个字节
//	
//			DevModem->smsRev->sms[length++]=result;      //转移短信内容  
//		}
//		
//		if(rbyte == 0xFF)					//判断是否是一条完整的短信
//		{	    
//		                                    //7bit格式的转换
//		    if((DevModem->smsRev->fmt & 0x0F) == 0)
//		    {
//		                                    //获取7bit的数据各保存为1个字节	                                        	        	        
//		        for(uc_i = DevModem->smsRev->smslength - 1; uc_i >= 0; uc_i--)
//		        {
//		                                    //1个字节1个字节的获取
//		            DevModem->smsRev->sms[uc_i] = 
//		                STR_GetBits(DevModem->smsRev->sms, 7, 7 * uc_i);
//		        }
//		    }
//			return true;					//....
//		}
//		return false;						//返回错误
//		
//	}


/************************************************************************
 * @function: GetOneChar
 * @描述: 从串口缓存中的当前行中读取一个ASCII字符形式的数字
 * 
 * @参数: 
 * @param: rbyte 存放最后读取的那个字节(256表示串口已无数据可读)
 * 
 * @返回: 
 * @return: uint16  0-0xFFFE 转换好的数字
 *                  0xFFFF 没有找到可转换的字符
 * @说明: 
 * @作者: yzy (2014/11/13)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8_t * GetOneCharAtStr(uint8_t * rbyte, uint8_t *val)
{
	uint8_t byte = 0;							//....
	uint16 value = 0xFFFF;				    //默认返回非法值
	bool isbcd = false;						//默认没有找到数字	
	uint8_t * p = rbyte;
	while(p !=  NULL)
	{
		byte = *p++;//ReadNextByte();				//从串口中读取下一个字节
		if(IsBcdChar(byte))					//判断是否是数字
		{
			if(!isbcd)						//判断是否是第一数字
			{
				value = byte - '0';			//赋值	
				isbcd = true;
			}
			else
			{
				value *= 10;				//移位后赋值
				value += byte - '0';
			}			
		}
		else
		{
            
			if(isbcd)						//已经扫描到过数字了
			{
				break;						//退出
			}
											//串口已经没数据了退出								
//				if(byte == '\r' || byte == '\n')
//				{
//					byte = 256;
//					break;
//				}
		}
	}
	*val = (uint8_t)value;
	return p;							//返回最终结果
}
uint8_t * GetOneShortAtStr(uint8_t * rbyte, uint16_t *val)
{
	uint8_t byte = 0;							//....
	uint16 value = 0xFFFF;				    //默认返回非法值
	bool isbcd = false;						//默认没有找到数字	
	uint8_t * p = rbyte;
	while(p !=  NULL)
	{
		byte = *p++;//ReadNextByte();				//从串口中读取下一个字节
		if(IsBcdChar(byte))					//判断是否是数字
		{
			if(!isbcd)						//判断是否是第一数字
			{
				value = byte - '0';			//赋值	
				isbcd = true;
			}
			else
			{
				value *= 10;				//移位后赋值
				value += byte - '0';
			}			
		}
		else
		{
            
			if(isbcd)						//已经扫描到过数字了
			{
				break;						//退出
			}
											//串口已经没数据了退出								
//				if(byte == '\r' || byte == '\n')
//				{
//					byte = 256;
//					break;
//				}
		}
	}
	*val = (uint16_t)value;
	return p;							//返回最终结果
}
uint8_t * GetOneLongAtStr(uint8_t * rbyte, uint32_t *val)
{
	uint8_t byte = 0;							//....
	uint32_t value = 0xFFFF;				    //默认返回非法值
	bool isbcd = false;						//默认没有找到数字	
	uint8_t * p = rbyte;
	while(p !=  NULL)
	{
		byte = *p++;//ReadNextByte();				//从串口中读取下一个字节
		if(IsBcdChar(byte))					//判断是否是数字
		{
			if(!isbcd)						//判断是否是第一数字
			{
				value = byte - '0';			//赋值	
				isbcd = true;
			}
			else
			{
				value *= 10;				//移位后赋值
				value += byte - '0';
			}			
		}
		else
		{
            
			if(isbcd)						//已经扫描到过数字了
			{
				break;						//退出
			}
											//串口已经没数据了退出								
//				if(byte == '\r' || byte == '\n')
//				{
//					byte = 256;
//					break;
//				}
		}
	}
	*val = (uint32_t)value;
	return p;							//返回最终结果
}

int DecodeStrTwoChar(uint8_t *rcv, uint8_t * out)
{
    uint8_t * p = rcv;
	if(p != NULL)
    {   
	    p = GetOneCharAtStr(p, &(out[0]));		//获取第一个数字
    }
    else
    {
        return -1;
    }
    
	if(p != NULL)
    {   
	    p = GetOneCharAtStr(p, &(out[1]));		//获取第一个数字
    }
    else
    {
        return -1;
    }
    
    return 0;
}
int DecodeStrFourChar(uint8_t *rcv, uint8_t * out)
{
    uint8_t * p = rcv;
    uint8_t val = 0;
	if(p != NULL)
    {   
	    p = GetOneCharAtStr(p, &val);		//获取第一个数字
	    out[0] = val;
    }
    else
    {
        return -1;
    }
    
	if(p != NULL)
    {   
	    p = GetOneCharAtStr(p, &val);		//获取第一个数字
	    out[1] = val;
    }
    else
    {
        return -1;
    }

	if(p != NULL)
    {   
	    p = GetOneCharAtStr(p, &val);		//获取第一个数字
	    out[2] = val;
    }
    else
    {
        return -1;
    }

	if(p != NULL)
    {   
	    p = GetOneCharAtStr(p, &val);		//获取第一个数字
	    out[3] = val;
    }
    else
    {
        return -1;
    }

    return 0;
}

/************************************************************************
 * @function: DecodeRecvDataOneChar
 * @描述: 从串口缓存的第一行中解析一个数字,然后去除这一行中剩下的数据
 * 
 * @参数: 
 * @param: mrfb 处理后的信息结构
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/11/13)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
int DecodeStrOneChar(uint8_t *rcv, uint8_t * out)
{	
	if(rcv != NULL)
    {   
	    GetOneCharAtStr(rcv, out);		//获取第一个数字
    }
    else
    {
        return -1;
    }
    
    return 0;
}

/************************************************************************
 * @function: DecodeRecvDataOneChar
 * @描述: 从串口缓存的第一行中解析一个数字,然后去除这一行中剩下的数据
 * 
 * @参数: 
 * @param: mrfb 处理后的信息结构
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/11/13)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
int DecodeStrOneShort(uint8_t *rcv, uint16_t * out)
{	
	if(rcv != NULL)
    {   
	    GetOneShortAtStr(rcv, out);		//获取第一个数字
    }
    else
    {
        return -1;
    }
    
    return 0;
}

/************************************************************************
 * @function: DecodeRecvDataOneChar
 * @描述: 从串口缓存的第一行中解析一个数字,然后去除这一行中剩下的数据
 * 
 * @参数: 
 * @param: mrfb 处理后的信息结构
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/11/13)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
int DecodeStrOneLong(uint8_t *rcv, uint32_t * out)
{	
	if(rcv != NULL)
    {   
	    GetOneLongAtStr(rcv, out);		//获取第一个数字
    }
    else
    {
        return -1;
    }
    
    return 0;
}

