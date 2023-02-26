

#include "protocol.h"
#include "../radio/api/sx1276-Api.h"
//#include "protocol.h"

extern uint8 bEZMACDebugType;
extern DEBUG_UPPKT* pEZMACRecvPacket;
//�ظ����Ա��ı�־
extern uint8 bDebugSend;
extern uint8 bSendMeterSearchSeq;

//�����㲥�����־
//extern uint8 bSendResetCmd;
extern EZMACPROREG EZMacProReg;
extern ADVER adpkt; // �㲥���ṹ��
//extern uint8 MeterDeadCount;
//�ɼ�����֤ʹ�ܲ���
//extern uint8 bSSValidateEnable;
//������ʾ����ʾ������Դ
extern uint8 u8TuopuDisplaySource;
//������ͬ��ʹ�ܲ���
extern uint8 bMeterInfoSynEnable;

extern uint8 bChannelSetManualMode;	//0Ϊ�Զ�ģʽ    1Ϊ�ֶ�ģʽ
//extern uint8 bJcDisallowFalg;

extern uint32 adv_updatetime;
extern uint8 adv_sublen;
extern uint8 adv_subtype;
//�㲥͸��ת��������
extern uint8 adv_Trans_pkt[16];
//extern uint8 nDeviceMacAddr[6];
extern uint16  gab;//Ƶ��ƫ��ֵ
extern uint8 bBroadStart;
extern uint8 bBroadStartTriger;
extern uint8 bHaveJCDataFlag;
extern uint8 nJCDataFlag[4];
extern uint32 u32BroadStartTrigerTime;
extern uint8 bCltorSearchStart;
extern uint8 bWaitForSendMeterAck;

extern uint8 bNeedDeleteMeter;
extern uint8 bNeedMeterDeleteAck;

//extern uint8 MeterDeadCount;
extern uint8 ReSendCount;

extern uint8 bSendNeedWait;
extern uint32 u32TxSlot;
//���ڵ����ɼ������в��ı�־
extern uint8 bSingleSearchSSSendMeter;
extern uint8 bSingleSearchSSSendMeterWaitAck;
extern uint32 u32TimeOfSingleSearchSSSendMeter;
extern uint8 bSingleSearchSSMeterSended;

//���ߵ��Ա��Ĵ����״̬��
extern uint8 EZMAC_Debug_Status;
extern uint8 EZMacStaAck; // ͳ�ư��Ļ�Ӧ
extern uint8 bEzMacSingleSearchSSFlag;
extern uint8 nEzMacSourceId;
extern uint8 netcid ;
extern uint8 localhop;
extern uint8 u8NeedSetSSID;
//�㲥��λʱ֡����
extern uint8 bNeedChangeChannel;
extern uint8 nChannelChangeNum;
extern uint8 nDefaultJumpChannel;
extern uint8 nBroadSendData;

extern uint32 EzMacDeadCont;
extern uint8 subtype_flag;//��Լ����
extern uint8 EzMacSubTypeFlag;
extern uint8 EzMacReqMeterID; //�ϲ������ID ��
extern uint8 ReqMeterID; //�ϲ������ID ��
extern uint8 	SSMapShadowSpace[MAX_VALIDATE_SS_NUM];

//extern uint32 updata_code_cache[64];//256byte����
extern uint8 bNeedOpenSSValidateEnable;
extern QUEUE rxdopktq;
extern EZ_Cur_Tx_Type EzCurTxType; 

//��һ�ε����汾
extern uint8 nBaseSSVersion;
//��������´���Ҫ�İ����
//extern uint16 u16NextNeedID;
extern uint16 updata_pkt_no; //�����ж��������������ż��
extern uint16 pkt_seq; //��˳���
//extern uint8 Write_Sucess;//дFLASH������ȷ��־λ

extern EZ_TEST ez_test;//���书�ʲ���
//�����Ƚ�����ɺ��־
extern uint8 EzTestRevDone;
extern LISN_TEST ls_test;//�����Ȳ���
extern uint32 RsResetTime;
extern uint8 bMesterDataWaitAck;
extern uint8 nMeterAckType;
extern uint8 bNeedAckMeter;
extern uint16 MeterID;
extern uint8 MeterEventSendStatus;	
extern DEBUG_UPPKT DebugSendPacket;

extern uint8 nDebugPacketRear;
extern uint8 nDebugPacketFront;
extern uint32 StaTxtime; // ͳ�ư��ķ���ʱ��
extern uint32 StaRxtime; // ���յ��ڵ㷴����ͳ�ư��Ľ��� ʱ��
extern uint32 MeterDataSendSlot;
extern uint8 NeedAckMeterAddr[6];
extern uint8 NeedAckSourceId;
//���ߵ����յ��ظ���־
extern uint8 bEZMacRecFlag;
extern uint8 nMeterEventSendID;
extern uint8 nMeterEventSendTime;
extern LPC_Uart_Config_t Uart0Config;
extern uint32 UartDeadCont;
extern const uint8 ucModeVer[];

uint8 rarea[] = 
{
        0x04, 0x00, 0x50, 0x00, 0x00, 0x00
}; //��Ϣ��

const uint8 FW_KEY[16] ={'C','o','p','y','r','i','g','h','t',' ','H','R','C','o','m','m'};

//uint8 venderpkt[] = //���̴���Ͱ汾��            
//{
//        0x68, 0x17, 0x0, 0x81, 0x00, 0x0, 0x10, 0x00, 0x0, 0x0, 0x03, 0x01, 0x00, 0x48, 0x52, 0x30, 0x31, 0x03, 0x10, 0x01, 0x01, 0xa5, 0x16
//};

uint8 venderpkt[] = //���̴���Ͱ汾��            
{
        0x68, 0x18, 0x0, 0x8a, 0x00, 0x0, 0x10, 0x00, 0x0, 0x0, 0x03, 0x01, 0x00, 0x48, 0x52, 0x30, 0x31, 0x03, 0x0, 0x10, 0x01, 0x01, 0xae, 0x16
};

uint8 statuspkt[] = //  �ز����ڵ�״̬�ֺ��ز�����     
{
        0x68, 0x13, 0x00, 0x81, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x03, 0x10, 0x00, 0x11, 0x01, 0x14, 0x80, 0x4A, 0x16
};
uint8 statuspktForDebug[] = //  �ز����ڵ�״̬�ֺ��ز�����     
{
        0x68, 0x13, 0x00, 0x81, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0xF0, 0x40, 0x02, 0x11, 0x01, 0x14, 0x80, 0x6B, 0x16
};

uint8 informpkt[] = //ȷ�ϻش�
{
        0x68, 0x13, 0x00, 0x8a, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0xFF, 0xFF, 0x05, 0x00, 0x9e, 0x16
};

uint8 informpktForDebug[] = //ȷ�ϻش�
{
        0x68, 0x13, 0x00, 0x8a, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0xF0, 0x02, 0x01, 0xFF, 0xFF, 0x05, 0x00, 0x90, 0x16
};

uint8 negativepkt[] = //�񶨻ش�
{
        0x68, 0x10, 0x00, 0x8a, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x9c, 0x16
};

uint8 negativepktForDebog[] = //�񶨻ش�
{
        0x68, 0x10, 0x00, 0x8a, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0xF0, 0x01, 0x01, 0x00, 0x8c, 0x16
};

uint8 powerpkt[] = //376.2������
{
        0x68, 0x2F, 0x00, 0x8a, 0x04, 0x00, 0x50, 0x00, 0x00, 0x00, 0x24, 0x10, 0x35, 0x00, 0x00, 0x00, 0x62, 0x03, 0x00, 0xA0, 0x00, 0x00, 0x02, 0x01, 0x00, 0x01, 0x12, 0x68, 0x24, 0x10, 0x35, 0x00, 0x00, 0x00, 0x68, 0x81, 0x06, 0x43, 0xC3, 0x49, 0x33, 0x33, 0x33, 0xA8, 0x16, 0xc8, 0x16
};

uint8 DebugMeterData[] = 
{
	0x68, 0x31, 0x00, 0xca, 0x04, 0x00, 0x50,0x00,0x00,0x00,0x19,0x21,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x02,0x00,0x3b,0x00,0x01,0x12,0x68,0x19,0x21,0x02,0x00,0x00,0x00,0x68,0x81,0x06,0x43,0xc3,0xaa,0x34,0x33,0x33,0xdd,0x16,0x80,0x16 
};


uint8 Code_Check_Sum(uint32 ulbase)
{

    uint8 tmpData[256];
    //ImageBody tmpImage;
    uint32 addr = ulbase;
    uint16 CRC_acc = 0x5555;
    uint16 crcSelf = 0;
    uint32 tmpLen = 256;
    uint32 ulen = 0;
    //uint32 i = 0;
    //BFLS_PreWrite();

    
//	    BFLS_Read((uint8 *)tmpData, addr, tmpLen);
//	    memcpy((uint8 *)&tmpImage,tmpData,32);
    ulen = upd_pkt.fileLen;//tmpImage.plen+0x20;
    if(ulen < 256) return SA_ERR_FT;
//	    CRC_acc = CRC_16BIT_Check2(CRC_acc, tmpData+32, 224);
          
    //addr+=tmpLen;
    while(addr < (ulbase+ulen))
    {
        if(addr + 256 < (ulbase+ulen))
        {
            tmpLen = 256;
        }
        else
        {
            tmpLen = (ulbase+ulen) - addr - 4;
            
        }
        memset(tmpData,0,256);
        
        //BFLS_Write((uint8 *)testData, addr, 256);
        BFLS_Read(tmpData, addr, tmpLen);
        
        CRC_acc = CRC_16BIT_Check2(CRC_acc, tmpData, tmpLen);

        addr+=tmpLen;
    }
    BFLS_Read((uint8 *)&crcSelf, addr, 2);
    if(CRC_acc == crcSelf)//У��ͨ��
        return SA_ERR_OK;
    else
        return SA_ERR_FT;

}

/*******************************************************************************
**�����������������Զ�������ı��,�Ӷ�����������ʱ������ͣ��
**�ڸ�λ֮��õ��ظ�
********************************************************************************/
const uint32 gul_UpdataMarkA_APP[4] = 
{
    0x12345678, 0x87654321, 0x11223344, 0x55667788
};


const uint32 gul_UpdataMarkB_APP[4] = 
{
    0,0,0,0
};
#define FLS_UPDATEMARK_ADDR      0          //����������������Flash�е�λ��

/***********************************************************
 * @function_name: ES_IAP_Mark
 * @function_file: iap_file.c
 * @����: ���û�������������         
 * @����: 
 * @param:   setorclr ���û������������� 
 * @param:   ID       ����ID
 * @param:  length   ���ݳ���
 * @param:  address  ���ݵ�ַ
 * @����: 
 * @return: true  д��ɹ�
           false  д��ʧ�� 
 * @����:  yzy (2010-11-11)
 *******************************************************/
uint8 ES_APP_IAP_Mark(bool setorclr, uint32 length, uint32 address)
{
    uint8 uc_data[28];   

    if(setorclr)                        //���ñ��
    {
                                        //����д����
        memcpy(uc_data, (void*)gul_UpdataMarkA_APP, sizeof(gul_UpdataMarkA_APP));
        uc_data[16] = 0;               //����ID
                                        //����
        LongToByte(uc_data + 18, length);
                                        //��ַ
        LongToByte(uc_data + 22, address);
                                        //����CRCУ��
        IntToByte(uc_data + 26, DoCrc16(uc_data, 26, 0x5555));
    }
    else                                //�����������ݾͿ�����
    {
        memcpy(uc_data, (void*)gul_UpdataMarkB_APP, sizeof(gul_UpdataMarkB_APP));
    }
    BFLS_PreWrite();
	return BFLS_Write((uint8 *)uc_data, FLS_UPDATEMARK_ADDR, 28);
}

/***********************************************************
 * @function_name: ES_APP_IAP_Check
 * @function_file: iap_file.c
 * @����: ����Flash����������ĺϷ���         
 * @����: 
 * @param:  startAddr  ��������λ��FLASH����ʼƫ��
 * @param�� length �������ݳ���
 * @����: 
 * @return: true  ���ݺϷ�,����ʼ���г�������
           false  ���ݷǷ�,�����ܽ��г�������
 * @����:  xugl (2010-11-11)
 *******************************************************/
bool ES_APP_IAP_CheckCRC(uint32 startAddr, uint32 length)
{
    uint8 uc_data[64];                  //����������ȡ������
    uint16 crc = 0x5555;                //CRCУ��ֵ
    uint16 cmp;                         //CRCУ��ֵ�ĶԱ�
    
    uint32 curAdress = startAddr;		//������ʼ��ַ
    
    if((length & 0x03) != 0)            //�жϳ��ȵĺϷ���
    {
        return false;                   //���ش���
    }
    
    //ES_FILE_Open();
    
    while(length > 0)                   //���ϵĶ�ȡ����Ȼ�����У��
    {
        if(length > 64)                 //����64�ֽڶ�64�ֽ�
        {
            BFLS_Read(uc_data, curAdress, 64);
            crc = DoCrc16(uc_data, 64, crc);
            length -= 64;
            curAdress += 64;
        }
        else                            //�����ȡʣ�����ݵĳ���
        {
            BFLS_Read(uc_data, curAdress, length);
                                        //����4���ֽ���CRCУ��
            crc = DoCrc16(uc_data, length - 4, crc);
                                        //��ȡCRCУ��ĶԱ�ֵ
            cmp = ByteToInt(uc_data + length - 2);
            length = 0;                 //�Ѿ���ȡ�����е�����
        }
    }
    
    //ES_FILE_Close();
    
    if(cmp != crc)                      //�ж�CRC��У��ֵ�Ƿ����
    {
        return false;
    }
    return true;                        //����OK        
}


/***********************************************************
 * @function_name: ES_APP_IAP_Check
 * @function_file: iap_file.c
 * @����: ����Flash����������ĺϷ���         
 * @����: 
 * @param:  db  ���ݿ�
 * @param�� len ����
 * @����: 
 * @return: true  ���ݺϷ�,����ʼ���г�������
           false  ���ݷǷ�,�����ܽ��г�������
 * @����:  xugl (2010-11-11)
 *******************************************************/
bool ES_APP_IAP_Check(uint32 length)
{
	return ES_APP_IAP_CheckCRC(FLS_PROGRAM_ADDR+FLS_START_ADDR, length);
}

/************************************************************************
 * @function: ES_IAP_Ready
 * @����: 
 * 
 * @����: 
 * @param: address ������ʼ��ַ
 * @param: length �û�������Ҫ�����ĳ���ĳ���
 * @param: isreset ������֮���Ƿ���Ҫ��λ�û�����
 * 
 * @����: 
 * @return: bool  true�ɹ�/falseʧ��
 * @˵��: 
 * @����: xugl (2013/7/16)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
bool ES_IAP_Ready(uint32 address, uint32 length, bool isreset)
{
    if(length > 0x80000)                //��֧����ô������������
    {
        return false;                   //���ش���
    }
                                        //ȷ���������ݵĺϷ���,���������ͱ��ݲ���CRC������ȷ����Ϊ����ȷ
    if(!ES_APP_IAP_Check(length))
    {
	    return false;                   //���ش���
    }
                                        //д��������־
    ES_APP_IAP_Mark(true, length, address);
                                        //����ϵͳ��λ
    //SYS_Reset();

    return true;
}

uint32 g_ulUartIdelCount = 0;
uint16 g_sUartByteCount[UART_MAX_CHANNEL];
uint8 g_sUartRxDone[UART_MAX_CHANNEL];
/*ȫ�ֱ�������*/
STUPCOMMBUF g_stUpportbuf;
unsigned char g_ucUpportbuf[UPCOMM_FRAME_MAX_LEN];


void muchframe_fill(unsigned char *pucFrame, unsigned short usDatalen, 
                    STMUCHFRAMEINDEX *pstMuchframeindex, unsigned char ucSour, unsigned char ucSeq)
{
	unsigned char ucFrameno = 0;
	//unsigned short usFrameaddr = 0;

	ucFrameno = pstMuchframeindex->ucHead;

//		if(ucFrameno != 0)
//			usFrameaddr = pstMuchframeindex->usAddr[ucFrameno - 1] + pstMuchframeindex->usLen[ucFrameno - 1];

	if(ucFrameno < CON_MUCHFRAME_NUM && (usDatalen) <= UPCOMM_FRAME_MAX_LEN)
	{
		//setdata_bytype(pucFrame, ucType, usDatalen, usFrameaddr);
        memcpy(&pstMuchframeindex->ucData[ucFrameno][0], pucFrame, usDatalen);
		pstMuchframeindex->usLen[ucFrameno]  = usDatalen;
		//pstMuchframeindex->usAddr[ucFrameno] = usFrameaddr;
		pstMuchframeindex->ucSeq[ucFrameno] = ucSeq;        
		pstMuchframeindex->ucSour = ucSour;				

		pstMuchframeindex->ucHead ++;

        pstMuchframeindex->ucTimes[ucFrameno] = 0;
        pstMuchframeindex->ttl = 0x80;
        if(pstMuchframeindex->ucHead >= CON_MUCHFRAME_NUM)
        {
            pstMuchframeindex->ucHead = 0;
        }
        if(pstMuchframeindex->ucHead == pstMuchframeindex->ucTail)
        {
            pstMuchframeindex->ucTail++;
            if(pstMuchframeindex->ucTail >= CON_MUCHFRAME_NUM)
            {
                pstMuchframeindex->ucTail = 0;
            }
        }
	}
}


void fill_list(uint8 ucSour, uint8 seq, uint8 * data, uint16 len)
{

	switch(ucSour)
	{
#ifdef UPCOMM_PORT
	case CON_SOUR_PORT:				//Զ�����߿�
		muchframe_fill(data, len, &g_stMuchframeindex_port, ucSour, seq);
		break;
#endif
    default:
        break;
    }
}
        
void port_init()
{
	g_stUpportbuf.usHeadpos = 0;
	g_stUpportbuf.usTailpos = 0;

	g_stUpportbuf.pucData = g_ucUpportbuf;
    
	memset((unsigned char*)(&g_stMuchframeindex_port), 0, sizeof(g_stMuchframeindex_port));
}
void port_recvdata()
{
	unsigned char ucRet = 0;
	unsigned char ucRecvdata = 0;
	unsigned short i = 0;

	STUPCOMMBUF *pstDebugportbuf = &g_stUpportbuf;

	for(i = 0; i < UPCOMM_FRAME_MAX_LEN; i ++)
	{
		ucRet = GetCommunicateRecDataEx1(UART_DEBUG, &ucRecvdata);
		if(ucRet == TRUE)
		{
            //LOG_DEBUG( DBGFMT"GetCommunicateRecDataEx1 = %x\n",DBGARG, ucRecvdata);

			pstDebugportbuf->pucData[pstDebugportbuf->usHeadpos ++] = ucRecvdata;
			if(pstDebugportbuf->usHeadpos >= UPCOMM_FRAME_MAX_LEN)
				pstDebugportbuf->usHeadpos = 0;
            if(pstDebugportbuf->usHeadpos == pstDebugportbuf->usTailpos) {
                pstDebugportbuf->usTailpos++;
                if(pstDebugportbuf->usTailpos >= UPCOMM_FRAME_MAX_LEN)
				pstDebugportbuf->usTailpos = 0;
            }
		}
		else
			break;
	}
}

unsigned short get_port_recvdata(unsigned char *pucData)
{
	unsigned short usDatalen = 0;
	STUPCOMMBUF *pstDebugportbuf = &g_stUpportbuf;

	if(pstDebugportbuf->usHeadpos >= pstDebugportbuf->usTailpos)
	{
		usDatalen = (pstDebugportbuf->usHeadpos - pstDebugportbuf->usTailpos);
		memcpy(pucData, pstDebugportbuf->pucData + pstDebugportbuf->usTailpos, usDatalen);
	}
	else
	{
		usDatalen = (UPCOMM_FRAME_MAX_LEN - pstDebugportbuf->usTailpos);
		memcpy(pucData, pstDebugportbuf->pucData + pstDebugportbuf->usTailpos, usDatalen);

		memcpy(pucData + usDatalen, pstDebugportbuf->pucData, pstDebugportbuf->usHeadpos);
		usDatalen += pstDebugportbuf->usHeadpos;
	}
	return usDatalen;
}


unsigned char frame_check(unsigned char *pucData, unsigned short *pusLen)
{
	unsigned char ucChecksum = 0;
	unsigned short usContentlen = 0;
	unsigned short i = 0;

    if((*pusLen) < 8)						//��С����Ϊ20�����κ���������
        return RET_TOO_LITTLE;

    if(pucData[0] != FRAME_HEAD_CODE_376_2)					//֡ͷ
        return RET_HEAD_ERROR;

//	    if(pucData[5] != FRAME_HEAD_CODE_376_1)
//			return RET_ERROR;
    
//		if((pucData[1] != pucData[3]) || (pucData[2] != pucData[4]))
//	        return RET_ERROR;
//	
//	    if((pucData[1] & 0x03) != FRAME_CTRL_CODE_376_1)			//����96�汾��Լ ????????????????????????????????LCQ
//	        return RET_ERROR;
//	
//	    if((pucData[3] & 0x03) != FRAME_CTRL_CODE_376_1)
//	        return RET_ERROR;

    usContentlen = (pucData[1]) + pucData[2] * 0x100;
	
	//if(usContentlen > 600)/*��󳤶�����*/
	if(usContentlen > UPCOMM_FRAME_MAX_LEN) //edit by JRF 2013.06.28
		return RET_ERROR;

    if((*pusLen) < (usContentlen))
        return RET_NOENOUGH_LONG;

    ucChecksum = 0;                         //���У����
    for(i = 0; i < usContentlen - (FRAME_HEAD_AREA_BYTES + FRAME_TAIL_AREA_BYTES); i ++)
        ucChecksum += pucData[i + FRAME_HEAD_AREA_BYTES];
    if(ucChecksum != pucData[usContentlen - FRAME_TAIL_AREA_BYTES])
        return RET_ERROR;

    if(pucData[usContentlen - 1] != FRAME_TAIL_CODE_376_2)		//֡β..............
        return RET_ERROR;

    (*pusLen) = (usContentlen);
    
    return RET_SUCCESS;
}
extern uint8 g_pdata[UPCOMM_FRAME_MAX_LEN];

unsigned short recvdata_proc(unsigned char ucSour)
{
	unsigned char ucTmp[UPCOMM_FRAME_MAX_LEN];
    unsigned short usSourceDatalen = 0;
	unsigned short usDatalen = 0;

	unsigned char ucRet = 0;
	unsigned char i = 0;
//	unsigned char ucChan = 0;
    
    UPPKT* pkt = (UPPKT*)(&recvuppkt);
    uint8 afnoffset, AddrOffSet;
    uint8* buf = pkt->data;

	//STTERRUNDATACURR stTerrundataTmp;

	switch(ucSour)
	{
#ifdef UPCOMM_PORT	
	case CON_SOUR_PORT:			//���ص��Կ�
		usSourceDatalen = get_port_recvdata(ucTmp);
		break;
#endif

//	#ifdef UPCOMM_IRPORT
//		case CON_SOUR_IRPORT:				//�����
//			usSourceDatalen = get_irport_recvdata(ucTmp);
//			break;
//	#endif
//	
//	#ifdef UPCOMM_WM
//		case CON_SOUR_WMPORT:				//Զ�����߿�
//			usSourceDatalen = get_wmport_recvdata(ucTmp, &ucChan);
//			ucSour += (ucChan<<4);
//			break;
//	#endif
//	
//	#ifdef UPCOMM_ETHERNET
//		case CON_SOUR_ETHERNETPORT:			//��̫����
//			usSourceDatalen = get_ethernetport_recvdata(ucTmp, &ucChan);
//			ucSour += (ucChan<<4);
//			break;
//	#endif

	default:break;
	}

    usDatalen = usSourceDatalen;
	if(usSourceDatalen != 0)
	{

        if(ucTmp[0] == 0x7E)
        {

    		ucRet = rf_frame_check(ucTmp, &usDatalen);
            
    		switch(ucRet)
    		{
    		case RET_HEAD_ERROR:
    			usDatalen = usSourceDatalen;
                LOG_DEBUG( DBGFMT"recv rf RET_HEAD_ERROR frame len = %d\n",DBGARG, usSourceDatalen);

                LOG_DUMPHEX(LOG_LEVEL_DEBUG, "rf RET_HEAD_ERROR : \n", ucTmp, usSourceDatalen);

    			break;

    		case RET_SUCCESS:

                usDatalen+=4;
    			//recvframe_proc(ucSour, ucTmp + i, usDatalen);
                LED_BlinkSet(LED_LOCAL_RX, 0, 0, 0);
                UartDeadCont = UART_DEAD_CNT; // 2��Сʱ����(���Ź�)
            //  Uart_Status = UART_STATUS_BUSY;//æ״̬
                rf_proc(ucTmp, &usDatalen); //����������������***
                
    			//usDatalen += i;
    			break;

    		case RET_ERROR:
                LOG_DEBUG( DBGFMT"recv rf RET_ERROR frame len = %d\n",DBGARG, usSourceDatalen);
                LOG_DUMPHEX(LOG_LEVEL_DEBUG, "rf RET_ERROR : \n", ucTmp, usSourceDatalen);
                LED_BlinkSet(LED_LOCAL_RX, 1, 5, 2);
                
                LED_BlinkSet(LED_LOCAL_TX, 1, 5, 2);
                usDatalen = usSourceDatalen;
    			break;

    		default:
    			usDatalen = 0;
    			break;
    		}
            
        }
        else
        {
		ucRet = frame_check(ucTmp, &usDatalen);

		if(ucRet == RET_HEAD_ERROR)
		{
			for(i = 1; i < 4; i ++)
			{
				usDatalen -= 1;
				ucRet = frame_check(ucTmp + i, &usDatalen);
				if(ucRet != RET_HEAD_ERROR)
					break;
			}
		}

		switch(ucRet)
		{
		case RET_HEAD_ERROR:
			usDatalen = i;
                LOG_DEBUG( DBGFMT"recv RET_HEAD_ERROR frame len = %d\n",DBGARG, usSourceDatalen);

            LOG_DUMPHEX(LOG_LEVEL_DEBUG, "RET_HEAD_ERROR : \n", ucTmp, usSourceDatalen);

			break;

		case RET_SUCCESS:

#if 0	
			if(ucSour == CON_SOUR_PORT)
				memset((unsigned char*)(&g_stMuchframeindex_port), 0, sizeof(g_stMuchframeindex_port));
#endif
            memcpy(buf, &ucTmp[i], usDatalen);
            
        	pkt->len = *(buf + OFFSET_UP_LEN) + (*(buf + (OFFSET_UP_LEN + 1))) * 0x100 ;
        	pkt->ctrl = *(buf + OFFSET_UP_CTRL);
        	pkt->rarea = (STINFO3762*)(buf + OFFSET_UP_RAREA); //��Ϣ��
        	if (pkt->rarea->stDown.bit1.moduleFlg)//�Ե�����
        	{
        		afnoffset = OFFSET_UP_AFN_BASIC + 6 *(pkt->rarea->stDown.bit1.routeNum + 2); //->hoplevel
        		AddrOffSet = OFFSET_UP_AFN_BASIC;
        	}
        	else//��ģ�����
        	{
        		afnoffset = OFFSET_UP_AFN_BASIC;
        		AddrOffSet = 0;
        	}
        	if (afnoffset > pkt->len)
        	    return i;
        	pkt->afnoffset = afnoffset;

        	pkt->AddrOffSet = AddrOffSet;
        	pkt->afn = *(buf + afnoffset); //AFN      
        	pkt->dataflag = *(buf + afnoffset + 1)+(*(buf + afnoffset + 2)<<8);//���ݵ�Ԫ��ʶ***	

			//recvframe_proc(ucSour, ucTmp + i, usDatalen);
            LED_BlinkSet(LED_LOCAL_RX, 0, 0, 0);
            UartDeadCont = UART_DEAD_CNT; // 2��Сʱ����(���Ź�)
        //  Uart_Status = UART_STATUS_BUSY;//æ״̬
            Handle_Up_PKT(&recvuppkt, 0, NULL); //����������������***
            
			usDatalen += i;
			break;

		case RET_ERROR:
            LOG_DEBUG( DBGFMT"recv RET_ERROR frame len = %d\n",DBGARG, usSourceDatalen);
            LOG_DUMPHEX(LOG_LEVEL_DEBUG, "RET_ERROR : \n", ucTmp, usSourceDatalen);
            LED_BlinkSet(LED_LOCAL_RX, 1, 5, 2);
            
            LED_BlinkSet(LED_LOCAL_TX, 1, 5, 2);
            usDatalen = usSourceDatalen;
			break;

		default:
			usDatalen = 0;
			break;
		}
        }
        if(g_ulUartIdelCount > 80 && usSourceDatalen <= usDatalen)
        {
            UART_Init(UART0);
            port_init();            
            g_ulUartIdelCount = 0;
            usDatalen = 0;
        }
        
	}
    if(g_ulUartIdelCount > (60*1000 / 25))
    {
        UART_Init(UART0);
        port_init();            
        g_ulUartIdelCount = 0;
    }
	return usDatalen;
}
void port_proc()
{
	unsigned short i = 0;
	unsigned short usLen = 0;

	STUPCOMMBUF *pstDebugportbuf = &g_stUpportbuf;

	port_recvdata();
    
	usLen = recvdata_proc(CON_SOUR_PORT);

	for(i = 0; i < usLen; i ++)
	{
		pstDebugportbuf->usTailpos ++;
		if(pstDebugportbuf->usTailpos >= UPCOMM_FRAME_MAX_LEN)
			pstDebugportbuf->usTailpos = 0;
	}
}

void Uart_App_Frame_Proc()
{

#ifdef RF_MESH_LEVEL2
    Setp_RecvProc();
#else
    port_proc();
#endif
    upcomm_mng();
}


void EZMAC_Debug_App_Frame_Proc()
{
//		uint8 bInitFlag = 0;
//		DEBUG_UPPKT* pDebugPkt = &(DebugPacket[nDebugPacketFront]);
//		EZ_TX_PKT* pSendPkt = &EzNetTxPkt;
//		if(nDebugPacketRear != nDebugPacketFront)
//		{
//			if(0 == pDebugPkt->nTTL)
//			{
//				nDebugPacketFront = (nDebugPacketFront + 1) % DEBUG_PACKET_LEN;
//				if(EZMAC_Debug_Status != EZMAC_DEBUG_STATUS_IDLE)
//					EZMAC_Debug_Status = EZMAC_DEBUG_STATUS_IDLE;
//			}
//			else
//			{
//				if(0 == pSendPkt->bValid)
//				{
//					if(EZMAC_Debug_Status != EZMAC_DEBUG_STATUS_TX_DONE)
//					{
//						Handle_Up_PKT(&(pDebugPkt->RecvPacket), 1, pDebugPkt);
//						if(EZMAC_DEBUG_STATUS_IDLE == EZMAC_Debug_Status)
//							nDebugPacketFront = (nDebugPacketFront + 1) % DEBUG_PACKET_LEN;
//					}
//				}
//			}
//		}
//		
//		if(bEZMacRecFlag)
//		{
//			bInitFlag = 1;
//			if(EZMAC_Debug_Status == EZMAC_DEBUG_STATUS_TX_DONE)
//			{
//				if(pDebugPkt->RecvPacket.isneedack)
//				{
//					pDebugPkt = &(DebugPacket[nDebugPacketFront]);
//					if ((PKT_TYPE_SS_SEARCH_METER == EZMacStaAck) || (PKT_TYPE_SS_NET_SET == EZMacStaAck))
//					{
//						if(SendPacketToEzmacProc(pDebugPkt, informpkt, 0x13, 0))
//						{
//							EZMAC_Debug_Status = EZMAC_DEBUG_STATUS_IDLE;
//							nDebugPacketFront = (nDebugPacketFront + 1) % DEBUG_PACKET_LEN;
//						}
//						else
//							bInitFlag = 0;
//					}	
//					else if (PKT_TYPE_DATA == EZMacStaAck) 
//					{
//						if(0 == pSendPkt->bValid)
//						{
//							Handle_RX_Up_Nage_PKT(&negapkt, 1, pDebugPkt);
//							EZMAC_Debug_Status = EZMAC_DEBUG_STATUS_IDLE;
//							nDebugPacketFront = (nDebugPacketFront + 1) % DEBUG_PACKET_LEN;						
//						}
//						else
//							bInitFlag = 0;
//					}     							
//				}
//			}
//			if(bInitFlag)
//				bEZMacRecFlag = 0;
//		}
}

				//ԭ�������ݿ�汾��Ϣ������Ϊ16���ֽ�


unsigned char write_prg_to_flash(unsigned int addr, unsigned short usNo, unsigned char *pucData, unsigned short usDatalen)
{
	unsigned char ucRet = SA_ERR_OK;
	unsigned char ucTmp[UPGRADE_LEN_LIMIT];
	unsigned short usprgsize;
	
//		pucTmp = MEM_Alloc(usDatalen);
//		if(NULL == pucTmp)
//		{
//			LOG_ERROR("%s-%d:  malloc fail!\r\n", __FUNCTION__, __LINE__);
//			return CON_BOOL_FALSE;
//		}

	//getdata_bytype((unsigned char*)(&ulPrgallsize), DT_PRGUPDATE, (4 << 4) + 3, DP_DOWNLOAD_LENGTH_ADDR_RAM);
	//getdata_bytype((unsigned char*)(&usprgsize), DT_PRGUPDATE, (2 << 4) + 4, DP_DOWNLOAD_LENGTH_PER_FRAME_RAM);
    usprgsize = upd_pkt.prgOneSize;

    BFLS_PreWrite();
    if(0 != BFLS_Write((uint8 *)pucData, addr + (usNo * usprgsize), usDatalen))
    {
        
        LOG_DEBUG( DBGFMT" upgrade data write err size %d\n",DBGARG, usprgsize);
        return SA_ERR_FT;
    }

    if(0 != BFLS_Read((uint8 *)ucTmp, addr + (usNo * usprgsize), usDatalen))
    {
    
        LOG_DEBUG( DBGFMT" upgrade data read  err size %d\n",DBGARG, usprgsize);
        return SA_ERR_FT;
    }


//		setdata_bytype(pucData, DT_PRGUPDATE, ((usDatalen << 4) + 1), (usNo * usprgsize));//д��������
//		getdata_bytype(pucTmp,	DT_PRGUPDATE, ((usDatalen << 4) + 1), (usNo * usprgsize));//����������

	//�Ƚ�
	if(memcmp(pucData, ucTmp, usDatalen) != 0)
	{
//			setdata_bytype(pucData, DT_PRGUPDATE, ((usDatalen << 4) + 1), (usNo * usprgsize));//д��������
//			getdata_bytype(pucTmp,	DT_PRGUPDATE, ((usDatalen << 4) + 1), (usNo * usprgsize));//����������
		//�Ƚ�
		
        LOG_DEBUG( DBGFMT" upgrade data read and write check err size %d\n",DBGARG, usprgsize);
        //LOG_DUMPHEX(LOG_LEVEL_DEBUG, "HexData : \n", updata_pkt_back, m);
        BFLS_PreWrite();
        if(0 != BFLS_Write((uint8 *)pucData, addr + (usNo * usprgsize), usDatalen))
        {
            return SA_ERR_FT;
        }
        
        if(0 != BFLS_Read((uint8 *)ucTmp, addr + (usNo * usprgsize), usDatalen))
        {
            return SA_ERR_FT;
        }
		if(memcmp(pucData, ucTmp, usDatalen) != 0)
			ucRet = SA_ERR_FT;
	}
	//if(NULL != pucTmp) MEM_Free(pucTmp);

	return ucRet;
}
#ifdef UPGRADE_TEST
#define UPGRADE_SEGMENT_FLG_LEN     2///4
#else
#define UPGRADE_SEGMENT_FLG_LEN     4///4
#endif
//extern STUPGRADE g_stUpgrade;

uint32 g_ucUpgradeCount = 0;

