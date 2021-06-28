#ifndef _FARP_RF_STAR_
#define _FARP_RF_STAR_

#ifndef EXT_FARP
#define EXT_FARP extern
#endif

typedef struct 
{
    uint8_t wkmd;                             //工作模式(0低功耗     1非低功耗)
    
    uint8_t login;                          //登录帧是否发送成功

    uint8_t paired;

    uint8 rdct;                             //重拨次数
    uint8 rtct;                             //重试次数
    uint8 mcsq;                             //信号质量
    uint16 hbcy;                            //心跳周期(s)
    //uint16 rdtm;                            //重拨间隔
    uint16 rttm;                            //重试间隔
    uint16 slep;                            //无数据休眠时间
    uint32 dcip;                            //主站地址
    uint16 dcpt;                            //主站端口号

    uint8_t modemip[4];                         //终端的IP地址
    uint32 wmsv;                            //远程无线猫程序版本号
    uint32 wmst;                            //无线猫状态字

    uint8 ldlen;                            //登录帧长度
    uint8 ld[64];                           //登录帧
    uint8 lodlen;                           //退出登陆帧长度
    uint8 lod[64];                          //退出登陆帧
    uint8 hklen;                            //心跳帧长度
    uint8 hkd[64];                          //心跳帧
}FarpVar;

EXT_FARP FarpVar gs_FarpVar;                //远程端口的参数


#endif
