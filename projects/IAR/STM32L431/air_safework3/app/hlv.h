/****************************************Copyright (c)**************************************************
**                                  ____科技有限公司
**                                          开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: hlv.h
**创   建   人: yzy
**最后修改日期: 2019年6月1日
**描        述: hlv进程
**注        意:
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2019年6月1日
** 描　述: 原始版本
********************************************************************************************************/

#ifndef _HLV_H_
#define _HLV_H_



#ifndef EXT_HLV
#define EXT_HLV extern
#endif



/************************************************************************
**安全保存数据所需要的启动时间
*************************************************************************/
#define SYS_SAFE_SECS    15



//EXT_HLV SysVar gs_SysVar;


/****************************************************************************
**表示系统状态以及系统状态控制计数器
******************************************************************************/
#define HLV_STT_NENG    0x01               //停电工作状态-欠压
#define HLV_STT_USBE    0x02               //USB插拔状态
#define HLV_STT_USBP    0x04               //USB升级状态
#define HLV_STT_UTSTF   0x08               //USB测试失败
#define HLV_STT_UTSTS   0x10               //USB测试成功
#define HLV_STT_LENG    0x20               //停电工作状态-低压







/************************************************************************
**当前系统分钟数
*************************************************************************/

EXT_HLV S_TimeLabel gs_SysLastDayTime;

/************************************************************************
**第一类冻结标志(bit0-2表示3个不同抄表端口)
*************************************************************************/
EXT_HLV uint8 guc_HourSaveFlag;
EXT_HLV uint8 guc_DaySaveFlag;
EXT_HLV uint8 guc_MonSaveFlag;
EXT_HLV uint8 guc_RecSaveFlag;

/************************************************************************
**第二类冻结标志(bit0-2表示0分,15分,30分,45分曲线记录点)
*************************************************************************/
EXT_HLV uint8 guc_QuarSaveFlag;


typedef struct
{
    uint16 num;
    uint8 update[128];          //1:收到,0:未收到
}S_UPDATAMSG;
EXT_HLV S_UPDATAMSG  gs_UpDtMsg;











//fram块号宏
#define FM_MIS     0                            //gs_FramMisData(0)  27/32 = 1
//#define FM_RECPT   1                            //抄表日数据冻结指针(1-4)
#define FM_PARASTT 2                            //参数状态(1)       32/32 = 1
//#define FM_TMSTT   10                           //gs_TmStatData(10)
//#define FM_VLSTT   11                           //gs_VolStatData(11)
//#define FM_TASK    2                           //2类任务上报时间(78-88)
#define FM_EVTDT    3                        //停电发生时间（2-5）  //  5*24/32 = 4







/************************************************************************
**inp参数\数据结构体 
**(线制,pt,ct,零线ct,额定电压,额定电流,线制亦包含在 gs_InpExtraData 中,但未被赋值使用) 
*************************************************************************/
typedef struct
{
    uint8  evtbit[8];                           //事件记录有效标志位
    uint8  evtrank[8];                          //事件重要性等级标志位
}S_InpExtraVar;

EXT_HLV S_InpExtraVar gs_InpVarData;


typedef struct
{
    uint8  gbbreak;                             //广播打断抄表(因为抄表时间很长,且抄表期间不处理其他消息)
    uint16 gbbvsec;                             //广播打断抄表倒计时(防止长期打断抄表)
    uint32 cbday;                               //抄表日 BS
    uint8  cbhour;                              //抄表时辰 bin
    uint8  cbmin;                               //抄表时刻 bin
    uint8  netitv;                              //终端抄表间隔(1-60)
    uint8  cbqjn;                               //允许抄表时段数
    uint16 cbqjv[24][2];                        //终端抄表允许区间
    uint8  cten;                                //广播校时使能
    uint8  ctdhm[3];                            //广播校时时间
}S_InpExtraCB;

EXT_HLV S_InpExtraCB gs_InpExtracb[MCB_PORT_NUM];
  

/************************************************************************
**
*************************************************************************/
typedef union
{
    uint8 bytes[32];
    struct
    {
        S_TimeLabel stime;
        S_TimeLabel rtime;
        uint8 eft[4];
        uint16 crc;
    }seg;
}S_DATA_SEG_TEMPLET;





/************************************************************************
**抄表日冻结数据模板信息
*************************************************************************/
#define SIZE_RECDAY_DAYS        31
#define MAX_LEN_RECDAY_SEG      128




/************************************************************************
**抄表日冻结数据段0信息
*************************************************************************/
#define LEN_RECDAY_SEG0     96
#if(LEN_RECDAY_SEG0 > MAX_LEN_RECDAY_SEG)
    #error "rec seg alloc err"
#endif

