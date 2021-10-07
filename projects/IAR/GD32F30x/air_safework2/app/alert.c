/****************************************Copyright (c)**************************************************
**                                  ____�Ƽ����޹�˾
**                                          ������
**
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: GD_alert.c
**��   ��   ��: yzy
**����޸�����: 2019-06-18
**��        ��: �澯ģ�����
**ע        ��:
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: yzy
** ��  ��: v1.0
** �ա���: 2019-06-18
** �衡��: ԭʼ�汾
********************************************************************************************************/
#define EXT_GD_ALERT


#include "public.h"
#include "extapi.h"
#include "task.h"
#include "evt.h"
#include "hlv.h"
#include "sal_import.h"
#include "atmodem.h"
//	uint8 EvtTmpBuf[200];                   //д��FLS�¼�����(��OSLOCKʹ��)

#define CON_EVENT_RECORD_MAX_LEN  0x80

#define ERR_TYPE_485        2
#define ERR_TYPE_CP         6
#define ERR_TYPE_LOSTVOL    5

#define ERR_CODE_485_NORMAL     0
#define ERR_CODE_485_INVALID    1
#define ERR_CODE_485_NORSP      2
#define ERR_CODE_485_CHECK      3
#define ERR_CODE_485_CMD        4
#define ERR_CODE_485_DATA       5

/************************************************************************
**��������: uint8 ALRT_WriteLog(uint8 evtno, uint8* pt_ex, uint8 len_ex)
**�� ��: ��flash��д���¼���¼
**
**�� ��: evtno д����¼���(1-64)
         pt_ex д������ָ��
         len_ex д�����ݵĳ���(<=200)

**�� ��: true �ɹ� 
         false ʧ��(�����¼�����������)

**�� ע: ��������̱���
**--------------------------------------------------------
**�� ��: yzy
**�� ��: 
*************************************************************************/
uint8 ALRT_WriteLog(uint8 evtno, uint8* pt_ex, uint8 len_ex)
{
    TDataBlock db;
    uint32 addr;                        //flash���ݿ���ƫ�Ƶ�ַ
    uint8* Pt_EvtCnt;
    uint8 erc = evtno - 1;
    int m = 0;
    uint8_t tmp[6];
    
                                        //�ж��Ƿ��Ѿ��������
    uint8_t EvtTmpBuf[CON_EVENT_RECORD_MAX_LEN]; 
    memset(EvtTmpBuf,0,CON_EVENT_RECORD_MAX_LEN);
    
    if(!(gl_SysRunStt & HRS_PARALD))
    {
        return false;
    }
                                        //�ж��¼����Ƿ���Ч
    if(erc > 63)
    {
        return false;
    }
                                        //�ٴ��ж��Ƿ���Ҫ��¼
//	    if(!STR_GetBits(gs_InpVarData.evtbit, 1, erc))
//	    {
//	        return false;
//	    }
//	    
    //����̵���.���õ�ȫ���¼�ָ��
    CPSR_ALLOC();
    RHINO_CPU_INTRPT_DISABLE();    
                                        
    if(gs_FramMisData.e1cnt == 255)
    {
        gs_FramMisData.e1cnt = 0;
    }

    addr = CON_EVENT_RECORD_MAX_LEN * gs_FramMisData.e1cnt;
    Pt_EvtCnt = &gs_FramMisData.e1cnt;
    
                                        //ɢת׼���¼�����
    MoveBuffer(pt_ex, EvtTmpBuf+8, len_ex);

    RHINO_CPU_INTRPT_ENABLE();                   //���̽���

    EvtTmpBuf[m++] = len_ex+7;
    gs_RealTime.ReadBCDDateTime((TIME*)tmp);
    MoveBuffer(tmp, EvtTmpBuf+m, 6);  
    InvBuffer(EvtTmpBuf+m, 6);
    m+=6;
    EvtTmpBuf[m++] = evtno;    
//	    EvtTmpBuf[1] = evtno;

    
    BuildBufferCRC(EvtTmpBuf, CON_EVENT_RECORD_MAX_LEN, true);
    
                                        //д��flash
//	    ES_FILE_DB_Open(DB_EVT, &db, TDB_MODE_RW);
//	    ES_FILE_DB_WriteFrom(&db, EvtTmpBuf, 0xC0, addr);
//	    ES_FILE_DB_Close(&db);
    CBB_FILE_Write_Event(DB_FLASH, EvtTmpBuf, CON_EVENT_RECORD_MAX_LEN);
    (*Pt_EvtCnt)++;                     //�¼���������һ

//	    SaveFramMisData(MIS_E1C);           //������º��¼�ָ�뵽����
//	    SaveFramMisData(MIS_E2C);
    return true;

}







/************************************************************************
**��������: uint8 ALRT_ReadLog(uint8* pm, uint8* pn, uint8* data, uint8 type)
**�� ��: ��ȡ�澯����
**
**�� ��: pm ��ʼָ�� (0-255)
         pn ����ָ�� (0-255)
         data ���ݴ�Ż��� 
         type 0 ��Ҫ�¼�  1 һ���¼�
**�� ��: ��ȡ���ݵĳ���,������ݳ��Ȳ�����200�ֽ�
**
**�� ע: 
**--------------------------------------------------------
**�� ��: yzy
**�� ��: 08-11-26
*************************************************************************/
int ALRT_ReadLog(uint8* pm, uint8* pn, uint8* data, uint8 type)
{
    uint32 addr;
    uint8 tmp[200];                     //����
    uint16 len = 0;
    int buflen = 200;
    TDataBlock db;
    uint8_t exnum = 0;

    //SYS_FILE_DB_Open(DB_EVENT, &db, TDB_MODE_RW);

    for(; (*pm) != (*pn); (*pm)++)      //��pm = 0,pn = 1���ȡ������¼. pm,pn�������
    {                                   //����fls��ַ
//	        addr = 0x80 * (*pm);
//	        addr += (type ? 0xC000 : 0);
                                        //��ȡ��У��
//	        SYS_FILE_DB_ReadFrom(&db, tmp, CON_EVENT_RECORD_MAX_LEN, addr);
        buflen = 200;
        memset(tmp,0,200);
        if(SYS_ERR_OK != CBB_FILE_Read_Event(DB_FLASH, *pm, 1, tmp, &buflen))
        {
            break;
        }
//	        CBB_FILE_Read_Event(DB_FLASH, *pm, 1, tmp, 128);

        if(CheckBufferCRC(tmp, CON_EVENT_RECORD_MAX_LEN, true) == false)
        {
            break;
        }
                                        //������Ч�¼�����
        _IF_TRUE_DO(tmp[0] > 0x80, continue;)
        _IF_TRUE_DO(len + (tmp[0] + 2) > 480, break;)
//	        data[0] = tmp[0];
//	        len++;
        MoveBuffer(tmp+1, data + len, tmp[0]);
        len += tmp[0];
        exnum++;
#ifndef DEBUG        
        extern IWDG_HandleTypeDef hiwdg;
        HAL_IWDG_Refresh(&hiwdg); //ι��
#endif        
    }

//	    SYS_FILE_DB_Close(&db);
    * pn = exnum;

    return len;
}









