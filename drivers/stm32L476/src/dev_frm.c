/****************************************Copyright (c)*************************
**                               	________科技有限公司
**                                     		开发部
**
**
**--------------文件信息-------------------------------------------------------
**文   件   名: dev_frm.c
**创   建   人: yzy
**最后修改日期: 2010年02月26日
**描        述: 规约处理程序
**注        意:
**--------------历史版本信息---------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2010年02月26日
** 描　述: 原始版本
******************************************************************************/
#define EXT_DEV_FRM


#include "aos/include.h"
//	#include "k_types.h"
//	#include "k_api.h"
//	#include "lib_app.h"
#include "pst_vs.h"
#include "pst_zy.h"

#define NETP_FRM_BS_96_8_1_N    0x18 //9600波特率  1停止位 无校验 8位数据位
#define NETP_FRM_BS_96_8_1_O    0x78 //9600波特率  1停止位 偶校验 8位数据位
#define NETP_FRM_BS_48_8_1_N    0x1D //9600波特率  1停止位 无校验 8位数据位


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
    PST_FRM_BG_END,
    PST_FRM_BG_CSSP,
    PST_FRM_BG_MLEN,
    PST_FRM_BG_ATR,
    PST_FRM_BG_CSSL,
    PST_FRM_BG_CSOS,
};


#endif                                  //PST_FRM_BG_EN



/******************************************************************************
**威星规约信息
******************************************************************************/
#if PST_FRM_VS_EN > 0
const uint8 gucs_PSTVSHead[] =          //定义部规的头
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


#define PST_FRM_VS_LP       0x0a        //定义长度起始的位置
#define PST_FRM_VS_LATR     (PST_LATR_DBLEN)
#define PST_FRM_VS_END      0x16        //定义结尾符
#define PST_FRM_VS_CSSP     0x00        //定义累加和计算起始位置
#define PST_FRM_VS_ATR      (PST_ATR_CSCAL_SC)
#define PST_FRM_VS_MLEN     0x17        //最小帧的长度
#define PST_FRM_VS_CSSL     0x82        //定义累加和计算的长度偏移
#define PST_FRM_VS_CSOS     0x82        //定义累加和的位置偏移


/******************************************************************************
**部规规约结构体信息
******************************************************************************/
const PSTFrameStr gs_PSTFrameVSStr = 
{
    (uint8*)gucs_PSTVSHead,
    (uint8)sizeof(gucs_PSTVSHead),
    PST_FRM_VS_LP,
    PST_FRM_VS_LATR,
    0,
    0,
    PST_FRM_VS_END,
    PST_FRM_VS_CSSP,
    PST_FRM_VS_MLEN,
    PST_FRM_VS_ATR,
    PST_FRM_VS_CSSL,
    PST_FRM_VS_CSOS,
};


#endif                                  //PST_FRM_VS_EN






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
    PST_FRM_ZY_END,
    PST_FRM_ZY_CSSP,
    PST_FRM_ZY_MLEN,
    PST_FRM_ZY_ATR,
    PST_FRM_ZY_CSSL,
    PST_FRM_ZY_CSOS,
};

#endif                                  //PST_FRM_ZY_EN





#if PST_FRM_MB_EN1 > 0

const uint8 gucs_PSTMBHead[] = 
{
    PST_FRM_ANY,
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
    PST_FRM_WM_END,
    PST_FRM_WM_CSSP,
    PST_FRM_WM_MLEN,
    PST_FRM_WM_ATR,
    PST_FRM_WM_CSSL,
    PST_FRM_WM_CSOS,
};


#endif   





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
    PST_FRM_WM_END,
    PST_FRM_WM_CSSP,
    PST_FRM_WM_MLEN,
    PST_FRM_WM_ATR,
    PST_FRM_WM_CSSL,
    PST_FRM_WM_CSOS,
};


#endif                                  //PST_FRM_WM_EN

const NetpFrameDataStr gs_NetpFrameWXICPrepayWDataStr = 
{
    0x10,
    7,
    {0x00,0xd0,0,0x12},
    4,
    1,
    1,
    {
        {NETP_D_Ty_WXIC_PREPAY,18,NETP_WRITE_WXIC_PREPAY,NETP_D_B_DEV_ADDR}
    },
    {
        {NETP_D_Ty_WXIC_PREPAY_HEX, 2, NETP_WRITE_WXIC_PREPAY_RET, 0},
    },
    
};


const NetpFrameDataStr gs_NetpFrameWXICPrepayRDataStr = 
{
    0x03,
    23,
    {0x00,0xd0,0,0x12},
    2,
    1,
    1,
    {
        {NETP_D_STR_DATA_TO_BUF,2,NETP_WRITE_WXIC_PREPAY,0}
    },
    {
        {NETP_D_Ty_WXIC_PREPAY_HEX, 18, NETP_WRITE_WXIC_PREPAY_RET, 0},
    },
    
};


const NetpFrameDataStr gs_NetpFrameWXICRechargeWDataStr = 
{
    0x10,
    27,
    {0,0x80,0,0x0a},
    4,
    1,
    1,
    {
        {NETP_D_STR_DATA_TO_BUF,10,NETP_WRITE_WXIC_RECHARGE,0}
    },
    {
        {NETP_D_Ty_S_HEX, 22, NETP_WRITE_WXIC_RECHARGE_RET, 0},
    },
    
};




const NetpFrameDataStr gs_NetpFrameWXICPriceWDataStr = 
{
    0x10,
    7,
    {0,0xA0,0,0x2F},
    4,
    1,
    1,
    {
        {NETP_D_STR_DATA_TO_BUF,47,NETP_WRITE_WXIC_PRICE,0}
    },
    {
        {NETP_D_Ty_WXIC_PREPAY_HEX, 2, NETP_WRITE_WXIC_PRICE_RET, 0},
    },
    
};
        
const NetpFrameDataStr gs_NetpFrameWXICVlvWDataStr  = 
{
    0x05,
    8,
    {0,0xAC,0,0},
    2,
    1,
    1,
    {
        {NETP_D_STR_DATA_TO_BUF,2,0,0}
    },
    {
        {NETP_D_Ty_HEX, 2, NETP_COMMON, 0},
    },
    
};



const NetpFrameDataStr gs_NetpFrameWXICPriceRDataStr = 
{
    0x03,
    52,
    {0,0xA0,0,0},
    2,
    1,
    1,
    {
        {NETP_D_STR_DATA_TO_BUF,2,NETP_WRITE_WXIC_PRICE,0}
    },
    {
        {NETP_D_Ty_WXIC_PREPAY_HEX, 47, NETP_WRITE_WXIC_PRICE_RET, 0},
    },
    
};


#if CID_WXIC > 0

const uint8 gucs_NetpWXICHead[] =          //WXIC卡控制器
{
    0xDD,
};
    
#define NETP_FRM_WXIC_LP       0x02        //定义长度起始的位置
#define NETP_FRM_WXIC_LATR     (PST_LATR_NOLEN)
#define NETP_FRM_WXIC_DLP       0x02        //定义数据域起始的位置

#define NETP_FRM_WXIC_END      00        //定义结尾符
#define NETP_FRM_WXIC_CSSP     0x00        //定义累加和计算起始位置
#define NETP_FRM_WXIC_ATR      (PST_ATR_CSCAL_CRC3|PST_ATR_END)
#define NETP_FRM_WXIC_MLEN     4        //最小帧的长度
#define NETP_FRM_WXIC_CSSL     0x82        //定义累加和计算的长度偏移
#define NETP_FRM_WXIC_CSOS     0x82        //定义累加和的位置偏移
#define NETP_FRM_WXIC_DIATR    (0)//低位是DI跟帧头位置

#define NETP_FRM_COMP_WXIC_LP       0x00        //定义长度起始的位置
#define NETP_FRM_COMP_WXIC_LATR     (PST_LATR_NOLEN)
#define NETP_FRM_COMP_WXIC_ADLEN     0x01       //地址域偏移
#define NETP_FRM_COMP_WXIC_ADLP      0x00//高位1表示 地址域长度要累加 0表示不需要累加
#define NETP_FRM_COMP_WXIC_CMDLP     0x01
#define NETP_FRM_COMP_WXIC_DIATR    (PST_DIATR_DB_MB | 0x02)//低位是DI跟帧头位置

#define NETP_FRM_COMP_WXIC_BIT (uint16_t)(PST_FRM_BIT_HEAD|PST_FRM_BIT_ADDR \
                                     | PST_FRM_BIT_CMD | PST_FRM_BIT_DATA \
                                     | PST_FRM_BIT_DI | PST_FRM_BIT_CS)
#define NETP_FRM_COMP_WXIC_LEN_BIT (uint16_t)(0)

#define NETP_FRM_WXIC_BS    0x18 //9600波特率  1停止位 无校验 8位数据位

/******************************************************************************
**____扩展规约结构体信息
******************************************************************************/
const NetpFrameStr gs_NetpFrameWXICStr = 
{
    {PST_FRM_ANY},
    1,
    NETP_FRM_WXIC_LP,
    NETP_FRM_WXIC_DLP,
    NETP_FRM_WXIC_LATR,
    0,
    0,
    NETP_FRM_WXIC_END,
    NETP_FRM_WXIC_CSSP,
    NETP_FRM_WXIC_MLEN,
    NETP_FRM_WXIC_ATR,
    NETP_FRM_WXIC_CSSL,
    NETP_FRM_WXIC_CSOS,
    1,
    NETP_FRM_COMP_WXIC_CMDLP,
    NETP_FRM_WXIC_DIATR,
};
    
const NetpComposeFrameStr gs_NetpCompFrameWXICStr = 
{
    {PST_FRM_ANY},
    1,
    NETP_FRM_COMP_WXIC_LP,
    NETP_FRM_COMP_WXIC_LATR,
    0,
    0,
    NETP_FRM_WXIC_END,
    NETP_FRM_WXIC_CSSP,
    NETP_FRM_WXIC_ATR,
    NETP_FRM_WXIC_CSSL,    
    NETP_FRM_COMP_WXIC_ADLEN,
    NETP_FRM_COMP_WXIC_ADLP,
    1,
    NETP_FRM_COMP_WXIC_CMDLP,
    NETP_FRM_COMP_WXIC_DIATR,
    NETP_FRM_COMP_WXIC_BIT,
    NETP_FRM_COMP_WXIC_LEN_BIT,
};

const NetpFrameDataStr gs_NetpFrameWXICDataStr = 
{
    0x03,
    23,
    {0x00,0x00,0x00,0x09},
    4,
    1,
    7,
    {
        {NETP_D_Ty_A_N,0,0,0}
    },
    {
        {NETP_D_WXIC_LONG, 4, NETP_READ_REMAIND_GAS, 0},
        {NETP_D_WXIC_LONG, 4, NETP_READ_ACCUMULATE_TOTAL, 0}, 
        {NETP_D_WXIC_LONG, 4, NETP_READ_RECHARGE_GAS, 0},
        {NETP_D_WXIC_LONG,4,NETP_READ_RECHARGE_TOTAL_GAS,0},
//        {NETP_D_Ty_P_FLOAT,1,NETP_READ_VOL,0},
//        {NETP_D_Ty_WS_HEX,1,NETP_READ_VALVE,0},
//        {NETP_D_Ty_S_HEX,1,NETP_READ_STT,0},
    },
    
};



