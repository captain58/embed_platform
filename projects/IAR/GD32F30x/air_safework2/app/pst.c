/****************************************Copyright (c)**************************************************
**                                  ____科技有限公司
**                                          开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: pst.c
**创   建   人: yzy
**最后修改日期: 2013年7月3日
**描        述: 被动通讯应用管理进程
**注        意:
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2018年3月22日
** 描　述: 原始版本
**-------------------------------------------------------------------------------------------------------
** 修改人: 
** 版  本: 
** 描  述: 
********************************************************************************************************/
#define EXT_PST

#include <k_api.h>
#include <stdio.h>
#include <stdlib.h>
#include "public.h"





/************************************************************************
 * @function: PstFrmProc
 * @描述: 被动通讯处理函数(将FRM设备进行封装,各进程可直接调用该函数)
 * 
 * @参数: 
 * @param: channel  通道号;不同的通道需要处理不通的规约
 * @param: rbuffer  接收缓存
 * @param: length  接收数据内容长度
 * @param: sbuffer 
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2018年3月22日)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 PstFrmProc(PORT_UART_TYPE channel, uint8* rbuffer, uint16 length, uint8* sbuffer, uint16 sbuflen)
{
    PST_Frame frm;                          //规约帧结构体
    uint8 protocal;                         //规约号
    uint16 bak;
    uint8 err;
    
    SYS_VAR_CHECK(channel >= PST_CHN_NO);    //参数检查
                                            //通道规约帧判断
    if(PST_Channel(channel, rbuffer, length, &protocal) != PST_ERR_OK)
    {
        return PST_ERR_NE;
    }

    frm.chninfo = channel & PST_FRM_CHNNO;  //通道号
    
                                            //规约帧解析
    if(FRM_Decode(protocal, rbuffer, sbuffer, sbuflen, &frm) == PST_ERR_OK)
    {
        FRM_Proc(protocal, &frm);        //规约处理
    }
    
    if(channel == PST_CHN_TCP)
    {
#if SYS_MODEM_EN > 0
        extern kbuf_queue_t gs_TKFarpQueue;
        uint8 msg = MSG_FARP_RECVVALIDIP;
        krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
#endif        
    }
//	    else if(channel == PST_CHN_SER)
//	    {
//	        uint8 msg = MSG_FARP_RECVVALIDSER;
//	        krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
//	        
//	    }

    SYS_OK();
}






/************************************************************************
 * @function: PST_CHN0_Send
 * @描述: 通道发送
 * 
 * @参数: 
 * @param: buffer 发送数据的缓存
 * @param: len 发送数据的长度
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2018年3月22日)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 PST_CHN0_Send(uint8* buffer, uint16 len)
{                                       //亮灯
    gs_GPIO.BlinkSet(GPIO_LED_485, 0, 0, 0);
                                        //
    SYS_SER_Open(UART_CHANNEL_DEBUG, TDB_MODE_W);
    SYS_SER_Write(UART_CHANNEL_DEBUG, buffer, len, 1000);
    //SER_Close(UART_CHANNEL_DEBUG);
    aos_msleep(500);
    LOG_DUMPHEX(LOG_LEVEL_DEBUG, "coms:\n", buffer, len);
    SYS_OK();
}




/************************************************************************
 * @function: PST_CHN1_Send
 * @描述: 通道发送
 * 
 * @参数: 
 * @param: buffer 发送数据的缓存
 * @param: len 发送数据的长度
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2018年3月22日)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 PST_CHN1_Send(uint8* buffer, uint16 length)
{
#if SYS_MODEM_EN > 0
        return Farp_SendIpData(buffer, length);
#else
        return 0;
#endif    
}


/************************************************************************
 * @function: PST_CHN2_Send
 * @描述: 通道发送
 * 
 * @参数: 
 * @param: buffer 发送数据的缓存
 * @param: len 发送数据的长度
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2018年3月22日)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 PST_CHN2_Send(uint8* buffer, uint16 length)
{
    gs_GPIO.BlinkSet(GPIO_LED_485, 0, 0, 0);
                                        //
    SYS_SER_Open(NETP_PORT_NO, TDB_MODE_W);
    SYS_SER_Write(NETP_PORT_NO, buffer, length, 1000);
    SYS_SER_Close(NETP_PORT_NO);
    SYS_OK();

}

uint8 PST_CHN3_Send(uint8* buffer, uint16 length)
{
    gs_GPIO.BlinkSet(GPIO_LED_485, 0, 0, 0);
                                        //
    SYS_SER_Open(NETP2_PORT_NO, TDB_MODE_W);
    SYS_SER_Write(NETP2_PORT_NO, buffer, length, 1000);
    SYS_SER_Close(NETP2_PORT_NO);
    SYS_OK();

}
uint8 PST_CHN4_Send(uint8* buffer, uint16 length)
{
    gs_GPIO.BlinkSet(GPIO_LED_485, 0, 0, 0);
                                        //
    SYS_SER_Open(NETP3_PORT_NO, TDB_MODE_W);
    SYS_SER_Write(NETP3_PORT_NO, buffer, length, 1000);
    SYS_SER_Close(NETP3_PORT_NO);
    SYS_OK();

}
uint8 PST_CHN5_Send(uint8* buffer, uint16 length)
{
    gs_GPIO.BlinkSet(GPIO_LED_485, 0, 0, 0);
                                        //
    SYS_SER_Open(NETP4_PORT_NO, TDB_MODE_W);
    SYS_SER_Write(NETP4_PORT_NO, buffer, length, 1000);
    SYS_SER_Close(NETP4_PORT_NO);
    SYS_OK();

}

//	/************************************************************************
//	 * @function: PST_CHN3_Send
//	 * @描述: 通道发送-组网
//	 * 
//	 * @参数: 
//	 * @param: buffer 发送数据的缓存
//	 * @param: len 发送数据的长度
//	 * 
//	 * @返回: 
//	 * @return: uint8  
//	 * @说明: 
//	 * @作者: yzy (2018年3月22日)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
//	 ************************************************************************/
//	uint8 PST_CHN3_Send(uint8* buffer, uint16 length)
//	{
//	    uint8 err;                          //返回结果
//	    uint16 tlen;
//	                                        //打开串口
//	    gs_Uart.open(NETP_PORT_NO, false);
//	                                        //写入数据到串口
//	    while(length > 0)
//	    {
//	        tlen = length > 256 ? 256 : length;
//	        err = gs_Uart.Write(NETP_PORT_NO, buffer, tlen);
//	        buffer += tlen;
//	        length -= tlen;
//	    }
//	    gs_Uart.close(NETP_PORT_NO);             //关闭串口
//	                                
//	    return err;                             //返回数据
//	}
//	
//	
//	
//	/************************************************************************
//	 * @function: PST_CHN4_Send
//	 * @描述: 通道发送
//	 * 
//	 * @参数: 
//	 * @param: buffer 发送数据的缓存
//	 * @param: len 发送数据的长度
//	 * 
//	 * @返回: 
//	 * @return: uint8  
//	 * @说明: 
//	 * @作者: yzy (2018年3月22日)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
//	 ************************************************************************/
//	uint8 PST_CHN4_Send(uint8* buffer, uint16 length)
//	{
//	    return Farp_SendTcpServerData(buffer, length);
//	}











