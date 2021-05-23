/****************************************Copyright (c)*************************
**                               	________�Ƽ����޹�˾
**                                     		������
**
**
**--------------�ļ���Ϣ-------------------------------------------------------
**��   ��   ��: dev_frm.c
**��   ��   ��: yzy
**����޸�����: 2010��02��26��
**��        ��: ��Լ��������ͷ�ļ�
**ע        ��:
**--------------��ʷ�汾��Ϣ---------------------------------------------------
** ������: yzy
** ��  ��: v1.0
** �ա���: 2010��02��24��
** �衡��: ԭʼ�汾
******************************************************************************/
#ifndef _DEV_FRM_H_
#define _DEV_FRM_H_


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


//
typedef enum
{
    NETP_COMMON = 0,
    NETP_READ_TIME = 1,                 //ʱ��
    NETP_READ_CUR_FLOW,             //���˲ʱ����
    NETP_READ_WORKING_CUR_FLOW,      //������˲ʱ����
    
    NETP_READ_ACCUMULATE_TOTAL,     //����ۼ�����
    NETP_READ_WORKING_ACCUMULATE_TOTAL,     //�����ۼ�����

    NETP_READ_CUR_FLOW_PREPAY,             //Ԥ���ѱ��˲ʱ����
    NETP_READ_WORKING_CUR_FLOW_PREPAY,      //Ԥ���ѹ�����˲ʱ����
    
    NETP_READ_ACCUMULATE_TOTAL_PREPAY,     //Ԥ���ѱ���ۼ�����
    NETP_READ_WORKING_ACCUMULATE_TOTAL_PREPAY,     //Ԥ���ѹ����ۼ�����

    
    NETP_READ_TEMP,                 //�¶�
    NETP_READ_PRESSURE,             //ѹ��
    NETP_READ_WARN_FLAG,            //������λ��˵��
    NETP_READ_STT,                  //״̬��
    NETP_READ_REMAIND_GAS,          //ʣ����
    
    NETP_READ_CUR_PRICE,          //��ǰ����
    NETP_READ_ACCUMULATE_GAS,       //�ۻ�����
    NETP_READ_RECHARGE_GAS,         //��ǰ��ֵ��
    NETP_READ_RECHARGE_TOTAL_GAS,   //�ܳ�ֵ��
    NETP_READ_VOL_HADFLOW,                  //��ѹ״̬,�ٷֱȣ��Ƿ��к���
    NETP_READ_VALVE,                //����״̬
    NETP_READ_WXIC_METER_STT,       //����IC�����������״̬
    NETP_WRITE_WXIC_PREPAY,                  //IC������Ԥ����
    NETP_WRITE_WXIC_PREPAY_RET,                  //IC������Ԥ����
    NETP_WRITE_WXIC_RECHARGE,                  //IC��������ֵ
    NETP_WRITE_WXIC_RECHARGE_RET,                  //IC��������ֵ
    NETP_WRITE_WXIC_PRICE,                  //IC����������
    NETP_WRITE_WXIC_PRICE_RET,                  //IC����������
    
}EM_RD_ID;

//D--Data  Ty-Type  
//A-array  T-ʱ��  TM-�¶�
//uint8_t array ȫ0

#define NETP_D_Ty_A_N           1
//uint8_t array ȫ0xEE

#define NETP_D_Ty_A_E                   2
//uint8_t array ȫ0xFF

#define NETP_D_Ty_A_F                   3
//YYYY MM DD HH mm ss
//͸���ַ�
#define NETP_D_Ty_WXIC_PREPAY           4

#define NETP_D_Ty_MODBUS_REG_ADDR       5
#define NETP_D_Ty_STR                   6//�ַ���
#define NETP_D_STR_DATA_TO_BUF          7//�ַ���


#define NETP_D_Ty_T_BCD1        30
#define NETP_D_BIGEND_TO_LITTLE_6B        31//�ߵ�λת����4�ֽ�������2�ֽ�С��
#define NETP_D_BIG_TO_LIT_4B_DECI2        32//�ߵ�λת��,4�ֽڣ�2λС��
#define NETP_D_BIG_TO_LIT_2B_DECI1        33//�ߵ�λת��,2�ֽڣ�1λС��
#define NETP_D_BIG_TO_LIT_4B_DECI1        34//�ߵ�λת��,4�ֽڣ�1λС��

#define NETP_D_BIG_TO_LIT_8B_INT6B_DECI2B_DIV   35//�ߵ�λת�����ܳ�8�ֽڣ�6�ֽ�hex������2�ֽ�С�����㷨����0x10000��
#define NETP_D_BIG_TO_LIT_4B_INT3B_DECI1B_DIV   36//�ߵ�λת��,4�ֽڣ�3�ֽ�hex������2λС�����㷨����0x100��

#define NETP_D_BIG_TO_LIT_4B_LONG_REV2       37//�ߵ�λת��,4�ֽ�hex����������λ��ǰ������λ�ں�
#define NETP_D_BIG_TO_LIT_4B_FLOAT_REV2       38//�ߵ�λת��,4�ֽڸ�����������λ��ǰ������λ�ں�

#define NETP_D_BIG_TO_LIT_6B_INT4B_REV2_DECI2B_INVLID1B       39//�ߵ�λת��,4�ֽ�hex����������λ��ǰ������λ�ں�,2�ֽ�С��λ��ֻ����2λС��

#define NETP_D_ASCII10B_TO_HEX5B        40//asciiתhex ����
#define NETP_D_ASCII8B_TO_HEX4B_DECI2        41//asciiת4�ֽ�hex ������С��λ2λ
#define NETP_D_ASCII6B_TO_HEX3B_DECI1        42//asciiת3�ֽ�hex ������С��λ1λ
#define NETP_D_ASCII6B_TO_HEX3B_DECI2        43//asciiת3�ֽ�hex ������С��λ2λ
#define NETP_D_ASCII6B_TO_HEX3B              44//asciiת3�ֽ�hex ����

#define NETP_D_ASCII5B_TO_HEX2B_DECI1_SYMBOL        45//asciiת2�ֽ�hex ������С��λ1λ,����λ1λ
#define NETP_D_BIG_TO_LIT_4B_LONG4B        46//�ߵ�λת��,4�ֽ�����
#define NETP_D_BIG_TO_LIT_8B_LONG4B_DECI4B        47//�ߵ�λת��,4�ֽ�hex���Σ�4�ֽ�hex����С��

#define NETP_D_BIG_TO_LIT_8B_DOUBLE       48//�ߵ�λת��,8�ֽ�˫���ȸ�����
#define NETP_D_BIG_TO_LIT_4B_FLOAT       49//�ߵ�λת��,4�ֽڸ�����
#define NETP_D_BIG_TO_LIT_4B_DECI3        50//�ߵ�λת��,4�ֽڣ�3λС��
#define NETP_D_BIG_TO_LIT_2B_DECI2        51//�ߵ�λת��,2�ֽڣ�2λС��

//�ߵ�λת��,2������������һ��������Ϊ��5λF1���ڶ���������Ϊ����λ��С��F2�������F1*100000 + F2
#define NETP_D_BIG_TO_LIT_8B_FLOAT2_HIGH5_LOW3_DECI       52

#define NETP_D_MSB_BCD6B_DECI3        53//��λ��ǰ��MSB First��,6�ֽڣ�3λС��
#define NETP_D_LSB_4B_FLOAT       54//��λ��ǰ��MSB First��,4�ֽڸ�����
//��λ��ǰ��MSB First��,��һ��4�ֽڸ�����F1���ڶ���4�ֽڸ�����F2 �����F1*0x10000000+F2
#define NETP_D_LSB_8B_FLOAT4B_H7_FLOAT4B_LOW       55
#define NETP_D_MSB_8B_LONG4B_H7_LONG4B_LOW       56


#define NETP_D_Ty_TM_FLOAT      60

#define NETP_D_Ty_P_FLOAT      NETP_D_Ty_TM_FLOAT


//���˲ʱ���� TX13     IST -instant Standard conditions flow
//����ۼ�����     TX13 AT accumulative total
#define NETP_D_Ty_FLOAT     100
#define NETP_D_WXIC_LONG     101

