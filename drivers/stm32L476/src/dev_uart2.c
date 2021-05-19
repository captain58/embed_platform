/****************************************Copyright (c)*************************
**                               	______�Ƽ����޹�˾
**                                     		������
**
**
**--------------�ļ���Ϣ-------------------------------------------------------
**��   ��   ��: dev_uart.c
**��   ��   ��: yzy
**����޸�����: 2013��08��28��
**��        ��: UART����ͨ���������
**ע        ��:
**--------------��ʷ�汾��Ϣ---------------------------------------------------
** ������: yzy
** ��  ��: v2.0
** �ա���: 2013��08��28��
** �衡��: ԭʼ�汾
******************************************************************************/
#define EXT_DEV_UART

//	#include "es.h"

#include "aos/include.h"
//	#include "apis.h"
//	#include "k_types.h"
//	#include "k_api.h"
//	#include "uartcfg.h"
//	#include "lib_app.h"
#include "hal.h"
#include <stdarg.h>
#include <stdio.h>
#if (NO_OF_SERIAL == 0)
#error "û�д��ڲ���Ҫ�������ļ�!"
#endif

/************************************************************************
**�����ⲿ����
************************************************************************/
void _UartWriteHook(uint8 port, uint8* buffer, uint16 length);
void _UartReadHook(uint8 port, uint8* buffer, uint16 length);


/************************************************************************
**�����ⲿ����
************************************************************************/
extern const SerialID* const gss_UartSID[NO_OF_SERIAL];

#ifdef EXT_DEV_UART

/******************************************************************************
**���ڻ������״̬
******************************************************************************/
//	EXT_DEV_UART uint8 gucs_UartRevFlag[NO_OF_SERIAL];
//	EXT_DEV_UART uint8 gucs_UartRevStart[NO_OF_SERIAL];
//	EXT_DEV_UART uint8 gucs_UartRevByteTimeout[NO_OF_SERIAL];

//	__SUart g_uartTbl[NO_OF_SERIAL];// = 
//	{
//	//	    {UART_CHANNEL_PARA, SYS_UART0_IDX, false},
//	//	    {UART_CHANNEL_IRDA, SYS_UART1_IDX, false},
//	//	    {UART_CHANNEL_GPRS, SYS_UART2_IDX, false},
//	//	    {UART_CHANNEL_DEBUG, SYS_UART3_IDX, false},    
//	    {UART_CHANNEL_ESAM, SYS_UART0_IDX, false},
//	    {UART_CHANNEL_GPRS, SYS_UART1_IDX, false},
//	    {UART_CHANNEL_DEBUG, SYS_UART2_IDX, false}, 
//	    {UART_CHANNEL_CARD, SYS_UART3_IDX, false},
//	
//	};
#endif


#define __UART_ON_RECV_FLAG				0x1				//�н��������¼���־
#define __UART_ON_SEND_FLAG				0x2				//�з��������¼���־

//	static kevent_t			gflagUartSend;				//���ͱ�־�飬����ͬһͨ���ŷ�������ʱͬ��


/************************************************************************
 * @function: SYS_SER_WriteOption
 * @����: �������ݷ���
 * 
 * @����: 
 * @param: port ���ں�
 * @param: buffer �������ݻ���
 * @param: length �������ݳ���
 * @param: opt ѡ��:b0=1������������,b0=0ϵͳ��������
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2013/8/28)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	uint8 SYS_SER_WriteOption(uint8 port, uint8 * buffer, uint16 length, uint16 opt)
//	{
//	    volatile SerialBuffer* uart = gss_UartSID[port]->uart_Buffer;
//	                                    //�����Ϸ����ж�
//	    if((port >= NO_OF_SERIAL) || (length == 0))
//	    {
//	        return SYS_ERR_FT;
//	    }
//	                                    //���÷��ͽṹ��
//	    uart->tbuff = buffer;           //���͵����ݻ���
//	    uart->tlen = length;            //���ͻ��泤��
//	    uart->tcnt = length;            //���͵��ֽ���
//	    uart->tp = 0;
//	                                    //�������Ͳ��ȴ��������
//	#if SYS_UART9_EN > 0
//	    if(port == SYS_UART9_IDX)
//	    {
//	        Uart9_StartSend(gss_UartSID[port]);
//	    }
//	    else
//	#endif
//	    {
//	        Uartx_StartSend(gss_UartSID[port]);
//	    }
//	//    uart_dev_t
//	#if SER_SEND_HOOK_EN > 0
//	    _UartWriteHook(port, buffer, length);
//	#endif
//	    
//	//	    while(true)
//	//	    {
//	//	        if(uart->tcnt == 0)
//	//	        {
//	//	            break;
//	//	        }
//	//	        
//	//	        if(opt & 0x0001)
//	//	        {
//	//	            //������������
//	//	        }
//	//	        else
//	//	        {
//	//	            //ϵͳ��������,Ĭ�Ϸ�ʽ.
//	//	            sys_sleep(1);
//	//	        }
//	//	    }
//	
//	    
//	    SYS_OK();
//	}


/************************************************************************
 * @function: SYS_SER_Write
 * @����: �������ݷ���
 * 
 * @����: 
 * @param: port ���ں�
 * @param: buffer �������ݻ���
 * @param: length �������ݳ���
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2013/8/28)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	uint8 SYS_SER_Write(uint8 port, uint8* buffer, uint16 length)
//	{
//	    return SYS_SER_WriteOption(port, buffer, length, 0);
//	}

/************************************************************************
 * @function: SYS_SER_Read
 * @����: �������ݶ�ȡ
 * 
 * @����: 
 * @param: port �˿�
 * @param: buffer �������,__NULL��ʾ����
 * @param: length ��ȡ����
 * 
 * @����: 
 * @return: uint16  ʵ�ʶ�ȡ����
 * @˵��: 
 * @����: yzy (2013/8/28)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	uint16 SYS_SER_Read(uint8 port, uint8* buffer, uint16 length)
//	{
//	    SerialBuffer* uart = gss_UartSID[port]->uart_Buffer;
//	    uint16 pr;
//	    uint16 count;
//	    uint16 i;
//	                                   //�����Ϸ����ж�
//	    if((port >= NO_OF_SERIAL) || (length == 0))
//	    {
//	        return 0;
//	    }
//	//	    gss_UartSID[port]->usart_RcvEnable();
//	
//	    CPSR_ALLOC();
//	    RHINO_CPU_INTRPT_DISABLE();
//	    count = uart->rcnt;
//	    if(count > length)              //��ȡʵ�ʶ�ȡ�����ݳ���
//	    {
//	        count = length;
//	    }
//	    RHINO_CPU_INTRPT_ENABLE();
//	                                    //ѭ��ת������
//	    if(__NULL != buffer)            //not dequeue
//	    {
//	        pr = uart->pr;              //���ݶ�ȡ��ʼָ��
//	        for(i = 0; i < count; i++)
//	        {
//	            buffer[i] = uart->rbuff[pr];
//	            pr++;
//	            if(pr >= uart->rlen)    //����
//	            {
//	                pr = 0;
//	            }
//	        }
//	    }
//	    else                            //dequeue
//	    {
//	#if SER_RECV_HOOK_EN > 0
//	        if(count > (uart->rlen - uart->pr))
//	        {
//	            _UartReadHook(port, &uart->rbuff[uart->pr], uart->rlen - uart->pr);
//	            _UartReadHook(port, &uart->rbuff[0], count - (uart->rlen - uart->pr));
//	        }
//	        else
//	        {
//	            _UartReadHook(port, &uart->rbuff[uart->pr], count);
//	        }
//	#endif
//	    }
//	    
//	    uart->pr += count;              //���ݴ���ָ��ǰ��
//	    while(uart->pr > uart->rlen)   //����
//	    {
//	        uart->pr -= uart->rlen;
//	    }
//	    
//	    RHINO_CPU_INTRPT_DISABLE();
//	    uart->rcnt -= count;            //δ�������ݳ��ȵݼ� yzy->rcnt��handler�лᱻ�ı�
//	    RHINO_CPU_INTRPT_ENABLE();
//	
//	#if SER_RECV_HOOK_EN > 0
//	     _IF_TRUE_DO(__NULL != buffer, _UartReadHook(port, buffer, count));
//	#endif
//	
//	    return count;
//	}

/************************************************************************
 * @function: SYS_SER_ToBeRead
 * @����: ���ڿɶ�ȡ���ݵĳ���
 * 
 * @����: 
 * @param: port 
 * 
 * @����: 
 * @return: uint16  
 * @˵��: 
 * @����: yzy (2013/8/28)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
int32_t SER_ToBeRead_By_Port(uint8 ucChannelId)
{

    int32_t ret = -1;
    SerialID *ids = NULL; 

    ids = __Uart_Find(ucChannelId);
    if( NULL== ids ){ 
        return -1;
    }

    return hal_uart_to_be_read(ids->handle);

}






//	void Uart_Read_Callback(uint8 ch)
//	{
//		__SUart * uart = &g_uartTbl[ch];
//	
//	//		uart = __Uart_Find(ch);
//	//		if(!uart)
//	//		{
//	//			return;
//	//		}
//	
//	    if(uart->has_Wait_rcv == true)
//	    {
//	        krhino_event_set(&uart->event, __UART_ON_RECV_FLAG, RHINO_OR);
//	        uart->has_Wait_rcv = false;
//	    }
//	}

//	void Uart_Send_Callback(uint8 ch)
//	{
//	//		__SUart * uart;
//	//	
//	//		uart = __Uart_Find(ch);
//	//		if(!uart)
//	//		{
//	//			return;
//	//		}
//	    __SUart * uart = &g_uartTbl[ch];
//	
//	//	    if(uart->has_Wait_rcv == true)
//	//	    {
//	    krhino_event_set(&uart->event, __UART_ON_SEND_FLAG, RHINO_OR);
//	//	        uart->has_Wait_rcv = false;
//	//	    }
//	}

/*******************************************************************************
 * @function_name:  SYS_SER_RevMessageLoop
 * @function_file:  dev_msgloop.c
 * @����: 100ms,UART�������ݽ�����Ϣ�ַ�����
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
void SYS_SER_RevMessageLoop(void)
{
    for(uint8 uartidx = 0; uartidx < NO_OF_SERIAL; uartidx++)
    {
//          if(SER_ToBeRead_By_Port(uartidx) > 0)
        if(gucs_UartRevStart[uartidx])
        {
            if(gucs_UartRevByteTimeout[uartidx]>0)//�����ֽڼ䳬ʱ
            {
                gucs_UartRevFlag[uartidx] = 1;
                gucs_UartRevStart[uartidx] = 0;
                gucs_UartRevByteTimeout[uartidx] = 0;
//	                Uart_Read_Callback(gss_UartSID[uartidx]->ch);
            }
            gucs_UartRevByteTimeout[uartidx]++;
        }
    }

}


/*==================================================================
//��������  SER_RecvData_Sync
//���ߣ�    ������
//���ڣ�    2018-1-24
//���ܣ�    ���ڽ�������,�ȴ�ͬ����Ϣ���������߳�ʱ����
//���������1��ucChannelId������ͨ����         
//          2��nMaxLen���������յ�������ֽ���
//          3��nTimeout�����ճ�ʱʱ�䣨��λ�����룩����Ϊ0����ʾ���ȴ�
//���������1��strData��Ҫ���յ�����
//����ֵ��  �ɹ�����ʵ�ʽ��յ����ֽ�����ʧ�ܷ���<0
//=================================================================*/
int32_t SER_RecvData(unsigned char ucChannelId, unsigned char *strData, 
							  int32 nMaxLen, int32 nTimeout)
{
//		__SUart * uart = &g_uartTbl[ucChannelId];
//		kstat_t err;
//		int nReadCnt;
//		int nCnt;
//		tick_t nStartTicks;
//		tick_t nLeftTicks;
//	    uint32_t eventInfo = 0;
    uint32_t recv_size;
    int32_t ret = -1;
    SerialID *ids = NULL; 
    ids = __Uart_Find(ucChannelId);
    if( NULL== ids ){ 
        return -1;
    }

    
	if( nMaxLen < 0 || nTimeout < 0)
	{
		return API_ERR_PARA;
	}
	if(!nMaxLen)
	{
		return 0; 
	}
    ret = hal_uart_recv_II(ids->handle, strData, nMaxLen, &recv_size, nTimeout);

    if(ret == 0)
    {
        return recv_size;
    }
    else
    {
        return 0;
    }
//		uart = __Uart_Find(ucChannelId);
//		if(!uart)
//		{
//			return API_ERR_PARA;
//		}
//	    if(nTimeout == 0)//���ȴ�
//	    {
//	        return SYS_SER_Read(ucChannelId, strData, nMaxLen);
//	    }
//	    else
//	    {
//	        if(!strData)
//	        {
//	            return API_ERR_PARA;
//	        }
//	    	nLeftTicks = (nTimeout * RHINO_CONFIG_TICKS_PER_SECOND / 1000) + 1;
//	    	nReadCnt = 0;
//	        gs_DEVLowPwr.Set((LOWPWRENUM)ucChannelId);
//	    	while(nMaxLen > 0)
//	    	{
//	    		//����������������ݣ����ȶ������������
//	    		nCnt = SYS_SER_Read(ucChannelId, strData + nReadCnt, nMaxLen);
//	    		nReadCnt += nCnt;
//	    		nMaxLen -= nCnt;
//	    		if(nMaxLen <= 0 || nLeftTicks <= 0)
//	    		{
//	    			break;
//	    		}
//	
//	    		nStartTicks = krhino_sys_tick_get();
//	    		uart->has_Wait_rcv = true;
//	    		err = krhino_event_get(&uart->event, __UART_ON_RECV_FLAG, RHINO_AND_CLEAR, &eventInfo, nLeftTicks);
//	    		if(err == RHINO_BLK_TIMEOUT || err == RHINO_SUCCESS)
//	    		{
//	    			uart->has_Wait_rcv = false;
//	    			//����ʵ�ֻ��Ƶ�ԭ�򣬿��ܻ�©�����ݶ����³�ʱ��������ȥ��ȡ�������������
//	    			nReadCnt += SYS_SER_Read(ucChannelId, strData + nReadCnt, nMaxLen);
//	    			break;
//	    		}
//	
//	    		uart->has_Wait_rcv = false;
//	    		nLeftTicks -= krhino_sys_tick_get() - nStartTicks;
//	    	}
//	        gs_DEVLowPwr.Clear((LOWPWRENUM)ucChannelId);
//	
//	    	return nReadCnt;
//	    }
}


