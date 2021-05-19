/****************************************Copyright (c)**************************************************
**                                  ________�Ƽ����޹�˾
**                                          ������
**
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: netp_fd.c
**��   ��   ��: yzy
**����޸�����: 2013��6��8��
**��        ��: �����˿�(�ǵ�������,Ƕ�뵽pst������)
**ע        ��:
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
**������: yzy
**��  ��: v1.0
**�ա���: 2013��6��8��
**�衡��: ԭʼ�汾
********************************************************************************************************/
#define EXT_NETP_FD
#include "es.h"
#include "hal.h"
#include "bsp.h"
#include "app.h"
#include "apis.h"
#include "extapi.h"


const uint8 err_addr[6] = {0xee, 0xee, 0xee, 0xee, 0xee, 0xee};


/************************************************************************
 * @function: MapSystitleToBgaddr
 * @����: ����systitle��������(ֻ�ܵõ���ŵ�8λ,��8λ����Ϊ��)
 * 
 * @����: 
 * @param: systitle ����,8�ֽ�
 * @param: bgaddr ���,6�ֽ�
 * 
 * @����: 
 * @return: uint8 Э������:3����,4����,0xFF�����쳣
 * @˵��: ����:systitle-48 58 45 01 30 D1 52 59.����:���:0x0D15259=13718105
 * @����: yzy (2014/6/2)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
/*
uint8 DLMS_MapSystitleToBgaddr(uint8* systitle, uint8* bgaddr)
{
    uint32 tmp;
                                        //ȡ��systitle�д����ŵ��ǲ���
    tmp = ByteToLong(systitle + 4);
    InvBuffer((uint8*)&tmp, sizeof(tmp));
    tmp &= 0x0FFFFFFF;
                                        //ת����bcd
    memset(bgaddr, 0, 6);
    HexToBcd(bgaddr, tmp, 5, 4);

    if(systitle[3] == 0x01)
    {
        return 3;
    }
    else if(systitle[3] == 0x03)
    {
        return 4;
    }
    else
    {
        return 0xff;
    }
}
*/

/************************************************************************
 * @function: CalcDlmsSaddr
 * @����: ���ݶ̵�ַ�����DLMSʵ�ʱ����еĵ�ַ��
 *
 * @����:
 * @param: saddr �̵�ַ
 *
 * @����:
 * @return: uint16 DLMS�����еĶ̵�ַ��
 * @˵��:
 * @����: yzy (2014/6/4)
 *-----------------------------------------------------------------------
 * @�޸���:
 ************************************************************************/
uint16 CalcDlmsSaddr(uint16 saddr)
{
    uint16 mtidx;
    uint16 tmp;
                                        //����HDLC�̵�ַ
    mtidx = saddr;
	mtidx <<= 1;
	tmp = mtidx & 0x00FF;
	mtidx <<= 1;
	mtidx &= 0xFE00;
	mtidx = (mtidx | tmp | 0x01);

    /*
    uint8 hfaddr = saddr >> 8;
    uint8 lfaddr = saddr;
    outbuffer[5] = ((hfaddr << 1) + (lfaddr >> 7)) << 1;
    outbuffer[6] = (lfaddr << 1) + 0x01;
    */

    return mtidx;
}


/************************************************************************
 * @function: DLMS_AppReadPack
 * @����: dlmsӦ�ò�����֡���(����0x7E֡ͷ,0x7E֡β,CRC)
 *
 * @����:
 * @param: indata Ӧ�ò�������
 * @param: inlen Ӧ�ò����ݳ���
 * @param: outbuffer ���֡����
 * @param: saddr �̵�ַ
 *
 * @����:
 * @return: uint8  ����õĳ���
 * @˵��:
 * @����: yzy (2014/6/4)
 *-----------------------------------------------------------------------
 * @�޸���:
 ************************************************************************/
uint8 DLMS_AppReadPack(uint8* indata, uint8 inlen, uint8* outbuffer, uint16 saddr)
{
    uint16 crc;
    uint16 mtidx;
                                        //����HDLC�̵�ַ
    mtidx = CalcDlmsSaddr(saddr);
                                        //����������
    MoveBuffer(indata, outbuffer + 14, inlen);
                                        //����֡ͷ
    outbuffer[0] = 0x7E;
    outbuffer[1] = 0xA0;                //frame type(�޺���֡)
    outbuffer[1] |= (inlen + 15) >> 8;  //length_1
    outbuffer[2] = (inlen + 15) & 0xFF; //length_2
    outbuffer[3] = 0x00;                //�̶�(Ŀ���ַһ����)
    outbuffer[4] = 0x02;                //�̶�(Ŀ���ַһ����)
    outbuffer[5] = mtidx >> 8;          //hdlc�̵�ַ(Ŀ���ַһ����)
    outbuffer[6] = mtidx;               //hdlc�̵�ַ(Ŀ���ַһ����)

    outbuffer[7] = 0x03;                //�̶�,Դ��ַ
    outbuffer[8] = 0x54;                //��֪����ʲô����.
                                        //У��֡ͷcrc
    crc = DoCrcDlms(outbuffer + 1, 8, 0xffff);
    outbuffer[9] = (uint8)crc;
    outbuffer[10] = (uint8)(crc >> 8);
                                        //����������ݲ���ȫ֡crc
    outbuffer[11] = 0xE6;
    outbuffer[12] = 0xE6;
    outbuffer[13] = 0;
    crc = DoCrcDlms(outbuffer + 1, 14 + inlen - 1, 0xffff);
    outbuffer[14 + inlen] = (uint8)crc;
    outbuffer[15 + inlen] = (uint8)(crc >> 8);
    outbuffer[16 + inlen] = 0x7E;
                                        //�����ܳ���
    return inlen + 17;
}




