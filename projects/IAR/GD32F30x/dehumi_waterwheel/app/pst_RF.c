/****************************************Copyright (c)**************************************************
**                                  ________�Ƽ����޹�˾
**                                          ������
**
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: pst_RF.c
**��   ��   ��: LGY
**����޸�����: 2015��11��23��
**��        ��: ΢��������RFģ��___��Լ�����ļ���������ͨѶ�ͱ���ͨѶ
**ע        ��:
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
**������: LGY
**��  ��: v1.0
**�ա���: 2015��11��23��
**�衡��: ԭʼ�汾
********************************************************************************************************/
#define EXT_PST_RF
#include <k_api.h>
#include <stdio.h>
#include <stdlib.h>
#include "public.h"
#include "pst_RF.h"
#include "frm.h"
#include "user_func.h"
#include "wirelessSendCache.h"
#include "LinkMng.h"
#include "paradef.h"
#define PKT_HEAD_LEN (1+2+1+1+sizeof(STINFOWIRE)+2)
#define RSSI_OFFSET_LF                              -164.0
#define RSSI_OFFSET_HF                              -157.0
#define RSSI_OFFSET_MF                              10

uint8 PST_SRF_Pack(PST_Frame* frame, uint8 err, uint16* length);
/*************************************************************************
 * Function Name: FillRouteInfo
 * Parameters:  
 * Return:  
 * Description:
 *
 *************************************************************************/\
//uint16 Select_Route(uint16 Sid, uint16* pData, uint8* pLen)
//{
//	uint8 rc = 0;
//	COLLECTOR* pCilor = &cltor[Sid];
//	uint8 bMother  = 0;
//	uint8 Loop = 0;
//	uint8 Error = 0;;
//	if((*pLen) <= 12)
//	{
//		if((*pLen) > 1)
//		{
//			for(Loop = 0; Loop < (*pLen) - 1; Loop++)
//			{
//				if(Sid == pData[Loop])
//				{
//					Error = 1;
//
//#ifdef _Debug
//                    memset(Test, 0X11, sizeof(Test));
//                    UART_PutArray(UART1, Test, 10); // for test		
//#endif
//					break;
//				}
//			}
//		}
//
//		if(0 == Error)
//		{
//			if(pCilor->father != 0)
//			{
//				if(pCilor->father == localid)
//					rc = 1;
//				else
//				{
//					pData[*pLen] = pCilor->father;
//					(*pLen) ++;			
//					rc = Select_Route(pCilor->father, pData, pLen);
//					if(0 == rc)	
//					{
//						(*pLen) --;
//						bMother = 1;
//					}
//				}
//			}
//			else
//				bMother = 1;
//
//			 if(bMother)
//			 {
//				if(pCilor->mother != 0)
//				{
//					if(pCilor->mother == localid)
//						rc = 1;		
//					else
//					{
//						pData[*pLen] = pCilor->mother;
//						(*pLen) ++;			
//						rc = Select_Route(pCilor->mother, pData, pLen);
//						if(0 == rc)
//							(*pLen)--;
//					}	
//				}
//			 }
//		}
//	}
//    else 
//      Error = 1;
//	return rc;
//}
//
//
////------------------------------------------------------------------------
////	ROUTE Fill_Route(uint8 *data, uint8 type, uint8 id)
////	{
////	    uint8 temp;
////	    ROUTE route;
////	    uint8 Path[20];
////	    // Ĭ������
////	    route.rinfo.downlink = ROUTE_DOWN_LINK;
////	    route.rinfo.rtype = type;
////	    route.rinfo.rdepth = 0;
////	    route.did = id;
////	
////	
////		uint8 Len = 0;        
////		if(Select_Route(id, Path, &Len))
////		{
////			if(Len > 0)
////				route.did = Path[Len - 1];
////			else
////				route.did = id;
////			route.rinfo.rdepth = Len;
////			route.rinfo.rtype = MASTER_ROUTE_PATH;
////	                
////			for(temp = 0; temp < Len; temp++)
////			//	data[temp] = Path[Len - temp - 1];
////				data[temp] = Path[temp];
////			
////			//TEST
////	#ifdef _Debug
////	        memset(Test, 0, sizeof(Test));
////	        Test[0] = 0x55;
////	        Test[1] = id;
////	        for(uint8 Loop = 0; Loop < Len; Loop++)
////	        {
////	        	Test[Loop + 2] =  data[Loop];
////	        }
////	        UART_PutArray(UART1, Test, Len + 2); // for test		
////	#endif
////		}
////		else
////	    {
////			route.rinfo.rtype = ERROR_ROUTE_PATH;
////			
////	#ifdef _Debug
////	        Test[0] = 0x56;
////	        UART_PutArray(UART1, Test, 1); // for test
////	#endif
////	    }
////	
////	
////	    return route;
////	}
//ROUTE Fill_Route(uint8 *data, uint8 type, uint16 id)
//{
//    uint8 temp;
//    ROUTE route;
//    uint16 Path[20];
//    // Ĭ������
//    route.rinfo.downlink = ROUTE_DOWN_LINK;
//    route.rinfo.rtype = type;
//    route.rinfo.rdepth = 0;
//    route.did = id;
//
//    memset((uint8 *)&Path, 0, 40);
//	uint8 Len = 0;        
//	if(Select_Route(id, Path, &Len))
//	{
//		if(Len > 0)
//			route.did = Path[Len - 1];
//		else
//			route.did = id;
//		route.rinfo.rdepth = Len;
//		route.rinfo.rtype = MASTER_ROUTE_PATH;
//                
////			for(temp = 0; temp < Len; temp++)
////			//	data[temp] = Path[Len - temp - 1];
////				data[temp] = Path[temp];
//        for(temp = 0; temp < Len; temp++)
//        {
//            //  data[temp] = Path[Len - temp - 1];
//            //data[temp] = Path[temp];
//            memcpy(data + temp * 6, cltor[Path[Len - temp - 1]].devAddr, 6);
//        }
//	}
//	else
//    {
//		route.rinfo.rtype = ERROR_ROUTE_PATH;
//		
//    }
//
//
//    return route;
//}

//#pragma location= FLASH_MAP_ADD
__no_init HASHT *htable1;//[MAX_HASH_TABLE_FLASH_SIZE]; //��ϣ��(32K)

//#pragma location= FLASH_MAP_ADD2
__no_init HASHT *htable2;//[MAX_HASH_TABLE_FLASH_SIZE]; //��ϣ��(32K)

#include "star_rf_func.c"
//	/********************************************************** 
//	//������ԭ��
//	**********************************************************/
//	typedef struct _RF_CMD_TABLE_ {
//		uint16          cmd; 		//!< ������
//		void*			func;	    //!< ��������
//	} RF_CMD_TABLE_t;
//	
//	typedef uint8 (*FW_COMMAD_t)(const RF_CMD_TABLE_t* tbl, RF_Frame* frm);

