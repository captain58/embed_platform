/****************************************Copyright (c)**************************************************
**                                  ________�Ƽ����޹�˾
**                                          ������
**
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: dev_lowpwr.c
**��   ��   ��: yzy
**����޸�����: 2018��04��25��
**��        ��: �͹���ģ��
*********************************************************************************************************/

#define EXT_DEV_LOWPWR

#include "include.h"


/***********************************************************************/
static uint16_t sleepDelay;
static uint32_t hwBusy=0;

/**
 * ��Ƭ���������ߣ��ر�����
 * 
 * @author Zzzzz (2018/4/1)
 * 
 * @param void 
 */
void SYS_LOWPWR_Sleep(void)
{
    if(hwBusy==0 && sleepDelay==0)
    {
        NOP();  /* Add one instruction delay */
        STOP(); /* Enter STOP mode */
        NOP();  /* Add one instruction delay */
    }
    
    if(sleepDelay>0)
        sleepDelay--;
}

uint32_t SYS_LOWPWR_Set_Busy(LOWPWRENUM module )
{
    hwBusy |= (uint32)1 << module;

    return hwBusy;
}

uint32_t SYS_LOWPWR_Clear_Busy(LOWPWRENUM module )
{
    hwBusy &= ~((uint32)1 << module);
    return hwBusy;
}

/**
 * ���Ƶ�Ƭ����Ҫ���ߵ�ʱ��
 * 
 * @author Zzzzz (2018/3/28)
 * 
 * @param tVal ��λtick
 */
void SYS_LOWPWR_Delay(uint16_t tVal)
{
    sleepDelay=tVal;
}


const _F_LOWPWR gs_DEVLowPwr = 
{
    SYS_LOWPWR_Sleep,
    SYS_LOWPWR_Set_Busy,
    SYS_LOWPWR_Clear_Busy,
    SYS_LOWPWR_Delay
};


