/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2003
 *
 *    File name   : user_func.h
 *    Description : User function include file
 *
 *    History :
 *    1. Date        : Feb 10, 2005
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 30870 $
 **************************************************************************/

#ifndef  __USER_FUNC_H
#define  __USER_FUNC_H



//#include "includes.h"
////#include "log.h"
//#include     "LPC_I2c.h"
//#include "hxRfProt.h"
//#include "CompressAddr.h"
//#include "LinkMng.h"
//#include "wirelessSendCache.h"
//#include "protocol.h"
//标识绍兴平水项目,用于版本控制
//此工程修改处:
// 1、没有做电表老化的功能
// 2、从采集器上来的报文:电表地址上报的格式不一样，没有公约类型
//#define PROJECT_PINGSHUI 					

//#define _Debug

#define SI4432_TYPE  1
#define SX1276_TYPE  2

#define WIRELESS_MODE   SX1276_TYPE

//温度补偿功能
//#define  EN_TEMPERAUREDEGC

#define RF_RESET_INTERVAL           (15 * 60 * 1000) //15分钟

//#define MAX_REAL_SUP_SS_NUM 250	//250以上的序号作为调试采集器序号
//CW方式发送
#define CW_TEST 0

//用户定义
#define USER_DianLi 			0
#define USER_LuDeng 		1
#define USER_JuHua_433m 	2
#define USER_DEFINE 			USER_DianLi

#define RFLR_RSSI_VALUE_LIMT        (-70)
#define RFLR_RSSI_VALUE_LIMT_MIN    (-95)

//普通广播包中的字节定义
#define POWER 						0
#define I_SS_BASE_VERSION			1
#define I_SS_VERSION 				2
#define I_SS_VERSION_BOLCK_NUM 	3

//普通广播包长度
#define ADV_SUB_LEN 				4

#define EZMAC_DEBUG_STATUS_IDLE 	0
#define EZMAC_DEBUG_STATUS_BUSY 	1
#define EZMAC_DEBUG_STATUS_TX_DONE 2

#define SYS_TIMER_COUNT_MS  20
#define SYS_TIMER_SLOT_INTERVAL_MS 120

#define FUNC_DELAY_MS(ms)   (ms / SYS_TIMER_COUNT_MS)//SYS_TIMER_SLOT_INTERVAL_MS)


typedef struct ctrl_area{
	uint8 DataFlag:1;
	uint8 UpFlag:1;
	uint8 reserve:2;
//	uint8 seq:4;
	uint8  msgtype:4;
}__packed CTRL_AREA;

typedef union reg_ctrl{  //union关键字
	uint8 all;   //共用体
	CTRL_AREA bits;
}__packed REGCTRL;


typedef struct cid_area{
	uint8  hop:4;
	uint8 cid:4;
}__packed CID_AREA;

typedef union reg_cid{
	uint8 all;  //?
	CID_AREA bits;
}__packed REGCID;

typedef struct coordinate{
	uint8 slot;
	uint8 freq : 4;
	uint8 type : 4;	//behavior in this coordinate
}__packed COORD;

// resource in time and frequency
typedef struct elem{
	COORD coord;
	uint8 masterid;
	uint8 slaveid;
}ELEM ;


// local behavior
typedef struct behavior{
	uint8 behave;
	uint8 freq;
    uint8 rebirth;
}BEHAVIOR ;



typedef struct {
	uint8 Type : 4;
	uint8 NeedTopo : 1;
//	uint8 Speed : 1;
	uint8 I_SSCodeUpdataEnable: 1;
	uint8 SSCodeUpdataEnable :1;
	uint8 StartBoardMeterData :1;
}SUB_TYPE;
typedef struct adverpkt{
	uint8 Curslot;//(每次BS发广播时，它都为0)
	uint8 CurTime;//current time cycle
	uint8 SlotNum;//节点时隙个数
	uint8 FrqNum;//频点个数
	uint8 StarChannel;//起始频点
	uint8 FreqAvail[8];
	uint8 FrqSeq ; 
	uint8 LinkQua[4]; // 填充的链路质量信息
	uint8 SoftVersion;// 采集器软件版本
	uint8 SoftBlockNum;//软件的总共的块数，200Byte为一块
	SUB_TYPE subtype;// 广播附属数据的消息类型
}__packed ADVER;


// struct of pkt's head ,it will be written to regs
typedef struct head{
	REGCTRL ctrl;
	REGCID cid;		
	uint8 sid ; // 发送者地址
	uint8 did ; // 接收者地址
}__packed HEAD;

#define QUEUE_LEN_MASK (QUEUE_MAX_LEN - 1)
// struct of queue


typedef struct datafrag{
	uint8 IsFlag:1;//是否为分片
	uint8 MoreFlag:1;//是否为最后一个分片
	uint8 FlagOffset:6;//在分片中的位置，是分片的第几块
}__packed DATAFRAG;