typedef union
{
    uint8 bytes[LEN_RECDAY_SEG0];
    struct
    {
        S_TimeLabel stime;
        S_TimeLabel rtime;
        uint32 Wp[5];
        uint8 eft[4];
        uint16 crc;
    }seg;

}S_RECDATA_SEG0;


#define EFT_RECDAY_WP  0

/************************************************************************
**抄表日冻结数据段2信息
*************************************************************************/
#define LEN_RECDAY_SEG2    128
#if(LEN_RECDAY_SEG2 > MAX_LEN_RECDAY_SEG)
    #error "rec seg alloc err"
#endif

typedef union
{
    uint8 bytes[LEN_RECDAY_SEG2];
    struct
    {
        S_TimeLabel stime;
        S_TimeLabel rtime;
        uint32 dpp[5];                  //正向有功需量
        uint32 dppt[5];                 //正向有功需量时间(AFMT)
        uint32 dnp[5];                  //反向有功需量
        uint32 dnpt[5];                 //反向有功需量时间(AFMT)
        uint8 eft[4];
        uint16 crc;
    }seg;
}S_RECDATA_SEG2;

#define EFT_RECDAY_DPP   0
#define EFT_RECDAY_DPPT  1
#define EFT_RECDAY_DNP   2
#define EFT_RECDAY_DNPT  3





/************************************************************************
**日冻结数据模板
*************************************************************************/
#define SIZE_DAY_DAYS       31
#define MAX_LEN_DAY_SEG     128




/************************************************************************
**日冻结数据段0信息
*************************************************************************/
#define LEN_DAY_SEG0    96
#if(LEN_DAY_SEG0 > MAX_LEN_DAY_SEG)
    #error "day seg alloc err"
#endif

typedef union
{
    uint8 bytes[LEN_DAY_SEG0];
    struct
    {
        S_TimeLabel stime;  //12 字节
        S_TimeLabel rtime;  //12字节
        uint32 Wp[5];                   //正有功电量    20字节
        uint32 Wn[5];                   //反有功电量    20字节
        uint8 eft[4];       //4字节
        uint16 crc;         //2字节
    }seg;
}S_DAYDATA_SEG0; //70 字节


#define EFT_DAY_WP  0
#define EFT_DAY_WN  1



/************************************************************************
**日冻结数据段2信息
*************************************************************************/
#define LEN_DAY_SEG2    128
#if(LEN_DAY_SEG2 > MAX_LEN_DAY_SEG)
    #error "day seg alloc err"
#endif

typedef union
{
    uint8 bytes[LEN_DAY_SEG2];
    struct
    {
        S_TimeLabel stime;      //12字节
        S_TimeLabel rtime;      //12字节
        uint32 dpp[5];                  //正向有功需量      20字节
        uint32 dppt[5];                 //正向有功需量时间(AFMT)    20字节
        uint32 dnp[5];                  //反向有功需量      20字节
        uint32 dnpt[5];                 //反向有功需量时间(AFMT)      20字节
        uint8 eft[4];   //4字节
        uint16 crc;     //2字节
    }seg;
}S_DAYDATA_SEG2;  //110


#define EFT_DAY_DPP   0
#define EFT_DAY_DPPT  1
#define EFT_DAY_DNP   2
#define EFT_DAY_DNPT  3









/************************************************************************
**月冻结数据模板
*************************************************************************/
#define SIZE_MON_MONS    13
#define MAX_LEN_MON_SEG  MAX_LEN_DAY_SEG


/************************************************************************
**月冻结数据段0信息
*************************************************************************/
#define LEN_MON_SEG0     LEN_DAY_SEG0

#define S_MONDATA_SEG0   S_DAYDATA_SEG0

#define EFT_MONTH_WP     EFT_DAY_WP
#define EFT_MONTH_WN     EFT_DAY_WN


/************************************************************************
**月冻结数据段0信息
*************************************************************************/
#define LEN_MON_SEG2    LEN_DAY_SEG2

#define S_MONDATA_SEG2  S_DAYDATA_SEG2

#define EFT_MON_DPP     EFT_DAY_DPP
#define EFT_MON_DPPT    EFT_DAY_DPPT
#define EFT_MON_DNP     EFT_DAY_DNP
#define EFT_MON_DNPT    EFT_DAY_DNPT










/************************************************************************
**小时冻结数据模板
*************************************************************************/
#define SIZE_HOUR_HOURS     288       //12天 * 24小时/天
#define MAX_LEN_HOUR_SEG     56



/************************************************************************
**小时冻结数据段0信息
*************************************************************************/
#define LEN_HOUR_SEG0    56
#if(LEN_HOUR_SEG0 > MAX_LEN_HOUR_SEG)
    #error "hour seg alloc err"
#endif

