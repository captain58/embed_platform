/****************************************Copyright (c)**************************************************
**                                  ________�Ƽ����޹�˾
**                                          ������
**
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: atmodem.c
**��   ��   ��: yzy
**����޸�����: 2018��2��20��
**��        ��: ����è�豸����
**ע        ��:
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: yzy
** ��  ��: v1.0
** �ա���: 2018��2��20��
** �衡��: ԭʼ�汾
********************************************************************************************************/
/****************************************ATָ���ʽ��˵��***********************************************
1)Command Structure
Each AT command has the "AT" or ��at�� prefix string (except the commands A/ and +++). 
Each AT command has the suffix <CR> (except the commands A/ and +++). 
Example: AT+CSQ<CR> ATS24?<CR> An AT command line may contain one or more commands. 
Delimiters are used to separate the commands from each other. 
The delimiter is either a semicolon ";" or none, meaning space (basic commands). 
Example: ATS0=1V1Q0E0<CR> AT+IFC=0,0;+ICF=3,4;+CNMI=2,1,0,0,0<CR>

2) Results Code Structure
By default, the G610 responds with verbose response codes. 
The results code prefix is <CR><LF>. 
Example: <CR><LF>+CSQ: 99,99<CR><LF> 
<CR><LF>OK<CR><LF> 

3)The Unsolicited results code is same as the Results code.
********************************************************************************************************/

#define EXT_ATMODEM 

#include "public.h"
#include "k_types.h"
#include "k_api.h"
//	#include "lib_app.h"
#include "hal.h"

#include <sal_import.h>
#include "atmodem.h"
#include <stdio.h>
#include "atcmd_config_module.h"
#include "atparser.h"

#if SYS_MODEM_EN == 0
    #error "û��ʹ��MODEM���,���ش���!"
#endif
uint8_t inited = 0;

aos_mutex_t g_link_mutex;
aos_mutex_t g_domain_mutex;
aos_sem_t   g_domain_sem;
aos_sem_t   g_ftp_start_sem;

//char  g_pccmd[EC20_CONN_CMD_LEN];
netconn_data_input_cb_t g_netconn_data_input_cb;
static TModem       gs_stModem;

/***********************************************************
 * @function_name: gs_MODMDrvIntf
 * @function_file: dev_modem.c
 * @����:�������ģ��ĸ��ִ�����
 * 
 * @����: 
 * @����: 
 * @����:
 *---------------------------------------------------------
 * @�޸���: yzy (2010/11/1)
 **********************************************************/
const TModemDrive gs_MODMDrvIntf[] =
{
#if Modem_EC20_EN > 0
    {
        ec20_modemcheck,
    
        ec20_uart_init,
        ec20_modemon,
        ec20_modemoff,
        ec20_forcesoff,

        ec20_gprs_status_check,
        ec20_gprs_ip_init,
        ec20_gprs_got_ip,
        
        ec20_domaintoip,//(char *domain, char ip[16])
        ec20_start,//(sal_conn_t *conn)
        ec20_close,//(int fd, int32_t remote_port)
        ec20_send,//(int fd,
        ec20_ftp_status_check,
        ec20_ftp_start,
        ec20_ftp_get,
        ec20_ftp_close,
    },
#endif 
    
#if Modem_M72X_EN > 0
    {
        SYS_M72X_DecodeRecvCmd, 
        SYS_M72X_Check, 
        SYS_M72X_ModemMainLoop, 
        SYS_M72X_ProcRecvAtCommand, 
        SYS_M72X_ProcUnsolictData, 
        SYS_M72X_ModemOn,
        SYS_M72X_ModemOff,
        MODM_TYPE_M72X,
    },
#endif 
#if Modem_ELFIN_EG41A_EN > 0
    {
        elfin_eg41a_modemcheck,
    
        elfin_eg41a_uart_init,
        elfin_eg41a_modemon,
        elfin_eg41a_modemoff,
        elfin_eg41a_forcesoff,

        elfin_eg41a_gprs_status_check,
        elfin_eg41a_gprs_ip_init,
        elfin_eg41a_gprs_got_ip,
        
        elfin_eg41a_domaintoip,//(char *domain, char ip[16])
        elfin_eg41a_start,//(sal_conn_t *conn)
        elfin_eg41a_close,//(int fd, int32_t remote_port)
        elfin_eg41a_send,//(int fd,
        elfin_eg41a_ftp_status_check,
        elfin_eg41a_ftp_start,
        elfin_eg41a_ftp_get,
        elfin_eg41a_ftp_close,
    },
#endif 

};
kbuf_queue_t * gst_FarpQueue;


