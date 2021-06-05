/****************************************Copyright (c)**************************************************
**                                  ____�Ƽ����޹�˾
**                                          ������
**
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: pst.c
**��   ��   ��: yzy
**����޸�����: 2013��7��3��
**��        ��: ����ͨѶӦ�ù������
**ע        ��:
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: yzy
** ��  ��: v1.0
** �ա���: 2018��3��22��
** �衡��: ԭʼ�汾
**-------------------------------------------------------------------------------------------------------
** �޸���: 
** ��  ��: 
** ��  ��: 
********************************************************************************************************/
#define EXT_PST

#include <k_api.h>
#include <stdio.h>
#include <stdlib.h>
#include "public.h"





/************************************************************************
 * @function: PstFrmProc
 * @����: ����ͨѶ������(��FRM�豸���з�װ,�����̿�ֱ�ӵ��øú���)
 * 
 * @����: 
 * @param: channel  ͨ����;��ͬ��ͨ����Ҫ����ͨ�Ĺ�Լ
 * @param: rbuffer  ���ջ���
 * @param: length  �����������ݳ���
 * @param: sbuffer 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2018��3��22��)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 PstFrmProc(PORT_UART_TYPE channel, uint8* rbuffer, uint16 length, uint8* sbuffer, uint16 sbuflen)
{
    PST_Frame frm;                          //��Լ֡�ṹ��
    uint8 protocal;                         //��Լ��
    uint16 bak;
    uint8 err;
    
    SYS_VAR_CHECK(channel >= PST_CHN_NO);    //�������
                                            //ͨ����Լ֡�ж�
    if(PST_Channel(channel, rbuffer, length, &protocal) != PST_ERR_OK)
    {
        return PST_ERR_NE;
    }

    frm.chninfo = channel & PST_FRM_CHNNO;  //ͨ����
    
                                            //��Լ֡����
    if(FRM_Decode(protocal, rbuffer, sbuffer, sbuflen, &frm) == PST_ERR_OK)
    {
        FRM_Proc(protocal, &frm);        //��Լ����
    }
    
    if(channel == PST_CHN_TCP)
    {
#if SYS_MODEM_EN > 0
        extern kbuf_queue_t gs_TKFarpQueue;
        uint8 msg = MSG_FARP_RECVVALIDIP;
        krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
#endif        
    }
//	    else if(channel == PST_CHN_SER)
//	    {
//	        uint8 msg = MSG_FARP_RECVVALIDSER;
//	        krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
//	        
//	    }

    SYS_OK();
}






/************************************************************************
 * @function: PST_CHN0_Send
 * @����: ͨ������
 * 
 * @����: 
 * @param: buffer �������ݵĻ���
 * @param: len �������ݵĳ���
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2018��3��22��)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 PST_CHN0_Send(uint8* buffer, uint16 len)
{                                       //����
    gs_GPIO.BlinkSet(GPIO_LED_485, 0, 0, 0);
                                        //
    SYS_SER_Open(UART_CHANNEL_DEBUG, TDB_MODE_W);
    SYS_SER_Write(UART_CHANNEL_DEBUG, buffer, len, 1000);
    //SER_Close(UART_CHANNEL_DEBUG);
    aos_msleep(500);
    LOG_DUMPHEX(LOG_LEVEL_DEBUG, "coms:\n", buffer, len);
    SYS_OK();
}




/************************************************************************
 * @function: PST_CHN1_Send
 * @����: ͨ������
 * 
 * @����: 
 * @param: buffer �������ݵĻ���
 * @param: len �������ݵĳ���
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2018��3��22��)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 PST_CHN1_Send(uint8* buffer, uint16 length)
{
#if SYS_MODEM_EN > 0
        return Farp_SendIpData(buffer, length);
#else
        return 0;
#endif    
}


/************************************************************************
 * @function: PST_CHN2_Send
 * @����: ͨ������
 * 
 * @����: 
 * @param: buffer �������ݵĻ���
 * @param: len �������ݵĳ���
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2018��3��22��)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 PST_CHN2_Send(uint8* buffer, uint16 length)
{
    gs_GPIO.BlinkSet(GPIO_LED_485, 0, 0, 0);
                                        //
    SYS_SER_Open(NETP_PORT_NO, TDB_MODE_W);
    SYS_SER_Write(NETP_PORT_NO, buffer, length, 1000);
    SYS_SER_Close(NETP_PORT_NO);
    SYS_OK();

}

uint8 PST_CHN3_Send(uint8* buffer, uint16 length)
{
    gs_GPIO.BlinkSet(GPIO_LED_485, 0, 0, 0);
                                        //
    SYS_SER_Open(NETP2_PORT_NO, TDB_MODE_W);
    SYS_SER_Write(NETP2_PORT_NO, buffer, length, 1000);
    SYS_SER_Close(NETP2_PORT_NO);
    SYS_OK();

}
uint8 PST_CHN4_Send(uint8* buffer, uint16 length)
{
    gs_GPIO.BlinkSet(GPIO_LED_485, 0, 0, 0);
                                        //
    SYS_SER_Open(NETP3_PORT_NO, TDB_MODE_W);
    SYS_SER_Write(NETP3_PORT_NO, buffer, length, 1000);
    SYS_SER_Close(NETP3_PORT_NO);
    SYS_OK();

}
uint8 PST_CHN5_Send(uint8* buffer, uint16 length)
{
    gs_GPIO.BlinkSet(GPIO_LED_485, 0, 0, 0);
                                        //
    SYS_SER_Open(NETP4_PORT_NO, TDB_MODE_W);
    SYS_SER_Write(NETP4_PORT_NO, buffer, length, 1000);
    SYS_SER_Close(NETP4_PORT_NO);
    SYS_OK();

}

//	/************************************************************************
//	 * @function: PST_CHN3_Send
//	 * @����: ͨ������-����
//	 * 
//	 * @����: 
//	 * @param: buffer �������ݵĻ���
//	 * @param: len �������ݵĳ���
//	 * 
//	 * @����: 
//	 * @return: uint8  
//	 * @˵��: 
//	 * @����: yzy (2018��3��22��)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	uint8 PST_CHN3_Send(uint8* buffer, uint16 length)
//	{
//	    uint8 err;                          //���ؽ��
//	    uint16 tlen;
//	                                        //�򿪴���
//	    gs_Uart.open(NETP_PORT_NO, false);
//	                                        //д�����ݵ�����
//	    while(length > 0)
//	    {
//	        tlen = length > 256 ? 256 : length;
//	        err = gs_Uart.Write(NETP_PORT_NO, buffer, tlen);
//	        buffer += tlen;
//	        length -= tlen;
//	    }
//	    gs_Uart.close(NETP_PORT_NO);             //�رմ���
//	                                
//	    return err;                             //��������
//	}
//	
//	
//	
//	/************************************************************************
//	 * @function: PST_CHN4_Send
//	 * @����: ͨ������
//	 * 
//	 * @����: 
//	 * @param: buffer �������ݵĻ���
//	 * @param: len �������ݵĳ���
//	 * 
//	 * @����: 
//	 * @return: uint8  
//	 * @˵��: 
//	 * @����: yzy (2018��3��22��)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	uint8 PST_CHN4_Send(uint8* buffer, uint16 length)
//	{
//	    return Farp_SendTcpServerData(buffer, length);
//	}











/************************************************************************
 * @function: PST_Init
 * @����: �������̳�ʼ��
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2018��3��22��)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void PST_Init(void)
{

    SYS_SER_Init(UART_CHANNEL_DEBUG, NULL);    //��ʼ������   
    
//	    void InitFileDownLoad(void);
//	    InitFileDownLoad();
    
    gs_PstPara.repwait = 80;
    gs_PstPara.reptry = 3;
    
    LoadSystemParam(PARA_TYPE_PST);
}


//	
//	/************************************************************************
//	 * @function: PST_PreInit
//	 * @����: �������̳�ʼ��
//	 * @����: 
//	 * @����: 
//	 * @˵��: 
//	 * @����: yzy (2018��3��22��)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	void PST_PreInit(void)
//	{
//	                                        //�����򵥱�ʶ����
//	    gs_Array.FIFO_Create(
//	        size_array_4,
//	        &gs_FifoAfnReady,
//	        gss_AfnReadyTeam,
//	        sizeof(gss_AfnReadyTeam)
//	    );
//	                                        //��ʼ���¼�����������
//	    guc_e1bk = gs_FramMisData.e1cnt;
//	    guc_e2bk = gs_FramMisData.e2cnt;
//	    
//	    void InitFileDownLoad(void);
//	    InitFileDownLoad();
//	    
//	    LoadSystemParam(PARA_TYPE_PST);
//	    
//	}



uint8 g_pdata[512];
/************************************************************************
 * @function: Setp_RecvProc
 * @����: ���ö˿ڽ��յ�����֮������ݴ���
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2018��3��22��)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void Setp_RecvProc(void)
{
    uint16 length;
    
    //SER_Open(UART_CHANNEL_DEBUG, TDB_MODE_R | TDB_MODE_W);
    length = SYS_SER_Read(UART_CHANNEL_DEBUG, g_pdata, 256, 0);
    //SER_Close(UART_CHANNEL_DEBUG);
//	    while(gs_Uart.ToBeRead(SETP_PORT_NO) > 384)
//	    {
//	        length = gs_Uart.Read(SETP_PORT_NO, pdata, 384);
//	    }
//	    length = gs_Uart.Read(SETP_PORT_NO, pdata, 384);
//	    gs_Uart.close(SETP_PORT_NO);

#ifdef DEBUG_ATMODEM_TRANS
    //GPRS_PORT<->PST_PORT
    gs_Uart.open(SYS_GPRS_PORT, false);
    gs_Uart.Write(SYS_GPRS_PORT, pdata, length);
    gs_Uart.close(SYS_GPRS_PORT);
    return;
#else
    
    aos_msleep(20);
                                        //������ͨ��0������
    if(length > 1)
    {
        LOG_DUMPHEX(LOG_LEVEL_DEBUG, "recv:\n", g_pdata, length);
        if(SYS_ERR_OK == PstFrmProc(PST_CHN_SET, g_pdata, length, g_pdata, sizeof(g_pdata)));
    }
#endif
}




#ifdef DEBUG_ATMODEM_TRANS
/************************************************************************
 * @function: Mdmp_RecvProc
 * @����: GPRS_PORT<->PST_PORT
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2018��3��22��)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void Mdmp_RecvProc(void)
{
    uint8 pdata[384];
    uint16 length;
    
    gs_Uart.open(SYS_GPRS_PORT, false);
    while(gs_Uart.ToBeRead(SYS_GPRS_PORT) > 384)
    {
        length = gs_Uart.Read(SYS_GPRS_PORT, pdata, 384);
    }
    length = gs_Uart.Read(SYS_GPRS_PORT, pdata, 384);
    gs_Uart.close(SYS_GPRS_PORT);

    gs_Uart.open(SETP_PORT_NO, false);
    gs_Uart.Write(SETP_PORT_NO, pdata, length);
    gs_Uart.close(SETP_PORT_NO);
    return;
}
#endif


/************************************************************************
**��������: void PST_UpdateVar(void* pdata)
**�� ��: ��·�������ʼ��
**
**�� ��: 
**�� ��: 
**
**�� ע: δ��������Ч�Խ��м��
**-----------------------------------------------------------------------
**�� ��: yzy
**�� ��: 08-09-09
*************************************************************************/
void PST_UpdateVar(void)
{
    LoadSystemParam(PARA_TYPE_PST);
}







