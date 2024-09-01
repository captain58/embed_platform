/*
 * Copyright (C) 2015-2017 Alibaba Group Holding Limited
 */

#include <k_api.h>
#include <stdio.h>
#include <stdlib.h>
#include "sys.h"
#include "hal.h"
//#include "aos/hal/rtc.h"
#include "stm32l4xx_hal.h"
#include "hal_rtc_stm32l4.h"
#include "stm32l4xx_hal_rtc.h"

#ifdef HAL_RTC_MODULE_ENABLED

/* Init and deInit function for rtc */
static int32_t rtc_Init(rtc_dev_t *rtc);
static int32_t rtc_DeInit(void);

/* function used to transform hal para to stm32l4 para */
static int32_t rtc_format_transform(uint16_t format_hal, uint16_t *format_stm32l4);

/* handle for rtc */
RTC_HandleTypeDef rtc_handle;

rtc_dev_t g_stRtcDev;


int32_t hal_rtc_init(rtc_dev_t *rtc)
{
    int32_t ret = -1;

    if (rtc == NULL) {
        return -1;
    }

    /*init rtc handle*/
    memset(&rtc_handle, 0, sizeof(rtc_handle));

    rtc->priv = &rtc_handle;
    ret = rtc_Init(rtc);

    return ret;
}

int32_t hal_rtc_get_time(rtc_dev_t *rtc, rtc_time_t *time)
{
    uint16_t format = 0;
    int32_t ret1 = -1;
    int32_t ret2 = -1;	
    int32_t ret = -1;
    RTC_TimeTypeDef time_st;
    RTC_DateTypeDef data_st;

    if ((rtc == NULL) || (time == NULL)) {
        return -1;
    }

    ret = rtc_format_transform(rtc->config.format, &format);
    if (ret == 0) {
        ret1 = HAL_RTC_GetTime((RTC_HandleTypeDef *)rtc->priv, &time_st, format);
        ret2 = HAL_RTC_GetDate((RTC_HandleTypeDef *)rtc->priv, &data_st, format);

        if ((ret1 == 0) && (ret2 == 0)) {
            time->year = data_st.Year;
            time->month = data_st.Month;
            time->date = data_st.Date;
            time->weekday = data_st.WeekDay;
            time->hr = time_st.Hours;
            time->min = time_st.Minutes;
            time->sec = time_st.Seconds;

            ret = 0;
        } else {
            ret = -1;
        }
    }

    return ret;
}

int32_t hal_rtc_set_time(rtc_dev_t *rtc, const rtc_time_t *time)
{
    uint16_t format = 0;
    int32_t ret1 = -1;
    int32_t ret2 = -1;	
    int32_t ret = -1;
    RTC_TimeTypeDef time_st;
    RTC_DateTypeDef data_st;

    if ((rtc == NULL) || (time == NULL)) {
		    return -1;
		}

    memset(&time_st, 0, sizeof(time_st));
    memset(&data_st, 0, sizeof(data_st));
		
    ret = rtc_format_transform(rtc->config.format, &format);
    if (ret == 0) {
        data_st.Year = time->year;
        data_st.Month = time->month;
        data_st.Date = time->date;
        data_st.WeekDay = time->weekday;
        time_st.Hours = time->hr;
        time_st.Minutes = time->min;
        time_st.Seconds = time->sec;
			
        ret1 = HAL_RTC_SetTime((RTC_HandleTypeDef *)rtc->priv, &time_st, format);
        ret2 = HAL_RTC_SetDate((RTC_HandleTypeDef *)rtc->priv, &data_st, format);

        if ((ret1 == 0) && (ret2 == 0)) {
            ret = 0;
        } else {
            ret = -1;
        }
    }

    return ret;
}

int32_t hal_rtc_finalize(rtc_dev_t *rtc)
{
    int32_t ret = -1;

    if (rtc == NULL) {
        return -1;
    }

    ret = rtc_DeInit();

    return ret;
}

