/****************************************Copyright (c)*************************
**                               	______科技有限公司
**                                     		开发部
**
**
**--------------文件信息-------------------------------------------------------
**文   件   名: dev_uart.c
**创   建   人: yzy
**最后修改日期: 2013年08月28日
**描        述: UART串口通道管理进程
**注        意:
**--------------历史版本信息---------------------------------------------------
** 创建人: yzy
** 版  本: v2.0
** 日　期: 2013年08月28日
** 描　述: 原始版本
******************************************************************************/
#define EXT_DEV_UART

//	#include "es.h"

#include "aos/include.h"
//	#include "apis.h"
//	#include "k_types.h"
//	#include "k_api.h"
//	#include "uartcfg.h"
//	#include "lib_app.h"
#include "hal.h"
#include <stdarg.h>
#include <stdio.h>
#if (NO_OF_SERIAL == 0)
#error "没有串口不需要包含此文件!"
#endif

/************************************************************************
**声明外部函数
************************************************************************/
void _UartWriteHook(uint8 port, uint8* buffer, uint16 length);
void _UartReadHook(uint8 port, uint8* buffer, uint16 length);


/************************************************************************
**声明外部变量
************************************************************************/
extern const SerialID* const gss_UartSID[NO_OF_SERIAL];

#ifdef EXT_DEV_UART

/******************************************************************************
**串口缓存接收状态
******************************************************************************/
//	EXT_DEV_UART uint8 gucs_UartRevFlag[NO_OF_SERIAL];
//	EXT_DEV_UART uint8 gucs_UartRevStart[NO_OF_SERIAL];
//	EXT_DEV_UART uint8 gucs_UartRevByteTimeout[NO_OF_SERIAL];

//	__SUart g_uartTbl[NO_OF_SERIAL];// = 
//	{
//	//	    {UART_CHANNEL_PARA, SYS_UART0_IDX, false},
//	//	    {UART_CHANNEL_IRDA, SYS_UART1_IDX, false},
//	//	    {UART_CHANNEL_GPRS, SYS_UART2_IDX, false},
//	//	    {UART_CHANNEL_DEBUG, SYS_UART3_IDX, false},    
//	    {UART_CHANNEL_ESAM, SYS_UART0_IDX, false},
//	    {UART_CHANNEL_GPRS, SYS_UART1_IDX, false},
//	    {UART_CHANNEL_DEBUG, SYS_UART2_IDX, false}, 
//	    {UART_CHANNEL_CARD, SYS_UART3_IDX, false},
//	
//	};
#endif


#define __UART_ON_RECV_FLAG				0x1				//有接收数据事件标志
#define __UART_ON_SEND_FLAG				0x2				//有发送数据事件标志

//	static kevent_t			gflagUartSend;				//发送标志组，用来同一通道号发送数据时同步


