/****************************************Copyright (c)**************************************************
**                                  ________科技有限公司
**                                          开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: spiio.h
**创   建   人: yzy
**最后修改日期: 2007年8月4日
**描        述: 硬件SPI驱动模块的头文件
**注        意:
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2007年8月4日
** 描　述: 原始版本
********************************************************************************************************/
#ifndef _SPIIO_H_
#define _SPIIO_H_


#ifndef EXT_SPIIO
#define EXT_SPIIO extern
#endif


/******************************************************************************
**文件操作资源
******************************************************************************/
//EXT_SPIIO ksem_t gs_SpiioRes;

//EXT_SPIIO uint8 * guc_SpiioUserTkid;        //当前占用资源的进程id,0xFF表示未被占用.

/*****************************************************************************
**定义SPI端口结构体
******************************************************************************/
//typedef struct
//{
//    COMPORT* portMISO;                  //SPI器件主入从出,输入
//    COMPORT* portMOSI;                  //SPI器件主出从入
//    COMPORT* portSCK;                   //SPI器件时钟线
//    COMPORT* portPWR;                    //SPI器件电源脚
//    COMPORT* portCS;                    //SPI器件片选线
//    COMPORT* portWP;                    //SPI器件保护线
//    COMPORT* portPWR2;                    //SPI器件电源脚
//    COMPORT* portCS2;                   //SPI器件2片选线
//    COMPORT* portWP2;                   //SPI器件2保护线
//    COMPORT* portPWR3;                    //SPI器件电源脚
//    COMPORT* portCS3;                   //SPI器件3片选线
//    COMPORT* portWP3;                   //SPI器件3保护线
//    
//}SPIIO_PORTS;



/*****************************************************************************
**定义SPI端口操作的结构体
******************************************************************************/
typedef struct 
{
    uint8  command[8];                  //命令字
    uint8_t dev;
//	    uint8_t line;   //0:4线  1：三线
    uint8* data;                        //发送数据
    uint16 length;                      //发送长度
    uint16 cmdnum;                      //命令长度
    void (*csup)(uint8_t, const SPIIO_PORTS* ports);                //CS线拉高操作
    void (*csdown)(uint8_t, const SPIIO_PORTS* ports);              //CS线拉低操作
}SPIIO;


/************************************************************************
 * @function: SPI_CS_1
 * @描述: SPI口片选脚拉高(禁能)
 * 
 * @参数: 
 * @param: devidx 设备索引(0-N),0xFF表示所有
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/6/16)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void SPI_CS_1(uint8 devidx, const SPIIO_PORTS* ports);


/************************************************************************
 * @function: SPI_CS_0
 * @描述: SPI口片选脚拉低
 * 
 * @参数: 
 * @param: devidx 设备索引(0-N)
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/6/16)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void SPI_CS_0(uint8 devidx, const SPIIO_PORTS* ports);


/************************************************************************
 * @function: SPI_WP_1
 * @描述: SPI口保护脚拉高
 * 
 * @参数: 
 * @param: devidx 设备索引(0-N)
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/6/16)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void SPI_WP_1(uint8 devidx, const SPIIO_PORTS* ports);


/************************************************************************
 * @function: SPI_WP_0
 * @描述: SPI口保护脚拉低
 * 
 * @参数: 
 * @param: devidx 设备索引(0-N)
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/6/16)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void SPI_WP_0(uint8 devidx, const SPIIO_PORTS* ports);


/************************************************************************
 * @function: SPI_Write
 * @描述: 写操作
 * 
 * @参数: 
 * @param: spi 写入数据的地址的结构信息** 
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2014/6/16)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 SPI_Write(SPIIO* spi, const SPIIO_PORTS* ports);

/************************************************************************
 * @function: SPI_Read
 * @描述: 读操作
 * 
 * @参数: 
 * @param: spi 读取数据的地址的结构信息** 
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2014/6/16)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 SPI_Read(SPIIO* spi, const SPIIO_PORTS* ports);

/************************************************************************
 * @function: SPI_Close
 * @描述: SPI进入低功耗
 * 
 * @参数: 
 * @param: devidx 设备索引(0-N)
 * 
 * @返回: 
 * @return: 
 * @说明: 
 * @作者: yzy (2018-07-02)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void SPI_Close(unsigned char devidx, const SPIIO_PORTS* ports);


/************************************************************************
 * @function: SPI_PWR_1
 * @描述: SPI设备电源脚拉高
 * 
 * @参数: 
 * @param: devidx 设备索引(0-N)
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/6/16)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void SPI_PWR_1(uint8 devidx, const SPIIO_PORTS* ports);

/************************************************************************
 * @function: SPI_PWR_0
 * @描述: SPI设备电源脚拉低
 * 
 * @参数: 
 * @param: devidx 设备索引(0-N)
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/6/16)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void SPI_PWR_0(uint8 devidx, const SPIIO_PORTS* ports);

/************************************************************************
 * @function: SPI_ReadAndCompare
 * @描述: 从器件中读取指定长度的数据和缓存里面的内容进行比较
 * 
 * @参数: 
 * @param: spi 读取数据的地址的结构信息**  
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2014/6/16)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 SPI_ReadAndCompare(SPIIO* spi, const SPIIO_PORTS* ports);

/************************************************************************
 * @function: Init_SPI
 * @描述: 初始化SPI的IO口
 * 
 * @参数: 
 * @param: ports 端口列表
 * @param: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/6/16)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void Init_SPI(const SPIIO_PORTS* ports);

#endif

