/****************************************Copyright (c)**************************************************
**                                  ________科技有限公司
**                                          开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: dev_led.c
**创   建   人: yzy
**最后修改日期: 2010年09月10日
**描        述: led设备模块
*********************************************************************************************************/
#define EXT_DEV_LED

#include "aos/include.h"
#include "k_types.h"
#include "k_api.h"
#include "drv.h"
#include "hal.h"
#include "gpio.h"

static LedLights sLed[LED_NUM];
static ktimer_t     timer_Led;

extern gpio_dev_t brd_led_table[];

/*******************************************************************************
 * @function_name:  SYS_LED_OnOff
 * @function_file:  dev_gpio.c
 * @描述: LED开关函数
 * @参数: 
 * @param: led(LEDNO): 
 * @param: oc(bool): true点亮, false 熄灭
 * @返回: 
 * @作者: yzy (2010-03-11)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
void SYS_LED_OnOff(uint32 led, uint8_t oc)
{
    _IF_TRUE_RETURN_VOID(led >= LED_NUM);   //参数检验
                                            //获取LED端口指针
//	    GPO_PORTS *gp = (GPO_PORTS*)(gs_LedPort + led);
    gpio_dev_t *gp = brd_led_table+led;
    led_dev_priv * rvs = (led_dev_priv *)(gp->priv);
    if(oc)                                  //输出高电平
    {
        if(*(rvs->rvs))                         //逻辑电平翻转
        {
//	            Chip_GPIO_SetPinState(gp->port->pingrp, gp->port->pinnum, false);
            hal_gpio_output_low(gp);
        }
        else
        {
//	            Chip_GPIO_SetPinState(gp->port->pingrp, gp->port->pinnum, true);
            hal_gpio_output_high(gp);
        }
    }
    else                                    //输出低电平
    {
        if(*(rvs->rvs))
        {
//	            Chip_GPIO_SetPinState(gp->port->pingrp, gp->port->pinnum, true); 
            hal_gpio_output_high(gp);
        }
        else
        {
//	            Chip_GPIO_SetPinState(gp->port->pingrp, gp->port->pinnum, false);
            hal_gpio_output_low(gp);
        }
    }
}





/*******************************************************************************
 * @function_name:  LED_Server
 * @function_file:  dev_gpio.c
 * @描述: LED定时服务程序(100ms周期)
 * 
 * 
 * @参数: 
 * @param: pdata(void*): 
 * 
 * @返回: 
 * @return:  Boolean   
 * @作者: yzy (2010-03-11)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
void LED_Server(void *timer, void *arg)
{                                            //循环处理各LED
    for(uint32 i = 0; i < LED_NUM; i++)
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
			SYS_LED_OnOff(i, true);
			ll->type = 0xFF;            //下一次进来就不亮了
		}
		else if(ll->type == 1)          //闪烁
		{
            if(ll->curstt)              //当前点亮
            {
                if(ll->count >= ll->on_interval)
                {                       
                    SYS_LED_OnOff(i, false);//到间隔时间之后就熄灭
                    ll->curstt = 0;     
                    ll->count = 0;      //计数器清零
                    continue;
                }
            }
            else                        //当前熄灭
            {
                if(ll->count >= ll->off_interval)
                {                       //到间隔时间之后就点亮
                    SYS_LED_OnOff(i, true);
                    ll->curstt = 1;
                    ll->count = 0;      //计数器清零
                    continue;
                }
            }
			ll->count++;                //计数器累加
		}
        else if(ll->type == 2)          //常亮
        {
            SYS_LED_OnOff(i, true);
        }
        else
        {
            SYS_LED_OnOff(i, false);      //其他情况关闭LED
        }
    }
    
    //return true;
}




/*******************************************************************************
 * @function_name:  SYS_LED_Init
 * @function_file:  dev_gpio.c
 * @描述: LED初始化
 * 
 * @参数: 
 * @返回: 
 * @作者: yzy (2010-03-11)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
void SYS_LED_Init(void)
{
    GPO_PORTS* gpo;  
    
    //循环初始化各个LED端口
//	    for(uint32 uc_i = 0; uc_i < LED_NUM; uc_i++)
//	    {
//	        gpo = (GPO_PORTS*)(gs_LedPort + uc_i);  //LED端口信息
//	                                                //配置端口功能,并设为输出模式
//	                                                                                    
//	        Chip_GPIO_SetPinDigit(gpo->port->pingrp, gpo->port->pinnum);
//	        
//	        Chip_GPIO_SetPinPFSEG(gpo->port->pingrp, gpo->port->pinnum, gpo->port->pinseg);
//	        Chip_GPIO_SetPinOutputMode(gpo->port->pingrp, gpo->port->pinnum, gpo->port->modefunc & 0x01);
//	        Chip_GPIO_SetPinDIROutput(gpo->port->pingrp, gpo->port->pinnum);   
//	    }
    
                                                //申请缓存
    gsp_LedLights = &sLed;//(LedLights*)SYS_GetMEM(sizeof(LedLights) * LED_NUM);
    ClearBuffer((uint8*)gsp_LedLights, sizeof(LedLights) * LED_NUM);
    
    
    //创建亮灯服务定时器
//	    SYS_Timer_Create(LED_Server, __NULL, 10, TIMER_ID_LED, false);
//	    krhino_timer_create(&timer_Led, "timer_Led", LED_Server,
//	                            10, 10, 0, 1);                                            

}


/************************************************************************
 * @function: SYS_LED_BlinkSet
 * @描述: LED点亮熄灭接口函数
 * 
 * @参数: 
 * @param: led 操作的LED灯序号
 * @param: type 0 只亮一下,1 闪烁,2 常亮,其他 熄灭
 * @param: interval 闪烁间隔,时间单位100ms(type为1时有效)
 * @param: time 亮的时间 单位1s(0表示永久有效)
 * @返回: 
 * @说明: 
 * @作者: yzy (2013/6/8)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void SYS_LED_BlinkSet(LedNo led, uint8 type, uint8 interval, uint8 time)
{
    if(interval > 1)                //闪烁间隔中1份用于点亮,其余用于熄灭
    {
        SYS_LED_BlinkSet1(led, type, 1, interval - 1, time);
    }
    else
    {
        SYS_LED_BlinkSet1(led, type, 1, 1, time);
    }
}



/************************************************************************
 * @function: SYS_LED_BlinkSetAll
 * @描述: LED统一点亮熄灭接口函数
 * 
 * @参数: 
 * @param: type 0 只亮一下, 1 闪烁, 2 常亮,其他 熄灭
 * @param: interval 闪烁间隔,时间单位100ms(type为1时有效)
 * @param: time 亮的时间 单位1s(0表示永久有效)
 * @返回: 
 * @说明: 
 * @作者: yzy (2013/6/8)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void SYS_LED_BlinkSetAll(uint8 type, uint8 interval, uint8 time)
{
    if(interval > 1)                //闪烁间隔中1份用于点亮,其余用于熄灭
    {
        SYS_LED_BlinkSetAll1(type, 1, interval - 1, time);
    }
    else
    {
        SYS_LED_BlinkSetAll1(type, 1, 1, time);
    }
}




/************************************************************************
 * @function: SYS_LED_BlinkSet1
 * @描述: LED点亮熄灭接口函数
 * 
 * @参数: 
 * @param: led 操作的LED灯序号
 * @param: type 0 只亮一下, 1 闪烁, 2 常亮,其他 熄灭
 * @param: oninterval 点亮间隔,时间单位100ms(type为1时有效)
 * @param: offinterval 熄灭间隔,时间单位100ms(type为1时有效)
 * @param: time 亮的时间 单位1s(0表示永久有效)
 * @返回: 
 * @说明: 
 * @作者: yzy (2013/6/8)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void SYS_LED_BlinkSet1(LedNo led, uint8 type, uint8 oninterval, uint8 offinterval, uint8 time)
{
    _IF_TRUE_RETURN_VOID(led >= LED_NUM);    //参数检验
    
    CPSR_ALLOC();
    RHINO_CPU_INTRPT_DISABLE();

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
        ll->time = time * 10;
    }
                                            //仅当计数值溢出interval范围外时重置.//用于实现blinkset()的可重复设置.
	if(((!ll->curstt) && (ll->count > ll->off_interval)) || 
       (ll->curstt && (ll->count > ll->on_interval)))
	{
		ll->count = 0;
	}
    
    RHINO_CPU_INTRPT_ENABLE();
}




/************************************************************************
 * @function: SYS_LED_BlinkSetAll1
 * @描述: LED统一点亮熄灭接口函数
 * 
 * @参数: 
 * @param: type 0 只亮一下, 1 闪烁, 2 常亮,其他 熄灭
 * @param: oninterval 闪烁间隔,时间单位100ms(type为1时有效)
 * @param: offinterval 熄灭间隔,时间单位100ms(type为1时有效)
 * @param: time 亮的时间 单位1s(0表示永久有效)
 * @返回: 
 * @说明: 
 * @作者: yzy (2013/6/8)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void SYS_LED_BlinkSetAll1(uint8 type, uint8 oninterval, uint8 offinterval, uint8 time)
{
    LedLights* ll;
    uint32 bak;
    
    CPSR_ALLOC();
    RHINO_CPU_INTRPT_DISABLE();


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
            ll->time = time * 10;
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
    
    RHINO_CPU_INTRPT_ENABLE();
}


/************************************************************************
 * @function: SYS_LED_TIMEROC
 * @描述: LED定时器开关函数
 * 
 * @参数: 
 * @param: oc(bool): true启动, false 停止
 * @返回: 
 * @说明: 
 * @作者: yzy (2013/6/8)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void SYS_LED_TIMEROC(uint8 oc)
{
    if(oc)
    {
        krhino_timer_start(&timer_Led);
    }
    else
    {
        krhino_timer_stop(&timer_Led);
    }
}