#define NETP_D_Ty_DOUBLE      120
//WS-warn status
#define NETP_D_Ty_HEX        140

#define NETP_D_Ty_WS_HEX        140
//S  ter status
#define NETP_D_Ty_S_HEX        NETP_D_Ty_WS_HEX
#define NETP_D_Ty_WXIC_PREPAY_HEX        NETP_D_Ty_WS_HEX

#define NETP_D_B_DEV_ADDR       1



#define CON_MAX_DEVICE_TYPES  64   // ֧�ֵ��豸���͵����� 

#define MAX_NETP_CP_FRAME_NUM  8

#define MAX_NETP_CP_ITEMS   4  //�������������
#define MAX_NETP_CP_RET_ITEMS   16//���������������

#define MAX_NETP_EFT_BITS 2

#define NETP_FRM_CMDLP_1     0x01 //�����������֡ͷ��λ��Ϊ1

#define NETP_FRM_DECOMP_DIATR_NULL    (0)//��λ��DI��֡ͷλ��,��֡��DI��

#define NETP_FRM_ADLEN_1     0x01       //��ַ�򳤶�Ϊ1
#define NETP_FRM_ADLP_0      0x00//��ַ�������֡ͷ��λ��,���и�λ��1��ʾ ��ַ�򳤶�Ҫ�ۼ� 0��ʾ����Ҫ�ۼ�



#define NETP_FRM_COMP_LP_0       0x00        //���峤����ʼ��λ��Ϊ0

#define NETP_FRM_COMP_LATR_NOLEN     (PST_LATR_NOLEN)//

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
    uint8  end;                         //��β��־����
    uint8  cssp;                        //��У�����������֡ͷ��λ��
    uint8  extlen;                      //���ݳ���Ϊ0ʱ��֡����
    uint8  atr;                         //����
    uint8  cssl;                        //��У�����ݳ���,�����(�����򳤶�+extlen)����(bit7:0��1��)
    uint8  csco;                        //У��Ͳ���,�����(�����򳤶�+extlen)����(bit7:0��1��)
}PSTFrameStr;                           //֡�ṹ����
#pragma pack(1)


typedef struct
{
    uint8_t id;
    uint8_t len;
    uint8_t atr;
    uint8_t mult;//͸����ʱ��mult�����Ǳ����
}NetpDataItem;
//	typedef union{
//	    uint8_t str[4];
//	    struct{
//	        uint8_t id;
//	        uint8_t len;
//	        uint8_t atr;
//	        uint8_t b;
//	    }bit;
//	}NetpDataItem;

typedef struct
{
    uint8_t cmd;
    uint8_t rlen;
    uint8_t scmd[4];//second cmd
    uint8_t dilen;
    uint8_t inum;
    uint8_t rinum;   
    NetpDataItem item[MAX_NETP_CP_ITEMS];
    NetpDataItem retItem[MAX_NETP_CP_RET_ITEMS];
}NetpFrameDataStr;


/***********************************************************************
**����֡�ṹ�Ľṹ��
**********************************************************************/
typedef struct
{   
    uint8_t  head[12];                        //֡ͷ
    uint8_t  hlen;                        //֡ͷ�ĳ���
    uint8_t  lp;                          //���ݳ��������֡ͷ��λ��
    uint8_t  dlp;                         //�����������֡ͷ��λ��
    uint8_t  latr;                        //���ݳ��ȵ�����
    uint8_t  llft : 4;                    //���ݳ��Ƚ���ʱ������,���ʱ������
    uint8_t  lrft : 4;                    //���ݳ��Ƚ���ʱ������,���ʱ������
    uint8_t  end;                         //��β��־����
    uint8_t  cssp;                        //��У�����������֡ͷ��λ��
    uint8_t  extlen;                      //���ݳ���Ϊ0ʱ��֡����
    uint8_t  atr;                         //����
    uint8_t  cssl;                        //��У�����ݳ���,�����(�����򳤶�+extlen)����(bit7:0��1��)
    uint8_t  csco;                        //У��Ͳ���,�����(�����򳤶�+extlen)����(bit7:0��1��)
    uint8_t  cmdlen;                      //�����볤��
    uint8_t  cmdlp;                       //�����������֡ͷ��λ��
    uint8_t  DIatr;                       //DI����
    uint8_t  alen;                        //��ַ�򳤶�
    uint8_t  adlp;                        //��ַ�������֡ͷ��λ��
}NetpFrameStr;                           //֡�ṹ����

typedef struct
{   
    uint8_t  head[12];                    //֡ͷ
    uint8_t  hlen;                        //֡ͷ�ĳ���
    uint8_t  lp;                          //���ݳ��������֡ͷ��λ��
    uint8_t  latr;                        //���ݳ��ȵ�����
    uint8_t  llft : 4;                    //���ݳ��Ƚ���ʱ������,���ʱ������
    uint8_t  lrft : 4;                    //���ݳ��Ƚ���ʱ������,���ʱ������
    uint8_t  end;                         //��β��־����
    uint8_t  cssp;                        //��У�����������֡ͷ��λ��
    uint8_t  atr;                         //����
    uint8_t  cssl;                        //��У�����ݳ���,�����(�����򳤶�+extlen)����(bit7:0��1��)
    uint8_t  alen;                        //��ַ�򳤶�
    uint8_t  adlp;                        //��ַ�������֡ͷ��λ��
    uint8_t  cmdlen;                      //�����볤��
    uint8_t  cmdlp;                       //�����������֡ͷ��λ��
    uint8_t  DIatr;                       //DI����(������������)
    uint16_t frm_bit;                     //֡�ṹ����
    uint16_t len_bit;                     //֡�ṹ����
}NetpComposeFrameStr;                           //֡�ṹ����


typedef struct _ST_Netp_REG
{
    uint8_t pf;                           //�������Լ
    uint8_t device_name[4];


}ST_Netp_Reg,*PST_Netp_Reg;


typedef struct _ST_Netp_FW
{
    uint8_t pf;                           //�������Լ
    uint8_t device_name[4];
    uint8_t bs;                           //ͨ�ſ�����
    uint8_t datanum;
    NetpFrameStr frameStr;
    NetpComposeFrameStr compFrameStr;
    NetpFrameDataStr frameDtaStr[MAX_NETP_CP_FRAME_NUM];
//	    uint8_t plen[16];//Ҫ�ϸ������������
//	    uint16_t eft;//�ϸ�ʱҪ��������
    
//	    uint16 crc;
}ST_Netp_FW,*PST_Netp_FW;


typedef struct _ST_NETP_FW_BASE
{
    uint8_t pf;                           //�������Լ
    uint8_t device_name[4];
    uint8_t bs;                           //ͨ�ſ�����
    uint8_t datanum;
    NetpFrameStr frameStr;
    NetpComposeFrameStr compFrameStr;
    NetpFrameDataStr frameDtaStr[MAX_NETP_CP_FRAME_NUM];
//	    uint8_t plen[16];//Ҫ�ϸ������������
//	    uint16_t eft;//�ϸ�ʱҪ��������
    NetpFrameDataStr PrepayWParaStr;//Ԥ���Ѳ���д
    NetpFrameDataStr PrepayRParaStr;//Ԥ���Ѳ�����
    NetpFrameDataStr PriceWStr;     //����
    NetpFrameDataStr PriceRStr;     //������
    NetpFrameDataStr RechargeStr;   //��ֵ
    NetpFrameDataStr ValveStr;      //����

    uint16 crc;
}ST_NETP_FW_BASE,*PST_NETP_FW_BASE;


#pragma pack(4)

#define PST_LATR_NOLEN      (uint8)0x80 //��λ��ʾû��֡����
#define PST_LATR_DBLEN      (uint8)0x40 //��λ��ʾ��˫�ֽڳ���
#define PST_LATR_RELEN      (uint8)0x20 //��λ��ʾ�������ظ�һ��
#define PST_LATR_BFLEN      (uint8)0x04 //���ֽ���ǰ

