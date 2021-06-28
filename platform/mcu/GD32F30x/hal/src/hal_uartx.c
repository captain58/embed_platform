/******************************Copyright(c)******************************
**                         
**
** File Name: hal_uartx.c
** Author: 
** Date Last Update: 2019-11-02
** Description: uart驱动模块
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: 文件创建
** 依赖模块:hal_hwtimer.c用来提供RS485收发切换定时器
*************************************************************************/
#define EXT_HAL_UART

#include "sys.h"
#include "hal.h"
#include <string.h>
/************************************************************************
 * @串口数量(目前最多可支持5,即0-4)
 ************************************************************************/
#define   UART_MAX_RX    6         //编号0-(UART_MAX_RX-1)


/************************************************************************
 *定义串口使用的cashwtimer定时器编号
 ************************************************************************/
#define   ID_CASHWTIMR_UARTROT(X)      (X << 1)
#define   ID_CASHWTIMR_UARTTRC(X)      ((X << 1) + 1)


/************************************************************************
 * @存放初始化时输入的串口配置
 ************************************************************************/
const SerialID*  gps_uartInfoList[UART_MAX_RX];



/************************************************************************
 * @为了是UART0与UART1-4实现统一调用而设置的常量数组
 ************************************************************************/
//以下成员实际类型包含: LPC_USART0_T*,LPC_USARTN_T*
//	const void* _LpcUSART[UART_MAX_RX] = {USART1, USART1, UART2, UART3, UART4, UART5};
//	
//	const uint8 _IRQnType[UART_MAX_RX] = {UART0_IRQn, UART1_IRQn, UART2_IRQn, UART3_IRQn, UART4_IRQn, UART5_IRQn};
//	
//	const uint32_t _UartClk[UART_MAX_RX] = {UART0CLK, UART1CLK, UART2CLK, UART3CLK, UART4CLK, UART5CLK};

//const void* _LpcUSART[] = {USART1, USART2};

//const uint8 _IRQnType[] = {USART1_IRQn, USART2_IRQn};









