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

#ifndef  __ARCHIVES_H
#define  __ARCHIVES_H

#include "includes.h"
#include "user_func.h"


uint8 SS_Hash_Correct(uint32 dst, uint8* src, uint8 type);
void SS_Hash_Transfer(uint8 type);
uint8 Updata_SS_Hash_Table(uint16 hash, WB_SPACE *elem, uint8 type) ;
void Search_Validate_SS(uint8 *data);
int SS_Hash_Table_Insert(WB_SPACE* elem, uint8 type); //Ìí¼Ó¹þÏ£±í
void Add_Validate_W_B_List(uint8* pData);
void Add_Validate_W_B(uint8* pAddr, uint8 len, uint8 type);
void Del_Validate_W_B(uint8* pData, uint8 addrLen);
void Del_Validate_W_B_List(uint8* pData);
uint8 SS_Sn_Check(uint8* sn_addr);
uint8 Check_Black(uint8* sn_addr, uint8 len);
int  Hash_Table_Search(uint8 * amaddr);
int  Hash_Table_Search1(uint8 * amaddr, uint8 len);
int  Hash_Table_Insert(HASHT *elem, int* pReturnHash);
int  Hash_Table_Insert1(HASHT *elem, int* pReturnHash);
//	void Updata_Hash_Table1(uint16 hash, HASHT *elem);
void Init_Hash_Table(void);
void Updata_Hash_Table(uint16 hash , HASHT *elem);
void Hash_Transfer(void);
void Hash_Correct(uint32 dst,uint8* src );
uint8 read_archive(uint8 * buf, uint32 addr, uint16 len);
uint8 Meter_Check1(uint8 cmd, uint8 * panid, uint8* addr, uint8 addrLen, uint16 * pos);
uint8 GetRealArchivesAddr(uint16 id, uint8 * addr, uint8 * len);
uint8 DevCheck(uint8 * addr, uint8 addrLen, uint8* ppanid);
int Hash_Table_Search_By_Compressed(uint8 *addr, uint8 len);

#endif