/************************************************************************
 * @function: SYS_SER_WriteOption
 * @描述: 串口数据发送
 * 
 * @参数: 
 * @param: port 串口号
 * @param: buffer 发送数据缓存
 * @param: length 发送数据长度
 * @param: opt 选项:b0=1串口性能优先,b0=0系统开销优先
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2013/8/28)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	uint8 SYS_SER_WriteOption(uint8 port, uint8 * buffer, uint16 length, uint16 opt)
//	{
//	    volatile SerialBuffer* uart = gss_UartSID[port]->uart_Buffer;
//	                                    //参数合法性判断
//	    if((port >= NO_OF_SERIAL) || (length == 0))
//	    {
//	        return SYS_ERR_FT;
//	    }
//	                                    //配置发送结构体
//	    uart->tbuff = buffer;           //发送的数据缓存
//	    uart->tlen = length;            //发送缓存长度
//	    uart->tcnt = length;            //发送的字节数
//	    uart->tp = 0;
//	                                    //启动发送并等待发送完成
//	#if SYS_UART9_EN > 0
//	    if(port == SYS_UART9_IDX)
//	    {
//	        Uart9_StartSend(gss_UartSID[port]);
//	    }
//	    else
//	#endif
//	    {
//	        Uartx_StartSend(gss_UartSID[port]);
//	    }
//	//    uart_dev_t
//	#if SER_SEND_HOOK_EN > 0
//	    _UartWriteHook(port, buffer, length);
//	#endif
//	    
//	//	    while(true)
//	//	    {
//	//	        if(uart->tcnt == 0)
//	//	        {
//	//	            break;
//	//	        }
//	//	        
//	//	        if(opt & 0x0001)
//	//	        {
//	//	            //串口性能优先
//	//	        }
//	//	        else
//	//	        {
//	//	            //系统开销优先,默认方式.
//	//	            sys_sleep(1);
//	//	        }
//	//	    }
//	
//	    
//	    SYS_OK();
//	}


/************************************************************************
 * @function: SYS_SER_Write
 * @描述: 串口数据发送
 * 
 * @参数: 
 * @param: port 串口号
 * @param: buffer 发送数据缓存
 * @param: length 发送数据长度
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2013/8/28)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	uint8 SYS_SER_Write(uint8 port, uint8* buffer, uint16 length)
//	{
//	    return SYS_SER_WriteOption(port, buffer, length, 0);
//	}

/************************************************************************
 * @function: SYS_SER_Read
 * @描述: 串口数据读取
 * 
 * @参数: 
 * @param: port 端口
 * @param: buffer 输出缓存,__NULL表示丢弃
 * @param: length 读取长度
 * 
 * @返回: 
 * @return: uint16  实际读取长度
 * @说明: 
 * @作者: yzy (2013/8/28)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	uint16 SYS_SER_Read(uint8 port, uint8* buffer, uint16 length)
//	{
//	    SerialBuffer* uart = gss_UartSID[port]->uart_Buffer;
//	    uint16 pr;
//	    uint16 count;
//	    uint16 i;
//	                                   //参数合法性判断
//	    if((port >= NO_OF_SERIAL) || (length == 0))
//	    {
//	        return 0;
//	    }
//	//	    gss_UartSID[port]->usart_RcvEnable();
//	
//	    CPSR_ALLOC();
//	    RHINO_CPU_INTRPT_DISABLE();
//	    count = uart->rcnt;
//	    if(count > length)              //获取实际读取的数据长度
//	    {
//	        count = length;
//	    }
//	    RHINO_CPU_INTRPT_ENABLE();
//	                                    //循环转移数据
//	    if(__NULL != buffer)            //not dequeue
//	    {
//	        pr = uart->pr;              //数据读取起始指针
//	        for(i = 0; i < count; i++)
//	        {
//	            buffer[i] = uart->rbuff[pr];
//	            pr++;
//	            if(pr >= uart->rlen)    //卷绕
//	            {
//	                pr = 0;
//	            }
//	        }
//	    }
//	    else                            //dequeue
//	    {
//	#if SER_RECV_HOOK_EN > 0
//	        if(count > (uart->rlen - uart->pr))
//	        {
//	            _UartReadHook(port, &uart->rbuff[uart->pr], uart->rlen - uart->pr);
//	            _UartReadHook(port, &uart->rbuff[0], count - (uart->rlen - uart->pr));
//	        }
//	        else
//	        {
//	            _UartReadHook(port, &uart->rbuff[uart->pr], count);
//	        }
//	#endif
//	    }
//	    
//	    uart->pr += count;              //数据处理指针前移
//	    while(uart->pr > uart->rlen)   //卷绕
//	    {
//	        uart->pr -= uart->rlen;
//	    }
//	    
//	    RHINO_CPU_INTRPT_DISABLE();
//	    uart->rcnt -= count;            //未处理数据长度递减 yzy->rcnt在handler中会被改变
//	    RHINO_CPU_INTRPT_ENABLE();
//	
//	#if SER_RECV_HOOK_EN > 0
//	     _IF_TRUE_DO(__NULL != buffer, _UartReadHook(port, buffer, count));
//	#endif
//	
//	    return count;
//	}

/************************************************************************
 * @function: SYS_SER_ToBeRead
 * @描述: 串口可读取数据的长度
 * 
 * @参数: 
 * @param: port 
 * 
 * @返回: 
 * @return: uint16  
 * @说明: 
 * @作者: yzy (2013/8/28)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
int32_t SER_ToBeRead_By_Port(uint8 ucChannelId)
{

    int32_t ret = -1;
    SerialID *ids = NULL; 

    ids = __Uart_Find(ucChannelId);
    if( NULL== ids ){ 
        return -1;
    }

    return hal_uart_to_be_read(ids->handle);

}






//	void Uart_Read_Callback(uint8 ch)
//	{
//		__SUart * uart = &g_uartTbl[ch];
//	
//	//		uart = __Uart_Find(ch);
//	//		if(!uart)
//	//		{
//	//			return;
//	//		}
//	
//	    if(uart->has_Wait_rcv == true)
//	    {
//	        krhino_event_set(&uart->event, __UART_ON_RECV_FLAG, RHINO_OR);
//	        uart->has_Wait_rcv = false;
//	    }
//	}

//	void Uart_Send_Callback(uint8 ch)
//	{
//	//		__SUart * uart;
//	//	
//	//		uart = __Uart_Find(ch);
//	//		if(!uart)
//	//		{
//	//			return;
//	//		}
//	    __SUart * uart = &g_uartTbl[ch];
//	
//	//	    if(uart->has_Wait_rcv == true)
//	//	    {
//	    krhino_event_set(&uart->event, __UART_ON_SEND_FLAG, RHINO_OR);
//	//	        uart->has_Wait_rcv = false;
//	//	    }
//	}

/*******************************************************************************
 * @function_name:  SYS_SER_RevMessageLoop
 * @function_file:  dev_msgloop.c
 * @描述: 100ms,UART串口数据接收消息分发机制
 * 
 * 
 * @参数: 
 * 
 * @返回:
 * @作者: yzy (2018年3月22日)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
void SYS_SER_RevMessageLoop(void)
{
    for(uint8 uartidx = 0; uartidx < NO_OF_SERIAL; uartidx++)
    {
//          if(SER_ToBeRead_By_Port(uartidx) > 0)
        if(gucs_UartRevStart[uartidx])
        {
            if(gucs_UartRevByteTimeout[uartidx]>0)//接收字节间超时
            {
                gucs_UartRevFlag[uartidx] = 1;
                gucs_UartRevStart[uartidx] = 0;
                gucs_UartRevByteTimeout[uartidx] = 0;
//	                Uart_Read_Callback(gss_UartSID[uartidx]->ch);
            }
            gucs_UartRevByteTimeout[uartidx]++;
        }
    }

}


/*==================================================================
//函数名：  SER_RecvData_Sync
//作者：    杨州宇
//日期：    2018-1-24
//功能：    串口接收数据,等待同步消息结束，或者超时结束
//输入参数：1、ucChannelId：串口通道号         
//          2、nMaxLen：期望接收到的最大字节数
//          3、nTimeout：接收超时时间（单位：毫秒），若为0，表示不等待
//输出参数：1、strData：要接收的数据
//返回值：  成功返回实际接收到的字节数，失败返回<0
//=================================================================*/
int32_t SER_RecvData(unsigned char ucChannelId, unsigned char *strData, 
							  int32 nMaxLen, int32 nTimeout)
{
//		__SUart * uart = &g_uartTbl[ucChannelId];
//		kstat_t err;
//		int nReadCnt;
//		int nCnt;
//		tick_t nStartTicks;
//		tick_t nLeftTicks;
//	    uint32_t eventInfo = 0;
    uint32_t recv_size;
    int32_t ret = -1;
    SerialID *ids = NULL; 
    ids = __Uart_Find(ucChannelId);
    if( NULL== ids ){ 
        return -1;
    }

    
	if( nMaxLen < 0 || nTimeout < 0)
	{
		return API_ERR_PARA;
	}
	if(!nMaxLen)
	{
		return 0; 
	}
    ret = hal_uart_recv_II(ids->handle, strData, nMaxLen, &recv_size, nTimeout);

    if(ret == 0)
    {
        return recv_size;
    }
    else
    {
        return 0;
    }
//		uart = __Uart_Find(ucChannelId);
//		if(!uart)
//		{
//			return API_ERR_PARA;
//		}
//	    if(nTimeout == 0)//不等待
//	    {
//	        return SYS_SER_Read(ucChannelId, strData, nMaxLen);
//	    }
//	    else
//	    {
//	        if(!strData)
//	        {
//	            return API_ERR_PARA;
//	        }
//	    	nLeftTicks = (nTimeout * RHINO_CONFIG_TICKS_PER_SECOND / 1000) + 1;
//	    	nReadCnt = 0;
//	        gs_DEVLowPwr.Set((LOWPWRENUM)ucChannelId);
//	    	while(nMaxLen > 0)
//	    	{
//	    		//如果缓冲区里有数据，则先读缓冲里的数据
//	    		nCnt = SYS_SER_Read(ucChannelId, strData + nReadCnt, nMaxLen);
//	    		nReadCnt += nCnt;
//	    		nMaxLen -= nCnt;
//	    		if(nMaxLen <= 0 || nLeftTicks <= 0)
//	    		{
//	    			break;
//	    		}
//	
//	    		nStartTicks = krhino_sys_tick_get();
//	    		uart->has_Wait_rcv = true;
//	    		err = krhino_event_get(&uart->event, __UART_ON_RECV_FLAG, RHINO_AND_CLEAR, &eventInfo, nLeftTicks);
//	    		if(err == RHINO_BLK_TIMEOUT || err == RHINO_SUCCESS)
//	    		{
//	    			uart->has_Wait_rcv = false;
//	    			//由于实现机制的原因，可能会漏掉数据而导致超时，所以再去读取缓冲区里的数据
//	    			nReadCnt += SYS_SER_Read(ucChannelId, strData + nReadCnt, nMaxLen);
//	    			break;
//	    		}
//	
//	    		uart->has_Wait_rcv = false;
//	    		nLeftTicks -= krhino_sys_tick_get() - nStartTicks;
//	    	}
//	        gs_DEVLowPwr.Clear((LOWPWRENUM)ucChannelId);
//	
//	    	return nReadCnt;
//	    }
}


