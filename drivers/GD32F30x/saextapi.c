/****************************************Copyright (c)**************************************************
**                                  ________�Ƽ����޹�˾
**                                          ������
**
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: saextapi.c
**��   ��   ��: yzy
**����޸�����: 2018��3��22��
**��        ��: Ӧ�ò���չAPI�ļ�
**ע        ��:
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: yzy
** ��  ��: v1.0
** �ա���: 2018��3��22��
** �衡��: ԭʼ�汾
**-------------------------------------------------------------------------------------------------------
** �޸���: 
** ��  ��: 
** ��  ��: 
********************************************************************************************************/
#define EXT_SAEXTAPI

#include "public.h"





extern unsigned char g_ucFileOpenFlag;
/************************************************************************
 * @function: ReadFlash
 * @����: ��Flash���ȡ����,����֤У����.����3��
 * 
 * @����: 
 * @param: fileid �ļ���
 * @param: buffer ���ݻ�����
 * @param: len ���鳤��
 * @param: start �ļ�����ʼλ��
 * 
 * @����: 
 * @return: bool  ��ȡ�ɹ�����true ʧ�ܷ���false
 * @˵��: 
 * @����: yzy (2013/6/20)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
TResult ReadFlash(uint16 fileid, uint8* buffer, uint16 len, uint32 start)
{    
    TDataBlock db;
    TResult r = SYS_ERR_OK;
    if(SYS_ERR_OK != SYS_FILE_DB_Open(fileid, &db, TDB_MODE_RW) != SYS_ERR_OK)
    {
        ClearBuffer(buffer, len);
        return SYS_ERR_FT;
    }
	
    for(uint8 i = 0; i < 3; i++)        //�������3��
    {
        r = SYS_FILE_DB_ReadFrom(&db, buffer, len, start);
        if(r == SYS_ERR_OK)
        {
            break;
        }
    }
    SYS_FILE_DB_Close(&db);
//	    if(r == SYS_ERR_OK)
//	    {
//	        return true;
//	    }
//	    else
//	    {
//	        return false;
//	    }
    return r;

}

/************************************************************************
 * @function: WriteFlashSpec
 * @����: ���������У����,д�뵽Flash��
 * 
 * @����: 
 * @param: fileid �ļ���
 * @param: buffer ���ݻ�����
 * @param: len ���鳤��
 * @param: start �ļ�����ʼλ��
 * 
 * @����: 
 * @return: bool  д��ɹ�����1;ʧ�ܷ���0
 * @˵��: 
 * @����: yzy (2013/6/20)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
int WriteFlashSpec(uint16_t fileid, uint8_t* buffer, uint16_t len, uint32_t start)
{
    TDataBlock db;
    int8_t r;
    if(len < 1)
    {
        return CBB_ERR_VAR;
    }
//	    printf("WriteFlashSpec buf head = %x len = %d\n", buffer[0], len);
    if(SYS_FILE_DB_Open(fileid, &db, TDB_MODE_RW) != CBB_ERR_OK)  
    {
        return CBB_ERR_FT;
    }

    for(uint8_t i = 0; i < 3; i++)
    {
        r = SYS_FILE_DB_WriteSpec(&db, buffer, len, start); 
        if(r == CBB_ERR_OK)
        {
            break;
        }
    }
    
    SYS_FILE_DB_Close(&db);

    return r;
}



/************************************************************************
 * @function: WriteFlash
 * @����: ���������У����,д�뵽Flash��
 * 
 * @����: 
 * @param: fileid �ļ���
 * @param: buffer ���ݻ�����
 * @param: len ���鳤��
 * @param: start �ļ�����ʼλ��
 * 
 * @����: 
 * @return: bool  д��ɹ�����1;ʧ�ܷ���0
 * @˵��: 
 * @����: yzy (2013/6/20)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
TResult WriteFlash(uint16 fileid, uint8* buffer, uint16 len, uint32 start)
{
    TDataBlock db;
    TResult r = SYS_ERR_OK;
    if(len < 1)
    {
        return SYS_ERR_FT;
    }

    if(SYS_FILE_DB_Open(fileid, &db, TDB_MODE_RW) != SYS_ERR_OK)  
    {
        return SYS_ERR_FT;
    }
	
    for(uint8 i = 0; i < 3; i++)
    {
        r = SYS_FILE_DB_WriteFrom(&db, buffer, len, start); 
        if(r == SYS_ERR_OK)
        {
            break;
        }
    }
    SYS_FILE_DB_Close(&db);
//	    if(r == SYS_ERR_OK)
//	    {
//	        return true;
//	    }
//	    else
//	    {
//	        return false;
//	    }
    return r;
}




/************************************************************************
 * @function: ReadFlashWithCRC
 * @����: ��Flash���ȡ����,��֤У����,��ƥ���������������
 * 
 * @����: 
 * @param: fileid �ļ���
 * @param: buffer ���ݻ�����
 * @param: len ���鳤��
 * @param: start �ļ�����ʼλ��
 * 
 * @����: 
 * @return: bool  У������ȷ����1,����ȷ����0
 * @˵��: 
 * @����: yzy (2013/6/20)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
TResult ReadFlashWithCRC(uint16 fileid, uint8* buffer, uint16 len, uint32 start)
{    
//	    bool bl = ReadFlash(fileid, buffer, len, start);
    if(SYS_ERR_OK != ReadFlash(fileid, buffer, len, start))
    {
        ClearBuffer(buffer, len);
        return SYS_ERR_FT;
    }

    if(CheckBufferCRC(buffer, len, true))
    {
        return SYS_ERR_OK;   
    }
    else
    {
        ClearBuffer(buffer, len);
        return SYS_ERR_FT;
    }
}



/************************************************************************
 * @function: WriteFlashWithCRC
 * @����: ���������У����,д�뵽Flash��
 * 
 * @����: 
 * @param: fileid �ļ���
 * @param: buffer ���ݻ�����
 * @param: len ���鳤��
 * @param: start �ļ�����ʼλ��
 * 
 * @����: 
 * @return: bool  
 * @˵��: 
 * @����: yzy (2013/6/20)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
TResult WriteFlashWithCRC(uint16 fileid, uint8* buffer, uint16 len, uint32 start)
{
    BuildBufferCRC(buffer, len, true); 
    return WriteFlash(fileid, buffer, len, start);
}


/************************************************************************
**��������: bool STR_IsFlashBitSet(uint16 fileid, uint32 start, uint16 bit)
**�� ��: �ж�ĳλ�Ƿ���һ
**
**�� ��: 
**       fileid ���ݿ��
**       start ���ݿ���λͼ����ʼ��ַ
**       bit Ҫ�жϵ�λ��(��0��ʼ)
**�� ��: 
**
**�� ע: 
**-----------------------------------------------------------------------
**�� ��: yzy
**�� ��: 10-01-26
*************************************************************************/
bool STR_IsFlashBitSet(uint16 fileid, uint32 start, uint16 bit)
{
    uint8 byte;
                                        //��ȡ���ж�λ���ڵ��ֽ�,����ȡʧ��,ֱ���˳�
    if(SYS_ERR_OK != ReadFlash(fileid, &byte, 1, start + (bit >> 3)))
    {
        return false;
    }
                                        //һ�����ж�
    if(byte & Bit_Map8[bit & 0x07])
    {
        return true;
    }
    else
    {
        return false;
    }
}


