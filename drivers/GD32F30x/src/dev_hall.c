/***********************************************************************************************************************
* �ļ�����dev_hall.c
* ��  ����
* ��  ����������������Ӧ��Դ�ļ�
* �����ߣ�
* ��  �ڣ�
***********************************************************************************************************************/

#define EXT_HALL

#include "sys.h"
#include "hal.h"
#include "bsp.h"
#include "iomap.h"
#include "logc_event.h"
#include "reportApi.h"
#include "measureApi.h"
#include "WOSsys.h"
#define HWTIMER_HALL_CHECK_ID  12

ST_HALL_COUNT    HallCount = {0, 0};                 //��������ֵ   
HallCheck_ST    HallA;                              //HallA�жϽṹ�� 
HallCheck_ST    HallB;                              //HallB�жϽṹ�� 
static unsigned char HallState = HALL_ISNORMAL;     //����״̬
//static unsigned char Timer_HallCheck = 0;           //Hall�Ź��������ⶨʱ�����


/*******************************************************************************
 * ��ȡ����A  IO��״̬
 * @return
 ******************************************************************************/
/* unsigned char GpioHallARead(void) */
unsigned char PCB_HallARead(void)
{
    return HALLA_GETSTT();
}

/*******************************************************************************
 * ��ȡ����B  IO��״̬
 * @return
 ******************************************************************************/
/* unsigned char GpioHallBRead(void)//��� */
unsigned char PCB_HallBRead(void)
{
    return HALLB_GETSTT();
}
/*
 * ��ȡ��ǰHall״̬
 */
unsigned char GetHallState(void)
{
    return HallState;
}
/*
 * ����A�ж�
 */
void HallACheck(void)
{   
    HallA.Count++;
    
    if(HallA.Step ==0 ) //�˲�(ͨѶ����)
    {
        if(HallA.GetPortInput() == 1) //�˲��ڼ��⵽�ߵ�ƽ�������������
        {
            HallA.Run   = 0;
        }
        else
        {
            if(HallA.Count >= HallA.TimeFilter) //�˲�ʱ�䵽��ת�Ź����ж�
            {
                HallA.Count = 0;
                HallA.Step  = 1;
            }
        }
    }
    else //Step == 1    �Ź����ж�
    {
        if(HallB.GetPortInput() == 1)    //HallBΪ�ߵ�ƽ�������Ź�����⣬����Ч�����ź�
        {
            HallA.HallPulseFun();
            HallA.Run   = 0;
        }
        else        //HallBΪ�͵�ƽʱ��������Ƿ��дŹ���
        {
            if(HallA.Count >= HallA.TimeHallAttack) //�Ź����ж�ʱ�䵽���ǴŹ�����Ч
            {
                HallA.HallAttackFun();
                HallA.Run = 0;
            }   
        }
    }
}

/*
 * ����B�ж�
 */
void HallBCheck(void)
{       
    HallB.Count++;
    
    if(HallB.Step ==0 ) //�˲�(ͨѶ����)
    {
        if(HallB.GetPortInput() == 1) //�˲��ڼ��⵽�ߵ�ƽ�������������
        {
            HallB.Run   = 0;
        }
        else 
        {
            if(HallB.Count >= HallB.TimeFilter) //�˲�ʱ�䵽��ת�Ź����ж�
            {
                HallB.Count = 0;
                HallB.Step  = 1;
            }
        }
    }
    else //Step == 1    �Ź����ж�
    {     
        if(HallA.GetPortInput() == 1)    //HallAΪ�ߵ�ƽ�������Ź�����⣬����Ч�����ź�
        {
            HallB.HallPulseFun();
            HallB.Run   = 0;
        }
        else        //HallAΪ�͵�ƽʱ��������Ƿ��дŹ���
        {
            if(HallB.Count >= HallB.TimeHallAttack) //�Ź����ж�ʱ�䵽���ǴŹ�����Ч
            {
                HallB.HallAttackFun();
                HallB.Run = 0;
            }   
        }
    }
}

/*
 * HALL��ʱ���
 */
void HallCheck(void * para)
{
    if(HallA.Run == 1)
    {
        HallACheck();
    }
    if(HallB.Run == 1)
    {
        HallBCheck();
    }
    if((HallA.Run == 0) && (HallB.Run == 0))  //û�л������������
    {
//        SYS_TimerStop(SYS_LPTIMER);
        casHwTimerStop(HWTIMER_HALL_CHECK_ID);
    } 
}

/**
 * �Ź����ص�����
 */
void HallAttackCall(void)
{
    if(HallState == HALL_ISERR)         //�Ѿ����쳣״̬
    {
        return;
    }
    if(IsIotUpcomingBusy() != 1)     //�ϸ��ڼ䲻���Ź��������и���
    {
        HallState = HALL_ISERR;
        APP_EventMutexClean(EVT_TYPE_COUNTPAY, APP_EVT_CNT_EMI);
        APP_EventWrite(EVT_TYPE_COUNTPAY, APP_EVT_CNT_EMI);
    }
}

/*
 * �Ź����ж�
 */
