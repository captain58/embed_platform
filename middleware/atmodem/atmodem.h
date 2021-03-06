/****************************************Copyright (c)**************************************************
**                                  ________科技有限公司
**                                          开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: dev_modem.h
**创   建   人: yzy
**最后修改日期: 2010年3月5日
**描        述: modem设备操作
**注        意:
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2010年3月5日
** 描　述: 原始版本
********************************************************************************************************/


#ifndef _EXT_ATMODEM_H_
#define _EXT_ATMODEM_H_

#ifndef EXT_ATMODEM
#define EXT_ATMODEM extern
#endif

/*******************************************************************************
**模块交互过程的识别信息
********************************************************************************/
EXT_ATMODEM uint8 gucs_ATMID[64];              //AT模块识别信息(其中GDAT为46字节)
EXT_ATMODEM uint8 gucs_IMEI[15];               //AT模块IMEI号字符串
EXT_ATMODEM uint8 gucs_CCID[20];               //SIM卡CCID号字符串
EXT_ATMODEM uint32 gul_localIP;                //LOCALIP.有效性同pppoe.(例:10.12.234.102对应0x66EA0C0A)




/*******************************************************************************
**表示modem操作步骤元素的结构体
********************************************************************************/
typedef uint8 (*CallBackElement)(void*);




/*******************************************************************************
**表示无线模块链表消息类别等参数
********************************************************************************/
typedef struct{
    uint8* sms;                         //短信
    uint8* number;                      //号码
    uint8  smslength;                   //短信长度
    uint8  fmt;                         //格式(数据编码方案TP-DCS,0-4-8分别表示7bit、8bit、16bit）
    uint8  numlength;                   //号码长度
}TModemSMS;

typedef struct{
    uint32 addr;                        //IP地址
    uint16 port;                        //端口号  
}TModemIPAddr;

typedef struct{
    String apn;                         //APN
    String usr;                         //USERNAME
    String pwd;                         //PASSWORD
}TModemLogin;

typedef struct{
    uint16 len;                         //长度
    uint8* buffer;                      //缓存
    uint16 rp;                          //当前可插入数据的位置
    uint16 process;                     //当前未处理数据的起始位置
}TModemIPData;



/*******************************************************************************
**表示无线猫处理函数
********************************************************************************/
typedef struct{
    int (*check)(void);                //确定是否为该类型的函数
    int (*uart_init)(void);
    void (*on)(void);                   //打开模块的过程
    void (*off)(void);                  //关闭模块的过程
    
    void (*doff)(void);                  //关闭模块的过程
    int (*status_check)(uint8_t *, uint32_t);
    int (*ip_init)(void);
    int (*get_ip)(uint8_t *);
    
    int (*domaintoip)(char * ,char *);
    int (*start)(sal_conn_t *);
    int (*close)(int , int32_t);
    int (*send)(int ,
                 uint8_t *,
                 uint32_t ,
                 char *,
                 int32_t ,
                 int32_t );
    int (*ftp_check)(uint8_t *, uint8_t *);    
    int (*fstart)(uint8_t *, uint16_t, uint8_t * fold);
    int (*ftp_get)(uint8_t * , uint8_t * , uint32_t , uint16_t * );
    
    int (*ftp_close)(void);
}TModemDrive;


