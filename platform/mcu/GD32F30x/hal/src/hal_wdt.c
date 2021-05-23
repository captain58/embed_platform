/******************************Copyright(c)******************************
**                         
**
** File Name: hal_wdt.c
** Author: 
** Date Last Update: 2019-11-02
** Description: ���Ź�����
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: �ļ�����
*************************************************************************/
#define EXT_HAL_WDT

#include "sys.h"
#include "hal.h"




#if SYS_EXTWDT_EN > 0
/************************************************************************
*Ӳ�����Ź�ι��״̬
************************************************************************/
uint8 gb_WDTStt;



/************************************************************************
*Ӳ�����Ź��˿�ȫ�ֽṹ��.
************************************************************************/
const COMPORT* gsp_halExtWdtPorts;



/************************************************************************
*Ӳ�����Ź�������
************************************************************************/
void EXT_WDT_INIT(const COMPORT* cp)
{                                       //����ģʽ
    HAL_GPIO_PinConfig(cp->port, cp->pin, cp->mode);
    HAL_GPIO_SetPinDIR(cp->port, cp->pin, true);
                                        //ȫ�ֱ���
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
  * @brief IWDG Initialization Function ��ʼ�����Ź�
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
        //�����ⲿ���Ź�
#if SYS_EXTWDT_EN > 0
    EXT_WDT_INIT(&gs_ExtWdtPort);
#endif

}

 /************************************************************************
 * @Function: HAL_WDT_Feed
 * @Description: ι��
 * @Arguments: 
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void HAL_WDT_Feed(void)
{
    //ι�ڲ����Ź�
#ifndef __DEBUG   
    HAL_IWDG_Refresh(&hiwdg);
    
    //ι�ⲿ���Ź�
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
 * @Description: ��Ƭ�ڿ��Ź�
 * 
 * @Arguments: 
 * @param: time ι������(��)
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
//	    ����,FclkanaΪһϵ��ö��Ƶ��.
//	    */
//	    //WDT oscillator freq = 0.6MHz divided by 64 = 9.375khz
//		Chip_Clock_SetWDTOSC(WDTLFO_OSC_0_60, 64);
//	    
//	    //Use WDTOSC as the WDT clock
//	    Chip_WWDT_SelClockSource(LPC_WWDT, WWDT_CLKSRC_WATCHDOG_WDOSC);
//	    
//	    //���ó�ʱʱ��
//	    Chip_WWDT_SetTimeOut(LPC_WWDT, 9375 * time);
//	    
//	    //ʹ�ܿ��Ź������λ
//		Chip_WWDT_SetOption(LPC_WWDT, WWDT_WDMOD_WDRESET);
//		
//		//���״̬
//		Chip_WWDT_ClearStatusFlag(LPC_WWDT, WWDT_WDMOD_WDTOF | WWDT_WDMOD_WDINT);
//	
//		//�������Ź�
//		Chip_WWDT_Start(LPC_WWDT);
//	}





/************************************************************************
 * @Function: HAL_WDT_Reset
 * @Description: ��ֹι��ʹϵͳ��λ
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
