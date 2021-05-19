/****************************************Copyright (c)**************************************************
**                                  ____�Ƽ����޹�˾
**                                          ������
**
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: inp.c
**��   ��   ��: yzy
**����޸�����: 2019-06-18
**��        ��: �ڲ�����ģ��������
**ע        ��: ����inp���Ƕ����Ľ���,��λ��stat�����൱
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: yzy
** ��  ��: v1.0
** �ա���: 2019-06-18
** �衡��: ԭʼ�汾
********************************************************************************************************/
#define EXT_INP

//	#include "es.h"
//	#include "hal.h"
//	#include "bsp.h"
//	#include "app.h"
//	#include "apis.h"
//	#include "extapi.h"

#include "public.h"
#include "hlv.h"
#include "saextapi.h"
#include "evt.h"
#include "inp.h"






//***********************************************************************
#define REAL_DMD_BLOCK         64
#define REAL_VOL_BLOCK         64

//	
//	/************************************************************************
//	 * @function: SA_REALDATA_Write
//	 * @����: ��һ��������Ŀ��д������(FLASH���ڴ���)
//	 * 
//	 * @����: 
//	 * @param: data ��д�������
//	 * @param: tno �������
//	 * @param: type �������� 0:ʵʱ�¼�����,1:ʵʱ��������,3:��ѹ����
//	 * 
//	 * @����: 
//	 * @return: uint8  
//	 * @˵��: 
//	 * @����: yzy (2014/2/20)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	uint8 SA_REALDATA_Write(uint8* data, uint16 tno, uint8 type)
//	{
//	    TDevFlsOP op;                       //����һ��Flash�����ṹ
//	    uint16 itmno;                       //�洢��Ŀ��
//	    uint8 result;
//	
//	    if(type == 0)
//	    {
//	       itmno = tno;
//	    }
//	    else if(type == 1)
//	    {
//	       itmno = REAL_DMD_BLOCK + tno;
//	    }
//	    else
//	    {
//	       itmno = REAL_VOL_BLOCK + REAL_DMD_BLOCK + tno;
//	    }
//	
//	    op.addr = itmno;                    //����ڵڼ���4K����.
//	//    op.len = (uint32)&gs_flsRealDataConst; //����(������ʵʱ��������洢��Ϣ�ṹ��)
//	    op.obj = data;                      //����
//	    op.type = DEV_FLS_TYPE_VFRAM;       //����
//	    op.rw = DEV_FLS_RW_W;               //��д��ʽ
//	
//	    ES_FRAM_Open();
//	    result = DevFls_ProcMSG(&op);
//	    ES_FRAM_Close();
//	    
//	    return result;
//	}



/************************************************************************
 * @function: SA_REALDATA_Read
 * @����: ��һ�������ڴ���Ŀ�ж�ȡ����(FLASH���ڴ���)
 * 
 * @����: 
 * @param: data ������ݵĻ���
 * @param: tno �������
 * @param: type �������� 0:ʵʱ�¼�����,1:ʵʱ��������,3:��ѹ����
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2014/2/8)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	uint8 SA_REALDATA_Read(uint8* data, uint16 tno, uint8 type)
//	{
//	    TDevFlsOP op;                       //����һ��Flash�����ṹ
//	    uint16 num;                         //���ݿ��
//	    uint8 result;
//	
//	    if(type == 0)
//	    {
//	       num = tno;
//	    }
//	    else if(type == 1)
//	    {
//	       num = REAL_DMD_BLOCK + tno;
//	    }
//	    else
//	    {
//	       num = REAL_VOL_BLOCK + REAL_DMD_BLOCK + tno;
//	    }
//	
//	    op.addr = num;                      //��ַ
//	//    op.len = (uint32)&gs_flsRealDataConst; //����(������ʵʱ��������洢��Ϣ�ṹ��)
//	    op.obj = data;                      //����
//	    op.type = DEV_FLS_TYPE_VFRAM;       //����
//	    op.rw = DEV_FLS_RW_R;               //��д��ʽ
//	    
//	    ES_FRAM_Open();
//	    result = DevFls_ProcMSG(&op);
//	    ES_FRAM_Close();
//	    
//	    return result;
//	    
//	}









/************************************************************************
 * @function: MapEft
 * @����: ����һ����Ч������ȥ������һ����Ч������
 * 
 * @����: 
 * @param: s Դ����
 * @param: d Ŀ������
 * @param: s_map ��Ӧ�ṹ��
 * @param: s_len ��Ӧ�ṹ������
 * @����: 
 * @˵��: 
 * @����: yzy (2011-9-21)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void MapEft(uint8* s, uint8* d, S_EFT_MAP* s_map, uint8 s_len)
{
    for(uint8 uc_i = 0; uc_i < s_len; uc_i++)
    {
        if(STR_IsBitSet(s, s_map[uc_i].seft))
        {
            STR_SetBit(d, s_map[uc_i].deft);
        }
        else
        {
            STR_ClrBit(d, s_map[uc_i].deft);
        }
    }
}










/************************************************************************
 * �����ռ�¼ʱ��ָ��
 ************************************************************************/
typedef struct
{
    uint8 bp;                               //��¼��ָ��,��Χ0<X<SIZE_RECDAY_DAYS
    uint8 Ymd[SIZE_RECDAY_DAYS][3];         //������¼���ʱ��
}S_RECDAY_TL;




/************************************************************************
 * �����ն���Ϣ����
 ************************************************************************/


const S_DATASEGINFO gss_recSegInfo[] = 
{
    {0, LEN_RECDAY_SEG0},
    {2, LEN_RECDAY_SEG2},
};


/************************************************************************
 * @function: _RecDataAddr
 * @����: ������������ʼ��ַ
 * 
 * @����: 
 * @param: bpidx ��ŵ��ڼ�����¼
 * @param: tno �������
 * @param: segno �κ�
 * @param: outinfo �νṹ��Ϣ��,���.(Ϊ__NULLʱ�����)
 * 
 * @����: 
 * @return: uint32  ���ݿ��е���ʼ��ַ,0xffffffffΪ�Ƿ�.
 * @˵��: 
 * @����: yzy (2014/2/14)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	uint32 _RecDataAddr(uint8 bpidx, uint8 tno, uint8 segno, const S_DATASEGINFO** outinfo)
//	{
//	    const S_DATASEGINFO* _s;
//	    uint32 addr;                            //����ֵ
//	    uint32 totallen;                        //��¼�ܳ���
//	    uint32 segoffset;                       //��λ�ڵ�ǰ��¼�е�ƫ��
//	    uint8 isGot;                            //�Ƿ��ҵ�ָ���Ķ�
//	
//	    totallen = 0;
//	    segoffset = 0;
//	    isGot = 0;
//	    for(uint8 uc_i = 0; uc_i < sizeof(gss_recSegInfo)/sizeof(S_DATASEGINFO); uc_i++)
//	    {
//	        _s = &gss_recSegInfo[uc_i];
//	        totallen += _s->seglen;
//	        if(!isGot)
//	        {
//	            if(_s->segno == segno)
//	            {
//	                isGot = 1;
//	                if(outinfo != __NULL)
//	                {
//	                    *outinfo = _s;
//	                }
//	            }
//	            else
//	            {
//	                segoffset += _s->seglen;
//	            }
//	        }
//	    }
//	
//	    if(!isGot)
//	    {
//	        return 0xFFFFFFFF;
//	    }
//	                                            //ADDRESS(DIVIDED BY DAY)
//	    addr = bpidx * (totallen * CORE_TST_NO);//��¼��(��Ӧʱ��)��ʼ��ַ
//	    addr += tno * totallen;                 //������(��Ӧ��¼����)��ʼ��ַ
//	    addr += segoffset;                      //��(��Ӧ������)��ʼ��ַ
//	    return addr;
//	
//	};



/************************************************************************ 
 *�����ն���.����Чλ��Ӧ��ϵ
//	************************************************************************/
//	const S_EFT_MAP gss_EftMapRealToRecSeg0[] = 
//	{
//	    {EFT_WP, EFT_RECDAY_WP},
//	};
//	
//	
//	const S_EFT_MAP gss_EftMapEvtToRecSeg2[] = 
//	{
//	    {EFT_DMD_DPP,   EFT_RECDAY_DPP},
//	    {EFT_DMD_DPPT,  EFT_RECDAY_DPPT},
//	    {EFT_DMD_DNP,   EFT_RECDAY_DNP},
//	    {EFT_DMD_DNPT,  EFT_RECDAY_DNPT},
//	};


