/****************************************Copyright (c)**************************************************
**                                  ____�Ƽ����޹�˾
**                                          ������
**
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: hlv.h
**��   ��   ��: yzy
**����޸�����: 2019��6��1��
**��        ��: hlv����
**ע        ��:
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: yzy
** ��  ��: v1.0
** �ա���: 2019��6��1��
** �衡��: ԭʼ�汾
********************************************************************************************************/

#ifndef _HLV_H_
#define _HLV_H_



#ifndef EXT_HLV
#define EXT_HLV extern
#endif



/************************************************************************
**��ȫ������������Ҫ������ʱ��
*************************************************************************/
#define SYS_SAFE_SECS    15



//EXT_HLV SysVar gs_SysVar;


/****************************************************************************
**��ʾϵͳ״̬�Լ�ϵͳ״̬���Ƽ�����
******************************************************************************/
#define HLV_STT_NENG    0x01               //ͣ�繤��״̬-Ƿѹ
#define HLV_STT_USBE    0x02               //USB���״̬
#define HLV_STT_USBP    0x04               //USB����״̬
#define HLV_STT_UTSTF   0x08               //USB����ʧ��
#define HLV_STT_UTSTS   0x10               //USB���Գɹ�
#define HLV_STT_LENG    0x20               //ͣ�繤��״̬-��ѹ







/************************************************************************
**��ǰϵͳ������
*************************************************************************/

EXT_HLV S_TimeLabel gs_SysLastDayTime;

/************************************************************************
**��һ�ඳ���־(bit0-2��ʾ3����ͬ����˿�)
*************************************************************************/
EXT_HLV uint8 guc_HourSaveFlag;
EXT_HLV uint8 guc_DaySaveFlag;
EXT_HLV uint8 guc_MonSaveFlag;
EXT_HLV uint8 guc_RecSaveFlag;

/************************************************************************
**�ڶ��ඳ���־(bit0-2��ʾ0��,15��,30��,45�����߼�¼��)
*************************************************************************/
EXT_HLV uint8 guc_QuarSaveFlag;


typedef struct
{
    uint16 num;
    uint8 update[128];          //1:�յ�,0:δ�յ�
}S_UPDATAMSG;
EXT_HLV S_UPDATAMSG  gs_UpDtMsg;











//fram��ź�
#define FM_MIS     0                            //gs_FramMisData(0)  27/32 = 1
//#define FM_RECPT   1                            //���������ݶ���ָ��(1-4)
#define FM_PARASTT 2                            //����״̬(1)       32/32 = 1
//#define FM_TMSTT   10                           //gs_TmStatData(10)
//#define FM_VLSTT   11                           //gs_VolStatData(11)
//#define FM_TASK    2                           //2�������ϱ�ʱ��(78-88)
#define FM_EVTDT    3                        //ͣ�緢��ʱ�䣨2-5��  //  5*24/32 = 4







/************************************************************************
**inp����\���ݽṹ�� 
**(����,pt,ct,����ct,���ѹ,�����,����������� gs_InpExtraData ��,��δ����ֵʹ��) 
*************************************************************************/
typedef struct
{
    uint8  evtbit[8];                           //�¼���¼��Ч��־λ
    uint8  evtrank[8];                          //�¼���Ҫ�Եȼ���־λ
}S_InpExtraVar;

EXT_HLV S_InpExtraVar gs_InpVarData;


typedef struct
{
    uint8  gbbreak;                             //�㲥��ϳ���(��Ϊ����ʱ��ܳ�,�ҳ����ڼ䲻����������Ϣ)
    uint16 gbbvsec;                             //�㲥��ϳ�����ʱ(��ֹ���ڴ�ϳ���)
    uint32 cbday;                               //������ BS
    uint8  cbhour;                              //����ʱ�� bin
    uint8  cbmin;                               //����ʱ�� bin
    uint8  netitv;                              //�ն˳�����(1-60)
    uint8  cbqjn;                               //������ʱ����
    uint16 cbqjv[24][2];                        //�ն˳�����������
    uint8  cten;                                //�㲥Уʱʹ��
    uint8  ctdhm[3];                            //�㲥Уʱʱ��
}S_InpExtraCB;

EXT_HLV S_InpExtraCB gs_InpExtracb[MCB_PORT_NUM];
  

/************************************************************************
**
*************************************************************************/
typedef union
{
    uint8 bytes[32];
    struct
    {
        S_TimeLabel stime;
        S_TimeLabel rtime;
        uint8 eft[4];
        uint16 crc;
    }seg;
}S_DATA_SEG_TEMPLET;





/************************************************************************
**�����ն�������ģ����Ϣ
*************************************************************************/
#define SIZE_RECDAY_DAYS        31
#define MAX_LEN_RECDAY_SEG      128




/************************************************************************
**�����ն������ݶ�0��Ϣ
*************************************************************************/
#define LEN_RECDAY_SEG0     96
#if(LEN_RECDAY_SEG0 > MAX_LEN_RECDAY_SEG)
    #error "rec seg alloc err"
