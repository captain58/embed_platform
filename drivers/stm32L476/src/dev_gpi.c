/****************************************Copyright (c)*************************
**                               	________科技有限公司
**                                     		开发部
**
**
**--------------文件信息-------------------------------------------------------
**文   件   名: dev_gpio.h
**创   建   人: yzy
**最后修改日期: 2010年3月10日
**描        述: 普通输入输出(包括LED输出)端口管理进程函数配置头文件
**注        意:
**--------------历史版本信息---------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2010年3月10日
** 描　述: 原始版本
******************************************************************************/
#define EXT_DEV_GPI

#include "aos/include.h"
//	#include "k_types.h"
//	#include "k_api.h"
//	#include "lib_app.h"
//	#include "hal.h"
#include "drv.h"
#include "gpio.h"
#include "tasks.h"

static GPIO sGpio;
#if SYS_INT_EN > 0
extern const InterruptID* const gss_IntSID[NO_OF_INTERRUPT];
#endif

#if (SYS_LGPI_EN > 0)
//	static ktimer_t     timer_Lgpi;

/*******************************************************************************
 * @function_name:  SYS_LGPI_Scan
 * @function_file:  gpi.c
 * @描述: 慢速输入端口扫描事件
 * 
 * 
 * @参数: 
 * @param: pdata(void*): 
 * 
 * @返回: 
 * @return:  Boolean   
 * @作者: yzy (2010-03-10)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
//	Boolean SYS_LGPI_Scan(void* pdata)
extern gpio_dev_t brd_gpi_table[];
void SYS_LGPI_Scan(void *timer, void *arg)
{
    uint8   uc_i;
    gpio_dev_t* lp;
    uint32_t val = 0;
#if LGPI_KEY_NUM > 0
//	    uint8 tkid;
//	
//	    extern const KTaskDeclare __TKDeclare[SYS_TK_NUM];
//	    const KTaskDeclare* dec = __TKDeclare;
//	    
//	    for(tkid = 0; tkid < SYS_TK_NUM; tkid++)
//	    {
//	        if(guc_MsgApplied[tkid] & Bit_Map8[MSG_CLS_GPIO])
//	        {
//	            break;
//	        }
//	    }
//	    if(tkid >= SYS_TK_NUM)
//	        return;
    //按键输入口状态扫描(含去抖功能及按键保持判断)
    for(uc_i = 0; uc_i < LGPI_KEY_NUM; uc_i++)
    {
        lp = (brd_gpi_table + uc_i);
                                                //当前高电平.判断下降事件
//	        if(lp->pullup)
//	        {
//	            Chip_GPIO_SetPinState(lp->pingrp, lp->pinnum, true);
//	            sleep_ms(100);
//	        }
        hal_gpio_input_get(lp, &val);

        if(gsp_GpioStt->gpistt & (0x01 << uc_i))
        {
            //uint8 p = Chip_GPIO_GetMaskedPortValue(lp->port->pingrp);
 
            if(val == 0)//低
            //if(!((p >> lp->port->pinnum) & 1))
            {
                gsp_GpioStt->cnt[uc_i]++;       //计数器累加
            }
            else                                //清计数器
            {
                gsp_GpioStt->cnt[uc_i] = 0;
            }
            gsp_GpioStt->lastcnt[uc_i] = 0;     //高电平状态下按键保持计数器清零
        }
        else                                    //当前低电平
        {
            if(val == GPIO_PIN_SET)//高
            {
                gsp_GpioStt->cnt[uc_i]++;       //计数器累加
            }
            else                                //低电平
            {
                gsp_GpioStt->cnt[uc_i] = 0;     //计数器清零
                if(gsp_GpioStt->lastcnt[uc_i] <= SYS_LGPI_LAST)
                {
                    gsp_GpioStt->lastcnt[uc_i]++;   //低电平状态下按键保持计数器累加
                }
            }
        }
//	        if(lp->pullup)
//	        {
//	            Chip_GPIO_SetPinState(lp->pingrp, lp->pinnum, false);
//	        }
        
                                                //按键事件
        if(gsp_GpioStt->cnt[uc_i] >= SYS_LGPI_GATE)
        {
            CPSR_ALLOC();
            RHINO_CPU_INTRPT_DISABLE();        
            //下降沿事件
            gsp_GpioStt->keyfevt |= gsp_GpioStt->gpistt & (0x01 << uc_i);
            gsp_GpioStt->gpistt ^= (0x01 << uc_i);//状态取反
            gsp_GpioStt->keyrevt |= gsp_GpioStt->gpistt & (0x01 << uc_i);//上升沿事件
            RHINO_CPU_INTRPT_ENABLE();

            gsp_GpioStt->cnt[uc_i] = 0;         //计数器清零
            
        }
                                                //按键保持事件判断
        if(gsp_GpioStt->lastcnt[uc_i] == SYS_LGPI_LAST)
        {
//	            gsp_GpioStt->lastcnt[uc_i] = SYS_LGPI_LAST - SYS_LGPI_GATE;
            gsp_GpioStt->keylevt |= (0x01 << uc_i);
        }
    }
#endif
    
    //普通GPI输入端口扫描(无去抖功能)
//	    for(uc_i = LGPI_KEY_NUM; uc_i < LGPI_PORT_NUM; uc_i++)
//	    {
//	        lp = (GPI_PORTS*)(gs_LGPIPort + uc_i);
//	
//	        if(lp->pullup)
//	        {
//	            Chip_GPIO_SetPinState(lp->pingrp, lp->pinnum, true);
//	        }
//	        
//	        if(Chip_GPIO_GetPinState(lp->port->pingrp, lp->port->pinnum) == true)//高电平
//	        {
//	            gsp_GpioStt->gpistt |= (0x01 << uc_i);
//	        }
//	        else                                    //低电平
//	        {
//	            gsp_GpioStt->gpistt &= ~(0x01 << uc_i);
//	        }
//	
//	        if(lp->pullup)
//	        {
//	            Chip_GPIO_SetPinState(lp->pingrp, lp->pinnum, false);
//	        }
//	    }
    
    //return true;
}
#endif




#if (SYS_FGPI_EN > 0)
//	static ktimer_t     timer_Fgpi;

/*******************************************************************************
 * @function_name:  SYS_FGPI_Scan
 * @function_file:  dev_gpio.c
 * @描述: 快速输入端口扫描处理,主要是停电来电指示端口
 * 
 * @参数: 
 * 
 * @返回: 
 * @return:  Boolean   
 * @作者: yzy (2010-03-12)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
//	Boolean SYS_FGPI_Scan(void* pdata)
void SYS_FGPI_Scan(void *timer, void *arg)

{
    uint8   uc_i;
    GPI_PORTS* lp;
    
    for(uc_i = 0; uc_i < FGPI_PORT_NUM; uc_i++)
    {
        lp = (GPI_PORTS*)(gs_FGPIPort + uc_i);
        
        if(lp->pullup)
        {
            Chip_GPIO_SetPinState(lp->pingrp, lp->pinnum, true);
        }
                                                //当前高电平.判断下降事件
        if(gsp_GpioStt->fgpistt & (0x01 << uc_i))
        {
            if(Chip_GPIO_GetPinState(lp->port->pingrp, lp->port->pinnum) == false)//低
            {
                gsp_GpioStt->fcnt[uc_i]++;      //计数器累加
            }
            else                                //清计数器
            {
                gsp_GpioStt->fcnt[uc_i] = 0;
            }
        }
        else                                    //当前低电平
        {
            if(Chip_GPIO_GetPinState(lp->port->pingrp, lp->port->pinnum) == true)//高
            {
                gsp_GpioStt->fcnt[uc_i]++;      //计数器累加
            }
            else                                //低电平
            {
                gsp_GpioStt->fcnt[uc_i] = 0;    //计数器清零
            }
        }
        if(lp->pullup)
        {
            Chip_GPIO_SetPinState(lp->pingrp, lp->pinnum, false);
        }
        
                                                //按键事件
        if(gsp_GpioStt->fcnt[uc_i] >= SYS_FGPI_GATE)
        {                                       //下降沿事件
            gsp_GpioStt->fgpifevt |= gsp_GpioStt->fgpistt & (0x01 << uc_i);
            gsp_GpioStt->fgpistt ^= (0x01 << uc_i);//状态取反
            gsp_GpioStt->fgpirevt |= gsp_GpioStt->fgpistt & (0x01 << uc_i);//上升沿事件
            gsp_GpioStt->fcnt[uc_i] = 0;           //计数器清零
        }
    }
}
#endif

#if (SYS_HALL_EN > 0)
SCallback * g_func_hall = NULL;

static void SYS_HALL_cb(void *timer, void *arg)
{
    InterruptID * id = (InterruptID *)arg;

//	    if(id->hallch == 0)
//	    {
    if(g_func_hall != NULL)
    {
        //intHallCallback(id->hallch);
        ((SCallback)g_func_hall)(id->hallch);
    }
//	    }
//	    else if(id->hallch == 1)
//	    {
//	        intHallCallback(2);
//	    }
}
#endif
/*******************************************************************************
 * @function_name:  SYS_GPI_Init
 * @function_file:  gpi.c
 * @描述: 
 * 
 * @参数: 
 * @返回: 
 * @作者: yzy (2010-03-10)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
void SYS_GPI_Init(void)
{
#if ((SYS_LGPI_EN > 0) || (SYS_FGPI_EN > 0))
    gpio_dev_t* lp;
#endif
    uint8 uc_i = 0;
    uint32_t val = 0;

                                            //申请缓存
    gsp_GpioStt = &sGpio;
    ClearBuffer((uint8*)gsp_GpioStt, sizeof(GPIO));
    memset(&gsp_GpioStt->gpistt, 0xFF, sizeof(gsp_GpioStt->gpistt));//慢速GPI端口初始状态
    memset(&gsp_GpioStt->fgpistt, 0xFF, sizeof(gsp_GpioStt->fgpistt));//快速GPI端口初始状态
//	    gsp_GpioStt->gpistt = 0xFFFFFFFF;       //慢速GPI端口初始状态
//	    gsp_GpioStt->fgpistt = 0xFFFFFFFF;      //快速GPI端口初始状态
    
#if (SYS_FGPI_EN > 0)
                                            //初始化为输入状态
    for(uint8 uc_i = 0; uc_i < FGPI_PORT_NUM; uc_i++)
    {
        lp = (GPI_PORTS*)(gs_FGPIPort + uc_i);
                                            //配置端口功能,并设为输入模式
                                                                                
        Chip_GPIO_SetPinDigit(lp->port->pingrp, lp->port->pinnum);
        
        Chip_GPIO_SetPinPFSEG(lp->port->pingrp, lp->port->pinnum, lp->port->pinseg);
        Chip_GPIO_SetPinOutputMode(lp->port->pingrp, lp->port->pinnum, lp->port->modefunc & 0x01);
        
        Chip_GPIO_SetPinDIRInput(lp->port->gpio, lp->port->pingrp, lp->port->pinnum);
    }
                                            //创建快速端口扫描定时器
//	    SYS_Timer_Create(SYS_FGPI_Scan, __NULL, 1, TIMER_ID_FGPI, false);
//	    krhino_timer_create(&timer_Fgpi, "timer_Fgpi", SYS_FGPI_Scan,
//	                            10, 1, 0, 1);                                            
#endif
    
#if (SYS_LGPI_EN > 0)
                                            //初始化为输入状态
//	    for(uint8 uc_i = 0; uc_i < GPI_NUM; uc_i++)
//	    {
//	        lp = (brd_gpi_table + uc_i);
//	                                            //配置端口功能,并设为输入模式
//	        Chip_GPIO_SetPinDigit(lp->port->pingrp, lp->port->pinnum);
//	        
//	        Chip_GPIO_SetPinPFSEG(lp->port->pingrp, lp->port->pinnum, lp->port->pinseg);
//	        Chip_GPIO_SetPinOutputMode(lp->port->pingrp, lp->port->pinnum, lp->port->modefunc & 0x01);
//	
//	        Chip_GPIO_SetPinDIRInput(lp->port->pingrp, lp->port->pinnum);
//	    }
                                            //创建慢速端口扫描定时器
//	    SYS_Timer_Create(SYS_LGPI_Scan, __NULL, 10, TIMER_ID_LGPI, false);
//	    krhino_timer_create(&timer_Lgpi, "timer_Lgpi", SYS_LGPI_Scan,
//	                            2, 2, 0, 1);                                            
#if LGPI_KEY_NUM > 0
    //按键输入口状态扫描(含去抖功能及按键保持判断)
//	    for(uc_i = 0; uc_i < LGPI_KEY_NUM; uc_i++)
//	    {
//	        lp = (brd_gpi_table + uc_i);
//	                                                //当前高电平.判断下降事件
//	//	        if(lp->pullup)
//	//	        {
//	//	            Chip_GPIO_SetPinState(lp->pingrp, lp->pinnum, true);
//	//	            sleep_ms(100);
//	//	        }
//	        hal_gpio_input_get(lp, &val);
//	        if(val == GPIO_PIN_RESET)//低
//	        {
//	            gsp_GpioStt->gpistt ^= (0x01 << uc_i);//状态取反
//	        }
//	
//	    }
#endif

#endif

#if (SYS_INT_EN > 0)
//	    for(uint8 uc_i = 0; uc_i < NO_OF_INTERRUPT; uc_i++)
//	    {
//	        if(gss_IntSID[uc_i]->mode == 0)
//	        {
//	            Init_Intx(gss_IntSID[uc_i], (timer_cb_t __far *)SYS_LGPI_Scan);
//	        }
//	        else if(gss_IntSID[uc_i]->mode == 1)
//	        {
//	            Init_Intx(gss_IntSID[uc_i], (timer_cb_t __far *)SYS_HALL_cb);
//	        }
//	    }
#endif    

}


//	void SYS_HALL_Init(SCallback __far * hall)
//	{
//	    g_func_hall = hall;
//	}



/*******************************************************************************
 * @function_name:  SYS_GPI_EvtMessageLoop
 * @function_file:  dev_gpio.c
 * @描述: GPI端口事件消息分发机制
 * 
 * @参数: 
 * @返回: 
 * @作者: yzy (2010-03-13)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
void SYS_GPI_EvtMessageLoop(void)
{
    uint8 tkid;
    uint8 msg = 0xff;
    
    extern const KTaskDeclare __TKDeclare[SYS_TK_NUM];
    const KTaskDeclare* dec = __TKDeclare;
    
    for(tkid = 0; tkid < SYS_TK_NUM; tkid++)
    {
        msg = 0xff;
        if(guc_MsgApplied[tkid] & Bit_Map8[MSG_CLS_GPIO])
        {
            if(gsp_GpioStt->fgpifevt & ~gsp_GpioStt->fgpifevtbak)          //快速端口下降沿
            {
                msg = MSG_FIFEVT;
                
            }
            if(gsp_GpioStt->fgpirevt & ~gsp_GpioStt->fgpirevtbak)          //快速端口上升沿
            {
                msg = MSG_FIREVT;
//	                SYS_Message_Send(MSG_FIREVT, tkid);
            }
            if(gsp_GpioStt->keyfevt & ~gsp_GpioStt->keyfevtbak)           //慢速端口下降沿
            {
                msg = MSG_LIFEVT;
//	                SYS_Message_Send(MSG_LIFEVT, tkid);
            }
            if(gsp_GpioStt->keyrevt & ~gsp_GpioStt->keyrevtbak)           //慢速端口上升沿
            {
                msg = MSG_LIREVT;
//	                SYS_Message_Send(MSG_LIREVT, tkid);
            }
            if(gsp_GpioStt->keylevt & ~gsp_GpioStt->keylevtbak)           //慢速端口保持
            {
                msg = MSG_LILEVT;
//	                SYS_Message_Send(MSG_LILEVT, tkid);
            }
            if(msg != 0xff)
            {
                krhino_buf_queue_send(dec[tkid].ktask->msg, &msg, 1);
            }
        }
    }
    CPSR_ALLOC();
    RHINO_CPU_INTRPT_DISABLE();
            //yzy:解决以下风险:当判断到第N个进程时,evt发生变化,这该变化无法在前N个进程中得到体现
    gsp_GpioStt->fgpifevtbak = gsp_GpioStt->fgpifevt;
    gsp_GpioStt->fgpirevtbak = gsp_GpioStt->fgpirevt;
    gsp_GpioStt->keyfevtbak = gsp_GpioStt->keyfevt;
    gsp_GpioStt->keyrevtbak = gsp_GpioStt->keyrevt;
    gsp_GpioStt->keylevtbak = gsp_GpioStt->keylevt;
    
    RHINO_CPU_INTRPT_ENABLE();
    
}

















/*******************************************************************************
 * @function_name:  SYS_GPI_GetStt
 * @function_file:  dev_gpio.c
 * @描述: 获取输入口输入状态
 * 
 * 
 * @参数: 
 * @param: type 0:慢速口输入状态
 *              1:快速口输入状态
 * 
 * @返回: 
 * @return:  uint32   
 * @作者: yzy (2018年3月22日)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
uint32 SYS_GPI_GetStt(uint8 type)
{
    if(type == 0)
    {
        return gsp_GpioStt->gpistt;
    }
    else
    {
        return gsp_GpioStt->fgpistt;
    }
}
/*******************************************************************************
 * @function_name:  SYS_GPI_Get_PinStt
 * @function_file:  dev_gpio.c
 * @描述: 获取输入口输入状态
 * 
 * 
 * @参数: 
 * @param: type 0:慢速口输入状态 
 *              1:快速口输入状态
 * @param: pin 对照iomap.h的管脚定义
 * @返回:   true 高电平 false 低电平
 * @return:  bool   
 * @作者: yzy (2018年3月22日)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
bool SYS_GPI_Get_PinStt(uint8 type, GPIENUM pin)
{
    if(type == 0)
    {
        return (bool)(gsp_GpioStt->gpistt & (1 << pin));
    }
    else
    {
        return (bool)(gsp_GpioStt->fgpistt & (1 << pin));
    }

}

/*******************************************************************************
 * @function_name:  SYS_GPI_Event
 * @function_file:  dev_gpio.c
 * @描述: 获取输入口事件
 * 
 * 
 * @参数: 
 * @param: type事件类型,格式为GPI_T
 * 
 * @返回: 
 * @return:  uint16   
 * @作者: yzy (2018年3月22日)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
uint32 SYS_GPI_Event(uint8 type)
{
    uint32 result;
    
    CPSR_ALLOC();
    RHINO_CPU_INTRPT_DISABLE();
    
    switch((GPI_T)type)
    {
		case GPI_TYPE_LIFEVT:              //获取慢速口下降沿事件
            result = gsp_GpioStt->keyfevt;
            gsp_GpioStt->keyfevt = 0;
            break;
        case GPI_TYPE_LIREVT:              //获取慢速口上升沿事件
            result = gsp_GpioStt->keyrevt;
            gsp_GpioStt->keyrevt = 0;
            break;
        case GPI_TYPE_LILEVT:              //获取慢速口保持事件
            result = gsp_GpioStt->keylevt;
            gsp_GpioStt->keylevt= 0;
            break;
        case GPI_TYPE_FIFEVT:              //获取快速口下降沿事件
            result = gsp_GpioStt->fgpifevt;
            gsp_GpioStt->fgpifevt= 0;
            break;
        case GPI_TYPE_FIREVT:              //获取快速口上升沿事件
            result = gsp_GpioStt->fgpirevt;
            gsp_GpioStt->fgpirevt = 0;
            break;
    }
    RHINO_CPU_INTRPT_ENABLE();
    return result;
}

void SYS_GPI_Event_Clear(void)
{
    gsp_GpioStt->fgpirevt = 0;
    gsp_GpioStt->fgpifevt= 0;
    gsp_GpioStt->keylevt= 0;
    gsp_GpioStt->keyrevt = 0;
    gsp_GpioStt->keyfevt = 0;
}


#if SYS_INT_EN > 0

/*******************************************************************************
 * @function_name:  SYS_KEY_Open
 * @function_file:  dev_gpi.c
 * @描述: 打开按键事件
 * 
 * 
 * @参数: 
 * @param: key
 * 
 * @返回: 
 * @return:  uint16   
 * @作者: yzy (2018-03-02)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
void SYS_KEY_Open(const InterruptID* Iid)
{
    Iid->intx_Enable();
}

/*******************************************************************************
 * @function_name:  SYS_KEY_Close
 * @function_file:  dev_gpi.c
 * @描述: 关闭按键事件
 * 
 * 
 * @参数: 
 * @param: key
 * 
 * @返回: 
 * @return:  uint16   
 * @作者: yzy (2018-03-02)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
void SYS_KEY_Close(const InterruptID* Iid)
{
    Iid->intx_Disable();
}

/*******************************************************************************
 * @function_name:  SYS_KEY_Close
 * @function_file:  dev_gpi.c
 * @描述: 按键中断使能
 * 
 * 
 * @参数: 
 * @param: key
 * 
 * @返回: 
 * @return:  uint16   
 * @作者: yzy (2018-03-02)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
void SYS_KEY_Start(void)
{
    for(uint8 uc_i = 0; uc_i < NO_OF_INTERRUPT; uc_i++)
    {
        SYS_KEY_Open((const InterruptID*)gss_IntSID[uc_i]);
    }
}
#endif