uint8 g_ucData[256];
int32 filetrans_flag(UPPKT *pkt/*,unsigned char *pucData2,unsigned short *pusLen, unsigned char ucSour*/)
{
	unsigned short m=0;
	unsigned char ucRet=SA_ERR_OK;
    
	unsigned char ucTmp[8];
	unsigned char *pucTmp2 = g_ucData;//(uint8 *)&updata_code_cache[0];//[512];

	unsigned long ulAllsegment=0; 			//�ܶ���
	//unsigned long uladdr=0;
	unsigned short usprgsize=0;
	unsigned long ulPrgallsize=0;
	unsigned long ulFrmNo=0;
	unsigned long ulFrmNoLast=0;
	unsigned long ulFrmnum=0, ulBaseAddr = 0;
    unsigned char *pucData1 = pkt->data + 13;
    int32 retID = -1;
    g_ucUpgradeCount = 0;
	if(pkt->len <= 0)
		return -1;

//		pucTmp2 = MEM_Alloc(((*pusLen) > 256) ? (*pusLen) : 256 );
//		if(NULL == pucTmp2)
//		{
//			LOG_ERROR("%s-%d:  malloc fail!\r\n", __FUNCTION__, __LINE__);
//			return;
//		}

    memcpy((uint8 *)&retID, pkt->data + 18, 4);
	if(pucData1[0] == 0x00)
	{
		//���ֽ�00 00 00 ��ʾ�����������
		//LOG_DEBUG("%s-%d:  upgrade clear! \r\n", __FUNCTION__, __LINE__);
        //I2cWrite(0xA0,  (uint8 *)&upd_pkt, EEPROM_UPDATE_PARA, sizeof(UPDATE_PKT));  
        memset((uint8 *)&upd_pkt, 0, sizeof(UPDATE_PKT));
        upd_pkt.type = pucData1[0];
		ulAllsegment = 0;
		//setdata_bytype((unsigned char*)(&ulAllsegment) ,DT_PRGUPDATE, (2 << 4), DP_DOWNLOAD_FRAME_NUM_ADDR);
		upd_pkt.total = 0;
		//��ʼ��λͼ
		memset(pucTmp2, 0, CON_DOWNLOADBITMAP_SIZE);
		//setdata_bytype(pucTmp2, DT_PRGUPDATE, (CON_DOWNLOADBITMAP_SIZE << 4) + 3, DP_DOWNLOAD_BITMAP_ADDR);
        I2cWrite(0xA0,  pucTmp2, DP_DOWNLOAD_BITMAP_ADDR, CON_DOWNLOADBITMAP_SIZE); 
        
        g_stUpgrade.flg = 0x00;
        g_stUpgrade.crc = CRC_16BIT_Check1((uint8 *)&g_stUpgrade, sizeof(STUPGRADE) - 2);
        I2cWrite(0xA0,  (uint8 *)&g_stUpgrade, DP_DOWNLOAD_PROC_PARA_ADDR, sizeof(STUPGRADE)); 
//			memset(ucTmp, 0, 8);
//			ucTmp[0] = CON_TERDP_STATE_NORMAL;
//			ucTmp[1] = (ucSour & 0x0F);
//			ucTmp[2] = CON_FRM_DP_TYPE_PROG;//pucData[2];//��������
//			setdata_bytype(ucTmp, DT_PRGUPDATE, (8 << 4), DP_DOWNLOAD_EXE_FLAG_ADDR);//������־
//	
//			memset(ucTmp,0x00,4); 
//			memcpy(pucData1+m, ucTmp, 4);
//			m = m+4;		
//			(*pusLen) = m;
	}
	else if(pucData1[0] == UPGRADE_TYPE_TER_MODULE || (pucData1[0] == UPGRADE_TYPE_METER_MODULE) || pucData1[0] == UPGRADE_TYPE_METER_SELF
        || pucData1[0] == UPGRADE_TYPE_COLLECT_SELF || pucData1[0] == UPGRADE_TYPE_REPEAT_MODULE)
	{
	    g_ucUpgradeFlgForPush = 0xAA;
	    upd_pkt.type = pucData1[0];
		//   ���ֽڹ�Լ����
		//	03H: ����ͨ��ģ�� ,FDHΪ��ƣ�07Ϊ�ɼ���ͨ��ģ��
		//   00H: ��ʼ���ģ�01H:��������
		//   00H: ���ķ�ʽ����

		//��i�����ݳ���Lf
		usprgsize = pucData1[5 + UPGRADE_SEGMENT_FLG_LEN] + pucData1[6 + UPGRADE_SEGMENT_FLG_LEN]*0x100;
        if(usprgsize > 224)
        {
            return -1;
        }
		//��i�α�ʶ��ƫ�ƣ�i=0��n��
#if (UPGRADE_SEGMENT_FLG_LEN == 4)		
		ulFrmNo = pucData1[5] + pucData1[6]*0x100 + pucData1[7]*0x10000 + pucData1[8]*0x1000000;
#else
        ulFrmNo = pucData1[5] + pucData1[6]*0x100;
#endif

		//getdata_bytype(ucTmp, DT_PRGUPDATE, (4<<4) + 3, DP_SEGMENT_SHIFT_ADDR_RAM);		
		ulFrmNoLast = upd_pkt.seq;//;ucTmp[0] + ucTmp[1]*0x100 + ucTmp[2]*0x10000 + ucTmp[3]*0x1000000;

		//getdata_bytype((unsigned char*)(&ulPrgallsize), DT_PRGUPDATE, (4 << 4) + 3, DP_DOWNLOAD_LENGTH_ADDR_RAM);
        ulPrgallsize = upd_pkt.prgAllSize;

		if(ulFrmNo == 0 && pucData1[1] == 0x00)
		{ 
			//��һ֡����
			//LOG_DEBUG("%s-%d:  first frame! \r\n", __FUNCTION__, __LINE__);
			//STDATETIME stCurrenttime = get_currenttime();
			//RamWrite(ADDR_UPGRADE_TIME, (unsigned char *)&stCurrenttime, sizeof(STDATETIME));
            upd_pkt.flg = 0xAA;
            
            g_stUpgrade.flg = 0x00;
            g_stUpgrade.crc = CRC_16BIT_Check1((uint8 *)&g_stUpgrade, sizeof(STUPGRADE) - 2);
            I2cWrite(0xA0,  (uint8 *)&g_stUpgrade, DP_DOWNLOAD_PROC_PARA_ADDR, sizeof(STUPGRADE)); 
            
			ulPrgallsize = 0;
			//setdata_bytype((unsigned char*)(&ulPrgallsize), DT_PRGUPDATE, (4 << 4) + 3, DP_DOWNLOAD_LENGTH_ADDR_RAM);
			//setdata_bytype((unsigned char*)(&usprgsize), DT_PRGUPDATE, (2 << 4) + 4, DP_DOWNLOAD_LENGTH_PER_FRAME_RAM);
            upd_pkt.prgAllSize = ulPrgallsize;
            
            ulPrgallsize += usprgsize;
            upd_pkt.prgOneSize = usprgsize;

			ulAllsegment=pucData1[3]+pucData1[4]*0x100;
			//��¼�ܶ���
			upd_pkt.total = ulAllsegment;
			//setdata_bytype((unsigned char*)(&ulAllsegment) ,DT_PRGUPDATE, (2 << 4), DP_DOWNLOAD_FRAME_NUM_ADDR);
			//��ʼ��λͼ
			memset(pucTmp2, 0, CON_DOWNLOADBITMAP_SIZE);
			//setdata_bytype(pucTmp2, DT_PRGUPDATE, (CON_DOWNLOADBITMAP_SIZE << 4) + 3, DP_DOWNLOAD_BITMAP_ADDR);
            I2cWrite(0xA0,  pucTmp2, DP_DOWNLOAD_BITMAP_ADDR, CON_DOWNLOADBITMAP_SIZE); 


		}
		else if(ulFrmNo == ulFrmNoLast+1)                
		{
			ulPrgallsize += usprgsize;
		}
		else if(ulFrmNo == ulFrmNoLast)
		{
			ulPrgallsize +=0;						//�ظ����ĵĴ���
		}
		else
		{
			ulPrgallsize +=0;
			ucRet = SA_ERR_FT;
		}

		ulFrmnum = ulFrmNo;

		memcpy(pucTmp2, pucData1 + 7 + UPGRADE_SEGMENT_FLG_LEN, usprgsize);
		//x_reverse(pucTmp2, usprgsize); 
		
        uint8 *pxor;
        uint8 loop = 0;
        switch(upd_pkt.type)
        {
        case UPGRADE_TYPE_METER_MODULE:
        case UPGRADE_TYPE_REPEAT_MODULE:
            ulBaseAddr = EXT_FLASH_SS_CODE_START;
            break;
        case UPGRADE_TYPE_METER_SELF:
        case UPGRADE_TYPE_COLLECT_SELF:
            ulBaseAddr = EXT_FLASH_I_SS_CODE_START;
            break;
        case UPGRADE_TYPE_TER_MODULE:
            ulBaseAddr = EXT_FLASH_BS_CODE_START;
            if(ulFrmNo==0)  //��һ�飬ͷ��32�ֽڲ���Ҫ����
            {
                pxor =(uint8 *)pucTmp2+32;
                for(loop=0;loop<(usprgsize - 32);loop++)
                {
                    *pxor = (*pxor)^FW_KEY[loop&0xf];
                    pxor++;
                }
            }
            else
            {
                pxor =(uint8 *)pucTmp2;
                for(loop=0;loop<usprgsize;loop++)
                {
                    *pxor = (*pxor)^FW_KEY[loop&0xf];
                    pxor++;
                }
            }
            
            break;
        default:
            ucRet = SA_ERR_FT;
            break;
        }
		upd_pkt.baseAddr = ulBaseAddr;
        
		if(ucRet == SA_ERR_OK)
        {      
	        ucRet = write_prg_to_flash(ulBaseAddr, ulFrmNo, pucTmp2, usprgsize);
        }
        else
        {
            LOG_DEBUG( DBGFMT"upgrade recv wrong frame and request new frame +++++    \n",DBGARG);

        }
		if(ucRet == SA_ERR_OK)
		{
			/*����λͼ*/
			unsigned char ucBitNo=0;
			unsigned char ucBitmap = 0;
			unsigned short  usByteNo=0;
			usByteNo = (unsigned short)(ulFrmNo / 8);
			ucBitNo = (unsigned char)(ulFrmNo % 8);
            
            I2cRead(0xA0,  pucTmp2, (DP_DOWNLOAD_BITMAP_ADDR + usByteNo), 1); 
			//getdata_bytype((unsigned char *)(&ucBitmap), DT_PRGUPDATE, (1 << 4) + 3, (DP_DOWNLOAD_BITMAP_ADDR + usByteNo));
			ucBitmap |= (1<<ucBitNo);
            
            I2cWrite(0xA0,  pucTmp2, (DP_DOWNLOAD_BITMAP_ADDR + usByteNo), 1); 
			//setdata_bytype(&ucBitmap, DT_PRGUPDATE, (1 << 4) + 3, (DP_DOWNLOAD_BITMAP_ADDR + usByteNo));//дλͼһ���ֽ�
			//setdata_bytype((unsigned char*)(&ulPrgallsize), DT_PRGUPDATE, (4 << 4) + 3, DP_DOWNLOAD_LENGTH_ADDR_RAM);
            upd_pkt.prgAllSize = ulPrgallsize;
            upd_pkt.seq = ulFrmNo;
            

			//Ϊ���б�����׼��
			ulAllsegment=pucData1[3]+pucData1[4]*0x100;
			if(ulAllsegment != 0)
			{
				memcpy(ucTmp, pucData1+5, 4); 
				memcpy(pucData1+m, ucTmp, 4);
				m = m+4;									  
			}
			else
			{
				memset(ucTmp,0x00,4);
				memset(pucData1+m,0,4);					         
				m = m + 4;
			}
			//(*pusLen )= m;
		}
		else		//����ǰһ֡�Ķα�ʶ
		{
//				memcpy(ucTmp, (unsigned char *)&ulFrmNoLast, 4);
//				//memset(pucData1+m, 0xFF, 4);					//����һ����鷢���ļ�������д0xFFFFFFFFH
//				memcpy(pucData1+m, ucTmp, 4);				//���������鷢���ļ�������д�ϴ�֡��ţ��ݴ���վ
//				m = m+4;
//				(*pusLen )= m;
		}
        
        retID = upd_pkt.seq;

		//setdata_bytype(ucTmp, DT_PRGUPDATE, (4 << 4) + 3, DP_SEGMENT_SHIFT_ADDR_RAM);
        //memcpy((uint8 *)&upd_pkt.seq, ucTmp, 2);
		//getdata_bytype((unsigned char*)(&ulAllsegment) ,DT_PRGUPDATE, (2 << 4), DP_DOWNLOAD_FRAME_NUM_ADDR);
		if(ulFrmnum +1== upd_pkt.total)			//���һ�����ݰ�
		{
			//LOG_DEBUG("%s-%d:  upgrade  last! \r\n", __FUNCTION__, __LINE__);
			if(ulPrgallsize <= 128*1024 )
			{
                upd_pkt.prgAllSize = ulPrgallsize;
                upd_pkt.flg = 0x55;
                g_ucUpgradeFlgForPush = 0xFF;
				//setdata_bytype((unsigned char*)(&ulPrgallsize), DT_PRGUPDATE, (4 << 4) + 3, DP_DOWNLOAD_LENGTH_ADDR_RAM);
//					getdata_bytype(ucTmp, DT_PRGUPDATE, (8 << 4), DP_DOWNLOAD_EXE_FLAG_ADDR);//�ɲ�д�����־
//	
//					ucTmp[0] = CON_TERDP_STATE_UPDATE;
//					setdata_bytype(ucTmp, DT_PRGUPDATE, (8 << 4), DP_DOWNLOAD_EXE_FLAG_ADDR);//�ɲ�д�����־
//					uladdr=CON_BOOTLOADER_BEGADDR;
//					setdata_bytype((unsigned char*)(&uladdr), DT_PRGUPDATE, (4 << 4), DP_DOWNLOAD_START_ADDR);
                if(upd_pkt.type == UPGRADE_TYPE_TER_MODULE)//����ģ�����
                {
                    
                    ImageBody tmpImage;
                    BFLS_Read((uint8 *)&tmpImage, EXT_FLASH_BS_CODE_START, 32);
                    if(Code_Check_Sum(EXT_FLASH_BS_CODE_START) == SA_ERR_OK)
                    {                     
                        ES_APP_IAP_Mark(true, tmpImage.plen, FLS_APPSTART_ADDR);
                        retID = upd_pkt.seq;
                    }
                    else
                    {
                        retID = -1;
                    }
                    //��ʱ200MS
//	                    Delay_ms(200);                                      
//	                    Reset();
                }
                else
                {
                    g_stUpgrade.flg = 0x55;
                    g_stUpgrade.type = upd_pkt.type;
                    g_stUpgrade.step = 0;
                    g_stUpgrade.p2pFaildTimes = 0;
                    g_stUpgrade.allByte = ulPrgallsize;
                    //extern uint32 ExtCtrlDeadCont;
                    ExtCtrlDeadCont = 0;
                    uint16 tmp = ulPrgallsize%DOWN_UPGRADE_DIVISION_LEN;
                    if(tmp != 0)
                    {
                        g_stUpgrade.frameNum = 1 + ulPrgallsize/DOWN_UPGRADE_DIVISION_LEN;
                    }
                    else
                    {
                        g_stUpgrade.frameNum = ulPrgallsize/DOWN_UPGRADE_DIVISION_LEN;
                    }
                    g_stUpgrade.seq = 0;
                    memset(g_stUpgrade.bitMap, 0x00, DOWN_UPGRADE_BITMAP_SIZE);
                    
                    g_stUpgrade.crc = CRC_16BIT_Check1((uint8 *)&g_stUpgrade, sizeof(STUPGRADE) - 2);
                    I2cWrite(0xA0,  (uint8 *)&g_stUpgrade, DP_DOWNLOAD_PROC_PARA_ADDR, sizeof(STUPGRADE));  
                }

			}
			else
				retID = -1;

		}
	}
	else
		retID = -1;//ucRet = CON_BOOL_FALSE;
		
    upd_pkt.crc = CRC_16BIT_Check1((uint8 *)&upd_pkt, sizeof(UPDATE_PKT) - 2);
    I2cWrite(0xA0,  (uint8 *)&upd_pkt, EEPROM_UPDATE_PARA, sizeof(UPDATE_PKT));  

    return retID;
	//if(NULL != pucTmp2) MEM_Free(pucTmp2);    
}


void upVersion(void)
{
    static uint16 count = 0;

    if(ExtCtrlDeadCont != 0)
    {
        return ;
    }

    
    if(g_bTerType == 0xFF)
    {
        ExtCtrlDeadCont = 40;
//#ifdef RF_MESH_LEVEL2
        if(count % 2 == 1)
        {
            PST_Frame frm;                          //��Լ֡�ṹ��
            frm.protocol = PST_FRM_802_L_NO;
            frm._ie.app = g_pdata;
            frm._ie.send = g_pdata;
            frm._ie.R = g_pdata;
            frm._ie.seq = 0;
            frm._ie.ftd = 0x03;
            frm._ie.fn = 0x1A;
            frm._ie.app[0] = 0x1A;
            PST_802_Proc(&frm);
        }
    //#else
        else
        {
        	uint8 data[55]; //��������ظ���
        	uint8 m = 0;
            
            uint16 tmp;
            memcpy(data, informpkt, 10);
            data[5] = rfpara.rf_channel & 0x0F;
            data[3] = 0xCA;
            m+=10;
            data[m++] = 0x03;
            data[m++] = 0x02;
            data[m++] = 0x01;
            
        	data[m++] = 0x73;//��·�ɣ���Ҫ�·��ӽڵ�
        	data[m++] = 0x28;//
        	data[m++] = (getChannelNum() - 1);//�ŵ�����
        	data[m++] = 0x00 ;//
        	
            uint8 temp[4];
            uint8 Freq = 0;
            I2cRead(0xA0, &temp[0], FM_FREQ_DEFINE+8, 4); //������Ƶ�㶨�����***	
            Freq = temp[2] & 0xF0;

            if(Freq == 0x40)
            {
        	    data[m++] = FREQ_433M;//��������Ƶ��
            }
            else if(Freq == 0x90)
            {
        	    data[m++] = FREQ_915M;//��������Ƶ��
            }
            else if(Freq == 0x70)
            {
        	    data[m++] = FREQ_470M;//��������Ƶ��
            }
            else if(Freq == 0x80)    
            {
        	    data[m++] = FREQ_868M;//��������Ƶ��
            }
            else
            {
        	    data[m++] = SX127X_FREQ;//��������Ƶ��
            }	
        	//data[m++] = SX127X_FREQ;//��������Ƶ��
        	data[m++] = 0x00;//
            //�ӽڵ�����ʱ

        	tmp = (TIME_OUT_MAX * MAX_DATA_TIME_LIVE * SYS_TIMER_COUNT_MS * 2) / 1000;
            tmp = (tmp == 0) ? 1 : tmp;
        	data[m++] = tmp & 0xff;//
        	data[m++] = tmp >> 8;// 
        	//�㲥��ʱ
        	tmp = (FULLTIME * SYS_TIMER_COUNT_MS) / 1000;
            tmp = (tmp == 0) ? 1 : tmp;
        	data[m++] = tmp & 0xff;//
        	data[m++] = tmp >> 8;//   
        	//1376.2����ĳ���
        	data[m++] = 0x00;//
        	data[m++] = 0x01;//  
        	//1376.2�ļ�����֧�ֵ���󵥸����ݰ�����
        	data[m++] = 0xC0;//
        	data[m++] = 0x00;// 
        	data[m++] = 0x05;//���������ȴ�ʱ��
        	memcpy(data + m, (uint8 *)nDeviceMacAddr, 6);
            m+=6;
            //���ӽڵ�����
            uint16 nMeterNum = rfpara.rf_slotnum - 1;
        //	                updata_pkt_back[m++] = 0x00;//
        //					updata_pkt_back[m++] = 0x06;// 
        	memcpy(data + m, &nMeterNum, 2);
            m+=2;
        	//��ǰ�ӽڵ�����
        	
        	nMeterNum = Meter_Num();
            memcpy(data + m, &nMeterNum, 2);
            m+=2;
        //					updata_pkt_back[m++] = 0x00;//
        //					updata_pkt_back[m++] = 0x04;// 
        	//ͨ��ģ��ʹ��Э�鷢������
        	data[m++] = 0x12;//
        	data[m++] = 0x12;// 
        	data[m++] = 0x05;//		
        	//ͨ��ģ����󱸰����ڷ�������
        	data[m++] = 0x12;//
        	data[m++] = 0x12;// 
        	data[m++] = 0x05;//
        				//ͨ��ģ�鳧��    
        	memcpy(data + m, (uint8 *)&sVersiong, 9);
            m+=9;
            data[m++] = 0x00;// 
            data[m++] = 0x00;//
        //	    memcpy(data + m, (uint8 *)&sVersiong, 9);
        //	    m+=9;
            data[m]=Check_SUM(&data[3], m-3); //У���
            m++;
            data[m++]=0x16;                                                                                
            
            data[1] = m & 0xff; //������ 
            data[2] = m >> 8;
            if(bEZMACDebugType)
                SendPacketToEzmacProc(pEZMACRecvPacket, data, m, 0);
            else
                UART_PutArray(UART0, data, m);   
        }
    //#endif

        count++;
    }
}

//	#define AFN_TYPE_INFORM  0x0
//	#define AFN_TYPE_INITIALIZE 0x1
//	#define AFN_TYPE_DATARELAY 0x2
//	#define AFN_TYPE_DATAREQUES 0x3
//	#define AFN_TYPE_LINKDET 0x4
//	#define AFN_TYPE_CTLCMD 0x5
//	#define AFN_TYPE_REPORT 0x6
//	#define AFN_TYPE_ROUTE_POLL 0x10
//	#define AFN_TYPE_ROUTE_SET 0x11
//	#define AFN_TYPE_ROUTE_CTL 0x12
//  #define AFN_TYPE_ROUTEDATARELAY 0x13
//	#define AFN_TYPE_UPDATA  0X15
//AFN_TYPE_DEBUG


