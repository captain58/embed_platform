#include <k_api.h>
#include <stdio.h>
#include <stdlib.h>
#include "public.h"
#include "user_func.h"
#include "wirelessSendCache.h"
#include "A7139reg.h"
#include "LinkMng.h"
extern uint32 nCurTimeMin;
extern uint32 nLastCurTimeMin;

/****�㲥����ض���**************************************************/
extern uint32 adv_updatetime;
extern uint8 adv_sublen;
extern uint8 adv_subtype;

//�����ɼ���ʱ���������һ�εĲɼ����ϻ�ʱ����
extern uint8 bSendTickToSS;
extern uint32 u32LastFoundSlot;

extern uint8 netcid ;
extern uint8 localhop;
#define DELETE_CTRL_NUM 10
typedef struct _STDELETECTRL
{
    uint16 id;
    uint16 times;
}STDELETECTRL;

STDELETECTRL g_stDeleteCtrl[DELETE_CTRL_NUM];
//	extern volatile uint32 MSR;
extern EZ_Cur_Tx_Type EzCurTxType;
double g_rfLR_rssi_lmit = RFLR_RSSI_VALUE_LIMT;
extern uint8 BackOffSlot;//����ʱ��

int getDeleteCtrlST(uint16 id)
{
    uint16 i;
    for(i = 0; i < DELETE_CTRL_NUM; i++)
    {
        if(g_stDeleteCtrl[i].id == id)
        {
            break;
        }
    }

    if(i == DELETE_CTRL_NUM)
    {
        for(i = 0; i < DELETE_CTRL_NUM; i++)
        {
            if(g_stDeleteCtrl[i].id == 0xFFFF)//��
            {
                g_stDeleteCtrl[i].id = id;
                break;
            }
        }
    }
    if(i == DELETE_CTRL_NUM)
        return 0xFFFF;
    else
        return i;
}
uint8 decDeleteCtrlST(uint16 id)
{
    int i = getDeleteCtrlST(id);

    if(i == 0xFFFF)
        return 1;
    g_stDeleteCtrl[i].times++;
    if(g_stDeleteCtrl[i].times >= 5)
    {
        g_stDeleteCtrl[i].id=0xFFFF;
        g_stDeleteCtrl[i].times = 0;
        return 1;
    }
    return 0;
}

