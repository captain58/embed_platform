/******************************Copyright(c)******************************
**                         
**
** File Name: extapis.c
** Author: 
** Date Last Update: 2019-11-02
** Description: 
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: �ļ�����
*************************************************************************/
#define EXT_EXTAPI

#include "sys.h"
#include "hal.h"
#include "bsp.h"
#include "apis.h"
#include "extapi.h"





#if (SYS_FRAM_EN > 0)




/************************************************************************
 * @function: ReadFramWithCRC
 * @����: ��ȡ��������,������ݲ�����CRCУ���������ȡ���
 * 
 * @����: 
 * @param: buffer Ŀ�껺����
 * @param: blocknum ��ʼ������Ŀ���
 * @param: len �������ֽ���
 * 
 * @����: 
 * @return: bool  ���ݶ�ȡ�ɹ�,����1; ��ȡ���������CRCУ��,��ջ�����,����0
 * @˵��: 
 * @����: yzy (2013/10/28)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
bool ReadFramWithCRC(uint8* buffer, uint16 blocknum, uint16 len)
{
    SYS_FRAM_Open();
    Fram_SafeRead(buffer, blocknum, len);
    SYS_FRAM_Close();
    
    if(CheckBufferCRC(buffer, len, true)) //У������ȷ 
    {
        return true;
    }
    
    memset_s(buffer, 0, len); 
    return false;
}







/************************************************************************
 * @function: WriteFramWithCRC
 * @����: ������������CRCУ����,������
 * 
 * @����: 
 * @param: buffer Ŀ�껺����
 * @param: blocknum ��ʼ������Ŀ���
 * @param: len �����ֽ���
 * 
 * @����: 
 * @return: bool  
 * @˵��: 
 * @����: yzy (2013/10/28)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
bool WriteFramWithCRC(uint8* buffer,uint16 blocknum, uint16 len)
{
    uint8 ret;
    
    BuildBufferCRC(buffer, len, true); //����У����
    
    SYS_FRAM_Open();
    ret = Fram_SafeWrite(buffer, blocknum, len);   //�������ݵ�����
    SYS_FRAM_Close();
    
    if(ret == SYS_ERR_FT)
    {
        return false;
    }
    else
    {
        return true;
    }
}
#endif
#if SYS_E2_EN > 0
uint8 WriteE2(uint8* data, uint32_t addr, uint32_t len)
{
    return SYS_E2_SafeWrite(data, addr, len);
}

uint8 ReadE2(uint8* data, uint32_t addr, uint32_t len)
{
    return SYS_E2_SafeRead(data, addr, len);
}

#endif
#if SYS_IFLASH_EN > 0
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
uint8_t ReadIFlash(uint16 fileid, uint8* buffer, uint16 len, uint32 start)
{    
    TDataBlock db;
    TResult r;
    if(SYS_FILE_DB_Open(fileid, &db, TDB_MODE_RW) != SYS_ERR_OK)
    {
        memset(buffer, 0, len);
        return 2;
    }

    for(uint8 i = 0; i < 3; i++)        //�������3��
    {
        r = SYS_IFILE_DB_ReadFrom(&db, buffer, len, start);
        if(r == SYS_ERR_OK)
        {
            break;
        }
    }
    
    SYS_FILE_DB_Close(&db);
    if(r == SYS_ERR_OK)
    {
        return 0;
    }
    else
    {
        return 2;
    }

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
uint8_t WriteIFlash(uint16 fileid, uint8* buffer, uint16 len, uint32 start)
{
    TDataBlock db;
    TResult r;
    if(len < 1)
    {
        return 2;
    }

    if(SYS_FILE_DB_Open(fileid, &db, TDB_MODE_RW) != SYS_ERR_OK)  
    {
        return 2;
    }

    for(uint8 i = 0; i < 3; i++)
    {
        r = SYS_IFILE_DB_WriteFrom(&db, buffer, len, start); 
        if(r == SYS_ERR_OK)
        {
            break;
        }
    }
    
    SYS_FILE_DB_Close(&db);
    if(r == SYS_ERR_OK)
    {
        return 0;
    }
    else
    {
        return 2;
    }
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
 * @return: uint8 SYS_ERR_OK / SYS_ERR_FT
 * @˵��: 
 * @����: yzy
 *-----------------------------------------------------------------------
 * @�޸���: yzy (2011-4-1)
 ************************************************************************/
