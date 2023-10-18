/****************************************Copyright (c)**************************************************
**                                  ________科技有限公司
**                                          开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: main.c
**创   建   人: yzy
**最后修改日期: 2013年6月8日
**描        述: 应用程序主进程
**注        意:
**--------------历史版本信息----------------------------------------------------------------------------
**创建人: yzy
**版  本: v1.0
**日　期: 2013年6月8日
**描　述: 原始版本
**-------------------------------------------------------------------------------------------------------
** 修改人: 
** 版  本: 
** 描  述: 
********************************************************************************************************/

/*******************************************************************************
**定义这个宏告诉接口头文件你在引用它，注意一个工程只需要在一个c
**文件中定义这个量就行了
********************************************************************************/
#define _USR_MAIN

/*******************************************************************************
**这个宏定义告诉接口文件你现在是在调试模式下，那么平台启动的时候
**将不会对用户程序进行完整性的CRC校验
********************************************************************************/
#ifdef DEBUG
#define _USR_DEBUG
#endif

#define _USR_FLASH
//#define USER_VAR
/*******************************************************************************
**用户程序的每一个源文件都用该引用这个头文件，否则将无法使用系统调用
********************************************************************************/
#define EXT_MAIN

#include "ext.h"
#include <k_api.h>
#include <stdio.h>
#include <stdlib.h>
//#include "sys.h"
#include "public.h"
//#include "bsp.h"

#include "app.h"
#include "pst.h"
//#include "hlv.h"
//#include "farp.h"
//#include "netp.h"
#include "task.h"

#include "paradef.h"
/*******************************************************************************
**用户程序版本号
********************************************************************************/
const __root uint32 gul_UsrFuncVer@FLS_USRVER_ADDR = 0x23033019;
const __root uint8 gucs_PrjCode[6]@FLS_USRPRJ_ADDR = "RTU01";
const __root uint8_t gucs_softVer[]="RF-WT-R(V0.";

const __root uint32 gul_UsrHardcVer = 0x19071701;
const __root uint8_t gucs_HDVer[]="4G-LS-R(V0.";

static char         g_MQ_buf_recv[BUFQUEUE_MSG_MAX+4];

size_t       g_MQ_recv_size;
ktimer_t     g_MainTimer;

cpu_stack_t  gs_MainStack[TASK_MAIN_STKL];
ktask_t      gs_MainHandle;
kbuf_queue_t gs_MainQueue;
char         gc_MQbuf[MSG_BUFF_LEN];
uint8_t guc_netStat = NODE_STATUS_OUT;
uint8_t guc_RegisterStat = NODE_STATUS_OUT;
uint8_t guc_SwitchSeq = 0;

uint8_t guc_AllowLogin = 0;

//	ST_WATER_STT gst_water_level;
ST_WATER_STT        gst_water_stt;
ST_WATER_CTRL       gst_water_ctrl;
#ifdef MASTER_NODE
ST_WATER_STT gst_sub_node_water_stt;
#endif
uint32_t gul_lcd_remaind_times;
extern uint32_t g_timer_tick;
void WDT_IRQHandler(void)
{
    CPSR_ALLOC();
    RHINO_CPU_INTRPT_DISABLE();
    RHINO_CPU_INTRPT_ENABLE();
}
#define CON_LIVE_SLEEP  100

#define CON_LIVE_TIME_OUT 18//(1800 / CON_LIVE_SLEEP)
#define CON_LIVE_REQ_COUNT (10000 / CON_LIVE_SLEEP)

