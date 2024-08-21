/****************************************Copyright (c)**************************************************
**                                  ________科技有限公司
**                                          开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: spiio.c
**创   建   人: yzy
**最后修改日期: 2007年8月4日
**描        述: 硬件SPI驱动模块
**注        意:
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2007年8月4日
** 描　述: 原始版本
********************************************************************************************************/
#define EXT_SPIIO

#include "sys.h"
#include "hal.h"






/************************************************************************
 * @function: _SPI_SendByte
 * @描述: 发送一个字节到SPI总线
 * 
 * @参数: 
 * @param: byte 要发送的字节
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/6/16)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void _SPI_SendByte(uint8 byte)
{
    if(byte & 0x80)                 //SPI从MSB开始发送数据
    {
        SPI_DOUT_1();               //根据最高位的情况进行置位和清零
    }
    else
    {
        SPI_DOUT_0();               //发送低电平
    }
    SPI_SCK_1();                    //上升沿              
    SPI_SCK_0();                    //下降沿   
    if(byte & 0x40)                 //SPI从MSB开始发送数据
    {
        SPI_DOUT_1();               //根据最高位的情况进行置位和清零
    }
    else
    {
        SPI_DOUT_0();               //发送低电平
    }
    SPI_SCK_1();                    //上升沿              
    SPI_SCK_0();                    //下降沿   
    if(byte & 0x20)                 //SPI从MSB开始发送数据
    {
        SPI_DOUT_1();               //根据最高位的情况进行置位和清零
    }
    else
    {
        SPI_DOUT_0();               //发送低电平
    }
    SPI_SCK_1();                    //上升沿              
    SPI_SCK_0();                    //下降沿   
    if(byte & 0x10)                 //SPI从MSB开始发送数据
    {
        SPI_DOUT_1();               //根据最高位的情况进行置位和清零
    }
    else
    {
        SPI_DOUT_0();               //发送低电平
    }
    SPI_SCK_1();                    //上升沿              
    SPI_SCK_0();                    //下降沿   
    if(byte & 0x08)                 //SPI从MSB开始发送数据
    {
        SPI_DOUT_1();               //根据最高位的情况进行置位和清零
    }
    else
    {
        SPI_DOUT_0();               //发送低电平
    }
    SPI_SCK_1();                    //上升沿              
    SPI_SCK_0();                    //下降沿   
    if(byte & 0x04)                 //SPI从MSB开始发送数据
    {
        SPI_DOUT_1();               //根据最高位的情况进行置位和清零
    }
    else
    {
        SPI_DOUT_0();               //发送低电平
    }
    SPI_SCK_1();                    //上升沿              
    SPI_SCK_0();                    //下降沿   

    if(byte & 0x02)                 //SPI从MSB开始发送数据
    {
        SPI_DOUT_1();               //根据最高位的情况进行置位和清零
    }
    else
    {
        SPI_DOUT_0();               //发送低电平
    }
    SPI_SCK_1();                    //上升沿              
    SPI_SCK_0();                    //下降沿   
                                    // 
    if(byte & 0x01)                 //SPI从MSB开始发送数据
    {
        SPI_DOUT_1();               //根据最高位的情况进行置位和清零
    }
    else
    {
        SPI_DOUT_0();               //发送低电平
    }
    SPI_SCK_1();                    //上升沿              
    SPI_SCK_0();                    //下降沿   

}



/************************************************************************
 * @function: _SPI_ReceiveByte
 * @描述: 从SPI总线上接收一个字节
 * @参数: 
 * 
 * @返回: 
 * @return: uint8  接收到的一个字节
 * @说明: 
 * @作者: yzy (2014/6/16)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 _SPI_ReceiveByte(void)
{
    uint8 byte = 0;
    
    SPI_SCK_1();
    if(SPI_GET_DIN())
    {
        byte |= 0x80;
    }
    SPI_SCK_0();        

    SPI_SCK_1();
    if(SPI_GET_DIN())
    {
        byte |= 0x40;
    }
    SPI_SCK_0();

    SPI_SCK_1();
    if(SPI_GET_DIN())
    {
        byte |= 0x20;
    }
    SPI_SCK_0();

    SPI_SCK_1();
    if(SPI_GET_DIN())
    {
        byte |= 0x10;
    }
    SPI_SCK_0();

    SPI_SCK_1();
    if(SPI_GET_DIN())
    {
        byte |= 0x08;
    }
    SPI_SCK_0();

    SPI_SCK_1();
    if(SPI_GET_DIN())
    {
        byte |= 0x04;
    }
    SPI_SCK_0();

    SPI_SCK_1();
    if(SPI_GET_DIN())
    {
        byte |= 0x02;
    }
    SPI_SCK_0();

    SPI_SCK_1();
    if(SPI_GET_DIN())
    {
        byte |= 0x01;
    }
    SPI_SCK_0();
    return byte;
}





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
uint8 SPI_Write(SPIIO* spi)
{
    uint16 ui_i;                         //循环变量
    SYS_VAR_CHECK(spi->cmdnum > 8);    
    SPI_SCK_0();
    spi->csdown();                      //cs线被拉低
                                        //循环发送指令和器件内部地址
    for(ui_i = 0; ui_i < spi->cmdnum; ui_i ++)
    {                                   //发送指令和地址
        _SPI_SendByte(spi->command[ui_i]);
    }
                                        //循环逐个接收字节
    for(ui_i = 0; ui_i < spi->length; ui_i ++)
    {                                        
        _SPI_SendByte(spi->data[ui_i]); //逐个接收字节        
    }
    spi->csup();                        //将CS线拉高
    SYS_OK();
}



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
uint8 SPI_Read(SPIIO* spi)
{
    uint16 ui_i;                         //循环变量
    SYS_VAR_CHECK(spi->cmdnum > 8);    
    SPI_SCK_0();
    spi->csdown();                      //cs线被拉低
                                        //循环发送指令和器件内部地址
    for(ui_i = 0; ui_i < spi->cmdnum; ui_i ++)
    {                                   //发送指令和地址
        _SPI_SendByte(spi->command[ui_i]);
    }
                                        //循环逐个接收字节
    for(ui_i = 0; ui_i < spi->length; ui_i ++)
    {
                                        //逐个接收字节
        spi->data[ui_i] = _SPI_ReceiveByte();   
    }
    spi->csup();                        //将CS线拉高
    return SPI_ReadAndCompare(spi);     //返回读出比较的结果
}


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
uint8 SPI_ReadAndCompare(SPIIO* spi)
{
    uint16 ui_i;
    SYS_VAR_CHECK(spi->cmdnum > 8);    
    SPI_SCK_0();
    spi->csdown();                      //cs线被拉低
                                        //循环发送指令和器件内部地址
    for(ui_i = 0; ui_i < spi->cmdnum; ui_i ++)
    {                                   //发送指令和地址
        _SPI_SendByte(spi->command[ui_i]);
    }   
                                        //循环逐个接收字节
    for(ui_i = 0; ui_i < spi->length; ui_i ++)
    {
                                        //逐个接收字节
        if(spi->data[ui_i] != _SPI_ReceiveByte())
        {
            spi->csup();                //将CS线拉高
            return SYS_ERR_FT;
        }
    }
    spi->csup();                        //将CS线拉高
    SYS_OK();
}




/************************************************************************
 * @function: Init_SPI
 * @描述: 初始化SPI的IO口
 * 
 * @参数: 
 * @param: ports 端口列表
 * @param: portnum 端口数量
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/6/16)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void Init_SPI(const SPIIO_PORTS* ports)
{
    COMPORT* lp;  
                                            //初始化SPI所属的各GPIO端口
    for(uint8 uc_i = 0; uc_i < (sizeof(SPIIO_PORTS) / sizeof(COMPORT*)); uc_i++)
    {
        lp = *((COMPORT**)ports + uc_i);
        
        if(lp != __NULL)                    //定义存在
        {
            HAL_GPIO_PinConfig(lp->port, lp->pin, lp->mode);
        }
    }
                                            //创建用户资源,供spi总线设备使用
    SYS_SEM_Create(1, &gs_SpiioRes);
}








