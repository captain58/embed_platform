/******************************Copyright(c)******************************
**                         
**
** File Name: dev_frm.h
** Author: 
** Date Last Update: 2019-11-02
** Description: 
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: 文件创建
*************************************************************************/
#define EXT_DEV_FRM

#include "sys.h"
#include "hal.h"
#include "bsp.h"
//#include "app.h"        //需要应用层定义各规约处理函数
#include "public.h"
#include "frmcfg.h"
#include "frm.h"
#include <string.h>

#include "pst_zy.h"
const uint8 gucs_PSTEndX16[] =          //定义部规的尾
{
    0x16,
};


const uint8 gucs_PSTEndX7E[] =          //定义DLMS的尾
{
    0x7E,
};


/******************************************************************************
**部规规约信息
******************************************************************************/
#if PST_FRM_BG_EN > 0
const uint8 gucs_PSTBGHead[] =          //定义部规的头
{
    0x68, 
    PST_FRM_ANY,
    PST_FRM_ANY,
    PST_FRM_ANY,
    PST_FRM_ANY,
    PST_FRM_ANY,
    PST_FRM_ANY,
    0x68,
};


#define PST_FRM_BG_LP       0x09        //定义长度起始的位置
#define PST_FRM_BG_LATR     (0)
#define PST_FRM_BG_END      0x16        //定义结尾符
#define PST_FRM_BG_CSSP     0x00        //定义累加和计算起始位置
#define PST_FRM_BG_ATR      (PST_ATR_CSCAL_SC)
#define PST_FRM_BG_MLEN     0x0C        //最小帧的长度
#define PST_FRM_BG_CSSL     0x82        //定义累加和计算的长度偏移
#define PST_FRM_BG_CSOS     0x82        //定义累加和的位置偏移


/******************************************************************************
**部规规约结构体信息
******************************************************************************/
const PSTFrameStr gs_PSTFrameBGStr = 
{
    (uint8*)gucs_PSTBGHead,
    (uint8)sizeof(gucs_PSTBGHead),
    PST_FRM_BG_LP,
    PST_FRM_BG_LATR,
    0,
    0,
    (uint8*)gucs_PSTEndX16,
    (uint8)sizeof(gucs_PSTEndX16),
    PST_FRM_BG_CSSP,
    PST_FRM_BG_MLEN,
    PST_FRM_BG_ATR,
    PST_FRM_BG_CSSL,
    PST_FRM_BG_CSOS,
};


#endif                                  //PST_FRM_BG_EN









/******************************************************************************
**____扩展规约信息
******************************************************************************/
//	#if PST_FRM_ZY_EN > 0
//	const uint8 gucs_PSTZYHead[] =          //定义____扩展的头
//	{
//	    0x68,
//	    PST_FRM_ANY,
//	    PST_FRM_ANY,
//	    PST_FRM_ANY,
//	    PST_FRM_ANY,
//	    PST_FRM_ANY,
//	    0xFB,
//	    0x68,
//	};
//	
//	
//	#define PST_FRM_ZY_LP       0x09        //定义长度起始的位置
//	#define PST_FRM_ZY_LATR     (0)
//	#define PST_FRM_ZY_END      0x16        //定义结尾符
//	#define PST_FRM_ZY_CSSP     0x00        //定义累加和计算起始位置
//	#define PST_FRM_ZY_ATR      (PST_ATR_CSCAL_SC)
//	#define PST_FRM_ZY_MLEN     0x0C        //最小帧的长度
//	#define PST_FRM_ZY_CSSL     0x82        //定义累加和计算的长度偏移
//	#define PST_FRM_ZY_CSOS     0x82        //定义累加和的位置偏移
//	
//	
//	
//	/******************************************************************************
//	**____扩展规约结构体信息
//	******************************************************************************/
//	const PSTFrameStr gs_PSTFrameZYStr = 
//	{
//	    (uint8*)gucs_PSTZYHead,
//	    (uint8)sizeof(gucs_PSTZYHead),
//	    PST_FRM_ZY_LP,
//	    PST_FRM_ZY_LATR,
//	    0,
//	    0,
//	    (uint8*)gucs_PSTEndX16,
//	    (uint8)sizeof(gucs_PSTEndX16),
//	    PST_FRM_ZY_CSSP,
//	    PST_FRM_ZY_MLEN,
//	    PST_FRM_ZY_ATR,
//	    PST_FRM_ZY_CSSL,
//	    PST_FRM_ZY_CSOS,
//	};

//#endif                                  //PST_FRM_ZY_EN


/******************************************************************************
**____扩展规约信息
******************************************************************************/
#if PST_FRM_ZY_EN > 0
const uint8 gucs_PSTZYHead[] =          //定义____扩展的头
{
    0x68,
    PST_FRM_ANY,
    PST_FRM_ANY,
    PST_FRM_ANY,
    PST_FRM_ANY,
    PST_FRM_ANY,
    PST_FRM_ANY,
    0x68,
};


#define PST_FRM_ZY_LP       0x09        //定义长度起始的位置
#define PST_FRM_ZY_LATR     (PST_LATR_DBLEN)
#define PST_FRM_ZY_END      0x16        //定义结尾符
#define PST_FRM_ZY_CSSP     0x00        //定义累加和计算起始位置
#define PST_FRM_ZY_ATR      (PST_ATR_CSCAL_SC)
#define PST_FRM_ZY_MLEN     13        //最小帧的长度
#define PST_FRM_ZY_CSSL     0x82        //定义累加和计算的长度偏移
#define PST_FRM_ZY_CSOS     0x82        //定义累加和的位置偏移



/******************************************************************************
**____扩展规约结构体信息
******************************************************************************/
const PSTFrameStr gs_PSTFrameZYStr = 
{
    (uint8*)gucs_PSTZYHead,
    (uint8)sizeof(gucs_PSTZYHead),
    PST_FRM_ZY_LP,
    PST_FRM_ZY_LATR,
    0,
    0,
    (uint8*)gucs_PSTEndX16,
    (uint8)sizeof(gucs_PSTEndX16),
    PST_FRM_ZY_CSSP,
    PST_FRM_ZY_MLEN,
    PST_FRM_ZY_ATR,
    PST_FRM_ZY_CSSL,
    PST_FRM_ZY_CSOS,
};

#endif                                  //PST_FRM_ZY_EN





/******************************************************************************
**____扩展规约信息
******************************************************************************/
#if PST_FRM_ZBF_EN > 0
const uint8 gucs_PSTZBFHead[] =          //定义____扩展的头
{
    0x68,
    PST_FRM_ANY,
    PST_FRM_ANY,
    PST_FRM_ANY,
    PST_FRM_ANY,
    PST_FRM_ANY,
    0xFD,
    0x68,
};


#define PST_FRM_ZBF_LP       0x09        //定义长度起始的位置
#define PST_FRM_ZBF_LATR     (PST_LATR_DBLEN)
#define PST_FRM_ZBF_END      0x16        //定义结尾符
#define PST_FRM_ZBF_CSSP     0x00        //定义累加和计算起始位置
#define PST_FRM_ZBF_ATR      (PST_ATR_CSCAL_SC)
#define PST_FRM_ZBF_MLEN     0x0D        //最小帧的长度
#define PST_FRM_ZBF_CSSL     0x82        //定义累加和计算的长度偏移
#define PST_FRM_ZBF_CSOS     0x82        //定义累加和的位置偏移