uint8_t get_run_stt_by_sensor(ST_WATER_SENSOR * pst_sensor);
/*******************************************************************************
 * @function_name:  HB_TaskLiveReq
 * @function_file:  main.c
 * @描述: 发送保活请求
 * 
 * 
 * @参数: 
 * 
 * @返回: 
 * @作者: yzy (2011-03-28)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
uint8_t HB_TaskLiveReq(int count)
{
                                            //保活判断
    extern const KTaskDeclare __TKDeclare[SYS_TK_NUM];
    const KTaskDeclare* dec;
    uint8 msg = MSG_LIVE;
    if((count % CON_LIVE_REQ_COUNT) != 0)
    {
        for(uint32 ut_i = 0; ut_i < (SYS_TK_NUM); ut_i++) 
        {
            if(gucs_LiveCnt[ut_i] > CON_LIVE_TIME_OUT)			//3分钟未回复保活消息
            {
                LOG_DEBUG("soft watch dog thread %d could be died,sys will reboot!!! \n", ut_i);
                return SYS_ERR_FT;
            }        
        }
        return SYS_ERR_OK;
    }
    for(uint32 ut_i = 0; ut_i < (SYS_TK_NUM); ut_i++)  //排除最高和最低进程,最高进程执行软件定时器喂狗
    {
        dec = __TKDeclare + ut_i;
        if((dec->ktask == __NULL) || (dec->stklen == 0) 
            || (dec->ktask->tbl == 0) || dec->ktask->msg == NULL)
        {                                   //进程存在且支持消息机制
            continue;
        }
        
//	        if(ut_i == gs_TkTcbCur->tkid)       //当前进程排除在外
//	        {
//	            continue;
//	        }
        
//	        gs_OS.Message_Send(MSG_LIVE, ut_i);
        krhino_buf_queue_send(dec->ktask->msg, &msg, 1);
        
        CPSR_ALLOC();
        RHINO_CPU_INTRPT_DISABLE();
                              //多进程共享保护
        gucs_LiveCnt[ut_i]++;
        RHINO_CPU_INTRPT_ENABLE();
        
        if(gucs_LiveCnt[ut_i] > CON_LIVE_TIME_OUT)			//3分钟未回复保活消息
        {
//	            SYS_Reset();
            LOG_DEBUG("soft watch dog thread %d could be died,sys will reboot!!! \n", ut_i);
            return SYS_ERR_FT;
        }
    }
    
    return SYS_ERR_OK;
}

/*******************************************************************************
 * @function_name:  HB_RetLive
 * @function_file:  
 * @描述: 其它进程的保活处理
 * 
 * 
 * @参数:
 * 
 * @返回: 
 * @作者: yzy (2012-02-16)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
void HB_RetLive(uint8 tkid)
{
    
    CPSR_ALLOC();
    RHINO_CPU_INTRPT_DISABLE();
                             //多进程共享保护
    gucs_LiveCnt[tkid] = 0;
    RHINO_CPU_INTRPT_ENABLE();
}


/*******************************************************************************
 * @function_name:  HB_LiveInit
 * @function_file:  
 * @描述: 进程保活机制初始化
 * 
 * 
 * @参数:
 * 
 * @返回: 
 * @作者: yzy (2012-02-16)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
void HB_LiveInit(void)
{
    ClearBuffer(gucs_LiveCnt, sizeof(gucs_LiveCnt));
}

uint8 Log_Send(uint8* buffer, uint16 len)
{
    return SYS_SER_Write(UART_CHANNEL_DEBUG, buffer, len, 1000);
}



/*******************************************************************************
 * @function_name:  Main_PreInit
 * @function_file:  main.c
 * @描述: 应用层main变量初始化
 * 
 * 
 * @参数: 
 * 
 * @返回: 
 * @作者: yzy (2011-03-03)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
void Main_PreInit(void)
{
    gl_SysRunStt = 0;
    gl_SysRunStt |= HRS_PARALD;

}

/*******************************************************************************
 * @function_name:  SYS_MAIN_Init
 * @function_file:  main.c
 * @描述: 应用层main初始化
 * 
 * 
 * @参数: 
 * 
 * @返回: 
 * @作者: yzy (2011-03-03)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
void SYS_MAIN_Init(void)
{
   
      //系统状态寄存器初始化
    
    gl_SysRunStt = 0;                      //运行状态初始为0
    
    ClearBuffer((uint8*)&gs_ErrLog, sizeof(gs_ErrLog));
    
    for(uint8 ut_i = 0; ut_i < sizeof(gucs_LiveCnt); ut_i++)
    {
       gucs_LiveCnt[ut_i] = 0;				//保活进程计数器清0
    }
	
    
    gl_SysRunStt |= HRS_PARALD;
    gl_SysRunStt |= HRS_APPRUN;

//	    SYS_LED_BlinkSetAll1(0, 2, 1);
//	    gs_OS.TK_Sleep(200);
//	    SYS_LED_BlinkSetAll(0, 2, 1);
//	    gs_OS.TK_Sleep(20);
    SYS_Dev_OptBlinkSetAll(2, 0, 0, 0);               //所有灯齐闪
    gs_OS.TK_Sleep(200);
    
    SYS_Dev_OptBlinkSetAll(3, 0, 0, 0);               //所有灯灭

//	    
    Hash_Table_Init();

     //SER_Open(UART_CHANNEL_DEBUG, TDB_MODE_R | TDB_MODE_W);
    LogInit(LOG_LEVEL_DEBUG,256, (log_fun_ptr *)Log_Send);
     //SER_Open(UART_CHANNEL_GPRS, TDB_MODE_R | TDB_MODE_W);
//	    gl_SysRunInit = 1;
    Get_Net_Parameter();
    User_Parameter_Init();


    
    GD_Para_RW(REGISTER_FLAG, &guc_RegisterStat, 1, false);
    GD_Para_RW(PARENT_ADDR, nParentMacAddr, METER_ADDRESS_LENGTH_MAX, false);
#if (SYS_LCD_HT1621 > 0)    
    SYS_LCD_Init();
#endif

#if (SYS_LCD_HT1621 > 0)    
    Water_Ctrl_Init();
#endif
    SYS_Dev_OptBlinkSet(SYS_LED_RUN, 2, 100, 100, 0);    //运行灯秒闪(overlay last configuration)


    LoadSystemParam(PARA_TYPE_FARP);

    memset((uint8_t *)&gst_water_stt,0,sizeof(ST_WATER_STT));
    
#ifdef MASTER_NODE    
//	    SYS_Dev_OptBlinkSet(SYS_LED_BUZZ, 0, 0, 0, 1);
    SYS_BUZZ_Passive_Blink(0);

    memset((uint8_t *)&gst_sub_node_water_stt,0,sizeof(ST_WATER_STT));
#endif

    if(SYS_GPI_GetLPort(GPI_LOWLEVEL))
    {
        gst_water_stt.st_sensor.low = 1;
    }
    else
    {
        gst_water_stt.st_sensor.low = 0;
    }
    if(SYS_GPI_GetLPort(GPI_LOWMIDDLELEVEL))
    {
        gst_water_stt.st_sensor.low_mid = 1;
    }
    else
    {
        gst_water_stt.st_sensor.low_mid = 0;
    }
    if(SYS_GPI_GetLPort(GPI_HIGHMIDDLELEVEL))
    {
        gst_water_stt.st_sensor.high_mid = 1;
    }
    else
    {
        gst_water_stt.st_sensor.high_mid = 0;
    }
    if(SYS_GPI_GetLPort(GPI_HIGHLEVEL))
    {
        gst_water_stt.st_sensor.high = 1;
    }
    else
    {
        gst_water_stt.st_sensor.high = 0;
    }

    gst_water_stt.cur_stt = get_run_stt_by_sensor(&gst_water_stt.st_sensor);
    gul_lcd_remaind_times = CON_LCD_REMAIND_TIME_MAX;



    
}



extern uint8_t g_ucPutcharEn;
uint32_t gul_remain_set_tick = 0;
uint32_t gul_time_set_tick = 0;
#define CON_REMAIN_SET_ADD_INTERVAL 5
/*******************************************************************************
 * @function_name:  KeyProc
 * @function_file:  main.c
 * @描述: 按键处理
 * 
 * 
 * @参数: 
 * 
 * @返回: 
 * @作者: yzy (2014-07-28)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
void KeyProc(uint8 key)
{
    uint32 event;
    uint8 msg = 0;
    if(key == MSG_LIFEVT)
    {
        event = SYS_GPI_Event(GPI_TYPE_LIFEVT);  //慢速口下降沿状态(按键松开)
        
        if(event & 1)               //KEY1
        {
	        LOG_DEBUG("key 1 failing!\n");
            gs_SysVar.terstt.bit.DI0linked = 1; 
            
        }
        
        if(event & 2)               //KEY2
        {
            gs_SysVar.terstt.bit.blecheck = 0;
        }
        
        if(event & 4)               //KEY3
        {
//#ifdef MASTER_NODE
            SYS_RF_Set_FallingEdge(GPI_DIO1);
//#else
////	            guc_SwitchOnOff = 0;
//#endif
            LOG_DEBUG("~~~~~~~~GPI_DIO1 failing!~~~~~~~~~~~~~~~\n");
        }
        
        if(event & 8)               //KEY4
        {
	
            SYS_RF_Set_FallingEdge(GPI_DIO2);
            //LOG_DEBUG("key 4 failing!\n");
            LOG_DEBUG("~~~~~~~~GPI_DIO2 failing!~~~~~~~~~~~~~~~\n");
        }
        
        if(event & 0x10)               //KEY5
        {
            LOG_DEBUG("key 5 failing!\n");
//            if(guc_SwitchOnOff != 0)
//            {
//                extern kbuf_queue_t gs_TKSlvQueue;
//                krhino_buf_queue_send(&gs_TKSlvQueue, &msgidA[MSG_EVENT_CHANGE], 1);
//
//            }
//            guc_SwitchOnOff = 0;
        }
        if(event & 0x20)               //KEY6
        {
            LOG_DEBUG("key 6 failing!\n");
        }   
        if(event & 0x40)               //KEY7
        {
            LOG_DEBUG("key 7 failing!\n");
        }   
        if(event & 0x80)               //KEY8
        {
            LOG_DEBUG("key 8 failing!\n");
        }           
        if(event & CON_KEY9_BIT)               //KEY9
        {
            LOG_DEBUG("key set failing!\n");
#if (SYS_LCD_HT1621 > 0)
            if(2 == Water_Ctrl_WakeUp(0))
            {
                if(gst_water_stt.time_set_flag)
                {
                    gst_water_stt.time_set_site ++;
                    if(gst_water_stt.time_set_site > CON_TIME_SET_SITE_MINL)
                    {
                        gst_water_stt.time_set_site  = CON_TIME_SET_SITE_YEAR;
                    }
                }
            }

#endif            
        }   
        
        if(event & CON_KEY10_BIT)               //KEY10
        {
            LOG_DEBUG("key add pump failing!\n");
#if (SYS_LCD_HT1621 > 0)
            if(2 == Water_Ctrl_WakeUp(0))
            {
                if(gst_water_stt.time_set_flag)
                {   
//	                    if(gst_water_ctrl.remain_day > 0)
//	                        gst_water_ctrl.remain_day--;
//	
//	                    Water_para_Need_Refresh();
                    Timer_Add(&gst_water_stt.time, gst_water_stt.time_set_site);
                }
                else if(gst_water_stt.remain_set_flag)
                {   
//	                    if(gst_water_ctrl.remain_day > 0)
//	                        gst_water_ctrl.remain_day--;
                    remain_Add(&gst_water_ctrl.remain_day, gst_water_stt.remain_set_site);
                    Water_para_Need_Refresh();
                }
                else
                {
                    if(gst_water_ctrl.auto_manmual == CON_MOTOR_CTRL_MANUAL)
                    {
                        if(gst_water_stt.motor_stt != CON_MOTOR_STT_PUMP)
                        {
                            Water_Ctrl_Pump();
                        }
                        else
                        {
                            Water_Ctrl_Close();
                        }
                    }
                }
                
                gul_remain_set_tick = g_tick_count;
            }

#endif            
        }   
        if(event & CON_KEY11_BIT)               //KEY11
        {
            LOG_DEBUG("key reduce depump failing!\n");
#if (SYS_LCD_HT1621 > 0)
            if(2 == Water_Ctrl_WakeUp(0))
            {
                if(gst_water_stt.time_set_flag)
                {   
//	                    if(gst_water_ctrl.remain_day > 0)
//	                        gst_water_ctrl.remain_day--;
//	
//	                    Water_para_Need_Refresh();
                    Timer_Redeuce(&gst_water_stt.time, gst_water_stt.time_set_site);
                    

                }
                else if(gst_water_stt.remain_set_flag)
                {
//	                    if(gst_water_ctrl.remain_day < 99)
//	                        gst_water_ctrl.remain_day++;
                    remain_Reduce(&gst_water_ctrl.remain_day, gst_water_stt.remain_set_site);
                    Water_para_Need_Refresh();
                }
                else
                {
                    if(gst_water_ctrl.auto_manmual == CON_MOTOR_CTRL_MANUAL)
                    {
//	                	    Water_Ctrl_Drain();
                        if(gst_water_stt.motor_stt != CON_MOTOR_STT_DRAIN)
                        {
                            Water_Ctrl_Drain();
                        }
                        else
                        {
                            Water_Ctrl_Close();
                        }                        
                    }
                }
                
                gul_remain_set_tick = g_tick_count;
            }

#endif            
        }   
        if(event & CON_KEY12_BIT)               //KEY12
        {
            LOG_DEBUG("key auto failing!\n");
#if (SYS_LCD_HT1621 > 0)
            if(2 == Water_Ctrl_WakeUp(0))
            {
                Water_Ctrl_Set_Auto(!gst_water_ctrl.auto_manmual);
            }
#endif            
        }           
        if(event & CON_KEY13_BIT)               //KEY13
        {
            LOG_DEBUG("key onoff failing!\n");
#if (SYS_LCD_HT1621 > 0)
            if(2 == Water_Ctrl_WakeUp(1))
            {
                Water_Ctrl_Set_Onoff(0);

            }else if(0 == Water_Ctrl_WakeUp(1))
            {
                
                SYS_BUZZ_Passive_Blink(CON_PASSIVE_BUZZ_SOUND_1);
//                if(SYS_LCD_Get_Onoff())
//                {
//                    Water_Ctrl_Set_Onoff(0);
//                }
//                else
                {
                    Water_Ctrl_Set_Onoff(1);
                }
            }

#endif
        }   
if(event & CON_KEY14_BIT)               //KEY13
        {
            LOG_DEBUG("key remaind failing!\n");
#if (SYS_LCD_HT1621 > 0)
            if(2 == Water_Ctrl_WakeUp(0))
            {
                if(gst_water_stt.remain_set_flag)
                {
                    gst_water_stt.remain_set_site ++;
                    if(gst_water_stt.remain_set_site > 1)
                    {
                        gst_water_stt.remain_set_site  = 0;
                    }
                }



                gul_remain_set_tick = g_tick_count;

                
            }

#endif
        }           
        
    }
    if(key == MSG_LILEVT)
    {
        event = SYS_GPI_Event(GPI_TYPE_LILEVT);  //慢速口保持状态(按键长按)
        
        if(event & 1)               //KEY1
        {
	        LOG_DEBUG("key 1 keep !\n");
            SYS_Dev_OptBlinkSet(SYS_LED_MATCH, 1, 10, 10, 0);
            
            SYS_Dev_OptBlinkSet(SYS_LED_RUN, 1, 10, 10, 0);
            guc_AllowLogin = 1;
            //bBroadMeterEnable = 1;
            sysSlotTime=0;
#ifdef MASTER_NODE            
            Cltor_init();
            Reset_Hash_Table();
//            SYS_Dev_OptBlinkSet(GPIO_LED_SUB1_NORM, 3, 0, 0, 0);
//            SYS_Dev_OptBlinkSet(GPIO_LED_SUB1_ERR, 3, 0, 0, 0);
////            SYS_Dev_OptBlinkSet(GPIO_LED_SUB2_NORM, 3, 0, 0, 0);
//            SYS_Dev_OptBlinkSet(GPIO_LED_SUB2_ERR, 3, 0, 0, 0);
#else
            guc_netStat = NODE_STATUS_OUT;
#endif
        }
        
        if(event & 2)               //KEY2
        {

//		        LOG_DEBUG("key 2 keep!\n");
        }
        
        if(event & 4)               //KEY3
        {
//		        LOG_DEBUG("key 3 keep!\n");
        }
        
        if(event & 8)               //KEY4
        {
//	            LOG_DEBUG("key 4 keep!\n");
        }
        
        if(event & 0x10)               //KEY5
        {
            LOG_DEBUG("key 5 keep!\n");
        }  
        if(event & 0x20)               //KEY6
        {
            LOG_DEBUG("key 6 keep!\n");
        }  
        if(event & 0x40)               //KEY7
        {
            LOG_DEBUG("key 7 keep!\n");
        }  
        if(event & 0x80)               //KEY8
        {
            LOG_DEBUG("key 8 keep!\n");
        }      
        if(event & CON_KEY9_BIT)               //KEY9
        {
            LOG_DEBUG("key set keep!\n");
            gst_water_stt.time_set_flag = !gst_water_stt.time_set_flag;
            gst_water_stt.time_set_site = 0;
            gul_remain_set_tick = g_tick_count;
            if(gst_water_stt.time_set_flag)
            {
                gst_water_stt.time = *GetTime();
            }
            else
            {
                SYS_MCU_WriteDateTime(&gst_water_stt.time);
            }

        }
        
        if(event & CON_KEY10_BIT)               //KEY10
        {
            LOG_DEBUG("key reduce keep!\n");
        }  
        if(event & CON_KEY11_BIT)               //KEY11
        {
            LOG_DEBUG("key add keep!\n");
        }  
        if(event & CON_KEY12_BIT)               //KEY12
        {
            LOG_DEBUG("key auto keep!\n");
        }  
        if(event & CON_KEY13_BIT)               //KEY13
        {
            LOG_DEBUG("key onoff keep!\n");
        }    
        if(event & CON_KEY14_BIT)               //KEY13
        {
            LOG_DEBUG("key remaind keep!\n");
            
            gst_water_stt.remain_set_flag = !gst_water_stt.remain_set_flag;
            gul_remain_set_tick = g_tick_count;
            gst_water_stt.remain_set_site = 0;
            
        }         
    }  
    if(key == MSG_LIREVT)
    {
        event = SYS_GPI_Event(GPI_TYPE_LIREVT);  //慢速口下降沿状态(按键松开)
        
        if(event & 1)               //KEY1
        {
	        LOG_DEBUG("key 1 right!\n");

            guc_AllowLogin = 0;
#ifdef MASTER_NODE
            SYS_Dev_OptBlinkSet(SYS_LED_RUN, 2, 50, 50, 0);
#else
//	            SYS_Dev_OptBlinkSet(SYS_LED_RUN, 1, 100, 100, 0);
//	            if(NODE_STATUS_LOGIN == guc_netStat)
//	            {
//	                SYS_Dev_OptBlinkSet(SYS_LED_RUN, 1, 50, 50, 0);    //运行灯秒闪(overlay last configuration)
//	            }
//	            SYS_Dev_OptBlinkSet(SYS_LED_MATCH, 0, 100, 100, 0);
#endif

        }
        
        if(event & 2)               //KEY2
        {
            //蓝牙断开建立
            g_ucPutcharEn = 0;
//	            hal_gpio_output_high(&brd_gpio_table[GPO_BLE_UART_CTL]);
//            gs_GPIO.GPO_Out(GPO_BLE_UART_CTL,true);

//		        LOG_DEBUG("key 2 right!\n");
        }
        
        if(event & 4)               //KEY3
        {
//	            LOG_DEBUG("key 3 right!\n");
//		        gs_SysVar.terstt.bit.DI0linked = 0; 
            LOG_DEBUG("~~~~~~~~GPI_DIO1 right!~~~~~~~~~~~~~~~\n");
            SYS_RF_Set_RightEdge(GPI_DIO1);

#ifdef MASTER_NODE
//	            SYS_RF_Set_FallingEdge(GPI_DIO1);
#else
//	            guc_SwitchOnOff = 1;
#endif
        }
        
        if(event & 8)               //KEY4
        {
//	            LOG_DEBUG("key 4 right!\n");
            LOG_DEBUG("~~~~~~~~GPI_DIO2 right!~~~~~~~~~~~~~~~\n");
            SYS_RF_Set_RightEdge(GPI_DIO2);

        }
        
        if(event & 0x10)               //KEY4
        {
            LOG_DEBUG("key 5 right!\n");
//	            if(guc_SwitchOnOff != 1)
//	            {
//	//	                extern kbuf_queue_t gs_RFMngQueue;
//	//	                krhino_buf_queue_send(&gs_RFMngQueue, &msgidA[MSG_SWITCH_CHANGE], 1);
//	//                          SYS_Dev_OptBlinkSet(GPIO_BUZ_CARD, 0, 0, 0, 0); 
//	                extern kbuf_queue_t gs_TKSlvQueue;
//	                krhino_buf_queue_send(&gs_TKSlvQueue, &msgidA[MSG_EVENT_CHANGE], 1);
//	
//	            }
//	            guc_SwitchOnOff = 1;
            
        }
        if(event & 0x20)               //KEY4
        {
            LOG_DEBUG("key 6 right!\n");
        }
        if(event & 0x40)               //KEY4
        {
	        LOG_DEBUG("key 7 right!\n");
        }
        if(event & 0x80)               //KEY4
        {
	        LOG_DEBUG("key 8 right!\n");
        }  
        if(event & CON_KEY9_BIT)               //KEY9
        {
            LOG_DEBUG("key set right!\n");
        }
        if(event & CON_KEY10_BIT)               //KEY10
        {
	        LOG_DEBUG("key reduce right!\n");
        }
        if(event & CON_KEY11_BIT)               //KEY11
        {
	        LOG_DEBUG("key add right!\n");
        }   
        if(event & CON_KEY12_BIT)               //KEY12
        {
	        LOG_DEBUG("key auto right!\n");
        }
        if(event & CON_KEY13_BIT)               //KEY13
        {
	        LOG_DEBUG("key onoff right!\n");
        }    
        if(event & CON_KEY14_BIT)               //KEY13
        {
	        LOG_DEBUG("key remaind right!\n");
        }        
    }        
}


/************************************************************************
 * @function: SYS_MAIN_Reset
 * @描述: 
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2013/7/10)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	void SYS_MAIN_Reset(void)
//	{
//	    gs_OS.TK_Sleep(200);                //等待2秒再复位,待pst进程数据回复完毕
//	    TDev* dev;
//	    dev = gs_DEVProc.DEV_Open(DEV_NO_SYS, 0 ,__NULL);
//	    ((TDevSys*)dev->devfunc)->Reset();
//	    gs_DEVProc.DEV_Close(dev);
//	}



/*******************************************************************************
 * @function_name:  MAIN_HardCheck
 * @function_file:  
 * @描述: 上电自检硬件
 * 
 * 
 * @参数:
 * 
 * @返回: 
 * @作者: yzy (2014-11-29)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
//	void MAIN_HardCheck()
//	{
//	    uint8 buffertst[10] = { 0x11, 0x22, 0x33, 0x00, 0x55, 0x66, 0x77, 0x88, 0xff, 0xaa};
//	    uint8 buffertemp[10];
//	    memset(buffertemp, 0x99, sizeof(buffertemp));
//	    gs_FarpVar.ftst = 0;
//	    
//	#if SYS_FRAM_EN > 0
//	    Fram_SafeRead((uint8 *)&gs_EventTime, FM_PUSH_TIME, sizeof(gs_EventTime));   //从铁电读取上次推送时间
//	    Fram_SafeWrite((uint8 *)&buffertst, FM_PUSH_TIME, sizeof(buffertst));       //将测试数据写入flash
//	    Fram_SafeRead(buffertemp, FM_PUSH_TIME, sizeof(buffertemp));                //将测试数据从flash中读出
//	    Fram_SafeWrite((uint8 *)&gs_EventTime, FM_PUSH_TIME, sizeof(gs_EventTime));   //往铁电重新写入flash
//	#endif  
//	    WriteFlash(DB_PARA, buffertst, sizeof(buffertst), 100);                 //写外部Flash
//	    ReadFlash(DB_PARA, buffertemp, sizeof(buffertemp), 100);                //读外部Flash
//	    if(CmpBuffer((uint8 *)&buffertst, buffertemp, 10) != 0)                     //数据比对不一致
//	    {
//	        gs_FarpVar.ftst |= 0x01;
//	    }
//	  
//	    PST_UartDownInteract();                //定时交互帧
//	    gul_PstInteractSecCnt = 0;      //定期交互秒间隔计数器清0
//	}
//	


/*******************************************************************************
 * @function_name:  UpdateSysTimeStruct
 * @function_file:  main.c
 * @描述: 更新系统时标
 * 
 * @参数: 
 * 
 * @返回: 
 * @作者: yzy (2014-11-28)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
//	void UpdateSysTimeStruct(void)
//	{
//	    const TIME* time = gs_RealTime.GetTime();
//	
//	    gs_OS.LockMMTK();
//	    MoveBuffer((uint8*)&time->sec, gs_SysTime.YMDhms, 6);
//	    InvBuffer(gs_SysTime.YMDhms, 6);
//	    gs_SysTime.mins = CalcMinuteHex(gs_SysTime.YMDhms[0], gs_SysTime.YMDhms[1], gs_SysTime.YMDhms[2], gs_SysTime.YMDhms[3], gs_SysTime.YMDhms[4]);
//	    gs_OS.UnLockMMTK();
//	}



/*******************************************************************************
 * @function_name:  MAIN_UpdataChkProc
 * @function_file:  main.c
 * @描述: 升级文件校验
 * 
 * @参数: 
 * 
 * @返回: 
 * @作者: yzy (2014-11-28)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
//	void MAIN_UpdataChkProc(void)
//	{
//	    if(gs_DlmsUpdata.FlagState == F_WaitVerify)
//	    {
//	        if(UPDATA_CheckCRC_BOOT())      //执行文件校验
//	        {
//	            gs_DlmsUpdata.FlagState=F_VerifyOk;  //校验成功
//	            STR_SetBits(gs_FarpVar.MdAlt, 1, MDALM_UPDATA_OK, 1);   //模块升级成功
//	        }
//	        else
//	        {
//	            gs_DlmsUpdata.FlagState=F_VerifyFailed;  //校验失败
//	            STR_SetBits(gs_FarpVar.MdAlt, 1, MDALM_UPDATA_BAD, 1);    //模块升级失败
//	        }
//	        WriteFlashWithCRC(DB_UPDATA_PARA, (uint8 *)&gs_DlmsUpdata, sizeof(ST_DLMSUPDATA), PARA_ADDR_UPDATA);
//	    }
//	}
#ifdef MASTER_NODE


#define CON_MOTOR_WORK_AWAY_MAX_TIME      (90000)//30s 10分钟


const uint8_t *auto_str[] =
{
    "auto",
    "manul",
};
const uint8_t *motor_str[] =
{
    "idel",
    "pump",
    "drain",
    "pause"
};

void MAIN_UpdataWaterPump(void)
{
    LOG_INFO("before run[%s] motor[%s] master[%d] sub[%d]!!!\n", 
        auto_str[gst_water_ctrl.auto_manmual%2],motor_str[gst_water_stt.motor_stt%4], 
        gst_water_stt.cur_stt, gst_sub_node_water_stt.cur_stt);
    uint32_t clac_tick = 0;
    if(CON_WATER_CTRL_OFF != gst_water_ctrl.onoff)
    {
        if(gst_water_ctrl.auto_manmual == CON_MOTOR_CTRL_AUTO)
        {

            if(gst_water_stt.motor_stt == CON_MOTOR_STT_IDEL)
            {
                if(gst_sub_node_water_stt.cur_stt == CON_WATER_TANK_STT_LOW /*&& gst_sub_node_water_stt.cur_stt == CON_WATER_TANK_STT_LOW_MID*/)
                {
                    //打开加水泵
                    if(gst_water_stt.cur_stt > CON_WATER_TANK_STT_LOW)
                    {
                        SYS_GPO_Out(GPO_DRAIN_WATER, true);
                        SYS_GPO_Out(GPO_PUMP_WATER, false);
                        gst_water_stt.motor_stt = CON_MOTOR_STT_DRAIN;
                        gst_water_stt.tick = g_tick_count;
                    }
                }

                else if(gst_sub_node_water_stt.cur_stt == CON_WATER_TANK_STT_HIGH_MORE)
                {
                    //打开抽水泵
                    if(gst_water_stt.cur_stt < CON_WATER_TANK_STT_HIGH_MORE)
                    {
                        SYS_GPO_Out(GPO_PUMP_WATER, true);
                        SYS_GPO_Out(GPO_DRAIN_WATER, false);
                        gst_water_stt.motor_stt = CON_MOTOR_STT_PUMP;
                        gst_water_stt.tick = g_tick_count;
                    }
                }
            }
            else if(gst_water_stt.motor_stt == CON_MOTOR_STT_DRAIN)
            {
                
                if(gst_water_stt.tick > g_tick_count)
                {
                    clac_tick = 0xffffffff - gst_water_stt.tick + g_tick_count;
                }
                else
                {
                    clac_tick = g_tick_count - gst_water_stt.tick;
                }
                LOG_INFO("CON_MOTOR_STT_DRAIN[%d]!!!\n", clac_tick);
                
                if(gst_sub_node_water_stt.cur_stt == CON_WATER_TANK_STT_ERR || 
                    (gst_sub_node_water_stt.cur_stt >= CON_WATER_TANK_STT_HIGH && krhino_ticks_to_ms(clac_tick) > CON_MOTOR_WORK_AWAY_MAX_TIME) ||
                    gst_water_stt.cur_stt  == CON_WATER_TANK_STT_ERR || 
                    (gst_water_stt.cur_stt  == CON_WATER_TANK_STT_LOW && krhino_ticks_to_ms(clac_tick) > CON_MOTOR_WORK_AWAY_MAX_TIME) )
                {
                    SYS_GPO_Out(GPO_PUMP_WATER, false);
                    SYS_GPO_Out(GPO_DRAIN_WATER, false);
                    gst_water_stt.motor_stt = CON_MOTOR_STT_IDEL;
                    Water_Disp_Close();
                }
            }
            else if(gst_water_stt.motor_stt == CON_MOTOR_STT_PUMP)
            {
                if(gst_water_stt.tick > g_tick_count)
                {
                    clac_tick = 0xffffffff - gst_water_stt.tick + g_tick_count;
                }
                else
                {
                    clac_tick = g_tick_count - gst_water_stt.tick;
                }
                LOG_INFO("CON_MOTOR_STT_PUMP[%d]!!!\n", clac_tick);
                
                if((gst_sub_node_water_stt.cur_stt <= CON_WATER_TANK_STT_LOW_MID && krhino_ticks_to_ms(clac_tick) > CON_MOTOR_WORK_AWAY_MAX_TIME)  ||
                    gst_water_stt.cur_stt  == CON_WATER_TANK_STT_ERR || 
                    (gst_water_stt.cur_stt  == CON_WATER_TANK_STT_HIGH_MORE && krhino_ticks_to_ms(clac_tick) > CON_MOTOR_WORK_AWAY_MAX_TIME) )
                {
                    SYS_GPO_Out(GPO_PUMP_WATER, false);
                    SYS_GPO_Out(GPO_DRAIN_WATER, false);
                    gst_water_stt.motor_stt = CON_MOTOR_STT_IDEL;
                    Water_Disp_Close();
                }

            }
        }
        else
        {//手动
            if(gst_water_stt.motor_stt == CON_MOTOR_STT_PUMP)
            {
                if(gst_water_stt.tick > g_tick_count)
                {
                    clac_tick = 0xffffffff - gst_water_stt.tick + g_tick_count;
                }
                else
                {
                    clac_tick = g_tick_count - gst_water_stt.tick;
                }
                LOG_INFO("CON_MOTOR_STT_PUMP[%d]!!!\n", clac_tick);
                
                if((gst_sub_node_water_stt.cur_stt <= CON_WATER_TANK_STT_LOW_MID && krhino_ticks_to_ms(clac_tick) > CON_MOTOR_WORK_AWAY_MAX_TIME) ||
                    gst_water_stt.cur_stt  == CON_WATER_TANK_STT_ERR || 
                    (gst_water_stt.cur_stt  == CON_WATER_TANK_STT_HIGH_MORE && krhino_ticks_to_ms(clac_tick) > CON_MOTOR_WORK_AWAY_MAX_TIME) )
                {
                    SYS_GPO_Out(GPO_PUMP_WATER, false);
                    SYS_GPO_Out(GPO_DRAIN_WATER, false);
                    gst_water_stt.motor_stt = CON_MOTOR_STT_IDEL;
                    Water_Disp_Close();
                }
                else
                {
                    SYS_GPO_Out(GPO_PUMP_WATER, true);
                    SYS_GPO_Out(GPO_DRAIN_WATER, false);
                    
                }
                
            }
            else if(gst_water_stt.motor_stt == CON_MOTOR_STT_DRAIN)
            {
                
                if(gst_water_stt.tick > g_tick_count)
                {
                    clac_tick = 0xffffffff - gst_water_stt.tick + g_tick_count;
                }
                else
                {
                    clac_tick = g_tick_count - gst_water_stt.tick;
                }
                LOG_INFO("CON_MOTOR_STT_DRAIN[%d]!!!\n", clac_tick);
                
                if(gst_sub_node_water_stt.cur_stt == CON_WATER_TANK_STT_ERR || 
                    (gst_sub_node_water_stt.cur_stt >= CON_WATER_TANK_STT_HIGH && krhino_ticks_to_ms(clac_tick) > CON_MOTOR_WORK_AWAY_MAX_TIME) ||
                    gst_water_stt.cur_stt  == CON_WATER_TANK_STT_ERR || 
                    (gst_water_stt.cur_stt  == CON_WATER_TANK_STT_LOW && krhino_ticks_to_ms(clac_tick) > CON_MOTOR_WORK_AWAY_MAX_TIME) )
                {
                    SYS_GPO_Out(GPO_PUMP_WATER, false);
                    SYS_GPO_Out(GPO_DRAIN_WATER, false);
                    gst_water_stt.motor_stt = CON_MOTOR_STT_IDEL;
                    Water_Disp_Close();                    
                }
                else
                {
                    SYS_GPO_Out(GPO_PUMP_WATER, false);
                    SYS_GPO_Out(GPO_DRAIN_WATER, true);
                }                
            }

        }
    }
    LOG_INFO("after Updata Water Pump [%s] master[%d] sub[%d]!!!\n", motor_str[gst_water_stt.motor_stt%4], gst_water_stt.cur_stt, gst_sub_node_water_stt.cur_stt);

}
#endif

