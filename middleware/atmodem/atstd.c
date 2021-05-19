/****************************************Copyright (c)**************************************************
**                               	____�Ƽ����޹�˾
**                                     		������
**
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: atstd.c
**��   ��   ��: yzy
**����޸�����: 2014��11��19��
**��        ��: ͨ��ATָ�������������
**ע        ��: GSM Rec07.07/GSM Rec07.05/ITU-T Rec.V25ter
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: yzy
** ��  ��: v1.0
** �ա���: 2014��11��19��
** �衡��: ԭʼ�汾
********************************************************************************************************/
#define EXT_ATSTD

#include "public.h"
#include "k_types.h"
#include "k_api.h"
//	#include "lib_app.h"
#include "hal.h"


#include "atmodem.h"

#include "modemlib.h"
#include "atstd.h"




/************************************************************************
 * ��������Ŵ�������
 <1>�յ�unsolicitedָ��.+CMTI: <mem>,<index>
 <2>List���ж���,��������ŵĴ洢������FIFO��
    AT+CMGL=4
    +CMGL: 1,1,,160 xxxxxx
    +CMGL: 2,1,,82 xxxxxx
    +CMGL: 3,0,,155 xxxxxx
 <3>��buffer����δ�������,
        <3.1>��FIFO�г���һ��δ����������
        <3.2>Read��������Ӧ�Ķ������ݵ�buffer
        <3.3>������Ϣ֪ͨӦ�ò㴦��.
        <3.4>Delete�Ѷ�ȡ�Ķ���.AT+CMGD=X
   ��buffer����δ�������,�ȴ�Ӧ�ò㴦����ټ�����������.

 *----------------------------------------------------------
 <4>��ʱ��ѯ���ն���FIFO���Ƿ��Դ���δ������,���������<3>.
 *----------------------------------------------------------
 *
 * ���ոû�����ƺ�,���մ��������ɾ��,ͨ������´洢�ռ��в����ѹ��������
 ************************************************************************/


/************************************************************************
 *��������Ӧ�ò㽻��״̬����ʱ��ͼ
 *DRIVER TF
 *-------------------------------------------------------------
     |��ȡSMS��buffer|
    (1)             (2)
                    \
                    \
                    \
                    V
                    (3)       (4)                           (5)
                                |Ӧ�ò㴦��buffer������|
 *-------------------------------------------------------------
 *APP TF
 ˵��:
     (2)��Ӧ������<3.2>������ɵ�ʱ��
     (1)-(2)Ϊ������ִ��SMS������ʱ��.
     (2)-(3)��ʾ������֪ͨӦ�ò�buffer��������δ����Ķ���.
     (4)-(5)ΪӦ�ò㴦��buffer���ݵ�ʱ��
     
     stt.bit.smsBufSem:
        bufferռ��ʱ��(1)-(5),ռ���ڼ䲻��ִ��SMS��ȡ����������buffer����.
     stt.bit.smsdex:
        buffer������Ч(2)-(5),��ʱ���buffer�д�����Ч����,��δ��Ӧ�ò㴦��.
************************************************************************/






/************************************************************************
**��Ҫɾ���Ķ��ŵ�����
************************************************************************/
uint16 gui_DelSmgIndex;


