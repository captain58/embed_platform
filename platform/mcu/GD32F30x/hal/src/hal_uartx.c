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

static int32_t uart_send_it(const SerialID* sid, const void *data, uint32_t size,uint32_t timeout)
{
    HAL_StatusTypeDef sendRlt = HAL_BUSY;

    SYS_SEM_Wait((TESRes*)sid->uart_tx_mutex, 0);

    sendRlt = HAL_UART_Transmit_IT(sid->phandle, (uint8_t *)data, size);

    SYS_SEM_Wait((TESRes*)sid->uart_tx_sem, timeout);
    SYS_SEM_Release((TESRes*)sid->uart_tx_mutex);

    return (sendRlt==HAL_OK)?0:-1;
}

static int32_t uart_send_dma(const SerialID* sid, const void *data, uint32_t size,uint32_t timeout)
{
    HAL_StatusTypeDef sendRlt = HAL_BUSY;
    
    SYS_SEM_Wait((TESRes*)&sid->uart_tx_mutex, 0);

    sendRlt = HAL_UART_Transmit_DMA(sid->phandle, (uint8_t *)data, size);

    /*wait for the end of transfer*/
    SYS_SEM_Wait((TESRes*)sid->uart_tx_sem, timeout);
    SYS_SEM_Release((TESRes*)sid->uart_tx_mutex);


    return (sendRlt==HAL_OK)?0:-1;
}

int32_t hal_uart_send(const SerialID* sid, const void *data, uint32_t size, uint32_t timeout)
{
    int32_t ret = -1;
    UART_HandleTypeDef *handle = NULL;
    
    if ((sid == NULL) || (data == NULL)) {
        return -1;
    }    

    handle = sid->phandle;//uart_get_handle(uart->port);
    if (handle == NULL) {
        return -1;
    }
    if(*(sid->inited)!=1)
    {
        return -1;
    }
    if(sid->rs485)
    {
        casHwTimerStop(ID_CASHWTIMR_UARTTRC(sid->uart_no));//停止定时器
        ((void(*)())sid->rs485->setModeSend)();//转到发送口的状态
    }

    /* if  UART Tx DMA Handle is NULL, then start data send in interrupt mode
     * otherwise in DMA mode
     */
    if(handle->hdmatx ==NULL)
        ret = uart_send_it(sid,data, size, timeout);
    else
        ret = uart_send_dma(sid,data, size, timeout);

    return ret;
}
extern const SerialID* const gss_UartSID[NO_OF_SERIAL];

static SerialID *  GetAppPortFromPhyInstanse(const void* uartIns)
{
    SerialID * rc = NULL;
    int8_t i = 0;
    for(i = 0; i<NO_OF_SERIAL; i++)
    {
        if( (USART_TypeDef*)gss_UartSID[i]->pUART == (USART_TypeDef*)uartIns)
        {
            rc = (SerialID *)gss_UartSID[i];
            break;
        }
    }
    return rc;
}

static void UART_DMA_RxCpltCallback(SerialID * ids)//PORT_UART_TYPE appPort, uint32_t max_buffer_size)
{
    UART_HandleTypeDef huart = *ids->phandle;//stm32_uart[appPort].hal_uart_handle;
    SerialBuffer * gsp_uart = ids->buffer;
    uint32_t max_buffer_size = gsp_uart->rlen;
    //switch DMA Destination to another buffer region
    if(huart.hdmarx->Instance->CMAR == (uint32_t)&gsp_uart->rbuff[0])//(uint32_t)&stm32_uart[appPort].UartRxBuf[0])
    {
        if(gsp_uart->rcnt < max_buffer_size/2)
        {
            HAL_UART_Receive_DMA(&huart,(uint8_t*)&gsp_uart->rbuff[max_buffer_size/2],max_buffer_size/2);
            gsp_uart->uart_dma_stop =0;
            gsp_uart->previous_dma_leftbyte = max_buffer_size/2;
        } else {
            gsp_uart->uart_dma_stop =1;
        }
        gsp_uart->rp = (uint16_t)max_buffer_size/2;
        if(gsp_uart->rp == gsp_uart->rcnt)
            gsp_uart->RxBuf_is_full =1;
    } else {
        if(gsp_uart->rcnt >= max_buffer_size/2)
        {
            HAL_UART_Receive_DMA(&huart,(uint8_t*)&gsp_uart->rbuff[0],max_buffer_size/2);
            gsp_uart->uart_dma_stop =0;
            gsp_uart->rp =0;
            gsp_uart->previous_dma_leftbyte = max_buffer_size/2;
        } else {
            gsp_uart->uart_dma_stop =1;
            gsp_uart->rp = 0;//in case uart_rx_in = uart_rx_out, should take in account of "RxBuf_is_full" to identify if buffer is full
            if(gsp_uart->rp == gsp_uart->rcnt)
                gsp_uart->RxBuf_is_full =1;
        }
    }
}


