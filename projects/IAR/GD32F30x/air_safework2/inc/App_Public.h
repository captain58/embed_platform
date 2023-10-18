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
#define UPLINK_DATA_TIMEOUT 512 //上行的否定回答时间
#define UART_BYTE_TIMEOUT   40 // 20*25ms 0.5
#define UART_PKT_TIMEOUT   	 240 // 6
///////eeprom
#define UPGRADE_FRAME_LIMIT         192
#define EEPROM_UPDATE_PARA      0x400

#define DP_DOWNLOAD_BITMAP_ADDR (EEPROM_UPDATE_PARA + 32)
#define	CON_DOWNLOADBITMAP_SIZE		128

#define DP_DOWNLOAD_PROC_PARA_ADDR   (DP_DOWNLOAD_BITMAP_ADDR + CON_DOWNLOADBITMAP_SIZE)
#define DP_DOWNLOAD_PROC_LEN    (sizeof(STUPGRADE))


#define FM_NEED_OPEN_SS_VALIDATE 0xDD9 	//从节点主动注册标志

#define FM_BROAD_METER_ENABLE 0xDDA 	//从节点主动注册标志
#define FM_BROAD_METER_TIME 0xDDB 	//从节点主动注册持续时间

#define FM_CHANNEL_SET_MODE 0xDDD 	//信道配置模式
#define FM_FREQ_CHANNEL_NUM 0xDDE 	//频段号
#define FM_NEED_RESET_CMD 0xDDF 	//重启后需要发复位命令的标志

#define FM_I_SS_CODEUPDATA_ENABLE 0xDE0 	//I型采集器底座升级使能标志位
#define FM_DEVICE_ADDR_LEN 0xDE1 	//载波主节点地址(6个字节)


#define FM_BS_INFO 0xDF0 	//无线模块的出厂序号
#define FM_NETPARA_ADDR 0xE00 //网络参数地址  (原0x01FF )
#define FM_CLTP_ADDR       0xE10 //路由参数、采集器参数地址   (原0x0205  )
#define FM_HASH_USED 0xE20  //哈希表示是否使用标识位地址
#define FM_CAP_MEND 0xE21  //电容校准值地址
#define FM_RESET_MESAGE 0xE22  // 本地模块复位信息值记录地址
#define FM_SS_CODEUPDATA_ENABLE  0xE23  // 采集器升级使能标志位
#define FM_METER_DEAD_COUNT  0xE24  // 电表老化次数

#define FM_TUOPU_SOURCE  0xE25  // 拓扑显示的节点内容来源
#define FM_WRITE_HASH_USED  0xE26  // 白名单使用标志
#define FM_BLACK_HASH_USED  0xE27  // 黑名单使用标志

#define FM_SS_VALIDATE_ENABLE  0xE28  // 采集器认证使能参数
#define FM_METER_INFO_SYN_ENABLE  0xE29  // 测量点同步使能参数

#define FM_FREQ_DEFINE 0xE30 //频点定义 
#define FM_CHANNEL_PARA 0xE40 //信道参数
#define FM_GPS_PARA 0xE50 //GPS
#define FM_DEVICE_ADDR 0xE60 	//载波主节点地址(16个字节 ascii码)
/////////////////////////////////////////////////////////////RF

enum  //发
{
	PKT_TYPE_DATA,   //0
   	PKT_TYPE_SS_DATA,
   	PKT_TYPE_SS_SEARCH_METER,
   	PKT_TYPE_TOPO,
	PKT_TYPE_SS_NET_SET,
	PKT_TYPE_HD_TEST,	
	PKT_TYPE_DATA_802,   //0
};

enum  //发
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

enum  //收
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


//路由控制中F1添加载波从节点

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

#define MAX_MASTER_NODE_TO_LED_CLOSE_COUNT  600 //100毫秒周期
//#define FRQ_BRODCAST_CHAN 0x02// for Net3

