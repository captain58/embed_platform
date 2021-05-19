/****************************************Copyright (c)*************************
**                               	________�Ƽ����޹�˾
**                                     		������
**
**
**--------------�ļ���Ϣ-------------------------------------------------------
**��   ��   ��: dev_frm.c
**��   ��   ��: yzy
**����޸�����: 2010��02��26��
**��        ��: ��Լ�������
**ע        ��:
**--------------��ʷ�汾��Ϣ---------------------------------------------------
** ������: yzy
** ��  ��: v1.0
** �ա���: 2010��02��26��
** �衡��: ԭʼ�汾
******************************************************************************/
#define EXT_DEV_FRM


#include "aos/include.h"
//	#include "k_types.h"
//	#include "k_api.h"
//	#include "lib_app.h"
#include "pst_vs.h"
#include "pst_zy.h"

#define NETP_FRM_BS_96_8_1_N    0x18 //9600������  1ֹͣλ ��У�� 8λ����λ
#define NETP_FRM_BS_96_8_1_O    0x78 //9600������  1ֹͣλ żУ�� 8λ����λ
#define NETP_FRM_BS_48_8_1_N    0x1D //9600������  1ֹͣλ ��У�� 8λ����λ


/******************************************************************************
**�����Լ��Ϣ
******************************************************************************/
#if PST_FRM_BG_EN > 0
const uint8 gucs_PSTBGHead[] =          //���岿���ͷ
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


#define PST_FRM_BG_LP       0x09        //���峤����ʼ��λ��
#define PST_FRM_BG_LATR     (0)
#define PST_FRM_BG_END      0x16        //�����β��
#define PST_FRM_BG_CSSP     0x00        //�����ۼӺͼ�����ʼλ��
#define PST_FRM_BG_ATR      (PST_ATR_CSCAL_SC)
#define PST_FRM_BG_MLEN     0x0C        //��С֡�ĳ���
#define PST_FRM_BG_CSSL     0x82        //�����ۼӺͼ���ĳ���ƫ��
#define PST_FRM_BG_CSOS     0x82        //�����ۼӺ͵�λ��ƫ��


/******************************************************************************
**�����Լ�ṹ����Ϣ
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
**���ǹ�Լ��Ϣ
******************************************************************************/
#if PST_FRM_VS_EN > 0
const uint8 gucs_PSTVSHead[] =          //���岿���ͷ
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


#define PST_FRM_VS_LP       0x0a        //���峤����ʼ��λ��
#define PST_FRM_VS_LATR     (PST_LATR_DBLEN)
#define PST_FRM_VS_END      0x16        //�����β��
#define PST_FRM_VS_CSSP     0x00        //�����ۼӺͼ�����ʼλ��
#define PST_FRM_VS_ATR      (PST_ATR_CSCAL_SC)
#define PST_FRM_VS_MLEN     0x17        //��С֡�ĳ���
#define PST_FRM_VS_CSSL     0x82        //�����ۼӺͼ���ĳ���ƫ��
#define PST_FRM_VS_CSOS     0x82        //�����ۼӺ͵�λ��ƫ��


/******************************************************************************
**�����Լ�ṹ����Ϣ
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
**____��չ��Լ��Ϣ
******************************************************************************/
#if PST_FRM_ZY_EN > 0
const uint8 gucs_PSTZYHead[] =          //����____��չ��ͷ
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


#define PST_FRM_ZY_LP       0x09        //���峤����ʼ��λ��
#define PST_FRM_ZY_LATR     (PST_LATR_DBLEN)
#define PST_FRM_ZY_END      0x16        //�����β��
#define PST_FRM_ZY_CSSP     0x00        //�����ۼӺͼ�����ʼλ��
#define PST_FRM_ZY_ATR      (PST_ATR_CSCAL_SC)
#define PST_FRM_ZY_MLEN     13        //��С֡�ĳ���
#define PST_FRM_ZY_CSSL     0x82        //�����ۼӺͼ���ĳ���ƫ��
#define PST_FRM_ZY_CSOS     0x82        //�����ۼӺ͵�λ��ƫ��



/******************************************************************************
**____��չ��Լ�ṹ����Ϣ
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


#define PST_FRM_WM_LP       0x02        //���峤����ʼ��λ��
#define PST_FRM_WM_LATR     (PST_LATR_DBLEN)
#define PST_FRM_WM_END      0x16        //�����β��
#define PST_FRM_WM_CSSP     0x00        //�����ۼӺͼ�����ʼλ��
#define PST_FRM_WM_ATR      (PST_ATR_CSCAL_SC)
#define PST_FRM_WM_MLEN     0x06        //��С֡�ĳ���
#define PST_FRM_WM_CSSL     0x82        //�����ۼӺͼ���ĳ���ƫ��
#define PST_FRM_WM_CSOS     0x82        //�����ۼӺ͵�λ��ƫ��

/******************************************************************************
**����èͨѶ��Լ�ṹ����Ϣ
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
**����èͨѶ��Լ��Ϣ
******************************************************************************/
#if PST_FRM_WM_EN > 0

