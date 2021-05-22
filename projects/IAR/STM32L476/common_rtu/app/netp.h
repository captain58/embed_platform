/****************************************Copyright (c)**************************************************
**                                  ____�Ƽ����޹�˾
**                                          ������
**
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: netp.h
**��   ��   ��: yzy
**����޸�����: 2007��9��5��
**��        ��: ����������ͨѶ�˿ڵ�ͨ�ú�����ͷ�ļ�
**ע        ��:
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: yzy
** ��  ��: v1.0
** �ա���: 2007��9��5��
** �衡��: ԭʼ�汾
********************************************************************************************************/

#ifndef _NETP_H_
#define _NETP_H_


#ifndef EXT_NETP
#define EXT_NETP extern
#endif


#define NETP_ERR_OK         (uint8)0
#define NETP_ERR_NT         (uint8)1
#define NETP_ERR_FT         (uint8)2
#define NETP_ERR_UV         (uint8)3        //�лظ�,������Чֵ(�ѱ�ʱʹ��)


EXT_NETP uint8 ucDlmsReceCnt;   //DLMS����֡����
EXT_NETP uint8 ucDlmsSendCnt;   //DLMS����֡����

typedef struct
{   
    uint8 hour;
    uint8 day;
    uint8 month;
}STDBFLAG;
EXT_NETP STDBFLAG dbflag;
/************************************************************************ 
**�¹ұ�ͨѶ���������
*************************************************************************/
EXT_NETP  uint32 gul_NetpReadMins[MCB_PORT_NUM];




/************************************************************************ 
**�������ݳ�����Ϣ�ṹ��
*************************************************************************/
typedef struct
{
    uint32  di;                              //�����ʶ
    uint16 patr;                            //��������
    uint16 datr;                            //��������
    uint32 add;                             //��Ե�ַ
    uint8 zhtp;                             //���������(0:�������,1:����������,others:Ԥ��)
    uint8 zhid;                             //�������([zhtp != 0] 0xFF:�鳭,others:����)
    uint8 zhnum;                            //���������([zhtp != 0 && zhid == 0],����̶�Ϊ1)
    uint8 sbit;                             //��Ӧ��Ч״̬������ʼλ��
    uint8 dgt;                              //������С��λ��\ͳһ��ʽ����
    uint8 len;                              //ÿ������ݳ��� (<=4)
    uint16 _r;                              //���ݱ���(ÿ�����ݱ���Ψһ,���ڴ�С���ʶ��)
    void* func;                             //���⴦������

}NETPRD;

#define  NTRD_ZHTP_NULL  0                  //�������
#define  NTRD_ZHTP_MUFL  1                  //���������


#define  NTRD_PATR_SKIP  0x0001             //�������������
#define  NTRD_PATR_MUST  0x0002             //�س���


#define  NTRD_DATR_THEX  0x0001             //ת����HEX��ʽ
#define  NTRD_DATR_BEXT  0x0002             //�ֽ���չΪ4�ֽ�
#define  NTRD_DATR_PDIV  0x0004             //��СС��λ��
#define  NTRD_DATR_PMUL  0x0008             //��չС��λ��
#define  NTRD_DATR_INSG  0x0010             //������������(BCD)
#define  NTRD_DATR_OUSG  0x0020             //�����������(HEX)
#define  NTRD_DATR_SPEC  0x0080             //���⴦��







/************************************************************************ 
**�������ݸ��˿ڳ�����Ϣ�ṹ��
*************************************************************************/
#define SIZE_NETPBMP   8

typedef struct
{
    uint8*  data;                           //�ڴ�ṹ���ַ*
    uint8*  eft;                            //������Ч��־λ*
    NETPRD* tbl;                            //��ʶ�б�*
    uint8   tbllen;                         //��ʶ�б�����*
    uint8   tstno;                          //�������*
    uint8   idx;                            //��ǰ��ʶλͼ����    
    uint8   prio;                           //��ʶ���ȼ�(0�س���)
    uint8   bmp_m[SIZE_NETPBMP];            //�����ȼ���ʶλͼ_must
    uint8   bmp_n[SIZE_NETPBMP];            //�����ȼ���ʶλͼ_normal

}NETPSTR;


