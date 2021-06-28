#define EXT_FARP
#include <k_api.h>
#include <stdio.h>
#include <stdlib.h>
#include "public.h"
#include "app.h"
#include "pst.h"
//#include "hlv.h"
//#include "farp.h"
//#include "netp.h"
#include "task.h"
#include "farp.h"


uint16 gui_FarpWmSafeCnt;                   //数据通讯状态断线重连安全时间倒计时(秒)
uint16 gui_FarpSerSafeCnt;                   //数据通讯状态断线重连安全时间倒计时(秒)



/******************************************************************************
**重连间隔倒计时
******************************************************************************/
uint16 gui_FarpWmRcSec;                     //GPRS通路重连间隔计数器(秒倒计时)
uint16 gui_FarpSerRcSec;                     //GPRS Tcp Server通路重连间隔计数器(秒倒计时)


/******************************************************************************
**连续重连失败次数
******************************************************************************/
uint8 guc_FarpWmRcFailCnt;                  //GPRS连续重连失败次数
uint8 guc_FarpSerRcFailCnt;                 //Tcp Server连续重连失败次数
uint16 guc_FarpWmRcFailNorFtpCnt;                  //GPRS连续重连但是FTP在线


/******************************************************************************
**重连次数
******************************************************************************/
uint8 guc_FarpWmRcCnt;                      //联网失败后的重连次数(yzy:可在发起联网前赋值)
uint8 guc_FarpSerRcCnt;                     //联网失败后的重连次数(yzy:可在发起联网前赋值)



/******************************************************************************
**心跳周期计数器
******************************************************************************/
uint16 gui_FarpWmHBCYCnt;                   //心跳周期计数器


uint8 guc_FarpWmHKUnRpyCnt;                 //心跳无回复次数计数器




/******************************************************************************
**其它计数器
******************************************************************************/
uint16 gui_FarpHdRstCnt;                    //无线猫初始化花费时间计数器(如未插卡时会出现初始化失败)
uint8 guc_FarpPstTryCnt;                    //GPRS通路被动激活重拨次数计数器
uint8 guc_FarpNVHours;                      //无数据通讯小时数
uint8 guc_FastWmSttReqCnt;                  //MD 操作步骤延时

uint16 gui_FarpServerNoDataCnt;             //TCP Server  在侦听的情况下，无数据计数
uint16 gui_FarpFromClnNoDataCnt;             //TCP Server  与客户端1链接的情况下，无数据计数
uint8  guc_FarpFromClnCloseCnt;             //TCP Server  与客户端1链接的情况下, 主动关闭客户端次数

uint8  guc_FarpReActiveFlag;                //被动激活模式，采集器重启后，需要先与主站建立链接



uint16 gui_smseq;
static char         g_RFMng_buf_recv[BUFQUEUE_MSG_MAX+4];

size_t       g_RFMng_recv_size;
ktimer_t     g_RFMngTimer;