typedef union
{
    uint8 bytes[LEN_HOUR_SEG0];
    struct
    {
        S_TimeLabel stime;
        S_TimeLabel rtime;
        uint32 Wp;
        uint32 Wn;
        uint32 I;
        uint16 U[3];
        uint8 eft[2];
        uint16 crc;
    }seg;
}S_HOURDATA_SEG0;


#define EFT_HOUR_WP 0
#define EFT_HOUR_WN 1
#define EFT_HOUR_I  2
#define EFT_HOUR_U  3

/************************************************************************
**15分钟曲线冻结数据模板
*************************************************************************/
#define SIZE_CUR_POINTS  26784          //(288* 31 * 3)(21* 24 * 4)
#define LEN_CUR_SEG0     92

typedef union                       
{
   uint8 bytes[LEN_CUR_SEG0];
   struct
   {
        S_TimeLabel stime;                  //12 字节
        uint32  rtime;                      //4 字节，抄表时间
        uint8_t dev;
        uint8_t factory_id[2];
        uint8_t factory_model[2];
        uint8_t ST[12];                       //标况总量    高位补0   S-standard  T-total
        uint8_t WT[8];                       //工况总量   working condition total
        uint8_t SI[8];                       //标况瞬时量standard     insaneous quantity
        uint8_t WI[8];                       //工况瞬时量   working    insaneous quantity
        uint8_t TMP[8];                       //温度
        uint8_t P[8];                       //压力 
        uint8 eft[2];                       //2字节
        uint16  crc;                        //2字节
   }seg;

   struct
   {
        S_TimeLabel stime;                  //12 字节
        uint32  rtime;                      //4 字节，抄表时间
        uint8_t dev;
        uint8_t factory_id[2];
        uint8_t factory_model[2];
        uint8_t pflag;          //预付费功能  1开启，0未开启
        uint8_t ST[6];                       //标况总量    高位补0   S-standard  T-total
        uint8_t RM[6];                       //剩余金额  remain money
        uint8_t UP[6];                       //当前单价 unit price
        uint8_t STT[4];                       //表具状态
        uint8_t vol;                       //锂/蓄电池电压 voltage
        uint8_t per;                       //锂/蓄电池电量百分比 percent
        uint8_t mflag;                  //流量计监控数据包（1-有/0-没有） monitor flag
        uint8_t ST1[6];                       //标况总量    高位补0   S-standard  T-total
        uint8_t WT[6];                       //工况总量   working condition total
        uint8_t SI[6];                       //标况瞬时量standard     insaneous quantity
        uint8_t WI[6];                       //工况瞬时量   working    insaneous quantity
        uint8_t TMP[6];                       //温度
        uint8_t P[6];                       //压力 
        
        uint8 eft[2];                       //2字节
        uint16  crc;                        //2字节
   }pseg;//prepay
}S_CURDATA_SEG0;        //92字节

#define EFT_MIN_WP  0
#define EFT_MIN_WPF 1
#define EFT_MIN_WN  2
#define EFT_MIN_WNF 3
#define EFT_MIN_I   4
#define EFT_MIN_IF  5
#define EFT_MIN_U   6
#define EFT_MIN_UF  7
//#define EFT_MIN_P   8

#define EFT_MIN_ST  0
#define EFT_MIN_WT 1
#define EFT_MIN_SI  2
#define EFT_MIN_WI 3
#define EFT_MIN_TMP   4
#define EFT_MIN_P  5
//	#define EFT_MIN_PST  6
//	#define EFT_MIN_PWT 7
//	#define EFT_MIN_PSI  8
//	#define EFT_MIN_PWI 9
//	#define EFT_MIN_PTMP   10
#define EFT_MIN_PRICE  11
#define EFT_MIN_RM 12
#define EFT_MIN_MSTT  13
#define EFT_MIN_VOLH  14//vol percent had flow

/************************************************************************
**终端电压采集15分钟曲线冻结数据模板
*************************************************************************/
#define SIZE_QUAR_POINTS  2976          //(31* 24 * 4)
#define LEN_QUAR_SEG0     24

typedef union                       
{
   uint8 bytes[LEN_QUAR_SEG0];
   struct
   {
        S_TimeLabel stime;
        uint32  vol;
        uint16  crc;
   }seg;
}S_QUARDATA_SEG0;
 



/************************************************************************
**统计类历史冻结数据模板
*************************************************************************/
#define SIZE_CBVOL_STAT    31
#define LEN_CBVOL_SEG0     128
     /************************************************************************ 
**电能表电压数据统计
*************************************************************************/
typedef struct
{
     S_TimeLabel rtime;
     uint16  utop_mins[3];                      //电压越上上限累计时间
     uint16  uup_mins[3];                       //电压越上限累计时间
     uint16  ucom_mins[3];                      //合格电压累计时间
     uint16  udn_mins[3];                       //电压下限累计时间
     uint16  ubtn_mins[3];                      //电压越下下限累计时间
     uint32  Vmax[3];                           //最大电压
     uint8   MaxTime[3][3];                     //最大电压发生时间
     uint32  Vmin[3];                           //最小电压
     uint8   MinTime[3][3];                     //最小电压发生时间
     uint32  usum[3];                           //电压累加和.用于计算平均电压
}S_CBStat;