/************************************************************************
 * @function: PST_DLMS_HDLC_Decode
 * @����: �жϵ�ǰ���������Ƿ�Ϊ�Ϸ�֡,����ǵĻ��������Ϊ����ͨѶ�ṹ��
 *
 * @����:
 * @param: receive ���յ����ݵĻ���
 * @param: send ��Ҫ���͵����ݵĻ���
 * @param: sbuflen ���ͻ��泤��
 * @param: frame ���ݽ����ɵĽṹ��
 *
 * @����:
 * @return: uint8
 * @˵��:
 * @����: yzy (2014/5/27)
 *-----------------------------------------------------------------------
 * @�޸���:
 ************************************************************************/
uint8 PST_DLMS_HDLC_Decode(uint8* recv, uint8* send, uint16 sbuflen, PST_Frame* frame)
{
    DLMS_Frame* frm = &frame->_dlms;

    #if (ES_MNT_EN > 0)
    ES_MNT_Puts("\r\nreceive valid dlms frame\r\n");
    #endif

    frm->cmd = recv[14];
    frm->recv = recv + 15;
    frm->send = send;
    
    frm->len = recv[2] + ((recv[1] & 0x7f) << 8) - 2 - 14;        //2�ֽ�crc,14�ֽ�(Byte1-ByteCmd).

    ES_OK();
}




/************************************************************************
 * @function: PST_DLMS_HDLC_Proc
 * @����:
 *
 * @����:
 * @param: frm ���ݽ����ɵĽṹ��
 *
 * @����:
 * @return: uint8  PST_ERR_OK/PST_ERR_FT
 * @˵��:
 * @����: yzy (2014/5/27)
 *-----------------------------------------------------------------------
 * @�޸���:
 ************************************************************************/
uint8 PST_DLMS_HDLC_Proc(PST_Frame* frame)
{
    DLMS_Frame* frm = &frame->_dlms;

    //**********************************
                                        //����,ִ��DLMS�ж�
    uint8 ZY_DlmsTransRecvProc(uint8* buffer, uint16 length);
    if(ES_ERR_OK == ZY_DlmsTransRecvProc(frm->recv - 15, frm->len + 2 + 14 + 2))
    {
        return ES_ERR_OK;               //�ѱ�͸������������,����Ҫ���к�������
    }

    //**********************************
    if(frm->cmd == 0x1E)                //discover response
    {
        uint8 Netp_DiscoUpRecvProc(uint8* data);
        Netp_DiscoUpRecvProc(frm->recv);
    }
    else
    {
        //����֡������,plcͨѶ֡ʹ�õȴ�����
    }
    ES_OK();
}




//�������ѱ�λ֡
const uint8 dlms_scm_reset[27] =
{
    0x7E,0xA0,0x19,0x00,0x02,0xFE,0xFF,0x03,
    0x13,0xF1,0x0B,0xE6,0xE6,0x00,0x19,0x08,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x7B,0x0C,0x7E
};


//����֡����(�°汾)
const uint8 dlms_scm_discover[15] =
{
    0x7E,0xA0,0x15,0x00,0x02,0xFE,0xFF,0x03,
    0x13,0x6B,0xBA,0xE6,0xE6,0x00,0x1D
};

const uint8 dlms_scm_tst_discover[23]=
{
   0x7E,0xA0,0x15,0x00,0x02,0xFE,0xFF,0x03,
   0x13,0x6B,0xBA,0xE6,0xE6,0x00,0x1D,0x64,
   0x00,0x19,0x00,0x00,0x53,0x99,0x7E
};
//ע��֡����
const uint8 dlms_scm_register[24] =
{
    0x7E,0xA0,0x23,0x00,0x02,0xFE,0xFF,0x03,
    0x13,0xEC,0x34,0xE6,0xE6,0x00,0x1C,0xFE,
    0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0x01
};


//�����֡
const uint8 dlms_scm_getid[30] =
{
    0x7E,0xA0,0x1C,0x00,0x02,0xFE,0xFF,0x03,
    0x32,0xD9,0xCB,0xE6,0xE6,0x00,0xC0,0x01,
    0xC1,0x00,0x01,0x00,0x00,0x60,0x01,0x00,
    0xFF,0x02,0x00,0x89,0xA0,0x7E
};

//#ifdef DEBUG
//const uint8 systitle_examp1[8] = {0x48, 0x58, 0x45, 0x01, 0x30, 0xD1, 0x52, 0x59};
//const uint8 systitle_examp2[8] = {0x48, 0x58, 0x45, 0x01, 0x30, 0xD1, 0x52, 0x60};
//#endif