cpu_stack_t  gs_RFMngStack[TASK_RFMNG_STKL];
ktask_t      gs_RFMngHandle;
kbuf_queue_t gs_RFMngQueue;
char         gc_RFMngbuf[MSG_BUFF_LEN];
/************************************************************************
 * @function: Farp_PreInit
 * @描述: 远程参数初始化
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void Farp_PreInit(void)
{                                           //参数载入
    memset((uint8*)&gs_FarpVar, 0, sizeof(FarpVar));
    memset((uint8_t *)&gs_FtpPara,0,sizeof(S_FTPPARA));
    
//    LoadSystemParam(PARA_TYPE_FARP);

//    gui_FarpWmRcSec = 10;                   //初始化完成10s后进行远程连接
//    gui_FarpSerRcSec = 10;
//    //gui_FarpEthRcSec = 10;
//    
//    guc_FarpWmHKUnRpyCnt = 0;               //心跳无回复次数计数器清零
//    //guc_FarpEthHKUnRpyCnt = 0;
//    
//    gui_FarpWmSafeCnt = 0;
//    gui_FarpSerSafeCnt = 0;
//    //gui_FarpEthSafeCnt = 0;
//    
//    guc_FarpWmRcCnt = 0;                   //联网失败后重连次数清零
//    guc_FarpSerRcCnt = 0;
//    //guc_FarpEthRcCnt = 0;
//    
//    gui_FarpHdRstCnt = 0;                   //模块复位计数器清零
//    guc_FarpNVHours = 0;                    //无IP数据时间清零
//    guc_FarpPstTryCnt = 0;                   //被动激活模式重拨次数计数器清零
//    
//    gui_FarpBkTmCnt = 0;                    //被动激活无数据休眠计数器清零
//    guc_FarpWmRcFailCnt = 0;
//    guc_FarpWmRcFailNorFtpCnt = 0;
//    //guc_FarpEthRcFailCnt = 0;
//    
//    gui_FarpServerNoDataCnt = 0;
//    guc_FarpFromClnCloseCnt = 0;
//    
//    guc_FarpLoginConfirmErr = 0;
//    gul_FarpLoginDeadTime = 0xFFFFFFFF;
//    
//    gs_FarpVar.wmsv = 0x88000000;
    //gs_SysVar.mLPstt |= HLV_LPTASK_MDCK;

}
///************************************************************************
// * @function: Farp_WMSecondProc
// * @描述: 无线猫的秒处理
// * @参数: 
// * @返回: 
// * @说明: 
// * @作者: yzy (2014/1/14)
// *-----------------------------------------------------------------------
// * @修改人: 
// ************************************************************************/
void Farp_WMSecondProc(void)
{
    uint8 uc_closeclnflag = 0;
//    uint8 uc_closeserflag = 0;
    /*
    if(gs_FarpVar.wmst & WMS_ETH0STT)
    {
        return;
    }
    */
                                            //yzy 2014-04-09,国网检测要求以太网优先,只要检测到网线即禁用GPRS.
//	    if(gs_FarpVar.wmst & WMS_EHSTT)
//	    {
//	        if((gs_FarpVar.wmst & WMS_STT_CLN) || (gs_FarpVar.wmst & WMS_STT_SER))
//	        {
//	//	            SYS_Message_Send(MSG_FARP_DISPPP, TASK_FARP_TKID);
//	            uint8 msg = MSG_FARP_DISPPP;
//	            krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
//	            
//	        }
//	        gs_FarpVar.wmst &= ~WMS_CHALLSTT;
//	        return;
//	    }                                       //yzy 2014-04-09
                                            //获取模块状态
    TRFModemState modemstt = SYS_RF_Status();//SYS_MODM_Status();
	
    if(gui_FarpWmSafeCnt > 0)               //数据通讯状态断线重连安全时间倒计时
    {
        gui_FarpWmSafeCnt--;
    }
    
    if(gui_FarpSerSafeCnt > 0)              //TCP Server 无数据通讯状态断线重连安全时间倒计时
    {
        gui_FarpSerSafeCnt--;
    }
    
//    if(!(modemstt.bit.typeChecked))         //模块还未识别
//    {
//        return;
//    }
    
    if(!(modemstt.bit.chnrdy))              //是否完成AT配置
    {
        if(++guc_FastWmSttReqCnt >= 5)      //每5秒钟
        {
            guc_FastWmSttReqCnt = 0;
            //SYS_MODM_Step(Modem_Init);
//            SYS_MODM_Reinit();
//	            HAL_WIFI_Init(TASK_FARP_TKID,"tw14",gs_FarpVar.ap , &g_ulTcp_fd, 4);
//            gs_GPIO.LED_BlinkSet(GPIO_LED_GPRS, 0xFF, 0, 0);
        }
        return;
    }
	
    if(!(modemstt.bit.regtt))               //是否完成网络注册
    {
        gs_FarpVar.wmst &= ~WMS_CHALLSTT;
        gs_FarpVar.mcsq = 0;
                                            //及时更新模块状态
        if(++guc_FastWmSttReqCnt >= 15)     //每15秒钟
        {
            guc_FastWmSttReqCnt = 0;
//	            HAL_WIFI_BeginCheck();
        }
        return;
    }
	
    gs_FarpVar.wmst |= WMS_STT_GSM;
    gs_FarpVar.mcsq = modemstt.bit.signl;
//    SYS_LED_BlinkSet1(GPIO_LED_GPRS, 1, 10, 10, 0);  //注册GSM网络成功，1HZ闪烁
    
    if(!(modemstt.bit.pppoe))       //是否完成PPP链接
    {
        gs_FarpVar.wmst &= ~WMS_STT_PPP;    //数据通道不可用
        if(++guc_FastWmSttReqCnt >= 30)     //每10秒钟
        {
	            SYS_Message_Send(MSG_FARP_CNTPPP, TASK_FARP_TKID);
//            uint8 msg = MSG_FARP_CNTPPP;
//            krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
            
            guc_FastWmSttReqCnt = 0;
        }
//	        
//        if(guc_FastWmSttReqCnt % 5 == 0)     //每3秒钟
//        {
//            HAL_WIFI_BeginCheck();
//        }
        return;
    }
    gs_FarpVar.wmst |= WMS_STT_PPP;
    
    
    


//	    if(gs_FarpVar.clnen == 1)           //客户端模式
    {
        if(gs_FarpVar.login == 0)    //TCP 不在线
        {
            gs_FarpVar.wmst &= ~WMS_STT_CLN;    //数据通道不可用
            if(gs_FarpVar.wkmd == 0x00)         //实时在线模式
            {
                if(gui_FarpWmRcSec == 0)        //永久在线模式断线后即开始重试倒计时
                {
                    gui_FarpWmRcSec = gs_FarpVar.rttm;
                }
                                                //数据通讯状态下掉线马上重连
                if(gui_FarpWmSafeCnt > 0)
                {
                    gui_FarpWmSafeCnt = 0;
                    
                    gui_FarpWmRcSec = 1;
                    guc_FarpWmRcCnt = 3;       //通讯状态下断线重连3次
                }
                if(gui_FarpWmRcSec > 0)
                {
                    gui_FarpWmRcSec--;         //计数器递减
                    if(gui_FarpWmRcSec == 0)   //重试间隔已到,进行重连
                    {
//	                        SYS_Message_Send(MSG_FARP_CNTGPRS, TASK_FARP_TKID);
                        uint8 msg = MSG_FARP_CNTGPRS;
                        krhino_buf_queue_send(&gs_RFMngQueue, &msg, 1);
                        
                    }
//	                    if(gui_FarpWmRcSec % 15 == 0)
//	                    {
//	                        int fd = -1;
//	                        fd = HAL_WIFI_TCP_Lnk_Chk();
//	                        if(fd < 0)
//	                        {
//	                            SYS_Message_Send(MSG_FARP_CNTGPRS, TASK_FARP_TKID);
//	                        }
//	                        else
//	                        {
//	                            if(g_ulTcp_fd < 0)
//	                            {
//	                                g_ulTcp_fd = fd;
//	                                guc_FarpLoginConfirmErr = 0;
//	                                gul_FarpLoginDeadTime = gul_SysRunSecs + 10;
//	                                                                //
//	//                                gs_FarpVar.wmst |= WMS_STT_CLN;
//	                                guc_FarpPstTryCnt = 0;          //被动模式重连次数清零
//	                                Farp_SendLoginData(0);          //发送登录帧
//	                                //Farp_SendHKData(0);             //发送心跳帧
//	                                                                //初始化心跳周期
//	                                gui_FarpWmHBCYCnt = 5;//gs_FarpVar.hbcy;
//	                                                                //实时在线模式无有效数据计数器初始化
//	                                gui_FarpWmSafeCnt = gs_FarpVar.hbcy;
//	                                guc_FarpWmRcFailCnt = 0;        //重连失败次数清零
//	                            }
//	                        }
//	                    }
                }
            }
            else if(gs_FarpVar.wkmd == 0x01)    //被动激活模式
            {
                //
            }
        }
        else
        {
            gs_FarpVar.wmst |= WMS_STT_CLN;     //数据通道可用
            
            if(guc_FarpLoginConfirmErr)           //登录后n秒内未收到确认帧,执行重连操作
            {
                if(gul_SysRunSecs > gul_FarpLoginDeadTime)
                {
//	                    SYS_Message_Send(MSG_FARP_DISGPRS, TASK_FARP_TKID);
                    uint8 msg = MSG_FARP_DISGPRS;
                    krhino_buf_queue_send(&gs_RFMngQueue, &msg, 1);
                    
                    uc_closeclnflag = 1;
                    gul_FarpLoginDeadTime = 0xFFFFFFFF;
                }
            }
            
            if(gui_FarpWmHBCYCnt > 0)
            {
                gui_FarpWmHBCYCnt--;
                if(gui_FarpWmHBCYCnt == 0)      //心跳周期到发心跳帧
                {
                    if(guc_FarpWmHKUnRpyCnt >= 3) //心跳无回复次数超过3次
                    {
//	                        SYS_Message_Send(MSG_FARP_DISGPRS, TASK_FARP_TKID);
                        uint8 msg = MSG_FARP_DISGPRS;
                        krhino_buf_queue_send(&gs_RFMngQueue, &msg, 1);
                        

                        uc_closeclnflag = 1;
                        guc_FarpWmHKUnRpyCnt = 0; //心跳无回复次数清零
                    }
                    else
                    {
                        //Farp_SendHKData(0);      //发送心跳数据
                        Farp_SendTestData(0);
                        guc_FarpWmHKUnRpyCnt++;
                        gui_FarpWmHBCYCnt = gs_FarpVar.hbcy;
//                        g_ulTcp_fd = HAL_WIFI_TCP_Lnk_Chk();
//	                        int fd = -1;
//	                        fd = HAL_WIFI_TCP_Lnk_Chk();
//	                        if(fd < 0)
//	                        {
//	                            SYS_Message_Send(MSG_FARP_CNTGPRS, TASK_FARP_TKID);
//	                        }
//	                        else
//	                        {
//	                            if(g_ulTcp_fd < 0)
//	                            {
//	                                g_ulTcp_fd = fd;
//	                                guc_FarpLoginConfirmErr = 0;
//	                                gul_FarpLoginDeadTime = gul_SysRunSecs + 10;
//	                                                                //
//	//                                gs_FarpVar.wmst |= WMS_STT_CLN;
//	                                guc_FarpPstTryCnt = 0;          //被动模式重连次数清零
//	//                                Farp_SendLoginData(0);          //发送登录帧
//	                                //Farp_SendHKData(0);             //发送心跳帧
//	                                                                //初始化心跳周期
//	                                gui_FarpWmHBCYCnt = 5;//gs_FarpVar.hbcy;
//	                                                                //实时在线模式无有效数据计数器初始化
//	                                gui_FarpWmSafeCnt = gs_FarpVar.hbcy;
//	                                guc_FarpWmRcFailCnt = 0;        //重连失败次数清零
//	                            }
//	                        }                    
                    }
                }
            }
            else
            {
                gui_FarpWmHBCYCnt = gs_FarpVar.hbcy;
            }
            gui_FarpWmRcSec = 0;                //重试次数清零
                                                // 
//	            if(gs_FarpVar.wkmd == 0)            //实时在线模式
//	            {
//	                //....
//	            }
//	            else if(gs_FarpVar.wkmd == 1)       //被动激活模式
//	            {
//	                gui_FarpBkTmCnt++;              //无数据休眠计数器累加
//	                                                //无数据通讯时间超过定值或需要断开连接
//	                if(gui_FarpBkTmCnt >= gs_FarpVar.slep)
//	                {                               //置断开连接消息
//	                    SYS_Message_Send(MSG_FARP_DISGPRS, TASK_FARP_TKID);
//	//	                    uint8 msg = MSG_FARP_DISGPRS;
//	//	                    krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
//	                    
//	                    uc_closeclnflag = 1;
//	                }
//	            }
//	            else                                //短信模式直接断开连接
//	            {
//	                SYS_Message_Send(MSG_FARP_DISGPRS, TASK_FARP_TKID);
//	                
//	//	                uint8 msg = MSG_FARP_DISGPRS;
//	//	                krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
//	                uc_closeclnflag = 1;
//	            }
        }
    }
    

    if((uc_closeclnflag == 1) && (gs_FarpVar.wkmd == 0))
    {
//	        SYS_Message_Send(MSG_FARP_CNTGPRS, TASK_FARP_TKID);
        uint8 msg = MSG_FARP_CNTGPRS;
        krhino_buf_queue_send(&gs_RFMngQueue, &msg, 1);
        
    }
}
void SYS_RFMng_Task(void * arg)
{
    TIME time;
    kstat_t ret = 0;
                                            //链表消息
    //uint8 nmsg;                             //数字消息
    
    krhino_buf_queue_create(&gs_RFMngQueue, "rf_queue",
                         gc_RFMngbuf, MSG_BUFF_LEN, BUFQUEUE_MSG_MAX);

    Farp_PreInit();
    
    SYS_RF_Init();
//    static uint8_t ble_name[14] = {'V','S'};
//    ByteArrayBcdToHexString(gs_PstPara.Addr, ble_name+2, 6, 0);
    
//    g_ucPutcharEn = 0;
//
//    SYS_MAIN_Init();
                                            //申请获取时间消息
    SYS_MSG_Apply(TASK_RFMNG_TKID, MSG_CLS_TM);//*初始化监控机制
//    SYS_MSG_Apply(TASK_MAIN_TKID, MSG_CLS_GPIO);
//    SYS_MSG_ApplyExt(TASK_MAIN_TKID, MSG_CLS_UART, UART_CHANNEL_DEBUG);

//    LOG_DEBUG("\nVS Project %s  Softver[%x] Hardver[%x]!!!\n", gucs_PrjCode, gul_UsrFuncVer, gul_UsrHardcVer);
    //Flash_Test();

    for(;;)
    {   
        ret = krhino_buf_queue_recv(&gs_RFMngQueue, 1/*RHINO_WAIT_FOREVER*/, g_RFMng_buf_recv,
                          &g_RFMng_recv_size);
        if(ret == RHINO_SUCCESS)
        {
            switch(g_RFMng_buf_recv[0])
            {
                case MSG_SEC:

                    break;
                case MSG_LIVE:                  //回复保活消息
                    HB_RetLive(TASK_RFMNG_TKID);
                    break; 
                default:
                    
                    break;
            }
        }
        else
        {
#ifdef MASTER_NODE          
            SYS_A7139_Proc(1);
#else
            SYS_A7139_Proc(0);
#endif            
        }
    }
}

/******************************************************************************
**RF进程块常量部分定义
******************************************************************************/
const KTaskConst gs_TkLP2PCmm = 
{
    &gs_RFMngHandle,
    SYS_RFMng_Task,
    gs_RFMngStack,
    TASK_RFMNG_TKID,                           //进程编号
    true,                                    //消息循环机制进程
    TASK_RFMNG_STKL,
    "rfmng",
    &gs_RFMngQueue,
};