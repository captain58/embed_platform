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
#ifndef _DEV_FRM_H_
#define _DEV_FRM_H_

#include "public.h"

#define PST_ERR_OK      (uint8)0            //û������
#define PST_ERR_VAR     (uint8)1            //��������
#define PST_ERR_FT      (uint8)2            //��������
#define PST_ERR_NT      (uint8)4            //δ֪����
#define PST_ERR_LV      (uint8)5            //Ȩ�޲���
#define PST_ERR_RE      (uint8)9            //����ظ�
#define PST_ERR_NE      (uint8)10           //������Ч֡
#define PST_ERR_TS      (uint8)11           //����̫��
#define PST_ERR_DI      (uint8)12           //��������ݱ�ʶ


#define PST_FRM_ANY     (uint8)0xFF         //����ƥ�����

#define FRM_HEADCHAR    5                 //��Ч֡ɨ��ǰ���ַ���









/***********************************************************************
**����֡�ṹ�Ľṹ��
**********************************************************************/
typedef struct
{   
    uint8* head;                        //֡ͷ
    uint8  hlen;                        //֡ͷ�ĳ���
    uint8  lp;                          //���ݳ��������֡ͷ��λ��
    uint8  latr;                        //���ݳ��ȵ�����
    uint8  llft : 4;                    //���ݳ��Ƚ���ʱ������,���ʱ������
    uint8  lrft : 4;                    //���ݳ��Ƚ���ʱ������,���ʱ������
    uint8* end;                         //��β��־����
    uint8  elen;                        //��β�ĳ���
    uint8  cssp;                        //��У�����������֡ͷ��λ��
    uint8  extlen;                      //���ݳ���Ϊ0ʱ��֡����
    uint8  atr;                         //����
    uint8  cssl;                        //��У�����ݳ���,�����(�����򳤶�+extlen)����(bit7:0��1��)
    uint8  csco;                        //У��Ͳ���,�����(�����򳤶�+extlen)����(bit7:0��1��)
}PSTFrameStr;                           //֡�ṹ����



#define PST_LATR_NOLEN      (uint8)0x80 //��λ��ʾû��֡����
#define PST_LATR_DBLEN      (uint8)0x40 //��λ��ʾ��˫�ֽڳ���
#define PST_LATR_RELEN      (uint8)0x20 //��λ��ʾ�������ظ�һ��
#define PST_LATR_BFLEN      (uint8)0x04 //���ֽ���ǰ

#define PST_ATR_PARIT       (uint8)0x80 //��λ��ʾû��У��λ
#define PST_ATR_END         (uint8)0x40 //��λ��ʾû�н�β��
#define PST_ATR_CSCAL       (uint8)0x07 //У��͵��㷨

#define PST_ATR_CSCAL_SC    (uint8)0    //�ۼӺ�
#define PST_ATR_CSCAL_BS    (uint8)1    //λУ��
#define PST_ATR_CSCAL_SD    (uint8)2    //˫�ֽ��ۼӺ�
#define PST_ATR_CSCAL_CRC1  (uint8)3    //DLMS_CRC16
#define PST_ATR_CSCAL_CRC2  (uint8)4    //EDMI_CRC16



/*******************************************************************************
ͨ����Ŷ���
*******************************************************************************/
typedef enum
{
    PST_CHN_SET = 0,             //����485
    PST_CHN_WL,                 //����
    PST_CHN_DUMB,                //��ͨ��
}CHN_ENUM;


/***********************************************************************
**����ÿ��ͨ����֧�ֵ�֡�����������Լ������ǰ����
**********************************************************************/
typedef struct
{
    uint8* frameno;                     //֡������
    uint8  count;                       //֡������
    uint8  headchars;                   //֡��ǰ����������
}PSTChnInfo;

#ifdef PST_FRM_SIMPLE

/***********************************************************************
**����ͨ��֡����ṹ��
**********************************************************************/
typedef struct
{
    uint8 chninfo;
    uint8 tstno;                        //�������
    uint8 cmd;                          //������
    uint8 power;                        //Ȩ�޵ȼ�
    uint8* recv;                        //����֡������
    uint8* send;                        //����֡������
    uint16 len;                         //����
    uint8 atr;                          //����
    uint8 tbd;                          //����չ
    Word32 wd;                          //���ݱ�־,������
    
}PST_Frame;