/************************************************************************
 * @Function: Uartx_TRChange
 * @Description: 485数据发送完毕之后需要延时一段时间,
 *              直到移位寄存器空再进行发送态到接收态的转换
 * 
 * @Arguments: 
 * @param: serialID 
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/25
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void Uartx_TRChange(void* serialID)
{
    const SerialID* sid = serialID;
    uint8 flag = 0;
//    void *pUART = (void*)_LpcUSART[sid->uart_no];
    
    //发送寄存器都已经空了

//    if(RESET == UARTx_TXBUFSTA_TXFF_Chk(pUART))
//    {
//        flag = 1;
//    }
    
//    if(flag)
    {
        if(sid->rs485)
        {                                   //默认为接收状态
            ((void(*)())sid->rs485->setModeRecv)();
        }
                                            //停止定时器
        casHwTimerStop(ID_CASHWTIMR_UARTTRC(sid->uart_no));
    }
    
}



/************************************************************************
 * @Function: Uartx_ROverTime
 * @Description: 接收超时结束处理函数
 * 
 * @Arguments: 
 * @param: serialID 
 * 
 * @Note: 
 * @Return:   
 * @Auther: yzy
 * Date: 2015/5/25
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void Uartx_ROverTime(void* serialID)
{
    const SerialID* sid = serialID;
    
    //设置接收标记位
    *(sid->recvFlag) = 1;          //数据帧接收完毕标记
    
    casHwTimerStop(ID_CASHWTIMR_UARTROT(sid->uart_no));
#if (SYS_UARTSEM_EN > 0)
    SYS_SEM_Release((TESRes*)sid->uart_rx_sem);
#endif

}




#if (SYS_UART_ACCT > 0)
/************************************************************************
 * @function: UartACCTransmitProc
 * @描述: ACCT模式下Uart的发送处理函数
 * @参数: 
 * @param: sid 串口信息结构体
 * @返回:
 * @ret:  bool 真 处理完毕,假 未处理
 * @说明: 
 * @作者: yzy (2014/5/22)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
bool UartACCTransmitProc(const SerialID* sid)
{
    uint16 dd;
    SerialBuffer* gsp_Uartx = sid->buffer;
    void* pUART = (void*)_LpcUSART[sid->uart_no];
    
    if(gsp_Uartx->acc_tlen > 0)         //存在加速缓存
    {
        //1)加速缓存未满,转移到加速缓存中
        while((gsp_Uartx->tcnt > 0) && 
               (gsp_Uartx->acc_tcnt != gsp_Uartx->acc_tlen))
        {
            dd = gsp_Uartx->tbuff[gsp_Uartx->tp];
            gsp_Uartx->acc_tbuff[gsp_Uartx->acc_tp] = dd;
                                        //acc_buff卷绕复位
            gsp_Uartx->acc_tp++;
            if(gsp_Uartx->acc_tp >= gsp_Uartx->acc_tlen)
            {
                gsp_Uartx->acc_tp = 0;
            }
                                        //tbuff卷绕复位
            gsp_Uartx->tp++;
            if(gsp_Uartx->tp >= gsp_Uartx->tlen)
            {
                gsp_Uartx->tp = 0;
            }
                                        //更新acc_buff和tbuff中有效数据长度
            gsp_Uartx->acc_tcnt++;
            gsp_Uartx->tcnt--;
        }
        
        //2)优先处理发送加速缓存中的数据
        if(sid->uart_no == 0)
        {
            while((gsp_Uartx->acc_tcnt > 0) && 
                  (Chip_UART0_ReadLineStatus(LPC_USART0) & UART0_LSR_THRE))
            {                               //发送一个字节
                dd = gsp_Uartx->acc_tbuff[gsp_Uartx->acc_pt++];
                Chip_UART0_SendByte(LPC_USART0, dd);
            
                if(gsp_Uartx->acc_pt >= gsp_Uartx->acc_tlen)
                {                           //卷绕复位
                    gsp_Uartx->acc_pt = 0;
                }
                                            //所需要发送的数据减少
                gsp_Uartx->acc_tcnt--;
            }
        }
        else
        {
            
            while((gsp_Uartx->acc_tcnt > 0) && 
                (Chip_UARTN_GetStatus((LPC_USARTN_T*)pUART) & UARTN_STAT_TXRDY))
            {                               //发送一个字节
                dd = gsp_Uartx->acc_tbuff[gsp_Uartx->acc_pt++];
                Chip_UARTN_SendByte((LPC_USARTN_T*)pUART, dd);

                if(gsp_Uartx->acc_pt >= gsp_Uartx->acc_tlen)
                {                           //卷绕复位
                    gsp_Uartx->acc_pt = 0;
                }
                                            //所需要发送的数据减少
                gsp_Uartx->acc_tcnt--;
            }
        }
        
        //3)数据帧发送完毕
        if((gsp_Uartx->tcnt == 0) && (gsp_Uartx->acc_tcnt == 0))
        {
            if(sid->rs485)
            {                           //启动定时从而来判断是否已经发送完最后的字节
                casHwTimerStart(ID_CASHWTIMR_UARTTRC(sid->uart_no));
            }
        }
        return true;
    }
    return false;
}
#endif


/************************************************************************
 * @function: Uartx_Config
 * @描述: 配置串口的通讯参数
 * 
 * @参数: 
 * @param: ss 串口的配置
 * @param: sid 串口信息结构体
 * @返回: 
 * @说明: 这个函数是普通中断安全的,但不是进程安全的
 * @作者: yzy (2014/5/22)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
int Uartx_Config(SerialSets * ss, const SerialID* sid)
{
//    uint32 config = 0;
    
//  UART_HandleTypeDef *pstuarthandle = NULL;
    
    int32_t ret = -1;
    SYS_ENTER_SCRT();
	

    ((void(*)())sid->pinConfig)();          //端口功能选择为UART功能
    if(sid->rs485)
    {
        ((void(*)())sid->rs485->setModeInit)();
        ((void(*)())sid->rs485->setModeRecv)();//高收、低发,默认为接收状态
    }
//    pstuarthandle = sid->phandle;
//
//    pstuarthandle->Init.BaudRate               = ss->baudrate;
//    ret = uart_dataWidth_transform(ss->databits, &pstuarthandle->Init.WordLength);
//    ret |= uart_parity_transform(ss->parit, &pstuarthandle->Init.Parity);
//    ret |= uart_stop_bits_transform(ss->stopbits, &pstuarthandle->Init.StopBits);
//    ret |= uart_flow_control_transform(ss->flowctrl, &pstuarthandle->Init.HwFlowCtl);
//    ret |= uart_mode_transform(ss->mode, &pstuarthandle->Init.Mode);
//    if (ret) {
////        LOG_ERROR("invalid uart data \r\n");
////	        memset(pstuarthandle, 0, sizeof(*pstuarthandle));
////	        m_free(sid->phandle);
//        return -1;
//    } 
//    pstuarthandle->Instance = (USART_TypeDef*)sid->pUART;
//    pstuarthandle->Init.OverSampling = sid->overSampling;
//    pstuarthandle->Init.OneBitSampling         = sid->OneBitSampling;
//    pstuarthandle->AdvancedInit.AdvFeatureInit = sid->AdvFeatureInit;
//
//
//    /* init uart */
//    ret = HAL_UART_Init(pstuarthandle);
//    if (ret != HAL_OK) {
////          LOG_ERROR("uart %d init fail \r\n", uart->port);
////          aos_free(stm32_uart[uart->port].UartRxBuf);
////          stm32_uart[uart->port].UartRxBuf = NULL;
////	        m_free(sid->phandle);
//        return ret;
//    }
//
//    if(pstuarthandle->hdmarx ==NULL)
//        ret = uart_receive_start_it(sid,sid->recvBufLen);
//    else
//        ret = uart_receive_start_dma(sid,sid->recvBufLen);
//
//    if (ret) {
//    }
    
    rcu_periph_clock_enable(sid->clk);
    /* USART configure */
    usart_deinit(sid->pUART);
    usart_baudrate_set(sid->pUART, ss->baudrate);//115200U);
    usart_receive_config(sid->pUART, USART_RECEIVE_ENABLE);
    usart_transmit_config(sid->pUART, USART_TRANSMIT_ENABLE);
    usart_enable(sid->pUART);
    
    

    *(sid->inited) = 1;
    /* enable USART0 receive interrupt */
    usart_interrupt_enable(sid->pUART, USART_INT_RBNE);
    
    /* enable USART0 transmit interrupt */
    usart_interrupt_enable(sid->pUART, USART_INT_TC);
    
    nvic_irq_enable(sid->irqn, 0, 0);

    SYS_EXIT_SCRT();

    return ret;
}

