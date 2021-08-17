/****************************************Copyright (c)**************************************************
**                                  ________�Ƽ����޹�˾
**                                          ������
**
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: SYS_file.c
**��   ��   ��: yzy
**����޸�����: 2018��2��20��
**��        ��: �ļ��豸����
**ע        ��:
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: yzy
** ��  ��: v1.0
** �ա���: 2018��2��20��
** �衡��: ԭʼ�汾
********************************************************************************************************/

#define EXT_FILE
#define EXT_DEV_FILE
#define EXT_IIC_EEPROM
#define EXT_MXFLASH

//	#include <stdint.h>
//	#include "bdef.h"
//	#include "cdef.h"
//	#include "bsp.h"
//	
//	#include "iomap.h"
//	//	#include "flsmap.h"
//	//	#include <lib_app.h>
//	
//	
//	//	#include "apis.h"
//	#include "k_types.h"
//	#include "k_api.h"
//	//#include "uartcfg.h"
//	#include "lib_app.h"
//	#include "hal.h"
#include "aos/include.h"
#include "hal.h"
#include "flsmap.h"
/******************************************************************************
**��̬���ݿ����õĺϷ����ж�
******************************************************************************/
#if SYS_FILE_DYM_EN > 0
    #if DB_DYM_DB_NUM > 168
        #warning "��̬��̫����!"
    #endif

    #if ((1 << DB_DYM_DB_2M) != DB_DYM_DB_SIZE)
        #warning "��̬�鶨�����!"
    #endif 
#endif



/******************************************************************************
**�����ⲿ����
******************************************************************************/
extern uint32 gul_FramSize;
extern const S_FILEBLOCKS gss_FileBlocks[];
extern const uint8 LEN_DB_LIST;
extern const TFlsCycleConst gs_flsFramConst;

/******************************************************************************
**��̬���ݿ���Ϣ����ָ��,ָ��Ļ����СΪ(DB_DYM_DB_NUM + 1) * 3 + 1 
**���ݻ���˵��, �ֽ�0--DB_DYM_DB_NUM - 1: ���ݿ����� 
**              �ֽ�DB_DYM_DB_NUM--DB_DYM_DB_NUM * 2 - 1    ����Ķ�̬�����ʼ��
**              �ֽ�DB_DYM_DB_NUM * 2--DB_DYM_DB_NUM*3 - 1  ����Ķ�̬������ݿ�����
**              �ֽ�DB_DYM_DB_NUM*3                     ���п�ͷָ��
**              �ֽ�DB_DYM_DB_NUM*3 + 1                 ���п�����
**              �ֽ�DB_DYM_DB_NUM*3 + 2--(DB_DYM_DB_NUM + 1) * 3  CRCУ��
******************************************************************************/
uint8* DymBuffer;




/***********************************************************
 * @function_name: DevFls_ProcMSG
 * @function_file: dev_fls.c
 * @����:  ���������Ϣ(ע��,����ǰ����������Ӧ���ߵ���Դ��)
 * 
 * 
 * @����: 
 * @param:msg  
 * @����: 
 * @����:
 *---------------------------------------------------------
 * @�޸���: houpm (2010/2/20)
 **********************************************************/
uint8 DevFls_ProcMSG(TDevFlsOP* op)
{
    uint8 result = SYS_ERR_FT;
    if(op->type == DEV_FLS_TYPE_IDEL)//������
    {

    }

#if SYS_EEPROM_EN > 0
    
    else if(op->type == DEV_FLS_TYPE_EEPROM)//����FRAM
    {
       switch(op->rw)
       {
           case DEV_FLS_RW_R:           //������
               result = Eeprom_Read((uint8*)op->obj, op->addr, op->len);
               break;
           case DEV_FLS_RW_W:           //д����
               result = Eeprom_Write((uint8*)op->obj, op->addr, op->len);
               break;
       }
    }
#endif    
#if SYS_FILE_EN > 0

    else if(op->type == DEV_FLS_TYPE_FLS)     //����Flash
    {
       switch(op->rw)
       {
//           case DEV_FLS_RW_R:           //������
//               result = FLS_Read((uint8*)op->obj, op->addr, op->len);
//               break;
//           case DEV_FLS_RW_W:           //д����
//               FLS_PreWrite();
//               result = FLS_Write((uint8*)op->obj, op->addr, op->len);
//               break;
//           case DEV_FLS_RW_S:           //���ò���
//               FLS_PreWrite();
//               result = FLS_Set((uint8)(uint32)op->obj, op->addr, op->len);
//               break;
//           case DEV_FLS_RW_E:           //��������
//               FLS_PreWrite();
//               result = FLS_Erase(op->addr, op->len);
//               break;
//           case DEV_FLS_RW_BE:           //��������
//               FLS_PreWrite();
//               result = FLS_BErase(op->addr, op->len);
//               break;
//               
//           case DEV_FLS_RW_WS:          //Flash����д�����
//               FLS_PreWrite();
//               result = FLS_WriteSpec((uint8*)op->obj, op->addr, op->len);
//               break;
       }
    }
#endif
#if SYS_FRAM_EN > 0
    #if SYS_FLASH_FRAM_EN == 0

    else if(op->type == DEV_FLS_TYPE_FRAM)//����FRAM
    {
       switch(op->rw)
       {
           case DEV_FLS_RW_R:           //������
               result = FRAM_Read((uint8*)op->obj, op->addr, op->len);
               break;
           case DEV_FLS_RW_W:           //д����
               FRAM_PreWrite();
               result = FRAM_Write((uint8*)op->obj, op->addr, op->len);
               break;
       }
    }

    #else

    else if(op->type == DEV_FLS_TYPE_VFRAM)//����VFRAM
    {
        switch(op->rw)
        {
           case DEV_FLS_RW_R:           //������
               result = SYS_VFRAM_ReadSample((uint8*)op->obj, op->addr, (const TFlsCycleConst*)op->len);
               break;
           case DEV_FLS_RW_W:           //д����
               result = SYS_VFRAM_WriteSample((uint8*)op->obj, op->addr, (const TFlsCycleConst*)op->len);
               break;
        }
    }
    #endif
#endif
    return result;

    
}



