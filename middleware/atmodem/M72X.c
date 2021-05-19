/****************************************Copyright (c)**************************************************
**                               	����xx���޹�˾
**                                     		������
**
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: M72X.c
**��   ��   ��: yzy
**����޸�����: 2018��2��1��
**��        ��: GSMģ�������������
**ע        �⣺
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: yzy
** ��  ��: v1.0
** �ա���: 2007��1��18��
** �衡��: ԭʼ�汾
********************************************************************************************************/
#define EXT_M72D

#include "public.h"
#include "k_types.h"
#include "k_api.h"
//	#include "lib_app.h"
#include "hal.h"


#include "atmodem.h"

#include "modemlib.h"
#include "atstd.h"



/*******************************************************************************
**M72Xģ����Թ����е���������˵��:
1)tcpcnt�ɹ�,����ʱ��ʾerror.
���ֱ���:Ŀ��ip�˿�����,��Ŀ��IP������ӳ��˿ڵ�ǰ�û�δ����.
2)...
********************************************************************************/

uint8 guc_M72XRingCount;

/*******************************************************************************
**ͨ����,socketid��,����
********************************************************************************/
typedef enum
{
    NET_TYPE_TCPCLIENT = 0,
    NET_TYPE_TCPSERVER,
    NET_TYPE_UDP,
}e_MODM_NET_TYPE;

typedef enum
{
    CHN_DEFAULT = 0,
}e_MODM_CHN;



//M72X:Valid socket numbers - 0,1,2,3 and 4
typedef enum
{
    SOCKET_TCP_CLIENT = 0,
    SOCKET_UDP_CLIENT = 1,
//	    SOCKET_SERVER_1 = 3,
//	    SOCKET_SERVER_2 = 4,
    SOCKET_MAX = 2,
}e_MODM_SOCKET;

#define SOCKET_TCP_STR "TCP"
#define SOCKET_UDP_STR "UDP"
//	unsigned char guc_modembuff[MODEM_STRING_BUFF_LEN];

/************************************************************************
 * @function: SYS_M72X_ModemOn
 * @����: ��������(ִ�к�ռ�ô���)
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/11/15)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void SYS_M72X_ModemOn(void)
{    
    SerialSets ss;
#ifdef FARP_PORT_BAUD
    ss.baudrate = FARP_PORT_BAUD;
#else
    ss.baudrate = 19200;
#endif
    ss.databits = DataBits_8bits;
    ss.parit = Parit_N;
    ss.stopbits = StopBits_1;
//	    SYS_SER_Init(SYS_GPRS_PORT, &ss);
    SER_Init(SYS_GPRS_PORT, &ss);
    SER_Clear(SYS_GPRS_PORT);

    _ATMODEM_VIN();
  
  //��ʼֵ
    _ATMODEM_ONOFF_0();
                                            //��λ(�������ϵ�)
    //_ATMODEM_PORTCLOSE();                   //�رմ��ڵ�txd,rxd��,��ֹ���������
    //_ATMODEM_PWROFF();
    //_ATMODEM_PWRWAIT();
    //sleep_ms(1200);
    _ATMODEM_PWRON();
    //_ATMODEM_PORTOPEN();
                                            //����ʱ��
    sleep_ms(1000);
    _ATMODEM_ONOFF_1();
    sleep_ms(2000);
    _ATMODEM_ONOFF_0();
    sleep_ms(100);
    _ATMODEM_PORTOPEN();
                                            //�������ô��ڲ���
//	    SER_Open(UART_CHANNEL_DEBUG, TDB_MODE_R | TDB_MODE_W);

    guc_M72XRingCount = 0;
}


/************************************************************************
 * @function: SYS_M72X_ModemOff
 * @����: �ػ�����
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/11/15)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void SYS_M72X_ModemOff(void)
{
    _ATMODEM_PORTCLOSE();
    _ATMODEM_PWROFF();
}

/************************************************************************
 * @function: SYS_M72X_Check
 * @����: ��ѯģ���ʶ�Ƿ�ƥ��
 * @����: 
 * 
 * @����: 
 * @return: bool true�ɹ�,falseʧ��(�ɹ��󲻹رմ���,����ʶ�𵽵Ĵ������ó���ռ��)
 * @˵��: 
 * @����: yzy (2014/11/15)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
bool SYS_M72X_Check(void)
{
	SerialSets ss;
    uint8 overtime;
    uint8 uc_data[60];
    uint8 len;
    uint8 flag;
                                            //���ô��ڲ���
#ifdef FARP_PORT_BAUD
    ss.baudrate = FARP_PORT_BAUD;
#else
    ss.baudrate = 19200;
#endif
    ss.databits = DataBits_8bits;
    ss.parit = Parit_N;
    ss.stopbits = StopBits_1;
    SER_Init(SYS_GPRS_PORT, &ss);
//	    SYS_SER_Init(SYS_GPRS_PORT, &ss);
    SER_Clear(SYS_GPRS_PORT);
                                            //�򿪴���,�����ɹ��Ͳ��ر���
    SER_Open(SYS_GPRS_PORT, TDB_MODE_R | TDB_MODE_W);
    sleep_ms(200);
				                            //����ATE0,��η�����Ϊ����ģ���Զ�ʶ������.
    for(uint8 trycnt = 0; trycnt < 5; trycnt++)
    {
        ClearBuffer(uc_data, sizeof(uc_data));
        //
        SER_SendData(SYS_GPRS_PORT, "ATE0\r", 5, MODEM_SER_TIMEOUT);//ATE0
        //sleep_ms(100);
        flag = 0;
        len = SER_RecvData(SYS_GPRS_PORT, uc_data, sizeof(uc_data), MODEM_SER_TIMEOUT/5);
        for(uint8 uc_i = 0; uc_i < len; uc_i++)
        {
            if(!(CmpBuffer(uc_data + uc_i, "OK", 2)))
            {
                flag = 1;
                break;
            }
        }
        
        if(flag)
        {
            break;
        }
    }
    
    if(flag == 0)
    {
        SER_Close(SYS_GPRS_PORT);
        return false;
    }
	//

    ClearBuffer(uc_data, sizeof(uc_data));
    	                                    //���Ͷ�ȡģ���ʶָ��
    SER_SendData(SYS_GPRS_PORT, "AT+GMM\r", 7, MODEM_SER_TIMEOUT);
    overtime = 100;
    flag = 0;
    while(overtime--)
    {
        sleep_ms(60);
        
        len = SER_LineLen(SYS_GPRS_PORT);
        if((len > 0) && (len < 255))
        {
            SER_Line(SYS_GPRS_PORT, uc_data, sizeof(uc_data));
											//�Ƚ϶�ȡ�������Ƿ�Ϊ��Ӧ��ģ���ʶ
            for(uint8 uc_i = 0; uc_i < 32; uc_i++)
            {
                if((!(CmpBuffer(uc_data + uc_i, "Quectel_M72", 11))))
                {
                    flag = 1;
                    SER_Clear(SYS_GPRS_PORT);//�ظ�֡����и�"OK"��Ҫɾ��
                    break;
                }
            }
            
            if(flag)
            {
                break;
            }
        }
    }
    
    if(flag == 0)
    {
        SER_Close(SYS_GPRS_PORT);
        return false;
    }
    
    return true;                            //���سɹ�
}




/************************************************************************
 * @function: DecodeRecvIPClose
 * @����: 
 * 
 * @����: 
 * @param: mrfb 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/11/17)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void DecodeRecvIPClose(ModemRecvFeedBack* mrfb)
{
    /*��һ�ֻظ�
     *      AT+MIPCLOSE?
     *      +MIPCLOSE: 0    //No opened sockets(0��M72Xָ�����ǲ��Ϸ���socketid)
     *      OK
     *      ����
     *      +MIPCLOSE: 1,2  //Sockets 1 and 2 are opened*
     *      OK
     *�ڶ��ֻظ�
     *      AT+MIPCLOSE=1
     *      OK
     *      +MIPCLOSE: 1,0  //Socket 1�رճɹ�,�Է���Ӧ��ACK*
     *      ����
     *      +MIPCLOSE: 1,1  //Socket 1�رճ�ʱ,�Է���Ӧ��*
     *      ����
     *      +MIPCLOSE: 1,1024,2 //Socket 1�رյ��������:1024 bytes were acked,�Է����ظ�RST.
     *      //ָ��˵��+MIPCLOSE: <Socket_ID>[,<number_of_acknowledged_bytes>],<close_type>
     * 
     *ע��:��*�ŵĵ�һ�ֻظ��͵ڶ��ֻظ���Ҫ�����ﾳ���н���. 
     */
	
    uint16 byte;
    uint16 num[4];
    uint8 n = 0;                        //����������ֵ����
                                        //��ʼ��Ϊ0xffff
    memset(num, 0xff, sizeof(num));
                                        //ѭ����ȡ���5����ֵ,ֱ�����ж���
//	    for(uint8 uc_i = 0; uc_i < 4; uc_i++)
//	    {
    num[0] = GetOneChar(&byte);	//��ȡ��N������
//	        if(num[uc_i] != 0xffff)
//	        {
//	            n++;
//	        }
//	        
//	        if(byte == 0x0D || byte == 0x0A)//��һ�ж������˳�
//	        {
//	            break;
//	        }
//	    }

    //if(gs_ModemStep == Modem_IPState)   //��ѯ״̬(num1-num4Ϊopened socketid)
    {                                   //tcp�����Ƿ񻹴���
        if((num[0] == SOCKET_TCP_CLIENT))
        {
            DevModem->stt.bit.tcpcc = 0;
        }
                                        //udp�����Ƿ񻹴���
        if((num[0] == SOCKET_UDP_CLIENT))
        {
            DevModem->stt.bit.udpcc = 0;
        }
                                        //����tcp��udp����״̬ȷ��ip״̬
        if(!DevModem->stt.bit.tcpcc && !DevModem->stt.bit.udpcc)
        {
            DevModem->stt.bit.ipcc = 0;
        }
    }
    
    {
        DecodeRecvDataNone(mrfb);		//....
    }
}

/************************************************************************
 * @function: DecodeRecvIPFail
 * @����: 
 * 
 * @����: 
 * @param: mrfb 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/11/17)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void DecodeRecvIPFail(ModemRecvFeedBack* mrfb)
{
    /*��һ�ֻظ�
     *      0, CONNECT FAIL  //socket 0 ����ʧ��
     *      
     */
	
    uint16 num=0xff;
    uint16 byte;
                                        //��ʼ��Ϊ0xffff
                                        //ѭ����ȡ���5����ֵ,ֱ�����ж���
    num = GetOneChar(&byte);	//��ȡ��N������
        
    SYS_MODM_EndCnt();      //������ʱ�������ļ�ʱ  
    SYS_MODM_Error();       //�յ�ERROR֮��Ĵ���

    if((num == SOCKET_TCP_CLIENT))
    {
        SYS_MODM_Step(Modem_TcpDiscnt);
    }
                                    //udp�����Ƿ񻹴���
    if((num == SOCKET_UDP_CLIENT))
    {
        SYS_MODM_Step(Modem_UdpDiscnt);
    }


    DecodeRecvDataNone(mrfb);		//....
}

/************************************************************************
 * @function: DecodeRecvTCPData
 * @����: �����յ���IP����,����ŵ���Ӧ��FIFO��ȥ
 * 
 * @����: 
 * @param: mrfb ��������Ϣ�ṹ
 * @����: 
 * @˵��: 
 * @����: yzy (2014/11/17)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void DecodeRecvTCPData(ModemRecvFeedBack* mrfb)
{
    //+MIPRTCP: <socket_ID>,<Left>,<Data>
    //+QIRD:116.228.146.250:7070,TCP,36<CR><LF>
    TModemIPData* iprev = DevModem->ipRev;  //IP���ݽ��ջ���ָ��
    uint16 socket;
  	uint16 byte;
    uint16 len = 0;
    uint16 ctr;
    
    socket = GetOneChar(&byte);             //����socket
//	    if(SOCKET_TCP_CLIENT != socket)
//	    {
//	        return;
//	    }
                                            //
    len = GetOneChar(&byte);
    if(0xFFFF == len)
    {
        return;
    }
    DecodeRecvDataNone(mrfb);//ȥ������ 0x0A

    sleep_ms(5);
    ctr = len + 100;
	while(len > 0)                          //��ͣ�Ķ�ȡ����
	{
		byte = ReadNextByte();
        if(byte == 0x100)                   //������ݻ�û�н�����
        {
            if(ctr == 0)                    //�����ܳ���110ms��
            {
                sleep_ms(1);             //˯��10ms
                return;
            }
            sleep_ms(1);                 //˯��10ms
            ctr--;
            continue;
        }

        len--;

        if(iprev->len < LEN_OF_IPRECV)      //����ת�Ƶ����ݻ���
        {
            iprev->buffer[iprev->rp++] = byte;
            if(iprev->rp >= LEN_OF_IPRECV)  //���ƴ���
            {
                iprev->rp = 0;
            }
            iprev->len++;                   //���յ������ݳ����ۼ�
        }
	}
    
    DecodeRecvDataNone(mrfb);
}




/************************************************************************
 * @function: DecodeRecvUDPData
 * @����: �����յ���IP����,����ŵ���Ӧ��FIFO��ȥ
 * 
 * @����: 
 * @param: mrfb ��������Ϣ�ṹ
 * @����: 
 * @˵��: 
 * @����: yzy (2014/11/17)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void DecodeRecvUDPData(ModemRecvFeedBack* mrfb)
{
    TModemIPData* iprev = DevModem->ipRev;  //IP���ݽ��ջ���ָ��
    uint16 byte;
    uint16 len = 0;
    uint16 ctr;
    uint8 result[32];
                                            //��������IP,�˿ں�,
    uint8 commas = 0;
    uint8 lenmax = 24;
    uint8 uc_i;
    for(uc_i = 0; uc_i < lenmax; uc_i++)
    {
        result[uc_i] = ReadNextByte();
        if(',' == result[uc_i])
        {
            commas++;
            if(commas == 2)
            {
                break;
            }
        }
    }
    
    if(uc_i == lenmax)
    {
        return;
    }
    
    byte = ReadNextByte();                  //�ٶ���socket
    if('1' != byte)				            //socket��1
    {
        return;
    }
    byte = ReadNextByte();                  //socket֮���и�������
    if(',' != byte)				            //,
    {
        return;
    }
    
    len = GetOneChar(&byte);                //�����������ݵĳ���
    if(0X0D != byte)
    {
        return;
    }

    sleep_ms(5);
    ctr = len + 150;
	while(len > 0)                          //���ݳ��Ȳ�ͣ�Ķ�ȡ����
	{
		byte = ReadNextByte();
        if(byte == 0x100)                   //������ݻ�û�н�����
        {
            if(ctr == 0)                    //�����ܳ���110ms��
            {
                sleep_ms(1);             //˯��10ms
                return;
            }
            sleep_ms(1);                 //˯��10ms
            ctr--;
            continue;
        }

        len--;

        if(iprev->len < LEN_OF_IPRECV)      //����ת�Ƶ����ݻ���
        {
            iprev->buffer[iprev->rp++] = byte;
            if(iprev->rp >= LEN_OF_IPRECV)  //���ƴ���
            {
                iprev->rp = 0;
            }
            iprev->len++;                   //���յ������ݳ����ۼ�
        }
	}
    
    DecodeRecvDataNone(mrfb);
}


/************************************************************************
 * @function: DecodeRecvIPSend
 * @����: �յ�+MIPSEND��Ĵ���
 * 
 * @����: 
 * @param: mrfb ��������Ϣ�ṹ
 * @����: 
 * @˵��: 
 * @����: yzy (2014/11/17)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void DecodeRecvIPSend(ModemRecvFeedBack* mrfb)
{
    /*��һ�ֻظ�
        AT+MIPSEND=1,"4444" 
        +MIPSEND: 1,0,2046      //+MIPSEND:<Socket_ID>,<Status>,<FreeSize>. 
        OK                      //2048-2 chars 'DD' = 2046
    
      �ڶ��ֻظ�
        AT+MIPSEND? 
        +MIPSEND: 1,2048        //+MIPSEND:<Socket_ID>,<FreeSize>>
        +MIPSEND: 2,2048
        OK
        +QTUNBUF: 2048,3584,1536,448
    */
    
    uint16 byte;
    uint16 num[4];
    uint8 n = 0;                        //����������ֵ����
                                        //��ʼ��Ϊ0xffff
    memset(num, 0xff, sizeof(num));
                                        //ѭ����ȡ���5����ֵ,ֱ�����ж���
    for(uint8 uc_i = 0; uc_i < 4; uc_i++)
    {
        num[uc_i] = GetOneChar(&byte);	//��ȡ��N������
        if(num[uc_i] != 0xffff)
        {
            n++;
        }
        
        if(byte == 0x0D || byte == 0x0A)//��һ�ж������˳�
        {
            break;
        }
    }
                                        //AT+MIPSEND?�Ļظ�֡
//	    if(n == 2)
    {
        SetModemMMS(num[1]);
    }
}