#endif

typedef union
{
    uint8 bytes[LEN_RECDAY_SEG0];
    struct
    {
        S_TimeLabel stime;
        S_TimeLabel rtime;
        uint32 Wp[5];
        uint8 eft[4];
        uint16 crc;
    }seg;

}S_RECDATA_SEG0;


#define EFT_RECDAY_WP  0

/************************************************************************
**�����ն������ݶ�2��Ϣ
*************************************************************************/
#define LEN_RECDAY_SEG2    128
#if(LEN_RECDAY_SEG2 > MAX_LEN_RECDAY_SEG)
    #error "rec seg alloc err"
#endif

typedef union
{
    uint8 bytes[LEN_RECDAY_SEG2];
    struct
    {
        S_TimeLabel stime;
        S_TimeLabel rtime;
        uint32 dpp[5];                  //�����й�����
        uint32 dppt[5];                 //�����й�����ʱ��(AFMT)
        uint32 dnp[5];                  //�����й�����
        uint32 dnpt[5];                 //�����й�����ʱ��(AFMT)
        uint8 eft[4];
        uint16 crc;
    }seg;
}S_RECDATA_SEG2;

#define EFT_RECDAY_DPP   0
#define EFT_RECDAY_DPPT  1
#define EFT_RECDAY_DNP   2
#define EFT_RECDAY_DNPT  3





/************************************************************************
**�ն�������ģ��
*************************************************************************/
#define SIZE_DAY_DAYS       31
#define MAX_LEN_DAY_SEG     128




/************************************************************************
**�ն������ݶ�0��Ϣ
*************************************************************************/
#define LEN_DAY_SEG0    96
#if(LEN_DAY_SEG0 > MAX_LEN_DAY_SEG)
    #error "day seg alloc err"
#endif

typedef union
{
    uint8 bytes[LEN_DAY_SEG0];
    struct
    {
        S_TimeLabel stime;  //12 �ֽ�
        S_TimeLabel rtime;  //12�ֽ�
        uint32 Wp[5];                   //���й�����    20�ֽ�
        uint32 Wn[5];                   //���й�����    20�ֽ�
        uint8 eft[4];       //4�ֽ�
        uint16 crc;         //2�ֽ�
    }seg;
}S_DAYDATA_SEG0; //70 �ֽ�


#define EFT_DAY_WP  0
#define EFT_DAY_WN  1



/************************************************************************
**�ն������ݶ�2��Ϣ
*************************************************************************/
#define LEN_DAY_SEG2    128
#if(LEN_DAY_SEG2 > MAX_LEN_DAY_SEG)
    #error "day seg alloc err"
#endif

typedef union
{
    uint8 bytes[LEN_DAY_SEG2];
    struct
    {
        S_TimeLabel stime;      //12�ֽ�
        S_TimeLabel rtime;      //12�ֽ�
        uint32 dpp[5];                  //�����й�����      20�ֽ�
        uint32 dppt[5];                 //�����й�����ʱ��(AFMT)    20�ֽ�
        uint32 dnp[5];                  //�����й�����      20�ֽ�
        uint32 dnpt[5];                 //�����й�����ʱ��(AFMT)      20�ֽ�
        uint8 eft[4];   //4�ֽ�
        uint16 crc;     //2�ֽ�
    }seg;
}S_DAYDATA_SEG2;  //110


#define EFT_DAY_DPP   0
#define EFT_DAY_DPPT  1
#define EFT_DAY_DNP   2
#define EFT_DAY_DNPT  3









/************************************************************************
**�¶�������ģ��
*************************************************************************/
#define SIZE_MON_MONS    13
#define MAX_LEN_MON_SEG  MAX_LEN_DAY_SEG


/************************************************************************
**�¶������ݶ�0��Ϣ
*************************************************************************/
#define LEN_MON_SEG0     LEN_DAY_SEG0

#define S_MONDATA_SEG0   S_DAYDATA_SEG0

#define EFT_MONTH_WP     EFT_DAY_WP
#define EFT_MONTH_WN     EFT_DAY_WN


/************************************************************************
**�¶������ݶ�0��Ϣ
*************************************************************************/
#define LEN_MON_SEG2    LEN_DAY_SEG2

#define S_MONDATA_SEG2  S_DAYDATA_SEG2

#define EFT_MON_DPP     EFT_DAY_DPP
#define EFT_MON_DPPT    EFT_DAY_DPPT
#define EFT_MON_DNP     EFT_DAY_DNP
#define EFT_MON_DNPT    EFT_DAY_DNPT










/************************************************************************
**Сʱ��������ģ��
*************************************************************************/
#define SIZE_HOUR_HOURS     288       //12�� * 24Сʱ/��
#define MAX_LEN_HOUR_SEG     56



/************************************************************************
**Сʱ�������ݶ�0��Ϣ
*************************************************************************/
#define LEN_HOUR_SEG0    56
#if(LEN_HOUR_SEG0 > MAX_LEN_HOUR_SEG)
    #error "hour seg alloc err"