void DeleteProcInit(void)
{
    for(int i = 0; i < DELETE_CTRL_NUM; i++)
    {
        g_stDeleteCtrl[i].id = 0xFFFF;
        g_stDeleteCtrl[i].times = 0;
    }
}
//=======================================================
void Handle_JRep(void)//�������������
{
    uint16 loop, AckID;
    uint8 AckTTL, bNeedAck;
    EZ_TX_PKT * ezPkt = getIdelSendCacheByPri(CON_SEND_PRIORITY_HIGH);
    
    if(ezPkt == NULL)//���籨�Ļ�����Ч
    {
        return ;
    }

    bNeedAck = 0;
	
	//ȡ�����ЧSS�洢�ռ�
	//����SS�����������IDΪ0�Ļظ�����
#ifdef PROJECT_PINGSHUI	
	AckID  =  1;
	for (loop = 1; loop <= rfpara.rf_slotnum; loop++)	
	{
		//�ж��Ƿ���Ҫ�ظ�����Ӧ��
		if (cltor_shadow[loop].nod.needack)
		{
			if(0 == bNeedAck)
			{
				AckID = 	loop;
				AckTTL = cltor_shadow[loop].FrameDeadTime;
				bNeedAck = 1;
			}
			else
			{
				if(cltor_shadow[loop].FrameDeadTime < AckTTL)
				{
					AckID = 	loop;
					AckTTL = cltor_shadow[loop].FrameDeadTime;
				}
			}
		}
	}
#else
		//ȡ����Ҫɾ���Ľڵ�
	AckID  =  0;
	for (loop = 2; loop <= rfpara.rf_slotnum; loop++)	
	{
		if(cltor[loop].nodestatus.bNeedDelete && cltor_shadow[loop].FrameDeadTime == 0 && 
            checkDuplicateWithPri(CON_SEND_PRIORITY_HIGH, loop) == 0)
		{
			AckID = loop;
			bNeedAck = 1;
			break;
		}
	}

	if(0 == bNeedAck)
	{
		for (loop = 0; loop <= rfpara.rf_slotnum; loop++)	
		{
			//�ж��Ƿ���Ҫ�ظ�����Ӧ��
			if(1 != loop)
			{
				if (cltor_shadow[loop].nodestatus.bNetAck && checkDuplicateWithPri(CON_SEND_PRIORITY_HIGH, loop) == 0)
				{
					if(0 == bNeedAck)
					{
						AckID = loop;
						AckTTL = cltor_shadow[loop].FrameDeadTime;
						bNeedAck = 1;
					}	
					else
					{
						if(cltor_shadow[loop].FrameDeadTime < AckTTL)
						{
							AckID = 	loop;
							AckTTL = cltor_shadow[loop].FrameDeadTime;
						}
					}
				}
			}
		}
	}
#endif

	if(bNeedAck)
	{
        //LOG_DEBUG( DBGFMT"Create_JRep[%x] point addr %x\n",DBGARG, AckID, &(*(int *)ezPkt));                        

    
		Create_JRep(AckID, ezPkt);

		//ɾ���ɼ����ڵ�
//			if(cltor[AckID].nodestatus.bNeedDelete && decDeleteCtrlST(AckID))
//	        {
//	            int ret;
//				ret = Hash_Table_Search1(cltor[AckID].devAddr, cltor[AckID].addrLen); //���ҹ�ϣ���е�����ֵ				
//	            if (ret != ( - 1))
//	            {                     
//	                Updata_Hash_Table(ret, NULL);  //ɾ��                                              
//	
//	                //ɾ����֤�ɼ�������
//	                //Del_Validate_W_B(cltor[id].devAddr);
//	            }
//	            Delete_SS_Node(AckID);
                        
            
//        }      
	}
		
}
void Handle_Affirm(void)//������������
{
    uint16 loop, AckID;
    uint8 AckTTL, bNeedAck;
    EZ_TX_PKT * ezPkt = getIdelSendCacheByPri(CON_SEND_PRIORITY_NORMAL);
    
    if(ezPkt == NULL)//���籨�Ļ�����Ч
    {
        return ;
    }
//	    if(EzDataTxPkt.bValid)//���籨�Ļ�����Ч
//	        return ;

    bNeedAck = 0;
	
	//ȡ�����ЧSS�洢�ռ�
	//����SS�����������IDΪ0�Ļظ�����

	if(0 == bNeedAck)
	{
		for (loop = 0; loop <= rfpara.rf_slotnum; loop++)	
		{
			//�ж��Ƿ���Ҫ�ظ�����Ӧ��
			if (cltor_shadow[loop].nodestatus.bDataAck != 0 && checkDuplicateWithPri(CON_SEND_PRIORITY_NORMAL, loop) == 0)
			{
				if(0 == bNeedAck)
				{
					AckID = loop;
					AckTTL = cltor_shadow[loop].FrameDeadTime;
					bNeedAck = 1;
				}	
				else
				{
					if(cltor_shadow[loop].FrameDeadTime < AckTTL )
					{
						AckID = 	loop;
						AckTTL = cltor_shadow[loop].FrameDeadTime;
					}
				}
			}
		}
	}
	if(bNeedAck)
	{
		Create_Affirm(AckID, ezPkt);
 	}
		
}


/*************************************************************************
 * Function Name: UpdateNode
 * Parameters:  
 * Return:  
 * Description: 
 *
 *************************************************************************/

