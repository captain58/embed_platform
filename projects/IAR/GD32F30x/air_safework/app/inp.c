/****************************************Copyright (c)**************************************************
**                                  ____科技有限公司
**                                          开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: inp.c
**创   建   人: yzy
**最后修改日期: 2019-06-18
**描        述: 内部进程模块主程序
**注        意: 这里inp不是独立的进程,地位和stat进程相当
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2019-06-18
** 描　述: 原始版本
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
//	 * @描述: 向一个虚拟条目中写入数据(FLASH当内存用)
//	 * 
//	 * @参数: 
//	 * @param: data 欲写入的内容
//	 * @param: tno 测量点号
//	 * @param: type 数据类型 0:实时事件数据,1:实时需量数据,3:电压采样
//	 * 
//	 * @返回: 
//	 * @return: uint8  
//	 * @说明: 
//	 * @作者: yzy (2014/2/20)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
//	 ************************************************************************/
//	uint8 SA_REALDATA_Write(uint8* data, uint16 tno, uint8 type)
//	{
//	    TDevFlsOP op;                       //定义一个Flash操作结构
//	    uint16 itmno;                       //存储条目号
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
//	    op.addr = itmno;                    //存放在第几个4K扇区.
//	//    op.len = (uint32)&gs_flsRealDataConst; //长度(测量点实时数据虚拟存储信息结构体)
//	    op.obj = data;                      //缓存
//	    op.type = DEV_FLS_TYPE_VFRAM;       //类型
//	    op.rw = DEV_FLS_RW_W;               //读写方式
//	
//	    ES_FRAM_Open();
//	    result = DevFls_ProcMSG(&op);
//	    ES_FRAM_Close();
//	    
//	    return result;
//	}



/************************************************************************
 * @function: SA_REALDATA_Read
 * @描述: 从一个虚拟内存条目中读取数据(FLASH当内存用)
 * 
 * @参数: 
 * @param: data 存放数据的缓存
 * @param: tno 测量点号
 * @param: type 数据类型 0:实时事件数据,1:实时需量数据,3:电压采样
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2014/2/8)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	uint8 SA_REALDATA_Read(uint8* data, uint16 tno, uint8 type)
//	{
//	    TDevFlsOP op;                       //定义一个Flash操作结构
//	    uint16 num;                         //数据块号
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
//	    op.addr = num;                      //地址
//	//    op.len = (uint32)&gs_flsRealDataConst; //长度(测量点实时数据虚拟存储信息结构体)
//	    op.obj = data;                      //缓存
//	    op.type = DEV_FLS_TYPE_VFRAM;       //类型
//	    op.rw = DEV_FLS_RW_R;               //读写方式
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
 * @描述: 根据一个有效性数组去建立另一个有效性数组
 * 
 * @参数: 
 * @param: s 源数组
 * @param: d 目标数组
 * @param: s_map 对应结构体
 * @param: s_len 对应结构体项数
 * @返回: 
 * @说明: 
 * @作者: yzy (2011-9-21)
 *-----------------------------------------------------------------------
 * @修改人: 
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
 * 抄表日记录时标指针
 ************************************************************************/
typedef struct
{
    uint8 bp;                               //记录块指针,范围0<X<SIZE_RECDAY_DAYS
    uint8 Ymd[SIZE_RECDAY_DAYS][3];         //各个记录块的时间
}S_RECDAY_TL;




/************************************************************************
 * 抄表日段信息集合
 ************************************************************************/


const S_DATASEGINFO gss_recSegInfo[] = 
{
    {0, LEN_RECDAY_SEG0},
    {2, LEN_RECDAY_SEG2},
};


