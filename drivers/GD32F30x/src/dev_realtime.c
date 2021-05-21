/******************************Copyright(c)******************************
**                         
**
** File Name: dev_realtime.c
** Author: 实时时钟设备
** Date Last Update: 2019-11-02
** Description: 
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: 文件创建
*************************************************************************/
#define EXT_DEV_REALTIME
#ifdef __MODULE__
#include "WOSsys.h"
#endif
#include "sys.h"
#include "hal.h"
#include "bsp.h"
//	#include "string.h"
#include "apis.h"

/******************************************************************************
**记录当前时间的变量
******************************************************************************/
TIME gs_Time;                               //当前时钟
TIME gs_TimeBk;                             //备份时钟



/******************************************************************************
**时钟操作资源
******************************************************************************/
TESRes gs_RealTimeRes;



/******************************************************************************
**是否允许从外部硬时钟校时
******************************************************************************/
bool f_timecheck;							//是否允许从外部硬时钟校时



#if (SYS_RTC_EN == 0)

TIME gs_vtime;                  //RTC不存在时的秒计数
    

/************************************************************************
 * @Function: _virtualTimeInit
 * @Description: 初始化虚拟RTC时间
 * @Arguments: 
 * @Note: 
 * @Auther: yzy
 * Date: 2015/6/1
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void _virtualTimeInit(void)
{   //2007年1月1日星期1
    TIME* time = &gs_vtime;
    time->sec = 0;
    time->min = 0;
    time->hour = 0;
    time->day = 1;
    time->month = 1;
    time->year = 7;
    time->dyear = 0;
    time->week = 1;
}



/************************************************************************
 * @Function: VirtualTimeSecProc
 * @Description: 虚拟RTC走时处理
 * @Arguments: 
 * @Note: 
 * @Auther: yzy
 * Date: 2015/6/1
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void _virtualTimeSecProc(void)
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
    SYS_ENTER_SCRT();
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
        time->day++;
        time->week++;
        time->dyear++;
    }
                                            //日更新
    if((time->day) == 0 || (time->day > monthday[time->month - 1]))
    {
        time->day = 1;
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
    
    if(time->week > 6)                    //dweek更新
    {
        time->week = 0;
    }
    SYS_EXIT_SCRT();
}



/************************************************************************
 * @Function: _getVirtualTime
 * @Description: 获取虚拟RTC时钟
 * 
 * @Arguments: 
 * @param: datetime 
 * @Note: 
 * @Auther: yzy
 * Date: 2015/6/1
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void _getVirtualTime(TIME* datetime)
{
    TIME* time = &gs_vtime;
    datetime->sec = time->sec;
    datetime->min = time->min;
    datetime->hour = time->hour;
    datetime->day = time->day;
    datetime->month = time->month;
    datetime->year = time->year;
    datetime->dweek = time->dweek;
    datetime->dyear = time->dweek;
}



/************************************************************************
 * @Function: _setVirtualTime
 * @Description: 设置虚拟RTC时钟
 * 
 * @Arguments: 
 * @param: datetime 
 * @Note: 
 * @Auther: yzy
 * Date: 2015/6/1
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void _setVirtualTime(TIME* datetime)
{
    //SYS_ENTER_AllSCRT();           //1)虚拟时钟不涉及中断处理,在进程中计时
                                    //2)进程接口函数中已有 gs_RealTimeRes 作为读写互斥.
                                    //3)该函数在 _SystemTaskInitHook()中调用,不允许存在开启中断
    TIME* time = &gs_vtime;
    time->sec = datetime->sec;
    time->min = datetime->min;
    time->hour = datetime->hour;
    time->day = datetime->day;
    time->month = datetime->month;
    time->year = datetime->year;
    time->dweek = CalcWeek((uint8*)&datetime->day);
    time->dyear = datetime->dweek;
    
    //SYS_EXIT_AllSCRT();
}


#endif

    
    

#if (RTC_HEX > 0)

/************************************************************************
 * @Function: SYS_MCU_ReadDateTime
 * @Description: 读取内部时钟,格式hex
 * 
 * @Arguments: 
 * @param: datetime 时间结构体缓存
 * 
 * @Note: 
 * @Return: uint8  
 * @Auther: yzy
 * Date: 2015/6/1
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8 SYS_MCU_ReadDateTime(TIME* datetime)
{
#if (SYS_RTC_EN == 0)
    _getVirtualTime(datetime);
    return SYS_ERR_OK;
    
#else
    HAL_RTC_GetTimes(datetime);
    return SYS_ERR_OK;
#endif
}


/************************************************************************
 * @Function: SYS_MCU_ReadBCDDateTime
 * @Description: 读取内部时钟,格式BCD
 * 
 * @Arguments: 
 * @param: datetime 
 * 
 * @Note: 
 * @Return: uint8  
 * @Auther: yzy
 * Date: 2015/6/1
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8 SYS_MCU_ReadBCDDateTime(TIME* datetime)
{ 
    if(SYS_MCU_ReadDateTime(datetime) == SYS_ERR_OK)
    {
        datetime->sec = ByteHexToBcd(datetime->sec);
        datetime->min = ByteHexToBcd(datetime->min);
        datetime->hour = ByteHexToBcd(datetime->hour);
        datetime->day = ByteHexToBcd(datetime->day);
        datetime->month = ByteHexToBcd(datetime->month);
        datetime->year = ByteHexToBcd(datetime->year);
        datetime->dweek = datetime->dweek;
        datetime->dyear = gp_calclib->IntHexToBcd(datetime->dyear);
        return SYS_ERR_OK;
    }
    
    return SYS_ERR_FT;
}


/************************************************************************
 * @Function: SYS_MCU_WriteDateTime
 * @Description: 设置内部时钟,格式hex
 * 
 * @Arguments: 
 * @param: datetime 时间结构体,其中的秒,分,时,日,月,年必须正确
 * 
 * @Note: 
 * @Return: uint8  
 * @Auther: yzy
 * Date: 2015/6/1
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8 SYS_MCU_WriteDateTime(TIME* datetime)
{
    const uint8* monthday;
                                            //判断设置数据的合法性
    if(datetime->year > 99 || datetime->month == 0 || 
       datetime->month > 12 || datetime->day == 0 || 
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
    if(datetime->day > monthday[datetime->month - 1])
    {
        return SYS_ERR_FT;
    }
                                            //当前为一年中的第几天
    datetime->dyear = gui_MonthBeginDay[datetime->month - 1] + datetime->day - 1;
                                            //判断今年是否为闰年,是否已过2月
    if((datetime->year & 0x03) == 0 && datetime->month > 2)
    {
        datetime->dyear++;
    }
    datetime->dweek = CalcWeek((uint8*)&datetime->day);
    
#if (SYS_RTC_EN == 0)
    _setVirtualTime(datetime);
    return SYS_ERR_OK;
    
#else
    HAL_RTC_SetTimes(datetime);
    return SYS_ERR_OK;
#endif
}


/************************************************************************
 * @Function: SYS_MCU_WriteBCDDateTime
 * @Description: 设置内部时钟(格式bcd)
 * 
 * @Arguments: 
 * @param: datetime 时间结构体其中的秒,分,时,日,月,年必须正确
 * 
 * @Note: 
 * @Return: uint8  
 * @Auther: yzy
 * Date: 2015/6/1
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8 SYS_MCU_WriteBCDDateTime(TIME* datetime)
{
    datetime->sec = ByteBcdToHex(datetime->sec);
    datetime->min = ByteBcdToHex(datetime->min);
    datetime->hour = ByteBcdToHex(datetime->hour);
    datetime->day = ByteBcdToHex(datetime->day);
    datetime->month = ByteBcdToHex(datetime->month);
    datetime->year = ByteBcdToHex(datetime->year);
    datetime->dweek = datetime->dweek;
    datetime->dyear = gp_calclib->IntBcdToHex(datetime->dyear);
    return SYS_MCU_WriteDateTime(datetime);
}


#else

/************************************************************************
 * @Function: SYS_MCU_ReadDateTime
 * @Description: 读取内部时钟,格式hex
 * 
 * @Arguments: 
 * @param: datetime 时间结构体缓存
 * 
 * @Note: 
 * @Return: uint8  
 * @Auther: yzy
 * Date: 2015/6/1
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8 SYS_MCU_ReadBCDDateTime(TIME* datetime)

{
#if (SYS_RTC_EN == 0)
    _getVirtualTime(datetime);
    return SYS_ERR_OK;
    
#else
    HAL_RTC_GetTimes(datetime);
    return SYS_ERR_OK;
#endif
}


/************************************************************************
 * @Function: SYS_MCU_ReadBCDDateTime
 * @Description: 读取内部时钟,格式BCD
 * 
 * @Arguments: 
 * @param: datetime 
 * 
 * @Note: 
 * @Return: uint8  
 * @Auther: yzy
 * Date: 2015/6/1
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8 SYS_MCU_ReadDateTime(TIME* datetime)

{ 
    if(SYS_MCU_ReadBCDDateTime(datetime) == SYS_ERR_OK)
    {
        datetime->sec = ByteBcdToHex(datetime->sec);
        datetime->min = ByteBcdToHex(datetime->min);
        datetime->hour = ByteBcdToHex(datetime->hour);
        datetime->day = ByteBcdToHex(datetime->day);
        datetime->month = ByteBcdToHex(datetime->month);
        datetime->year = ByteBcdToHex(datetime->year);
        datetime->week = datetime->week;
        datetime->dyear = ByteBcdToHex(datetime->dyear);
        return SYS_ERR_OK;
    }
    
    return SYS_ERR_FT;
}

/************************************************************************
 * @Function: SYS_MCU_WriteBCDDateTime
 * @Description: 设置内部时钟(格式bcd)
 * 
 * @Arguments: 
 * @param: datetime 时间结构体其中的秒,分,时,日,月,年必须正确
 * 
 * @Note: 
 * @Return: uint8  
 * @Auther: yzy
 * Date: 2015/6/1
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8 SYS_MCU_WriteBCDDateTime(TIME* datetime)
{
    datetime->sec = ByteHexToBcd(datetime->sec);
    datetime->min = ByteHexToBcd(datetime->min);
    datetime->hour = ByteHexToBcd(datetime->hour);
    datetime->day = ByteHexToBcd(datetime->day);
    datetime->month = ByteHexToBcd(datetime->month);
    datetime->year = ByteHexToBcd(datetime->year);
    datetime->week = datetime->week;
    datetime->dyear = ByteHexToBcd(datetime->dyear);
    return HAL_RTC_SetTimes(datetime);
}

/************************************************************************
 * @Function: SYS_MCU_WriteDateTime
 * @Description: 设置内部时钟,格式hex
 * 
 * @Arguments: 
 * @param: datetime 时间结构体,其中的秒,分,时,日,月,年必须正确
 * 
 * @Note: 
 * @Return: uint8  
 * @Auther: yzy
 * Date: 2015/6/1
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8 SYS_MCU_WriteDateTime(TIME* datetime)
{
    const uint8* monthday;
                                            //判断设置数据的合法性
    if(datetime->year > 99 || datetime->month == 0 || 
       datetime->month > 12 || datetime->day == 0 || 
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
    if(datetime->day > monthday[datetime->month - 1])
    {
        return SYS_ERR_FT;
    }
                                            //当前为一年中的第几天
    datetime->dyear = gui_MonthBeginDay[datetime->month - 1] + datetime->day - 1;
                                            //判断今年是否为闰年,是否已过2月
    if((datetime->year & 0x03) == 0 && datetime->month > 2)
    {
        datetime->dyear++;
    }
    datetime->week = CalcWeek((uint8*)&datetime->day);
    
#if (SYS_RTC_EN == 0)
    _setVirtualTime(datetime);
    return SYS_ERR_OK;
    
#else
    SYS_MCU_WriteBCDDateTime(datetime);
    return SYS_ERR_OK;
#endif
}



#endif


/************************************************************************
 * @Function: RTC_ProcTEsMsg
 * @Description: 实时时钟处理进程链表消息处理函数
 * 
 * @Arguments: 
 * @param: OpType 操作类型
 * @param: obj 
 * 
 * @Note: 
 * @Return: uint8  
 * @Auther: yzy
 * Date: 2015/6/1
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
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
            memcpy_s(time, (uint8*)obj, 3);//秒分时
            memcpy_s(time + 4,(uint8*)obj + 3,  3);//日月年
            time[3] = CalcWeek(time + 4);          //周
            result = I2C_HRTC_WriteBcdTime(time);
#endif
            result = SYS_MCU_WriteBCDDateTime(obj);      //同时更新RTC时间,字节顺序一致TIME类型和该time[]
            break;
            
        case HRTC_RW_WH:                    //写入HEX格式时间
#if SYS_HRTC_EN > 0
            memcpy_s(time, (uint8*)obj, 3);//秒分时
            memcpy_s(time + 4, (uint8*)obj + 3, 3);//日月年
            time[3] = CalcWeek(time + 4);          //周
            result = I2C_HRTC_WriteHexTime(time);
#endif
            result = SYS_MCU_WriteDateTime((TIME*)obj); 
            break;
            
        default:
            break;
    
    }
    
    return result;

}



/************************************************************************
 * @Function: SYS_WriteDateTime
 * @Description: 设置时间,格式HEX
 * 
 * @Arguments: 
 * @param: datetime 
 * 
 * @Note: 
 * @Return: uint8  
 * @Auther: yzy
 * Date: 2015/6/1
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8 SYS_WriteDateTime(TIME* datetime)
{
    uint8 result;
#ifndef __NO_SYS__     
    SYS_SEM_Wait(&gs_RealTimeRes, 0);
    result = RTC_ProcTEsMsg(HRTC_RW_WH, datetime);
    SYS_SEM_Release(&gs_RealTimeRes);
#else
    result = RTC_ProcTEsMsg(HRTC_RW_WH, datetime);
#endif
    return result;
}


/************************************************************************
 * @Function: SYS_WriteBCDDateTime
 * @Description: 设置时间,格式bcd
 * 
 * @Arguments: 
 * @param: datetime 
 * 
 * @Note: 
 * @Return: uint8  
 * @Auther: yzy
 * Date: 2015/6/1
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8 SYS_WriteBCDDateTime(TIME* datetime)
{
    uint8 result;
#ifndef __NO_SYS__     
    SYS_SEM_Wait(&gs_RealTimeRes, 0);
    result = RTC_ProcTEsMsg(HRTC_RW_WB, datetime);
    SYS_SEM_Release(&gs_RealTimeRes);
#else
    result = RTC_ProcTEsMsg(HRTC_RW_WB, datetime);
#endif    
    return result; 
}


/************************************************************************
 * @Function: SYS_ReadDateTime
 * @Description: 读取时间,格式HEX
 * 
 * @Arguments: 
 * @param: datetime 
 * 
 * @Note: 
 * @Return: uint8  
 * @Auther: yzy
 * Date: 2015/6/1
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8 SYS_ReadDateTime(TIME* datetime)
{
    uint8 result;
#ifndef __NO_SYS__ 
    SYS_SEM_Wait(&gs_RealTimeRes, 0);
    result = RTC_ProcTEsMsg(HRTC_RW_RH, datetime);
    SYS_SEM_Release(&gs_RealTimeRes);
#else
    result = RTC_ProcTEsMsg(HRTC_RW_RH, datetime);
#endif
    return result; 
}

/************************************************************************
 * @Function: SYS_ReadBCDDateTime
 * @Description: 读取时钟,格式bcd
 * 
 * @Arguments: 
 * @param: datetime 
 * 
 * @Note: 
 * @Return: uint8  
 * @Auther: yzy
 * Date: 2015/6/1
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8 SYS_ReadBCDDateTime(TIME* datetime)
{
    uint8 result;
#ifndef __NO_SYS__ 
    SYS_SEM_Wait(&gs_RealTimeRes, 0);
    result = RTC_ProcTEsMsg(HRTC_RW_RB, datetime);
    SYS_SEM_Release(&gs_RealTimeRes);
#else
    result = RTC_ProcTEsMsg(HRTC_RW_RB, datetime);
#endif    
    return result; 
}





/************************************************************************
 * @Function: GetTime
 * @Description: 获取当前的时间和日期的全部信息, 这个信息只读
 * @Arguments: 
 * 
 * @Note: 
 * @Return: const TIME*  
 * @Auther: yzy
 * Date: 2015/6/1
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
const TIME* GetTime(void)
{
    return (const TIME*)&gs_Time;
}



/************************************************************************
 * @Function: GetSysRunSecs
 * @Description: 获取系统连续运行时间
 * @Arguments: 
 * 
 * @Note: 
 * @Return: uint32  
 * @Auther: yzy
 * Date: 2015/6/1
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint32 GetSysRunSecs(void)
{
    return gul_SysRunSecs;
}


    
#if SYS_HRTC_EN > 0

/************************************************************************
 * @Function: SYS_RTC_CheckTime
 * @Description: 实时时钟校时
 * @Arguments: 
 * 
 * @Note: 
 * @Return: uint8  
 * @Auther: yzy
 * Date: 2015/6/1
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8 SYS_RTC_CheckTime(void)
{
    uint8 buff[7];                              //ssmmhhwwddmmyy
    uint8 time[6];
    uint32 sec,sec1;
    TIME tm;
    
    if(I2C_HRTC_ReadHexTime(buff) == SYS_ERR_OK)  //读取硬时钟时间
    {
        memcpy_s(time, buff, 3);               //秒分时
        memcpy_s(time + 3, buff + 4, 3);       //日月年(跳过周)
        sec = CalcSecs(time, false);
        
        I2C_HRTC_ReadHexTime(buff);
        memcpy_s(time, buff, 3);               //秒分时
        memcpy_s(time + 3, buff + 4, 3);       //日月年(跳过周)
        sec1 = CalcSecs(time, false);
        if(AbsVal(sec1 - sec) <= 2)		        //两次读取硬件RTC的时间差在2秒以内才写入MCU的RTC
        {                                       //转换数据格式
            tm.sec = buff[0];
            tm.min = buff[1];
            tm.hour = buff[2];
            tm.day = buff[4];
            tm.month = buff[5];
            tm.year = buff[6];
            return SYS_MCU_WriteDateTime(&tm);
        }
    }
    return SYS_ERR_FT;
}
#endif



#ifndef __NO_SYS__ 
/************************************************************************
 * @Function: SYS_RTC_SecProc
 * @Description: 实时时钟秒处理
 * 
 * @Arguments: 
 * @param: pdata 
 * 
 * @Note: 
 * @Return: bool  
 * @Auther: yzy
 * Date: 2015/6/1
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
bool SYS_RTC_SecProc(void* pdata)
{
#if (SYS_RTC_EN == 0)
    _virtualTimeSecProc();
#endif
                                            //读取内部RTC时间
    if(SYS_MCU_ReadDateTime(&gs_Time) != SYS_ERR_OK)
    {
        SYS_MCU_ReadDateTime(&gs_Time);
    }
                                            //系统运行时间增一
    gul_SysRunSecs++;
    
#if (true)                                  //事件分发
    extern const TESTcbDeclare __TKDeclare[SYS_TK_NUM];
    const TESTcbDeclare* dec = __TKDeclare + 1;

    for(uint32 uc_i = 1; uc_i < (SYS_TK_NUM - 1); uc_i++, dec++)
    {
                                            //如果该进程不存在则不发送
        if(dec->tcbc == __NULL || dec->stklen == 0)
        {
            continue;
        }
        
        if(!(gucs_MsgApplied[uc_i] & Bit_Map8[MSG_CLS_TM]))
        {
            continue;
        }
        
        if(dec->tcbc->tbl != 0)             //时间消息仅针对消息进程
        {
            if(gs_TimeBk.year != gs_Time.year) //年事件
            {
                SYS_Message_Send(MSG_YEAR, uc_i);
            }
            if(gs_TimeBk.month != gs_Time.month)//月事件
            {
                SYS_Message_Send(MSG_MON, uc_i);
            }
            if(gs_TimeBk.day != gs_Time.day) //天事件
            {
                SYS_Message_Send(MSG_DAY, uc_i);
            }
            if(gs_TimeBk.hour != gs_Time.hour) //小时事件
            {
                SYS_Message_Send(MSG_HOUR, uc_i);
            }
            if(gs_TimeBk.min != gs_Time.min)   //分事件
            {
				f_timecheck = true;			//过分后第30秒允许RTC校时
                SYS_Message_Send(MSG_MIN, uc_i);
            }
            SYS_Message_Send(MSG_SEC, uc_i); //发送秒消息
        }
    }
#endif

#if SYS_HRTC_EN > 0
    if((gs_Time.sec >= 30) && (gs_Time.sec <= 50) && f_timecheck)          	//在第30秒校时.每分钟校时一次.
    {
        SYS_RTC_CheckTime();
        f_timecheck = false;
    }
#endif
                                            //备份时间
    memcpy_s((uint8*)&gs_TimeBk, (uint8*)&gs_Time, sizeof(TIME));

    return true;
}
#else
#include "define.h"
/************************************************************************
 * @Function: SYS_RTC_SecProc
 * @Description: 实时时钟秒处理
 * 
 * @Arguments: 
 * @param: pdata 
 * 
 * @Note: 
 * @Return: bool  
 * @Auther: yzy
 * Date: 2015/6/1
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void SYS_RTC_SecProc(void)
{
#if (SYS_RTC_EN == 0)
    _virtualTimeSecProc();
#endif
                                            //读取内部RTC时间
    if(SYS_MCU_ReadDateTime(&gs_Time) != SYS_ERR_OK)
    {
        SYS_MCU_ReadDateTime(&gs_Time);
    }
                                            //系统运行时间增一
    gul_SysRunSecs++;
    
#if (true)                                  //事件分发
    //extern const TESTcbDeclare __TKDeclare[SYS_TK_NUM];
    //const TESTcbDeclare* dec = __TKDeclare + 1;

    for(uint32 uc_i = 0; uc_i < (SYS_TK_NUM); uc_i++)
    {

        
//        if(!(gucs_MsgApplied[uc_i] & Bit_Map8[MSG_CLS_TM]))
//        {
//            continue;
//        }
//        
////	        if(dec->tcbc->tbl != 0)             //时间消息仅针对消息进程
//        {
//            if(gs_TimeBk.year != gs_Time.year) //年事件
//            {
//                //SYS_Message_Send(MSG_YEAR, uc_i);
//            }
//            if(gs_TimeBk.month != gs_Time.month)//月事件
//            {
//                //SYS_Message_Send(MSG_MON, uc_i);
//            }
//            if(gs_TimeBk.day != gs_Time.day) //天事件
//            {
//                //SYS_Message_Send(MSG_DAY, uc_i);
//            }
//            if(gs_TimeBk.hour != gs_Time.hour) //小时事件
//            {
//                //SYS_Message_Send(MSG_HOUR, uc_i);
//            }
//            if(gs_TimeBk.min != gs_Time.min)   //分事件
//            {
//				f_timecheck = true;			//过分后第30秒允许RTC校时
//                //SYS_Message_Send(MSG_MIN, uc_i);
//                SET_SYSE_TMR_MINU;
//            }
//            //SYS_Message_Send(MSG_SEC, uc_i); //发送秒消息
//        }
    }
#endif

#if SYS_HRTC_EN > 0
    if((gs_Time.sec >= 30) && (gs_Time.sec <= 50) && f_timecheck)          	//在第30秒校时.每分钟校时一次.
    {
        SYS_RTC_CheckTime();
        f_timecheck = false;
    }
#endif
                                            //备份时间
    memcpy_s((uint8*)&gs_TimeBk, (uint8*)&gs_Time, sizeof(TIME));

    //return true;
}


#endif
/************************************************************************
 * @Function: SYS_RTC_Init
 * @Description: 设备模块初始化
 * @Arguments: 
 * @Note: 
 * @Auther: yzy
 * Date: 2015/6/1
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void SYS_RTC_Init(void)
{
#if (SYS_RTC_EN == 0)
    _virtualTimeInit();
#else
    HAL_InitRTC();
#endif
    
#if SYS_HRTC_EN > 0
    Init_I2C_HRTC(&gs_HrtcPort);            //初始化i2c总线
    SYS_RTC_CheckTime();
#endif
 #ifndef __NO_SYS__ 
    //创建用户资源
    SYS_SEM_Create(1, &gs_RealTimeRes);
    //创建一个定时器(定时时间1s)    
    SYS_Timer_Create(SYS_RTC_SecProc, __NULL, SYS_TICK_PER_SEC, ID_SWTIMER_RTC, false);
#endif	

    gul_SysRunSecs = 0;
	                                        //初始化备份时间
    SYS_MCU_ReadDateTime(&gs_Time);          //
    memcpy_s((uint8*)&gs_TimeBk, (uint8*)&gs_Time, sizeof(TIME));
    
    f_timecheck = true;					    //上电后就允许RTC校时
}


/************************************************************************
 * @Function: SYS_MSG_Init
 * @Description: 消息分发模块初始化
 * @Arguments: 
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/27
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void APP_RTC_Init(void)
{

#ifdef  __NO_SYS__
    s_S_MSK_INT_TYPE sTmType;

    sTmType.ucType = SYS_TM_1s;
    sTmType.ucPri = 0;  ///
    //创建消息分发服务定时器
    SMskSetTmFunc(sTmType, SYS_RTC_SecProc);
#else
    //SYS_Timer_Create(MSG_Server, __NULL, 1, ID_SWTIMER_MSG, false);
#endif
}

void APP_RTC_DeInit(void)
{

#ifdef  __NO_SYS__
//	    s_S_MSK_INT_TYPE sTmType;
//	
//	    sTmType.ucType = SYS_TM_1s;
//	    sTmType.ucPri = 0;  ///
    //创建消息分发服务定时器
    SMskClrTmFunc(SYS_TM_1s);
#else
    //SYS_Timer_Create(MSG_Server, __NULL, 1, ID_SWTIMER_MSG, false);
#endif
}


