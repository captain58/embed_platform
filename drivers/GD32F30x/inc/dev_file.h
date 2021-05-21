/******************************Copyright(c)******************************
**                         
**
** File Name: dev_file.h
** Author: 
** Date Last Update: 2019-11-02
** Description: 
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: 文件创建
*************************************************************************/
#ifndef _DEV_FILE_H_
#define _DEV_FILE_H_

#ifndef EXT_DEV_FILE
#define EXT_DEV_FILE extern
#endif



/******************************************************************************
**flash总线资源
******************************************************************************/
//#define gs_FileRes  gs_SpiioRes
#define gs_FramRes  gs_SpiioRes


#define guc_FileUserTkid    guc_SpiioUserTkid
#define guc_FramUserTkid    guc_SpiioUserTkid



/****************************************************************************** 
**定义数据内容的操作结构体
**其它进程发送消息时赋值给content
******************************************************************************/
typedef struct
{
    Object obj;                         //要操作的内容
    uint32 addr;                        //要操作的地址
    uint32 len;                         //操作的长度
    uint8  type;                        //类型
    uint8  rw;                          //读写方式
    //TResult re;                         //返回的结果
    uint8 __a;
}TDevFlsOP;


enum
{
    DEV_FLS_RW_R = 0,       //读
    DEV_FLS_RW_W = 1,       //写
    DEV_FLS_RW_S = 2,       //设置一样的数据
    DEV_FLS_RW_E = 3,       //擦除
    DEV_FLS_RW_WS = 4,      //特殊写函数
};

enum
{
    DEV_FLS_TYPE_FLS = 0,   //操作Flash
    DEV_FLS_TYPE_FRAM = 1,  //操作FRAM
    DEV_FLS_TYPE_VFRAM = 2, //操作虚拟FRAM
    DEV_FLS_TYPE_E2 = 3,    //操作E2
    DEV_FLS_TYPE_IFLS = 4,  //操作内部flash
    
};



uint8 DevFls_ProcMSG(TDevFlsOP* op);
TResult SYS_FILE_Write(uint8* buffer, uint32 addr, uint32 length);
TResult SYS_FILE_Read(uint8* buffer, uint32 addr, uint32 length);
uint32 SYS_FILE_Test(void);

void SYS_FILE_Open(void);
void SYS_FILE_Close(void);
TResult SYS_FILE_DB_Open(uint16_t user,uint16 ID, TDataBlock* db, TDB_MODE RW);
void SYS_FILE_DB_Close(TDataBlock* db);
TResult SYS_FILE_DB_ReadFrom(TDataBlock* db, uint8* buffer, uint32 length, uint32 addr);
TResult SYS_FILE_DB_ReadWithCRC(TDataBlock* db, uint8* buffer, uint32 length, uint32 addr);
TResult SYS_FILE_DB_ReadWithBackup(TDataBlock* db, uint8* buffer, uint32 length, uint32 addr);
TResult SYS_FILE_DB_WriteFrom(TDataBlock* db, uint8* buffer, uint32 length, uint32 addr);
TResult SYS_FILE_DB_SetFrom(TDataBlock* db, uint32 addr, uint32 length, uint8 templet);
TResult SYS_FILE_DB_WriteWithCRC(TDataBlock* db, uint8* buffer, uint32 length, uint32 addr);
TResult SYS_FILE_DB_WriteWithBackup(TDataBlock* db, uint8* buffer, uint32 length, uint32 addr);
TResult SYS_FILE_DB_WriteSpec(TDataBlock* db, uint8* buffer, uint32 length, uint32 addr);
TResult SYS_FILE_DB_BufferCheck(TDataBlock* db, uint8* buffer, uint32 length, uint32 addr);
TResult SYS_FILE_DB_Erase(TDataBlock* db, uint32 sectorfrom, uint32 sectornum);

TResult SYS_FRAM_Open(void);
void SYS_FRAM_Close(void);
TResult SYS_FRAM_Write(uint8* buffer, uint32 length, uint32 addr);
TResult SYS_FRAM_Read(uint8* buffer, uint32 length, uint32 addr);
TResult SYS_FRAM_SafeWrite(uint8* data, uint16 num);
TResult SYS_FRAM_SafeRead(uint8* data, uint16 num);
uint8 Fram_SafeRead(uint8* data, uint16 sno, uint16 len);
uint8 Fram_SafeWrite(uint8* data, uint16 sno, uint16 len);

TResult SYS_IFILE_DB_WriteFrom(TDataBlock* db, uint8* buffer, uint32 length, uint32 addr);
TResult SYS_IFILE_DB_ReadFrom(TDataBlock* db, uint8* buffer, uint32 length, uint32 addr);
/***********************************************************
 * @function_name: SYS_IFILE_DB_SetFrom
 * @function_file: es_file.c
 * @描述:设置相同数据
 * 
 * 
 * @参数: 
 * @param:db    
 * @param:addr 
 * @param:length
 * @param:templet
 * 
 * @返回: 
 * @return: TResult 
 * @作者:
 *---------------------------------------------------------
 * @修改人: yzy (2010/2/22)
 **********************************************************/
TResult SYS_IFILE_DB_SetFrom(TDataBlock* db, uint32 addr, uint32 length, uint8 templet);
TResult SYS_IFILE_DB_WriteWithBackup(TDataBlock* db, uint8* buffer, uint32 length, uint32 addr);
TResult SYS_IFILE_DB_ReadWithBackup(TDataBlock* db, uint8* buffer, uint32 length, uint32 addr);

/************************************************************************
 * @function: SYS_E2_SafeWrite
 * @描述: 向一个e2条目中写入数据
 * 
 * @参数: 
 * @param: data 欲写入的内容
 * @param: num 铁电条目编号
 * 
 * @返回: 
 * @return: TResult
 * @说明: 
 * @作者: yzy (2013/10/28)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 SYS_E2_SafeWrite(uint8* data, uint32_t addr, uint32_t len);
/************************************************************************
 * @function: SYS_E2_SafeRead
 * @描述: 从一个e2条目中读取数据
 * 
 * @参数: 
 * @param: data 一个铁电条目,大小至少为32字节,否则会导致缓存溢出
 * @param: num 铁电条目编号
 * 
 * @返回: 
 * @return: TResult  
 * @说明: 
 * @作者: yzy (2013/10/28)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 SYS_E2_SafeRead(uint8* data, uint32_t addr, uint32_t len);


/************************************************************************
 * @function: SYS_FILE_Init
 * @描述: 文件设备初始化
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/6/21)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void SYS_FILE_Init(void);







#endif