//#define FRQ_BRODCAST_CHAN 0x0
//#define DATAPKT_PAYLOAD_OFFSET 6
#define MIN_TIME_LIVE 40
//#define MAX_TIME_LIVE 120
#define MAX_TIME_LIVE 100
#define TIME_OUT_MAX    7//7级路由，7倍最大超时
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

#define MAX_SUP_SLOT_NUM 32 //业务时隙的个数
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
#define FULLTIME 1600   // 40S 广播消息
//#define BROAD_METER_SEARCH_TIME 4800   // 2minutes 查找电表广播消息
#define BROAD_METER_SEARCH_TIME 72000   // 30minutes 查找电表广播消息
//#define BROAD_METER_SEARCH_TIME 24000   // 10minutes 查找电表广播消息

#define METER_DEAD_COUNT 30  //电表老化的次数

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

#define SUBTYPE_TRANS 0X00 //透明转发
#define SUBTYPE_97 0X01 //97规约
#define SUBTYPE_07 0X02 //07规约

//#define FM_SS_INFO 0x400 	//邻居表信息；未使用


#define OVER_TIME 0 //通信超时
#define INVALID_DATA 1 //无效数据单元
#define WRONG_LEN 2 //长度错
#define WRONG_CHECK 3 //校验错
#define REREA_CHECK 4 //信息类不存在
#define WRONG_FORM 5 //格式错误
#define OVERLAP_ADDR 6 //表号重复
#define FIND_NO_ADDR 7 //表号不存在
#define NO_ANSWER 8 //电表应用层无应答

#define PROGRAM_I_SS_BASE_VERSION_ADDR  9

#define PROGRAM_SS_TYPE_ADDR  9


#define FM_METER_INFO_BASE_ADDR 0x1000//入网节点信息保存地址

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

//无线发送两个区域的选择
typedef enum
{
   EZ_TX_TYPE_DATA =0,
   EZ_TX_TYPE_NET,
   EZ_TX_TYPE_HHU
}EZ_Cur_Tx_Type;
	

//检测版本，增加版本:添加电表
//#define DECTECT_BEIJING


#define RS_RESET_TIME ((30 * 60 * 1000) / 25)

#define UPCOMM_FRAME_MAX_LEN 512
#define UART_DEBUG UART0_CHANNEL
#define CON_MUCHFRAME_SECTOR_NUM	2	/*多帧数据存储空间占用dataflash的扇区数(每扇区4k字节)*/
#define CON_MUCHFRAME_NUM			4

#define DOWN_UPGRADE_DIVISION_LEN   96
#define DOWN_UPGRADE_BITMAP_SIZE     96

#define CON_DEV_ADDR_LEN_6  6//地址长度
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

#define FTRANS_ERR_OK       0               //下载成功
#define FTRANS_ERR_FT       1               //下载出错
#define FTRANS_ERR_UP       2               //下载完成,可开始校验升级

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
#define AFN_TYPE_DEBUG 0xF0  //调试模式
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





#define MAX_PRO_STORE_SIZE     0x8000  // 32K大小
#define PROGRAM_BLOCK_SIZE    200 

#define MAX_PRO_STORE_SIZE_I_SS     0x10000  // 64K大小
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

#define NODE_DEAD_TIME   2400//1200  节点更新时间
#define NODE_UPDATE_TIME (NODE_DEAD_TIME*24)// 2400*25ms*2 , 2 minutes




#define EZ_TEST_BIT         0X30
#if 1
#define OCCUPIED 0xf//在哈希表中该位被占用(有效数据)
#define EMPTY 0xff
#define DEAD   0//无效数据
#else 
#define OCCUPIED 0x2//在哈希表中该位被占用(有效数据)
#define EMPTY 0x3
#define DEAD   0//无效数据
#endif
#define FM_OCCUPIED 0xf//该位被占用(有效数据)
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


