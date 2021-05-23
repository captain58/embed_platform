/******************************Copyright(c)******************************
**                                
**
** File Name: hal_hwtimer.h
** Author: 
** Date Last Update: 2015-05-18
** Description: 通用硬件定时器
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: 文件创建
*************************************************************************/
#ifndef _HAL_HWTIMER_H_
#define _HAL_HWTIMER_H_

#ifndef EXT_HAL_HWTIMER
#define EXT_HAL_HWTIMER    extern
#endif




/************************************************************************
 * @描述:timer函数原型
 ************************************************************************/
typedef void (*THwTimerFunc)(void*);



/************************************************************************
 * @Function: HAL_InitSysTimer
 * @Description: 功能初始化
 * @Arguments: 
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/18
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void HAL_InitSysTimer(void);



/************************************************************************
 * @Function: SysTimer_DlyConfig
 * @Description: 配置系统定时器
 * 
 * @Arguments: 
 * @param: func 表示当前路中断产生之后调用的函数
 * @param: fpara 中断处理函数的传入参数
 * @param: tmrIdx 表示第几路
 * @param: mills 表示这个延时是多少ms
 * 
 * @Attention: 应用模块应该在系统启动的时候就已经将这些寄存器配置好,这个函数非进程安全,非中断安全 
 * @Return: uint8  
 * @Auther: yzy
 * Date: 2015/5/18
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8 SysTimer_DlyConfig(void (*func)(void*), void* fpara, uint8 tmrIdx, uint16 mills);

/************************************************************************
 * @Function: SysTimer_DlyStart
 * @Description: 启动硬件定时器
 * 
 * @Arguments: 
 * @param: tmrIdx 表示第几路 
 * 
 * @Attention: 这个函数不是进程安全的
 * @Return: uint8  
 * @Auther: yzy
 * Date: 2015/5/18
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8 SysTimer_DlyStart(uint8 tmrIdx);

/************************************************************************
 * @Function: SysTimer_DlyStop
 * @Description: 停止硬件定时器
 * 
 * @Arguments: 
 * @param: tmrIdx 表示第几路 
 * 
 * @Note: 这个函数不是进程安全的
 * @Return: uint8  
 * @Auther: yzy
 * Date: 2015/5/18
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8 SysTimer_DlyStop(uint8 tmrIdx);




#if (true)   //级联硬件定时器功能

/************************************************************************
 * @定时处理函数原型
 ************************************************************************/
typedef void (*TCasHwTimerFunc)(void*);




/************************************************************************
 * @级联硬件定时器信息结构体
 ************************************************************************/
typedef struct _TCasHwTimer
{
    uint8 stoped;               //是否停止
    uint8 timerid;              //定时器id
    uint16 timeout;             //定时周期
    uint16 dlycnt;              //倒计时计数器
    TCasHwTimerFunc tickfunc;   //定时处理函数,函数返回false时停止timer
    void* tickpara;             //定时处理函数传入参数
    struct _TCasHwTimer* pnext;
}TCasHwTimer;


/************************************************************************
 * @Function: casHwTimerCreate
 * @Description: 创建串口专用定时器(目的为了减少硬件定时器的依赖数量)
 * 
 * @Arguments: 
 * @param: tickfunc 定时处理函数
 * @param: tickpara 传入参数
 * @param: timerid 定义的定时器号
 * @param: dlytick 定时的tick数
 * 
 * @Note: 
 * @Return: uint8  
 * @Auther: yzy
 * Date: 2015/6/3
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8 casHwTimerCreate(TCasHwTimerFunc tickfunc, void* tickpara, uint8 timerid, uint16 dlytick);


/************************************************************************
 * @Function: casHwTimerStart
 * @Description: 启动timer
 * 
 * @Arguments: 
 * @param: timerid 
 * 
 * @Note: 
 * @Return: TResult  
 * @Auther: yzy
 * Date: 2015/6/1
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8 casHwTimerStart(uint8 timerid);


/************************************************************************
 * @Function: casHwTimerStop
 * @Description: 停止timer
 * 
 * @Arguments: 
 * @param: timerid 
 * 
 * @Note: 
 * @Return: TResult  
 * @Auther: yzy
 * Date: 2015/6/1
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8 casHwTimerStop(uint8 timerid);


/************************************************************************
 * @Function: casHwTimerConfig
 * @Description: 配置
 * 
 * @Arguments: 
 * @param: sysTimerID 绑定的systimerID
 * @param: tickMills 单次定时中断的毫秒数
 * 
 * @Note: 
 * @Return: uint8  
 * @Auther: yzy
 * Date: 2015/6/3
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8 casHwTimerConfig(uint8 sysTimerID, uint16 tickMills);

uint8 casHwTimerClose(uint8 timerid);

#endif              //#if (true)   //级联硬件定时器功能


#endif