/************************************************************************
**��������: void ALRT_SendData(uint8 type)
**�� ��: ��������δ�ϱ��澯��¼
**
**�� ��: type 0 ��Ҫ�¼� 1 һ���¼�
**�� ��: 
**
**�� ע: 
**-----------------------------------------------------------------------
**�� ��: yzy
**�� ��: 09-10-13
*************************************************************************/
void ALRT_SendData(uint8 type)
{
    TDataBlock db;
    PST_Frame pstFrame;
    ZY_Frame* frame = &pstFrame._zy;
    uint32 addr;
    uint8 data[256];
    uint8 tp[10];
    uint8 time[6];                      //��ǰʱ��(���ʱ������)

    uint8 len;
    uint8 ept;
                                        //����fls��ַ
//	    if(type == 0)
    {
        ept = gs_FramMisData.e1pt;
        addr = CON_EVENT_RECORD_MAX_LEN * ept;
    }

                                        //��ȡ��У��
    SYS_FILE_DB_Open(DB_EVENT, &db, TDB_MODE_RW);
    SYS_FILE_DB_ReadFrom(&db, data, CON_EVENT_RECORD_MAX_LEN, addr);
    SYS_FILE_DB_Close(&db);

    if(CheckBufferCRC(data, CON_EVENT_RECORD_MAX_LEN, true) == false)
    {
        return;
    }
    len = data[0];            //�¼����ݳ���
    MoveBuffer(data+1, data + 21, len);   //�Ƶ�֡����Ӧλ��

    frame->cmd = 0x05;          //seq��(��֡,����ȷ��)
    frame->send = data;          //���ͻ�����
    frame->len = len;
    
    frame->atr = GetMeterNum();
    frame->atr <<= 4;
    frame->atr |= 0x06;
    frame->seq = 0;
    ByteArrayHexToBcd(gss_TaskATimer.time.YMDhms, time, 6);
    
    frame->wd.lword = 0xA624;
    
    REP_CFG repcfg;
    repcfg.pseq = 2;
    repcfg.en = 0;
//	    frame->send += 21;
//	    data[14] = 0;                       //pn
//	    data[15] = 0;
//	    data[16] = type ? 0x02 : 0x01;      //Fn
//	    data[17] = 0;
//	                                        //��������
//	    data[18] = gs_FramMisData.e1cnt;
//	    data[19] = gs_FramMisData.e2cnt;
//	    data[20] = ept;
//	    data[21] = ept + 1;
//	    
//	    frame->atr = 0;                      //����tp
//	    frame->send = data;                  //���ͻ���
//	    frame->tbd = (CMD_DIR | CMD_PRM | 0x04);//������(����|����վ|������4:�û�����)
//	    frame->cmd = 0x0E;                   //afn
//	    frame->len = len;                    //���ݳ���
//	    frame->send[11] = 0;                 //��վ��ַ
//	    frame->tstno = SEQ_FIR_FIN;          //seq(��֡)
//	    frame->tstno |= guc_pseq & 0x0F;     //seq(֡���)
                                        //����
    /*
    if(gs_PstPara.AckServ & 0x04)
    {
        frame.tstno |= 0x10;            //seq(Ҫ����վȷ��)
        len = GD_Pack(frame);
        REP_Enqueue(frame.send, len);
    }
    else
    {
        len = GD_Pack(frame);
        gfs_PSTChnSend[PST_CHN_TCP](frame.send, len);
    }
    */
    //frm->atr |= EXIST_TP;               //��TP(EXIST_TP)
//	    tp[0] = guc_pseq;
//	    gs_RealTime.ReadBCDDateTime((TIME*)(tp + 1));
//	    tp[5] = 0;
//	    frame->wd.lword = (uint32)tp;
//	    len = GD_Pack(frame);              //���
    if(PST_ERR_OK == PST_ZY_Pack(&pstFrame,SYS_ERR_OK,&len))
    {
//	        if(gs_FarpVar.wmst & WMS_STT_CLN )
        {
            gfs_PSTChnSend[PST_CHN_TCP](frame->send, len);
        }
    }
//	    else
//	    {
//	        Farp_SendSMS(frame->send, len, 4, 1);
//	    }
                                        //��������������
    guc_pseq++;
}


uint8 guc_AlrtLinkWaitSec = 120;              //���¼��ϱ�ʱ,��������״̬����ʱ

extern kbuf_queue_t gs_TKFarpQueue;
extern intptr_t g_ulTcp_fd;
extern int32_t HAL_TCP_Destroy(intptr_t fd);
/************************************************************************
 * @function: ALRT_ActiveReport
 * @����: �澯��¼�������ϱ�
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/2/11)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void ALRT_ActiveReport(void)
{
  
                                        //�Ƿ����������ϱ�
    /*if(!(gs_FramMisData.enable & 0x01))
    {
        return;
    }*/


                                        //�Ƿ����¼��ϱ�
    if(gs_FramMisData.e1pt != gs_FramMisData.e1cnt)
    {
        if(gs_SysVar.mDGstt & HLV_STT_NENG)
        {
            if((gs_SysVar.mLPstt & HLV_LPTASK_CLSCK) == HLV_LPTASK_CLSCK)
            {
                return ;
            }
            if((gs_SysVar.mLPstt & HLV_LPTASK_LOSTVOL) != HLV_LPTASK_LOSTVOL)
            {
                return ;
            }
            g_ulTcp_fd = Farp_WM_Connect_Once();
            if(g_ulTcp_fd >= 0)
            {
//	                frm.chninfo = PST_CHN_TCP;        //0����ͨ��;3Զ��ͨ��
            }
            else
            {   SYS_MODM_Shutdown();
                gs_SysVar.mLPstt &= ~HLV_LPTASK_LOSTVOL; 
                //LOG_INFO("Farp_WM_Connect_Once failed modem shut down\n");    
                return ;
            }

            for(;gs_FramMisData.e1pt != gs_FramMisData.e1cnt;)
            {
                ALRT_SendData(0);
                gs_FramMisData.e1pt++;
            }
            
            if(gs_SysVar.mDGstt & HLV_STT_NENG)
            {
                if(g_ulTcp_fd >= 0)
                {
                    HAL_TCP_Destroy(g_ulTcp_fd);
                    g_ulTcp_fd = -1;
                    //Farp_Close_PPP();
                    //SYS_MODM_Shutdown();
                    LOG_INFO("report warn finish modem close socket\n");    
                }
            }
            gs_SysVar.mLPstt &= ~HLV_LPTASK_LOSTVOL;

        }
        else
        {
                                            //�Ƿ�������
            if(!(gs_FarpVar.wmst & (WMS_STT_CLN | WMS_ETH0STT)))
            {
                
                if(gs_FarpVar.wmst & WMS_STT_GSM)     //�������ź�
                {
                    if(guc_AlrtLinkWaitSec > 0)     //���ڵȴ���������
                    {
                        guc_AlrtLinkWaitSec--;
                        return ;
                    }
                                                    //�ȴ���ʱ���ٴ���������
                    if(gs_FramMisData.e1pt != gs_FramMisData.e1cnt)  
                    {
                        gs_OS.Message_Send(&gs_TKFarpQueue, &msgidA[MSG_FARP_PRIOCNT], 1);
                        guc_AlrtLinkWaitSec = 60;
                        return;
                    }
                }
            
                return;
            }

        
//	            ALRT_SendData(0);
//	            gs_FramMisData.e1pt++;
            for(;gs_FramMisData.e1pt != gs_FramMisData.e1cnt;)
            {
                ALRT_SendData(0);
                gs_FramMisData.e1pt++;
                HB_RetLive(TASK_SLV_TKID);
            }

            gs_SysVar.mLPstt &= ~HLV_LPTASK_LOSTVOL;            

            
            if(gs_SysVar.mDGstt & HLV_STT_NENG)
            {
                if((gs_SysVar.mLPstt & (HLV_LPTASK_CP | HLV_LPTASK_RP | HLV_LPTASK_LOSTVOL))== 0)
                {
                    gs_SysVar.mLPstt |= HLV_LPTASK_CLSCK;
                    gs_OS.Message_Send(&gs_TKFarpQueue, &msgidA[MSG_ENTER_LP], 1);
                }
            }
        }
//	        SaveFramMisData(MIS_E1P);
    }
