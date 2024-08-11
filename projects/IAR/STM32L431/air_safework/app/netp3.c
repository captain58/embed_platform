/****************************************Copyright (c)**************************************************
**                                  ____科技有限公司
**                                          开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: netp3.c
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
#define EXT_NETP3
#include "es.h"
#include "hal.h"
#include "bsp.h"
#include "app.h"
#include "apis.h"
#include "extapi.h"



void Netp_SecProc(uint8 index);
void Netp_InitData(uint8 index);
void Netp_CheckTstTime(uint8 index);
void SynToF10Para(uint8 index);




/************************************************************************
 * @function: ES_NETP3_Task
 * @描述: 组网进程处理函数
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/2/11)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void ES_NETP3_Task(void)
{
                                            //链表消息
    TESMsg* msg = (TESMsg*)gs_Mem.GetMEM(sizeof(TESMsg));
    uint8 nmsg;                             //数字消息
    
    
    ES_MSG_Apply(MSG_CLS_TM);
    
    for(;;)
    {
        nmsg = gs_OS.Message_Wait(&msg);    //获取进程消息
        switch(nmsg)                        //根据消息分别处理
        {
            case MSG_SEC:
                Netp_SecProc(2);
                break;

            case MSG_NETP_IDATA:            //端口3数据抄读开始
                Netp_InitData(2);
                PST_UART_Init();
                break;

            case MSG_NETP_CKTIME:
                Netp_CheckTstTime(2);
                PST_UART_Init();
                break;
                
            case MSG_LIVE:
				HB_RetLive();
                break;
        }
    }
}

/******************************************************************************
**组网进程块常量部分定义
******************************************************************************/
const TESTcbConst gs_TkNetp3 = 
{
    ES_NETP3_Task,
    TASK_NETP3_TKID >> 3,                   //应用管理进程
    TASK_NETP3_TKID,                        //进程编号
    TASK_NETP3_TKID & 7,                    //优先级内进程编号位
    true,                                      //消息循环机制进程
};

