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

//#include "bsp_common.h"


#include "hal.h"


#if (SPI_IO_MACRO_EN == 0)
//说明: 本系统只允许有一组SPI口,可下挂多个设备,根据不同片选脚区分设备.

/************************************************************************
*SPI口端口全局结构体.
************************************************************************/
//const SPIIO_PORTS* gsp_halSpiioPorts;


/************************************************************************
*SPI口操作宏
************************************************************************/
//	#define SPI_GET_DIN()   ((gsp_halSpiioPorts->portMISO->gpio[gsp_halSpiioPorts->portMISO->pingrp].PIN >> gsp_halSpiioPorts->portMISO->pinnum) & 1)
//#define SPI_GET_DIN()    (hal_gpio_input_get_stt(gsp_halSpiioPorts->portMISO))
//
////	#define SPI_DOUT_1()    {gsp_halSpiioPorts->portMOSI->gpio[gsp_halSpiioPorts->portMOSI->pingrp].SET |= 1UL << gsp_halSpiioPorts->portMOSI->pinnum;}
////	#define SPI_DOUT_1()   {Chip_GPIO_SetPinDIROutput(gsp_halSpiioPorts->portMOSI->pingrp, gsp_halSpiioPorts->portMOSI->pinnum);}
////	
////	#define SPI_DOUT_0()    {gsp_halSpiioPorts->portMOSI->gpio[gsp_halSpiioPorts->portMOSI->pingrp].CLR |= 1UL << gsp_halSpiioPorts->portMOSI->pinnum;}
//#define SPI_DOUT_1()    {hal_gpio_output_high(gsp_halSpiioPorts->portMOSI);}
//#define SPI_DOUT_0()    {hal_gpio_output_low(gsp_halSpiioPorts->portMOSI);}
//
//
////	#define SPI_SCK_1()     {gsp_halSpiioPorts->portSCK->gpio[gsp_halSpiioPorts->portSCK->pingrp].SET |= 1UL << gsp_halSpiioPorts->portSCK->pinnum;}
////	#define SPI_SCK_0()     {gsp_halSpiioPorts->portSCK->gpio[gsp_halSpiioPorts->portSCK->pingrp].CLR |= 1UL << gsp_halSpiioPorts->portSCK->pinnum;}
//#define SPI_SCK_1()    {hal_gpio_output_high(gsp_halSpiioPorts->portSCK);}
//#define SPI_SCK_0()    {hal_gpio_output_low(gsp_halSpiioPorts->portSCK);}

//与上述代码功能相同,当上述代码速度更快
#define SPI_SET_IOIN()   (HAL_GPIO_PinConfig(gsp_halSpiioPorts->portMISO))
#define SPI_SET_IOOUT()   (HAL_GPIO_PinConfig(gsp_halSpiioPorts->portMOSI))

