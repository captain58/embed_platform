/****************************************Copyright (c)**************************************************
**                                  ________�Ƽ����޹�˾
**                                          ������
**
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: fram.h
**��   ��   ��: yzy
**����޸�����: 2007��8��7��
**��        ��: ��������ģ�������ͷ�ļ�
**ע        ��:
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: yzy
** ��  ��: v1.0
** �ա���: 2007��8��7��
** �衡��: ԭʼ�汾
********************************************************************************************************/

#ifndef _FRAM_H_
#define _FRAM_H_


#define FRAM_ERR_EXIST      (uint8)0x10
#define FRAM_ERR_READ       (uint8)0x20
#define FRAM_ERR_WRITE      (uint8)0x40

#define FRAM_ERR_OK         (uint8)0
#define FRAM_ERR_VAR        (uint8)1
#define FRAM_ERR_FT         (uint8)2

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
TResult FRAM_Read(uint8* buffer, uint32 address, uint16 length);

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
uint8 FRAM_PreWrite(void);

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
TResult FRAM_Write(uint8* buffer, uint32 address, uint16 length);

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
uint8 HAL_InitFRAM(void);




#endif
