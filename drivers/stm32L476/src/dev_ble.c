/****************************************Copyright (c)**************************************************
**                                  ________�Ƽ����޹�˾
**                                          ������
**
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: dev_ble.c
**��   ��   ��: yzy
**����޸�����: 2018��2��20��
**��        ��: ����
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

#define EXT_BLE

#include "public.h"
#include "k_types.h"
#include "k_api.h"
//	#include "lib_app.h"
#include "hal.h"

#include "lsb4bt.h"

#include "aos/yloop.h"
#if SYS_BLEMODEM_EN == 0
    #error "û��ʹ������MODEM���,���ش���!"
#endif

static TBLEModem       gs_stBleModem;

/***********************************************************
 * @function_name: gs_BleMODMDrvIntf
 * @function_file: dev_modem.c
 * @����:�������ģ��ĸ��ִ�����
 * 
 * @����: 
 * @����: 
 * @����:
 *---------------------------------------------------------
 * @�޸���: yzy (2010/11/1)
 **********************************************************/
const TBLEModemDrive gs_BleMODMDrvIntf[] =
{
#if Modem_LSB4BT_EN > 0
    {
        lsb4bt_modemcheck,
    
        lsb4bt_modemon,
        lsb4bt_modemoff,

        lsb4bt_setname,
                 
    },
#endif 
    
    

};
kbuf_queue_t * gst_bleQueue;



TBLEModemState HAL_BLE_Status(void)
{
    return DevBleModem->stt;
}
/************************************************************************
 * @function: HAL_BLE_ShutDown
 * @����: �ر�����ģ��
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2013/11/1)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/

uint8 HAL_BLE_ShutDown(void)
{
    if(DevBleModem == NULL || DevBleModem->drive == NULL || DevBleModem->stt.bit.chnrdy != 1)
        return SYS_ERR_FT;

    DevBleModem->drive->off();
    DevBleModem->stt.bit.chnrdy = 0;
    return SYS_ERR_OK;
}
/************************************************************************
 * @function: HAL_BLE_Reset
 * @����: ��λ����ģ��
 * @����: 
 * @����: 0:�ɹ���������ʧ��
 * @˵��: 
 * @����: yzy (2013/11/1)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 HAL_BLE_Reset(void)
{

    uint8_t sig = 99;
    if(DevBleModem == NULL || DevBleModem->drive == NULL || DevBleModem->stt.bit.typeChecked != 1)
        return SYS_ERR_FT;
    DevBleModem->drive->off();            //�ر�ģ��
    aos_msleep(1000);
    DevBleModem->drive->on();

    if(DevBleModem->stt.bit.typeChecked == 1)
    {
        DevBleModem->stt.bit.chnrdy = 1;
    }
    return SYS_ERR_OK;
}



/************************************************************************
 * @function: SYS_MODM_Check
 * @����: ��ʼ������ģ��,�Զ��ж�ģ������
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2013/11/1)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
int HAL_BLE_Check(void)
{
	uint32 i = 0;                           //ѭ������
	
	while(true)                             //ѭ�����ģ��
	{
        aos_msleep(100);       //����ǰ��ʱ
        
		gs_BleMODMDrvIntf[i].on();

		aos_msleep(100);   //��������ʱ
		
		if(!gs_BleMODMDrvIntf[i].check())       //ȷ���Ƿ�Ϊ��ǰѡ����ģ��
		{
            
			DevBleModem->drive = (TBLEModemDrive*)&gs_BleMODMDrvIntf[i];
            DevBleModem->stt.bit.typeChecked = 1;
			break;
		}

		i++;                                //ѭ�������һ��
		if(i >= sizeof(gs_BleMODMDrvIntf)/sizeof(TBLEModemDrive))
		{
		    i=0;
            gs_BleMODMDrvIntf[i].off();
            return -1;
		}
		gs_BleMODMDrvIntf[i].off();            //�ر�ģ��
        
	} 
                                            //������Ϣ��ʾ��Ҫ��ʼ��


	LOG_INFO("BLE Modem recognized! \nBegin to adv bluetooth\n");    
    return 0;
}
void HAL_BLE_Init_Delayed_Action(void *arg)
{
    if(DevBleModem->stt.bit.typeChecked != 1)
    {
        LOGD("BLE", "post init event \r\n");
        uint8_t msg = MSG_MAIN_BLE_CHK;
        krhino_buf_queue_send(gst_bleQueue, &msg, 1);
    }

}

/************************************************************************
 * @function: HAL_BLE_Init
 * @����: ��ʼ������ģ��
 * @����: 
 * @param:[in]queue ��Ϣ�ص�handle
 * @param: [in]name ����ID
 * @param:[in]len ����ID����
 * @����: 
 * @return: int 0:�ɹ���������ʧ�� 
 * @˵��: 
 * @����: yzy (2013/11/1)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
int HAL_BLE_Init(kbuf_queue_t *queue, uint8_t *name, uint8_t len)
{
    int ret = 0;
    uint32_t linknum = 0;

    ClearBuffer(gucs_BLEMAC, sizeof(gucs_BLEMAC));
    
    ClearBuffer((uint8_t *)&gs_stBleModem, sizeof(gs_stBleModem));
    gs_stBleModem.drive = NULL;

    DevBleModem=(TBLEModem*)&gs_stBleModem;

    gst_bleQueue = queue;


    ret = HAL_BLE_Check();
    if (ret) {
        LOG_INFO( "%s %d failed \r\n", __func__, __LINE__);
        DevBleModem->stt.bit.chnrdy = 0;
    
        gs_SysVar.terstt.bit.blestt = ~DevBleModem->stt.bit.chnrdy;
//	        aos_post_delayed_action(15000, ble_init_delayed_action, NULL);
        goto err;
    }
    ret = DevBleModem->drive->set_name(name,len);
    if (ret) 
    {
        LOG_INFO( "%s %d failed \r\n", __func__, __LINE__);
        DevBleModem->stt.bit.chnrdy = 0;
        gs_SysVar.terstt.bit.blestt = ~DevBleModem->stt.bit.chnrdy;
        
        goto err;
    }
    DevBleModem->stt.bit.chnrdy = 1;
    
    
    return 0;
err:

    return -1;
}


int HAL_BLE_Send(        uint8_t *data, uint32_t len, int32_t timeout)
{
    return SER_SendData(PORT_UART_BLE, data, len,timeout);
}