static void UART_IT_RxCpltCallback(SerialID * sid)
{
    SerialBuffer * gsp_Uartx = sid->buffer;
    //TODO:
    //should take care to avoid UART_RX_IN get accross UART_RX_OUT, which will loss unread data
    //if receive buffer is too small, or application read data out of receive buffer at very low frequence
    //this situation may happen
    if(++gsp_Uartx->rp >= gsp_Uartx->rlen)//卷绕复位
        gsp_Uartx->rp = 0;
    gsp_Uartx->rcnt++;          //数量+1
                                //判断是否已经超出缓存
//	    if(gsp_Uartx->rp >= gsp_Uartx->rlen)
//	    {
//	        gsp_Uartx->rp = 0;      //卷绕复位
//	    }
//	    if(rflag)
    {
        casHwTimerStart(ID_CASHWTIMR_UARTROT(sid->uart_no));
    }

    HAL_UART_Receive_IT(sid->phandle, &gsp_Uartx->rbuff[gsp_Uartx->rp], 1);

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    const SerialID * sid = GetAppPortFromPhyInstanse(huart->Instance);
//	    UART_MAPPING* uartIns = GetUARTMapping(appPort);
//    SerialBuffer*  gsp_Uartx = sid->buffer;

    //check if it is called from UART_DMAReceiveCplt
    if(sid->phandle->hdmarx != NULL)
        UART_DMA_RxCpltCallback((SerialID *)sid);//uartIns->attr.max_buf_bytes);
    else
        UART_IT_RxCpltCallback((SerialID *)sid);

//	    SerialID * ids = __Uart_Find(appPort);
//	    
//	    if( NULL != ids ){ 
//	        *(ids->uart_RecvStart) = 1;
//	        *(ids->uart_ByteTimeout) = 0;
//	    }
//	    aos_sem_signal(&stm32_uart[appPort].uart_rx_sem);
}