typedef struct datapkt{
	uint8 ttl;//生存周期
	RINFO route;//
	uint8 dataseq;//包顺序号
	uint8 destid;//目的节点号
	uint8 subtype; // 数据包子类型
	uint8 paylen;//长度
	
}DATAPKT;





typedef struct pkt2{    //物理层包结构
	//HEAD pkthead;//4                                                        			//4字节
	STIEEE apdu;
    //STINFO3762 stDownInfo;//返回正确的seq
    //uint8 downAfn;         //透传的时候记住afn
	uint8 len;
	uint8 sentoffset; // 已经发送的长度或已接收的数据长度
	uint8 data[256];
    uint8 ttl;
} PKT2;

#define PKT_HEAD_LEN (1+2+1+1+sizeof(STINFOWIRE)+2)

// struct of pkt 
typedef struct ctrlrts{   //RTS结构体
	HEAD pkthead;	
	uint8 len;
	uint8 Payload[7];
}__packed CTRLRTS;

typedef struct ctrlpkt{
	HEAD pkthead;	//4           //4字节
	uint8 len;
	uint8 Payload;
}__packed CTRPKT;


typedef struct Meter_Address_Rec_Mark{	
	uint8 bReceived : 1;
	uint8 bAcked :1;
	uint8 bFlagChenkOut:1;
	uint8 reserve:5;
}METER_ADDRESS_REC_MARK;

typedef struct netpara{
	uint8 netfreqnum;  //频点个数
	uint8 netslotnum;   //节点时隙个数
	uint8 StartChannel;  //起始频点
	uint8 broadcast_freq;//广播频率
	uint8 FreqSeq: 4;        //FreqSeq
	uint8 netcid : 4;        //网络ID
}__packed NET_PARAMETER; //网络设置参数结构体



typedef struct rffreq{
	uint8 startChanne[4];//起始频点
	uint16 channel_Wideth;//频道带宽
	uint8 frq_num;//频点个数
	uint8 reverse;
    uint32 orderNum;//订单号	
}__packed RF_FREQ; //网络频点定义结构体

typedef struct ez{
	uint8 power;//功率
	uint8 netid;//网络号
	uint8 frq;//频点	
	uint8 cnt;//计数
	uint8 msr;
}__packed EZ_TEST; //发射功率测试结构体

typedef struct ls{	
	uint8 netid;//网络号
	uint8 frq;//频点	
	uint8 cnt;//计数
	uint8 msr;
	uint32 deadtime;
}__packed LISN_TEST; //发射功率测试结构体

typedef struct channelparameter{
	uint8 channel_num;//起始频点
	uint8 data_start;//频道带宽
	uint8 data_end;//业务频点	
	uint8 data_freq_num;//业务频点个数
}__packed CHANNEL_PARA; //信道参数结构体

typedef struct EZMacReg{
	uint8 RSSILR;
	uint8 RXRSSIVALUE;	
	
	uint8 Temper_ADC_Value;		//上一次的温度值
	uint8 Cur_Crystal_Value;		//当前设置的校准值
	uint8 CrystalValue;				//存储的校准值
	}EZMACPROREG;

typedef struct addmap{
	uint8 id;
	uint8 coladdr[6];
}ADDMAP;

#define MAX_MAP_SIZE 256
typedef struct maptable{
	ADDMAP map[MAX_MAP_SIZE];
	uint32 mapsize;
}MAPTABLE;

//struct of infoarea 
typedef struct infoarea{
	
	uint8 routeflag : 1;//路由标识
	uint8 subflag :1 ;//附属节点标识
	uint8 txmodule:1;//通信模块标识
	uint8 collisiondet:1;   //冲突检测
	uint8 hoplevel:4;//中继级别
	
	uint8 channel:4;//信道标志
	uint8 codeflag:4;//纠错编码标识

	uint8 answerbytes;//预计应答字节数
	
	uint16 txrate:15;//通信速率
	uint16 rateflag:1;//速率单位标识

	uint8 reserved;//预留
	
}__packed INFO;

//struct of uplink pkt

typedef struct DEBUG_UPPKT{
	uint8 nSourceId;
	uint8 nTTL;
	UPPKT RecvPacket;
} DEBUG_UPPKT;

typedef struct subpkt{
	uint8 type;//规约类型
	uint8 subpktlen;//报文长度
	uint8 startflag;//起始标志位
	uint8 ammeteraddr[6];
}SUBPKT;


typedef struct joinrequest{
	uint8 ttl;//生存周期
	RINFO route;//路由级数
	uint8 dataseq;//包顺序号
	uint8 sourid; //源节点号
	uint8 parentid;//父节点
	uint8 neignum;//邻居个数
	uint8 SN[6];//节点唯一标识
	uint8 softver;//采集器版本号
}__packed JOINREQ;




typedef struct joinresponse{
	uint8 ttl;
	RINFO route;//一字节
	uint8 dataseq;//包顺序号
	uint8 destid;//目的节点号
}JOINRSP;

