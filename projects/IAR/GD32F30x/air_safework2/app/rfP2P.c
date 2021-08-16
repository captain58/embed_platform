#define EXT_FARP
#include <k_api.h>
#include <stdio.h>
#include <stdlib.h>
#include "public.h"
#include "app.h"
#include "pst.h"
//#include "hlv.h"
//#include "farp.h"
//#include "netp.h"
#include "task.h"
#include "farp.h"

#include "A7139reg.h"
uint16 gui_FarpWmSafeCnt;                   //����ͨѶ״̬����������ȫʱ�䵹��ʱ(��)
uint16 gui_FarpSerSafeCnt;                   //����ͨѶ״̬����������ȫʱ�䵹��ʱ(��)



/******************************************************************************
**�����������ʱ
******************************************************************************/
uint16 gui_FarpWmRcSec;                     //GPRSͨ·�������������(�뵹��ʱ)
uint16 gui_FarpSerRcSec;                     //GPRS Tcp Serverͨ·�������������(�뵹��ʱ)


/******************************************************************************
**��������ʧ�ܴ���
******************************************************************************/
uint8 guc_FarpWmRcFailCnt;                  //GPRS��������ʧ�ܴ���
uint8 guc_FarpSerRcFailCnt;                 //Tcp Server��������ʧ�ܴ���
uint16 guc_FarpWmRcFailNorFtpCnt;                  //GPRS������������FTP����


/******************************************************************************
**��������
******************************************************************************/
uint8 guc_FarpWmRcCnt;                      //����ʧ�ܺ����������(yzy:���ڷ�������ǰ��ֵ)
uint8 guc_FarpSerRcCnt;                     //����ʧ�ܺ����������(yzy:���ڷ�������ǰ��ֵ)



/******************************************************************************
**�������ڼ�����
******************************************************************************/
uint16 gui_FarpWmHBCYCnt;                   //�������ڼ�����


uint8 guc_FarpWmHKUnRpyCnt;                 //�����޻ظ�����������




/******************************************************************************
**����������
******************************************************************************/
uint16 gui_FarpHdRstCnt;                    //����è��ʼ������ʱ�������(��δ�忨ʱ����ֳ�ʼ��ʧ��)
uint8 guc_FarpPstTryCnt;                    //GPRSͨ·���������ز�����������
uint8 guc_FarpNVHours;                      //������ͨѶСʱ��
uint8 guc_FastWmSttReqCnt;                  //MD ����������ʱ

uint16 gui_FarpServerNoDataCnt;             //TCP Server  ������������£������ݼ���
uint16 gui_FarpFromClnNoDataCnt;             //TCP Server  ��ͻ���1���ӵ�����£������ݼ���
uint8  guc_FarpFromClnCloseCnt;             //TCP Server  ��ͻ���1���ӵ������, �����رտͻ��˴���

uint8  guc_FarpReActiveFlag;                //��������ģʽ���ɼ�����������Ҫ������վ��������



uint16 gui_smseq;
static char         g_RFMng_buf_recv[BUFQUEUE_MSG_MAX+4];

size_t       g_RFMng_recv_size;
ktimer_t     g_RFMngTimer;

cpu_stack_t  gs_RFMngStack[TASK_RFMNG_STKL];
ktask_t      gs_RFMngHandle;
kbuf_queue_t gs_RFMngQueue;
char         gc_RFMngbuf[MSG_BUFF_LEN];