void SYS_MODM_Init(kbuf_queue_t *queue)
{
    gst_FarpQueue = queue;
}



/************************************************************************
 * @function: SYS_MODM_Status
 * @����: ��ȡģ��״̬
 * @����: 
 * 
 * @����: 
 * @return: TModemState  
 * @˵��: 
 * @����: yzy (2013/11/1)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
TModemState SYS_MODM_Status(void)
{
    return DevModem->stt;
}


uint8 SYS_MODM_Reset(uint8 way)
{
//	    _IF_TRUE_RETURN_Z(!DevModem->stt.bit.typeChecked, false); //δ���ģ��
//	    
//	    if(way == 0 || way == 2)
//	    {                                       //ǿ�ƽ������������еĲ���
//	//	        SYS_Message_Send(MSG_MODEM_ABORT, DEV_MODEM_TKID);
//	        uint8 msg = MSG_MODEM_ABORT;
//	        krhino_buf_queue_send(&gs_ModemQueue, &msg, 1);
//	        
//	    }
//	                                            //Ӳ����λ
//	    if(way == 1 || way == 2)
//	    {
//	        krhino_sem_take(&gs_ModemEvent, RHINO_WAIT_FOREVER);     //��ȡ������Դ
//	        
//	//	        guc_ResetAtTask = *(ktask_t *)krhino_cur_task_get();
//	        guc_IP1Task= krhino_cur_task_get();
//	        if(SYS_ERR_OK == SYS_MODM_Step(Modem_Reset))
//	        {
//	            krhino_task_suspend(krhino_cur_task_get());
//	            krhino_sem_give(&gs_ModemEvent); //�ͷ���Դ
//	            //���н��
//	        }
//	        else
//	        {
//	            krhino_sem_give(&gs_ModemEvent); //�ͷ���Դ
//	            return SYS_ERR_NRES;
//	        }
//	    }
    uint8_t sig = 99;
//    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/RHINO_CONFIG_TICKS_PER_SECOND);
    //aos_msleep(2000);
    if (!inited) {
        return SYS_ERR_FT;
    }


    DevModem->drive->doff();            //�ر�ģ��
    aos_msleep(1000);
    DevModem->drive->on();

    
    int ret = DevModem->drive->uart_init();
    if (ret) {
        LOG_INFO( "%s %d failed \r\n", __func__, __LINE__);
        DevModem->stt.bit.chnrdy = 0;
        goto err;
    }
    DevModem->stt.bit.chnrdy = 1;
    ret = DevModem->drive->status_check(&sig,(uint32_t)0xffff);
    if (ret) {
        LOG_INFO( "%s %d failed \r\n", __func__, __LINE__);
        DevModem->stt.bit.regtt = 0;
        goto err;
    }
    DevModem->stt.bit.regtt = 1;
    DevModem->stt.bit.signl = sig;

    ret = DevModem->drive->ip_init();
    if (ret) {
        LOG_INFO( "%s %d failed \r\n", __func__, __LINE__);
        DevModem->stt.bit.pppoe = 0;
        goto err;
    }

    DevModem->stt.bit.pppoe = 1;
    //      if(SYS_MODM_CreatePPP("cmnet","usr","pwd"))

    //      ret = DevModem->drive->get_ip(DevModem->ModemIp);

    ret = DevModem->drive->get_ip(DevModem->ModemIp);
    if (ret) {
        LOG_INFO( "%s %d failed \r\n", __func__, __LINE__);
        goto err;
    }

    return SYS_ERR_OK;
err:
    return SYS_ERR_FT;
}


uint8 SYS_MODM_Shutdown(void)
{
    if (!inited) {
        return SYS_ERR_FT;
    }

    DevModem->drive->doff();            //�ر�ģ��
    DevModem->stt.bit.chnrdy = 0;
    DevModem->stt.bit.regtt = 0;
    DevModem->stt.bit.signl = 0;
    DevModem->stt.bit.pppoe = 0;
    return SYS_ERR_OK;
}



/************************************************************************
 * @function: SYS_MODM_BeginCheck
 * @����: ����ģ��״̬
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2013/11/1)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8_t SYS_MODM_BeginCheck(void)
{
    
    uint8_t sig = 99;

	//δ��⵽ģ��,�����в�ѯ
    if (!inited) {
        return SYS_ERR_FT;
    }

	if(DevModem->stt.bit.typeChecked == 0)
    {
        return 99;
    }
//	    
//	    //�Ƿ��ȡ������ģ��Ĵ���
//	    if(!(DevModem->stt.bit.sninfo))
//	    {
//	        SYS_MODM_Step(Modem_Info);
//	    }
//	    
//	    SYS_MODM_Step(Modem_QAndReg);
//	    SYS_MODM_Step(Modem_IPState);
    int ret = DevModem->drive->status_check(&sig, 0x02);
    if (ret) {
        LOG_INFO( "%s %d failed \r\n", __func__, __LINE__);
        //DevModem->stt.bit.regtt = 0;
        return 99;
    }
    //DevModem->stt.bit.regtt = 1;
    DevModem->stt.bit.signl = sig;
    
    return sig;
}



/*******************************************************************************
** @function_name:  SYS_MODM_GetIP
** @function_file:  dev_modem.c
** @����: ��ȡ����GPRSģ������IP��ַ
** 
** 
** @����: 
** 
** @����: 
** @return:  uint32   
** @����: yzy (2012/6/20)
**-----------------------------------------------------------------------------
** @�޸���: 
** @�޸�˵��: 
*******************************************************************************/
uint8_t * SYS_MODM_GetIP(void)
{
//	    _IF_TRUE_RETURN_Z(DevModem->drive == __NULL, 0);
//	    if(!(DevModem->stt.bit.pppoe))
//	    {
//	        return 0;
//	    }
    return DevModem->ModemIp;
}

