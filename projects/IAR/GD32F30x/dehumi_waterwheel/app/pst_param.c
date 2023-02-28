/****************************************Copyright (c)**************************************************
**                                  ________�Ƽ����޹�˾
**                                          ������
**
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: pst.c
**��   ��   ��: yzy
**����޸�����: 2013��7��3��
**��        ��: 1376.2�������벿��
**ע        ��:
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: yzy
** ��  ��: v1.0
** �ա���: 2013��7��3��
** �衡��: ԭʼ�汾
**-------------------------------------------------------------------------------------------------------
** �޸���: 
** ��  ��: 
** ��  ��: 
********************************************************************************************************/









/************************************************************************ 
  *�����洢�ṹ��
************************************************************************/
typedef struct
{
    uint8  paralen;             //����(�ظ�)
    uint8  paralenbk;           //����(�ظ�)
    uint16 crc;                 //����CRC��β��
    uint8  darray1;             //��һ���ֽ���������...
    //uint8 darray2~darrayn;
    
}shortParaHeader_t;             //�򵥲����洢�ṹͷ






/************************************************************************
 * @function: WriteShortPara
 * @����: д��򵥲���
 * 
 * @����: 
 * @param: dbno ���ݿ�
 * @param: addr ���������ݿ��е���ʼ��ַ
 * @param: pdata ���ݻ���
 * @param: plen ���ݳ���
 * 
 * @����: 
 * @return: uint8 SYS_ERR_OK/SYS_ERR_FT
 * @˵��: 
 * @����: yzy (2014/3/11)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 WriteShortPara(uint8 dbno, uint32 addr, uint8* pdata, uint8 plen)
{
    uint8 temp[8];
    shortParaHeader_t* hdr = (void*)temp;
                                            //д�����������
//    if(SYS_ERR_OK != WriteFlash(dbno, pdata, plen, addr + 4))
//    {
//        return SYS_ERR_FT;
//    }
                                            //д��4�ֽ�ͷ
    hdr->paralen = plen;
    hdr->paralenbk = plen;
    hdr->crc = DoCrc16(pdata, plen, 0x5555);
//    if(SYS_ERR_OK != WriteFlash(dbno, (uint8*)hdr, 4, addr))
//    {
//        return SYS_ERR_FT;
//    }
    SYS_OK();
}




/************************************************************************
 * @function: ReadShortPara
 * @����: ��ȡ�򵥲���
 * 
 * @����: 
 * @param: dbno ���ݿ�
 * @param: addr ���������ݿ��е���ʼ��ַ
 * @param: pbuff   ���ݻ���
 * @param: bufflen ���ݵ����������
 * 
 * @����: 
 * @return: 0 ��ȡʧ��. others ��ȡ�����ݳ���
 * @˵��: ��ȡʧ��ʱ,��Ĭ�Ͻ���������
 * @����: yzy (2013/6/20)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 ReadShortPara(uint8 dbno, uint32 addr, uint8* pbuff, uint8 bufflen)
{
    uint16 crc;
    uint8 temp[8];
    shortParaHeader_t* hdr = (void*)temp;

    if(SYS_ERR_OK == ReadFlash(dbno, (uint8*)hdr, sizeof(shortParaHeader_t), addr))
    {
        if((hdr->paralen != hdr->paralenbk) || (hdr->paralen > bufflen))
        {
            memset(pbuff, 0, bufflen);
            return 0;
        }
        
        if(SYS_ERR_OK == ReadFlash(dbno, pbuff, hdr->paralen, addr + 4))
        {
            crc = DoCrc16(pbuff, hdr->paralen, 0x5555);
            if(hdr->crc == crc)
            {
                return hdr->paralen;
            }
        }
    }
    return 0;
}



/************************************************************************
 * @function: CrackShortPara
 * @����: ��ռ򵥲���
 * 
 * @����: 
 * @param: dbno ���ݿ�
 * @param: addr ���������ݿ��е���ʼ��ַ
 * @����: 
 * @˵��: 
 * @����: yzy (2014/3/11)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void CrackShortPara(uint8 dbno, uint32 addr)
{
    uint8 temp[8];
    shortParaHeader_t* hdr = (void*)temp;
                                //����Ч����
    memset(temp, 0xff, sizeof(temp));
                                //д��
//    WriteFlash(dbno, (uint8*)hdr, 4, addr);
}



/************************************************************************
 * @function: GD_Para_RW
 * @����: ����дflash����
 * 
 * @����: 
 * @param: addr flashƫ�Ƶ�ַ
 * @param: buff ��д����
 * @param: len ��Ҫ��д�ĳ���
 * @param: rw false��; trueд
 * 
 * @����: 
 * @return: uint8  SYS_ERR_OK/SYS_ERR_FT
 * @˵��: 
 * @����: yzy (2014/2/12)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 GD_Para_RW(uint32 addr, uint8* buff, uint16 len, bool rw)
{
    if(rw)
    {
        if(SYS_ERR_OK == WriteFlash(DB_PARA, buff, len, addr))
        {
            SYS_OK();
        }
    }
    else
    {
        if(SYS_ERR_OK == ReadFlash(DB_PARA, buff, len, addr))
        {
            SYS_OK();
        }
    }
    
    return SYS_ERR_FT;
}

/************************************************************************
 * @function: RUN_Para_RW
 * @����: ����дflash����
 * 
 * @����: 
 * @param: addr flashƫ�Ƶ�ַ
 * @param: buff ��д����
 * @param: len ��Ҫ��д�ĳ���
 * @param: rw false��; trueд
 * 
 * @����: 
 * @return: uint8  SYS_ERR_OK/SYS_ERR_FT
 * @˵��: 
 * @����: yzy (2014/2/12)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 RUN_Para_RW(uint32 addr, uint8* buff, uint16 len, bool rw)
{
//    if(rw)
//    {
//        if(SYS_ERR_OK == WriteFlash(DB_RUN_PARA, buff, len, addr))
//        {
//            SYS_OK();
//        }
//    }
//    else
//    {
//        if(SYS_ERR_OK == ReadFlash(DB_RUN_PARA, buff, len, addr))
//        {
//            SYS_OK();
//        }
//    }
    
    return SYS_ERR_FT;
}


//	uint8_t g_tmp_netpPara[2048];
uint8 Netp_Framework_RW(uint32 addr, uint8* buff, uint16 len, bool rw)
{
//    if(rw)
//    {
////        memcpy(g_tmp_netpPara+addr, buff, len);
//        if(SYS_ERR_OK == WriteFlash(DB_PARA_NETP, buff, len, addr))
//        {
//            SYS_OK();
//        }
//    }
//    else
//    {
////        memcpy(buff, g_tmp_netpPara+addr, len);
//        if(SYS_ERR_OK == ReadFlash(DB_PARA_NETP, buff, len, addr))
//        {
//            SYS_OK();
//        }
//    }
    
    return SYS_ERR_FT;
}


/************************************************************************
 * @function: GD_ParaShort_W
 * @����: ��У�鼰������Ϣ�Ķ̲�����д
 * 
 * @����: 
 * @param: addr ������ַ
 * @param: buff ���ݻ���(__NULL��ʾɾ��)
 * @param: len  д�����ݵĳ���(=��������)
 * 
 * @����: 
 * @return: uint8  SYS_ERR_OK/SYS_ERR_FT
 * @˵��: ��ȷ���õ�ַ��ʼ�洢���Ƕ̲���
 * @����: yzy (2014/3/11)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 GD_ParaShort_W(uint32 addr, uint8* buff, uint8 len)
{
    return WriteShortPara(DB_PARA, addr, buff, len);
}



/************************************************************************
 * @function: GD_ParaShort_R
 * @����: ��У�鼰������Ϣ�Ķ̲����Ķ�
 * 
 * @����: 
 * @param: fn  ������ʶ
 * @param: buff ���ݻ���
 * @param: len  ���ݻ��泤��(>=��������)
 * 
 * @����: 
 * @return: uint8  0 ��ȡʧ��. others ��ȡ�����ݳ���
 * @˵��: ��ȷ���õ�ַ��ʼ�洢���Ƕ̲���
 * @����: yzy (2014/3/11)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 GD_ParaShort_R(uint32 addr, uint8* buff, uint8 len)
{
    return ReadShortPara(DB_PARA, addr, buff, len);
}

#if 0

/************************************************************************
 * @function: GD_ParaShort_Rst
 * @����: ��У�鼰������Ϣ�Ķ̲����ĸ�λ
 * 
 * @����: 
 * @param: addr 
 * @����: 
 * @˵��: ��ȷ���õ�ַ��ʼ�洢���Ƕ̲���
 * @����: yzy (2014/3/12)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void GD_ParaShort_Rst(uint32 addr)
{
    CrackShortPara(DB_PARA, addr);
}




/************************************************************************
 * @function: GD_ParaShort_W
 * @����: ��У�鼰������Ϣ�Ķ̲�����д
 * 
 * @����: 
 * @param: fn  ������ʶ
 * @param: buff ���ݻ���(__NULL��ʾɾ��)
 * @param: len  д�����ݵĳ���(=��������)
 * 
 * @����: 
 * @return: uint8  SYS_ERR_OK/SYS_ERR_FT
 * @˵��: 
 * @����: yzy (2014/3/11)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 GD_ParaShort_Fn_W(uint16 fn, uint8* buff, uint8 len)
{
    const S_PARAINFO* pinfo;
                                        //���Ҳ�����ַ
    pinfo = GetParaInfo(fn);
    if(pinfo == __NULL)
    {
        return SYS_ERR_FT;
    }
                                        //�ж��Ƿ�Ϊ�̵�ַ����
    if((pinfo->type & PARA_ORG_MASK) != PARA_ORG_ST)
    {
        return SYS_ERR_FT;
    }

    if(__NULL == buff)                  //ɾ��
    {
        CrackShortPara(DB_PARA, pinfo->addr);
    }
    else
    {                                   //���ȱ���һ��
        if(len != pinfo->len)
        {
            return SYS_ERR_FT;
        }
    }
                                        //ִ�ж�д����
    return WriteShortPara(DB_PARA, pinfo->addr, buff, pinfo->len);
}




/************************************************************************
 * @function: GD_ParaShort_Fn_R
 * @����: ��У�鼰������Ϣ�Ķ̲����Ķ�
 * 
 * @����: 
 * @param: fn  ������ʶ
 * @param: buff ���ݻ���
 * @param: len  ���ݻ��泤��(>=��������)
 * 
 * @����: 
 * @return: uint8  0 ��ȡʧ��. others ��ȡ�����ݳ���
 * @˵��: 
 * @����: yzy (2014/3/11)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 GD_ParaShort_Fn_R(uint16 fn, uint8* buff, uint8 len)
{
    const S_PARAINFO* pinfo;
    uint8 rdlen;
                                        //���Ҳ�����ַ
    pinfo = GetParaInfo(fn);
    if(pinfo == __NULL)
    {
        return 0;
    }
                                        //�ж��Ƿ�Ϊ�̵�ַ����
    if((pinfo->type & PARA_ORG_MASK) != PARA_ORG_ST)
    {
        return 0;
    }
                                        //���泤��ֻ���Դ�,������С
    if(len < pinfo->len)
    {
        return 0;
    }
                                        //ִ�ж�д����
    rdlen = ReadShortPara(DB_PARA, pinfo->addr, buff, len);
    if((rdlen == 0) || (rdlen != pinfo->len))  //��ȡʧ��,��������Ȳ�һ��
    {
        return 0;
    }

    return pinfo->len;

}






/************************************************************************
 * @function: GD_ParaShort_Fn_Rst
 * @����: ��У�鼰������Ϣ�Ķ̲��������
 * 
 * @����: 
 * @param: fn ������ʶ
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2014/3/12)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 GD_ParaShort_Fn_Rst(uint16 fn)
{
    const S_PARAINFO* pinfo;
                                        //���Ҳ�����ַ
    pinfo = GetParaInfo(fn);
    if(pinfo == __NULL)
    {
        return SYS_ERR_FT;
    }
                                        //�ж��Ƿ�Ϊ�̵�ַ����
    if((pinfo->type & PARA_ORG_MASK) != PARA_ORG_ST)
    {
        return SYS_ERR_FT;
    }
                                        //ִ�ж�д����
    CrackShortPara(DB_PARA, pinfo->addr);
    SYS_OK();

}




/************************************************************************
 * @function: GD_ParaDispatch_W
 * @����: �ֵ�洢�������д
 * 
 * @����: 
 * @param: addr  ������ַ
 * @param: buff  д���ݻ���(__NULL��ʾɾ���õ�)
 * @param: dplen ��Ҫд�ĳ���(=��������)
 * @param: dpidx �洢�����(0-)
 * 
 * @����: 
 * @return: uint8  
 * @˵��: ��ȷ���õ�ַ��ʼ�洢������ȷ
 * @����: yzy (2014/3/11)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 GD_ParaDispatch_W(uint32 addr, uint8* buff, uint8 dplen, uint16 dpidx)
{
    uint8 bmps[32];
    uint32 data_addr;
                                        //������������λͼ��Χ
    if(dpidx >= (sizeof(bmps) * 8))
    {
        return SYS_ERR_FT;
    }
                                        //��FLASH��ȡλͼ
    if(GD_Para_RW(addr, bmps, sizeof(bmps), false) != SYS_ERR_OK)
    {
        return SYS_ERR_FT;
    }

    if(__NULL == buff)                  //ɾ��
    {
        if(STR_IsBitClr(bmps, dpidx))   //�Ѿ�������
        {
            SYS_OK();
        }
                                        //����λͼ
        STR_ClrBit(bmps, dpidx);
    }
    else                                //����
    {
        data_addr = addr + sizeof(bmps) + dplen * dpidx;
        GD_Para_RW(data_addr, buff, dplen, true);
                                        //����λͼ
        STR_SetBit(bmps, dpidx);
    }
                                        //λͼ����FLASH
    GD_Para_RW(addr, bmps, sizeof(bmps), true);
    SYS_OK();
}




/************************************************************************
 * @function: GD_ParaDispatch_R
 * @����: �ֵ�洢������Ķ�
 * 
 * @����: 
 * @param: addr ������ַ
 * @param: buff �����ݻ���
 * @param: dplen ��Ҫ���ĳ���(=��������)
 * @param: dpidx �洢�����(0-)
 * 
 * @����: 
 * @return: uint8  
 * @˵��: ��ȷ���õ�ַ��ʼ�洢������ȷ
 * @����: yzy (2014/3/12)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 GD_ParaDispatch_R(uint32 addr, uint8* buff, uint8 dplen, uint16 dpidx)
{
    uint8 bmps[32];
    uint32 data_addr;
                                        //������������λͼ��Χ
    if(dpidx >= (sizeof(bmps) * 8))
    {
        return SYS_ERR_FT;
    }
                                        //��FLASH��ȡλͼ
    if(GD_Para_RW(addr, bmps, sizeof(bmps), false) != SYS_ERR_OK)
    {
        return SYS_ERR_FT;
    }
                                        //�ô洢�㲻����
    if(STR_IsBitClr(bmps, dpidx))
    {
        return SYS_ERR_FT;
    }

    data_addr = addr + sizeof(bmps) + dplen * dpidx;
    return GD_Para_RW(data_addr, buff, dplen, false);
}





/************************************************************************
 * @function: GD_ParaDispatch_Rst
 * @����: �ֵ�洢������ĸ�λ���
 * 
 * @����: 
 * @param: addr ������ַ
 * 
 * @����: 
 * @return: uint8  
 * @˵��: ��ȷ���õ�ַ��ʼ�洢������ȷ
 * @����: yzy (2014/3/12)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 GD_ParaDispatch_Rst(uint32 addr)
{
    uint8 bmps[32];
                                        //λͼ����FLASH
    memset(bmps, 0, sizeof(bmps));
    GD_Para_RW(addr, bmps, sizeof(bmps), true);
    SYS_OK();
}







/************************************************************************
 * @function: GD_ParaDispatch_Fn_W
 * @����: �ֵ�洢�������д
 * 
 * @����: 
 * @param: fn ������ʶ
 * @param: buff д���ݻ���(__NULL��ʾɾ���õ�)
 * @param: len  ��Ҫд�ĳ���(<=��������)
 * @param: dpidx �洢�����(0-)
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2014/3/11)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 GD_ParaDispatch_Fn_W(uint16 fn, uint8* buff, uint8 len, uint16 dpidx)
{
    const S_PARAINFO* pinfo;
    uint8 bmps[32];
    uint32 addr;
                                        //������������λͼ��Χ
    if(dpidx >= (sizeof(bmps) * 8))
    {
        return SYS_ERR_FT;
    }
                                        //���Ҳ�����ַ
    pinfo = GetParaInfo(fn);
    if(pinfo == __NULL)
    {
        return SYS_ERR_FT;
    }
                                        //�ж��Ƿ�Ϊ�ֵ�洢����
    if((pinfo->type & PARA_ORG_MASK) != PARA_ORG_DP)
    {
        return SYS_ERR_FT;
    }
                                        //��FLASH��ȡλͼ
    if(GD_Para_RW(pinfo->addr, bmps, sizeof(bmps), false) != SYS_ERR_OK)
    {
        return SYS_ERR_FT;
    }

    if(__NULL == buff)                  //ɾ��
    {
        if(STR_IsBitClr(bmps, dpidx))   //�Ѿ�������
        {
            SYS_OK();
        }
                                        //����λͼ
        STR_ClrBit(bmps, dpidx);
    }
    else                                //����
    {
        if(len > pinfo->len)            //д�����ݳ��Ȳ��ô���ʵ�ʲ�������
        {
            return SYS_ERR_FT;
        }
                                        //д������
        addr = pinfo->addr + sizeof(bmps) + pinfo->len * dpidx;
        GD_Para_RW(addr, buff, len, true);
                                        //����λͼ
        STR_SetBit(bmps, dpidx);
    }
                                        //λͼ����FLASH
    GD_Para_RW(pinfo->addr, bmps, sizeof(bmps), true);
    SYS_OK();
}







/************************************************************************
 * @function: GD_ParaDispatch_Fn_R
 * @����: �ֵ�洢������Ķ�
 * 
 * @����: 
 * @param: fn ������ʶ
 * @param: buff �����ݻ���
 * @param: len  ��Ҫ���ĳ���(����������޹�)
 * @param: dpidx �洢�����(0-)
 * 
 * @����: 
 * @return: uint8  SYS_ERR_FT/SYS_ERR_OK
 * @˵��: 
 * @����: yzy (2014/3/11)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 GD_ParaDispatch_Fn_R(uint16 fn, uint8* buff, uint8 len, uint16 dpidx)
{
    const S_PARAINFO* pinfo;
    uint8 bmps[32];
    uint32 addr;
                                        //������������λͼ��Χ
    if(dpidx >= (sizeof(bmps) * 8))
    {
        return SYS_ERR_FT;
    }
                                        //���Ҳ�����ַ
    pinfo = GetParaInfo(fn);
    if(pinfo == __NULL)
    {
        return SYS_ERR_FT;
    }
                                        //�ж��Ƿ�Ϊ�ֵ�洢����
    if((pinfo->type & PARA_ORG_MASK) != PARA_ORG_DP)
    {
        return SYS_ERR_FT;
    }
                                        //��FLASH��ȡλͼ
    if(GD_Para_RW(pinfo->addr, bmps, sizeof(bmps), false) != SYS_ERR_OK)
    {
        return SYS_ERR_FT;
    }
                                        //�ô洢�㲻����
    if(STR_IsBitClr(bmps, dpidx))
    {
        return SYS_ERR_FT;
    }

    addr = pinfo->addr + sizeof(bmps) + pinfo->len * dpidx;
    return GD_Para_RW(addr, buff, len, false);
}




/************************************************************************
 * @function: GD_ParaDispatch_Fn_Rst
 * @����: �ֵ�洢������ĸ�λ���
 * 
 * @����: 
 * @param: fn ������ʶ
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2014/3/12)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 GD_ParaDispatch_Fn_Rst(uint16 fn)
{
    const S_PARAINFO* pinfo;
    uint8 bmps[32];
                                        //���Ҳ�����ַ
    pinfo = GetParaInfo(fn);
    if(pinfo == __NULL)
    {
        return SYS_ERR_FT;
    }
                                        //�ж��Ƿ�Ϊ�ֵ�洢����
    if((pinfo->type & PARA_ORG_MASK) != PARA_ORG_DP)
    {
        return SYS_ERR_FT;
    }
                                        //λͼ����FLASH
    memset(bmps, 0, sizeof(bmps));
    GD_Para_RW(pinfo->addr, bmps, sizeof(bmps), true);
    SYS_OK();
}


#endif