//	    else if(gs_FramMisData.e2pt != gs_FramMisData.e2cnt)
//	    {
//	        ALRT_SendData(1);
//	        gs_FramMisData.e2pt++;
//	        SaveFramMisData(MIS_E2P);
//	    }
}












/************************************************************************
 * @function: ALRT_ERC1
 * @����: ���ݳ�ʼ���Ͱ汾�����¼
 * 
 * @����: 
 * @param: pdata MSG_ALRT_ERC1 ���������ݳ�ʼ��  /NULL �汾����ж�
 * @����: 
 * @˵��: 1.�汾�仯(״̬����,ÿ�����ж�)  2.��������������ʼ��(��Ϣ����)
 * @����: yzy (2014/2/9)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void ALRT_ERC1(uint8 pdata)
{
#if ERC1_EN == 1
//	    uint8 dat[32];
//	    if(pdata == MSG_ALRT_ERC1)
//	    {
//	        gs_RealTime.ReadBCDDateTime((TIME*)dat);                   //��¼������ʼ���¼�
//	        MoveBuffer(dat + 1, dat, 5);
//	        dat[5] = 0x01;
//	        ByteArrayBcdToHexString((uint8*)&gul_UsrFuncVer, &dat[6], 2, false);
//	        ByteArrayBcdToHexString((uint8*)&gul_UsrFuncVer, &dat[10], 2, false);
//	        ALRT_WriteLog(1, dat, 14);
//	    }
//	    else
//	    {
//	        if(gul_UsrFuncVer != gs_FramMisData.ver)
//	        {
//	            gs_RealTime.ReadBCDDateTime((TIME*)dat);               //��¼�汾�仯�¼�
//	            MoveBuffer(dat + 1, dat, 5);
//	            dat[5] = 0x02;
//	            ByteArrayBcdToHexString((uint8*)&gs_FramMisData.ver, &dat[6], 2, false);
//	            ByteArrayBcdToHexString((uint8*)&gul_UsrFuncVer, &dat[10], 2, false);
//	            ALRT_WriteLog(1, dat, 14);
//	                                                //���±��ݰ汾�ŵ�fram
//	            gs_FramMisData.ver = gul_UsrFuncVer;
//	            SaveFramMisData(MIS_VER);
//	            ERC44_Collctor_Updata();
//	        }
//	    }
#endif
}



/************************************************************************
**��������: void ALRT_ERC4(void)
**�� ��: ״̬����λ��¼
**
**�� ��: 
**�� ��: 
**
**�� ע: ��Ҫÿ���Ӵ���
**--------------------------------------------------------
**�� ��: yzy
**�� ��: 2009-05-31
*************************************************************************/
//	void ALRT_ERC4(void)
//	{
//	#if ERC4_EN == 1
//	    uint8 chg;                          //��λ��־,BS2
//	    uint8 dat[32];
//	
//	    chg = gs_MKStruct.mFall;
//	    chg |= gs_MKStruct.mRaise;
//	    gs_MKStruct.mFall = 0;              //���������־
//	    gs_MKStruct.mRaise = 0;             //���㷢����־
//	
//	    if(chg)
//	    {
//	        gs_RealTime.ReadBCDDateTime((TIME*)dat);
//	        MoveBuffer(dat + 1, dat, 5);//mmHHDDMMYY
//	        dat[5] = chg;                   //��λ��־
//	        dat[6] = gs_MKStruct.mState;    //��λ��״̬
//	                                        //д���¼�
//	        ALRT_WriteLog(4, dat, 7);
//	    }
//	#endif
//	}




