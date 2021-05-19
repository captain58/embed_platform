#ifndef _B_UART_H     
#define _B_UART_H 

#include "stdint.h"

#define SerialPutString(x) Serial_PutString((uint8_t*)(x))

extern void Serial_PutString(uint8_t *s);
extern void vUartxRecData(void);

#endif