/************************************************************************
 * @function: DecodeRecvDataCMTI
 * @����: CMTI����Ҫ�����������
 * 
 * @����: 
 * @param: mrfb 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/11/13)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void DecodeRecvDataCMTI(ModemRecvFeedBack* mrfb)
{
    //+CMTI: <mem>,<index>
    //��+CMTI: "SM",1 ��ʾ�½�����Ϣ,�洢λ��SM_1
    //��+CMTI: "ME",7 ��ʾ�½�����Ϣ,�洢λ��ME_7
	
	DecodeRecvDataNone(mrfb);       //�������ǵ�Ӧ��,���ù����ϱ���Ϣ.
}



/************************************************************************
 * @function: DecodeRecvDataCMGL
 * @����: CMGL����Ҫ�����������,��ȡ�����б��ִ�еĺ���
 * 
 * @����: 
 * @param: mrfb ��������Ϣ�ṹ
 * @����: 
 * @˵��: 
 * @����: yzy (2014/11/13)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void DecodeRecvDataCMGL(ModemRecvFeedBack* mrfb)
{
    //+CMGL:<index>,<stat>,[<alpha>],<length><CR><LF><pdu><CR><LF>
    
	uint16 byte;
	uint16 smsidx;
										
	smsidx = GetOneChar(&byte);	        //��ȡһ���ֽڵ���Ϣ
	
	if((smsidx != 0xffff) && (smsidx != 0))//���¶��ŷ����б�
	{
        SYS_FIFO_Enqueue(&gs_FifoNewMessage, &smsidx);//��ȴ�����CMGL����Ӻ�,�ٽ��ж�ȡ
		sleep_ms(4);                              //�ȴ����ݽ�����ȫ,��ʵ��������,������ȫĩβ��0x0A��.
	}
	
	if((byte != 0x0D) && (byte != 0x0A))//�����û�е���β
	{
		DecodeRecvDataNone(mrfb);		//ȥ������ʣ�����Ч��Ϣ
	}
}


/************************************************************************
 * @function: DecodeRecvDataCMGR
 * @����: CMGR����Ҫ�����������,�յ����ţ�����ȴ����Ž��գ�
 * 
 * @����: 
 * @param: mrfb 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/11/13)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void DecodeRecvDataCMGR(ModemRecvFeedBack* mrfb)
{
    //AT+CMGR=<index>
    //+CMGR:<stat>,[<alpha>],<length><CR><LF><pdu>
    
	mrfb = mrfb;
	DecodeRecvDataNone(mrfb);			//ȥ������ʣ�����Ч��Ϣ
	sleep_ms(10);                    //�ȴ����Ž���
	DevModem->stt.bit.smWait = 0x01;	//��ֱ�ӷ�ͬ����Ϣ,�����ڽ�������ź��ٷ���Ϣ
}



/*************************************************************************
*����ATָ������                       //��STDAT_RECV��Ӧ
**************************************************************************/
const string STD_ATRecvHdr[NUM_AT_RECVITM] =
{
    "OK",
    
    "RING",
    "ERROR",
    "NO CARRIER",
    "BUSY",
    "+ABC",
    ">",
    "+CSQ:",
    "+CREG:",
    "+CME:",                            //��������
    "+CMTI:",                           //�¶�������
    "+CMGS:",
    "+CMGR:",
    "+CMGD:",
    "+CMGL:",
};




/*************************************************************************
*ATָ�����Ԥ��������                 //��STDAT_RECV��Ӧ
**************************************************************************/
const DecodeRecvData gs_STDATDecodeRecvData[] =
{
    DecodeRecvDataNone,					//AT_OK = 0,
    
    DecodeRecvDataNone,                 //AT_RING,
    DecodeRecvDataOneChar,              //AT_ERROR,
    DecodeRecvDataNone,                 //AT_NC,
    DecodeRecvDataNone,                 //AT_BS,
    DecodeRecvDataNone,                 //AT_CONNECT,����Զ������ָ��ظ�
    DecodeRecvDataNone,                 //AT_GT,
    DecodeRecvDataOneChar,              //AT_CSQ,
    DecodeRecvDataTwoChar,              //AT_CREG,
    DecodeRecvDataNone,                 //AT_CME,
    DecodeRecvDataCMTI,                 //AT_CMTI,
    DecodeRecvDataNone,                 //AT_CMGS,
    DecodeRecvDataCMGR,                 //AT_CMGR,
    DecodeRecvDataNone,                 //AT_CMGD,
    DecodeRecvDataCMGL,                 //AT_CMGL,
    
};