/******************************************************************************
**____扩展规约结构体信息
******************************************************************************/
const PSTFrameStr gs_PSTFrameZBFStr = 
{
    (uint8*)gucs_PSTZBFHead,
    (uint8)sizeof(gucs_PSTZBFHead),
    PST_FRM_ZBF_LP,
    PST_FRM_ZBF_LATR,
    0,
    0,
    (uint8*)gucs_PSTEndX16,
    (uint8)sizeof(gucs_PSTEndX16),
    PST_FRM_ZBF_CSSP,
    PST_FRM_ZBF_MLEN,
    PST_FRM_ZBF_ATR,
    PST_FRM_ZBF_CSSL,
    PST_FRM_ZBF_CSOS,
};

#endif                                  //PST_FRM_ZBF_EN




/******************************************************************************
** *SET *GET规约信息
******************************************************************************/
#if PST_FRM_GS_EN > 0
const uint8 gucs_PSTGSHead[] =          //定义*SET *GET规约的头
{
    '*',
    PST_FRM_ANY,
    'E',
    'T',
};
const uint8 gucs_PSTEndGS[] =          //定义DLMS的尾</h:rt>
{
    '#',
};


#define PST_FRM_GS_LP       0x00        //定义长度起始的位置
#define PST_FRM_GS_LATR     (PST_LATR_NOLEN)
#define PST_FRM_GS_END      '#'         //定义结尾符
#define PST_FRM_GS_CSSP     0x00        //定义累加和计算起始位置
#define PST_FRM_GS_ATR      (PST_ATR_PARIT)
#define PST_FRM_GS_MLEN     0x0F        //最小帧的长度
#define PST_FRM_GS_CSSL     0x00        //定义累加和计算的长度偏移
#define PST_FRM_GS_CSOS     0x00        //定义累加和的位置偏移

/******************************************************************************
** *SET * GET通讯规约结构体信息
******************************************************************************/
const PSTFrameStr gs_PSTFrameGSStr = 
{
    (uint8*)gucs_PSTGSHead,
    (uint8)sizeof(gucs_PSTGSHead),
    PST_FRM_GS_LP,
    PST_FRM_GS_LATR,
    0,
    0,
    (uint8*)gucs_PSTEndGS,
    (uint8)sizeof(gucs_PSTEndGS),
    PST_FRM_GS_CSSP,
    PST_FRM_GS_MLEN,
    PST_FRM_GS_ATR,
    PST_FRM_GS_CSSL,
    PST_FRM_GS_CSOS,
};

#endif                                  //PST_FRM_GS_EN 





/******************************************************************************
**无线猫通讯规约信息
******************************************************************************/
#if PST_FRM_WM_EN > 0

const uint8 gucs_PSTWMHead[] = 
{
    0xAA,
};


#define PST_FRM_WM_LP       0x02        //定义长度起始的位置
#define PST_FRM_WM_LATR     (PST_LATR_DBLEN)
#define PST_FRM_WM_END      0x16        //定义结尾符
#define PST_FRM_WM_CSSP     0x00        //定义累加和计算起始位置
#define PST_FRM_WM_ATR      (PST_ATR_CSCAL_SC)
#define PST_FRM_WM_MLEN     0x06        //最小帧的长度
#define PST_FRM_WM_CSSL     0x82        //定义累加和计算的长度偏移
#define PST_FRM_WM_CSOS     0x82        //定义累加和的位置偏移

/******************************************************************************
**无线猫通讯规约结构体信息
******************************************************************************/
const PSTFrameStr gs_PSTFrameWMStr = 
{
    (uint8*)gucs_PSTWMHead,
    (uint8)sizeof(gucs_PSTWMHead),
    PST_FRM_WM_LP,
    PST_FRM_WM_LATR,
    0,
    0,
    (uint8*)gucs_PSTEndX16,
    (uint8)sizeof(gucs_PSTEndX16),
    PST_FRM_WM_CSSP,
    PST_FRM_WM_MLEN,
    PST_FRM_WM_ATR,
    PST_FRM_WM_CSSL,
    PST_FRM_WM_CSOS,
};


#endif                                  //PST_FRM_WM_EN










/******************************************************************************
**国电1376.1规约帧信息
******************************************************************************/
#if PST_FRM_GD_1_EN > 0

const uint8 gucs_PSTGD1Head[] =          //定义国电规约的头
{
    0x68, 
    PST_FRM_ANY,
    PST_FRM_ANY,
    PST_FRM_ANY,
    PST_FRM_ANY,
    0x68,
};


#define PST_FRM_GD_1_LP       0x01        //定义长度起始的位置
#define PST_FRM_GD_1_LATR     (PST_LATR_DBLEN | PST_LATR_RELEN)
#define PST_FRM_GD_1_LLFR     0           //解析时先左移
#define PST_FRM_GD_1_LRFR     2           //解析时后右移
#define PST_FRM_GD_1_END      0x16        //定义结尾符
#define PST_FRM_GD_1_CSSP     0x06        //定义累加和计算起始位置
#define PST_FRM_GD_1_ATR      (PST_ATR_CSCAL_SC)
#define PST_FRM_GD_1_MLEN     0x08        //最小帧的长度
#define PST_FRM_GD_1_CSSL     0x88        //定义累加和计算的长度偏移
#define PST_FRM_GD_1_CSOS     0x82        //定义累加和的位置偏移


/******************************************************************************
**国电1376.1规约帧结构体信息
******************************************************************************/
const PSTFrameStr gs_PSTFrameGD1Str = 
{
    (uint8*)gucs_PSTGD1Head,
    (uint8)sizeof(gucs_PSTGD1Head),
    PST_FRM_GD_1_LP,
    PST_FRM_GD_1_LATR,
    PST_FRM_GD_1_LLFR,
    PST_FRM_GD_1_LRFR,
    (uint8*)gucs_PSTEndX16,
    (uint8)sizeof(gucs_PSTEndX16),
    PST_FRM_GD_1_CSSP,
    PST_FRM_GD_1_MLEN,
    PST_FRM_GD_1_ATR,
    PST_FRM_GD_1_CSSL,
    PST_FRM_GD_1_CSOS,
};


#endif                                      //PST_FRM_GD_1_EN
















/******************************************************************************
**国电1376.2规约帧信息
******************************************************************************/
#if PST_FRM_GD_2_EN > 0

const uint8 gucs_PSTGD2Head[] =          //定义1376.2规约的头
{
    0x68,
};


#define PST_FRM_GD_2_LP       0x01        //定义长度起始的位置
#define PST_FRM_GD_2_LATR     (PST_LATR_DBLEN)
#define PST_FRM_GD_2_END      0x16        //定义结尾符
#define PST_FRM_GD_2_CSSP     0x02        //定义累加和计算起始位置(控制域与用户数据区)
#define PST_FRM_GD_2_ATR      (PST_ATR_CSCAL_SC)
#define PST_FRM_GD_2_MLEN     0           //最小帧的长度
#define PST_FRM_GD_2_CSSL     0x84        //定义累加和计算的长度偏移
#define PST_FRM_GD_2_CSOS     0x82        //定义累加和的位置偏移

