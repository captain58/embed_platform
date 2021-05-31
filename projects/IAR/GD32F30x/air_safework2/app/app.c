/****************************************Copyright (c)**************************************************
**                                  ________�Ƽ����޹�˾
**                                          ������
**
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: main.c
**��   ��   ��: yzy
**����޸�����: 2013��6��8��
**��        ��: Ӧ�ó���������
**ע        ��:
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
**������: yzy
**��  ��: v1.0
**�ա���: 2013��6��8��
**�衡��: ԭʼ�汾
**-------------------------------------------------------------------------------------------------------
** �޸���: 
** ��  ��: 
** ��  ��: 
********************************************************************************************************/

/*******************************************************************************
**�����������߽ӿ�ͷ�ļ�������������ע��һ������ֻ��Ҫ��һ��c
**�ļ��ж��������������
********************************************************************************/
#define _USR_MAIN

/*******************************************************************************
**����궨����߽ӿ��ļ����������ڵ���ģʽ�£���ôƽ̨������ʱ��
**��������û�������������Ե�CRCУ��
********************************************************************************/
#ifdef DEBUG
#define _USR_DEBUG
#endif

#define _USR_FLASH

/*******************************************************************************
**�û������ÿһ��Դ�ļ����ø��������ͷ�ļ��������޷�ʹ��ϵͳ����
********************************************************************************/
#define EXT_MAIN


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
/*******************************************************************************
**�û�����汾��
********************************************************************************/
const __root uint32 gul_UsrFuncVer@FLS_USRVER_ADDR = 0x19090603;
const __root uint8 gucs_PrjCode[6]@FLS_USRPRJ_ADDR = "RTU01";
const __root uint8_t gucs_softVer[]="4G-LS-R(V0.";

const __root uint32 gul_UsrHardcVer = 0x19071701;
const __root uint8_t gucs_HDVer[]="4G-LS-R(V0.";

//	const __root uint32 gul_DebugMarkA@FLS_USRMARKA_ADDR = 0;
//	const __root uint32 gul_DebugMarkB@FLS_USRMARKB_ADDR = 0;

static char         g_MQ_buf_recv[BUFQUEUE_MSG_MAX+4];

size_t       g_MQ_recv_size;
ktimer_t     g_MainTimer;