/************************************************************************
 * @function: DLMS_ClearRegisMt
 * @����: ���ע��ڵ���Ϣ��
 * @����: kpmapλͼ,������һ�Ľڵ�,ɾ������Ľڵ�.
 * @����:
 * @˵��:
 * @����: yzy (2014/5/27)
 *-----------------------------------------------------------------------
 * @�޸���:
 ************************************************************************/
void DLMS_ClearRegisMt(uint8* kpmap)
{
    uint8 bidx;
    if(kpmap == __NULL)
    {
        ClearBuffer((uint8*)&gs_dlmsMt, sizeof(gs_dlmsMt));
    }
    else
    {
        STR_And(gs_dlmsMt.eft, kpmap, gs_dlmsMt.eft, sizeof(gs_dlmsMt.eft));
        //STR_Or(gs_dlmsMt.eft, kpmap, gs_dlmsMt.eft, sizeof(gs_dlmsMt.eft));
    }
    
    if(STR_CalcBitSetNum(gs_dlmsMt.eft, DLMS_MT_MAP) >= CORE_TST_NO)//���ִ洢λͼ����
    {
        for(bidx = 0; bidx < CORE_TST_NO; bidx++)
        {
            if(STR_IsBitClr(kpmap, bidx))//�����ѱ�λͼ�Ƿ���ڿ�λ
            {
                STR_ClrBit(gs_dlmsMt.eft, bidx);    //���洢λͼ���
            }
        }
    }
    
//#ifdef DEBUG
//    MoveBuffer((uint8*)systitle_examp1, gs_dlmsMt.mt[0].systitle, 8);
//    MoveBuffer((uint8*)systitle_examp2, gs_dlmsMt.mt[1].systitle, 8);
//    gs_dlmsMt.eft[0] = 0x03;
//#endif
}

/************************************************************************
 * @function: DLMS_ClrRegisMt
 * @����: ����ѱ���
 * @����: 
 * @����: 
 * @˵��: 
 * @����: ZJC (2014/8/21)
 *-----------------------------------------------------------------------
 ************************************************************************/
void DLMS_ClrRegisMt(void)
{
    ClearBuffer((uint8*)&gs_dlmsMt,sizeof(gs_dlmsMt));  //���������
    WriteFlashWithCRC(DB_MTLIST, (uint8*)&gs_dlmsMt, sizeof(gs_dlmsMt), 0);
    ClearBuffer((uint8*)&gs_scmDlms, sizeof(gs_scmDlms));
    
    //ClearBuffer((uint8*)&gs_NetpReadStEVT, sizeof(gs_NetpReadStEVT));       //���¼�ȫ�����
    //Fram_SafeWrite((uint8 *)&gs_NetpReadStEVT, FM_EVTDT, sizeof(gs_NetpReadStEVT));   //������д�¼�ʱ��
}

/************************************************************************
 * @function: DLMS_SaveRegisMt
 * @����: �����¹ұ���Ϣ�б�
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/7/1)
 *-----------------------------------------------------------------------
 * @�޸���: ZJC(2014-08-21)
 * @˵��: ���ѱ������뵽F10��Ƶ���
 ************************************************************************/
void DLMS_SaveRegisMt(void)
{
    bool ucNewFindFlag;
    uint8 uc_i,uc_j;
    uint8 defparaf10[LEN_TST_CFG];
    uint8 bmapsbf[LEN_TST_BMPS];
    uint8 bmaps[LEN_TST_BMPS];
    uint32 addr;
    
    gs_GPIO.LED_BlinkSet(LED_MBUS, 3, 0, 0);  //��
    if(STR_IsEmpty(gs_dlmsMt.eft, DLMS_MT_MAP))//���û�з����κ��±����ٴ��ѱ�
    {
        ES_Message_Send(MSG_NETP_DISCOSTART, TASK_NETP_TKID);
        return;
    }
    
    gb_dlmsMtFlashCrcOk = WriteFlashWithCRC(DB_MTLIST, (uint8*)&gs_dlmsMt, sizeof(gs_dlmsMt), 0);//д�ѱ���
    
    GD_Para_RW(F10_PADDR, bmaps, LEN_TST_BMPS, false);   //��������λͼ
    ClearBuffer(bmapsbf,LEN_TST_BMPS);
    for(uc_i = 0; uc_i < CORE_TST_NO; uc_i++)//ɨ���ѱ���
    {
        if(STR_IsBitSet(gs_dlmsMt.eft, uc_i) &&                                     //�ѱ���Ч
           (CmpBuffer(gs_dlmsMt.mt[uc_i].bgaddr, (uint8*)&DefParaF10[6], 6) != 0))  //�ѱ��ַΪ��0
        {
            STR_SetBits(bmapsbf, 1, uc_i, 1);  //����λͼ
            ucNewFindFlag = true;
            for(uc_j = 2; uc_j < CORE_TST_NO; uc_j++)//ɨ�賭����
            {
                addr = F10_PADDR + LEN_TST_BMPS + LEN_TST_CFG * uc_j;
                GD_Para_RW(addr, defparaf10, LEN_TST_CFG, false);   //��������ַ
                if(STR_IsBitSet(bmaps, uc_j) &&                                     //������Ч
                   (CmpBuffer(gs_dlmsMt.mt[uc_i].bgaddr, defparaf10 + 6, 6) == 0) )   //��ַ��ͬ
                {
                    ucNewFindFlag = false;//�ñ�ĵ����Ѿ�����
                    break;
                }
            }
            if(ucNewFindFlag == true)   //�������±�
            {
                MoveBuffer((uint8*)DefParaF10, defparaf10, LEN_TST_CFG);//����Ĭ�ϲ���
                MoveBuffer(gs_dlmsMt.mt[uc_i].bgaddr, defparaf10 + 6, 6);  //�����
                
                MoveBuffer(gs_PstPara.Addr + 2, defparaf10 + 20, 2);  //��д�����ɼ���ͨ�ŵ�ַ
                MoveBuffer(gs_PstPara.Addr, defparaf10 + 22, 2);  
                
                if(gc_FindMtMode == 1)  //�Զ�����F10����
                {
                    for(uc_j = 2; uc_j < CORE_TST_NO; uc_j++)//ɨ�賭����
                    {
                        if(STR_IsBitClr(bmaps, uc_j))   //�ҵ���λ
                        {
                            defparaf10[0] = uc_j;
                            defparaf10[2] = uc_j;
                            addr = F10_PADDR + LEN_TST_BMPS + LEN_TST_CFG * uc_j;
                            GD_Para_RW(addr, defparaf10, LEN_TST_CFG, true);   //дF10������ַ
                            ParamLoad_F10();    //���뵵����Ϣ
                            ALRT_ERC39_46(uc_j, uc_j, defparaf10, 39);   //�����±�
                            break;
                        }
                    }
                }
                else 
                {
                    ALRT_ERC39_46(9999, 9999, defparaf10, 39);   //�����±�
                }
            }
        }   
    }
    
    if(STR_IsEmpty(bmapsbf, LEN_TST_BMPS))  //����λͼΪ�գ����ٴ��ѱ�
    {
        ES_Message_Send(MSG_NETP_DISCOSTART, TASK_NETP_TKID);
    }
    else
    {
        ES_Message_Send(MSG_NETP_IDATA, TASK_NETP_TKID);//�ѱ���ٽ����ϵ�����ֳ���
    }
}


