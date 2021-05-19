#include <k_api.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commModule_M72D.h"
#include "communication_gprs.h"
#include "hwCtrl.h"



static enum
{
    eStatusCommApp_Idle,    //空闲
    eStatusCommApp_LinkUp,  //连接上告
    eStatusCommApp_WaitCmd, //等待指令
}eStatusCommApp;


//指令接收缓存
static uint8_t cmdRecvBuf[256];

//测试用的上告指令
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

/**
 * 解析接收到的指令 
 * 解析指令的处理方法也在此函数内完成 
 * 
 * @author Zzzzz (2018/4/1)
 * 
 * @param tBuf 接收到的指令缓存指针
 * @param tLen 接收到的指令长度
 */
void commDealCmd(uint8_t *tBuf,uint16_t tLen)
{
    if(strstr((char *)tBuf,"CLOSED")!=NULL)
    {
        eStatusCommApp=eStatusCommApp_Idle;
        hwUartChanSend(UART_CHAN_DEBUG,"Close\r\n",8);
    }
    else
    {
        hwUartChanSend(UART_CHAN_DEBUG,"GetCmd\r\n",8);
    }
}



/**
 * 动连接后数据上报 
 * 连接上服务器后自动上报数据
 * 
 * @author Zzzzz (2018/4/1)
 * 
 * @param void 
 */
void communication_report(void)
{
    if(eStatusCommApp==eStatusCommApp_Idle)
    {
        eStatusCommApp=eStatusCommApp_LinkUp;
    }
    else if(eStatusCommApp==eStatusCommApp_WaitCmd)
    {
        //发送上报数据
        devSendStr((uint8_t *)cmdUpBuf,sizeof(cmdUpBuf),50);
    }
}

/**
 * 关闭连接
 * 
 * @author Zzzzz (2018/4/1)
 * 
 * @param void 
 */
void communication_close(void)
{
    eStatusCommApp = eStatusCommApp_Idle;
}

stu_dev_para devPara;
/**
 * 通信应用主线程
 * 
 * @author Zzzzz (2018/4/1)
 * 
 * @param arg 
 */
void commApp_task(void *arg)
{
    uint16_t tLen;
    while(1)
    {
        switch(eStatusCommApp)
        {
        case eStatusCommApp_Idle:
            devDeinit();
            krhino_task_sleep(10);
            break;
        case eStatusCommApp_LinkUp://连接上后直接发送上报数据
            
            //测试用的IP地址及端口
            devPara.Ip[0]=115;
            devPara.Ip[1]=236;
            devPara.Ip[2]=33;
            devPara.Ip[3]=164;
            devPara.NetPort=8013;

            devInit(&devPara);
            while(devIsReady()!=1)
            {
                if(devIsReady()==0xff)
                {
                    devDeinit();
                    krhino_task_sleep(50);
                    devInit(&devPara);
                }
                krhino_task_sleep(10);
            }
            //发送上报数据
            devSendStr((uint8_t *)cmdUpBuf,sizeof(cmdUpBuf),50);
            eStatusCommApp=eStatusCommApp_WaitCmd;
            krhino_task_sleep(10);
            break;
        case eStatusCommApp_WaitCmd:
            tLen=devRecvStr(cmdRecvBuf,256,10);
            if(tLen>0)
                commDealCmd(cmdRecvBuf,tLen);
            krhino_task_yield();
            break;
        default:
           krhino_task_sleep(10);
           break;
        }
    }
}



