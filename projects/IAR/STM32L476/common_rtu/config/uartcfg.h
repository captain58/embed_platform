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

//	    void Uart0SendEnable(void)
//	    {
//	        STIF0 = 0U;    /* clear INTST0 interrupt flag */
//	        STMK0 = 0U;    /* enable INTST0 interrupt */
//	        SO0 |= _0001_SAU_CH0_DATA_OUTPUT_1;    /* output level normal */
//	        SOE0 |= _0001_SAU_CH0_OUTPUT_ENABLE;    /* enable UART0 output */
//	        SS0 |= _0002_SAU_CH1_START_TRG_ON | _0001_SAU_CH0_START_TRG_ON;    /* enable UART0 receive and transmit */
//	        Chip_INT0_Clear_Flag(gs_Uart1SID.usart_Int_Snd);//STIF1 = 0U;    /* clear INTST1 interrupt flag */
//	        Chip_INT0_Set_Enable(gs_Uart1SID.usart_Int_Snd);//STMK1 = 0U;    /* enable INTST1 interrupt */
//	        Chip_UART_Set_SO_Output(gs_Uart1SID.ch, gs_Uart1SID.USARTx, 0x01<<((gs_Uart1SID.ch & 0x01)*2));//_0001_SAU_CH0_DATA_OUTPUT_1);//SO0 |= _0004_SAU_CH2_DATA_OUTPUT_1;    /* output level normal */
//	        Chip_UART_Set_SOE_Output_Enable(gs_Uart1SID.ch, gs_Uart1SID.USARTx, 0x01<<((gs_Uart1SID.ch & 0x01)*2));//_0001_SAU_CH0_OUTPUT_ENABLE);//SOE0 |= _0004_SAU_CH2_OUTPUT_ENABLE;    /* enable UART1 output */
//	        Chip_UART_Set_SS_Output_Start(gs_Uart1SID.ch, gs_Uart1SID.USARTx);//SS0 |= _0008_SAU_CH3_START_TRG_ON | _0004_SAU_CH2_START_TRG_ON;    /* enable UART1 receive and transmit */
//	        
//	    }
//	    
//	    void Uart0SendDisable(void)
//	    {
//	        ST0 |= _0001_SAU_CH0_STOP_TRG_ON;    /* disable UART0 transmit */
//	        SOE0 &= ~_0001_SAU_CH0_OUTPUT_ENABLE;    /* disable UART0 output */
//	        STMK0 = 1U;    /* disable INTST0 interrupt */
//	        STIF0 = 0U;    /* clear INTST0 interrupt flag */
//	        
//	    }
//	    
//	    void Uart0ReceiveEnable(void)
//	    {
//	        SRIF0 = 0U;    /* clear INTSR0 interrupt flag */
//	        //SRMK0 = 0U;    /* enable INTSR0 interrupt */
//	        Chip_INT0_Set_Enable(RENSE_INT_PRI_SRPR0);
//	        SO0 |= _0001_SAU_CH0_DATA_OUTPUT_1;    /* output level normal */
//	        SOE0 |= _0001_SAU_CH0_OUTPUT_ENABLE;    /* enable UART0 output */
//	        SS0 |= _0002_SAU_CH1_START_TRG_ON | _0001_SAU_CH0_START_TRG_ON;    /* enable UART0 receive and transmit */
//	
//	
//	
//	    }
//	    
//	    void Uart0ReceiveDisable(void)
//	    {
//	        ST0 |= _0002_SAU_CH1_STOP_TRG_ON;    /* disable UART0 receive */
//	        SRMK0 = 1U;    /* disable INTSR0 interrupt */
//	        SRIF0 = 0U;    /* clear INTSR0 interrupt flag */
//	    }
//	   
    void Uart0_PinCfg(void)
    {
//	        SAU0EN = 1U;    /* supply SAU0 clock */
//          Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 0, (IOCON_FUNC1 | IOCON_MODE_PULLUP));
//          Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 1, (IOCON_FUNC1 | IOCON_MODE_PULLUP));
        /* Set RxD0 pin */
//	        PFSEG5 &= 0xF7U;
//	        PM1 |= 0x80U;
//	        //PFSEG3 &= 0xEFU;  //端口第二功能
//	        //PM4 |= 0x80U;     //端口第二功能
//	        /* Set TxD0 pin */
//	        PFSEG5 &= 0xEFU;
//	        P0 |= 0x01U;
//	        PM0 &= 0xFEU;
        Chip_Clock_EnablePeriphClock(1<< RENSE_CLOCK_PLL_SAU0);

        Chip_GPIO_SetPinPFSEG(0,0,1);
        Chip_GPIO_SetPinPFSEG(1,7,1);
        Chip_GPIO_SetPinDIROutput(0,0);
        Chip_GPIO_SetPinDIRInput(1,7);
        Chip_GPIO_SetPinState(0,0,1);
    
    };
    void Uart0Send(uint8 data)
    {
//	        TXD0 = data;
        RENSE_UART_SDR0(0) = (RENSE_UART_SDR0(0) & 0xff00) | data;
        
    }
    void Uart0Rcv(uint8 * data)
    {
        *data = RENSE_UART_SDR0(1)&0xFF;
    }
    const SerialID gs_Uart0SID = 
    {
        0,
        SYS_UART0_IDX,
        UART0_485_EN,
        RENSE_INT_PRI_CSIPR0_IICPR0_STPR0,
        0x03,
        RENSE_INT_PRI_SRPR0,
        0x03,
        RENSE_INT_PRI_SREPR0_TMPR01H,
        Uart0_RS485_SetModeRcv,
        Uart0_RS485_SetModeSnd,
        Uart0_RS485_SetModeInit,
        Uart0Send,
        Uart0Rcv,
        
        0xFF,
        //
        NULL,//RENSE_UART,
        NULL,//RENSE_UART0_SDR,
//	            UART1_IRQn,
//	            PRI_UART1,
        Uart0_PinCfg,
        &gs_Uart0Buffer,
        &gucs_UartRevFlag[SYS_UART0_IDX],
        &gucs_UartRevByteTimeout[SYS_UART0_IDX],
        &gucs_UartRevStart[SYS_UART0_IDX],
        
        guc_Uart0BufRcv,
        guc_Uart0BufSnd,
        LEN_OF_RECV0,
    };
    
    void UART0_IRQHandler_Send(void)
    {
        CPSR_ALLOC();
        RHINO_CPU_INTRPT_DISABLE();
        RHINO_CPU_INTRPT_ENABLE();
    
        Uartx_Handler_Send(&gs_Uart0SID);
    }

    void UART0_IRQHandler_Recv(void)
    {
        CPSR_ALLOC();
        RHINO_CPU_INTRPT_DISABLE();
        RHINO_CPU_INTRPT_ENABLE();
    
        Uartx_Handler_Rcv(&gs_Uart0SID);
    }
        
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


//	#endif                                  //#ifdef _USR_MAIN




#endif










