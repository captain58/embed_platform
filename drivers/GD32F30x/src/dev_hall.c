/***********************************************************************************************************************
* 文件名：dev_hall.c
* 版  本：
* 描  述：霍尔计数流程应用源文件
* 开发者：
* 日  期：
***********************************************************************************************************************/

#define EXT_HALL

#include "sys.h"
#include "hal.h"
#include "bsp.h"
#include "iomap.h"
#include "logc_event.h"
#include "reportApi.h"
#include "measureApi.h"
#include "WOSsys.h"
#define HWTIMER_HALL_CHECK_ID  12

ST_HALL_COUNT    HallCount = {0, 0};                 //霍尔计量值   
HallCheck_ST    HallA;                              //HallA判断结构体 
HallCheck_ST    HallB;                              //HallB判断结构体 
static unsigned char HallState = HALL_ISNORMAL;     //霍尔状态
//static unsigned char Timer_HallCheck = 0;           //Hall磁攻击清除检测定时器编号


/*******************************************************************************
 * 获取霍尔A  IO的状态
 * @return
 ******************************************************************************/
/* unsigned char GpioHallARead(void) */
unsigned char PCB_HallARead(void)
{
    return HALLA_GETSTT();
}

/*******************************************************************************
 * 获取霍尔B  IO的状态
 * @return
 ******************************************************************************/
/* unsigned char GpioHallBRead(void)//完成 */
unsigned char PCB_HallBRead(void)
{
    return HALLB_GETSTT();
}
/*
 * 获取当前Hall状态
 */
unsigned char GetHallState(void)
{
    return HallState;
}
/*
 * 霍尔A判断
 */
void HallACheck(void)
{   
    HallA.Count++;
    
    if(HallA.Step ==0 ) //滤波(通讯干扰)
    {
        if(HallA.GetPortInput() == 1) //滤波期间检测到高电平，结束霍尔检测
        {
            HallA.Run   = 0;
        }
        else
        {
            if(HallA.Count >= HallA.TimeFilter) //滤波时间到，转磁攻击判断
            {
                HallA.Count = 0;
                HallA.Step  = 1;
            }
        }
    }
    else //Step == 1    磁攻击判断
    {
        if(HallB.GetPortInput() == 1)    //HallB为高电平，结束磁攻击检测，记有效霍尔信号
        {
            HallA.HallPulseFun();
            HallA.Run   = 0;
        }
        else        //HallB为低电平时持续检测是否有磁攻击
        {
            if(HallA.Count >= HallA.TimeHallAttack) //磁攻击判断时间到，记磁攻击有效
            {
                HallA.HallAttackFun();
                HallA.Run = 0;
            }   
        }
    }
}

/*
 * 霍尔B判断
 */
void HallBCheck(void)
{       
    HallB.Count++;
    
    if(HallB.Step ==0 ) //滤波(通讯干扰)
    {
        if(HallB.GetPortInput() == 1) //滤波期间检测到高电平，结束霍尔检测
        {
            HallB.Run   = 0;
        }
        else 
        {
            if(HallB.Count >= HallB.TimeFilter) //滤波时间到，转磁攻击判断
            {
                HallB.Count = 0;
                HallB.Step  = 1;
            }
        }
    }
    else //Step == 1    磁攻击判断
    {     
        if(HallA.GetPortInput() == 1)    //HallA为高电平，结束磁攻击检测，记有效霍尔信号
        {
            HallB.HallPulseFun();
            HallB.Run   = 0;
        }
        else        //HallA为低电平时持续检测是否有磁攻击
        {
            if(HallB.Count >= HallB.TimeHallAttack) //磁攻击判断时间到，记磁攻击有效
            {
                HallB.HallAttackFun();
                HallB.Run = 0;
            }   
        }
    }
}

/*
 * HALL定时检测
 */
void HallCheck(void * para)
{
    if(HallA.Run == 1)
    {
        HallACheck();
    }
    if(HallB.Run == 1)
    {
        HallBCheck();
    }
    if((HallA.Run == 0) && (HallB.Run == 0))  //没有霍尔检测在运行
    {
//        SYS_TimerStop(SYS_LPTIMER);
        casHwTimerStop(HWTIMER_HALL_CHECK_ID);
    } 
}

/**
 * 磁攻击回调函数
 */
void HallAttackCall(void)
{
    if(HallState == HALL_ISERR)         //已经在异常状态
    {
        return;
    }
    if(IsIotUpcomingBusy() != 1)     //上告期间不检测磁攻击，会有干扰
    {
        HallState = HALL_ISERR;
        APP_EventMutexClean(EVT_TYPE_COUNTPAY, APP_EVT_CNT_EMI);
        APP_EventWrite(EVT_TYPE_COUNTPAY, APP_EVT_CNT_EMI);
    }
}

/*
 * 磁攻击判断
 */