/*******************************************************************************
 * @function_name:  MAIN_MinProc
 * @function_file:  main.c
 * @描述: 分处理
 * 
 * @参数: 
 * 
 * @返回: 
 * @作者: yzy (2014-11-28)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
//	void MAIN_MinProc(void)
//	{
//	    UpdateSysTimeStruct();
//	    //检查是否有待升级文件
//	    if(gs_DlmsUpdata.FlagState == F_UpdataFirm)     //允许软件升级标志
//	    {
//	        if((gs_SysTime.mins >= gs_DlmsUpdata.UpdataTime) && (gs_DlmsUpdata.UpdataTime != 0))  //软件升级时间到
//	        {
//	            ActionUpdataProc();     //执行升级
//	        }
//	    } 
//	}

uint8_t get_run_stt_by_sensor(ST_WATER_SENSOR * pst_sensor)
{
    uint8_t ret = CON_WATER_TANK_STT_ERR;
    if(pst_sensor->low == (uint8_t)0 && pst_sensor->low_mid == 0 && pst_sensor->high_mid == 0 && pst_sensor->high == 0)
    {
        ret = CON_WATER_TANK_STT_LOW;
    }
    else if(pst_sensor->low == 1 && pst_sensor->low_mid == 0 && pst_sensor->high_mid == 0 && pst_sensor->high == 0)
    {
        ret = CON_WATER_TANK_STT_LOW_MID;
    }
    else if(pst_sensor->low == 1 && pst_sensor->low_mid == 1 && pst_sensor->high_mid == 0 && pst_sensor->high == 0)
    {
        ret = CON_WATER_TANK_STT_HIGH_MID;
    } 
    else if(pst_sensor->low == 1 && pst_sensor->low_mid == 1 && pst_sensor->high_mid == 1 && pst_sensor->high == 0)
    {
        ret = CON_WATER_TANK_STT_HIGH;
    }    
    else if(pst_sensor->low == 1 && pst_sensor->low_mid == 1 && pst_sensor->high_mid == 1 && pst_sensor->high == 1)
    {
        ret = CON_WATER_TANK_STT_HIGH_MORE;
    }      
    else
    {
        ret = CON_WATER_TANK_STT_ERR;
    }
    return ret;
}

/*******************************************************************************
 * @function_name:  MAIN_SecProc
 * @function_file:  main.c
 * @描述: 秒处理
 * 
 * @参数: 
 * 
 * @返回: 
 * @作者: yzy (2014-11-28)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
void MAIN_SecProc(void)
{
    int i = 0;

    if(SYS_GPI_GetLPort(GPI_LOWLEVEL))
    {
        gst_water_stt.st_sensor.low = 1;
    }
    else
    {
        gst_water_stt.st_sensor.low = 0;
    }
    if(SYS_GPI_GetLPort(GPI_LOWMIDDLELEVEL))
    {
        gst_water_stt.st_sensor.low_mid = 1;
    }
    else
    {
        gst_water_stt.st_sensor.low_mid = 0;
    }
    if(SYS_GPI_GetLPort(GPI_HIGHMIDDLELEVEL))
    {
        gst_water_stt.st_sensor.high_mid = 1;
    }
    else
    {
        gst_water_stt.st_sensor.high_mid = 0;
    }
    if(SYS_GPI_GetLPort(GPI_HIGHLEVEL))
    {
        gst_water_stt.st_sensor.high = 1;
    }
    else
    {
        gst_water_stt.st_sensor.high = 0;
    }    


    
//状态去抖
    for(i = CON_SENSOR_FILTER - 1; i > 0; i --)
    {
        gst_water_stt.ready_stt[i] = gst_water_stt.ready_stt[i - 1];
    }

    gst_water_stt.ready_stt[0] = get_run_stt_by_sensor(&gst_water_stt.st_sensor);

    uint8_t stt = gst_water_stt.ready_stt[0];
//临时状态缓存
    for(i = 1; i < CON_SENSOR_FILTER; i ++)
    {
        if(stt != gst_water_stt.ready_stt[i])
            break;
    }
    if(i >= CON_SENSOR_FILTER)
    {
        if(stt != gst_water_stt.cur_stt)//状态需要切换
        {
            //主节点
            gst_water_stt.last_stt = gst_water_stt.cur_stt;
            gst_water_stt.cur_stt = stt;  
            gst_water_stt.tick = g_tick_count;
#ifndef MASTER_NODE
            //从节点
                       
            extern kbuf_queue_t gs_RFMngQueue;
            krhino_buf_queue_send(&gs_RFMngQueue, &msgidA[MSG_SWITCH_CHANGE], 1);
#endif
        }
    }
#if (SYS_LCD_HT1621 > 0)
    SYS_LCD_Set_Wheel_Water_Level(gst_water_stt.cur_stt, g_timer_tick);
    
    SYS_LCD_Set_Tank_Water_Level(gst_sub_node_water_stt.cur_stt, g_timer_tick);
#endif
#ifdef MASTER_NODE
    LOG_INFO("mod[%d] sub[%d] low[%d] mlow[%d] hmid[%d] high[%d]\n", gst_water_stt.motor_stt, gst_sub_node_water_stt.cur_stt, gst_water_stt.st_sensor.low, 
        gst_water_stt.st_sensor.low_mid, gst_water_stt.st_sensor.high_mid, gst_water_stt.st_sensor.high);
#else
    LOG_INFO("low[%d] mid low[%d] high mid[%d] high[%d]\n", gst_water_stt.st_sensor.low, 
        gst_water_stt.st_sensor.low_mid, gst_water_stt.st_sensor.high_mid, gst_water_stt.st_sensor.high);
    
#endif    

#if (SYS_LCD_HT1621 > 0)
//    if(gst_water_stt.motor_stt == CON_MOTOR_STT_DRAIN)
//    {
//        Water_Disp_Drain();
//    }
//    else if(gst_water_stt.motor_stt == CON_MOTOR_STT_PUMP)
//    {
//        Water_Disp_Pump();
//    }
//    else
//    {
//        Water_Disp_Close();
//    }
    uint32_t clac_tick = 0;
    if(gst_water_stt.remain_set_flag || gst_water_stt.time_set_flag)
    {
        if(gul_remain_set_tick > g_tick_count)
        {
            clac_tick = 0xffffffff - gul_remain_set_tick + g_tick_count;
        }
        else
        {
            clac_tick = g_tick_count - gul_remain_set_tick;
        }
        if(krhino_ticks_to_ms(clac_tick) > CON_SET_TIME_MAX * 1000)
        {
            gst_water_stt.remain_set_flag = 0;
            gst_water_stt.time_set_flag = 0;
        }

    }
    Water_para_Cycle();
#endif

}

static void timer_main(void *timer, void *arg)
{
//	    HB_TaskLiveReq();
//	    if(dispLockTime>0)
//	    {
//	        dispLockTime--;
//	        if(dispLockTime==0)
//	            dispRefresh();    //显示任务            
//	    }
//	    //启动5秒任务
//	    if((timertick%5)==0)
//	    {
//	        powRefresh();
//	        if(dispLockTime==0)
//	        {
//	            dispRefresh();
//	        }
//	    }
//	
//	    //启动24小时任务
//	    if((timertick % 3600)==0)
//	    {
//	        
//	    }  



}

void SYS_TIMER_Init(void)
{
    krhino_timer_create(&g_MainTimer, "main_timer", timer_main,
                        krhino_ms_to_ticks(1000), krhino_ms_to_ticks(1000), 0, 1);
    
}
//#include "dev_lcd_dsbk2348a.h"
void SYS_APP_Init()
{
    UART_Init();
//	
    SYS_FILE_Init();
//    SYS_FILE_Start();
#if SYS_GPO_EN > 0
    SYS_GPO_Init();
#endif
#if SYS_GPI_EN > 0
    SYS_GPI_Init();
#endif
//#if (SYS_AD_EN > 0)    
//    SYS_AD_Init();
//#endif    
////	    OV_LcdInit();
    SYS_RTC_Init();
    SYS_MSG_Init();
//    SYS_LED_Init(); 
    SYS_BlinkDev_Init();
//    HB_LiveInit();
////	    SYS_TIMER_Init();
//    Netp_PreInit(0);                    //线程
//
//    HLV_PreInit();                      //线程
//    Farp_PreInit();                     //线程
////	    PST_PreInit();                      //线程
//    Task_PreInit();                     //线程
//
    PST_Init();
//
//    Main_PreInit();
//    SYS_RF_Init();

}

void SYS_APP_Start()
{
#if SYS_INT_EN > 0
    SYS_KEY_Start();
#endif
    //SYS_RTC_Start();
}

/*******************************************************************************
 * @function_name:  SYS_MAIN_Task
 * @function_file:  main.c
 * @描述:主进程处理函数
 * 
 * 
 * @参数: 
 * 
 * @返回: 
 * @作者: yzy (2011-1-24)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
#include <stdio.h>

#include <aos/kernel.h>
//#include <network/network.h>

int application_start(int argc, char *argv[])
{
    int count = 0;
    SYS_APP_Init();

    printf("nano entry here!\r\n");
    SYS_TASKS_Init();
    

    while(1) {
        if(SYS_ERR_OK == HB_TaskLiveReq(count++))
#ifndef DEBUG        
            HAL_IWDG_Refresh(&hiwdg); //喂狗
#else
            ;
#endif

        aos_msleep(CON_LIVE_SLEEP);
    };
}

extern uint8 nDeviceMacAddr[METER_ADDRESS_LENGTH_MAX];
extern const uint8 sBroadAddrFE[8];

uint32_t gul_lcd_remaind_times = 0;

//uint8_t guc_test = CON_LCD_TEST1;

void Handle_Lcd()
{

    static uint32_t tick_count = 0;
    tick_count++;
    SYS_LCD_Set_Stt(1);
    //SYS_LCD_Set_Byte_Test(g_timer_tick);
    if(gst_water_stt.time_set_flag)
    {
        SYS_LCD_Blink_Time_With_Pos(gst_water_stt.time, 0, tick_count, gst_water_stt.time_set_site);
    }
    else
    {
        
        SYS_LCD_Set_Time(*GetTime(), 0);
    }
    SYS_LCD_Set(CON_LCD_WHEEL_WATER_BOX, 1);
    SYS_LCD_Set(CON_LCD_CHANGE_WATER_STT, 1);
//                  SYS_LCD_Set(CON_LCD_RF_STT, 1);
    if(gst_water_stt.remain_set_flag)
    {
//	        SYS_LCD_Set(CON_LCD_CHANGE_WATER_STT, tick_count % 2);
        SYS_LCD_Blink_Change_Water_Date(gst_water_ctrl.remain_day, tick_count, gst_water_stt.remain_set_site);
    }
    else
    {
//	        SYS_LCD_Set(CON_LCD_CHANGE_WATER_STT, 1);
        SYS_LCD_Set_Change_Water_Date(gst_water_ctrl.remain_day);

    }
    
//                  if(gst_water_stt.time_set_flag)
//                  {
//                      SYS_LCD_Set(CON_LCD_RF_STT, g_timer_tick % 2);
//                  }
//                  else
//                  {
//                      SYS_LCD_Set(CON_LCD_RF_STT, 1);
//                  }                
//                SYS_LCD_Set_Wheel_Water_Level(g_timer_tick % 6);
//                SYS_LCD_Set_Tank_Water_Level(g_timer_tick % 6);

//                for(int i = CON_LCD_TEST1; i < CON_LCD_TEST8+1; i++)
//                {
//                    SYS_LCD_Set(i, g_timer_tick % 2);
//                }
//  //                  SYS_LCD_Set(guc_test, g_timer_tick % 2);
//	    SYS_LCD_Set_Change_Water_Date(gst_water_ctrl.remain_day);

    if(gst_water_stt.motor_stt == CON_MOTOR_STT_DRAIN)
    {
        Water_Disp_Drain(tick_count);
    }
    else if(gst_water_stt.motor_stt == CON_MOTOR_STT_PUMP)
    {
        Water_Disp_Pump(tick_count);
    }
    else
    {
        Water_Disp_Close();
    }
    
    SYS_LCD_Handld();
}
ktimer_t     gst_lcd_timer;
uint32_t gul_master_conn_to=0; 
void SYS_MAIN_Task(void * arg)
{
    TIME time;
                                            //链表消息
    krhino_buf_queue_create(&gs_MainQueue, "main_queue",
                         gc_MQbuf, MSG_BUFF_LEN, BUFQUEUE_MSG_MAX);

    static uint8_t ble_name[14] = {'V','S'};
    ByteArrayBcdToHexString(gs_PstPara.Addr, ble_name+2, 6, 0);
    gul_master_conn_to=0;
    g_ucPutcharEn = 1;

#ifndef MASTER_NODE    
//	    memcpy(nDeviceMacAddr, (uint8_t *)sBroadAddrAA, 8);
    memset(nDeviceMacAddr, 0, 8);
    memset(nParentMacAddr, 0xff, 8);
    memset(nParentMacAddrTemp, 0xff, 8);

    nDeviceMacAddr[0] = 1;
//    HAL_RFID_Init(&gs_MainQueue, ble_name, 14);
    guc_SwitchOnOff = 0;
    guc_SwitchNorErr = 0;
    guc_BuzzerNorErr = 0;
//	    SYS_GPO_Out(GPO_SWITCH_PWR,true);
    msleep(10);
    if(SYS_GPI_GetLPort(GPI_LOWLEVEL))
    {
        guc_SwitchOnOff = 1;
    }
#else
    memset(nDeviceMacAddr, 0, 8);
#endif    

    SYS_MAIN_Init();
                                            //申请获取时间消息
    SYS_MSG_Apply(TASK_MAIN_TKID, MSG_CLS_TM);//*初始化监控机制
    SYS_MSG_Apply(TASK_MAIN_TKID, MSG_CLS_GPIO);
    SYS_MSG_ApplyExt(TASK_MAIN_TKID, MSG_CLS_UART, UART_CHANNEL_DEBUG);
//    printf("\nVS Project %s  Softver[%x] Hardver[%x]!!!\n", gucs_PrjCode, gul_UsrFuncVer, gul_UsrHardcVer);
    LOG_DEBUG("\nVS Project %s  Softver[%x] Hardver[%x]!!!\n", gucs_PrjCode, gul_UsrFuncVer, gul_UsrHardcVer);
    //Flash_Test();
//	    SYS_IFLS_Test();
    uint8_t tmp[10] = {88,1,4,0,0,0,0,0,0,0};
    memset(tmp,0,10);
    GD_Para_RW(F251_PADDR, tmp, 10, false);
#if (SYS_LCD_HT1621 > 0)  
    krhino_timer_create(&gst_lcd_timer, "lcd_timer", Handle_Lcd,
                        krhino_ms_to_ticks(1), krhino_ms_to_ticks(200), 0, 1);    
#endif


    for(;;)
    {   
        krhino_buf_queue_recv(&gs_MainQueue, RHINO_WAIT_FOREVER, g_MQ_buf_recv,
                          &g_MQ_recv_size);
        switch(g_MQ_buf_recv[0])
        {
            case MSG_SEC:
                g_timer_tick++;
#if (SYS_LCD_HT1621 > 0)                
                if(gul_lcd_remaind_times > 0)
                    gul_lcd_remaind_times--;

                if(0 == gul_lcd_remaind_times)
                {
                    Water_Ctrl_Sleep();
                }
                if(gul_master_conn_to > 0)
                {
                    gul_master_conn_to--;
                
                    if(0 == gul_master_conn_to)
                    {
                        LOG_ERROR("water tank lost connection,will close motor!\n");
                        SYS_LCD_Set(CON_LCD_CONNECT_STT, 0);
                        if( gst_water_stt.motor_stt > 0)
                        {
                            Water_Ctrl_Close();
                        }
                    }
                }
#endif
//	                LOG_DEBUG("second ! %d\n", g_timer_tick);
#ifdef MASTER_NODE  

                if(gst_water_stt.motor_stt > 0)
                {
                    MAIN_UpdataWaterPump();
                }
#endif                
            case MSG_WT_LEVEL_CHANGE:    
                MAIN_SecProc();
                break;
#ifdef MASTER_NODE
            case MSG_WT_SUB_LEVEL_CHANGE:
                MAIN_UpdataWaterPump();
                break;
#endif                
            case MSG_MIN:

                LOG_DEBUG("%02d-%02d-%02d %02d:%02d:%02d %02d!\n",((TIME *)GetTime())->year, 
                    ((TIME *)GetTime())->month, ((TIME *)GetTime())->day, 
                    ((TIME *)GetTime())->hour, ((TIME *)GetTime())->min, 
                    ((TIME *)GetTime())->sec, ((TIME *)GetTime())->week);
                break;
            case MSG_DAY:
#if (SYS_LCD_HT1621 > 0)              
                if(!gst_water_stt.time_set_flag && gst_water_ctrl.remain_day > 0)
                {
                    gst_water_ctrl.remain_day--;

                    Water_para_Need_Refresh();
                }
#endif                
                break;
            case MSG_LIFEVT:                //按键下降沿
                KeyProc(MSG_LIFEVT);
//                time.day = 13;
//                time.year = 18;
//                time.month = 3;
//                time.hour = 15;
//                time.min = 23;
//                time.sec = 30;
                //SYS_WriteDateTime(&time);
                break;
                
            case MSG_LIREVT:                //按键上升沿
                KeyProc(MSG_LIREVT);
                break;
                
            case MSG_LILEVT:                //按键保持
                KeyProc(MSG_LILEVT);
                break;
                
            case MSG_MAIN_RESET:            //硬件复位
//	                SYS_MAIN_Reset();
                break;
                
            case MSG_MAIN_UPDATA_CHK:       //升级文件校验
//	                MAIN_UpdataChkProc();
                break;
                
            case MSG_MAIN_HARD_CHK:         //硬件自检
//	                MAIN_HardCheck();
                break;
            case MSG_PSTP:                  //串口消息
//	                PST_RecvProc();
                Setp_RecvProc();
                break;
            case MSG_CARD_INSERT:
                break;
            case MSG_LIVE:                  //回复保活消息
                HB_RetLive(TASK_MAIN_TKID);
                break;
            case MSG_PST_VAR:
//                PST_UpdateVar(); 
                LOG_DEBUG("******MSG_PST_VAR******** !!!!\n");
            default:
                break;
        }
    }

}



/******************************************************************************
**MAIN进程块常量部分定义
******************************************************************************/
const KTaskConst gs_TkMAIN = 
{
    &gs_MainHandle,
    SYS_MAIN_Task,
    gs_MainStack,
    TASK_MAIN_TKID,                           //进程编号
    true,                                    //消息循环机制进程
    TASK_MAIN_STKL,
    "main",
    &gs_MainQueue,
};


