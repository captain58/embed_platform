




uint8 FmtBgTimeTemp[6];                                 //秒分时,日月年

/************************************************************************
 * @function: ntrd_FmtC010
 * @描述: 将C010数据YYMMDDWW填入测量点变量
 * 
 * @参数: 
 * @param: buff 数据
 * @param: nt 格式信息结构体
 * @param: tstno 测量点号
 * @param: num 正在处理的项编号
 * @返回: 
 * @说明: 
 * @作者: yzy (2011-11-25)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void ntrd_FmtC010(uint8* buff, NETPSTR* rb, uint8 tstno, uint8 num)
{
    S_TstRealData*  tstdata;    
    //BCD格式WW(DDMMYY)->mh(DMH)
    tstdata = (S_TstRealData*)rb->data;
    MoveBuffer(buff + 1, tstdata->time + 3, 3);
}



/************************************************************************
 * @function: ntrd_FmtC011
 * @描述: 将C011数据hhmmss填入测量点变量
 * 
 * @参数: 
 * @param: buff 数据
 * @param: nt 格式信息结构体
 * @param: tstno 测量点号
 * @param: num 正在处理的项编号
 * @返回: 
 * @说明: 
 * @作者: yzy (2011-11-25)
 *-----------------------------------------------------------------------
 * @修改人: zgb
 ************************************************************************/
void ntrd_FmtC011(uint8* buff, NETPSTR* rb, uint8 tstno, uint8 num)
{
    S_TstRealData*  tstdata;                            //BCD格式ss(mmhh)->(mh)DMH
    tstdata = (S_TstRealData*)rb->data;
    MoveBuffer(buff, tstdata->time, 3);      //转换格式
    //tstdata->time = FMT_UnifyTime(FmtBgTimeTemp, true, false);
}





/************************************************************************
 * @function: ntrd_Fmt07JSTime
 * @描述: 将最近一次校时时间填入测量点变量
 * 
 * @参数: 
 * @param: buff 
 * @param: rb 
 * @param: tstno 
 * @param: num 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/2/7)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void ntrd_Fmt07JSTime(uint8* buff, NETPSTR* rb, uint8 tstno, uint8 num)
{
    NETPRD* nt = rb->tbl + rb->idx;
    MoveBuffer(buff + 4 , rb->data + nt->add ,  12);   
}





/************************************************************************
**函数名称: void FmtDXN(uint8* buff, NETPRD* nt, uint8 tstno, uint8 phase)
**描 述: 将13010001/13020001/13030001等分相断相次数填入测量点变量
**
**输 入: buff,数据 
**       tstno,测量点号
**       phase 相别(0-2)
**输 出: 
**
**备 注: 
**-----------------------------------------------------------------------
**作 者: 
**日 期: 
*************************************************************************/
void FmtDXN(uint8* buff, NETPSTR* rb, uint8 tstno, uint8 phase)
{
    uint32 dxn = 0;
    S_EvtRealData* evtreal;
    evtreal = (S_EvtRealData*)rb->data;
    NETPRD* nt = rb->tbl + rb->idx;                     //更新分相断相次数
    BcdToHex(buff, rb->data + nt->add, 3, 4);
                                    //计算总断相次数
    for(uint8 uc_i = 0; uc_i < 3; uc_i++)
    {
        dxn +=  evtreal->dxn[uc_i + 1];
    }
                                    //645-2007不含总断相统计
    evtreal->dxn[0] = dxn;
}




/************************************************************************
**函数名称: ntrd_FmtDxnA()
**描 述: 将A相断相次数填入测量点变量
**输 入: 
**输 出: 
*************************************************************************/
void ntrd_FmtDxnA(uint8* buff, NETPSTR* rb, uint8 tstno, uint8 num)
{
    FmtDXN(buff, rb ,tstno, 0);
}

/************************************************************************
**函数名称: ntrd_FmtDxnB()
**描 述: 将B相断相次数填入测量点变量
**输 入: 
**输 出: 
*************************************************************************/
void ntrd_FmtDxnB(uint8* buff, NETPSTR* rb, uint8 tstno, uint8 num)
{
    FmtDXN(buff , rb , tstno , 1);
}

/************************************************************************
**函数名称: ntrd_FmtDxnC()
**描 述: 将C相断相次数填入测量点变量
**输 入: 
**输 出: 
*************************************************************************/
void ntrd_FmtDxnC(uint8* buff, NETPSTR* rb, uint8 tstno, uint8 num)
{
    FmtDXN(buff, rb , tstno , 2);
}
//----------------------------------------------------------------------




