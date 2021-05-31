/****************************************Copyright (c)**************************************************
**                                  ____�Ƽ����޹�˾
**                                          ������
**
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: farp.c
**��   ��   ��: yzy
**����޸�����: 2017��9��1��
**��        ��: ��ͨ��������
**ע        ��:
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: yzy
** ��  ��: v1.0
*******************************************************************************/

#define EXT_FARP

#include <k_api.h>
#include <stdio.h>
#include <stdlib.h>
#include "public.h"
#include "app.h"
#include <sal_import.h>
#include "atmodem.h"
#include "farp.h"
#include "hlv.h"
//#include "iot_import.h"

#include <network/network.h>


/******************************************************************************
**����ͨѶ״̬����������ȫʱ������
******************************************************************************/
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

cpu_stack_t  gs_TKFarpStack[TASK_FARP_STKL];
ktask_t      gs_TKFarpHandle;
kbuf_queue_t gs_TKFarpQueue;
char         gc_TKFarpQbuf[MSG_BUFF_LEN];
static char         g_TKFarpQ_buf_recv[BUFQUEUE_MSG_MAX+4];
static size_t       g_TKFarpQ_recv_size;
ksem_t gs_FarpProc;   
intptr_t g_ulTcp_fd = -1;

extern kbuf_queue_t gs_TKSlvQueue;
extern intptr_t HAL_TCP_Establish(const char *host, uint16_t port);
extern int32_t HAL_TCP_Destroy(intptr_t fd);
extern int32_t HAL_TCP_Read(intptr_t fd, char *buf, uint32_t len, uint32_t timeout_ms);
extern int32_t HAL_TCP_Write(intptr_t fd, const char *buf, uint32_t len,uint32_t timeout_ms);
    