/************************************************************************
 * @function: SYS_MODM_Reinit
 * @����: ����ATͨ��
 * @����: 
 * @����: 
 * @˵��: ���ʱΪ����FARP���ƽ��̵���
 * @����: yzy (2013/11/1)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void SYS_MODM_Reinit(void)
{
    int ret = DevModem->drive->uart_init();
    if (ret) {
        LOG_INFO( "%s %d failed \r\n", __func__, __LINE__);
        DevModem->stt.bit.chnrdy = 0;
    }
    DevModem->stt.bit.chnrdy = 1;
}






/************************************************************************
 * @function: SYS_MODM_Check
 * @����: ��ʼ��Modem,�Զ��ж�ģ������
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2013/11/1)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void HAL_SAL_Check(void)
{
	uint32 i = 0;                           //ѭ������
	
	while(true)                             //ѭ�����ģ��
	{
        aos_msleep(1000);       //����ǰ��ʱ
        
		gs_MODMDrvIntf[i].on();

		aos_msleep(3000);   //��������ʱ
		
		if(!gs_MODMDrvIntf[i].check())       //ȷ���Ƿ�Ϊ��ǰѡ����ģ��
		{
            
			DevModem->drive = (TModemDrive*)&gs_MODMDrvIntf[i];
            DevModem->stt.bit.typeChecked = 1;
			break;
		}

		i++;                                //ѭ�������һ��
		if(i >= sizeof(gs_MODMDrvIntf)/sizeof(TModemDrive))
		{
			i = 0;
		}
		gs_MODMDrvIntf[i].off();            //�ر�ģ��
        
	} 
                                            //������Ϣ��ʾ��Ҫ��ʼ��


	LOG_INFO("Modem recognized! \nBegin to init AT channel!\n");    
    
}
/************************************************************************
 * @function: SYS_MODM_ClosePPP
 * @����: �Ͽ�pppͨ�� 
 * @����: 
 * 
 * @����: 
 * @return: TResult  
 * @˵��: 
 * @����: yzy (2013/11/6)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
TResult SYS_MODM_ClosePPP(void)
{
    if(!(DevModem->stt.bit.regtt))
    {
        return SYS_ERR_EXIST;
    }


    return SYS_ERR_OK;    
}
/************************************************************************
 * @function: SYS_MODM_CreatePPP
 * @����: ����PPPͨ��
 * 
 * @����: 
 * @param: apn 
 * @param: vname 
 * @param: vpassword 
 * 
 * @����: 
 * @return: TResult  
 * @˵��: 
 * @����: yzy (2013/11/6)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
TResult SYS_MODM_CreatePPP(string apn, string vname, string vpassword)
{

    if(0 == DevModem->drive->ip_init())
    {
        DevModem->stt.bit.pppoe = 1;
        return SYS_ERR_OK;
    }
    DevModem->stt.bit.pppoe = 0;

    return SYS_ERR_FT;   
}
/************************************************************************
 * @function: SYS_MODM_CloseIP
 * @����: �Ͽ�IPͨ�� 
 * 
 * @����: 
 * @param: channel Ҫ�Ͽ���ͨ��
 * 
 * @����: 
 * @return: TResult  
 * @˵��: 
 * @����: yzy (2013/11/6)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
TResult SYS_MODM_CloseIP(void)
{
//	    ModemStepCommand cmd;
//	    
//	    if(!(DevModem->stt.bit.regtt))
//	    {
//	        return SYS_ERR_NRES;                 //��ʾͨ��û�б���
//	    }
//	    
//	    krhino_sem_take(&gs_ModemEvent, RHINO_WAIT_FOREVER);         //��ȡ������Դ
//	
//	    if(DevModem->stt.bit.tcpcc)
//	    {
//	        cmd.command = Modem_TcpDiscnt;
//	    }
//	    else if(DevModem->stt.bit.udpcc)
//	    {
//	        cmd.command = Modem_UdpDiscnt;
//	    }
//	    else
//	    {
//	        krhino_sem_give(&gs_ModemEvent); 
//	        return SYS_ERR_FT;
//	    }
//	                                            //�������ݽ������
//	    guc_IP1Task = krhino_cur_task_get();
//	    if(SYS_ERR_OK == SYS_FIFO_Enqueue(&gs_FifoModemStep, &cmd))
//	    {
//	        krhino_task_suspend(krhino_cur_task_get());
//	        krhino_sem_give(&gs_ModemEvent);     //�ͷ���Դ
//	        return guc_IP1TaskRet;
//	    }
//	
//	    krhino_sem_give(&gs_ModemEvent);         //�ͷ���Դ
    return SYS_ERR_NRES;
}

int HAL_SAL_Init(void)
{
    int ret = 0;
    uint32_t linknum = 0;
    uint8_t sig = 99;
    uint8_t ip[4] = {0};
    ClearBuffer(gucs_ATMID, sizeof(gucs_ATMID));
    ClearBuffer(gucs_IMEI, sizeof(gucs_IMEI));
    ClearBuffer(gucs_CCID, sizeof(gucs_CCID));
    gul_localIP = 0;

    DevModem=(TModem*)&gs_stModem;

    if (inited) {
        LOG_INFO( "ec20 gprs module have already inited \r\n");
        return 0;
    }


    if (0 != aos_mutex_new(&g_link_mutex)) {
        LOG_INFO( "Creating link mutex failed (%s %d).", __func__, __LINE__);
        goto err;
    }

    if (0 != aos_mutex_new(&g_domain_mutex)) {
        LOG_INFO( "Creating link mutex failed (%s %d).", __func__, __LINE__);
        goto err;
    }

    if (0 != aos_sem_new(&g_domain_sem, 0)) {
        LOG_INFO( "Creating domain mutex failed (%s %d).", __func__, __LINE__);
        goto err;
    }

    if (0 != aos_sem_new(&g_ftp_start_sem, 0)) {
        LOG_INFO( "Creating ftp mutex failed (%s %d).", __func__, __LINE__);
        goto err;
    }

    HAL_SAL_Check();
    
    ret = DevModem->drive->uart_init();
    if (ret) {
        LOG_INFO( "%s %d failed \r\n", __func__, __LINE__);
        DevModem->stt.bit.chnrdy = 0;
        goto err;
    }
    DevModem->stt.bit.chnrdy = 1;
    ret = DevModem->drive->status_check(&sig,0xffff);
    if (ret) {
        LOG_INFO( "%s %d failed \r\n", __func__, __LINE__);
        DevModem->stt.bit.regtt = 0;
        goto err;
    }
    DevModem->stt.bit.regtt = 1;
    DevModem->stt.bit.signl = sig;

    ret = DevModem->drive->ip_init();
    if (ret) {
        LOG_INFO( "%s %d failed \r\n", __func__, __LINE__);
        DevModem->stt.bit.pppoe = 0;
        goto err;
    }
    
    DevModem->stt.bit.pppoe = 1;
//	    if(SYS_MODM_CreatePPP("cmnet","usr","pwd"))

//	    ret = DevModem->drive->get_ip(DevModem->ModemIp);
    
    ret = DevModem->drive->get_ip(DevModem->ModemIp);
    if (ret) {
        LOG_INFO( "%s %d failed \r\n", __func__, __LINE__);
        goto err;
    }
//	    memcpy(DevModem->ModemIp, ip, 4);
    inited = 1;

    return 0;
err:
    if (aos_mutex_is_valid(&g_link_mutex)) {
        aos_mutex_free(&g_link_mutex);
    }

    if (aos_mutex_is_valid(&g_domain_mutex)) {
        aos_mutex_free(&g_domain_mutex);
    }

    if (aos_sem_is_valid(&g_domain_sem)) {
        aos_sem_free(&g_domain_sem);
    }
    if (aos_sem_is_valid(&g_ftp_start_sem)) {
        aos_sem_free(&g_ftp_start_sem);
    }

    return -1;
}


int HAL_SAL_Deinit()
{
    if (!inited) {
        return 0;
    }

    aos_mutex_free(&g_link_mutex);
    aos_mutex_free(&g_domain_mutex);
    aos_sem_free(&g_domain_sem);
    
    aos_sem_free(&g_ftp_start_sem);
    inited = 0;
    return 0;
}

int HAL_SAL_DomainToIp(char *domain, char ip[16])
{
    return DevModem->drive->domaintoip(domain, ip);
}

int HAL_SAL_Start(sal_conn_t *conn)
{
    return DevModem->drive->start(conn);
}

int HAL_FTP_Check(uint8_t * usr, uint8_t * pwd)
{
    return DevModem->drive->ftp_check(usr,pwd);
}

int HAL_FTP_Start(uint8_t *domain, uint16_t port, uint8_t * fold)
{
    if(domain == NULL || fold == NULL || strlen(domain) > 128 || strlen(domain) > 128)
        return -1;
    
    return DevModem->drive->fstart(domain, port, fold);
}

int HAL_FTP_Get(uint8_t * file, uint8_t * outbuf, uint32_t offset, uint16_t * len)
{
    if(file == NULL || outbuf == NULL || strlen(file) > 128 ||  *len == 0)
        return -1;
    return DevModem->drive->ftp_get(file,outbuf,offset,len);
}

int HAL_FTP_Close(void)
{
    return DevModem->drive->ftp_close();
}

int HAL_SAL_Close(int fd, int32_t remote_port)
{
    return DevModem->drive->close(fd, remote_port);

}

int HAL_SAL_Send(int fd,
                 uint8_t *data,
                 uint32_t len,
                 char remote_ip[16],
                 int32_t remote_port,
                 int32_t timeout)
{
    return DevModem->drive->send(fd, data,len,remote_ip,remote_port,timeout);

}

int HAL_SAL_RegisterNetconnDataInputCb(netconn_data_input_cb_t cb)
{
    if (cb) {
        g_netconn_data_input_cb = cb;
    }
    return 0;
}




int sal_device_init(void)
{
    at_init();
    return 0;
}