/************************************************************************
 * @function: DecodeRecvSimDrop
 * @����: ��⵽SIM��������Ĵ���ʽ
 * 
 * @����: 
 * @param: mrfb 
 * @����: 
 * @˵��: 
 * @����: ZJC (2015/01/29)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void DecodeRecvSimDrop(ModemRecvFeedBack* mrfb)
{
    SYS_MODM_SendCmd("AT+QSIMDET", 10, 0);//ִ��AT+GTSIMRST ��λSIM��
    sleep_ms(100);                     //���н���
    
    DecodeRecvDataNone(mrfb);
}


/************************************************************************
 * @function: DecodeRecvPPPStt
 * @����: ��ȡPPP����״ֵ̬
 * 
 * @����: 
 * @param: mrfb 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/11/17)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void DecodeRecvPPPStt(ModemRecvFeedBack* mrfb)
{
    /*��һ�����:
    AT+MIPCALL=1,"mtnirancell","","" 
    OK
    +MIPCALL: 10.7.200.19
    ��
    ERROR: <err>
    +MIPCALL: 0
    
    �ڶ������:
    +MIPCALL?
    +MIPCALL: 1,123.145.167.230 //+MIPCALL: <status>[,<IP>]
    */
    uint8 str[32];
    uint16 byte;
    int num[4];
    uint8 ctget = false;
    uint8 uc_i = 0;
    uint8 n = 0;                        //����������ֵ����
                                        //��ʼ��Ϊ0xffff
    memset(num, 0, sizeof(num));
    memset(str,0,32);
    
    for(uc_i = 0; uc_i < 5; uc_i++)
    {
        byte = ReadNextByte();
//          if(byte == 0x100)
//          {
//              return;
//          }
        
        if(byte != '\r'&& byte != '\n')
        {
            str[0] = byte;    //������ַ�����һ��λ��
            ctget = true;
            break;
        }
    }
    
    if(!ctget)                          //δɨ�赽˫����,�˳�
    {
        return;
    }
                                        //�µڶ���λ�ÿ�ʼ����
    for(uc_i = 1; uc_i < sizeof(gucs_CCID); uc_i++)
    {
        byte = ReadNextByte();
        if((byte == '\r') || (byte == '\n'))//if(byte == 0x100)
        {
            break;
        }
        str[uc_i] = byte;
    }

    if(uc_i >= 11)
    {
        sscanf((const char *)str,"%d.%d.%d.%d",num,(num+1), (num+2), (num+3));
        DevModem->ModemIp = gul_localIP = (uint32)num[0] | ((uint32)num[1] << 8) | ((uint32)num[2] << 16) | ((uint32)num[3] << 24);
        DevModem->stt.bit.pppoe = 1;
    }
                                        
                                        //ѭ����ȡ���5����ֵ,ֱ�����ж���
//	    for(uint8 uc_i = 0; uc_i < 5; uc_i++)
//	    {
//	        num[uc_i] = GetOneChar(&byte);	//��ȡ��N������
//	        if(num[uc_i] != 0xffff)
//	        {
//	            n++;
//	        }
//	        
//	        if(byte == 0x0D || byte == 0x0A)//��һ�ж������˳�
//	        {
//	            break;
//	        }
//	    }
//	    
//	    if(n == 4)
//	    {
//	        gul_localIP = (uint32)num[0] | (num[1] << 8) | ((uint32)num[2] << 16) | ((uint32)num[3] << 24);
//	    }
//	    else if((n == 1) && (num[0] == 0))
//	    {
//	        DevModem->stt.bit.udpcc = 0;
//	        DevModem->stt.bit.tcpcc = 0;
//	        DevModem->stt.bit.ipcc = 0;
//	        DevModem->stt.bit.pppoe = 0;
//	        //�����ϲ���Ŀ��,�����쳣��ֹ.
//	        SYS_MODM_EndCnt();
//	        SYS_MODM_Error();
//	        //
//	    }
//	    else if((n == 5) && (num[0] == 1))
//	    {
//	        DevModem->stt.bit.pppoe = 1;
//	        gul_localIP = (uint32)num[1] | ((uint32)num[2] << 8) | ((uint32)num[3] << 16) | ((uint32)num[4] << 24);
//	    }

    DecodeRecvDataNone(mrfb);
}

/*************************************************************************
*����ATָ������                       //��STDAT_RECV��Ӧ
**************************************************************************/
const string STD_IPSTAT[] =
{
    "IP INITIAL",
    "IP START",
    "IP CONFIG",
    "IP IND",
    "IP GPRSACT",
    "IP STATUS",
    "TCP CONNECTING",
    "UDP CONNECTING",
    "IP CLOSE",
    "CONNECT OK",
    "PDP DEACT",
};

/************************************************************************
 * @function: DecodeRecvIPStt
 * @����: ��ȡIP����״ֵ̬
 * 
 * @����: 
 * @param: mrfb 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/11/17)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void DecodeRecvIPStt(ModemRecvFeedBack* mrfb)
{
    //+MIPSTAT: <socket_ID>,<n>[,<number_of_acknowl d_bytes >]
    
    /********************************************************************
    ����1:��ΪAT+MIPOPEN���쳣�ظ�֡.(��Ҫ���IPCONNECT��������쳣��ֹ)
    AT+MIPOPEN=1,34127,"217.219.84.194",1024,0 OK
    +MIPSTAT: 1,1
    AT+MIPOPEN=1,34127,"217.219.84.194",1024,0 OK
    +MIPOPEN: 1,1
    
    ����2:�����쳣ʱ�������ϱ�֡.
    ************************************************************************/
    
    uint16 socketid;
    uint16 stat;
    uint16 byte;
    //uint8 rdlen = 0;
    uint8 itmidx = 0;
    //uint8 rdbuf[64];
    uint8 linelen = 0;
    //uint16 ebytes = 0;
    //memset(rdbuf,0,64);
    
//	    rdlen =SER_Line(SYS_GPRS_PORT, uc_data, sizeof(uc_data));
//	
//	    for(itmidx = 0; itmidx < (sizeof(STD_IPSTAT) / sizeof(string)); itmidx++)
//	    {                                       //1)��ȡ��ATָ�������ַ�����ʶ�𳤶�
//	        uint8 itmlen = strlen(STD_IPSTAT[itmidx]);
//	                                            //2)��ȡ�ĳ���>=�����ַ�������,ִ��ָ��ʶ�����
//	        if(rdlen >= itmlen) 
//	        {
//	            if(0 == CmpBuffer(uc_data, (uint8*)STD_IPSTAT[itmidx], itmlen))
//	            {
//	                DequeueByte(itmlen);        //�Ӵ��ڻ�����,ɾ�������ַ���
//	                break;              //���ط��ϵ�ָ������
//	            }
//	        }
//	    }

    
    socketid = GetOneChar(&byte);		    //��ȡ��һ������
    linelen = SER_LineLen(SYS_GPRS_PORT);
    //stat = GetOneChar(&byte);               //��ȡ�ڶ�������(0��ȷ,otherʧ��)
//	    stat = itmidx;
//	    if(stat >= 4)
//	    {
//	        DevModem->stt.bit.pppoe = 1;        //ppp�����ɹ�
//	    }
    if(socketid == SOCKET_TCP_CLIENT)
    {
        if(linelen > 9)
        {
            DevModem->stt.bit.tcpcc = 1;
        }
        else
        {
            DevModem->stt.bit.tcpcc = 0;
            //�����ϲ���Ŀ��,�����쳣��ֹ.
            if(gs_ModemStep == Modem_TcpCnt)
            {
                SYS_MODM_EndCnt();
                SYS_MODM_Error();
            }
            //
        }
    }
    else if(socketid == SOCKET_UDP_CLIENT)
    {
        if(linelen > 9)
        {
            DevModem->stt.bit.udpcc = 1;
        }
        else
        {
            DevModem->stt.bit.udpcc = 0;
            //�����ϲ���Ŀ��,�����쳣��ֹ.
            if(gs_ModemStep == Modem_UdpCnt)
            {
                SYS_MODM_EndCnt();
                SYS_MODM_Error();
            }
            //
        }
    }
                                            //����tcp��udp����״̬ȷ��ip״̬
    if(DevModem->stt.bit.tcpcc || DevModem->stt.bit.udpcc)
    {
        DevModem->stt.bit.ipcc = 1;
    }
    else
    {
        DevModem->stt.bit.ipcc = 0;
    }

    DecodeRecvDataNone(mrfb);
}




/************************************************************************
 * @function: DecodeRecvIMEI
 * @����: ��ȡͨѶģ���IMEI
 * 
 * @����: 
 * @param: mrfb 
 * @����: 
 * @˵��: 
 * @����: yzy (2013/11/1)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void DecodeRecvIMEI(ModemRecvFeedBack* mrfb)
{
    /*
    AT+CGSN? 
    +CGSN: ��004400013805666�� 
    OK
    */
    
    uint16 byte;
    uint8 ctget = false;                //�Ƿ�ɨ�赽��Ч���ݵ�ͷ

    ClearBuffer(gucs_IMEI, sizeof(gucs_IMEI));
    
                                        //ɨ���һ��˫����,���ɨ��5���ַ�
    for(uint8 uc_i = 0; uc_i < 5; uc_i++)
    {
        byte = ReadNextByte();
        //if(byte == 0x100)
        //{
        //    return;
        //}
        
        if(byte != '\r'&& byte != '\n')
        {
            gucs_IMEI[0] = byte;
            ctget = true;
            break;
        }
    }
    
    if(!ctget)                          //δɨ�赽˫����,�˳�
    {
        return;
    }
                                        //IMEI�����15�ֽ�
    for(uint8 uc_i = 1; uc_i < sizeof(gucs_IMEI); uc_i++)
    {
        byte = ReadNextByte();
        if((byte == '\r') || (byte == '\n'))
        {
            break;
        }
        gucs_IMEI[uc_i] = byte;
    }
                                        //ɾ������ʣ���ַ�
    DecodeRecvDataNone(mrfb);
}




