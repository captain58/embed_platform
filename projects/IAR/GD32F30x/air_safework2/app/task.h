/****************************************Copyright (c)**************************************************
**                                  ____科技有限公司
**                                          开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: GD_task.h
**创   建   人: yzy
**最后修改日期: 2009-06-01
**描        述: 任务上报模块程序头文件
**注        意:
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2009-06-01
** 描　述: 原始版本
********************************************************************************************************/
#ifndef _GD_TASK_H_
#define _GD_TASK_H_

#ifndef EXT_GD_TASK
#define EXT_GD_TASK extern
#endif


/************************************************************************ 
**主动上报帧主站确认回复标志位图(其实只要2字节即可.因为SEQ中只有4位表示pseq.//bitx:1收到确认,0未收到确认)
*************************************************************************/
EXT_GD_TASK uint8 bmap_AckRecv[32];
#define TASK_PARA_LEN 6
/************************************************************************ 
**任务参数结构体(暂不做进程保护)
*************************************************************************/
EXT_GD_TASK uint8 gucs_TaskParaBuffer[TASK_PARA_LEN];


/************************************************************************ 
**任务时间结构体
*************************************************************************/
typedef struct
{
    uint8 en;           //使能标志
    uint8 atr;          //任务属性(bit7为执行标志,bit0-bit5为任务编号(0-63))
    uint16 itv;         //发送周期,min,最大45天.
    uint32 jz;          //发送基准,min

}S_TaskTimer;

typedef struct
{
    uint8 flag;           //485抄表完成标志
    uint8 runflag;           //485抄表线程运行标志
    uint8 atr;          //任务属性(bit7为执行标志,bit0-bit5为任务编号(0-63))
    S_TimeLabel time;
}S_TaskUp;


EXT_GD_TASK S_TaskUp gss_TaskATimer;
//EXT_GD_TASK S_TaskTimer gss_TaskBTimer[TASK_EN_NUM];


//EXT_GD_TASK S_TaskUp gss_TaskPostpay;
//EXT_GD_TASK S_TaskUp gss_TaskPrepay;
//EXT_GD_TASK S_TaskTimer gss_TaskWarn;



#define TKT_ATR_FLAG    0x80
#define TKT_LPATR_FLAG    0x40

#define TKT_ATR_BHMK    0x3F



/************************************************************************ 
**需确认上报帧的发送处理结构体
*************************************************************************/
typedef struct
{
    uint8 en;                           //上报帧需确认功能开关.为0时其他参数无效
    uint8 pseq;                         //发送序号
    uint8 waitsec;                      //等待时间,秒
    uint8 trycnt;                       //重试次数
}REP_CFG;




/************************************************************************
 * @function: _JudgeTaskFlag
 * @描述: 置执行标志位.依赖分消息.必须放在实时进程中
 * 
 * @参数: 
 * @param:  
 * 
 * @返回: 
 * @return:  
 * @说明: 
 * @作者: yzy (2011-5-13)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void _JudgeTaskFlag(void);

/************************************************************************
**函数名称: Task_ExecuteA()
**描 述: 1类任务处理
**
**输 入: frame-帧解析结构体.(用于模仿下行帧数据)
**          frame.recv //任务参数
**          frame.send //发送缓冲区
**          frame.tbd\tstno(除CON位)\cmd\atr已填写,详见:Task_Proc()
**       con 是否须主站确认
**输 出: 
**
**备 注: 1.extra.info[7]为冻结密度 
**       2.多进程调用,既用于被动查询,又用于主动上报
**-----------------------------------------------------------------------
**作 者: yzy
**日 期: 09-10-15
*************************************************************************/
void Task_Execute(PST_Frame* frame,  uint8* st_bk, uint8* nt_bk, bool con);


/************************************************************************
 * @function: Task_Proc
 * @描述: 普通任务处理函数
 * @参数: 
 * 
 * @返回: 
 * @return: bool  true 任务全部上报完成. false 未完成
 * @说明: 初入公司时写的代码问题很多,趁内蒙测试时重写了整套机制
 * @作者: yzy (2011-5-16)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
bool Task_Proc(void);

/************************************************************************
**函数名称: void Task_PreInit(void* pdata)
**描 述: 任务进程初始化
**
**输 入: 
**输 出: 
**
**备 注: 
**-----------------------------------------------------------------------
**作 者: yzy
**日 期: 09-09-09
*************************************************************************/
void Task_PreInit(void);

/************************************************************************
**函数名称: void Task_SecProc(void* pdata)
**描 述: 秒处理
**
**输 入: 
**输 出: 
**
**备 注: 
**-----------------------------------------------------------------------
**作 者: yzy
**日 期: 09-09-09
*************************************************************************/
void Task_SecProc(void);


/************************************************************************
**函数名称: void Task_MinProc(void* pdata)
**描 述: 分处理
**
**输 入: 
**输 出: 
**
**备 注: 
**-----------------------------------------------------------------------
**作 者: yzy
**日 期: 09-09-09
*************************************************************************/
void Task_MinProc(void);
void Task_HourProc(void);

/************************************************************************
 * @function: _JudgeTaskFlag
 * @描述: 置执行标志位.依赖分消息.必须放在实时性进程中
 * 
 * @参数: 
 * @param:  
 * 
 * @返回: 
 * @return:  
 * @说明: 
 * @作者: yzy (2011-5-13)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void JudgeTaskFlag(uint8_t port);
void SetTaskRunFlag(uint8_t port);

/************************************************************************
**函数名称: uint8 REP_ChnSend(uint8* buffer, uint16 length, CHN_ENUM chninfo, REP_CFG* cfg)
**描 述: 发送需确认上报帧.
**
**输 入: 
**输 出: SYS_ERR_OK / others
**
**备 注: 
**-----------------------------------------------------------------------
**作 者: yzy
**日 期: 09-09-09
*************************************************************************/
uint8 REP_ChnSend(uint8* buffer, uint16 length, uint8_t chninfo, REP_CFG* cfg);
void Task_Clear();

#endif


