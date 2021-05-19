#ifndef _FROM_UART_H     
#define _FROM_UART_H

#include "stdint.h"

extern void From_Uart_Task(void);
extern void Start_Menu(void);
extern void UartIapStartTask(void);
extern void Enter_App_Call(uint8_t flag);

#endif
