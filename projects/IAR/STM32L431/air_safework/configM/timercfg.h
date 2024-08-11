/****************************************Copyright (c)*************************
**                               	________科技有限公司
**                                     		开发部
**
**
**--------------文件信息-------------------------------------------------------
**文   件   名: timercfg.h
**创   建   人: yzy
**最后修改日期: 2014年07月25日
**描        述: 串口配置函数集合
**注        意:
**--------------历史版本信息---------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2014年07月25日
** 描　述: 原始版本
******************************************************************************/
#ifndef _TIMERCFG_H_
#define _TIMERCFG_H_

#ifndef EXT_TIMER
#define EXT_TIMER    extern
#endif

#define TIMER_CLOCK_SRC (_0001_TAU_CKM0_fCLK_1 | _0010_TAU_CKM1_fCLK_1 | _0000_TAU_CKM2_fCLK_1 | _0000_TAU_CKM3_fCLK_8)

#if  SYS_TIMER0_EN > 0
    const TimerSets gs_Timer0TS = 
    {
        1,//指令周期个数
        MODE_PWM_MASTER,//PWM function master
        0,
        TIMN_EDGE_FALLING,//下降沿
        TRIGGER_SOFTWARE,
        COMBINATION_MASTER,//master
        CLOCK_MODE_CKS,//clock MCK specified by CKSmn bit
        0,
        CLOCK_SELECT_CKM0,
        0,//正逻辑 
        0,
        0,//不输出 
    }

    void Timer0_PinCfg(void)
    {
//	        Chip_GPIO_SetPinPFSEG(0, 1, 1);//P01
//	        Chip_GPIO_SetPinDIROutput(0,1);
//	        Chip_GPIO_SetPinState(0,1, false);

    };
    
    const TimerID gs_Timer0SID = 
    {
        0,
        SYS_TIMER0_IDX,    
        RENSE_INT_PRI_TMPR00,
        3,
        TIMER_TRIGGER_ON,
        (TimerSets *)&gs_Timer0TS,
        NULL,//RENSE_GPIOINT,
        Timer0_PinCfg,
    };
    
    void Timer0_IRQHandler(void)
    {
        CPSR_ALLOC();
        RHINO_CPU_INTRPT_DISABLE();
        Timerx_Handler(&gs_Timer0SID);
        RHINO_CPU_INTRPT_ENABLE();    
    }
#else
    void Timer0_IRQHandler(void)
    {
        CPSR_ALLOC();
        RHINO_CPU_INTRPT_DISABLE();
        
        RHINO_CPU_INTRPT_ENABLE();    
    }
#endif
/******************************************************************************
各外部中断口的配置
******************************************************************************/
#if  SYS_TIMER1_EN > 0
    const TimerSets gs_Timer1TS = 
    {
        1,//指令周期个数
        MODE_PWM_SLAVE,//PWM function slave
        0,
        TIMN_EDGE_FALLING,//双边沿
        TRIGGER_MASTER_INT,
        COMBINATION_SLAVE,//slave
        CLOCK_MODE_CKS,//cks  clock MCK specified by CKSmn bit
        0,
        CLOCK_SELECT_CKM0,
        0,//正逻辑  
        0,//输出0
        1,//输出波形 
    }
    
    void Timer1_PinCfg(void)
    {
//	        Chip_GPIO_SetPinPFSEG(0, 1, 1);//P01
//	        Chip_GPIO_SetPinDIROutput(0,1);
//	        Chip_GPIO_SetPinState(0,1, false);

    };
    
    const TimerID gs_Timer1SID = 
    {
        1,
        SYS_TIMER1_IDX,    
        RENSE_INT_PRI_TMPR01,
        3,
        TIMER_OUTPUT_ON | TIMER_TRIGGER_ON,
        (TimerSets *)&gs_Timer1TS,
        NULL,//RENSE_GPIOINT,
        Timer1_PinCfg,
    };
    
    void Timer1_IRQHandler(void)
    {
        CPSR_ALLOC();
        RHINO_CPU_INTRPT_DISABLE();
        Timerx_Handler(&gs_Timer1SID);
        RHINO_CPU_INTRPT_ENABLE();    
    }
