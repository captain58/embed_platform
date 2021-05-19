/****************************************Copyright (c)**************************
**                               	________科技有限公司
**                                     		开发部
**
**
**--------------文件信息--------------------------------------------------------
**文   件   名: realtime.c
**创   建   人: yzy
**最后修改日期: 2018年04月28日
**描        述: 实时时钟处理函数
**注        意:
**--------------历史版本信息----------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2018年04月28日
** 描　述: 原始版本
*******************************************************************************/
#define EXT_DEV_REALTIME
#include "rtc.h"
#include "hal_rtc_stm32l4.h"
#include "aos/include.h"
//	#include "k_types.h"
//	#include "k_api.h"
//	#include "lib_app.h"
//	#include "hal.h"
#include "tasks.h"




/******************************************************************************
**记录当前时间的变量
******************************************************************************/
TIME gs_Time;                               //当前时钟
TIME gs_TimeBk;                             //备份时钟



/******************************************************************************
**时钟操作资源
******************************************************************************/
ksem_t gs_RealTimeRes;
ktimer_t     g_s_timer;



/******************************************************************************
**是否允许从外部硬时钟校时
******************************************************************************/
bool f_timecheck;							//是否允许从外部硬时钟校时



#if (SYS_RTC_EN == 0)

TIME gs_vtime;                  //RTC不存在时的秒计数
    
