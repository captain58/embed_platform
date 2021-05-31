/****************************************Copyright (c)**************************************************
**                                  ________�Ƽ����޹�˾
**                                          ������
**
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: pst_zy.h
**��   ��   ��: yzy
**����޸�����: 2019��6��18��
**��        ��: ͨѶģ��____��Լ�����ļ���������ͨѶ�ͱ���ͨѶ
**ע        ��:
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: yzy
** ��  ��: v1.0
** �ա���: 2019��6��18��
** �衡��: ԭʼ�汾
********************************************************************************************************/
#ifndef _PST_ZY_H_
#define _PST_ZY_H_


#ifndef EXT_ZY
#define EXT_ZY  extern
#else
#define EXT_ZY_SELF
#endif


#define ERR_OK                      0x00    //����
#define ERR_HARDFAULT_WRITE         0x01    //дӲ������
#define ERR_HARDFAULT_READ          0x02    //��Ӳ������
#define ERR_NO_RECORDE              0x03    //����ؼ�¼
#define ERR_NOT_SUPPORT             0x04    //�޴˹���

#define ERR_INVALID_PARA            0x06    //������Ч
#define ERR_CHARGE_INDEX            0x07    //��ֵ��������
#define ERR_MAXREMAIN               0x08    //����������
#define ERR_MAXCHARGE               0x09    //�ﵽ��ֵ����

#define ERR_DATATYPE                0x0d    //�����������ʹ���
#define ERR_DATA                    0x0e    //���ݴ���

#define ERR_OUT_OF_LIMIT            0x12    //��������������
#define ERR_RTC                     0x13    //RTC�豸����
#define ERR_CANNOT_CTL_DOOR         0x14    //Զ�̽�ֹ���ſ���
#define ERR_INVALID_ADDR            0x15    //��ַ������Ч��Χ
#define ERR_CHARGEINDEX_LO          0x16    //��ֵ����С
#define ERR_CHARGEINDEX_HI          0x17    //��ֵ������
#define ERR_WRITE_ID                0x18    //д��Ŵ���
#define ERR_CLOCK                   0x19    //д���ʱ�Ӵ���
#define ERR_CANNOT_WRITE_VER        0x1a    //��֧��д��߰汾��
#define ERR_DEVICE_NUMS             0x1b    //�����豸��Ŀ����
#define ERR_DONT_HAD_DEL_ADDR       0x1c    //Ҫɾ�����豸��ַ������
#define ERR_DONT_HAD_ADDR           0x1d    //���ŵ�ַ������
#define ERR_HAD_ADDR                0x1e    //���Ӱ�װ�豸��ַ�Ѵ���
#define ERR_ADDR                    0x1f    //�豸��ַ����
#define ERR_NO_RESPOND              0x20    //���ſ������޻�Ӧ
#define	ERR_FILEINVALID             0x21	//�����ļ���Ч	                       
#define	ERR_ALREADYUPGRADE          0x22	//��Ϊ���³���	                       
#define	ERR_MPRICEINVALID           0x23	//������Ϣ��Ч	                       
#define	ERR_MPRICEINDEX             0x24	//������Ŵ���	                       
#define	ERR_ALREADYMPRICE           0x25	//�ѵ��ۣ����·��۸����ͬ��	         
#define	ERR_OPERATE                 0x26	//����ʧ��	                           


#define LEN_SMS_FRM   110
#define LEN_GPRS_FRM  (LEN_OF_IPSEND - 30)     //
#define LEN_485_FRM   240


/************************************************************************ 
**�������̷��ͻ��� 
**������ջ��,�ɵ���ջ���,��AFN0C_F137
*************************************************************************/
EXT_ZY uint8 gucs_PstSbuf[LEN_GPRS_FRM];


/************************************************************************
 * ���Ź����������ϱ���Ϣ�ṹ��
 ************************************************************************/
typedef struct _TWdtTstUp
{
    uint16 key;                //�ϱ���λԿ��(0xBBBB��ʾ�ϵ�ִ�������ϱ�)
    uint16 crc;                //�ϱ�����crc
    uint8  chn;                //�ϱ�����ͨ��
    uint8  len;                //�ϱ����ݳ���

}TWdtTstUp;

