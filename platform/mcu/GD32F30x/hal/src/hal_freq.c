/******************************Copyright(c)******************************
**                          
**
** File Name: hal_freq.c
** Author: 
** Date Last Update: 2015-05-18
** Description: ϵͳ����Ƶ������
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: �ļ�����
*************************************************************************/
#define EXT_HAL_FREQ

#include "sys.h"
#include "hal.h"



/************************************************************************
 * @function: HAL_NopDelay
 * @����: �ͼ���ʱ����
 * @����: 
 * @����: 
 * @˵��: ÿ����λ��ʱ10��nop
 * @����: yzy (2014/6/27)
 *-----------------------------------------------------------------------
 * @�޸���: 
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
 * @����: ��ʼ��ϵͳƵ��
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/6/27)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/

//	void HAL_InitSystemFreq(void)
//	{
//	    /*��BOOT�е��� Chip_SetupIrcClocking()ѡ������ԴΪIRC��PLLʱ�ӡ���ϵͳʱ��,
//	    �˴�Ҫ���� Chip_SetupXtalClocking()�л�Ϊ������ԴΪXRC��PLLʱ�ӡ���
//	    �л�ǰҪ�Ƚ�ϵͳʱ�� ����ɷ�PLL,����ᵼ�±�����
//	    */
//	    Chip_Clock_SetMainClockSource(SYSCTL_MAINCLKSRC_IRC);
//	    
//		//Enable IOCON clock
//		Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_IOCON);
//	    
//	    //�������ⲿ����ܽ�
//	    Chip_IOCON_PinMuxSet(LPC_IOCON, 2, 0, (IOCON_FUNC1 | IOCON_MODE_INACT));//XTALIN
//	    Chip_IOCON_PinMuxSet(LPC_IOCON, 2, 1, (IOCON_FUNC1 | IOCON_MODE_INACT));//XTALOUT
//	    
//	    //�����ⲿƵ��
//	    Chip_SetupXtalClocking();
//	    //Chip_SetupIrcClocking();
//	    //��ȡʵ�ʵ�Ƶ��ֵ,�ж�������ֵ�Ƿ�һ��.
//	    SystemCoreClockUpdate();
//	    if(SystemCoreClock != SYS_SYS_CLK)
//	    {
//	        NVIC_SystemReset();
//	    }
//	}





/************************************************************************
 * @function: HAL_InitSysTick
 * @����: ��ʼ��SysTick�Ĵ�����
 * @����: 
 * @����: 
 * @˵��: ����������ǽ��̰�ȫ��,ȷ��ͬһʱ��ֻ��һ��������ʹ��
 * @����: yzy (2014/6/27)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void HAL_InitSysTick(void)
{
     //******************************
    //1.����SYSTICK�ж����ȼ�
    //2.����SYSTICK����������ֵ
    //3.��ʼ��SYSTICK��������ǰֵΪ��
    //4.����SYSTICK��ʱ��Դ\ʹ�ܴ����ж�\ʹ�ܶ�ʱ��.(ͬʱ����һ��PENDSV���ȼ�.)
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




