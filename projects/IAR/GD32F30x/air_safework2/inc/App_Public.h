#ifndef _APP_STRUCT_H_
#define _APP_STRUCT_H_

/***************define***************************/
#define METER_ADDRESS_LENGTH_MAX 16
#define READ_METER_NUM_MAX  25

#define OFFSET_UP_RAREA  4
#define OFFSET_UP_CTRL  3
#define OFFSET_UP_LEN   1
#define OFFSET_UP_AFN_BASIC 10

#define UPLINK_TIMEOUT   120 // 240*25ms ,6S
#define UPLINK_DATA_TIMEOUT 512 //���еķ񶨻ش�ʱ��
#define UART_BYTE_TIMEOUT   40 // 20*25ms 0.5
#define UART_PKT_TIMEOUT   	 240 // 6
///////eeprom
#define UPGRADE_FRAME_LIMIT         192
#define EEPROM_UPDATE_PARA      0x400

#define DP_DOWNLOAD_BITMAP_ADDR (EEPROM_UPDATE_PARA + 32)
#define	CON_DOWNLOADBITMAP_SIZE		128

#define DP_DOWNLOAD_PROC_PARA_ADDR   (DP_DOWNLOAD_BITMAP_ADDR + CON_DOWNLOADBITMAP_SIZE)
#define DP_DOWNLOAD_PROC_LEN    (sizeof(STUPGRADE))


#define FM_NEED_OPEN_SS_VALIDATE 0xDD9 	//�ӽڵ�����ע���־

#define FM_BROAD_METER_ENABLE 0xDDA 	//�ӽڵ�����ע���־
#define FM_BROAD_METER_TIME 0xDDB 	//�ӽڵ�����ע�����ʱ��

#define FM_CHANNEL_SET_MODE 0xDDD 	//�ŵ�����ģʽ
#define FM_FREQ_CHANNEL_NUM 0xDDE 	//Ƶ�κ�
#define FM_NEED_RESET_CMD 0xDDF 	//��������Ҫ����λ����ı�־

#define FM_I_SS_CODEUPDATA_ENABLE 0xDE0 	//I�Ͳɼ�����������ʹ�ܱ�־λ
#define FM_DEVICE_ADDR_LEN 0xDE1 	//�ز����ڵ��ַ(6���ֽ�)


#define FM_BS_INFO 0xDF0 	//����ģ��ĳ������
#define FM_NETPARA_ADDR 0xE00 //���������ַ  (ԭ0x01FF )
#define FM_CLTP_ADDR       0xE10 //·�ɲ������ɼ���������ַ   (ԭ0x0205  )
#define FM_HASH_USED 0xE20  //��ϣ��ʾ�Ƿ�ʹ�ñ�ʶλ��ַ
#define FM_CAP_MEND 0xE21  //����У׼ֵ��ַ
#define FM_RESET_MESAGE 0xE22  // ����ģ�鸴λ��Ϣֵ��¼��ַ
#define FM_SS_CODEUPDATA_ENABLE  0xE23  // �ɼ�������ʹ�ܱ�־λ
#define FM_METER_DEAD_COUNT  0xE24  // ����ϻ�����

#define FM_TUOPU_SOURCE  0xE25  // ������ʾ�Ľڵ�������Դ
#define FM_WRITE_HASH_USED  0xE26  // ������ʹ�ñ�־
#define FM_BLACK_HASH_USED  0xE27  // ������ʹ�ñ�־

#define FM_SS_VALIDATE_ENABLE  0xE28  // �ɼ�����֤ʹ�ܲ���
#define FM_METER_INFO_SYN_ENABLE  0xE29  // ������ͬ��ʹ�ܲ���

#define FM_FREQ_DEFINE 0xE30 //Ƶ�㶨�� 
#define FM_CHANNEL_PARA 0xE40 //�ŵ�����
#define FM_GPS_PARA 0xE50 //GPS
#define FM_DEVICE_ADDR 0xE60 	//�ز����ڵ��ַ(16���ֽ� ascii��)
/////////////////////////////////////////////////////////////RF

enum  //��
{
	PKT_TYPE_DATA,   //0
   	PKT_TYPE_SS_DATA,
   	PKT_TYPE_SS_SEARCH_METER,
   	PKT_TYPE_TOPO,
	PKT_TYPE_SS_NET_SET,
	PKT_TYPE_HD_TEST,	
	PKT_TYPE_DATA_802,   //0
};

enum  //��
{
	TX_STATE_LBT_LISTEN,   //0
   	TX_STATE_WAIT_FOR_RTS,
   	TX_STATE_WAIT_FOR_CTS,
   	TX_STATE_WAIT_FOR_TX,
   	TX_STATE_WAIT_FOR_TX_MORE,
   	TX_STATE_WAIT_FOR_ACK,
   	TX_ERROR_NO_ACK,
   	TX_ERROR_CHANNEL_BUSY,
   	TX_ERROR_STATE
};

enum  //��
{
	RX_STATE_FREQUENCY_SEARCH,
	RX_STATE_WAIT_FOR_SYNC,
	RX_STATE_WAIT_FOR_HEADERS,
	RX_STATE_WAIT_FOR_RTS,
	RX_STATE_WAIT_FOR_CTS,
	RX_STATE_WAIT_FOR_NACK,
	RX_STATE_WAIT_FOR_PACKET,
	RX_STATE_WAIT_FOR_SEND_ACK,
	RX_ERROR_STATE
};
#define LIGHT_ON        1
#define LIGHT_OFF       0

#define LIGHT_SLOW      1
#define LIGHT_FAST      0

#define LIGHT_OFF_VALUE 0
#define LIGHT_ON_VALUE  0x3FF


#define UN_HANDLED 0x1
#define HANDLED 0x0


//·�ɿ�����F1����ز��ӽڵ�

#define MAX_SLOT_TIME  254

#define SUP_SS_INDEX_START  2
#ifdef MASTER_NODE
#define MAX_SUP_SS_NUM  (8+1)
#else
#define MAX_SUP_SS_NUM  (3+1)
#endif

//#define MAX_SS_UPDATA_BLOCK_NUM  255
#define MAX_I_SS_UPDATA_BLOCK_NUM  0xFFFF
#define MAX_SS_DEAD_TIME  60000

#define MAX_MASTER_NODE_TO_LED_CLOSE_COUNT  600 //100��������
//#define FRQ_BRODCAST_CHAN 0x02// for Net3

//#define FRQ_BRODCAST_CHAN 0x0
//#define DATAPKT_PAYLOAD_OFFSET 6
#define MIN_TIME_LIVE 40
//#define MAX_TIME_LIVE 120
#define MAX_TIME_LIVE 100
#define TIME_OUT_MAX    7//7��·�ɣ�7�����ʱ
#define MAX_DATA_TIME_LIVE 255
//#define MAX_DATA_TIME_LIVE 120

#define MAX_NUM_COL 256
#ifdef MASTER_NODE
#define MAX_HASH_TABLE_FLASH_SIZE 8//1024//(MAX_SUP_SS_NUM)
#else
#define MAX_HASH_TABLE_FLASH_SIZE 3//1024//(MAX_SUP_SS_NUM)
#endif
#define MAX_HASH_TABLE_CACHE_SIZE 32
#define MAX_HASH_TABLE_SECTOR_SIZE 8
#define MAX_HASH_TABLE_WB_SIZE 16//white--black
#define BLOCK_NO (MAX_HASH_TABLE_FLASH_SIZE/MAX_HASH_TABLE_SECTOR_SIZE)
#define MAC_OK 1
#define TRANSFER_BUFFER_SIZE    512


