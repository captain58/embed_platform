/******************************Copyright(c)******************************
**                         
**
** File Name: dev_uart.c
** Author: 
** Date Last Update: 2019-11-02
** Description: UART串口通道管理进程
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: 文件创建
** 1)必须使用open和close操作的接口函数: SYS_SER_Read()\SYS_SER_Clear()\
**      SYS_SER_Peek()\SYS_SER_Line()\SYS_SER_LineLen()
** 2)建议:若某个进程独占某串口,可以在进程初始化中open()然后不再作close()操作
** 3)总结:无需open\close操作的函数有:SYS_SER_ToBeRead()
*************************************************************************/
#define EXT_DEV_UART
#ifdef __MODULE__
#include "WOSsys.h"
#endif


#include "sys.h"
#include "hal.h"
#include "bsp.h"
//#include <string.h>
#include "apis.h"
#include "tasks.h"
#include "uartcfg.h"
#if (NO_OF_SERIAL == 0)
    #error "没有串口不需要包含此文件!"
#endif




/************************************************************************
**声明外部函数
************************************************************************/
void _UartClearHook(uint8 port, uint8* buffer, uint16 length);
void _UartReadHook(uint8 port, uint8* buffer, uint16 length);
void _UartWriteHook(uint8 port, uint8* buffer, uint16 length);


/************************************************************************
**声明外部变量
************************************************************************/
#ifndef __MODULE__
extern const SerialID* const gss_UartSID[NO_OF_SERIAL];
#else
extern SerialID* gss_UartSID[NO_OF_SERIAL];
#endif
extern const uint8_t _ucPortMap[];
//int _UartSSToUS(SerialSets ss,UART_InitTypeDef *us)
//{
////	    SerialSets ss;
////	    ss.baudrate = 9600;
////	    ss.databits = DataBits_8bits;
////	    ss.parit = Parit_N;
////	    ss.stopbits = StopBits_1;   
////	    Uartx_Config(&ss, sid);s
//
//    us->BaudRate = ss.baudrate;
//    switch(ss.parit)
//    {
//    case Parit_E:
//        us->ParityBit = EVEN;
//        break;
//    case Parit_O:
//        us->ParityBit = ODD;
//        break;   
//    case Parit_N:
//    default:
//        us->ParityBit = NONE;
//      break;
//        
//    }
//
//    us->StopBit = (UART_StopBitTypeDef)ss.stopbits;
//    us->DataBit = (UART_DataBitTypeDef)ss.databits;
//
//    return 0;
////	    
////	    if(handleuart->Baud == UartBaud1200)            UART_para.BaudRate = 1200;
////	    else if(handleuart->Baud == UartBaud2400)       UART_para.BaudRate = 2400;
////	    else if(handleuart->Baud == UartBaud4800)       UART_para.BaudRate = 4800;
////	    else if(handleuart->Baud == UartBaud9600)       UART_para.BaudRate = 9600;
////	    else if(handleuart->Baud == UartBaud19200)      UART_para.BaudRate = 19200;
////	    else if(handleuart->Baud == UartBaud38400)      UART_para.BaudRate = 38400;
////	    else if(handleuart->Baud == UartBaud115200)     UART_para.BaudRate = 115200;
////	    else if(handleuart->Baud == UartBaud10753)      UART_para.BaudRate = 10753;
////	    if(handleuart->Parity == 0)                                 UART_para.ParityBit = NONE;
////	    else if(handleuart->Parity == 1)                            UART_para.ParityBit = ODD;
////	    else if(handleuart->Parity == 2)                            UART_para.ParityBit = EVEN; 
////	    if((handleuart->StopBit == 0)||(handleuart->StopBit == 1))  UART_para.StopBit = OneBit;
////	    else if(handleuart->StopBit == 2)                           UART_para.StopBit = TwoBit;
////	    if(handleuart->DataBit == 0)                                UART_para.DataBit = Seven7Bit;
////	    else if(handleuart->DataBit == 1)                           UART_para.DataBit = Eight8Bit;
////	    else if(handleuart->DataBit == 2)                           UART_para.DataBit = Nine9Bit;
//
//}