/************************************************************************
 * @function: ATCommandRecvMap
 * @����:  ɨ�贮�ڻ����е������Ƿ������������
 * 
 * @����: 
 * @param: atRecvHdr atָ��ʶ�����ַ����б�
 * @param: itmnum �б����
 * 
 * @����: 
 * @return: uint8   0xFF �������ݲ�����ָ������
 *                  0xfe ����������/ִ����ջ���
 *                  others ɨ�赽����ָ����������/ɾ�����ڻ����������ַ���
 *
 * @˵��: ATָ�����
 *      1)���ն���ʱ�����лظ�,�ڶ���û�������ַ�
 *      2)����SMS,��ĳЩģ�鷢��IP����ʱ.�ȷ��ͷ��͸�Ҫ,Ȼ����ȴ�ģ��ظ�'>'(��������)
 *      3)������������������,�յ���֡��������ͨ�������������ж�����:��ȡ��+�ж������ַ�
 *      4)ĳЩָ����+MIPRTCP,
 *              ��ʱ���յ�����,֡��ʱ�����յ��н�β��0x0D/0x0A,
 *              ��ʱ�����յ�0x0D/0x0A�������,�޽�β��.
 *              ���3)Ӧ����Ϊ:��ȡ��ͷ����+�ж������ַ�
 * @����: yzy (2014/11/17)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 ATCommandRecvMap(const string* atRecvHdr, uint8 itmnum)
{
    uint8 rdbuf[96];                        //��ȡ���ݵĻ���,��Ҫ��������������ַ���
    uint16 rdlen = 0;                        //�Ѷ�ȡ���ݵĳ���
    uint16 linelen = 0;
    uint16 ebytes;

	uint16 itmlen;                          //��ǰ�����ַ�������
    uint8 itmidx;                           //ATָ�������
                                            //0)��ȡ������������,����������ַ������ȵ�����
    while(1)
    {
        ebytes = 0;
        memset(rdbuf,0,96);
        rdlen = PeekByte(rdbuf, 0, sizeof(rdbuf));
        if(rdlen == 0)
        {
            return 0xfe;
        }
                                            //�޳���������ǰ�Ľ�β��0x0D/0x0A,��֤����е��׸��ֽ�
        linelen = LineLen(rdbuf, rdlen, &ebytes);
        if(linelen == 1)                    //only end character
        {
            DequeueByte(ebytes);
            continue;
        }
        else
        {
            break;
        }
    }
#if SYS_MNT_EN > 0                           //��ʼ��ʼ��ATͨ��
    LOG_DEBUG("%s\n", rdbuf);    
#endif
    //������׼ָ�
    for(itmidx = 0; itmidx < NUM_AT_RECVITM; itmidx++)
    {                                       //1)��ȡ��ATָ�������ַ�����ʶ�𳤶�
        itmlen = strlen(STD_ATRecvHdr[itmidx]);
//	        ASSERT(itmlen <= sizeof(rdbuf));
                                            //2)��ȡ�ĳ���>=�����ַ�������,ִ��ָ��ʶ�����
        for(uint8 idx = 0; idx <= 3; idx++)
        {
            if(rdlen - idx >= itmlen) 
            {
                if(0 == CmpBuffer(rdbuf+idx, (uint8*)STD_ATRecvHdr[itmidx], itmlen))
                {
                    if(idx == 0)
                    {
                        DequeueByte(itmlen);        //�Ӵ��ڻ�����,ɾ�������ַ���
                    }
                    return itmidx;              //���ط��ϵ�ָ������
                }
            }
        }
    }
    
    //����ģ���Զ���ָ�
    for(itmidx = 0; itmidx < itmnum - NUM_AT_RECVITM; itmidx++)
    {                                       //1)��ȡ��ATָ�������ַ�����ʶ�𳤶�
        itmlen = strlen(atRecvHdr[itmidx]);
//	        ASSERT(itmlen <= sizeof(rdbuf));
                                            //2)��ȡ�ĳ���>=�����ַ�������,ִ��ָ��ʶ�����
        for(uint8 idx = 0; idx <= 3; idx++)
        {
            if(rdlen - idx >= itmlen) 
            {
                if(0 == CmpBuffer(rdbuf + idx, (uint8*)atRecvHdr[itmidx], itmlen))
                {
                    if(idx == 0)
                    {
                        DequeueByte(itmlen);        //�Ӵ��ڻ�����,ɾ�������ַ���
                    }
                    return (NUM_AT_RECVITM + itmidx);//���ط��ϵ�ָ������
                }
            }
        }
    }

    return 0xff;
}







/**************************************************************************
**��ʼ������
***************************************************************************/
const ModemElement gs_ATInit[] =
{
    {"ATE0", AT_OK, 500, 0, (CallBackElement)0},
    {"AT+CNMI=2,1", AT_OK, 200, 0, (CallBackElement)0},        //������Ϣ��ʾģʽ,GSM_Rec_07.05
    {"AT+CMGF=0", AT_OK, 200, 0, (CallBackElement)0},          //����SMS��ʽ,GSM_Rec_07.05
    {(string)0, AT_OK, 0, 0, (CallBackElement)0},
};