/************************************************************************
 * @function: fRFCMD_FR
 * @����: ��HXRF��Ƶ�ʻ�׼��
 * 
 * @����: 
 * @param: frm 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: lgy (2015/12/16)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 fRFCMD_FR(const CMD_TABLE_t* tbl, RF_Frame* frm)
{   
    if(frm->len == 4)
    {
        frm->send[4]=0;
//	        
//	        ParaFR = HxDevRF_FR();
//	        IntToHByte(frm->send+5,ParaFR);  //���ֽ���ǰ
        frm->len=7;
    }
    else
    {
        frm->send[4]=1;
        frm->len=5;
    }
  
    SYS_OK();
}

/************************************************************************
 * @function: fRFCMD_ID
 * @����: �޸Ļ��HXRF�ı��ַ
 * 
 * @����: 
 * @param: frm 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: lgy (2015/12/16)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 fRFCMD_ID(const CMD_TABLE_t* tbl, RF_Frame* frm)
{         
    if(frm->len == 4)
    {
        frm->send[4]=0;
        MoveBuffer(nDeviceMacAddr, frm->send+5, 6); 
        InvBuffer(frm->send+5,6);
        frm->len=11;
    }
    else if(frm->len > 9)
    {
        InvBuffer(frm->recv+4,6);
        if(CmpBuffer(frm->recv+4, nDeviceMacAddr, 6)!=0)
        {
            MoveBuffer(frm->recv+4,nDeviceMacAddr,6); 
            uint8 addrLen = 6;
//            I2cWrite(0xA0, &addrLen, FM_DEVICE_ADDR_LEN, 1);
//            
//			I2cWrite(0xA0, frm->recv+4, FM_DEVICE_ADDR , 6);
//			I2cRead(0xA0, nDeviceMacAddr, FM_DEVICE_ADDR, 6); 
        }        
        frm->send[4]=0;

        frm->len=5;
        //return SYS_ERR_NOREPLY;              //����ҪӦ��
    }
    else
    {
        frm->send[4]=1;
        frm->len=5;
    }
  
    SYS_OK();
}
/************************************************************************
 * @function: fRFCMD_CH
 * @����: �޸Ļ��HXRF��ͨ��
 * 
 * @����: 
 * @param: frm 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: lgy (2015/12/16)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 fRFCMD_CH(const CMD_TABLE_t* tbl, RF_Frame* frm)
{         
    if(frm->len == 4)
    {
        frm->send[4] = 0;
        I2cRead(0xA0, &frm->send[5], FM_NETPARA_ADDR, 1);

        frm->len=6;
    }
    else if(frm->len > 4)
    {
        if(frm->recv[4] > 0 && frm->recv[4] < getChannelNum())
        {
            I2cWrite(0xA0, &frm->recv[4], FM_NETPARA_ADDR, 1);
            rfpara.rf_channel = frm->recv[4];
            uint8 tmp[4];
            I2cRead(0xA0, &tmp[0], FM_FREQ_DEFINE+8, 4); //������Ƶ�㶨�����***
            int32 order = tmp[0]*0x1000000 + tmp[1]*0x10000 + tmp[2]*0x100 + tmp[3];

//            Radio->Init(order, rfpara.rf_channel, rfpara.rf_power);
            //SX1276LoRaInit( );
//            Radio->StartRx( );
            frm->send[4]=0;

        }

//	         gsPara.CH = frm->recv[4];
//	         if(ParamSave_CH()==SYS_ERR_OK)
//	         {
//	           frm->send[4]=0;
//	           ClrFlag(HRS_LORA_CONF,gl_SysRunStt); //��������ͨ�� 
//	           ClrFlag(HRS_RF_FINDCH,gl_SysRunStt); //����ͨ�� Ĭ������ȷͨ��
//	         }
        else
        {
            frm->send[4]=1;
        }
        frm->len=5;
    }
    else
    {
        frm->send[4]=1;
        frm->len=5;
    }
  
    SYS_OK();
}
/************************************************************************
 * @function: fRFCMD_PL
 * @����: �޸Ļ��HXRF��Ƶ�ʻ�׼��
 * 
 * @����: 
 * @param: frm 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: lgy (2015/12/16)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 fRFCMD_PL(const CMD_TABLE_t* tbl, RF_Frame* frm)
{         
    if(frm->len == 4)
    {
        frm->send[4] = 0;
        frm->send[5] = rfpara.rf_power;       
        frm->len=6;
    }
    else if(frm->len > 4)
    {
        frm->send[4]=1;
        if(frm->recv[4]<8)
        {
//	            gsPara.PL = frm->recv[4];
//	            if(ParamSave_PL()==SYS_ERR_OK)
//	            {
//	                frm->send[4]=0;
//	                ClrFlag(HRS_LORA_CONF,gl_SysRunStt); //��������RF
//	            }

            uint8 rf_power = frm->recv[4];
            if(rf_power != rfpara.rf_power && rf_power < getPwrLevelNum())
            {
                rfpara.rf_power = rf_power;
                I2cWrite(0xA0, (uint8*)&rfpara.rf_power, FM_NETPARA_ADDR+1, 1); //���������***	
                //LoRaSettings.Power = powerArray[rfpara.rf_power];
                //SX1276LoRaSetRFPower( LoRaSettings.Power );
//                setRfPwr(rfpara.rf_power);
            }

            frm->send[4]=0;
        }        
        frm->len=5;
    }
    else
    {
        frm->send[4]=1;
        frm->len=5;
    }
    SYS_OK();
}
/************************************************************************
 * @function: fRFCMD_SV
 * @����: ��HXRF�Ķ�ȡ�汾��
 * 
 * @����: 
 * @param: frm 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: lgy (2015/12/16)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 fRFCMD_SV(const CMD_TABLE_t* tbl, RF_Frame* frm)
{         
    uint8 m = 0;
    
    if(frm->len == 4)
    {
        uint8 * buffer = frm->send+5;
        frm->send[4] = 0;

        memcpy(buffer + m, (uint8 *)ucModeVer, 20);

        buffer[m + 8] = (sVersiong.swCode[0] / 0x10) + '0';
        buffer[m + 9] = (sVersiong.swCode[0] % 0x10) + '0';

        buffer[m + 11] = (sVersiong.yy / 0x10) + '0';
        buffer[m + 12] = (sVersiong.yy % 0x10) + '0';
        buffer[m + 13] = (sVersiong.mm / 0x10) + '0';
        buffer[m + 14] = (sVersiong.mm % 0x10) + '0';
        buffer[m + 15] = (sVersiong.dd / 0x10) + '0';
        buffer[m + 16] = (sVersiong.dd % 0x10) + '0';

        m+=20;
        //MoveBuffer((uint8*)gc_HxNkVer,frm->send+5,sizeof(gc_HxNkVer));          
        frm->len=m+5;
    }    
    else
    {
        frm->send[4]=1;
        frm->len=5;
    }
  
    SYS_OK();
}
/************************************************************************
 * @function: fRFCMD_RS
 * @����: ���յ��������ź�ǿ��
 * 
 * @����: 
 * @param: frm 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: lgy (2015/12/16)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 fRFCMD_RS(const CMD_TABLE_t* tbl, RF_Frame* frm)
{         

    SYS_OK();
}
/************************************************************************
 * @function: fRFCMD_RS
 * @����: ����ģʽ
 * 
 * @����: 
 * @param: frm 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: lgy (2015/12/16)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 fRFCMD_FT(const CMD_TABLE_t* tbl, RF_Frame* frm)
{         

    g_ucfactory_mod = 0x55;
    frm->send[4] = 0;
    frm->len=5;
    SYS_OK();
}
/************************************************************************
 * @function: fRFCMD_RS
 * @����: ���յ��������ź�ǿ��
 * 
 * @����: 
 * @param: frm 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: lgy (2015/12/16)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 fRFCMD_HT(const CMD_TABLE_t* tbl, RF_Frame* frm)
{       
    uint8 temp = 0;
    frm->send[4] = 0;

    
    uint8 ret = FlashTest(256);
    temp |= ret << HD_TEST_FLASH;
    ret = EEpromTest(256);
    temp |= ret << HD_TEST_EEPROM;

    if(!(SPI_Check()))
    {
        temp |= 1 << HD_TEST_RF;
    }
    //buffer[m++] = temp;
    frm->send[5] = temp;       
    frm->len=6;

    SYS_OK();
}
/************************************************************************
 * @function: fRFCMD_RS
 * @����: ���յ��������ź�ǿ��
 * 
 * @����: 
 * @param: frm 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: lgy (2015/12/16)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 fRFCMD_CT(const CMD_TABLE_t* tbl, RF_Frame* frm)
{         
    extern COLLECTOR_SHADOW cltor_shadow[MAX_SUP_SS_NUM + 1];//�ӽڵ���Ϣ����            
    if(g_ucfactory_mod == 0x55)
    {
		recvuppkt.deadtime = MAX_DATA_TIME_LIVE;
		StaAck = PKT_TYPE_HD_TEST;	
//        EZ_TX_PKT * ezPkt = getIdelSendCacheByPri(CON_SEND_PRIORITY_NORMAL);
//
//    	if(NULL != ezPkt)
//		//if(0 == EzDataTxPkt.bValid)
//		{
//			PKT* pTxPkt 	= 	&(ezPkt->TxPkt);
//			
//			pTxPkt->ttl = MAX_DATA_TIME_LIVE;//(uint8)recvuppkt.deadtime;
//                    
//            memset((uint8 *)&pTxPkt->head.apdu, 0, sizeof(STAPDU));
//			pTxPkt->head.apdu.ctrl.dir = 0;
//            pTxPkt->head.apdu.ctrl.prm = 1;
//            pTxPkt->head.apdu.ctrl.evtmode = 1;
//            pTxPkt->head.apdu.ctrl.ftd = 1;
//            pTxPkt->head.apdu.seq = cltor_shadow[0].sendseq;
//            pTxPkt->head.apdu.fn = 49;//����Ӧ
//            pTxPkt->head.apdu.stInfo.stDown.bit1.routeFlg = 0;//pkt->rarea->stDown.bit1.routeFlg;
//            pTxPkt->head.apdu.stInfo.stDown.bit1.nodeFlg = 0;//pkt->rarea->stDown.bit1.nodeFlg;
//            pTxPkt->head.apdu.stInfo.stDown.bit1.addrFlg = 0;
//            pTxPkt->head.apdu.stInfo.stDown.bit1.conflict = 1;//pkt->rarea->stDown.bit1.conflict;
//            pTxPkt->head.apdu.stInfo.stDown.bit1.routeNum = 0;//pkt->rarea->stDown.bit1.routeNum;
//            pTxPkt->head.apdu.stInfo.stDown.bit2.channelFlg = 0;//pkt->rarea->stDown.bit2.channelFlg;
//            pTxPkt->head.apdu.stInfo.stDown.bit2.errEncodeFlg = 0;//pkt->rarea->stDown.bit2.errEncodeFlg & 0x03;
//            pTxPkt->head.apdu.addrlen = 12;//pkt->afnoffset -  pkt->AddrOffSet;                        
//            pTxPkt->head.apdu.addr = pTxPkt->data + PKT_HEAD_LEN;
//            extern uint8 nDeviceMacAddr[6];
//            extern const uint8 sBroadAddrFD[6];
//            memcpy(pTxPkt->head.apdu.addr, nDeviceMacAddr, UNIQUE_MAC_ADDR_LEN);//�ڵ�Ψһ��ʶ(����)	
//            memcpy(pTxPkt->head.apdu.addr + 6, sBroadAddrFD, UNIQUE_MAC_ADDR_LEN);//�ڵ�Ψһ��ʶ(����)	
//
//    		
//            pTxPkt->head.apdu.len = 0;
//
//
//            //recvuppkt.isneedack = 1; // ��Ҫ ack
//
//            ezPkt->bValid = 1;//ҵ�񻺴���Ч
//            ezPkt->nBackOffSlot = 0;   //���Ϸ���
//            cltor_shadow[0].sendseq++; //ÿ����һ����1
//            //negapkt.ttl = 0xff;
//
//        }
    }
    SYS_NOREPLY();
}
/************************************************************************
 * @function: fRFCMD_RS
 * @����: ���յ��������ź�ǿ��
 * 
 * @����: 
 * @param: frm 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: lgy (2015/12/16)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 fRFCMD_NS(const CMD_TABLE_t* tbl, RF_Frame* frm)
{         
    if(frm->len == 4)
    {
        frm->send[4] = 0;
        //frm->send[5] = gsPara.PL; 
        I2cRead(0xA0, &frm->send[5], FM_FREQ_DEFINE+8, 4); //������Ƶ�㶨�����***	
        
        frm->len=9;
    }
    else if(frm->len > 4)
    {
        uint8 tmp[4];
        //uint8 orderFreq = 0;
        uint32 oldOrder = 0, newOrder = 0;
        I2cRead(0xA0, &tmp[0], FM_FREQ_DEFINE+8, 4); //������Ƶ�㶨�����***	
        oldOrder = tmp[0]*0x1000000 + tmp[1]*0x10000 + tmp[2]*0x100 + tmp[3];
        newOrder = frm->recv[4]*0x1000000 + frm->recv[5]*0x10000 + frm->recv[6]*0x100 + frm->recv[7];
        //orderFreq = frm->recv[6] & 0xF0;
        if(oldOrder != newOrder)
        {

//	#if (SX127X_FREQ == FREQ_433M)
//	            if(orderFreq == 0x40)
//	#elif  (SX127X_FREQ == FREQ_915M)
//	            if(orderFreq == 0x90)
//	#elif  (SX127X_FREQ == FREQ_470M)
//	            if(orderFreq == 0x70)
//	#elif  (SX127X_FREQ == FREQ_868M)
//	            if(orderFreq == 0x80)                            
//	#endif
            {
                I2cWrite(0xA0, &frm->recv[4], FM_FREQ_DEFINE+8, 4); //������Ƶ�㶨�����***	
                //SX1276LoRaInit( );
                uint8 tmp[4];
                I2cRead(0xA0, &tmp[0], FM_FREQ_DEFINE+8, 4); //������Ƶ�㶨�����***
                int32 order = tmp[0]*0x1000000 + tmp[1]*0x10000 + tmp[2]*0x100 + tmp[3];
               
//                Radio->Init(order, rfpara.rf_channel, rfpara.rf_power);
//                Radio->StartRx( );
                
                frm->send[4]=0;
                frm->len=5;        

                SYS_OK();
            }
//            else
//            {
//                frm->send[4]=1;
//                SYS_ERR();
//            }        
        }
        else
        {
            frm->send[4]=0;
            frm->len=5;        
        }

    }
    else
    {
        frm->send[4]=1;
        frm->len=5;
    }
    SYS_OK();
}
/************************************************************************
 * @function: fRFCMD_LG
 * @����: ��־����
 * 
 * @����: 
 * @param: frm 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: lgy (2015/12/16)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 fRFCMD_LG(const CMD_TABLE_t* tbl, RF_Frame* frm)
{         
    if(frm->len == 4)
    {
        frm->send[4] = 0;
        frm->send[5] = SYS_LOG_OCStt(); 
        frm->send[6] = SYS_LOG_LevelStt();
        frm->len=7;
    }
    else if(frm->len > 4)
    {
        
        if(frm->recv[4]<9)
        {
            uint8 en = frm->recv[4];
            uint8 level = frm->recv[5];
            if(SYS_ERR_OK == SYS_LOG_OC(en, level))
            {
                frm->send[4]=0;
            }
            else
            {
                frm->send[4]=1;
            }
        }        
        frm->len=5;
    }
    else
    {
        frm->send[4]=1;
        frm->len=5;
    }
    SYS_OK();
}
/**********************************************************
����Ӳ������������. ��������Ŵ�С���У���Ϊʹ����ֵ�����
**********************************************************/
const CMD_TABLE_t stRFCMDTbl[] = {
    { RF_CMD_CH,		(void*)fRFCMD_CH,},
    { RF_CMD_CT,		(void*)fRFCMD_CT,},    
	{ RF_CMD_FR,		(void*)fRFCMD_FR,},
    { RF_CMD_FT,		(void*)fRFCMD_FT,},     
    { RF_CMD_HT,		(void*)fRFCMD_HT,},
	{ RF_CMD_ID,		(void*)fRFCMD_ID,},  
    { RF_CMD_LG,		(void*)fRFCMD_LG,},
    { RF_CMD_NS,		(void*)fRFCMD_NS,},      
    { RF_CMD_PL,		(void*)fRFCMD_PL,},
    { RF_CMD_RS,		(void*)fRFCMD_RS,},
    { RF_CMD_SV,		(void*)fRFCMD_SV,},
    { NULL,			NULL,	},
};
/************************************************************************
 * @function: PST_RF_Decode
 * @����: �жϵ�ǰ���������Ƿ�Ϊ�Ϸ�֡,����ǵĻ��������Ϊ����ͨѶ�ṹ��
 * 
 * @����: 
 * @param: receive ���յ����ݵĻ���
 * @param: send ��Ҫ���͵����ݵĻ���
 * @param: sbuflen ���ͻ��泤��
 * @param: frame ���ݽ����ɵĽṹ��
 * 
 * @����: 
 * @return: uint8  SYS_ERR_OK/SYS_ERR_FT
 * @˵��: 
 * @����: xugl (2013/7/16)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//uint8 PST_RF_Decode(uint8* receive, uint8* send, uint16 sbuflen, PST_Frame* frame)
//{
//    RF_Frame* frm = &frame->_rf;
//    
//    frm->len = ByteToHInt(receive + 1 ); //����
//    frm->FrameType = receive[3];        //����������
//    frm->FrameID = receive[4];          //������
//    frm->cmd = ByteToHInt(receive + 5 ); //������
//    if(frm->cmd > 0x6100)               //Сд��ĸ�Ĵ�д
//    {
//      frm->cmd-=0x2020;
//    }
//    
//    MoveBuffer(receive,send,7);         //ΪӦ������׼��
//    frm->recv = receive + 3;            //���յ�������
//    frm->send = send + 3;               //�������ݻ������ʼ��ַ
//    
//    if((frm->FrameType != 0x08) &&
//       (frm->FrameID != 0x01))
//    {
//        return SYS_ERR_FT;
//    }   
//    
//     SYS_OK();                            //������ͨ��
//}
//
//
//
///************************************************************************
// * @function: PST_RF_Proc
// * @����: 
// * 
// * @����: 
// * @param: frm ���ݽ����ɵĽṹ��
// * 
// * @����: 
// * @return: uint8  SYS_ERR_OK/SYS_ERR_FT
// * @˵��: 
// * @����: xugl (2014/5/27)
// *-----------------------------------------------------------------------
// * @�޸���: 
// ************************************************************************/
//uint8 PST_RF_Proc(PST_Frame* frame)
//{
//    RF_Frame* frm = &frame->_rf;
//    uint16 index;
//    uint16 length;
//    const CMD_TABLE_t* tbl;
//    uint8 err;
//    
//    index = ADVMidWaySearch(
//        (uint8*)stRFCMDTbl,
//        sizeof(stRFCMDTbl)/sizeof(CMD_TABLE_t),
//        sizeof(CMD_TABLE_t),
//        frm->cmd
//        );
//    
//    if(index == 0xFFFF)                 //û���������򷵻����ݱ�ʶ����
//    {
//        frm->len = 5;
//        frm->send[4]=1;                 //����ʧ��
//        err = SYS_ERR_DI;                //��Ч����ID  
//    }
//    else
//    {
//    
//        tbl = stRFCMDTbl + index;
//        if(tbl->func == __NULL)
//        {           
//            frm->len = 5;
//            frm->send[4]=1;             //����ʧ��
//            err = SYS_ERR_FT;           //������ݱ�ʶ��֧�ֶ�,ֱ�ӻظ�����֡
//        }
//        else
//        {                               //ִ�ж�����
//            err = ((FW_COMMAD_CONF_t)tbl->func)(tbl, frm);
//        }
//        
//    } 
//
//    if(err == SYS_ERR_NOREPLY)               //����Ҫ�ظ�����֡,ֱ���˳�
//    {        
//        return SYS_ERR_NOREPLY;
//    }
//
//    frm->send-=3;
//    PST_RF_Pack(frame, err, &length);     //�������֡
//
//    length+=1;
//    gfs_PSTChnSend[frame->chninfo & PST_FRM_CHNNO](frm->send, length);
//
//    SYS_OK();
//}
//
//
///************************************************************************
// * @function: PST_RF_Pack
// * @����: ����Դ֡���д��,����ͨѶ
// * 
// * @����: 
// * @param: frame ����Ľṹ��
// * @param: err 
// * @param: length ����Ϊ����������󳤶�,���Ϊ�����֮��ĳ���
// * 
// * @����: 
// * @return: uint8  
// * @˵��: 
// * @����: lgy (2015/11/23)
// *-----------------------------------------------------------------------
// * @�޸���: 
// ************************************************************************/
//uint8 PST_RF_Pack(PST_Frame* frame, uint8 err, uint16* length)
//{
//    RF_Frame* frm = &frame->_rf;
//    
//    frm->send[1] = frm->len>>8;         //����
//    frm->send[2] = frm->len ;           //����
//    frm->send[3] = frm->FrameType + 0x80;//������
//                                        //�������
////    *length = PST_DoneFrame(PST_FRM_CONF_NO, frm->send);
//
//    if(*length > 0)                     //�������>0
//    {
//        SYS_OK();
//    }
//    return SYS_ERR_FT;                  //���ط�����������
//}

