/******************************Copyright(c)******************************
**                          
**
** File Name: dev_msgloop.c
** Author: 
** Date Last Update: 2019-11-02
** Description: ��Ϣѭ������
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: �ļ�����
*************************************************************************/
#define EXT_DEV_MSGLOOP
#ifdef __MODULE__
#include "WOSsys.h"
#endif
#include "sys.h"
#include "hal.h"
#include "bsp.h"
#include "apis.h"

/************************************************************************
 * @ģ�������ʾ
 ************************************************************************/
#if !defined(ID_SWTIMER_MSG)
    #define ID_SWTIMER_MSG    0xff
    #warning "�붨����Ϣѭ��ɨ�趨ʱ��ID��ź�"
#endif


#ifndef __NO_SYS__ 




/************************************************************************
 * @Function: MSG_Server
 * @Description: ��Ϣ�ַ���ʱ����
 * 
 * @Arguments: 
 * @param: pdata 
 * 
 * @Note: 
 * @Return: bool  
 * @Auther: yzy
 * Date: 2015/5/27
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
bool MSG_Server(void* pdata)
{
#if (SYS_GPI_EN > 0)
    SYS_GPI_EvtMessageLoop();
#endif
 
#if (SYS_UART_EN > 0)
    SYS_UART_EvtMessageLoop();
#endif
    return true;
}


void SYS_MSG_PreInit(void)
{

}


/************************************************************************
 * @Function: SYS_MSG_Init
 * @Description: ��Ϣ�ַ�ģ���ʼ��
 * @Arguments: 
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/27
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void SYS_MSG_Init(void)
{
    memset_s(gucs_MsgApplied, 0, sizeof(gucs_MsgApplied));
#if (SYS_UART_EN > 0)
    memset_s((uint8*)gucs_MsgUartTkMap, 0, sizeof(gucs_MsgUartTkMap));
#endif
    
    //������Ϣ�ַ�����ʱ��
    SYS_Timer_Create(MSG_Server, __NULL, 1, ID_SWTIMER_MSG, false);
}








#else
#include "define.h"
//#include "common\Sys\SysMsk\SMsk.h"
//#include "common\Sys\SysMsk\SMskExt.h"
/************************************************************************
 * @Function: MSG_Server
 * @Description: ��Ϣ�ַ���ʱ����
 * 
 * @Arguments: 
 * @param: pdata 
 * 
 * @Note: 
 * @Return: bool  
 * @Auther: yzy
 * Date: 2015/5/27
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void MSG_Server(void * para)
{
#if (SYS_GPI_EN > 0)
    SYS_GPI_EvtMessageLoop();
#endif
 
#if (SYS_UART_EN > 0)
    SYS_UART_EvtMessageLoop();
#endif
    //return true;
    
//	#if (SYS_LGPI_EN > 0)
//	    extern void SYS_LGPI_Scan(void *arg);
//	
//	    SYS_LGPI_Scan(NULL);
//	#endif

#if (SYS_LED_EN > 0)
    extern bool LED_Server(void* pdata);
    
    LED_Server(NULL);
#endif  

}

void SYS_MSG_PreInit(void)
{
    memset_s(gucs_MsgApplied, 0, sizeof(gucs_MsgApplied));
#if (SYS_UART_EN > 0)
    memset_s((uint8*)gucs_MsgUartTkMap, 0, sizeof(gucs_MsgUartTkMap));
#endif
}
#define HWTIMER_MSGLOOP_ID 13
/************************************************************************
 * @Function: SYS_MSG_Init
 * @Description: ��Ϣ�ַ�ģ���ʼ��
 * @Arguments: 
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/27
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void SYS_MSG_Init(void)
{

#ifdef  __NO_SYS__    
//	    s_S_MSK_INT_TYPE sTmType;
//	
//	    sTmType.ucType = SYS_TM_125ms;
//	    sTmType.ucPri = 0;  ///
//	    //������Ϣ�ַ�����ʱ��
//	//	    SYS_Timer_Create(MSG_Server, __NULL, 1, ID_SWTIMER_MSG, false);
//	    SMskSetTmFunc(sTmType, MSG_Server);
    
    casHwTimerCreate(MSG_Server, NULL, HWTIMER_MSGLOOP_ID, 100);
    casHwTimerStart(HWTIMER_MSGLOOP_ID);

#else
    SYS_Timer_Create(MSG_Server, __NULL, 1, ID_SWTIMER_MSG, false);
#endif
}

void SYS_MSG_DeInit(void)
{
//    s_S_MSK_INT_TYPE sTmType;
//	    sTmType.ucType = SYS_TM_125ms;
//	    sTmType.ucPri = 0;  ///
//	    SMskClrTmFunc(SYS_TM_125ms);
    casHwTimerClose(HWTIMER_MSGLOOP_ID);
}

#endif
/************************************************************************
 * @Function: SYS_MSG_Apply
 * @Description: ������Ϣ
 * 
 * @Arguments: 
 * @param: msgcls ��Ϣ����
 * 
 * @Note: 
 * @Return: bool  
 * @Auther: yzy
 * Date: 2015/5/27
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
bool SYS_MSG_Apply(uint8 tkid, uint8 msgcls)
{
//#ifndef __NO_SYS__ 
//    uint8 tkid = gs_TkTcbCur->tkid;
//#else
//    uint8 tkid = SMskGetMdlNo();
//    if(tkid == 0xff)
//    {
//        tkid = 0;
//    }
//#endif

    
    if(msgcls >= 8)
    {
        return false;
    }
    
#if (SYS_UART_EN > 0)                        //��������Ϣ,�������Ϊ���ڱ��
    if(msgcls == MSG_CLS_UART)
    {
        for(uint8 index = 0; index < NO_OF_SERIAL; index++)
        {
            STR_SetBit(gucs_MsgUartTkMap[index], tkid);
        }
    }
#endif
                                            //��Ǵ���
    gucs_MsgApplied[tkid] |= Bit_Map8[msgcls];
        
    return true;
}

/************************************************************************
 * @Function: SYS_MSG_ApplyExt
 * @Description: ������Ϣ(���亯��)
 * 
 * @Arguments: 
 * @param: msgcls ��Ϣ����
 * @param: subcls ��ϢС��,������Ϊ������Ϣʱ,С��Ϊ���ں�
 * 
 * @Note: 
 * @Return: bool  
 * @Auther: yzy
 * Date: 2015/5/27
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8_t SYS_MSG_ApplyExt(uint8 tkid, uint8_t msgcls, uint8_t subcls)
{
#ifndef __NO_SYS__ 
    tkid = gs_TkTcbCur->tkid;
//#else
//    uint8 tkid = SMskGetMdlNo();
#endif
    if(msgcls >= 8)
    {
        return 1;
    }
    
#if (SYS_UART_EN > 0)                        //��������Ϣ,�������Ϊ���ڱ��
    extern const uint8_t _ucPortMap[];
    if(msgcls == MSG_CLS_UART)
    {
        if(_ucPortMap[subcls] > NO_OF_SERIAL)
        {
            return 1;
        }
        STR_SetBit(gucs_MsgUartTkMap[_ucPortMap[subcls]], tkid);
    }
#endif
                                            //��Ǵ���
    gucs_MsgApplied[tkid] |= Bit_Map8[msgcls];
        
    return 0;
}