int32_t Uartx_Init( SerialID* sid, SerialSets * ss)
{
    int32_t ret = -1;
//    UART_HandleTypeDef *pstuarthandle = NULL;
//	    UART_MAPPING* uartIns = NULL;

    SYS_VAR_CHECK(sid->uart_no >= UART_MAX_RX);
    ((void(*)())sid->pinConfig)();          //端口功能选择为UART功能

    //其他额外配置
    if(sid->rs485)
    {
        ((void(*)())sid->rs485->setModeInit)();
        ((void(*)())sid->rs485->setModeRecv)();//高收、低发,默认为接收状态
        casHwTimerCreate(Uartx_TRChange, (void*)sid, ID_CASHWTIMR_UARTTRC(sid->uart_no), 10);
    }
                                      //配置接收超时定时器(实时性要求不高,可以使用软件定时器)
    casHwTimerCreate(Uartx_ROverTime, (void*)sid, ID_CASHWTIMR_UARTROT(sid->uart_no), CalcUartRecvTimeOut(1200));
    casHwTimerConfig(0, 1);

    if (sid == NULL) {
        return -1;
    }
    //no found this port in function-physical uartIns, no need initialization
//	    uartIns = GetUARTMapping((PORT_UART_TYPE)uart->port);
//	    if( NULL== uartIns ){ 
//	        return -1;
//	    }
    sid->msp();
    
//	    sid->phandle = 
//	      (UART_HandleTypeDef *)m_malloc(sizeof(UART_HandleTypeDef));
    
//	    memset(&stm32_uart[uart->port],0,sizeof(stm32_uart_t));
    
    
    rcu_periph_clock_enable(sid->clk);
    /* USART configure */
    usart_deinit(sid->pUART);
    usart_baudrate_set(sid->pUART, ss->baudrate);//115200U);
    usart_receive_config(sid->pUART, USART_RECEIVE_ENABLE);
    usart_transmit_config(sid->pUART, USART_TRANSMIT_ENABLE);
    usart_enable(sid->pUART);
    
//    pstuarthandle = sid->phandle;
//    memset((uint8_t *)pstuarthandle,0,sizeof(UART_HandleTypeDef));
//
//    pstuarthandle->Init.BaudRate               = ss->baudrate;
//    ret = uart_dataWidth_transform(ss->databits, &pstuarthandle->Init.WordLength);
//    ret |= uart_parity_transform(ss->parit, &pstuarthandle->Init.Parity);
//    ret |= uart_stop_bits_transform(ss->stopbits, &pstuarthandle->Init.StopBits);
//    ret |= uart_flow_control_transform(ss->flowctrl, &pstuarthandle->Init.HwFlowCtl);
//    ret |= uart_mode_transform(ss->mode, &pstuarthandle->Init.Mode);
//    if (ret) {
////	        printf("invalid uart data \r\n");
//        memset(pstuarthandle, 0, sizeof(*pstuarthandle));
//        m_free(sid->phandle);
//        return -1;
//    }    
//
////	    if(NULL == stm32_uart[uart->port].UartRxBuf){
////	        stm32_uart[uart->port].UartRxBuf = aos_malloc(uartIns->attr.max_buf_bytes);
////	    }
////	    
////	    if (NULL == stm32_uart[uart->port].UartRxBuf) {
////	        printf("Fail to malloc memory size %d at %s %d \r\n", uartIns->attr.max_buf_bytes, __FILE__, __LINE__);
////	        return -1;
////	    }
////	    memset(stm32_uart[uart->port].UartRxBuf, 0, uartIns->attr.max_buf_bytes);
//
//    //uart->priv = pstuarthandle; //priv must not be used in uart driver
//
//    pstuarthandle->Instance = (USART_TypeDef*)sid->pUART;
//    pstuarthandle->Init.OverSampling = sid->overSampling;
//    pstuarthandle->Init.OneBitSampling         = sid->OneBitSampling;
//    pstuarthandle->AdvancedInit.AdvFeatureInit = sid->AdvFeatureInit;
//
//
//    /* init uart */
//    ret = HAL_UART_Init(pstuarthandle);
//    if (ret != HAL_OK) {
////          printf("uart %d init fail \r\n", uart->port);
////          aos_free(stm32_uart[uart->port].UartRxBuf);
////          stm32_uart[uart->port].UartRxBuf = NULL;
//        m_free(sid->phandle);
//        return ret;
//    }
//        
        

    
    
    //初始化信息结构体变量
    gps_uartInfoList[sid->uart_no] = sid;
    memset((uint8*)sid->buffer, 0, sizeof(SerialBuffer));
    sid->buffer->rlen = sid->recvBufLen;
    sid->buffer->rbuff = (uint8*)m_malloc(sid->recvBufLen);
    
//	#if (SYS_UART_ACCT > 0)
//	    sid->buffer->acc_tlen = sid->accBufLen;
//	    sid->buffer->acc_tbuff = (uint8*)m_malloc(sid->accBufLen);
//	#endif

#if (SYS_UARTSEM_EN > 0)
    SYS_SEM_Create(1, (TESRes*)sid->uart_mutex);
    SYS_SEM_Create(1, (TESRes*)sid->uart_rx_mutex);
    SYS_SEM_Create(1, (TESRes*)sid->uart_tx_mutex);

    SYS_SEM_Create(0, (TESRes*)sid->uart_rx_sem);
    SYS_SEM_Create(0, (TESRes*)sid->uart_tx_sem);
#endif

    
    /* if UART Rx DMA Handle is NULL, then start data receive in interrupt mode
     * otherwise in DMA mode
     */
//    if(pstuarthandle->hdmarx ==NULL)
//        ret = uart_receive_start_it(sid,sid->recvBufLen);
//    else
//        ret = uart_receive_start_dma(sid,sid->recvBufLen);
//
//    if (ret) {
////        m_free(sid->phandle);
//        m_free(sid->buffer->rbuff);
//    } 
//    else 
    {
        *(sid->inited) = 1;
        /* enable USART0 receive interrupt */
        usart_interrupt_enable(sid->pUART, USART_INT_RBNE);
        
        /* enable USART0 transmit interrupt */
        usart_interrupt_enable(sid->pUART, USART_INT_TC);
        
        nvic_irq_enable(sid->irqn, 0, 0);

    }
    return ret;
}