/*==================================================================
//函数名：  SER_SendData
//作者：    杨州宇
//日期：    2018-1-24
//功能：    串口发送数据
//输入参数：1、ucChannelId：串口通道号
//          2、strData：要发送的数据
//          3、nDataLen：要发送的数据长度
//          4、nTimeout：发送超时时间（单位：毫秒）， 若为0， 表示不等待
//输出参数：无
//返回值：  成功返回0，失败返回<0
//=================================================================*/
int32_t SER_SendData(unsigned char ucChannelId, unsigned char *strData, 
							  int32 nDataLen, int32_t nTimeout)
{
//		__SUart * uart = &g_uartTbl[ucChannelId];
//	//	int nIndex;
//	//	unsigned char ucPowerState;
//		kstat_t os_err;
//	    uint32_t eventInfo = 0;
//	    tick_i_t nLeftTicks;
//		tick_i_t nStartTicks;
    SerialID *ids = NULL; 
    ids = __Uart_Find(ucChannelId);
    if( NULL== ids ){ 
        return -1;
    }
    
	if(!strData || nDataLen < 0 || nTimeout < 0)
	{
		return API_ERR_PARA;
	}
	if(!nDataLen)
	{
		return API_SUCCESS; 
	}
    return hal_uart_send(ids->handle, strData, nDataLen, nTimeout);

//		uart = __Uart_Find(ucChannelId);
//		if(!uart)
//		{
//			return API_ERR_PARA;
//		}



//		if(uart->ucType == __UART_TYPE_RS485)
//		{
//			if(API_SUCCESS != POWER_GetStatus(&ucPowerState))
//			{
//				return API_ERR_FAILURE;
//			}
//			if(ucPowerState == POWER_STATUS_OFF)
//			{
//				return API_ERR_FAILURE;
//			}
//		}
//		nLeftTicks = (nTimeout * RHINO_CONFIG_TICKS_PER_SECOND / 1000) + 1;
//	    nStartTicks = krhino_sys_tick_get();
//	    
//		os_err = krhino_event_get(&gflagUartSend, 1 << ucChannelId, RHINO_AND_CLEAR, &eventInfo, nLeftTicks);
//		if (os_err != RHINO_SUCCESS) return API_ERR_FAILURE;
//	    
//	    gs_DEVLowPwr.Set((LOWPWRENUM)ucChannelId);
//	
//		SYS_SER_Write(ucChannelId, (uint8 *)strData, nDataLen);
//	    
//	    krhino_event_set(&gflagUartSend, 1 << ucChannelId, RHINO_OR);
//	
//	
//		nLeftTicks -= krhino_sys_tick_get() - nStartTicks;
//		//nReadCnt = 0;
//		while(1)
//		{
//		    volatile SerialBuffer* serBuf = gss_UartSID[ucChannelId]->uart_Buffer;
//			
//			if(serBuf->tcnt <= 0)
//			{
//				break;
//			}
//	
//			os_err = krhino_event_get(&uart->event, __UART_ON_SEND_FLAG, RHINO_AND_CLEAR, &eventInfo, nLeftTicks);
//			if(os_err == RHINO_BLK_TIMEOUT)
//			{
//			    return API_ERR_FAILURE;
//			}
//	        else if(os_err == RHINO_SUCCESS)
//	        {
//	            return API_SUCCESS;
//	        }
//			nLeftTicks -= krhino_sys_tick_get() - nStartTicks;
//	
//	        if (nLeftTicks <= 0)
//	        {
//	            gs_DEVLowPwr.Clear((LOWPWRENUM)ucChannelId);
//	            return API_ERR_FAILURE;
//	        }
//		}
//	    gs_DEVLowPwr.Clear((LOWPWRENUM)ucChannelId);

//	return API_SUCCESS;
}