#define PST_DIATR_NONE      (uint8)0x00 //û��DI
#define PST_DIATR_DB_MB        (uint8)0x80 //modbus�Ĵ�����ַ

#define PST_DIATR_DBLP      (uint8)0x02 //DI����

#define PST_ADLP_ADDLEN      (uint8)0x80 //��λ��ʾ��ַ�򳤶�Ҫ�ۼ�
#define PST_ADLP_ADD_TO_ASCII      (uint8)0x40 //��λ��ʾ��ַ��ת����ASCII
#define PST_ADLP_ADDLEN_MASK      (uint8)0x3F //

#define PST_ATR_PARIT       (uint8)0x80 //��λ��ʾû��У��λ
#define PST_ATR_END         (uint8)0x40 //��λ��ʾû�н�β��
#define PST_ATR_CSCAL       (uint8)0x0F //У��͵��㷨

#define PST_ATR_CSCAL_SC    (uint8)0    //�ۼӺ�
#define PST_ATR_CSCAL_BS    (uint8)1    //λУ��
#define PST_ATR_CSCAL_SD    (uint8)2    //˫�ֽ��ۼӺ�
#define PST_ATR_CSCAL_CRC1  (uint8)3    //DLMS_CRC16
#define PST_ATR_CSCAL_CRC2  (uint8)4    //EDMI_CRC16
#define PST_ATR_CSCAL_SD1   (uint8)5    //˫�ֽ��ۼӺ�,��λΪ0
#define PST_ATR_CSCAL_CRC3  (uint8)6    //RtuModbus
#define PST_ATR_CSCAL_CRC4  (uint8)7    //RtuModbus ��λ��ǰ
#define PST_ATR_CSCAL_BS_ASCII    (uint8)8    //λУ��,ASCIIתbin

#define PST_FRM_BIT_HEAD      (uint16_t)0x0001 //��λ��ʾ��֡ͷ
#define PST_FRM_BIT_ADDR      (uint16_t)0x0002 //��λ��ʾ�е�ַ��
#define PST_FRM_BIT_LEN       (uint16_t)0x0004 //��λ��ʾ�г�����
#define PST_FRM_BIT_CMD       (uint16_t)0x0008 //��λ��ʾ�й�������
#define PST_FRM_BIT_DI        (uint16_t)0x0010 //��λ��ʾ��DI��

#define PST_FRM_BIT_DATA      (uint16_t)0x0020 //��λ��ʾ��������

#define PST_FRM_BIT_CS        (uint16_t)0x0040 //��λ��ʾ��У�����
#define PST_FRM_BIT_TAIL      (uint16_t)0x0080 //��λ��ʾ�н�������

#define NETP_FRM_CSSL_SUB_2     0x82        //�����ۼӺͼ���ĳ���ƫ��
#define NETP_FRM_CSSL_SUB_3     0x83        //�����ۼӺͼ���ĳ���ƫ��
#define NETP_FRM_CSSL_SUB_4     0x84        //�����ۼӺͼ���ĳ���ƫ��
#define NETP_FRM_CSSL_SUB_5     0x85        //�����ۼӺͼ���ĳ���ƫ��

#define NETP_FRM_CSOS_SUB_3     0x83        //�����ۼӺ͵�λ��ƫ��
#define NETP_FRM_CSOS_SUB_2     0x82        //�����ۼӺ͵�λ��ƫ��
#define NETP_FRM_CSOS_SUB_5     0x85        //�����ۼӺ͵�λ��ƫ��


#define NETP_FRM_LP_02       0x02        //���峤����ʼ��λ��
#define NETP_FRM_DLP_02       0x02        //������������ʼ��λ��
#define NETP_FRM_DLP_03       0x03        //������������ʼ��λ��

#define NETP_FRM_WITHOUT_END      00        //�����β��

//���������֡ͷ����ַ����������������DI��У����
#define NETP_FRM_COMP_BIT_H_A_C_D_I_C (uint16_t)(PST_FRM_BIT_HEAD|PST_FRM_BIT_ADDR \
                                     | PST_FRM_BIT_CMD | PST_FRM_BIT_DATA \
                                     | PST_FRM_BIT_DI | PST_FRM_BIT_CS)
