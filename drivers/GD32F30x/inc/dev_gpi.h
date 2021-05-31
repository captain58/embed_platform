/******************************Copyright(c)******************************
**                          
**
** File Name: dev_gpi.h
** Author: 
** Date Last Update: 2019-11-02
** Description: 普通输入口设备头文件
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: 文件创建
*************************************************************************/
#ifndef _DEV_GPI_H_
#define _DEV_GPI_H_

#ifndef EXT_DEV_GPI
#define EXT_DEV_GPI extern
#endif 





/************************************************************************
**GPI输入端口扫描结构体信息
*************************************************************************/
typedef struct
{
    uint32 gpistt;                          //慢速输入端口状态
    uint32 keyfevt;                         //按键下降沿事件
    uint32 keyrevt;                         //按键上升沿事件
    uint32 keylevt;                         //按键保持事件
    uint32 keyfevtbak;                      //按键下降沿事件备份
    uint32 keyrevtbak;                      //按键上升沿事件备份
    uint32 keylevtbak;                      //按键保持事件备份
    //
    uint32 fgpistt;                         //快速输入端口状态
    uint32 fgpifevt;                        //快速端口下降沿
    uint32 fgpirevt;                        //快速端口上升沿
    uint32 fgpifevtbak;                     //快速端口下降沿备份
    uint32 fgpirevtbak;                     //快速端口上升沿备份
    
    //模块内部使用,对外只读
#if (SYS_LGPI_EN > 0)
    uint8 cnt[LGPI_PORT_NUM];                //输入端口扫描计数器
    uint8 lastcnt[LGPI_PORT_NUM];            //按键持续时间计数器
#endif
    
#if (SYS_FGPI_EN > 0)
    uint8 fcnt[FGPI_PORT_NUM];               //快速端口扫描计数器
#endif
}GPIO_IN;

EXT_DEV_GPI GPIO_IN * gsp_GpioStt;













/************************************************************************
*输入端口事件类型枚举
*************************************************************************/
typedef enum
{
    GPI_TYPE_LIFEVT = 0,            //慢速下降沿
    GPI_TYPE_LIREVT,                //慢速上升沿
    GPI_TYPE_LILEVT,                //按键保持
    GPI_TYPE_FIFEVT,                //快速下降沿
    GPI_TYPE_FIREVT,                //快速上升沿

}GPI_ENUM;


/************************************************************************
 * @Function: SYS_GPI_Init
 * @Description: 输入口模块初始化
 * @Arguments: 
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/27
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void SYS_GPI_Init(void);

void SYS_GPI_WakeUp(void);

/************************************************************************
 * @Function: SYS_GPI_EvtMessageLoop
 * @Description: GPI事件消息分发函数,提供给消息单元
 * @Arguments: 
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/27
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void SYS_GPI_EvtMessageLoop(void);

/************************************************************************
 * @Function: SYS_GPI_GetStt
 * @Description: 获取输入口输入状态
 * 
 * @Arguments: 
 * @param: type 0:慢速口输入状态
 *              1:快速口输入状态
 * 
 * @Note: 
 * @Return: uint32  
 * @Auther: yzy
 * Date: 2015/5/27
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint32 SYS_GPI_GetStt(uint8 type);

/************************************************************************
 * @Function: SYS_GPI_Event
 * @Description: 获取输入口事件
 * 
 * @Arguments: 
 * @param: type 事件类型枚举,类型为GPI_ENUM
 * 
 * @Note: 
 * @Return: uint32  
 * @Auther: yzy
 * Date: 2015/5/27
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint32 SYS_GPI_Event(uint8 type);




#endif