typedef union
{
    uint8  bytes[LEN_CBVOL_SEG0];
    struct
    {
        S_TimeLabel stime;
        S_CBStat  bank1;
        uint16    crc;
    }seg;
}S_CBVOL_SEG0;
    




/************************************************************************
**各测量点的实时数据
*************************************************************************/
typedef struct
{
    uint32 Wp[5];                       //正向总\4费率有功电能示值  hex4.2
    uint32 Wn[5];                       //反向总\4费率有功电能示值  hex4.2
    uint32 I[3];                        //A相电流                   hex4.3
    uint16 U[3];                        //A相电压                   hex2.4
    int32  P;                           //总功率                    shex4.4
    uint8  time[6];                     //电能表日历时钟            6个字节
    uint8  evsta[4];                    //事件状态      4字节
    uint8  eft[5];                      //状态字                    BS
    uint8  errcnt;                      //抄表失败次数
    S_TimeLabel rtime;                  //抄表时间

}S_TstRealData;
typedef struct
{
    uint8  mGateIn;                     //遥信类型.按位0:常开.1常闭
    uint8  mState;                      //当前的事件状态
    uint8  mRaise;                      //产生事件状态位
    uint8  mFall;                       //恢复事件状态位
    uint8  mValid;                      //先前状态是否有效
    uint8  mb;                          //状态量变位标志BS8
}MKStruct;


EXT_HLV MKStruct gs_MKStruct;


EXT_HLV S_TstRealData gss_TstRealData[CORE_TST_NO]; //测量点实时数据

EXT_HLV uint8 guc_StopCnt[CORE_TST_NO];  //电能表停走时间计数器

#define ADDR_WP     (uint32)((uint8*)gss_TstRealData[0].Wp - (uint8*)&gss_TstRealData[0])
#define ADDR_WN     (uint32)((uint8*)gss_TstRealData[0].Wn - (uint8*)&gss_TstRealData[0])
#define ADDR_U      (uint32)((uint8*)gss_TstRealData[0].U - (uint8*)&gss_TstRealData[0])
#define ADDR_I      (uint32)((uint8*)gss_TstRealData[0].I - (uint8*)&gss_TstRealData[0])
#define ADDR_TIME   (uint32)((uint8*)&gss_TstRealData[0].time - (uint8*)&gss_TstRealData[0])
#define ADDR_P      (uint32)((uint8*)&gss_TstRealData[0].P - (uint8*)&gss_TstRealData[0])
#define ADDR_EVS    (uint32)((uint8*)&gss_TstRealData[0].evsta - (uint8*)&gss_TstRealData[0])


/************************************************************************ 
**实时数据有效位定义
*************************************************************************/
#define EFT_WP      (0)
#define EFT_WN      (EFT_WP + 5)
#define EFT_U       (EFT_WN + 5)
#define EFT_I       (EFT_U + 3)
#define EFT_TIME    (EFT_I + 3)
#define EFT_P       (EFT_TIME + 1)
#define EFT_EVS     (EFT_P + 1)




/************************************************************************
**各测量点的负荷曲线
*************************************************************************/
//	typedef struct
//	{
//	    uint32 Wp[4];                       //正向总\三分相有功电能示值  hex4.2
//	    uint32 Wn[4];                       //反向总\三分相有功电能示值  hex4.2
//	    uint16 U[9];                        //相电压                   hex2.4
//	    uint16 I[9];                        //相电流                   hex4.3
//	    int32  P[2];                        //总功率                    shex4.4
//	    uint8  eft[8];                      //有效位
//	    uint8  ymdhm[5];                    //读取时间
//	}S_TstCurveData;
#pragma pack(1)
typedef struct
{
    uint8_t dev;
    uint8_t factory_id[2];
    uint8_t factory_model[2];
    uint8_t ST[12];                       //标况总量    高位补0   S-standard  T-total
    uint8_t WT[8];                       //工况总量   working condition total
    uint8_t SI[8];                       //标况瞬时量standard     insaneous quantity
    uint8_t WI[8];                       //工况瞬时量   working    insaneous quantity
    uint8_t TMP[8];                       //温度
    uint8_t P[8];                       //压力 
    uint8  ymdhm[5];                    //读取时间
    uint8_t RM[6];                       //剩余金额  remain money
    uint8_t UP[6];                       //当前单价 unit price
    uint8_t STT[4];                       //表具状态
    uint8_t vol;                       //锂/蓄电池电压 voltage
    uint8_t per;                       //锂/蓄电池电量百分比 percent
    uint8_t mflag;                  //流量计监控数据包（1-有/0-没有） monitor flag


    
    uint8  eft[8];                      //有效位
}S_TstCurveData;