typedef struct
{
    uint8_t type;
    uint8_t addr;//�豸��ַ
    uint8_t main_addr;//���������豸��ַ
    uint8_t baud;//������
    uint8_t factory_model[2];
    uint8_t protocol[2];
}STMeterInfo, *pSTMeterInfo;
#pragma pack(1)
typedef struct
{
    uint8_t addr;//�豸��ַ
    uint16_t factory_model;//����ID
    uint16_t factory_type;//�ͺ�
    uint8_t remain_max[6];//������
    uint8_t overdraft[6];//͸֧��
    uint8_t alarm_volume[6];//������
    uint8_t reserve[2];//Ԥ��
}STPrepayDevInfo;

typedef struct
{
    uint8_t addr;//�豸��ַ
    uint16_t factory_model;//����ID
    uint16_t factory_type;//�ͺ�
    uint8_t recharge[6];//��ֵ��
    uint8_t index[4];//��ֵ���
    uint8_t reserve[12];//Ԥ��
}STChargeInfo;
typedef struct
{
    uint8_t addr;//�豸��ַ
    uint16_t factory_model;//����ID
    uint16_t factory_type;//�ͺ�
    uint16_t errno;//�������
    
    uint8_t recharge[6];//��ֵ��
    uint8_t index[4];//��ֵ���
    uint32_t meterstt;//���״̬
    uint8_t reserve[8];//Ԥ��
}STChargeret;

typedef struct
{
    uint32_t price;
    uint32_t gas;
}STStep;

typedef struct
{
    uint8_t addr;//�豸��ַ
    uint16_t factory_model;//����ID
    uint16_t factory_type;//�ͺ�
    uint8_t step_cycle;//��������
    uint8_t exe_time[4];
    STStep step[5];
    uint16_t index;//�������
}STPriceAdjust;

#pragma pack(4)

/************************************************************************
**��֧����������������Ϣ
*************************************************************************/
typedef struct 
{
    uint16  fn;                         //fn
    uint16  pn;                         //pn
    uint8*  rbuf;                       //��������ָ��
    uint8*  sbuf;                       //��������ָ��
    uint8   info[8];                    //������Ϣ
    uint16  len;                        //����

}DATA_EXTRA;

typedef uint8 (*ftype_paraload)(void);


typedef uint8 (*ftype_pararst)(PST_TableStr*);

#define PARA_TYPE_ALL  0x00FF           //��������������
#define PARA_TYPE_FARP 0x0001
#define PARA_TYPE_INP  0x0002
#define PARA_TYPE_PST  0x0004
#define PARA_TYPE_TASK 0x0008
#define RUNPARA_TYPE_FARP 0x0010


/************************************************************************
**����0_��Ҫ��ʷ���ݱ��ݽṹ��
*************************************************************************/
typedef struct
{
    uint32 ver;                     //��ʷ����汾ascii
    uint8  tbk[6];                  //�Ϸ���ʱ��,������ʱ����(hex)
    uint8  tdt[6];                  //�ϴ�ͣ��ʱ��,������ʱ����(hex)
    uint8  e1cnt;                   //һ��(��Ҫ)�¼�������
//	    uint8  e2cnt;                   //����(һ��)�¼�������
    uint8  e1pt;                    //һ��(��Ҫ)�¼�����ָ��
//	    uint8  e2pt;                    //����(һ��)�¼�����ָ��
    uint8  ckten;                   //���ܱ��ʱ���ܿ���(afn05f30����)
    uint8  rcnt;                    //��ȷ���ϱ�֡������(0-63)
    uint8  rpt;                     //��ȷ���ϱ�֡����ָ��(0-63)
    uint8  enable;                  //ʹ�ܼ�¼(B0:�����ϱ�,B1:�޳�δʵ��)
    uint8  esamst;                  //esam ģ��״̬
    uint8  pwrflag;                 //����״̬
    uint16 crc;

}S_FramMisData;