/******************************************************************************
**组网端口实时数据(部规1997)
******************************************************************************/
const NETPRD gss_NetpBgTstData[] = 
{
//***********************
    {                                                   //+W电能示值集抄
        0x0000901F,
        NTRD_PATR_MUST | NTRD_PATR_SKIP,
        NTRD_DATR_THEX, 
        ADDR_WP,
        //zhtp, zhid, zhnum,
        NTRD_ZHTP_MUFL, 0xFF, 5,
        //sbit, dgt, len, _r, func
        EFT_WP, 0, 4, DTP_WP, 0,
    },
    {                                                   //+W总
        0x00009010,
        NTRD_PATR_MUST,
        NTRD_DATR_THEX, 
        ADDR_WP,
        //zhtp, zhid, zhnum,
        NTRD_ZHTP_MUFL, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_WP, 0, 4, DTP_WP, 0,
    },
    {                                                   //+W_f1
        0x00009011,
        0,
        NTRD_DATR_THEX,
        ADDR_WP + 4,
        //zhtp, zhid, zhnum,
        NTRD_ZHTP_MUFL, 1, 1,
        //sbit, dgt, len, _r, func
        EFT_WP + 1, 0, 4, DTP_WP, 0,
    },
    {                                                   //+W_f2
        0x00009012,
        0,
        NTRD_DATR_THEX,
        ADDR_WP + 8,
        //zhtp, zhid, zhnum,
        NTRD_ZHTP_MUFL, 2, 1,
        //sbit, dgt, len, _r, func
        EFT_WP + 2, 0, 4, DTP_WP, 0,
    },
    {                                                   //+W_f3
        0x00009013,
        0,
        NTRD_DATR_THEX, 
        ADDR_WP + 12,
        //zhtp, zhid, zhnum,
        NTRD_ZHTP_MUFL, 3, 1,
        //sbit, dgt, len, _r, func
        EFT_WP + 3, 0, 4, DTP_WP, 0,
    },
    {                                                   //+W_f4
        0x00009014,
        0,
        NTRD_DATR_THEX, 
        ADDR_WP + 16,
        //zhtp, zhid, zhnum,
        NTRD_ZHTP_MUFL, 4, 1,
        //sbit, dgt, len, _r, func
        EFT_WP + 4, 0, 4, DTP_WP, 0,
    },
//***********************
    {                                                   //-W电能示值集抄
        0x902F,
        NTRD_PATR_SKIP,
        NTRD_DATR_THEX, 
        ADDR_WN,
        //zhtp, zhid, zhnum,
        NTRD_ZHTP_MUFL, 0xFF, 5,
        //sbit, dgt, len, _r, func
        EFT_WN, 0, 4, DTP_WN, 0,
    },
    {                                                   //-W总
        0x00009020,
        0,
        NTRD_DATR_THEX, 
        ADDR_WN,
        //zhtp, zhid, zhnum,
        NTRD_ZHTP_MUFL, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_WN, 0, 4, DTP_WN, 0,
    },
    {                                                   //-W_f1
        0x00009021,
        0,
        NTRD_DATR_THEX, 
        ADDR_WN + 4,
        //zhtp, zhid, zhnum,
        NTRD_ZHTP_MUFL, 1, 1,
        //sbit, dgt, len, _r, func
        EFT_WN + 1, 0, 4, DTP_WN, 0,
    },
    {                                                   //-W_f2
        0x00009022,
        0,
        NTRD_DATR_THEX, 
        ADDR_WN + 8,
        //zhtp, zhid, zhnum,
        NTRD_ZHTP_MUFL, 2, 1,
        //sbit, dgt, len, _r, func
        EFT_WN + 2, 0, 4, DTP_WN, 0,
    },
    {                                                   //-W_f3
        0x00009023,
        0,
        NTRD_DATR_THEX, 
        ADDR_WN + 12,
        //zhtp, zhid, zhnum,
        NTRD_ZHTP_MUFL, 3, 1,
        //sbit, dgt, len, _r, func
        EFT_WN + 3, 0, 4, DTP_WN, 0,
    },
    {                                                   //-W_f4
        0x00009024,
        0,
        NTRD_DATR_THEX, 
        ADDR_WN + 16,
        //zhtp, zhid, zhnum,
        NTRD_ZHTP_MUFL, 4, 1,
        //sbit, dgt, len, _r, func
        EFT_WN + 4, 0, 4, DTP_WN, 0,
    },
//***********************
    {
        0xB61F,                                         //三相电压集抄
        NTRD_PATR_SKIP,
        NTRD_DATR_THEX | NTRD_DATR_PMUL,
        ADDR_U,
        //zhtp, zhid, zhnum,
        0, 0xFF, 3,
        //sbit, dgt, len, _r, func
        EFT_U, 2, 2, DTP_U, 0,
    },
    {
        0xB611,                                         //A相电压
        0,
        NTRD_DATR_THEX | NTRD_DATR_PMUL,
        ADDR_U,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_U, 2, 2, DTP_U, 0,
    },
    {
        0xB612,                                         //B相电压
        0,
        NTRD_DATR_THEX | NTRD_DATR_PMUL,
        ADDR_U + 2,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_U + 1, 2, 2, DTP_U, 0,
    },
    {
        0xB613,                                         //C相电压
        0,
        NTRD_DATR_THEX | NTRD_DATR_PMUL,
        ADDR_U + 4,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_U + 2, 2, 2, DTP_U, 0,
    },
//***********************
    {
        0xB62F,                                         //三相电流集抄
        NTRD_PATR_SKIP,
        NTRD_DATR_THEX | NTRD_DATR_PMUL | NTRD_DATR_BEXT,
        ADDR_I,
        //zhtp, zhid, zhnum,
        0, 0xFF, 3,
        //sbit, dgt, len, _r, func
        EFT_I, 1, 2, DTP_I, 0
    },
    {
        0xB621,                                         //A相电流
        0,
        NTRD_DATR_THEX | NTRD_DATR_PMUL | NTRD_DATR_BEXT,
        ADDR_I,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_I, 1, 2, DTP_I, 0
    },
    {
        0xB622,                                         //B相电流
        0,
        NTRD_DATR_THEX | NTRD_DATR_PMUL | NTRD_DATR_BEXT,
        ADDR_I + 4,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_I + 1, 1, 2, DTP_I, 0
    },
    {
        0xB623,                                         //C相电流
        0,
        NTRD_DATR_THEX | NTRD_DATR_PMUL | NTRD_DATR_BEXT,
        ADDR_I + 8,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_I + 2, 1, 2, DTP_I, 0
    },
//***********************
    {                                                   //日月年 C010(先C010)
        0xC010,
        0,
        NTRD_DATR_SPEC,
        ADDR_TIME,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_TIME, 0, 4, DTP_TIME, (void*)ntrd_FmtC010,
    },
    {                                                   //分时 C011(后C011)
        0xC011,
        0,
        NTRD_DATR_SPEC,
        ADDR_TIME,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_TIME, 0, 3, DTP_TIME, (void*)ntrd_FmtC011,
    },
    {                                                   //总功率
        0xB630,
        0,
        NTRD_DATR_THEX | NTRD_DATR_BEXT,
        ADDR_P,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_P, 0, 3, DTP_P, 0,
    },


};

#define LEN_BGTSTDATA (sizeof(gss_NetpBgTstData) / sizeof(NETPRD))



