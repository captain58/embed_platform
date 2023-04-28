/****************************************Copyright (c)**************************************************
**                                  ________科技有限公司
**                                          开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: dev_dec.h
**创   建   人: yzy
**最后修改日期: 2018年3月7日
**描        述: esos操作系统设备的声明文件
**注        意:
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2018年3月7日
** 描　述: 原始版本
********************************************************************************************************/
#ifndef _TASKS_H_
#define _TASKS_H_


#ifndef EXT_TASKS
#define EXT_TASKS   extern
#endif

/********************************************************* 
**定义进程块常量部分
**********************************************************/
typedef struct
{
    ktask_t * handle;
    ThreadFunc pfunc;    //进程的启动函数
    cpu_stack_t * stack;
    uint8 prior;        //优先级
    uint8 tbl;          //消息表,如果为0则表示没有消息循环机制,否则有(yzy:在创建进程时用于判断是否建立消息列表结构体)
    uint32 stklen;      //栈的长度(yzy:貌似没用)
    const name_t * name;        //进程名称
    kbuf_queue_t * msg;
    void * arg;
}KTaskConst;

/********************************************************* 
**进程的声明,在系统进程中利用这个声明去创建所有的进程
**进程的声明固定在一个section里面
**********************************************************/
typedef struct{
    const KTaskConst * ktask;
    uint32 stklen;          //栈的长度,单位:字节. yzy 这个跟tcbc->stklen有何区别?貌似后者没有用到.
}KTaskDeclare;




EXT_TASKS  ktask_t* gs_TkHandles[SYS_TK_NUM];     //所有进程的TCB指针


/******************************************************************************
**main进程
******************************************************************************/
#define TASK_MAIN_TKID           1
#define TASK_MAIN_STKL           384//
const extern KTaskConst gs_TkMAIN;

#define TASK_DECLARE_1_MAIN() {&gs_TkMAIN, TASK_MAIN_STKL}

/******************************************************************************
**main进程
******************************************************************************/
#define TASK_RFMNG_TKID           10
#define TASK_RFMNG_STKL           384//
const extern KTaskConst gs_TkLP2PCmm;

#define TASK_DECLARE_10_RFMNG() {&gs_TkLP2PCmm, TASK_RFMNG_STKL}

/******************************************************************************
**无线modem进程
******************************************************************************/

#define DEV_MODEM_TKID           2
#define DEV_MODEM_STKL           1024//
const extern KTaskConst gs_TkDevModem;

#define TASK_DECLARE_2_MODEM() {&gs_TkDevModem, DEV_MODEM_STKL}

//	
//	/******************************************************************************
//	**PST被动通讯主进程
//	******************************************************************************/
//	#define TASK_PST_TKID           2
//	#define TASK_PST_STKL           512//512会溢出.FB升级时
//	extern ktask_t gs_TkPst;
//	#define TASK_DECLARE_2_PST() {&gs_TkPst, TASK_PST_STKL}
//	
//	
/******************************************************************************
**远程通道管理进程
******************************************************************************/
#define TASK_FARP_TKID          3
#define TASK_FARP_STKL          2048
const extern KTaskConst gs_TkFarp;
#define TASK_DECLARE_3_FARP() {&gs_TkFarp, TASK_FARP_STKL}
//	


/******************************************************************************
**netp进程
******************************************************************************/
#define TASK_NETP_TKID          4
#define TASK_NETP_STKL          1024
extern const KTaskConst gs_TkNetp;
#define TASK_DECLARE_4_NETP() {&gs_TkNetp, TASK_NETP_STKL}

#define TASK_NETP2_TKID          5
//	#define TASK_NETP2_STKL          512
extern const KTaskConst gs_TkNetp2;
#define TASK_DECLARE_5_NETP() {&gs_TkNetp2, TASK_NETP_STKL}

#define TASK_NETP3_TKID          6
//	#define TASK_NETP3_STKL          512
extern const KTaskConst gs_TkNetp3;
#define TASK_DECLARE_6_NETP() {&gs_TkNetp3, TASK_NETP_STKL}

#define TASK_NETP4_TKID          7
//	#define TASK_NETP4_STKL          512
extern const KTaskConst gs_TkNetp4;
#define TASK_DECLARE_7_NETP() {&gs_TkNetp4, TASK_NETP_STKL}



/******************************************************************************
**hlv进程
******************************************************************************/
#define TASK_HLV_TKID          2
#define TASK_HLV_STKL          1024
extern const KTaskConst gs_TkHLV;
#define TASK_DECLARE_2_HLV() {&gs_TkHLV, TASK_HLV_STKL}

#define TASK_SLV_TKID          9
#define TASK_SLV_STKL          192
extern const KTaskConst gs_TkSLV;
#define TASK_DECLARE_9_SLV() {&gs_TkSLV, TASK_SLV_STKL}

void SYS_TASKS_Init(void);


#endif