const ST_NETP_FW_BASE gs_netp_fw_wxic = 
{
    CID_WXIC,
    {'W','X','I','C'},
    NETP_FRM_WXIC_BS,
    
    1,
    {
        {PST_FRM_ANY},
        1,
        NETP_FRM_WXIC_LP,
        NETP_FRM_WXIC_DLP,
        NETP_FRM_WXIC_LATR,
        0,
        0,
        NETP_FRM_WXIC_END,
        NETP_FRM_WXIC_CSSP,
        NETP_FRM_WXIC_MLEN,
        NETP_FRM_WXIC_ATR,
        NETP_FRM_WXIC_CSSL,
        NETP_FRM_WXIC_CSOS,
        1,
        NETP_FRM_COMP_WXIC_CMDLP,
        NETP_FRM_WXIC_DIATR,
        NETP_FRM_COMP_WXIC_ADLEN,
        NETP_FRM_COMP_WXIC_ADLP,
    },
    {
        {PST_FRM_ANY},
        1,
        NETP_FRM_COMP_WXIC_LP,
        NETP_FRM_COMP_WXIC_LATR,
        0,
        0,
        NETP_FRM_WXIC_END,
        NETP_FRM_WXIC_CSSP,
        NETP_FRM_WXIC_ATR,
        NETP_FRM_WXIC_CSSL,    
        NETP_FRM_COMP_WXIC_ADLEN,
        NETP_FRM_COMP_WXIC_ADLP,
        1,
        NETP_FRM_COMP_WXIC_CMDLP,
        NETP_FRM_COMP_WXIC_DIATR,
        NETP_FRM_COMP_WXIC_BIT,
        NETP_FRM_COMP_WXIC_LEN_BIT,
    },
    //NetpFrameDataStr frameDtaStr[MAX_NETP_CP_FRAME_NUM];
    {
        {
            0x03,
            64,
            {0x00,0x20,0x00,0x3B},
            4,
            1,
            6,
            {
                {NETP_D_Ty_A_N,0,0,0}
            },
            {
                {NETP_D_Ty_STR, 1, NETP_COMMON, 0},
                {NETP_D_Ty_STR, 6, NETP_READ_ACCUMULATE_TOTAL_PREPAY, 0},
                {NETP_D_Ty_STR, 6, NETP_READ_REMAIND_GAS, 0}, 
                {NETP_D_Ty_STR, 6, NETP_READ_CUR_PRICE, 0},
                {NETP_D_Ty_STR, 4, NETP_READ_WXIC_METER_STT,0},
                {NETP_D_Ty_STR, 3, NETP_READ_VOL_HADFLOW,0},
            },
        
        },
    },
    //(EFT_MIN_ST_BIT|EFT_MIN_SI_BIT|EFT_MIN_TMP_BIT|EFT_MIN_P_BIT),
    {
        0x10,
        7,
        {0x00,0xd0,0,0x12},
        4,
        1,
        1,
        {
            {NETP_D_Ty_WXIC_PREPAY,18,NETP_WRITE_WXIC_PREPAY,NETP_D_B_DEV_ADDR}
        },
        {
            {NETP_D_Ty_WXIC_PREPAY_HEX, 2, NETP_WRITE_WXIC_PREPAY_RET, 0},
        },
        
    },


    {
        0x03,
        23,
        {0x00,0xd0,0,0x12},
        2,
        1,
        1,
        {
            {NETP_D_STR_DATA_TO_BUF,2,NETP_WRITE_WXIC_PREPAY,0}
        },
        {
            {NETP_D_Ty_WXIC_PREPAY_HEX, 18, NETP_WRITE_WXIC_PREPAY_RET, 0},
        },
        
    },






    {
        0x10,
        7,
        {0,0xA0,0,0x2F},
        4,
        1,
        1,
        {
            {NETP_D_STR_DATA_TO_BUF,47,NETP_WRITE_WXIC_PRICE,0}
        },
        {
            {NETP_D_Ty_WXIC_PREPAY_HEX, 2, NETP_WRITE_WXIC_PRICE_RET, 0},
        },
        
    },
        

    {
        0x03,
        52,
        {0,0xA0,0,0},
        2,
        1,
        1,
        {
            {NETP_D_STR_DATA_TO_BUF,2,NETP_WRITE_WXIC_PRICE,0}
        },
        {
            {NETP_D_Ty_WXIC_PREPAY_HEX, 47, NETP_WRITE_WXIC_PRICE_RET, 0},
        },
        
    },    
    {
        0x10,
        27,
        {0,0x80,0,0x0a},
        4,
        1,
        1,
        {
            {NETP_D_STR_DATA_TO_BUF,10,NETP_WRITE_WXIC_RECHARGE,0}
        },
        {
            {NETP_D_Ty_S_HEX, 22, NETP_WRITE_WXIC_RECHARGE_RET, 0},
        },
        
    },
    {
        0x05,
        8,
        {0,0xAC,0,0},
        2,
        1,
        1,
        {
            {NETP_D_STR_DATA_TO_BUF,2,0,0}
        },
        {
            {NETP_D_Ty_HEX, 2, NETP_COMMON, 0},
        },
        
    },
    0xA5A5,
};
#endif

#ifdef CID_TXV13 

const uint8 gucs_NetpTX13Head[] =          //天信13
{
    0xCC,
};
    
#define NETP_FRM_TX13_LP       0x03        //定义长度起始的位置
#define NETP_FRM_TX13_LATR     (PST_LATR_DBLEN)
#define NETP_FRM_TX13_DLP       0x05        //定义数据域起始的位置

#define NETP_FRM_TX13_END      0xEE        //定义结尾符
#define NETP_FRM_TX13_CSSP     0x00        //定义累加和计算起始位置
#define NETP_FRM_TX13_ATR      (PST_ATR_CSCAL_SD)
#define NETP_FRM_TX13_MLEN     8        //最小帧的长度
#define NETP_FRM_TX13_CSSL     0x83        //定义累加和计算的长度偏移
#define NETP_FRM_TX13_CSOS     0x83        //定义累加和的位置偏移
#define NETP_FRM_COMP_TX13_ATR      (PST_ATR_CSCAL_SD1)

#define NETP_FRM_COMP_TX13_LP       0x03        //定义长度起始的位置
#define NETP_FRM_COMP_TX13_LATR     (PST_LATR_NOLEN)
#define NETP_FRM_COMP_TX13_ADLEN     0x01
#define NETP_FRM_COMP_TX13_ADLP      0x81//高位1表示 地址域长度要累加 0表示不需要累加
#define NETP_FRM_COMP_TX13_CMDLP     0x02
#define NETP_FRM_COMP_TX13_DIATR    PST_DIATR_NONE

#define NETP_FRM_COMP_TX13_BIT (uint16_t)(PST_FRM_BIT_HEAD|PST_FRM_BIT_ADDR \
                                     | PST_FRM_BIT_CMD | PST_FRM_BIT_DATA \
                                     | PST_FRM_BIT_CS | PST_FRM_BIT_TAIL)
#define NETP_FRM_COMP_TX13_LEN_BIT (uint16_t)(0)

#define NETP_FRM_TX13_BS    0x18 //9600波特率  1停止位 无校验 8位数据位

/******************************************************************************
**____扩展规约结构体信息
******************************************************************************/
const NetpFrameStr gs_NetpFrameTX13Str = 
{
    {0xCC},
    1,
    NETP_FRM_TX13_LP,
    NETP_FRM_TX13_DLP,
    NETP_FRM_TX13_LATR,
    0,
    0,
    NETP_FRM_TX13_END,
    NETP_FRM_TX13_CSSP,
    NETP_FRM_TX13_MLEN,
    NETP_FRM_TX13_ATR,
    NETP_FRM_TX13_CSSL,
    NETP_FRM_TX13_CSOS,
    1,
    NETP_FRM_COMP_TX13_CMDLP,
    
    NETP_FRM_COMP_TX13_DIATR,
};
    
const NetpComposeFrameStr gs_NetpCompFrameTX13Str = 
{
    {0xCC},
    1,

    NETP_FRM_COMP_TX13_LP,
    NETP_FRM_COMP_TX13_LATR,
    0,
    0,
    NETP_FRM_TX13_END,
    NETP_FRM_TX13_CSSP,
    NETP_FRM_COMP_TX13_ATR,
    NETP_FRM_TX13_CSSL,    
    NETP_FRM_COMP_TX13_ADLEN,
    NETP_FRM_COMP_TX13_ADLP,
    1,
    NETP_FRM_COMP_TX13_CMDLP,
    NETP_FRM_COMP_TX13_DIATR,
    NETP_FRM_COMP_TX13_BIT,
    NETP_FRM_COMP_TX13_LEN_BIT,
};

//阶-1有效
#define TX13_FLOAT_RANK_VALID                 0x80
#define TX13_FLOAT_RANK_MASK                  0x80
//阶长度
#define TX13_FLOAT_RANK_LEN_2               0x40 
#define TX13_FLOAT_RANK_LEN_MASK            0x60
//整数长度
#define TX13_FLOAT_INTEGER_LEN_4             0x10
#define TX13_FLOAT_INTEGER_LEN_MASK          0x1C
//小数长度
#define TX13_FLOAT_DEC_LEN_2                 0x02
#define TX13_FLOAT_DEC_LEN_MASK              0x03

const NetpFrameDataStr gs_NetpFrameTX13DataStr = 
{
    0x30,
    36,
    {0},
    0,
    1,
    7,
    {
        {NETP_D_Ty_A_N,14,0,0}
    },
    {
        {NETP_D_Ty_T_BCD1, 7, NETP_READ_TIME, 0},
        {NETP_D_Ty_FLOAT, 4, NETP_READ_CUR_FLOW, 0}, 
        {NETP_D_Ty_DOUBLE, 6, NETP_READ_ACCUMULATE_TOTAL, (uint8_t)((TX13_FLOAT_RANK_VALID|TX13_FLOAT_RANK_LEN_2|TX13_FLOAT_INTEGER_LEN_4|TX13_FLOAT_DEC_LEN_2))},
        {NETP_D_Ty_TM_FLOAT,4,NETP_READ_TEMP,0},
        {NETP_D_Ty_P_FLOAT,4,NETP_READ_PRESSURE,0},
        {NETP_D_Ty_WS_HEX,2,NETP_READ_WARN_FLAG,0},
        {NETP_D_Ty_S_HEX,1,NETP_READ_STT,0},
    },
    
};

const ST_NETP_FW_BASE gs_netp_fw_tx13 = 
{
    CID_TXV13,
    {'T','X','1','3'},
    NETP_FRM_TX13_BS,
    
    1,
    {
        {0xCC},
        1,
        NETP_FRM_TX13_LP,
        NETP_FRM_TX13_DLP,
        NETP_FRM_TX13_LATR,
        0,
        0,
        NETP_FRM_TX13_END,
        NETP_FRM_TX13_CSSP,
        NETP_FRM_TX13_MLEN,
        NETP_FRM_TX13_ATR,
        NETP_FRM_TX13_CSSL,
        NETP_FRM_TX13_CSOS,
        1,
        NETP_FRM_COMP_TX13_CMDLP,
        
        NETP_FRM_COMP_TX13_DIATR,
        NETP_FRM_COMP_TX13_ADLEN,
        NETP_FRM_COMP_TX13_ADLP,
        
    },
    {
        {0xCC},
        1,
        
        NETP_FRM_COMP_TX13_LP,
        NETP_FRM_COMP_TX13_LATR,
        0,
        0,
        NETP_FRM_TX13_END,
        NETP_FRM_TX13_CSSP,
        NETP_FRM_COMP_TX13_ATR,
        NETP_FRM_TX13_CSSL,    
        NETP_FRM_COMP_TX13_ADLEN,
        NETP_FRM_COMP_TX13_ADLP,
        1,
        NETP_FRM_COMP_TX13_CMDLP,
        NETP_FRM_COMP_TX13_DIATR,
        NETP_FRM_COMP_TX13_BIT,
        NETP_FRM_COMP_TX13_LEN_BIT,
    },
    //NetpFrameDataStr frameDtaStr[MAX_NETP_CP_FRAME_NUM];
    {
        {
            0x30,
            36,
            {0},
            0,
            1,
            7,
            {
                {NETP_D_Ty_A_N,14,0,0}
            },
            {
                {NETP_D_Ty_T_BCD1, 7, NETP_READ_TIME, 0},
                {NETP_D_Ty_FLOAT, 4, NETP_READ_CUR_FLOW, 0}, 
                {NETP_D_Ty_DOUBLE, 6, NETP_READ_ACCUMULATE_TOTAL, 0},
                {NETP_D_Ty_FLOAT,4,NETP_READ_TEMP,0},
                {NETP_D_Ty_FLOAT,4,NETP_READ_PRESSURE,0},
                {NETP_D_Ty_WS_HEX,2,NETP_READ_WARN_FLAG,0},
                {NETP_D_Ty_S_HEX,1,NETP_READ_STT,0},
            },
        
        },
    },
    //(EFT_MIN_ST_BIT|EFT_MIN_SI_BIT|EFT_MIN_TMP_BIT|EFT_MIN_P_BIT),
//	    {{0}},{{0}},{{0}},{{0}},{{0}},{{0}},
//	    0xA5A5,
};
#endif
#if CID_XDMA > 0

const uint8 gucs_NetpXDMAHead[] =          //信东modbus
{
    0xDD,
};
#define NETP_FRM_XDMA_LATR     (PST_LATR_NOLEN)

#define NETP_FRM_XDMA_ATR      (PST_ATR_CSCAL_CRC3|PST_ATR_END)
#define NETP_FRM_XDMA_MLEN     4        //最小帧的长度
#define NETP_FRM_XDMA_DIATR    (0)//低位是DI跟帧头位置

#define NETP_FRM_COMP_XDMA_LP       0x00        //定义长度起始的位置
#define NETP_FRM_COMP_XDMA_LATR     (PST_LATR_NOLEN)
#define NETP_FRM_COMP_XDMA_ADLEN     0x01       //地址域偏移
#define NETP_FRM_COMP_XDMA_ADLP      0x00//高位1表示 地址域长度要累加 0表示不需要累加
#define NETP_FRM_COMP_XDMA_CMDLP     0x01
#define NETP_FRM_COMP_XDMA_DIATR    (PST_DIATR_DB_MB | 0x02)//低位是DI跟帧头位置

#define NETP_FRM_COMP_XDMA_BIT (uint16_t)(PST_FRM_BIT_HEAD|PST_FRM_BIT_ADDR \
                                     | PST_FRM_BIT_CMD | PST_FRM_BIT_DATA \
                                     | PST_FRM_BIT_DI | PST_FRM_BIT_CS)

#define NETP_FRM_XDMA_BS    0x18 //9600波特率  1停止位 无校验 8位数据位



