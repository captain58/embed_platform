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
#define EXT_DEV_FILE

#include "sys.h"
#include "hal.h"
#include "bsp.h"
#include <string.h>

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
#ifndef __NO_SYS__
TESRes gs_FileRes;
uint8 guc_SpiioUserTkid;

#endif
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
 * @�޸���: yzy (2010/2/20)
 **********************************************************/
uint8 DevFls_ProcMSG(TDevFlsOP* op)
{
    uint8 result = SYS_ERR_FT;

#if SYS_IFLASH_EN == 1

    if(op->type == DEV_FLS_TYPE_IFLS)     //����Flash
    {
       switch(op->rw)
       {
           case DEV_FLS_RW_R:           //������
               result = HAL_IFLASH_Read((uint8*)op->obj, op->addr, op->len);
               break;
           case DEV_FLS_RW_W:           //д����
//	               FLS_PreWrite();
               result = HAL_IFLASH_Write((uint8*)op->obj, op->addr, op->len);
               break;
#ifndef __NO_SYS__           
           case DEV_FLS_RW_S:           //���ò���
//	               FLS_PreWrite();
               result = HAL_IFLASH_Set((uint8)(uint32)op->obj, op->addr, op->len);
               break;
           case DEV_FLS_RW_E:           //��������
//	               FLS_PreWsrite();
               result = HAL_IFLASH_Erase(op->addr, op->len);
               break;
           case DEV_FLS_RW_WS:          //Flash����д�����
//	               FLS_PreWrite();
               result = HAL_IFLASH_WriteSpec((uint8*)op->obj, op->addr, op->len, 1);
               break;
#endif           
       }
       return result;
    }
#endif        
#if SYS_FLASH_EN == 1

    if(op->type == DEV_FLS_TYPE_FLS)     //����Flash
    {
       switch(op->rw)
       {
           case DEV_FLS_RW_R:           //������
               result = FLS_Read((uint8*)op->obj, op->addr, op->len);
               break;
           case DEV_FLS_RW_W:           //д����
               FLS_PreWrite();
               result = FLS_Write((uint8*)op->obj, op->addr, op->len);
               break;
           case DEV_FLS_RW_S:           //���ò���
               FLS_PreWrite();
               result = FLS_Set((uint8)(uint32)op->obj, op->addr, op->len);
               break;
           case DEV_FLS_RW_E:           //��������
               FLS_PreWrite();
               result = FLS_Erase(op->addr, op->len);
               break;
           case DEV_FLS_RW_WS:          //Flash����д�����
               FLS_PreWrite();
               result = FLS_WriteSpec((uint8*)op->obj, op->addr, op->len);
               break;
       }
       return result;
    }
#endif    
#if SYS_E2_EN == 1
    
    if(op->type == DEV_FLS_TYPE_E2)//����e2
    {
       switch(op->rw)
       {
           case DEV_FLS_RW_R:           //������
               result = SYS_E2_Read((uint8*)op->obj, op->addr, op->len);
               break;
           case DEV_FLS_RW_W:           //д����
//	               FRAM_PreWrite();
               result = SYS_E2_Write((uint8*)op->obj, op->addr, op->len);
               break;
       }
       return result;
    }
    
#endif

#if SYS_FRAM_EN > 0
#if SYS_FLASH_FRAM_EN == 0

    if(op->type == DEV_FLS_TYPE_FRAM)//����FRAM
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
       return result;
    }

#else

    if(op->type == DEV_FLS_TYPE_VFRAM)//����VFRAM
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
        
        return result;
    }
