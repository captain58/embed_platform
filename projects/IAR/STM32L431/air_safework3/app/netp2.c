/****************************************Copyright (c)**************************************************
**                                  ____科技有限公司
**                                          开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: netp2.c
**创   建   人: yzy
**最后修改日期: 2007年9月5日
**描        述: 配变程序组网通讯端口的通用函数
**注        意:
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2007年9月5日
** 描　述: 原始版本
********************************************************************************************************/
#define EXT_NETP2
#include "public.h"
#include "lib_com/calclib_gw.h"

#include "netp.h"
#include "hlv.h"
//	#include "netp_ditbl.c"
#include "paradef.h"
#include "pst_zy.h"
#include "evt.h"
#include "netp_fm.h"

#include "task.h"



void Netp_SecProc(uint8 index);
void Netp_InitData(uint8 index);
void Netp_CheckTstTime(uint8 index);
void SynToF10Para(uint8 index);

cpu_stack_t  gs_TKNetp2Stack[TASK_NETP_STKL];
ktask_t      gs_TKNetp2Handle;
kbuf_queue_t gs_TKNetp2Queue;
char         gc_TKNetp2Qbuf[MSG_BUFF_LEN];
static char         g_TKNetp2Q_buf_recv[MSG_BUFF_LEN];
static size_t       g_TKNetp2Q_recv_size;


static void Port_Signin(uint8_t port)
{
    uint8_t msg[2];
    msg[0] = MSG_NETP_TASK_RUN_FLAG_PER_485;
    msg[1] = port;
    extern kbuf_queue_t gs_TKSlvQueue;
    kstat_t t = gs_OS.Message_Send(&gs_TKSlvQueue, msg,2);
    LOG_DEBUG("port[%d] CP flag note slv to sign [%d] \n", port, t);

}
/************************************************************************
 * @function: ES_NETP2_Task
 * @描述: 组网进程处理函数
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/2/11)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void SYS_NETP2_Task(void)
{
    //数字消息
    krhino_buf_queue_create(&gs_TKNetp2Queue, "netp2_queue",
                         gc_TKNetp2Qbuf, MSG_BUFF_LEN, BUFQUEUE_MSG_MAX);
    
    //SYS_MSG_Apply(TASK_NETP2_TKID, MSG_CLS_TM);
    aos_msleep(500);
//	    Port_Signin(1);
    SetTaskRunFlag(1);
    for(;;)
    {
        krhino_buf_queue_recv(&gs_TKNetp2Queue, RHINO_WAIT_FOREVER, g_TKNetp2Q_buf_recv,
                                  &g_TKNetp2Q_recv_size);
        switch(g_TKNetp2Q_buf_recv[0])                        //根据消息分别处理
        {
            case MSG_SEC:
                //Netp_SecProc();
                break;

            case MSG_NETP_CURDATA:            //端口2数据抄读开始
                Netp_InitData(1);
                break;

            case MSG_NETP_CKTIME:
//	                Netp_CheckTstTime(1);
                break;
            case MSG_NETP_CHECK:
                Netp_Check(1);
                break;
            case MSG_LIVE:
				HB_RetLive(TASK_NETP2_TKID);
                break;
        }
    }
}
/******************************************************************************
**抄表进程块常量部分定义
******************************************************************************/
const KTaskConst gs_TkNetp2 = 
{
    &gs_TKNetp2Handle,
    SYS_NETP2_Task,                           //应用管理进程
    gs_TKNetp2Stack,
    TASK_NETP2_TKID,                           //进程编号
    true,                                    //消息循环机制进程
    TASK_NETP_STKL,
    "netp",
    &gs_TKNetp2Queue,
};


cpu_stack_t  gs_TKNetp3Stack[TASK_NETP_STKL];
ktask_t      gs_TKNetp3Handle;
kbuf_queue_t gs_TKNetp3Queue;
char         gc_TKNetp3Qbuf[MSG_BUFF_LEN];
static char         g_TKNetp3Q_buf_recv[MSG_BUFF_LEN];
static size_t       g_TKNetp3Q_recv_size;