#define MAX_VALIDATE_SS_NUM 512
#define MAX_SS_MAP_SIZE 128
#define MAX_SS_MAP_SECTOR_SIZE 8
#define WB_BLOCK_NO (MAX_HASH_TABLE_FLASH_SIZE/MAX_HASH_TABLE_WB_SIZE)

#define MAX_SUP_SLOT_NUM 32 //ҵ��ʱ϶�ĸ���
#define MASTER_ROUTE_PATH 0
#define BACKUP_ROUTE_PATH 1
#define ERROR_ROUTE_PATH 2

#define HEART_BEAT_PKT 1
#define JOIN_NET_PEK 0

#define UNIQUE_MAC_ADDR_LEN 6
#define MAX_NUM_DOWN_SENDPKT 16 

#define BEHAVIOR_BROADCAST      0
#define BEHAVIOR_RECV           1 
#define BEHAVIOR_SEND           2
#define BEHAVIOR_LISTEN         3
#define BEHAVIOR_SLEEP          4
#define BEHAVIOR_SHARE_TX       5
#define BEHAVIOR_RECV_ADV       6
#define BEHAVIOR_COMPETITION    7
#define BEHAVIOR_BROADALLOWLOGIN      8
#define BEHAVIOR_RESET      9

#define MAX_ROUTE_DEPTH  0xf 
#define FULLTIME 1600   // 40S �㲥��Ϣ
//#define BROAD_METER_SEARCH_TIME 4800   // 2minutes ���ҵ��㲥��Ϣ
#define BROAD_METER_SEARCH_TIME 72000   // 30minutes ���ҵ��㲥��Ϣ
//#define BROAD_METER_SEARCH_TIME 24000   // 10minutes ���ҵ��㲥��Ϣ

#define METER_DEAD_COUNT 30  //����ϻ��Ĵ���

#define LBT_FIXED_TIME_4000US 4
#define LBT_FIXED_TIME_3500US 4
#define LBT_FIXED_TIME_1000US 1

//#define RTS_TX_FIXED_TIME 7
#define RTS_TX_FIXED_TIME 10000
#define RTS_RX_FIXED_TIME 10000

#define CTS_TX_FIXED_TIME 7000
#define CTS_RX_FIXED_TIME 10000

#define ACK_TX_FIXED_TIME 7000
#define ACK_RX_FIXED_TIME 10000

//#define SYNC_RX_FIXED_TIME 12


#if DISTANCE_TEST
#define SYNC_RX_FIXED_TIME 16000
#else
#define SYNC_RX_FIXED_TIME 12000
#endif


#define ADV_TX_FIXED_TIME 18000

#define ADV_RX_FIXED_TIME 19000

#define MAX_PKT_RX_FIXED_TIME 60000

#define SUBTYPE_TRANS 0X00 //͸��ת��
#define SUBTYPE_97 0X01 //97��Լ
#define SUBTYPE_07 0X02 //07��Լ

//#define FM_SS_INFO 0x400 	//�ھӱ���Ϣ��δʹ��


#define OVER_TIME 0 //ͨ�ų�ʱ
#define INVALID_DATA 1 //��Ч���ݵ�Ԫ
#define WRONG_LEN 2 //���ȴ�
#define WRONG_CHECK 3 //У���
#define REREA_CHECK 4 //��Ϣ�಻����
#define WRONG_FORM 5 //��ʽ����
#define OVERLAP_ADDR 6 //����ظ�
#define FIND_NO_ADDR 7 //��Ų�����
#define NO_ANSWER 8 //���Ӧ�ò���Ӧ��

#define PROGRAM_I_SS_BASE_VERSION_ADDR  9

#define PROGRAM_SS_TYPE_ADDR  9


#define FM_METER_INFO_BASE_ADDR 0x1000//�����ڵ���Ϣ�����ַ

#define UNLOCKED   0
#define LOCKED 1

#define LOCK(q)   do{(q).lock=LOCKED;} while(0)
#define UNLOCK(q)   do{(q).lock=UNLOCKED;} while(0)

#define SPILOCK(q)   do{(q) = LOCKED;} while(0)
#define SPIUNLOCK(q)   do{(q) =UNLOCKED;} while(0)



typedef enum
{
   PACKET_TYPE_DATA=0,
   PACKET_TYPE_ACK=0x1,
   PACKET_TYPE_NACK=0x2,
   PACKET_TYPE_SETLINK=0x3,
   PACKET_TYPE_JOIN_REQUEST=0x4,
   PACKET_TYPE_JOIN_RESPONSE=0x5,
   PACKET_TYPE_TOPO_UPDATE=0x6,
   PACKET_TYPE_BORADCAST=0x7,
   PACKET_TYPE_RTS=0x8,
   PACKET_TYPE_CTS=0x9,
   PACKET_TYPE_STATISTIC_REQUEST=0xA,
   PACKET_TYPE_STATISTIC_RESPONE=0xB
}PacketTypeTable;
//#define BROADCAST_FREQ  0


#define  PACKET_TYPE_2STEP  0x80
#define  TCR_TXDATA_READY   0x80
#define  TCR_BACKOFF_MASK   0x0F
#define  PACKET_TYPE_MASK   0xf0
#define  PACKET_CID_MASK   	0xF0
#define START_FLAG 0x68
#define END_FLAG 0x16

typedef enum
{
   SYSTEM_STATUS_STARTUP=0,
   SYSTEM_STATUS_SEARCH,
   SYSTEM_STATUS_WAIT_FOR_SYNC,
   SYSTEM_STATUS_SYNC,
   SYSTEM_STATUS_LOGIN
}SystemStatusTable;

typedef enum
{
   NODE_STATUS_OUT=0,
   NODE_STATUS_LOGIN
}NodeStatus;

//���߷������������ѡ��
typedef enum
{
   EZ_TX_TYPE_DATA =0,
   EZ_TX_TYPE_NET,
   EZ_TX_TYPE_HHU
}EZ_Cur_Tx_Type;
	

//���汾�����Ӱ汾:��ӵ��
//#define DECTECT_BEIJING


#define RS_RESET_TIME ((30 * 60 * 1000) / 25)

#define UPCOMM_FRAME_MAX_LEN 512
#define UART_DEBUG UART0_CHANNEL
#define CON_MUCHFRAME_SECTOR_NUM	2	/*��֡���ݴ洢�ռ�ռ��dataflash��������(ÿ����4k�ֽ�)*/
#define CON_MUCHFRAME_NUM			4

#define DOWN_UPGRADE_DIVISION_LEN   96
#define DOWN_UPGRADE_BITMAP_SIZE     96

#define CON_DEV_ADDR_LEN_6  6//��ַ����
#define CON_DEV_ADDR_LEN_8  8
#define CON_DEV_ADDR_LEN_10  8
#define CON_DEV_ADDR_LEN_16  16
#define QUEUE_MAX_LEN 4


#define EEPROM_START 0x00000000
#define EEPROM_END   0x00010000
#define FLASH_START 0x00000000
#define FLASH_END   FLASH1_SIZE
#define RAM_START 0x10000000
#define RAM_END   0x10007fff
#define RAM2_START 0x2007C000
#define RAM2_END   0x20083fff

#define IAP_ACTION_IDLE  0
#define IAP_ACTION_ERASE_HASH1  1
#define IAP_ACTION_ERASE_HASH2  2