/*******************************************************************************
 * @function_name:  VirtualTimeInit
 * @function_file:  realtime.c
 * @描述: 初始化虚拟RTC时间
 * 
 * 
 * @参数: 
 * @param:   
 * 
 * @返回: 
 * @return:    
 * @作者: yzy (2010-02-20)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
void VirtualTimeInit(void)
{   //2007年1月1日星期1
    TIME* time = &gs_vtime;
    time->sec = 0;
    time->min = 0;
    time->hour = 0;
    time->dmon = 1;
    time->month = 1;
    time->year = 7;
    time->dyear = 0;
    time->dweek = 1;
}


/*******************************************************************************
 * @function_name:  VirtualTimeSecProc
 * @function_file:  realtime.c
 * @描述: 虚拟RTC走时处理
 * 
 * 
 * @参数: 
 * @param:   
 * 
 * @返回: 
 * @return:    
 * @作者: yzy (2010-02-20)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
void VirtualTimeSecProc(void)
{
    TIME* time = &gs_vtime;
    const uint8* monthday;
    
    if((time->year & 0x03) == 0)        //判断今年是否为闰年
    {
        monthday = guc_BYearMonth;
    }
    else
    {
        monthday = guc_LYearMonth;
    }
    CPSR_ALLOC();
    RHINO_CPU_INTRPT_DISABLE();
                                            //秒更新
    time->sec++;
    if(time->sec >= 60)
    {
        time->sec = 0;
        time->min++;
    }
                                            //分更新
    if(time->min >= 60)
    {
        time->min = 0;
        time->hour++;
    }
                                            //小时更新
    if(time->hour >= 24)
    {
        time->hour = 0;
        time->dmon++;
        time->dweek++;
        time->dyear++;
    }
                                            //日更新
    if((time->dmon) == 0 || (time->dmon > monthday[time->month - 1]))
    {
        time->dmon = 1;
        time->month++;
    }
                                            //月更新,dyear更新
    if((time->month == 0) || (time->month > 12))
    {
        time->month = 1;
        time->dyear = 0;
        time->year++;
    }
                                            //年更新
    if(time->year > 99)
    {
        time->year = 0;
    }
    
    if(time->dweek > 6)                     //dweek更新
    {
        time->dweek = 0;
    }
    RHINO_CPU_INTRPT_ENABLE();
}


/*******************************************************************************
 * @function_name:  GetVirtualTime
 * @function_file:  realtime.c
 * @描述: 获取虚拟RTC时钟
 * 
 * 
 * @参数: 
 * @param:   
 * 
 * @返回: 
 * @return:    
 * @作者: yzy (2010-02-20)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
void GetVirtualTime(TIME* datetime)
{
    TIME* time = &gs_vtime;
    datetime->sec = time->sec;
    datetime->min = time->min;
    datetime->hour = time->hour;
    datetime->dmon = time->dmon;
    datetime->month = time->month;
    datetime->year = time->year;
    datetime->dweek = time->dweek;
    datetime->dyear = time->dweek;
}


/*******************************************************************************
 * @function_name:  SetVirtualTime
 * @function_file:  realtime.c
 * @描述: 设置虚拟RTC时钟
 * 
 * 
 * @参数: 
 * @param:   
 * 
 * @返回: 
 * @return:    
 * @作者: yzy (2010-02-20)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
void SetVirtualTime(TIME* datetime)
{
    //SYS_ENTER_AllSCRT();           //1)2015-3-27 yzy 删除
                                    //2)已有gs_RealTimeRes作为读写互斥.可删除.
                                    //3)该函数在_SystemTaskInitHook()中调用,不允许存在开启中断
    TIME* time = &gs_vtime;
    time->sec = datetime->sec;
    time->min = datetime->min;
    time->hour = datetime->hour;
    time->dmon = datetime->dmon;
    time->month = datetime->month;
    time->year = datetime->year;
    time->dweek = CalcWeek((uint8*)&datetime->dmon, 0);
    time->dyear = datetime->dweek;
    
    //SYS_EXIT_AllSCRT();
}


#endif

    
    
    
#if SYS_HRTC_EN > 0
/*******************************************************************************
 * @function_name:  SYS_RTC_CheckTime
 * @function_file:  realtime.c
 * @描述: 实时时钟校时
 * 
 * 
 * @参数: 
 * @param: pdata  
 * 
 * @返回: 
 * @return:  bool   
 * @作者: yzy (2010-02-20)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
uint8 SYS_RTC_CheckTime(void)
{
    uint8 buff[7];//ssmmhhwwddmmyy
    uint8 time[6];
    uint32 sec,sec1;
    TIME tm;
    
    if(I2C_HRTC_ReadHexTime(buff) == SYS_ERR_OK)             //读取硬时钟时间
    {
        MoveBuffer(buff, time, 3);               //秒分时
        MoveBuffer(buff + 4, time + 3, 3);       //日月年(跳过周)
        sec = CalcSecs(time, false);
        
        I2C_HRTC_ReadHexTime(buff);
        MoveBuffer(buff, time, 3);               //秒分时
        MoveBuffer(buff + 4, time + 3, 3);       //日月年(跳过周)
        sec1 = CalcSecs(time, false);
        if(AbsVal(sec1 - sec) <= 2)		        //两次读取硬件RTC的时间差在2秒以内才写入MCU的RTC
        {                                                   //转换数据格式
            tm.sec = buff[0];
            tm.min = buff[1];
            tm.hour = buff[2];
            tm.dmon = buff[4];
            tm.month = buff[5];
            tm.year = buff[6];
            return SYS_MCU_WriteDateTime(&tm);
        }
    }
    return SYS_ERR_FT;
}
#endif



/*******************************************************************************
 * @function_name:  SYS_RTC_SecProc
 * @function_file:  realtime.c
 * @描述: 实时时钟秒处理
 * 
 * 
 * @参数: 
 * @param: pdata  
 * 
 * @返回: 
 * @return:  bool   
 * @作者: yzy (2010-02-20)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
void SYS_RTC_SecProc(void *timer, void *arg)
{
#if (SYS_RTC_EN == 0)
    VirtualTimeSecProc();
#endif
    uint8_t msg = 0;
    //printf("SYS_RTC_SecProc = %d\n", gul_SysRunSecs);

                                            //读取内部RTC时间
    if(SYS_MCU_ReadDateTime(&gs_Time) != SYS_ERR_OK)
    {
        SYS_MCU_ReadDateTime(&gs_Time);
    }
                                            //系统运行时间增一
    gul_SysRunSecs++;
    
    extern const KTaskDeclare __TKDeclare[SYS_TK_NUM];
    const KTaskDeclare* dec = __TKDeclare;

    for(uint32 uc_i = 0; uc_i < SYS_TK_NUM; uc_i++, dec++)
    {
                                            //如果该进程不存在则不发送
        if(dec->ktask == __NULL || dec->stklen == 0)
        {
            continue;
        }
        
        if(!(guc_MsgApplied[uc_i] & Bit_Map8[MSG_CLS_TM]))
        {
            continue;
        }
        
        if(dec->ktask->tbl != 0 && dec->ktask->msg != NULL)             //时间消息仅针对消息进程
        {
            if(gs_TimeBk.year != gs_Time.year) //年事件
            {
//	                SYS_Message_Send(MSG_YEAR, uc_i);
                msg = MSG_YEAR;
                krhino_buf_queue_send(dec->ktask->msg, &msg, 1);
            }
            if(gs_TimeBk.month != gs_Time.month)//月事件
            {
//	                SYS_Message_Send(MSG_MON, uc_i);
                msg = MSG_MON;
                krhino_buf_queue_send(dec->ktask->msg, &msg, 1);
                
            }
            if(gs_TimeBk.dmon != gs_Time.dmon) //天事件
            {
//	                SYS_Message_Send(MSG_DAY, uc_i);
                msg = MSG_DAY;
                krhino_buf_queue_send(dec->ktask->msg, &msg, 1);

            }
            if(gs_TimeBk.hour != gs_Time.hour) //小时事件
            {
//	                SYS_Message_Send(MSG_HOUR, uc_i);
                msg = MSG_HOUR;
                krhino_buf_queue_send(dec->ktask->msg, &msg, 1);
                
            }
            if(gs_TimeBk.min != gs_Time.min)   //分事件
            {
				f_timecheck = true;			//过分后第30秒允许RTC校时
//	                SYS_Message_Send(MSG_MIN, uc_i);
                msg = MSG_MIN;
                krhino_buf_queue_send(dec->ktask->msg, &msg, 1);
                
            }
//	            SYS_Message_Send(MSG_SEC, uc_i); //发送秒消息
            msg = MSG_SEC;
            krhino_buf_queue_send(dec->ktask->msg, &msg, 1);
            
        }
    }
    
#if SYS_HRTC_EN > 0
    if((gs_Time.sec >= 30) && (gs_Time.sec <= 50) && f_timecheck)          	//在第30秒校时.每分钟校时一次.
    {
        SYS_RTC_CheckTime();
        f_timecheck = false;
    }
#endif
                                            //备份时间
    MoveBuffer((uint8*)&gs_Time, (uint8*)&gs_TimeBk, sizeof(TIME));


}

rtc_dev_t g_stRtcDev;

/*******************************************************************************
 * @function_name:  SYS_RTC_Init
 * @function_file:  realtime.c
 * @描述: 
 * 
 * @参数: 
 * @返回: 
 * @作者: yzy (2010-02-20)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
void SYS_RTC_Init(void)
{
#if (SYS_RTC_EN == 0)
    VirtualTimeInit();
#else
    g_stRtcDev.port = 0;
    g_stRtcDev.config.format = HAL_RTC_FORMAT_BCD;

    hal_rtc_init(&g_stRtcDev);

    //HAL_Init_RTC();
    //HAL_RTC_Start();
#endif
    
#if SYS_HRTC_EN > 0
    Init_I2C_HRTC(&gs_HrtcPort);            //初始化i2c总线
    SYS_RTC_CheckTime();
#endif

                                            //创建用户资源
//	    SYS_SEM_Create(1, &gs_RealTimeRes);
    krhino_sem_create(&gs_RealTimeRes, "RealTime", 1);

                                            //创建一个定时器(定时时间1s)
    gul_SysRunSecs = 0;
//	    SYS_Timer_Create(SYS_RTC_SecProc, __NULL, 100, TIMER_ID_RTC, false);
    krhino_timer_create(&g_s_timer, "sec_timer", SYS_RTC_SecProc,
                        krhino_ms_to_ticks(1000), krhino_ms_to_ticks(1000), 0, 1);
	                                        //初始化备份时间
    SYS_MCU_ReadDateTime(&gs_Time);          //
    MoveBuffer((uint8*)&gs_Time, (uint8*)&gs_TimeBk, sizeof(TIME));
    
    f_timecheck = true;					    //上电后就允许RTC校时

}

void SYS_RTC_Start(void)
{
//	#if (SYS_RTC_EN == 1)
//	    HAL_RTC_Start();
//	#endif

}

/*******************************************************************************
 * @function_name:  SYS_MCU_ReadBCDDateTime
 * @function_file:  realtime.c
 * @描述: 读取RTC的时间,格式BCD
 * @参数: 
 * @param: tm(TIME*): 时间结构体缓存
 * @返回: 
 * @作者: yzy (2010-11-16)
 ******************************************************************************/