#endif
#endif
    return result;

    
}
#if (SYS_E2_EN > 0)


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
TResult SYS_E2_Open(void)
{
//	    SYS_SEM_Wait(&gs_FramRes, 0);
//	    guc_FramUserTkid = gs_TkTcbCur->tkid;  //Ϊ��ǰ�û����̱��
    
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
 * @����: yzy (2013/8/15)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void SYS_E2_Close(void)
{
//	    if(guc_FramUserTkid == gs_TkTcbCur->tkid)
//	    {
//	        guc_FramUserTkid = 0xFF;        //�ָ��ɳ�ʼֵ
//	        SYS_SEM_Release(&gs_FramRes);    //�ͷ�����ź���
//	    }
}

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
uint8 SYS_E2_SafeWrite(uint8* data, uint32_t addr, uint32_t len)
{
    TDevFlsOP op;                       //����һ��Flash�����ṹ
    uint8 result;
#ifndef __NO_SYS__    
                                        //�ж��Ƿ��ѱ���������ռ��
    if(guc_FramUserTkid != gs_TkTcbCur->tkid)
    {
        return SYS_ERR_FT;
    }
#endif    
    op.addr = addr;                      //��ַ
    op.len = len;  //����(����������Ϣ�ṹ��)
    op.obj = data;                      //����
    op.type = DEV_FLS_TYPE_E2;       //����
    op.rw = DEV_FLS_RW_W;               //��д��ʽ
    result = DevFls_ProcMSG(&op);

    return result;                      //δ֪����
}

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
uint8 SYS_E2_SafeRead(uint8* data, uint32_t addr, uint32_t len)
{
    TDevFlsOP op;                       //����һ��Flash�����ṹ
    uint8 result;
#ifndef __NO_SYS__    
    
                                        //�ж��Ƿ��ѱ���������ռ��
    if(guc_FramUserTkid != gs_TkTcbCur->tkid)
    {
        return SYS_ERR_FT;
    }
#endif    
    op.addr = addr;                      //��ַ
    op.len = len;  //����(����������Ϣ�ṹ��)
    op.obj = data;                      //����
    op.type = DEV_FLS_TYPE_E2;       //����
    op.rw = DEV_FLS_RW_R;               //��д��ʽ
    result = DevFls_ProcMSG(&op);
    
    return result;                      //δ֪����
}

//#include "stdlib.h"
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
uint32 SYS_E2_Test(void)
{
    uint8 cmp1[DEF_EE_BUF];                          //ԭʼ����
    uint8 cmp2[DEF_EE_BUF];                          //�޸ĺ�����д������
    uint8 cmp3[DEF_EE_BUF];                          //�޸ĺ�ʵ��д������
    uint32 err = 0;
#ifndef __NO_SYS__
                                        //�ж��Ƿ��ѱ���������ռ��
    if(guc_FileUserTkid != gs_TkTcbCur->tkid)
    {
        return SYS_ERR_FT;
    }
#endif    
                                            //ִ�в���
    for(int k = 0; k < DEF_EE_SIZE/DEF_EE_BUF; k++)
    {
                                            //��ȡԭʼ����
        if(SYS_E2_SafeRead(cmp1, k*DEF_EE_BUF, DEF_EE_BUF) != SYS_ERR_OK)
        {
            err = k+1;
            break;
        }
                                            //�޸�ԭʼ����
        for(uint8 i = 0; i < DEF_EE_BUF; i++)
        {
            cmp2[i] = cmp1[i] + i;
        }
                                            //д������д������
        if(SYS_E2_SafeWrite(cmp2, k*DEF_EE_BUF, DEF_EE_BUF) != SYS_ERR_OK)
        {
            err = k+1;
            break;
        }
                                            //��ȡʵ��д������
        if(SYS_E2_SafeRead(cmp3, k*DEF_EE_BUF, DEF_EE_BUF) != SYS_ERR_OK)
        {
            err = k+1;
            break;
        }
        
        SYS_E2_SafeWrite(cmp1, k*DEF_EE_BUF, DEF_EE_BUF);   //�ָ�ԭʼֵ
        if(memcmp(cmp2, cmp3, DEF_EE_BUF) != 0)   //�ȶ�����ʵ��ֵ
        {
            err = k+1;
            break;
        }
    }
    
    return err;
}





#endif               //#if SYS_FLASH_FRAM_EN > 0





/***********************************************************
 * @function_name: SYS_FILE_Open
 * @function_file: es_file.c
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
void SYS_FILE_Open(void)
{
#ifndef __NO_SYS__
    SYS_SEM_Wait(&gs_FileRes, 0);        //������Դ
    guc_FileUserTkid = gs_TkTcbCur->tkid;  //Ϊ��ǰ�û����̱��
#endif    
}

/***********************************************************
 * @function_name: SYS_FILE_Close
 * @function_file: es_file.c
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
#ifndef __NO_SYS__
    if(guc_FileUserTkid == gs_TkTcbCur->tkid)
    {
        guc_FileUserTkid = 0xFF;        //�ָ��ɳ�ʼֵ
        SYS_SEM_Release(&gs_FileRes);    //�ͷ�����ź���
    }
#endif    
}



/***********************************************************
 * @function_name: SYS_FILE_DB_Open
 * @function_file: es_file.c
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
 * @�޸���: yzy (2010/2/21)
 **********************************************************/
TResult SYS_FILE_DB_Open(uint16_t user, uint16_t ID, TDataBlock* db, TDB_MODE RW)
{
    uint32 start;
    uint16 nblock;
    uint16 k;
    uint16_t id_level = 0;
    uint32_t err = 0;
    SYS_VAR_CHECK(user >= TDB_PERMS_USER && ((ID % 0x100) < 8));
    //�������ݲ�����ı���
    id_level = ID >> 8;
    start = 0;
    nblock = 0;
    for(k = 0; k < LEN_DB_LIST; k++)
    {
        if((gss_FileBlocks[k].dbno + gss_FileBlocks[k].sml) < (id_level << 8))
        {
            continue;
        }
    
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
        if((gss_FileBlocks[k].dbno + gss_FileBlocks[k].sml) >= ((id_level + 1)<< 8))
        {
            err = SYS_ERR_FT;
            break;
        }
        
    }
    
    if(k == LEN_DB_LIST || err != 0)
    {
        return SYS_ERR_FT;
    }
                                        //
    if((gss_FileBlocks[k].atr & RW) != RW)
    {
        return SYS_ERR_TYPE;
    }
                                        //
    ASSERT(start < 0x000FFFFF);         //ȷ������12λ�����uint32_max.
    if(id_level == 0)
    {
        db->start = DB_LEN_BLOCK(0, start);
        db->length = DB_LEN_BLOCK(0, nblock);
    }
    else
    {
        db->start = DB_LEN_BLOCK(1, start);
        db->length = DB_LEN_BLOCK(1, nblock);
    }
    db->RW = gss_FileBlocks[k].atr;
    db->ID = ID;
    
//	    if((db->start == 0) && (nblock > 1))//�������ݿ��׸�block�������updatemark
//	    {
//	        db->start = DB_LEN_BLOCK(1);
//	        nblock -= 1;                    //ʵ��������һ��block
//	    }
                                        //������Դ
    SYS_FILE_Open();
    
    SYS_OK();
}


/***********************************************************
 * @function_name: SYS_FILE_DB_Close
 * @function_file: es_file.c
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
 * @�޸���: yzy (2010/2/21)
 **********************************************************/
void SYS_FILE_DB_Close(TDataBlock* db)
{
    SYS_FILE_Close();
}
#if SYS_IFLASH_EN > 0

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
 * @�޸���: yzy (2010/2/22)
 **********************************************************/
TResult _SYS_IFLS_Write(uint8* buffer, uint32 addr, uint32 length)
{
    TDevFlsOP op;                       //����һ��Flash�����ṹ
    uint8 result;
#ifndef __NO_SYS__    
                                        //�ж��Ƿ��ѱ���������ռ��
    if(guc_FileUserTkid != gs_TkTcbCur->tkid)
    {
        return SYS_ERR_FT;
    }
#endif    
    op.addr = addr;                     //��ַ
    op.len = length;                    //����
    op.obj = buffer;                    //����
    op.type = DEV_FLS_TYPE_IFLS;         //����
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
 * @�޸���: yzy (2010/2/21)
 **********************************************************/
TResult _SYS_IFLS_Read(uint8* buffer, uint32 addr, uint32 length)
{
    TDevFlsOP op;                       //����һ��Flash�����ṹ
    uint8 result;
#ifndef __NO_SYS__     
                                        //�ж��Ƿ��ѱ���������ռ��
    if(guc_FileUserTkid != gs_TkTcbCur->tkid)
    {
        return SYS_ERR_FT;
    }
#endif    
    op.addr = addr;                     //��ַ
    op.len = length;                    //����
    op.obj = buffer;                    //����
    op.type = DEV_FLS_TYPE_IFLS;         //����
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
//#include "stdlib.h"
#define DEF_IFLS_BUF          512
#define DEF_IFLS_SIZE         0x40000

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
uint32 SYS_IFLS_Test(void)
{
    uint8 cmp1[DEF_IFLS_BUF];                          //ԭʼ����
    uint8 cmp2[DEF_IFLS_BUF];                          //�޸ĺ�����д������
    uint8 cmp3[DEF_IFLS_BUF];                          //�޸ĺ�ʵ��д������
    uint32 err = 0;
    SYS_FILE_Open();
#ifndef __NO_SYS__

    //�ж��Ƿ��ѱ���������ռ��
    if(guc_FileUserTkid != gs_TkTcbCur->tkid)
    {
        return SYS_ERR_FT;
    }
#endif    
                                            //ִ�в���
    for(int k = 256; k < DEF_IFLS_SIZE/DEF_IFLS_BUF; k++)
    {
                                            //��ȡԭʼ����
        if(_SYS_IFLS_Read(cmp1, k*DEF_IFLS_BUF, DEF_IFLS_BUF) != SYS_ERR_OK)
        {
            err = k+1;
            break;
        }
                                            //�޸�ԭʼ����
        for(int i = 0; i < DEF_IFLS_BUF; i++)
        {
            cmp2[i] = cmp1[i] + i;
        }
                                            //д������д������
        if(_SYS_IFLS_Write(cmp2, k*DEF_IFLS_BUF, DEF_IFLS_BUF) != SYS_ERR_OK)
        {
            err = k+1;
            break;
        }
                                            //��ȡʵ��д������
        if(_SYS_IFLS_Read(cmp3, k*DEF_IFLS_BUF, DEF_IFLS_BUF) != SYS_ERR_OK)
        {
            err = k+1;
            break;
        }
        
        _SYS_IFLS_Write(cmp1, k*DEF_IFLS_BUF, DEF_IFLS_BUF);   //�ָ�ԭʼֵ
        if(memcmp(cmp2, cmp3, DEF_IFLS_BUF) != 0)   //�ȶ�����ʵ��ֵ
        {
            err = k+1;
            break;
        }
    }
    SYS_FILE_Close();
    return err;
}


/***********************************************************
 * @function_name: SYS_IFILE_DB_ReadFrom
 * @function_file: es_file.c
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
 * @�޸���: yzy (2010/2/21)
 **********************************************************/
TResult SYS_IFILE_DB_ReadFrom(TDataBlock* db, uint8* buffer, uint32 length, uint32 addr)
{
    TDevFlsOP op;                       //����һ��Flash�����ṹ
    uint8 result;
    
    if(!(db->RW & TDB_MODE_R))          //��Ҫ֧�ֶ����Բ������ȡ
    {
        return SYS_ERR_TYPE;
    }
#ifndef __NO_SYS__    
                                        //�ж��Ƿ��ѱ���������ռ��
    if(guc_FileUserTkid != gs_TkTcbCur->tkid)
    {
        return SYS_ERR_FT;
    }
#endif
    op.addr = db->start + addr;         //��ַ
    op.len = length;                    //����
    op.obj = buffer;                    //����
    op.type = DEV_FLS_TYPE_IFLS;         //����
    op.rw = DEV_FLS_RW_R;               //��д��ʽ

    result = DevFls_ProcMSG(&op);

    return result;
}
/***********************************************************
 * @function_name: SYS_IFILE_DB_WriteFrom
 * @function_file: es_file.c
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
 * @�޸���: yzy (2010/2/22)
 **********************************************************/
TResult SYS_IFILE_DB_WriteFrom(TDataBlock* db, uint8* buffer, uint32 length, uint32 addr)
{
    if(!(db->RW & TDB_MODE_W))          //��Ҫ֧�ֶ����Բ������ȡ
    {
        return SYS_ERR_TYPE;
    }
                                        //ȷ�ϲ����Ƿ���ȷ
    SYS_VAR_CHECK(db->length < addr + length);
#ifndef __NO_SYS__    
                                        //�ж��Ƿ��ѱ���������ռ��
    if(guc_FileUserTkid != gs_TkTcbCur->tkid)
    {
        return SYS_ERR_FT;
    }
#endif
    TDevFlsOP op;                       //����һ��Flash�����ṹ
    uint8 result;

    op.addr = db->start + addr;         //��ַ
    op.len = length;                    //����
    op.obj = buffer;                    //����
    op.type = DEV_FLS_TYPE_IFLS;         //����
    op.rw = DEV_FLS_RW_W;               //��д��ʽ

    result = DevFls_ProcMSG(&op);
    
    return result; 
}
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
TResult SYS_IFILE_DB_SetFrom(TDataBlock* db, uint32 addr, uint32 length, uint8 templet)
{
    if(!(db->RW & TDB_MODE_W))          //��Ҫ֧�ֶ����Բ������ȡ
    {
        return SYS_ERR_TYPE;
    }
                                        //ȷ�ϲ����Ƿ���ȷ
    SYS_VAR_CHECK(db->length < addr + length);
                                        //�ж��Ƿ��ѱ���������ռ��
#ifndef __NO_SYS__                                         
    if(guc_FileUserTkid != gs_TkTcbCur->tkid)
    {
        return SYS_ERR_FT;
    }
#endif
    TDevFlsOP op;                       //����һ��Flash�����ṹ
    uint8 result;

    op.addr = db->start + addr;         //��ַ
    op.len = length;                    //����
    op.obj = (Object)templet;           //����
    op.type = DEV_FLS_TYPE_IFLS;         //����
    op.rw = DEV_FLS_RW_S;               //��д��ʽ

    result = DevFls_ProcMSG(&op);
    
    return result;
}
/***********************************************************
 * @function_name: SYS_IFILE_DB_Erase
 * @function_file: es_file.c
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
 * @�޸���: yzy (2010/2/22)
 **********************************************************/
TResult SYS_IFILE_DB_Erase(TDataBlock* db, uint32 sectorfrom, uint32 sectornum)
{
    TDevFlsOP op;                       //����һ��Flash�����ṹ
    uint8 result;
    uint32 dbsectors;                   //���ݿ����������������
                                        //ȷ�ϲ����Ƿ���ȷ
    SYS_VAR_CHECK(sectorfrom > sectornum);
    SYS_VAR_CHECK(sectornum == 0);
    
                                        //�ж��Ƿ��ѱ���������ռ��
#ifndef __NO_SYS__                                         
    if(guc_FileUserTkid != gs_TkTcbCur->tkid)
    {
        return SYS_ERR_FT;
    }
#endif    
                                        //����������
    dbsectors = (db->length >> 9);
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

    op.addr = db->start + (sectorfrom << 9);//Ҫ�������������ݿ��ڵľ��Ե�ַ
    op.len = sectornum;                 //������������
    op.obj = __NULL;
    op.type = DEV_FLS_TYPE_IFLS;
    op.rw = DEV_FLS_RW_E;
    
    result = DevFls_ProcMSG(&op);
    
    return result;
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
TResult SYS_IFILE_DB_WriteSpec(TDataBlock* db, uint8* buffer, uint32 length, uint32 addr)
{
    if(!(db->RW & TDB_MODE_W))          //��Ҫ֧��д���Բ������ȡ
    {
        return SYS_ERR_TYPE;
    }
                                        //ȷ�ϲ����Ƿ���ȷ
    SYS_VAR_CHECK(db->length < addr + length);
    
                                        //�ж��Ƿ��ѱ���������ռ��
#ifndef __NO_SYS__                                         
    if(guc_FileUserTkid != gs_TkTcbCur->tkid)
    {
        return SYS_ERR_FT;
    }
#endif
    TDevFlsOP op;                       //����һ��Flash�����ṹ
    uint8 result;

    op.addr = db->start + addr;         //��ַ
    op.len = length;                    //����
    op.obj = buffer;                    //����
    op.type = DEV_FLS_TYPE_IFLS;         //����
    op.rw = DEV_FLS_RW_WS;              //��д��ʽ

    result = DevFls_ProcMSG(&op);
    
    return result;
}


/***********************************************************
 * @function_name: SYS_FILE_DB_WriteWithCRC
 * @function_file: es_file.c
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
 * @�޸���: yzy (2010/2/22)
 **********************************************************/
TResult SYS_IFILE_DB_WriteWithCRC(TDataBlock* db, uint8* buffer, uint32 length, uint32 addr)
{
    BuildBufferCRC(buffer, length, true);
    return SYS_IFILE_DB_WriteFrom(db, buffer, length, addr);
}


/***********************************************************
 * @function_name: SYS_FILE_DB_WriteWithBackup
 * @function_file: es_file.c
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
 * @�޸���: yzy (2010/2/22)
 **********************************************************/
TResult SYS_IFILE_DB_WriteWithBackup(TDataBlock* db, uint8* buffer, uint32 length, uint32 addr)
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
    r1 = SYS_IFILE_DB_WriteWithCRC(db, buffer, length, addr);
    r2 = SYS_IFILE_DB_WriteWithCRC(db, buffer, length, bkaddr);
    
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


/***********************************************************
 * @function_name: SYS_FILE_DB_ReadWithCRC
 * @function_file: es_file.c
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
 * @�޸���: yzy (2010/2/22)
 **********************************************************/
TResult SYS_IFILE_DB_ReadWithCRC(TDataBlock* db, uint8* buffer, uint32 length, uint32 addr)
{
    TResult r;
    r = SYS_IFILE_DB_ReadFrom(db, buffer, length, addr);

    if(ER(r))
    {
        if(!CheckBufferCRC(buffer, length, true))
        {
            memset_s(buffer, 0x00, length);
            r = SYS_ERR_CHK;
        }
    }
    return r;
}

/***********************************************************
 * @function_name: SYS_FILE_DB_ReadWithBackup
 * @function_file: es_file.c
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
 * @�޸���: yzy (2010/2/22)
 **********************************************************/
TResult SYS_IFILE_DB_ReadWithBackup(TDataBlock* db, uint8* buffer, uint32 length, uint32 addr)
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
    r = SYS_IFILE_DB_ReadWithCRC(db, buffer, length, addr);

    if(!ER(r))                          //�����������ȡ������
    {
        r = SYS_IFILE_DB_ReadWithCRC(db, buffer, length, bkaddr);
        if(ER(r))
        {
            return SYS_ERR_MAIN;         //��������,����������ȷ
        }
    }
    return r;                           //����д����

}

#endif


#if SYS_FLASH_EN > 0
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
 * @�޸���: yzy (2010/2/22)
 **********************************************************/
TResult SYS_FILE_Write(uint8* buffer, uint32 addr, uint32 length)
{
    TDevFlsOP op;                       //����һ��Flash�����ṹ
    uint8 result;
                                        //�ж��Ƿ��ѱ���������ռ��
    if(guc_FileUserTkid != gs_TkTcbCur->tkid)
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
 * @�޸���: yzy (2010/2/21)
 **********************************************************/
TResult SYS_FILE_Read(uint8* buffer, uint32 addr, uint32 length)
{
    TDevFlsOP op;                       //����һ��Flash�����ṹ
    uint8 result;
                                        //�ж��Ƿ��ѱ���������ռ��
    if(guc_FileUserTkid != gs_TkTcbCur->tkid)
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
    if(guc_FileUserTkid != gs_TkTcbCur->tkid)
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
        if(memcmp(cmp2, cmp3, 5) != 0)   //�ȶ�����ʵ��ֵ
        {
            err |= Bit_Map8[k];
            break;
        }
    }
    
    return err;
}

/***********************************************************
 * @function_name: SYS_FILE_DB_ReadFrom
 * @function_file: es_file.c
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
 * @�޸���: yzy (2010/2/21)
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
    if(guc_FileUserTkid != gs_TkTcbCur->tkid)
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
 * @function_file: es_file.c
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
 * @�޸���: yzy (2010/2/22)
 **********************************************************/
TResult SYS_FILE_DB_ReadWithCRC(TDataBlock* db, uint8* buffer, uint32 length, uint32 addr)
{
    TResult r;
    r = SYS_FILE_DB_ReadFrom(db, buffer, length, addr);

    if(ER(r))
    {
        if(!CheckBufferCRC(buffer, length, true))
        {
            memset_s(buffer, 0x00, length);
            r = SYS_ERR_CHK;
        }
    }
    return r;
}

/***********************************************************
 * @function_name: SYS_FILE_DB_ReadWithBackup
 * @function_file: es_file.c
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
 * @�޸���: yzy (2010/2/22)
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
 * @function_file: es_file.c
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
 * @�޸���: yzy (2010/2/22)
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
    if(guc_FileUserTkid != gs_TkTcbCur->tkid)
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
TResult SYS_FILE_DB_SetFrom(TDataBlock* db, uint32 addr, uint32 length, uint8 templet)
{
    if(!(db->RW & TDB_MODE_W))          //��Ҫ֧�ֶ����Բ������ȡ
    {
        return SYS_ERR_TYPE;
    }
                                        //ȷ�ϲ����Ƿ���ȷ
    SYS_VAR_CHECK(db->length < addr + length);
                                        //�ж��Ƿ��ѱ���������ռ��
    if(guc_FileUserTkid != gs_TkTcbCur->tkid)
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
 * @function_file: es_file.c
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
 * @�޸���: yzy (2010/2/22)
 **********************************************************/
TResult SYS_FILE_DB_WriteWithCRC(TDataBlock* db, uint8* buffer, uint32 length, uint32 addr)
{
    BuildBufferCRC(buffer, length, true);
    return SYS_FILE_DB_WriteFrom(db, buffer, length, addr);
}


/***********************************************************
 * @function_name: SYS_FILE_DB_WriteWithBackup
 * @function_file: es_file.c
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
 * @�޸���: yzy (2010/2/22)
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
    if(guc_FileUserTkid != gs_TkTcbCur->tkid)
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
 * @function_file: es_file.c
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
 * @�޸���: yzy (2010/3/3)
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
 * @function_file: es_file.c
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
 * @�޸���: yzy (2010/2/22)
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
    if(guc_FileUserTkid != gs_TkTcbCur->tkid)
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
#ifndef __NO_SYS__
    SYS_SEM_Create(1, &gs_FileRes);
#endif
}












