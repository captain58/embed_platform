/****************************************Copyright (c)**************************************************
**                                  ________科技有限公司
**                                          开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: dev_ble.c
**创   建   人: yzy
**最后修改日期: 2018年2月20日
**描        述: 蓝牙
**注        意:
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2018年2月20日
** 描　述: 原始版本
********************************************************************************************************/
/****************************************AT指令格式的说明***********************************************
1)Command Structure
Each AT command has the "AT" or “at” prefix string (except the commands A/ and +++). 
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

#define EXT_RFID

#include "sys.h"
#include "hal.h"
#include "bsp.h"
//#include <string.h>
#include "apis.h"
#include "tasks.h"

#if SYS_RFIDMODEM_EN == 0
    #error "没有使用蓝牙MODEM组件,返回错误!"
#endif

static TRFIDModem       gs_stBleModem;

/***********************************************************
 * @function_name: gs_BleMODMDrvIntf
 * @function_file: dev_modem.c
 * @描述:定义各个模块的各种处理函数
 * 
 * @参数: 
 * @返回: 
 * @作者:
 *---------------------------------------------------------
 * @修改人: yzy (2010/11/1)
 **********************************************************/
const TRFIDModemDrive gs_BleMODMDrvIntf[] =
{
//#if Modem_ICM522_EN > 0
    {
        icm522_modemcheck,
    
        icm522_modemon,
        icm522_modemoff,

        icm522_setname,
        icm522_getpagex,
        icm522_sleep,
    },
//#endif 
    
    

};
kbuf_queue_t * gst_bleQueue;



