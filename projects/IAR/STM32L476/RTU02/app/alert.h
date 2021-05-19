/****************************************Copyright (c)**************************************************
**                                  ____科技有限公司
**                                          开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: GD_alert.h
**创   建   人: yzy
**最后修改日期: 2019-06-18
**描        述: 告警模块程序头文件
**注        意:
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2019-06-18
** 描　述: 原始版本
********************************************************************************************************/
#ifndef _GD_ALERT_H_
#define _GD_ALERT_H_

#ifndef EXT_GD_ALERT
#define EXT_GD_ALERT  extern
#endif





//集中器本地电压采集相关
#define NUM_SECVOL    6                     //15
EXT_GD_ALERT uint8  guc_SecDensIndex;
EXT_GD_ALERT uint32 gul_SecDens[NUM_SECVOL];     //用于计算分平均电压的秒脉冲密度
EXT_GD_ALERT uint32 gul_MinDens;            //分平均电压脉冲密度
EXT_GD_ALERT uint32 gul_MinVoltage;         //分平均电压值,hex4.2
EXT_GD_ALERT uint32 gul_vcfChkDens;         //校表时的脉冲采样密度
EXT_GD_ALERT uint32 gul_vcfChkVol;          //校表时的电压,hex4.3





/************************************************************************
**函数名称: uint8 ALRT_ReadLog(uint8* pm, uint8* pn, uint8* data, uint8 type)
**描 述: 获取告警数据
**
**输 入: pm 起始指针 (0-255)
         pn 结束指针 (0-255)
         data 数据存放缓存 
         type 0 重要事件  1 一般事件
**输 出: 获取数据的长度,最大数据长度不超过200字节
**
**备 注: 
**--------------------------------------------------------
**作 者: yzy
**日 期: 
*************************************************************************/
int ALRT_ReadLog(uint8* pm, uint8* pn, uint8* data, uint8 type);

/************************************************************************
**函数名称: void ALRT_ParaChg(uint8 host, uint8* pdi, uint8 di_no)
**描 述: ERC3 参数变更记录 
**
**输 入: host   主站地址
         pdi    参数变更DI表的指针
         di_no  DI数量(最多20,大于20则记录前20条)
**输 出: 
**
**备 注: 由外部帧处理进程调用
**--------------------------------------------------------
**作 者: yzy
**日 期: 
*************************************************************************/
void ALRT_ParaChg(uint8 host, uint8* pdi, uint8 di_no);



