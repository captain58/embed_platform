/****************************************Copyright (c)*************************
**                               	________科技有限公司
**                                     		开发部
**
**
**--------------文件信息-------------------------------------------------------
**文   件   名: dev_gpio.h
**创   建   人: yzy
**最后修改日期: 2010年3月10日
**描        述: 普通输入输出(包括LED输出)端口管理进程函数配置头文件
**注        意:
**--------------历史版本信息---------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2010年3月10日
** 描　述: 原始版本
******************************************************************************/
#define EXT_DEV_GPO

#include "aos/include.h"
//	#include "k_types.h"
//	#include "k_api.h"
//	#include "lib_app.h"
//	#include "hal.h"
#include "gpio.h"


/*******************************************************************************
**资源量
******************************************************************************/
#if SYS_GPO_EN > 0
ksem_t gs_GPORes;
extern gpio_dev_t brd_gpio_table[];
#endif

/*******************************************************************************
 * @function_name:  SYS_GPO_Init
 * @function_file:  dev_gpio.c
 * @描述: GPO设备初始化
 * 
 * @参数: 
 * @返回: 
 * @作者: yzy (2010-03-11)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
void SYS_GPO_Init(void)
{
#if SYS_GPO_EN > 0
//	    GPO_PORTS* gpo;
                                            //循环初始化各个GPO端口
//	    for(uint32 uc_i = 0; uc_i < GPO_NUM; uc_i++)
//	    {
//	        gpo = (GPO_PORTS*)gs_GpoPort + uc_i;  //GPO端口指针
//	                                            //配置端口功能,并设为输出模式
//	        Chip_GPIO_SetPinState(gpo->port->pingrp, gpo->port->pinnum, true);
//	                                            
//	        Chip_GPIO_SetPinDigit(gpo->port->pingrp, gpo->port->pinnum);
//	        
//	        Chip_GPIO_SetPinPFSEG(gpo->port->pingrp, gpo->port->pinnum, gpo->port->pinseg);
//	        Chip_GPIO_SetPinOutputMode(gpo->port->pingrp, gpo->port->pinnum, gpo->port->modefunc & 0x01);
//	//	        Chip_IOCON_PinMuxSet(LPC_IOCON, gpo->port->pingrp, gpo->port->pinnum, gpo->port->modefunc);
//	        Chip_GPIO_SetPinDIROutput(gpo->port->pingrp, gpo->port->pinnum);
//	        if(gpo->set)
//	        {
//	            Chip_GPIO_SetPinState(gpo->port->pingrp, gpo->port->pinnum, true); 
//	        }
//	        else
//	        {
//	            Chip_GPIO_SetPinState(gpo->port->pingrp, gpo->port->pinnum, false);
//	        }
//	
//	    }
    
//	    SYS_SEM_Create(1, &gs_GPORes);
    krhino_sem_create(&gs_GPORes, "gpo_sem", 1);

#endif
}


/*******************************************************************************
 * @function_name:  SYS_GPO_Out
 * @function_file:  dev_gpio.c
 * @描述: GPO输出
 * 
 * 
 * @参数: 
 * @param: gpo(GPOENUM): 
 * @param: oc(bool): true 输出高电平, false 输出低电平
 * @返回: 
 * @作者: yzy (2010-03-11)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
void SYS_GPO_Out(GPOENUM gpo, uint8 oc)
{
#if SYS_GPO_EN > 0
//	    GPO_PORTS* gp;
    _IF_TRUE_RETURN_VOID(gpo >= GPO_NUM);   //参数检验
    krhino_sem_take(&gs_GPORes, RHINO_WAIT_FOREVER);             //申请资源
    
    gpio_dev_t *gp = brd_gpio_table+gpo;
//	    uint8_t * rvs = (uint8_t *)(gp->priv);
//	    gp = (GPO_PORTS*)(gs_GpoPort + gpo);      //端口信息指针
    
    if(oc)                                  //输出高电平
    {
        hal_gpio_output_high(gp);
//	        if(gp->rvs)                         //逻辑电平翻转
//	        {
//	            Chip_GPIO_SetPinState(gp->port->pingrp, gp->port->pinnum, false);
//	        }
//	        else
//	        {
//	            Chip_GPIO_SetPinState(gp->port->pingrp, gp->port->pinnum, true);
//	        }
    }
    else                                    //输出低电平
    {
        hal_gpio_output_low(gp);
//	        if(gp->rvs)
//	        {
//	            Chip_GPIO_SetPinState(gp->port->pingrp, gp->port->pinnum, true); 
//	        }
//	        else
//	        {
//	            Chip_GPIO_SetPinState(gp->port->pingrp, gp->port->pinnum, false);
//	        }
    }
    krhino_sem_give(&gs_GPORes);//释放资源
#endif
}







//	#if (SYS_GPO_EN > 0)
//	const _F_GPO gs_DEVGpo = 
//	{
//	    SYS_GPO_Init,
//	    SYS_GPO_Out,
//	};
//	#endif