/************************************************************************
 * @function: ALRT_ParaChg
 * @����: ERC3���������¼ 
 * 
 * @����: 
 * @param: host ��վ��ַ
 * @param: pdi �������DI���ָ��
 * @param: di_no DI����(���20,����20���¼ǰ20��)
 * @����: 
 * @˵��: ���ⲿ֡������̵���
 * @����: yzy (2014/2/9)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void ALRT_ParaChg(uint8 host, uint8* pdi, uint8 di_no)
{
#if ERC3_EN == 1
    uint8 dat[90];
    gs_RealTime.ReadBCDDateTime((TIME*)dat);
    MoveBuffer(dat + 1, dat, 5);
                                                //��վ��ַ
    dat[5] = host;
                                                //��ʶ
    di_no = (di_no > 20) ? 20 : di_no;
    MoveBuffer(pdi, &dat[6], di_no << 2);
    ALRT_WriteLog(3, dat, 6 + (di_no << 2));
#endif
}



/************************************************************************
 * @function: ALRT_ERC8
 * @����: ���ܱ�������
 * 
 * @����: 
 * @param: tstno 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/2/9)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	void ALRT_ERC8(uint8 tstno)
//	{
//	#if ERC8_EN == 1
//	    uint8 bit_chg;
//	    uint8 dat[32];
//	
//	    _IF_TRUE_DO(gss_CDATATst[tstno].ef != 0x01, return);
//	
//	    bit_chg = 0;                        //�仯��־����
//	    dat[7] = 0;
//	                                        //����ʱ�� bit0
//	    if(gss_ALRTStt[tstno].ewr1.gsd !=  0)
//	    {
//	        dat[7] |= 0x01;
//	        bit_chg = 1;
//	        gss_ALRTStt[tstno].ewr1.gsd = 0;
//	    }
//	                                        //���ʱ�� bit1
//	    if(gss_ALRTStt[tstno].ewr1.gsj != 0)
//	    {
//	        dat[7] |= 0x02;
//	        bit_chg = 1;
//	        gss_ALRTStt[tstno].ewr1.gsj = 0;
//	    }
//	                                        //������ bit2
//	    if(gss_ALRTStt[tstno].ewr1.cbr != 0)
//	    {
//	        dat[7] |= 0x04;
//	        bit_chg = 1;
//	        gss_ALRTStt[tstno].ewr1.cbr = 0;
//	    }
//	                                        //���峣�� bit3
//	    if(gss_ALRTStt[tstno].ewr1.mcc != 0)
//	    {
//	        dat[7] |= 0x08;
//	        bit_chg = 1;
//	        gss_ALRTStt[tstno].ewr1.mcc = 0;
//	    }
//	                                        //���������� bit4
//	    if(gss_ALRTStt[tstno].ewr1.hgq != 0)
//	    {
//	        dat[7] |= 0x10;
//	        bit_chg = 1;
//	        gss_ALRTStt[tstno].ewr1.hgq = 0;
//	    }
//	                                        //��������ʱ�� bit5
//	    if(gss_ALRTStt[tstno].ewr1.clr != 0)
//	    {
//	        dat[7] |= 0x20;
//	        bit_chg = 1;
//	        gss_ALRTStt[tstno].ewr1.clr = 0;
//	    }
//	
//	    if(bit_chg == 1)
//	    {
//	        gs_RealTime.ReadBCDDateTime((TIME*)dat);
//	        MoveBuffer(dat + 1, dat, 5);
//	        IntToByte(dat + 5, tstno);
//	        ALRT_WriteLog(8, dat, 8);
//	    }
//	#endif
//	}




/************************************************************************
 * @function: ALRT_ERC12
 * @����: ���ܱ�ʱ�䳬��
 * 
 * @����: 
 * @param: tstno 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/2/11)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	void ALRT_ERC12(uint8 tstno)
//	{
//	#if ERC12_EN == 1
//	    uint8 dat[32];
//	    _IF_TRUE_DO(gss_CDATATst[tstno].ef != 0x01, return);
//	                                            //���״γ���
//	    if(gss_ALRTStt[tstno].ewr1.tim == 1)
//	    {
//	        gss_ALRTStt[tstno].ewr1.tim = 0;
//	
//	        gs_RealTime.ReadBCDDateTime((TIME*)dat);
//	        MoveBuffer(dat + 1, dat, 5);
//	        IntToByte(dat + 5, (0x8000 | tstno));
//	        ALRT_WriteLog(12, dat, 7);
//	    }
//	                                            //���״�ֹͣ����
//	    if(gss_ALRTStt[tstno].ewf1.tim == 1)
//	    {
//	        gss_ALRTStt[tstno].ewf1.tim = 0;
//	
//	        gs_RealTime.ReadBCDDateTime((TIME*)dat);
//	        MoveBuffer(dat + 1, dat, 5);
//	        IntToByte(dat + 5, tstno);
//	        ALRT_WriteLog(12, dat, 7);
//	    }
//	#endif
//	}
//	



/************************************************************************
 * @function: ALRT_ERC13
 * @����: ��������Ϣ
 * 
 * @����: 
 * @param: tstno 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/2/10)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	void ALRT_ERC13(uint8 tstno)
//	{
//	#if ERC13_EN == 1
//	    uint8 dat[32];
//	    uint8 iserr = 0;
//	    dat[7] = 0;
//	                                            //��̴���/������������仯
//	    if(gss_ALRTStt[tstno].ewr1.bcb || gss_ALRTStt[tstno].ewr1.xlb)
//	    {
//	        gss_ALRTStt[tstno].ewr1.bcb = 0;
//	        gss_ALRTStt[tstno].ewr1.xlb = 0;
//	        dat[7] |= 0x01;
//	        iserr = 1;
//	    }
//	                                            //��������仯
//	    if(gss_ALRTStt[tstno].ewr1.dxb)
//	    {
//	        gss_ALRTStt[tstno].ewr1.dxb = 0;
//	        dat[7] |= 0x02;
//	        iserr = 1;
//	    }
//	                                            //ʧѹ�����仯
//	    if(gss_ALRTStt[tstno].ewr1.syb)
//	    {
//	        gss_ALRTStt[tstno].ewr1.syb = 0;
//	        dat[7] |= 0x04;
//	        iserr = 1;
//	    }
//	                                            //ͣ������仯
//	    if(gss_ALRTStt[tstno].ewr1.tdb)
//	    {
//	        gss_ALRTStt[tstno].ewr1.tdb = 0;
//	        dat[7] |= 0x08;
//	        iserr = 1;
//	    }
//	                                            //���Ƿѹ
//	    if(gss_ALRTStt[tstno].ewr1.dcd)
//	    {
//	        gss_ALRTStt[tstno].ewr1.dcd = 0;
//	        dat[7] |= 0x10;
//	        iserr = 1;
//	    }
//	
//	    if(iserr)
//	    {
//	        gs_RealTime.ReadBCDDateTime((TIME*)dat);
//	        MoveBuffer(dat + 1, dat, 5);
//	        IntToByte(dat + 5, tstno);
//	        ALRT_WriteLog(13, dat, 8);
//	    }
//	#endif
//	}



/************************************************************************
 * @function: ALRT_ERC14
 * @����: �ϵ��¼���¼
 * 
 * @����: 
 * @param: type 0�ϵ�. 1ͣ��
 * @����: 
 * @˵��: 
 * @����: yzy (2014/2/10)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void ALRT_ERC14(uint8 type)
{
#if ERC14_EN == 1
    uint8 buffer[32];
    uint8 tn[10];
    int m = 0;
//	    gs_RealTime.ReadBCDDateTime((TIME*)tn);
    
    if(type == 0)                       //�ϵ��¼�
    {
//	        MoveBuffer(gs_FramMisData.tdt, buffer, 5);
//	        InvBuffer(buffer, 5);
//	        ByteArrayHexToBcd(buffer, buffer, 5);
//	        MoveBuffer((uint8*)&GetTime()->min, buffer + 5, 5);
//	        ByteArrayHexToBcd(buffer + 5, buffer + 5, 5);
        buffer[m++] = 0x00;

        buffer[m++] = SYS_AD_GetValue(AD_ID_CHECKVOL) / 100;  
        
    }
    else if(type == 1)                       //ͣ���¼�
    {
//	        MoveBuffer(gs_FramMisData.tbk, buffer, 5);
//	        InvBuffer(buffer, 5);
//	        ByteArrayHexToBcd(buffer, buffer, 5);
//	        memset(buffer + 5, 0xEE, 5);
        buffer[m++] = 0x01;
        buffer[m++] = SYS_AD_GetValue(AD_ID_CHECKVOL) / 100;        
    }
    else
    {
        return ;
    }
    
    memcpy(buffer + m, (uint8_t *)&gs_SysVar.terstt.lword, 4);
    m+=4;
    ALRT_WriteLog(ERR_TYPE_LOSTVOL, buffer, m);
#endif

}




/************************************************************************
 * @function: ALRT_ERC21
 * @����: �ն˹��ϼ�¼
 * 
 * @����: 
 * @param: port ����˿ں�
 * @����: 
 * @˵��: ֧����:��·485�������.
 *        ����485��(������)������ʱ��Ӧ��¼
 * @����: yzy (2014/2/10)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	void ALRT_ERC21(uint8 port)
//	{
//	#if ERC21_EN == 1
//	    uint8 dat[10];
//	    uint8 k;
//	                                    
//	    gs_RealTime.ReadBCDDateTime((TIME*)dat);//ssmmHHDDMMYY
//	    MoveBuffer(dat + 1, dat, 5);
//	    dat[5] = 4;
//	    ALRT_WriteLog(21, dat, 6);
//	                                    //�����·485�����в�����ı�־
//	    for(k = 0; k < CORE_TST_NO; k++)
//	    {
//	        if((gss_CDATATst[k].tf == 1) && (gss_CDATATst[k].pt == port))
//	        {
//	            gss_ALRTStt[k].ewr1.trn = 0;
//	            gss_ALRTStt[k].ewf1.trn = 0;
//	        }
//	    }
//	#endif
//	}



/************************************************************************
 * @function: ALRT_ERC27_28_29_30
 * @����: �������쳣(ʾ���½�,����,����,ͣ��)
 * 
 * @����: 
 * @param: nWp 
 * @param: tstno 
 * @����: 
 * @˵��: 1.�������ָ��¼�. 2.�������ʾֵ�� ���ܵ��� ���¼�����
 * @����: yzy (2014/2/10)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	void ALRT_ERC27_28_29_30(uint32 nWp, uint8 tstno)
//	{
//	    _IF_TRUE_DO(gss_CDATATst[tstno].ef != 0x01, return);
//	
//	#if ERC27_EN == 1
//	    uint8 dat[24];
//	    
//	    if(gss_ALRTStt[tstno].ewr1.szj)//ERC27ʾ���½�����
//	    {
//	        gss_ALRTStt[tstno].ewr1.szj = 0;
//	        gs_RealTime.ReadBCDDateTime((TIME*)dat);
//	        MoveBuffer(dat + 1, dat, 5);
//	
//	        IntToByte(dat + 5, (0x8000 | tstno));
//	        Hex42ToGD14(gss_TstRealData[tstno].Wp[0], dat + 7);
//	        Hex42ToGD14(nWp, dat + 12);
//	
//	        ALRT_WriteLog(27, dat, 17);
//	    }
//	
//	    if(gss_ALRTStt[tstno].ewf1.szj)//ERC27ʾ���½��ָ�
//	    {
//	        gss_ALRTStt[tstno].ewf1.szj = 0;
//	        /*
//	        gs_RealTime.ReadBCDDateTime((TIME*)dat);
//	        MoveBuffer(dat + 1, dat, 5);
//	
//	        IntToByte(dat + 5, tstno);
//	        Hex42ToGD14(gss_TstRealData[tstno].Wp[0], dat + 7);
//	        Hex42ToGD14(nWp, dat + 12);
//	
//	        ALRT_WriteLog(27, dat, 17);
//	        */
//	    }
//	#endif
//	
//	#if ERC28_EN == 1
//	    if(gss_ALRTStt[tstno].ewr1.fast)//ERC28�����
//	    {
//	        gss_ALRTStt[tstno].ewr1.fast = 0;
//	        gs_RealTime.ReadBCDDateTime((TIME*)dat);
//	        MoveBuffer(dat + 1, dat, 5);
//	
//	        IntToByte(dat + 5, (0x8000 | tstno));
//	        Hex42ToGD14(gss_TstRealData[tstno].Wp[0] , dat + 7);
//	        Hex42ToGD14(nWp , dat + 12);
//	        dat[17] = ByteHexToBcd(gs_EVTComVar.wfast);
//	
//	        ALRT_WriteLog(28, dat, 18);
//	    }
//	
//	    if(gss_ALRTStt[tstno].ewf1.fast)//ERC28����ָ�
//	    {
//	        gss_ALRTStt[tstno].ewf1.fast = 0;
//	        /*
//	        gs_RealTime.ReadBCDDateTime((TIME*)dat);
//	        MoveBuffer(dat + 1, dat, 5);
//	
//	        IntToByte(dat + 5, tstno);
//	        Hex42ToGD14(gss_TstRealData[tstno].Wp[0] , dat + 7);
//	        Hex42ToGD14(nWp , dat + 12);
//	        dat[17] = ByteHexToBcd(gs_EVTComVar.wfast);
//	
//	        ALRT_WriteLog(28, dat, 18);
//	        */
//	    }
//	#endif
//	
//	#if ERC29_EN == 1
//	    if(gss_ALRTStt[tstno].ewr1.fly)//ERC29���߷���
//	    {
//	        gss_ALRTStt[tstno].ewr1.fly = 0;
//	        gs_RealTime.ReadBCDDateTime((TIME*)dat);
//	        MoveBuffer(dat + 1, dat, 5);
//	
//	        IntToByte(dat + 5, (0x8000 | tstno));
//	        Hex42ToGD14(gss_TstRealData[tstno].Wp[0] , dat + 7);
//	        Hex42ToGD14(nWp, dat + 12);
//	        dat[17] = ByteHexToBcd(gs_EVTComVar.wfly);
//	
//	        ALRT_WriteLog(29, dat, 18);
//	    }
//	
//	    if(gss_ALRTStt[tstno].ewf1.fly)//ERC29���߻ָ�
//	    {
//	        gss_ALRTStt[tstno].ewf1.fly = 0;
//	        /*
//	        gs_RealTime.ReadBCDDateTime((TIME*)dat);
//	        MoveBuffer(dat + 1, dat, 5);
//	
//	        IntToByte(dat + 5, tstno);
//	        Hex42ToGD14(gss_TstRealData[tstno].Wp[0] , dat + 7);
//	        Hex42ToGD14(nWp, dat + 12);
//	        dat[17] = ByteHexToBcd(gs_EVTComVar.wfly);
//	
//	        ALRT_WriteLog(29, dat, 18);
//	        */
//	    }
//	#endif
//	
//	#if ERC30_EN == 1
//	    if(gss_ALRTStt[tstno].ewr1.bjt)//ERC30 ͣ�߷���
//	    {
//	        gss_ALRTStt[tstno].ewr1.bjt = 0;
//	
//	        gs_RealTime.ReadBCDDateTime((TIME*)dat);
//	        MoveBuffer(dat + 1, dat, 5);
//	
//	        IntToByte(dat + 5, (0x8000 | tstno));
//	        Hex42ToGD14(nWp, dat + 7);
//	        dat[12] = gs_EVTComVar.stop;
//	
//	        ALRT_WriteLog(30, dat, 13);
//	    }
//	
//	    if(gss_ALRTStt[tstno].ewf1.bjt)//ERC30 ͣ�߻ָ�
//	    {
//	        gss_ALRTStt[tstno].ewf1.bjt = 0;
//	        /*
//	        gs_RealTime.ReadBCDDateTime((TIME*)dat);
//	        MoveBuffer(dat + 1, dat, 5);
//	
//	        IntToByte(dat + 5, tstno);
//	        Hex42ToGD14(nWp, dat + 7);
//	        dat[12] = gs_EVTComVar.stop;
//	
//	        ALRT_WriteLog(30, dat, 13);
//	        */
//	    }
//	#endif
//	}