// struct of statistic request packet
typedef struct stapkt{
	uint8 ttl;
	RINFO route;//路由级数
	uint8 seq;//包顺序号
	uint8 destid;//目的节点号
	uint8 staseq;//统计周期序号
	
}STAREQUEST;

typedef struct ExtData{
	uint8 ttl;
	RINFO route;//路由级数
	uint8 seq;//包顺序号
	uint8 destid;//目的节点号
	uint8 SourceID;
	uint8 SubType;
	uint8 PayloadLen;
}EXTDATA;

typedef struct starxpkt{
	uint8 staseq;//统计周期的序号
	uint8 sourid; // 节点地址
	uint8 txsour; //源生发送成功数据包个数
	uint8 uprelay;// 上行转发成功数据包个数
	uint8 downrelay;// 下行转发成功数据包个数
	uint8 rxnum;//接收到数据包个数
	uint8 txfail;//发送失败的数据包个数
	uint8 delay;
	uint32 txtime;
	uint32 rxtime;
}STARXPKT;

typedef struct stareport{
	uint8 stamodeon; // 开关
	uint8 curstaseq;//当前统计周期序号
	uint32 recvmap;//接收完成的点
	//uint32 curcycletime;
	uint32 nextcycletime;//下一次统计周期开始时间
	//STARXPKT staresult[MAX_SUP_SS_NUM];//每个点的统计结果
}STAREPORT;


typedef struct topoinfo{
	uint8 nodeid;
	uint8 hop;
	uint8 father;
	uint8 mother;
}TOPOINFO;

typedef struct MOD_VER_S{
	uint8 dd;
	uint8 yy;
	uint16 ver;
}MOD_VER;



typedef struct HWCODE_S{
	uint8 no:5;
	uint8 type:3;
}HWCODE_INFO;

typedef struct IMAGE_HEAD_S{
	uint8 vid[2];			//厂商代码目前固定为'H' 'R' 不能变
	uint8 pid[2];			//芯片代码
	MOD_VER mver; 		//版本
	
	HWCODE_INFO hwCode;			//运行硬件代码
	uint8 swCode;			//运行软件代码
	uint16 checksum;		//代码段校验和，不含头部数据
	uint32 plen;			//代码段长度
	uint8 reserved[16];		//保留字节
	//uint8 playload[256];		//代码段	
}__packed ImageBody;


#define UPGRADE_FORM_BIN        0x01
#define UPGRADE_FORM_HEX        0x02
#define UPGRADE_FORM_MOTO       0x03

#define UPGRADE_TYPE_NEW_TER_MODULE     0x01 
#define UPGRADE_TYPE_NEW_DEV            0x02
#define UPGRADE_TYPE_NEW_MODULE_BOOT    0x03

#define UPGRADE_LEN_LIMIT       480

#ifdef UPGRADE_TEST
#define UPGRADE_TYPE_TER_MODULE     0x07
#define UPGRADE_TYPE_METER_MODULE   0x08
#define UPGRADE_TYPE_REPEAT_MODULE   0x03
#else
#define UPGRADE_TYPE_TER_MODULE     0x03
#define UPGRADE_TYPE_METER_MODULE   0x07
#define UPGRADE_TYPE_REPEAT_MODULE   0x08

#endif
#define UPGRADE_TYPE_COLLECT_SELF   0x06
#define UPGRADE_TYPE_METER_SELF     0xFD

#define DEV_TYPE_MASK               0x0F
//#define STA_MODE_ON



//	#if FREQ_NUM_BRAZIL
//	#define MAX_FREQ_SCAN       38
//	#define MAX_AVAILBLE_FRQ    38
//	#else
//	#if FREQ_NUM_49
//	#define MAX_FREQ_SCAN       48
//	#define MAX_AVAILBLE_FRQ    48
//	#else
//	#define MAX_FREQ_SCAN       32
//	#define MAX_AVAILBLE_FRQ    32
//	#endif
//	#endif

#define DATAPKT_HEAD_LEN 6
#define STATPKT_HEAD_LEN 5
#define DATAPKT_PAYLOAD_OFFSET 6
#define METER_ADDRESS_LENGTH 6



//广播抄表命令的延时触发时间
#define BROAD_METER_DATA_CMD_TRIGER   	 20 // 20*25ms 0.5

//对单个采集器进行查表的延时时间
#define SINGLE_SS_SEARCH_METER_TIME		24000  // 24000*25ms 10

#ifndef DEBUG
#define  WATCH_DOG_ON   //开启看门狗模式
#endif

#define ACTION_FIRE   0x1
#define ACTION_WAIT_ACK 0x2 
#define ACTION_WAIT_CTS 0x6
#define ACTION_WAIT_DATA 0x7
#define ACTION_IDLE   0x3
//#define ACTION_CCA    0x4
#define ACTION_SEND_RTS    0x4
#define ACTION_SEND_DATA    0x5
#define ACTION_SEND_ADV   0x8 
#define ACTION_WAIT_DATA_SENT  0x5