/************************************************************************
 * @function: _RecDataAddr
 * @描述: 抄表日数据起始地址
 * 
 * @参数: 
 * @param: bpidx 存放到第几个记录
 * @param: tno 测量点号
 * @param: segno 段号
 * @param: outinfo 段结构信息体,输出.(为__NULL时不输出)
 * 
 * @返回: 
 * @return: uint32  数据块中的起始地址,0xffffffff为非法.
 * @说明: 
 * @作者: yzy (2014/2/14)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	uint32 _RecDataAddr(uint8 bpidx, uint8 tno, uint8 segno, const S_DATASEGINFO** outinfo)
//	{
//	    const S_DATASEGINFO* _s;
//	    uint32 addr;                            //返回值
//	    uint32 totallen;                        //记录总长度
//	    uint32 segoffset;                       //段位于当前记录中的偏移
//	    uint8 isGot;                            //是否找到指定的段
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
//	    addr = bpidx * (totallen * CORE_TST_NO);//记录块(对应时间)起始地址
//	    addr += tno * totallen;                 //测量点(对应记录块内)起始地址
//	    addr += segoffset;                      //段(对应测量点)起始地址
//	    return addr;
//	
//	};



/************************************************************************ 
 *抄表日冻结.段有效位对应关系
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
 * @描述: 抄表日数据段的准备
 * 
 * @参数: 
 * @param: tno 测量点号
 * @param: segno 段号
 * @param: opbuffer 存放到的地址.(至少能放得下输出的数据)
 * 
 * @返回: 
 * @return: uint16 段内容长度
 * @说明: 本来存放的数据量就不多,实现起来繁琐点也就算了
 * @作者: yzy (2014/2/15)
 *-----------------------------------------------------------------------
 * @修改人: 
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
//	        //以下代码占用较多堆栈
//	        uint8 tempbuf[SA_FLASH_REALDATA_ITMBABY_BYTE];
//	        S_TstDmdData* dmdata =(S_TstDmdData*)tempbuf; 
//	        if(SA_REALDATA_Read(tempbuf, tno, 1) != SA_ERR_OK)
//	        {
//	             return 0;
//	        }
//	        //以上代码占用较多堆栈
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
 * @描述: 冻结某端口下的测量点抄表日数据
 * 
 * @参数: 
 * @param: port 端口索引
 * @param: segno 段号
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2014/2/15)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	uint8 gdw_SaveRecData(uint8 port, uint8 segno)
//	{
//	    S_RECDAY_TL tlabel;
//	    uint32 addr;
//	    uint16 dlen;
//	    uint8 opbuffer[MAX_LEN_RECDAY_SEG];
//	    const S_DATASEGINFO* segstr;
//	                                            //抄表日数据存储指针结构体
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
 * @描述: 获取某个测量点的抄表日数据
 * 
 * @参数: 
 * @param: time 年月日
 * @param: tno 测量点号
 * @param: segno 端口号
 * @param: outbuffer 输出缓存
 * 
 * @返回: 
 * @return: uint8 成功/失败
 * @说明: 
 * @作者: yzy (2014/2/15)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	uint8 gdw_ReadRecData(uint8* time, uint8 tno, uint8 segno, uint8* outbuffer)
//	{
//	    S_RECDAY_TL tlabel;
//	    uint32  addr;
//	    uint8*  timept;                         //冻结时标
//	    uint8   timelen;                        //判断冻结时标的长度
//	    uint8   uc_i;
//	    const S_DATASEGINFO* segstr;
//	
//	    ES_VAR_CHECK(tno > CORE_TST_NO);
//	    timept = ((S_DATA_SEG_TEMPLET*)outbuffer)->seg.stime.YMDhms;
//	    timelen = 3;
//	                                            //获取存储指针列表
//	    //ES_FRAM_Open();
//	    //Fram_SafeRead((uint8*)&tlabel, FM_RECPT, sizeof(tlabel));  //ZJC
//	    //ES_FRAM_Close();
//	                                            //ADJUST LOCATION POINT
//	    if(tlabel.bp >= SIZE_RECDAY_DAYS)
//	    {
//	        tlabel.bp = 0;
//	    }
//	                                            //寻找对应时标的数据
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
//	                                            //获取段的数据块起始地址,以及段(长度)信息
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
 * @描述: 清空表日数据
 * 
 * @参数: 
 * @param:  
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/2/15)
 *-----------------------------------------------------------------------
 * @修改人: 
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
 * 日冻结段信息集合
 ************************************************************************/
//	const S_DATASEGINFO gss_daySegInfo[] = 
//	{
//	    {0, LEN_DAY_SEG0},
//	    {2, LEN_DAY_SEG2},
//	};