void Update_Node()//���������Ľڵ�
{
    uint32 i;
//        uint8 cnt;
//        cnt=5;
		
//	ȡ�����ЧSS�洢�ռ�

	for (i = 2; i <= rfpara.rf_slotnum; i++)  // �Ƿ����
    {  
		if (Before(cltor_shadow[i].father_update + (NODE_UPDATE_TIME*cltparm.routeuptime), curslottime) 
//				&& Before(cltor_shadow[i].mother_update +   (NODE_UPDATE_TIME*cltparm.routeuptime), curslottime) 
			//&& (cltor[i].nod.NetStatus == NODE_STATUS_LOGIN)
			&& (cltor[i].devAddr[0] < 0xFF))
		{
#ifdef _Debug
			Test[0] = 0x33;
			Test[1] = 0x33;
			Test[2] = 0x33;
			Test[3] = i;
			memcpy(&Test[4], cltor[i].devAddr, 6);
			UART_PutArray(UART1, Test, 10); // for test
#endif
            cltor[i].nodestatus.bNeedDelete = 1;
            cltor_shadow[i].nodestatus.ans_pkt_type = 3;
            if((!cltor_shadow[i].nodestatus.bDataAck) && (!cltor_shadow[i].nodestatus.bNetAck))
            {
                cltor_shadow[i].FrameDeadTime = 0;
            }

            LOG_DEBUG( DBGFMT"node[%d] addr[%02x%02x%02x%02x%02x%02x] heartBeat = %d Update_Node  curslottime = %d, father_update = %d \n",DBGARG, i,
                cltor[i].devAddr[5], cltor[i].devAddr[4],cltor[i].devAddr[3],cltor[i].devAddr[2],cltor[i].devAddr[1],cltor[i].devAddr[0],
                cltparm.routeuptime, curslottime, cltor_shadow[i].father_update);

//				Delete_SS_Node(i);	
    	}
    }
}



void BoardPktTimeOutProc()
{
	if(nLastCurTimeMin != nCurTimeMin)		
	{
		nLastCurTimeMin = nCurTimeMin;
		if((nBroadMeterTime > 0) && (bBroadMeterEnable))
		{
			nBroadMeterTime --;
		//	I2cWrite(0xA0, (uint8*)&nBroadMeterTime, FM_BROAD_METER_TIME , 2); 
		}
	}
	
	if(bBroadMeterEnable)
	{
		if(0 == nBroadMeterTime)
		{
			//�رչ㲥���
			CloseBoardSearchMeter(0);			
		}
		else
		{
			//���͹㲥�����
			adv_subtype = ADV_TYPE_ADDR_SEARCH; //����ѯ      
			adv_updatetime = curslottime + 2400 * nBroadMeterTime;
			adv_sublen = 0;		
		}
	}
	else
	{			
		if (Before(adv_updatetime, curslottime))//�ض��㲥����ʱ�����
		{
			if(ADV_TYPE_BOARD_JC != adv_subtype)
			{
				adv_subtype = ADV_TYPE_EMPTY;//��ͨ��
				adv_sublen = ADV_SUB_LEN;   
			}
		}
	}
}



