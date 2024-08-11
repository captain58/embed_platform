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

#ifndef _EXTAPI_H_
#define _EXTAPI_H_

#ifndef EXT_EXTAPI
#define EXT_EXTAPI     extern
#endif








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
bool ReadFramWithCRC(uint8* buffer, uint16 blocknum, uint16 len);


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
bool WriteFramWithCRC(uint8* buffer,uint16 blocknum, uint16 len);

uint8_t ReadIFlash(uint16 fileid, uint8* buffer, uint16 len, uint32 start);
uint8_t WriteIFlash(uint16 fileid, uint8* buffer, uint16 len, uint32 start);


//	#if SYS_IFLASH_EN > 0
#define WriteFlash  WriteIFlash
#define ReadFlash   ReadIFlash
//	#else
//	/************************************************************************
//	 * @function: ReadFlash
//	 * @描述: 从Flash里读取数据,不验证校验码.重试3次
//	 * 
//	 * @参数: 
//	 * @param: fileid 文件号
//	 * @param: buffer 数据缓冲区
//	 * @param: len 数组长度
//	 * @param: start 文件里起始位置
//	 * 
//	 * @返回: 
//	 * @return: bool  读取成功返回true 失败返回false
//	 * @说明: 
//	 * @作者: yzy (2013/6/20)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
//	 ************************************************************************/
//	bool ReadFlash(uint16 fileid, uint8* buffer, uint16 len, uint32 start);
//	
//	/************************************************************************
//	 * @function: WriteFlash
//	 * @描述: 对数组计算校验码,写入到Flash里
//	 * 
//	 * @参数: 
//	 * @param: fileid 文件号
//	 * @param: buffer 数据缓冲区
//	 * @param: len 数组长度
//	 * @param: start 文件里起始位置
//	 * 
//	 * @返回: 
//	 * @return: bool  写入成功返回1;失败返回0
//	 * @说明: 
//	 * @作者: yzy (2013/6/20)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
//	 ************************************************************************/
//	bool WriteFlash(uint16 fileid, uint8* buffer, uint16 len, uint32 start);
//	
//	#endif

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
bool ReadFlashWithCRC(uint16 fileid, uint8* buffer, uint16 len, uint32 start);


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
bool WriteFlashWithCRC(uint16 fileid, uint8* buffer, uint16 len, uint32 start);



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
uint8 SetFlashFrom(uint8 dbno, uint32 start, uint16 len, uint8 data);

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
uint8 CopyFlashFrom(uint8 srcdb, uint32 srcaddr, uint8 desdb, uint32 desaddr, uint16 length, uint8* ram, uint16 ramlen);





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
bool STR_IsFlashBitSet(uint16 fileid, uint32 start, uint16 bit);


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
bool STR_IsFlashBitClr(uint16 fileid, uint32 start, uint16 bit);



/************************************************************************
 * @function: STR_SetFlashBit
 * @描述: 单位置一
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
bool STR_SetFlashBit(uint16 fileid, uint32 start, uint16 bit, uint16 mbyte);


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
bool STR_ClrFlashBit(uint16 fileid, uint32 start, uint16 bit, uint16 mbyte);




uint8 WriteE2(uint8* data, uint32_t addr, uint32_t len);
uint8 ReadE2(uint8* data, uint32_t addr, uint32_t len);
uint8 SetIFlashFrom(uint8 dbno, uint32 start, uint16 len, uint8 data);
//	uint8 GD_Para_RW(uint32 addr, uint8* buff, uint16 len, bool rw);

#endif




