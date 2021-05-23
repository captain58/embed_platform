/******************************Copyright(c)******************************
**                          
**
** File Name: hal_iic_hrtc.c
** Author: 
** Date Last Update: 2019-11-02
** Description:  iicӲ��ʱ������
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: �ļ�����
*************************************************************************/
#define EXT_IIC_HRTC

#include "sys.h"
#include "hal.h"


/************************************************************************
*��������ַ
************************************************************************/
#define I2C_RTC_ID  0x64                   //I2C��������ַ



/************************************************************************
*Ӳ��ʱ��IIC�ڶ˿�ȫ�ֽṹ��.
************************************************************************/
const IIC_PORTS* gsp_halHrtcIICPorts;



/************************************************************************
*Ӳ��ʱ��IIC�ڲ�����
************************************************************************/
#define I2C_HRTC_GETSDA()    (HAL_GPIO_GetPinState(gsp_halHrtcIICPorts->portSDA->port, gsp_halHrtcIICPorts->portSDA->pin))

#define I2C_HRTC_SDAIN()    {HAL_GPIO_SetPinDIR(gsp_halHrtcIICPorts->portSDA->port, gsp_halHrtcIICPorts->portSDA->pin, false);}
#define I2C_HRTC_SDAOUT()   {HAL_GPIO_SetPinDIR(gsp_halHrtcIICPorts->portSDA->port, gsp_halHrtcIICPorts->portSDA->pin, true);}
#define I2C_HRTC_SCKOUT()   {HAL_GPIO_SetPinDIR(gsp_halHrtcIICPorts->portSCK->port, gsp_halHrtcIICPorts->portSCK->pin, true);}

#define I2C_HRTC_SDA_1()    {HAL_GPIO_SetPinState(gsp_halHrtcIICPorts->portSDA->port, gsp_halHrtcIICPorts->portSDA->pin, true);}
#define I2C_HRTC_SDA_0()    {HAL_GPIO_SetPinState(gsp_halHrtcIICPorts->portSDA->port, gsp_halHrtcIICPorts->portSDA->pin, false);}

#define I2C_HRTC_SCK_1()    {HAL_GPIO_SetPinState(gsp_halHrtcIICPorts->portSCK->port, gsp_halHrtcIICPorts->portSCK->pin, true);}
#define I2C_HRTC_SCK_0()    {HAL_GPIO_SetPinState(gsp_halHrtcIICPorts->portSCK->port, gsp_halHrtcIICPorts->portSCK->pin, false);}

    
    
    