/************************************************************************
 * @function: _DayDataAddr
 * @描述: 日冻结数据起始地址
 * 
 * @参数: 
 * @param: ymd 输入,时间
 * @param: tno 输入,测量点号
 * @param: segno 输入,段号
 * @param: outinfo 输出,段信息
 * @param: outbidx 输出,位图索引
 * 
 * @返回: 
 * @return: uint32  
 * @说明: 
 * @作者: yzy (2014/2/16)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	uint32 _DayDataAddr(uint8* ymd, uint8 tno, uint8 segno, const S_DATASEGINFO** outinfo, uint16* outbidx)
//	{
//	    const S_DATASEGINFO* _s;
//	    uint32 addr;                            //返回值
//	    uint32 totallen;                        //记录总长度
//	    uint32 segoffset;                       //段位于当前记录中的偏移
//	    uint8 isGot;                            //是否找到指定的段
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
//	                                            //有效性位图
//	    addr = SA_4096B_SORTED(SIZE_DAY_DAYS * CORE_TST_NO);
//	    if(outbidx != __NULL)
//	    {
//	        *outbidx = tno * SIZE_DAY_DAYS + day;
//	    }
//	                                            //数据分布
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
//	 *日冻结.段有效位对应关系
//	************************************************************************/
//	const S_EFT_MAP gss_EftMapRealToDaySeg0[] = 
//	{
//	    {EFT_WP, EFT_DAY_WP},
//	    {EFT_WN, EFT_DAY_WN},
//	};
//	
//	/************************************************************************ 
//	  **第2段
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
//	 * @描述: 日冻结数据段的准备
//	 * 
//	 * @参数: 
//	 * @param: tno 测量点号
//	 * @param: segno 段号
//	 * @param: opbuffer 存放到的地址.(至少能放得下输出的数据)
//	 * 
//	 * @返回: 
//	 * @return: uint16 段内容长度
//	 * @说明: 本来存放的数据量就不多,实现起来繁琐点也就算了
//	 * @作者: yzy (2014/2/15)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
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
//	        //以下代码占用较多堆栈
//	        uint8 tempbuf[SA_FLASH_REALDATA_ITMBABY_BYTE];
//	        S_TstDmdData* dmdata =(S_TstDmdData*)tempbuf; 
//	        if(SA_REALDATA_Read(tempbuf, tno, 1) != SA_ERR_OK)
//	        {
//	             return 0;
//	        }
//	        //以上代码占用较多堆栈
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
 * @描述: 将某个测量点的实时数据冻结为上日日数据
 * 
 * @参数: 
 * @param: tno 测量点号
 * @param: segno 段号
 * 
 * @返回: 
 * @return: uint8  成功/失败
 * @说明: 
 * @作者: yzy (2014/2/16)
 *-----------------------------------------------------------------------
 * @修改人: 
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
//	        STR_ClrFlashBit(DB_DAY, 0, bidx, 0xffff);//0有效,1无效
//	    }
//	    
//	    ES_OK();
//	}