/************************************************************************
 * @function: _RecDataContextReady
 * @����: ���������ݶε�׼��
 * 
 * @����: 
 * @param: tno �������
 * @param: segno �κ�
 * @param: opbuffer ��ŵ��ĵ�ַ.(�����ܷŵ������������)
 * 
 * @����: 
 * @return: uint16 �����ݳ���
 * @˵��: ������ŵ��������Ͳ���,ʵ������������Ҳ������
 * @����: yzy (2014/2/15)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	uint16 _RecDataContextReady(uint8 tno, uint8 segno, uint8* opbuffer)
//	{
//	    if(segno == 0)
//	    {
//	        S_RECDATA_SEG0* recdata = (S_RECDATA_SEG0*)opbuffer;
//	        recdata->seg.stime = gs_SysTime;
//	        recdata->seg.rtime = gss_TstRealData[tno].rtime;
//	        MoveBuffer((uint8*)gss_TstRealData[tno].Wp, (uint8*)recdata->seg.Wp, sizeof(recdata->seg.Wp));
//	        //
//	        memset(recdata->seg.eft, 0, sizeof(recdata->seg.eft));
//	        MapEft(gss_TstRealData[tno].eft, recdata->seg.eft, (S_EFT_MAP*)gss_EftMapRealToRecSeg0, sizeof(gss_EftMapRealToRecSeg0)/ sizeof(S_EFT_MAP));
//	        return sizeof(S_RECDATA_SEG0);
//	    }
//	    else if(segno == 2)
//	    {
//	        //���´���ռ�ý϶��ջ
//	        uint8 tempbuf[SA_FLASH_REALDATA_ITMBABY_BYTE];
//	        S_TstDmdData* dmdata =(S_TstDmdData*)tempbuf; 
//	        if(SA_REALDATA_Read(tempbuf, tno, 1) != SA_ERR_OK)
//	        {
//	             return 0;
//	        }
//	        //���ϴ���ռ�ý϶��ջ
//	        
//	        uint8 time[6];
//	        S_RECDATA_SEG2* recdata = (S_RECDATA_SEG2*)opbuffer;
//	        recdata->seg.stime = gs_SysTime;
//	        MoveBuffer((uint8*)dmdata->ymdhm, time, 5);
//	        BuildTimeLabel(TTYPE_YMDHMS, time, &(recdata->seg.rtime));
//	        MoveBuffer((uint8*)dmdata->dpp,  (uint8*)recdata->seg.dpp,  sizeof(recdata->seg.dpp));
//	        MoveBuffer((uint8*)dmdata->dppt, (uint8*)recdata->seg.dppt, sizeof(recdata->seg.dppt));
//	        MoveBuffer((uint8*)dmdata->dnp,  (uint8*)recdata->seg.dnp,  sizeof(recdata->seg.dnp));
//	        MoveBuffer((uint8*)dmdata->dnpt, (uint8*)recdata->seg.dnpt, sizeof(recdata->seg.dnpt));
//	        //
//	        memset(recdata->seg.eft, 0, sizeof(recdata->seg.eft));
//	        MapEft(dmdata->eft, recdata->seg.eft, (S_EFT_MAP*)gss_EftMapEvtToRecSeg2, sizeof(gss_EftMapEvtToRecSeg2)/ sizeof(S_EFT_MAP));
//	        return sizeof(S_RECDATA_SEG2);
//	    }
//	
//	    return 0;
//	
//	};





/************************************************************************
 * @function: gdw_SaveRecData
 * @����: ����ĳ�˿��µĲ����㳭��������
 * 
 * @����: 
 * @param: port �˿�����
 * @param: segno �κ�
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2014/2/15)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	uint8 gdw_SaveRecData(uint8 port, uint8 segno)
//	{
//	    S_RECDAY_TL tlabel;
//	    uint32 addr;
//	    uint16 dlen;
//	    uint8 opbuffer[MAX_LEN_RECDAY_SEG];
//	    const S_DATASEGINFO* segstr;
//	                                            //���������ݴ洢ָ��ṹ��
//	    //ES_FRAM_Open();
//	    //Fram_SafeRead((uint8*)&tlabel, FM_RECPT, sizeof(tlabel));  //ZJC
//	    //ES_FRAM_Close();
//	                                            //ADJUST LOCATION POINT
//	    if(tlabel.bp >= SIZE_RECDAY_DAYS)
//	    {
//	        tlabel.bp = 0;
//	    }
//	                                            //IF ALREADY PAIRED,QUIT ADJUSTING
//	    if(CmpBuffer(gs_SysTime.YMDhms, tlabel.Ymd[tlabel.bp], 3))
//	    {
//	        tlabel.bp++;
//	        if(tlabel.bp >= SIZE_RECDAY_DAYS)
//	        {
//	            tlabel.bp = 0;
//	        }
//	        MoveBuffer(gs_SysTime.YMDhms, tlabel.Ymd[tlabel.bp], 3);
//	    }
//	                                            //
//	    for(uint8 uc_i = 1; uc_i < CORE_TST_NO; uc_i++)
//	    {
//	        if((gss_CDATATst[uc_i].ef != 0x01) || (gss_CDATATst[uc_i].pt != port))
//	        {
//	            continue;
//	        }
//	        gs_OS.TK_Sleep(5);
//	       
//	        addr = _RecDataAddr(tlabel.bp, uc_i, segno, &segstr);
//	        if(addr == 0xffffffff)
//	        {
//	            return SYS_ERR_FT;
//	        }
//	
//	        dlen = _RecDataContextReady(uc_i, segno, opbuffer);
//	        if(dlen == segstr->seglen)
//	        {
//	            WriteFlashWithCRC(DB_RECDAY, opbuffer, dlen, addr);
//	        }
//	    }
//	    //ES_FRAM_Open();
//	    //Fram_SafeWrite((uint8*)&tlabel, FM_RECPT, sizeof(tlabel));  //ZJC
//	    //ES_FRAM_Close();
//	    ES_OK();
//	
//	}



/************************************************************************
 * @function: gdw_ReadRecData
 * @����: ��ȡĳ��������ĳ���������
 * 
 * @����: 
 * @param: time ������
 * @param: tno �������
 * @param: segno �˿ں�
 * @param: outbuffer �������
 * 
 * @����: 
 * @return: uint8 �ɹ�/ʧ��
 * @˵��: 
 * @����: yzy (2014/2/15)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	uint8 gdw_ReadRecData(uint8* time, uint8 tno, uint8 segno, uint8* outbuffer)
//	{
//	    S_RECDAY_TL tlabel;
//	    uint32  addr;
//	    uint8*  timept;                         //����ʱ��
//	    uint8   timelen;                        //�ж϶���ʱ��ĳ���
//	    uint8   uc_i;
//	    const S_DATASEGINFO* segstr;
//	
//	    ES_VAR_CHECK(tno > CORE_TST_NO);
//	    timept = ((S_DATA_SEG_TEMPLET*)outbuffer)->seg.stime.YMDhms;
//	    timelen = 3;
//	                                            //��ȡ�洢ָ���б�
//	    //ES_FRAM_Open();
//	    //Fram_SafeRead((uint8*)&tlabel, FM_RECPT, sizeof(tlabel));  //ZJC
//	    //ES_FRAM_Close();
//	                                            //ADJUST LOCATION POINT
//	    if(tlabel.bp >= SIZE_RECDAY_DAYS)
//	    {
//	        tlabel.bp = 0;
//	    }
//	                                            //Ѱ�Ҷ�Ӧʱ�������
//	    uint16 index = tlabel.bp;
//	    for(uc_i = 0; uc_i < SIZE_RECDAY_DAYS; uc_i++)
//	    {
//	        if(0 == CmpBuffer(tlabel.Ymd[index], time, 3))
//	        {
//	            break;
//	        }
//	        
//	        if(index > 0)
//	        {
//	            index--;
//	        }
//	        else
//	        {
//	            index = SIZE_RECDAY_DAYS - 1;
//	        }
//	    }
//	
//	    if(uc_i == SIZE_RECDAY_DAYS)
//	    {
//	        return SYS_ERR_FT;
//	    }
//	                                            //��ȡ�ε����ݿ���ʼ��ַ,�Լ���(����)��Ϣ
//	    addr = _RecDataAddr(index, tno, segno, &segstr);
//	    if(addr == 0xffffffff)
//	    {
//	        return SYS_ERR_FT;
//	    }
//	
//	    if(false == ReadFlashWithCRC(DB_RECDAY, outbuffer, segstr->seglen, addr))
//	    {
//	        return SYS_ERR_FT;
//	    }
//	
//	    if(CmpBuffer(timept, time, timelen))
//	    {
//	        return SYS_ERR_FT;
//	    }
//	
//	    ES_OK();
//	}




/************************************************************************
 * @function: gdw_CrackRecData
 * @����: ��ձ�������
 * 
 * @����: 
 * @param:  
 * @����: 
 * @˵��: 
 * @����: yzy (2014/2/15)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void gdw_CrackRecData(void)
{
    S_RECDAY_TL tlabel;
    
    ClearBuffer((uint8*)&tlabel, sizeof(tlabel));
    //ES_FRAM_Open();
    //Fram_SafeWrite((uint8*)&tlabel, FM_RECPT, sizeof(tlabel));  //ZJC
    //ES_FRAM_Close();
}










/************************************************************************
 * �ն������Ϣ����
 ************************************************************************/