#define SS_VALIDATE_TYPE_WHITE		1
#define SS_VALIDATE_TYPE_BLACK		0
//	typedef struct{
//	    uint8 routeFlg:1;
//	    uint8 reverse:1;
//	    uint8 moduleFlg:1;
//	    uint8 reverse1:1;
//	    uint8 routeNum:4;
//	}bit1;

#define FTRANS_ERR_OK       0               //���سɹ�
#define FTRANS_ERR_FT       1               //���س���
#define FTRANS_ERR_UP       2               //�������,�ɿ�ʼУ������

#define ROUTE_DOWN_LINK  0
#define ROUTE_UP_LINK  1
#define  MAX_DOWN_PKT_LEN 64

#define MAX_SLOT_NUM 1536
#define LITTLE_SLOT_NUM   24

#define CON_NODE_UPDATE_HEARTBEAT       0
#define CON_NODE_UPDATE_LOGIN           1
#define CON_NODE_UPDATE_PUSHUP          2
#define CON_NODE_UPDATE_DEL             3
#define CON_NODE_UPDATE_REGISTER        4
#define CON_NODE_REALY_DATA             5
#define CON_NODE_UPDATE_HEARTBEAT1      6
#define CON_NODE_DEFAULT                (CON_NODE_UPDATE_HEARTBEAT1 + 1)

#define CON_BEACON_CTRL_BIT_UP  0x01
#define CON_BEACON_CTRL_BIT_ROUTE  0x02
#define CON_BEACON_CTRL_BIT_MODE  0x04
#define CON_BEACON_CTRL_BIT_TIME  0x08
#define CON_BEACON_CTRL_BIT_UPGRADE  0x10
#define CON_BEACON_CTRL_BIT_RESET  0x20
#define CON_BEACON_CTRL_BIT_NETIN  0x40
#define CON_BEACON_CTRL_BIT_BEACON  0x80





//#define ROUTE_MODE 1 
#define AFN_TYPE_INFORM         0x0
#define AFN_TYPE_INITIALIZE     0x1
#define AFN_TYPE_DATARELAY      0x2
#define AFN_TYPE_DATAREQUES     0x3
#define AFN_TYPE_LINKDET        0x4
#define AFN_TYPE_CTLCMD         0x5
#define AFN_TYPE_REPORT         0x6
#define AFN_TYPE_ROUTE_POLL     0x10
#define AFN_TYPE_ROUTE_SET      0x11
#define AFN_TYPE_ROUTE_CTL      0x12
#define AFN_TYPE_ROUTEDATARELAY 0x13

#define AFN_TYPE_UPDATA         0X15
#define AFN_TYPE_STATISTIC_REQUEST 0x18
//#define AFN_TYPE_TOPO_REQUEST 0x19
#define AFN_TYPE_DEBUG 0xF0  //����ģʽ
//#define AFN_TYPE_METER_ADDR_REPORT 0x20
//#define AFN_TYPE_METER_ADDR_RECV_ACK 0x21

#define CHANNEL_CLEAR_STATE   1
#define CHANNEL_UNCLEAR_STATE 0



#define MSG_TYPE_CONFIRM        0x0
#define MSG_TYPE_CTRL           0x1
#define MSG_TYPE_RELAY          0x2
#define MSG_TYPE_UP             0x3
#define MSG_TYPE_BROADCAST      0x4
#define MSG_TYPE_QUERY          0x5
#define MSG_TYPE_ROUTE_MNG      0x6
#define MSG_TYPE_ROUTE_REQ      0x7
#define MSG_TYPE_LINK_DETECT    0x8
#define MSG_TYPE_FILE           0x9

//#define MSG_TYPE_MAX            0x0A
#define MSG_TYPE_FN_01       0x01
#define MSG_TYPE_FN_02       0x02
#define MSG_TYPE_FN_03       0x03
#define MSG_TYPE_FN_04       0x04
#define MSG_TYPE_FN_05       0x05
#define MSG_TYPE_FN_06       0x06
#define MSG_TYPE_FN_07       0x07
#define MSG_TYPE_FN_08       0x08
#define MSG_TYPE_FN_09       0x09
#define MSG_TYPE_FN_10       0x0A
#define MSG_TYPE_FN_11       0x0B
#define MSG_TYPE_FN_12       0x0C
#define MSG_TYPE_FN_13       0x0D
#define MSG_TYPE_FN_14       0x0E
#define MSG_TYPE_FN_15       0x0F
#define MSG_TYPE_FN_16       0x10//16
#define MSG_TYPE_FN_17       0x11//17
#define MSG_TYPE_FN_18       0x12//18
#define MSG_TYPE_FN_19       0x13//19
#define MSG_TYPE_FN_20       0x14//20
#define MSG_TYPE_FN_21       0x15//21
#define MSG_TYPE_FN_22       0x16//22
#define MSG_TYPE_FN_23       0x17//23
#define MSG_TYPE_FN_24       0x18//24
#define MSG_TYPE_FN_25       0x19//25
#define MSG_TYPE_FN_26       0x1A//26
#define MSG_TYPE_FN_27       0x1B//27
#define MSG_TYPE_FN_28       0x1C//28
#define MSG_TYPE_FN_29       0x1D//29
#define MSG_TYPE_FN_30       0x1E//30
#define MSG_TYPE_FN_31       0x1F//31
#define MSG_TYPE_FN_39       0x27//39
#define MSG_TYPE_FN_40       0x28//40
#define MSG_TYPE_FN_41       0x29//41
#define MSG_TYPE_FN_42       0x2A//42
#define MSG_TYPE_FN_43       0x2B//43
#define MSG_TYPE_FN_44       0x2C//44
#define MSG_TYPE_FN_45       0x2D//45
#define MSG_TYPE_FN_46       0x2E//46
#define MSG_TYPE_FN_47       0x2F//43
#define MSG_TYPE_FN_48       0x30//44
#define MSG_TYPE_FN_49       0x31//45
#define MSG_TYPE_FN_50       0x32//46
#define MSG_TYPE_FN_52       0x34//52
#define MSG_TYPE_FN_53       0x35//53
#define MSG_TYPE_FN_98       0x62//98
#define MSG_TYPE_FN_99       0x63//99

#define MSG_TYPE_DATA   0x11
#define MSG_TYPE_ACK     0x1
#define MSG_TYPE_NACK   0x2
#define MSG_TYPE_SETLINK  0x3
#define MSG_TYPE_JREQUE 0x4
#define MSG_TYPE_JREPON 0x5
#define MSG_TYPE_TOPO  0x6
#define MSG_TYPE_BROAD  0x7
#define MSG_TYPE_RTS  0x8
#define MSG_TYPE_CTS 0x9
#define MSG_TYPE_STATC_REQUEST 0x0a
#define MSG_TYPE_STATC_RESPONSE  0X0b
	
#define MSG_TYPE_CALL_REQUEST 0x0c
#define MSG_TYPE_CALL_RESPONSE  0X0d
	
#define MSG_TYPE_EXT_DATA  0X0E
	
#define MSG_TYPE_RESET_BOARD_CMD  0X0F
	
#define MSG_TYPE_MAX 0x10

#define JOINREQ_SN_OFFSET 6
#define JOINREQ_ROUTE_OFFSET (JOINREQ_SN_OFFSET+UNIQUE_MAC_ADDR_LEN)
#define JOINRSP_SN_OFFSET 4
#define JOINRSP_ROUTE_OFFSET 11