//	0x00��ͨ�Żָ�����
//	0x01����Ч��ָ��
//	0x02���豸�޻�Ӧ
//	0x03������֡У��ʧ��
//	0x04��ָ��ش���
//	0x05�������쳣(ѹ��<=0��������Ϊ����)


/************************************************************************
 * @function: ALRT_ERC31
 * @����: 485����ʧ���¼�
 * 
 * @����: 
 * @param: tstno 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/2/10)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void ALRT_ERC31(uint8 tstno, uint8_t type)
{
#if ERC31_EN == 1
    uint8 dat[77];
    int m = 0;
    memset(dat,0,70);
//	    void ALRT_ERC31_COMMONIN(uint8* dat, uint8 tstno);

    if(gs_SysVar.mDGstt & HLV_STT_NENG || tstno >= CORE_TST_NO)
    {
        return;
    }

#if 0  

    if(gss_ALRTStt[tstno].ewr1.trn & 0x01)
    {
        gss_ALRTStt[tstno].ewr1.trn = 0;
      
                                            //��¼�¼�
        dat[m++] = ERR_CODE_485_NORSP;
        dat[m++] = tstno+1;
        dat[m++] = gss_CDATATst[tstno].info.main_addr;
        
        dat[m++] = gss_CDATATst[tstno].pf;
        m+=66;
//	        IntToByte(dat + 5, (0x8000 | tstno));
//	        ALRT_ERC31_COMMONIN(dat, tstno);
        ALRT_WriteLog(ERR_TYPE_485, dat, m);
    }

    if(gss_ALRTStt[tstno].ewf1.trn & 0x01)
    {
        gss_ALRTStt[tstno].ewf1.trn = 0;
                                            //��¼�¼�
//	        IntToByte(dat + 5, tstno);
//	        ALRT_ERC31_COMMONIN(dat, tstno);
        dat[m++] = ERR_CODE_485_NORMAL;
        dat[m++] = tstno+1;
        dat[m++] = gss_CDATATst[tstno].info.main_addr;
        
        dat[m++] = gss_CDATATst[tstno].pf;
        m+=66;

        ALRT_WriteLog(ERR_TYPE_485, dat, m);
    }
#endif   
    if(gss_ALRTStt[tstno].ewr1.trn & 0x01)
    {
        gss_ALRTStt[tstno].ewr1.trn = 0;
      
                                            //��¼�¼�
        dat[m++] = 0x01;
        dat[m++] = tstno+1;
//	        dat[m++] = gss_CDATATst[tstno].info.main_addr;
//	        
//	        dat[m++] = gss_CDATATst[tstno].pf;
//	        m+=66;
//          IntToByte(dat + 5, (0x8000 | tstno));
//          ALRT_ERC31_COMMONIN(dat, tstno);
//	        ALRT_WriteLog(ERR_TYPE_CP, dat, m);
        memcpy(dat + m, (uint8_t *)&gs_SysVar.terstt.lword, 4);
        m+=4;
        ALRT_WriteLog(ERR_TYPE_CP, dat, m);

    }

    if(gss_ALRTStt[tstno].ewf1.trn & 0x01)
    {
        m = 0;
        gss_ALRTStt[tstno].ewf1.trn = 0;
                                            //��¼�¼�
//          IntToByte(dat + 5, tstno);
//          ALRT_ERC31_COMMONIN(dat, tstno);
        dat[m++] = 0;
        dat[m++] = tstno+1;
//	        dat[m++] = gss_CDATATst[tstno].info.main_addr;
//	        
//	        dat[m++] = gss_CDATATst[tstno].pf;
//	        m+=66;
        memcpy(dat + m, (uint8_t *)&gs_SysVar.terstt.lword, 4);
        m+=4;
        ALRT_WriteLog(ERR_TYPE_CP, dat, m);

    }

#endif
}


//	void ALRT_ERC31_COMMONIN(uint8* dat, uint8 tstno)
//	{
//	    uint8 tmp[10];
//	
//	    gs_RealTime.ReadBCDDateTime((TIME*)tmp);
//	    MoveBuffer(tmp + 1, dat, 5);
//	
//	    ByteArrayHexToBcd(gss_TstRealData[tstno].rtime.YMDhms, dat + 7, 5);
//	    InvBuffer(dat + 7, 5);
//	    Hex42ToGD14(gss_TstRealData[tstno].Wp[0], dat + 12);
//	    Hex42ToGD11(0, dat + 17);
//	}





/************************************************************************
**��������: void ALRT_ERC32(void)
**�� ��: ͨ�����������¼�
**
**�� ��: 
**�� ��: 
**
**�� ע: 
**-----------------------------------------------------------------------
**�� ��: yzy
**�� ��: 09-10-12
*************************************************************************/
//	void ALRT_ERC32(void)
//	{
//	#if ERC32_EN == 1
//	    uint32 GetMonthFlux(void);
//	    uint32 flux;
//	    uint8 dat[20];
//	    
//	    if(gs_EVTComVar.mflux != 0)
//	    {
//	        flux = GetMonthFlux();
//	        if(flux > gs_EVTComVar.mflux)
//	        {
//	            if(gss_ALRTStt[0].ew1.llc == 0)
//	            {
//	                gss_ALRTStt[0].ew1.llc = 1;
//	
//	                gs_RealTime.ReadBCDDateTime((TIME*)dat);
//	                MoveBuffer(dat + 1, dat, 5);
//	                LongToByte(dat + 5, flux);
//	                LongToByte(dat + 9, gs_EVTComVar.mflux);
//	                ALRT_WriteLog(32, dat, 13);
//	            }
//	        }
//	        else
//	        {
//	            gss_ALRTStt[0].ew1.llc = 0;
//	        }
//	    }
//	#endif
//	}