//	typedef struct
//	{
//	    uint8_t dev;
//	    uint8_t factory_id[2];
//	    uint8_t factory_model[2];
//	    uint8_t pflag;          //预付费功能  1开启，0未开启
//	    uint8_t ST[6];                       //标况总量    高位补0   S-standard  T-total
//	    uint8_t RM[6];                       //剩余金额  remain money
//	    uint8_t UP[6];                       //当前单价 unit price
//	    uint8_t STT[4];                       //表具状态
//	    uint8_t vol;                       //锂/蓄电池电压 voltage
//	    uint8_t per;                       //锂/蓄电池电量百分比 percent
//	    uint8_t mflag;                  //流量计监控数据包（1-有/0-没有） monitor flag
//	    uint8_t ST1[6];                       //标况总量    高位补0   S-standard  T-total
//	    uint8_t WT[6];                       //工况总量   working condition total
//	    uint8_t SI[6];                       //标况瞬时量standard     insaneous quantity
//	    uint8_t WI[6];                       //工况瞬时量   working    insaneous quantity
//	    uint8_t TMP[6];                       //温度
//	    uint8_t P[6];                       //压力 
//	
//	    uint8 eft[8];                       //2字节
//	}S_TstCurPreData;


#pragma pack(4)

EXT_HLV S_TstCurveData gss_TstCurveData[CORE_TST_NO]; //测量点实时数据

//	#define ADDR_CUR_WP     (uint32)((uint8*)gss_TstCurveData[0].Wp - (uint8*)&gss_TstCurveData[0])
//	#define ADDR_CUR_WN     (uint32)((uint8*)gss_TstCurveData[0].Wn - (uint8*)&gss_TstCurveData[0])
//	#define ADDR_CUR_U      (uint32)((uint8*)gss_TstCurveData[0].U - (uint8*)&gss_TstCurveData[0])
//	#define ADDR_CUR_I      (uint32)((uint8*)gss_TstCurveData[0].I - (uint8*)&gss_TstCurveData[0])
//	#define ADDR_CUR_P      (uint32)((uint8*)&gss_TstCurveData[0].P - (uint8*)&gss_TstCurveData[0])



/************************************************************************ 
**负荷曲线数据有效位定义
*************************************************************************/
#define EFT_CUR_WP      (0)
#define EFT_CUR_WN      (EFT_CUR_WP + 4)
#define EFT_CUR_U       (EFT_CUR_WN + 4)
#define EFT_CUR_I       (EFT_CUR_U + 9)
#define EFT_CUR_P       (EFT_CUR_I + 9)



/************************************************************************
**各测量点的需量数据
*************************************************************************/
//	typedef struct
//	{
//	    uint32 dpp[5];                  //正向有功需量
//	    uint32 dppt[5];                 //正向有功需量时间(AFMT)
//	    uint32 dnp[5];                  //反向有功需量
//	    uint32 dnpt[5];                 //反向有功需量时间
//	    uint8  eft[4];                  //有效位
//	    uint8  ymdhm[5];                //读取时间
//	}S_TstDmdData;

//	EXT_HLV S_TstDmdData gs_tstdmd[MCB_PORT_NUM];//存储时使用
//	
//	#define ADDR_DMD_DPP     (uint32)((uint8*)gs_tstdmd[0].dpp  - (uint8*)&gs_tstdmd[0])
//	#define ADDR_DMD_DPPT    (uint32)((uint8*)gs_tstdmd[0].dppt - (uint8*)&gs_tstdmd[0])
//	#define ADDR_DMD_DNP     (uint32)((uint8*)gs_tstdmd[0].dnp  - (uint8*)&gs_tstdmd[0])
//	#define ADDR_DMD_DNPT    (uint32)((uint8*)gs_tstdmd[0].dnpt - (uint8*)&gs_tstdmd[0])
//	
//	#define EFT_DMD_DPP       0
//	#define EFT_DMD_DPPT      1
//	#define EFT_DMD_DNP       2
//	#define EFT_DMD_DNPT      3


