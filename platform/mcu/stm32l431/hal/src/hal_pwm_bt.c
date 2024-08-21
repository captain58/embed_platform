/******************************Copyright(c)******************************
**                          
**
** File Name: hal_hwtimer.c
** Author: 
** Date Last Update: 2015-05-18
** Description: pwm定时器
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: 文件创建
** 1)etime实现pwm输出
*************************************************************************/
#define EXT_HAL_PWM

#include "sys.h"
#include "hal.h"
#include <stdlib.h>
uint32_t g_nstandard = 0;
int32_t g_nCurPwm = 0;
#define CON_PWM_STEP_MAX() (SystemCoreClock/1024 - 1)

static TIM_HandleTypeDef htim3;


void HAL_Init_PWM(uint32_t para)//BTIMER的PWM输出示例程序
{
    g_nCurPwm = 0;

    /* USER CODE BEGIN TIM1_Init 0 */
    
    /* USER CODE END TIM1_Init 0 */
    
//    TIM_MasterConfigTypeDef sMasterConfig = {0};
//    TIM_OC_InitTypeDef sConfigOC = {0};
//    TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};
//    
//    /* USER CODE BEGIN TIM1_Init 1 */
//    
//    /* USER CODE END TIM1_Init 1 */
//    htim1.Instance = TIM1;
//    htim1.Init.Prescaler = 0;
//    htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
//    htim1.Init.Period = 0;
//    htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
//    htim1.Init.RepetitionCounter = 0;
//    htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
//    if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
//    {
//        Error_Handler();
//    }
//    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
//    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
//    if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
//    {
//      Error_Handler();
//    }
//    sConfigOC.OCMode = TIM_OCMODE_PWM1;
//    sConfigOC.Pulse = 0;
//    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
//    sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
//    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
//    sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
//    sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
//    if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
//    {
//      Error_Handler();
//    }
//    sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
//    sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
//    sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
//    sBreakDeadTimeConfig.DeadTime = 0;
//    sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
//    sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
//    sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
//    if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
//    {
//      Error_Handler();
//    }
    /* USER CODE BEGIN TIM1_Init 2 */
    
    /* USER CODE END TIM1_Init 2 */
//	    HAL_TIM_MspPostInit(&htim1);

    __HAL_RCC_TIM3_CLK_ENABLE();
    /* USER CODE BEGIN TIM3_Init 1 */
    
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};
    TIM_OC_InitTypeDef sConfigOC = {0};
    
    /* USER CODE BEGIN TIM3_Init 1 */
    
    /* USER CODE END TIM3_Init 1 */
    htim3.Instance = TIM3;
    htim3.Init.Prescaler = 0;
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim3.Init.Period = CON_PWM_STEP_MAX();//SystemCoreClock/4096 - 1;
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
    {
      Error_Handler();
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
    {
      Error_Handler();
    }
    if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
    {
      Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
    {
      Error_Handler();
    }
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = CON_PWM_STEP_MAX()/2;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_ENABLE;
    if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
    {
      Error_Handler();
    }


    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
    /* USER CODE BEGIN TIM3_Init 2 */
    
    /* USER CODE END TIM3_Init 2 */
//    HAL_TIM_MspPostInit(&htim3);


}

#define CON_PWM_OFF     0
#define CON_PWM_ON      1
#define CON_PWM_CENTRE  2

//extern void RCHF_Adj_Proc(ETIMx_Type* ETIMx, uint8_t ClkMode );
uint8_t HAL_Adj_PWM(uint32_t ledlm, int32_t adj, float fix)
{
    //RCHF温度调校函数，RCHF=8MHZ全温区+/-2%，RCHF8MHZ以上需要进行温度调校。
    //调校的前提是有比较精准的外部32K晶体
    //调校按实际需求可以采用定时（时间用户自己确定）调校;也可以采用ADC查询温度，温差大于一定值再进行调校
    //任何一路ETIM都可以 

    int64_t val = (int64_t)((float)adj*CON_PWM_STEP_MAX()*1000);
//	    val/=(int32_t)(ledlm*1000);

    int32_t radj = val /(int32_t)(ledlm*1000);
    if(adj > 0)
    {
//	        radj= (int32_t)(CON_PWM_STEP_MAX * adj * 1000/ (int32_t)ledlm) / 1000;
        if(radj == 0)
        {
            radj = 1;
        }
        else if(radj > (g_nCurPwm/2))//超过当前pwm 50%
        {
            radj = (radj + 1)/2;
        }
    }
    else
    {
//	        radj= (int32_t)(CON_PWM_STEP_MAX * adj * 1000/ (int32_t)ledlm) / 1000;
        if(radj == 0)
        {
            radj = -1;
        }
        else if(abs(radj) >= g_nCurPwm)
        {
            radj = -(g_nCurPwm + 1)/2;
        }

    }
    val = g_nCurPwm + radj;

    if(val > CON_PWM_STEP_MAX())
    {
        val = CON_PWM_STEP_MAX();
    }
    else if(val < 0)
    {
        val = 0;
    }
        

//	    if(abs(1000 * adj / ledlm) > 1000 - g_nCurPwm )
//	    = 1000 - g_nCurPwm - (int32_t)(1000 * adj / ledlm);
    if(val >= 0 && val <= CON_PWM_STEP_MAX() && val != g_nCurPwm)
    {
        g_nCurPwm = val;
        TIM_OC_InitTypeDef sConfigOC = {0};
        sConfigOC.OCMode = TIM_OCMODE_PWM1;
        sConfigOC.Pulse = CON_PWM_STEP_MAX() - g_nCurPwm;
        sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
        sConfigOC.OCFastMode = TIM_OCFAST_ENABLE;
        int i = 3;
        while(i-->0)
        {
            if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) == HAL_OK)
            {
                break;
            }
        }

//HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
        HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
//	        ETIMx_ETxCR_CEN_Setable(ETIM1, DISABLE);    //启动定时器   
//	        ETIMx_ETxIVR_Write(ETIM1, 0xFFFF - 1000); /* ETx初值寄存器 */
//	        
//	        ETIMx_ETxCMP_Write(ETIM1, 0xffff - (1000 - (uint16_t)val));         /* ETx比较寄存器 */
//	        ETIMx_ETxCR_CEN_Setable(ETIM1, ENABLE); //启动定时器   
        
//	        BTIMx_BTCR1_PWM_Setable(BTIM2, DISABLE);  //PWM使能，不需要配置LLEN个LHEN
//	            
//	        BTIMx_BTLOADH_Write(BTIM2, 0);//高位加载值
//	        BTIMx_BTLOADL_Write(BTIM2, val % 0x100);//低位加载值
//	            
//	        BTIMx_BTCR1_PWM_Setable(BTIM2, ENABLE);  //PWM使能，不需要配置LLEN个LHEN
//	        BTIMx_BTCR1_CHEN_Setable(BTIM2, ENABLE);//使能计数器
//	        BTIMx_BTCR1_CLEN_Setable(BTIM2, ENABLE);//使能计数器

        
    }
    //RCHF_Adj_Proc(ETIM4, clkmode);    
    //HAL_PWM_RCHF_ADJ();
    return 0;
}

