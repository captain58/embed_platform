/******************************Copyright(c)******************************
**                         
**
** File Name: hal_rtc.c
** Author: 片内时钟模块
** Date Last Update: 2019-11-02
** Description: 
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: 文件创建
*************************************************************************/
#define EXT_HAL_RTC

#include "sys.h"
#include "hal.h"
#include <string.h>




/*
 * Copyright (C) 2015-2017 Alibaba Group Holding Limited
 */


#include <stdio.h>
#include <stdlib.h>

//	#include "aos/hal/rtc.h"
//	#include "stm32l4xx_hal.h"
//	#include "hal_rtc_stm32l4.h"
//	#include "stm32l4xx_hal_rtc.h"

#ifdef SYS_RTC_EN

/* Init and deInit function for rtc */
//static int32_t rtc_Init(rtc_dev_t *rtc);
//static int32_t rtc_DeInit(void);
//
///* function used to transform hal para to stm32l4 para */
//static int32_t rtc_format_transform(uint16_t format_hal, uint16_t *format_stm32l4);

/* handle for rtc */
//RTC_HandleTypeDef rtc_handle;
//rtc_dev_t g_stRtcDev;
//
//int32_t hal_rtc_init(rtc_dev_t *rtc)
//{
//    int32_t ret = -1;
//
//    if (rtc == NULL) {
//        return -1;
//    }
//
//    /*init rtc handle*/
//    memset(&rtc_handle, 0, sizeof(rtc_handle));
//
//    rtc->priv = &rtc_handle;
//    ret = rtc_Init(rtc);
//
//    return ret;
//}
//
//int32_t hal_rtc_get_time(rtc_dev_t *rtc, rtc_time_t *time)
//{
//    uint16_t format = 0;
//    int32_t ret1 = -1;
//    int32_t ret2 = -1;	
//    int32_t ret = -1;
//    RTC_TimeTypeDef time_st;
//    RTC_DateTypeDef data_st;
//
//    if ((rtc == NULL) || (time == NULL)) {
//        return -1;
//    }
//
//    ret = rtc_format_transform(rtc->config.format, &format);
//    if (ret == 0) {
//        ret1 = HAL_RTC_GetTime((RTC_HandleTypeDef *)rtc->priv, &time_st, format);
//        ret2 = HAL_RTC_GetDate((RTC_HandleTypeDef *)rtc->priv, &data_st, format);
//
//        if ((ret1 == 0) && (ret2 == 0)) {
//            time->year = data_st.Year;
//            time->month = data_st.Month;
//            time->date = data_st.Date;
//            time->weekday = data_st.WeekDay;
//            time->hr = time_st.Hours;
//            time->min = time_st.Minutes;
//            time->sec = time_st.Seconds;
//
//            ret = 0;
//        } else {
//            ret = -1;
//        }
//    }
//
//    return ret;
//}
//
//int32_t hal_rtc_set_time(rtc_dev_t *rtc, const rtc_time_t *time)
//{
//    uint16_t format = 0;
//    int32_t ret1 = -1;
//    int32_t ret2 = -1;	
//    int32_t ret = -1;
//    RTC_TimeTypeDef time_st;
//    RTC_DateTypeDef data_st;
//
//    if ((rtc == NULL) || (time == NULL)) {
//		    return -1;
//		}
//
//    memset(&time_st, 0, sizeof(time_st));
//    memset(&data_st, 0, sizeof(data_st));
//		
//    ret = rtc_format_transform(rtc->config.format, &format);
//    if (ret == 0) {
//        data_st.Year = time->year;
//        data_st.Month = time->month;
//        data_st.Date = time->date;
//        data_st.WeekDay = time->weekday;
//        time_st.Hours = time->hr;
//        time_st.Minutes = time->min;
//        time_st.Seconds = time->sec;
//			
//        ret1 = HAL_RTC_SetTime((RTC_HandleTypeDef *)rtc->priv, &time_st, format);
//        ret2 = HAL_RTC_SetDate((RTC_HandleTypeDef *)rtc->priv, &data_st, format);
//
//        if ((ret1 == 0) && (ret2 == 0)) {
//            ret = 0;
//        } else {
//            ret = -1;
//        }
//    }
//
//    return ret;
//}
//
//int32_t hal_rtc_finalize(rtc_dev_t *rtc)
//{
//    int32_t ret = -1;
//
//    if (rtc == NULL) {
//        return -1;
//    }
//
//    ret = rtc_DeInit();
//
//    return ret;
//}
//
//int32_t rtc_Init(rtc_dev_t *rtc)
//{
//    int32_t ret = 0;
//
//    rtc_handle.Instance = RTC;
//	
//    rtc_handle.Init.HourFormat     = RTC_HOURFORMAT_24;//RTC_INIT_HOUR_FORMAT;
//    rtc_handle.Init.AsynchPrediv   = 127;//RTC_INIT_ASYNC_PREDIV;
//    rtc_handle.Init.SynchPrediv    = 255;//RTC_INIT_SYNC_PREDIV;
//    rtc_handle.Init.OutPut         = RTC_HOURFORMAT_24;//RTC_INIT_OUTPUT;
//    rtc_handle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;//RTC_INIT_OUTPUT_PRI;
//    rtc_handle.Init.OutPutType     = RTC_OUTPUT_TYPE_OPENDRAIN;//RTC_INIT_OUTPUT_TYPE;
////  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
////  hrtc.Init.AsynchPrediv = 127;
////  hrtc.Init.SynchPrediv = 255;
////  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
////  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
////  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
//    __HAL_RCC_RTC_ENABLE();
//    ret = HAL_RTC_Init(&rtc_handle);
//
//    return ret;
//}
//
//int32_t rtc_DeInit(void)
//{
//    int32_t ret = -1;
//
//    /* adc1 deinitialization */
//    ret = HAL_RTC_DeInit(&rtc_handle);
//
//    return ret;
//}
//
//static int32_t rtc_format_transform(uint16_t format_hal, uint16_t *format_stm32l4)
//{
//    uint16_t format = 0;
//    int32_t	ret = 0;
//
//    if(format_hal == HAL_RTC_FORMAT_DEC)
//    {
//        format = RTC_FORMAT_BIN;
//    }
//    else if(format_hal == HAL_RTC_FORMAT_BCD)
//    {
//        format = RTC_FORMAT_BCD;
//    }
//    else
//    {
//        ret = -1;
//    }
//
//    if(ret == 0)
//    {
//        *format_stm32l4 = format;
//    }
//
//    return ret;
//}


