/****************************************Copyright (c)**************************************************
**                                  ________科技有限公司
**                                          开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: dev_file.h
**创   建   人: yzy
**最后修改日期: 2010年11月11日
**描        述: 数据存储系统的头文件
**注        意: 
***--------------历史版本信息----------------------------------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2010年2月15日
** 描　述: 更改文件系统,采用固定数据块模式
********************************************************************************************************/ 
#ifndef _DEV_FILE_H_
#define _DEV_FILE_H_

#ifndef EXT_DEV_FILE
#define EXT_DEV_FILE extern
#endif

/******************************************************************************
**文件操作资源
******************************************************************************/
EXT_DEV_FILE ksem_t gs_FileRes;

EXT_DEV_FILE ktask_t * guc_FileUserTkid;        //当前占用资源的进程id,NULL表示未被占用.

EXT_DEV_FILE ksem_t gs_EepromRes;

EXT_DEV_FILE ktask_t * guc_EepromUserTkid;        //当前占用资源的进程id,NULL表示未被占用.


//	/******************************************************************************
//	**flash总线资源
//	******************************************************************************/
//	#define gs_FileRes  gs_SpiioRes
//	#define gs_EepromRes  gs_I2CioRes
//	
//	
//	#define guc_FileUserTkid    guc_SpiioUserTkid
//	#define guc_EepromUserTkid    guc_I2CioUserTkid



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
    
    DEV_FLS_RW_BE = 5,       //擦除
};

enum
{
    DEV_FLS_TYPE_FLS = 0,   //操作Flash
    DEV_FLS_TYPE_EEPROM = 1,   //操作eeprom
    DEV_FLS_TYPE_FRAM = 2,  //操作FRAM
    DEV_FLS_TYPE_VFRAM = 3, //操作虚拟FRAM
    DEV_FLS_TYPE_IFLS = 4,  //操作内部flash
    DEV_FLS_TYPE_IDEL = 5, //操作虚拟FRAM
};

#define DB_FLASH            0x1//     flash
#define DB_EEPROM           0x2//   eeprom
#define DT_EVENT_DATA_LEN   128//事件长度
#define DT_EVENT_DATA_NUM      256

#define DT_RECHARGE_DATA_LEN   64//事件长度
#define DT_RECHARGE_DATA_NUM      1024

#define CON_FILE_OPEN 0xAA
#define CON_FILE_CLOSE 0x55



#define CBB_ERR_VAR SYS_ERR_VAR
#define CBB_ERR_FT SYS_ERR_FT
#define CBB_ERR_OK SYS_ERR_OK
#define CBB_ERR_CLOSE SYS_ERR_EXIST
#define CBB_ERR_EXIST SYS_ERR_EXIST

uint8 DevFls_ProcMSG(TDevFlsOP* op);
TResult SYS_FILE_Write(uint8* buffer, uint32 addr, uint32 length);
TResult SYS_FILE_Read(uint8* buffer, uint32 addr, uint32 length);
uint32 SYS_FILE_Test(void);

TResult SYS_FILE_Open(void);
void SYS_FILE_Close(void);
TResult SYS_FILE_DB_Open(uint16 ID, TDataBlock* db, TDB_MODE RW);
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
TResult SYS_FILE_DB_BErase(TDataBlock* db, uint32 sectorfrom, uint32 sectornum);

TResult SYS_FRAM_Open(void);
void SYS_FRAM_Close(void);
TResult SYS_FRAM_Write(uint8* buffer, uint32 length, uint32 addr);
TResult SYS_FRAM_Read(uint8* buffer, uint32 length, uint32 addr);
TResult SYS_FRAM_SafeWrite(uint8* data, uint16 num);
TResult SYS_FRAM_SafeRead(uint8* data, uint16 num);
uint8 Fram_SafeRead(uint8* data, uint16 sno, uint16 len);
uint8 Fram_SafeWrite(uint8* data, uint16 sno, uint16 len);

TResult SYS_EEPROM_Open(void);
void SYS_EEPROM_Close(void);
uint8 Eeprom_SafeRead(uint8* data, uint32 addr, uint16 len);
uint8 Eeprom_SafeWrite(uint8* data, uint32 addr, uint16 len);


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
/************************************************************************
 * @function: SYS_FILE_Start
 * @描述: 文件设备开始
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/6/21)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void SYS_FILE_Start(void);

/************************************************************************
 * @function: SYS_FILE_Stop
 * @描述: 文件设备关闭
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/6/21)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/

void SYS_FILE_Stop(void);





#endif