#define CLEAR_TX_FIFO 0
#define CLEAR_RX_FIFO 1

#define UART_DEAD_CNT  288000 // 2个小时，未从串口接收到正确数据
#define EZMAC_DEAD_CNT  48000 // 20分钟，未从串口接收到正确数据


//#define IAP_ACTION_UPDATE_HASH  2
#if (PROG_FLASH_512)

#define IAP_ACTION_ERASE_BS_CODE 3  //主程序升级存储前擦除FLASH

#define IAP_ACTION_ERASE_SS_CODE 4  //采集器程序升级存储前擦除FLASH

#define IAP_ACTION_WRITE_BS_CODE 5 //更新主程序代码
#define IAP_ACTION_WRITE_SS_CODE 6 //更新采集器程序代码

#define IAP_ACTION_ERASE_I_SS_CODE  17  //程序升级存储前擦除FLASH
#define IAP_ACTION_WRITE_I_SS_CODE  18  //更新I型采集器底座的程序代码

#endif
#define IAP_ACTION_TRANSFER_HASH1 7
#define IAP_ACTION_TRANSFER_HASH2 8
//	
//	#define IAP_ACTION_ERASE_WRITE_SS_HASH1  9
//	#define IAP_ACTION_ERASE_WRITE_SS_HASH2  10
//	#define IAP_ACTION_ERASE_BLACK_SS_HASH1  11
//	#define IAP_ACTION_ERASE_BLACK_SS_HASH2  12
//	#define IAP_ACTION_TRANSFER_WRITE_SS_HASH1  13
//	#define IAP_ACTION_TRANSFER_WRITE_SS_HASH2  14
//	#define IAP_ACTION_TRANSFER_BLACK_SS_HASH1  15
//	#define IAP_ACTION_TRANSFER_BLACK_SS_HASH2  16


//#define IAP_ACTION_UPDATE_SELF1_CODE 8 //更新程序代码
//#define IAP_ACTION_UPDATE_SELF2_CODE 9 //更新程序代码
//#define IAP_ACTION_ERASE_SELF1_CODE 10  //程序升级存储前擦除FLASH
//#define IAP_ACTION_ERASE_SELF2_CODE 11  //程序升级存储前擦除FLASH

//#define MAX_BS_STORE_SIZE     0x27FFF  // 160K大小
#define MAX_BS_STORE_SIZE     0x1FFFF  // 128K大小
#if (PROG_FLASH_512)
#ifdef APP_HIGH
#define IAP_BS_MAIN_ENTRY 0X40000

#define IAP_BS_CODE_START 0X10000
//#define IAP_BS_CODE_END 0X3FFFF
#define IAP_BS_CODE_END 0X2FFFF

#pragma location= IAP_BS_CODE_START
__no_init uint8 Update_Bs_Source[MAX_BS_STORE_SIZE]; //160K采集器程序地址

#define IAP_BS_FLASH_PAGE_START 9
//#define IAP_BS_FLASH_PAGE_END 14
#define IAP_BS_FLASH_PAGE_END 12


#define IAP_SS_CODE_START 0X8000
#define IAP_SS_CODE_END 0XFFFF

#define IAP_SS_FLASH_PAGE_START 8
#define IAP_SS_FLASH_PAGE_END 8
#else
#define IAP_BS_MAIN_ENTRY 0X10000

#define IAP_BS_CODE_START 0X40000
//#define IAP_BS_CODE_END 0X67FFF
#define IAP_BS_CODE_END 0X5FFFF

#pragma location= IAP_BS_CODE_START
__no_init uint8 Update_Bs_Source[MAX_BS_STORE_SIZE]; //160K采集器程序地址
#define IAP_BS_FLASH_PAGE_START 15
//#define IAP_BS_FLASH_PAGE_END 19
#define IAP_BS_FLASH_PAGE_END 18


#define IAP_SS_CODE_START 0X8000
#define IAP_SS_CODE_END 0XFFFF

#define IAP_SS_FLASH_PAGE_START 8
#define IAP_SS_FLASH_PAGE_END 8
#endif

#define IAP_I_SS_CODE_START 0X30000
#define IAP_I_SS_CODE_END 0X3FFFF


#else
#define IAP_BS_MAIN_ENTRY 0X10000

#endif

#define CODE_VALID_FLAG     0xA5A5A5A5

#define EXT_FLASH_SS_CODE_PARA      0x1000
#define EXT_FLASH_I_SS_CODE_PARA    0x2000
#define EXT_FLASH_BS_CODE_PARA      0x3000



#define EXT_FLASH_SS_CODE_START     0x4000
#define EXT_FLASH_SS_CODE_END       0x23FFF
#if (PROG_FLASH_512)
#define FLASH_SS_CODE_LEN       (IAP_SS_CODE_END - IAP_SS_CODE_START + 1)
#else
#define FLASH_SS_CODE_LEN       (EXT_FLASH_SS_CODE_END - EXT_FLASH_SS_CODE_START + 1)
#endif