/******************************************************************************
**组网端口实时数据(部规2007)
******************************************************************************/
/************************************************************************ 
**645-2007组网抄读标识, 
**标识次序:DI1(费率数)DI0(上N结算日)DI3(大类)DI2(小类)
*************************************************************************/ 
const NETPRD gss_NetpXgTstData[] = 
{
//***********************
    {                                                   //+W电能示值集抄
        0x0001FF00,
        NTRD_PATR_MUST | NTRD_PATR_SKIP,
        NTRD_DATR_THEX, 
        ADDR_WP,
        //zhtp, zhid, zhnum,
        NTRD_ZHTP_MUFL, 0xFF, 5,
        //sbit, dgt, len, _r, func
        EFT_WP, 0, 4, DTP_WP, 0,
    },
    {                                                   //+W总
        0x00010000,
        NTRD_PATR_MUST,
        NTRD_DATR_THEX, 
        ADDR_WP,
        //zhtp, zhid, zhnum,
        NTRD_ZHTP_MUFL, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_WP, 0, 4, DTP_WP, 0,
    },
    {                                                   //+W_f1
        0x00010100,
        0,
        NTRD_DATR_THEX,
        ADDR_WP + 4,
        //zhtp, zhid, zhnum,
        NTRD_ZHTP_MUFL, 1, 1,
        //sbit, dgt, len, _r, func
        EFT_WP + 1, 0, 4, DTP_WP, 0,
    },
    {                                                   //+W_f2
        0x00010200,
        0,
        NTRD_DATR_THEX, 
        ADDR_WP + 8,
        //zhtp, zhid, zhnum,
        NTRD_ZHTP_MUFL, 2, 1,
        //sbit, dgt, len, _r, func
        EFT_WP + 2, 0, 4, DTP_WP, 0,
    },
    {                                                   //+W_f3
        0x00010300,
        0,
        NTRD_DATR_THEX,
        ADDR_WP + 12,
        //zhtp, zhid, zhnum,
        NTRD_ZHTP_MUFL, 3, 1,
        //sbit, dgt, len, _r, func
        EFT_WP + 3, 0, 4, DTP_WP, 0,
    },
    {                                                   //+W_f4
        0x00010400,
        0,
        NTRD_DATR_THEX,
        ADDR_WP + 16,
        //zhtp, zhid, zhnum,
        NTRD_ZHTP_MUFL, 4, 1,
        //sbit, dgt, len, _r, func
        EFT_WP + 4, 0, 4, DTP_WP, 0,
    },
//***********************
    {                                                   //-W电能示值集抄
        0x0002FF00,
        NTRD_PATR_SKIP,
        NTRD_DATR_THEX, 
        ADDR_WN,
        //zhtp, zhid, zhnum,
        NTRD_ZHTP_MUFL, 0xFF, 5,
        //sbit, dgt, len, _r, func
        EFT_WN, 0, 4, DTP_WN, 0,
    },
    {                                                   //-W总
        0x00020000,
        0,
        NTRD_DATR_THEX, 
        ADDR_WN,
        //zhtp, zhid, zhnum,
        NTRD_ZHTP_MUFL, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_WN, 0, 4, DTP_WN, 0,
    },
    {                                                   //-W_f1
        0x00020100,
        0,
        NTRD_DATR_THEX, 
        ADDR_WN + 4,
        //zhtp, zhid, zhnum,
        NTRD_ZHTP_MUFL, 1, 1,
        //sbit, dgt, len, _r, func
        EFT_WN + 1, 0, 4, DTP_WN, 0,
    },
    {                                                   //-W_f2
        0x00020200,
        0,
        NTRD_DATR_THEX,
        ADDR_WN + 8,
        //zhtp, zhid, zhnum,
        NTRD_ZHTP_MUFL, 2, 1,
        //sbit, dgt, len, _r, func
        EFT_WN + 2, 0, 4, DTP_WN, 0,
    },
    {                                                   //-W_f3
        0x00020300,
        0,
        NTRD_DATR_THEX,
        ADDR_WN + 12,
        //zhtp, zhid, zhnum,
        NTRD_ZHTP_MUFL, 3, 1,
        //sbit, dgt, len, _r, func
        EFT_WN + 3, 0, 4, DTP_WN, 0,
    },
    {                                                   //-W_f4
        0x00020400,
        0,
        NTRD_DATR_THEX, 
        ADDR_WN + 16,
        //zhtp, zhid, zhnum,
        NTRD_ZHTP_MUFL, 4, 1,
        //sbit, dgt, len, _r, func
        EFT_WN + 4, 0, 4, DTP_WN, 0,
    },
//***********************
    {                                                   //三相电压集抄
        0x0201FF00,
        NTRD_PATR_SKIP,
        NTRD_DATR_THEX | NTRD_DATR_PMUL,
        ADDR_U,
        //zhtp, zhid, zhnum,
        0, 0xFF, 3,
        //sbit, dgt, len, _r, func
        EFT_U, 1, 2, DTP_U, 0,
    },
    {                                                   //A相电压
        0x02010100,
        0,
        NTRD_DATR_THEX | NTRD_DATR_PMUL,
        ADDR_U,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_U, 1, 2, DTP_U, 0,
    },
    {                                                   //B相电压
        0x02010200,
        0,
        NTRD_DATR_THEX | NTRD_DATR_PMUL,
        ADDR_U + 2,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_U + 1, 1, 2, DTP_U, 0,
    },
    {                                                   //C相电压
        0x02010300,
        0,
        NTRD_DATR_THEX | NTRD_DATR_PMUL,
        ADDR_U + 3,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_U + 2, 1, 2, DTP_U, 0,
    },

//***********************
    {                                                   //三相电流集抄
        0x0202FF00,
        NTRD_PATR_SKIP,
        NTRD_DATR_THEX | NTRD_DATR_BEXT | NTRD_DATR_INSG,
        ADDR_I,
        //zhtp, zhid, zhnum,
        0, 0xFF, 3,
        //sbit, dgt, len, _r, func
        EFT_I, 1, 3, DTP_I, 0,
    },
    {                                                   //A相电流
        0x02020100,
        0,
        NTRD_DATR_THEX | NTRD_DATR_BEXT | NTRD_DATR_INSG,
        ADDR_I,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_I, 1, 3, DTP_I, 0,
    },
    {                                                   //B相电流
        0x02020200,
        0,
        NTRD_DATR_THEX | NTRD_DATR_BEXT | NTRD_DATR_INSG,
        ADDR_I + 4,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_I + 1, 1, 3, DTP_I, 0,
    },
    {                                                   //C相电流
        0x02020300,
        0,
        NTRD_DATR_THEX | NTRD_DATR_BEXT | NTRD_DATR_INSG,
        ADDR_I + 8,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_I + 2, 1, 3, DTP_I, 0,
    },
//***********************
    {                                                   //日月年(WWDDMMYY,先C010)
        0x04000101,
        0,
        NTRD_DATR_SPEC,
        ADDR_TIME,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_TIME, 0, 4, DTP_TIME, (void*)ntrd_FmtC010,
    },
    {                                                   //分时(ssmmhh,后C011)
        0x04000102,
        0,
        NTRD_DATR_SPEC,
        ADDR_TIME,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_TIME, 0, 3, DTP_TIME, (void*)ntrd_FmtC011,
    },
    {                                                   //总
        0x02030000,
        0,
        NTRD_DATR_THEX | NTRD_DATR_BEXT | NTRD_DATR_INSG | NTRD_DATR_OUSG,
        ADDR_P,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_P, 0, 3, DTP_P, 0,
    },

};

#define LEN_XGTSTDATA (sizeof(gss_NetpXgTstData) / sizeof(NETPRD))


/************************************************************************
**函数名称: ntrd_Fmt97DemandTime()
**描 述: 将97表需量时间填入缓存
**输 入: 
**输 出: 
*************************************************************************/
void ntrd_Fmt97DemandTime(uint8* buff, NETPSTR* rb, uint8 tstno, uint8 num)
{
    NETPRD* nt = rb->tbl + rb->idx;
    uint8* paddr = rb->data  + nt->add;
    uint8 mHDMY[5];

    for(uint8 uc_i = 0; uc_i < 5; uc_i++)
    {
        MoveBuffer(buff + uc_i * 4, mHDMY, 4);
        mHDMY[4] = 0;
        LongToByte(paddr + uc_i * 4, FMT_UnifyTime(mHDMY, true, false));
    }

}

/************************************************************************
**函数名称: ntrd_Fmt07DemandData()
**描 述: 将07表需量数据及时间填入缓存
**输 入: 
**输 出: 
*************************************************************************/
void ntrd_Fmt07DemandData(uint8* buff, NETPSTR* rb, uint8 tstno, uint8 num)
{
    NETPRD* nt = rb->tbl + rb->idx;
    uint8* paddr = rb->data + nt->add;
    for(uint8 uc_i = 0; uc_i < 5; uc_i++)
    {                                       //需量及时间(XX.XXXXYYMMDDhhmm)
        BcdToHex(buff + uc_i * 8, paddr + uc_i * 4, 3, 4);
        LongToByte(paddr + 20 + uc_i * 4, FMT_UnifyTime(buff + uc_i * 8 + 3, true, false));
    }
                                            //需要补充置位
    if(nt->sbit == EFT_DMD_DPP)
    {
        STR_SetBit(rb->eft, EFT_DMD_DPP);
        STR_SetBit(rb->eft, EFT_DMD_DPPT);
    }
    else if(nt->sbit == EFT_DMD_DNP)
    {
        STR_SetBit(rb->eft, EFT_DMD_DNP);
        STR_SetBit(rb->eft, EFT_DMD_DNPT);
    }
}

