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


uint16 gui_FarpWmSafeCnt;                   //����ͨѶ״̬����������ȫʱ�䵹��ʱ(��)
uint16 gui_FarpSerSafeCnt;                   //����ͨѶ״̬����������ȫʱ�䵹��ʱ(��)



/******************************************************************************
**�����������ʱ
******************************************************************************/
uint16 gui_FarpWmRcSec;                     //GPRSͨ·�������������(�뵹��ʱ)
uint16 gui_FarpSerRcSec;                     //GPRS Tcp Serverͨ·�������������(�뵹��ʱ)


/******************************************************************************
**��������ʧ�ܴ���
******************************************************************************/
uint8 guc_FarpWmRcFailCnt;                  //GPRS��������ʧ�ܴ���
uint8 guc_FarpSerRcFailCnt;                 //Tcp Server��������ʧ�ܴ���
uint16 guc_FarpWmRcFailNorFtpCnt;                  //GPRS������������FTP����


/******************************************************************************
**��������
******************************************************************************/
uint8 guc_FarpWmRcCnt;                      //����ʧ�ܺ����������(yzy:���ڷ�������ǰ��ֵ)
uint8 guc_FarpSerRcCnt;                     //����ʧ�ܺ����������(yzy:���ڷ�������ǰ��ֵ)



/******************************************************************************
**�������ڼ�����
******************************************************************************/
uint16 gui_FarpWmHBCYCnt;                   //�������ڼ�����


uint8 guc_FarpWmHKUnRpyCnt;                 //�����޻ظ�����������




/******************************************************************************
**����������
******************************************************************************/
uint16 gui_FarpHdRstCnt;                    //����è��ʼ������ʱ�������(��δ�忨ʱ����ֳ�ʼ��ʧ��)
uint8 guc_FarpPstTryCnt;                    //GPRSͨ·���������ز�����������
uint8 guc_FarpNVHours;                      //������ͨѶСʱ��
uint8 guc_FastWmSttReqCnt;                  //MD ����������ʱ

uint16 gui_FarpServerNoDataCnt;             //TCP Server  ������������£������ݼ���
uint16 gui_FarpFromClnNoDataCnt;             //TCP Server  ��ͻ���1���ӵ�����£������ݼ���
uint8  guc_FarpFromClnCloseCnt;             //TCP Server  ��ͻ���1���ӵ������, �����رտͻ��˴���

uint8  guc_FarpReActiveFlag;                //��������ģʽ���ɼ�����������Ҫ������վ��������



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
 * @����: Զ�̲�����ʼ��
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void Farp_PreInit(void)
{                                           //��������
    memset((uint8*)&gs_FarpVar, 0, sizeof(FarpVar));
    memset((uint8_t *)&gs_FtpPara,0,sizeof(S_FTPPARA));
    
//    LoadSystemParam(PARA_TYPE_FARP);

//    gui_FarpWmRcSec = 10;                   //��ʼ�����10s�����Զ������
//    gui_FarpSerRcSec = 10;
//    //gui_FarpEthRcSec = 10;
//    
//    guc_FarpWmHKUnRpyCnt = 0;               //�����޻ظ���������������
//    //guc_FarpEthHKUnRpyCnt = 0;
//    
//    gui_FarpWmSafeCnt = 0;
//    gui_FarpSerSafeCnt = 0;
//    //gui_FarpEthSafeCnt = 0;
//    
//    guc_FarpWmRcCnt = 0;                   //����ʧ�ܺ�������������
//    guc_FarpSerRcCnt = 0;
//    //guc_FarpEthRcCnt = 0;
//    
//    gui_FarpHdRstCnt = 0;                   //ģ�鸴λ����������
//    guc_FarpNVHours = 0;                    //��IP����ʱ������
//    guc_FarpPstTryCnt = 0;                   //��������ģʽ�ز���������������
//    
//    gui_FarpBkTmCnt = 0;                    //�����������������߼���������
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
// * @����: ����è���봦��
// * @����: 
// * @����: 
// * @˵��: 
// * @����: yzy (2014/1/14)
// *-----------------------------------------------------------------------
// * @�޸���: 
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
                                            //yzy 2014-04-09,�������Ҫ����̫������,ֻҪ��⵽���߼�����GPRS.
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
                                            //��ȡģ��״̬
    TRFModemState modemstt = SYS_RF_Status();//SYS_MODM_Status();
	
    if(gui_FarpWmSafeCnt > 0)               //����ͨѶ״̬����������ȫʱ�䵹��ʱ
    {
        gui_FarpWmSafeCnt--;
    }
    
    if(gui_FarpSerSafeCnt > 0)              //TCP Server ������ͨѶ״̬����������ȫʱ�䵹��ʱ
    {
        gui_FarpSerSafeCnt--;
    }
    
//    if(!(modemstt.bit.typeChecked))         //ģ�黹δʶ��
//    {
//        return;
//    }
    
    if(!(modemstt.bit.chnrdy))              //�Ƿ����AT����
    {
        if(++guc_FastWmSttReqCnt >= 5)      //ÿ5����
        {
            guc_FastWmSttReqCnt = 0;
            //SYS_MODM_Step(Modem_Init);
//            SYS_MODM_Reinit();
//	            HAL_WIFI_Init(TASK_FARP_TKID,"tw14",gs_FarpVar.ap , &g_ulTcp_fd, 4);
//            gs_GPIO.LED_BlinkSet(GPIO_LED_GPRS, 0xFF, 0, 0);
        }
        return;
    }
	
    if(!(modemstt.bit.regtt))               //�Ƿ��������ע��
    {
        gs_FarpVar.wmst &= ~WMS_CHALLSTT;
        gs_FarpVar.mcsq = 0;
                                            //��ʱ����ģ��״̬
        if(++guc_FastWmSttReqCnt >= 15)     //ÿ15����
        {
            guc_FastWmSttReqCnt = 0;
//	            HAL_WIFI_BeginCheck();
        }
        return;
    }
	
    gs_FarpVar.wmst |= WMS_STT_GSM;
    gs_FarpVar.mcsq = modemstt.bit.signl;
//    SYS_LED_BlinkSet1(GPIO_LED_GPRS, 1, 10, 10, 0);  //ע��GSM����ɹ���1HZ��˸
    
    if(!(modemstt.bit.pppoe))       //�Ƿ����PPP����
    {
        gs_FarpVar.wmst &= ~WMS_STT_PPP;    //����ͨ��������
        if(++guc_FastWmSttReqCnt >= 30)     //ÿ10����
        {
	            SYS_Message_Send(MSG_FARP_CNTPPP, TASK_FARP_TKID);
//            uint8 msg = MSG_FARP_CNTPPP;
//            krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
            
            guc_FastWmSttReqCnt = 0;
        }
//	        
//        if(guc_FastWmSttReqCnt % 5 == 0)     //ÿ3����
//        {
//            HAL_WIFI_BeginCheck();
//        }
        return;
    }
    gs_FarpVar.wmst |= WMS_STT_PPP;
    
    
    


//	    if(gs_FarpVar.clnen == 1)           //�ͻ���ģʽ
    {
        if(gs_FarpVar.login == 0)    //TCP ������
        {
            gs_FarpVar.wmst &= ~WMS_STT_CLN;    //����ͨ��������
            if(gs_FarpVar.wkmd == 0x00)         //ʵʱ����ģʽ
            {
                if(gui_FarpWmRcSec == 0)        //��������ģʽ���ߺ󼴿�ʼ���Ե���ʱ
                {
                    gui_FarpWmRcSec = gs_FarpVar.rttm;
                }
                                                //����ͨѶ״̬�µ�����������
                if(gui_FarpWmSafeCnt > 0)
                {
                    gui_FarpWmSafeCnt = 0;
                    
                    gui_FarpWmRcSec = 1;
                    guc_FarpWmRcCnt = 3;       //ͨѶ״̬�¶�������3��
                }
                if(gui_FarpWmRcSec > 0)
                {
                    gui_FarpWmRcSec--;         //�������ݼ�
                    if(gui_FarpWmRcSec == 0)   //���Լ���ѵ�,��������
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
//	                                guc_FarpPstTryCnt = 0;          //����ģʽ������������
//	                                Farp_SendLoginData(0);          //���͵�¼֡
//	                                //Farp_SendHKData(0);             //��������֡
//	                                                                //��ʼ����������
//	                                gui_FarpWmHBCYCnt = 5;//gs_FarpVar.hbcy;
//	                                                                //ʵʱ����ģʽ����Ч���ݼ�������ʼ��
//	                                gui_FarpWmSafeCnt = gs_FarpVar.hbcy;
//	                                guc_FarpWmRcFailCnt = 0;        //����ʧ�ܴ�������
//	                            }
//	                        }
//	                    }
                }
            }
            else if(gs_FarpVar.wkmd == 0x01)    //��������ģʽ
            {
                //
            }
        }
        else
        {
            gs_FarpVar.wmst |= WMS_STT_CLN;     //����ͨ������
            
            if(guc_FarpLoginConfirmErr)           //��¼��n����δ�յ�ȷ��֡,ִ����������
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
                if(gui_FarpWmHBCYCnt == 0)      //�������ڵ�������֡
                {
                    if(guc_FarpWmHKUnRpyCnt >= 3) //�����޻ظ���������3��
                    {
//	                        SYS_Message_Send(MSG_FARP_DISGPRS, TASK_FARP_TKID);
                        uint8 msg = MSG_FARP_DISGPRS;
                        krhino_buf_queue_send(&gs_RFMngQueue, &msg, 1);
                        

                        uc_closeclnflag = 1;
                        guc_FarpWmHKUnRpyCnt = 0; //�����޻ظ���������
                    }
                    else
                    {
                        //Farp_SendHKData(0);      //������������
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
//	                                guc_FarpPstTryCnt = 0;          //����ģʽ������������
//	//                                Farp_SendLoginData(0);          //���͵�¼֡
//	                                //Farp_SendHKData(0);             //��������֡
//	                                                                //��ʼ����������
//	                                gui_FarpWmHBCYCnt = 5;//gs_FarpVar.hbcy;
//	                                                                //ʵʱ����ģʽ����Ч���ݼ�������ʼ��
//	                                gui_FarpWmSafeCnt = gs_FarpVar.hbcy;
//	                                guc_FarpWmRcFailCnt = 0;        //����ʧ�ܴ�������
//	                            }
//	                        }                    
                    }
                }
            }
            else
            {
                gui_FarpWmHBCYCnt = gs_FarpVar.hbcy;
            }
            gui_FarpWmRcSec = 0;                //���Դ�������
                                                // 
//	            if(gs_FarpVar.wkmd == 0)            //ʵʱ����ģʽ
//	            {
//	                //....
//	            }
//	            else if(gs_FarpVar.wkmd == 1)       //��������ģʽ
//	            {
//	                gui_FarpBkTmCnt++;              //���������߼������ۼ�
//	                                                //������ͨѶʱ�䳬����ֵ����Ҫ�Ͽ�����
//	                if(gui_FarpBkTmCnt >= gs_FarpVar.slep)
//	                {                               //�öϿ�������Ϣ
//	                    SYS_Message_Send(MSG_FARP_DISGPRS, TASK_FARP_TKID);
//	//	                    uint8 msg = MSG_FARP_DISGPRS;
//	//	                    krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
//	                    
//	                    uc_closeclnflag = 1;
//	                }
//	            }
//	            else                                //����ģʽֱ�ӶϿ�����
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
                                            //������Ϣ
    //uint8 nmsg;                             //������Ϣ
    
    krhino_buf_queue_create(&gs_RFMngQueue, "rf_queue",
                         gc_RFMngbuf, MSG_BUFF_LEN, BUFQUEUE_MSG_MAX);

    Farp_PreInit();
    
    SYS_RF_Init();
//    static uint8_t ble_name[14] = {'V','S'};
//    ByteArrayBcdToHexString(gs_PstPara.Addr, ble_name+2, 6, 0);
    
//    g_ucPutcharEn = 0;
//
//    SYS_MAIN_Init();
                                            //�����ȡʱ����Ϣ
    SYS_MSG_Apply(TASK_RFMNG_TKID, MSG_CLS_TM);//*��ʼ����ػ���
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
                case MSG_LIVE:                  //�ظ�������Ϣ
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
**RF���̿鳣�����ֶ���
******************************************************************************/
const KTaskConst gs_TkLP2PCmm = 
{
    &gs_RFMngHandle,
    SYS_RFMng_Task,
    gs_RFMngStack,
    TASK_RFMNG_TKID,                           //���̱��
    true,                                    //��Ϣѭ�����ƽ���
    TASK_RFMNG_STKL,
    "rfmng",
    &gs_RFMngQueue,
};