/************************************************************************
 * @function: PST_Init
 * @描述: 被动进程初始化
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2018年3月22日)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void PST_Init(void)
{

    SYS_SER_Init(UART_CHANNEL_DEBUG, NULL);    //初始化串口   
    
//	    void InitFileDownLoad(void);
//	    InitFileDownLoad();
    
    gs_PstPara.repwait = 80;
    gs_PstPara.reptry = 3;
    
    LoadSystemParam(PARA_TYPE_PST);
}


//	
//	/************************************************************************
//	 * @function: PST_PreInit
//	 * @描述: 被动进程初始化
//	 * @参数: 
//	 * @返回: 
//	 * @说明: 
//	 * @作者: yzy (2018年3月22日)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
//	 ************************************************************************/
//	void PST_PreInit(void)
//	{
//	                                        //建立简单标识队列
//	    gs_Array.FIFO_Create(
//	        size_array_4,
//	        &gs_FifoAfnReady,
//	        gss_AfnReadyTeam,
//	        sizeof(gss_AfnReadyTeam)
//	    );
//	                                        //初始化事件计数器备份
//	    guc_e1bk = gs_FramMisData.e1cnt;
//	    guc_e2bk = gs_FramMisData.e2cnt;
//	    
//	    void InitFileDownLoad(void);
//	    InitFileDownLoad();
//	    
//	    LoadSystemParam(PARA_TYPE_PST);
//	    
//	}



