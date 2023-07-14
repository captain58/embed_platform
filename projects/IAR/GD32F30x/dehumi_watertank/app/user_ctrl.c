#include "sys.h"
#include "public.h"
#include "App_Public.h"
#include "user_func.h"


extern ST_WATER_CTRL       gst_water_ctrl;
extern uint32_t gul_lcd_remaind_times;
#include "paradef.h"
uint8_t Water_Ctrl_Init()
{
    memset((uint8_t *)&gst_water_ctrl,0,sizeof(ST_WATER_CTRL));
    gst_water_ctrl.onoff = CON_WATER_CTRL_ON;
    ST_WATER_CTRL sater_ctrl;
    GD_Para_RW(WATER_CTRL_PARA_ADDR, &sater_ctrl, sizeof(ST_WATER_CTRL), false);
    if(sater_ctrl.crc == DoCrcDlms(&sater_ctrl, sizeof(ST_WATER_CTRL) - 2, 0xffff))
    {
        memcpy((uint8_t *)&gst_water_ctrl, &sater_ctrl, sizeof(ST_WATER_CTRL));
    }
    gst_water_ctrl.onoff = CON_WATER_CTRL_ON;
    Water_Ctrl_Set_Onoff(gst_water_ctrl.onoff);
    Water_Ctrl_Set_Auto(gst_water_ctrl.auto_manmual);
    return 0;

}

uint8_t Water_Ctrl_Set_Auto(uint8_t flag)
{
    if(flag != gst_water_ctrl.auto_manmual)
    {
        gst_water_ctrl.auto_manmual = flag;
        gst_water_ctrl.crc = DoCrcDlms(&gst_water_ctrl, sizeof(ST_WATER_CTRL) - 2, 0xffff);
        //GD_Para_RW(WATER_CTRL_PARA_ADDR, &gst_water_ctrl, sizeof(ST_WATER_CTRL), true);
        
        Water_Ctrl_Close();
        Water_Disp_Close();

    }
    SYS_LCD_Set(CON_LCD_AUTO_STT, flag);
    SYS_LCD_Set(CON_LCD_MANUAL_STT, !flag);
    return 0;
}

uint8_t Water_Ctrl_Set_Onoff(uint8_t flag)
{
    if(flag != gst_water_ctrl.onoff)
    {
        gst_water_ctrl.onoff = flag;
        gst_water_ctrl.crc = DoCrcDlms(&gst_water_ctrl, sizeof(ST_WATER_CTRL) - 2, 0xffff);
        //GD_Para_RW(WATER_CTRL_PARA_ADDR, &gst_water_ctrl, sizeof(ST_WATER_CTRL), true);
    }
    if(0 == flag)
    {
        SYS_LCD_Onoff(flag);
        Water_Ctrl_Close();
        SYS_Dev_OptBlinkSet(SYS_LED_RUN, 3, 50, 50, 0);
    }
    else
    {
        Water_Ctrl_Set_Auto(gst_water_ctrl.auto_manmual);
        SYS_LCD_Onoff(flag);
        SYS_Dev_OptBlinkSet(SYS_LED_RUN, 1, 50, 50, 0);
    }
    return 0;
}

uint8_t Water_Ctrl_WakeUp(uint8_t force)
{
    int ret = 0;
    
    SYS_Dev_OptBlinkSet(SYS_LED_BUZZ, 0, 0, 0, 0);
    if(gst_water_ctrl.onoff != CON_WATER_CTRL_OFF)
    {
        SYS_LCD_Onoff(CON_WATER_CTRL_ON);
        ret = 2;
        if(gst_water_ctrl.onoff == CON_WATER_CTRL_IDEL)
            ret = 1;

        
        gst_water_ctrl.onoff = CON_WATER_CTRL_ON;
        gul_lcd_remaind_times = CON_LCD_REMAIND_TIME_MAX;

    }

    
    return ret;
}

uint8_t Water_Ctrl_Sleep(void)
{
    SYS_LCD_Onoff(CON_WATER_CTRL_IDEL);
    gst_water_ctrl.onoff = CON_WATER_CTRL_IDEL;
    return 0;
}