/*==================================================================
//��������  SER_SendData
//���ߣ�    ������
//���ڣ�    2018-1-24
//���ܣ�    ���ڷ�������
//���������1��ucChannelId������ͨ����
//          2��strData��Ҫ���͵�����
//          3��nDataLen��Ҫ���͵����ݳ���
//          4��nTimeout�����ͳ�ʱʱ�䣨��λ�����룩�� ��Ϊ0�� ��ʾ���ȴ�
//�����������
//����ֵ��  �ɹ�����0��ʧ�ܷ���<0
//=================================================================*/
int32_t SER_SendData(unsigned char ucChannelId, unsigned char *strData, 
							  int32 nDataLen, int32_t nTimeout)
{
//		__SUart * uart = &g_uartTbl[ucChannelId];
//	//	int nIndex;
//	//	unsigned char ucPowerState;
//		kstat_t os_err;
//	    uint32_t eventInfo = 0;
//	    tick_i_t nLeftTicks;
//		tick_i_t nStartTicks;
    SerialID *ids = NULL; 
    ids = __Uart_Find(ucChannelId);
    if( NULL== ids ){ 
        return -1;
    }
    
	if(!strData || nDataLen < 0 || nTimeout < 0)
	{
		return API_ERR_PARA;
	}
	if(!nDataLen)
	{
		return API_SUCCESS; 
	}
    return hal_uart_send(ids->handle, strData, nDataLen, nTimeout);

//		uart = __Uart_Find(ucChannelId);
//		if(!uart)
//		{
//			return API_ERR_PARA;
//		}



//		if(uart->ucType == __UART_TYPE_RS485)
//		{
//			if(API_SUCCESS != POWER_GetStatus(&ucPowerState))
//			{
//				return API_ERR_FAILURE;
//			}
//			if(ucPowerState == POWER_STATUS_OFF)
//			{
//				return API_ERR_FAILURE;
//			}
//		}
//		nLeftTicks = (nTimeout * RHINO_CONFIG_TICKS_PER_SECOND / 1000) + 1;
//	    nStartTicks = krhino_sys_tick_get();
//	    
//		os_err = krhino_event_get(&gflagUartSend, 1 << ucChannelId, RHINO_AND_CLEAR, &eventInfo, nLeftTicks);
//		if (os_err != RHINO_SUCCESS) return API_ERR_FAILURE;
//	    
//	    gs_DEVLowPwr.Set((LOWPWRENUM)ucChannelId);
//	
//		SYS_SER_Write(ucChannelId, (uint8 *)strData, nDataLen);
//	    
//	    krhino_event_set(&gflagUartSend, 1 << ucChannelId, RHINO_OR);
//	
//	
//		nLeftTicks -= krhino_sys_tick_get() - nStartTicks;
//		//nReadCnt = 0;
//		while(1)
//		{
//		    volatile SerialBuffer* serBuf = gss_UartSID[ucChannelId]->uart_Buffer;
//			
//			if(serBuf->tcnt <= 0)
//			{
//				break;
//			}
//	
//			os_err = krhino_event_get(&uart->event, __UART_ON_SEND_FLAG, RHINO_AND_CLEAR, &eventInfo, nLeftTicks);
//			if(os_err == RHINO_BLK_TIMEOUT)
//			{
//			    return API_ERR_FAILURE;
//			}
//	        else if(os_err == RHINO_SUCCESS)
//	        {
//	            return API_SUCCESS;
//	        }
//			nLeftTicks -= krhino_sys_tick_get() - nStartTicks;
//	
//	        if (nLeftTicks <= 0)
//	        {
//	            gs_DEVLowPwr.Clear((LOWPWRENUM)ucChannelId);
//	            return API_ERR_FAILURE;
//	        }
//		}
//	    gs_DEVLowPwr.Clear((LOWPWRENUM)ucChannelId);

//	return API_SUCCESS;
}

