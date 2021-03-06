/****************************************Copyright (c)**************************************************
**                                  ________科技有限公司
**                                          开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: pst_RF.h
**创   建   人: lgy
**最后修改日期: 2015年12月16日
**描        述: 微功率无线RF模块____规约处理文件包括主动通讯和被动通讯
**注        意:
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: LGY
** 版  本: v1.0
** 日　期: 2015年12月16日
** 描　述: 原始版本
********************************************************************************************************/
#ifndef _PST_RF_H_
#define _PST_RF_H_

//#include "Frm.h"

#ifndef EXT_PST_RF
#define EXT_PST_RF  extern
#endif

#define HD_TEST_FLASH   0
#define HD_TEST_EEPROM  1
#define HD_TEST_RF      2
/**********************************************************
* RF命令码
**********************************************************/
#define RF_CMD_CH   0x4348
#define RF_CMD_FR   0x4652
#define RF_CMD_ID   0x4944
#define RF_CMD_PL   0x504C
#define RF_CMD_RS   0x5253
#define RF_CMD_SC   0x5343
#define RF_CMD_SV   0x5356   
#define RF_CMD_LG   0x4C47   

#define   RF_CMD_FT                       0x4654
#define   RF_CMD_HT                       0x4854
#define   RF_CMD_CT                       0x4354
#define   RF_CMD_NS                       0x4E53
/************************************************************************
 * @function: PST_RF_Decode
 * @描述: 判断当前缓存里面是否为合法帧,如果是的话则解析成为被动通讯结构体
 * 
 * @参数: 
 * @param: receive 接收到数据的缓存
 * @param: send 需要发送的数据的缓存
 * @param: sbuflen 发送缓存长度
 * @param: frame 数据解析成的结构体
 * 
 * @返回: 
 * @return: uint8  SYS_ERR_OK/SYS_ERR_FT
 * @说明: 
 * @作者: lgy (2015/12/16)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 PST_RF_Decode(uint8* receive, uint8* send, uint16 sbuflen, PST_Frame* frame);


/************************************************************************
 * @function: PST_RF_Proc
 * @描述: 
 * 
 * @参数: 
 * @param: frm 数据解析成的结构体
 * 
 * @返回: 
 * @return: uint8  SYS_ERR_OK/SYS_ERR_FT
 * @说明: 
 * @作者: lgy (2015/12/16)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 PST_RF_Proc(PST_Frame* frm);


/************************************************************************
 * @function: PST_RF_Pack
 * @描述: 对资源帧进行打包,被动通讯
 * 
 * @参数: 
 * @param: frame 打包的结构体
 * @param: err 
 * @param: length 输入为打包缓存的最大长度,输出为打包好之后的长度
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: lgy (2015/12/16)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 PST_RF_Pack(PST_Frame* frame, uint8 err, uint16* length);

void PstRF_Reply(uint16 cmd, uint8 value);
uint8 PST_RX_Down_PKT(PKT *pkt) ;
uint8 PST_SRF_Decode(uint8* receive, uint8* send, uint16 sbuflen, PST_Frame* frame);
uint8 PST_SRF_Proc(PST_Frame* frame);

#endif