EXT_NETP NETPSTR gss_NetpPortStt[MCB_PORT_NUM];




/************************************************************************
 *������Լ�����С�����������FN�Ķ�Ӧ
 ************************************************************************/
#define MAX_MAP_DITYPE  32

typedef struct
{
    uint8 dlh;
    uint8 xlh;
    uint8 _r;
    uint8 afncmap[32];                      //��Ҫ������һ��������
    uint8 afndmap[32];                      //��Ҫ�����Ķ���������
    uint8 dimap[MAX_MAP_DITYPE];            //��Ҫ������ʵʱ���ݱ�ʶλͼ
    uint16 crc;
}DXLFnMap;


//EXT_PSTGW DXLFnMap gs_afnDICfg;




/************************************************************************ 
**����״̬�ṹ��
*************************************************************************/
#define NUM_TSTFAILREC     10

typedef struct
{
    S_TimeLabel start;          //���������ʼʱ��
    S_TimeLabel end;            //�����������ʱ��
    S_TimeLabel extime;         //����ִ��ʱ��
    uint8 BSTOP;                //�Ƿ�ֹͣ״̬
    uint8 num_mt;               //������
    uint8 num_fl;               //����ʧ������
    uint8 fl_list[NUM_TSTFAILREC];          //����ʧ���б�
}S_NetpStt;


__no_init EXT_NETP S_NetpStt gs_NetpReadStt[MCB_PORT_NUM];


EXT_NETP uint32 gul_noPlcReadSecs;

//�������ݲ�ѯ�¼�����
#define NETP_READCURE_MIN           2     //�Ӻ��ȡ����ʱ�䣬��ֹ����û��������ߡ�
#define NETP_REREADCURE_HOUR        1    //ÿ��N��Сʱ��ѯ���������Ƿ�����©

//�ն������ݲ�ѯ�¼�����
#define NETP_READDAY_MIN   5        //�Ӻ��ȡ����ʱ�䣬��ֹ����û������ն���
#define NETP_REREADDAY_HOUR     8    //ÿ��N��Сʱ��ѯ�����Ƿ�����©

//�¶������ݲ�ѯ�¼�����
#define NETP_READMONTH_MIN   8       //�Ӻ��ȡ����ʱ�䣬��ֹ����û������¶���

/************************************************************************ 
**���¼��ı�־λ
*************************************************************************/
typedef struct
{
	uint8 time[5];  //�¼���ʼʱ��
//    uint8 curstart[6];  //������ʼʱ��
//    uint8 curend[6];    //���߽���ʱ��
}S_NetpStEVT;


__no_init EXT_NETP S_NetpStEVT gs_NetpReadStEVT[CORE_TST_NO];



/************************************************************************ 
**�Ա�Уʱ����
*************************************************************************/
typedef struct
{
    uint16 period;  //���� Сʱ
    uint8 datatime[5];  //��ʼʱ�� ��������ʱ��
    uint32 hours;   //��Сʱ��
    uint8  stt;     //Уʱ״̬
}S_NetpMtTime;


EXT_NETP S_NetpMtTime gs_NetpMtTime;

  
    
/************************************************************************
 * DLMS ���������ʽ
************************************************************************/

typedef struct
{
    uint8 port;
    uint8 tstno;
    uint8 cmdtype;
    uint8_t addr;//�������豸��ַ
    uint8_t addr_len;
    uint8_t baud_cfg;
    uint8 di;
    uint8 nfrm;
    uint8_t ss;
    ST_Netp_FW * fw;
}ST_RDMT_CMD;    //����ID����


/************************************************************************
 * ���������ʽ
************************************************************************/
#define     ACT_MAX_INBUF_H_LEN       512
#define     ACT_MAX_INBUF_L_LEN       256
#define     ACT_MAX_INBUF_S_LEN       256

