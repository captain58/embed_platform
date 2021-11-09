/****************************************Copyright (c)*************************
**                               	________科技有限公司
**                                     		开发部
**
**
**--------------文件信息-------------------------------------------------------
**文   件   名: uartcfg.h
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
#ifndef _INTCFG_H_
#define _INTCFG_H_

#ifndef EXT_INT
#define EXT_INT    extern
#endif

/******************************************************************************
各外部中断口的配置
******************************************************************************/
#if  SYS_INT0_EN > 0
    void Int0Enable(void)
    {
        Chip_INT0_Set_Enable(RENSE_INT_PRI_PPR0);
    }
    
    void Int0Disable(void)
    {
        Chip_INT0_Set_Disable(RENSE_INT_PRI_PPR0); /* disable INT0 interrupt */
        Chip_INT0_Clear_Flag(RENSE_INT_PRI_PPR0);/* clear INT0 interrupt flag */
    }
    void Int0Priority(void)
    {
        Chip_INT_Set_PRI(RENSE_INT_PRI_PPR0, 0x03);
    }
    
    void Int0_PinCfg(void)
    {

    };
    
    const InterruptID gs_Int0SID = 
    {
        0,
        SYS_INT0_IDX,
        FALLING_EDGE_INT,
        0,
        0,
        Int0Enable,
        Int0Disable,
        Int0Priority,
        NULL,//RENSE_GPIOINT,
        Int0_PinCfg,
    };
    
    void Int0_IRQHandler(void)
    {
        CPSR_ALLOC();
        RHINO_CPU_INTRPT_DISABLE();
        Intx_Handler(&gs_Int0SID);
        RHINO_CPU_INTRPT_ENABLE();    
    }

#else
    void Int0_IRQHandler(void)
    {
        CPSR_ALLOC();
        RHINO_CPU_INTRPT_DISABLE();
        
        RHINO_CPU_INTRPT_ENABLE();    
    }
    
#endif

/******************************************************************************
各外部中断口的配置
******************************************************************************/
#if  SYS_INT1_EN > 0
    void Int1Enable(void)
    {
        Chip_INT0_Set_Enable(RENSE_INT_PRI_PPR1);
    }
    
    void Int1Disable(void)
    {
        Chip_INT0_Set_Disable( RENSE_INT_PRI_PPR1); /* disable INT0 interrupt */
        Chip_INT0_Clear_Flag( RENSE_INT_PRI_PPR1);/* clear INT0 interrupt flag */


    }
    void Int1Priority(void)
    {
        Chip_INT_Set_PRI(RENSE_INT_PRI_PPR1, 0x03);
    }
    
    void Int1_PinCfg(void)
    {
        Chip_GPIO_SetPinPFSEG(5, 2, 1);//pin52
        Chip_GPIO_SetPinDIRInput(5, 2);
    };
    
    const InterruptID gs_Int1SID = 
    {
        1,
        SYS_INT1_IDX,
        FALLING_EDGE_INT,
        0,
        0,
        Int1Enable,
        Int1Disable,
        Int1Priority,
        NULL,//RENSE_GPIOINT,
        Int1_PinCfg,
    };
    
    void Int1_IRQHandler(void)
    {
        CPSR_ALLOC();
        RHINO_CPU_INTRPT_DISABLE();
        Intx_Handler(&gs_Int1SID);
        RHINO_CPU_INTRPT_ENABLE();    
    }
#else
    void Int1_IRQHandler(void)
    {
        CPSR_ALLOC();
        RHINO_CPU_INTRPT_DISABLE();
        
        RHINO_CPU_INTRPT_ENABLE();    
    }
#endif

#if  SYS_INT2_EN > 0
    void Int2Enable(void)
    {
        Chip_INT0_Set_Enable((uint64_t)1<<RENSE_INT_PRI_PPR2);
    }
    
    void Int2Disable(void)
    {
        Chip_INT0_Set_Disable( RENSE_INT_PRI_PPR2); /* disable INT0 interrupt */
        Chip_INT0_Clear_Flag( RENSE_INT_PRI_PPR2);/* clear INT0 interrupt flag */


    }
    void Int2Priority(void)
    {
        Chip_INT_Set_PRI(RENSE_INT_PRI_PPR2, 0x03);
    }
    
    void Int2_PinCfg(void)
    {
        Chip_GPIO_SetPinPFSEG(5, 3, 1);//pin53
        Chip_GPIO_SetPinDIRInput(5, 3);

    };
    
    const InterruptID gs_Int2SID = 
    {
        2,
            SYS_INT2_IDX,
        FALLING_EDGE_INT,
        0,
        0,
        Int2Enable,
        Int2Disable,
        Int2Priority,
        NULL,//RENSE_GPIOINT,
        Int2_PinCfg,
    };
    
    void Int2_IRQHandler(void)
    {
        CPSR_ALLOC();
        RHINO_CPU_INTRPT_DISABLE();
        Intx_Handler(&gs_Int2SID);
        RHINO_CPU_INTRPT_ENABLE();    
    }