#else
    void Timer1_IRQHandler(void)
    {
        CPSR_ALLOC();
        RHINO_CPU_INTRPT_DISABLE();
        
        RHINO_CPU_INTRPT_ENABLE();    
    }
#endif

#if  SYS_TIMER2_EN > 0
    const TimerSets gs_Timer2TS = 
    {
        1,//指令周期个数
        MODE_PWM_MASTER,//PWM function master
        0,
        TIMN_EDGE_FALLING,//下降沿
        TRIGGER_SOFTWARE,
        COMBINATION_MASTER,//master
        CLOCK_MODE_CKS,//clock MCK specified by CKSmn bit
        0,
        CLOCK_SELECT_CKM0,
        0,//正逻辑 
        0,
        0,//不输出 
    }

    void Timer2_PinCfg(void)
    {
//	        Chip_GPIO_SetPinPFSEG(0, 1, 1);//P01
//	        Chip_GPIO_SetPinDIROutput(0,1);
//	        Chip_GPIO_SetPinState(0,1, false);

    };
    
    const TimerID gs_Timer2SID = 
    {
        2,
        SYS_TIMER2_IDX,    
        RENSE_INT_PRI_TMPR02,
        3,
        TIMER_TRIGGER_ON,
        (TimerSets *)&gs_Timer2TS,
        NULL,//RENSE_GPIOINT,
        Timer2_PinCfg,
    };
    
    void Timer2_IRQHandler(void)
    {
        CPSR_ALLOC();
        RHINO_CPU_INTRPT_DISABLE();
        Timerx_Handler(&gs_Timer2SID);
        RHINO_CPU_INTRPT_ENABLE();    
    }
#else
    void Timer2_IRQHandler(void)
    {
        CPSR_ALLOC();
        RHINO_CPU_INTRPT_DISABLE();
        
        RHINO_CPU_INTRPT_ENABLE();    
    }
#endif
/******************************************************************************
各外部中断口的配置
******************************************************************************/
#if  SYS_TIMER3_EN > 0
    const TimerSets gs_Timer3TS = 
    {
        1,//指令周期个数
        MODE_PWM_SLAVE,//PWM function slave
        0,
        TIMN_EDGE_FALLING,//双边沿
        TRIGGER_MASTER_INT,
        COMBINATION_SLAVE,//slave
        CLOCK_MODE_CKS,//cks  clock MCK specified by CKSmn bit
        0,
        CLOCK_SELECT_CKM0,
        0,//正逻辑  
        0,//输出0
        1,//输出波形 
    }
    
    void Timer3_PinCfg(void)
    {
//	        Chip_GPIO_SetPinPFSEG(4, 1, 1);//P41
//	        Chip_GPIO_SetPinDIROutput(4,1);
//	        Chip_GPIO_SetPinState(4,1, false);

    };
    
    const TimerID gs_Timer3SID = 
    {
        3,
        SYS_TIMER3_IDX,    
        RENSE_INT_PRI_TMPR03,
        3,
        TIMER_OUTPUT_ON | TIMER_TRIGGER_ON,
        (TimerSets *)&gs_Timer3TS,
        NULL,//RENSE_GPIOINT,
        Timer3_PinCfg,
    };
    
    void Timer3_IRQHandler(void)
    {
        CPSR_ALLOC();
        RHINO_CPU_INTRPT_DISABLE();
        Timerx_Handler(&gs_Timer3SID);
        RHINO_CPU_INTRPT_ENABLE();    
    }
#else
    void Timer3_IRQHandler(void)
    {
        CPSR_ALLOC();
        RHINO_CPU_INTRPT_DISABLE();
        
        RHINO_CPU_INTRPT_ENABLE();    
    }
#endif