#if 1//REMESH_OLD
void Handle_Up_PKT(UPPKT *pkt, uint8 bEZMACDebugTypeTemp, DEBUG_UPPKT* pRecvPacketTemp)
{
	uint16 id;
	uint8 temp, m = 0;
	uint8 *deladd;
	uint8 updata_pkt_back[55]; //��������ظ���
	uint8 *p;
	HASHT elem;
	HASHT searchelem;
	HASHT* pHasht;	
	int ret;
	ROUTE temprt;
	//DATAPKT *dpkt;
//	uint8 *pxor;
	int loop;
//	ImageBody *pImageBody;
	int ReturnHash = 0;
	//uint8 recvid;
	uint8 dis[32] ;
    uint16 find_id;
	//STAREQUEST *spkt;
	//EXTDATA* pExtDta;
	int return_hash;	
//	uint8 SN[6]={0};
	uint16 handled_no,i;
    pkt->isneedack = 0;
//	PKT *pSend;
	
//	PKT* pEzTxPkt;
//	uint8* pEzTxData;
	
	uint8 insertok = 1; //��ӳɹ���־λ         
	uint8 nStart;
	uint8 bError = 0;	

	uint16 nMeterNum = 0;
	uint16 nStartNum = 0;
	uint16 nSSNum = 0;
	
	uint8 nNeedNum;
	uint8 nCurSendNum = 0;
	uint8* pSn, *pSendData;
	WB_SPACE* pBlackSSSpace;
	
//	WB_SPACE* pWriteSSSpace;
	//uint8 nSw;
    //��Ϣ��
    STINFO3762 stinfo;
    memset((uint8 *)&stinfo, 0, sizeof(STINFO3762));
    stinfo.stUp.bit1.routeFlg = pkt->rarea->stDown.bit1.routeFlg;
    stinfo.stUp.bit1.moduleFlg = pkt->rarea->stDown.bit1.moduleFlg;
    stinfo.stUp.bit1.routeNum = pkt->rarea->stDown.bit1.routeNum;
    stinfo.stUp.bit2.channelFlg = rfpara.rf_channel & 0x0F;    
    stinfo.stUp.bit2.channelFlg = pkt->rarea->stDown.bit2.channelFlg;
    stinfo.stUp.bit3.meterFlg = 0x05;
    stinfo.stUp.seq = pkt->rarea->stDown.seq;

	bEZMACDebugType = bEZMACDebugTypeTemp;
	pEZMACRecvPacket = pRecvPacketTemp;
	if(0 == bEZMACDebugType)
		bDebugSend = 0;
	
	switch (pkt->afn)
	{
		case AFN_TYPE_INITIALIZE:     //��ʼ��                   
		{
			if (pkt->dataflag == 0x1)   //Ӳ����ʼ��                             
			{    
HW_INIT_F1:			
				SendReturnpkt(0, &stinfo);
				
				//��ʱ200MS
				DelayNS(100);
			//	Delay_ms(200);
				Reset();//��λ
				break;			
			}
			if (pkt->dataflag == 0x2)    //F2��������ʼ�� (�������)                           
			{
PARA_INIT_F2:			
				bError = 0;
				if(0 == bSendResetCmd)
				{
					memset(dis,0xff,32);
					for(i=0;i<32;i++)
					{	                                   
						I2cWrite(0xA0, dis, 0+(i*32), 32); //��λӰ�ӿռ�***
						memset(shadow_space+(i*32),0xff,32);						 
					}
                    Init_Hash_Table();                    
				}
				else
					bError = 1;

                nChangeChannelTime = curslottime + MAX_SLOT_TIME * 10;
                bNeedChangeChannel = TRUE;

                for(i = SUP_SS_INDEX_START; i < MAX_SUP_SS_NUM+1; i++)  //����
                {
                    if(cltor[i].devAddr[0] < 0xFF)//
                    {
                        Delete_SS_Node(i);          
                    }
                }

				SendReturnpkt(bError, &stinfo);	
				
				if(0 == bError)
				{
					//��ʱ200MS
					DelayNS(100);
				//	Delay_ms(200);
				//	Reset();//��λ					
				}
				break;				
			}
			if (pkt->dataflag == 0x4)  //F3 ��������ʼ��      
			{
DATA_INIT_F3:			
				//	Erase_FM_SN();//������������м�¼��SN
				SendReturnpkt(0, &stinfo);
				break;
			}

			if (pkt->dataflag == 0x8)  //F4 �ָ���������
			{
				uint8 bValue = 0;
				uint8 Temp[16];
				bError = 0;
				if(0 == bSendResetCmd)
				{
					memset(dis,0xff,32);
					for(i=0;i<32;i++)
					{	                                   
						I2cWrite(0xA0, dis, 0+(i*32), 32); //��λӰ�ӿռ�***
						memset(shadow_space+(i*32),0xff,32);						 
					}
                    Init_Hash_Table();                    

					//��ʼ��·�ɲ���
					cltparm.routeuptime = 60;//����      
					cltparm.maxttl = 120;//��������
					cltparm.routeseltime = 30;//ѡ���ڵ�ʱ��    
					I2cWrite(0xA0, (uint8*) &cltparm, FM_CLTP_ADDR, sizeof(CLTP)); //д��·�ɲ���         

					//��ʼ���������
                    //extern uint8 nDeviceMacAddr[6];
                    uint8 channelId = Cal_Hash_Value(nDeviceMacAddr) % getChannelNum();
                    if(channelId == 0)
                    {
                        channelId = 1;
                    }                  
					memset(Temp, 0, sizeof(RF_PARAMETER));
    
                    RF_PARAMETER * p = (RF_PARAMETER *)Temp;

                    p->rf_channel = channelId; //�ŵ���
                    p->rf_power = 0;//���书��
                    p->rf_boad = 0x1;//���в�����
                    p->rf_limit = 157;//rssi����        
                    p->rf_net_id = 0x01;//�����ʶ
                    p->rf_route = 0x01;//·�ɷ�ʽ
                    p->rf_jump_chaneel=0x2;//��Ƶ���к�
                    p->rf_slotnum=MAX_SUP_SS_NUM;//ʱ֡����
                    //p->panid = 0xFFFF;
                    memcpy((uint8 *)&p->panid, nDeviceMacAddr, 2);
                    //rfpara.panid = 0xFFFF;
                    p->shortID = localid;

                    
					I2cWrite(0xA0, Temp, FM_NETPARA_ADDR, sizeof(RF_PARAMETER)); //���������***

					memset(Temp, 0, 9);
                    uint8 addrLen = 6;
                    I2cWrite(0xA0, &addrLen, FM_DEVICE_ADDR_LEN, 1);
                    
					I2cWrite(0xA0, Temp, FM_DEVICE_ADDR , 6);
				//	I2cWrite(0xA0, Temp, FM_BROAD_METER_TIME , 2); 

					bValue = 0;
					I2cWrite(0xA0, &bValue, FM_BROAD_METER_ENABLE , 1); 
					I2cWrite(0xA0, &bValue, FM_SS_CODEUPDATA_ENABLE , 1); 
					I2cWrite(0xA0,  &bValue, FM_I_SS_CODEUPDATA_ENABLE, 1);   
					I2cWrite(0xA0,  &bValue, FM_CHANNEL_SET_MODE, 1);         						

					bValue = METER_DEAD_COUNT;
					I2cWrite(0xA0,  &bValue, FM_METER_DEAD_COUNT, 1);     
		
					if(0 == pkt->data[13])
						bValue = 1;		 
					else
						bValue = 0;		 
					I2cWrite(0xA0, &bValue, FM_SS_VALIDATE_ENABLE , 1); 
					I2cWrite(0xA0, &bValue, FM_METER_INFO_SYN_ENABLE, 1);	

                    nChangeChannelTime = curslottime + MAX_SLOT_TIME * 10;
                    bNeedChangeChannel = TRUE;

                    for(i = SUP_SS_INDEX_START; i < MAX_SUP_SS_NUM+1; i++)  //����
                    {
                        if(cltor[i].devAddr[0] < 0xFF)//
                        {
                            Delete_SS_Node(i);          
                        }
                    }
                    
				}
				else
					bError = 1;
				
				SendReturnpkt(bError, &stinfo);
				if(0 == bError)
				{
					DelayNS(100);
					//Reset();//��λ
				}
				break;
			}
			
			if(0 == bEZMACDebugType)
				Uart_Status = UART_STATUS_IDLE;			
			break;
		}
		case AFN_TYPE_DATAREQUES:  //��������                      
		{        			
			#if 0
			memcpy((uint8*)updata_code_cache,(uint8*)HashUpdateSectorAdd,256);
			UART_PutArray(UART1, (uint8*)updata_code_cache, 20); //FLASH�ڴ�
			updata_code_cache[0]=updata_code_cache[0]&(~(1<<xcnt));
			UART_PutArray(UART1, (uint8*)updata_code_cache, 20); //ram�ڴ�
			xcnt++;
			IapAction = IAP_ACTION_TRANSFER_HASH1;//дtable1
			HashUpdateSectorAdd = 0x68700;
			while (IapAction != IAP_ACTION_IDLE);		
			#endif
	
			if (pkt->dataflag == 0x1)  //F1       ���̴���Ͱ汾��Ϣ                       
			{                                 
GET_VERSION_F1:				

				memcpy(updata_pkt_back, pkt->data, 13); 
                memcpy(updata_pkt_back + 4, (uint8 *)&stinfo, 6);
                m = 13;
				//updata_pkt_back[1] = 18; //������       
				updata_pkt_back[3] = 0x8a; 	//������    
	
				memcpy(updata_pkt_back + m, (uint8 *)&sVersiong, 9);
                m+=9;
				updata_pkt_back[m]=Check_SUM(&updata_pkt_back[3], m-3); //У���
				m++;
				updata_pkt_back[m++]=0x16;                                                                                
	
                updata_pkt_back[1] = m & 0xff; //������ 
                updata_pkt_back[2] = m >> 8;
				if(bEZMACDebugType)
					SendPacketToEzmacProc(pEZMACRecvPacket, updata_pkt_back, m, 0);
				else
					UART_PutArray(UART0, updata_pkt_back, m);	
				break;			
			}
			//Test ����²������еĸ���ֵ�����ڵ���
			else if(pkt->dataflag == 0x2)
			{
GET_TEMPER_VALUE_F2:			
				memcpy(updata_pkt_back, pkt->data, 13);                                          
				updata_pkt_back[1] = 18; 		//������     
				updata_pkt_back[3] = 0x8a; 	//������    
				updata_pkt_back[13] = EZMacProReg.Temper_ADC_Value ;		//��ǰ�¶�ֵ
				updata_pkt_back[14] = EZMacProReg.Cur_Crystal_Value;		//��ǰУ׼ֵ
				updata_pkt_back[15] = EZMacProReg.CrystalValue;				//��ʼУ׼ֵ

				updata_pkt_back[16]=Check_SUM(&updata_pkt_back[3], 13); //У���	                 
				updata_pkt_back[17]=0x16;                                                                                

				if(bEZMACDebugType)
					SendPacketToEzmacProc(pEZMACRecvPacket, updata_pkt_back, 18, 0);
				else			
					UART_PutArray(UART0, updata_pkt_back, 18);        
				break;						
			}
			else if (pkt->dataflag == 0x08)  //F4        �ز����ڵ��ַ
			{
GET_DEVICE_ADDR_F4:			
				memcpy(updata_pkt_back, pkt->data, 13);                                          
				updata_pkt_back[1] = 21; 	//������     
				updata_pkt_back[3] = 0x8a; 	//������   
				
				I2cRead(0xA0, &updata_pkt_back[13], FM_DEVICE_ADDR, 6); //ȡ����ͨ�Ų���***
				updata_pkt_back[19]=Check_SUM(&updata_pkt_back[3], 16); //У���	                 
				updata_pkt_back[20]=0x16;                                                                                

				if(bEZMACDebugType)
					SendPacketToEzmacProc(pEZMACRecvPacket, updata_pkt_back, 21, 0);
				else											
					UART_PutArray(UART0, updata_pkt_back, 21);    	
				break;					
			}				
			else if (pkt->dataflag == 0x10)  //F5          �ز����ڵ�״̬�ֺ��ز�����                   
			{
GET_DEVICE_STATUS_F5:	
				if(bEZMACDebugType)
					SendPacketToEzmacProc(pEZMACRecvPacket, statuspkt, 0x13, 0);
				else	
				{					
					if(bDebugSend)				
						UART_PutArray(UART0, statuspktForDebug, 0x13);	
					else
						UART_PutArray(UART0, statuspkt, 0x13);	
				}
				break;						
			}
			else if (pkt->dataflag == 0x40)  //F7 ��ѯ����ͨ�Ų���                             
			{       
GET_SZ_COMM_PARA_F7:				
				memcpy(updata_pkt_back, pkt->data, 13); 
				updata_pkt_back[1] = 23; //������
				updata_pkt_back[3] = 0x8a; 	//������    
				I2cRead(0xA0, &updata_pkt_back[13], FM_NETPARA_ADDR, 8); //ȡ����ͨ�Ų���***
				updata_pkt_back[21]=Check_SUM(&updata_pkt_back[3], 18); //У���
				updata_pkt_back[22]=0x16;
				if(bEZMACDebugType)
					SendPacketToEzmacProc(pEZMACRecvPacket, updata_pkt_back, 23, 0);
				else	
					UART_PutArray(UART0, updata_pkt_back, 23);
				break;					
			}	
			else if (pkt->dataflag == 0x80)  //F8  ��ѯƵ�㶨��                     
			{            
GET_SZ_FREQ_PARA_F8:
                m = 0;
				memcpy(updata_pkt_back, pkt->data, 13); 
                
                memcpy(updata_pkt_back + 4, (uint8 *)&stinfo, 6);
                m = 13;
                
				//updata_pkt_back[1] = 22; //������
				updata_pkt_back[3] = 0x8a; 	//������    
				//I2cRead(0xA0, &updata_pkt_back[m], FM_CHANNEL_PARA, 2);
                memcpy(updata_pkt_back+m, (uint8 *)&rfpara, 2);
                m+=2;
				updata_pkt_back[m]=Check_SUM(&updata_pkt_back[3], m-3); //У���
				m++;
				updata_pkt_back[m++]=0x16;
                updata_pkt_back[1] = m & 0xff; //������ 
                updata_pkt_back[2] = m >> 8;
				if(bEZMACDebugType)
					SendPacketToEzmacProc(pEZMACRecvPacket, updata_pkt_back, m, 0);
				else					
					UART_PutArray(UART0, updata_pkt_back, m);
				break;						
			}	
			else if (pkt->dataflag == 0x101)  //F9  ��ѯ�ŵ�����                
			{          
GET_CHANNEL_PARA_F9:					
                m = 0;
				memcpy(senduppkt.data, pkt->data, 13); 
                senduppkt.data[3] = 0x8a;  //������    
                memcpy(senduppkt.data + 4, (uint8 *)&stinfo, 6);
                m = 13;
                temp = pkt->data[14];//����
                ret = 0xFFFF;
                i = getWirelessBaudRate(getSF(), getBW(), getErrorCoding());
                if(i != 0)
                {
                    ret = (temp * 8) / i;
                    ret = (ret == 0) ? 1: ret;
                }
                memcpy(senduppkt.data + m, (uint8 *)&ret, 2);
                m+=2;
                
				memcpy(senduppkt.data + m, pkt->data + 13, temp + 2); 
                m += (temp + 2);
				senduppkt.data[m]=Check_SUM(&senduppkt.data[3], m-3); //У���
				m++;
				senduppkt.data[m++]=0x16;
                senduppkt.data[1] = m & 0xff; //������ 
                senduppkt.data[2] = m >> 8;
				if(bEZMACDebugType)
					SendPacketToEzmacProc(pEZMACRecvPacket, senduppkt.data, m, 0);
				else						
					UART_PutArray(UART0, senduppkt.data, m);
				break;				
			} 
			else if (pkt->dataflag == 0x102)  //F10  ��ѯ·�ɲ���                          
			{
                uint16 tmp;
GET_ROUTE_PARA_F10:
				memcpy(updata_pkt_back, pkt->data, 13); 
                memcpy(updata_pkt_back + 4, (uint8 *)&stinfo, 6);
                m = 13;
				//updata_pkt_back[1] = 18; //������       
				updata_pkt_back[3] = 0x8a; 	//������    
				//I2cRead(0xA0, &updata_pkt_back[13], FM_CLTP_ADDR, 3);
				updata_pkt_back[m++] = 0x73;//��·�ɣ���Ҫ�·��ӽڵ�
				updata_pkt_back[m++] = 0x28;//
				updata_pkt_back[m++] = (getChannelNum() - 1);//�ŵ�����
				updata_pkt_back[m++] = 0x00 ;//
				updata_pkt_back[m++] = SX127X_FREQ;//��������Ƶ��
				updata_pkt_back[m++] = cltparm.routeuptime;//
				//�ӽڵ�����ʱ
				tmp = (TIME_OUT_MAX * MAX_DATA_TIME_LIVE * SYS_TIMER_COUNT_MS * 2) / 1000;
                tmp = (tmp == 0) ? 1 : tmp;
				updata_pkt_back[m++] = tmp & 0xff;//
				updata_pkt_back[m++] = tmp >> 8;// 
				//�㲥��ʱ
				tmp = (FULLTIME * SYS_TIMER_COUNT_MS) / 1000;
                tmp = (tmp == 0) ? 1 : tmp;
				updata_pkt_back[m++] = tmp & 0xff;//
				updata_pkt_back[m++] = tmp >> 8;//   
				//1376.2����ĳ���
				updata_pkt_back[m++] = 0x00;//
				updata_pkt_back[m++] = 0x01;//  
				//1376.2�ļ�����֧�ֵ���󵥸����ݰ�����
				updata_pkt_back[m++] = 0xC0;//
				updata_pkt_back[m++] = 0x00;// 
				updata_pkt_back[m++] = 0x05;//���������ȴ�ʱ��
				memcpy(updata_pkt_back + m, (uint8 *)nDeviceMacAddr, 6);
                m+=6;
                //���ӽڵ�����
                nMeterNum = rfpara.rf_slotnum - 1;
//	                updata_pkt_back[m++] = 0x00;//
//					updata_pkt_back[m++] = 0x06;// 
				memcpy(updata_pkt_back + m, &nMeterNum, 2);
                m+=2;
				//��ǰ�ӽڵ�����
				
				nMeterNum = Meter_Num();
                memcpy(updata_pkt_back + m, &nMeterNum, 2);
                m+=2;
//					updata_pkt_back[m++] = 0x00;//
//					updata_pkt_back[m++] = 0x04;// 
				//ͨ��ģ��ʹ��Э�鷢������
				updata_pkt_back[m++] = 0x12;//
				updata_pkt_back[m++] = 0x12;// 
				updata_pkt_back[m++] = 0x05;//		
				//ͨ��ģ����󱸰����ڷ�������
				updata_pkt_back[m++] = 0x12;//
				updata_pkt_back[m++] = 0x12;// 
				updata_pkt_back[m++] = 0x05;//
				//ͨ��ģ�鳧��

				memcpy(updata_pkt_back + m, (uint8 *)&sVersiong, 9);
                m+=9;
				updata_pkt_back[m++] = 0x00;// 
				updata_pkt_back[m++] = 0x00;//
                
				updata_pkt_back[m]=Check_SUM(&updata_pkt_back[3], m-3); //У���
				m++;
				updata_pkt_back[m++]=0x16;
                updata_pkt_back[1] = m & 0xff; //������ 
                updata_pkt_back[2] = m >> 8;
				if(bEZMACDebugType)
					SendPacketToEzmacProc(pEZMACRecvPacket, updata_pkt_back, m, 0);
				else						
					UART_PutArray(UART0, updata_pkt_back, m);         
				break;					
			} 	                                                                                                 
			else if (pkt->dataflag == 0x104)  //F11  ��ѯ����ַ_����ָ��                              
			{      		
METER_ADDR_START_CMD_F11:					
				i=0;
				Search_Addr(i);  			
				break;					
			} 	
			else if (pkt->dataflag == 0x108)  //F12 ��ѯ����ַ_����ָ��                              
			{    
METER_ADDR_TEANS_CMD_F12:	
				p=pkt->data;
//					Report_Unhandled_Addr(p);
				break;					
			} 
			else if (pkt->dataflag == 0x110)  //F13  ��ѯ�ɼ�����Ϣ_����ָ��                              
			{    
SS_INFO_START_CMD_F13:				
				p=pkt->data;
//					Search_SS(p);
				break;					
			} 
			else if (pkt->dataflag == 0x120)  //F14  ��ѯ�ɼ�����Ϣ_����״ָ̬��                              
			{    
SS_INFO_TRANS_CMD_F14:			
//					Check_SS_Status();
				break;					
			} 
			else if (pkt->dataflag == 0x140)  //F15  ��ȡ�Ѵ����� _����ָ��
			{    
METER_HANDLE_START_CMD_F15:			
				i=1;
				Search_Addr(i);  	
				break;					
			} 
			else if (pkt->dataflag == 0x180)  //F16  ��ȡ�Ѵ�����  _����ָ��
			{    
METER_HANDLE_TRANS_CMD_F16:		
				handled_no=pkt->data[13]|(pkt->data[14]<<8);    //�������Ѵ������
//					Report_Haneled_Addr(handled_no);
				break;					
			} 
			else if (pkt->dataflag == 0x201)	 //F17��ѯ�ɼ�������ʹ�ܲ���
			{		
GET_SS_UPDATA_PARA_F17:
				memcpy(updata_pkt_back, pkt->data, 13);                                          
				updata_pkt_back[1] = 16; //������    
				updata_pkt_back[3] = 0x8a; 	//������    
//					updata_pkt_back[13]= adpkt.subtype.SSCodeUpdataEnable;
				updata_pkt_back[14]=Check_SUM(&updata_pkt_back[3], 11); //У���	                 
				updata_pkt_back[15]=0x16;                                                                                
				if(bEZMACDebugType)
					SendPacketToEzmacProc(pEZMACRecvPacket, updata_pkt_back, 16, 0);
				else						
					UART_PutArray(UART0, updata_pkt_back, 16);    
				break;					
			}	
			else if (pkt->dataflag == 0x202)	 //F18��ѯ����ϻ�����
			{		
GET_METER_DEAD_COUNT_F18:			
				memcpy(updata_pkt_back, pkt->data, 13);                                          
				updata_pkt_back[1] = 17; 	//������    
				updata_pkt_back[3] = 0x8a; 	//������    
				updata_pkt_back[13] = MeterDeadCount;
				updata_pkt_back[14] = 0;	
				updata_pkt_back[15]=Check_SUM(&updata_pkt_back[3], 11); //У���	                 
				updata_pkt_back[16]=0x16;                                                                                
				if(bEZMACDebugType)
					SendPacketToEzmacProc(pEZMACRecvPacket, updata_pkt_back, 17, 0);
				else						
					UART_PutArray(UART0, updata_pkt_back, 17);        
				break;					
			}	
			else if (pkt->dataflag == 0x204)	 //F19 ��ѯ�ɼ�����֤ʹ�ܲ���
			{	
GET_SS_VALIDATE_PARA_F19:					
				memcpy(updata_pkt_back, pkt->data, 13);                                          
				updata_pkt_back[1] = 16; 	//������        
				updata_pkt_back[3] = 0x8a; 	//������    
				updata_pkt_back[13] = bSSValidateEnable;	
				updata_pkt_back[14]=Check_SUM(&updata_pkt_back[3], 11); //У���	                 
				updata_pkt_back[15]=0x16;                                                                                
				if(bEZMACDebugType)
					SendPacketToEzmacProc(pEZMACRecvPacket, updata_pkt_back, 16, 0);
				else						
					UART_PutArray(UART0, updata_pkt_back, 16);                    				
				break;					
			}	
			else if (pkt->dataflag == 0x208)	 //F20 ��ѯ��֤�Ĳɼ�����Ϣ_����ָ��
			{	
SS_VALIDATE_START_CMD_F20:
				Search_Validate_SS(pkt->data);
				break;					
			}

			else if (pkt->dataflag == 0x210)	 //F21��ѯ������ʾ����ʾ������Դ
			{		
GET_TUOPU_DISPLAY_PARA_F21:				
				memcpy(updata_pkt_back, pkt->data, 13);                                          
				updata_pkt_back[1] = 16; 	//������   
				updata_pkt_back[3] = 0x8a; 	//������    
				updata_pkt_back[13] = u8TuopuDisplaySource;	
				updata_pkt_back[14]=Check_SUM(&updata_pkt_back[3], 11); //У���	                 
				updata_pkt_back[15]=0x16;                                                                                
				if(bEZMACDebugType)
					SendPacketToEzmacProc(pEZMACRecvPacket, updata_pkt_back, 16, 0);
				else			
					UART_PutArray(UART0, updata_pkt_back, 16);     
				break;					
			}	
			else if (pkt->dataflag == 0x220)	 //F22��ѯ����ģ��ĳ������
			{		
GET_BS_INFO_PARA_F22:
				memcpy(updata_pkt_back, pkt->data, 13);                                          
				updata_pkt_back[1] = 31; 	//������    
				updata_pkt_back[3] = 0x8a; 	//������    
				I2cRead(0xA0, (uint8*) &updata_pkt_back[13], FM_BS_INFO, 16); //����ģ��ĳ������
				updata_pkt_back[29]=Check_SUM(&updata_pkt_back[3], 26); //У���	                 
				updata_pkt_back[30]=0x16;                                                                                
				if(bEZMACDebugType)
					SendPacketToEzmacProc(pEZMACRecvPacket, updata_pkt_back, 31, 0);
				else							
					UART_PutArray(UART0, updata_pkt_back, 31);      
				break;					
			}	

			else if (pkt->dataflag == 0x240)	 //F23 ��ѯ������ͬ��ʹ�ܲ���
			{	
GET_METER_INFO_SYN_PARA_F23:
				memcpy(updata_pkt_back, pkt->data, 13);                                          
				updata_pkt_back[1] = 16; 	//������                  
				updata_pkt_back[3] = 0x8a; 	//������    
				updata_pkt_back[13] = bMeterInfoSynEnable;	
				updata_pkt_back[14]=Check_SUM(&updata_pkt_back[3], 11); //У���	                 
				updata_pkt_back[15]=0x16;                                                                                
				if(bEZMACDebugType)
					SendPacketToEzmacProc(pEZMACRecvPacket, updata_pkt_back, 16, 0);
				else							
					UART_PutArray(UART0, updata_pkt_back, 16);               
				break;					
			}	
			else if (pkt->dataflag == 0x280)	 //F24��ѯI�Ͳɼ�����������ʹ�ܲ���
			{		
GET_BASE_UNDATA_PARA_F24:			
				memcpy(updata_pkt_back, pkt->data, 13);                                          
				updata_pkt_back[1] = 16; //������        
				updata_pkt_back[3] = 0x8a; 	//������    
//					updata_pkt_back[13]= adpkt.subtype.I_SSCodeUpdataEnable;
				updata_pkt_back[14]=Check_SUM(&updata_pkt_back[3], 11); //У���	                 
				updata_pkt_back[15]=0x16;                                                                                
				if(bEZMACDebugType)
					SendPacketToEzmacProc(pEZMACRecvPacket, updata_pkt_back, 16, 0);
				else							
					UART_PutArray(UART0, updata_pkt_back, 16);       
				break;					
			}
			else if (pkt->dataflag == 0x301)	 //F25��ѯƵ�β���
			{		
GET_FREQ_CHANNEL_PARA_F25:			
				memcpy(updata_pkt_back, pkt->data, 13);                                          
				updata_pkt_back[1] = 17; //������      
				updata_pkt_back[3] = 0x8a; 	//������    
				updata_pkt_back[13]= nFreqChannel;
				updata_pkt_back[14]= rfpara.rf_channel;
				updata_pkt_back[15]=Check_SUM(&updata_pkt_back[3], 12); //У���	                 
				updata_pkt_back[16]=0x16;                                                                                
				if(bEZMACDebugType)
					SendPacketToEzmacProc(pEZMACRecvPacket, updata_pkt_back, 17, 0);
				else							
					UART_PutArray(UART0, updata_pkt_back, 17);       
				break;					
			}
			else if (pkt->dataflag == 0x302)	 //F26��ѯ�ŵ�����ģʽ����
			{		
GET_CHANNEL_SET_MODE_F26:				
				memcpy(updata_pkt_back, pkt->data, 13);                                          
				updata_pkt_back[1] = 22; //������   
				updata_pkt_back[3] = 0x8a; 	//������    
				updata_pkt_back[13]= bChannelSetManualMode;
				updata_pkt_back[14] = 0x00;
				updata_pkt_back[15] = 0x00;
				updata_pkt_back[16] = 0x00;
				updata_pkt_back[17] = 0x00;
				/*if(FREQ_481M == nFreqChannel)
				{
					updata_pkt_back[14] = 0x04;
					updata_pkt_back[15] = 0x33;
				}
				else */
                if(FREQ_470M == nFreqChannel)
				{
					updata_pkt_back[14] = 0x04;
					updata_pkt_back[15] = 0x70;					
				}
                /*
				else if(FREQ_500M == nFreqChannel)
				{
					updata_pkt_back[14] = 0x05;
					updata_pkt_back[15] = 0x00;					
				}	
				else if(FREQ_491M == nFreqChannel)
				{
					updata_pkt_back[14] = 0x04;
					updata_pkt_back[15] = 0x91;					
				}*/
				updata_pkt_back[18]= rfpara.rf_channel;
				updata_pkt_back[19]= 0;
				updata_pkt_back[20]=Check_SUM(&updata_pkt_back[3], updata_pkt_back[1] - 5); //У���	                 
				updata_pkt_back[21]=0x16;                                                                                
				if(bEZMACDebugType)
					SendPacketToEzmacProc(pEZMACRecvPacket, updata_pkt_back, updata_pkt_back[1], 0);
				else								
					UART_PutArray(UART0, updata_pkt_back, updata_pkt_back[1]);       
				break;					
			}

			else if (pkt->dataflag == 0x308)	 //F28����ѯ�����������������
			{		
GET_JC_DISALLOW_FALG_F28:			
				memcpy(updata_pkt_back, pkt->data, 13);                                          
				updata_pkt_back[1] = 16; //������       
				updata_pkt_back[3] = 0x8a; 	//������    
				updata_pkt_back[13]= bJcDisallowFalg;
				updata_pkt_back[14]=Check_SUM(&updata_pkt_back[3], 11); //У���	                 
				updata_pkt_back[15]=0x16;                  
				if(bEZMACDebugType)
					SendPacketToEzmacProc(pEZMACRecvPacket, updata_pkt_back, 16, 0);
				else							
					UART_PutArray(UART0, updata_pkt_back, 16);       
				break;	
			}
			else if(pkt->dataflag == 0x310)	//F29����ѯ�ӽڵ�����ע�����
			{
GET_BOARD_SEARCH_METER_FALG_F29:			
				memcpy(updata_pkt_back, pkt->data, 13);                                          
				updata_pkt_back[1] = 25; //������     
				updata_pkt_back[3] = 0x8a; 	//������    
				memset(updata_pkt_back + 13, 0, 6);
				memcpy(updata_pkt_back + 19, (uint8*)&nBroadMeterTime, 2);
				updata_pkt_back[21] = 0;
				updata_pkt_back[22] = bBroadMeterEnable;
				updata_pkt_back[23]=Check_SUM(&updata_pkt_back[3], 20); //У���	                 
				updata_pkt_back[24]=0x16;                  
				if(bEZMACDebugType)
					SendPacketToEzmacProc(pEZMACRecvPacket, updata_pkt_back, 25, 0);
				else							
					UART_PutArray(UART0, updata_pkt_back, 25);  
				break;	
			}
			else if (pkt->dataflag == 0x1001)	 //F128��ѯ����
			{	
				p=(uint8 *)updata_code_cache;
                
                m=0;
				//extern uint32 ExtCtrlDeadCont;
				//-------------------�����б���------------------------
				memcpy(p, &g_stUpgrade, sizeof(STUPGRADE));		
                m+=sizeof(STUPGRADE);
				memcpy(p, &ExtCtrlDeadCont, 4);		
                m+=4;
                Compose_3762Data(0x80, p, m, NULL, 0, &stinfo, pkt->afn, pkt->dataflag);

	
				break;
			}
			else if(pkt->dataflag == 0x1E80)	//F248���ڿذ汾��
            {
				memcpy(updata_pkt_back, pkt->data, 13); 
                memcpy(updata_pkt_back + 4, (uint8 *)&stinfo, 6);
                m = 13;
				//updata_pkt_back[1] = 18; //������       
				updata_pkt_back[3] = 0x8a; 	//������    
	
				memcpy(updata_pkt_back + m, (uint8 *)ucModeVer, 20);

                updata_pkt_back[m + 8] = (sVersiong.swCode[0] / 0x10) + '0';
                updata_pkt_back[m + 9] = (sVersiong.swCode[0] % 0x10) + '0';

                updata_pkt_back[m + 11] = (sVersiong.yy / 0x10) + '0';
                updata_pkt_back[m + 12] = (sVersiong.yy % 0x10) + '0';
                updata_pkt_back[m + 13] = (sVersiong.mm / 0x10) + '0';
                updata_pkt_back[m + 14] = (sVersiong.mm % 0x10) + '0';
                updata_pkt_back[m + 15] = (sVersiong.dd / 0x10) + '0';
                updata_pkt_back[m + 16] = (sVersiong.dd % 0x10) + '0';

                
                m+=20;

                
				updata_pkt_back[m]=Check_SUM(&updata_pkt_back[3], m-3); //У���
				m++;
				updata_pkt_back[m++]=0x16;                                                                                
	
                updata_pkt_back[1] = m & 0xff; //������ 
                updata_pkt_back[2] = m >> 8;
				if(bEZMACDebugType)
					SendPacketToEzmacProc(pEZMACRecvPacket, updata_pkt_back, m, 0);
				else
					UART_PutArray(UART0, updata_pkt_back, m);	
				break;			

            }         
			else if (pkt->dataflag == 0x1002)	 //F129 RSSI ����
			{		

                m = 0;
				memcpy(updata_pkt_back, pkt->data, 13); 
                
                memcpy(updata_pkt_back + 4, (uint8 *)&stinfo, 6);
                m = 13;
                
				//updata_pkt_back[1] = 22; //������
				updata_pkt_back[3] = 0x8a; 	//������    
				//I2cRead(0xA0, &updata_pkt_back[m], FM_CHANNEL_PARA, 2);
                memcpy(updata_pkt_back+m, (uint8 *)&rfpara.rf_limit, 1);
                m+=1;
				updata_pkt_back[m]=Check_SUM(&updata_pkt_back[3], m-3); //У���
				m++;
				updata_pkt_back[m++]=0x16;
                updata_pkt_back[1] = m & 0xff; //������ 
                updata_pkt_back[2] = m >> 8;
				if(bEZMACDebugType)
					SendPacketToEzmacProc(pEZMACRecvPacket, updata_pkt_back, m, 0);
				else					
					UART_PutArray(UART0, updata_pkt_back, m);
                
				break;
			}
			if(0 == bEZMACDebugType	)
				Uart_Status = UART_STATUS_IDLE;
			break;
		}
		case AFN_TYPE_CTLCMD:    //��������                    
		{

				//F3ȷ�Ϲ�Լ���϶�!! //by peter				
			if (pkt->dataflag == 0x4)   //F3 �����㲥                  
			{
START_ADV_F3:			
				temp = (*(pkt->data + 13)) ; //��ȡ������ 
			//	if((0 == temp) && (pkt->data[14] <= 8) && (adv_subtype != ADV_TYPE_TRANSMISSION)) //͸��ת��
				if((0 == temp) && (pkt->data[14] <= 128) && (adv_subtype == ADV_TYPE_EMPTY)) //͸��ת��			
				{			
    				adv_sublen = pkt->data[14];	
    				memcpy(adv_Trans_pkt, pkt->data + 15, adv_sublen);							
    				adv_subtype = ADV_TYPE_TRANSMISSION;   
    				adv_updatetime = curslottime + FULLTIME;

    				SendReturnpkt(0, &stinfo);
				}
                else
                {
                    SendReturnpkt(1, &stinfo);
                }
				break;
			}              
			if (pkt->dataflag == 0x01)  //F1  
			{		
SET_DEVICE_ADDR_F1:
				bError = 0;
				I2cRead(0xA0, &updata_pkt_back[0], FM_DEVICE_ADDR, 6); 
				if(LPC_Memory_CMP(updata_pkt_back, &pkt->data[13], 6))
				{
				    if(nDeviceMacAddr[0] == 0xFF)
                    {
                        uint8 channelId = Cal_Hash_Value(&pkt->data[13]) % getChannelNum();
                        if(channelId == 0)
                        {
                            channelId = 1;
                        }                  
                        
                        //rfpara.rf_channel = channelId; //�ŵ���
                        
                        I2cWrite(0xA0, (uint8*)&channelId, FM_NETPARA_ADDR, 1); //��ȡ�������    
                    }
                    uint8 addrLen = 6;
                    I2cWrite(0xA0, &addrLen, FM_DEVICE_ADDR_LEN, 1);
                    
					I2cWrite(0xA0, &pkt->data[13], FM_DEVICE_ADDR , 6);
					I2cRead(0xA0, nDeviceMacAddr, FM_DEVICE_ADDR, 6); 
					bError = 1;
				}

				SendReturnpkt(0, &stinfo);

				if((bError) && (0 == bChannelSetManualMode))
				{
					//��ʱ200MS
					DelayNS(100);
				//	Delay_ms(200);
//					Reset(); //����		
                    nChangeChannelTime = curslottime + MAX_SLOT_TIME * 10;
                    bNeedChangeChannel = TRUE;
                
    			}
				break;				
			}
			if (pkt->dataflag == 0x2)	//F2����/��ֹ�ӽڵ������ϱ�
    		{
                dis[0] = 0xC7;
    			if (pkt->data[13] == 0x1)//����
                {
                }
                else// if(pkt->data[13] == 0x0)
                {
                    dis[0] = 0x40;
                    //bError = EZMacPRO_Transmit_Adv(2, dis, 1);//��ֹ�ϱ�
                }
                bError = EZMacPRO_Transmit_Adv(2, dis, 1);//ȫ������

    			SendReturnpkt(!bError, &stinfo);
    			break;
    		}
			if (pkt->dataflag == 0x8)	//F4��������ͨ�Ų���
			{
SET_SZ_COMM_PARA_F4:	
				if(pkt->data[13] > 16)
					pkt->data[13] = 0;	
				bError = 1;
				if(0 == pkt->data[13])
					bError = 0;
				if(bError != bChannelSetManualMode)
				{
					bChannelSetManualMode = bError;
					I2cWrite(0xA0,  &bChannelSetManualMode, FM_CHANNEL_SET_MODE, 1);      					
				}
						  	
				pkt->data[16] = pkt->data[13];
				pkt->data[17] = 0;
				I2cWrite(0xA0, &pkt->data[13], FM_NETPARA_ADDR, 8); //���������***
				
				SendReturnpkt(0, &stinfo);
				
				//��ʱ200MS
				DelayNS(100);
			//	Delay_ms(200);
			//	Reset(); //����
				break;
			}					
			if (pkt->dataflag == 0x10)	//F5��������ͨ�Ų���
            {
                uint8 channelId;

SET_SZ_FREQ_PARA_F5:	  
    
				if(((pkt->data[13] >= 1 && pkt->data[13] <= 6) || pkt->data[13] == 254 || pkt->data[13] == 255)
                    && (pkt->data[14] < 4))//�ж���ȷ��
				{ 
                    //I2cRead(0xA0, (uint8*)&rfpara, FM_NETPARA_ADDR, 3); //��ȡ������� 
                    if(pkt->data[13] == 254)
                    {
                        //extern uint8 nDeviceMacAddr[6];
                        channelId = Cal_Hash_Value(nDeviceMacAddr) % getChannelNum();
                        if(channelId == 0)
                        {
                            channelId = 1;
                        }                  
                        //rfpara.rf_channel = channelId;

                    }
                    else if(pkt->data[13] == 255)
                    {
                        //����
                        if(ES_LOG_OCStt())
                        {
                            ES_LOG_OC(0, LOG_LEVEL_DEBUG);
                        }
                    }
                    else
                    {
                    
                        channelId  = pkt->data[13];

                    }
                    
                    rfpara.rf_power = pkt->data[14];

                    if(channelId != rfpara.rf_channel)
                    {
                        nChangeChannelTime = curslottime + MAX_SLOT_TIME * 10;
                        bNeedChangeChannel = TRUE;

                        for(i = SUP_SS_INDEX_START; i < MAX_SUP_SS_NUM+1; i++)  //����
                        {
                            if(cltor[i].devAddr[0] < 0xFF)//
                            {
                                Delete_SS_Node(i);          
                            }
                        }
                        I2cWrite(0xA0, (uint8*)&channelId, FM_NETPARA_ADDR, 1);
                    }
                    //rfpara.rf_channel = channelId;
				    I2cWrite(0xA0, (uint8*)&rfpara.rf_power, FM_NETPARA_ADDR+1, 1); //���������***	
//						if ((pkt->data[14]>0x89)||(pkt->data[14]<0x81))//��ʼƵ�㲻�ܴ���489MHZ��С��481MHZ
//							return;
				    //I2cWrite(0xA0, &pkt->data[13], FM_FREQ_DEFINE, 7); //������Ƶ�㶨�����***	 	 	 // 	 	 
					SendReturnpkt(0, &stinfo);

					//��ʱ200MS
					DelayNS(100);
				//	Delay_ms(200);
					//Reset(); //����					
				}
				else
                {            
                    
                    if(ES_LOG_OCStt())
                    {
                        ES_LOG_OC(0, LOG_LEVEL_DEBUG);
                    }
                    else
                    {
                        ES_LOG_OC(1, LOG_LEVEL_DEBUG);
                    }
                    stinfo.stUp.bit2.reverse = ES_LOG_OCStt();
                    SendReturnpkt(1, &stinfo);
                }
				break;
			}
				
					 //������ȷ�Լ��!!!! //by peter		
			if (pkt->dataflag == 0x20)	//F6�����ŵ�����
		    {
SET_CHANNEL_PARA_F6:
//					if(pkt->data[13]==16)
//					{
//						if ((gab+pkt->data[14] +31)>99)//Ƶ�����
//							return;//�޷��أ��������Զ���ʾ����ʧ��
//						else if(pkt->data[14]<16)//ҵ����ʼƵ��С�ڹ㲥Ƶ��
//							return;//�޷��أ��������Զ���ʾ����ʧ��
//						pkt->data[13]=rfpara.rf_channel;	//��ǰ�ŵ���
//					//	I2cWrite(0xA0, &pkt->data[13], (FM_CHANNEL_PARA+((rfpara.rf_channel-1)<<2)), 4); //����
//					//	data_fhc=gab+pkt->data[14];//�������޸Ĺ���ҵ����ʼ
//					}
//					else//����15���ŵ�
//					{
//						temp=(pkt->data[13]-1)<<2;//ƫ��ֵ
//						pkt->data[14]=20;//Ƶ����ʼֵ
//						pkt->data[15]=50;//Ƶ����ֵֹ
//						pkt->data[16]=31;//Ƶ�����
//					//	I2cWrite(0xA0, &pkt->data[14], (FM_CHANNEL_PARA+temp), 3);
//					}
                memcpy(&handled_no, &pkt->data[13], 2);

                if(handled_no > MAX_SUP_SS_NUM)
                {
                    bError = 1;
                }
                else
                {   
                    if(handled_no != rfpara.rf_slotnum)
                    {
                        rfpara.rf_slotnum = handled_no;
			            I2cWrite(0xA0, (uint8*)&rfpara.rf_slotnum, FM_NETPARA_ADDR+8, 2); //���������***	
                    }
                }


				SendReturnpkt(bError, &stinfo);
				break;
			}

				
			//������ȷ�Լ��!!!! //by peter		
			if (pkt->dataflag == 0x40)	//F7����·�ɲ���
	      	{
SET_ROUTE_PARA_F7:	
				I2cWrite(0xA0, &pkt->data[13], FM_CLTP_ADDR , 3); //������Ƶ�㶨�����***
				SendReturnpkt(0, &stinfo);
				
				//��ʱ200MS
				DelayNS(100);
			//	Delay_ms(200);                                         
				Reset(); //����
				break;
			}
			if (pkt->dataflag == 0x80)	//F8���òɼ�������ʹ�ܲ���
	      	{
SET_SS_UPDATA_PARA_F8:    	
				if(pkt->data[13]  > 1)
					pkt->data[13]  = 1;
//					if(adpkt.subtype.SSCodeUpdataEnable !=  pkt->data[13])
				{
//						adpkt.subtype.SSCodeUpdataEnable= pkt->data[13];		 
					I2cWrite(0xA0, &pkt->data[13], FM_SS_CODEUPDATA_ENABLE , 1); //������Ƶ�㶨�����*
				}
				SendReturnpkt(0, &stinfo);
				break;
			}

			if(pkt->dataflag == 0x101)  //F9���õ���ϻ�ʱ��
			{
SET_METER_DEAD_COUNT_F9:	
				if((pkt->data[13] > 0) && (pkt->data[13] != MeterDeadCount))
				{
					MeterDeadCount = pkt->data[13];							
					I2cWrite(0xA0,  &MeterDeadCount, FM_METER_DEAD_COUNT, 1);     
				}
				SendReturnpkt(0, &stinfo);
				break;
			}
			if(pkt->dataflag == 0x102) //F10���ù㲥������
			{
SET_BROAD_METER_DATA_F10:			
				if((0 == bEZMACDebugType) && (0 == bBroadMeterEnable))
				{
					if((bBroadStart != pkt->data[13]) && (0 == bBroadStartTriger))
					{
						bBroadStartTriger = 1;
						if(pkt->data[13])
						{
							if(20 == pkt->len) //�㲥���������д����ݱ�ʶ
							{
								//��¼���ݱ�ʶ
								bHaveJCDataFlag = 1;
								memcpy(nJCDataFlag, &pkt->data[14], 4);
							}
							else
								bHaveJCDataFlag = 0;
						}
						u32BroadStartTrigerTime = curslottime + BROAD_METER_DATA_CMD_TRIGER;
					}
					if(bDebugSend)			
						UART_PutArray(UART0, informpktForDebug, 0x13); 					
					else
						UART_PutArray(UART0, informpkt, 0x13); //ȷ�ϻش�
				}
				break;
			}
				
			if(pkt->dataflag == 0x104) //F11���õ��ά������(�����㲥���)
			{
SET_BROAD_SEARCH_METER_F11:		
				if((0 == bEZMACDebugType) && (0 == bBroadMeterEnable))
				{
					if(bMeterSearchStart != pkt->data[13])
					{									
						bMeterSearchStart = pkt->data[13];
						if(0 == bMeterSearchStart)	//������ϻ��Ĵ���
						{
							if(0 == HASH_Table_Used)
								pHasht = htable1;
							else
								pHasht = htable2;
							
							for(loop = 0; loop < MAX_HASH_TABLE_FLASH_SIZE; loop++)
							{
								if((pHasht[loop].status == OCCUPIED) && (0 == MeterStatus[loop].bReceive))
								{
									if(shadow_space[loop].times <= 1)
									{
								
										MeterStatus[loop].bNeedDelete = 1;	
										MeterStatus[loop].bDeleteWaitAck = 0;
										bNeedDeleteMeter = 1; 
									}
									else
									{
										if(shadow_space[loop].times > MeterDeadCount)
											shadow_space[loop].times = MeterDeadCount;
										shadow_space[loop].times--;; 		//�ϻ���������1
										I2cWrite(0xA0,(uint8*)&shadow_space[loop], loop, 1);
									}
								}
								MeterStatus[loop].bAck = 0;
								MeterStatus[loop].bReceive = 0;
								MeterStatus[loop].bWaitAck = 0;
							}

							bMeterDeadStart = 1;
							bCltorSearchStart = 1;
							bNeedMeterDeleteAck = 0;
							bWaitForSendMeterAck = 0;
							if(adv_subtype == ADV_TYPE_ADDR_SEARCH)
							{
								adv_subtype = ADV_TYPE_EMPTY;
								adv_sublen = ADV_SUB_LEN;
							}
						}
						else				//��������ѯ
						{
							for(loop = 0; loop < MAX_HASH_TABLE_FLASH_SIZE; loop++)
							{
								MeterStatus[loop].bNeedDelete = 0;
								MeterStatus[loop].bDeleteWaitAck= 0;
								
								MeterStatus[loop].bAck = 0;
								MeterStatus[loop].bReceive = 0;
								MeterStatus[loop].bWaitAck = 0;									
								MeterStatus[loop].bSingleSSSearchMeterReceive = 0;
							}
							bNeedMeterDeleteAck = 0;		
							bNeedDeleteMeter = 0;								

							bMeterDeadStart = 0;
							bCltorSearchStart = 0;
							
							//ȡ���Ե����ɼ������в�������
							bSingleSearchSSSendMeter = 0;
							bSingleSearchSSMeterSended = 0;
							bSingleSearchSSSendMeterWaitAck = 0;
							
							adv_subtype = ADV_TYPE_ADDR_SEARCH; //����ѯ      
							adv_updatetime = curslottime + BROAD_METER_SEARCH_TIME;
							adv_sublen = 0;	
						}
					}
					if(bDebugSend)				
						UART_PutArray(UART0, informpktForDebug, 0x13);	
					else
						UART_PutArray(UART0, informpkt, 0x13); //ȷ�ϻش�	
				}
				break;
			}

			if(pkt->dataflag == 0x108)	//F12  ɨ�赥���ɼ����µĵ��
			{
SET_SINGLE_SEARCH_METER_F12:
 
				break;
			}
		
			if(pkt->dataflag == 0x110)	//F13  ���òɼ�����֤ʹ�ܲ���
			{
SET_SS_VALIDATE_PARA_F13:			
				if(pkt->data[13]  > 1)
					pkt->data[13]  = 1;	 
				if(bSSValidateEnable !=  pkt->data[13])
				{
					bSSValidateEnable = pkt->data[13];		 
					I2cWrite(0xA0, &pkt->data[13], FM_SS_VALIDATE_ENABLE , 1); //������Ƶ�㶨�����*
				}
				SendReturnpkt(0, &stinfo);	
				
				//��ʱ200MS
				DelayNS(100);				
			//	Delay_ms(200);					
				Reset(); //����
				break;
			}
	 
			if(pkt->dataflag == 0x120)	//F14:����������ʾ����ʾ������Դ
			{
SET_TUOPU_DISPLAY_PARA_F14:	
				if(pkt->data[13]  > 1)
					pkt->data[13]  = 1;	 
				if(pkt->data[13] != u8TuopuDisplaySource)
				{
					u8TuopuDisplaySource = pkt->data[13];							
					I2cWrite(0xA0,  &u8TuopuDisplaySource, FM_TUOPU_SOURCE, 1);     
				}
				
				SendReturnpkt(0, &stinfo);	
				break;
			}		
			if (pkt->dataflag == 0x140)	 //F15��������ģ��ĳ������
			{		
SET_BS_INFO_PARA_F15:		
				I2cWrite(0xA0, &pkt->data[13], FM_BS_INFO , 16);	//����ģ��ĳ������

				SendReturnpkt(0, &stinfo);	
				break;
			}

			if (pkt->dataflag == 0x180)	 //F16���òɼ����������
			{		
SET_SS_COMM_PARA_F16:				

				break;
			}
				
			if(pkt->dataflag == 0x201)	//F17  ���ò�����ͬ��ʹ�ܲ���
			{
SET_METER_INFO_SYN_PARA_F17:				
				if(pkt->data[13]  > 1)
					pkt->data[13]  = 1;	 
				if(bMeterInfoSynEnable !=  pkt->data[13])
				{
					bMeterInfoSynEnable = pkt->data[13];		 
					I2cWrite(0xA0, &pkt->data[13], FM_METER_INFO_SYN_ENABLE , 1); //������Ƶ�㶨�����*
				}
				SendReturnpkt(0, &stinfo);		
				break;
			}
			if (pkt->dataflag == 0x202)	//F18����I�Ͳɼ�����������ʹ�ܲ���
		      	{
SET_BASE_UNDATA_PARA_F18:
				if(pkt->data[13]  > 1)
					pkt->data[13]  = 1;
//					if(adpkt.subtype.I_SSCodeUpdataEnable !=  pkt->data[13])
				{
//						adpkt.subtype.I_SSCodeUpdataEnable= pkt->data[13];		 
					I2cWrite(0xA0, &pkt->data[13], FM_I_SS_CODEUPDATA_ENABLE , 1); 
				}
				SendReturnpkt(0, &stinfo);
				break;
			}
			if (pkt->dataflag == 0x204)	//F19 Ƶ���ŵ�ת������
		    {
CHANNEL_CHANGE_CMD_F19:
                nFreqChannelNum = pkt->data[13];
                nChannelChangeNum = pkt->data[14];
                if((nChannelChangeNum > 0) && (nChannelChangeNum < 18) && (nFreqChannelNum < FREQ_NUM) 
                    && (USER_DEFINE == USER_DianLi))
                {
                    if(0 == pkt->data[15])
                    {
                        if(nFreqChannelNum != nFreqChannel)
                        {
                            I2cWrite(0xA0,  &nFreqChannelNum, FM_FREQ_CHANNEL_NUM, 1);  

                            SendReturnpkt(0, &stinfo);

                            //��ʱ200MS
                            DelayNS(100);
                            //	Delay_ms(200);					
                            Reset(); //����		      				
                        }
                        else
                        {
                            SendReturnpkt(1, &stinfo);
                        }
                    }
					else
					{
                        if((0 == bSendResetCmd) && (FALSE == bNeedChangeChannel))
                        {
                            //	if(0 == bChannelSetManualMode)
                            //		nChannelChangeNum = rfpara.rf_channel;

                            if(FREQ_470M == nFreqChannelNum)
                            {
                                nBroadSendData = ((nFreqChannelNum - 1) << 5) + nChannelChangeNum;
                            }
                            else
                            {
                                nBroadSendData = (nFreqChannelNum << 5) + nChannelChangeNum;
                            }
                            nChangeChannelTime = curslottime + MAX_SLOT_TIME * 5;
                            bNeedChangeChannel = TRUE;

                            SendReturnpkt(0, &stinfo);	
                        }
                        else
                        {
                            SendReturnpkt(1, &stinfo);		
                        }
					}
                }
                else
                {            
                    SendReturnpkt(1, &stinfo);
                }
		      	break;
			}
			
			if (pkt->dataflag == 0x208)	 //F20���òɼ��������շ�ģʽ
			{		
SET_SS_UPDATA_MODE_PARA_F20:				
	
				break;
			}
			if (pkt->dataflag == 0x220)	 //F22�����ü����������������
			{
SET_JC_DISALLOW_FLAG_F22:			
				if(pkt->data[13]  > 1)
					pkt->data[13]  = 1;
				if(bJcDisallowFalg !=  pkt->data[13])
				{
					if(pkt->data[13])
						EzMacDeadCont = EZMAC_DEAD_CNT;
					bJcDisallowFalg = pkt->data[13];		 
				}
				SendReturnpkt(0, &stinfo);
				break;
			}
			if (pkt->dataflag == 0x1001)	 //F128��������
			{		
			    g_stUpgrade.flg = 0;
                
                g_stUpgrade.crc = CRC_16BIT_Check1((uint8 *)&g_stUpgrade, sizeof(STUPGRADE) - 2);
                I2cWrite(0xA0,  (uint8 *)&g_stUpgrade, DP_DOWNLOAD_PROC_PARA_ADDR, sizeof(STUPGRADE));

				SendReturnpkt(0, &stinfo);
				break;
			}
			if (pkt->dataflag == 0x1002)	 //F129 RSSI ����
			{		
                    //I2cRead(0xA0, (uint8*)&rfpara, FM_NETPARA_ADDR, 3); //��ȡ������� 
                if(rfpara.rf_limit != pkt->data[13])
                {
                    rfpara.rf_limit = pkt->data[13];
                    I2cWrite(0xA0, (uint8*)&rfpara.rf_limit, FM_NETPARA_ADDR+3, 1); 
                }
				SendReturnpkt(0, &stinfo);
				break;
			}
			break;
		}
		case AFN_TYPE_DATARELAY:   //����ת�� 
        case AFN_TYPE_ROUTEDATARELAY:
		{
            SUBPKT subpkt;
			//if (pkt->dataflag == 0x1)   //F1                             
			{
DATA_TRANS:			
				//	recvuppkt.deadtime = curslottime + MAX_DATA_TIME_LIVE * 2; // set deadtime 10s
				if(bEZMACDebugType)
				{
					if(EZMAC_Debug_Status == EZMAC_DEBUG_STATUS_IDLE)
					{
						pEZMACRecvPacket->nTTL = MAX_DATA_TIME_LIVE;
						EZMAC_Debug_Status = EZMAC_DEBUG_STATUS_BUSY;
						EZMacStaAck = PKT_TYPE_DATA;
					}
				}
				else
				{					
					//if(Uart_Status == UART_STATUS_RX_DONE)
					{			
						if(0 == bJcDisallowFalg)
						{						
							recvuppkt.deadtime = TIME_OUT_MAX * MAX_DATA_TIME_LIVE;
							Uart_Status = UART_STATUS_BUSY;	
							StaAck = PKT_TYPE_DATA;	
						}
					}					
				}

                //uint8 bHaveDAddr = 0;
                //uint8* pSourceAddr;
                uint8* pDestAddr;
                uint8 dataOffset;
                uint8 commDelayFlg = 0;
                if(pkt->afn == AFN_TYPE_DATARELAY)
                {
    				//subpkt = (SUBPKT*)(pkt->data + pkt->afnoffset + 3); //376.2�����еĹ�Լ�����ֶεĵ�ַ
    				memcpy((uint8 *)&subpkt, pkt->data + pkt->afnoffset + 3, sizeof(SUBPKT));
                    dataOffset = 5;
                }
                else
                {
                    uint8 attachNodeNum = *(pkt->data + pkt->afnoffset + 5);
                    subpkt.type = *(pkt->data + pkt->afnoffset + 3);
                    commDelayFlg = *(pkt->data + pkt->afnoffset + 4);
                    subpkt.subpktlen = *(pkt->data + pkt->afnoffset + 6 + 6 * attachNodeNum);
                    memcpy(&subpkt.startflag, pkt->data + pkt->afnoffset + 6 + 6 * attachNodeNum, 7);
                    dataOffset = 7 + 6 * attachNodeNum;
                }

				if(0 == bEZMACDebugType)
					subtype_flag = subpkt.type; //��������
				else
					EzMacSubTypeFlag = subpkt.type;
			//	uint8* pSourceAddr = pkt->data + pkt->AddrOffSet;
			//	pSourceAddr = pkt->data + pkt->AddrOffSet + 6;
			    pDestAddr = pkt->data + pkt->afnoffset - 6;
				id = 0;
//					if(pkt->AddrOffSet)
//					{
//						if(!((0 == pSourceAddr[0]) && (0 == pSourceAddr[1]) && (0 == pSourceAddr[2])
//							&& (0 == pSourceAddr[3]) && (0 == pSourceAddr[4]) && (0 == pSourceAddr[5])))
//						{
//							bHaveDAddr = 1;
//							id = Sn_Search_Id(pSourceAddr);		//add by ben  0210		
//							if ((id < 2) /*|| (id >= MAX_REAL_SUP_SS_NUM) */|| (cltor[id].hop < 2))    // ID �Ƿ�
//								bHaveDAddr = 0;
//						}
//					}
				ret = 0;
                negapkt.data[7] = subpkt.type;
                negapkt.data[8] = commDelayFlg;
                negapkt.data[9] = pkt->afn;
                
				if (subpkt.type == SUBTYPE_TRANS)//��Լ����͸��ת��                                        
				{
					p = (pkt->data + pkt->afnoffset + dataOffset); //�·�645���ĵĿ�ʼ��ַ
					while (*p == 0xFE)//���˵�0xFE                                                
					{
						p++;
					}
//						if(0 == bHaveDAddr)
//							ret = Hash_Table_Search(pDestAddr);//Hash_Table_Search(p + 1); //����ַ��ͷ

                    id = Sn_Search_Id(pDestAddr);     //add by ben  0210      
                    if ((id < 2)  || (cltor[id].hop < 2) )//|| cltor[id].nodestatus.NetStatus == NODE_STATUS_OUT)    // ID �Ƿ�
                        ret = -1;

					memcpy(negapkt.data, pkt->data + pkt->afnoffset - 7, 7); //������������	
					negapkt.data[0] = START_FLAG;
				}
				else //��͸��ת��                                       
				{
//						if(0 == bHaveDAddr)
//							ret = Hash_Table_Search(subpkt.ammeteraddr); //
                    id = Sn_Search_Id(subpkt.ammeteraddr);     //add by ben  0210      
                    if ((id < 2) || (cltor[id].hop < 2) )//|| cltor[id].nodestatus.NetStatus == NODE_STATUS_OUT)    // ID �Ƿ�
                        ret = -1;

					memcpy(negapkt.data, pkt->data + pkt->afnoffset + dataOffset, 7); //������������ben											 
				}
                
                memcpy((uint8 *)&negapkt.data[10], pkt->rarea, 6);
                
				if (ret != ( - 1))
				{
					if((0 == bEZMACDebugType) && (bJcDisallowFalg))
						goto AFN_TYPE_DATARELAY_fail;
                    
                    EZ_TX_PKT * ezPkt = getIdelSendCacheByPri(CON_SEND_PRIORITY_NORMAL);

                	if(NULL != ezPkt)
					//if(0 == EzDataTxPkt.bValid)
					{
						PKT* pTxPkt 	= 	&(ezPkt->TxPkt);
						
						if(bEZMACDebugType)
							pTxPkt->ttl = pEZMACRecvPacket->nTTL;
						else
							pTxPkt->ttl = MAX_DATA_TIME_LIVE;//(uint8)recvuppkt.deadtime;
                        
                        memset((uint8 *)&pTxPkt->head.apdu, 0, sizeof(STAPDU));
						pTxPkt->head.apdu.ctrl.dir = 0;
                        pTxPkt->head.apdu.ctrl.prm = 1;
                        pTxPkt->head.apdu.ctrl.evtmode = 1;
                        pTxPkt->head.apdu.ctrl.ftd = 2;
                        pTxPkt->head.apdu.seq = cltor_shadow[id].sendseq;
                        pTxPkt->head.apdu.fn = 0;//����Ӧ
                        pTxPkt->head.apdu.stInfo.stDown.bit1.routeFlg = pkt->rarea->stDown.bit1.routeFlg;
                        pTxPkt->head.apdu.stInfo.stDown.bit1.nodeFlg = pkt->rarea->stDown.bit1.nodeFlg;
                        pTxPkt->head.apdu.stInfo.stDown.bit1.addrFlg = 0;
                        pTxPkt->head.apdu.stInfo.stDown.bit1.conflict = pkt->rarea->stDown.bit1.conflict;
                        pTxPkt->head.apdu.stInfo.stDown.bit1.routeNum = pkt->rarea->stDown.bit1.routeNum;
                        pTxPkt->head.apdu.stInfo.stDown.bit2.channelFlg = pkt->rarea->stDown.bit2.channelFlg;
                        pTxPkt->head.apdu.stInfo.stDown.bit2.errEncodeFlg = pkt->rarea->stDown.bit2.errEncodeFlg & 0x03;
                        //pTxPkt->head.apdu.addrlen = pkt->afnoffset -  pkt->AddrOffSet;
                        pTxPkt->head.apdu.addr = pTxPkt->data + PKT_HEAD_LEN;

                        memcpy(pTxPkt->head.apdu.addr, nDeviceMacAddr, UNIQUE_MAC_ADDR_LEN);//�ڵ�Ψһ��ʶ(����)	

                		ROUTE temprt = Fill_Route(pTxPkt->head.apdu.addr + 6,  MASTER_ROUTE_PATH, id); //�����ߵ�ַ��·��
                		if(temprt.rinfo.rtype != ERROR_ROUTE_PATH)
                        {      
			                pTxPkt->head.apdu.stInfo.stDown.bit1.routeNum = temprt.rinfo.rdepth;
                        
                            //memcpy(pTxPkt->head.apdu.addr, pkt->data + pkt->AddrOffSet, pTxPkt->head.apdu.addrlen);

                            memcpy(pTxPkt->head.apdu.addr + (temprt.rinfo.rdepth + 1) * 6, cltor[id].devAddr, UNIQUE_MAC_ADDR_LEN);//�ڵ�Ψһ��ʶ(����)

                            pTxPkt->head.apdu.addrlen = (temprt.rinfo.rdepth + 2) * 6;
                            
                            // =  ;
                            pTxPkt->head.apdu.len = subpkt.subpktlen;
                            pTxPkt->head.apdu.data = pTxPkt->data + PKT_HEAD_LEN + pTxPkt->head.apdu.addrlen;
                            memcpy(pTxPkt->head.apdu.data, pkt->data + pkt->afnoffset + dataOffset, pTxPkt->head.apdu.len);
                            //pTxPkt->head.apdu.data = pkt->data + pkt->afnoffset + 5;
                            //memcpy((uint8 *)&pTxPkt->stDownInfo, pkt->rarea, 6);
                            //pTxPkt->downAfn = pkt->afn;

    						pkt->isneedack = 1; // ��Ҫ ack
    						
    						ezPkt->bValid = 1;//ҵ�񻺴���Ч
    						ezPkt->nBackOffSlot = 0;   //���Ϸ���
    						cltor_shadow[id].sendseq++; //ÿ����һ����1
    						negapkt.ttl = 0xff;
    						if(bEZMACDebugType)
    						{							
    							EzMacReqMeterID = id;
    							EZMAC_Debug_Status = EZMAC_DEBUG_STATUS_TX_DONE;							
    						}
    						else
    						{
    							ReqMeterID = id; // ��¼����ID
    							Uart_Status = UART_STATUS_TX_DONE;		
    							Uppktacked = 0; //wait for ack
    						}
                        }
					}
                    else
                    {
                        goto AFN_TYPE_DATARELAY_fail;
                    }
				}
				else
				{
					// ���ط�Ӧ��
AFN_TYPE_DATARELAY_fail: 
					//	memcpy(negapkt.data, pkt->data + pkt->afnoffset + 5, 7); //for test
					memcpy((uint8 *)&negapkt.data[10], pkt->rarea, 6);
					Handle_RX_Up_Nage_PKT(&negapkt, bEZMACDebugType, pEZMACRecvPacket); //����ʧ����Ϣ��		
					if(bEZMACDebugType)
						EZMAC_Debug_Status = EZMAC_DEBUG_STATUS_IDLE;		
					else
					{
						Uart_Status = UART_STATUS_IDLE;
						recvuppkt.deadtime = 0;
					}
				}
			}
			break;
		}
		case AFN_TYPE_ROUTE_POLL://·�ɲ�ѯ
		{	
			if(pkt->dataflag == 0x1)		//F1���ӽڵ�����
			{				
ROUTE_CHECK_F1_NODE_NUM:	

				nMeterNum = Meter_Num();

				memcpy(updata_pkt_back, pkt->data, 13);  
                memcpy(updata_pkt_back + 4, (uint8 *)&stinfo, 6);
				updata_pkt_back[1] = 19; 	//������         
				updata_pkt_back[3] = 0x8a; 	//������    
				memcpy(updata_pkt_back + 13, &nMeterNum, 2);
//					updata_pkt_back[15] = 0x00;
//					updata_pkt_back[16] = 0x04;
                nMeterNum = (rfpara.rf_slotnum-1);
				memcpy(updata_pkt_back + 15, &nMeterNum, 2);
				updata_pkt_back[17]=Check_SUM(&updata_pkt_back[3], 14); //У���	                 
				updata_pkt_back[18]=0x16;                                                                                
				if(bEZMACDebugType)
					SendPacketToEzmacProc(pEZMACRecvPacket, updata_pkt_back, 19, 0);
				else												
					UART_PutArray(UART0, updata_pkt_back, 19);      				
				break;
			}
			
			if(pkt->dataflag == 0x2)		//F2���ӽڵ���Ϣ
			{				
ROUTE_CHECK_F2_NODE_INFO:			
				memcpy(&nStartNum, pkt->data + 13, 2);
				nNeedNum = pkt->data[15];
                if(nNeedNum == 0xff)
                {
                    goto ROUTE_CHECK_F129_NODE_INFO;
                }
				nCurSendNum = 0;
				p=(uint8 *)updata_code_cache;

                if(HASH_Table_Used==0)//��ǰʹ��table1
                    pHasht = htable1;
                else
                    pHasht = htable2;
                
                
                for(i = 0; i < MAX_HASH_TABLE_FLASH_SIZE; i++)
                {
                    if((pHasht[i].status == OCCUPIED))
                    {
                        m++;
                    }
                }
				nMeterNum = 0;
				for(i=0; i < MAX_HASH_TABLE_FLASH_SIZE; i++)
				{
					if(pHasht[i].status == OCCUPIED && pHasht[i].devType != 2)
					{
						nMeterNum ++;
						if((nMeterNum >= nStartNum) && (nCurSendNum < nNeedNum) && (nCurSendNum < 29))
						{
							//���
							pSendData =  p + 16 + nCurSendNum * 8;
							pSn = pHasht[i].addr;
							memcpy(pSendData, pSn, 6);
							find_id = Sn_Search_Id(pSn);	//����SN���Ҷ�ӦID
							if(find_id > 0)
								*(pSendData + 6) = cltor[find_id].hop;
							else
								*(pSendData + 6) = 0;
							*(pSendData + 7) = (pHasht[i].SubType << 3) | 0x01;	//��1��
							nCurSendNum++;	
						}
					}
				}
				
				//-------------------�����б���------------------------
				memcpy(p, pkt->data, 13); 
            
                memcpy(p + 4, (uint8 *)&stinfo, 6);
				p[1]=15+(nCurSendNum * 8) + 3;		//������
				p[3] = 0x8a; 	//������    				
				memcpy(p + 13, &nMeterNum, 2);		//���������
				p[15] = nCurSendNum;				//�������
				p[16 + (nCurSendNum * 8)]= Check_SUM(p + 3, 13 + (nCurSendNum * 8)); 
				p[17 + (nCurSendNum * 8)]= 0x16;
				if(bEZMACDebugType)
					SendPacketToEzmacProc(pEZMACRecvPacket, p, p[1], 0);
				else						
					UART_PutArray(UART0, p, p[1]);	// �ش�				
				break;	
			}
			
			if (pkt->dataflag == 0x4)  //F1ָ���ز��ӽڵ����һ���м�·����Ϣ(�·�һ������ַ��ѯ��·��)                         
			{		
				ret = Hash_Table_Search(&pkt->data[13]); //���ϣ��	
				if (ret != ( - 1))//������
				{
					if(HASH_Table_Used==0)
					searchelem = htable1[ret]; //ȡ����ϣ��1���Ԫ��
					else
					searchelem = htable2[ret]; //ȡ����ϣ��2���Ԫ��	
					id=Sn_Search_Id(searchelem.addr);
					//id = searchelem.colid;
				}
				else//�������
				{					
					Handle_RX_Up_Nage_PKT2(&pkt->data[13],FIND_NO_ADDR, bEZMACDebugType, pEZMACRecvPacket);
					//�����Ҳ�������� 		//by peter
					break;
				}


				//if (After(cltor_shadow[id].father_update, cltor_shadow[id].mother_update))
				{
					temprt = Fill_Route(p, MASTER_ROUTE_PATH, id);
				}
//					else
//					{
//						temprt = Fill_Route(p, MASTER_ROUTE_PATH, id);
//					}
				pkt->data[3] = 0x8a;
				pkt->data[13]=0x00;//�ӽڵ�������(��ʱû��ʵ��)
				pkt->data[14]=0xff;//�ӽڵ�������
				pkt->data[15]=temprt.rinfo.rdepth;//·������
				for(i=0;i<pkt->data[15];i++)//���·��
				{
					memcpy(&pkt->data[16+(i*8)],p+(i*6),6);
					memset(&pkt->data[22+(i*8)],0x0,2);
				}
				pkt->data[1]=17+8*temprt.rinfo.rdepth;//����
				pkt->data[15+8*temprt.rinfo.rdepth]=Check_SUM(&pkt->data[3], 12+8*temprt.rinfo.rdepth); //У���
				pkt->data[16+8*temprt.rinfo.rdepth]=0x16;
				if(bEZMACDebugType)
					SendPacketToEzmacProc(pEZMACRecvPacket, pkt->data, pkt->data[1], 0);
				else					
					UART_PutArray(UART0, pkt->data, pkt->data[1]);//���
				break;	
			}
			if (pkt->dataflag == 0x8)  
			{
ROUTE_CHECK_F4_ROUT_STATUS:
                m = 0;

				nMeterNum = Meter_Num();
			
				nSSNum = 0;
				for (i = 2; i <= rfpara.rf_slotnum; i++)
				{  
					if ((cltor[i].devAddr[0] < 0xFF) && (cltor[i].nodestatus.NetStatus == NODE_STATUS_LOGIN))
						nSSNum ++;
				}
				
				memcpy(updata_pkt_back, pkt->data, 13);                                           
				//updata_pkt_back[1] = 31; 	//������       
				updata_pkt_back[3] = 0x8a; 	//������  
				m = 13;
//////////////////////////////////·���ȶ��ڣ���Խ�࣬�ȴ�ʱ��Խ����һ���������Ժ�·���ȶ�
                int x;
                if(nMeterNum > 1000)
                {
                    x=8;
                }
                else if(nMeterNum > 800)
                {
                    x=7;
                }
                else if(nMeterNum > 600)
                {
                    x=6;
                }
                else if(nMeterNum > 400)
                {
                    x=5;
                }
                else if(nMeterNum > 200)
                {
                    x=4;
                }
                else if(nMeterNum > 100)
                {
                    x=3;
                }
                else
                {
                    x=2;
                }

                if((2*nSSNum) >= nMeterNum || (curslottime > (nMeterNum * x * 1600)))// 2 * 40s * xֵ��
				{
				    updata_pkt_back[m++] = 0x03;//���ڹ�����·��ѧϰ���
                }
                else
                {
				    updata_pkt_back[m++] = 0x02;//���ڹ�����·��ѧϰ���
                }
				memcpy(updata_pkt_back + m, &nMeterNum, 2);
                m+=2;
				memcpy(updata_pkt_back + m, &nSSNum, 2);
                m+=2;
//					updata_pkt_back[16] = nSSNum;
//					updata_pkt_back[17] = 0;
				memset(updata_pkt_back + m, 0, 3);
                m+=3;
                i = getWirelessBaudRate(getSF(), getBW(), getErrorCoding());
                memcpy(updata_pkt_back + m, (uint8 *)&i, 2);
                m+=2;

				memset(updata_pkt_back + m, 0, 6);
                m+=6;
                
				updata_pkt_back[m]=Check_SUM(&updata_pkt_back[3], m-3); //У���
				m++;
				updata_pkt_back[m++]=0x16;
                updata_pkt_back[1] = m & 0xff; //������ 
                updata_pkt_back[2] = m >> 8;
                
//					updata_pkt_back[29]=Check_SUM(&updata_pkt_back[3], 26); //У���	                 
//					updata_pkt_back[30]=0x16;                                                                                
				if(bEZMACDebugType)
					SendPacketToEzmacProc(pEZMACRecvPacket, updata_pkt_back, m, 0);
				else						
					UART_PutArray(UART0, updata_pkt_back, m);      				
				break;			
			}
			if (pkt->dataflag == 0x10)  
			{
ROUTE_CHECK_F5_FAIL_SS:			
				memcpy(&nStartNum, pkt->data + 13, 2);
				nNeedNum = pkt->data[15];
				nCurSendNum = 0;
				p=(uint8 *)updata_code_cache;

				nMeterNum = 0;

//				uint8 nCheckOutSSnum = 0;
			//	if(bSSValidateEnable)
//					{
//						//����ɼ���δ�����ı�־
//						for(i=0; i < MAX_SS_MAP_SIZE; i++)
//							MeterAddrRecMark[i].bFlagChenkOut = 0;
//					
//						if(0 == CurWhiteSSMap)
//							pWriteSSSpace = WhiteSSMap1;
//						else
//							pWriteSSSpace = WhiteSSMap2;	
//						
//						for(i=0; i < MAX_SS_MAP_SIZE; i++)
//						{
//							if(pWriteSSSpace[i].status == OCCUPIED)
//							{
//								find_id = Sn_Search_Id(pWriteSSSpace[i].addr);	//����SN���Ҷ�ӦID
//								if(0 == find_id)
//								{
//									nMeterNum++;
//									MeterAddrRecMark[i].bFlagChenkOut = 1;
//								}
//							}
//						}
//	
//						if(nMeterNum >= nStartNum)
//						{
//							for(i=0; i < MAX_SS_MAP_SIZE; i++)
//							{
//								if(MeterAddrRecMark[i].bFlagChenkOut)
//								{
//									nCheckOutSSnum++;							
//									if((nCheckOutSSnum >= nStartNum) && (nCurSendNum < nNeedNum) && (nCurSendNum < 29))
//									{
//										//���
//										pSendData =  p + 16 + nCurSendNum * 8;
//										memcpy(pSendData,  pWriteSSSpace[i].addr, 6);
//										*(pSendData + 6) = 0;
//										*(pSendData + 7) = 0;	
//										nCurSendNum++;	
//									}
//								}							
//							}						
//						}
//					}
				
				//-------------------�����б���------------------------
				memcpy(p, pkt->data, 13);     	       
				p[1]=15+(nCurSendNum * 8) + 3;		//������
				p[3] = 0x8a; 	//������  				
				memcpy(p + 13, &nMeterNum, 2);		//���������
				p[15] = nCurSendNum;				//�������
				p[16 + (nCurSendNum * 8)]= Check_SUM(p + 3, 13 + (nCurSendNum * 8)); 
				p[17 + (nCurSendNum * 8)]= 0x16;
				if(bEZMACDebugType)
					SendPacketToEzmacProc(pEZMACRecvPacket, p, p[1], 0);
				else						
					UART_PutArray(UART0, p, p[1]);	// �ش�				
				break;	
			}
    		if(pkt->dataflag == 0x20)		//F6������ע��ӽڵ���Ϣ
    		{				
    //ROUTE_CHECK_F6_NODE_INFO:			
    			memcpy(&nStartNum, pkt->data + 13, 2);
    			nNeedNum = pkt->data[15];

    			nCurSendNum = 0;
    			p=(uint8 *)updata_code_cache;

                if(HASH_Table_Used==0)//��ǰʹ��table1
                    pHasht = htable1;
                else
                    pHasht = htable2;
                
    			nMeterNum = 0;
    			for(i=0; i < MAX_HASH_TABLE_FLASH_SIZE; i++)
    			{
    				if(pHasht[i].status == OCCUPIED && pHasht[i].src == 0x55 && pHasht[i].devType != 2)
    				{
    					nMeterNum ++;
    					if((nMeterNum >= nStartNum) && (nCurSendNum < nNeedNum) && (nCurSendNum < 29))
    					{
    						//���
    						pSendData =  p + 16 + nCurSendNum * 8;
    						pSn = pHasht[i].addr;
    						memcpy(pSendData, pSn, 6);
    						find_id = Sn_Search_Id(pSn);	//����SN���Ҷ�ӦID
    						if(find_id > 0)
    							*(pSendData + 6) = cltor[find_id].hop;
    						else
    							*(pSendData + 6) = 0;
    						*(pSendData + 7) = 0x01;	//��1��
    						nCurSendNum++;	
    					}
    				}
    			}
    			
    			//-------------------�����б���------------------------
    			memcpy(p, pkt->data, 13); 
            
                memcpy(p + 4, (uint8 *)&stinfo, 6);
    			p[1]=15+(nCurSendNum * 8) + 3;		//������
    			p[3] = 0x8a; 	//������    				
    			memcpy(p + 13, &nMeterNum, 2);		//���������
    			p[15] = nCurSendNum;				//�������
    			p[16 + (nCurSendNum * 8)]= Check_SUM(p + 3, 13 + (nCurSendNum * 8)); 
    			p[17 + (nCurSendNum * 8)]= 0x16;
    			if(bEZMACDebugType)
    				SendPacketToEzmacProc(pEZMACRecvPacket, p, p[1], 0);
    			else						
    				UART_PutArray(UART0, p, p[1]);	// �ش�				
    			break;	
    		}  
        
    		if(pkt->dataflag == 0x40)		//F7����ģ������״̬
            {
                nMeterNum = Meter_Num();
            
                nSSNum = 0;
                for (i = 2; i <= rfpara.rf_slotnum; i++)
                {  
                    if ((cltor[i].devAddr[0] < 0xFF) && (cltor[i].nodestatus.NetStatus == NODE_STATUS_LOGIN))
                        nSSNum ++;
                }
                
                //memcpy(updata_pkt_back, pkt->data, 13);                                           
                //updata_pkt_back[1] = 31;  //������       
                //updata_pkt_back[3] = 0x8a;  //������  
                //m = 13;
        //////////////////////////////////·���ȶ��ڣ���Խ�࣬�ȴ�ʱ��Խ����һ���������Ժ�·���ȶ�
                int x;
                if(nMeterNum > 1000)
                {
                    x=8;
                }
                else if(nMeterNum > 800)
                {
                    x=7;
                }
                else if(nMeterNum > 600)
                {
                    x=6;
                }
                else if(nMeterNum > 400)
                {
                    x=5;
                }
                else if(nMeterNum > 200)
                {
                    x=4;
                }
                else if(nMeterNum > 100)
                {
                    x=3;
                }
                else
                {
                    x=2;
                }

                if((2*nSSNum) >= nMeterNum || (curslottime > (nMeterNum * x * 1600)))// 2 * 40s * xֵ��
                {
                    updata_pkt_back[m++] = 0x01;//���ڹ�����·��ѧϰ���
                }
                else
                {
                    updata_pkt_back[m++] = 0x00;//���ڹ�����·��ѧϰ���
                }                
                Compose_3762Data(0x80, updata_pkt_back, m, NULL, 0, &stinfo, pkt->afn, pkt->dataflag);
                break;
            }
    		if(pkt->dataflag == 0x80)		//F8����ģ������״̬
            {
                //memcpy(&nStartNum, pkt->data + 13, 2);
                nNeedNum = pkt->data[13];
                
                pSn = pkt->data + 14;
                nCurSendNum = 0;
    			p=(uint8 *)updata_code_cache;


                nMeterNum = 0;
                m=1;
                for(i=0; i < nNeedNum; i++)
                {
                    pSendData =  p + m;
                    find_id = Sn_Search_Id(pSn);    //����SN���Ҷ�ӦID
                    memcpy(pSendData, pSn, 6);
                    if(find_id > 1)
                    {
                        memcpy(pSendData+6, (uint8 *)&find_id, 2);
                        pSendData[9] = cltor[find_id].nodestatus.NetStatus;
                        pSendData[10] = cltor_shadow[find_id].rRssi;
                        
                    }
                    else
                    {
                        memset(pSendData+6, 0xff, 2);
                        pSendData[9] = 0;
                        pSendData[10] = 0;
                        
                    }
                    nCurSendNum++;  
                    m+=10;
                }
                p[0] = nNeedNum;
                Compose_3762Data(0x80, p, m, NULL, 0, &stinfo, pkt->afn, pkt->dataflag);
                break;  
            }
            if(pkt->dataflag == 0x101)		//F9��������Ϣ
            {		
                uint16 father;
                STMETERPARAFLASH stMeter;

    //ROUTE_CHECK_F9_NODE_INFO:
                memcpy(&nStartNum, pkt->data + 13, 2);
                nNeedNum = pkt->data[15];
                if(nNeedNum > 10)
                {
                    nNeedNum = 10;
                }
                nCurSendNum = 0;
                p=(uint8 *)updata_code_cache;

                m = 14;

                nMeterNum = 0;
 //               uint32 routeUpdateSlot = 0;
                for(i=SUP_SS_INDEX_START;i < MAX_SUP_SS_NUM+1; i++)//����
                {
                    if ( cltor[i].devAddr[0] < 0xFF)
                    {
                        nMeterNum ++;
                        if((nMeterNum >= nStartNum) && (nCurSendNum < nNeedNum) && (nCurSendNum < 11))
                        {
                            //���
                            pSendData =  p + m;// + nCurSendNum * 12;
                            pSn = cltor[i].devAddr;
                            memcpy(pSendData, pSn, 6);
                            find_id = i;//Sn_Search_Id(pSn);	//����SN���Ҷ�ӦID
                            father = cltor[i].father;
                            if(father < 2 || father > MAX_SUP_SS_NUM)
                            {
                                memcpy(pSendData+6, nDeviceMacAddr, 6);
                            }
                            else
                            {
                                memcpy(pSendData+6, cltor[father].devAddr, 6);
                            }
                            //if(find_id > 0)
//	                            *(pSendData + 6) = cltor[find_id].hop;
    //								else
    //									*(pSendData + 6) = 0;
    
                            *(pSendData + 12) = cltor[find_id].nodestatus.NetStatus;	//��1��
                            *(pSendData + 13) = getRssiByRssiQuality(cltor_shadow[find_id].nodeRssi);
                            GetCltorPara(find_id, &stMeter);
                            memcpy(pSendData + 14, stMeter.GIS, 8);

                            
                            nCurSendNum++;
                            m+=22;
                        }
                    }
                }
                
            
                //-------------------�����б���------------------------
                memcpy(p, pkt->data, 13); 
                memcpy(p + 4, (uint8 *)&stinfo, 6);
                //p[1]=15+(nCurSendNum * 8) + 3;		//������
                p[3] = 0x8a; 	//������    				
                p[13] = nCurSendNum;				//�������
                p[m]=Check_SUM(&p[3], m-3); //У���
                m++;
                p[m++]=0x16;                                                                                

                p[1] = m & 0xff; //������ 
                p[2] = m >> 8;

                if(bEZMACDebugType)
                    SendPacketToEzmacProc(pEZMACRecvPacket, p, m, 0);
                else						
                    UART_PutArray(UART0, p, m);	// �ش�				
                break;	
            }              
            if(pkt->dataflag == 0x1001)		//F128�����ߴӽڵ�����
            {				
    //ROUTE_CHECK_F128_NODE_NUM:	

                memcpy(updata_pkt_back, pkt->data, 13);  
                memcpy(updata_pkt_back + 4, (uint8 *)&stinfo, 6);
                m=13;
                //updata_pkt_back[1] = 19; 	//������         
                updata_pkt_back[3] = 0x8a; 	//������ 
                nMeterNum = getSnNum();
                memcpy(updata_pkt_back + m, &nMeterNum, 2);
                m+=2;
                nMeterNum = Meter_Num();
                memcpy(updata_pkt_back + m, &nMeterNum, 2);
                m+=2;
                
    //					updata_pkt_back[17]=Check_SUM(&updata_pkt_back[3], 14); //У���	                 
    //					updata_pkt_back[18]=0x16;   
                
                updata_pkt_back[m]=Check_SUM(&updata_pkt_back[3], m-3); //У���
                m++;
                updata_pkt_back[m++]=0x16;                                                                                

                updata_pkt_back[1] = m & 0xff; //������ 
                updata_pkt_back[2] = m >> 8;

                
                if(bEZMACDebugType)
                    SendPacketToEzmacProc(pEZMACRecvPacket, updata_pkt_back, m, 0);
                else												
                    UART_PutArray(UART0, updata_pkt_back, m);      				
                break;
            }
			
            if(pkt->dataflag == 0x1002)		//F129�����ߴӽڵ���Ϣ
            {				
    ROUTE_CHECK_F129_NODE_INFO:			
                memcpy(&nStartNum, pkt->data + 13, 2);
                nNeedNum = pkt->data[15];
                if(nNeedNum == 0xff)
                {
                    nNeedNum = 20;
                }
                nCurSendNum = 0;
                p=(uint8 *)updata_code_cache;

                m = 16;
                memcpy(p+m, (uint8 *)&curslottime, 4);
                m+=4;
                nMeterNum = 0;
                uint32 routeUpdateSlot = 0;
                for(i=SUP_SS_INDEX_START;i < MAX_SUP_SS_NUM+1; i++)//����
                {
                    if ( cltor[i].devAddr[0] < 0xFF)
                    {
                        nMeterNum ++;
                        if((nMeterNum >= nStartNum) && (nCurSendNum < nNeedNum) && (nCurSendNum < 29))
                        {
                            //���
                            pSendData =  p + m;// + nCurSendNum * 12;
                            pSn = cltor[i].devAddr;
                            memcpy(pSendData, pSn, 6);
                            find_id = i;//Sn_Search_Id(pSn);	//����SN���Ҷ�ӦID
                            //if(find_id > 0)
                            *(pSendData + 6) = cltor[find_id].hop;
    //								else
    //									*(pSendData + 6) = 0;
                            *(pSendData + 7) = cltor[find_id].nodestatus.NetStatus;	//��1��
                            routeUpdateSlot = cltor_shadow[find_id].father_update;
//	                            if(cltor_shadow[find_id].mother_update > cltor_shadow[find_id].father_update)
//	                            {
//	                                routeUpdateSlot = cltor_shadow[find_id].mother_update;
//	                            }
                            memcpy(pSendData + 8, (uint8 *)&routeUpdateSlot, 4);
                            nCurSendNum++;
                            m+=12;
                        }
                    }
                }
                
            
                //-------------------�����б���------------------------
                memcpy(p, pkt->data, 13); 
                p[11] = 0x02;//fn
                p[12] = 0x10;
                memcpy(p + 4, (uint8 *)&stinfo, 6);
                //p[1]=15+(nCurSendNum * 8) + 3;		//������
                p[3] = 0x8a; 	//������    				
                memcpy(p + 13, &nMeterNum, 2);		//���������
                p[15] = nCurSendNum;				//�������
    //					p[16 + (nCurSendNum * 8)]= Check_SUM(p + 3, 13 + (nCurSendNum * 8)); 
    //					p[17 + (nCurSendNum * 8)]= 0x16;
                p[m]=Check_SUM(&p[3], m-3); //У���
                m++;
                p[m++]=0x16;                                                                                

                p[1] = m & 0xff; //������ 
                p[2] = m >> 8;

                if(bEZMACDebugType)
                    SendPacketToEzmacProc(pEZMACRecvPacket, p, m, 0);
                else						
                    UART_PutArray(UART0, p, m);	// �ش�				
                break;	
            }   
    		if(pkt->dataflag == 0x208)		//F20������������
    		{		
                uint32 baseAddr;
                uint8 buf[TRANSFER_BUFFER_SIZE];
    //ROUTE_CHECK_F20_NODE_NUM:	
//	            	if(0 == CurBlackSSMap)
//	            		pBlackSSSpace = BlackSSMap1;
//	            	else
//	            		pBlackSSSpace = BlackSSMap2;		
//	                nMeterNum=0;
//	        		for(i = 0; i < MAX_SS_MAP_SIZE; i++)
//	        		{
//	        			if((pBlackSSSpace[i].status == OCCUPIED))
//	        			{
//	        				nMeterNum++;
//	        			}
//	        		}
            	if(0 == CurBlackSSMap)
            		baseAddr = EXT_FLASH_BLACK_MAP1_START;
            	else
                    baseAddr = EXT_FLASH_BLACK_MAP2_START;

                nMeterNum=0;

            	for(loop=0; loop < WB_BLOCK_NO; loop++)
            	{
                    memset(buf, 0, TRANSFER_BUFFER_SIZE);
                    //memset(p, 0xFF, TRANSFER_BUFFER_SIZE);
            	
                    if(SA_ERR_OK != read_archive(buf, baseAddr + (loop * MAX_HASH_TABLE_WB_SIZE * sizeof(WB_SPACE)), MAX_HASH_TABLE_WB_SIZE * sizeof(WB_SPACE)))
                    {
                        continue ;//SA_ERR_FT;
                    }
            	
            		for (i =0 ; i <MAX_HASH_TABLE_WB_SIZE; i++)
            		{
                        pBlackSSSpace = (WB_SPACE*)&buf[i * sizeof(WB_SPACE)];
            			if((pBlackSSSpace->status == OCCUPIED))
            			{
            			    nMeterNum++;
            			}
            		}				
            	}
                
    			memcpy(updata_pkt_back, pkt->data, 13);  
                m=13;
                memcpy(updata_pkt_back + 4, (uint8 *)&stinfo, 6);
    			//updata_pkt_back[1] = 19; 	//������         
    			updata_pkt_back[3] = 0x8a; 	//������    
    			memcpy(updata_pkt_back + m, &nMeterNum, 2);
                m+=2;
    //				updata_pkt_back[17]=Check_SUM(&updata_pkt_back[3], 14); //У���	                 
    //				updata_pkt_back[18]=0x16;  
    			updata_pkt_back[m]=Check_SUM(&updata_pkt_back[3], m-3); //У���
    			m++;
    			updata_pkt_back[m++]=0x16;                                                                                
                updata_pkt_back[1] = m & 0xff; //������ 
                updata_pkt_back[2] = m >> 8;

    			if(bEZMACDebugType)
    				SendPacketToEzmacProc(pEZMACRecvPacket, updata_pkt_back, m, 0);
    			else												
    				UART_PutArray(UART0, updata_pkt_back, m);      				
    			break;
    		}
    		
    		if(pkt->dataflag == 0x210)		//F21���������ڵ���Ϣ
    		{				
    //ROUTE_CHECK_F21_NODE_INFO:			
    			memcpy(&nStartNum, pkt->data + 13, 2);
    			nNeedNum = pkt->data[15];

    			nCurSendNum = 0;
    			p=(uint8 *)updata_code_cache;

                uint32 baseAddr;
                uint8 buf[TRANSFER_BUFFER_SIZE];

            	if(0 == CurBlackSSMap)
            		baseAddr = EXT_FLASH_BLACK_MAP1_START;
            	else
                    baseAddr = EXT_FLASH_BLACK_MAP2_START;

                
            	nMeterNum = 0;

                m=16;
            	for(loop=0; loop < WB_BLOCK_NO; loop++)
            	{
                    memset(buf, 0, TRANSFER_BUFFER_SIZE);
                    //memset(p, 0xFF, TRANSFER_BUFFER_SIZE);
            	
                    if(SA_ERR_OK != read_archive(buf, baseAddr + (loop * MAX_HASH_TABLE_WB_SIZE * sizeof(WB_SPACE)), MAX_HASH_TABLE_WB_SIZE * sizeof(WB_SPACE)))
                    {
                        continue ;//SA_ERR_FT;
                    }
            	
            		for (i =0 ; i <MAX_HASH_TABLE_WB_SIZE; i++)
            		{
                        pBlackSSSpace = (WB_SPACE*)&buf[i * sizeof(WB_SPACE)];
            			if((pBlackSSSpace->status == OCCUPIED))
            			{
                			nMeterNum ++;
                			if((nMeterNum >= nStartNum) && (nCurSendNum < nNeedNum) && (nCurSendNum < 29))
                			{
                				//���
                				pSendData =  p + m;
                				pSn = pBlackSSSpace->addr;
                				memcpy(pSendData, pSn, 6);
                				nCurSendNum++;	
                                m+=6;
                			}
            			}
            		}				
            	}

//	            	WB_SPACE* pBlackSSSpace;
//	            	if(0 == CurBlackSSMap)
//	            		pBlackSSSpace = BlackSSMap1;
//	            	else
//	            		pBlackSSSpace = BlackSSMap2;		
//	
//	                
//	    			nMeterNum = 0;
//	
//	                m=16;
//	    			for(i=0; i < MAX_SS_MAP_SIZE; i++)
//	    			{
//	    				if(pBlackSSSpace[i].status == OCCUPIED)
//	    				{
//	    					nMeterNum ++;
//	    					if((nMeterNum >= nStartNum) && (nCurSendNum < nNeedNum) && (nCurSendNum < 29))
//	    					{
//	    						//���
//	    						pSendData =  p + m;
//	    						pSn = pBlackSSSpace[i].addr;
//	    						memcpy(pSendData, pSn, 6);
//	    						nCurSendNum++;	
//	                            m+=6;
//	    					}
//	    				}
//	    			}
    			
    			//-------------------�����б���------------------------
    			memcpy(p, pkt->data, 13); 
            
                memcpy(p + 4, (uint8 *)&stinfo, 6);
    			//p[1]=m+=2;		//������
    			p[3] = 0x8a; 	//������    				
    			memcpy(p + 13, &nMeterNum, 2);		//���������
    			p[15] = nCurSendNum;				//�������
    //				p[16 + (nCurSendNum * 6)]= Check_SUM(p + 3, 13 + (nCurSendNum * 6)); 
    //				p[17 + (nCurSendNum * 6)]= 0x16;
    			p[m]=Check_SUM(&p[3], m-3); //У���
    			m++;
    			p[m++]=0x16;                                                                                
                p[1] = m & 0xff; //������ 
                p[2] = m >> 8;
    			if(bEZMACDebugType)
    				SendPacketToEzmacProc(pEZMACRecvPacket, p, p[1], 0);
    			else						
    				UART_PutArray(UART0, p, p[1]);	// �ش�				
    			break;	
    		}	
            break;
        }
        
        case AFN_TYPE_ROUTE_SET:  //·������                      
		{
			if (pkt->dataflag == 0x1)  //F1����ز��ӽڵ�   
			{
ROUTE_SET_F1_ADD_SN:			
				temp = pkt->data[13]; //��ӵĽڵ����
				bError = 0;
				if(bMeterInfoSynEnable)
				{
					if(0 == bSendResetCmd)
					{
#ifdef REMESH_OLD					
						for (i = 0; i < temp; i++)
						{		
							pSn = &(pkt->data[14 + i * 9]);		//�ɼ�����ַ
							if(((pSn[0] > 0) || (pSn[1] > 0) || (pSn[2] > 0) || (pSn[3] > 0) || (pSn[4] > 0) || (pSn[5] > 0))
								&& (pSn[0] <= 0x99) && (pSn[1] <= 0x99) && (pSn[2] <= 0x99) && (pSn[3] <= 0x99) 
								&& (pSn[4] <= 0x99) && (pSn[5] <= 0x99))						
								Add_Validate_W_B(pSn,6, SS_VALIDATE_TYPE_WHITE);
							
						/*	memcpy(elem.addr, pkt->data + 14 + i * 9, 6); //����ַ
							memcpy(elem.falsh_sn, elem.addr, 6);
							elem.SubType = pSource[2];
							return_hash=Hash_Table_Insert(&elem, &ReturnHash);
							if (return_hash < 0)        //��ӹ�ϣ��                                        
								insertok = 0; //���ʧ�ܣ��ñ�־λ

							if (insertok)
							{  
								if (return_hash==MAX_HASH_TABLE_FLASH_SIZE)
								{	
									HASH_Table_Used++;
									HASH_Table_Used=(HASH_Table_Used&1);   //1��ʾ֮ǰ��Table1,֮��Table2;0��ʾ��Table2
									I2cWrite(0xA0,&HASH_Table_Used, FM_HASH_USED, 1);//�����ʶλ
									Hash_Transfer();//����,ת�ƹ�ϣ��
									return_hash = Hash_Table_Insert(&elem, &ReturnHash);//�ٴβ����ϴ�δ��ӳɹ���
									if((return_hash >= 0 ) && (return_hash == MAX_HASH_TABLE_FLASH_SIZE))
									{
										bError = 1;
										break;
									}
								}
							}
							else                                
								bError = 1;			
							*/ 
						}
#else
                        for (i = 0; i < temp; i++)
                        {       
                            pSn = &(pkt->data[14 + i * 7]);     //�ɼ�����ַ
//	                            if(((pSn[0] > 0) || (pSn[1] > 0) || (pSn[2] > 0) || (pSn[3] > 0) || (pSn[4] > 0) || (pSn[5] > 0))
//	                                && (pSn[0] <= 0x99) && (pSn[1] <= 0x99) && (pSn[2] <= 0x99) && (pSn[3] <= 0x99) 
//	                                && (pSn[4] <= 0x99) && (pSn[5] <= 0x99))    
//	                            {
//	                                Add_Validate_W_B(pSn, 6, SS_VALIDATE_TYPE_WHITE);
//	                            }

                            memcpy(elem.addr, pkt->data + 14 + i * 7, 6); //����ַ
//	                            memcpy(elem.falsh_sn, elem.addr, 6);
                            elem.SubType = pkt->data[14 + i * 7 + 6];
                            if(elem.SubType == 1 || elem.SubType == 2 || elem.SubType == 3 
                                || elem.SubType == 4 || elem.SubType == 5)
                            {
                                elem.devType = 1;
                            }
                            else
                            {
                                elem.devType = 0;
                            }
                            return_hash=Hash_Table_Insert(&elem, &ReturnHash);
//	                            if (return_hash < 0)        //��ӹ�ϣ��                                        
//	                                insertok = 0; //���ʧ�ܣ��ñ�־λ
//	
//	                            if (insertok)
//	                            {  
                            if (return_hash==MAX_HASH_TABLE_FLASH_SIZE)
                            {   
                                HASH_Table_Used++;
                                HASH_Table_Used=(HASH_Table_Used&1);   //1��ʾ֮ǰ��Table1,֮��Table2;0��ʾ��Table2
                                I2cWrite(0xA0,&HASH_Table_Used, FM_HASH_USED, 1);//�����ʶλ
                                Hash_Transfer();//����,ת�ƹ�ϣ��
                                return_hash = Hash_Table_Insert(&elem, &ReturnHash);//�ٴβ����ϴ�δ��ӳɹ���
                                if((return_hash == MAX_HASH_TABLE_FLASH_SIZE))
                                {
                                    bError = 1;
                                    break;
                                }
                            }
//	                            }
//	                            else                                
//	                                bError = 1;         
                             
                        }

#endif
					}
					else
						bError = 1;	
				}
				else
					bError = 0;	

				SendReturnpkt(bError, &stinfo);
				
			}	
			if (pkt->dataflag == 0x2) //F2ɾ���ز��ӽڵ�         
			{
ROUTE_SET_F2_DELETE_METER:
				bError = 0;
				if(bMeterInfoSynEnable)
				{
					if(0 == bSendResetCmd)
					{
						HASHT* pHtable = NULL;
						if(HASH_Table_Used==0)
							pHtable = htable1;
						else
							pHtable = htable2;

						temp = pkt->data[13]; //Ҫɾ���ĸ���
						for (loop = 0; loop < temp; loop++)
						{
							deladd = pkt->data + 14 + loop * 6;

							//ɾ��������
							ret = Hash_Table_Search(deladd); //���ҹ�ϣ���е�����ֵ				
							if (ret != ( - 1))
							{
                                for(i=SUP_SS_INDEX_START;i<MAX_SUP_SS_NUM+1;i++)   //����
                                {
                                    if(!LPC_Memory_CMP(cltor[i].devAddr,pHtable[ret].addr, 6))//�Ա�һ��
                                    {
                                        cltor[i].nodestatus.bNeedDelete = 1;
                                        cltor_shadow[i].nodestatus.ans_pkt_type = 3;
                                        if((!cltor_shadow[i].nodestatus.bDataAck) && (!cltor_shadow[i].nodestatus.bNetAck))
                                        {
                                            cltor_shadow[i].FrameDeadTime = 0;
                                        }
                                        break;
                                    }
                                }
                                if(i == (MAX_SUP_SS_NUM+1))
                                {
								//ɾ������ַ
									Updata_Hash_Table(ret, NULL);  //ɾ��    
                                }
							}

							//ɾ����֤�ɼ�������
//								Del_Validate_W_B(deladd);
						}
					}
					else
						bError = 1;					
				}

				SendReturnpkt(bError, &stinfo);
				break;
			}

			if (pkt->dataflag == 0x4) //F3���ôӽڵ�̶��м�·��(������ӵ��)      
			{	
			//	uint8 insertok;
ROUTE_SET_F3_SET_ROUTE:		                                
//					memcpy(elem.falsh_sn,&pkt->data[20],6);
				memcpy(elem.addr, pkt->data + 13, 6); //����ַ
				return_hash=Hash_Table_Insert(&elem, &ReturnHash);
				if (return_hash == MAX_HASH_TABLE_FLASH_SIZE)        //��ӹ�ϣ��                                        
				{                                                        
					insertok = 0; //���ʧ�ܣ��ñ�־λ
				}
				else
					insertok = 0; //���ʧ�ܣ��ñ�־λ

				if(0 == insertok)
				{
				//TODO:����ʧ��!!!!!!	
				}
				SendReturnpkt(0, &stinfo);
				break;
			}
			if(pkt->dataflag == 0x08)  //F4  ����·�ɹ���ģʽ
			{
ROUTE_SET_F4_SET_ROUTE_MODE:	
				SendReturnpkt(0, &stinfo);
				break;
			}
			
			if(pkt->dataflag == 0x10)  //F5  �����ز��ӽڵ�����ע��
			{
//ROUTE_SET_F5_START_SEARCH_METER:				
				if(0 == bEZMACDebugType)
				{				
					bError = 0;
					uint16 tTime = 0;
					memcpy(&tTime, pkt->data + 19, 2);
					if(0 == tTime)
					{			
						if(bBroadMeterEnable)
                        {                  
							CloseBoardSearchMeter(1);
                        }
					}
					else
					{
						if(0 == bSendResetCmd)				
						{					
							//�ر���֤ʹ�ܲ���
							if(bSSValidateEnable)
							{
								bSSValidateEnable = 0;		 
								I2cWrite(0xA0, &bSSValidateEnable, FM_SS_VALIDATE_ENABLE , 1); 	

								bNeedOpenSSValidateEnable = 1;
								I2cWrite(0xA0, &bNeedOpenSSValidateEnable, FM_NEED_OPEN_SS_VALIDATE , 1); 	
							}
														
							//��ʼ��FLASH				
						/*	memset(dis,0xff,32);
							for(i=0;i<32;i++)
							{	                                   
								I2cWrite(0xA0, dis, 0+(i*32), 32); //��λӰ�ӿռ�***
								memset(shadow_space+(i*32),0xff,32);						 
							}
							IapAction = IAP_ACTION_ERASE_HASH1; //IAP��ʶλ
							while (IapAction != IAP_ACTION_IDLE);
							IapAction = IAP_ACTION_ERASE_HASH2; //IAP��ʶλ
							while (IapAction != IAP_ACTION_IDLE);	// Hold on	
							IapAction = IAP_ACTION_ERASE_WRITE_SS_HASH1; //IAP��ʶλ
							while (IapAction != IAP_ACTION_IDLE);
							IapAction = IAP_ACTION_ERASE_WRITE_SS_HASH2; //IAP��ʶλ
							while (IapAction != IAP_ACTION_IDLE);
							IapAction = IAP_ACTION_ERASE_BLACK_SS_HASH1; //IAP��ʶλ
							while (IapAction != IAP_ACTION_IDLE);
							IapAction = IAP_ACTION_ERASE_BLACK_SS_HASH2; //IAP��ʶλ
							while (IapAction != IAP_ACTION_IDLE);	*/

							//��¼��ǰ��־��ʱ��
							bBroadMeterEnable = 1;
							I2cWrite(0xA0, &bBroadMeterEnable, FM_BROAD_METER_ENABLE , 1); 
							nBroadMeterTime = tTime;
							I2cWrite(0xA0, (uint8*)&nBroadMeterTime, FM_BROAD_METER_TIME , 2); 

							//��ʼ�㲥���									
							bMeterSearchStart = 1;
							for(loop = 0; loop < MAX_HASH_TABLE_FLASH_SIZE; loop++)
							{
								MeterStatus[loop].bNeedDelete = 0;
								MeterStatus[loop].bDeleteWaitAck= 0;
								
								MeterStatus[loop].bAck = 0;
								MeterStatus[loop].bReceive = 0;
								MeterStatus[loop].bWaitAck = 0;									
								MeterStatus[loop].bSingleSSSearchMeterReceive = 0;
							}
							bNeedMeterDeleteAck = 0;		
							bNeedDeleteMeter = 0;								

							bMeterDeadStart = 0;
							bCltorSearchStart = 0;
							
							//ȡ���Ե����ɼ������в�������
							bSingleSearchSSSendMeter = 0;
							bSingleSearchSSMeterSended = 0;
							bSingleSearchSSSendMeterWaitAck = 0;
							
							adv_subtype = ADV_TYPE_ADDR_SEARCH; //����ѯ      
							adv_updatetime = curslottime + 2400 * tTime;
							adv_sublen = 0;						
						}
						else
							bError = 1;
					}
					SendReturnpkt(bError, &stinfo);
				}	
				break;
			}
			if(pkt->dataflag == 0x20) //F6 ��ֹ�ӽڵ�����ע��//�����Ҫ��֤�Ĳɼ����б�     
			{
ROUTE_SET_F6_ADD_SS:			
				//Add_Validate_W_B_List(pkt->data);
                CloseBoardSearchMeter(1);
                SendReturnpkt(0, &stinfo);
				break;
			}				
			if(pkt->dataflag == 0x40)  //F7 ɾ����֤�Ĳɼ���
			{
ROUTE_SET_F7_DELETE_SS:				
				Del_Validate_W_B_List(pkt->data);
                
                SendReturnpkt(0, &stinfo);
				break;
			}

			if(pkt->dataflag == 0x104)  //F11��ӵ��(���)
			{
				//set route 
ROUTE_SET_F11_ADD_METER:	
				bError = 0;
				if(bMeterInfoSynEnable)
				{
					if((0 == bSendResetCmd))
					{
						temp = pkt->data[13]; //��ӵĽڵ����
						for (i = 0; i < temp; i++)
						{	
							nStart = 14 + i*13;
//								memcpy(elem.falsh_sn,  pkt->data+ nStart + 6, 6);		//�ɼ�����ַ							
							memcpy(elem.addr, pkt->data + nStart, 6); 			//����ַ
							elem.SubType = *(pkt->data + nStart + 12);
							
							insertok = 1;
							return_hash=Hash_Table_Insert(&elem, &ReturnHash);
//								if (return_hash < 0)       	//��ӹ�ϣ��                                        
//									insertok = 0; 			//���ʧ�ܣ��ñ�־λ
//	
//								if (insertok)
//								{  
								if (return_hash==MAX_HASH_TABLE_FLASH_SIZE)
								{	
									HASH_Table_Used++;
									HASH_Table_Used=(HASH_Table_Used&1);   //1��ʾ֮ǰ��Table1,֮��Table2;0��ʾ��Table2
									I2cWrite(0xA0,&HASH_Table_Used, FM_HASH_USED, 1);//�����ʶλ
									Hash_Transfer();//����,ת�ƹ�ϣ��
									return_hash = Hash_Table_Insert(&elem, &ReturnHash);//�ٴβ����ϴ�δ��ӳɹ���
									if((return_hash == MAX_HASH_TABLE_FLASH_SIZE))
									{
										bError = 1;
										break;
									}
								}
						//	}
						//	else                                
						//		bError = 1;			
//								Add_Validate_W_B(elem.falsh_sn, SS_VALIDATE_TYPE_WHITE);
						}
					}
					else
						bError = 1;
				}
				else
					bError = 0;

				SendReturnpkt(bError, &stinfo);
				break;
			}
			if (pkt->dataflag == 0x208)  //F20��Ӻ�����
			{
//ROUTE_SET_F20_ADD_BLACK_SN:			
				temp = pkt->data[13]; //��ӵĽڵ����
				bError = 0;
				if(bMeterInfoSynEnable)
				{
					if(0 == bSendResetCmd)
					{
						for (i = 0; i < temp; i++)
						{		
							pSn = &(pkt->data[14 + i * 9]);		//�ɼ�����ַ
							if(((pSn[0] > 0) || (pSn[1] > 0) || (pSn[2] > 0) || (pSn[3] > 0) || (pSn[4] > 0) || (pSn[5] > 0))
								&& (pSn[0] < 0xFF) && (pSn[1] < 0xFF) && (pSn[2] < 0xFF) && (pSn[3] < 0xFF) 
								&& (pSn[4] < 0xFF) && (pSn[5] < 0xFF))						
								Add_Validate_W_B(pSn, 6, SS_VALIDATE_TYPE_BLACK);
						}
					}
					else
						bError = 1;	
				}
				else
					bError = 0;	

				SendReturnpkt(bError, &stinfo);
				break;
			}	
			if (pkt->dataflag == 0x210) //F21ɾ��������
			{
//ROUTE_SET_F21_DELETE_BLACK_METER:
				bError = 0;
				if(bMeterInfoSynEnable)
				{
					if(0 == bSendResetCmd)
					{
						temp = pkt->data[13]; //Ҫɾ���ĸ���
						for (i = 0; i < temp; i++)
						{
							deladd = pkt->data + 14 + i * 6;

							//ɾ��������

							Del_Validate_W_B(deladd, 6);
						}
					}
					else
						bError = 1;					
				}
				SendReturnpkt(bError, &stinfo);
				break;
			}
			//----------------------------------------------------------------
			if(0 == bEZMACDebugType)
				Uart_Status = UART_STATUS_IDLE;
			break;
		}
		case AFN_TYPE_ROUTE_CTL:	//·�ɿ���
		{
            dis[0] = 0xC7;
			if (pkt->dataflag == 0x1 && pkt->dataflag == 0x4)//����
            {
            }
            else if(pkt->dataflag == 0x2)
            {
                dis[0] = 0x40;
                //bError = EZMacPRO_Transmit_Adv(2, dis, 1);//��ֹ�ϱ�
            }
            bError = 1;//EZMacPRO_Transmit_Adv(2, dis, 1);//ȫ������

			SendReturnpkt(!bError, &stinfo);
			break;
		}
        case AFN_TYPE_UPDATA:
        {
			if (pkt->dataflag == 0x1)//�ļ�����
            {
                int32 seq;
				memcpy(updata_pkt_back, pkt->data, 13); 
                memcpy(updata_pkt_back + 4, (uint8 *)&stinfo, 6);
                m = 13;
				//updata_pkt_back[1] = 18; //������       
				updata_pkt_back[3] = 0x8a; 	//������    
				seq = filetrans_flag(pkt);
                LOG_DEBUG( DBGFMT"filetrans_flag ret = %d    \n",DBGARG, seq);
                if(seq < 0)
                {
                    memset(updata_pkt_back + m, 0xFF, 4);
                }
                else
                {
                    memcpy(updata_pkt_back + m, (uint8 *)&seq, 4);
                }
                m+=4;
				updata_pkt_back[m]=Check_SUM(&updata_pkt_back[3], m-3); //У���
				m++;
				updata_pkt_back[m++]=0x16;                                                                                
	
                updata_pkt_back[1] = m & 0xff; //������ 
                updata_pkt_back[2] = m >> 8;


				if(bEZMACDebugType)
					SendPacketToEzmacProc(pEZMACRecvPacket, updata_pkt_back, m, 0);
				else
					UART_PutArray(UART0, updata_pkt_back, m);	

                //LOG_DEBUG( DBGFMT" upgrade send len = %d\n",DBGARG, m);
                //LOG_DUMPHEX(LOG_LEVEL_DEBUG, "HexData : \n", updata_pkt_back, m);
                

                
                if(upd_pkt.flg == 0x55 && upd_pkt.type == UPGRADE_TYPE_TER_MODULE)
                {
                    upd_pkt.flg = 0xFF;
                    g_ucUpgradeFlgForPush = 0xFF;
                    upd_pkt.crc = CRC_16BIT_Check1((uint8 *)&upd_pkt, sizeof(UPDATE_PKT) - 2);
                    I2cWrite(0xA0,  (uint8 *)&upd_pkt, EEPROM_UPDATE_PARA, sizeof(UPDATE_PKT));  
                    Delay_ms(200);                                      
                    Reset();
                }
            }
            break;
        }
		case AFN_TYPE_DEBUG ://�ڲ�����ģʽ
		{
			if (pkt->dataflag == 0x1)//���˲�ѯ
			{	
				senduppkt.data[0] = START_FLAG;
				senduppkt.data[1] = 55; // �趨���˰��ĳ���  
				senduppkt.data[2] = 0x0;

				senduppkt.data[3] = 0x8a; //������							
				senduppkt.data[4] = 0x00; //[6�ֽ���Ϣ��]
				senduppkt.data[5] = 0x00;
				senduppkt.data[6] = 0x50;
				senduppkt.data[7] = 0x00;
				senduppkt.data[8] = 0x00;
				senduppkt.data[9] = 0x00;

				senduppkt.data[10] = 0xF0; //AFN
				senduppkt.data[11] = 0x01; //���ݵ�Ԫ��ʶ[2�ֽ�]
				senduppkt.data[12] = 0x00;


				memcpy(&senduppkt.data[13],&pkt->data[13],6);//SN

				senduppkt.data[19] = 0x0; // �ڵ��(16λ)
				senduppkt.data[20] = 0x0; // �ڵ��
				senduppkt.data[21] = 0x0; //����汾��
				senduppkt.data[22] = 0x0;// ��·�ɽڵ�(16λ)
				senduppkt.data[23] = 0x0; // ��·��
				senduppkt.data[24] = 0x0; // ��·�ɵ�����ֵ
				senduppkt.data[25] = 0x0;// ��·�ɽڵ�(16λ)
				senduppkt.data[26] = 0x0; // ��·��
				senduppkt.data[27] = 0x0; // ��·�ɵ�����ֵ
				senduppkt.data[28] = 8; //��¼��							

				for(i=0;i<24;i++)
		    	{
		    		senduppkt.data[29+i]=0x0;// 24��0x00								  
		    	}
				senduppkt.data[53] = Check_SUM(senduppkt.data + 3, 26); //У���ܺ�
				senduppkt.data[54] = END_FLAG;
				senduppkt.len = 55;

				find_id=Sn_Search_Id(&pkt->data[13]);//����SN���Ҷ�ӦID
                STMETERPARAFLASH stMeterParaF;
                GetCltorPara(find_id, &stMeterParaF);

				if(u8TuopuDisplaySource)
				{
					//������վ��ֱ�ӻ�ȡ�ڵ��·����Ϣ
					if((find_id > 1))
					{			  	   			
						senduppkt.data[19] = find_id; // �ڵ��(16λ)
						senduppkt.data[20] = 0x0; // �ڵ��
						senduppkt.data[21] = stMeterParaF.softver; //����汾��
						//	if((cltor[find_id].father == 0) || (cltor[find_id].father > rfpara.rf_slotnum))
						//		i = 0;
						senduppkt.data[22] = cltor[find_id].father; // ��·�ɽڵ�(16λ)
						senduppkt.data[23] = 0x0; // ��·��
						senduppkt.data[24] = 0x0; // ��·�ɵ�����ֵ						
						senduppkt.data[25] = cltor[find_id].mother;// ��·�ɽڵ�(16λ)
						senduppkt.data[26] = 0x0; // ��·��
						senduppkt.data[27] = 0x0; // ��·�ɵ�����ֵ
						senduppkt.data[28] = 0; //��¼��		

						//	senduppkt.data[29] = Check_SUM(senduppkt.data + 3, 26); //У���ܺ�
						//	senduppkt.data[30] = END_FLAG;
						//	senduppkt.len = 31;

						senduppkt.data[29] = stMeterParaF.nHwType;
						senduppkt.data[30] = 0;//stMeterParaF.nSoftFn;							
						senduppkt.data[31] = 0;//stMeterParaF.nSubSoftVersion;

						senduppkt.data[32] = Check_SUM(senduppkt.data + 3, 29); //У���ܺ�
						senduppkt.data[33] = END_FLAG;	
						senduppkt.len = 34;

						senduppkt.data[1] = senduppkt.len; // �趨���˰��ĳ���  						
					}								
					if(bEZMACDebugType)
						SendPacketToEzmacProc(pEZMACRecvPacket, senduppkt.data, senduppkt.len, 0);		
					else
					{
						UART_PutArray(UART0, senduppkt.data, senduppkt.len);   
						Uart_Status = UART_STATUS_IDLE;														
					}
				}
				else
				{
					//�Ӳɼ����л�ȡ�ڵ��������Ϣ
					//	recvuppkt.deadtime = curslottime + MAX_TIME_LIVE * 2; // 6S��ʱ							
					if(bEZMACDebugType)
					{
						if(EZMAC_Debug_Status == EZMAC_DEBUG_STATUS_IDLE)
						{
							pEZMACRecvPacket->nTTL = MAX_DATA_TIME_LIVE;
							EZMAC_Debug_Status = EZMAC_DEBUG_STATUS_BUSY;
							EZMacStaAck = PKT_TYPE_TOPO;
						}
					}
					else
					{						
						//if(Uart_Status == UART_STATUS_RX_DONE)
						{
							recvuppkt.deadtime = MAX_DATA_TIME_LIVE;
							Uart_Status = UART_STATUS_BUSY;	
							StaAck = PKT_TYPE_TOPO;	
						}
					}

					if(find_id > 1)		//�鵽��ӦID
					{		
//	                        if(EzNetTxPkt.bValid)//������Ч
//	                            goto TOPO_FAILED;

					}	
					else
					{
//TOPO_FAILED:  
						if(bEZMACDebugType)
						{
							SendPacketToEzmacProc(pEZMACRecvPacket, senduppkt.data, senduppkt.len, 0);						
							EZMAC_Debug_Status = EZMAC_DEBUG_STATUS_IDLE;
						}
						else
						{		
							UART_PutArray(UART0, senduppkt.data, senduppkt.len);  
							Uart_Status = UART_STATUS_IDLE;
							recvuppkt.deadtime = 0;							
						}
					}
				}	
				break;
			}

			if (pkt->dataflag == 0x2)//F2ģ�Ⳮ��
			{
				break;
			}
			if (pkt->dataflag == 0x4)//F3��������
			{

				break;
			}
			if (pkt->dataflag == 0x8)//F4���书�ʲ���
			{
				ez_test.power=pkt->data[13];
				if(ez_test.power < 8)
				{
					ez_test.netid=pkt->data[14];
					ez_test.frq=pkt->data[15];
					ez_test.cnt=0;
					//SpiWriteRegister(SI4432_TX_POWER, (ez_test.power |0x18)); // ���书��0 ~ 7
					ez_test.msr=EZ_TEST_BIT;//����ģʽ
					if(bEZMACDebugType)
						SendPacketToEzmacProc(pEZMACRecvPacket,  informpkt, 0x13, 0);		
					else
						UART_PutArray(UART0, informpkt, 0x13); //ȷ�ϻش�
				}
				break;
			}
			if (pkt->dataflag == 0x10)//F5�����Ȳ���
			{
				if(0 == EzTestRevDone)
				{
					ls_test.netid=pkt->data[13];
					ls_test.frq=pkt->data[14];
					ls_test.msr=EZ_TEST_BIT;//����ģʽ
					ls_test.cnt=0;
					ls_test.deadtime=(curslottime+100);
					if(bEZMACDebugType)
						SendPacketToEzmacProc(pEZMACRecvPacket,  informpkt, 0x13, 0);		
					else
						UART_PutArray(UART0, informpkt, 0x13); //ȷ�ϻش�					

					//�����ڷ�������յĲ��������£���ֹ����
					RsResetTime = RS_RESET_TIME;
				}
				break;
			}
			if (pkt->dataflag == 0x20)//F6У׼
			{
				if(0 == bEZMACDebugType)
				{
					UART_PutArray(UART0, informpkt, 0x13); //ȷ�ϻش�
					//Correct_RF_Clock(pkt->data[13],pkt->data[14]);//У׼��Ƶʱ��
				}
				break;
			}
			if(0 == bEZMACDebugType)
			{
				bDebugSend = 1;
				switch(pkt->dataflag)
				{
					case 0x102:		//F10 F1��ȷ��
					{
						goto INFORM_RETURN;
					}
					case 0x104:		//F11 F1��Ӳ����ʼ��
					{
						goto HW_INIT_F1;
					}
					case 0x108:		//F12 F2����������ʼ��
					{
						goto PARA_INIT_F2;
					}
					case 0x110:		//F13 F3����������ʼ��
					{
						goto DATA_INIT_F3;
					}
					case 0x120:		//F14 F1��ת������
					{
						goto DATA_TRANS;
					}
					case 0x140:		//F15 �����ϱ����״̬�ֵĻظ�
					{
						goto REPORT_RETURN;
					}					
					case 0x208:		//F20 	F1�����̴���Ͱ汾��Ϣ
					{
						goto GET_VERSION_F1;	
					}
					case 0x210:		//F21	F2������ֵ
					{
						goto GET_TEMPER_VALUE_F2;	
					}
					case 0x220:		//F22	F4���ز����ڵ��ַ
					{
						goto GET_DEVICE_ADDR_F4;
					}
					case 0x240:		//F23	F5���ز����ڵ�״̬�ֺ��ز�����
					{
						goto GET_DEVICE_STATUS_F5;
					}
					case 0x280:		//F24	F7����ѯ����ͨ�Ų���?				
					{
						goto GET_SZ_COMM_PARA_F7;
					}
					case 0x301:		//F25	F8����ѯƵ�㶨��
					{
						goto GET_SZ_FREQ_PARA_F8;
					}
					case 0x302:		//F26	F9����ѯ�ŵ�����
					{
						goto GET_CHANNEL_PARA_F9;
					}
					case 0x304:		//F27	  F10�� ��ѯ·�ɲ���
					{
						goto GET_ROUTE_PARA_F10;
					}
					case 0x308:		//F28	F11:��ѯ�����Ϣ_����ָ��
					{
						goto METER_ADDR_START_CMD_F11;
					}	
					case 0x310:		//F29	F12��ѯ�����Ϣ_����ָ��
					{
						goto METER_ADDR_TEANS_CMD_F12;
					}			
					case 0x320:		//F30	F13�� ��ѯ�ɼ�����Ϣ_����ָ��
					{
						goto SS_INFO_START_CMD_F13;
					}	
					case 0x340:		//F31	F14�� ��ѯ�ɼ�����Ϣ_����״ָ̬��
					{
						goto SS_INFO_TRANS_CMD_F14;
					}	
					case 0x380:		//F32	F15����ȡ�Ѵ������ַ_����ָ��
					{
						goto METER_HANDLE_START_CMD_F15;
					}		
					case 0x401:		//F33	F16����ȡ�Ѵ������ַ_����ָ��
					{
						goto METER_HANDLE_TRANS_CMD_F16;
					}	
					case 0x402:		//F34	F17�� ��ѯ�ɼ�������ʹ�ܲ���
					{
						goto GET_SS_UPDATA_PARA_F17;
					}	
					case 0x404:		//F35	F18: ��ȡ����ϻ�����
					{
						goto GET_METER_DEAD_COUNT_F18;
					}	
					case 0x408:		//F36	F19����ѯ�ɼ�����֤ʹ�ܲ���
					{
						goto GET_SS_VALIDATE_PARA_F19;
					}	
					case 0x410:		//F37	F20����ѯ��֤�Ĳɼ�����Ϣ_����ָ��
					{
						goto SS_VALIDATE_START_CMD_F20;
					}
					case 0x420:		//F38	F21����ѯ������ʾ����ʾ������Դ
					{
						goto GET_TUOPU_DISPLAY_PARA_F21;
					}		
					case 0x440:		//F39	F22����ѯ����ģ��ĳ������
					{
						goto GET_BS_INFO_PARA_F22;
					}			
					case 0x480:		//F40	F23����ѯ������ͬ��ʹ�ܲ���
					{
						goto GET_METER_INFO_SYN_PARA_F23;
					}		
					case 0x501:		//F41	F24����ѯI�Ͳɼ�����������ʹ�ܲ���
					{
						goto GET_BASE_UNDATA_PARA_F24;
					}	
					case 0x502:		//F42 	F25����ѯƵ�β���
					{
						goto GET_FREQ_CHANNEL_PARA_F25;
					}
					case 0x504:		//F43 	F26����ѯƵ�β���
					{
						goto GET_CHANNEL_SET_MODE_F26;
					}
					case 0x508:		//F44 	F28����ѯ�����������������
					{
						goto GET_JC_DISALLOW_FALG_F28;			
					}
					case 0x510:		//F45	F29����ѯ�ӽڵ�����ע�����
					{
						goto GET_BOARD_SEARCH_METER_FALG_F29;
					}
					
					case 0x602:		//F50	F1�������ز����ڵ��ַ 
					{
						goto SET_DEVICE_ADDR_F1;
					}	
					case 0x604:		//F51	F3�������㲥   
					{
						goto START_ADV_F3;
					}	
					case 0x608:		//F52	F4����������ͨ�Ų���
					{
						goto SET_SZ_COMM_PARA_F4;
					}	
					case 0x610:		//F53	F5������Ƶ�㶨��
					{
						goto SET_SZ_FREQ_PARA_F5;
					}	
					case 0x620:		//F54	F6�������ŵ����� 
					{
						goto SET_CHANNEL_PARA_F6;
					}	
					case 0x640:		//F55	F7������·�ɲ���
					{
						goto SET_ROUTE_PARA_F7;
					}	
					case 0x680:		//F56	F8�����òɼ�������ʹ�ܲ���
					{
						goto SET_SS_UPDATA_PARA_F8;
					}	
					case 0x701:		//F57	F9:���õ���ϻ�����
					{
						goto SET_METER_DEAD_COUNT_F9;
					}	
					case 0x702:		//F58	F10:���ù㲥������
					{
						goto SET_BROAD_METER_DATA_F10;
					}	
					case 0x704:		//F59	F11:���õ��ά������
					{
						goto SET_BROAD_SEARCH_METER_F11;
					}	
					case 0x708:		//F60	F12��ɨ�赥���ɼ����µĵ��
					{
						goto SET_SINGLE_SEARCH_METER_F12;
					}					
					case 0x710:		//F61	F13�����òɼ�����֤ʹ�ܲ���
					{
						goto SET_SS_VALIDATE_PARA_F13;
					}	
					case 0x720:		//F62	F14: ����������ʾ����ʾ������Դ
					{
						goto SET_TUOPU_DISPLAY_PARA_F14;
					}	
					case 0x740:		//F63	F15����������ģ��ĳ������
					{
						goto SET_BS_INFO_PARA_F15;
					}	
					case 0x780:		//F64	F16�����òɼ����������
					{
						goto SET_SS_COMM_PARA_F16;
					}
					case 0x801:		//F65	F17�����ò�����ͬ��ʹ�ܲ���
					{
						goto SET_METER_INFO_SYN_PARA_F17;
					}
					case 0x802:		//F66	F18������I�Ͳɼ�����������ʹ�ܲ���
					{
						goto SET_BASE_UNDATA_PARA_F18;
					}			
					case 0x804:		//F67	F19��Ƶ���ŵ�ת������
					{
						goto CHANNEL_CHANGE_CMD_F19;
					}
					case 0x808:		//F68	F20���òɼ��������շ�ģʽ
					{
						goto SET_SS_UPDATA_MODE_PARA_F20;
					}
					case 0x810:		//F22�����ü����������������	
					{
						goto SET_JC_DISALLOW_FLAG_F22;
					}
						
					case 0x902:		//F74	F1�����ز��ӽڵ�
					{
						goto ROUTE_SET_F1_ADD_SN;
					}			
					case 0x904:		//F75	F2ɾ���ز��ӽڵ�
					{
						goto ROUTE_SET_F2_DELETE_METER;
					}			
					case 0x908:		//F76	F3�����ز��ӽڵ�̶��м�·��(��ӵ��)
					{
						goto ROUTE_SET_F3_SET_ROUTE;
					}	
					case 0x910:		//F77	F4����·�ɹ���ģʽ
					{
						goto ROUTE_SET_F4_SET_ROUTE_MODE;
					}					
					case 0x920:		//F78	F6�����֤�Ĳɼ�����Ϣ
					{
						goto ROUTE_SET_F6_ADD_SS;
					}		
					case 0x940:		//F79	F7ɾ����֤�Ĳɼ�����Ϣ
					{
						goto ROUTE_SET_F7_DELETE_SS;
					}
					case 0x980:		//F80	F11����ӵ�������
					{
						goto ROUTE_SET_F11_ADD_METER;
					}
				/*	case 0xa01:		//F81	F5�������ز��ӽڵ�����ע��
					{
						goto ROUTE_SET_F5_START_SEARCH_METER;
					}*/
					case 0xa10:		//F85	F1���ز��ӽڵ�����
					{
						goto ROUTE_CHECK_F1_NODE_NUM;
					}
					case 0xa20:		//F86	F2���ز��ӽڵ���Ϣ
					{
						goto ROUTE_CHECK_F2_NODE_INFO;
					}
					case 0xa40:		//F87	F4��·������״̬
					{
						goto ROUTE_CHECK_F4_ROUT_STATUS;	
					}
					case 0xa80:		//F88	F5��δ�����ɹ����ز��ӽڵ���Ϣ
					{
						goto ROUTE_CHECK_F5_FAIL_SS;
					}
					default:
					{
						bDebugSend = 0;
						break;
					}
				}
			}		
			break;
		}
		case AFN_TYPE_INFORM:
		{
INFORM_RETURN:		
			if((bWaitForSendMeterAck) && (bMeterSearchStart) && (bSendMeterSearchSeq == pkt->rarea->stDown.seq))
			{
				for(loop = 0; loop < MAX_HASH_TABLE_FLASH_SIZE; loop++)
				{
					if((MeterStatus[loop].bReceive) && (MeterStatus[loop].bWaitAck))
						MeterStatus[loop].bAck = 1;
				}
				bWaitForSendMeterAck = 0;
			}
			else if((bMeterDeadStart) && (bNeedDeleteMeter) && (bNeedMeterDeleteAck))
			{
				for(loop = 0; loop < MAX_HASH_TABLE_FLASH_SIZE; loop++)
				{
					if((MeterStatus[loop].bNeedDelete) && (MeterStatus[loop].bDeleteWaitAck))
						Updata_Hash_Table(loop, NULL); 		//ɾ�����						
				}
				bNeedMeterDeleteAck = 0;
			}
			else if((bMeterDeadStart) && (0 == bNeedDeleteMeter) && (bNeedMeterDeleteAck))
			{
				//�޵��Ĳɼ�����ַ�ϱ��õ��ظ�
				bNeedMeterDeleteAck = 0;
			}							
			else if((bSingleSearchSSSendMeter) && (bSingleSearchSSSendMeterWaitAck))
			{
				for(loop = 0; loop < MAX_HASH_TABLE_FLASH_SIZE; loop++)
				{
					if((MeterStatus[loop].bSingleSSSearchMeterReceive) && (MeterStatus[loop].bWaitAck))
					{
						MeterStatus[loop].bSingleSSSearchMeterReceive = 0;
						MeterStatus[loop].bWaitAck = 0;
					}
				}			
				bSingleSearchSSSendMeterWaitAck = 0;
			}	
			else if((bBroadStart) && (bMesterDataWaitAck))
			{
				if(nMeterAckType)
					bNeedAckMeter = 1;
				else if(0 == MeterStatus[MeterID].bBroadMeterDataStatus)
					MeterStatus[MeterID].bBroadMeterDataStatus = 1;
				bMesterDataWaitAck = FALSE;	
				
#ifdef _Debug
				Test[0] = 0x33;
				Test[1] = 0x33;
				Test[2] = nMeterAckType;
//					Test[3] = MeterSourceID[MeterID];
				UART_PutArray(UART1, Test, 4);	
#endif    									
			}
            LOG_DEBUG( DBGFMT"recv confirm frame seq = %d \n",DBGARG, pkt->rarea->stDown.seq);

            if(g_stMuchframeindex_port.ucHead != g_stMuchframeindex_port.ucTail)
            {
                for(i = 0; i < CON_MUCHFRAME_NUM; i++)
                {
                    LOG_DEBUG( DBGFMT"much frame seq = %d \n",DBGARG, g_stMuchframeindex_port.ucSeq[i]);
                    if(g_stMuchframeindex_port.ucSeq[i] == pkt->rarea->stDown.seq)
                    {
                        g_stMuchframeindex_port.usLen[i] = 0;
                        break;
                    }
                    //i = (i + 1) % CON_MUCHFRAME_NUM;
                }

            }
			break;
		}
		case AFN_TYPE_REPORT:
		{
REPORT_RETURN:			
			if(pkt->dataflag == 0x08)
			{
				if(USER_DEFINE != USER_LuDeng)		
				{
					if(1 == MeterEventSendStatus) 
						MeterEventSendStatus = 2;
				}	
			}
			break;
		}
		default:
		{
			if(0 == bEZMACDebugType)
				Uart_Status = UART_STATUS_IDLE;
			else
				EZMAC_Debug_Status = EZMAC_DEBUG_STATUS_IDLE;
			break;
		}
        }
        return ;
}
#else
uint8 resetcmd_proc(UPPKT *pkt, STINFO3762 * upInfo, uint8 bEZMACDebugTypeTemp, DEBUG_UPPKT* pRecvPacketTemp)
{
    uint8 bError = 0; 
    switch(pkt->dataflag)
    {
        case 0x01://if (pkt->dataflag == 0x1)   //Ӳ����ʼ��                             
        {    
HW_INIT_F1:         
            SendReturnpkt(0);
            
            //��ʱ200MS
            DelayNS(100);
        //  Delay_ms(200);
            Reset();//��λ
        }
        
        break;          
        case 0x02://if (pkt->dataflag == 0x2)    //F2��������ʼ�� (�������)                           
        {
PARA_INIT_F2:           
            bError = 0;
            if(0 == bSendResetCmd)
            {
                memset(dis,0xff,32);
                for(i=0;i<32;i++)
                {                                      
                    I2cWrite(0xA0, dis, 0+(i*32), 32); //��λӰ�ӿռ�***
                    memset(shadow_space+(i*32),0xff,32);                         
                }
                IapAction = IAP_ACTION_ERASE_HASH1; //IAP��ʶλ
                while (IapAction != IAP_ACTION_IDLE);
                
                IapAction = IAP_ACTION_ERASE_HASH2; //IAP��ʶλ
                while (IapAction != IAP_ACTION_IDLE);   // Hold on  
                
                IapAction = IAP_ACTION_ERASE_WRITE_SS_HASH1; //IAP��ʶλ
                while (IapAction != IAP_ACTION_IDLE);
                
                IapAction = IAP_ACTION_ERASE_WRITE_SS_HASH2; //IAP��ʶλ
                while (IapAction != IAP_ACTION_IDLE);
                
                IapAction = IAP_ACTION_ERASE_BLACK_SS_HASH1; //IAP��ʶλ
                while (IapAction != IAP_ACTION_IDLE);
                
                IapAction = IAP_ACTION_ERASE_BLACK_SS_HASH2; //IAP��ʶλ
                while (IapAction != IAP_ACTION_IDLE);   
            }
            else
                bError = 1;

            SendReturnpkt(bError);  
            
            if(0 == bError)
            {
                //��ʱ200MS
                DelayNS(100);
            //  Delay_ms(200);
                Reset();//��λ                  
            }
            break;              
        }
        case if (pkt->dataflag == 0x4)  //F3 ��������ʼ��      
        {
DATA_INIT_F3:           
            //  Erase_FM_SN();//������������м�¼��SN
            SendReturnpkt(0);
            break;
        }
        
                    if (pkt->dataflag == 0x8)  //F4 �ָ���������
                    {
                        uint8 bValue = 0;
                        uint8 Temp[9];
                        bError = 0;
                        if(0 == bSendResetCmd)
                        {
                            memset(dis,0xff,32);
                            for(i=0;i<32;i++)
                            {                                      
                                I2cWrite(0xA0, dis, 0+(i*32), 32); //��λӰ�ӿռ�***
                                memset(shadow_space+(i*32),0xff,32);                         
                            }
                            IapAction = IAP_ACTION_ERASE_HASH1; //IAP��ʶλ
                            while (IapAction != IAP_ACTION_IDLE);
                            IapAction = IAP_ACTION_ERASE_HASH2; //IAP��ʶλ
                            while (IapAction != IAP_ACTION_IDLE);   // Hold on  
                            IapAction = IAP_ACTION_ERASE_WRITE_SS_HASH1; //IAP��ʶλ
                            while (IapAction != IAP_ACTION_IDLE);
                            IapAction = IAP_ACTION_ERASE_WRITE_SS_HASH2; //IAP��ʶλ
                            while (IapAction != IAP_ACTION_IDLE);
                            IapAction = IAP_ACTION_ERASE_BLACK_SS_HASH1; //IAP��ʶλ
                            while (IapAction != IAP_ACTION_IDLE);
                            IapAction = IAP_ACTION_ERASE_BLACK_SS_HASH2; //IAP��ʶλ
                            while (IapAction != IAP_ACTION_IDLE);   
        
                            //��ʼ��·�ɲ���
                            cltparm.routeuptime = 60;//����      
                            cltparm.maxttl = 120;//��������
                            cltparm.routeseltime = 30;//ѡ���ڵ�ʱ��    
                            I2cWrite(0xA0, (uint8*) &cltparm, FM_CLTP_ADDR, sizeof(CLTP)); //д��·�ɲ���         
        
                            //��ʼ���������
                            Temp[0] = 1;
                            Temp[1] = 6;
                            Temp[2] = 1;
                            Temp[3] = 1;
                            Temp[4] = 0;
                            Temp[5] = 1;
                            Temp[6] = 2;
                            Temp[7] = 0xfe;
                            Temp[8] = 0;
                            I2cWrite(0xA0, Temp, FM_NETPARA_ADDR, 8); //���������***
        
                            memset(Temp, 0, 9);
                            I2cWrite(0xA0, Temp, FM_DEVICE_ADDR , 6);
                        //  I2cWrite(0xA0, Temp, FM_BROAD_METER_TIME , 2); 
        
                            bValue = 0;
                        //  I2cWrite(0xA0, &bValue, FM_BROAD_METER_ENABLE , 1); 
                            I2cWrite(0xA0, &bValue, FM_SS_CODEUPDATA_ENABLE , 1); 
                            I2cWrite(0xA0,  &bValue, FM_I_SS_CODEUPDATA_ENABLE, 1);   
                            I2cWrite(0xA0,  &bValue, FM_CHANNEL_SET_MODE, 1);                               
        
                            bValue = METER_DEAD_COUNT;
                            I2cWrite(0xA0,  &bValue, FM_METER_DEAD_COUNT, 1);     
                
                            if(0 == pkt->data[13])
                                bValue = 1;      
                            else
                                bValue = 0;      
                            I2cWrite(0xA0, &bValue, FM_SS_VALIDATE_ENABLE , 1); 
                            I2cWrite(0xA0, &bValue, FM_METER_INFO_SYN_ENABLE, 1);                           
                        }
                        else
                            bError = 1;
                        
                        SendReturnpkt(bError);
                        if(0 == bError)
                        {
                            DelayNS(100);
                            Reset();//��λ
                        }
                        break;
                    }
                    
                    if(0 == bEZMACDebugType)
                        Uart_Status = UART_STATUS_IDLE;   
            }

}