#define EXT_FLASH_I_SS_CODE_START   0x24000
#define EXT_FLASH_I_SS_CODE_END     0x43FFF
#if (PROG_FLASH_512)
#define FLASH_I_SS_CODE_LEN       (IAP_I_SS_CODE_END - IAP_I_SS_CODE_START + 1)
#else
#define FLASH_I_SS_CODE_LEN       (EXT_FLASH_I_SS_CODE_END - EXT_FLASH_I_SS_CODE_START + 1)
#endif

#define EXT_FLASH_BS_CODE_START     0x44000
#define EXT_FLASH_BS_CODE_END       0x83FFF
#if (PROG_FLASH_512)
#define FLASH_BS_CODE_LEN       (IAP_BS_CODE_END - IAP_BS_CODE_START + 1)
#else
#define FLASH_BS_CODE_LEN       (EXT_FLASH_BS_CODE_END - EXT_FLASH_BS_CODE_START + 1)
#endif

#define EXT_FLASH_BLACK_MAP1_START   0x84000
#define EXT_FLASH_BLACK_MAP1_END     0x8BFFF

#define EXT_FLASH_BLACK_MAP2_START   0x8C000
#define EXT_FLASH_BLACK_MAP2_END     0x93FFF

#define EXT_FLASH_WHITE_MAP1_START   0x94000
#define EXT_FLASH_WHITE_MAP1_END     0x9BFFF

#define EXT_FLASH_WHITE_MAP2_START   0x9C000
#define EXT_FLASH_WHITE_MAP2_END     0xA3FFF

#define IAP_I_SS_FLASH_PAGE_START 13
#define IAP_I_SS_FLASH_PAGE_END 14


#define MCR_IDLE   0x0
#define MCR_NWAIT_ACK   0x1
#define MCR_WAIT_ACK  0x2
#define MCR_WAIT_CTS  0x3
#define MCR_WAIT_DATA  0x4
#define MCR_WAIT_DATA_SENT  0x5


#define MCR_CTS_ARRIVE  0x9
#define MCR_ACK_ARRIVE  0xA
#define MCR_DATA_ARRIVE  0xB
#define MCR_DATA_SENT   0xC


#define IMMEDIATE_SEND 0x1
#define DELAY_SEND 0x0

#define BROADCAST_ADDR  0xFF



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


/* The golden ration: an arbitrary value */
#define JHASH_GOLDEN_RATIO	0x9e3779b9
#define __jhash_mix(a, b, c) \
{ \
  a -= b; a -= c; a ^= (c>>13); \
  b -= c; b -= a; b ^= (a<<8); \
  c -= a; c -= b; c ^= (b>>13); \
  a -= b; a -= c; a ^= (c>>12);  \
  b -= c; b -= a; b ^= (a<<16); \
  c -= a; c -= b; c ^= (b>>5); \
  a -= b; a -= c; a ^= (c>>3);  \
  b -= c; b -= a; b ^= (a<<10); \
  c -= a; c -= b; c ^= (b>>15); \
}

#define DEBUG_PACKET_LEN 4

//extern UPPKT recvuppkt;
//extern uint32 curslottime; //目前的总时隙个数
//extern uint8 Uart_Status;
extern uint8 nFreqChannel;



typedef struct _STUPCOMMBUF{
	unsigned short usHeadpos;
	unsigned short usTailpos;
	unsigned char *pucData;
}STUPCOMMBUF;





//#define UPCOMM_FRAME_SHADOW_MAX_LEN 1024
#define UPCOMM_PORT


#define CON_SOUR_PORT		1	//本地表口
#define CON_SOUR_TESTPORT			2	//调试口
/*报文格式检查*/
typedef enum
{
	RET_SUCCESS=0,				//成功
	RET_ERROR,					//错误
	RET_TOO_LITTLE,				//未到最小长度
	RET_HEAD_ERROR,				//帧头错误
	RET_NOENOUGH_LONG,			//长度未到
	RET_DUPLICATE_ADDR,			//重复表地址
	RET_SINGLE,					//事件返回单个
	RET_PARA_ERR,				//参数错误
	RET_BUSY,					//忙
	RET_UNFINISHED,				//未完成
	RET_IDLE,					//空
}ENURETVALUE;

typedef struct __STFUNCPARA
{
    uint8 * data;
    uint8 len;
}STFUNCPARA;

/*!
 * RF process function return codes
 */
typedef enum
{
    RF_IDLE,
    RF_BUSY,
    RF_RX_DONE,
    RF_RX_TIMEOUT,
    RF_TX_DONE,
    RF_TX_TIMEOUT,
    RF_LEN_ERROR,
    RF_CHANNEL_EMPTY,
    RF_CHANNEL_ACTIVITY_DETECTED,
    RF_RX_CRCERR,
}tRFProcessReturnCodes;
/*!
 * RF state machine
 */