#if  SYS_TIMER4_EN > 0
    const TimerSets gs_Timer4TS = 
    {
        1,//指令周期个数
        MODE_PWM_MASTER,//PWM function master
        0,
        TIMN_EDGE_FALLING,//下降沿
        TRIGGER_SOFTWARE,
        COMBINATION_MASTER,//master
        CLOCK_MODE_CKS,//clock MCK specified by CKSmn bit
        0,
        CLOCK_SELECT_CKM0,
        0,//正逻辑 
        0,
        0,//不输出 
    };

    void Timer4_PinCfg(void)
    {
//	        Chip_GPIO_SetPinPFSEG(0, 1, 1);//P01
//	        Chip_GPIO_SetPinDIROutput(0,1);
//	        Chip_GPIO_SetPinState(0,1, false);

    };
    
    const TimerID gs_Timer4SID = 
    {
        4,
        SYS_TIMER4_IDX,    
        RENSE_INT_PRI_TMMK04,
        3,
        TIMER_TRIGGER_ON,
        (TimerSets *)&gs_Timer4TS,
        NULL,//RENSE_GPIOINT,
        Timer4_PinCfg,
    };
    
    void Timer4_IRQHandler(void)
    {
        CPSR_ALLOC();
        RHINO_CPU_INTRPT_DISABLE();
        Timerx_Handler(&gs_Timer4SID);
        RHINO_CPU_INTRPT_ENABLE();    
    }
#else
    void Timer4_IRQHandler(void)
    {
        CPSR_ALLOC();
        RHINO_CPU_INTRPT_DISABLE();
        
        RHINO_CPU_INTRPT_ENABLE();    
    }
#endif
/******************************************************************************
各外部中断口的配置
******************************************************************************/
#if  SYS_TIMER5_EN > 0
    const TimerSets gs_Timer5TS = 
    {
        1,//指令周期个数
        MODE_PWM_SLAVE,//PWM function slave
        0,
        TIMN_EDGE_FALLING,//双边沿
        TRIGGER_MASTER_INT,
        COMBINATION_SLAVE,//slave
        CLOCK_MODE_CKS,//cks  clock MCK specified by CKSmn bit
        0,
        CLOCK_SELECT_CKM0,
        0,//正逻辑  
        0,//输出0
        1,//输出波形 
    };
    
    void Timer5_PinCfg(void)
    {
        Chip_GPIO_SetPinPFSEG(0, 1, 1);//P01
        Chip_GPIO_SetPinDIROutput(0,1);
        Chip_GPIO_SetPinState(0,1, false);

    };
    
    const TimerID gs_Timer5SID = 
    {
        5,
        SYS_TIMER5_IDX,    
        RENSE_INT_PRI_TMMK05,
        3,
        (TIMER_MODE)(TIMER_OUTPUT_ON | TIMER_TRIGGER_ON),
        (TimerSets *)&gs_Timer5TS,
        NULL,//RENSE_GPIOINT,
        Timer5_PinCfg,
    };
    
    void Timer5_IRQHandler(void)
    {
        CPSR_ALLOC();
        RHINO_CPU_INTRPT_DISABLE();
        Timerx_Handler(&gs_Timer5SID);
        RHINO_CPU_INTRPT_ENABLE();    
    }
#else
    void Timer5_IRQHandler(void)
    {
        CPSR_ALLOC();
        RHINO_CPU_INTRPT_DISABLE();
        
        RHINO_CPU_INTRPT_ENABLE();    
    }
#endif

