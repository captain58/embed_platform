/******************************Copyright(c)******************************
**                          
**
** File Name: dev_msgloop.h
** Author: 
** Date Last Update: 2019-11-02
** Description: ��Ϣѭ������ͷ�ļ�
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: �ļ�����
*************************************************************************/
#ifndef _DEV_MSGLOOP_H_
#define _DEV_MSGLOOP_H_

#ifndef EXT_DEV_MSGLOOP
#define EXT_DEV_MSGLOOP extern
#endif


/************************************************************************
**�û���Ҫ�������Ϣ������ļĴ���
************************************************************************/
#if (SYS_UART_EN > 0)
//������Ϣ���������Ϣ
EXT_DEV_MSGLOOP uint8 gucs_MsgUartTkMap[NO_OF_SERIAL][SYS_TK_NUM >> 3];

#endif

//������Ϣ���������Ϣ
//	#ifndef __NO_SYS__
EXT_DEV_MSGLOOP uint8 gucs_MsgApplied[SYS_TK_NUM];
//	#else
//	typedef struct _STMsgApply
//	{
//	    uint8_t applied;
//	    uint8_t msg;
//	}STMsgApply;
//	
//	EXT_DEV_MSGLOOP STMsgApply gucs_MsgApplied[SYS_TK_NUM];
//	
//	#endif




/************************************************************************
 * @Function: SYS_MSG_Init
 * @Description: ��Ϣ�ַ�ģ���ʼ��
 * @Arguments: 
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/27
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void SYS_MSG_Init(void);
void SYS_MSG_PreInit(void);
void SYS_MSG_DeInit(void);

/************************************************************************
 * @Function: SYS_MSG_Apply
 * @Description: ������Ϣ
 * 
 * @Arguments: 
 * @param: msgcls ��Ϣ����
 * 
 * @Note: 
 * @Return: bool  
 * @Auther: yzy
 * Date: 2015/5/27
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
bool SYS_MSG_Apply(uint8_t tkid, uint8_t msgcls);

/************************************************************************
 * @Function: SYS_MSG_ApplyExt
 * @Description: ������Ϣ(���亯��)
 * 
 * @Arguments: 
 * @param: msgcls ��Ϣ����
 * @param: subcls ��ϢС��,������Ϊ������Ϣʱ,С��Ϊ���ں�
 * 
 * @Note: 
 * @Return: bool  
 * @Auther: yzy
 * Date: 2015/5/27
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8_t SYS_MSG_ApplyExt(uint8_t tkid, uint8_t msgcls, uint8_t subcls);


#endif







