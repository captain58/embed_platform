#define EXT_RF_LOCAL
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
uint16 gui_RFWmSafeCnt;                   //����ͨѶ״̬����������ȫʱ�䵹��ʱ(��)
uint16 gui_RFSerSafeCnt;                   //����ͨѶ״̬����������ȫʱ�䵹��ʱ(��)



/******************************************************************************
**�����������ʱ
******************************************************************************/
uint16 gui_RFWmRcSec;                     //GPRSͨ·�������������(�뵹��ʱ)
uint16 gui_RFSerRcSec;                     //GPRS Tcp Serverͨ·�������������(�뵹��ʱ)


/******************************************************************************
**��������ʧ�ܴ���
******************************************************************************/
uint8 guc_RFWmRcFailCnt;                  //GPRS��������ʧ�ܴ���
uint8 guc_RFSerRcFailCnt;                 //Tcp Server��������ʧ�ܴ���
uint16 guc_RFWmRcFailNorFtpCnt;                  //GPRS������������FTP����


/******************************************************************************
**��������
******************************************************************************/
uint8 guc_RFWmRcCnt;                      //����ʧ�ܺ����������(yzy:���ڷ�������ǰ��ֵ)
uint8 guc_RFSerRcCnt;                     //����ʧ�ܺ����������(yzy:���ڷ�������ǰ��ֵ)



/******************************************************************************
**�������ڼ�����
******************************************************************************/
uint16 gui_RFWmHBCYCnt;                   //�������ڼ�����


uint8 guc_RFWmHKUnRpyCnt;                 //�����޻ظ�����������




/******************************************************************************
**����������
******************************************************************************/
uint16 gui_RFHdRstCnt;                    //����è��ʼ������ʱ�������(��δ�忨ʱ����ֳ�ʼ��ʧ��)
uint8 guc_RFPstTryCnt;                    //GPRSͨ·���������ز�����������
uint8 guc_RFNVHours;                      //������ͨѶСʱ��
uint8 guc_Fast2WmSttReqCnt;                  //MD ����������ʱ

uint16 gui_RFServerNoDataCnt;             //TCP Server  ������������£������ݼ���
uint16 gui_RFFromClnNoDataCnt;             //TCP Server  ��ͻ���1���ӵ�����£������ݼ���
uint8  guc_RFFromClnCloseCnt;             //TCP Server  ��ͻ���1���ӵ������, �����رտͻ��˴���

uint8  guc_RFReActiveFlag;                //��������ģʽ���ɼ�����������Ҫ������վ��������



//uint16 gui_smseq;
static char         g_RFMng_buf_recv[BUFQUEUE_MSG_MAX+4];

size_t       g_RFMng_recv_size;
ktimer_t     g_RFMngTimer;

cpu_stack_t  gs_RFMngStack[TASK_RFMNG_STKL];
ktask_t      gs_RFMngHandle;
kbuf_queue_t gs_RFMngQueue;
char         gc_RFMngbuf[MSG_BUFF_LEN];