uint8_t HAL_PWM_Test(uint32_t init, uint32_t cmp)
{

//	    ETIMx_ETxCR_CEN_Setable(ETIM1, DISABLE);    //启动定时器   
//	    ETIMx_ETxIVR_Write(ETIM1, 0xFFFF - init); /* ETx初值寄存器 */
//	    
//	    ETIMx_ETxCMP_Write(ETIM1, 0xffff - cmp);         /* ETx比较寄存器 */
//	    ETIMx_ETxCR_CEN_Setable(ETIM1, ENABLE); //启动定时器   
//	    RCHF_Adj_Proc(ETIM4, clkmode);  
//	    BTIMx_BTCR1_PWM_Setable(BTIM2, DISABLE);  //PWM使能，不需要配置LLEN个LHEN
//	        
//	    BTIMx_BTLOADH_Write(BTIM2, 0);//高位加载值
//	    BTIMx_BTLOADL_Write(BTIM2, init % 0x100);//低位加载值
//	        
//	    BTIMx_BTCR1_PWM_Setable(BTIM2, ENABLE);  //PWM使能，不需要配置LLEN个LHEN
//	    BTIMx_BTCR1_CHEN_Setable(BTIM2, ENABLE);//使能计数器
//	    BTIMx_BTCR1_CLEN_Setable(BTIM2, ENABLE);//使能计数器

    return 0;
}


uint8_t HAL_PWM_OnOff(uint8_t onoff)
{
    if(onoff)
        HAL_PWM_Test(CON_PWM_STEP_MAX(), 0);
    else
        HAL_PWM_Test(0, 0);
    
    return 0;
}


uint8_t HAL_PWM_STS(void)
{
//    switch(g_nCurPwm)
//    {
//    case 0:
//        return CON_PWM_OFF;
//    case CON_PWM_STEP_MAX():
//        return CON_PWM_ON;
//    default:
//        return CON_PWM_CENTRE;
//    }
    if(0 == g_nCurPwm)
    {
        return CON_PWM_OFF;
    }
    else if(CON_PWM_STEP_MAX() == g_nCurPwm)
    {
        return CON_PWM_ON;
    }
    else
    {
        return CON_PWM_CENTRE;
    }
      
}

int32_t HAL_PWM_GetLm(void)
{
    return g_nCurPwm;
}


void HAL_PWM_RCHF_ADJ(void)
{
//	    SYS_ENTER_SCRT();
//	    RCHF_Adj_Proc(ETIM4, clkmode);    
//	    SYS_EXIT_SCRT();
}