uint8_t Water_Disp_Close(void)
{
    SYS_LCD_Set(CON_LCD_PUMP_MOTOR_STT, 0);
    SYS_LCD_Set(CON_LCD_PUMP_MOTOR_FAN1_STT, 0);
    SYS_LCD_Set(CON_LCD_PUMP_MOTOR_FAN2_STT, 0);
    SYS_LCD_Set(CON_LCD_PUMP_WATER_ARROWS1_STT, 0);
    SYS_LCD_Set(CON_LCD_PUMP_WATER_ARROWS2_STT, 0);
    SYS_LCD_Set(CON_LCD_PUMP_WATER_ARROWS3_STT, 0);
    SYS_LCD_Set(CON_LCD_PUMP_WATER_ARROWS4_STT, 0);
    SYS_LCD_Set(CON_LCD_PUMP_WATER_ARROWS5_STT, 0);
    SYS_LCD_Set(CON_LCD_PUMP_WATER_ARROWS6_STT, 0);
    
    
    
    
    SYS_LCD_Set(CON_LCD_DEPUMP_MOTOR_STT, 0);
    SYS_LCD_Set(CON_LCD_DEPUMP_MOTOR_FAN1_STT, 0);
    SYS_LCD_Set(CON_LCD_DEPUMP_MOTOR_FAN2_STT, 0);
    SYS_LCD_Set(CON_LCD_DEPUMP_WATER_ARROWS1_STT, 0);
    SYS_LCD_Set(CON_LCD_DEPUMP_WATER_ARROWS2_STT, 0);
    SYS_LCD_Set(CON_LCD_DEPUMP_WATER_ARROWS3_STT, 0);
    SYS_LCD_Set(CON_LCD_DEPUMP_WATER_ARROWS4_STT, 0);
    SYS_LCD_Set(CON_LCD_DEPUMP_WATER_ARROWS5_STT, 0);
    SYS_LCD_Set(CON_LCD_DEPUMP_WATER_ARROWS6_STT, 0);
    
    return 0;
}
//uint8_t guc_pump_tick = 0;
extern uint32_t g_timer_tick;
extern ST_WATER_STT gst_water_stt;
uint8_t Water_Disp_Pump(void)
{
    int i = 0;
    SYS_LCD_Set(CON_LCD_PUMP_MOTOR_STT, 1);
    SYS_LCD_Set(CON_LCD_DEPUMP_MOTOR_STT, 0);
    
    SYS_LCD_Set(CON_LCD_PUMP_MOTOR_FAN1_STT, g_timer_tick % 2);
    SYS_LCD_Set(CON_LCD_PUMP_MOTOR_FAN2_STT, !(g_timer_tick % 2));
  
    for(i = 0; i < 6; i++)
    {
        if(i == g_timer_tick % 6 )
        {
            SYS_LCD_Set(CON_LCD_PUMP_WATER_ARROWS1_STT + i, 1);
        }
        else
        {
            SYS_LCD_Set(CON_LCD_PUMP_WATER_ARROWS1_STT + i, 0);
        }
    }
    
    return 0;
}

uint8_t Water_Disp_Drain(void)
{
    int i = 0;
    SYS_LCD_Set(CON_LCD_PUMP_MOTOR_STT, 0);
    SYS_LCD_Set(CON_LCD_DEPUMP_MOTOR_STT, 1);
    
    SYS_LCD_Set(CON_LCD_DEPUMP_MOTOR_FAN1_STT, g_timer_tick % 2);
    SYS_LCD_Set(CON_LCD_DEPUMP_MOTOR_FAN2_STT, !(g_timer_tick % 2));
  
    for(i = 0; i < 6; i++)
    {
        if(i == g_timer_tick % 6 )
        {
            SYS_LCD_Set(CON_LCD_DEPUMP_WATER_ARROWS1_STT + i, 1);
        }
        else
        {
            SYS_LCD_Set(CON_LCD_DEPUMP_WATER_ARROWS1_STT + i, 0);
        }
    }
    
    return 0;
}

#ifdef MASTER_NODE
extern ST_WATER_STT gst_sub_node_water_stt;
#endif
uint8_t Water_Ctrl_Drain(void)
{
    gst_water_stt.motor_stt = CON_MOTOR_STT_DRAIN;
    gst_water_stt.tick = g_tick_count;
    Water_Disp_Close();
    Water_Disp_Drain();

    if(gst_sub_node_water_stt.cur_stt == CON_WATER_TANK_STT_ERR || 
                        gst_sub_node_water_stt.cur_stt >= CON_WATER_TANK_STT_HIGH ||
                        gst_water_stt.cur_stt  == CON_WATER_TANK_STT_ERR )
        return 1;                        


    SYS_GPO_Out(GPO_DRAIN_WATER, true);
    SYS_GPO_Out(GPO_PUMP_WATER, false);



    return 0;
}