#if (RHINO_CONFIG_USER_HOOK > 0)
#include "rtc.h"
#include "hal_rtc_stm32l4.h"

extern void SystemClock_Config(void);
extern int32_t MX_ADC1_DeInit(void);
extern void MX_GPIO_Init(void);
extern void MX_GPIO_Init1(void);
extern void MX_DMA_Init(void);
extern void stm32_soc_peripheral_init(void);
extern void MX_ADC1_Init(void);
extern int at_reset_uart();
extern int at_uart_close();
extern void brd_gpio_suspend(void);
extern void brd_gpio_resume(void);
extern rtc_dev_t g_stRtcDev;
extern uint8 SYS_MODM_Shutdown(void);
extern const _F_UART gs_Uart;
extern void SystemClock_Config_16M(void);
extern int SystemClock_Stop(void);

void hwEnterSleep()
{
    HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
    CPSR_ALLOC();
    RHINO_CPU_INTRPT_DISABLE();
    if(HAL_OK != SystemClock_Stop())
    {
        aos_reboot();
    }
    RHINO_CPU_INTRPT_ENABLE();

}

//	void ResumeSys(void)
//	{
//	    HAL_SuspendTick();
//	    hwEnterSleep();
//	    HAL_ResumeTick();
//	    SystemClock_Config();
//	    
//	    //stm32_soc_peripheral_init();
//	
//	    //at_reset_uart();
//	    
//	    //gs_Uart.Init(PORT_UART_STD, NULL);
//	    //SYS_FILE_Start();
//	    
//	}