const ST_NETP_FW_BASE gs_netp_fw_xdma = 
{
    CID_XDMA,
    {'X','D','M','A'},
    NETP_FRM_BS_96_8_1_N,
    
    1,
    {
        {PST_FRM_ANY},
        1,
        NETP_FRM_LP_02,
        NETP_FRM_DLP_02,
        PST_LATR_NOLEN,
        0,
        0,
        NETP_FRM_WITHOUT_END,
        NETP_FRM_CSSP_FROM_0,
        NETP_FRM_XDMA_MLEN,
        NETP_FRM_XDMA_ATR,
        NETP_FRM_CSSL_SUB_2,
        NETP_FRM_CSOS_SUB_2,
        1,
        NETP_FRM_COMP_XDMA_CMDLP,
        NETP_FRM_XDMA_DIATR,
        NETP_FRM_COMP_XDMA_ADLEN,
        NETP_FRM_COMP_XDMA_ADLP,
    },
    {
        {PST_FRM_ANY},
        1,
        NETP_FRM_COMP_XDMA_LP,
        NETP_FRM_COMP_XDMA_LATR,
        0,
        0,
        NETP_FRM_WITHOUT_END,
        NETP_FRM_CSSP_FROM_0,
        NETP_FRM_XDMA_ATR,
        NETP_FRM_CSSL_SUB_2,    
        NETP_FRM_COMP_XDMA_ADLEN,
        NETP_FRM_COMP_XDMA_ADLP,
        1,
        NETP_FRM_COMP_XDMA_CMDLP,
        NETP_FRM_COMP_XDMA_DIATR,
        NETP_FRM_COMP_XDMA_BIT,
        NETP_FRM_COMP_LEN_BIT_NULL,
    },
    //NetpFrameDataStr frameDtaStr[MAX_NETP_CP_FRAME_NUM];
    {
        {
            0x03,
            31,
            {0x00,0x00,0x00,0x0D},
            4,
            1,
            7,
            {
                {NETP_D_Ty_A_N,0,0,0}
            },
            {
                {NETP_D_Ty_STR, 1, NETP_COMMON, 0},
                {NETP_D_BIGEND_TO_LITTLE_6B, 6, NETP_READ_ACCUMULATE_TOTAL, 0},
                {NETP_D_BIGEND_TO_LITTLE_6B, 6, NETP_READ_WORKING_ACCUMULATE_TOTAL, 0}, 
                {NETP_D_BIG_TO_LIT_4B_DECI2, 4, NETP_READ_CUR_FLOW, 0},
                {NETP_D_BIG_TO_LIT_4B_DECI2, 4, NETP_READ_WORKING_CUR_FLOW,0},
                {NETP_D_BIG_TO_LIT_2B_DECI1, 2, NETP_READ_TEMP,0},
                {NETP_D_BIG_TO_LIT_4B_DECI1, 4, NETP_READ_PRESSURE,0},
                
            },
        
        },
    },
    {
        0x10,
    },


    {
        0x03,
       
    },






    {
        0x10,
    },
        

    {
        0x03,
    },    
    {
        0x10,
    },
    {
        0x05,
    },
    0xA5A5,
};
#endif

#if CID_SYMA > 0

#define NETP_FRM_XDMA_LATR     (PST_LATR_NOLEN)

#define NETP_FRM_WITHOUT_END      00        //定义结尾符
#define NETP_FRM_XDMA_ATR      (PST_ATR_CSCAL_CRC3|PST_ATR_END)
#define NETP_FRM_XDMA_MLEN     4        //最小帧的长度
//	#define NETP_FRM_CSSL_SUB_2     0x82        //定义累加和计算的长度偏移
//	#define NETP_FRM_CSOS_SUB_2     0x82        //定义累加和的位置偏移
#define NETP_FRM_XDMA_DIATR    (0)//低位是DI跟帧头位置

#define NETP_FRM_COMP_XDMA_LP       0x00        //定义长度起始的位置
#define NETP_FRM_COMP_XDMA_LATR     (PST_LATR_NOLEN)
#define NETP_FRM_COMP_XDMA_ADLEN     0x01       //地址域偏移
#define NETP_FRM_COMP_XDMA_ADLP      0x00//高位1表示 地址域长度要累加 0表示不需要累加
#define NETP_FRM_COMP_XDMA_CMDLP     0x01
#define NETP_FRM_COMP_XDMA_DIATR    (PST_DIATR_DB_MB | 0x02)//低位是DI跟帧头位置

#define NETP_FRM_COMP_XDMA_BIT (uint16_t)(PST_FRM_BIT_HEAD|PST_FRM_BIT_ADDR \
                                     | PST_FRM_BIT_CMD | PST_FRM_BIT_DATA \
                                     | PST_FRM_BIT_DI | PST_FRM_BIT_CS)
#define NETP_FRM_COMP_XDMA_LEN_BIT (uint16_t)(0)

#define NETP_FRM_XDMA_BS    0x18 //9600波特率  1停止位 无校验 8位数据位



const ST_NETP_FW_BASE gs_netp_fw_syma = 
{
    CID_SYMA,
    {'S','Y','M','A'},
    NETP_FRM_BS_96_8_1_N,
    
    1,
    {
        {PST_FRM_ANY},
        1,
        NETP_FRM_LP_02,
        NETP_FRM_DLP_02,
        PST_LATR_NOLEN,
        0,
        0,
        NETP_FRM_WITHOUT_END,
        NETP_FRM_CSSP_FROM_0,
        NETP_FRM_XDMA_MLEN,
        NETP_FRM_XDMA_ATR,
        NETP_FRM_CSSL_SUB_2,
        NETP_FRM_CSOS_SUB_2,
        1,
        NETP_FRM_COMP_XDMA_CMDLP,
        NETP_FRM_XDMA_DIATR,
        NETP_FRM_COMP_XDMA_ADLEN,
        NETP_FRM_COMP_XDMA_ADLP,
    },
    {
        {PST_FRM_ANY},
        1,
        NETP_FRM_COMP_XDMA_LP,
        NETP_FRM_COMP_XDMA_LATR,
        0,
        0,
        NETP_FRM_WITHOUT_END,
        NETP_FRM_CSSP_FROM_0,
        NETP_FRM_XDMA_ATR,
        NETP_FRM_CSSL_SUB_2,    
        NETP_FRM_COMP_XDMA_ADLEN,
        NETP_FRM_COMP_XDMA_ADLP,
        1,
        NETP_FRM_COMP_XDMA_CMDLP,
        NETP_FRM_COMP_XDMA_DIATR,
        NETP_FRM_COMP_XDMA_BIT,
        NETP_FRM_COMP_XDMA_LEN_BIT,
    },
    //NetpFrameDataStr frameDtaStr[MAX_NETP_CP_FRAME_NUM];
    {
        {
            0x03,
            31,
            {0x00,0x00,0x00,0x0D},
            4,
            1,
            7,
            {
                {NETP_D_Ty_A_N,0,0,0}
            },
            {
                {NETP_D_Ty_STR, 1, NETP_COMMON, 0},
                {NETP_D_BIGEND_TO_LITTLE_6B, 6, NETP_READ_ACCUMULATE_TOTAL, 0},
                {NETP_D_BIGEND_TO_LITTLE_6B, 6, NETP_READ_WORKING_ACCUMULATE_TOTAL, 0}, 
                {NETP_D_BIG_TO_LIT_4B_DECI2, 4, NETP_READ_CUR_FLOW, 0},
                {NETP_D_BIG_TO_LIT_4B_DECI2, 4, NETP_READ_WORKING_CUR_FLOW,0},
                {NETP_D_BIG_TO_LIT_2B_DECI1, 2, NETP_READ_TEMP,0},
                {NETP_D_BIG_TO_LIT_4B_DECI1, 4, NETP_READ_PRESSURE,0},
                
            },
        
        },
    },
    //(EFT_MIN_ST_BIT|EFT_MIN_SI_BIT|EFT_MIN_TMP_BIT|EFT_MIN_P_BIT),
    {
        0x10,
        7,
        {0x00,0xd0,0,0x12},
        4,
        1,
        1,
        {
            {NETP_D_Ty_WXIC_PREPAY,18,NETP_WRITE_WXIC_PREPAY,NETP_D_B_DEV_ADDR}
        },
        {
            {NETP_D_Ty_WXIC_PREPAY_HEX, 2, NETP_WRITE_WXIC_PREPAY_RET, 0},
        },
        
    },


    {
        0x03,
        23,
        {0x00,0xd0,0,0x12},
        2,
        1,
        1,
        {
            {NETP_D_STR_DATA_TO_BUF,2,NETP_WRITE_WXIC_PREPAY,0}
        },
        {
            {NETP_D_Ty_WXIC_PREPAY_HEX, 18, NETP_WRITE_WXIC_PREPAY_RET, 0},
        },
        
    },






    {
        0x10,
        7,
        {0,0xA0,0,0x2F},
        4,
        1,
        1,
        {
            {NETP_D_STR_DATA_TO_BUF,47,NETP_WRITE_WXIC_PRICE,0}
        },
        {
            {NETP_D_Ty_WXIC_PREPAY_HEX, 2, NETP_WRITE_WXIC_PRICE_RET, 0},
        },
        
    },
        

    {
        0x03,
        52,
        {0,0xA0,0,0},
        2,
        1,
        1,
        {
            {NETP_D_STR_DATA_TO_BUF,2,NETP_WRITE_WXIC_PRICE,0}
        },
        {
            {NETP_D_Ty_WXIC_PREPAY_HEX, 47, NETP_WRITE_WXIC_PRICE_RET, 0},
        },
        
    },    
    {
        0x10,
        27,
        {0,0x80,0,0x0a},
        4,
        1,
        1,
        {
            {NETP_D_STR_DATA_TO_BUF,10,NETP_WRITE_WXIC_RECHARGE,0}
        },
        {
            {NETP_D_Ty_S_HEX, 22, NETP_WRITE_WXIC_RECHARGE_RET, 0},
        },
        
    },
    {
        0x05,
        8,
        {0,0xAC,0,0},
        2,
        1,
        1,
        {
            {NETP_D_STR_DATA_TO_BUF,2,0,0}
        },
        {
            {NETP_D_Ty_HEX, 2, NETP_COMMON, 0},
        },
        
    },
    0xA5A5,
};
#endif
#if CID_CNV12 > 0//苍南V12modbus

#define NETP_FRM_XDMA_LATR     (PST_LATR_NOLEN)


#define NETP_FRM_WITHOUT_END      00        //定义结尾符
#define NETP_FRM_XDMA_ATR      (PST_ATR_CSCAL_CRC3|PST_ATR_END)
#define NETP_FRM_XDMA_MLEN     4        //最小帧的长度
//	#define NETP_FRM_CSSL_SUB_2     0x82        //定义累加和计算的长度偏移
//	#define NETP_FRM_CSOS_SUB_2     0x82        //定义累加和的位置偏移
#define NETP_FRM_XDMA_DIATR    (0)//低位是DI跟帧头位置

#define NETP_FRM_COMP_XDMA_LP       0x00        //定义长度起始的位置
#define NETP_FRM_COMP_XDMA_LATR     (PST_LATR_NOLEN)
#define NETP_FRM_COMP_XDMA_ADLEN     0x01       //地址域偏移
#define NETP_FRM_COMP_XDMA_ADLP      0x00//高位1表示 地址域长度要累加 0表示不需要累加
#define NETP_FRM_COMP_XDMA_CMDLP     0x01
#define NETP_FRM_COMP_XDMA_DIATR    (PST_DIATR_DB_MB | 0x02)//低位是DI跟帧头位置

#define NETP_FRM_COMP_XDMA_BIT (uint16_t)(PST_FRM_BIT_HEAD|PST_FRM_BIT_ADDR \
                                     | PST_FRM_BIT_CMD | PST_FRM_BIT_DATA \
                                     | PST_FRM_BIT_DI | PST_FRM_BIT_CS)
#define NETP_FRM_COMP_XDMA_LEN_BIT (uint16_t)(0)

#define NETP_FRM_XDMA_BS    0x18 //9600波特率  1停止位 无校验 8位数据位


