/****************************************Copyright (c)**************************************************
**                                  ________科技有限公司
**                                          开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: tasks.c
**创   建   人: yzy
**最后修改日期: 2018年3月7日
**描        述: 应用层任务模块
**注        意:
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2018年3月7日
** 描　述: 原始版本
********************************************************************************************************/
#define EXT_TASKS
//	#include "es.h"
//	#include "hal.h"
//	#include "bsp.h"
//	#include "app.h"
//	#include "apis.h"
//	#include "extapi.h"
#include <k_api.h>
#include <stdio.h>
#include <stdlib.h>
#include "public.h"


/*******************************************************************************
 * @function_name:  
 * @function_file: 
 * @描述: 所有任务的结构体
 * 
 * 
 * @参数: 
 * @param: 
 * 
 * @返回: 
 * @return:   
 * @作者: houpm 
 *-----------------------------------------------------------------------------
 * @修改人   yzy
 * @修改人   注意.8个进程一组,编号为0-7,各组进程的调度间隔如下
 *        0、1:1个tick
 *        2、3:2个tick
 *        4、5:4个tick
 *        6、7:8个tick
 ******************************************************************************/
const KTaskDeclare __TKDeclare[SYS_TK_NUM] = 
{   //进程0-7
    {__NULL, 0},                        //系统进程启动时已经初始化,不需要声明(gs_TkSystemc,栈gt_TkSystemStk[SYS_SYS_STK_LEN])
    TASK_DECLARE_1_MAIN(),              //meter控制进程
    {__NULL, 0},//TASK_DECLARE_2_HLV(),//{__NULL, 0},
    {__NULL, 0},//TASK_DECLARE_3_FARP(),               //远程通道管理进程
    {__NULL, 0},//TASK_DECLARE_4_NETP(),//{__NULL, 0},
    {__NULL, 0},//TASK_DECLARE_5_NETP(),//{__NULL, 0},
    {__NULL, 0},//TASK_DECLARE_6_NETP(),//{__NULL, 0},
    {__NULL, 0},//TASK_DECLARE_7_NETP(),//
    {__NULL, 0},
#ifndef MASTER_NODE     
    TASK_DECLARE_9_SLV(),
#else
    {__NULL, 0}, 
#endif    
    TASK_DECLARE_10_RFMNG(),
    
};

/***********************************************************
 * @function_name: SYS_TASKS_Init
 * @function_file: SYS_core.c
 * @描述:所有用户进程的创建
 * 
 * @参数: 
 * @返回: 
 * @作者:
 *---------------------------------------------------------
 * @修改人: yzy (2010/2/11)
 **********************************************************/
void SYS_TASKS_Init(void)
{
    extern const KTaskDeclare __TKDeclare[SYS_TK_NUM];
    const KTaskDeclare* dec = __TKDeclare;

    //ktask_t * handle;
    for(uint32 k = 0; k < (SYS_TK_NUM ); k++, dec++)   //循环初始化
    {
        if(dec->ktask == __NULL || dec->stklen == 0)
        {
            continue;
        }
        
        //开始创建进程
        memset(dec->ktask->stack, 0xFF, dec->stklen * sizeof(cpu_stack_t));//yzy 初始化栈,便于观察是否溢出
        krhino_task_create(dec->ktask->handle, dec->ktask->name, dec->ktask->arg, dec->ktask->prior, 
            50, dec->ktask->stack, dec->stklen, dec->ktask->pfunc, 1);

        aos_msleep(100);

    }
}