//	const S_DATASEGINFO gss_daySegInfo[] = 
//	{
//	    {0, LEN_DAY_SEG0},
//	    {2, LEN_DAY_SEG2},
//	};



/************************************************************************
 * @function: _DayDataAddr
 * @����: �ն���������ʼ��ַ
 * 
 * @����: 
 * @param: ymd ����,ʱ��
 * @param: tno ����,�������
 * @param: segno ����,�κ�
 * @param: outinfo ���,����Ϣ
 * @param: outbidx ���,λͼ����
 * 
 * @����: 
 * @return: uint32  
 * @˵��: 
 * @����: yzy (2014/2/16)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	uint32 _DayDataAddr(uint8* ymd, uint8 tno, uint8 segno, const S_DATASEGINFO** outinfo, uint16* outbidx)
//	{
//	    const S_DATASEGINFO* _s;
//	    uint32 addr;                            //����ֵ
//	    uint32 totallen;                        //��¼�ܳ���
//	    uint32 segoffset;                       //��λ�ڵ�ǰ��¼�е�ƫ��
//	    uint8 isGot;                            //�Ƿ��ҵ�ָ���Ķ�
//	    uint16 day;
//	
//	    totallen = 0;
//	    segoffset = 0;
//	    isGot = 0;
//	    for(uint8 uc_i = 0; uc_i < sizeof(gss_daySegInfo)/sizeof(S_DATASEGINFO); uc_i++)
//	    {
//	        _s = &gss_daySegInfo[uc_i];
//	        totallen += _s->seglen;
//	        if(!isGot)
//	        {
//	            if(_s->segno == segno)
//	            {
//	                isGot = 1;
//	                if(outinfo != __NULL)
//	                {
//	                    *outinfo = _s;
//	                }
//	            }
//	            else
//	            {
//	                segoffset += _s->seglen;
//	            }
//	        }
//	    }
//	
//	    if(!isGot)
//	    {
//	        return 0xFFFFFFFF;
//	    }
//	                                            //ADDRESS(DIVIDED BY TST)
//	    day = CalcDayHex(ymd[0], ymd[1], ymd[2]);
//	    day %= SIZE_DAY_DAYS;
//	                                            //��Ч��λͼ
//	    addr = SA_4096B_SORTED(SIZE_DAY_DAYS * CORE_TST_NO);
//	    if(outbidx != __NULL)
//	    {
//	        *outbidx = tno * SIZE_DAY_DAYS + day;
//	    }
//	                                            //���ݷֲ�
//	    addr += tno * (SIZE_DAY_DAYS * totallen);
//	    addr += day * totallen;
//	    addr += segoffset;
//	    return addr;
//	
//	};
//	
//	
//	
//	/************************************************************************ 
//	 *�ն���.����Чλ��Ӧ��ϵ
//	************************************************************************/
//	const S_EFT_MAP gss_EftMapRealToDaySeg0[] = 
//	{
//	    {EFT_WP, EFT_DAY_WP},
//	    {EFT_WN, EFT_DAY_WN},
//	};
//	
//	/************************************************************************ 
//	  **��2��
//	************************************************************************/
//	const S_EFT_MAP gss_EftMapDmdToDaySeg2[] = 
//	{
//	    {EFT_DMD_DPP,  EFT_DAY_DPP},
//	    {EFT_DMD_DPPT, EFT_DAY_DPPT},
//	    {EFT_DMD_DNP,  EFT_DAY_DNP},
//	    {EFT_DMD_DNPT, EFT_DAY_DNPT},
//	};
//	
//	
//	/************************************************************************
//	 * @function: _DayDataContextReady
//	 * @����: �ն������ݶε�׼��
//	 * 
//	 * @����: 
//	 * @param: tno �������
//	 * @param: segno �κ�
//	 * @param: opbuffer ��ŵ��ĵ�ַ.(�����ܷŵ������������)
//	 * 
//	 * @����: 
//	 * @return: uint16 �����ݳ���
//	 * @˵��: ������ŵ��������Ͳ���,ʵ������������Ҳ������
//	 * @����: yzy (2014/2/15)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	uint16 _DayDataContextReady(uint8 tno, uint8 segno, uint8* opbuffer)
//	{
//	    if(segno == 0)
//	    {
//	        S_DAYDATA_SEG0* daydata = (S_DAYDATA_SEG0*)opbuffer;
//	        daydata->seg.stime = gs_SysTime;
//	        daydata->seg.rtime = gss_TstRealData[tno].rtime;
//	        MoveBuffer((uint8*)gss_TstRealData[tno].Wp, (uint8*)daydata->seg.Wp, sizeof(daydata->seg.Wp));
//	        MoveBuffer((uint8*)gss_TstRealData[tno].Wn, (uint8*)daydata->seg.Wn, sizeof(daydata->seg.Wn));
//	        memset(daydata->seg.eft, 0, sizeof(daydata->seg.eft));
//	        MapEft(gss_TstRealData[tno].eft, daydata->seg.eft, (S_EFT_MAP*)gss_EftMapRealToDaySeg0, sizeof(gss_EftMapRealToDaySeg0)/ sizeof(S_EFT_MAP));
//	        return sizeof(S_DAYDATA_SEG0);
//	    }
//	    else if(segno == 2)
//	    {
//	        //���´���ռ�ý϶��ջ
//	        uint8 tempbuf[SA_FLASH_REALDATA_ITMBABY_BYTE];
//	        S_TstDmdData* dmdata =(S_TstDmdData*)tempbuf; 
//	        if(SA_REALDATA_Read(tempbuf, tno, 1) != SA_ERR_OK)
//	        {
//	             return 0;
//	        }
//	        //���ϴ���ռ�ý϶��ջ
//	                                        //PREPARE FOR DATA
//	        S_DAYDATA_SEG2* daydata = (S_DAYDATA_SEG2*)opbuffer;
//	        uint8 time[5];
//	        daydata->seg.stime = gs_SysTime;
//	        MoveBuffer((uint8*)dmdata->ymdhm , time , 5);
//	        BuildTimeLabel(TTYPE_YMDHMS, time, &daydata->seg.rtime);
//	        MoveBuffer((uint8*)dmdata->dpp,  (uint8*)daydata->seg.dpp,  sizeof(daydata->seg.dpp));
//	        MoveBuffer((uint8*)dmdata->dppt, (uint8*)daydata->seg.dppt, sizeof(daydata->seg.dppt));
//	        MoveBuffer((uint8*)dmdata->dnp,  (uint8*)daydata->seg.dnp,  sizeof(daydata->seg.dnp));
//	        MoveBuffer((uint8*)dmdata->dnpt, (uint8*)daydata->seg.dnpt, sizeof(daydata->seg.dnpt));
//	        memset(daydata->seg.eft, 0, sizeof(daydata->seg.eft));
//	        MapEft(dmdata->eft, daydata->seg.eft, (S_EFT_MAP*)gss_EftMapDmdToDaySeg2, sizeof(gss_EftMapDmdToDaySeg2)/ sizeof(S_EFT_MAP));
//	        return sizeof(S_DAYDATA_SEG2);
//	    }
//	
//	    return 0;
//	}
//	