/************************************************************************
 * @function: SYS_UART_Init
 * @描述: 串口功能初始化
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2013/8/28)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void UART_Init(void)
{
    //name_t name[32];
    
	//__SUart* uart;

//	    uint8 uc_i, uc_j = NO_OF_SERIAL;
    for(uint8 uc_i = 0; uc_i < NO_OF_SERIAL; uc_i++)
    {
        gucs_UartRevFlag[uc_i] = 0;
        gucs_UartRevStart[uc_i] = 0;
        gucs_UartRevByteTimeout[uc_i] = 0;

        hal_uart_init(gss_UartSID[uc_i]->handle);
    
    }
    
//		krhino_event_create(&gflagUartSend, "uart send flag", 0xffffffff);
}








/************************************************************************
 * @function: SYS_SER_Init
 * @描述: 串口配置
 * 
 * @参数: 
 * @param: port 
 * @param: ss 
 * @返回: 
 * @说明: 
 * @作者: yzy (2013/8/28)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
int32_t SER_Init(uint8 ucChannelId, uart_config_t * ss)
{
    int32_t ret = -1;
    SerialID *ids = NULL; 

    ids = __Uart_Find(ucChannelId);
    if( NULL== ids ){ 
        return -1;
    }
    return hal_uart_reset(ids->handle, ss);


}

/************************************************************************
 * @function: SER_Open
 * @描述: 串口打开
 * 
 * @参数: 
 * @param: port 
 * @param: option TDB_MODE_R:允许读 .TDB_MODE_W:允许写
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2018/5/2)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
int32 SER_Open(uint8 ucChannelId, uint8 option)
{
//	//	    return SER_OpenCallBack(port, option, __NULL);
//	
//	//	    __SUart* uart;
//	//	    uint8 port = 0;
//	//	    uart = __Uart_Find(ucChannelId);
//	//	    if(!uart)
//	//	    {
//	//	        return API_ERR_PARA;
//	//	    }
//	//	    
//	//	    port = uart->port;
//	
//	
//	    Uartx_Enable(gss_UartSID[ucChannelId], option);
//	
    SerialID *ids = NULL; 
    ids = __Uart_Find(ucChannelId);
    if( NULL== ids ){ 
        return -1;
    }
    
    return hal_uart_open(ids->handle->port);

}
//	
//	
/************************************************************************
 * @function: SYS_SER_Close
 * @描述: 串口关闭
 * 
 * @参数: 
 * @param: port 
 * @返回: 
 * @说明: 
 * @作者: yzy (2018/5/2)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
int32 SER_Close(uint8 ucChannelId)
{
//	    __SUart* uart;
//	    uint8 port = 0;
//	    uart = __Uart_Find(ucChannelId);
//	    if(!uart)
//	    {
//	        return API_ERR_PARA;
//	    }
//	    
//	    port = uart->port;
	
    
//	    Uartx_Disable(gss_UartSID[ucChannelId]);
	
    int32_t ret = -1;
    SerialID *ids = NULL; 

    ids = __Uart_Find(ucChannelId);
    if( NULL== ids ){ 
        return -1;
    }
    return hal_uart_close(ids->handle);
}


/************************************************************************
 * @function: SER_Clear
 * @描述: 清空串口缓存
 * 
 * @参数: 
 * @param: port 
 * @返回: 
 * @说明: 
 * @作者: yzy (2013/8/28)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
int32 SER_Clear(uint8 port)
{
//	    __SUart* uartst;
//	    uint8 port = 0;
//	    uartst = __Uart_Find(ucChannelId);
//	    if(!uartst)
//	    {
//	        return API_ERR_PARA;
//	    }
//	    
//	    port = uartst->port;
    
//	    SerialBuffer* uart = gss_UartSID[port]->uart_Buffer;
//	#if SER_RECV_HOOK_EN > 0
//	    uint16 count;
//	    count = uart->rcnt;             //串口缓存中实际可读数据数
//	    if(count > (uart->rlen - uart->pr))
//	    {
//	        _UartReadHook(port, &uart->rbuff[uart->pr], uart->rlen - uart->pr);
//	        _UartReadHook(port, &uart->rbuff[0], count - (uart->rlen - uart->pr));
//	    }
//	    else
//	    {
//	        _UartReadHook(port, &uart->rbuff[uart->pr], count);
//	    }
//	#endif
//	    
//	    CPSR_ALLOC();
//	    RHINO_CPU_INTRPT_DISABLE();
//	    
//	    uart->rcnt = 0;
//	    uart->rp = 0;
//	    uart->pr = 0;
//	    uart->tp = 0;
//	    uart->tcnt = 0;
//	    uart->tbuff = __NULL;
//	    RHINO_CPU_INTRPT_ENABLE();
    
    return API_SUCCESS;
}



/************************************************************************
 * @function: SER_Peek
 * @描述: 串口接收数据窥探,执行后数据仍然存在于接收缓存内
 * 
 * @参数: 
 * @param: port 端口
 * @param: buffer 存放窥探数据的缓存
 * @param: start 从第几个数据开始窥探(0-) 
 * @param: length 待窥探数据长度
 * 
 * @返回: 
 * @return: uint16  窥探得到的实际数据长度
 * @说明: SYS_SER_Read()的部分功能
 * @作者: yzy (2013/8/28)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	int16 SER_Peek(uint8 port, uint8* buffer, uint16 start, uint16 length)
//	{
//	    uint16 pr;
//	    uint16 count;
//	    uint16 i;
//	
//	//	    __SUart * uartHandle = __Uart_Find(ch);
//	//	    if(!uartHandle)
//	//	    {
//	//	        return API_ERR_PARA;
//	//	    }
//	
//	    SerialBuffer* uart = gss_UartSID[port]->uart_Buffer;
//	                                    //参数合法性判断
//	    if((port >= NO_OF_SERIAL) || (length == 0))
//	    {
//	        return 0;
//	    }
//	    
//	    if(start >= uart->rcnt)         //起始字节数合法性判断
//	    {
//	        return 0;
//	    }
//	
//	    CPSR_ALLOC();
//	    RHINO_CPU_INTRPT_DISABLE();
//	    if((start + length) < uart->rcnt)//"待读取长度"与"串口缓存中实际可读数据数"的比较
//	    {
//	        count = length;
//	    }
//	    else
//	    {
//	        count = uart->rcnt - start;
//	    }
//	    RHINO_CPU_INTRPT_ENABLE();
//	                                    //循环转移数据
//	    pr = uart->pr + start;          //数据读取起始指针
//	    if(pr >= uart->rlen)
//	    {
//	        pr -= uart->rlen;
//	    }
//	    
//	    for(i = 0; i < count; i++)
//	    {
//	        buffer[i] = uart->rbuff[pr];
//	        pr++;
//	        if(pr >= uart->rlen)        //卷绕
//	        {
//	            pr = 0;
//	        }
//	    }
//	
//	    return count;
//	}

//	/*==================================================================
//	//函数名：  SER_Read
//	//作者：    杨州宇
//	//日期：    2018-1-24
//	//功能：    串口接收数据
//	//输入参数：1、ucChannelId：串口通道号         
//	//          2、nMaxLen：期望接收到的最大字节数
//	//输出参数：1、strData：要接收的数据
//	//返回值：  成功返回实际接收到的字节数，失败返回<0
//	//=================================================================*/
//	int32_t SER_Read(unsigned char ucChannelId, unsigned char *strData, int32_t nMaxLen)
//	{
//		__SUart* uart;
//		int nCnt;
//		if(nMaxLen < 0)
//		{
//			return API_ERR_PARA;
//		}
//		if(!nMaxLen)
//		{
//			return 0; 
//		}
//	
//		uart = __Uart_Find(ucChannelId);
//		if(!uart)
//		{
//			return API_ERR_PARA;
//		}
//	
//		nCnt = SYS_SER_Read(uart->port, strData, nMaxLen);
//	
//	
//		return nCnt;
//	}


