/****************************************Copyright (c)**************************************************
**                                  ________�Ƽ����޹�˾
**                                          ������
**
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: iic_Eeprom.c
**��   ��   ��: yzy
**����޸�����: 2018-07-02
**��        ��: Eeprom����������
**ע        ��:
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: yzy
** ��  ��: v1.0
** �ա���: 2013��10��05��
** �衡��: Eeprom����
********************************************************************************************************/
//	#define __TYPEDEF__
#define EXT_DEV_E2

#include "sys.h"
#include "hal.h"
#include "bsp.h"


/*******************************************************************************
**eeprom����iic�ṹ��
********************************************************************************/
SPIIO gs_EEPROM_iic;

/*******************************************************************************
 * @function_name:  Init_I2C_Eeprom
 * @function_file:  IIC_Eeprom.C
 * @����: EepromI2C���߳�ʼ��
 * 
 * @����: 
 * @����: 
 * @����: yzy (2018-07-02)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
void SYS_E2_Init(void)
{

    HAL_E2_Init(&gs_e2Port);
//	    I2C_Eeprom_PWROUT();
//	
//	//��ַ����
//	
//	    I2C_Eeprom_ADDR1OUT();
//	    I2C_Eeprom_ADDR2OUT();
//	    I2C_Eeprom_ADDR3OUT();
//	
//	    
//	    I2C_Eeprom_SDAOUT();                       //�����ź���Ĭ�����
//	    I2C_Eeprom_SCKOUT();                      //ʱ���ź���Ĭ�����
//	    I2C_Eeprom_SCK_1();                       //Ĭ������͵�ƽ
//	    I2C_Eeprom_SDA_1();


}

void SYS_E2_Idel(void)
{
    HAL_E2_Idel();


}






/************************************************************************
 * @function: PCB_Eeprom_Read
 * @����: ��ȡeeprom�ڲ�ָ����ַ������
 * 
 * @����: 
 * @param: buffer ��ȡ���ݵĻ���
 * @param: address ��ȡ���ݵ���ʼ��ַ
 * @param: length ��ȡ���ݵĳ���
 * 
 * @����: 
 * @return: TResult  
 * @˵��: 
 * @����: yzy (2014/8/22)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8_t SYS_E2_Read(uint8_t* buffer, uint32_t address, uint16_t length)
{
    uint8_t m = 0;
                                        //������֤
    SYS_VAR_CHECK(address + length > DEF_EE_SIZE);
    SYS_VAR_CHECK(length == 0);
    SYS_VAR_CHECK(buffer == NULL);                                        
                                        //��ȡ��ַ����
    if(DEF_EE_SIZE >= 0x20000)
    {
        gs_EEPROM_iic.command[m++] = address >> 16;
        gs_EEPROM_iic.command[m++] = address >> 8;
        gs_EEPROM_iic.command[m++] = address;
    }
    else
    {
        gs_EEPROM_iic.command[m++] = address >> 8;
        gs_EEPROM_iic.command[m++] = address;
    }
    gs_EEPROM_iic.cmdnum = m;           //����
    
    gs_EEPROM_iic.length = length;         //����
    gs_EEPROM_iic.data = buffer;           //����
    return HAL_E2_Read(&gs_EEPROM_iic);       //���ض�ȡ���
}



/************************************************************************
 * @function: FRAM_Write
 * @����: д��eeprom�ڲ�ָ����ַ������
 * 
 * @����: 
 * @param: buffer д�����ݵĻ���
 * @param: address д�����ݵ���ʼ��ַ
 * @param: length д�����ݵĳ���
 * 
 * @����: 
 * @return: uint8_t  
 * @˵��: 
 * @����: yzy (2014/8/22)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8_t SYS_E2_Write_Page(uint8_t* buffer, uint32_t address, uint16_t length)
{
    uint8_t m = 0;
    for(uint8_t uc_i = 0; uc_i < 3; uc_i++)
    {
        m=0;
                                        //������֤
        SYS_VAR_CHECK(address + length > DEF_EE_SIZE || address > DEF_EE_SIZE);
                                        //д���ַ����

        if(DEF_EE_SIZE >= 0x20000)
        {
            gs_EEPROM_iic.command[m++] = address >> 16;
            gs_EEPROM_iic.command[m++] = address >> 8;
            gs_EEPROM_iic.command[m++] = address;
        }
        else
        {
            gs_EEPROM_iic.command[m++] = address >> 8;
            gs_EEPROM_iic.command[m++] = address;
        }
        
        gs_EEPROM_iic.cmdnum = m;           //����
        gs_EEPROM_iic.length = length;      //����
        gs_EEPROM_iic.data = buffer;        //����
                                         //�жϰ�ȫ��   
        if(HAL_E2_Write(&gs_EEPROM_iic) != SYS_ERR_OK)
        {
            continue;                //����һ��
        }

        if(HAL_E2_RdAndCmp(&gs_EEPROM_iic) != SYS_ERR_OK)
        {
            continue;                   //����һ��
        }
        SYS_OK();                        //����д��ɹ�
    }
    return SYS_ERR_FT;                 //���ط�����������

}
uint8_t SYS_E2_Write(uint8_t* buffer, uint32_t address, uint16_t length)
{
    uint16_t ui_len = 0;                      //����SECTOR��д��ĳ���
    uint32_t addr = address;

    
    SYS_VAR_CHECK(address + length > DEF_EE_SIZE);
    SYS_VAR_CHECK(length == 0);
    SYS_VAR_CHECK(buffer == NULL);
    /************************************************************************
    //ѭ������ÿ����Ҫ���ĵ�SECTOR
    ************************************************************************/
    while(true)                             //ѭ������ÿ����Ҫ���ĵ�SECTOR
    {
        if(length + (addr & DEF_EE_BUF_MASK) > DEF_EE_BUF)  //���
        {
            ui_len = DEF_EE_BUF - (addr & DEF_EE_BUF_MASK); //���������ݳ���
        }
        else                                //���ڲ���
        {
            ui_len = length;                //�������ݳ���
        }
        if(SYS_ERR_OK != SYS_E2_Write_Page(buffer, addr, ui_len))
            return SYS_ERR_FT;

        length -= ui_len;                   //���������ݳ��ȵݼ�
        if(length == 0)                     //�����Ѿ��������
        {
            SYS_OK();                        //����д��ɹ�
        }

        buffer += ui_len;                   //��������ָ��ǰ��
        addr += ui_len;                     //ʵ�ʲ�����ַ
    }

}