void RF_SendTestData(uint8 ch);
/************************************************************************
 * @function: RF_PreInit
 * @����: Զ�̲�����ʼ��
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void RF_PreInit(void)
{                                           //��������
    memset((uint8*)&gs_RFVar, 0, sizeof(FarpVar));
    memset((uint8_t *)&gs_FtpPara,0,sizeof(S_FTPPARA));
    
//    LoadSystemParam(PARA_TYPE_FARP);

//    gui_RFWmRcSec = 10;                   //��ʼ�����10s�����Զ������
//    gui_RFSerRcSec = 10;
//    //gui_RFEthRcSec = 10;
//    
//    guc_RFWmHKUnRpyCnt = 0;               //�����޻ظ���������������
//    //guc_RFEthHKUnRpyCnt = 0;
//    
//    gui_RFWmSafeCnt = 0;
//    gui_RFSerSafeCnt = 0;
//    //gui_RFEthSafeCnt = 0;
//    
//    guc_RFWmRcCnt = 0;                   //����ʧ�ܺ�������������
//    guc_RFSerRcCnt = 0;
//    //guc_RFEthRcCnt = 0;
//    
//    gui_RFHdRstCnt = 0;                   //ģ�鸴λ����������
//    guc_RFNVHours = 0;                    //��IP����ʱ������
//    guc_RFPstTryCnt = 0;                   //��������ģʽ�ز���������������
//    
//    gui_RFBkTmCnt = 0;                    //�����������������߼���������
//    guc_RFWmRcFailCnt = 0;
//    guc_RFWmRcFailNorFtpCnt = 0;
//    //guc_RFEthRcFailCnt = 0;
//    
//    gui_RFServerNoDataCnt = 0;
//    guc_RFFromClnCloseCnt = 0;
//    
//    guc_RFLoginConfirmErr = 0;
//    gul_RFLoginDeadTime = 0xFFFFFFFF;
//    
//    gs_RFVar.wmsv = 0x88000000;
    //gs_SysVar.mLPstt |= HLV_LPTASK_MDCK;

}
/************************************************************************
 * @function: RF_SendIpData
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
uint8 RF_SendIpData(uint8* buffer, uint16 len)
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
//            krhino_buf_queue_send(&gs_TKRFQueue, &msg, 1);
//            
//            gui_RFWmRcSec = 10;
//        }
//    }
    return SYS_ERR_FT;                      //IP���ݷ���ʧ��
}
/************************************************************************
 * @function: RF_SendTestData
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
void RF_SendTestData(uint8 ch)
{
    if(gs_RFVar.login)
    {
        gs_RFVar.hklen = RF_BuildLinkFrm(0x09, 0, gs_RFVar.hkd);
    }
    else
    {
        gs_RFVar.hklen = RF_BuildLinkFrm(0x05, 0xA619, gs_RFVar.hkd);
    }
    if(gs_RFVar.hklen > 0)
    {
        if(ch == 0)
        {
//	            RF_SendIpData((uint8 *)cmdUpBuf, sizeof(cmdUpBuf));
            RF_SendIpData((uint8 *)gs_RFVar.hkd, gs_RFVar.hklen);

        }
        else
        {
            //RF_SendEthData(gs_RFVar.hkd, gs_RFVar.hklen);
        }
    }
}

///************************************************************************
// * @function: RF_WMSecondProc
// * @����: ����è���봦��
// * @����: 
// * @����: 
// * @˵��: 
// * @����: yzy (2014/1/14)
// *-----------------------------------------------------------------------
// * @�޸���: 
// ************************************************************************/
void RF_WMSecondProc(void)
{
    uint8 uc_closeclnflag = 0;
//    uint8 uc_closeserflag = 0;
    /*
    if(gs_RFVar.wmst & WMS_ETH0STT)
    {
        return;
    }
    */
                                            //yzy 2014-04-09,�������Ҫ����̫������,ֻҪ��⵽���߼�����GPRS.
//	    if(gs_RFVar.wmst & WMS_EHSTT)
//	    {
//	        if((gs_RFVar.wmst & WMS_STT_CLN) || (gs_RFVar.wmst & WMS_STT_SER))
//	        {
//	//	            SYS_Message_Send(MSG_FARP_DISPPP, TASK_FARP_TKID);
//	            uint8 msg = MSG_FARP_DISPPP;
//	            krhino_buf_queue_send(&gs_TKRFQueue, &msg, 1);
//	            
//	        }
//	        gs_RFVar.wmst &= ~WMS_CHALLSTT;
//	        return;
//	    }                                       //yzy 2014-04-09
                                            //��ȡģ��״̬
    TRFModemState modemstt = SYS_RF_Status();//SYS_MODM_Status();
	
    if(gui_RFWmSafeCnt > 0)               //����ͨѶ״̬����������ȫʱ�䵹��ʱ
    {
        gui_RFWmSafeCnt--;
    }
    
    if(gui_RFSerSafeCnt > 0)              //TCP Server ������ͨѶ״̬����������ȫʱ�䵹��ʱ
    {
        gui_RFSerSafeCnt--;
    }
    
//    if(!(modemstt.bit.typeChecked))         //ģ�黹δʶ��
//    {
//        return;
//    }
    
    if(!(modemstt.bit.chnrdy))              //�Ƿ����AT����
    {
        if(++guc_Fast2WmSttReqCnt >= 5)      //ÿ5����
        {
            guc_Fast2WmSttReqCnt = 0;
            //SYS_MODM_Step(Modem_Init);
//            SYS_MODM_Reinit();
//	            HAL_WIFI_Init(TASK_FARP_TKID,"tw14",gs_RFVar.ap , &g_ulTcp_fd, 4);
//            gs_GPIO.LED_BlinkSet(GPIO_LED_GPRS, 0xFF, 0, 0);
        }
        return;
    }
	
    if(!(modemstt.bit.regtt))               //�Ƿ��������ע��
    {
        gs_RFVar.wmst &= ~WMS_CHALLSTT;
        gs_RFVar.mcsq = 0;
                                            //��ʱ����ģ��״̬
        if(++guc_Fast2WmSttReqCnt >= 15)     //ÿ15����
        {
            guc_Fast2WmSttReqCnt = 0;
//	            HAL_WIFI_BeginCheck();
        }
        return;
    }
	
    gs_RFVar.wmst |= WMS_STT_GSM;
    gs_RFVar.mcsq = modemstt.bit.signl;
//    SYS_LED_BlinkSet1(GPIO_LED_GPRS, 1, 10, 10, 0);  //ע��GSM����ɹ���1HZ��˸
    
    if(!(modemstt.bit.pppoe))       //�Ƿ����PPP����
    {
        gs_RFVar.wmst &= ~WMS_STT_PPP;    //����ͨ��������
        if(++guc_Fast2WmSttReqCnt >= 30)     //ÿ10����
        {
//	            SYS_Message_Send(MSG_FARP_CNTPPP, TASK_FARP_TKID);
            uint8 msg = MSG_FARP_CNTPPP;
            krhino_buf_queue_send(&gs_RFMngQueue, &msg, 1);
            
            guc_Fast2WmSttReqCnt = 0;
        }
//	        
//        if(guc_FastWmSttReqCnt % 5 == 0)     //ÿ3����
//        {
//            HAL_WIFI_BeginCheck();
//        }
        return;
    }
    gs_RFVar.wmst |= WMS_STT_PPP;
    
    
    


//	    if(gs_RFVar.clnen == 1)           //�ͻ���ģʽ
    {
        if(gs_RFVar.login == 0)    //TCP ������
        {
            gs_RFVar.wmst &= ~WMS_STT_CLN;    //����ͨ��������
            if(gs_RFVar.wkmd == 0x00)         //ʵʱ����ģʽ
            {
                if(gui_RFWmRcSec == 0)        //��������ģʽ���ߺ󼴿�ʼ���Ե���ʱ
                {
                    gui_RFWmRcSec = gs_RFVar.rttm;
                }
                                                //����ͨѶ״̬�µ�����������
                if(gui_RFWmSafeCnt > 0)
                {
                    gui_RFWmSafeCnt = 0;
                    
                    gui_RFWmRcSec = 1;
                    guc_RFWmRcCnt = 3;       //ͨѶ״̬�¶�������3��
                }
                if(gui_RFWmRcSec > 0)
                {
                    gui_RFWmRcSec--;         //�������ݼ�
                    if(gui_RFWmRcSec == 0)   //���Լ���ѵ�,��������
                    {
//	                        SYS_Message_Send(MSG_FARP_CNTGPRS, TASK_FARP_TKID);
                        uint8 msg = MSG_FARP_CNTGPRS;
                        krhino_buf_queue_send(&gs_RFMngQueue, &msg, 1);
                        
                    }
//	                    if(gui_RFWmRcSec % 15 == 0)
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
//	                                guc_RFLoginConfirmErr = 0;
//	                                gul_RFLoginDeadTime = gul_SysRunSecs + 10;
//	                                                                //
//	//                                gs_RFVar.wmst |= WMS_STT_CLN;
//	                                guc_RFPstTryCnt = 0;          //����ģʽ������������
//	                                RF_SendLoginData(0);          //���͵�¼֡
//	                                //RF_SendHKData(0);             //��������֡
//	                                                                //��ʼ����������
//	                                gui_RFWmHBCYCnt = 5;//gs_RFVar.hbcy;
//	                                                                //ʵʱ����ģʽ����Ч���ݼ�������ʼ��
//	                                gui_RFWmSafeCnt = gs_RFVar.hbcy;
//	                                guc_RFWmRcFailCnt = 0;        //����ʧ�ܴ�������
//	                            }
//	                        }
//	                    }
                }
            }
            else if(gs_RFVar.wkmd == 0x01)    //��������ģʽ
            {
                //
            }
        }
        else
        {
            gs_RFVar.wmst |= WMS_STT_CLN;     //����ͨ������
            
            if(guc_RFLoginConfirmErr)           //��¼��n����δ�յ�ȷ��֡,ִ����������
            {
                if(gul_SysRunSecs > gul_RFLoginDeadTime)
                {
//	                    SYS_Message_Send(MSG_FARP_DISGPRS, TASK_FARP_TKID);
                    uint8 msg = MSG_FARP_DISGPRS;
                    krhino_buf_queue_send(&gs_RFMngQueue, &msg, 1);
                    
                    uc_closeclnflag = 1;
                    gul_RFLoginDeadTime = 0xFFFFFFFF;
                }
            }
            
            if(gui_RFWmHBCYCnt > 0)
            {
                gui_RFWmHBCYCnt--;
                if(gui_RFWmHBCYCnt == 0)      //�������ڵ�������֡
                {
                    if(guc_RFWmHKUnRpyCnt >= 3) //�����޻ظ���������3��
                    {
//	                        SYS_Message_Send(MSG_FARP_DISGPRS, TASK_FARP_TKID);
                        uint8 msg = MSG_FARP_DISGPRS;
                        krhino_buf_queue_send(&gs_RFMngQueue, &msg, 1);
                        

                        uc_closeclnflag = 1;
                        guc_RFWmHKUnRpyCnt = 0; //�����޻ظ���������
                    }
                    else
                    {
                        //RF_SendHKData(0);      //������������
                        RF_SendTestData(0);
                        guc_RFWmHKUnRpyCnt++;
                        gui_RFWmHBCYCnt = gs_RFVar.hbcy;
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
//	                                guc_RFLoginConfirmErr = 0;
//	                                gul_RFLoginDeadTime = gul_SysRunSecs + 10;
//	                                                                //
//	//                                gs_RFVar.wmst |= WMS_STT_CLN;
//	                                guc_RFPstTryCnt = 0;          //����ģʽ������������
//	//                                RF_SendLoginData(0);          //���͵�¼֡
//	                                //RF_SendHKData(0);             //��������֡
//	                                                                //��ʼ����������
//	                                gui_RFWmHBCYCnt = 5;//gs_RFVar.hbcy;
//	                                                                //ʵʱ����ģʽ����Ч���ݼ�������ʼ��
//	                                gui_RFWmSafeCnt = gs_RFVar.hbcy;
//	                                guc_RFWmRcFailCnt = 0;        //����ʧ�ܴ�������
//	                            }
//	                        }                    
                    }
                }
            }
            else
            {
                gui_RFWmHBCYCnt = gs_RFVar.hbcy;
            }
            gui_RFWmRcSec = 0;                //���Դ�������
                                                // 
//	            if(gs_RFVar.wkmd == 0)            //ʵʱ����ģʽ
//	            {
//	                //....
//	            }
//	            else if(gs_RFVar.wkmd == 1)       //��������ģʽ
//	            {
//	                gui_RFBkTmCnt++;              //���������߼������ۼ�
//	                                                //������ͨѶʱ�䳬����ֵ����Ҫ�Ͽ�����
//	                if(gui_RFBkTmCnt >= gs_RFVar.slep)
//	                {                               //�öϿ�������Ϣ
//	                    SYS_Message_Send(MSG_FARP_DISGPRS, TASK_FARP_TKID);
//	//	                    uint8 msg = MSG_FARP_DISGPRS;
//	//	                    krhino_buf_queue_send(&gs_TKRFQueue, &msg, 1);
//	                    
//	                    uc_closeclnflag = 1;
//	                }
//	            }
//	            else                                //����ģʽֱ�ӶϿ�����
//	            {
//	                SYS_Message_Send(MSG_FARP_DISGPRS, TASK_FARP_TKID);
//	                
//	//	                uint8 msg = MSG_FARP_DISGPRS;
//	//	                krhino_buf_queue_send(&gs_TKRFQueue, &msg, 1);
//	                uc_closeclnflag = 1;
//	            }
        }
    }
    

    if((uc_closeclnflag == 1) && (gs_RFVar.wkmd == 0))
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
extern uint8_t guc_netStat;
ktimer_t     g_rf_tick_timer;
ktimer_t     g_rf_mainloop_timer;
void SYS_RFMng_Task(void * arg)
{
    TIME time;
    kstat_t ret = 0;
                                            //������Ϣ
    //uint8 nmsg;                             //������Ϣ
    
    krhino_buf_queue_create(&gs_RFMngQueue, "rf_queue",
                         gc_RFMngbuf, MSG_BUFF_LEN, BUFQUEUE_MSG_MAX);
    localid = 1; 

    RF_PreInit();
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
                        krhino_ms_to_ticks(1000), krhino_ms_to_ticks(SYS_TIMER_COUNT_MS), 0, 1);
#ifdef MASTER_NODE
    extern void Handle_Proc();
    krhino_timer_create(&g_rf_mainloop_timer, "rf_mainloop_timer", Handle_Proc,
                        krhino_ms_to_ticks(100), krhino_ms_to_ticks(100), 0, 1);
   
#endif    
    for(;;)
    {   
        ret = krhino_buf_queue_recv(&gs_RFMngQueue, 1/*RHINO_WAIT_FOREVER*/, g_RFMng_buf_recv,
                          &g_RFMng_recv_size);
        if(ret == RHINO_SUCCESS)
        {
            switch(g_RFMng_buf_recv[0])
            {
                case MSG_SEC:
                    RF_WMSecondProc();
                    break;


                case MSG_SWITCH_CHANGE:
                    if(guc_netStat == NODE_STATUS_LOGIN)
                    {
//	                        EZMacPRO_Transmit_Adv(3, NULL, 0);	
                        EZMacPRO_Transmit_Adv(3, NULL, 0);
                    }
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
#if (SYS_LOW_POWER > 0)                
                case MSG_ENTER_SLEEP://��������
                    if(!gs_RFVar.sleep)
                    {
                        Radio->enter_sleep();
                        gs_RFVar.sleep = 1;
                        
                        msleep(500);
                        
                        krhino_timer_stop(&g_rf_tick_timer);
                        SYS_Dev_OptBlinkSet(SYS_LED_RUN, 3, 1, 99, 0); 
	
                    }
                    gs_SysVar.mLPsysstt |= HLV_STT_NENG;   //(2)
                    break;
                case MSG_WAKE_UP://����
                    if(gs_RFVar.sleep)
                    {
                        krhino_timer_start(&g_rf_tick_timer);
                        msleep(1);
                        SYS_RF_Init(0,0,0);
//                        Radio->wake_up();
                        gs_RFVar.sleep = 0;
                        SYS_Dev_OptBlinkSet(SYS_LED_RUN, 1, 50, 50, 0); 
	
                    }
                    gs_SysVar.mLPsysstt &= ~HLV_STT_NENG;   //(2)
                    break;
#endif                    
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
