
/****************************************Copyright (c)**************************************************
**                                  ____科技有限公司
**                                          开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: farp.c
**创   建   人: yzy
**最后修改日期: 2017年9月1日
**描        述: 主通道处理函数
**注        意:
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2017年9月1日
** 描　述: 原始版本
********************************************************************************************************/

#ifndef _FARP_H_
#define _FARP_H_

#ifndef EXT_FARP
#define EXT_FARP extern
#endif


/******************************************************************************
**以太网联网参数
******************************************************************************/
typedef struct
{
    uint8 ip[4];
    uint8 mark[4];
    uint8 gateway[4];
    uint8 mac[6];
}TEthPara;




/*******************************************************************************
**无线猫状态字
*******************************************************************************/
#define		WMS_STT_GSM			    LBIT(0)      //通道GSM状态  
#define		WMS_STT_PPP			    LBIT(1)      //通道PPP状态
#define		WMS_STT_CLN			    LBIT(2)      //数据通道  TCP客户端通道
#define		WMS_STT_SER			    LBIT(3)      //数据通道  TCP服务器通道
#define		WMS_STT_SER_CNT1 	    LBIT(4)      //数据通道  TCP服务器 与 客户端1 建立链接
#define		WMS_STT_SER_CNT2 	    LBIT(5)      //数据通道  TCP服务器 与 客户端2 建立链接
#define		WMS_STT_FTP 	        LBIT(6)      //FTP

#define     WMS_CHALLSTT        (WMS_STT_GSM | WMS_STT_PPP | WMS_STT_CLN | WMS_STT_SER | WMS_STT_SER_CNT1 | WMS_STT_SER_CNT2)


#define     WMS_EHSTT           LBIT(0x19)   //以太网状态
#define     WMS_ETH0STT         LBIT(0x1A)   //以太网通道0状态



typedef struct 
{
    uint8 clnen;                             //客户端开启使能（1 使能 0 不使能 ）
    uint8 seren;                             //服务器开启使能（1 使能 0 不使能  ）
    uint8 wkmd;                             //工作模式(0永久在线1被动激活)
    
    uint8_t login;                          //登录帧是否发送成功

    uint8_t paired;
    uint8_t sleep;                          //休眠状态

    uint8 rdct;                             //重拨次数
    uint8 rtct;                             //重试次数
    uint8 ntpc;                             //连接方式,(00:TCP_01:UDP)
    uint8 mcsq;                             //信号质量
    uint16 hbcy;                            //心跳周期(s)
    //uint16 rdtm;                            //重拨间隔
    uint16 rttm;                            //重试间隔
    uint16 slep;                            //无数据休眠时间
    uint32 dcip;                            //主站地址
    uint16 dcpt;                            //主站端口号
    uint8 gapn[30];                         //APN
    uint8 user[30];                         //专网用户名
    uint8 word[30];                         //专网密码

    uint32 dcib;                            //主站备用地址
    uint16 dcpb;                            //主站备用端口号
    uint16 lispt;                           //服务器侦听端口
    uint8  lisflg[2];                       //收到的两个客户端链接的状态（0：未连接，1：链接）
    uint8_t modemip[4];                         //终端的IP地址
    uint32 wmsv;                            //远程无线猫程序版本号
    uint32 wmst;                            //无线猫状态字
    //uint8 simc[4];                          //终端地址
    uint8 smsc[17];                         //短信服务中心(未使用)
    uint8 smdc[17];                         //短信数据中心(主站号码)
    uint8 smfc[17];                         //短信维护中心(最近下发命令号码)
    uint8 ldlen;                            //登录帧长度
    uint8 ld[64];                           //登录帧
    uint8 lodlen;                           //退出登陆帧长度
    uint8 lod[64];                          //退出登陆帧
    uint8 hklen;                            //心跳帧长度
    uint8 hkd[64];                          //心跳帧
}FarpVar;

EXT_FARP FarpVar gs_FarpVar;                //远程端口的参数



/******************************************************************************
**接收到的目标短信号码
******************************************************************************/
EXT_FARP uint8 guc_RecvSmgNo[17];



/******************************************************************************
**主备用主站地址登陆异常标志
**(IP创建成功置异常,收到国网确认帧后请异常标志.//只允许置一位,每次登陆前判断异常位来选择登陆主IP还是备用IP)
******************************************************************************/
EXT_FARP uint8 guc_FarpLoginConfirmErr;     //b0:主用ip异常,b1:备用ip异常
EXT_FARP uint32 gul_FarpLoginDeadTime;      //等待确认截止时间,单位为全局秒


