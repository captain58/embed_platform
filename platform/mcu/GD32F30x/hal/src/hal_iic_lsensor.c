/******************************Copyright(c)******************************
**                          
**
** File Name: hal_iic_hrtc.c
** Author: 
** Date Last Update: 2019-11-02
** Description:  iicӲ����д���������
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: �ļ�����
*************************************************************************/
#define EXT_IIC_LSENSOR

#include "sys.h"
#include "hal.h"


/************************************************************************
*��������ַ
************************************************************************/
#define I2C_LS_WR_ID  0x46                   //I2C��������ַ
#define I2C_LS_RD_ID  0x47                   //I2C��������ַ



/************************************************************************
*Ӳ��ʱ��IIC�ڶ˿�ȫ�ֽṹ��.
************************************************************************/
const IIC_PORTS* gsp_LSIICPrt;



/************************************************************************
*Ӳ��ʱ��IIC�ڲ�����
************************************************************************/
#define I2C_LS_GETSDA()    (HAL_GPIO_GetPinState(gsp_LSIICPrt->portSDAIn->gpio, gsp_LSIICPrt->portSDAIn->pinnum))
//	HAL_GPIO_PinConfig(&gsp_LSIICPrt->portSDA->gpio[gsp_LSIICPrt->portSDA->pingrp], gsp_LSIICPrt->portSDA->pinnum, gsp_LSIICPrt->portSDA->type, gsp_LSIICPrt->portSDA->analog, 1);

#define I2C_LS_SDAIN()    {HAL_GPIO_PinConfig(gsp_LSIICPrt->portSDAIn->gpio, gsp_LSIICPrt->portSDAIn->pinnum, gsp_LSIICPrt->portSDAIn->modefunc, 0);}
#define I2C_LS_SDAOUT()   {HAL_GPIO_PinConfig(gsp_LSIICPrt->portSDAOut->gpio, gsp_LSIICPrt->portSDAOut->pinnum, gsp_LSIICPrt->portSDAOut->modefunc, 2);}
#define I2C_LS_SCKOUT()   {HAL_GPIO_PinConfig(gsp_LSIICPrt->portSCK->gpio, gsp_LSIICPrt->portSCK->pinnum, gsp_LSIICPrt->portSCK->modefunc, 2);}

#define I2C_LS_SDA_1()    {HAL_GPIO_SetPinState(gsp_LSIICPrt->portSDAOut->gpio, gsp_LSIICPrt->portSDAOut->pinnum, 1);}
#define I2C_LS_SDA_0()    {HAL_GPIO_SetPinState(gsp_LSIICPrt->portSDAOut->gpio, gsp_LSIICPrt->portSDAOut->pinnum, 0);}

#define I2C_LS_SCK_1()    {HAL_GPIO_SetPinState(gsp_LSIICPrt->portSCK->gpio, gsp_LSIICPrt->portSCK->pinnum, 1);}
#define I2C_LS_SCK_0()    {HAL_GPIO_SetPinState(gsp_LSIICPrt->portSCK->gpio, gsp_LSIICPrt->portSCK->pinnum, 0);}

    
    
    

