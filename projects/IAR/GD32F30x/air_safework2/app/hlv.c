/****************************************Copyright (c)**************************************************
**                                  ____�Ƽ����޹�˾
**                                          ������
**
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: main.c
**��   ��   ��: yzy
**����޸�����: 2019��6��1��
**��        ��: �����ϵͳ����ļ�
**ע        ��:�û���ʹ��ƽ̨ʱӦ�ñ������������ƽ̨�Ľӿ��뱾�ļ�һ��
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: yzy
** ��  ��: v1.0
** �ա���: 2019��6��1��
** �衡��: ԭʼ�汾
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
 * @����: ����Ϣ
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2013/10/30)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void HLV_SecProc(void)
{
                                            //ȷ�����᳤��ֹͣ����
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
    if(gs_SysVar.mDGstt & HLV_STT_USBP)         //����������100ms��˸
    {
        gs_GPIO.BlinkSet(SYS_LED_RUN, 1, 1, 0);
    }
    else if(gs_SysVar.mDGstt & HLV_STT_USBE)    //U�̴���200ms��˸
    {
        gs_GPIO.BlinkSet(GPIO_LED_RUN, 1, 2, 0);
    }
    else if(gs_SysVar.mDGstt & (HLV_STT_NENG || HLV_STT_LENG))
    {
         gs_GPIO.BlinkSet1(GPIO_LED_RUN, 1, 1, 10, 0);
    }        
    else                                //��������������³���
    {
         gs_GPIO.BlinkSet1(GPIO_LED_RUN, 1, 5, 5, 0);
    }
#ifdef SYS_LP_BY_AD   
    if(vol >= 14000)
#else
    if(!(gs_GPIO.GPI_GetStt(0) & FGPI_STT_ENG))        //�е�
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
            ALRT_ERC14(0);                  //ϵͳ���й����е��ϵ��¼
            if(aos_now_ms() > 5000)//����ָ�ʱ�������ݣ��ϵ�5��ǰ��Ϊ�Ǹ�λ
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
    else                                    //ͣ��
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
            gs_SysVar.mDGcnt = 0;          //�����涨����󱣳�60��
            LOG_INFO("vol lost = %d V\n", vol);
            
                                            //�󱸵�س����ǰ����,��ͣ����¼ͣ��ʱ��
//	            MoveBuffer(gs_SysTime.YMDhms, gs_FramMisData.tbk, 6);
//	            SaveFramMisData(MIS_TBK);
        }

        if((gs_SysVar.mDGstt & HLV_STT_NENG))
        {
            
            if(gs_SysVar.mDGcnt < 60)
                gs_SysVar.mDGcnt++;
            if(gs_SysVar.mDGcnt == 2)//����
            {
            
                ALRT_ERC14(1);                  //ϵͳ���й����еĵ����¼
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

//	    if(gs_SysVar.mDGcnt > 0)                //ϵͳ״̬���Ƽ�����
//	    {
//	        gs_SysVar.mDGcnt--;
//	    }

    EVT_SecProc();                       //ң�ŵ��ж�

//	    if(gs_SysVar.mDGstt & HLV_STT_NENG)
//	    {
//	        if(gs_SysVar.mDGcnt == 0)
//	        {                                   //��ɱ����
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
 * @����: �رյ��
 * @����: 
 * @����: 
 * @˵��: 
 * @����: ZJC (2015/05/15)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void HLV_BatCtl(void)
{
//	    gs_OS.TK_Sleep(200);
//	    gs_GPIO.GPO_Out(GPO_BAT_CTL, false);
}




/************************************************************************
 * @function: UpdateSysTimeStruct
 * @����: ����gs_SysTime��gs_SysLastDayTime
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2013/10/30)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void UpdateSysTimeStruct(void)
{
    uint8 ymdhms[6];                    //������ʱ����(hex)
    const TIME* time = gs_RealTime.GetTime();
                                        //���YMDHMS
    MoveBuffer((uint8*)&time->sec, ymdhms, 6);
    InvBuffer(ymdhms, 6);
                                        //����ϵͳʱ��ṹ��
    CPSR_ALLOC();
    RHINO_CPU_INTRPT_DISABLE();
    BuildTimeLabel(TTYPE_YMDHMS, ymdhms, &gs_SysTime);
    StepDays(ymdhms, -1);
    BuildTimeLabel(TTYPE_YMDHMS, ymdhms, &gs_SysLastDayTime);
    RHINO_CPU_INTRPT_ENABLE();
    
}





/************************************************************************
 * @function: HLV_MinProc
 * @����: ��ʱ��
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2013/10/31)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void HLV_MinProc(void)
{
    const TIME* time = GetTime();

    UpdateSysTimeStruct();
    
//	    _JudgeTaskFlag();    
                                        //���е��ǰ����,ÿ���Ӹ��¼�¼����ͣ��ʱ��
    if(!(gs_SysVar.mDGstt & HLV_STT_NENG))
    {
        //MoveBuffer(gs_SysTime.YMDhms, gs_FramMisData.tbk, 6);
        //SaveFramMisData(MIS_TBK);
    }
                                        //ͳ�Ʒִ���
    //STAT_MinProc();


    //-------------------------------------------------------------

    

    //-------------------------------------------------------------
//	    uint8 flag = 0;                         //���������־
//	    uint8 iscbqj;                           //�Ƿ��ڳ��������־
//	    uint32 dmins;                           //���������
//	    uint32 nxm;                             //�¸��������_next mins
//	
//	                                            //����������˿����δ���
//	    for(uint8 uc_i = 0; uc_i < MCB_PORT_NUM; uc_i++)
//	    {
//	                                           //�㲥Уʱ
//	        if(gs_FramMisData.ckten == 0xAA)   //�ܿ���
//	        {
//	            if((gs_InpExtracb[uc_i].ctdhm[0] == GetTime()->dmon) || (gs_InpExtracb[uc_i].ctdhm[0] == 0))
//	            {
//	                if((gs_InpExtracb[uc_i].ctdhm[1] == GetTime()->hour) && 
//	                   (gs_InpExtracb[uc_i].ctdhm[2] == GetTime()->min))
//	                {                               //�ö˿���ҪУ�Ա��
//	                    if(gs_InpExtracb[uc_i].cten == 1)//�ֿ���
//	                    {
//	                        gs_InpExtracb[uc_i].gbbreak = 1;
//	                        gs_InpExtracb[uc_i].gbbvsec = 60;
//	                        //gs_OS.Message_Send(MSG_NETP_CKTIME, TASK_NETP_TKID + uc_i);
//	                    }
//	                }
//	            }
//	        }
//	                                            //����ʱ�ε��ж�
//	        flag = 0;
//	        iscbqj = 0;                         //�ж��Ƿ��ڳ�������
//	        dmins = gul_NetpReadMins[uc_i] % 1440;
//	        if((gs_InpExtracb[uc_i].cbqjn == 0) || (gs_InpExtracb[uc_i].cbqjn > 24))
//	        {                                   //�Ƿ�����ʱʼ�տ��Գ���
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
//	        {                                   //����/����������(�������ӿ�ʼ��ʱ��)
//	            nxm = gul_NetpReadMins[uc_i] - (dmins % gs_InpExtracb[uc_i].netitv) + gs_InpExtracb[uc_i].netitv;
//	            if((gs_SysTime.mins >= nxm) || (gs_SysTime.mins < gul_NetpReadMins[uc_i]))
//	            {
//	                flag = 1;
//	            }
//	                                            //������ʱ�̳���
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
//	        if(gs_SysTime.mins % 15 == 0)     //15min����һ��
//	        {
//	            flag = 1;
//	        }
//	        if(flag)                            //��Ҫ����
//	        {
//	            gul_NetpReadMins[uc_i] = gs_SysTime.mins;
//	            //gs_OS.Message_Send(MSG_NETP_IDATA, TASK_NETP_TKID + uc_i);
//	        }
//	    }


}


/************************************************************************
 * @function: HLV_HourProc
 * @����: Сʱ��Ϣ
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2013/10/30)
 *-----------------------------------------------------------------------
 * @�޸���: 
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
 * @����: ����Ϣ
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2013/10/30)
 *-----------------------------------------------------------------------
 * @�޸���: 
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
 * @����: �״��ϵ�ʱ��ͣ�ϵ紦��
 * @����: 
 * @����: 
 * @����: yzy (2010-3-27)
 *---------------------------------------------------------
 * @�޸���: 
 **********************************************************/
