/******************************Copyright(c)******************************
**                         
**
** File Name: dev_uart.h
** Author: 
** Date Last Update: 2019-11-02
** Description: UART串口通道管理进程头文件
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: 文件创建
*************************************************************************/
#ifndef _DEV_UART_H_
#define _DEV_UART_H_

#ifndef EXT_DEV_UART
#define EXT_DEV_UART    extern
#endif



#include "aos/kernel.h"

/******************************************************************************
**串口接收数据导出的回调函数原型
******************************************************************************/
typedef void (*TDataExport) (uint8 port, uint8* buffer, uint16 length);



#if (SYS_SER_EN > 0)

/******************************************************************************
**串口进程管理相关
******************************************************************************/
EXT_DEV_UART aos_sem_t gss_UartRes[NO_OF_SERIAL];              //串口占用资源量,同一时刻只能被一个进程占用
EXT_DEV_UART uint8  gucs_UartUserTkid[NO_OF_SERIAL];        //当前占用该端口的进程id,0xFF表示未被占用.



/******************************************************************************
**串口缓存接收状态
******************************************************************************/
EXT_DEV_UART uint8_t gucs_UartRevFlag[NO_OF_SERIAL];
EXT_DEV_UART uint8_t gucs_UartInited[NO_OF_SERIAL];
EXT_DEV_UART uint8_t gucs_UartInited[NO_OF_SERIAL];

/*******************************************************************************
**串口处理回调函数
********************************************************************************/
EXT_DEV_UART TCallback gfs_SerialCallBack[NO_OF_SERIAL];    //__NULL表示无回调函数


#endif              //#if (SYS_UART_EN > 0)

void UART_Init(void);

/************************************************************************
 * @Function: SYS_SER_Init
 * @Description: 串口配置
 * 
 * @Arguments: 
 * @param: port 串口序号(非硬件串口号)
 * @param: ss 
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void SYS_SER_Init(uint8 port, void * ss);


/************************************************************************
 * @Function: SYS_SER_OpenCallBack
 * @Description: 串口打开
 * 
 * @Arguments: 
 * @param: port 串口序号(非硬件串口号)
 * @param: isAnsyc true:异步,打不开则退出.false:同步,等待直到获取资源
 * @param: cbs 回调函数,__NULL表示空(回调函数中不可有非实时操作,或发送同步消息,或TKsuspend)
 * 
 * @Note: 
 * @Return: bool  
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
bool SYS_SER_OpenCallBack(uint8 port, bool isAnsyc, TCallback cbs);


/************************************************************************
 * @Function: SYS_SER_Open
 * @Description: 串口打开
 * 
 * @Arguments: 
 * @param: port 串口序号(非硬件串口号)
 * @param: isAnsyc true:异步,打不开则退出.false:同步,等待直到获取资源
 * 
 * @Note: 
 * @Return: bool  
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
bool SYS_SER_Open(uint8 port, bool isAnsyc);


/************************************************************************
 * @Function: SYS_SER_Close
 * @Description: 串口关闭
 * 
 * @Arguments: 
 * @param: port 串口序号(非硬件串口号)
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void SYS_SER_Close(uint8 port);


/************************************************************************
 * @Function: SYS_SER_Clear
 * @Description: 清空串口缓存
 * 
 * @Arguments: 
 * @param: port 串口序号(非硬件串口号)
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void SYS_SER_Clear(uint8_t port);


/************************************************************************
 * @Function: SYS_SER_ReadDataExport
 * @Description: 串口接收数据的复制导出,串口缓存中数据不删除
 * 
 * @Arguments: 
 * @param: port 串口序号(非硬件串口号)
 * @param: func 导出函数
 * 
 * @Note: 
 * @Return: uint16 导出的数据
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 代码实现参考 SYS_SER_Clear()
 ************************************************************************/
void SYS_SER_ReadDataExport(uint8 port, TDataExport func);


