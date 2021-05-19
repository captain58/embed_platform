/****************************************Copyright (c)**************************************************
**                                  ________科技有限公司
**                                          开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: dev_lowpwr.c
**创   建   人: yzy
**最后修改日期: 2018年04月25日
**描        述: 低功耗模块
*********************************************************************************************************/

#define EXT_DEV_LOWPWR

#include "include.h"


/***********************************************************************/
static uint16_t sleepDelay;
static uint32_t hwBusy=0;

/**
 * 单片机进入休眠，关闭外设
 * 
 * @author Zzzzz (2018/4/1)
 * 
 * @param void 
 */
void SYS_LOWPWR_Sleep(void)
{
    if(hwBusy==0 && sleepDelay==0)
    {
        NOP();  /* Add one instruction delay */
        STOP(); /* Enter STOP mode */
        NOP();  /* Add one instruction delay */
    }
    
    if(sleepDelay>0)
        sleepDelay--;
}

uint32_t SYS_LOWPWR_Set_Busy(LOWPWRENUM module )
{
    hwBusy |= (uint32)1 << module;

    return hwBusy;
}

uint32_t SYS_LOWPWR_Clear_Busy(LOWPWRENUM module )
{
    hwBusy &= ~((uint32)1 << module);
    return hwBusy;
}

/**
 * 控制单片机不要休眠的时间
 * 
 * @author Zzzzz (2018/3/28)
 * 
 * @param tVal 单位tick
 */
void SYS_LOWPWR_Delay(uint16_t tVal)
{
    sleepDelay=tVal;
}


const _F_LOWPWR gs_DEVLowPwr = 
{
    SYS_LOWPWR_Sleep,
    SYS_LOWPWR_Set_Busy,
    SYS_LOWPWR_Clear_Busy,
    SYS_LOWPWR_Delay
};


