/****************************************Copyright (c)**************************************************
**                                  ________�Ƽ����޹�˾
**                                          ������
**
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: iap_file.c
** ������: yzy
** ��  ��: v1.0
** �ա���: 2010��11��11��
** �衡��: ԭʼ�汾
**         ���ļ�����boot�ļ�������Ӧ,Ӧ�ó���ֻ������bootָ����flash��ַд�������ļ�.
**         �������������,�ڸ�λ����boot����ִ��
********************************************************************************************************/
#define EXT_DEV_IAP

#include "es.h"
#include "hal.h"
#include "bsp.h"


#if SYS_IAP_EN > 0


//�ⲿflash��غ궨��
#define FLS_PROGRAM_ADDR         (4096 + 256)       //�������ⲿflash�е�λ��(ǰ32�ֽ��������upatemark)
#define FLS_PROGRAM_MAXLEN       524288     //�����ܿռ��С512K�ֽ�


#define FLS_UPDATEMARK_ADDR      0          //����������������Flash�е�λ��

/*******************************************************************************
**�����������������Զ�������ı��,�Ӷ�����������ʱ������ͣ��
**�ڸ�λ֮��õ��ظ�
********************************************************************************/
const uint32 gul_UpdataMarkA_APP[4] = 
{
    0x12345678, 0x87654321, 0x11223344, 0x55667788
};


const uint32 gul_UpdataMarkB_APP[4] = 
{
    0,0,0,0
};



/***********************************************************
 * @function_name: SYS_IAP_Mark
 * @function_file: iap_file.c
 * @����: ���û�������������         
 * @����: 
 * @param:   setorclr ���û������������� 
 * @param:   ID       ����ID
 * @param:  length   ���ݳ���
 * @param:  address  ���ݵ�ַ
 * @����: 
 * @return:  
 * @����:  yzy (2010-11-11)
 *******************************************************/
void SYS_APP_IAP_Mark(bool setorclr, uint32 length, uint32 address)
{
    uint8 uc_data[28];   

    if(setorclr)                        //���ñ��
    {
                                        //����д����
        memcpy(uc_data, (void*)gul_UpdataMarkA_APP, sizeof(gul_UpdataMarkA_APP));
        uc_data[16] = 0;               //����ID
                                        //����
        LongToByte(uc_data + 18, length);
                                        //��ַ
        LongToByte(uc_data + 22, address);
                                        //����CRCУ��
        IntToByte(uc_data + 26, DoCrcRtuModbus(uc_data, 26, 0xFFFF));
    }
    else                                //�����������ݾͿ�����
    {
        memcpy(uc_data, (void*)gul_UpdataMarkB_APP, sizeof(gul_UpdataMarkB_APP));
    }
    
    SYS_FILE_Open();
	SYS_FILE_Write(uc_data, FLS_UPDATEMARK_ADDR, 28);
    SYS_FILE_Close();
}



/***********************************************************
 * @function_name: SYS_APP_IAP_Check
 * @function_file: iap_file.c
 * @����: ����Flash����������ĺϷ���         
 * @����: 
 * @param:  startAddr  ��������λ��FLASH����ʼƫ��
 * @param�� length �������ݳ���
 * @����: 
 * @return: true  ���ݺϷ�,����ʼ���г�������
           false  ���ݷǷ�,�����ܽ��г�������
 * @����:  yzy (2010-11-11)
 *******************************************************/
bool SYS_APP_IAP_CheckCRC(uint32 startAddr, uint32 length)
{
    uint8 uc_data[64];                  //����������ȡ������
    uint16 crc = 0xFFFF;                //CRCУ��ֵ
    uint16 cmp;                         //CRCУ��ֵ�ĶԱ�
    
    uint32 curAdress = startAddr;		//������ʼ��ַ
    
    if((length & 0x03) != 0)            //�жϳ��ȵĺϷ���
    {
        return false;                   //���ش���
    }
    
    SYS_FILE_Open();
    
    while(length > 0)                   //���ϵĶ�ȡ����Ȼ�����У��
    {
        if(length > 64)                //����64�ֽڶ�64�ֽ�
        {
            
            SYS_FILE_Read(uc_data, curAdress, 64);
            crc = DoCrcRtuModbus(uc_data, 64, crc);
            length -= 64;
            curAdress += 64;
        }
        else                            //�����ȡʣ�����ݵĳ���
        {
            SYS_FILE_Read(uc_data, curAdress, length);
                                        //����4���ֽ���CRCУ��
            crc = DoCrcRtuModbus(uc_data, length - 4, crc);
                                        //��ȡCRCУ��ĶԱ�ֵ
            cmp = ByteToInt(uc_data + length - 2);
            length = 0;                 //�Ѿ���ȡ�����е�����
        }
    }
    
    SYS_FILE_Close();
    
    if(cmp != crc)                      //�ж�CRC��У��ֵ�Ƿ����
    {
        return false;
    }
    return true;                        //����OK        
}

/***********************************************************
 * @function_name: SYS_APP_IAP_Check
 * @function_file: iap_file.c
 * @����: ����Flash����������ĺϷ���         
 * @����: 
 * @param:  db  ���ݿ�
 * @param�� len ����
 * @����: 
 * @return: true  ���ݺϷ�,����ʼ���г�������
           false  ���ݷǷ�,�����ܽ��г�������
 * @����:  yzy (2010-11-11)
 *******************************************************/
bool SYS_APP_IAP_Check(uint32 length)
{
	return SYS_APP_IAP_CheckCRC(FLS_PROGRAM_ADDR, length);
}



/************************************************************************
 * @function: SYS_IAP_Ready
 * @����: 
 * 
 * @����: 
 * @param: address ������ʼ��ַ
 * @param: length �û�������Ҫ�����ĳ���ĳ���
 * @param: isreset ������֮���Ƿ���Ҫ��λ�û�����
 * 
 * @����: 
 * @return: bool  true�ɹ�/falseʧ��
 * @˵��: 
 * @����: yzy (2013/7/16)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
bool SYS_IAP_Ready(uint32 address, uint32 length, bool isreset)
{
    if(length > 0x80000)                //��֧����ô������������
    {
        return false;                   //���ش���
    }
                                        //ȷ���������ݵĺϷ���,���������ͱ��ݲ���CRC������ȷ����Ϊ����ȷ
    if(!SYS_APP_IAP_Check(length))
    {
	    return false;                   //���ش���
    }
                                        //д��������־
    SYS_APP_IAP_Mark(true, length, address);
                                        //����ϵͳ��λ
    SYS_Reset();

    return true;
}

#endif                                  //SA_IAP_EN == 1

/*********************************************************************************************************
*************************************�ļ�����*************************************************************
**********************************************************************************************************/


