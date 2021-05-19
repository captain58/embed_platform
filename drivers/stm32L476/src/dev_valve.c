/****************************************Copyright (c)**************************************************
**                                  ________�Ƽ����޹�˾
**                                          ������
**
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: dev_valve.c
**��   ��   ��: yzy
**����޸�����: 2018��04��25��
**��        ��: ����ģ��
*********************************************************************************************************/
#define EXT_DEV_VALVE

#include "include.h"
//	#include "k_types.h"
//	#include "k_api.h"
//	#include "lib_app.h"
//	#include "hal.h"


static kmutex_t mutex_Valve;
static eHWStatus valveStatus=eHWStatus_VALVE_UNKNOW; //��ǰ����״̬

void SYS_VALVE_Init()
{
    krhino_mutex_create(&mutex_Valve,"ValveMutex");
}


/**
 * �ط�
 * 
 * @author Zzzzz (2018/3/14)
 */
void SYS_VALVE_Close(void)
{
    uint8_t time=0;
    if((valveStatus != eHWStatus_VALVE_CLOSED) && (valveStatus != eHWStatus_VALVE_CLOSING))
    {
        krhino_mutex_lock(&mutex_Valve,RHINO_WAIT_FOREVER);
//	        portValveClose();
//	            IO_M_SLEEP_1;
//	    IO_M_CTRL1_1;
//	    IO_M_CTRL2_0;
        SYS_GPO_Out(GPO_VALVE_SLEEP,1);
        SYS_GPO_Out(GPO_VALVE_OPEN,1);
        SYS_GPO_Out(GPO_VALVE_CLOSE,0);
        valveStatus=eHWStatus_VALVE_CLOSING;
//	        while(time<HW_VALVE_CLOSETIME)
//	        {
//	            time++;
//	            krhino_task_sleep(1);
//	        }
        sleep_ms(SYS_VALVE_CLOSETIME);


//	        portValveStop();
        SYS_GPO_Out(GPO_VALVE_SLEEP,0);
        SYS_GPO_Out(GPO_VALVE_OPEN,0);
        SYS_GPO_Out(GPO_VALVE_CLOSE,0);

        krhino_mutex_unlock(&mutex_Valve);
        valveStatus = eHWStatus_VALVE_CLOSED;
    }
}

/**
 * ����
 * 
 * @author Zzzzz (2018/3/14)
 */
void SYS_VALVE_Open(void)
{
    uint8_t time=0;
    if((valveStatus != eHWStatus_VALVE_OPEN) && (valveStatus != eHWStatus_VALVE_OPENING))
    
    {
        krhino_mutex_lock(&mutex_Valve,RHINO_WAIT_FOREVER);
        SYS_GPO_Out(GPO_VALVE_SLEEP,1);
        SYS_GPO_Out(GPO_VALVE_OPEN,0);
        SYS_GPO_Out(GPO_VALVE_CLOSE,1);
        valveStatus=eHWStatus_VALVE_OPENING;
//	        while(time<HW_VALVE_OPENTIME)
//	        {
//	            time++;
//	            krhino_task_sleep(1);
//	        }
        sleep_ms(SYS_VALVE_OPENTIME);

        SYS_GPO_Out(GPO_VALVE_SLEEP,0);
        SYS_GPO_Out(GPO_VALVE_OPEN,0);
        SYS_GPO_Out(GPO_VALVE_CLOSE,0);
        krhino_mutex_unlock(&mutex_Valve);
        valveStatus = eHWStatus_VALVE_OPEN;
    }
}

eHWStatus SYS_VALVE_Status_Get(void)
{
    return valveStatus;
}

#if (SYS_VALVE_EN > 0)
const _F_VALVE gs_DEVValve = 
{
    SYS_VALVE_Init,
    SYS_VALVE_Open,
    SYS_VALVE_Close
};
#endif
