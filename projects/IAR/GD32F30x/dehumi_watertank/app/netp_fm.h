/****************************************Copyright (c)**************************************************
**                                  ____�Ƽ����޹�˾
**                                          ������
**
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: Hexing_Dlms.h
**��   ��   ��: ZJC
**����޸�����: 2014��11��3��
**��        ��: ����DLMSЭ�����
**ע        ��:
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: ZJC
** ��  ��: v1.0
** �ա���: 2014��11��3��
** �衡��: ԭʼ�汾
********************************************************************************************************/

#ifndef _HEXING_DLMS_H_
#define _HEXING_DLMS_H_


#ifndef EXT_HEXING_DLMS
#define EXT_HEXING_DLMS extern
#endif


/************************************************************************
 *   DLMS ��������
************************************************************************/

#define		COMM_DATATYPE_NULL		0//��
#define		COMM_DATATYPE_ARRAY		1//����
#define		COMM_DATATYPE_STRUCT	2//�ṹ��
#define		COMM_DATATYPE_BOOLEAN	3//��������
#define		COMM_DATATYPE_BITSTRING 4//BIT��
#define		COMM_DATATYPE_INT32    	5//�з���32λ
#define		COMM_DATATYPE_INT32U    6//�޷���32λ
#define		COMM_DATATYPE_OCTS		9//�ַ�������
#define		COMM_DATATYPE_ASCII		10//ASCII��
#define		COMM_DATATYPE_BCD		13//BCD������
#define		COMM_DATATYPE_INT8      15//�з���8λ
#define		COMM_DATATYPE_INT16     16//�з���16λ
#define		COMM_DATATYPE_INT8U		17//�޷���8λ
#define		COMM_DATATYPE_INT16U	18//�޷���16λ
#define		COMM_DATATYPE_ENUM		22//ö��
#define		COMM_DATATYPE_FLOAT32	23//����32λ��

typedef enum
{
    NETP_DLMS_TYPE_LINK = 1,		//��·������
    NETP_DLMS_TYPE_APP,     		//Ӧ�ò�����

    NETP_DLMS_TYPE_GET = 0x11,		//��ѯ����
    NETP_DLMS_TYPE_SET,		        //���ñ���
    NETP_DLMS_TYPE_ACTION,          //ִ�б���
    NETP_DLMS_TYPE_GET_AF,          //����֡����
    NETP_DLMS_TYPE_REALY,           //�м̷�ʽ
    NETP_DLMS_TYPE_LINK_AF,       //link����֡����
}EM_DLMS_TYPE;

//�������ݲ�ѯ�¼�����
#define NETP_READCURE_MIN           2     //�Ӻ��ȡ����ʱ�䣬��ֹ����û��������ߡ�
#define NETP_REREADCURE_HOUR        1    //ÿ��N��Сʱ��ѯ���������Ƿ�����©

//�ն������ݲ�ѯ�¼�����
#define NETP_READDAY_MIN   5        //�Ӻ��ȡ����ʱ�䣬��ֹ����û������ն���
#define NETP_REREADDAY_HOUR     8    //ÿ��N��Сʱ��ѯ�����Ƿ�����©

//�¶������ݲ�ѯ�¼�����
#define NETP_READMONTH_MIN   8       //�Ӻ��ȡ����ʱ�䣬��ֹ����û������¶���


typedef enum
{
    NETP_READ_TX13_F = 0,       //����1.3
//	    NETP_READ_DLMS_ES = 0,          //�¼�״̬
    NETP_READ_DLMS_E1,       //��׼�¼�
    NETP_READ_DLMS_E2,       //�Ե��¼�
    NETP_READ_DLMS_E3,       //�̵����¼�
    NETP_READ_DLMS_E4,       //�����¼�

    NETP_READ_DLMS_WP,       //�����й�����
    NETP_READ_DLMS_WN,       //�����޹�����
    NETP_READ_DLMS_U,       //��ѹ
    NETP_READ_DLMS_I,       //����
    NETP_READ_DLMS_P,       //����

    NETP_READ_DLMS_DE,       //�ն������
    NETP_READ_DLMS_DM,       //�¶�������
    NETP_READ_DLMS_TM,       //Уʱ

    NETP_UPDATA_ENABLE,       //����ʹ��
    NETP_UPDATA_MODE,       //����ģʽ
    NETP_UPDATA_INIT,       //������Ϣ��ʼ��
    NETP_UPDATA_TRAN,       //�ļ�����
    NETP_UPDATA_MAP,       //��λͼ
    NETP_UPDATA_CHECK,       //��֤
    NETP_UPDATA_DATETIME,       //��������ʱ��
    NETP_UPDATA_ACTION_NOW,       //����ִ��
    NETP_UPDATA_READ_STAT,       //��״̬
    NETP_READ_VERSION,       //������汾����ֹ�ظ�����

    NETP_READ_HX_OBIS_MAX,        //���ͨ����
}EM_RD_DLMS_ID;

    
    
/************************************************************************
 * DLMS ��������
************************************************************************/
#define UPDATABLOCKSIZE     192         //��������������


#define		F_UpdataFailed		7	//(7)Image activation failed 
#define		F_UpdataOk			6	//(6)Image activation successful,		
#define		F_UpdataFirm		5	//(5)Image activation initiated,			
#define		F_VerifyFailed		4	//(4)Image verification failed,
#define		F_VerifyOk			3	//(3)Image verification successful,
#define		F_WaitVerify		2	//(2)Image verification initiated, 
#define		F_InitOk			1	//(1)Image transfer initiated,
#define		F_NO				0	//(0)Image transfer not initiated,

typedef enum
{
    METER_UPGRADE_NONE = 0,     //��
    METER_UPGRADE_ENABLED,      //��1���������� 1
    METER_UPGRADE_READY,       //��2���������Ѵ��꣬2
    METER_UPGRADE_START,       //��3����ʼ����
    METER_UPGRADE_INIT,        //��4����ʼ��ʼ��
    METER_UPGRADE_DATAING,      //��5����������
    METER_UPGRADE_CHECK,        //��6��У��
    METER_UPGRADE_ACTION,       //��7��ִ��
    METER_UPGRADE_VERSION,      //��8����ȡ�汾��
    METER_UPGRADE_OK,           //��9�������ɹ�
    METER_UPGRADE_BAD,          //��10������ʧ��
}EM_MT_UPGRADE_ID;

    
typedef	struct								//Զ�������õĽṹ��
{
	uint8   mtVer[20];          //�����������汾��
    uint8   mtMD5[16];          //���������������
    uint8   mtUpdtm[6];            //��������ִ��ʱ��
    uint32  TotalSize;		    //�������ֽ���
	uint16  BlockSum;           //�ְܷ���
	uint8   LastSize;           //���һ�����ֽ��� 
	uint8   FlagState[CORE_TST_NO];          //����״̬��־λ
    uint16  tstno;              //��ǰ���������Ĳ������
    uint8   chktype;            //�������ͣ�00�������ܣ�01��GCM�� 02 ��ECB��
}EXT_UPDATASTRUCT;
EXT_HEXING_DLMS  EXT_UPDATASTRUCT  g_Updata;

EXT_HEXING_DLMS  uint8  guc_mtUpdata_sendfailcnt;       //��������������������ʧ�ܴ���



/************************************************************************
 * @function: DLMS_ByteToInt24
 * @����: �����ֽ�ת����24λ��
 * 
 * @����: 
 * @param: byte ���Ҫת���������ֽڵĻ���
* @param: isbcd �����ʽ(��hex,��bcd)
 * 
 * @����: 
 * @return: uint32  �����ֽ�ת����24λ��֮��Ľ��
 * @˵��: ����byte[0] = 0x03, byte[1] = 0x02, byte[2] = 0x04��
 *        ��ôת����Ľ��Ϊ0x030204
 * @����: zjc (2014/10/9)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint32 DLMS_ByteToInt24(uint8* byte, bool isbcd);
   
/************************************************************************
 * @function: DLMS_UpEvtERC40414243
 * @����: ���ͱ��Ƶ��¼�ID��ERC40414243
 * 
 * @����: 
 * @param:
 * @����: 
 * @˵��: 
 * @����: zjc (2014/8/14)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	void DLMS_UpEvtERC40414243(uint8 uc_i, uint8 di, uint8 *buffer);

/************************************************************************
 * @function: Netp_mtGetEvt
 * @����: ��ȡ�¼�
 * 
 * @����: 
 * @param:
 * @����: 
 * @˵��: 
 * @����: zjc (2014/8/14)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	void Netp_mtGetEvt(ST_ACT_INPBUF_L *inpLbuf, ST_ACT_INPBUF_S *inpSbuf, ST_RDMT_CMD *stpDlmsCmd);

/************************************************************************
 * @function: Netp_ReadEvent
 * @����: ��ȡDLMS�¼�
 * 
 * @����: 
 * @param: index �����˿�����
 * @����: 
 * @˵��: 
 * @����: zjc (2014/8/14)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	void Netp_ReadEvent(uint8 index);

/************************************************************************
 * @function: ReadDlmsLastDayData
 * @����: DLMS������һ�յ��ն�������
 * 
 * @����: 
 * @param: rdenable  1�������һ�����ݣ�0��������������Ƿ������ 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: zjc (2014-8-16)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	void ReadDlmsLastDayData(uint8 rdenable);

/************************************************************************
 * @function: ReadDlmsLastMonEnergy
 * @����: DLMS������һ�յ��¶������
 * 
 * @����: 
 * @param: rdenable  1�������һ�����ݣ�0��������������Ƿ������ 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: zjc (2014-8-16)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	void ReadDlmsLastMonEnergy(uint8 rdenable);

/************************************************************************
 * @function: ReadDlmsLastMonthDMData
 * @����: DLMS������һ�µ��¶�������
 * 
 * @����: 
 * @param: rdenable  1�������һ�����ݣ�0��������������Ƿ������ 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: zjc (2014-8-18)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	void ReadDlmsLastMonthDMData(uint8 rdenable);

/************************************************************************
 * @function: Netp_ReadCure
 * @����: ��ȡDLMS����ͨ�����ݣ�
 * 
 * @����: 
 * @param: time  ���ߵ�ʱ��
 * @param: din   �ٲ��������
 * @param: uc_i ������
 * @param: fw  Э�������ļ�
 * @����: 
 * @˵��: 
 * @����: zjc (2014/8/14)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//uint8 Netp_ReadCure(uint8 *time , uint8 *din, uint8 uc_i, PST_Netp_FW fw);

/************************************************************************
 * @function: Netp_ReadCureData
 * @����: ��ȡDLMS����ͨ�����ݣ�����15�������ߣ�
 * 
 * @����: 
 * @param: index �����˿�����
 * @����: 
 * @˵��: 
 * @����: zjc (2014/8/14)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void Netp_ReadCureData(uint8 index);


/************************************************************************
 * @function: Netp_ReReadCureData
 * @����: ÿ��һ��ʱ��β�ѯ��ʱ����������Ƿ���©������©���߽��в���
 * 
 * @����: 
 * @param: index �����˿�����
 * @param: readnum ������
 * @����: 
 * @˵��: 
 * @����: zjc (2014/8/14)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	void Netp_ReReadCureData(uint8 index, uint8 readnum);

/************************************************************************
 * @function: Netp_CheckTstTimeDlms
 * @����: ���¹�dlms�������Уʱ
 * 
 * @����: 
 * @param: index �����˿�����
 * @����: 
 * @˵��: 
 * @����: yzy (2014/2/11)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	void Netp_CheckTstTimeDlms(uint8 index);
   
/************************************************************************
 * @function: Netp_LoadFeeDlms
 * @����: ���¹�dlms������з�����װ
 * 
 * @����: 
 * @param: index �����˿�����
 * @����: 
 * @˵��: 
 * @����: zjc (2015/01/21)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	void Netp_LoadFeeDlms(uint8 index);

/************************************************************************
 * @function: Netp_UpdataMTInit
 * @����: ��������ʼ
 * 
 * @����: 
 * @param: 
 * @����: 
 * @˵��: 
 * @����: zjc (2014/9/3)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	void Netp_UpdataMTInit();
//	
//	/************************************************************************
//	 * @function: Netp_UpdataMTDataing
//	 * @����: ��������ʼ
//	 * 
//	 * @����: 
//	 * @param: 
//	 * @����: 
//	 * @˵��: 
//	 * @����: zjc (2014/9/3)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	void Netp_UpdataMTDataing();
//	
//	/************************************************************************
//	 * @function: Netp_UpdataMTAction
//	 * @����: ��������ʼ
//	 * 
//	 * @����: 
//	 * @param: 
//	 * @����: 
//	 * @˵��: 
//	 * @����: zjc (2014/9/3)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	void Netp_UpdataMTAction();
//	
//	/************************************************************************
//	 * @function: Netp_GetVersion
//	 * @����: ��ȡ�汾��Ϣ
//	 * 
//	 * @����: 
//	 * @param: 
//	 * @����: 
//	 * @˵��: 
//	 * @����: yzy (2014/11/14)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	void Netp_GetVersion();
//	
//	
//	/************************************************************************
//	 * @function: Netp_UpdataMTStart
//	 * @����: ��������ʼ
//	 * 
//	 * @����: 
//	 * @param: 
//	 * @����: 
//	 * @˵��: 
//	 * @����: zjc (2014/9/3)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	void Netp_UpdataMTStart();
//	
//	
//	/************************************************************************
//	 * @function: Netp_MtUpdataPro
//	 * @����: ������������
//	 * 
//	 * @����: 
//	 * @param: 
//	 * @����: 
//	 * @˵��: 
//	 * @����: zjc (2015/04/03)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	void Netp_MtUpdataPro();
//	   
//	/************************************************************************
//	 * @function: Netp_MtUpdataParaInit
//	 * @����: ��������������ʼ��
//	 * 
//	 * @����: 
//	 * @param: 
//	 * @����: 
//	 * @˵��: 
//	 * @����: zjc (2015/04/03)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	void Netp_MtUpdataParaInit();
/************************************************************************
 * @function: Netp_ReadCureData
 * @����: ��ȡDLMS����ͨ�����ݣ�����15�������ߣ�
 * 
 * @����: 
 * @param: index �����˿�����
 * @����: 
 * @˵��: 
 * @����: zjc (2014/8/14)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void Netp_ReadCureData(uint8 index);

//int Netp_Get_Framework(uint8_t pf, PST_Netp_FW fw);

/************************************************************************
 * @function: Netp_ReadDLMS
 * @����: �����˿�DLMS���ݶ�ȡ����
 * 
 * @����: 
 * @param: buffer �������ݻ���
 * @param: di ���ݱ�ʶ
 * @param: length �������ݵ�Ԥ�Ƴ���
 * @param: tstno �������
 * @param: cmdtype ��������
 * @����: 
 * @return: uint8  NETP_ERR_OK,NETP_ERR_NT(�лظ�,����֧��),NETP_ERR_FT
 * @˵��: 
 * @����: ZJC (2014/8/12)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 Netp_CommonRead(ST_ACT_INPBUF_L *inpLbuf, uint8_t * data, 
                                ST_RDMT_CMD *stpActCmd);
unsigned char GetProtocolNum(unsigned char *tMan, unsigned char *tPro);
//int Netp_Register_Set(ST_NETP_FW_BASE * fw);
int Netp_Register_Init(void);
uint16_t Netp_Send_With_Retry2(ST_ACT_INPBUF_L *inpLbuf, uint8_t * data, ST_RDMT_CMD *stpActCmd);

#endif