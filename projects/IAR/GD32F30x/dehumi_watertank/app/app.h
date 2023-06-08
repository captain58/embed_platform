/****************************************Copyright (c)**************************************************
**                                  ________科技有限公司
**                                          开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: main.h
**创   建   人: yzy
**最后修改日期: 2013年6月8日
**描        述: 应用程序主进程文件
**注        意:
**--------------历史版本信息----------------------------------------------------------------------------
**创建人: yzy
**版  本: v1.0
**日　期: 2013年6月8日
**描　述: 原始版本
**-------------------------------------------------------------------------------------------------------
** 修改人: 
** 版  本: 
** 描  述: 
********************************************************************************************************/
#ifndef _MAIN_H_
#define _MAIN_H_

#ifndef EXT_MAIN
#define EXT_MAIN  extern
#endif



/*******************************************************************************
**用户程序版本号
********************************************************************************/
extern const uint32 gul_UsrFuncVer;
extern const uint8 gucs_PrjCode[6];




EXT_MAIN volatile uint8 guc_debug;
EXT_MAIN volatile uint16 gui_debug;
EXT_MAIN volatile uint32 gul_debug;

#define CORE_TST_NO     8
#define LEN_TST_CFG     8
#define LEN_TST_BMPS    2
#define LEN_PREPAY_CFG     25
#define LEN_RECHARGE_INFO     27
#define LEN_PRICEADJUST_INFO     52

/*******************************************************************************
**表示不同进程是否已经回复保活消息的计数器
**yzy 目前由于优先级消息数量有限,不采用消息来操作变量,而是多进程直接共享(只做简单保护).
********************************************************************************/
EXT_MAIN uint8 gucs_LiveCnt[SYS_TK_NUM]; //排除最高系统进程和最低空闲进程



//#define gul_UsrFuncVer  0x00007003
//#define gul_Scy200Ver   0x20000046



/******************************************************************************
**系统运行状态字
******************************************************************************/
EXT_MAIN uint32 gl_SysRunStt;
//	EXT_MAIN uint32 gl_SysRunInit = 0;

#define HRS_PARALD  0x00000001                  //上电已载入参数
#define HRS_FIRSTM  0x00000002                  //上电首次过分
#define HRS_APPRUN  0x00000004                  //是否允许应用进程运行(应用进程在启动后须在该位置位后进入消息处理流程)




/******************************************************************************
**相关错误LOG记录
******************************************************************************/
typedef struct
{
    uint16 errcnt_amiload;                  //
    uint16 fls_errcnt;                      //Flash读写出错次数
    uint16 fram_errcnt;                     //铁电读写出错次数
    uint16 hrtc_errcnt;					    //硬件RTC时钟出错次数
}ERRLOG;


EXT_MAIN ERRLOG gs_ErrLog;


/******************************************************************************
**各进程初始化完成标志数组
******************************************************************************/
EXT_MAIN uint8 gucs_TaskInitStt[8];


/****************************************************************************
**表示系统状态以及系统状态控制计数器
******************************************************************************/
#define HLV_STT_NENG    0x01               //停电工作状态
typedef union{
    uint32_t lword;
    struct{
        uint32_t pwrstt:2;              //电压状态 00:正常01欠压10低压
        uint32_t rtccheck:1;           //rtc状态0正常1：异常
        uint32_t rtcstt:1;              //时间状态0：已校时1：未校时
        uint32_t flashstt:1;            //存储状态0正常，1异常
        uint32_t blestt:1;              //蓝牙状态0正常，1异常
        uint32_t blecheck:1;              //蓝牙通讯检测0正常，1异常
        uint32_t ipcheck:1;              //ip通讯检测通讯检测0正常，1异常
        
        uint32_t modemstt:2;              //gsm模块状态0：未识别1：已识别2：已注册4：已链接
        uint32_t csq:4; 
        uint32_t DI0linked:1;             //DI1   DI连接状态 0:断开 1:连接
        uint32_t DI1linked:1;             //DI2   DI连接状态 0:断开 1:连接
        
        uint32_t dev1linked:1;            //设备1   485连接状态 0:断开 1:连接
        uint32_t dev2linked:1;            //设备2   485连接状态 0:断开 1:连接
        uint32_t dev3linked:1;            //设备3   485连接状态 0:断开 1:连接
        uint32_t dev4linked:1;            //设备4   485连接状态 0:断开 1:连接
        uint32_t dev5linked:1;            //设备5   485连接状态 0:断开 1:连接
        uint32_t dev6linked:1;            //设备6   485连接状态 0:断开 1:连接
        uint32_t dev7linked:1;            //设备7   485连接状态 0:断开 1:连接
        uint32_t dev8linked:1;            //设备8   485连接状态 0:断开 1:连接        
        uint32_t extpwrstt:1;             //市电状态 0:断开 1:连接    
        uint32_t door:1;                  //门禁开关 0: 开 1:关
        uint32_t ftpstt:2;                  //ftp文件获取状态
        uint32_t chk4851:1;                  //485-1检测结果
        uint32_t chk4852:1;                  //485-2检测结果
        uint32_t chk4853:1;                  //485-3检测结果
        uint32_t chk4854:1;                  //485-4检测结果
    }bit;
}TTerState;
typedef struct
{
    uint8  mDGcnt;                       //停电进入低功耗倒计时
    uint8  mDGstt;                       //系统状态
    uint16 mLPsysstt;                       //低功耗系统状态
    uint16 mLPstt;                       //进入低功耗的进程的状态
    TTerState terstt;
    uint8_t AI0;                         //模拟检测口1电压
    uint8_t AI1;                         //模拟检测口2电压
    uint8_t batVal;
    uint8 netpRegNum;
    uint16_t netpTsk;
}SysVar;