#define ADV_TYPE_EMPTY 0   // 普通消息
#define ADV_TYPE_CTIME  0x1  // 校时消息
#define ADV_TYPE_FREEZE 0x2 //  冻结消息
#define ADV_TYPE_ADDR_SEARCH  0x3 //  进行电表地址查询
#define ADV_TYPE_TRANSMISSION  0x4 //  进行电表地址查询
#define ADV_TYPE_BOARD_JC  0x5 //  广播抄表时的数据标识

//#define ADV_TYPE_METER_DATA  0x4 //  进行电表地址查询

#define ERR 0  //  失败
#define SUCCESS  1 //  成功

//#define ENABLE_MAC_INTERRUPTS()        do {VICIntEnable = (1<<EINT2_INT)|(1<<TIMER0_INT);}while(0)
//#define DISABLE_MAC_INTERRUPTS()      do {VICIntEnClr = (1<<EINT2_INT)|(1<<TIMER0_INT);}while(0)

//#define ENABLE_MAC_EXT_INTERRUPT()     do {VICIntEnable = (1<<EINT2_INT);}while(0)
//#define DISABLE_MAC_EXT_INTERRUPT()   do {VICIntEnClr = (1<<EINT2_INT);}while(0)
//#define CLEAR_MAC_EXT_INTERRUPT()      do {EXTINT = 0x04 ; }while(0)
//#define PHY_SEND_FIRE_ON()             do{SpiWriteRegister(0x07, 0x09);}while(0)


//#define	CON_DOWNLOADBITMAP_SIZE		256
//#define DP_DOWNLOAD_BITMAP_ADDR			0			//记录下载数据的位图,实时的位图记录到后备内存  modify 2013.8.14
#define DP_DOWNLOAD_SETBITMAP_ADDR		(DP_DOWNLOAD_BITMAP_ADDR + CON_DOWNLOADBITMAP_SIZE)			//设置位图地址

#define DP_DOWNLOAD_EXE_FLAG_ADDR		(DP_DOWNLOAD_BITMAP_ADDR + CON_DOWNLOADBITMAP_SIZE * 2)	//程序下装过程标记地址，8个字节   

#define DP_DOWNLOAD_VERSION_ADDR		(DP_DOWNLOAD_EXE_FLAG_ADDR + 0x20)						//程序数据块版本信息，长度为16个字节

#define DP_SEGMENT_SHIFT_ADDR		    (DP_DOWNLOAD_EXE_FLAG_ADDR + 0x35)

#define DP_DOWNLOAD_LENGTH_ADDR			(DP_DOWNLOAD_EXE_FLAG_ADDR + 0x40)						//程序数据块长度的总字节数，长度为4个字节
#define DP_DOWNLOAD_START_ADDR			(DP_DOWNLOAD_EXE_FLAG_ADDR + 0x44)						//程序数据块起始地址，长度为4个字节
#define DP_DOWNLOAD_FRAME_NUM_ADDR		(DP_DOWNLOAD_EXE_FLAG_ADDR + 0x48)						//程序数据块报文的总帧数，长度为2个字节,占用8字节
#define DP_DOWNLOAD_PARASTART_ADDR		(DP_DOWNLOAD_EXE_FLAG_ADDR + 0x4A)						//参数数据块起始地址，长度为4个字节
#define DP_DOWNLOAD_PARA_FRAME_NUM_ADDR	(DP_DOWNLOAD_EXE_FLAG_ADDR + 0x4E)						//参数总帧数地址，长度为2个字节
#define DP_DOWNLOAD_PARA_LENGTH_ADDR	(DP_DOWNLOAD_EXE_FLAG_ADDR + 0x50)						//参数字节数，长度为4个字节

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
	uint8 rf_channel;  //无线信道
	uint8 rf_power;   //主无线模块发射功率
	uint8 rf_boad;  //无线空中波特率
	uint8 rf_limit;//场强门限
	uint16 rf_net_id;//无线网络标识
	uint8 rf_route;//路由方式
	uint8 rf_jump_chaneel;  //跳频序列号
	uint16 rf_slotnum;  //时帧长度
	uint8 modemType;//调制解调类型 
	uint8 nodetype;
    uint16 panid;
    uint16 shortID;
}__packed RF_PARAMETER; //网络设置参数结构体



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
	uint8 ctrl; //控制域
	uint8 afn;//AFN
	uint8 isneedack;
	uint8 afnoffset;
	uint8 AddrOffSet;
	uint16 len;
	uint16 dataflag;//Fn
	STINFO3762 * rarea;//信息域
	uint32 arrivetime;
	uint32 deadtime;//超时时间
	uint32 bytetime;//每个字节的时间戳
	uint8 data[256];
} UPPKT;