/******************************************************************************
**国电1376.2规约帧结构体信息
******************************************************************************/
const PSTFrameStr gs_PSTFrameGD2Str = 
{
    (uint8*)gucs_PSTGD2Head,
    (uint8)sizeof(gucs_PSTGD2Head),
    PST_FRM_GD_2_LP,
    PST_FRM_GD_2_LATR,
    0,
    0,
    (uint8*)gucs_PSTEndX16,
    (uint8)sizeof(gucs_PSTEndX16),
    PST_FRM_GD_2_CSSP,
    PST_FRM_GD_2_MLEN,
    PST_FRM_GD_2_ATR,
    PST_FRM_GD_2_CSSL,
    PST_FRM_GD_2_CSOS,
};

#endif                                      //PST_FRM_GD_2_EN





#if PST_FRM_EM_EN > 0

/******************************************************************************
**R8C-ESAM规约帧头
******************************************************************************/
const uint8 gucs_PSTEMHead[] =
{
    0xA5, 
    PST_FRM_ANY,
    0xA5,
};


#define PST_FRM_EM_LP       0x03        //定义长度起始的位置
#define PST_FRM_EM_LATR     (PST_LATR_DBLEN)
#define PST_FRM_EM_END      0x16        //定义结尾符
#define PST_FRM_EM_CSSP     0x00        //定义累加和计算起始位置
#define PST_FRM_EM_ATR      (PST_ATR_CSCAL_SC)
#define PST_FRM_EM_MLEN     0x07        //最小帧的长度需要满足
#define PST_FRM_EM_CSSL     0x82        //定义累加和计算的长度偏移
#define PST_FRM_EM_CSOS     0x82        //定义累加和的位置偏移


/******************************************************************************
**R8C-ESAM规约帧结构体信息
******************************************************************************/
const PSTFrameStr gs_PSTFrameEMStr = 
{
    (uint8*)gucs_PSTEMHead,
    (uint8)sizeof(gucs_PSTEMHead),
    PST_FRM_EM_LP,
    PST_FRM_EM_LATR,
    0,
    0,
    (uint8*)gucs_PSTEndX16,
    (uint8)sizeof(gucs_PSTEndX16),
    PST_FRM_EM_CSSP,
    PST_FRM_EM_MLEN,
    PST_FRM_EM_ATR,
    PST_FRM_EM_CSSL,
    PST_FRM_EM_CSOS,
};


#endif                                  //PST_FRM_EM_EN




#if PST_FRM_WL_1_EN > 0
/******************************************************************************
**全功率无线通信上行协议接口规约帧头
******************************************************************************/
const uint8 gucs_PSTWL1Head[] =
{
    PST_FRM_ANY,
};


#define PST_FRM_WL_1_LP       0x01        //定义长度起始的位置
#define PST_FRM_WL_1_LATR     (PST_LATR_DBLEN) //
#define PST_FRM_WL_1_LLFT     0           //解析时先左移
#define PST_FRM_WL_1_LRFT     0           //解析时后右移
#define PST_FRM_WL_1_END      0             //定义结尾符
#define PST_FRM_WL_1_CSSP     0x00        //定义累加和计算起始位置
#define PST_FRM_WL_1_ATR      (PST_ATR_END | PST_ATR_CSCAL_SC)
#define PST_FRM_WL_1_MLEN     0x00        //最小帧的长度需要满足
#define PST_FRM_WL_1_CSSL     0x81        //定义累加和计算的长度偏移
#define PST_FRM_WL_1_CSOS     0x81        //定义累加和的位置偏移

/******************************************************************************
**DLMS规约帧结构体信息
******************************************************************************/
const PSTFrameStr gs_PSTFrameWL1Str = 
{
    (uint8*)gucs_PSTWL1Head,
    (uint8)sizeof(gucs_PSTWL1Head),
    PST_FRM_WL_1_LP,
    PST_FRM_WL_1_LATR,
    PST_FRM_WL_1_LLFT,
    PST_FRM_WL_1_LRFT,
    PST_FRM_WL_1_END,
    PST_FRM_WL_1_CSSP,
    PST_FRM_WL_1_MLEN,
    PST_FRM_WL_1_ATR,
    PST_FRM_WL_1_CSSL,
    PST_FRM_WL_1_CSOS,
};

#endif      




#if PST_FRM_587_EN > 0

/******************************************************************************
**587上行协议接口规约帧头
******************************************************************************/
const uint8 gucs_PST587Head[] =
{
    0x02, 
    PST_FRM_ANY,
};
const uint8 gucs_PSTEnd587[] =          //定义部规的尾
{
    0x0,
};


#define PST_FRM_587_LP       0x01        //定义长度起始的位置
#define PST_FRM_587_LATR     (0)
#define PST_FRM_587_END      0x00        //定义结尾符
#define PST_FRM_587_CSSP     0x01        //定义累加和计算起始位置
#define PST_FRM_587_ATR      (PST_ATR_CSCAL_SD | PST_ATR_END)
#define PST_FRM_587_MLEN     0x02        //最小帧的长度需要满足
#define PST_FRM_587_CSSL     0x83        //定义累加和计算的长度偏移
#define PST_FRM_587_CSOS     0x82        //定义累加和的位置偏移

/******************************************************************************
**R8C-ESAM规约帧结构体信息
******************************************************************************/
const PSTFrameStr gs_PSTFrame587Str = 
{
    (uint8*)gucs_PST587Head,
    (uint8)sizeof(gucs_PST587Head),
    PST_FRM_587_LP,
    PST_FRM_587_LATR,
    0,
    0,
    (uint8*)gucs_PSTEnd587,
    (uint8)sizeof(gucs_PSTEnd587),
    PST_FRM_587_CSSP,
    PST_FRM_587_MLEN,
    PST_FRM_587_ATR,
    PST_FRM_587_CSSL,
    PST_FRM_587_CSOS,
};


#endif                                  //PST_FRM_587_EN




#if PST_FRM_DLMS_HDLC_EN > 0

/******************************************************************************
**DLMS上行协议接口规约帧头
******************************************************************************/
const uint8 gucs_PSTDLMS_HDLCHead[] =
{
    0x7E,
};


#define PST_FRM_DLMS_HDLC_LP       0x01        //定义长度起始的位置
#define PST_FRM_DLMS_HDLC_LATR     (PST_LATR_DBLEN | PST_LATR_BFLEN) //
#define PST_FRM_DLMS_HDLC_LLFT     5           //解析时先左移
#define PST_FRM_DLMS_HDLC_LRFT     0           //解析时后右移
#define PST_FRM_DLMS_HDLC_END      0x7E        //定义结尾符
#define PST_FRM_DLMS_HDLC_CSSP     0x01        //定义累加和计算起始位置
#define PST_FRM_DLMS_HDLC_ATR      (PST_ATR_CSCAL_CRC1)
#define PST_FRM_DLMS_HDLC_MLEN     0x02        //最小帧的长度需要满足
#define PST_FRM_DLMS_HDLC_CSSL     0x84        //定义累加和计算的长度偏移
#define PST_FRM_DLMS_HDLC_CSOS     0x83        //定义累加和的位置偏移