/*******************************************************************************
 * @function_name:  _DELAY
 * @function_file:  IIC_HRTC.C
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
 * @function_name:  I2C_HRTC_WriteByte
 * @function_file:  IIC_HRTC.C
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
uint8 I2C_HRTC_WriteByte(uint8 byte)
{
    uint8 err = 0;

    for(uint8 uc_i = 0;uc_i < 8;uc_i++)     //ѭ�������������λ
    {
        I2C_HRTC_SCK_0();                   //ʱ���ߵ�
        _DELAY(100);                         //��ʱ,����
        if(byte & 0x80)                     //����һλ����
        {
            I2C_HRTC_SDA_1();
        }
        else
        {
            I2C_HRTC_SDA_0();
        }
        _DELAY(150);                        //��ʱ,����
        I2C_HRTC_SCK_1();                   //ʱ�Ӹ��ƽ
        _DELAY(200);                        //��ʱ����
        byte <<= 1;                         //��λ
    }
                                            //�ȴ�ACK�ź�
    I2C_HRTC_SCK_0();                       //ʱ�ӵ�
    _DELAY(50);  
    I2C_HRTC_SDAIN();                       //�����߸�Ϊ����
    _DELAY(150);   
                                            //��ʱ����
    I2C_HRTC_SCK_1();                       //ʱ�Ӹ�
    _DELAY(100);                            //��ʱ����
    if(I2C_HRTC_GETSDA())                   //��ȡ������״̬
    {
        err = 1;
        //return 1;                           //NAK,����ERR
    }
    _DELAY(50);                             //��ʱ����
    I2C_HRTC_SCK_0();                       //ʱ�ӵ�
    _DELAY(50);                             //��ʱ����
    //I2C_3231_SDA_1();
    I2C_HRTC_SDAOUT();                      //���������
    return err;
}

/*******************************************************************************
 * @function_name:  I2C_HRTC_ReadByte
 * @function_file:  IIC_HRTC.C
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
uint8 I2C_HRTC_ReadByte(void)
{
    uint8 byte= 0;                          //���յ��ֽ����ݳ�ʼֵ

    I2C_HRTC_SDAIN();                       //����������
    for(uint8 uc_i = 0;uc_i < 8;uc_i++)     //ѭ�����ո�λ����
    {
        I2C_HRTC_SCK_0();                   //ʱ�ӵ�
        _DELAY(200);                        //��ʱ����
        I2C_HRTC_SCK_1();                   //ʱ�Ӹ�
        _DELAY(150);                        //��ʱ����
        byte <<= 1;                         //����λ����
        if(I2C_HRTC_GETSDA())               //��ȡ����λ
        {
            byte |= 0x01;
        }
        _DELAY(50);
    }

    return byte;
}

/*******************************************************************************
 * @function_name:  I2C_HRTC_Start
 * @function_file:  IIC_HRTC.C
 * @����: i2c��������
 * 
 * @����: 
 * @����: 
 * @����: yzy (2019-11-03)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
void I2C_HRTC_Start(void)
{
    I2C_HRTC_SDA_1();                       //�����������
    I2C_HRTC_SCK_1();                       //ʱ���������
    _DELAY(200);                            //��ʱ ����
    I2C_HRTC_SDA_0();                       //�������½���
    _DELAY(200);                            //��ʱ ����
    I2C_HRTC_SCK_0();                       //ʱ���������
    
}

/*******************************************************************************
 * @function_name:  I2C_HRTC_Stop
 * @function_file:  IIC_HRTC.C
 * @����: I2C����ֹͣ
 * 
 * @����: 
 * @����: 
 * @����: yzy (2019-11-03)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
void I2C_HRTC_Stop(void)
{
    //I2C_3231_SCK_0();                       //ʱ���������
    I2C_HRTC_SDA_0();                       //�����������
    _DELAY(200);                            //��ʱ ����
    I2C_HRTC_SCK_1();                       //ʱ���������
    _DELAY(200);                            //��ʱ ����
    I2C_HRTC_SDA_1();                       //������������
    //_DELAY(100);                            //��ʱ ����
    //I2C_3231_SCK_0();                       //ʱ���������

}

/*******************************************************************************
 * @function_name:  I2C_HRTC_ACK
 * @function_file:  IIC_HRTC.C
 * @����: I2C����ACKӦ���ź�
 * 
 * @����: 
 * @����: 
 * @����: yzy (2019-11-03)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
void I2C_HRTC_ACK(void)
{
    
    I2C_HRTC_SCK_0();                       //ʱ���ߵ͵�ƽ
    _DELAY(50);
    I2C_HRTC_SDAOUT();
    _DELAY(100);
    I2C_HRTC_SDA_0();                       //�����ߵ͵�ƽ
    _DELAY(100);                            //��ʱ ����
    I2C_HRTC_SCK_1();                       //ʱ����������
    _DELAY(200);                            //��ʱ ����
    I2C_HRTC_SCK_0();                       //ʱ�����½���
}

/*******************************************************************************
 * @function_name:  I2C_HRTC_NAK
 * @function_file:  IIC_HRTC.C
 * @����: I2C����NAK�ź�
 * 
 * @����: 
 * @����: 
 * @����: yzy (2019-11-03)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
void I2C_HRTC_NAK(void)
{
    I2C_HRTC_SCK_0();                       //ʱ���ߵ͵�ƽ
    _DELAY(50);
    I2C_HRTC_SDAOUT();
    _DELAY(100);
    I2C_HRTC_SDA_1();                       //�����߸��ƽ
    _DELAY(100);                            //��ʱ ����
    I2C_HRTC_SCK_1();                       //ʱ��������
    _DELAY(200);                            //��ʱ ����
    I2C_HRTC_SCK_0();                       //ʱ�����½���
}



/*******************************************************************************
 * @function_name:  I2C_HRTC_ReadBcdTime
 * @function_file:  IIC_HRTC.C
 * @����: 3231ʱ�����ݶ�ȡ
 * 
 * 
 * @����: 
 * @param: time  ʱ�ӻ���,�ֽ�˳��Ϊ��,��,ʱ,��,��,��,��,BCD��ʽ
 * 
 * @����: 
 * @return:  uint8   
 * @����: yzy (2019-11-03)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
uint8 I2C_HRTC_ReadBcdTime(uint8* time)
{
    I2C_HRTC_Start();                       //I2C��������               
    I2C_HRTC_WriteByte(I2C_RTC_ID);         //д��������
    I2C_HRTC_WriteByte(0x00);               //д���ȡ���ݵĵ�ַ

    I2C_HRTC_Start();                       //I2C������������
    I2C_HRTC_WriteByte(I2C_RTC_ID + 1);     //��ȡ��������
    for(uint8 uc_i = 0; uc_i < 7; uc_i++)     //ѭ�������ֽ�����
    {
        time[uc_i] = I2C_HRTC_ReadByte();   //��ȡһ���ֽ�����
        if(uc_i == 6)                       //���һ���ֽڷ���NAK
        {
            I2C_HRTC_NAK();
        }
        else                                //����ACK
        {
            I2C_HRTC_ACK();
        }
    }
    I2C_HRTC_Stop();                        //I2C����ֹͣ
                                            //��ȡ����
    time[3] &= 0x7F;
    time[3] = STR_GetFirstSetBit(time + 3, 1);
    SYS_OK();
}

/*******************************************************************************
 * @function_name:  I2C_HRTC_ReadHexTime
 * @function_file:  IIC_HRTC.C
 * @����: 3231ʱ�����ݶ�ȡ,HEX��ʽ
 * 
 * 
 * @����: 
 * @param: time  ʱ�ӻ���,�ֽ�˳��Ϊ��,��,ʱ,��,��,��,��
 * 
 * @����: 
 * @return:  uint8   
 * @����: yzy (2019-11-03)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
uint8 I2C_HRTC_ReadHexTime(uint8* time)
{
    I2C_HRTC_ReadBcdTime(time);                 //��ȡʱ������
    ByteArrayBcdToHex(time, time, 7);       //��BCDתΪHEX
    SYS_OK();
}

/*******************************************************************************
 * @function_name:  I2C_HRTC_WriteBcdTime
 * @function_file:  IIC_HRTC.C
 * @����: 3231ʱ������д��
 * 
 * 
 * @����: 
 * @param: time  ʱ�ӻ���,�ֽ�˳��Ϊ��,��,ʱ,��,��,��,��,BCD��ʽ
 * 
 * @����: 
 * @return:  uint8   
 * @����: yzy (2019-11-03)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
uint8 I2C_HRTC_WriteBcdTime(uint8* time)
{
    uint8 err = 1;                          //д����Ĭ��err

    time[3] = Bit_Map8[time[3] % 7];        //��ȡ��Ҫд�������
                                            //���д�벻�ɹ�����д,�����д3��
    for(uint8 uc_i = 0;(uc_i < 3) && (err != 0); uc_i++)
    {
        I2C_HRTC_Start();                   //i2c��������
                                            //д����
        err = I2C_HRTC_WriteByte(I2C_RTC_ID);
        err |= I2C_HRTC_WriteByte(0x00);    //д�������ַ

        for(uint8 uc_j = 0;uc_j < 7;uc_j++) //ѭ��д������ֽ�
        {
          //_DELAY(50);
            err |= I2C_HRTC_WriteByte(time[uc_j]);
        }
        I2C_HRTC_Stop();                    //i2c����ֹͣ
    }
    return err;                             //���ؽ��
}

/*******************************************************************************
 * @function_name:  I2C_HRTC_WriteHexTime
 * @function_file:  IIC_HRTC.C
 * @����: 3231ʱ������д��HEX��ʽд��
 * 
 * 
 * @����: 
 * @param: time  ʱ�ӻ���,�ֽ�˳��Ϊ��,��,ʱ,��,��,��,��
 * 
 * @����: 
 * @return:  uint8   
 * @����: yzy (2019-11-03)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
uint8 I2C_HRTC_WriteHexTime(uint8* time)
{
    ByteArrayHexToBcd(time, time, 7);       //��HEXת��BCD
    return I2C_HRTC_WriteBcdTime(time);         //д��ʱ������
}

/*******************************************************************************
 * @function_name:  I2C_HRTC_SecIntOnOff
 * @function_file:  IIC_HRTC.C
 * @����: Ӳʱ�����ж�������غ���
 * 
 * 
 * @����: 
 * @param: oc  true��, false�ر�
 * 
 * @����: 
 * @return:  uint8   
 * @����: yzy (2019-11-3)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
uint8 I2C_HRTC_SecIntOnOff(bool oc)
{
    uint8 uc_data[3];                       //д������ݻ���
    uint8 err = 1;                          //������Ĭ��Ϊ1

    ClearBuffer(uc_data, sizeof(uc_data));
    if(!oc)                                 //�ر�,д��0x40
    {
        uc_data[2] = 0x40;
    }
    else                                    //��,д��0x60
    {
        uc_data[2] = 0x60;
    }
                                            //д������,ʧ������,���Դ���3��
    for(uint8 uc_i = 0;(uc_i < 3) && (err != 0);uc_i++)
    {
        I2C_HRTC_Start();                   //I2C��������
                                            //д����
        err = I2C_HRTC_WriteByte(I2C_RTC_ID);
        err |= I2C_HRTC_WriteByte(0x0D);     //д��ĵ�ַ
        for(uint8 uc_j = 0;uc_j < 3;uc_j++)
        {                                   //д������,�򿪻��߹ر����ж�
            err |= I2C_HRTC_WriteByte(uc_data[uc_j]);
        }
        I2C_HRTC_Stop();                    //I2C����ֹͣ
    }
    return err;
}

/*******************************************************************************
 * @function_name:  Init_I2C_HRTC
 * @function_file:  IIC_HRTC.C
 * @����: Ӳʱ��I2C���߳�ʼ��
 * 
 * @����: 
 * @����: 
 * @����: yzy (2019-11-3)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
void Init_I2C_HRTC(const IIC_PORTS* ports)
{
    uint8 uc_i;
                                            //����SDA�˿ں�SCK�˿�
    HAL_GPIO_PinConfig(ports->portSDA->port, ports->portSDA->pin, ports->portSDA->mode);
    HAL_GPIO_PinConfig(ports->portSCK->port, ports->portSCK->pin, ports->portSCK->mode);
                                            //��ֵȫ�ֽṹ��
    gsp_halHrtcIICPorts = ports;
    
    I2C_HRTC_SDAIN();                       //�����ź���Ĭ������
    I2C_HRTC_SCKOUT();                      //ʱ���ź���Ĭ�����
    I2C_HRTC_SCK_1();                       //Ĭ������͵�ƽ

    for(uc_i = 0;uc_i < 10;uc_i++)          //ѭ�����������״̬,��ʱ������Ϊ����̬
    {
        _DELAY(150);
        if(I2C_HRTC_GETSDA())               //�ߵ�ƽ,�����һ��Start�ź�
        {
            I2C_HRTC_SDAOUT();
            I2C_HRTC_SDA_1();
            break;
        }
                                            //�����ߵ͵�ƽ,���������,�任ʱ����(������),����ʱ��
        I2C_HRTC_SCK_0();                   //ʱ�������
        _DELAY(200);                        //��ʱ����
        I2C_HRTC_SCK_1();                   //ʱ�������(������)
        _DELAY(200);                        //��ʱ����
    }
    if(uc_i == 10)                          //10��ʱ��������,δ�ܽ�������ʱ��,����
    {
        return;
    }
    I2C_HRTC_SCK_0();
    _DELAY(200);
    I2C_HRTC_SCK_1();
    _DELAY(150);
    I2C_HRTC_SDA_1();

    //I2C_3231_F32KOFF();                     //Ĭ�Ϲر�32K�ź����
   // I2C_HRTC_SecIntOnOff(true);             //��ʼ�������ж����
}