uint8 Uartx_DeInit(const SerialID* sid)
{
//    UARTx_Type *pUART = sid->pUART;
//
//    HAL_HalfDuplex_Init(sid->phandle);

    casHwTimerClose(0);
//    m_free(sid->phandle);
    m_free(sid->buffer->rbuff);
    sid->buffer->rbuff = NULL;
#if (SYS_UART_ACCT > 0)
    m_free(sid->buffer->acc_tbuff);
    sid->buffer->acc_tbuff = NULL;
#endif    
    SYS_OK();
}
uint8_t Uartx_WaitSendBufNull(const SerialID* sid)
{
//	    uint32_t flagstatus = (USART_REG_VAL2(sid->pUART, USART_INT_FLAG_TBE) & BIT(USART_BIT_POS2(USART_INT_FLAG_TBE)));
//	    while(RESET == usart_interrupt_flag_get(sid->pUART, USART_INT_FLAG_TBE)) ;
    while(SET == USART_REG_VAL2(sid->pUART, USART_INT_FLAG_TBE) & BIT(USART_BIT_POS2(USART_INT_FLAG_TBE))) ;
    
    return 0;
}

/************************************************************************
 * @Function: Uartx_StartSend
 * @Description: 启动Uart的发送
 * 
 * @Arguments: 
 * @param: sid 串口信息结构体
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/25
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void Uartx_StartSend(const SerialID* sid)
{ 
    uint16 dd;
    SerialBuffer* gsp_Uartx = sid->buffer;
    
    SYS_ENTER_SCRT();
    
    if(sid->rs485)
    {
        casHwTimerStop(ID_CASHWTIMR_UARTTRC(sid->uart_no));//停止定时器
        ((void(*)())sid->rs485->setModeSend)();//转到发送口的状态
    }
    
#if (SYS_UART_ACCT > 0)
    if(UartACCTransmitProc(sid))
    {
        SYS_EXIT_SCRT();
        return;
    }
#endif
                                        //处理发送缓存中的数据

    if(gsp_Uartx->tcnt > 0)
    {                                   //发送一个字节
        dd = gsp_Uartx->tbuff[gsp_Uartx->tp++];
//	        if(sid->uart_no == 0)
//	        {
//	            Chip_UART0_SendByte(LPC_USART0, dd);
//	            UARTx_TXREG_Write(UARTx, TestTxData[i]);
//	        }
//	        else
//	        {
//	            while((Chip_UARTN_GetStatus((UARTx_Type*)_LpcUSART[sid->uart_no]) & UARTN_STAT_TXRDY) == 0)
//	            {
//	                //do nothing
//	                //wait until data is moved from the transmit buffer to the transmit shift register
//	            }
//	            Chip_UARTN_SendByte((LPC_USARTN_T*)_LpcUSART[sid->uart_no], dd);
//	            Chip_UARTN_IntEnable((LPC_USARTN_T*)_LpcUSART[sid->uart_no], UARTN_INTEN_TXRDY);
//        while(SET == UARTx_TXBUFSTA_TXFF_Chk((UARTx_Type*)_LpcUSART[sid->uart_no]));	//等待发送完成
//
//        UART_UARTIF_RxTxIF_ClrEx((UARTx_Type*)_LpcUSART[sid->uart_no]);	//清除发送中断标志
//    	UART_UARTIE_RxTxIE_SetableEx((UARTx_Type*)_LpcUSART[sid->uart_no], TxInt, ENABLE);//打开发送中断
//
//        UARTx_TXREG_Write((UARTx_Type*)_LpcUSART[sid->uart_no], dd); //发送第一个数据启动发送
        usart_data_transmit(sid->pUART, dd);
//	    }
        
        if(gsp_Uartx->tp >= gsp_Uartx->tcnt)
        {                               //卷绕复位
            gsp_Uartx->tp = 0;
        }
        gsp_Uartx->tcnt--;              //所需要发送的数据减少
    }
    
    SYS_EXIT_SCRT();
}
/**
  * @brief  Rx IDLE callbacks.
  * @param  huart pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  */