/************************************************************************
 * @function: AT_Proc_CSQ
 * @����: ���ź��������н���
 * 
 * @����: 
 * @param: content 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2013/11/1)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 AT_Proc_CSQ(void* content)
{
    uint8 value = (uint8)((uint32)content);

    DevModem->stt.bit.signl = value;    //��¼�ź�����
    
    SYS_MODM_StartCnt(200);               //������ʱ������
    guc_ModemElement++;				    //Ԫ�ز���Ӽ�
    
    return 0;
}





/************************************************************************
 * @function: AT_Proc_Creg
 * @����: ��ע��������н���
 * 
 * @����: 
 * @param: content ��ǰ��ע�����
 * 
 * @����: 
 * @return: uint8  0 �����ɹ���������Ĳ���
 *		           1 ����ʧ�ܲ���������Ĳ���
 * @˵��: 
 * @����: yzy (2013/11/1)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 AT_Proc_Creg(void* content)
{
    Word32 wd;
    wd.lword = (uint32)content;			//��ȡ����
                                        //�жϵ�ǰ�Ƿ�ע��ɹ�(1:��ע�᱾��,5:��ע������)
    if((wd.byte[1] != 1) && (wd.byte[1] != 5))
    {
        DevModem->stt.bit.regtt = 0;
    }
    else					            //�������ʾע��ɹ�
    {
        DevModem->stt.bit.regtt = 1;
    }
    
    SYS_MODM_StartCnt(200);               //������ʱ������
    guc_ModemElement++;				    //Ԫ�ز���Ӽ�
    return 0;
}

/*************************************************************************
**��ѯ�ź�������ע�����
**************************************************************************/
const ModemElement gs_ATQAndReg[] =
{
    {"AT+CSQ", AT_OK, 500, 0, (CallBackElement)0},                       //�ź�����,GSM_Rec_07.07
    {"_csq ret", AT_CSQ, 200, 1, (CallBackElement)AT_Proc_CSQ},
    //{"AT+CPIN?", AT_OK, 0, 200, (CallBackElement)0},                     //sim�������
    //{"_cpin ret", AT_CREG, 0, 1, (CallBackElement)AT_Proc_Creg},
    {"AT+CREG?", AT_OK, 0, 500, (CallBackElement)0},                     //����ע�����,GSM_Rec_07.07
    {"_creg ret", AT_CREG, 0, 1, (CallBackElement)AT_Proc_Creg},
    
    {(string)0, AT_OK, 0, 200, (CallBackElement)0},
};



uint8 uc_smsNumPdu[16]; //pdu��ʽ���ź���(�Զ����뵽ż������,����'+')
uint8 uc_smsNumLen;     //pdu��ʽ���ź��볤��(�Զ�������ż��,����'+')
uint8 uc_smsNumType;    //pdu��ʽ���ź�������(0����,1����)