typedef struct
{
    uint8 buf[ACT_MAX_INBUF_H_LEN];
    uint16 len;
}ST_ACT_INPBUF_H;    //���������ʽ

typedef struct
{
    uint8 buf[ACT_MAX_INBUF_L_LEN];
    uint16 len;
}ST_ACT_INPBUF_L;    //���������ʽ

typedef struct
{
    uint8 buf[ACT_MAX_INBUF_S_LEN];
    uint8 len;
}ST_ACT_INPBUF_S;    //���������ʽ

typedef struct
{
    uint8 * buf;
    uint8 len;
}ST_ACT_INPBUF_N;    //���������ʽ



/************************************************************************
 * @function: Netp_PreInit
 * @����: �����˿ڳ�ʼ��
 * 
 * @����: 
 * @param: index �˿ڱ��
 * @����: 
 * @˵��: 
 * @����: yzy (2014/2/7)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void Netp_PreInit(uint8 index);

/************************************************************************
 * @function: Netp_PentTran
 * @����: �����˿�����͸������
 * 
 * @����: 
 * @param: buffer �շ�����
 * @param: length �շ��������ݳ���
 * @param: ss �˿�����
 * @param: port �˿ں�(0-!)
 * @param: overtime �ȴ�ʱ��,��λ100ms
 * @param: nfrm ͨѶ�Ĺ�Լ���,0xff��ʾΪ�м�
 * 
 * @����: 
 * @return: uint8  SA_ERR_OK, SYS_ERR_FT
 * @˵��: 
 * @����: yzy (2011-9-8)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 Netp_PentTran(uint8* buffer, uint16* length, uart_config_t* ss, uint16 overtime, uint8 port);

/************************************************************************
 * @function: Netp_Trans
 * @����: �����˿�����͸������(��������)
 * 
 * @����: 
 * @param: buffer 
 * @param: length 
 * @param: overtime 
 * @param: nfrm 
 * @param: tstno 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2011-9-8)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 Netp_Trans(uint8* buffer, uint16* len, uint8 overtime, uint8 nfrm);

/************************************************************************
 * @function: Netp_BGRead
 * @����: �����˿ڲ������ݶ�ȡ����
 * 
 * @����: 
 * @param: buffer �������ݻ���
 * @param: di �����ʶ
 * @param: length �������ݵ�Ԥ�Ƴ���
 * @param: tstno �������
 * 
 * @����: 
 * @return: uint8  NETP_ERR_FT/NETP_ERR_OK
 * @˵��: 
 * @����: yzy (2014/2/7)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	uint8 Netp_BGRead(uint8* buffer, uint32 di, uint16 length, uint8 tstno);

/************************************************************************
 * @function: Netp_XBGRead
 * @����: �����˿ڲ������ݶ�ȡ����
 * 
 * @����: 
 * @param: buffer �������ݻ���
 * @param: di ���ݱ�ʶ
 * @param: length �������ݵ�Ԥ�Ƴ���
 * @param: tstno �������
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2014/2/7)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	uint8 Netp_XBGRead(uint8* buffer, uint32 di, uint16 length, uint8 tstno);
//	
//	/************************************************************************
//	 * @function: Netp_Read
//	 * @����: �����˿ڶ����ݴ���
//	 * 
//	 * @����: 
//	 * @param: buffer �������ݻ���
//	 * @param: di �����ı�ʶ
//	 * @param: length �������ݵĳ���
//	 * @param: tstno �������
//	 * 
//	 * @����: 
//	 * @return: uint8  NETP_ERR_FT/NETP_ERR_OK
//	 * @˵��: 
//	 * @����: yzy (2014/2/7)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	uint8 Netp_Read(uint8* buffer, uint32 di, uint16 length, uint8 tstno);



/************************************************************************
 * @function: Netp_ReadTstDemandData
 * @����: ����FLASH��������(Ϊ���õ�ʱ����ʱȥ����,�������¶���)
 * 
 * @����: 
 * @param: 
 * @����: SYS_ERR_FT, SA_ERR_OK
 * @˵��: 
 * @����: yzy (2012-4-11)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	void Netp_ReadTstDemandData(uint8 port);





/************************************************************************
 * @function: BG_ExtractData
 * @����: ��ȡ��Ч��������
 * 
 * @����: 
 * @param: buffer �������ݻ���,������Ч��������
 * @param: length ����Ϊ���ջ��泤��,����Ϊ��Ч�������ݳ���
 * @param: di ��ʶ(�ձ�ʾ���б�ʶ)
 * 
 * @����: 
 * @return: uint8 NETP_ERR_OK,NETP_ERR_NT,NETP_ERR_FT
 * @˵��: 
 * @����: yzy (2011-9-7)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	uint8 BG_ExtractData(uint8* buffer, uint16* length, uint32 di);

/************************************************************************
 * @function: XBG_ExtractData
 * @����: ��ȡ��Ч��������
 * 
 * @����: 
 * @param: buffer �������ݻ���,������Ч��������
 * @param: length ����Ϊ���ջ��泤��,����Ϊ��Ч�������ݳ���
 * @param: di ��ʶ(�ձ�ʾ���б�ʶ)
 * 
 * @����: 
 * @return: uint8 NETP_ERR_OK,NETP_ERR_NT,NETP_ERR_FT
 * @˵��: 
 * @����: yzy (2011-9-7)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	uint8 XBG_ExtractData(uint8* buffer, uint16* length, uint32 di);

/************************************************************************
**��������: void Netp_StDayProc(uint8 cbport)
**�� ��: ����������
**
**�� ��:  cbport �����˿ں�
**�� ��: 
**
**�� ע: 
**--------------------------------------------------------
**�� ��: yzy
**�� ��: 2009-08-01
*************************************************************************/
void Netp_StDayProc(uint8 cbport);