void Farp_SendTestData(uint8 ch);
/************************************************************************
 * @function: Farp_PreInit
 * @����: Զ�̲�����ʼ��
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void Farp_PreInit(void)
{                                           //��������
    memset((uint8*)&gs_FarpVar, 0, sizeof(FarpVar));
    memset((uint8_t *)&gs_FtpPara,0,sizeof(S_FTPPARA));
    
//    LoadSystemParam(PARA_TYPE_FARP);

//    gui_FarpWmRcSec = 10;                   //��ʼ�����10s�����Զ������
//    gui_FarpSerRcSec = 10;
//    //gui_FarpEthRcSec = 10;
//    
//    guc_FarpWmHKUnRpyCnt = 0;               //�����޻ظ���������������
//    //guc_FarpEthHKUnRpyCnt = 0;
//    
//    gui_FarpWmSafeCnt = 0;
//    gui_FarpSerSafeCnt = 0;
//    //gui_FarpEthSafeCnt = 0;
//    
//    guc_FarpWmRcCnt = 0;                   //����ʧ�ܺ�������������
//    guc_FarpSerRcCnt = 0;
//    //guc_FarpEthRcCnt = 0;
//    
//    gui_FarpHdRstCnt = 0;                   //ģ�鸴λ����������
//    guc_FarpNVHours = 0;                    //��IP����ʱ������
//    guc_FarpPstTryCnt = 0;                   //��������ģʽ�ز���������������
//    
//    gui_FarpBkTmCnt = 0;                    //�����������������߼���������
//    guc_FarpWmRcFailCnt = 0;
//    guc_FarpWmRcFailNorFtpCnt = 0;
//    //guc_FarpEthRcFailCnt = 0;
//    
//    gui_FarpServerNoDataCnt = 0;
//    guc_FarpFromClnCloseCnt = 0;
//    
//    guc_FarpLoginConfirmErr = 0;
//    gul_FarpLoginDeadTime = 0xFFFFFFFF;
//    
//    gs_FarpVar.wmsv = 0x88000000;
    //gs_SysVar.mLPstt |= HLV_LPTASK_MDCK;

}
/************************************************************************
 * @function: Farp_SendIpData
 * @����: Զ��ͨ������GPRS��IP����
 * 
 * @����: 
 * @param: buffer ���ͻ���
 * @param: len ���͵ĳ���
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 Farp_SendIpData(uint8* buffer, uint16 len)
{
                                            //IP������ͨ�����õ�����·���
//    if(g_ulTcp_fd >=0 )
//    {                                       //����IP����
////	        if(!SYS_MODM_SendIP(buffer, len))
//        LOG_DUMPHEX(LOG_LEVEL_DEBUG, "send:\n", buffer, len);
//
//        if(HAL_TCP_Write(g_ulTcp_fd, buffer, len,3000) >= 0)
//        {
////	            gs_GPIO.LED_BlinkSet(LED_FAR_T, 0, 0, 0);
//            SYS_OK();                        //���ͳɹ�
//        }
//        else
//        {
//            uint8 msg = MSG_FARP_DISGPRS;
//            krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
//            
//            gui_FarpWmRcSec = 10;
//        }
//    }
    return SYS_ERR_FT;                      //IP���ݷ���ʧ��
}
/************************************************************************
 * @function: Farp_SendTestData
 * @����: Զ��ͨ����������֡
 * 
 * @����: 
 * @param: ch 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void Farp_SendTestData(uint8 ch)
{
    if(gs_FarpVar.login)
    {
        gs_FarpVar.hklen = RF_BuildLinkFrm(0x09, 0, gs_FarpVar.hkd);
    }
    else
    {
        gs_FarpVar.hklen = RF_BuildLinkFrm(0x05, 0xA619, gs_FarpVar.hkd);
    }
    if(gs_FarpVar.hklen > 0)
    {
        if(ch == 0)
        {
//	            Farp_SendIpData((uint8 *)cmdUpBuf, sizeof(cmdUpBuf));
            Farp_SendIpData((uint8 *)gs_FarpVar.hkd, gs_FarpVar.hklen);

        }
        else
        {
            //Farp_SendEthData(gs_FarpVar.hkd, gs_FarpVar.hklen);
        }
    }
}

///************************************************************************
// * @function: Farp_WMSecondProc
// * @����: ����è���봦��
// * @����: 
// * @����: 
// * @˵��: 
// * @����: yzy (2014/1/14)
// *-----------------------------------------------------------------------
// * @�޸���: 
// ************************************************************************/
void Farp_WMSecondProc(void)
{
    uint8 uc_closeclnflag = 0;
//    uint8 uc_closeserflag = 0;
    /*
    if(gs_FarpVar.wmst & WMS_ETH0STT)
    {
        return;
    }
    */
                                            //yzy 2014-04-09,�������Ҫ����̫������,ֻҪ��⵽���߼�����GPRS.
//	    if(gs_FarpVar.wmst & WMS_EHSTT)
//	    {
//	        if((gs_FarpVar.wmst & WMS_STT_CLN) || (gs_FarpVar.wmst & WMS_STT_SER))
//	        {
//	//	            SYS_Message_Send(MSG_FARP_DISPPP, TASK_FARP_TKID);
//	            uint8 msg = MSG_FARP_DISPPP;
//	            krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
//	            
//	        }
//	        gs_FarpVar.wmst &= ~WMS_CHALLSTT;
//	        return;
//	    }                                       //yzy 2014-04-09
                                            //��ȡģ��״̬
    TRFModemState modemstt = SYS_RF_Status();//SYS_MODM_Status();
	
    if(gui_FarpWmSafeCnt > 0)               //����ͨѶ״̬����������ȫʱ�䵹��ʱ
    {
        gui_FarpWmSafeCnt--;
    }
    
    if(gui_FarpSerSafeCnt > 0)              //TCP Server ������ͨѶ״̬����������ȫʱ�䵹��ʱ
    {
        gui_FarpSerSafeCnt--;
    }
    
//    if(!(modemstt.bit.typeChecked))         //ģ�黹δʶ��
//    {
//        return;
//    }
    
    if(!(modemstt.bit.chnrdy))              //�Ƿ����AT����
    {
        if(++guc_FastWmSttReqCnt >= 5)      //ÿ5����
        {
            guc_FastWmSttReqCnt = 0;
            //SYS_MODM_Step(Modem_Init);
//            SYS_MODM_Reinit();
//	            HAL_WIFI_Init(TASK_FARP_TKID,"tw14",gs_FarpVar.ap , &g_ulTcp_fd, 4);
//            gs_GPIO.LED_BlinkSet(GPIO_LED_GPRS, 0xFF, 0, 0);
        }
        return;
    }
	
    if(!(modemstt.bit.regtt))               //�Ƿ��������ע��
    {
        gs_FarpVar.wmst &= ~WMS_CHALLSTT;
        gs_FarpVar.mcsq = 0;
                                            //��ʱ����ģ��״̬
        if(++guc_FastWmSttReqCnt >= 15)     //ÿ15����
        {
            guc_FastWmSttReqCnt = 0;
//	            HAL_WIFI_BeginCheck();
        }
        return;
    }
	
    gs_FarpVar.wmst |= WMS_STT_GSM;
    gs_FarpVar.mcsq = modemstt.bit.signl;
//    SYS_LED_BlinkSet1(GPIO_LED_GPRS, 1, 10, 10, 0);  //ע��GSM����ɹ���1HZ��˸
    
    if(!(modemstt.bit.pppoe))       //�Ƿ����PPP����
    {
        gs_FarpVar.wmst &= ~WMS_STT_PPP;    //����ͨ��������
        if(++guc_FastWmSttReqCnt >= 30)     //ÿ10����
        {
//	            SYS_Message_Send(MSG_FARP_CNTPPP, TASK_FARP_TKID);
            uint8 msg = MSG_FARP_CNTPPP;
            krhino_buf_queue_send(&gs_RFMngQueue, &msg, 1);
            
            guc_FastWmSttReqCnt = 0;
        }
//	        
//        if(guc_FastWmSttReqCnt % 5 == 0)     //ÿ3����
//        {
//            HAL_WIFI_BeginCheck();
//        }
        return;
    }
    gs_FarpVar.wmst |= WMS_STT_PPP;
    
    
    


//	    if(gs_FarpVar.clnen == 1)           //�ͻ���ģʽ
    {
        if(gs_FarpVar.login == 0)    //TCP ������
        {
            gs_FarpVar.wmst &= ~WMS_STT_CLN;    //����ͨ��������
            if(gs_FarpVar.wkmd == 0x00)         //ʵʱ����ģʽ
            {
                if(gui_FarpWmRcSec == 0)        //��������ģʽ���ߺ󼴿�ʼ���Ե���ʱ
                {
                    gui_FarpWmRcSec = gs_FarpVar.rttm;
                }
                                                //����ͨѶ״̬�µ�����������
                if(gui_FarpWmSafeCnt > 0)
                {
                    gui_FarpWmSafeCnt = 0;
                    
                    gui_FarpWmRcSec = 1;
                    guc_FarpWmRcCnt = 3;       //ͨѶ״̬�¶�������3��
                }
                if(gui_FarpWmRcSec > 0)
                {
                    gui_FarpWmRcSec--;         //�������ݼ�
                    if(gui_FarpWmRcSec == 0)   //���Լ���ѵ�,��������
                    {
//	                        SYS_Message_Send(MSG_FARP_CNTGPRS, TASK_FARP_TKID);
                        uint8 msg = MSG_FARP_CNTGPRS;
                        krhino_buf_queue_send(&gs_RFMngQueue, &msg, 1);
                        
                    }
//	                    if(gui_FarpWmRcSec % 15 == 0)
//	                    {
//	                        int fd = -1;
//	                        fd = HAL_WIFI_TCP_Lnk_Chk();
//	                        if(fd < 0)
//	                        {
//	                            SYS_Message_Send(MSG_FARP_CNTGPRS, TASK_FARP_TKID);
//	                        }
//	                        else
//	                        {
//	                            if(g_ulTcp_fd < 0)
//	                            {
//	                                g_ulTcp_fd = fd;
//	                                guc_FarpLoginConfirmErr = 0;
//	                                gul_FarpLoginDeadTime = gul_SysRunSecs + 10;
//	                                                                //
//	//                                gs_FarpVar.wmst |= WMS_STT_CLN;
//	                                guc_FarpPstTryCnt = 0;          //����ģʽ������������
//	                                Farp_SendLoginData(0);          //���͵�¼֡
//	                                //Farp_SendHKData(0);             //��������֡
//	                                                                //��ʼ����������
//	                                gui_FarpWmHBCYCnt = 5;//gs_FarpVar.hbcy;
//	                                                                //ʵʱ����ģʽ����Ч���ݼ�������ʼ��
//	                                gui_FarpWmSafeCnt = gs_FarpVar.hbcy;
//	                                guc_FarpWmRcFailCnt = 0;        //����ʧ�ܴ�������
//	                            }
//	                        }
//	                    }
                }
            }
            else if(gs_FarpVar.wkmd == 0x01)    //��������ģʽ
            {
                //
            }
        }
        else
        {
            gs_FarpVar.wmst |= WMS_STT_CLN;     //����ͨ������
            
            if(guc_FarpLoginConfirmErr)           //��¼��n����δ�յ�ȷ��֡,ִ����������
            {
                if(gul_SysRunSecs > gul_FarpLoginDeadTime)
                {
//	                    SYS_Message_Send(MSG_FARP_DISGPRS, TASK_FARP_TKID);
                    uint8 msg = MSG_FARP_DISGPRS;
                    krhino_buf_queue_send(&gs_RFMngQueue, &msg, 1);
                    
                    uc_closeclnflag = 1;
                    gul_FarpLoginDeadTime = 0xFFFFFFFF;
                }
            }
            
            if(gui_FarpWmHBCYCnt > 0)
            {
                gui_FarpWmHBCYCnt--;
                if(gui_FarpWmHBCYCnt == 0)      //�������ڵ�������֡
                {
                    if(guc_FarpWmHKUnRpyCnt >= 3) //�����޻ظ���������3��
                    {
//	                        SYS_Message_Send(MSG_FARP_DISGPRS, TASK_FARP_TKID);
                        uint8 msg = MSG_FARP_DISGPRS;
                        krhino_buf_queue_send(&gs_RFMngQueue, &msg, 1);
                        

                        uc_closeclnflag = 1;
                        guc_FarpWmHKUnRpyCnt = 0; //�����޻ظ���������
                    }
                    else
                    {
                        //Farp_SendHKData(0);      //������������
                        Farp_SendTestData(0);
                        guc_FarpWmHKUnRpyCnt++;
                        gui_FarpWmHBCYCnt = gs_FarpVar.hbcy;
//                        g_ulTcp_fd = HAL_WIFI_TCP_Lnk_Chk();
//	                        int fd = -1;
//	                        fd = HAL_WIFI_TCP_Lnk_Chk();
//	                        if(fd < 0)
//	                        {
//	                            SYS_Message_Send(MSG_FARP_CNTGPRS, TASK_FARP_TKID);
//	                        }
//	                        else
//	                        {
//	                            if(g_ulTcp_fd < 0)
//	                            {
//	                                g_ulTcp_fd = fd;
//	                                guc_FarpLoginConfirmErr = 0;
//	                                gul_FarpLoginDeadTime = gul_SysRunSecs + 10;
//	                                                                //
//	//                                gs_FarpVar.wmst |= WMS_STT_CLN;
//	                                guc_FarpPstTryCnt = 0;          //����ģʽ������������
//	//                                Farp_SendLoginData(0);          //���͵�¼֡
//	                                //Farp_SendHKData(0);             //��������֡
//	                                                                //��ʼ����������
//	                                gui_FarpWmHBCYCnt = 5;//gs_FarpVar.hbcy;
//	                                                                //ʵʱ����ģʽ����Ч���ݼ�������ʼ��
//	                                gui_FarpWmSafeCnt = gs_FarpVar.hbcy;
//	                                guc_FarpWmRcFailCnt = 0;        //����ʧ�ܴ�������
//	                            }
//	                        }                    
                    }
                }
            }
            else
            {
                gui_FarpWmHBCYCnt = gs_FarpVar.hbcy;
            }
            gui_FarpWmRcSec = 0;                //���Դ�������
                                                // 
//	            if(gs_FarpVar.wkmd == 0)            //ʵʱ����ģʽ
//	            {
//	                //....
//	            }
//	            else if(gs_FarpVar.wkmd == 1)       //��������ģʽ
//	            {
//	                gui_FarpBkTmCnt++;              //���������߼������ۼ�
//	                                                //������ͨѶʱ�䳬����ֵ����Ҫ�Ͽ�����
//	                if(gui_FarpBkTmCnt >= gs_FarpVar.slep)
//	                {                               //�öϿ�������Ϣ
//	                    SYS_Message_Send(MSG_FARP_DISGPRS, TASK_FARP_TKID);
//	//	                    uint8 msg = MSG_FARP_DISGPRS;
//	//	                    krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
//	                    
//	                    uc_closeclnflag = 1;
//	                }
//	            }
//	            else                                //����ģʽֱ�ӶϿ�����
//	            {
//	                SYS_Message_Send(MSG_FARP_DISGPRS, TASK_FARP_TKID);
//	                
//	//	                uint8 msg = MSG_FARP_DISGPRS;
//	//	                krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
//	                uc_closeclnflag = 1;
//	            }
        }
    }
    

    if((uc_closeclnflag == 1) && (gs_FarpVar.wkmd == 0))
    {
//	        SYS_Message_Send(MSG_FARP_CNTGPRS, TASK_FARP_TKID);
        uint8 msg = MSG_FARP_CNTGPRS;
        krhino_buf_queue_send(&gs_RFMngQueue, &msg, 1);
        
    }
}


