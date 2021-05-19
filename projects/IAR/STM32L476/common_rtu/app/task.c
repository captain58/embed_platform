/****************************************Copyright (c)**************************************************
**                                  ____�Ƽ����޹�˾
**                                          ������
**
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: task.c
**��   ��   ��: yzy
**����޸�����: 2019��6��1��
**��        ��: �������ļ�
**ע        ��:
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: yzy
** ��  ��: v1.0
** �ա���: 2019��6��1��
** �衡��: ԭʼ�汾
********************************************************************************************************/

#define EXT_GD_TASK

#include "public.h"
#include "task.h"
#include "pst_zy.h"
#include "hlv.h"
#include "sal_import.h"
#include "atmodem.h"
#include "farp.h"
/**************************************************************
**��������ִ���ı�־
***************************************************************/
uint8 guc_TaskMinFlag;




/************************************************************************
**��������: uint8 REP_ChnSend(uint8* buffer, uint16 length, CHN_ENUM chninfo, REP_CFG* cfg)
**�� ��: ������ȷ���ϱ�֡.
**
**�� ��: 
**�� ��: SA_ERR_OK / others
**
**�� ע: 
**-----------------------------------------------------------------------
**�� ��: yzy
**�� ��: 09-09-09
*************************************************************************/
uint8 REP_ChnSend(uint8* buffer, uint16 length, uint8_t chninfo, REP_CFG* cfg)
{
    uint8 waitsec = gs_PstPara.repwait;
    uint8 trycnt = gs_PstPara.reptry + 1;       //����ִ�д���Ϊ�ط�����+1(�����ٷ���һ��)
    
    if(cfg->en == 0)                            //������ȴ��ظ�֡,ֱ�ӷ���
    {
        return gfs_PSTChnSend[chninfo](buffer, length);
    }
    else
    {                                           //���ȳ�����ŵ�ȷ�ϻظ�֡���ձ�־
        STR_ClrBit(bmap_AckRecv, cfg->pseq & 0x0F);
        
        for(uint8 uc_i = 0; uc_i < trycnt; uc_i++)
        {
            if(gfs_PSTChnSend[chninfo](buffer, length) == PST_ERR_OK)
            {                                   //�ȴ�����ȷ�ϻظ�֡,ÿ��100�����ж��Ƿ���ձ�־
                for(uint8 uc_j = 0; uc_j < waitsec; uc_j++)
                {
                    gs_OS.TK_Sleep(100);
                    if(STR_IsBitSet(bmap_AckRecv, cfg->pseq & 0x0F))
                    {
                        STR_ClrBit(bmap_AckRecv, cfg->pseq & 0x0F);
                        SYS_OK();
                    }
                }
            }
            else
            {
                aos_msleep(5000);
            }
        }
        
        return SYS_ERR_FT;
    }
    
}





//==============================================================