cpu_stack_t  gs_MainStack[TASK_MAIN_STKL];
ktask_t      gs_MainHandle;
kbuf_queue_t gs_MainQueue;
char         gc_MQbuf[MSG_BUFF_LEN];
/*******************************************************************************
 * @function_name:  SYS_MAIN_Init
 * @function_file:  __WaitForAllTaskReady
 * @����: �ȴ��������̳�ʼ�����
 * 
 * 
 * @����: 
 * 
 * @����: 
 * @����: yzy (2018��3��22��)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
//	void __WaitForAllTaskReady(void)
//	{
//	    while(!(gl_SysRunStt & HRS_APPRUN))
//	    {
//	        SYS_ENTER_SCRT();
//	        STR_SetBit(gucs_TaskInitStt, gs_TkTcbCur->tkid);
//	        SYS_EXIT_SCRT();
//	        
//	        gs_OS.TK_Sleep(5);
//	    }
//	}
//	
//	
//	typedef struct
//	{
//	    uint16 key;                //�ϱ���λԿ��
//	    uint8* data;               //�ϱ�����
//	    uint8  len;                //�ϱ����ݳ���
//	    uint16 crc;                //�ϱ�����crc
//	    uint8  chn;                //�ϱ�����ͨ��
//	}S_HX_WDTTstUp;
//	
//	S_HX_WDTTstUp gs_wdtTstUp;
//	


/************************************************************************
 * @function: GdUp_HXWDTTst
 * @����: 
 * 
 * @����: 
 * @param: 
 * @param: 
 * 
 * @����: 
 * @return: uint8  SYS_ERR_OK,SYS_ERR_FT,SYS_ERR_NOREPLY
 * @˵��: 
 * @����: yzy (2014/2/9)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	void GdUp_HXWDTTst(void)
//	{
//	    if(gs_wdtTstUp.key == 0xAA55)
//	    {
//	        //�����������.���������,������֮��Ĵ��븴λ,�����뷴��������ѭ��.
//	        gs_wdtTstUp.key = 0;              //������Ź���λ��־
//	        
//	        //����
//	        if(gs_wdtTstUp.crc == DoCrc16(gs_wdtTstUp.data, gs_wdtTstUp.len, 0x5555))
//	        {
//	            gfs_PSTChnSend[gs_wdtTstUp.chn](gs_wdtTstUp.data, gs_wdtTstUp.len);
//	        }
//	        
//	    }
//	    
//	
//	}

void WDT_IRQHandler(void)
{
    CPSR_ALLOC();
    RHINO_CPU_INTRPT_DISABLE();
    RHINO_CPU_INTRPT_ENABLE();
}
#define CON_LIVE_SLEEP  100

#define CON_LIVE_TIME_OUT 18//(1800 / CON_LIVE_SLEEP)
#define CON_LIVE_REQ_COUNT (10000 / CON_LIVE_SLEEP)
/*******************************************************************************
 * @function_name:  HB_TaskLiveReq
 * @function_file:  main.c
 * @����: ���ͱ�������
 * 
 * 
 * @����: 
 * 
 * @����: 
 * @����: yzy (2011-03-28)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
uint8_t HB_TaskLiveReq(int count)
{
                                            //�����ж�
    extern const KTaskDeclare __TKDeclare[SYS_TK_NUM];
    const KTaskDeclare* dec;
    uint8 msg = MSG_LIVE;
    if((count % CON_LIVE_REQ_COUNT) != 0)
    {
        for(uint32 ut_i = 0; ut_i < (SYS_TK_NUM); ut_i++) 
        {
            if(gucs_LiveCnt[ut_i] > CON_LIVE_TIME_OUT)			//3����δ�ظ�������Ϣ
            {
                LOG_DEBUG("soft watch dog thread %d could be died,sys will reboot!!! \n", ut_i);
                return SYS_ERR_FT;
            }        
        }
        return SYS_ERR_OK;
    }
    for(uint32 ut_i = 0; ut_i < (SYS_TK_NUM); ut_i++)  //�ų���ߺ���ͽ���,��߽���ִ�������ʱ��ι��
    {
        dec = __TKDeclare + ut_i;
        if((dec->ktask == __NULL) || (dec->stklen == 0) 
            || (dec->ktask->tbl == 0) || dec->ktask->msg == NULL)
        {                                   //���̴�����֧����Ϣ����
            continue;
        }
        
//	        if(ut_i == gs_TkTcbCur->tkid)       //��ǰ�����ų�����
//	        {
//	            continue;
//	        }
        
//	        gs_OS.Message_Send(MSG_LIVE, ut_i);
        krhino_buf_queue_send(dec->ktask->msg, &msg, 1);
        
        CPSR_ALLOC();
        RHINO_CPU_INTRPT_DISABLE();
                              //����̹�����
        gucs_LiveCnt[ut_i]++;
        RHINO_CPU_INTRPT_ENABLE();
        
        if(gucs_LiveCnt[ut_i] > CON_LIVE_TIME_OUT)			//3����δ�ظ�������Ϣ
        {
//	            SYS_Reset();
            LOG_DEBUG("soft watch dog thread %d could be died,sys will reboot!!! \n", ut_i);
            return SYS_ERR_FT;
        }
    }
    
    return SYS_ERR_OK;
}
#if 0
uint8_t HB_TaskLiveReq(int count)
{
                                            //�����ж�
    extern const KTaskDeclare __TKDeclare[SYS_TK_NUM];
    const KTaskDeclare* dec;
    uint8 msg = MSG_LIVE;
    if((count % 10) != 0)
    {
        for(uint32 ut_i = 0; ut_i < (SYS_TK_NUM); ut_i++) 
        {
            if(gucs_LiveCnt[ut_i] > 18)			//3����δ�ظ�������Ϣ
            {
                LOG_DEBUG("soft watch dog thread %d could be died,sys will reboot!!! \n", ut_i);
                return SYS_ERR_FT;
            }        
        }
        return SYS_ERR_OK;
    }
    for(uint32 ut_i = 0; ut_i < (SYS_TK_NUM); ut_i++)  //�ų���ߺ���ͽ���,��߽���ִ�������ʱ��ι��
    {
        dec = __TKDeclare + ut_i;
        if((dec->ktask == __NULL) || (dec->stklen == 0) 
            || (dec->ktask->tbl == 0) || dec->ktask->msg == NULL)
        {                                   //���̴�����֧����Ϣ����
            continue;
        }
        
//	        if(ut_i == gs_TkTcbCur->tkid)       //��ǰ�����ų�����
//	        {
//	            continue;
//	        }
        
//	        gs_OS.Message_Send(MSG_LIVE, ut_i);
        krhino_buf_queue_send(dec->ktask->msg, &msg, 1);
        
        CPSR_ALLOC();
        RHINO_CPU_INTRPT_DISABLE();
                              //����̹�����
        gucs_LiveCnt[ut_i]++;
        RHINO_CPU_INTRPT_ENABLE();
        
        if(gucs_LiveCnt[ut_i] > 18)			//3����δ�ظ�������Ϣ
        {
//	            SYS_Reset();
            LOG_DEBUG("soft watch dog thread %d could be died,sys will reboot!!! \n", ut_i);
            return SYS_ERR_FT;
        }
    }
    
    return SYS_ERR_OK;
}
#endif



/*******************************************************************************
 * @function_name:  HB_RetLive
 * @function_file:  
 * @����: �������̵ı����
 * 
 * 
 * @����:
 * 
 * @����: 
 * @����: yzy (2012-02-16)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
void HB_RetLive(uint8 tkid)
{
    
    CPSR_ALLOC();
    RHINO_CPU_INTRPT_DISABLE();
                             //����̹�����
    gucs_LiveCnt[tkid] = 0;
    RHINO_CPU_INTRPT_ENABLE();
}


/*******************************************************************************
 * @function_name:  HB_LiveInit
 * @function_file:  
 * @����: ���̱�����Ƴ�ʼ��
 * 
 * 
 * @����:
 * 
 * @����: 
 * @����: yzy (2012-02-16)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
void HB_LiveInit(void)
{
    ClearBuffer(gucs_LiveCnt, sizeof(gucs_LiveCnt));
}

uint8 Log_Send(uint8* buffer, uint16 len)
{
    return SER_SendData(UART_CHANNEL_DEBUG, buffer, len, 1000);
}



/*******************************************************************************
 * @function_name:  Main_PreInit
 * @function_file:  main.c
 * @����: Ӧ�ò�main������ʼ��
 * 
 * 
 * @����: 
 * 
 * @����: 
 * @����: yzy (2011-03-03)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
void Main_PreInit(void)
{
    gl_SysRunStt = 0;
    gl_SysRunStt |= HRS_PARALD;

}

/*******************************************************************************
 * @function_name:  SYS_MAIN_Init
 * @function_file:  main.c
 * @����: Ӧ�ò�main��ʼ��
 * 
 * 
 * @����: 
 * 
 * @����: 
 * @����: yzy (2011-03-03)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
void SYS_MAIN_Init(void)
{
   
      //ϵͳ״̬�Ĵ�����ʼ��
    
    gl_SysRunStt = 0;                      //����״̬��ʼΪ0
    //gub_EthPara_SetFlag = false;          //�������ȡ��־
    
    ClearBuffer((uint8*)&gs_ErrLog, sizeof(gs_ErrLog));
    
    for(uint8 ut_i = 0; ut_i < sizeof(gucs_LiveCnt); ut_i++)
    {
       gucs_LiveCnt[ut_i] = 0;				//������̼�������0
    }
    //aos_msleep(1000);
	
    
    gl_SysRunStt |= HRS_PARALD;
    gl_SysRunStt |= HRS_APPRUN;

//	    SYS_LED_BlinkSetAll1(0, 2, 1);
//	    gs_OS.TK_Sleep(200);
//	    SYS_LED_BlinkSetAll(0, 2, 1);
//	    gs_OS.TK_Sleep(20);
    SYS_Dev_OptBlinkSetAll(2, 0, 0, 0);               //���е�����
    gs_OS.TK_Sleep(200);
    
    SYS_Dev_OptBlinkSetAll(3, 0, 0, 0);               //���е���

    SYS_Dev_OptBlinkSet(GPIO_LED_RUN, 1, 10, 10, 0);    //���е�����(overlay last configuration)
//	    

     //SER_Open(UART_CHANNEL_DEBUG, TDB_MODE_R | TDB_MODE_W);
//	     LogInit(LOG_LEVEL_DEBUG,256, (log_fun_ptr *)Log_Send);
     //SER_Open(UART_CHANNEL_GPRS, TDB_MODE_R | TDB_MODE_W);
//	    gl_SysRunInit = 1;
}



extern uint8_t g_ucPutcharEn;

/*******************************************************************************
 * @function_name:  KeyProc
 * @function_file:  main.c
 * @����: ��������
 * 
 * 
 * @����: 
 * 
 * @����: 
 * @����: yzy (2014-07-28)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
void KeyProc(uint8 key)
{
    uint32 event;
    uint8 msg = 0;
    if(key == MSG_LIFEVT)
    {
        event = SYS_GPI_Event(GPI_TYPE_LIFEVT);  //���ٿ��½���״̬(�����ɿ�)
        
        if(event & 1)               //KEY1
        {
	        LOG_DEBUG("key 1 failing!\n");
            gs_SysVar.terstt.bit.DI0linked = 1; 
            
//	            gs_SysVar.AI0 = SYS_AD_GetValue(AD_ID_AI0)/100;
//	            gs_SysVar.AI1 = SYS_AD_GetValue(AD_ID_AI1)/100;

//	            LOG_DEBUG("AI0 = %d AI1 = %d !\n",gs_SysVar.AI0,gs_SysVar.AI1);
        }
        
        if(event & 2)               //KEY2
        {
            //�����Ͽ�����
//	            hal_gpio_output_low(&brd_gpio_table[GPO_BLE_UART_CTL]);
            gs_GPIO.GPO_Out(GPO_BLE_UART_CTL,false);
//            if((gs_SysVar.mLPstt & HLV_LPTASK_TST) == 0)
//                g_ucPutcharEn = 1;
            gs_SysVar.terstt.bit.blecheck = 0;
	        LOG_DEBUG("key 2 !\n");
        }
        
        if(event & 4)               //KEY3
        {
	        LOG_DEBUG("key 3 failing!\n");
            gs_SysVar.terstt.bit.DI1linked = 1; 
//	            msg = MSG_CARD_INSERT;
//	            krhino_buf_queue_send(&gs_MainQueue, &msg, 1);
        }
        
        if(event & 8)               //KEY4
        {
	
            LOG_DEBUG("key 4 failing!\n");
        }
        
        if(event & 0x10)               //KEY5
        {
        
            LOG_DEBUG("key 5 failing!\n");
        }
        
    }
    if(key == MSG_LILEVT)
    {
        event = SYS_GPI_Event(GPI_TYPE_LILEVT);  //���ٿ��½���״̬(�����ɿ�)
        
        if(event & 1)               //KEY1
        {
	        LOG_DEBUG("key 1 keep !\n");
        }
        
        if(event & 2)               //KEY2
        {

	        LOG_DEBUG("key 2 keep!\n");
        }
        
        if(event & 4)               //KEY3
        {
	        LOG_DEBUG("key 3 keep!\n");
        }
        
        if(event & 8)               //KEY4
        {
            LOG_DEBUG("key 4 keep!\n");
        }
        
        if(event & 0x10)               //KEY5
        {
            LOG_DEBUG("key 5 keep!\n");
        }        
    }  
    if(key == MSG_LIREVT)
    {
        event = SYS_GPI_Event(GPI_TYPE_LIREVT);  //���ٿ��½���״̬(�����ɿ�)
        
        if(event & 1)               //KEY1
        {
	        LOG_DEBUG("key 1 right!\n");
            gs_SysVar.terstt.bit.DI0linked = 0; 
        }
        
        if(event & 2)               //KEY2
        {
            //�����Ͽ�����
            g_ucPutcharEn = 0;
//	            hal_gpio_output_high(&brd_gpio_table[GPO_BLE_UART_CTL]);
            gs_GPIO.GPO_Out(GPO_BLE_UART_CTL,true);

	        LOG_DEBUG("key 2 right!\n");
        }
        
        if(event & 4)               //KEY3
        {
            LOG_DEBUG("key 3 right!\n");
	        gs_SysVar.terstt.bit.DI0linked = 0; 
        }
        
        if(event & 8)               //KEY4
        {
            LOG_DEBUG("key 4 right!\n");
        }
        
        if(event & 0x10)               //KEY4
        {
            LOG_DEBUG("key 5 right!\n");
        }
    }        
}


/************************************************************************
 * @function: SYS_MAIN_Reset
 * @����: 
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2013/7/10)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	void SYS_MAIN_Reset(void)
//	{
//	    gs_OS.TK_Sleep(200);                //�ȴ�2���ٸ�λ,��pst�������ݻظ����
//	    TDev* dev;
//	    dev = gs_DEVProc.DEV_Open(DEV_NO_SYS, 0 ,__NULL);
//	    ((TDevSys*)dev->devfunc)->Reset();
//	    gs_DEVProc.DEV_Close(dev);
//	}



/*******************************************************************************
 * @function_name:  MAIN_HardCheck
 * @function_file:  
 * @����: �ϵ��Լ�Ӳ��
 * 
 * 
 * @����:
 * 
 * @����: 
 * @����: yzy (2014-11-29)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
//	void MAIN_HardCheck()
//	{
//	    uint8 buffertst[10] = { 0x11, 0x22, 0x33, 0x00, 0x55, 0x66, 0x77, 0x88, 0xff, 0xaa};
//	    uint8 buffertemp[10];
//	    memset(buffertemp, 0x99, sizeof(buffertemp));
//	    gs_FarpVar.ftst = 0;
//	    
//	#if SYS_FRAM_EN > 0
//	    Fram_SafeRead((uint8 *)&gs_EventTime, FM_PUSH_TIME, sizeof(gs_EventTime));   //�������ȡ�ϴ�����ʱ��
//	    Fram_SafeWrite((uint8 *)&buffertst, FM_PUSH_TIME, sizeof(buffertst));       //����������д��flash
//	    Fram_SafeRead(buffertemp, FM_PUSH_TIME, sizeof(buffertemp));                //���������ݴ�flash�ж���
//	    Fram_SafeWrite((uint8 *)&gs_EventTime, FM_PUSH_TIME, sizeof(gs_EventTime));   //����������д��flash
//	#endif  
//	    WriteFlash(DB_PARA, buffertst, sizeof(buffertst), 100);                 //д�ⲿFlash
//	    ReadFlash(DB_PARA, buffertemp, sizeof(buffertemp), 100);                //���ⲿFlash
//	    if(CmpBuffer((uint8 *)&buffertst, buffertemp, 10) != 0)                     //���ݱȶԲ�һ��
//	    {
//	        gs_FarpVar.ftst |= 0x01;
//	    }
//	  
//	    PST_UartDownInteract();                //��ʱ����֡
//	    gul_PstInteractSecCnt = 0;      //���ڽ���������������0
//	}
//	


/*******************************************************************************
 * @function_name:  UpdateSysTimeStruct
 * @function_file:  main.c
 * @����: ����ϵͳʱ��
 * 
 * @����: 
 * 
 * @����: 
 * @����: yzy (2014-11-28)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
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
 * @����: �����ļ�У��
 * 
 * @����: 
 * 
 * @����: 
 * @����: yzy (2014-11-28)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
//	void MAIN_UpdataChkProc(void)
//	{
//	    if(gs_DlmsUpdata.FlagState == F_WaitVerify)
//	    {
//	        if(UPDATA_CheckCRC_BOOT())      //ִ���ļ�У��
//	        {
//	            gs_DlmsUpdata.FlagState=F_VerifyOk;  //У��ɹ�
//	            STR_SetBits(gs_FarpVar.MdAlt, 1, MDALM_UPDATA_OK, 1);   //ģ�������ɹ�
//	        }
//	        else
//	        {
//	            gs_DlmsUpdata.FlagState=F_VerifyFailed;  //У��ʧ��
//	            STR_SetBits(gs_FarpVar.MdAlt, 1, MDALM_UPDATA_BAD, 1);    //ģ������ʧ��
//	        }
//	        WriteFlashWithCRC(DB_UPDATA_PARA, (uint8 *)&gs_DlmsUpdata, sizeof(ST_DLMSUPDATA), PARA_ADDR_UPDATA);
//	    }
//	}



/*******************************************************************************
 * @function_name:  MAIN_MinProc
 * @function_file:  main.c
 * @����: �ִ���
 * 
 * @����: 
 * 
 * @����: 
 * @����: yzy (2014-11-28)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
//	void MAIN_MinProc(void)
//	{
//	    UpdateSysTimeStruct();
//	    //����Ƿ��д������ļ�
//	    if(gs_DlmsUpdata.FlagState == F_UpdataFirm)     //�������������־
//	    {
//	        if((gs_SysTime.mins >= gs_DlmsUpdata.UpdataTime) && (gs_DlmsUpdata.UpdataTime != 0))  //�������ʱ�䵽
//	        {
//	            ActionUpdataProc();     //ִ������
//	        }
//	    } 
//	}



/*******************************************************************************
 * @function_name:  MAIN_SecProc
 * @function_file:  main.c
 * @����: �봦��
 * 
 * @����: 
 * 
 * @����: 
 * @����: yzy (2014-11-28)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
//	void MAIN_SecProc(void)
//	{
//	    if(gs_GPIO.GPI_GetStt(1) & FGPI_STT_ENG)        //�е�
//	    {
//	        if(gs_SysVar.mDGstt & HLV_STT_NENG) //(1)
//	        {
//	            STR_SetBits(gs_FarpVar.MdAlt, 1, MDALM_POWER_ON, 1);
//	        }
//	        gs_SysVar.mDGstt &= ~HLV_STT_NENG;  //(2)
//	    }
//	    else                                    //ͣ��
//	    {
//	        if(!(gs_SysVar.mDGstt & HLV_STT_NENG)) //(1)
//	        {
//	                                            
//	            STR_SetBits(gs_FarpVar.MdAlt, 1, MDALM_POWER_ON, 1);
//	        }
//	        gs_SysVar.mDGstt |= HLV_STT_NENG;   //(2)
//	    }
//	}





static void timer_main(void *timer, void *arg)
{
//	    HB_TaskLiveReq();
//	    if(dispLockTime>0)
//	    {
//	        dispLockTime--;
//	        if(dispLockTime==0)
//	            dispRefresh();    //��ʾ����            
//	    }
//	    //����5������
//	    if((timertick%5)==0)
//	    {
//	        powRefresh();
//	        if(dispLockTime==0)
//	        {
//	            dispRefresh();
//	        }
//	    }
//	
//	    //����24Сʱ����
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
//    SYS_RTC_Init();
//    SYS_MSG_Init();
//    SYS_LED_Init(); 
    SYS_BlinkDev_Init();
//    HB_LiveInit();
////	    SYS_TIMER_Init();
//    Netp_PreInit(0);                    //�߳�
//
//    HLV_PreInit();                      //�߳�
//    Farp_PreInit();                     //�߳�
////	    PST_PreInit();                      //�߳�
//    Task_PreInit();                     //�߳�
//
//    PST_Init();
//
//    Main_PreInit();

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
 * @����:�����̴�����
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
#include <stdio.h>

#include <aos/kernel.h>
//#include <network/network.h>

//	#pragma pack(1)
//	typedef struct {
//	uint32_t aa[8];
//	uint8_t * bb;
//	}test_struct, *ptest_struct;
//extern IWDG_HandleTypeDef hiwdg;
int application_start(int argc, char *argv[])
{
    int count = 0;
//    extern LPTIM_HandleTypeDef hlptim1;
//    HAL_LPTIM_Counter_Start_IT(&hlptim1, 1000);  //����lp��ʱ����lsi 1s���
//
//#ifndef DEBUG        
//    HAL_IWDG_Refresh(&hiwdg); //ι��
//#endif
    SYS_APP_Init();
//    
//#ifndef DEBUG        
//    HAL_IWDG_Refresh(&hiwdg); //ι��
//#endif
//    
//    printf("nano entry here!\r\n");
//    SYS_TASKS_Init();
    

    while(1) {
//	        printf("hello world!!! count %d \r\n", count++);
        if(SYS_ERR_OK == HB_TaskLiveReq(count++))
#ifndef DEBUG        
            HAL_IWDG_Refresh(&hiwdg); //ι��
#else
            ;
#endif

        aos_msleep(CON_LIVE_SLEEP);
    };
}
void SYS_MAIN_Task(void * arg)
{
    TIME time;
                                            //������Ϣ
    //uint8 nmsg;                             //������Ϣ
    
    krhino_buf_queue_create(&gs_MainQueue, "main_queue",
                         gc_MQbuf, MSG_BUFF_LEN, BUFQUEUE_MSG_MAX);

    static uint8_t ble_name[14] = {'V','S'};
    ByteArrayBcdToHexString(gs_PstPara.Addr, ble_name+2, 6, 0);
    
    g_ucPutcharEn = 0;
//    HAL_BLE_Init(&gs_MainQueue, ble_name, 14);
//    if(SYS_GPI_GetStt(0) & 0x02)
//    {
//        g_ucPutcharEn = 0;
//    }
//    else
//    {
//        if((gs_SysVar.mLPstt & HLV_LPTASK_TST) == 0)
//            g_ucPutcharEn = 1;
//    }
//	    char tmp = 0;
//	char *ABC = &tmp;
//	
//	    //asm volatile("DCD ABC\n");
//	    asm volatile("mov %0, #0\n" : "=r"(ABC));
//	    asm volatile("add %0,%1, #1\n" : "=r"(ABC), "=r"(ABC));    
 //   asm ("mov ABC,#1");
 //       LDR     R1, =g_crash_steps
 //   LDR     R2, [R1]
//    ADD     R3, R2, #1
    SYS_MAIN_Init();
                                            //�����ȡʱ����Ϣ
//    SYS_MSG_Apply(TASK_MAIN_TKID, MSG_CLS_TM);//*��ʼ����ػ���
//    SYS_MSG_Apply(TASK_MAIN_TKID, MSG_CLS_GPIO);
//    SYS_MSG_ApplyExt(TASK_MAIN_TKID, MSG_CLS_UART, UART_CHANNEL_DEBUG);

    LOG_DEBUG("\nVS Project %s  Softver[%x] Hardver[%x]!!!\n", gucs_PrjCode, gul_UsrFuncVer, gul_UsrHardcVer);
    //Flash_Test();

    for(;;)
    {   
        krhino_buf_queue_recv(&gs_MainQueue, RHINO_WAIT_FOREVER, g_MQ_buf_recv,
                          &g_MQ_recv_size);
        switch(g_MQ_buf_recv[0])
        {
            case MSG_SEC:
                //SYS_WDT_Feed(0xFF);          //*ι��
//	                MAIN_SecProc();
//	                SYS_ReadDateTime(&time);
//	                LOG_DEBUG("second !\n");
                break;
                
            case MSG_MIN:
//	                HB_TaskLiveReq();           //*�����������
//                gs_SysVar.mLPstt |= HLV_LPTASK_SMSG_MAIN;

                LOG_DEBUG("%02d-%02d-%02d %02d:%02d:%02d %02d!\n",((TIME *)GetTime())->year, 
                    ((TIME *)GetTime())->month, ((TIME *)GetTime())->day, 
                    ((TIME *)GetTime())->hour, ((TIME *)GetTime())->min, 
                    ((TIME *)GetTime())->sec, ((TIME *)GetTime())->week);
                
//                HAL_BLE_Init_Delayed_Action(NULL);
//                gs_SysVar.mLPstt &= ~HLV_LPTASK_SMSG_MAIN;
                
//	                MAIN_MinProc();
                break;
                
            case MSG_LIFEVT:                //�����½���
                KeyProc(MSG_LIFEVT);
                time.day = 13;
                time.year = 18;
                time.month = 3;
                time.hour = 15;
                time.min = 23;
                time.sec = 30;
                //SYS_WriteDateTime(&time);
                break;
                
            case MSG_LIREVT:                //����������
                KeyProc(MSG_LIREVT);
                break;
                
            case MSG_LILEVT:                //��������
                KeyProc(MSG_LILEVT);
                break;
                
            case MSG_MAIN_RESET:            //Ӳ����λ
//	                SYS_MAIN_Reset();
                break;
                
            case MSG_MAIN_UPDATA_CHK:       //�����ļ�У��
//	                MAIN_UpdataChkProc();
                break;
                
            case MSG_MAIN_HARD_CHK:         //Ӳ���Լ�
//	                MAIN_HardCheck();
                break;
            case MSG_PSTP:                  //������Ϣ
//	                PST_RecvProc();
//                Setp_RecvProc();
                break;
            case MSG_CARD_INSERT:
                break;
            case MSG_LIVE:                  //�ظ�������Ϣ
                HB_RetLive(TASK_MAIN_TKID);
                break;
            case MSG_MAIN_BLE_CHK:
                
//                ByteArrayBcdToHexString(gs_PstPara.Addr, ble_name+2, 6, 0);
//                g_ucPutcharEn = 0;
//
//                HAL_BLE_Init(&gs_MainQueue, ble_name, 14);
//                if(SYS_GPI_GetStt(0) & 0x02)
//                {
//                    g_ucPutcharEn = 0;
//                }
//                else
//                {
//#ifndef DEBUG
//                    if((gs_SysVar.mLPstt & HLV_LPTASK_TST) == 0)
//#endif
//                        g_ucPutcharEn = 1;
//                }
            
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
**MAIN���̿鳣�����ֶ���
******************************************************************************/
const KTaskConst gs_TkMAIN = 
{
    &gs_MainHandle,
    SYS_MAIN_Task,
    gs_MainStack,
    TASK_MAIN_TKID,                           //���̱��
    true,                                    //��Ϣѭ�����ƽ���
    TASK_MAIN_STKL,
    "main",
    &gs_MainQueue,
};



//	int main(void)
//	{
//	    //PIOR = 0x05U;
//	    krhino_init();
//		
//	    bsp_init();
//	    
//	    extern void tick_init(uint32_t ticks_per_sec);
//	    tick_init(RHINO_CONFIG_TICKS_PER_SECOND);
//	    
//		
//	    
//	    SYS_TASKS_Init();
//	    
//	    krhino_start();
//	    
//	    return 0;
//	}



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
    HAL_IWDG_Refresh(&hiwdg); //ι��
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
    
    stm32_soc_peripheral_init();
//	//	    hal_rtc_init(&g_stRtcDev);
//	//	//	    brd_gpio_resume();
//	    at_reset_uart();
    
//	    gs_Uart.Init(PORT_UART_STD, NULL);
//    SYS_FILE_Start();
#ifndef DEBUG        
    HAL_IWDG_Refresh(&hiwdg); //ι��
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