/******************************************************************************
**组网端口需量数据(部规1997)
******************************************************************************/
const NETPRD gss_NetpBgTstDemand[] = 
{
    {                                                   //+W需量(总\4费率需量,Hex4.4)
        0xA01F,
        NTRD_PATR_MUST,
        NTRD_DATR_THEX | NTRD_DATR_BEXT,
        ADDR_DMD_DPP,
        //zhtp, zhid, zhnum,
        0, 0, 5,
        //sbit, dgt, len, _r, func
        EFT_DMD_DPP, 0, 3, DTP_DMD_DPP, 0,
    },
    {                                                   //+WT需量时间(MMDDHHmm->AFMT)
        0xB01F,
        0,
        NTRD_DATR_SPEC,
        ADDR_DMD_DPPT,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_DMD_DPPT, 0, 20, DTP_DMD_DPPT, (void*)ntrd_Fmt97DemandTime,
    },
    {                                                   //-W需量(总\4费率需量,Hex4.4)
        0xA02F,
        0,
        NTRD_DATR_THEX | NTRD_DATR_BEXT,
        ADDR_DMD_DNP,
        //zhtp, zhid, zhnum,
        0, 0, 5,
        //sbit, dgt, len, _r, func
        EFT_DMD_DNP, 0, 3, DTP_DMD_DNP, 0,
    },
    {                                                   //-WT需量时间(MMDDHHmm->AFMT)
        0xB02F,
        0,
        NTRD_DATR_SPEC,
        ADDR_DMD_DNPT,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_DMD_DNPT, 0, 20, DTP_DMD_DNPT, (void*)ntrd_Fmt97DemandTime,
    },
};

#define LEN_BGTSTDEMAND (sizeof(gss_NetpBgTstDemand) / sizeof(NETPRD))


/******************************************************************************
**组网端口需量数据(部规2007)
******************************************************************************/
const NETPRD gss_NetpXgTstDemand[] = 
{
    {                                                   //+W需量及时间(XX.XXXXYYMMDDHHmm->Hex4.4+AFMT)
        0x0101FF00,
        NTRD_PATR_MUST,
        NTRD_DATR_SPEC, 
        ADDR_DMD_DPP,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_DMD_DPP, 0, 40, DTP_DMD_DPP, (void*)ntrd_Fmt07DemandData,
    },
    {                                                   //-W需量及时间(XX.XXXXYYMMDDHHmm->Hex4.4+AFMT)
        0x0102FF00,
        0,
        NTRD_DATR_SPEC, 
        ADDR_DMD_DNP,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_DMD_DNP, 0, 40, DTP_DMD_DNP, (void*)ntrd_Fmt07DemandData,
    },

};


#define LEN_XGTSTDEMAND (sizeof(gss_NetpXgTstDemand) / sizeof(NETPRD))




/******************************************************************************
**事件数据(部规1997)
******************************************************************************/
const NETPRD gss_NetpBgEvtData[] = 
{
    {                                                   //电表运行状态字 C020
        0xC020,
        0,
        0x0000, 
        ADDR_EVT_BSTT,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_BSTT, 0, 1, DTP_EVT_BSTT, 0,
    },
    {                                                   //抄表日 C117
        0xC117,
        0,
        0x0000,
        ADDR_EVT_DH,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_DH, 0, 2, DTP_EVT_DH, 0,
    },
    {                                                   //有功电表常数 C030
        0xC030,
        0,
        NTRD_DATR_THEX | NTRD_DATR_BEXT, 
        ADDR_EVT_IMP0,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_IMP0, 0, 3, DTP_EVT_IMP0, 0,
    },
    {                                                   //无功电表常数 C031
        0xC031,
        0,
        NTRD_DATR_THEX | NTRD_DATR_BEXT, 
        ADDR_EVT_IMP1,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_IMP1, 0, 3, DTP_EVT_IMP1, 0,
    },
    {                                                   //时段费率 C33F
        0xC33F,
        0,
        0, 
        ADDR_EVT_FTN,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_FTN, 0, 24, DTP_EVT_FTN, 0,
    },
    {                                                   //编程次数 B212
        0xB212,
        0,
        NTRD_DATR_THEX | NTRD_DATR_BEXT, 
        ADDR_EVT_PCNT,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_PCNT, 0, 2, DTP_EVT_PCNT, 0,
    },
    {                                                   //需量清零次数 B213
        0xB213,
        0,
        NTRD_DATR_THEX | NTRD_DATR_BEXT, 
        ADDR_EVT_DEMDCNT,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_DEMDCNT, 0, 2, DTP_EVT_DEMDCNT, 0,
    },
    {                                                   //最近需量清零时间 B211
        0xB211,
        0,
        0, 
        ADDR_EVT_DEMDT,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_DEMDT, 0, 4, DTP_EVT_DEMDT,  0,
    },
    {                                                   //断相次数 B31F
        0xB31F,
        0,
        NTRD_DATR_THEX | NTRD_DATR_BEXT, 
        ADDR_EVT_DXN,
        //zhtp, zhid, zhnum,
        0, 0, 4,
        //sbit, dgt, len, _r, func
        EFT_EVT_DXN, 0, 2, DTP_EVT_DXN, 0,
    },
    {                                                   //电池工作时间 B214
        0xB214,
        0,
        NTRD_DATR_THEX | NTRD_DATR_BEXT, 
        ADDR_EVT_BATY,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_BATY, 0, 3, DTP_EVT_BATY, 0,
    },
};

#define LEN_BGEVTDATA (sizeof(gss_NetpBgEvtData) / sizeof(NETPRD))



