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
#include "protocol.h"
#include "pst_RF.h"

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
  
    ES_OK();
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
            I2cWrite(0xA0, &addrLen, FM_DEVICE_ADDR_LEN, 1);
            
			I2cWrite(0xA0, frm->recv+4, FM_DEVICE_ADDR , 6);
			I2cRead(0xA0, nDeviceMacAddr, FM_DEVICE_ADDR, 6); 
        }        
        frm->send[4]=0;

        frm->len=5;
        //return ES_ERR_NOREPLY;              //����ҪӦ��
    }
    else
    {
        frm->send[4]=1;
        frm->len=5;
    }
  
    ES_OK();
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

            Radio->Init(order, rfpara.rf_channel, rfpara.rf_power);
            //SX1276LoRaInit( );
            Radio->StartRx( );
            frm->send[4]=0;

        }

//	         gsPara.CH = frm->recv[4];
//	         if(ParamSave_CH()==ES_ERR_OK)
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
  
    ES_OK();
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
//	            if(ParamSave_PL()==ES_ERR_OK)
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
                setRfPwr(rfpara.rf_power);
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
    ES_OK();
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
  
    ES_OK();
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

    ES_OK();
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
    ES_OK();
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

    ES_OK();
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
        EZ_TX_PKT * ezPkt = getIdelSendCacheByPri(CON_SEND_PRIORITY_NORMAL);

    	if(NULL != ezPkt)
		//if(0 == EzDataTxPkt.bValid)
		{
			PKT* pTxPkt 	= 	&(ezPkt->TxPkt);
			
			pTxPkt->ttl = MAX_DATA_TIME_LIVE;//(uint8)recvuppkt.deadtime;
                    
            memset((uint8 *)&pTxPkt->head.apdu, 0, sizeof(STAPDU));
			pTxPkt->head.apdu.ctrl.dir = 0;
            pTxPkt->head.apdu.ctrl.prm = 1;
            pTxPkt->head.apdu.ctrl.evtmode = 1;
            pTxPkt->head.apdu.ctrl.ftd = 1;
            pTxPkt->head.apdu.seq = cltor_shadow[0].sendseq;
            pTxPkt->head.apdu.fn = 49;//����Ӧ
            pTxPkt->head.apdu.stInfo.stDown.bit1.routeFlg = 0;//pkt->rarea->stDown.bit1.routeFlg;
            pTxPkt->head.apdu.stInfo.stDown.bit1.nodeFlg = 0;//pkt->rarea->stDown.bit1.nodeFlg;
            pTxPkt->head.apdu.stInfo.stDown.bit1.addrFlg = 0;
            pTxPkt->head.apdu.stInfo.stDown.bit1.conflict = 1;//pkt->rarea->stDown.bit1.conflict;
            pTxPkt->head.apdu.stInfo.stDown.bit1.routeNum = 0;//pkt->rarea->stDown.bit1.routeNum;
            pTxPkt->head.apdu.stInfo.stDown.bit2.channelFlg = 0;//pkt->rarea->stDown.bit2.channelFlg;
            pTxPkt->head.apdu.stInfo.stDown.bit2.errEncodeFlg = 0;//pkt->rarea->stDown.bit2.errEncodeFlg & 0x03;
            pTxPkt->head.apdu.addrlen = 12;//pkt->afnoffset -  pkt->AddrOffSet;                        
            pTxPkt->head.apdu.addr = pTxPkt->data + PKT_HEAD_LEN;
            extern uint8 nDeviceMacAddr[6];
            extern const uint8 sBroadAddrFD[6];
            memcpy(pTxPkt->head.apdu.addr, nDeviceMacAddr, UNIQUE_MAC_ADDR_LEN);//�ڵ�Ψһ��ʶ(����)	
            memcpy(pTxPkt->head.apdu.addr + 6, sBroadAddrFD, UNIQUE_MAC_ADDR_LEN);//�ڵ�Ψһ��ʶ(����)	

    		
            pTxPkt->head.apdu.len = 0;


            //recvuppkt.isneedack = 1; // ��Ҫ ack

            ezPkt->bValid = 1;//ҵ�񻺴���Ч
            ezPkt->nBackOffSlot = 0;   //���Ϸ���
            cltor_shadow[0].sendseq++; //ÿ����һ����1
            //negapkt.ttl = 0xff;

        }
    }
    SA_NOREPLY();
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
               
                Radio->Init(order, rfpara.rf_channel, rfpara.rf_power);
                Radio->StartRx( );
                
                frm->send[4]=0;
                frm->len=5;        

                ES_OK();
            }
