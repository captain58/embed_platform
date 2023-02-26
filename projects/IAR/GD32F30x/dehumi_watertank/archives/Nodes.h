/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright HEX NET
 *
 *    File name   : nodes.h
 *    Description : real meter map; white meter; black meter.
 *
 *    History :
 *    1. Date        : oct 12, 2016
 *       Author      : yzy
 *       Description : Create
 *
 **************************************************************************/

#ifndef  __NODES_H
#define  __NODES_H

//#include "includes.h"
#include "user_func.h"

uint8 GetCltorPara(uint16 id, STMETERPARAFLASH * pstMeterParaF);
uint8 SaveCltorPara(uint16 id, STMETERPARAFLASH * pstMeterParaF);
uint8 SyncCltorPara(uint16 id, COLLECTOR * pCltor, STMETERPARAFLASH *pstMeter);
void RecoverCltorPara(void);
uint16 Sn_Search_Id(uint8* sn_addr);//根据SN查询ID
uint16 Sn_Search_Id2(uint8* sn_addr, uint8 len);//根据SN查询ID
uint16 Re_Allocate_Id(uint8* sn_addr, uint8 len);//重新分配ID
uint8 getSnNum(void);
uint8 getSnLoginedNum(void);
void Delete_SS_Node(uint16 id);
uint8 updataNodeCache(uint16 id, uint8 type, uint8 errCode, uint8 seq, uint8 protocol, 
                        uint8 rssi, uint8 * addr, uint8 len, STMETERPARAFLASH *pstMeter);
uint16 getOrAllocateIdByAddr(uint8 cmd, uint8 * addr, uint8 len);
uint16 findIdByAddr(uint8 cmd, uint8 * addr, uint8 len);
uint8 getNetworkStatus(void);
#endif

