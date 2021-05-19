/****************************************Copyright (c)**************************************************
**                                  ________科技有限公司
**                                          开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: iap_file.c
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2010年11月11日
** 描　述: 原始版本
**         该文件需与boot文件操作对应,应用程序只负责往boot指定的flash地址写入升级文件.
**         具体的升级操作,在复位后由boot程序执行
********************************************************************************************************/
#define EXT_DEV_IAP

#include "es.h"
#include "hal.h"
#include "bsp.h"


#if SYS_IAP_EN > 0


//外部flash相关宏定义
#define FLS_PROGRAM_ADDR         (4096 + 256)       //程序在外部flash中的位置(前32字节用来存放upatemark)
#define FLS_PROGRAM_MAXLEN       524288     //程序总空间大小512K字节


#define FLS_UPDATEMARK_ADDR      0          //定义程序升级标记在Flash中的位置

/*******************************************************************************
**这两组数据用来存放远程升级的标记,从而可以让升级时的意外停电
**在复位之后得到回复
********************************************************************************/
const uint32 gul_UpdataMarkA_APP[4] = 
{
    0x12345678, 0x87654321, 0x11223344, 0x55667788
};


const uint32 gul_UpdataMarkB_APP[4] = 
{
    0,0,0,0
};



/***********************************************************
 * @function_name: SYS_IAP_Mark
 * @function_file: iap_file.c
 * @描述: 设置或者清除升级标记         
 * @参数: 
 * @param:   setorclr 设置或者清除升级标记 
 * @param:   ID       数据ID
 * @param:  length   数据长度
 * @param:  address  数据地址
 * @返回: 
 * @return:  
 * @作者:  yzy (2010-11-11)
 *******************************************************/
void SYS_APP_IAP_Mark(bool setorclr, uint32 length, uint32 address)
{
    uint8 uc_data[28];   

    if(setorclr)                        //设置标记
    {
                                        //拷贝写入标记
        memcpy(uc_data, (void*)gul_UpdataMarkA_APP, sizeof(gul_UpdataMarkA_APP));
        uc_data[16] = 0;               //数据ID
                                        //长度
        LongToByte(uc_data + 18, length);
                                        //地址
        LongToByte(uc_data + 22, address);
                                        //计算CRC校验
        IntToByte(uc_data + 26, DoCrcRtuModbus(uc_data, 26, 0xFFFF));
    }
    else                                //仅仅拷贝数据就可以了
    {
        memcpy(uc_data, (void*)gul_UpdataMarkB_APP, sizeof(gul_UpdataMarkB_APP));
    }
    
    SYS_FILE_Open();
	SYS_FILE_Write(uc_data, FLS_UPDATEMARK_ADDR, 28);
    SYS_FILE_Close();
}



/***********************************************************
 * @function_name: SYS_APP_IAP_Check
 * @function_file: iap_file.c
 * @描述: 检验Flash中升级程序的合法性         
 * @参数: 
 * @param:  startAddr  升级数据位于FLASH的起始偏移
 * @param； length 升级数据长度
 * @返回: 
 * @return: true  数据合法,将开始进行程序升级
           false  数据非法,将不能进行程序升级
 * @作者:  yzy (2010-11-11)
 *******************************************************/
bool SYS_APP_IAP_CheckCRC(uint32 startAddr, uint32 length)
{
    uint8 uc_data[64];                  //缓存用来读取数据用
    uint16 crc = 0xFFFF;                //CRC校验值
    uint16 cmp;                         //CRC校验值的对比
    
    uint32 curAdress = startAddr;		//程序起始地址
    
    if((length & 0x03) != 0)            //判断长度的合法性
    {
        return false;                   //返回错误
    }
    
    SYS_FILE_Open();
    
    while(length > 0)                   //不断的读取数据然后进行校验
    {
        if(length > 64)                //大于64字节读64字节
        {
            
            SYS_FILE_Read(uc_data, curAdress, 64);
            crc = DoCrcRtuModbus(uc_data, 64, crc);
            length -= 64;
            curAdress += 64;
        }
        else                            //否则读取剩余数据的长度
        {
            SYS_FILE_Read(uc_data, curAdress, length);
                                        //最后的4个字节是CRC校验
            crc = DoCrcRtuModbus(uc_data, length - 4, crc);
                                        //获取CRC校验的对比值
            cmp = ByteToInt(uc_data + length - 2);
            length = 0;                 //已经读取完所有的数据
        }
    }
    
    SYS_FILE_Close();
    
    if(cmp != crc)                      //判断CRC的校验值是否相等
    {
        return false;
    }
    return true;                        //返回OK        
}

/***********************************************************
 * @function_name: SYS_APP_IAP_Check
 * @function_file: iap_file.c
 * @描述: 检验Flash中升级程序的合法性         
 * @参数: 
 * @param:  db  数据块
 * @param； len 长度
 * @返回: 
 * @return: true  数据合法,将开始进行程序升级
           false  数据非法,将不能进行程序升级
 * @作者:  yzy (2010-11-11)
 *******************************************************/
bool SYS_APP_IAP_Check(uint32 length)
{
	return SYS_APP_IAP_CheckCRC(FLS_PROGRAM_ADDR, length);
}



/************************************************************************
 * @function: SYS_IAP_Ready
 * @描述: 
 * 
 * @参数: 
 * @param: address 升级起始地址
 * @param: length 用户程序需要升级的程序的长度
 * @param: isreset 升级完之后是否需要复位用户程序
 * 
 * @返回: 
 * @return: bool  true成功/false失败
 * @说明: 
 * @作者: yzy (2013/7/16)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
bool SYS_IAP_Ready(uint32 address, uint32 length, bool isreset)
{
    if(length > 0x80000)                //不支持这么长的升级工作
    {
        return false;                   //返回错误
    }
                                        //确认升级数据的合法性,若主升级和备份部分CRC都不正确才认为不正确
    if(!SYS_APP_IAP_Check(length))
    {
	    return false;                   //返回错误
    }
                                        //写入升级标志
    SYS_APP_IAP_Mark(true, length, address);
                                        //进行系统复位
    SYS_Reset();

    return true;
}

#endif                                  //SA_IAP_EN == 1

/*********************************************************************************************************
*************************************文件结束*************************************************************
**********************************************************************************************************/