/************************************************************************
 * @function: gdw_SaveTstDayData
 * @����: ��ĳ���������ʵʱ���ݶ���Ϊ����������
 * 
 * @����: 
 * @param: tno �������
 * @param: segno �κ�
 * 
 * @����: 
 * @return: uint8  �ɹ�/ʧ��
 * @˵��: 
 * @����: yzy (2014/2/16)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	uint8 gdw_SaveTstDayData(uint8 tno, uint8 segno)
//	{
//	    uint32 addr;
//	    uint16 dlen;
//	    uint16 bidx;
//	    uint8 opbuffer[MAX_LEN_DAY_SEG];
//	    const S_DATASEGINFO* segstr;
//	
//	    if(tno > CORE_TST_NO)
//	    {
//	        return SYS_ERR_FT;
//	    }
//	    
//	    if(gss_CDATATst[tno].ef != 0x01)
//	    {
//	        return SYS_ERR_FT;
//	    }
//	                                        //PREPARE FOR DATA
//	    addr = _DayDataAddr(gs_SysLastDayTime.YMDhms, tno, segno, &segstr, &bidx);
//	    if(addr == 0xffffffff)
//	    {
//	        return SYS_ERR_FT;
//	    }
//	
//	    dlen = _DayDataContextReady(tno, segno, opbuffer);
//	    if(dlen == segstr->seglen)
//	    {
//	        WriteFlashWithCRC(DB_DAY, opbuffer, dlen, addr);
//	        STR_ClrFlashBit(DB_DAY, 0, bidx, 0xffff);//0��Ч,1��Ч
//	    }
//	    
//	    ES_OK();
//	}



/************************************************************************
 * @function: gdw_SaveDayData
 * @����: ����ĳ�˿��µĲ�����������
 * 
 * @����: 
 * @param: port 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2014/2/16)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	uint8 gdw_SaveDayData(uint8 port)
//	{
//	    for(uint8 uc_i = 1; uc_i < CORE_TST_NO; uc_i++)
//	    {
//	        if(gss_CDATATst[uc_i].ef != 0x01)
//	        {
//	            continue;
//	        }
//	        gs_OS.TK_Sleep(5);
//	        
//	#if (GDW_07DIN_EN > 0)
//	        if(gss_CDATATst[uc_i].pf != TST_PFID_DLT07)     //07��Լ�ն������������ʵʱ���ݱ���
//	        {
//	            gdw_SaveTstDayData(uc_i, 0);
//	        }
//	#endif
//	        gdw_SaveTstDayData(uc_i, 2);
//	    }
//	
//	    ES_OK();
//	
//	}





/************************************************************************
 * @function: gdw_ReadDayData
 * @����: ��ȡĳ���������������
 * 
 * @����: 
 * @param: time ������
 * @param: tno �������
 * @param: segno �κ�
 * @param: outbuffer 
 * 
 * @����: 
 * @return: uint8  �ɹ�/ʧ��
 * @˵��: 
 * @����: yzy (2014/2/16)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	uint8 gdw_ReadDayData(uint8* time, uint8 tno, uint8 segno, uint8* outbuffer)
//	{
//	    uint32  addr;
//	    uint8   timecmp[3];
//	    uint8   timelen;
//	    uint16  bidx;
//	    const S_DATASEGINFO* segstr;
//	
//	    ES_VAR_CHECK(tno > CORE_TST_NO);
//	                                            //��ȡ�ε����ݿ���ʼ��ַ,�Լ���(����)��Ϣ
//	    addr = _DayDataAddr(time, tno, segno, &segstr, &bidx);
//	    if(addr == 0xffffffff)
//	    {
//	        return SYS_ERR_FT;
//	    }
//	                                            //��Чλͼ�ж�.0��Ч,1��Ч
//	    if(STR_IsFlashBitClr(DB_DAY, 0, bidx) == false)
//	    {
//	        ClearBuffer(outbuffer, segstr->seglen);
//	        return SYS_ERR_FT;
//	    }
//	                                            //��ȡ����
//	    if(false == ReadFlashWithCRC(DB_DAY, outbuffer, segstr->seglen, addr))
//	    {
//	        ClearBuffer(outbuffer, segstr->seglen);
//	        return SYS_ERR_FT;
//	    }
//	                                            //ʹ��ʵ�ʶ���ʱ�����һ��,��Ҫ��ʱ��Ƚ�
//	    MoveBuffer(((S_DATA_SEG_TEMPLET*)outbuffer)->seg.stime.YMDhms, timecmp, sizeof(timecmp));
//	    StepDays(timecmp, -1);
//	    timelen = 3;
//	    
//	    if(CmpBuffer(timecmp, time, timelen))
//	    {
//	        ClearBuffer(outbuffer, segstr->seglen);
//	        return SYS_ERR_FT;
//	    }
//	
//	    ES_OK();
//	
//	}


/************************************************************************
 * @function: gdw_CrackDayData
 * @����: ����ն�������
 * 
 * @����: 
 * @param: 
 * @����: 
 * @˵��: 
 * @����: yzy (2011-9-21)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	void gdw_CrackDayData(void)
//	{
//	    TDataBlock db;
//	    ES_FILE_DB_Open(DB_DAY, &db, TDB_MODE_RW);
//	    ES_FILE_DB_Erase(&db, 0, 2);
//	    ES_FILE_DB_Close(&db);
//	}









/************************************************************************
 * �¶������Ϣ����
 ************************************************************************/
