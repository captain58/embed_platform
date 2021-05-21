/******************************Copyright(c)******************************
**                                
**
** File Name: dev_gpo.h
** Author: 
** Date Last Update: 2019-11-02
** Description: ��ͨ�����ͷ�ļ�
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: �ļ�����
*************************************************************************/
#ifndef _DEV_GPO_H_
#define _DEV_GPO_H_

#ifndef EXT_DEV_GPO
#define EXT_DEV_GPO extern
#endif 




/************************************************************************
 * @Function: SYS_GPO_Init
 * @Description: GPO�豸��ʼ��
 * @Arguments: 
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/27
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void SYS_GPO_Init(void);


/************************************************************************
 * @Function: SYS_GPO_Out
 * @Description: 
 * 
 * @Arguments: 
 * @param: gpo_name ����ڱ��ö��(GPO_ENUM)
 * @param: oc -true ����ߵ�ƽ, false ����͵�ƽ
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/27
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void SYS_GPO_Out(uint32 gpo_name, bool oc);
void SYS_GPO_WakeUp(void);
void SYS_GPO_Idel(void);


#endif