typedef struct _STVERSION
{
	uint8 vid[2];			//厂商代码目前固定为'H' 'X' 不能变
	uint8 pid[2];			//芯片代码
	uint8 dd;
    uint8 mm;
    uint8 yy;
	uint8 swCode[2];			//运行软件代码
	uint8 hwCode;
	uint8 reverse[6];
}STVERSION;


#define MAX_NEIGHBOR_NUM 8

typedef struct  nodmeg{
	uint8 needack :1;//是否需要答复 0表示不用 1表示需要
	uint8 bNeedDelete:1;	//节点需要离网
	uint8 bNeedAck:1;//0不回复，1回复
	uint8 ans_pkt_type:2;//表示回入网包还是心跳包 0表示心跳包  1表示入网包
	uint8 result:3;//0成功，1地址重复，2非法设备，3容量不足
}__packed NODMEG;

typedef struct  detail{
	uint8 occupied:4;//是否被占用
	uint8 handled:4;	//是否被处理
	
}__packed DETAIL;

typedef  union DEV_SN_U {
  unsigned short dw[3];
  unsigned char byte[6];
}DEV_SN;

typedef struct  nod_status{
    uint8 protocol:5;
	uint8 evented:1;
	uint8 NetStatus :1;		//节点入网状况    
	uint8 bNeedDelete:1;	//节点需要离网	
}__packed NOD_STATUS;

typedef struct  __ST_NOD_STAS{
	uint8 bNetAck :1;//网络是否需要答复 0表示不用 1表示需要

	uint8 bDataAck:1;//数据0不回复，1回复
//		uint8 evented:1;
//		uint8 NetStatus :1;		//节点入网状况
//		uint8 fixed:1;	//表示节点是否绑定ID 0表示未绑定 1表示绑定	
//		uint8 handled:1; //0表示未被处理，1表示已处理
	uint8 ans_pkt_type:3;//表示回入网包还是心跳包 0表示心跳包  1表示入网包
	//V1 0成功，1地址重复，2非法设备，3容量不足
	//V2 0成功，1容量满，2拒绝接入
	uint8 result:3;
	//V1 0正常，1无效数据，2长度错误，3校验错误，4信息类不支持，5格式错误，6表号重复，7表号不存在
	//V2 0正常，1业务溢出，2业务过期，3信道接入失败，4无效地址，5无效GTS时隙，6无应答，7计数器出错，8帧太长，9密钥不可用，10不支持加密，11无效参数
	uint8 errCode:3;
    uint8 switchstt:1;
    uint8 cardstt:1;
    uint8 reserve:3;
}__packed ST_NOD_STAS;