/******************************************************************************
**事件数据(部规2007)
******************************************************************************/
const NETPRD gss_NetpXgEvtData[] =
{
    {                                                   //电表运行状态字(XXXX*7)
        0x040005FF,
        0,
        0x0000, 
        ADDR_EVT_BSTT,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_BSTT, 0, 14, DTP_EVT_BSTT, 0,
    },
    {                                                   //最近跳闸时间(ssmmhhDDMMYY)
        0x1D000101,
        0,
        0x0000, 
        ADDR_EVT_DTIME,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_DTIME, 0, 6, DTP_EVT_DTIME,  0,
    },
    {                                                   //最近合闸时间(ssmmhhDDMMYY)
        0x1E000101,
        0,
        0x0000, 
        ADDR_EVT_HTIME,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_HTIME, 0, 6, DTP_EVT_HTIME, 0,
    },
    {                                                   //编程次数(XXXXXX)
        0x03300000,
        0,
        NTRD_DATR_THEX | NTRD_DATR_BEXT, 
        ADDR_EVT_PCNT,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_PCNT, 0, 3, DTP_EVT_PCNT, 0,
    },
    {                                                   //最近编程时间(ssmmhhDDMMYY)
        0x03300001,
        0,
        0x000000,
        ADDR_EVT_PROGT,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_PROGT, 0, 6, DTP_EVT_PROGT, 0,
    },
    {
        0x03300E00,                                     //开端钮盒次数
        0,
        NTRD_DATR_THEX | NTRD_DATR_BEXT, 
        ADDR_EVT_DNCNT,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_DNCNT, 0, 3, DTP_EVT_DNCNT, 0,   
    },
    {
        0x03300D00,                                     //开表盖次数
        0,
        NTRD_DATR_THEX | NTRD_DATR_BEXT, 
        ADDR_EVT_WGCNT,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_WGCNT, 0, 3, DTP_EVT_WGCNT, 0,   
    },
    {
        0x03300D01,                                     //开表盖时间
        0,
        0x0000,
        ADDR_EVT_WGTIME,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_WGTIME, 0, 6, DTP_EVT_WGTIME, 0,
    },
    {
        0x03300400,                                     //校时次数
        0,
        NTRD_DATR_THEX | NTRD_DATR_BEXT, 
        ADDR_EVT_JSCNT,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_JSCNT, 0, 3, DTP_EVT_JSCNT, 0,   
    },
    {
        0x03300401,                                     //最近一次校时时间
        0,
        NTRD_DATR_SPEC,
        ADDR_EVT_JSTIME,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_JSTIME, 0, 12, DTP_EVT_JSTIME, (void*)ntrd_Fmt07JSTime,
    },
    {
        0x03300500,                                     //时段表编程次数 
        0,
        NTRD_DATR_THEX | NTRD_DATR_BEXT, 
        ADDR_EVT_DPCNT,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_DPCNT, 0, 3, DTP_EVT_DPCNT, 0,   
    },
    {
        0x03300501,                                     //时段表编程时间
        0,
        0x0000,
        ADDR_EVT_DPTIME,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_DPTIME, 0, 6, DTP_EVT_DPTIME, 0,
    },
    {                                                   //需量清零次数(XXXXXX)
        0x03300200,
        0,
        NTRD_DATR_THEX | NTRD_DATR_BEXT, 
        ADDR_EVT_DEMDCNT,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_DEMDCNT, 0, 3, DTP_EVT_DEMDCNT, 0,
    },
    {                                                   //最近需量清零时间(ssmmhhDDMMYY)
        0x03300201,
        0,
        0x0000, 
        ADDR_EVT_DEMDT,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_DEMDT, 0, 6, DTP_EVT_DEMDT,  0,
    },
    {                                                   //第一抄表日(DDhh)
        0x04000B01,
        0,
        0x0000, 
        ADDR_EVT_DH,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_DH, 0, 2, DTP_EVT_DH, 0,
    },
    {                                                   //第二抄表日(DDhh)
        0x04000B02,
        0,
        0x0000, 
        ADDR_EVT_DH + 2,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_DH + 1, 0, 2, DTP_EVT_DH, 0,
    },
    {                                                   //第三抄表日(DDhh)
        0x04000B03,
        0,
        0x0000, 
        ADDR_EVT_DH + 4,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_DH + 2, 0, 2, DTP_EVT_DH, 0,
    },
    {                                                   //时段费率
        0x04010001,
        0,
        0, 
        ADDR_EVT_FTN,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_FTN, 0, 24, DTP_EVT_FTN, 0,
    },
    {                                                   //有功电表常数(XXXXXX)
        0x04000409,
        0,
        NTRD_DATR_THEX | NTRD_DATR_BEXT, 
        ADDR_EVT_IMP0,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_IMP0, 0, 3, DTP_EVT_IMP0, 0,
    },
    {                                                   //无功电表常数(XXXXXX)
        0x0400040A,
        0,
        NTRD_DATR_THEX | NTRD_DATR_BEXT, 
        ADDR_EVT_IMP1,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_IMP1, 0, 3, DTP_EVT_IMP1, 0,
    },
    {                                                   //A相断相次数(XXXXXX)
        0x13010001,
        0,
        NTRD_DATR_SPEC,
        ADDR_EVT_DXN + 4,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_DXN, 0, 3, DTP_EVT_DXN, (void*)ntrd_FmtDxnA,
    },
    {                                                   //B相断相次数(XXXXXX)
        0x13020001,
        0,
        NTRD_DATR_SPEC,
        ADDR_EVT_DXN + 8,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_DXN, 0, 3, DTP_EVT_DXN, (void*)ntrd_FmtDxnB,
    },
    {                                                   //C相断相次数(XXXXXX)
        0x13030001,
        0,
        NTRD_DATR_SPEC,
        ADDR_EVT_DXN + 12,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_DXN, 0, 3, DTP_EVT_DXN, (void*)ntrd_FmtDxnC,
    }, 
    {
        0x03110000,                                     //停电次数
        0,
        NTRD_DATR_THEX | NTRD_DATR_BEXT,
        ADDR_EVT_TDCNT,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_TDCNT, 0, 3, DTP_EVT_TDCNT, 0,
    },
    {
        0x10000001,                                     //失压次数
        0,
        NTRD_DATR_THEX | NTRD_DATR_BEXT,
        ADDR_EVT_SYCNT,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_SYCNT, 0, 3, DTP_EVT_SYCNT, 0,
    },
    {
        0x03350000,                                     //恒定磁场干扰次数
        0,
        NTRD_DATR_THEX | NTRD_DATR_BEXT,
        ADDR_EVT_CCOUNT,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_CCOUNT, 0, 3, DTP_EVT_CCOUNT, 0,
    },
    {
        0x04000307,                                     //电压互感器变比(XXXXXX)
        0,
        NTRD_DATR_THEX | NTRD_DATR_BEXT,
        ADDR_EVT_PTBB,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_PTBB, 0, 3, DTP_EVT_PTBB, 0,
    },
    {
        0x04000306,                                     //电流互感器变比(XXXXXX)
        0,
        NTRD_DATR_THEX | NTRD_DATR_BEXT,
        ADDR_EVT_CTBB,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_CTBB, 0, 3, DTP_EVT_CTBB, 0,
    },
    {                                                   //电池工作时间(XXXXXXXX)
        0x0280000A,
        0,
        NTRD_DATR_THEX | NTRD_DATR_BEXT, 
        ADDR_EVT_BATY,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_BATY, 0, 4, DTP_EVT_BATY, 0,
    },
};
#define LEN_XGEVTDATA  (sizeof(gss_NetpXgEvtData) / sizeof(NETPRD));





#if (GDW_07DIN_EN > 0)