/************************************************************************
**各测量点的的事件数据
*************************************************************************/
typedef struct
{
   uint32 pcnt;                           //编程次数                  bin
   uint8  progt[6];                       //最近一次编程时间 
   uint8  htime[6];                       //最近一次合闸时间
   uint8  dtime[6];                       //最近一次开闸时间
   uint32 dncnt;                          //开端钮盒次数              
   uint32 wgcnt;                          //开表尾盖次数              bin
   uint8  wgtime[6];                      //最近一次开表尾盖时间
   uint32 jscnt;                          //最近一次校时次数          bin
   uint8  jstime[12];                     //校时前后时间
   uint32 dpcnt;                          //时段表编程次数            bin
   uint8  dptime[6];                      //时段表最近一次段时间编程时间
   uint32 demdcnt;                        //清最大需量次数
   uint8  demdt[6];                       //最近清需量时间
   uint8  ftn[24];                        //费率时段                  
   uint16 dh[3];                          //抄表日                    97:I0
   uint32 imp0;                           //有功常数                  bin
   uint32 imp1;                           //无功常数                  bin
   uint32 dxn[4];                         //断相次数                   
   uint32 tdcnt;                          //停电次数
   uint32 sycnt;                          //失压次数
   uint8  bstt[14];                       //电表状态字
   uint32 ccount;                         //恒定磁场干扰总次数
   uint32 ptbb;                           //电压互感器变比
   uint32 ctbb;                           //电流互感器变比
   uint32 baty;                           //电池工作时间              bin
   uint8  eft[6];                         //有效数据标志
   uint8  ymdhm[5];                       //抄表时间
}S_EvtRealData;

EXT_HLV S_EvtRealData gs_TstEvtData[MCB_PORT_NUM];   //抄读时使用

#define ADDR_EVT_PCNT          (uint32)((uint8*)&gs_TstEvtData[0].pcnt     -   (uint8*)&gs_TstEvtData[0])
#define ADDR_EVT_PROGT         (uint32)((uint8*)&gs_TstEvtData[0].progt    -   (uint8*)&gs_TstEvtData[0])
#define ADDR_EVT_HTIME         (uint32)((uint8*)&gs_TstEvtData[0].htime    -   (uint8*)&gs_TstEvtData[0])
#define ADDR_EVT_DTIME         (uint32)((uint8*)&gs_TstEvtData[0].dtime    -   (uint8*)&gs_TstEvtData[0])
#define ADDR_EVT_DNCNT         (uint32)((uint8*)&gs_TstEvtData[0].dncnt    -   (uint8*)&gs_TstEvtData[0])
#define ADDR_EVT_WGCNT         (uint32)((uint8*)&gs_TstEvtData[0].wgcnt    -   (uint8*)&gs_TstEvtData[0])
#define ADDR_EVT_WGTIME        (uint32)((uint8*)&gs_TstEvtData[0].wgtime   -   (uint8*)&gs_TstEvtData[0])
#define ADDR_EVT_JSCNT         (uint32)((uint8*)&gs_TstEvtData[0].jscnt    -   (uint8*)&gs_TstEvtData[0])
#define ADDR_EVT_JSTIME        (uint32)((uint8*)&gs_TstEvtData[0].jstime   -   (uint8*)&gs_TstEvtData[0])
#define ADDR_EVT_DPCNT         (uint32)((uint8*)&gs_TstEvtData[0].dpcnt    -   (uint8*)&gs_TstEvtData[0])
#define ADDR_EVT_DPTIME        (uint32)((uint8*)&gs_TstEvtData[0].dptime   -   (uint8*)&gs_TstEvtData[0])
#define ADDR_EVT_DEMDCNT       (uint32)((uint8*)&gs_TstEvtData[0].demdcnt  -   (uint8*)&gs_TstEvtData[0])
#define ADDR_EVT_DEMDT         (uint32)((uint8*)&gs_TstEvtData[0].demdt    -   (uint8*)&gs_TstEvtData[0])
#define ADDR_EVT_FTN           (uint32)((uint8*)&gs_TstEvtData[0].ftn      -   (uint8*)&gs_TstEvtData[0])
#define ADDR_EVT_DH            (uint32)((uint8*)&gs_TstEvtData[0].dh       -   (uint8*)&gs_TstEvtData[0])
#define ADDR_EVT_IMP0          (uint32)((uint8*)&gs_TstEvtData[0].imp0     -   (uint8*)&gs_TstEvtData[0])
#define ADDR_EVT_IMP1          (uint32)((uint8*)&gs_TstEvtData[0].imp1     -   (uint8*)&gs_TstEvtData[0])
#define ADDR_EVT_DXN           (uint32)((uint8*)&gs_TstEvtData[0].dxn      -   (uint8*)&gs_TstEvtData[0])
#define ADDR_EVT_TDCNT         (uint32)((uint8*)&gs_TstEvtData[0].tdcnt    -   (uint8*)&gs_TstEvtData[0])
#define ADDR_EVT_SYCNT         (uint32)((uint8*)&gs_TstEvtData[0].sycnt    -   (uint8*)&gs_TstEvtData[0])
#define ADDR_EVT_BSTT          (uint32)((uint8*)&gs_TstEvtData[0].bstt     -   (uint8*)&gs_TstEvtData[0])
#define ADDR_EVT_CCOUNT        (uint32)((uint8*)&gs_TstEvtData[0].ccount   -   (uint8*)&gs_TstEvtData[0])
#define ADDR_EVT_PTBB          (uint32)((uint8*)&gs_TstEvtData[0].ptbb     -   (uint8*)&gs_TstEvtData[0])
#define ADDR_EVT_CTBB          (uint32)((uint8*)&gs_TstEvtData[0].ctbb     -   (uint8*)&gs_TstEvtData[0])
#define ADDR_EVT_BATY          (uint32)((uint8*)&gs_TstEvtData[0].baty     -   (uint8*)&gs_TstEvtData[0])


