/****************************************Copyright (c)**************************************************
**                                  ____科技有限公司
**                                          开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: main.c
**创   建   人: yzy
**最后修改日期: 2019年6月1日
**描        述: 配变主系统入口文件
**注        意:用户在使用平台时应该保持自身程序与平台的接口与本文件一致
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2019年6月1日
** 描　述: 原始版本
********************************************************************************************************/
#define EXT_HLV

#include "public.h"
#include "lib_com/calclib_gw.h"
#include "hlv.h"
#include "netp.h"
#include "extapi.h"
#include "alert.h"
#include "inp.h"
//#include "dev_ble.h"
#include "task.h"
cpu_stack_t  gs_TKHlvStack[TASK_HLV_STKL];
ktask_t      gs_TKHlvHandle;
kbuf_queue_t gs_TKHlvQueue;
char         gc_TKHlvQbuf[MSG_BUFF_LEN];
static char         g_TKHlvQ_buf_recv[BUFQUEUE_MSG_MAX+4];
static size_t       g_TKHlvQ_recv_size;

void EVT_SecProc(void);

extern uint8_t g_ucPutcharEn;
extern intptr_t g_ulTcp_fd;
extern kbuf_queue_t gs_TKFarpQueue;
extern uint16 gui_FarpWmRcSec;
/************************************************************************
 * @function: HLV_SecProc
 * @描述: 秒消息
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2013/10/30)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void HLV_SecProc(void)
{
                                            //确保不会长期停止抄表
//	    for(uint8 uc_i = 0; uc_i < MCB_PORT_NUM; uc_i++)
//	    {
//	        if(gs_InpExtracb[uc_i].gbbreak)
//	        {
//	            if(gs_InpExtracb[uc_i].gbbvsec)
//	            {
//	                gs_InpExtracb[uc_i].gbbvsec--;
//	            }
//	            else
//	            {
//	                gs_InpExtracb[uc_i].gbbreak = 0;
//	            }
//	        }
//	    }
    if(SYS_GPI_GetStt(0) & 0x01)
    {
        gs_SysVar.terstt.bit.DI0linked = 0; 
    }
    else
    {
        gs_SysVar.terstt.bit.DI0linked = 1; 

    }
    if(SYS_GPI_GetStt(0) & 0x04)
    {
        gs_SysVar.terstt.bit.DI1linked = 0; 
    }
    else
    {
        gs_SysVar.terstt.bit.DI1linked = 1; 

    }

    uint32_t vol = SYS_AD_GetValue(AD_ID_CHECKVOL);
    gs_SysVar.batVal = vol/100;
    if(gs_SysVar.mDGstt & HLV_STT_USBP)         //升级过程中100ms闪烁
    {
        gs_GPIO.BlinkSet(SYS_LED_RUN, 1, 1, 0);
    }
    else if(gs_SysVar.mDGstt & HLV_STT_USBE)    //U盘存在200ms闪烁
    {
        gs_GPIO.BlinkSet(GPIO_LED_RUN, 1, 2, 0);
    }
    else if(gs_SysVar.mDGstt & (HLV_STT_NENG || HLV_STT_LENG))
    {
         gs_GPIO.BlinkSet1(GPIO_LED_RUN, 1, 1, 10, 0);
    }        
    else                                //正常非联网情况下常亮
    {
         gs_GPIO.BlinkSet1(GPIO_LED_RUN, 1, 5, 5, 0);
    }
#ifdef SYS_LP_BY_AD   
    if(vol >= 14000)
#else
    if(!(gs_GPIO.GPI_GetStt(0) & FGPI_STT_ENG))        //有电
#endif
    {
//	        gs_GPIO.GPO_Out(GPO_BAT_CTL, true);
	
        if(gs_SysVar.mDGstt & HLV_STT_NENG) //(1)
        {
//	            MoveBuffer(gs_FramMisData.tbk , gs_FramMisData.tdt , 6);
//	            ResumeSys();
//            HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/RHINO_CONFIG_TICKS_PER_SECOND);

            if(SYS_GPI_GetStt(0) & 0x02)
            {
                g_ucPutcharEn = 0;
            }
            else
            {
                if((gs_SysVar.mLPstt & HLV_LPTASK_TST) == 0)
                    g_ucPutcharEn = 1;
            }
//            if(!HAL_BLE_Status().bit.chnrdy)
//            {
//                HAL_BLE_Reset();
//            }
            ALRT_ERC14(0);                  //系统运行过程中的上电记录
            if(aos_now_ms() > 5000)//掉电恢复时补报数据，上电5秒前认为是复位
            {
                gss_TaskATimer.atr |= TKT_LPATR_FLAG;
                gs_SysVar.mLPstt |= HLV_LPTASK_RP;
            }
            extern uint16 gui_FarpHdRstCnt;
            gui_FarpHdRstCnt = 119;        
            gui_FarpWmRcSec = 10;
            LOG_INFO("vol normal = %d V\n", vol);
        }
        gs_SysVar.mDGstt &= ~(HLV_STT_NENG | HLV_STT_LENG);  //(2)
        gs_SysVar.mLPsysstt &= ~HLV_STT_NENG;  //(2)

        gs_SysVar.mDGcnt = 0;               //(2)
        gs_SysVar.terstt.bit.pwrstt = 0;
        gs_SysVar.terstt.bit.extpwrstt = 1;
    }
    else                                    //停电
    {
        gs_SysVar.terstt.bit.extpwrstt = 0;
        if(SYS_AD_GetValue(AD_ID_CHECKVOL) < 10000)
        {
//	            gs_GPIO.GPO_Out(GPO_BAT_CTL, false);
            gs_SysVar.terstt.bit.pwrstt = 2;
            gs_SysVar.mDGstt |= HLV_STT_LENG;   //(2)

        }else
        {
            
            gs_SysVar.mDGstt &= ~HLV_STT_LENG;  //(2)
            gs_SysVar.terstt.bit.pwrstt = 1;
        }
        if(!(gs_SysVar.mDGstt & HLV_STT_NENG)) //(1)
        {
            gs_SysVar.mDGcnt = 0;          //国网规定掉电后保持60秒
            LOG_INFO("vol lost = %d V\n", vol);
            
                                            //后备电池充足的前提下,在停电后记录停电时间
//	            MoveBuffer(gs_SysTime.YMDhms, gs_FramMisData.tbk, 6);
//	            SaveFramMisData(MIS_TBK);
        }

        if((gs_SysVar.mDGstt & HLV_STT_NENG))
        {
            
            if(gs_SysVar.mDGcnt < 60)
                gs_SysVar.mDGcnt++;
            if(gs_SysVar.mDGcnt == 2)//防抖
            {
            
                ALRT_ERC14(1);                  //系统运行过程中的掉电记录
                gs_SysVar.mLPstt |= HLV_LPTASK_LOSTVOL;
//	                if((gs_SysVar.mLPstt & (HLV_LPTASK_CP | HLV_LPTASK_RP | HLV_LPTASK_TST))== 0)
//	                {
//	                    gs_SysVar.mLPstt |= HLV_LPTASK_CLSCK;
//	                    gs_OS.Message_Send(&gs_TKFarpQueue, &msgidA[MSG_ENTER_LP], 1);
//	                }
                //HAL_BLE_ShutDown();
                   //(2)
                
            } 
            if(gs_SysVar.mDGcnt >= 2)
            {
                gs_SysVar.mLPsysstt |= HLV_STT_NENG;
            }

        }
        gs_SysVar.mDGstt |= HLV_STT_NENG;   //(2)
        
        //g_ucPutcharEn = 0;
    }

//	    if(gs_SysVar.mDGcnt > 0)                //系统状态控制计数器
//	    {
//	        gs_SysVar.mDGcnt--;
//	    }

    EVT_SecProc();                       //遥信的判断

//	    if(gs_SysVar.mDGstt & HLV_STT_NENG)
//	    {
//	        if(gs_SysVar.mDGcnt == 0)
//	        {                                   //自杀操作
//	//	            ES_Message_Send(MSG_HLV_BATCTL, TASK_HLV_TKID);
//	        }
//	    }
//	    else
//	    {
//	        gs_SysVar.mDGcnt = 0;
//	    }

}


/************************************************************************
 * @function: HLV_BatCtl
 * @描述: 关闭电池
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: ZJC (2015/05/15)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void HLV_BatCtl(void)
{
//	    gs_OS.TK_Sleep(200);
//	    gs_GPIO.GPO_Out(GPO_BAT_CTL, false);
}




/************************************************************************
 * @function: UpdateSysTimeStruct
 * @描述: 更新gs_SysTime和gs_SysLastDayTime
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2013/10/30)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void UpdateSysTimeStruct(void)
{
    uint8 ymdhms[6];                    //年月日时分秒(hex)
    const TIME* time = gs_RealTime.GetTime();
                                        //获得YMDHMS
    MoveBuffer((uint8*)&time->sec, ymdhms, 6);
    InvBuffer(ymdhms, 6);
                                        //更新系统时间结构体
    CPSR_ALLOC();
    RHINO_CPU_INTRPT_DISABLE();
    BuildTimeLabel(TTYPE_YMDHMS, ymdhms, &gs_SysTime);
    StepDays(ymdhms, -1);
    BuildTimeLabel(TTYPE_YMDHMS, ymdhms, &gs_SysLastDayTime);
    RHINO_CPU_INTRPT_ENABLE();
    
}





/************************************************************************
 * @function: HLV_MinProc
 * @描述: 分时间
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2013/10/31)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void HLV_MinProc(void)
{
    const TIME* time = GetTime();

    UpdateSysTimeStruct();
    
//	    _JudgeTaskFlag();    
                                        //在有电的前提下,每分钟更新记录意外停电时间
    if(!(gs_SysVar.mDGstt & HLV_STT_NENG))
    {
        //MoveBuffer(gs_SysTime.YMDhms, gs_FramMisData.tbk, 6);
        //SaveFramMisData(MIS_TBK);
    }
                                        //统计分处理
    //STAT_MinProc();


    //-------------------------------------------------------------

    

    //-------------------------------------------------------------
//	    uint8 flag = 0;                         //启动抄表标志
//	    uint8 iscbqj;                           //是否在抄表区间标志
//	    uint32 dmins;                           //当天分钟数
//	    uint32 nxm;                             //下个抄表分钟_next mins
//	
//	                                            //按三个抄表端口依次处理
//	    for(uint8 uc_i = 0; uc_i < MCB_PORT_NUM; uc_i++)
//	    {
//	                                           //广播校时
//	        if(gs_FramMisData.ckten == 0xAA)   //总开关
//	        {
//	            if((gs_InpExtracb[uc_i].ctdhm[0] == GetTime()->dmon) || (gs_InpExtracb[uc_i].ctdhm[0] == 0))
//	            {
//	                if((gs_InpExtracb[uc_i].ctdhm[1] == GetTime()->hour) && 
//	                   (gs_InpExtracb[uc_i].ctdhm[2] == GetTime()->min))
//	                {                               //该端口需要校对标记
//	                    if(gs_InpExtracb[uc_i].cten == 1)//分开关
//	                    {
//	                        gs_InpExtracb[uc_i].gbbreak = 1;
//	                        gs_InpExtracb[uc_i].gbbvsec = 60;
//	                        //gs_OS.Message_Send(MSG_NETP_CKTIME, TASK_NETP_TKID + uc_i);
//	                    }
//	                }
//	            }
//	        }
//	                                            //抄表时段的判断
//	        flag = 0;
//	        iscbqj = 0;                         //判断是否处于抄表区间
//	        dmins = gul_NetpReadMins[uc_i] % 1440;
//	        if((gs_InpExtracb[uc_i].cbqjn == 0) || (gs_InpExtracb[uc_i].cbqjn > 24))
//	        {                                   //非法参数时始终可以抄表
//	            iscbqj = 1;
//	        }
//	        else
//	        {
//	            iscbqj  = 0;
//	            for(uint8 k = 0; k < gs_InpExtracb[uc_i].cbqjn; k++)
//	            {
//	                if((dmins >= gs_InpExtracb[uc_i].cbqjv[k][0]) && (dmins <= gs_InpExtracb[uc_i].cbqjv[k][1]))
//	                {
//	                    iscbqj  = 1;
//	                    break;
//	                }
//	            }
//	        }
//	
//	        if(iscbqj)
//	        {                                   //过日/抄表间隔抄表(抄表间隔从开始抄时算)
//	            nxm = gul_NetpReadMins[uc_i] - (dmins % gs_InpExtracb[uc_i].netitv) + gs_InpExtracb[uc_i].netitv;
//	            if((gs_SysTime.mins >= nxm) || (gs_SysTime.mins < gul_NetpReadMins[uc_i]))
//	            {
//	                flag = 1;
//	            }
//	                                            //抄表日时刻抄表
//	            if((gs_InpExtracb[uc_i].cbday & Bit_Map32[time->dmon - 1]) != 0)
//	            {
//	                if((gs_InpExtracb[uc_i].cbhour == time->hour) && (gs_InpExtracb[uc_i].cbmin == time->min))
//	                {
//	                    guc_RecSaveFlag &= ~Bit_Map8[uc_i];
//	                    flag = 1;
//	                }
//	            }
//	        }
//	
//	        if(gs_SysTime.mins % 15 == 0)     //15min抄表一次
//	        {
//	            flag = 1;
//	        }
//	        if(flag)                            //需要抄表
//	        {
//	            gul_NetpReadMins[uc_i] = gs_SysTime.mins;
//	            //gs_OS.Message_Send(MSG_NETP_IDATA, TASK_NETP_TKID + uc_i);
//	        }
//	    }


}


/************************************************************************
 * @function: HLV_HourProc
 * @描述: 小时消息
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2013/10/30)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void HLV_HourProc(void)
{
    if(gul_SysRunSecs > SYS_SAFE_SECS)
    {
        guc_HourSaveFlag = 0;
        guc_QuarSaveFlag = 0;
    }
//	    gs_SysVar.mLPstt &= ~HLV_LPTASK_TST;
}


/************************************************************************
 * @function: HLV_DayProc
 * @描述: 日消息
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2013/10/30)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void HLV_DayProc(void)
{
    if(gul_SysRunSecs > SYS_SAFE_SECS)
    {
        guc_DaySaveFlag = 0;
        guc_MonSaveFlag = 0;
    }

}






/***********************************************************
 * @function: HLV_LDProc
 * @描述: 首次上电时对停上电处理
 * @参数: 
 * @返回: 
 * @作者: yzy (2010-3-27)
 *---------------------------------------------------------
 * @修改人: 
 **********************************************************/