/************************************************************************
 * @function: SER_ReadByte
 * @描述: 从串口读取一个字节数据
 * 
 * @参数: 
 * @param: port 
 * 
 * @返回: 
 * @return: uint16  
 * @说明: 
 * @作者: yzy (2013/8/28)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	int16 SER_ReadByte(uint8 ch)
//	{
//	    uint8 result;
//	
//	    if(SER_RecvData(ch, &result, 1, 100) == 1)
//	    {
//	        return result;
//	    }
//	    return 256;
//	}





/************************************************************************
 * @function: SER_ToBeRead
 * @描述: 串口可读取数据的长度
 * 
 * @参数: 
 * @param: port 
 * 
 * @返回: 
 * @return: uint16  
 * @说明: 
 * @作者: yzy (2013/8/28)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	int16 SER_ToBeRead(uint8 port)
//	{
//	//	    SerialBuffer* uart = gss_UartSID[port]->uart_Buffer;
//	    uint16 result;
//	//	    __SUart * uartHandle = __Uart_Find(ch);
//	//	    if(!uartHandle)
//	//	    {
//	//	        return API_ERR_PARA;
//	//	    }
//	
//	    SerialBuffer* uart = gss_UartSID[port]->uart_Buffer;
//	
//	    
//	    if(port >= NO_OF_SERIAL)
//	    {
//	        return 0;
//	    }
//	
//	    CPSR_ALLOC();
//	    RHINO_CPU_INTRPT_DISABLE();
//	    result = uart->rcnt;
//	    RHINO_CPU_INTRPT_ENABLE();
//	
//	    return result;
//	}
//	
//	
//	
//	
//	/************************************************************************
//	 * @function: SER_Line
//	 * @描述: 串口读取行命令(若未找到行结束符,串口缓存被读空)
//	 * 
//	 * @参数: 
//	 * @param: port 端口号
//	 * @param: buffer 存放数据缓存,__NULL表示丢弃
//	 * @param: length 存放数据缓存的长度
//	 * 
//	 * @返回: 
//	 * @return: uint16  读取的行长度(包括一个结尾符).
//	 *          0表示无数据或未找到行结束符,1表示仅读到结尾符
//	 * @说明: 
//	 * @作者: yzy (2013/8/28)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
//	 ************************************************************************/
//	int16 SER_Line(uint8 port, uint8* buffer, uint16 length)
//	{
//	//	    SerialBuffer* uart = gss_UartSID[port]->uart_Buffer;
//	    uint16 count;
//	    uint16 pr;
//	    uint16 i;
//	    uint8 sbyte;
//	
//	//	    __SUart * uartHandle = __Uart_Find(ch);
//	//	    if(!uartHandle)
//	//	    {
//	//	        return API_ERR_PARA;
//	//	    }
//	
//	    SerialBuffer* uart = gss_UartSID[port]->uart_Buffer;
//	
//	
//	    if(port >= NO_OF_SERIAL)
//	    {
//	        return 0;
//	    }
//	
//	    if(uart->rcnt == 0)             //未处理接收数据长度为0,返回
//	    {
//	        return 0;
//	    }
//	
//	    CPSR_ALLOC();
//	    RHINO_CPU_INTRPT_DISABLE();
//	    count = uart->rcnt;             //未处理数据长度
//	    if(count > length)              //操作的数据长度
//	    {
//	        count = length;
//	    }
//	    RHINO_CPU_INTRPT_ENABLE();
//	                                    //循环判定行尾,同时转移数据
//	    pr = uart->pr;                  //处理指针
//	    for(i = 0; i < count; i++)      //循环读取数据
//	    {
//	        sbyte = uart->rbuff[pr];
//	        _IF_TRUE_DO(__NULL != buffer, buffer[i] = sbyte);
//	        pr++;
//	        if(pr >= uart->rlen)
//	        {
//	            pr = 0;
//	        }
//	        if((sbyte == '\r') || (sbyte == '\n'))
//	        {
//	            break;
//	        }
//	    }
//	
//	    if(i == count)                  //不存在一行内容
//	    {
//	        return 0;
//	    }
//	    count = i + 1;
//	    
//	#if SER_RECV_HOOK_EN > 0
//	    if(__NULL == buffer)
//	    {
//	        if(count > (uart->rlen - uart->pr))
//	        {
//	            _UartReadHook(uartHandle->port, &uart->rbuff[uart->pr], uart->rlen - uart->pr);
//	            _UartReadHook(uartHandle->port, &uart->rbuff[0], count - (uart->rlen - uart->pr));
//	        }
//	        else
//	        {
//	            _UartReadHook(uartHandle->port, &uart->rbuff[uart->pr], count);
//	        }
//	    }
//	#endif
//	    
//	    uart->pr += count;              //处理指针前移
//	    while(uart->pr >= uart->rlen)
//	    {
//	        uart->pr -= uart->rlen;
//	    }
//	    
//	    RHINO_CPU_INTRPT_DISABLE();
//	    uart->rcnt -= count;            //未处理数据长度递减
//	    RHINO_CPU_INTRPT_ENABLE();
//	
//	    
//	#if SER_RECV_HOOK_EN > 0
//	     _IF_TRUE_DO(__NULL != buffer, _UartReadHook(uartHandle->port, buffer, count));
//	#endif
//	
//	    return count;
//	
//	}