//�����򣬿�
#define NETP_FRM_COMP_LEN_BIT_NULL (uint16_t)(0)

#define NETP_FRM_CSSP_FROM_0     0x00        //�����ۼӺͼ�����ʼλ��


#define NETP_FRM_MIN_LEN_4     4        //֡��С����Ϊ4

#define NETP_FRM_ATR_MODBUSCRC_NO_END      (PST_ATR_CSCAL_CRC3|PST_ATR_END)

#define NETP_FRM_COMP_DIATR_HAS_02    (PST_DIATR_DB_MB | 0x02)//��DI��DI��֡ͷλ��Ϊ2

#define EFT_MIN_ST_BIT  0x0001//�������
#define EFT_MIN_WT_BIT 0x0002//��������
#define EFT_MIN_SI_BIT  0x0004//���˲ʱ
#define EFT_MIN_WI_BIT 0x0008//����˲ʱ
#define EFT_MIN_TMP_BIT   0x0010//�¶�
#define EFT_MIN_P_BIT  0x0020//ѹ��



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
}NETP_Frame;


typedef struct
{
    uint8 cmd;                          //������(0x1EΪdiscover_reply)
    uint8* recv;                        //����֡������
    uint8* send;                        //����֡������
    uint16 len;                         //����
    uint16 adr;                         //�߼���ַ
}DLMS_Frame;



/***********************************************************************
**����ͨ��֡����ṹ��
**********************************************************************/
typedef struct
{
    uint8 chninfo;
    union
    {
#if (_DLT_FRM_INC > 0 || _VS_FRM_INC > 0)
        DLT_Frame _dlt;
#endif
#if (_DLT_FRM_INC > 0 || _VS_FRM_INC > 0)
        ZY_Frame _zy;
#endif
#if _PLC_FRM_INC > 0 
        PLC_Frame _plc;
#endif
#if _DLMS_FRM_INC > 0 
        DLMS_Frame _dlms;
#endif
#if _NETP_FRM_INC > 0
        NETP_Frame _netp;
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


uint8 PST_CheckFrame3(uint8 nfrm, uint8* buffer, uint16* len);

/************************************************************************
 * @function: NETP_CheckFrame2
 * @����: ָ������֡��Ч�Եļ��(������ǰ����,��ȥ��ǰ����./��ȥ��֡�����Ч�ַ�)
 * 
 * @����: 
 * @param: str ��Լ�����ṹ��
 * @param: buffer ֡����
 * @param: len ֡����
 * 
 * @����: 
 * @return:0�ɹ�  ����ʧ��
 * @˵��: 
 * @����: yzy (2012-5-15)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/

uint8 NETP_CheckFrame2(const NetpFrameStr* str, uint8* buffer, uint16* len);
/*********************************************************************************************************
**��������  int NETP_DoneFrame(const NetpComposeFrameStr * frmstr, const NetpFrameDataStr * frameDtaStr, uint8_t * buf, uint8* addr, uint16* length)
**����
        frmstr ����Ľṹ��
        frameDtaStr ������������
        buf ���ݻ���
        addr ����ĵ�ַ            
        length ����Ϊ����������󳤶ȣ����Ϊ�����֮��ĳ���
**���  PST_ERR_OK
        PST_ERR_VAR
**      PST_ERR_FT
**˵��  �Գ���֡֡���д��������ͨѶ
** ȫ�ֱ���:
** ����ģ��:
**
**------------------------------------------------------------------------------------------------------
** ��  ��: yzy
** �ա���: 2019��6��18��
** ��  ��: 
********************************************************************************************************/

//int NETP_DoneFrame(const NetpComposeFrameStr * frmstr, const NetpFrameDataStr * frameDtaStr, uint8_t * buf, uint8* addr, uint16* length);
int NETP_DoneFrame( const NetpComposeFrameStr * frmstr, 
                    const NetpFrameDataStr * frameDtaStr, 
                            uint8_t * buf, uint8_t* addr, uint8_t *data, uint16_t* length);

uint16_t Netp_Get_FW_num(void);


#endif