void HLV_LDProc(void)
{
    uint32 tnow;
    uint32 tpass;
    uint8 tn[6];                            //ssmmhhDDMMYY
    uint8 tp[6];                            //ssmmhhDDMMYY
                                            //��¼�ϴ�ͣ��ʱ��
//	    MoveBuffer(gs_FramMisData.tbk, gs_FramMisData.tdt, 6);
//	    MoveBuffer(gs_FramMisData.tdt, tp, 6);
//	    InvBuffer(tp, 6);
                                            //��ȡ��ǰʱ��
    MoveBuffer((uint8*)&GetTime()->sec, tn, 6);
                                            //���㵱��ͣ���ۼ�ʱ��
    tnow = CalcMins(tn + 1, false);
    tpass = CalcMins(tp + 1, false);
//	    if(tnow > tpass)
//	    {
//	        if(CmpBuffer(tn + 3, tp + 3, 3) == 0)
//	        {                                   //ͣ��ʱ���ڵ���
//	            gs_TmStatData.tdn += (tnow - tpass);
//	        }
//	        else
//	        {                                   //ͣ��ʱ��ǵ���
//	            gs_TmStatData.tdn = tn[2] * 60 + tn[1];
//	        }
//	    }
//	    gs_TmStatData.rstn += 1;                //�ո�λ����
//	    STAT_TMSave();                          //����ͳ��
}









