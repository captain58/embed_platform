/****************************************Copyright (c)**************************************************
**                                  ________�Ƽ����޹�˾
**                                          ������
**
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: dev_system.h
**��   ��   ��: yzy
**����޸�����: 2018��3��7��
**��        ��: ϵͳ�豸
**ע        ��:
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: yzy
** ��  ��: v1.0
** �ա���: 2018��3��7��
** �衡��: ԭʼ�汾
********************************************************************************************************/


#ifndef _EXT_DEV_SYSTEM_H_
#define _EXT_DEV_SYSTEM_H_



typedef struct
{
    TDevConst devc;
    void (*EnterLPC)(void);
    void (*SetTime)(TIME* datetime);
    void (*GetTime)(TIME* datetime);
    void (*FeedDOG)(void);
    void (*OpenSWDOG)(uint8);
    void (*Reset)(void);

}TDevSys;


/***********************************************************
 * @function_name: DEV_SysInit
 * @function_file: freq.c
 * @����:������ϵͳ��Ϊһ���豸����
 * 
 * @����: 
 * @����: 
 * @����:
 *---------------------------------------------------------
 * @�޸���: houpm (2010/2/18)
 **********************************************************/
void DEV_SysInit(void);

#endif