void SendTickToSSProc()
{
	//�����ɼ����������ϻ�ʱ����ȶȶ�SS���е���
	//�м���Ҫ��¼��һ�ε�ʱ����
	//��ǰ�����μ����ȵ�SS, ���û���ҵ�,���������ȵ�SS
	uint8 bFound = 0;
	uint8 u8FoundID = 0;
	uint32 u32UpdataSlot;
	uint8 bHaveMinSS = 0;
	uint32 u32MinSlot =0;
	uint8 u8MinSlotID = 0;
    uint16 Loop;
	uint32 CurSlot = curslottime;
	PKT *pkt;
//	uint8 recvid;
	STAREQUEST *spkt;
	uint32 MaxTickSlot = 0;
	uint32 u32CurFoundSlot =  0;
	
//		if(bSendTickToSS)
//		{
//	//			if(Before(u32SendTickToSSDeadTime, curslottime))
//				bSendTickToSS = 0;
//		}
    EZ_TX_PKT * ezPkt = getIdelSendCacheByPri(CON_SEND_PRIORITY_HIGH);

	if(NULL == ezPkt)//���籨�Ļ�����Ч
    {   
		return;
    }
	
	if(0 == bSendTickToSS)
	{
		CurSlot = curslottime;
		MaxTickSlot = ((uint32)cltparm.routeuptime * 2400);
		u32MinSlot = CurSlot;
		u32CurFoundSlot = CurSlot;
		COLLECTOR* pCltor;
		COLLECTOR_SHADOW* pCltor_shadow;
        
		for(Loop = 2; Loop <= rfpara.rf_slotnum; Loop++)
		{
			pCltor = &(cltor[Loop]);
			pCltor_shadow= &(cltor_shadow[Loop]);
            
			if(pCltor->devAddr[0] < 0xFF)
			{
				//ȡSS�����¸���ʱ��
//					if(pCltor_shadow->father_update > pCltor_shadow->mother_update)
//						u32UpdataSlot = pCltor_shadow->father_update;
//					else
//						u32UpdataSlot = pCltor_shadow->mother_update;
                u32UpdataSlot = pCltor_shadow->father_update;
				//ȡ���ϻ�ʱ�����ڰ����һ���������ڵ�SS
				if((Before(u32UpdataSlot + MaxTickSlot / 2, CurSlot))
					&& (Before(CurSlot, u32UpdataSlot + MaxTickSlot)))
				{
					if(u32UpdataSlot > u32LastFoundSlot) 
					{
						//ȡ��ʱ��������һ�δ�ģ����μ����ȵ�SS
						if (u32UpdataSlot < u32CurFoundSlot)
						{
							u32CurFoundSlot = u32UpdataSlot;
							u8FoundID = Loop;
							bFound = 1;
						}
					}
					else 
					{		
						//��û�з��ִμ����ȵ�SSʱ����Ҫ��������ȵ�SS
						if(0 == bFound)
						{
							//��������ȵ�SS
							if(u32UpdataSlot < u32MinSlot)
							{
								u32MinSlot = u32UpdataSlot;
								u8MinSlotID = Loop;
								bHaveMinSS =1;	
							}
						}
					}		
				}
			}
		}

		
		if(bFound)
			//ֱ���ҵ��μ����ȵ�SS ʱ����¼ʱ����
			u32LastFoundSlot = u32CurFoundSlot; 
		else
		{
			//û���ҵ��μ����ȵ�SSʱ���鿴�Ƿ�������ȵ�SS
			//���������SS������¼�ϴ��ϻ�ʱ����
			if(bHaveMinSS)
			{
				u32LastFoundSlot = u32MinSlot;
				u8FoundID = u8MinSlotID;
				bFound = 1;										
			}
			else
				//��û���ҵ���Ҫ������SSʱ����ɾ����¼���ϴμ��
				u32LastFoundSlot = 0;
		}

		if(bFound)
		{
			//����������	
		//	pkt = &(txdopktq.rxpkt[txdopktq.tail]);//Ҫ���Ӧ���(�������β��)
			pkt = &(ezPkt->TxPkt);
			
		//	joinresp = (JOINRSP*)(pkt->data); //����ַ(��joinresp�������ڶ�������PAYLOAD����)
			spkt = (STAREQUEST*)(pkt->data);//����ַ
			
			spkt->ttl = MAX_TIME_LIVE;
			spkt->route.rtype = MASTER_ROUTE_PATH; 
			spkt->route.pkt_type=1;
			spkt->route.downlink = ROUTE_DOWN_LINK;
			spkt->seq = cltor_shadow[u8FoundID].sendseq;
		     
		    	//*********************************************************
			spkt->destid = u8FoundID;//Ŀ�Ľڵ��
			
#ifdef _Debug
            Test[0] = 0xFF;		
            Test[1] = u8FoundID;
            UART_PutArray(UART1, Test, 2); // for test
#endif
			
			ROUTE temprt = Fill_Route(pkt->data + CALLRSP_ROUTE_OFFSET,  MASTER_ROUTE_PATH, u8FoundID); //�����ߵ�ַ��·��

			if (temprt.rinfo.rtype != ERROR_ROUTE_PATH) 
			{
//				spkt->route.rdepth = temprt.rinfo.rdepth;
//				recvid = temprt.did;
				
//				pkt->len = CALLRSP_ROUTE_OFFSET + (spkt->route.rdepth); 
				//*********************************************************�����
//					pkt->pkthead.ctrl.bits.msgtype = MSG_TYPE_CALL_REQUEST;	//������_������
//					
//				//	pkt->pkthead.ctrl.bits.seq = cltor[u8FoundID].sendseq;//������_���к�
//	                                pkt->pkthead.ctrl.bits.DataFlag = 0;
//	                                pkt->pkthead.ctrl.bits.UpFlag = 0;
//					pkt->pkthead.cid.all = (netcid << 4) | (localhop);//�����ʶ
//					pkt->pkthead.sid = localid;//����ID     
//	
//					pkt->pkthead.did = recvid;//������ID  
				
				ezPkt->bValid = 1;//���籨�Ļ�����Ч
				ezPkt->nBackOffSlot = 0;
				cltor_shadow[u8FoundID].sendseq++;
			}
			bSendTickToSS = 1;
//				u32SendTickToSSDeadTime = curslottime + MAX_TIME_LIVE * 2 + 2400;	//��ʱ1����
		}
	}										
}

