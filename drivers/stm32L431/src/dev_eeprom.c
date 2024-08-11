/****************************************Copyright (c)**************************************************
**                                  ________科技有限公司
**                                          开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: iic_Eeprom.c
**创   建   人: yzy
**最后修改日期: 2018-07-02
**描        述: Eeprom驱动管理函数
**注        意:
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2013年10月05日
** 描　述: Eeprom驱动
********************************************************************************************************/
//	#define __TYPEDEF__
#define EXT_DEV_E2

#include "sys.h"
#include "hal.h"
#include "bsp.h"


/*******************************************************************************
**eeprom操作iic结构体
********************************************************************************/
SPIIO gs_EEPROM_iic;

/*******************************************************************************
 * @function_name:  Init_I2C_Eeprom
 * @function_file:  IIC_Eeprom.C
 * @描述: EepromI2C总线初始化
 * 
 * @参数: 
 * @返回: 
 * @作者: yzy (2018-07-02)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
void SYS_E2_Init(void)
{

    HAL_E2_Init(&gs_e2Port);
//	    I2C_Eeprom_PWROUT();
//	
//	//地址配置
//	
//	    I2C_Eeprom_ADDR1OUT();
//	    I2C_Eeprom_ADDR2OUT();
//	    I2C_Eeprom_ADDR3OUT();
//	
//	    
//	    I2C_Eeprom_SDAOUT();                       //数据信号线默认输出
//	    I2C_Eeprom_SCKOUT();                      //时钟信号线默认输出
//	    I2C_Eeprom_SCK_1();                       //默认输出低电平
//	    I2C_Eeprom_SDA_1();


}

void SYS_E2_Idel(void)
{
    HAL_E2_Idel();


}






/************************************************************************
 * @function: PCB_Eeprom_Read
 * @描述: 读取eeprom内部指定地址的数据
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
uint8_t SYS_E2_Read(uint8_t* buffer, uint32_t address, uint16_t length)
{
    uint8_t m = 0;
                                        //参数验证
    SYS_VAR_CHECK(address + length > DEF_EE_SIZE);
    SYS_VAR_CHECK(length == 0);
    SYS_VAR_CHECK(buffer == NULL);                                        
                                        //读取地址操作
    if(DEF_EE_SIZE >= 0x20000)
    {
        gs_EEPROM_iic.command[m++] = address >> 16;
        gs_EEPROM_iic.command[m++] = address >> 8;
        gs_EEPROM_iic.command[m++] = address;
    }
    else
    {
        gs_EEPROM_iic.command[m++] = address >> 8;
        gs_EEPROM_iic.command[m++] = address;
    }
    gs_EEPROM_iic.cmdnum = m;           //命令
    
    gs_EEPROM_iic.length = length;         //长度
    gs_EEPROM_iic.data = buffer;           //缓存
    return HAL_E2_Read(&gs_EEPROM_iic);       //返回读取结果
}



/************************************************************************
 * @function: FRAM_Write
 * @描述: 写入eeprom内部指定地址的数据
 * 
 * @参数: 
 * @param: buffer 写入数据的缓存
 * @param: address 写入数据的起始地址
 * @param: length 写入数据的长度
 * 
 * @返回: 
 * @return: uint8_t  
 * @说明: 
 * @作者: yzy (2014/8/22)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8_t SYS_E2_Write_Page(uint8_t* buffer, uint32_t address, uint16_t length)
{
    uint8_t m = 0;
    for(uint8_t uc_i = 0; uc_i < 3; uc_i++)
    {
        m=0;
                                        //参数验证
        SYS_VAR_CHECK(address + length > DEF_EE_SIZE || address > DEF_EE_SIZE);
                                        //写入地址操作

        if(DEF_EE_SIZE >= 0x20000)
        {
            gs_EEPROM_iic.command[m++] = address >> 16;
            gs_EEPROM_iic.command[m++] = address >> 8;
            gs_EEPROM_iic.command[m++] = address;
        }
        else
        {
            gs_EEPROM_iic.command[m++] = address >> 8;
            gs_EEPROM_iic.command[m++] = address;
        }
        
        gs_EEPROM_iic.cmdnum = m;           //命令
        gs_EEPROM_iic.length = length;      //长度
        gs_EEPROM_iic.data = buffer;        //缓存
                                         //判断安全字   
        if(HAL_E2_Write(&gs_EEPROM_iic) != SYS_ERR_OK)
        {
            continue;                //再来一次
        }

        if(HAL_E2_RdAndCmp(&gs_EEPROM_iic) != SYS_ERR_OK)
        {
            continue;                   //再来一次
        }
        SYS_OK();                        //返回写入成功
    }
    return SYS_ERR_FT;                 //返回发生致命错误

}
uint8_t SYS_E2_Write(uint8_t* buffer, uint32_t address, uint16_t length)
{
    uint16_t ui_len = 0;                      //单个SECTOR中写入的长度
    uint32_t addr = address;

    
    SYS_VAR_CHECK(address + length > DEF_EE_SIZE);
    SYS_VAR_CHECK(length == 0);
    SYS_VAR_CHECK(buffer == NULL);
    /************************************************************************
    //循环处理每个需要更改的SECTOR
    ************************************************************************/
    while(true)                             //循环处理每个需要更改的SECTOR
    {
        if(length + (addr & DEF_EE_BUF_MASK) > DEF_EE_BUF)  //跨段
        {
            ui_len = DEF_EE_BUF - (addr & DEF_EE_BUF_MASK); //操作的数据长度
        }
        else                                //段内操作
        {
            ui_len = length;                //操作数据长度
        }
        if(SYS_ERR_OK != SYS_E2_Write_Page(buffer, addr, ui_len))
            return SYS_ERR_FT;

        length -= ui_len;                   //待操作数据长度递减
        if(length == 0)                     //数据已经操作完毕
        {
            SYS_OK();                        //返回写入成功
        }

        buffer += ui_len;                   //操作数据指针前移
        addr += ui_len;                     //实际操作地址
    }

}


