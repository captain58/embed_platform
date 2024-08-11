/******************************Copyright(c)******************************
**                                
**
** File Name: dev_gpo.h
** Author: 
** Date Last Update: 2019-11-02
** Description: 普通输出口头文件
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: 文件创建
*************************************************************************/
#ifndef _DEV_GPO_H_
#define _DEV_GPO_H_

#ifndef EXT_DEV_GPO
#define EXT_DEV_GPO extern
#endif 




/************************************************************************
 * @Function: SYS_GPO_Init
 * @Description: GPO设备初始化
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
 * @param: gpo_name 输出口编号枚举(GPO_ENUM)
 * @param: oc -true 输出高电平, false 输出低电平
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