//void HAL_UART_IdleCallback(const SerialID* sid)//(UART_HandleTypeDef *huart)
//{
//    uint32_t left_byte;
//    SerialBuffer * gsp_Uartx = sid->buffer;
//
////	    const PORT_UART_TYPE appPort = GetAppPortFromPhyInstanse(huart->Instance);
////	    UART_MAPPING* uartIns = GetUARTMapping(appPort);
//
//    if (sid == NULL) {
//        return;
//    }
//
//    if (gsp_Uartx->uart_dma_stop) {
//        if (gsp_Uartx->uart_error_count < 0xffffffff) {
//            gsp_Uartx->uart_error_count++;
//        }
//    } else {
//        left_byte = __HAL_DMA_GET_COUNTER(sid->phandle->hdmarx);
//        /* if left_byte=0, means DMA transfer complete interrupt maybe has happened
//         * uart_rx_in will be update in DMA TC interrupt
//         * don't do it repeatedly 
//         */
//        if(left_byte < gsp_Uartx->previous_dma_leftbyte && left_byte!=0)
//        {
//            gsp_Uartx->rp += gsp_Uartx->previous_dma_leftbyte -left_byte;
////	            stm32_uart[appPort].uart_rx_in += stm32_uart[appPort].previous_dma_leftbyte -left_byte;
////	            if(stm32_uart[appPort].uart_rx_in == uartIns->attr.max_buf_bytes)
//            if(gsp_Uartx->rp == gsp_Uartx->rlen) gsp_Uartx->rp = 0;
////	                stm32_uart[appPort].uart_rx_in =0;
//            gsp_Uartx->previous_dma_leftbyte = left_byte;
////	            if(stm32_uart[appPort].uart_rx_in == stm32_uart[appPort].uart_rx_out)
////	                stm32_uart[appPort].RxBuf_is_full =1;
//            if(gsp_Uartx->rp == gsp_Uartx->rcnt)
//                gsp_Uartx->RxBuf_is_full =1;
//
//        }
//    }
////	    aos_sem_signal(&stm32_uart[appPort].uart_rx_sem);  
//
//}
//
//static void UartIdleHandler(const SerialID* sid)//( const USART_TypeDef* ins)
//{
//    uint32_t isrflags = 0;
//    uint32_t icrflags = 0;
//    UART_HandleTypeDef* huart_handle;
////	    const PORT_UART_TYPE appPort = GetAppPortFromPhyInstanse(ins);
//
//    huart_handle = sid->phandle;//&(stm32_uart[appPort].hal_uart_handle);
//
//    isrflags = READ_REG(huart_handle->Instance->ISR);
//
//    if(isrflags & USART_ISR_IDLE)
//    {
//        //clear IDLE bit
//        SET_BIT(huart_handle->Instance->ICR, USART_ICR_IDLECF_Msk);
//
//        HAL_UART_IdleCallback(sid);
//    }
//
//}
//
//
//static void UartIRQProcessor(const SerialID* sid)//(const USART_TypeDef* ins )
//{
////	    const PORT_UART_TYPE appPort = GetAppPortFromPhyInstanse(ins);
//    uint32_t isrflags   = READ_REG(sid->pUART->ISR);
//    uint32_t cr1its     = READ_REG(sid->pUART->CR1);
//    //deal with IDLE interrupt, HAL_UART_IRQHandler doesn't do it , we don't want to change HAL_UART_IRQHandler
//    if(((isrflags & USART_ISR_IDLE) != RESET) && ((cr1its & USART_CR1_IDLEIE) != RESET))
//        UartIdleHandler(sid);
//
//    if( sid!=NULL ) 
//    {
//        HAL_UART_IRQHandler(sid->phandle);  
//    }
//
//}