/************************************************************************
 * @function: ALRT_ERC33
 * @����: ���״̬�ֱ�λ�¼�
 * 
 * @����: 
 * @param: pnew ��ǰ״̬�� 
 * @param: pold ֮ǰ״̬��
 * @param: tstno ������
 * 
 * @����: 
 * @return: uint8  true �ɹ� /false ʧ��(�����¼)
 * @˵��: 
 * @����: yzy (2014/2/10)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	uint8 ALRT_ERC33(uint8* pnew, uint8* pold, uint8 tstno)
//	{
//	#if ERC33_EN == 1
//	    uint8 dat[40];
//	    gs_RealTime.ReadBCDDateTime((TIME*)dat);
//	    MoveBuffer(dat + 1, dat, 5);
//	
//	    IntToByte(dat + 5, tstno);
//	    STR_Xor(pnew, pold, dat + 7, 14);
//	    MoveBuffer(pnew, dat + 21, 14);
//	
//	    return ALRT_WriteLog(33, dat, 35);
//	#else
//	    return false;
//	#endif
//	}



/************************************************************************
 * @function: ALRT_ERC37
 * @����: ������¼�
 * 
 * @����: 
 * @param: buffer �ӷ���ʱ�̿�ʼ����ϸ��Ϣ.�ӱ��ڻ�ȡ
 * @param: cnt ���Ǵ���
 * @param: tstno �������
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2014/2/11)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	uint8 ALRT_ERC37(uint8* buffer, uint32 cnt, uint8 tstno)
//	{
//	#if ERC37_EN == 1
//	    uint8 time[10];
//	    MoveBuffer(buffer, buffer + 10, 60);
//	    gs_RealTime.ReadBCDDateTime((TIME*)time);
//	    MoveBuffer(time + 1, buffer, 5);
//	    IntToByte(buffer + 5, tstno);
//	    HexToBcd(buffer + 7, cnt, 3, 4);
//	    return ALRT_WriteLog(37, buffer, 70);
//	#else
//	    return false;
//	#endif
//	}



/************************************************************************
 * @function: ALRT_ERC38
 * @����: ����ť���¼�
 * 
 * @����: 
 * @param: buffer �ӷ���ʱ�̿�ʼ����ϸ��Ϣ.�ӱ��ڻ�ȡ
 * @param: cnt ����ť�д���
 * @param: tstno ������
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2014/2/11)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	uint8 ALRT_ERC38(uint8* buffer, uint32 cnt, uint8 tstno)
//	{
//	#if ERC38_EN == 1
//	    uint8 time[10];
//	    MoveBuffer(buffer,buffer + 10, 60);
//	    gs_RealTime.ReadBCDDateTime((TIME*)time);
//	    MoveBuffer(time + 1 , buffer, 5);
//	    IntToByte(buffer + 5, tstno);
//	    HexToBcd(buffer + 7, cnt, 3, 4);
//	    return ALRT_WriteLog(38, buffer, 70);
//	#else
//	    return false;
//	#endif
//	}

/************************************************************************
 * @function: ALRT_ERC39_46
 * @����: �����±�/ɾ����� �¼���¼
 * 
 * @����: 
 * @param: tstno 
 * @param: tstpo 
 * @param: buf 
 * @param: ercid 
 * @����: 
 * @return: uint8  
 * @˵��: ������˹̹
 * @����: ZJC (2014/8/22)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	uint8 ALRT_ERC39_46(uint16 tstno,uint16 tstpo, uint8 *buffer, uint8 ercid)
//	{
//	#if ERC39_DLMS_EN == 1 || ERC46_DLMS_EN == 1
//	    uint8 time[10];
//	    uint8 buf[38];
//	    
//	    IntToByte(buffer, tstno);    //
//	    IntToByte(buffer + 2, tstpo);    //
//	    
//	    gs_RealTime.ReadBCDDateTime((TIME*)time);
//	    MoveBuffer(time + 1 , buf , 5);
//	    //IntToByte(buf + 5, tstno);    //
//	    MoveBuffer(buffer, buf + 5, LEN_TST_CFG);
//	    //MoveBuffer(time ,buf + 5 + LEN_TST_CFG, 6);  //����ʱ��
//	    return ALRT_WriteLog(ercid, buf, 32);
//	#endif
//	}
/************************************************************************
 * @function: ALRT_ERC40
 * @����: �ų��쳣�¼���¼
 * 
 * @����: 
 * @param: tstno 
 * @param: type 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2014/2/11)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	uint8 ALRT_ERC40(uint8 tstno, uint8 type)
//	{
//	#if ERC40_EN == 1
//	    uint8 time[10];
//	    uint8 buffer[20];
//	    gs_RealTime.ReadBCDDateTime((TIME*)time);
//	    MoveBuffer(time + 1 , buffer , 5);
//	    IntToByte(buffer + 5, tstno);
//	    if((gss_CDATATst[tstno].pt == 0)||(gss_CDATATst[tstno].pt == 1))
//	    {
//	         buffer[7] = 0x02;     //���ܱ�     
//	    }
//	    else
//	    {
//	         buffer[7] = 0x00;     //�ն���Ӵ�����
//	    }
//	    MoveBuffer(gss_CDATATst[tstno].ad ,buffer + 8 , 6);
//	    buffer[14] = type;
//	    return ALRT_WriteLog(40, buffer, 15);
//	    
//	#else
//	    return false;
//	#endif
//	}
/************************************************************************
 * @function: ALRT_DLMS_ERC40
 * @����: �ų��쳣�¼���¼
 * 
 * @����: 
 * @param: tstno 
 * @param: type 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: ������˹̹����Ʊ�׼�¼�
 * @����: ZJC (2014/8/15)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	uint8 ALRT_DLMS_ERC40_41_42_43(uint8 tstno, uint8 type, uint8 *inbuf, uint8 inbuflen)
//	{
//	#if (ERC40_DLMS_EN == 1) || (ERC41_DLMS_EN == 1) || (ERC42_DLMS_EN == 1) || (ERC43_DLMS_EN == 1)
//	
//	    uint8 time[10];
//	    uint8 buffer[14];
//	
//	    gs_RealTime.ReadBCDDateTime((TIME*)time);
//	    MoveBuffer(time + 1 , buffer , 5);
//	    IntToByte(buffer + 5, tstno);
//	    //IntToByte(buffer + 7, tstno);
//	    MoveBuffer(inbuf ,buffer + 7 , inbuflen);
//	    type += 39;
//	    return ALRT_WriteLog(type, buffer, 14);
//	#else
//	    return false;
//	#endif
//	}


/************************************************************************
 * @function: ALRT_ERC41
 * @����: ��ʱ�¼���¼
 * 
 * @����: 
 * @param: oldtime bcd��ʽ
 * @param: newtime bcd��ʽ
 * @param: tstno 
 * 
 * @����: 
 * @return: uint8  true �ɹ� /false ʧ��(�����¼)
 * @˵��: 
 * @����: yzy (2014/2/11)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	uint8 ALRT_ERC41(uint8* oldtime, uint8* newtime, uint8 tstno)
//	{
//	#if ERC41_EN == 1
//	    uint8 buffer[20];
//	    MoveBuffer(oldtime, buffer + 2, 6);
//	    MoveBuffer(newtime, buffer + 8, 6);
//	    IntToByte(buffer, tstno); 
//	    return ALRT_WriteLog(41, buffer, 14);
//	#else
//	    return false;
//	#endif
//	}

/************************************************************************
 * @function: ALRT_DLMS_ERC44
 * @����: �ɼ��������¼�
 * 
 * @����: 
 * @param: tstno 
 * @param: type �á�1������Ƴ����������á�2������Ʋ������ã��á�3���������������������������ã�
 * @param: flag �á�1�����ɹ����á�0����ʧ�ܣ�
 * 
 * @����: 
 * @return: uint8  
 * @˵��: ������˹̹��
 * @����: ZJC (2014/11/1)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	uint8 ALRT_DLMS_ERC44(uint8 tstno, uint8 type, uint8 flag, ST_ACT_INPBUF_S *stpinbuf)
//	{
//	#if ERC47_DLMS_EN == 1
//	    uint8 time[10];
//	    uint8 buffer[41];
//	    ClearBuffer(buffer, 41);
//	    
//	    gs_RealTime.ReadBCDDateTime((TIME*)time);
//	    MoveBuffer(time + 1 , buffer , 5);
//	    IntToByte(buffer + 5, tstno);
//	    buffer[7] = type;
//	    buffer[8] = flag;
//	    if(stpinbuf->len > 32)
//	    {
//	        stpinbuf->len = 32;
//	    }
//	    MoveBuffer(stpinbuf->buf, buffer + 9 , stpinbuf->len);
//	    
//	    return ALRT_WriteLog(44, buffer, 41);
//	#else
//	    return false;
//	#endif
//	}

/************************************************************************
 * @function: ALRT_DLMS_ERC47
 * @����: �Ա��Уʱʧ���¼�
 * 
 * @����: 
 * @param: tstno 
 * @param: type  �¼����루1�����ڱ�Ʋ�֧�ֶ�ʱ������2�������ʱ����������÷�ֵ��3������ͨ��ʧ������Ķ�ʱʧ�ܲ�����
 * 
 * @����: 
 * @return: uint8  
 * @˵��: ������˹̹��
 * @����: ZJC (2014/11/1)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	uint8 ALRT_DLMS_ERC47(uint8 tstno, uint8 type)
//	{
//	#if ERC47_DLMS_EN == 1
//	    uint8 time[10];
//	    uint8 buffer[14];
//	    
//	    gs_RealTime.ReadBCDDateTime((TIME*)time);
//	    MoveBuffer(time + 1 , buffer , 5);
//	    IntToByte(buffer + 5, tstno);
//	    buffer[7] = type;
//	    MoveBuffer(time, buffer + 8 , 6);
//	    return ALRT_WriteLog(47, buffer, 14);
//	#else
//	    return false;
//	#endif
//	}
//	

/***********************************************************
 * @function: ALRT_ERC60
 * @����: ��������¼���¼
 * 
 * @����: 
 * @param: ext  
 * 
 * @����: 
 * @return: uint8   
 * @����: yzy (2010-3-30)
 *---------------------------------------------------------
 * @�޸���: 
 **********************************************************/