uint8 SYS_MCU_ReadBCDDateTime(TIME * datetime)
{
    
//	    TDev* dev = SYS_DEV_Open(DEV_NO_SYS, 0, __NULL);
//	    if(dev != __NULL)
//	    {                                       //获取时间
//	        ((TDevSys*)dev->devfunc)->GetTime(datetime);
//	        SYS_DEV_Close(dev);
//	        return SYS_ERR_OK;
//	    }
    
    rtc_time_t rtc_t;

    if(!hal_rtc_get_time(&g_stRtcDev,&rtc_t))
    {
        datetime->sec = rtc_t.sec;
        datetime->min = rtc_t.min;
        datetime->hour = rtc_t.hr;
        datetime->dmon = rtc_t.date;
        datetime->month = rtc_t.month;
        datetime->year = rtc_t.year;
        datetime->dweek = rtc_t.weekday;
        return SYS_ERR_OK;
    }
    return SYS_ERR_FT;
}

/*******************************************************************************
 * @function_name:  SYS_MCU_ReadDateTime
 * @function_file:  realtime.c
 * @描述: 读取RTC的时间,格式HEX
 * 
 * @参数: 
 * @param: tm(TIME*): 时间结构体缓存
 * @返回: 
 * @作者: yzy (2010-11-16)
 ******************************************************************************/