#define SPI_GET_DIN()   (HAL_GPIO_GetPinState(gsp_halSpiioPorts->portMISO, gsp_halSpiioPorts->portMISO->pinnum))
#define SPI_DOUT_1()    {HAL_GPIO_SetPinState(gsp_halSpiioPorts->portMOSI, gsp_halSpiioPorts->portMOSI->pinnum, true);}
#define SPI_DOUT_0()    {HAL_GPIO_SetPinState(gsp_halSpiioPorts->portMOSI, gsp_halSpiioPorts->portMOSI->pinnum, false);}
#define SPI_SCK_1()     {HAL_GPIO_SetPinState(gsp_halSpiioPorts->portSCK, gsp_halSpiioPorts->portSCK->pinnum, true);}
#define SPI_SCK_0()     {HAL_GPIO_SetPinState(gsp_halSpiioPorts->portSCK, gsp_halSpiioPorts->portSCK->pinnum, false);}





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
void SPI_CS_1(uint8 devidx, const SPIIO_PORTS* ports)
{
    COMPORT* cpt;
    SPIIO_PORTS* gsp_halSpiioPorts = ports;
    if(devidx == 0)
    {
        cpt = gsp_halSpiioPorts->portCS;
    }
    else if(devidx == 1)
    {
        cpt = gsp_halSpiioPorts->portCS2;
    }
    else if(devidx == 2)
    {
        cpt = gsp_halSpiioPorts->portCS3;
    }
    else
    {
        return;
    }
    
    if(cpt != __NULL)
    {
//        hal_gpio_output_high(cpt);
        HAL_GPIO_SetPinState(cpt, cpt->pinnum, 1);
//	        HAL_GPIO_SetPinState(cpt->pingrp, cpt->pinnum, true);
    }
}


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
void SPI_CS_0(uint8 devidx, const SPIIO_PORTS* ports)
{
    COMPORT* cpt;
    SPIIO_PORTS* gsp_halSpiioPorts = ports;

    if(devidx == 0)
    {
        cpt = gsp_halSpiioPorts->portCS;
    }
    else if(devidx == 1)
    {
        cpt = gsp_halSpiioPorts->portCS2;
    }
    else if(devidx == 2)
    {
        cpt = gsp_halSpiioPorts->portCS3;
    }
    else
    {
        return;
    }
    
    if(cpt != __NULL)
    {
//        hal_gpio_output_low(cpt);
        HAL_GPIO_SetPinState(cpt, cpt->pinnum, 0);
    }
}



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
void SPI_WP_1(uint8 devidx, const SPIIO_PORTS* ports)
{
    COMPORT* cpt;
    SPIIO_PORTS* gsp_halSpiioPorts = ports;

    if(devidx == 0)
    {
        cpt = gsp_halSpiioPorts->portWP;
    }
    else if(devidx == 1)
    {
        cpt = gsp_halSpiioPorts->portWP2;
    }
    else if(devidx == 2)
    {
        cpt = gsp_halSpiioPorts->portWP3;
    }
    else
    {
        return;
    }
    
    if(cpt != __NULL)
    {
//        hal_gpio_output_high(cpt);
        HAL_GPIO_SetPinState( cpt, cpt->pinnum, 1);
    }
}



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
void SPI_WP_0(uint8 devidx, const SPIIO_PORTS* ports)
{
    COMPORT* cpt;
    SPIIO_PORTS* gsp_halSpiioPorts = ports;

    if(devidx == 0)
    {
        cpt = gsp_halSpiioPorts->portWP;
    }
    else if(devidx == 1)
    {
        cpt = gsp_halSpiioPorts->portWP2;
    }
    else if(devidx == 2)
    {
        cpt = gsp_halSpiioPorts->portWP3;
    }
    else
    {
        return;
    }
    
    if(cpt != __NULL)
    {
//        hal_gpio_output_low(cpt);
        HAL_GPIO_SetPinState( cpt, cpt->pinnum, 0);
    }
}

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
void SPI_PWR_1(uint8 devidx, const SPIIO_PORTS* ports)
{
    COMPORT* cpt;    
    SPIIO_PORTS* gsp_halSpiioPorts = ports;

    if(devidx == 0)
    {
        cpt = gsp_halSpiioPorts->portPWR;
    }
    else if(devidx == 1)
    {
        cpt = gsp_halSpiioPorts->portPWR2;
    }
    else if(devidx == 2)
    {
        cpt = gsp_halSpiioPorts->portPWR3;
    }
    else
    {
        return;
    }
    
    if(cpt != __NULL)
    {
//        hal_gpio_output_high(cpt);    
        HAL_GPIO_SetPinState( cpt, cpt->pinnum, 1);
    }
}



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
void SPI_PWR_0(uint8 devidx, const SPIIO_PORTS* ports)
{
    COMPORT* cpt;
    SPIIO_PORTS* gsp_halSpiioPorts = ports;

    if(devidx == 0)
    {
        cpt = gsp_halSpiioPorts->portPWR;
    }
    else if(devidx == 1)
    {
        cpt = gsp_halSpiioPorts->portPWR2;
    }
    else if(devidx == 2)
    {
        cpt = gsp_halSpiioPorts->portPWR3;
    }
    else
    {
        return;
    }
    
    if(cpt != __NULL)
    {
//        hal_gpio_output_low(cpt);    
        HAL_GPIO_SetPinState( cpt, cpt->pinnum, 0);
    }
}