/************************************************************************
 * @function: PstRF_Reply
 * @����: RFЭ��Ӧ����
 * 
 * @����: 
 * @param: frm 
 * 
 * @����: 
 * @return: void  
 * @˵��: 
 * @����: lgy (2015/12/16)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void PstRF_Reply(uint16 cmd, uint8 value)
{
    uint8 send[16];
    uint16 length;
    
    send[1] = 0;         //����
    send[2] = 5 ;        //����
    send[3] = 0x88;      //������
    send[4] = 0x01;    
    send[5] = cmd>>8;  
    send[6] = cmd ;
//    if(value == TRUE)
//    {
//        send[7] = 0;
//    }
//    else
//    {
//        send[7] = 1;
//    }
//    length = PST_DoneFrame(PST_FRM_CONF_NO, send);
    gfs_PSTChnSend[PST_CHN_SET](send, length);
}
uint16 getIdWithAddr(uint8 * addr)
{
    uint16 i = 0;
    for(i = 0; i < MAX_SUP_SS_NUM+1; i++)
    {
        if(memcmp(cltor[i].devAddr, addr, 6) == 0 )
        {
            break;
        }

    }
    return i;
}

/************************************************************************
 * @function: PST_SRF_Decode
 * @����: �жϵ�ǰ���������Ƿ�Ϊ�Ϸ�֡,����ǵĻ��������Ϊ����ͨѶ�ṹ��
 * 
 * @����: 
 * @param: receive ���յ����ݵĻ���
 * @param: send ��Ҫ���͵����ݵĻ���
 * @param: sbuflen ���ͻ��泤��
 * @param: frame ���ݽ����ɵĽṹ��
 * 
 * @����: 
 * @return: uint8  SYS_ERR_OK/SYS_ERR_FT
 * @˵��: 
 * @����: yzy (2013/7/16)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 PST_SRF_Decode(uint8* receive, uint8* send, uint16 sbuflen, PST_Frame* frame)
{
    SRF_Frame* frm = &frame->_srf;
    //GRF_R_Frame *rf ;    
    uint16 m = 0;//uiTemp
    uint8 addrLen = CON_DEV_ADDR_LEN_6;
    uint8 destAddr[CON_DEV_ADDR_LEN_6];
    uint8 srcAddr[CON_DEV_ADDR_LEN_6];
    memset(destAddr, 0, CON_DEV_ADDR_LEN_6);
    memset(srcAddr, 0, CON_DEV_ADDR_LEN_6);
    uint16 frameLen = receive[1] + receive[2] * 0x100;
    
    memset((uint8 *)&frm->apdu,0,sizeof(STAPDU));
    
    frm->send = send;
    frm->recv = receive;
    frm->len = frameLen;
    frm->apdu.ctrl = *(CTRL_BIT *)(&receive[m]);
    frm->bNeedReAllocate = FALSE;
    frm->bDebugFlg = FALSE;
    frm->id = 0;
    frm->rssi = 0;
    
    m++;
    
    m+=2;
    
    frm->apdu.seq = receive[m++];
    frm->apdu.fn = receive[m++];
    
    memcpy(&frm->apdu.stInfo, receive+m, sizeof(STINFOWIRE));
    m+=sizeof(STINFOWIRE);
    memcpy(&frm->apdu.pandId, receive+m, 2);
    m+=2;
    if(frm->apdu.stInfo.stUp.bit2.channelFlg != (rfpara.rf_channel & 0x0F) ||
        frm->apdu.stInfo.stUp.bit1.routeNum != frm->apdu.stInfo.stUp.bit4.relayNum)
    {
        LOG_DEBUG( DBGFMT"rssi[%4.1f] CH[%d] myCh[%d] route[%d] relay[%d] dest channel err %02x%02x%02x%02x%02x%02x\n",DBGARG, SX1276LoRaGetPacketRssi(),
            frm->apdu.stInfo.stUp.bit2.channelFlg, rfpara.rf_channel, frm->apdu.stInfo.stUp.bit1.routeNum, frm->apdu.stInfo.stUp.bit4.relayNum, 
            receive[m+5], receive[m+4],receive[m+3],receive[m+2],receive[m+1],receive[m]);                        
    
        SYS_ERR();
    }

    frm->apdu.routeNum = frm->apdu.stInfo.stUp.bit1.routeNum;//pkt->apdu.info[1] & 0x0f;
    
    if(frm->apdu.stInfo.stUp.bit1.addrFlg == 1)
    {
        addrLen = 2;
    }
    else
    {
        addrLen = 6;

    }

    frm->apdu.addr = receive + m;
    frm->apdu.addrlen = (frm->apdu.routeNum+2) * addrLen;
    memcpy(destAddr, receive + m + (frm->apdu.routeNum+1) * addrLen  ,addrLen);

    if(addrLen != 6)
    {
       frm->id = frm->apdu.addr[1] + frm->apdu.addr[0] * 0x100;
       //�̵�ַģʽ��Ҫ���Ŀǰû���ⷽ����ƣ�����ȫ�ǿ��ǳ���ַ�����
        if(frm->id > MAX_SUP_SS_NUM)
        {//������
            frm->id = 0;
        }
       
       memcpy(srcAddr, cltor[frm->id].devAddr, CON_DEV_ADDR_LEN_6);
    }
    else
    {
       frm->id = getIdWithAddr(&frm->apdu.addr[0]);
        if(frm->id > MAX_SUP_SS_NUM)
        {//������
            frm->id = 0;
        }
        memcpy(srcAddr, frm->apdu.addr, CON_DEV_ADDR_LEN_6);
    }
    
    //frm->apdu.snr = SX1276LR->RegPktSnrValue;
    //frm->apdu.rssi = SX1276LR->RegPktRssiValue;
//    getRegSnrRssi(&frm->apdu.snr, &frm->apdu.rssi);
    if(frm->apdu.ctrl.ftd > 9)
    {
        SYS_ERR();
    }

    if(frm->apdu.stInfo.stUp.bit1.addrFlg == 1)
    {
    
        if(frm->apdu.ctrl.dir == 0)
        {
            SYS_ERR();
        }
        
        if(memcmp(destAddr, (uint8 *)&rfpara.shortID, addrLen) != 0)
        {
            SYS_ERR();
        }
    }
    else
    {
        if(frm->apdu.ctrl.dir == 0)
        {
            if(memcmp(frm->apdu.addr, sBroadAddrFE, addrLen) == 0)
            {
                if(memcmp(destAddr, nDeviceMacAddr, addrLen) == 0)
                {
                    frm->apdu.broadCastFlg = 0;
                }
                else if(memcmp(destAddr/*frm->apdu.addr*/, sBroadAddr99, addrLen) == 0)
                {
                    frm->apdu.broadCastFlg = 1;                        
                }
                else
                {
                    SYS_ERR();
                }
                LOG_DEBUG( DBGFMT"rssi[%4.1f]HHU comm\n",DBGARG, SX1276LoRaGetPacketRssi());
            }
            else if(memcmp(destAddr, sBroadAddr99, addrLen) == 0 || 
                    memcmp(destAddr, sBroadAddrFF, addrLen) == 0)
            {
                frm->apdu.broadCastFlg = 1;
//	                if(guc_AllowLogin)
                {
                    
                }
                
            }
            else if(memcmp(destAddr, nDeviceMacAddr, addrLen) == 0 ||
                    memcmp(destAddr, nParentMacAddr, addrLen) == 0 ||
                memcmp(destAddr, nParentMacAddrTemp, addrLen) == 0)
            {
                frm->apdu.broadCastFlg = 0;
            }
            else
            {
                LOG_DEBUG( DBGFMT"rssi[%4.1f]down data src %02x%02x%02x%02x%02x%02x dest %02x%02x%02x%02x%02x%02x\n",DBGARG, SX1276LoRaGetPacketRssi(),
                frm->apdu.addr[5], frm->apdu.addr[4],frm->apdu.addr[3],frm->apdu.addr[2],frm->apdu.addr[1],frm->apdu.addr[0],
                destAddr[5], destAddr[4],destAddr[3],destAddr[2],destAddr[1],destAddr[0]);                        
                SYS_ERR();
            }
        }
        else
        {
            if(memcmp(destAddr, nDeviceMacAddr, addrLen) != 0 )
            {
                if(memcmp(destAddr, sBroadAddr99, addrLen) != 0 && 
                    memcmp(destAddr, sBroadAddrAA, addrLen) != 0 &&
                      memcmp(destAddr, sBroadAddrFE, addrLen) != 0)
                {
                    LOG_DEBUG( DBGFMT"rssi[%4.1f]dest addr err %02x%02x%02x%02x%02x%02x\n",DBGARG, SX1276LoRaGetPacketRssi(),
                        destAddr[5], destAddr[4],destAddr[3],destAddr[2],destAddr[1],destAddr[0]);                        

                    SYS_ERR();
                }
                else
                {
                    frm->apdu.broadCastFlg = 1;
                }
            }
            else
            {
                frm->apdu.broadCastFlg = 0;
            }
        }
    }
    

    m+=frm->apdu.addrlen;
    frm->apdu.len = frameLen - (PKT_HEAD_LEN + 1 + frm->apdu.addrlen);
    frm->apdu.data = receive + m;

    //frm->apdu.snr = SX1276LR->RegPktSnrValue;
    //frm->apdu.rssi = SX1276LR->RegPktRssiValue;