/************************************************************************
 * @function: gdw_SaveDayData
 * @描述: 冻结某端口下的测量点日数据
 * 
 * @参数: 
 * @param: port 
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2014/2/16)
 *-----------------------------------------------------------------------
 * @修改人: 
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
//	        if(gss_CDATATst[uc_i].pf != TST_PFID_DLT07)     //07规约日冻结电量不采用实时数据保存
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
 * @描述: 获取某个测量点的日数据
 * 
 * @参数: 
 * @param: time 年月日
 * @param: tno 测量点号
 * @param: segno 段号
 * @param: outbuffer 
 * 
 * @返回: 
 * @return: uint8  成功/失败
 * @说明: 
 * @作者: yzy (2014/2/16)
 *-----------------------------------------------------------------------
 * @修改人: 
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
//	                                            //获取段的数据块起始地址,以及段(长度)信息
//	    addr = _DayDataAddr(time, tno, segno, &segstr, &bidx);
//	    if(addr == 0xffffffff)
//	    {
//	        return SYS_ERR_FT;
//	    }
//	                                            //有效位图判断.0有效,1无效
//	    if(STR_IsFlashBitClr(DB_DAY, 0, bidx) == false)
//	    {
//	        ClearBuffer(outbuffer, segstr->seglen);
//	        return SYS_ERR_FT;
//	    }
//	                                            //读取数据
//	    if(false == ReadFlashWithCRC(DB_DAY, outbuffer, segstr->seglen, addr))
//	    {
//	        ClearBuffer(outbuffer, segstr->seglen);
//	        return SYS_ERR_FT;
//	    }
//	                                            //使用实际冻结时间的上一天,与要求时标比较
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
 * @描述: 清空日冻结数据
 * 
 * @参数: 
 * @param: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2011-9-21)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	void gdw_CrackDayData(void)
//	{
//	    TDataBlock db;
//	    ES_FILE_DB_Open(DB_DAY, &db, TDB_MODE_RW);
//	    ES_FILE_DB_Erase(&db, 0, 2);
//	    ES_FILE_DB_Close(&db);
//	}









/************************************************************************
 * 月冻结段信息集合
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
//	 * @描述: 月冻结数据起始地址
//	 * 
//	 * @参数: 
//	 * @param: ym  输入,时间
//	 * @param: tno 输入,测量点号
//	 * @param: segno 输入,段号
//	 * @param: outinfo 输出,段信息
//	 * @param: outbidx 输出,位图索引
//	 * 
//	 * @返回: 
//	 * @return: uint32  
//	 * @说明: 
//	 * @作者: yzy (2014/2/20)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
//	 ************************************************************************/
//	uint32 _MonthDataAddr(uint8* ym, uint8 tno, uint8 segno, const S_DATASEGINFO** outinfo, uint16* outbidx)
//	{
//	    const S_DATASEGINFO* _s;
//	    uint32 addr;                            //返回值
//	    uint32 totallen;                        //记录总长度
//	    uint32 segoffset;                       //段位于当前记录中的偏移
//	    uint8 isGot;                            //是否找到指定的段
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
//	                                            //有效性位图
//	    addr = SA_4096B_SORTED(SIZE_MON_MONS * CORE_TST_NO);
//	    if(outbidx != __NULL)
//	    {
//	        *outbidx = tno * SIZE_MON_MONS + month;
//	    }
//	                                            //数据分布
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
//	 * @描述: 月冻结数据段的准备
//	 * 
//	 * @参数: 
//	 * @param: tno 测量点号
//	 * @param: segno 段号
//	 * @param: opbuffer 存放到的地址.(至少能放得下输出的数据)
//	 * 
//	 * @返回: 
//	 * @return: uint16 段内容长度
//	 * @说明: 本来存放的数据量就不多,实现起来繁琐点也就算了
//	 * @作者: yzy (2014/2/15)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
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
//	 * @描述: 将某个测量点的实时数据冻结为上月月数据
//	 * 
//	 * @参数: 
//	 * @param: tno 测量点号
//	 * @param: segno 段号
//	 * 
//	 * @返回: 
//	 * @return: uint8  成功/失败
//	 * @说明: 
//	 * @作者: yzy (2014/2/16)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
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
//	        STR_ClrFlashBit(DB_MONTH, 0, bidx, 0xffff);//0有效,1无效
//	    }
//	    
//	    ES_OK();
//	}
//	
//	
//	/************************************************************************
//	 * @function: gdw_SaveMonthData
//	 * @描述: 冻结某端口下的测量点上月数据
//	 * 
//	 * @参数: 
//	 * @param: port 
//	 * 
//	 * @返回: 
//	 * @return: uint8  
//	 * @说明: 
//	 * @作者: yzy (2014/2/20)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
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
//	 * @描述: 获取某个测量点的月数据
//	 * 
//	 * @参数: 
//	 * @param: time 年月
//	 * @param: tno 测量点号
//	 * @param: segno 段号
//	 * @param: outbuffer 输出缓存
//	 * 
//	 * @返回: 
//	 * @return: uint8  
//	 * @说明: 
//	 * @作者: yzy (2014/2/20)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
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
//	                                            //获取段的数据块起始地址,以及段(长度)信息
//	    addr = _MonthDataAddr(time, tno, segno, &segstr, &bidx);
//	    if(addr == 0xffffffff)
//	    {
//	        return SYS_ERR_FT;
//	    }
//	                                            //有效位图判断.0有效,1无效
//	    if(STR_IsFlashBitClr(DB_MONTH, 0, bidx) == false)
//	    {
//	        ClearBuffer(outbuffer, segstr->seglen);
//	        return SYS_ERR_FT;
//	    }
//	                                            //读取数据
//	    if(false == ReadFlashWithCRC(DB_MONTH, outbuffer, segstr->seglen, addr))
//	    {
//	        ClearBuffer(outbuffer, segstr->seglen);
//	        return SYS_ERR_FT;
//	    }
//	                                            //使用实际冻结时间的上一天,与要求时标比较
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
//	 * @描述: 清空月冻结数据
//	 * 
//	 * @参数: 
//	 * @param: 
//	 * @返回: 
//	 * @说明: 
//	 * @作者: yzy (2011-9-21)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
//	 ************************************************************************/
//	void gdw_CrackMonthData(void)
//	{
//	    TDataBlock db;
//	    ES_FILE_DB_Open(DB_MONTH, &db, TDB_MODE_RW);
//	    ES_FILE_DB_Erase(&db, 0, 2);
//	    ES_FILE_DB_Close(&db);
//	}








