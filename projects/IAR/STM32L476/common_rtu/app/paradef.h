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
//	/************************************************************************ 
//	**参数地址排布宏
//	*************************************************************************/
//	//************************自定义参数***************************************
//	#define F70_PADDR             0        //MAC地址
//	#define F251_PADDR            10       //终端逻辑地址
//	#define F252_PADDR            20       //终端通讯密码
//	#define INPADDR               30       //生产序列号,部规地址
//	#define ZKVADDR               40       //客户化版本(4B HEX)
//	#define K_VCF_PUL             50       //校表时的实时脉冲数
//	#define K_VCF_VOL             60       //校表时的实时电压
//	
//	#define VER_HEXING            70       //采集内测软件版本号
//	
//	//************************终端参数***************************************
//	#define F1_PADDR            100         //终端通信参数(LEN=6)
//	#define F2_PADDR            150         //中继转发配置(MAX<50)
//	#define F3_PADDR            200         //主站IP地址和端口(LEN=28)
//	#define F4_PADDR            250         //主站和短信中心号码(LEN=16)
//	#define F5_PADDR            300         //消息认证参数(LEN=3)
//	#define F6_PADDR            350         //组地址(MAX<50)
//	#define F7_PADDR            400         //终端IP地址和端口号(MAX<100)
//	#define F8_PADDR            500         //终端上行通信工作方式(LEN=8)
//	#define F9_PADDR            550         //事件记录配置(LEN=16)
//	#define F11_PADDR           600         //脉冲量配置(MAX<350)
//	#define F12_PADDR           950         //状态量配置(LEN=2)
//	#define F13_PADDR           960         //模拟量配置(MAX<=7)
//	#define F14_PADDR           1000        //总加组配置(MAX<700)
//	#define F15_PADDR           1700        //差动组越限参数(MAX<100)
//	#define F16_PADDR           1800        //虚拟专网用户名\密码(LEN=64)
//	#define F17_PADDR           1900        //保安定值(LEN=2)
//	#define F18_PADDR           1950        //功控时段(LEN=12)
//	#define F19_PADDR           2000        //时段控浮动系数(LEN=1)
//	#define F20_PADDR           2050        //月电控浮动系数(LEN=1)
//	#define F21_PADDR           2100        //费率时段和费率数(LEN=49)
//	#define F22_PADDR           2200        //费率(MAX<200)
//	#define F23_PADDR           2400        //催费告警允许/禁止(LEN=3)
//	
//	
//	//************************测量点参数***************************************
//	#define F25_PADDR           2500        //测量点基本参数(SLEN=11,STST=192)
//	#define F26_PADDR           4700        //测量点限值参数(SLEN=57,STST=192)
//	#define F27_PADDR          16000        //铜损,铁损参数(SLEN=24)
//	#define F28_PADDR          21000        //测量点功率因素分段限值(SLEN=4)
//	#define F30_PADDR          21800        //停抄设置(SLEN=1)
//	
//	
//	//************************终端参数***************************************
//	#define F33_PADDR           22000       //终端抄表参数(MAX<2200)
//	#define F34_PADDR           24400       //终端通信模块参数(MAX<200)
//	#define F35_PADDR           24600       //台区重点用户设置(MAX<100)
//	#define F36_PADDR           24700       //终端通信流量(LEN=2)
//	#define F37_PADDR           24750       //终端级联通信参数(MAX<100)
//	#define F38_PADDR           24900       //1类数据配置设置(MAX< (33*16*16))
//	#define F39_PADDR           34000       //2类数据配置设置(MAX< (33*16*16))
//	
//	
//	//************************终端参数***************************************
//	#define F57_PADDR           43000       //声音告警使能(LEN=3)
//	#define F58_PADDR           43050       //允许连续无通信时间(LEN=1)
//	#define F59_PADDR           43060       //电能表异常判别阀值(LEN=4)
//	#define F60_PADDR           43070       //谐波限值(LEN=80)
//	
//	
//	//************************终端参数***************************************
//	#define F91_PADDR           43200       //终端地位置信息(LEN=10)
//	#define F150_PADDR          43250       //测量点状态(LEN = 512)
//	#define F241_PADDR          44000       //电能表通信参数自动维护功能开启与关闭(AFN05F149)(LEN = 1)
//	#define F246_PADDR          44050       //集中器对表计定时对时设置(AFN04F246)(LEN = 7)
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

#define F10_PADDR               100       //表配置  8*8 = 64
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












#endif