/************************************************************************
 * @function: SER_LineLen
 * @描述: 获取串口当前行长度
 * 
 * @参数: 
 * @param: port 端口号
 * 
 * @返回: 
 * @return: uint16  行长度(包括一个结尾符)
 *          0表示无数据或未找到行结束符,1表示仅读到结尾符
 * @说明: 
 * @作者: yzy (2013/8/28)
 *-----------------------------------------------------------------------
 * @修改人: 
//	 ************************************************************************/
//	int16 SER_LineLen(uint8 port)
//	{
//	//	    SerialBuffer* uart = gss_UartSID[port]->uart_Buffer;
//	    uint16 count;
//	    uint16 pr;
//	    uint16 i;
//	
//	//	    __SUart * uartHandle = __Uart_Find(ch);
//	//	    if(!uartHandle)
//	//	    {
//	//	        return API_ERR_PARA;
//	//	    }
//	
//	    SerialBuffer* uart = gss_UartSID[port]->uart_Buffer;
//	
//	
//	    if(port >= NO_OF_SERIAL)
//	    {
//	        return 0;
//	    }
//	
//	    if(uart->rcnt == 0)             //没有为读取数据返回0
//	    {
//	        return 0;
//	    }
//	
//	    CPSR_ALLOC();
//	    RHINO_CPU_INTRPT_DISABLE();
//	            
//	    count = uart->rcnt;             //未处理数据长度
//	    pr = uart->pr;                  //处理指针
//	    RHINO_CPU_INTRPT_ENABLE();
//	    
//	    for(i = 0; i < count; i++)        //循环判断
//	    {
//	        if((uart->rbuff[pr] == '\r') || (uart->rbuff[pr] == '\n'))
//	        {
//	            break;
//	        }
//	        pr++;
//	        if(pr >= uart->rlen)
//	        {
//	            pr = 0;
//	        }
//	    }
//	            
//	    if(i == count)                    //不存在命令行
//	    {
//	        return 0;
//	    }
//	
//	    count = i + 1;
//	
//	    return count;
//	
//	}
//	