//	/************************************************************************
//	 * @function: SYS_PST_Task
//	 * @����: ����ͨѶӦ�ù�����̴�����
//	 * @����: 
//	 * @����: 
//	 * @˵��: 
//	 * @����: yzy (2018��3��22��)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	void SYS_PST_Task(void)
//	{
//	                                            //������Ϣ
//	    TESMsg* msg = (TESMsg*)gs_Mem.GetMEM(sizeof(TESMsg));
//	    uint8 nmsg;                             //������Ϣ
//	    
//	    PST_Init();
//	                                            //���봮����Ϣ
//	    SYS_MSG_ApplyExt(MSG_CLS_UART, SETP_PORT_NO);
//	#ifdef DEBUG_ATMODEM_TRANS
//	    SYS_MSG_ApplyExt(MSG_CLS_UART, SYS_GPRS_PORT);
//	#endif
//	    
//	    void GdUp_HXWDTTst(void);               //���Ź�����
//	    GdUp_HXWDTTst();
//	    //���Ź����������ϱ�
//	    void WdtTestExeStartPush(void);
//	    WdtTestExeStartPush();
//	    
//	    for(;;)
//	    {
//	        nmsg = gs_OS.Message_Wait(&msg); 
//	        switch(nmsg)
//	        {
//	            case MSG_PSTP:                  //���ÿڱ���ͨѶ����
//	                Setp_RecvProc();
//	                break;
//	                
//	            case MSG_PST_VAR:
//	                PST_UpdateVar();
//	                break;
//	#ifdef DEBUG_ATMODEM_TRANS
//	            case MSG_MDMP:
//	                Mdmp_RecvProc();
//	                break;
//	#endif
//	                
//	            case MSG_LIVE:
//					HB_RetLive();
//	                break;
//	        }
//	    }
//	}
//	
//	/************************************************************************
//	**����ͨѶ���̿鳣�����ֶ���
//	************************************************************************/
//	const TESTcbConst gs_TkPst = 
//	{
//	    SYS_PST_Task,
//	    TASK_PST_TKID >> 3,                    //��̫��Ӧ�ù������
//	    TASK_PST_TKID,                         //���̱��
//	    TASK_PST_TKID & 7,                     //���ȼ��ڽ��̱��Ϊ
//	    true,                                  //��Ϣѭ�����ƽ���
//	};