#if SYS_FILE_EN > 0

/***********************************************************
 * @function_name: SYS_FILE_Write
 * @function_file: dev_file.c
 * @����:д������
 * 
 * 
 * @����: 
 * @param:db  
 * @param:buffer  
 * @param:length  
 * @param:addr  
 * 
 * @����: 
 * @return: TResult 
 * @����:
 *---------------------------------------------------------
 * @�޸���: houpm (2010/2/22)
 **********************************************************/
TResult SYS_FILE_Write(uint8* buffer, uint32 addr, uint32 length)
{
    TDevFlsOP op;                       //����һ��Flash�����ṹ
    uint8 result;
                                        //�ж��Ƿ��ѱ���������ռ��
    if(guc_FileUserTkid != krhino_cur_task_get())
    {
        return SYS_ERR_FT;
    }
    
    op.addr = addr;                     //��ַ
    op.len = length;                    //����
    op.obj = buffer;                    //����
    op.type = DEV_FLS_TYPE_FLS;         //����
    op.rw = DEV_FLS_RW_W;               //��д��ʽ

                                        //������Ϣ
    result = DevFls_ProcMSG(&op);  
    return result;                      //δ֪����
}



/***********************************************************
 * @function_name: SYS_FILE_ReadFromAddr
 * @function_file: app_calc.c
 * @����:��ָ����ƫ�Ƶ�ַ��ʼ��ȡ����
 * 
 * 
 * @����: 
 * @param:db  
 * @param:buffer  
 * @param:length  
 * @param:addr  
 * 
 * @����: 
 * @return: TResult 
 * @����:
 *---------------------------------------------------------
 * @�޸���: houpm (2010/2/21)
 **********************************************************/
TResult SYS_FILE_Read(uint8* buffer, uint32 addr, uint32 length)
{
    TDevFlsOP op;                       //����һ��Flash�����ṹ
    uint8 result;
                                        //�ж��Ƿ��ѱ���������ռ��
    if(guc_FileUserTkid != krhino_cur_task_get())
    {
        return SYS_ERR_FT;
    }
    
    op.addr = addr;                     //��ַ
    op.len = length;                    //����
    op.obj = buffer;                    //����
    op.type = DEV_FLS_TYPE_FLS;         //����
    op.rw = DEV_FLS_RW_R;               //��д��ʽ
    
    result = DevFls_ProcMSG(&op);
    return result;
}

/************************************************************************
 * @function: SYS_FILE_Test
 * @����: flash���ܲ���
 * @����: 
 * 
 * @����: 
 * @return: uint32  SYS_ERR_OK ��������, SYS_ERR_FT �����쳣(������Ϊ����dev_file2.cһ��)
 * @˵��: 
 * @����: yzy (2013/6/8)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint32 SYS_FILE_Test(void)
{
    uint8 cmp1[8];                          //ԭʼ����
    uint8 cmp2[8];                          //�޸ĺ�����д������
    uint8 cmp3[8];                          //�޸ĺ�ʵ��д������
    uint32 err = 0;

                                        //�ж��Ƿ��ѱ���������ռ��
    if(guc_FileUserTkid != krhino_cur_task_get())
    {
        return SYS_ERR_FT;
    }
    
    //Խ��ʹ�õײ㺯��,�Ա�ָ������ĳ��flash
    uint8 ReadFlsInChip(uint8* buffer, uint32 address, uint16 length, uint8 x);
    uint8 WriteFlsInChip(uint8* buffer, uint32 addr, uint16 length, uint8 x, uint8 sdmode);
                                            //ִ�в���
    for(uint8 k = 0; k < FLS_CHIP_NO; k++)
    {
                                            //��ȡԭʼ����
        if(ReadFlsInChip(cmp1, 0, 5, k) != SYS_ERR_OK)
        {
            err |= Bit_Map8[k];
            break;
        }
                                            //�޸�ԭʼ����
        for(uint8 i = 0; i < 5; i++)
        {
            cmp2[i] = cmp1[i] + 2;
        }
                                            //д������д������
        if(WriteFlsInChip(cmp2, 0, 5, k, 0) != SYS_ERR_OK)
        {
            err |= Bit_Map8[k];
            break;
        }
                                            //��ȡʵ��д������
        if(ReadFlsInChip(cmp3, 0, 5, k) != SYS_ERR_OK)
        {
            err |= Bit_Map8[k];
            break;
        }
        
        WriteFlsInChip(cmp1, 0, 5, k, 0);   //�ָ�ԭʼֵ
        if(CmpBuffer(cmp2, cmp3, 5) != 0)   //�ȶ�����ʵ��ֵ
        {
            err |= Bit_Map8[k];
            break;
        }
    }
    
    return err;
}


/***********************************************************
 * @function_name: SYS_FILE_Open
 * @function_file: SYS_file.c
 * @����:�����ݿ�
 * 
 * 
 * @����: 
 * @param:
 * 
 * @����: 
 * @return:  
 * @����:
 *---------------------------------------------------------
 * @�޸���: yzy (2010/2/21)
 **********************************************************/