/************************************************************************
 * @function: SER_PrintValueOption
 * @描述: 发送数据,按照字符串的格式化进行发送
 * 
 * @参数: 
 * @param: port 串口号
 * @param: str 发送的描述字符串
 * @param: value 发送的数据
 * @param: opt 选项:b0=1串口性能优先,b0=0系统开销优先
 * 
 * @返回: 
 * @return: uint8  成功/失败
 * @说明: 
 * @作者: yzy (2013/6/10)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	uint8 SER_PrintValueOption(uint8 ch, string str, int value, int32_t nTimeout)
//	{
//	    uint32 i;
//	    uint32 len = strlen(str);
//	    uint16 templen;
//	    uint8 data[12];
//	
//	    if(len == 0)                        //如果字符串长度为0则直接返回
//	    {
//	        SYS_OK();                        //返回处理成功
//	    }
//	
//	    for(i = 0; i < len; i++)            //扫描直到遇到'%'
//	    {                                        
//	        if(str[i] == '%')               //判断是否为'%'        
//	        {                               
//	            if(str[i + 1] == 'x' 
//	               || str[i + 1] == 'd' 
//	               || str[i + 1] == 's'
//	               )
//	            {                           //打印前面的字符串
//	                //SYS_SER_WriteOption(port, (uint8*)str, i, opt);
//	                SER_SendData(ch, (uint8*)str, i, nTimeout);
//	                break;                  //跳出
//	            }
//	        }
//	    }
//	    if(i == len)                        //如果已经扫描完或者最后一个为%则直接退出
//	    {                                   //输出信息
//	//	       SYS_SER_WriteOption(port, (uint8*)str, len, opt);
//	       SER_SendData(ch, (uint8*)str, len, nTimeout);
//	       SYS_OK();                         //返回处理成功
//	    }
//	
//	    switch(str[i + 1])                  //选择处理字
//	    {
//	        case 'x':                       //HEX输出  
//	                                        //判断是否为'0' - '9'      
//	            if(!IsBcdChar(str[i + 2]))
//	            {
//	                break;                  //不是的话退出
//	            }
//	            NumToHexString(//数字到Hex字符串的转换
//	                data, 
//	                (uint8*)&value, 
//	                (str[i + 2] - '0') >> 1
//	                );
//	
//	            SER_SendData(               //打印转换出来的数据
//	                ch, 
//	                data, 
//	                str[i + 2] - '0',
//	                nTimeout
//	                );
//	            i++;                        //方便后面的处理
//	            break;
//	        case 'd':                       //数字字符输出    
//	                                        //判断是否为格式化输出
//	            if(IsBcdChar(str[i + 2]))  
//	            {
//	                templen = str[i + 2] - '0';
//	                i++;
//	            }
//	            else
//	            {
//	                templen = 0;
//	            }
//	                                        //数字到BCD字符串的转换
//	            templen = NumToBcdString(   
//	                data, 
//	                value, 
//	                templen
//	                );
//	
//	            SER_SendData(         //输出数据
//	                ch, 
//	                data, 
//	                templen,
//	                nTimeout
//	                );
//	            break;
//	        case 's':                       //直接输出字符串        
//	            SER_SendData(         //输出字符串
//	                ch, 
//	                (uint8 *)value, 
//	                sizeof((uint8 *)value),
//	                nTimeout
//	                );
//	            break;
//	    }
//	    i += 2;
//	    
//	    if(i >= len)                        //如果已经扫描完或者最后一个为%则直接退出
//	    {
//	       SYS_OK();                         //返回处理成功
//	    }
//	                                        //输出后面的内容
//	    SER_SendData(ch, (uint8*)str + i, strlen(str + i), nTimeout);
//	
//	    SYS_OK();                            //返回结果
//	}
//	
//	
//	




/************************************************************************
 * @function: SER_PrintValue
 * @描述: 发送数据,按照字符串的格式化进行发送
 * 
 * @参数: 
 * @param: ch 串口号
 * @param: buffer 缓存  
 * @param: buflen 缓存长度     
 * @param: timeout 发送延时
 * @param: str 发送的描述字符串
 * @param: ... 发送的数据
 * 
 * @返回: 
 * @return: uint8  成功/失败
 * @说明: 
 * @作者: yzy (2013/6/10)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	int32 SER_PrintValue(uint8 ch, uint8 * buff, uint16 buflen, int32 nTimeout, const int8 *strFormat, ...)
//	{
//		int32 ret = 0;
//		va_list  vlist;
//	
//		if (strFormat == NULL) 
//	    {
//	        return SYS_ERR_VAR;
//	    }
//		
//	    memset(buff, 0, buflen);
//		
//		va_start(vlist, strFormat);
//		vsnprintf((char *)buff, 
//	               buflen, 
//	               (char const *)strFormat, vlist);
//		va_end(vlist);
//	    
//	   
//	    SER_SendData(ch, (uint8 *)buff, strlen((char const *)buff), nTimeout);
//	
//		return ret;
//	}





