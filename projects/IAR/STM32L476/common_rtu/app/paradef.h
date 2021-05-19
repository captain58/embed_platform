/****************************************Copyright (c)**************************************************
**                                  ____�Ƽ����޹�˾
**                                          ������
**
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: paradef.h
**��   ��   ��: yzy
**����޸�����: 2019-06-18
**��        ��: �������ö�����ͷ�ļ�
**ע        ��:
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: yzy
** ��  ��: v1.0
** �ա���: 2019-06-18
** �衡��: ԭʼ�汾
********************************************************************************************************/
#ifndef _PARADEF_H_
#define _PARADEF_H_




/************************************************************************ 
**���������Ų���(�ܳ���)
*************************************************************************/
//	#define  F1_PLEN        6
//	#define  F2_PLEN        33
//	#define  F3_PLEN        28
//	#define  F4_PLEN        16
//	#define  F5_PLEN        3 
//	#define  F6_PLEN        16
//	#define  F7_PLEN        64
//	#define  F8_PLEN        8 
//	#define  F9_PLEN        16
//	#define  F10_PLEN       6000
//	#define  F11_PLEN       11
//	#define  F12_PLEN       2 
//	#define  F13_PLEN       7 
//	#define  F14_PLEN       41
//	#define  F15_PLEN       37
//	#define  F16_PLEN       64
//	#define  F17_PLEN       2 
//	#define  F18_PLEN       12
//	#define  F19_PLEN       1 
//	#define  F20_PLEN       1 
//	#define  F21_PLEN       49
//	#define  F22_PLEN       193
//	#define  F23_PLEN       3 
//	#define  F25_PLEN       11
//	#define  F26_PLEN       57
//	#define  F27_PLEN       24
//	#define  F28_PLEN       4
//	#define  F29_PLEN       12
//	#define  F30_PLEN       1
//	#define  F31_PLEN       121
//	#define  F33_PLEN       110
//	#define  F34_PLEN       19
//	#define  F35_PLEN       41
//	#define  F36_PLEN       4
//	#define  F37_PLEN       19
//	#define  F38_PLEN       33          //8448
//	#define  F39_PLEN       33          //8448
//	#define  F41_PLEN       17
//	#define  F42_PLEN       6
//	#define  F43_PLEN       1
//	#define  F44_PLEN       8
//	#define  F45_PLEN       1
//	#define  F46_PLEN       4
//	#define  F47_PLEN       17
//	#define  F48_PLEN       1
//	#define  F49_PLEN       1
//	#define  F57_PLEN       3
//	#define  F58_PLEN       1
//	#define  F59_PLEN       5
//	#define  F60_PLEN       80
//	#define  F65_PLEN       (TASK_PARA_LEN + 9)
//	#define  F66_PLEN       (TASK_PARA_LEN + 9)
//	#define  F67_PLEN       1
//	#define  F68_PLEN       1
//	#define  F70_PLEN       6
//	#define  F91_PLEN       10
//	
//	
//	
//	#define  F149_PLEN      1
//	#define  F150_PLEN      512
//	#define  F239_PLEN      6000
//	#define  F241_PLEN      1
//	#define  F246_PLEN      13
//	
//	#define  F251_PLEN      4 
//	#define  F252_PLEN      16
//	#define  F253_PLEN      6
//	#define  F254_PLEN      2
//	#define  F255_PLEN      4
//	#define  F256_PLEN      8
//	
//	
//	/************************************************************************ 
//	**������ַ�Ų���
//	*************************************************************************/
//	//************************�Զ������***************************************
//	#define F70_PADDR             0        //MAC��ַ
//	#define F251_PADDR            10       //�ն��߼���ַ
//	#define F252_PADDR            20       //�ն�ͨѶ����
//	#define INPADDR               30       //�������к�,�����ַ
//	#define ZKVADDR               40       //�ͻ����汾(4B HEX)
//	#define K_VCF_PUL             50       //У��ʱ��ʵʱ������
//	#define K_VCF_VOL             60       //У��ʱ��ʵʱ��ѹ
//	
//	#define VER_HEXING            70       //�ɼ��ڲ�����汾��
//	
//	//************************�ն˲���***************************************
//	#define F1_PADDR            100         //�ն�ͨ�Ų���(LEN=6)
//	#define F2_PADDR            150         //�м�ת������(MAX<50)
//	#define F3_PADDR            200         //��վIP��ַ�Ͷ˿�(LEN=28)
//	#define F4_PADDR            250         //��վ�Ͷ������ĺ���(LEN=16)
//	#define F5_PADDR            300         //��Ϣ��֤����(LEN=3)
//	#define F6_PADDR            350         //���ַ(MAX<50)
//	#define F7_PADDR            400         //�ն�IP��ַ�Ͷ˿ں�(MAX<100)
//	#define F8_PADDR            500         //�ն�����ͨ�Ź�����ʽ(LEN=8)
//	#define F9_PADDR            550         //�¼���¼����(LEN=16)
//	#define F11_PADDR           600         //����������(MAX<350)
//	#define F12_PADDR           950         //״̬������(LEN=2)
//	#define F13_PADDR           960         //ģ��������(MAX<=7)
//	#define F14_PADDR           1000        //�ܼ�������(MAX<700)
//	#define F15_PADDR           1700        //���Խ�޲���(MAX<100)
//	#define F16_PADDR           1800        //����ר���û���\����(LEN=64)
//	#define F17_PADDR           1900        //������ֵ(LEN=2)
//	#define F18_PADDR           1950        //����ʱ��(LEN=12)
//	#define F19_PADDR           2000        //ʱ�οظ���ϵ��(LEN=1)
//	#define F20_PADDR           2050        //�µ�ظ���ϵ��(LEN=1)
//	#define F21_PADDR           2100        //����ʱ�κͷ�����(LEN=49)
//	#define F22_PADDR           2200        //����(MAX<200)
//	#define F23_PADDR           2400        //�߷Ѹ澯����/��ֹ(LEN=3)
//	
//	
//	//************************���������***************************************
//	#define F25_PADDR           2500        //�������������(SLEN=11,STST=192)
//	#define F26_PADDR           4700        //��������ֵ����(SLEN=57,STST=192)
//	#define F27_PADDR          16000        //ͭ��,�������(SLEN=24)
//	#define F28_PADDR          21000        //�����㹦�����طֶ���ֵ(SLEN=4)
//	#define F30_PADDR          21800        //ͣ������(SLEN=1)
//	
//	
//	//************************�ն˲���***************************************
//	#define F33_PADDR           22000       //�ն˳������(MAX<2200)
//	#define F34_PADDR           24400       //�ն�ͨ��ģ�����(MAX<200)
//	#define F35_PADDR           24600       //̨���ص��û�����(MAX<100)
//	#define F36_PADDR           24700       //�ն�ͨ������(LEN=2)
//	#define F37_PADDR           24750       //�ն˼���ͨ�Ų���(MAX<100)
//	#define F38_PADDR           24900       //1��������������(MAX< (33*16*16))
//	#define F39_PADDR           34000       //2��������������(MAX< (33*16*16))
//	
//	
//	//************************�ն˲���***************************************
//	#define F57_PADDR           43000       //�����澯ʹ��(LEN=3)
//	#define F58_PADDR           43050       //����������ͨ��ʱ��(LEN=1)
//	#define F59_PADDR           43060       //���ܱ��쳣�б�ֵ(LEN=4)
//	#define F60_PADDR           43070       //г����ֵ(LEN=80)
//	
//	
//	//************************�ն˲���***************************************
//	#define F91_PADDR           43200       //�ն˵�λ����Ϣ(LEN=10)
//	#define F150_PADDR          43250       //������״̬(LEN = 512)
//	#define F241_PADDR          44000       //���ܱ�ͨ�Ų����Զ�ά�����ܿ�����ر�(AFN05F149)(LEN = 1)
//	#define F246_PADDR          44050       //�������Ա�ƶ�ʱ��ʱ����(AFN04F246)(LEN = 7)
//********************************************************************
//����

//********************************************************************
#define PREPAYPARA_PLEN     200
#define RECHARGE_PLEN     (27*8)
#define PRICE_PLEN     (27*8)

#define     PARA_COPY_INTERVAL_DLEN 2
#define     NETP_FW_PLEN      (sizeof(ST_NETP_FW_BASE)) 
#define     NETP_FW_PART_PLEN      (sizeof(ST_Netp_FW)) 

#define     PREPAYPARA_PER_LEN 25
//Ԥ��64ֻ��λ��
#define     F10_PLEN        (64+2+64)
#define     PARA_GPRS_PLEN        96
#define     PARA_FTP_PLEN        232
#define     PARA_HB_LEN         2

#define F10_PADDR               100       //������  8*8 = 64
#define PARA_COPY_INTERVAL      (F10_PADDR+F10_PLEN)        
#define PARA_GPRS               (PARA_COPY_INTERVAL+PARA_COPY_INTERVAL_DLEN)        
#define PARA_FTP                (PARA_GPRS+PARA_GPRS_PLEN)        
#define PARA_HB                 (PARA_FTP+PARA_FTP_PLEN)      ///����  

#define PREPAYPARA_PADDR        1000       //Ԥ����

#define RECHARGE_PADDR          (PREPAYPARA_PADDR+PREPAYPARA_PLEN)        //Ԥ����

#define PRICE_PADDR             (RECHARGE_PADDR+RECHARGE_PLEN)        //Ԥ����




#define NETP_FW_PADDR       0

#define     RUNPARA_FTP_PLEN        sizeof(S_FTPPARA)

#define     RUNPARA_FTP     0       

//	#define F31_PADDR           50400       //�ز��ӽڵ㸽���ڵ�(SMAX<121,���ù���ռ�,��ֻ֧�ֵ�������������)
//	
//	
//	//************************�������***************************************
//	#define F67_PADDR           51000       //1��������ͣ
//	#define F68_PADDR           51100       //2��������ͣ
//	#define F65_PADDR           51200       //1���������
//	#define F66_PADDR          117600       //2���������
//	#define FXX_PADDR          184000       //END
//	
//	#define F239_PADDR          184000       //���ܱ�/�������� ����  132*24 = 3168












#endif