//------------------------------------------------------------------------














/*********************************************************************************************************
** ��������: void HLV_LGPIFallProc(void* pdata)
** ��������: ���ٶ˿��½����¼�
** �䡡��:
            pdata û������

** �䡡��: ��
**
** ȫ�ֱ���: ��
** ����ģ��: ��
**
** ������: yzy
** �ա���: 2007��9��4��
**-------------------------------------------------------------------------------------------------------
** �޸���:
** �ա���:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void HLV_LGPIFallProc(void* pdata)
{
    uint32 event = gs_GPIO.GPI_Event(GPI_TYPE_LIFEVT);
    if(event & 0x01)                        //��̼�����
    {
        //gs_OS.Message_Send(MSG_SLV_PGKEY, TASK_SLV_TKID);
    }
}



/*********************************************************************************************************
** ��������: void HLV_LGPIUpProc(void* pdata)
** ��������: ���ٶ˿��������¼�
** �䡡��:
            pdata û������

** �䡡��: ��
**
** ȫ�ֱ���: ��
** ����ģ��: ��
**
** ������: yzy
** �ա���: 2007��9��4��
**-------------------------------------------------------------------------------------------------------
** �޸���:
** �ա���:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void HLV_LGPIUpProc(void* pdata)
{
    uint32 event = gs_GPIO.GPI_Event(GPI_TYPE_LIREVT);
    if(event & 0x01)                        //��̼�̧��
    {
        //gs_OS.Message_Send(MSG_SLV_PGKUP, TASK_SLV_TKID);
    }
}


/*********************************************************************************************************
** ��������: void HLV_LGPILastProc(void* pdata)
** ��������: ���ٶ˿ڳ����¼�
** �䡡��:
            pdata û������

** �䡡��: ��
**
** ȫ�ֱ���: ��
** ����ģ��: ��
**
** ������: yzy
** �ա���: 2007��9��4��
**-------------------------------------------------------------------------------------------------------
** �޸���:
** �ա���:
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
** ��������: void HLV_Init(void* pdata)
** ��������: �߼�ɨ����̵ĳ�ʼ�������������þ���Ӧ�ó�ʼ�����Ӻ���
** �䡡��:
            pdata û������

** �䡡��: ��
**
** ȫ�ֱ���: ��
** ����ģ��: ��
**
** ������: yzy
** �ա���: 2007��9��4��
**-------------------------------------------------------------------------------------------------------
** �޸���:
** �ա���:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void HLV_Init(void)
{
    //�ϵ������������ֳ���
    //ES_Message_Send(MSG_NETP_DISCOSTART, TASK_NETP_TKID); //�ѱ�
    //gs_OS.Message_Send(MSG_NETP_IDATA, TASK_NETP_TKID);
}

/************************************************************************
 * @function: HLV_PreInit
 * @����: ϵͳ����ǰ�ĳ�ʼ������
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/11/12)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void HLV_PreInit(void)
{
#if (SA_MNT_EN > 0)
    SA_MNT_OC(false);
#endif
                                            //����
    ClearBuffer((uint8*)gss_CDATATst, sizeof(gss_CDATATst));
    ClearBuffer((uint8*)&gs_InpVarData, sizeof(gs_InpVarData));
    ClearBuffer((uint8*)gs_InpExtracb, sizeof(gs_InpExtracb));
    ClearBuffer((uint8*)gss_TstRealData, sizeof(gss_TstRealData));
    
    UpdateSysTimeStruct();                  //����Ӧ�ò�ϵͳʱ��ṹ��
    gul_NetpReadMins[0] = gs_SysTime.mins;
    gul_NetpReadMins[1] = gs_SysTime.mins;
    gul_NetpReadMins[2] = gs_SysTime.mins;
    
    guc_HourSaveFlag = 0xFF;
    guc_DaySaveFlag = 0xFF;
    guc_RecSaveFlag = 0xFF;
    guc_MonSaveFlag = 0xFF;
    guc_QuarSaveFlag = Bit_Map8[GetTime()->min / 15];
    
    
    //��ʼ��ϵͳ����
    gs_SysVar.mDGstt = HLV_STT_NENG;        //��ǰΪͣ��״̬
    gs_SysVar.mLPsysstt = 0;        //��ǰΪͣ��״̬
    gs_SysVar.mDGcnt = 0;                   //��ǰ������Ϊ5
    gs_SysVar.terstt.lword = 0;
    gs_SysVar.batVal = 160;//16v
    gs_SysVar.AI0 = 0;
    gs_SysVar.AI1 = 0;
    gs_SysVar.mLPstt = 0;
                                            //����
    ClearBuffer((uint8*)&gs_MKStruct, sizeof(gs_MKStruct));
    //�������
    LoadSystemParam(PARA_TYPE_INP);
    LoadSystemParam(PARA_TYPE_TASK);
//	    LoadFramMisData();  
    int evtNum = 0;
//    CBB_FILE_Read_Event_Num(DB_FLASH, &evtNum, &gs_FramMisData.e1cnt);
    gs_FramMisData.e1pt = gs_FramMisData.e1cnt;
    //������ʼ��
    HLV_LDProc();                           //����ʱ��ͣ�ϵ�Ĵ���
    
    
}


/************************************************************************
**��������: void EVT_SecProc(void)
**�� ��: ���ն��¼����µ�������0(ÿ����)
**
**�� ��: 
**�� ��: 
**
**�� ע: 
**--------------------------------------------------------
**�� ��: yzy
**�� ��: 
*************************************************************************/
void EVT_SecProc(void)
{
    uint8 EVT_JudgeMK(void);
    EVT_JudgeMK();
}





