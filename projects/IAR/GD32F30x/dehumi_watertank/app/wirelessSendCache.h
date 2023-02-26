/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright HEX NET
 *
 *    File name   : archives.h
 *    Description : real meter map; white meter; black meter.
 *
 *    History :
 *    1. Date        : oct 12, 2016
 *       Author      : yzy
 *       Description : Create
 *
 **************************************************************************/

#ifndef  __WIRELESS_CACHE_H
#define  __WIRELESS_CACHE_H

//#include "includes.h"
//#include "user_func.h"

#define CON_SEND_PRIORITY_HIGH      0
#define CON_SEND_PRIORITY_NORMAL      1
#define CON_SEND_PRIORITY_LOW      2
#define CON_SEND_PRIORITY_HIGH_NUM      2
#define CON_SEND_PRIORITY_NORMAL_NUM      2
#define CON_SEND_PRIORITY_LOW_NUM      2
void sendCacheInit(void);
EZ_TX_PKT * getIdelSendCacheByPri(uint8 level);
EZ_TX_PKT * getIdelSendCache(uint8 level);
EZ_TX_PKT * getReadySendCache(uint8 * level);
EZ_TX_PKT * getIdelSendCacheCanbeSleep(uint8 level, int32 ms);
uint8 checkDuplicateWithPri(uint8 level, uint16 id);
uint8 checkSendCache(void);
void decTTL(uint8 msr);
#endif