int32_t rtc_Init(rtc_dev_t *rtc)
{
    int32_t ret = 0;

    rtc_handle.Instance = RTC;
	
    rtc_handle.Init.HourFormat     = RTC_INIT_HOUR_FORMAT;
    rtc_handle.Init.AsynchPrediv   = RTC_INIT_ASYNC_PREDIV;
    rtc_handle.Init.SynchPrediv    = RTC_INIT_SYNC_PREDIV;
    rtc_handle.Init.OutPut         = RTC_INIT_OUTPUT;
    rtc_handle.Init.OutPutPolarity = RTC_INIT_OUTPUT_PRI;
    rtc_handle.Init.OutPutType     = RTC_INIT_OUTPUT_TYPE;

    ret = HAL_RTC_Init(&rtc_handle);

    return ret;
}

int32_t rtc_DeInit(void)
{
    int32_t ret = -1;

    /* adc1 deinitialization */
    ret = HAL_RTC_DeInit(&rtc_handle);

    return ret;
}

static int32_t rtc_format_transform(uint16_t format_hal, uint16_t *format_stm32l4)
{
    uint16_t format = 0;
    int32_t	ret = 0;

    if(format_hal == HAL_RTC_FORMAT_DEC)
    {
        format = RTC_FORMAT_BIN;
    }
    else if(format_hal == HAL_RTC_FORMAT_BCD)
    {
        format = RTC_FORMAT_BCD;
    }
    else
    {
        ret = -1;
    }

    if(ret == 0)
    {
        *format_stm32l4 = format;
    }

    return ret;
}
#endif


void HAL_InitRTC(void)
{
#if SYS_RTC_EN > 0
  
  
  
//    hrtc.Instance = RTC;
//    hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
//    hrtc.Init.AsynchPrediv = 127;
//    hrtc.Init.SynchPrediv = 255;
//    hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
//    hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
//    hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
//    hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  
        g_stRtcDev.port = 0;
    g_stRtcDev.config.format = HAL_RTC_FORMAT_BCD;

    hal_rtc_init(&g_stRtcDev);
#endif
}


uint8_t HAL_RTC_SetTimes(TIME* datetime)
{
    uint8_t Result = 1;
    rtc_time_t rtc_t;

#if SYS_RTC_EN > 0

//    uint32_t ulCount = Calendar2Sec(datetime, 0);//CalcSecs(datetime, 0);
//    rtc_counter_set(ulCount);
//    rtc_time_t rtc_t;
    rtc_t.sec = ByteHexToBcd(datetime->sec);
    rtc_t.min = ByteHexToBcd(datetime->min);
    rtc_t.hr = ByteHexToBcd(datetime->hour);
    rtc_t.date = ByteHexToBcd(datetime->day);
    rtc_t.month = ByteHexToBcd(datetime->month);
    rtc_t.year = ByteHexToBcd(datetime->year);
    rtc_t.weekday = datetime->week;
    

    if(!hal_rtc_set_time(&g_stRtcDev,(const rtc_time_t*)&rtc_t))
    {
        return SYS_ERR_OK;
    }
    else
    {
        return SYS_ERR_FT;
    }
#endif
//    return SYS_ERR_OK;//Result;
    
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
uint8_t HAL_RTC_GetTimes(TIME* datetime)
{
    memset((uint8*)datetime, 0, sizeof(TIME));
    uint8_t Result = 1;
    
#if SYS_RTC_EN > 0
//    uint32_t ulCount = rtc_counter_get();
//    Sec2Calendar(datetime, ulCount);
//    
//    return SYS_ERR_OK;
    rtc_time_t rtc_t;

    if(!hal_rtc_get_time(&g_stRtcDev,&rtc_t))
    {
        datetime->sec = rtc_t.sec;
        datetime->min = rtc_t.min;
        datetime->hour = rtc_t.hr;
        datetime->day = rtc_t.date;
        datetime->month = rtc_t.month;
        datetime->year = rtc_t.year;
        datetime->week = rtc_t.weekday;
        return SYS_ERR_OK;
    }
    return SYS_ERR_FT;
#endif
}

