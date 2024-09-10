/******************************Copyright(c)******************************
**                          
**
** File Name: hal_hwtimer.c
** Author: 
** Date Last Update: 2015-05-18
** Description: 通用硬件定时器
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: 文件创建
** 1)采用TIMER32_0实现4路硬件定时器
*************************************************************************/
#define EXT_HAL_HWTIMER

#include "sys.h"
#include "hal.h"



/************************************************************************
 *定时clock数
 ************************************************************************/
#define CLK_HAL_HWTIMER                (SystemCoreClock)


/************************************************************************
 *表示4路比较器延时到了之后应该调用的函数
 ************************************************************************/
THwTimerFunc gfs_hwTimerDelay[4];
void* paraHwTimerDelay[4];


/************************************************************************
 *表示4路比较器延时时间,单位ms
 ************************************************************************/
static uint32 gul_HwTimerDelay[4];          //总的定时时钟数



/************************************************************************
 *默认的空处理函数
 ************************************************************************/
void _sysTimerDoNothing(void *a)
{
    //do nothing
}

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;

/************************************************************************
 *代码从17xx拷贝,这里兼容性
 ************************************************************************/
#define LPC_HWTIMER     LPC_TIMER32_0
#define HWTIMER_IRQn    TIMER_32_0_IRQn

uint32_t g_timer_tick = 0;
uint32_t g_timer1_tick = 0;
uint32_t g_timer2_tick = 0;
/**
  * @brief  Period elapsed callback in non-blocking mode
  * @param  htim TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    
  /* Prevent unused argument(s) compilation warning */
//  UNUSED(htim);
    if (htim->Instance == TIM1)
    {
        g_timer1_tick++;
        gfs_hwTimerDelay[0](paraHwTimerDelay[0]);
    }
    else if (htim->Instance == TIM2)
    {
        g_timer2_tick++;
        gfs_hwTimerDelay[1](paraHwTimerDelay[1]);
    }

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIM_PeriodElapsedCallback could be implemented in the user file
   */
}

//#ifndef GD32F30X_HD
//    void TIMER0_TRG_CMT_TIMER10_IRQHandler(void)
//#else
//    void TIMER0_TRG_CMT_IRQHandler(void)
//#endif
//{
//    /* clear TIMER interrupt flag */
//    timer_interrupt_flag_clear(TIMER0,TIMER_INT_FLAG_CMT);
//
//    krhino_intrpt_enter();
//    gfs_hwTimerDelay[0](paraHwTimerDelay[0]);
//    krhino_intrpt_exit();
//    
//
//}


/**
  * @brief This function handles TIM1 update interrupt and TIM16 global interrupt.
  */
void TIM1_UP_TIM16_IRQHandler(void)
{
  krhino_intrpt_enter();
  /* USER CODE BEGIN TIM1_UP_TIM16_IRQn 0 */

  /* USER CODE END TIM1_UP_TIM16_IRQn 0 */
  HAL_TIM_IRQHandler(&htim1);
  /* USER CODE BEGIN TIM1_UP_TIM16_IRQn 1 */

  /* USER CODE END TIM1_UP_TIM16_IRQn 1 */
  krhino_intrpt_exit();
}

/**
  * @brief This function handles TIM2 global interrupt.
  */
void TIM2_IRQHandler(void)
{
  krhino_intrpt_enter();
  /* USER CODE BEGIN TIM2_IRQn 0 */

  /* USER CODE END TIM2_IRQn 0 */
  HAL_TIM_IRQHandler(&htim2);
  /* USER CODE BEGIN TIM2_IRQn 1 */

  /* USER CODE END TIM2_IRQn 1 */
  krhino_intrpt_exit();
}