uint8 SYS_MCU_ReadDateTime(TIME* datetime)
{ 
#if (SYS_RTC_EN == 0)
    GetVirtualTime(datetime);
    return SYS_ERR_OK;
#else
    if(SYS_MCU_ReadBCDDateTime(datetime) == SYS_ERR_OK)
    {
        datetime->sec = ByteBcdToHex(datetime->sec);
        datetime->min = ByteBcdToHex(datetime->min);
        datetime->hour = ByteBcdToHex(datetime->hour);
        datetime->dmon = ByteBcdToHex(datetime->dmon);
        datetime->month = ByteBcdToHex(datetime->month);
        datetime->year = ByteBcdToHex(datetime->year);
        datetime->dweek = datetime->dweek;
//	        datetime->dyear = IntHexToBcd(datetime->dyear);
        return SYS_ERR_OK;
    }
    return SYS_ERR_FT;
#endif    
}

/*******************************************************************************
 * @function_name:  SYS_MCU_WriteDateTime
 * @function_file:  dev_realtime.c
 * @描述: 设置ARM RTC时间
 * 
 * 
 * @参数: 
 * @param: datetime 时间结构体,其中的秒,分,时,日,月,年必须正确
 * 
 * @返回: 
 * @return:  uint8   
 * @作者: yzy (2011-02-21)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
uint8 SYS_MCU_WriteDateTime(TIME * datetime)
{
    const uint8* monthday;
    rtc_time_t rtc_t;
                                            //判断设置数据的合法性
    if(datetime->year > 99 || datetime->month == 0 || 
       datetime->month > 12 || datetime->dmon == 0 || 
       datetime->hour >= 24 || datetime->min >= 60 || 
       datetime->sec >= 60)
    {
        return SYS_ERR_FT;
    }
    
    if((datetime->year & 0x03) == 0)        //判断今年是否为闰年
    {
        monthday = guc_BYearMonth;
    }
    else
    {
        monthday = guc_LYearMonth;  
    }
                                            //日的合法性
    if(datetime->dmon > monthday[datetime->month - 1])
    {
        return SYS_ERR_FT;
    }
                                            //当前为一年中的第几天
//	    datetime->dyear = gui_MonthBeginDay[datetime->month - 1] + datetime->dmon - 1;
//	                                            //判断今年是否为闰年,是否已过2月
//	    if((datetime->year & 0x03) == 0 && datetime->month > 2)
//	    {
//	        datetime->dyear++;
//	    }
    datetime->dweek = CalcWeek((uint8*)&datetime->dmon, 1);
    
#if (SYS_RTC_EN == 0)
    SetVirtualTime(datetime);
    return SYS_ERR_OK;
    
#else
                                            //打开设备
//	    TDev* dev = SYS_DEV_Open(DEV_NO_SYS, 0, __NULL);
//	    if(dev != __NULL)
//	    {                                       //设置ARMRTC时间
//	        ((TDevSys*)dev->devfunc)->SetTime(datetime);
//	        SYS_DEV_Close(dev); 
//	        return SYS_ERR_OK;
//	    }
    rtc_t.sec = ByteHexToBcd(datetime->sec);
    rtc_t.min = ByteHexToBcd(datetime->min);
    rtc_t.hr = ByteHexToBcd(datetime->hour);
    rtc_t.date = ByteHexToBcd(datetime->dmon);
    rtc_t.month = ByteHexToBcd(datetime->month);
    rtc_t.year = ByteHexToBcd(datetime->year);
    rtc_t.weekday = datetime->dweek;
    

    if(!hal_rtc_set_time(&g_stRtcDev,(const rtc_time_t*)&rtc_t))
    {
        return SYS_ERR_OK;
    }

    return SYS_ERR_FT;
#endif
}

/*******************************************************************************
 * @function_name:  SYS_MCU_WriteBCDDateTime
 * @function_file:  dev_realtime.c
 * @描述: 设置ARM RTC时间(BCD格式)
 * 
 * 
 * @参数: 
 * @param: datetime 时间结构体BCD格式,其中的秒,分,时,日,月,年必须正确
 * 
 * @返回: 
 * @return:  uint8   
 * @作者: yzy (2011-02-21)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
uint8 SYS_MCU_WriteBCDDateTime(TIME* datetime)
{
    datetime->sec = ByteBcdToHex(datetime->sec);
    datetime->min = ByteBcdToHex(datetime->min);
    datetime->hour = ByteBcdToHex(datetime->hour);
    datetime->dmon = ByteBcdToHex(datetime->dmon);
    datetime->month = ByteBcdToHex(datetime->month);
    datetime->year = ByteBcdToHex(datetime->year);
    datetime->dweek = datetime->dweek;
//	    datetime->dyear = IntBcdToHex(datetime->dyear);
    return SYS_MCU_WriteDateTime(datetime);
}





/*******************************************************************************
 * @function_name:  RTC_ProcTEsMsg
 * @function_file:  realtime.c
 * @描述: 实时时钟处理进程链表消息处理函数
 * 
 * 
 * @参数: 
 * @param: OpType 操作类型  HRTC_RW_RB等
 * @返回: 
 * @作者: yzy (2010-02-20)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
uint8 RTC_ProcTEsMsg(uint8 OpType, TIME* obj)
{
#if SYS_HRTC_EN > 0
    uint8 time[10];                         //时钟缓存
#endif
    uint8 result = SYS_ERR_FT;
    
	switch(OpType)                         //根据消息操作类型非别处理
    {

        case HRTC_RW_RB:                    //读取BCD格式时间
            result = SYS_MCU_ReadBCDDateTime(obj);
            break;
            
        case HRTC_RW_RH:                    //读取HEX格式时间
            result = SYS_MCU_ReadDateTime(obj);
            break;
            
        case HRTC_RW_WB:                    //写入BCD格式时间
#if SYS_HRTC_EN > 0
            MoveBuffer((uint8*)obj, time, 3);//秒分时
            MoveBuffer((uint8*)obj + 3, time + 4, 3);//日月年
            time[3] = CalcWeek(time + 4, 0);          //周
            result = I2C_HRTC_WriteBcdTime(time);
#endif
            result |= SYS_MCU_WriteBCDDateTime(obj);      //同时更新RTC时间,字节顺序一致TIME类型和该time[]
            break;
            
        case HRTC_RW_WH:                    //写入HEX格式时间
#if SYS_HRTC_EN > 0
            MoveBuffer((uint8*)obj, time, 3);//秒分时
            MoveBuffer((uint8*)obj + 3, time + 4, 3);//日月年
            time[3] = CalcWeek(time + 4, 0);          //周
            result = I2C_HRTC_WriteHexTime(time);
#endif
            result |= SYS_MCU_WriteDateTime((TIME*)obj); 
            break;
            
        default:
            break;
    
    }
    
    return result;

}


/*******************************************************************************
 * @function_name:  SYS_HRTC_WriteDateTime
 * @function_file:  realtime.c
 * @描述: 设置时间,格式HEX
 * 
 * @参数: 
 * @param: tm(TIME*): 时间结构体缓存
 * @返回: 
 * @作者: yzy (2010-11-16)
 ******************************************************************************/
