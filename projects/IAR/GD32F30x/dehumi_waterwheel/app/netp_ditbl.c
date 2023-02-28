




uint8 FmtBgTimeTemp[6];                                 //���ʱ,������

/************************************************************************
 * @function: ntrd_FmtC010
 * @����: ��C010����YYMMDDWW������������
 * 
 * @����: 
 * @param: buff ����
 * @param: nt ��ʽ��Ϣ�ṹ��
 * @param: tstno �������
 * @param: num ���ڴ��������
 * @����: 
 * @˵��: 
 * @����: yzy (2011-11-25)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void ntrd_FmtC010(uint8* buff, NETPSTR* rb, uint8 tstno, uint8 num)
{
    S_TstRealData*  tstdata;    
    //BCD��ʽWW(DDMMYY)->mh(DMH)
    tstdata = (S_TstRealData*)rb->data;
    MoveBuffer(buff + 1, tstdata->time + 3, 3);
}



/************************************************************************
 * @function: ntrd_FmtC011
 * @����: ��C011����hhmmss������������
 * 
 * @����: 
 * @param: buff ����
 * @param: nt ��ʽ��Ϣ�ṹ��
 * @param: tstno �������
 * @param: num ���ڴ��������
 * @����: 
 * @˵��: 
 * @����: yzy (2011-11-25)
 *-----------------------------------------------------------------------
 * @�޸���: zgb
 ************************************************************************/
void ntrd_FmtC011(uint8* buff, NETPSTR* rb, uint8 tstno, uint8 num)
{
    S_TstRealData*  tstdata;                            //BCD��ʽss(mmhh)->(mh)DMH
    tstdata = (S_TstRealData*)rb->data;
    MoveBuffer(buff, tstdata->time, 3);      //ת����ʽ
    //tstdata->time = FMT_UnifyTime(FmtBgTimeTemp, true, false);
}





/************************************************************************
 * @function: ntrd_Fmt07JSTime
 * @����: �����һ��Уʱʱ��������������
 * 
 * @����: 
 * @param: buff 
 * @param: rb 
 * @param: tstno 
 * @param: num 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/2/7)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void ntrd_Fmt07JSTime(uint8* buff, NETPSTR* rb, uint8 tstno, uint8 num)
{
    NETPRD* nt = rb->tbl + rb->idx;
    MoveBuffer(buff + 4 , rb->data + nt->add ,  12);   
}





/************************************************************************
**��������: void FmtDXN(uint8* buff, NETPRD* nt, uint8 tstno, uint8 phase)
**�� ��: ��13010001/13020001/13030001�ȷ���������������������
**
**�� ��: buff,���� 
**       tstno,�������
**       phase ���(0-2)
**�� ��: 
**
**�� ע: 
**-----------------------------------------------------------------------
**�� ��: 
**�� ��: 
*************************************************************************/
void FmtDXN(uint8* buff, NETPSTR* rb, uint8 tstno, uint8 phase)
{
    uint32 dxn = 0;
    S_EvtRealData* evtreal;
    evtreal = (S_EvtRealData*)rb->data;
    NETPRD* nt = rb->tbl + rb->idx;                     //���·���������
    BcdToHex(buff, rb->data + nt->add, 3, 4);
                                    //�����ܶ������
    for(uint8 uc_i = 0; uc_i < 3; uc_i++)
    {
        dxn +=  evtreal->dxn[uc_i + 1];
    }
                                    //645-2007�����ܶ���ͳ��
    evtreal->dxn[0] = dxn;
}




/************************************************************************
**��������: ntrd_FmtDxnA()
**�� ��: ��A��������������������
**�� ��: 
**�� ��: 
*************************************************************************/
void ntrd_FmtDxnA(uint8* buff, NETPSTR* rb, uint8 tstno, uint8 num)
{
    FmtDXN(buff, rb ,tstno, 0);
}