void SuspendSleep()
{
//	    at_uart_close();
    SYS_MODM_Shutdown();
    HAL_BLE_ShutDown();
    
//	    gs_Uart.Close(PORT_UART_STD);      
    gs_GPIO.GPO_Out(GPO_485_PWR,false);  
    gs_GPIO.GPO_Out(GPO_BLE_UART_CTL,false);
    SYS_Dev_OptBlinkSetAll(3, 0, 0, 0); 
//	    brd_gpio_suspend();
    //hal_rtc_finalize(&g_stRtcDev);

//	    MX_ADC1_DeInit();
    MX_GPIO_Init();
#ifndef DEBUG        
    HAL_IWDG_Refresh(&hiwdg); //喂狗
#endif

    
}

void ResumeSleep()
{
    uint32_t val = 0;
//	    hal_gpio_input_get(brd_gpi_table[GPI_EXT_PWR], &val);
//	    if(val == 0)
//	    {
//	
//	    }
//	    MX_GPIO_Init();

//	    MX_DMA_Init();
//	//	
//	    MX_ADC1_Init();
    
//    stm32_soc_peripheral_init();
//	//	    hal_rtc_init(&g_stRtcDev);
//	//	//	    brd_gpio_resume();
//	    at_reset_uart();
    
//	    gs_Uart.Init(PORT_UART_STD, NULL);
//    SYS_FILE_Start();
#ifndef DEBUG        
    HAL_IWDG_Refresh(&hiwdg); //喂狗
#endif


}