const NETPRD gss_NetpXgExtra[] = 
{    
    {                                                   //上日
        0x05060101,
        0,
        NTRD_DATR_THEX, 
        ADDR_EXT_WPD,
        //zhtp, zhid, zhnum,
        0, 0, 5,
        //sbit, dgt, len, _r, func
        EFT_EXT_WPD, 0, 4, DTP_EXT_WPD, 0,
    },
    {                                                   //上日-W电能示值集抄
        0x05060201,
        0,
        NTRD_DATR_THEX,
        ADDR_EXT_WND,
        //zhtp, zhid, zhnum,
        0, 0, 5,
        //sbit, dgt, len, _r, func
        EFT_EXT_WND, 0, 4, DTP_EXT_WND, 0,
    },
};

#define LEN_XGEXTRA (sizeof(gss_NetpXgExtra) / sizeof(NETPRD))

#endif

const NETPRD gss_NetpDlmsCure[] = 
{    
    {                                                   //上日-W电能示值集抄
        0x0a,       //日冻结
        0,
        NTRD_DATR_THEX,
        ADDR_DLMS_EXT_WP,
        //zhtp, zhid, zhnum,
        0, 0, 5,
        //sbit, dgt, len, _r, func
        EFT_DLMS_EXT_WN, 0, 4, DTP_EXT_WND, 0,
    },
};

#define LEN_DLMSCURE (sizeof(gss_NetpDlmsCure) / sizeof(NETPRD))

/************************************************************************
 *国网规约数据项FN对应部规标识
 ************************************************************************/
typedef struct
{
    uint16  fn;
    uint16  dinum;                  //
    const uint16* dilist;           //高字节为di类型,低字节为类型数据编码
}FnMapToDI;


/************************************************************************
 *事件数据配置信息
 ************************************************************************/
const uint16 gss_erc8DataList[] = 
{
    DTP_EVT_FTN,
    DTP_EVT_PROGT,
    DTP_EVT_DH,
    DTP_EVT_IMP0,
    DTP_EVT_IMP1,
    DTP_EVT_CTBB,
    DTP_EVT_PTBB,
    DTP_EVT_DEMDT,
};

const uint16 gss_erc12DataList[] = 
{
    DTP_TIME,
};

const uint16 gss_erc13DataList[] = 
{
    DTP_EVT_PCNT,
    DTP_EVT_DEMDCNT,
    DTP_EVT_DXN,
    DTP_EVT_SYCNT,
    DTP_EVT_TDCNT,
    DTP_EVT_BSTT,
};



const uint16 gss_erc27DataList[] = 
{
    DTP_WP,
};
#define gss_erc28DataList gss_erc27DataList
#define gss_erc29DataList gss_erc27DataList

const uint16 gss_erc30DataList[] = 
{
    DTP_WP,
    DTP_P,
};


const uint16 gss_erc33DataList[] = 
{
    DTP_EVT_BSTT,
};

const uint16 gss_erc37DataList[] = 
{
    DTP_EVT_WGCNT,
};

const uint16 gss_erc38DataList[] = 
{
    DTP_EVT_DNCNT,
};

const uint16 gss_erc40DataList[] = 
{
    DTP_EVT_CCOUNT,
};

const uint16 gss_erc41DataList[] = 
{
    DTP_EVT_JSCNT,
};


const FnMapToDI gss_ercMapToDI[] = 
{
    {
        8,
        sizeof(gss_erc8DataList) / sizeof(uint16),
        gss_erc8DataList,
    },
    {
        12,
        sizeof(gss_erc12DataList) / sizeof(uint16),
        gss_erc12DataList,
    },
    {
        13,
        sizeof(gss_erc13DataList) / sizeof(uint16),
        gss_erc13DataList,
    },
    {
        27,
        sizeof(gss_erc27DataList) / sizeof(uint16),
        gss_erc27DataList,
    },
    {
        28,
        sizeof(gss_erc28DataList) / sizeof(uint16),
        gss_erc28DataList,
    },
    {
        29,
        sizeof(gss_erc29DataList) / sizeof(uint16),
        gss_erc29DataList,
    },
    {
        30,
        sizeof(gss_erc30DataList) / sizeof(uint16),
        gss_erc30DataList,
    },
    {
        33,
        sizeof(gss_erc33DataList) / sizeof(uint16),
        gss_erc33DataList,
    },
    {
        37,
        sizeof(gss_erc37DataList) / sizeof(uint16),
        gss_erc37DataList,
    },
    {
        38,
        sizeof(gss_erc38DataList) / sizeof(uint16),
        gss_erc38DataList,
    },
    {
        40,
        sizeof(gss_erc40DataList) / sizeof(uint16),
        gss_erc40DataList,
    },
    {
        41,
        sizeof(gss_erc41DataList) / sizeof(uint16),
        gss_erc41DataList,
    },
};







/************************************************************************
 *一类实时数据配置信息
 ************************************************************************/
//必备项
const uint16 gss_0CF0DataList[] = 
{
    DTP_TIME,
    DTP_WP,
    DTP_WN,
    DTP_EXT_TSD,
    DTP_EXT_WPD,
    DTP_EXT_WND,
};

const uint16 gss_0CF25DataList[] = 
{
    DTP_P,
    DTP_U,
    DTP_I,
};

const uint16 gss_0CF27DataList[] = 
{
    DTP_TIME,
    DTP_EVT_BATY,
    DTP_EVT_PCNT,
    DTP_EVT_PROGT,
    DTP_EVT_DEMDCNT,
    DTP_EVT_DEMDT,
};

const uint16 gss_0CF28DataList[] = 
{
    DTP_EVT_BSTT,
};

const uint16 gss_0CF33DataList[] = 
{
    DTP_WP,
    DTP_WN,
};

const uint16 gss_0CF35DataList[] = 
{
    DTP_DMD_DPP,
    DTP_DMD_DPPT,
};

const uint16 gss_0CF36DataList[] = 
{
    DTP_DMD_DNP,
    DTP_DMD_DNPT,
};


const uint16 gss_0CF97DataList[] = 
{
    DTP_U,
};

#define gss_0CF98DataList gss_0CF97DataList
#define gss_0CF99DataList gss_0CF97DataList
#define gss_0CF179DataList gss_0CF97DataList
#define gss_0CF180DataList gss_0CF97DataList

const uint16 gss_0CF109DataList[] = 
{
    DTP_WP,
};


const uint16 gss_0CF111DataList[] = 
{
    DTP_WN,
};


const uint16 gss_0CF129DataList[] = 
{
    DTP_WP,
};

const uint16 gss_0CF131DataList[] = 
{
    DTP_WN,
};

const uint16 gss_0CF145DataList[] = 
{
    DTP_DMD_DPP,
    DTP_DMD_DPPT,
};

const uint16 gss_0CF147DataList[] = 
{
    DTP_DMD_DNP,
    DTP_DMD_DNPT,
};


const uint16 gss_0CF161DataList[] = 
{
    DTP_EVT_HTIME,
    DTP_EVT_DTIME,
};


const uint16 gss_0CF162DataList[] = 
{
    DTP_TIME,
};