/************************************************************************
 * @function: AT_Proc_SendSmsCmd
 * @����: ���Ͷ�������
 * 
 * @����: 
 * @param: content 
 * 
 * @����: 
 * @return: uint8   0 �����ɹ���������Ĳ���
 *		            1 ����ʧ�ܲ���������Ĳ���
 * @˵��: 
 * @����: yzy (2013/11/1)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 AT_Proc_SendSmsCmd(void* content)
{
                                        //��Ҫ���͵Ķ�������
    TModemSMS* sc = DevModem->smsSend;  //(TModemSMS*)content;
    uint8 templen;
    uint8 i, tempch;
                                        //��ȡĿ����뼰�䳤��
    if(sc->number[0] == '+')            //���ʺ���
    {
        uc_smsNumLen = sc->numlength - 1;
        if(uc_smsNumLen > 16)           //���Ŀ����볤�ȳ���16λ
        {
            return 1;
        }
        MoveBuffer(sc->number + 1, uc_smsNumPdu, uc_smsNumLen);
        uc_smsNumType = 0x01;
    }
    else                                //���ں���
    {
        uc_smsNumLen = sc->numlength;
        if(uc_smsNumLen > 16)           //���Ŀ����볤�ȳ���16λ
        {
            return 1;
        }
        MoveBuffer(sc->number, uc_smsNumPdu, uc_smsNumLen);
        uc_smsNumType = 0x00;
    }
    
    if(uc_smsNumLen & 0x01)			    //����������Ҫ����ż��
    {
        uc_smsNumPdu[uc_smsNumLen] = 'F';//����'F'
        uc_smsNumLen++;
    }
    
    templen = uc_smsNumLen;
    templen >>= 1;
    
    for(i = 0; i < templen; i++)
    {
        tempch = uc_smsNumPdu[2 * i];		    //����ǰ���ֽڵĽ���
        uc_smsNumPdu[2 * i] = uc_smsNumPdu[2 * i + 1];
        uc_smsNumPdu[2 * i + 1] = tempch;	
    }

    templen += 6 + 2 + sc->smslength;   //AT+CMGS=len��len��ֵ
                                        //
    DevModem->stt.bit.state_wait = 1;   //��ʼ�ȴ�>	
    
    PrintValueData(
        "AT+CMGS=%d\r",
        templen
        );

    SYS_MODM_StartCnt(600);              //������ʱ������
    
    return 0;							//��ʾ׼�����ݳɹ�
}




/************************************************************************
 * @function: AT_Proc_SendSmsFlow
 * @����: ���Ͷ�������
 * 
 * @����: 
 * @param: content 
 * 
 * @����: 
 * @return: uint8  0 �����ɹ���������Ĳ���
 *		           1 ����ʧ�ܲ���������Ĳ���
 * @˵��: 
 * @����: yzy (2013/11/1)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 AT_Proc_SendSmsFlow(void* content)
{
    TModemSMS* sc = DevModem->smsSend;
    uint8 tempdata[16];
    uint8 templen;
    uint8 i;
                                        //����PDU��ʽ�ĺ��볤�ȵ�
    if(uc_smsNumType == 1)
    {
        PrintValueData(
            "001100%x291",              //"91",���ʺ���
            uc_smsNumLen
            );
    }
    else
    {
        PrintValueData(
            "001100%x281",              //"81",���ں���
            uc_smsNumLen
            );
    }
                                        //����Ŀ�����
    SER_SendData(
        SYS_GPRS_PORT,
        uc_smsNumPdu,
        uc_smsNumLen,
        1000);
                                        //���Ͷ��Ÿ�ʽ
    PrintValueData(
        "00%x201",
        sc->fmt
        );
                                        //���Ͷ��ŵĳ���
    PrintValueData(
        "%x2",
        sc->smslength
        );
                                        //���Ͷ�������
    for(i = 0; i < sc->smslength; i += 8)
    {
                                        //8���ֽ�,8���ֽڵĽ���ת��
        if(i + 8 < sc->smslength)
        {
            templen = 8;
        }
        else
        {
            templen = sc->smslength - i;
        }
    
        NumToHexString(                 //���ֽڵ�Hex�ַ�����ת��
            tempdata,
            sc->sms + i,
            templen
            );
    
        SER_SendData(                   //���һ�����ֽ�
            SYS_GPRS_PORT,
            tempdata,
            templen * 2,
            1000);
    }
    tempdata[0] = 0x1A;                 //��β��
    SYS_MODM_SendCmd(tempdata, 1, 600);	//���Ͷ��Ž���
    guc_ModemElement++;			//��������
    return 0;
}




/************************************************************************
**���Ͷ���
*************************************************************************/
const ModemElement gs_ATSendSmg[] =
{
    {"_sms send start", AT_OK, 200, 1, AT_Proc_SendSmsCmd},       //���Ͷ���,GSM_Rec_07.05
    {"_sms send data", AT_GT, 600, 1, AT_Proc_SendSmsFlow},
    {(string)0, AT_CMGS, 0, 0, (CallBackElement)0},
};