//void TIMER0_Channel_IRQHandler(void)
//{
//	/* clear TIMER interrupt flag */
//    timer_interrupt_flag_clear(TIMER0,TIMER_INT_FLAG_CH0);
//    g_timer_tick++;
//    krhino_intrpt_enter();
//    gfs_hwTimerDelay[0](paraHwTimerDelay[0]);
//    krhino_intrpt_exit();
//    
//}
//
//void TIMER1_IRQHandler(void)
//
//{
//    /* clear TIMER interrupt flag */
//    timer_interrupt_flag_clear(TIMER1,TIMER_INT_FLAG_CH0);
//
//    krhino_intrpt_enter();
//    gfs_hwTimerDelay[1](paraHwTimerDelay[1]);
//    krhino_intrpt_exit();
//    
//
//}
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
void HAL_InitSysTimer(void)
{
//	    Chip_TIMER_Init(LPC_HWTIMER);            //打开timer3功率模块
//	    
//	    //为掌控细节,下面不使用nxp库
//	    LPC_HWTIMER->CTCR = 0;                   //选择计数控制为定时器模式
//	    LPC_HWTIMER->PR = 0;                     //选择每个PCLK周期+1         //Chip_TIMER_PrescaleSet(0);
//	    LPC_HWTIMER->MCR = 0;                    //禁能4路匹配通道
//	    LPC_HWTIMER->TCR = TIMER_RESET;          //复位并使能计数器           //Chip_TIMER_Reset(LPC_HWTIMER)
//	    LPC_HWTIMER->IR = 0xFF;                  //复位所有的T3中断
//	    LPC_HWTIMER->TCR = TIMER_ENABLE;         //开始计数                  //Chip_TIMER_Enable(LPC_HWTIMER)
    
    //初始化回调函数寄存器组
    for(uint8 uc_i = 0; uc_i < 4; uc_i++)
    {
        gfs_hwTimerDelay[uc_i] = _sysTimerDoNothing;
        paraHwTimerDelay[uc_i] = __NULL;
        gul_HwTimerDelay[uc_i] = CLK_HAL_HWTIMER;
        //SysTimer_DlyStop(uc_i);
    }
}

//extern void Error_Handler(void);



static uint8 Hal_Timer_Config(uint8 tmrIdx)
{
//	    ETIM_InitTypeDef init_para;
//	    ETIMx_Type* ETIMx;
//      volatile uint08 EtimNum;
//  
//      EtimNum = ((uint32_t)ETIMx - ETIMER1_BASE)>>5;
//	timer_oc_parameter_struct timer_ocintpara;
//	timer_parameter_struct timer_initpara;
//	timer_break_parameter_struct timer_breakpara;
//    timer_ic_parameter_struct timer_icinitpara;

    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_SlaveConfigTypeDef sSlaveConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};
    TIM_OC_InitTypeDef sConfigOC = {0};
    TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};
  
    uint32 cycle;
    SYS_VAR_CHECK(tmrIdx >= 3);           //判断参数的正确性
    cycle = gul_HwTimerDelay[tmrIdx];    //获取延时间隔
    SYS_VAR_CHECK(cycle > 1000);           //判断参数的正确性

    //信号源参数