uint8 SetIFlashFrom(uint8 dbno, uint32 start, uint16 len, uint8 data)
{
    TDataBlock db;
    
    if(SYS_FILE_DB_Open(dbno, &db, TDB_MODE_RW) != SYS_ERR_OK)
    {
        return SYS_ERR_FT;
    }
    
    SYS_IFILE_DB_SetFrom(&db, start, len, data);
    SYS_FILE_DB_Close(&db);

    SYS_OK();
}

#endif
#if (SYS_FLASH_EN > 0)
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
bool ReadFlash(uint16 fileid, uint8* buffer, uint16 len, uint32 start)
{    
    TDataBlock db;
    TResult r;
    if(SYS_FILE_DB_Open(fileid, &db, TDB_MODE_RW) != SYS_ERR_OK)
    {
        memset_s(buffer, 0, len);
        return false;
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
    if(r == SYS_ERR_OK)
    {
        return true;
    }
    else
    {
        return false;
    }

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
bool WriteFlash(uint16 fileid, uint8* buffer, uint16 len, uint32 start)
{
    TDataBlock db;
    TResult r;
    if(len < 1)
    {
        return false;
    }

    if(SYS_FILE_DB_Open(fileid, &db, TDB_MODE_RW) != SYS_ERR_OK)  
    {
        return false;
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
    if(r == SYS_ERR_OK)
    {
        return true;
    }
    else
    {
        return false;
    }
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
bool ReadFlashWithCRC(uint16 fileid, uint8* buffer, uint16 len, uint32 start)
{    
    bool bl = ReadFlash(fileid, buffer, len, start);
    if(!bl)
    {
        memset_s(buffer, 0, len);
        return false;
    }

    if(CheckBufferCRC(buffer, len, true))
    {
        return true;   
    }
    else
    {
        memset_s(buffer, 0, len);
        return false;
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
bool WriteFlashWithCRC(uint16 fileid, uint8* buffer, uint16 len, uint32 start)
{
    BuildBufferCRC(buffer, len, true); 
    return WriteFlash(fileid, buffer, len, start);
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
 * @return: uint8 SYS_ERR_OK / SYS_ERR_FT
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
 * @function: CopyFlashFrom
 * @����: �����������ݿ�֮�������
 * 
 * @����: 
 * @param: srcdb Դ���ݿ�
 * @param: srcaddr Դ���ݿ���ʼ��ַ
 * @param: desdb Ŀ�����ݿ�
 * @param: desaddr Ŀ�����ݿ��ַ
 * @param: length �����������ݳ���
 * @param: ram ����ʱ����ת����ʹ�õ��ڴ�
 * @param: ramlen ����ʱ����ת����ʹ�õ��ڴ泤��
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2012-9-3)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 CopyFlashFrom(uint8 srcdb, uint32 srcaddr, uint8 desdb, uint32 desaddr, uint16 length, uint8* ram, uint16 ramlen)
{
    uint32 offset = 0;

    while(length)
    {
        if(length >= ramlen)
        {
            if(true == ReadFlash(srcdb, ram, ramlen, srcaddr + offset))
            {
                WriteFlash(desdb, ram, ramlen, desaddr + offset);
            }
            length -= ramlen;
            offset += ramlen;
        }
        else
        {
            if(true == ReadFlash(srcdb, ram, length, srcaddr + offset))
            {
                WriteFlash(desdb, ram, length, desaddr + offset);
            }
            length -= length;
            offset += length;
        }
    }

    SYS_OK();

}




//��stream.c����չ

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
    if(ReadFlash(fileid, &byte, 1, start + (bit >> 3)) == false)
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
    if(ReadFlash(fileid, &byte, 1, start + (bit >> 3)) == false)
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

//��stream.c����չ
#endif


/************************************************************************
 * @function: GD_Para_RW
 * @����: ����дflash����
 * 
 * @����: 
 * @param: addr flashƫ�Ƶ�ַ
 * @param: buff ��д����
 * @param: len ��Ҫ��д�ĳ���
 * @param: rw false��; trueд
 * 
 * @����: 
 * @return: uint8  ES_ERR_OK/SYS_ERR_FT
 * @˵��: 
 * @����: yzy (2014/2/12)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	uint8 GD_Para_RW(uint32 addr, uint8* buff, uint16 len, bool rw)
//	{
//	    if(rw)
//	    {
//	        if(SYS_ERR_OK == WriteFlash(DB_PARA, buff, len, addr))
//	        {
//	            SYS_OK();
//	        }
//	    }
//	    else
//	    {
//	        if(SYS_ERR_OK == ReadFlash(DB_PARA, buff, len, addr))
//	        {
//	            SYS_OK();
//	        }
//	    }
//	    
//	    return SYS_ERR_FT;
//	}