#define HLV_LPTASK_SWITCH   0x01               //勾子动作s
#define HLV_LPTASK_MDCK     0x01               //4G模块识别
#define HLV_LPTASK_CP       0x02               //抄表任务
#define HLV_LPTASK_RP       0x04               //上告任务
#define HLV_LPTASK_CLSCK    0x08               //关闭socket
#define HLV_LPTASK_TST      0x10               //自检
#define HLV_LPTASK_SMSG_HLV      0x20               //hlv秒消息
#define HLV_LPTASK_SMSG_MAIN      0x40               //main秒消息
#define HLV_LPTASK_SMSG_NETP      0x80               //netp秒消息
#define HLV_LPTASK_SMSG_FARP      0x100               //farp秒消息
#define HLV_LPTASK_SMSG_SLV      0x200               //slv秒消息
#define HLV_LPTASK_LOSTVOL      0x400               //掉电告警上告消息
#if 0
//struct of information about collector
typedef struct collector{	
	uint32 father_update; 		// 父节点的路由更新时间
	uint32 mother_update;		// 母亲节点的路由更新时间
	uint32 nUpdataPktTime;		//采集器升级请求的收到时间
	uint32 nSSBASEUpdataPktTime;		//采集器升级请求的收到时间

	//DEV_SN sn; //每个节点的唯一标识
	//I型采集器底座升级请求块数
	uint16 nSSBaseUpdataBlockNum;
	
	uint16 DeadTime;		//存储空间的更新时间
	uint16 FrameDeadTime;// 报文失效时间
	
	uint16 father;//第一父节点
	uint16 mother;//第二父节点
	uint8 devAddr[6];
	uint8 hop; //节点的路由级别
	uint8 sendseq; //发送序列号
	uint8 recvseq; //收到的每个节点最新的序列号
	uint8 neighnum; // 邻居个数
//	uint8 frssi ; // 主路由的RSSI
//	uint8 mrssi; // 从路由的RSSI
	uint8 softver; // 节点的软件版本号
	NODMEG nod;//
    //uint16 old_id;//上报的ID
	NOD_STATUS nodestatus;

	//采集器升级请求块数
	uint8 nUpdataBlockNum;

	//电表事件回复时间
//	uint8 MeterStatusAckDeadTime;

	uint8 nHwType;				//硬件标识
	uint8 nSoftFn;				//Fn
	uint8 nSubSoftVersion;		//附属器件软件版本
} COLLECTOR;
#endif

typedef struct _STMETERPARAFLASH//32字节
{
    uint8 addrLen;
	uint8 devAddr[CON_DEV_ADDR_LEN_8];
	uint8 hop; //节点的路由级别
	
	uint16 father;//第一父节点
	uint16 mother;//第二父节点
	uint16 panid; 
    uint16 realPos;//如果是压缩地址，这里是名单实际地址
    //uint8 protocol;
    NOD_STATUS nodestatus;
    uint8 reverse;
	uint16 softver; // 节点的软件版本号
	uint8 nHwType;				//硬件标识
	uint8 GIS[8];
    uint8 checksum;
}STMETERPARAFLASH;

typedef struct _STMETERPARARAM
{

    uint8 addrLen;
	uint8 devAddr[CON_DEV_ADDR_LEN_8];
    uint8 card[CON_DEV_ADDR_LEN_16];
	uint8 hop; //节点的路由级别
	
	uint16 father;//第一父节点
	uint16 mother;//第二父节点
	uint16 panid; 
    uint16 realPos;//如果是压缩地址，这里是名单实际地址
    //uint8 protocol;
    NOD_STATUS nodestatus;
    uint8 loginFailedTimes;
	uint8 neighnum; // 邻居个数
	uint8 loginNo;
    
    uint32_t vbat;
    uint32_t login_count;
}COLLECTOR;

typedef struct _STMETERPARARAMSHADOW
{
	uint32 father_update; 		// 父节点的路由更新时间
	//uint32 mother_update;		// 母亲节点的路由更新时间
    uint8 rRssi;
    uint8 nodeRssi;             //节点接收信号质量
	uint8 sendseq; //发送序列号
	uint8 recvseq; //收到的每个节点最新的序列号
    ST_NOD_STAS nodestatus;
	uint8 FrameDeadTime;// 报文失效时间
}COLLECTOR_SHADOW;