/**
  * @brief  Tx Transfer completed callback
  * @param  huart: UART handle. 
  * @note   This example shows a simple way to report end of DMA Tx transfer, and 
  *         you can add your own implementation. 
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    const SerialID * sid = GetAppPortFromPhyInstanse(huart->Instance);
    if(sid->rs485 > 0)
    {                           //启动定时从而来判断是否已经发送完最后的字节
        casHwTimerStart(ID_CASHWTIMR_UARTTRC(sid->uart_no));
    }

//	    aos_sem_signal(&stm32_uart[appPort].uart_tx_sem);
    if(0 == huart->TxXferCount)
        SYS_SEM_Release((TESRes*)sid->uart_tx_sem);
}



int32_t uart_dataWidth_transform(uint8_t data_width_hal,
        uint32_t *data_width_stm32l4)
{
    uint32_t data_width = 0;
    int32_t ret = 0;
#if defined(USART_CR1_M1)
    if(data_width_hal == DataBits_7bits)
    {
        data_width = UART_WORDLENGTH_7B;
    }
    else 
#endif      
    if(data_width_hal == DataBits_8bits)
    {
        data_width = UART_WORDLENGTH_8B;
    }
    else if(data_width_hal == DataBits_9bits)
    {
        data_width = UART_WORDLENGTH_9B;
    }
    else
    {
        ret = -1;
    }

    if(ret == 0)
    {
        *data_width_stm32l4 = data_width;
    }

    return ret;
}

int32_t uart_parity_transform(uint8_t parity_hal,
        uint32_t *parity_stm32l4)
{
    uint32_t parity = 0;
    int32_t ret = 0;

    if(parity_hal == Parit_N)
    {
        parity = UART_PARITY_NONE;
    }
    else if(parity_hal == Parit_O)
    {
        parity = UART_PARITY_ODD;
    }
    else if(parity_hal == Parit_E)
    {
        parity = UART_PARITY_EVEN;        
    }
    else
    {
        ret = -1;
    }

    if(ret == 0)
    {
        *parity_stm32l4 = parity;
    }

    return ret;
}

int32_t uart_stop_bits_transform(uint8_t stop_bits_hal,
        uint32_t *stop_bits_stm32l4)
{
    uint32_t stop_bits = 0;
    int32_t ret = 0;

    if(stop_bits_hal == StopBits_1)
    {
        stop_bits = UART_STOPBITS_1;
    }
    else if(stop_bits_hal == StopBits_2)
    {
        stop_bits = UART_STOPBITS_2;
    }
    else
    {
        ret = -1;
    }

    if(ret == 0)
    {
        *stop_bits_stm32l4 = stop_bits;
    }

    return ret;
}

int32_t uart_flow_control_transform(uint8_t flow_control_hal,
        uint32_t *flow_control_stm32l4)
{
    uint32_t flow_control = 0;
    int32_t ret = 0;

    if(flow_control_hal == FLOW_CONTROL_DISABLED)
    {
        flow_control = UART_HWCONTROL_NONE;
    }
    else if(flow_control_hal == FLOW_CONTROL_CTS)
    {
        flow_control = UART_HWCONTROL_CTS;
    }
    else if(flow_control_hal == FLOW_CONTROL_RTS)
    {
        flow_control = UART_HWCONTROL_RTS;
    }
    else if(flow_control_hal == FLOW_CONTROL_RTS)
    {
        flow_control = UART_HWCONTROL_RTS_CTS;
    }
    else
    {
        ret = -1;
    }

    if(ret == 0)
    {
        *flow_control_stm32l4 = flow_control;
    }

    return ret;
}
int32_t uart_mode_transform(uint8_t mode_hal, uint32_t *mode_stm32l4)
{
    uint32_t mode = 0;
    int32_t ret = 0;

    if(mode_hal == MODE_TX)
    {
        mode = UART_MODE_TX;
    }
    else if(mode_hal == MODE_RX)
    {
        mode = UART_MODE_RX;
    }
    else if(mode_hal == MODE_TX_RX)
    {
        mode = UART_MODE_TX_RX;
    }
    else
    {
        ret = -1;
    }

    if(ret == 0)
    {
        *mode_stm32l4 = mode;
    }

    return ret;
}

static int32_t uart_receive_start_it(const SerialID* sid, uint32_t max_buffer_size)
{
    UART_HandleTypeDef *pstuarthandle = NULL;

    pstuarthandle = sid->phandle;
    //HAL_UART_RxCpltCallback is called per 1 byte, to update uart_rx_in
    if(HAL_UART_Receive_IT(pstuarthandle,(uint8_t*)sid->buffer->rbuff,1)!= HAL_OK)
    {
        return -1;
    }

    return 0;
}

static int32_t uart_receive_start_dma(const SerialID* sid, uint32_t max_buffer_size)
{
    UART_HandleTypeDef *pstuarthandle = NULL;
    uint32_t temp_reg;

    pstuarthandle = sid->phandle;//&stm32_uart[uart_port].hal_uart_handle;
//	    printf("uart %d enter uart_receive_start_dma instance 0x%x", 
//	        uart_port, pstuarthandle->Instance);
    //enable IDLE interrupt
    if(HAL_UART_Receive_DMA(pstuarthandle,(uint8_t*)sid->buffer->rbuff,max_buffer_size/2)!= HAL_OK)
    {
        return -1;
    }

    temp_reg = READ_REG(pstuarthandle->Instance->CR1);
    temp_reg |= USART_CR1_IDLEIE;
    WRITE_REG(pstuarthandle->Instance->CR1, temp_reg);
//	    stm32_uart[uart_port].previous_dma_leftbyte = max_buffer_size/2;

    return 0;
}
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
    UART_HandleTypeDef *pstuarthandle = NULL;
    
    int32_t ret = -1;
    SYS_ENTER_SCRT();
	

    ((void(*)())sid->pinConfig)();          //端口功能选择为UART功能
    if(sid->rs485)
    {
        ((void(*)())sid->rs485->setModeInit)();
        ((void(*)())sid->rs485->setModeRecv)();//高收、低发,默认为接收状态
    }
    pstuarthandle = sid->phandle;

    pstuarthandle->Init.BaudRate               = ss->baudrate;
    ret = uart_dataWidth_transform(ss->databits, &pstuarthandle->Init.WordLength);
    ret |= uart_parity_transform(ss->parit, &pstuarthandle->Init.Parity);
    ret |= uart_stop_bits_transform(ss->stopbits, &pstuarthandle->Init.StopBits);
    ret |= uart_flow_control_transform(ss->flowctrl, &pstuarthandle->Init.HwFlowCtl);
    ret |= uart_mode_transform(ss->mode, &pstuarthandle->Init.Mode);
    if (ret) {
//        LOG_ERROR("invalid uart data \r\n");
//	        memset(pstuarthandle, 0, sizeof(*pstuarthandle));
//	        m_free(sid->phandle);
        return -1;
    } 
    pstuarthandle->Instance = (USART_TypeDef*)sid->pUART;
    pstuarthandle->Init.OverSampling = sid->overSampling;
    pstuarthandle->Init.OneBitSampling         = sid->OneBitSampling;
    pstuarthandle->AdvancedInit.AdvFeatureInit = sid->AdvFeatureInit;


    /* init uart */
    ret = HAL_UART_Init(pstuarthandle);
    if (ret != HAL_OK) {
//          LOG_ERROR("uart %d init fail \r\n", uart->port);
//          aos_free(stm32_uart[uart->port].UartRxBuf);
//          stm32_uart[uart->port].UartRxBuf = NULL;
//	        m_free(sid->phandle);
        return ret;
    }

    if(pstuarthandle->hdmarx ==NULL)
        ret = uart_receive_start_it(sid,sid->recvBufLen);
    else
        ret = uart_receive_start_dma(sid,sid->recvBufLen);

    if (ret) {
    }


    HAL_NVIC_DisableIRQ(sid->irqn);    
    HAL_NVIC_SetPriority(sid->irqn, sid->irqPri , 0);
    HAL_NVIC_EnableIRQ(sid->irqn);

    SYS_EXIT_SCRT();

    return ret;
}