//	const S_DATASEGINFO gss_MonthSegInfo[] = 
//	{
//	    {0, LEN_MON_SEG0},
//	    {2, LEN_MON_SEG2},
//	};
//	
//	
//	
//	/************************************************************************
//	 * @function: _MonthDataAddr
//	 * @����: �¶���������ʼ��ַ
//	 * 
//	 * @����: 
//	 * @param: ym  ����,ʱ��
//	 * @param: tno ����,�������
//	 * @param: segno ����,�κ�
//	 * @param: outinfo ���,����Ϣ
//	 * @param: outbidx ���,λͼ����
//	 * 
//	 * @����: 
//	 * @return: uint32  
//	 * @˵��: 
//	 * @����: yzy (2014/2/20)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	uint32 _MonthDataAddr(uint8* ym, uint8 tno, uint8 segno, const S_DATASEGINFO** outinfo, uint16* outbidx)
//	{
//	    const S_DATASEGINFO* _s;
//	    uint32 addr;                            //����ֵ
//	    uint32 totallen;                        //��¼�ܳ���
//	    uint32 segoffset;                       //��λ�ڵ�ǰ��¼�е�ƫ��
//	    uint8 isGot;                            //�Ƿ��ҵ�ָ���Ķ�
//	    uint16 month;
//	
//	    totallen = 0;
//	    segoffset = 0;
//	    isGot = 0;
//	    for(uint8 uc_i = 0; uc_i < sizeof(gss_MonthSegInfo)/sizeof(S_DATASEGINFO); uc_i++)
//	    {
//	        _s = &gss_MonthSegInfo[uc_i];
//	        totallen += _s->seglen;
//	        if(!isGot)
//	        {
//	            if(_s->segno == segno)
//	            {
//	                isGot = 1;
//	                if(outinfo != __NULL)
//	                {
//	                    *outinfo = _s;
//	                }
//	            }
//	            else
//	            {
//	                segoffset += _s->seglen;
//	            }
//	        }
//	    }
//	
//	    if(!isGot)
//	    {
//	        return 0xFFFFFFFF;
//	    }
//	                                            //ADDRESS(DIVIDED BY TST)
//	    month = ym[0] * 12 + ym[1] - 1;
//	    month %= SIZE_MON_MONS;
//	                                            //��Ч��λͼ
//	    addr = SA_4096B_SORTED(SIZE_MON_MONS * CORE_TST_NO);
//	    if(outbidx != __NULL)
//	    {
//	        *outbidx = tno * SIZE_MON_MONS + month;
//	    }
//	                                            //���ݷֲ�
//	    addr += tno * (SIZE_MON_MONS * totallen);
//	    addr += month * totallen;
//	    addr += segoffset;
//	    return addr;
//	
//	};
//	
//	
//	
//	/************************************************************************
//	 * @function: _MonthDataContextReady
//	 * @����: �¶������ݶε�׼��
//	 * 
//	 * @����: 
//	 * @param: tno �������
//	 * @param: segno �κ�
//	 * @param: opbuffer ��ŵ��ĵ�ַ.(�����ܷŵ������������)
//	 * 
//	 * @����: 
//	 * @return: uint16 �����ݳ���
//	 * @˵��: ������ŵ��������Ͳ���,ʵ������������Ҳ������
//	 * @����: yzy (2014/2/15)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	uint16 _MonthDataContextReady(uint8 tno, uint8 segno, uint8* opbuffer)
//	{
//	    return _DayDataContextReady(tno, segno, opbuffer);
//	}
//	
//	
//	
//	
//	/************************************************************************
//	 * @function: gdw_SaveTstMonthData
//	 * @����: ��ĳ���������ʵʱ���ݶ���Ϊ����������
//	 * 
//	 * @����: 
//	 * @param: tno �������
//	 * @param: segno �κ�
//	 * 
//	 * @����: 
//	 * @return: uint8  �ɹ�/ʧ��
//	 * @˵��: 
//	 * @����: yzy (2014/2/16)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	uint8 gdw_SaveTstMonthData(uint8 tno, uint8 segno)
//	{
//	    uint32 addr;
//	    uint16 dlen;
//	    uint16 bidx;
//	    uint8 opbuffer[MAX_LEN_MON_SEG];
//	    const S_DATASEGINFO* segstr;
//	
//	    if(tno > CORE_TST_NO)
//	    {
//	        return SYS_ERR_FT;
//	    }
//	    
//	    if(gss_CDATATst[tno].ef != 0x01)
//	    {
//	        return SYS_ERR_FT;
//	    }
//	                                        //PREPARE FOR DATA
//	    addr = _MonthDataAddr(gs_SysLastDayTime.YMDhms, tno, segno, &segstr, &bidx);
//	    if(addr == 0xffffffff)
//	    {
//	        return SYS_ERR_FT;
//	    }
//	
//	    dlen = _MonthDataContextReady(tno, segno, opbuffer);
//	    if(dlen == segstr->seglen)
//	    {
//	        WriteFlashWithCRC(DB_MONTH, opbuffer, dlen, addr);
//	        STR_ClrFlashBit(DB_MONTH, 0, bidx, 0xffff);//0��Ч,1��Ч
//	    }
//	    
//	    ES_OK();
//	}
//	
//	
//	/************************************************************************
//	 * @function: gdw_SaveMonthData
//	 * @����: ����ĳ�˿��µĲ�������������
//	 * 
//	 * @����: 
//	 * @param: port 
//	 * 
//	 * @����: 
//	 * @return: uint8  
//	 * @˵��: 
//	 * @����: yzy (2014/2/20)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	uint8 gdw_SaveMonthData(uint8 port)
//	{
//	    for(uint8 uc_i = 1; uc_i < CORE_TST_NO; uc_i++)
//	    {
//	        if((gss_CDATATst[uc_i].ef != 0x01) || (gss_CDATATst[uc_i].pt != port))
//	        {
//	            continue;
//	        }
//	        gs_OS.TK_Sleep(5);
//	
//	        gdw_SaveTstMonthData(uc_i, 0);
//	        gdw_SaveTstMonthData(uc_i, 2);
//	    }
//	
//	    ES_OK();
//	}
//	
//	
//	
//	
//	/************************************************************************
//	 * @function: gdw_ReadMonthData
//	 * @����: ��ȡĳ���������������
//	 * 
//	 * @����: 
//	 * @param: time ����
//	 * @param: tno �������
//	 * @param: segno �κ�
//	 * @param: outbuffer �������
//	 * 
//	 * @����: 
//	 * @return: uint8  
//	 * @˵��: 
//	 * @����: yzy (2014/2/20)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	uint8 gdw_ReadMonthData(uint8* time, uint8 tno, uint8 segno, uint8* outbuffer)
//	{
//	    uint32  addr;
//	    uint8   timecmp[3];
//	    uint8   timelen;
//	    uint16  bidx;
//	    const S_DATASEGINFO* segstr;
//	
//	    ES_VAR_CHECK(tno > CORE_TST_NO);
//	                                            //��ȡ�ε����ݿ���ʼ��ַ,�Լ���(����)��Ϣ
//	    addr = _MonthDataAddr(time, tno, segno, &segstr, &bidx);
//	    if(addr == 0xffffffff)
//	    {
//	        return SYS_ERR_FT;
//	    }
//	                                            //��Чλͼ�ж�.0��Ч,1��Ч
//	    if(STR_IsFlashBitClr(DB_MONTH, 0, bidx) == false)
//	    {
//	        ClearBuffer(outbuffer, segstr->seglen);
//	        return SYS_ERR_FT;
//	    }
//	                                            //��ȡ����
//	    if(false == ReadFlashWithCRC(DB_MONTH, outbuffer, segstr->seglen, addr))
//	    {
//	        ClearBuffer(outbuffer, segstr->seglen);
//	        return SYS_ERR_FT;
//	    }
//	                                            //ʹ��ʵ�ʶ���ʱ�����һ��,��Ҫ��ʱ��Ƚ�
//	    MoveBuffer(((S_DATA_SEG_TEMPLET*)outbuffer)->seg.stime.YMDhms, timecmp, sizeof(timecmp));
//	    StepDays(timecmp, -1);
//	    timelen = 2;
//	    
//	    if(CmpBuffer(((S_DATA_SEG_TEMPLET*)outbuffer)->seg.stime.YMDhms, time, timelen) && CmpBuffer(timecmp, time, timelen))
//	    {
//	        ClearBuffer(outbuffer, segstr->seglen);
//	        return SYS_ERR_FT;
//	    }
//	
//	    ES_OK();
//	
//	}
//	
//	
//	
//	
//	
//	/************************************************************************
//	 * @function: gdw_CrackMonthData
//	 * @����: ����¶�������
//	 * 
//	 * @����: 
//	 * @param: 
//	 * @����: 
//	 * @˵��: 
//	 * @����: yzy (2011-9-21)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	void gdw_CrackMonthData(void)
//	{
//	    TDataBlock db;
//	    ES_FILE_DB_Open(DB_MONTH, &db, TDB_MODE_RW);
//	    ES_FILE_DB_Erase(&db, 0, 2);
//	    ES_FILE_DB_Close(&db);
//	}








/************************************************************************
 * ���߶������Ϣ����
 ************************************************************************/
