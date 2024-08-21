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
#ifndef _HAL_RTC_H_
#define _HAL_RTC_H_

#ifndef EXT_HAL_RTC
#define EXT_HAL_RTC extern
#endif
#define HAL_RTC_FORMAT_DEC 1
#define HAL_RTC_FORMAT_BCD 2

typedef struct {
    uint8_t  format; /* time formart DEC or BCD */
} rtc_config_t;

typedef struct {
    uint8_t port;        /* rtc port */
    rtc_config_t config; /* rtc config */
    void   *priv;        /* priv data */
} rtc_dev_t;
/*
 * RTC time
 */
typedef struct {
    uint8_t sec;     /* DEC format:value range from 0 to 59, BCD format:value range from 0x00 to 0x59 */
    uint8_t min;     /* DEC format:value range from 0 to 59, BCD format:value range from 0x00 to 0x59 */
    uint8_t hr;      /* DEC format:value range from 0 to 23, BCD format:value range from 0x00 to 0x23 */
    uint8_t weekday; /* DEC format:value range from 1 to  7, BCD format:value range from 0x01 to 0x07 */
    uint8_t date;    /* DEC format:value range from 1 to 31, BCD format:value range from 0x01 to 0x31 */
    uint8_t month;   /* DEC format:value range from 1 to 12, BCD format:value range from 0x01 to 0x12 */
    uint8_t year;    /* DEC format:value range from 0 to 99, BCD format:value range from 0x00 to 0x99 */
} rtc_time_t;

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
void HAL_InitRTC(void);
	
	
	
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
uint8_t HAL_RTC_SetTimes(TIME* datetime);
	
	
	
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
uint8_t HAL_RTC_GetTimes(TIME* datetime);


#endif



