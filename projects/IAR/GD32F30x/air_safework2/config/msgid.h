/****************************************Copyright (c)**************************************************
**                                  ________�Ƽ����޹�˾
**                                          ������
**
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: msgid.h
**��   ��   ��: yzy
**����޸�����: 2013��2��25��
**��        ��: ����б�ͷ�ļ�����
**ע        ��:
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ��  ��: yzy
** ��  ��: v1.0
** �ա���: 2013��2��25��
** �衡��: ԭʼ�汾
********************************************************************************************************/




///////////////////////////////////////////////////////////////////
//   
// ��ʱ�����
//
/////////////////////////////////////////////////////////////////
#define TIMER_ID_RTC     0                   //��ʱTIMER���
#define TIMER_ID_MSG     1
#define TIMER_ID_WDT     2
#define TIMER_ID_LGPI    3
#define TIMER_ID_FGPI    4
#define TIMER_ID_LED     5
#define TIMER_ID_UART    6
#define TIMER_ID_AMI     7



///////////////////////////////////////////////////////////////////
//   
// Ӳ��ʱ�����(ƥ��ͨ��)
//
/////////////////////////////////////////////////////////////////
#define TIME3_ID_UART2_SOT  0
#define TIME3_ID_UART3_SOT  1
#define TIME3_ID_UART4_SOT  2
#define TIME3_ID_UART9_SOT  3




///////////////////////////////////////////////////////////////////
//   
// ��Ϣ��Ŷ���
//
/////////////////////////////////////////////////////////////////
//===============ϵͳ����Ϣ(0-30,�����̲����ظ�����)=======================================
#define MSG_YEAR        0                   //����Ϣ//ʱ����ϢӦ������Ϊ���ȼ����
#define MSG_MON         1                   //����Ϣ
#define MSG_DAY         2                   //����Ϣ
#define MSG_HOUR        3                   //Сʱ��Ϣ
#define MSG_MIN         4                   //����Ϣ
#define MSG_SEC         5                   //����Ϣ


#define MSG_LIVE        6                   //������Ϣ
#define MSG_UART0       7                   //�߼�����0�յ���Ϣ UART_ENUM
#define MSG_UART1       8                   //�߼�����1�յ���Ϣ
#define MSG_UART2       9
#define MSG_UART3       10
#define MSG_UART4       11
#define MSG_UART5       12
#define MSG_UART9       13

#define MSG_ENTER_LP    14                  //����͹���

#define MSG_LIFEVT      16                  //���ٿ��½���
#define MSG_LIREVT      17                  //���ٿ�������
#define MSG_LILEVT      18                  //���ٿڱ���
#define MSG_FIREVT      19                  //����������
#define MSG_FIFEVT      20                  //�����½���

#define MSG_FARP_RECVDATA           21         //��ʾ�յ�����ETHͨ��������
#define MSG_FROMCLNDATA             22          //��ʾtcp server�յ����Կͻ���ͨ��1������
#define MSG_IP1DATA                 23          //��ʾ�յ�GPRSͨ��1 ������
#define MSG_SMRECV                  24          //��������
#define MSG_RING         25             //�յ���������

//===============(�����谴ʵ�ʴ��ڱ�Ŵ�С��������)=======================================
#define MSG_PSTP         MSG_UART1      
#define MSG_MDMP        (MSG_UART0 + 1)
#define MSG_ESAM        (MSG_UART0 + 2)           //�� SYS_PST_PORT ��Ӧ
#define MSG_CARD        (MSG_UART0 + 3) 
#define MSG_NETP        (MSG_UART0 + 4)

//===============Ӧ�ü���Ϣ(30-63,�����̿��ظ�����)=======================================
//======================================================
#define MSG_HLV_BATCTL      30
#define MSG_HLV_VAR         31
#define MSG_HLV_CLR      32

#define MSG_MAIN_RESET          35      //MAIN������λ

#define MSG_MAIN_UPDATA_CHK     40      //�����ļ�У��
#define MSG_MAIN_HARD_CHK       41      //Ӳ���Լ�
#define MSG_MAIN_BLE_CHK       42      //Ӳ���Լ�
#define MSG_MAIN_RFID_CHK       42      //Ӳ���Լ�
#define MSG_MAIN_ADDR_CHG       43      //��ַ���

//	#define MSG_FARP_PRIOCNT        51
//	#define MSG_FARP_DISGPRS        52  //���ȼ������MSG_FARP_CNTGPRS
//	#define MSG_FARP_CNTGPRS        53
//	#define MSG_FARP_RECVVALIDIP    54
//	#define MSG_FARP_DISETH         55  //���ȼ������MSG_FARP_CNTETH
//	#define MSG_FARP_CNTETH         56
//	#define MSG_FARP_VAR            57
//	#define MSG_FARP_ETHINIT        58
//	#define MSG_FARP_PARACHG        61      //�����������
//	#define MSG_FARP_ETHCHG         62      //��̫�����
//	#define MSG_FARP_MACCHG         63      //��̫��mac����
//	#define MSG_FARP_ETHRST         MSG_FARP_ETHCHG//��߸�λ��̫��ģ�鹦��



#define MSG_FARP_PRIOCNT        44

#define MSG_FARP_DISPPP         45  //PPP����
#define MSG_FARP_CNTPPP         46  //�Ͽ�PPP����
#define MSG_FARP_DISSERVER      47  //�Ͽ�TCP SERVER ����
#define MSG_FARP_CNTSERVER      48  //����TCP SERVER ����

#define MSG_FARP_DISFROMCLN1     49  //�Ͽ��յ��Ŀͻ�������1
#define MSG_FARP_DISFROMCLN2     50  //�Ͽ��յ��Ŀͻ�������2

#define MSG_FARP_DISGPRS        51  //���ȼ������MSG_FARP_CNTGPRS
#define MSG_FARP_CNTGPRS        52
#define MSG_FARP_RECVVALIDIP    53
#define MSG_FARP_RECVVALIDSER   54
//	#define MSG_FARP_DISETH         55  //���ȼ������MSG_FARP_CNTETH
//	#define MSG_FARP_CNTETH         56

#define MSG_FARP_VAR            57
#define MSG_FARPE_FCNT          60      //����ʧ����Ϣ

#define MSG_FARP_PARACHG        61      //�����������
#define MSG_FARP_CHECK          62      //ģ���Լ�
#define MSG_MODEM_TIMEOUT       64
#define MSG_MODEM_ABORT         65

#define MSG_CARD_INSERT         70

#define MSG_NETP_IDATA     80              //��ʼ��
#define MSG_NETP_CURDATA   81             //����������
#define MSG_NETP_BCURDATA  82             //����������
#define MSG_NETP_DAYDATA   83             //���ն�������
#define MSG_NETP_MONDATA   84             //���¶�������
#define MSG_NETP_EVNDATA   85             //���¼�

#define MSG_NETP_CKTIME    86             //���¹ұ�Уʱ
#define MSG_NETP_FEE       87             //���¹ұ���װ����

#define MSG_NETP_MT_UPDATA_START    88      //���¹ұ��������ʼ
#define MSG_NETP_MT_UPDATA_INIT     89      //�Ա�������ʼ��
#define MSG_NETP_MT_UPDATA_DATAING  90      //�����ļ���
#define MSG_NETP_MT_UPDATA_CHECK    91      //У���ļ���
#define MSG_NETP_MT_UPDATA_ACTION   92      //ִ������
#define MSG_NETP_MT_READ_VERSION    93      //����汾
#define MSG_NETP_FINISH             94      //���������Ϣ
#define MSG_NETP_TASK_RUN_FLAG_PER_485    95      //����485������Ϣ

#define MSG_NETP_CHECK      96


#define MSG_HLV_INP     100              //�ڲ�ģ����ز�������
#define MSG_HLV_FARP    101              //Զ��ģ����ز�������
#define MSG_HLV_PST     102              //ͨѶģ����ز�������
#define MSG_HLV_TASK    103              //����ģ����ز�������
#define MSG_PST_VAR     104      //pst���̲���������Ϣ
#define MSG_ALRT_ERC1           105      //���������ݳ�ʼ�� erc1
#define MSG_ALRT_CLR            106      //������м�¼

#define MSG_SWITCH_CHANGE   110
#define MSG_EVENT_CHANGE    111
#define MSG_ENTER_SLEEP     112
#define MSG_WAKE_UP         113  
//	#define MSG_FTP_OPEN            110
//	#define MSG_FTP_OPEN_SUCC       111
//	#define MSG_FTP_OPEN_FAILED     112
//	#define MSG_FTP_GET             113
//	#define MSG_FTP_GET_SUCC        114
//	#define MSG_FTP_GET_FAILED      115
//	#define MSG_FTP_GET_FNS         116
//	#define MSG_FTP_REPORT_RESULT   117
//	#define MSG_FTP_CLOSE           118
//	#define MSG_FTP_PROJ_FAILED         119

#ifndef EXT_MSGID
#define EXT_MSGID extern 
#endif

EXT_MSGID const uint8_t msgidA[];