const uint16 gss_0CF165DataList[] = 
{
    DTP_EVT_PCNT,
    DTP_EVT_PROGT,
    DTP_EVT_WGCNT,
    DTP_EVT_WGTIME,
};

const uint16 gss_0CF166DataList[] = 
{
    DTP_EVT_JSCNT,
    DTP_EVT_JSTIME,
    DTP_EVT_DPCNT,
    DTP_EVT_DPTIME,
};


const FnMapToDI gss_afn0CFnMapToDI[] = 
{
    {
        0,
        sizeof(gss_0CF0DataList) / sizeof(uint16),
        gss_0CF0DataList,
    },
    {
        25,
        sizeof(gss_0CF25DataList) / sizeof(uint16),
        gss_0CF25DataList,
    },
    {
        27,
        sizeof(gss_0CF27DataList) / sizeof(uint16),
        gss_0CF27DataList,
    },
    {
        28,
        sizeof(gss_0CF28DataList) / sizeof(uint16),
        gss_0CF28DataList,
    },
    {
        33,
        sizeof(gss_0CF33DataList) / sizeof(uint16),
        gss_0CF33DataList,
    },
    {
        35,
        sizeof(gss_0CF35DataList) / sizeof(uint16),
        gss_0CF35DataList,
    },
    {
        36,
        sizeof(gss_0CF36DataList) / sizeof(uint16),
        gss_0CF36DataList,
    },
    {
        97,
        sizeof(gss_0CF97DataList) / sizeof(uint16),
        gss_0CF97DataList,
    },
    {
        98,
        sizeof(gss_0CF98DataList) / sizeof(uint16),
        gss_0CF98DataList,
    },
    {
        99,
        sizeof(gss_0CF99DataList) / sizeof(uint16),
        gss_0CF99DataList,
    },
    {
        109,
        sizeof(gss_0CF109DataList) / sizeof(uint16),
        gss_0CF109DataList,
    },
    {
        111,
        sizeof(gss_0CF111DataList) / sizeof(uint16),
        gss_0CF111DataList,
    },
    {
        129,
        sizeof(gss_0CF129DataList) / sizeof(uint16),
        gss_0CF129DataList,
    },
    {
        131,
        sizeof(gss_0CF131DataList) / sizeof(uint16),
        gss_0CF131DataList,
    },
    {
        145,
        sizeof(gss_0CF145DataList) / sizeof(uint16),
        gss_0CF145DataList,
    },
    {
        147,
        sizeof(gss_0CF147DataList) / sizeof(uint16),
        gss_0CF147DataList,
    },
    {
        161,
        sizeof(gss_0CF161DataList) / sizeof(uint16),
        gss_0CF161DataList,
    },
    {
        162,
        sizeof(gss_0CF162DataList) / sizeof(uint16),
        gss_0CF162DataList,
    },
    {
        165,
        sizeof(gss_0CF165DataList) / sizeof(uint16),
        gss_0CF165DataList,
    },
    {
        166,
        sizeof(gss_0CF166DataList) / sizeof(uint16),
        gss_0CF166DataList,
    },
    {
        179,
        sizeof(gss_0CF179DataList) / sizeof(uint16),
        gss_0CF179DataList,
    },
    {
        180,
        sizeof(gss_0CF180DataList) / sizeof(uint16),
        gss_0CF180DataList,
    },
};





/************************************************************************
 *二类历史数据配置信息(包括了97和07规约要抄读的帧)
 ************************************************************************/
//必备项
const uint16 gss_0DF0DataList[] = 
{
    DTP_WP,
    DTP_WN,
    DTP_EXT_TSD,
    DTP_EXT_WPD,
    DTP_EXT_WND,
};


//日冻结需量
const uint16 gss_0DF3DataList[] = 
{
    DTP_DMD_DPP,
    DTP_DMD_DPPT,
};

const uint16 gss_0DF4DataList[] = 
{
    DTP_DMD_DNP,
    DTP_DMD_DNPT,
};

//抄表日冻结需量
#define gss_0DF11DataList gss_0DF3DataList
#define gss_0DF12DataList gss_0DF4DataList

//月冻结需量
#define gss_0DF19DataList gss_0DF3DataList
#define gss_0DF20DataList gss_0DF4DataList


//日总及分相需量统计
const uint16 gss_0DF26DataList[] = 
{
    DTP_DMD_DPP,
    DTP_DMD_DPPT,
};
//日电压统计
const uint16 gss_0DF27DataList[] = 
{
    DTP_U,
};

//月总及分相需量统计
#define gss_0DF34DataList gss_0DF26DataList
//月电压统计
#define gss_0DF35DataList gss_0DF27DataList


//电压曲线
const uint16 gss_0DF89DataList[] = 
{
    DTP_U,
};
#define gss_0DF90DataList gss_0DF89DataList
#define gss_0DF91DataList gss_0DF89DataList

//电流曲线
const uint16 gss_0DF92DataList[] = 
{
    DTP_I,
};
#define gss_0DF93DataList gss_0DF92DataList
#define gss_0DF94DataList gss_0DF92DataList

//日冻结需量
#define gss_0DF185DataList gss_0DF3DataList
#define gss_0DF187DataList gss_0DF4DataList
//抄表日冻结需量
#define gss_0DF189DataList gss_0DF3DataList
#define gss_0DF191DataList gss_0DF4DataList



const FnMapToDI gss_afn0DFnMapToDI[] = 
{
    {
        0,
        sizeof(gss_0DF0DataList) / sizeof(uint16),
        gss_0DF0DataList,
    },
    {
        3,
        sizeof(gss_0DF3DataList) / sizeof(uint16),
        gss_0DF3DataList,
    },
    {
        4,
        sizeof(gss_0DF4DataList) / sizeof(uint16),
        gss_0DF4DataList,
    },
    {
        11,
        sizeof(gss_0DF11DataList) / sizeof(uint16),
        gss_0DF11DataList,
    },
    {
        12,
        sizeof(gss_0DF12DataList) / sizeof(uint16),
        gss_0DF12DataList,
    },
    {
        19,
        sizeof(gss_0DF19DataList) / sizeof(uint16),
        gss_0DF19DataList,
    },
    {
        20,
        sizeof(gss_0DF20DataList) / sizeof(uint16),
        gss_0DF20DataList,
    },
    {
        26,
        sizeof(gss_0DF26DataList) / sizeof(uint16),
        gss_0DF26DataList,
    },
    {
        27,
        sizeof(gss_0DF27DataList) / sizeof(uint16),
        gss_0DF27DataList,
    },
    {
        34,
        sizeof(gss_0DF34DataList) / sizeof(uint16),
        gss_0DF34DataList,
    },
    {
        35,
        sizeof(gss_0DF35DataList) / sizeof(uint16),
        gss_0DF35DataList,
    },
    {
        89,
        sizeof(gss_0DF89DataList) / sizeof(uint16),
        gss_0DF89DataList,
    },
    {
        90,
        sizeof(gss_0DF90DataList) / sizeof(uint16),
        gss_0DF90DataList,
    },
    {
        91,
        sizeof(gss_0DF91DataList) / sizeof(uint16),
        gss_0DF91DataList,
    },
    {
        92,
        sizeof(gss_0DF92DataList) / sizeof(uint16),
        gss_0DF92DataList,
    },
    {
        93,
        sizeof(gss_0DF93DataList) / sizeof(uint16),
        gss_0DF93DataList,
    },
    {
        94,
        sizeof(gss_0DF94DataList) / sizeof(uint16),
        gss_0DF94DataList,
    },
    {
        185,
        sizeof(gss_0DF185DataList) / sizeof(uint16),
        gss_0DF185DataList,
    },
    {
        187,
        sizeof(gss_0DF187DataList) / sizeof(uint16),
        gss_0DF187DataList,
    },
    {
        189,
        sizeof(gss_0DF189DataList) / sizeof(uint16),
        gss_0DF189DataList,
    },
    {
        191,
        sizeof(gss_0DF191DataList) / sizeof(uint16),
        gss_0DF191DataList,
    },
};