/************************************************************************
 * @function: DecodeRecvCCID
 * @����: ��ȡSIM�����к�
 * 
 * @����: 
 * @param: mrfb 
 * @����: 
 * @˵��: 
 * @����: yzy (2013/11/1)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void DecodeRecvCCID(ModemRecvFeedBack* mrfb)
{
    /*
    AT+CCID? 
    +CCID: 89860018190839008096
    OK
    */
    
    uint16 byte;
    uint8 ctget = false;                //�Ƿ�ɨ�赽��Ч���ݵ�ͷ
    uint8 idx = 0;

    ClearBuffer(gucs_CCID, sizeof(gucs_CCID));

                                        //ɨ���һ���ǿո��ַ�,���ɨ��5���ַ�
    for(uint8 uc_i = 0; uc_i < 5; uc_i++)
    {
        byte = ReadNextByte();
//	        if(byte == 0x100)
//	        {
//	            return;
//	        }
        
        if(byte != '\r'&& byte != '\n')
        {
            gucs_CCID[idx++] = byte;    //������ַ�����һ��λ��
            ctget = true;
            break;
        }
    }
    
    if(!ctget)                          //δɨ�赽˫����,�˳�
    {
        return;
    }
                                        //�µڶ���λ�ÿ�ʼ����
    for(uint8 uc_i = 1; uc_i < sizeof(gucs_CCID); uc_i++)
    {
        byte = ReadNextByte();
        if((byte == '\r') || (byte == '\n'))//if(byte == 0x100)
        {
            break;
        }
        gucs_CCID[uc_i] = byte;
    }
    
    DecodeRecvDataNone(mrfb);
}



/************************************************************************
 * @function: DecodeRecvCGMR
 * @����: ��ȡģ��汾��Ϣ
 * 
 * @����: 
 * @param: mrfb 
 * @����: 
 * @˵��: 
 * @����: yzy (2013/11/1)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void DecodeRecvCGMR(ModemRecvFeedBack* mrfb)
{
    /*
    AT+CGMR
    +CGMR: "G510_V0D.00.16"
    OK
    */
    
    uint16 byte;
    uint8 ctget = false;                //�Ƿ�ɨ�赽��Ч���ݵ�ͷ

    ClearBuffer(gucs_ATMID, sizeof(gucs_ATMID));
    
                                        //ɨ���һ��˫����,���ɨ��5���ַ�
    for(uint8 uc_i = 0; uc_i < 5; uc_i++)
    {
        byte = ReadNextByte();
        //if(byte == 0x100)
        //{
        //    return;
        //}
        
        if(byte == ' ')
        {
            ctget = true;
            break;
        }
    }
    
    if(!ctget)                          //δɨ�赽˫����,�˳�
    {
        return;
    }
    
    for(uint8 uc_i = 0; uc_i < sizeof(gucs_ATMID); uc_i++)
    {
        byte = ReadNextByte();
        if((byte == '\r') || (byte == '\n'))//if((byte == 0x100) || (byte == '"'))
        {
            break;
        }
        gucs_ATMID[uc_i] = byte;
    }
    
    DecodeRecvDataNone(mrfb);
}





/*******************************************************************************
*����ATָ�����е�ö��
*******************************************************************************/
typedef enum                      		
{
    M72X_READY = NUM_AT_RECVITM,

    M72X_SIMDROP,                           //SIM ����
     
    M72X_MIPCALL,
    M72X_MIPCLOSE,                          //�ر�����/��ѯ����״̬
    M72X_MIPOPEN,
    M72X_MIPRTCP,
    M72X_MIPRUDP,
    M72X_MIPSEND,
    M72X_MIPSTAT,                           //�����ϱ�����״̬
    M72X_MIPSETS,                           //����socket��buffersize&timerout�ظ�,���Ͷ�������ʱtimerout��0(�ò�����)
    M72X_MIPPUSH,                           //+MIPSEND֮ǰ�ķ���״̬�ϱ�֡

    M72X_IMEI,
    M72X_CCID,
    M72X_CGMR,
    M72X_QISACK,
    M72X_QISRVC,
    M72X_DEACT,
    M72X_ALREADY,
    M72X_CLOSE,
    M72X_CONNECTFAIL,
    M72X_SENDOK,
    M72X_RECEIVE,
    NUM_M72X_RECVITM,                       //�ɽ���ָ�������
}M72X_RECV;


/*******************************************************************************
*����ATָ������
*******************************************************************************/
const string M72X_ATRecvHdr[NUM_M72X_RECVITM] =
{
    "CALL READY",
    
    "+SIM DROP",
      
    "AT+QILOCIP",//"+MIPCALL:",
    "CLOSE OK",//"+MIPCLOSE:",
    "CONNECT OK",   //"+MIPOPEN:",
    "+QIRD:",       //"+MIPRTCP:",
    "+MIPRUDP:",
    "+QTUNBUF:",      //"+MIPSEND:",
    "+QISTAT:",//"STATE: ",        //"+MIPSTAT:",
    "+MIPSETS:",
    "+MIPPUSH:",
    
    "AT+CGSN",                              //ģ��IMEI��
    "AT+QCCID",                              //SIM��CCID��
    "Revision",//"+CGMR:",                              //ģ��汾��Ϣ
    "+QISACK:",
    "+QISRVC:",
    "DEACT",
    "ALREADY CONNECT",
    "CLOSE",
    "CONNECT FAIL",
    "SEND OK",
    "+RECEIVE:"
    
};
/************************************************************************
 * @function: DecodeRecvDataNone
 * @����: ȥ����һ����ʣ�µ�����
 * 
 * @����: 
 * @param: mrfb ��������Ϣ�ṹ(�ú����ò���)
 * @����: 
 * @˵��: 
 * @����: yzy (2014/11/13)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void DecodeRecvAlready(ModemRecvFeedBack* mrfb)
{
	uint16 byte;						//����Ӵ��ڶ������ֽ�
	
	while(1)
	{
		byte = ReadNextByte();			//��ȡ��һ���ֽ�
		if(byte == 0x0D || byte == 0x0A)//��һ�ж������˳�
		{
			break;
		}
		
		if(byte > 255)					//���е����ݶ�����Ҳ�˳�
		{
			break;
		}	
	}
    mrfb->recv = M72X_MIPOPEN;
    //if(socketid == SOCKET_TCP_CLIENT)
    {
        {
            DevModem->stt.bit.tcpcc = 1;
        }
    }
    //else if(socketid == SOCKET_UDP_CLIENT)
    {
        {
            DevModem->stt.bit.udpcc = 1;
        }    
    }
    DevModem->stt.bit.ipcc = 1;
}


/*******************************************************************************
*ATָ�����Ԥ��������
*******************************************************************************/
const DecodeRecvData gs_M72XDecodeRecvData[NUM_M72X_RECVITM] =
{
	DecodeRecvDataNone,					//M72X_READY,
    
    DecodeRecvSimDrop,
    
    DecodeRecvPPPStt,                   //M72X_MIPCALL,
    DecodeRecvDataNone,//DecodeRecvIPClose,                  //M72X_MIPCLOSE,
    DecodeRecvDataNone,                 //M72X_MIPOPEN, //����ִ��ָ��,���ò�ѯָ��,��˲����ж���ϸ����
	DecodeRecvTCPData,                  //M72X_MIPRTCP,
    DecodeRecvUDPData,                  //M72X_MIPRUDP,
    DecodeRecvIPSend,                   //M72X_MIPSEND,
	DecodeRecvIPStt,				    //M72X_MIPSTAT,
    DecodeRecvDataNone,                 //M72X_MIPSETS,
    DecodeRecvDataNone,                 //M72X_MIPPUSH,
    
    DecodeRecvIMEI,                     //M72X_IMEI
    DecodeRecvCCID,                     //M72X_CCID
    DecodeRecvCGMR,                     //M72X_CGMR
    
    DecodeRecvDataNone,
    DecodeRecvDataNone,
    DecodeRecvDataNone,
    
    DecodeRecvAlready,
    DecodeRecvIPClose,
    DecodeRecvIPFail,
    DecodeRecvDataNone,
    DecodeRecvTCPData,
};