TResult SYS_FILE_Open(void)
{
    //������Դ
    if(guc_FileUserTkid != NULL && guc_FileUserTkid == krhino_cur_task_get())
        return SYS_ERR_OK;
    krhino_sem_take(&gs_FileRes, RHINO_WAIT_FOREVER);
    guc_FileUserTkid = krhino_cur_task_get();  //Ϊ��ǰ�û����̱��
    return SYS_ERR_OK;
}

/***********************************************************
 * @function_name: SYS_FILE_Close
 * @function_file: SYS_file.c
 * @����:�ر����ݿ�
 * 
 * 
 * @����: 
 * @param:
 * 
 * @����: 
 * @return:  
 * @����:
 *---------------------------------------------------------
 * @�޸���: yzy (2010/2/21)
 **********************************************************/
void SYS_FILE_Close(void)
{
    if(guc_FileUserTkid == krhino_cur_task_get())
    {
        guc_FileUserTkid = NULL;        //�ָ��ɳ�ʼֵ
        //krhino_sem_give(&gs_FileRes);    //�ͷ�����ź���
        krhino_sem_give(&gs_FileRes);
    }
}

#include <assert.h>


/***********************************************************
 * @function_name: SYS_FILE_DB_Open
 * @function_file: SYS_file.c
 * @����:��ָ����ŵ����ݿ�
 * 
 * 
 * @����: 
 * @param:ID  ���ݿ���
 * @param:db  ���ݿ�ṹ
 * 
 * @����: 
 * @return: TResult 
 * @����:
 *---------------------------------------------------------
 * @�޸���: houpm (2010/2/21)
 **********************************************************/
TResult SYS_FILE_DB_Open(uint16 ID, TDataBlock* db, TDB_MODE RW)
{
    uint32 start;
    uint16 nblock;
    uint16 k;
                                        //�������ݲ�����ı���
    start = 0;
    nblock = 0;
    for(k = 0; k < LEN_DB_LIST; k++)
    {
        if(ID > (gss_FileBlocks[k].dbno + gss_FileBlocks[k].sml))
        {
            start += (gss_FileBlocks[k].sml + 1) * gss_FileBlocks[k].blocks;
        }
        else if((ID >= gss_FileBlocks[k].dbno) && (ID <= gss_FileBlocks[k].dbno + gss_FileBlocks[k].sml))
        {
            start += (ID - gss_FileBlocks[k].dbno) * gss_FileBlocks[k].blocks;
            nblock = gss_FileBlocks[k].blocks;
            break;
        }
        else
        {
            return SYS_ERR_FT;
        }
    }
    
    if(k == LEN_DB_LIST)
    {
        return SYS_ERR_FT;
    }
                                        //
    if((gss_FileBlocks[k].atr & RW) != RW)
    {
        return SYS_ERR_TYPE;
    }
                                        //
    assert(start < 0x000FFFFF);         //ȷ������12λ�����uint32_max.
    db->start = DB_LEN_BLOCK(start);
    db->length = DB_LEN_BLOCK(nblock);
    db->RW = gss_FileBlocks[k].atr;
    db->ID = ID;
    
    if((db->start == 0) && (nblock > 1))//�������ݿ��׸�block�������updatemark
    {
        db->start = DB_LEN_BLOCK(1);
        nblock -= 1;                    //ʵ��������һ��block
    }
                                        //������Դ
    SYS_FILE_Open();
    
    SYS_OK();
}


/***********************************************************
 * @function_name: SYS_FILE_DB_Close
 * @function_file: SYS_file.c
 * @����:�ر�ָ����ŵ����ݿ�
 * 
 * 
 * @����: 
 * @param:db  ���ݿ���
 * 
 * @����: 
 * @return:  
 * @����:
 *---------------------------------------------------------
 * @�޸���: houpm (2010/2/21)
 **********************************************************/
void SYS_FILE_DB_Close(TDataBlock* db)
{
    SYS_FILE_Close();
}


/***********************************************************
 * @function_name: SYS_FILE_DB_ReadFrom
 * @function_file: SYS_file.c
 * @����:��ָ����ƫ�ƿ�ʼ��ȡ����
 * 
 * 
 * @����: 
 * @param:db  
 * @param:buffer  
 * @param:length  
 * @param:addr  
 * 
 * @����: 
 * @return: TResult 
 * @����:
 *---------------------------------------------------------
 * @�޸���: houpm (2010/2/21)
 **********************************************************/