/******************************************************************************
**DLMS规约帧结构体信息
******************************************************************************/
const PSTFrameStr gs_PSTFrameDLMS_HDLCStr = 
{
    (uint8*)gucs_PSTDLMS_HDLCHead,
    (uint8)sizeof(gucs_PSTDLMS_HDLCHead),
    PST_FRM_DLMS_HDLC_LP,
    PST_FRM_DLMS_HDLC_LATR,
    PST_FRM_DLMS_HDLC_LLFT,
    PST_FRM_DLMS_HDLC_LRFT,
    (uint8*)gucs_PSTEndX7E,
    (uint8)sizeof(gucs_PSTEndX7E),
    PST_FRM_DLMS_HDLC_CSSP,
    PST_FRM_DLMS_HDLC_MLEN,
    PST_FRM_DLMS_HDLC_ATR,
    PST_FRM_DLMS_HDLC_CSSL,
    PST_FRM_DLMS_HDLC_CSOS,
};


#endif                                  //PST_FRM_DLMS_HDLC_EN


#if PST_FRM_DLMS_IP_EN > 0

/******************************************************************************
**DLMS上行协议接口规约帧头
******************************************************************************/
const uint8 gucs_PSTDLMS_IPHead[] =
{
    0x00,0x01,
};


#define PST_FRM_DLMS_IP_LP       0x06        //定义长度起始的位置
#define PST_FRM_DLMS_IP_LATR     (PST_LATR_DBLEN | PST_LATR_BFLEN) //
#define PST_FRM_DLMS_IP_LLFT     0           //解析时先左移
#define PST_FRM_DLMS_IP_LRFT     0           //解析时后右移
#define PST_FRM_DLMS_IP_END      0x7E        //定义结尾符
#define PST_FRM_DLMS_IP_CSSP     0x01        //定义累加和计算起始位置
#define PST_FRM_DLMS_IP_ATR      (PST_ATR_PARIT | PST_ATR_END)
#define PST_FRM_DLMS_IP_MLEN     0x08        //最小帧的长度需要满足
#define PST_FRM_DLMS_IP_CSSL     0x84        //定义累加和计算的长度偏移
#define PST_FRM_DLMS_IP_CSOS     0x83        //定义累加和的位置偏移

/******************************************************************************
**DLMS规约帧结构体信息
******************************************************************************/
const PSTFrameStr gs_PSTFrameDLMS_IPStr = 
{
    (uint8*)gucs_PSTDLMS_IPHead,
    (uint8)sizeof(gucs_PSTDLMS_IPHead),
    PST_FRM_DLMS_IP_LP,
    PST_FRM_DLMS_IP_LATR,
    PST_FRM_DLMS_IP_LLFT,
    PST_FRM_DLMS_IP_LRFT,
    (uint8*)gucs_PSTEndX7E,
    (uint8)sizeof(gucs_PSTEndX7E),
    PST_FRM_DLMS_IP_CSSP,
    PST_FRM_DLMS_IP_MLEN,
    PST_FRM_DLMS_IP_ATR,
    PST_FRM_DLMS_IP_CSSL,
    PST_FRM_DLMS_IP_CSOS,
};


#endif                                  //PST_FRM_DLMS_IP_EN


#if PST_FRM_EDMI_EN > 0

/******************************************************************************
**EDMI协议接口规约帧头
******************************************************************************/
const uint8 gucs_PSTEDMIHead[] =
{
    0x02,
};

const uint8 gucs_PSTEndEDMI[] =          //定义部规的尾
{
    0x03,
};


#define PST_FRM_EDMI_LP       0x01        //定义长度起始的位置
#define PST_FRM_EDMI_LATR     PST_LATR_NOLEN //无帧长度
#define PST_FRM_EDMI_LLFT     0           //解析时先左移
#define PST_FRM_EDMI_LRFT     0           //解析时后右移
#define PST_FRM_EDMI_END      0x03        //定义结尾符
#define PST_FRM_EDMI_CSSP     0           //定义累加和计算起始位置
#define PST_FRM_EDMI_ATR      (PST_ATR_CSCAL_CRC2)
#define PST_FRM_EDMI_MLEN     0x0C        //最小帧的长度需要满足
#define PST_FRM_EDMI_CSSL     0x83        //定义累加和计算的长度偏移
#define PST_FRM_EDMI_CSOS     0x83        //定义累加和的位置偏移

/******************************************************************************
**EDMI规约帧结构体信息
******************************************************************************/
const PSTFrameStr gs_PSTFrameEDMIStr = 
{
    (uint8*)gucs_PSTEDMIHead,
    (uint8)sizeof(gucs_PSTEDMIHead),
    PST_FRM_EDMI_LP,
    PST_FRM_EDMI_LATR,
    PST_FRM_EDMI_LLFT,
    PST_FRM_EDMI_LRFT,
    (uint8*)gucs_PSTEndEDMI,
    (uint8)sizeof(gucs_PSTEndEDMI),
    PST_FRM_EDMI_CSSP,
    PST_FRM_EDMI_MLEN,
    PST_FRM_EDMI_ATR,
    PST_FRM_EDMI_CSSL,
    PST_FRM_EDMI_CSOS,
};


#endif 


//dev_frm.c  定义南京规约结构体  zwj
#if PST_FRM_DM_EN > 0

/******************************************************************************
**结构体
******************************************************************************/
const uint8 gucs_PSTDMHead[] =
{
	0x45,
	PST_FRM_ANY,
	PST_FRM_ANY,
	0x45,
};

const uint8 gucs_PSTEndTDM[] =          //定义部规的尾
{
    0x54,
};
#define PST_FRM_DM_LP       0x01        //定义长度起始的位置
#define PST_FRM_DM_LATR     (0)
#define PST_FRM_DM_END      0x54        //定义结尾符
#define PST_FRM_DM_CSSP     0x04        //定义累加和计算起始位置
#define PST_FRM_DM_ATR      (PST_ATR_CSCAL_SC)//累加和
#define PST_FRM_DM_MLEN     0x06        //最小帧的长度需要满足
#define PST_FRM_DM_CSSL     0x86        //定义累加和计算的长度偏移
#define PST_FRM_DM_CSOS     0x82        //定义累加和的位置偏移

/******************************************************************************
**结构体
******************************************************************************/
const PSTFrameStr gs_PSTFrameDMStr =
{
	(uint8*)gucs_PSTDMHead,
	(uint8)sizeof(gucs_PSTDMHead),
	PST_FRM_DM_LP,
	PST_FRM_DM_LATR,
	0,
	0,
    (uint8*)gucs_PSTEndTDM,
    (uint8)sizeof(gucs_PSTEndTDM),
	PST_FRM_DM_CSSP,
	PST_FRM_DM_MLEN,
	PST_FRM_DM_ATR,
	PST_FRM_DM_CSSL,
	PST_FRM_DM_CSOS,
};



#endif                                  



/******************************************************************************
**国电1376.1规约帧信息
******************************************************************************/
#if PST_FRM_XML_EN > 0

const uint8 gucs_PSTXMLHead[] =          //定义国电规约的头"<? xml version='1.0' ?>"
{
    '<', 
    '?',
    ' ',
    'x',
    'm',
    'l',
    ' ', 
    'v',
    'e',
    'r',
    's',
    'i',  
    'o',
    'n',
    '=',
    '\'',
    '1', 
    '.',
    '0',
    '\'',    
    ' ',
    '?',
    '>',     
};

