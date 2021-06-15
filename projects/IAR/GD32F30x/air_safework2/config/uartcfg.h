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
#ifndef _UARTCFG_H_
#define _UARTCFG_H_

#ifndef EXT_UART
#define EXT_UART    extern
#endif








//	#ifdef _USR_MAIN                        //应用层主文件中定义

/******************************************************************************
各串口的配置
******************************************************************************/
#if  SYS_UART0_EN > 0
    SerialBuffer gs_Uart0Buffer;
    unsigned char guc_Uart0BufRcv[LEN_OF_RECV0];
    unsigned char guc_Uart0BufSnd[LEN_OF_SND0];
    
    void Uart0_RS485_SetModeInit(void)
    {

    };
    
    void Uart0_RS485_SetModeSnd(void)
    {
        
    };
    
    void Uart0_RS485_SetModeRcv(void)
    {
        
    };


    void Uart0_PinCfg(void)
    {
       
        /* enable GPIO clock */
        rcu_periph_clock_enable(RCU_GPIOA);
        
        gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
        /* connect port to USARTx_Rx */
        gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
    
    };
        void Uart0_Msp(void)
    {
//        __HAL_RCC_USART1_CLK_ENABLE();
    }
//    void Uart0Send(uint8 data)
//    {
////	        TXD0 = data;
//        RENSE_UART_SDR0(0) = (RENSE_UART_SDR0(0) & 0xff00) | data;
//        
//    }
//    void Uart0Rcv(uint8 * data)
//    {
//        *data = RENSE_UART_SDR0(1)&0xFF;
//    }
    const SerialID gs_Uart0SID = 
    {
        0,
        PRI_UART0,
        USART0_IRQn,
        &gucs_UartInited[SYS_UART0_IDX],
        USART0,
        RCU_USART0,
        Uart0_PinCfg,
        Uart0_Msp,
        NULL,//(SerialTRC *)&_rs485_usart2,
        &gs_Uart0Buffer,
        &gucs_UartRevFlag[SYS_UART0_IDX],
        LEN_OF_RECV0,  
//        &gs_Uart1Handle,
//        &gs_Uart1Mutex,
//        &gs_Uart1RxMutex,
//        &gs_Uart1TxMutex,
//        &gs_Uart1RxSem,
//        &gs_Uart1TxSem,
//        UART_OVERSAMPLING_16,
//        UART_ONE_BIT_SAMPLE_DISABLE,
//        UART_ADVFEATURE_NO_INIT,
    };
    
//    void UART0_IRQHandler_Send(void)
//    {
//        CPSR_ALLOC();
//        RHINO_CPU_INTRPT_DISABLE();
//        RHINO_CPU_INTRPT_ENABLE();
//    
//        Uartx_Handler_Send(&gs_Uart0SID);
//    }
//
//    void UART0_IRQHandler_Recv(void)
//    {
//        CPSR_ALLOC();
//        RHINO_CPU_INTRPT_DISABLE();
//        RHINO_CPU_INTRPT_ENABLE();
//    
//        Uartx_Handler_Rcv(&gs_Uart0SID);
//    }
        
#else


#endif

#if  SYS_UART1_EN > 0
    const uart_dev_t gst_Uart1Dev = 
    {
        PORT_UART_STD,
        {9600,DATA_WIDTH_8BIT,NO_PARITY,STOP_BITS_1,FLOW_CONTROL_DISABLED,MODE_TX_RX},
        NULL,
    };

    const SerialID gs_Uart1SID = 
    {
        (uart_dev_t *)&gst_Uart1Dev,
        SYS_UART1_IDX,
        LPUART1,
        &gucs_UartRevFlag[SYS_UART1_IDX],
        &gucs_UartRevByteTimeout[SYS_UART1_IDX],
        &gucs_UartRevStart[SYS_UART1_IDX],
    };

#else