//	const S_DATASEGINFO gss_HourSegInfo[] = 
//	{
//	    {0, LEN_HOUR_SEG0},
//	};
//	
//	
//	
//	/************************************************************************
//	 * @function: _HourDataAddr
//	 * @����: ���߶���������ʼ��ַ
//	 * 
//	 * @����: 
//	 * @param: ymdh ����,������ʱ
//	 * @param: tno ����,�������
//	 * @param: segno ����,�κ�
//	 * @param: outinfo ���,����Ϣ
//	 * @param: outbidx ���,λͼ����
//	 * 
//	 * @����: 
//	 * @return: uint32  
//	 * @˵��: 
//	 * @����: yzy (2014/2/16)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	uint32 _HourDataAddr(uint8* ymdh, uint8 tno, uint8 segno, const S_DATASEGINFO** outinfo, uint16* outbidx)
//	{
//	    const S_DATASEGINFO* _s;
//	    uint32 addr;                            //����ֵ
//	    uint32 totallen;                        //��¼�ܳ���
//	    uint32 segoffset;                       //��λ�ڵ�ǰ��¼�е�ƫ��
//	    uint8 isGot;                            //�Ƿ��ҵ�ָ���Ķ�
//	    uint32 hour;
//	
//	    totallen = 0;
//	    segoffset = 0;
//	    isGot = 0;
//	    for(uint8 uc_i = 0; uc_i < sizeof(gss_HourSegInfo)/sizeof(S_DATASEGINFO); uc_i++)
//	    {
//	        _s = &gss_HourSegInfo[uc_i];
//	        totallen += _s->seglen;
//	        if(!isGot)
//	        {
//	            if(_s->segno == segno)
//	            {
//	                isGot = 1;
//	                if(outinfo != __NULL)
//	                {
//	                    *outinfo = _s;
//	                }
//	            }
//	            else
//	            {
//	                segoffset += _s->seglen;
//	            }
//	        }
//	    }
//	
//	    if(!isGot)
//	    {
//	        return 0xFFFFFFFF;
//	    }
//	                                            //ADDRESS(DIVIDED BY TST)
//	    hour =  CalcDayHex(ymdh[0], ymdh[1], ymdh[2]) * 24 + ymdh[3];
//	    hour %= SIZE_HOUR_HOURS;
//	                                            //��Ч��λͼ
//	    addr = SA_4096B_SORTED(SIZE_HOUR_HOURS * CORE_TST_NO) / 8;
//	    if(outbidx != __NULL)
//	    {
//	        *outbidx = tno * SIZE_HOUR_HOURS + hour;
//	    }
//	                                            //���ݷֲ�
//	    addr += tno * (SIZE_HOUR_HOURS * totallen);
//	    addr += hour * totallen;
//	    addr += segoffset;
//	    return addr;
//	
//	};
//	
//	
//	
//	/************************************************************************ 
//	 *���߶���.����Чλ��Ӧ��ϵ
//	************************************************************************/
//	const S_EFT_MAP gss_EftMapRealToHourSeg0[] = 
//	{
//	    {EFT_WP, EFT_HOUR_WP},
//	    {EFT_WN, EFT_HOUR_WN},
//	    {EFT_I,  EFT_HOUR_I},
//	    {EFT_U,  EFT_HOUR_U},
//	};
//	
//	
//	
//	/************************************************************************
//	 * @function: _HourDataContextReady
//	 * @����: Сʱ�������ݶε�׼��
//	 * 
//	 * @����: 
//	 * @param: tno �������
//	 * @param: segno �κ�
//	 * @param: opbuffer ��ŵ��ĵ�ַ.(�����ܷŵ������������)
//	 * 
//	 * @����: 
//	 * @return: uint16 �����ݳ���
//	 * @˵��: ������ŵ��������Ͳ���,ʵ������������Ҳ������
//	 * @����: yzy (2014/2/15)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	uint16 _HourDataContextReady(uint8 tno, uint8 segno, uint8* opbuffer)
//	{
//	    if(segno == 0)
//	    {
//	        S_HOURDATA_SEG0* hourdata = (S_HOURDATA_SEG0*)opbuffer;
//	        hourdata->seg.stime = gs_SysTime;
//	        hourdata->seg.rtime = gss_TstRealData[tno].rtime;
//	        hourdata->seg.Wp = gss_TstRealData[tno].Wp[0];
//	        hourdata->seg.Wn = gss_TstRealData[tno].Wn[0];
//	        hourdata->seg.I = gss_TstRealData[tno].I[0];
//	        MoveBuffer((uint8*)&gss_TstRealData[tno].U[0], (uint8*)&hourdata->seg.U[0], sizeof(hourdata->seg.U));
//	        memset(hourdata->seg.eft, 0, sizeof(hourdata->seg.eft));
//	        MapEft(gss_TstRealData[tno].eft, hourdata->seg.eft, (S_EFT_MAP*)gss_EftMapRealToHourSeg0, sizeof(gss_EftMapRealToHourSeg0)/ sizeof(S_EFT_MAP));
//	        return sizeof(S_HOURDATA_SEG0);
//	    }
//	
//	    return 0;
//	}
//	
//	
//	
//	
//	/************************************************************************
//	 * @function: gdw_SaveTstHourData
//	 * @����: ��ĳ���������ʵʱ���ݶ���Ϊ��������
//	 * 
//	 * @����: 
//	 * @param: tno �������
//	 * @param: segno �κ�
//	 * 
//	 * @����: 
//	 * @return: uint8  �ɹ�/ʧ��
//	 * @˵��: 
//	 * @����: yzy (2014/2/16)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	uint8 gdw_SaveTstHourData(uint8 tno, uint8 segno)
//	{
//	    uint32 addr;
//	    uint16 dlen;
//	    uint16 bidx;
//	    uint8 opbuffer[MAX_LEN_HOUR_SEG];
//	    const S_DATASEGINFO* segstr;
//	
//	    if(tno > CORE_TST_NO)
//	    {
//	        return SYS_ERR_FT;
//	    }
//	    
//	    if(gss_CDATATst[tno].ef != 0x01)
//	    {
//	        return SYS_ERR_FT;
//	    }
//	                                        //PREPARE FOR DATA
//	    addr = _HourDataAddr(gs_SysTime.YMDhms, tno, segno, &segstr, &bidx);
//	    if(addr == 0xffffffff)
//	    {
//	        return SYS_ERR_FT;
//	    }
//	
//	    dlen = _HourDataContextReady(tno, segno, opbuffer);
//	    if(dlen == segstr->seglen)
//	    {
//	        WriteFlashWithCRC(DB_HOUR, opbuffer, dlen, addr);
//	        STR_ClrFlashBit(DB_HOUR, 0, bidx, 0xffff);//0��Ч,1��Ч
//	    }
//	    
//	    ES_OK();
//	}
//	
//	
//	
//	/************************************************************************
//	 * @function: gdw_SaveHourData
//	 * @����: ����ĳ�˿��µĲ�����Сʱ����
//	 * 
//	 * @����: 
//	 * @param: port 
//	 * 
//	 * @����: 
//	 * @return: uint8  
//	 * @˵��: 
//	 * @����: yzy (2014/2/20)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	uint8 gdw_SaveHourData(uint8 port)
//	{
//	    for(uint8 uc_i = 1; uc_i < CORE_TST_NO; uc_i++)
//	    {
//	        if((gss_CDATATst[uc_i].ef != 0x01) || (gss_CDATATst[uc_i].pt != port))
//	        {
//	            continue;
//	        }
//	        gs_OS.TK_Sleep(5);
//	        gdw_SaveTstHourData(uc_i, 0);
//	    }
//	
//	    ES_OK();
//	
//	}
//	
//	
//	
//	
//	
//	
//	/************************************************************************
//	 * @function: gdw_ReadHourData
//	 * @����: ��ȡĳ���������Сʱ����
//	 * 
//	 * @����: 
//	 * @param: time ������ʱ��
//	 * @param: tno �������
//	 * @param: segno �κ�
//	 * @param: outbuffer 
//	 * 
//	 * @����: 
//	 * @return: uint8  
//	 * @˵��: 
//	 * @����: yzy (2014/2/20)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	uint8 gdw_ReadHourData(uint8* time, uint8 tno, uint8 segno, uint8* outbuffer)
//	{
//	    uint32 addr;
//	    uint8* timept;
//	    uint8  timelen;
//	    uint16 bidx;
//	    const S_DATASEGINFO* segstr;
//	
//	    ES_VAR_CHECK((tno > CORE_TST_NO) || (tno == 0));
//	                                            //��ȡ�ε����ݿ���ʼ��ַ,�Լ���(����)��Ϣ
//	    addr = _HourDataAddr(time, tno, segno, &segstr, &bidx);
//	    if(addr == 0xffffffff)
//	    {
//	        return SYS_ERR_FT;
//	    }
//	                                            //��Чλͼ�ж�.0��Ч,1��Ч
//	    if(STR_IsFlashBitClr(DB_HOUR, 0, bidx) == false)
//	    {
//	        return SYS_ERR_FT;
//	    }
//	                                            //��ȡ����
//	    if(false == ReadFlashWithCRC(DB_HOUR, outbuffer, segstr->seglen, addr))
//	    {
//	        return SYS_ERR_FT;
//	    }
//	
//	    timept = ((S_DATA_SEG_TEMPLET*)outbuffer)->seg.stime.YMDhms;
//	    timelen = 4;
//	    if(CmpBuffer(timept, time, timelen))
//	    {
//	        return SYS_ERR_FT;
//	    }
//	
//	    ES_OK();
//	
//	}
//	
//	
//	
//	/************************************************************************
//	 * @function: gdw_CrackHourData
//	 * @����: ���Сʱ��������
//	 * @����: 
//	 * @����: 
//	 * @˵��: 
//	 * @����: yzy (2014/2/20)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	void gdw_CrackHourData(void)
//	{
//	    TDataBlock db;
//	    ES_FILE_DB_Open(DB_HOUR, &db, TDB_MODE_RW);
//	    ES_FILE_DB_Erase(&db, 0, 2);
//	    ES_FILE_DB_Close(&db); 
//	}