#if  SYS_TIMER6_EN > 0
    const TimerSets gs_Timer6TS = 
    {
        1,//指令周期个数
        MODE_PWM_MASTER,//PWM function master
        0,
        TIMN_EDGE_FALLING,//下降沿
        TRIGGER_SOFTWARE,
        COMBINATION_MASTER,//master
        CLOCK_MODE_CKS,//clock MCK specified by CKSmn bit
        0,
        CLOCK_SELECT_CKM0,
        0,//正逻辑 
        0,
        0,//不输出 
    };

    void Timer6_PinCfg(void)
    {
//	        Chip_GPIO_SetPinPFSEG(0, 1, 1);//P01
//	        Chip_GPIO_SetPinDIROutput(0,1);
//	        Chip_GPIO_SetPinState(0,1, false);

    };
    
    const TimerID gs_Timer6SID = 
    {
        6,
        SYS_TIMER6_IDX,    
        RENSE_INT_PRI_TMMK06,
        3,
        TIMER_TRIGGER_ON,
        (TimerSets *)&gs_Timer6TS,
        NULL,//RENSE_GPIOINT,
        Timer6_PinCfg,
    };
    
    void Timer6_IRQHandler(void)
    {
        CPSR_ALLOC();
        RHINO_CPU_INTRPT_DISABLE();
        Timerx_Handler(&gs_Timer6SID);
        RHINO_CPU_INTRPT_ENABLE();    
    }
#else
    void Timer6_IRQHandler(void)
    {
        CPSR_ALLOC();
        RHINO_CPU_INTRPT_DISABLE();
        
        RHINO_CPU_INTRPT_ENABLE();    
    }
#endif
/******************************************************************************
各外部中断口的配置
******************************************************************************/
#if  SYS_TIMER7_EN > 0
    const TimerSets gs_Timer7TS = 
    {
        1,//指令周期个数
        MODE_PWM_SLAVE,//PWM function slave
        0,
        TIMN_EDGE_FALLING,//双边沿
        TRIGGER_MASTER_INT,
        COMBINATION_SLAVE,//slave
        CLOCK_MODE_CKS,//cks  clock MCK specified by CKSmn bit
        0,
        CLOCK_SELECT_CKM0,
        0,//正逻辑  
        0,//输出0
        1,//输出波形 
    };
    
    void Timer7_PinCfg(void)
    {
        Chip_GPIO_SetPinPFSEG(4, 1, 1);//P41
        Chip_GPIO_SetPinDIROutput(4,1);
        Chip_GPIO_SetPinState(4,1, false);

    };
    
    const TimerID gs_Timer7SID = 
    {
        7,
        SYS_TIMER7_IDX,    
        RENSE_INT_PRI_TMMK07,
        3,
        (TIMER_MODE)(TIMER_OUTPUT_ON | TIMER_TRIGGER_ON),
        (TimerSets *)&gs_Timer7TS,
        NULL,//RENSE_GPIOINT,
        Timer7_PinCfg,
    };
    
    void Timer7_IRQHandler(void)
    {
        CPSR_ALLOC();
        RHINO_CPU_INTRPT_DISABLE();
        Timerx_Handler(&gs_Timer7SID);
        RHINO_CPU_INTRPT_ENABLE();    
    }
#else
    void Timer7_IRQHandler(void)
    {
        CPSR_ALLOC();
        RHINO_CPU_INTRPT_DISABLE();
        
        RHINO_CPU_INTRPT_ENABLE();    
    }
#endif
/******************************************************************************
硬件定时器配置的结构体集合
******************************************************************************/
const TimerID* const gss_TimerSID[NO_OF_TIMERERRUPT] = 
{
#if SYS_TIMER0_EN > 0
    &gs_Timer0SID,
#endif

#if SYS_TIMER1_EN > 0
    &gs_Timer1SID,
#endif

#if SYS_TIMER2_EN > 0
    &gs_Timer2SID,
#endif

#if SYS_TIMER3_EN > 0
    &gs_Timer3SID,
#endif

#if SYS_TIMER4_EN > 0
    &gs_Timer4SID,
#endif

#if SYS_TIMER5_EN > 0
    &gs_Timer5SID,
#endif

#if SYS_TIMER6_EN > 0
    &gs_Timer6SID,
#endif

#if SYS_TIMER7_EN > 0
    &gs_Timer7SID,
#endif

};

#endif