/************************************************************************
 * @function: PST_DLMS_HDLC_Decode
 * @����: �жϵ�ǰ���������Ƿ�Ϊ�Ϸ�֡,����ǵĻ��������Ϊ����ͨѶ�ṹ��
 * 
 * @����: 
 * @param: receive ���յ����ݵĻ���
 * @param: send ��Ҫ���͵����ݵĻ���
 * @param: sbuflen ���ͻ��泤��
 * @param: frame ���ݽ����ɵĽṹ��
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2014/5/27)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	uint8 PST_DLMS_HDLC_Decode(uint8* recv, uint8* send, uint16 sbuflen, PST_Frame* frame);

/************************************************************************
 * @function: PST_DLMS_HDLC_Proc
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
//	uint8 PST_DLMS_HDLC_Proc(PST_Frame* frame);

/************************************************************************
 * @function: Netp_chktimeisok
 * @����: �жϱ��ƶ�ʱʱ���Ƿ�
 * 
 * @����: 
 * @param:  
 * @����: 
 * @˵��: 
 * @����: zjc (2015/5/11)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void Netp_chktimeisok(void);  //

/************************************************************************
 * @function: Netp_Send
 * @����: �����˿�͸������(��������)
 * 
 * @����: 
 * @param: phyport �˿�
 * @param: buffer �շ�����
 * @param: len ���ͳ���
 * @param: overtime ��ʱʱ��, ��λ100ms
 * @param: ss ���ڿ�����
 * @param: fw ֡����
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2011-11-2)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//uint8 Netp_Send(uint8_t phyport, uint8_t * buffer, uint16* len, 
//                    uint8_t overtime, uart_config_t *ss, ST_Netp_FW * fw);
uint8 Netp_Send(uint8_t phyport, uint8* buffer, uint16* len, 
                uint8 overtime, uart_config_t *ss, NetpFrameStr * frameStr);

uint8 Netp_Check(uint8_t port);

#endif