/************************************************************************
 * @function: SYS_M72X_DecodeRecvCmd
 * @����: ���ڽ������ݵĽ�������
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2013/11/1)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void SYS_M72X_DecodeRecvCmd(void)
{
    uint8 order;
    uint16 byte;
    ModemRecvFeedBack mrfb;				//Ԥ�����ɨ����

    if(DevModem->stt.bit.state_wait)    //���ڵȴ�'>'״̬
    {
        //
    }                                   //���û���ҵ���
    else if(SER_LineLen(SYS_GPRS_PORT) == 0)
    {
        if(SER_ToBeRead(SYS_GPRS_PORT) == LEN_UARTR_GPRS)
        {                               //����������˻�û���ҵ��еĻ�
            SER_Clear(SYS_GPRS_PORT); //��ջ���
        }
        return;
    }
    
    while(true)
    {                                   //ɨ���ʶ��Ϸ�ָ��
        order = ATCommandRecvMap(M72X_ATRecvHdr, NUM_M72X_RECVITM);
        
        if(order != 0xFF)               //�Ѿ�ɨ�赽�˺Ϸ�����
        {                               //����ʣ������
            if(order < NUM_M72X_RECVITM)
            {
                memset(&mrfb, 0x00, sizeof(ModemRecvFeedBack));
                mrfb.recv = order;
                
                if(order < NUM_AT_RECVITM)
                {
//#if (Modem_M72X_SMS_EN > 0)                
                    gs_STDATDecodeRecvData[order](&mrfb);
//#endif
                }
                else
                {
                    gs_M72XDecodeRecvData[order - NUM_AT_RECVITM](&mrfb);
                }
            
                if(DevModem->stt.bit.smWait == 0)	//����Ҫ�ȴ�ʣ����������������xx
                {
                    SYS_MODM_SynRecv(&mrfb);
                }
            }
        }
        else
        {                               //�����ϳ����ַ���,�ж��Ƿ�Ϊ��������
            if(DevModem->stt.bit.smWait)//���Ž���
            {
                byte = ReadNextByte();
                if(IsBcdChar(byte))	    //�ж��Ƿ���BCD����
                {
                    DevModem->stt.bit.smWait = 0;
                                        //����PDU����
                    if(DecodeAndEnqueuePduMsg(byte))
                    {
                        memset(&mrfb, 0x00, sizeof(ModemRecvFeedBack));
                        mrfb.recv = AT_CMGR;	
                        mrfb.proced = DevModem->smsRev; //�յ��Ķ���
                        SYS_MODM_SynRecv(&mrfb);
                    }
                }
            }
                                        //ɾ������
            if(order != 0xfe)
            {
                DequeueLine();
            }
        }
                                        //1)�ȴ�'>'
        if(DevModem->stt.bit.state_wait && 
           (SER_ToBeRead(SYS_GPRS_PORT) > 0))
        {
            continue;
        }
                                        //2)�ж��Ƿ񻹴����µ�һ��
        if(SER_LineLen(SYS_GPRS_PORT) == 0)
        {
            SER_Clear(SYS_GPRS_PORT); //��ջ���(added by yzy 2014/11/13)
            break;				        //�����������ɨ��
        }
    }
}





/************************************************************************
 * @function: M72X_Proc_DumbWait
 * @����: �����κβ���,�����ȴ��¸�����
 * 
 * @����: 
 * @param: content 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2013/11/6)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
#define M72X_Proc_DumbWait AT_Proc_DumbWait










/************************************************************************
miss juice 2014/11/27 10:47:07
����,AT+MIPCALL=1����ָ��͵��յ�AT+MIPCALL=1,�����ʱ�Ƕ���?

���ͨ������ 2014/11/27 11:02:07
���ʱ�������������ģ�鶨������ȡIPʱ��120S����ʱ�ͻ��ϱ�ʧ��

���ͨ������ 2014/11/27 11:03:25
�и���ʱ��ȡ����

���ͨ������ 2014/11/27 11:06:07
AT+GTSET=��ACTRETRY��,X,Y
X: ȡֵ��Χ��  5-30 ......... PDP����ĳ�ʱ����ʱ�䣬��λ���룬ģ��Ĭ��30,
Y: ȡֵ��Χ��2-4   ����ĳ�ʱ��ģ���Զ����¼������, ȱʡ4����������һ�γ�ʱ������
************************************************************************/



/************************************************************************
 * @function: M72X_Proc_SetAPN
 * @����: ����APN
 * 
 * @����: 
 * @param: content 
 * 
 * @����: 
 * @return: uint8  0 �����ɹ���������Ĳ���
 * 		           1 ����ʧ�ܲ���������Ĳ���
 * @˵��: 
 * @����: yzy (2013/11/6)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 M72X_Proc_SetAPN(void* content)
{
	uint8 len;
										//��������
	len = strlen((string)DevModem->log.apn);
	
	if(len > 32)						//�������32�򷵻ش���
	{
		return 1;						//....
	}
    unsigned char uc_modembuff[MODEM_STRING_BUFF_LEN];
    memset(uc_modembuff,0,MODEM_STRING_BUFF_LEN);
    //+MIPCALL=<Operation>[<APN>/<phone number>,[<User name>,<Password>]]
    //����<Operation>����:
    //0-disconnect a link,
    //1-establish GPRS link,
    //2-establish CSD link
                                        //APN
//	    PrintValueData(
//	        "AT+QICSGP=1,\"%s\",\"",
//	        (uint32)DevModem->log->apn
//	        );
//	                                        //�û���
//	    PrintValueData(
//	        "%s\",\"",
//	        (uint32)DevModem->log->usr
//	        );
//	                                        //����
//	    PrintValueData(
//	        "%s\"\r",
//	        (uint32)DevModem->log->pwd
//	        );
    SER_PrintValue(SYS_GPRS_PORT, uc_modembuff, MODEM_STRING_BUFF_LEN, MODEM_SER_TIMEOUT,
        "AT+QICSGP=1,\"%s\",\"%s\",\"%s\"\r",DevModem->log.apn,DevModem->log.usr,DevModem->log.pwd);
#if SYS_MNT_EN > 0                           //��ʼ��ʼ��ATͨ��
    LOG_DEBUG("%s\n", uc_modembuff);    
#endif

	SYS_MODM_StartCnt(500);             //������ʱ������
    guc_ModemElement++;					//��������
	return 0;
}

/**************************************************************************
**��ʼ������
***************************************************************************/
const ModemElement gs_M72XInit[] =
{
    {"AT+QIMUX=1",      AT_OK, 500,  0, (CallBackElement)0},   //disable the function of MUXIP 

    {(string)0, AT_OK, 0, 0, (CallBackElement)0},
};

const ModemElement gs_M72XPPPoE[] =
{
	{"AT+QIFGCNT=0",    AT_OK, 200,  0, (CallBackElement)0},//set the context 0 as FGCNT	

    {"AT+CGATT=1",      AT_OK, 1200, 0, (CallBackElement)0},        //AT+CGATT=[<state>],����GPRS�����������
//    {"AT+QIMODE=0",     AT_OK, 500,  0, (CallBackElement)0},    //set the session mode as non-transparent
    {"AT+QIDNSIP=0",    AT_OK, 2000,  0, (CallBackElement)0},    //use IP address as the address to establish TCP/UDP session
    {"_set APN",        AT_OK, 1000, 1, (CallBackElement)M72X_Proc_SetAPN},
    {"AT+QIDEACT",     AT_OK, 2000,  0, (CallBackElement)0},
    {"AT+QIREGAPP",     M72X_DEACT, 2000,  0, (CallBackElement)0},
    {"AT+QIACT",        AT_OK, 10000, 0, (CallBackElement)0},
	{(string)0, AT_OK, 0, 0, (CallBackElement)0},//{(string)0, M72X_MIPCALL, 0, 0, (CallBackElement)0},
};


const ModemElement gs_M72XDiscPPP[] =
{
    {"AT+CSQ", AT_OK, 200, 0, (CallBackElement)0},
    {(string)0, AT_OK, 0, 0, (CallBackElement)0},
};