#else
    void Int2_IRQHandler(void)
    {
        CPSR_ALLOC();
        RHINO_CPU_INTRPT_DISABLE();
        
        RHINO_CPU_INTRPT_ENABLE();    
    }
#endif

#if  SYS_INT3_EN > 0
    void Int3Enable(void)
    {
        Chip_INT0_Set_Enable(RENSE_INT_PRI_PPR3);
    }
    
    void Int3Disable(void)
    {
        Chip_INT0_Set_Disable(RENSE_INT_PRI_PPR3); /* disable INT0 interrupt */
        Chip_INT0_Clear_Flag(RENSE_INT_PRI_PPR3);/* clear INT0 interrupt flag */


    }
    void Int3Priority(void)
    {
        Chip_INT_Set_PRI(RENSE_INT_PRI_PPR3, 0x03);
    }
    
    void Int3_PinCfg(void)
    {
        Chip_GPIO_SetPinPFSEG(3, 1, 1);//pin31
        Chip_GPIO_SetPinDIRInput(3, 1);

    };
    
    const InterruptID gs_Int3SID = 
    {
        3,
            SYS_INT3_IDX,
        FALLING_EDGE_INT,
        0,
        0,
        Int3Enable,
        Int3Disable,
        Int3Priority,
        NULL,//RENSE_GPIOINT,
        Int3_PinCfg,
    };
    
    void Int3_IRQHandler(void)
    {
        CPSR_ALLOC();
        RHINO_CPU_INTRPT_DISABLE();
        Intx_Handler(&gs_Int3SID);
        RHINO_CPU_INTRPT_ENABLE();    
    }
#else
    void Int3_IRQHandler(void)
    {
        CPSR_ALLOC();
        RHINO_CPU_INTRPT_DISABLE();
        
        RHINO_CPU_INTRPT_ENABLE();    
    }
#endif

#if  SYS_INT4_EN > 0
    void Int4Enable(void)
    {
        Chip_INT0_Set_Enable(RENSE_INT_PRI_PPR4);
    }
    
    void Int4Disable(void)
    {
        Chip_INT0_Set_Disable(RENSE_INT_PRI_PPR4); /* disable INT0 interrupt */
        Chip_INT0_Clear_Flag(RENSE_INT_PRI_PPR4);/* clear INT0 interrupt flag */


    }
    void Int4Priority(void)
    {
        Chip_INT_Set_PRI(RENSE_INT_PRI_PPR4, 0x03);
    }
    
    void Int4_PinCfg(void)
    {
        Chip_GPIO_SetPinPFSEG(3, 3, 1);//pin33
        Chip_GPIO_SetPinDIRInput(3, 3);

    };
    
    const InterruptID gs_Int4SID = 
    {
        4,
            SYS_INT4_IDX,
        FALLING_EDGE_INT,
        0,
        0,
        Int4Enable,
        Int4Disable,
        Int4Priority,
        NULL,//RENSE_GPIOINT,
        Int4_PinCfg,
    };
    
    void Int4_IRQHandler(void)
    {
        CPSR_ALLOC();
        RHINO_CPU_INTRPT_DISABLE();
        Intx_Handler(&gs_Int4SID);
        RHINO_CPU_INTRPT_ENABLE();    
    }
#else
    void Int4_IRQHandler(void)
    {
        CPSR_ALLOC();
        RHINO_CPU_INTRPT_DISABLE();
        
        RHINO_CPU_INTRPT_ENABLE();    
    }
#endif

#if  SYS_INT5_EN > 0
    void Int5Enable(void)
    {
        Chip_INT0_Set_Enable(RENSE_INT_PRI_PPR5);
    }
    
    void Int5Disable(void)
    {
        Chip_INT0_Set_Disable(RENSE_INT_PRI_PPR5); /* disable INT0 interrupt */
        Chip_INT0_Clear_Flag(RENSE_INT_PRI_PPR5);/* clear INT0 interrupt flag */


    }
    void Int5Priority(void)
    {
        Chip_INT_Set_PRI(RENSE_INT_PRI_PPR5, 0x03);
    }
    
    void Int5_PinCfg(void)
    {
        Chip_GPIO_SetPinPFSEG(0, 1, 1);//pin01
        Chip_GPIO_SetPinDIRInput(0, 1);

    };
    
    const InterruptID gs_Int5SID = 
    {
        5,
            SYS_INT5_IDX,
        FALLING_EDGE_INT,
        0,
        0,
        Int5Enable,
        Int5Disable,
        Int5Priority,
        NULL,//RENSE_GPIOINT,
        Int5_PinCfg,
    };
    
    void Int5_IRQHandler(void)
    {
        CPSR_ALLOC();
        RHINO_CPU_INTRPT_DISABLE();
        Intx_Handler(&gs_Int5SID);
        RHINO_CPU_INTRPT_ENABLE();    
    }
