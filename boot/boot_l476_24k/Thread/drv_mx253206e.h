#ifndef __MX25L32_H
#define __MX25L32_H

#include "stdint.h"
#include "stm32l4xx_hal.h"

#define DLYTIME 5000

#define Flash_WriteEnable(a)    (a)?(HAL_GPIO_WritePin(GPIOD,GPIO_PIN_8, GPIO_PIN_RESET)):(HAL_GPIO_WritePin(GPIOD,GPIO_PIN_8, GPIO_PIN_SET))
#define Flash_Select(a)         (a)?(HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12, GPIO_PIN_RESET)):(HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12, GPIO_PIN_SET))
#define FLASH_CLK(a)            (a)?(HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13, GPIO_PIN_SET)):(HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13, GPIO_PIN_RESET))
#define FLASH_MOSI(a)           (a)?(HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15, GPIO_PIN_SET)):(HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15, GPIO_PIN_RESET))

extern uint8_t ReadFlashData(uint32_t addr,uint16_t Len,uint8_t *value);
extern uint8_t WriteFlashData(uint32_t addr,uint16_t Len,uint8_t *value);
extern void InitialExFlashIO(void);

#endif