typedef struct _STUPGRADE
{
    uint8  flg;//0x55:开始
    uint8  type;//升级类型
    uint8  step;//0:广播阶段，1:单发阶段
    uint8  frameSeq;//帧序号
    uint16 seq;//下发帧序号
    uint16 delay;//延时
    uint32 timeCount;//当前帧时隙
    uint32 allByte;
    uint16 frameNum;
    uint16 ssID;//当前处理表ID
    uint16 p2pSeq;
    uint8 p2pStep;
    uint8 p2pFaildTimes;
    uint8 p2pBitMapIndex;//第i段位图
    uint8 p2pBitMapCmpFaileTimes;//位图比较失败
    uint8 thisp2pBitMapFinishFlg;
    uint8 bitMapFlg;//返回数据标识
    uint8 p2pSegmentNum;//位图总段数
    uint8 eventFlg;
    uint8 meterProtoBit;//下行接入表类型
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
	uint16 total;  //总包数
	uint16 prgOneSize;//
	uint8 flg;     //完成标记
	uint8 type;	//文件标识
	uint8 property;//文件属性
	uint8 command;//文件指令
	uint32 baseAddr;//
	uint32 prgAllSize;//已经收到的总长度
	uint32 seq;    //包顺序号
	uint32 addr;
    uint32 fileLen;
	uint16 nextSeq;//下一包序列号
	uint16 crc;    //crc校验
}__packed UPDATE_PKT;


typedef struct cltparam{
	uint8 routeuptime; // 心跳周期，单位为分钟，默认为10		
	uint8 maxttl; //ttl 最大值，单位为一个时隙，默认为120
	uint8 routeseltime; //路由选择间隔，单位为8s ， 默认为30	
}CLTP;


typedef struct _ST_SEG_CTRL_ACK//ack
{
    uint8 len;//段长度是指从帧控制域开始到FCS校验字之前的报文总字节数。
    struct{
        uint8 status :4;//状态返回字
        uint8 rssiQ:4;//信道链接质量(0-15)
    }bit2;
    struct{
        uint8 LSF:1;//LSF：Last Segment Flag，0--有后续帧，1--最后一帧
        uint8 CAP:1;//CAP：Channel Access Priority, 0--正常优先级，1--高优先级
        uint8 EVT:1;
        uint8 OC:1;//OC：1--集中器-主节点-从节点透传报文，0--集中器与主节点，或主节点与从节点通讯报文
        uint8 channel:4;//信道标识：用于标识当前通讯的信道，0-15分别表示信道0-15。TMR=0时，该值恒为0
    }bit3;
}ST_SEG_CTRL_ACK;

typedef struct _ST_SEG_CTRL_LD//local down
{
    uint8 lenl;//长度低字节
    struct{
        uint8 lenh :2;//长度高字节
        uint8 seg_count:6;//段计数
    }bit2;
    struct{
        uint8 LSF:1;
        uint8 CAP:1;
        uint8 CC:1;//CC：Contention Control, 0--下一帧传输需要通过竞争，1--下一段传输时使用专用时隙（CFS）
        uint8 OC:1;
        uint8 channel:4;
    }bit3;
}ST_SEG_CTRL_LD;

typedef struct _ST_SEG_CTRL_RM//remote
{
    struct{
        uint8 len :7;//长度高字节
        uint8 relay:1;//中继请求
    }bit1;    
    struct{
        uint8 seg_count;//段计数
    }bit2;
    struct{
        uint8 LSF:1;
        uint8 CAP:1;
        uint8 CC:1;//CC：Contention Control, 0--下一帧传输需要通过竞争，1--下一段传输时使用专用时隙（CFS）
        uint8 OC:1;
        uint8 channel:4;
    }bit3;
}ST_SEG_CTRL_RM;

