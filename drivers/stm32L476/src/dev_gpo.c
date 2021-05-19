/****************************************Copyright (c)*************************
**                               	________�Ƽ����޹�˾
**                                     		������
**
**
**--------------�ļ���Ϣ-------------------------------------------------------
**��   ��   ��: dev_gpio.h
**��   ��   ��: yzy
**����޸�����: 2010��3��10��
**��        ��: ��ͨ�������(����LED���)�˿ڹ�����̺�������ͷ�ļ�
**ע        ��:
**--------------��ʷ�汾��Ϣ---------------------------------------------------
** ������: yzy
** ��  ��: v1.0
** �ա���: 2010��3��10��
** �衡��: ԭʼ�汾
******************************************************************************/
#define EXT_DEV_GPO

#include "aos/include.h"
//	#include "k_types.h"
//	#include "k_api.h"
//	#include "lib_app.h"
//	#include "hal.h"
#include "gpio.h"


/*******************************************************************************
**��Դ��
******************************************************************************/
#if SYS_GPO_EN > 0
ksem_t gs_GPORes;
extern gpio_dev_t brd_gpio_table[];
#endif

/*******************************************************************************
 * @function_name:  SYS_GPO_Init
 * @function_file:  dev_gpio.c
 * @����: GPO�豸��ʼ��
 * 
 * @����: 
 * @����: 
 * @����: yzy (2010-03-11)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
void SYS_GPO_Init(void)
{
#if SYS_GPO_EN > 0
//	    GPO_PORTS* gpo;
                                            //ѭ����ʼ������GPO�˿�
//	    for(uint32 uc_i = 0; uc_i < GPO_NUM; uc_i++)
//	    {
//	        gpo = (GPO_PORTS*)gs_GpoPort + uc_i;  //GPO�˿�ָ��
//	                                            //���ö˿ڹ���,����Ϊ���ģʽ
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
 * @����: GPO���
 * 
 * 
 * @����: 
 * @param: gpo(GPOENUM): 
 * @param: oc(bool): true ����ߵ�ƽ, false ����͵�ƽ
 * @����: 
 * @����: yzy (2010-03-11)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
void SYS_GPO_Out(GPOENUM gpo, uint8 oc)
{
#if SYS_GPO_EN > 0
//	    GPO_PORTS* gp;
    _IF_TRUE_RETURN_VOID(gpo >= GPO_NUM);   //��������
    krhino_sem_take(&gs_GPORes, RHINO_WAIT_FOREVER);             //������Դ
    
    gpio_dev_t *gp = brd_gpio_table+gpo;
//	    uint8_t * rvs = (uint8_t *)(gp->priv);
//	    gp = (GPO_PORTS*)(gs_GpoPort + gpo);      //�˿���Ϣָ��
    
    if(oc)                                  //����ߵ�ƽ
    {
        hal_gpio_output_high(gp);
//	        if(gp->rvs)                         //�߼���ƽ��ת
//	        {
//	            Chip_GPIO_SetPinState(gp->port->pingrp, gp->port->pinnum, false);
//	        }
//	        else
//	        {
//	            Chip_GPIO_SetPinState(gp->port->pingrp, gp->port->pinnum, true);
//	        }
    }
    else                                    //����͵�ƽ
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
    krhino_sem_give(&gs_GPORes);//�ͷ���Դ
#endif
}







//	#if (SYS_GPO_EN > 0)
//	const _F_GPO gs_DEVGpo = 
//	{
//	    SYS_GPO_Init,
//	    SYS_GPO_Out,
//	};
//	#endif





