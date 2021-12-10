/******************************Copyright(c)******************************
**                         
**
** File Name: dev_led.c
** Author: 
** Date Last Update: 2019-11-02
** Description: LED模块文件(属于GPO的特殊应用)
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: 文件创建
** 与v5.0的差别:1)定时处理有10个tick改为1个tick,用于提供更快的闪烁频率
**            2)接口函数中的 持续时间 单位改为tick.
*************************************************************************/
#define EXT_LED
#ifdef __MODULE__
#include "WOSsys.h"
#endif

#include "sys.h"
#include "hal.h"
#include "bsp.h"



/************************************************************************
 * @模块编译提示
 ************************************************************************/
#if (SYS_LED_BEEP_EN > 0)
    
    #if !defined(LED_NUM) || (LED_NUM < 1)
        #define LED_NUM  1
        #warning "LED总数宏错误"
    #endif
     
    
//    #if !defined(ID_SWTIMER_LED)
//        #define ID_SWTIMER_LED    0xff
//        #warning "请定义LED扫描定时器ID编号宏"
//    #endif
#endif




/************************************************************************
**LED亮灯结构体信息
************************************************************************/
typedef struct
{
    uint8  type;                         //表示灯亮的类型(0只亮一下,1闪烁,2常亮,其他灭)
    uint8  off_interval;                 //表示灯熄灭间隔(type == 1有效)
    uint8  on_interval;                  //表示灯点亮间隔(type == 1有效)
    uint8  curstt;                       //当前是on还是off.0表示off.
    uint16 time;                         //表示灯作用持续时间(0xFFFF表示永久有效)
    uint8  count;                        //表示灯闪烁计时器(type == 1有效)
}LedLights;

LedLights* gsp_LedLights;

static ktimer_t     gst_LedTimer; 

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
void SYS_BlinkDev_OnOff(uint32 led, bool oc)
{
#if (SYS_LED_BEEP_EN > 0)
    _IF_TRUE_RETURN_VOID(led >= LED_NUM);   //参数检验
                                            //获取LED端口指针
    GPO_PORTS *gp = (GPO_PORTS*)gs_LedPort + led;
    if(oc)                                  //输出高电平
    {
        if(gp->rvs)                         //逻辑电平翻转
        {
            HAL_GPIO_SetPinState(gp->port, gp->port->pinnum, false);
        }
        else
        {
            HAL_GPIO_SetPinState(gp->port, gp->port->pinnum, true);
        }
    }
    else                                    //输出低电平
    {
        if(gp->rvs)
        {
            HAL_GPIO_SetPinState(gp->port, gp->port->pinnum, true);
        }
        else
        {
            HAL_GPIO_SetPinState(gp->port, gp->port->pinnum, false);
        }
    }
#endif
}




