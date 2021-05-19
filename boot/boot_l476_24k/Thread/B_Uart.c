#include "B_Uart.h"
#include "stm32l4xx_hal.h"
#include "Parameter.h"

extern UART_HandleTypeDef hlpuart1;

void Serial_PutString(uint8_t *s);
void vUartxRecData(void);

void Serial_PutString(uint8_t *s)
{
    while (*s != '\0')
    {
        HAL_UART_Transmit(&hlpuart1,s, 1, 2);
        s++;
    }
}

void vUartxRecData(void)
{
    while(1)
    {
        if(HAL_UART_Receive(&hlpuart1, &prv_ucUartxRxBuf[prv_ucUartxRxLen], 1, 2)==HAL_OK)
        break;
    }
}