#else

/************************************************************************ 
**֡��Ϣ�ṹ��
*************************************************************************/
typedef struct
{
    uint16 len;                         //Ӧ�����ݳ���
    uint8  atr;                         //����
    uint8  cmd;                         //������
    uint8* R;                           //R��
    uint8* src_addr;                    //Դ��ַ
    uint8* dest_addr;                   //Ŀ���ַ
    uint8  afn;                         //AFN       (Ӧ��������1)
    uint8  fn;                          //FN        (Ӧ��������2)
    uint8* app;                         //Ӧ������  (Ӧ��������3)
    uint8* send;                        //���ͻ���
    Word32 wd;                          //0:�������,1:ʵ�����
}PLC_Frame;


typedef struct
{
    uint8 tstno;                        //�������
    uint8 cmd;                          //������
    uint16 sbuflen;                     //���ͻ��泤��
    uint8* send;                        //���ͻ���
    uint8* recv;                        //����֡������
    uint16 len;                         //����
    uint8 atr;                          //����
    uint8 tbd;                          //����չ
    Word32 wd;                          //���ݱ�־,������
}DLT_Frame;


typedef struct
{
    uint8 cmd;                          //������(0x1EΪdiscover_reply)
    uint8* recv;                        //����֡������
    uint8* send;                        //����֡������
    uint16 len;                         //����
    uint16 adr;                         //�߼���ַ
}DLMS_Frame;

//zwj  �Ͼ�DM֡�ṹ��
typedef struct
{
	uint8 cmd;                          //������(0x1EΪdiscover_reply)
	uint8* recv;                        //����֡������
	uint8* send;                        //����֡������
	uint8 len;                         //����
	uint16 adr;                         //�߼���ַ
}DM_Frame;


typedef struct
{
//	    uint8 tstno;                        //�������
    uint8 cmd;                          //������
    uint16 sbuflen;                     //���ͻ��泤��
    uint8* send;                        //���ͻ���
    uint8* recv;                        //����֡������
    uint16 len;                         //����
    uint8 atr;                          //����
    uint8 tbd;                          //����չ
    uint8 * time;
    uint8 seq;
    Word32 wd;                          //���ݱ�־,������
}ZY_Frame;



typedef struct _S_TerAddr{
    uint8 ucMeterAddLen;
    uint8 ucMeterAdd[16];
}S_TerAddr;

typedef struct
{
    uint8 ret;
    uint8 version;                        //XMLЭ�鹫˾�ڲ��汾��
    S_TerAddr saddr;                          //���Ͷ˵ĵ�ַ
    uint8 type;                     //�����������ݴ���Э��
    uint8 cmd;                        //������
    uint8 seq;                        //XML֡���
    uint8 encrypt;                         //���ݼ����㷨
    uint8 check;                          //У���㷨
    uint8 random[12];                          //�����
    uint32 verifyData[8];
    uint16 recvLen;                     //����������
    uint8* recv;                        //����֡������
    uint8* send;                        //����֡������,����1024���ֽ�
}XML_Frame;
typedef struct
{
    
	STAPDU apdu;
    double rssi;
    uint16 id;
    uint8 bNeedReAllocate;
    uint8 bDebugFlg;
    uint8* recv;                        //����֡������
    uint8* send;                        //����֡������
    uint16 len;                         //����
}SRF_Frame;
typedef struct
{
    uint16 len;                         //Ӧ�����ݳ���
    uint8  FrameType;                   //����������
    uint8  FrameID;                     //������
    uint16 cmd;                         //������
    uint8* recv;                        //Ӧ������ 
    uint8* send;                        //���ͻ���
}RF_Frame;