void HallAttackCheck(void)
{
    if((PCB_HallARead()) || (PCB_HallBRead()))            //δ��⵽�Ź���
    {   
        APP_EventMutexClean(EVT_TYPE_COUNTPAY, APP_EVT_CNT_EMI);
        APP_EventWrite(EVT_TYPE_COUNTPAY, APP_EVT_CNT_EMICANCEL);         //��Ź����¼� 
        HallState = HALL_ISNORMAL;
    }
    else
    {
        APP_EventMutexClean(EVT_TYPE_COUNTPAY, APP_EVT_CNT_EMI);
        APP_EventWrite(EVT_TYPE_COUNTPAY, APP_EVT_CNT_EMI);   //�ôŹ����¼�
        HallState = HALL_ISERR;
    }
}

/*
 * �Ź��������⣬ֻ����������
 */
void HallAttackClrCheck(void)
{
    if(HallState == HALL_ISERR)
    {
        if((PCB_HallARead()) || (PCB_HallBRead()))            //δ��⵽�Ź���
        {   
            APP_EventMutexClean(EVT_TYPE_COUNTPAY, APP_EVT_CNT_EMI);
            APP_EventWrite(EVT_TYPE_COUNTPAY, APP_EVT_CNT_EMICANCEL);         //��Ź����¼� 
            HallState = HALL_ISNORMAL;
        }
    }
}

/*
 * HallA��Ч�����ص�����
 */
void HallAPulse(void)
{
    HallCount.HallACount++;
}

/* 
 * HallA�жϣ�����������ʱ
 */
void HallAInt(void * para)
{ 
    HallA.Count = 0;
    HallA.Step  = 0;
    HallA.Run   = 1;
    casHwTimerStart(HWTIMER_HALL_CHECK_ID);

}

/*
 * HallB��Ч�����ص�����
 */
void HallBPulse(void)
{
    u8 ucdata;
    
    if(HallCount.HallACount > 0)
    {
        HallCount.HallACount = 0;
        HallCount.HallBCount = 0;
        ucdata = 0x5b;
        SysOptDat(0x0cffffe0,BIT7,0,1,&ucdata);//��ȡ�����������
    }
    else
    {
        HallCount.HallBCount++;
    }   
}

/* 
 * HallB�жϣ�����������ʱ
 */
void HallBInt(void * para)
{
    HallB.Count = 0;
    HallB.Step  = 0;
    HallB.Run   = 1;
//    SYS_TimerStart(SYS_LPTIMER);
    casHwTimerStart(HWTIMER_HALL_CHECK_ID);

}
/*
 *  ������ʼ��
 */
void InitHall(void)
{     
//	    PCB_HALLA_DIRCFGIN;
//	    PCB_HALLB_DIRCFGIN;
//	    PCB_HallASetInt(1, HallAInt);
//	    PCB_HallBSetInt(1, HallBInt);
    
    HAL_GPIO_PinConfig(HALLA_PORT, HALLA_PIN, IN_PULLUP, ANALOG_DISENABLE, GPIO_DIR_IN);
	HAL_GPIO_EInt_Cfg(EXT_INT_CHANNL_HALLA, HALLA_PORT, HALLA_PIN, GPIO_FALLING, HallAInt, NULL);
	HAL_GPIO_PinConfig(HALLB_PORT, HALLB_PIN, IN_PULLUP, ANALOG_DISENABLE, GPIO_DIR_IN);
	HAL_GPIO_EInt_Cfg(EXT_INT_CHANNL_HALLB, HALLB_PORT, HALLB_PIN, GPIO_FALLING, HallBInt, NULL);

    
    HallA.Run   = 0;
    HallA.Step  = 0;
    HallA.Count = 0;
    HallA.TimeFilter        = 20;        //20*10 = 200ms
    HallA.TimeHallAttack    = 20;        //20*10 = 200ms   
    HallA.GetPortInput      = PCB_HallARead;
    HallA.HallPulseFun      = HallAPulse;
    HallA.HallAttackFun     = HallAttackCall;   //�Ź����ص�����
    
    HallB.Run   = 0;
    HallB.Step  = 0;
    HallB.Count = 0;
    HallB.TimeFilter        = 20;        //20*10 = 200ms
    HallB.TimeHallAttack    = 20;        //20*10 = 200ms   
    HallB.GetPortInput      = PCB_HallBRead;
    HallB.HallPulseFun      = HallBPulse;
    HallB.HallAttackFun     = HallAttackCall;   //�Ź����ص�����
    
//	    SYS_TimerCfg(SYS_LPTIMER, 0, HallCheck);    //����Hall���ר�ö�ʱ��
    
    casHwTimerCreate(HallCheck, NULL, HWTIMER_HALL_CHECK_ID, 10);
    HallAttackCheck();                          //��ʼ��ʱ�ȼ���»���״̬
    //Timer_HallCheck = SYS_AskTimeNumber();    //��������Ź��������ⶨʱ�����
    s_S_MSK_INT_TYPE sTmType;
    sTmType.ucType = SYS_TM_1s;
    sTmType.ucPri = 0;  ///���ü���ģ��500ms�Ķ�ʱ�����ȼ�
    SMskSetTmFunc(sTmType,&HallAttackClrCheck);
//	    SYS_StartTime(SYS_SUBTASK, ltid_HallNmlChk, 1, HallAttackClrCheck);  //ÿ1s����һ��
}