/************************************************************************
**��������: ntrd_FmtDxnB()
**�� ��: ��B��������������������
**�� ��: 
**�� ��: 
*************************************************************************/
void ntrd_FmtDxnB(uint8* buff, NETPSTR* rb, uint8 tstno, uint8 num)
{
    FmtDXN(buff , rb , tstno , 1);
}

/************************************************************************
**��������: ntrd_FmtDxnC()
**�� ��: ��C��������������������
**�� ��: 
**�� ��: 
*************************************************************************/
void ntrd_FmtDxnC(uint8* buff, NETPSTR* rb, uint8 tstno, uint8 num)
{
    FmtDXN(buff, rb , tstno , 2);
}
//----------------------------------------------------------------------




/******************************************************************************
**�����˿�ʵʱ����(����1997)
******************************************************************************/
const NETPRD gss_NetpBgTstData[] = 
{
//***********************
    {                                                   //+W����ʾֵ����
        0x0000901F,
        NTRD_PATR_MUST | NTRD_PATR_SKIP,
        NTRD_DATR_THEX, 
        ADDR_WP,
        //zhtp, zhid, zhnum,
        NTRD_ZHTP_MUFL, 0xFF, 5,
        //sbit, dgt, len, _r, func
        EFT_WP, 0, 4, DTP_WP, 0,
    },
    {                                                   //+W��
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
    {                                                   //-W����ʾֵ����
        0x902F,
        NTRD_PATR_SKIP,
        NTRD_DATR_THEX, 
        ADDR_WN,
        //zhtp, zhid, zhnum,
        NTRD_ZHTP_MUFL, 0xFF, 5,
        //sbit, dgt, len, _r, func
        EFT_WN, 0, 4, DTP_WN, 0,
    },
    {                                                   //-W��
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
        0xB61F,                                         //�����ѹ����
        NTRD_PATR_SKIP,
        NTRD_DATR_THEX | NTRD_DATR_PMUL,
        ADDR_U,
        //zhtp, zhid, zhnum,
        0, 0xFF, 3,
        //sbit, dgt, len, _r, func
        EFT_U, 2, 2, DTP_U, 0,
    },
    {
        0xB611,                                         //A���ѹ
        0,
        NTRD_DATR_THEX | NTRD_DATR_PMUL,
        ADDR_U,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_U, 2, 2, DTP_U, 0,
    },
    {
        0xB612,                                         //B���ѹ
        0,
        NTRD_DATR_THEX | NTRD_DATR_PMUL,
        ADDR_U + 2,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_U + 1, 2, 2, DTP_U, 0,
    },
    {
        0xB613,                                         //C���ѹ
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
        0xB62F,                                         //�����������
        NTRD_PATR_SKIP,
        NTRD_DATR_THEX | NTRD_DATR_PMUL | NTRD_DATR_BEXT,
        ADDR_I,
        //zhtp, zhid, zhnum,
        0, 0xFF, 3,
        //sbit, dgt, len, _r, func
        EFT_I, 1, 2, DTP_I, 0
    },
    {
        0xB621,                                         //A�����
        0,
        NTRD_DATR_THEX | NTRD_DATR_PMUL | NTRD_DATR_BEXT,
        ADDR_I,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_I, 1, 2, DTP_I, 0
    },
    {
        0xB622,                                         //B�����
        0,
        NTRD_DATR_THEX | NTRD_DATR_PMUL | NTRD_DATR_BEXT,
        ADDR_I + 4,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_I + 1, 1, 2, DTP_I, 0
    },
    {
        0xB623,                                         //C�����
        0,
        NTRD_DATR_THEX | NTRD_DATR_PMUL | NTRD_DATR_BEXT,
        ADDR_I + 8,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_I + 2, 1, 2, DTP_I, 0
    },
//***********************
    {                                                   //������ C010(��C010)
        0xC010,
        0,
        NTRD_DATR_SPEC,
        ADDR_TIME,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_TIME, 0, 4, DTP_TIME, (void*)ntrd_FmtC010,
    },
    {                                                   //��ʱ C011(��C011)
        0xC011,
        0,
        NTRD_DATR_SPEC,
        ADDR_TIME,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_TIME, 0, 3, DTP_TIME, (void*)ntrd_FmtC011,
    },
    {                                                   //�ܹ���
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
**�����˿�ʵʱ����(����2007)
******************************************************************************/
/************************************************************************ 
**645-2007����������ʶ, 
**��ʶ����:DI1(������)DI0(��N������)DI3(����)DI2(С��)
*************************************************************************/ 
const NETPRD gss_NetpXgTstData[] = 
{
//***********************
    {                                                   //+W����ʾֵ����
        0x0001FF00,
        NTRD_PATR_MUST | NTRD_PATR_SKIP,
        NTRD_DATR_THEX, 
        ADDR_WP,
        //zhtp, zhid, zhnum,
        NTRD_ZHTP_MUFL, 0xFF, 5,
        //sbit, dgt, len, _r, func
        EFT_WP, 0, 4, DTP_WP, 0,
    },
    {                                                   //+W��
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
    {                                                   //-W����ʾֵ����
        0x0002FF00,
        NTRD_PATR_SKIP,
        NTRD_DATR_THEX, 
        ADDR_WN,
        //zhtp, zhid, zhnum,
        NTRD_ZHTP_MUFL, 0xFF, 5,
        //sbit, dgt, len, _r, func
        EFT_WN, 0, 4, DTP_WN, 0,
    },
    {                                                   //-W��
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
    {                                                   //�����ѹ����
        0x0201FF00,
        NTRD_PATR_SKIP,
        NTRD_DATR_THEX | NTRD_DATR_PMUL,
        ADDR_U,
        //zhtp, zhid, zhnum,
        0, 0xFF, 3,
        //sbit, dgt, len, _r, func
        EFT_U, 1, 2, DTP_U, 0,
    },
    {                                                   //A���ѹ
        0x02010100,
        0,
        NTRD_DATR_THEX | NTRD_DATR_PMUL,
        ADDR_U,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_U, 1, 2, DTP_U, 0,
    },
    {                                                   //B���ѹ
        0x02010200,
        0,
        NTRD_DATR_THEX | NTRD_DATR_PMUL,
        ADDR_U + 2,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_U + 1, 1, 2, DTP_U, 0,
    },
    {                                                   //C���ѹ
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
    {                                                   //�����������
        0x0202FF00,
        NTRD_PATR_SKIP,
        NTRD_DATR_THEX | NTRD_DATR_BEXT | NTRD_DATR_INSG,
        ADDR_I,
        //zhtp, zhid, zhnum,
        0, 0xFF, 3,
        //sbit, dgt, len, _r, func
        EFT_I, 1, 3, DTP_I, 0,
    },
    {                                                   //A�����
        0x02020100,
        0,
        NTRD_DATR_THEX | NTRD_DATR_BEXT | NTRD_DATR_INSG,
        ADDR_I,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_I, 1, 3, DTP_I, 0,
    },
    {                                                   //B�����
        0x02020200,
        0,
        NTRD_DATR_THEX | NTRD_DATR_BEXT | NTRD_DATR_INSG,
        ADDR_I + 4,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_I + 1, 1, 3, DTP_I, 0,
    },
    {                                                   //C�����
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
    {                                                   //������(WWDDMMYY,��C010)
        0x04000101,
        0,
        NTRD_DATR_SPEC,
        ADDR_TIME,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_TIME, 0, 4, DTP_TIME, (void*)ntrd_FmtC010,
    },
    {                                                   //��ʱ(ssmmhh,��C011)
        0x04000102,
        0,
        NTRD_DATR_SPEC,
        ADDR_TIME,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_TIME, 0, 3, DTP_TIME, (void*)ntrd_FmtC011,
    },
    {                                                   //��
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
**��������: ntrd_Fmt97DemandTime()
**�� ��: ��97������ʱ�����뻺��
**�� ��: 
**�� ��: 
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
**��������: ntrd_Fmt07DemandData()
**�� ��: ��07���������ݼ�ʱ�����뻺��
**�� ��: 
**�� ��: 
*************************************************************************/
void ntrd_Fmt07DemandData(uint8* buff, NETPSTR* rb, uint8 tstno, uint8 num)
{
    NETPRD* nt = rb->tbl + rb->idx;
    uint8* paddr = rb->data + nt->add;
    for(uint8 uc_i = 0; uc_i < 5; uc_i++)
    {                                       //������ʱ��(XX.XXXXYYMMDDhhmm)
        BcdToHex(buff + uc_i * 8, paddr + uc_i * 4, 3, 4);
        LongToByte(paddr + 20 + uc_i * 4, FMT_UnifyTime(buff + uc_i * 8 + 3, true, false));
    }
                                            //��Ҫ������λ
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
**�����˿���������(����1997)
******************************************************************************/
const NETPRD gss_NetpBgTstDemand[] = 
{
    {                                                   //+W����(��\4��������,Hex4.4)
        0xA01F,
        NTRD_PATR_MUST,
        NTRD_DATR_THEX | NTRD_DATR_BEXT,
        ADDR_DMD_DPP,
        //zhtp, zhid, zhnum,
        0, 0, 5,
        //sbit, dgt, len, _r, func
        EFT_DMD_DPP, 0, 3, DTP_DMD_DPP, 0,
    },
    {                                                   //+WT����ʱ��(MMDDHHmm->AFMT)
        0xB01F,
        0,
        NTRD_DATR_SPEC,
        ADDR_DMD_DPPT,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_DMD_DPPT, 0, 20, DTP_DMD_DPPT, (void*)ntrd_Fmt97DemandTime,
    },
    {                                                   //-W����(��\4��������,Hex4.4)
        0xA02F,
        0,
        NTRD_DATR_THEX | NTRD_DATR_BEXT,
        ADDR_DMD_DNP,
        //zhtp, zhid, zhnum,
        0, 0, 5,
        //sbit, dgt, len, _r, func
        EFT_DMD_DNP, 0, 3, DTP_DMD_DNP, 0,
    },
    {                                                   //-WT����ʱ��(MMDDHHmm->AFMT)
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
**�����˿���������(����2007)
******************************************************************************/
const NETPRD gss_NetpXgTstDemand[] = 
{
    {                                                   //+W������ʱ��(XX.XXXXYYMMDDHHmm->Hex4.4+AFMT)
        0x0101FF00,
        NTRD_PATR_MUST,
        NTRD_DATR_SPEC, 
        ADDR_DMD_DPP,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_DMD_DPP, 0, 40, DTP_DMD_DPP, (void*)ntrd_Fmt07DemandData,
    },
    {                                                   //-W������ʱ��(XX.XXXXYYMMDDHHmm->Hex4.4+AFMT)
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
**�¼�����(����1997)
******************************************************************************/
const NETPRD gss_NetpBgEvtData[] = 
{
    {                                                   //�������״̬�� C020
        0xC020,
        0,
        0x0000, 
        ADDR_EVT_BSTT,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_BSTT, 0, 1, DTP_EVT_BSTT, 0,
    },
    {                                                   //������ C117
        0xC117,
        0,
        0x0000,
        ADDR_EVT_DH,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_DH, 0, 2, DTP_EVT_DH, 0,
    },
    {                                                   //�й������ C030
        0xC030,
        0,
        NTRD_DATR_THEX | NTRD_DATR_BEXT, 
        ADDR_EVT_IMP0,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_IMP0, 0, 3, DTP_EVT_IMP0, 0,
    },
    {                                                   //�޹������ C031
        0xC031,
        0,
        NTRD_DATR_THEX | NTRD_DATR_BEXT, 
        ADDR_EVT_IMP1,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_IMP1, 0, 3, DTP_EVT_IMP1, 0,
    },
    {                                                   //ʱ�η��� C33F
        0xC33F,
        0,
        0, 
        ADDR_EVT_FTN,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_FTN, 0, 24, DTP_EVT_FTN, 0,
    },
    {                                                   //��̴��� B212
        0xB212,
        0,
        NTRD_DATR_THEX | NTRD_DATR_BEXT, 
        ADDR_EVT_PCNT,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_PCNT, 0, 2, DTP_EVT_PCNT, 0,
    },
    {                                                   //����������� B213
        0xB213,
        0,
        NTRD_DATR_THEX | NTRD_DATR_BEXT, 
        ADDR_EVT_DEMDCNT,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_DEMDCNT, 0, 2, DTP_EVT_DEMDCNT, 0,
    },
    {                                                   //�����������ʱ�� B211
        0xB211,
        0,
        0, 
        ADDR_EVT_DEMDT,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_DEMDT, 0, 4, DTP_EVT_DEMDT,  0,
    },
    {                                                   //������� B31F
        0xB31F,
        0,
        NTRD_DATR_THEX | NTRD_DATR_BEXT, 
        ADDR_EVT_DXN,
        //zhtp, zhid, zhnum,
        0, 0, 4,
        //sbit, dgt, len, _r, func
        EFT_EVT_DXN, 0, 2, DTP_EVT_DXN, 0,
    },
    {                                                   //��ع���ʱ�� B214
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
**�¼�����(����2007)
******************************************************************************/
const NETPRD gss_NetpXgEvtData[] =
{
    {                                                   //�������״̬��(XXXX*7)
        0x040005FF,
        0,
        0x0000, 
        ADDR_EVT_BSTT,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_BSTT, 0, 14, DTP_EVT_BSTT, 0,
    },
    {                                                   //�����բʱ��(ssmmhhDDMMYY)
        0x1D000101,
        0,
        0x0000, 
        ADDR_EVT_DTIME,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_DTIME, 0, 6, DTP_EVT_DTIME,  0,
    },
    {                                                   //�����բʱ��(ssmmhhDDMMYY)
        0x1E000101,
        0,
        0x0000, 
        ADDR_EVT_HTIME,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_HTIME, 0, 6, DTP_EVT_HTIME, 0,
    },
    {                                                   //��̴���(XXXXXX)
        0x03300000,
        0,
        NTRD_DATR_THEX | NTRD_DATR_BEXT, 
        ADDR_EVT_PCNT,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_PCNT, 0, 3, DTP_EVT_PCNT, 0,
    },
    {                                                   //������ʱ��(ssmmhhDDMMYY)
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
        0x03300E00,                                     //����ť�д���
        0,
        NTRD_DATR_THEX | NTRD_DATR_BEXT, 
        ADDR_EVT_DNCNT,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_DNCNT, 0, 3, DTP_EVT_DNCNT, 0,   
    },
    {
        0x03300D00,                                     //����Ǵ���
        0,
        NTRD_DATR_THEX | NTRD_DATR_BEXT, 
        ADDR_EVT_WGCNT,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_WGCNT, 0, 3, DTP_EVT_WGCNT, 0,   
    },
    {
        0x03300D01,                                     //�����ʱ��
        0,
        0x0000,
        ADDR_EVT_WGTIME,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_WGTIME, 0, 6, DTP_EVT_WGTIME, 0,
    },
    {
        0x03300400,                                     //Уʱ����
        0,
        NTRD_DATR_THEX | NTRD_DATR_BEXT, 
        ADDR_EVT_JSCNT,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_JSCNT, 0, 3, DTP_EVT_JSCNT, 0,   
    },
    {
        0x03300401,                                     //���һ��Уʱʱ��
        0,
        NTRD_DATR_SPEC,
        ADDR_EVT_JSTIME,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_JSTIME, 0, 12, DTP_EVT_JSTIME, (void*)ntrd_Fmt07JSTime,
    },
    {
        0x03300500,                                     //ʱ�α��̴��� 
        0,
        NTRD_DATR_THEX | NTRD_DATR_BEXT, 
        ADDR_EVT_DPCNT,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_DPCNT, 0, 3, DTP_EVT_DPCNT, 0,   
    },
    {
        0x03300501,                                     //ʱ�α���ʱ��
        0,
        0x0000,
        ADDR_EVT_DPTIME,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_DPTIME, 0, 6, DTP_EVT_DPTIME, 0,
    },
    {                                                   //�����������(XXXXXX)
        0x03300200,
        0,
        NTRD_DATR_THEX | NTRD_DATR_BEXT, 
        ADDR_EVT_DEMDCNT,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_DEMDCNT, 0, 3, DTP_EVT_DEMDCNT, 0,
    },
    {                                                   //�����������ʱ��(ssmmhhDDMMYY)
        0x03300201,
        0,
        0x0000, 
        ADDR_EVT_DEMDT,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_DEMDT, 0, 6, DTP_EVT_DEMDT,  0,
    },
    {                                                   //��һ������(DDhh)
        0x04000B01,
        0,
        0x0000, 
        ADDR_EVT_DH,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_DH, 0, 2, DTP_EVT_DH, 0,
    },
    {                                                   //�ڶ�������(DDhh)
        0x04000B02,
        0,
        0x0000, 
        ADDR_EVT_DH + 2,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_DH + 1, 0, 2, DTP_EVT_DH, 0,
    },
    {                                                   //����������(DDhh)
        0x04000B03,
        0,
        0x0000, 
        ADDR_EVT_DH + 4,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_DH + 2, 0, 2, DTP_EVT_DH, 0,
    },
    {                                                   //ʱ�η���
        0x04010001,
        0,
        0, 
        ADDR_EVT_FTN,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_FTN, 0, 24, DTP_EVT_FTN, 0,
    },
    {                                                   //�й������(XXXXXX)
        0x04000409,
        0,
        NTRD_DATR_THEX | NTRD_DATR_BEXT, 
        ADDR_EVT_IMP0,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_IMP0, 0, 3, DTP_EVT_IMP0, 0,
    },
    {                                                   //�޹������(XXXXXX)
        0x0400040A,
        0,
        NTRD_DATR_THEX | NTRD_DATR_BEXT, 
        ADDR_EVT_IMP1,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_IMP1, 0, 3, DTP_EVT_IMP1, 0,
    },
    {                                                   //A��������(XXXXXX)
        0x13010001,
        0,
        NTRD_DATR_SPEC,
        ADDR_EVT_DXN + 4,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_DXN, 0, 3, DTP_EVT_DXN, (void*)ntrd_FmtDxnA,
    },
    {                                                   //B��������(XXXXXX)
        0x13020001,
        0,
        NTRD_DATR_SPEC,
        ADDR_EVT_DXN + 8,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_DXN, 0, 3, DTP_EVT_DXN, (void*)ntrd_FmtDxnB,
    },
    {                                                   //C��������(XXXXXX)
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
        0x03110000,                                     //ͣ�����
        0,
        NTRD_DATR_THEX | NTRD_DATR_BEXT,
        ADDR_EVT_TDCNT,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_TDCNT, 0, 3, DTP_EVT_TDCNT, 0,
    },
    {
        0x10000001,                                     //ʧѹ����
        0,
        NTRD_DATR_THEX | NTRD_DATR_BEXT,
        ADDR_EVT_SYCNT,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_SYCNT, 0, 3, DTP_EVT_SYCNT, 0,
    },
    {
        0x03350000,                                     //�㶨�ų����Ŵ���
        0,
        NTRD_DATR_THEX | NTRD_DATR_BEXT,
        ADDR_EVT_CCOUNT,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_CCOUNT, 0, 3, DTP_EVT_CCOUNT, 0,
    },
    {
        0x04000307,                                     //��ѹ���������(XXXXXX)
        0,
        NTRD_DATR_THEX | NTRD_DATR_BEXT,
        ADDR_EVT_PTBB,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_PTBB, 0, 3, DTP_EVT_PTBB, 0,
    },
    {
        0x04000306,                                     //�������������(XXXXXX)
        0,
        NTRD_DATR_THEX | NTRD_DATR_BEXT,
        ADDR_EVT_CTBB,
        //zhtp, zhid, zhnum,
        0, 0, 1,
        //sbit, dgt, len, _r, func
        EFT_EVT_CTBB, 0, 3, DTP_EVT_CTBB, 0,
    },
    {                                                   //��ع���ʱ��(XXXXXXXX)
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
    {                                                   //����
        0x05060101,
        0,
        NTRD_DATR_THEX, 
        ADDR_EXT_WPD,
        //zhtp, zhid, zhnum,
        0, 0, 5,
        //sbit, dgt, len, _r, func
        EFT_EXT_WPD, 0, 4, DTP_EXT_WPD, 0,
    },
    {                                                   //����-W����ʾֵ����
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
    {                                                   //����-W����ʾֵ����
        0x0a,       //�ն���
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
 *������Լ������FN��Ӧ�����ʶ
 ************************************************************************/
typedef struct
{
    uint16  fn;
    uint16  dinum;                  //
    const uint16* dilist;           //���ֽ�Ϊdi����,���ֽ�Ϊ�������ݱ���
}FnMapToDI;


/************************************************************************
 *�¼�����������Ϣ
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
 *һ��ʵʱ����������Ϣ
 ************************************************************************/
//�ر���
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
 *������ʷ����������Ϣ(������97��07��ԼҪ������֡)
 ************************************************************************/
//�ر���
const uint16 gss_0DF0DataList[] = 
{
    DTP_WP,
    DTP_WN,
    DTP_EXT_TSD,
    DTP_EXT_WPD,
    DTP_EXT_WND,
};


//�ն�������
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

//�����ն�������
#define gss_0DF11DataList gss_0DF3DataList
#define gss_0DF12DataList gss_0DF4DataList

//�¶�������
#define gss_0DF19DataList gss_0DF3DataList
#define gss_0DF20DataList gss_0DF4DataList


//���ܼ���������ͳ��
const uint16 gss_0DF26DataList[] = 
{
    DTP_DMD_DPP,
    DTP_DMD_DPPT,
};
//�յ�ѹͳ��
const uint16 gss_0DF27DataList[] = 
{
    DTP_U,
};

//���ܼ���������ͳ��
#define gss_0DF34DataList gss_0DF26DataList
//�µ�ѹͳ��
#define gss_0DF35DataList gss_0DF27DataList


//��ѹ����
const uint16 gss_0DF89DataList[] = 
{
    DTP_U,
};
#define gss_0DF90DataList gss_0DF89DataList
#define gss_0DF91DataList gss_0DF89DataList

//��������
const uint16 gss_0DF92DataList[] = 
{
    DTP_I,
};
#define gss_0DF93DataList gss_0DF92DataList
#define gss_0DF94DataList gss_0DF92DataList

//�ն�������
#define gss_0DF185DataList gss_0DF3DataList
#define gss_0DF187DataList gss_0DF4DataList
//�����ն�������
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
 * @����: ����һ��������ݵĳ���������
 * 
 * @����: 
 * @param: dlh �����
 * @param: xlh С���
 * @param: output �������
 * 
 * @����: 
 * @return: uint8  �ɹ�/ʧ��
 * @˵��: 
 * @����: yzy (2012-8-21)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 LoadafnDICfg(uint8 dlh, uint8 xlh, DXLFnMap* output)
{
                                            //���������������
//	    memset((uint8*)output, 0, sizeof(DXLFnMap));
//	
//	    if((dlh > 15) || (xlh > 15))            //�Ƿ�֡,�˳�
//	    {
//	        return SYS_ERR_FT;
//	    }
//	                                            //1.�����С�������
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
//	                                            //�жϲ�����Ч��
//	        output->dlh = dlh;
//	        output->xlh = xlh;
//	        output->_r = GetTime()->hour;
//	    }
//	    //*******************************
//	                                            //2.���ݴ�С�������,����������FN��Ӧ�Ĳ���DI
//	    const FnMapToDI* ftdtbl;
//	    uint16 tbllen;
//	    uint8* afnmap;
//	    for(uint8 index = 0; index < 2; index++)
//	    {
//	        if(index == 0)                      //��������AFN0CFN�б�
//	        {
//	            tbllen = sizeof(gss_afn0CFnMapToDI) / sizeof(FnMapToDI);
//	            ftdtbl = gss_afn0CFnMapToDI;
//	            afnmap = output->afncmap;
//	        }
//	        else if(index == 1)                 //�������AFN0DFN�б�
//	        {
//	            tbllen = sizeof(gss_afn0DFnMapToDI) / sizeof(FnMapToDI);
//	            ftdtbl = gss_afn0DFnMapToDI;
//	            afnmap = output->afndmap;
//	        }
//	                                            //�����ն�֧�ֵ�FN�б�
//	        for(uint8 uc_i = 0; uc_i < tbllen; uc_i++, ftdtbl++)
//	        {                                   //���ڴ˴�С�����,��FN���Ƿ�����Ϊ����.(fn==0��ʾ��ѡ��)
//	            if((ftdtbl->fn == 0) || (STR_IsBitSet(afnmap, (ftdtbl->fn - 1))))
//	            {                               //��λ��FN��Ӧ�ı�ʶλ
//	                for(uint8 uc_j = 0; uc_j < ftdtbl->dinum; uc_j++)
//	                {
//	                    STR_SetBit(output->dimap, ftdtbl->dilist[uc_j]);
//	                }
//	            }
//	        }
//	    }
//	    //*******************************
//	                                            //3.�����¼�����,��������Ķ��ⲿ��DI
//	    tbllen = sizeof(gss_ercMapToDI) / sizeof(FnMapToDI);
//	    ftdtbl = gss_ercMapToDI;
//	                                            //����
//	    for(uint8 uc_i = 0; uc_i < tbllen; uc_i++, ftdtbl++)
//	    {                                       //���¼��Ƿ�ʹ��.erc��Χ��0��ʼ.
//	        if((ftdtbl->fn > 0) && STR_IsBitSet(gs_InpVarData.evtbit, (ftdtbl->fn - 1)))
//	        {                                   //��¼���¼�����Ҫ�����ĵ�DI
//	            for(uint8 uc_j = 0; uc_j < ftdtbl->dinum; uc_j++)
//	            {
//	                STR_SetBit(output->dimap, ftdtbl->dilist[uc_j]);
//	            }
//	        }
//	    }
//	                                            //����CRC
//	    BuildBufferCRC((uint8*)output, sizeof(DXLFnMap), true);
    SYS_OK();
}



/************************************************************************
 * @function: ResetafnDICfg
 * @����: ��λ�����С�������,��Ҫ����������
 * 
 * @����: 
 * 
 * @����: 
 * @return: 
 * @˵��: 
 * @����: yzy (2012-8-21)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void ResetafnDICfg(void)
{
    //ClearBuffer((uint8*)&gs_afnDICfg, sizeof(gs_afnDICfg));
}


/************************************************************************
 * @function: IsGatpDIRead
 * @����: �жϸñ�ʶ�Ƿ���Ҫ����
 * 
 * @����: 
 * @param: tno �������
 * @param: nrd �����ṹ������
 * 
 * @����: 
 * @return: uint8  0���ó���,1Ҫ����,2-0xFF�ӳٳ���(Ҫ������δ����������,�������ն���ʱ�䲻һ��ʱ,�ӳٳ������ն�������)
 * @˵��: �ж��Ƿ񳭶��Ļ���:�洢���Ƿ��Ѿ�������.
 * @����: yzy (2012-6-7)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 IsNetpDIRead(uint16 tno, NETPRD* nrd)
{
    return 1;
    /*
    uint8 dlh = gss_CDATATst[tno].dicfg >> 4;
    uint8 xlh = gss_CDATATst[tno].dicfg & 0x0F;
    
    //�˴���������С��ŵ��ж�
                                     //��������С�������
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
                                    //�жϸñ�ʶ�Ƿ����
    if(STR_IsBitSet(gs_afnDICfg.dimap, nrd->_r))
    {
        return 1;
    }
    //�˴���������С��ŵ��ж�

    return 0;
    */
}