TResult SYS_FILE_DB_ReadFrom(TDataBlock* db, uint8* buffer, uint32 length, uint32 addr)
{
    TDevFlsOP op;                       //����һ��Flash�����ṹ
    uint8 result;
    
    if(!(db->RW & TDB_MODE_R))          //��Ҫ֧�ֶ����Բ������ȡ
    {
        return SYS_ERR_TYPE;
    }
                                        //�ж��Ƿ��ѱ���������ռ��
    if(guc_FileUserTkid != krhino_cur_task_get())
    {
        return SYS_ERR_FT;
    }

    op.addr = db->start + addr;         //��ַ
    op.len = length;                    //����
    op.obj = buffer;                    //����
    op.type = DEV_FLS_TYPE_FLS;         //����
    op.rw = DEV_FLS_RW_R;               //��д��ʽ

    result = DevFls_ProcMSG(&op);

    return result;
}


/***********************************************************
 * @function_name: SYS_FILE_DB_ReadWithCRC
 * @function_file: SYS_file.c
 * @����:��У��Ķ�ȡ
 * 
 * 
 * @����: 
 * @param:db  
 * @param:buffer  
 * @param:length  
 * @param:addr  
 * 
 * @����: 
 * @return: TResult 
 * @����:
 *---------------------------------------------------------
 * @�޸���: houpm (2010/2/22)
 **********************************************************/
TResult SYS_FILE_DB_ReadWithCRC(TDataBlock* db, uint8* buffer, uint32 length, uint32 addr)
{
    TResult r;
    r = SYS_FILE_DB_ReadFrom(db, buffer, length, addr);

    if(ER(r))
    {
        if(!CheckBufferCRC(buffer, length, true))
        {
            memset(buffer, 0x00, length);
            r = SYS_ERR_CHK;
        }
    }
    return r;
}

/***********************************************************
 * @function_name: SYS_FILE_DB_ReadWithBackup
 * @function_file: SYS_file.c
 * @����:������+У��Ķ�ȡ
 * 
 * 
 * @����: 
 * @param:db  
 * @param:buffer  
 * @param:length  
 * @param:addr  
 * 
 * @����: 
 * @return: TResult 
 * @����:
 *---------------------------------------------------------
 * @�޸���: houpm (2010/2/22)
 **********************************************************/
TResult SYS_FILE_DB_ReadWithBackup(TDataBlock* db, uint8* buffer, uint32 length, uint32 addr)
{
    TResult r; 

    if(!(db->RW & TDB_MODE_BK))         //ȷ��֧�ֱ��ݿ�����
    {
        return SYS_ERR_TYPE;
    }
                                        //ȷ�ϲ�������ȷ��
    SYS_VAR_CHECK(addr + length > (db->length >> 1));
                                        //���ݵ�ַ
    uint32 bkaddr = addr + (db->length >> 1);
                                        //��CRCУ��Ķ�ȡ
    r = SYS_FILE_DB_ReadWithCRC(db, buffer, length, addr);

    if(!ER(r))                          //�����������ȡ������
    {
        r = SYS_FILE_DB_ReadWithCRC(db, buffer, length, bkaddr);
        if(ER(r))
        {
            return SYS_ERR_MAIN;         //��������,����������ȷ
        }
    }
    return r;                           //����д����

}


/***********************************************************
 * @function_name: SYS_FILE_DB_WriteFrom
 * @function_file: SYS_file.c
 * @����:д������
 * 
 * 
 * @����: 
 * @param:db  
 * @param:buffer  
 * @param:length  
 * @param:addr  
 * 
 * @����: 
 * @return: TResult 
 * @����:
 *---------------------------------------------------------
 * @�޸���: houpm (2010/2/22)
 **********************************************************/
TResult SYS_FILE_DB_WriteFrom(TDataBlock* db, uint8* buffer, uint32 length, uint32 addr)
{
    if(!(db->RW & TDB_MODE_W))          //��Ҫ֧�ֶ����Բ������ȡ
    {
        return SYS_ERR_TYPE;
    }
                                        //ȷ�ϲ����Ƿ���ȷ
    SYS_VAR_CHECK(db->length < addr + length);
                                        //�ж��Ƿ��ѱ���������ռ��
    if(guc_FileUserTkid != krhino_cur_task_get())
    {
        return SYS_ERR_FT;
    }

    TDevFlsOP op;                       //����һ��Flash�����ṹ
    uint8 result;

    op.addr = db->start + addr;         //��ַ
    op.len = length;                    //����
    op.obj = buffer;                    //����
    op.type = DEV_FLS_TYPE_FLS;         //����
    op.rw = DEV_FLS_RW_W;               //��д��ʽ

    result = DevFls_ProcMSG(&op);
    
    return result; 
}



/***********************************************************
 * @function_name: SYS_FILE_DB_SetFrom
 * @function_file: SYS_file.c
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
 * @�޸���: houpm (2010/2/22)
 **********************************************************/
TResult SYS_FILE_DB_SetFrom(TDataBlock* db, uint32 addr, uint32 length, uint8 templet)
{
    if(!(db->RW & TDB_MODE_W))          //��Ҫ֧�ֶ����Բ������ȡ
    {
        return SYS_ERR_TYPE;
    }
                                        //ȷ�ϲ����Ƿ���ȷ
    SYS_VAR_CHECK(db->length < addr + length);
                                        //�ж��Ƿ��ѱ���������ռ��
    if(guc_FileUserTkid != krhino_cur_task_get())
    {
        return SYS_ERR_FT;
    }

    TDevFlsOP op;                       //����һ��Flash�����ṹ
    uint8 result;

    op.addr = db->start + addr;         //��ַ
    op.len = length;                    //����
    op.obj = (Object)templet;           //����
    op.type = DEV_FLS_TYPE_FLS;         //����
    op.rw = DEV_FLS_RW_S;               //��д��ʽ

    result = DevFls_ProcMSG(&op);
    
    return result;
}


