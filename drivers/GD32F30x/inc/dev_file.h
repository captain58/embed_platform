/****************************************Copyright (c)**************************************************
**                                  ________�Ƽ����޹�˾
**                                          ������
**
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: dev_file.h
**��   ��   ��: yzy
**����޸�����: 2010��11��11��
**��        ��: ���ݴ洢ϵͳ��ͷ�ļ�
**ע        ��: 
***--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: yzy
** ��  ��: v1.0
** �ա���: 2010��2��15��
** �衡��: �����ļ�ϵͳ,���ù̶����ݿ�ģʽ
********************************************************************************************************/ 
#ifndef _DEV_FILE_H_
#define _DEV_FILE_H_

#ifndef EXT_DEV_FILE
#define EXT_DEV_FILE extern
#endif

/******************************************************************************
**�ļ�������Դ
******************************************************************************/
EXT_DEV_FILE ksem_t gs_FileRes;

EXT_DEV_FILE ktask_t * guc_FileUserTkid;        //��ǰռ����Դ�Ľ���id,NULL��ʾδ��ռ��.

EXT_DEV_FILE ksem_t gs_EepromRes;

EXT_DEV_FILE ktask_t * guc_EepromUserTkid;        //��ǰռ����Դ�Ľ���id,NULL��ʾδ��ռ��.


//	/******************************************************************************
//	**flash������Դ
//	******************************************************************************/
//	#define gs_FileRes  gs_SpiioRes
//	#define gs_EepromRes  gs_I2CioRes
//	
//	
//	#define guc_FileUserTkid    guc_SpiioUserTkid
//	#define guc_EepromUserTkid    guc_I2CioUserTkid



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
    
    DEV_FLS_RW_BE = 5,       //����
};

enum
{
    DEV_FLS_TYPE_FLS = 0,   //����Flash
    DEV_FLS_TYPE_EEPROM = 1,   //����eeprom
    DEV_FLS_TYPE_FRAM = 2,  //����FRAM
    DEV_FLS_TYPE_VFRAM = 3, //��������FRAM
    DEV_FLS_TYPE_IFLS = 4,  //�����ڲ�flash
    DEV_FLS_TYPE_IDEL = 5, //��������FRAM
};

#define DB_FLASH            0x1//     flash
#define DB_EEPROM           0x2//   eeprom
#define DT_EVENT_DATA_LEN   128//�¼�����
#define DT_EVENT_DATA_NUM      256

#define DT_RECHARGE_DATA_LEN   64//�¼�����
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
 * @����: �ļ��豸��ʼ��
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/6/21)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void SYS_FILE_Init(void);
/************************************************************************
 * @function: SYS_FILE_Start
 * @����: �ļ��豸��ʼ
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/6/21)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void SYS_FILE_Start(void);

/************************************************************************
 * @function: SYS_FILE_Stop
 * @����: �ļ��豸�ر�
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/6/21)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/

void SYS_FILE_Stop(void);





#endif