/************************************************************************
 * 曲线冻结段信息集合
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
//	 * @描述: 曲线冻结数据起始地址
//	 * 
//	 * @参数: 
//	 * @param: ymdh 输入,年月日时
//	 * @param: tno 输入,测量点号
//	 * @param: segno 输入,段号
//	 * @param: outinfo 输出,段信息
//	 * @param: outbidx 输出,位图索引
//	 * 
//	 * @返回: 
//	 * @return: uint32  
//	 * @说明: 
//	 * @作者: yzy (2014/2/16)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
//	 ************************************************************************/
//	uint32 _HourDataAddr(uint8* ymdh, uint8 tno, uint8 segno, const S_DATASEGINFO** outinfo, uint16* outbidx)
//	{
//	    const S_DATASEGINFO* _s;
//	    uint32 addr;                            //返回值
//	    uint32 totallen;                        //记录总长度
//	    uint32 segoffset;                       //段位于当前记录中的偏移
//	    uint8 isGot;                            //是否找到指定的段
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
//	                                            //有效性位图
//	    addr = SA_4096B_SORTED(SIZE_HOUR_HOURS * CORE_TST_NO) / 8;
//	    if(outbidx != __NULL)
//	    {
//	        *outbidx = tno * SIZE_HOUR_HOURS + hour;
//	    }
//	                                            //数据分布
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
//	 *曲线冻结.段有效位对应关系
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
//	 * @描述: 小时冻结数据段的准备
//	 * 
//	 * @参数: 
//	 * @param: tno 测量点号
//	 * @param: segno 段号
//	 * @param: opbuffer 存放到的地址.(至少能放得下输出的数据)
//	 * 
//	 * @返回: 
//	 * @return: uint16 段内容长度
//	 * @说明: 本来存放的数据量就不多,实现起来繁琐点也就算了
//	 * @作者: yzy (2014/2/15)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
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
//	 * @描述: 将某个测量点的实时数据冻结为曲线数据
//	 * 
//	 * @参数: 
//	 * @param: tno 测量点号
//	 * @param: segno 段号
//	 * 
//	 * @返回: 
//	 * @return: uint8  成功/失败
//	 * @说明: 
//	 * @作者: yzy (2014/2/16)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
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
//	        STR_ClrFlashBit(DB_HOUR, 0, bidx, 0xffff);//0有效,1无效
//	    }
//	    
//	    ES_OK();
//	}
//	
//	
//	
//	/************************************************************************
//	 * @function: gdw_SaveHourData
//	 * @描述: 冻结某端口下的测量点小时数据
//	 * 
//	 * @参数: 
//	 * @param: port 
//	 * 
//	 * @返回: 
//	 * @return: uint8  
//	 * @说明: 
//	 * @作者: yzy (2014/2/20)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
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
//	 * @描述: 获取某个测量点的小时数据
//	 * 
//	 * @参数: 
//	 * @param: time 年月日时分
//	 * @param: tno 测量点号
//	 * @param: segno 段号
//	 * @param: outbuffer 
//	 * 
//	 * @返回: 
//	 * @return: uint8  
//	 * @说明: 
//	 * @作者: yzy (2014/2/20)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
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
//	                                            //获取段的数据块起始地址,以及段(长度)信息
//	    addr = _HourDataAddr(time, tno, segno, &segstr, &bidx);
//	    if(addr == 0xffffffff)
//	    {
//	        return SYS_ERR_FT;
//	    }
//	                                            //有效位图判断.0有效,1无效
//	    if(STR_IsFlashBitClr(DB_HOUR, 0, bidx) == false)
//	    {
//	        return SYS_ERR_FT;
//	    }
//	                                            //读取数据
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
//	 * @描述: 清空小时冻结数据
//	 * @参数: 
//	 * @返回: 
//	 * @说明: 
//	 * @作者: yzy (2014/2/20)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
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
 * @描述: 存储曲线数据(间隔15分钟)
 * @参数: 
 * @param: time 年月日时分秒
 * @param: tno 测量点号
 * @param: outbuffer 输出缓存
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: zjc (2014/8/19)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 gdw_SaveCureData(uint8* time, uint8 tno, uint8* outbuffer)
{    
    uint32 addr;
    uint32 min;
    uint8_t ret = 0;
    SYS_VAR_CHECK(tno > CORE_TST_NO);
    
    min = CalcMinuteHex(time[0], time[1], time[2], time[3], time[4]); //总分钟数
    min = min / gs_PstPara.cp_interval;  //得到总点数
    min %= SIZE_CUR_POINTS;  //存储位置点数
    
    addr = SA_4096B_SORTED(SA_8B_SORTED(SIZE_CUR_POINTS * CORE_TST_NO) >> 3);  //跳过位图数据区
    addr =addr + (tno * SIZE_CUR_POINTS * LEN_CUR_SEG0);
    addr =addr + (min * LEN_CUR_SEG0);
                                        //存储操作
    ret = WriteFlashWithCRC(DB_CURE, outbuffer, sizeof(S_CURDATA_SEG0), addr);
    STR_ClrFlashBit(DB_CURE, 0, min, 0xffff);//0有效,1无效
    //SYS_OK();
    return ret;
}