uint8 CheckConflict(void)
{
    uint8 ret = 0;
    double rssi = -130;//SX1276ReadRssi();
    static double x = 90.0;
    LOG_DEBUG( DBGFMT"rssi[%4.1f]\n",DBGARG, rssi);
    g_rfLR_rssi_lmit = -rfpara.rf_limit;
    if((g_rfLR_rssi_lmit) < RFLR_RSSI_VALUE_LIMT_MIN)
    {
        g_rfLR_rssi_lmit = RFLR_RSSI_VALUE_LIMT_MIN;
    }
    else if((g_rfLR_rssi_lmit) > RFLR_RSSI_VALUE_LIMT)
    {
        g_rfLR_rssi_lmit = RFLR_RSSI_VALUE_LIMT;
    }
    double baseRssi = (double)((x * g_rfLR_rssi_lmit) / 100);
    if(rssi > baseRssi)
    {
        LOG_DEBUG( DBGFMT"Type[%d] rssi[%4.1f] baseRssi[%4.1f] rssi_lmit[%4.1f] \n",DBGARG, 
            g_stSendCacheIndex.level, rssi, baseRssi, rfpara.rf_limit);

        ret = 1;
    }
    return ret;
}


void wireless_send()
{
    uint8 msr = getMSR();
    if ((msr & EZMAC_STATE_BIT) == EZMAC_PRO_IDLE)    //����״̬                            
    {                                       	
		//Switch_Channel(behave.freq); //��ҵ���ŵ�����ͨ�� 
		// �ڿ����㲥ʱ϶�ĵط������ܹ�����ҵ������
		//if(slot < ((rfpara.rf_slotnum * 2) - 3))
		{
			if(checkSendCache() )
			{	

                if(CheckConflict())
                {
                    goto recv;
                }
                else
                {
                    EZMacPRO_Transmit();//����RTS	
                }
			}
			else
            {
				goto recv;
            }
		}
        return ;
recv: 
		{
			//�ɼ����ϻ������еĵ���������				
			//EZMacPro_PacketType = PACKET_TYPE_2STEP;
			EZMacPRO_Receive(); //�������ݰ�
			if (BackOffSlot > 0)
			    BackOffSlot--;						
		}										
        
    }

    
}
//extern tRadioDriver *Radio;
extern uint8 g_ucUpgradeFlgForPush;
extern uint32 g_ucUpgradeCount;

