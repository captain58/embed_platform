/******************************Copyright(c)******************************
**                          
**
** File Name: dev_wdt.c
** Author: 
** Date Last Update: 2019-11-02
** Description: 看门狗设备
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: 文件创建
*************************************************************************/
#define EXT_DEV_WDT

#include "sys.h"
#include "hal.h"
#include "bsp.h"







/************************************************************************
 * @Function: WDT_Server
 * @Description: WDT定时服务程序
 * 
 * @Arguments: 
 * @param: pdata 
 * 
 * @Note: 
 * @Return: bool  
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
bool WDT_Server(void* pdata)
{
    if(guc_TickDogFeedDIS != 0xaa)
    {
        if(guc_TickDogFeedCnt != 0)
        {
            guc_TickDogFeedCnt--;
            HAL_WDT_Feed();
        }
    }
    
    return true;
}





/************************************************************************
 * @Function: SYS_WDT_Init
 * @Description: 看门狗设备初始化
 * @Arguments: 
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void SYS_WDT_Init(void)
{
    uint16 timeout;

    //使能高级看门狗
    guc_TickDogFeedDIS = 0;
    
    //初始化喂狗值
    guc_TickDogFeedCnt = 100;
    
    //创建亮灯服务定时器
    timeout = SYS_TICK_PER_SEC / 10;
    timeout = (timeout < 1) ? 1 : timeout;
    SYS_Timer_Create(WDT_Server, __NULL, timeout, ID_SWTIMER_WDT, false);
}




/************************************************************************
 * @Function: SYS_WDT_Feed
 * @Description: 喂狗
 * 
 * @Arguments: 
 * @param: wdttick wdttick.狗的寿命(单位100ms)
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void SYS_WDT_Feed(uint8 wdttick)   
{
    guc_TickDogFeedCnt = wdttick;
}




/************************************************************************
 * @Function: SYS_WDT_DIS
 * @Description: 停止喂狗
 * @Arguments: 
 * @Note: 仅限测试使用.注意,有外部看门狗的时候禁用内部看门狗.否则测试时无法有效测试外部看门狗
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void SYS_WDT_DIS(void)
{
    guc_TickDogFeedDIS = 0xaa;
}