/************************************************************************
 * @function: DLMS_LoadRegisMt
 * @����: �����¹ұ���Ϣ�б�
 * @����: 
 * 
 * @����: 
 * @return: bool  ��:ִ�гɹ�, ��:�������
 * @˵��: 
 * @����: yzy (2014/7/1)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
bool DLMS_LoadRegisMt(void)
{
    gb_dlmsMtFlashCrcOk = ReadFlashWithCRC(DB_MTLIST, (uint8*)&gs_dlmsMt, sizeof(gs_dlmsMt), 0);
    return gb_dlmsMtFlashCrcOk;
}





/************************************************************************
 * @function: DLMS_ResetRegisSend
 * @����: ��λ�ѱ�ע��״̬
 * 
 * @����: 
 * @param: pdata 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/5/27)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void DLMS_ResetRegisSend(void)
{
    gfs_PSTChnSend[PST_CHN_NET]((uint8*)dlms_scm_reset, sizeof(dlms_scm_reset));
}




/************************************************************************
 * @function: DLMS_DiscoverSend
 * @����: �ѱ���֡
 * 
 * @����:
 * @param: pdata �ⲿ����ķ��ͻ���,Ŀ�����ڼ���ջ���
 * @param: ratio �ϱ�����(0-100)
 * @param: slots �ϱ�ʱ��Ƭ(��λ1ms)
 * @����: uint8 ֡����
 * @˵��: 
 * @����: yzy (2014/5/27)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 DLMS_DiscoverSend(uint8* pdata, uint8 ratio, uint16 slots)
{
    uint16 crc;
    uint8 len;
                                            //�����Ϸ����ж�
    if(ratio > 100)
    {
        ratio = 100;
    }
                                            //֡ͷ
    MoveBuffer((uint8*)dlms_scm_discover, pdata, sizeof(dlms_scm_discover));
    len = sizeof(dlms_scm_discover);
                                            //�ϱ�����&ʱ��Ƭ
    pdata[len++] = ratio;
    pdata[len++] = (uint8)(slots >> 8);
    pdata[len++] = (uint8)slots;
    pdata[len++] = 0;
    pdata[len++] = 0;
                                            //crc
    crc = DoCrcDlms(pdata + 1, len - 1, 0xffff);
    pdata[len++] = (uint8)crc;
    pdata[len++] = (uint8)(crc >> 8);
    pdata[len++] = 0x7E;
                                            //����
    gfs_PSTChnSend[PST_CHN_NET](pdata, len);
                                            //���س���
    return len;
}







/************************************************************************
 * @function: DLMS_RegisSend
 * @����: �ѱ�ע��֡
 * 
 * @����:
 * @param: pdata �ⲿ����ķ��ͻ���,Ŀ�����ڼ���ջ���
 * @param: systitle ���ֽ�systemtitle
 * @param: saddr �߼���ַ,�̵�ַ
 * 
 * @����: 
 * @return: uint8  ֡����
 * @˵��: 
 * @����: yzy (2014/5/27)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 DLMS_RegisSend(uint8* pdata, uint8* systitle, uint16 saddr)
{
    uint16 crc;
    uint8 len;
                                            //֡ͷ
    MoveBuffer((uint8*)dlms_scm_register, pdata, sizeof(dlms_scm_register));
    len = sizeof(dlms_scm_register);
                                            //������
    MoveBuffer(systitle, pdata + len, 8);
    len += 8;
    pdata[len++] = (uint8)(saddr >> 8);
    pdata[len++] = (uint8)saddr;
                                            //crc
    crc = DoCrcDlms(pdata + 1, len - 1, 0xffff);
    pdata[len++] = (uint8)crc;
    pdata[len++] = (uint8)(crc >> 8);
    pdata[len++] = 0x7E;
                                            //���Ͳ��ط�
    gfs_PSTChnSend[PST_CHN_NET](pdata, len);
    gs_OS.TK_Sleep(100);
    gfs_PSTChnSend[PST_CHN_NET](pdata, len);
    gs_OS.TK_Sleep(100);
                                            //���س���
    return len;
}




/************************************************************************
 * @function: DLMS_GetMtId
 * @����: ��ȡ���
 * 
 * @����:
 * @param: pdata �ⲿ����ķ��ͻ���,Ŀ�����ڼ���ջ���
 * @param: pdmax ���ͻ��泤��
 * @param: saddr ����,�߼���ַ,�̵�ַ
 * @param: outaddr ���,���
 * 
 * @����: 
 * @return: uint8  �ɹ�/ʧ��
 * @˵��: ����: �ظ�֡ 7ea0210300020005300778e6e700c40101000a0c323031343333333335353031686b7e
 *            ����,���Ϊ 0a0c323031343333333335353031
 * @����: yzy (2014/7/1)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 DLMS_GetMtId(uint8* pdata, uint8 pdmax, uint16 saddr, uint8* outaddr, uint8 trycnt)
{
    uint16 crc;
    uint16 dlen;
    uint8 tmp;
    uint16 mtidx;
                                        //���Ͳ���ȡ��ַ
    for(uint8 uc_i = 0; uc_i < trycnt; uc_i++)//����N��
    {
                                        //�������֡.
        MoveBuffer((uint8*)dlms_scm_getid, pdata, sizeof(dlms_scm_getid));
                                        //�滻Ŀ���ַ
        mtidx = CalcDlmsSaddr(saddr);
        pdata[5] = mtidx >> 8;          //hdlc�̵�ַ
        pdata[6] = mtidx;               //hdlc�̵�ַ
                                        //У��֡ͷcrc
        crc = DoCrcDlms(pdata + 1, 8, 0xffff);
        pdata[9] = (uint8)crc;
        pdata[10] = (uint8)(crc >> 8);
                                        //ȫ֡crc
        tmp = pdata[2] - 15;            //���Ƴ����ݳ���
        crc = DoCrcDlms(pdata + 1, 14 + tmp - 1, 0xffff);
        pdata[14 + tmp] = (uint8)crc;
        pdata[15 + tmp] = (uint8)(crc >> 8);
        pdata[16 + tmp] = 0x7E;
    
        gs_Uart.open(NETP_PORT_NO, false);
        ES_LED_BlinkSet(LED_MBUS, 1, 0, 0);
        gs_Uart.Write(NETP_PORT_NO, pdata, sizeof(dlms_scm_getid));
        dlen = WaitForDlms(pdata, pdmax, __NULL, NETP_PORT_NO);
        gs_Uart.close(NETP_PORT_NO);
                                         //��ȡ��ź͹�Լ
        if(dlen > 32)
        {
            //ES_LED_BlinkSet(LED_MBUS, 1, 0, 0);
            dlen -= 23;                 //��ȡ����ֽ���:��ȥ20�ֽڱ��ǰ���� + 3�ֽ�crc
            if((dlen > 0) && (dlen <= 12))//���λ���ɱ�:1-12λ
            {
                if(dlen != 12)
                {
                    MoveBuffer(pdata + 20, pdata + 20 + (12 - dlen), dlen);
                    memset(pdata + 20, 0x30, (12 - dlen));
                }
                HexStringToNum(pdata + 20, outaddr, 12);
                InvBuffer(outaddr, 6);
                gs_OS.TK_Sleep(50);     //��ʱ500ms,��ֹĳЩ���ȱ�Ʊ�����
                return ES_ERR_OK;
            }
            gs_OS.TK_Sleep(50);         //��ʱ500ms,��ֹĳЩ���ȱ�Ʊ�����
        }
    }

    return ES_ERR_FT;

}




//�����˿�Ӳ������֡.
const uint8 netp_hwtest_frm1[] = 
{
    0x7E,
    0xA0,0x23,
    0x00,0x02,
    0xFE,0xFF,
    0x03,
    0x93,
    0xE4,0xB0,

    0x81,0x80,0x14,0x05,0x02,0x07,0xD0,0x06,0x02,0x07,0xD0,0x07,0x04,0x00,0x00,0x00,0x01,0x08,0x04,0x00,0x00,0x00,0x01,
    0x3A,0xF2,
    0x7E
};


const uint8 netp_hwtest_frm2[] = 
{
    0x68, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x68, 0x13, 0x00, 0xDF, 0x16
};
/************************************************************************
 * @function: Netp_HwTest
 * @����: ����ͨ��Ӳ������.
 * @����: 
 * 
 * @����: 
 * @return: uint8  ES_ERR_OK/ES_ERR_FT
 * @˵��: ��������,�¹ҵ���DLMS��.
 * @����: yzy (2014/6/4)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 Netp_HwTest(void)
{
    uint16 dlen;
    uint8 rbuf[64];

    //ʹ��DLMS֡��̽
    gs_Uart.open(NETP_PORT_NO, false);
    ES_LED_BlinkSet(LED_MBUS, 1, 0, 0);
    gs_Uart.Write(NETP_PORT_NO, (uint8*)netp_hwtest_frm1, sizeof(netp_hwtest_frm1));
    dlen = WaitForDlms(rbuf, sizeof(rbuf), __NULL, NETP_PORT_NO);
    gs_Uart.close(NETP_PORT_NO);

//    if(dlen)
//    {
//        ES_LED_BlinkSet(LED_RXD, 0, 0, 0);
//        return ES_ERR_OK;
//    }
//
//    //ʹ��DLT֡��̽
//    gs_Uart.open(ES_NET_PORT, false);
//    ES_LED_BlinkSet(LED_TXD, 0, 0, 0);
//    gs_Uart.Write(ES_NET_PORT, (uint8*)netp_hwtest_frm2, sizeof(netp_hwtest_frm2));
//    dlen = WaitForBg(rbuf, sizeof(rbuf), __NULL, ES_NET_PORT);
//    gs_Uart.close(ES_NET_PORT);

    if(dlen)
    {
        ES_LED_BlinkSet(LED_MBUS, 1, 0, 0);
        return ES_ERR_OK;
    }
    else
    {
        return ES_ERR_FT;
    }
    
}




/************************************************************************
//�ѱ��ϱ�֡,��֡����<48�ֽ�.����48�ֽ�һ֡����,��֡�ϱ�ʱ��10ms. 
//��ʹ�������16������,�����ʱ2000msӦ�ù���. 
//�ѱ����:0)���ע��״̬(��������)
//         1)50%,2s�ѱ�,ֱ���ޱ��ϱ�.(�����������ϱ������ע��.)
//         2)100%,2s�ѱ�,ֱ������3���ޱ��ϱ�.(�����������ϱ������ע��.)
 ************************************************************************/