//	    init_para.sig_src_para.SIG1SEL = ETIMx_ETxINSEL_SIG1SEL_GROUP1;     /* 内部信号1源选择GROUP1 */
    
    switch(tmrIdx)
    {
        case 0:
            {
              
                htim1.Instance = TIM1;
                htim1.Init.Prescaler = SystemCoreClock / 10000 - 1;
                htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
                htim1.Init.Period = cycle*10 - 1;
                htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
                htim1.Init.RepetitionCounter = 0;
                htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
                if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
                {
                Error_Handler();
                }
                sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
                if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
                {
                Error_Handler();
                }
                if (HAL_TIM_OC_Init(&htim1) != HAL_OK)
                {
                Error_Handler();
                }
                sSlaveConfig.SlaveMode = TIM_SLAVEMODE_RESET;
                sSlaveConfig.InputTrigger = TIM_TS_ITR0;
                if (HAL_TIM_SlaveConfigSynchro(&htim1, &sSlaveConfig) != HAL_OK)
                {
                Error_Handler();
                }
                sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
                sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
                sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
                if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
                {
                Error_Handler();
                }
                sConfigOC.OCMode = TIM_OCMODE_TIMING;
                sConfigOC.Pulse = 0;
                sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
                sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
                sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
                sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
                sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
                if (HAL_TIM_OC_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
                {
                Error_Handler();
                }
                sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
                sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
                sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
                sBreakDeadTimeConfig.DeadTime = 0;
                sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
                sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
                sBreakDeadTimeConfig.BreakFilter = 0;
                sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
                sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
                sBreakDeadTimeConfig.Break2Filter = 0;
                sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
                if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
                {
                Error_Handler();
                }
            }
            break;
        case 1:
            {
                //信号源参数
                /* USER CODE BEGIN TIM2_Init 1 */

                /* USER CODE END TIM2_Init 1 */
                htim2.Instance = TIM2;
                htim2.Init.Prescaler = SystemCoreClock / 10000 - 1;
                htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
                htim2.Init.Period = cycle*10 - 1;
                htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
                htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
                if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
                {
                Error_Handler();
                }
                sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
                if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
                {
                Error_Handler();
                }
                sSlaveConfig.SlaveMode = TIM_SLAVEMODE_RESET;
                sSlaveConfig.InputTrigger = TIM_TS_ITR0;
                if (HAL_TIM_SlaveConfigSynchro(&htim2, &sSlaveConfig) != HAL_OK)
                {
                Error_Handler();
                }
                sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
                sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
                if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
                {
                Error_Handler();
                }        

            }
            break;

       
        
        default:
            break;
    }

    SYS_OK();
}
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
uint8 SysTimer_DlyConfig(THwTimerFunc func, void* fpara, uint8 tmrIdx, uint16 mills)
{
    SYS_VAR_CHECK(tmrIdx >= 4);              //判断参数的正确性
    SYS_VAR_CHECK(mills == 0);               //判断参数的正确性
    gfs_hwTimerDelay[tmrIdx] = func;        //回调函数及传入参数
    paraHwTimerDelay[tmrIdx] = fpara;
                                            //定时周期数
    gul_HwTimerDelay[tmrIdx] = mills;

    Hal_Timer_Config(tmrIdx);

//	    SysTimer_DlyStart(tmrIdx);
    SYS_OK();
}



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
uint8 SysTimer_DlyStart(uint8 tmrIdx)
{

    switch(tmrIdx)
    {
    case 0:
        //信号源参数
//        /* TIMER0 channel control update interrupt enable */
//        timer_interrupt_enable(TIMER0,TIMER_INT_CMT);
//        /* TIMER0 break interrupt disable */
//        timer_interrupt_enable(TIMER0,TIMER_INT_BRK);
//
//        /* TIMER0 counter enable */
//        timer_enable(TIMER0);
//#ifndef GD32F30X_HD
//		nvic_irq_enable(TIMER0_TRG_CMT_TIMER10_IRQn, 2, 1);
//#else
//		nvic_irq_enable(TIMER0_TRG_CMT_IRQn, 0, 1);
//#endif

//		timer_interrupt_enable(TIMER0,TIMER_INT_CH0);
//		timer_enable(TIMER0);
//
//		nvic_irq_enable(TIMER0_Channel_IRQn, 0, 1);
//        __HAL_RCC_TIM1_CLK_ENABLE();
//        /* TIM1 interrupt Init */
//        HAL_NVIC_SetPriority(TIM1_TRG_COM_IRQn, 0, 0);
//        HAL_NVIC_EnableIRQ(TIM1_TRG_COM_IRQn);
//        HAL_NVIC_SetPriority(TIM1_CC_IRQn, 0, 0);
//        HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);
        HAL_TIM_Base_Start_IT(&htim1);
        
        break;
    case 1:
        //信号源参数
		/* TIMER1 channel control update interrupt enable */
//		timer_interrupt_enable(TIMER1,TIMER_INT_CH0);
//		/* TIMER1 break interrupt disable */
////			timer_interrupt_disable(TIMER1,TIMER_INT_BRK);
//
//		/* TIMER1 counter enable */
//		timer_enable(TIMER1);
//		nvic_irq_enable(TIMER1_IRQn, 0, 1);
//        __HAL_RCC_TIM2_CLK_ENABLE();
//        /* TIM2 interrupt Init */
//        HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
//        HAL_NVIC_EnableIRQ(TIM2_IRQn);
        HAL_TIM_Base_Start_IT(&htim2);
        break;        
    default:
   
        break;
    }
	
	SYS_OK();  
}

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
uint8 SysTimer_DlyStop(uint8 tmrIdx)
{
    SYS_VAR_CHECK(tmrIdx >= 4);           //判断参数的正确性
    switch(tmrIdx)                      //根据号码进行散转
    {
        case 0:                         //关闭中断功能
            /* TIMER0 channel control update interrupt enable */
//	            timer_interrupt_disable(TIMER0,TIMER_INT_CMT);
            /* TIMER0 break interrupt disable */
//            timer_interrupt_disable(TIMER0,TIMER_INT_CH0);
//
//            /* TIMER0 counter enable */
//            timer_disable(TIMER0);
//			nvic_irq_disable(TIMER0_Channel_IRQn);
          /* USER CODE BEGIN TIM1_MspDeInit 0 */

          /* USER CODE END TIM1_MspDeInit 0 */
            /* Peripheral clock disable */
//            __HAL_RCC_TIM1_CLK_DISABLE();
//
//            /* TIM1 interrupt DeInit */
//            HAL_NVIC_DisableIRQ(TIM1_TRG_COM_IRQn);
//            HAL_NVIC_DisableIRQ(TIM1_CC_IRQn);
          /* USER CODE BEGIN TIM1_MspDeInit 1 */
            HAL_TIM_Base_Stop_IT(&htim1);
          /* USER CODE END TIM1_MspDeInit 1 */
            break;
        case 1:                         //关闭中断功能
			/* TIMER0 channel control update interrupt enable */
//			timer_interrupt_disable(TIMER1,TIMER_INT_CMT);
//			/* TIMER0 break interrupt disable */
//			timer_interrupt_disable(TIMER1,TIMER_INT_BRK);
//
//			/* TIMER0 counter enable */
//			timer_disable(TIMER1);
//
        //			nvic_irq_disable(TIMER1_IRQn);
//            __HAL_RCC_TIM2_CLK_DISABLE();
//
//            /* TIM2 interrupt DeInit */
//            HAL_NVIC_DisableIRQ(TIM2_IRQn);
            HAL_TIM_Base_Start_IT(&htim2);
            break;

        default:
            return SYS_ERR_FT;
//	            Chip_TIMER_MatchDisableInt(LPC_HWTIMER, 3);
//	            Chip_TIMER_StopOnMatchDisable(LPC_HWTIMER, 3);
            break;
    }
    SYS_OK();                            //返回停止成功
}