typedef struct _ST_SEG_CTRL_NET_REQ//入网申请
{
    struct{
        uint8 len :7;//长度高字节
        uint8 relay:1;//中继请求
    }bit1;    
    struct{
        uint8 phase:2;//当前相位
        uint8 pwr:2;//当前发射功率
        uint8 signalQ:4;//信号质量0-15，弱到强
    }bit2;
    struct{
        uint8 LSF:1;
        uint8 CAP:1;
        uint8 CC:1;//CC：Contention Control, 0--下一帧传输需要通过竞争，1--下一段传输时使用专用时隙（CFS）
        uint8 OC:1;
        uint8 channel:4;
    }bit3;
}ST_SEG_CTRL_NET_REQ;


typedef struct _ST_SEG_CTRL_BEACON0//remote
{
    struct{
        uint8 len :7;//长度高字节
        uint8 rebirth:1;//再生标识
    }bit1;    
    struct{
        uint8 netinnumL;//已入网节点数
    }bit2;
    struct{
        uint8 netinnumH:4;//入网数量等级
        uint8 channel:4;
    }bit3;
}ST_SEG_CTRL_BEACON0;


typedef struct _ST_SEG_CTRL_BEACON1//remote
{
    struct{
        uint8 len :7;//长度高字节
        uint8 rebirth:1;//再生标识
    }bit1;    
    struct{
        uint8 upenable:1;//上报允许
        uint8 routeenable:1;//转发允许
        uint8 modeenable:1;//模式调整
        uint8 checktime:1;//自主对时
        uint8 upgrade:1;//准备升级
        uint8 reset:1;//系统复位
        uint8 netinenable:1;//入网允许
        uint8 beenable:1;//信标允许
        
    }bit2;
    struct{
        uint8 canrebirth:1;//允许再生
        uint8 netlevel:3;//入网数量等级
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
        uint8 dir:1;//dir传输方向 0下行；1上行
    }bit1;
    struct{
        uint8 prm:1;//发起端
        uint8 ie:1;//路由标识 0:无路由；1有路由
        uint8 dest:2;//目标地址方式
        uint8 version:2;//frame version
        uint8 src:2;//源地址方式
    }bit2;
}ST_FRAME_CTRL;

typedef struct _ST_ROUTE_CTRL//local down
{
    struct{
        uint8 routeCount :4;//中继节点索引
        uint8 routeNum:4;//中继节点数
    }bit1;
    struct{
        uint8 addrMode:2;//地址格式
        uint8 reverse:6;//保留
    }bit2;
}ST_ROUTE_CTRL;

typedef struct _ST_IE_CTRL//local down
{
    struct{
        uint8 len :6;//ie content长度
        uint8 typeL:2;//ie类型
    }bit1;
    struct{
        uint8 typeH:7;//ie类型
        uint8 mode:1;//
    }bit2;
}ST_IE_CTRL;

typedef struct _ST_SUPER_FRAME_CTRL//超帧定义域
{
    struct{
        uint8 CAP :4;//final CAP slot超帧中 CAP时隙长度
        uint8 battery:1;//电池
        uint8 reverse:1;//保留
        uint8 PAN:1;//如果是1则是PAN协调器发出
        uint8 netinEnable:1;//接入允许
    }bit1;
    struct{
        uint8 BO:4;//信标指数macBeaconOrder
        uint8 SO:4;//超帧指数macSuperOrder
    }bit2;
}ST_SUPER_FRAME_CTRL;

typedef struct _ST_GTS_DEFINE//GTS定义域
{
    uint8 GTSNum :3;//GTS描述器个数
    uint8 reverse:4;//保留
    uint8 GTSEnable:1;//GTS允许
}ST_GTS_DEFINE;

typedef struct _ST_GTS_DIRECTION//GTS方向域
{
    uint8 GTS0 :1;//GTS0 Mask
    uint8 GTS1 :1;//GTS1 Mask
    uint8 GTS2 :1;//GTS2 Mask
    uint8 GTS3 :1;//GTS3 Mask
    uint8 GTS4 :1;//GTS4 Mask
    uint8 GTS5 :1;//GTS5 Mask
    uint8 GTS6 :1;//GTS6 Mask
    uint8 reverse :1;//保留

}ST_GTS_DIRECTION;