int32_t Uartx_Init( SerialID* sid, SerialSets * ss)
{
    int32_t ret = -1;
    UART_HandleTypeDef *pstuarthandle = NULL;
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
    
    pstuarthandle = sid->phandle;
    memset((uint8_t *)pstuarthandle,0,sizeof(UART_HandleTypeDef));

    pstuarthandle->Init.BaudRate               = ss->baudrate;
    ret = uart_dataWidth_transform(ss->databits, &pstuarthandle->Init.WordLength);
    ret |= uart_parity_transform(ss->parit, &pstuarthandle->Init.Parity);
    ret |= uart_stop_bits_transform(ss->stopbits, &pstuarthandle->Init.StopBits);
    ret |= uart_flow_control_transform(ss->flowctrl, &pstuarthandle->Init.HwFlowCtl);
    ret |= uart_mode_transform(ss->mode, &pstuarthandle->Init.Mode);
    if (ret) {
//	        printf("invalid uart data \r\n");
        memset(pstuarthandle, 0, sizeof(*pstuarthandle));
        m_free(sid->phandle);
        return -1;
    }    

//	    if(NULL == stm32_uart[uart->port].UartRxBuf){
//	        stm32_uart[uart->port].UartRxBuf = aos_malloc(uartIns->attr.max_buf_bytes);
//	    }
//	    
//	    if (NULL == stm32_uart[uart->port].UartRxBuf) {
//	        printf("Fail to malloc memory size %d at %s %d \r\n", uartIns->attr.max_buf_bytes, __FILE__, __LINE__);
//	        return -1;
//	    }
//	    memset(stm32_uart[uart->port].UartRxBuf, 0, uartIns->attr.max_buf_bytes);

    //uart->priv = pstuarthandle; //priv must not be used in uart driver

    pstuarthandle->Instance = (USART_TypeDef*)sid->pUART;
    pstuarthandle->Init.OverSampling = sid->overSampling;
    pstuarthandle->Init.OneBitSampling         = sid->OneBitSampling;
    pstuarthandle->AdvancedInit.AdvFeatureInit = sid->AdvFeatureInit;


    /* init uart */
    ret = HAL_UART_Init(pstuarthandle);
    if (ret != HAL_OK) {
//          printf("uart %d init fail \r\n", uart->port);
//          aos_free(stm32_uart[uart->port].UartRxBuf);
//          stm32_uart[uart->port].UartRxBuf = NULL;
        m_free(sid->phandle);
        return ret;
    }
        
        

    
    
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
    if(pstuarthandle->hdmarx ==NULL)
        ret = uart_receive_start_it(sid,sid->recvBufLen);
    else
        ret = uart_receive_start_dma(sid,sid->recvBufLen);

    if (ret) {
        m_free(sid->phandle);
        m_free(sid->buffer->rbuff);
    } 
    else 
    {
        *(sid->inited) = 1;
        HAL_NVIC_DisableIRQ(sid->irqn);    
        HAL_NVIC_SetPriority(sid->irqn, sid->irqPri , 0);
        HAL_NVIC_EnableIRQ(sid->irqn);

    }
    return ret;
}

uint8 Uartx_DeInit(const SerialID* sid)
{
    UARTx_Type *pUART = sid->pUART;

    HAL_HalfDuplex_Init(sid->phandle);

    casHwTimerClose(0);
    m_free(sid->phandle);
    m_free(sid->buffer->rbuff);
    sid->buffer->rbuff = NULL;
#if (SYS_UART_ACCT > 0)
    m_free(sid->buffer->acc_tbuff);
    sid->buffer->acc_tbuff = NULL;
#endif    
    SYS_OK();
}

