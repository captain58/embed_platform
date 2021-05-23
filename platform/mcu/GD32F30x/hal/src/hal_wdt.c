/******************************Copyright(c)******************************
**                         
**
** File Name: hal_wdt.c
** Author: 
** Date Last Update: 2019-11-02
** Description: 看门狗驱动
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: 文件创建
*************************************************************************/
#define EXT_HAL_WDT

#include "sys.h"
#include "hal.h"




#if SYS_EXTWDT_EN > 0
/************************************************************************
*硬件看门狗喂狗状态
************************************************************************/
uint8 gb_WDTStt;



/************************************************************************
*硬件看门狗端口全局结构体.
************************************************************************/
const COMPORT* gsp_halExtWdtPorts;



/************************************************************************
*硬件看门狗操作宏
************************************************************************/
void EXT_WDT_INIT(const COMPORT* cp)
{                                       //配置模式
    HAL_GPIO_PinConfig(cp->port, cp->pin, cp->mode);
    HAL_GPIO_SetPinDIR(cp->port, cp->pin, true);
                                        //全局变量
    gsp_halExtWdtPorts = cp;
}


void EXT_WDT_1(void)
{
    HAL_GPIO_SetPinState(gsp_halExtWdtPorts->port, gsp_halExtWdtPorts->pin, true);
}


void EXT_WDT_0(void)  
{
    HAL_GPIO_SetPinState(gsp_halExtWdtPorts->port, gsp_halExtWdtPorts->pin, false);
}

#endif

/* Private variables ---------------------------------------------------------*/
IWDG_HandleTypeDef hiwdg;




/**
  * @brief IWDG Initialization Function 初始化看门狗
  * @param None
  * @retval None
  */
void HAL_InitWDT(void)
{

  /* USER CODE BEGIN IWDG_Init 0 */

  /* USER CODE END IWDG_Init 0 */

  /* USER CODE BEGIN IWDG_Init 1 */

  /* USER CODE END IWDG_Init 1 */
  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_4;
  hiwdg.Init.Window = 4095;
  hiwdg.Init.Reload = 4095;
  if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
  {
//    Error_Handler();
  }
  /* USER CODE BEGIN IWDG_Init 2 */

  /* USER CODE END IWDG_Init 2 */
        //配置外部看门狗
#if SYS_EXTWDT_EN > 0
    EXT_WDT_INIT(&gs_ExtWdtPort);
#endif

}

 /************************************************************************
 * @Function: HAL_WDT_Feed
 * @Description: 喂狗
 * @Arguments: 
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void HAL_WDT_Feed(void)
{
    //喂内部看门狗
#ifndef __DEBUG   
    HAL_IWDG_Refresh(&hiwdg);
    
    //喂外部看门狗
#if SYS_EXTWDT_EN > 0
    if(gb_WDTStt)
    {
        EXT_WDT_1();
    }
    else
    {
        EXT_WDT_0();
    }
    gb_WDTStt = !gb_WDTStt;
#endif
#endif    
}





/************************************************************************
 * @Function: HAL_WDT_Open
 * @Description: 打开片内看门狗
 * 
 * @Arguments: 
 * @param: time 喂狗周期(秒)
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
//	void HAL_WDT_Open(uint8 time)
//	{
//	    /*
//	    wdt_osc_clk = Fclkana/(2 * (1 + DIVSEL)) = 9.4 kHz to 2.3 MHz (nominal values).
//	    其中,Fclkana为一系列枚举频率.
//	    */
//	    //WDT oscillator freq = 0.6MHz divided by 64 = 9.375khz
//		Chip_Clock_SetWDTOSC(WDTLFO_OSC_0_60, 64);
//	    
//	    //Use WDTOSC as the WDT clock
//	    Chip_WWDT_SelClockSource(LPC_WWDT, WWDT_CLKSRC_WATCHDOG_WDOSC);
//	    
//	    //设置超时时间
//	    Chip_WWDT_SetTimeOut(LPC_WWDT, 9375 * time);
//	    
//	    //使能看门狗溢出复位
//		Chip_WWDT_SetOption(LPC_WWDT, WWDT_WDMOD_WDRESET);
//		
//		//清除状态
//		Chip_WWDT_ClearStatusFlag(LPC_WWDT, WWDT_WDMOD_WDTOF | WWDT_WDMOD_WDINT);
//	
//		//开启看门狗
//		Chip_WWDT_Start(LPC_WWDT);
//	}





/************************************************************************
 * @Function: HAL_WDT_Reset
 * @Description: 禁止喂狗使系统复位
 * @Arguments: 
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void HAL_WDT_Reset(void)
{
    SYS_ENTER_AllSCRT();

//	    HAL_WDT_Open(1);
    while(1)
    {
        //loop for ever.
    }
}