#endif

typedef union
{
    uint8 bytes[LEN_HOUR_SEG0];
    struct
    {
        S_TimeLabel stime;
        S_TimeLabel rtime;
        uint32 Wp;
        uint32 Wn;
        uint32 I;
        uint16 U[3];
        uint8 eft[2];
        uint16 crc;
    }seg;
}S_HOURDATA_SEG0;


#define EFT_HOUR_WP 0
#define EFT_HOUR_WN 1
#define EFT_HOUR_I  2
#define EFT_HOUR_U  3

/************************************************************************
**15�������߶�������ģ��
*************************************************************************/
#define SIZE_CUR_POINTS  26784          //(288* 31 * 3)(21* 24 * 4)
#define LEN_CUR_SEG0     92

typedef union                       
{
   uint8 bytes[LEN_CUR_SEG0];
   struct
   {
        S_TimeLabel stime;                  //12 �ֽ�
        uint32  rtime;                      //4 �ֽڣ�����ʱ��
        uint8_t dev;
        uint8_t factory_id[2];
        uint8_t factory_model[2];
        uint8_t ST[12];                       //�������    ��λ��0   S-standard  T-total
        uint8_t WT[8];                       //��������   working condition total
        uint8_t SI[8];                       //���˲ʱ��standard     insaneous quantity
        uint8_t WI[8];                       //����˲ʱ��   working    insaneous quantity
        uint8_t TMP[8];                       //�¶�
        uint8_t P[8];                       //ѹ�� 
        uint8 eft[2];                       //2�ֽ�
        uint16  crc;                        //2�ֽ�
   }seg;

   struct
   {
        S_TimeLabel stime;                  //12 �ֽ�
        uint32  rtime;                      //4 �ֽڣ�����ʱ��
        uint8_t dev;
        uint8_t factory_id[2];
        uint8_t factory_model[2];
        uint8_t pflag;          //Ԥ���ѹ���  1������0δ����
        uint8_t ST[6];                       //�������    ��λ��0   S-standard  T-total
        uint8_t RM[6];                       //ʣ����  remain money
        uint8_t UP[6];                       //��ǰ���� unit price
        uint8_t STT[4];                       //���״̬
        uint8_t vol;                       //�/���ص�ѹ voltage
        uint8_t per;                       //�/���ص����ٷֱ� percent
        uint8_t mflag;                  //�����Ƽ�����ݰ���1-��/0-û�У� monitor flag
        uint8_t ST1[6];                       //�������    ��λ��0   S-standard  T-total
        uint8_t WT[6];                       //��������   working condition total
        uint8_t SI[6];                       //���˲ʱ��standard     insaneous quantity
        uint8_t WI[6];                       //����˲ʱ��   working    insaneous quantity
        uint8_t TMP[6];                       //�¶�
        uint8_t P[6];                       //ѹ�� 
        
        uint8 eft[2];                       //2�ֽ�
        uint16  crc;                        //2�ֽ�
   }pseg;//prepay
}S_CURDATA_SEG0;        //92�ֽ�

#define EFT_MIN_WP  0
#define EFT_MIN_WPF 1
#define EFT_MIN_WN  2
#define EFT_MIN_WNF 3
#define EFT_MIN_I   4
#define EFT_MIN_IF  5
#define EFT_MIN_U   6
#define EFT_MIN_UF  7
//#define EFT_MIN_P   8

#define EFT_MIN_ST  0
#define EFT_MIN_WT 1
#define EFT_MIN_SI  2
#define EFT_MIN_WI 3
#define EFT_MIN_TMP   4
#define EFT_MIN_P  5
//	#define EFT_MIN_PST  6
//	#define EFT_MIN_PWT 7
//	#define EFT_MIN_PSI  8
//	#define EFT_MIN_PWI 9
//	#define EFT_MIN_PTMP   10
#define EFT_MIN_PRICE  11
#define EFT_MIN_RM 12
#define EFT_MIN_MSTT  13
#define EFT_MIN_VOLH  14//vol percent had flow

/************************************************************************
**�ն˵�ѹ�ɼ�15�������߶�������ģ��
*************************************************************************/
#define SIZE_QUAR_POINTS  2976          //(31* 24 * 4)
#define LEN_QUAR_SEG0     24

typedef union                       
{
   uint8 bytes[LEN_QUAR_SEG0];
   struct
   {
        S_TimeLabel stime;
        uint32  vol;
        uint16  crc;
   }seg;
}S_QUARDATA_SEG0;
 