/************************************************************************
 * @Function: HAL_InitRTC
 * @Description: 初始化RTC模块
 * @Arguments: 
 * @Note: 这个函数不是进程安全的,确保同一时刻只在一个进程内使用
 *          初始化函数并不打开RTC中断,需要应用自行打开该中断
 * @Auther: yzy
 * Date: 2015/6/1
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void HAL_InitRTC(void)
{
#if SYS_RTC_EN > 0
//    g_stRtcDev.port = 0;
//    g_stRtcDev.config.format = HAL_RTC_FORMAT_BCD;
//
//    hal_rtc_init(&g_stRtcDev);
    /* enable PMU and BKPI clocks */
    rcu_periph_clock_enable(RCU_BKPI);
    rcu_periph_clock_enable(RCU_PMU);
    /* allow access to BKP domain */
    pmu_backup_write_enable();

    /* reset backup domain */
    bkp_deinit();
    
    /* enable LXTAL */
    rcu_osci_on(RCU_LXTAL);
    /* wait till LXTAL is ready */
    rcu_osci_stab_wait(RCU_LXTAL);
    
    /* select RCU_LXTAL as RTC clock source */
    rcu_rtc_clock_config(RCU_RTCSRC_LXTAL);

    /* enable RTC Clock */
    rcu_periph_clock_enable(RCU_RTC);

    /* wait for RTC registers synchronization */
    rtc_register_sync_wait();

    /* wait until last write operation on RTC registers has finished */
    rtc_lwoff_wait();

    /* enable the RTC second interrupt*/
    rtc_interrupt_enable(RTC_INT_SECOND);

    /* wait until last write operation on RTC registers has finished */
    rtc_lwoff_wait();

    /* set RTC prescaler: set RTC period to 1s */
    rtc_prescaler_set(32767);

    /* wait until last write operation on RTC registers has finished */
    rtc_lwoff_wait();  
  
#endif

}
//#if SYS_RTC_EN > 0
//
//uint8_t _RTC_GetRTC(RTC_TimeDateTypeDef* para)
//{
//    uint8_t n, i;
//    uint8_t Result = 1;
//
//    RTC_TimeDateTypeDef TempTime1,TempTime2;
//    
//    for(n=0 ;n<3; n++)
//    {
//        RTC_TimeDate_GetEx(&TempTime1);//读一次时间
//        RTC_TimeDate_GetEx(&TempTime2);//再读一次时间
//        
//        for(i=0; i<7; i++)//两者一致, 表示读取成功
//        {
//            if(((uint8_t*)(&TempTime1))[i] != ((uint8_t*)(&TempTime2))[i]) break;
//        }
//        if(i == 7)
//        {
//            Result = 0;
//            memcpy((uint8_t*)(para), (uint8_t*)(&TempTime1), 7);//读取正确则更新新的时间		
//            break;
//        }
//    }
//    return Result;
//}
//#endif