void Handle_Up_PKT(UPPKT *pkt, uint8 bEZMACDebugTypeTemp, DEBUG_UPPKT* pRecvPacketTemp)
{
    uint8 id;
    uint8 temp;
    uint8 *deladd;
    uint8 updata_pkt_back[35]; //��������ظ���
    uint8 *p;
    SUBPKT *subpkt;
    HASHT elem;
    HASHT searchelem;
    HASHT* pHasht;  
    int ret;
    ROUTE temprt;
    DATAPKT *dpkt;
    uint8 *pxor;
    int loop;
    ImageBody *pImageBody;
    int ReturnHash = 0;
    uint8 recvid;
    uint8 find_id,dis[32] ;
    STAREQUEST *spkt;
    EXTDATA* pExtDta;
    int return_hash;    
//  uint8 SN[6]={0};
    uint16 handled_no,i;
    pkt->isneedack = 0;
    PKT *pSend;
    
    PKT* pEzTxPkt;
    uint8* pEzTxData;
    
    uint8 insertok = 1; //��ӳɹ���־λ         
    uint8 nStart;
    uint8 bError = 0;  

    uint16 nMeterNum = 0;
    uint16 nStartNum = 0;
    uint8 nSSNum = 0;
    
    uint8 nNeedNum;
    uint8 nCurSendNum = 0;
    uint8* pSn, *pSendData;
    
    
    WB_SPACE* pWriteSSSpace;
    uint8 nSw;
    //��Ϣ��
    STINFO3762 stinfo;
    memset((uint8 *)&stinfo, 0, sizeof(STINFO3762));
    stinfo.stUp.bit1.routeFlg = pkt->rarea->stDown.bit1.routeFlg;
    stinfo.stUp.bit1.moduleFlg = pkt->rarea->stDown.bit1.moduleFlg;
    stinfo.stUp.bit1.routeNum = pkt->rarea->stDown.bit1.routeNum;
    stinfo.stUp.bit2.channelFlg = pkt->rarea->stDown.bit2.channelFlg;
    stinfo.stUp.bit3.meterFlg = 0x05;
    stinfo.stUp.seq = pkt->rarea->stDown.seq;

    bEZMACDebugType = bEZMACDebugTypeTemp;
    pEZMACRecvPacket = pRecvPacketTemp;
    if(0 == bEZMACDebugType)
        bDebugSend = 0;
	switch (pkt->afn)
	{
		case AFN_TYPE_INITIALIZE:     //��ʼ��      
		    break;
        case AFN_TYPE_DATARELAY:
            break;
        case AFN_TYPE_DATAREQUES:
            break;
        case AFN_TYPE_LINKDET:
            break;
        case AFN_TYPE_CTLCMD:
            break;
        case AFN_TYPE_REPORT:
            break;
        case AFN_TYPE_ROUTE_POLL:
            break;
        case AFN_TYPE_ROUTE_SET:
            break;
        case AFN_TYPE_ROUTE_CTL:
            break;
        case AFN_TYPE_ROUTEDATARELAY:
            break;
        case AFN_TYPE_UPDATA:
            break;
        case AFN_TYPE_STATISTIC_REQUEST:
            break;
        default:
            break;
    }
}
#endif

