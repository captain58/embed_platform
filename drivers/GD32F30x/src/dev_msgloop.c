/****************************************Copyright (c)*************************
**                               	________科技有限公司
**                                     		开发部
**
**
**--------------文件信息-------------------------------------------------------
**文   件   名: dev_messageloop.c
**创   建   人: yzy
**最后修改日期: 2011年04月27日
**描        述: 各状态量处理管理进程
**注        意:
**--------------历史版本信息---------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2011年04月27日
** 描　述: 原始版本
******************************************************************************/
#define EXT_DEV_MSGLOOP

#include "aos/include.h"
//	#include "k_types.h"
//	#include "k_api.h"
//	//	#include "lib_app.h"
#include "hal.h"
#include "bsp.h"
#include "tasks.h"

extern uint8 gucs_UartRevFlag[NO_OF_SERIAL];
/************************************************************************
 * @function: UartRevMessageDelivery
 * @描述: 将某个串口接收消息,分发给申请该消息的进程
 * 
 * @参数: 
 * @param: uartidx 串口
 * @param: msg_recv 对应的串口接收消息
 * @返回: 
 * @说明: 
 * @作者: yzy (2013/11/4)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void UartRevMessageDelivery(uint8 uartidx, uint8 msg_recv)
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



/*******************************************************************************
 * @function_name:  SYS_UART_RevMessageLoop
 * @function_file:  dev_msgloop.c
 * @描述: UART串口数据接收消息分发机制
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
void SYS_UART_RevMessageLoop(void)
{
    for(uint8 uartidx = 0; uartidx < NO_OF_SERIAL; uartidx++)
    {
//	        if(SER_ToBeRead_By_Port(uartidx) > 0)
        if(gucs_UartRevFlag[uartidx])
        {                               //存在回调函数,则执行
//	            if(gfs_SerialCallBack[uartidx] != __NULL)
//	            {
//	                gfs_SerialCallBack[uartidx]();
//	            }
//	            else                        //否则,进行接收消息分发
//	            {
            UartRevMessageDelivery(uartidx, MSG_UART0 + uartidx);
//	            }
            gucs_UartRevFlag[uartidx] = 0;
        }
    }
}



/*******************************************************************************
* @function_name:  MSG_Server
* @function_file:  dev_messageloop.c
* @描述: 消息分配管理进程处理函数
* 
* 
* @参数: 
* 
* @返回: 
* @作者: yzy (2011-02-24)
*-----------------------------------------------------------------------------
* @修改人: 
* @修改说明: 
******************************************************************************/
void MSG_Server(void *timer, void *arg)
{
#if SYS_GPI_EN > 0
    SYS_GPI_EvtMessageLoop();
#endif
#if SYS_SER_EN > 0
//    SYS_SER_RevMessageLoop();

    SYS_UART_RevMessageLoop();
#endif
#if (SYS_LGPI_EN > 0)
    extern void SYS_LGPI_Scan(void *timer, void *arg);

    SYS_LGPI_Scan(timer, arg);
#endif

#if (SYS_FGPI_EN > 0)
    extern void SYS_FGPI_Scan(void *timer, void *arg);

    SYS_FGPI_Scan(timer, arg);
#endif
    
#if (SYS_LED_BEEP_EN > 0)
    extern void LED_Server(void *timer, void *arg);
	
    LED_Server(timer,arg);
#endif    
}


ktimer_t     g_msg_server_timer;


/*******************************************************************************
* @function_name:  SYS_MSG_Init
* @function_file:  dev_messageloop.c
* @描述: 消息分配管理初始化
* 
* 
* @参数: 
* 
* @返回: 
* @作者: yzy (2011-02-24)
*-----------------------------------------------------------------------------
* @修改人: 
* @修改说明: 
******************************************************************************/
void SYS_MSG_Init(void)
{
    ClearBuffer(guc_MsgApplied, sizeof(guc_MsgApplied));
    ClearBuffer((uint8*)guc_MsgUartTkMap, sizeof(guc_MsgUartTkMap));
    
    //创建消息分发服务定时器
//	    SYS_Timer_Create(MSG_Server, __NULL, 1, TIMER_ID_MSG, false);
    krhino_timer_create(&g_msg_server_timer, "msg_server_timer", MSG_Server,
                        krhino_ms_to_ticks(100), krhino_ms_to_ticks(100), 0, 1);


}




/*******************************************************************************
* @function_name:  SYS_MSG_Apply
* @function_file:  dev_messageloop.c
* @描述: 申请消息
* 
* 
* @参数: msgcls 消息大类
* @返回:  
* @作者: yzy (2011-02-24)
******************************************************************************/
uint8_t SYS_MSG_Apply(uint32_t tkid, uint8_t msgcls)
{
    
    if(msgcls >= 8)
    {
        return false;
    }

    
    if(msgcls == MSG_CLS_UART)
    {
        for(uint8 index = 0; index < NO_OF_SERIAL; index++)
        {
            STR_SetBit(guc_MsgUartTkMap[index], tkid);
        }
    }
                                            //标记大类
    guc_MsgApplied[tkid] |= Bit_Map8[msgcls];
    if(msgcls == MSG_CLS_GPIO)
    {
        SYS_GPI_Event_Clear();
    }
    return true;
}


/*******************************************************************************
* @function_name:  SYS_MSG_ApplyUartRecv
* @function_file:  dev_messageloop.c
* @描述: 申请消息(扩充函数)
* 
* 
* @参数: msgcls 消息大类
*        subcls 消息小类,当大类为串口消息时,小类为串口号
* @返回:  
* @作者: yzy (2011-02-24)
******************************************************************************/
uint8 SYS_MSG_ApplyExt(uint8 tkid, uint8 msgcls, uint8 subcls )
{
//	    uint8 tkid = gs_TkTcbCur->tkid;
//	    
    if(msgcls >= 8)
    {
        return false;
    }
                                            //串口类消息,标记子类为串口编号
    if(msgcls == MSG_CLS_UART)
    {
//        uint8_t port = ((SerialID*)__Uart_Find(subcls))->port;
//        if(port > NO_OF_SERIAL)
//        {
//            return false;
//        }
        STR_SetBit(guc_MsgUartTkMap[subcls], tkid);
    }
                                            //标记大类
    guc_MsgApplied[tkid] |= Bit_Map8[msgcls];
        
    return true;
}



