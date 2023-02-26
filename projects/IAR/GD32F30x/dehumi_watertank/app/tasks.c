/****************************************Copyright (c)**************************************************
**                                  ________�Ƽ����޹�˾
**                                          ������
**
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: tasks.c
**��   ��   ��: yzy
**����޸�����: 2018��3��7��
**��        ��: Ӧ�ò�����ģ��
**ע        ��:
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: yzy
** ��  ��: v1.0
** �ա���: 2018��3��7��
** �衡��: ԭʼ�汾
********************************************************************************************************/
#define EXT_TASKS
//	#include "es.h"
//	#include "hal.h"
//	#include "bsp.h"
//	#include "app.h"
//	#include "apis.h"
//	#include "extapi.h"
#include <k_api.h>
#include <stdio.h>
#include <stdlib.h>
#include "public.h"


/*******************************************************************************
 * @function_name:  
 * @function_file: 
 * @����: ��������Ľṹ��
 * 
 * 
 * @����: 
 * @param: 
 * 
 * @����: 
 * @return:   
 * @����: houpm 
 *-----------------------------------------------------------------------------
 * @�޸���   yzy
 * @�޸���   ע��.8������һ��,���Ϊ0-7,������̵ĵ��ȼ������
 *        0��1:1��tick
 *        2��3:2��tick
 *        4��5:4��tick
 *        6��7:8��tick
 ******************************************************************************/
const KTaskDeclare __TKDeclare[SYS_TK_NUM] = 
{   //����0-7
    {__NULL, 0},                        //ϵͳ��������ʱ�Ѿ���ʼ��,����Ҫ����(gs_TkSystemc,ջgt_TkSystemStk[SYS_SYS_STK_LEN])
    TASK_DECLARE_1_MAIN(),              //meter���ƽ���
    {__NULL, 0},//TASK_DECLARE_2_HLV(),//{__NULL, 0},
    {__NULL, 0},//TASK_DECLARE_3_FARP(),               //Զ��ͨ���������
    {__NULL, 0},//TASK_DECLARE_4_NETP(),//{__NULL, 0},
    {__NULL, 0},//TASK_DECLARE_5_NETP(),//{__NULL, 0},
    {__NULL, 0},//TASK_DECLARE_6_NETP(),//{__NULL, 0},
    {__NULL, 0},//TASK_DECLARE_7_NETP(),//
    {__NULL, 0},
#ifndef MASTER_NODE     
    TASK_DECLARE_9_SLV(),
#else
    {__NULL, 0}, 
#endif    
    TASK_DECLARE_10_RFMNG(),
    
};

/***********************************************************
 * @function_name: SYS_TASKS_Init
 * @function_file: SYS_core.c
 * @����:�����û����̵Ĵ���
 * 
 * @����: 
 * @����: 
 * @����:
 *---------------------------------------------------------
 * @�޸���: yzy (2010/2/11)
 **********************************************************/
void SYS_TASKS_Init(void)
{
    extern const KTaskDeclare __TKDeclare[SYS_TK_NUM];
    const KTaskDeclare* dec = __TKDeclare;

    //ktask_t * handle;
    for(uint32 k = 0; k < (SYS_TK_NUM ); k++, dec++)   //ѭ����ʼ��
    {
        if(dec->ktask == __NULL || dec->stklen == 0)
        {
            continue;
        }
        
        //��ʼ��������
        memset(dec->ktask->stack, 0xFF, dec->stklen * sizeof(cpu_stack_t));//yzy ��ʼ��ջ,���ڹ۲��Ƿ����
        krhino_task_create(dec->ktask->handle, dec->ktask->name, dec->ktask->arg, dec->ktask->prior, 
            50, dec->ktask->stack, dec->stklen, dec->ktask->pfunc, 1);

        aos_msleep(100);

    }
}


