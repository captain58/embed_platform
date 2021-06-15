
#include <k_api.h>
#include <stdio.h>
#include <stdlib.h>
#include "public.h"
#include "app.h"
#include "pst.h"
//#include "hlv.h"
//#include "farp.h"
//#include "netp.h"
#include "task.h"

/******************************************************************************
**MAIN进程块常量部分定义
******************************************************************************/
const KTaskConst gs_TkLP2PCmm = 
{
    &gs_MainHandle,
    SYS_MAIN_Task,
    gs_MainStack,
    TASK_MAIN_TKID,                           //进程编号
    true,                                    //消息循环机制进程
    TASK_MAIN_STKL,
    "main",
    &gs_MainQueue,
};