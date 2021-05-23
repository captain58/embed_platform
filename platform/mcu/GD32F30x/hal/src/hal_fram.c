/******************************Copyright(c)******************************
**                          
**
** File Name: hal_fram.c
** Author: 
** Date Last Update: 2015-05-18
** Description: SPI铁电驱动
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: 文件创建
*************************************************************************/
#define EXT_FRAM

#include "sys.h"
#include "hal.h"



/************************************************************************
**铁电大小
*************************************************************************/
uint32 gul_FramSize;


/************************************************************************
**铁电操作指令集
*************************************************************************/
#define FRAM_STR_WREN   0x06            //写使能
#define FRAM_STR_WRDI   0x04            //写禁能
#define FRAM_STR_RDSR   0x05            //读状态寄存器
#define FRAM_STR_WRSR   0x01            //写状态寄存器
#define FRAM_STR_READ   0x03            //读数据
#define FRAM_STR_WRITE  0x02            //写数据




/************************************************************************
**铁电操作SPI结构体
*************************************************************************/
SPIIO gs_FRAMSPI;

/************************************************************************
**安全操作字
*************************************************************************/
uint32 gul_FRAMSafeCode;

/************************************************************************
**从铁电读取的当前铁电的状态
*************************************************************************/
uint8 guc_FRAMStatus;


/************************************************************************
**将CS线拉高的函数格式
*************************************************************************/
void FRAM_csup(void)
{   
#if (SPI_IO_MACRO_EN > 0)
    SPI_CS_1_FRAM();
#else
    SPI_CS_1(SPI_DEV_ID_FRAM);
#endif
}

/************************************************************************
**将CS线拉低的函数格式
*************************************************************************/
void FRAM_csdown(void)
{
#if (SPI_IO_MACRO_EN > 0)
    SPI_CS_0_FRAM();
#else
    SPI_CS_0(SPI_DEV_ID_FRAM);
#endif
}



/************************************************************************
**获取铁电状态的结构体
*************************************************************************/
const SPIIO gs_FRAMStatusR = 
{
    {FRAM_STR_RDSR,},
    &guc_FRAMStatus,
    1,
    1,
    FRAM_csup,
    FRAM_csdown,
};


/************************************************************************
**设置铁电状态的结构体
*************************************************************************/
const SPIIO gs_FRAMStatusW = 
{
    {FRAM_STR_WRSR,},
    &guc_FRAMStatus,
    1,
    1,
    FRAM_csup,
    FRAM_csdown,
};


/************************************************************************
**设置铁电的内部写保护
*************************************************************************/
const SPIIO gs_FRAMEnableWP = 
{
    {FRAM_STR_WRDI,},
    __NULL,
    0,
    1,
    FRAM_csup,
    FRAM_csdown,
};


/************************************************************************
**禁止铁电的内部写保护
*************************************************************************/
const SPIIO gs_FRAMDisableWP = 
{
    {FRAM_STR_WREN,},
    __NULL,
    0,
    1,
    FRAM_csup,
    FRAM_csdown,
};


/************************************************************************
**获取铁电状态的宏定义
*************************************************************************/
#define FRAM_GetStatus() {SPI_Read((SPIIO*)&gs_FRAMStatusR);}

/************************************************************************
**设置铁电状态的宏定义
*************************************************************************/
#define FRAM_SetStatus() {SPI_Write((SPIIO*)&gs_FRAMStatusW);}

/************************************************************************
**禁止铁电的写保护
*************************************************************************/
void FRAM_Disable_WP(void) 
{
#if (SPI_IO_MACRO_EN > 0)
    SPI_WP_1_FRAM();
#else
    SPI_WP_1(SPI_DEV_ID_FRAM);
#endif
    SPI_Write((SPIIO*)&gs_FRAMDisableWP);
}



/************************************************************************
**使能铁电的写保护
*************************************************************************/
void FRAM_Enable_WP(void) 
{
    SPI_Write((SPIIO*)&gs_FRAMEnableWP);
#if (SPI_IO_MACRO_EN > 0)
    SPI_WP_0_FRAM();
#else
    SPI_WP_0(SPI_DEV_ID_FRAM);
#endif
}