/***********************************************************
 * @function_name: SYS_FILE_DB_WriteWithCRC
 * @function_file: SYS_file.c
 * @����:��CRCУ���д��,ע��д��Ļ���Ҫ���������ֽ�
 * 
 * 
 * @����: 
 * @param:db  
 * @param:buffer  
 * @param:length  
 * @param:addr  
 * 
 * @����: 
 * @return: TResult 
 * @����:
 *---------------------------------------------------------
 * @�޸���: houpm (2010/2/22)
 **********************************************************/
TResult SYS_FILE_DB_WriteWithCRC(TDataBlock* db, uint8* buffer, uint32 length, uint32 addr)
{
    BuildBufferCRC(buffer, length, true);
    return SYS_FILE_DB_WriteFrom(db, buffer, length, addr);
}


/***********************************************************
 * @function_name: SYS_FILE_DB_WriteWithBackup
 * @function_file: SYS_file.c
 * @����:д������,������CRCУ��,��������,������blockʵ�����õĴ�Сֻ��
 * 
 * 
 * @����: 
 * @param:db  
 * @param:buffer  
 * @param:length  
 * @param:addr  
 * 
 * @����: 
 * @return: TResult 
 * @����:
 *---------------------------------------------------------
 * @�޸���: houpm (2010/2/22)
 **********************************************************/
TResult SYS_FILE_DB_WriteWithBackup(TDataBlock* db, uint8* buffer, uint32 length, uint32 addr)
{
    TResult r1, r2; 

    if(!(db->RW & TDB_MODE_BK))         //ȷ��֧�ֱ��ݿ�����
    {
        return SYS_ERR_TYPE;
    }
                                        //ȷ�ϲ�������ȷ��
    SYS_VAR_CHECK(addr + length > (db->length >> 1));
                                        //���ݵ�ַ
    uint32 bkaddr = addr + (db->length >> 1);
                                        //��CRCУ���д��
    r1 = SYS_FILE_DB_WriteWithCRC(db, buffer, length, addr);
    r2 = SYS_FILE_DB_WriteWithCRC(db, buffer, length, bkaddr);
    
    if(!ER(r1) && ER(r2))               //��������,��������ȷ
    {
        return SYS_ERR_MAIN;
    }

    if(!ER(r2) && ER(r1))               //����������,������ȷ
    {
        return SYS_ERR_BACK;
    }
    
    if(!ER(r2) && !ER(r1))              //����������,��������
    {
        return SYS_ERR_CHK;
    }
    SYS_OK();
}

/*******************************************************************************
** @function_name:  SYS_FILE_DB_WriteSpec
** @function_file:  dev_file.c
** @����: �ļ����ݿ�����д����(���������뱣��ԭ�����ݵ�д����,�ر������ڴ�����������д��)
** 
** 
** @����: 
** @param: db 
** @param: buffer 
** @param: length 
** @param: addr 
** 
** @����: 
** @return:  TResult   
** @����: yzy (2011-07-25)
**-----------------------------------------------------------------------------
** @�޸���: 
** @�޸�˵��: 
*******************************************************************************/
TResult SYS_FILE_DB_WriteSpec(TDataBlock* db, uint8* buffer, uint32 length, uint32 addr)
{
    if(!(db->RW & TDB_MODE_W))          //��Ҫ֧��д���Բ������ȡ
    {
        return SYS_ERR_TYPE;
    }
                                        //ȷ�ϲ����Ƿ���ȷ
    SYS_VAR_CHECK(db->length < addr + length);
    
                                        //�ж��Ƿ��ѱ���������ռ��
    if(guc_FileUserTkid != krhino_cur_task_get())
    {
        return SYS_ERR_FT;
    }

    TDevFlsOP op;                       //����һ��Flash�����ṹ
    uint8 result;

    op.addr = db->start + addr;         //��ַ
    op.len = length;                    //����
    op.obj = buffer;                    //����
    op.type = DEV_FLS_TYPE_FLS;         //����
    op.rw = DEV_FLS_RW_WS;              //��д��ʽ

    result = DevFls_ProcMSG(&op);
    
    return result;
}

/***********************************************************
 * @function_name: SYS_FILE_DB_BufferCheck
 * @function_file: SYS_file.c
 * @����:У������buffer�г���Ϊlenght������,
 *        ���buffer�е�����CRCУ��ͨ���򷵻�.
 *        �����flash�и�������.flash���ݴ����db,��ַΪaddr.
 * 
 * 
 * @����: 
 * @param:db  
 * @param:buffer  
 * @param:length  
 * @param:addr  
 * 
 * @����: 
 * @return: TResult 
 * @����:
 *---------------------------------------------------------
 * @�޸���: houpm (2010/3/3)
 **********************************************************/