//void wireless_send()
//{
//    uint8 msr = getMSR();
//    if ((msr & EZMAC_STATE_BIT) == EZMAC_PRO_IDLE)    //����״̬                            
//    {                                       	
//		//Switch_Channel(behave.freq); //��ҵ���ŵ�����ͨ�� 
//		// �ڿ����㲥ʱ϶�ĵط������ܹ�����ҵ������
//		//if(slot < ((rfpara.rf_slotnum * 2) - 3))
//		{
//			if(checkSendCache() )
//			{	
//
//                if(CheckConflict())
//                {
//                    goto recv;
//                }
//                else
//                {
//                    EZMacPRO_Transmit();//����RTS	
//                }
//			}
//			else
//            {
//				goto recv;
//            }
//		}
//        return ;
//recv: 
//		{
//			//�ɼ����ϻ������еĵ���������				
//			//EZMacPro_PacketType = PACKET_TYPE_2STEP;
//			EZMacPRO_Receive(); //�������ݰ�
////			if (BackOffSlot > 0)
////			    BackOffSlot--;						
//		}										
//        
//    }
//
//    
//}
//extern tRadioDriver *Radio;
extern uint8 g_ucUpgradeFlgForPush;
extern uint32 g_ucUpgradeCount;

//=======================================================
//void wireless_mng(void)//״̬������
//{
//    switch( Radio->Process( ) )
//    {
//    case RF_RX_TIMEOUT:
//
//        break;
//    case RF_RX_DONE:
//    {
//        PKT *pkt;
//        uint8 tail;
//        SYS_Dev_HalfBlinkSet(LED_FAR_RX, 0, 0, 0);
//        
//        if(g_ucUpgradeFlgForPush != 0xAA)
//        {
//            //uint16 frameLen = 0;
//            tail = rxdopktq.tail;
//            pkt = &(rxdopktq.rxpkt[tail]); //�������ݰ���ַ                        
//            //LOG_DEBUG( DBGFMT"======================RF_RX_DONE ========================= = \n",DBGARG);    
//
//            pkt->len = Radio->GetRxPacket( pkt->data);//, ( uint16* )&pkt->len );
//
//            CHN_ENUM channel = PST_CHN_WL;
//            uint8 protocal;   
//            if(gs_FRM.Channel(channel, pkt->data, pkt->len, &protocal) != PST_ERR_OK)
//            {
//                LOG_DEBUG( DBGFMT"rssi[%4.1f] crc check err %02x%02x%02x%02x%02x%02x\n",DBGARG, SX1276LoRaGetPacketRssi(),
//                    pkt->data[18], pkt->data[17],pkt->data[16],pkt->data[15],pkt->data[14],pkt->data[13]);     
//
//                LOG_DUMPHEX(LOG_LEVEL_DEBUG, "err data : \n", pkt->data, pkt->len);
//                
//                SYS_Dev_HalfBlinkSet(LED_FAR_RX, 1, 5, 2);
//                SYS_Dev_HalfBlinkSet(LED_FAR_TX, 1, 5, 2);        
//                break;
//            }
//
//            //frm.chninfo = channel & PST_FRM_CHNNO;  //ͨ����
//            pkt->protocol = protocal;
//    
//            rxdopktq.rxpkt[rxdopktq.tail].sentoffset = 0; // ���offsetֵ
//            
//            //�����յ��ĳ����������
//            rxdopktq.tail = (rxdopktq.tail + 1) & QUEUE_LEN_MASK;
//            rxdopktq.len = (rxdopktq.len + 1);                                          
//        }
//                //MSR = RX_STATE_BIT | RX_STATE_WAIT_FOR_SEND_ACK;
//        break;
//    }
//                
//    case RF_TX_DONE:
//        SYS_Dev_HalfBlinkSet(LED_FAR_TX, 0, 0, 0);
//
//        Radio->StartRx( );
//        if(NULL != g_stSendCacheIndex.ezPkt)
//        {
//            uint16 temp8 = 0;
//            uint8 errCode = 0;
//            g_stSendCacheIndex.ezPkt->bValid = 0;
//            g_stSendCacheIndex.ezPkt->nBackOffSlot = 0;
//            temp8 = g_stSendCacheIndex.ezPkt->TxPkt.index;
//            
//            if(g_stSendCacheIndex.level == CON_SEND_PRIORITY_NORMAL)
//            {
////	                temp8 = g_stSendCacheIndex.ezPk->TxPkt.index;
//                if(cltor_shadow[temp8].nodestatus.bDataAck != 0)
//                {
//                    cltor_shadow[temp8].nodestatus.bDataAck = 0;
//                }
////	                g_stSendCacheIndex.ezPk->bValid = 0;
////	                g_stSendCacheIndex.ezPk->nBackOffSlot = 0;
//                errCode = cltor_shadow[temp8].nodestatus.errCode;
//                
////	                g_ucPktRssiValue[EzDataTxPkt.TxPkt.apdu.seq] = 0;
//            }
//            else if(g_stSendCacheIndex.level == CON_SEND_PRIORITY_HIGH)
//            {
//
//                cltor_shadow[temp8].nodestatus.bNetAck = 0;//�Է��յ�ACK    
//                
//                errCode = cltor_shadow[temp8].nodestatus.result;
//                //cltor[EzNetTxPkt.TxPkt.apdu.index].nod.needack = 0;//�Է��յ�ACK    
//                
////	                g_ucPktRssiValue[EzNetTxPkt.TxPkt.apdu.seq] = 0;
//            }
////	            else
////	            {
////	                EzHHUTxPkt.bValid = 0;
////	                EzHHUTxPkt.nBackOffSlot = 0;
////	                temp8 = 0;
////	            }//	            if(EzCurTxType == EZ_TX_TYPE_DATA)
////	            {
////	                temp8 = EzDataTxPkt.TxPkt.index;
////	                if(cltor[temp8].nodestatus.bDataAck != 0)
////	                {
////	                    cltor[temp8].nodestatus.bDataAck = 0;
////	                }
////	                EzDataTxPkt.bValid = 0;
////	                EzDataTxPkt.nBackOffSlot = 0;
////	                errCode = cltor[temp8].nodestatus.errCode;
////	                
////	//	                g_ucPktRssiValue[EzDataTxPkt.TxPkt.apdu.seq] = 0;
////	            }
////	            else if(EzCurTxType == EZ_TX_TYPE_NET)
////	            {
////	                EzNetTxPkt.bValid = 0;
////	                EzNetTxPkt.nBackOffSlot = 0;
////	                temp8 = EzNetTxPkt.TxPkt.index;
////	                cltor[temp8].nodestatus.bNetAck = 0;//�Է��յ�ACK    
////	                
////	                errCode = cltor[temp8].nodestatus.result;
////	                //cltor[EzNetTxPkt.TxPkt.apdu.index].nod.needack = 0;//�Է��յ�ACK    
////	                
////	//	                g_ucPktRssiValue[EzNetTxPkt.TxPkt.apdu.seq] = 0;
////	            }
////	            else
////	            {
////	                EzHHUTxPkt.bValid = 0;
////	                EzHHUTxPkt.nBackOffSlot = 0;
////	                temp8 = 0;
////	            }
//            
//            if ((temp8 != 0) && (cltor[temp8].devAddr[0] < 0xFF)
//                && (cltor[temp8].nodestatus.NetStatus == NODE_STATUS_OUT
//                && errCode == 0)
//                /*&& (cltor[temp8].nod.ans_pkt_type == 0)*/)
//            {
//                cltor[temp8].nodestatus.NetStatus = NODE_STATUS_LOGIN;
//            }
//        }
//        break;
//    case RF_TX_TIMEOUT:
//        
////	        if(EzCurTxType == EZ_TX_TYPE_DATA)//1���
////	        {
////	            EzDataTxPkt.nBackOffSlot = 40;
////	        }
////	        else
////	        {
////	            EzNetTxPkt.nBackOffSlot = 40;
////	        }
//
//        g_stSendCacheIndex.ezPkt->nBackOffSlot = FUNC_DELAY_MS(1000);
//        //Radio->StartRx( );
//        break;
//    case RF_IDLE:
//        Radio->StartRx( );
//        break;
////	    case RF_RX_CRCERR:
////	        LED_BlinkSet(LED_FAR_RX, 1, 5, 2);
////	        break;
//    default:
//        break;
//    }
//
//}
////=======================================================
//void wireless_proc(void)//
//{
//    wireless_mng();
//}