void HallAttackCheck(void)
{
    if((PCB_HallARead()) || (PCB_HallBRead()))            //未检测到磁攻击
    {   
        APP_EventMutexClean(EVT_TYPE_COUNTPAY, APP_EVT_CNT_EMI);
        APP_EventWrite(EVT_TYPE_COUNTPAY, APP_EVT_CNT_EMICANCEL);         //清磁攻击事件 
        HallState = HALL_ISNORMAL;
    }
    else
    {
        APP_EventMutexClean(EVT_TYPE_COUNTPAY, APP_EVT_CNT_EMI);
        APP_EventWrite(EVT_TYPE_COUNTPAY, APP_EVT_CNT_EMI);   //置磁攻击事件
        HallState = HALL_ISERR;
    }
}

/*
 * 磁攻击清楚检测，只负责清除检测
 */
void HallAttackClrCheck(void)
{
    if(HallState == HALL_ISERR)
    {
        if((PCB_HallARead()) || (PCB_HallBRead()))            //未检测到磁攻击
        {   
            APP_EventMutexClean(EVT_TYPE_COUNTPAY, APP_EVT_CNT_EMI);
            APP_EventWrite(EVT_TYPE_COUNTPAY, APP_EVT_CNT_EMICANCEL);         //清磁攻击事件 
            HallState = HALL_ISNORMAL;
        }
    }
}

/*
 * HallA有效计数回调函数
 */
void HallAPulse(void)
{
    HallCount.HallACount++;
}

/* 
 * HallA中断，启动消抖计时
 */
void HallAInt(void * para)
{ 
    HallA.Count = 0;
    HallA.Step  = 0;
    HallA.Run   = 1;
    casHwTimerStart(HWTIMER_HALL_CHECK_ID);

}

/*
 * HallB有效计数回调函数
 */
void HallBPulse(void)
{
    u8 ucdata;
    
    if(HallCount.HallACount > 0)
    {
        HallCount.HallACount = 0;
        HallCount.HallBCount = 0;
        ucdata = 0x5b;
        SysOptDat(0x0cffffe0,BIT7,0,1,&ucdata);//读取脉冲电量增量
    }
    else
    {
        HallCount.HallBCount++;
    }   
}

/* 
 * HallB中断，启动消抖计时
 */
void HallBInt(void * para)
{
    HallB.Count = 0;
    HallB.Step  = 0;
    HallB.Run   = 1;
//    SYS_TimerStart(SYS_LPTIMER);
    casHwTimerStart(HWTIMER_HALL_CHECK_ID);

}
/*
 *  霍尔初始化
 */
void InitHall(void)
{     
//	    PCB_HALLA_DIRCFGIN;
//	    PCB_HALLB_DIRCFGIN;
//	    PCB_HallASetInt(1, HallAInt);
//	    PCB_HallBSetInt(1, HallBInt);
    
    HAL_GPIO_PinConfig(HALLA_PORT, HALLA_PIN, IN_PULLUP, ANALOG_DISENABLE, GPIO_DIR_IN);
	HAL_GPIO_EInt_Cfg(EXT_INT_CHANNL_HALLA, HALLA_PORT, HALLA_PIN, GPIO_FALLING, HallAInt, NULL);
	HAL_GPIO_PinConfig(HALLB_PORT, HALLB_PIN, IN_PULLUP, ANALOG_DISENABLE, GPIO_DIR_IN);
	HAL_GPIO_EInt_Cfg(EXT_INT_CHANNL_HALLB, HALLB_PORT, HALLB_PIN, GPIO_FALLING, HallBInt, NULL);

    
    HallA.Run   = 0;
    HallA.Step  = 0;
    HallA.Count = 0;
    HallA.TimeFilter        = 20;        //20*10 = 200ms
    HallA.TimeHallAttack    = 20;        //20*10 = 200ms   
    HallA.GetPortInput      = PCB_HallARead;
    HallA.HallPulseFun      = HallAPulse;
    HallA.HallAttackFun     = HallAttackCall;   //磁攻击回调函数
    
    HallB.Run   = 0;
    HallB.Step  = 0;
    HallB.Count = 0;
    HallB.TimeFilter        = 20;        //20*10 = 200ms
    HallB.TimeHallAttack    = 20;        //20*10 = 200ms   
    HallB.GetPortInput      = PCB_HallBRead;
    HallB.HallPulseFun      = HallBPulse;
    HallB.HallAttackFun     = HallAttackCall;   //磁攻击回调函数
    
//	    SYS_TimerCfg(SYS_LPTIMER, 0, HallCheck);    //配置Hall检测专用定时器
    
    casHwTimerCreate(HallCheck, NULL, HWTIMER_HALL_CHECK_ID, 10);
    HallAttackCheck();                          //初始化时先检测下霍尔状态
    //Timer_HallCheck = SYS_AskTimeNumber();    //申请霍尔磁攻击清除检测定时器编号
    s_S_MSK_INT_TYPE sTmType;
    sTmType.ucType = SYS_TM_1s;
    sTmType.ucPri = 0;  ///设置计量模块500ms的定时的优先级
    SMskSetTmFunc(sTmType,&HallAttackClrCheck);
//	    SYS_StartTime(SYS_SUBTASK, ltid_HallNmlChk, 1, HallAttackClrCheck);  //每1s调用一次
}



