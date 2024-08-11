/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright HEX NET
 *
 *    File name   : LinkMng.h
 *    Description : real meter map; white meter; black meter.
 *
 *    History :
 *    1. Date        : oct 12, 2016
 *       Author      : yzy
 *       Description : Create
 *
 **************************************************************************/

#ifndef  __LINKMNG_H
#define  __LINKMNG_H

//#include    "LPC_data_type.h"
#include    "App_Public.h"

uint8 Fill_Route_INFO(uint8 * data, RINFO route ,uint8 id );
ROUTE  Fill_Route(uint8 * data,uint8 type,uint16 id );
ROUTE Fill_Route2(uint8 *data, uint8 type,  uint8 protocol, uint8 addrType, uint16 id);

void UpdataRouteInfo(uint16 SourceID, uint8* pRouteData, uint8 RouteNum);
void UpdataRouteInfo1(uint16 SourceID, PKT *pkt);
void UpdataRouteInfo2(uint16 SourceID, uint8 protocol, uint8 addrFlg, uint8* pAddr, uint8 RouteNum);



#endif