#else
    void Int5_IRQHandler(void)
    {
        CPSR_ALLOC();
        RHINO_CPU_INTRPT_DISABLE();
        
        RHINO_CPU_INTRPT_ENABLE();    
    }
#endif

#if  SYS_INT6_EN > 0
    void Int6Enable(void)
    {
        Chip_INT0_Set_Enable(RENSE_INT_PRI_PMK6);
    }
    
    void Int6Disable(void)
    {
        Chip_INT0_Set_Disable( RENSE_INT_PRI_PMK6); /* disable INT0 interrupt */
        Chip_INT0_Clear_Flag( RENSE_INT_PRI_PMK6);/* clear INT0 interrupt flag */


    }
    void Int6Priority(void)
    {
        Chip_INT_Set_PRI(RENSE_INT_PRI_PMK6, 0x03);
    }
    
    void Int6_PinCfg(void)
    {
        Chip_GPIO_SetPinPFSEG(5, 7, 1);//pin57
        Chip_GPIO_SetPinDIRInput(5, 7);

    };
    
    const InterruptID gs_Int6SID = 
    {
        6,
        SYS_INT6_IDX,
        FALLING_EDGE_INT,
        1,
        1,
        Int6Enable,
        Int6Disable,
        Int6Priority,
        NULL,//RENSE_GPIOINT,
        Int6_PinCfg,
    };
    
    void Int6_IRQHandler(void)
    {
        CPSR_ALLOC();
        RHINO_CPU_INTRPT_DISABLE();
        Intx_Handler(&gs_Int6SID);
        RHINO_CPU_INTRPT_ENABLE();    
    }
#else
    void Int6_IRQHandler(void)
    {
        CPSR_ALLOC();
        RHINO_CPU_INTRPT_DISABLE();
        
        RHINO_CPU_INTRPT_ENABLE();
    }
#endif

#if  SYS_INT7_EN > 0
    void Int7Enable(void)
    {
        Chip_INT0_Set_Enable(RENSE_INT_PRI_PMK7);
    }
    
    void Int7Disable(void)
    {
        Chip_INT0_Set_Disable(RENSE_INT_PRI_PMK7); /* disable INT0 interrupt */
        Chip_INT0_Clear_Flag(RENSE_INT_PRI_PMK7);/* clear INT0 interrupt flag */


    }
    void Int7Priority(void)
    {
        Chip_INT_Set_PRI(RENSE_INT_PRI_PMK7, 0x03);
    }
    
    void Int7_PinCfg(void)
    {
        Chip_GPIO_SetPinPFSEG(0, 2, 1);//pin02
        Chip_GPIO_SetPinDIRInput(0, 2);

    };
    
    const InterruptID gs_Int7SID = 
    {
        7,
            SYS_INT7_IDX,
        FALLING_EDGE_INT,
        1,
        2,
        Int7Enable,
        Int7Disable,
        Int7Priority,
        NULL,//RENSE_GPIOINT,
        Int7_PinCfg,
    };
    
    void Int7_IRQHandler(void)
    {
        CPSR_ALLOC();
        RHINO_CPU_INTRPT_DISABLE();
        Intx_Handler(&gs_Int7SID);
        RHINO_CPU_INTRPT_ENABLE();
    }
#else
    void Int7_IRQHandler(void)
    {
        CPSR_ALLOC();
        RHINO_CPU_INTRPT_DISABLE();
        RHINO_CPU_INTRPT_ENABLE();
    }
#endif

/******************************************************************************
外部中断口配置的结构体集合
******************************************************************************/
const InterruptID* const gss_IntSID[NO_OF_INTERRUPT] = 
{
#if SYS_INT0_EN > 0
    &gs_Int0SID,
#endif

#if SYS_INT1_EN > 0
    &gs_Int1SID,
#endif

#if SYS_INT2_EN > 0
    &gs_Int2SID,
#endif

#if SYS_INT3_EN > 0
    &gs_Int3SID,
#endif

#if SYS_INT4_EN > 0
    &gs_Int4SID,
#endif

#if SYS_INT5_EN > 0
    &gs_Int5SID,
#endif

#if SYS_INT6_EN > 0
    &gs_Int6SID,
#endif

#if SYS_INT7_EN > 0
    &gs_Int7SID,
#endif
};
//	#endif                                  //#ifdef _USR_MAIN
#endif