typedef struct _ST_GTS_LIST//GTS描述器列表域
{
    uint8 shortAddrL;//短地址低字节   
    uint8 shortAddrH;//短地址高字节   
    struct{
        uint8 start:4;//GTS时序起始序号
        uint8 keep:4;//连续占用GTS时隙长度
    }bit3;
}ST_GTS_LIST;

typedef struct _ST_PENDING_ADDR_CTRL//数据就绪地址定义域
{
    uint8 shortAddrNum :3;//数据就绪设备的16位短地址个数
    uint8 reverse:1;//保留
    uint8 longAddrNum:3;//数据就绪设备的64位长地址个数
    uint8 reverse1:1;//保留

}ST_PENDING_ADDR_CTRL;

typedef struct _STIEEE
{
    UN_SEG_CTRL seg_ctrl;
    uint8 seq;//帧序号
    ST_FRAME_CTRL frame_ctrl;
    uint8 security_flg;                 //安全控制域
    uint8 rout_flg;                     //路由信息
    uint8 ftd;                          //frame type
    uint8  afn;                         //AFN       (应用数据域1)
    uint8  fn;                          //FN        (应用数据域2)
    uint8 routeNum;
    
    uint8 * src_panid;                   //源pan id
    uint8 * src_addr;                    //源地址
    uint8 * dest_panid;                  //目标pan id
    uint8 * dest_addr;                   //目标地址
    uint8 * sec_data;                    //安全域密匙识别字
    uint8 * rout_data;                  //路由数据
    uint8 * app;                         //应用数据  (应用数据域3)
    uint8 * send;                        //发送缓存
    uint8 * addr;
    uint8 * data;
    
    uint32 sec_count;                   //安全帧计数    

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
        uint8 addrFlg:1;//0表示6字节长地址，1表示2字节短地址
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
        uint8 secretFlg:2;     //加密方式
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
    uint8  ftd:4; //帧类型域
    uint8 acdmode:1;
    uint8 evtmode:1;
    uint8 prm:1;
    uint8 dir:1;
}__packed CTRL_BIT;


typedef struct _STAPDU
{
    CTRL_BIT ctrl;
    uint8 seq;//帧序号
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

typedef struct pkt{    //物理层包结构
    union{
    	STAPDU apdu;
        STIEEE mhr;
    } head;
    uint16 index;
    uint8 protocol;
	uint8 len;
	uint8 sentoffset; // 已经发送的长度或已接收的数据长度
	uint8 data[256];
    uint8 ttl;
} PKT;

//无线数据发送结构
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
	uint8 handled:1;//是否被处理状态
    uint8 maturing:1;//是否需要老化
	uint8 reverse:1;
	uint8 times:5;//连续抄表失败次数
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

typedef struct addrhashelem{//24字节
 	uint8  addrLen;
 	uint8  addr[METER_ADDRESS_LENGTH_MAX]; // 地址
	uint8  status;//状态
	uint8  SubType;  //规约类型 	
	uint8  src;//来源0x55 自动注册
	uint8  devType;//0:采集器1:表计2:中继器
	uint8  baudRate;
    uint8  ucreverse[10];
}HASHT;

typedef struct WB_SPACE{
    uint8  addrLen;
	uint8 	addr[METER_ADDRESS_LENGTH_MAX];		
	uint8	status;		//状态
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
	uint8 rdepth :4;//表示发送点在网络中的跳数
	uint8 rtype:2;	//表示路由优先级，0代表主路由，1表示备用路由 
	uint8 pkt_type:1;//包类型  0代表入网包 1代表心跳包
	uint8 downlink:1;//数据上下行标识,1表示上行数据，0表示下行数据
}__packed RINFO;

typedef struct  {
	uint16 did;   // 接收者，中继
	uint8 len;
	RINFO rinfo; // 路由的长度
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