/************************************************************************
 * @function: SYS_NETP3_Task
 * @描述: 组网进程处理函数
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/2/11)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void SYS_NETP3_Task(void)
{
    //数字消息
    krhino_buf_queue_create(&gs_TKNetp3Queue, "netp3_queue",
                         gc_TKNetp3Qbuf, MSG_BUFF_LEN, BUFQUEUE_MSG_MAX);
    
    //SYS_MSG_Apply(TASK_NETP3_TKID, MSG_CLS_TM);
    aos_msleep(500);
//	    Port_Signin(2);
    SetTaskRunFlag(2);

    for(;;)
    {
        krhino_buf_queue_recv(&gs_TKNetp3Queue, RHINO_WAIT_FOREVER, g_TKNetp3Q_buf_recv,
                                  &g_TKNetp3Q_recv_size);
        switch(g_TKNetp3Q_buf_recv[0])                        //根据消息分别处理
        {
            case MSG_SEC:
                //Netp_SecProc();
                break;

            case MSG_NETP_CURDATA:            //端口2数据抄读开始
                Netp_InitData(2);
                break;

            case MSG_NETP_CKTIME:
                Netp_CheckTstTime(2);
                break;
            case MSG_NETP_CHECK:
                Netp_Check(2);
                break;
            case MSG_LIVE:
				HB_RetLive(TASK_NETP3_TKID);
                break;
        }
    }
}
/******************************************************************************
**抄表进程块常量部分定义
******************************************************************************/
const KTaskConst gs_TkNetp3 = 
{
    &gs_TKNetp3Handle,
    SYS_NETP3_Task,                           //应用管理进程
    gs_TKNetp3Stack,
    TASK_NETP3_TKID,                           //进程编号
    true,                                    //消息循环机制进程
    TASK_NETP_STKL,
    "netp",
    &gs_TKNetp3Queue,
};

cpu_stack_t  gs_TKNetp4Stack[TASK_NETP_STKL];
ktask_t      gs_TKNetp4Handle;
kbuf_queue_t gs_TKNetp4Queue;
char         gc_TKNetp4Qbuf[MSG_BUFF_LEN];
static char         g_TKNetp4Q_buf_recv[MSG_BUFF_LEN];
static size_t       g_TKNetp4Q_recv_size;

/************************************************************************
 * @function: SYS_NETP4_Task
 * @描述: 组网进程处理函数
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/2/11)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void SYS_NETP4_Task(void)
{
    //数字消息
    krhino_buf_queue_create(&gs_TKNetp4Queue, "netp4_queue",
                         gc_TKNetp4Qbuf, MSG_BUFF_LEN, BUFQUEUE_MSG_MAX);
    
    //SYS_MSG_Apply(TASK_NETP4_TKID, MSG_CLS_TM);
    aos_msleep(500);
//	    Port_Signin(3);
    SetTaskRunFlag(3);

    for(;;)
    {
        krhino_buf_queue_recv(&gs_TKNetp4Queue, RHINO_WAIT_FOREVER, g_TKNetp4Q_buf_recv,
                                  &g_TKNetp4Q_recv_size);
        switch(g_TKNetp4Q_buf_recv[0])                        //根据消息分别处理
        {
            case MSG_SEC:
                //Netp_SecProc();
                break;

            case MSG_NETP_CURDATA:            //端口2数据抄读开始
                Netp_InitData(3);
                break;

            case MSG_NETP_CKTIME:
                Netp_CheckTstTime(3);
                break;
            case MSG_NETP_CHECK:
                Netp_Check(3);
                break;
            case MSG_LIVE:
				HB_RetLive(TASK_NETP4_TKID);
                break;
        }
    }
}
/******************************************************************************
**抄表进程块常量部分定义
******************************************************************************/
const KTaskConst gs_TkNetp4 = 
{
    &gs_TKNetp4Handle,
    SYS_NETP4_Task,                           //应用管理进程
    gs_TKNetp4Stack,
    TASK_NETP4_TKID,                           //进程编号
    true,                                    //消息循环机制进程
    TASK_NETP_STKL,
    "netp",
    &gs_TKNetp4Queue,
};