/************************************************************************
**ͳ������ʷ��������ģ��
*************************************************************************/
#define SIZE_CBVOL_STAT    31
#define LEN_CBVOL_SEG0     128
     /************************************************************************ 
**���ܱ��ѹ����ͳ��
*************************************************************************/
typedef struct
{
     S_TimeLabel rtime;
     uint16  utop_mins[3];                      //��ѹԽ�������ۼ�ʱ��
     uint16  uup_mins[3];                       //��ѹԽ�����ۼ�ʱ��
     uint16  ucom_mins[3];                      //�ϸ��ѹ�ۼ�ʱ��
     uint16  udn_mins[3];                       //��ѹ�����ۼ�ʱ��
     uint16  ubtn_mins[3];                      //��ѹԽ�������ۼ�ʱ��
     uint32  Vmax[3];                           //����ѹ
     uint8   MaxTime[3][3];                     //����ѹ����ʱ��
     uint32  Vmin[3];                           //��С��ѹ
     uint8   MinTime[3][3];                     //��С��ѹ����ʱ��
     uint32  usum[3];                           //��ѹ�ۼӺ�.���ڼ���ƽ����ѹ
}S_CBStat;

typedef union
{
    uint8  bytes[LEN_CBVOL_SEG0];
    struct
    {
        S_TimeLabel stime;
        S_CBStat  bank1;
        uint16    crc;
    }seg;
}S_CBVOL_SEG0;
    




/************************************************************************
**���������ʵʱ����
*************************************************************************/
typedef struct
{
    uint32 Wp[5];                       //������\4�����й�����ʾֵ  hex4.2
    uint32 Wn[5];                       //������\4�����й�����ʾֵ  hex4.2
    uint32 I[3];                        //A�����                   hex4.3
    uint16 U[3];                        //A���ѹ                   hex2.4
    int32  P;                           //�ܹ���                    shex4.4
    uint8  time[6];                     //���ܱ�����ʱ��            6���ֽ�
    uint8  evsta[4];                    //�¼�״̬      4�ֽ�
    uint8  eft[5];                      //״̬��                    BS
    uint8  errcnt;                      //����ʧ�ܴ���
    S_TimeLabel rtime;                  //����ʱ��

}S_TstRealData;
typedef struct
{
    uint8  mGateIn;                     //ң������.��λ0:����.1����
    uint8  mState;                      //��ǰ���¼�״̬
    uint8  mRaise;                      //�����¼�״̬λ
    uint8  mFall;                       //�ָ��¼�״̬λ
    uint8  mValid;                      //��ǰ״̬�Ƿ���Ч
    uint8  mb;                          //״̬����λ��־BS8
}MKStruct;


EXT_HLV MKStruct gs_MKStruct;


EXT_HLV S_TstRealData gss_TstRealData[CORE_TST_NO]; //������ʵʱ����

EXT_HLV uint8 guc_StopCnt[CORE_TST_NO];  //���ܱ�ͣ��ʱ�������

#define ADDR_WP     (uint32)((uint8*)gss_TstRealData[0].Wp - (uint8*)&gss_TstRealData[0])
#define ADDR_WN     (uint32)((uint8*)gss_TstRealData[0].Wn - (uint8*)&gss_TstRealData[0])
#define ADDR_U      (uint32)((uint8*)gss_TstRealData[0].U - (uint8*)&gss_TstRealData[0])
#define ADDR_I      (uint32)((uint8*)gss_TstRealData[0].I - (uint8*)&gss_TstRealData[0])
#define ADDR_TIME   (uint32)((uint8*)&gss_TstRealData[0].time - (uint8*)&gss_TstRealData[0])
#define ADDR_P      (uint32)((uint8*)&gss_TstRealData[0].P - (uint8*)&gss_TstRealData[0])
#define ADDR_EVS    (uint32)((uint8*)&gss_TstRealData[0].evsta - (uint8*)&gss_TstRealData[0])


/************************************************************************ 
**ʵʱ������Чλ����
*************************************************************************/
#define EFT_WP      (0)
#define EFT_WN      (EFT_WP + 5)
#define EFT_U       (EFT_WN + 5)
#define EFT_I       (EFT_U + 3)
#define EFT_TIME    (EFT_I + 3)
#define EFT_P       (EFT_TIME + 1)
#define EFT_EVS     (EFT_P + 1)




/************************************************************************
**��������ĸ�������
*************************************************************************/
//	typedef struct
//	{
//	    uint32 Wp[4];                       //������\�������й�����ʾֵ  hex4.2
//	    uint32 Wn[4];                       //������\�������й�����ʾֵ  hex4.2
//	    uint16 U[9];                        //���ѹ                   hex2.4
//	    uint16 I[9];                        //�����                   hex4.3
//	    int32  P[2];                        //�ܹ���                    shex4.4
//	    uint8  eft[8];                      //��Чλ
//	    uint8  ymdhm[5];                    //��ȡʱ��
//	}S_TstCurveData;
#pragma pack(1)
typedef struct
{
    uint8_t dev;
    uint8_t factory_id[2];
    uint8_t factory_model[2];
    uint8_t ST[12];                       //�������    ��λ��0   S-standard  T-total
    uint8_t WT[8];                       //��������   working condition total
    uint8_t SI[8];                       //���˲ʱ��standard     insaneous quantity
    uint8_t WI[8];                       //����˲ʱ��   working    insaneous quantity
    uint8_t TMP[8];                       //�¶�
    uint8_t P[8];                       //ѹ�� 
    uint8  ymdhm[5];                    //��ȡʱ��
    uint8_t RM[6];                       //ʣ����  remain money
    uint8_t UP[6];                       //��ǰ���� unit price
    uint8_t STT[4];                       //���״̬
    uint8_t vol;                       //�/���ص�ѹ voltage
    uint8_t per;                       //�/���ص����ٷֱ� percent
    uint8_t mflag;                  //�����Ƽ�����ݰ���1-��/0-û�У� monitor flag


    
    uint8  eft[8];                      //��Чλ
}S_TstCurveData;