typedef union{
    uint64 lword;
    struct{
        uint32 typeChecked:1;       //是否已识别模块类型
        uint32 chnrdy:1;            //AT指令是否完成基本配置并准备就绪
        uint32 signl:6;             //信号质量(0-31)
        uint32 regtt:1;             //是否已经注册成功
        uint32 sninfo:1;            //是否已经获取到模块和卡信息
        
        uint32 pppoe:1;             //PPP连接是否已经打开
        //uint32 ipcc:1;              //IP连接是否已经连上(包括TCP或UDP)
        //uint32 tcpcc:1;             //TCP连接是否已经连上
        //uint32 udpcc:1;             //UDP连接是否已经连上
        uint32 gsstt:1;             //GPRS server(tcp or udp) 侦听是否建立
        uint32 gsclncc1:1;           //GPRS server(tcp or udp) 是否 与 客户端1 的建立链接
        uint32 gsclncc2:1;           //GPRS server(tcp or udp) 是否 与 客户端2 的建立链接
        uint32 gsclnno:4;        //主动链接过来的客户端的链接号（当前的）
        uint32 gsclnoldno:4;        //主动链接过来的客户端的链接号（被取代的）
        uint32 gsurcreadno:4;           //主动上报的数据，所在的socket ID
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
}TModemState;

typedef struct __TModem{
//    TDev dev;
    const TModemDrive* drive;

    uint32 gsLinkIp[2];                 //GPRS server监听链接IP
    TModemIPAddr ipAddr;               //目的IP地址信息
    TModemState stt;
    
    uint8_t ModemIp[4];                     //GPRS本身的IP地址

}TModem;









/******************************************************************************
**用于记录发起复杂流程操作的应用层进程
*******************************************************************************/




/************************************************************************
 *无线猫参数,状态信息
 ************************************************************************/
EXT_ATMODEM TModem* DevModem;             //无线猫参数,状态信息


void SYS_MODM_Init(kbuf_queue_t *queue);




/************************************************************************
 * @function: SYS_MODM_ReadIP
 * @描述: 读取IP通道的数据
 * 
 * @参数: 
 * @param: buffer 要读取数据的缓存
 * @param: length 要读取数据的长度  
 * 
 * @返回: 
 * @return: uint16  
 * @说明: 
 * @作者: yzy (2013/11/6)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint16 SYS_MODM_ReadIP(uint8* buffer, uint16 length);



/************************************************************************
 * @function: SYS_MODM_CreateIP
 * @描述: 创建IP通道
 * 
 * @参数: 
 * @param: channel 通道号
 * @param: type 0:tcp 1:udp,目前没啥用
 * @param: address 目标地址
 * @param: port 目标地址端口
 * 
 * @返回: 
 * @return: TResult  
 * @说明: 
 * @作者: yzy (2013/11/1)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
TResult SYS_MODM_CreateIP(uint32 type, uint32 address, uint16 port);


/************************************************************************
 * @function: SYS_MODM_SendIP
 * @描述: 发送IP通道的数据
 * 
 * @参数: 
 * @param: ipdata 要发送的数据
 * @param: length 要发送的数据的长度
 * @param: channel 发送数据的通道
 * 
 * @返回: 
 * @return: TResult  
 * @说明: 
 * @作者: yzy (2013/11/6)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
TResult SYS_MODM_SendIP(uint8* ipdata, uint16 length);


/************************************************************************
 * @function: SYS_MODM_BeginCheck
 * @描述: 更新模块状态
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2013/11/1)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8_t SYS_MODM_BeginCheck(void);


/************************************************************************
 * @function: SYS_MODM_ClosePPP
 * @描述: 断开ppp通道 
 * @参数: 
 * 
 * @返回: 
 * @return: TResult  
 * @说明: 
 * @作者: yzy (2013/11/6)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
TResult SYS_MODM_ClosePPP(void);


/************************************************************************
 * @function: SYS_MODM_CreatePPP
 * @描述: 创建PPP通道
 * 
 * @参数: 
 * @param: apn 
 * @param: vname 
 * @param: vpassword 
 * 
 * @返回: 
 * @return: TResult  
 * @说明: 
 * @作者: yzy (2013/11/6)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
TResult SYS_MODM_CreatePPP(string apn, string vname, string vpassword);

/************************************************************************
 * @function: SYS_MODM_Reinit
 * @描述: 
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2013/11/1)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void SYS_MODM_Reinit(void);



/************************************************************************
 * @function: SYS_MODM_CloseIP
 * @描述: 断开IP通道 
 * 
 * @参数: 
 * @param: channel 要断开的通道
 * 
 * @返回: 
 * @return: TResult  
 * @说明: 
 * @作者: yzy (2013/11/6)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
TResult SYS_MODM_CloseIP(void);



/************************************************************************
 * @function: SYS_MODM_Reset
 * @描述: 复位AT通道 
 * 
 * @参数: 
 * @param: way 0:软件复位模块
               1:硬件复位模块
               2:软硬件都复位模块
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2013/11/6)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 SYS_MODM_Reset(uint8 way);


/************************************************************************
 * @function: SYS_MODM_Off
 * @描述: 关闭整个AT通道以及模块
 * @参数: 
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2013/11/1)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 SYS_MODM_Off(void);



/************************************************************************
 * @function: SYS_MODM_Status
 * @描述: 获取模块状态
 * @参数: 
 * 
 * @返回: 
 * @return: TModemState  
 * @说明: 
 * @作者: yzy (2013/11/1)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
TModemState SYS_MODM_Status(void);





/*******************************************************************************
** @function_name:  SYS_MODM_GetIP
** @function_file:  dev_modem.c
** @描述: 获取无线GPRS模块分配的IP地址
** 
** 
** @参数: 
** 
** @返回: 
** @return:  uint32   
** @作者: yzy (2012/6/20)
**-----------------------------------------------------------------------------
** @修改人: 
** @修改说明: 
*******************************************************************************/
uint8_t * SYS_MODM_GetIP(void);
uint8 SYS_MODM_Shutdown(void);



int HAL_FTP_Check(uint8_t * usr, uint8_t * pwd);

int HAL_FTP_Start(uint8_t *domain, uint16_t port, uint8_t * fold);
int HAL_FTP_Get(uint8_t * file, uint8_t * outbuf, uint32_t offset, uint16_t * len);
int HAL_FTP_Close(void);


#endif


