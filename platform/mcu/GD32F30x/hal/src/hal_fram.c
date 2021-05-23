/******************************Copyright(c)******************************
**                          
**
** File Name: hal_fram.c
** Author: 
** Date Last Update: 2015-05-18
** Description: SPI��������
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: �ļ�����
*************************************************************************/
#define EXT_FRAM

#include "sys.h"
#include "hal.h"



/************************************************************************
**�����С
*************************************************************************/
uint32 gul_FramSize;


/************************************************************************
**�������ָ�
*************************************************************************/
#define FRAM_STR_WREN   0x06            //дʹ��
#define FRAM_STR_WRDI   0x04            //д����
#define FRAM_STR_RDSR   0x05            //��״̬�Ĵ���
#define FRAM_STR_WRSR   0x01            //д״̬�Ĵ���
#define FRAM_STR_READ   0x03            //������
#define FRAM_STR_WRITE  0x02            //д����




/************************************************************************
**�������SPI�ṹ��
*************************************************************************/
SPIIO gs_FRAMSPI;

/************************************************************************
**��ȫ������
*************************************************************************/
uint32 gul_FRAMSafeCode;

/************************************************************************
**�������ȡ�ĵ�ǰ�����״̬
*************************************************************************/
uint8 guc_FRAMStatus;


/************************************************************************
**��CS�����ߵĺ�����ʽ
*************************************************************************/
void FRAM_csup(void)
{   
#if (SPI_IO_MACRO_EN > 0)
    SPI_CS_1_FRAM();
#else
    SPI_CS_1(SPI_DEV_ID_FRAM);
#endif
}

/************************************************************************
**��CS�����͵ĺ�����ʽ
*************************************************************************/
void FRAM_csdown(void)
{
#if (SPI_IO_MACRO_EN > 0)
    SPI_CS_0_FRAM();
#else
    SPI_CS_0(SPI_DEV_ID_FRAM);
#endif
}



/************************************************************************
**��ȡ����״̬�Ľṹ��
*************************************************************************/
const SPIIO gs_FRAMStatusR = 
{
    {FRAM_STR_RDSR,},
    &guc_FRAMStatus,
    1,
    1,
    FRAM_csup,
    FRAM_csdown,
};


/************************************************************************
**��������״̬�Ľṹ��
*************************************************************************/
const SPIIO gs_FRAMStatusW = 
{
    {FRAM_STR_WRSR,},
    &guc_FRAMStatus,
    1,
    1,
    FRAM_csup,
    FRAM_csdown,
};


/************************************************************************
**����������ڲ�д����
*************************************************************************/
const SPIIO gs_FRAMEnableWP = 
{
    {FRAM_STR_WRDI,},
    __NULL,
    0,
    1,
    FRAM_csup,
    FRAM_csdown,
};


/************************************************************************
**��ֹ������ڲ�д����
*************************************************************************/
const SPIIO gs_FRAMDisableWP = 
{
    {FRAM_STR_WREN,},
    __NULL,
    0,
    1,
    FRAM_csup,
    FRAM_csdown,
};


/************************************************************************
**��ȡ����״̬�ĺ궨��
*************************************************************************/
#define FRAM_GetStatus() {SPI_Read((SPIIO*)&gs_FRAMStatusR);}

/************************************************************************
**��������״̬�ĺ궨��
*************************************************************************/
#define FRAM_SetStatus() {SPI_Write((SPIIO*)&gs_FRAMStatusW);}

/************************************************************************
**��ֹ�����д����
*************************************************************************/
void FRAM_Disable_WP(void) 
{
#if (SPI_IO_MACRO_EN > 0)
    SPI_WP_1_FRAM();
#else
    SPI_WP_1(SPI_DEV_ID_FRAM);
#endif
    SPI_Write((SPIIO*)&gs_FRAMDisableWP);
}



/************************************************************************
**ʹ�������д����
*************************************************************************/
void FRAM_Enable_WP(void) 
{
    SPI_Write((SPIIO*)&gs_FRAMEnableWP);
#if (SPI_IO_MACRO_EN > 0)
    SPI_WP_0_FRAM();
#else
    SPI_WP_0(SPI_DEV_ID_FRAM);
#endif
}