const uint8 gucs_PSTEndXML[] =          //定义邋邋DLMS的尾</h:rt>
{
    '<',
    '/',
    'h',
    ':',
    'r',
    't',
    '>',
};



#define PST_FRM_XML_LP       0x01        //定义长度起始的位置
#define PST_FRM_XML_LATR     PST_LATR_NOLEN//(PST_LATR_DBLEN | PST_LATR_RELEN)
#define PST_FRM_XML_LLFR     0           //解析时先左移
#define PST_FRM_XML_LRFR     2           //解析时后右移
#define PST_FRM_XML_END      0x16        //定义结尾符
#define PST_FRM_XML_CSSP     0x06        //定义累加和计算起始位置
#define PST_FRM_XML_ATR      (PST_ATR_CSCAL_SC)
#define PST_FRM_XML_MLEN     (68)        //最小帧的长度
#define PST_FRM_XML_CSSL     0x88        //定义累加和计算的长度偏移
#define PST_FRM_XML_CSOS     0x82        //定义累加和的位置偏移


/******************************************************************************
**国电1376.1规约帧结构体信息
******************************************************************************/
const PSTFrameStr gs_PSTFrameXMLStr = 
{
    (uint8*)gucs_PSTXMLHead,
    (uint8)sizeof(gucs_PSTXMLHead),
    PST_FRM_XML_LP,
    PST_FRM_XML_LATR,
    PST_FRM_XML_LLFR,
    PST_FRM_XML_LRFR,
    (uint8*)gucs_PSTEndXML,
    (uint8)sizeof(gucs_PSTEndXML),
    PST_FRM_XML_CSSP,
    PST_FRM_XML_MLEN,
    PST_ATR_PARIT,//PST_FRM_XML_ATR,
    PST_FRM_XML_CSSL,
    PST_FRM_XML_CSOS,
};


#endif                                      //PST_FRM_GD_1_EN




/*******************************************************************************
**定义通道信息结构体数组定义及初始化
*******************************************************************************/
#if PST_CHN_NO > 0 
                     
const PSTChnInfo gss_PSTChnInfo[PST_CHN_NO] = 
{
    {
        (uint8*)gucs_PSTChn0frms,
        (uint8)sizeof(gucs_PSTChn0frms),
        (uint8)PST_CHN0_HCHARS,
    },
#if PST_CHN_NO > 1
    {
        (uint8*)gucs_PSTChn1frms,
        (uint8)sizeof(gucs_PSTChn1frms),
        PST_CHN1_HCHARS,
    },
#endif

#if PST_CHN_NO > 2 
    {
        (uint8*)gucs_PSTChn2frms,
        (uint8)sizeof(gucs_PSTChn2frms),
        (uint8)PST_CHN2_HCHARS,
    },
#endif

#if PST_CHN_NO > 3
    {
        (uint8*)gucs_PSTChn3frms,
        (uint8)sizeof(gucs_PSTChn3frms),
        (uint8)PST_CHN3_HCHARS,
    },
#endif

#if PST_CHN_NO > 4
    {
        (uint8*)gucs_PSTChn4frms,
        (uint8)sizeof(gucs_PSTChn4frms),
        (uint8)PST_CHN4_HCHARS,
    },
#endif

#if PST_CHN_NO > 5
    {
        (uint8*)gucs_PSTChn5frms,
        (uint8)sizeof(gucs_PSTChn5frms),
        (uint8)PST_CHN5_HCHARS,
    },
#endif
    
#if PST_CHN_NO > 6
    {
        (uint8*)gucs_PSTChn6frms,
        (uint8)sizeof(gucs_PSTChn6frms),
        (uint8)PST_CHN6_HCHARS,
    },
#endif

};

#endif                                 



/******************************************************************************
**定义帧结构常量
******************************************************************************/
const PSTFrameStr* const gss_PSTFrameStr[] = 
{
#if (PST_FRM_BG_EN > 0)
    &gs_PSTFrameBGStr,
#endif

#if (PST_FRM_ZY_EN > 0)
    &gs_PSTFrameZYStr,
#endif
#if (PST_FRM_WL_1_EN > 0)
    &gs_PSTFrameWL1Str,
#endif

#if (PST_FRM_ZBF_EN > 0)
    &gs_PSTFrameZBFStr,
#endif

#if (PST_FRM_GS_EN > 0)
    &gs_PSTFrameGSStr,
#endif

#if (PST_FRM_WM_EN > 0)
    &gs_PSTFrameWMStr,
#endif

#if (PST_FRM_GD_1_EN > 0)
    &gs_PSTFrameGD1Str,
#endif

#if (PST_FRM_GD_2_EN > 0)
    &gs_PSTFrameGD2Str,
#endif

#if (PST_FRM_EM_EN > 0)
    &gs_PSTFrameEMStr,
#endif

#if (PST_FRM_587_EN > 0)
    &gs_PSTFrame587Str,
#endif

#if (PST_FRM_DLMS_HDLC_EN > 0)
    &gs_PSTFrameDLMS_HDLCStr,
#endif
    
#if (PST_FRM_DLMS_IP_EN > 0)
    &gs_PSTFrameDLMS_IPStr,
#endif
    
#if (PST_FRM_EDMI_EN > 0)
    &gs_PSTFrameEDMIStr,
#endif
    //zwj
#if (PST_FRM_DM_EN > 0)
    &gs_PSTFrameDMStr,
#endif
#if (PST_FRM_XML_EN > 0)
    &gs_PSTFrameXMLStr,
#endif

};