/************************************************************************
 * @function: gdw_SaveCureData
 * @����: �洢��������(���15����)
 * @����: 
 * @param: time ������ʱ����
 * @param: tno �������
 * @param: outbuffer �������
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: zjc (2014/8/19)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 gdw_SaveCureData(uint8* time, uint8 tno, uint8* outbuffer)
{    
    uint32 addr;
    uint32 min;
    uint8_t ret = 0;
    SYS_VAR_CHECK(tno > CORE_TST_NO);
    
    min = CalcMinuteHex(time[0], time[1], time[2], time[3], time[4]); //�ܷ�����
    min = min / gs_PstPara.cp_interval;  //�õ��ܵ���
    min %= SIZE_CUR_POINTS;  //�洢λ�õ���
    
    addr = SA_4096B_SORTED(SA_8B_SORTED(SIZE_CUR_POINTS * CORE_TST_NO) >> 3);  //����λͼ������
    addr =addr + (tno * SIZE_CUR_POINTS * LEN_CUR_SEG0);
    addr =addr + (min * LEN_CUR_SEG0);
                                        //�洢����
    ret = WriteFlashWithCRC(DB_CURE, outbuffer, sizeof(S_CURDATA_SEG0), addr);
    STR_ClrFlashBit(DB_CURE, 0, min, 0xffff);//0��Ч,1��Ч
    //SYS_OK();
    return ret;
}

/************************************************************************
 * @function: gdw_ReadCureData
 * @����: ��ȡ��������
 * 
 * @����: 
 * @param: time ������ʱ��
 * @param: outbuffer �������
 * 
 * @����: 
 * @return: uint8  
 * @˵��: �ݲ���λͼ,���岻���
 * @����: ZJC (2014/8/19)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 gdw_ReadCureData(uint8* time, uint8 tno, uint8* outbuffer)
{
    uint32 addr = 0;
    uint32 min = 0;
    
    SYS_VAR_CHECK(tno > CORE_TST_NO);
                                        //�洢��ַ
    min = CalcMinuteHex(time[0], time[1], time[2], time[3], time[4]); //�ܷ�����
    min = min / gs_PstPara.cp_interval;  //�õ��ܵ���
    min %= SIZE_CUR_POINTS;  //�洢λ�õ���
    
    addr = SA_4096B_SORTED(SA_8B_SORTED(SIZE_CUR_POINTS * CORE_TST_NO) >> 3);  //����λͼ������
    addr =addr + (tno * SIZE_CUR_POINTS * LEN_CUR_SEG0);
    addr =addr + (min * LEN_CUR_SEG0);
    
    if(STR_IsFlashBitClr(DB_CURE, 0, min) == false)  //��Чλͼ�ж�.0��Ч,1��Ч
    {
        ClearBuffer(outbuffer, sizeof(S_CURDATA_SEG0)); 
        return SYS_ERR_FT;
    }
                                        //��ȡ����
    if(SYS_ERR_OK != ReadFlashWithCRC(DB_CURE, outbuffer, sizeof(S_CURDATA_SEG0), addr))
    {
        ClearBuffer(outbuffer, sizeof(S_CURDATA_SEG0)); 
        return SYS_ERR_FT;
    }
    
    if(CmpBuffer(((S_CURDATA_SEG0 *)outbuffer)->seg.stime.YMDhms, time, 5)) //ʱ��ȶ�
    {
        ClearBuffer(outbuffer, sizeof(S_CURDATA_SEG0)); 
        return SYS_ERR_FT;
    }
    SYS_OK();
} 

/************************************************************************
 * @function: gdw_CrackCureData
 * @����: �����������
 * 
 * @����: 
 * @param: 
 * @����: 
 * @˵��: 
 * @����: zjc (2014-8-24)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void gdw_CrackCureData(void)
{
    TDataBlock db;
    SYS_FILE_DB_Open(DB_CURE, &db, TDB_MODE_RW);//
    SYS_FILE_DB_Erase(&db, 0, ((SA_8B_SORTED(SIZE_CUR_POINTS * CORE_TST_NO) >> 3)/0x1000)+1);  //��λͼ
    SYS_FILE_DB_Close(&db);
}


/************************************************************************
 * @function: gdw_SaveQuarData
 * @����: �洢��ƽ����ѹ����(���15����)
 * @����: 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: �ݲ���λͼ,���岻���
 * @����: yzy (2014/2/20)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	uint8 gdw_SaveQuarData(void)
//	{    
//	    S_QUARDATA_SEG0 quardata;
//	    uint8 time[6];
//	    uint32 addr;
//	    uint32 min;
//	                                        //��������
//	    quardata.seg.stime = gs_SysTime;
//	    quardata.seg.vol   = gul_MinVoltage;
//	                                        //�洢��ַ
//	    MoveBuffer(gs_SysTime.YMDhms, time, 5);
//	    min = CalcDayHex(time[0], time[1], time[2]);
//	    min += (min * 96) + (time[3] * 4) + (time[4] / 15);
//	    min %= SIZE_QUAR_POINTS;
//	    addr = min * LEN_QUAR_SEG0;
//	                                        //�洢����
//	    WriteFlashWithCRC(DB_QUAR, (uint8*)&quardata, sizeof(S_QUARDATA_SEG0), addr);
//	    ES_OK();
//	}
//	
//	
//	
//	
//	
//	
//	
//	/************************************************************************
//	 * @function: gdw_ReadQuarData
//	 * @����: ��ȡ�洢�ķ�ƽ����ѹ
//	 * 
//	 * @����: 
//	 * @param: time ������ʱ��
//	 * @param: quardata �������
//	 * 
//	 * @����: 
//	 * @return: uint8  
//	 * @˵��: �ݲ���λͼ,���岻���
//	 * @����: yzy (2014/2/20)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	uint8 gdw_ReadQuarData(uint8* time, S_QUARDATA_SEG0* quardata)
//	{
//	    uint32 addr;
//	    uint32 min;
//	    uint8* timept;
//	    uint8 timelen;
//	                                        //�洢��ַ
//	    min = CalcDayHex(time[0], time[1], time[2]);
//	    min += (min * 96) + (time[3] * 4) + (time[4] / 15);
//	    min %= SIZE_QUAR_POINTS;
//	    addr = min * LEN_QUAR_SEG0;
//	                                        //��ȡ����
//	    if(false == ReadFlashWithCRC(DB_QUAR, (uint8*)quardata, sizeof(S_QUARDATA_SEG0), addr))
//	    {
//	        return SYS_ERR_FT;
//	    }
//	                                        //ʱ��ȶ�
//	    timept = quardata->seg.stime.YMDhms;
//	    timelen = 4;
//	    if(CmpBuffer(timept, time, timelen))
//	    {
//	        return SYS_ERR_FT;
//	    }
//	    ES_OK();
//	} 
//	
//	
//	
//	
//	
//	
//	
//	
//	
//	
//	
//	
//	/************************************************************************
//	 * @function: _DayStatAddr
//	 * @����: ͳ�������ݴ洢��ַ
//	 * 
//	 * @����: 
//	 * @param: ymd ����,����
//	 * @param: tno ����,�������
//	 * @param: outbidx ���,λͼ����
//	 * 
//	 * @����: 
//	 * @return: uint32  
//	 * @˵��: 
//	 * @����: yzy (2014/2/20)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	uint32 _DayStatAddr(uint8* ymd, uint8 tno, uint16* outbidx)
//	{
//	    uint32 addr;
//	    uint16 day;
//	                                            //����
//	    day =  CalcDayHex(ymd[0], ymd[1], ymd[2]);
//	    day %= SIZE_DAY_DAYS;
//	                                            //��Ч��λͼ
//	    addr = SA_4096B_SORTED(SIZE_DAY_DAYS * CORE_TST_NO);
//	    if(outbidx != __NULL)
//	    {
//	        *outbidx = tno * SIZE_DAY_DAYS + day;
//	    }
//	                                            //���ݷֲ�
//	    addr += tno * (SIZE_DAY_DAYS * sizeof(S_CBVOL_SEG0));
//	    addr += day * sizeof(S_CBVOL_SEG0);
//	
//	    return addr;
//	}
//	
//	
//	/************************************************************************
//	 * @function: gdw_SaveTstDayStat
//	 * @����: �洢�������ѹͳ��������
//	 * @����: 
//	 * 
//	 * @����: 
//	 * @return: uint8  
//	 * @˵��: 
//	 * @����: yzy (2014/2/20)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	uint8 gdw_SaveTstDayStat(void)
//	{
//	    S_CBVOL_SEG0 daydata;
//	    uint32 addr;
//	    uint16 bidx;
//	    S_CBStat* pstat;
//	    
//	    uint8 buffer[SA_FLASH_REALDATA_ITMBABY_BYTE];
//	    CBMixStat* statblock = (CBMixStat*)buffer;
//	                                    //��������ѭ��
//	    for(uint8 k = 1; k < CORE_TST_NO; k++)
//	    {
//	        if(gss_CDATATst[k].ef != 0x01)
//	        {
//	            continue;
//	        }
//	                                        //��ȡͳ������
//	        if(SA_REALDATA_Read(buffer, k, 3) != SA_ERR_OK)
//	        {
//	            continue;
//	        }
//	                                        //��ȡ��ͳ������
//	        pstat = &(statblock->daystat);
//	                                        //׼���ն�����������
//	        daydata.seg.stime = gs_SysTime;
//	        MoveBuffer((uint8*)pstat, (uint8*)&(daydata.seg.bank1), sizeof(S_CBStat));
//	                                        //�����ն�������
//	        addr = _DayStatAddr(gs_SysLastDayTime.YMDhms, k, &bidx);
//	        WriteFlashWithCRC(DB_DAYSTAT, (uint8*)&daydata, sizeof(daydata), addr);
//	        STR_ClrFlashBit(DB_DAYSTAT, 0, bidx, 0xffff);//0��Ч,1��Ч
//	                                        //��λ��ͳ������
//	        memset(pstat, 0, sizeof(S_CBStat));
//	        pstat->rtime = gs_SysTime;
//	        pstat->Vmin[0] = 0xffffffff;
//	        pstat->Vmin[1] = 0xffffffff;
//	        pstat->Vmin[2] = 0xffffffff;
//	        SA_REALDATA_Write(buffer, k, 3);
//	    } 
//	    ES_OK();
//	         
//	}
//	
//	
//	
//	
//	/************************************************************************
//	 * @function: gdw_ReadTstDayStat
//	 * @����: �洢�������ѹͳ��������
//	 * 
//	 * @����: 
//	 * @param: time ������
//	 * @param: pn �������
//	 * @param: voldata �������
//	 * 
//	 * @����: 
//	 * @return: uint8  
//	 * @˵��: 
//	 * @����: yzy (2014/2/20)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	uint8 gdw_ReadTstDayStat(uint8* time, uint8 tno, S_CBVOL_SEG0* voldata)
//	{
//	    uint32  addr;
//	    uint8   timecmp[3];
//	    uint8   timelen;
//	    uint16  bidx;
//	    
//	    ES_VAR_CHECK(tno > CORE_TST_NO);
//	                                            //�����ַ
//	    addr = _DayStatAddr(time, tno, &bidx);
//	                                            //��Чλͼ�ж�.0��Ч,1��Ч
//	    if(STR_IsFlashBitClr(DB_DAYSTAT, 0, bidx) == false)
//	    {
//	        return SYS_ERR_FT;
//	    }
//	                                            //��ȡ����
//	    if(false == ReadFlashWithCRC(DB_DAYSTAT, (uint8*)voldata, sizeof(S_CBVOL_SEG0), addr))
//	    {
//	        return SYS_ERR_FT;  
//	    }
//	                                            //�ȶ�ʱ��
//	    MoveBuffer(voldata->seg.stime.YMDhms, timecmp, sizeof(timecmp));
//	    StepDays(timecmp, -1);
//	    timelen = 3;
//	    if(CmpBuffer(timecmp, time, timelen))
//	    {
//	        return SYS_ERR_FT;
//	    }
//	    ES_OK();
//	}
//	
//	
//	
//	/************************************************************************
//	 * @function: gdw_CrackDayStat
//	 * @����: �����ͳ������
//	 * @����: 
//	 * @����: 
//	 * @˵��: 
//	 * @����: yzy (2014/2/20)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	void gdw_CrackDayStat(void)
//	{
//	    TDataBlock db;
//	    ES_FILE_DB_Open(DB_DAYSTAT, &db, TDB_MODE_RW);
//	    ES_FILE_DB_Erase(&db, 0, 2);
//	    ES_FILE_DB_Close(&db); 
//	}
//	
//	
//	
//	
//	
//	
//	/************************************************************************
//	 * @function: _MonthStatAddr
//	 * @����: ͳ�������ݴ洢��ַ
//	 * 
//	 * @����: 
//	 * @param: ymd 
//	 * @param: tno 
//	 * 
//	 * @����: 
//	 * @return: uint32  
//	 * @˵��: 
//	 * @����: yzy (2014/2/20)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	uint32 _MonthStatAddr(uint8* ymd, uint8 tno, uint16* outbidx)
//	{
//	    uint32 addr;
//	    uint16 month;
//	                                            //����
//	    month = ymd[0] * 12 + ymd[1] - 1;
//	    month %= SIZE_MON_MONS;
//	                                            //��Ч��λͼ
//	    addr = SA_4096B_SORTED(SIZE_MON_MONS * CORE_TST_NO);
//	    if(outbidx != __NULL)
//	    {
//	        *outbidx = tno * SIZE_MON_MONS + month;
//	    }
//	                                            //���ݷֲ�
//	    addr += tno * (SIZE_MON_MONS *  64);
//	    addr += month * 64;
//	
//	    return addr;
//	}
//	
//	
//	
//	
//	/************************************************************************
//	 * @function: gdw_SaveTstMonthStat
//	 * @����: �洢�������ѹͳ��������
//	 * @����: 
//	 * 
//	 * @����: 
//	 * @return: uint8  
//	 * @˵��: 
//	 * @����: yzy (2014/2/20)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	uint8 gdw_SaveTstMonthStat(void)
//	{
//	    S_CBVOL_SEG0 mondata;
//	    uint32 addr;
//	    uint16 bidx;
//	    S_CBStat* pstat;
//	    
//	    uint8 buffer[SA_FLASH_REALDATA_ITMBABY_BYTE];
//	    CBMixStat* statblock = (CBMixStat*)buffer;
//	    
//	    for(uint8 k = 1; k < CORE_TST_NO; k++)
//	    {
//	        if(gss_CDATATst[k].ef != 0x01)
//	        {
//	            continue;
//	        }
//	        
//	        if(SA_REALDATA_Read(buffer, k, 3) != SA_ERR_OK)
//	        {
//	            continue;
//	        }
//	                                    //��ȡ��ͳ������
//	        pstat = &(statblock->monstat);
//	                                    //׼���¶�����������
//	        mondata.seg.stime = gs_SysTime;
//	        MoveBuffer((uint8*)pstat, (uint8*)&(mondata.seg.bank1), sizeof(S_CBStat));
//	                                    //�����¶�������
//	        addr = _MonthStatAddr(gs_SysLastDayTime.YMDhms, k, &bidx);
//	        WriteFlashWithCRC(DB_MONSTAT, (uint8*)&mondata, sizeof(mondata), addr);
//	        STR_ClrFlashBit(DB_MONSTAT, 0, bidx, 0xffff);//0��Ч,1��Ч
//	                                    //��λ��ͳ������
//	        memset(pstat, 0, sizeof(S_CBStat));
//	        pstat->rtime = gs_SysTime;
//	        pstat->Vmin[0] = 0xffffffff;
//	        pstat->Vmin[1] = 0xffffffff;
//	        pstat->Vmin[2] = 0xffffffff;
//	        SA_REALDATA_Write(buffer, k, 3);
//	    } 
//	    ES_OK();
//	}
//	
//	
//	
//	
//	
//	/************************************************************************
//	 * @function: gdw_ReadTstMonthStat
//	 * @����: ��ȡ�������ѹͳ��������
//	 * 
//	 * @����: 
//	 * @param: time 
//	 * @param: tno 
//	 * @param: voldata 
//	 * 
//	 * @����: 
//	 * @return: uint8  
//	 * @˵��: 
//	 * @����: yzy (2014/2/20)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	uint8 gdw_ReadTstMonthStat(uint8* time, uint8 tno, S_CBVOL_SEG0* voldata)
//	{
//	    uint32  addr;
//	    uint8   timecmp[3];
//	    uint8   timelen;
//	    uint16  bidx;
//	    
//	    ES_VAR_CHECK(tno > CORE_TST_NO);
//	                                            //��ַ����
//	    addr = _MonthStatAddr(time, tno, &bidx);
//	                                            //��Чλͼ�ж�.0��Ч,1��Ч
//	    if(STR_IsFlashBitClr(DB_MONSTAT, 0, bidx) == false)
//	    {
//	        return SYS_ERR_FT;
//	    }
//	                                            //��ȡ����
//	    if(false == ReadFlashWithCRC(DB_MONSTAT, (uint8*)voldata, sizeof(S_CBVOL_SEG0), addr))
//	    {
//	        return SYS_ERR_FT;
//	    }
//	                                            //�ȶ�ʱ��
//	    MoveBuffer(voldata->seg.stime.YMDhms, timecmp, sizeof(timecmp));
//	    StepDays(timecmp, -1);
//	    timelen = 2;
//	    if(CmpBuffer(timecmp, time, timelen))
//	    {
//	        return SYS_ERR_FT;
//	    }
//	    ES_OK();
//	}
//	
//	/************************************************************************
//	 * @function: gdw_CrackMonthStat
//	 * @����: �����ͳ������
//	 * @����: 
//	 * @����: 
//	 * @˵��: 
//	 * @����: yzy (2014/2/20)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	void gdw_CrackMonthStat(void)
//	{
//	    TDataBlock db;
//	    ES_FILE_DB_Open(DB_MONSTAT, &db, TDB_MODE_RW);
//	    ES_FILE_DB_Erase(&db, 0, 2);
//	    ES_FILE_DB_Close(&db);
//	}

/************************************************************************
**��������: void Inp_DataClear(void)
**�� ��: �������庯��
**
**�� ��: 
**�� ��: 
**
**�� ע: 
**--------------------------------------------------------
**�� ��: yzy
**�� ��: 2009-09-09
*************************************************************************/
void Inp_DataClear(void)
{
//	    STAT_Reset();
//	    STAT_TMDayCrack();
//	    STAT_TMMonCrack();
    EVT_Reset();
//	    gdw_CrackRecData();
//	    gdw_CrackDayData();
//	    gdw_CrackMonthData();
//	    gdw_CrackHourData();
//	    gdw_CrackDayStat();
//	    gdw_CrackMonthStat();
    gdw_CrackCureData();
}