TResult SYS_FILE_DB_BufferCheck(TDataBlock* db, uint8* buffer, uint32 length, uint32 addr)
{
    TResult r = SYS_ERR_OK;

    if(!CheckBufferCRC(buffer, length, true))
    {
        r = SYS_ERR_CHK;
    }

    if(!ER(r))
    {
        r = SYS_FILE_DB_ReadWithBackup(db, buffer, length, addr);
    }
    
    return r;
}   


/***********************************************************
 * @function_name: SYS_FILE_DB_Erase
 * @function_file: SYS_file.c
 * @����:
 * 
 * 
 * @����: 
 * @param: db Ҫ���������ݿ�
 * @param: sectorfrom ��ʼ��������(0~)
 * @param: sectornum ��������������(�������ݿ��С���ֲ���)
 * 
 * @����: 
 * @return: TResult 
 * @����:
 *---------------------------------------------------------
 * @�޸���: houpm (2010/2/22)
 **********************************************************/
TResult SYS_FILE_DB_Erase(TDataBlock* db, uint32 sectorfrom, uint32 sectornum)
{
    TDevFlsOP op;                       //����һ��Flash�����ṹ
    uint8 result;
    uint32 dbsectors;                   //���ݿ����������������
                                        //ȷ�ϲ����Ƿ���ȷ
    SYS_VAR_CHECK(sectorfrom > sectornum);
    SYS_VAR_CHECK(sectornum == 0);
    
                                        //�ж��Ƿ��ѱ���������ռ��
    if(guc_FileUserTkid != krhino_cur_task_get())
    {
        return SYS_ERR_FT;
    }
                                        //����������
    dbsectors = (db->length >> 12);
                                        //��ʼsector���������ݿ鷶Χ��
    if(sectorfrom >= dbsectors)
    {
        return SYS_ERR_FT;
    }
                                        //���������������������ݿ鷶Χ��
    if((sectorfrom + sectornum) > dbsectors)
    {
        sectornum = dbsectors - sectorfrom;
    }

    op.addr = db->start + (sectorfrom << 12);//Ҫ�������������ݿ��ڵľ��Ե�ַ
    op.len = sectornum;                 //������������
    op.obj = __NULL;
    op.type = DEV_FLS_TYPE_FLS;
    op.rw = DEV_FLS_RW_E;
    
    result = DevFls_ProcMSG(&op);
    
    return result;
}

TResult SYS_FILE_DB_BErase(TDataBlock* db, uint32 sectorfrom, uint32 sectornum)
{
    TDevFlsOP op;                       //����һ��Flash�����ṹ
    uint8 result;
    uint32 dbsectors;                   //���ݿ����������������
                                        //ȷ�ϲ����Ƿ���ȷ
    SYS_VAR_CHECK(sectorfrom > sectornum);
    SYS_VAR_CHECK(sectornum == 0);
    
                                        //�ж��Ƿ��ѱ���������ռ��
    if(guc_FileUserTkid != krhino_cur_task_get())
    {
        return SYS_ERR_FT;
    }
                                        //����������
    dbsectors = (db->length >> 16);
                                        //��ʼsector���������ݿ鷶Χ��
    if(sectorfrom >= dbsectors)
    {
        return SYS_ERR_FT;
    }
                                        //���������������������ݿ鷶Χ��
    if((sectorfrom + sectornum) > dbsectors)
    {
        sectornum = dbsectors - sectorfrom;
    }

    op.addr = db->start + (sectorfrom << 16);//Ҫ�������������ݿ��ڵľ��Ե�ַ
    op.len = sectornum;                 //������������
    op.obj = __NULL;
    op.type = DEV_FLS_TYPE_FLS;
    op.rw = DEV_FLS_RW_BE;
    
    result = DevFls_ProcMSG(&op);
    
    return result;
}


#endif


#if SYS_FRAM_EN > 0


#if (SYS_FLASH_FRAM_EN > 0)