//LoRa
typedef enum
{
    RFLR_STATE_IDLE,
    RFLR_STATE_RX_INIT,
    RFLR_STATE_RX_RUNNING,
    RFLR_STATE_RX_DONE,
    RFLR_STATE_RX_TIMEOUT,
    RFLR_STATE_TX_INIT,
    RFLR_STATE_TX_RUNNING,
    RFLR_STATE_TX_DONE,
    RFLR_STATE_TX_TIMEOUT,
    RFLR_STATE_CAD_INIT,
    RFLR_STATE_CAD_RUNNING,
}tRFLRStates;

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
#define FRAME_HEAD_CODE_376_2	0x68//帧头码
#define FRAME_TAIL_CODE_376_2	0x16//帧尾码

#define FRAME_HEAD_AREA_BYTES		3//帧头部分字节数:帧头码 + 长度（2字节） + 长度（2字节）+ 帧头码
#define FRAME_TAIL_AREA_BYTES		2//帧头部分字节数：校验码 + 帧尾码

//#pragma location= FLASH_MAP_ADD
//__no_init HASHT htable1[MAX_HASH_TABLE_FLASH_SIZE]; //哈希表(32K)
//
//#pragma location= FLASH_MAP_ADD2
//__no_init HASHT htable2[MAX_HASH_TABLE_FLASH_SIZE]; //哈希表(32K)

#if (PROG_FLASH_512)
#pragma location= PROGRAM_START_ADDR
__no_init uint8 UpdateSource[MAX_PRO_STORE_SIZE]; //32K采集器程序地址

#pragma location= IAP_I_SS_CODE_START
__no_init uint8 Update_I_SS_Source[MAX_PRO_STORE_SIZE_I_SS]; //64KI型采集器底座程序地址
#endif

/* 定义CCLK值大小，单位为KHz */
//#define  IAP_FCCLK            11059
#define  IAP_FCCLK            60000

#define FLASH_MAP_ADD 0x30000  //哈希表1入口地址
#define FLASH_MAP_SECTOR 20

#define FLASH_MAP_ADD2 0x38000 //备份哈希表入口地址
#define FLASH_MAP_SECTOR2 21


#define PROGRAM_SELF1_START_ADDR	(0x10000)
#define PROGRAM_SELF2_START_ADDR	(0x40000)

//#define PROGRAM_START_ADDR	(0x8000)

#define PROGRAM_END_ADDR	(1024*14-1)	
#define PROGRAM_PARAM_ADDR			(0x07)					//程序末端的起始7为参数
#define PROGRAM_VERSION_ADDR			(PROGRAM_PARAM_ADDR+0)			//版本号地址
#define PROGRAM_BLOCK_NUM_ADDR			(PROGRAM_PARAM_ADDR+1)			//该版本程序所含块数

#define PROGRAM_START_SECTOR 1
#define PROGRAM_END_SECTOR 4

//发送频段定义
#define FREQ_408M       0

#define FREQ_433M 		(FREQ_408M + 1)
#define FREQ_470M 		(FREQ_433M + 1)
#define FREQ_868M 		(FREQ_470M + 1)
#define FREQ_915M 		(FREQ_868M + 1)
#define FREQ_928M 		(FREQ_915M + 1)
//#define FREQ_915M       (FREQ_408M + 1)
#define FREQ_NUM 		(FREQ_928M+1)

#define SX127X_FREQ             FREQ_433M//发送频段定义
#define FREQ_408M       0

#define FREQ_433M 		(FREQ_408M + 1)
#define FREQ_470M 		(FREQ_433M + 1)
#define FREQ_868M 		(FREQ_470M + 1)
#define FREQ_915M 		(FREQ_868M + 1)
#define FREQ_928M 		(FREQ_915M + 1)
//#define FREQ_915M       (FREQ_408M + 1)
#define FREQ_NUM 		(FREQ_928M+1)

#define SX127X_FREQ             FREQ_433M

#if (SX127X_FREQ == FREQ_433M)
#define LORA_CHANNEL_NUM    7
#define LORA_CHANNEL_START  1
#define LORA_CHANNEL_END    6
#else
#define LORA_CHANNEL_NUM    7
#define LORA_CHANNEL_START  1
#define LORA_CHANNEL_END    6
#endif

//#endif // __INCLUDES_H


#define GET_TICK_COUNT( )                           ( TickCounter )