#endif





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
void _SPI_SendByte(uint8 byte, const SPIIO_PORTS* ports)
{
    SPIIO_PORTS* gsp_halSpiioPorts = ports;

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
uint8 _SPI_ReceiveByte(const SPIIO_PORTS* ports)
{
    uint8 byte = 0;
    SPIIO_PORTS* gsp_halSpiioPorts = ports;
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
uint8 SPI_Write(SPIIO* spi, const SPIIO_PORTS* ports)
{
    uint16 ui_i;                         //循环变量
    SPIIO_PORTS* gsp_halSpiioPorts = ports;
    SYS_VAR_CHECK(spi->cmdnum > 8);    
    SPI_SCK_0();
    spi->csdown(spi->dev, ports);                      //cs线被拉低
                                        //循环发送指令和器件内部地址
    for(ui_i = 0; ui_i < spi->cmdnum; ui_i ++)
    {                                   //发送指令和地址
        _SPI_SendByte(spi->command[ui_i], ports);
    }
                                        //循环逐个接收字节
    for(ui_i = 0; ui_i < spi->length; ui_i ++)
    {                                        
        _SPI_SendByte(spi->data[ui_i], ports); //逐个接收字节        
    }
    spi->csup(spi->dev, ports);                        //将CS线拉高
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
uint8 SPI_Read(SPIIO* spi, const SPIIO_PORTS* ports)
{
    uint16 ui_i;                         //循环变量
    SPIIO_PORTS* gsp_halSpiioPorts = ports;
    SYS_VAR_CHECK(spi->cmdnum > 8);    
    SPI_SCK_0();
    spi->csdown(spi->dev, ports);                      //cs线被拉低
                                        //循环发送指令和器件内部地址
    for(ui_i = 0; ui_i < spi->cmdnum; ui_i ++)
    {                                   //发送指令和地址
        _SPI_SendByte(spi->command[ui_i], ports);
    }
                                        //循环逐个接收字节
    if(ports->type== 1)    
    {
        SPI_SET_IOIN();
    }
    for(ui_i = 0; ui_i < spi->length; ui_i ++)
    {
                                        //逐个接收字节
        spi->data[ui_i] = _SPI_ReceiveByte(ports);   
    }
    if(ports->type == 1)    
    {
        SPI_SET_IOOUT();
    }
    spi->csup(spi->dev, ports);                        //将CS线拉高
    if(ports->cmp)
        return SPI_ReadAndCompare(spi, ports);     //返回读出比较的结果
    else
        return 0;
}

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
void SPI_Close(unsigned char devidx, const SPIIO_PORTS* ports)
{
    SPIIO_PORTS* gsp_halSpiioPorts = ports;
    if(devidx == 0)
    {
        SPI_SCK_0();
        SPI_DOUT_0();
//	        _SPI_SCK_Down();
//	        _SPI_DOUT_Down();
    }
    else if(devidx == 1)
    {
    }
    else if(devidx == 2)
    {
    }
    else
    {
        return;
    }
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
uint8 SPI_ReadAndCompare(SPIIO* spi, const SPIIO_PORTS* ports)
{
    uint16 ui_i;
    SPIIO_PORTS* gsp_halSpiioPorts = ports;
    SYS_VAR_CHECK(spi->cmdnum > 8);    
    SPI_SCK_0();
    spi->csdown(spi->dev, gsp_halSpiioPorts);                      //cs线被拉低
                                        //循环发送指令和器件内部地址
    for(ui_i = 0; ui_i < spi->cmdnum; ui_i ++)
    {                                   //发送指令和地址
        _SPI_SendByte(spi->command[ui_i], ports);
    }   
                                        //循环逐个接收字节
    if(ports->type == 1)    
    {
        SPI_SET_IOIN();
    }                                        
    for(ui_i = 0; ui_i < spi->length; ui_i ++)
    {
                                        //逐个接收字节
        if(spi->data[ui_i] != _SPI_ReceiveByte(ports))
        {
            spi->csup(spi->dev, gsp_halSpiioPorts);                //将CS线拉高
            return SYS_ERR_FT;
        }
    }
    if(ports->type == 1)    
    {
        SPI_SET_IOOUT();
    }    
    spi->csup(spi->dev, gsp_halSpiioPorts);                        //将CS线拉高
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
    COMPORT * lp;  
                                            //初始化SPI所属的各GPIO端口
    for(uint8 uc_i = 0; uc_i < (sizeof(SPIIO_PORTS) / sizeof(COMPORT*) - 1); uc_i++)
    {
        lp = *((COMPORT**)ports + uc_i);
        
        if(lp != __NULL)                    //定义存在
        {
            HAL_GPIO_PinConfig(lp);
//	            Chip_GPIO_SetPinDigit(lp->pingrp, lp->pinnum);
//	            
//	            Chip_GPIO_SetPinPFSEG(lp->pingrp, lp->pinnum, lp->pinseg);
//	            Chip_GPIO_SetPinOutputMode(lp->pingrp, lp->pinnum, lp->modefunc & 0x01);
//	            
//	            if(lp->dir)                     //输出
//	            {
//	                Chip_GPIO_SetPinDIROutput(lp->pingrp, lp->pinnum);
//	                
//	            }
//	            else                            //输入
//	            {
//	                Chip_GPIO_SetPinDIRInput(lp->pingrp, lp->pinnum);
//	            }
        }
    }
    
#if (SPI_IO_MACRO_EN == 0)
//    gsp_halSpiioPorts = ports;              //赋值全局结构体
#endif
                                            //创建用户资源,供spi总线设备使用
//	    guc_SpiioUserTkid = NULL;
//	    SYS_SEM_Create(1, &gs_SpiioRes);
    
}