#define CALLRSP_ROUTE_OFFSET 4





#define MAX_PRO_STORE_SIZE     0x8000  // 32K��С
#define PROGRAM_BLOCK_SIZE    200 

#define MAX_PRO_STORE_SIZE_I_SS     0x10000  // 64K��С
#define PROGRAM_BLOCK_SIZE_I_SS	64 


#define DATA_SUBTYPE_METER  0
#define DATA_SUBTYPE_UPDATE_REQUEST  1
#define DATA_SUBTYPE_UPDATE_RESPONSE  2
#define DATA_SUBTYPE_METER_ADDR 3
#define DATA_SUBTYPE_METER_ADDR_RESPONSE 4
#define DATA_SUBTYPE_METER_BROAD_DATA 5
#define DATA_SUBTYPE_METER_BROAD_DATA_RESPONSE 6
#define DATA_SUBTYPE_METER_ERROR_RESPONSE 7
#define DATA_SUBTYPE_METER_EVENT 				8
#define DATA_SUBTYPE_METER_EVENT_RESPONSE 	9
#define DATA_SUBTYPE_I_SS_UPDATE_REQUEST 	10
#define DATA_SUBTYPE_I_SS_UPDATE_RESPONSE	11



#define EXT_DATA_TYPE_SCAN_METER_REQUEST 	18
#define EXT_DATA_TYPE_SCAN_METER_RESPONE 	19
#define EXT_DATA_TYPE_TOPO_REPORT_REQUEST 	20
#define EXT_DATA_TYPE_TOPO_REPORT_RESPONE 	21

#define EXT_DATA_TYPE_ROUTE_ERROR 		22
#define EXT_DATA_TYPE_FATHER_REQUEST 	23
#define EXT_DATA_TYPE_FATHER_RESPONE 	24

#define EXT_DATA_TYPE_SET_PARA_REQUEST 	25
#define EXT_DATA_TYPE_SET_PARA_RESPONE 	26

#define EXT_DATA_TYPE_SET_SS_UPDATA_MODE_REQUEST 	27
#define EXT_DATA_TYPE_SET_SS_UPDATA_MODE_RESPONE 	28

#define EXT_DATA_TYPE_DATA_TRANS_REQUEST 	29
#define EXT_DATA_TYPE_DATA_TRANS_RESPONE 	30

#define STA_RESPONSE_TAIL 10

#define NODE_DEAD_TIME   2400//1200  �ڵ����ʱ��
#define NODE_UPDATE_TIME (NODE_DEAD_TIME*24)// 2400*25ms*2 , 2 minutes




#define EZ_TEST_BIT         0X30
#if 1
#define OCCUPIED 0xf//�ڹ�ϣ���и�λ��ռ��(��Ч����)
#define EMPTY 0xff
#define DEAD   0//��Ч����
#else 
#define OCCUPIED 0x2//�ڹ�ϣ���и�λ��ռ��(��Ч����)
#define EMPTY 0x3
#define DEAD   0//��Ч����
#endif
#define FM_OCCUPIED 0xf//��λ��ռ��(��Ч����)
#define FM_EMPTY 0xff

#define TIME_FIRE_ON    4000
#define TIME_ACK_ARRIVE 8
#define TIME_CTS_ARRIVE 10
#define TIME_DATA_ARRIVE 20 
#define TIME_DATA_SENDING  20 

#define BUTT_EVENT_1    1
#define BUTT_EVENT_2    2
#define BUTT_FLAG_1     0x10
#define BUTT_FLAG_2     0x20
#define BUTT_DEB        2    /* 20ms */


#define ADV_TYPE_EMPTY 0   // ��ͨ��Ϣ
#define ADV_TYPE_CTIME  0x1  // Уʱ��Ϣ
#define ADV_TYPE_FREEZE 0x2 //  ������Ϣ
#define ADV_TYPE_ADDR_SEARCH  0x3 //  ���е���ַ��ѯ
#define ADV_TYPE_TRANSMISSION  0x4 //  ���е���ַ��ѯ
#define ADV_TYPE_BOARD_JC  0x5 //  �㲥����ʱ�����ݱ�ʶ

//#define ADV_TYPE_METER_DATA  0x4 //  ���е���ַ��ѯ

#define ERR 0  //  ʧ��
#define SUCCESS  1 //  �ɹ�

//#define ENABLE_MAC_INTERRUPTS()        do {VICIntEnable = (1<<EINT2_INT)|(1<<TIMER0_INT);}while(0)
//#define DISABLE_MAC_INTERRUPTS()      do {VICIntEnClr = (1<<EINT2_INT)|(1<<TIMER0_INT);}while(0)

//#define ENABLE_MAC_EXT_INTERRUPT()     do {VICIntEnable = (1<<EINT2_INT);}while(0)
//#define DISABLE_MAC_EXT_INTERRUPT()   do {VICIntEnClr = (1<<EINT2_INT);}while(0)
//#define CLEAR_MAC_EXT_INTERRUPT()      do {EXTINT = 0x04 ; }while(0)
//#define PHY_SEND_FIRE_ON()             do{SpiWriteRegister(0x07, 0x09);}while(0)


//#define	CON_DOWNLOADBITMAP_SIZE		256
//#define DP_DOWNLOAD_BITMAP_ADDR			0			//��¼�������ݵ�λͼ,ʵʱ��λͼ��¼�����ڴ�  modify 2013.8.14
#define DP_DOWNLOAD_SETBITMAP_ADDR		(DP_DOWNLOAD_BITMAP_ADDR + CON_DOWNLOADBITMAP_SIZE)			//����λͼ��ַ

#define DP_DOWNLOAD_EXE_FLAG_ADDR		(DP_DOWNLOAD_BITMAP_ADDR + CON_DOWNLOADBITMAP_SIZE * 2)	//������װ���̱�ǵ�ַ��8���ֽ�   

#define DP_DOWNLOAD_VERSION_ADDR		(DP_DOWNLOAD_EXE_FLAG_ADDR + 0x20)						//�������ݿ�汾��Ϣ������Ϊ16���ֽ�

#define DP_SEGMENT_SHIFT_ADDR		    (DP_DOWNLOAD_EXE_FLAG_ADDR + 0x35)

#define DP_DOWNLOAD_LENGTH_ADDR			(DP_DOWNLOAD_EXE_FLAG_ADDR + 0x40)						//�������ݿ鳤�ȵ����ֽ���������Ϊ4���ֽ�
#define DP_DOWNLOAD_START_ADDR			(DP_DOWNLOAD_EXE_FLAG_ADDR + 0x44)						//�������ݿ���ʼ��ַ������Ϊ4���ֽ�
#define DP_DOWNLOAD_FRAME_NUM_ADDR		(DP_DOWNLOAD_EXE_FLAG_ADDR + 0x48)						//�������ݿ鱨�ĵ���֡��������Ϊ2���ֽ�,ռ��8�ֽ�
#define DP_DOWNLOAD_PARASTART_ADDR		(DP_DOWNLOAD_EXE_FLAG_ADDR + 0x4A)						//�������ݿ���ʼ��ַ������Ϊ4���ֽ�
#define DP_DOWNLOAD_PARA_FRAME_NUM_ADDR	(DP_DOWNLOAD_EXE_FLAG_ADDR + 0x4E)						//������֡����ַ������Ϊ2���ֽ�
#define DP_DOWNLOAD_PARA_LENGTH_ADDR	(DP_DOWNLOAD_EXE_FLAG_ADDR + 0x50)						//�����ֽ���������Ϊ4���ֽ�

#define DP_DOWNLOAD_OLD_VERSION_ADDR	(DP_DOWNLOAD_EXE_FLAG_ADDR + 0x80)		


#define CON_SEND_PRIORITY_HIGH      0
#define CON_SEND_PRIORITY_NORMAL      1
#define CON_SEND_PRIORITY_LOW      2
#define CON_SEND_PRIORITY_HIGH_NUM      2
#define CON_SEND_PRIORITY_NORMAL_NUM      2
#define CON_SEND_PRIORITY_LOW_NUM      2

/***************struct*********************************/
//typedef struct __STFUNCPARA
//{
//    uint8 * data;
//    uint8 len;
//}STFUNCPARA;

typedef struct rfparameter{
	uint8 rf_channel;  //�����ŵ�
	uint8 rf_power;   //������ģ�鷢�书��
	uint8 rf_boad;  //���߿��в�����
	uint8 rf_limit;//��ǿ����
	uint16 rf_net_id;//���������ʶ
	uint8 rf_route;//·�ɷ�ʽ
	uint8 rf_jump_chaneel;  //��Ƶ���к�
	uint16 rf_slotnum;  //ʱ֡����
	uint8 modemType;//���ƽ������ 
	uint8 nodetype;
    uint16 panid;
    uint16 shortID;
}__packed RF_PARAMETER; //�������ò����ṹ��



typedef struct _STINFO3762UP
{
    struct{
        uint8 routeFlg:1;
        uint8 reverse:1;
        uint8 moduleFlg:1;
        uint8 reverse1:1;
        uint8 routeNum:4;
    }bit1;
    struct{
        uint8 channelFlg:4;
        uint8 reverse:4;
    }bit2;
    struct{
        uint8 phaseFlg:4;
        uint8 meterFlg:4;
    }bit3;
    struct{
        uint8 recvSignal:4;
        uint8 sendSignal:4;
    }bit4;    
    struct{
        uint8 warnFlg:1;
        uint8 reverse:7;
    }bit5;
    uint8 seq;
}STINFO3762UP;

typedef struct _STINFO3762DOWN
{
    struct{
        uint8 routeFlg:1;
        uint8 nodeFlg:1;
        uint8 moduleFlg:1;
        uint8 conflict:1;
        uint8 routeNum:4;
    }bit1;
    struct{
        uint8 channelFlg:4;
        uint8 errEncodeFlg:4;
    }bit2;
//	    struct{
//	        uint8 phaseFlg:4;
//	        uint8 meterFlg:4;
//	    }bit3;
    uint8 bitNumAns;
    struct{
        uint8 baudRate;
    }bit4;    
    struct{
        uint8 baudRate:7;
        uint8 rateFlg:1;
    }bit5;
    uint8 seq;
}STINFO3762DOWN;

typedef union _STINFO3762
{
    STINFO3762UP   stUp;
    STINFO3762DOWN stDown;
}STINFO3762;


typedef struct uppkt{
	uint8 ctrl; //������
	uint8 afn;//AFN
	uint8 isneedack;
	uint8 afnoffset;
	uint8 AddrOffSet;
	uint16 len;
	uint16 dataflag;//Fn
	STINFO3762 * rarea;//��Ϣ��
	uint32 arrivetime;
	uint32 deadtime;//��ʱʱ��
	uint32 bytetime;//ÿ���ֽڵ�ʱ���
	uint8 data[256];
} UPPKT;

typedef struct _STVERSION
{
	uint8 vid[2];			//���̴���Ŀǰ�̶�Ϊ'H' 'X' ���ܱ�
	uint8 pid[2];			//оƬ����
	uint8 dd;
    uint8 mm;
    uint8 yy;
	uint8 swCode[2];			//�����������
	uint8 hwCode;
	uint8 reverse[6];
}STVERSION;


#define MAX_NEIGHBOR_NUM 8

typedef struct  nodmeg{
	uint8 needack :1;//�Ƿ���Ҫ�� 0��ʾ���� 1��ʾ��Ҫ
	uint8 bNeedDelete:1;	//�ڵ���Ҫ����
	uint8 bNeedAck:1;//0���ظ���1�ظ�
	uint8 ans_pkt_type:2;//��ʾ������������������ 0��ʾ������  1��ʾ������
	uint8 result:3;//0�ɹ���1��ַ�ظ���2�Ƿ��豸��3��������
}__packed NODMEG;

typedef struct  detail{
	uint8 occupied:4;//�Ƿ�ռ��
	uint8 handled:4;	//�Ƿ񱻴���
	
}__packed DETAIL;

typedef  union DEV_SN_U {
  unsigned short dw[3];
  unsigned char byte[6];
}DEV_SN;

typedef struct  nod_status{
    uint8 protocol:5;
	uint8 evented:1;
	uint8 NetStatus :1;		//�ڵ�����״��    
	uint8 bNeedDelete:1;	//�ڵ���Ҫ����	
}__packed NOD_STATUS;

typedef struct  __ST_NOD_STAS{
	uint8 bNetAck :1;//�����Ƿ���Ҫ�� 0��ʾ���� 1��ʾ��Ҫ

	uint8 bDataAck:1;//����0���ظ���1�ظ�
//		uint8 evented:1;
//		uint8 NetStatus :1;		//�ڵ�����״��
//		uint8 fixed:1;	//��ʾ�ڵ��Ƿ��ID 0��ʾδ�� 1��ʾ��	
//		uint8 handled:1; //0��ʾδ������1��ʾ�Ѵ���
	uint8 ans_pkt_type:3;//��ʾ������������������ 0��ʾ������  1��ʾ������
	//V1 0�ɹ���1��ַ�ظ���2�Ƿ��豸��3��������
	//V2 0�ɹ���1��������2�ܾ�����
	uint8 result:3;
	//V1 0������1��Ч���ݣ�2���ȴ���3У�����4��Ϣ�಻֧�֣�5��ʽ����6����ظ���7��Ų�����
	//V2 0������1ҵ�������2ҵ����ڣ�3�ŵ�����ʧ�ܣ�4��Ч��ַ��5��ЧGTSʱ϶��6��Ӧ��7����������8̫֡����9��Կ�����ã�10��֧�ּ��ܣ�11��Ч����
	uint8 errCode:3;
    uint8 switchstt:1;
    uint8 cardstt:1;
    uint8 reserve:3;
}__packed ST_NOD_STAS;

