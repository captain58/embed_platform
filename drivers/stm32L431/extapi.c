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
** Description: 文件创建
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
 * @描述: 读取铁电数据,如果数据不符合CRC校验则清零读取结果
 * 
 * @参数: 
 * @param: buffer 目标缓冲区
 * @param: blocknum 起始铁电条目序号
 * @param: len 数据总字节数
 * 
 * @返回: 
 * @return: bool  数据读取成功,返回1; 读取结果不符合CRC校验,清空缓冲区,返回0
 * @说明: 
 * @作者: yzy (2013/10/28)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
bool ReadFramWithCRC(uint8* buffer, uint16 blocknum, uint16 len)
{
    SYS_FRAM_Open();
    Fram_SafeRead(buffer, blocknum, len);
    SYS_FRAM_Close();
    
    if(CheckBufferCRC(buffer, len, true)) //校验码正确 
    {
        return true;
    }
    
    memset_s(buffer, 0, len); 
    return false;
}







/************************************************************************
 * @function: WriteFramWithCRC
 * @描述: 对数据区构建CRC校验码,并保存
 * 
 * @参数: 
 * @param: buffer 目标缓冲区
 * @param: blocknum 起始铁电条目序号
 * @param: len 数据字节数
 * 
 * @返回: 
 * @return: bool  
 * @说明: 
 * @作者: yzy (2013/10/28)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
bool WriteFramWithCRC(uint8* buffer,uint16 blocknum, uint16 len)
{
    uint8 ret;
    
    BuildBufferCRC(buffer, len, true); //构建校验码
    
    SYS_FRAM_Open();
    ret = Fram_SafeWrite(buffer, blocknum, len);   //保存数据到铁电
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
 * @描述: 从Flash里读取数据,不验证校验码.重试3次
 * 
 * @参数: 
 * @param: fileid 文件号
 * @param: buffer 数据缓冲区
 * @param: len 数组长度
 * @param: start 文件里起始位置
 * 
 * @返回: 
 * @return: bool  读取成功返回true 失败返回false
 * @说明: 
 * @作者: yzy (2013/6/20)
 *-----------------------------------------------------------------------
 * @修改人: 
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

    for(uint8 i = 0; i < 3; i++)        //最多重试3次
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
 * @描述: 对数组计算校验码,写入到Flash里
 * 
 * @参数: 
 * @param: fileid 文件号
 * @param: buffer 数据缓冲区
 * @param: len 数组长度
 * @param: start 文件里起始位置
 * 
 * @返回: 
 * @return: bool  写入成功返回1;失败返回0
 * @说明: 
 * @作者: yzy (2013/6/20)
 *-----------------------------------------------------------------------
 * @修改人: 
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
 * @描述: 在一个已经打开的数据块内偏移地址起设置一个数据
 * 
 * @参数: 
 * @param: dbno 需要操作的数据块
 * @param: start 需要设置的数据长度
 * @param: len 设置的起始位置
 * @param: data 设置的数据
 * 
 * @返回: 
 * @return: uint8 SYS_ERR_OK / SYS_ERR_FT
 * @说明: 
 * @作者: yzy
 *-----------------------------------------------------------------------
 * @修改人: yzy (2011-4-1)
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
 * @描述: 从Flash里读取数据,不验证校验码.重试3次
 * 
 * @参数: 
 * @param: fileid 文件号
 * @param: buffer 数据缓冲区
 * @param: len 数组长度
 * @param: start 文件里起始位置
 * 
 * @返回: 
 * @return: bool  读取成功返回true 失败返回false
 * @说明: 
 * @作者: yzy (2013/6/20)
 *-----------------------------------------------------------------------
 * @修改人: 
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

    for(uint8 i = 0; i < 3; i++)        //最多重试3次
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
 * @描述: 对数组计算校验码,写入到Flash里
 * 
 * @参数: 
 * @param: fileid 文件号
 * @param: buffer 数据缓冲区
 * @param: len 数组长度
 * @param: start 文件里起始位置
 * 
 * @返回: 
 * @return: bool  写入成功返回1;失败返回0
 * @说明: 
 * @作者: yzy (2013/6/20)
 *-----------------------------------------------------------------------
 * @修改人: 
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
 * @描述: 从Flash里读取数据,验证校验码,不匹配则清空整个数组
 * 
 * @参数: 
 * @param: fileid 文件号
 * @param: buffer 数据缓冲区
 * @param: len 数组长度
 * @param: start 文件里起始位置
 * 
 * @返回: 
 * @return: bool  校验码正确返回1,不正确返回0
 * @说明: 
 * @作者: yzy (2013/6/20)
 *-----------------------------------------------------------------------
 * @修改人: 
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
 * @描述: 对数组计算校验码,写入到Flash里
 * 
 * @参数: 
 * @param: fileid 文件号
 * @param: buffer 数据缓冲区
 * @param: len 数组长度
 * @param: start 文件里起始位置
 * 
 * @返回: 
 * @return: bool  
 * @说明: 
 * @作者: yzy (2013/6/20)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
bool WriteFlashWithCRC(uint16 fileid, uint8* buffer, uint16 len, uint32 start)
{
    BuildBufferCRC(buffer, len, true); 
    return WriteFlash(fileid, buffer, len, start);
}





/************************************************************************
 * @function: SetFlashFrom
 * @描述: 在一个已经打开的数据块内偏移地址起设置一个数据
 * 
 * @参数: 
 * @param: dbno 需要操作的数据块
 * @param: start 需要设置的数据长度
 * @param: len 设置的起始位置
 * @param: data 设置的数据
 * 
 * @返回: 
 * @return: uint8 SYS_ERR_OK / SYS_ERR_FT
 * @说明: 
 * @作者: yzy
 *-----------------------------------------------------------------------
 * @修改人: yzy (2011-4-1)
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
 * @描述: 拷贝两个数据块之间的数据
 * 
 * @参数: 
 * @param: srcdb 源数据块
 * @param: srcaddr 源数据块起始地址
 * @param: desdb 目标数据块
 * @param: desaddr 目标数据块地址
 * @param: length 待拷贝的数据长度
 * @param: ram 拷贝时单次转移所使用的内存
 * @param: ramlen 拷贝时单次转移所使用的内存长度
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2012-9-3)
 *-----------------------------------------------------------------------
 * @修改人: 
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




//对stream.c的扩展

/************************************************************************
**函数名称: bool STR_IsFlashBitSet(uint16 fileid, uint32 start, uint16 bit)
**描 述: 判断某位是否置一
**
**输 入: 
**       fileid 数据块号
**       start 数据块内位图流起始地址
**       bit 要判断的位数(从0开始)
**输 出: 
**
**备 注: 
**-----------------------------------------------------------------------
**作 者: yzy
**日 期: 10-01-26
*************************************************************************/
bool STR_IsFlashBitSet(uint16 fileid, uint32 start, uint16 bit)
{
    uint8 byte;
                                        //读取带判断位所在的字节,若读取失败,直接退出
    if(ReadFlash(fileid, &byte, 1, start + (bit >> 3)) == false)
    {
        return false;
    }
                                        //一般性判断
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
**函数名称: bool STR_IsFlashBitClr(uint16 fileid, uint32 start, uint16 bit)
**描 述: 判断某位是否为零
**
**输 入: 
**       fileid 数据块号
**       start 数据块内位图流起始地址
**       bit 要判断的位数(从0开始)
**输 出: 
**
**备 注: 
**-----------------------------------------------------------------------
**作 者: yzy
**日 期: 10-01-26
*************************************************************************/
bool STR_IsFlashBitClr(uint16 fileid, uint32 start, uint16 bit)
{
    uint8 byte;
                                        //读取带判断位所在的字节,若读取失败,直接退出
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
 * @描述: 单位置一
 * 
 * @参数: 
 * @param: fileid 数据块号
 * @param: start 位图起始字节
 * @param: bit   设置的位索引
 * @param: mbyte 位图的最大字节数
 * 
 * @返回: 
 * @return: bool  
 * @说明: 
 * @作者: yzy (2012-9-25)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
bool STR_SetFlashBit(uint16 fileid, uint32 start, uint16 bit, uint16 mbyte)
{
    uint8 byte;
    TDataBlock db;
                                        //超限保护
    if((mbyte == 0) || ((bit >> 3) >= (mbyte - 1)))
    {
        return false;
    }
    
    if(SYS_FILE_DB_Open(fileid, &db, TDB_MODE_RW) != SYS_ERR_OK)
    {
        return false;
    }
                                        //读取带判断位所在的字节,若读取失败,直接退出
    if(SYS_FILE_DB_ReadFrom(&db, &byte, 1, start + (bit >> 3)) != SYS_ERR_OK)
    {
        SYS_FILE_DB_Close(&db); 
        return false;
    }
                                        //判断是否已经置位
    if(byte & Bit_Map8[bit & 7])
    {
        SYS_FILE_DB_Close(&db); 
        return true;
    }
                                        //置位
    byte |= Bit_Map8[bit & 7];
                                        //回写
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
 * @描述: 单位置零
 * 
 * @参数: 
 * @param: fileid 数据块号
 * @param: start 位图起始字节
 * @param: bit   清零的位索引
 * @param: mbyte 位图的最大字节数
 * 
 * @返回: 
 * @return: bool  
 * @说明: 
 * @作者: yzy (2012-9-25)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
bool STR_ClrFlashBit(uint16 fileid, uint32 start, uint16 bit, uint16 mbyte)
{
    uint8 byte;
    TDataBlock db;
                                        //超限保护
    if((mbyte == 0) || ((bit >> 3) >= (mbyte - 1)))
    {
        return false;
    }
    
    if(SYS_FILE_DB_Open(fileid, &db, TDB_MODE_RW) != SYS_ERR_OK)
    {
        return false;
    }
                                        //读取带判断位所在的字节,若读取失败,直接退出
    if(SYS_FILE_DB_ReadFrom(&db, &byte, 1, start + (bit >> 3)) != SYS_ERR_OK)
    {
        SYS_FILE_DB_Close(&db); 
        return false;
    }
                                        //判断是否已经清零
    if(!(byte & Bit_Map8[bit & 7]))
    {
        SYS_FILE_DB_Close(&db); 
        return true;
    }
                                        //清零
    byte &= ~Bit_Map8[bit & 7];
                                        //回写
    if(SYS_FILE_DB_WriteFrom(&db, &byte, 1, start + (bit >> 3)) != SYS_ERR_OK)
    {
        SYS_FILE_DB_Close(&db); 
        return false;
    }
    
    SYS_FILE_DB_Close(&db); 
    return true;
}

//对stream.c的扩展
#endif


/************************************************************************
 * @function: GD_Para_RW
 * @描述: 读或写flash参数
 * 
 * @参数: 
 * @param: addr flash偏移地址
 * @param: buff 读写缓存
 * @param: len 需要读写的长度
 * @param: rw false读; true写
 * 
 * @返回: 
 * @return: uint8  ES_ERR_OK/SYS_ERR_FT
 * @说明: 
 * @作者: yzy (2014/2/12)
 *-----------------------------------------------------------------------
 * @修改人: 
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



