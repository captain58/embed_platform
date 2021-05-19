#ifndef _B_FLASH_H     
#define _B_FLASH_H 

#include "stdint.h"

extern uint32_t GetInFlashData(uint32_t addr);
extern void EarseAllFlash(void);
extern uint8_t WriteInFlash(uint32_t Address, uint32_t DataAddr);

#endif