#define HLV_LPTASK_SWITCH   0x01               //���Ӷ���s
#define HLV_LPTASK_MDCK     0x01               //4Gģ��ʶ��
#define HLV_LPTASK_CP       0x02               //��������
#define HLV_LPTASK_RP       0x04               //�ϸ�����
#define HLV_LPTASK_CLSCK    0x08               //�ر�socket
#define HLV_LPTASK_TST      0x10               //�Լ�
#define HLV_LPTASK_SMSG_HLV      0x20               //hlv����Ϣ
#define HLV_LPTASK_SMSG_MAIN      0x40               //main����Ϣ
#define HLV_LPTASK_SMSG_NETP      0x80               //netp����Ϣ
#define HLV_LPTASK_SMSG_FARP      0x100               //farp����Ϣ
#define HLV_LPTASK_SMSG_SLV      0x200               //slv����Ϣ
#define HLV_LPTASK_LOSTVOL      0x400               //����澯�ϸ���Ϣ
#if 0
//struct of information about collector
typedef struct collector{	
	uint32 father_update; 		// ���ڵ��·�ɸ���ʱ��
	uint32 mother_update;		// ĸ�׽ڵ��·�ɸ���ʱ��
	uint32 nUpdataPktTime;		//�ɼ�������������յ�ʱ��
	uint32 nSSBASEUpdataPktTime;		//�ɼ�������������յ�ʱ��

	//DEV_SN sn; //ÿ���ڵ��Ψһ��ʶ
	//I�Ͳɼ������������������
	uint16 nSSBaseUpdataBlockNum;
	
	uint16 DeadTime;		//�洢�ռ�ĸ���ʱ��
	uint16 FrameDeadTime;// ����ʧЧʱ��
	
	uint16 father;//��һ���ڵ�
	uint16 mother;//�ڶ����ڵ�
	uint8 devAddr[6];
	uint8 hop; //�ڵ��·�ɼ���
	uint8 sendseq; //�������к�
	uint8 recvseq; //�յ���ÿ���ڵ����µ����к�
	uint8 neighnum; // �ھӸ���
//	uint8 frssi ; // ��·�ɵ�RSSI
//	uint8 mrssi; // ��·�ɵ�RSSI
	uint8 softver; // �ڵ������汾��
	NODMEG nod;//
    //uint16 old_id;//�ϱ���ID
	NOD_STATUS nodestatus;

	//�ɼ��������������
	uint8 nUpdataBlockNum;

	//����¼��ظ�ʱ��
//	uint8 MeterStatusAckDeadTime;

	uint8 nHwType;				//Ӳ����ʶ
	uint8 nSoftFn;				//Fn
	uint8 nSubSoftVersion;		//������������汾
} COLLECTOR;
#endif

typedef struct _STMETERPARAFLASH//32�ֽ�
{
    uint8 addrLen;
	uint8 devAddr[CON_DEV_ADDR_LEN_8];
	uint8 hop; //�ڵ��·�ɼ���
	
	uint16 father;//��һ���ڵ�
	uint16 mother;//�ڶ����ڵ�
	uint16 panid; 
    uint16 realPos;//�����ѹ����ַ������������ʵ�ʵ�ַ
    //uint8 protocol;
    NOD_STATUS nodestatus;
    uint8 reverse;
	uint16 softver; // �ڵ������汾��
	uint8 nHwType;				//Ӳ����ʶ
	uint8 GIS[8];
    uint8 checksum;
}STMETERPARAFLASH;

typedef struct _STMETERPARARAM
{

    uint8 addrLen;
	uint8 devAddr[CON_DEV_ADDR_LEN_8];
    uint8 card[CON_DEV_ADDR_LEN_16];
	uint8 hop; //�ڵ��·�ɼ���
	
	uint16 father;//��һ���ڵ�
	uint16 mother;//�ڶ����ڵ�
	uint16 panid; 
    uint16 realPos;//�����ѹ����ַ������������ʵ�ʵ�ַ
    //uint8 protocol;
    NOD_STATUS nodestatus;
    uint8 loginFailedTimes;
	uint8 neighnum; // �ھӸ���
	uint8 loginNo;
    
    uint32_t vbat;
    uint32_t login_count;
}COLLECTOR;

typedef struct _STMETERPARARAMSHADOW
{
	uint32 father_update; 		// ���ڵ��·�ɸ���ʱ��
	//uint32 mother_update;		// ĸ�׽ڵ��·�ɸ���ʱ��
    uint8 rRssi;
    uint8 nodeRssi;             //�ڵ�����ź�����
	uint8 sendseq; //�������к�
	uint8 recvseq; //�յ���ÿ���ڵ����µ����к�
    ST_NOD_STAS nodestatus;
	uint8 FrameDeadTime;// ����ʧЧʱ��
}COLLECTOR_SHADOW;


typedef struct _STUPGRADE
{
    uint8  flg;//0x55:��ʼ
    uint8  type;//��������
    uint8  step;//0:�㲥�׶Σ�1:�����׶�
    uint8  frameSeq;//֡���
    uint16 seq;//�·�֡���
    uint16 delay;//��ʱ
    uint32 timeCount;//��ǰ֡ʱ϶
    uint32 allByte;
    uint16 frameNum;
    uint16 ssID;//��ǰ�����ID
    uint16 p2pSeq;
    uint8 p2pStep;
    uint8 p2pFaildTimes;
    uint8 p2pBitMapIndex;//��i��λͼ
    uint8 p2pBitMapCmpFaileTimes;//λͼ�Ƚ�ʧ��
    uint8 thisp2pBitMapFinishFlg;
    uint8 bitMapFlg;//�������ݱ�ʶ
    uint8 p2pSegmentNum;//λͼ�ܶ���
    uint8 eventFlg;
    uint8 meterProtoBit;//���н��������
    uint16 bitIndex;
    uint8  bitMap[DOWN_UPGRADE_BITMAP_SIZE];
    uint8  oldbitMap[DOWN_UPGRADE_BITMAP_SIZE];
    uint16 crc;
}STUPGRADE;

typedef struct {
	uint8 bBroadMeterDataStatus:2;		
	
	uint8 bReceive:1;
	uint8 bWaitAck:1;
	uint8 bAck:1;
	uint8 bNeedDelete: 1;
	uint8 bDeleteWaitAck: 1;
	
	uint8 bSingleSSSearchMeterReceive:1;
}METER_STATUS;

typedef struct upt{
	uint16 total;  //�ܰ���
	uint16 prgOneSize;//
	uint8 flg;     //��ɱ��
	uint8 type;	//�ļ���ʶ
	uint8 property;//�ļ�����
	uint8 command;//�ļ�ָ��
	uint32 baseAddr;//
	uint32 prgAllSize;//�Ѿ��յ����ܳ���
	uint32 seq;    //��˳���
	uint32 addr;
    uint32 fileLen;
	uint16 nextSeq;//��һ�����к�
	uint16 crc;    //crcУ��
}__packed UPDATE_PKT;


typedef struct cltparam{
	uint8 routeuptime; // �������ڣ���λΪ���ӣ�Ĭ��Ϊ10		
	uint8 maxttl; //ttl ���ֵ����λΪһ��ʱ϶��Ĭ��Ϊ120
	uint8 routeseltime; //·��ѡ��������λΪ8s �� Ĭ��Ϊ30	
}CLTP;


typedef struct _ST_SEG_CTRL_ACK//ack
{
    uint8 len;//�γ�����ָ��֡������ʼ��FCSУ����֮ǰ�ı������ֽ�����
    struct{
        uint8 status :4;//״̬������
        uint8 rssiQ:4;//�ŵ���������(0-15)
    }bit2;
    struct{
        uint8 LSF:1;//LSF��Last Segment Flag��0--�к���֡��1--���һ֡
        uint8 CAP:1;//CAP��Channel Access Priority, 0--�������ȼ���1--�����ȼ�
        uint8 EVT:1;
        uint8 OC:1;//OC��1--������-���ڵ�-�ӽڵ�͸�����ģ�0--�����������ڵ㣬�����ڵ���ӽڵ�ͨѶ����
        uint8 channel:4;//�ŵ���ʶ�����ڱ�ʶ��ǰͨѶ���ŵ���0-15�ֱ��ʾ�ŵ�0-15��TMR=0ʱ����ֵ��Ϊ0
    }bit3;
}ST_SEG_CTRL_ACK;