/************************************************************************
 * @function: SYS_UART_Init
 * @����: ���ڹ��ܳ�ʼ��
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2013/8/28)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void UART_Init(void)
{
    //name_t name[32];
    
	//__SUart* uart;

//	    uint8 uc_i, uc_j = NO_OF_SERIAL;
    for(uint8 uc_i = 0; uc_i < NO_OF_SERIAL; uc_i++)
    {
        gucs_UartRevFlag[uc_i] = 0;
        gucs_UartRevStart[uc_i] = 0;
        gucs_UartRevByteTimeout[uc_i] = 0;

        hal_uart_init(gss_UartSID[uc_i]->handle);
    
    }
    
//		krhino_event_create(&gflagUartSend, "uart send flag", 0xffffffff);
}








/************************************************************************
 * @function: SYS_SER_Init
 * @����: ��������
 * 
 * @����: 
 * @param: port 
 * @param: ss 
 * @����: 
 * @˵��: 
 * @����: yzy (2013/8/28)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
int32_t SER_Init(uint8 ucChannelId, uart_config_t * ss)
{
    int32_t ret = -1;
    SerialID *ids = NULL; 

    ids = __Uart_Find(ucChannelId);
    if( NULL== ids ){ 
        return -1;
    }
    return hal_uart_reset(ids->handle, ss);


}

/************************************************************************
 * @function: SER_Open
 * @����: ���ڴ�
 * 
 * @����: 
 * @param: port 
 * @param: option TDB_MODE_R:����� .TDB_MODE_W:����д
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2018/5/2)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
int32 SER_Open(uint8 ucChannelId, uint8 option)
{
//	//	    return SER_OpenCallBack(port, option, __NULL);
//	
//	//	    __SUart* uart;
//	//	    uint8 port = 0;
//	//	    uart = __Uart_Find(ucChannelId);
//	//	    if(!uart)
//	//	    {
//	//	        return API_ERR_PARA;
//	//	    }
//	//	    
//	//	    port = uart->port;
//	
//	
//	    Uartx_Enable(gss_UartSID[ucChannelId], option);
//	
    SerialID *ids = NULL; 
    ids = __Uart_Find(ucChannelId);
    if( NULL== ids ){ 
        return -1;
    }
    
    return hal_uart_open(ids->handle->port);

}
//	
//	
/************************************************************************
 * @function: SYS_SER_Close
 * @����: ���ڹر�
 * 
 * @����: 
 * @param: port 
 * @����: 
 * @˵��: 
 * @����: yzy (2018/5/2)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
int32 SER_Close(uint8 ucChannelId)
{
//	    __SUart* uart;
//	    uint8 port = 0;
//	    uart = __Uart_Find(ucChannelId);
//	    if(!uart)
//	    {
//	        return API_ERR_PARA;
//	    }
//	    
//	    port = uart->port;
	
    
//	    Uartx_Disable(gss_UartSID[ucChannelId]);
	
    int32_t ret = -1;
    SerialID *ids = NULL; 

    ids = __Uart_Find(ucChannelId);
    if( NULL== ids ){ 
        return -1;
    }
    return hal_uart_close(ids->handle);
}


/************************************************************************
 * @function: SER_Clear
 * @����: ��մ��ڻ���
 * 
 * @����: 
 * @param: port 
 * @����: 
 * @˵��: 
 * @����: yzy (2013/8/28)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
int32 SER_Clear(uint8 port)
{
//	    __SUart* uartst;
//	    uint8 port = 0;
//	    uartst = __Uart_Find(ucChannelId);
//	    if(!uartst)
//	    {
//	        return API_ERR_PARA;
//	    }
//	    
//	    port = uartst->port;
    
//	    SerialBuffer* uart = gss_UartSID[port]->uart_Buffer;
//	#if SER_RECV_HOOK_EN > 0
//	    uint16 count;
//	    count = uart->rcnt;             //���ڻ�����ʵ�ʿɶ�������
//	    if(count > (uart->rlen - uart->pr))
//	    {
//	        _UartReadHook(port, &uart->rbuff[uart->pr], uart->rlen - uart->pr);
//	        _UartReadHook(port, &uart->rbuff[0], count - (uart->rlen - uart->pr));
//	    }
//	    else
//	    {
//	        _UartReadHook(port, &uart->rbuff[uart->pr], count);
//	    }
//	#endif
//	    
//	    CPSR_ALLOC();
//	    RHINO_CPU_INTRPT_DISABLE();
//	    
//	    uart->rcnt = 0;
//	    uart->rp = 0;
//	    uart->pr = 0;
//	    uart->tp = 0;
//	    uart->tcnt = 0;
//	    uart->tbuff = __NULL;
//	    RHINO_CPU_INTRPT_ENABLE();
    
    return API_SUCCESS;
}



/************************************************************************
 * @function: SER_Peek
 * @����: ���ڽ������ݿ�̽,ִ�к�������Ȼ�����ڽ��ջ�����
 * 
 * @����: 
 * @param: port �˿�
 * @param: buffer ��ſ�̽���ݵĻ���
 * @param: start �ӵڼ������ݿ�ʼ��̽(0-) 
 * @param: length ����̽���ݳ���
 * 
 * @����: 
 * @return: uint16  ��̽�õ���ʵ�����ݳ���
 * @˵��: SYS_SER_Read()�Ĳ��ֹ���
 * @����: yzy (2013/8/28)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	int16 SER_Peek(uint8 port, uint8* buffer, uint16 start, uint16 length)
//	{
//	    uint16 pr;
//	    uint16 count;
//	    uint16 i;
//	
//	//	    __SUart * uartHandle = __Uart_Find(ch);
//	//	    if(!uartHandle)
//	//	    {
//	//	        return API_ERR_PARA;
//	//	    }
//	
//	    SerialBuffer* uart = gss_UartSID[port]->uart_Buffer;
//	                                    //�����Ϸ����ж�
//	    if((port >= NO_OF_SERIAL) || (length == 0))
//	    {
//	        return 0;
//	    }
//	    
//	    if(start >= uart->rcnt)         //��ʼ�ֽ����Ϸ����ж�
//	    {
//	        return 0;
//	    }
//	
//	    CPSR_ALLOC();
//	    RHINO_CPU_INTRPT_DISABLE();
//	    if((start + length) < uart->rcnt)//"����ȡ����"��"���ڻ�����ʵ�ʿɶ�������"�ıȽ�
//	    {
//	        count = length;
//	    }
//	    else
//	    {
//	        count = uart->rcnt - start;
//	    }
//	    RHINO_CPU_INTRPT_ENABLE();
//	                                    //ѭ��ת������
//	    pr = uart->pr + start;          //���ݶ�ȡ��ʼָ��
//	    if(pr >= uart->rlen)
//	    {
//	        pr -= uart->rlen;
//	    }
//	    
//	    for(i = 0; i < count; i++)
//	    {
//	        buffer[i] = uart->rbuff[pr];
//	        pr++;
//	        if(pr >= uart->rlen)        //����
//	        {
//	            pr = 0;
//	        }
//	    }
//	
//	    return count;
//	}

//	/*==================================================================
//	//��������  SER_Read
//	//���ߣ�    ������
//	//���ڣ�    2018-1-24
//	//���ܣ�    ���ڽ�������
//	//���������1��ucChannelId������ͨ����         
//	//          2��nMaxLen���������յ�������ֽ���
//	//���������1��strData��Ҫ���յ�����
//	//����ֵ��  �ɹ�����ʵ�ʽ��յ����ֽ�����ʧ�ܷ���<0
//	//=================================================================*/
//	int32_t SER_Read(unsigned char ucChannelId, unsigned char *strData, int32_t nMaxLen)
//	{
//		__SUart* uart;
//		int nCnt;
//		if(nMaxLen < 0)
//		{
//			return API_ERR_PARA;
//		}
//		if(!nMaxLen)
//		{
//			return 0; 
//		}
//	
//		uart = __Uart_Find(ucChannelId);
//		if(!uart)
//		{
//			return API_ERR_PARA;
//		}
//	
//		nCnt = SYS_SER_Read(uart->port, strData, nMaxLen);
//	
//	
//		return nCnt;
//	}


