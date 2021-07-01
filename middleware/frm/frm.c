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
** Description: �ļ�����
*************************************************************************/
#define EXT_DEV_FRM

#include "sys.h"
#include "hal.h"
#include "bsp.h"
//#include "app.h"        //��ҪӦ�ò㶨�����Լ������
#include "public.h"
#include "frmcfg.h"
#include "frm.h"
#include <string.h>

#include "pst_zy.h"
const uint8 gucs_PSTEndX16[] =          //���岿���β
{
    0x16,
};


const uint8 gucs_PSTEndX7E[] =          //����DLMS��β
{
    0x7E,
};


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
**____��չ��Լ��Ϣ
******************************************************************************/
//	#if PST_FRM_ZY_EN > 0
//	const uint8 gucs_PSTZYHead[] =          //����____��չ��ͷ
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
//	#define PST_FRM_ZY_LP       0x09        //���峤����ʼ��λ��
//	#define PST_FRM_ZY_LATR     (0)
//	#define PST_FRM_ZY_END      0x16        //�����β��
//	#define PST_FRM_ZY_CSSP     0x00        //�����ۼӺͼ�����ʼλ��
//	#define PST_FRM_ZY_ATR      (PST_ATR_CSCAL_SC)
//	#define PST_FRM_ZY_MLEN     0x0C        //��С֡�ĳ���
//	#define PST_FRM_ZY_CSSL     0x82        //�����ۼӺͼ���ĳ���ƫ��
//	#define PST_FRM_ZY_CSOS     0x82        //�����ۼӺ͵�λ��ƫ��
//	
//	
//	
//	/******************************************************************************
//	**____��չ��Լ�ṹ����Ϣ
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
**____��չ��Լ��Ϣ
******************************************************************************/
#if PST_FRM_ZBF_EN > 0
const uint8 gucs_PSTZBFHead[] =          //����____��չ��ͷ
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


#define PST_FRM_ZBF_LP       0x09        //���峤����ʼ��λ��
#define PST_FRM_ZBF_LATR     (PST_LATR_DBLEN)
#define PST_FRM_ZBF_END      0x16        //�����β��
#define PST_FRM_ZBF_CSSP     0x00        //�����ۼӺͼ�����ʼλ��
#define PST_FRM_ZBF_ATR      (PST_ATR_CSCAL_SC)
#define PST_FRM_ZBF_MLEN     0x0D        //��С֡�ĳ���
#define PST_FRM_ZBF_CSSL     0x82        //�����ۼӺͼ���ĳ���ƫ��
#define PST_FRM_ZBF_CSOS     0x82        //�����ۼӺ͵�λ��ƫ��


/******************************************************************************
**____��չ��Լ�ṹ����Ϣ
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
** *SET *GET��Լ��Ϣ
******************************************************************************/
#if PST_FRM_GS_EN > 0
const uint8 gucs_PSTGSHead[] =          //����*SET *GET��Լ��ͷ
{
    '*',
    PST_FRM_ANY,
    'E',
    'T',
};
const uint8 gucs_PSTEndGS[] =          //����DLMS��β</h:rt>
{
    '#',
};


#define PST_FRM_GS_LP       0x00        //���峤����ʼ��λ��
#define PST_FRM_GS_LATR     (PST_LATR_NOLEN)
#define PST_FRM_GS_END      '#'         //�����β��
#define PST_FRM_GS_CSSP     0x00        //�����ۼӺͼ�����ʼλ��
#define PST_FRM_GS_ATR      (PST_ATR_PARIT)
#define PST_FRM_GS_MLEN     0x0F        //��С֡�ĳ���
#define PST_FRM_GS_CSSL     0x00        //�����ۼӺͼ���ĳ���ƫ��
#define PST_FRM_GS_CSOS     0x00        //�����ۼӺ͵�λ��ƫ��

/******************************************************************************
** *SET * GETͨѶ��Լ�ṹ����Ϣ
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
**����1376.1��Լ֡��Ϣ
******************************************************************************/
#if PST_FRM_GD_1_EN > 0