//苍南V12
const ST_NETP_FW_BASE gs_netp_fw_cn12 = 
{
    CID_CNV12,
    {'C','N','1','2'},
    NETP_FRM_BS_96_8_1_O,
    
    1,
    {
        {PST_FRM_ANY},
        1,
        NETP_FRM_LP_02,
        NETP_FRM_DLP_02,
        PST_LATR_NOLEN,
        0,
        0,
        NETP_FRM_WITHOUT_END,
        NETP_FRM_CSSP_FROM_0,
        NETP_FRM_XDMA_MLEN,
        NETP_FRM_XDMA_ATR,
        NETP_FRM_CSSL_SUB_2,
        NETP_FRM_CSOS_SUB_2,
        1,
        NETP_FRM_COMP_XDMA_CMDLP,
        NETP_FRM_XDMA_DIATR,
        NETP_FRM_COMP_XDMA_ADLEN,
        NETP_FRM_COMP_XDMA_ADLP,
    },
    {
        {PST_FRM_ANY},
        1,
        NETP_FRM_COMP_XDMA_LP,
        NETP_FRM_COMP_XDMA_LATR,
        0,
        0,
        NETP_FRM_WITHOUT_END,
        NETP_FRM_CSSP_FROM_0,
        NETP_FRM_XDMA_ATR,
        NETP_FRM_CSSL_SUB_2,    
        NETP_FRM_COMP_XDMA_ADLEN,
        NETP_FRM_COMP_XDMA_ADLP,
        1,
        NETP_FRM_COMP_XDMA_CMDLP,
        NETP_FRM_COMP_XDMA_DIATR,
        NETP_FRM_COMP_XDMA_BIT,
        NETP_FRM_COMP_XDMA_LEN_BIT,
    },
    //NetpFrameDataStr frameDtaStr[MAX_NETP_CP_FRAME_NUM];
    {
        {
            0x03,
            37,
            {0x00,0x00,0x00,0x10},
            4,
            1,
            7,
            {
                {NETP_D_Ty_A_N,0,0,0}
            },
            {
                {NETP_D_Ty_STR, 1, NETP_COMMON, 0},
                {NETP_D_BIG_TO_LIT_8B_INT6B_DECI2B_DIV, 8, NETP_READ_WORKING_ACCUMULATE_TOTAL, 0},
                {NETP_D_BIG_TO_LIT_8B_INT6B_DECI2B_DIV, 8, NETP_READ_ACCUMULATE_TOTAL, 0}, 
                {NETP_D_BIG_TO_LIT_4B_INT3B_DECI1B_DIV, 4, NETP_READ_WORKING_CUR_FLOW, 0},
                {NETP_D_BIG_TO_LIT_4B_INT3B_DECI1B_DIV, 4, NETP_READ_CUR_FLOW,0},
                {NETP_D_BIG_TO_LIT_4B_INT3B_DECI1B_DIV, 4, NETP_READ_TEMP,0},
                {NETP_D_BIG_TO_LIT_4B_INT3B_DECI1B_DIV, 4, NETP_READ_PRESSURE,0},
                
            },
        
        },
    },
    //(EFT_MIN_ST_BIT|EFT_MIN_SI_BIT|EFT_MIN_TMP_BIT|EFT_MIN_P_BIT),
    {
        0x10,
        7,
        {0x00,0xd0,0,0x12},
        4,
        1,
        1,
        {
            {NETP_D_Ty_WXIC_PREPAY,18,NETP_WRITE_WXIC_PREPAY,NETP_D_B_DEV_ADDR}
        },
        {
            {NETP_D_Ty_WXIC_PREPAY_HEX, 2, NETP_WRITE_WXIC_PREPAY_RET, 0},
        },
        
    },


    {
        0x03,
        23,
        {0x00,0xd0,0,0x12},
        2,
        1,
        1,
        {
            {NETP_D_STR_DATA_TO_BUF,2,NETP_WRITE_WXIC_PREPAY,0}
        },
        {
            {NETP_D_Ty_WXIC_PREPAY_HEX, 18, NETP_WRITE_WXIC_PREPAY_RET, 0},
        },
        
    },






    {
        0x10,
        7,
        {0,0xA0,0,0x2F},
        4,
        1,
        1,
        {
            {NETP_D_STR_DATA_TO_BUF,47,NETP_WRITE_WXIC_PRICE,0}
        },
        {
            {NETP_D_Ty_WXIC_PREPAY_HEX, 2, NETP_WRITE_WXIC_PRICE_RET, 0},
        },
        
    },
        

    {
        0x03,
        52,
        {0,0xA0,0,0},
        2,
        1,
        1,
        {
            {NETP_D_STR_DATA_TO_BUF,2,NETP_WRITE_WXIC_PRICE,0}
        },
        {
            {NETP_D_Ty_WXIC_PREPAY_HEX, 47, NETP_WRITE_WXIC_PRICE_RET, 0},
        },
        
    },    
    {
        0x10,
        27,
        {0,0x80,0,0x0a},
        4,
        1,
        1,
        {
            {NETP_D_STR_DATA_TO_BUF,10,NETP_WRITE_WXIC_RECHARGE,0}
        },
        {
            {NETP_D_Ty_S_HEX, 22, NETP_WRITE_WXIC_RECHARGE_RET, 0},
        },
        
    },
    {
        0x05,
        8,
        {0,0xAC,0,0},
        2,
        1,
        1,
        {
            {NETP_D_STR_DATA_TO_BUF,2,0,0}
        },
        {
            {NETP_D_Ty_HEX, 2, NETP_COMMON, 0},
        },
        
    },
    0xA5A5,
};
#endif
#if CID_ETM1 > 0//埃尔斯特MODBUS(EK-205) 

    
#define NETP_FRM_XDMA_LATR     (PST_LATR_NOLEN)

#define NETP_FRM_XDMA_ATR      (PST_ATR_CSCAL_CRC3|PST_ATR_END)
#define NETP_FRM_XDMA_MLEN     4        //最小帧的长度
//	#define NETP_FRM_CSSL_SUB_2     0x82        //定义累加和计算的长度偏移
//	#define NETP_FRM_CSOS_SUB_2     0x82        //定义累加和的位置偏移
#define NETP_FRM_XDMA_DIATR    (0)//低位是DI跟帧头位置

#define NETP_FRM_COMP_XDMA_LP       0x00        //定义长度起始的位置
#define NETP_FRM_COMP_XDMA_LATR     (PST_LATR_NOLEN)
#define NETP_FRM_COMP_XDMA_ADLEN     0x01       //地址域偏移
#define NETP_FRM_COMP_XDMA_ADLP      0x00//高位1表示 地址域长度要累加 0表示不需要累加
#define NETP_FRM_COMP_XDMA_CMDLP     0x01
#define NETP_FRM_COMP_XDMA_DIATR    (PST_DIATR_DB_MB | 0x02)//低位是DI跟帧头位置

#define NETP_FRM_COMP_XDMA_BIT (uint16_t)(PST_FRM_BIT_HEAD|PST_FRM_BIT_ADDR \
                                     | PST_FRM_BIT_CMD | PST_FRM_BIT_DATA \
                                     | PST_FRM_BIT_DI | PST_FRM_BIT_CS)
#define NETP_FRM_COMP_XDMA_LEN_BIT (uint16_t)(0)



//埃尔斯特MODBUS(EK-205) 
const ST_NETP_FW_BASE gs_netp_fw_etek = 
{
    CID_ETM1,
    {'E','T','E','K'},
    NETP_FRM_BS_96_8_1_N,
    
    3,
    {
        {PST_FRM_ANY},
        1,
        NETP_FRM_LP_02,
        NETP_FRM_DLP_02,
        PST_LATR_NOLEN,
        0,
        0,
        NETP_FRM_WITHOUT_END,
        NETP_FRM_CSSP_FROM_0,
        NETP_FRM_XDMA_MLEN,
        NETP_FRM_XDMA_ATR,
        NETP_FRM_CSSL_SUB_2,
        NETP_FRM_CSOS_SUB_2,
        1,
        NETP_FRM_COMP_XDMA_CMDLP,
        NETP_FRM_XDMA_DIATR,
        NETP_FRM_COMP_XDMA_ADLEN,
        NETP_FRM_COMP_XDMA_ADLP,
    },
    {
        {PST_FRM_ANY},
        1,
        NETP_FRM_COMP_XDMA_LP,
        NETP_FRM_COMP_XDMA_LATR,
        0,
        0,
        NETP_FRM_WITHOUT_END,
        NETP_FRM_CSSP_FROM_0,
        NETP_FRM_XDMA_ATR,
        NETP_FRM_CSSL_SUB_2,    
        NETP_FRM_COMP_XDMA_ADLEN,
        NETP_FRM_COMP_XDMA_ADLP,
        1,
        NETP_FRM_COMP_XDMA_CMDLP,
        NETP_FRM_COMP_XDMA_DIATR,
        NETP_FRM_COMP_XDMA_BIT,
        NETP_FRM_COMP_XDMA_LEN_BIT,
    },
    //NetpFrameDataStr frameDtaStr[MAX_NETP_CP_FRAME_NUM];
    {
        {
            0x03,
            13,
            {0x00,0x64,0x00,0x04},
            4,
            1,
            3,
            {
                {NETP_D_Ty_A_N,0,0,0}
            },
            {
                {NETP_D_Ty_STR, 1, NETP_COMMON, 0},
                {NETP_D_BIG_TO_LIT_4B_LONG_REV2, 4, NETP_READ_WORKING_ACCUMULATE_TOTAL, 0},
                {NETP_D_BIG_TO_LIT_4B_LONG_REV2, 4, NETP_READ_ACCUMULATE_TOTAL, 0}, 
               
            },
        
        },
        {
            0x03,
            13,
            {0x01,0x32,0x00,0x04},
            4,
            1,
            3,
            {
                {NETP_D_Ty_A_N,0,0,0}
            },
            {
                {NETP_D_Ty_STR, 1, NETP_COMMON, 0},
                {NETP_D_BIG_TO_LIT_4B_FLOAT_REV2, 4, NETP_READ_PRESSURE, 0},
                {NETP_D_BIG_TO_LIT_4B_FLOAT_REV2, 4, NETP_READ_TEMP, 0}, 
               
            },
        
        },      
        {
            0x03,
            13,
            {0x01,0x46,0x00,0x04},
            4,
            1,
            3,
            {
                {NETP_D_Ty_A_N,0,0,0}
            },
            {
                {NETP_D_Ty_STR, 1, NETP_COMMON, 0},
                {NETP_D_BIG_TO_LIT_4B_FLOAT_REV2, 4, NETP_READ_WORKING_CUR_FLOW, 0},
                {NETP_D_BIG_TO_LIT_4B_FLOAT_REV2, 4, NETP_READ_CUR_FLOW, 0}, 
               
            },
        
        },            
    },
    //(EFT_MIN_ST_BIT|EFT_MIN_SI_BIT|EFT_MIN_TMP_BIT|EFT_MIN_P_BIT),
    {
        0x10,
        7,
        {0x00,0xd0,0,0x12},
        4,
        1,
        1,
        {
            {NETP_D_Ty_WXIC_PREPAY,18,NETP_WRITE_WXIC_PREPAY,NETP_D_B_DEV_ADDR}
        },
        {
            {NETP_D_Ty_WXIC_PREPAY_HEX, 2, NETP_WRITE_WXIC_PREPAY_RET, 0},
        },
        
    },


    {
        0x03,
        23,
        {0x00,0xd0,0,0x12},
        2,
        1,
        1,
        {
            {NETP_D_STR_DATA_TO_BUF,2,NETP_WRITE_WXIC_PREPAY,0}
        },
        {
            {NETP_D_Ty_WXIC_PREPAY_HEX, 18, NETP_WRITE_WXIC_PREPAY_RET, 0},
        },
        
    },






    {
        0x10,
        7,
        {0,0xA0,0,0x2F},
        4,
        1,
        1,
        {
            {NETP_D_STR_DATA_TO_BUF,47,NETP_WRITE_WXIC_PRICE,0}
        },
        {
            {NETP_D_Ty_WXIC_PREPAY_HEX, 2, NETP_WRITE_WXIC_PRICE_RET, 0},
        },
        
    },
        

    {
        0x03,
        52,
        {0,0xA0,0,0},
        2,
        1,
        1,
        {
            {NETP_D_STR_DATA_TO_BUF,2,NETP_WRITE_WXIC_PRICE,0}
        },
        {
            {NETP_D_Ty_WXIC_PREPAY_HEX, 47, NETP_WRITE_WXIC_PRICE_RET, 0},
        },
        
    },    
    {
        0x10,
        27,
        {0,0x80,0,0x0a},
        4,
        1,
        1,
        {
            {NETP_D_STR_DATA_TO_BUF,10,NETP_WRITE_WXIC_RECHARGE,0}
        },
        {
            {NETP_D_Ty_S_HEX, 22, NETP_WRITE_WXIC_RECHARGE_RET, 0},
        },
        
    },
    {
        0x05,
        8,
        {0,0xAC,0,0},
        2,
        1,
        1,
        {
            {NETP_D_STR_DATA_TO_BUF,2,0,0}
        },
        {
            {NETP_D_Ty_HEX, 2, NETP_COMMON, 0},
        },
        
    },
    0xA5A5,
};
#endif
#if CID_SZMA > 0//思筑modbus
    
#define NETP_FRM_XDMA_LATR     (PST_LATR_NOLEN)

#define NETP_FRM_XDMA_ATR      (PST_ATR_CSCAL_CRC3|PST_ATR_END)
#define NETP_FRM_XDMA_MLEN     4        //最小帧的长度
//	#define NETP_FRM_CSSL_SUB_2     0x82        //定义累加和计算的长度偏移
//	#define NETP_FRM_CSOS_SUB_2     0x82        //定义累加和的位置偏移
#define NETP_FRM_XDMA_DIATR    (0)//低位是DI跟帧头位置

#define NETP_FRM_COMP_XDMA_LP       0x00        //定义长度起始的位置
#define NETP_FRM_COMP_XDMA_LATR     (PST_LATR_NOLEN)
#define NETP_FRM_COMP_XDMA_ADLEN     0x01       //地址域偏移
#define NETP_FRM_COMP_XDMA_ADLP      0x00//高位1表示 地址域长度要累加 0表示不需要累加
#define NETP_FRM_COMP_XDMA_CMDLP     0x01
#define NETP_FRM_COMP_XDMA_DIATR    (PST_DIATR_DB_MB | 0x02)//低位是DI跟帧头位置

#define NETP_FRM_COMP_XDMA_BIT (uint16_t)(PST_FRM_BIT_HEAD|PST_FRM_BIT_ADDR \
                                     | PST_FRM_BIT_CMD | PST_FRM_BIT_DATA \
                                     | PST_FRM_BIT_DI | PST_FRM_BIT_CS)
#define NETP_FRM_COMP_XDMA_LEN_BIT (uint16_t)(0)