/************************************************************************
 * @function: Farp_SendIpData
 * @����: Զ��ͨ������GPRS��IP����
 * 
 * @����: 
 * @param: buffer ���ͻ���
 * @param: len ���͵ĳ���
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 Farp_SendIpData(uint8* buffer, uint16 len)
{
                                            //IP������ͨ�����õ�����·���
    if(g_ulTcp_fd >=0 )
    {                                       //����IP����
//	        if(!SYS_MODM_SendIP(buffer, len))
        LOG_DUMPHEX(LOG_LEVEL_DEBUG, "send:\n", buffer, len);

        if(HAL_TCP_Write(g_ulTcp_fd, buffer, len,3000) >= 0)
        {
//	            gs_GPIO.LED_BlinkSet(LED_FAR_T, 0, 0, 0);
            SYS_OK();                        //���ͳɹ�
        }
        else
        {
            uint8 msg = MSG_FARP_DISGPRS;
            krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
            
            gui_FarpWmRcSec = 10;
        }
    }
    return SYS_ERR_FT;                      //IP���ݷ���ʧ��
}


const uint8_t cmdUpBuf[]=
                   {0x68, 0x33, 0x19, 0x27, 0x03, 0x18, 0x20, 0x68, 0x85, 0x4C, 
                    0x00 ,0x18 ,0x03 ,0x27 ,0x20 ,0x11 ,0x22 ,0x12 ,0x01 ,0xAA ,
                    0x00 ,0x1D ,0x01 ,0x18 ,0x03 ,0x27 ,0x20 ,0x11 ,0x22 ,0x00 ,
                    0x00 ,0x00 ,0x00 ,0x00 ,0xFF ,0xFF ,0xFF ,0xFF ,0xFF ,0xFF ,
                    0xFF ,0xFF ,0xFF ,0xFF ,0xFF ,0xFF ,0xFF ,0xFF ,0xA0 ,0x00 ,
                    0x96 ,0x40 ,0x08 ,0x07 ,0x64 ,0xFF ,0xFF ,0xFF ,0xFF ,0xFF ,
                    0xFF ,0xFF ,0xFF ,0xFF ,0xFF ,0xFF ,0xFF ,0xFF ,0xFF ,0xFF ,
                    0xFF ,0xFF ,0xFF ,0xFF ,0xFF ,0xFF ,0xFF ,0xFF ,0xFF ,0xFF ,
                    0xFF ,0xFF ,0x00 ,0xFF ,0xFF ,0x89 ,0x01 ,0x9C ,0x16 };
/************************************************************************
 * @function: Farp_SendLoginData
 * @����: Զ��ͨ�����͵�¼֡
 * 
 * @����: 
 * @param: ch 0:GPRS,1:ETH
 * @����: 
 * @˵��: 
 * @����: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void Farp_SendLoginData(uint8 ch)
{
    gs_FarpVar.ldlen = VS_BuildLinkFrm(0x08, 0, gs_FarpVar.ld);
    if(gs_FarpVar.ldlen > 0)
    {
        if(ch == 0)
        {
            Farp_SendIpData(gs_FarpVar.ld, gs_FarpVar.ldlen);
            //Farp_SendIpData((uint8 *)cmdUpBuf, sizeof(cmdUpBuf));
        }
        else
        {
            //Farp_SendEthData(gs_FarpVar.ld, gs_FarpVar.ldlen);
        }
    }
}


/************************************************************************
 * @function: Farp_SendLoginOutData
 * @����: Զ��ͨ�������˳���¼֡
 * 
 * @����: 
 * @param: ch 0:GPRS,1:ETH
 * @����: 
 * @˵��: 
 * @����: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void Farp_SendLoginOutData(uint8 ch)
{
    //gs_FarpVar.lodlen = GD_BuildLinkFrm(2, gs_FarpVar.lod);
    if(gs_FarpVar.lodlen > 0)
    {
        if(ch == 0)
        {
            Farp_SendIpData(gs_FarpVar.lod, gs_FarpVar.lodlen);
        }
        else
        {
            //Farp_SendEthData(gs_FarpVar.lod, gs_FarpVar.lodlen);
        }
    }
}

uint8_t g_ucRtuLogin;

/************************************************************************
 * @function: Farp_SendTestData
 * @����: Զ��ͨ����������֡
 * 
 * @����: 
 * @param: ch 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void Farp_SendTestData(uint8 ch)
{
    if(gs_FarpVar.login)
    {
        gs_FarpVar.hklen = VS_BuildLinkFrm(0x09, 0, gs_FarpVar.hkd);
    }
    else
    {
        gs_FarpVar.hklen = VS_BuildLinkFrm(0x05, 0xA619, gs_FarpVar.hkd);
    }
    if(gs_FarpVar.hklen > 0)
    {
        if(ch == 0)
        {
//	            Farp_SendIpData((uint8 *)cmdUpBuf, sizeof(cmdUpBuf));
            Farp_SendIpData((uint8 *)gs_FarpVar.hkd, gs_FarpVar.hklen);

        }
        else
        {
            //Farp_SendEthData(gs_FarpVar.hkd, gs_FarpVar.hklen);
        }
    }
}

/************************************************************************
 * @function: Farp_SendTestData
 * @����: Զ��ͨ����������֡
 * 
 * @����: 
 * @param: ch 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void Farp_SendReportData(uint8 ch, uint8_t cmd, uint16_t err, uint16_t id)
{
    gs_FarpVar.hklen = VS_BuildReportFrm(cmd, id, err, gs_FarpVar.hkd);

    if(gs_FarpVar.hklen > 0)
    {
        if(ch == 0)
        {
//	            Farp_SendIpData((uint8 *)cmdUpBuf, sizeof(cmdUpBuf));
            Farp_SendIpData((uint8 *)gs_FarpVar.hkd, gs_FarpVar.hklen);

        }
        else
        {
            //Farp_SendEthData(gs_FarpVar.hkd, gs_FarpVar.hklen);
        }
    }
}


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
    ClearBuffer((uint8*)&gs_FarpVar, sizeof(FarpVar));
    memset((uint8_t *)&gs_FtpPara,0,sizeof(S_FTPPARA));
    
    LoadSystemParam(PARA_TYPE_FARP);

    gui_FarpWmRcSec = 10;                   //��ʼ�����10s�����Զ������
    gui_FarpSerRcSec = 10;
    //gui_FarpEthRcSec = 10;
    
    guc_FarpWmHKUnRpyCnt = 0;               //�����޻ظ���������������
    //guc_FarpEthHKUnRpyCnt = 0;
    
    gui_FarpWmSafeCnt = 0;
    gui_FarpSerSafeCnt = 0;
    //gui_FarpEthSafeCnt = 0;
    
    guc_FarpWmRcCnt = 0;                   //����ʧ�ܺ�������������
    guc_FarpSerRcCnt = 0;
    //guc_FarpEthRcCnt = 0;
    
    gui_FarpHdRstCnt = 0;                   //ģ�鸴λ����������
    guc_FarpNVHours = 0;                    //��IP����ʱ������
    guc_FarpPstTryCnt = 0;                   //��������ģʽ�ز���������������
    
    gui_FarpBkTmCnt = 0;                    //�����������������߼���������
    guc_FarpWmRcFailCnt = 0;
    guc_FarpWmRcFailNorFtpCnt = 0;
    //guc_FarpEthRcFailCnt = 0;
    
    gui_FarpServerNoDataCnt = 0;
    guc_FarpFromClnCloseCnt = 0;
    
    guc_FarpLoginConfirmErr = 0;
    gul_FarpLoginDeadTime = 0xFFFFFFFF;
    
    gs_FarpVar.wmsv = 0x88000000;
    //gs_SysVar.mLPstt |= HLV_LPTASK_MDCK;

}

void Load_Para(void)
{
//	    gs_FarpVar.dcip = 0xC0A80101;                   //��վ��ѡip��ַ
//	    gs_FarpVar.dcpt = 8080;                   //��վ��ѡ�˿ں�
//	    gs_FarpVar.dcib = 0xC0A80101;                   //��վ����ip��ַ
//	    gs_FarpVar.dcpb = 8080;                   //��վ���ö˿ں�
//	    gs_FarpVar.dcip = 0xDA197792;                   //��վ��ѡip��ַ
//	    gs_FarpVar.dcpt = 5555;                   //��վ��ѡ�˿ں�
//	    gs_FarpVar.dcib = 0xDA197792;                   //��վ����ip��ַ
//	    gs_FarpVar.dcpb = 5555;                   //��վ���ö˿ں�
//	    devPara.Ip[0]=115;
//	    devPara.Ip[1]=236;
//	    devPara.Ip[2]=33;
//	    devPara.Ip[3]=164;
//	    devPara.NetPort=8013;
//	    gs_FarpVar.dcip = 0x73EC21A4;                   //��վ��ѡip��ַ

//	    gs_FarpVar.dcpt = 8012;                   //��վ��ѡ�˿ں�
//	    gs_FarpVar.dcib = 0x73EC21A4;                   //��վ����ip��ַ
//	    gs_FarpVar.dcpb = 8012;                   //��վ���ö˿ں�
//	
//	    MoveBuffer("cmnet", gs_FarpVar.gapn, 6);
    
    gs_FarpVar.hbcy = 120;              //Ĭ��10min
    gs_FarpVar.login = 0;
    gs_FarpVar.ntpc = 0;                 //���ӷ�ʽ(00:TCP_01:UDP)
    gs_PstPara.wkms = 1;       //ģ�鹤��ģʽ
    gs_PstPara.fpms = 1;       //�ͻ��� ����ģʽ
    gs_PstPara.rdct = 3;              //(����ģʽ)�ز�����
    gs_FarpVar.rttm = 60;  //�������(��)
    
    //MoveBuffer(buff + 5, gs_PstPara.wksd, 3);//(ʱ��ģʽ)����ʱ��
    //gs_FarpVar.clnen = 1;
    //gs_FarpVar.seren = 0;

    Farp_SetWkmd();
//	    memset((uint8_t *)&gs_FtpPara, 0, sizeof(gs_FtpPara));
//	    strcpy(gs_FtpPara.ftp_domain,"118.31.50.214");
//	    gs_FtpPara.port = 21;
//	    strcpy(gs_FtpPara.ftp_fold,"rtu");
//	    strcpy(gs_FtpPara.ftp_file,"RTU01.bin");
//	    strcpy(gs_FtpPara.ftp_user,"sgp");
//	    strcpy(gs_FtpPara.ftp_pwd,"viewshine@123");
    gs_FtpPara.err_times = 0;
//	    memset(gs_FarpVar.user,0,32);
//	    memset(gs_FarpVar.word,0,32);
//	    memset(gs_PstPara.device,0,21);
//	
//	    MoveBuffer("user", gs_FarpVar.user, strlen("user"));       //ר���û���
//	    MoveBuffer("pwd", gs_FarpVar.word, strlen("pwd"));  //ר������
    MoveBuffer("12345678901234567890", gs_PstPara.device, 20);
//	    gs_PstPara.Addr[0] = 0x01;
//	    gs_PstPara.Addr[1] = 0x01;
//	    gs_PstPara.Addr[2] = 0x15;
//	    gs_PstPara.Addr[3] = 0x05;
//	    gs_PstPara.Addr[4] = 0x18;
//	    gs_PstPara.Addr[5] = 0x20;


}

/************************************************************************
 * @function: Farp_InitProc
 * @����: Զ�̽��̳�ʼ������
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void Farp_InitProc(void)
{ 
//	    SYS_MODM_Reinit();                       //����ATMODEM����
    SYS_MODM_Init(&gs_TKFarpQueue);
    guc_FarpReActiveFlag = 1;
}



/************************************************************************
 * @function: Farp_WMSecondProc
 * @����: ����è���봦��
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void Farp_WMSecondProc(void)
{
    uint8 uc_closeclnflag = 0;
    uint8 uc_closeserflag = 0;
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
    TModemState modemstt = SYS_MODM_Status();

    if(gui_FarpWmSafeCnt > 0)               //����ͨѶ״̬����������ȫʱ�䵹��ʱ
    {
        gui_FarpWmSafeCnt--;
    }
    
    if(gui_FarpSerSafeCnt > 0)              //TCP Server ������ͨѶ״̬����������ȫʱ�䵹��ʱ
    {
        gui_FarpSerSafeCnt--;
    }
    
    if(!(modemstt.bit.typeChecked))         //ģ�黹δʶ��
    {
        return;
    }
    
    if(!(modemstt.bit.chnrdy))              //�Ƿ����AT����
    {
        if(++guc_FastWmSttReqCnt >= 5)      //ÿ5����
        {
            guc_FastWmSttReqCnt = 0;
            //SYS_MODM_Step(Modem_Init);
            SYS_MODM_Reinit();
            
            gs_GPIO.BlinkSet(GPIO_LED_GPRS, 0xFF, 0, 0);
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
            SYS_MODM_BeginCheck();
        }
        return;
    }

    gs_FarpVar.wmst |= WMS_STT_GSM;
    gs_FarpVar.mcsq = modemstt.bit.signl;
    SYS_Dev_OptBlinkSet(GPIO_LED_GPRS, 1, 10, 10, 0);  //ע��GSM����ɹ���1HZ��˸
    
    if(!(modemstt.bit.pppoe))       //�Ƿ����PPP����
    {
        gs_FarpVar.wmst &= ~WMS_STT_PPP;    //����ͨ��������
        if(++guc_FastWmSttReqCnt >= 10)     //ÿ10����
        {
//	            SYS_Message_Send(MSG_FARP_CNTPPP, TASK_FARP_TKID);
            uint8 msg = MSG_FARP_CNTPPP;
            krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
            
            guc_FastWmSttReqCnt = 0;
        }
//	        
//	        if(guc_FastWmSttReqCnt % 3 == 0)     //ÿ3����
//	        {
//	            SYS_MODM_BeginCheck();
//	        }
        return;
    }
    gs_FarpVar.wmst |= WMS_STT_PPP;
    
    
    


//	    if(gs_FarpVar.clnen == 1)           //�ͻ���ģʽ
    {
        if(g_ulTcp_fd < 0)    //TCP ������
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
                        krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
                        
                    }
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
                    krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
                    
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
                        krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
                        
                        uc_closeclnflag = 1;
                        guc_FarpWmHKUnRpyCnt = 0; //�����޻ظ���������
                    }
                    else
                    {
                        //Farp_SendHKData(0);      //������������
                        Farp_SendTestData(0);
                        guc_FarpWmHKUnRpyCnt++;
                        gui_FarpWmHBCYCnt = gs_FarpVar.hbcy;
                    }
                }
            }
            else
            {
                gui_FarpWmHBCYCnt = gs_FarpVar.hbcy;
            }
            gui_FarpWmRcSec = 0;                //���Դ�������
                                                // 
            if(gs_FarpVar.wkmd == 0)            //ʵʱ����ģʽ
            {
                //....
            }
            else if(gs_FarpVar.wkmd == 1)       //��������ģʽ
            {
                gui_FarpBkTmCnt++;              //���������߼������ۼ�
                                                //������ͨѶʱ�䳬����ֵ����Ҫ�Ͽ�����
                if(gui_FarpBkTmCnt >= gs_FarpVar.slep)
                {                               //�öϿ�������Ϣ
//	                    SYS_Message_Send(MSG_FARP_DISGPRS, TASK_FARP_TKID);
                    uint8 msg = MSG_FARP_DISGPRS;
                    krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
                    
                    uc_closeclnflag = 1;
                }
            }
            else                                //����ģʽֱ�ӶϿ�����
            {
//	                SYS_Message_Send(MSG_FARP_DISGPRS, TASK_FARP_TKID);
                
                uint8 msg = MSG_FARP_DISGPRS;
                krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
                uc_closeclnflag = 1;
            }
        }
    }
    
//	    if((modemstt.bit.gsstt == 0)  && (uc_closeclnflag == 1))     //�����ͻ���ʱ�����ַ���������Ҳʧ�ܵģ�  ��������PPP
//	    {
//	//	        SYS_Message_Send(MSG_FARP_DISPPP, TASK_FARP_TKID);                 
//	//	        SYS_Message_Send(MSG_FARP_CNTPPP, TASK_FARP_TKID);    
//	        uint8 msg = MSG_FARP_DISPPP;
//	        krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
//	        msg = MSG_FARP_CNTPPP;
//	        krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
//	    }
//	    if(uc_closeserflag == 1)
//	    {
//	//	        SYS_Message_Send(MSG_FARP_CNTSERVER, TASK_FARP_TKID);
//	        uint8 msg = MSG_FARP_CNTSERVER;
//	        krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
//	        
//	    }
    if((uc_closeclnflag == 1) && (gs_FarpVar.wkmd == 0))
    {
//	        SYS_Message_Send(MSG_FARP_CNTGPRS, TASK_FARP_TKID);
        uint8 msg = MSG_FARP_CNTGPRS;
        krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
        
    }
}






/************************************************************************
 * @function: Farp_SecondProc
 * @����: Զ��ͨ�����¼�������
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void Farp_SecondProc(void)
{
    
    if(gs_SysVar.mDGstt & HLV_STT_NENG)
    {
        return ;
    }

    Farp_WMSecondProc();
    
  
                                            //��Ʒ���
    if(gs_FarpVar.wmst & (WMS_STT_CLN | WMS_ETH0STT))
    {
         SYS_LED_BlinkSet(GPIO_LED_GPRS, 1, 2, 0);//����
    }
    else if(gs_FarpVar.wmst & WMS_STT_GSM)      //GSM ���ߣ���˸
    {
         SYS_LED_BlinkSet(GPIO_LED_GPRS, 1, 10, 0);    //����
    }  
    else
    {
         SYS_LED_BlinkSet(GPIO_LED_GPRS, 0xFF, 0, 0);
    }
                                            //��ʼ���쳣��������
    if((!SYS_MODM_Status().bit.regtt) || (!SYS_MODM_Status().bit.chnrdy))
    {
        gui_FarpHdRstCnt++;                 //�������ۼ�
    
        if(gui_FarpHdRstCnt >= 120)         //���������ӻ�δ��ɳ�ʼ��,��λ�豸
        {
            gui_FarpHdRstCnt = 0;           //��λ����������
            SYS_MODM_Reset(1);               //��λ�豸
        }
        return;
    }
    else
    {
        gui_FarpHdRstCnt = 0;               //��ʼ�����,�������
    }
    
    if(guc_FarpReActiveFlag && gs_FarpVar.clnen)    //��������ģʽ���ɼ�����������Ҫ������վ��������
    {
        if((gs_FarpVar.wmst & WMS_STT_GSM) && (gs_FarpVar.wmst & WMS_STT_PPP)) 
        {
            guc_FarpReActiveFlag = 0;
//	            SYS_Message_Send(MSG_FARP_CNTGPRS, TASK_FARP_TKID);
            uint8 msg = MSG_FARP_CNTGPRS;
            krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
            
        }
        
    }

}



/************************************************************************
 * @function: Farp_MinProc
 * @����: �ִ���
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void Farp_MinProc(void)
{
    if(gs_SysVar.mDGstt & HLV_STT_NENG)
    {
        return ;
    }

    if((gs_FarpVar.wmst & WMS_STT_FTP) == 0)
        SYS_MODM_BeginCheck();                   //��������ģ��״̬��
    
    //--------------------------------------------------------------------
    
    if(gs_PstPara.fpms == 3)                //ʱ������ģʽ
    {                                       //ʵʱ���¹���״̬
        void Farp_SetWkmd(void);
        Farp_SetWkmd();
    }
    
}





/************************************************************************
 * @function: Farp_HourProc
 * @����: Сʱ������
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void Farp_HourProc(void)
{
    guc_FarpNVHours++;                      //������ͨѶСʱ������

    if(guc_FarpNVHours >= 16)               //��������������ͨѶ��λARMϵͳ
    {
        aos_msleep(30000);                  //�ȴ�30s,��ʹϵͳ�ܹ�����Ҫ���ݱ������
        guc_FarpNVHours = 0;
                                            //��λϵͳ
        //gs_OS.Message_Send(MSG_MAIN_RESET, TASK_MAIN_TKID);
    }
}


/************************************************************************
 * @function: Farp_DayProc
 * @����: �촦����
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void Farp_DayProc(void)
{
//	    SYS_Message_Send(MSG_FARP_VAR, TASK_FARP_TKID);
}

#if (Modem_M72X_SMS_EN > 0)
/************************************************************************
 * @function: Farp_SmgReadProc
 * @����: ��ȡ����Ϣ
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void Farp_SmgReadProc(void)
{
    uint8 uc_data[256];                     //��ʱ����
    uint8 format;
    uint8 uc_len = 0;

    uc_len = SYS_MODM_ReadSMS(uc_data, guc_RecvSmgNo, &format);

    _IF_TRUE_RETURN_VOID(uc_len == 0);

//	    SYS_LED_BlinkSet(LED_FAR_R, 0, 0, 0);
                                            //ת��Ŀ�����
    MoveBuffer(guc_RecvSmgNo, gs_FarpVar.smfc, 17);
                                            //��������֡��������
//	    PstFrmProc(PST_CHN_SMS, uc_data, uc_len, uc_data, uc_len);

}
#endif



/************************************************************************
 * @function: Farp_IpDataReadProc
 * @����: ��ȡIP���ݲ�����
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void Farp_IpDataReadProc(void)
{
    uint8 uc_data[LEN_OF_IPRECV];                    //��ʱ����
    int32_t length;
    uint16 ui_len;
                                            //ѭ����ȡIP����(����֡��Ҫ��ȡ���)
    length = 0;
//	    for(uint8 uc_i = 0; uc_i < 10; uc_i++)
//	    {                                       //���ζ�ȡ����
//	        ui_len = SYS_MODM_ReadIP(uc_data + length, sizeof(uc_data) - length);
//	        length += ui_len;
//	                                            //��������,�����ݳ��ȹ�С,������Ϊ�ǳ���֡���ս���.
//	        if((ui_len > 0) && (ui_len < 900))
//	        {
//	            break;
//	        }
//	                                            //��������,�˳�.
//	        if(length >= sizeof(uc_data))
//	        {
//	            break;
//	        }
//	        aos_msleep(200); 
//	    }
    length = HAL_TCP_Read(g_ulTcp_fd, uc_data, LEN_OF_IPRECV, 0);
    

    _IF_TRUE_RETURN_VOID(length <= 0);      //���������ݳ���Ϊ0�򷵻ز�����

//	    SYS_LED_BlinkSet(LED_FAR_R, 0, 0, 0);
    
    LOG_DUMPHEX(LOG_LEVEL_DEBUG, "tcp recv:\n", uc_data, length);
    PstFrmProc(PST_CHN_TCP, uc_data, length, uc_data, length);  //����֡�ı�������

    guc_FarpWmHKUnRpyCnt = 0;               //�յ�IP����,�������޻ظ�����������
    guc_FarpNVHours = 0;                    //��IP����ʱ������
}




/************************************************************************
 * @function: Farp_UpDataActiveIP
 * @����: ��������ģʽ�£������������ݣ�֮ǰ�Ƚ����ͻ���
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2015/03/27)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void Farp_UpDataActiveIP(void)
{
    if(gs_FarpVar.clnen == 1)           //�ͻ���ģʽ
    {
        if(g_ulTcp_fd < 0) 
        {             
//	            SYS_Message_Send(MSG_FARP_CNTGPRS, TASK_FARP_TKID);   //������GPRS��Ϣ
            uint8 msg = MSG_FARP_CNTGPRS;
            krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
        }
    }
    
    guc_FarpPstTryCnt = 0;                   //����ģʽ���Դ�������.
}


/************************************************************************
 * @function: Farp_RingProc
 * @����: �����źŴ���
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void Farp_RingProc(void)
{
                                            //������GPRS��Ϣ
    uint8 msg = MSG_FARP_CNTGPRS;
    krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
                                            
//	    SYS_Message_Send(MSG_FARP_CNTGPRS, TASK_FARP_TKID);
    guc_FarpPstTryCnt = 0;                   //����ģʽ���Դ�������.
}


/************************************************************************
 * @function: Farp_Close_PPP
 * @����: ע��PPP����
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2015/03/23)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void Farp_Close_PPP(void)
{
    SYS_MODM_ClosePPP();                     //�Ͽ�PPP����
    
    gs_FarpVar.wmst &= ~WMS_STT_PPP;        //����״̬��
    gs_FarpVar.wmst &= ~WMS_STT_CLN;        //����״̬��
    gs_FarpVar.wmst &= ~WMS_STT_SER;        //����״̬��
    gs_FarpVar.wmst &= ~WMS_STT_SER_CNT1;
    gs_FarpVar.wmst &= ~WMS_STT_SER_CNT2;
}


/************************************************************************
 * @function: Farp_Creat_PPP
 * @����: ����PPP����
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2015/03/23)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void Farp_Creat_PPP(void)
{
    if(!(gs_FarpVar.wmst & WMS_STT_GSM))      //
    {
        return;
    }
    if((gs_FarpVar.wmst & WMS_STT_PPP))
    {
        SYS_MODM_ClosePPP();                 //�ȹر�PPP����
    }
    
    for(uint8 uc_i = 0; uc_i < 3; uc_i++)
    {
        aos_msleep(1000);
        if(!SYS_MODM_CreatePPP(
                (string)gs_FarpVar.gapn, 
                (string)gs_FarpVar.user, 
                (string)gs_FarpVar.word
                ))      
        {                                   
            gs_FarpVar.wmst |= WMS_STT_PPP;
            return;                     //�����ɹ�ֱ���˳�
        }
    }
    SYS_MODM_ClosePPP();                 //TCP���Ӳ��ɹ�,�ر�PPP����
return ;
}



/************************************************************************
 * @function: Farp_WM_Connect
 * @����: ����GPRS����
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void Farp_WM_Connect(void)
{
    uint8 uc_i;                             //ѭ������
    uint8_t ip[32];
    memset(ip,0,32);
    if(!(gs_FarpVar.wmst & WMS_STT_GSM) || !(gs_FarpVar.wmst & WMS_STT_PPP))      //
    {
        return;
    }
    
    if(!gs_FarpVar.clnen)   //�ͻ���ģʽ����
    {
        return;
    }
    
    if((gs_FarpVar.wmst & WMS_STT_CLN))       //��ǰ����ͨ���Ƿ��Ѿ�����
    {                                       //�öϿ�������Ϣ
//	        SYS_Message_Send(MSG_FARP_DISGPRS, TASK_FARP_TKID);
        uint8 msg = MSG_FARP_DISGPRS;
        krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
        
        gui_FarpWmRcSec = 1;                //�Ͽ����Ӻ���������        
        guc_FarpPstTryCnt = 0;               //����ģʽ������������
        return;
    }

    if(gs_FarpVar.wkmd == 1)                //��������ģʽ
    {                                       //�������������������򲻽�������
        if(guc_FarpPstTryCnt > gs_FarpVar.rtct)
        {
            return;
        }
        guc_FarpPstTryCnt++;                 //�������������ۼ�
    }

    guc_FarpWmRcFailCnt++;                  //����ʧ�ܴ����ۼ�
    if(guc_FarpWmRcFailCnt > 2)             //��������:����3������ʧ��,��λģ��
    {
        if(gs_FarpVar.wmst & WMS_STT_FTP)
        {
            guc_FarpWmRcFailNorFtpCnt ++;
            if(guc_FarpWmRcFailNorFtpCnt > 1000)
            {
                SYS_MODM_Reset(1);                   //��λ�豸
                gs_FarpVar.wmst = 0;
                guc_FarpWmRcFailNorFtpCnt = 0;
            }
        }
        else
        {
            SYS_MODM_Reset(1);                   //��λ�豸
            gs_FarpVar.wmst = 0;
        }
        guc_FarpWmRcFailCnt = 0;

        return ;
    }
    
    gui_FarpWmRcSec = 0;                    //���Լ������������
    sprintf(ip,"%03d.%03d.%03d.%03d", (uint8_t)(gs_FarpVar.dcip >> 24), (uint8_t)(gs_FarpVar.dcip >> 16), (uint8_t)(gs_FarpVar.dcip >> 8), (uint8_t)(gs_FarpVar.dcip));
    for(uc_i = 0; uc_i < gs_FarpVar.rdct; uc_i++)
    {                                       //��������֮�������ʱ
        aos_msleep(1000);
        
        if(!(gs_FarpVar.wmst & WMS_STT_CLN))
        {
                                                //������ѡ��վ
            if(!(guc_FarpLoginConfirmErr & 0x01))   //�ϴ����õ�½ȷ�����쳣
            {
                
//	                if(!SYS_MODM_CreateIP(gs_FarpVar.ntpc, gs_FarpVar.dcip, gs_FarpVar.dcpt))
                g_ulTcp_fd = HAL_TCP_Establish(ip, gs_FarpVar.dcpt);
                if(g_ulTcp_fd >= 0)
                {
                    guc_FarpLoginConfirmErr = 0;
                    gul_FarpLoginDeadTime = gul_SysRunSecs + 10;
                                                    //
                    gs_FarpVar.wmst |= WMS_STT_CLN;
                    guc_FarpPstTryCnt = 0;          //����ģʽ������������
                    Farp_SendLoginData(0);          //���͵�¼֡
                    //Farp_SendHKData(0);             //��������֡
                                                    //��ʼ����������
                    gui_FarpWmHBCYCnt = 5;//gs_FarpVar.hbcy;
                                                    //ʵʱ����ģʽ����Ч���ݼ�������ʼ��
                    gui_FarpWmSafeCnt = gs_FarpVar.hbcy;
                    guc_FarpWmRcFailCnt = 0;        //����ʧ�ܴ�������
                    
                    //SYS_MODM_Step(Modem_IPState);
                    return;
                }
                else
                {
                    //����ʧ��
                    //SYS_MODM_Reset(0);
                }                
            }
                                                    //���ӱ�����վ
//	            if(!(guc_FarpLoginConfirmErr & 0x02))   //�ϴα��õ�½ȷ�����쳣
//	            {
//	//	                if(!SYS_MODM_CreateIP(gs_FarpVar.ntpc, gs_FarpVar.dcib, gs_FarpVar.dcpb))
//	                g_ulTcp_fd = HAL_TCP_Establish(ip, gs_FarpVar.dcpt);
//	                if(g_ulTcp_fd >= 0)
//	                {
//	                    guc_FarpLoginConfirmErr = 0;
//	                    gul_FarpLoginDeadTime = gul_SysRunSecs + 10;
//	                                                    //
//	                    gs_FarpVar.wmst |= WMS_STT_CLN;
//	                    guc_FarpPstTryCnt = 0;          //����ģʽ������������
//	                    Farp_SendLoginData(0);          //���͵�¼֡
//	                    //Farp_SendHKData(0);             //��������֡
//	                                                    //��ʼ����������
//	                    gui_FarpWmHBCYCnt = 5;//gs_FarpVar.hbcy;
//	                    gui_FarpWmSafeCnt = gs_FarpVar.hbcy;
//	                    guc_FarpWmRcFailCnt = 0;        //����ʧ�ܴ�������
//	                    
//	                    //SYS_MODM_Step(Modem_IPState);
//	                    return;
//	                }
//	                else
//	                {
//	                    //����ʧ��
//	                    //SYS_MODM_Reset(0);
//	                }
//            }
            guc_FarpLoginConfirmErr = 0;
        }
        if((!(gs_FarpVar.wmst & WMS_STT_CLN)) && (!(gs_FarpVar.wmst & WMS_STT_SER)))
        {
//	            SYS_Message_Send(MSG_FARP_DISPPP, TASK_FARP_TKID);                 //TCP���Ӳ��ɹ�,�ر�PPP����
            uint8 msg = MSG_FARP_DISPPP;
            krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
            
        }
    }
                                            //�����ϴ˴�����Ҫdiscnt
                                            //����ʹ�ù����з���ĳЩģ��CreateIP���ɹ���Ҳ��ҪDiscntIP
//	    SYS_Message_Send(MSG_FARP_DISGPRS, TASK_FARP_TKID);
                                            
//	    SYS_Message_Send(MSG_FARPE_FCNT, TASK_FARP_TKID);
    uint8 msg = MSG_FARP_DISGPRS;
    krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
    msg = MSG_FARPE_FCNT;//����ʧ�ܴ���
    krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);                                            
    if(guc_FarpWmRcCnt > 0)                //ͨѶ״̬�¶�������ʧ�ܺ�5�������
    {
        guc_FarpWmRcCnt--;
        gui_FarpWmRcSec = 10;
    }
}


/************************************************************************
 * @function: Farp_Creat_Server
 * @����: ����TCP�����
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	void Farp_Creat_Server(void)
//	{
//	    uint8 uc_i;                             //ѭ������
//	    
//	    if(!(gs_FarpVar.wmst & WMS_STT_GSM) || !(gs_FarpVar.wmst & WMS_STT_PPP))      //
//	    {
//	        return;
//	    }
//	    
//	    if(!gs_FarpVar.seren)   //�ͻ���ģʽ����
//	    {
//	        return;
//	    }
//	    
//	    if((gs_FarpVar.wmst & WMS_STT_SER))       //��ǰ����ͨ���Ƿ��Ѿ�����
//	    {                                       //�öϿ�������Ϣ
//	//	        SYS_Message_Send(MSG_FARP_DISSERVER, TASK_FARP_TKID);
//	        uint8 msg = MSG_FARP_DISSERVER;
//	        krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
//	        
//	        gui_FarpSerRcSec = 1;                //�Ͽ����Ӻ���������        
//	        return;
//	    }
//	
//	    guc_FarpSerRcFailCnt++;                  //����ʧ�ܴ����ۼ�
//	    if(guc_FarpSerRcFailCnt > 5)             //��������:����5������ʧ��,��λģ��
//	    {
//	        SYS_MODM_Reset(1);                   //��λ�豸
//	        guc_FarpSerRcFailCnt = 0;
//	    }
//	    
//	    gui_FarpSerRcSec = 0;                    //���Լ������������
//	
//	    for(uc_i = 0; uc_i < gs_FarpVar.rdct; uc_i++)
//	    {                                       //��������֮�������ʱ
//	        sleep_ms(1000);
//	        
//	        if(!(gs_FarpVar.wmst & WMS_STT_SER))
//	        {
//	            if(!SYS_MODM_CreatServer(0, gs_FarpVar.lispt))  //
//	            {
//	                gs_FarpVar.wmst |= WMS_STT_SER;
//	                gs_FarpVar.wmst &= ~WMS_STT_SER_CNT1;
//	                gs_FarpVar.wmst &= ~WMS_STT_SER_CNT2;
//	                
//	                gui_FarpFromClnNoDataCnt = gs_FarpVar.hbcy * 10;    //���չ涨Ϊ��10������������
//	                gui_FarpSerSafeCnt = gs_FarpVar.hbcy;
//	                guc_FarpSerRcFailCnt = 0;        //����ʧ�ܴ�������
//	                guc_FarpFromClnCloseCnt = 0;
//	                
//	                SYS_MODM_Step(Modem_IPState);
//	                return;
//	            }
//	        }
//	    }
//	    
//	    if((!(gs_FarpVar.wmst & WMS_STT_CLN)) && (!(gs_FarpVar.wmst & WMS_STT_SER)))
//	    {
//	//	        SYS_Message_Send(MSG_FARP_DISPPP, TASK_FARP_TKID);                 //TCP���Ӳ��ɹ�,�ر�PPP����
//	        uint8 msg = MSG_FARP_DISPPP;
//	        krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
//	        
//	    }
//	                                            //�����ϴ˴�����Ҫdiscnt
//	                                            //����ʹ�ù����з���ĳЩģ��CreateIP���ɹ���Ҳ��ҪDiscntIP
//	//	    SYS_Message_Send(MSG_FARP_DISSERVER, TASK_FARP_TKID);
//	//	                                            //����ʧ�ܴ���
//	//	    SYS_Message_Send(MSG_FARPE_FCNT, TASK_FARP_TKID);
//	    uint8 msg = MSG_FARP_DISSERVER;
//	    krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
//	    msg = MSG_FARPE_FCNT;
//	    krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
//	    if(guc_FarpSerRcCnt > 0)                //ͨѶ״̬�¶�������ʧ�ܺ�5�������
//	    {
//	        guc_FarpSerRcCnt--;
//	        gui_FarpSerRcSec = 10;
//	    }
//	}

/************************************************************************
 * @function: Farp_PrioConnect
 * @����: �������ȼ�ѡ��ͨ����������
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void Farp_PrioConnect(void)
{
    if(!(gs_FarpVar.wmst & WMS_STT_CLN))
//	    {
//	        SYS_Message_Send(MSG_FARP_CNTETH, TASK_FARP_TKID);
//	        uint8 msg = MSG_FARP_DISFROMCLN1;
//	        krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
//	        
//	    }
//	    else
    {
//	        SYS_Message_Send(MSG_FARP_CNTGPRS, TASK_FARP_TKID);
        uint8 msg = MSG_FARP_CNTGPRS;
        krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
        
    }
}

intptr_t Farp_WM_Connect_Once(void)
{
    uint8 uc_i;                             //ѭ������
    uint8_t ip[32];
    memset(ip,0,32);
    intptr_t socket_id = -1;

    if(g_ulTcp_fd >= 0)
        return g_ulTcp_fd;
//	    gs_FarpVar.wmst &= ~WMS_STT_PPP;        //����״̬��
    if((!SYS_MODM_Status().bit.pppoe))
    {
        if(SYS_ERR_OK != SYS_MODM_Reset(0))
        {
            return socket_id;
        }
    }
    LOG_INFO( "%s %d WM Connect \r\n", __func__, __LINE__);
    sprintf(ip,"%03d.%03d.%03d.%03d", (uint8_t)(gs_FarpVar.dcip >> 24), (uint8_t)(gs_FarpVar.dcip >> 16), (uint8_t)(gs_FarpVar.dcip >> 8), (uint8_t)(gs_FarpVar.dcip));
    for(uc_i = 0; uc_i < 2; uc_i++)
    {                                       
       
        socket_id = HAL_TCP_Establish(ip, gs_FarpVar.dcpt);
        if(socket_id >= 0)
        {
            Farp_SendLoginData(0);          //���͵�¼֡
            return socket_id;
        }

    }
    return socket_id;
}
const uint32_t gl_constTestIp   = 0x73EC21A4;
const uint32_t gl_constTestPort = 8012;

int IP_Check(void)
{
    uint8_t ip[32];
    memset(ip,0,32);
    intptr_t g_ulTstTcp_fd = -1;

    sprintf(ip,"%03d.%03d.%03d.%03d", (uint8_t)(gl_constTestIp >> 24), (uint8_t)(gl_constTestIp >> 16), (uint8_t)(gl_constTestIp >> 8), (uint8_t)(gl_constTestIp));
    if((gs_FarpVar.wmst & WMS_STT_GSM))
    {
        for(int uc_i = 0; uc_i < 2; uc_i++)
        {   
            g_ulTstTcp_fd = HAL_TCP_Establish(ip, gl_constTestPort);
            if(g_ulTstTcp_fd >= 0)
            {
                break;
            }
        }
        if(g_ulTstTcp_fd < 0)
        {
            gs_SysVar.terstt.bit.ipcheck = 1;
            
            LOG_INFO(DBGFMT_A"IP check failed fd = %d\n",DBGARG_A, g_ulTstTcp_fd);
        }
        else
        {
            gs_SysVar.terstt.bit.ipcheck = 0;
            
            HAL_TCP_Destroy(g_ulTstTcp_fd);
            
            LOG_INFO(DBGFMT_A"IP check succ fd = %d\n",DBGARG_A, g_ulTstTcp_fd);
        }
    }
    else
    {
        aos_msleep(5000);
        gs_OS.Message_Send(&gs_TKSlvQueue, &msgidA[MSG_FARP_CHECK], 1);
        gs_SysVar.terstt.bit.ipcheck = 1;
        
        LOG_INFO(DBGFMT_A"IP check wait 5s\n",DBGARG_A);
    }
    return 0;
}



/************************************************************************
 * @function: Farp_Close_Server
 * @����: �Ͽ�TCP Server ����
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	void Farp_Close_Server(void)
//	{
//	    SYS_MODM_CloseTcpServer();                     //�Ͽ�TCP����
//	                                            //�Ͽ����ӳɹ�
//	    gs_FarpVar.wmst &= ~WMS_STT_SER;        //����״̬��
//	    gs_FarpVar.wmst &= ~WMS_STT_SER_CNT1;
//	    gs_FarpVar.wmst &= ~WMS_STT_SER_CNT2;
//	}



/************************************************************************
 * @function: Farp_WM_DisCnt
 * @����: �Ͽ�GPRS����
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void Farp_WM_DisCnt(void)
{
//	    Farp_SendLoginOutData(0);                //�����˳���½֡
    aos_msleep(200);
//	    SYS_MODM_CloseIP();                     //�Ͽ�TCP����
    if(g_ulTcp_fd >= 0)
    {
        HAL_TCP_Destroy(g_ulTcp_fd);
        g_ulTcp_fd = -1;
    }
    gs_FarpVar.wmst &= ~WMS_STT_CLN;        //����״̬��
    gui_FarpBkTmCnt = 0;                    //��������ģʽ������ͨѶ����ʱ�����������
}




/************************************************************************
 * @function: Farp_Close_From_Client1
 * @����: �Ͽ��յ��Ŀͻ�������1
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	void Farp_Close_From_Client1(void)
//	{
//	    SYS_MODM_CloseFromClient(0);                     //�Ͽ��յ��Ŀͻ�������1
//	
//	    gs_FarpVar.wmst &= ~WMS_STT_SER_CNT1;
//	}


/************************************************************************
 * @function: Farp_Close_From_Client2
 * @����: �Ͽ��յ��Ŀͻ�������2
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	void Farp_Close_From_Client2(void)
//	{
//	    SYS_MODM_CloseFromClient(1);                     //�Ͽ��յ��Ŀͻ�������1
//	
//	    gs_FarpVar.wmst &= ~WMS_STT_SER_CNT2;
//	}



/************************************************************************
 * @function: Farp_RecvValidIpProc
 * @����: Զ��ͨ���յ���ЧIp���ݺ�Ĵ���
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void Farp_RecvValidIpProc(void)
{
    gui_FarpBkTmCnt = 0;                    //���������߼���������
    
    gui_FarpWmSafeCnt = gs_FarpVar.hbcy;    //������Ч���ݼ�������λ
    //gui_FarpEthSafeCnt = gs_FarpVar.hbcy;
    
    gui_FarpWmHBCYCnt = gs_FarpVar.hbcy;
//#if SG_INSPECT
//    gui_FarpEthHBCYCnt = 60;//Ĭ��1����
//#else
//    gui_FarpEthHBCYCnt = gs_FarpVar.hbcy;
//#endif    
}

       
       
       
/************************************************************************
 * @function: Farp_RecvValidTcpSerProc
 * @����: Զ��ͨ���յ���ЧTcpServer���ݺ�Ĵ���
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void Farp_RecvValidTcpSerProc(void)
{
    gui_FarpSerSafeCnt = gs_FarpVar.hbcy;
    gui_FarpFromClnNoDataCnt = gs_FarpVar.hbcy * 10;    //���չ涨Ϊ��10������������
    guc_FarpFromClnCloseCnt = 0;
}
       
/************************************************************************
**��������: void Farp_SetWkmd(void)
**�� ��: ����ģ�鹤��ģʽ���趨
**
**�� ��: 
**�� ��: 
**
**�� ע: 
**-----------------------------------------------------------------------
**�� ��: yzy
**�� ��: 09-09-15
*************************************************************************/
void Farp_SetWkmd(void)
{
  
    uint8 ucClnen = gs_FarpVar.clnen;
    uint8 ucSeren = gs_FarpVar.seren;
                                        //�趨�ն˹���ģʽ
    if(gs_PstPara.wkms == 1)            //�ͻ���ģʽ
    {
        gs_FarpVar.clnen = 1;
        gs_FarpVar.seren = 0;
    }
    else if(gs_PstPara.wkms == 2)       //������ģʽ
    {
        gs_FarpVar.clnen = 0;
        gs_FarpVar.seren = 1;
    }
    else                        // ���ģʽ
    {
        gs_FarpVar.clnen = 1;
        gs_FarpVar.seren = 1;
        gs_PstPara.fpms = 2;    //���ģʽ�£��ͻ���Ϊ��������
    }
    
    if(gs_FarpVar.seren)
    {
        if(ucSeren == 0)    //�� ��TCP server ״̬  �� ����TCP server ״̬
        {
//	            SYS_Message_Send(MSG_FARP_CNTSERVER, TASK_FARP_TKID);
            uint8 msg = MSG_FARP_CNTSERVER;
            krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
            
        }
    }
    
    if(gs_FarpVar.clnen)
    {
                                    //�趨�ͻ��˹���ģʽ
        uint8 mode = gs_FarpVar.wkmd;
        
                                            
        if(gs_PstPara.fpms == 2)                //��������ģʽ
        {
            gs_FarpVar.wkmd = 1;
        }
        else if(gs_PstPara.fpms == 3)           //ʱ������ģʽ
        {
            if(STR_GetBits(gs_PstPara.wksd, 1, GetTime()->hour))
            {
                gs_FarpVar.wkmd = 0;
            }
            else
            {
                gs_FarpVar.wkmd = 1;
            }
        }
        else                                    //��������ģʽ
        {
            gs_FarpVar.wkmd = 0;
        }
                                                //�趨�ز����Դ���
        if(gs_FarpVar.wkmd == 0)                //��������ģʽ
        {
            gs_FarpVar.rdct = 2;
        }
        else if(gs_FarpVar.wkmd == 1)           //����ģʽ
        {
            gs_FarpVar.rdct = gs_PstPara.rdct;  //�ز�����
            if(gs_FarpVar.rdct == 0)            //��������ģʽ�����ز�1��
            {
                gs_FarpVar.rdct = 1;
            }
            gs_FarpVar.rtct = 1;                //����һ��(ÿ�ΰ��ز���������)
        }
                                                //�ӱ����л�������,��Ҫ����
        if(((mode > gs_FarpVar.wkmd) && ((gs_FarpVar.wmst & WMS_STT_CLN) == 0)) || (ucClnen == 0))
        {
//	            SYS_Message_Send(MSG_FARP_CNTGPRS, TASK_FARP_TKID);
            uint8 msg = MSG_FARP_CNTGPRS;
            krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
            
        }
    }
}











