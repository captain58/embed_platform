/******************************Copyright(c)******************************
**                          
**
** File Name: mxflash.c
** Author: 
** Date Last Update: 2015-05-18
** Description:  Flash驱动程序
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: 文件创建
*************************************************************************/

#ifndef _IFLASH_H_
#define _IFLASH_H_
/*******************************************************************************
** @function_name:  HAL_IFLASH_Read
** @function_file:  mxflash.c
** @描述: 从Flash指定地址读取指定长度的字节数据
** 
** 
** @参数: 
** @param: buffer(uint8*): 数据缓存
** @param: address(uint32): 地址
** @param: length(uint16): 读取的数据长度
** 
** @返回: 
** @return:  uint8   
** @作者: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @修改人: 
** @修改说明: 
*******************************************************************************/
uint8 HAL_IFLASH_Read(uint8* buffer, uint32_t address, uint16_t length);

/*******************************************************************************
** @function_name:  HAL_IFLASH_Write
** @function_file:  mxflash.c
** @描述: Flash 写操作
** 
** 
** @参数: 
** @param: buffer(uint8*): 待写入数据的存放缓存
** @param: address(uint32): flash片内地址
** @param: length(uint16): 数据长度
** @param: x(uint8): 第几片flash
** @param: sdmode(uint8): 扇区数据保留模式.[0.保留扇区所有(默认),1.保留扇区位于待写数据之前的数据,2.删除扇区所有]
** 
** @返回: 
** @return:  uint8   
** @作者: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @修改人: yzy(20111-01-10)
** @修改说明: 更改函数实现方式,简化程序
*******************************************************************************/
uint8 HAL_IFLASH_Write(uint8* buffer, uint32 addr, uint16 length);
/*******************************************************************************
** @function_name:  HAL_IFLASH_Set
** @function_file:  Iflash.c
** @描述: 将Flash某块区域置为特定值 
** 
** 
** @参数: 
** @param: templet(uint8): 置入的数据
** @param: address(uint32): 写入数据的地址
** @param: length(uint16): 写入数据的长度
** @param: x(uint8): flash芯片编号
** 
** @返回: 
** @return:  uint8   
** @作者: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @修改人: yzy
** @修改说明: 操作机制同 WriteFlsInChip()
*******************************************************************************/
uint8 HAL_IFLASH_Set(uint8 templet, uint32 addr, uint16 length);
/*******************************************************************************
** @function_name:  HAL_IFLASH_Erase
** @function_file:  Iflash.c
** @描述: MX Flash 檫除操作
** 
** 
** @参数: 
** @param: address: 起始地址
** @param: sectornum 扇区数量
** 
** @返回: 
** @return:  uint8   
** @作者: yzy (2010-05-10)
**-----------------------------------------------------------------------------
** @修改人: 
** @修改说明: 
*******************************************************************************/
uint8 HAL_IFLASH_Erase(uint32 address, uint32 sectornum);

/*******************************************************************************
** @function_name:  HAL_IFLASH_WriteSpec
** @function_file:  Iflash.c
** @描述: Flash 写操作
** 
** 
** @参数: 
** @param: buffer(unsigned char*): 待写入数据的存放缓存
** @param: address(unsigned long): flash片内地址
** @param: length(unsigned short): 数据长度
** @param: sdmode(unsigned char): 扇区数据保留模式.[0.保留扇区所有(默认),1.保留扇区位于待写数据之前的数据,2.删除扇区所有]
** 
** @返回: 
** @return:  unsigned char   
** @作者: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @修改人: yzy(20111-01-10)
** @修改说明: 更改函数实现方式,简化程序
*******************************************************************************/
unsigned char HAL_IFLASH_WriteSpec(unsigned char* buffer, unsigned long addr, unsigned short length, unsigned char sdmode);

uint8 HAL_IFlash_Init(void);

#endif