/************************************************************************
 * @function: SER_ReadByte
 * @����: �Ӵ��ڶ�ȡһ���ֽ�����
 * 
 * @����: 
 * @param: port 
 * 
 * @����: 
 * @return: uint16  
 * @˵��: 
 * @����: yzy (2013/8/28)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	int16 SER_ReadByte(uint8 ch)
//	{
//	    uint8 result;
//	
//	    if(SER_RecvData(ch, &result, 1, 100) == 1)
//	    {
//	        return result;
//	    }
//	    return 256;
//	}





/************************************************************************
 * @function: SER_ToBeRead
 * @����: ���ڿɶ�ȡ���ݵĳ���
 * 
 * @����: 
 * @param: port 
 * 
 * @����: 
 * @return: uint16  
 * @˵��: 
 * @����: yzy (2013/8/28)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	int16 SER_ToBeRead(uint8 port)
//	{
//	//	    SerialBuffer* uart = gss_UartSID[port]->uart_Buffer;
//	    uint16 result;
//	//	    __SUart * uartHandle = __Uart_Find(ch);
//	//	    if(!uartHandle)
//	//	    {
//	//	        return API_ERR_PARA;
//	//	    }
//	
//	    SerialBuffer* uart = gss_UartSID[port]->uart_Buffer;
//	
//	    
//	    if(port >= NO_OF_SERIAL)
//	    {
//	        return 0;
//	    }
//	
//	    CPSR_ALLOC();
//	    RHINO_CPU_INTRPT_DISABLE();
//	    result = uart->rcnt;
//	    RHINO_CPU_INTRPT_ENABLE();
//	
//	    return result;
//	}
//	
//	
//	
//	
//	/************************************************************************
//	 * @function: SER_Line
//	 * @����: ���ڶ�ȡ������(��δ�ҵ��н�����,���ڻ��汻����)
//	 * 
//	 * @����: 
//	 * @param: port �˿ں�
//	 * @param: buffer ������ݻ���,__NULL��ʾ����
//	 * @param: length ������ݻ���ĳ���
//	 * 
//	 * @����: 
//	 * @return: uint16  ��ȡ���г���(����һ����β��).
//	 *          0��ʾ�����ݻ�δ�ҵ��н�����,1��ʾ��������β��
//	 * @˵��: 
//	 * @����: yzy (2013/8/28)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	int16 SER_Line(uint8 port, uint8* buffer, uint16 length)
//	{
//	//	    SerialBuffer* uart = gss_UartSID[port]->uart_Buffer;
//	    uint16 count;
//	    uint16 pr;
//	    uint16 i;
//	    uint8 sbyte;
//	
//	//	    __SUart * uartHandle = __Uart_Find(ch);
//	//	    if(!uartHandle)
//	//	    {
//	//	        return API_ERR_PARA;
//	//	    }
//	
//	    SerialBuffer* uart = gss_UartSID[port]->uart_Buffer;
//	
//	
//	    if(port >= NO_OF_SERIAL)
//	    {
//	        return 0;
//	    }
//	
//	    if(uart->rcnt == 0)             //δ����������ݳ���Ϊ0,����
//	    {
//	        return 0;
//	    }
//	
//	    CPSR_ALLOC();
//	    RHINO_CPU_INTRPT_DISABLE();
//	    count = uart->rcnt;             //δ�������ݳ���
//	    if(count > length)              //���������ݳ���
//	    {
//	        count = length;
//	    }
//	    RHINO_CPU_INTRPT_ENABLE();
//	                                    //ѭ���ж���β,ͬʱת������
//	    pr = uart->pr;                  //����ָ��
//	    for(i = 0; i < count; i++)      //ѭ����ȡ����
//	    {
//	        sbyte = uart->rbuff[pr];
//	        _IF_TRUE_DO(__NULL != buffer, buffer[i] = sbyte);
//	        pr++;
//	        if(pr >= uart->rlen)
//	        {
//	            pr = 0;
//	        }
//	        if((sbyte == '\r') || (sbyte == '\n'))
//	        {
//	            break;
//	        }
//	    }
//	
//	    if(i == count)                  //������һ������
//	    {
//	        return 0;
//	    }
//	    count = i + 1;
//	    
//	#if SER_RECV_HOOK_EN > 0
//	    if(__NULL == buffer)
//	    {
//	        if(count > (uart->rlen - uart->pr))
//	        {
//	            _UartReadHook(uartHandle->port, &uart->rbuff[uart->pr], uart->rlen - uart->pr);
//	            _UartReadHook(uartHandle->port, &uart->rbuff[0], count - (uart->rlen - uart->pr));
//	        }
//	        else
//	        {
//	            _UartReadHook(uartHandle->port, &uart->rbuff[uart->pr], count);
//	        }
//	    }
//	#endif
//	    
//	    uart->pr += count;              //����ָ��ǰ��
//	    while(uart->pr >= uart->rlen)
//	    {
//	        uart->pr -= uart->rlen;
//	    }
//	    
//	    RHINO_CPU_INTRPT_DISABLE();
//	    uart->rcnt -= count;            //δ�������ݳ��ȵݼ�
//	    RHINO_CPU_INTRPT_ENABLE();
//	
//	    
//	#if SER_RECV_HOOK_EN > 0
//	     _IF_TRUE_DO(__NULL != buffer, _UartReadHook(uartHandle->port, buffer, count));
//	#endif
//	
//	    return count;
//	
//	}







/************************************************************************
 * @function: SER_LineLen
 * @����: ��ȡ���ڵ�ǰ�г���
 * 
 * @����: 
 * @param: port �˿ں�
 * 
 * @����: 
 * @return: uint16  �г���(����һ����β��)
 *          0��ʾ�����ݻ�δ�ҵ��н�����,1��ʾ��������β��
 * @˵��: 
 * @����: yzy (2013/8/28)
 *-----------------------------------------------------------------------
 * @�޸���: 
//	 ************************************************************************/
//	int16 SER_LineLen(uint8 port)
//	{
//	//	    SerialBuffer* uart = gss_UartSID[port]->uart_Buffer;
//	    uint16 count;
//	    uint16 pr;
//	    uint16 i;
//	
//	//	    __SUart * uartHandle = __Uart_Find(ch);
//	//	    if(!uartHandle)
//	//	    {
//	//	        return API_ERR_PARA;
//	//	    }
//	
//	    SerialBuffer* uart = gss_UartSID[port]->uart_Buffer;
//	
//	
//	    if(port >= NO_OF_SERIAL)
//	    {
//	        return 0;
//	    }
//	
//	    if(uart->rcnt == 0)             //û��Ϊ��ȡ���ݷ���0
//	    {
//	        return 0;
//	    }
//	
//	    CPSR_ALLOC();
//	    RHINO_CPU_INTRPT_DISABLE();
//	            
//	    count = uart->rcnt;             //δ�������ݳ���
//	    pr = uart->pr;                  //����ָ��
//	    RHINO_CPU_INTRPT_ENABLE();
//	    
//	    for(i = 0; i < count; i++)        //ѭ���ж�
//	    {
//	        if((uart->rbuff[pr] == '\r') || (uart->rbuff[pr] == '\n'))
//	        {
//	            break;
//	        }
//	        pr++;
//	        if(pr >= uart->rlen)
//	        {
//	            pr = 0;
//	        }
//	    }
//	            
//	    if(i == count)                    //������������
//	    {
//	        return 0;
//	    }
//	
//	    count = i + 1;
//	
//	    return count;
//	
//	}
//	



