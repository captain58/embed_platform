/****************************************Copyright (c)*************************
**                               	________科技有限公司
**                                     		开发部
**
**
**--------------文件信息-------------------------------------------------------
**文   件   名: dev_gpio.h
**创   建   人: yzy
**最后修改日期: 2010年3月10日
**描        述: 普通输入输出(包括LED输出)端口管理进程函数配置头文件
**注        意:
**--------------历史版本信息---------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2010年3月10日
** 描　述: 原始版本
******************************************************************************/
#ifndef _DEV_GPI_H_
#define _DEV_GPI_H_

#ifndef EXT_DEV_GPI
#define EXT_DEV_GPI extern
#endif 






/******************************************************************************
**GPI输入端口扫描结构体信息
******************************************************************************/
typedef struct
{
    int gpistt;                          //慢速输入端口状态
    int keyfevt;                         //按键下降沿事件
    int keyrevt;                         //按键上升沿事件
    int keylevt;                         //按键保持事件
    int keyfevtbak;                      //按键下降沿事件备份
    int keyrevtbak;                      //按键上升沿事件备份
    int keylevtbak;                      //按键保持事件备份
    //
    int fgpistt;                         //快速输入端口状态
    int fgpifevt;                        //快速端口下降沿
    int fgpirevt;                        //快速端口上升沿
    int fgpifevtbak;                     //快速端口下降沿备份
    int fgpirevtbak;                     //快速端口上升沿备份
    //
#if LGPI_KEY_NUM > 0
    uint8 cnt[LGPI_KEY_NUM];                //输入端口扫描计数器
    uint8 lastcnt[LGPI_KEY_NUM];            //按键持续时间计数器
#endif
    uint8 fcnt[2];                          //快速端口扫描计数器
}GPIO;

EXT_DEV_GPI  GPIO* gsp_GpioStt;













/*******************************************************************************
*输入端口事件类型枚举
*******************************************************************************/
typedef enum
{
    GPI_TYPE_LIFEVT = 0,            //慢速下降沿
    GPI_TYPE_LIREVT,                //慢速上升沿
    GPI_TYPE_LILEVT,                //按键保持
    GPI_TYPE_FIFEVT,                //快速下降沿
    GPI_TYPE_FIREVT,                //快速上升沿

}GPI_T;


/*******************************************************************************
 * @function_name:  SYS_GPI_Init
 * @function_file:  gpi.c
 * @描述: 
 * 
 * @参数: 
 * @返回: 
 * @作者: yzy (2010-03-10)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
void SYS_GPI_Init(void);

/*******************************************************************************
 * @function_name:  SYS_HALL_Init
 * @function_file:  gpi.c
 * @描述: 
 * 
 * @参数: hall 霍尔回调函数
 * @返回: 
 * @作者: yzy (2010-03-10)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
void SYS_HALL_Init(SCallback * hall);

/*******************************************************************************
 * @function_name:  SYS_GPI_Event
 * @function_file:  dev_gpio.c
 * @描述: 获取输入口事件
 * 
 * 
 * @参数: 
 * @param: type事件类型,格式为GPI_T
 * 
 * @返回: 
 * @return:  uint16   
 * @作者: yzy (2018年3月22日)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
uint32 SYS_GPI_Event(uint8 type);

/*******************************************************************************
 * @function_name:  SYS_GPI_GetStt
 * @function_file:  dev_gpio.c
 * @描述: 获取输入口输入状态
 * 
 * 
 * @参数: 
 * @param: type 0:慢速口输入状态
 *              1:快速口输入状态
 * 
 * @返回: 
 * @return:  uint32   
 * @作者: yzy (2018年3月22日)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
uint32 SYS_GPI_GetStt(uint8 type);
/*******************************************************************************
 * @function_name:  SYS_GPI_Get_PinStt
 * @function_file:  dev_gpio.c
 * @描述: 获取输入口输入状态
 * 
 * 
 * @参数: 
 * @param: type 0:慢速口输入状态
 *              1:快速口输入状态
 *          pin 指定哪一根脚 
 * @返回: 
 * @return:  uint32   
 * @作者: yzy (2018年3月22日)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/

bool SYS_GPI_Get_PinStt(uint8 type, GPIENUM pin);

/*******************************************************************************
 * @function_name:  SYS_GPI_EvtMessageLoop
 * @function_file:  dev_gpio.c
 * @描述: GPI端口事件消息分发机制
 * 
 * @参数: 
 * @返回: 
 * @作者: yzy (2010-03-13)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
void SYS_GPI_EvtMessageLoop(void);
#if SYS_INT_EN > 0

/*******************************************************************************
 * @function_name:  SYS_KEY_Open
 * @function_file:  dev_gpi.c
 * @描述: 打开按键事件
 * 
 * 
 * @参数: 
 * @param: key
 * 
 * @返回: 
 * @return:  uint16   
 * @作者: yzy (2018-03-02)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
void SYS_KEY_Open(const InterruptID* Iid);
/*******************************************************************************
 * @function_name:  SYS_KEY_Close
 * @function_file:  dev_gpi.c
 * @描述: 关闭按键事件
 * 
 * 
 * @参数: 
 * @param: key
 * 
 * @返回: 
 * @return:  uint16   
 * @作者: yzy (2018-03-02)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
void SYS_KEY_Close(const InterruptID* Iid);
#endif
void SYS_KEY_Start(void);
void SYS_GPI_Event_Clear(void);

#endif