/************************************************************************
 * @function: PST_CheckFrame
 * @描述: 指定帧类型的有效帧判断
 * 
 * @参数: 
 * @param: num 需要判断的帧的类型编号
 * @param: buffer 需要判断有效帧的缓存
 * @param: length 需要判断的帧的长度
 * 
 * @返回: 
 * @return: uint8  PST_ERR_OK
 *      PST_ERR_VAR
 *      PST_ERR_TS
 *      PST_ERR_NE
 * @说明: 
 * @作者: yzy (2013/7/15)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 PST_CheckFrame(uint8 num, uint8* buffer, uint16 length)
{
    const PSTFrameStr* str;                 //定义指向帧信息结构体
    volatile uint16 len;                   //定义帧数据区长度
    uint16 ut_i,ut_j;                             //循环变量
    uint16 sumlen;                          //待校验数据长度
    uint16 sumos;                           //校验和存放的位置
    uint16 cs;

    SYS_VAR_CHECK(num >= PST_FRM_NO);        //参数验证
    SYS_VAR_CHECK(length > PST_LEN_MAX);     //参数验证
                                            //获取当前要扫描的规约的结构体
    str = (PSTFrameStr*)gss_PSTFrameStr[num];

    if(str->extlen > length)                //判断是否已经达到最小帧长度
    {
        return PST_ERR_TS;                  //返回扫描长度太短
    }
                                            //判断帧头
    for(ut_i = 0; ut_i < str->hlen; ut_i++)
    {                                       //如果与匹配字符匹配那么继续下一个
        if(buffer[ut_i] == str->head[ut_i])
        {
            continue;
        }
                                            //如果当前的匹配字符为任意匹配字符则继续
        if(str->head[ut_i] == PST_FRM_ANY) 
        {
            continue;
        }
        return PST_ERR_NE;                  //返回不是有效帧
    }

    if(str->latr & PST_LATR_NOLEN)          //没有数据区长度
    {
        len = 0;                            //则定义数据区长度为0
        
        //在数据区无数据长度的情况下，先判断校验和，确认数据区长度
        if(!(str->atr & PST_ATR_END))           //有结尾符
        {
                                            //循环扫描结尾符
            for(ut_i = str->extlen - 1; ut_i < length; ut_i++)
            {
//	                if(buffer[ut_i] == str->end)
//	                {
//	                    len = ut_i + 1 - str->extlen;
//	                }
                for(ut_j = 0; ut_j < str->elen; ut_j++)
                {                                       //如果与匹配字符匹配那么继续下一个
                    if(buffer[ut_i+ut_j] == str->end[ut_j])
                    {
                        continue;
                    }
                                                        //如果当前的匹配字符为任意匹配字符则继续
                    break;
                }
                if(ut_j == str->elen)
                {
                    len = ut_i + 1 - str->extlen;
                    break;
                }

            }
            if(len == 0)
            {
                return PST_ERR_NE;                  //返回不是有效帧
            }
        }
    }
    else                                    //有数据区长度
    {
        if(str->latr & PST_LATR_DBLEN)      //如果是双字节长度
        {
            if(str->latr &  PST_LATR_BFLEN) //高字节在前
            {
                len = buffer[str->lp + 1]           //数据长度
                    + (buffer[str->lp ] << 8);
            }
            else                            //低字节在前
            {
                len = buffer[str->lp]           //数据长度
                    + (buffer[str->lp + 1] << 8);
            }
        }
        else                                //否则就是单字节的帧长
        {
            len = buffer[str->lp];          //数据长度
        }
        
        if(str->latr & PST_LATR_DBLEN)      //如果是双字节长度
        {
            len <<= str->llft;              //为防止代码被编译器优化，需要申明为volatile属性
            len &= 0xffff;
            len >>= str->llft;
            len >>= str->lrft;
        }
        else
        {
            len <<= str->llft;
            len &= 0xff;
            len >>= str->llft;
            len >>= str->lrft;
        }
    }

    if(len + str->extlen > length)          //如果数据区长度与最小帧长度之后大于扫描的帧长那么肯定不是一个有效帧
    {
        return PST_ERR_NE;                  //返回不是有效帧    
    }
                                            
    if(!(str->atr & PST_ATR_PARIT))         //有校验位
    {
                                            //计算待校验数据的长度
        sumlen = len + str->extlen;
        if(str->cssl & 0x80)                //负
        {
            if(sumlen > (str->cssl & 0x7F))
            {
                sumlen -= (str->cssl & 0x7F);
            }
            else
            {
                return PST_ERR_NE;
            }
        }
        else                                //正
        {
            sumlen += (str->cssl & 0x7F);
        }
                                            //计算校验和存放的位置偏移
        sumos = len + str->extlen;
        if(str->csco & 0x80)                //负
        {
            if(sumos > (str->csco & 0x7F))
            {
                sumos -= (str->csco & 0x7F);
            }
            else
            {
                return PST_ERR_NE;
            }
        }
        else                                //正
        {
            sumos += (str->csco & 0x7F);
        }
        
        switch(str->atr & PST_ATR_CSCAL)
        {
            case PST_ATR_CSCAL_SC:          //累加和
                cs = DoSum(
                    buffer + str->cssp, 
                    sumlen
                    );
                
                if((cs & 0xFF) != buffer[sumos])
                {
                    return PST_ERR_NE;      //返回不是有效帧   
                }
                break;
                
            case PST_ATR_CSCAL_SD:          //双字节累加和
                cs = DoSum(
                    buffer + str->cssp, 
                    sumlen
                    );
                
                if(cs != ByteToInt(buffer + sumos))
                {
                    return PST_ERR_NE;      //返回不是有效帧   
                }
                break;

            case PST_ATR_CSCAL_BS:          //位加和
                cs = DoBitSum(
                    buffer + str->cssp, 
                    sumlen
                    );

                if((uint8)cs != buffer[sumos])
                {
                    return PST_ERR_NE;      //返回不是有效帧      
                }
                break;
                
            case PST_ATR_CSCAL_CRC1:
                cs = DoCrcDlms(
                    buffer + str->cssp, 
                    sumlen,
                    0xffff
                    );
                
                if(cs != ByteToInt(buffer + sumos))
                {
                    return PST_ERR_NE;      //返回不是有效帧   
                }
                break;
            case PST_ATR_CSCAL_CRC2:    //EDMI, 在判断检验和之前，需要做转码处理
                for(uint16 ui_k = 1; ui_k < (length - 1); ui_k++)  //去掉转义符
                {
                    if((buffer[ui_k] == 0x10) && ((buffer[ui_k + 1] == 0x42) || (buffer[ui_k + 1] == 0x43) ||
                        (buffer[ui_k + 1] == 0x50) || (buffer[ui_k + 1] == 0x51) || (buffer[ui_k + 1] == 0x53)))
                    {	
                        buffer[ui_k] = buffer[ui_k + 1] - 0x40;
                        memcpy(buffer + ui_k + 1, buffer + ui_k + 2, sumlen + 1 - ui_k);
                        length --;
                        sumlen--;
                        sumos--;
                    }
                }
                cs = DoCrc16(
                    buffer + str->cssp, 
                    sumlen, 0
                    );
                
                if(cs !=( ((uint16)buffer[sumos] << 8) + (uint16)buffer[sumos + 1]))
                {
                    return PST_ERR_NE;      //返回不是有效帧   
                }
                break;
            default:
                return PST_ERR_NE;          //返回不是有效帧         
        }
    }

    if(!(str->atr & PST_ATR_END))           //有结尾符
    {
        if(str->latr & PST_LATR_NOLEN)      //没有数据区长度
        {
                                            //循环扫描结尾符
            for(ut_i = str->extlen - 1; ut_i < length; ut_i++)
            {
                for(ut_j = 0; ut_j < str->elen; ut_j++)
                {                                       //如果与匹配字符匹配那么继续下一个
                    if(buffer[ut_i+ut_j] == str->end[ut_j])
                    {
                        continue;
                    }
                                                        //如果当前的匹配字符为任意匹配字符则继续
                    break;
                }
                if(ut_j == str->elen)
                {
                    len = ut_i + 1 - str->extlen;
                    SYS_OK();
                }

            }
            return PST_ERR_NE;              //返回不是有效帧  
        }
        else
        {                                   //判断是否为结尾符
//	            if(buffer[str->extlen - 1 + len] != str->end)
//	            {
//	                return PST_ERR_NE;          //返回不是有效帧      
//              }
            for(ut_i = 0,ut_j = str->extlen - 1 + len; ut_i < str->elen; ut_j++,ut_i++)
            {                                       //如果与匹配字符匹配那么继续下一个
                if(buffer[ut_j] == str->end[ut_i])
                {
                    continue;
                }
                                                    //如果当前的匹配字符为任意匹配字符则继续
                break;
            }
            if(ut_i != str->elen)
            {
                return PST_ERR_NE;
            }

        }
    }
    SYS_OK();                                //返回扫描到有效帧
}


/************************************************************************
 * @function: PST_Channel
 * @描述: 带前导符扫描接收报文,判断出为何种规约
 * 
 * @参数: 
 * @param: channel 需要判断有效帧的通道编号
 * @param: buffer 需要判断有效帧的缓存
 * @param: length 需要判断的帧的长度
 * @param: protocol 输出,实际规约编号,如果是0xFF就表示不是有效规约(例:PST_FRM_ZY_NO)
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2013/7/15)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 PST_Channel(uint8_t channel, uint8* buffer, uint16 length, uint8* protocol)
{
    const PSTChnInfo* info;             //通道信息
    uint16 ut_i, ut_j;                  //两个循环变量
                                        //参数确认
    SYS_VAR_CHECK(channel >= PST_CHN_NO);
    SYS_VAR_CHECK(length > PST_LEN_MAX);
    
    info = gss_PSTChnInfo + channel;    //获取当前的通道信息结构体
                                        //对通道支持的规约进行扫描
    for(ut_i = 0; ut_i < info->count; ut_i++)
    {                                   //对前导字符进行扫描
        for(ut_j = 0; ut_j <= info->headchars; ut_j++)
        {
            switch(PST_CheckFrame(      //进行有效帧的判断,并对结果散转处理
                info->frameno[ut_i],
                buffer + ut_j,
                length - ut_j))
            {
                case PST_ERR_OK:        //扫描到了有效帧
                    //转移数据到缓存的头上
                    memcpy(buffer, buffer + ut_j,length - ut_j);
                    //当前有效的规约编号
                    *protocol = info->frameno[ut_i];
                    SYS_OK();

                case PST_ERR_TS:        // 如果帧长太小
                case PST_ERR_VAR:       // 或者参数错误
                                        // 退出当前循环
                    ut_j = info->headchars;
                    break;               
            }
        }
    }

    *protocol = 0xFF;                   //无效的规约编号
    return PST_ERR_NE;                  //返回没有扫描到有效帧
}


/************************************************************************
 * @function: PST_DoneFrame
 * @描述: 进行帧头\帧尾\累加和的填写,对于没有数据区长度的帧这里无法完成其打包
 * 
 * @参数: 
 * @param: num 需要打包的帧的类型编号
 * @param: buffer 需要打包成有效帧的缓存
 * 
 * @返回: 
 * @return: uint16  0 打包失败, 其他 打包后数据的长度
 * @说明: 
 * @作者: yzy (2013/7/15)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint16 PST_DoneFrame(uint8 num, uint8* buffer)
{
    const PSTFrameStr* str;                 //定义指向帧信息结构体
    uint16 ut_i;                            //循环变量
    uint16 len;                             //定义帧数据区长度
    uint16 sumlen;                          //待校验数据长度
    uint16 sumos;                           //校验和存放的位置
    uint16 cs;

    if(num >= PST_FRM_NO)                   //参数验证
    {
        return 0;
    }
                                            //获取当前要扫描的规约的结构体
    str = (PSTFrameStr*)gss_PSTFrameStr[num];
    
                                            //帧头的填写
    for(ut_i = 0; ut_i < str->hlen; ut_i ++)
    {
        if(str->head[ut_i] != PST_FRM_ANY)
        {
            buffer[ut_i] = str->head[ut_i];
        }
    }

    if(str->latr & PST_LATR_NOLEN)          //没有数据区长度
    {
        return 0;                           //没有长度区的话后面没法打包
    }
    else                                    //有数据区长度
    {
        if(str->latr & PST_LATR_DBLEN)      //如果是双字节长度
        {
            if(str->latr &  PST_LATR_BFLEN) //高字节在前
            {
                len = buffer[str->lp + 1]    //数据长度
                    + (buffer[str->lp ] << 8);
            }
            else                            //低字节在前
            {
                len = buffer[str->lp]        //数据长度
                    + (buffer[str->lp + 1] << 8);
            }
        }
        else                                //否则就是单字节的帧长
        {
            len = buffer[str->lp];          //数据长度
        }
        
        if(str->latr & PST_LATR_DBLEN)      //如果是双字节长度
        {
            len <<= str->llft;
            len &= 0xffff;
            len >>= str->llft;
            len >>= str->lrft;
        }
        else
        {
            len <<= str->llft;
            len &= 0xff;
            len >>= str->llft;
            len >>= str->lrft;
        }
    }
    
    if(len + str->extlen > PST_LEN_MAX)     //如果数据区长度与最小帧长度之后大于扫描的帧长那么肯定不是一个有效帧
    {
        return 0;                           //返回不是有效帧    
    }
    
    if(!(str->atr & PST_ATR_PARIT))         //有校验位
    {
                                            //计算待校验数据的长度
        sumlen = len + str->extlen;
        if(str->cssl & 0x80)                //负
        {
            if(sumlen > (str->cssl & 0x7F))
            {
                sumlen -= (str->cssl & 0x7F);
            }
            else
            {
                return PST_ERR_NE;
            }
        }
        else                                //正
        {
            sumlen += (str->cssl & 0x7F);
        }
                                            //计算校验和存放的位置偏移
        sumos = len + str->extlen;
        if(str->csco & 0x80)                //负
        {
            if(sumos > (str->csco & 0x7F))
            {
                sumos -= (str->csco & 0x7F);
            }
            else
            {
                return PST_ERR_NE;
            }
        }
        else                                //正
        {
            sumos += (str->csco & 0x7F);
        }
        
        switch(str->atr & PST_ATR_CSCAL)
        {
            case PST_ATR_CSCAL_SC:            
                cs = DoSum(                 //进行累加和运算
                    buffer + str->cssp, 
                    sumlen
                    );
                                            //累加和赋值
                buffer[sumos] = (uint8)cs;
                break;
                
            case PST_ATR_CSCAL_SD:            
                cs = DoSum(                 //进行累加和运算
                    buffer + str->cssp, 
                    sumlen
                    );
                                            //累加和赋值
                IntToByte(buffer + sumos, cs);
                break;
                
            case PST_ATR_CSCAL_BS:
                cs = DoBitSum(              //进行位加和运算
                    buffer + str->cssp, 
                    sumlen
                    );
                                            //累加和赋值
                buffer[len + sumos] = (uint8)cs;
                break;
                
            case PST_ATR_CSCAL_CRC1:
                cs = DoCrcDlms(
                    buffer + str->cssp, 
                    sumlen,
                    0xffff
                    );
                IntToByte(buffer + sumos, cs);
                break;

            default:
                return 0;                   //返回打包失败
        }
        len += str->extlen - 1;             //长度增加
    }

    if(!(str->atr & PST_ATR_END))           //有结尾符
    {
//	        buffer[len] = str->end;             //判断是否为结尾符            
//	        len++;                              //长度增加
        memcpy(&buffer[len], str->end, str->elen);
        len += str->elen;
    }
    return len;                             //返回长度
}

/******************************************************************************
**各个规约解析函数散转列表
******************************************************************************/
uint8 (*const gfs_PSTDecode[])(uint8* rbuffer, uint8* sbuffer, uint16 sbuflen, PST_Frame* frm) = 
{
#if (PST_FRM_BG_EN > 0)
    PST_BG_Decode,
#endif

#if (PST_FRM_ZY_EN > 0)
    PST_ZY_Decode,
#endif

#if (PST_FRM_ZBF_EN > 0)
    PST_ZBF_Decode,
#endif

#if (PST_FRM_GS_EN > 0)
    PST_GS_Decode,
#endif

#if (PST_FRM_WM_EN > 0)
    PST_WM_Decode,
#endif

#if (PST_FRM_GD_1_EN > 0)
    PST_GD_1_Decode,
#endif

#if (PST_FRM_GD_2_EN > 0)
    PST_GD_2_Decode,
#endif

#if (PST_FRM_EM_EN > 0)
    PST_EM_Decode,
#endif

#if (PST_FRM_587_EN > 0)
    PST_587_Decode,
#endif
    
#if (PST_FRM_DLMS_HDLC_EN > 0)
    PST_DLMS_HDLC_Decode,
#endif
    
#if (PST_FRM_DLMS_IP_EN > 0)
    PST_DLMS_IP_Decode,
#endif
    
#if (PST_FRM_EDMI_EN > 0)
    PST_EDMI_Decode,
#endif
   
#if (PST_FRM_DM_EN > 0)   //zwj   南京模块
    PST_DM_Decode,
#endif
#if (PST_FRM_XML_EN > 0)   //zwj   南京模块
    PST_XML_Decode,
#endif

};