uint8 g_pdata[512];
/************************************************************************
 * @function: Setp_RecvProc
 * @描述: 设置端口接收到数据之后的数据处理
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2018年3月22日)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void Setp_RecvProc(void)
{
    uint16 length;
    
    //SER_Open(UART_CHANNEL_DEBUG, TDB_MODE_R | TDB_MODE_W);
    length = SYS_SER_Read(UART_CHANNEL_DEBUG, g_pdata, 256, 0);
    //SER_Close(UART_CHANNEL_DEBUG);
//	    while(gs_Uart.ToBeRead(SETP_PORT_NO) > 384)
//	    {
//	        length = gs_Uart.Read(SETP_PORT_NO, pdata, 384);
//	    }
//	    length = gs_Uart.Read(SETP_PORT_NO, pdata, 384);
//	    gs_Uart.close(SETP_PORT_NO);

#ifdef DEBUG_ATMODEM_TRANS
    //GPRS_PORT<->PST_PORT
    gs_Uart.open(SYS_GPRS_PORT, false);
    gs_Uart.Write(SYS_GPRS_PORT, pdata, length);
    gs_Uart.close(SYS_GPRS_PORT);
    return;
#else
    
    aos_msleep(20);
                                        //处理被动通道0的数据
    if(length > 1)
    {
        LOG_DUMPHEX(LOG_LEVEL_DEBUG, "recv:\n", g_pdata, length);
        if(SYS_ERR_OK == PstFrmProc(PST_CHN_SET, g_pdata, length, g_pdata, sizeof(g_pdata)));
    }
#endif
}




#ifdef DEBUG_ATMODEM_TRANS
/************************************************************************
 * @function: Mdmp_RecvProc
 * @描述: GPRS_PORT<->PST_PORT
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2018年3月22日)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void Mdmp_RecvProc(void)
{
    uint8 pdata[384];
    uint16 length;
    
    gs_Uart.open(SYS_GPRS_PORT, false);
    while(gs_Uart.ToBeRead(SYS_GPRS_PORT) > 384)
    {
        length = gs_Uart.Read(SYS_GPRS_PORT, pdata, 384);
    }
    length = gs_Uart.Read(SYS_GPRS_PORT, pdata, 384);
    gs_Uart.close(SYS_GPRS_PORT);

    gs_Uart.open(SETP_PORT_NO, false);
    gs_Uart.Write(SETP_PORT_NO, pdata, length);
    gs_Uart.close(SETP_PORT_NO);
    return;
}
#endif


/************************************************************************
**函数名称: void PST_UpdateVar(void* pdata)
**描 述: 链路层参数初始化
**
**输 入: 
**输 出: 
**
**备 注: 未对数据有效性进行检查
**-----------------------------------------------------------------------
**作 者: yzy
**日 期: 08-09-09
*************************************************************************/
void PST_UpdateVar(void)
{
    LoadSystemParam(PARA_TYPE_PST);
}







//	/************************************************************************
//	 * @function: SYS_PST_Task
//	 * @描述: 被动通讯应用管理进程处理函数
//	 * @参数: 
//	 * @返回: 
//	 * @说明: 
//	 * @作者: yzy (2018年3月22日)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
//	 ************************************************************************/
//	void SYS_PST_Task(void)
//	{
//	                                            //链表消息
//	    TESMsg* msg = (TESMsg*)gs_Mem.GetMEM(sizeof(TESMsg));
//	    uint8 nmsg;                             //数字消息
//	    
//	    PST_Init();
//	                                            //申请串口消息
//	    SYS_MSG_ApplyExt(MSG_CLS_UART, SETP_PORT_NO);
//	#ifdef DEBUG_ATMODEM_TRANS
//	    SYS_MSG_ApplyExt(MSG_CLS_UART, SYS_GPRS_PORT);
//	#endif
//	    
//	    void GdUp_HXWDTTst(void);               //看门狗测试
//	    GdUp_HXWDTTst();
//	    //看门狗测试启动上报
//	    void WdtTestExeStartPush(void);
//	    WdtTestExeStartPush();
//	    
//	    for(;;)
//	    {
//	        nmsg = gs_OS.Message_Wait(&msg); 
//	        switch(nmsg)
//	        {
//	            case MSG_PSTP:                  //设置口被动通讯处理
//	                Setp_RecvProc();
//	                break;
//	                
//	            case MSG_PST_VAR:
//	                PST_UpdateVar();
//	                break;
//	#ifdef DEBUG_ATMODEM_TRANS
//	            case MSG_MDMP:
//	                Mdmp_RecvProc();
//	                break;
//	#endif
//	                
//	            case MSG_LIVE:
//					HB_RetLive();
//	                break;
//	        }
//	    }
//	}
//	
//	/************************************************************************
//	**被动通讯进程块常量部分定义
//	************************************************************************/
//	const TESTcbConst gs_TkPst = 
//	{
//	    SYS_PST_Task,
//	    TASK_PST_TKID >> 3,                    //以太网应用管理进程
//	    TASK_PST_TKID,                         //进程编号
//	    TASK_PST_TKID & 7,                     //优先级内进程编号为
//	    true,                                  //消息循环机制进程
//	};