int _FindExtPort(uint8_t port)
{
    for(int i = 0; i < NO_OF_SERIAL; i++)
    {
        if(_ucPortMap[i] == port)
            return i;
    }

    return -1;
}
/************************************************************************
 * @Function: SYS_SER_Init
 * @Description: 串口配置
 * 
 * @Arguments: 
 * @param: port 串口序号(非硬件串口号)
 * @param: ss 
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void SYS_SER_Init(uint8 port, void * ss)
{
    if(_ucPortMap[port] >= NO_OF_SERIAL)
    {
        return;
    }
    
//    UART_SInitTypeDef us;
// 
//    _UartSSToUS(*(SerialSets*)ss,&us);
#if SYS_UARTS_EN > 0
    if(_ucPortMap[port] == SYS_UARTS_IDX)
    {
        UartS_Config(ss, gss_UartSID[_ucPortMap[port]]);
    }
    else
#endif
    {
        Uartx_Config((SerialSets *)ss, gss_UartSID[_ucPortMap[port]]);
    }

}




#ifndef __NO_SYS__ 
/************************************************************************
 * @Function: SYS_SER_OpenCallBack
 * @Description: 串口打开
 * 
 * @Arguments: 
 * @param: port 串口序号(非硬件串口号)
 * @param: isAnsyc true:异步,打不开则退出.false:同步,等待直到获取资源
 * @param: cbs 回调函数,__NULL表示空(回调函数中不可有非实时操作,或发送同步消息,或TKsuspend)
 * 
 * @Note: 
 * @Return: bool  
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
bool SYS_SER_OpenCallBack(uint8 port, bool isAnsyc, TCallback cbs)
{

    if(_ucPortMap[port] >= NO_OF_SERIAL)            //先判断串口号
    {
        return false;
    }
    
//    if(isAnsyc)                         //如果是异步的
//    {
//        if(SYS_SEM_Accept(&gss_UartRes[_ucPortMap[port]]) > 0)
//        {
//            gfs_SerialCallBack[_ucPortMap[port]] = cbs;
//        }
//        else
//        {
//            return false;
//        }
//    }
//    else                                //同步等待
//    {
//        if(SYS_SEM_Wait(&gss_UartRes[_ucPortMap[port]], 0) == SYS_ERR_OK)
//        {
//            gfs_SerialCallBack[_ucPortMap[port]] = cbs;
//        }
//    }
//#ifndef __NO_SYS__     
//                                        //为当前用户进程编号
//    gucs_UartUserTkid[_ucPortMap[port]] = gs_TkTcbCur->tkid;
//#endif    
    return true;                        //返回打开串口成功
}

#endif

/************************************************************************
 * @Function: SYS_SER_Open
 * @Description: 串口打开
 * 
 * @Arguments: 
 * @param: port 串口序号(非硬件串口号)
 * @param: isAnsyc true:异步,打不开则退出.false:同步,等待直到获取资源
 * 
 * @Note: 
 * @Return: bool  
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
bool SYS_SER_Open(uint8 port, bool isAnsyc)
{
#ifndef __NO_SYS__ 
    return SYS_SER_OpenCallBack(_ucPortMap[port], isAnsyc, __NULL);
#else
//	    Uartx_Open(gss_UartSID[_ucPortMap[port]]);

    return true;
#endif
}


/************************************************************************
 * @Function: SYS_SER_Close
 * @Description: 串口关闭
 * 
 * @Arguments: 
 * @param: port 串口序号(非硬件串口号)
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void SYS_SER_Close(uint8 port)
{
    if(_ucPortMap[port] >= NO_OF_SERIAL)            //先判断串口号
    {
        return;
    }
#ifndef __NO_SYS__ 
//    if(gucs_UartUserTkid[_ucPortMap[port]] == gs_TkTcbCur->tkid)
//    {
//        gucs_UartUserTkid[_ucPortMap[port]] = 0xFF;      //恢复成初始值
//        SYS_SEM_Release(&gss_UartRes[_ucPortMap[port]]);   //释放这个信号量
//
////	#if (SYS_UARTSEM_EN > 0)
////	        SYS_SEM_Release((TESRes *)gss_UartSID[_ucPortMap[port]]->uart_rx_sem);
////	        SYS_SEM_Release((TESRes *)gss_UartSID[_ucPortMap[port]]->uart_tx_sem);
////	#endif
//
//        
//        return;
//    }
#else
//	    Uartx_Close(gss_UartSID[_ucPortMap[port]]);
#endif    
    return;
}



/************************************************************************
 * @Function: SYS_SER_Clear
 * @Description: 清空串口缓存
 * 
 * @Arguments: 
 * @param: port 串口序号(非硬件串口号)
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void SYS_SER_Clear(uint8_t port)
{
    SerialBuffer* uart = gss_UartSID[_ucPortMap[port]]->buffer;

    if(_ucPortMap[port] >= NO_OF_SERIAL)
    {
        return;
    }
#ifndef __NO_SYS__     
                                    //判断串口是否已被其他进程占用
//    if(gucs_UartUserTkid[_ucPortMap[port]] != gs_TkTcbCur->tkid)
//    {
//        return;
//    }
#endif    
#if UART_CLEAR_HOOK_EN > 0
    uint16 count;
    count = uart->rcnt;             //串口缓存中实际可读数据数
    if(count > (uart->rlen - uart->pr))
    {
        _UartClearHook(_ucPortMap[port], &uart->rbuff[uart->pr], uart->rlen - uart->pr);
        _UartClearHook(_ucPortMap[port], &uart->rbuff[0], count - (uart->rlen - uart->pr));
    }
    else
    {
        _UartClearHook(_ucPortMap[port], &uart->rbuff[uart->pr], count);
    }
#endif
    
    SYS_ENTER_AllSCRT();
    uart->rcnt = 0;
    uart->rp = 0;
    uart->pr = 0;
    uart->tp = 0;
    uart->tcnt = 0;
    uart->tbuff = __NULL;
    SYS_EXIT_AllSCRT();

}




/************************************************************************
 * @Function: SYS_SER_ReadDataExport
 * @Description: 串口接收数据的复制导出,串口缓存中数据不删除
 * 
 * @Arguments: 
 * @param: port 串口序号(非硬件串口号)
 * @param: func 导出函数
 * 
 * @Note: 
 * @Return: uint16 导出的数据
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 代码实现参考 SYS_SER_Clear()
 ************************************************************************/