//思筑 UFG620 modbus
const ST_NETP_FW_BASE gs_netp_fw_szma = 
{
    CID_SZMA,
    {'S','Z','M','A'},
    NETP_FRM_BS_96_8_1_N,
    1,
    {
        {PST_FRM_ANY},
        1,
        NETP_FRM_LP_02,
        NETP_FRM_DLP_02,
        PST_LATR_NOLEN,
        0,
        0,
        NETP_FRM_WITHOUT_END,
        NETP_FRM_CSSP_FROM_0,
        NETP_FRM_XDMA_MLEN,
        NETP_FRM_XDMA_ATR,
        NETP_FRM_CSSL_SUB_2,
        NETP_FRM_CSOS_SUB_2,
        1,
        NETP_FRM_COMP_XDMA_CMDLP,
        NETP_FRM_XDMA_DIATR,
        NETP_FRM_COMP_XDMA_ADLEN,
        NETP_FRM_COMP_XDMA_ADLP,
    },
    {
        {PST_FRM_ANY},
        1,
        NETP_FRM_COMP_XDMA_LP,
        NETP_FRM_COMP_XDMA_LATR,
        0,
        0,
        NETP_FRM_WITHOUT_END,
        NETP_FRM_CSSP_FROM_0,
        NETP_FRM_XDMA_ATR,
        NETP_FRM_CSSL_SUB_2,    
        NETP_FRM_COMP_XDMA_ADLEN,
        NETP_FRM_COMP_XDMA_ADLP,
        1,
        NETP_FRM_COMP_XDMA_CMDLP,
        NETP_FRM_COMP_XDMA_DIATR,
        NETP_FRM_COMP_XDMA_BIT,
        NETP_FRM_COMP_XDMA_LEN_BIT,
    },
    //NetpFrameDataStr frameDtaStr[MAX_NETP_CP_FRAME_NUM];
    {
        {
            0x03,
            33,
            {0x0D,0xAD,0x00,0x0E},
            4,
            1,
            7,
            {
                {NETP_D_Ty_A_N,0,0,0}
            },
            {
                {NETP_D_Ty_STR, 1, NETP_COMMON, 0},
                {NETP_D_BIG_TO_LIT_6B_INT4B_REV2_DECI2B_INVLID1B, 6, NETP_READ_WORKING_ACCUMULATE_TOTAL, 0},
                {NETP_D_BIG_TO_LIT_6B_INT4B_REV2_DECI2B_INVLID1B, 6, NETP_READ_ACCUMULATE_TOTAL, 0}, 
                {NETP_D_BIG_TO_LIT_4B_FLOAT_REV2, 4, NETP_READ_WORKING_CUR_FLOW, 0},
                {NETP_D_BIG_TO_LIT_4B_FLOAT_REV2, 4, NETP_READ_CUR_FLOW,0},
                {NETP_D_BIG_TO_LIT_4B_FLOAT_REV2, 4, NETP_READ_TEMP,0},
                {NETP_D_BIG_TO_LIT_4B_FLOAT_REV2, 4, NETP_READ_PRESSURE,0},
                
            },
        
        },
    },
    //(EFT_MIN_ST_BIT|EFT_MIN_SI_BIT|EFT_MIN_TMP_BIT|EFT_MIN_P_BIT),
    {
        0x10,
        7,
        {0x00,0xd0,0,0x12},
        4,
        1,
        1,
        {
            {NETP_D_Ty_WXIC_PREPAY,18,NETP_WRITE_WXIC_PREPAY,NETP_D_B_DEV_ADDR}
        },
        {
            {NETP_D_Ty_WXIC_PREPAY_HEX, 2, NETP_WRITE_WXIC_PREPAY_RET, 0},
        },
        
    },


    {
        0x03,
        23,
        {0x00,0xd0,0,0x12},
        2,
        1,
        1,
        {
            {NETP_D_STR_DATA_TO_BUF,2,NETP_WRITE_WXIC_PREPAY,0}
        },
        {
            {NETP_D_Ty_WXIC_PREPAY_HEX, 18, NETP_WRITE_WXIC_PREPAY_RET, 0},
        },
        
    },






    {
        0x10,
        7,
        {0,0xA0,0,0x2F},
        4,
        1,
        1,
        {
            {NETP_D_STR_DATA_TO_BUF,47,NETP_WRITE_WXIC_PRICE,0}
        },
        {
            {NETP_D_Ty_WXIC_PREPAY_HEX, 2, NETP_WRITE_WXIC_PRICE_RET, 0},
        },
        
    },
        

    {
        0x03,
        52,
        {0,0xA0,0,0},
        2,
        1,
        1,
        {
            {NETP_D_STR_DATA_TO_BUF,2,NETP_WRITE_WXIC_PRICE,0}
        },
        {
            {NETP_D_Ty_WXIC_PREPAY_HEX, 47, NETP_WRITE_WXIC_PRICE_RET, 0},
        },
        
    },    
    {
        0x10,
        27,
        {0,0x80,0,0x0a},
        4,
        1,
        1,
        {
            {NETP_D_STR_DATA_TO_BUF,10,NETP_WRITE_WXIC_RECHARGE,0}
        },
        {
            {NETP_D_Ty_S_HEX, 22, NETP_WRITE_WXIC_RECHARGE_RET, 0},
        },
        
    },
    {
        0x05,
        8,
        {0,0xAC,0,0},
        2,
        1,
        1,
        {
            {NETP_D_STR_DATA_TO_BUF,2,0,0}
        },
        {
            {NETP_D_Ty_HEX, 2, NETP_COMMON, 0},
        },
        
    },
    0xA5A5,
};
#endif
#if CID_ZHMA > 0//中核维思MODBUS CL-2 

    
#define NETP_FRM_XDMA_LATR     (PST_LATR_NOLEN)

#define NETP_FRM_XDMA_ATR      (PST_ATR_CSCAL_CRC3|PST_ATR_END)
#define NETP_FRM_XDMA_MLEN     4        //最小帧的长度
//	#define NETP_FRM_CSSL_SUB_2     0x82        //定义累加和计算的长度偏移
//	#define NETP_FRM_CSOS_SUB_2     0x82        //定义累加和的位置偏移
#define NETP_FRM_XDMA_DIATR    (0)//低位是DI跟帧头位置

#define NETP_FRM_COMP_XDMA_LP       0x00        //定义长度起始的位置
#define NETP_FRM_COMP_XDMA_LATR     (PST_LATR_NOLEN)
#define NETP_FRM_COMP_XDMA_ADLEN     0x01       //地址域偏移
#define NETP_FRM_COMP_XDMA_ADLP      0x00//高位1表示 地址域长度要累加 0表示不需要累加
#define NETP_FRM_COMP_XDMA_CMDLP     0x01
#define NETP_FRM_COMP_XDMA_DIATR    (PST_DIATR_DB_MB | 0x02)//低位是DI跟帧头位置

#define NETP_FRM_COMP_XDMA_BIT (uint16_t)(PST_FRM_BIT_HEAD|PST_FRM_BIT_ADDR \
                                     | PST_FRM_BIT_CMD | PST_FRM_BIT_DATA \
                                     | PST_FRM_BIT_DI | PST_FRM_BIT_CS)
#define NETP_FRM_COMP_XDMA_LEN_BIT (uint16_t)(0)



//中核维思MODBUS CL-2 
const ST_NETP_FW_BASE gs_netp_fw_zhma = 
{
    CID_ZHMA,
    {'Z','H','M','A'},
    NETP_FRM_BS_96_8_1_N,
    
    1,
    {
        {PST_FRM_ANY},
        1,
        NETP_FRM_LP_02,
        NETP_FRM_DLP_02,
        PST_LATR_NOLEN,
        0,
        0,
        NETP_FRM_WITHOUT_END,
        NETP_FRM_CSSP_FROM_0,
        NETP_FRM_XDMA_MLEN,
        NETP_FRM_XDMA_ATR,
        NETP_FRM_CSSL_SUB_2,
        NETP_FRM_CSOS_SUB_2,
        1,
        NETP_FRM_COMP_XDMA_CMDLP,
        NETP_FRM_XDMA_DIATR,
        NETP_FRM_COMP_XDMA_ADLEN,
        NETP_FRM_COMP_XDMA_ADLP,
    },
    {
        {PST_FRM_ANY},
        1,
        NETP_FRM_COMP_XDMA_LP,
        NETP_FRM_COMP_XDMA_LATR,
        0,
        0,
        NETP_FRM_WITHOUT_END,
        NETP_FRM_CSSP_FROM_0,
        NETP_FRM_XDMA_ATR,
        NETP_FRM_CSSL_SUB_2,    
        NETP_FRM_COMP_XDMA_ADLEN,
        NETP_FRM_COMP_XDMA_ADLP,
        1,
        NETP_FRM_COMP_XDMA_CMDLP,
        NETP_FRM_COMP_XDMA_DIATR,
        NETP_FRM_COMP_XDMA_BIT,
        NETP_FRM_COMP_XDMA_LEN_BIT,
    },
    //NetpFrameDataStr frameDtaStr[MAX_NETP_CP_FRAME_NUM];
    {
        {
            0x03,
            69,
            {0x00,0x4C,0x00,0x20},
            4,
            1,
            9,
            {
                {NETP_D_Ty_A_N,0,0,0}
            },
            {
                {NETP_D_Ty_STR, 1, NETP_COMMON, 0},
                {NETP_D_BIG_TO_LIT_4B_FLOAT_REV2, 4, NETP_READ_WORKING_ACCUMULATE_TOTAL, 0},
                {NETP_D_BIG_TO_LIT_4B_FLOAT_REV2, 4, NETP_READ_ACCUMULATE_TOTAL, 0}, 
                {NETP_D_Ty_STR, 32, NETP_COMMON, 0},
                {NETP_D_BIG_TO_LIT_4B_FLOAT_REV2, 4, NETP_READ_PRESSURE, 0},
                {NETP_D_BIG_TO_LIT_4B_FLOAT_REV2, 4, NETP_READ_TEMP, 0}, 
                {NETP_D_Ty_STR, 8, NETP_COMMON, 0},
                {NETP_D_BIG_TO_LIT_4B_FLOAT_REV2, 4, NETP_READ_WORKING_CUR_FLOW, 0},
                {NETP_D_BIG_TO_LIT_4B_FLOAT_REV2, 4, NETP_READ_CUR_FLOW, 0}, 
                
                
            },
        
        },
        {
            0x03,
            13,
            {0x01,0x32,0x00,0x04},
            4,
            1,
            3,
            {
                {NETP_D_Ty_A_N,0,0,0}
            },
            {
                {NETP_D_Ty_STR, 1, NETP_COMMON, 0},
                {NETP_D_BIG_TO_LIT_4B_FLOAT_REV2, 4, NETP_READ_PRESSURE, 0},
                {NETP_D_BIG_TO_LIT_4B_FLOAT_REV2, 4, NETP_READ_TEMP, 0}, 
               
            },
        
        },      
        {
            0x03,
            13,
            {0x01,0x46,0x00,0x04},
            4,
            1,
            3,
            {
                {NETP_D_Ty_A_N,0,0,0}
            },
            {
                {NETP_D_Ty_STR, 1, NETP_COMMON, 0},
                {NETP_D_BIG_TO_LIT_4B_FLOAT_REV2, 4, NETP_READ_WORKING_CUR_FLOW, 0},
                {NETP_D_BIG_TO_LIT_4B_FLOAT_REV2, 4, NETP_READ_CUR_FLOW, 0}, 
               
            },
        
        },            
    },
    //(EFT_MIN_ST_BIT|EFT_MIN_SI_BIT|EFT_MIN_TMP_BIT|EFT_MIN_P_BIT),
    {
        0x10,
        7,
        {0x00,0xd0,0,0x12},
        4,
        1,
        1,
        {
            {NETP_D_Ty_WXIC_PREPAY,18,NETP_WRITE_WXIC_PREPAY,NETP_D_B_DEV_ADDR}
        },
        {
            {NETP_D_Ty_WXIC_PREPAY_HEX, 2, NETP_WRITE_WXIC_PREPAY_RET, 0},
        },
        
    },


    {
        0x03,
        23,
        {0x00,0xd0,0,0x12},
        2,
        1,
        1,
        {
            {NETP_D_STR_DATA_TO_BUF,2,NETP_WRITE_WXIC_PREPAY,0}
        },
        {
            {NETP_D_Ty_WXIC_PREPAY_HEX, 18, NETP_WRITE_WXIC_PREPAY_RET, 0},
        },
        
    },






    {
        0x10,
        7,
        {0,0xA0,0,0x2F},
        4,
        1,
        1,
        {
            {NETP_D_STR_DATA_TO_BUF,47,NETP_WRITE_WXIC_PRICE,0}
        },
        {
            {NETP_D_Ty_WXIC_PREPAY_HEX, 2, NETP_WRITE_WXIC_PRICE_RET, 0},
        },
        
    },
        

    {
        0x03,
        52,
        {0,0xA0,0,0},
        2,
        1,
        1,
        {
            {NETP_D_STR_DATA_TO_BUF,2,NETP_WRITE_WXIC_PRICE,0}
        },
        {
            {NETP_D_Ty_WXIC_PREPAY_HEX, 47, NETP_WRITE_WXIC_PRICE_RET, 0},
        },
        
    },    
    {
        0x10,
        27,
        {0,0x80,0,0x0a},
        4,
        1,
        1,
        {
            {NETP_D_STR_DATA_TO_BUF,10,NETP_WRITE_WXIC_RECHARGE,0}
        },
        {
            {NETP_D_Ty_S_HEX, 22, NETP_WRITE_WXIC_RECHARGE_RET, 0},
        },
        
    },
    {
        0x05,
        8,
        {0,0xAC,0,0},
        2,
        1,
        1,
        {
            {NETP_D_STR_DATA_TO_BUF,2,0,0}
        },
        {
            {NETP_D_Ty_HEX, 2, NETP_COMMON, 0},
        },
        
    },
    0xA5A5,
};
#endif
#if CID_XKVZD > 0

    
#define NETP_FRM_XKZD_LATR     (PST_LATR_NOLEN)

#define NETP_FRM_XKMA_END      0x5E        //定义结尾符
#define NETP_FRM_CSSP_FROM_1     0x01        //定义累加和计算起始位置
#define NETP_FRM_XKZD_ATR      (PST_ATR_PARIT)
#define NETP_FRM_XKZD_MLEN     7        //最小帧的长度

