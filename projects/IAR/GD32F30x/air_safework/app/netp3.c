/****************************************Copyright (c)**************************************************
**                                  ____�Ƽ����޹�˾
**                                          ������
**
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: netp3.c
**��   ��   ��: yzy
**����޸�����: 2007��9��5��
**��        ��: ����������ͨѶ�˿ڵ�ͨ�ú���
**ע        ��:
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: yzy
** ��  ��: v1.0
** �ա���: 2007��9��5��
** �衡��: ԭʼ�汾
********************************************************************************************************/
#define EXT_NETP3
#include "es.h"
#include "hal.h"
#include "bsp.h"
#include "app.h"
#include "apis.h"
#include "extapi.h"



void Netp_SecProc(uint8 index);
void Netp_InitData(uint8 index);
void Netp_CheckTstTime(uint8 index);
void SynToF10Para(uint8 index);




/************************************************************************
 * @function: ES_NETP3_Task
 * @����: �������̴�����
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/2/11)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void ES_NETP3_Task(void)
{
                                            //������Ϣ
    TESMsg* msg = (TESMsg*)gs_Mem.GetMEM(sizeof(TESMsg));
    uint8 nmsg;                             //������Ϣ
    
    
    ES_MSG_Apply(MSG_CLS_TM);
    
    for(;;)
    {
        nmsg = gs_OS.Message_Wait(&msg);    //��ȡ������Ϣ
        switch(nmsg)                        //������Ϣ�ֱ���
        {
            case MSG_SEC:
                Netp_SecProc(2);
                break;

            case MSG_NETP_IDATA:            //�˿�3���ݳ�����ʼ
                Netp_InitData(2);
                PST_UART_Init();
                break;

            case MSG_NETP_CKTIME:
                Netp_CheckTstTime(2);
                PST_UART_Init();
                break;
                
            case MSG_LIVE:
				HB_RetLive();
                break;
        }
    }
}

/******************************************************************************
**�������̿鳣�����ֶ���
******************************************************************************/
const TESTcbConst gs_TkNetp3 = 
{
    ES_NETP3_Task,
    TASK_NETP3_TKID >> 3,                   //Ӧ�ù������
    TASK_NETP3_TKID,                        //���̱��
    TASK_NETP3_TKID & 7,                    //���ȼ��ڽ��̱��λ
    true,                                      //��Ϣѭ�����ƽ���
};