uint8 xcnt=0;
/**************************************************************************************************
*
*
**************************************************************************************************/
void SendReturnpkt(uint8 bError, STINFO3762 * pstinfo)
{
    uint8 * p;
    uint8 len;
	if(bEZMACDebugType)
	{
		if(bError)
        {      
            p = negativepkt;
            len = 0x10;
            //SendPacketToEzmacProc(pEZMACRecvPacket, negativepkt, 0x10, 0); 	//�������ʧ����Ϣ���ظ���ӣ�
        }
        else
        {     
        
            p = informpkt;
            len = 0x13;
            //SendPacketToEzmacProc(pEZMACRecvPacket, informpkt, 0x13, 0);		//������ӳɹ���Ϣ
        }
        memcpy(p + 4, (uint8 *)pstinfo, 6);
        p[len - 2] = Check_SUM(p + 3, len - 5); //У��λ
        
        SendPacketToEzmacProc(pEZMACRecvPacket, p, len, 0);        //������ӳɹ���Ϣ
	}
	else		
	{
		if(bDebugSend)				
		{
            if(bError)
            {         
                p = negativepktForDebog;
                len = 0x10;
            
                //UART_PutArray(UART0, negativepktForDebog, 0x10); //�������ʧ����Ϣ���ظ���ӣ�	
            }
            else
            {         
                p = informpktForDebug;
                len = 0x13;
            
                //UART_PutArray(UART0, informpktForDebug, 0x13); //������ӳɹ���Ϣ	
            }
		}
		else
		{							
            if(bError)
            {
                p = negativepkt;
                len = 0x10;
                //UART_PutArray(UART0, negativepkt, 0x10); //�������ʧ����Ϣ���ظ���ӣ�	
            }
            else
            {         
                p = informpkt;
                len = 0x13;
            
                //UART_PutArray(UART0, informpkt, 0x13); //������ӳɹ���Ϣ
            }
		}
        
        memcpy(p + 4, (uint8 *)pstinfo, 6);
        p[len - 2] = Check_SUM(p + 3, len - 5); //У��λ
        
        UART_PutArray(UART0, p, len); //������ӳɹ���Ϣ
        
	}	
}