#define NETP_FRM_XKZD_DIATR    (0)//低位是DI跟帧头位置
#define NETP_FRM_COMP_XKZD_ATR      (PST_ATR_PARIT|PST_ATR_END)

#define NETP_FRM_COMP_XKZD_LP       0x00        //定义长度起始的位置
#define NETP_FRM_COMP_XKZD_LATR     (PST_LATR_NOLEN)
#define NETP_FRM_COMP_XKZD_ADLEN     0x02       //地址域长度
#define NETP_FRM_COMP_XKZD_ADLP      (PST_ADLP_ADDLEN|PST_ADLP_ADD_TO_ASCII|0x01)//高位1表示 地址域长度要累加 0表示不需要累加
#define NETP_FRM_COMP_XKZD_CMDLP     0x03
#define NETP_FRM_COMP_XKZD_DIATR    (PST_DIATR_DB_MB | 0x02)//低位是DI跟帧头位置

#define NETP_FRM_COMP_XKZD_BIT (uint16_t)(PST_FRM_BIT_HEAD|PST_FRM_BIT_ADDR)
#define NETP_FRM_COMP_XKZD_LEN_BIT (uint16_t)(0)




const ST_NETP_FW_BASE gs_netp_fw_xkzd = 
{
    CID_XKVZD,
    {'X','K','Z','D'},
    NETP_FRM_BS_48_8_1_N,
    1,
    {
        {0x5B},
        1,
        NETP_FRM_LP_02,
        NETP_FRM_DLP_03,
        PST_LATR_NOLEN,
        0,
        0,
        NETP_FRM_XKMA_END,
        NETP_FRM_CSSP_FROM_1,
        NETP_FRM_XKZD_MLEN,
        NETP_FRM_XKZD_ATR,
        NETP_FRM_CSSL_SUB_5,
        NETP_FRM_CSOS_SUB_3,
        0,
        NETP_FRM_COMP_XKZD_CMDLP,
        NETP_FRM_XKZD_DIATR,
        NETP_FRM_COMP_XKZD_ADLEN,
        NETP_FRM_COMP_XKZD_ADLP,
    },
    {
        {0x21},
        1,
        NETP_FRM_COMP_XKZD_LP,
        NETP_FRM_COMP_XKZD_LATR,
        0,
        0,
        NETP_FRM_WITHOUT_END,
        NETP_FRM_CSSP_FROM_0,
        NETP_FRM_COMP_XKZD_ATR,
        NETP_FRM_CSSL_SUB_2,    
        NETP_FRM_COMP_XKZD_ADLEN,
        NETP_FRM_COMP_XKZD_ADLP,
        1,
        NETP_FRM_COMP_XKZD_CMDLP,
        NETP_FRM_COMP_XKZD_DIATR,
        NETP_FRM_COMP_XKZD_BIT,
        NETP_FRM_COMP_XKZD_LEN_BIT,
    },
    //NetpFrameDataStr frameDtaStr[MAX_NETP_CP_FRAME_NUM];
    {
        {
            0,
            49,
            {0x00,0x00,0x00,0x0D},
            0,
            1,
            7,
            {
                {NETP_D_Ty_A_N,0,0,0}
            },
            {
                {NETP_D_Ty_STR, 1, NETP_COMMON, 0},
                {NETP_D_ASCII10B_TO_HEX5B,        10, NETP_READ_ACCUMULATE_TOTAL, 0},
                {NETP_D_ASCII8B_TO_HEX4B_DECI2,   8, NETP_READ_CUR_FLOW, 0}, 
                {NETP_D_ASCII6B_TO_HEX3B_DECI1,   6, NETP_READ_PRESSURE, 0},
                {NETP_D_ASCII6B_TO_HEX3B,         6, NETP_READ_WORKING_ACCUMULATE_TOTAL,0},
                {NETP_D_ASCII6B_TO_HEX3B_DECI2,   6, NETP_READ_WORKING_CUR_FLOW,0},
                {NETP_D_ASCII5B_TO_HEX2B_DECI1_SYMBOL, 5, NETP_READ_TEMP,0},
                
            },
        
        },
    },
    //(EFT_MIN_ST_BIT|EFT_MIN_SI_BIT|EFT_MIN_TMP_BIT|EFT_MIN_P_BIT),
    {
        0x10,
        7,
        {0x00,0xd0,0,0x12},
        4,
        1,
        1,
        {
            {NETP_D_Ty_WXIC_PREPAY,18,NETP_WRITE_WXIC_PREPAY,NETP_D_B_DEV_ADDR}
        },
        {
            {NETP_D_Ty_WXIC_PREPAY_HEX, 2, NETP_WRITE_WXIC_PREPAY_RET, 0},
        },
        
    },


    {
        0x03,
        23,
        {0x00,0xd0,0,0x12},
        2,
        1,
        1,
        {
            {NETP_D_STR_DATA_TO_BUF,2,NETP_WRITE_WXIC_PREPAY,0}
        },
        {
            {NETP_D_Ty_WXIC_PREPAY_HEX, 18, NETP_WRITE_WXIC_PREPAY_RET, 0},
        },
        
    },






    {
        0x10,
        7,
        {0,0xA0,0,0x2F},
        4,
        1,
        1,
        {
            {NETP_D_STR_DATA_TO_BUF,47,NETP_WRITE_WXIC_PRICE,0}
        },
        {
            {NETP_D_Ty_WXIC_PREPAY_HEX, 2, NETP_WRITE_WXIC_PRICE_RET, 0},
        },
        
    },
        

    {
        0x03,
        52,
        {0,0xA0,0,0},
        2,
        1,
        1,
        {
            {NETP_D_STR_DATA_TO_BUF,2,NETP_WRITE_WXIC_PRICE,0}
        },
        {
            {NETP_D_Ty_WXIC_PREPAY_HEX, 47, NETP_WRITE_WXIC_PRICE_RET, 0},
        },
        
    },    
    {
        0x10,
        27,
        {0,0x80,0,0x0a},
        4,
        1,
        1,
        {
            {NETP_D_STR_DATA_TO_BUF,10,NETP_WRITE_WXIC_RECHARGE,0}
        },
        {
            {NETP_D_Ty_S_HEX, 22, NETP_WRITE_WXIC_RECHARGE_RET, 0},
        },
        
    },
    {
        0x05,
        8,
        {0,0xAC,0,0},
        2,
        1,
        1,
        {
            {NETP_D_STR_DATA_TO_BUF,2,0,0}
        },
        {
            {NETP_D_Ty_HEX, 2, NETP_COMMON, 0},
        },
        
    },
    0xA5A5,
};
#endif

#if CID_SDAS > 0// 斯达特AS通信协议(AS-25\32\40\50\80)(实为标准Modbus RTU协议)      

const ST_NETP_FW_BASE gs_netp_fw_sdas = 
{
    CID_SDAS,
    {'S','D','A','S'},
    NETP_FRM_BS_48_8_1_N,
    
    1,
    {
        {PST_FRM_ANY},
        1,
        NETP_FRM_LP_02,
        NETP_FRM_DLP_02,
        PST_LATR_NOLEN,
        0,
        0,
        NETP_FRM_WITHOUT_END,
        NETP_FRM_CSSP_FROM_0,
        NETP_FRM_MIN_LEN_4,
        NETP_FRM_ATR_MODBUSCRC_NO_END,
        NETP_FRM_CSSL_SUB_2,
        NETP_FRM_CSOS_SUB_2,
        1,
        NETP_FRM_CMDLP_1,
        NETP_FRM_DECOMP_DIATR_NULL,
        NETP_FRM_ADLEN_1,
        NETP_FRM_ADLP_0,
    },
    {
        {PST_FRM_ANY},
        1,
        NETP_FRM_COMP_LP_0,
        NETP_FRM_COMP_LATR_NOLEN,
        0,
        0,
        NETP_FRM_WITHOUT_END,
        NETP_FRM_CSSP_FROM_0,
        NETP_FRM_ATR_MODBUSCRC_NO_END,
        NETP_FRM_CSSL_SUB_2,    
        NETP_FRM_ADLEN_1,
        NETP_FRM_ADLP_0,
        1,
        NETP_FRM_CMDLP_1,
        NETP_FRM_COMP_DIATR_HAS_02,
        NETP_FRM_COMP_BIT_H_A_C_D_I_C,
        NETP_FRM_COMP_LEN_BIT_NULL,
    },
    //NetpFrameDataStr frameDtaStr[MAX_NETP_CP_FRAME_NUM];
    {
        {
            0x03,
            35,
            {0x01,0xFF,0x00,0x0F},
            4,
            1,
            7,
            {
                {NETP_D_Ty_A_N,0,0,0}
            },
            {
                {NETP_D_Ty_STR, 1, NETP_COMMON, 0},
                {NETP_D_BIG_TO_LIT_4B_DECI2, 4, NETP_READ_WORKING_CUR_FLOW,0},
                {NETP_D_BIG_TO_LIT_4B_DECI2, 4, NETP_READ_CUR_FLOW, 0},
                {NETP_D_BIG_TO_LIT_4B_DECI2, 4, NETP_READ_PRESSURE,0},
                {NETP_D_BIG_TO_LIT_4B_DECI2, 4, NETP_READ_TEMP,0},
                {NETP_D_BIG_TO_LIT_4B_DECI1, 4, NETP_READ_WORKING_ACCUMULATE_TOTAL, 0}, 
                {NETP_D_BIG_TO_LIT_4B_DECI1, 4, NETP_READ_ACCUMULATE_TOTAL, 0},
                
            },
        
        },
    },
    
    {
        0,
    },

    {
        0,
    },


    {
        0,
    },

    {
        0,
    }, 
    {
        0,
    },

    {
        0,
    },
    0xA5A5,
};
#endif

#if CID_SDM2 > 0// 斯达特AS通信协议(AS-25\32\40\50\80)(实为标准Modbus RTU协议)      

const ST_NETP_FW_BASE gs_netp_fw_sdm2 = 
{
    CID_SDM2,
    {'S','D','M','2'},
    NETP_FRM_BS_48_8_1_N,
    
    1,
    {
        {PST_FRM_ANY},
        1,
        NETP_FRM_LP_02,
        NETP_FRM_DLP_02,
        PST_LATR_NOLEN,
        0,
        0,
        NETP_FRM_WITHOUT_END,
        NETP_FRM_CSSP_FROM_0,
        NETP_FRM_MIN_LEN_4,
        NETP_FRM_ATR_MODBUSCRC_NO_END,
        NETP_FRM_CSSL_SUB_2,
        NETP_FRM_CSOS_SUB_2,
        1,
        NETP_FRM_CMDLP_1,
        NETP_FRM_DECOMP_DIATR_NULL,
        NETP_FRM_ADLEN_1,
        NETP_FRM_ADLP_0,
    },
    {
        {PST_FRM_ANY},
        1,
        NETP_FRM_COMP_LP_0,
        NETP_FRM_COMP_LATR_NOLEN,
        0,
        0,
        NETP_FRM_WITHOUT_END,
        NETP_FRM_CSSP_FROM_0,
        NETP_FRM_ATR_MODBUSCRC_NO_END,
        NETP_FRM_CSSL_SUB_2,    
        NETP_FRM_ADLEN_1,
        NETP_FRM_ADLP_0,
        1,
        NETP_FRM_CMDLP_1,
        NETP_FRM_COMP_DIATR_HAS_02,
        NETP_FRM_COMP_BIT_H_A_C_D_I_C,
        NETP_FRM_COMP_LEN_BIT_NULL,
    },
    //NetpFrameDataStr frameDtaStr[MAX_NETP_CP_FRAME_NUM];
    {
        {
            0x03,
            35,
            {0x01,0xFF,0x00,0x0F},
            4,
            1,
            7,
            {
                {NETP_D_Ty_A_N,0,0,0}
            },
            {
                {NETP_D_Ty_STR, 1, NETP_COMMON, 0},
                {NETP_D_BIG_TO_LIT_4B_DECI2, 4, NETP_READ_WORKING_CUR_FLOW,0},
                {NETP_D_BIG_TO_LIT_4B_DECI2, 4, NETP_READ_CUR_FLOW, 0},
                {NETP_D_BIG_TO_LIT_4B_DECI2, 4, NETP_READ_PRESSURE,0},
                {NETP_D_BIG_TO_LIT_4B_DECI2, 4, NETP_READ_TEMP,0},
                {NETP_D_BIG_TO_LIT_4B_LONG4B, 4, NETP_READ_WORKING_ACCUMULATE_TOTAL, 0}, 
                {NETP_D_BIG_TO_LIT_4B_LONG4B, 4, NETP_READ_ACCUMULATE_TOTAL, 0},
                
            },
        
        },
    },
    
    {
        0,
    },

    {
        0,
    },


    {
        0,
    },

    {
        0,
    }, 
    {
        0,
    },

    {
        0,
    },
    0xA5A5,
};
#endif
#if CID_ITRON > 0//埃创MODBUS  