/************************************************************************
 * @Function: SysTimer_GetStatus
 * @Description: 获取硬件定时器状态
 * 
 * @Arguments: 
 * @param: tmrIdx 表示第几路 
 * 
 * @Note: 这个函数不是进程安全的
 * @Return: bool true使能,false禁能  
 * @Auther: yzy
 * Date: 2015/5/18
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
bool SysTimer_GetStatus(uint8 tmrIdx)
{
    SYS_VAR_CHECK(tmrIdx >= 4);              //判断参数的正确性
//	    ETIMx_Type* ETIMx;
    uint8_t stt = 0;
    switch(tmrIdx)
    {
        case 0:
//            stt = timer_flag_get(TIMER0, TIMER_INT_FLAG_CMT);
            stt = HAL_TIM_OC_GetState(&htim1);
            break;
        case 1:
//			stt = timer_flag_get(TIMER1, TIMER_INT_FLAG_CMT);
            stt = HAL_TIM_OC_GetState(&htim2);
            break;       
        default:
            return false;
            break;
    }
    if(HAL_TIM_STATE_BUSY == stt)
    {
        return true;
    }
    else
    {
        return false;
    }
}




#if (true)   //级联硬件定时器功能


/************************************************************************
 * @描述:级联hwtimer的链表
 ************************************************************************/
TCasHwTimer* gsp_CasHwTimerLink = __NULL;

