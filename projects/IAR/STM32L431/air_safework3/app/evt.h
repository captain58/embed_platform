/****************************************Copyright (c)**************************************************
**                                  ____科技有限公司
**                                          开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: GD_evt.h
**创   建   人: yzy
**最后修改日期: 2019-06-18
**描        述: 事件判断相关头文件
**注        意:
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2019-06-18
** 描　述: 原始版本
********************************************************************************************************/
#ifndef _GD_EVT_H_
#define _GD_EVT_H_


#ifndef EXT_GD_EVT
#define EXT_GD_EVT extern
#endif





/**********************************************************************
**测量点事件1使能配置
************************************************************************/
#define   EWD1_GSD_EN   1              //时段费率更改
#define   EWD1_GSD_NO   0
#define   EWD1_GSJ_EN   1               //编程时间更改
#define   EWD1_GSJ_NO   (EWD1_GSD_NO + 1* EWD1_GSD_EN)
#define   EWD1_CBR_EN   1               //抄表日更改
#define   EWD1_CBR_NO   (EWD1_GSJ_NO + 1* EWD1_GSJ_EN)
#define   EWD1_MCC_EN   1               //脉冲常数更改
#define   EWD1_MCC_NO   (EWD1_CBR_NO + 1* EWD1_CBR_EN)
#define   EWD1_HGQ_EN   1               //互感器倍率更改
#define   EWD1_HGQ_NO   (EWD1_MCC_NO + 1* EWD1_MCC_EN)
#define   EWD1_CLR_EN   1               //最大需量清零
#define   EWD1_CLR_NO   (EWD1_HGQ_NO + 1* EWD1_HGQ_EN)
#define   EWD2_TIM_EN   1               //时间超差
#define   EWD2_TIM_NO   (EWD1_CLR_NO + 1* EWD1_CLR_EN)
#define   EWD2_DCD_EN   1              //电池电量低
#define   EWD2_DCD_NO   (EWD2_TIM_NO + 1* EWD2_TIM_EN)
#define   EWD2_TRN_EN   1               //通讯故障
#define   EWD2_TRN_NO   (EWD2_DCD_NO + 1* EWD2_DCD_EN)
#define   EWD2_SZJ_EN   1               //视值下降 
#define   EWD2_SZJ_NO   (EWD2_TRN_NO + 1* EWD2_TRN_EN)
#define   EWD2_FAST_EN  1               //电量超差
#define   EWD2_FAST_NO  (EWD2_SZJ_NO + 1* EWD2_SZJ_EN)
#define   EWD2_FLY_EN   1              //电量飞走
#define   EWD2_FLY_NO   (EWD2_FAST_NO + 1* EWD2_FAST_EN)
#define   EWD2_BJT_EN   1               //表计停走
#define   EWD2_BJT_NO   (EWD2_FLY_NO + 1* EWD2_FLY_EN)
#define   EWD2_BCB_EN   1               //编程次数变化
#define   EWD2_BCB_NO   (EWD2_BJT_NO + 1* EWD2_BJT_EN)
#define   EWD2_XLB_EN   1               //需量清零次数变化
#define   EWD2_XLB_NO   (EWD2_BCB_NO + 1* EWD2_BCB_EN)
#define   EWD2_DXB_EN   1              //断相次数变化
#define   EWD2_DXB_NO   (EWD2_XLB_NO + 1* EWD2_XLB_EN)
#define   EWD2_LLC_EN   1               //通讯流量超限
#define   EWD2_LLC_NO   (EWD2_DXB_NO + 1* EWD2_DXB_EN)
#define   EWD2_TDB_EN   1               //停电次数变化
#define   EWD2_TDB_NO   (EWD2_LLC_NO + 1* EWD2_LLC_EN)
#define   EWD2_SYB_EN   1               //失压次数变化
#define   EWD2_SYB_NO   (EWD2_TDB_NO + 1* EWD2_TDB_EN)


/**********************************************************************
**测量点事件状态字1
************************************************************************/
typedef struct
{
#if EWD1_GSD_EN > 0
    uint32 gsd: 1;                      //时段或费率更改 r_only erc8
#endif
#if EWD1_GSJ_EN > 0
    uint32 gsj: 1;                      //编程时间更改   r_only erc8
#endif
#if EWD1_CBR_EN > 0
    uint32 cbr: 1;                      //抄表日更改     r_only erc8
#endif
#if EWD1_MCC_EN > 0
    uint32 mcc: 1;                      //脉冲常数更改   r_only erc8
#endif
#if EWD1_HGQ_EN > 0
    uint32 hgq: 1;                      //互感器倍率更改 r_only erc8
#endif
#if EWD1_CLR_EN > 0
    uint32 clr: 1;                      //最大需量清零   r_only erc8
#endif
#if EWD2_TIM_EN > 0
    uint32 tim : 1;                     //时间超差(测量点) erc12
#endif
#if EWD2_DCD_EN > 0
    uint32 dcd : 1;                     //电池电量低       erc13
#endif
#if EWD2_TRN_EN > 0
    uint32 trn : 1;                     //通讯故障
#endif
#if EWD2_SZJ_EN > 0
    uint32 szj : 1;                     //视值下降         r_only(unrev)
#endif
#if EWD2_FAST_EN > 0
    uint32 fast: 1;                     //电量超差         r_only(unrev)
#endif
#if EWD2_FLY_EN > 0
    uint32 fly : 1;                     //电量飞走         r_only(unrev)
#endif
#if EWD2_BJT_EN > 0
    uint32 bjt : 1;                     //表计停走
#endif
#if EWD2_BCB_EN > 0
    uint32 bcb : 1;                     //编程次数变化     r_only erc13
#endif
#if EWD2_XLB_EN > 0
    uint32 xlb : 1;                     //需量清零次数变化 r_only erc13
#endif
#if EWD2_DXB_EN > 0
    uint32 dxb : 1;                     //断相次数变化     r_only erc13
#endif
#if EWD2_LLC_EN > 0
    uint32 llc : 1;                     //通信流量超限     tml_only
#endif
#if EWD2_TDB_EN > 0
    uint32 tdb : 1;                     //停电次数变化     r_only erc13
#endif
#if EWD2_SYB_EN > 0
    uint32 syb : 1;                     //失压次数变化     r_only erc13
#endif
}EventWord1;