//	typedef struct
//	{
//	    uint8_t dev;
//	    uint8_t factory_id[2];
//	    uint8_t factory_model[2];
//	    uint8_t pflag;          //Ԥ���ѹ���  1������0δ����
//	    uint8_t ST[6];                       //�������    ��λ��0   S-standard  T-total
//	    uint8_t RM[6];                       //ʣ����  remain money
//	    uint8_t UP[6];                       //��ǰ���� unit price
//	    uint8_t STT[4];                       //���״̬
//	    uint8_t vol;                       //�/���ص�ѹ voltage
//	    uint8_t per;                       //�/���ص����ٷֱ� percent
//	    uint8_t mflag;                  //�����Ƽ�����ݰ���1-��/0-û�У� monitor flag
//	    uint8_t ST1[6];                       //�������    ��λ��0   S-standard  T-total
//	    uint8_t WT[6];                       //��������   working condition total
//	    uint8_t SI[6];                       //���˲ʱ��standard     insaneous quantity
//	    uint8_t WI[6];                       //����˲ʱ��   working    insaneous quantity
//	    uint8_t TMP[6];                       //�¶�
//	    uint8_t P[6];                       //ѹ�� 
//	
//	    uint8 eft[8];                       //2�ֽ�
//	}S_TstCurPreData;


#pragma pack(4)

EXT_HLV S_TstCurveData gss_TstCurveData[CORE_TST_NO]; //������ʵʱ����

//	#define ADDR_CUR_WP     (uint32)((uint8*)gss_TstCurveData[0].Wp - (uint8*)&gss_TstCurveData[0])
//	#define ADDR_CUR_WN     (uint32)((uint8*)gss_TstCurveData[0].Wn - (uint8*)&gss_TstCurveData[0])
//	#define ADDR_CUR_U      (uint32)((uint8*)gss_TstCurveData[0].U - (uint8*)&gss_TstCurveData[0])
//	#define ADDR_CUR_I      (uint32)((uint8*)gss_TstCurveData[0].I - (uint8*)&gss_TstCurveData[0])
//	#define ADDR_CUR_P      (uint32)((uint8*)&gss_TstCurveData[0].P - (uint8*)&gss_TstCurveData[0])



/************************************************************************ 
**��������������Чλ����
*************************************************************************/
#define EFT_CUR_WP      (0)
#define EFT_CUR_WN      (EFT_CUR_WP + 4)
#define EFT_CUR_U       (EFT_CUR_WN + 4)
#define EFT_CUR_I       (EFT_CUR_U + 9)
#define EFT_CUR_P       (EFT_CUR_I + 9)



/************************************************************************
**�����������������
*************************************************************************/
//	typedef struct
//	{
//	    uint32 dpp[5];                  //�����й�����
//	    uint32 dppt[5];                 //�����й�����ʱ��(AFMT)
//	    uint32 dnp[5];                  //�����й�����
//	    uint32 dnpt[5];                 //�����й�����ʱ��
//	    uint8  eft[4];                  //��Чλ
//	    uint8  ymdhm[5];                //��ȡʱ��
//	}S_TstDmdData;

//	EXT_HLV S_TstDmdData gs_tstdmd[MCB_PORT_NUM];//�洢ʱʹ��
//	
//	#define ADDR_DMD_DPP     (uint32)((uint8*)gs_tstdmd[0].dpp  - (uint8*)&gs_tstdmd[0])
//	#define ADDR_DMD_DPPT    (uint32)((uint8*)gs_tstdmd[0].dppt - (uint8*)&gs_tstdmd[0])
//	#define ADDR_DMD_DNP     (uint32)((uint8*)gs_tstdmd[0].dnp  - (uint8*)&gs_tstdmd[0])
//	#define ADDR_DMD_DNPT    (uint32)((uint8*)gs_tstdmd[0].dnpt - (uint8*)&gs_tstdmd[0])
//	
//	#define EFT_DMD_DPP       0
//	#define EFT_DMD_DPPT      1
//	#define EFT_DMD_DNP       2
//	#define EFT_DMD_DNPT      3


