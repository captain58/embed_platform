/****************************************Copyright (c)**************************************************
**                                  ____科技有限公司
**                                          开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: GD_evt.c
**创   建   人: yzy
**最后修改日期: 2019-06-18
**描        述: 事件判断相关(从属于告警任务)
**注        意:
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2019-06-18
** 描　述: 原始版本
********************************************************************************************************/
#define EXT_GD_EVT
//	#include "es.h"
//	#include "hal.h"
//	#include "bsp.h"
//	#include "app.h"
//	#include "apis.h"
//	#include "extapi.h"
#include "public.h"

#include "netp.h"
//#include "core.h"
#include "evt.h"
#include "alert.h"

/************************************************************************
**函数名称: void EVT_PreInit(void);
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
void EVT_PreInit(void)
{                                       //清零事件标志
    ClearBuffer((uint8*)gss_ALRTStt, sizeof(gss_ALRTStt));

//	    ClearBuffer(guc_StopCnt, sizeof(guc_StopCnt));
}



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
void EVT_Reset(void)
{                                       //清零事件标志
    ClearBuffer((uint8*)gss_ALRTStt, sizeof(gss_ALRTStt));
                                        //清零
//	    ClearBuffer(guc_StopCnt, sizeof(guc_StopCnt));

}



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
//	void EVT_ERC8_Proc(uint8* ldata, uint8* ndata, uint8 tno)
//	{
//	    S_EvtRealData*  levtreal; 
//	    S_EvtRealData*  nevtreal;
//	    levtreal = (S_EvtRealData*)ldata;
//	    nevtreal = (S_EvtRealData*)ndata;
//	                                        //电能表时段费率发生更改(yzy 山东是按照时段表编程记录03300501来判断的)
//	    if(STR_IsBitSet(nevtreal->eft, EFT_EVT_FTN) && STR_IsBitSet(levtreal->eft, EFT_EVT_FTN))
//	    {
//	        if(CmpBuffer((uint8*)nevtreal->ftn, (uint8*)levtreal->ftn, sizeof(levtreal->ftn)))
//	        {
//	            gss_ALRTStt[tno].ewr1.gsd = 1;
//	        }
//	    }
//	                                        //编程时间更改判断
//	    if(STR_IsBitSet(nevtreal->eft, EFT_EVT_PROGT) && STR_IsBitSet(levtreal->eft, EFT_EVT_PROGT))
//	    {
//	        if(CmpBuffer((uint8*)nevtreal->progt, (uint8*)levtreal->progt, sizeof(levtreal->demdt)))
//	        {
//	            gss_ALRTStt[tno].ewr1.gsj = 1;
//	        }
//	    }
//	                                        //抄表日更改判断
//	    if(STR_IsBitSet(nevtreal->eft, EFT_EVT_DH) && STR_IsBitSet(levtreal->eft, EFT_EVT_DH))
//	    {
//	        if(CmpBuffer((uint8*)nevtreal->dh, (uint8*)levtreal->dh, sizeof(levtreal->dh)))
//	        {
//	            gss_ALRTStt[tno].ewr1.cbr = 1;
//	        }
//	    }
//	                                        //电能表脉冲常数发生更改
//	    if(STR_IsBitSet(nevtreal->eft, EFT_EVT_IMP0) && STR_IsBitSet(levtreal->eft, EFT_EVT_IMP0))
//	    {
//	        if(nevtreal->imp0 != levtreal->imp0)
//	        {
//	            gss_ALRTStt[tno].ewr1.mcc = 1;
//	        }
//	    }
//	    if(STR_IsBitSet(nevtreal->eft, EFT_EVT_IMP1) && STR_IsBitSet(levtreal->eft, EFT_EVT_IMP1))
//	    {
//	        if(nevtreal->imp1 != levtreal->imp1)
//	        {
//	            gss_ALRTStt[tno].ewr1.mcc = 1;
//	        }
//	    }
//	                                        //互感器倍率发生更改
//	    if(STR_IsBitSet(nevtreal->eft, EFT_EVT_CTBB) && STR_IsBitSet(levtreal->eft, EFT_EVT_CTBB))
//	    {
//	        if(nevtreal->ctbb != levtreal->ctbb)
//	        {
//	            gss_ALRTStt[tno].ewr1.hgq = 1;
//	        }
//	    }
//	    if(STR_IsBitSet(nevtreal->eft, EFT_EVT_PTBB) && STR_IsBitSet(levtreal->eft, EFT_EVT_PTBB))
//	    {
//	        if(nevtreal->ptbb != levtreal->ptbb)
//	        {
//	            gss_ALRTStt[tno].ewr1.hgq = 1;
//	        }
//	    }
//	                                        //需量清零判断
//	    if(STR_IsBitSet(nevtreal->eft, EFT_EVT_DEMDT) && STR_IsBitSet(levtreal->eft, EFT_EVT_DEMDT))
//	    {
//	        if(CmpBuffer((uint8*)&nevtreal->demdt, (uint8*)&levtreal->demdt, sizeof(levtreal->demdt)))
//	        {
//	            gss_ALRTStt[tno].ewr1.clr = 1;
//	        }
//	    }
//	
//	    ALRT_ERC8(tno);  
//	}
//	
//	
//	
//	/************************************************************************
//	 * @function: EVT_ERC12_Proc
//	 * @描述: 电能表时间超差事件判断(当读完电量以及参数之后)
//	 * 
//	 * @参数: 
//	 * @param: tstdata 实时数据
//	 * @param: tno 测量点号
//	 * @返回: 
//	 * @说明: 
//	 * @作者: yzy (2014/2/9)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
//	 ************************************************************************/
//	void EVT_ERC12_Proc(S_TstRealData* tstdata, uint8 tno)
//	{
//	    uint32 t1;
//	    uint32 t2;
//	
//	    if(STR_IsBitSet(tstdata->eft, EFT_TIME))
//	    {
//	        t1 = CalcSecs((uint8*)&(GetTime()->sec), false);
//	        t2 = CalcSecs(tstdata->time, true);
//	        t1 = AbsVal(t1 - t2);
//	                                        //如果电能表时钟和当前时间差超过限值则认为异常
//	        if(t1 >= gs_EVTComVar.tchk)
//	        {
//	            if(!(gss_ALRTStt[tno].ew1.tim))
//	            {
//	                gss_ALRTStt[tno].ewr1.tim = 1;   
//	            }
//	            gss_ALRTStt[tno].ew1.tim = 1;
//	        }
//	        else
//	        {
//	            if(gss_ALRTStt[tno].ew1.tim == 1)
//	            {
//	                gss_ALRTStt[tno].ewf1.tim = 1;
//	            }
//	            gss_ALRTStt[tno].ew1.tim = 0;
//	        }
//	    }
//	    ALRT_ERC12(tno);
//	}
//	
//	
//	
//	/************************************************************************
//	 * @function: EVT_ERC13_33_Proc
//	 * @描述: 电能表故障信息事件判断(当读完电量以及参数之后)
//	 * 
//	 * @参数: 
//	 * @param: ldata 
//	 * @param: ndata 
//	 * @param: tno 
//	 * @返回: 
//	 * @说明: 
//	 * @作者: yzy (2014/2/10)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
//	 ************************************************************************/
//	void EVT_ERC13_33_Proc(uint8* ldata, uint8* ndata, uint8 tno)
//	{
//	    S_EvtRealData* levtreal;
//	    S_EvtRealData* nevtreal;
//	    levtreal = (S_EvtRealData*)ldata;
//	    nevtreal = (S_EvtRealData*)ndata;
//	
//	                                        //编程次数变化
//	    if(STR_IsBitSet(nevtreal->eft, EFT_EVT_PCNT) && STR_IsBitSet(levtreal->eft, EFT_EVT_PCNT))
//	    {
//	        if(nevtreal->pcnt != levtreal->pcnt)
//	        {
//	            gss_ALRTStt[tno].ewr1.bcb = 1;
//	        }
//	    }
//	                                        //需量清零次数变化
//	    if(STR_IsBitSet(nevtreal->eft, EFT_EVT_DEMDCNT) && STR_IsBitSet(levtreal->eft, EFT_EVT_DEMDCNT))
//	    {
//	        if(nevtreal->demdcnt != levtreal->demdcnt)
//	        {
//	            gss_ALRTStt[tno].ewr1.xlb = 1;
//	        }
//	    }
//	                                        //断相次数变化
//	    if(STR_IsBitSet(nevtreal->eft, EFT_EVT_DXN) && STR_IsBitSet(levtreal->eft, EFT_EVT_DXN))
//	    {
//	        if(CmpBuffer((uint8*)nevtreal->dxn, (uint8*)levtreal->dxn, sizeof(levtreal->dxn)))
//	        {
//	            gss_ALRTStt[tno].ewr1.dxb = 1;
//	        }
//	    }
//	                                        //失压次数变化
//	    if(STR_IsBitSet(nevtreal->eft, EFT_EVT_SYCNT) && STR_IsBitSet(levtreal->eft, EFT_EVT_SYCNT))
//	    {
//	        if(nevtreal->sycnt != levtreal->sycnt)
//	        {
//	            gss_ALRTStt[tno].ewr1.syb = 1;
//	        }
//	    }
//	                                        //停电次数变化
//	    if(STR_IsBitSet(nevtreal->eft, EFT_EVT_TDCNT) && STR_IsBitSet(levtreal->eft, EFT_EVT_TDCNT))
//	    {
//	        if(nevtreal->tdcnt != levtreal->tdcnt)
//	        {
//	            gss_ALRTStt[tno].ewr1.tdb = 1;
//	        }
//	    }
//	                                        //状态字处理(电池欠压事件,状态字变更事件)
//	    if(STR_IsBitSet(nevtreal->eft, EFT_EVT_BSTT) && STR_IsBitSet(levtreal->eft, EFT_EVT_BSTT))
//	    {
//	        if(CmpBuffer((uint8*)nevtreal->bstt, (uint8*)levtreal->bstt, sizeof(levtreal->bstt)))
//	        {
//	            ALRT_ERC33(nevtreal->bstt, levtreal->bstt, tno);
//	        }
//	    }
//	                                        //当前电压低
//	    if(STR_IsBitSet(nevtreal->eft, EFT_EVT_BSTT))
//	    {
//	        if(nevtreal->bstt[0] & 0x04)
//	        {
//	            if(!gss_ALRTStt[tno].ew1.dcd)
//	            {
//	                gss_ALRTStt[tno].ewr1.dcd = 1;
//	            }
//	            gss_ALRTStt[tno].ew1.dcd = 1;
//	        }
//	        else
//	        {
//	            gss_ALRTStt[tno].ew1.dcd = 0;
//	        }      
//	    }
//	    ALRT_ERC13(tno);  
//	}
//	
//	
//	
//	
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
void EVT_ERC21_31_Proc(uint8 port)
{
#if ERC21_EN == 1
    uint8 flag_trn = 0;                 //1:新发生抄表失败
    uint8 IsGet = 0;                    //1:存在某块表
    uint8 k;
                                        //停电状态下,补充更新单块表的失败标志
    if(gs_SysVar.mDGstt & HLV_STT_NENG)
    {
        for(k = 1; k < CORE_TST_NO; k++)
        {
            gss_ALRTStt[k].ew1.trn = 0;
        }
        return;                         //退出
    }
                                        //参数合法性验证
    _IF_TRUE_RETURN_VOID(port >= MCB_PORT_NUM);
	
    for(k = 0; k < CORE_TST_NO; k++)
    {
        if(!((gss_CDATATst[k].info.type == 0x01 || gss_CDATATst[k].info.type == 0x03)
                    && gss_CDATATst[k].pt == port))   //要求:1流量计 3IC卡控制器
        {
            continue;
        }
                                        //新发生抄表失败
        if(gss_ALRTStt[k].ewr1.trn || gss_ALRTStt[k].ewf1.trn)
        {
            flag_trn = 1;
        }
                                        //任意表抄到,就不算终端485故障
//	        if(!gss_ALRTStt[k].ew1.trn)
//	        {
//	            IsGet = 1;
//	            break;
//	        }
    }
	
    if(flag_trn)                        //发生抄表失败
    {
//	        if(IsGet)                       //有正常表
        {
            for(k = 0; k < CORE_TST_NO; k++)
            {
                if(!((gss_CDATATst[k].info.type == 0x01 || gss_CDATATst[k].info.type == 0x03)
                            && gss_CDATATst[k].pt == port))   //要求:1流量计 3IC卡控制器
                {
                    continue;
                }
                ALRT_ERC31(k);          //记录单个测量点的抄表失败事件.并清标记.
            }
        }
//	        else                            //无正常表
//	        {
//	            ALRT_ERC21(port);
//	        }
    }
#endif
	
}
//	
//	
//	/************************************************************************
//	 * @function: EVT_ERC27_28_29_30_Proc
//	 * @描述: 电能表时间超差事件判断(当读完电量以及参数之后)
//	 * 
//	 * @参数: 
//	 * @param: tstdata 
//	 * @param: tno 
//	 * @返回: 
//	 * @说明: 
//	 * @作者: yzy (2014/2/10)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
//	 ************************************************************************/
//	void EVT_ERC27_28_29_30_Proc(S_TstRealData* tstdata, uint8 tno)
//	{
//	    uint32 val;
//	    uint32 mx;
//	                                        //前后数据必须有效
//	    if(STR_IsBitClr(tstdata->eft, EFT_WP) || STR_IsBitClr(gss_TstRealData[tno].eft, EFT_WP))
//	    {
//	        return;
//	    }
//	
//	    //示值下降事件判断(按山东技术条件,不用考虑满量程下降情况)
//	    if((tstdata->Wp[0] < gss_TstRealData[tno].Wp[0]))
//	    {
//	        if(!gss_ALRTStt[tno].ew1.szj)   //产生
//	        {
//	            gss_ALRTStt[tno].ewr1.szj = 1;
//	        }
//	        gss_ALRTStt[tno].ew1.szj = 1;
//	    }
//	    else
//	    {
//	        if(gss_ALRTStt[tno].ew1.szj)    //恢复
//	        {
//	            gss_ALRTStt[tno].ewf1.szj = 1;
//	        }
//	        gss_ALRTStt[tno].ew1.szj = 0;
//	    }
//	
//	    //计算两次抄表的电量差值
//	    val = tstdata->Wp[0] - gss_TstRealData[tno].Wp[0];
//	
//	    //电量超差事件判断
//	    if(gs_EVTComVar.wfast > 0)
//	    {
//	        mx = gs_EVTComVar.wfast;
//	        if(val > mx)
//	        {
//	            if(!gss_ALRTStt[tno].ew1.fast)  //产生
//	            {
//	                gss_ALRTStt[tno].ewr1.fast = 1;
//	            }
//	            gss_ALRTStt[tno].ew1.fast = 1;
//	        }
//	        else
//	        {
//	            if(gss_ALRTStt[tno].ew1.fast)   //恢复
//	            {
//	                gss_ALRTStt[tno].ewf1.fast = 1;
//	            }
//	            gss_ALRTStt[tno].ew1.fast = 0;
//	        }
//	    }
//	
//	    //电量飞走事件判断
//	    if(gs_EVTComVar.wfly > 0)
//	    {
//	        mx = gs_EVTComVar.wfly;
//	        if(val > mx)
//	        {
//	            if(!gss_ALRTStt[tno].ew1.fly)   //产生
//	            {
//	                gss_ALRTStt[tno].ewr1.fly = 1;
//	            }
//	            gss_ALRTStt[tno].ew1.fly = 1;
//	        }
//	        else
//	        {
//	            if(gss_ALRTStt[tno].ew1.fly)    //恢复
//	            {
//	                gss_ALRTStt[tno].ewf1.fly = 1;
//	            }
//	            gss_ALRTStt[tno].ew1.fly = 0;
//	        }
//	    }
//	
//	    //电量停走事件判断
//	    if(STR_IsBitSet(tstdata->eft, EFT_P))
//	    {
//	        if((tstdata->Wp[0] == gss_TstRealData[tno].Wp[0]) && (tstdata->P > 100))//0.0100kw
//	        {
//	            guc_StopCnt[tno] += gs_InpExtracb[gss_CDATATst[tno].pt].netitv;
//	        }
//	        else
//	        {
//	            guc_StopCnt[tno] = 0;
//	        }
//	
//	        if(guc_StopCnt[tno] >= gs_EVTComVar.stop)
//	        {
//	            if(!gss_ALRTStt[tno].ew1.bjt)   //产生
//	            {
//	                gss_ALRTStt[tno].ewr1.bjt = 1;
//	            }
//	            gss_ALRTStt[tno].ew1.bjt = 1;
//	        }
//	        else
//	        {
//	            if(gss_ALRTStt[tno].ew1.bjt)    //恢复
//	            {
//	                gss_ALRTStt[tno].ewf1.bjt = 1;
//	            }
//	            gss_ALRTStt[tno].ew1.bjt = 0;
//	        }
//	    }
//	    
//	    ALRT_ERC27_28_29_30(tstdata->Wp[0], tno); 
//	
//	    /* 
//	    //山东技术规范要求的电能表停走判断. 
//	    unsigned long gul_EngRecord[6] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};   //yzy全局变量
//	    unsigned long gul_PowRecord[6] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};   //yzy全局变量
//	    //yzy
//	    gul_EngRecord[0] = gul_EngRecord[1];      //[0]存放上上次,[1]存放上一次,[2]存放这一次
//	    gul_EngRecord[1] = gul_EngRecord[2];
//	    gul_EngRecord[2] = gul_EngRecord[3];
//	    gul_EngRecord[3] = gul_EngRecord[4];
//	    gul_EngRecord[4] = gul_EngRecord[5];
//	    gul_EngRecord[5] = ulEnerNew;
//	    
//	                                                //重新计算 old, new值
//	    ulEnerOld = 0;
//	    num = 0;
//	    for(k = 0; k < 5; k++)
//	    {
//	        if(gul_EngRecord[0 + k] != 0xFFFFFFFF)
//	        {
//	            ulEnerOld  += gul_EngRecord[0 + k];
//	            num++;
//	        };
//	    }
//	    if(num)
//	    {
//	        ulEnerOld = ulEnerOld / num;
//	    }
//	    
//	    ulEnerNew = 0;
//	    num = 0;
//	    for(k = 0; k < 5; k++)
//	    {
//	        if(gul_EngRecord[1 + k] != 0xFFFFFFFF)
//	        {
//	            ulEnerNew  += gul_EngRecord[1 + k];
//	            num++;
//	        };
//	        
//	    }
//	    if(num)
//	    {
//	        ulEnerNew = ulEnerNew / num;
//	    }
//	    
//	    gul_PowRecord[0] = gul_PowRecord[1];      //[0]存放上上次,[1]存放上一次,[2]存放这一次
//	    gul_PowRecord[1] = gul_PowRecord[2];
//	    gul_PowRecord[2] = gul_PowRecord[3];
//	    gul_PowRecord[3] = gul_PowRecord[4];
//	    gul_PowRecord[4] = gul_PowRecord[5];
//	    gul_PowRecord[5] = ulInstantPow;
//	    
//	    ulInstantPow = 0;
//	    num = 0;
//	    for(k = 0; k < 5; k++)
//	    {
//	        if(gul_PowRecord[1 + k] != 0xFFFFFFFF)
//	        {
//	            ulInstantPow  += gul_PowRecord[1 + k];
//	            num++;
//	        };
//	    }
//	    if(num)
//	    {
//	        ulInstantPow = ulInstantPow / num;
//	    } 
//	    */ 
//	    
//	}
//	
//	
//	
//	/************************************************************************
//	 * @function: EVT_ERC37_38_Proc
//	 * @描述: 电能表开表盖事件判断
//	 * 
//	 * @参数: 
//	 * @param: ldata 
//	 * @param: ndata 
//	 * @param: tno 
//	 * @返回: 
//	 * @说明: 
//	 * @作者: yzy (2014/2/11)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
//	 ************************************************************************/
//	void EVT_ERC37_38_Proc(uint8* ldata ,uint8* ndata , uint8 tno)
//	{
//	    S_EvtRealData*  levtreal;
//	    S_EvtRealData* nevtreal;
//	    levtreal = (S_EvtRealData*)ldata;
//	    nevtreal = (S_EvtRealData*)ndata;
//	    uint8 buffer[128];
//	    uint8 err;
//	                                        //开表盖事件
//	    if(STR_IsBitSet(nevtreal->eft, EFT_EVT_WGCNT) && STR_IsBitSet(levtreal->eft, EFT_EVT_WGCNT))
//	    {
//	        if(nevtreal->wgcnt != levtreal->wgcnt)
//	        {
//	            err = Netp_Read(buffer, 0x03300D01, sizeof(buffer), tno);
//	            if(err == SYS_ERR_OK)
//	            {
//	                ALRT_ERC37(buffer, nevtreal->wgcnt, tno);
//	            }
//	        }
//	    }
//	                                        //开端钮盖事件
//	    if(STR_IsBitSet(nevtreal->eft, EFT_EVT_DNCNT) && STR_IsBitSet(levtreal->eft , EFT_EVT_DNCNT))
//	    {
//	        if(nevtreal->dncnt != levtreal->dncnt)
//	        {
//	            err = Netp_Read(buffer, 0x03300E01, sizeof(buffer), tno);
//	            if(err == SYS_ERR_OK)
//	            {
//	                ALRT_ERC38(buffer, nevtreal->dncnt, tno);
//	            }
//	        }
//	    }
//	}
//	
//	
//	
//	/************************************************************************
//	 * @function: EVT_ERC40_41_Proc
//	 * @描述: 电能表磁场异常事件/对时事件判断
//	 * 
//	 * @参数: 
//	 * @param: ldata 
//	 * @param: ndata 
//	 * @param: tno 
//	 * @返回: 
//	 * @说明: 
//	 * @作者: yzy (2014/2/11)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
//	 ************************************************************************/
//	void EVT_ERC40_41_Proc(uint8* ldata, uint8* ndata, uint8 tno)
//	{
//	    S_EvtRealData*  levtreal;
//	    S_EvtRealData*  nevtreal;
//	    levtreal = (S_EvtRealData*)ldata;
//	    nevtreal = (S_EvtRealData*)ndata;
//	    uint8 buffer[128];
//	    uint8 err;
//	                                        //磁场异常事件
//	    if(STR_IsBitSet(nevtreal->eft, EFT_EVT_CCOUNT) && STR_IsBitSet(levtreal->eft, EFT_EVT_CCOUNT))
//	    {
//	        if(nevtreal->ccount != levtreal->ccount)
//	        {
//	            err = Netp_Read(buffer, 0x03350001, sizeof(buffer), tno);
//	            if(err == SYS_ERR_OK)
//	            {
//	                ALRT_ERC40(tno , 0x02); 
//	            }
//	        }
//	    }
//	                                        //对时事件
//	    if(STR_IsBitSet(nevtreal->eft, EFT_EVT_JSCNT) && STR_IsBitSet(levtreal->eft, EFT_EVT_JSCNT))
//	    {
//	        if(nevtreal->jscnt != levtreal->jscnt)
//	        {
//	            if(STR_IsBitSet(nevtreal->eft, EFT_EVT_JSTIME))
//	            {
//	                ALRT_ERC41(nevtreal->jstime, nevtreal->jstime + 6, tno);
//	            }
//	        }
//	    }
//	}
//	
//	
//	
//	
//	
//	
//	/************************************************************************
//	**函数名称: void EVT_SecProc(void)
//	**描 述: 将终端事件更新到测量点0(每秒钟)
//	**
//	**输 入: 
//	**输 出: 
//	**
//	**备 注: 
//	**--------------------------------------------------------
//	**作 者: yzy
//	**日 期: 
//	*************************************************************************/
//	void EVT_SecProc(void)
//	{
//	    uint8 EVT_JudgeMK(void);
//	    EVT_JudgeMK();
//	}
//	
//	
//	
//	
//	
//	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//	
//	
//	
//	
//	/************************************************************************
//	 * @function: EVT_JudgeMK
//	 * @描述: 门开的瞬时状态
//	 * @参数: 
//	 * 
//	 * @返回: 
//	 * @return: uint8  
//	 * @说明: 
//	 * @作者: yzy (2014/2/9)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
//	 ************************************************************************/
//	uint8 EVT_JudgeMK(void)
//	{
//	    uint32 iost;
//	    uint32 iowd;
//	    MKStruct* es = &gs_MKStruct;
//	    uint8 change;
//	
//	    iowd = gs_GPIO.GPI_GetStt(0);       //获取当前的状态
//	    iost = iowd;                        //获取端口状态
//	    iost >>= YX_IOBIT;
//	
//	    for(uint8 uc_i = 0; uc_i < YX_NUM; uc_i++)
//	    {
//	        if(!(es->mGateIn & (1 << uc_i)))//如果是常开点
//	        {
//	            iost ^= (1 << uc_i);
//	        }
//	    }
//	                                        //返回当前的状态
//	    iost &= Bit_Mask8[YX_NUM];
//	
//	    if(es->mValid == 0)                 //首次执行
//	    {
//	        es->mValid = 1;
//	        es->mState = iost;
//	        SYS_OK();
//	    }
//	                                        //判断变位
//	    if(iost != es->mState)              //如果瞬时状态和当前状态不一致
//	    {
//	                                        //变化的内容
//	        change = iost ^ es->mState;
//	
//	        for(uint32 m = 0; m < YX_NUM; m++)
//	        {
//	            if(change & (1 << m))
//	            {                           //计数器清零然后状态更改
//	                es->mState ^= (1 << m);
//	                                        //判断产生上升事件还是下降事件
//	                if(es->mState & (1 << m))
//	                {
//	                    es->mRaise |= (1 << m);
//	                }
//	                else
//	                {
//	                    es->mFall |= (1 << m);
//	                }
//	            }
//	        }
//	
//	        gs_MKStruct.mb |= change;    //未上报标志
//	    }
//	    SYS_OK();
//	}