///************************************************************************
// * @Function: RTC_Handler
// * @Description: RTC中断处理函数,过秒触发
// * @Arguments: 
// * @Note: 
// * @Auther: yzy
// * Date: 2015/6/1
// *-----------------------------------------------------------------------
// * @History: 
// ************************************************************************/
//void RTC_Handler(void)
//{
//    //
//}




/************************************************************************
 * @Function: HAL_RTC_SetTimes
 * @Description: 设置芯片片内RTC时间
 * 
 * @Arguments: 
 * @param: datetime 需要设置的时间和日期
 * @Note: 这个函数不是进程安全的,应用时需要保证datetime的数据都是正确的
 *         这里不判断数据的正确性
 * @Auther: yzy
 * Date: 2015/6/1
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
//	uint8_t HAL_RTC_SetTimes(TIME* datetime)
//	{
//	#if SYS_RTC_EN > 0
//	    
//	#endif
//	}


uint8_t HAL_RTC_SetTimes(TIME* datetime)
{
    uint8_t Result = 1;
    rtc_time_t rtc_t;

#if SYS_RTC_EN > 0

//	    uint8_t n, i;
//	    RTC_TimeDateTypeDef para;
//	    RTC_TimeDateTypeDef TempTime1;
//	
//	    para.Second = datetime->sec;
//	    para.Minute = datetime->min;
//	    para.Hour = datetime->hour;
//	    para.Date = datetime->day;
//	    para.Month = datetime->month;
//	    para.Year = datetime->year;
//	    para.Week = datetime->week;
//	    
//	    for(n=0 ;n<3; n++)
//	    {
//	        RTC_RTCWE_Write(RTC_WRITE_ENABLE);	//解除RTC写保护
//	        RTC_TimeDate_SetEx(&para);			//设置RTC
//	        RTC_RTCWE_Write(RTC_WRITE_DISABLE);	//打开RTC写保护
//	        
//	        Result = _RTC_GetRTC(&TempTime1);	//读取确认设置结果
//	        if(Result == 0)
//	        {
//	            Result = 1;
//	            for(i=0; i<7; i++)//两者一致, 表示设置成功
//	            {
//	                if(((uint8_t*)(&TempTime1))[i] != ((uint8_t*)(&para))[i]) break;
//	            }
//	            if(i == 7)
//	            {
//	                Result = 0;
//	                break;
//	            }
//	        }
//	    }
//    rtc_t.sec = ByteHexToBcd(datetime->sec);
//    rtc_t.min = ByteHexToBcd(datetime->min);
//    rtc_t.hr = ByteHexToBcd(datetime->hour);
//    rtc_t.date = ByteHexToBcd(datetime->day);
//    rtc_t.month = ByteHexToBcd(datetime->month);
//    rtc_t.year = ByteHexToBcd(datetime->year);
//    rtc_t.weekday = datetime->week;
//    
//
//    if(!hal_rtc_set_time(&g_stRtcDev,(const rtc_time_t*)&rtc_t))
//    {
//        return SYS_ERR_OK;
//    }
    uint32_t ulCount = CalcSecs(datetime, 0);
    rtc_counter_set(ulCount);

#endif
    return SYS_ERR_OK;//Result;
    
}



/************************************************************************
 * @Function: HAL_RTC_GetTimes
 * @Description: 获取芯片片内RTC时间
 * 
 * @Arguments: 
 * @param: datetime 需要读取的时间存放的结构体
 * @Note: 
 * @Auther: yzy
 * Date: 2015/6/1
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
//	void HAL_RTC_GetTimes(TIME* datetime)
//	{
//	    ClearBuffer((uint8*)datetime, sizeof(TIME));
//	#if SYS_RTC_EN > 0
//	    
//	}
uint8_t HAL_RTC_GetTimes(TIME* datetime)
{
    memset((uint8*)datetime, 0, sizeof(TIME));
    uint8_t Result = 1;
    
#if SYS_RTC_EN > 0
    uint32_t ulCount = rtc_counter_get();
    CalcTimeFromSecs(datetime, ulCount);
    
//    rtc_time_t rtc_t;
//    
//    if(!hal_rtc_get_time(&g_stRtcDev,&rtc_t))
//    {
//        datetime->sec = rtc_t.sec;
//        datetime->min = rtc_t.min;
//        datetime->hour = rtc_t.hr;
//        datetime->day = rtc_t.date;
//        datetime->month = rtc_t.month;
//        datetime->year = rtc_t.year;
//        datetime->week = rtc_t.weekday;
//        return SYS_ERR_OK;
//    }
    return SYS_ERR_OK;

#endif
}


#endif