uint8 SendPacketToEzmacProc(DEBUG_UPPKT* pRecvPacket, uint8* pData, uint8 len, uint8 bNotRespone)
{
    return 0;
}
//	extern EZ_TX_PKT EzHHUTxPkt;//���������

uint8 SendPacketToMac(CTRL_BIT * ctrl_bit, uint8 fn, uint8 hop, STFUNCPARA * stAddr, STFUNCPARA * stData, uint8 seq, uint8 rssi, EZ_TX_PKT * etpkt)
{
	uint8 rc = 0;

    PKT *pkt;
    //uint8 recvid , bNotSend = 0;
    //JOINRSP *joinresp;
    //EXTDATA* pExtData;
    //uint8 bTypeExtData = 0;
    //ROUTE temprt;
    //uint8 m = 0;
    //uint16 whiteMeterNum = 0;
    if(NULL == etpkt || etpkt->bValid)//���籨�Ļ�����Ч
        return rc;

    pkt = &(etpkt->TxPkt);

        //uint8* pTxData =  pkt->data;

    pkt->ttl = 0xff;  //��������by ben 29

    memset((uint8 *)&pkt->head.apdu, 0, sizeof(STAPDU));
    //��������
    pkt->index = 0;
    
    pkt->head.apdu.ctrl = *ctrl_bit;
//	        pkt->head.apdu.ctrl.prm = 0;
//	        pkt->head.apdu.ctrl.evtmode = 1;
//	        pkt->head.apdu.ctrl.ftd = ctrl_bit->ftd;
    pkt->head.apdu.seq = seq;
    pkt->head.apdu.srssi = rssi;
    
    pkt->head.apdu.fn = fn;//ȷ��
    if(pkt->head.apdu.ctrl.dir)
    {
        pkt->head.apdu.stInfo.stUp.bit1.routeFlg = 0;
        pkt->head.apdu.stInfo.stUp.bit1.nodeFlg = 0;
        pkt->head.apdu.stInfo.stUp.bit1.addrFlg = 0;
        pkt->head.apdu.stInfo.stUp.bit1.conflict = 1;
        pkt->head.apdu.stInfo.stUp.bit1.routeNum = hop;//pkt->rarea->stDown.bit1.routeNum;
        pkt->head.apdu.stInfo.stUp.bit2.channelFlg = rfpara.rf_channel;//Cal_Hash_Value(nDeviceMacAddr) % LORA_CHANNEL_NUM;//pkt->rarea->stDown.bit2.channelFlg;
        pkt->head.apdu.stInfo.stUp.bit2.rssi = 0;
    }
    else
    {
        pkt->head.apdu.stInfo.stDown.bit1.routeFlg = 0;
        pkt->head.apdu.stInfo.stDown.bit1.nodeFlg = 0;
        pkt->head.apdu.stInfo.stDown.bit1.addrFlg = 0;
        pkt->head.apdu.stInfo.stDown.bit1.conflict = 1;
        pkt->head.apdu.stInfo.stDown.bit1.routeNum = hop;//pkt->rarea->stDown.bit1.routeNum;
        pkt->head.apdu.stInfo.stDown.bit2.channelFlg = rfpara.rf_channel;//Cal_Hash_Value(nDeviceMacAddr) % LORA_CHANNEL_NUM;//pkt->rarea->stDown.bit2.channelFlg;
        pkt->head.apdu.stInfo.stDown.bit2.errEncodeFlg = 0;
    }
    pkt->head.apdu.addrlen = stAddr->len+UNIQUE_MAC_ADDR_LEN;
    pkt->head.apdu.addr = pkt->data + PKT_HEAD_LEN;

    memcpy(pkt->head.apdu.addr, nDeviceMacAddr, UNIQUE_MAC_ADDR_LEN);//�ڵ�Ψһ��ʶ(����)    
    
    memcpy(pkt->head.apdu.addr+6, stAddr->data, stAddr->len);//�ڵ�Ψһ��ʶ(����)    


    pkt->head.apdu.data = pkt->data + PKT_HEAD_LEN + pkt->head.apdu.addrlen;

    memcpy(pkt->head.apdu.data, stData->data, stData->len);//�ڵ�Ψһ��ʶ(����)    
    pkt->head.apdu.len = stData->len;



    //*********************************************************�����

    etpkt->bValid = 1;//ҵ�񻺴���Ч
    etpkt->nBackOffSlot = 0;
    

	return rc;	
}

void Handle_RX_Down_Data_PKT(PKT *pkt, uint8 StartMode, uint8 SID, uint16 MeterID, uint8 bEzMacDebug) //�����߳������ݻظ�������
{
    uint8 *data; //���ڴ�Żظ�������������
    uint8 *pktdata; //
    uint8 afnoffset;
    DATAPKT *datapkt;
    HASHT* pHasht;
    datapkt = (DATAPKT*)(pkt->data); //����ַ

	pktdata = pkt->data + DATAPKT_PAYLOAD_OFFSET+1; //����ַ

    data = senduppkt.data; 

   // if (pktdata[0] != START_FLAG)
    //        return ;

    afnoffset = 22;
    
    senduppkt.afn = 0x02;
    senduppkt.dataflag = 0x01;
    //��ظ����������İ�
       
	data[0] = START_FLAG; //��ʼλ
	memcpy(data + 4, rarea, 6); //��Ϣ��
//	memcpy(data + 10, pktdata + 1, 6); // ����ַ
	memset(data + 10, 0, 6); // �ɼ�����ַ
	if(SID > 0)
		memcpy(data + 10, cltor[SID].devAddr, 6); // �ɼ�����ַ
	else
	{
		if(0 == bEzMacDebug)
		{
			if((ReqMeterID >= 2) && (ReqMeterID <= rfpara.rf_slotnum) /*&& (ReqMeterID < MAX_REAL_SUP_SS_NUM)*/)
				memcpy(data + 10, cltor[ReqMeterID].devAddr, 6);
		}
		else
		{
			if((EzMacReqMeterID >= 2) && (EzMacReqMeterID <= rfpara.rf_slotnum) /*&& (EzMacReqMeterID < MAX_REAL_SUP_SS_NUM)*/)
				memcpy(data + 10, cltor[EzMacReqMeterID].devAddr, 6);
		}
	}
	
//	memset(data + 16, 0, 6);
	memcpy(data + 16, nDeviceMacAddr, 6);	//�ز����ڵ�

	if(0 == HASH_Table_Used)
		pHasht = htable1;
	else
		pHasht = htable2;							
	
	if(2 == StartMode)
	{	
		//����¼����ĵ�����
		senduppkt.len = datapkt->paylen + 17+14;
		data[1] = senduppkt.len; //������
		data[2] = 0;
		data[3] = 0XCA; //����λ��������
		data[afnoffset] = AFN_TYPE_REPORT;
		data[afnoffset + 1] = 8;			//F4
		data[afnoffset + 2] = 0;

		data[afnoffset + 3] = SID;
		data[afnoffset + 4] = 0;
		data[afnoffset + 5] = 0;	// ��Լ����
		data[afnoffset + 6] = datapkt->paylen; //���ĳ���	
		memcpy(data + afnoffset + 7, pktdata, pkt->len); //���߽��յ�����
	} 	
	else if(1 == StartMode)
	{	
		//�㲥�������ݵ�����
		senduppkt.len = datapkt->paylen + 17+14;
		data[1] = senduppkt.len; //������
		data[2] = 0;
		data[3] = 0XCA; //����λ��������
		data[afnoffset] = AFN_TYPE_REPORT;
		data[afnoffset + 1] = 2;			//F2
		data[afnoffset + 2] = 0;

		data[afnoffset + 3] = SID;
		data[afnoffset + 4] = 0;
		if(MeterID >= MAX_HASH_TABLE_FLASH_SIZE)
			data[afnoffset + 5] = Check_Meter_Type(pktdata);
		else
			data[afnoffset + 5] = pHasht[MeterID].SubType;	// ��Լ����
		data[afnoffset + 6] = datapkt->paylen; //���ĳ���	
		memcpy(data + afnoffset + 7, pktdata, pkt->len); //���߽��յ�����		
	}
	else
	{
		//���㳭����������
		senduppkt.len = datapkt->paylen + 17+12-1;
		data[1] = senduppkt.len; //������
		data[2] = 0;
		data[3] = 0X8A;
		data[afnoffset] = AFN_TYPE_DATARELAY;
		data[afnoffset + 1] = 1;			//F1
		data[afnoffset + 2] = 0;		

		if(0 == bEzMacDebug)
			data[afnoffset + 3] = subtype_flag; // ��Լ����   ***����д��
		else
			data[afnoffset + 3] = EzMacSubTypeFlag;
			
		//data[afnoffset + 4] = datapkt->paylen; //���ĳ���
		data[afnoffset + 4] = datapkt->paylen-1; //���ĳ���

		//memcpy(data + afnoffset + 5, pktdata, pkt->len); //���߽��յ�����
		memcpy(data + afnoffset + 5, pktdata, datapkt->paylen-1); 
		
		if(0 == bEzMacDebug)
			Uppktacked = 1; //�Ѿ����������ظ���־λ
	}
	data[senduppkt.len - 2] = Check_SUM(data + 3, senduppkt.len - 5); //У��λ	
	data[senduppkt.len - 1] = END_FLAG; //0X16	
	
//		if(0 == bEzMacDebug)
	UART_PutArray(UART0, data, senduppkt.len); //�����������ͳ������ݰ�
//		else
//			SendPacketToEzmacProc(&(DebugPacket[nDebugPacketFront]), data, senduppkt.len, 1);
}

/*************************************************************************
 * Function Name: HandleRxUpNagePkt
 * Parameters:  ���ظ��ϲ�ķ񶨻ش�
 * Return:  none
 * Description: HandleRxUpNagePkt
 *
 *************************************************************************/

void Handle_RX_Up_Nage_PKT2(uint8 *pkt,uint8 type, uint8 bEZMACDebug, DEBUG_UPPKT* pEZMACRecvPacketTemp)
{
        uint8 data[28]={0};
        //uint8 afnoffset;

#if 0
        if (pkt->data[0] != START_FLAG)
                return ;
        data = senduppkt.data; //��ַ
        afnoffset = 22;

        senduppkt.len = pkt->len + 17+12;
        senduppkt.ctrl = 0x81;
        senduppkt.rarea = (INFO*)(data + OFFSET_UP_RAREA);
        senduppkt.afn = 0x02;
        senduppkt.dataflag = 0x01;

        memcpy(data + afnoffset + 5, pkt->data, pkt->len);
        data[0] = START_FLAG;
        data[1] = senduppkt.len;
        data[2] = 0;
        data[3] = senduppkt.ctrl;
        memcpy(data + 4, rarea, 6);
        memcpy(data + 10, pkt->data + 1, 6); // source address
        memcpy(data + 16, ccl_addr, 6); // dest address
        data[afnoffset] = senduppkt.afn;
        data[afnoffset + 1] = senduppkt.dataflag;
        data[afnoffset + 2] = 0;
        data[afnoffset + 3] = 0x1; // type
        data[afnoffset + 4] = pkt->len;
        data[senduppkt.len - 2] = Check_SUM(data + 3, senduppkt.len - 5);
        data[senduppkt.len - 1] = END_FLAG;
        
        #endif
	memcpy(data , negativepkt, 4);       
	memcpy(data+4 , powerpkt+4, 6);
	if(pkt!=NULL)
	{
		data[1]=28;//����
	//	memcpy(data+10 ,pkt+1, 6);//��Ϣ��
		memcpy(data+10 ,pkt, 6);//��Ϣ��	
		memset(data+17,0x0,6);
		if((bDebugSend) && (0 == bEZMACDebug))
			memcpy(data+22 , negativepktForDebog+10, 3);
		else
			memcpy(data+22 , negativepkt+10, 3);
		data[25] =type;//������
		data[26] = Check_SUM(data + 3, 23);
		data[27]=0x16;
		if(bEZMACDebug)
			SendPacketToEzmacProc(pEZMACRecvPacketTemp, data, 28, 0);
		else	
			UART_PutArray(UART0, data, 28); //
	}
	else
	{
		data[1]=16;//����
		if((bDebugSend) && (0 == bEZMACDebug))
			memcpy(data+10 , negativepktForDebog+10, 3);
		else
			memcpy(data+10 , negativepkt+10, 3);
		data[13] =type;//������
		data[14] = Check_SUM(data + 3, 22);
		data[15]=0x16;
		if(bEZMACDebug)
			SendPacketToEzmacProc(pEZMACRecvPacketTemp, data, 16, 0);
		else
			UART_PutArray(UART0, data, 16); //
	}
	if(0 == bEZMACDebug)
        	Uppktacked = 1; //�Ѿ����������ظ���־λ

}


/*************************************************************************
 * Function Name: HandleRxUpNagePkt
 * Parameters:  ���س���ʧ�ܱ���
 * Return:  none
 * Description: HandleRxUpNagePkt
 *
 *************************************************************************/

void Handle_RX_Up_Nage_PKT(PKT *pkt, uint8 bEZMACDebug, DEBUG_UPPKT* pEZMACRecvPacketTemp)
{
    uint8 *data;
    uint8 afnoffset;
    STINFO3762 stUpInfo;
    STINFO3762 *pstDownInfo;
    uint8 m = 0;
    uint8 type = pkt->data[7];
    if(pkt->data[0] != START_FLAG)
        return ;
    data = senduppkt.data; //��ַ
    afnoffset = 22;
    pstDownInfo = ((STINFO3762 *)&(negapkt.data[10]));

    memset((uint8 *)&stUpInfo, 0, sizeof(STINFO3762));
    stUpInfo.stUp.bit1.routeFlg = pstDownInfo->stDown.bit1.routeFlg;
    stUpInfo.stUp.bit1.moduleFlg = pstDownInfo->stDown.bit1.moduleFlg;
    stUpInfo.stUp.bit1.routeNum = pstDownInfo->stDown.bit1.routeNum;
    stUpInfo.stUp.bit2.channelFlg = rfpara.rf_channel & 0x0F;//pkt->stDownInfo.stDown.bit2.channelFlg;
    stUpInfo.stUp.bit3.meterFlg = 0x05;
    stUpInfo.stUp.seq = pstDownInfo->stDown.seq;


    senduppkt.len = pkt->len + 17+12;
    senduppkt.ctrl = 0x8a;
    senduppkt.rarea = (STINFO3762*)(&stUpInfo);
    senduppkt.afn = negapkt.data[9];
    senduppkt.dataflag = 0x01;

    memcpy(data + afnoffset + 5, pkt->data, pkt->len);
    data[m++] = START_FLAG;
//	    data[1] = senduppkt.len;
//	    data[2] = 0;
    m+=2;
    data[m++] = senduppkt.ctrl;
    memcpy(data + m, senduppkt.rarea, 6);
    m+=6;
    memcpy(data + m, pkt->data + 1, 6); // source address
    m+=6;
//     memcpy(data + 16, ccl_addr, 6); // dest address
    memcpy(data + m, nDeviceMacAddr, 6); // dest address
    m+=6;
    data[m++] = senduppkt.afn;
    data[m++] = senduppkt.dataflag;
    data[m++] = 0;
    
    if(senduppkt.afn == AFN_TYPE_ROUTEDATARELAY)
    {
        data[m++] = 0x00;//��ǰ���ı���ͨ������ʱ��
        data[m++] = 0x00;
    }
    
    data[m++] = type; // type
    data[m++] = pkt->len;
    data[m] = Check_SUM(data + 3, m - 3);
    m++;
    data[m++] = END_FLAG;
    data[1] = m & 0xFF; //����
    data[2] = m >> 8;

    senduppkt.len = m;

	if(bEZMACDebug)
		SendPacketToEzmacProc(pEZMACRecvPacketTemp, data, senduppkt.len, 0);	
	else
	{
		UART_PutArray(UART0, data, senduppkt.len); // 
		Uppktacked = 1; //�Ѿ����������ظ���־λ
	}
}

/*************************************************************************
 * Function Name: HandleRxUpNagePkt
 * Parameters:  ���س���ʧ�ܱ���
 * Return:  none
 * Description: HandleRxUpNagePkt
 *
 *************************************************************************/

void Handle_RX_Up_Nage_PKT_802(PKT *pkt)
{
    uint8 *data;
    uint16 m = 0, length = 0;
    uint8 frameNum = 0;
    data = senduppkt.data; //��ַ
    uint8 destAddrLen = 0, addrLen = 0;
    
    if(pkt->data[0] != START_FLAG)
        return ;

    pkt->head.mhr.seg_ctrl.ctrl_ld.bit3.LSF = 1;

    pkt->head.mhr.frame_ctrl.bit1.dir = 1;
    pkt->head.mhr.frame_ctrl.bit2.ie = 0;
    pkt->head.mhr.frame_ctrl.bit2.dest = 3;
    pkt->head.mhr.frame_ctrl.bit2.src = 3;


    memcpy(data + m, (uint8 *)&rfpara.panid, 2);
    m+=2;

    pkt->head.mhr.frame_ctrl.bit2.dest = 1;
    destAddrLen = nDeviceMacAddrLen;
    m++;
    memcpy(data+m, nDeviceMacAddr, destAddrLen);
#if (ADDR_MODE_ASCII > 0)
    uint8 devAddr[METER_ADDRESS_LENGTH_MAX];

    memset(devAddr, 0, METER_ADDRESS_LENGTH_MAX);
    destAddrLen = NumToHexString(devAddr, nDeviceMacAddr, nDeviceMacAddrLen, false);

    memcpy(data+m, devAddr, destAddrLen);
#endif
    data[m-1] = destAddrLen - 1;
    m += destAddrLen;
        
    memcpy(data + m, (uint8 *)&rfpara.panid, 2);
    m+=2;

//	        if(addrLen != CON_DEV_ADDR_LEN_8)
//	        {
    pkt->head.mhr.frame_ctrl.bit2.src = 1;//�䳤

    addrLen = pkt->data[1];
    m++;
    memcpy(data+m, pkt->data+2, addrLen);
#if (ADDR_MODE_ASCII > 0)

    memset(devAddr, 0, METER_ADDRESS_LENGTH_MAX);
    addrLen = NumToHexString(devAddr, pkt->data+2, addrLen, false);

    memcpy(data+m, devAddr, addrLen);
#endif
    data[m-1] = addrLen - 1;
    m += addrLen;
        
//	        buffer[m++] = ((addrLen-1) & 0x0F);
//	        
//	        memcpy(buffer+m, (uint8 *)addr, addrLen);
//	        m+=addrLen;

//	    memcpy(data + m, nDeviceMacAddr, CON_DEV_ADDR_LEN_8); // dest address
//	    m+=CON_DEV_ADDR_LEN_8;
//	    memcpy(data + m, &pkt->data[1], CON_DEV_ADDR_LEN_8); // src address
//	    m+=CON_DEV_ADDR_LEN_8;

    frameNum = pkt->data[11] & 0xf;

    memcpy(data + m, pkt->data+2+METER_ADDRESS_LENGTH_MAX, 3); // src address
    m+=3;
    memset(data + m, 0, frameNum);
    m+=frameNum;
    length = m + 3;
    pkt->head.mhr.seg_ctrl.ctrl_ld.lenl = (length & 0xff);
    pkt->head.mhr.seg_ctrl.ctrl_ld.bit2.lenh = (length >> 8);
    
    length = IE_Pack(&pkt->head.mhr.seg_ctrl, &pkt->head.mhr.frame_ctrl, pkt->head.mhr.seq, data, m);
    


    senduppkt.len = length;

	UART_PutArray(UART0, data, senduppkt.len); // 
	Uppktacked = 1; //�Ѿ����������ظ���־λ
}

/*************************************************************************
 * Function Name: HandleUpdateRequest
 * Parameters:  ��������
 * Return:  none
 * Description: ����ɼ����������󣬲����͸���Ӧ��
 *
 *************************************************************************/

void Handle_Update_Request(PKT *pkt)
{
#if 0
    //	uint8 recvid;
    uint8 id;
    uint8 blocknum;
    uint8 softver;
    //	DATAPKT* spkt = (DATAPKT *)pkt->data;

    id = pkt->data[DATAPKT_HEAD_LEN]; // Դ�ڵ��
    softver = pkt->data[DATAPKT_HEAD_LEN + 1]; // ����汾��
    blocknum = pkt->data[DATAPKT_HEAD_LEN + 2]; // �����������

    //	uint8* pData = pkt->data + DATAPKT_PAYLOAD_OFFSET + pkt->data[DATAPKT_PAYLOAD_OFFSET - 1];
    //	UpdataRouteInfo(id, pData, spkt->route.rdepth);		

    //�ñ�־λ
    uint8 type = 0;
#if (PROG_FLASH_512)  
    type = ((ImageBody *)IAP_SS_CODE_START)->hwCode.type;
#else
    BFLS_Read(&type, EXT_FLASH_SS_CODE_START+PROGRAM_SS_TYPE_ADDR, 1);//
    type >>= 5;//����5λ
#endif
    COLLECTOR* pCltor = &(cltor[id]);
    
    STMETERPARAFLASH stMeter;
    GetCltorPara(id, &stMeter);                        


    if((pCltor->devAddr[0] <= 0x99) && ((softver == adpkt.SoftVersion) || (softver == adv_pkt[I_SS_VERSION])) && (softver > 0)
        && (5 != type))
    {
		if(pCltor->nUpdataBlockNum == MAX_SS_UPDATA_BLOCK_NUM)
		{
			pCltor->nUpdataBlockNum = blocknum;
			pCltor->nUpdataPktTime = curslottime;

			#ifdef _Debug
			Test[0] = 0x22;
			Test[1] = id;
			Test[2] = softver;			
			Test[3] = blocknum;
			UART_PutArray(UART1, Test, 4);	
			#endif
		}
		else
		{
			#ifdef _Debug
			Test[0] = 0x11;
			Test[1] = id;
			Test[2] = softver;			
			Test[3] = blocknum;
			UART_PutArray(UART1, Test, 4);					
			#endif
		}
	}
	else
		pCltor->nUpdataBlockNum = MAX_SS_UPDATA_BLOCK_NUM;
		
		
		if(EzNetTxPkt.bValid)
			return;
		
		DATAPKT *reply = (DATAPKT*)(EzNetTxPkt.TxPkt.data);	
		
		//��������Ӧ���
		reply->ttl = MAX_TIME_LIVE;
		reply->destid = id;
		reply->route.rtype = MASTER_ROUTE_PATH;
		reply->route.downlink = ROUTE_DOWN_LINK; //���а�
		reply->dataseq = cltor[id].sendseq;
		reply->subtype = DATA_SUBTYPE_UPDATE_RESPONSE;
		
		EzNetTxPkt.TxPkt.data[DATAPKT_HEAD_LEN] = softver;
		EzNetTxPkt.TxPkt.data[DATAPKT_HEAD_LEN + 1] = blocknum;
		reply->paylen = 2+Get_Flash_Pro(EzNetTxPkt.TxPkt.data + DATAPKT_HEAD_LEN + 2, blocknum);//��FLASHȡ������
		
		ROUTE temprt = Fill_Route(EzNetTxPkt.TxPkt.data + DATAPKT_HEAD_LEN + reply->paylen,  MASTER_ROUTE_PATH, id); //�����ߵ�ַ��·��	
		if (temprt.rinfo.rtype != ERROR_ROUTE_PATH) 
		{
			reply->route.rdepth = temprt.rinfo.rdepth;
			recvid = temprt.did;

			EzNetTxPkt.TxPkt.len = DATAPKT_HEAD_LEN + (reply->route.rdepth) + reply->paylen;
			EzNetTxPkt.TxPkt.pkthead.ctrl.bits.msgtype = MSG_TYPE_DATA;
			//	EzNetTxPkt.TxPkt.pkthead.ctrl.bits.seq = cltor[id].sendseq;
			EzNetTxPkt.TxPkt.pkthead.ctrl.bits.DataFlag = 0;
			EzNetTxPkt.TxPkt.pkthead.ctrl.bits.UpFlag = 0; 
			EzNetTxPkt.TxPkt.pkthead.did = recvid;
			EzNetTxPkt.TxPkt.pkthead.sid = localid;
			EzNetTxPkt.TxPkt.pkthead.cid.all = (netcid << 4) | localhop;
			EzNetTxPkt.bValid = 1;
			EzNetTxPkt.nBackOffSlot = 0;
			cltor[id].sendseq++;
		}	
#endif 
}
/*************************************************************************
 * Function Name: Compose_3762Data
 * Parameters:  376.2������֡����
 *              flg :bit0-3:1��Ҫȷ�ϣ�0����Ҫȷ��;bit 4-7:��Ӧ�������bit4-7ctrl 
 *              data ��������
 *              len ���ݳ���
 *              addr ��ַ��
 *              addrLen ��ַ�򳤶�
 *              stinfo  ��Ϣ��
 *              afn     afn
 *              fn      fn
 * Return:  none
 *
 *************************************************************************/