typedef struct _ST_SEG_CTRL_LD//local down
{
    uint8 lenl;//���ȵ��ֽ�
    struct{
        uint8 lenh :2;//���ȸ��ֽ�
        uint8 seg_count:6;//�μ���
    }bit2;
    struct{
        uint8 LSF:1;
        uint8 CAP:1;
        uint8 CC:1;//CC��Contention Control, 0--��һ֡������Ҫͨ��������1--��һ�δ���ʱʹ��ר��ʱ϶��CFS��
        uint8 OC:1;
        uint8 channel:4;
    }bit3;
}ST_SEG_CTRL_LD;

typedef struct _ST_SEG_CTRL_RM//remote
{
    struct{
        uint8 len :7;//���ȸ��ֽ�
        uint8 relay:1;//�м�����
    }bit1;    
    struct{
        uint8 seg_count;//�μ���
    }bit2;
    struct{
        uint8 LSF:1;
        uint8 CAP:1;
        uint8 CC:1;//CC��Contention Control, 0--��һ֡������Ҫͨ��������1--��һ�δ���ʱʹ��ר��ʱ϶��CFS��
        uint8 OC:1;
        uint8 channel:4;
    }bit3;
}ST_SEG_CTRL_RM;

typedef struct _ST_SEG_CTRL_NET_REQ//��������
{
    struct{
        uint8 len :7;//���ȸ��ֽ�
        uint8 relay:1;//�м�����
    }bit1;    
    struct{
        uint8 phase:2;//��ǰ��λ
        uint8 pwr:2;//��ǰ���书��
        uint8 signalQ:4;//�ź�����0-15������ǿ
    }bit2;
    struct{
        uint8 LSF:1;
        uint8 CAP:1;
        uint8 CC:1;//CC��Contention Control, 0--��һ֡������Ҫͨ��������1--��һ�δ���ʱʹ��ר��ʱ϶��CFS��
        uint8 OC:1;
        uint8 channel:4;
    }bit3;
}ST_SEG_CTRL_NET_REQ;


typedef struct _ST_SEG_CTRL_BEACON0//remote
{
    struct{
        uint8 len :7;//���ȸ��ֽ�
        uint8 rebirth:1;//������ʶ
    }bit1;    
    struct{
        uint8 netinnumL;//�������ڵ���
    }bit2;
    struct{
        uint8 netinnumH:4;//���������ȼ�
        uint8 channel:4;
    }bit3;
}ST_SEG_CTRL_BEACON0;


typedef struct _ST_SEG_CTRL_BEACON1//remote
{
    struct{
        uint8 len :7;//���ȸ��ֽ�
        uint8 rebirth:1;//������ʶ
    }bit1;    
    struct{
        uint8 upenable:1;//�ϱ�����
        uint8 routeenable:1;//ת������
        uint8 modeenable:1;//ģʽ����
        uint8 checktime:1;//������ʱ
        uint8 upgrade:1;//׼������
        uint8 reset:1;//ϵͳ��λ
        uint8 netinenable:1;//��������
        uint8 beenable:1;//�ű�����
        
    }bit2;
    struct{
        uint8 canrebirth:1;//��������
        uint8 netlevel:3;//���������ȼ�
        uint8 channel:4;
    }bit3;
}ST_SEG_CTRL_BEACON1;

typedef union _UN_SEG_CTRL
{
    ST_SEG_CTRL_ACK ctrl_ack;
    ST_SEG_CTRL_LD  ctrl_ld;
    ST_SEG_CTRL_RM  ctrl_rd;
    ST_SEG_CTRL_BEACON0 ctrl_be0;
    ST_SEG_CTRL_BEACON1 ctrl_be1; 
    ST_SEG_CTRL_NET_REQ ctrl_net;
}UN_SEG_CTRL;

typedef struct _ST_PENDING_ADDR_LIST//
{
    uint8 shortAddrNum;
    uint8 longAddrNum;

    uint16 shortAddr[7];
    uint8 longAddr[7][CON_DEV_ADDR_LEN_8];
}ST_PENDING_ADDR_LIST;

typedef struct _ST_FRAME_CTRL//local down
{
    struct{
        uint8 ftd :3;//frame type
        uint8 sec_en:1;//security enable
        uint8 pending:1;//frame pending
        uint8 ack:1;//Ack Request
        uint8 panid:1;//PAN ID Compression
        uint8 dir:1;//dir���䷽�� 0���У�1����
    }bit1;
    struct{
        uint8 prm:1;//�����
        uint8 ie:1;//·�ɱ�ʶ 0:��·�ɣ�1��·��
        uint8 dest:2;//Ŀ���ַ��ʽ
        uint8 version:2;//frame version
        uint8 src:2;//Դ��ַ��ʽ
    }bit2;
}ST_FRAME_CTRL;

typedef struct _ST_ROUTE_CTRL//local down
{
    struct{
        uint8 routeCount :4;//�м̽ڵ�����
        uint8 routeNum:4;//�м̽ڵ���
    }bit1;
    struct{
        uint8 addrMode:2;//��ַ��ʽ
        uint8 reverse:6;//����
    }bit2;
}ST_ROUTE_CTRL;

typedef struct _ST_IE_CTRL//local down
{
    struct{
        uint8 len :6;//ie content����
        uint8 typeL:2;//ie����
    }bit1;
    struct{
        uint8 typeH:7;//ie����
        uint8 mode:1;//
    }bit2;
}ST_IE_CTRL;

typedef struct _ST_SUPER_FRAME_CTRL//��֡������
{
    struct{
        uint8 CAP :4;//final CAP slot��֡�� CAPʱ϶����
        uint8 battery:1;//���
        uint8 reverse:1;//����
        uint8 PAN:1;//�����1����PANЭ��������
        uint8 netinEnable:1;//��������
    }bit1;
    struct{
        uint8 BO:4;//�ű�ָ��macBeaconOrder
        uint8 SO:4;//��ָ֡��macSuperOrder
    }bit2;
}ST_SUPER_FRAME_CTRL;

typedef struct _ST_GTS_DEFINE//GTS������
{
    uint8 GTSNum :3;//GTS����������
    uint8 reverse:4;//����
    uint8 GTSEnable:1;//GTS����
}ST_GTS_DEFINE;

typedef struct _ST_GTS_DIRECTION//GTS������
{
    uint8 GTS0 :1;//GTS0 Mask
    uint8 GTS1 :1;//GTS1 Mask
    uint8 GTS2 :1;//GTS2 Mask
    uint8 GTS3 :1;//GTS3 Mask
    uint8 GTS4 :1;//GTS4 Mask
    uint8 GTS5 :1;//GTS5 Mask
    uint8 GTS6 :1;//GTS6 Mask
    uint8 reverse :1;//����

}ST_GTS_DIRECTION;

typedef struct _ST_GTS_LIST//GTS�������б���
{
    uint8 shortAddrL;//�̵�ַ���ֽ�   
    uint8 shortAddrH;//�̵�ַ���ֽ�   
    struct{
        uint8 start:4;//GTSʱ����ʼ���
        uint8 keep:4;//����ռ��GTSʱ϶����
    }bit3;
}ST_GTS_LIST;

typedef struct _ST_PENDING_ADDR_CTRL//���ݾ�����ַ������
{
    uint8 shortAddrNum :3;//���ݾ����豸��16λ�̵�ַ����
    uint8 reverse:1;//����
    uint8 longAddrNum:3;//���ݾ����豸��64λ����ַ����
    uint8 reverse1:1;//����

}ST_PENDING_ADDR_CTRL;