void krhino_idle_hook(void)
{
    if((gs_SysVar.mLPsysstt & HLV_STT_NENG) == HLV_STT_NENG &&
        gs_SysVar.mLPstt == 0)
    {
        SuspendSleep();
        //HAL_SuspendTick();
        hwEnterSleep();
        //HAL_ResumeTick();
        
        ResumeSleep();
//	        gs_SysVar.mLPsysstt &= ~HLV_STT_NENG; 

    }

}

void krhino_init_hook(void)
{
}

/**
 * This function will provide system start hook
 */
void krhino_start_hook(void)
{
}

/**
 * This function will provide task create hook
 * @param[in]  task  pointer to the task
 */
void krhino_task_create_hook(ktask_t *task)
{
}
/**
 * This function will provide task delete hook
 * @param[in]  task  pointer to the task
 */
void krhino_task_del_hook(ktask_t *task, res_free_t *arg)
{
}

/**
 * This function will provide task abort hook
 * @param[in]  task  pointer to the task
 */
void krhino_task_abort_hook(ktask_t *task)
{
}

/**
 * This function will provide task switch hook
 */
void krhino_task_switch_hook(ktask_t *orgin, ktask_t *dest)
{
}

/**
 * This function will provide system tick hook
 */
void krhino_tick_hook(void)
{
    //countTick();
    //hwSleepDelayReduce();
}

/**
 * This function will provide  krhino_mm_alloc hook
 */
void krhino_mm_alloc_hook(void *mem, size_t size)
{
}
void krhino_idle_pre_hook(void)
{
	
}
#endif



