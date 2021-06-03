/****************************************Copyright (c)*************************
**                               	________�Ƽ����޹�˾
**                                     		������
**
**
**--------------�ļ���Ϣ-------------------------------------------------------
**��   ��   ��: dev_messageloop.c
**��   ��   ��: yzy
**����޸�����: 2011��04��27��
**��        ��: ��״̬������������
**ע        ��:
**--------------��ʷ�汾��Ϣ---------------------------------------------------
** ������: yzy
** ��  ��: v1.0
** �ա���: 2011��04��27��
** �衡��: ԭʼ�汾
******************************************************************************/
#define EXT_DEV_MSGLOOP

#include "aos/include.h"
//	#include "k_types.h"
//	#include "k_api.h"
//	//	#include "lib_app.h"
#include "hal.h"
#include "bsp.h"
#include "tasks.h"

extern uint8 gucs_UartRevFlag[NO_OF_SERIAL];
/************************************************************************
 * @function: UartRevMessageDelivery
 * @����: ��ĳ�����ڽ�����Ϣ,�ַ����������Ϣ�Ľ���
 * 
 * @����: 
 * @param: uartidx ����
 * @param: msg_recv ��Ӧ�Ĵ��ڽ�����Ϣ
 * @����: 
 * @˵��: 
 * @����: yzy (2013/11/4)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void UartRevMessageDelivery(uint8 uartidx, uint8 msg_recv)
{
    uint8 subtk;
    uint8 tkmap;
    
    extern const KTaskDeclare __TKDeclare[SYS_TK_NUM];
    const KTaskDeclare* dec = __TKDeclare;
    
    for(uint8 uc_i = 0; uc_i < sizeof(guc_MsgUartTkMap[0]); uc_i++)
    {
        tkmap = guc_MsgUartTkMap[uartidx][uc_i];
        while(tkmap)
        {
            subtk = Bit_Maps[tkmap];
//	            SYS_Message_Send(msg_recv, (uc_i << 3) + subtk);
//	
//	            msg = MSG_YEAR;
//	            dec+=(uc_i << 3) + subtk;
            krhino_buf_queue_send(dec[(uc_i << 3) + subtk].ktask->msg, &msg_recv, 1);

            
            tkmap &= ~Bit_Map8[subtk];
        }
    }
}



/*******************************************************************************
 * @function_name:  SYS_UART_RevMessageLoop
 * @function_file:  dev_msgloop.c
 * @����: UART�������ݽ�����Ϣ�ַ�����
 * 
 * 
 * @����: 
 * 
 * @����:
 * @����: yzy (2018��3��22��)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
void SYS_UART_RevMessageLoop(void)
{
    for(uint8 uartidx = 0; uartidx < NO_OF_SERIAL; uartidx++)
    {
//	        if(SER_ToBeRead_By_Port(uartidx) > 0)
        if(gucs_UartRevFlag[uartidx])
        {                               //���ڻص�����,��ִ��
//	            if(gfs_SerialCallBack[uartidx] != __NULL)
//	            {
//	                gfs_SerialCallBack[uartidx]();
//	            }
//	            else                        //����,���н�����Ϣ�ַ�
//	            {
            UartRevMessageDelivery(uartidx, MSG_UART0 + uartidx);
//	            }
            gucs_UartRevFlag[uartidx] = 0;
        }
    }
}



/*******************************************************************************
* @function_name:  MSG_Server
* @function_file:  dev_messageloop.c
* @����: ��Ϣ���������̴�����
* 
* 
* @����: 
* 
* @����: 
* @����: yzy (2011-02-24)
*-----------------------------------------------------------------------------
* @�޸���: 
* @�޸�˵��: 
******************************************************************************/
void MSG_Server(void *timer, void *arg)
{
#if SYS_GPI_EN > 0
    SYS_GPI_EvtMessageLoop();
#endif
#if SYS_SER_EN > 0
//    SYS_SER_RevMessageLoop();

    SYS_UART_RevMessageLoop();
#endif
#if (SYS_LGPI_EN > 0)
    extern void SYS_LGPI_Scan(void *timer, void *arg);

    SYS_LGPI_Scan(timer, arg);
#endif

#if (SYS_FGPI_EN > 0)
    extern void SYS_FGPI_Scan(void *timer, void *arg);

    SYS_FGPI_Scan(timer, arg);
#endif
    
#if (SYS_LED_BEEP_EN > 0)
    extern void LED_Server(void *timer, void *arg);
	
    LED_Server(timer,arg);
#endif    
}


ktimer_t     g_msg_server_timer;


/*******************************************************************************
* @function_name:  SYS_MSG_Init
* @function_file:  dev_messageloop.c
* @����: ��Ϣ��������ʼ��
* 
* 
* @����: 
* 
* @����: 
* @����: yzy (2011-02-24)
*-----------------------------------------------------------------------------
* @�޸���: 
* @�޸�˵��: 
******************************************************************************/
void SYS_MSG_Init(void)
{
    ClearBuffer(guc_MsgApplied, sizeof(guc_MsgApplied));
    ClearBuffer((uint8*)guc_MsgUartTkMap, sizeof(guc_MsgUartTkMap));
    
    //������Ϣ�ַ�����ʱ��
//	    SYS_Timer_Create(MSG_Server, __NULL, 1, TIMER_ID_MSG, false);
    krhino_timer_create(&g_msg_server_timer, "msg_server_timer", MSG_Server,
                        krhino_ms_to_ticks(100), krhino_ms_to_ticks(100), 0, 1);


}




/*******************************************************************************
* @function_name:  SYS_MSG_Apply
* @function_file:  dev_messageloop.c
* @����: ������Ϣ
* 
* 
* @����: msgcls ��Ϣ����
* @����:  
* @����: yzy (2011-02-24)
******************************************************************************/
uint8_t SYS_MSG_Apply(uint32_t tkid, uint8_t msgcls)
{
    
    if(msgcls >= 8)
    {
        return false;
    }

    
    if(msgcls == MSG_CLS_UART)
    {
        for(uint8 index = 0; index < NO_OF_SERIAL; index++)
        {
            STR_SetBit(guc_MsgUartTkMap[index], tkid);
        }
    }
                                            //��Ǵ���
    guc_MsgApplied[tkid] |= Bit_Map8[msgcls];
    if(msgcls == MSG_CLS_GPIO)
    {
        SYS_GPI_Event_Clear();
    }
    return true;
}


/*******************************************************************************
* @function_name:  SYS_MSG_ApplyUartRecv
* @function_file:  dev_messageloop.c
* @����: ������Ϣ(���亯��)
* 
* 
* @����: msgcls ��Ϣ����
*        subcls ��ϢС��,������Ϊ������Ϣʱ,С��Ϊ���ں�
* @����:  
* @����: yzy (2011-02-24)
******************************************************************************/
uint8 SYS_MSG_ApplyExt(uint8 tkid, uint8 msgcls, uint8 subcls )
{
//	    uint8 tkid = gs_TkTcbCur->tkid;
//	    
    if(msgcls >= 8)
    {
        return false;
    }
                                            //��������Ϣ,�������Ϊ���ڱ��
    if(msgcls == MSG_CLS_UART)
    {
//        uint8_t port = ((SerialID*)__Uart_Find(subcls))->port;
//        if(port > NO_OF_SERIAL)
//        {
//            return false;
//        }
        STR_SetBit(guc_MsgUartTkMap[subcls], tkid);
    }
                                            //��Ǵ���
    guc_MsgApplied[tkid] |= Bit_Map8[msgcls];
        
    return true;
}