uint8 Compose_3762Data(uint8 flg, uint8 * data, uint16 len, uint8 * addr, uint8 addrLen, STINFO3762 * stinfo, uint8 afn, uint16 fn)
{
    uint8 ml = 0;
    senduppkt.data[ml++] = START_FLAG;
    ml+=2;
    senduppkt.data[ml++] = (flg & 0xC0) | 0xA; //������:�����ϱ�����
    
    memcpy(senduppkt.data + ml, (uint8 *)stinfo, 6);
    ml+=6;
    if(stinfo->stUp.bit1.moduleFlg == 1)
    {
        memcpy(senduppkt.data + ml, addr, addrLen);
        ml+=addrLen;
        memcpy(senduppkt.data + ml, nDeviceMacAddr, 6);
        ml+=6;
    }
    senduppkt.data[ml++] = afn; //AFN
//	    memcpy(senduppkt.data+ml, (uint8 *)&fn, 2);
//	    ml+=2;
    if(fn < 1)
    {
        return 1;
    }
    
    memcpy(senduppkt.data+ml,(uint8 *)&fn,2);
    ml+=2;
    
//	    senduppkt.data[ml++] = 1 << ((fn - 1) % 8); //���ݵ�Ԫ��ʶ
//	    senduppkt.data[ml++] = fn / 8;
     
    memcpy(&senduppkt.data[ml], data, len);
    //ȥ��SN�ŵ�����ֽ�
    ml+=len;
    senduppkt.data[ml] = Check_SUM(senduppkt.data + 3, ml - 3); //У��λ
    ml++;
    senduppkt.data[ml++] = END_FLAG;
    
    senduppkt.data[1] = ml & 0xFF; //����
    senduppkt.data[2] = ml >> 8;
    
    senduppkt.len = ml;
    if(g_ucUpgradeFlgForPush != 0xAA)
    {
        UART_PutArray(UART0, senduppkt.data, senduppkt.len); //�����������ͳ������ݰ�
        if(flg&0x0f)
        {
            muchframe_fill(senduppkt.data, senduppkt.len, &g_stMuchframeindex_port, CON_SOUR_PORT, stinfo->stUp.seq);
        }
    }
    else
    {
        LOG_DEBUG( DBGFMT"we are upgrading now, pause send push data  \n",DBGARG);                        
    }
    return 0;
}
#define SIGNAL_QUALITY_LEVEL    15
/*************************************************************************
 * Function Name: getSignalQuality
 * Parameters:  ��ȡ�ź�����  1-15
 *              rssi :�յ����ź�����
 *              max  ��õ��ź�
 *              low  ���������
 * Return:  none
 *
 *************************************************************************/

