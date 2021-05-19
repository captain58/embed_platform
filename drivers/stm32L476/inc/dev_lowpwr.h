/****************************************Copyright (c)**************************************************
**                                  ________科技有限公司
**                                          开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: dev_lowpwr.h
**创   建   人: yzy
**最后修改日期: 2018年4月25日
**描        述: 低功耗驱动模块的头文件
**注        意:
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2018年4月25日
** 描　述: 原始版本
********************************************************************************************************/
#ifndef _DEV_LOWPWR_H_
#define _DEV_LOWPWR_H_

#ifndef EXT_DEV_LOWPWR
#define EXT_DEV_LOWPWR extern
#endif

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#ifndef __TYPEDEF__
#define DI      __disable_interrupt
#define EI      __enable_interrupt
#define HALT    __halt
#define NOP     __no_operation
#define STOP    __stop
/* Status list definition */
#define MD_STATUSBASE        (0x00U)
#define MD_OK                (MD_STATUSBASE + 0x00U) /* register setting OK */
#define MD_SPT               (MD_STATUSBASE + 0x01U) /* IIC stop */
#define MD_NACK              (MD_STATUSBASE + 0x02U) /* IIC no ACK */
#define MD_BUSY1             (MD_STATUSBASE + 0x03U) /* busy 1 */
#define MD_BUSY2             (MD_STATUSBASE + 0x04U) /* busy 2 */

/* Error list definition */
#define MD_ERRORBASE         (0x80U)
#define MD_ERROR             (MD_ERRORBASE + 0x00U)  /* error */
#define MD_ARGERROR          (MD_ERRORBASE + 0x01U)  /* error agrument input error */
#define MD_ERROR1            (MD_ERRORBASE + 0x02U)  /* error 1 */
#define MD_ERROR2            (MD_ERRORBASE + 0x03U)  /* error 2 */
#define MD_ERROR3            (MD_ERRORBASE + 0x04U)  /* error 3 */
#define MD_ERROR4            (MD_ERRORBASE + 0x05U)  /* error 4 */
#define MD_ERROR5            (MD_ERRORBASE + 0x06U)  /* error 5 */
#endif

typedef enum
{
    LOWPWR_UART0 = 0,
    LOWPWR_UART1,
    LOWPWR_UART2,
    LOWPWR_UART3,
    LOWPWR_SMCI0,
}LOWPWRENUM;


void SYS_LOWPWR_Sleep(void);
uint32_t SYS_LOWPWR_Set_Busy(LOWPWRENUM module );
uint32_t SYS_LOWPWR_Clear_Busy(LOWPWRENUM module );
void SYS_LOWPWR_Delay(uint16_t tVal);

typedef struct 
{
    void (*Sleep)(void);
    uint32_t (*Set)(LOWPWRENUM module);
    uint32_t (*Clear)(LOWPWRENUM module);
    void (*Delay)(uint16_t);
}_F_LOWPWR;

EXT_DEV_LOWPWR const _F_LOWPWR gs_DEVLowPwr;


#endif

