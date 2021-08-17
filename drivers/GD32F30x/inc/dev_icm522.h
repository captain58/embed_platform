/*
 * Copyright (C) 2015-2017 Alibaba Group Holding Limited
 */

#ifndef _BLE_LSB4BT_CONFIG_MODULE
#define _BLE_LSB4BT_CONFIG_MODULE

#include "aos/hal/uart.h"



void lsb4bt_modemon(void);
void lsb4bt_modemoff(void);
int lsb4bt_modemcheck(void);
int lsb4bt_setname(uint8_t *name, uint8_t len);






#endif
