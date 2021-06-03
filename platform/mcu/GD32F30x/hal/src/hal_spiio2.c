/****************************************Copyright (c)**************************************************
**                                  ________�Ƽ����޹�˾
**                                          ������
**
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: spiio.c
**��   ��   ��: yzy
**����޸�����: 2007��8��4��
**��        ��: Ӳ��SPI����ģ��
**ע        ��:
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: yzy
** ��  ��: v1.0
** �ա���: 2007��8��4��
** �衡��: ԭʼ�汾
********************************************************************************************************/
#define EXT_SPIIO

//#include "bsp_common.h"


#include "hal.h"


#if (SPI_IO_MACRO_EN == 0)
//˵��: ��ϵͳֻ������һ��SPI��,���¹Ҷ���豸,���ݲ�ͬƬѡ�������豸.

/************************************************************************
*SPI�ڶ˿�ȫ�ֽṹ��.
************************************************************************/
const SPIIO_PORTS* gsp_halSpiioPorts;


/************************************************************************
*SPI�ڲ�����
************************************************************************/
//	#define SPI_GET_DIN()   ((gsp_halSpiioPorts->portMISO->gpio[gsp_halSpiioPorts->portMISO->pingrp].PIN >> gsp_halSpiioPorts->portMISO->pinnum) & 1)
#define SPI_GET_DIN()    (hal_gpio_input_get_stt(gsp_halSpiioPorts->portMISO))

//	#define SPI_DOUT_1()    {gsp_halSpiioPorts->portMOSI->gpio[gsp_halSpiioPorts->portMOSI->pingrp].SET |= 1UL << gsp_halSpiioPorts->portMOSI->pinnum;}
//	#define SPI_DOUT_1()   {Chip_GPIO_SetPinDIROutput(gsp_halSpiioPorts->portMOSI->pingrp, gsp_halSpiioPorts->portMOSI->pinnum);}
//	
//	#define SPI_DOUT_0()    {gsp_halSpiioPorts->portMOSI->gpio[gsp_halSpiioPorts->portMOSI->pingrp].CLR |= 1UL << gsp_halSpiioPorts->portMOSI->pinnum;}
#define SPI_DOUT_1()    {hal_gpio_output_high(gsp_halSpiioPorts->portMOSI);}
#define SPI_DOUT_0()    {hal_gpio_output_low(gsp_halSpiioPorts->portMOSI);}


//	#define SPI_SCK_1()     {gsp_halSpiioPorts->portSCK->gpio[gsp_halSpiioPorts->portSCK->pingrp].SET |= 1UL << gsp_halSpiioPorts->portSCK->pinnum;}
//	#define SPI_SCK_0()     {gsp_halSpiioPorts->portSCK->gpio[gsp_halSpiioPorts->portSCK->pingrp].CLR |= 1UL << gsp_halSpiioPorts->portSCK->pinnum;}
#define SPI_SCK_1()    {hal_gpio_output_high(gsp_halSpiioPorts->portSCK);}
#define SPI_SCK_0()    {hal_gpio_output_low(gsp_halSpiioPorts->portSCK);}

//���������빦����ͬ,�����������ٶȸ���
//#define SPI_GET_DIN()   (Chip_GPIO_GetPinState(gsp_halSpiioPorts->portMISO->gpio, gsp_halSpiioPorts->portMISO->pingrp, gsp_halSpiioPorts->portMISO->pinnum))
//#define SPI_DOUT_1()    {Chip_GPIO_SetPinState(gsp_halSpiioPorts->portMOSI->gpio, gsp_halSpiioPorts->portMOSI->pingrp, gsp_halSpiioPorts->portMOSI->pinnum, true);}
//#define SPI_DOUT_0()    {Chip_GPIO_SetPinState(gsp_halSpiioPorts->portMOSI->gpio, gsp_halSpiioPorts->portMOSI->pingrp, gsp_halSpiioPorts->portMOSI->pinnum, false);}
//#define SPI_SCK_1()     {Chip_GPIO_SetPinState(gsp_halSpiioPorts->portSCK->gpio, gsp_halSpiioPorts->portSCK->pingrp, gsp_halSpiioPorts->portSCK->pinnum, true);}
//#define SPI_SCK_0()     {Chip_GPIO_SetPinState(gsp_halSpiioPorts->portSCK->gpio, gsp_halSpiioPorts->portSCK->pingrp, gsp_halSpiioPorts->portSCK->pinnum, false);}





