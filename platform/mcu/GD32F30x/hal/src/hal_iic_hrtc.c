/******************************Copyright(c)******************************
**                          
**
** File Name: hal_iic_hrtc.c
** Author: 
** Date Last Update: 2019-11-02
** Description:  iic硬件时钟驱动
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: 文件创建
*************************************************************************/
#define EXT_IIC_HRTC

#include "sys.h"
#include "hal.h"


/************************************************************************
*从器件地址
************************************************************************/
#define I2C_RTC_ID  0x64                   //I2C从器件地址



/************************************************************************
*硬件时钟IIC口端口全局结构体.
************************************************************************/
const IIC_PORTS* gsp_halHrtcIICPorts;



/************************************************************************
*硬件时钟IIC口操作宏
************************************************************************/
#define I2C_HRTC_GETSDA()    (HAL_GPIO_GetPinState(gsp_halHrtcIICPorts->portSDA->port, gsp_halHrtcIICPorts->portSDA->pin))

#define I2C_HRTC_SDAIN()    {HAL_GPIO_SetPinDIR(gsp_halHrtcIICPorts->portSDA->port, gsp_halHrtcIICPorts->portSDA->pin, false);}
#define I2C_HRTC_SDAOUT()   {HAL_GPIO_SetPinDIR(gsp_halHrtcIICPorts->portSDA->port, gsp_halHrtcIICPorts->portSDA->pin, true);}
#define I2C_HRTC_SCKOUT()   {HAL_GPIO_SetPinDIR(gsp_halHrtcIICPorts->portSCK->port, gsp_halHrtcIICPorts->portSCK->pin, true);}

#define I2C_HRTC_SDA_1()    {HAL_GPIO_SetPinState(gsp_halHrtcIICPorts->portSDA->port, gsp_halHrtcIICPorts->portSDA->pin, true);}
#define I2C_HRTC_SDA_0()    {HAL_GPIO_SetPinState(gsp_halHrtcIICPorts->portSDA->port, gsp_halHrtcIICPorts->portSDA->pin, false);}

#define I2C_HRTC_SCK_1()    {HAL_GPIO_SetPinState(gsp_halHrtcIICPorts->portSCK->port, gsp_halHrtcIICPorts->portSCK->pin, true);}
#define I2C_HRTC_SCK_0()    {HAL_GPIO_SetPinState(gsp_halHrtcIICPorts->portSCK->port, gsp_halHrtcIICPorts->portSCK->pin, false);}

    
    
    