/************************************************************************
**��������ĵ��¼�����
*************************************************************************/
typedef struct
{
   uint32 pcnt;                           //��̴���                  bin
   uint8  progt[6];                       //���һ�α��ʱ�� 
   uint8  htime[6];                       //���һ�κ�բʱ��
   uint8  dtime[6];                       //���һ�ο�բʱ��
   uint32 dncnt;                          //����ť�д���              
   uint32 wgcnt;                          //����β�Ǵ���              bin
   uint8  wgtime[6];                      //���һ�ο���β��ʱ��
   uint32 jscnt;                          //���һ��Уʱ����          bin
   uint8  jstime[12];                     //Уʱǰ��ʱ��
   uint32 dpcnt;                          //ʱ�α��̴���            bin
   uint8  dptime[6];                      //ʱ�α����һ�ζ�ʱ����ʱ��
   uint32 demdcnt;                        //�������������
   uint8  demdt[6];                       //���������ʱ��
   uint8  ftn[24];                        //����ʱ��                  
   uint16 dh[3];                          //������                    97:I0
   uint32 imp0;                           //�й�����                  bin
   uint32 imp1;                           //�޹�����                  bin
   uint32 dxn[4];                         //�������                   
   uint32 tdcnt;                          //ͣ�����
   uint32 sycnt;                          //ʧѹ����
   uint8  bstt[14];                       //���״̬��
   uint32 ccount;                         //�㶨�ų������ܴ���
   uint32 ptbb;                           //��ѹ���������
   uint32 ctbb;                           //�������������
   uint32 baty;                           //��ع���ʱ��              bin
   uint8  eft[6];                         //��Ч���ݱ�־
   uint8  ymdhm[5];                       //����ʱ��
}S_EvtRealData;

EXT_HLV S_EvtRealData gs_TstEvtData[MCB_PORT_NUM];   //����ʱʹ��

#define ADDR_EVT_PCNT          (uint32)((uint8*)&gs_TstEvtData[0].pcnt     -   (uint8*)&gs_TstEvtData[0])
#define ADDR_EVT_PROGT         (uint32)((uint8*)&gs_TstEvtData[0].progt    -   (uint8*)&gs_TstEvtData[0])
#define ADDR_EVT_HTIME         (uint32)((uint8*)&gs_TstEvtData[0].htime    -   (uint8*)&gs_TstEvtData[0])
#define ADDR_EVT_DTIME         (uint32)((uint8*)&gs_TstEvtData[0].dtime    -   (uint8*)&gs_TstEvtData[0])
#define ADDR_EVT_DNCNT         (uint32)((uint8*)&gs_TstEvtData[0].dncnt    -   (uint8*)&gs_TstEvtData[0])
#define ADDR_EVT_WGCNT         (uint32)((uint8*)&gs_TstEvtData[0].wgcnt    -   (uint8*)&gs_TstEvtData[0])
#define ADDR_EVT_WGTIME        (uint32)((uint8*)&gs_TstEvtData[0].wgtime   -   (uint8*)&gs_TstEvtData[0])
#define ADDR_EVT_JSCNT         (uint32)((uint8*)&gs_TstEvtData[0].jscnt    -   (uint8*)&gs_TstEvtData[0])
#define ADDR_EVT_JSTIME        (uint32)((uint8*)&gs_TstEvtData[0].jstime   -   (uint8*)&gs_TstEvtData[0])
#define ADDR_EVT_DPCNT         (uint32)((uint8*)&gs_TstEvtData[0].dpcnt    -   (uint8*)&gs_TstEvtData[0])
#define ADDR_EVT_DPTIME        (uint32)((uint8*)&gs_TstEvtData[0].dptime   -   (uint8*)&gs_TstEvtData[0])
#define ADDR_EVT_DEMDCNT       (uint32)((uint8*)&gs_TstEvtData[0].demdcnt  -   (uint8*)&gs_TstEvtData[0])
#define ADDR_EVT_DEMDT         (uint32)((uint8*)&gs_TstEvtData[0].demdt    -   (uint8*)&gs_TstEvtData[0])
#define ADDR_EVT_FTN           (uint32)((uint8*)&gs_TstEvtData[0].ftn      -   (uint8*)&gs_TstEvtData[0])
#define ADDR_EVT_DH            (uint32)((uint8*)&gs_TstEvtData[0].dh       -   (uint8*)&gs_TstEvtData[0])
#define ADDR_EVT_IMP0          (uint32)((uint8*)&gs_TstEvtData[0].imp0     -   (uint8*)&gs_TstEvtData[0])
#define ADDR_EVT_IMP1          (uint32)((uint8*)&gs_TstEvtData[0].imp1     -   (uint8*)&gs_TstEvtData[0])
#define ADDR_EVT_DXN           (uint32)((uint8*)&gs_TstEvtData[0].dxn      -   (uint8*)&gs_TstEvtData[0])
#define ADDR_EVT_TDCNT         (uint32)((uint8*)&gs_TstEvtData[0].tdcnt    -   (uint8*)&gs_TstEvtData[0])
#define ADDR_EVT_SYCNT         (uint32)((uint8*)&gs_TstEvtData[0].sycnt    -   (uint8*)&gs_TstEvtData[0])
#define ADDR_EVT_BSTT          (uint32)((uint8*)&gs_TstEvtData[0].bstt     -   (uint8*)&gs_TstEvtData[0])
#define ADDR_EVT_CCOUNT        (uint32)((uint8*)&gs_TstEvtData[0].ccount   -   (uint8*)&gs_TstEvtData[0])
#define ADDR_EVT_PTBB          (uint32)((uint8*)&gs_TstEvtData[0].ptbb     -   (uint8*)&gs_TstEvtData[0])
#define ADDR_EVT_CTBB          (uint32)((uint8*)&gs_TstEvtData[0].ctbb     -   (uint8*)&gs_TstEvtData[0])
#define ADDR_EVT_BATY          (uint32)((uint8*)&gs_TstEvtData[0].baty     -   (uint8*)&gs_TstEvtData[0])