/************************************************************************
 * @function: Netp_DiscoCheckExist
 * @����: �ѱ�ǰ����֤��ע��ı��Ƿ���Ȼ����.
 *          (���ѱ�RESETǰ����.������,����ӵ��ѱ��ֽڵ�λͼ��.)
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/7/1)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void Netp_DiscoCheckExist(void)
{
    uint8 buffer[48];
    uint8 bgaddr[6];
    uint8 bidx;
    uint8 errnum = 0;                   //�Ѵ��ڽڵ�������֤ʧ�ܸ���

    //����systitle�Ƿ��Ѿ��м�¼
    for(bidx = 0; bidx < CORE_TST_NO; bidx++)
    {
        if(STR_IsBitSet(gs_dlmsMt.eft, bidx))
        {                               //���ݶ̵�ַ��ȡ���
            if(ES_ERR_OK == DLMS_GetMtId(buffer, sizeof(buffer), bidx + DLMS_SADDR_OFFSET, bgaddr, 2))
            {                           //�ȶԱ��
                if(!CmpBuffer(bgaddr, gs_dlmsMt.mt[bidx].bgaddr, 6))
                {                       //���һ��,���뵽�ѷ����б���(����ͬ Netp_DiscoUpRecvProc())
                    STR_SetBit(gs_scmDlms.bmap, bidx);
                }
                errnum = 0;
            }
            else                        //��ֹ�ֳ������ɼ���ʱ,�ϵ��ѱ�ʱ�����(������24�����֤һ����Ҫ��3����)
            {
                errnum++;
                if(errnum >= 5)         //����n�����֤ʧ��,����Ϊԭ�м�¼������,ֹͣ������֤.
                {
                    break;
                }
            }
        }
    }
}



/************************************************************************
 * @function: Netp_DiscoStart
 * @����: �����ӽڵ�����ע������
 * @����:
 * @����:
 * @˵��:
 * @����: yzy (2013/7/12)
 *-----------------------------------------------------------------------
 * @�޸���:
 ************************************************************************/
