/******************************Copyright(c)******************************
**                          
**
** File Name: dev_gpo.c
** Author: 
** Date Last Update: 2019-11-02
** Description: ��ͨ������豸
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: �ļ�����
*************************************************************************/
#define EXT_GPO

#include "sys.h"
#include "hal.h"
#include "bsp.h"





/************************************************************************
 * @ģ�������ʾ
 ************************************************************************/
#if (SYS_GPO_EN > 0)

    #if !defined(GPO_PORT_NUM) || (GPO_PORT_NUM < 1)
        #define GPO_PORT_NUM  1
        #warning "GPO���������"
    #endif

#endif





/************************************************************************
 *��Դ��
 ************************************************************************/
#if (SYS_GPO_EN > 0)
aos_sem_t gs_GPORes;
#endif




/************************************************************************
 * @Function: SYS_GPO_Init
 * @Description: GPO�豸��ʼ��
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
                                            //ѭ����ʼ������GPO�˿�
    for(uint32 uc_i = 0; uc_i < GPO_PORT_NUM; uc_i++)
    {
        gpo = (GPO_PORTS*)gs_GpoPort + uc_i;//GPO�˿�ָ��
        _IF_TRUE_DO(gpo == __NULL, continue);
                                            //���ö˿ڹ���,����Ϊ���ģʽ

        HAL_GPIO_PinConfig(gpo->port);
                                                    
//        HAL_GPIO_PinConfig(&gpo->gpio[gpo->pingrp], gpo->pinnum, gpo->modefunc, gpo->dir);
//                                            
        if(gpo->rvs != gpo->ival)           //��ͬ�����
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
 * @param: gpo_name ����ڱ��ö��(GPO_ENUM)
 * @param: oc -true ����ߵ�ƽ, false ����͵�ƽ
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
    _IF_TRUE_RETURN_VOID(gpo_name >= GPO_PORT_NUM);//��������
    
    gpo = (GPO_PORTS*)gs_GpoPort + gpo_name;//�˿���Ϣָ��
    _IF_TRUE_RETURN_VOID(gpo == __NULL);
#ifndef __NO_SYS__
//    SYS_SEM_Wait(&gs_GPORes, 0);             //������Դ
    krhino_sem_take(&gs_GPORes, RHINO_WAIT_FOREVER);
#endif

    HAL_GPIO_PinConfig(gpo->port);

    if(oc)                                  //����ߵ�ƽ
    {
        if(gpo->rvs)                        //�߼���ƽ��ת
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
    else                                    //����͵�ƽ
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
    krhino_sem_give(&gs_GPORes);             //�ͷ���Դ
#endif
#endif
}

void SYS_GPO_WakeUp(void)
{
#if (SYS_GPO_EN > 0)
    GPO_PORTS* gpo;
                                            //ѭ����ʼ������GPO�˿�
    for(uint32 uc_i = 0; uc_i < GPO_PORT_NUM; uc_i++)
    {
        gpo = (GPO_PORTS*)gs_GpoPort + uc_i;//GPO�˿�ָ��
        _IF_TRUE_DO(gpo == __NULL, continue);
                                            //���ö˿ڹ���,����Ϊ���ģʽ
        HAL_GPIO_PinConfig(gpo->port);
                                            
        if(gpo->rvs != gpo->ival)           //��ͬ�����
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
                                            //ѭ����ʼ������GPO�˿�
    for(uint32 uc_i = 0; uc_i < GPO_SLEEPPORT_NUM; uc_i++)
    {
        gpo = (COMPORT*)gs_GpoPort + uc_i;//GPO�˿�ָ��
        _IF_TRUE_DO(gpo == __NULL, continue);
                                            //���ö˿ڹ���,����Ϊ���ģʽ
        HAL_GPIO_PinConfig(&gpo->gpio[gpo->pingrp], gpo->pinnum, gpo->lptype, gpo->analog, gpo->lpdir);
                                            
        if(gpo->rvs != gpo->ival)           //��ͬ�����
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