#define EFT_EVT_PCNT               (0)
#define EFT_EVT_HTIME              (EFT_EVT_PCNT + 1)
#define EFT_EVT_DTIME              (EFT_EVT_HTIME + 1)
#define EFT_EVT_PROGT              (EFT_EVT_DTIME + 1)
#define EFT_EVT_DNCNT              (EFT_EVT_PROGT + 1)
#define EFT_EVT_WGCNT              (EFT_EVT_DNCNT + 1)
#define EFT_EVT_WGTIME             (EFT_EVT_WGCNT + 1)
#define EFT_EVT_JSCNT              (EFT_EVT_WGTIME + 1)
#define EFT_EVT_JSTIME             (EFT_EVT_JSCNT + 1)
#define EFT_EVT_DPCNT              (EFT_EVT_JSTIME + 1)
#define EFT_EVT_DPTIME             (EFT_EVT_DPCNT + 1)
#define EFT_EVT_DEMDCNT            (EFT_EVT_DPTIME + 1)
#define EFT_EVT_DEMDT              (EFT_EVT_DEMDCNT + 1)
#define EFT_EVT_FTN                (EFT_EVT_DEMDT + 1)
#define EFT_EVT_DH                 (EFT_EVT_FTN + 1)
#define EFT_EVT_IMP0               (EFT_EVT_DH + 3)
#define EFT_EVT_IMP1               (EFT_EVT_IMP0 + 1)
#define EFT_EVT_DXN                (EFT_EVT_IMP1 + 1)
#define EFT_EVT_TDCNT              (EFT_EVT_DXN + 1)
#define EFT_EVT_SYCNT              (EFT_EVT_TDCNT + 1)
#define EFT_EVT_BSTT               (EFT_EVT_SYCNT + 1)
#define EFT_EVT_CCOUNT             (EFT_EVT_BSTT + 1)
#define EFT_EVT_PTBB               (EFT_EVT_CCOUNT + 1)
#define EFT_EVT_CTBB               (EFT_EVT_PTBB + 1)
#define EFT_EVT_BATY               (EFT_EVT_CTBB + 1)



/************************************************************************
**07规约测量点的表内冻结数据
*************************************************************************/
typedef struct
{
    uint8  Tsd[5];                      //07表上次日冻结时间        mhDMY(BCD)
    uint32 Wpd[5];                      //07表上日正向总\4费率有功电能示值  hex4.2
    uint32 Wnd[5];                      //07表上日反向总\4费率有功电能示值  hex4.2
    uint8  Tsd2[5];                     //07表上二日冻结时间
    uint32 Wpd2[5];                     //07表上日正向总\4费率有功电能示值  hex4.2
    uint32 Wnd2[5];                     //07表上日反向总\4费率有功电能示值  hex4.2
    uint8  Tsd3[5];                     //07表上二日冻结时间
    uint32 Wpd3[5];                     //07表上日正向总\4费率有功电能示值  hex4.2
    uint32 Wnd3[5];                     //07表上日反向总\4费率有功电能示值  hex4.2
    uint8  eft[6];
    S_TimeLabel rtime;

}S_Tst07ExtraData;

EXT_HLV S_Tst07ExtraData gss_Tst07ExtraData;

#define ADDR_EXT_TSD     (uint32)((uint8*)gss_Tst07ExtraData.Tsd - (uint8*)&gss_Tst07ExtraData)
#define ADDR_EXT_WPD     (uint32)((uint8*)gss_Tst07ExtraData.Wpd - (uint8*)&gss_Tst07ExtraData)
#define ADDR_EXT_WND     (uint32)((uint8*)gss_Tst07ExtraData.Wnd - (uint8*)&gss_Tst07ExtraData)
#define ADDR_EXT_TSD2    (uint32)((uint8*)gss_Tst07ExtraData.Tsd2 - (uint8*)&gss_Tst07ExtraData)
#define ADDR_EXT_WPD2    (uint32)((uint8*)gss_Tst07ExtraData.Wpd2 - (uint8*)&gss_Tst07ExtraData)
#define ADDR_EXT_WND2    (uint32)((uint8*)gss_Tst07ExtraData.Wnd2 - (uint8*)&gss_Tst07ExtraData)
#define ADDR_EXT_TSD3    (uint32)((uint8*)gss_Tst07ExtraData.Tsd3 - (uint8*)&gss_Tst07ExtraData)
#define ADDR_EXT_WPD3    (uint32)((uint8*)gss_Tst07ExtraData.Wpd3 - (uint8*)&gss_Tst07ExtraData)
#define ADDR_EXT_WND3    (uint32)((uint8*)gss_Tst07ExtraData.Wnd3 - (uint8*)&gss_Tst07ExtraData)

