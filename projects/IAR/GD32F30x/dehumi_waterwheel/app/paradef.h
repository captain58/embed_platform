/****************************************Copyright (c)**************************************************
**                                  ____科技有限公司
**                                          开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: paradef.h
**创   建   人: yzy
**最后修改日期: 2019-06-18
**描        述: 参数配置定公共头文件
**注        意:
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2019-06-18
** 描　述: 原始版本
********************************************************************************************************/
#ifndef _PARADEF_H_
#define _PARADEF_H_




/************************************************************************ 
**参数长度排布宏(总长度)
*************************************************************************/
//	#define  F1_PLEN        6
//	#define  F2_PLEN        33
//	#define  F3_PLEN        28
//	#define  F4_PLEN        16
//	#define  F5_PLEN        3 
//	#define  F6_PLEN        16
//	#define  F7_PLEN        64
//	#define  F8_PLEN        8 
//	#define  F9_PLEN        16
//	#define  F10_PLEN       6000
//	#define  F11_PLEN       11
//	#define  F12_PLEN       2 
//	#define  F13_PLEN       7 
//	#define  F14_PLEN       41
//	#define  F15_PLEN       37
//	#define  F16_PLEN       64
//	#define  F17_PLEN       2 
//	#define  F18_PLEN       12
//	#define  F19_PLEN       1 
//	#define  F20_PLEN       1 
//	#define  F21_PLEN       49
//	#define  F22_PLEN       193
//	#define  F23_PLEN       3 
//	#define  F25_PLEN       11
//	#define  F26_PLEN       57
//	#define  F27_PLEN       24
//	#define  F28_PLEN       4
//	#define  F29_PLEN       12
//	#define  F30_PLEN       1
//	#define  F31_PLEN       121
//	#define  F33_PLEN       110
//	#define  F34_PLEN       19
//	#define  F35_PLEN       41
//	#define  F36_PLEN       4
//	#define  F37_PLEN       19
//	#define  F38_PLEN       33          //8448
//	#define  F39_PLEN       33          //8448
//	#define  F41_PLEN       17
//	#define  F42_PLEN       6
//	#define  F43_PLEN       1
//	#define  F44_PLEN       8
//	#define  F45_PLEN       1
//	#define  F46_PLEN       4
//	#define  F47_PLEN       17
//	#define  F48_PLEN       1
//	#define  F49_PLEN       1
//	#define  F57_PLEN       3
//	#define  F58_PLEN       1
//	#define  F59_PLEN       5
//	#define  F60_PLEN       80
//	#define  F65_PLEN       (TASK_PARA_LEN + 9)
//	#define  F66_PLEN       (TASK_PARA_LEN + 9)
//	#define  F67_PLEN       1
//	#define  F68_PLEN       1
//	#define  F70_PLEN       6
//	#define  F91_PLEN       10
//	
//	
//	
//	#define  F149_PLEN      1
//	#define  F150_PLEN      512
//	#define  F239_PLEN      6000
//	#define  F241_PLEN      1
//	#define  F246_PLEN      13
//	
//	#define  F251_PLEN      4 
//	#define  F252_PLEN      16
//	#define  F253_PLEN      6
//	#define  F254_PLEN      2
//	#define  F255_PLEN      4
//	#define  F256_PLEN      8
//	
//	
/************************************************************************ 
**参数地址排布宏
*************************************************************************/
//************************自定义参数***************************************
#define F70_PADDR             0        //MAC地址
#define F251_PADDR            10       //终端逻辑地址
#define F252_PADDR            20       //终端通讯密码
#define INPADDR               30       //生产序列号,部规地址
#define ZKVADDR               40       //客户化版本(4B HEX)
#define K_VCF_PUL             50       //校表时的实时脉冲数
#define K_VCF_VOL             60       //校表时的实时电压
	
#define VER_HEXING            70       //采集内测软件版本号
#define NETP_CHECK            90       //协议匹配初始化标记
#define REGISTER_FLAG         94
#define PARENT_ADDR                 100
#define WATER_CTRL_PARA_ADDR        120

//********************************************************************
//空余