void Netp_DiscoStart(void)
{
    scm_dlms_t* scm = &gs_scmDlms;
                                        //������ִ���ѱ�,��ֱ���˳�
    if(((scm->stt & SCM_STT_EXMASK) != SCM_STT_IDLE) && 
       ((scm->stt & SCM_STT_EXMASK) != SCM_STT_FIN))
    {
        return;
    }
    ES_LED_BlinkSet(LED_MBUS, 1, 0, 0);
                                        //1)
    DLMS_LoadRegisMt();                 //�����ϴ��ѱ���
    ClearBuffer((uint8*)scm, sizeof(scm_dlms_t));
                                        //2)
    scm->stt &= ~SCM_STT_EXMASK;
    scm->stt |= SCM_STT_CKEST;
    Netp_DiscoCheckExist();             //��֤֮ǰ�Ľڵ��Ƿ���Ȼ����,�Դ��ڵ�ֱ����Ϊ�״��ѱ��ϱ��ڵ�
                                        //3)
    DLMS_ClearRegisMt(gs_scmDlms.bmap); //����ӽڵ����ϱ����.

                                        //4)��λ���нڵ��ע��״̬
    gs_OS.TK_Sleep(50);                 //��ʱ500ms
    for(uint8 uc_i = 0; uc_i < 3; uc_i++)
    {
        scm->stt &= ~SCM_STT_EXMASK;
        scm->stt |= SCM_STT_RESET;
        DLMS_ResetRegisSend();
        gs_OS.TK_Sleep(200);
    }
                                        //��ʱ����
    scm->vcnt = 1;                      //n���ִ��
    scm->startsecs = GetSysRunSecs();
    scm->stt &= ~SCM_STT_EXMASK;
    scm->stt |= SCM_STT_DISCO;          //����ñ��λ
}



