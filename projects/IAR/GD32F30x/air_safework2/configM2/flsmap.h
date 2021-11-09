/****************************************Copyright (c)*************************
**                               	________�Ƽ����޹�˾
**                                     		������
**
**
**--------------�ļ���Ϣ-------------------------------------------------------
**��   ��   ��: flsmap.h
**��   ��   ��: yzy
**����޸�����: 2014��07��25��
**��        ��: fls��������
**ע        ��:
**--------------��ʷ�汾��Ϣ---------------------------------------------------
** ������: yzy
** ��  ��: v1.0
** �ա���: 2014��07��25��
** �衡��: ԭʼ�汾
******************************************************************************/
#ifndef _FLSMAP_H_
#define _FLSMAP_H_


/******************************************************************************
**Ƭ��flash��غ궨��
******************************************************************************/
//boot������
#define  FLS_START_ADDR         0x00000000
#define  FLS_ENTRYFUNC_ADDR     0x00000100              //__iar_program_start()ָ��
#define  FLS_BOOTVER_ADDR       0x00000104              //bootloader�汾��

//usr����Χ����
#define  FLS_APPSTART_ADDR      0x08006000
#define  FLS_APPEND_ADDR        0x0007FFFF

//usr������
#define  FLS_USRMARKA_ADDR      (FLS_APPSTART_ADDR + 0x200) //�û�������Գ�����Ч�Ա�־
#define  FLS_USRMARKB_ADDR      (FLS_APPSTART_ADDR + 0x204) //�û�������Գ�����Ч�Ա�־
#define  FLS_USRPROGLEN_ADDR    (FLS_APPSTART_ADDR + 0x208) //�û����򳤶�
#define  FLS_USRVER_ADDR        (FLS_APPSTART_ADDR + 0x20C) //�û�����汾��
#define  FLS_USRPRJ_ADDR        (FLS_APPSTART_ADDR + 0x210) //�û����̱���(6�ֽ�)

#define FLS_WDTTST_ADDR   32                //��ſ��Ź����Ե��ϱ���Ϣ���ϱ�����(���ݳ������256�ֽ�)



#define FLS_CHIP_NO     1
#define FLASH1_SIZE     0x2000000        //32MM
#define FLASH2_SIZE     0x200000        //2M 2010-01-10


#if SYS_FILE_EN > 0
    #define SYS_IAP_EN   1
#else
    #define SYS_IAP_EN   0
#endif

/******************************************************************************
**flash��ź�
******************************************************************************/
//ע��:��Ҫ���ʼ���ļ�һ��
#define DB_CODE                 0 

#define DB_UPDATE               1               //����
//	#define DB_FACTORY              1               //��������

#define DB_PARA                 2               //����
#define DB_DEV                  3
#define DB_UPDATA_PARA          3               //ģ����������
#define DB_RUN_PARA             4               //���в���
#define DB_MON_FREEZE           5               //�¶���
#define DB_EVENT                6               //�¼���¼
#define DB_USER_DEFINE          7               //�û��Զ���
#define DB_EVENT_INDEX          8               //�¼�����
#define DB_HOUR_FREEZE          9               //Сʱ����
#define DB_RECHARGE_INDEX       10              //��ֵ����
#define DB_RECHARGE             11              //��ֵ�¼���¼
#define DB_DAY_FREEZE_LAST      12              //��������ն���
#define DB_PARA_NETP            13               //����
#define DB_CURE                 14                              //������������ļ���
#define DB_HASH1                15
#define DB_HASH2                16
#ifdef _USR_FLASH                        //Ӧ�ò����ļ��ж���

/******************************************************************************
**���ݿ��ż���Ӧ��С���ݿ���
******************************************************************************/
__root const S_FILEBLOCKS gss_FileBlocks[] = 
{
    //��ʼ�ļ���, ͬ����, ��С���ݿ���
    
    {DB_CODE,         0, 52, TDB_MODE_RW},    //Զ������
    {DB_UPDATE,         0, 48, TDB_MODE_RW},    //Զ������
//	    {DB_FACTORY,        0, 2, TDB_MODE_RW},     //��������    
    
    {DB_PARA,           0, 2,  TDB_MODE_RW},    //����
    {DB_DEV,            0, 2,  TDB_MODE_RW},
    {DB_HASH1,            0, 1,  TDB_MODE_RW},    
    {DB_HASH2,            0, 1,  TDB_MODE_RW},    

};

const uint8 LEN_DB_LIST = (sizeof(gss_FileBlocks) / sizeof(S_FILEBLOCKS));

#endif
/******************************************************************************
 *�ļ��Լ����ݿ��������
******************************************************************************/
#if (SYS_FILE_EN > 0)
    #define SYS_FILE_DYM_EN  0

    #define DB_LEN_BLOCK(X)    ((X) << 11)     //X�εĳ���

    #if SYS_FILE_DYM_EN > 0
    #define DB_DYMI_DB          4               //֧�ֶ�̬�����DB������DB,��DB����֧��BACKUP����
    #define DB_DYM_DB           5               //֧�ֶ�̬�����DB
    #define DB_DYM_DB_2M        14
    #define DB_DYM_DB_SIZE      0x4000          //16K, ��̬�����С��16k
    #define DB_DYM_DB_NUM       0x40            //64��,
    #define DB_DYM_SIZE         (DB_DYM_DB_SIZE * DB_DYM_DB_NUM)
    #endif
#endif



/******************************************************************************
**EEPROMϵͳ���
*******************************************************************************/
#define SYS_EEPROM_EN  0


#ifdef    EXT_FILE


#if SYS_EEPROM_EN > 0
    #define SYS_EEPROM_KEYDNO  0x70
    #define SYS_FRAM_CRC     0x10u
    #define SYS_FRAM_CRCBK   0x108u
    #define SYS_FRAM_KEYD    0x200u
    #define SYS_FRAM_KEYDBK  0x1000u
#endif






#endif

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


#endif