/************************************************************************
 * @Function: UartN_Handler
 * @Description: 中断处理(包含接收\发送及状态变更)
 * 
 * @Arguments: 
 * @param: sid 
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/25
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void UartN_Handler(const SerialID* sid)
{
    volatile uint8 dd;
    uint8 rflag = 0;
    
    SerialBuffer* gsp_Uartx = sid->buffer;
//    UART_TypeDef *pUART = (UART_TypeDef*)sid->pUART;//_LpcUSART[sid->uart_no];
    
    

	//发送中断处理
//		if((ENABLE == UART_UARTIE_RxTxIE_GetableEx(pUART, TxInt))
//			&&(SET == UART_UARTIF_RxTxIF_ChkEx(pUART, TxInt)))
//		{
//			//发送中断标志可通过写txreg寄存器清除或txif写1清除
//			//发送指定长度的数据
//			if(UARTxOp[0].TxOpc < UARTxOp[0].TxLen)
//			{
//				UARTx_TXREG_Write(UART0, UARTxOp[0].TxBuf[UARTxOp[0].TxOpc]); //发送一个数据
//				UARTxOp[0].TxOpc++;
//			}
//			UART_UARTIF_RxTxIF_ClrEx(UART0);	//清除发送中断标志
//		}
//	
//	    if(true)
//  	if(pUART->INTSTS & UART_INTSTS_TXDONEIF)
    if(RESET != usart_interrupt_flag_get(sid->pUART, USART_INT_FLAG_TC))
    {
        /*
        TXRDY:
        Transmitter Ready flag. 
        When 1, this bit indicates that data may be written to the transmit buffer. 
        Previous data may still be in the process of being transmitted.
        Cleared when data is written to TXDAT. 
        Set when the data is moved from the transmit buffer to the transmit shift register.
        */