/************************************************************************
 * @function: M72X_Proc_MIPCONF
 * @����: ʹ�ܷ���״̬�ϱ�����
 * 
 * @����: 
 * @param: content ��һ����ִ�н��
 * 
 * @����: 
 * @return: uint8  0 �����ɹ���������Ĳ���
 *                  1 ����ʧ�ܲ���������Ĳ��� 
 * @˵��: 
 * @����: yzy (2014/11/17)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 M72X_Proc_MIPCONF(void* content)
{
    //AT+MIPCONF=<socket_ID>[[,<retr_num>][,<min_TO>][,<max_TO>][,<max_close_delay>] [,<is_nack_ind_req>]]
    //retr_num�����ط�����,Ҳ���ǵȴ���ʱ
    //is_nack_ind_reqʹ�ܷ���״̬�ϱ�.
    //�м�����ֵ���鲻��,��Ϊ10,600,75
    uint8 SocketID;
    unsigned char uc_modembuff[64];
    
//	    if(gs_ModemStep == Modem_TcpCnt)	    //����TCP
//		{
//	        SocketID = SOCKET_TCP_CLIENT;
//		}
//		else                                    //����UDP
//		{
//	        SocketID = SOCKET_UDP_CLIENT;
//		}
//	    
//	                                            //Socket_ID
//	    PrintValueData(
//	        "AT+MIPCONF=%d,",
//	        SocketID
//	        );
//	    
//	    SYS_MODM_SendCmd("5,10,600,75,2", 13, 20);

    SER_PrintValue(SYS_GPRS_PORT, uc_modembuff, 64, MODEM_SER_TIMEOUT,
        "AT+QITCFG=%s\r\n","3,2,512,1");
#if SYS_MNT_EN > 0                           //��ʼ��ʼ��ATͨ��
    LOG_DEBUG("%s\n", uc_modembuff);    
#endif

    SYS_MODM_StartCnt(2000);
    guc_ModemElement++;				        //�ֲ����ۼ�
    return 0;
}


/************************************************************************
 * @function: M72X_Proc_IPOpen
 * @����: ����IP����
 * 
 * @����: 
 * @param: content 
 * 
 * @����: 
 * @return: uint8  0 �����ɹ���������Ĳ���
 *                  1 ����ʧ�ܲ���������Ĳ���
 * @˵��: 
 * @����: yzy (2014/11/17)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 M72X_Proc_IPOpen(void* content)
{
	Word32 wd;
	uint8 uc_data[21];
    uint8 i;
    uint8 nettype;
    uint8 SocketID;
    uint8 * socketType = NULL;
    unsigned char uc_modembuff[MODEM_STRING_BUFF_LEN];
    TModemIPAddr* tia = &DevModem->ipAddr;   //�ǲ�������ָ��,ʹ��ȫ�������ǳ�����(TModemIPAddr*)content;
    wd.lword = tia->addr;

    if(gs_ModemStep == Modem_TcpCnt)	    //����TCP
	{
	    socketType = SOCKET_TCP_STR;
        SocketID = SOCKET_TCP_CLIENT;
        nettype = NET_TYPE_TCPCLIENT;
	}
	else                                    //����UDP
	{
	    socketType = SOCKET_UDP_STR;
        SocketID = SOCKET_UDP_CLIENT;
        nettype = NET_TYPE_UDP;
	}

    //AT+MIPOPEN=<Socket_ID>,<Source_Port>,<Remote_IP>,<Remote_Port>,<Protocol>
    /************************************************
    AT+MIPOPEN=1,34127,"217.219.84.194",1024,0 OK
    +MIPSTAT: 1,1
    AT+MIPOPEN=1,34127,"217.219.84.194",1024,0 OK
    +MIPOPEN: 1,1
    *************************************************/
                                            //Socket_ID
//	    PrintValueData(
//	        "AT+MIPOPEN=%d,",
//	        SocketID
//	        );
//	                                            //Source_Port
//	    PrintValueData(
//	        "%d,\"",
//	        (gul_Random % (49999 - 5000)) + 5001//ȡ  5001--49999  ֮��������
//	        );

	for(i = 0; i < 4; i++)                  //ѭ��ת��IP��ַ
	{
		NumToBcdString(uc_data + i * 4, wd.byte[i], 3);
		uc_data[i * 4 + 3] = '.';
	}
											//ת���˿ں�
//	    for(i = 0; i < 3; i++)                  //��ӡĿ���ַǰ3�ֽ�
//		{
//	        PrintValueData(
//	            "%d.",
//	            wd.byte[i]
//	            );
//		}
//	                                            //��ӡĿ���ַ�����ֽ�
//		PrintValueData(
//	        "%d\"",
//	        wd.byte[3]
//	        );
//	                                            //���Ͷ˿ں�
//	    PrintValueData(
//	        ",%d,",
//	        tia->port
//	        );
//	                                            //Э������
//	    PrintValueData( 
//	        "%d\r",
//	        nettype
//	        );

    SER_PrintValue(SYS_GPRS_PORT, uc_modembuff, MODEM_STRING_BUFF_LEN, MODEM_SER_TIMEOUT,
        "AT+QIOPEN=%d,\"%s\",\"%d.%d.%d.%d\",\"%u\"\r\n", SocketID, socketType,wd.byte[0], 
        wd.byte[1],wd.byte[2],wd.byte[3],tia->port);

#if SYS_MNT_EN > 0                           //��ʼ��ʼ��ATͨ��
    LOG_DEBUG("%s\n", uc_modembuff);    
#endif

	SYS_MODM_StartCnt(90000);                  //���鳬ʱ����1.5����
	guc_ModemElement++;						//�ֲ����ۼ�
	
	return 0;
}



/************************************************************************
18:22:44
���ͨ������ 2014/11/20 18:22:44
"MIPCONF"�����һ����������,
ָ�����ú󣬳ɹ����͵�����������Զ��ϱ������¡�����ʹ�ü��ֲ�˵����
  ����ʹ���������ò����������������ʱ������ʹ�ã�

AT+MIPCALL=1,"CMNET"
OK
+MIPCALL: 10.255.198.138
AT+MIPCONF=1,5,5,600,75,2  --���÷������ɹ��յ����ݺ��ϱ���
OK
AT+MIPOPEN=1,,"203.195.189.64",3000,0
OK
+MIPOPEN: 1,1
AT+MIPSEND=1,5
>12345
OK
+MIPPUSH: 1,0,5  ---��ʾ�������ɹ��ӵ�����
************************************************************************/

/************************************************************************
���ͨ-������  12:03:08
AT+MIPSETS=1,1024,100 ERROR  
����ָ��G610-A20ģ���ǲ�֧�ֵ�,A50֧��
���ͨ-������  12:09:45
AT+MIPSEND=1,5�����Ƕ�����������
�Ƽ���  12:10:06
�����Ǹ���ʱʱ�䲻���ã�û��ϵô��
���ͨ-������  12:10:07
���Ͷ������ݾͲ���Ҫʹ�����ָ��,û��ϵ��
���Ͳ���������ʱ,��Ҫ���ó�ʱʱ��.
************************************************************************/

/********A20��A50ģ�����IPOPENָ��Ĳ���*********************************
*A20:�ȷ��ء�OK��,�ٷ��ء�+MIPOPEN: 1,1��;����ʱ,�ͻ���������ʱ,�ϱ���IP��ַû������
*A50:�ȷ��ء�+MIPOPEN: 1,1��,�ٷ��ء�OK��;�ϱ���IP��ַ������.
************************************************************************/
const ModemElement gs_M72XTcpCnt[] =
{
//	    {"AT+QIREGAPP",     AT_OK, 200,  0, (CallBackElement)0},
//	    {"AT+QIACT",        AT_OK, 5000, 0, (CallBackElement)0},
    {"ATE1", AT_OK, 500, 0, (CallBackElement)0},
    {"AT+QILOCIP",        AT_OK, 5000, 0, (CallBackElement)0},
    {"ATE0", M72X_MIPCALL, 500, 0, (CallBackElement)0},
    //{"_ip config",      AT_OK, 500, 1, (CallBackElement)M72X_Proc_MIPCONF},//yzy ��������,���ⲻͬģ��汾Ĭ��ֵ��ͬ
//	    {"AT+QIMUX=1",      AT_OK, 500,  0, (CallBackElement)0},
    {"_open ip link",   AT_OK, 500, 1, (CallBackElement)M72X_Proc_IPOpen},
//	    {"AT+QIMUX=0",      AT_OK, 500,  0, (CallBackElement)0},
    {"AT+QTUNBUF?",     M72X_MIPOPEN, 200, 0, (CallBackElement)0},          //�����A20&A50MIPSEND
	{"_dump wait",      M72X_MIPSEND, 0, 1, (CallBackElement)M72X_Proc_DumbWait},
    {(string)0, AT_OK, 0, 0, (CallBackElement)0},
};

#define gs_M72XUdpCnt gs_M72XTcpCnt		//˵��UDP���ӵĲ����TCP��һ�µ�






/************************************************************************
 * @function: M72X_Proc_DisCntTcp
 * @����: �Ͽ�GPRS����
 * 
 * @����: 
 * @param: content 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2013/11/6)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 M72X_Proc_DisCntTcp(void* content)
{
    uint8 chn = (uint32)content;
    uint8 SocketID;

    SocketID = SOCKET_TCP_CLIENT;
                                        //��������ָ��//AT+MYNETCLOSE=<socketid>
    PrintValueData(
        "AT+QICLOSE\r",
        SocketID
        );
#if SYS_MNT_EN > 0                           //��ʼ��ʼ��ATͨ��
    LOG_DEBUG("TCP:AT+QICLOSE\n");    
#endif
    SYS_MODM_StartCnt(5000);
    return 0;

}



/************************************************************************
 * @function: M72X_Proc_DisCntUdp
 * @����: �Ͽ�GPRS����
 * 
 * @����: 
 * @param: content 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2013/11/6)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 M72X_Proc_DisCntUdp(void* content)
{
    //uint8 chn = (uint32)content;
    uint8 SocketID;
	
    SocketID = SOCKET_UDP_CLIENT;
//	                                        //��������ָ��//AT+MYNETCLOSE=<socketid>
//	    PrintValueData(
//	        "AT+MIPCLOSE=%d\r",
//	        SocketID
//	        );
    PrintValueData(
        "AT+QICLOSE\r",
        SocketID
        );
#if SYS_MNT_EN > 0                           //��ʼ��ʼ��ATͨ��
    LOG_DEBUG("UDP:AT+QICLOSE\n");    
#endif    
    SYS_MODM_StartCnt(5000);
    return 0;

}

const ModemElement gs_M72XDisTcp[] = 
{
    {"_close tcp link", AT_OK, 3000, 1, (CallBackElement)M72X_Proc_DisCntTcp},
    {(string)0, M72X_SENDOK, 50, 0, (CallBackElement)0},    //�ڽ��պ����д���
};


const ModemElement gs_M72XDisUdp[] = 
{
    {"_close udp link", AT_OK, 3000, 1, (CallBackElement)M72X_Proc_DisCntUdp},
    {(string)0, M72X_SENDOK, 50, 0, (CallBackElement)0},    //�ڽ��պ����д���
};





/************************************************************************
 * @function: M72X_Proc_SendTcpCmd
 * @����: ����Զ����������
 * 
 * @����: 
 * @param: content 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2013/11/6)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 M72X_Proc_SendTcpCmd(void* content)
{
    //AT+MIPSEND=<Socket_ID>,<Data>
                                        //��������
    PrintValueData(
        "AT+QISEND=0,%d\r",            //SOCKET_TCP_CLIENT
        DevModem->ipSend->len
        );
#if SYS_MNT_EN > 0                           //��ʼ��ʼ��ATͨ��
    LOG_DEBUG("AT+QISEND=0,%d\r\n", DevModem->ipSend->len);    
#endif
	SYS_MODM_StartCnt(5000);               //������ʱ������
                                        //
	DevModem->stt.bit.state_wait = 1;   //��ʼ�ȴ�>
	guc_ModemElement++;		
    return 0;							//��ʾ׼�����ݳɹ�
}


/************************************************************************
 * @function: M72X_Proc_SendUdpCmd
 * @����: ����Զ����������
 * 
 * @����: 
 * @param: content 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2013/11/6)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 M72X_Proc_SendUdpCmd(void* content)
{
    //AT+MIPSEND=<Socket_ID>,<Dlen>
    //>
    //HEXDATA
                                        //��������
    PrintValueData(
        "AT+MIPSEND=1,%d\r",            //SOCKET_UDP_CLIENT
        DevModem->ipSend->len
        );
#if SYS_MNT_EN > 0                           //��ʼ��ʼ��ATͨ��
    LOG_DEBUG("AT+MIPSEND=1,%d\r\n", DevModem->ipSend->len);    
#endif
	SYS_MODM_StartCnt(5000);               //������ʱ������
                                        //
	DevModem->stt.bit.state_wait = 1;   //��ʼ�ȴ�>
	guc_ModemElement++;		
    return 0;							//��ʾ׼�����ݳɹ�
}


/************************************************************************
 * @function: GDAT_Proc_SendIPFlow
 * @����: ����IP����
 * 
 * @����: 
 * @param: content 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2013/11/6)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 M72X_Proc_SendIPFlow(void* content)
{
    //AT+MIPSEND=<Socket_ID>,<Dlen>
    //>
    //HEXDATA
//	    PrintValueData(
//	        "AT+QISEND=%d\r\n>",            //SOCKET_TCP_CLIENT
//	        DevModem->ipSend->len
//	        );    
//	#if SYS_MNT_EN > 0                           //��ʼ��ʼ��ATͨ��
//	    LOG_DEBUG("AT+QISEND=%d\r\n", DevModem->ipSend->len);    
//	#endif
    
                                        //����(yzy,���Բ��÷���"\r\n")
    SendCommadData(DevModem->ipSend->buffer, DevModem->ipSend->len);
#if SYS_MNT_EN > 0                           //��ʼ��ʼ��ATͨ��
    LOG_DUMPHEX(LOG_LEVEL_DEBUG,"", DevModem->ipSend->buffer,DevModem->ipSend->len);    
#endif

    SYS_MODM_StartCnt(20000);              //������ʱ������
	guc_ModemElement++;	
    return 0;
}


const ModemElement gs_M72XTcpSend[] =
{
	{"_tcp send start", AT_OK, 0, 1, (CallBackElement)M72X_Proc_SendTcpCmd},
	{"_tcp send data", AT_GT, 0, 1, (CallBackElement)M72X_Proc_SendIPFlow},
//	    {"_wait again", M72X_SENDOK, 0, 1, (CallBackElement)M72X_Proc_DumbWait},
    {(string)0, M72X_SENDOK, 0, 0, (CallBackElement)0},
};


const ModemElement gs_M72XUdpSend[] =
{
	{"_udp send start", AT_OK, 0, 1, (CallBackElement)M72X_Proc_SendUdpCmd},
	{"_udp send data", AT_GT, 0, 1, (CallBackElement)M72X_Proc_SendIPFlow},
//	    {"_wait again", M72X_SENDOK, 0, 0, (CallBackElement)M72X_Proc_DumbWait},
    {(string)0, M72X_SENDOK, 0, 0, (CallBackElement)0},
};










//��ѯ״̬ʱ,����ѯPPP״̬,ֻ��ѯIP״̬.��IP����ʧ��ʱ,�Ͽ�PPP+IP��ȫ����������.
const ModemElement gs_M72XIPStt[] =
{
	{(string)"AT+QISTAT", AT_OK, 600, 0, (CallBackElement)0},//QISTAT
    {(string)0, AT_OK, 0, 0, (CallBackElement)0},
};


/************************************************************************
 * @function: M72X_Proc_ShutDown
 * @����: �ر�
 * 
 * @����: 
 * @param: content 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2013/11/6)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 M72X_Proc_ShutDown(void* content)
{
    //������֡����,ֻҪ��һ֡�ظ�OK���㲽�����.
    
//	    SYS_MODM_SendCmd("AT+GTSIMRST", 11, 0);//ִ��AT+GTSIMRST ��λSIM��
//	    sleep_ms(100);                     //���н���
  
    SYS_MODM_SendCmd("AT+CFUN=1,1", 11, 0);//Hardware reset.
    sleep_ms(50);                     //���н���
    
    SYS_MODM_SendCmd("AT+QPOWD=1", 10, 0);//G610-A50ִ��AT+CPWROFF ����ȥ�ػ�
    sleep_ms(50);                     //���н���
    
//	    SYS_MODM_SendCmd("AT+MRST", 7, 0);    //G610-A20ִ��AT+MRST ����ȥ�ػ�

    SYS_MODM_StartCnt(2000);                //������ʱ������
	guc_ModemElement++;
    return 0;
}


const ModemElement gs_M72XReset[] =
{
	{"_shutdown before reset", AT_OK, 50, 1, (CallBackElement)M72X_Proc_ShutDown},
    {(string)0, AT_OK, 0, 0, (CallBackElement)0},
};

#define  gs_M72XShutDown   gs_M72XReset




const ModemElement gs_M72XInfo[] =
{
    
    {"ATE1",        AT_OK,      500, 0, (CallBackElement)0},
    {"AT+CGSN",     AT_OK,      500, 0, (CallBackElement)0},      //��ȡģ��IMEI
    {"_dumb wait",  M72X_IMEI,  200, 1, (CallBackElement)M72X_Proc_DumbWait},
    {"AT+QCCID",    AT_OK,      600, 0, (CallBackElement)0},      //��ȡSIM��CCID
    {"_dumb wait",  M72X_CCID,  200, 1, (CallBackElement)M72X_Proc_DumbWait},
    {"ATE0", AT_OK, 200, 0, (CallBackElement)0},    
    {"AT+CGMR",     AT_OK,      500, 0, (CallBackElement)0},      //��ȡģ��汾��Ϣ
    {"_dumb wait",  M72X_CGMR,  200, 1, (CallBackElement)M72X_Proc_DumbWait},
    {(string)0, AT_OK, 0, 0, (CallBackElement)0},
};


//�Ǳ�׼ATָ��輯��
const ModemElement* const gs_M72X[] =
{
    gs_M72XPPPoE,                       //Modem_PPPoE,
    gs_M72XDiscPPP,                     //Modem_PPPDiscnt,
    gs_M72XTcpCnt,                      //Modem_TcpCnt,
    gs_M72XUdpCnt,                      //Modem_UdpCnt,
    gs_M72XDisTcp,                      //Modem_TcpDiscnt,
    gs_M72XDisUdp,                      //Modem_UdpDiscnt,
    gs_M72XTcpSend,                     //Modem_TcpSend,
    gs_M72XUdpSend,                     //Modem_UdpSend,
    gs_M72XIPStt,                       //Modem_IPState,
    gs_M72XReset,                       //Modem_Reset,
    gs_M72XShutDown,                    //Modem_ShutDown,
    (void*)0,                           //Modem_RecvIPData, //M72X�������ݾ������ϱ�,�޶�ȡ����
    gs_M72XInfo,                        //Modem_Info
    gs_M72XInit,                        //Modem_Spec_Init

};




/************************************************************************
 * @function: SYS_M72X_ProcRecvAtCommand
 * @����: ģ�����Ԥ����õĽ���ָ��,�ڲ���������
 * 
 * @����: 
 * @param: mrfb 
 * @����: 
 * @˵��: �� SYS_TASK_MODM �����е���
 * @����: yzy (2013/11/1)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void SYS_M72X_ProcRecvAtCommand(ModemRecvFeedBack* mrfb)
{
	ModemElement* me;
	
	DevModem->stt.bit.state_wait = 0;	//�κβ����Ŀ�ʼ������Ҫ�ȴ�
	
	if(gs_ModemStep >= NUM_ALL_MODEMSTEP)
	{
		DevModem->stt.bit.state_mid = 0;
	}
	                                    //��ȡ��ǰ�Ľṹָ��
    if(gs_ModemStep >= NUM_STD_MODEMSTEP)
    {
        me = (ModemElement*)gs_M72X[gs_ModemStep - NUM_STD_MODEMSTEP];
    }
    else
    {
//#if (Modem_M72X_SMS_EN > 0)     
        me = (ModemElement*)gs_STDAT[gs_ModemStep];
//#endif
    }
    
    if(me == (void*)0)			        //����ò�����֧��
    {
        return;
    }
    
	me += guc_ModemElement;
	
    switch(mrfb->recv)
    {
        case M72X_READY:                //�����ϱ�:ģ���״�׼������ʱ�ϱ�.
            SYS_MODM_Step(Modem_Init);
            break;

        case AT_CMTI:                 //�����ϱ�:����Ϣ����
            SYS_MODM_Step(Modem_ListMsg);
            break;

            //M72Xϵ�в�Ʒ,���յ����ݺ�ֱ�ӽ�����������unsolicited���������ϱ�
            //�������ϱ��������ݻ�����Ϣ,Ȼ�������û�ȥ��ȡ.
        case M72X_MIPRTCP:              //�����ϱ�:��IP����
        case M72X_MIPRUDP:
        case M72X_RECEIVE:
            //SYS_MODM_Step(Modem_RecvIPData);
            if(DevModem->ipRev->len > 0)
            {                           //֪ͨӦ�ò��ȡ�����е�IP����
                DevModem->stt.bit.ipRevData = 1;
            }
//	            if((DevModem->gsRev[0]->len > 0) || (DevModem->gsRev[1]->len > 0))
//	            {
//	                DevModem->stt.bit.gsip = 1;
//	            }
            break;

        case AT_RING:
            if(guc_M72XRingCount == 0)
            {
                guc_M72XRingCount++;
            }
            else
            {
                guc_M72XRingCount = 0;
                DevModem->stt.bit.ringRev = 1;
                SYS_MODM_Step(Modem_HoldUp);
            }
            break;

        case AT_ERROR:
            if(gs_ModemStep == Modem_PPPDiscnt)
            {
                DevModem->stt.bit.pppoe = 0;
                DevModem->stt.bit.ipcc = 0;
                DevModem->stt.bit.tcpcc = 0;
                DevModem->stt.bit.udpcc = 0;
            }
            SYS_MODM_EndCnt();		//������ʱ�������ļ�ʱ	
            SYS_MODM_Error();		//�յ�ERROR֮��Ĵ���
            break;

        case AT_CME:
            SYS_MODM_EndCnt();		//������ʱ�������ļ�ʱ	
            SYS_MODM_Error();		//�յ�ERROR֮��Ĵ���
            break;
            			
        default:
            if(mrfb->recv == me->recv)
            {
                SYS_MODM_EndCnt();	//������ʱ�������ļ�ʱ	
                if(me->flag & 0x01)
                {
                    if(me->cbe(mrfb->proced) != 0)
                    {
                        SYS_MODM_Error();
                        break;      //����swtich,case
                    }
                    
                    if(me->send == (string)0)
                    {
                        SYS_MODM_Finish();
                    }
                }
                else if(me->send != (string)0)
                {
                                    //���ͺ���Ԫ�ص�����	
                    SYS_MODM_SendCmd((uint8*)me->send, strlen(me->send), me->overtime);
                                    //�Ӳ�������
                    guc_ModemElement++;
                }
                else				//����ͱ�ʾ�Ѿ�˳����������еĲ���
                {
                    DevModem->stt.bit.state_mid = 0;
                    guc_ModemElement = 0;
                                    //�����ɹ���Ĵ���
                    SYS_MODM_Finish();
                }
            }
            else
            {
                //�˴�����������.
                //1.ֻҪ�ڲ����ж�����Ҫ�ȴ��Ļظ�֡,�ڵȴ��ڼ��յ�����֡,���б��Ƿ�Ϊ��Ҫ����������ϱ�֡.
                //�ԼȲ��������ϱ�֡����,�ֲ��ǲ�����Ҫ��֡.ֱ������,�������ȴ�.
                //
            }
    }

}



/************************************************************************
 * @function: SYS_M72X_ProcUnsolictData
 * @����: ģ�������ϸ����ݵĴ���
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/11/17)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void SYS_M72X_ProcUnsolictData(ModemRecvFeedBack* mrfb)
{
    switch(mrfb->recv)
    {
        case M72X_READY:                //�����ϱ�:ģ���״�׼������ʱ�ϱ�.
            SYS_MODM_Step(Modem_Init);
            break;

        case AT_CMTI:                 //�����ϱ�:����Ϣ����
            SYS_MODM_Step(Modem_ListMsg);
            break;

        case M72X_MIPRTCP:			    //�����ϱ�:��IP��������
        case M72X_MIPRUDP:
        case M72X_RECEIVE:
            //SYS_MODM_Step(Modem_RecvIPData);
            if(DevModem->ipRev->len > 0)
            {                           //֪ͨӦ�ò��ȡ�����е�IP����
                DevModem->stt.bit.ipRevData = 1;
            }
//	            if((DevModem->gsRev[0]->len > 0) || (DevModem->gsRev[1]->len > 0))
//	            {
//	                DevModem->stt.bit.gsip = 1;
//	            }
            break;

        case AT_RING:
            if(guc_M72XRingCount == 0)
            {
                guc_M72XRingCount++;
            }
            else
            {
                guc_M72XRingCount = 0;
                DevModem->stt.bit.ringRev = 1;
                SYS_MODM_Step(Modem_HoldUp);
            }
            break;
    }
}


/************************************************************************
 * @function: SYS_M72X_ModemMainLoop
 * @����: ѭ��
 * 
 * @����: 
 * @param: msc 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/11/17)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void SYS_M72X_ModemMainLoop(ModemStepCommand* msc)
{
    ModemElement* me;
                                        //ɢת�Ϸ��Ե��ж�
    if(msc->command >= NUM_ALL_MODEMSTEP)
    {
        return;			            //�Ƿ���ֱ�ӷ���
    }
                                        //��ȡ��ǰ�����������
    if(msc->command >= NUM_STD_MODEMSTEP)
    {
        me = (ModemElement*)gs_M72X[msc->command - NUM_STD_MODEMSTEP];
    }
    else
    {
//#if (Modem_M72X_SMS_EN > 0)     
        me = (ModemElement*)gs_STDAT[msc->command];
//#endif
    }		
    
    if(me == (void*)0)			        //����ò�����֧��
    {
        return;
    }
    
    gs_ModemStep = msc->command;	    //��ǰ����
	
    DevModem->stt.bit.state_wait = 0;   //�κβ����Ŀ�ʼ������Ҫ�ȴ�

    if(me->flag & 0x01)			        //�ж��Ƿ���Ҫ���⴦��
    {
        switch(me->cbe(msc->msg))	    //�������⴦��֮���ٽ��к�������
        {
            case 0:
                DevModem->stt.bit.state_mid = 1; 
                guc_ModemElement = 1;	//Ԫ�ز�������
                break;
            case 1:
                SYS_MODM_Error();	    //�������
                break;
        }
    }
    else				                //����ֱ�ӷ�������
    {
        SYS_MODM_SendCmd((uint8*)me->send, strlen(me->send), me->overtime);
        DevModem->stt.bit.state_mid = 1;//��ǰת�벻�ɴ��״̬
        guc_ModemElement = 1;		    //Ԫ�ز�������
    }
}






