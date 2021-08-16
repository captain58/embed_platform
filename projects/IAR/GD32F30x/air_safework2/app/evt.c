/****************************************Copyright (c)**************************************************
**                                  ____�Ƽ����޹�˾
**                                          ������
**
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: GD_evt.c
**��   ��   ��: yzy
**����޸�����: 2019-06-18
**��        ��: �¼��ж����(�����ڸ澯����)
**ע        ��:
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: yzy
** ��  ��: v1.0
** �ա���: 2019-06-18
** �衡��: ԭʼ�汾
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
**��������: void EVT_PreInit(void);
**�� ��: �¼��ṹ���ⲿ��ʼ��
**
**�� ��: 
**�� ��: 
**
**�� ע: 
**--------------------------------------------------------
**�� ��: yzy
**�� ��: 
*************************************************************************/
void EVT_PreInit(void)
{                                       //�����¼���־
    ClearBuffer((uint8*)gss_ALRTStt, sizeof(gss_ALRTStt));

//	    ClearBuffer(guc_StopCnt, sizeof(guc_StopCnt));
}



/************************************************************************
**��������: void EVT_Reset(void)
**�� ��: �¼�������
**
**�� ��: 
**�� ��: 
**
**�� ע: 
**--------------------------------------------------------
**�� ��: yzy
**�� ��: 
*************************************************************************/
void EVT_Reset(void)
{                                       //�����¼���־
    ClearBuffer((uint8*)gss_ALRTStt, sizeof(gss_ALRTStt));
                                        //����
//	    ClearBuffer(guc_StopCnt, sizeof(guc_StopCnt));

}