void System_Time_Update (void);
void Alarm (void);
//void DelayNS(LPC_INT32U dly);
void EINT3_IRQHandler(void);
void Copy_ADPKT_To_FIFO(ADVER pkt);
void Timer0_ISR(void* arg);
void SwSend();
void SwRecv();
void SwIDLE();
void User_Parameter_Init(void);
uint8 Check_PKT(uint8 *buf ,int limit ,UPPKT * pkt);
uint8 Check_SUM(uint8 * data, int len) ;
void Handle_Up_PKT(UPPKT *pkt, uint8 bEZMACDebugTypeTemp, DEBUG_UPPKT* pRecvPacketTemp);
uint8 Handle_RX_Down_PKT(PKT *pkt);
static  inline uint32 Jhash_3words(uint32 a, uint32 b, uint32 c, uint32 initval);
extern void Set_Timeout(uint32 msecd);
void Send_Dlink_PKT(void* sendpkt ,uint8 imfire);
void SwRssi();
void Si_Pameter_Scan();
BEHAVIOR Get_Current_Behave(uint16 slot,uint8 timeframe);
BEHAVIOR Get_Current_Behave2(uint16 slot, uint8 timeframe);
void Freq_Trans(uint8 methord, uint8 * frqlist, uint8 * MacFrqlist, uint8 frqnum);
void Check_Freq(uint8 methord, uint8 *MacFrqlist, uint8 frqnum);
uint8 Timer_INT_Random (void);
void  Update_Tx_Ts();
void Handle_RX_Up_Nage_PKT(PKT *pkt, uint8 bEZMACDebug, DEBUG_UPPKT* pEZMACRecvPacketTemp);
void Handle_RX_Up_Nage_PKT2(uint8 *pkt,uint8 type, uint8 bEZMACDebug, DEBUG_UPPKT* pEZMACRecvPacketTemp);

/*************************************************************************
 * Function Name: HandleRxUpNagePkt
 * Parameters:  返回抄表失败报文
 * Return:  none
 * Description: HandleRxUpNagePkt
 *
 *************************************************************************/

void Handle_RX_Up_Nage_PKT_802(PKT *pkt);
//void Fill_Sta_PKT();
void Handle_Sta_Report(STAREPORT * sp);
uint8  LPC_Memory_CMP(uint8 *dest,uint8 *sour,uint32 len);
void Update_Node();
void Display_Neighbor_RSSI();
void Externel_INT_WakeUp(uint8 state);
void Ez_MAC_Init();
 void Reset(void);
 void Set_Self_ID();
uint8 Get_Flash_Pro(uint8 * destdata,uint8 block);
void Clear_FIFO(uint8 rxortx);
uint8  EZMacPRO_Receive(void);
uint8 EZMacPRO_Transmit_Adv(uint8 type, uint8 * data, uint8 len);
uint8 EZMacPRO_Transmit_RTS(void);
void Timer0_Int_TX_StateMachine (uint8 state);
void Timer0_Int_RX_StateMachine (uint8 state);
void FSM_Goto_Next_State( void);
void  Externel_INT_RX_StateMachine(uint8 msr, uint8 intStatus1, uint8 intStatus2);
void  Externel_INT_TX_StateMachine(uint8 msr, uint8 intStatus1, uint8 intStatus2);
void Externel_INT_TimeOut(uint32);
uint8 Check_Network();
void Get_Net_Parameter(void);
void Init_Record(uint8 value);
uint8 Check_ID(uint16 id);
void Clean_PKT(PKT *pkt);
void Set_Crastyle( uint8 value);
uint8 Check_Sta_PKT(uint8 seq,uint8 id);
uint8 After(uint32 x , uint32 y);
uint8 Before(uint32 x , uint32 y);
void Handle_JRep();
void Handle_Affirm(void);
void Create_JRep(uint16 k, EZ_TX_PKT * ezPkt);
void Create_Affirm(uint16 k, EZ_TX_PKT * ezPkt);//推送数据应答包
uint16 Cal_Hash_Value(uint8 *meteraddr);
//uint8 Check_Get_ID(uint8 get_id,uint8* get_sn);
void Erase_FM_SN(void);

void Copy_EZ_TEST_PKT_To_FIFO(ADVER pkt);
void Correct_RF_Clock(uint8 set_para,uint8 accyracy);

uint8 FlashCRCCheck(uint8 blocknum);
void Cltor_init(void);
void Search_Addr(uint8 flag);	
void Report_Unhandled_Addr(uint8 *pkt);
void Search_SS(uint8 *data);
void Check_SS_Status(void);
void Record_Handled_Addr(int hash);
void Report_Haneled_Addr(uint16 handled_no);
//extern volatile int TickSysFlag;
void MeterSearchMng();
void BoardMeterDataResponce();
void SendTickToSSProc();
void BoardMeterDataStatusCheck();
void Single_Search_SS_Send_Meter();
void Meter_Addr_Rec_Ack();
void Meter_Status_Rec_Ack();
void Send_Broad();

