/******************************Copyright(c)******************************
**                          
**
** File Name: dev_wdt.c
** Author: 
** Date Last Update: 2019-11-02
** Description: ���Ź��豸
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: �ļ�����
*************************************************************************/
#define EXT_DEV_WDT

#include "sys.h"
#include "hal.h"
#include "bsp.h"







/************************************************************************
 * @Function: WDT_Server
 * @Description: WDT��ʱ�������
 * 
 * @Arguments: 
 * @param: pdata 
 * 
 * @Note: 
 * @Return: bool  
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
bool WDT_Server(void* pdata)
{
    if(guc_TickDogFeedDIS != 0xaa)
    {
        if(guc_TickDogFeedCnt != 0)
        {
            guc_TickDogFeedCnt--;
            HAL_WDT_Feed();
        }
    }
    
    return true;
}





/************************************************************************
 * @Function: SYS_WDT_Init
 * @Description: ���Ź��豸��ʼ��
 * @Arguments: 
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void SYS_WDT_Init(void)
{
    uint16 timeout;

    //ʹ�ܸ߼����Ź�
    guc_TickDogFeedDIS = 0;
    
    //��ʼ��ι��ֵ
    guc_TickDogFeedCnt = 100;
    
    //�������Ʒ���ʱ��
    timeout = SYS_TICK_PER_SEC / 10;
    timeout = (timeout < 1) ? 1 : timeout;
    SYS_Timer_Create(WDT_Server, __NULL, timeout, ID_SWTIMER_WDT, false);
}




/************************************************************************
 * @Function: SYS_WDT_Feed
 * @Description: ι��
 * 
 * @Arguments: 
 * @param: wdttick wdttick.��������(��λ100ms)
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void SYS_WDT_Feed(uint8 wdttick)   
{
    guc_TickDogFeedCnt = wdttick;
}




/************************************************************************
 * @Function: SYS_WDT_DIS
 * @Description: ֹͣι��
 * @Arguments: 
 * @Note: ���޲���ʹ��.ע��,���ⲿ���Ź���ʱ������ڲ����Ź�.�������ʱ�޷���Ч�����ⲿ���Ź�
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void SYS_WDT_DIS(void)
{
    guc_TickDogFeedDIS = 0xaa;
}