//    getRegSnrRssi(&frm->apdu.snr, &frm->apdu.rssi);
//    frm->rssi = (int)calculateRssi(frm->apdu.snr, frm->apdu.rssi);

    
    LOG_DEBUG( DBGFMT"CH[%d] rf recv ftd[%d] fn[%d] rssi[%4.1f] %02x%02x%02x%02x%02x%02x\n",DBGARG, 
        rfpara.rf_channel, frm->apdu.ctrl.ftd, frm->apdu.fn, frm->rssi, 
        frm->apdu.addr[5], frm->apdu.addr[4],frm->apdu.addr[3],frm->apdu.addr[2],frm->apdu.addr[1],frm->apdu.addr[0]);
    LOG_DUMPHEX(LOG_LEVEL_DEBUG, "HD : \n", receive, frameLen);

    if(frameLen < 18)
    {
        LOG_DEBUG( DBGFMT"frame addr %02x%02x%02x%02x%02x%02x wrang data\n",DBGARG, 
            frm->apdu.addr[5], frm->apdu.addr[4],frm->apdu.addr[3],frm->apdu.addr[2],frm->apdu.addr[1],frm->apdu.addr[0]);
        SYS_ERR();
    }
    //g_ucPktRssiValue[pkt->head.apdu.seq] = (uint8)(0 - rssi) ;

    
    //memcpy(SN, pkt->head.apdu.addr, 6);
    
    
