/******************************Copyright(c)******************************
**                         
**
** File Name: dev_iap.h
** Author: 
** Date Last Update: 2019-11-02
** Description: 
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: �ļ�����
*************************************************************************/

#ifndef _EXT_DEV_IAP_
#define _EXT_DEV_IAP_

#ifndef EXT_DEV_IAP
#define EXT_DEV_IAP
#endif


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
bool SYS_IAP_Ready(uint32 address, uint32 length, bool isreset);



#endif