extern __IO uint32_t uwTick;
ktimer_t     g_rf_tick_timer;
void SYS_RFMng_Task(void * arg)
{
    TIME time;
    kstat_t ret = 0;
                                            //������Ϣ
    //uint8 nmsg;                             //������Ϣ
    
    krhino_buf_queue_create(&gs_RFMngQueue, "rf_queue",
                         gc_RFMngbuf, MSG_BUFF_LEN, BUFQUEUE_MSG_MAX);
    localid = 1; 

    Farp_PreInit();
    Radio = RadioDriverInit( );
    SYS_RF_Init(0,0,0);
    Radio->Tick((uint32 *)&uwTick);
//    static uint8_t ble_name[14] = {'V','S'};
//    ByteArrayBcdToHexString(gs_PstPara.Addr, ble_name+2, 6, 0);
    
//    g_ucPutcharEn = 0;
//
//    SYS_MAIN_Init();
                                            //�����ȡʱ����Ϣ
    SYS_MSG_Apply(TASK_RFMNG_TKID, MSG_CLS_TM);//*��ʼ����ػ���
//    SYS_MSG_Apply(TASK_MAIN_TKID, MSG_CLS_GPIO);
//    SYS_MSG_ApplyExt(TASK_MAIN_TKID, MSG_CLS_UART, UART_CHANNEL_DEBUG);

//    LOG_DEBUG("\nVS Project %s  Softver[%x] Hardver[%x]!!!\n", gucs_PrjCode, gul_UsrFuncVer, gul_UsrHardcVer);
    //Flash_Test();
    krhino_timer_create(&g_rf_tick_timer, "rf_tick_timer", Slot_Time_IRQ,
                        krhino_ms_to_ticks(SYS_TIMER_COUNT_MS), krhino_ms_to_ticks(SYS_TIMER_COUNT_MS), 0, 1);
    for(;;)
    {   
        ret = krhino_buf_queue_recv(&gs_RFMngQueue, 1/*RHINO_WAIT_FOREVER*/, g_RFMng_buf_recv,
                          &g_RFMng_recv_size);
        if(ret == RHINO_SUCCESS)
        {
            switch(g_RFMng_buf_recv[0])
            {
                case MSG_SEC:
                    Farp_WMSecondProc();
                    break;
                case MSG_FARP_RECVDATA:
                {
                    PKT *pkt;
            		while (rxdopktq.len > 0)  //������ն�����������  	      
            		{
            			pkt = &(rxdopktq.rxpkt[rxdopktq.head]); //���߽��ն��е����ݰ���ַ***
            			uint8_t Return = PST_RX_Down_PKT(pkt); //�����������ݰ�
            			if(0 == Return)
            			{
            				Clean_PKT(pkt); // ���rxdopktq���Ѵ�������
            				rxdopktq.len = (rxdopktq.len - 1); //���г��ȼ�1
            				rxdopktq.head = (rxdopktq.head + 1) & QUEUE_LEN_MASK; //�жϷ�ת
            			}
            			else
                        {         
                            break;
                        }
            		}
                    break;
                }
                case MSG_LIVE:                  //�ظ�������Ϣ
                    HB_RetLive(TASK_RFMNG_TKID);
                    break; 
                default:
                    
                    break;
            }
        }
        else
        {
//#ifdef MASTER_NODE          
//	            SYS_A7139_Proc(1);
//#else
//            SYS_A7139_Proc(0);
//#endif            
        }
    }
}

/******************************************************************************
**RF���̿鳣�����ֶ���
******************************************************************************/
const KTaskConst gs_TkLP2PCmm = 
{
    &gs_RFMngHandle,
    SYS_RFMng_Task,
    gs_RFMngStack,
    TASK_RFMNG_TKID,                           //���̱��
    true,                                    //��Ϣѭ�����ƽ���
    TASK_RFMNG_STKL,
    "rfmng",
    &gs_RFMngQueue,
};