#endif

    
    
    
#if  SYS_UART2_EN > 0
    const uart_dev_t gst_Uart2Dev = 
    {
        NETP_PORT_NO,
        {9600,DATA_WIDTH_8BIT,NO_PARITY,STOP_BITS_1,FLOW_CONTROL_DISABLED,MODE_TX_RX},
        NULL,
    };

    const SerialID gs_Uart2SID = 
    {
        (uart_dev_t *)&gst_Uart2Dev,
        SYS_UART2_IDX,
        USART1,
        &gucs_UartRevFlag[SYS_UART2_IDX],
        &gucs_UartRevByteTimeout[SYS_UART2_IDX],
        &gucs_UartRevStart[SYS_UART2_IDX],
    };

#else


#endif



    
#if  SYS_UART3_EN > 0
    const uart_dev_t gst_Uart3Dev = 
    {
        NETP2_PORT_NO,
        {9600,DATA_WIDTH_8BIT,NO_PARITY,STOP_BITS_1,FLOW_CONTROL_DISABLED,MODE_TX_RX},
        NULL,
    };

    const SerialID gs_Uart3SID = 
    {
        (uart_dev_t *)&gst_Uart3Dev,
        SYS_UART3_IDX,
        USART3,
        &gucs_UartRevFlag[SYS_UART3_IDX],
        &gucs_UartRevByteTimeout[SYS_UART3_IDX],
        &gucs_UartRevStart[SYS_UART3_IDX],
    };

#else


#endif
    
    


    
#if  SYS_UART4_EN > 0

const uart_dev_t gst_Uart4Dev = 
{
    NETP3_PORT_NO,
    {9600,DATA_WIDTH_8BIT,NO_PARITY,STOP_BITS_1,FLOW_CONTROL_DISABLED,MODE_TX_RX},
    NULL,
};

const SerialID gs_Uart4SID = 
{
    (uart_dev_t *)&gst_Uart4Dev,
    SYS_UART4_IDX,
    USART2,
    &gucs_UartRevFlag[SYS_UART4_IDX],
    &gucs_UartRevByteTimeout[SYS_UART4_IDX],
    &gucs_UartRevStart[SYS_UART4_IDX],
};

#else
  

#endif
    
    
#if  SYS_UART5_EN > 0

const uart_dev_t gst_Uart5Dev = 
{
    NETP4_PORT_NO,
    {9600,DATA_WIDTH_8BIT,NO_PARITY,STOP_BITS_1,FLOW_CONTROL_DISABLED,MODE_TX_RX},
    NULL,
};

const SerialID gs_Uart5SID = 
{
    (uart_dev_t *)&gst_Uart5Dev,
    SYS_UART5_IDX,
    UART5,
    &gucs_UartRevFlag[SYS_UART5_IDX],
    &gucs_UartRevByteTimeout[SYS_UART5_IDX],
    &gucs_UartRevStart[SYS_UART5_IDX],
};

#else
  

#endif
    
    
    
    
    
    
    

/******************************************************************************
串口配置的结构体集合
******************************************************************************/
const SerialID* const gss_UartSID[NO_OF_SERIAL] = 
{
#if SYS_UART0_EN > 0
    &gs_Uart0SID,
#endif

#if SYS_UART1_EN > 0
    &gs_Uart1SID,
#endif

#if SYS_UART2_EN > 0
    &gs_Uart2SID,
#endif

#if SYS_UART3_EN > 0
    &gs_Uart3SID,
#endif

#if SYS_UART4_EN > 0
    &gs_Uart4SID,
#endif

#if SYS_UART5_EN > 0
    &gs_Uart5SID,
#endif
    
#if SYS_UART9_EN > 0
    &gs_Uart9SID,
#endif
    
#if SYS_UART10_EN > 0
    &gs_Uart10SID,
#endif

};

const uint8_t _ucPortMap[] = {SYS_UART0_IDX,SYS_UART1_IDX,SYS_UART0_IDX};

//	#endif                                  //#ifdef _USR_MAIN




#endif