#define EFT_EXT_TSD       0
#define EFT_EXT_WPD       (EFT_EXT_TSD + 1)
#define EFT_EXT_WND       (EFT_EXT_WPD + 5)
#define EFT_EXT_TSDS      (EFT_EXT_WND + 5)
#define EFT_EXT_WPDS      (EFT_EXT_TSDS + 1)
#define EFT_EXT_WNDS      (EFT_EXT_WPDS + 5)
#define EFT_EXT_TSDT      (EFT_EXT_WNDS + 5)
#define EFT_EXT_WPDT      (EFT_EXT_TSDT + 1)
#define EFT_EXT_WNDT      (EFT_EXT_WPDT + 5)


/************************************************************************
**DLMS规约测量点的表内冻结数据
*************************************************************************/
typedef struct
{
    uint32 Wp[5];                      //DLMS表上日正向总\4费率有功电能示值  hex4.2
    uint32 Wn[5];                      //DLMS表上日反向总\4费率有功电能示值  hex4.2
    uint8  eft[2];
    S_TimeLabel rtime;

}S_TstDLMSExtraData;

EXT_HLV S_TstDLMSExtraData gss_TstDLMSExtraData;


#define ADDR_DLMS_EXT_WP     (uint32)((uint8*)gss_TstDLMSExtraData.Wp - (uint8*)&gss_TstDLMSExtraData)
#define ADDR_DLMS_EXT_WN     (uint32)((uint8*)gss_TstDLMSExtraData.Wn - (uint8*)&gss_TstDLMSExtraData)


#define EFT_DLMS_EXT_WP       0
#define EFT_DLMS_EXT_WN       (EFT_DLMS_EXT_WP + 5)



/************************************************************************
**数据类别
*************************************************************************/
typedef enum
{
    DTP_UNVALID = 0 ,
    DTP_WP          ,
    DTP_WN          ,
    DTP_U           ,
    DTP_I           ,
    DTP_TIME        ,
    DTP_P           ,
    DTP_EVS         ,
    DTP_DMD_DPP     ,
    DTP_DMD_DPPT    ,
    DTP_DMD_DNP     ,
    DTP_DMD_DNPT    ,
    DTP_EVT_PCNT    ,
    DTP_EVT_HTIME   ,
    DTP_EVT_DTIME   ,
    DTP_EVT_PROGT   ,
    DTP_EVT_DNCNT   ,
    DTP_EVT_WGCNT   ,
    DTP_EVT_WGTIME  ,
    DTP_EVT_JSCNT   ,
    DTP_EVT_JSTIME  ,
    DTP_EVT_DPCNT   ,
    DTP_EVT_DPTIME  ,
    DTP_EVT_DEMDCNT ,
    DTP_EVT_DEMDT   ,
    DTP_EVT_FTN     ,
    DTP_EVT_DH      ,
    DTP_EVT_IMP0    ,
    DTP_EVT_IMP1    ,
    DTP_EVT_DXN     ,
    DTP_EVT_TDCNT   ,
    DTP_EVT_SYCNT   ,
    DTP_EVT_BSTT    ,
    DTP_EVT_CCOUNT  ,
    DTP_EVT_PTBB    ,
    DTP_EVT_CTBB    ,
    DTP_EVT_BATY    ,
    DTP_EXT_TSD     ,
    DTP_EXT_WPD     ,
    DTP_EXT_WND     ,
    DTP_EXT_TSDS    ,
    DTP_EXT_WPDS    ,
    DTP_EXT_WNDS    ,
    DTP_EXT_TSDT    ,
    DTP_EXT_WPDT    ,
    DTP_EXT_WNDT    ,
}e_DItype;











/************************************************************************
 * @function: UpdateSysTimeStruct
 * @描述: 更新gs_SysTime和gs_SysLastDayTime
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2013/10/30)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void UpdateSysTimeStruct(void);


/*********************************************************************************************************
** 函数名称: void HLV_PreInit(void* pdata)
** 功能描述: 高级扫描进程的初始化函数，并调用具体应用初始化钩子函数
** 输　入:
            pdata 没有意义

** 输　出: 无
**
** 全局变量: 无
** 调用模块: 无
**
** 作　者: yzy
** 日　期: 2007年9月4日
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void HLV_PreInit(void);
#endif


