/******************************Copyright(c)******************************
**                           
**
** File Name: hal_uartx.c
** Author: 
** Date Last Update: 2019-11-02
** Description: 通用串口工具
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: 文件创建
*************************************************************************/
#ifndef _HAL_UART_H_
#define _HAL_UART_H_


#ifndef EXT_HAL_UART
#define EXT_HAL_UART    extern
#endif







/************************************************************************
 * @function: Uartx_Config
 * @描述: 配置串口的通讯参数
 * 
 * @参数: 
 * @param: ss 串口的配置
 * @返回: 
 * @说明: 这个函数是普通中断安全的,但不是进程安全的
 * @作者: yzy (2014/5/22)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
int Uartx_Config(SerialSets* ss, const SerialID* sid);


/************************************************************************
 * @function: Init_Uartx
 * @描述: 初始化Uartx的寄存器组,并配置相关信息
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/5/22)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
int32_t Uartx_Init(SerialID* sid, SerialSets* ss);

uint8 Uartx_DeInit(const SerialID* sid);

/************************************************************************
 * @function: Uartx_StartSend
 * @描述: 启动Uart的发送
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/5/22)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	void Uartx_StartSend(const SerialID* sid);
int32_t hal_uart_send(const SerialID* sid, const void *data, uint32_t size, uint32_t timeout);

void Uartx_Close(const SerialID* sid);
void Uartx_Open(const SerialID* sid);

#endif
