/************************************************************************
 * @function: gdw_ReadCureData
 * @描述: 读取曲线数据
 * 
 * @参数: 
 * @param: time 年月日时分
 * @param: outbuffer 输出缓存
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 暂不带位图,总清不清除
 * @作者: ZJC (2014/8/19)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 gdw_ReadCureData(uint8* time, uint8 tno, uint8* outbuffer)
{
    uint32 addr = 0;
    uint32 min = 0;
    
    SYS_VAR_CHECK(tno > CORE_TST_NO);
                                        //存储地址
    min = CalcMinuteHex(time[0], time[1], time[2], time[3], time[4]); //总分钟数
    min = min / gs_PstPara.cp_interval;  //得到总点数
    min %= SIZE_CUR_POINTS;  //存储位置点数
    
    addr = SA_4096B_SORTED(SA_8B_SORTED(SIZE_CUR_POINTS * CORE_TST_NO) >> 3);  //跳过位图数据区
    addr =addr + (tno * SIZE_CUR_POINTS * LEN_CUR_SEG0);
    addr =addr + (min * LEN_CUR_SEG0);
    
    if(STR_IsFlashBitClr(DB_CURE, 0, min) == false)  //有效位图判断.0有效,1无效
    {
        ClearBuffer(outbuffer, sizeof(S_CURDATA_SEG0)); 
        return SYS_ERR_FT;
    }
                                        //读取数据
    if(SYS_ERR_OK != ReadFlashWithCRC(DB_CURE, outbuffer, sizeof(S_CURDATA_SEG0), addr))
    {
        ClearBuffer(outbuffer, sizeof(S_CURDATA_SEG0)); 
        return SYS_ERR_FT;
    }
    
    if(CmpBuffer(((S_CURDATA_SEG0 *)outbuffer)->seg.stime.YMDhms, time, 5)) //时标比对
    {
        ClearBuffer(outbuffer, sizeof(S_CURDATA_SEG0)); 
        return SYS_ERR_FT;
    }
    SYS_OK();
} 

/************************************************************************
 * @function: gdw_CrackCureData
 * @描述: 清空曲线数据
 * 
 * @参数: 
 * @param: 
 * @返回: 
 * @说明: 
 * @作者: zjc (2014-8-24)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void gdw_CrackCureData(void)
{
    TDataBlock db;
    SYS_FILE_DB_Open(DB_CURE, &db, TDB_MODE_RW);//
    SYS_FILE_DB_Erase(&db, 0, ((SA_8B_SORTED(SIZE_CUR_POINTS * CORE_TST_NO) >> 3)/0x1000)+1);  //清位图
    SYS_FILE_DB_Close(&db);
}


/************************************************************************
 * @function: gdw_SaveQuarData
 * @描述: 存储分平均电压数据(间隔15分钟)
 * @参数: 
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 暂不带位图,总清不清除
 * @作者: yzy (2014/2/20)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	uint8 gdw_SaveQuarData(void)
//	{    
//	    S_QUARDATA_SEG0 quardata;
//	    uint8 time[6];
//	    uint32 addr;
//	    uint32 min;
//	                                        //数据内容
//	    quardata.seg.stime = gs_SysTime;
//	    quardata.seg.vol   = gul_MinVoltage;
//	                                        //存储地址
//	    MoveBuffer(gs_SysTime.YMDhms, time, 5);
//	    min = CalcDayHex(time[0], time[1], time[2]);
//	    min += (min * 96) + (time[3] * 4) + (time[4] / 15);
//	    min %= SIZE_QUAR_POINTS;
//	    addr = min * LEN_QUAR_SEG0;
//	                                        //存储操作
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
//	 * @描述: 读取存储的分平均电压
//	 * 
//	 * @参数: 
//	 * @param: time 年月日时分
//	 * @param: quardata 输出缓存
//	 * 
//	 * @返回: 
//	 * @return: uint8  
//	 * @说明: 暂不带位图,总清不清除
//	 * @作者: yzy (2014/2/20)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
//	 ************************************************************************/
//	uint8 gdw_ReadQuarData(uint8* time, S_QUARDATA_SEG0* quardata)
//	{
//	    uint32 addr;
//	    uint32 min;
//	    uint8* timept;
//	    uint8 timelen;
//	                                        //存储地址
//	    min = CalcDayHex(time[0], time[1], time[2]);
//	    min += (min * 96) + (time[3] * 4) + (time[4] / 15);
//	    min %= SIZE_QUAR_POINTS;
//	    addr = min * LEN_QUAR_SEG0;
//	                                        //读取数据
//	    if(false == ReadFlashWithCRC(DB_QUAR, (uint8*)quardata, sizeof(S_QUARDATA_SEG0), addr))
//	    {
//	        return SYS_ERR_FT;
//	    }
//	                                        //时标比对
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
//	 * @描述: 统计日数据存储地址
//	 * 
//	 * @参数: 
//	 * @param: ymd 输入,日期
//	 * @param: tno 输入,测量点号
//	 * @param: outbidx 输出,位图索引
//	 * 
//	 * @返回: 
//	 * @return: uint32  
//	 * @说明: 
//	 * @作者: yzy (2014/2/20)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
//	 ************************************************************************/
//	uint32 _DayStatAddr(uint8* ymd, uint8 tno, uint16* outbidx)
//	{
//	    uint32 addr;
//	    uint16 day;
//	                                            //计算
//	    day =  CalcDayHex(ymd[0], ymd[1], ymd[2]);
//	    day %= SIZE_DAY_DAYS;
//	                                            //有效性位图
//	    addr = SA_4096B_SORTED(SIZE_DAY_DAYS * CORE_TST_NO);
//	    if(outbidx != __NULL)
//	    {
//	        *outbidx = tno * SIZE_DAY_DAYS + day;
//	    }
//	                                            //数据分布
//	    addr += tno * (SIZE_DAY_DAYS * sizeof(S_CBVOL_SEG0));
//	    addr += day * sizeof(S_CBVOL_SEG0);
//	
//	    return addr;
//	}
//	
//	
//	/************************************************************************
//	 * @function: gdw_SaveTstDayStat
//	 * @描述: 存储测量点电压统计日数据
//	 * @参数: 
//	 * 
//	 * @返回: 
//	 * @return: uint8  
//	 * @说明: 
//	 * @作者: yzy (2014/2/20)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
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
//	                                    //按测量点循环
//	    for(uint8 k = 1; k < CORE_TST_NO; k++)
//	    {
//	        if(gss_CDATATst[k].ef != 0x01)
//	        {
//	            continue;
//	        }
//	                                        //读取统计数据
//	        if(SA_REALDATA_Read(buffer, k, 3) != SA_ERR_OK)
//	        {
//	            continue;
//	        }
//	                                        //获取日统计数据
//	        pstat = &(statblock->daystat);
//	                                        //准备日冻结数据内容
//	        daydata.seg.stime = gs_SysTime;
//	        MoveBuffer((uint8*)pstat, (uint8*)&(daydata.seg.bank1), sizeof(S_CBStat));
//	                                        //保存日冻结数据
//	        addr = _DayStatAddr(gs_SysLastDayTime.YMDhms, k, &bidx);
//	        WriteFlashWithCRC(DB_DAYSTAT, (uint8*)&daydata, sizeof(daydata), addr);
//	        STR_ClrFlashBit(DB_DAYSTAT, 0, bidx, 0xffff);//0有效,1无效
//	                                        //复位日统计数据
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
//	 * @描述: 存储测量点电压统计日数据
//	 * 
//	 * @参数: 
//	 * @param: time 年月日
//	 * @param: pn 测量点号
//	 * @param: voldata 输出缓存
//	 * 
//	 * @返回: 
//	 * @return: uint8  
//	 * @说明: 
//	 * @作者: yzy (2014/2/20)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
//	 ************************************************************************/
//	uint8 gdw_ReadTstDayStat(uint8* time, uint8 tno, S_CBVOL_SEG0* voldata)
//	{
//	    uint32  addr;
//	    uint8   timecmp[3];
//	    uint8   timelen;
//	    uint16  bidx;
//	    
//	    ES_VAR_CHECK(tno > CORE_TST_NO);
//	                                            //冻结地址
//	    addr = _DayStatAddr(time, tno, &bidx);
//	                                            //有效位图判断.0有效,1无效
//	    if(STR_IsFlashBitClr(DB_DAYSTAT, 0, bidx) == false)
//	    {
//	        return SYS_ERR_FT;
//	    }
//	                                            //读取数据
//	    if(false == ReadFlashWithCRC(DB_DAYSTAT, (uint8*)voldata, sizeof(S_CBVOL_SEG0), addr))
//	    {
//	        return SYS_ERR_FT;  
//	    }
//	                                            //比对时标
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
//	 * @描述: 清空日统计数据
//	 * @参数: 
//	 * @返回: 
//	 * @说明: 
//	 * @作者: yzy (2014/2/20)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
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
//	 * @描述: 统计月数据存储地址
//	 * 
//	 * @参数: 
//	 * @param: ymd 
//	 * @param: tno 
//	 * 
//	 * @返回: 
//	 * @return: uint32  
//	 * @说明: 
//	 * @作者: yzy (2014/2/20)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
//	 ************************************************************************/
//	uint32 _MonthStatAddr(uint8* ymd, uint8 tno, uint16* outbidx)
//	{
//	    uint32 addr;
//	    uint16 month;
//	                                            //计算
//	    month = ymd[0] * 12 + ymd[1] - 1;
//	    month %= SIZE_MON_MONS;
//	                                            //有效性位图
//	    addr = SA_4096B_SORTED(SIZE_MON_MONS * CORE_TST_NO);
//	    if(outbidx != __NULL)
//	    {
//	        *outbidx = tno * SIZE_MON_MONS + month;
//	    }
//	                                            //数据分布
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
//	 * @描述: 存储测量点电压统计月数据
//	 * @参数: 
//	 * 
//	 * @返回: 
//	 * @return: uint8  
//	 * @说明: 
//	 * @作者: yzy (2014/2/20)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
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
//	                                    //获取月统计数据
//	        pstat = &(statblock->monstat);
//	                                    //准备月冻结数据内容
//	        mondata.seg.stime = gs_SysTime;
//	        MoveBuffer((uint8*)pstat, (uint8*)&(mondata.seg.bank1), sizeof(S_CBStat));
//	                                    //保存月冻结数据
//	        addr = _MonthStatAddr(gs_SysLastDayTime.YMDhms, k, &bidx);
//	        WriteFlashWithCRC(DB_MONSTAT, (uint8*)&mondata, sizeof(mondata), addr);
//	        STR_ClrFlashBit(DB_MONSTAT, 0, bidx, 0xffff);//0有效,1无效
//	                                    //复位月统计数据
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
//	 * @描述: 读取测量点电压统计月数据
//	 * 
//	 * @参数: 
//	 * @param: time 
//	 * @param: tno 
//	 * @param: voldata 
//	 * 
//	 * @返回: 
//	 * @return: uint8  
//	 * @说明: 
//	 * @作者: yzy (2014/2/20)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
//	 ************************************************************************/
//	uint8 gdw_ReadTstMonthStat(uint8* time, uint8 tno, S_CBVOL_SEG0* voldata)
//	{
//	    uint32  addr;
//	    uint8   timecmp[3];
//	    uint8   timelen;
//	    uint16  bidx;
//	    
//	    ES_VAR_CHECK(tno > CORE_TST_NO);
//	                                            //地址计算
//	    addr = _MonthStatAddr(time, tno, &bidx);
//	                                            //有效位图判断.0有效,1无效
//	    if(STR_IsFlashBitClr(DB_MONSTAT, 0, bidx) == false)
//	    {
//	        return SYS_ERR_FT;
//	    }
//	                                            //读取数据
//	    if(false == ReadFlashWithCRC(DB_MONSTAT, (uint8*)voldata, sizeof(S_CBVOL_SEG0), addr))
//	    {
//	        return SYS_ERR_FT;
//	    }
//	                                            //比对时标
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
//	 * @描述: 清空月统计数据
//	 * @参数: 
//	 * @返回: 
//	 * @说明: 
//	 * @作者: yzy (2014/2/20)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
//	 ************************************************************************/
//	void gdw_CrackMonthStat(void)
//	{
//	    TDataBlock db;
//	    ES_FILE_DB_Open(DB_MONSTAT, &db, TDB_MODE_RW);
//	    ES_FILE_DB_Erase(&db, 0, 2);
//	    ES_FILE_DB_Close(&db);
//	}

/************************************************************************
**函数名称: void Inp_DataClear(void)
**描 述: 数据总清函数
**
**输 入: 
**输 出: 
**
**备 注: 
**--------------------------------------------------------
**作 者: yzy
**日 期: 2009-09-09
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