/************************************************************************
 * @function: EVT_ERC8_Proc
 * @����: ���ܱ���������¼��ж�(����������Լ�����֮��)
 * 
 * @����: 
 * @param: ldata 
 * @param: ndata 
 * @param: tno 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/2/9)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	void EVT_ERC8_Proc(uint8* ldata, uint8* ndata, uint8 tno)
//	{
//	    S_EvtRealData*  levtreal; 
//	    S_EvtRealData*  nevtreal;
//	    levtreal = (S_EvtRealData*)ldata;
//	    nevtreal = (S_EvtRealData*)ndata;
//	                                        //���ܱ�ʱ�η��ʷ�������(yzy ɽ���ǰ���ʱ�α��̼�¼03300501���жϵ�)
//	    if(STR_IsBitSet(nevtreal->eft, EFT_EVT_FTN) && STR_IsBitSet(levtreal->eft, EFT_EVT_FTN))
//	    {
//	        if(CmpBuffer((uint8*)nevtreal->ftn, (uint8*)levtreal->ftn, sizeof(levtreal->ftn)))
//	        {
//	            gss_ALRTStt[tno].ewr1.gsd = 1;
//	        }
//	    }
//	                                        //���ʱ������ж�
//	    if(STR_IsBitSet(nevtreal->eft, EFT_EVT_PROGT) && STR_IsBitSet(levtreal->eft, EFT_EVT_PROGT))
//	    {
//	        if(CmpBuffer((uint8*)nevtreal->progt, (uint8*)levtreal->progt, sizeof(levtreal->demdt)))
//	        {
//	            gss_ALRTStt[tno].ewr1.gsj = 1;
//	        }
//	    }
//	                                        //�����ո����ж�
//	    if(STR_IsBitSet(nevtreal->eft, EFT_EVT_DH) && STR_IsBitSet(levtreal->eft, EFT_EVT_DH))
//	    {
//	        if(CmpBuffer((uint8*)nevtreal->dh, (uint8*)levtreal->dh, sizeof(levtreal->dh)))
//	        {
//	            gss_ALRTStt[tno].ewr1.cbr = 1;
//	        }
//	    }
//	                                        //���ܱ����峣����������
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
//	                                        //���������ʷ�������
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
//	                                        //���������ж�
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
//	 * @����: ���ܱ�ʱ�䳬���¼��ж�(����������Լ�����֮��)
//	 * 
//	 * @����: 
//	 * @param: tstdata ʵʱ����
//	 * @param: tno �������
//	 * @����: 
//	 * @˵��: 
//	 * @����: yzy (2014/2/9)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
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
//	                                        //������ܱ�ʱ�Ӻ͵�ǰʱ������ֵ����Ϊ�쳣
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
//	 * @����: ���ܱ������Ϣ�¼��ж�(����������Լ�����֮��)
//	 * 
//	 * @����: 
//	 * @param: ldata 
//	 * @param: ndata 
//	 * @param: tno 
//	 * @����: 
//	 * @˵��: 
//	 * @����: yzy (2014/2/10)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	void EVT_ERC13_33_Proc(uint8* ldata, uint8* ndata, uint8 tno)
//	{
//	    S_EvtRealData* levtreal;
//	    S_EvtRealData* nevtreal;
//	    levtreal = (S_EvtRealData*)ldata;
//	    nevtreal = (S_EvtRealData*)ndata;
//	
//	                                        //��̴����仯
//	    if(STR_IsBitSet(nevtreal->eft, EFT_EVT_PCNT) && STR_IsBitSet(levtreal->eft, EFT_EVT_PCNT))
//	    {
//	        if(nevtreal->pcnt != levtreal->pcnt)
//	        {
//	            gss_ALRTStt[tno].ewr1.bcb = 1;
//	        }
//	    }
//	                                        //������������仯
//	    if(STR_IsBitSet(nevtreal->eft, EFT_EVT_DEMDCNT) && STR_IsBitSet(levtreal->eft, EFT_EVT_DEMDCNT))
//	    {
//	        if(nevtreal->demdcnt != levtreal->demdcnt)
//	        {
//	            gss_ALRTStt[tno].ewr1.xlb = 1;
//	        }
//	    }
//	                                        //��������仯
//	    if(STR_IsBitSet(nevtreal->eft, EFT_EVT_DXN) && STR_IsBitSet(levtreal->eft, EFT_EVT_DXN))
//	    {
//	        if(CmpBuffer((uint8*)nevtreal->dxn, (uint8*)levtreal->dxn, sizeof(levtreal->dxn)))
//	        {
//	            gss_ALRTStt[tno].ewr1.dxb = 1;
//	        }
//	    }
//	                                        //ʧѹ�����仯
//	    if(STR_IsBitSet(nevtreal->eft, EFT_EVT_SYCNT) && STR_IsBitSet(levtreal->eft, EFT_EVT_SYCNT))
//	    {
//	        if(nevtreal->sycnt != levtreal->sycnt)
//	        {
//	            gss_ALRTStt[tno].ewr1.syb = 1;
//	        }
//	    }
//	                                        //ͣ������仯
//	    if(STR_IsBitSet(nevtreal->eft, EFT_EVT_TDCNT) && STR_IsBitSet(levtreal->eft, EFT_EVT_TDCNT))
//	    {
//	        if(nevtreal->tdcnt != levtreal->tdcnt)
//	        {
//	            gss_ALRTStt[tno].ewr1.tdb = 1;
//	        }
//	    }
//	                                        //״̬�ִ���(���Ƿѹ�¼�,״̬�ֱ���¼�)
//	    if(STR_IsBitSet(nevtreal->eft, EFT_EVT_BSTT) && STR_IsBitSet(levtreal->eft, EFT_EVT_BSTT))
//	    {
//	        if(CmpBuffer((uint8*)nevtreal->bstt, (uint8*)levtreal->bstt, sizeof(levtreal->bstt)))
//	        {
//	            ALRT_ERC33(nevtreal->bstt, levtreal->bstt, tno);
//	        }
//	    }
//	                                        //��ǰ��ѹ��
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
 * @����: ����ʧ��,�ն˹���(485)�¼�����
 * 
 * @����: 
 * @param: port 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/2/10)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void EVT_ERC21_31_Proc(uint8 port)
{
#if ERC21_EN == 1
    uint8 flag_trn = 0;                 //1:�·�������ʧ��
    uint8 IsGet = 0;                    //1:����ĳ���
    uint8 k;
                                        //ͣ��״̬��,������µ�����ʧ�ܱ�־
    if(gs_SysVar.mDGstt & HLV_STT_NENG)
    {
        for(k = 1; k < CORE_TST_NO; k++)
        {
            gss_ALRTStt[k].ew1.trn = 0;
        }
        return;                         //�˳�
    }
                                        //�����Ϸ�����֤
    _IF_TRUE_RETURN_VOID(port >= MCB_PORT_NUM);
	
    for(k = 0; k < CORE_TST_NO; k++)
    {
        if(!((gss_CDATATst[k].info.type == 0x01 || gss_CDATATst[k].info.type == 0x03)
                    && gss_CDATATst[k].pt == port))   //Ҫ��:1������ 3IC��������
        {
            continue;
        }
                                        //�·�������ʧ��
        if(gss_ALRTStt[k].ewr1.trn || gss_ALRTStt[k].ewf1.trn)
        {
            flag_trn = 1;
        }
                                        //�������,�Ͳ����ն�485����
//	        if(!gss_ALRTStt[k].ew1.trn)
//	        {
//	            IsGet = 1;
//	            break;
//	        }
    }
	
    if(flag_trn)                        //��������ʧ��
    {
//	        if(IsGet)                       //��������
        {
            for(k = 0; k < CORE_TST_NO; k++)
            {
                if(!((gss_CDATATst[k].info.type == 0x01 || gss_CDATATst[k].info.type == 0x03)
                            && gss_CDATATst[k].pt == port))   //Ҫ��:1������ 3IC��������
                {
                    continue;
                }
                ALRT_ERC31(k);          //��¼����������ĳ���ʧ���¼�.������.
            }
        }
//	        else                            //��������
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
//	 * @����: ���ܱ�ʱ�䳬���¼��ж�(����������Լ�����֮��)
//	 * 
//	 * @����: 
//	 * @param: tstdata 
//	 * @param: tno 
//	 * @����: 
//	 * @˵��: 
//	 * @����: yzy (2014/2/10)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	void EVT_ERC27_28_29_30_Proc(S_TstRealData* tstdata, uint8 tno)
//	{
//	    uint32 val;
//	    uint32 mx;
//	                                        //ǰ�����ݱ�����Ч
//	    if(STR_IsBitClr(tstdata->eft, EFT_WP) || STR_IsBitClr(gss_TstRealData[tno].eft, EFT_WP))
//	    {
//	        return;
//	    }
//	
//	    //ʾֵ�½��¼��ж�(��ɽ����������,���ÿ����������½����)
//	    if((tstdata->Wp[0] < gss_TstRealData[tno].Wp[0]))
//	    {
//	        if(!gss_ALRTStt[tno].ew1.szj)   //����
//	        {
//	            gss_ALRTStt[tno].ewr1.szj = 1;
//	        }
//	        gss_ALRTStt[tno].ew1.szj = 1;
//	    }
//	    else
//	    {
//	        if(gss_ALRTStt[tno].ew1.szj)    //�ָ�
//	        {
//	            gss_ALRTStt[tno].ewf1.szj = 1;
//	        }
//	        gss_ALRTStt[tno].ew1.szj = 0;
//	    }
//	
//	    //�������γ���ĵ�����ֵ
//	    val = tstdata->Wp[0] - gss_TstRealData[tno].Wp[0];
//	
//	    //���������¼��ж�
//	    if(gs_EVTComVar.wfast > 0)
//	    {
//	        mx = gs_EVTComVar.wfast;
//	        if(val > mx)
//	        {
//	            if(!gss_ALRTStt[tno].ew1.fast)  //����
//	            {
//	                gss_ALRTStt[tno].ewr1.fast = 1;
//	            }
//	            gss_ALRTStt[tno].ew1.fast = 1;
//	        }
//	        else
//	        {
//	            if(gss_ALRTStt[tno].ew1.fast)   //�ָ�
//	            {
//	                gss_ALRTStt[tno].ewf1.fast = 1;
//	            }
//	            gss_ALRTStt[tno].ew1.fast = 0;
//	        }
//	    }
//	
//	    //���������¼��ж�
//	    if(gs_EVTComVar.wfly > 0)
//	    {
//	        mx = gs_EVTComVar.wfly;
//	        if(val > mx)
//	        {
//	            if(!gss_ALRTStt[tno].ew1.fly)   //����
//	            {
//	                gss_ALRTStt[tno].ewr1.fly = 1;
//	            }
//	            gss_ALRTStt[tno].ew1.fly = 1;
//	        }
//	        else
//	        {
//	            if(gss_ALRTStt[tno].ew1.fly)    //�ָ�
//	            {
//	                gss_ALRTStt[tno].ewf1.fly = 1;
//	            }
//	            gss_ALRTStt[tno].ew1.fly = 0;
//	        }
//	    }
//	
//	    //����ͣ���¼��ж�
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
//	            if(!gss_ALRTStt[tno].ew1.bjt)   //����
//	            {
//	                gss_ALRTStt[tno].ewr1.bjt = 1;
//	            }
//	            gss_ALRTStt[tno].ew1.bjt = 1;
//	        }
//	        else
//	        {
//	            if(gss_ALRTStt[tno].ew1.bjt)    //�ָ�
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
//	    //ɽ�������淶Ҫ��ĵ��ܱ�ͣ���ж�. 
//	    unsigned long gul_EngRecord[6] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};   //yzyȫ�ֱ���
//	    unsigned long gul_PowRecord[6] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};   //yzyȫ�ֱ���
//	    //yzy
//	    gul_EngRecord[0] = gul_EngRecord[1];      //[0]������ϴ�,[1]�����һ��,[2]�����һ��
//	    gul_EngRecord[1] = gul_EngRecord[2];
//	    gul_EngRecord[2] = gul_EngRecord[3];
//	    gul_EngRecord[3] = gul_EngRecord[4];
//	    gul_EngRecord[4] = gul_EngRecord[5];
//	    gul_EngRecord[5] = ulEnerNew;
//	    
//	                                                //���¼��� old, newֵ
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
//	    gul_PowRecord[0] = gul_PowRecord[1];      //[0]������ϴ�,[1]�����һ��,[2]�����һ��
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
//	 * @����: ���ܱ�����¼��ж�
//	 * 
//	 * @����: 
//	 * @param: ldata 
//	 * @param: ndata 
//	 * @param: tno 
//	 * @����: 
//	 * @˵��: 
//	 * @����: yzy (2014/2/11)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	void EVT_ERC37_38_Proc(uint8* ldata ,uint8* ndata , uint8 tno)
//	{
//	    S_EvtRealData*  levtreal;
//	    S_EvtRealData* nevtreal;
//	    levtreal = (S_EvtRealData*)ldata;
//	    nevtreal = (S_EvtRealData*)ndata;
//	    uint8 buffer[128];
//	    uint8 err;
//	                                        //������¼�
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
//	                                        //����ť���¼�
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
//	 * @����: ���ܱ�ų��쳣�¼�/��ʱ�¼��ж�
//	 * 
//	 * @����: 
//	 * @param: ldata 
//	 * @param: ndata 
//	 * @param: tno 
//	 * @����: 
//	 * @˵��: 
//	 * @����: yzy (2014/2/11)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	void EVT_ERC40_41_Proc(uint8* ldata, uint8* ndata, uint8 tno)
//	{
//	    S_EvtRealData*  levtreal;
//	    S_EvtRealData*  nevtreal;
//	    levtreal = (S_EvtRealData*)ldata;
//	    nevtreal = (S_EvtRealData*)ndata;
//	    uint8 buffer[128];
//	    uint8 err;
//	                                        //�ų��쳣�¼�
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
//	                                        //��ʱ�¼�
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
//	**��������: void EVT_SecProc(void)
//	**�� ��: ���ն��¼����µ�������0(ÿ����)
//	**
//	**�� ��: 
//	**�� ��: 
//	**
//	**�� ע: 
//	**--------------------------------------------------------
//	**�� ��: yzy
//	**�� ��: 
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
//	 * @����: �ſ���˲ʱ״̬
//	 * @����: 
//	 * 
//	 * @����: 
//	 * @return: uint8  
//	 * @˵��: 
//	 * @����: yzy (2014/2/9)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	uint8 EVT_JudgeMK(void)
//	{
//	    uint32 iost;
//	    uint32 iowd;
//	    MKStruct* es = &gs_MKStruct;
//	    uint8 change;
//	
//	    iowd = gs_GPIO.GPI_GetStt(0);       //��ȡ��ǰ��״̬
//	    iost = iowd;                        //��ȡ�˿�״̬
//	    iost >>= YX_IOBIT;
//	
//	    for(uint8 uc_i = 0; uc_i < YX_NUM; uc_i++)
//	    {
//	        if(!(es->mGateIn & (1 << uc_i)))//����ǳ�����
//	        {
//	            iost ^= (1 << uc_i);
//	        }
//	    }
//	                                        //���ص�ǰ��״̬
//	    iost &= Bit_Mask8[YX_NUM];
//	
//	    if(es->mValid == 0)                 //�״�ִ��
//	    {
//	        es->mValid = 1;
//	        es->mState = iost;
//	        SYS_OK();
//	    }
//	                                        //�жϱ�λ
//	    if(iost != es->mState)              //���˲ʱ״̬�͵�ǰ״̬��һ��
//	    {
//	                                        //�仯������
//	        change = iost ^ es->mState;
//	
//	        for(uint32 m = 0; m < YX_NUM; m++)
//	        {
//	            if(change & (1 << m))
//	            {                           //����������Ȼ��״̬����
//	                es->mState ^= (1 << m);
//	                                        //�жϲ��������¼������½��¼�
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
//	        gs_MKStruct.mb |= change;    //δ�ϱ���־
//	    }
//	    SYS_OK();
//	}