/************************************************************************
 * @Function: SYS_SER_WriteOption
 * @Description: 串口数据发送
 * 
 * @Arguments: 
 * @param: port 串口序号(非硬件串口号)
 * @param: buffer 发送数据缓存
 * @param: length 发送数据长度
 * @param: opt 选项:b0=1串口性能优先,b0=0系统开销优先
 * 
 * @Note: 
 * @Return: uint8  
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
int SYS_SER_WriteOption(uint8 port, uint8* buffer, uint16 length, uint16 opt);


/************************************************************************
 * @Function: SYS_SER_Write
 * @Description: 串口数据发送
 * 
 * @Arguments: 
 * @param: port 串口序号(非硬件串口号)
 * @param: buffer 发送数据缓存
 * @param: length 发送数据长度
 * 
 * @Note: 
 * @Return: uint8  
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8_t SYS_SER_Write(uint8_t port, uint8_t* buffer, uint16_t length);


/************************************************************************
 * @Function: SYS_SER_Read
 * @Description: 串口数据读取
 * 
 * @Arguments: 
 * @param: port 串口序号(非硬件串口号)
 * @param: buffer 输出缓存,__NULL表示丢弃
 * @param: length 读取长度
 * 
 * @Note: 
 * @Return: uint16  
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint16_t SYS_SER_Read(uint8_t port, uint8_t* buffer, uint16_t length);


/************************************************************************
 * @Function: SYS_SER_Peek
 * @Description: 串口接收数据窥探,执行后数据仍然存在于接收缓存内
 * 
 * @Arguments: 
 * @param: port 串口序号(非硬件串口号)
 * @param: buffer 存放窥探数据的缓存
 * @param: start 从第几个数据开始窥探(0-) 
 * @param: length 待窥探数据长度
 * 
 * @Note: 
 * @Return: uint16  窥探得到的实际数据长度
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint16 SYS_SER_Peek(uint8 port, uint8* buffer, uint16 start, uint16 length);


/************************************************************************
 * @Function: SYS_SER_ReadByte
 * @Description: 从串口读取一个字节数据
 * 
 * @Arguments: 
 * @param: port 串口序号(非硬件串口号)
 * 
 * @Note: 
 * @Return: uint16  
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint16 SYS_SER_ReadByte(uint8 port);


/************************************************************************
 * @Function: SYS_SER_ToBeRead
 * @Description: 串口可读取数据的长度
 * 
 * @Arguments: 
 * @param: port 串口序号(非硬件串口号)
 * 
 * @Note: 
 * @Return: uint16  
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint16 SYS_SER_ToBeRead(uint8 port);


/************************************************************************
 * @Function: SYS_SER_Line
 * @Description: 串口读取行命令(若未找到行结束符,串口缓存被读空)
 * 
 * @Arguments: 
 * @param: port 串口序号(非硬件串口号)
 * @param: buffer 存放数据缓存,__NULL表示丢弃
 * @param: length 存放数据缓存的长度
 * 
 * @Note: 
 * @Return: uint16  读取的行长度(包括一个结尾符).
 *          0表示无数据或未找到行结束符,1表示仅读到结尾符
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint16 SYS_SER_Line(uint8 port, uint8* buffer, uint16 length);


/************************************************************************
 * @Function: SYS_SER_LineLen
 * @Description: 获取串口当前行长度
 * 
 * @Arguments: 
 * @param: port 串口序号(非硬件串口号)
 * 
 * @Note: 
 * @Return: uint16  行长度(包括一个结尾符)
 *          0表示无数据或未找到行结束符,1表示仅读到结尾符
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint16 SYS_SER_LineLen(uint8 port);


/************************************************************************
 * @Function: SYS_SER_PrintValueOption
 * @Description: 发送数据,按照字符串的格式化进行发送
 * 
 * @Arguments: 
 * @param: port 串口序号(非硬件串口号)
 * @param: str 发送的描述字符串
 * @param: value 发送的数据
 * @param: opt 选项:b0=1串口性能优先,b0=0系统开销优先
 * 
 * @Note: 
 * @Return: uint8  
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8 SYS_SER_PrintValueOption(uint8 port, string str, uint32 value, uint8 opt);


/************************************************************************
 * @Function: SYS_SER_PrintValue
 * @Description: 发送数据,按照字符串的格式化进行发送
 * 
 * @Arguments: 
 * @param: port 串口序号(非硬件串口号)
 * @param: str 发送的描述字符串
 * @param: value 发送的数据
 * 
 * @Note: 
 * @Return: uint8  成功/失败
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8 SYS_SER_PrintValue(uint8 port, string str, uint32 value);


/************************************************************************
 * @Function: SYS_UART_Init
 * @Description: 串口模块初始化
 * @Arguments: 
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void SYS_UART_Init(void);
void SYS_UART_DeInit(void);


/************************************************************************
 * @Function: SYS_UART_EvtMessageLoop
 * @Description: UART串口数据接收消息分发机制
 * @Arguments: 
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void SYS_UART_EvtMessageLoop(void);
void SYS_UART_Close(uint8 port);
void SYS_UART_Open(uint8 port);


uint32_t SYS_SER_Recv_IT(uint8 port, void *data, uint32_t expect_size,
                      uint32_t *recv_size, uint32_t timeout);


#endif
