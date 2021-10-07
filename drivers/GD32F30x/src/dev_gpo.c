/******************************Copyright(c)******************************
**                          
**
** File Name: dev_gpo.c
** Author: 
** Date Last Update: 2019-11-02
** Description: 普通输出口设备
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: 文件创建
*************************************************************************/
#define EXT_GPO

#include "sys.h"
#include "hal.h"
#include "bsp.h"





/************************************************************************
 * @模块编译提示
 ************************************************************************/
#if (SYS_GPO_EN > 0)

    #if !defined(GPO_PORT_NUM) || (GPO_PORT_NUM < 1)
        #define GPO_PORT_NUM  1
        #warning "GPO总数宏错误"
    #endif

#endif





/************************************************************************
 *资源量
 ************************************************************************/
#if (SYS_GPO_EN > 0)
aos_sem_t gs_GPORes;
#endif




/************************************************************************
 * @Function: SYS_GPO_Init
 * @Description: GPO设备初始化
 * @Arguments: 
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/27
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void SYS_GPO_Init(void)
{
#if (SYS_GPO_EN > 0)
    GPO_PORTS* gpo;
    HAL_InitGPIO();
                                            //循环初始化各个GPO端口
    for(uint32 uc_i = 0; uc_i < GPO_PORT_NUM; uc_i++)
    {
        gpo = (GPO_PORTS*)gs_GpoPort + uc_i;//GPO端口指针
        _IF_TRUE_DO(gpo == __NULL, continue);
                                            //配置端口功能,并设为输出模式

        HAL_GPIO_PinConfig(gpo->port);
                                                    
//        HAL_GPIO_PinConfig(&gpo->gpio[gpo->pingrp], gpo->pinnum, gpo->modefunc, gpo->dir);
//                                            
        if(gpo->rvs != gpo->ival)           //等同于异或
        {
//	            HAL_GPIO_SetPinState(&gpo->gpio[gpo->pingrp], gpo->pinnum, 1);
            HAL_GPIO_SetPinState(gpo->port, gpo->port->pinnum, true);
        }
        else
        {
            HAL_GPIO_SetPinState(gpo->port, gpo->port->pinnum, false);
        }
    }
#ifndef __NO_SYS__
    aos_sem_new(&gs_GPORes, 1);
#endif    
#endif
}



/************************************************************************
 * @Function: SYS_GPO_Out
 * @Description: 
 * 
 * @Arguments: 
 * @param: gpo_name 输出口编号枚举(GPO_ENUM)
 * @param: oc -true 输出高电平, false 输出低电平
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/27
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void SYS_GPO_Out(uint32 gpo_name, bool oc)
{
#if (SYS_GPO_EN > 0)
    GPO_PORTS* gpo;
    _IF_TRUE_RETURN_VOID(gpo_name >= GPO_PORT_NUM);//参数检验
    
    gpo = (GPO_PORTS*)gs_GpoPort + gpo_name;//端口信息指针
    _IF_TRUE_RETURN_VOID(gpo == __NULL);
#ifndef __NO_SYS__
//    SYS_SEM_Wait(&gs_GPORes, 0);             //申请资源
    krhino_sem_take(&gs_GPORes, RHINO_WAIT_FOREVER);
#endif

    HAL_GPIO_PinConfig(gpo->port);

    if(oc)                                  //输出高电平
    {
        if(gpo->rvs)                        //逻辑电平翻转
        {
//	            HAL_GPIO_SetPinState(gpo->cp->GPIOx, gpo->cp->pin, false);
            HAL_GPIO_SetPinState(gpo->port, gpo->port->pinnum, 0);
            
        }
        else
        {
//	            HAL_GPIO_SetPinState(gpo->cp->port, gpo->cp->pin, true);
            HAL_GPIO_SetPinState(gpo->port, gpo->port->pinnum, 1);
            
        }
    }
    else                                    //输出低电平
    {
        if(gpo->rvs)
        {
            //HAL_GPIO_SetPinState(gpo->cp->port, gpo->cp->pin, true); 
            HAL_GPIO_SetPinState(gpo->port, gpo->port->pinnum, 1);
            
        }
        else
        {
            //HAL_GPIO_SetPinState(gpo->cp->port, gpo->cp->pin, false);
            HAL_GPIO_SetPinState(gpo->port, gpo->port->pinnum, 0);
            
        }
    }
#ifndef __NO_SYS__    
    krhino_sem_give(&gs_GPORes);             //释放资源
#endif
#endif
}

void SYS_GPO_WakeUp(void)
{
#if (SYS_GPO_EN > 0)
    GPO_PORTS* gpo;
                                            //循环初始化各个GPO端口
    for(uint32 uc_i = 0; uc_i < GPO_PORT_NUM; uc_i++)
    {
        gpo = (GPO_PORTS*)gs_GpoPort + uc_i;//GPO端口指针
        _IF_TRUE_DO(gpo == __NULL, continue);
                                            //配置端口功能,并设为输出模式
        HAL_GPIO_PinConfig(gpo->port);
                                            
        if(gpo->rvs != gpo->ival)           //等同于异或
        {
            HAL_GPIO_SetPinState(gpo->port, gpo->port->pinnum, 1);
        }
        else
        {
            HAL_GPIO_SetPinState(gpo->port, gpo->port->pinnum, 0);
        }
    }
   
#endif
}
#ifdef __NO_SYS__
void SYS_GPO_Idel(void)
{
#if (SYS_GPO_EN > 0)
    COMPORT* gpo;
                                            //循环初始化各个GPO端口
    for(uint32 uc_i = 0; uc_i < GPO_SLEEPPORT_NUM; uc_i++)
    {
        gpo = (COMPORT*)gs_GpoPort + uc_i;//GPO端口指针
        _IF_TRUE_DO(gpo == __NULL, continue);
                                            //配置端口功能,并设为输出模式
        HAL_GPIO_PinConfig(&gpo->gpio[gpo->pingrp], gpo->pinnum, gpo->lptype, gpo->analog, gpo->lpdir);
                                            
        if(gpo->rvs != gpo->ival)           //等同于异或
        {
            HAL_GPIO_SetPinState(&gpo->gpio[gpo->pingrp], gpo->pinnum, 1);
        }
        else
        {
            HAL_GPIO_SetPinState(&gpo->gpio[gpo->pingrp], gpo->pinnum, 0);
        }
    }
   
#endif
}

#endif