uint8 CasHwTimerID = 0xff;




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
uint8 casHwTimerCreate(TCasHwTimerFunc tickfunc, void* tickpara, uint8 timerid, uint16 dlytick)
{
    TCasHwTimer* p;
    TCasHwTimer* q, *r; 

    SYS_ENTER_AllSCRT();                     //与中断函数共享资源,故需保证中断安全

    p = gsp_CasHwTimerLink;
    while(p != __NULL)                      //循环扫描到最后的timer
    {
        if(p->timerid == timerid)           //该ID的timer已经被创建则返回
        {
            SYS_EXIT_AllSCRT();
            return SYS_ERR_EXIST;
        }
        q = p;                              //保留上一个
        p = p->pnext;                       //下一个
    }
                                            //赋初值
    r = (TCasHwTimer*)m_malloc(sizeof(TCasHwTimer));
	r->timerid = timerid;
    r->tickfunc = tickfunc;
    r->tickpara = tickpara;
    r->timeout = dlytick;
    r->dlycnt = 0;
    r->pnext = __NULL;
    r->stoped = 1;                          //默认停止
    
    if(gsp_CasHwTimerLink == __NULL)
    {
        gsp_CasHwTimerLink = r;
    }
    else
    {
        q->pnext = r;
    }

    SYS_EXIT_AllSCRT();                      //解除共享
    SYS_OK();
}



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
uint8 casHwTimerStart(uint8 timerid)
{
    TCasHwTimer* p;

    SYS_VAR_CHECK(CasHwTimerID >= 4);
    SYS_ENTER_AllSCRT();                     //与中断函数共享资源,故需保证中断安全

    p = gsp_CasHwTimerLink;
    while(p != __NULL)                      //循环扫描到最后的timer
    {
        if(p->timerid == timerid)           //该ID的timer已经被创建则返回
        {
            p->stoped = 0;                  //启动          
            p->dlycnt = p->timeout + 1;     //防止定时器中途停止后重新启动时,dlyctr 值不等于 timeout.
            
            if(!SysTimer_GetStatus(CasHwTimerID))
            {
                SysTimer_DlyStart(CasHwTimerID);
            }
            SYS_EXIT_AllSCRT();
            return SYS_ERR_OK;
        }
        
        p = p->pnext;                       //下一个
    }
    SYS_EXIT_AllSCRT();
    return SYS_ERR_EXIST;                    //返回试图启动不存在的timer
}


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
uint8 casHwTimerStop(uint8 timerid)
{
    TCasHwTimer* p;
    uint8 ret = SYS_ERR_EXIST;
    bool _allstop = true;                   //是否所有定时器均停止
    
    SYS_VAR_CHECK(CasHwTimerID >= 4);
    
    SYS_ENTER_AllSCRT();                     //与中断函数共享资源,故需保证中断安全
    p = gsp_CasHwTimerLink;
    
    while(p != __NULL)                      //循环扫描到最后的timer
    {
        if(p->timerid == timerid)           //该ID的timer已经被创建则返回
        {
            p->stoped = 1;                  //停止
            ret = SYS_ERR_OK;                //执行成功
        }
        
        if(p->stoped != 1)                  //判断是否所有定时器均停止
        {
            _allstop = false;
        }
        
        p = p->pnext;                       //下一个
    }
    
    if(_allstop)
    {
        SysTimer_DlyStop(CasHwTimerID);
    }
    SYS_EXIT_AllSCRT();
    
    return ret;                             //返回试图启动不存在的timer
}

uint8 casHwTimerClose(uint8 timerid)
{
    TCasHwTimer* p;
    TCasHwTimer* q;
    uint8 ret = SYS_ERR_EXIST;
    bool _allstop = true;                   //是否所有定时器均停止
    uint8_t flg = 0;
    SYS_VAR_CHECK(CasHwTimerID >= 4);
    
    SYS_ENTER_AllSCRT();                     //与中断函数共享资源,故需保证中断安全
    p = gsp_CasHwTimerLink;
    
    while(p != __NULL)                      //循环扫描到最后的timer
    {
        flg = 0;
        if(p->timerid == timerid)           //该ID的timer已经被创建则返回
        {
            p->stoped = 1;                  //停止
            ret = SYS_ERR_OK;                //执行成功
            flg = 1;
        }
        
        if(p->stoped != 1)                  //判断是否所有定时器均停止
        {
            _allstop = false;
        }
        q=p;
        p = p->pnext;                       //下一个
        if(flg)
        {
//#ifndef __MODULE__    
            m_free(q);
//#else
//            FnSFree(q);
//#endif    
            q=NULL;
        }
    }
    
    if(_allstop)
    {
        SysTimer_DlyStop(CasHwTimerID);
    }
    SYS_EXIT_AllSCRT();
    
    return ret;         


}