//********************************************************************
#define PREPAYPARA_PLEN     200
#define RECHARGE_PLEN     (27*8)
#define PRICE_PLEN     (27*8)

#define     PARA_COPY_INTERVAL_DLEN 2
#define     NETP_FW_PLEN      (sizeof(ST_NETP_FW_BASE)) 
#define     NETP_FW_PART_PLEN      (sizeof(ST_Netp_FW)) 

#define     PREPAYPARA_PER_LEN 25
//预留64只表位置
#define     F10_PLEN        (64+2+64)
#define     PARA_GPRS_PLEN        96
#define     PARA_FTP_PLEN        232
#define     PARA_HB_LEN         2

#define F10_PADDR               200       //表配置  8*8 = 64
#define PARA_COPY_INTERVAL      (F10_PADDR+F10_PLEN)        
#define PARA_GPRS               (PARA_COPY_INTERVAL+PARA_COPY_INTERVAL_DLEN)        
#define PARA_FTP                (PARA_GPRS+PARA_GPRS_PLEN)        
#define PARA_HB                 (PARA_FTP+PARA_FTP_PLEN)      ///心跳  

#define PREPAYPARA_PADDR        1000       //预付费

#define RECHARGE_PADDR          (PREPAYPARA_PADDR+PREPAYPARA_PLEN)        //预付费

#define PRICE_PADDR             (RECHARGE_PADDR+RECHARGE_PLEN)        //预付费




#define NETP_FW_PADDR       0

#define     RUNPARA_FTP_PLEN        sizeof(S_FTPPARA)

#define     RUNPARA_FTP     0       

//	#define F31_PADDR           50400       //载波从节点附属节点(SMAX<121,暂用过多空间,先只支持单个测量点配置)
//	
//	
//	//************************任务参数***************************************
//	#define F67_PADDR           51000       //1类任务启停
//	#define F68_PADDR           51100       //2类任务启停
//	#define F65_PADDR           51200       //1类任务参数
//	#define F66_PADDR          117600       //2类任务参数
//	#define FXX_PADDR          184000       //END
//	
//	#define F239_PADDR          184000       //电能表/交采配置 密码  132*24 = 3168

#define CON_WATER_TANK_STT_ERR         0
#define CON_WATER_TANK_STT_LOW         1
#define CON_WATER_TANK_STT_LOW_MID     2
#define CON_WATER_TANK_STT_HIGH_MID    3
#define CON_WATER_TANK_STT_HIGH        4
#define CON_WATER_TANK_STT_HIGH_MORE   5


#define CON_STT_SWITCH_OFFSET 0
#define CON_STT_CARD_OFFSET 1
#define CON_STT_CARD_ID_OFFSET 2
#define CON_STT_WATER_LEVEL 3

#define CON_MOTOR_STT_IDEL      0
#define CON_MOTOR_STT_PUMP      1
#define CON_MOTOR_STT_DRAIN     2
#define CON_MOTOR_STT_PAUSE     3

#define CON_MOTOR_CTRL_AUTO      0
#define CON_MOTOR_CTRL_MANUAL      1


#define CON_LCD_REMAIND_TIME_MAX    300 //5分钟
#define CON_SET_TIME_MAX    15

#define CON_REMAIN_SET_OFF      0
#define CON_REMAIN_SET_ON       1    

#define CON_TIME_SET_OFF      0
#define CON_TIME_SET_ON       1    

#define CON_TIME_SET_SITE_YEAR      0
#define CON_TIME_SET_SITE_YEARL     1

#define CON_TIME_SET_SITE_MON       2
#define CON_TIME_SET_SITE_MONL      3

#define CON_TIME_SET_SITE_DAY       4
#define CON_TIME_SET_SITE_DAYL      5

#define CON_TIME_SET_SITE_HOUR      6
#define CON_TIME_SET_SITE_HOURL     7

#define CON_TIME_SET_SITE_MIN       8
#define CON_TIME_SET_SITE_MINL      9

#define CON_TIME_SET_SITE_SEC       10
#define CON_TIME_SET_SITE_SECL      11

#endif