const ST_NETP_FW_BASE gs_netp_fw_itma = 
{
    CID_ITRON,
    {'I','T','M','A'},
    NETP_FRM_BS_96_8_1_N,
    
    3,
    {
        {PST_FRM_ANY},
        1,
        NETP_FRM_LP_02,
        NETP_FRM_DLP_02,
        PST_LATR_NOLEN,
        0,
        0,
        NETP_FRM_WITHOUT_END,
        NETP_FRM_CSSP_FROM_0,
        NETP_FRM_MIN_LEN_4,
        NETP_FRM_ATR_MODBUSCRC_NO_END,
        NETP_FRM_CSSL_SUB_2,
        NETP_FRM_CSOS_SUB_2,
        1,
        NETP_FRM_CMDLP_1,
        NETP_FRM_DECOMP_DIATR_NULL,
        NETP_FRM_ADLEN_1,
        NETP_FRM_ADLP_0,
    },
    {
        {PST_FRM_ANY},
        1,
        NETP_FRM_COMP_LP_0,
        NETP_FRM_COMP_LATR_NOLEN,
        0,
        0,
        NETP_FRM_WITHOUT_END,
        NETP_FRM_CSSP_FROM_0,
        NETP_FRM_ATR_MODBUSCRC_NO_END,
        NETP_FRM_CSSL_SUB_2,    
        NETP_FRM_ADLEN_1,
        NETP_FRM_ADLP_0,
        1,
        NETP_FRM_CMDLP_1,
        NETP_FRM_COMP_DIATR_HAS_02,
        NETP_FRM_COMP_BIT_H_A_C_D_I_C,
        NETP_FRM_COMP_LEN_BIT_NULL,
    },
    //NetpFrameDataStr frameDtaStr[MAX_NETP_CP_FRAME_NUM];
    {
        {
            0x03,
            21,
            {0x03,0x26,0x00,0x08},
            4,
            1,
            3,
            {
                {NETP_D_Ty_A_N,0,0,0}
            },
            {
                {NETP_D_Ty_STR, 1, NETP_COMMON, 0},
                {NETP_D_BIG_TO_LIT_8B_LONG4B_DECI4B, 8, NETP_READ_WORKING_ACCUMULATE_TOTAL, 0}, 
                {NETP_D_BIG_TO_LIT_8B_LONG4B_DECI4B, 8, NETP_READ_ACCUMULATE_TOTAL, 0},
                
            },
        
        },
        {
            0x03,
            11,
            {0x03,0x36,0x00,0x04},
            4,
            1,
            3,
            {
                {NETP_D_Ty_A_N,0,0,0}
            },
            {
                {NETP_D_Ty_STR, 1, NETP_COMMON, 0},
                {NETP_D_BIG_TO_LIT_4B_FLOAT_REV2, 4, NETP_READ_WORKING_CUR_FLOW,0},
                {NETP_D_BIG_TO_LIT_4B_FLOAT_REV2, 4, NETP_READ_CUR_FLOW, 0},
                
            },
        
        },    
        {
            0x03,
            11,
            {0x03,0x42,0x00,0x04},
            4,
            1,
            3,
            {
                {NETP_D_Ty_A_N,0,0,0}
            },
            {
                {NETP_D_Ty_STR, 1, NETP_COMMON, 0},
                {NETP_D_BIG_TO_LIT_4B_FLOAT_REV2, 4, NETP_READ_TEMP,0},
                {NETP_D_BIG_TO_LIT_4B_FLOAT_REV2, 4, NETP_READ_PRESSURE,0},
                
            },
        
        },          
    },
    
    {
        0,
    },

    {
        0,
    },


    {
        0,
    },

    {
        0,
    }, 
    {
        0,
    },

    {
        0,
    },
    0xA5A5,
};
#endif
#if CID_DWVPM3X > 0// 德闻MODBUS PTZ-BOX3.0 新协议（long型）   

const ST_NETP_FW_BASE gs_netp_fw_dwdx = 
{
    CID_DWVPM3X,
    {'D','W','D','X'},
    NETP_FRM_BS_96_8_1_N,
    
    1,
    {
        {PST_FRM_ANY},
        1,
        NETP_FRM_LP_02,
        NETP_FRM_DLP_02,
        PST_LATR_NOLEN,
        0,
        0,
        NETP_FRM_WITHOUT_END,
        NETP_FRM_CSSP_FROM_0,
        NETP_FRM_MIN_LEN_4,
        NETP_FRM_ATR_MODBUSCRC_NO_END,
        NETP_FRM_CSSL_SUB_2,
        NETP_FRM_CSOS_SUB_2,
        1,
        NETP_FRM_CMDLP_1,
        NETP_FRM_DECOMP_DIATR_NULL,
        NETP_FRM_ADLEN_1,
        NETP_FRM_ADLP_0,
    },
    {
        {PST_FRM_ANY},
        1,
        NETP_FRM_COMP_LP_0,
        NETP_FRM_COMP_LATR_NOLEN,
        0,
        0,
        NETP_FRM_WITHOUT_END,
        NETP_FRM_CSSP_FROM_0,
        NETP_FRM_ATR_MODBUSCRC_NO_END,
        NETP_FRM_CSSL_SUB_2,    
        NETP_FRM_ADLEN_1,
        NETP_FRM_ADLP_0,
        1,
        NETP_FRM_CMDLP_1,
        NETP_FRM_COMP_DIATR_HAS_02,
        NETP_FRM_COMP_BIT_H_A_C_D_I_C,
        NETP_FRM_COMP_LEN_BIT_NULL,
    },
    //NetpFrameDataStr frameDtaStr[MAX_NETP_CP_FRAME_NUM];
    {
        {
            0x03,
            33,
            {0x00,0x03,0x00,0x0E},
            4,
            1,
            7,
            {
                {NETP_D_Ty_A_N,0,0,0}
            },
            {
                {NETP_D_Ty_STR, 1, NETP_COMMON, 0},
                {NETP_D_BIG_TO_LIT_8B_DOUBLE, 8, NETP_READ_ACCUMULATE_TOTAL, 0},
                {NETP_D_BIG_TO_LIT_4B_DECI2, 4, NETP_READ_WORKING_ACCUMULATE_TOTAL, 0}, 
                {NETP_D_BIG_TO_LIT_4B_FLOAT, 4, NETP_READ_PRESSURE,0},
                {NETP_D_BIG_TO_LIT_4B_FLOAT, 4, NETP_READ_TEMP,0},
                {NETP_D_BIG_TO_LIT_4B_FLOAT, 4, NETP_READ_WORKING_CUR_FLOW,0},
                {NETP_D_BIG_TO_LIT_4B_FLOAT, 4, NETP_READ_CUR_FLOW, 0},
                
            },
        
        },
    },
    
    {
        0,
    },

    {
        0,
    },


    {
        0,
    },

    {
        0,
    }, 
    {
        0,
    },

    {
        0,
    },
    0xA5A5,
};
#endif
#if CID_SSMA > 0//胜赛斯Modbus
const ST_NETP_FW_BASE gs_netp_fw_ssma = 
{
    CID_SSMA,
    {'S','S','M','A'},
    NETP_FRM_BS_96_8_1_N,
    1,
    {
        {PST_FRM_ANY},
        1,
        NETP_FRM_LP_02,
        NETP_FRM_DLP_02,
        PST_LATR_NOLEN,
        0,
        0,
        NETP_FRM_WITHOUT_END,
        NETP_FRM_CSSP_FROM_0,
        NETP_FRM_MIN_LEN_4,
        NETP_FRM_ATR_MODBUSCRC_NO_END,
        NETP_FRM_CSSL_SUB_2,
        NETP_FRM_CSOS_SUB_2,
        1,
        NETP_FRM_CMDLP_1,
        NETP_FRM_DECOMP_DIATR_NULL,
        NETP_FRM_ADLEN_1,
        NETP_FRM_ADLP_0,
    },
    {
        {PST_FRM_ANY},
        1,
        NETP_FRM_COMP_LP_0,
        NETP_FRM_COMP_LATR_NOLEN,
        0,
        0,
        NETP_FRM_WITHOUT_END,
        NETP_FRM_CSSP_FROM_0,
        NETP_FRM_ATR_MODBUSCRC_NO_END,
        NETP_FRM_CSSL_SUB_2,    
        NETP_FRM_ADLEN_1,
        NETP_FRM_ADLP_0,
        1,
        NETP_FRM_CMDLP_1,
        NETP_FRM_COMP_DIATR_HAS_02,
        NETP_FRM_COMP_BIT_H_A_C_D_I_C,
        NETP_FRM_COMP_LEN_BIT_NULL,
    },
    {
        {
            0x03,
            31,
            {0x00,0x00,0x00,0x0D},
            4,
            1,
            7,
            {
                {NETP_D_Ty_A_N,0,0,0}
            },
            {
                {NETP_D_Ty_STR, 1, NETP_COMMON, 0},
                {NETP_D_BIGEND_TO_LITTLE_6B, 6, NETP_READ_ACCUMULATE_TOTAL, 0},
                {NETP_D_BIGEND_TO_LITTLE_6B, 6, NETP_READ_WORKING_ACCUMULATE_TOTAL, 0}, 
                {NETP_D_BIG_TO_LIT_4B_DECI2, 4, NETP_READ_CUR_FLOW, 0},
                {NETP_D_BIG_TO_LIT_4B_DECI2, 4, NETP_READ_WORKING_CUR_FLOW,0},
                {NETP_D_BIG_TO_LIT_2B_DECI1, 2, NETP_READ_TEMP,0},
                {NETP_D_BIG_TO_LIT_4B_DECI1, 4, NETP_READ_PRESSURE,0},
                
            },
        
        },
    },
    
    {
        0x10,
    },
    {
        0x03,
    },
    {
        0x10,
    },
    {
        0x03,
    },    
    {
        0x10,
    },
    {
        0x05,
    },
    0xA5A5,
};
#endif
#if CID_HDMA > 0//哈德斯Modbus
const ST_NETP_FW_BASE gs_netp_fw_hdma = 
{
    CID_HDMA,
    {'H','D','M','A'},
    NETP_FRM_BS_96_8_1_N,
    1,
    {
        {PST_FRM_ANY},
        1,
        NETP_FRM_LP_02,
        NETP_FRM_DLP_02,
        PST_LATR_NOLEN,
        0,
        0,
        NETP_FRM_WITHOUT_END,
        NETP_FRM_CSSP_FROM_0,
        NETP_FRM_MIN_LEN_4,
        NETP_FRM_ATR_MODBUSCRC_NO_END,
        NETP_FRM_CSSL_SUB_2,
        NETP_FRM_CSOS_SUB_2,
        1,
        NETP_FRM_CMDLP_1,
        NETP_FRM_DECOMP_DIATR_NULL,
        NETP_FRM_ADLEN_1,
        NETP_FRM_ADLP_0,
    },
    {
        {PST_FRM_ANY},
        1,
        NETP_FRM_COMP_LP_0,
        NETP_FRM_COMP_LATR_NOLEN,
        0,
        0,
        NETP_FRM_WITHOUT_END,
        NETP_FRM_CSSP_FROM_0,
        NETP_FRM_ATR_MODBUSCRC_NO_END,
        NETP_FRM_CSSL_SUB_2,    
        NETP_FRM_ADLEN_1,
        NETP_FRM_ADLP_0,
        1,
        NETP_FRM_CMDLP_1,
        NETP_FRM_COMP_DIATR_HAS_02,
        NETP_FRM_COMP_BIT_H_A_C_D_I_C,
        NETP_FRM_COMP_LEN_BIT_NULL,
    },
    {
        {
            0x03,
            39,
            {0x00,0x01,0x00,0x11},
            4,
            1,
            7,
            {
                {NETP_D_Ty_A_N,0,0,0}
            },
            {
                {NETP_D_Ty_STR, 1, NETP_COMMON, 0},
                {NETP_D_BIG_TO_LIT_4B_LONG4B, 4, NETP_READ_WORKING_ACCUMULATE_TOTAL, 0}, 
                {NETP_D_BIG_TO_LIT_8B_FLOAT2_HIGH5_LOW3_DECI, 8, NETP_READ_ACCUMULATE_TOTAL, 0},
                {NETP_D_BIG_TO_LIT_4B_FLOAT, 4, NETP_READ_CUR_FLOW, 0},
                {NETP_D_BIG_TO_LIT_4B_FLOAT, 4, NETP_READ_WORKING_CUR_FLOW,0},
                {NETP_D_BIG_TO_LIT_4B_FLOAT, 4, NETP_READ_TEMP,0},
                {NETP_D_BIG_TO_LIT_4B_FLOAT, 4, NETP_READ_PRESSURE,0},
            },
        
        },
    },
    
    {0x00,},
    {0x00,},
    {0x00,},
    {0x00,},    
    {0x00,},
    {0x00,},
    0xA5A5,
};
#endif
#if CID_RTMA > 0//罗美特Modbus(Romet_Modbus)
const ST_NETP_FW_BASE gs_netp_fw_rtma = 
{
    CID_RTMA,
    {'R','T','M','A'},
    NETP_FRM_BS_96_8_1_N,
    1,
    {
        {PST_FRM_ANY},
        1,
        NETP_FRM_LP_02,
        NETP_FRM_DLP_02,
        PST_LATR_NOLEN,
        0,
        0,
        NETP_FRM_WITHOUT_END,
        NETP_FRM_CSSP_FROM_0,
        NETP_FRM_MIN_LEN_4,
        NETP_FRM_ATR_MODBUSCRC_NO_END,
        NETP_FRM_CSSL_SUB_2,
        NETP_FRM_CSOS_SUB_2,
        1,
        NETP_FRM_CMDLP_1,
        NETP_FRM_DECOMP_DIATR_NULL,
        NETP_FRM_ADLEN_1,
        NETP_FRM_ADLP_0,
    },
    {
        {PST_FRM_ANY},
        1,
        NETP_FRM_COMP_LP_0,
        NETP_FRM_COMP_LATR_NOLEN,
        0,
        0,
        NETP_FRM_WITHOUT_END,
        NETP_FRM_CSSP_FROM_0,
        NETP_FRM_ATR_MODBUSCRC_NO_END,
        NETP_FRM_CSSL_SUB_2,    
        NETP_FRM_ADLEN_1,
        NETP_FRM_ADLP_0,
        1,
        NETP_FRM_CMDLP_1,
        NETP_FRM_COMP_DIATR_HAS_02,
        NETP_FRM_COMP_BIT_H_A_C_D_I_C,
        NETP_FRM_COMP_LEN_BIT_NULL,
    },
    {
        {
            0x03,
            41,
            {0x0F,0xE4,0x00,0x12},
            4,
            1,
            9,
            {
                {NETP_D_Ty_A_N,0,0,0}
            },
            {
                {NETP_D_Ty_STR, 1, NETP_COMMON, 0},
                {NETP_D_BIG_TO_LIT_4B_FLOAT, 4, NETP_READ_CUR_FLOW, 0},
                {NETP_D_BIG_TO_LIT_4B_FLOAT, 4, NETP_READ_WORKING_CUR_FLOW,0},
                {NETP_D_BIG_TO_LIT_4B_FLOAT, 4, NETP_READ_TEMP,0},
                {NETP_D_BIG_TO_LIT_4B_FLOAT, 4, NETP_READ_PRESSURE,0},
                {NETP_D_Ty_STR, 2, NETP_COMMON, 0},
                
                {NETP_D_MSB_BCD6B_DECI3, 6, NETP_READ_ACCUMULATE_TOTAL, 0}, 
                {NETP_D_Ty_STR, 6, NETP_COMMON, 0},
                {NETP_D_MSB_BCD6B_DECI3, 6, NETP_READ_WORKING_ACCUMULATE_TOTAL, 0},
                
            },
        
        },
    },
    
    {0x00,},
    {0x00,},
    {0x00,},
    {0x00,},    
    {0x00,},
    {0x00,},
    0xA5A5,
};
#endif
#if CID_XKVM1 > 0//新科Modbus1
const ST_NETP_FW_BASE gs_netp_fw_xkm1 = 
{
    CID_XKVM1,
    {'X','K','M','1'},
    NETP_FRM_BS_96_8_1_N,
    1,
    {
        {PST_FRM_ANY},
        1,
        NETP_FRM_LP_02,
        NETP_FRM_DLP_02,
        PST_LATR_NOLEN,
        0,
        0,
        NETP_FRM_WITHOUT_END,
        NETP_FRM_CSSP_FROM_0,
        NETP_FRM_MIN_LEN_4,
        NETP_FRM_ATR_MODBUSCRC_NO_END,
        NETP_FRM_CSSL_SUB_2,
        NETP_FRM_CSOS_SUB_2,
        1,
        NETP_FRM_CMDLP_1,
        NETP_FRM_DECOMP_DIATR_NULL,
        NETP_FRM_ADLEN_1,
        NETP_FRM_ADLP_0,
    },
    {
        {PST_FRM_ANY},
        1,
        NETP_FRM_COMP_LP_0,
        NETP_FRM_COMP_LATR_NOLEN,
        0,
        0,
        NETP_FRM_WITHOUT_END,
        NETP_FRM_CSSP_FROM_0,
        NETP_FRM_ATR_MODBUSCRC_NO_END,
        NETP_FRM_CSSL_SUB_2,    
        NETP_FRM_ADLEN_1,
        NETP_FRM_ADLP_0,
        1,
        NETP_FRM_CMDLP_1,
        NETP_FRM_COMP_DIATR_HAS_02,
        NETP_FRM_COMP_BIT_H_A_C_D_I_C,
        NETP_FRM_COMP_LEN_BIT_NULL,
    },
    {
        {
            0x03,
            33,
            {0x00,0x00,0x00,0x0E},
            4,
            1,
            7,
            {
                {NETP_D_Ty_A_N,0,0,0}
            },
            {
                {NETP_D_Ty_STR, 1, NETP_COMMON, 0},
                {NETP_D_LSB_8B_FLOAT4B_H7_FLOAT4B_LOW, 8, NETP_READ_ACCUMULATE_TOTAL, 0}, 
                {NETP_D_LSB_4B_FLOAT, 4, NETP_READ_CUR_FLOW, 0},
                {NETP_D_LSB_4B_FLOAT, 4, NETP_READ_PRESSURE,0},
                {NETP_D_LSB_4B_FLOAT, 4, NETP_READ_TEMP,0},
                {NETP_D_LSB_4B_FLOAT, 4, NETP_READ_WORKING_ACCUMULATE_TOTAL, 0},
                {NETP_D_LSB_4B_FLOAT, 4, NETP_READ_WORKING_CUR_FLOW,0},
            },
        
        },
    },
    
    {0x00,},
    {0x00,},
    {0x00,},
    {0x00,},    
    {0x00,},
    {0x00,},
    0xA5A5,
};
#endif
#if CID_XKVM2 > 0//新科Modbus2
const ST_NETP_FW_BASE gs_netp_fw_xkm2 = 
{
    CID_XKVM2,
    {'X','K','M','2'},
    NETP_FRM_BS_96_8_1_N,
    1,
    {
        {PST_FRM_ANY},
        1,
        NETP_FRM_LP_02,
        NETP_FRM_DLP_02,
        PST_LATR_NOLEN,
        0,
        0,
        NETP_FRM_WITHOUT_END,
        NETP_FRM_CSSP_FROM_0,
        NETP_FRM_MIN_LEN_4,
        NETP_FRM_ATR_MODBUSCRC_NO_END,
        NETP_FRM_CSSL_SUB_2,
        NETP_FRM_CSOS_SUB_2,
        1,
        NETP_FRM_CMDLP_1,
        NETP_FRM_DECOMP_DIATR_NULL,
        NETP_FRM_ADLEN_1,
        NETP_FRM_ADLP_0,
    },
    {
        {PST_FRM_ANY},
        1,
        NETP_FRM_COMP_LP_0,
        NETP_FRM_COMP_LATR_NOLEN,
        0,
        0,
        NETP_FRM_WITHOUT_END,
        NETP_FRM_CSSP_FROM_0,
        NETP_FRM_ATR_MODBUSCRC_NO_END,
        NETP_FRM_CSSL_SUB_2,    
        NETP_FRM_ADLEN_1,
        NETP_FRM_ADLP_0,
        1,
        NETP_FRM_CMDLP_1,
        NETP_FRM_COMP_DIATR_HAS_02,
        NETP_FRM_COMP_BIT_H_A_C_D_I_C,
        NETP_FRM_COMP_LEN_BIT_NULL,
    },
    {
        {
            0x03,
            33,
            {0x00,0x00,0x00,0x0E},
            4,
            1,
            7,
            {
                {NETP_D_Ty_A_N,0,0,0}
            },
            {
                {NETP_D_Ty_STR, 1, NETP_COMMON, 0},
                {NETP_D_MSB_8B_LONG4B_H7_LONG4B_LOW, 8, NETP_READ_ACCUMULATE_TOTAL, 0}, 
                {NETP_D_BIG_TO_LIT_4B_DECI1, 4, NETP_READ_CUR_FLOW, 0},
                {NETP_D_BIG_TO_LIT_4B_DECI1, 4, NETP_READ_PRESSURE,0},
                {NETP_D_BIG_TO_LIT_4B_DECI1, 4, NETP_READ_TEMP,0},
                {NETP_D_BIG_TO_LIT_4B_DECI1, 4, NETP_READ_WORKING_ACCUMULATE_TOTAL, 0},
                {NETP_D_BIG_TO_LIT_4B_DECI1, 4, NETP_READ_WORKING_CUR_FLOW,0},
            },
        
        },
    },
    
    {0x00,},
    {0x00,},
    {0x00,},
    {0x00,},    
    {0x00,},
    {0x00,},
    0xA5A5,
};
#endif

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


};