/*******************************************************************************
 * @function_name:  _DELAY
 * @function_file:  IIC_lsensor.C
 * @����: ��ʱ
 * 
 * 
 * @����: 
 * @param: time  
 * @����: 
 * @����: yzy (2019-11-3)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
void _DELAY(uint16 time)
{
    while(time--);
}

/*******************************************************************************
 * @function_name:  I2C_LS_WriteByte
 * @function_file:  IIC_lsensor.C
 * @����: I2C���ߴ���һ�ֽ�����
 * 
 * 
 * @����: 
 * @param: byte  д����ֽ�����
 * 
 * @����: 
 * @return:  uint8   
 * @����: yzy (2019-11-03)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
uint8 I2C_LS_WriteByte(uint8 byte)
{
    uint8 err = 0;

    for(uint8 uc_i = 0;uc_i < 8;uc_i++)     //ѭ�������������λ
    {
        I2C_LS_SCK_0();                   //ʱ���ߵ�
        _DELAY(100);                         //��ʱ,����
        if(byte & 0x80)                     //����һλ����
        {
            I2C_LS_SDA_1();
        }
        else
        {
            I2C_LS_SDA_0();
        }
        _DELAY(150);                        //��ʱ,����
        I2C_LS_SCK_1();                   //ʱ�Ӹ��ƽ
        _DELAY(200);                        //��ʱ����
        byte <<= 1;                         //��λ
    }
                                            //�ȴ�ACK�ź�
    I2C_LS_SCK_0();                       //ʱ�ӵ�
    _DELAY(50);  
    I2C_LS_SDAIN();                       //�����߸�Ϊ����
    _DELAY(150);   
                                            //��ʱ����
    I2C_LS_SCK_1();                       //ʱ�Ӹ�
    _DELAY(100);                            //��ʱ����
    if(I2C_LS_GETSDA())                   //��ȡ������״̬
    {
        err = 1;
        //return 1;                           //NAK,����ERR
    }
    _DELAY(50);                             //��ʱ����
    I2C_LS_SCK_0();                       //ʱ�ӵ�
    _DELAY(50);                             //��ʱ����
    //I2C_3231_SDA_1();
    I2C_LS_SDAOUT();                      //���������
    return err;
}

/*******************************************************************************
 * @function_name:  I2C_LS_ReadByte
 * @function_file:  IIC_lsensor.C
 * @����: I2C���߽���һ�ֽ�����(������һ���ֽں���Ҫ����һ��ACK��NAK�ź�)
 * 
 * @����: 
 * 
 * @����: IIC�������յ����ֽ�����
 * @return:  uint8   
 * @����: yzy (2019-11-03)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
uint8 I2C_LS_ReadByte(void)
{
    uint8 byte= 0;                          //���յ��ֽ����ݳ�ʼֵ

    I2C_LS_SDAIN();                       //����������
    for(uint8 uc_i = 0;uc_i < 8;uc_i++)     //ѭ�����ո�λ����
    {
        I2C_LS_SCK_0();                   //ʱ�ӵ�
        _DELAY(200);                        //��ʱ����
        I2C_LS_SCK_1();                   //ʱ�Ӹ�
        _DELAY(150);                        //��ʱ����
        byte <<= 1;                         //����λ����
        if(I2C_LS_GETSDA())               //��ȡ����λ
        {
            byte |= 0x01;
        }
        _DELAY(50);
    }

    return byte;
}

/*******************************************************************************
 * @function_name:  I2C_LS_Start
 * @function_file:  IIC_lsensor.C
 * @����: i2c��������
 * 
 * @����: 
 * @����: 
 * @����: yzy (2019-11-03)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
void I2C_LS_Start(void)
{
    I2C_LS_SDA_1();                       //�����������
    I2C_LS_SCK_1();                       //ʱ���������
    _DELAY(200);                            //��ʱ ����
    I2C_LS_SDA_0();                       //�������½���
    _DELAY(200);                            //��ʱ ����
    I2C_LS_SCK_0();                       //ʱ���������
    
}

/*******************************************************************************
 * @function_name:  I2C_LS_Stop
 * @function_file:  IIC_lsensor.C
 * @����: I2C����ֹͣ
 * 
 * @����: 
 * @����: 
 * @����: yzy (2019-11-03)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
void I2C_LS_Stop(void)
{
    //I2C_3231_SCK_0();                       //ʱ���������
    I2C_LS_SDA_0();                       //�����������
    _DELAY(200);                            //��ʱ ����
    I2C_LS_SCK_1();                       //ʱ���������
    _DELAY(200);                            //��ʱ ����
    I2C_LS_SDA_1();                       //������������
    //_DELAY(100);                            //��ʱ ����
    //I2C_3231_SCK_0();                       //ʱ���������

}

/*******************************************************************************
 * @function_name:  I2C_LS_ACK
 * @function_file:  IIC_lsensor.C
 * @����: I2C����ACKӦ���ź�
 * 
 * @����: 
 * @����: 
 * @����: yzy (2019-11-03)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
void I2C_LS_ACK(void)
{
    
    I2C_LS_SCK_0();                       //ʱ���ߵ͵�ƽ
    _DELAY(50);
    I2C_LS_SDAOUT();
    _DELAY(100);
    I2C_LS_SDA_0();                       //�����ߵ͵�ƽ
    _DELAY(100);                            //��ʱ ����
    I2C_LS_SCK_1();                       //ʱ����������
    _DELAY(200);                            //��ʱ ����
    I2C_LS_SCK_0();                       //ʱ�����½���
}

/*******************************************************************************
 * @function_name:  I2C_LS_NAK
 * @function_file:  IIC_lsensor.C
 * @����: I2C����NAK�ź�
 * 
 * @����: 
 * @����: 
 * @����: yzy (2019-11-03)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
void I2C_LS_NAK(void)
{
    I2C_LS_SCK_0();                       //ʱ���ߵ͵�ƽ
    _DELAY(50);
    I2C_LS_SDAOUT();
    _DELAY(100);
    I2C_LS_SDA_1();                       //�����߸��ƽ
    _DELAY(100);                            //��ʱ ����
    I2C_LS_SCK_1();                       //ʱ��������
    _DELAY(200);                            //��ʱ ����
    I2C_LS_SCK_0();                       //ʱ�����½���
}



/*******************************************************************************
 * @function_name:  I2C_LS_ReadLX
 * @function_file:  IIC_lsensor.C
 * @����: �⴫�������ݶ�ȡ
 * 
 * 
 * @����: 
 * @param: date  ���ݻ���
 * 
 * @����: 
 * @return:  uint8   
 * @����: yzy (2019-11-03)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
uint8 I2C_LS_ReadLX(uint8* date)
{

    I2C_LS_Start();                       //I2C��������               
    I2C_LS_WriteByte(I2C_LS_RD_ID);         //д��������
    I2C_LS_WriteByte(0x00);               //д���ȡ���ݵĵ�ַ
    
    I2C_LS_Stop();
    I2C_LS_Stop();

    I2C_LS_Start();                       //I2C������������
    I2C_LS_WriteByte(I2C_LS_RD_ID);     //��ȡ��������
    for(uint8 uc_i = 0; uc_i < 2; uc_i++)     //ѭ�������ֽ�����
    {
        date[uc_i] = I2C_LS_ReadByte();   //��ȡһ���ֽ�����
        if(uc_i == 1)                       //���һ���ֽڷ���NAK
        {
            I2C_LS_NAK();
        }
        else                                //����ACK
        {
            I2C_LS_ACK();
        }
    }
    I2C_LS_Stop();                        //I2C����ֹͣ
                                            //��ȡ����
//	    time[3] &= 0x7F;
//	    time[3] = STR_GetFirstSetBit(time + 3, 1);
    SYS_OK();
}


/*******************************************************************************
 * @function_name:  I2C_LS_WriteBcdTime
 * @function_file:  IIC_lsensor.C
 * @����: �⴫��������д��
 * 
 * 
 * @����: 
 * @param: data  
 * 
 * @����: 
 * @return:  uint8   
 * @����: yzy (2019-11-03)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
uint8 I2C_LS_WriteLX(uint8 data)
{
    uint8 err = 1;                          //д����Ĭ��err

                                            //���д�벻�ɹ�����д,�����д3��
    for(uint8 uc_i = 0;(uc_i < 3) && (err != 0); uc_i++)
    {
        I2C_LS_Start();                   //i2c��������
                                            //д����
        err = I2C_LS_WriteByte(I2C_LS_WR_ID);
        //err |= I2C_LS_WriteByte(0x00);    //д�������ַ

//	        for(uint8 uc_j = 0;uc_j < 7;uc_j++) //ѭ��д������ֽ�
        {
          //_DELAY(50);
            err |= I2C_LS_WriteByte(data);
        }
        I2C_LS_Stop();                    //i2c����ֹͣ
    }
    return err;                             //���ؽ��
}


/*******************************************************************************
 * @function_name:  Init_I2C_LS
 * @function_file:  IIC_lsensor.C
 * @����: �⴫����I2C���߳�ʼ��
 * 
 * @����: 
 * @����: 
 * @����: yzy (2019-11-3)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
void Init_I2C_LS(const IIC_PORTS* ports)
{
    uint8 uc_i;
      __HAL_RCC_GPIOB_CLK_ENABLE();
                                            //����SDA�˿ں�SCK�˿�
    HAL_GPIO_PinConfig(ports->portSDAOut->gpio, ports->portSDAOut->pinnum, ports->portSDAOut->modefunc, ports->portSDAOut->dir);
    HAL_GPIO_PinConfig(ports->portSCK->gpio, ports->portSCK->pinnum, ports->portSCK->modefunc, ports->portSCK->dir);
    

    //��ֵȫ�ֽṹ��
    gsp_LSIICPrt = ports;
//    I2C_LS_SCK_0(); 
//    I2C_LS_SDA_0(); 
    
    I2C_LS_SDAIN();                       //�����ź���Ĭ������
    I2C_LS_SCKOUT();                      //ʱ���ź���Ĭ�����
    I2C_LS_SCK_1();                       //Ĭ������͵�ƽ

    for(uc_i = 0;uc_i < 10;uc_i++)          //ѭ�����������״̬,��ʱ������Ϊ����̬
    {
        _DELAY(150);
        if(I2C_LS_GETSDA())               //�ߵ�ƽ,�����һ��Start�ź�
        {
            I2C_LS_SDAOUT();
            I2C_LS_SDA_1();
            break;
        }
                                            //�����ߵ͵�ƽ,���������,�任ʱ����(������),����ʱ��
        I2C_LS_SCK_0();                   //ʱ�������
        _DELAY(200);                        //��ʱ����
        I2C_LS_SCK_1();                   //ʱ�������(������)
        _DELAY(200);                        //��ʱ����
    }
    if(uc_i == 10)                          //10��ʱ��������,δ�ܽ�������ʱ��,����
    {
        return;
    }
    I2C_LS_SCK_0();
    _DELAY(200);
    I2C_LS_SCK_1();
    _DELAY(150);
    I2C_LS_SDA_1();

    //I2C_3231_F32KOFF();                     //Ĭ�Ϲر�32K�ź����
   // I2C_LS_SecIntOnOff(true);             //��ʼ�������ж����
}