void Uart_App_Frame_Proc();
void SS_Updata_Req_Proc();
void I_SS_Base_Updata_Req_Proc();
void CW_generator(uint8 pwr);
void GetFreqChannel();
void EZMAC_Debug_App_Frame_Proc();
uint8 SendPacketToEzmacProc(DEBUG_UPPKT* pRecvPacket, uint8* pData, uint8 len, uint8 bNotRespone);
void BoardPktTimeOutProc();
void CloseBoardSearchMeter(uint8 flag);
void External_INT_init(void);
void SendReturnpkt(uint8 bError, STINFO3762 * pstinfo);
uint16 CRC_16BIT_Check1(uint8 *pentry,uint32 len);//FLASH数据校验
uint16 CRC_16BIT_Check2(uint16 CRC_acc, uint8 *pentry,uint32 len);//FLASH数据校验
//uint16 CRC_16BIT_Check2(uint16 CRC, uint8 *pentry,uint32 len);
uint16 CRC_16BIT_Check(uint16 len);
uint8 FlashCRCCheck(uint8 blocknum);
uint8 Do_Iap_EraseSector(uint8 sec1, uint8 sec2);
uint8 Check_Meter_Type(uint8 *buf);
uint8 Check_GetID(uint8 get_id,uint8* get_sn);//判断实际的SN与表中ID对应的SN是否一样
//uint16 Re_Allocate_Id(uint8* sn_addr);//重新分配ID
void Handle_Meter_Address(PKT *pkt);
void Handle_I_SS_Update_Request(PKT *pkt);
void Set_Crystal_Value(uint8 tmp_adc);
uint8 CheckConflict(void);
uint8 EZMacPRO_Transmit_Reset(void);//发送重启广播命令
uint8 EZMacPRO_Transmit(/*STAPDU * stPdu*/);
uint8 EZMacPRO_Transmit_Direct(EZ_TX_PKT * pEzTxPkt);/*STAPDU * stPdu*/
void upVersion(void);
uint8 Meter_Check(uint8* addr);
uint16 Meter_Num(void);
uint16 Record_Num(void);
uint16 Get_BroadCast_Delay(uint8 type);

void port_init();
void upcomm_mng();
uint8 SendPacketToMac(CTRL_BIT * ctrl_bit, uint8 fn, uint8 hop, STFUNCPARA * addr, STFUNCPARA * data, uint8 seq, uint8 rssi, EZ_TX_PKT * etpkt);
void DeleteProcInit(void);
uint8 Code_Check_Sum(uint32 ulbase);
uint8 EZMacPRO_Transmit_Test(void);
uint8 EZMacPRO_Transmit_Beacon(uint8 type, uint8 rebirth);

/***********************************************************
 * @function_name: ES_IAP_Mark
 * @function_file: iap_file.c
 * @描述: 设置或者清除升级标记         
 * @参数: 
 * @param:   setorclr 设置或者清除升级标记 
 * @param:   ID       数据ID
 * @param:  length   数据长度
 * @param:  address  数据地址
 * @返回: 
 * @return: true  写入成功
           false  写入失败 
 * @作者:  yzy (2010-11-11)
 *******************************************************/
uint8 ES_APP_IAP_Mark(bool setorclr, uint32 length, uint32 address);
/************************************************************************
 * @function: ES_IAP_Ready
 * @描述: 
 * 
 * @参数: 
 * @param: address 升级起始地址
 * @param: length 用户程序需要升级的程序的长度
 * @param: isreset 升级完之后是否需要复位用户程序
 * 
 * @返回: 
 * @return: bool  true成功/false失败
 * @说明: 
 * @作者: xugl (2013/7/16)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
bool ES_IAP_Ready(uint32 address, uint32 length, bool isreset);
uint16 getIdWithAddr(uint8 * addr);
unsigned char write_prg_to_flash(unsigned int addr, unsigned short usNo, unsigned char *pucData, unsigned short usDatalen);
void wireless_proc(void);
uint8 EEpromTest(uint32 len);
uint8 FlashTest(uint32 len);
uint8 getSignalQuality(int rssi, int max, int low);
uint8 Compose_3762Data(uint8 flg, uint8 * data, uint16 len, uint8 * addr, uint8 addrLen, STINFO3762 * stinfo, uint8 afn, uint16 fn);
//	EZ_TX_PKT * Get_Idle_Send_Pkt(void);
void wireless_send();
void muchframe_fill(unsigned char *pucFrame, unsigned short usDatalen, STMUCHFRAMEINDEX *pstMuchframeindex, unsigned char ucSour, unsigned char ucSeq);
uint8 getRssiByRssiQuality(uint8 rssiQ);
uint8 getRssiQualityByRssi(uint8 rssi);
uint8 paraRecorvery(uint8 type);
uint8 getNetSts(void);
uint8 checkMeterModuleProtocol(void);
void I2cWrite(uint8 Addr, uint8 *Data, uint16 inside_addr, uint16 NByte);
uint8 getSignalQuality(int rssi, int max, int low);
int Hash_Table_Insert(HASHT *elem, int* pReturnHash);
void Hash_Transfer(void);
void Init_Hash_Table(void);
uint8  I2cRead(uint8 Addr, uint8 *Data, uint16 inside_addr, uint16 NByte);


#endif  /* __USER_FUNC_H */