//            else
//            {
//                frm->send[4]=1;
//                SA_ERR();
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
    ES_OK();
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
        frm->send[5] = ES_LOG_OCStt(); 
        frm->send[6] = ES_LOG_LevelStt();
        frm->len=7;
    }
    else if(frm->len > 4)
    {
        
        if(frm->recv[4]<9)
        {
            uint8 en = frm->recv[4];
            uint8 level = frm->recv[5];
            if(ES_ERR_OK == ES_LOG_OC(en, level))
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
    ES_OK();
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
 * @return: uint8  ES_ERR_OK/ES_ERR_FT
 * @˵��: 
 * @����: xugl (2013/7/16)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 PST_RF_Decode(uint8* receive, uint8* send, uint16 sbuflen, PST_Frame* frame)
{
    RF_Frame* frm = &frame->_rf;
    
    frm->len = ByteToHInt(receive + 1 ); //����
    frm->FrameType = receive[3];        //����������
    frm->FrameID = receive[4];          //������
    frm->cmd = ByteToHInt(receive + 5 ); //������
    if(frm->cmd > 0x6100)               //Сд��ĸ�Ĵ�д
    {
      frm->cmd-=0x2020;
    }
    
    MoveBuffer(receive,send,7);         //ΪӦ������׼��
    frm->recv = receive + 3;            //���յ�������
    frm->send = send + 3;               //�������ݻ������ʼ��ַ
    
    if((frm->FrameType != 0x08) &&
       (frm->FrameID != 0x01))
    {
        return ES_ERR_FT;
    }   
    
     ES_OK();                            //������ͨ��
}



/************************************************************************
 * @function: PST_RF_Proc
 * @����: 
 * 
 * @����: 
 * @param: frm ���ݽ����ɵĽṹ��
 * 
 * @����: 
 * @return: uint8  ES_ERR_OK/ES_ERR_FT
 * @˵��: 
 * @����: xugl (2014/5/27)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 PST_RF_Proc(PST_Frame* frame)
{
    RF_Frame* frm = &frame->_rf;
    uint16 index;
    uint16 length;
    const CMD_TABLE_t* tbl;
    uint8 err;
    
    index = ADVMidWaySearch(
        (uint8*)stRFCMDTbl,
        sizeof(stRFCMDTbl)/sizeof(CMD_TABLE_t),
        sizeof(CMD_TABLE_t),
        frm->cmd
        );
    
    if(index == 0xFFFF)                 //û���������򷵻����ݱ�ʶ����
    {
        frm->len = 5;
        frm->send[4]=1;                 //����ʧ��
        err = ES_ERR_DI;                //��Ч����ID  
    }
    else
    {
    
        tbl = stRFCMDTbl + index;
        if(tbl->func == __NULL)
        {           
            frm->len = 5;
            frm->send[4]=1;             //����ʧ��
            err = ES_ERR_FT;           //������ݱ�ʶ��֧�ֶ�,ֱ�ӻظ�����֡
        }
        else
        {                               //ִ�ж�����
            err = ((FW_COMMAD_CONF_t)tbl->func)(tbl, frm);
        }
        
    } 

    if(err == ES_ERR_NOREPLY)               //����Ҫ�ظ�����֡,ֱ���˳�
    {        
        return ES_ERR_NOREPLY;
    }

    frm->send-=3;
    PST_RF_Pack(frame, err, &length);     //�������֡

    length+=1;
    gfs_PSTChnSend[frame->chninfo & PST_FRM_CHNNO](frm->send, length);

    ES_OK();
}


/************************************************************************
 * @function: PST_RF_Pack
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
uint8 PST_RF_Pack(PST_Frame* frame, uint8 err, uint16* length)
{
    RF_Frame* frm = &frame->_rf;
    
    frm->send[1] = frm->len>>8;         //����
    frm->send[2] = frm->len ;           //����
    frm->send[3] = frm->FrameType + 0x80;//������
                                        //�������
    *length = PST_DoneFrame(PST_FRM_CONF_NO, frm->send);

    if(*length > 0)                     //�������>0
    {
        ES_OK();
    }
    return ES_ERR_FT;                  //���ط�����������
}

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
    if(value == TRUE)
    {
        send[7] = 0;
    }
    else
    {
        send[7] = 1;
    }
    length = PST_DoneFrame(PST_FRM_CONF_NO, send);
    gfs_PSTChnSend[PST_CHN_SET](send, length);
}