/*******************************************************************************
 * @function_name:  FRM_Decode
 * @function_file:  dev_frm.c
 * @描述: 规约解析函数
 * 
 * 
 * @参数: 
 * @param: protocal  规约序号
 * @param: rbuffer  接收缓存
 * @param: sbuffer  发送缓存
 * @param: sbuflen  发送缓存长度
 * @param: frm  规约帧结构体信息指针
 * 
 * @返回: 
 * @return:  uint8   
 * @作者: yzy (2010-02-27)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
uint8 FRM_Decode(uint8 protocal, uint8* rbuffer, uint8* sbuffer, uint16 sbuflen, PST_Frame* frm)
{
    SYS_VAR_CHECK(protocal >= PST_FRM_NO);   //参数检查
    return gfs_PSTDecode[protocal](rbuffer, sbuffer, sbuflen, frm);         //散转处理
}



/******************************************************************************
**各个规约处理散转函数
******************************************************************************/
uint8 (*const gfs_PSTProc[])(PST_Frame* frm) = 
{
#if (PST_FRM_BG_EN > 0)
    PST_BG_Proc,
#endif

#if (PST_FRM_ZY_EN > 0)
    PST_ZY_Proc,
#endif

#if (PST_FRM_ZBF_EN > 0)
    PST_ZBF_Proc,
#endif

#if (PST_FRM_GS_EN > 0)
    PST_GS_Proc,
#endif

#if (PST_FRM_WM_EN > 0)
    PST_WM_Proc,
#endif

#if (PST_FRM_GD_1_EN > 0)
    PST_GD_1_Proc,
#endif

#if (PST_FRM_GD_2_EN > 0)
    PST_GD_2_Proc,
#endif

#if (PST_FRM_EM_EN > 0)
    PST_EM_Proc,
#endif

#if (PST_FRM_587_EN > 0)
    PST_587_Proc,
#endif

#if (PST_FRM_DLMS_HDLC_EN > 0)
    PST_DLMS_HDLC_Proc,
#endif
    
#if (PST_FRM_DLMS_IP_EN > 0)
    PST_DLMS_IP_Proc,
#endif
    
#if (PST_FRM_EDMI_EN > 0)
    PST_EDMI_Proc,
#endif
    
#if (PST_FRM_DM_EN > 0)    //zwj  南京模块
    PST_DM_Proc,
#endif
#if (PST_FRM_XML_EN > 0)
    PST_XML_Proc,
#endif

};