typedef struct _STIEEE
{
    UN_SEG_CTRL seg_ctrl;
    uint8 seq;//֡���
    ST_FRAME_CTRL frame_ctrl;
    uint8 security_flg;                 //��ȫ������
    uint8 rout_flg;                     //·����Ϣ
    uint8 ftd;                          //frame type
    uint8  afn;                         //AFN       (Ӧ��������1)
    uint8  fn;                          //FN        (Ӧ��������2)
    uint8 routeNum;
    
    uint8 * src_panid;                   //Դpan id
    uint8 * src_addr;                    //Դ��ַ
    uint8 * dest_panid;                  //Ŀ��pan id
    uint8 * dest_addr;                   //Ŀ���ַ
    uint8 * sec_data;                    //��ȫ���ܳ�ʶ����
    uint8 * rout_data;                  //·������
    uint8 * app;                         //Ӧ������  (Ӧ��������3)
    uint8 * send;                        //���ͻ���
    uint8 * addr;
    uint8 * data;
    
    uint32 sec_count;                   //��ȫ֡����    

    uint16 pandId;
    uint8 snr;
    uint8 rssi;
    uint8 srssi;
    //uint8 broadCastFlg;
    uint8 addrlen;
    uint8 len;
}STIEEE;



typedef struct _STWIREPROTOCOLUP
{
    struct{
        uint8 routeFlg:1;
        uint8 nodeFlg:1;
        uint8 addrFlg:1;//0��ʾ6�ֽڳ���ַ��1��ʾ2�ֽڶ̵�ַ
        uint8 conflict:1;
        uint8 routeNum:4;
    }bit1;
    struct{
        uint8 channelFlg :4;
        uint8 rssi:4;
    }bit2;
    struct{
        uint8 phaseFlg:4;
        uint8 protocolFlg:4;
    }bit3;
    struct{
        uint8 reverse:6;
        uint8 relayNum:2;
    }bit4;
    
    uint8 rssi;
    uint8 len;
}STWIREPROTOCOLUP;

typedef struct _STWIREPROTOCOLDOWN
{
    struct{
        uint8 routeFlg:1;
        uint8 nodeFlg:1;
        uint8 addrFlg:1;
        uint8 conflict:1;
        uint8 routeNum:4;
    }bit1;
    struct{
        uint8 channelFlg:4;
        uint8 secretFlg:2;     //���ܷ�ʽ
        uint8 errEncodeFlg:2;
    }bit2;
    struct{
        uint8 reverse:6;
        uint8 relayNum:2;
    }bit3;
    uint8 rssi;
    uint8 bitNumAns;
    uint8 uartFlg;
}STWIREPROTOCOLDOWN;

typedef union _STINFOWIRE
{
    STWIREPROTOCOLUP   stUp;
    STWIREPROTOCOLDOWN stDown;
}STINFOWIRE;

typedef struct ctrl_bit{
    uint8  ftd:4; //֡������
    uint8 acdmode:1;
    uint8 evtmode:1;
    uint8 prm:1;
    uint8 dir:1;
}__packed CTRL_BIT;


typedef struct _STAPDU
{
    CTRL_BIT ctrl;
    uint8 seq;//֡���
    uint8 fn;
    uint8 routeNum;
    
    STINFOWIRE stInfo;
    uint16 pandId;
    
    uint8 snr;
    uint8 rssi;
    uint8 srssi;
    uint8 broadCastFlg;
    uint8 addrlen;
    uint8 * addr;
    uint8 len;
    uint8 * data;
}STAPDU;

typedef struct pkt{    //�������ṹ
    union{
    	STAPDU apdu;
        STIEEE mhr;
    } head;
    uint16 index;
    uint8 protocol;
	uint8 len;
	uint8 sentoffset; // �Ѿ����͵ĳ��Ȼ��ѽ��յ����ݳ���
	uint8 data[256];
    uint8 ttl;
} PKT;

//�������ݷ��ͽṹ
typedef struct ez_tx_pkt{
	uint8 bValid;
	uint8 nBackOffSlot;
	PKT TxPkt;
} EZ_TX_PKT;

typedef struct queue{
	uint8 len;
	uint8 head;
	uint8 tail;
	uint8 lock;
	PKT rxpkt[QUEUE_MAX_LEN];
} QUEUE;

typedef struct HASH_S{	
	uint8 handled:1;//�Ƿ񱻴���״̬
    uint8 maturing:1;//�Ƿ���Ҫ�ϻ�
	uint8 reverse:1;
	uint8 times:5;//��������ʧ�ܴ���
}HASH_INFO;

typedef struct _ST_STAR_RF_PACK
{
    uint16         id;
    CTRL_BIT    ctrl_bit;
    uint8          fn;
    uint8          seq;
    uint8          rssi;
    uint8          broadCastFlg;
}ST_STAR_RF_PACK;

typedef struct addrhashelem{//24�ֽ�
 	uint8  addrLen;
 	uint8  addr[METER_ADDRESS_LENGTH_MAX]; // ��ַ
	uint8  status;//״̬
	uint8  SubType;  //��Լ���� 	
	uint8  src;//��Դ0x55 �Զ�ע��
	uint8  devType;//0:�ɼ���1:���2:�м���
	uint8  baudRate;
    uint8  ucreverse[10];
}HASHT;

typedef struct WB_SPACE{
    uint8  addrLen;
	uint8 	addr[METER_ADDRESS_LENGTH_MAX];		
	uint8	status;		//״̬
    uint8  ucreverse[14];
}WB_SPACE;

typedef struct _STMUCHFRAMEINDEX{
	//unsigned short usAddr[CON_MUCHFRAME_NUM];
	unsigned short usLen[CON_MUCHFRAME_NUM];
	unsigned char ucSeq[CON_MUCHFRAME_NUM];
	unsigned char ucTimes[CON_MUCHFRAME_NUM];
	unsigned char ucData[CON_MUCHFRAME_NUM][UPCOMM_FRAME_MAX_LEN];
    unsigned char ttl;
	unsigned char ucHead;
    unsigned char ucTail;
	unsigned char ucSour;
}STMUCHFRAMEINDEX;

//////////////////////Link//////////////
typedef struct  rinfo{
	uint8 rdepth :4;//��ʾ���͵��������е�����
	uint8 rtype:2;	//��ʾ·�����ȼ���0������·�ɣ�1��ʾ����·�� 
	uint8 pkt_type:1;//������  0���������� 1����������
	uint8 downlink:1;//���������б�ʶ,1��ʾ�������ݣ�0��ʾ��������
}__packed RINFO;

typedef struct  {
	uint16 did;   // �����ߣ��м�
	uint8 len;
	RINFO rinfo; // ·�ɵĳ���
}ROUTE;
typedef struct _ST_SEND_CACHE_INDEX
{
    uint8 level;
    EZ_TX_PKT* ezPkt;
}ST_SEND_CACHE_INDEX;


#define PARA_SYNC_LEN   19//sizeof(COLLECTOR)
#define PARA_ALL_LEN   (sizeof(STMETERPARAFLASH) - 1)


/********************func*************************/
void Delay_us(uint16 us);
void Delay_ms(uint16 ms);
void Set_System_Tick_Flag (void* arg);
void Slot_Time_IRQ (void* arg);
void DelayNS(uint32_t dly);

#endif