EXT_ZY S_FramMisData gs_FramMisData;

EXT_ZY    S_PSTPARA gs_PstPara;
/************************************************************************
 * @function: UserVarInit
 * @����: �ָ���������
 * 
 * @����: 
 * @param: tbl 
 * @param: frame 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2014/2/13)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 UserVarInit(void);


/************************************************************************
 * @function: PST_ZY_Decode
 * @����: �жϵ�ǰ���������Ƿ�Ϊ�Ϸ�֡,����ǵĻ��������Ϊ����ͨѶ�ṹ��
 * 
 * @����: 
 * @param: receive ���յ����ݵĻ���
 * @param: send ��Ҫ���͵����ݵĻ���
 * @param: sbuflen ���ͻ��泤��
 * @param: frame ���ݽ����ɵĽṹ��
 * 
 * @����: 
 * @return: uint8  PST_ERR_OK/PST_ERR_FT
 * @˵��: 
 * @����: yzy (2013/7/16)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 PST_ZY_Decode(uint8* receive, uint8* send, uint16 sbuflen, PST_Frame* frame);


/************************************************************************
 * @function: PST_ZY_Proc
 * @����: 
 * 
 * @����: 
 * @param: frm ���ݽ����ɵĽṹ��
 * 
 * @����: 
 * @return: uint8  PST_ERR_OK/PST_ERR_FT
 * @˵��: 
 * @����: yzy (2014/5/27)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 PST_ZY_Proc(PST_Frame* frm);


/************************************************************************
 * @function: PST_ZY_Pack
 * @����: ����Դ֡���д��,����ͨѶ
 * 
 * @����: 
 * @param: frame ����Ľṹ��
 * @param: err 
 * @param: length ����Ϊ����������󳤶�,���Ϊ�����֮��ĳ���
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2014/5/27)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 PST_ZY_Pack(PST_Frame* frame, uint8 err, uint16* length);



/************************************************************************
 * @function: PST_ZY_Do0F
 * @����: ����Դ֡���д��,����ͨѶ
 * 
 * @����: 
 * @param: data: 0F��һ���ֽڵĻ�����ָ��,��ʽ:C,DI1,DI0,������
 * @param: len: �������������ܳ���,����Ϊ��������������
 * 
 * @����: 
 * @return: uint8  
 * @˵��: �����ն˹�Լ0F������
 *      �����������Ӵ���ָ�뿪ʼ����,���ΰ���C,DI1,DI0,������
 *      ������C:01Ϊ��,04/05Ϊд
 * @����: yzy (2014/5/27)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 PST_ZY_Do0F(uint8* data, uint16* len);
uint8 VS_BuildLinkFrm(uint8 type, uint16_t id, uint8* buffer);
uint8_t VS_BuildReportFrm(uint8_t type, uint16_t id, uint16_t err, uint8* buffer);

/************************************************************************
 * @function: GD_Para_RW
 * @����: ����дflash����
 * 
 * @����: 
 * @param: addr flashƫ�Ƶ�ַ
 * @param: buff ��д����
 * @param: len ��Ҫ��д�ĳ���
 * @param: rw false��; trueд
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2014/2/12)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 GD_Para_RW(uint32 addr, uint8* buff, uint16 len, bool rw);
/************************************************************************
 * @function: Netp_Framework_RW
 * @����: ����������flash��ȡ
 * 
 * @����: 
 * @param: addr flashƫ�Ƶ�ַ
 * @param: buff ��д����
 * @param: len ��Ҫ��д�ĳ���
 * @param: rw false��; trueд
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2014/2/12)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 Netp_Framework_RW(uint32 addr, uint8* buff, uint16 len, bool rw);

#define SET_FLAG(X)     {X = 1;}
#define CLEAR_FLAG(X)   {X = 0;}

/*********************************************************************************
* Function: ���������
* Description:    
* Calls:          
* Called By:      
* Data Accessed: 
* Data Updated:  
* Input:      
* Output:         
* Return:         
* Others:         
*******************************************************************************/
unsigned char GetMeterNum(void);

#endif


