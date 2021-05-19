#ifndef _B_YM_H     
#define _B_YM_H 

#include "stdint.h"

#define PACKET_SEQNO_INDEX      (1)
#define PACKET_SEQNO_COMP_INDEX (2)

#define PACKET_HEADER           (3)
#define PACKET_TRAILER          (2)
#define PACKET_OVERHEAD         (PACKET_HEADER + PACKET_TRAILER)
#define PACKET_SIZE             (128)
#define PACKET_1K_SIZE          (1024)

#define SOH                     (0x01)  
#define STX                     (0x02)  
#define EOT                     (0x04) 
#define ACK                     (0x06)  
#define NAK                     (0x15) 
#define CA                      (0x18)  
#define CRC16                   (0x43)  

#define ABORT1                  (0x41) 
#define ABORT2                  (0x61)  

#define NAK_TIMEOUT             (0x100)//(0x010000)
#define MAX_ERRORS              (3)

#define FILE_NAME_LENGTH        (256)
#define FILE_SIZE_LENGTH        (16)

extern int32_t Ymodem_Receive(uint8_t *buf); 
extern uint32_t FLASH_PagesMask(volatile uint32_t Size);
extern void Int2Str(uint8_t* str, int32_t intnum);
extern uint32_t Str2Int(uint8_t *inputstr, int32_t *intnum);
extern uint16_t  Crc_HX(uint8_t * buffer, uint16_t len,uint16_t startposition);

#endif
