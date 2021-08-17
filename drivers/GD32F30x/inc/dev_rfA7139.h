#ifndef _MY_7108_
#define _MY_7108_

#ifndef EXT_RF
#define EXT_RF extern
#endif



/*******************************************************************************
**��ʾ����è������
********************************************************************************/
typedef struct{
    int (*check)(void);                //ȷ���Ƿ�Ϊ�����͵ĺ���
    void (*on)(void);                   //��ģ��Ĺ���
    void (*off)(void);                  //�ر�ģ��Ĺ���
//	    int (*set_name)(char *, uint8_t);                //ȷ���Ƿ�Ϊ�����͵ĺ���
//	    int (*sta_config)(char *ap, char * pwd);
//	    int (*sta_config_chk)(char *ap, char * pwd);    
//	    int (*sta_check_link)(char * ssid);
    
    int (*sta_link_close)(char *ap);
    int (*sta_scan)(char * ssids);
    int (*status_check)(uint16_t * csq);
    int (*mdl_reset)(void);
    int (*tcp_connect)(uint8_t type, char *ip, uint16_t port);
    int (*tcp_close)(void);
    int (*tcp_check_lnk)(uint8_t);    
    
    int (*sta_link_ipcfg)(uint8_t * ip);
}TRFModemDrive;


typedef union{
    uint32_t lword;
    struct{
        uint32 typeChecked:1;       //�Ƿ���ʶ��ģ������
        uint32 chnrdy:1;            //ATָ���Ƿ���ɻ������ò�׼������
        uint32 linked:1;
        uint32 signl:12;             //�ź�����(0-31)
        uint32 regtt:1;             //�Ƿ��Ѿ�ע��ɹ�
        uint32 sninfo:1;            //�Ƿ��Ѿ���ȡ��ģ��Ϳ���Ϣ
        
        uint32 pppoe:1;             //PPP�����Ƿ��Ѿ���
        //uint32 ipcc:1;              //IP�����Ƿ��Ѿ�����(����TCP��UDP)
        uint32 tcpcc:1;             //TCP�����Ƿ��Ѿ�����
        //uint32 udpcc:1;             //UDP�����Ƿ��Ѿ�����
        uint32 gsstt:1;             //GPRS server(tcp or udp) �����Ƿ���
        uint32 gsclncc1:1;           //GPRS server(tcp or udp) �Ƿ� �� �ͻ���1 �Ľ�������
        uint32 gsclncc2:1;           //GPRS server(tcp or udp) �Ƿ� �� �ͻ���2 �Ľ�������
//        uint32 gsclnno:4;        //�������ӹ����Ŀͻ��˵����Ӻţ���ǰ�ģ�
//        uint32 gsclnoldno:4;        //�������ӹ����Ŀͻ��˵����Ӻţ���ȡ���ģ�
//        uint32 gsurcreadno:4;           //�����ϱ������ݣ����ڵ�socket ID
        uint32 gsip:1;              //GPRS server�յ� �ͻ��� ��IP����
        uint32 gsipdex:1;           //�Ƿ����GPRS server�յ� �ͻ��� ��IP���� δ���ϲ㴦��
        uint32 ipdex:1;             //�Ƿ����IP����δ���ϲ㴦��
        uint32 smsdex:1;            //���Ž��ջ���������Ч��־
        uint32 ringRev:1;           //�Ƿ���������ź�,δ���ϲ㴦�� 
        
        uint32 ipRevData:1;         //��ʾ��Ҫ֪ͨӦ�ò㴦�����е�IP����.��Ϣ���ͺ���������.
//	        uint32 smRevData:1;         //��ʾ��Ҫ֪ͨӦ�ò㴦�����е�SMS����.��Ϣ���ͺ���������.
//	        uint32 smWait:1;            //�ȴ����Ž�����ȫ(��ȡ�������Ƿ����лظ���)
//	        
//	        uint32 smsBufSem:1;         //���Ž��ջ�����ź���.
//	        uint32 smsind:1;            //�Ƿ��յ������ϱ�,��δȥ��ȡ����
//	        uint32 smsend:1;            //���ŷ����Ƿ�ɹ�
        //uint32 gsNewL:1;            //������
        uint32 state_mid:1;			//��ʾ�Ƿ����м䲽��,���Ƿ�������
        uint32 state_wait:1;		//�Ƿ��ڵȴ�'>'���緢�Ͷ��Ż�IP����ʱ�ȴ��ظ�        
    }bit;
} TRFModemState;


typedef struct __TRFModem{
//    TDev dev;
    const TRFModemDrive* drive;

    TRFModemState stt;
    

}TRFModem;

EXT_RF TRFModem* DevRfModem;             //����è����,״̬��Ϣ
/*!
 * RF packet definition
 */
#define RF_BUFFER_SIZE_MAX                          256
#define RF_BUFFER_SIZE                              256

/*!
 * SX1276 LoRa General parameters definition
 */
typedef struct sLoRaSettings
{
    unsigned int RFFrequency;
    char Power;
    unsigned char SignalBw;                   // LORA [0: 7.8 kHz, 1: 10.4 kHz, 2: 15.6 kHz, 3: 20.8 kHz, 4: 31.2 kHz,
                                        // 5: 41.6 kHz, 6: 62.5 kHz, 7: 125 kHz, 8: 250 kHz, 9: 500 kHz, other: Reserved]  
    unsigned char SpreadingFactor;            // LORA [6: 64, 7: 128, 8: 256, 9: 512, 10: 1024, 11: 2048, 12: 4096  chips]
    unsigned char ErrorCoding;                // LORA [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8]
    bool CrcOn;                         // [0: OFF, 1: ON]
    bool ImplicitHeaderOn;              // [0: OFF, 1: ON]
    bool RxSingleOn;                    // [0: Continuous, 1 Single]
    bool FreqHopOn;                     // [0: OFF, 1: ON]
    unsigned char HopPeriod;                  // Hops every frequency hopping period symbols
    unsigned int TxPacketTimeout;
    unsigned int RxPacketTimeout;
    unsigned char PayloadLength;
    unsigned char LowDatarate;
    unsigned short preamble;
    unsigned char hopFreqChNum;               //��ƵƵ������
    
}tLoRaSettings;

//	#include "main.h"
/*********************************************************************
**  function Declaration
*********************************************************************/

void  INIT_MCU_RF_MAP(void);

void SYS_A7139_Proc(uint8_t mod);
uint8_t SYS_RF_Init(int freqCode, unsigned char ch, unsigned char pwr );
uint8_t SYS_A7139_Send(uint8_t * data, uint16_t len);
uint16_t SYS_A7139_Recv(uint8_t * data);


///*!
// * Radio driver structure defining the different function pointers
// */
//typedef struct sRadioDriver
//{
//    unsigned char ( *Init )(int freqCode, unsigned char ch, unsigned char pwr );
//    void ( *Reset )( void );
//    void ( *StartRx )( void );
//    uint16_t ( *GetRxPacket )( void *buffer);//, unsigned short *size );
//    uint8_t ( *SetTxPacket )( const void *buffer, unsigned short size );
//    unsigned int ( *Process )( void );
//    void (*Tick)(unsigned int * tick);
//}tRadioDriver;




TRFModemState SYS_RF_Status(void);
double SX1276LoRaGetPacketRssi( void );

void SYS_RF_Set_FallingEdge(uint8_t gpio);
tRadioDriver* RadioDriverInit( void );

#endif
