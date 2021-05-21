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
** Description: �ļ�����
*************************************************************************/
#ifndef _DEV_FILE_H_
#define _DEV_FILE_H_

#ifndef EXT_DEV_FILE
#define EXT_DEV_FILE extern
#endif



/******************************************************************************
**flash������Դ
******************************************************************************/
//#define gs_FileRes  gs_SpiioRes
#define gs_FramRes  gs_SpiioRes


#define guc_FileUserTkid    guc_SpiioUserTkid
#define guc_FramUserTkid    guc_SpiioUserTkid



/****************************************************************************** 
**�����������ݵĲ����ṹ��
**�������̷�����Ϣʱ��ֵ��content
******************************************************************************/
typedef struct
{
    Object obj;                         //Ҫ����������
    uint32 addr;                        //Ҫ�����ĵ�ַ
    uint32 len;                         //�����ĳ���
    uint8  type;                        //����
    uint8  rw;                          //��д��ʽ
    //TResult re;                         //���صĽ��
    uint8 __a;
}TDevFlsOP;


enum
{
    DEV_FLS_RW_R = 0,       //��
    DEV_FLS_RW_W = 1,       //д
    DEV_FLS_RW_S = 2,       //����һ��������
    DEV_FLS_RW_E = 3,       //����
    DEV_FLS_RW_WS = 4,      //����д����
};

enum
{
    DEV_FLS_TYPE_FLS = 0,   //����Flash
    DEV_FLS_TYPE_FRAM = 1,  //����FRAM
    DEV_FLS_TYPE_VFRAM = 2, //��������FRAM
    DEV_FLS_TYPE_E2 = 3,    //����E2
    DEV_FLS_TYPE_IFLS = 4,  //�����ڲ�flash
    
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
 * @����:������ͬ����
 * 
 * 
 * @����: 
 * @param:db    
 * @param:addr 
 * @param:length
 * @param:templet
 * 
 * @����: 
 * @return: TResult 
 * @����:
 *---------------------------------------------------------
 * @�޸���: yzy (2010/2/22)
 **********************************************************/
TResult SYS_IFILE_DB_SetFrom(TDataBlock* db, uint32 addr, uint32 length, uint8 templet);
TResult SYS_IFILE_DB_WriteWithBackup(TDataBlock* db, uint8* buffer, uint32 length, uint32 addr);
TResult SYS_IFILE_DB_ReadWithBackup(TDataBlock* db, uint8* buffer, uint32 length, uint32 addr);

/************************************************************************
 * @function: SYS_E2_SafeWrite
 * @����: ��һ��e2��Ŀ��д������
 * 
 * @����: 
 * @param: data ��д�������
 * @param: num ������Ŀ���
 * 
 * @����: 
 * @return: TResult
 * @˵��: 
 * @����: yzy (2013/10/28)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 SYS_E2_SafeWrite(uint8* data, uint32_t addr, uint32_t len);
/************************************************************************
 * @function: SYS_E2_SafeRead
 * @����: ��һ��e2��Ŀ�ж�ȡ����
 * 
 * @����: 
 * @param: data һ��������Ŀ,��С����Ϊ32�ֽ�,����ᵼ�»������
 * @param: num ������Ŀ���
 * 
 * @����: 
 * @return: TResult  
 * @˵��: 
 * @����: yzy (2013/10/28)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 SYS_E2_SafeRead(uint8* data, uint32_t addr, uint32_t len);


/************************************************************************
 * @function: SYS_FILE_Init
 * @����: �ļ��豸��ʼ��
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/6/21)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void SYS_FILE_Init(void);







#endif