#endif                                 



/******************************************************************************
**定义帧结构常量
******************************************************************************/
const ST_NETP_FW_BASE* const gss_NetpFrameFw[] = 
{
#if (CID_TXV13 > 0)
     &gs_netp_fw_tx13,
#endif

#if (CID_WXIC > 0)
    &gs_netp_fw_wxic,
#endif

#if (CID_XDMA > 0)
    &gs_netp_fw_xdma,
#endif
    
#if (CID_SYMA > 0)
    &gs_netp_fw_syma,
#endif
	
#if (CID_CNV12 > 0)
    &gs_netp_fw_cn12,
#endif
#if (CID_ETM1 > 0)
    &gs_netp_fw_etek,
#endif
	
#if (CID_SZMA > 0)
    &gs_netp_fw_szma,
#endif
#if (CID_ZHMA > 0)
    &gs_netp_fw_zhma,
#endif  

#if (CID_XKVZD > 0)
    &gs_netp_fw_xkzd,
#endif  
#if (CID_SDAS > 0)
    &gs_netp_fw_sdas,
#endif 
#if (CID_SDM2 > 0)
    &gs_netp_fw_sdm2,
#endif 
    
#if (CID_ITRON > 0)
    &gs_netp_fw_itma,
#endif 
#if (CID_DWVPM3X > 0)
    &gs_netp_fw_dwdx,
#endif 
#if (CID_SSMA > 0)
    &gs_netp_fw_ssma,
#endif 

#if (CID_HDMA > 0)
    &gs_netp_fw_hdma,
#endif 
    
#if (CID_RTMA > 0)
    &gs_netp_fw_rtma,
#endif 
    
#if (CID_XKVM1 > 0)
    &gs_netp_fw_xkm1,
#endif 
    
#if (CID_XKVM2 > 0)
    &gs_netp_fw_xkm2,
#endif 

};


const uint16 gui_NetpFwTblSize = sizeof(gss_NetpFrameFw) / sizeof(ST_NETP_FW_BASE *);


const PSTFrameStr* const gss_PSTFrameStr[] = 
{
#if (PST_FRM_VS_EN > 0)
     &gs_PSTFrameVSStr,
#endif

#if (PST_FRM_BG_EN > 0)
    &gs_PSTFrameBGStr,
#endif

#if (PST_FRM_ZY_EN > 0)
    &gs_PSTFrameZYStr,
#endif


#if (PST_FRM_WM_EN > 0)
    &gs_PSTFrameWMStr,
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
    uint16 ut_i;                             //循环变量
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
                if(buffer[ut_i] == str->end)
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
                        MoveBuffer(buffer + ui_k + 2, buffer + ui_k + 1, sumlen + 1 - ui_k);
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
            case PST_ATR_CSCAL_CRC3:
                cs = DoCrcRtuModbus(
                    buffer + str->cssp, 
                    sumlen,
                    0xffff
                    );
                
                if(cs != ByteToInt(buffer + sumos))
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
                if(buffer[ut_i] == str->end)
                {
                    SYS_OK();                //返回扫描到有效帧
                }
            }
            return PST_ERR_NE;              //返回不是有效帧  
        }
        else
        {                                   //判断是否为结尾符
            if(buffer[str->extlen - 1 + len] != str->end)
            {
                return PST_ERR_NE;          //返回不是有效帧      
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
uint8 PST_Channel(PORT_UART_TYPE channel, uint8* buffer, uint16 length, uint8* protocol)
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
                    MoveBuffer(         //转移数据到缓存的头上
                        buffer + ut_j, 
                        buffer, 
                        length - ut_j
                        );
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
            case PST_ATR_CSCAL_CRC3:
                cs = DoCrcRtuModbus(
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
        buffer[len] = str->end;             //判断是否为结尾符            
        len++;                              //长度增加
    }
    return len;                             //返回长度
}

/******************************************************************************
**各个规约解析函数散转列表
******************************************************************************/
uint8 (*const gfs_PSTDecode[])(uint8* rbuffer, uint8* sbuffer, uint16 sbuflen, PST_Frame* frm) = 
{
#if (PST_FRM_VS_EN > 0)
    PST_VS_Decode,
#endif

#if (PST_FRM_BG_EN > 0)
    PST_BG_Decode,
#endif

#if (PST_FRM_ZY_EN > 0)
    PST_ZY_Decode,
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
#if (PST_FRM_VS_EN > 0)
    PST_VS_Proc,
#endif

#if (PST_FRM_BG_EN > 0)
    PST_BG_Proc,
#endif

#if (PST_FRM_ZY_EN > 0)
    PST_ZY_Proc,
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
            MoveBuffer(buffer + uc_i, buffer, len - uc_i);
            return (len - uc_i);
        }
    }
    return 0;
}
/************************************************************************
 * @function: __PST_CheckFrame
 * @描述: 帧有效性检查,允许一定数量的前导符,并过滤前导符
 * 
 * @参数: 
 * @param: nfrm 通讯的规约编号,如果nfrm 为 0xff表示为中继
 * @param: buffer 收发缓存
 * @param: len 输入为接收数据长度,输出为有效数据长度
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2013/11/3)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 PST_CheckFrame3(uint8 nfrm, uint8* buffer, uint16* len)
{
    uint16 tplen; 
    for(uint32 uc_i = 0; (uc_i < 7) && ((*len) - uc_i) > 0; uc_i++)
    {
        tplen = (*len) - uc_i;
        if(PST_CheckFrame(nfrm, buffer + uc_i, tplen) == PST_ERR_OK)
        {
            MoveBuffer(buffer + uc_i, buffer, tplen);
            (*len) = tplen;
            return 0;
        }
    }
    return 2;
}

uint16_t Netp_Get_FW_num(void)
{

    return gui_NetpFwTblSize;
}