#if (SYS_LED_BEEP_EN > 0)
/************************************************************************
 * @Function: LED_Server
 * @Description: LED定时服务程序(tick周期)
 * 
 * @Arguments: 
 * @param: pdata 
 * 
 * @Note: 
 * @Return: bool  
 * @Auther: yzy
 * Date: 2015/5/27
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
bool LED_Server(void* pdata)
{                                      
    for(uint32 i = 0; i < LED_NUM; i++) //循环处理各LED
    {
        LedLights *ll = gsp_LedLights + i;//获取LED亮灯结构体指针
        
        if(ll->time != 0xFFFF)
        {
            if(ll->time > 0)            //更新有效持续时间
            {
                ll->time--;
            }
            else
            {
                ll->type = 0xFF;
            }
        }
        
		if(ll->type == 0)               //只亮一下的
		{
			SYS_BlinkDev_OnOff(i, true);
			ll->type = 0xFF;            //下一次进来就不亮了
		}
		else if(ll->type == 1)          //闪烁
		{
            if(ll->curstt)              //当前点亮
            {
                if(ll->count >= ll->on_interval)
                {                       
                    SYS_BlinkDev_OnOff(i, false);//到间隔时间之后就熄灭
                    ll->curstt = 0;     
                    ll->count = 0;      //计数器清零
                    continue;
                }
            }
            else                        //当前熄灭
            {
                if(ll->count >= ll->off_interval)
                {                       //到间隔时间之后就点亮
                    SYS_BlinkDev_OnOff(i, true);
                    ll->curstt = 1;
                    ll->count = 0;      //计数器清零
                    continue;
                }
            }
			ll->count++;                //计数器累加
		}
        else if(ll->type == 2)          //常亮
        {
            SYS_BlinkDev_OnOff(i, true);
        }
        else
        {
            SYS_BlinkDev_OnOff(i, false);      //其他情况关闭LED
        }
    }
    
    return true;

}
#endif



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

void SYS_BlinkDev_Init(void)
{
#if (SYS_LED_BEEP_EN > 0)
    GPO_PORTS* gpo;
    
    //循环初始化各个LED端口
    for(uint32 uc_i = 0; uc_i < LED_NUM; uc_i++)
    {
        gpo = (GPO_PORTS*)gs_LedPort + uc_i;//LED端口信息
                                            //配置端口功能,并设为输出模式
//        HAL_GPIO_PinConfig(gpo->cp->port, gpo->cp->pin, gpo->cp->mode);
//        HAL_GPIO_SetPinDIR(gpo->cp->port, gpo->cp->pin, true);
        HAL_GPIO_PinConfig(gpo->port);        
        if(gpo->rvs != gpo->ival)           //等同于异或
        {
            HAL_GPIO_SetPinState(gpo->port, gpo->port->pinnum, 1);
        }
        else
        {
            HAL_GPIO_SetPinState(gpo->port, gpo->port->pinnum, 0);
        }
    }
                                            //申请缓存
    gsp_LedLights = (LedLights*)m_malloc(sizeof(LedLights) * LED_NUM);
    memset((uint8*)gsp_LedLights, 0, sizeof(LedLights) * LED_NUM);
#ifndef __NO_SYS__
    //创建亮灯服务定时器
//    SYS_Timer_Create(LED_Server, __NULL, 10, ID_SWTIMER_LED, false);
    
//	    krhino_timer_create(&gst_LedTimer, "gst_LedTimer", LED_Server,
//	                        2, 1, 0, 1);   
//	    
    SysTimer_DlyConfig(LED_Server, __NULL, 1, 10);
	SysTimer_DlyStart(1);

#endif
#endif
}

void SYS_BlinkDev_Idel()
{
#if (SYS_LED_BEEP_EN > 0)
    GPO_PORTS* gpo;

    //循环初始化各个LED端口
    for(uint32 uc_i = 0; uc_i < LED_NUM; uc_i++)
    {
        gpo = (GPO_PORTS*)gs_LedPort + uc_i;//LED端口信息
                                            //配置端口功能,并设为输出模式
        HAL_GPIO_PinConfig(gpo->port);        
        if(gpo->rvs != gpo->ival)           //等同于异或
        {
            HAL_GPIO_SetPinState(gpo->port, gpo->port->pinnum, 1);
        }
        else
        {
            HAL_GPIO_SetPinState(gpo->port, gpo->port->pinnum, 0);
        }
    }

    m_free(gsp_LedLights);gsp_LedLights=NULL;
#endif
}
//	void SYS_BlinkDev_WakeUp()
//	{
//	#if (SYS_LED_BEEP_EN > 0)
//	    COMPORT* gpo;
//	
//	    //循环初始化各个LED端口
//	    for(uint32 uc_i = 0; uc_i < LED_NUM; uc_i++)
//	    {
//	        gpo = (COMPORT*)gs_LedPort + uc_i;//LED端口信息
//	                                            //配置端口功能,并设为输出模式
//	        HAL_GPIO_PinConfig(&gpo->gpio[gpo->pingrp], gpo->pinnum, gpo->type, gpo->analog, gpo->dir);        
//	        if(gpo->rvs != gpo->ival)           //等同于异或
//	        {
//	            HAL_GPIO_SetPinState(&gpo->gpio[gpo->pingrp], gpo->pinnum, 1);
//	        }
//	        else
//	        {
//	            HAL_GPIO_SetPinState(&gpo->gpio[gpo->pingrp], gpo->pinnum, 0);
//	        }
//	    }
//	#endif

//}


/************************************************************************
 * @Function: SYS_LED_BlinkSet
 * @Description: LED操作接口函数(50%占空比)
 * 
 * @Arguments: 
 * @param: led 灯编号枚举,类型 LED_ENUM
 * @param: type 0 只亮一下,1 闪烁,2 常亮,其他 熄灭
 * @param: interval 闪烁间隔,时间单位100mstype为1时有效)
 * @param: time 亮的时间 单位100ms(0表示永久有效)
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/27
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void SYS_Dev_HalfBlinkSet(uint32_t led, uint8_t type, uint8_t interval, uint16_t time)
{
#if (SYS_LED_BEEP_EN > 0)

    uint8 ont;
    uint8 offt;
    
    ont = interval >> 1;
    offt = interval - ont;
    
    if(ont < 1)
    {
        ont = 1;
    }
    
    if(offt < 1)
    {
        offt = 1;
    }

    SYS_Dev_OptBlinkSet(led, type, ont, offt, time);
#endif    
}




/************************************************************************
 * @Function: SYS_Dev_HalfBlinkSetAll
 * @Description: LED统一点亮熄灭接口函数(50%占空比)
 * 
 * @Arguments: 
 * @param: type 0 只亮一下, 1 闪烁, 2 常亮,其他 熄灭
 * @param: interval 闪烁间隔,时间单位100mstype为1时有效)
 * @param: time 亮的时间 单位100ms(0表示永久有效)
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/27
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void SYS_Dev_HalfBlinkSetAll(uint8_t type, uint8_t interval, uint16_t time)
{
#if (SYS_LED_BEEP_EN > 0)

    uint8 ont;
    uint8 offt;
    
    ont = interval >> 1;
    offt = interval - ont;
    
    if(ont < 1)
    {
        ont = 1;
    }
    
    if(offt < 1)
    {
        offt = 1;
    }
    
    SYS_Dev_OptBlinkSetAll(type, ont, offt, time);
#endif
}





/************************************************************************
 * @Function: SYS_Dev_OptBlinkSet
 * @Description: LED点亮熄灭接口函数
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
void SYS_Dev_OptBlinkSet(uint32_t led, uint8_t type, uint8_t oninterval, uint8_t offinterval, uint16_t time)
{
#if (SYS_LED_BEEP_EN > 0)
    _IF_TRUE_RETURN_VOID(led >= LED_NUM);    //参数检验
    
    SYS_ENTER_SCRT();

    LedLights* ll = gsp_LedLights + led;    //LED点亮结构体
    ll->type = type;                        //亮灭类型
    _IF_TRUE_DO(oninterval == 0, oninterval = 1);   //点亮间隔不为0
    _IF_TRUE_DO(offinterval == 0, offinterval = 1); //熄灭间隔不为0
    ll->off_interval = offinterval;                 //熄灭间隔
    ll->on_interval = oninterval;                   //点亮间隔
                                            //有效时间
    if(time == 0)
    {
        ll->time = 0xFFFF;
    }
    else
    {
        ll->time = time;
    }
                                            //仅当计数值溢出interval范围外时重置.//用于实现blinkset()的可重复设置.
	if(((!ll->curstt) && (ll->count > ll->off_interval)) || 
       (ll->curstt && (ll->count > ll->on_interval)))
	{
		ll->count = 0;
	}
    
    SYS_EXIT_SCRT();
#endif
}




/************************************************************************
 * @Function: SYS_Dev_OptBlinkSetAll
 * @Description: LED统一点亮熄灭接口函数
 * 
 * @Arguments: 
 * @param: type 0 只亮一下, 1 闪烁, 2 常亮,其他 熄灭
 * @param: oninterval 闪烁间隔,时间单位tick(type为1时有效)
 * @param: offinterval 熄灭间隔,时间单位tick(type为1时有效)
 * @param: time 亮的时间 单位tick(0表示永久有效)
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/27
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void SYS_Dev_OptBlinkSetAll(uint8_t type, uint8_t oninterval, uint8_t offinterval, uint16_t time)
{
#if (SYS_LED_BEEP_EN > 0)
    LedLights* ll;
    uint32 bak;
    
    SYS_ENTER_SCRT();

    _IF_TRUE_DO(oninterval == 0, oninterval = 1);   //点亮间隔不为0
    _IF_TRUE_DO(offinterval == 0, offinterval = 1); //熄灭间隔不为0

    for(uint32 led = 0; led < LED_NUM; led++)
    {
        ll = gsp_LedLights + led;                   //LED点亮结构体
        ll->type = type;                            //亮灭类型
        ll->off_interval = offinterval;             //熄灭间隔
        ll->on_interval = oninterval;               //点亮间隔
                                                    //有效时间
        if(time == 0)
        {
            ll->time = 0xFFFF;
        }
        else
        {
            ll->time = time;
        }
        
        if(led == 0)                        //按照第一个灯的count来赋值.
        {                                   //仅当计数值溢出interval范围外时重置.//用于实现blinkset()的可重复设置.
        	if(((!ll->curstt) && (ll->count > ll->off_interval)) || 
               (ll->curstt && (ll->count > ll->on_interval)))
        	{
        		ll->count = 0;
        	}
            bak = ll->count;
        }
        else
        {  
            ll->count = bak;
        }
    }
    
    SYS_EXIT_SCRT();
#endif
}



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
void SYS_LED_TIMEROC(bool oc)
{
#if (SYS_LED_BEEP_EN > 0)
    if(oc)
    {
//        SYS_Timer_Start(ID_SWTIMER_LED);
        krhino_timer_start(&gst_LedTimer);
    }
    else
    {
//        SYS_Timer_Stop(ID_SWTIMER_LED); 
        krhino_timer_stop(&gst_LedTimer);
    }
#endif
}