uint8 SYS_WriteDateTime(TIME* datetime)
{
    uint8 result;
    
    krhino_sem_take(&gs_RealTimeRes, RHINO_WAIT_FOREVER);
    result = RTC_ProcTEsMsg(HRTC_RW_WH, datetime);
    krhino_sem_give(&gs_RealTimeRes);
    return result;
}

/*******************************************************************************
 * @function_name:  SYS_HRTC_ReadDateTime
 * @function_file:  realtime.c
 * @描述: 读取时间,格式HEX
 * 
 * @参数: 
 * @param: tm(TIME*): 时间结构体缓存
 * @返回: 
 * @作者: yzy (2010-11-16)
 ******************************************************************************/
uint8 SYS_ReadDateTime(TIME* datetime)
{
    uint8 result;
    krhino_sem_take(&gs_RealTimeRes, RHINO_WAIT_FOREVER);
    result = RTC_ProcTEsMsg(HRTC_RW_RH, datetime);
    krhino_sem_give(&gs_RealTimeRes);
    return result; 
}


/*******************************************************************************
 * @function_name:  SYS_HRTC_ReadDateTime
 * @function_file:  realtime.c
 * @描述: 读取外部RTC的时间,格式BCD 
 * @参数: 
 * @param: tm(TIME*): 时间结构体缓存
 * @返回: 
 * @作者: yzy (2010-11-16)
 ******************************************************************************/