void HLV_LDProc(void)
{
    uint32 tnow;
    uint32 tpass;
    uint8 tn[6];                            //ssmmhhDDMMYY
    uint8 tp[6];                            //ssmmhhDDMMYY
                                            //记录上次停电时间
//	    MoveBuffer(gs_FramMisData.tbk, gs_FramMisData.tdt, 6);
//	    MoveBuffer(gs_FramMisData.tdt, tp, 6);
//	    InvBuffer(tp, 6);
                                            //获取当前时间
    MoveBuffer((uint8*)&GetTime()->sec, tn, 6);
                                            //计算当日停电累计时间
    tnow = CalcMins(tn + 1, false);
    tpass = CalcMins(tp + 1, false);
//	    if(tnow > tpass)
//	    {
//	        if(CmpBuffer(tn + 3, tp + 3, 3) == 0)
//	        {                                   //停电时间在当天
//	            gs_TmStatData.tdn += (tnow - tpass);
//	        }
//	        else
//	        {                                   //停电时间非当天
//	            gs_TmStatData.tdn = tn[2] * 60 + tn[1];
//	        }
//	    }
//	    gs_TmStatData.rstn += 1;                //日复位次数
//	    STAT_TMSave();                          //保存统计
}









//------------------------------------------------------------------------














/*********************************************************************************************************
** 函数名称: void HLV_LGPIFallProc(void* pdata)
** 功能描述: 慢速端口下降沿事件
** 输　入:
            pdata 没有意义

** 输　出: 无
**
** 全局变量: 无
** 调用模块: 无
**
** 作　者: yzy
** 日　期: 2007年9月4日
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void HLV_LGPIFallProc(void* pdata)
{
    uint32 event = gs_GPIO.GPI_Event(GPI_TYPE_LIFEVT);
    if(event & 0x01)                        //编程键按下
    {
        //gs_OS.Message_Send(MSG_SLV_PGKEY, TASK_SLV_TKID);
    }
}



/*********************************************************************************************************
** 函数名称: void HLV_LGPIUpProc(void* pdata)
** 功能描述: 慢速端口上升沿事件
** 输　入:
            pdata 没有意义

** 输　出: 无
**
** 全局变量: 无
** 调用模块: 无
**
** 作　者: yzy
** 日　期: 2007年9月4日
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void HLV_LGPIUpProc(void* pdata)
{
    uint32 event = gs_GPIO.GPI_Event(GPI_TYPE_LIREVT);
    if(event & 0x01)                        //编程键抬起
    {
        //gs_OS.Message_Send(MSG_SLV_PGKUP, TASK_SLV_TKID);
    }
}


/*********************************************************************************************************
** 函数名称: void HLV_LGPILastProc(void* pdata)
** 功能描述: 慢速端口持续事件
** 输　入:
            pdata 没有意义

** 输　出: 无
**
** 全局变量: 无
** 调用模块: 无
**
** 作　者: yzy
** 日　期: 2007年9月4日
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void HLV_LGPILastProc(void* pdata)
{
    uint32 event = gs_GPIO.GPI_Event(GPI_TYPE_LILEVT);
    if(event & 0x01)
    {
        //gs_OS.Message_Send(MSG_SLV_LPKEY, TASK_SLV_TKID);
    }
}





void HLV_UpdateVar(void)
{
    LoadSystemParam(PARA_TYPE_INP);
}

/*********************************************************************************************************
** 函数名称: void HLV_Init(void* pdata)
** 功能描述: 高级扫描进程的初始化函数，并调用具体应用初始化钩子函数
** 输　入:
            pdata 没有意义

** 输　出: 无
**
** 全局变量: 无
** 调用模块: 无
**
** 作　者: yzy
** 日　期: 2007年9月4日
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void HLV_Init(void)
{
    //上电立刻启动首轮抄表
    //ES_Message_Send(MSG_NETP_DISCOSTART, TASK_NETP_TKID); //搜表
    //gs_OS.Message_Send(MSG_NETP_IDATA, TASK_NETP_TKID);
}

/************************************************************************
 * @function: HLV_PreInit
 * @描述: 系统启动前的初始化操作
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/11/12)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void HLV_PreInit(void)
{
#if (SA_MNT_EN > 0)
    SA_MNT_OC(false);
#endif
                                            //清零
    ClearBuffer((uint8*)gss_CDATATst, sizeof(gss_CDATATst));
    ClearBuffer((uint8*)&gs_InpVarData, sizeof(gs_InpVarData));
    ClearBuffer((uint8*)gs_InpExtracb, sizeof(gs_InpExtracb));
    ClearBuffer((uint8*)gss_TstRealData, sizeof(gss_TstRealData));
    
    UpdateSysTimeStruct();                  //更新应用层系统时间结构体
    gul_NetpReadMins[0] = gs_SysTime.mins;
    gul_NetpReadMins[1] = gs_SysTime.mins;
    gul_NetpReadMins[2] = gs_SysTime.mins;
    
    guc_HourSaveFlag = 0xFF;
    guc_DaySaveFlag = 0xFF;
    guc_RecSaveFlag = 0xFF;
    guc_MonSaveFlag = 0xFF;
    guc_QuarSaveFlag = Bit_Map8[GetTime()->min / 15];
    
    
    //初始化系统参数
    gs_SysVar.mDGstt = HLV_STT_NENG;        //当前为停电状态
    gs_SysVar.mLPsysstt = 0;        //当前为停电状态
    gs_SysVar.mDGcnt = 0;                   //当前计数器为5
    gs_SysVar.terstt.lword = 0;
    gs_SysVar.batVal = 160;//16v
    gs_SysVar.AI0 = 0;
    gs_SysVar.AI1 = 0;
    gs_SysVar.mLPstt = 0;
                                            //清零
    ClearBuffer((uint8*)&gs_MKStruct, sizeof(gs_MKStruct));
    //载入参数
    LoadSystemParam(PARA_TYPE_INP);
    LoadSystemParam(PARA_TYPE_TASK);
//	    LoadFramMisData();  
    int evtNum = 0;
//    CBB_FILE_Read_Event_Num(DB_FLASH, &evtNum, &gs_FramMisData.e1cnt);
    gs_FramMisData.e1pt = gs_FramMisData.e1cnt;
    //其他初始化
    HLV_LDProc();                           //启动时对停上电的处理
    
    
}


/************************************************************************
**函数名称: void EVT_SecProc(void)
**描 述: 将终端事件更新到测量点0(每秒钟)
**
**输 入: 
**输 出: 
**
**备 注: 
**--------------------------------------------------------
**作 者: yzy
**日 期: 
*************************************************************************/
void EVT_SecProc(void)
{
    uint8 EVT_JudgeMK(void);
    EVT_JudgeMK();
}