/*
���� -  ˵: (2014-06-10 12:39:22)
�޹���ô?
���ڷ���DLMS���ѱ�֡�е�time slotsò��û��.
���æ����,�ɼ����¹�14���,�����õ���100%�ϱ�����,time slots = 200.

�������¹ұ�Ӧ��200*40ms = 8s������ϱ�.
ʵ�ʼ��ӷ���,�¹ұ�ÿ��ֻ�ڷ������ݵ�ǰ2�����ϱ�

���� -  ˵: (2014-06-10 12:39:33)
7EA0150002FEFF03136BBAE6E6001D6400C80000804C7E
�������·���discover֡

�ޱ� -  ˵: (2014-06-10 15:01:18)
��ʵ�ʴ����ʱ��,ֻ����time slots 2���ֽ��еĵ��ֽ�
�ޱ� -  ˵: (2014-06-10 15:04:11)
��һ���ֽڴ���0���ߵ�2���ֽڴ���60��ʱ��,�ظ���ʱ������������2s����~
*/
/************************************************************************
 * @function: Netp_FindSecProc
 * @����: �ӽڵ�����ע�����̿���
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2013/7/12)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void Netp_FindSecProc(void)
{
    scm_dlms_t* scm = &gs_scmDlms;
    uint8 buffer[64];
    uint16 bidx;
                                        //ע�ᷢ���ѽ�������δ��ʼ,�˳�
    if(((scm->stt & SCM_STT_EXMASK) == SCM_STT_FIN) || 
       ((scm->stt & SCM_STT_EXMASK) == SCM_STT_IDLE))
    {
        return;
    }
                                        //�ȴ�ֱ����ʱʱ�����.
    if(scm->vcnt > 0)
    {
        scm->vcnt--;
        return;
    }
                                        //�ж��ѱ�������ʱ��,����˵�������쳣���,ǿ��ֹͣ.
    if(GetSysRunSecs() >= (scm->startsecs + 600))
    {
        scm->stt &= ~SCM_STT_EXMASK;
        scm->stt |= (SCM_STT_FIN | SCM_STT_FIROK);
        scm->err = 0xFF;
        DLMS_SaveRegisMt();
        return;
    }
                                        //���������ѱ������в���
    bidx = STR_GetFirstSetBit(scm->bmap, sizeof(scm->bmap));
    if(bidx == 0xFFFF)                  //�޴�ע���
    {                                   //100%������,����5�����ϱ�,����Ϊ���.
        if((scm->ratio == 100) && (scm->nofd >= 5))
        {
            scm->stt &= ~SCM_STT_EXMASK;
            scm->stt |= (SCM_STT_FIN | SCM_STT_FIROK);
            DLMS_SaveRegisMt();
            return;
        }
                                        //1)����һ��
        scm->nofd++;
                                        //2)����discover����
        if(scm->ratio >= 100)           //���ʵ��ٷְ�,��������
        {
            scm->ratio = 100;
        }
        else if(scm->ratio == 0)        //����Ϊ��,���ʼ������
        {
            scm->ratio = 10;
            scm->nofd = 0;              //��
        }
        else                            //������0-100֮��
        {
            //���ٷ���
            if(scm->nofd >= 2) //����δ���յ�����,&||,����n��δ�յ���Ч֡ʱ,���Ӹ���
            {
                scm->ratio += 20;       //>100û��ϵ,���Զ��޶���100
                scm->nofd = 0;          //��
            }
            
            
            /*//�Ƚ�����
            if(scm->nofd >= 3) //����δ���յ�����,&||,����n��δ�յ���Ч֡ʱ,���Ӹ���
            {
                if(scm->ratio < 70)
                {
                    scm->ratio += 20;
                }
                else
                {
                    scm->ratio += 10;
                }
                scm->nofd = 0;          //��
            }*/
            
        }
        
        scm->slots = 50;                //50*40=2000ms
        scm->vcnt = 3;
        scm->ndnum = 0;
        scm->revd = 0;
        ClearBuffer(scm->bmap, sizeof(scm->bmap));
                                        //����
        scm->stt &= ~SCM_STT_EXMASK;
        scm->stt |= SCM_STT_DISCO;
        DLMS_DiscoverSend(buffer, scm->ratio, scm->slots);
    }
    else                                //�д�ע���
    {
        scm->stt &= ~SCM_STT_EXMASK;
        scm->stt |= SCM_STT_REGIS;
                                        //ע�����
        DLMS_RegisSend(buffer, gs_dlmsMt.mt[bidx].systitle, bidx + DLMS_SADDR_OFFSET);  //�߼���ַ��2��ʼ����.
                                        //ʹ��ע����߼���ַ��ȡ���
        if(ES_ERR_OK != DLMS_GetMtId(buffer, sizeof(buffer), bidx + DLMS_SADDR_OFFSET, gs_dlmsMt.mt[bidx].bgaddr, 3))
        {                               //����,�ܿ����Ǳ��ڱ�����,����MBUS����
            MoveBuffer((uint8*)err_addr, gs_dlmsMt.mt[bidx].bgaddr, 6);
        }

        gs_OS.TK_Sleep(50);             //��ʱ500ms
                                        //��������
        scm->vcnt = 2;                  //����register�ȴ���ʱ
        scm->nofd = 0;
        STR_ClrBit(scm->bmap, bidx);    //��λͼ
    }

}