uint8 getSignalQuality(int rssi, int max, int low)
{
    float x;
    uint8 ret;
    x = (float)(max - low) / SIGNAL_QUALITY_LEVEL;

    ret = (uint8)((float)(-(rssi - max)) / x);

    return ret;
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
void upcomm_mng()
{
    //uint8 i;
    if(recvuppkt.isneedack)
    {
        if(recvuppkt.deadtime == 0)
        {
            if (StaAck == PKT_TYPE_SS_DATA)  // ��ʱ��û�յ�ģ�Ⳮ��ظ�
            {
                UART_PutArray(UART0, senduppkt.data, senduppkt.len);//����ģ�Ⳮ��ʧ�ܻش�
                StaAck = 0;
            }
            else if (StaAck == PKT_TYPE_TOPO)  // ��ʱδ�յ����˻ظ�
            {
                UART_PutArray(UART0, senduppkt.data, senduppkt.len);//��������ʧ�ܻش�
                StaAck = 0;
            }
            else if(StaAck == PKT_TYPE_SS_SEARCH_METER) //�Ե����ɼ������в�������Ļظ�
            {
            //�ӷ񶨻ش�
                Handle_RX_Up_Nage_PKT2(&recvuppkt.data[13],FIND_NO_ADDR, 0, NULL);
            }
            else if(StaAck == PKT_TYPE_DATA) // ����ҵ����ķ񶨻ش�
            {                                                         
                Handle_RX_Up_Nage_PKT(&negapkt, 0, NULL);
            }     
            else if(StaAck == PKT_TYPE_DATA_802) // ����ҵ����ķ񶨻ش�
            {                                                         
                Handle_RX_Up_Nage_PKT_802(&negapkt);
            }       
            else if(StaAck == PKT_TYPE_SS_NET_SET)
            {
            //�ӷ񶨻ش�
                Handle_RX_Up_Nage_PKT2(&recvuppkt.data[13],FIND_NO_ADDR, 0, NULL);                  
            }
            recvuppkt.isneedack = 0;        
            Uppktacked = 1;//�ѻظ�
        }
    }

#ifdef UPCOMM_PORT	
	/*���Զ˿ڶ�֡����*/
	if(g_stMuchframeindex_port.ucHead != g_stMuchframeindex_port.ucTail)
	{
	    if(g_stMuchframeindex_port.ttl == 0)
        {
			if(g_stMuchframeindex_port.usLen[g_stMuchframeindex_port.ucTail] != 0 &&
                g_stMuchframeindex_port.ucTimes[g_stMuchframeindex_port.ucTail] < 2)
			{
				//getdata_bytype(ucTmp, DT_MUCHFRAME_DEBUGP_DATA, g_stMuchframeindex_port.usLen[i], g_stMuchframeindex_port.usAddr[i]);
                memcpy(senduppkt.data, g_stMuchframeindex_port.ucData[g_stMuchframeindex_port.ucTail], g_stMuchframeindex_port.usLen[g_stMuchframeindex_port.ucTail]);
                senduppkt.len = g_stMuchframeindex_port.usLen[g_stMuchframeindex_port.ucTail];
                UART_PutArray(UART0, senduppkt.data, senduppkt.len);//��������ʧ�ܻش�
				//set_debugport_senddata(0, ucTmp, g_stMuchframeindex_port.usLen[i]);
				//g_stMuchframeindex_port.usLen[i] = 0;
				
				//break;
				g_stMuchframeindex_port.ttl = 0x80;//2��
				g_stMuchframeindex_port.ucTimes[g_stMuchframeindex_port.ucTail]++;
			}
            else
            {
                g_stMuchframeindex_port.usLen[g_stMuchframeindex_port.ucTail] = 0;
                g_stMuchframeindex_port.ucTail++;
                if(g_stMuchframeindex_port.ucTail >= CON_MUCHFRAME_NUM)
                {
                    g_stMuchframeindex_port.ucTail = 0;
                }
            }
        }   
//			for(i = 0; i < g_stMuchframeindex_port.ucNum; i ++)
//			{
//				if(g_stMuchframeindex_port.usLen[i] != 0)
//				{
//					//getdata_bytype(ucTmp, DT_MUCHFRAME_DEBUGP_DATA, g_stMuchframeindex_port.usLen[i], g_stMuchframeindex_port.usAddr[i]);
//	                memcpy(senduppkt.data, g_ucPushDataShadow + g_stMuchframeindex_port.usAddr[i]);
//	                senduppkt.len = g_stMuchframeindex_port.usLen[i];
//	                UART_PutArray(UART0, senduppkt.data, senduppkt.len);//��������ʧ�ܻش�
//					//set_debugport_senddata(0, ucTmp, g_stMuchframeindex_port.usLen[i]);
//					//g_stMuchframeindex_port.usLen[i] = 0;
//					
//					break;
//				}
//			}
		//if(i >= g_stMuchframeindex_port.ucNum)
		//	memset((unsigned char*)(&g_stMuchframeindex_port), 0, sizeof(g_stMuchframeindex_port));
	}
#endif

    
}
/*************************************************************************
 * Function Name: HandleRxDoPkt
 * Parameters:  downlink packet
 * Return:  none
 * Description: handle the downlink packet
 *
 *************************************************************************/
//	#define MSG_TYPE_CONFIRM        0x0
//	#define MSG_TYPE_CTRL           0x1
//	#define MSG_TYPE_RELAY          0x2
//	#define MSG_TYPE_UP             0x3
//	#define MSG_TYPE_BROADCAST      0x4
//	#define MSG_TYPE_QUERY          0x5
//	#define MSG_TYPE_ROUTE_MNG      0x6
//	#define MSG_TYPE_ROUTE_REQ      0x7
//	#define MSG_TYPE_LINK_DETECT    0x8
//	#define MSG_TYPE_FILE           0x9
//extern uint8 g_ucfactory_mod;

#ifdef RF_MESH_LEVEL2
uint8 Handle_RX_Down_PKT(PKT *pkt) //�������߽�������
{

    RfFrmProc(PST_CHN_WL, pkt->protocol, pkt->data, pkt->len, senduppkt.data, (uint8)sizeof(senduppkt.data));

    return 0;
}
#else

uint8 Handle_RX_Down_PKT(PKT *pkt) //�������߽�������
{
	uint8 rc = 0;
    uint16 i,id = 0;
    uint16 ml = 0;
	
	uint8 bNeedReAllocate = FALSE;
    uint8 tmp[27];
    CTRL_BIT ctrl_bit;
	
    STINFO3762 stinfo;
    uint16 /*usSeq = 0,*/ frameLen = 0;
    uint8 len = 0, oneAddrLen = 0;
    uint8 SN[6];
    uint8 bDebugFlg = FALSE;
    uint8 berr = FALSE;
    uint8 fn;
    uint8 m = PKT_HEAD_LEN + pkt->head.apdu.addrlen;
    uint8 * p;
	
    STFUNCPARA stAddr;
    STFUNCPARA stData;

    if(pkt->head.apdu.stInfo.stUp.bit1.addrFlg == 1)
    {
       oneAddrLen = 2;
    }
    else
    {
       oneAddrLen = 6;
    }

    if(oneAddrLen != 6)
    {
       id = pkt->head.apdu.addr[1] + pkt->head.apdu.addr[0] * 0x100;
        
        if(id > MAX_SUP_SS_NUM)
        {//������
            id = 0;
        }
        
        memcpy(SN, cltor[id].devAddr, 6);
       
    }
    else
    {
       id = getIdWithAddr(&pkt->head.apdu.addr[0]);
       if(id > MAX_SUP_SS_NUM)
       {//������
           id = 0;
       }
       memcpy(SN, pkt->head.apdu.addr, 6);

    }

    frameLen = pkt->data[1] + pkt->data[2] * 0x100;
    
    memset((uint8 *)&stinfo, 0, sizeof(STINFO3762));
    memset((uint8 *)&ctrl_bit, 0, 1);
    
    double rssi = (int)calculateRssi(pkt->head.apdu.snr, pkt->head.apdu.rssi);

    LOG_DEBUG( DBGFMT"CH[%d] rf recv ftd[%d] fn[%d] rssi[%4.1f] %02x%02x%02x%02x%02x%02x\n",DBGARG, 
        rfpara.rf_channel, pkt->head.apdu.ctrl.ftd, pkt->head.apdu.fn, rssi, 
        pkt->head.apdu.addr[5], pkt->head.apdu.addr[4],pkt->head.apdu.addr[3],pkt->head.apdu.addr[2],pkt->head.apdu.addr[1],pkt->head.apdu.addr[0]);
    LOG_DUMPHEX(LOG_LEVEL_DEBUG, "HD : \n", pkt->data, pkt->len);
    if(frameLen < 18)
    {
        LOG_DEBUG( DBGFMT"frame addr %02x%02x%02x%02x%02x%02x wrang data\n",DBGARG, 
            pkt->head.apdu.addr[5], pkt->head.apdu.addr[4],pkt->head.apdu.addr[3],pkt->head.apdu.addr[2],pkt->head.apdu.addr[1],pkt->head.apdu.addr[0]);
        return 0;
    }
    //g_ucPktRssiValue[pkt->head.apdu.seq] = (uint8)(0 - rssi) ;

    
    
    
    if(Meter_Check(SN))
    {
        bNeedReAllocate = TRUE;
    }
    else
    {   
        if(memcmp(SN, sBroadAddrFE, 6) == 0)
        {
            if(pkt->head.apdu.broadCastFlg == 0)
            {
                bDebugFlg = TRUE;
            }
            LOG_DEBUG( DBGFMT" HHU read data\n",DBGARG);
        }
        else
        {
            LOG_DEBUG( DBGFMT" addr does not at my white space %02x%02x%02x%02x%02x%02x\n",DBGARG, pkt->head.apdu.addr[5], pkt->head.apdu.addr[4],pkt->head.apdu.addr[3],pkt->head.apdu.addr[2],pkt->head.apdu.addr[1],pkt->head.apdu.addr[0]);
        }
    }

	switch (pkt->head.apdu.ctrl.ftd)//pkthead.ctrl.bits.msgtype) //�ж�����
	{
	    case MSG_TYPE_CONFIRM:
            if(bNeedReAllocate)
            {
                id = Sn_Search_Id(SN);
                if ((id >= SUP_SS_INDEX_START) && ( id <= MAX_SUP_SS_NUM) )
                {
                    if(cltor_shadow[id].nod.bNeedDelete)
                    {
                        int ret;
    					ret = Hash_Table_Search(cltor[id].devAddr); //���ҹ�ϣ���е�����ֵ				
                        if (ret != ( - 1))
                        {                     
                            Updata_Hash_Table(ret, NULL);  //ɾ��                                              

                            //ɾ����֤�ɼ�������
                            //Del_Validate_W_B(cltor[id].devAddr);
                        }
    			        Delete_SS_Node(id);
                    }
                }
            }
            break;

        case MSG_TYPE_QUERY:
        {
            switch(pkt->head.apdu.fn)
            {
                case MSG_TYPE_FN_10:
                {
                    if(bNeedReAllocate)
                    {
                        uint8 version[2];// segmentSeq = pkt->head.apdu.data[2];
                        uint8 flashVersiong[4];
                        uint8 success = 0;
                        BFLS_Read((uint8 *)flashVersiong, EXT_FLASH_SS_CODE_START+10, 4);

                        memcpy(version, pkt->head.apdu.data, 2);
                        
                        //memcpy(longVersion, pkt->head.apdu.data+2, 20);

                        success = !(memcmp(version,flashVersiong,2));
                        
                        id = Sn_Search_Id(SN);
                        
                        if(id != g_stUpgrade.p2pSeq ||
                            pkt->head.apdu.seq != g_stUpgrade.frameSeq)//�κźͱ��ַ��һ����copy
                        {
                            g_stUpgrade.delay = 0;
                            break;
                        }
                        //memcpy(g_stUpgrade.bitMap, pkt->head.apdu.data+4, bitMapLen);
                        g_stUpgrade.eventFlg = 0x55;
                        
                        stinfo.stUp.bit1.moduleFlg = 0;
                        stinfo.stUp.bit3.meterFlg = 0x05;
                        stinfo.stUp.seq = cltor_shadow[1].sendseq++;
                        stinfo.stUp.bit4.recvSignal = getSignalQuality((int)rssi, RSSI_OFFSET_MF, RSSI_OFFSET_HF);
                        fn = 7;
                        if(oneAddrLen == 2)
                        {
                            memcpy(tmp, cltor[id].devAddr, 6);

                        }
                        else
                        {
                            memcpy(tmp, pkt->head.apdu.addr, oneAddrLen);
                        }
                        ml+=6;
                        tmp[ml++] = success;
                        
                        memcpy(tmp+ml, pkt->head.apdu.data+2, 20);
                        ml+=20;

                        
                        Compose_3762Data(0xC0, tmp, ml, NULL, 0, &stinfo, 6, ((fn / 8) * 0x100 + (1 << ((fn - 1) % 8))));
                        
                        g_stUpgrade.crc = CRC_16BIT_Check1((uint8 *)&g_stUpgrade, sizeof(STUPGRADE) - 2);
                        I2cWrite(0xA0,  (uint8 *)&g_stUpgrade, DP_DOWNLOAD_PROC_PARA_ADDR, sizeof(STUPGRADE));  
                    }
                    
                    break;
                }
                break;
            
            default:
                break;
            }            
            break;
        }
	    case MSG_TYPE_UP:
        {
            switch(pkt->head.apdu.fn)
            {
                case MSG_TYPE_FN_14:
                {
//	                    u32SendTickToSSDeadTime = curslottime + 40; //�յ��������ظ���ʱ���ڴ�������
                    
                    if(bNeedReAllocate)
                    {
                    
                        len = frameLen - m - 1 - 2;//pkt->data[m++];
                        stinfo.stUp.bit1.moduleFlg = 1;
                        stinfo.stUp.bit3.meterFlg = 0x05;
                        stinfo.stUp.bit4.recvSignal = getSignalQuality((int)rssi, RSSI_OFFSET_MF, RSSI_OFFSET_HF);
                        stinfo.stUp.seq = cltor_shadow[1].sendseq++;
                        fn = 6;

                        Compose_3762Data(0xC1, pkt->data+m+2,len,&pkt->data[PKT_HEAD_LEN],
                                /*pkt->head.apdu.addrlen-oneAddrLen*/ oneAddrLen, &stinfo, 6, ((fn / 8) * 0x100 + (1 << ((fn - 1) % 8))));

                        id = Sn_Search_Id(SN);

                        LOG_DEBUG( DBGFMT" search id = %d\n",DBGARG, id);
    					
    					cltor_shadow[id].nodestatus.errCode = 0;

                        if ((id > MAX_SUP_SS_NUM) ||( id < SUP_SS_INDEX_START) || (id > rfpara.rf_slotnum)/* || (id >= MAX_REAL_SUP_SS_NUM)*/)
                        {               
#ifdef PROJECT_PINGSHUI
                            break;       
#else
                            id = 0;                 //��û�з��䵽��ЧIDʱ�����䵽0�����ڻظ�SS 
                            cltor[id].addrLen = CON_DEV_ADDR_LEN_6;
                            memset(cltor[id].devAddr, 0, CON_DEV_ADDR_LEN_10);
                            
                            memcpy(cltor[0].devAddr, SN, 6);//����SN
                            
                            cltor_shadow[id].nodestatus.errCode = 6;
#endif
                        }
                        
					} 
					else
                    {
                        id = 0;
                        cltor[id].addrLen = CON_DEV_ADDR_LEN_6;
                        memset(cltor[id].devAddr, 0, CON_DEV_ADDR_LEN_10);
                        
                        memcpy(cltor[id].devAddr, SN, 6);  // ��¼��ӽڵ��Ψһ��ʶSN 
                        
    					cltor_shadow[id].nodestatus.errCode = 7;
                        if(pkt->head.apdu.broadCastFlg == 1)
                            break;
                    }               
                    //cltor[id].old_id=id;
//	                    cltor[id].nodestatus.fixed = 1;             //��ʾ�õ��  
                    cltor_shadow[id].recvseq = pkt->head.apdu.seq;
                    cltor_shadow[id].FrameDeadTime=  0xff;     
                    cltor_shadow[id].nod.bNeedAck = 1;      //��������
                    cltor_shadow[id].rRssi = (uint8)(0 - rssi);
                    //�����м̽ڵ�
                    UpdataRouteInfo1(id, pkt);//joinreq->route.rdepth);

                    if(id > 1)
                    {
                        SyncCltorPara(id, &cltor[id], NULL);
                    }
                    //LOG_DEBUG( DBGFMT" id = %d push data decompose done !\n",DBGARG, id);
                }
                break;
            }
            break;
        }
        case MSG_TYPE_RELAY:
        {
            if(bNeedReAllocate)
            {
                STINFO3762 *pstDownInfo;

                ml = 0;
                senduppkt.data[ml++] = START_FLAG;
                ml+=2;
                senduppkt.data[ml++] = 0x82; //������:͸���ظ�
                //������
                stinfo.stUp.bit1.routeFlg = 1;//pkt->head.apdu.stInfo.stUp.bit1.routeFlg;
                stinfo.stUp.bit1.moduleFlg = 1;//pkt->head.apdu.stInfo.stUp.bit1.moduleFlg;�Դӽڵ����
                stinfo.stUp.bit1.routeNum = 0;//pkt->head.apdu.stInfo.stUp.bit1.routeNum;
                stinfo.stUp.bit3.meterFlg = 0x05;
                stinfo.stUp.bit4.recvSignal = getSignalQuality((int)rssi, RSSI_OFFSET_MF, RSSI_OFFSET_HF);//pkt->head.apdu.stInfo.stUp.bit2.rssi;
                stinfo.stUp.bit3.phaseFlg = pkt->head.apdu.stInfo.stUp.bit3.phaseFlg;
                pstDownInfo = (STINFO3762 *)&negapkt.data[10];

                stinfo.stUp.seq = pstDownInfo->stDown.seq;//pkt->stDownInfo.stDown.seq;
                
                memcpy(senduppkt.data + ml, &stinfo, 6);
                ml+=6;
                memcpy(senduppkt.data + ml, &pkt->data[PKT_HEAD_LEN], 6);
                ml+=6;
                memcpy(senduppkt.data + ml, nDeviceMacAddr, 6);
                ml+=6;

//	                memcpy(senduppkt.data + ml, &pkt->data[PKT_HEAD_LEN], pkt->head.apdu.addrlen);
//	                ml+=pkt->head.apdu.addrlen;
                senduppkt.data[ml++] = negapkt.data[9];//0x02; //AFN
                senduppkt.data[ml++] = 0x01; //���ݵ�Ԫ��ʶ
                senduppkt.data[ml++] = 0x00;
                if(negapkt.data[9] == AFN_TYPE_ROUTEDATARELAY)
                {
                    senduppkt.data[ml++] = 0x00;//��ǰ���ı���ͨ������ʱ��
                    senduppkt.data[ml++] = 0x00;
                }
                senduppkt.data[ml++] = pkt->head.apdu.stInfo.stUp.bit3.protocolFlg;
                senduppkt.data[ml++] = pkt->head.apdu.len;
                
                memcpy(&senduppkt.data[ml], &pkt->data[m], pkt->head.apdu.len);
                //ȥ��SN�ŵ�����ֽ�
                ml+=pkt->head.apdu.len;
                
                senduppkt.data[ml] = Check_SUM(senduppkt.data + 3, ml - 3); //У��λ
                ml++;
                senduppkt.data[ml++] = END_FLAG;
                
                senduppkt.data[1] = ml & 0xFF; //����
                senduppkt.data[2] = ml >> 8;
                
                senduppkt.len = ml;
                UART_PutArray(UART0, senduppkt.data, senduppkt.len); //�����������ͳ������ݰ�
                
//	                LOG_DEBUG( DBGFMT"uart send p2p data    \n",DBGARG);
//	                LOG_DUMPHEX(LOG_LEVEL_DEBUG, "HexData : \n", senduppkt.data, senduppkt.len);
                
                {
                    recvuppkt.isneedack = 0;
                    Uppktacked = 1; //�Ѿ����������ظ���־λ                        
                }

            }
            break;
        }
        case MSG_TYPE_ROUTE_REQ:
		{		
            uint8 nFoundNum = 0; 
            uint16 nFoundId = 0;
            STMETERPARAFLASH stMeter;
            uint8 type;
            switch(pkt->head.apdu.fn)
            {
                case MSG_TYPE_FN_03://��½
                {
                    
                    if(bNeedReAllocate)
                    {
						for(i = SUP_SS_INDEX_START; i < MAX_SUP_SS_NUM + 1; i++)	//����
						{
							if(!LPC_Memory_CMP(cltor[i].devAddr, SN, 6))//�Ա�һ��
							{
								nFoundNum++;		
								nFoundId = i;
							}
						}	
						if(0 == nFoundNum)
							id = Re_Allocate_Id(SN, 6);	//����SN���·���
						else if(1 == nFoundNum)
							id = nFoundId;
						else
						{
							for(i = SUP_SS_INDEX_START; i < MAX_SUP_SS_NUM + 1; i++)	//����
							{
								if(!LPC_Memory_CMP(cltor[i].devAddr,SN, 6))//�Ա�һ��
									Delete_SS_Node(i);						
							}							
							id = Re_Allocate_Id(SN, 6);	//����SN���·���
						}                                                
						//	id = Re_Allocate_Id(joinreq->SN);	//����SN���·���    
                        cltor_shadow[id].nod.result = 0;
                        
    					if ((id > MAX_SUP_SS_NUM) ||( id < SUP_SS_INDEX_START) || (id > rfpara.rf_slotnum)/* || (id >= MAX_REAL_SUP_SS_NUM)*/)
    					{				
#ifdef PROJECT_PINGSHUI
                            break;       
#else
                            id = 0;					//��û�з��䵽��ЧIDʱ�����䵽0�����ڻظ�SS	
                            cltor[id].addrLen = CON_DEV_ADDR_LEN_6;
                            memset(cltor[id].devAddr, 0, CON_DEV_ADDR_LEN_10);
                            
                            memcpy(cltor[0].devAddr, SN, 6);//����SN
                            
                            cltor_shadow[id].nod.result = 3;
                            break;//���ظ�
#endif
    					}
                        //cltor_shadow[id].neighnum = 2;//joinreq->neignum;  //�ھӸ���                         
                        uint8 heartBeat = pkt->head.apdu.data[10];
                        if(heartBeat > cltparm.routeuptime)
                        {
                            cltparm.routeuptime = heartBeat;
                            
                            I2cWrite(0xA0, (uint8*) &cltparm, FM_CLTP_ADDR, 1); //д��·�ɲ���         
                        }
                                    
                        //�����м̽ڵ�
                        
                    }
                    else
                    {
                        if( Check_Black(SN, 6))
                        {
                            break;
                        }
                        if((uint8)(0 - rssi) > rfpara.rf_limit)
                        {
                            break;
                        }
                        type = pkt->head.apdu.data[1] >> 4;
                        if(bBroadMeterEnable || (type == 0x02))//����ע������м���
                        {
                            for(i=SUP_SS_INDEX_START;i<MAX_SUP_SS_NUM+1;i++)  //����
                            {
                                if(!LPC_Memory_CMP(cltor[i].devAddr, SN, 6))//�Ա�һ��
                                {
                                    nFoundNum++;        
                                    nFoundId = i;
                                }
                            }   
                            if(0 == nFoundNum)
                                id = Re_Allocate_Id(SN, 6);    //����SN���·���
                            else if(1 == nFoundNum)
                                id = nFoundId;
                            else
                            {
                                for(i=SUP_SS_INDEX_START;i<MAX_SUP_SS_NUM;i++)  //����
                                {
                                    if(!LPC_Memory_CMP(cltor[i].devAddr,SN, 6))//�Ա�һ��
                                        Delete_SS_Node(i);                      
                                }                           
                                id = Re_Allocate_Id(SN, 6);    //����SN���·���
                            }    

        					if ((id > MAX_SUP_SS_NUM) ||( id < SUP_SS_INDEX_START) || (id > rfpara.rf_slotnum))
                            {
                                //���������������򲻻ظ�
                                break;
                            }               
                        
                            cltor_shadow[id].nod.result = 4;
                        }
                        else
                        {
                    
                            id = 0;
                            cltor_shadow[id].nod.result = 2;
                            if(pkt->head.apdu.broadCastFlg == 1)
                                break;
                        }
                    }
                    GetCltorPara(id, &stMeter);
                    memcpy(stMeter.GIS, pkt->head.apdu.data+2, 8);
                    memcpy((uint8 *)&stMeter.softver, pkt->head.apdu.data+11, 2);

                    stMeter.nHwType = pkt->head.apdu.data[13];//joinreq->softver;//�ڵ�汾��   
                    cltor[id].addrLen = CON_DEV_ADDR_LEN_6;
                    memset(cltor[id].devAddr, 0, CON_DEV_ADDR_LEN_10);

                    memcpy(cltor[id].devAddr, SN, 6);  // ��¼��ӽڵ��Ψһ��ʶSN 
                    
                    cltor_shadow[id].nod.ans_pkt_type=0x1;     //��������ʶλ
//	                    cltor[id].nodestatus.fixed= 0x0;           //��ʾ�õ�δ��
                    cltor_shadow[id].nod.needack= 0x1;         //��ʾ�õ���Ҫ�ظ�
//	                    cltor[id].nodestatus.handled = UN_HANDLED;   //���±��Ϊδ����
                    cltor[id].nodestatus.NetStatus = NODE_STATUS_OUT;
                    //cltor[id].old_id=id;//joinreq->sourid;        //ԭ���ϱ��Ľڵ��
                    cltor_shadow[id].recvseq = pkt->head.apdu.seq;
                    cltor_shadow[id].FrameDeadTime = 0xff;//joinreq->ttl;     
                    cltor_shadow[id].rRssi = (uint8)(0 - rssi);

                    UpdataRouteInfo1(id, pkt);//joinreq->route.rdepth);
                    
                    if(id > 1)
                    {
                        SyncCltorPara(id, &cltor[id], &stMeter);
                    }
                    break;
                }
                case MSG_TYPE_FN_07:
                {

                    int return_hash = 0, ReturnHash = 0;
                    uint8 bError = 0, devType = 0;
                    HASHT elem;

                    if(pkt->head.apdu.broadCastFlg == 1)
                    {
                        break;
                    }
                    
                    if( Check_Black(SN, 6))
                    {
                        break;
                    }

                    for(i = SUP_SS_INDEX_START; i < MAX_SUP_SS_NUM+1; i++)  //����
                    {
                        if(!LPC_Memory_CMP(cltor[i].devAddr, SN, 6))//�Ա�һ��
                        {
                            nFoundNum++;        
                            nFoundId = i;
                        }
                        
                    }   
                    if(0 == nFoundNum)
                    {
                        id = 0;
                        cltor_shadow[id].nod.result = 0x6;
                        //id = Re_Allocate_Id(SN);    //����SN���·���
                    }
                    else if(1 == nFoundNum)
                    {
                        id = nFoundId;
                        cltor_shadow[id].nod.result = 0x7;

                    }
                    else
                    {
                        for(i=SUP_SS_INDEX_START;i<MAX_SUP_SS_NUM+1;i++)  //����
                        {
                            if(!LPC_Memory_CMP(cltor[i].devAddr,SN, 6))//�Ա�һ��
                                Delete_SS_Node(i);                      
                        }                           
                        id = Re_Allocate_Id(SN, 6);    //����SN���·���
                        cltor_shadow[id].nod.result = 0x7;

                    }    

					if ((id <= MAX_SUP_SS_NUM) && ( id >= SUP_SS_INDEX_START) && (id <= rfpara.rf_slotnum))
                    {
                        devType = pkt->head.apdu.data[1] >> 4;
                        GetCltorPara(id, &stMeter);
                        memcpy(stMeter.GIS, pkt->head.apdu.data+2, 8);
                        uint8 heartBeat = pkt->head.apdu.data[10];
                        if(heartBeat > cltparm.routeuptime)
                        {
                            cltparm.routeuptime = heartBeat;
                            
                            I2cWrite(0xA0, (uint8*) &cltparm, FM_CLTP_ADDR, 1); //д��·�ɲ���         
                        }
                        memcpy((uint8 *)&stMeter.softver, pkt->head.apdu.data+11, 2);
                        
                        stMeter.nHwType = pkt->head.apdu.data[13];//joinreq->softver;//�ڵ�汾��                   
                                    
                        
                        memcpy(elem.addr, SN, 6); //����ַ
//	                        memcpy(elem.falsh_sn, SN, 6);
                        elem.SubType = pkt->head.apdu.stInfo.stUp.bit3.protocolFlg;
                        switch(devType)
                        {
                        case 0:
                            elem.devType = 1;
                            break;
                        case 1:
                            elem.devType = 0;
                            break;
                        default:
                            elem.devType = devType;
                            break;
                        }
                        elem.src = 0x55;//�Զ�ע��
                        return_hash=Hash_Table_Insert(&elem, &ReturnHash);
    //	                    if (return_hash)        //��ӹ�ϣ��                                        
    //	                    {  
                        if (return_hash==MAX_HASH_TABLE_FLASH_SIZE)
                        {   
                            HASH_Table_Used++;
                            HASH_Table_Used=(HASH_Table_Used&1);   //1��ʾ֮ǰ��Table1,֮��Table2;0��ʾ��Table2
                            I2cWrite(0xA0,&HASH_Table_Used, FM_HASH_USED, 1);//�����ʶλ
                            Hash_Transfer();//����,ת�ƹ�ϣ��
                            return_hash = Hash_Table_Insert(&elem, &ReturnHash);//�ٴβ����ϴ�δ��ӳɹ���
                            if((return_hash >= 0 ) && (return_hash == MAX_HASH_TABLE_FLASH_SIZE))
                            {
                                bError = 1;
                            }
                        }
    //	                    }
    //	                    else                                
    //	                        bError = 1;      

                        if(bError)//�Ƿ��豸���������
                        {
                            id = 0;
                            cltor_shadow[id].nod.result = 0x6;                        
                        }
                        else
                        {
                            cltor_shadow[id].nod.result = 0x07;//��ʾ����ɹ�
                            if(elem.devType != 2)//�м������ϱ�
                            {
                                MeterStatus[return_hash].bReceive = 1;
                            }
                        }
                    }
                    else
                    {
                        id = 0;
                        cltor_shadow[id].nod.result = 0x6;                        
                    }
                    cltor[id].addrLen = CON_DEV_ADDR_LEN_6;
                    memset(cltor[id].devAddr, 0, CON_DEV_ADDR_LEN_10);
                    
                    memcpy(cltor[id].devAddr, SN, 6);  // ��¼��ӽڵ��Ψһ��ʶSN 
                    
                    cltor_shadow[id].nod.ans_pkt_type=0x1;     //��������ʶλ
//	                    cltor[id].nodestatus.fixed= 0x0;           //��ʾ�õ�δ��
                    cltor_shadow[id].nod.needack= 0x1;         //��ʾ�õ���Ҫ�ظ�
//	                    cltor[id].nodestatus.handled = UN_HANDLED;   //���±��Ϊδ����
                    //cltor[id].nodestatus.NetStatus = NODE_STATUS_OUT;
                    //cltor[id].old_id=id;//joinreq->sourid;        //ԭ���ϱ��Ľڵ��
                    cltor_shadow[id].recvseq = pkt->head.apdu.seq;
                    cltor_shadow[id].FrameDeadTime = 0xff;//joinreq->ttl;     
                    cltor_shadow[id].rRssi = (uint8)(0 - rssi);
                    
                    UpdataRouteInfo1(id, pkt);//joinreq->route.rdepth);
                    
                    if(id > 1 && (!bError))
                    {
                        SyncCltorPara(id, &cltor[id], &stMeter);
                    }
                    break;
                }
                case MSG_TYPE_FN_05://����
                case MSG_TYPE_FN_04://�ǳ�
                {
//	                    u32SendTickToSSDeadTime = curslottime + 40; //�յ��������ظ���ʱ���ڴ�������
                    
                    if(bNeedReAllocate)
                    {
                        id = Sn_Search_Id(SN);
//							for(i=SUP_SS_INDEX_START;i<MAX_SUP_SS_NUM;i++)	//����
//							{
//								if(!LPC_Memory_CMP(cltor[i].devAddr,SN, 6))//�Ա�һ��
//									Delete_SS_Node(i);						
//							}						
//							cltor[id].nodestatus.handled=UN_HANDLED;			//���±��Ϊδ����
						
                        cltor_shadow[id].nod.result = 0;
                        if ((id > MAX_SUP_SS_NUM) ||( id < SUP_SS_INDEX_START) || (id > rfpara.rf_slotnum) /*|| (id >= MAX_REAL_SUP_SS_NUM)*/)
                        {               
#ifdef PROJECT_PINGSHUI
                            break;       
#else
                            id = 0;                 //��û�з��䵽��ЧIDʱ�����䵽0�����ڻظ�SS
                            cltor[id].addrLen = CON_DEV_ADDR_LEN_6;
                            memset(cltor[id].devAddr, 0, CON_DEV_ADDR_LEN_10);
                            
                            memcpy(cltor[0].devAddr, SN, 6);//����SN
                            
                            cltor_shadow[id].nod.result = 3;
#endif
                        }
						//ɾ��
						if(pkt->head.apdu.fn == MSG_TYPE_FN_04)
                        {
                            cltor[id].nodestatus.NetStatus = NODE_STATUS_OUT;
                        }                  
					} 
					else
                    {
                        id = 0;

                        {
                            cltor_shadow[id].nod.result = 2;
                            
                            if(pkt->head.apdu.broadCastFlg == 1)
                                break;
                        }
                    } 
                    cltor[id].addrLen = CON_DEV_ADDR_LEN_6;
                    memset(cltor[id].devAddr, 0, CON_DEV_ADDR_LEN_10);

                    memcpy(cltor[id].devAddr, SN, 6);   //����SN    

    				if((cltor[id].devAddr[0] < 0xFF))
    				{
    					cltor_shadow[id].nod.ans_pkt_type=0;		//��������ʶλ
    					
						if(pkt->head.apdu.fn == MSG_TYPE_FN_04)
                        {
                            cltor_shadow[id].nod.ans_pkt_type = 2;
                        }
                        
    					cltor_shadow[id].nod.needack= 1;		//������ʱ��Ҫ�ظ�
    					//cltor[id].old_id=id;
//	    					cltor[id].nodestatus.fixed= 1;				//��ʾ�õ��				
    					cltor_shadow[id].FrameDeadTime=  0xff;		
                        
                        cltor_shadow[id].recvseq = pkt->head.apdu.seq;
                    }
                    cltor_shadow[id].rRssi = (uint8)(0 - rssi);
                    //�����м̽ڵ�
                    UpdataRouteInfo1(id, pkt);//joinreq->route.rdepth);
                    
                    if(id > 1)
                    {
                        SyncCltorPara(id, &cltor[id], NULL);
                    }
//	                    if((0 != joinreq->neignum) && (8 != joinreq->neignum))
//	                    {
//	                        i = JOINREQ_SN_OFFSET + UNIQUE_MAC_ADDR_LEN + 1;                
//	                        cltor[id].nHwType = pkt->data[i];       //Ӳ����ʶ
//	                        cltor[id].nSoftFn = pkt->data[i + 1];       //Fn
//	                        cltor[id].nSubSoftVersion = pkt->data[i + 2];   //������������汾                      
//	                    }
                    break;
                }                
                default:
                    break;
            }
            break;
		}            
        case MSG_TYPE_FILE:
        {
            switch(pkt->head.apdu.fn)
            {
                case MSG_TYPE_FN_04:
                {
                    if(bNeedReAllocate)
                    {
                        uint8 segmentSeq = pkt->head.apdu.data[2];
                        uint8 bitMapLen = pkt->head.apdu.data[3];
                        
                        id = Sn_Search_Id(SN);
                        if(bitMapLen > DOWN_UPGRADE_BITMAP_SIZE)
                        {
                            bitMapLen = DOWN_UPGRADE_BITMAP_SIZE;
                        }
                        if(segmentSeq != g_stUpgrade.p2pBitMapIndex ||
                            id != g_stUpgrade.p2pSeq ||
                            pkt->head.apdu.seq != g_stUpgrade.frameSeq)//�κźͱ��ַ��һ����copy
                        {
                            g_stUpgrade.delay = 0;
                            break;
                        }
                        memcpy(g_stUpgrade.bitMap, pkt->head.apdu.data+4, bitMapLen);
                        g_stUpgrade.bitMapFlg = 0x55;
                        
                        g_stUpgrade.crc = CRC_16BIT_Check1((uint8 *)&g_stUpgrade, sizeof(STUPGRADE) - 2);
                        I2cWrite(0xA0,  (uint8 *)&g_stUpgrade, DP_DOWNLOAD_PROC_PARA_ADDR, sizeof(STUPGRADE));  
                    }
                    
                    break;
                }
                break;
            
            default:
                break;
            }
        }
        case MSG_TYPE_BROADCAST:
        {
            switch(pkt->head.apdu.fn)
            {
                case MSG_TYPE_FN_04:
                {
                    if(bNeedReAllocate)
                    {
                    }
                    else
                    {
                        //extern const uint8 sBroadAddrFC[6];
                        extern uint8 bNeedRelayBroadCast;
                        if(memcmp(SN, sBroadAddrFC, 6) == 0)
                        {
                            if(EzBroadCastPkt.bValid)
                            {
                                break;
                            }
                            memset((uint8 *)&EzBroadCastPkt, 0, sizeof(EZ_TX_PKT));
                            EzBroadCastPkt.bValid = 1;
                            EzBroadCastPkt.nBackOffSlot = 0;
                            memcpy(EzBroadCastPkt.TxPkt.data, pkt->data, frameLen);
                            EzBroadCastPkt.TxPkt.len = frameLen;
                            bNeedRelayBroadCast = TRUE;
                            
                            nChangeChannelTime = curslottime + MAX_SLOT_TIME * 2;
                            stinfo.stUp.bit1.moduleFlg = 1;
                            stinfo.stUp.bit3.meterFlg = 0x05;
                            stinfo.stUp.bit4.recvSignal = getSignalQuality((int)rssi, RSSI_OFFSET_MF, RSSI_OFFSET_HF);
                            stinfo.stUp.seq = cltor_shadow[1].sendseq++;
                            fn = 6; 

                            Compose_3762Data(0xC1, pkt->data+m + 1,pkt->head.apdu.len - 1, &pkt->data[PKT_HEAD_LEN],
                                    pkt->head.apdu.addrlen-oneAddrLen,&stinfo, 6, ((fn / 8) * 0x100 + (1 << ((fn - 1) % 8))));
                        }

                    }
                    //EzBroadCastPkt;
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case MSG_TYPE_CTRL:
        {
            switch(pkt->head.apdu.fn)
            {
                case MSG_TYPE_FN_01:
                    if(bDebugFlg == TRUE)
                    {
                    tmp[ml++] = 0xFF;
                    tmp[ml++] = 0xFF;
                    
                    tmp[ml++] = 0x00;
                    tmp[ml++] = 0x00;
                    
                    ctrl_bit.dir = 1;
                    ctrl_bit.prm = 0;
                    ctrl_bit.evtmode = 0;
                    ctrl_bit.ftd = 0;
                    p = tmp;
                    fn = 1;
                    berr = TRUE;
                    //extern uint8 bResetCmd;
                    g_bResetCmd = 1;
                    nChangeChannelTime = curslottime + 360;
                    }
                    break;
                case MSG_TYPE_FN_02:
                    if(bDebugFlg == TRUE)
                {
                    tmp[ml++] = 0xFF;
                    tmp[ml++] = 0xFF;
                    
                    tmp[ml++] = 0x00;
                    tmp[ml++] = 0x00;
                    
                    ctrl_bit.dir = 1;
                    ctrl_bit.prm = 0;
                    ctrl_bit.evtmode = 0;
                    ctrl_bit.ftd = 0;
                    p = tmp;
                    fn = 1;
                    berr = TRUE;

                    uint8 dis[32] ;
					memset(dis,0xff,32);
					for(i=0;i<32;i++)
					{	                                   
						I2cWrite(0xA0, dis, 0+(i*32), 32); //��λӰ�ӿռ�***
						memset(shadow_space+(i*32),0xff,32);						 
					}
                    Init_Hash_Table();                    
//						IapAction = IAP_ACTION_ERASE_HASH1; //IAP��ʶλ
//						while (IapAction != IAP_ACTION_IDLE);
//	                    
//						IapAction = IAP_ACTION_ERASE_HASH2; //IAP��ʶλ
//						while (IapAction != IAP_ACTION_IDLE);	// Hold on	
//						
//						IapAction = IAP_ACTION_ERASE_WRITE_SS_HASH1; //IAP��ʶλ
//						while (IapAction != IAP_ACTION_IDLE);
//	                    
//						IapAction = IAP_ACTION_ERASE_WRITE_SS_HASH2; //IAP��ʶλ
//						while (IapAction != IAP_ACTION_IDLE);
//	                    
//						IapAction = IAP_ACTION_ERASE_BLACK_SS_HASH1; //IAP��ʶλ
//						while (IapAction != IAP_ACTION_IDLE);
//	                    
//						IapAction = IAP_ACTION_ERASE_BLACK_SS_HASH2; //IAP��ʶλ
//						while (IapAction != IAP_ACTION_IDLE);	

                    nChangeChannelTime = curslottime + MAX_SLOT_TIME * 10;
                    bNeedChangeChannel = TRUE;

                    for(i = SUP_SS_INDEX_START; i < MAX_SUP_SS_NUM+1; i++)  //����
                    {
                        if(cltor[i].devAddr[0] < 0xFF)//
                        {
                            Delete_SS_Node(i);          
                        }
                    }
                }
                    break;

                case MSG_TYPE_FN_03:
                    if(bDebugFlg == TRUE)
                    {
                    
                    tmp[ml++] = 0xFF;
                    tmp[ml++] = 0xFF;
                    
                    tmp[ml++] = 0x00;
                    tmp[ml++] = 0x00;
                    
                    ctrl_bit.dir = 1;
                    ctrl_bit.prm = 0;
                    ctrl_bit.evtmode = 0;
                    ctrl_bit.ftd = 0;
                    p = tmp;
                    fn = 1;
                    berr = TRUE;
                    }
                    break;

            case MSG_TYPE_FN_11://����RSSI����ֵ
                    if(bDebugFlg == TRUE)
                    {
                        memset(tmp, 0, 27);
                        ml = 0;
                        
                        if(rfpara.rf_limit != pkt->head.apdu.data[0])
                        {
                            rfpara.rf_limit = pkt->head.apdu.data[0];

                            I2cWrite(0xA0, (uint8*)&rfpara.rf_limit, FM_NETPARA_ADDR+3, 1);
                        }
                        tmp[ml++] = 0xFF;
                        tmp[ml++] = 0xFF;
                        
                        tmp[ml++] = 0x00;
                        tmp[ml++] = 0x00;

                        berr = TRUE;

                        
                        ctrl_bit.dir = 1;
                        ctrl_bit.prm = 0;
                        ctrl_bit.evtmode = 0;
                        ctrl_bit.ftd = 0;
                        fn = 1;
                        p = tmp;
                    }  
                    break;
                case MSG_TYPE_FN_43://��ȡRSSI����ֵ
                    //if(bDebugFlg == TRUE)
                    {
                        memset(tmp, 0, 27);
                        ml = 0;
                        
                        tmp[ml++] = rfpara.rf_limit;

                        p = tmp;
                        ctrl_bit.dir = 1;
                        ctrl_bit.prm = 0;
                        ctrl_bit.evtmode = 0;
                        ctrl_bit.ftd = pkt->head.apdu.ctrl.ftd;
                        
                        fn = pkt->head.apdu.fn;
                        berr = TRUE;
                    }
                    break;
                    
                case MSG_TYPE_FN_13:
                    if(bDebugFlg == TRUE)
                    {
                        memset(tmp, 0, 27);
                        ml = 0;

                        memcpy(tmp, pkt->head.apdu.data, 6);

                        if(LPC_Memory_CMP(tmp, nDeviceMacAddr, 6))
                        {
                            uint8 addrLen = 6;
                            I2cWrite(0xA0, &addrLen, FM_DEVICE_ADDR_LEN, 1);
                        
                            I2cWrite(0xA0, &pkt->data[13], FM_DEVICE_ADDR , 6);
                            I2cRead(0xA0, nDeviceMacAddr, FM_DEVICE_ADDR, 6); 

                            nChangeChannelTime = curslottime + MAX_SLOT_TIME * 10;
                            bNeedChangeChannel = TRUE;
                        }

                        tmp[ml++] = 0xFF;
                        tmp[ml++] = 0xFF;
                        
                        tmp[ml++] = 0x00;
                        tmp[ml++] = 0x00;
                        
                        ctrl_bit.dir = 1;
                        ctrl_bit.prm = 0;
                        ctrl_bit.evtmode = 0;
                        ctrl_bit.ftd = 0;
                        p = tmp;
                        fn = 1;
                        berr = TRUE;
                    }                      
                    break;        
                case MSG_TYPE_FN_44:
                    //if(bDebugFlg == TRUE)
                    {
                        memset(tmp, 0, 27);
                        ml = 0;
                        
                        memcpy(tmp+ml, nDeviceMacAddr, 6);
                        ml+=6;
                        p = tmp;
                        
                        ctrl_bit.dir = 1;
                        ctrl_bit.prm = 0;
                        ctrl_bit.evtmode = 0;
                        ctrl_bit.ftd = pkt->head.apdu.ctrl.ftd;
                        
                        fn = pkt->head.apdu.fn;
                        berr = TRUE;
                    }
                    break;
                case MSG_TYPE_FN_17:
                    if(bDebugFlg == TRUE)
                    {
                        memset(tmp, 0, 27);
                        ml = 0;
                        uint8 moedemType = pkt->head.apdu.data[0] >> 5;
                        uint8 channel = pkt->head.apdu.data[0] & 0x1F;
                        memcpy(tmp, pkt->head.apdu.data, 1);
                        
                        if(moedemType != rfpara.modemType)
                        {
                            I2cWrite(0xA0, (uint8*)&moedemType, FM_NETPARA_ADDR + 10, 1);                        
                        }
                        if(channel != rfpara.rf_channel)
                        {
                            nChangeChannelTime = curslottime + MAX_SLOT_TIME * 10;
                            bNeedChangeChannel = TRUE;

                            for(i = SUP_SS_INDEX_START; i < MAX_SUP_SS_NUM+1; i++)  //����
                            {
                                if(cltor[i].devAddr[0] < 0xFF)//
                                {
                                    Delete_SS_Node(i);          
                                }
                            }
                            I2cWrite(0xA0, (uint8*)&channel, FM_NETPARA_ADDR, 1);                        
                        }
                        //rfpara.rf_channel = channelId;
                        tmp[ml++] = 0xFF;
                        tmp[ml++] = 0xFF;
                        
                        tmp[ml++] = 0x00;
                        tmp[ml++] = 0x00;
                        p = tmp;
                        ctrl_bit.dir = 1;
                        ctrl_bit.prm = 0;
                        ctrl_bit.evtmode = 0;
                        ctrl_bit.ftd = 0;

                        fn = 1;
                        berr = TRUE;
                        
                    }                      
                    break;                 
                case MSG_TYPE_FN_18:
                    if(bDebugFlg == TRUE)
                    {
                        memset(tmp, 0, 27);
                        ml = 0;
                        memcpy(tmp, pkt->head.apdu.data, 11);
                        
                        I2cWrite(0xA0, (uint8*)tmp, FM_GPS_PARA, 11);   
                        
                        tmp[ml++] = 0xFF;
                        tmp[ml++] = 0xFF;
                        
                        tmp[ml++] = 0x00;
                        tmp[ml++] = 0x00;
                        p = tmp;
                        ctrl_bit.dir = 1;
                        ctrl_bit.prm = 0;
                        ctrl_bit.evtmode = 0;
                        ctrl_bit.ftd = 0;

                        fn = 1;
                        berr = TRUE;
                        
                    }                      
                    break;                                     
                case MSG_TYPE_FN_20:
                    //if(bDebugFlg == TRUE)
                    {
                        memset(tmp, 0, 27);
                        ml = 0;
                        
                        tmp[ml++] = 0;//pandId
                        tmp[ml++] = 0;
                        memcpy(tmp+ml, nDeviceMacAddr, 6);//����ģ���ַ
                        ml+=6;
                        tmp[ml++] = 0;//�ű��ʶ
                        uint16 nMeterNum = (MAX_SUP_SS_NUM-1);
                        uint16 nMeterNum2;
                        memcpy(tmp + ml, &nMeterNum, 2);
                        ml+=2;
                        tmp[ml++] = rfpara.rf_limit;//��ǿ����
                        tmp[ml++] = rfpara.rf_channel;//�����ŵ����
                        memcpy(tmp + ml, (uint8 *)&curslottime, 2);//ʱ϶��
                        ml+=2;
                        tmp[ml++] = 0;//���
                        tmp[ml++] = rfpara.rf_power;//���书�ʵȼ�
                        tmp[ml++] = 1;//����״̬
//	                        extern volatile uint32 MSR;                        
                        tmp[ml++] = getMSR();//����״̬
                        
                        nMeterNum = Meter_Num();//��ģ��������
                        memcpy(tmp + ml, &nMeterNum, 2);
                        ml+=2;
                        
                        nMeterNum2 = getSnLoginedNum();//������ģ������
                        memcpy(tmp + ml, &nMeterNum2, 2);
                        ml+=2;
                        nMeterNum -= nMeterNum2;//δ����ģ������
                        memcpy(tmp + ml, &nMeterNum, 2);
                        ml+=2;
                        tmp[ml++] = 0;
                        p = tmp;
                        ctrl_bit.dir = 1;
                        ctrl_bit.prm = 0;
                        ctrl_bit.evtmode = 0;
                        ctrl_bit.ftd = pkt->head.apdu.ctrl.ftd;
                        
                        fn = pkt->head.apdu.fn;
                        berr = TRUE;
                    }
                    break;
                case MSG_TYPE_FN_21:
                    if(bDebugFlg == TRUE)
                    {
                        memset(tmp, 0, 27);
                        ml = 0;
                        
                        rfpara.rf_power = pkt->head.apdu.data[3];

                        I2cWrite(0xA0, (uint8*)&rfpara.rf_power, FM_NETPARA_ADDR+1, 1);   
    
                        tmp[ml++] = 0xFF;
                        tmp[ml++] = 0xFF;
                        
                        tmp[ml++] = 0x00;
                        tmp[ml++] = 0x00;
                        p = tmp;
                        ctrl_bit.dir = 1;
                        ctrl_bit.prm = 0;
                        ctrl_bit.evtmode = 0;
                        ctrl_bit.ftd = 0;


                        stAddr.data = (uint8 *)sBroadAddrFE;
                        stAddr.len = 6;
                        stData.data = tmp;
                        stData.len = ml;
                        
                        SendPacketToMac(&ctrl_bit, 1, 0, &stAddr, &stData, pkt->head.apdu.seq, (uint8)(0 - rssi), getIdelSendCacheByPri(CON_SEND_PRIORITY_LOW));
                        Delay_ms(2000);
                        Reset();
                    }                    
                    break;
                case MSG_TYPE_FN_22:
                    //if(bDebugFlg == TRUE)
                    {
                        memset(tmp, 0, 27);
                        ml = 0;
//	                        extern const STSX1276PARA ChannelSetting[];
                        tmp[ml++] = getSF();//LoRaSettings.SpreadingFactor;//pandId
                        tmp[ml++] = getBW();//LoRaSettings.SignalBw;
                        tmp[ml++] = getPreamble();//ChannelSetting[rfpara.rf_channel].preamble;
                        tmp[ml++] = rfpara.rf_power;
                        p = tmp;
                        ctrl_bit.dir = 1;
                        ctrl_bit.prm = 0;
                        ctrl_bit.evtmode = 0;
                        ctrl_bit.ftd = pkt->head.apdu.ctrl.ftd;
                        
                        fn = pkt->head.apdu.fn;
                        berr = TRUE;
                    }
                    break;
                case MSG_TYPE_FN_23:
                    if(bDebugFlg == TRUE)
                    {
                        memset(tmp, 0, 27);
                        ml = 0;
                        uint8 channel = pkt->head.apdu.data[0];

                        if(channel != rfpara.rf_channel)
                        {
                            nChangeChannelTime = curslottime + MAX_SLOT_TIME * 10;
                            bNeedChangeChannel = TRUE;

                            for(i = SUP_SS_INDEX_START; i < MAX_SUP_SS_NUM+1; i++)  //����
                            {
                                if(cltor[i].devAddr[0] < 0xFF)//
                                {
                                    Delete_SS_Node(i);          
                                }
                            }
                            I2cWrite(0xA0, (uint8*)&channel, FM_NETPARA_ADDR, 1);                        
                        }
                        //rfpara.rf_channel = channelId;
                        tmp[ml++] = 0xFF;
                        tmp[ml++] = 0xFF;
                        
                        tmp[ml++] = 0x00;
                        tmp[ml++] = 0x00;
                        p = tmp;
                        ctrl_bit.dir = 1;
                        ctrl_bit.prm = 0;
                        ctrl_bit.evtmode = 0;
                        ctrl_bit.ftd = 0;

                        fn = 1;
                        berr = TRUE;
                        
                    }                    
                    break;
                case MSG_TYPE_FN_24:
                    //if(bDebugFlg == TRUE)
                    {
                        memset(tmp, 0, 27);
                        ml = 0;
                        
                        tmp[ml++] = rfpara.rf_channel;
                        p = tmp;
                        
                        ctrl_bit.dir = 1;
                        ctrl_bit.prm = 0;
                        ctrl_bit.evtmode = 0;
                        ctrl_bit.ftd = pkt->head.apdu.ctrl.ftd;
                        
                        fn = pkt->head.apdu.fn;
                        berr = TRUE;
                    }
                    break;
                    
                case MSG_TYPE_FN_25:
                    //if(bDebugFlg == TRUE)
                    {
                        memcpy(tmp, (uint8 *)ucModeVer, 20);
                        
                        tmp[8] = (sVersiong.swCode[0] / 0x10) + '0';
                        tmp[9] = (sVersiong.swCode[0] % 0x10) + '0';
                        
                        tmp[11] = (sVersiong.yy / 0x10) + '0';
                        tmp[12] = (sVersiong.yy % 0x10) + '0';
                        tmp[13] = (sVersiong.mm / 0x10) + '0';
                        tmp[14] = (sVersiong.mm % 0x10) + '0';
                        tmp[15] = (sVersiong.dd / 0x10) + '0';
                        tmp[16] = (sVersiong.dd % 0x10) + '0';
                        p = tmp;

                        ml = 20;
                        ctrl_bit.dir = 1;
                        ctrl_bit.prm = 0;
                        ctrl_bit.evtmode = 0;
                        ctrl_bit.ftd = pkt->head.apdu.ctrl.ftd;

                        fn = pkt->head.apdu.fn;
                        berr = TRUE;
                    }
                    break;
                case MSG_TYPE_FN_26:
                    if(bDebugFlg == TRUE)
                    {
                        memset(tmp, 0, 27);
                        ml = 0;
                        //��¼��ǰ��־��ʱ��
                        bBroadMeterEnable = 1;
                        I2cWrite(0xA0, &bBroadMeterEnable, FM_BROAD_METER_ENABLE , 1); 
                        
                        //��ʼ�㲥���                                  
                        bMeterSearchStart = 1;
                        for(uint16 loop = 0; loop < MAX_HASH_TABLE_FLASH_SIZE; loop++)
                        {
                            MeterStatus[loop].bNeedDelete = 0;
                            MeterStatus[loop].bDeleteWaitAck= 0;
                            
                            MeterStatus[loop].bAck = 0;
                            MeterStatus[loop].bReceive = 0;
                            MeterStatus[loop].bWaitAck = 0;                                 
                            MeterStatus[loop].bSingleSSSearchMeterReceive = 0;
                        }
                        bNeedMeterDeleteAck = 0;        
                        bNeedDeleteMeter = 0;                               
                        
                        bMeterDeadStart = 0;
                        bCltorSearchStart = 0;
                        //rfpara.rf_channel = channelId;
                        tmp[ml++] = 0xFF;
                        tmp[ml++] = 0xFF;
                        
                        tmp[ml++] = 0x00;
                        tmp[ml++] = 0x00;
                        p = tmp;
                        ctrl_bit.dir = 1;
                        ctrl_bit.prm = 0;
                        ctrl_bit.evtmode = 0;
                        ctrl_bit.ftd = 0;

                        fn = 1;
                        berr = TRUE;
                        
                    }                    
                    break;  
                case MSG_TYPE_FN_27:
                {
                    if(bDebugFlg == TRUE)
                    {
                        uint16 nStartNum, find_id;
                        uint8 type, nNeedNum;
                        uint8 nMeterNum, nCurSendNum;
                        uint8 * pSendData;
                        uint8 * pSn;
                        memset(tmp, 0, 27);
                        ml = 0;

                        type = pkt->head.apdu.data[0];
                        memcpy((uint8 *)&nStartNum, pkt->head.apdu.data + 1, 2);
                        nNeedNum = pkt->head.apdu.data[3];

                        nCurSendNum = 0;
                        p=(uint8 *)updata_code_cache;
                        HASHT* pHasht = NULL;
        
                        if(HASH_Table_Used==0)//��ǰʹ��table1
                            pHasht = htable1;
                        else
                            pHasht = htable2;
                        
                        nMeterNum = 0;
                        ml = 4;
                        if(type == 1)
                        {
                            for(i=SUP_SS_INDEX_START;i < MAX_SUP_SS_NUM+1; i++)//����
                            {
                                if ( cltor[i].devAddr[0] < 0xFF && cltor[i].nodestatus.NetStatus)
                                {
                                    nMeterNum ++;
                                    if((nMeterNum >= nStartNum) && (nCurSendNum < nNeedNum) && (nCurSendNum < 29))
                                    {
                                        //���
                                        pSendData =  p + ml;// + nCurSendNum * 12;
                                        pSn = cltor[i].devAddr;
                                        memcpy(pSendData, pSn, 6);

                                        nCurSendNum++;
                                        ml+=6;
                                    }
                                }
                            }
                        }
                        else
                        {
                            for(i=0; i < MAX_HASH_TABLE_FLASH_SIZE; i++)
                            {
                                if(pHasht[i].status == OCCUPIED)
                                {
                                    nMeterNum ++;
                                    if((nMeterNum >= nStartNum) && (nCurSendNum < nNeedNum) && (nCurSendNum < 22))
                                    {
                                        if(type == 4 && pHasht[i].devType != 2)
                                        {
                                            continue;
                                        }
                                        //���
                                        pSendData =  p + ml;
                                        pSn = pHasht[i].addr;
                                        
                                        if(type == 3)//δ��������
                                        {
                                            find_id = Sn_Search_Id(pSn);    //����SN���Ҷ�ӦID
                                        
                                            if(find_id < 2 || find_id > MAX_SUP_SS_NUM || cltor[find_id].nodestatus.NetStatus)
                                            {
                                                continue;
                                            }
                                        }
                                            
                                        memcpy(pSendData, pSn, 6);
                                        
                                        nCurSendNum++;  
                                        ml+=6;
                                    }
                                }
                            }
                        }
                        memcpy(p, pkt->head.apdu.data, 3);
                        p[3] = nCurSendNum;
                        
                        ctrl_bit.dir = 1;
                        ctrl_bit.prm = 0;
                        ctrl_bit.evtmode = 0;
                        ctrl_bit.ftd = pkt->head.apdu.ctrl.ftd;
                        
                        fn = pkt->head.apdu.fn;
                        berr = TRUE;
                    }

                    break;
                }
                case MSG_TYPE_FN_30:
                    //if(bDebugFlg == TRUE)
                    {
                        memset(tmp, 0, 27);
                        ml = 0;
                        //memcpy(tmp, pkt->head.apdu.data, 11);
                        
                        I2cRead(0xA0, (uint8*)tmp, FM_GPS_PARA, 11);   
                        
                        ml+=11;
                        p = tmp;
                        ctrl_bit.dir = 1;
                        ctrl_bit.prm = 0;
                        ctrl_bit.evtmode = 0;
                        ctrl_bit.ftd = pkt->head.apdu.ctrl.ftd;

                        fn = pkt->head.apdu.fn;
                        berr = TRUE;
                        
                    }                      
                    break;      
                case MSG_TYPE_FN_31:
                    if(bDebugFlg == TRUE)
                    {
                        memset(tmp, 0, 27);
                        ml = 0;
                        //memcpy(tmp, pkt->head.apdu.data, 11);
                        
                        I2cRead(0xA0, (uint8*)&rfpara.rf_channel, FM_NETPARA_ADDR, 1);   
                        I2cRead(0xA0, (uint8*)&rfpara.modemType, FM_NETPARA_ADDR+11, 1);  
                        tmp[0] = (rfpara.modemType << 5) | (rfpara.rf_channel & 0x1F);
                        
                        ml+=1;
                        p = tmp;
                        ctrl_bit.dir = 1;
                        ctrl_bit.prm = 0;
                        ctrl_bit.evtmode = 0;
                        ctrl_bit.ftd = pkt->head.apdu.ctrl.ftd;

                        fn = pkt->head.apdu.fn;
                        berr = TRUE;
                        
                    }                      
                    break;
                case MSG_TYPE_FN_39://����״̬
                    if(bDebugFlg == TRUE)
                    {
                        memset(tmp, 0, 27);
                        ml = 0;
                        //memcpy(tmp, pkt->head.apdu.data, 11);
                        tmp[0] = 0;
                        
                        ml+=1;
                        p = tmp;
                        ctrl_bit.dir = 1;
                        ctrl_bit.prm = 0;
                        ctrl_bit.evtmode = 0;
                        ctrl_bit.ftd = pkt->head.apdu.ctrl.ftd;

                        fn = pkt->head.apdu.fn;
                        berr = TRUE;
                        
                    }                      
                    break;                    
                case MSG_TYPE_FN_40:
                {
                    if(bDebugFlg == TRUE)
                    {
                        uint32 addr;
                        
                        memset(tmp, 0, 27);
                        ml = 0;
                        memcpy((uint8 *)&addr, pkt->head.apdu.data, 4);
                        len = pkt->head.apdu.data[4];
                        p=(uint8 *)updata_code_cache;
#define RAM_START 0x10000000
#define RAM_END   0x10007fff
#define RAM2_START 0x2007C000
#define RAM2_END   0x20083fff
                        if(len > 192)
                        {
                            len = 192;
                        }
                        if((addr >= RAM_START && addr <= RAM_END) || (addr >= RAM2_START && addr <= RAM2_END) )
                        {
                            p[ml++] = len;
                            memcpy(p + ml, ((unsigned char*)addr), len);
                            //p[0] = len;
                            
                            //ml+=1;
                            ml+=len;
                            //p = tmp;
                            ctrl_bit.dir = 1;
                            ctrl_bit.prm = 0;
                            ctrl_bit.evtmode = 0;
                            ctrl_bit.ftd = pkt->head.apdu.ctrl.ftd;

                            fn = pkt->head.apdu.fn;
                        }
                        else
                        {
                            memset(tmp, 0, 27);
                            ml = 0;
                            
                            //rfpara.rf_channel = channelId;
                            tmp[ml++] = 0x05;
                            p = tmp;
                            ctrl_bit.dir = 1;
                            ctrl_bit.prm = 0;
                            ctrl_bit.evtmode = 0;
                            ctrl_bit.ftd = 0;
                            
                            fn = 2;
                        }
                        
                        berr = TRUE;
                    }                      
                    
                    break;
                }
                case MSG_TYPE_FN_41:
                {
                    if(bDebugFlg == TRUE)
                    {
                        uint32 addr;
                        
                        memset(tmp, 0, 27);
                        ml = 0;
                        memcpy((uint8 *)&addr, pkt->head.apdu.data, 4);
                        len = pkt->head.apdu.data[4];
                        p=(uint8 *)updata_code_cache;
#define FLASH_START 0x00000000
#define FLASH_END   FLASH1_SIZE
                        if(len > 192)
                        {
                            len = 192;
                        }
                        if((addr <= FLASH_END) )
                        {
                            p[ml++] = len;
                            BFLS_Read(p + ml, (addr), len);
                            p[0] = len;
                            
                            //ml+=1;
                            ml+=len;
                            //p = tmp;
                            ctrl_bit.dir = 1;
                            ctrl_bit.prm = 0;
                            ctrl_bit.evtmode = 0;
                            ctrl_bit.ftd = pkt->head.apdu.ctrl.ftd;

                            fn = pkt->head.apdu.fn;
                        }
                        else
                        {
                            memset(tmp, 0, 27);
                            ml = 0;
                            
                            //rfpara.rf_channel = channelId;
                            tmp[ml++] = 0x05;
                            p = tmp;
                            ctrl_bit.dir = 1;
                            ctrl_bit.prm = 0;
                            ctrl_bit.evtmode = 0;
                            ctrl_bit.ftd = 0;
                            
                            fn = 2;
                        }
                        
                        berr = TRUE;
                    }                      
                    
                    break;
                }            
                case MSG_TYPE_FN_42:
                {
                    if(bDebugFlg == TRUE)
                    {
                        uint32 addr;
                        
                        memset(tmp, 0, 27);
                        ml = 0;
                        memcpy((uint8 *)&addr, pkt->head.apdu.data, 4);
                        len = pkt->head.apdu.data[4];
                        p=(uint8 *)updata_code_cache;
#define EEPROM_START 0x00000000
#define EEPROM_END   0x00010000
                        if(len > 192)
                        {
                            len = 192;
                        }
                        if((addr <= EEPROM_END) )
                        {
                            p[ml++] = len;
                        
                            I2cRead(0xA0,  p+ml, addr, len); 
                            
                            //ml+=1;
                            ml+=len;
                            //p = tmp;
                            ctrl_bit.dir = 1;
                            ctrl_bit.prm = 0;
                            ctrl_bit.evtmode = 0;
                            ctrl_bit.ftd = pkt->head.apdu.ctrl.ftd;

                            fn = pkt->head.apdu.fn;
                        }
                        else
                        {
                            memset(tmp, 0, 27);
                            ml = 0;
                            
                            //rfpara.rf_channel = channelId;
                            tmp[ml++] = 0x05;
                            p = tmp;
                            ctrl_bit.dir = 1;
                            ctrl_bit.prm = 0;
                            ctrl_bit.evtmode = 0;
                            ctrl_bit.ftd = 0;
                            
                            fn = 2;
                        }
                        
                        berr = TRUE;
                    }                      
                    
                    break;
            case MSG_TYPE_FN_48://����RSSI����ֵ
                    if(bDebugFlg == TRUE)
                    {
                        memset(tmp, 0, 27);
                        ml = 0;
                        
                        if(rfpara.rf_power != pkt->head.apdu.data[0])
                        {
                            rfpara.rf_power = pkt->head.apdu.data[0];

                            I2cWrite(0xA0, (uint8*)&rfpara.rf_limit, FM_NETPARA_ADDR+1, 1);
                        }
                        tmp[ml++] = 0xFF;
                        tmp[ml++] = 0xFF;
                        
                        tmp[ml++] = 0x00;
                        tmp[ml++] = 0x00;

                        berr = TRUE;

                        
                        ctrl_bit.dir = 1;
                        ctrl_bit.prm = 0;
                        ctrl_bit.evtmode = 0;
                        ctrl_bit.ftd = 0;
                        fn = 1;
                        p = tmp;
                    }  
                    
                case MSG_TYPE_FN_49:

                    if(bDebugFlg == TRUE)
                    {
                        memset(tmp, 0, 27);
                        ml = 0;
                        
                        tmp[ml++] = rfpara.rf_power;

                        p = tmp;
                        ctrl_bit.dir = 1;
                        ctrl_bit.prm = 0;
                        ctrl_bit.evtmode = 0;
                        ctrl_bit.ftd = pkt->head.apdu.ctrl.ftd;
                        
                        fn = pkt->head.apdu.fn;
                        berr = TRUE;
                    }
                    else if(g_ucfactory_mod == 0x55)
                    {
                        cltor_shadow[0].rRssi = (uint8)(0 - rssi);
                        AtcmdReply_RF(AT_CMD_CT, READING, pkt->head.apdu.stInfo.stUp.rssi);

                    }

                    break;
                case MSG_TYPE_FN_52:
                    if(bDebugFlg == TRUE)
                    {
                        memset(tmp, 0, 27);
                        ml = 0;
                        uint8 orderFreq = 0;
                        uint32 oldOrder = 0, newOrder = 0;
                        I2cRead(0xA0, &tmp[0], FM_FREQ_DEFINE+8, 4); //������Ƶ�㶨�����***	
                        oldOrder = tmp[0]*0x1000000 + tmp[1]*0x10000 + tmp[2]*0x100 + tmp[3];
                        newOrder = pkt->apdu.data[3]*0x1000000 + pkt->apdu.data[2]*0x10000 + pkt->apdu.data[1]*0x100 + pkt->apdu.data[0];
                        orderFreq = pkt->apdu.data[1] & 0xF0;

//	                        newOrder = pkt->apdu.data[0]*0x1000000 + pkt->apdu.data[1]*0x10000 + pkt->apdu.data[2]*0x100 + pkt->apdu.data[3];
//	                        orderFreq = pkt->apdu.data[2] & 0xF0;
                        if(oldOrder != newOrder)
                        {

//	#if (SX127X_FREQ == FREQ_433M)
//	                            if(orderFreq == 0x40)
//	#elif  (SX127X_FREQ == FREQ_915M)
//	                            if(orderFreq == 0x90)
//	#elif  (SX127X_FREQ == FREQ_470M)
//	                            if(orderFreq == 0x70)
//	#elif  (SX127X_FREQ == FREQ_868M)
//	                            if(orderFreq == 0x80)                            
//	#endif
                            {
                                I2cWrite(0xA0, &pkt->apdu.data[0], FM_FREQ_DEFINE+8, 4); //������Ƶ�㶨�����***	
                                extern uint8 bResetCmd;
                                extern uint32 nChangeChannelTime;
                                bResetCmd = 1;
                                nChangeChannelTime = curslottime + 140;
                                berr = TRUE;
                            }

                        }
                        
                        if(berr == TRUE)
                        {
                            tmp[ml++] = 0xFF;
                            
                            tmp[ml++] = 0x00;
                            tmp[ml++] = 0x00;

                            ctrl_bit.dir = 1;
                            ctrl_bit.prm = 0;
                            ctrl_bit.evtmode = 0;
                            ctrl_bit.ftd = 0;
                            fn = 1;
                            p = tmp;
                        }
                        else
                        {
                            tmp[ml++] = 0x05;
                            p = tmp;
                            ctrl_bit.dir = 1;
                            ctrl_bit.prm = 0;
                            ctrl_bit.evtmode = 0;
                            ctrl_bit.ftd = 0;
                            
                            fn = 2;

                        }
                        berr = TRUE;

                    }

                    break;
                case MSG_TYPE_FN_53:
                    
                    if(bDebugFlg == TRUE)
                    {
                        memset(tmp, 0, 27);
                        ml = 0;
                        I2cRead(0xA0, tmp + ml, FM_FREQ_DEFINE+8, 4); //������Ƶ�㶨�����***	
                        //tmp[ml++] = rfpara.rf_power;
                        ml+=4;
                        p = tmp;
                        ctrl_bit.dir = 1;
                        ctrl_bit.prm = 0;
                        ctrl_bit.evtmode = 0;
                        ctrl_bit.ftd = pkt->apdu.ctrl.ftd;
                        
                        fn = pkt->apdu.fn;
                        berr = TRUE;
                    }
                    break;                    
                }                             
                default:
                    break;
            }

            if(berr == TRUE)
            {
                stAddr.data = (uint8 *)sBroadAddrFE;
                stAddr.len = 6;
                stData.data = p;
                stData.len = ml;
                
                SendPacketToMac(&ctrl_bit, fn, 0, &stAddr, &stData, pkt->head.apdu.seq, (uint8)(0 - rssi), getIdelSendCacheByPri(CON_SEND_PRIORITY_LOW));

            }
            
            break;
        }        
		default:
		{
            Uart_Status = UART_STATUS_IDLE;
            break;
		}
	}
	return rc;
}

#endif



