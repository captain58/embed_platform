/******************************Copyright(c)******************************
**                          
**
** File Name: mxflash.c
** Author: 
** Date Last Update: 2015-05-18
** Description:  Flash��������
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: �ļ�����
*************************************************************************/

#ifndef _IFLASH_H_
#define _IFLASH_H_
/*******************************************************************************
** @function_name:  HAL_IFLASH_Read
** @function_file:  mxflash.c
** @����: ��Flashָ����ַ��ȡָ�����ȵ��ֽ�����
** 
** 
** @����: 
** @param: buffer(uint8*): ���ݻ���
** @param: address(uint32): ��ַ
** @param: length(uint16): ��ȡ�����ݳ���
** 
** @����: 
** @return:  uint8   
** @����: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @�޸���: 
** @�޸�˵��: 
*******************************************************************************/
uint8 HAL_IFLASH_Read(uint8* buffer, uint32_t address, uint16_t length);

/*******************************************************************************
** @function_name:  HAL_IFLASH_Write
** @function_file:  mxflash.c
** @����: Flash д����
** 
** 
** @����: 
** @param: buffer(uint8*): ��д�����ݵĴ�Ż���
** @param: address(uint32): flashƬ�ڵ�ַ
** @param: length(uint16): ���ݳ���
** @param: x(uint8): �ڼ�Ƭflash
** @param: sdmode(uint8): �������ݱ���ģʽ.[0.������������(Ĭ��),1.��������λ�ڴ�д����֮ǰ������,2.ɾ����������]
** 
** @����: 
** @return:  uint8   
** @����: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @�޸���: yzy(20111-01-10)
** @�޸�˵��: ���ĺ���ʵ�ַ�ʽ,�򻯳���
*******************************************************************************/
uint8 HAL_IFLASH_Write(uint8* buffer, uint32 addr, uint16 length);
/*******************************************************************************
** @function_name:  HAL_IFLASH_Set
** @function_file:  Iflash.c
** @����: ��Flashĳ��������Ϊ�ض�ֵ 
** 
** 
** @����: 
** @param: templet(uint8): ���������
** @param: address(uint32): д�����ݵĵ�ַ
** @param: length(uint16): д�����ݵĳ���
** @param: x(uint8): flashоƬ���
** 
** @����: 
** @return:  uint8   
** @����: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @�޸���: yzy
** @�޸�˵��: ��������ͬ WriteFlsInChip()
*******************************************************************************/
uint8 HAL_IFLASH_Set(uint8 templet, uint32 addr, uint16 length);
/*******************************************************************************
** @function_name:  HAL_IFLASH_Erase
** @function_file:  Iflash.c
** @����: MX Flash �߳�����
** 
** 
** @����: 
** @param: address: ��ʼ��ַ
** @param: sectornum ��������
** 
** @����: 
** @return:  uint8   
** @����: yzy (2010-05-10)
**-----------------------------------------------------------------------------
** @�޸���: 
** @�޸�˵��: 
*******************************************************************************/
uint8 HAL_IFLASH_Erase(uint32 address, uint32 sectornum);

/*******************************************************************************
** @function_name:  HAL_IFLASH_WriteSpec
** @function_file:  Iflash.c
** @����: Flash д����
** 
** 
** @����: 
** @param: buffer(unsigned char*): ��д�����ݵĴ�Ż���
** @param: address(unsigned long): flashƬ�ڵ�ַ
** @param: length(unsigned short): ���ݳ���
** @param: sdmode(unsigned char): �������ݱ���ģʽ.[0.������������(Ĭ��),1.��������λ�ڴ�д����֮ǰ������,2.ɾ����������]
** 
** @����: 
** @return:  unsigned char   
** @����: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @�޸���: yzy(20111-01-10)
** @�޸�˵��: ���ĺ���ʵ�ַ�ʽ,�򻯳���
*******************************************************************************/
unsigned char HAL_IFLASH_WriteSpec(unsigned char* buffer, unsigned long addr, unsigned short length, unsigned char sdmode);

uint8 HAL_IFlash_Init(void);

#endif
