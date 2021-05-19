#ifndef _AIRTHMETIC_H     
#define _AIRTHMETIC_H

#include "stdint.h"

extern uint16_t Crc_Change(uint8_t *buf,uint16_t trans_length,uint16_t start,uint16_t crc);
extern void DelaySuiyi(uint32_t time);

#endif