//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>




/************************************************************************
 * @function: EVT_JudgeMK
 * @����: �ſ���˲ʱ״̬
 * @����: 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2014/2/9)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 EVT_JudgeMK(void)
{
    uint32 iost;
    uint32 iowd;
    MKStruct* es = &gs_MKStruct;
    uint8 change;

    iowd = gs_GPIO.GPI_GetStt(0);       //��ȡ��ǰ��״̬
    iost = iowd;                        //��ȡ�˿�״̬
    iost >>= YX_IOBIT;

    for(uint8 uc_i = 0; uc_i < YX_NUM; uc_i++)
    {
        if(!(es->mGateIn & (1 << uc_i)))//����ǳ�����
        {
            iost ^= (1 << uc_i);
        }
    }
                                        //���ص�ǰ��״̬
    iost &= Bit_Mask8[YX_NUM];

    if(es->mValid == 0)                 //�״�ִ��
    {
        es->mValid = 1;
        es->mState = iost;
        SYS_OK();
    }
                                        //�жϱ�λ
    if(iost != es->mState)              //���˲ʱ״̬�͵�ǰ״̬��һ��
    {
                                        //�仯������
        change = iost ^ es->mState;

        for(uint32 m = 0; m < YX_NUM; m++)
        {
            if(change & (1 << m))
            {                           //����������Ȼ��״̬����
                es->mState ^= (1 << m);
                                        //�жϲ��������¼������½��¼�
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

        gs_MKStruct.mb |= change;    //δ�ϱ���־
    }
    SYS_OK();
}











extern kbuf_queue_t gs_TKFarpQueue;
extern kbuf_queue_t gs_MainQueue;
/************************************************************************
 * @function: SYS_HLV_Task
 * @����: HLV���̴�����
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2013/10/31)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void SYS_HLV_Task(void)
{
    //������Ϣ
    krhino_buf_queue_create(&gs_TKHlvQueue, "hlv_queue",
                         gc_TKHlvQbuf, MSG_BUFF_LEN, BUFQUEUE_MSG_MAX);
    
    HLV_Init();

    SYS_MSG_Apply(TASK_HLV_TKID, MSG_CLS_TM);

    for(;;)
    {
        krhino_buf_queue_recv(&gs_TKHlvQueue, RHINO_WAIT_FOREVER, g_TKHlvQ_buf_recv,
                                  &g_TKHlvQ_recv_size);

        switch(g_TKHlvQ_buf_recv[0])                        //������Ϣ�ֱ���
        {
            case MSG_SEC:                   //Ӧ������������Ϣ����
                gs_SysVar.mLPstt |= HLV_LPTASK_SMSG_HLV;
                HLV_SecProc();
            
                gs_SysVar.mLPstt &= ~HLV_LPTASK_SMSG_HLV;
                break;

            case MSG_MIN:                   //����Ϣ����
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
                
            case MSG_HLV_BATCTL:        //��ʱ�رյ��
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
                
            case MSG_HLV_TASK:               //�����������
//	                Task_SetTimerCfg(false);
                break;            
            case MSG_ALRT_ERC1:             //���������ݳ�ʼ��
                ALRT_ERC1(MSG_ALRT_ERC1);
                break;
            case MSG_ALRT_CLR:              //��������¼�
                ALRT_CLR();
                LOG_DEBUG("******MSG_ALRT_CLR******** !!!!\n");
                
                break;
            case MSG_HLV_CLR:               //����
                LOG_DEBUG("******MSG_HLV_CLR******** !!!!\n");
                Inp_DataClear();
                break;

        }

    }
}

/******************************************************************************
**Ӧ�ó��������̿鳣�����ֶ���
******************************************************************************/
//	const TESTcbConst gs_TkHLV = 
//	{
//	    ES_HLV_Task,
//	    TASK_HLV_TKID >> 3,                     //Ӧ�ù������
//	    TASK_HLV_TKID,                          //���̱��
//	    TASK_HLV_TKID & 7,                      //���ȼ��ڽ��̱��
//	    true,                                   //��Ϣѭ�����ƽ���
//	};
const KTaskConst gs_TkHLV = 
{
    &gs_TKHlvHandle,
    SYS_HLV_Task,                           //Ӧ�ù������
    gs_TKHlvStack,
    TASK_HLV_TKID,                           //���̱��
    true,                                    //��Ϣѭ�����ƽ���
    TASK_HLV_STKL,
    "Hlv",
    &gs_TKHlvQueue,
};



