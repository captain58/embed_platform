#ifndef _MY_7108_
#define _MY_7108_

#ifndef EXT_RF
#define EXT_RF extern
#endif



/*******************************************************************************
**表示无线猫处理函数
********************************************************************************/
typedef struct{
    int (*check)(void);                //确定是否为该类型的函数
    void (*on)(void);                   //打开模块的过程
    void (*off)(void);                  //关闭模块的过程
//	    int (*set_name)(char *, uint8_t);                //确定是否为该类型的函数
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
        uint32 typeChecked:1;       //是否已识别模块类型
        uint32 chnrdy:1;            //AT指令是否完成基本配置并准备就绪
        uint32 linked:1;
        uint32 signl:12;             //信号质量(0-31)
        uint32 regtt:1;             //是否已经注册成功
        uint32 sninfo:1;            //是否已经获取到模块和卡信息
        
        uint32 pppoe:1;             //PPP连接是否已经打开
        //uint32 ipcc:1;              //IP连接是否已经连上(包括TCP或UDP)
        uint32 tcpcc:1;             //TCP连接是否已经连上
        //uint32 udpcc:1;             //UDP连接是否已经连上
        uint32 gsstt:1;             //GPRS server(tcp or udp) 侦听是否建立
        uint32 gsclncc1:1;           //GPRS server(tcp or udp) 是否 与 客户端1 的建立链接
        uint32 gsclncc2:1;           //GPRS server(tcp or udp) 是否 与 客户端2 的建立链接
//        uint32 gsclnno:4;        //主动链接过来的客户端的链接号（当前的）
//        uint32 gsclnoldno:4;        //主动链接过来的客户端的链接号（被取代的）
//        uint32 gsurcreadno:4;           //主动上报的数据，所在的socket ID
        uint32 gsip:1;              //GPRS server收到 客户端 的IP数据
        uint32 gsipdex:1;           //是否存在GPRS server收到 客户端 的IP数据 未被上层处理
        uint32 ipdex:1;             //是否存在IP数据未被上层处理
        uint32 smsdex:1;            //短信接收缓存数据有效标志
        uint32 ringRev:1;           //是否存在振铃信号,未被上层处理 
        
        uint32 ipRevData:1;         //表示需要通知应用层处理缓存中的IP数据.消息发送后立刻清零.
//	        uint32 smRevData:1;         //表示需要通知应用层处理缓存中的SMS数据.消息发送后立刻清零.
//	        uint32 smWait:1;            //等待短信接收完全(读取短信是是分两行回复的)
//	        
//	        uint32 smsBufSem:1;         //短信接收缓存的信号量.
//	        uint32 smsind:1;            //是否收到短信上报,但未去读取短信
//	        uint32 smsend:1;            //短信发送是否成功
        //uint32 gsNewL:1;            //无意义
        uint32 state_mid:1;			//表示是否在中间步骤,即是否不允许打断
        uint32 state_wait:1;		//是否在等待'>'。如发送短信或IP数据时等待回复        
    }bit;
} TRFModemState;


typedef struct __TRFModem{
//    TDev dev;
    const TRFModemDrive* drive;

    TRFModemState stt;
    

}TRFModem;

EXT_RF TRFModem* DevRfModem;             //无线猫参数,状态信息
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
    unsigned char hopFreqChNum;               //跳频频道总数
    
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
