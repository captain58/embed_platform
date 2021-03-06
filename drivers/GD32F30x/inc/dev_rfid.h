/****************************************Copyright (c)**************************************************
**                                  ________科技有限公司
**                                          开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: dev_ble.h
**创   建   人: yzy
**最后修改日期: 2010年3月5日
**描        述: 蓝牙操作
**注        意:
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2010年3月5日
** 描　述: 原始版本
********************************************************************************************************/


#ifndef _EXT_RFID_H_
#define _EXT_RFID_H_

#ifndef EXT_RFID
#define EXT_RFID extern
#endif

/*******************************************************************************
**模块交互过程的识别信息
********************************************************************************/
EXT_RFID uint8 gucs_RFIDMAC[6];              //蓝牙模块MAC








/*******************************************************************************
**表示无线猫处理函数
********************************************************************************/
typedef struct{
    int (*check)(void);                //确定是否为该类型的函数
    void (*on)(void);                   //打开模块的过程
    void (*off)(void);                  //关闭模块的过程
    int (*set_name)(uint8_t *, uint8_t);                //确定是否为该类型的函数
    int (*get_pagex)(uint8_t pagex, uint8_t *data, uint8_t * len);
}TRFIDModemDrive;


typedef union{
    uint32_t lword;
    struct{
        uint32 typeChecked:1;       //是否已识别模块类型
        uint32 chnrdy:1;            //AT指令是否完成基本配置并准备就绪
        uint32 linked:1;
    }bit;
}TRFIDModemState;


typedef struct __TRFIDModem{
//    TDev dev;
    const TRFIDModemDrive* drive;

    TRFIDModemState stt;
    uint16 card_len;
    uint8_t card_id[16];

}TRFIDModem;









/******************************************************************************
**用于记录发起复杂流程操作的应用层进程
*******************************************************************************/




/************************************************************************
 *无线猫参数,状态信息
 ************************************************************************/
EXT_RFID TRFIDModem* DevBleModem;             //无线猫参数,状态信息


int HAL_RFID_Init(kbuf_queue_t *queue, uint8_t * name, uint8_t len);

void HAL_RFID_Init_Delayed_Action(void *arg);


uint8 HAL_RFID_ShutDown();
uint8 HAL_RFID_Reset();

TRFIDModemState HAL_RFID_Status(void);
int HAL_RFID_GetCardID(uint8_t *data);

//	
//	
//	
//	/************************************************************************
//	 * @function: SYS_RFIDMODM_Send
//	 * @描述: 发送IP通道的数据
//	 * 
//	 * @参数: 
//	 * @param: ipdata 要发送的数据
//	 * @param: length 要发送的数据的长度
//	
//	 * 
//	 * @返回: 
//	 * @return: TResult  
//	 * @说明: 
//	 * @作者: yzy (2013/11/6)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
//	 ************************************************************************/
//	TResult SYS_RFIDMODM_Send(uint8* data, uint16 length);
//	
//	
//	
//	
//	/************************************************************************
//	 * @function: SYS_MODM_Reset
//	 * @描述: 复位AT通道 
//	 * 
//	 * @参数: 
//	 * @param: way 0:软件复位模块
//	               1:硬件复位模块
//	               2:软硬件都复位模块
//	 * 
//	 * @返回: 
//	 * @return: uint8  
//	 * @说明: 
//	 * @作者: yzy (2013/11/6)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
//	 ************************************************************************/
//	uint8 SYS_RFIDMODM_Reset(uint8 way);
//	
//	
//	
//	
//	
//	
//	/************************************************************************
//	 * @function: SYS_MODM_Status
//	 * @描述: 获取模块状态
//	 * @参数: 
//	 * 
//	 * @返回: 
//	 * @return: TModemState  
//	 * @说明: 
//	 * @作者: yzy (2013/11/1)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
//	 ************************************************************************/
//	TRFIDModemState SYS_RFIDMODM_Status(void);
//	
//	
//	
//	







#endif