/**
  * @brief  Rx IDLE callbacks.
  * @param  huart pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  */

void HAL_UART_IdleCallback(const SerialID* sid)//(UART_HandleTypeDef *huart)
{
    uint32_t left_byte;
    SerialBuffer * gsp_Uartx = sid->buffer;

//	    const PORT_UART_TYPE appPort = GetAppPortFromPhyInstanse(huart->Instance);
//	    UART_MAPPING* uartIns = GetUARTMapping(appPort);

    if (sid == NULL) {
        return;
    }

    if (gsp_Uartx->uart_dma_stop) {
        if (gsp_Uartx->uart_error_count < 0xffffffff) {
            gsp_Uartx->uart_error_count++;
        }
    } else {
        left_byte = __HAL_DMA_GET_COUNTER(sid->phandle->hdmarx);
        /* if left_byte=0, means DMA transfer complete interrupt maybe has happened
         * uart_rx_in will be update in DMA TC interrupt
         * don't do it repeatedly 
         */
        if(left_byte < gsp_Uartx->previous_dma_leftbyte && left_byte!=0)
        {
            gsp_Uartx->rp += gsp_Uartx->previous_dma_leftbyte -left_byte;
//	            stm32_uart[appPort].uart_rx_in += stm32_uart[appPort].previous_dma_leftbyte -left_byte;
//	            if(stm32_uart[appPort].uart_rx_in == uartIns->attr.max_buf_bytes)
            if(gsp_Uartx->rp == gsp_Uartx->rlen) gsp_Uartx->rp = 0;
//	                stm32_uart[appPort].uart_rx_in =0;
            gsp_Uartx->previous_dma_leftbyte = left_byte;
//	            if(stm32_uart[appPort].uart_rx_in == stm32_uart[appPort].uart_rx_out)
//	                stm32_uart[appPort].RxBuf_is_full =1;
            if(gsp_Uartx->rp == gsp_Uartx->rcnt)
                gsp_Uartx->RxBuf_is_full =1;

        }
    }
//	    aos_sem_signal(&stm32_uart[appPort].uart_rx_sem);  

}

static void UartIdleHandler(const SerialID* sid)//( const USART_TypeDef* ins)
{
    uint32_t isrflags = 0;
    uint32_t icrflags = 0;
    UART_HandleTypeDef* huart_handle;
//	    const PORT_UART_TYPE appPort = GetAppPortFromPhyInstanse(ins);

    huart_handle = sid->phandle;//&(stm32_uart[appPort].hal_uart_handle);

    isrflags = READ_REG(huart_handle->Instance->ISR);

    if(isrflags & USART_ISR_IDLE)
    {
        //clear IDLE bit
        SET_BIT(huart_handle->Instance->ICR, USART_ICR_IDLECF_Msk);

        HAL_UART_IdleCallback(sid);
    }

}


static void UartIRQProcessor(const SerialID* sid)//(const USART_TypeDef* ins )
{
//	    const PORT_UART_TYPE appPort = GetAppPortFromPhyInstanse(ins);
    uint32_t isrflags   = READ_REG(sid->pUART->ISR);
    uint32_t cr1its     = READ_REG(sid->pUART->CR1);
    //deal with IDLE interrupt, HAL_UART_IRQHandler doesn't do it , we don't want to change HAL_UART_IRQHandler
    if(((isrflags & USART_ISR_IDLE) != RESET) && ((cr1its & USART_CR1_IDLEIE) != RESET))
        UartIdleHandler(sid);

    if( sid!=NULL ) 
    {
        HAL_UART_IRQHandler(sid->phandle);  
    }

}


void USART1_IRQHandler(void)
{
    //引入gps_uartInfoList是为了隔离，防止hal驱动直接引用外部 gs_Uart1SID。
    SYS_LockMMTK();
    if((gps_uartInfoList[1] != __NULL)
       && (gps_uartInfoList[1]->uart_no == 1))
    {
        UartIRQProcessor(gps_uartInfoList[1]);
    }
       
    SYS_UnLockMMTK();
}
void USART2_IRQHandler(void)
{
    //引入gps_uartInfoList是为了隔离，防止hal驱动直接引用外部 gs_Uart1SID。
    SYS_LockMMTK();
    if((gps_uartInfoList[2] != __NULL)
       && (gps_uartInfoList[2]->uart_no == 2))
    {
        UartIRQProcessor(gps_uartInfoList[2]);
    }
       
    SYS_UnLockMMTK();
}



