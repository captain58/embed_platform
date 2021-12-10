/*
 * Copyright (C) 2015-2017 Captain Group Holding Limited
 */


#include "sys.h"
#include "hal.h"
#include "bsp.h"
//#include <string.h>
#include "apis.h"
#include "tasks.h"
//#include "uartcfg.h"


#define TAG "ICM522 "



//extern gpio_dev_t brd_gpio_table[];
#define ICM522_DEFAULT_CMD_LEN    64
#define ICM522_DEFAULT_RSP_LEN    64
#define ICM522_SET_MODE_RSP_LEN    10




#define ICM522_RETRY_MAX          3



void HAL_BLE_Check(void);





int uart_send_wait_reply(const char *cmd, int cmdlen, uint8_t delimiter,
                       const char *data, int datalen,
                       char *replybuf, int bufsize)
{

    SYS_SER_Clear(UART_CHANNEL_CARD);
    if (SYS_SER_Write(UART_CHANNEL_CARD, cmd, cmdlen,1000) != 0) {
        return -1;
    }

    if (data && datalen) {

        if (SYS_SER_Write(UART_CHANNEL_CARD, data, datalen,1000) != 0) {
            return -1;
        }
    }
    
    msleep(100);
    if (SYS_SER_Read(UART_CHANNEL_CARD,replybuf,bufsize,350) <=  0) {
        return -1;
    }

    return 0;
}

static int uart_send_with_retry(const char *cmd, int cmdlen, bool delimiter, 
                                    const char *data,int datalen, char *rspbuf, int buflen, 
                                    const char *expectrsp, int exprctlen, uint8_t times)
{
    int retry = 0;
    int len = 0;
    if (NULL == cmd || 0 == cmdlen || NULL == rspbuf ||
        0 == buflen || NULL == expectrsp) {
        LOG_INFO( "Invalid input %s %d\r\n", __func__, __LINE__);
        return -1;
    }

    while(true) {
        retry++;
        memset(rspbuf, 0, buflen);
        len = uart_send_wait_reply(cmd, cmdlen, delimiter, data, datalen, rspbuf, buflen);
        if (memcmp(rspbuf, expectrsp, exprctlen) != 0) {
            if (retry > times) {
                return -1;
            }

            LOG_INFO( "%s %d failed retry count %d\r\n", __func__, __LINE__, retry);
            LOG_DUMPHEX(LOG_LEVEL_INFO, "rsp:\n", rspbuf, len);
            aos_msleep(100);
        } else {
            break;
        }
    }

    return 0;
}


int icm522_uart_selfadaption(const char *command, uint32_t cmdlen, const char *rsp, uint32_t rsplen)
{
    char buffer[ICM522_DEFAULT_RSP_LEN] = {0};

    if (NULL == command || NULL == rsp || 0 == rsplen) {
        LOG_INFO("invalid input %s %d\r\n", __FILE__, __LINE__);
        return -1;
    }

    if (uart_send_with_retry(command, cmdlen, false, NULL, 0,
        buffer, ICM522_DEFAULT_RSP_LEN, rsp, rsplen, ICM522_RETRY_MAX) < 0) {
        return -1;
    }

    return 0;
}

const uint8 closeRfid[] = {0,0, 0x03, 0x02, 0x00, 0x01};
const uint8 openRfid[] = {0,0, 0x03, 0x02, 0x01, 0x00};

//mode 0:关闭天线和寻卡 1:打开天线
void icm522_modem_set_mode(uint8_t mode)
{

//    int ret = 0;
//    int m = 0;
//    uint8_t cmd[ICM522_SET_MODE_RSP_LEN] = {0,0, 0x03, 0x02, 0x03, 0x02};
//    uint8_t rsp_result[4] = {0xfe,0x02,0x02,0x00};
//    uint8_t rsp[ICM522_SET_MODE_RSP_LEN];
//
//    if(mode)
//    {
//        memcpy(cmd, openRfid, sizeof(openRfid));
//    }
//    else
//    {
//        memcpy(cmd, closeRfid, sizeof(closeRfid));
//    }
//
//    m=6;
//    memset(rsp,0,ICM522_SET_MODE_RSP_LEN);
//    if (uart_send_with_retry(cmd, m, false, NULL, 0,
//        rsp, ICM522_SET_MODE_RSP_LEN, rsp_result, 4, 0) < 0) 
//    {
//        ret = -1;
//    }
//    else
//    {
//
//    }

//    return ret;                            //返回成功
}