/************************************************************************
 * @function: Netp_DiscoUpRecvProc
 * @����: ���յ������ϱ����ֽڵ�ʱ�Ĵ�����
 * 
 * @����: 
 * @param: data ������(DLMS������֮�������) 
 * 
 * @����: 
 * @return: uint8  �ɹ�/ʧ��
 * @˵��: 
 * @����: yzy (2013/7/13)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 Netp_DiscoUpRecvProc(uint8* data)
{
    scm_dlms_t* scm = &gs_scmDlms;
    uint16 bidx;
    uint8 isExist = 0;
    
    //����systitle�Ƿ��Ѿ��м�¼
    for(bidx = 0; bidx < CORE_TST_NO; bidx++)
    {
        if(STR_IsBitSet(gs_dlmsMt.eft, bidx))
        {
            if(0 == CmpBuffer(gs_dlmsMt.mt[bidx].systitle, data + 1, 8))
            {                   //�Ѿ�����,����ѭ��
                isExist = 1;
                break;
            }
        }
    }
    
    //��û�м�¼,���
    if(!isExist)
    {
        bidx = STR_GetFirstClrBit(gs_dlmsMt.eft, DLMS_MT_MAP);
        if(bidx == 0xFFFF)
        {
            return ES_ERR_FT;
        }
                                //��ֵsystitle����Ч��־
        MoveBuffer(data + 1, gs_dlmsMt.mt[bidx].systitle, 8);
        MoveBuffer((uint8*)err_addr, gs_dlmsMt.mt[bidx].bgaddr, 6);//��Ч
        STR_SetBit(gs_dlmsMt.eft, bidx);//ע��:��ʱ��Ȼ��λ��Ч,��bgaddr��Ϊȫ��,��ʵ��ֵ
    }
    else
    {
        //�쳣״̬:�Ѿ���ע��ı�,�ٴ��ϱ�.
        //����ԭ��1)���쳣,��ʧע��״̬,2)�����쳣,�ѵ�systitle���ע��֡�ղ���,3)MBUS��·�쳣
    }
                                //��ֵ����״̬��Ч��־
    STR_SetBit(scm->bmap, bidx);
    ES_OK();
}



/************************************************************************
 * @function: Netp_DiscoSetRecvFlag
 * @����: ���ֹ����н��յ�MBUS����ʱ�ý��ձ�־λ
 * 
 * @����: 
 * @param: 
 * 
 * @����: 
 * @return: uint8  �ɹ�/ʧ��
 * @˵��: 
 * @����: yzy (2013/7/13)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void Netp_DiscoSetRecvFlag(void)
{
    scm_dlms_t* scm = &gs_scmDlms;
    if((scm->stt & SCM_STT_EXMASK) == SCM_STT_DISCO)
    {
        if(scm->revd != 0xff)
        {
            scm->revd++;
        }
    }
}






/*******************************************************************************
 * @function_name:  ES_Netp_Init
 * @function_file:  netp.c
 * @����: ���̳�ʼ��
 * 
 * 
 * @����: 
 * 
 * @����: 
 * @����: yzy (2011-03-03)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
void ES_Netp_Init(void)
{
                                        //��ʼ��������
    SerialSets ss;
    ss.baudrate = NETP_PORT_BAUD;
    ss.parit = Parit_N;
    ss.databits = DataBits_8bits;
    ss.stopbits = StopBits_1;
    gs_Uart.Init(NETP_PORT_NO, &ss);
                                        //�����ȫ�ֱ���
    gul_noPlcReadSecs = 0;
                                        //���ȫ�ֽṹ�����
    //DLMS_ClearRegisMt(__NULL);
    DLMS_LoadRegisMt();
    ClearBuffer((uint8*)&gs_scmDlms, sizeof(gs_scmDlms));
    
    GD_Para_RW(F241_PADDR, &gc_FindMtMode, 1, false);  //

    ES_Message_Send(MSG_NETP_DISCOSTART, TASK_NETP_TKID); //�ѱ�
}



/*******************************************************************************
 * @function_name:  Netp_MbusPowerOnDelay
 * @function_file:  netp.c
 * @����: �ȴ�Mbus��Դ�ȶ�
 * 
 * 
 * @����: 
 * 
 * @����: 
 * @����: yzy (2011-03-03)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
void Netp_MbusPowerOnDelay(void)
{
    gs_GPIO.GPO_Out(GPO_MBUS_PWR, true);
    gs_OS.TK_Sleep(200);                //���鷢��,ֻ��ʱ1s������µ�һ֡���ջ��쳣.
}