/************************************************************************
 * @function: ALRT_ERC8
 * @描述: 电能表参数变更
 * 
 * @参数: 
 * @param: tstno 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/2/9)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void ALRT_ERC8(uint8 tstno);


/************************************************************************
 * @function: ALRT_ERC12
 * @描述: 电能表时间超差
 * 
 * @参数: 
 * @param: tstno 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/2/11)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void ALRT_ERC12(uint8 tstno);


/************************************************************************
 * @function: ALRT_ERC13
 * @描述: 电表故障信息
 * 
 * @参数: 
 * @param: tstno 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/2/10)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void ALRT_ERC13(uint8 tstno);

/************************************************************************
 * @function: ALRT_ERC14
 * @描述: 上电事件记录
 * 
 * @参数: 
 * @param: type 0上电. 1停电
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/2/10)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void ALRT_ERC14(uint8 type);

/************************************************************************
 * @function: ALRT_ERC21
 * @描述: 终端故障记录
 * 
 * @参数: 
 * @param: port 抄表端口号
 * @返回: 
 * @说明: 支持项:单路485抄表故障.
 *        当单块表丢失时不清除事件发生标志,保留以备ERC31使用
 * @作者: yzy (2014/2/10)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void ALRT_ERC21(uint8 port);

/************************************************************************
 * @function: ALRT_ERC27_28_29_30
 * @描述: 电能量异常(示度下降,超差,飞走,停走)
 * 
 * @参数: 
 * @param: nWp 
 * @param: tstno 
 * @返回: 
 * @说明: 总清电能示值后 可能导致 该事件发生
 * @作者: yzy (2014/2/10)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void ALRT_ERC27_28_29_30(uint32 nWp, uint8 tstno);

/************************************************************************
 * @function: ALRT_ERC31
 * @描述: 485抄表失败事件
 * 
 * @参数: 
 * @param: tstno 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/2/10)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void ALRT_ERC31(uint8 tstno);


/************************************************************************
 * @function: ALRT_ERC33
 * @描述: 
 * 
 * @参数: 
 * @param: pnew 当前状态字 
 * @param: pold 之前状态字
 * @param: tstno 测量点
 * 
 * @返回: 
 * @return: uint8  true 成功 /false 失败(或不需记录)
 * @说明: 
 * @作者: yzy (2014/2/10)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 ALRT_ERC33(uint8* pnew, uint8* pold, uint8 tstno);

/************************************************************************
 * @function: ALRT_ERC37
 * @描述: 开表盖事件
 * 
 * @参数: 
 * @param: buffer 从发生时刻开始的详细信息.从表内获取
 * @param: cnt 开盖次数
 * @param: tstno 测量点号
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2014/2/11)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 ALRT_ERC37(uint8* buffer, uint32 cnt, uint8 tstno);

/************************************************************************
 * @function: ALRT_ERC38
 * @描述: 开端钮盒事件
 * 
 * @参数: 
 * @param: buffer 从发生时刻开始的详细信息.从表内获取
 * @param: cnt 开端钮盒次数
 * @param: tstno 测量点
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2014/2/11)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 ALRT_ERC38(uint8* buffer, uint32 cnt, uint8 tstno);
/************************************************************************
 * @function: ALRT_ERC39_46
 * @描述: 发现新表/删除电表 事件记录
 * 
 * @参数: 
 * @param: tstno 
 * @param: tstpo 
 * @param: buf 
 * @param: ercid 
 * @返回: 
 * @return: uint8  
 * @说明: 塔吉克斯坦
 * @作者: ZJC (2014/8/22)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 ALRT_ERC39_46(uint16 tstno,uint16 tstpo, uint8 *buffer, uint8 ercid);
/************************************************************************
 * @function: ALRT_ERC40
 * @描述: 磁场异常事件记录
 * 
 * @参数: 
 * @param: tstno 
 * @param: type 
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2014/2/11)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 ALRT_ERC40(uint8 tstno, uint8 type);

/************************************************************************
 * @function: ALRT_DLMS_ERC40
 * @描述: 磁场异常事件记录
 * 
 * @参数: 
 * @param: tstno 
 * @param: type 
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 塔吉克斯坦，表计标准事件
 * @作者: ZJC (2014/8/15)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 ALRT_DLMS_ERC40_41_42_43(uint8 tstno, uint8 type, uint8 *inbuf, uint8 inbuflen);
/************************************************************************
 * @function: ALRT_ERC41
 * @描述: 对时事件记录
 * 
 * @参数: 
 * @param: oldtime 
 * @param: newtime 
 * @param: tstno 
 * 
 * @返回: 
 * @return: uint8  true 成功 /false 失败(或不需记录)
 * @说明: 
 * @作者: yzy (2014/2/11)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 ALRT_ERC41(uint8* oldtime, uint8* newtime, uint8 tstno);

/************************************************************************
 * @function: ALRT_DLMS_ERC44
 * @描述: 采集器升级事件
 * 
 * @参数: 
 * @param: tstno 
 * @param: type 
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 塔吉克斯坦，
 * @作者: ZJC (2014/11/1)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 ALRT_DLMS_ERC44(uint8 tstno, uint8 type, uint8 flag, ST_ACT_INPBUF_S *stpinbuf);

/************************************************************************
 * @function: ALRT_DLMS_ERC47
 * @描述: 对表计校时失败事件
 * 
 * @参数: 
 * @param: tstno 
 * @param: type 
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 塔吉克斯坦，
 * @作者: ZJC (2014/11/1)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 ALRT_DLMS_ERC47(uint8 tstno, uint8 type);

/***********************************************************
 * @function: ALRT_ERC60
 * @描述: 请求测试事件记录
 * 
 * @参数: 
 * @param: ext  
 * 
 * @返回: 
 * @return: uint8   
 * @作者: yzy (2010-3-30)
 *---------------------------------------------------------
 * @修改人: 
 **********************************************************/
uint8 ALRT_ERC60(uint32 ext);


/************************************************************************
**函数名称: void ALRT_Init(void* pdata)
**描 述: 告警进程初始化
**
**输 入: 
**输 出: 
**
**备 注: 
**--------------------------------------------------------
**作 者: yzy
**日 期: 09-09-09
*************************************************************************/
void ALRT_Init(void);


/************************************************************************
**函数名称: void ERC44_Collctor_Updata(void* pdata)
**描 述: 上电后判断采集器是否升级成功
**
**输 入: 
**输 出: 
**
**备 注: 
**--------------------------------------------------------
**作 者: zjc
**日 期: 09-09-09
*************************************************************************/
void ERC44_Collctor_Updata(void);

/************************************************************************
**函数名称: void ERC44_MT_Updata(void* pdata)
**描 述: 表计升级是否成功
**
 * @参数: tsno  测量点
 * @参数: okflag  1： 升级成功 ；0：升级失败
**输 出: 
**
**备 注: 
**--------------------------------------------------------
**作 者: zjc
**日 期: 09-09-09
*************************************************************************/
void ERC44_MT_Updata(uint8 tsno, uint8 okflag);


/************************************************************************
 * @function: ALRT_ERC1
 * @描述: 数据初始化和版本变更记录
 * 
 * @参数: 
 * @param: pdata MSG_ALRT_ERC1 参数及数据初始化  /NULL 版本变更判断
 * @返回: 
 * @说明: 1.版本变化(状态驱动,每分钟判断)  2.参数及数据区初始化(消息驱动)
 * @作者: yzy (2014/2/9)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void ALRT_ERC1(uint8 pdata);
/************************************************************************
**函数名称: void ALRT_CLR(void* pdata)
**描 述: 清除所有事件记录
**
**输 入: 
**输 出: 
**
**备 注: 
**--------------------------------------------------------
**作 者: yzy
**日 期: 09-12-30
*************************************************************************/
void ALRT_CLR(void);

/************************************************************************
 * @function: ALRT_ActiveReport
 * @描述: 告警记录的主动上报
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/2/11)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void ALRT_ActiveReport(void);

#endif