const uint8 gucs_PSTGD1Head[] =          //��������Լ��ͷ
{
    0x68, 
    PST_FRM_ANY,
    PST_FRM_ANY,
    PST_FRM_ANY,
    PST_FRM_ANY,
    0x68,
};


#define PST_FRM_GD_1_LP       0x01        //���峤����ʼ��λ��
#define PST_FRM_GD_1_LATR     (PST_LATR_DBLEN | PST_LATR_RELEN)
#define PST_FRM_GD_1_LLFR     0           //����ʱ������
#define PST_FRM_GD_1_LRFR     2           //����ʱ������
#define PST_FRM_GD_1_END      0x16        //�����β��
#define PST_FRM_GD_1_CSSP     0x06        //�����ۼӺͼ�����ʼλ��
#define PST_FRM_GD_1_ATR      (PST_ATR_CSCAL_SC)
#define PST_FRM_GD_1_MLEN     0x08        //��С֡�ĳ���
#define PST_FRM_GD_1_CSSL     0x88        //�����ۼӺͼ���ĳ���ƫ��
#define PST_FRM_GD_1_CSOS     0x82        //�����ۼӺ͵�λ��ƫ��


/******************************************************************************
**����1376.1��Լ֡�ṹ����Ϣ
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
**����1376.2��Լ֡��Ϣ
******************************************************************************/
#if PST_FRM_GD_2_EN > 0

const uint8 gucs_PSTGD2Head[] =          //����1376.2��Լ��ͷ
{
    0x68,
};


#define PST_FRM_GD_2_LP       0x01        //���峤����ʼ��λ��
#define PST_FRM_GD_2_LATR     (PST_LATR_DBLEN)
#define PST_FRM_GD_2_END      0x16        //�����β��
#define PST_FRM_GD_2_CSSP     0x02        //�����ۼӺͼ�����ʼλ��(���������û�������)
#define PST_FRM_GD_2_ATR      (PST_ATR_CSCAL_SC)
#define PST_FRM_GD_2_MLEN     0           //��С֡�ĳ���
#define PST_FRM_GD_2_CSSL     0x84        //�����ۼӺͼ���ĳ���ƫ��
#define PST_FRM_GD_2_CSOS     0x82        //�����ۼӺ͵�λ��ƫ��

/******************************************************************************
**����1376.2��Լ֡�ṹ����Ϣ
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
**R8C-ESAM��Լ֡ͷ
******************************************************************************/
const uint8 gucs_PSTEMHead[] =
{
    0xA5, 
    PST_FRM_ANY,
    0xA5,
};


#define PST_FRM_EM_LP       0x03        //���峤����ʼ��λ��
#define PST_FRM_EM_LATR     (PST_LATR_DBLEN)
#define PST_FRM_EM_END      0x16        //�����β��
#define PST_FRM_EM_CSSP     0x00        //�����ۼӺͼ�����ʼλ��
#define PST_FRM_EM_ATR      (PST_ATR_CSCAL_SC)
#define PST_FRM_EM_MLEN     0x07        //��С֡�ĳ�����Ҫ����
#define PST_FRM_EM_CSSL     0x82        //�����ۼӺͼ���ĳ���ƫ��
#define PST_FRM_EM_CSOS     0x82        //�����ۼӺ͵�λ��ƫ��


/******************************************************************************
**R8C-ESAM��Լ֡�ṹ����Ϣ
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
**ȫ��������ͨ������Э��ӿڹ�Լ֡ͷ
******************************************************************************/
const uint8 gucs_PSTWL1Head[] =
{
    PST_FRM_ANY,
};


#define PST_FRM_WL_1_LP       0x01        //���峤����ʼ��λ��
#define PST_FRM_WL_1_LATR     (PST_LATR_DBLEN) //
#define PST_FRM_WL_1_LLFT     0           //����ʱ������
#define PST_FRM_WL_1_LRFT     0           //����ʱ������
#define PST_FRM_WL_1_END      0             //�����β��
#define PST_FRM_WL_1_CSSP     0x00        //�����ۼӺͼ�����ʼλ��
#define PST_FRM_WL_1_ATR      (PST_ATR_END | PST_ATR_CSCAL_SC)
#define PST_FRM_WL_1_MLEN     0x00        //��С֡�ĳ�����Ҫ����
#define PST_FRM_WL_1_CSSL     0x81        //�����ۼӺͼ���ĳ���ƫ��
#define PST_FRM_WL_1_CSOS     0x81        //�����ۼӺ͵�λ��ƫ��

/******************************************************************************
**DLMS��Լ֡�ṹ����Ϣ
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
**587����Э��ӿڹ�Լ֡ͷ
******************************************************************************/
const uint8 gucs_PST587Head[] =
{
    0x02, 
    PST_FRM_ANY,
};
const uint8 gucs_PSTEnd587[] =          //���岿���β
{
    0x0,
};


#define PST_FRM_587_LP       0x01        //���峤����ʼ��λ��
#define PST_FRM_587_LATR     (0)
#define PST_FRM_587_END      0x00        //�����β��
#define PST_FRM_587_CSSP     0x01        //�����ۼӺͼ�����ʼλ��
#define PST_FRM_587_ATR      (PST_ATR_CSCAL_SD | PST_ATR_END)
#define PST_FRM_587_MLEN     0x02        //��С֡�ĳ�����Ҫ����
#define PST_FRM_587_CSSL     0x83        //�����ۼӺͼ���ĳ���ƫ��
#define PST_FRM_587_CSOS     0x82        //�����ۼӺ͵�λ��ƫ��

/******************************************************************************
**R8C-ESAM��Լ֡�ṹ����Ϣ
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
**DLMS����Э��ӿڹ�Լ֡ͷ
******************************************************************************/
const uint8 gucs_PSTDLMS_HDLCHead[] =
{
    0x7E,
};


#define PST_FRM_DLMS_HDLC_LP       0x01        //���峤����ʼ��λ��
#define PST_FRM_DLMS_HDLC_LATR     (PST_LATR_DBLEN | PST_LATR_BFLEN) //
#define PST_FRM_DLMS_HDLC_LLFT     5           //����ʱ������
#define PST_FRM_DLMS_HDLC_LRFT     0           //����ʱ������
#define PST_FRM_DLMS_HDLC_END      0x7E        //�����β��
#define PST_FRM_DLMS_HDLC_CSSP     0x01        //�����ۼӺͼ�����ʼλ��
#define PST_FRM_DLMS_HDLC_ATR      (PST_ATR_CSCAL_CRC1)
#define PST_FRM_DLMS_HDLC_MLEN     0x02        //��С֡�ĳ�����Ҫ����
#define PST_FRM_DLMS_HDLC_CSSL     0x84        //�����ۼӺͼ���ĳ���ƫ��
#define PST_FRM_DLMS_HDLC_CSOS     0x83        //�����ۼӺ͵�λ��ƫ��

/******************************************************************************
**DLMS��Լ֡�ṹ����Ϣ
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
**DLMS����Э��ӿڹ�Լ֡ͷ
******************************************************************************/
const uint8 gucs_PSTDLMS_IPHead[] =
{
    0x00,0x01,
};


#define PST_FRM_DLMS_IP_LP       0x06        //���峤����ʼ��λ��
#define PST_FRM_DLMS_IP_LATR     (PST_LATR_DBLEN | PST_LATR_BFLEN) //
#define PST_FRM_DLMS_IP_LLFT     0           //����ʱ������
#define PST_FRM_DLMS_IP_LRFT     0           //����ʱ������
#define PST_FRM_DLMS_IP_END      0x7E        //�����β��
#define PST_FRM_DLMS_IP_CSSP     0x01        //�����ۼӺͼ�����ʼλ��
#define PST_FRM_DLMS_IP_ATR      (PST_ATR_PARIT | PST_ATR_END)
#define PST_FRM_DLMS_IP_MLEN     0x08        //��С֡�ĳ�����Ҫ����
#define PST_FRM_DLMS_IP_CSSL     0x84        //�����ۼӺͼ���ĳ���ƫ��
#define PST_FRM_DLMS_IP_CSOS     0x83        //�����ۼӺ͵�λ��ƫ��

/******************************************************************************
**DLMS��Լ֡�ṹ����Ϣ
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
**EDMIЭ��ӿڹ�Լ֡ͷ
******************************************************************************/
const uint8 gucs_PSTEDMIHead[] =
{
    0x02,
};

const uint8 gucs_PSTEndEDMI[] =          //���岿���β
{
    0x03,
};


#define PST_FRM_EDMI_LP       0x01        //���峤����ʼ��λ��
#define PST_FRM_EDMI_LATR     PST_LATR_NOLEN //��֡����
#define PST_FRM_EDMI_LLFT     0           //����ʱ������
#define PST_FRM_EDMI_LRFT     0           //����ʱ������
#define PST_FRM_EDMI_END      0x03        //�����β��
#define PST_FRM_EDMI_CSSP     0           //�����ۼӺͼ�����ʼλ��
#define PST_FRM_EDMI_ATR      (PST_ATR_CSCAL_CRC2)
#define PST_FRM_EDMI_MLEN     0x0C        //��С֡�ĳ�����Ҫ����
#define PST_FRM_EDMI_CSSL     0x83        //�����ۼӺͼ���ĳ���ƫ��
#define PST_FRM_EDMI_CSOS     0x83        //�����ۼӺ͵�λ��ƫ��

/******************************************************************************
**EDMI��Լ֡�ṹ����Ϣ
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


//dev_frm.c  �����Ͼ���Լ�ṹ��  zwj
#if PST_FRM_DM_EN > 0

/******************************************************************************
**�ṹ��
******************************************************************************/
const uint8 gucs_PSTDMHead[] =
{
	0x45,
	PST_FRM_ANY,
	PST_FRM_ANY,
	0x45,
};

const uint8 gucs_PSTEndTDM[] =          //���岿���β
{
    0x54,
};
#define PST_FRM_DM_LP       0x01        //���峤����ʼ��λ��
#define PST_FRM_DM_LATR     (0)
#define PST_FRM_DM_END      0x54        //�����β��
#define PST_FRM_DM_CSSP     0x04        //�����ۼӺͼ�����ʼλ��
#define PST_FRM_DM_ATR      (PST_ATR_CSCAL_SC)//�ۼӺ�
#define PST_FRM_DM_MLEN     0x06        //��С֡�ĳ�����Ҫ����
#define PST_FRM_DM_CSSL     0x86        //�����ۼӺͼ���ĳ���ƫ��
#define PST_FRM_DM_CSOS     0x82        //�����ۼӺ͵�λ��ƫ��

/******************************************************************************
**�ṹ��
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
**����1376.1��Լ֡��Ϣ
******************************************************************************/
#if PST_FRM_XML_EN > 0

const uint8 gucs_PSTXMLHead[] =          //��������Լ��ͷ"<? xml version='1.0' ?>"
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

const uint8 gucs_PSTEndXML[] =          //��������DLMS��β</h:rt>
{
    '<',
    '/',
    'h',
    ':',
    'r',
    't',
    '>',
};



#define PST_FRM_XML_LP       0x01        //���峤����ʼ��λ��
#define PST_FRM_XML_LATR     PST_LATR_NOLEN//(PST_LATR_DBLEN | PST_LATR_RELEN)
#define PST_FRM_XML_LLFR     0           //����ʱ������
#define PST_FRM_XML_LRFR     2           //����ʱ������
#define PST_FRM_XML_END      0x16        //�����β��
#define PST_FRM_XML_CSSP     0x06        //�����ۼӺͼ�����ʼλ��
#define PST_FRM_XML_ATR      (PST_ATR_CSCAL_SC)
#define PST_FRM_XML_MLEN     (68)        //��С֡�ĳ���
#define PST_FRM_XML_CSSL     0x88        //�����ۼӺͼ���ĳ���ƫ��
#define PST_FRM_XML_CSOS     0x82        //�����ۼӺ͵�λ��ƫ��


/******************************************************************************
**����1376.1��Լ֡�ṹ����Ϣ
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
**����֡�ṹ����
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
    uint16 ut_i,ut_j;                             //ѭ������
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
//	                if(buffer[ut_i] == str->end)
//	                {
//	                    len = ut_i + 1 - str->extlen;
//	                }
                for(ut_j = 0; ut_j < str->elen; ut_j++)
                {                                       //�����ƥ���ַ�ƥ����ô������һ��
                    if(buffer[ut_i+ut_j] == str->end[ut_j])
                    {
                        continue;
                    }
                                                        //�����ǰ��ƥ���ַ�Ϊ����ƥ���ַ������
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
                for(ut_j = 0; ut_j < str->elen; ut_j++)
                {                                       //�����ƥ���ַ�ƥ����ô������һ��
                    if(buffer[ut_i+ut_j] == str->end[ut_j])
                    {
                        continue;
                    }
                                                        //�����ǰ��ƥ���ַ�Ϊ����ƥ���ַ������
                    break;
                }
                if(ut_j == str->elen)
                {
                    len = ut_i + 1 - str->extlen;
                    SYS_OK();
                }

            }
            return PST_ERR_NE;              //���ز�����Ч֡  
        }
        else
        {                                   //�ж��Ƿ�Ϊ��β��
//	            if(buffer[str->extlen - 1 + len] != str->end)
//	            {
//	                return PST_ERR_NE;          //���ز�����Ч֡      
//              }
            for(ut_i = 0,ut_j = str->extlen - 1 + len; ut_i < str->elen; ut_j++,ut_i++)
            {                                       //�����ƥ���ַ�ƥ����ô������һ��
                if(buffer[ut_j] == str->end[ut_i])
                {
                    continue;
                }
                                                    //�����ǰ��ƥ���ַ�Ϊ����ƥ���ַ������
                break;
            }
            if(ut_i != str->elen)
            {
                return PST_ERR_NE;
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
uint8 PST_Channel(uint8_t channel, uint8* buffer, uint16 length, uint8* protocol)
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
                    //ת�����ݵ������ͷ��
                    memcpy(buffer, buffer + ut_j,length - ut_j);
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

            default:
                return 0;                   //���ش��ʧ��
        }
        len += str->extlen - 1;             //��������
    }

    if(!(str->atr & PST_ATR_END))           //�н�β��
    {
//	        buffer[len] = str->end;             //�ж��Ƿ�Ϊ��β��            
//	        len++;                              //��������
        memcpy(&buffer[len], str->end, str->elen);
        len += str->elen;
    }
    return len;                             //���س���
}

/******************************************************************************
**������Լ��������ɢת�б�
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
   
#if (PST_FRM_DM_EN > 0)   //zwj   �Ͼ�ģ��
    PST_DM_Decode,
#endif
#if (PST_FRM_XML_EN > 0)   //zwj   �Ͼ�ģ��
    PST_XML_Decode,
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
    
#if (PST_FRM_DM_EN > 0)    //zwj  �Ͼ�ģ��
    PST_DM_Proc,
#endif
#if (PST_FRM_XML_EN > 0)
    PST_XML_Proc,
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
            memcpy(buffer, buffer + uc_i, len - uc_i);
            return (len - uc_i);
        }
    }
    return 0;
}


/************************************************************************
 * @function: PST_GetFrmLen
 * @����: ���buffer��һ����Ч֡�ĳ���
 * 
 * @����: 
 * @param: nfrm ͨѶ�Ĺ�Լ���.���nfrmΪ0xff��ʾΪ�м�
 * @param: buffer ֡����
 * @param: len ֡����
 * 
 * @����: 
 * @return: uint16 ��֡�ĳ���(0��ʾû�кϸ�֡)
 * @˵��: 
 * @����: 
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint16 PST_GetFrmLen(uint8 nfrm, uint8* buffer)
{
    const PSTFrameStr* str;                 //����ָ��֡��Ϣ�ṹ��
    uint8 len;
    len = 0;
    
    if(nfrm >= PST_FRM_NO)                   //������֤
    {
        return 0;
    }
    
    str = (PSTFrameStr*)gss_PSTFrameStr[nfrm];
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
    
    len += str->extlen;     //֡�ĳ���
    return len;   
}