/************************************************************************
**��������: bool STR_IsFlashBitClr(uint16 fileid, uint32 start, uint16 bit)
**�� ��: �ж�ĳλ�Ƿ�Ϊ��
**
**�� ��: 
**       fileid ���ݿ��
**       start ���ݿ���λͼ����ʼ��ַ
**       bit Ҫ�жϵ�λ��(��0��ʼ)
**�� ��: 
**
**�� ע: 
**-----------------------------------------------------------------------
**�� ��: yzy
**�� ��: 10-01-26
*************************************************************************/
bool STR_IsFlashBitClr(uint16 fileid, uint32 start, uint16 bit)
{
    uint8 byte;
                                        //��ȡ���ж�λ���ڵ��ֽ�,����ȡʧ��,ֱ���˳�
    if(SYS_ERR_OK != ReadFlash(fileid, &byte, 1, start + (bit >> 3)))
    {
        return false;
    }
    
    if((byte & Bit_Map8[bit & 0x07]) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}




/************************************************************************
 * @function: STR_SetFlashBit
 * @����: ��λ��һ
 * 
 * @����: 
 * @param: fileid ���ݿ��
 * @param: start λͼ��ʼ�ֽ�
 * @param: bit   ���õ�λ����
 * @param: mbyte λͼ������ֽ���
 * 
 * @����: 
 * @return: bool  
 * @˵��: 
 * @����: yzy (2012-9-25)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
bool STR_SetFlashBit(uint16 fileid, uint32 start, uint16 bit, uint16 mbyte)
{
    uint8 byte;
    TDataBlock db;
                                        //���ޱ���
    if((mbyte == 0) || ((bit >> 3) >= (mbyte - 1)))
    {
        return false;
    }
    
    if(SYS_FILE_DB_Open(fileid, &db, TDB_MODE_RW) != SYS_ERR_OK)
    {
        return false;
    }
                                        //��ȡ���ж�λ���ڵ��ֽ�,����ȡʧ��,ֱ���˳�
    if(SYS_FILE_DB_ReadFrom(&db, &byte, 1, start + (bit >> 3)) != SYS_ERR_OK)
    {
        SYS_FILE_DB_Close(&db); 
        return false;
    }
                                        //�ж��Ƿ��Ѿ���λ
    if(byte & Bit_Map8[bit & 7])
    {
        SYS_FILE_DB_Close(&db); 
        return true;
    }
                                        //��λ
    byte |= Bit_Map8[bit & 7];
                                        //��д
    if(SYS_FILE_DB_WriteFrom(&db, &byte, 1, start + (bit >> 3)) != SYS_ERR_OK)
    {
        SYS_FILE_DB_Close(&db); 
        return false;
    }
    
    SYS_FILE_DB_Close(&db); 
    return true;
}



/************************************************************************
 * @function: STR_ClrFlashBit
 * @����: ��λ����
 * 
 * @����: 
 * @param: fileid ���ݿ��
 * @param: start λͼ��ʼ�ֽ�
 * @param: bit   �����λ����
 * @param: mbyte λͼ������ֽ���
 * 
 * @����: 
 * @return: bool  
 * @˵��: 
 * @����: yzy (2012-9-25)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
bool STR_ClrFlashBit(uint16 fileid, uint32 start, uint16 bit, uint16 mbyte)
{
    uint8 byte;
    TDataBlock db;
                                        //���ޱ���
    if((mbyte == 0) || ((bit >> 3) >= (mbyte - 1)))
    {
        return false;
    }
    
    if(SYS_FILE_DB_Open(fileid, &db, TDB_MODE_RW) != SYS_ERR_OK)
    {
        return false;
    }
                                        //��ȡ���ж�λ���ڵ��ֽ�,����ȡʧ��,ֱ���˳�
    if(SYS_FILE_DB_ReadFrom(&db, &byte, 1, start + (bit >> 3)) != SYS_ERR_OK)
    {
        SYS_FILE_DB_Close(&db); 
        return false;
    }
                                        //�ж��Ƿ��Ѿ�����
    if(!(byte & Bit_Map8[bit & 7]))
    {
        SYS_FILE_DB_Close(&db); 
        return true;
    }
                                        //����
    byte &= ~Bit_Map8[bit & 7];
                                        //��д
    if(SYS_FILE_DB_WriteFrom(&db, &byte, 1, start + (bit >> 3)) != SYS_ERR_OK)
    {
        SYS_FILE_DB_Close(&db); 
        return false;
    }
    
    SYS_FILE_DB_Close(&db); 
    return true;
}


/************************************************************************
 * @function: SetFlashFrom
 * @����: ��һ���Ѿ��򿪵����ݿ���ƫ�Ƶ�ַ������һ������
 * 
 * @����: 
 * @param: dbno ��Ҫ���������ݿ�
 * @param: start ��Ҫ���õ����ݳ���
 * @param: len ���õ���ʼλ��
 * @param: data ���õ�����
 * 
 * @����: 
 * @return: uint8 SA_ERR_OK / SA_ERR_FT
 * @˵��: 
 * @����: yzy
 *-----------------------------------------------------------------------
 * @�޸���: yzy (2011-4-1)
 ************************************************************************/
uint8 SetFlashFrom(uint8 dbno, uint32 start, uint16 len, uint8 data)
{
    TDataBlock db;
    
    if(SYS_FILE_DB_Open(dbno, &db, TDB_MODE_RW) != SYS_ERR_OK)
    {
        return SYS_ERR_FT;
    }
    
    SYS_FILE_DB_SetFrom(&db, start, len, data);
    SYS_FILE_DB_Close(&db);

    SYS_OK();
}

/************************************************************************
 * @function: EraseFlash
 * @����: ����
 * 
 * @����: 
 * @param: fileid �ļ���
 * @param: sectorfrom ��ʼ��������(0~)
 * @param: sectornum ��������������(�������ݿ��С���ֲ���)
 * 
 * @����: 
 * @return: bool  д��ɹ�����0;ʧ�ܷ�������
 * @˵��: 
 * @����: yzy (2013/6/20)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/


int EraseFlash(uint16_t fileid, uint32_t sectorfrom, uint32_t sectornum)
{
    TDataBlock db;
    int8_t r;
    if(sectornum < 1)
    {
        return CBB_ERR_VAR;
    }

    if(SYS_FILE_DB_Open(fileid, &db, TDB_MODE_RW) != CBB_ERR_OK)  
    {
        return CBB_ERR_FT;
    }

    for(uint8_t i = 0; i < 3; i++)
    {
        r = SYS_FILE_DB_Erase(&db, sectorfrom, sectornum); 
        if(r == CBB_ERR_OK)
        {
            break;
        }
    }
    
    SYS_FILE_DB_Close(&db);

    return r;

}

int CBB_FILE_Erase_Event (unsigned char type)
{
    int ret = 1;
    uint8_t recharge = type >> 4;
    
    if(g_ucFileOpenFlag != CON_FILE_OPEN)
    {
        return CBB_ERR_CLOSE;
    }
    switch(type & 0x0f)
    {
    case DB_FLASH:
#if SYS_FILE_EN > 0
        if(recharge)
        {
            ret = EraseFlash(DB_RECHARGE_INDEX,0,2);
            ret = EraseFlash(DB_RECHARGE,0,10);
        }
        else
        {
            ret = EraseFlash(DB_EVENT_INDEX,0,2);
            ret = EraseFlash(DB_EVENT,0,10);
        }
#endif
        break;            
    }
    return ret;

}

int CBB_FILE_Write_Event (unsigned char type, unsigned char * pdata,  int len)
{
    int ret = 1;
    uint8_t recharge = type >> 4;
    uint8_t *tmp = NULL;
    uint32_t data_len,db_area,db_index_area,data_num;
    if(g_ucFileOpenFlag != CON_FILE_OPEN)
    {
        return CBB_ERR_CLOSE;
    }
    
    switch(type & 0x0f)
    {
        case DB_FLASH:
#if CBB_FILE_EN > 0
            {
                if(recharge)
                {
                    data_len = DT_RECHARGE_DATA_LEN;
                    db_area = DB_RECHARGE;
                    db_index_area = DB_RECHARGE_INDEX;
                    data_num = DT_RECHARGE_DATA_NUM;
                }
                else
                {
                    data_len = DT_EVENT_DATA_LEN;
                    db_area = DB_EVENT;
                    db_index_area = DB_EVENT_INDEX;
                    data_num = DT_EVENT_DATA_NUM;
                }
                int i;
                tmp = aos_malloc(data_num);
                if(NULL == tmp)
                {
                    LOG_ERROR(DBGFMT"malloc err!!!\n", DBGARG);
                    return SYS_ERR_FT;
                }
                
                memset(tmp,0xff,data_num);
                if(SYS_ERR_OK == ReadFlash(db_index_area, tmp, data_num, 0))
                {
                    for(i = 0; i < data_num; i++)
                    {
                        if(tmp[i] == 0xFF)
                            break;
                    }
//                    printf("event index = %d\n", i);
//	                    ret = WriteFlash(db_area, pdata, len, data_len * i);
                    if(SYS_ERR_OK == WriteFlash(db_area, pdata, len, data_len * i))
                    {
                        //��д����ҳ
                        tmp[i] = 0xAA;
                        
                        if(i >= (data_num-1))
                        {
                            ret = EraseFlash(db_index_area,0,1);
                        }
                        else
                        {
//	                            printf("tmp = %x %x %x\n", tmp[0], tmp[1], tmp[i]);
                            ret = WriteFlashSpec(db_index_area, &tmp[i], 1, i);
                        }
                        if(ret != CBB_ERR_OK)
                        {
                            if(NULL != tmp) aos_free(tmp);
                            return ret;
                        }
                        //��д������ҳ
                        memset(tmp,0xff,data_len);
                        if(SYS_ERR_OK == ReadFlash(db_index_area, tmp, data_num, 4096))
                        {
                            if(tmp[i] == 0xFF)
                            {
                                tmp[i] = 0xAA;
                                ret = WriteFlashSpec(db_index_area, &tmp[i], 1, 4096 + i);
                            }
                        }
                        else
                        {
                            
                            ret = CBB_ERR_FT;
                        }
                    }
                }
                
                if(NULL != tmp) aos_free(tmp);
            }
#endif
            break;
        case DB_EEPROM:
#if (CBB_EEPROM_EN > 0)
            {
                int index = 0;
                int num = 0;
                ret = Eeprom_Read((uint8_t *)&index, 0, sizeof(int));
                if(ret != CBB_ERR_OK)
                {
                    return ret;
                }
                ret = Eeprom_Read((uint8_t *)&num, sizeof(int), sizeof(int));
                if(ret != CBB_ERR_OK)
                {
                    return ret;
                }
//	                if(index > 255)
//	                {}
                if(CBB_ERR_OK == Eeprom_Write(pdata, 
                    32 + DT_EVENT_DATA_LEN*((index+1)%DT_EVENT_DATA_NUM),  len))
                {
                    index = ((index+1)%DT_EVENT_DATA_NUM);
                    num = ((num+1)>DT_EVENT_DATA_NUM) ? DT_EVENT_DATA_NUM : (num+1);
                    ret = Eeprom_Write((uint8_t *)&index, 0, sizeof(int));
                    if(ret != CBB_ERR_OK)
                    {
                        return ret;
                    }
                    ret = Eeprom_Write((uint8_t *)&num, sizeof(int), sizeof(int));
                    if(ret != CBB_ERR_OK)
                    {
                        return ret;
                    }                    
                }
                else
                {
                    ret = CBB_ERR_FT;
                }
                
                
            }
#endif
            break;            
    }
    return ret;
}

int CBB_FILE_Read_Event (unsigned char type, int start, int num, unsigned char * pdata,  int * len)
{
    int ret = 1;
    int index = 0;
    int eventnum = 0;
    int bufferlen = *len;
    int readlen = 0;
    int datalen = 0;
    int i;
    uint8_t recharge = type >> 4;
    uint8_t *tmp = NULL;
    uint32_t data_len,db_area,db_index_area,data_num;
    
    if(g_ucFileOpenFlag != CON_FILE_OPEN)
    {
        return CBB_ERR_CLOSE;
    }
    switch(type & 0x0f)
    {
        case DB_FLASH:
#if CBB_FILE_EN > 0
            {
                if(recharge)
                {
                    data_len = DT_RECHARGE_DATA_LEN;
                    db_area = DB_RECHARGE;
                    db_index_area = DB_RECHARGE_INDEX;
                    data_num = DT_RECHARGE_DATA_NUM;
                }
                else
                {
                    data_len = DT_EVENT_DATA_LEN;
                    db_area = DB_EVENT;
                    db_index_area = DB_EVENT_INDEX;
                    data_num = DT_EVENT_DATA_NUM;
                }            
//	                uint8_t tmp[DT_EVENT_DATA_NUM];
                tmp = aos_malloc(data_num);
                if(NULL == tmp)
                {
                    LOG_ERROR(DBGFMT"malloc err!!!\n", DBGARG);
                    return SYS_ERR_FT;
                }
                memset(tmp,0xff,data_num);
                if(SYS_ERR_OK == ReadFlash(db_index_area, tmp, data_num, 0))
                {
                    for(i = 0; i < data_num; i++)
                    {
                        if(tmp[i] == 0xFF)
                            break;
                    }
                    index = (i + data_num - 1) % data_num;
                }
                memset(tmp,0xff,data_num);
                if(SYS_ERR_OK == ReadFlash(db_index_area, tmp, data_num, 4096))
                {
                    for(i = 0; i < data_num; i++)
                    {
                        if(tmp[i] == 0xFF)
                            break;
                    }
                    eventnum = i;
                }       
                if(eventnum == 0 || (eventnum != data_num && index >= eventnum))
                {
                    if(NULL != tmp) aos_free(tmp);
                    return CBB_ERR_EXIST;
                }
                if(num > eventnum)
                {
                    num = eventnum;
                }
                for(i=0;i<num;i++)
                {
                    if(bufferlen < ((i + 1) *data_len))
                    {
//	                        ret = CBB_ERR_VAR
//	                        break;
                        readlen = (bufferlen + data_len) - (i + 1) *data_len;
                    }
                    else
                    {
                        readlen = data_len;
                    }

                    datalen += readlen;
//	                    printf("index = %d i = %d addr = %d\n", index, i, DT_EVENT_DATA_LEN * ((index + DT_EVENT_DATA_NUM - start - i)%DT_EVENT_DATA_NUM));
                    memset(tmp,0xff,data_num);

                    ret = ReadFlash(db_area, tmp, readlen, 
                        data_len * ((index + data_num - start - i)%data_num));
                    
                    if(SYS_ERR_OK != ret || readlen != data_len)
                    {
                        memset(tmp, 0xEE, data_len);
                        //break;
                    }
                    memcpy(pdata+i*data_len, tmp, data_len);
//	                    printf("pdata = %x %x %x %x\n", pdata[0], pdata[1], pdata[2], pdata[3]);
                }
                
                if(NULL != tmp) aos_free(tmp);
            }
#endif
            break;
        case DB_EEPROM:
#if (CBB_EEPROM_EN > 0)
            {
                ret = Eeprom_Read((uint8_t *)&index, 0, sizeof(int));
                if(ret != CBB_ERR_OK)
                {
                    return ret;
                }
                ret = Eeprom_Read((uint8_t *)&eventnum, sizeof(int), sizeof(int));
                if(ret != CBB_ERR_OK)
                {
                    return ret;
                }
//                  if(index > 255)
//                  {}
                if(eventnum == 0 || (eventnum != DT_EVENT_DATA_NUM && index >= eventnum))
                {
                    return CBB_ERR_EXIST;
                }
                if(num > eventnum)
                {
                    num = eventnum;
                }
                for(i=0;i<num;i++)
                {
                    if(bufferlen < ((i + 1) *DT_EVENT_DATA_LEN))
                    {
//                          ret = CBB_ERR_VAR
//                          break;
                        readlen = (bufferlen + DT_EVENT_DATA_LEN) - (i + 1) *DT_EVENT_DATA_LEN;
                    }
                    else
                    {
                        readlen = DT_EVENT_DATA_LEN;
                    }
                    datalen += readlen;
                    ret = Eeprom_Read(pdata, 
                        32 + DT_EVENT_DATA_LEN * ((index + DT_EVENT_DATA_NUM - start - i)%DT_EVENT_DATA_NUM),  readlen);

                    if(CBB_ERR_OK != ret || readlen != DT_EVENT_DATA_LEN)
                    {
                        
                        break;
                    }
                }
                
                
            }
#endif
            break;
        default:
            ret = CBB_ERR_FT;
            break;
    }
    *len = datalen;
    return ret;

}
int CBB_FILE_Read_Event_Num (unsigned char type,  int * len, uint8_t * index)
{
    int ret = 1;
    int eventnum = 0;
    int eventindex = 0;
    
    uint8_t recharge = type >> 4;
    uint8_t *tmp = NULL;
    uint32_t db_index_area,data_num;
    
    if(g_ucFileOpenFlag != CON_FILE_OPEN)
    {
        return CBB_ERR_CLOSE;
    }
    switch(type  & 0x0f)
    {
        case DB_FLASH:
#if CBB_FILE_EN > 0
            {
//	                uint8_t tmp[DT_EVENT_DATA_NUM];
                if(recharge)
                {
                    db_index_area = DB_RECHARGE_INDEX;
                    data_num = DT_RECHARGE_DATA_NUM;
                }
                else
                {
                    db_index_area = DB_EVENT_INDEX;
                    data_num = DT_EVENT_DATA_NUM;
                }   

                int i;
                tmp = aos_malloc(data_num);
                if(NULL == tmp)
                {
                    LOG_ERROR(DBGFMT"malloc err!!!\n", DBGARG);
                    return SYS_ERR_FT;
                }
                
                memset(tmp,0xff,data_num);
                if(SYS_ERR_OK == ReadFlash(db_index_area, tmp, data_num, 4096))
                {
                    for(i = 0; i < data_num; i++)
                    {
                        if(tmp[i] == 0xFF)
                            break;
                    }
                    eventnum = i;
                    ret = CBB_ERR_OK;
                }       
                memset(tmp,0xff,data_num);
                if(SYS_ERR_OK == ReadFlash(db_index_area, tmp, data_num, 0))
                {
                    for(i = 0; i < data_num; i++)
                    {
                        if(tmp[i] == 0xFF)
                            break;
                    }
                    eventindex = i;
                    ret = CBB_ERR_OK;
                }                       
                
                if(NULL != tmp) aos_free(tmp);
            }
#endif
            break;
        case DB_EEPROM:
#if (CBB_EEPROM_EN > 0)
            {
                
                ret = Eeprom_Read((uint8_t *)&eventnum, sizeof(int), sizeof(int));
                if(ret != CBB_ERR_OK)
                {
                    return ret;
                }
                
            }
#endif
            break;
        default:
            break;
    }
    *len = eventnum;
    *index = eventindex;
    return ret;

}