//        pUART->INTSTS |= UART_INTSTS_TXDONEIF;
//        gsp_Uartx->sflag = 0x55;
//	        while((gsp_Uartx->tcnt > 0) && 
//	               (pUART->INTSTS & UART_INTSTS_TXDONEIF))
        usart_interrupt_flag_clear(sid->pUART, USART_INT_FLAG_TC);
        if(gsp_Uartx->tcnt > 0)
        {                                   //发送一个字节
        
//            pUART->INTSTS |= UART_INTSTS_TXDONEIF;
           
            if(gsp_Uartx->tp < gsp_Uartx->tlen)
            {
                dd = gsp_Uartx->tbuff[gsp_Uartx->tp++];
//	                UARTx_TXREG_Write(pUART, dd);
//                pUART->DATA = dd;
                usart_data_transmit(sid->pUART, dd);
            }
            
            if(gsp_Uartx->tp >= gsp_Uartx->tlen)
            {                               //卷绕复位
                gsp_Uartx->tp = 0;
            }
                                            //所需要发送的数据减少
            gsp_Uartx->tcnt--;
        }
        if(gsp_Uartx->tcnt == 0)
        {
            if(sid->rs485 > 0)
            {                           //启动定时从而来判断是否已经发送完最后的字节
                casHwTimerStart(ID_CASHWTIMR_UARTTRC(sid->uart_no));
            }
//	            Chip_UARTN_IntDisable(pUART, UARTN_INTEN_TXRDY);
        }
//	        pUART->INTSTS |= UART_INTSTS_TXDONEIF;
                                            //数据帧发送完毕
	}

    //接收处理
	//接收中断处理
//		if((ENABLE == UART_UARTIE_RxTxIE_GetableEx(UART0, RxInt))
//			&&(SET == UART_UARTIF_RxTxIF_ChkEx(UART0, RxInt)))
//		{
//			//中断转发接收到的数据
//			tmp08 = UARTx_RXREG_Read(UART0);//接收中断标志仅可通过读取rxreg寄存器清除
//			UARTx_TXREG_Write(UART0, tmp08);
//		}    
//	    if((ENABLE == UART_UARTIE_RxTxIE_GetableEx(pUART, RxInt))
//	            &&(SET == UART_UARTIF_RxTxIF_ChkEx(pUART, RxInt)))
            