EXT_MAIN SysVar gs_SysVar;

#define CON_SENSOR_FILTER 3
typedef struct _ST_WATER_SENSOR_
{
    uint8_t low;
    uint8_t low_mid;
    uint8_t high_mid;
    uint8_t high;
}ST_WATER_SENSOR;



typedef struct _ST_WATER_STT_
{
    uint8_t motor_stt;
    uint8_t cur_stt;
    uint8_t last_stt;
    uint8_t ready_stt[CON_SENSOR_FILTER];
    uint8_t para_save_flag;
    uint8_t remain_set_flag;
    uint8_t time_set_flag;   
    uint8_t time_set_site;   
    uint16_t para_save_tick;
    uint16_t count;
    uint32_t tick;
    ST_WATER_SENSOR st_sensor;
}ST_WATER_STT;


typedef struct _ST_WATER_CTRL_
{
    uint8_t onoff;
    uint8_t auto_manmual;
    uint8_t remain_day;
    uint8_t pump_stt;       //抽水泵状态
    uint8_t depump_stt;     //加水泵状态
//    uint8_t remain_set_flag;
//    uint8_t time_set_flag;
    uint8_t reserver[5];
    uint16_t crc;
}ST_WATER_CTRL;

/************************************************************************
**时间标签
*************************************************************************/
typedef struct
{
    uint32 mins;
    uint8  YMDhms[6];
}S_SysTime;     //12字节


//	EXT_MAIN S_SysTime gs_SysTime;
EXT_MAIN S_TimeLabel gs_SysTime;



/*******************************************************************************
 * @function_name:  __WaitForAllTaskReady
 * @function_file:  main.c
 * @描述: 等待其他进程初始化完毕
 * 
 * 
 * @参数: 
 * 
 * @返回: 
 * @作者: yzy (2018年3月22日)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
void __WaitForAllTaskReady(void);


/*******************************************************************************
 * @function_name:  SYS_MAIN_Init
 * @function_file:  main.c
 * @描述: 应用层main初始化
 * 
 * 
 * @参数: 
 * 
 * @返回: 
 * @作者: yzy (2011-03-03)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
void SYS_MAIN_Init(void);



/*******************************************************************************
 * @function_name:  HB_TaskLiveReq
 * @function_file:  main.c
 * @描述: 发送保活请求
 * 
 * 
 * @参数: 
 * 
 * @返回: 
 * @作者: yzy (2011-03-28)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
uint8_t HB_TaskLiveReq(int count);



/*******************************************************************************
 * @function_name:  HB_RetLive
 * @function_file:  
 * @描述: 其它进程的保活处理
 * 
 * 
 * @参数:
 * 
 * @返回: 
 * @作者: yzy (2012-02-16)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
void HB_RetLive(uint8 tkid);



/*******************************************************************************
 * @function_name:  HB_LiveInit
 * @function_file:  
 * @描述: 进程保活机制初始化
 * 
 * 
 * @参数:
 * 
 * @返回: 
 * @作者: yzy (2012-02-16)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
void HB_LiveInit(void);




void ResumeSys(void);


#endif