//	    if(Meter_Check(srcAddr))
    extern uint8_t guc_AllowLogin;
#ifdef MASTER_NODE    
    if(guc_AllowLogin || Meter_Check(srcAddr))
#else
    if(guc_AllowLogin )
#endif      
    {
        frm->bNeedReAllocate = TRUE;
    }
    else
    {   
        if(memcmp(srcAddr, sBroadAddrFE, CON_DEV_ADDR_LEN_6) == 0)
        {
            if(frm->apdu.broadCastFlg == 0)
            {
                frm->bDebugFlg = TRUE;
            }
            LOG_DEBUG( DBGFMT" HHU read data\n",DBGARG);
        }
#ifndef MASTER_NODE
        else if(0 == memcmp(srcAddr, nParentMacAddr,CON_DEV_ADDR_LEN_6))
        {
        
        }
#endif        
        
        else
        {
            LOG_DEBUG( DBGFMT" addr does not at my white space %02x%02x%02x%02x%02x%02x\n",DBGARG, 
                frm->apdu.addr[5], frm->apdu.addr[4],frm->apdu.addr[3],frm->apdu.addr[2],frm->apdu.addr[1],frm->apdu.addr[0]);
        }
    }
    
    //memcpy((uint8 *)&pkt->stDownInfo, (uint8 *)&EzDataTxPkt.TxPkt.stDownInfo, 6);
    //pkt->downAfn = EzDataTxPkt.TxPkt.downAfn;
    
    
    SYS_OK();                            //������ͨ��
}