//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>




/************************************************************************
 * @function: EVT_JudgeMK
 * @描述: 门开的瞬时状态
 * @参数: 
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2014/2/9)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 EVT_JudgeMK(void)
{
    uint32 iost;
    uint32 iowd;
    MKStruct* es = &gs_MKStruct;
    uint8 change;

    iowd = gs_GPIO.GPI_GetStt(0);       //获取当前的状态
    iost = iowd;                        //获取端口状态
    iost >>= YX_IOBIT;

    for(uint8 uc_i = 0; uc_i < YX_NUM; uc_i++)
    {
        if(!(es->mGateIn & (1 << uc_i)))//如果是常开点
        {
            iost ^= (1 << uc_i);
        }
    }
                                        //返回当前的状态
    iost &= Bit_Mask8[YX_NUM];

    if(es->mValid == 0)                 //首次执行
    {
        es->mValid = 1;
        es->mState = iost;
        SYS_OK();
    }
                                        //判断变位
    if(iost != es->mState)              //如果瞬时状态和当前状态不一致
    {
                                        //变化的内容
        change = iost ^ es->mState;

        for(uint32 m = 0; m < YX_NUM; m++)
        {
            if(change & (1 << m))
            {                           //计数器清零然后状态更改
                es->mState ^= (1 << m);
                                        //判断产生上升事件还是下降事件
                if(es->mState & (1 << m))
                {
                    es->mRaise |= (1 << m);
                }
                else
                {
                    es->mFall |= (1 << m);
                }
            }
        }

        gs_MKStruct.mb |= change;    //未上报标志
    }
    SYS_OK();
}











extern kbuf_queue_t gs_TKFarpQueue;
extern kbuf_queue_t gs_MainQueue;
/************************************************************************
 * @function: SYS_HLV_Task
 * @描述: HLV进程处理函数
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2013/10/31)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void SYS_HLV_Task(void)
{
    //数字消息
    krhino_buf_queue_create(&gs_TKHlvQueue, "hlv_queue",
                         gc_TKHlvQbuf, MSG_BUFF_LEN, BUFQUEUE_MSG_MAX);
    
    HLV_Init();

    SYS_MSG_Apply(TASK_HLV_TKID, MSG_CLS_TM);

    for(;;)
    {
        krhino_buf_queue_recv(&gs_TKHlvQueue, RHINO_WAIT_FOREVER, g_TKHlvQ_buf_recv,
                                  &g_TKHlvQ_recv_size);

        switch(g_TKHlvQ_buf_recv[0])                        //根据消息分别处理
        {
            case MSG_SEC:                   //应用主进程秒消息处理
                gs_SysVar.mLPstt |= HLV_LPTASK_SMSG_HLV;
                HLV_SecProc();
            
                gs_SysVar.mLPstt &= ~HLV_LPTASK_SMSG_HLV;
                break;

            case MSG_MIN:                   //分消息处理
                HLV_MinProc();
                break;

            case MSG_HOUR:
                HLV_HourProc();
                break;

            case MSG_DAY:
                HLV_DayProc();
                break;

            case MSG_LIVE:
                HB_RetLive(TASK_HLV_TKID);
                break;
                
            case MSG_HLV_BATCTL:        //延时关闭电池
                HLV_BatCtl();
                break;
            case MSG_HLV_INP:
                LOG_DEBUG("******MSG_HLV_INP******** !!!!\n");
                HLV_UpdateVar();
                break;

                
            case MSG_HLV_FARP:
//	                gs_OS.Message_Send(MSG_FARP_VAR, TASK_FARP_TKID);
                gs_OS.Message_Send(&gs_TKFarpQueue, &msgidA[MSG_FARP_VAR], 1);
                break;
                
            case MSG_HLV_PST:
//	                gs_OS.Message_Send(MSG_PST_VAR, TASK_MAIN_TKID);
                gs_OS.Message_Send(&gs_MainQueue, &msgidA[MSG_PST_VAR], 1);
                
                break;
                
            case MSG_HLV_TASK:               //更新任务参数
//	                Task_SetTimerCfg(false);
                break;            
            case MSG_ALRT_ERC1:             //参数及数据初始化
                ALRT_ERC1(MSG_ALRT_ERC1);
                break;
            case MSG_ALRT_CLR:              //清除所有事件
                ALRT_CLR();
                LOG_DEBUG("******MSG_ALRT_CLR******** !!!!\n");
                
                break;
            case MSG_HLV_CLR:               //总清
                LOG_DEBUG("******MSG_HLV_CLR******** !!!!\n");
                Inp_DataClear();
                break;

        }

    }
}

/******************************************************************************
**应用程序主进程块常量部分定义
******************************************************************************/
//	const TESTcbConst gs_TkHLV = 
//	{
//	    ES_HLV_Task,
//	    TASK_HLV_TKID >> 3,                     //应用管理进程
//	    TASK_HLV_TKID,                          //进程编号
//	    TASK_HLV_TKID & 7,                      //优先级内进程编号
//	    true,                                   //消息循环机制进程
//	};
const KTaskConst gs_TkHLV = 
{
    &gs_TKHlvHandle,
    SYS_HLV_Task,                           //应用管理进程
    gs_TKHlvStack,
    TASK_HLV_TKID,                           //进程编号
    true,                                    //消息循环机制进程
    TASK_HLV_STKL,
    "Hlv",
    &gs_TKHlvQueue,
};