/************************************************************************
 * @function: SYS_FRAM_Open
 * @����: ��һ��������Ŀ��д������
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
TResult SYS_FRAM_Open(void)
{
//	    SYS_SEM_Wait(&gs_FramRes, 0);
    krhino_sem_take(&gs_FramRes, RHINO_WAIT_FOREVER);
    guc_FramUserTkid = krhino_cur_task_get();  //Ϊ��ǰ�û����̱��
    
    return SYS_ERR_OK;
}


/************************************************************************
 * @function: SYS_SER_Close
 * @����: ���ڹر�
 * 
 * @����: 
 * @param:  
 * @����: 
 * @˵��: 
 * @����: yzy (2018/5/2)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void SYS_FRAM_Close(void)
{
    if(guc_FramUserTkid == krhino_cur_task_get())
    {
        guc_FramUserTkid = 0xFF;        //�ָ��ɳ�ʼֵ
        krhino_sem_give(&gs_FramRes);    //�ͷ�����ź���
    }
}

/************************************************************************
 * @function: SYS_FRAM_SafeWrite
 * @����: ��һ��������Ŀ��д������
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
TResult SYS_FRAM_SafeWrite(uint8* data, uint16 num)
{
    TDevFlsOP op;                       //����һ��Flash�����ṹ
    uint8 result;
                                        //�ж��Ƿ��ѱ���������ռ��
    if(guc_FramUserTkid != krhino_cur_task_get())
    {
        return SYS_ERR_FT;
    }
    
    op.addr = num;                      //��ַ
    op.len = (uint32)&gs_flsFramConst;  //����(����������Ϣ�ṹ��)
    op.obj = data;                      //����
    op.type = DEV_FLS_TYPE_VFRAM;       //����
    op.rw = DEV_FLS_RW_W;               //��д��ʽ
    result = DevFls_ProcMSG(&op);

    return result;                      //δ֪����
}

/************************************************************************
 * @function: SYS_FRAM_SafeRead
 * @����: ��һ��������Ŀ�ж�ȡ����
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
TResult SYS_FRAM_SafeRead(uint8* data, uint16 num)
{
    TDevFlsOP op;                       //����һ��Flash�����ṹ
    uint8 result;
                                        //�ж��Ƿ��ѱ���������ռ��
    if(guc_FramUserTkid != krhino_cur_task_get())
    {
        return SYS_ERR_FT;
    }
    
    op.addr = num;                      //��ַ
    op.len = (uint32)&gs_flsFramConst;  //����(����������Ϣ�ṹ��)
    op.obj = data;                      //����
    op.type = DEV_FLS_TYPE_VFRAM;       //����
    op.rw = DEV_FLS_RW_R;               //��д��ʽ
    result = DevFls_ProcMSG(&op);
    
    return result;                      //δ֪����
}



/************************************************************************
 * @function: Fram_SafeRead
 * @����: ������İ�ȫ�������������Ķ�ȡ����
 *         (ʹ��ǰ����Ҫִ��open��close)
 * 
 * @����: 
 * @param: data ��ȡ��������Ҫ����Ļ���
 * @param: sno ��ȡ�����ݵ���ʼ���
 * @param: len ��ȡ�����ݵĳ���
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2013/10/28)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 Fram_SafeRead(uint8* data, uint16 sno, uint16 len)
{
    uint16 k = len >> 5;                //��ȡ�ܹ���Ҫ�����ݿ���
    uint8 t[32];

    if(len & 0x1F)
    {
        k++;
    }

    for(uint16 m = 0; m < k; m++)       //�ֿ��ȡ����
    {
                                        //��ȡ����
        if(SYS_FRAM_SafeRead(t, sno + m) != SYS_ERR_OK)
        {
            if(SYS_FRAM_SafeRead(t, sno + m) != SYS_ERR_OK)
            {
                continue;
            }
        }
        if((m < (k - 1)) || !(len & 0x1F))  //�ж��Ƿ������ֽ���
        {
            memcpy(data + (m << 5), t, 32);
        }
        else
        {
            memcpy(data + (m << 5), t, len & 0x1F);
        }
    }
    
    SYS_OK();
}


/************************************************************************
 * @function: Fram_SafeWrite
 * @����: ������İ�ȫ��������������д������
 *       (ʹ��ǰ����Ҫִ��open��close)
 * 
 * @����: 
 * @param: data д������ݵĻ���
 * @param: sno д������ݵ���ʼ���
 * @param: len д������ݵĳ���
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2013/10/28)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 Fram_SafeWrite(uint8* data, uint16 sno, uint16 len)
{
    uint16 k = len >> 5;                //��ȡ�ܹ���Ҫ�����ݿ���

    if(len & 0x1F)
    {
        k ++;
    }

    for(uint16 m = 0; m < k; m++)       //�ֿ�д������
    {
        if(SYS_FRAM_SafeWrite(data + (m << 5), sno + m) != SYS_ERR_OK)
        {
            SYS_FRAM_SafeWrite(data + (m << 5), sno + m);
        }
    }
    
    SYS_OK();
}



/***********************************************************
 * @function_name: SYS_FRAM_Write
 * @function_file: SYS_file.c
 * @����:д����������,buffer����������ֽ�Ԥ����CRCУ��
 * 
 * 
 * @����: 
 * @param:buffer  
 * @param:length  
 * @param:addr  
 * 
 * @����: 
 * @return: TResult 
 * @����:
 *---------------------------------------------------------
 * @�޸���: houpm (2010/2/22)
 **********************************************************/
TResult SYS_FRAM_Write(uint8* buffer, uint32 length, uint32 addr)
{
    SYS_OK();
}

/***********************************************************
 * @function_name: SYS_FRAM_Read
 * @function_file: SYS_file.c
 * @����:�����������ȡ����
 * 
 * 
 * @����: 
 * @param:buffer  
 * @param:length  
 * @param:addr  
 * 
 * @����: 
 * @return: TResult 
 * @����:
 *---------------------------------------------------------
 * @�޸���: houpm (2010/2/22)
 **********************************************************/
TResult SYS_FRAM_Read(uint8* buffer, uint32 length, uint32 addr)
{
    SYS_OK();
}



