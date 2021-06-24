#ifndef _MY_7108_
#define _MY_7108_

//	#include "main.h"
/*********************************************************************
**  function Declaration
*********************************************************************/

void  INIT_MCU_RF_MAP(void);

void SYS_A7139_Proc(uint8_t mod);
uint8_t SYS_RF_Init(void);
uint8_t SYS_A7139_Send(uint8_t * data, uint16_t len);
uint16_t SYS_A7139_Recv(uint8_t * data);

#endif