/*******************************************************************************
 * @function_name:  FRM_Proc
 * @function_file:  dev_frm.c
 * @描述: 规约处理函数
 * 
 * 
 * @参数: 
 * @param: protocal  规约序号
 * @param: frm  规约帧结构体信息指针
 * 
 * @返回: 
 * @return:  uint8   
 * @作者: yzy (2010-03-03)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
uint8 FRM_Proc(uint8 protocal, PST_Frame* frm)
{
    SYS_VAR_CHECK(protocal >= PST_FRM_NO);   //参数检验
    return gfs_PSTProc[protocal](frm);           //散转处理
}




/************************************************************************
 * @function: PST_CheckFrame2
 * @描述: 指定类型帧有效性的检查(若存在前导符,则去掉前导符./不去除帧后的无效字符)
 * 
 * @参数: 
 * @param: nfrm 通讯的规约编号.如果nfrm为0xff表示为中继
 * @param: buffer 帧缓存
 * @param: len 帧长度
 * 
 * @返回: 
 * @return: uint16 合格帧的长度(0表示没有合格帧)
 * @说明: 
 * @作者: yzy (2012-5-15)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint16 PST_CheckFrame2(uint8 nfrm, uint8* buffer, uint16 len)
{
    for(uint8 uc_i = 0; (uc_i < 7) && (len - uc_i > 0); uc_i++)
    {
        if(PST_CheckFrame(nfrm, buffer + uc_i, len - uc_i) == PST_ERR_OK)
        {
            memcpy(buffer, buffer + uc_i, len - uc_i);
            return (len - uc_i);
        }
    }
    return 0;
}


/************************************************************************
 * @function: PST_GetFrmLen
 * @描述: 获得buffer中一个有效帧的长度
 * 
 * @参数: 
 * @param: nfrm 通讯的规约编号.如果nfrm为0xff表示为中继
 * @param: buffer 帧缓存
 * @param: len 帧长度
 * 
 * @返回: 
 * @return: uint16 单帧的长度(0表示没有合格帧)
 * @说明: 
 * @作者: 
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint16 PST_GetFrmLen(uint8 nfrm, uint8* buffer)
{
    const PSTFrameStr* str;                 //定义指向帧信息结构体
    uint8 len;
    len = 0;
    
    if(nfrm >= PST_FRM_NO)                   //参数验证
    {
        return 0;
    }
    
    str = (PSTFrameStr*)gss_PSTFrameStr[nfrm];
    if(str->latr & PST_LATR_NOLEN)          //没有数据区长度
    {
        return 0;                           //没有长度区的话后面没法打包
    }
    else                                    //有数据区长度
    {
        if(str->latr & PST_LATR_DBLEN)      //如果是双字节长度
        {
            if(str->latr &  PST_LATR_BFLEN) //高字节在前
            {
                len = buffer[str->lp + 1]    //数据长度
                    + (buffer[str->lp ] << 8);
            }
            else                            //低字节在前
            {
                len = buffer[str->lp]        //数据长度
                    + (buffer[str->lp + 1] << 8);
            }
        }
        else                                //否则就是单字节的帧长
        {
            len = buffer[str->lp];          //数据长度
        }
        
        if(str->latr & PST_LATR_DBLEN)      //如果是双字节长度
        {
            len <<= str->llft;
            len &= 0xffff;
            len >>= str->llft;
            len >>= str->lrft;
        }
        else
        {
            len <<= str->llft;
            len &= 0xff;
            len >>= str->llft;
            len >>= str->lrft;
        }
    }
    
    len += str->extlen;     //帧的长度
    return len;   
}