void SYS_SER_ReadDataExport(uint8 port, TDataExport func)
{
    SerialBuffer* uart = gss_UartSID[_ucPortMap[port]]->buffer;

    if(_ucPortMap[port] >= NO_OF_SERIAL)
    {
        return;
    }
#ifndef __NO_SYS__     
                                    //判断串口是否已被其他进程占用
//    if(gucs_UartUserTkid[_ucPortMap[port]] != gs_TkTcbCur->tkid)
//    {
//        return;
//    }
#endif
#if (true)
    uint16 count;
    count = uart->rcnt;             //串口缓存中实际可读数据数
    if(count > (uart->rlen - uart->pr))
    {
        func(_ucPortMap[port], &uart->rbuff[uart->pr], uart->rlen - uart->pr);
        func(_ucPortMap[port], &uart->rbuff[0], count - (uart->rlen - uart->pr));
    }
    else
    {
        func(_ucPortMap[port], &uart->rbuff[uart->pr], count);
    }
#endif

}




/************************************************************************
 * @Function: SYS_SER_WriteOption
 * @Description: 串口数据发送
 * 
 * @Arguments: 
 * @param: port 串口序号(非硬件串口号)
 * @param: buffer 发送数据缓存
 * @param: length 发送数据长度
 * @param: opt 选项:b0=1串口性能优先,b0=0系统开销优先
 * 
 * @Note: 
 * @Return: uint8  成功\失败
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
int SYS_SER_WriteOption(uint8 port, uint8* buffer, uint16 length, uint16 opt)
{
    volatile SerialBuffer* uart = gss_UartSID[_ucPortMap[port]]->buffer;
                                    //参数合法性判断
    if((_ucPortMap[port] >= NO_OF_SERIAL) || (length == 0))
    {
        return SYS_ERR_FT;
    }
#ifndef __NO_SYS__     
                                    //判断串口是否已被其他进程占用
//    if(gucs_UartUserTkid[_ucPortMap[port]] != gs_TkTcbCur->tkid)
//    {
//        return SYS_ERR_FT;
//    }
#endif 

//    return hal_uart_send(gss_UartSID[_ucPortMap[port]], buffer, length, 3000);
//}
//	                                    //配置发送结构体
    uart->tbuff = buffer;           //发送的数据缓存
    uart->tlen = length;            //发送缓存长度
    uart->tcnt = length;            //发送的字节数
    uart->tp = 0;
                                    //启动发送并等待发送完成
#if SYS_UARTS_EN > 0
    if(_ucPortMap[port] == SYS_UARTS_IDX)
    {
        UartS_StartSend(gss_UartSID[_ucPortMap[port]]);
    }
    else
#endif
    {
        Uartx_StartSend(gss_UartSID[_ucPortMap[port]]);
    }
    
#if UART_WRITE_HOOK_EN > 0
    _UartWriteHook(_ucPortMap[port], buffer, length);
#endif
//	    uint16_t tick = 100000;
    while(1)
    {
        if(uart->tcnt == 0)
        {
            break;
        }
        
        if(opt & 0x0001)
        {
            //串口性能优先
        }
        else
        {
#ifndef __NO_SYS__           
            //系统开销优先,默认方式.
            msleep(10);
#else
            msDelay(10);
            static int stcnt = 0;
            if(stcnt!= 0 && stcnt == uart->tcnt)
            {

                {
                    Uartx_Send(gss_UartSID[_ucPortMap[port]]);
                }              

            }
            stcnt = uart->tcnt;
#endif              
        }
    }
//    msleep(1);
//    Uartx_WaitSendBufNull(gss_UartSID[_ucPortMap[port]]);
    SYS_OK();
}


//	extern int guc_count;
//	extern uint8_t * p_tmpbuf;
/************************************************************************
 * @Function: SYS_SER_Write
 * @Description: 串口数据发送
 * 
 * @Arguments: 
 * @param: port 串口序号(非硬件串口号)
 * @param: buffer 发送数据缓存
 * @param: length 发送数据长度
 * 
 * @Note: 
 * @Return: uint8  成功\失败
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8_t SYS_SER_Write(uint8_t port, uint8_t* buffer, uint16_t length, uint32_t to)
{
//    memcpy_s(p_tmpbuf+guc_count, buffer, length);
//    guc_count+=length;

  
  
    return SYS_SER_WriteOption(port, buffer, length, 0);
}





/************************************************************************
 * @Function: SYS_SER_Read
 * @Description: 串口数据读取
 * 
 * @Arguments: 
 * @param: port 串口序号(非硬件串口号)
 * @param: buffer 输出缓存,__NULL表示丢弃
 * @param: length 读取长度
 * 
 * @Note: 
 * @Return: uint16  
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint16 SYS_SER_Read(uint8 port, uint8* buffer, uint16 length, uint32_t to)
{
    SerialBuffer* uart = gss_UartSID[_ucPortMap[port]]->buffer;
    uint16 pr;
    uint16 count;
    uint16 i;
    uint16_t leftLength = length;
                                    //参数合法性判断
    if((_ucPortMap[port] >= NO_OF_SERIAL) || (length == 0))
    {
        return 0;
    }
#ifndef __NO_SYS__     
                                    //判断串口是否已被其他进程占用
//    if(gucs_UartUserTkid[_ucPortMap[port]] != gs_TkTcbCur->tkid)
//    {
//        return 0;
//    }
#endif    
    while(to > 0)
    {
        SYS_ENTER_AllSCRT();
        count = uart->rcnt;
        if(count > length)              //获取实际读取的数据长度
        {
            count = length;
        }
        SYS_EXIT_AllSCRT();
                                        //循环转移数据
        if(__NULL != buffer)            //not dequeue
        {
            pr = uart->pr;              //数据读取起始指针
            for(i = 0; i < count; i++)
            {
                buffer[i] = uart->rbuff[pr];
                pr++;
                if(pr >= uart->rlen)    //卷绕
                {
                    pr = 0;
                }
            }
        }
        else                            //dequeue
        {
#if UART_READ_HOOK_EN > 0
            if(count > (uart->rlen - uart->pr))
            {
                _UartReadHook(_ucPortMap[port], &uart->rbuff[uart->pr], uart->rlen - uart->pr);
                _UartReadHook(_ucPortMap[port], &uart->rbuff[0], count - (uart->rlen - uart->pr));
            }
            else
            {
                _UartReadHook(_ucPortMap[port], &uart->rbuff[uart->pr], count);
            }
#endif
        }
        
        uart->pr += count;              //数据处理指针前移
        while(uart->pr >= uart->rlen)   //卷绕
        {
            uart->pr -= uart->rlen;
        }
        
        SYS_ENTER_AllSCRT();
        uart->rcnt -= count;            //未处理数据长度递减 yzy->rcnt在handler中会被改变
        SYS_EXIT_AllSCRT();

        leftLength -= count;

        if(0 == leftLength)
        {
            break;
        }
        msleep(10);
        to -= 10;
    }
#if UART_READ_HOOK_EN > 0
     _IF_TRUE_DO(__NULL != buffer, _UartReadHook(_ucPortMap[port], buffer, count));
#endif

    return count;
}
/************************************************************************
 * @Function: SYS_SER_Recv_IT
 * @Description: 串口数据读取
 * 
 * @Arguments: 
 * @param: port 串口序号(非硬件串口号)
 * @param: buffer 输出缓存,__NULL表示丢弃
 * @param: length 读取长度
 * 
 * @Note: 
 * @Return: uint16  
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
#if (SYS_UARTSEM_EN > 0)

uint32_t SYS_SER_Recv_IT(uint8 port, void *data, uint32_t expect_size,
                      uint32_t *recv_size, uint32_t timeout)
{
    uint8_t *pdata = (uint8_t *)data;  
    uint32_t rx_count = 0;
//    uint32_t count = 0;
//	    uint16 pr;
//    int32_t ret = -1;
    SerialBuffer* uart = gss_UartSID[_ucPortMap[port]]->buffer;

    if((_ucPortMap[port] >= NO_OF_SERIAL) || (expect_size == 0)
        || __NULL == data)
    {
        return 0;
    }
#ifndef __NO_SYS__     
                                    //判断串口是否已被其他进程占用
    if(gucs_UartUserTkid[_ucPortMap[port]] != gs_TkTcbCur->tkid)
    {
        return 0;
    }
#endif    

    while ( rx_count < expect_size )
    {
//	        SYS_ENTER_AllSCRT();
//	        count = uart->rcnt;
//	        
//	        SYS_EXIT_AllSCRT();
//	                                        //循环转移数据
//	        pr = uart->pr;              //数据读取起始指针
        while(uart->rcnt > 0)
        {
            pdata[rx_count++] = uart->rbuff[uart->pr];
            SYS_ENTER_AllSCRT();
            uart->pr++;
            if(uart->pr >= uart->rlen)    //卷绕
            {
                uart->pr = 0;
            }
            uart->rcnt--;
            SYS_EXIT_AllSCRT();
            if(rx_count==expect_size)
            {
                break;
            }
        }


        if(rx_count==expect_size)
        {
            break;
        }
        if(SYS_ERR_OK == SYS_SEM_Wait((TESRes *)gss_UartSID[_ucPortMap[port]]->uart_rx_sem, timeout))
        {
            while(uart->rcnt > 0)
            {
                pdata[rx_count++] = uart->rbuff[uart->pr];
                SYS_ENTER_AllSCRT();
                uart->pr++;
                if(uart->pr >= uart->rlen)    //卷绕
                {
                    uart->pr = 0;
                }
                uart->rcnt--;
                SYS_EXIT_AllSCRT();
                if(rx_count==expect_size)
                {
                    break;
                }
            }
        }
        else
        {
            break;
        }
    }

    if (recv_size != NULL)
    {
        *recv_size = rx_count;
    }

  
    return rx_count;
}
#endif
/************************************************************************
 * @Function: SYS_SER_Peek
 * @Description: 串口接收数据窥探,执行后数据仍然存在于接收缓存内
 * 
 * @Arguments: 
 * @param: port 串口序号(非硬件串口号)
 * @param: buffer 存放窥探数据的缓存
 * @param: start 从第几个数据开始窥探(0-) 
 * @param: length 待窥探数据长度
 * 
 * @Note: 
 * @Return: uint16  窥探得到的实际数据长度
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint16 SYS_SER_Peek(uint8 port, uint8* buffer, uint16 start, uint16 length)
{
    SerialBuffer* uart = gss_UartSID[_ucPortMap[port]]->buffer;
    uint16 pr;
    uint16 count;
    uint16 i;
                                    //参数合法性判断
    if((_ucPortMap[port] >= NO_OF_SERIAL) || (length == 0))
    {
        return 0;
    }
    
    if(start >= uart->rcnt)         //起始字节数合法性判断
    {
        return 0;
    }
#ifndef __NO_SYS__     
//                                    //判断串口是否已被其他进程占用
//    if(gucs_UartUserTkid[_ucPortMap[port]] != gs_TkTcbCur->tkid)
//    {
//        return 0;
//    }
#endif
    SYS_ENTER_AllSCRT();
    if((start + length) < uart->rcnt)//"待读取长度"与"串口缓存中实际可读数据数"的比较
    {
        count = length;
    }
    else
    {
        count = uart->rcnt - start;
    }
    SYS_EXIT_AllSCRT();
                                    //循环转移数据
    pr = uart->pr + start;          //数据读取起始指针
    if(pr >= uart->rlen)
    {
        pr -= uart->rlen;
    }
    
    for(i = 0; i < count; i++)
    {
        buffer[i] = uart->rbuff[pr];
        pr++;
        if(pr >= uart->rlen)        //卷绕
        {
            pr = 0;
        }
    }

    return count;
}




/************************************************************************
 * @Function: SYS_SER_ReadByte
 * @Description: 从串口读取一个字节数据
 * 
 * @Arguments: 
 * @param: port 串口序号(非硬件串口号)
 * 
 * @Note: 
 * @Return: uint16  
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint16 SYS_SER_ReadByte(uint8 port)
{
    uint8 result;

    if(SYS_SER_Read(port, &result, 1, 0) == 1)
    {
        return result;
    }
    return 256;
}






/************************************************************************
 * @Function: SYS_SER_ToBeRead
 * @Description: 串口可读取数据的长度
 * 
 * @Arguments: 
 * @param: port 串口序号(非硬件串口号)
 * 
 * @Note: 
 * @Return: uint16  
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint16 SYS_SER_ToBeRead(uint8 port)
{
    SerialBuffer* uart = gss_UartSID[_ucPortMap[port]]->buffer;
    uint16 result;
    if(_ucPortMap[port] >= NO_OF_SERIAL)
    {
        return 0;
    }

    SYS_ENTER_AllSCRT();
    result = uart->rcnt;
    SYS_EXIT_AllSCRT();

    return result;
}




/************************************************************************
 * @Function: SYS_SER_Line
 * @Description: 串口读取行命令(若未找到行结束符,串口缓存被读空)
 * 
 * @Arguments: 
 * @param: port 串口序号(非硬件串口号)
 * @param: buffer 存放数据缓存,__NULL表示丢弃
 * @param: length 存放数据缓存的长度
 * 
 * @Note: 
 * @Return: uint16  读取的行长度(包括一个结尾符).
 *          0表示无数据或未找到行结束符,1表示仅读到结尾符
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint16 SYS_SER_Line(uint8 port, uint8* buffer, uint16 length)
{
    SerialBuffer* uart = gss_UartSID[_ucPortMap[port]]->buffer;
    uint16 count;
    uint16 pr;
    uint16 i;
    uint8 sbyte;

    if(_ucPortMap[port] >= NO_OF_SERIAL)
    {
        return 0;
    }

    if(uart->rcnt == 0)             //未处理接收数据长度为0,返回
    {
        return 0;
    }
#ifndef __NO_SYS__     
                                    //判断串口是否已被其他进程占用
//    if(gucs_UartUserTkid[_ucPortMap[port]] != gs_TkTcbCur->tkid)
//    {
//        return 0;
//    }
#endif
    SYS_ENTER_AllSCRT();
    count = uart->rcnt;             //未处理数据长度
    if(count > length)              //操作的数据长度
    {
        count = length;
    }
    SYS_EXIT_AllSCRT();
                                    //循环判定行尾,同时转移数据
    pr = uart->pr;                  //处理指针
    for(i = 0; i < count; i++)      //循环读取数据
    {
        sbyte = uart->rbuff[pr];
        _IF_TRUE_DO(__NULL != buffer, buffer[i] = sbyte);
        pr++;
        if(pr >= uart->rlen)
        {
            pr = 0;
        }
        if((sbyte == '\r') || (sbyte == '\n'))
        {
            break;
        }
    }

    if(i == count)                  //不存在一行内容
    {
        return 0;
    }
    count = i + 1;
    
#if UART_READ_HOOK_EN > 0
    if(__NULL == buffer)
    {
        if(count > (uart->rlen - uart->pr))
        {
            _UartReadHook(_ucPortMap[port], &uart->rbuff[uart->pr], uart->rlen - uart->pr);
            _UartReadHook(_ucPortMap[port], &uart->rbuff[0], count - (uart->rlen - uart->pr));
        }
        else
        {
            _UartReadHook(_ucPortMap[port], &uart->rbuff[uart->pr], count);
        }
    }
#endif
    
    uart->pr += count;              //处理指针前移
    while(uart->pr >= uart->rlen)
    {
        uart->pr -= uart->rlen;
    }
    
    SYS_ENTER_AllSCRT();
    uart->rcnt -= count;            //未处理数据长度递减
    SYS_EXIT_AllSCRT();

    
#if UART_READ_HOOK_EN > 0
     _IF_TRUE_DO(__NULL != buffer, _UartReadHook(_ucPortMap[port], buffer, count));
#endif

    return count;

}







/************************************************************************
 * @Function: SYS_SER_LineLen
 * @Description: 获取串口当前行长度
 * 
 * @Arguments: 
 * @param: port 串口序号(非硬件串口号)
 * 
 * @Note: 
 * @Return: uint16  行长度(包括一个结尾符)
 *          0表示无数据或未找到行结束符,1表示仅读到结尾符
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint16 SYS_SER_LineLen(uint8 port)
{
    SerialBuffer* uart = gss_UartSID[port]->buffer;
    uint16 count;
    uint16 pr;
    uint16 i;

    if(port >= NO_OF_SERIAL)
    {
        return 0;
    }

    if(uart->rcnt == 0)                 //没有为读取数据返回0
    {
        return 0;
    }
#ifndef __NO_SYS__ 
                                        //判断串口是否已被其他进程占用
//    if(gucs_UartUserTkid[port] != gs_TkTcbCur->tkid)
//    {
//        return 0;
//    }
#endif            
    count = uart->rcnt;                 //未处理数据长度
    pr = uart->pr;                      //处理指针
    for(i = 0; i < count; i++)          //循环判断
    {
        if((uart->rbuff[pr] == '\r') || (uart->rbuff[pr] == '\n'))
        {
            break;
        }
        pr++;
        if(pr >= uart->rlen)
        {
            pr = 0;
        }
    }
    
    if(i == count)                      //不存在命令行
    {
        return 0;
    }
    
    count = i + 1;
    
    return count;

}





/************************************************************************
 * @Function: SYS_SER_PrintValueOption
 * @Description: 发送数据,按照字符串的格式化进行发送
 * 
 * @Arguments: 
 * @param: port 串口序号(非硬件串口号)
 * @param: str 发送的描述字符串
 * @param: value 发送的数据
 * @param: opt 选项:b0=1串口性能优先,b0=0系统开销优先
 * 
 * @Note: 
 * @Return: uint8  
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8 SYS_SER_PrintValueOption(uint8 port, string str, uint32 value, uint8 opt)
{
//    uint32 i;
//    uint32 len = strlen(str);
//    uint16 templen;
//    uint8 data[12];
//
//    if(len == 0)                        //如果字符串长度为0则直接返回
//    {
//        SYS_OK();                        //返回处理成功
//    }
//
//    for(i = 0; i < len; i++)            //扫描直到遇到'%'
//    {                                        
//        if(str[i] == '%')               //判断是否为'%'        
//        {                               
//            if(str[i + 1] == 'x' 
//               || str[i + 1] == 'd' 
//               || str[i + 1] == 's'
//               )
//            {                           //打印前面的字符串
//                SYS_SER_WriteOption(port, (uint8*)str, i, opt);
//                break;                  //跳出
//            }
//        }
//    }
//    if(i == len)                        //如果已经扫描完或者最后一个为%则直接退出
//    {                                   //输出信息
//       SYS_SER_WriteOption(port, (uint8*)str, len, opt);
//       SYS_OK();                         //返回处理成功
//    }
//
//    switch(str[i + 1])                  //选择处理字
//    {
//        case 'x':                       //HEX输出  
//                                        //判断是否为'0' - '9'      
//            if(!IsBcdChar(str[i + 2]))
//            {
//                break;                  //不是的话退出
//            }
//                                        //数字到Hex字符串的转换
//            NumToHexString(
//                data, 
//                (uint8*)&value, 
//                (str[i + 2] - '0') >> 1
//                );
//                                        //打印转换出来的数据
//            SYS_SER_WriteOption(
//                port, 
//                data, 
//                str[i + 2] - '0',
//                opt
//                );
//            i++;                        //方便后面的处理
//            break;
//        case 'd':                       //数字字符输出    
//                                        //判断是否为格式化输出
//            if(IsBcdChar(str[i + 2]))  
//            {
//                templen = str[i + 2] - '0';
//                i++;
//            }
//            else
//            {
//                templen = 0;
//            }
//                                        //数字到BCD字符串的转换
//            templen = NumToBcdString(   
//                data, 
//                value, 
//                templen
//                );
//
//            SYS_SER_WriteOption(         //输出数据
//                port, 
//                data, 
//                templen,
//                opt
//                );
//            break;
//        case 's':                       //直接输出字符串        
//            SYS_SER_WriteOption(         //输出字符串
//                port, 
//                (uint8*)value, 
//                strlen((string)value),
//                opt
//                );
//            break;
//    }
//    i += 2;
//    
//    if(i >= len)                        //如果已经扫描完或者最后一个为%则直接退出
//    {
//       SYS_OK();                         //返回处理成功
//    }
//                                        //输出后面的内容
//    SYS_SER_WriteOption(port, (uint8*)str + i, strlen(str + i), opt);

    SYS_OK();                            //返回结果
}





/************************************************************************
 * @Function: SYS_SER_PrintValue
 * @Description: 发送数据,按照字符串的格式化进行发送
 * 
 * @Arguments: 
 * @param: port 串口序号(非硬件串口号)
 * @param: str 发送的描述字符串
 * @param: value 发送的数据
 * 
 * @Note: 
 * @Return: uint8  成功/失败
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8 SYS_SER_PrintValue(uint8 port, string str, uint32 value)
{
    return SYS_SER_PrintValueOption(port, str, value, 0);
}


void UART_Init(void)
{
    SYS_UART_Init();
}

/************************************************************************
 * @Function: SYS_UART_Init
 * @Description: 串口模块初始化
 * @Arguments: 
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void SYS_UART_Init(void)
{

    SerialSets ss;
//    UART_InitTypeDef us;
    ss.baudrate = 9600;
    ss.databits = DataBits_8bits;
    ss.parit = Parit_N;
    ss.stopbits = StopBits_1;
//    ss.flowctrl = FLOW_CONTROL_DISABLED;
//    ss.mode = MODE_TX_RX;
//    _UartSSToUS(ss,&us);

    for(uint8 uc_i = 0; uc_i < NO_OF_SERIAL; uc_i++)
    {
        gucs_UartUserTkid[uc_i] = 0xFF;
#ifndef __NO_SYS__         
//        SYS_SEM_Create(1, &gss_UartRes[uc_i]);
        aos_sem_new(&gss_UartRes[uc_i], 1);

#endif        
        gucs_UartRevFlag[uc_i] = 0;
    
    #if SYS_UARTS_EN > 0
        if(uc_i == SYS_UARTS_IDX)
        {
            Init_UartS(gss_UartSID[uc_i]);
        }
        else
    #endif
        {
            Uartx_Init((SerialID *)gss_UartSID[uc_i], &ss);
        }
    }
}

void SYS_UART_Close(uint8 port)
{
    Uartx_Close(gss_UartSID[_ucPortMap[port]]);

}

void SYS_UART_Open(uint8 port)
{
    Uartx_Open(gss_UartSID[_ucPortMap[port]]);

//	    UART_SInitTypeDef us;
//	    SerialSets ss;
//	    ss.baudrate = 9600;
//	    ss.parit = Parit_N;
//	    ss.databits = DataBits_8bits;
//	    ss.stopbits = StopBits_1;
//	
//	    _UartSSToUS(ss,&us);
//	#if SYS_UARTS_EN > 0
//	    if(_ucPortMap[port] == SYS_UARTS_IDX)
//	    {
//	        UartS_Config(&us, gss_UartSID[_ucPortMap[port]]);
//	    }
//	    else
//	#endif
//	    {
//	        Uartx_Config(&us, gss_UartSID[_ucPortMap[port]]);
//	    }

}

void SYS_UART_DeInit(void)
{
    
    for(uint8 uc_i = 0; uc_i < NO_OF_SERIAL; uc_i++)
    {
        Uartx_DeInit(gss_UartSID[uc_i]);
    }
}




//#if (SYS_UARTEVT_EN > 0)
//#ifdef __NO_SYS__
//#include "define.h"
//#endif
/************************************************************************
 * @Function: _UartRevMessageDelivery
 * @Description: 检查各个进程是否有申请该串口消息,有则发送消息
 * 
 * @Arguments: 
 * @param: uartidx 串口
 * @param: msg_recv 对应的串口接收消息
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
//void _UartRevMessageDelivery(uint8 uartidx, uint8 msg_recv)
//{
//    uint8 grptk;
//    uint8 subtk;
//    uint8 tkmap;
//    int extPort = -1;
//    for(grptk = 0; grptk < sizeof(gucs_MsgUartTkMap[uartidx]); grptk++)
//    {
//        tkmap = gucs_MsgUartTkMap[uartidx][grptk];
//        extPort = _FindExtPort(uartidx);
//        if(extPort < 0)
//            return;
//        while(tkmap)
//        {
//            subtk = FBit_Maps(tkmap);
//#ifndef __NO_SYS__ 
//            SYS_Message_Send(msg_recv+extPort, (grptk << 3) + subtk);
//#else
//            //extern uint8_t  gucSMskEvt[S_MSK_MAXMDL];
////	            gucSMskEvt[ (grptk << 3) + subtk] |= 1 << uartidx;
//            SysSndMsg( (grptk << 3) + subtk,  msg_recv + extPort);
//#endif
//            tkmap &= ~Bit_Map8[subtk];
//        }
//    }
//}
//#endif

void _UartRevMessageDelivery(uint8 uartidx, uint8 msg_recv)
{
    uint8 subtk;
    uint8 tkmap;
    
    extern const KTaskDeclare __TKDeclare[SYS_TK_NUM];
    const KTaskDeclare* dec = __TKDeclare;
    
    for(uint8 uc_i = 0; uc_i < sizeof(guc_MsgUartTkMap[0]); uc_i++)
    {
        tkmap = guc_MsgUartTkMap[uartidx][uc_i];
        while(tkmap)
        {
            subtk = Bit_Maps[tkmap];
//	            SYS_Message_Send(msg_recv, (uc_i << 3) + subtk);
//	
//	            msg = MSG_YEAR;
//	            dec+=(uc_i << 3) + subtk;
            krhino_buf_queue_send(dec[(uc_i << 3) + subtk].ktask->msg, &msg_recv, 1);

            
            tkmap &= ~Bit_Map8[subtk];
        }
    }
}


/************************************************************************
 * @Function: SYS_UART_EvtMessageLoop
 * @Description: UART串口数据接收消息分发机制
 * @Arguments: 
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void SYS_UART_EvtMessageLoop(void)
{
#if (SYS_UARTEVT_EN > 0)
    for(uint8 uartidx = 0; uartidx < NO_OF_SERIAL; uartidx++)
    {
        if(gucs_UartRevFlag[uartidx])
        {                               //存在回调函数,则执行
            if(gfs_SerialCallBack[uartidx] != __NULL)
            {
                gfs_SerialCallBack[uartidx]();
            }
            else                        //否则,进行接收消息分发
            {
                _UartRevMessageDelivery(uartidx, MSG_UART_IDX0);
            }
            gucs_UartRevFlag[uartidx] = 0;
        }
    }
#endif
}