/************************************************************************
 * @function: _JudgeTaskFlag
 * @����: ��ִ�б�־λ.��������Ϣ.�������ʵʱ�Խ�����
 * 
 * @����: 
 * @param:  
 * 
 * @����: 
 * @return:  
 * @˵��: 
 * @����: yzy (2011-5-13)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void JudgeTaskFlag(uint8_t port)
{

    gss_TaskATimer.flag |= 1 << port;

    LOG_DEBUG("485 port[%d] set TaskFlag ********[ %02x ]******!!!!\n",port, gss_TaskATimer.flag);

    if((gss_TaskATimer.flag & gss_TaskATimer.runflag) == gss_TaskATimer.runflag)
    {
        gss_TaskATimer.atr |= TKT_ATR_FLAG; 
        
        gs_SysVar.mLPstt &= ~HLV_LPTASK_CP;
        gs_SysVar.mLPstt |= HLV_LPTASK_RP;
        
        gss_TaskATimer.flag = 0;
        gs_GPIO.GPO_Out(GPO_485_PWR, false);

    }


}




uint8 guc_TaskLinkWaitSec = 120;              //�������ϱ�ʱ,��������״̬����ʱ


/************************************************************************
 * @function: Task_Proc
 * @����: ��ͨ��������
 * @����: 
 * 
 * @����: 
 * @return: bool  true ����ȫ���ϱ����. false δ���(��ÿ���ٴν���)
 * @˵��: ÿ���ӵ���
 * @����: yzy (2011-5-16)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
ST_Netp_FW gs_report_fw;
extern intptr_t g_ulTcp_fd;
extern int32_t HAL_TCP_Destroy(intptr_t fd);
extern kbuf_queue_t gs_TKFarpQueue;

bool Task_Proc(void)
{
    //uint8 para[TASK_PARA_LEN];        //�����������
    uint8* para = gucs_TaskParaBuffer;  //�����������
    uint8 time[6];                      //��ǰʱ��(���ʱ������)
    uint8 lasttime[6];                  //��ʷ�ϱ�ʱ��(���ʱ������)
    uint8 index;
    
    PST_Frame frm;
    ZY_Frame* frame = &frm._zy;

    if(gs_SysVar.mDGstt & HLV_STT_LENG)
    {
        gs_SysVar.mLPstt &= ~HLV_LPTASK_RP;
        return false;
    }
    if(gss_TaskATimer.atr & TKT_ATR_FLAG)
    {
        if(gs_SysVar.mDGstt & HLV_STT_NENG)
        {
            if((gs_SysVar.mLPstt & HLV_LPTASK_CLSCK) == HLV_LPTASK_CLSCK)
            {
                return false;
            }
            g_ulTcp_fd = Farp_WM_Connect_Once();
            if(g_ulTcp_fd >= 0)
            {
                frm.chninfo = PST_CHN_TCP;        //0����ͨ��;3Զ��ͨ��
            }
            else
            {   SYS_MODM_Shutdown();
                //LOG_INFO("Farp_WM_Connect_Once failed modem shut down\n");    
                return false;
            }
            frame->recv = para;                  //�������
            frame->send = gucs_PstSbuf;          //���ͻ�����
            frame->cmd = 0x05;          //seq��(��֡,����ȷ��)
            frame->atr = GetMeterNum();
            frame->atr <<= 4;
            frame->atr |= 0x06;
            frame->seq = 0;
            ByteArrayHexToBcd(gss_TaskATimer.time.YMDhms, time, 6);
            memcpy(frame->recv, time,6);
            frame->len = 6;

            frame->wd.lword = 0xA621;
            frame->send += 21;
            Task_Execute(&frm, lasttime, time, true);

            
            frame->wd.lword = 0xA622;
            frame->send += 21;
            Task_Execute(&frm, lasttime, time, true);

            gss_TaskATimer.atr &= ~TKT_ATR_FLAG;
            gss_TaskATimer.flag = 0;
            
    //	        aos_msleep(300);
            

            if(gs_SysVar.mDGstt & HLV_STT_NENG)
            {
                if(g_ulTcp_fd >= 0)
                {
                    HAL_TCP_Destroy(g_ulTcp_fd);
                    g_ulTcp_fd = -1;
                    Farp_Close_PPP();
                    SYS_MODM_Shutdown();
                    LOG_INFO("report finish modem shut down\n");    
                }
            }
            gs_SysVar.mLPstt &= ~HLV_LPTASK_RP;
            
        }
        else
        {
            if(gs_FarpVar.wmst & WMS_STT_CLN)
            {
                frm.chninfo = PST_CHN_TCP;        //0����ͨ��;3Զ��ͨ��
            }
            else
            {
                if(gs_FarpVar.wmst & WMS_STT_GSM)     //�������ź�
                {
                    if(guc_TaskLinkWaitSec > 0)     //���ڵȴ���������
                    {
                        guc_TaskLinkWaitSec--;
                        return false;
                    }
                    if((gss_TaskATimer.flag & gss_TaskATimer.runflag) == gss_TaskATimer.runflag)
                    {
                        gs_OS.Message_Send(&gs_TKFarpQueue, &msgidA[MSG_FARP_PRIOCNT], 1);
                        guc_TaskLinkWaitSec = 60;
                        return false;
                    }
                }

                return true;
            }
            frame->recv = para;                  //�������
            frame->send = gucs_PstSbuf;          //���ͻ�����
            frame->cmd = 0x05;          //seq��(��֡,����ȷ��)
            frame->atr = GetMeterNum();
            frame->atr <<= 4;
            frame->atr |= 0x06;
            frame->seq = 0;
            ByteArrayHexToBcd(gss_TaskATimer.time.YMDhms, time, 6);
            memcpy(frame->recv, time,6);
            frame->len = 6;

            frame->wd.lword = 0xA621;
            frame->send += 21;
            Task_Execute(&frm, lasttime, time, true);

            
            frame->wd.lword = 0xA622;
            frame->send += 21;
            Task_Execute(&frm, lasttime, time, true);

            gss_TaskATimer.atr &= ~TKT_ATR_FLAG;

            if(gs_SysVar.mDGstt & HLV_STT_NENG)
            {
                if((gs_SysVar.mLPstt & (HLV_LPTASK_CP | HLV_LPTASK_RP))== 0)
                {
                    gs_SysVar.mLPstt |= HLV_LPTASK_CLSCK;
                    gs_OS.Message_Send(&gs_TKFarpQueue, &msgidA[MSG_ENTER_LP], 1);
                }
            }
            gs_SysVar.mLPstt &= ~HLV_LPTASK_RP;
        }
    }

    return true;

}






/************************************************************************
**��������: void Task_PreInit(void* pdata)
**�� ��: ������̳�ʼ��
**
**�� ��: 
**�� ��: 
**
**�� ע: 
**-----------------------------------------------------------------------
**�� ��: yzy
**�� ��: 09-09-09
*************************************************************************/
void Task_PreInit(void)
{
    guc_TaskLinkWaitSec = 0;
    guc_TaskMinFlag = 0;
                                        //��ʼ�������ϱ�״̬��
    ClearBuffer(bmap_AckRecv, sizeof(bmap_AckRecv));
                                        //�������
    LoadSystemParam(PARA_TYPE_TASK);
    
//	    ClearBuffer((uint8*)&gss_TaskATimer, sizeof(gss_TaskATimer));
//	    Task_SetTimerCfg(true);             //������������ִ�б�־
//	    Task_Clear();
    ClearBuffer((uint8*)&gss_TaskATimer, sizeof(gss_TaskATimer));
}

void Task_Clear()
{
    gss_TaskATimer.atr = 0;
    gss_TaskATimer.flag = 0;
//	    ClearBuffer((uint8*)&gss_TaskATimer, sizeof(gss_TaskATimer));
}

/************************************************************************
**��������: void Task_SecProc(void* pdata)
**�� ��: �봦��
**
**�� ��: 
**�� ��: 
**
**�� ע: 
**-----------------------------------------------------------------------
**�� ��: yzy
**�� ��: 09-09-09
*************************************************************************/
void Task_SecProc(void)
{
    //REP_Proc();                       //����վȷ��֡���ϱ�
    //if(guc_TaskMinFlag == 1)
    {
        //if(GetTime()->sec > 10)         //Ϊ�˱����0���ϱ�ʱ2������δ����
        {
            if(Task_Proc() == true)
            {
                guc_TaskMinFlag = 0;
            }
        }
    }
}


/************************************************************************
**��������: void Task_MinProc(void* pdata)
**�� ��: �ִ���
**
**�� ��: 
**�� ��: 
**
**�� ע: 
**-----------------------------------------------------------------------
**�� ��: yzy
**�� ��: 09-09-09
*************************************************************************/
void Task_MinProc(void)
{
    guc_TaskMinFlag = 1;
}