/************************************************************************
 * @function: AT_Proc_ReadMsg
 * @����: ��ȡ����
 * 
 * @����: 
 * @param: content 
 * 
 * @����: 
 * @return: uint8  0 �����ɹ���������Ĳ���
 *		           1 ����ʧ�ܲ���������Ĳ���
 * @˵��: 
 * @����: yzy (2013/11/1)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 AT_Proc_ReadMsg(void* content)
{
    //AT+CMGR=<index>
    //+CMGR:<stat>,[<alpha>],<length><CR><LF><pdu>
    
    uint16 smsidx;
    if(gs_FifoNewMessage.count > 0)	    //������µĶ��Ŷ�ȡ
    {                                   //��ȡҪ��ȡ�Ķ��ŵ�����
        SYS_FIFO_Get(&gs_FifoNewMessage, &smsidx);
    
        PrintValueData(
            "AT+CMGR=%d\r",
            smsidx
            );
            
        SYS_MODM_StartCnt(600);          //������ʱ������
        gui_DelSmgIndex = smsidx;      //��һ��Ҫɾ���Ķ��ŵ�����
    }
    else
    {
        return 1;
    }
    
    return 0;
}

/**************************************************************************
**���ն���
***************************************************************************/
const ModemElement gs_ATReadSmg[] =
{
    {"_read sms", AT_OK, 600, 1, (CallBackElement)AT_Proc_ReadMsg},
    {(string)0, AT_CMGR, 0, 0, (CallBackElement)0},
};


const ModemElement gs_ATListSmg[] =
{
    {"AT+CMGL=4",AT_OK, 300, 0, (CallBackElement)0},//AT+CMGL[=<stat>]
    {(string)0, AT_CMGL, 0, 0,  (CallBackElement)0},
};




/************************************************************************
 * @function: AT_Proc_DelMsg
 * @����: ɾ������
 * 
 * @����: 
 * @param: content 
 * 
 * @����: 
 * @return: uint8  0 �����ɹ���������Ĳ���
		   1 ����ʧ�ܲ���������Ĳ���
 * @˵��: 
 * @����: yzy (2013/11/1)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 AT_Proc_DelMsg(void* content)
{
    PrintValueData(
        "AT+CMGD=%d\r",
        gui_DelSmgIndex
        );
    SYS_MODM_StartCnt(600);          //������ʱ������
    return 0;
}

const ModemElement gs_ATDelSmg[] =
{
    {"_del sms", AT_OK, 600, 1, (CallBackElement)AT_Proc_DelMsg},
    {(string)0, AT_OK, 0, 0, (CallBackElement)0},
};



/************************************************************************
 * @function: AT_Proc_ATD
 * @����: CSD����(δ������֤)
 * 
 * @����: 
 * @param: content 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2014/11/17)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 AT_Proc_ATD(void* content)
{
    SYS_MODM_SendCmd("ATD13606503620", 14, 600);
    return 0;
}


const ModemElement gs_ATCSD[] = 		
{
    {"_set atd", AT_OK, 600, 1, (CallBackElement)AT_Proc_ATD},
    {(string)0, AT_OK, 0, 0, (CallBackElement)0},
};


const ModemElement gs_ATcsca[] =
{
    {"AT+CSCA?", AT_OK, 200, 0, (CallBackElement)0},
    {(string)0, AT_OK, 0, 0, (CallBackElement)0},
};




const ModemElement gs_ATHoldUp[] =
{
	{"ATH", AT_OK, 500, 0, (CallBackElement)0},
	{(string)0, AT_OK, 0, 0, (CallBackElement)0},
};




/************************************************************************
 * ��׼�����Ĳ��輯��
 ************************************************************************/
const ModemElement* const gs_STDAT[] =
{
    gs_ATInit,                          //Modem_Init,
    gs_ATQAndReg,                       //Modem_QAndReg,
    gs_ATSendSmg,                       //Modem_SendMsg,
    gs_ATReadSmg,                       //Modem_ReadMsg,
    gs_ATListSmg,                       //Modem_ListMsg,
    gs_ATDelSmg,                        //Modem_DelMsg,
    gs_ATCSD,                           //Modem_CSDCall,
    (void*)0,                           //Modem_CSDBecall,
    gs_ATcsca,                          //Modem_CSCA,
    gs_ATHoldUp,                        //Modem_HoldUp,
};


const uint8 STDAT_ELEMENT_NUM = (sizeof(gs_STDAT) / sizeof(void*));