//=======================================================
void wireless_mng(void)//״̬������
{
    switch( Radio->Process( ) )
    {
    case RF_RX_TIMEOUT:

        break;
    case RF_RX_DONE:
    {
        PKT *pkt;
        uint8 tail;
        SYS_Dev_OptBlinkSet(LED_FAR_RX, 0, 0, 0, 0);
        
        if(g_ucUpgradeFlgForPush != 0xAA)
        {
            //uint16 frameLen = 0;
            tail = rxdopktq.tail;
            pkt = &(rxdopktq.rxpkt[tail]); //�������ݰ���ַ                        
            //LOG_DEBUG( DBGFMT"======================RF_RX_DONE ========================= = \n",DBGARG);    

            pkt->len = Radio->GetRxPacket( pkt->data);//, ( uint16* )&pkt->len );

            uint8_t channel = PST_CHN_WL;
            uint8 protocal;   
            if(gs_FRM.Channel(channel, pkt->data, pkt->len, &protocal) != PST_ERR_OK)
            {
                LOG_DEBUG( DBGFMT"rssi[%4.1f] crc check err %02x%02x%02x%02x%02x%02x\n",DBGARG, SX1276LoRaGetPacketRssi(),
                    pkt->data[18], pkt->data[17],pkt->data[16],pkt->data[15],pkt->data[14],pkt->data[13]);     

                LOG_DUMPHEX(LOG_LEVEL_DEBUG, "err data : \n", pkt->data, pkt->len);
                
                SYS_Dev_OptBlinkSet(LED_FAR_RX, 1, 5, 2, 0);
                SYS_Dev_OptBlinkSet(LED_FAR_TX, 1, 5, 2, 0);        
                break;
            }

            //frm.chninfo = channel & PST_FRM_CHNNO;  //ͨ����
            pkt->protocol = protocal;
    
            rxdopktq.rxpkt[rxdopktq.tail].sentoffset = 0; // ���offsetֵ
            
            //�����յ��ĳ����������
            rxdopktq.tail = (rxdopktq.tail + 1) & QUEUE_LEN_MASK;
            rxdopktq.len = (rxdopktq.len + 1);  
            uint8_t msg = MSG_FARP_RECVDATA;
            extern kbuf_queue_t gs_RFMngQueue;
            krhino_buf_queue_send(&gs_RFMngQueue, &msg, 1);
        }
        Radio->StartRx( 0);
                //MSR = RX_STATE_BIT | RX_STATE_WAIT_FOR_SEND_ACK;
        break;
    }
                
    case RF_TX_DONE:
        SYS_Dev_OptBlinkSet(LED_FAR_TX, 0, 0, 0, 0);

        Radio->StartRx( 0);
        if(NULL != g_stSendCacheIndex.ezPkt)
        {
            uint16 temp8 = 0;
            uint8 errCode = 0;
            g_stSendCacheIndex.ezPkt->bValid = 0;
            g_stSendCacheIndex.ezPkt->nBackOffSlot = 0;
            temp8 = g_stSendCacheIndex.ezPkt->TxPkt.index;
            
            if(g_stSendCacheIndex.level == CON_SEND_PRIORITY_NORMAL)
            {
//	                temp8 = g_stSendCacheIndex.ezPk->TxPkt.index;
                if(cltor_shadow[temp8].nodestatus.bDataAck != 0)
                {
                    cltor_shadow[temp8].nodestatus.bDataAck = 0;
                }
//	                g_stSendCacheIndex.ezPk->bValid = 0;
//	                g_stSendCacheIndex.ezPk->nBackOffSlot = 0;
                errCode = cltor_shadow[temp8].nodestatus.errCode;
                
//	                g_ucPktRssiValue[EzDataTxPkt.TxPkt.apdu.seq] = 0;
            }
            else if(g_stSendCacheIndex.level == CON_SEND_PRIORITY_HIGH)
            {

                cltor_shadow[temp8].nodestatus.bNetAck = 0;//�Է��յ�ACK    
                
                errCode = cltor_shadow[temp8].nodestatus.result;
                //cltor[EzNetTxPkt.TxPkt.apdu.index].nod.needack = 0;//�Է��յ�ACK    
                
//	                g_ucPktRssiValue[EzNetTxPkt.TxPkt.apdu.seq] = 0;
            }
//	            else
//	            {
//	                EzHHUTxPkt.bValid = 0;
//	                EzHHUTxPkt.nBackOffSlot = 0;
//	                temp8 = 0;
//	            }//	            if(EzCurTxType == EZ_TX_TYPE_DATA)
//	            {
//	                temp8 = EzDataTxPkt.TxPkt.index;
//	                if(cltor[temp8].nodestatus.bDataAck != 0)
//	                {
//	                    cltor[temp8].nodestatus.bDataAck = 0;
//	                }
//	                EzDataTxPkt.bValid = 0;
//	                EzDataTxPkt.nBackOffSlot = 0;
//	                errCode = cltor[temp8].nodestatus.errCode;
//	                
//	//	                g_ucPktRssiValue[EzDataTxPkt.TxPkt.apdu.seq] = 0;
//	            }
//	            else if(EzCurTxType == EZ_TX_TYPE_NET)
//	            {
//	                EzNetTxPkt.bValid = 0;
//	                EzNetTxPkt.nBackOffSlot = 0;
//	                temp8 = EzNetTxPkt.TxPkt.index;
//	                cltor[temp8].nodestatus.bNetAck = 0;//�Է��յ�ACK    
//	                
//	                errCode = cltor[temp8].nodestatus.result;
//	                //cltor[EzNetTxPkt.TxPkt.apdu.index].nod.needack = 0;//�Է��յ�ACK    
//	                
//	//	                g_ucPktRssiValue[EzNetTxPkt.TxPkt.apdu.seq] = 0;
//	            }
//	            else
//	            {
//	                EzHHUTxPkt.bValid = 0;
//	                EzHHUTxPkt.nBackOffSlot = 0;
//	                temp8 = 0;
//	            }
            
            if ((temp8 != 0) && (cltor[temp8].devAddr[0] < 0xFF)
                && (cltor[temp8].nodestatus.NetStatus == NODE_STATUS_OUT
                && errCode == 0)
                /*&& (cltor[temp8].nod.ans_pkt_type == 0)*/)
            {
                cltor[temp8].nodestatus.NetStatus = NODE_STATUS_LOGIN;
            }
        }
        break;
    case RF_TX_TIMEOUT:
        
//	        if(EzCurTxType == EZ_TX_TYPE_DATA)//1���
//	        {
//	            EzDataTxPkt.nBackOffSlot = 40;
//	        }
//	        else
//	        {
//	            EzNetTxPkt.nBackOffSlot = 40;
//	        }

        g_stSendCacheIndex.ezPkt->nBackOffSlot = FUNC_DELAY_MS(1000);
        //Radio->StartRx( );
        break;
    case RF_IDLE:
        Radio->StartRx( 0);
        break;
    case RF_RX_CRCERR:
        SYS_Dev_OptBlinkSet(LED_FAR_RX, 1, 5, 2, 0);
        break;
    default:
        break;
    }

}
//=======================================================
void wireless_proc(void)//
{
    wireless_mng();
}