/**********************************************************************
**定义测量点状态字和事件字<@1>
************************************************************************/
typedef struct
{
    EventWord1 ew1, ewr1, ewf1;

}ALRTStt;


EXT_GD_EVT ALRTStt gss_ALRTStt[CORE_TST_NO];








//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//typedef struct
//{
//    uint8  mGateIn;                     //遥信类型.按位0:常开.1常闭
//    uint8  mState;                      //当前的事件状态
//    uint8  mRaise;                      //产生事件状态位
//    uint8  mFall;                       //恢复事件状态位
//    uint8  mValid;                      //先前状态是否有效
//    uint8  mb;                          //状态量变位标志BS8
//}MKStruct;


//EXT_GD_EVT MKStruct gs_MKStruct;





/**********************************************************************
**终端门限参数
**Inp_VarFresh()中赋值
************************************************************************/
typedef struct
{
    uint8 wfast;                        //超差阈值 hex1.1 (0-9.9)
    uint8 wfly;                         //飞走阈值 hex1.1 (0-9.9)
    uint8 stop;                         //停走阈值 hex_min(ERC30限制其最大为255)
    uint16 tchk;                        //校时阈值(时间超差),单位秒(376.1单位为分,1376.1单位为秒)
    uint32 mflux;                       //通信流量门限

}EVTComVar;

EXT_GD_EVT EVTComVar gs_EVTComVar;






/************************************************************************
**函数名称: void EVT_PreInit(void)
**描 述: 事件结构的外部初始化
**
**输 入: 
**输 出: 
**
**备 注: 
**--------------------------------------------------------
**作 者: yzy
**日 期: 
*************************************************************************/
void EVT_PreInit(void);


/************************************************************************
**函数名称: void EVT_Reset(void)
**描 述: 事件的总清
**
**输 入: 
**输 出: 
**
**备 注: 
**--------------------------------------------------------
**作 者: yzy
**日 期: 
*************************************************************************/
void EVT_Reset(void);

/************************************************************************
**函数名称: void EVT_SecProc(void)
**描 述: 每秒钟处理,将终端事件更新到测量点
**
**输 入: 
**输 出: 
**
**备 注: 
**--------------------------------------------------------
**作 者: yzy
**日 期: 
*************************************************************************/
void EVT_SecProc(void);



/************************************************************************
 * @function: EVT_ERC8_Proc
 * @描述: 电能表参数更变事件判断(当读完电量以及参数之后)
 * 
 * @参数: 
 * @param: ldata 
 * @param: ndata 
 * @param: tno 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/2/9)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void EVT_ERC8_Proc(uint8* ldata, uint8* ndata, uint8 tno);

/************************************************************************
 * @function: EVT_ERC12_Proc
 * @描述: 电能表时间超差事件判断(当读完电量以及参数之后)
 * 
 * @参数: 
 * @param: tstdata 实时数据
 * @param: tno 测量点号
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/2/9)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//void EVT_ERC12_Proc(S_TstRealData* tstdata, uint8 tno);

/************************************************************************
 * @function: EVT_ERC13_33_Proc
 * @描述: 电能表故障信息事件判断(当读完电量以及参数之后)
 * 
 * @参数: 
 * @param: ldata 
 * @param: ndata 
 * @param: tno 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/2/10)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void EVT_ERC13_33_Proc(uint8* ldata, uint8* ndata, uint8 tno);


/************************************************************************
 * @function: EVT_ERC21_31_Proc
 * @描述: 抄表失败,终端故障(485)事件处理
 * 
 * @参数: 
 * @param: port 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/2/10)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void EVT_ERC21_31_Proc(uint8 port);

/************************************************************************
 * @function: EVT_ERC27_28_29_30_Proc
 * @描述: 电能表时间超差事件判断(当读完电量以及参数之后)
 * 
 * @参数: 
 * @param: tstdata 
 * @param: tno 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/2/10)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//void EVT_ERC27_28_29_30_Proc(S_TstRealData* tstdata, uint8 tno);


/************************************************************************
 * @function: EVT_ERC37_38_Proc
 * @描述: 电能表开表盖事件判断
 * 
 * @参数: 
 * @param: ldata 
 * @param: ndata 
 * @param: tno 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/2/11)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void EVT_ERC37_38_Proc(uint8* ldata ,uint8* ndata , uint8 tno);

/************************************************************************
 * @function: EVT_ERC40_41_Proc
 * @描述: 电能表磁场异常事件/对时事件判断
 * 
 * @参数: 
 * @param: ldata 
 * @param: ndata 
 * @param: tno 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/2/11)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void EVT_ERC40_41_Proc(uint8* ldata, uint8* ndata, uint8 tno);


#endif