uint8 SYS_ReadBCDDateTime(TIME* datetime)
{
    uint8 result;
    krhino_sem_take(&gs_RealTimeRes, RHINO_WAIT_FOREVER);
    result = RTC_ProcTEsMsg(HRTC_RW_RB, datetime);
    krhino_sem_give(&gs_RealTimeRes);
    return result; 
}

/*******************************************************************************
 * @function_name:  SYS_HRTC_ReadDateTime
 * @function_file:  realtime.c
 * @描述: 设置外部RTC的时间,格式BCD
 * @参数: 
 * @param: tm(TIME*): 时间结构体缓存
 * @返回: 
 * @作者: yzy (2010-11-16)
 ******************************************************************************/
uint8 SYS_WriteBCDDateTime(TIME* datetime)
{
    uint8 result;
    krhino_sem_take(&gs_RealTimeRes, RHINO_WAIT_FOREVER);
    result = RTC_ProcTEsMsg(HRTC_RW_WB, datetime);
    krhino_sem_give(&gs_RealTimeRes);
    return result; 
}





/*******************************************************************************
 * @function_name:  GetTime
 * @function_file:  realtime.c
 * @描述: 获取当前的时间和日期的全部信息, 这个信息只读
 * 
 * 
 * @参数: 
 * @param: 
 * @返回: 当前时间结构体
 * @作者: yzy (2010-02-20)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
const TIME* GetTime(void)
{
    return (const TIME*)&gs_Time;
}



/*******************************************************************************
 * @function_name:  GetSysRunSecs
 * @function_file:  realtime.c
 * @描述: 获取系统连续运行时间
 * 
 * 
 * @参数: 
 * @param: 
 * @返回: 当前系统运行总秒数
 * @作者: yzy (2010-02-20)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
uint32 GetSysRunSecs(void)
{
    return gul_SysRunSecs;
}

