/******************************Copyright(c)******************************
**                          
**
** File Name: hal_freq.c
** Author: 
** Date Last Update: 2015-05-18
** Description: 系统工作频率配置
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: 文件创建
*************************************************************************/
#define EXT_HAL_FREQ

#include "sys.h"
#include "hal.h"



/************************************************************************
 * @function: HAL_NopDelay
 * @描述: 低级延时函数
 * @参数: 
 * @返回: 
 * @说明: 每个单位延时10个nop
 * @作者: yzy (2014/6/27)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void HAL_NopDelay(uint32 cnt)
{
    for(uint32 ul_i = 0; ul_i < cnt; ul_i++)
    {
		__NOP();
		__NOP();
		__NOP();
		__NOP();
		__NOP();
        __NOP();
		__NOP();
		__NOP();
        __NOP();
		__NOP();
    }
}





/************************************************************************
 * @function: HAL_InitSystemFreq
 * @描述: 初始化系统频率
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/6/27)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/

//	void HAL_InitSystemFreq(void)
//	{
//	    /*从BOOT中调用 Chip_SetupIrcClocking()选择“输入源为IRC的PLL时钟”作系统时钟,
//	    此处要调用 Chip_SetupXtalClocking()切换为“输入源为XRC的PLL时钟“。
//	    切换前要先将系统时钟 先配成非PLL,否则会导致崩溃。
//	    */
//	    Chip_Clock_SetMainClockSource(SYSCTL_MAINCLKSRC_IRC);
//	    
//		//Enable IOCON clock
//		Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_IOCON);
//	    
//	    //先配置外部晶振管脚
//	    Chip_IOCON_PinMuxSet(LPC_IOCON, 2, 0, (IOCON_FUNC1 | IOCON_MODE_INACT));//XTALIN
//	    Chip_IOCON_PinMuxSet(LPC_IOCON, 2, 1, (IOCON_FUNC1 | IOCON_MODE_INACT));//XTALOUT
//	    
//	    //设置外部频率
//	    Chip_SetupXtalClocking();
//	    //Chip_SetupIrcClocking();
//	    //获取实际的频率值,判断与理论值是否一致.
//	    SystemCoreClockUpdate();
//	    if(SystemCoreClock != SYS_SYS_CLK)
//	    {
//	        NVIC_SystemReset();
//	    }
//	}





/************************************************************************
 * @function: HAL_InitSysTick
 * @描述: 初始化SysTick寄存器组
 * @参数: 
 * @返回: 
 * @说明: 这个函数不是进程安全的,确保同一时刻只在一个进程内使用
 * @作者: yzy (2014/6/27)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void HAL_InitSysTick(void)
{
     //******************************
    //1.设置SYSTICK中断优先级
    //2.设置SYSTICK计数器重载值
    //3.初始化SYSTICK计数器当前值为零
    //4.配置SYSTICK的时钟源\使能触发中断\使能定时器.(同时配置一下PENDSV优先级.)
    //******************************
//	//	
//	//	    RCC_ClkInitTypeDef    clkconfig;
//	//	    uint32_t              pFLatency;
//	//	
//	//	
//	//	
//	//	    /* Get clock configuration */
//	//	    HAL_RCC_GetClockConfig(&clkconfig, &pFLatency);

//	    RCC_GetClocksFreq(&RCC_Clocks);
#ifndef SYS_TICK_PER_SEC
    HAL_SYSTICK_Config(SystemCoreClock / 100);
#else
    HAL_SYSTICK_Config(HAL_RCC_GetSysClockFreq() / SYS_TICK_PER_SEC);
#endif
    
    HAL_NVIC_SetPriority(PendSV_IRQn,  PRI_PENDSV, 0);     //PendSV_IRQn
}


//	HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
//	{
//	  /*Configure the SysTick to have interrupt in 1ms time basis*/
//	  if (HAL_SYSTICK_Config(SystemCoreClock / (1000U / uwTickFreq)) > 0U)
//	  {
//	    return HAL_ERROR;
//	  }
//	
//	  /* Configure the SysTick IRQ priority */
//	  if (TickPriority < (1UL << __NVIC_PRIO_BITS))
//	  {
//	    HAL_NVIC_SetPriority(SysTick_IRQn, TickPriority, 0U);
//	    uwTickPrio = TickPriority;
//	  }
//	  else
//	  {
//	    return HAL_ERROR;
//	  }
//	
//	   /* Return function status */
//	  return HAL_OK;
//	}