const uint8 gucs_PSTWMHead[] = 
{
    0xAA,
};


#define PST_FRM_WM_LP       0x02        //���峤����ʼ��λ��
#define PST_FRM_WM_LATR     (PST_LATR_DBLEN)
#define PST_FRM_WM_END      0x16        //�����β��
#define PST_FRM_WM_CSSP     0x00        //�����ۼӺͼ�����ʼλ��
#define PST_FRM_WM_ATR      (PST_ATR_CSCAL_SC)
#define PST_FRM_WM_MLEN     0x06        //��С֡�ĳ���
#define PST_FRM_WM_CSSL     0x82        //�����ۼӺͼ���ĳ���ƫ��
#define PST_FRM_WM_CSOS     0x82        //�����ۼӺ͵�λ��ƫ��

/******************************************************************************
**����èͨѶ��Լ�ṹ����Ϣ
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

const uint8 gucs_NetpWXICHead[] =          //WXIC��������
{
    0xDD,
};
    
#define NETP_FRM_WXIC_LP       0x02        //���峤����ʼ��λ��
#define NETP_FRM_WXIC_LATR     (PST_LATR_NOLEN)
#define NETP_FRM_WXIC_DLP       0x02        //������������ʼ��λ��

#define NETP_FRM_WXIC_END      00        //�����β��
#define NETP_FRM_WXIC_CSSP     0x00        //�����ۼӺͼ�����ʼλ��
#define NETP_FRM_WXIC_ATR      (PST_ATR_CSCAL_CRC3|PST_ATR_END)
#define NETP_FRM_WXIC_MLEN     4        //��С֡�ĳ���
#define NETP_FRM_WXIC_CSSL     0x82        //�����ۼӺͼ���ĳ���ƫ��
#define NETP_FRM_WXIC_CSOS     0x82        //�����ۼӺ͵�λ��ƫ��
#define NETP_FRM_WXIC_DIATR    (0)//��λ��DI��֡ͷλ��

#define NETP_FRM_COMP_WXIC_LP       0x00        //���峤����ʼ��λ��
#define NETP_FRM_COMP_WXIC_LATR     (PST_LATR_NOLEN)
#define NETP_FRM_COMP_WXIC_ADLEN     0x01       //��ַ��ƫ��
#define NETP_FRM_COMP_WXIC_ADLP      0x00//��λ1��ʾ ��ַ�򳤶�Ҫ�ۼ� 0��ʾ����Ҫ�ۼ�
#define NETP_FRM_COMP_WXIC_CMDLP     0x01
#define NETP_FRM_COMP_WXIC_DIATR    (PST_DIATR_DB_MB | 0x02)//��λ��DI��֡ͷλ��

#define NETP_FRM_COMP_WXIC_BIT (uint16_t)(PST_FRM_BIT_HEAD|PST_FRM_BIT_ADDR \
                                     | PST_FRM_BIT_CMD | PST_FRM_BIT_DATA \
                                     | PST_FRM_BIT_DI | PST_FRM_BIT_CS)
#define NETP_FRM_COMP_WXIC_LEN_BIT (uint16_t)(0)

#define NETP_FRM_WXIC_BS    0x18 //9600������  1ֹͣλ ��У�� 8λ����λ

/******************************************************************************
**____��չ��Լ�ṹ����Ϣ
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

const uint8 gucs_NetpTX13Head[] =          //����13
{
    0xCC,
};
    
#define NETP_FRM_TX13_LP       0x03        //���峤����ʼ��λ��
#define NETP_FRM_TX13_LATR     (PST_LATR_DBLEN)
#define NETP_FRM_TX13_DLP       0x05        //������������ʼ��λ��

#define NETP_FRM_TX13_END      0xEE        //�����β��
#define NETP_FRM_TX13_CSSP     0x00        //�����ۼӺͼ�����ʼλ��
#define NETP_FRM_TX13_ATR      (PST_ATR_CSCAL_SD)
#define NETP_FRM_TX13_MLEN     8        //��С֡�ĳ���
#define NETP_FRM_TX13_CSSL     0x83        //�����ۼӺͼ���ĳ���ƫ��
#define NETP_FRM_TX13_CSOS     0x83        //�����ۼӺ͵�λ��ƫ��
#define NETP_FRM_COMP_TX13_ATR      (PST_ATR_CSCAL_SD1)

#define NETP_FRM_COMP_TX13_LP       0x03        //���峤����ʼ��λ��
#define NETP_FRM_COMP_TX13_LATR     (PST_LATR_NOLEN)
#define NETP_FRM_COMP_TX13_ADLEN     0x01
#define NETP_FRM_COMP_TX13_ADLP      0x81//��λ1��ʾ ��ַ�򳤶�Ҫ�ۼ� 0��ʾ����Ҫ�ۼ�
#define NETP_FRM_COMP_TX13_CMDLP     0x02
#define NETP_FRM_COMP_TX13_DIATR    PST_DIATR_NONE

#define NETP_FRM_COMP_TX13_BIT (uint16_t)(PST_FRM_BIT_HEAD|PST_FRM_BIT_ADDR \
                                     | PST_FRM_BIT_CMD | PST_FRM_BIT_DATA \
                                     | PST_FRM_BIT_CS | PST_FRM_BIT_TAIL)
#define NETP_FRM_COMP_TX13_LEN_BIT (uint16_t)(0)

#define NETP_FRM_TX13_BS    0x18 //9600������  1ֹͣλ ��У�� 8λ����λ

/******************************************************************************
**____��չ��Լ�ṹ����Ϣ
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

//��-1��Ч
#define TX13_FLOAT_RANK_VALID                 0x80
#define TX13_FLOAT_RANK_MASK                  0x80
//�׳���
#define TX13_FLOAT_RANK_LEN_2               0x40 
#define TX13_FLOAT_RANK_LEN_MASK            0x60
//��������
#define TX13_FLOAT_INTEGER_LEN_4             0x10
#define TX13_FLOAT_INTEGER_LEN_MASK          0x1C
//С������
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

const uint8 gucs_NetpXDMAHead[] =          //�Ŷ�modbus
{
    0xDD,
};
#define NETP_FRM_XDMA_LATR     (PST_LATR_NOLEN)

#define NETP_FRM_XDMA_ATR      (PST_ATR_CSCAL_CRC3|PST_ATR_END)
#define NETP_FRM_XDMA_MLEN     4        //��С֡�ĳ���
#define NETP_FRM_XDMA_DIATR    (0)//��λ��DI��֡ͷλ��

#define NETP_FRM_COMP_XDMA_LP       0x00        //���峤����ʼ��λ��
#define NETP_FRM_COMP_XDMA_LATR     (PST_LATR_NOLEN)
#define NETP_FRM_COMP_XDMA_ADLEN     0x01       //��ַ��ƫ��
#define NETP_FRM_COMP_XDMA_ADLP      0x00//��λ1��ʾ ��ַ�򳤶�Ҫ�ۼ� 0��ʾ����Ҫ�ۼ�
#define NETP_FRM_COMP_XDMA_CMDLP     0x01
#define NETP_FRM_COMP_XDMA_DIATR    (PST_DIATR_DB_MB | 0x02)//��λ��DI��֡ͷλ��

#define NETP_FRM_COMP_XDMA_BIT (uint16_t)(PST_FRM_BIT_HEAD|PST_FRM_BIT_ADDR \
                                     | PST_FRM_BIT_CMD | PST_FRM_BIT_DATA \
                                     | PST_FRM_BIT_DI | PST_FRM_BIT_CS)

#define NETP_FRM_XDMA_BS    0x18 //9600������  1ֹͣλ ��У�� 8λ����λ



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

#define NETP_FRM_WITHOUT_END      00        //�����β��
#define NETP_FRM_XDMA_ATR      (PST_ATR_CSCAL_CRC3|PST_ATR_END)
#define NETP_FRM_XDMA_MLEN     4        //��С֡�ĳ���
//	#define NETP_FRM_CSSL_SUB_2     0x82        //�����ۼӺͼ���ĳ���ƫ��
//	#define NETP_FRM_CSOS_SUB_2     0x82        //�����ۼӺ͵�λ��ƫ��
#define NETP_FRM_XDMA_DIATR    (0)//��λ��DI��֡ͷλ��

#define NETP_FRM_COMP_XDMA_LP       0x00        //���峤����ʼ��λ��
#define NETP_FRM_COMP_XDMA_LATR     (PST_LATR_NOLEN)
#define NETP_FRM_COMP_XDMA_ADLEN     0x01       //��ַ��ƫ��
#define NETP_FRM_COMP_XDMA_ADLP      0x00//��λ1��ʾ ��ַ�򳤶�Ҫ�ۼ� 0��ʾ����Ҫ�ۼ�
#define NETP_FRM_COMP_XDMA_CMDLP     0x01
#define NETP_FRM_COMP_XDMA_DIATR    (PST_DIATR_DB_MB | 0x02)//��λ��DI��֡ͷλ��

#define NETP_FRM_COMP_XDMA_BIT (uint16_t)(PST_FRM_BIT_HEAD|PST_FRM_BIT_ADDR \
                                     | PST_FRM_BIT_CMD | PST_FRM_BIT_DATA \
                                     | PST_FRM_BIT_DI | PST_FRM_BIT_CS)
#define NETP_FRM_COMP_XDMA_LEN_BIT (uint16_t)(0)

#define NETP_FRM_XDMA_BS    0x18 //9600������  1ֹͣλ ��У�� 8λ����λ



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
#if CID_CNV12 > 0//����V12modbus

#define NETP_FRM_XDMA_LATR     (PST_LATR_NOLEN)


#define NETP_FRM_WITHOUT_END      00        //�����β��
#define NETP_FRM_XDMA_ATR      (PST_ATR_CSCAL_CRC3|PST_ATR_END)
#define NETP_FRM_XDMA_MLEN     4        //��С֡�ĳ���
//	#define NETP_FRM_CSSL_SUB_2     0x82        //�����ۼӺͼ���ĳ���ƫ��
//	#define NETP_FRM_CSOS_SUB_2     0x82        //�����ۼӺ͵�λ��ƫ��
#define NETP_FRM_XDMA_DIATR    (0)//��λ��DI��֡ͷλ��

#define NETP_FRM_COMP_XDMA_LP       0x00        //���峤����ʼ��λ��
#define NETP_FRM_COMP_XDMA_LATR     (PST_LATR_NOLEN)
#define NETP_FRM_COMP_XDMA_ADLEN     0x01       //��ַ��ƫ��
#define NETP_FRM_COMP_XDMA_ADLP      0x00//��λ1��ʾ ��ַ�򳤶�Ҫ�ۼ� 0��ʾ����Ҫ�ۼ�
#define NETP_FRM_COMP_XDMA_CMDLP     0x01
#define NETP_FRM_COMP_XDMA_DIATR    (PST_DIATR_DB_MB | 0x02)//��λ��DI��֡ͷλ��

#define NETP_FRM_COMP_XDMA_BIT (uint16_t)(PST_FRM_BIT_HEAD|PST_FRM_BIT_ADDR \
                                     | PST_FRM_BIT_CMD | PST_FRM_BIT_DATA \
                                     | PST_FRM_BIT_DI | PST_FRM_BIT_CS)
#define NETP_FRM_COMP_XDMA_LEN_BIT (uint16_t)(0)

#define NETP_FRM_XDMA_BS    0x18 //9600������  1ֹͣλ ��У�� 8λ����λ


//����V12
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
#if CID_ETM1 > 0//����˹��MODBUS(EK-205) 

    
#define NETP_FRM_XDMA_LATR     (PST_LATR_NOLEN)

#define NETP_FRM_XDMA_ATR      (PST_ATR_CSCAL_CRC3|PST_ATR_END)
#define NETP_FRM_XDMA_MLEN     4        //��С֡�ĳ���
//	#define NETP_FRM_CSSL_SUB_2     0x82        //�����ۼӺͼ���ĳ���ƫ��
//	#define NETP_FRM_CSOS_SUB_2     0x82        //�����ۼӺ͵�λ��ƫ��
#define NETP_FRM_XDMA_DIATR    (0)//��λ��DI��֡ͷλ��

#define NETP_FRM_COMP_XDMA_LP       0x00        //���峤����ʼ��λ��
#define NETP_FRM_COMP_XDMA_LATR     (PST_LATR_NOLEN)
#define NETP_FRM_COMP_XDMA_ADLEN     0x01       //��ַ��ƫ��
#define NETP_FRM_COMP_XDMA_ADLP      0x00//��λ1��ʾ ��ַ�򳤶�Ҫ�ۼ� 0��ʾ����Ҫ�ۼ�
#define NETP_FRM_COMP_XDMA_CMDLP     0x01
#define NETP_FRM_COMP_XDMA_DIATR    (PST_DIATR_DB_MB | 0x02)//��λ��DI��֡ͷλ��

#define NETP_FRM_COMP_XDMA_BIT (uint16_t)(PST_FRM_BIT_HEAD|PST_FRM_BIT_ADDR \
                                     | PST_FRM_BIT_CMD | PST_FRM_BIT_DATA \
                                     | PST_FRM_BIT_DI | PST_FRM_BIT_CS)
#define NETP_FRM_COMP_XDMA_LEN_BIT (uint16_t)(0)



//����˹��MODBUS(EK-205) 
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
#if CID_SZMA > 0//˼��modbus
    
#define NETP_FRM_XDMA_LATR     (PST_LATR_NOLEN)

#define NETP_FRM_XDMA_ATR      (PST_ATR_CSCAL_CRC3|PST_ATR_END)
#define NETP_FRM_XDMA_MLEN     4        //��С֡�ĳ���
//	#define NETP_FRM_CSSL_SUB_2     0x82        //�����ۼӺͼ���ĳ���ƫ��
//	#define NETP_FRM_CSOS_SUB_2     0x82        //�����ۼӺ͵�λ��ƫ��
#define NETP_FRM_XDMA_DIATR    (0)//��λ��DI��֡ͷλ��

#define NETP_FRM_COMP_XDMA_LP       0x00        //���峤����ʼ��λ��
#define NETP_FRM_COMP_XDMA_LATR     (PST_LATR_NOLEN)
#define NETP_FRM_COMP_XDMA_ADLEN     0x01       //��ַ��ƫ��
#define NETP_FRM_COMP_XDMA_ADLP      0x00//��λ1��ʾ ��ַ�򳤶�Ҫ�ۼ� 0��ʾ����Ҫ�ۼ�
#define NETP_FRM_COMP_XDMA_CMDLP     0x01
#define NETP_FRM_COMP_XDMA_DIATR    (PST_DIATR_DB_MB | 0x02)//��λ��DI��֡ͷλ��

#define NETP_FRM_COMP_XDMA_BIT (uint16_t)(PST_FRM_BIT_HEAD|PST_FRM_BIT_ADDR \
                                     | PST_FRM_BIT_CMD | PST_FRM_BIT_DATA \
                                     | PST_FRM_BIT_DI | PST_FRM_BIT_CS)
#define NETP_FRM_COMP_XDMA_LEN_BIT (uint16_t)(0)


//˼�� UFG620 modbus
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
#if CID_ZHMA > 0//�к�ά˼MODBUS CL-2 

    
#define NETP_FRM_XDMA_LATR     (PST_LATR_NOLEN)

#define NETP_FRM_XDMA_ATR      (PST_ATR_CSCAL_CRC3|PST_ATR_END)
#define NETP_FRM_XDMA_MLEN     4        //��С֡�ĳ���
//	#define NETP_FRM_CSSL_SUB_2     0x82        //�����ۼӺͼ���ĳ���ƫ��
//	#define NETP_FRM_CSOS_SUB_2     0x82        //�����ۼӺ͵�λ��ƫ��
#define NETP_FRM_XDMA_DIATR    (0)//��λ��DI��֡ͷλ��

#define NETP_FRM_COMP_XDMA_LP       0x00        //���峤����ʼ��λ��
#define NETP_FRM_COMP_XDMA_LATR     (PST_LATR_NOLEN)
#define NETP_FRM_COMP_XDMA_ADLEN     0x01       //��ַ��ƫ��
#define NETP_FRM_COMP_XDMA_ADLP      0x00//��λ1��ʾ ��ַ�򳤶�Ҫ�ۼ� 0��ʾ����Ҫ�ۼ�
#define NETP_FRM_COMP_XDMA_CMDLP     0x01
#define NETP_FRM_COMP_XDMA_DIATR    (PST_DIATR_DB_MB | 0x02)//��λ��DI��֡ͷλ��

#define NETP_FRM_COMP_XDMA_BIT (uint16_t)(PST_FRM_BIT_HEAD|PST_FRM_BIT_ADDR \
                                     | PST_FRM_BIT_CMD | PST_FRM_BIT_DATA \
                                     | PST_FRM_BIT_DI | PST_FRM_BIT_CS)
#define NETP_FRM_COMP_XDMA_LEN_BIT (uint16_t)(0)



//�к�ά˼MODBUS CL-2 
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

#define NETP_FRM_XKMA_END      0x5E        //�����β��
#define NETP_FRM_CSSP_FROM_1     0x01        //�����ۼӺͼ�����ʼλ��
#define NETP_FRM_XKZD_ATR      (PST_ATR_PARIT)
#define NETP_FRM_XKZD_MLEN     7        //��С֡�ĳ���

#define NETP_FRM_XKZD_DIATR    (0)//��λ��DI��֡ͷλ��
#define NETP_FRM_COMP_XKZD_ATR      (PST_ATR_PARIT|PST_ATR_END)

#define NETP_FRM_COMP_XKZD_LP       0x00        //���峤����ʼ��λ��
#define NETP_FRM_COMP_XKZD_LATR     (PST_LATR_NOLEN)
#define NETP_FRM_COMP_XKZD_ADLEN     0x02       //��ַ�򳤶�
#define NETP_FRM_COMP_XKZD_ADLP      (PST_ADLP_ADDLEN|PST_ADLP_ADD_TO_ASCII|0x01)//��λ1��ʾ ��ַ�򳤶�Ҫ�ۼ� 0��ʾ����Ҫ�ۼ�
#define NETP_FRM_COMP_XKZD_CMDLP     0x03
#define NETP_FRM_COMP_XKZD_DIATR    (PST_DIATR_DB_MB | 0x02)//��λ��DI��֡ͷλ��

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

#if CID_SDAS > 0// ˹����ASͨ��Э��(AS-25\32\40\50\80)(ʵΪ��׼Modbus RTUЭ��)      

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

#if CID_SDM2 > 0// ˹����ASͨ��Э��(AS-25\32\40\50\80)(ʵΪ��׼Modbus RTUЭ��)      

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
#if CID_ITRON > 0//����MODBUS  

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
#if CID_DWVPM3X > 0// ����MODBUS PTZ-BOX3.0 ��Э�飨long�ͣ�   

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
#if CID_SSMA > 0//ʤ��˹Modbus
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
#if CID_HDMA > 0//����˹Modbus
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
#if CID_RTMA > 0//������Modbus(Romet_Modbus)
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
#if CID_XKVM1 > 0//�¿�Modbus1
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
#if CID_XKVM2 > 0//�¿�Modbus2
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
**����ͨ����Ϣ�ṹ�����鶨�弰��ʼ��
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
**����֡�ṹ����
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
 * @����: ָ��֡���͵���Ч֡�ж�
 * 
 * @����: 
 * @param: num ��Ҫ�жϵ�֡�����ͱ��
 * @param: buffer ��Ҫ�ж���Ч֡�Ļ���
 * @param: length ��Ҫ�жϵ�֡�ĳ���
 * 
 * @����: 
 * @return: uint8  PST_ERR_OK
 *      PST_ERR_VAR
 *      PST_ERR_TS
 *      PST_ERR_NE
 * @˵��: 
 * @����: yzy (2013/7/15)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 PST_CheckFrame(uint8 num, uint8* buffer, uint16 length)
{
    const PSTFrameStr* str;                 //����ָ��֡��Ϣ�ṹ��
    volatile uint16 len;                   //����֡����������
    uint16 ut_i;                             //ѭ������
    uint16 sumlen;                          //��У�����ݳ���
    uint16 sumos;                           //У��ʹ�ŵ�λ��
    uint16 cs;

    SYS_VAR_CHECK(num >= PST_FRM_NO);        //������֤
    SYS_VAR_CHECK(length > PST_LEN_MAX);     //������֤
                                            //��ȡ��ǰҪɨ��Ĺ�Լ�Ľṹ��
    str = (PSTFrameStr*)gss_PSTFrameStr[num];

    if(str->extlen > length)                //�ж��Ƿ��Ѿ��ﵽ��С֡����
    {
        return PST_ERR_TS;                  //����ɨ�賤��̫��
    }
                                            //�ж�֡ͷ
    for(ut_i = 0; ut_i < str->hlen; ut_i++)
    {                                       //�����ƥ���ַ�ƥ����ô������һ��
        if(buffer[ut_i] == str->head[ut_i])
        {
            continue;
        }
                                            //�����ǰ��ƥ���ַ�Ϊ����ƥ���ַ������
        if(str->head[ut_i] == PST_FRM_ANY) 
        {
            continue;
        }
        return PST_ERR_NE;                  //���ز�����Ч֡
    }

    if(str->latr & PST_LATR_NOLEN)          //û������������
    {
        len = 0;                            //��������������Ϊ0
        
        //�������������ݳ��ȵ�����£����ж�У��ͣ�ȷ������������
        if(!(str->atr & PST_ATR_END))           //�н�β��
        {
                                            //ѭ��ɨ���β��
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
                return PST_ERR_NE;                  //���ز�����Ч֡
            }
        }
    }
    else                                    //������������
    {
        if(str->latr & PST_LATR_DBLEN)      //�����˫�ֽڳ���
        {
            if(str->latr &  PST_LATR_BFLEN) //���ֽ���ǰ
            {
                len = buffer[str->lp + 1]           //���ݳ���
                    + (buffer[str->lp ] << 8);
            }
            else                            //���ֽ���ǰ
            {
                len = buffer[str->lp]           //���ݳ���
                    + (buffer[str->lp + 1] << 8);
            }
        }
        else                                //������ǵ��ֽڵ�֡��
        {
            len = buffer[str->lp];          //���ݳ���
        }
        
        if(str->latr & PST_LATR_DBLEN)      //�����˫�ֽڳ���
        {
            len <<= str->llft;              //Ϊ��ֹ���뱻�������Ż�����Ҫ����Ϊvolatile����
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

    if(len + str->extlen > length)          //�����������������С֡����֮�����ɨ���֡����ô�϶�����һ����Ч֡
    {
        return PST_ERR_NE;                  //���ز�����Ч֡    
    }
                                            
    if(!(str->atr & PST_ATR_PARIT))         //��У��λ
    {
                                            //�����У�����ݵĳ���
        sumlen = len + str->extlen;
        if(str->cssl & 0x80)                //��
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
        else                                //��
        {
            sumlen += (str->cssl & 0x7F);
        }
                                            //����У��ʹ�ŵ�λ��ƫ��
        sumos = len + str->extlen;
        if(str->csco & 0x80)                //��
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
        else                                //��
        {
            sumos += (str->csco & 0x7F);
        }
        
        switch(str->atr & PST_ATR_CSCAL)
        {
            case PST_ATR_CSCAL_SC:          //�ۼӺ�
                cs = DoSum(
                    buffer + str->cssp, 
                    sumlen
                    );
                
                if((cs & 0xFF) != buffer[sumos])
                {
                    return PST_ERR_NE;      //���ز�����Ч֡   
                }
                break;
                
            case PST_ATR_CSCAL_SD:          //˫�ֽ��ۼӺ�
                cs = DoSum(
                    buffer + str->cssp, 
                    sumlen
                    );
                
                if(cs != ByteToInt(buffer + sumos))
                {
                    return PST_ERR_NE;      //���ز�����Ч֡   
                }
                break;

            case PST_ATR_CSCAL_BS:          //λ�Ӻ�
                cs = DoBitSum(
                    buffer + str->cssp, 
                    sumlen
                    );

                if((uint8)cs != buffer[sumos])
                {
                    return PST_ERR_NE;      //���ز�����Ч֡      
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
                    return PST_ERR_NE;      //���ز�����Ч֡   
                }
                break;
            case PST_ATR_CSCAL_CRC2:    //EDMI, ���жϼ����֮ǰ����Ҫ��ת�봦��
                for(uint16 ui_k = 1; ui_k < (length - 1); ui_k++)  //ȥ��ת���
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
                    return PST_ERR_NE;      //���ز�����Ч֡   
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
                    return PST_ERR_NE;      //���ز�����Ч֡   
                }
                break;
                
            default:
                return PST_ERR_NE;          //���ز�����Ч֡         
        }
    }

    if(!(str->atr & PST_ATR_END))           //�н�β��
    {
        if(str->latr & PST_LATR_NOLEN)      //û������������
        {
                                            //ѭ��ɨ���β��
            for(ut_i = str->extlen - 1; ut_i < length; ut_i++)
            {
                if(buffer[ut_i] == str->end)
                {
                    SYS_OK();                //����ɨ�赽��Ч֡
                }
            }
            return PST_ERR_NE;              //���ز�����Ч֡  
        }
        else
        {                                   //�ж��Ƿ�Ϊ��β��
            if(buffer[str->extlen - 1 + len] != str->end)
            {
                return PST_ERR_NE;          //���ز�����Ч֡      
            }
        }
    }
    SYS_OK();                                //����ɨ�赽��Ч֡
}


/************************************************************************
 * @function: PST_Channel
 * @����: ��ǰ����ɨ����ձ���,�жϳ�Ϊ���ֹ�Լ
 * 
 * @����: 
 * @param: channel ��Ҫ�ж���Ч֡��ͨ�����
 * @param: buffer ��Ҫ�ж���Ч֡�Ļ���
 * @param: length ��Ҫ�жϵ�֡�ĳ���
 * @param: protocol ���,ʵ�ʹ�Լ���,�����0xFF�ͱ�ʾ������Ч��Լ(��:PST_FRM_ZY_NO)
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2013/7/15)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 PST_Channel(PORT_UART_TYPE channel, uint8* buffer, uint16 length, uint8* protocol)
{
    const PSTChnInfo* info;             //ͨ����Ϣ
    uint16 ut_i, ut_j;                  //����ѭ������
                                        //����ȷ��
    SYS_VAR_CHECK(channel >= PST_CHN_NO);
    SYS_VAR_CHECK(length > PST_LEN_MAX);
    
    info = gss_PSTChnInfo + channel;    //��ȡ��ǰ��ͨ����Ϣ�ṹ��
                                        //��ͨ��֧�ֵĹ�Լ����ɨ��
    for(ut_i = 0; ut_i < info->count; ut_i++)
    {                                   //��ǰ���ַ�����ɨ��
        for(ut_j = 0; ut_j <= info->headchars; ut_j++)
        {
            switch(PST_CheckFrame(      //������Ч֡���ж�,���Խ��ɢת����
                info->frameno[ut_i],
                buffer + ut_j,
                length - ut_j))
            {
                case PST_ERR_OK:        //ɨ�赽����Ч֡
                    MoveBuffer(         //ת�����ݵ������ͷ��
                        buffer + ut_j, 
                        buffer, 
                        length - ut_j
                        );
                                        //��ǰ��Ч�Ĺ�Լ���
                    *protocol = info->frameno[ut_i];
                    SYS_OK();

                case PST_ERR_TS:        // ���֡��̫С
                case PST_ERR_VAR:       // ���߲�������
                                        // �˳���ǰѭ��
                    ut_j = info->headchars;
                    break;               
            }
        }
    }

    *protocol = 0xFF;                   //��Ч�Ĺ�Լ���
    return PST_ERR_NE;                  //����û��ɨ�赽��Ч֡
}


/************************************************************************
 * @function: PST_DoneFrame
 * @����: ����֡ͷ\֡β\�ۼӺ͵���д,����û�����������ȵ�֡�����޷��������
 * 
 * @����: 
 * @param: num ��Ҫ�����֡�����ͱ��
 * @param: buffer ��Ҫ�������Ч֡�Ļ���
 * 
 * @����: 
 * @return: uint16  0 ���ʧ��, ���� ��������ݵĳ���
 * @˵��: 
 * @����: yzy (2013/7/15)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint16 PST_DoneFrame(uint8 num, uint8* buffer)
{
    const PSTFrameStr* str;                 //����ָ��֡��Ϣ�ṹ��
    uint16 ut_i;                            //ѭ������
    uint16 len;                             //����֡����������
    uint16 sumlen;                          //��У�����ݳ���
    uint16 sumos;                           //У��ʹ�ŵ�λ��
    uint16 cs;

    if(num >= PST_FRM_NO)                   //������֤
    {
        return 0;
    }
                                            //��ȡ��ǰҪɨ��Ĺ�Լ�Ľṹ��
    str = (PSTFrameStr*)gss_PSTFrameStr[num];
    
                                            //֡ͷ����д
    for(ut_i = 0; ut_i < str->hlen; ut_i ++)
    {
        if(str->head[ut_i] != PST_FRM_ANY)
        {
            buffer[ut_i] = str->head[ut_i];
        }
    }

    if(str->latr & PST_LATR_NOLEN)          //û������������
    {
        return 0;                           //û�г������Ļ�����û�����
    }
    else                                    //������������
    {
        if(str->latr & PST_LATR_DBLEN)      //�����˫�ֽڳ���
        {
            if(str->latr &  PST_LATR_BFLEN) //���ֽ���ǰ
            {
                len = buffer[str->lp + 1]    //���ݳ���
                    + (buffer[str->lp ] << 8);
            }
            else                            //���ֽ���ǰ
            {
                len = buffer[str->lp]        //���ݳ���
                    + (buffer[str->lp + 1] << 8);
            }
        }
        else                                //������ǵ��ֽڵ�֡��
        {
            len = buffer[str->lp];          //���ݳ���
        }
        
        if(str->latr & PST_LATR_DBLEN)      //�����˫�ֽڳ���
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
    
    if(len + str->extlen > PST_LEN_MAX)     //�����������������С֡����֮�����ɨ���֡����ô�϶�����һ����Ч֡
    {
        return 0;                           //���ز�����Ч֡    
    }
    
    if(!(str->atr & PST_ATR_PARIT))         //��У��λ
    {
                                            //�����У�����ݵĳ���
        sumlen = len + str->extlen;
        if(str->cssl & 0x80)                //��
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
        else                                //��
        {
            sumlen += (str->cssl & 0x7F);
        }
                                            //����У��ʹ�ŵ�λ��ƫ��
        sumos = len + str->extlen;
        if(str->csco & 0x80)                //��
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
        else                                //��
        {
            sumos += (str->csco & 0x7F);
        }
        
        switch(str->atr & PST_ATR_CSCAL)
        {
            case PST_ATR_CSCAL_SC:            
                cs = DoSum(                 //�����ۼӺ�����
                    buffer + str->cssp, 
                    sumlen
                    );
                                            //�ۼӺ͸�ֵ
                buffer[sumos] = (uint8)cs;
                break;
                
            case PST_ATR_CSCAL_SD:            
                cs = DoSum(                 //�����ۼӺ�����
                    buffer + str->cssp, 
                    sumlen
                    );
                                            //�ۼӺ͸�ֵ
                IntToByte(buffer + sumos, cs);
                break;
                
            case PST_ATR_CSCAL_BS:
                cs = DoBitSum(              //����λ�Ӻ�����
                    buffer + str->cssp, 
                    sumlen
                    );
                                            //�ۼӺ͸�ֵ
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
                return 0;                   //���ش��ʧ��
        }
        len += str->extlen - 1;             //��������
    }

    if(!(str->atr & PST_ATR_END))           //�н�β��
    {
        buffer[len] = str->end;             //�ж��Ƿ�Ϊ��β��            
        len++;                              //��������
    }
    return len;                             //���س���
}

/******************************************************************************
**������Լ��������ɢת�б�
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
 * @����: ��Լ��������
 * 
 * 
 * @����: 
 * @param: protocal  ��Լ���
 * @param: rbuffer  ���ջ���
 * @param: sbuffer  ���ͻ���
 * @param: sbuflen  ���ͻ��泤��
 * @param: frm  ��Լ֡�ṹ����Ϣָ��
 * 
 * @����: 
 * @return:  uint8   
 * @����: yzy (2010-02-27)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
uint8 FRM_Decode(uint8 protocal, uint8* rbuffer, uint8* sbuffer, uint16 sbuflen, PST_Frame* frm)
{
    SYS_VAR_CHECK(protocal >= PST_FRM_NO);   //�������
    return gfs_PSTDecode[protocal](rbuffer, sbuffer, sbuflen, frm);         //ɢת����
}



/******************************************************************************
**������Լ����ɢת����
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
 * @����: ��Լ������
 * 
 * 
 * @����: 
 * @param: protocal  ��Լ���
 * @param: frm  ��Լ֡�ṹ����Ϣָ��
 * 
 * @����: 
 * @return:  uint8   
 * @����: yzy (2010-03-03)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
uint8 FRM_Proc(uint8 protocal, PST_Frame* frm)
{
    SYS_VAR_CHECK(protocal >= PST_FRM_NO);   //��������
    return gfs_PSTProc[protocal](frm);           //ɢת����
}




/************************************************************************
 * @function: PST_CheckFrame2
 * @����: ָ������֡��Ч�Եļ��(������ǰ����,��ȥ��ǰ����./��ȥ��֡�����Ч�ַ�)
 * 
 * @����: 
 * @param: nfrm ͨѶ�Ĺ�Լ���.���nfrmΪ0xff��ʾΪ�м�
 * @param: buffer ֡����
 * @param: len ֡����
 * 
 * @����: 
 * @return: uint16 �ϸ�֡�ĳ���(0��ʾû�кϸ�֡)
 * @˵��: 
 * @����: yzy (2012-5-15)
 *-----------------------------------------------------------------------
 * @�޸���: 
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
 * @����: ֡��Ч�Լ��,����һ��������ǰ����,������ǰ����
 * 
 * @����: 
 * @param: nfrm ͨѶ�Ĺ�Լ���,���nfrm Ϊ 0xff��ʾΪ�м�
 * @param: buffer �շ�����
 * @param: len ����Ϊ�������ݳ���,���Ϊ��Ч���ݳ���
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2013/11/3)
 *-----------------------------------------------------------------------
 * @�޸���: 
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