/************************************************************************
**��������: void Farp_UpDateVar(void)
**�� ��: ��ʼ��Զ��ͨѶ�Ĳ���
**
**�� ��: 
**�� ��: 
**
**�� ע: 
**-----------------------------------------------------------------------
**�� ��: chenyj 
**�� ��: 
**-----------------------------------------------------------------------
**�� ��: yzy 
**�� ��: 
*************************************************************************/
void Farp_UpdateVar(void)
{
    LoadSystemParam(PARA_TYPE_FARP);
}



/************************************************************************
 * @function: Farp_ParaChg
 * @����: ��Ҫ���Ӳ�������,��Ҫ��������״̬
 * 
 * @����: 
 * @param: pdata 
 * @����: 
 * @˵��:
 * @����: yzy (2011-2-24)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void Farp_ParaChg(void)
{
    if(gs_FarpVar.wkmd == 0)            //ʵʱ����ģʽ
    {                                   //����
//	        SYS_Message_Send(MSG_FARP_CNTGPRS, TASK_FARP_TKID);
        uint8 msg = MSG_FARP_CNTGPRS;
        krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
        
    }
    else if(gs_FarpVar.wkmd == 1)       //��������ģʽ
    {                                   //�Ͽ�����
//	        SYS_Message_Send(MSG_FARP_DISGPRS, TASK_FARP_TKID);
        uint8 msg = MSG_FARP_DISGPRS;
        krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
        
    }
}


/*******************************************************************************
 * @function_name:  SYS_Farp_Task
 * @function_file:  farp.c
 * @����: Զ��ͨѶ���̴�����
 * 
 * 
 * @����: 
 * 
 * @����: 
 * @����: yzy (2011-1-24)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
void SYS_Farp_Task(void * arg)
{
                                            //������Ϣ
    krhino_buf_queue_create(&gs_TKFarpQueue, "farp_queue",
                         gc_TKFarpQbuf, MSG_BUFF_LEN, BUFQUEUE_MSG_MAX);
//	    krhino_sem_create(&gs_FarpProc, "farp_proc_sem", 1);
    
    Farp_InitProc();
        
    Load_Para();

#ifdef WITH_SAL
    while(1)
    {
        gs_SysVar.mLPstt |= HLV_LPTASK_MDCK;
    
        if(0 == sal_init())
            break;
        gs_SysVar.mLPstt &= ~HLV_LPTASK_MDCK;
        aos_msleep(10000);
    }
    gs_SysVar.mLPstt &= ~HLV_LPTASK_MDCK;
    if((gs_SysVar.mLPstt & HLV_LPTASK_CLSCK) == HLV_LPTASK_CLSCK)
    {
        Farp_WM_DisCnt();
        Farp_Close_PPP();
        SYS_MODM_Shutdown();
        gs_SysVar.mLPstt &= ~HLV_LPTASK_CLSCK;
        
        LOG_INFO("modem check ok! but ext pwr losed,so modem should shut down\n");    
    }
#endif    
    
    SYS_MSG_Apply(TASK_FARP_TKID, MSG_CLS_TM);               //��ȡʱ����Ϣ
    //SYS_MSG_Apply(TASK_FARP_TKID, MSG_CLS_MD);               //��ȡʱ����Ϣ
    memcpy(gs_FarpVar.modemip, SYS_MODM_GetIP(), 4);   //��ȡ����IP
//	    krhino_buf_queue_flush(&gs_TKFarpQueue);
    for(;;)
    {
//��ȡ������Ϣ
        krhino_buf_queue_recv(&gs_TKFarpQueue, RHINO_WAIT_FOREVER, g_TKFarpQ_buf_recv,
                                  &g_TKFarpQ_recv_size);

        switch(g_TKFarpQ_buf_recv[0])                        //������Ϣ�ֱ���
        {
            case MSG_SEC:
                gs_SysVar.mLPstt |= HLV_LPTASK_SMSG_FARP;
                
                Farp_SecondProc();
           
                gs_SysVar.mLPstt &= ~HLV_LPTASK_SMSG_FARP;
            
                break;

            case MSG_MIN:
                Farp_MinProc();
                break;

            case MSG_HOUR:
                Farp_HourProc();
                break;

            case MSG_DAY:
                Farp_DayProc();
                break;
#if (Modem_M72X_SMS_EN > 0)
            case MSG_SMRECV:                //��������
                Farp_SmgReadProc();
                break;
#endif                
            case MSG_RING:                  //����
                Farp_RingProc();
                break;
                
            case MSG_IP1DATA:               //IP����
                Farp_IpDataReadProc();
                break;
                
            case MSG_FARP_DISPPP:           //�ر�PPP����
                Farp_Close_PPP();
                break;
                
            case MSG_FARP_CNTPPP:           //����PPP����
                Farp_Creat_PPP();
                break;
                
            case MSG_FARP_CNTGPRS:          //����GPRS
                Farp_WM_Connect();
                break;

            case MSG_FARP_DISGPRS:          //�Ͽ�GPRS
                Farp_WM_DisCnt();
                break;
                

//	            case MSG_FARP_CNTSERVER:           //����һ��TCP Server
//	                Farp_Creat_Server();
//	                break;
//	                
//	            case MSG_FARP_DISFROMCLN1:           //�ر� �յ��� һ���ͻ�������1
//	                Farp_Close_From_Client1();
//	                break;
//	           
//	            case MSG_FARP_DISFROMCLN2:           //�ر� �յ��� һ���ͻ�������2
//	                Farp_Close_From_Client2();
//	                break;

            case MSG_FARP_RECVVALIDIP:      //IP�յ���Ч����
                Farp_RecvValidIpProc();
                break;
                
            case MSG_FARP_RECVVALIDSER:     //TCP Server �յ���Ч����
                Farp_RecvValidTcpSerProc(); 
                break;
                
            case MSG_FARP_PRIOCNT:          //�����ȼ�����
                Farp_PrioConnect();
                break;
            case MSG_FARP_VAR:              //��������
                Farp_UpdateVar();
                LOG_DEBUG("******MSG_FARP_VAR******** !!!!\n");
                
                break;

            case MSG_FARP_PARACHG:          //�����������
            
                Farp_ParaChg();
                break;
            case MSG_ENTER_LP:
                Farp_WM_DisCnt();
                Farp_Close_PPP();
                SYS_MODM_Shutdown();
                
                LOG_INFO("HLV_LPTASK_CLSCK modem shut down\n");    
                gs_SysVar.mLPstt &= ~HLV_LPTASK_CLSCK;
                break;
            case MSG_LIVE:
				HB_RetLive(TASK_FARP_TKID);
                break;

            default:
                break;
        }
        memset(g_TKFarpQ_buf_recv,0,2);
    }
}

/******************************************************************************
**Զ��ͨ�Ž��̿鳣�����ֶ���
//	******************************************************************************/
//	const TESTcbConst gs_TkFarp = 
//	{
//	    SYS_Farp_Task,
//	    TASK_FARP_TKID >> 3,                    //Ӧ�ù������
//	    TASK_FARP_TKID,                         //���̱��
//	    TASK_FARP_TKID & 7,                     //���ȼ��ڽ��̱��λ
//	    true,                                      //��Ϣѭ�����ƽ���
//	};



const KTaskConst gs_TkFarp = 
{
    &gs_TKFarpHandle,
    SYS_Farp_Task,                           //����è����
    gs_TKFarpStack,
    TASK_FARP_TKID,                           //���̱��
    true,                                    //��Ϣѭ�����ƽ���
    TASK_FARP_STKL,
    "modem",
    &gs_TKFarpQueue,
};