/******************************************************************************
**被动激活模式无数据休眠计数器
******************************************************************************/
EXT_FARP uint16 gui_FarpBkTmCnt;


/************************************************************************
**函数名称: void Farp_SetWkmd(void)
**描 述: 无线模块工作模式的设定
**
**输 入: 
**输 出: 
**
**备 注: 
**-----------------------------------------------------------------------
**作 者: yzy
**日 期: 09-09-15
*************************************************************************/
void Farp_SetWkmd(void);

/************************************************************************
 * @function: Farp_SendIpData
 * @描述: 远程通道发送GPRS的IP数据
 * 
 * @参数: 
 * @param: buffer 发送缓存
 * @param: len 发送的长度
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 Farp_SendIpData(uint8* buffer, uint16 len);


/************************************************************************
 * @function: Farp_SendSMS
 * @描述: 远程通道发送短消息
 * 
 * @参数: 
 * @param: buffer 待发送的短信内容缓存.
 * @param: len 待发送的短信内容长度
 * @param: format 发送的短信格式
 * @param: type 目标短信号码类别,
 *                  0: 从哪里来到哪里去
 *                  1: 短信数据中心
 *                  2. 短信维护中心
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 Farp_SendSMS(uint8* buffer, uint16 len, uint8 format, uint8 type);


/************************************************************************
 * @function: Farp_SendEthData
 * @描述: 远程通道发送ETH的IP数据
 * 
 * @参数: 
 * @param: buffer 发送缓存
 * @param: len 发送的长度
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 Farp_SendEthData(uint8* buffer, uint16 len);

/************************************************************************
 * @function: Farp_SendTcpServerData
 * @描述: 远程通道发送GPRS的IP数据
 * 
 * @参数: 
 * @param: buffer 发送缓存
 * @param: len 发送的长度
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 Farp_SendTcpServerData(uint8* buffer, uint16 len);

/************************************************************************
 * @function: Farp_PreInit
 * @描述: 远程参数初始化
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void Farp_PreInit(void);
int IP_Check(void);
intptr_t Farp_WM_Connect_Once(void);
void Farp_Close_PPP(void);

#define FTP_DOMAIN_MAX_LEN  64
#define FTP_PARA_MAX_LEN  32

/************************************************************************
终端通信参数
*************************************************************************/
typedef struct
{
    uint8   repwait;                    //终端等待从动站响应超时时间(秒)
    uint8   reptry;                     //终端等待从动站响应重发次数(0-3)
    uint8   AckServ;                    //需要主站确认的服务(B0-2:1.2.3类数据)
    uint8   rdct;                       //重拨次数
    uint8   wkms;                       //终端的工作模式（0-2 混合模式、客户机模式、服务器模式）
    uint8   fpms;                       //工作模式(1-3:永久在线,被动激活,时段在线)
    uint8   wksd[3];                    //在线时段标志
    uint8   Addr[6];                    //行政区划码+终端地址
    uint8_t device[21];
    uint8   pwd[16];                    //终端密码
    uint8   ausk;                       //消息认证方案(255表示硬件专用方案, 254表示自定义密码认证)
    uint16  auth;                       //消息认证参数(未作定义)
    uint16  zdz[8];                     //八组组地址
    uint16_t cp_interval;
    uint8_t ftp_domain[FTP_DOMAIN_MAX_LEN];
    uint8_t ftp_user[FTP_PARA_MAX_LEN];
    uint8_t ftp_pwd[FTP_PARA_MAX_LEN];
    uint16_t port;
}S_PSTPARA;



typedef struct 
{
    uint8_t flag;
    uint8_t err_times;
    TIME time;
    uint8_t ftp_domain[FTP_DOMAIN_MAX_LEN];
    uint16_t port;
    uint8_t ftp_fold[FTP_DOMAIN_MAX_LEN];
    uint8_t ftp_file[FTP_PARA_MAX_LEN];
    uint8_t ftp_user[FTP_PARA_MAX_LEN];
    uint8_t ftp_pwd[FTP_PARA_MAX_LEN];
    uint32_t fileLen;
    uint32_t getLen;
}S_FTPPARA;

EXT_FARP    S_PSTPARA gs_PstPara;
EXT_FARP    S_FTPPARA gs_FtpPara;


#define FTP_STEP_IDEL       0x00
#define FTP_STEP_START      0x11
#define FTP_STEP_ING        0x22
#define FTP_STEP_FINISH     0x33
#define FTP_STEP_WAIT       0x44
#define FTP_STEP_OPEN       0x55
#define FTP_STEP_REBOOT     0x66
#define FTP_STEP_FAILED       0x77




#endif


