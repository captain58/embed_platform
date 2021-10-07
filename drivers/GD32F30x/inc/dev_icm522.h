/*
 * Copyright (C) 2015-2017 Alibaba Group Holding Limited
 */

#ifndef _RFID_ICM522_CONFIG_MODULE
#define _RFID_ICM522_CONFIG_MODULE

//	#include "aos/hal/uart.h"



void icm522_modemon(void);
void icm522_modemoff(void);
int icm522_modemcheck(void);
int icm522_setname(uint8_t *name, uint8_t len);
int icm522_getpagex(uint8_t pagex, uint8_t *data, uint8_t * len);





#endif