/************************************************************************
 * @function: SPI_CS_1
 * @����: SPI��Ƭѡ������(����)
 * 
 * @����: 
 * @param: devidx �豸����(0-N),0xFF��ʾ����
 * @����: 
 * @˵��: 
 * @����: yzy (2014/6/16)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void SPI_CS_1(uint8 devidx)
{
    gpio_dev_t* cpt;
    
    if(devidx == 0)
    {
        cpt = gsp_halSpiioPorts->portCS;
    }
    else if(devidx == 1)
    {
        cpt = gsp_halSpiioPorts->portCS2;
    }
    else if(devidx == 2)
    {
        cpt = gsp_halSpiioPorts->portCS3;
    }
    else
    {
        return;
    }
    
    if(cpt != __NULL)
    {
        hal_gpio_output_high(cpt);
//	        Chip_GPIO_SetPinState(cpt->pingrp, cpt->pinnum, true);
    }
}


/************************************************************************
 * @function: SPI_CS_0
 * @����: SPI��Ƭѡ������
 * 
 * @����: 
 * @param: devidx �豸����(0-N)
 * @����: 
 * @˵��: 
 * @����: yzy (2014/6/16)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void SPI_CS_0(uint8 devidx)
{
    gpio_dev_t* cpt;
    
    if(devidx == 0)
    {
        cpt = gsp_halSpiioPorts->portCS;
    }
    else if(devidx == 1)
    {
        cpt = gsp_halSpiioPorts->portCS2;
    }
    else if(devidx == 2)
    {
        cpt = gsp_halSpiioPorts->portCS3;
    }
    else
    {
        return;
    }
    
    if(cpt != __NULL)
    {
        hal_gpio_output_low(cpt);
//	        Chip_GPIO_SetPinState(cpt->pingrp, cpt->pinnum, false);
    }
}



/************************************************************************
 * @function: SPI_WP_1
 * @����: SPI�ڱ���������
 * 
 * @����: 
 * @param: devidx �豸����(0-N)
 * @����: 
 * @˵��: 
 * @����: yzy (2014/6/16)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void SPI_WP_1(uint8 devidx)
{
    gpio_dev_t* cpt;
    
    if(devidx == 0)
    {
        cpt = gsp_halSpiioPorts->portWP;
    }
    else if(devidx == 1)
    {
        cpt = gsp_halSpiioPorts->portWP2;
    }
    else if(devidx == 2)
    {
        cpt = gsp_halSpiioPorts->portWP3;
    }
    else
    {
        return;
    }
    
    if(cpt != __NULL)
    {
        hal_gpio_output_high(cpt);
//	        Chip_GPIO_SetPinState( cpt->pingrp, cpt->pinnum, true);
    }
}



/************************************************************************
 * @function: SPI_WP_0
 * @����: SPI�ڱ���������
 * 
 * @����: 
 * @param: devidx �豸����(0-N)
 * @����: 
 * @˵��: 
 * @����: yzy (2014/6/16)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void SPI_WP_0(uint8 devidx)
{
    gpio_dev_t* cpt;
    
    if(devidx == 0)
    {
        cpt = gsp_halSpiioPorts->portWP;
    }
    else if(devidx == 1)
    {
        cpt = gsp_halSpiioPorts->portWP2;
    }
    else if(devidx == 2)
    {
        cpt = gsp_halSpiioPorts->portWP3;
    }
    else
    {
        return;
    }
    
    if(cpt != __NULL)
    {
        hal_gpio_output_low(cpt);
//	        Chip_GPIO_SetPinState( cpt->pingrp, cpt->pinnum, false);
    }
}

/************************************************************************
 * @function: SPI_PWR_1
 * @����: SPI�豸��Դ������
 * 
 * @����: 
 * @param: devidx �豸����(0-N)
 * @����: 
 * @˵��: 
 * @����: yzy (2014/6/16)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void SPI_PWR_1(uint8 devidx)
{
    gpio_dev_t* cpt;
    
    if(devidx == 0)
    {
        cpt = gsp_halSpiioPorts->portPWR;
    }
    else if(devidx == 1)
    {
        cpt = gsp_halSpiioPorts->portPWR2;
    }
    else if(devidx == 2)
    {
        cpt = gsp_halSpiioPorts->portPWR3;
    }
    else
    {
        return;
    }
    
    if(cpt != __NULL)
    {
        hal_gpio_output_high(cpt);    
//	        Chip_GPIO_SetPinState( cpt->pingrp, cpt->pinnum, true);
    }
}



/************************************************************************
 * @function: SPI_PWR_0
 * @����: SPI�豸��Դ������
 * 
 * @����: 
 * @param: devidx �豸����(0-N)
 * @����: 
 * @˵��: 
 * @����: yzy (2014/6/16)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void SPI_PWR_0(uint8 devidx)
{
    gpio_dev_t* cpt;
    
    if(devidx == 0)
    {
        cpt = gsp_halSpiioPorts->portPWR;
    }
    else if(devidx == 1)
    {
        cpt = gsp_halSpiioPorts->portPWR2;
    }
    else if(devidx == 2)
    {
        cpt = gsp_halSpiioPorts->portPWR3;
    }
    else
    {
        return;
    }
    
    if(cpt != __NULL)
    {
        hal_gpio_output_low(cpt);    
//	        Chip_GPIO_SetPinState( cpt->pingrp, cpt->pinnum, false);
    }
}


#endif





/************************************************************************
 * @function: _SPI_SendByte
 * @����: ����һ���ֽڵ�SPI����
 * 
 * @����: 
 * @param: byte Ҫ���͵��ֽ�
 * @����: 
 * @˵��: 
 * @����: yzy (2014/6/16)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void _SPI_SendByte(uint8 byte)
{
    if(byte & 0x80)                 //SPI��MSB��ʼ��������
    {
        SPI_DOUT_1();               //�������λ�����������λ������
    }
    else
    {
        SPI_DOUT_0();               //���͵͵�ƽ
    }
    SPI_SCK_1();                    //������              
    SPI_SCK_0();                    //�½���   
    if(byte & 0x40)                 //SPI��MSB��ʼ��������
    {
        SPI_DOUT_1();               //�������λ�����������λ������
    }
    else
    {
        SPI_DOUT_0();               //���͵͵�ƽ
    }
    SPI_SCK_1();                    //������              
    SPI_SCK_0();                    //�½���   
    if(byte & 0x20)                 //SPI��MSB��ʼ��������
    {
        SPI_DOUT_1();               //�������λ�����������λ������
    }
    else
    {
        SPI_DOUT_0();               //���͵͵�ƽ
    }
    SPI_SCK_1();                    //������              
    SPI_SCK_0();                    //�½���   
    if(byte & 0x10)                 //SPI��MSB��ʼ��������
    {
        SPI_DOUT_1();               //�������λ�����������λ������
    }
    else
    {
        SPI_DOUT_0();               //���͵͵�ƽ
    }
    SPI_SCK_1();                    //������              
    SPI_SCK_0();                    //�½���   
    if(byte & 0x08)                 //SPI��MSB��ʼ��������
    {
        SPI_DOUT_1();               //�������λ�����������λ������
    }
    else
    {
        SPI_DOUT_0();               //���͵͵�ƽ
    }
    SPI_SCK_1();                    //������              
    SPI_SCK_0();                    //�½���   
    if(byte & 0x04)                 //SPI��MSB��ʼ��������
    {
        SPI_DOUT_1();               //�������λ�����������λ������
    }
    else
    {
        SPI_DOUT_0();               //���͵͵�ƽ
    }
    SPI_SCK_1();                    //������              
    SPI_SCK_0();                    //�½���   

    if(byte & 0x02)                 //SPI��MSB��ʼ��������
    {
        SPI_DOUT_1();               //�������λ�����������λ������
    }
    else
    {
        SPI_DOUT_0();               //���͵͵�ƽ
    }
    SPI_SCK_1();                    //������              
    SPI_SCK_0();                    //�½���   
                                    // 
    if(byte & 0x01)                 //SPI��MSB��ʼ��������
    {
        SPI_DOUT_1();               //�������λ�����������λ������
    }
    else
    {
        SPI_DOUT_0();               //���͵͵�ƽ
    }
    SPI_SCK_1();                    //������              
    SPI_SCK_0();                    //�½���   

}



/************************************************************************
 * @function: _SPI_ReceiveByte
 * @����: ��SPI�����Ͻ���һ���ֽ�
 * @����: 
 * 
 * @����: 
 * @return: uint8  ���յ���һ���ֽ�
 * @˵��: 
 * @����: yzy (2014/6/16)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 _SPI_ReceiveByte(void)
{
    uint8 byte = 0;
    
    SPI_SCK_1();
    if(SPI_GET_DIN())
    {
        byte |= 0x80;
    }
    SPI_SCK_0();        

    SPI_SCK_1();
    if(SPI_GET_DIN())
    {
        byte |= 0x40;
    }
    SPI_SCK_0();

    SPI_SCK_1();
    if(SPI_GET_DIN())
    {
        byte |= 0x20;
    }
    SPI_SCK_0();

    SPI_SCK_1();
    if(SPI_GET_DIN())
    {
        byte |= 0x10;
    }
    SPI_SCK_0();

    SPI_SCK_1();
    if(SPI_GET_DIN())
    {
        byte |= 0x08;
    }
    SPI_SCK_0();

    SPI_SCK_1();
    if(SPI_GET_DIN())
    {
        byte |= 0x04;
    }
    SPI_SCK_0();

    SPI_SCK_1();
    if(SPI_GET_DIN())
    {
        byte |= 0x02;
    }
    SPI_SCK_0();

    SPI_SCK_1();
    if(SPI_GET_DIN())
    {
        byte |= 0x01;
    }
    SPI_SCK_0();
    return byte;
}





/************************************************************************
 * @function: SPI_Write
 * @����: д����
 * 
 * @����: 
 * @param: spi д�����ݵĵ�ַ�Ľṹ��Ϣ** 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2014/6/16)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 SPI_Write(SPIIO* spi)
{
    uint16 ui_i;                         //ѭ������
    SYS_VAR_CHECK(spi->cmdnum > 8);    
    SPI_SCK_0();
    spi->csdown();                      //cs�߱�����
                                        //ѭ������ָ��������ڲ���ַ
    for(ui_i = 0; ui_i < spi->cmdnum; ui_i ++)
    {                                   //����ָ��͵�ַ
        _SPI_SendByte(spi->command[ui_i]);
    }
                                        //ѭ����������ֽ�
    for(ui_i = 0; ui_i < spi->length; ui_i ++)
    {                                        
        _SPI_SendByte(spi->data[ui_i]); //��������ֽ�        
    }
    spi->csup();                        //��CS������
    SYS_OK();
}



/************************************************************************
 * @function: SPI_Read
 * @����: ������
 * 
 * @����: 
 * @param: spi ��ȡ���ݵĵ�ַ�Ľṹ��Ϣ** 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2014/6/16)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 SPI_Read(SPIIO* spi)
{
    uint16 ui_i;                         //ѭ������
    SYS_VAR_CHECK(spi->cmdnum > 8);    
    SPI_SCK_0();
    spi->csdown();                      //cs�߱�����
                                        //ѭ������ָ��������ڲ���ַ
    for(ui_i = 0; ui_i < spi->cmdnum; ui_i ++)
    {                                   //����ָ��͵�ַ
        _SPI_SendByte(spi->command[ui_i]);
    }
                                        //ѭ����������ֽ�
    for(ui_i = 0; ui_i < spi->length; ui_i ++)
    {
                                        //��������ֽ�
        spi->data[ui_i] = _SPI_ReceiveByte();   
    }
    spi->csup();                        //��CS������
    return SPI_ReadAndCompare(spi);     //���ض����ȽϵĽ��
}

/************************************************************************
 * @function: SPI_Close
 * @����: SPI����͹���
 * 
 * @����: 
 * @param: devidx �豸����(0-N)
 * 
 * @����: 
 * @return: 
 * @˵��: 
 * @����: yzy (2018-07-02)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void SPI_Close(unsigned char devidx)
{
    if(devidx == 0)
    {
        SPI_SCK_0();
        SPI_DOUT_0();
//	        _SPI_SCK_Down();
//	        _SPI_DOUT_Down();
    }
    else if(devidx == 1)
    {
    }
    else if(devidx == 2)
    {
    }
    else
    {
        return;
    }
}

/************************************************************************
 * @function: SPI_ReadAndCompare
 * @����: �������ж�ȡָ�����ȵ����ݺͻ�����������ݽ��бȽ�
 * 
 * @����: 
 * @param: spi ��ȡ���ݵĵ�ַ�Ľṹ��Ϣ**  
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2014/6/16)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 SPI_ReadAndCompare(SPIIO* spi)
{
    uint16 ui_i;
    SYS_VAR_CHECK(spi->cmdnum > 8);    
    SPI_SCK_0();
    spi->csdown();                      //cs�߱�����
                                        //ѭ������ָ��������ڲ���ַ
    for(ui_i = 0; ui_i < spi->cmdnum; ui_i ++)
    {                                   //����ָ��͵�ַ
        _SPI_SendByte(spi->command[ui_i]);
    }   
                                        //ѭ����������ֽ�
    for(ui_i = 0; ui_i < spi->length; ui_i ++)
    {
                                        //��������ֽ�
        if(spi->data[ui_i] != _SPI_ReceiveByte())
        {
            spi->csup();                //��CS������
            return SYS_ERR_FT;
        }
    }
    spi->csup();                        //��CS������
    SYS_OK();
}




/************************************************************************
 * @function: Init_SPI
 * @����: ��ʼ��SPI��IO��
 * 
 * @����: 
 * @param: ports �˿��б�
 * @param: portnum �˿�����
 * @����: 
 * @˵��: 
 * @����: yzy (2014/6/16)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void Init_SPI(const SPIIO_PORTS* ports)
{
    gpio_dev_t* lp;  
                                            //��ʼ��SPI�����ĸ�GPIO�˿�
    for(uint8 uc_i = 0; uc_i < (sizeof(SPIIO_PORTS) / sizeof(gpio_dev_t*)); uc_i++)
    {
        lp = *((gpio_dev_t**)ports + uc_i);
        
        if(lp != __NULL)                    //�������
        {
            hal_gpio_init(lp);
//	            Chip_GPIO_SetPinDigit(lp->pingrp, lp->pinnum);
//	            
//	            Chip_GPIO_SetPinPFSEG(lp->pingrp, lp->pinnum, lp->pinseg);
//	            Chip_GPIO_SetPinOutputMode(lp->pingrp, lp->pinnum, lp->modefunc & 0x01);
//	            
//	            if(lp->dir)                     //���
//	            {
//	                Chip_GPIO_SetPinDIROutput(lp->pingrp, lp->pinnum);
//	                
//	            }
//	            else                            //����
//	            {
//	                Chip_GPIO_SetPinDIRInput(lp->pingrp, lp->pinnum);
//	            }
        }
    }
    
#if (SPI_IO_MACRO_EN == 0)
    gsp_halSpiioPorts = ports;              //��ֵȫ�ֽṹ��
#endif
                                            //�����û���Դ,��spi�����豸ʹ��
//	    guc_SpiioUserTkid = NULL;
//	    SYS_SEM_Create(1, &gs_SpiioRes);
    
}