void Handle_Led()
{
#ifdef MASTER_NODE
	uint16 i,m = 0 ;
    uint8_t flag = 0;
	for(i=MAX_SUP_SS_NUM;i >= SUP_SS_INDEX_START; i--)//����
	{
		if ( cltor[i].devAddr[0] < 0xFF/* && cltor[i].nodestatus.NetStatus*/)
        {
            if(cltor[i].login_count > 0)
            {
                cltor[i].login_count--;
                if((flag & (1 << cltor[i].loginNo)) == 0)
                {
                    flag |= 1 << cltor[i].loginNo;
        			switch(cltor[i].loginNo)	
                    {
                        case 1:
                            if(cltor_shadow[i].nodestatus.switchstt)
                            {
    //                            SYS_Dev_OptBlinkSet(GPIO_LED_SUB1_NORM, 2, 0, 0, 0);
                                SYS_Dev_OptBlinkSet(GPIO_LED_SUB1_ERR, 2, 0, 0, 0);
                            }
                            else
                            {
    //                            SYS_Dev_OptBlinkSet(GPIO_LED_SUB1_NORM, 3, 0, 0, 0);
                                SYS_Dev_OptBlinkSet(GPIO_LED_SUB1_ERR, 1, 10, 10, 0);
                            }
                            break;
                        case 2:
                            if(cltor_shadow[i].nodestatus.switchstt)
                            {
    //                            SYS_Dev_OptBlinkSet(GPIO_LED_SUB2_NORM, 2, 0, 0, 0);
                                SYS_Dev_OptBlinkSet(GPIO_LED_SUB2_ERR, 2, 0, 0, 0);
                            }
                            else
                            {
    //                            SYS_Dev_OptBlinkSet(GPIO_LED_SUB2_NORM, 3, 0, 0, 0);
                                SYS_Dev_OptBlinkSet(GPIO_LED_SUB2_ERR, 1, 10, 10, 0);
                            }
                            break;
                        default:
                            break;
                
                    }     
                }
            }
            else
            {
                switch(cltor[i].loginNo)    
                {
                    case 1:
                        SYS_Dev_OptBlinkSet(GPIO_LED_SUB1_ERR, 0xff, 0, 0, 0);

                        break;
                    case 2:
                        
                        SYS_Dev_OptBlinkSet(GPIO_LED_SUB2_ERR, 0xff, 0, 0, 0);

                        break;
                    default:
                        break;
            
                }     

            }
        }
    }
#endif    
}

void Handle_Proc()
{
    static uint32_t count = 0;
    count++;
    Handle_JRep();//����������
    if(count % 2 == 0)
    {
        Handle_Affirm();
    }
    Handle_Led();
}

