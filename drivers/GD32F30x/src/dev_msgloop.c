/******************************Copyright(c)******************************
**                          
**
** File Name: dev_msgloop.c
** Author: 
** Date Last Update: 2019-11-02
** Description: 消息循环机制
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: 文件创建
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
 * @模块编译提示
 ************************************************************************/
#if !defined(ID_SWTIMER_MSG)
    #define ID_SWTIMER_MSG    0xff
    #warning "请定义消息循环扫描定时器ID编号宏"
#endif


#ifndef __NO_SYS__ 




/************************************************************************
 * @Function: MSG_Server
 * @Description: 消息分发定时处理
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
 * @Description: 消息分发模块初始化
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
    
    //创建消息分发服务定时器
    SYS_Timer_Create(MSG_Server, __NULL, 1, ID_SWTIMER_MSG, false);
}








#else
#include "define.h"
//#include "common\Sys\SysMsk\SMsk.h"
//#include "common\Sys\SysMsk\SMskExt.h"
/************************************************************************
 * @Function: MSG_Server
 * @Description: 消息分发定时处理
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
 * @Description: 消息分发模块初始化
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
//	    //创建消息分发服务定时器
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
 * @Description: 申请消息
 * 
 * @Arguments: 
 * @param: msgcls 消息大类
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
    
#if (SYS_UART_EN > 0)                        //串口类消息,标记子类为串口编号
    if(msgcls == MSG_CLS_UART)
    {
        for(uint8 index = 0; index < NO_OF_SERIAL; index++)
        {
            STR_SetBit(gucs_MsgUartTkMap[index], tkid);
        }
    }
#endif
                                            //标记大类
    gucs_MsgApplied[tkid] |= Bit_Map8[msgcls];
        
    return true;
}

/************************************************************************
 * @Function: SYS_MSG_ApplyExt
 * @Description: 申请消息(扩充函数)
 * 
 * @Arguments: 
 * @param: msgcls 消息大类
 * @param: subcls 消息小类,当大类为串口消息时,小类为串口号
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
    
#if (SYS_UART_EN > 0)                        //串口类消息,标记子类为串口编号
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
                                            //标记大类
    gucs_MsgApplied[tkid] |= Bit_Map8[msgcls];
        
    return 0;
}