TRFIDModemState HAL_RFID_Status(void)
{
    return DevBleModem->stt;

}
/************************************************************************
 * @function: HAL_RFID_ShutDown
 * @描述: 关闭蓝牙模块
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2013/11/1)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/

uint8 HAL_RFID_ShutDown(void)
{
#if Modem_ICM522_EN > 0  
    if(DevBleModem == NULL || DevBleModem->drive == NULL || DevBleModem->stt.bit.chnrdy != 1)
        return SYS_ERR_FT;

    DevBleModem->drive->off();
    DevBleModem->stt.bit.chnrdy = 0;
#endif    
    return SYS_ERR_OK;
}
/************************************************************************
 * @function: HAL_RFID_Reset
 * @描述: 复位蓝牙模块
 * @参数: 
 * @返回: 0:成功；其他：失败
 * @说明: 
 * @作者: yzy (2013/11/1)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 HAL_RFID_Reset(void)
{
#if Modem_ICM522_EN > 0
    uint8_t sig = 99;
    if(DevBleModem == NULL || DevBleModem->drive == NULL || DevBleModem->stt.bit.typeChecked != 1)
        return SYS_ERR_FT;
    DevBleModem->drive->off();            //关闭模块
    aos_msleep(1000);
    DevBleModem->drive->on();

    if(DevBleModem->stt.bit.typeChecked == 1)
    {
        DevBleModem->stt.bit.chnrdy = 1;
    }
#else
    DevBleModem->stt.bit.typeChecked = 1;
    DevBleModem->stt.bit.chnrdy = 1;
#endif    
    return SYS_ERR_OK;
}


int HAL_RFID_Sleep(void)
{
#if Modem_ICM522_EN > 0
    if(DevBleModem == NULL || DevBleModem->drive == NULL || DevBleModem->stt.bit.typeChecked != 1)
        return SYS_ERR_FT;  
    DevBleModem->drive->sleep();
#endif    
    return 0;
}
/************************************************************************
 * @function: SYS_MODM_Check
 * @描述: 初始化蓝牙模块,自动判断模块类型
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2013/11/1)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
int HAL_RFID_Check(void)
{
	uint32 i = 0;                           //循环变量
#if Modem_ICM522_EN > 0
	while(true)                             //循环检测模块
	{
        aos_msleep(100);       //开机前延时
        
		gs_BleMODMDrvIntf[i].on();

		aos_msleep(100);   //开机后延时
		
		if(!gs_BleMODMDrvIntf[i].check())       //确认是否为当前选定的模块
		{
            
			DevBleModem->drive = (TRFIDModemDrive*)&gs_BleMODMDrvIntf[i];
            DevBleModem->stt.bit.typeChecked = 1;
			break;
		}

		i++;                                //循环检测下一个
		if(i >= sizeof(gs_BleMODMDrvIntf)/sizeof(TRFIDModemDrive))
		{
		    i=0;
            gs_BleMODMDrvIntf[i].off();
            return -1;
		}
		gs_BleMODMDrvIntf[i].off();            //关闭模块
        
	} 
                                            //发送消息表示需要初始化
#else
    DevBleModem->drive = (TRFIDModemDrive*)&gs_BleMODMDrvIntf[i];
    DevBleModem->stt.bit.typeChecked = 1;    
#endif    

	LOG_INFO("RFID Modem recognized! \nBegin to adv bluetooth\n");    
    return 0;
}
void HAL_RFID_Init_Delayed_Action(void *arg)
{
    if(DevBleModem->stt.bit.typeChecked != 1)
    {
        LOGD("RFID", "post init event \r\n");
        uint8_t msg = MSG_MAIN_RFID_CHK;
        krhino_buf_queue_send(gst_bleQueue, &msg, 1);
    }

}

/************************************************************************
 * @function: HAL_RFID_Init
 * @描述: 初始化蓝牙模块
 * @参数: 
 * @param:[in]queue 消息回调handle
 * @param: [in]name 蓝牙ID
 * @param:[in]len 蓝牙ID长度
 * @返回: 
 * @return: int 0:成功；其他：失败 
 * @说明: 
 * @作者: yzy (2013/11/1)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
int HAL_RFID_Init(kbuf_queue_t *queue, uint8_t *name, uint8_t len)
{
    int ret = 0;
    uint32_t linknum = 0;

    SerialSets ss;
    ss.baudrate = 19200;
    ss.parit = Parit_N;
    ss.databits = DataBits_8bits;
    ss.stopbits = StopBits_1;

    SYS_SER_Init(UART_CHANNEL_CARD, (void *)&ss);    //初始化串口   


    ClearBuffer(gucs_RFIDMAC, sizeof(gucs_RFIDMAC));
    
    ClearBuffer((uint8_t *)&gs_stBleModem, sizeof(gs_stBleModem));
    gs_stBleModem.drive = NULL;

    DevBleModem=(TRFIDModem*)&gs_stBleModem;

    gst_bleQueue = queue;


    ret = HAL_RFID_Check();
    if (ret) {
        LOG_INFO( "%s %d failed \r\n", __func__, __LINE__);
        DevBleModem->stt.bit.chnrdy = 0;
    
        gs_SysVar.terstt.bit.blestt = ~DevBleModem->stt.bit.chnrdy;
//	        aos_post_delayed_action(15000, ble_init_delayed_action, NULL);
        goto err;
    }
//    ret = DevBleModem->drive->set_name(name,len);
//    if (ret) 
//    {
//        LOG_INFO( "%s %d failed \r\n", __func__, __LINE__);
//        DevBleModem->stt.bit.chnrdy = 0;
//        gs_SysVar.terstt.bit.blestt = ~DevBleModem->stt.bit.chnrdy;
//        
//        goto err;
//    }
    DevBleModem->stt.bit.chnrdy = 1;
    
    DevBleModem->drive->sleep();
    return 0;
err:

    return -1;
}


int HAL_RFID_Send(        uint8_t *data, uint32_t len, int32_t timeout)
{
#if Modem_ICM522_EN > 0
    return SER_SendData(UART_CHANNEL_CARD, data, len,timeout);
#else
    return 0;
#endif    
}

int HAL_RFID_ReadCardID(        uint8_t *data, uint8_t len)
{
#if Modem_ICM522_EN > 0  
    SerialSets ss;
    ss.baudrate = 19200;
    ss.parit = Parit_N;
    ss.databits = DataBits_8bits;
    ss.stopbits = StopBits_1;

    SYS_SER_Init(UART_CHANNEL_CARD, (void *)&ss);    //初始化串口  

    if(DevBleModem->stt.bit.chnrdy)
    {
        if(0 == DevBleModem->drive->get_pagex(0, data, &len))
        {
            DevBleModem->stt.bit.linked = 1;
            memcpy(DevBleModem->card_id, data, len);
            DevBleModem->card_len = len;
//	            DevBleModem->drive->sleep();
            return len;
        }
        else
        {
            DevBleModem->stt.bit.linked = 0;
            return -1;
        }
    }
    return -1;
#else
    return 12;
#endif    
}
int HAL_RFID_GetCardID(uint8_t *data)
{
#if Modem_ICM522_EN > 0  
    memcpy(data, DevBleModem->card_id, DevBleModem->card_len);

    return DevBleModem->card_len;
#else
    return 12;
#endif    
}