/************************************************************************
 * @function: icm522_modemon
 * @描述: 开机开机(执行后将占用串口)
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/11/15)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void icm522_modemon(void)

{    
  
  //初始值
//	    hal_gpio_output_low(&brd_gpio_table[GPO_BLE_PWR]);//_ATMODEM_ONOFF_0();
//	                                            //复位(或重新上电)
//	    aos_msleep(200);                                            
//	    hal_gpio_output_high(&brd_gpio_table[GPO_BLE_PWR]);//_ATMODEM_PWRON();
//	    //_ATMODEM_PORTOPEN();
//	                                            //开机时序
//	    aos_msleep(200);

}
/************************************************************************
 * @function: icm522_modemoff
 * @描述: 关机操作
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/11/15)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void icm522_modemoff(void)
{


    
//	    hal_gpio_output_low(&brd_gpio_table[GPO_BLE_PWR]);   
//	    aos_msleep(100);
}
/************************************************************************
 * @function: icm522_modemcheck
 * @描述: 查询模块标识是否匹配
 * @参数: 
 * 
 * @返回: 
 * @return: bool true成功,false失败(成功后不关闭串口,将以识别到的串口配置长期占用)
 * @说明: 
 * @作者: yzy (2014/11/15)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
int icm522_modemcheck(void)
{
    int ret = 0;
    uint8_t cmd[10] = {0,0, 0x03, 0x02, 0x03, 0x02};
    uint8_t rsp_result[10] = {0xfe,0x02,0x02,0x00};
    icm522_modem_set_mode(1);
    /* uart baudrate self adaption*/
    ret = icm522_uart_selfadaption(cmd, 6, rsp_result, 4);
    if (ret) {
        LOG_ERROR(TAG "icm522_uart_selfadaption fail \r\n");
        icm522_modem_set_mode(0);
        return ret;
    }
    icm522_modem_set_mode(0);
    return ret;                            //返回成功
}


int icm522_setname(uint8_t *name, uint8_t len)
{
    int ret = 0;
    int m = 0;
    uint8_t cmd[32] = {0x01,0xFC, 0x08, 0x00};
    uint8_t rsp_result[2] = {0x04,0xFC};
    uint8_t rsp[ICM522_DEFAULT_RSP_LEN];
    if(len > 20)
    {
        return -1;
    }
    
    icm522_modem_set_mode(1);

    cmd[3] = len;
    m+=4;
    memcpy(cmd+4, name, len);
    m+=len;
    memset(rsp,0,ICM522_DEFAULT_RSP_LEN);
    if (uart_send_with_retry(cmd, m, false, NULL, 0,
        rsp, ICM522_DEFAULT_RSP_LEN, rsp_result, 2, ICM522_RETRY_MAX) < 0) 
    {
        ret = -1;
    }
    if(ret == -1 || rsp[2] == 0x02)
    {
        ret = -1;
    }
//	    ret = icm522_uart_selfadaption(cmd, 4, rsp_result, 4);
//	    if (ret) {
//	        LOGE(TAG, "icm522_uart_selfadaption fail \r\n");
//	        icm522_modem_set_mode(0);
//	        return ret;
//	    }
    icm522_modem_set_mode(0);
    return ret;                            //返回成功
}
int icm522_getpagex(uint8_t pagex, uint8_t *data, uint8_t * len)
{
    int ret = 0;
    int m = 0;
//    uint8_t cmd[32] = {0x00, 0x00, 0x0A, 0x04, 0x00, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F};
    uint8_t cmd[32] = {0x00, 0x00, 0x03, 0x14, 0x04, 0x13, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F};
    uint8_t rsp_result[3] = {0xfe};
    uint8_t rsp[ICM522_DEFAULT_RSP_LEN];

    *len = 0;
    icm522_modem_set_mode(1);

//	    cmd[3] = len;
//	    m+=4;
//	    memcpy(cmd+4, name, len);
//	    m+=len;
//    m=13;
    m=6;
    memset(rsp,0,ICM522_DEFAULT_RSP_LEN);
    if (uart_send_with_retry(cmd, m, false, NULL, 0,
        rsp, ICM522_DEFAULT_RSP_LEN, rsp_result, 1, 0) < 0) 
    {
        ret = -1;
    }
    else
    {
        if(0x14 == rsp[2])
        {
            memcpy(data, &rsp[3], 16);
            *len = 16;
        }
        else
        {
            ret = -1;
        }
    }
//	    if(ret == -1 || rsp[2] == 0x02)
//	    {
//	        ret = -1;
//	    }
//	    ret = icm522_uart_selfadaption(cmd, 4, rsp_result, 4);
//	    if (ret) {
//	        LOGE(TAG, "icm522_uart_selfadaption fail \r\n");
//	        icm522_modem_set_mode(0);
//	        return ret;
//	    }
    icm522_modem_set_mode(0);
    return ret;                            //返回成功
}

/************************************************************************
 * @function: HAL_SAL_Check
 * @描述: 初始化Modem,自动判断模块类型
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2013/11/1)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	void HAL_SAL_Check(void)
//	{
//		uint32_t i = 0;                           //循环变量
//		
//		while(true)                             //循环检测模块
//		{
//	        aos_msleep(1000);       //开机前延时
//	        
//			ec20_modemon();
//	
//			aos_msleep(3000);   //开机后延时
//			
//			if(!ec20_modemcheck())       //确认是否为当前选定的模块
//			{
//	
//	            
//	//				DevModem->drive = (TModemDrive*)&gs_MODMDrvIntf[i];
//	//				DevModem->stt.bit.typeChecked = 1;
//				break;
//			}
//	
//	//			i++;                                //循环检测下一个
//	//			if(i >= sizeof(gs_MODMDrvIntf)/sizeof(TModemDrive))
//	//			{
//	//				i = 0;
//	//			}
//			ec20_modemoff();            //关闭模块
//	        
//		} 
//	
//	
//		LOG_INFO("Modem recognized! \nBegin to init AT channel!\n");    
//	    
//	}