/************************************************************************
 * @function: LoadafnDICfg
 * @描述: 载入一类二类数据的抄读项配置
 * 
 * @参数: 
 * @param: dlh 大类号
 * @param: xlh 小类号
 * @param: output 配置输出
 * 
 * @返回: 
 * @return: uint8  成功/失败
 * @说明: 
 * @作者: yzy (2012-8-21)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 LoadafnDICfg(uint8 dlh, uint8 xlh, DXLFnMap* output)
{
                                            //首先清零输出缓存
//	    memset((uint8*)output, 0, sizeof(DXLFnMap));
//	
//	    if((dlh > 15) || (xlh > 15))            //非法帧,退出
//	    {
//	        return SYS_ERR_FT;
//	    }
//	                                            //1.载入大小类号配置
//	    for(uint8 uc_i = 0; uc_i < 2; uc_i++)
//	    {
//	        if(uc_i == 0)                       //afn0c
//	        {
//	            LoadParaDXL(dlh, xlh, 0x0C, output->afncmap);
//	            output->afncmap[31] = 0;
//	        }
//	        else                                //afn0d
//	        {
//	            LoadParaDXL(dlh, xlh, 0x0D, output->afndmap);
//	            output->afndmap[31] = 0;
//	        }
//	                                            //判断参数有效性
//	        output->dlh = dlh;
//	        output->xlh = xlh;
//	        output->_r = GetTime()->hour;
//	    }
//	    //*******************************
//	                                            //2.根据大小类号配置,搜索待抄读FN对应的部规DI
//	    const FnMapToDI* ftdtbl;
//	    uint16 tbllen;
//	    uint8* afnmap;
//	    for(uint8 index = 0; index < 2; index++)
//	    {
//	        if(index == 0)                      //首先搜索AFN0CFN列表
//	        {
//	            tbllen = sizeof(gss_afn0CFnMapToDI) / sizeof(FnMapToDI);
//	            ftdtbl = gss_afn0CFnMapToDI;
//	            afnmap = output->afncmap;
//	        }
//	        else if(index == 1)                 //其次搜索AFN0DFN列表
//	        {
//	            tbllen = sizeof(gss_afn0DFnMapToDI) / sizeof(FnMapToDI);
//	            ftdtbl = gss_afn0DFnMapToDI;
//	            afnmap = output->afndmap;
//	        }
//	                                            //遍历终端支持的FN列表
//	        for(uint8 uc_i = 0; uc_i < tbllen; uc_i++, ftdtbl++)
//	        {                                   //判在此大小类号中,该FN项是否配置为抄读.(fn==0表示必选项)
//	            if((ftdtbl->fn == 0) || (STR_IsBitSet(afnmap, (ftdtbl->fn - 1))))
//	            {                               //置位该FN对应的标识位
//	                for(uint8 uc_j = 0; uc_j < ftdtbl->dinum; uc_j++)
//	                {
//	                    STR_SetBit(output->dimap, ftdtbl->dilist[uc_j]);
//	                }
//	            }
//	        }
//	    }
//	    //*******************************
//	                                            //3.根据事件配置,搜索所需的额外部规DI
//	    tbllen = sizeof(gss_ercMapToDI) / sizeof(FnMapToDI);
//	    ftdtbl = gss_ercMapToDI;
//	                                            //遍历
//	    for(uint8 uc_i = 0; uc_i < tbllen; uc_i++, ftdtbl++)
//	    {                                       //该事件是否使能.erc范围从0开始.
//	        if((ftdtbl->fn > 0) && STR_IsBitSet(gs_InpVarData.evtbit, (ftdtbl->fn - 1)))
//	        {                                   //记录该事件所需要抄读的的DI
//	            for(uint8 uc_j = 0; uc_j < ftdtbl->dinum; uc_j++)
//	            {
//	                STR_SetBit(output->dimap, ftdtbl->dilist[uc_j]);
//	            }
//	        }
//	    }
//	                                            //建立CRC
//	    BuildBufferCRC((uint8*)output, sizeof(DXLFnMap), true);
    SYS_OK();
}



/************************************************************************
 * @function: ResetafnDICfg
 * @描述: 复位大类号小类号配置,即要求重新载入
 * 
 * @参数: 
 * 
 * @返回: 
 * @return: 
 * @说明: 
 * @作者: yzy (2012-8-21)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void ResetafnDICfg(void)
{
    //ClearBuffer((uint8*)&gs_afnDICfg, sizeof(gs_afnDICfg));
}


/************************************************************************
 * @function: IsGatpDIRead
 * @描述: 判断该标识是否需要抄读
 * 
 * @参数: 
 * @param: tno 测量点号
 * @param: nrd 抄读结构体数组
 * 
 * @返回: 
 * @return: uint8  0不用抄读,1要抄读,2-0xFF延迟抄读(要抄读但未到抄读世纪,比如上日冻结时间不一致时,延迟抄读上日冻结数据)
 * @说明: 判断是否抄读的机制:存储区是否已经有数据.
 * @作者: yzy (2012-6-7)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 IsNetpDIRead(uint16 tno, NETPRD* nrd)
{
    return 1;
    /*
    uint8 dlh = gss_CDATATst[tno].dicfg >> 4;
    uint8 xlh = gss_CDATATst[tno].dicfg & 0x0F;
    
    //此处加入大类号小类号的判断
                                     //载入大类号小类号配置
    if(!CheckBufferCRC((uint8*)&gs_afnDICfg, sizeof(gs_afnDICfg), true) || 
       (gs_afnDICfg.dlh != dlh) || 
       (gs_afnDICfg.xlh != xlh) ||
       (gs_afnDICfg._r != GetTime()->hour))
    {
        if(LoadafnDICfg(dlh, xlh, &gs_afnDICfg) != SA_ERR_OK)
        {
            return 0;
        }
    }
                                    //判断该标识是否待抄
    if(STR_IsBitSet(gs_afnDICfg.dimap, nrd->_r))
    {
        return 1;
    }
    //此处加入大类号小类号的判断

    return 0;
    */
}











