#ifndef _PARAMETER_H     
#define _PARAMETER_H

#include "stdint.h"
#include "B_YM.h"

typedef enum 
{
    FROM_NONE,
    FROM_UART,
    FROM_FLASH
}IAP_FROM_FLAG;

typedef enum 
{
    UART_APPLICATION_FLAG_NONE,
    UART_APPLICATION_FLAG_RUN,
    UART_APPLICATION_FLAG_UPDATA,
    UART_APPLICATION_FLAG_RESET,
}UART_APPLICATION_FLAG;

typedef  void (*pFunction)(void);

extern uint8_t prv_ucUartxRxBuf[8];
extern uint16_t prv_ucUartxRxLen;
extern uint8_t file_name[FILE_NAME_LENGTH];

#define IS_AF(c)        ((c >= 'A') && (c <= 'F'))
#define IS_af(c)        ((c >= 'a') && (c <= 'f'))
#define IS_09(c)        ((c >= '0') && (c <= '9'))

#define ISVALIDHEX(c)  IS_AF(c) || IS_af(c) || IS_09(c)
#define ISVALIDDEC(c)  IS_09(c)
#define CONVERTDEC(c)  (c - '0')

#define CONVERTHEX_alpha(c)  (IS_AF(c) ? (c - 'A'+10) : (c - 'a'+10))
#define CONVERTHEX(c)   (IS_09(c) ? (c - '0') : CONVERTHEX_alpha(c)) 

#define ApplicationAddress          0x8006000
#define PAGE_SIZE                   0x800
#define FLASH_ALL_SIZE              0x100000
#define FLASH_MAX_FILE_SIZE         FLASH_ALL_SIZE - (ApplicationAddress - 0x08000000)
#define EX_FLASH_SP 0x1000

extern uint8_t ReadFlash28data[28];
extern uint8_t flashreadbuf[4096];
extern uint8_t flashwritebuf[4096];

#endif