//  	if(pUART->INTSTS&UART_INTSTS_RXIF)
    if(RESET != usart_interrupt_flag_get(sid->pUART, USART_INT_FLAG_RBNE))
    {                                   //注意:intstatus和status不一样
    
        
//        do
        {
            rflag = 1;
            
//            pUART->INTSTS |= UART_INTSTS_RXIF;
            dd = (uint8_t)usart_data_receive(sid->pUART);//pUART->DATA;//UARTx_RXREG_Read(pUART);
            if((gsp_Uartx->rlen == 0)/* || (gsp_Uartx->rcnt >= gsp_Uartx->rlen)*/)
            {
                //do nothing
            }
            else
            {
//                guc_count++;
                                            //否则开始接收数据
                gsp_Uartx->rbuff[gsp_Uartx->rp++] = dd;


                                            //判断是否已经超出缓存
                if(gsp_Uartx->rp >= gsp_Uartx->rlen)
                {
                    gsp_Uartx->rp = 0;      //卷绕复位
                }
                
                if(gsp_Uartx->rcnt < gsp_Uartx->rlen)
                {
                    gsp_Uartx->rcnt++;          //数量+1
                }
                else
                {
                    gsp_Uartx->pr = gsp_Uartx->rp;//读指针也要卷绕
                }

            }
//            casHwTimerStart(ID_CASHWTIMR_UARTROT(sid->uart_no));
        }//while(pUART->INTSTS&UART_INTSTS_RXIF);
                                            //启动定时从而来判断是否接收完一帧
        if(rflag)
        {
            casHwTimerStart(ID_CASHWTIMR_UARTROT(sid->uart_no));
        }
    }

}



/*!
    \brief      this function handles USART0 exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void USART0_IRQHandler(void)
{
    krhino_intrpt_enter();
    if((gps_uartInfoList[0] != __NULL)
       && (gps_uartInfoList[0]->uart_no == 0))
    {
        UartN_Handler(gps_uartInfoList[0]);
    }
    krhino_intrpt_exit();
    
//    if(RESET != usart_interrupt_flag_get(USART0, USART_INT_FLAG_RBNE)){
//        /* read one byte from the receive data register */
//        rx_buffer[rx_counter++] = (uint8_t)usart_data_receive(USART0);
//
//        if(rx_counter >= nbr_data_to_read)
//        {
//            /* disable the USART0 receive interrupt */
//            usart_interrupt_disable(USART0, USART_INT_RBNE);
//        }
//    }
//       
//    if(RESET != usart_interrupt_flag_get(USART0, USART_INT_FLAG_TBE)){
//        /* write one byte to the transmit data register */
//        usart_data_transmit(USART0, tx_buffer[tx_counter++]);
//
//        if(tx_counter >= nbr_data_to_send)
//        {
//            /* disable the USART0 transmit interrupt */
//            usart_interrupt_disable(USART0, USART_INT_TBE);
//        }
//    }
}

//void USART1_IRQHandler(void)
//{
//    //引入gps_uartInfoList是为了隔离，防止hal驱动直接引用外部 gs_Uart1SID。
//    SYS_LockMMTK();
//    if((gps_uartInfoList[1] != __NULL)
//       && (gps_uartInfoList[1]->uart_no == 1))
//    {
//        UartIRQProcessor(gps_uartInfoList[1]);
//    }
//       
//    SYS_UnLockMMTK();
//}
void USART2_IRQHandler(void)
{
    //引入gps_uartInfoList是为了隔离，防止hal驱动直接引用外部 gs_Uart1SID。
    krhino_intrpt_enter();
    if((gps_uartInfoList[2] != __NULL)
       && (gps_uartInfoList[2]->uart_no == 2))
    {
        UartN_Handler(gps_uartInfoList[2]);
//        UartIRQProcessor(gps_uartInfoList[2]);
    }
       
    krhino_intrpt_exit();
}



