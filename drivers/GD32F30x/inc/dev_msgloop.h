/******************************Copyright(c)******************************
**                          
**
** File Name: dev_msgloop.h
** Author: 
** Date Last Update: 2019-11-02
** Description: 消息循环机制头文件
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: 文件创建
*************************************************************************/
#ifndef _DEV_MSGLOOP_H_
#define _DEV_MSGLOOP_H_

#ifndef EXT_DEV_MSGLOOP
#define EXT_DEV_MSGLOOP extern
#endif


/************************************************************************
**用户需要申请的消息的种类的寄存器
************************************************************************/
#if (SYS_SER_EN > 0)
//串口消息申请进程信息
EXT_DEV_MSGLOOP uint8 gucs_MsgUartTkMap[NO_OF_SERIAL][SYS_TK_NUM >> 3];

#endif
/******************************************************************************
**需要分发的消息类别
******************************************************************************/
//#define MSG_CLS_IN      0x00            //内部通道的消息类别
//#define MSG_CLS_OU      0x01            //外部通道的消息类别
#define MSG_CLS_UART    0x02            //串口消息的消息类别
#define MSG_CLS_GPIO    0x03            //GPIO的消息类别    
#define MSG_CLS_TM      0x04            //时间组建的消息类别
#define MSG_CLS_MD      0x05            //Modem通道的消息类别
#define MSG_CLS_ETH     0x06            //以太网通道的消息
//其他消息申请进程信息
//	#ifndef __NO_SYS__
EXT_DEV_MSGLOOP uint8 guc_MsgApplied[SYS_TK_NUM];
EXT_DEV_MSGLOOP uint8 guc_MsgUartTkMap[NO_OF_SERIAL][SYS_TK_NUM >> 3];
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
 * @Description: 消息分发模块初始化
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
 * @Description: 申请消息
 * 
 * @Arguments: 
 * @param: msgcls 消息大类
 * 
 * @Note: 
 * @Return: bool  
 * @Auther: yzy
 * Date: 2015/5/27
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8_t SYS_MSG_Apply(uint32_t tkid, uint8_t msgcls);

/************************************************************************
 * @Function: SYS_MSG_ApplyExt
 * @Description: 申请消息(扩充函数)
 * 
 * @Arguments: 
 * @param: msgcls 消息大类
 * @param: subcls 消息小类,当大类为串口消息时,小类为串口号
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