/************************************************************************
 * @Function: _casHwTimerTickProc
 * @Description: 定时中断处理
 * 
 * @Arguments: 
 * @param: para 
 * @Note: 
 * @Auther: yzy
 * Date: 2015/6/2
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void _casHwTimerTickProc(void* para)
{
    TCasHwTimer* p = gsp_CasHwTimerLink;

    if(p != __NULL)
    { 
        while(p != __NULL)                  //不断循环处理各个timer
        {
            if(p->stoped == 0)              //判断是否启动
            {
                p->dlycnt--;                //倒计时

                if(p->dlycnt == 0)          //到0则触发
                {
                    p->dlycnt = p->timeout; //timer恢复然后调用回调函数
                    p->tickfunc(p->tickpara);
                }
            }
            p = p->pnext;                   //下一个
        }
    }
}




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
uint8 casHwTimerConfig(uint8 sysTimerID, uint16 tickMills)
{
    uint8 ret;
    SYS_VAR_CHECK(sysTimerID >= 4);
    
    SYS_ENTER_AllSCRT();                     //与中断函数共享资源,故需保证中断安全
    CasHwTimerID = sysTimerID;
    ret = SysTimer_DlyConfig(_casHwTimerTickProc, __NULL, sysTimerID, tickMills);
    SYS_EXIT_AllSCRT();
    
    return ret;
}


#endif          //#if (true)   //级联硬件定时器功能

//	void RCHF_Adj_EtimxCfg(ETIMx_Type* ETIMx)
//	{
//		ETIM_InitTypeDef init_para;
//		volatile uint8_t EtimNum;
//	
//		EtimNum = ((uint32_t)ETIMx - ETIMER1_BASE)>>5;
//		
//		//信号源参数
//		init_para.sig_src_para.SIG1SEL = ETIMx_ETxINSEL_SIG1SEL_GROUP1;		/* 内部信号1源选择GROUP1 */
//		init_para.sig_src_para.SIG2SEL = ETIMx_ETxINSEL_SIG2SEL_GROUP2;		/* 内部信号2源选择GROUP2 */
//		
//		switch(EtimNum)
//		{
//			case 0:	//ETIM1
//				//信号源参数
//				init_para.sig_src_para.GRP1SEL = ETIMx_ETxINSEL_GRP1SEL_ET1_APBCLK;		/* GROUP1 信号选择APBCLK */
//				init_para.sig_src_para.GRP2SEL = ETIMx_ETxINSEL_GRP2SEL_ET1_XTLF;		/* GROUP2 信号选择XTLF */			
//				//ET1时钟使能	
//				RCC_PERCLK_SetableEx( ET1CLK, ENABLE );		
//				//NVIC中断配置
//				NVIC_DisableIRQ(ETIM1_IRQn);	
//				break;
//	
//			case 1:	//ETIM2
//				//信号源参数
//				init_para.sig_src_para.GRP1SEL = ETIMx_ETxINSEL_GRP1SEL_ET2_APBCLK;		/* GROUP1 信号选择APBCLK */
//				init_para.sig_src_para.GRP2SEL = ETIMx_ETxINSEL_GRP2SEL_ET2_XTLF;		/* GROUP2 信号选择XTLF */		
//				//ET2时钟使能	
//				RCC_PERCLK_SetableEx( ET2CLK, ENABLE );	
//				//NVIC中断配置
//				NVIC_DisableIRQ(ETIM2_IRQn);			
//				break;
//	
//			case 2:	//ETIM3
//				//信号源参数
//				init_para.sig_src_para.GRP1SEL = ETIMx_ETxINSEL_GRP1SEL_ET3_APBCLK;		/* GROUP1 信号选择APBCLK */
//				init_para.sig_src_para.GRP2SEL = ETIMx_ETxINSEL_GRP2SEL_ET3_XTLF;		/* GROUP2 信号选择XTLF */		
//				//ET3时钟使能
//				RCC_PERCLK_SetableEx( ET3CLK, ENABLE );	
//				//NVIC中断配置
//				NVIC_DisableIRQ(ETIM3_IRQn);		
//				break;
//	
//			case 3:	//ETIM4
//				//信号源参数
//				init_para.sig_src_para.GRP1SEL = ETIMx_ETxINSEL_GRP1SEL_ET4_APBCLK;		/* GROUP1 信号选择APBCLK */
//				init_para.sig_src_para.GRP2SEL = ETIMx_ETxINSEL_GRP2SEL_ET4_XTLF;		/* GROUP2 信号选择XTLF */		
//				//ET4时钟使能
//				RCC_PERCLK_SetableEx( ET4CLK, ENABLE );		
//				//NVIC中断配置
//				NVIC_DisableIRQ(ETIM4_IRQn);
//				break;
//			
//			default:
//				break;
//		}
//		init_para.sig_src_para.PRESCALE1 = 0;    		//1分频		/* ETx预分频寄存器1 */ 00表示1分频，FF表示256分频
//		init_para.sig_src_para.PRESCALE2 = 127;    		//128分频		/* ETx预分频寄存器2 */ 00表示1分频，FF表示256分频
//		
//		//控制类参数
//		init_para.ctrl_para.EXFLT = ENABLE;								/* 引脚输入数字滤波使能打开 */
//		init_para.ctrl_para.MOD = ETIMx_ETxCR_MOD_CAPTURE;				/* 工作模式选择捕捉模式 */
//		init_para.ctrl_para.CASEN = DISABLE;							/* 扩展定时器级联关闭 */
//		init_para.ctrl_para.EDGESEL = ETIMx_ETxCR_EDGESEL_RISING;		/* 计数模式采沿方式选择（计数时钟选择mcu_clk时该位无效，总是采用mcu_clk时钟上升沿计数） */
//	
//		init_para.ctrl_para.PWM = DISABLE;								/* PWM输出关闭 */
//	
//		init_para.ctrl_para.CAPMOD = ETIMx_ETxCR_CAPMOD_PERIOD;			/* 捕捉模式控制 */
//		init_para.ctrl_para.CAPCLR = ENABLE;							/* 带清零捕捉模式控制 *///使能后计数器保持0，捕捉到第一个有效沿之后timer才开始计数，之后的捕捉不会再清零，只有关闭定时器后再打开
//		init_para.ctrl_para.CAPONCE = ENABLE;							/* 单次捕捉控制 */
//		init_para.ctrl_para.CAPEDGE = ETIMx_ETxCR_CAPEDGE_RISING;		/* 捕捉沿选择 */
//	
//		init_para.ctrl_para.CMPIE = DISABLE;		/* 扩展定时器比较中断使能 */
//		init_para.ctrl_para.CAPIE = DISABLE;		/* 扩展定时器捕捉中断使能 */
//		init_para.ctrl_para.OVIE = DISABLE;			/* 扩展定时器溢出中断使能 */
//	
//		init_para.ctrl_para.INITVALUE = 0; 		//计数初值;		/* ETx初值寄存器 */
//		init_para.ctrl_para.CMP = 0;			/* ETx比较寄存器 */
//		
//		init_para.ctrl_para.CEN = DISABLE;		/* 启动控制 */
//		
//		//初始化ETIM
//		ETIMx_Init(ETIMx, &init_para);
//	}
//	
//	//xtlf的32768Hz时钟作为参考源
//	//本函数仅适用于APB不分频的情况
//	//ClkMode 1 = 8M
//	//ClkMode 2 = 16M
//	//ClkMode 3 = 24M
//	//ClkMode 4 = 32M
//	void RCHF_Adj_Proc(ETIMx_Type* ETIMx, uint8_t ClkMode )
//	{
//		volatile uint16 Temp16;
//		unsigned char n,m,tempbyte,TrimBuf;	
//		float DIVclkmode = 1.0;
//		
//		//FDETO	停振检测模块输出
//		//1：XTLF未停振
//		//0：XTLF停振
//		if( RESET == ANAC_FDETIF_FDETO_Chk())//xtlf停振
//		{
//			//使用测试参数		
//			RCC_Init_RCHF_Trim(ClkMode);
//			return;
//		}
//		
//		RCHF_Adj_EtimxCfg(ETIMx);
//	
//		switch(ClkMode)
//		{
//			case 1://8
//				ETIMx_ETxPESCALE1_Write(ETIMx, 0);//计数源1分频
//				DIVclkmode		= 1.0;
//				break;
//			
//			case 2://16/2
//				ETIMx_ETxPESCALE1_Write(ETIMx, 1);//计数源2分频
//				DIVclkmode		= 1.0;
//				break;
//			
//			case 3://24/4
//				ETIMx_ETxPESCALE1_Write(ETIMx, 3);//计数源4分频
//				DIVclkmode		= 8.0/6.0;
//				break;
//			
//			case 4://36/4
//				ETIMx_ETxPESCALE1_Write(ETIMx, 3);//计数源4分频
//				DIVclkmode		= 8.0/9.0;
//				break;
//			
//			default://8
//				ETIMx_ETxPESCALE1_Write(ETIMx, 0);//计数源不分频
//				DIVclkmode		= 1.0;
//				break;	
//		}	
//			
//		for( m=0; m<6; m++ )
//		{			
//			ETIMx_ETxCR_CEN_Setable(ETIMx, ENABLE);	//启动定时器
//			
//			for( n=0; n<2; n++ ) //理论上判断需要3.906ms = 128/32768 s
//			{		
//				Temp16 = 0;
//				Do_DelayStart();
//				{
//					if( SET == ETIMx_ETxIF_CAPIF_Chk(ETIMx) ) {Temp16 = 1; break;}//等待捕捉中断标志
//				}While_DelayMsEnd(7.5*clkmode);		//7.5ms
//				ETIMx_ETxIF_CAPIF_Clr(ETIMx);		//清除捕捉中断标志
//			}	
//			ETIMx_ETxCR_CEN_Setable(ETIMx, DISABLE);	//关闭定时器
//			if( Temp16 == 0 ) continue;
//				
//			Temp16 = ETIMx_ETxIVR_Read(ETIMx);		//读取捕捉值
//	        if( Temp16 == 0 ) continue;
//			if( (Temp16>(31250-120)/DIVclkmode)&&(Temp16<(31250+120)/DIVclkmode) ) //return;//修正到理想值直接返回
//			{
//				break;
//			}
//			else
//			{
//				TrimBuf = RCC_RCHFTRIM_Read();	//读取RCHF校准值
//				if( Temp16 > 31250/DIVclkmode )	//31250 = 8M*128/32768; 156.25 = (15625*2/4*2M)*40k
//				{
//					Temp16 -= 31250/DIVclkmode;
//					tempbyte = (uint8_t)((float)Temp16/(156.25/DIVclkmode)+0.5);
//					
//					if( tempbyte > 0x20 ) tempbyte = 0x20;
//					
//					if( TrimBuf >= tempbyte )
//					{
//						RCC_RCHFTRIM_Write(TrimBuf - tempbyte);//更新RCHF校准值
//					}
//					else
//					{
//						RCC_RCHFTRIM_Write(0x00);//更新RCHF校准值
//					}			
//				}
//				else
//				{
//					Temp16 = (uint16_t)(31250/DIVclkmode-Temp16);
//					tempbyte = (uint8_t)((float)Temp16/(156.25/DIVclkmode)+0.5);
//					
//					if( tempbyte > 0x20 ) tempbyte = 0x20;
//					
//					if( (TrimBuf + tempbyte) <= 0x7F)
//					{
//						RCC_RCHFTRIM_Write(TrimBuf + tempbyte);//更新RCHF校准值
//					}
//					else
//					{
//						RCC_RCHFTRIM_Write(0x7F);//更新RCHF校准值
//					}
//				}
//				
//				if( m == 5 )				//未找到理想值,采用最后2次的值取平均
//				{
//					TrimBuf += RCC_RCHFTRIM_Read();
//					RCC_RCHFTRIM_Write((uint32_t)(TrimBuf/2.0+0.5));				
//				}
//			}
//		}
//	}