uint8_t Water_Ctrl_Pump(void)
{
    gst_water_stt.motor_stt = CON_MOTOR_STT_PUMP;
    gst_water_stt.tick = g_tick_count;
    
    Water_Disp_Close();
    Water_Disp_Pump();

    if(gst_sub_node_water_stt.cur_stt <= CON_WATER_TANK_STT_LOW_MID ||
                        gst_water_stt.cur_stt  == CON_WATER_TANK_STT_ERR)
    {
        return 1;         
    }

    SYS_GPO_Out(GPO_PUMP_WATER, true);
    SYS_GPO_Out(GPO_DRAIN_WATER, false);


    return 0;
}

uint8_t Water_Ctrl_Close(void)
{
    SYS_GPO_Out(GPO_PUMP_WATER, false);
    SYS_GPO_Out(GPO_DRAIN_WATER, false);
    gst_water_stt.motor_stt = CON_MOTOR_STT_IDEL;
    gst_water_stt.tick = g_tick_count;
    Water_Disp_Close();

    return 0;
}

uint8_t Water_para_Need_Refresh()
{
    gst_water_stt.para_save_tick = 3;
    gst_water_stt.para_save_flag = 1;
    return 0;
}

uint8_t Water_para_Cycle()
{

    if(gst_water_stt.para_save_flag)
    {
        gst_water_stt.para_save_tick --;
        if(gst_water_stt.para_save_tick == 0)
        {
            gst_water_stt.para_save_flag = 0;
            gst_water_ctrl.crc = DoCrcDlms(&gst_water_ctrl, sizeof(ST_WATER_CTRL) - 2, 0xffff);
            GD_Para_RW(WATER_CTRL_PARA_ADDR, &gst_water_ctrl, sizeof(ST_WATER_CTRL), true);
            
        }
    }
    return 0;
}


uint8_t Timer_Redeuce(uint8_t pos)
{
    TIME tm = *GetTime();

    if(pos == CON_TIME_SET_SITE_YEAR)
    {
        tm.year --;
        if(tm.year > 100) tm.year = 0;
    }
    else if(pos == CON_TIME_SET_SITE_MON)
    {
        tm.month --;
        if(tm.month > 12) tm.month = 1;
        else if(tm.month <= 0 ) tm.month = 12;
    }
    else if(pos == CON_TIME_SET_SITE_DAY)
    {
        uint32_t ulmin = Calendar2Sec(&tm, 0);
        ulmin -= 1440 * 60;
        
        Sec2Calendar((uint8_t *)&tm, ulmin);
    }
    else if(pos == CON_TIME_SET_SITE_HOUR)
    {
        tm.hour --;
        if(tm.month >= 24) tm.month = 0;
    }
    else if(pos == CON_TIME_SET_SITE_MIN)
    {
        tm.min --;
        tm.hour --;
        if(tm.month >= 60) tm.month = 0;
        
    }



    SYS_MCU_WriteDateTime(&tm);
    return 0;
}

uint8_t Timer_Add(uint8_t pos)
{
    TIME tm = *GetTime();

    if(pos == CON_TIME_SET_SITE_YEAR)
    {
        tm.year ++;
        if(tm.year > 100) tm.year = 0;
    }
    else if(pos == CON_TIME_SET_SITE_MON)
    {
        tm.month ++;
        if(tm.month > 12) tm.month = 1;
        else if(tm.month <= 0 ) tm.month = 12;
    }
    else if(pos == CON_TIME_SET_SITE_DAY)
    {
        uint32_t ulmin = Calendar2Sec(&tm, 0);
        ulmin += 1440 * 60;
        
        Sec2Calendar((uint8_t *)&tm, ulmin);

    }
    else if(pos == CON_TIME_SET_SITE_HOUR)
    {
        tm.hour ++;
        if(tm.month >= 24) tm.month = 0;
    }
    else if(pos == CON_TIME_SET_SITE_MIN)
    {
        tm.min ++;
        if(tm.month >= 60) tm.month = 0;
        
    }



    SYS_MCU_WriteDateTime(&tm);
    return 0;

}