#define EFT_EVT_PCNT               (0)
#define EFT_EVT_HTIME              (EFT_EVT_PCNT + 1)
#define EFT_EVT_DTIME              (EFT_EVT_HTIME + 1)
#define EFT_EVT_PROGT              (EFT_EVT_DTIME + 1)
#define EFT_EVT_DNCNT              (EFT_EVT_PROGT + 1)
#define EFT_EVT_WGCNT              (EFT_EVT_DNCNT + 1)
#define EFT_EVT_WGTIME             (EFT_EVT_WGCNT + 1)
#define EFT_EVT_JSCNT              (EFT_EVT_WGTIME + 1)
#define EFT_EVT_JSTIME             (EFT_EVT_JSCNT + 1)
#define EFT_EVT_DPCNT              (EFT_EVT_JSTIME + 1)
#define EFT_EVT_DPTIME             (EFT_EVT_DPCNT + 1)
#define EFT_EVT_DEMDCNT            (EFT_EVT_DPTIME + 1)
#define EFT_EVT_DEMDT              (EFT_EVT_DEMDCNT + 1)
#define EFT_EVT_FTN                (EFT_EVT_DEMDT + 1)
#define EFT_EVT_DH                 (EFT_EVT_FTN + 1)
#define EFT_EVT_IMP0               (EFT_EVT_DH + 3)
#define EFT_EVT_IMP1               (EFT_EVT_IMP0 + 1)
#define EFT_EVT_DXN                (EFT_EVT_IMP1 + 1)
#define EFT_EVT_TDCNT              (EFT_EVT_DXN + 1)
#define EFT_EVT_SYCNT              (EFT_EVT_TDCNT + 1)
#define EFT_EVT_BSTT               (EFT_EVT_SYCNT + 1)
#define EFT_EVT_CCOUNT             (EFT_EVT_BSTT + 1)
#define EFT_EVT_PTBB               (EFT_EVT_CCOUNT + 1)
#define EFT_EVT_CTBB               (EFT_EVT_PTBB + 1)
#define EFT_EVT_BATY               (EFT_EVT_CTBB + 1)



/************************************************************************
**07��Լ������ı��ڶ�������
*************************************************************************/
typedef struct
{
    uint8  Tsd[5];                      //07���ϴ��ն���ʱ��        mhDMY(BCD)
    uint32 Wpd[5];                      //07������������\4�����й�����ʾֵ  hex4.2
    uint32 Wnd[5];                      //07�����շ�����\4�����й�����ʾֵ  hex4.2
    uint8  Tsd2[5];                     //07���϶��ն���ʱ��
    uint32 Wpd2[5];                     //07������������\4�����й�����ʾֵ  hex4.2
    uint32 Wnd2[5];                     //07�����շ�����\4�����й�����ʾֵ  hex4.2
    uint8  Tsd3[5];                     //07���϶��ն���ʱ��
    uint32 Wpd3[5];                     //07������������\4�����й�����ʾֵ  hex4.2
    uint32 Wnd3[5];                     //07�����շ�����\4�����й�����ʾֵ  hex4.2
    uint8  eft[6];
    S_TimeLabel rtime;

}S_Tst07ExtraData;

EXT_HLV S_Tst07ExtraData gss_Tst07ExtraData;

#define ADDR_EXT_TSD     (uint32)((uint8*)gss_Tst07ExtraData.Tsd - (uint8*)&gss_Tst07ExtraData)
#define ADDR_EXT_WPD     (uint32)((uint8*)gss_Tst07ExtraData.Wpd - (uint8*)&gss_Tst07ExtraData)
#define ADDR_EXT_WND     (uint32)((uint8*)gss_Tst07ExtraData.Wnd - (uint8*)&gss_Tst07ExtraData)
#define ADDR_EXT_TSD2    (uint32)((uint8*)gss_Tst07ExtraData.Tsd2 - (uint8*)&gss_Tst07ExtraData)
#define ADDR_EXT_WPD2    (uint32)((uint8*)gss_Tst07ExtraData.Wpd2 - (uint8*)&gss_Tst07ExtraData)
#define ADDR_EXT_WND2    (uint32)((uint8*)gss_Tst07ExtraData.Wnd2 - (uint8*)&gss_Tst07ExtraData)
#define ADDR_EXT_TSD3    (uint32)((uint8*)gss_Tst07ExtraData.Tsd3 - (uint8*)&gss_Tst07ExtraData)
#define ADDR_EXT_WPD3    (uint32)((uint8*)gss_Tst07ExtraData.Wpd3 - (uint8*)&gss_Tst07ExtraData)
#define ADDR_EXT_WND3    (uint32)((uint8*)gss_Tst07ExtraData.Wnd3 - (uint8*)&gss_Tst07ExtraData)