//	uint8 ALRT_ERC60(uint32 ext)
//	{
//	#if ERC60_EN == 1
//	    uint8 dat[40];
//	    gs_RealTime.ReadBCDDateTime((TIME*)dat);
//	    MoveBuffer(dat + 1, dat, 5);
//	    LongToByte(dat + 5, ext);
//	
//	    return ALRT_WriteLog(60, dat, 9);
//	#endif
//	}



/************************************************************************
**��������: void ERC44_Collctor_Updata(void* pdata)
**�� ��: �ϵ���жϲɼ����Ƿ������ɹ�
**
**�� ��: 
**�� ��: 
**
**�� ע: 
**--------------------------------------------------------
**�� ��: zjc
**�� ��: 09-09-09
*************************************************************************/
//	void ERC44_Collctor_Updata(void)
//	{   
//	    ST_ACT_INPBUF_S    inSbuf;
//	    GD_Para_RW(VER_HEXING, inSbuf.buf, 20, false);    //��ȡFLASH�е��ڲ�汾��
//	    if(inSbuf.buf[0] == 'F' && inSbuf.buf[1] == 'W')
//	    {
//	        extern const uint8 DefParaC1F21F22[20];
//	        inSbuf.len = 20;
//	        if(CmpBuffer((uint8 *)DefParaC1F21F22, inSbuf.buf, 20) == 0)   //�汾��ȫ��ͬ�������ɹ�
//	        {
//	            ALRT_DLMS_ERC44(0 , 3 , 1, &inSbuf);
//	        }
//	        else
//	        {
//	            ALRT_DLMS_ERC44(0 , 3 , 0, &inSbuf);
//	        }
//	        memset(inSbuf.buf, 0xFF, 20);
//	        GD_Para_RW(VER_HEXING, inSbuf.buf, 20, true);    //���FLASH�е��ڲ�汾��
//	    }
//	}


