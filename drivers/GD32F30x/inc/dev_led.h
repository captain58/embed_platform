/******************************Copyright(c)******************************
**                         
**
** File Name: dev_led.h
** Author: 
** Date Last Update: 2019-11-02
** Description: LED模块头文件(属于GPO的特殊应用)
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: 文件创建
** 与v5.0的差别:1)定时处理有10个tick改为1个tick,用于提供更快的闪烁频率
**            2)接口函数中的 持续时间 单位改为tick.
*************************************************************************/
#ifndef _DEV_LED_H_
#define _DEV_LED_H_


#ifndef EXT_DEV_LED
#define EXT_DEV_LED extern
#endif
 





/************************************************************************
 * @Function: SYS_BlinkDev_OnOff
 * @Description: LED开关操作
 * 
 * @Arguments: 
 * @param: led 灯编号枚举,类型 LED_ENUM
 * @param: oc true点亮, false 熄灭
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/27
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void SYS_BlinkDev_OnOff(uint32 led, bool oc);


/************************************************************************
 * @Function: SYS_BlinkDev_Init
 * @Description: LED功能初始化
 * @Arguments: 
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/27
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void SYS_BlinkDev_Init(void);


/************************************************************************
 * @Function: SYS_LED_BlinkSet
 * @Description: LED操作接口函数(50%占空比)
 * 
 * @Arguments: 
 * @param: led 灯编号枚举,类型 LED_ENUM
 * @param: type 0 只亮一下,1 闪烁,2 常亮,其他 熄灭
 * @param: interval 闪烁间隔,时间单位tick(type为1时有效)
 * @param: time 亮的时间 单位tick(0表示永久有效)
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/27
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void SYS_Dev_HalfBlinkSet(uint32_t led, uint8_t type, uint8_t interval, uint16_t time);


/************************************************************************
 * @Function: SYS_LED_BlinkSetAll
 * @Description: LED统一点亮熄灭接口函数(50%占空比)
 * 
 * @Arguments: 
 * @param: type 0 只亮一下, 1 闪烁, 2 常亮,其他 熄灭
 * @param: interval 闪烁间隔,时间单位tick(type为1时有效)
 * @param: time 亮的时间 单位tick(0表示永久有效)
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/27
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void SYS_Dev_HalfBlinkSetAll(uint8_t type, uint8_t interval, uint16_t time);


/************************************************************************
 * @Function: SYS_Dev_OptBlinkSet
 * @Description: LED,beep点亮熄灭接口函数,(随意占空比)
 * 
 * @Arguments: 
 * @param: led 灯编号枚举,类型 LED_ENUM
 * @param: type 0 只亮一下, 1 闪烁, 2 常亮,其他 熄灭
 * @param: oninterval 点亮间隔,时间单位100ms(type为1时有效)
 * @param: offinterval 熄灭间隔,时间单位100ms(type为1时有效)
 * @param: time 亮的时间 单位100ms(0表示永久有效)
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/27
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void SYS_Dev_OptBlinkSet(uint32_t led, uint8_t type, uint8_t oninterval, uint8_t offinterval, uint16_t time);


/************************************************************************
 * @Function: SYS_LED_OptBlinkSetAll
 * @Description: LED统一点亮熄灭接口函数(随意占空比)
 * 
 * @Arguments: 
 * @param: type 0 只亮一下, 1 闪烁, 2 常亮,其他 熄灭
 * @param: oninterval 点亮间隔,时间单位100ms(type为1时有效)
 * @param: offinterval 熄灭间隔,时间单位100ms(type为1时有效)
 * @param: time 亮的时间 单位100ms(0表示永久有效)
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/27
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void SYS_Dev_OptBlinkSetAll(uint8_t type, uint8_t oninterval, uint8_t offinterval, uint16_t time);


/************************************************************************
 * @Function: SYS_LED_TIMEROC
 * @Description: LED定时器开关函数
 * 
 * @Arguments: 
 * @param: oc true启动, false 停止
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/27
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void SYS_LED_TIMEROC(bool oc);


void SYS_BlinkDev_Idel();
void SYS_BlinkDev_WakeUp();


#endif