/************************************************************************
 * @function: FRAM_Read
 * @����: ��ȡ�����ڲ�ָ����ַ������
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
TResult FRAM_Read(uint8* buffer, uint32 address, uint16 length)
{
                                        //������֤
    SYS_VAR_CHECK(address + length > gul_FramSize);
                                        //��ȡָ��
    gs_FRAMSPI.command[0] = FRAM_STR_READ;
                                        //��ȡ��ַ����
    if(gul_FramSize >= 0x20000)
    {
        gs_FRAMSPI.command[1] = address >> 16;
        gs_FRAMSPI.command[2] = address >> 8;
        gs_FRAMSPI.command[3] = address;
        gs_FRAMSPI.cmdnum = 4;           //����
    }
    else
    {
        gs_FRAMSPI.command[1] = address >> 8;
        gs_FRAMSPI.command[2] = address;
        gs_FRAMSPI.cmdnum = 3;              //����
    }
    gs_FRAMSPI.length = length;         //����
    gs_FRAMSPI.data = buffer;           //����
    return SPI_Read(&gs_FRAMSPI);       //���ض�ȡ���
}






/************************************************************************
 * @function: FRAM_PreWrite
 * @����: ����д���ݵ�Ԥ����
 * @����: 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2014/8/22)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 FRAM_PreWrite(void)
{
    gul_FRAMSafeCode = 0xA5A5A5A5;
    SYS_OK();
}





/************************************************************************
 * @function: FRAM_Write
 * @����: д�������ڲ�ָ����ַ������
 * 
 * @����: 
 * @param: buffer д�����ݵĻ���
 * @param: address д�����ݵ���ʼ��ַ
 * @param: length д�����ݵĳ���
 * 
 * @����: 
 * @return: TResult  
 * @˵��: 
 * @����: yzy (2014/8/22)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
TResult FRAM_Write(uint8* buffer, uint32 address, uint16 length)
{
    for(uint8 uc_i = 0; uc_i < 3; uc_i++)
    {
                                        //������֤
        SYS_VAR_CHECK(address + length > gul_FramSize || address > gul_FramSize);
                                        //�жϰ�ȫ��
        if(gul_FRAMSafeCode != 0xA5A5A5A5)  
        {
            return FRAM_ERR_FT;         //���ط�����������
        }
        FRAM_Disable_WP();              //����д����        
        gs_FRAMSPI.command[0] = FRAM_STR_WRITE;
                                        //д���ַ����

        if(gul_FramSize >= 0x20000)
        {
            gs_FRAMSPI.command[1] = address >> 16;
            gs_FRAMSPI.command[2] = address >> 8;
            gs_FRAMSPI.command[3] = address;
            gs_FRAMSPI.cmdnum = 4;           //����
        }
        else
        {
            gs_FRAMSPI.command[1] = address >> 8;
            gs_FRAMSPI.command[2] = address;
            gs_FRAMSPI.cmdnum = 3;           //����
        }
        gs_FRAMSPI.length = length;      //����
        gs_FRAMSPI.data = buffer;        //����
                                         //�жϰ�ȫ��   
        if(gul_FRAMSafeCode == 0xA5A5A5A5)  
        {                                //д������
            if(SPI_Write(&gs_FRAMSPI) != SYS_ERR_OK)
            {
                FRAM_Enable_WP();        //ʹ��д����
                continue;                //����һ��
            }
        }
        else
        {
            return FRAM_ERR_FT;          //������������
        }
        gs_FRAMSPI.command[0] = FRAM_STR_READ;
        uint8 SPI_ReadAndCompare(SPIIO* spi);
        if(SPI_ReadAndCompare(&gs_FRAMSPI) != SYS_ERR_OK)
        {
            FRAM_Enable_WP();           //ʹ��д����
            continue;                   //����һ��
        }
        FRAM_Enable_WP();               //ʹ��д����
        gul_FRAMSafeCode = 0;           //��λ��ȫ������
        SYS_OK();                        //����д��ɹ�
    }
    gul_FRAMSafeCode = 0;               //��ȫ�����ֹ���
    return FRAM_ERR_FT;                 //���ط�����������

}



/************************************************************************
 * @function: HAL_InitFRAM
 * @����: ��ʼ������洢��
 * @����: 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2014/8/22)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 HAL_InitFRAM(void)
{
    gs_FRAMSPI.csup = FRAM_csup;        //cs up
    gs_FRAMSPI.csdown = FRAM_csdown;    //cs down
    gul_FRAMSafeCode = 0;               //��ʼ����ȫ������
    
    uint8 mask = 0x70;                  //�������ʼ״̬����
    gul_FramSize = 0x20000;

    FRAM_GetStatus();                   //��ȡ״̬
    if((guc_FRAMStatus & mask) != 0)    //�ж�FRAM�Ƿ����
    {
        FRAM_GetStatus();               //�ٴλ�ȡ״̬
        if((guc_FRAMStatus & mask) != 0)    //�ж�FRAM�Ƿ����
        {
            return FRAM_ERR_FT;         //���ط�����������
        }
    }
    FRAM_Disable_WP();                  //ȥ��д��������
    guc_FRAMStatus = 0x80;              //׼������״̬�Ĵ���
    FRAM_SetStatus();                   //����״̬�Ĵ���
    FRAM_Enable_WP();                   //����ʹ��д���� 
    SYS_OK();                            //���ز����ɹ�                                                                            
}