/************************************************************************
 * @function: SER_PrintValueOption
 * @����: ��������,�����ַ����ĸ�ʽ�����з���
 * 
 * @����: 
 * @param: port ���ں�
 * @param: str ���͵������ַ���
 * @param: value ���͵�����
 * @param: opt ѡ��:b0=1������������,b0=0ϵͳ��������
 * 
 * @����: 
 * @return: uint8  �ɹ�/ʧ��
 * @˵��: 
 * @����: yzy (2013/6/10)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	uint8 SER_PrintValueOption(uint8 ch, string str, int value, int32_t nTimeout)
//	{
//	    uint32 i;
//	    uint32 len = strlen(str);
//	    uint16 templen;
//	    uint8 data[12];
//	
//	    if(len == 0)                        //����ַ�������Ϊ0��ֱ�ӷ���
//	    {
//	        SYS_OK();                        //���ش���ɹ�
//	    }
//	
//	    for(i = 0; i < len; i++)            //ɨ��ֱ������'%'
//	    {                                        
//	        if(str[i] == '%')               //�ж��Ƿ�Ϊ'%'        
//	        {                               
//	            if(str[i + 1] == 'x' 
//	               || str[i + 1] == 'd' 
//	               || str[i + 1] == 's'
//	               )
//	            {                           //��ӡǰ����ַ���
//	                //SYS_SER_WriteOption(port, (uint8*)str, i, opt);
//	                SER_SendData(ch, (uint8*)str, i, nTimeout);
//	                break;                  //����
//	            }
//	        }
//	    }
//	    if(i == len)                        //����Ѿ�ɨ����������һ��Ϊ%��ֱ���˳�
//	    {                                   //�����Ϣ
//	//	       SYS_SER_WriteOption(port, (uint8*)str, len, opt);
//	       SER_SendData(ch, (uint8*)str, len, nTimeout);
//	       SYS_OK();                         //���ش���ɹ�
//	    }
//	
//	    switch(str[i + 1])                  //ѡ������
//	    {
//	        case 'x':                       //HEX���  
//	                                        //�ж��Ƿ�Ϊ'0' - '9'      
//	            if(!IsBcdChar(str[i + 2]))
//	            {
//	                break;                  //���ǵĻ��˳�
//	            }
//	            NumToHexString(//���ֵ�Hex�ַ�����ת��
//	                data, 
//	                (uint8*)&value, 
//	                (str[i + 2] - '0') >> 1
//	                );
//	
//	            SER_SendData(               //��ӡת������������
//	                ch, 
//	                data, 
//	                str[i + 2] - '0',
//	                nTimeout
//	                );
//	            i++;                        //�������Ĵ���
//	            break;
//	        case 'd':                       //�����ַ����    
//	                                        //�ж��Ƿ�Ϊ��ʽ�����
//	            if(IsBcdChar(str[i + 2]))  
//	            {
//	                templen = str[i + 2] - '0';
//	                i++;
//	            }
//	            else
//	            {
//	                templen = 0;
//	            }
//	                                        //���ֵ�BCD�ַ�����ת��
//	            templen = NumToBcdString(   
//	                data, 
//	                value, 
//	                templen
//	                );
//	
//	            SER_SendData(         //�������
//	                ch, 
//	                data, 
//	                templen,
//	                nTimeout
//	                );
//	            break;
//	        case 's':                       //ֱ������ַ���        
//	            SER_SendData(         //����ַ���
//	                ch, 
//	                (uint8 *)value, 
//	                sizeof((uint8 *)value),
//	                nTimeout
//	                );
//	            break;
//	    }
//	    i += 2;
//	    
//	    if(i >= len)                        //����Ѿ�ɨ����������һ��Ϊ%��ֱ���˳�
//	    {
//	       SYS_OK();                         //���ش���ɹ�
//	    }
//	                                        //������������
//	    SER_SendData(ch, (uint8*)str + i, strlen(str + i), nTimeout);
//	
//	    SYS_OK();                            //���ؽ��
//	}
//	
//	
//	




/************************************************************************
 * @function: SER_PrintValue
 * @����: ��������,�����ַ����ĸ�ʽ�����з���
 * 
 * @����: 
 * @param: ch ���ں�
 * @param: buffer ����  
 * @param: buflen ���泤��     
 * @param: timeout ������ʱ
 * @param: str ���͵������ַ���
 * @param: ... ���͵�����
 * 
 * @����: 
 * @return: uint8  �ɹ�/ʧ��
 * @˵��: 
 * @����: yzy (2013/6/10)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	int32 SER_PrintValue(uint8 ch, uint8 * buff, uint16 buflen, int32 nTimeout, const int8 *strFormat, ...)
//	{
//		int32 ret = 0;
//		va_list  vlist;
//	
//		if (strFormat == NULL) 
//	    {
//	        return SYS_ERR_VAR;
//	    }
//		
//	    memset(buff, 0, buflen);
//		
//		va_start(vlist, strFormat);
//		vsnprintf((char *)buff, 
//	               buflen, 
//	               (char const *)strFormat, vlist);
//		va_end(vlist);
//	    
//	   
//	    SER_SendData(ch, (uint8 *)buff, strlen((char const *)buff), nTimeout);
//	
//		return ret;
//	}