/************************************************************************
 * @function: SYS_FRAM_Init
 * @����: �����豸��ʼ��
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/6/21)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void SYS_FRAM_Init(void)
{
    //framʹ��SPI�ӿ�,�ʲ���Ҫ����������Դ.
}




#endif               //#if SYS_FLASH_FRAM_EN > 0




#endif              //#if SYS_FRAM_EN > 0


#if (SYS_EEPROM_EN > 0)

/************************************************************************
 * @function: SYS_EEPROM_Open
 * @����: ��һ��eeprom��Ŀ��д������
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
TResult SYS_EEPROM_Open(void)
{
    krhino_sem_take(&gs_EepromRes, RHINO_WAIT_FOREVER);
    guc_EepromUserTkid = krhino_cur_task_get();  //Ϊ��ǰ�û����̱��
    return SYS_ERR_OK;
}


/************************************************************************
 * @function: SYS_EEPROM_Close
 * @����: eeprom�ر�
 * 
 * @����: 
 * @param:  
 * @����: 
 * @˵��: 
 * @����: yzy (2018/5/2)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void SYS_EEPROM_Close(void)
{
    if(guc_EepromUserTkid == krhino_cur_task_get())
    {
        guc_EepromUserTkid = NULL;        //�ָ��ɳ�ʼֵ
        krhino_sem_give(&gs_EepromRes);    //�ͷ�����ź���
    }
}

/************************************************************************
 * @function: Eeprom_SafeWrite
 * @����: ��һ��eeprom��Ŀ��д������
 * 
 * @����: 
 * @param: data ��д�������
 * @param: num eeprom��Ŀ���
 * 
 * @����: 
 * @return: TResult
 * @˵��: 
 * @����: yzy (2013/10/28)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 Eeprom_SafeWrite(uint8* data, uint32 addr, uint16 length)
{
    TDevFlsOP op;                       //����һ��Flash�����ṹ
    uint8 result;
                                        //�ж��Ƿ��ѱ���������ռ��
    if(guc_EepromUserTkid != krhino_cur_task_get())
    {
        return SYS_ERR_FT;
    }
    
    op.addr = addr;                      //��ַ
    op.len = length;  //����
    op.obj = data;                      //����
    op.type = DEV_FLS_TYPE_EEPROM;       //����
    op.rw = DEV_FLS_RW_W;               //��д��ʽ
    result = DevFls_ProcMSG(&op);

    return result;                      //δ֪����
}

/************************************************************************
 * @function: Eeprom_SafeRead
 * @����: ��eeprom�ж�ȡ����
 * 
 * @����: 
 * @param: data һ��eeprom��Ŀ,��С����Ϊ32�ֽ�,����ᵼ�»������
 * @param: addr ƫ�Ƶ�ַ
 * @param: length ����
 * @����: 
 * @return: TResult  
 * @˵��: 
 * @����: yzy (2013/10/28)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 Eeprom_SafeRead(uint8* data, uint32 addr, uint16 length)
{
    TDevFlsOP op;                       //����һ��Flash�����ṹ
    uint8 result;
                                        //�ж��Ƿ��ѱ���������ռ��
    if(guc_EepromUserTkid != krhino_cur_task_get())
    {
        return SYS_ERR_FT;
    }
    
    op.addr = addr;                      //��ַ
    op.len = (uint32)length;  //����
    op.obj = data;                      //����
    op.type = DEV_FLS_TYPE_EEPROM;       //����
    op.rw = DEV_FLS_RW_R;               //��д��ʽ
    result = DevFls_ProcMSG(&op);
    
    return result;                      //δ֪����
}



/***********************************************************
 * @function_name: SYS_EEPROM_Write
 * @function_file: SYS_file.c
 * @����:д��eeprom����,buffer����������ֽ�Ԥ����CRCУ��
 * 
 * 
 * @����: 
 * @param:buffer  
 * @param:length  
 * @param:addr  
 * 
 * @����: 
 * @return: TResult 
 * @����:
 *---------------------------------------------------------
 * @�޸���: houpm (2010/2/22)
 **********************************************************/
TResult SYS_EEPROM_Write(uint8* buffer, uint32 length, uint32 addr)
{
    SYS_OK();
}

/***********************************************************
 * @function_name: SYS_FRAM_Read
 * @function_file: SYS_file.c
 * @����:��eeprom�����ȡ����
 * 
 * 
 * @����: 
 * @param:buffer  
 * @param:length  
 * @param:addr  
 * 
 * @����: 
 * @return: TResult 
 * @����:
 *---------------------------------------------------------
 * @�޸���: houpm (2010/2/22)
 **********************************************************/
TResult SYS_EEPROM_Read(uint8* buffer, uint32 length, uint32 addr)
{
    SYS_OK();
}

#endif


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
void SYS_FILE_Init(void)
{
    //flashʹ��SPI�ӿ�,�ʲ���Ҫ����������Դ.
    
    krhino_sem_create(&gs_FileRes, "flash_sem", 1);
    krhino_sem_create(&gs_EepromRes, "eeprom_sem", 1);
    guc_FileUserTkid = NULL;
    guc_EepromUserTkid = NULL;
}
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


unsigned char g_ucFileOpenFlag = 0xFF;

void SYS_FILE_Start(void)
{
    uint8_t ret = SYS_ERR_OK;
#if SYS_EEPROM_EN > 0
    Init_I2C_EEPROM(&gs_EepromPort);            //��ʼ��i2c����
#endif
#if SYS_FILE_EN > 0
    ret = Flash_Init();
#endif
    if(ret == CBB_ERR_OK)
    {
        g_ucFileOpenFlag = CON_FILE_OPEN;
    }

}


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

void SYS_FILE_Stop(void)
{
#if SYS_EEPROM_EN > 0
    I2C_EEPROM_Idel();            //��ʼ��i2c����
#endif

#if SYS_FILE_EN > 0
    Flash_Close();
#endif


}







