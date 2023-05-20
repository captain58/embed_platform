/*
 * Copyright (C) 2015-2017 Captain Group Holding Limited
 */

#ifndef __HAL_UART_STM32L4_H
#define __HAL_UART_STM32L4_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32l4xx_hal.h"  
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "aos/kernel.h"

#include "board.h"

#include "aos/include.h"

extern uint8_t gucs_UartRevFlag[NO_OF_SERIAL];

extern uint8_t gucs_UartRevStart[NO_OF_SERIAL];
extern uint8_t gucs_UartRevByteTimeout[NO_OF_SERIAL];

typedef struct{    
    uint32_t       overSampling;
    uint32_t       OneBitSampling;
    uint32_t       AdvFeatureInit;
    uint32_t       max_buf_bytes;  //the size of UartRxBuf used by driver
}uartAttribute;


typedef struct{
    PORT_UART_TYPE uartFuncP;
    void*          uartPhyP; 
    uartAttribute  attr;
}UART_MAPPING;

//Mapping Table is defined in soc_init.c
extern UART_MAPPING UART_MAPPING_TABLE[PORT_UART_SIZE];
int32_t hal_uart_reset(uart_dev_t *uart, uart_config_t * ss);
int32_t hal_uart_close(uart_dev_t *uart);

int32_t hal_uart_open(PORT_UART_TYPE uart_port);

//	int32_t hal_uart_shut(PORT_UART_TYPE uart_port);

    /*==================================================================
    函数名：  __Uart_Find
    作者：    杨州宇
    日期：    2018-1-24
    功能：    查找串口
    输入参数：1、ucId：串口ID
    输出参数：无
    返回值：  成功返回串口数据指针，失败返回NULL
    =================================================================*/
SerialID * __Uart_Find(unsigned char ucId);

#ifdef __cplusplus
}
#endif

#endif /* __HAL_UART_STM32L4_H */