/**********************************************************
����Ӳ������������. ��������Ŵ�С���У���Ϊʹ����ֵ�����
**********************************************************/
const CMD_TABLE_t stSRFFTDTbl[] = {
    { 0x00,		(void*)fSRFFTD00,},     //ȷ��/����
    { 0x01,		(void*)fSRFFTD01,},     //APP�����֡�ṹ
    { 0x02,		(void*)fSRFFTD02,},     //����ת��
    { 0x03,		(void*)fSRFFTD03,},
    { 0x04,		(void*)fSRFFTD04,},     //�㲥ҵ��֡
    { 0x05,		(void*)fSRFFTD05,},
//    { 0x06,		(void*)fGRFFTD06,},
    { 0x07,		(void*)fSRFFTD07,},     //·������
//    { 0x08,		(void*)fGRFFTD08,},
    { 0x09,		(void*)fSRFFTD09,},     //�ļ�����֡
	{ NULL,			NULL,	},
};

/************************************************************************
 * @function: PST_SRF_Proc
 * @����: 
 * 
 * @����: 
 * @param: frm ���ݽ����ɵĽṹ��
 * 
 * @����: 
 * @return: uint8  SYS_ERR_OK/SYS_ERR_FT
 * @˵��: 
 * @����: yzy (2014/5/27)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 PST_SRF_Proc(PST_Frame* frame)
{
    SRF_Frame* frm = &frame->_srf;
    uint16 index;
    uint16 length;
    const CMD_TABLE_t* tbl;
    uint8 err;
    
    index = ADVMidWaySearch(
        (uint8*)stSRFFTDTbl,
        sizeof(stSRFFTDTbl)/sizeof(CMD_TABLE_t),
        sizeof(CMD_TABLE_t),
        frm->apdu.ctrl.ftd
        );
    
    if(index == 0xFFFF)                 //û���������򷵻����ݱ�ʶ����
    {
        frm->len = 5;
        frm->send[4]=1;                 //����ʧ��
        return SYS_ERR_DI;              //��Ч����ID  
    }
    else
    {
    
        tbl = stSRFFTDTbl + index;
        if(tbl->func == __NULL)
        {           
            frm->len = 5;
            frm->send[4]=1;             //����ʧ��
            err = SYS_ERR_FT;           //������ݱ�ʶ��֧�ֶ�,ֱ�ӻظ�����֡
        }
        else
        {                               //ִ�ж�����
            err = ((FW_COMMAD_t)tbl->func)(tbl, frm);
        }
        
    } 

    if(err == SYS_ERR_NOREPLY)               //����Ҫ�ظ�����֡,ֱ���˳�
    {        
        return SYS_ERR_NOREPLY;
    }

    //frm->send-=3;
    PST_SRF_Pack(frame, err, &length);     //�������֡

//	    length+=2;
//	    gfs_PSTChnSend[frame->chninfo & PST_FRM_CHNNO](frm->send, length);

    SYS_OK();
}

uint8 Star_Rf_Pack(ST_STAR_RF_PACK * pack, STFUNCPARA * stAddr, STFUNCPARA * stData, EZ_TX_PKT * etpkt)
{
	uint8 rc = SYS_ERR_FT;
    uint16 m = 0;
    PKT *pkt;
    //uint8 recvid , bNotSend = 0;
    //JOINRSP *joinresp;
    //EXTDATA* pExtData;
    //uint8 bTypeExtData = 0;
    //ROUTE temprt;
    //uint8 m = 0;
    //uint16 whiteMeterNum = 0;
    if(etpkt == NULL || etpkt->bValid)//���籨�Ļ�����Ч
    {
        return rc;
    }

    pkt = &(etpkt->TxPkt);

        //uint8* pTxData =  pkt->data;

    pkt->ttl = 0xff;  //��������by ben 29

    memset((uint8 *)&pkt->head.apdu, 0, sizeof(STAPDU));
    //��������

    pkt->data[m++] = *((uint8 *)&pack->ctrl_bit);
    m+=2;
    //SEQ
    pkt->data[m++] = pack->seq;
//fn
    pkt->data[m++] = pack->fn;

    if(pkt->head.apdu.ctrl.dir)
    {
        pkt->head.apdu.stInfo.stUp.bit1.routeFlg = 0;
        pkt->head.apdu.stInfo.stUp.bit1.nodeFlg = 0;
        pkt->head.apdu.stInfo.stUp.bit1.addrFlg = 0;
        pkt->head.apdu.stInfo.stUp.bit1.conflict = 1;
        pkt->head.apdu.stInfo.stUp.bit2.channelFlg = rfpara.rf_channel & 0x0F;//Cal_Hash_Value(nDeviceMacAddr) % LORA_CHANNEL_NUM;//pkt->rarea->stDown.bit2.channelFlg;
        pkt->head.apdu.stInfo.stUp.bit2.rssi = 0;
        pkt->head.apdu.stInfo.stUp.len = stData->len;
        pkt->head.apdu.stInfo.stUp.rssi = pack->rssi;//g_ucPktRssiValue[pEzTxPkt->TxPkt.head.apdu.seq];
        
    }
    else
    {
        pkt->head.apdu.stInfo.stDown.bit1.routeFlg = 0;
        pkt->head.apdu.stInfo.stDown.bit1.nodeFlg = 0;
        pkt->head.apdu.stInfo.stDown.bit1.addrFlg = 0;
        pkt->head.apdu.stInfo.stDown.bit1.conflict = 1;
        pkt->head.apdu.stInfo.stDown.bitNumAns = 128;
        
        pkt->head.apdu.stInfo.stDown.bit2.channelFlg = rfpara.rf_channel & 0x0F;//Cal_Hash_Value(nDeviceMacAddr) % LORA_CHANNEL_NUM;//pkt->rarea->stDown.bit2.channelFlg;
        pkt->head.apdu.stInfo.stDown.bit2.errEncodeFlg = 0;
        pkt->head.apdu.stInfo.stDown.rssi =  pack->rssi;//g_ucPktRssiValue[pEzTxPkt->TxPkt.head.apdu.seq];

    }

    
//	    pkt->head.apdu.index = 0;
//	    
//	    pkt->head.apdu.ctrl = *ctrl_bit;
//	//	        pkt->head.apdu.ctrl.prm = 0;
//	//	        pkt->head.apdu.ctrl.evtmode = 1;
//	//	        pkt->head.apdu.ctrl.ftd = ctrl_bit->ftd;
//	    pkt->head.apdu.seq = seq;
//	    pkt->head.apdu.srssi = rssi;
//	    
//	    pkt->head.apdu.fn = fn;//ȷ��
    pkt->head.apdu.addr = pkt->data + PKT_HEAD_LEN;

    memcpy(pkt->head.apdu.addr, nDeviceMacAddr, UNIQUE_MAC_ADDR_LEN);//�ڵ�Ψһ��ʶ(����)	
    pkt->head.apdu.addrlen  = UNIQUE_MAC_ADDR_LEN;
    
    if(pack->id >= SUP_SS_INDEX_START &&  pack->id < MAX_SUP_SS_NUM)
    {
        ROUTE temprt = Fill_Route(pkt->head.apdu.addr + UNIQUE_MAC_ADDR_LEN,  MASTER_ROUTE_PATH, pack->id); //�����ߵ�ַ��·��
		if(temprt.rinfo.rtype != ERROR_ROUTE_PATH)
        {
			pkt->head.apdu.stInfo.stDown.bit1.routeNum = temprt.rinfo.rdepth;
            
            memcpy(pkt->head.apdu.addr + (temprt.rinfo.rdepth + 1) * 6, cltor[pack->id].devAddr, UNIQUE_MAC_ADDR_LEN);//�ڵ�Ψһ��ʶ(����)
	
            pkt->head.apdu.addrlen = (temprt.rinfo.rdepth + 2) * 6;
	
            pkt->head.apdu.data = pkt->data + PKT_HEAD_LEN + pkt->head.apdu.addrlen;
        }      
    }
    else
    {
        memcpy(pkt->head.apdu.addr + UNIQUE_MAC_ADDR_LEN, stAddr->data, stAddr->len);
        pkt->head.apdu.addrlen  += stAddr->len;
    }
    //info
    memcpy(pkt->data + m, &pkt->head.apdu.stInfo, sizeof(STINFOWIRE));
    m+=sizeof(STINFOWIRE);
    //pandi
    memcpy(pkt->data + m, &rfpara.panid, 2);
    m+=2;
    
    memcpy(pkt->data + m, pkt->head.apdu.addr, pkt->head.apdu.addrlen);
    m+=pkt->head.apdu.addrlen;


    memcpy(pkt->data + m, stData->data, stData->len);
    m+=stData->len;
    pkt->len = m+1;
    pkt->data[1] = pkt->len & 0xFF;
    pkt->data[2] = pkt->len >> 8;
    pkt->data[m] = Check_SUM(pkt->data, m);
    m++;




//	    pkt->head.apdu.addrlen = stAddr->len+UNIQUE_MAC_ADDR_LEN;
//	    pkt->head.apdu.addr = pkt->data + PKT_HEAD_LEN;
//	
//	    memcpy(pkt->head.apdu.addr, nDeviceMacAddr, UNIQUE_MAC_ADDR_LEN);//�ڵ�Ψһ��ʶ(����)    
//	    
//	    memcpy(pkt->head.apdu.addr+6, stAddr->data, stAddr->len);//�ڵ�Ψһ��ʶ(����)    
//	
//	
//	    pkt->head.apdu.data = pkt->data + PKT_HEAD_LEN + pkt->head.apdu.addrlen;
//	
//	    memcpy(pkt->head.apdu.data, stData->data, stData->len);//�ڵ�Ψһ��ʶ(����)    
//	    pkt->head.apdu.len = stData->len;



    //*********************************************************�����
    pkt->protocol = PST_FRM_NO;
    pkt->index = pack->id;
    etpkt->bValid = 1;//ҵ�񻺴���Ч
    etpkt->nBackOffSlot = 0;
    rc = SYS_ERR_OK;

	return rc;	
}
//EZ_TX_PKT TxPktH[CON_SEND_PRIORITY_HIGH_NUM];//ҵ�������
//EZ_TX_PKT TxPktN[CON_SEND_PRIORITY_NORMAL_NUM];//@"AHB_RAM_MEMORY";//ҵ�������
//EZ_TX_PKT TxPktL[CON_SEND_PRIORITY_LOW_NUM];//@"AHB_RAM_MEMORY";//ҵ�������
//EZ_TX_PKT * getIdelSendCache(uint8 level)//�������ѱȲ������ȼ��͵Ķ�����仺��
//{
//    int i = 0;
//    switch(level)
//    {
//    case CON_SEND_PRIORITY_HIGH:
//        for(i=0; i < CON_SEND_PRIORITY_HIGH_NUM; i++)
//        {
//            if(TxPktH[i].bValid == 0)
//            {
//                return &TxPktH[i];
//            }
//        }
//    case CON_SEND_PRIORITY_NORMAL:
//        for(i=0; i < CON_SEND_PRIORITY_NORMAL_NUM; i++)
//        {
//            if(TxPktN[i].bValid == 0)
//            {
//                return &TxPktN[i];
//            }
//        }
//    case CON_SEND_PRIORITY_LOW:
//        for(i=0; i < CON_SEND_PRIORITY_LOW_NUM; i++)
//        {
//            if(TxPktL[i].bValid == 0)
//            {
//                return &TxPktL[i];
//            }
//        }
//        break;
//    }
//
//    return NULL;
//}
/************************************************************************
 * @function: PST_SRF_Pack
 * @����: ����Դ֡���д��,����ͨѶ
 * 
 * @����: 
 * @param: frame ����Ľṹ��
 * @param: err 
 * @param: length ����Ϊ����������󳤶�,���Ϊ�����֮��ĳ���
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: lgy (2015/11/23)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 PST_SRF_Pack(PST_Frame* frame, uint8 err, uint16* length)
{
    SRF_Frame* frm = &frame->_srf;
    ST_STAR_RF_PACK rf_pack;
    STFUNCPARA stAddr;
    STFUNCPARA stData;
    
    rf_pack.broadCastFlg = 0;
    rf_pack.ctrl_bit = frm->apdu.ctrl;
    rf_pack.fn = frm->apdu.fn;
    rf_pack.id = frm->id;
    rf_pack.rssi = (uint8)(0-frm->rssi);
    rf_pack.seq = frm->apdu.seq;

    stData.data = frm->send;
    stData.len = frm->len;

    stAddr.data = frm->apdu.addr;
    stAddr.len = CON_DEV_ADDR_LEN_6;

    return Star_Rf_Pack(&rf_pack, &stAddr, &stData, getIdelSendCache(CON_SEND_PRIORITY_NORMAL));//Get_Idle_Send_Pkt());
    
    //frm->send[0] = frm->FCD;         //
    //frm->send[1] = frm->Len ;          //����   
    //frm->send[2] = frm->Len>>8 ;       //����
    //frm->send[3] = frm->SN ;//
    //frm->send[4] = frm->FN;
                                        //�������
//	    *length = PST_DoneFrame(PST_FRM_WL_1_NO, frm->send);
//	
//	    if(*length > 0)                     //�������>0
//	    {
//	        SYS_OK();
//	    }
//	    return SYS_ERR_FT;                  //���ط�����������
}

/************************************************************************
 * @function: RfFrmProc
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
 * @����: yzy (2014/2/12)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 RfFrmProc(uint8_t channel, uint8 protocol, uint8* rbuffer, uint16 length, uint8* sbuffer, uint8 sbuflen)
{
    PST_Frame frm;                          //��Լ֡�ṹ��
//    uint8 protocal;                         //��Լ��
//    uint16 bak;
    //uint8 err;
    
    SYS_VAR_CHECK(channel >= PST_CHN_NO);    //�������
                                            //ͨ����Լ֡�ж�
    frm.chninfo = channel & PST_FRM_CHNNO;  //ͨ����
    frm.protocol = protocol;
    
                                           //��Լ֡����
    if(gs_FRM.Decode(protocol, rbuffer, sbuffer, sbuflen, &frm) == PST_ERR_OK)
    {
        gs_FRM.Proc(protocol, &frm);        //��Լ����
    }
    else
    {
        LOG_DEBUG( DBGFMT"Decode Failed proto[%x] \n",DBGARG, protocol);                        

        LOG_DUMPHEX(LOG_LEVEL_DEBUG, "data: \n", rbuffer, length);
    }
    
    SYS_OK();
}



/************************************************************************
**��������: uint8 GD_BuildLinkFrm(uint8 type, uint8* buffer)
**�� ��: ������¼֡,�˳�֡,����֡(AFN02)
**
**�� ��: type 1 ��¼֡ 2 �˳�֡ 3 ����֡
         buffer ���֡����
**�� ��: 
**
**�� ע: 
**-----------------------------------------------------------------------
**�� ��: 
**�� ��: 
*************************************************************************/
uint8 RF_BuildLinkFrm(uint8 type, uint16_t id, uint8* buffer)
{
    PST_Frame frame;
    SRF_Frame * _srf = &frame._srf;
    
    uint16 len;
    uint32 di;
    int m = 0;
    int len_pos = 0;
    TIME sectime;
    //uint8  time[16];
    //uint8  week;
//	    if(type == 0x08)
//	    {
//	        len = (13 << 2) + _VER_GDW_; 
//	    }
//	    else
//	    {
//	        len = (12 << 2) + _VER_GDW_;        //GDW376.1-2009
//	    }
    buffer[m++] = 0x68;
    memcpy(buffer+m, gs_PstPara.Addr, 6);
    m+=6;
    buffer[m++] = 0x68;
    buffer[m++] = type | 0x80;
    len_pos = m;
    m+=2;
    SYS_MCU_ReadBCDDateTime(&sectime);

//    
//    buffer[m++] = sectime.year;
//    buffer[m++] = sectime.month;
//    buffer[m++] = sectime.dmon;
//    buffer[m++] = sectime.hour;
//    buffer[m++] = sectime.min;
//    buffer[m++] = sectime.sec;
    buffer[m++] = 0x80 | 0x06;
    buffer[m++] = id;
    buffer[m++] = id >> 8;
    
    buffer[m++] = guc_pseq;

    if(id == 0xA619)
    {
        memcpy(buffer+m, gs_PstPara.device, 20);
        m+=20;
    
        memcpy(buffer+m, gs_PstPara.Addr, 6);
        m+=6;
    }
    buffer[len_pos] = (uint8_t)(m - (len_pos + 2));
    buffer[len_pos+1] = (uint8_t)((m - (len_pos + 2)) >> 8);
    
//	                                        //����֡
//    len = PST_RF_Pack(0, len);
    
    return (uint8)len;                  //����֡�ĳ���
}

uint8 PST_RX_Down_PKT(PKT *pkt) //�������߽�������
{

    RfFrmProc(PST_CHN_WL, pkt->protocol, pkt->data, pkt->len, senduppkt.data, (uint8)sizeof(senduppkt.data));

    return 0;
}