/*******************************************************************************
 * @function_name:  _DELAY
 * @function_file:  IIC_HRTC.C
 * @描述: 延时
 * 
 * 
 * @参数: 
 * @param: time  
 * @返回: 
 * @作者: yzy (2019-11-3)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
void _DELAY(uint16 time)
{
    while(time--);
}

/*******************************************************************************
 * @function_name:  I2C_HRTC_WriteByte
 * @function_file:  IIC_HRTC.C
 * @描述: I2C总线传输一字节数据
 * 
 * 
 * @参数: 
 * @param: byte  写入的字节数据
 * 
 * @返回: 
 * @return:  uint8   
 * @作者: yzy (2019-11-03)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
uint8 I2C_HRTC_WriteByte(uint8 byte)
{
    uint8 err = 0;

    for(uint8 uc_i = 0;uc_i < 8;uc_i++)     //循环处理各个数据位
    {
        I2C_HRTC_SCK_0();                   //时钟线低
        _DELAY(100);                         //延时,保持
        if(byte & 0x80)                     //发送一位数据
        {
            I2C_HRTC_SDA_1();
        }
        else
        {
            I2C_HRTC_SDA_0();
        }
        _DELAY(150);                        //延时,保持
        I2C_HRTC_SCK_1();                   //时钟搞电平
        _DELAY(200);                        //延时保持
        byte <<= 1;                         //移位
    }
                                            //等待ACK信号
    I2C_HRTC_SCK_0();                       //时钟低
    _DELAY(50);  
    I2C_HRTC_SDAIN();                       //数据线改为输入
    _DELAY(150);   
                                            //延时保持
    I2C_HRTC_SCK_1();                       //时钟高
    _DELAY(100);                            //延时保持
    if(I2C_HRTC_GETSDA())                   //获取数据线状态
    {
        err = 1;
        //return 1;                           //NAK,返回ERR
    }
    _DELAY(50);                             //延时保持
    I2C_HRTC_SCK_0();                       //时钟低
    _DELAY(50);                             //延时保持
    //I2C_3231_SDA_1();
    I2C_HRTC_SDAOUT();                      //数据线输出
    return err;
}

/*******************************************************************************
 * @function_name:  I2C_HRTC_ReadByte
 * @function_file:  IIC_HRTC.C
 * @描述: I2C总线接收一字节数据(接收完一个字节后需要发送一个ACK或NAK信号)
 * 
 * @参数: 
 * 
 * @返回: IIC总线上收到的字节数据
 * @return:  uint8   
 * @作者: yzy (2019-11-03)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
uint8 I2C_HRTC_ReadByte(void)
{
    uint8 byte= 0;                          //接收的字节数据初始值

    I2C_HRTC_SDAIN();                       //数据线输入
    for(uint8 uc_i = 0;uc_i < 8;uc_i++)     //循环接收各位数据
    {
        I2C_HRTC_SCK_0();                   //时钟低
        _DELAY(200);                        //延时保持
        I2C_HRTC_SCK_1();                   //时钟高
        _DELAY(150);                        //延时保持
        byte <<= 1;                         //数据位左移
        if(I2C_HRTC_GETSDA())               //获取数据位
        {
            byte |= 0x01;
        }
        _DELAY(50);
    }

    return byte;
}

/*******************************************************************************
 * @function_name:  I2C_HRTC_Start
 * @function_file:  IIC_HRTC.C
 * @描述: i2c总线启动
 * 
 * @参数: 
 * @返回: 
 * @作者: yzy (2019-11-03)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
void I2C_HRTC_Start(void)
{
    I2C_HRTC_SDA_1();                       //数据线输出高
    I2C_HRTC_SCK_1();                       //时钟线输出高
    _DELAY(200);                            //延时 保持
    I2C_HRTC_SDA_0();                       //数据线下降沿
    _DELAY(200);                            //延时 保持
    I2C_HRTC_SCK_0();                       //时钟线输出低
    
}

/*******************************************************************************
 * @function_name:  I2C_HRTC_Stop
 * @function_file:  IIC_HRTC.C
 * @描述: I2C总线停止
 * 
 * @参数: 
 * @返回: 
 * @作者: yzy (2019-11-03)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
void I2C_HRTC_Stop(void)
{
    //I2C_3231_SCK_0();                       //时钟线输出低
    I2C_HRTC_SDA_0();                       //数据线输出低
    _DELAY(200);                            //延时 保持
    I2C_HRTC_SCK_1();                       //时钟线输出高
    _DELAY(200);                            //延时 保持
    I2C_HRTC_SDA_1();                       //数据线上升沿
    //_DELAY(100);                            //延时 保持
    //I2C_3231_SCK_0();                       //时钟线输出低

}

/*******************************************************************************
 * @function_name:  I2C_HRTC_ACK
 * @function_file:  IIC_HRTC.C
 * @描述: I2C总线ACK应答信号
 * 
 * @参数: 
 * @返回: 
 * @作者: yzy (2019-11-03)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
void I2C_HRTC_ACK(void)
{
    
    I2C_HRTC_SCK_0();                       //时钟线低电平
    _DELAY(50);
    I2C_HRTC_SDAOUT();
    _DELAY(100);
    I2C_HRTC_SDA_0();                       //数据线低电平
    _DELAY(100);                            //延时 保持
    I2C_HRTC_SCK_1();                       //时钟先上升沿
    _DELAY(200);                            //延时 保持
    I2C_HRTC_SCK_0();                       //时钟线下降沿
}

/*******************************************************************************
 * @function_name:  I2C_HRTC_NAK
 * @function_file:  IIC_HRTC.C
 * @描述: I2C总线NAK信号
 * 
 * @参数: 
 * @返回: 
 * @作者: yzy (2019-11-03)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
void I2C_HRTC_NAK(void)
{
    I2C_HRTC_SCK_0();                       //时钟线低电平
    _DELAY(50);
    I2C_HRTC_SDAOUT();
    _DELAY(100);
    I2C_HRTC_SDA_1();                       //数据线搞电平
    _DELAY(100);                            //延时 保持
    I2C_HRTC_SCK_1();                       //时钟上升沿
    _DELAY(200);                            //延时 保持
    I2C_HRTC_SCK_0();                       //时钟线下降沿
}



/*******************************************************************************
 * @function_name:  I2C_HRTC_ReadBcdTime
 * @function_file:  IIC_HRTC.C
 * @描述: 3231时间数据读取
 * 
 * 
 * @参数: 
 * @param: time  时钟缓存,字节顺序为秒,分,时,周,日,月,年,BCD格式
 * 
 * @返回: 
 * @return:  uint8   
 * @作者: yzy (2019-11-03)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
uint8 I2C_HRTC_ReadBcdTime(uint8* time)
{
    I2C_HRTC_Start();                       //I2C总线启动               
    I2C_HRTC_WriteByte(I2C_RTC_ID);         //写数据命令
    I2C_HRTC_WriteByte(0x00);               //写入读取数据的地址

    I2C_HRTC_Start();                       //I2C总线重新启动
    I2C_HRTC_WriteByte(I2C_RTC_ID + 1);     //读取数据命令
    for(uint8 uc_i = 0; uc_i < 7; uc_i++)     //循环接收字节数据
    {
        time[uc_i] = I2C_HRTC_ReadByte();   //读取一个字节数据
        if(uc_i == 6)                       //最后一个字节返回NAK
        {
            I2C_HRTC_NAK();
        }
        else                                //返回ACK
        {
            I2C_HRTC_ACK();
        }
    }
    I2C_HRTC_Stop();                        //I2C总线停止
                                            //获取星期
    time[3] &= 0x7F;
    time[3] = STR_GetFirstSetBit(time + 3, 1);
    SYS_OK();
}

/*******************************************************************************
 * @function_name:  I2C_HRTC_ReadHexTime
 * @function_file:  IIC_HRTC.C
 * @描述: 3231时间数据读取,HEX格式
 * 
 * 
 * @参数: 
 * @param: time  时钟缓存,字节顺序为秒,分,时,周,日,月,年
 * 
 * @返回: 
 * @return:  uint8   
 * @作者: yzy (2019-11-03)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
uint8 I2C_HRTC_ReadHexTime(uint8* time)
{
    I2C_HRTC_ReadBcdTime(time);                 //读取时间数据
    ByteArrayBcdToHex(time, time, 7);       //将BCD转为HEX
    SYS_OK();
}

/*******************************************************************************
 * @function_name:  I2C_HRTC_WriteBcdTime
 * @function_file:  IIC_HRTC.C
 * @描述: 3231时间数据写入
 * 
 * 
 * @参数: 
 * @param: time  时钟缓存,字节顺序为秒,分,时,周,日,月,年,BCD格式
 * 
 * @返回: 
 * @return:  uint8   
 * @作者: yzy (2019-11-03)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
uint8 I2C_HRTC_WriteBcdTime(uint8* time)
{
    uint8 err = 1;                          //写入结果默认err

    time[3] = Bit_Map8[time[3] % 7];        //获取需要写入的星期
                                            //如果写入不成功则重写,最多重写3遍
    for(uint8 uc_i = 0;(uc_i < 3) && (err != 0); uc_i++)
    {
        I2C_HRTC_Start();                   //i2c总线启动
                                            //写命令
        err = I2C_HRTC_WriteByte(I2C_RTC_ID);
        err |= I2C_HRTC_WriteByte(0x00);    //写入操作地址

        for(uint8 uc_j = 0;uc_j < 7;uc_j++) //循环写入各个字节
        {
          //_DELAY(50);
            err |= I2C_HRTC_WriteByte(time[uc_j]);
        }
        I2C_HRTC_Stop();                    //i2c总线停止
    }
    return err;                             //返回结果
}

/*******************************************************************************
 * @function_name:  I2C_HRTC_WriteHexTime
 * @function_file:  IIC_HRTC.C
 * @描述: 3231时间数据写入HEX格式写入
 * 
 * 
 * @参数: 
 * @param: time  时钟缓存,字节顺序为秒,分,时,周,日,月,年
 * 
 * @返回: 
 * @return:  uint8   
 * @作者: yzy (2019-11-03)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
uint8 I2C_HRTC_WriteHexTime(uint8* time)
{
    ByteArrayHexToBcd(time, time, 7);       //将HEX转成BCD
    return I2C_HRTC_WriteBcdTime(time);         //写入时间数据
}

/*******************************************************************************
 * @function_name:  I2C_HRTC_SecIntOnOff
 * @function_file:  IIC_HRTC.C
 * @描述: 硬时钟秒中断输出开关函数
 * 
 * 
 * @参数: 
 * @param: oc  true打开, false关闭
 * 
 * @返回: 
 * @return:  uint8   
 * @作者: yzy (2019-11-3)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
uint8 I2C_HRTC_SecIntOnOff(bool oc)
{
    uint8 uc_data[3];                       //写入的数据缓存
    uint8 err = 1;                          //处理结果默认为1

    ClearBuffer(uc_data, sizeof(uc_data));
    if(!oc)                                 //关闭,写入0x40
    {
        uc_data[2] = 0x40;
    }
    else                                    //打开,写入0x60
    {
        uc_data[2] = 0x60;
    }
                                            //写入数据,失败重试,重试次数3次
    for(uint8 uc_i = 0;(uc_i < 3) && (err != 0);uc_i++)
    {
        I2C_HRTC_Start();                   //I2C总线启动
                                            //写命令
        err = I2C_HRTC_WriteByte(I2C_RTC_ID);
        err |= I2C_HRTC_WriteByte(0x0D);     //写入的地址
        for(uint8 uc_j = 0;uc_j < 3;uc_j++)
        {                                   //写入内容,打开或者关闭秒中断
            err |= I2C_HRTC_WriteByte(uc_data[uc_j]);
        }
        I2C_HRTC_Stop();                    //I2C总线停止
    }
    return err;
}

/*******************************************************************************
 * @function_name:  Init_I2C_HRTC
 * @function_file:  IIC_HRTC.C
 * @描述: 硬时钟I2C总线初始化
 * 
 * @参数: 
 * @返回: 
 * @作者: yzy (2019-11-3)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
void Init_I2C_HRTC(const IIC_PORTS* ports)
{
    uint8 uc_i;
                                            //配置SDA端口和SCK端口
    HAL_GPIO_PinConfig(ports->portSDA->port, ports->portSDA->pin, ports->portSDA->mode);
    HAL_GPIO_PinConfig(ports->portSCK->port, ports->portSCK->pin, ports->portSCK->mode);
                                            //赋值全局结构体
    gsp_halHrtcIICPorts = ports;
    
    I2C_HRTC_SDAIN();                       //数据信号线默认输入
    I2C_HRTC_SCKOUT();                      //时钟信号线默认输出
    I2C_HRTC_SCK_1();                       //默认输出低电平

    for(uc_i = 0;uc_i < 10;uc_i++)          //循环检测数据线状态,此时数据线为输入态
    {
        _DELAY(150);
        if(I2C_HRTC_GETSDA())               //高电平,则产生一个Start信号
        {
            I2C_HRTC_SDAOUT();
            I2C_HRTC_SDA_1();
            break;
        }
                                            //数据线低电平,从器件输出,变换时钟线(上升沿),结束时序
        I2C_HRTC_SCK_0();                   //时钟输出低
        _DELAY(200);                        //延时保持
        I2C_HRTC_SCK_1();                   //时钟输出高(上升沿)
        _DELAY(200);                        //延时保持
    }
    if(uc_i == 10)                          //10个时钟上升沿,未能结束总线时序,出错
    {
        return;
    }
    I2C_HRTC_SCK_0();
    _DELAY(200);
    I2C_HRTC_SCK_1();
    _DELAY(150);
    I2C_HRTC_SDA_1();

    //I2C_3231_F32KOFF();                     //默认关闭32K信号输出
   // I2C_HRTC_SecIntOnOff(true);             //初始化打开秒中断输出
}


