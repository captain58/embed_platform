/****************************************Copyright (c)**************************************************
**                                  ________�Ƽ����޹�˾
**                                          ������
**
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: dev_valve.h
**��   ��   ��: yzy
**����޸�����: 2018��4��25��
**��        ��: ��������ģ���ͷ�ļ�
**ע        ��:
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: yzy
** ��  ��: v1.0
** �ա���: 2018��4��25��
** �衡��: ԭʼ�汾
********************************************************************************************************/
#ifndef _DEV_VALVE_H_
#define _DEV_VALVE_H_


#ifndef EXT_DEV_VALVE
#define EXT_DEV_VALVE extern
#endif

typedef enum
{
    eHWStatus_VALVE_UNKNOW,
    eHWStatus_VALVE_CLOSED,
    eHWStatus_VALVE_CLOSING,
    eHWStatus_VALVE_OPEN,
    eHWStatus_VALVE_OPENING,
}eHWStatus;


/*******************************************************************************
 * @function_name:  SYS_VALVE_Init
 * @function_file:  dev_valve.c
 * @����: ���ų�ʼ��
 * 
 * @����: 
 * @����: 
 * @����: yzy (2018-04-25)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
void SYS_VALVE_Init(void);
/*******************************************************************************
 * @function_name:  SYS_VALVE_Close
 * @function_file:  dev_valve.c
 * @����: �رշ���
 * 
 * @����: 
 * @����: 
 * @����: yzy (2018-04-25)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
void SYS_VALVE_Close(void);

/*******************************************************************************
 * @function_name:  SYS_VALVE_Open
 * @function_file:  dev_valve.c
 * @����: ��������
 * 
 * @����: 
 * @����: 
 * @����: yzy (2018-04-25)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
void SYS_VALVE_Open(void);

/*******************************************************************************
 * @function_name:  SYS_VALVE_Status_Get
 * @function_file:  dev_valve.c
 * @����: ��ȡ����
 * 
 * @����: 
 * @����: 
 * @����: yzy (2018-04-25)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
eHWStatus SYS_VALVE_Status_Get(void);

typedef struct 
{
    void (*Init)(void);
    void (*Open)(void);
    void (*Close)(void);
    eHWStatus (*getStat)(void);
}_F_VALVE;

EXT_DEV_VALVE const _F_VALVE gs_DEVValve;

#endif