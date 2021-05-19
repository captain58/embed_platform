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

#define EXT_BLE

#include "public.h"
#include "k_types.h"
#include "k_api.h"
//	#include "lib_app.h"
#include "hal.h"

#include "lsb4bt.h"

#include "aos/yloop.h"
#if SYS_BLEMODEM_EN == 0
    #error "没有使用蓝牙MODEM组件,返回错误!"
#endif

static TBLEModem       gs_stBleModem;

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
 * @描述: 关闭蓝牙模块
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2013/11/1)
 *-----------------------------------------------------------------------
 * @修改人: 
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
 * @描述: 复位蓝牙模块
 * @参数: 
 * @返回: 0:成功；其他：失败
 * @说明: 
 * @作者: yzy (2013/11/1)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 HAL_BLE_Reset(void)
{

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
    return SYS_ERR_OK;
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
int HAL_BLE_Check(void)
{
	uint32 i = 0;                           //循环变量
	
	while(true)                             //循环检测模块
	{
        aos_msleep(100);       //开机前延时
        
		gs_BleMODMDrvIntf[i].on();

		aos_msleep(100);   //开机后延时
		
		if(!gs_BleMODMDrvIntf[i].check())       //确认是否为当前选定的模块
		{
            
			DevBleModem->drive = (TBLEModemDrive*)&gs_BleMODMDrvIntf[i];
            DevBleModem->stt.bit.typeChecked = 1;
			break;
		}

		i++;                                //循环检测下一个
		if(i >= sizeof(gs_BleMODMDrvIntf)/sizeof(TBLEModemDrive))
		{
		    i=0;
            gs_BleMODMDrvIntf[i].off();
            return -1;
		}
		gs_BleMODMDrvIntf[i].off();            //关闭模块
        
	} 
                                            //发送消息表示需要初始化


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