#define EFT_EXT_TSD       0
#define EFT_EXT_WPD       (EFT_EXT_TSD + 1)
#define EFT_EXT_WND       (EFT_EXT_WPD + 5)
#define EFT_EXT_TSDS      (EFT_EXT_WND + 5)
#define EFT_EXT_WPDS      (EFT_EXT_TSDS + 1)
#define EFT_EXT_WNDS      (EFT_EXT_WPDS + 5)
#define EFT_EXT_TSDT      (EFT_EXT_WNDS + 5)
#define EFT_EXT_WPDT      (EFT_EXT_TSDT + 1)
#define EFT_EXT_WNDT      (EFT_EXT_WPDT + 5)


/************************************************************************
**DLMS��Լ������ı��ڶ�������
*************************************************************************/
typedef struct
{
    uint32 Wp[5];                      //DLMS������������\4�����й�����ʾֵ  hex4.2
    uint32 Wn[5];                      //DLMS�����շ�����\4�����й�����ʾֵ  hex4.2
    uint8  eft[2];
    S_TimeLabel rtime;

}S_TstDLMSExtraData;

EXT_HLV S_TstDLMSExtraData gss_TstDLMSExtraData;


#define ADDR_DLMS_EXT_WP     (uint32)((uint8*)gss_TstDLMSExtraData.Wp - (uint8*)&gss_TstDLMSExtraData)
#define ADDR_DLMS_EXT_WN     (uint32)((uint8*)gss_TstDLMSExtraData.Wn - (uint8*)&gss_TstDLMSExtraData)


#define EFT_DLMS_EXT_WP       0
#define EFT_DLMS_EXT_WN       (EFT_DLMS_EXT_WP + 5)



/************************************************************************
**�������
*************************************************************************/
typedef enum
{
    DTP_UNVALID = 0 ,
    DTP_WP          ,
    DTP_WN          ,
    DTP_U           ,
    DTP_I           ,
    DTP_TIME        ,
    DTP_P           ,
    DTP_EVS         ,
    DTP_DMD_DPP     ,
    DTP_DMD_DPPT    ,
    DTP_DMD_DNP     ,
    DTP_DMD_DNPT    ,
    DTP_EVT_PCNT    ,
    DTP_EVT_HTIME   ,
    DTP_EVT_DTIME   ,
    DTP_EVT_PROGT   ,
    DTP_EVT_DNCNT   ,
    DTP_EVT_WGCNT   ,
    DTP_EVT_WGTIME  ,
    DTP_EVT_JSCNT   ,
    DTP_EVT_JSTIME  ,
    DTP_EVT_DPCNT   ,
    DTP_EVT_DPTIME  ,
    DTP_EVT_DEMDCNT ,
    DTP_EVT_DEMDT   ,
    DTP_EVT_FTN     ,
    DTP_EVT_DH      ,
    DTP_EVT_IMP0    ,
    DTP_EVT_IMP1    ,
    DTP_EVT_DXN     ,
    DTP_EVT_TDCNT   ,
    DTP_EVT_SYCNT   ,
    DTP_EVT_BSTT    ,
    DTP_EVT_CCOUNT  ,
    DTP_EVT_PTBB    ,
    DTP_EVT_CTBB    ,
    DTP_EVT_BATY    ,
    DTP_EXT_TSD     ,
    DTP_EXT_WPD     ,
    DTP_EXT_WND     ,
    DTP_EXT_TSDS    ,
    DTP_EXT_WPDS    ,
    DTP_EXT_WNDS    ,
    DTP_EXT_TSDT    ,
    DTP_EXT_WPDT    ,
    DTP_EXT_WNDT    ,
}e_DItype;











/************************************************************************
 * @function: UpdateSysTimeStruct
 * @����: ����gs_SysTime��gs_SysLastDayTime
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2013/10/30)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void UpdateSysTimeStruct(void);


/*********************************************************************************************************
** ��������: void HLV_PreInit(void* pdata)
** ��������: �߼�ɨ����̵ĳ�ʼ�������������þ���Ӧ�ó�ʼ�����Ӻ���
** �䡡��:
            pdata û������

** �䡡��: ��
**
** ȫ�ֱ���: ��
** ����ģ��: ��
**
** ������: yzy
** �ա���: 2007��9��4��
**-------------------------------------------------------------------------------------------------------
** �޸���:
** �ա���:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void HLV_PreInit(void);
#endif