/***********************************************************************
**����ͨ��֡����ṹ��
**********************************************************************/
typedef struct
{
    uint8 chninfo;
    uint8 protocol;
    union
    {
#if (_DLT_FRM_INC > 0 || _VS_FRM_INC > 0)
        ZY_Frame _zy;
#endif      
#if _DLT_FRM_INC > 0 
         DLT_Frame _dlt;
#endif
#if _PLC_FRM_INC > 0 
         PLC_Frame _plc;
#endif
#if _DLMS_FRM_INC > 0 
         DLMS_Frame _dlms;
#endif
#if _DM_FRM_INC > 0                    //zwj
         DM_Frame _dm;
#endif
#if PST_FRM_XML_EN > 0 
         XML_Frame _xml;
#endif
#if (_WL1_FRM_INC > 0)
        SRF_Frame _srf;
#endif 
    };
  
}PST_Frame;

#endif

#define PST_FRM_CHNNO   0x3F            //ͨ���������
                                        // 
#define PST_FRM_POWLV   0x0F            //Ȩ�޵ȼ�����
                                        // 
#define PST_FRM_ATRWR   0x01            //��д����
#define PST_FRM_ATRADR  0x06            //��ַ����
#define PST_FRM_ATRADE  0x08            //��ַ�Ƿ���ȷ

#define PST_FRM_ADROK   0x00            //��ͨ��ַ
#define PST_FRM_ADRBR   0x02            //�㲥��ַ
#define PST_FRM_ADRFL   0x04            //ȫ�ܵ�ַ




/***********************************************************************
**����ͨѶ�����
**********************************************************************/
//typedef struct
//{
//    uint16 tid;                         //��Լ���ݱ�ʶ
//    uint16 idatr;                       //��ʶ���� ͬ��������
//    uint16 cid;                         //�ں����ݱ�ʶ 0xFFFF��ʾ���⴦��
//    uint16 ftatr;                       //��ʽ����
//    void* read;                         //������
//    void* write;                        //д����
//}PST_TableStr;
/***********************************************************************
**����ͨѶ�����
**********************************************************************/
typedef struct
{
    uint16 tid;                         //��Լ���ݱ�ʶ
    uint16 idatr;                       //��ʶ���� ͬ��������
    uint16 cid;                         //�ں����ݱ�ʶ 0xFFFF��ʾ���⴦��
    uint16 ftatr;                       //��ʽ����
    void* read;                         //������
    void* write;                        //д����
    
    uint16  len;                        //��������(FLASH������ܳ���)
    uint32  addr;                       //�ն˲�����ַ(flash)
    
    uint16_t type;                      //
    void*   load;                       //���뺯��(����:uint8 (*func)(), __NULL��������)
    const uint8* a_def;                 //Ĭ�ϲ�����ַ(rom, 0��ʾĬ��ȫΪ��)
    void*   f_def;                      //Ĭ�ϲ������⴦��

}PST_TableStr;

typedef uint8 (*PSTTblPrc)(const PST_TableStr*, PST_Frame*);

#define PST_TBL_SMLR1   0x8000          //��һ��ͬ����
#define PST_TBL_SMLR2   0x4000          //�ڶ���ͬ����
#define PST_TBL_SMLRN   0x0FFF          //ͬ���������


/********************************************************** 
//������ԭ��
**********************************************************/
typedef struct _RF_CMD_TABLE_ {
	uint16          cmd; 		//!< ������
	void*			func;	//!< ��������
} CMD_TABLE_t;
	
typedef uint8 (*FW_COMMAD_t)(const CMD_TABLE_t* tbl, SRF_Frame* frm);
typedef uint8 (*FW_COMMAD_CONF_t)(const CMD_TABLE_t* tbl, RF_Frame* frm);

/************************************************************************
 * @function: PST_CheckFrame
 * @����: 
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
uint8 PST_CheckFrame(uint8 num, uint8* buffer, uint16 length);

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
uint8 PST_Channel(uint8_t channel, uint8* buffer, uint16 length, uint8* protocol);


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
uint16 PST_DoneFrame(uint8 num, uint8* buffer);

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
uint8 FRM_Decode(uint8 protocal, uint8* rbuffer, uint8* sbuffer, uint16 sbuflen, PST_Frame* frm);



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
uint8 FRM_Proc(uint8 protocal, PST_Frame* frm);


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
uint16 PST_CheckFrame2(uint8 nfrm, uint8* buffer, uint16 len);


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
uint16 PST_GetFrmLen(uint8 nfrm, uint8* buffer);


#endif
