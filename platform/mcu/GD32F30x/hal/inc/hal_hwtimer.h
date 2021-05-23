/******************************Copyright(c)******************************
**                                
**
** File Name: hal_hwtimer.h
** Author: 
** Date Last Update: 2015-05-18
** Description: ͨ��Ӳ����ʱ��
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: �ļ�����
*************************************************************************/
#ifndef _HAL_HWTIMER_H_
#define _HAL_HWTIMER_H_

#ifndef EXT_HAL_HWTIMER
#define EXT_HAL_HWTIMER    extern
#endif




/************************************************************************
 * @����:timer����ԭ��
 ************************************************************************/
typedef void (*THwTimerFunc)(void*);



/************************************************************************
 * @Function: HAL_InitSysTimer
 * @Description: ���ܳ�ʼ��
 * @Arguments: 
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/18
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void HAL_InitSysTimer(void);



/************************************************************************
 * @Function: SysTimer_DlyConfig
 * @Description: ����ϵͳ��ʱ��
 * 
 * @Arguments: 
 * @param: func ��ʾ��ǰ·�жϲ���֮����õĺ���
 * @param: fpara �жϴ������Ĵ������
 * @param: tmrIdx ��ʾ�ڼ�·
 * @param: mills ��ʾ�����ʱ�Ƕ���ms
 * 
 * @Attention: Ӧ��ģ��Ӧ����ϵͳ������ʱ����Ѿ�����Щ�Ĵ������ú�,��������ǽ��̰�ȫ,���жϰ�ȫ 
 * @Return: uint8  
 * @Auther: yzy
 * Date: 2015/5/18
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8 SysTimer_DlyConfig(void (*func)(void*), void* fpara, uint8 tmrIdx, uint16 mills);

/************************************************************************
 * @Function: SysTimer_DlyStart
 * @Description: ����Ӳ����ʱ��
 * 
 * @Arguments: 
 * @param: tmrIdx ��ʾ�ڼ�· 
 * 
 * @Attention: ����������ǽ��̰�ȫ��
 * @Return: uint8  
 * @Auther: yzy
 * Date: 2015/5/18
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8 SysTimer_DlyStart(uint8 tmrIdx);

/************************************************************************
 * @Function: SysTimer_DlyStop
 * @Description: ֹͣӲ����ʱ��
 * 
 * @Arguments: 
 * @param: tmrIdx ��ʾ�ڼ�· 
 * 
 * @Note: ����������ǽ��̰�ȫ��
 * @Return: uint8  
 * @Auther: yzy
 * Date: 2015/5/18
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8 SysTimer_DlyStop(uint8 tmrIdx);




#if (true)   //����Ӳ����ʱ������

/************************************************************************
 * @��ʱ������ԭ��
 ************************************************************************/
typedef void (*TCasHwTimerFunc)(void*);




/************************************************************************
 * @����Ӳ����ʱ����Ϣ�ṹ��
 ************************************************************************/
typedef struct _TCasHwTimer
{
    uint8 stoped;               //�Ƿ�ֹͣ
    uint8 timerid;              //��ʱ��id
    uint16 timeout;             //��ʱ����
    uint16 dlycnt;              //����ʱ������
    TCasHwTimerFunc tickfunc;   //��ʱ������,��������falseʱֹͣtimer
    void* tickpara;             //��ʱ�������������
    struct _TCasHwTimer* pnext;
}TCasHwTimer;


/************************************************************************
 * @Function: casHwTimerCreate
 * @Description: ��������ר�ö�ʱ��(Ŀ��Ϊ�˼���Ӳ����ʱ������������)
 * 
 * @Arguments: 
 * @param: tickfunc ��ʱ������
 * @param: tickpara �������
 * @param: timerid ����Ķ�ʱ����
 * @param: dlytick ��ʱ��tick��
 * 
 * @Note: 
 * @Return: uint8  
 * @Auther: yzy
 * Date: 2015/6/3
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8 casHwTimerCreate(TCasHwTimerFunc tickfunc, void* tickpara, uint8 timerid, uint16 dlytick);


/************************************************************************
 * @Function: casHwTimerStart
 * @Description: ����timer
 * 
 * @Arguments: 
 * @param: timerid 
 * 
 * @Note: 
 * @Return: TResult  
 * @Auther: yzy
 * Date: 2015/6/1
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8 casHwTimerStart(uint8 timerid);


/************************************************************************
 * @Function: casHwTimerStop
 * @Description: ֹͣtimer
 * 
 * @Arguments: 
 * @param: timerid 
 * 
 * @Note: 
 * @Return: TResult  
 * @Auther: yzy
 * Date: 2015/6/1
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8 casHwTimerStop(uint8 timerid);


/************************************************************************
 * @Function: casHwTimerConfig
 * @Description: ����
 * 
 * @Arguments: 
 * @param: sysTimerID �󶨵�systimerID
 * @param: tickMills ���ζ�ʱ�жϵĺ�����
 * 
 * @Note: 
 * @Return: uint8  
 * @Auther: yzy
 * Date: 2015/6/3
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8 casHwTimerConfig(uint8 sysTimerID, uint16 tickMills);

uint8 casHwTimerClose(uint8 timerid);

#endif              //#if (true)   //����Ӳ����ʱ������


#endif