/************************************************************************
 * @function: FRAM_Read
 * @描述: 读取铁电内部指定地址的数据
 * 
 * @参数: 
 * @param: buffer 读取数据的缓存
 * @param: address 读取数据的起始地址
 * @param: length 读取数据的长度
 * 
 * @返回: 
 * @return: TResult  
 * @说明: 
 * @作者: yzy (2014/8/22)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
TResult FRAM_Read(uint8* buffer, uint32 address, uint16 length)
{
                                        //参数验证
    SYS_VAR_CHECK(address + length > gul_FramSize);
                                        //读取指令
    gs_FRAMSPI.command[0] = FRAM_STR_READ;
                                        //读取地址操作
    if(gul_FramSize >= 0x20000)
    {
        gs_FRAMSPI.command[1] = address >> 16;
        gs_FRAMSPI.command[2] = address >> 8;
        gs_FRAMSPI.command[3] = address;
        gs_FRAMSPI.cmdnum = 4;           //命令
    }
    else
    {
        gs_FRAMSPI.command[1] = address >> 8;
        gs_FRAMSPI.command[2] = address;
        gs_FRAMSPI.cmdnum = 3;              //命令
    }
    gs_FRAMSPI.length = length;         //长度
    gs_FRAMSPI.data = buffer;           //缓存
    return SPI_Read(&gs_FRAMSPI);       //返回读取结果
}






/************************************************************************
 * @function: FRAM_PreWrite
 * @描述: 铁电写数据的预操作
 * @参数: 
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2014/8/22)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 FRAM_PreWrite(void)
{
    gul_FRAMSafeCode = 0xA5A5A5A5;
    SYS_OK();
}





/************************************************************************
 * @function: FRAM_Write
 * @描述: 写入铁电内部指定地址的数据
 * 
 * @参数: 
 * @param: buffer 写入数据的缓存
 * @param: address 写入数据的起始地址
 * @param: length 写入数据的长度
 * 
 * @返回: 
 * @return: TResult  
 * @说明: 
 * @作者: yzy (2014/8/22)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
TResult FRAM_Write(uint8* buffer, uint32 address, uint16 length)
{
    for(uint8 uc_i = 0; uc_i < 3; uc_i++)
    {
                                        //参数验证
        SYS_VAR_CHECK(address + length > gul_FramSize || address > gul_FramSize);
                                        //判断安全字
        if(gul_FRAMSafeCode != 0xA5A5A5A5)  
        {
            return FRAM_ERR_FT;         //返回发生致命错误
        }
        FRAM_Disable_WP();              //禁能写保护        
        gs_FRAMSPI.command[0] = FRAM_STR_WRITE;
                                        //写入地址操作

        if(gul_FramSize >= 0x20000)
        {
            gs_FRAMSPI.command[1] = address >> 16;
            gs_FRAMSPI.command[2] = address >> 8;
            gs_FRAMSPI.command[3] = address;
            gs_FRAMSPI.cmdnum = 4;           //命令
        }
        else
        {
            gs_FRAMSPI.command[1] = address >> 8;
            gs_FRAMSPI.command[2] = address;
            gs_FRAMSPI.cmdnum = 3;           //命令
        }
        gs_FRAMSPI.length = length;      //长度
        gs_FRAMSPI.data = buffer;        //缓存
                                         //判断安全字   
        if(gul_FRAMSafeCode == 0xA5A5A5A5)  
        {                                //写入数据
            if(SPI_Write(&gs_FRAMSPI) != SYS_ERR_OK)
            {
                FRAM_Enable_WP();        //使能写保护
                continue;                //再来一次
            }
        }
        else
        {
            return FRAM_ERR_FT;          //返回致命错误
        }
        gs_FRAMSPI.command[0] = FRAM_STR_READ;
        uint8 SPI_ReadAndCompare(SPIIO* spi);
        if(SPI_ReadAndCompare(&gs_FRAMSPI) != SYS_ERR_OK)
        {
            FRAM_Enable_WP();           //使能写保护
            continue;                   //再来一次
        }
        FRAM_Enable_WP();               //使能写保护
        gul_FRAMSafeCode = 0;           //复位安全操作字
        SYS_OK();                        //返回写入成功
    }
    gul_FRAMSafeCode = 0;               //安全操作字归零
    return FRAM_ERR_FT;                 //返回发生致命错误

}



/************************************************************************
 * @function: HAL_InitFRAM
 * @描述: 初始化铁电存储器
 * @参数: 
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2014/8/22)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 HAL_InitFRAM(void)
{
    gs_FRAMSPI.csup = FRAM_csup;        //cs up
    gs_FRAMSPI.csdown = FRAM_csdown;    //cs down
    gul_FRAMSafeCode = 0;               //初始化安全操作字
    
    uint8 mask = 0x70;                  //铁电出初始状态掩码
    gul_FramSize = 0x20000;

    FRAM_GetStatus();                   //获取状态
    if((guc_FRAMStatus & mask) != 0)    //判断FRAM是否存在
    {
        FRAM_GetStatus();               //再次获取状态
        if((guc_FRAMStatus & mask) != 0)    //判断FRAM是否存在
        {
            return FRAM_ERR_FT;         //返回发生致命错误
        }
    }
    FRAM_Disable_WP();                  //去掉写保护功能
    guc_FRAMStatus = 0x80;              //准备设置状态寄存器
    FRAM_SetStatus();                   //设置状态寄存器
    FRAM_Enable_WP();                   //重新使能写保护 
    SYS_OK();                            //返回操作成功                                                                            
}