/************************************************************************
**��������: void ERC44_MT_Updata(void* pdata)
**�� ��: ��������Ƿ�ɹ�
**
 * @����: tsno  ������
 * @����: okflag  1�� �����ɹ� ��0������ʧ��
**�� ��: 
**
**�� ע: 
**--------------------------------------------------------
**�� ��: zjc
**�� ��: 09-09-09
*************************************************************************/
//	void ERC44_MT_Updata(uint8 tsno, uint8 okflag)
//	{   
//	    ST_ACT_INPBUF_S    inSbuf;
//	    TDataBlock tdb;
//	    ES_FILE_DB_Open(DB_MT_UPDATE_PARA, &tdb, TDB_MODE_RW);
//	    ES_FILE_DB_ReadFrom(&tdb, (uint8 *)&g_Updata, sizeof(EXT_UPDATASTRUCT), 0);
//	    
//	    inSbuf.len = 20;
//	    MoveBuffer(g_Updata.mtVer, inSbuf.buf, 20);
//	
//	    if(okflag == 1)
//	    {
//	        g_Updata.FlagState[g_Updata.tstno] = METER_UPGRADE_OK;
//	    }
//	    else
//	    {
//	        g_Updata.FlagState[g_Updata.tstno] = METER_UPGRADE_BAD;
//	    }
//	
//	    ES_FILE_DB_WriteFrom(&tdb, (uint8 *)&g_Updata, sizeof(EXT_UPDATASTRUCT), 0);
//	    ES_FILE_DB_Close(&tdb);
//	    
//	    ALRT_DLMS_ERC44(tsno , 1 , okflag, &inSbuf);
//	}
//	


/************************************************************************
 * @function: ALRT_SecProc
 * @����: �봦����
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/2/11)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void ALRT_SecProc(void)
{
    ALRT_ERC1(0);                       //�汾����ж�
//	    ALRT_ERC4();                        //״̬����λ

    
    ALRT_ActiveReport();                //�����ϱ��¼�

}



/************************************************************************
**��������: void ALRT_MinProc(void* pdata)
**�� ��: �ִ�����
**
**�� ��: 
**�� ��: 
**
**�� ע: 
**--------------------------------------------------------
**�� ��: yzy
**�� ��: 
*************************************************************************/
//	void ALRT_MinProc(void)
//	{
//	    ALRT_ERC32();                               //��������
//	}






/************************************************************************
**��������: void ALRT_TstProc(void* pdata)
**�� ��: �ⲿ�������¼�����
**
**�� ��: 
**�� ��: 
**
**�� ע: ������ͳ�ƽ�����(ÿ��������)ִ��
**--------------------------------------------------------
**�� ��: yzy
**�� ��: 2008-11-29
*************************************************************************/
//	void ALRT_TstProc(void)
//	{
//	    for(uint8 uc_i = 0; uc_i < CORE_TST_NO; uc_i++)
//	    {                                       //��������Ч��������485��
//	        if((gss_CDATATst[uc_i].ef != 0x01) || (gss_CDATATst[uc_i].tf != 0x01))
//	        {
//	            continue;
//	        }
//	
//	        ALRT_ERC12(uc_i);                   //ʱ�䳬��
//	        ALRT_ERC13(uc_i);                   //������
//	    }
//	}



/************************************************************************
**��������: void ALRT_CLR(void* pdata)
**�� ��: ��������¼���¼
**
**�� ��: 
**�� ��: 
**
**�� ע: 
**--------------------------------------------------------
**�� ��: yzy
**�� ��: 09-12-30
*************************************************************************/
void ALRT_CLR(void)
{
    TDataBlock db;
//	    ES_FILE_DB_Open(DB_EVT, &db, TDB_MODE_RW);
//	    for(uint16 j = 0; j < 24; j++)
//	    {
//	        ES_FILE_DB_WriteFrom(&db, (void*)0, 4096, j * 4096);
//	    }
//	    ES_FILE_DB_Close(&db);
    CBB_FILE_Erase_Event(DB_FLASH);
}



/************************************************************************
**��������: void ALRT_Init(void* pdata)
**�� ��: �澯���̳�ʼ��
**
**�� ��: 
**�� ��: 
**
**�� ע: 
**--------------------------------------------------------
**�� ��: yzy
**�� ��: 09-09-09
*************************************************************************/
void ALRT_Init(void)
{
    //do nothing
    
}






/************************************************************************
 * @function: ALRT_DayProc
 * @����: �մ����� 
 * 
 * @����: 
 * @param: pdata 
 * @����: 
 * @˵��: 
 * @����: Zhenggb (2012-5-16)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	void ALRT_DayProc(void)
//	{                                   //1)���µ�����ͳ�ƶ���
//	     STAT_TMMonSave();
//	     if(GetTime()->dmon == 1)
//	     {
//	         STAT_TMMonCrack();
//	     }
//	                                    //2)��������ͳ�ƶ���
//	     STAT_TMDaySave();
//	     STAT_TMDayCrack();
//	                                    //3)��յ���ͳ��ʵʱ����
//	     STAT_Reset();
//	     
//	     gdw_SaveTstDayStat();             //���ܱ��ѹͳ�� �ն��� 
//	     
//	}

/************************************************************************
 * @function: ALRT_DayProc
 * @����: �մ����� 
 * 
 * @����: 
 * @param: pdata 
 * @����: 
 * @˵��: 
 * @����: Zhenggb (2012-5-16)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	void ALRT_MonthProc(void)
//	{
//	     gdw_SaveTstMonthStat();
//	}





















/*******************************************************************************
 * @function_name:  ES_ALRT_Task
 * @function_file:  alert.c
 * @����: �澯���̴�����
 * 
 * 
 * @����:  
 * 
 * @����:  
 * @����:  yzy (2011-1-24)
 *-----------------------------------------------------------------------------
 * @�޸���:  
 * @�޸�˵��: 
 ******************************************************************************/
//	void ES_ALRT_Task(void)
//	{
//	                                            //������Ϣ
//	    TESMsg* msg = (TESMsg*)gs_Mem.GetMEM(sizeof(TESMsg));
//	    uint8 nmsg;                             //������Ϣ
//	    
//	    ALRT_Init();
//	    
//	    ES_MSG_Apply(MSG_CLS_TM);
//	    
//	    for(;;)
//	    {
//	        nmsg = gs_OS.Message_Wait(&msg); //��ȡ������Ϣ
//	        switch(nmsg)                         //������Ϣ�ֱ���
//	        {
//	            case MSG_SEC:                   //����Ϣ����
//	                ALRT_SecProc();
//	                break;
//	                
//	            case MSG_MIN:                   //����Ϣ����
//	                ALRT_MinProc();
//	                break;
//	
//	            case MSG_DAY:                   //����Ϣ����
//	                ALRT_DayProc();
//	                break;
//	
//	            case MSG_MON:
//	                ALRT_MonthProc();
//	                break;
//	                
//	            case MSG_HLV_CLR:               //����
//	                Inp_DataClear();
//	                break;
//	                
//	            case MSG_ALRT_MTSTAT:           //������ݸ���,ͳ�����
//	                ALRT_TstProc();
//	                break;
//	
//	            case MSG_ALRT_ERC1:             //���������ݳ�ʼ��
//	                ALRT_ERC1(MSG_ALRT_ERC1);
//	                break;
//	
//	            case MSG_ALRT_CLR:              //��������¼�
//	                ALRT_CLR();
//	                break;
//	
//	            case MSG_ALRT_PARACKECK:        //�����ѱ�
//	                break;
//	                
//	            case MSG_LIVE:
//					HB_RetLive();
//	                break;
//	        }
//	        
//	    }
//	}
//	
//	/******************************************************************************
//	**Ӧ�ó��������̿鳣�����ֶ���
//	******************************************************************************/
//	const TESTcbConst gs_TkAlrt = 
//	{
//	    ES_ALRT_Task,
//	    TASK_ALRT_TKID >> 3,                    //Ӧ�ù������
//	    TASK_ALRT_TKID,                         //���̱��
//	    TASK_ALRT_TKID & 7,                     //���ȼ��ڽ��̱��
//	    true,                                   //��Ϣѭ�����ƽ���
//	};







