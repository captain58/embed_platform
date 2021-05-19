/****************************************Copyright (c)**************************************************
**                                  ____�Ƽ����޹�˾
**                                          ������
**
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: Hexing_Dlms.c
**��   ��   ��: ZJC
**����޸�����: 2014��11��3��
**��        ��: ������
**ע        ��:
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: ZJC
** ��  ��: v1.0
** �ա���: 2014��11��3��
** �衡��: ԭʼ�汾
********************************************************************************************************/
#define NETP_FM


#include "public.h"
#include "lib_com/calclib_gw.h"


#include "hlv.h"
#include "netp.h"
#include "paradef.h"
#include "inp.h"
#include "evt.h"
#include "netp_fm.h"

uint8  guc_mtUpdata_sendcnt;       //���������������������ʱ����ֹ���������и�λ�����쳣�����Լ�����ǰ����


/******************************************************************************
**�����˿�ʵʱ����(����DLMS)  OBIS�б�
******************************************************************************/
//	uint8 const HX_DLMSComand[NETP_READ_HX_OBIS_MAX][9]={  //����ID   	
//	 	
//	    {0,1, 0,0,97,129,0,255,2}, //�¼�״̬�Ĵ��� 0    //ʵʱ
//	    {0,7, 1,0,99,98,0,255,2}, //��׼�¼� 1    //ʵʱ
//	    {0,7, 1,0,99,98,1,255,2}, //��թ�¼� 2    //ʵʱ
//	    {0,7, 1,0,99,98,2,255,2}, //�̵����¼� 3    //ʵʱ
//	    {0,7, 1,0,99,98,4,255,2}, //�����¼� 4    //ʵʱ
//	    {0,7, 0,0,98,27,128,255,2}, //�����й����� 5 //15min
//	    {0,7, 0,0,98,28,128,255,2}, //�����й����� 6 //15min
//	    {0,7, 0,0,98,29,128,255,2}, //��ѹ 7 //15min
//	    {0,7, 0,0,98,30,128,255,2}, //���� 8 //15min
//	    {0,7, 0,0,98,31,128,255,2}, //���� 9 //15min
//	    {0,7, 0,0,98,32,128,255,2}, //�ն��� 10 //15min
//	    {0,7, 0,0,98,7,128,255,2},  //�¶��� 11 //15min
//	    {0,1, 0,0,202,11,5,255,2},  //Уʱ���� 12 //15min
//	    
//	    {0x00,0x12, 0x00,0x00,0x2C,0x00,0x00,0xFF,0x05},  //дʹ��
//	    {0x00,0x01, 0x00,0x00,0x2C,0x80,0x00,0xFF,0x02},  //д����ģʽ��
//	    {0x00,0x12, 0x00,0x00,0x2C,0x00,0x00,0xFF,0x01},  //������ʼ������Ϣͷ
//	    {0x00,0x12, 0x00,0x00,0x2C,0x00,0x00,0xFF,0x02},  //�ļ�����
//	    {0x00,0x12, 0x00,0x00,0x2C,0x00,0x00,0xFF,0x03},  //��λͼ
//	    {0x00,0x12, 0x00,0x00,0x2C,0x00,0x00,0xFF,0x03},  //��֤
//	    {0x00,0x16, 0x00,0x00,0x0F,0x00,0x02,0xFF,0x04},  //��������ʱ��
//	    {0x00,0x12, 0x00,0x00,0x2C,0x00,0x00,0xFF,0x04},  //����ִ��
//	    {0x00,0x12, 0x00,0x00,0x2C,0x00,0x00,0xFF,0x06},  //��״̬
//	    {0x00,0x01, 0x01,0x00,0x00,0x02,0x08,0xFF,0x02},  //������汾����ֹ�ظ�����
//	};	

/************************************************************************
 * @function: ByteToInt
 * @����: �����ֽڵ�����ת����һ��16λ����
 * 
 * @����: 
 * @param: byte ���Ҫת���������ֽڵĻ���
 * 
 * @����: 
 * @return: uint16  �����ֽ�ת����16λ��֮��Ľ��
 * @˵��: ����byte[0] = 0x03, byte[1] = 0x02��ôת����Ľ��Ϊ0x0302
 * @����: yzy (2013/6/6)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint16 DLMS_ByteToInt(uint8* byte)
{
    Word16 wd;                              //����һ��˫�ֽ�������
    wd.byte[0] = byte[1];                   //��һ���ֽ�
    wd.byte[1] = byte[0];                   //�ڶ����ֽ�
    return wd.word;
}

/************************************************************************
 * @function: ByteToLong
 * @����: �ĸ��ֽ�ת����32λ��
 * 
 * @����: 
 * @param: byte ���Ҫת�����ĸ��ֽڵĻ���
 * 
 * @����: 
 * @return: uint32  �ĸ��ֽ�ת����32λ��֮��Ľ��
 * @˵��: ����byte[0] = 0x03, byte[1] = 0x02,
 *         byte[2] = 0x04, byte[3] = 0x05,
 *         ��ôת����Ľ��Ϊ0x03020405
 * @����: yzy (2013/6/6)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint32 DLMS_ByteToLong(uint8* byte)
{
    Word32 wd;
    wd.byte[0] = byte[3];                   //�����ƶ�4���ֽ�
    wd.byte[1] = byte[2];
    wd.byte[2] = byte[1];
    wd.byte[3] = byte[0];
    return wd.lword;
}

/************************************************************************
 * @function: DLMS_ByteToInt24
 * @����: �����ֽ�ת����24λ��
 * 
 * @����: 
 * @param: byte ���Ҫת���������ֽڵĻ���
* @param: isbcd �����ʽ(��hex,��bcd)
 * 
 * @����: 
 * @return: uint32  �����ֽ�ת����24λ��֮��Ľ��
 * @˵��: ����byte[0] = 0x03, byte[1] = 0x02, byte[2] = 0x04��
 *        ��ôת����Ľ��Ϊ0x030204
 * @����: zjc (2014/10/9)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint32 DLMS_ByteToInt24(uint8* byte, bool isbcd)
{
    Word32 wd;
    InvBuffer(byte, 3);
    if(isbcd)
    {
        BcdToHex(byte, wd.byte, 3, 3);
    }
    else
    {
        MoveBuffer(byte, wd.byte, 3);
    }
    wd.byte[3] = 0;                         //����ֽ���Ч
    return wd.lword;
}



/************************************************************************
 * @function: DLMS_ByteToData
 * @����: ����DLMS����֡�Զ��жϣ��������ͣ����������
 * 
 * @����: 
 * @param: byte ���뻺���ַ
 * @param: m    ��������λ��
 * @����: 
 * @return: uint32  ���
 * @˵��: 
 *        
 * @����: zjc (2014/10/9)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint32  DLMS_ByteToData(uint8* byte, uint8 *m)
{   
    uint32 dtword = 0;
    uint8  caseid = byte[*m];
    *m += 1;
    switch(caseid)
    {
    case COMM_DATATYPE_INT32:
        dtword = DLMS_ByteToLong(byte + *m);
        *m += 4;
        break;
    case COMM_DATATYPE_INT32U:
        dtword = DLMS_ByteToLong(byte + *m);
        *m += 4;
        break;
    case COMM_DATATYPE_INT16:
        dtword = DLMS_ByteToInt(byte + *m);
        *m += 2;
        break;
    case COMM_DATATYPE_INT16U:
        dtword = DLMS_ByteToInt(byte + *m);
        *m += 2;
        break;
    case COMM_DATATYPE_INT8:
        dtword = byte[*m];
        *m += 1;
        break;
    case COMM_DATATYPE_INT8U:
        dtword = byte[*m];
        *m += 1;
        break;
    }

    return dtword;
    
}


/************************************************************************
 * @function: NETP_CheckFrame
 * @����: ָ��֡���͵���Ч֡�ж�
 * 
 * @����: 
 * @param: NetpFrameStr ��Ҫ�жϵ�֡�Ľṹ
 * @param: buffer ��Ҫ�ж���Ч֡�Ļ���
 * @param: length ��Ҫ�жϵ�֡�ĳ���
 * 
 * @����: 
 * @return: uint8  PST_ERR_OK
 *      PST_ERR_VAR
 *      PST_ERR_TS
 *      PST_ERR_NE
 * @˵��: 
 * @����: yzy (2013/7/15)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 NETP_CheckFrame(const NetpFrameStr* str, uint8* buffer, uint16 length, uint16_t *outLen)
{
//	    const PSTFrameStr* str;                 //����ָ��֡��Ϣ�ṹ��
    volatile uint16 len;                   //����֡����������
    uint16 ut_i;                             //ѭ������
    uint16 sumlen;                          //��У�����ݳ���
    uint16 sumos;                           //У��ʹ�ŵ�λ��
    uint16 cs;

//	    SYS_VAR_CHECK(num >= PST_FRM_NO);        //������֤
    SYS_VAR_CHECK(length > PST_LEN_MAX);     //������֤
                                            //��ȡ��ǰҪɨ��Ĺ�Լ�Ľṹ��
//	    str = (PSTFrameStr*)gss_PSTFrameStr[num];

    if(str->extlen > length)                //�ж��Ƿ��Ѿ��ﵽ��С֡����
    {
        return PST_ERR_TS;                  //����ɨ�賤��̫��
    }
                                            //�ж�֡ͷ
    for(ut_i = 0; ut_i < str->hlen; ut_i++)
    {                                       //�����ƥ���ַ�ƥ����ô������һ��
        if(buffer[ut_i] == str->head[ut_i])
        {
            continue;
        }
                                            //�����ǰ��ƥ���ַ�Ϊ����ƥ���ַ������
        if(str->head[ut_i] == PST_FRM_ANY) 
        {
            continue;
        }
        return PST_ERR_NE;                  //���ز�����Ч֡
    }

    if(str->latr & PST_LATR_NOLEN)          //û������������
    {
        len = 0;                            //��������������Ϊ0
        
        //�������������ݳ��ȵ�����£����ж�У��ͣ�ȷ������������
        if(!(str->atr & PST_ATR_END))           //�н�β��
        {
                                            //ѭ��ɨ���β��
            for(ut_i = str->extlen - 1; ut_i < length; ut_i++)
            {
                if(buffer[ut_i] == str->end)
                {
                    len = ut_i + 1 - str->extlen;
                    break;
                }
            }
            if(len == 0)
            {
                return PST_ERR_NE;                  //���ز�����Ч֡
            }
        }
        else
        {
            len = length - str->extlen;
        }
    }
    else                                    //������������
    {
        if(str->latr & PST_LATR_DBLEN)      //�����˫�ֽڳ���
        {
            if(str->latr &  PST_LATR_BFLEN) //���ֽ���ǰ
            {
                len = buffer[str->lp + 1]           //���ݳ���
                    + (buffer[str->lp ] << 8);
            }
            else                            //���ֽ���ǰ
            {
                len = buffer[str->lp]           //���ݳ���
                    + (buffer[str->lp + 1] << 8);
            }
        }
        else                                //������ǵ��ֽڵ�֡��
        {
            len = buffer[str->lp];          //���ݳ���
        }
        
        if(str->latr & PST_LATR_DBLEN)      //�����˫�ֽڳ���
        {
            len <<= str->llft;              //Ϊ��ֹ���뱻�������Ż�����Ҫ����Ϊvolatile����
            len &= 0xffff;
            len >>= str->llft;
            len >>= str->lrft;
        }
        else
        {
            len <<= str->llft;
            len &= 0xff;
            len >>= str->llft;
            len >>= str->lrft;
        }
    }

    if(len + str->extlen > length)          //�����������������С֡����֮�����ɨ���֡����ô�϶�����һ����Ч֡
    {
        return PST_ERR_NE;                  //���ز�����Ч֡    
    }
                                            
    if(!(str->atr & PST_ATR_PARIT))         //��У��λ
    {
                                            //�����У�����ݵĳ���
        sumlen = len + str->extlen;
        if(str->cssl & 0x80)                //��
        {
            if(sumlen > (str->cssl & 0x7F))
            {
                sumlen -= (str->cssl & 0x7F);
            }
            else
            {
                return PST_ERR_NE;
            }
        }
        else                                //��
        {
            sumlen += (str->cssl & 0x7F);
        }
                                            //����У��ʹ�ŵ�λ��ƫ��
        sumos = len + str->extlen;
        if(str->csco & 0x80)                //��
        {
            if(sumos > (str->csco & 0x7F))
            {
                sumos -= (str->csco & 0x7F);
            }
            else
            {
                return PST_ERR_NE;
            }
        }
        else                                //��
        {
            sumos += (str->csco & 0x7F);
        }
        
        switch(str->atr & PST_ATR_CSCAL)
        {
            case PST_ATR_CSCAL_SC:          //�ۼӺ�
                cs = DoSum(
                    buffer + str->cssp, 
                    sumlen
                    );
                
                if((cs & 0xFF) != buffer[sumos])
                {
                    return PST_ERR_NE;      //���ز�����Ч֡   
                }
                break;
                
            case PST_ATR_CSCAL_SD:          //˫�ֽ��ۼӺ�
                cs = DoSum(
                    buffer + str->cssp, 
                    sumlen
                    );
                
                if(cs != ByteToInt(buffer + sumos))
                {
                    return PST_ERR_NE;      //���ز�����Ч֡   
                }
                break;
            case PST_ATR_CSCAL_SD1:          //˫�ֽ��ۼӺ�
                cs = DoSum(
                    buffer + str->cssp, 
                    sumlen
                    );
                cs &= 0xFF;
                if(cs != ByteToInt(buffer + sumos))
                {
                    return PST_ERR_NE;      //���ز�����Ч֡   
                }
                break;

            case PST_ATR_CSCAL_BS:          //λ�Ӻ�
                cs = DoBitSum(
                    buffer + str->cssp, 
                    sumlen
                    );

                if((uint8)cs != buffer[sumos])
                {
                    return PST_ERR_NE;      //���ز�����Ч֡      
                }
                break;
                
            case PST_ATR_CSCAL_CRC1:
                cs = DoCrcDlms(
                    buffer + str->cssp, 
                    sumlen,
                    0xffff
                    );
                
                if(cs != ByteToInt(buffer + sumos))
                {
                    return PST_ERR_NE;      //���ز�����Ч֡   
                }
                break;
            case PST_ATR_CSCAL_CRC2:    //EDMI, ���жϼ����֮ǰ����Ҫ��ת�봦��
                for(uint16 ui_k = 1; ui_k < (length - 1); ui_k++)  //ȥ��ת���
                {
                    if((buffer[ui_k] == 0x10) && ((buffer[ui_k + 1] == 0x42) || (buffer[ui_k + 1] == 0x43) ||
                        (buffer[ui_k + 1] == 0x50) || (buffer[ui_k + 1] == 0x51) || (buffer[ui_k + 1] == 0x53)))
                    {	
                        buffer[ui_k] = buffer[ui_k + 1] - 0x40;
                        MoveBuffer(buffer + ui_k + 2, buffer + ui_k + 1, sumlen + 1 - ui_k);
                        length --;
                        sumlen--;
                        sumos--;
                    }
                }
                cs = DoCrc16(
                    buffer + str->cssp, 
                    sumlen, 0
                    );
                
                if(cs !=( ((uint16)buffer[sumos] << 8) + (uint16)buffer[sumos + 1]))
                {
                    return PST_ERR_NE;      //���ز�����Ч֡   
                }
                break;
            case PST_ATR_CSCAL_CRC3:
                cs = DoCrcRtuModbus(
                    buffer + str->cssp, 
                    sumlen,
                    0xffff
                    );
                
                if(cs != ByteToInt(buffer + sumos))
                {
                    return PST_ERR_NE;      //���ز�����Ч֡   
                }
                break;
                
            default:
                return PST_ERR_NE;          //���ز�����Ч֡         
        }
    }
    *outLen = len + str->extlen;
    if(!(str->atr & PST_ATR_END))           //�н�β��
    {
        if(str->latr & PST_LATR_NOLEN)      //û������������
        {
                                            //ѭ��ɨ���β��
            for(ut_i = str->extlen - 1; ut_i < length; ut_i++)
            {
                if(buffer[ut_i] == str->end)
                {
                    SYS_OK();                //����ɨ�赽��Ч֡
                }
            }
            return PST_ERR_NE;              //���ز�����Ч֡  
        }
        else
        {                                   //�ж��Ƿ�Ϊ��β��
            if(buffer[str->extlen - 1 + len] != str->end)
            {
                return PST_ERR_NE;          //���ز�����Ч֡      
            }
        }
    }
    SYS_OK();                                //����ɨ�赽��Ч֡
}


/************************************************************************
 * @function: NETP_CheckFrame2
 * @����: ֡��Ч�Լ��,����һ��������ǰ����,������ǰ����
 * 
 * @����: 
 * @param: str ��Ҫ�жϵ�֡�ṹ
 * @param: buffer �շ�����
 * @param: len ����Ϊ�������ݳ���,���Ϊ��Ч���ݳ���
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2013/11/3)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/

uint8 NETP_CheckFrame2(const NetpFrameStr* str, uint8* buffer, uint16* len)
{
    uint16 tplen;
    uint16_t frame_len = 0;
    for(uint32 uc_i = 0; (uc_i < 7) && ((*len) - uc_i) > 0; uc_i++)
    {
        tplen = (*len) - uc_i;
        if(NETP_CheckFrame(str, buffer + uc_i, tplen, &frame_len) == PST_ERR_OK)
        {
            MoveBuffer(buffer + uc_i, buffer, frame_len);
            (*len) = frame_len;
            return 0;
        }
    }
    return 2;
}

/*********************************************************************************************************
**��������  int NETP_DoneFrame(const NetpComposeFrameStr * frmstr, const NetpFrameDataStr * frameDtaStr, uint8_t * buf, uint8* addr, uint16* length)
**����
        frmstr ����Ľṹ��
        frameDtaStr ������������
        buf ���ݻ���
        addr ����ĵ�ַ            
        length ����Ϊ����������󳤶ȣ����Ϊ�����֮��ĳ���
**���  PST_ERR_OK
        PST_ERR_VAR
**      PST_ERR_FT
**˵��  �Գ���֡֡���д��������ͨѶ
** ȫ�ֱ���:
** ����ģ��:
**
**------------------------------------------------------------------------------------------------------
** ��  ��: yzy
** �ա���: 2019��6��18��
** ��  ��: 
********************************************************************************************************/
int NETP_DoneFrame(const NetpComposeFrameStr * frmstr, 
                            const NetpFrameDataStr * frameDtaStr, 
                            uint8_t * buf, uint8_t* addr, uint8_t *data, uint16_t* length)
{
    int m = 0;
    int frame_len = 0,len_area = 0,data_area=0;
    int i = 0;
    if(NULL == frmstr || NULL == frameDtaStr || 
        NULL == buf || NULL == addr || *(uint32_t *)frmstr == 0xFFFFFFFF)
        return SYS_ERR_FT;
//֡ͷ
    if((frmstr->frm_bit & PST_FRM_BIT_HEAD) == PST_FRM_BIT_HEAD && frmstr->hlen <= 32)
    {
        memcpy(buf + m, frmstr->head, frmstr->hlen);
        m+=frmstr->hlen;
    }
    if((frmstr->len_bit & PST_FRM_BIT_HEAD) == PST_FRM_BIT_HEAD)
    {
        frame_len+=frmstr->hlen;
    }
    
//��ַ��    
    if((frmstr->frm_bit & PST_FRM_BIT_ADDR) == PST_FRM_BIT_ADDR && frmstr->alen <= 32)
    {
        memcpy(buf + (frmstr->adlp & 0x7f), addr, frmstr->alen);
        if((frmstr->adlp & 0x80) == 0x80)
        {
            m+=frmstr->alen;
            if((frmstr->len_bit & PST_FRM_BIT_ADDR) == PST_FRM_BIT_ADDR)
            {
                frame_len+=frmstr->hlen;
            }
            
        }
    }
    
//������
    if((frmstr->frm_bit & PST_FRM_BIT_CMD) == PST_FRM_BIT_CMD && frmstr->cmdlen <= 4)
    {
        memcpy(buf + (frmstr->cmdlp & 0x7f), &frameDtaStr->cmd, frmstr->cmdlen);
        m+=frmstr->cmdlen;
    }
    if((frmstr->len_bit & PST_FRM_BIT_CMD) == PST_FRM_BIT_CMD)
    {
        frame_len+=frmstr->cmdlen;
    }

//DI��
    if((frmstr->frm_bit & PST_FRM_BIT_DI) == PST_FRM_BIT_DI && frameDtaStr->dilen <= 4)
    {
        memcpy(buf + (frmstr->DIatr & 0x7f), frameDtaStr->scmd, frameDtaStr->dilen);
        m+=frameDtaStr->dilen;
    }
    if((frmstr->len_bit & PST_FRM_BIT_DI) == PST_FRM_BIT_DI)
    {
        frame_len+=frameDtaStr->dilen;
    }
//������
    
    if(frmstr->latr & PST_LATR_NOLEN)          //û��������������
    {

    }
    else
    {
        if(frmstr->latr & PST_LATR_DBLEN)      //�����˫�ֽڳ���
        {
            len_area=2;
        }
        else
        {
            len_area=1;
        }
        if(frmstr->latr & PST_LATR_RELEN)
        {
            m+=2*len_area;
        }
        else
        {
            m+=len_area;
        }
    }
    
//������
    if((frmstr->frm_bit & PST_FRM_BIT_DATA) == PST_FRM_BIT_DATA)
    {
        {
            for(i=0;i<frameDtaStr->inum;i++)
            {
                
                if(SYS_ERR_OK != ADATA_FunctionW(frameDtaStr->item[i].id, 
                    (frameDtaStr->item[i].mult << 8)| frameDtaStr->item[i].len & 0xFF, 
                    buf+m, data))
                {
                    SYS_ERR();
                }
                else
                {
                    m+=frameDtaStr->item[i].len;
                    data_area+=frameDtaStr->item[i].len;
                }
            }
        }
    
//	        memcpy(frm->send + (frmstr->cmdlp & 0x7f), frameDtaStr->cmd, frmstr->cmdlen);
//	        m+=frmstr->cmdlen;
    }
    if((frmstr->len_bit & PST_FRM_BIT_DATA) == PST_FRM_BIT_DATA)
    {
        frame_len+=data_area;
    }
//У����    
    if((frmstr->len_bit & PST_FRM_BIT_CS) == PST_FRM_BIT_CS)
    {
        frame_len+=frmstr->cssl & 0x7f;
    }
    if((frmstr->len_bit & PST_FRM_BIT_TAIL) == PST_FRM_BIT_TAIL)           //�н�β��
    {
        frame_len+=1;                              //��������
    }

    if(frmstr->latr & PST_LATR_NOLEN)          //û��������������
    {
//	       return 0;                           //û�г������Ļ�����û�����
    }
    else                                    //������������
    {

        if(frmstr->latr & PST_LATR_DBLEN)      //�����˫�ֽڳ���
        {
            uint16_t tmp_len = (uint16_t)frame_len;
            
            tmp_len <<= frmstr->llft;
            tmp_len &= 0xffff;
            tmp_len >>= frmstr->llft;
            tmp_len <<= frmstr->lrft;
            frame_len = tmp_len;
        }
        else
        {
            uint8_t tmp_len = (uint8_t)frame_len;
        
            tmp_len <<= frmstr->llft;
            tmp_len &= 0xff;
            tmp_len >>= frmstr->llft;
            tmp_len <<= frmstr->lrft;
            frame_len = tmp_len;
        }
        
        //�����ƹ��������ֶ���ʱû���壬����������
       if(frmstr->latr & PST_LATR_DBLEN)      //�����˫�ֽڳ���
       {
           if(frmstr->latr &  PST_LATR_BFLEN) //���ֽ���ǰ
           {

               buf[frmstr->lp] = frame_len >> 8;
               buf[frmstr->lp + 1] = (uint8_t)frame_len;
               
           }
           else                            //���ֽ���ǰ
           {

               buf[frmstr->lp + 1] = frame_len >> 8;
               buf[frmstr->lp] = (uint8_t)frame_len;
               
           }
       }
       else                                //������ǵ��ֽڵ�֡��
       {
           buf[frmstr->lp] = (uint8_t)frame_len;
       }
       
    }

    if(m+3 > PST_LEN_MAX)     //�����������������С֡����֮�����ɨ���֡����ô�϶�����һ����Ч֡
    {
       return 0;                           //���ز�����Ч֡    
    }
    int sumlen = 0;
    if(!(frmstr->atr & PST_ATR_PARIT))         //��У��λ
    {
                                           //�����У�����ݵĳ���

        if(m>=frmstr->cssp)
            sumlen = m-frmstr->cssp;
        uint16_t cs = 0;
        switch(frmstr->atr & PST_ATR_CSCAL)
        {
            case PST_ATR_CSCAL_SC:            
                cs = DoSum(                 //�����ۼӺ�����
                    buf + frmstr->cssp, 
                    sumlen
                    );
                //�ۼӺ͸�ֵ
                buf[m++] = (uint8)cs;
                break;

            case PST_ATR_CSCAL_SD:            
                cs = DoSum(                 //�����ۼӺ�����
                    buf + frmstr->cssp,  
                    sumlen
                    );
                //�ۼӺ͸�ֵ
                IntToByte(buf+m, cs);
                m+=2;                                           
                break;
            case PST_ATR_CSCAL_SD1:            
                cs = DoSum(                 //�����ۼӺ�����
                    buf + frmstr->cssp,  
                    sumlen
                    );
                //�ۼӺ͸�ֵ
                cs &= 0xff;
                IntToByte(buf+m, cs);
                m+=2;                                           
                break;
            case PST_ATR_CSCAL_BS:
                cs = DoBitSum(              //����λ�Ӻ�����
                    buf + frmstr->cssp,  
                    sumlen
                    );
                //�ۼӺ͸�ֵ
                buf[m++] = (uint8)cs;
                break;

            case PST_ATR_CSCAL_CRC1:
                cs = DoCrcDlms(
                    buf + frmstr->cssp,
                    sumlen,
                    0xffff
                    );
                IntToByte(buf+m, cs);
                m+=2;
                break;
            case PST_ATR_CSCAL_CRC3:
                cs = DoCrcRtuModbus(
                    buf + frmstr->cssp,
                    sumlen,
                    0xffff
                    );
                IntToByte(buf+m, cs);
                m+=2;
                break;

            default:
                return PST_ERR_FT;                   //���ش��ʧ��
        }
//	        len += str->extlen - 1;             //��������
    }

   if(!(frmstr->atr & PST_ATR_END))           //�н�β��
   {
       buf[m++] = frmstr->end;             //�ж��Ƿ�Ϊ��β��            
   }

   *length = m;


   return PST_ERR_OK;                   //���ط�����������
}  


/************************************************************************
 * @function: _RefreshTrnFlag
 * @����: ˢ�²�������ڱ�־λ
 * 
 * @����: 
 * @param: tstno �������
 * @param: isok ��:����,��:��ʧ
 * @����: 
 * @˵��: 
 * @����: yzy (2011-9-30)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void _RefreshTrnFlag(uint8 tstno, bool isok)
{
    if(isok)
    {
        gs_SysVar.terstt.lword |= (0x1 << (16 + tstno));
        
                                            //��ͨ-->ͨ
        if(gss_ALRTStt[tstno].ew1.trn & 0x01)
        {
            gss_ALRTStt[tstno].ewf1.trn = 0x01;
        }
        gss_ALRTStt[tstno].ew1.trn = 0x00;
    }
    else
    {
         gs_SysVar.terstt.lword &= ~(0x1 << (16 + tstno));
   
                                            //ͨ-->��ͨ
        if(!(gss_ALRTStt[tstno].ew1.trn & 0x01))
        {
            gss_ALRTStt[tstno].ewr1.trn = 0x01;
        }
        gss_ALRTStt[tstno].ew1.trn = 0x01;
	
    }
}

/************************************************************************
 * @function: dataminok
 * @����: �ж�ʱ��ĺϷ���
 * 
 * @����: 
 * @param: date ������ʱ��
 * 
 * @����: 
 * @return: uint8  1���Ϸ���0���Ƿ�
 *                  
 * @˵��: 
 * @����: zjc (2014/8/21)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 dataminok(uint8* date)
{
    const uint8* monthday;
    if(date[0] < 7 || date[0] > 99)     //��ĺϷ���
    {
        return 0;
    }
    
    if((date[0] & 0x03) == 0)           //�жϽ����Ƿ�Ϊ����
    {
        monthday = guc_BYearMonth;
    }
    else
    {
        monthday = guc_LYearMonth;  
    }
    
    if(date[1] == 0 || date[1] > 12)    //�µĺϷ���
    {
        return 0;
    }
                                        //�յĺϷ���
    if(date[2] == 0 || date[2] > monthday[date[1] - 1])
    {
        return 0;
    }
    if(date[3] > 59 || date[4] > 59 )  //ʱ�ֵĺϷ���
    {
        return 0;
    }
    return 1;
}

/************************************************************************
 * @function: DLMS_UpEvtERC40414243
 * @����: ���ͱ�Ƶ��¼�ID��ERC40414243
 * 
 * @����: 
 * @param:
 * @����: 
 * @˵��: 
 * @����: zjc (2014/8/14)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	void DLMS_UpEvtERC40414243(uint8 uc_i, uint8 di, uint8 *buffer)
//	{
//	    uint8   inbuf[16];
//	    uint8   inbuflen;
//	    uint16 usyear;
//	    
//	    inbuflen = 7;
//	    inbuf[0]= buffer[13];  //�¼�����
//	    usyear=((uint16)buffer[0]<<8)+buffer[1];
//	    inbuf[5] = ByteHexToBcd(usyear-2000);       //��
//	    inbuf[4] = ByteHexToBcd(buffer[2]);    //��
//	    inbuf[3] = ByteHexToBcd(buffer[3]);    //��
//	    inbuf[2] = ByteHexToBcd(buffer[5]);    //ʱ
//	    inbuf[1] = ByteHexToBcd(buffer[6]);   //��
//	    inbuf[6] = ByteHexToBcd(buffer[7]);   //��
//	    
//	    ALRT_DLMS_ERC40_41_42_43(uc_i, di, inbuf, inbuflen);
//	}


/************************************************************************
 * @function: DLMS_PackRead
 * @����: ������ݶ�ȡ֡
 * 
 * @����: 
 * @param: buffer 
 * @param: di 
 * @param: tstno 
 * @param: cmdtype 
 * @����: 
 * @return: uint8  
 * @˵��: ������DLMS_Pack()�����ϼ򵥴����
 * @����: ZJC (2014-8-12)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	uint16 DLMS_PackRead(ST_ACT_INPBUF_L *inpLbuf, ST_ACT_INPBUF_S *inpSbuf, ST_RDMT_CMD *stpActCmd)
//	{
//	    uint8 m = 0;
//	    uint32 DLMSAddrss = 0,addr;
//	    uint16 uiCrc;
//	    uint8  parabuf[LEN_TST_CFG];
//	    
//	    inpLbuf->buf[m++] = 0x7E;  //֡ͷ
//	    inpLbuf->buf[m++] = 0xA0;
//	    m++;
//	    inpLbuf->buf[m++] = 0x00;  //�̶���ַ
//	    inpLbuf->buf[m++] = 0x02;
//	    
//	    //�������ѱ������߼���ַ
//	    
//	    addr = F10_PADDR + LEN_TST_BMPS + LEN_TST_CFG * stpActCmd->tstno;
//	    GD_Para_RW(addr, parabuf, LEN_TST_CFG, false);   //��������ַ
//	    for(uint8 uc_i = 0; uc_i < CORE_TST_NO; uc_i++)
//	    {
//	        if(STR_IsBitSet(gs_dlmsMt.eft, uc_i) &&                                     //������Ч
//	           (CmpBuffer(gs_dlmsMt.mt[uc_i].bgaddr, parabuf + 6, 6) == 0) )   //��ַ��ͬ
//	        {
//	            DLMSAddrss = uc_i + DLMS_SADDR_OFFSET;//ȡ�������еĵ�ַ
//	            break;
//	        }
//	    }
//	    if(DLMSAddrss < DLMS_SADDR_OFFSET)  //δ�ҵ���Ч���߼���ַ
//	    {
//	        return 0;
//	    }
//	
//	    DLMSAddrss <<= 1;
//	    uiCrc = DLMSAddrss & 0x00FF;
//	    DLMSAddrss <<= 1;
//	    DLMSAddrss = DLMSAddrss&0xFE00;
//	    DLMSAddrss = DLMSAddrss|uiCrc|0x01;
//	    inpLbuf->buf[m++] = DLMSAddrss>>8;
//	    inpLbuf->buf[m++] = DLMSAddrss;  //HDLC��ַ
//	    inpLbuf->buf[m++] = 0x03;//�̶���ַ
//	    
//	    
//	    if(stpActCmd->cmdtype == NETP_DLMS_TYPE_LINK)   //��·������
//	    {
//	        inpLbuf->buf[m++] = 0x93;
//	        m += 2;//CRC
//	        MoveBuffer(inpSbuf->buf, &inpLbuf->buf[m] ,inpSbuf->len);
//	        m += inpSbuf->len;  
//	        ucDlmsSendCnt = 0;
//	        ucDlmsReceCnt = 0;
//	    }
//	    else if(stpActCmd->cmdtype == NETP_DLMS_TYPE_APP)   //Ӧ�ò�����
//	    {
//	        inpLbuf->buf[m++] = 0x10;
//	        m += 2;//CRC
//	        MoveBuffer(inpSbuf->buf, &inpLbuf->buf[m] ,inpSbuf->len);
//	        m += inpSbuf->len; 
//	        ucDlmsSendCnt = 1;
//	        ucDlmsReceCnt = 0;
//	    }
//	    else
//	    {   
//	        
//	        inpLbuf->buf[m++] = ((((ucDlmsReceCnt)<<5)&0xF0)|0x10)+(((ucDlmsSendCnt)<<1)&0x0E);  //������
//	        
//	        if(stpActCmd->cmdtype == NETP_DLMS_TYPE_LINK_AF)  
//	        {
//	            inpLbuf->buf[8] = ((inpLbuf->buf[8] & 0xF0) | 0x01);
//	        }
//	        else
//	        {
//	            m += 2;//CRC
//	            inpLbuf->buf[m++] = 0xE6;
//	            inpLbuf->buf[m++] = 0xE6;
//	            inpLbuf->buf[m++] = 0x00;
//	            ucDlmsSendCnt ++;
//	        }
//	        
//	        if(stpActCmd->cmdtype == NETP_DLMS_TYPE_GET)  //��ȡ
//	        {
//	            inpLbuf->buf[m++] = 0xC0;
//	            inpLbuf->buf[m++] = 0x01;
//	            inpLbuf->buf[m++] = 0xC1;
//	        }
//	        else if(stpActCmd->cmdtype == NETP_DLMS_TYPE_GET_AF) //����֡�Ķ�ȡ��ʽ
//	        {
//	            inpLbuf->buf[m++] = 0xC0;
//	            inpLbuf->buf[m++] = 0x02;
//	            inpLbuf->buf[m++] = 0xC1;
//	        }
//	        else  if(stpActCmd->cmdtype == NETP_DLMS_TYPE_SET)  //���ñ���
//	        {
//	            inpLbuf->buf[m++] = 0xC1;
//	            inpLbuf->buf[m++] = 0x01;
//	            inpLbuf->buf[m++] = 0xC1;
//	        }
//	        else  if(stpActCmd->cmdtype == NETP_DLMS_TYPE_ACTION)  //ִ�б���
//	        {
//	            inpLbuf->buf[m++] = 0xC3;
//	            inpLbuf->buf[m++] = 0x01;
//	            inpLbuf->buf[m++] = 0xC1;
//	        }
//	        if(stpActCmd->cmdtype != NETP_DLMS_TYPE_LINK_AF)  
//	        {
//	           MoveBuffer(inpSbuf->buf, &inpLbuf->buf[m] ,inpSbuf->len);
//	            m += inpSbuf->len; 
//	        }
//	    }
//	    m += 2;//CRC
//	    inpLbuf->buf[m++] = 0x7E;//֡β
//	    inpLbuf->buf[2] = m - 2;//֡����
//	    uiCrc = DoCrcDlms(&inpLbuf->buf[1], 8, 0xffff);  //����У���
//	    inpLbuf->buf[9] = (uint8)uiCrc;
//	    inpLbuf->buf[10] = (uint8)(uiCrc>>8);
//	    uiCrc = DoCrcDlms(&inpLbuf->buf[1], m-4, 0xffff);  //����У���
//	    inpLbuf->buf[m-3] = (uint8)uiCrc;
//	    inpLbuf->buf[m-2] = (uint8)(uiCrc>>8);	
//	                     
//	    return m;
//	}

/************************************************************************
 * @function: DLMS_ExtractData
 * @����: ��ȡ��Ч��������
 * 
 * @����: 
 * @param: buffer �������ݻ���,������Ч��������
 * @param: length ����Ϊ���ջ��泤��,����Ϊ��Ч�������ݳ���
 * @param: di ��ʶ(�ձ�ʾ���б�ʶ)
 * 
 * @����: 
 * @return: uint8 NETP_ERR_OK,NETP_ERR_NT(�лظ�,����֧��),NETP_ERR_FT
 * @˵��: 
 * @����: yzy (2011-9-7)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	uint8 DLMS_ExtractData(uint8* buffer, uint16* length)
//	{
//	    PST_Frame frame;
//	    DLT_Frame* frm = &frame._dlt;
//	
//	    if(__PST_CheckFrame(PST_FRM_DLMS_HDLC_NO, buffer, length) == PST_ERR_OK)
//	    {                                                   //�����յ�������
//	        DLMS_Decode(buffer, &frame, __NULL);
//	        if(frm->len > 0)                     //ת�ƶ�ȡ��������
//	        {
//	            MoveBuffer(frm->recv, buffer, frm->len);
//	            (*length) = frm->len;
//	        }
//	
//	        SYS_OK();
//	    }
//	
//	    (*length) = 0;
//	    return NETP_ERR_FT;
//	}
uint8 NETP_Decode(uint8* receive, uint16_t len, PST_Frame* frame, NetpFrameStr * pframeStr, 
                        NetpFrameDataStr * pDataStr, uint8_t* addr, uint8_t addrlen)
{
    NETP_Frame* frm = &frame->_netp;
    S_TstCurveData stdata;
    uint8_t * data = NULL;
    uint16_t cmd = 0;
    if(addr != __NULL)                  //��Ҫ�жϵ�ַ
    {
        //�жϽ��յ��ĵ�ַ�Ƿ��Ҫ���һ��
        if(CmpBuffer(addr, receive + (pframeStr->adlp & 0x7F), addrlen) != 0)
        {
            return PST_ERR_FT;          //���ط�������
        }
    }
    memcpy(&cmd, receive+pframeStr->cmdlp, pframeStr->cmdlen);
    if(cmd != pDataStr->cmd)
    {
        //return PST_ERR_FT;
    }
    if(pframeStr->DIatr != PST_DIATR_NONE)
    {
        //����������У��
    }
    memset((uint8_t *)&stdata, 0, sizeof(S_TstCurveData));
    data = receive + pframeStr->dlp;

    
    frm->recv = receive + pframeStr->dlp;           //���յ������ݵ�������
    frm->len = len - pframeStr->dlp - (pframeStr->cssl&0x7f);              //����
    if(frm->len > 256)
    {
        frm->len = 256;
    }
    SYS_OK();
}


/************************************************************************
 * @function: DLMS_ExtractData
 * @����: ��ȡ��Ч��������
 * 
 * @����: 
 * @param: buffer �������ݻ���,������Ч��������
 * @param: length ����Ϊ���ջ��泤��,����Ϊ��Ч�������ݳ���
 * @param: di ��ʶ(�ձ�ʾ���б�ʶ)
 * 
 * @����: 
 * @return: uint8 NETP_ERR_OK,NETP_ERR_NT(�лظ�,����֧��),NETP_ERR_FT
 * @˵��: 
 * @����: yzy (2011-9-7)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 NETP_ExtractData(uint8* buffer, uint16* length, const NetpFrameStr * pframeStr, 
                            const NetpFrameDataStr * pDataStr, uint8_t *addr, uint8_t addrlen)
{
    PST_Frame frame;
    NETP_Frame* frm = &frame._netp;
//	    NetpFrameStr * pframeStr = &fw->frameStr;
//	    NetpFrameDataStr * pDataStr = &fw->frameDtaStr[index];

    if(NETP_CheckFrame2(pframeStr, buffer, length) == PST_ERR_OK)
    {                                                   //�����յ�������
        if(*length != pDataStr->rlen)
        {
            goto EX_ERR;
        }
        if(PST_ERR_OK == NETP_Decode(buffer, *length, &frame, pframeStr, pDataStr, addr, addrlen))
        {
            if(frm->len > 0)                     //ת�ƶ�ȡ��������
            {
                MoveBuffer(frm->recv, buffer, frm->len);
                (*length) = frm->len;
            }
            else
            {
                goto EX_ERR;
            }
        }
        else
        {
            goto EX_ERR;
        }
        SYS_OK();
    }
EX_ERR:
    (*length) = 0;
    return NETP_ERR_FT;
}


/************************************************************************
 * @function: Netp_ReadDLMS
 * @����: �����˿�DLMS���ݶ�ȡ����
 * 
 * @����: 
 * @param: buffer �������ݻ���
 * @param: di ���ݱ�ʶ
 * @param: length �������ݵ�Ԥ�Ƴ���
 * @param: tstno �������
 * @param: cmdtype ��������
 * @����: 
 * @return: uint8  NETP_ERR_OK,NETP_ERR_NT(�лظ�,����֧��),NETP_ERR_FT
 * @˵��: 
 * @����: ZJC (2014/8/12)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 Netp_CommonRead(ST_ACT_INPBUF_L *inpLbuf, uint8_t * data, 
                                ST_RDMT_CMD *stpActCmd)
{
    uint8 err;
    
    uart_config_t ss;                          //��ʱʱ��:��λ100ms
    
    PentCfgToSS(stpActCmd->baud_cfg, &ss);
    ST_Netp_FW * fw = stpActCmd->fw;
                                            //���
//	    inpLbuf->len = DLMS_PackRead(inpLbuf, inpSbuf, stpActCmd);

    err = NETP_DoneFrame(&fw->compFrameStr, &fw->frameDtaStr[stpActCmd->di], inpLbuf->buf, 
                    &stpActCmd->addr, data, &inpLbuf->len);
    if(err != 0)
    {
        return NETP_ERR_FT;
    }
                                            //���ݴ���ͨѶ
    if(Netp_Send(stpActCmd->port + NETP_PORT_NO, inpLbuf->buf, &inpLbuf->len, 2, &ss, &fw->frameStr) != NETP_ERR_OK)
    {
        return NETP_ERR_FT;
    }
                                            //��ȡ��������
    //len = sizeof(buffer);
    err = NETP_ExtractData(inpLbuf->buf, &inpLbuf->len, &fw->frameStr,&fw->frameDtaStr[stpActCmd->di],
                            &stpActCmd->addr,stpActCmd->addr_len);
    if(err != NETP_ERR_OK)
    {
        return err;
    }                                                 //ת�ƶ�ȡ��������
    //MoveBuffer(tmp, buffer, len);
    //inpLbuf->len = len;
    
    ucDlmsReceCnt ++;
    SYS_OK();
}

/************************************************************************
 * @function: Netp_DLMSRead
 * @����: �����˿�DLMS���ݶ�ȡ����
 * 
 * @����: 
 * @param: buffer �������ݻ���
 * @param: di ���ݱ�ʶ
 * @param: length �������ݵ�Ԥ�Ƴ���
 * @param: tstno �������
 * @param: cmdtype ��������
 * @����: 
 * @return: uint8  NETP_ERR_OK,NETP_ERR_NT(�лظ�,����֧��),NETP_ERR_FT
 * @˵��: 
 * @����: ZJC (2014/8/12)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/

//	uint8 Netp_DLMSRead(ST_ACT_INPBUF_L *inpLbuf, ST_ACT_INPBUF_S *inpSbuf, ST_RDMT_CMD *stpActCmd)
//	{   uint8 err;
//	    scm_dlms_t* scm = &gs_scmDlms;
//	    if(((scm->stt & SCM_STT_EXMASK) != SCM_STT_FIN))
//	    {
//	        ES_LED_BlinkSet(LED_MBUS, 1, 0, 0);
//	        return NETP_ERR_NT;
//	    }
//	    
//	    for(uint8 uc_i = 0; uc_i < 2; uc_i++)
//	    {
//	        err = Netp_ReadDLMS(inpLbuf, inpSbuf, stpActCmd);
//	        if((err == NETP_ERR_OK) || (err == NETP_ERR_NT))
//	        {
//	            break;
//	        }
//	    }
//	    return err;
//	}


/************************************************************************
 * @function: Act_BuildFrame
 * @����: HXING��ƣ�����������֡
 * 
 * @����: 
 * @param:
 * @����: 
 * @˵��: 
 * @����: zjc (2014/11/7)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint16_t Netp_Send_With_Retry(ST_ACT_INPBUF_L *inpLbuf, ST_RDMT_CMD *stpActCmd)
{
    uint8_t m=0;
    uint8_t err;
    
    uint16 usyear;
    ST_ACT_INPBUF_S   inBuf;

    for(uint8_t uc_i = 0; uc_i < 2; uc_i++)
    {
        err = Netp_CommonRead(inpLbuf, NULL, stpActCmd);
        if((err == NETP_ERR_OK) || (err == NETP_ERR_NT))
        {
            break;
        }
    }
    return err;
}


/************************************************************************
 * @function: Netp_mtGetEvt
 * @����: ��ȡ�¼�
 * 
 * @����: 
 * @param:
 * @����: 
 * @˵��: 
 * @����: zjc (2014/8/14)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	void Netp_mtGetEvt(ST_ACT_INPBUF_L *inpLbuf, ST_ACT_INPBUF_S *inpSbuf, ST_RDMT_CMD *stpDlmsCmd)
//	{
//	    stpDlmsCmd->cmdtype = NETP_DLMS_TYPE_GET;
//	    HX_SendDlms(inpLbuf, inpSbuf, stpDlmsCmd);   //���¼�
//	    
//	    if(inpLbuf->buf[1] == 0x01) //��֡
//	    {
//	        if(inpLbuf->len>10)
//	        {
//	            if(inpLbuf->buf[4] == 0x01 && inpLbuf->buf[5] == 0x01 )
//	            {
//	                DLMS_UpEvtERC40414243(stpDlmsCmd->tstno, stpDlmsCmd->di, &inpLbuf->buf[10]);
//	            }
//	        }
//	    }
//	    else //��֡
//	    {
//	        while(1)
//	        {
//	            if(inpLbuf->buf[1] != 0x02 || (inpLbuf->buf[3] != 0xff && inpLbuf->buf[3] != 0x00))
//	            {
//	                break;//���Ϸ�֡
//	            }
//	            if((inpLbuf->buf[4] == 0) && (inpLbuf->buf[5] == 0)&&
//	               (inpLbuf->buf[6] == 0) && (inpLbuf->buf[7] == 1))//��֡�ĵ�һ֡
//	            {
//	                DLMS_UpEvtERC40414243(stpDlmsCmd->tstno, stpDlmsCmd->di, &inpLbuf->buf[16]);
//	            }
//	            else
//	            {
//	                DLMS_UpEvtERC40414243(stpDlmsCmd->tstno, stpDlmsCmd->di, &inpLbuf->buf[14]);
//	            }
//	            
//	            if(inpLbuf->buf[3] == 0xff)  //���һ֡
//	            {
//	                break;
//	            }
//	            else
//	            {
//	                MoveBuffer(&inpLbuf->buf[4], inpSbuf->buf, 4);
//	                
//	                stpDlmsCmd->cmdtype = NETP_DLMS_TYPE_GET_AF;
//	                HX_SendDlms(inpLbuf, inpSbuf, stpDlmsCmd);   //���¼�����֡
//	            }
//	        }
//	    }
//	}
/************************************************************************
 * @function: Netp_ReadEvent
 * @����: ��ȡDLMS�¼�
 * 
 * @����: 
 * @param: index ����˿�����
 * @����: 
 * @˵��: 
 * @����: zjc (2014/8/14)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	void Netp_ReadEvent(uint8 index)
//	{
//	    ST_ACT_INPBUF_L    inLbuf;
//	    ST_ACT_INPBUF_S    inSbuf;
//	    ST_RDMT_CMD stDlmscmd;
//	    
//	    uint8 mtevtsta[4];
//	    uint8 YMDhms[6];
//	    uint8 YMDhms_1[6];
//	    uint8 flag=0;
//	    
//	    if(gs_InpExtracb[0].gbbreak)
//	    {
//	        return;
//	    }
//	                                     
//	    for(uint8 uc_i = 1; uc_i < CORE_TST_NO; uc_i++) //�ⲿ������ѭ����ȡ����
//	    {                
//	        if(gss_CDATATst[uc_i].ef != 0x01)  //Ҫ��:1.��������Ч,2.485��
//	        {
//	            continue;
//	        }
//	        HB_RetLive();                  //�����ڼ���������
//	        
//	        stDlmscmd.tstno = uc_i;
//	        stDlmscmd.cmdtype = NETP_DLMS_TYPE_GET;
//	        stDlmscmd.di = NETP_READ_DLMS_ES;  
//	        
//	        if(HX_SendDlms(&inLbuf, &inSbuf, &stDlmscmd) == NETP_ERR_OK)
//	        {
//	            MoveBuffer(&inLbuf.buf[5], mtevtsta, 4);
//	            if((mtevtsta[3] & 0x0F) != 0x00)   //���¼�����
//	            {
//	                ES_FRAM_Open();
//	                Fram_SafeRead((uint8 *)&gs_NetpReadStEVT, FM_EVTDT,sizeof(gs_NetpReadStEVT));   //�������ȡ�ϴ��¼�����ʱ��
//	                ES_FRAM_Close();
//	
//	                UpdateSysTimeStruct();  //����ϵͳʱ��
//	                
//	                if(CalcMins(gs_NetpReadStEVT[uc_i].time, 0) > CalcMins(gs_SysTime.YMDhms, 0) ||  //����ʱ����ڵ�ǰʱ��
//	                   dataminok(gs_NetpReadStEVT[uc_i].time) == 0 )  //ʱ��Ƿ�     
//	                {
//	                    MoveBuffer(gs_SysTime.YMDhms, gs_NetpReadStEVT[uc_i].time, 5);
//	                    StepDays(gs_NetpReadStEVT[uc_i].time,-1);   //ʱ����ǰһ��
//	                }
//	                MoveBuffer(gs_NetpReadStEVT[uc_i].time, YMDhms, 5); //��ʼʱ��
//	                YMDhms[5]  = 0;
//	                MoveBuffer(gs_SysTime.YMDhms,YMDhms_1,5);  //��ǰʱ��Ϊ��ֹʱ��
//	                YMDhms_1[5] = 59;
//	                MoveBuffer(YMDhms_1, gs_NetpReadStEVT[uc_i].time, 5); //ȡ��ǰʱ��Ϊ�������¼�����ʱ��
//	                flag = 1;   //�¼�����ʱ���б䶯
//	                
//	                MoveBuffer(YMDhms, inSbuf.buf, 6);
//	                MoveBuffer(YMDhms_1, inSbuf.buf + 6, 6);
//	                if((mtevtsta[3]&0x01)==0x01)  //���ֱ�׼�¼�
//	                {
//	                    stDlmscmd.cmdtype = NETP_DLMS_TYPE_GET;
//	                    stDlmscmd.di = NETP_READ_DLMS_E1;
//	                    Netp_mtGetEvt(&inLbuf, &inSbuf, &stDlmscmd);//����׼�¼�
//	                    mtevtsta[3] &= ~0x01;
//	                }
//	                if((mtevtsta[3]&0x02)==0x02)  //������թ�¼�
//	                {
//	                    stDlmscmd.cmdtype = NETP_DLMS_TYPE_GET;
//	                    stDlmscmd.di = NETP_READ_DLMS_E2;
//	                    Netp_mtGetEvt(&inLbuf, &inSbuf, &stDlmscmd);//����թ�¼�
//	                    mtevtsta[3] &= ~0x02;
//	                }
//	                if((mtevtsta[3]&0x04)==0x04)  //���̵ּ����¼�
//	                {
//	                    stDlmscmd.cmdtype = NETP_DLMS_TYPE_GET;
//	                    stDlmscmd.di = NETP_READ_DLMS_E3;
//	                    Netp_mtGetEvt(&inLbuf, &inSbuf, &stDlmscmd);//���̵����¼�
//	                    mtevtsta[3] &= ~0x04;
//	                }
//	                if((mtevtsta[3]&0x08)==0x08)  //���ֵ����¼�
//	                {
//	                    stDlmscmd.cmdtype = NETP_DLMS_TYPE_GET;
//	                    stDlmscmd.di = NETP_READ_DLMS_E4;
//	                    Netp_mtGetEvt(&inLbuf, &inSbuf, &stDlmscmd);//�������¼�
//	                    mtevtsta[3] &= ~0x08;
//	                }
//	                inSbuf.buf[0] = 0x06;
//	                MoveBuffer(mtevtsta, &inSbuf.buf[1], 4);
//	                
//	                stDlmscmd.cmdtype = NETP_DLMS_TYPE_SET;
//	                stDlmscmd.di = 0;  
//	                HX_SendDlms(&inLbuf, &inSbuf, &stDlmscmd);
//	            }
//	        }
//	    }
//	    if(flag == 1)   //
//	    {   //��֤һ�������дһ��
//	        ES_FRAM_Open();
//	        Fram_SafeWrite((uint8 *)&gs_NetpReadStEVT, FM_EVTDT, sizeof(gs_NetpReadStEVT));   //������д�¼�ʱ��
//	        ES_FRAM_Close();
//	    }
//	}

//	
//	/************************************************************************
//	 * @function: ReadDlmsLastDayData
//	 * @����: DLMS�����һ�յ��ն�������
//	 * 
//	 * @����: 
//	 * @param: rdenable  1�������һ�����ݣ�0��������������Ƿ������ 
//	 * 
//	 * @����: 
//	 * @return: uint8  
//	 * @˵��: 
//	 * @����: zjc (2014-8-16)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	void ReadDlmsLastDayData(uint8 rdenable)
//	{
//	    ST_ACT_INPBUF_L    inLbuf;
//	    ST_ACT_INPBUF_S    inSbuf;
//	    ST_RDMT_CMD stDlmscmd;
//	    
//	    S_TstDLMSExtraData tstdata;           //��ű������ʷ�ն�������
//	    S_DAYDATA_SEG0 daydata;             //����ն�����ʷ�ն�������
//	    const S_DATASEGINFO* segstr;
//	    uint8 YMDhms[6];   //ʱ��
//	    uint8 YMDhms_1[6];   //��һ��ʱ��
//	    uint32 addr;
//	    uint16 bidx;
//	    uint8 flag = 0;                     //��־�Ƿ��滻ԭ����
//	    uint8 readflag =0 , m;
//	           
//	    if(gs_InpExtracb[0].gbbreak)
//	    {
//	        return;
//	    }
//	    UpdateSysTimeStruct();  //����ϵͳʱ��
//	    
//	    //ʱ��β���
//	    MoveBuffer(gs_SysTime.YMDhms, YMDhms, 6);  //��ǰʱ��
//	    ClearBuffer(&YMDhms[3], 3);
//	    MoveBuffer(YMDhms,YMDhms_1, 6);  //��ǰʱ��
//	    StepDays(YMDhms_1, -1);      //��ǰ����һ��
//	    
//	    for(uint8 uc_k = 0; uc_k < 3; uc_k++)   //ȷ��3��
//	    {
//	        for(uint8 uc_i = 1; uc_i < CORE_TST_NO; uc_i++)  //�ⲿ������ѭ����ȡ����
//	        {    
//	            if(gss_CDATATst[uc_i].ef != 0x01)   //Ҫ��:1.��������Ч,2.485��
//	            { 
//	                continue;
//	            }
//	            HB_RetLive();                  //�����ڼ���������
//	                
//	            flag = 0;                
//	            readflag = 0 ;
//	            if(rdenable == 1)
//	            {
//	                readflag = 1;
//	            }  
//	            if((gs_SysTime.YMDhms[3] == 0) && (uc_k == 0))   //
//	            {   
//	                readflag = 1;   //ÿ�����һ��Ҫ��ȡһ�ζ�������
//	            }
//	            if(gdw_ReadDayData(YMDhms_1, uc_i, 0, (uint8*)&daydata) != SA_ERR_OK)  //���ݶ��������ԣ�����벹��
//	            {
//	                readflag = 1;
//	            }
//	            else
//	            {
//	                if(STR_IsBitClr(daydata.seg.eft, EFT_DAY_WP) || STR_IsBitClr(daydata.seg.eft, EFT_DAY_WN))  //λͼ��Ч���ٴβ���
//	                {
//	                    readflag = 1;
//	                }
//	            }
//	            if(readflag == 0)   //�����ȡ
//	            {
//	                continue;
//	            }
//	            MoveBuffer(YMDhms, inSbuf.buf, 6);   //ȡ���������ն�������
//	            MoveBuffer(YMDhms, inSbuf.buf + 6, 6);
//	            
//	            stDlmscmd.tstno = uc_i;
//	            stDlmscmd.cmdtype = NETP_DLMS_TYPE_GET;
//	            stDlmscmd.di = NETP_READ_DLMS_DE;  
//	            if(HX_SendDlms(&inLbuf, &inSbuf, &stDlmscmd) == NETP_ERR_OK)
//	            {
//	                if(inLbuf.buf[3] != 0x00 || inLbuf.buf[4] != 0x01 || inLbuf.len < 52)  //���Ϸ�֡����
//	                {
//	                    continue;
//	                }
//	                //��������
//	                m = 22;
//	                tstdata.Wp[0] = DLMS_ByteToData(inLbuf.buf, &m) * 10;
//	                tstdata.Wp[1] = DLMS_ByteToData(inLbuf.buf, &m) * 10;
//	                tstdata.Wp[2] = DLMS_ByteToData(inLbuf.buf, &m) * 10;
//	                tstdata.Wp[3] = DLMS_ByteToData(inLbuf.buf, &m) * 10;
//	                tstdata.Wp[4] = DLMS_ByteToData(inLbuf.buf, &m) * 10;
//	                tstdata.Wn[0] = DLMS_ByteToData(inLbuf.buf, &m) / 10;
//	
//	                if(CmpBuffer((uint8*)tstdata.Wp, (uint8*)daydata.seg.Wp, sizeof(daydata.seg.Wp)) || STR_IsBitClr(daydata.seg.eft, EFT_DAY_WP))
//	                {
//	                    MoveBuffer((uint8*)tstdata.Wp, (uint8*)daydata.seg.Wp, sizeof(daydata.seg.Wp));
//	                    STR_SetBit(daydata.seg.eft, EFT_DAY_WP);
//	                    flag = 1;
//	                }
//	        
//	                if(CmpBuffer((uint8*)tstdata.Wn, (uint8*)daydata.seg.Wn, sizeof(daydata.seg.Wn)) || STR_IsBitClr(daydata.seg.eft, EFT_DAY_WN))
//	                {
//	                    MoveBuffer((uint8*)tstdata.Wn, (uint8*)daydata.seg.Wn, sizeof(daydata.seg.Wn));
//	                    STR_SetBit(daydata.seg.eft, EFT_DAY_WN);
//	                    flag = 1;
//	                }
//	                                            //�����滻���ݺ�Ľṹ��
//	                if(flag)
//	                {                           //ADDRESS(DIVIDED BY TST)
//	                    BuildTimeLabel(TTYPE_YMDHMS, YMDhms, &daydata.seg.stime);
//	                    daydata.seg.rtime = gs_SysTime;
//	                                            //���浽��һ���ն���
//	                    addr = _DayDataAddr(YMDhms_1, uc_i, 0, &segstr, &bidx);
//	                    if(addr == 0xffffffff)
//	                    {
//	                        continue;
//	                    }
//	                    WriteFlashWithCRC(DB_DAY, (uint8*)&daydata, segstr->seglen, addr);
//	                    STR_ClrFlashBit(DB_DAY, 0, bidx, 0xffff);//0��Ч,1��Ч
//	                }
//	            }
//	        }
//	    }
//	}
//	
//	/************************************************************************
//	 * @function: ReadDlmsLastMonEnergy
//	 * @����: DLMS�����һ�յ��¶������
//	 * 
//	 * @����: 
//	 * @param: rdenable  1�������һ�����ݣ�0��������������Ƿ������ 
//	 * 
//	 * @����: 
//	 * @return: uint8  
//	 * @˵��: 
//	 * @����: zjc (2014-8-16)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	void ReadDlmsLastMonEnergy(uint8 rdenable)
//	{
//	    ST_ACT_INPBUF_L    inLbuf;
//	    ST_ACT_INPBUF_S    inSbuf;
//	    ST_RDMT_CMD stDlmscmd;
//	    
//	    S_TstDLMSExtraData tstdata;           //��ű������ʷ�ն�������
//	    S_DAYDATA_SEG0 daydata;             //����ն�����ʷ�ն�������
//	    const S_DATASEGINFO* segstr;
//	    uint8 YMDhms[6];   //ʱ��
//	    uint8 YMDhms_1[6];   //ʱ��
//	    uint32 addr;
//	    uint16 bidx;
//	    uint8 flag = 0;                     //��־�Ƿ��滻ԭ����
//	    uint8 readflag = 0, m; 
//	    
//	    if(gs_InpExtracb[0].gbbreak)
//	    {
//	        return;
//	    }
//	    UpdateSysTimeStruct();  //����ϵͳʱ��
//	    
//	    //ʱ��β���
//	    MoveBuffer(gs_SysTime.YMDhms,YMDhms,6);  //��ǰʱ��
//	    YMDhms[2] = 1;
//	    ClearBuffer(&YMDhms[3],3);
//	    MoveBuffer(YMDhms,YMDhms_1,6);  //��ǰʱ��
//	    StepDays(YMDhms_1, -1);      //��ǰ����һ��.
//	    
//	    for(uint8 uc_k = 0; uc_k < 3; uc_k++)   //ȷ��3��
//	    {
//	        for(uint8 uc_i = 1; uc_i < CORE_TST_NO; uc_i++)  //�ⲿ������ѭ����ȡ����
//	        {        
//	            if(gss_CDATATst[uc_i].ef != 0x01)  //Ҫ��:1.��������Ч,2.485��
//	            {
//	                continue;
//	            }
//	            HB_RetLive();                  //�����ڼ���������
//	                
//	            flag = 0;                
//	            readflag =0 ;
//	            if(rdenable == 1)
//	            {
//	                readflag = 1;
//	            } 
//	            if((gs_SysTime.YMDhms[2] == 1) && (uc_k == 0))   //��һ�ζ��¶���
//	            {   
//	                readflag = 1;   //ÿ�µ�һ��  һ��Ҫ��ȡһ�ζ�������
//	            }
//	            if(gdw_ReadMonthData(YMDhms_1, uc_i, 0, (uint8*)&daydata) != SA_ERR_OK)  //���ݶ�����������벹��һ������
//	            {
//	                readflag = 1;
//	            }
//	            else
//	            {
//	                if(STR_IsBitClr(daydata.seg.eft, EFT_MONTH_WP) || STR_IsBitClr(daydata.seg.eft, EFT_MONTH_WN))
//	                {
//	                    readflag = 1;  //����λͼʧЧ���򲹳�һ��
//	                }
//	            }
//	            if(readflag == 0)   //�����ȡ
//	            {
//	                continue;
//	            }
//	            
//	            MoveBuffer(YMDhms, inSbuf.buf, 6);   //ȡ��һ����ն�������
//	            MoveBuffer(YMDhms, inSbuf.buf + 6, 6);
//	            
//	            stDlmscmd.tstno = uc_i;
//	            stDlmscmd.cmdtype = NETP_DLMS_TYPE_GET;
//	            stDlmscmd.di = NETP_READ_DLMS_DE;  
//	            if(HX_SendDlms(&inLbuf, &inSbuf, &stDlmscmd) == NETP_ERR_OK)
//	            {
//	                if(inLbuf.buf[3] != 0x00 || inLbuf.buf[4] != 0x01 || inLbuf.len < 52)  //���Ϸ�֡����
//	                {
//	                    continue;
//	                }
//	                //��������
//	                m = 22;
//	                tstdata.Wp[0] = DLMS_ByteToData(inLbuf.buf, &m) * 10;
//	                tstdata.Wp[1] = DLMS_ByteToData(inLbuf.buf, &m) * 10;
//	                tstdata.Wp[2] = DLMS_ByteToData(inLbuf.buf, &m) * 10;
//	                tstdata.Wp[3] = DLMS_ByteToData(inLbuf.buf, &m) * 10;
//	                tstdata.Wp[4] = DLMS_ByteToData(inLbuf.buf, &m) * 10;
//	                tstdata.Wn[0] = DLMS_ByteToData(inLbuf.buf, &m) / 10;
//	
//	                if(CmpBuffer((uint8*)tstdata.Wp, (uint8*)daydata.seg.Wp, sizeof(daydata.seg.Wp)) || STR_IsBitClr(daydata.seg.eft, EFT_MONTH_WP))
//	                {
//	                    MoveBuffer((uint8*)tstdata.Wp, (uint8*)daydata.seg.Wp, sizeof(daydata.seg.Wp));
//	                    STR_SetBit(daydata.seg.eft, EFT_MONTH_WP);
//	                    flag = 1;
//	                }
//	        
//	                if(CmpBuffer((uint8*)tstdata.Wn, (uint8*)daydata.seg.Wn, sizeof(daydata.seg.Wn)) || STR_IsBitClr(daydata.seg.eft, EFT_MONTH_WN))
//	                {
//	                    MoveBuffer((uint8*)tstdata.Wn, (uint8*)daydata.seg.Wn, sizeof(daydata.seg.Wn));
//	                    STR_SetBit(daydata.seg.eft, EFT_MONTH_WN);
//	                    flag = 1;
//	                }
//	                                            //�����滻���ݺ�Ľṹ��
//	                if(flag)
//	                {                           //ADDRESS(DIVIDED BY TST)
//	                    BuildTimeLabel(TTYPE_YMDHMS, YMDhms, &daydata.seg.stime);
//	                    daydata.seg.rtime = gs_SysTime;
//	                                            //���浽��һ���ն���
//	                    addr = _MonthDataAddr(YMDhms_1, uc_i, 0, &segstr, &bidx);
//	                    if(addr == 0xffffffff)
//	                    {
//	                        continue;
//	                    }
//	                    WriteFlashWithCRC(DB_MONTH, (uint8*)&daydata, segstr->seglen, addr);
//	                    STR_ClrFlashBit(DB_MONTH, 0, bidx, 0xffff);//0��Ч,1��Ч
//	                }
//	            }
//	        }
//	    }
//	}
//	
//	/************************************************************************
//	 * @function: ReadDlmsLastMonthDMData
//	 * @����: DLMS�����һ�µ��¶�������
//	 * 
//	 * @����: 
//	 * @param: rdenable  1�������һ�����ݣ�0��������������Ƿ������ 
//	 * 
//	 * @����: 
//	 * @return: uint8  
//	 * @˵��: 
//	 * @����: zjc (2014-8-18)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	void ReadDlmsLastMonthDMData(uint8 rdenable)
//	{
//	    ST_ACT_INPBUF_L    inLbuf;
//	    ST_ACT_INPBUF_S    inSbuf;
//	    ST_RDMT_CMD stDlmscmd;
//	    
//	    S_TstDmdData tstdata;               //��ű������ʷ�¶�������
//	    S_MONDATA_SEG2 monthdata;             //����ն�����ʷ�¶�������
//	    const S_DATASEGINFO* segstr;
//	    uint8 YMDhms[6];   //ʱ��
//	    uint8  YMDhms_1[6];
//	    uint32 addr;
//	    uint16 bidx;
//	    uint8 flag = 0; 
//	    uint8 readflag;
//	    
//	    if(gs_InpExtracb[0].gbbreak)
//	    {
//	        return;
//	    }
//	    UpdateSysTimeStruct();  //����ϵͳʱ��
//	    
//	    MoveBuffer(gs_SysTime.YMDhms,YMDhms,6);  //��ǰʱ��
//	    YMDhms[2] = 1;
//	    ClearBuffer(&YMDhms[3],3);
//	    MoveBuffer(YMDhms,YMDhms_1,6);  //��ǰʱ��
//	    StepDays(YMDhms_1, -1);      //��ǰ����һ��.
//	    
//	    for(uint8 uc_k = 0; uc_k < 3; uc_k++)   //ȷ��3��
//	    {                                 
//	        for(uint8 uc_i = 1; uc_i < CORE_TST_NO; uc_i++) //�ⲿ������ѭ����ȡ����
//	        {         
//	            if(gss_CDATATst[uc_i].ef != 0x01)  //Ҫ��:1.��������Ч,2.485��
//	            {
//	                continue;
//	            }
//	            HB_RetLive();                  //�����ڼ���������
//	                
//	            flag = 0;                
//	            readflag =0 ;
//	            if(rdenable == 1)
//	            {
//	                readflag = 1;
//	            } 
//	            if((gs_SysTime.YMDhms[2] == 1) && (uc_k == 0))   //��һ�ζ��¶���
//	            {   
//	                readflag = 1;   //ÿ�µ�һ��  һ��Ҫ��ȡһ�ζ�������
//	            }
//	            if(gdw_ReadMonthData(YMDhms_1, uc_i, 2, (uint8*)&monthdata) != SA_ERR_OK)  //���ݶ�����������벹��һ������
//	            {
//	                readflag = 1;
//	            }
//	            else
//	            {
//	                if(STR_IsBitClr(monthdata.seg.eft, EFT_MON_DPP) || STR_IsBitClr(monthdata.seg.eft, EFT_MON_DPPT))
//	                {
//	                    readflag = 1;
//	                }
//	            }
//	            if(readflag == 0)   //�����ȡ
//	            {
//	                continue;
//	            }
//	            
//	            MoveBuffer(YMDhms, inSbuf.buf, 6);   
//	            MoveBuffer(YMDhms, inSbuf.buf + 6, 6);
//	            
//	            stDlmscmd.tstno = uc_i;
//	            stDlmscmd.cmdtype = NETP_DLMS_TYPE_GET;
//	            stDlmscmd.di = NETP_READ_DLMS_DM;  
//	            if(HX_SendDlms(&inLbuf, &inSbuf, &stDlmscmd) == NETP_ERR_OK)
//	            {
//	                if(inLbuf.buf[3] != 0x00 || inLbuf.buf[4] != 0x01 || inLbuf.len < 70)  //���Ϸ�֡����
//	                {
//	                    continue;
//	                }
//	                //��������
//	                tstdata.dpp[0] = DLMS_ByteToInt24(inLbuf.buf + 24, true) * 10;
//	                tstdata.dppt[0] = FMT_UnifyTime(inLbuf.buf + 27, true, true);
//	                tstdata.dpp[1] = DLMS_ByteToInt24(inLbuf.buf + 34, true) * 10;
//	                tstdata.dppt[1] = FMT_UnifyTime(inLbuf.buf + 37, true, true);
//	                tstdata.dpp[2] = DLMS_ByteToInt24(inLbuf.buf + 44, true) * 10;
//	                tstdata.dppt[2] = FMT_UnifyTime(inLbuf.buf + 47, true, true);
//	                tstdata.dpp[3] = DLMS_ByteToInt24(inLbuf.buf + 54, true) * 10;
//	                tstdata.dppt[3] = FMT_UnifyTime(inLbuf.buf + 57, true, true);
//	                tstdata.dpp[4] = DLMS_ByteToInt24(inLbuf.buf + 64, true) * 10;
//	                tstdata.dppt[4] = FMT_UnifyTime(inLbuf.buf + 67, true, true);
//	
//	                if(CmpBuffer((uint8*)tstdata.dpp, (uint8*)monthdata.seg.dpp, sizeof(monthdata.seg.dpp)) || STR_IsBitClr(monthdata.seg.eft, EFT_MON_DPP))
//	                {
//	                    MoveBuffer((uint8*)tstdata.dpp, (uint8*)monthdata.seg.dpp, sizeof(monthdata.seg.dpp));
//	                    STR_SetBit(monthdata.seg.eft, EFT_MON_DPP);
//	                    flag = 1;
//	                }
//	                
//	                if(CmpBuffer((uint8*)tstdata.dppt, (uint8*)monthdata.seg.dppt, sizeof(monthdata.seg.dppt)) || STR_IsBitClr(monthdata.seg.eft, EFT_MON_DPPT))
//	                {
//	                    MoveBuffer((uint8*)tstdata.dppt, (uint8*)monthdata.seg.dppt, sizeof(monthdata.seg.dppt));
//	                    STR_SetBit(monthdata.seg.eft, EFT_MON_DPPT);
//	                    flag = 1;
//	                }
//	                                            //�����滻���ݺ�Ľṹ��
//	                if(flag)
//	                {                           //ADDRESS(DIVIDED BY TST)
//	                    BuildTimeLabel(TTYPE_YMDHMS, YMDhms, &monthdata.seg.stime);
//	                    monthdata.seg.rtime = gs_SysTime;
//	                                            //���浽��һ���ն���
//	                    addr = _MonthDataAddr(YMDhms_1, uc_i, 2, &segstr, &bidx);
//	                    if(addr == 0xffffffff)
//	                    {
//	                        continue;
//	                    }
//	                    WriteFlashWithCRC(DB_MONTH, (uint8*)&monthdata, segstr->seglen, addr);
//	                    STR_ClrFlashBit(DB_MONTH, 0, bidx, 0xffff);//0��Ч,1��Ч
//	                }
//	            }
//	        }
//	    }
//	}



/************************************************************************
 * @function: Netp_ReadCure
 * @����: ��ȡDLMS����ͨ�����ݣ�
 * 
 * @����: 
 * @param: time  ���ߵ�ʱ��
 * @param: din   �ٲ��������
 * @param: uc_i ������
 * @param: rdtype ����������  true��ʵʱ��false������
 * @����: 
 * @˵��: 
 * @����: zjc (2014/8/14)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/


 


uint8 Netp_ReadCure(uint8 *time , uint8 *din, uint8 uc_i, PST_Netp_FW fw)
{
    ST_ACT_INPBUF_L    inLbuf;
//	    ST_ACT_INPBUF_S    inSbuf;
//	    uint8_t buf[ACT_MAX_INBUF_L_LEN];
    ST_RDMT_CMD stDlmscmd;
    S_TstCurveData tstdata;           //��ű������ʷ��������
    S_CURDATA_SEG0 curedata;             //����ն�����ʷ�ն�������
    NETP_Frame frame;
    uint8 flag = 0;                     //��־�Ƿ��滻ԭ����
    uint8 netflag = NETP_ERR_FT, m;
    
//	    HB_RetLive();                  //�����ڼ���������
    memset(&tstdata,0,LEN_CUR_SEG0);

    //���ݱ��ڶ���ʱ���ȡ��Ӧ���ն˶�������
    gdw_ReadCureData(time, uc_i, (uint8 *)&curedata); //�����ߵ�����

    for(int i = 0; i < fw->datanum; i++)
    {
//	        memset(buf, 0, ACT_MAX_INBUF_S_LEN);
//	        frame->recv = buf;
//	        frame->send = buf;
        NetpFrameDataStr * pDataStr = &fw->frameDtaStr[i];
        if( STR_IsBitSet(din ,i) )  //
        {
            stDlmscmd.port = gss_CDATATst[uc_i].pt;
            stDlmscmd.addr = gss_CDATATst[uc_i].info.main_addr;  
            stDlmscmd.addr_len = 1;
            stDlmscmd.fw = fw;
            stDlmscmd.baud_cfg = gss_CDATATst[uc_i].bs;
            stDlmscmd.di = i;
            if(Netp_Send_With_Retry(&inLbuf, &stDlmscmd) == NETP_ERR_OK)
            {
                flag = 1;
                _RefreshTrnFlag(uc_i, true);
                int m = 0;
                for(int i = 0; i < pDataStr->rinum; i++)
                {
                    switch(pDataStr->retItem[i].atr)
                    {
                        case NETP_READ_TIME:
                            break;
                        case NETP_READ_CUR_FLOW:
                            if(pDataStr->retItem[i].len < 12 && SYS_ERR_OK == ADATA_FunctionW(pDataStr->retItem[i].id, 
                                           (pDataStr->retItem[i].mult << 8)| pDataStr->retItem[i].len & 0xFF, 
                                           inLbuf.buf+m,  (uint8*)tstdata.SI))
                            {
    //	                            memcpy(tstdata.SI, inLbuf.buf+m, pDataStr->retItem[i].len);
                                if(CmpBuffer((uint8*)tstdata.SI, (uint8*)curedata.seg.SI, sizeof(curedata.seg.SI)) || 
                                    STR_IsBitClr(curedata.seg.eft, EFT_MIN_SI))
                                {
                                
                                    MoveBuffer((uint8*)tstdata.SI, (uint8*)curedata.seg.SI, sizeof(curedata.seg.SI));
                                    STR_SetBit(curedata.seg.eft, EFT_MIN_SI);
                                    flag = 1;
                                }  
                            }
                            break;
                        case NETP_READ_WORKING_CUR_FLOW:
                            if(pDataStr->retItem[i].len < 12 && SYS_ERR_OK == ADATA_FunctionW(pDataStr->retItem[i].id, 
                                           (pDataStr->retItem[i].mult << 8)| pDataStr->retItem[i].len & 0xFF, 
                                           inLbuf.buf+m,  (uint8*)tstdata.WI))
                            {
//	                                memcpy(tstdata.WI, inLbuf.buf+m, pDataStr->retItem[i].len);
                                if(CmpBuffer((uint8*)tstdata.WI, (uint8*)curedata.seg.WI, sizeof(curedata.seg.WI)) || 
                                    STR_IsBitClr(curedata.seg.eft, EFT_MIN_WI))
                                {
                                    MoveBuffer((uint8*)tstdata.WI, (uint8*)curedata.seg.WI, sizeof(curedata.seg.WI));
                                    STR_SetBit(curedata.seg.eft, EFT_MIN_WI);
                                    flag = 1;
                                }           
                            }
                            break;
                        
                        case NETP_READ_ACCUMULATE_TOTAL:
                            if(pDataStr->retItem[i].len < 12 && SYS_ERR_OK == ADATA_FunctionW(pDataStr->retItem[i].id, 
                                           (pDataStr->retItem[i].mult << 8)| pDataStr->retItem[i].len & 0xFF, 
                                           inLbuf.buf+m,  (uint8*)tstdata.ST))
                            {
//	                            memcpy(tstdata.ST, inLbuf.buf+m, pDataStr->retItem[i].len);
                                if(CmpBuffer((uint8*)tstdata.ST, (uint8*)curedata.seg.ST, sizeof(curedata.seg.ST)) || 
                                    STR_IsBitClr(curedata.seg.eft, EFT_MIN_ST))
                                {
                                
                                    MoveBuffer((uint8*)tstdata.ST, (uint8*)curedata.seg.ST, sizeof(curedata.seg.ST));
                                    STR_SetBit(curedata.seg.eft, EFT_MIN_ST);
                                    flag = 1;
                                }
                            }
                            
                            break;
                        case NETP_READ_WORKING_ACCUMULATE_TOTAL:
                            if(pDataStr->retItem[i].len < 12 && SYS_ERR_OK == ADATA_FunctionW(pDataStr->retItem[i].id, 
                                           (pDataStr->retItem[i].mult << 8)| pDataStr->retItem[i].len & 0xFF, 
                                           inLbuf.buf+m,  (uint8*)tstdata.WT))
                            {
//	                                memcpy(tstdata.WT, inLbuf.buf+m, pDataStr->retItem[i].len);
                                if(CmpBuffer((uint8*)tstdata.WT, (uint8*)curedata.seg.WT, sizeof(curedata.seg.WT)) || 
                                    STR_IsBitClr(curedata.seg.eft, EFT_MIN_WT))
                                {
                                    MoveBuffer((uint8*)tstdata.WT, (uint8*)curedata.seg.WT, sizeof(curedata.seg.WT));
                                    STR_SetBit(curedata.seg.eft, EFT_MIN_WT);
                                    flag = 1;
                                }
                            }
                            break;
                        case NETP_READ_TEMP:
                            if(pDataStr->retItem[i].len < 12 && SYS_ERR_OK == ADATA_FunctionW(pDataStr->retItem[i].id, 
                                           (pDataStr->retItem[i].mult << 8)| pDataStr->retItem[i].len & 0xFF, 
                                           inLbuf.buf+m,  (uint8*)tstdata.TMP))
                            {
//	                            memcpy(tstdata.TMP, inLbuf.buf+m, pDataStr->retItem[i].len);
                                if(CmpBuffer((uint8*)tstdata.TMP, (uint8*)curedata.seg.TMP, sizeof(curedata.seg.TMP)) || 
                                    STR_IsBitClr(curedata.seg.eft, EFT_MIN_TMP))
                                {
                                    MoveBuffer((uint8*)tstdata.TMP, (uint8*)curedata.seg.TMP, sizeof(curedata.seg.TMP));
                                    STR_SetBit(curedata.seg.eft, EFT_MIN_TMP);
                                    flag = 1;
                                }
                            }
                            break;
                        case NETP_READ_PRESSURE:
                            if(pDataStr->retItem[i].len < 12 && SYS_ERR_OK == ADATA_FunctionW(pDataStr->retItem[i].id, 
                                           (pDataStr->retItem[i].mult << 8)| pDataStr->retItem[i].len & 0xFF, 
                                           inLbuf.buf+m,  (uint8*)tstdata.P))
                            {
//	                            memcpy(tstdata.P, inLbuf.buf+m, pDataStr->retItem[i].len);
                                if(CmpBuffer((uint8*)tstdata.P, (uint8*)curedata.seg.P, sizeof(curedata.seg.P)) || 
                                    STR_IsBitClr(curedata.seg.eft, EFT_MIN_P))
                                {
                                    MoveBuffer((uint8*)tstdata.P, (uint8*)curedata.seg.P, sizeof(curedata.seg.P));
                                    STR_SetBit(curedata.seg.eft, EFT_MIN_P);
                                    flag = 1;
                                }               
                            }
                            break;
                        case NETP_READ_REMAIND_GAS:
                            if(pDataStr->retItem[i].len < 12 && SYS_ERR_OK == ADATA_FunctionW(pDataStr->retItem[i].id, 
                                           (pDataStr->retItem[i].mult << 8)| pDataStr->retItem[i].len & 0xFF, 
                                           inLbuf.buf+m,  (uint8*)tstdata.RM))
                            {
//	                            memcpy(tstdata.P, inLbuf.buf+m, pDataStr->retItem[i].len);
                                if(CmpBuffer((uint8*)tstdata.RM, (uint8*)curedata.pseg.RM, sizeof(curedata.pseg.RM)) || 
                                    STR_IsBitClr(curedata.pseg.eft, EFT_MIN_RM))
                                {
                                    MoveBuffer((uint8*)tstdata.RM, (uint8*)curedata.pseg.RM, sizeof(curedata.pseg.RM));
                                    STR_SetBit(curedata.pseg.eft, EFT_MIN_RM);
                                    flag = 1;
                                }               
                            }                            
                            break;
                        case NETP_READ_ACCUMULATE_TOTAL_PREPAY:
                            if(pDataStr->retItem[i].len < 12 && SYS_ERR_OK == ADATA_FunctionW(pDataStr->retItem[i].id, 
                                           (pDataStr->retItem[i].mult << 8)| pDataStr->retItem[i].len & 0xFF, 
                                           inLbuf.buf+m,  (uint8*)tstdata.ST))
                            {
//                              memcpy(tstdata.ST, inLbuf.buf+m, pDataStr->retItem[i].len);
                                if(CmpBuffer((uint8*)tstdata.ST, (uint8*)curedata.pseg.ST, sizeof(curedata.pseg.ST)) || 
                                    STR_IsBitClr(curedata.pseg.eft, EFT_MIN_ST))
                                {
                                
                                    MoveBuffer((uint8*)tstdata.ST, (uint8*)curedata.pseg.ST, sizeof(curedata.pseg.ST));
                                    STR_SetBit(curedata.pseg.eft, EFT_MIN_ST);
                                    flag = 1;
                                }
                            }
                            break;
                        case NETP_READ_CUR_PRICE:
                            if(pDataStr->retItem[i].len < 12 && SYS_ERR_OK == ADATA_FunctionW(pDataStr->retItem[i].id, 
                                           (pDataStr->retItem[i].mult << 8)| pDataStr->retItem[i].len & 0xFF, 
                                           inLbuf.buf+m,  (uint8*)tstdata.UP))
                            {
//                              memcpy(tstdata.ST, inLbuf.buf+m, pDataStr->retItem[i].len);
                                if(CmpBuffer((uint8*)tstdata.UP, (uint8*)curedata.pseg.UP, sizeof(curedata.pseg.UP)) || 
                                    STR_IsBitClr(curedata.pseg.eft, EFT_MIN_PRICE))
                                {
                                
                                    MoveBuffer((uint8*)tstdata.UP, (uint8*)curedata.pseg.UP, sizeof(curedata.pseg.UP));
                                    STR_SetBit(curedata.pseg.eft, EFT_MIN_PRICE);
                                    flag = 1;
                                }
                            }
                            
                            break;
                        case NETP_READ_WXIC_METER_STT:
                            if(pDataStr->retItem[i].len < 12 && SYS_ERR_OK == ADATA_FunctionW(pDataStr->retItem[i].id, 
                                           (pDataStr->retItem[i].mult << 8)| pDataStr->retItem[i].len & 0xFF, 
                                           inLbuf.buf+m,  (uint8*)tstdata.STT))
                            {
//                              memcpy(tstdata.ST, inLbuf.buf+m, pDataStr->retItem[i].len);
                                if(CmpBuffer((uint8*)tstdata.STT, (uint8*)curedata.pseg.STT, sizeof(curedata.pseg.STT)) || 
                                    STR_IsBitClr(curedata.pseg.eft, EFT_MIN_MSTT))
                                {
                                
                                    MoveBuffer((uint8*)tstdata.STT, (uint8*)curedata.pseg.STT, sizeof(curedata.pseg.STT));
                                    STR_SetBit(curedata.pseg.eft, EFT_MIN_MSTT);
                                    flag = 1;
                                }
                            }
                        case NETP_READ_VOL_HADFLOW:
                            if(pDataStr->retItem[i].len < 12 && SYS_ERR_OK == ADATA_FunctionW(pDataStr->retItem[i].id, 
                                           (pDataStr->retItem[i].mult << 8)| pDataStr->retItem[i].len & 0xFF, 
                                           inLbuf.buf+m,  (uint8*)&tstdata.vol))
                            {
//                              memcpy(tstdata.ST, inLbuf.buf+m, pDataStr->retItem[i].len);
                                if(CmpBuffer((uint8*)&tstdata.vol, (uint8*)&curedata.pseg.vol, 3) || 
                                    STR_IsBitClr(curedata.pseg.eft, EFT_MIN_VOLH))
                                {
                                
                                    MoveBuffer((uint8*)&tstdata.vol, (uint8*)&curedata.pseg.vol, 3);
                                    STR_SetBit(curedata.pseg.eft, EFT_MIN_VOLH);
                                    flag = 1;
                                }
                            }
                            
                            break;                                           
                        case NETP_READ_WARN_FLAG:
                        case NETP_READ_STT:
                        default:
                            break;
                    }
                    
                    m+=pDataStr->retItem[i].len;
                }

//	                if(CmpBuffer((uint8*)tstdata.ST, (uint8*)curedata.seg.ST, sizeof(curedata.seg.ST)) || 
//	                    STR_IsBitClr(curedata.seg.eft, EFT_MIN_ST))
//	                {
//	                    MoveBuffer((uint8*)tstdata.ST, (uint8*)curedata.seg.ST, sizeof(curedata.seg.ST));
//	                    STR_SetBit(curedata.seg.eft, EFT_MIN_ST);
//	                    flag = 1;
//	                }
//	                if(inLbuf.buf[3] == 0x00 && inLbuf.buf[4] == 0x01 && inLbuf.len >= 27)  //���Ϸ�֡����
//	                {
//	                    m = 22;
//	                    tstdata.Wp[0] = DLMS_ByteToData(inLbuf.buf, &m) / 10;
//	                    if(inLbuf.len > 28)
//	                    {
//	                        tstdata.Wp[1] = DLMS_ByteToData(inLbuf.buf, &m) / 10;
//	                        tstdata.Wp[2] = DLMS_ByteToData(inLbuf.buf, &m) / 10;
//	                        tstdata.Wp[3] = DLMS_ByteToData(inLbuf.buf, &m) / 10;
//	                        STR_SetBit(curedata.seg.eft, EFT_MIN_WPF);
//	                    }
//	                    else
//	                    {
//	                        STR_ClrBit(curedata.seg.eft, EFT_MIN_WPF);
//	                    }
//	                    if(CmpBuffer((uint8*)tstdata.Wp, (uint8*)curedata.seg.Wp, sizeof(curedata.seg.Wp)) || STR_IsBitClr(curedata.seg.eft, EFT_MIN_WP))
//	                    {
//	                        MoveBuffer((uint8*)tstdata.Wp, (uint8*)curedata.seg.Wp, sizeof(curedata.seg.Wp));
//	                        STR_SetBit(curedata.seg.eft, EFT_MIN_WP);
//	                        flag = 1;
//	                    }
//	                }
                netflag = NETP_ERR_OK;
            }
            else
            {
                _RefreshTrnFlag(uc_i, false);
            }
        }


    }



//	    MoveBuffer(time, inSbuf.buf, 6);   
//	    MoveBuffer(time, inSbuf.buf + 6, 6);
//	    
//	    stDlmscmd.tstno = uc_i;
//	    stDlmscmd.cmdtype = NETP_DLMS_TYPE_GET;
    

//	    if( STR_IsBitSet(din ,NETP_READ_DLMS_WN) && ((netflag == NETP_ERR_OK) || (rdtype == false)))  //�޹����ܸ���
//	    {
//	        stDlmscmd.di = NETP_READ_DLMS_WN;  
//	        if(HX_SendDlms(&inLbuf, &inSbuf, &stDlmscmd) == NETP_ERR_OK)
//	        {
//	            if(inLbuf.buf[3] == 0x00 && inLbuf.buf[4] == 0x01 && inLbuf.len >= 27)  //���Ϸ�֡����
//	            {
//	                m = 22;
//	                tstdata.Wn[0] = DLMS_ByteToData(inLbuf.buf, &m) / 10;
//	                if(inLbuf.len > 28)
//	                {
//	                    tstdata.Wn[1] = DLMS_ByteToData(inLbuf.buf, &m) / 10;
//	                    tstdata.Wn[2] = DLMS_ByteToData(inLbuf.buf, &m) / 10;
//	                    tstdata.Wn[3] = DLMS_ByteToData(inLbuf.buf, &m) / 10;
//	                    STR_SetBit(curedata.seg.eft, EFT_MIN_WNF);
//	                }
//	                else
//	                {
//	                    STR_ClrBit(curedata.seg.eft, EFT_MIN_WNF);
//	                }
//	                if(CmpBuffer((uint8*)tstdata.Wn, (uint8*)curedata.seg.Wn, sizeof(curedata.seg.Wn)) || STR_IsBitClr(curedata.seg.eft, EFT_MIN_WN))
//	                {
//	                    MoveBuffer((uint8*)tstdata.Wn, (uint8*)curedata.seg.Wn, sizeof(curedata.seg.Wn));
//	                    STR_SetBit(curedata.seg.eft, EFT_MIN_WN);
//	                    flag = 1;
//	                }
//	            }
//	            netflag = NETP_ERR_OK;
//	        }
//	    }

    if(flag)
    {                           //ADDRESS(DIVIDED BY TST)
        BuildTimeLabel(TTYPE_YMDHMS, time, &curedata.seg.stime);
        curedata.seg.dev = gss_CDATATst[uc_i].info.addr;  
        memcpy(curedata.seg.factory_id, fw->device_name, 4);
        UpdateSysTimeStruct();  //����ϵͳʱ��
        curedata.seg.rtime = FMT_UnifyTime(gs_SysTime.YMDhms, false, true);   //���泭��ʱ��
        gdw_SaveCureData(time, uc_i, (uint8*)&curedata);  //���浽
    }
    return netflag;
}

ST_Netp_Reg gs_netpRegMap[CON_MAX_DEVICE_TYPES];

int Netp_Register_Init(void)
{
    int i = 0;
    int m = 0;
    uint8_t tmp[10];
    uint8_t ret = 0;
    memset((uint8_t *)&gs_netpRegMap[0], 0, sizeof(ST_Netp_Reg) * CON_MAX_DEVICE_TYPES);
    for(int i = 0; i < CON_MAX_DEVICE_TYPES; i++)
    {
        memset(tmp,0,10);
        ret = Netp_Framework_RW(NETP_FW_PADDR + i * NETP_FW_PLEN, tmp, sizeof(ST_Netp_Reg), false);
        if(SYS_ERR_OK == ret && tmp[0] == i)
        {
            memcpy((uint8_t *)&gs_netpRegMap[m++],tmp, sizeof(ST_Netp_Reg));
        }
    }
    gs_SysVar.netpRegNum = m;
    
    return 0;
}



int Netp_Register_Set(ST_NETP_FW_BASE * fw)
{
    int ret = 0;
    if(fw->pf >= CON_MAX_DEVICE_TYPES)
    {
        return -1;
    }
    ret = Netp_Framework_RW(NETP_FW_PADDR + fw->pf  * NETP_FW_PLEN, (uint8_t *)fw, NETP_FW_PLEN, true);
    if(SYS_ERR_OK == ret)
    {
//	        if(gs_SysVar.netpRegNum < MAX_DEVICE_TYPES)
//	            memcpy((uint8_t *)&gs_netpRegMap[gs_SysVar.netpRegNum++],(uint8_t *)fw, sizeof(ST_Netp_Reg));
        return 0;
    }
    return -1;
}

int Netp_Register_Check(uint8_t pf, PST_Netp_FW fw)
{
    return -1;
}


int Netp_Get_Framework(uint8_t pf, PST_Netp_FW fw)
{
    if(pf >= CON_MAX_DEVICE_TYPES)
    {
        return -1;
    }

    int ret = Netp_Framework_RW(NETP_FW_PADDR + pf * NETP_FW_PLEN, fw, NETP_FW_PART_PLEN, false);

    if(SYS_ERR_OK == ret)
    {
        if(fw->pf < CON_MAX_DEVICE_TYPES && IsAsciiChar(fw->device_name[0])
            && IsAsciiChar(fw->device_name[1]) && IsAsciiChar(fw->device_name[2])
            && IsAsciiChar(fw->device_name[3]))
        {
            SYS_OK();
        }
        else
        {
            SYS_ERR();
        }
    }
    else
    {
        SYS_ERR();
    }
}
//	#pragma location = ".mmm"
ST_Netp_FW gs_netp_fw_tmp[MCB_PORT_NUM];
/************************************************************************
 * @function: Netp_ReadCureData
 * @����: ��ȡDLMS����ͨ�����ݣ�����15�������ߣ�
 * 
 * @����: 
 * @param: index ����˿�����
 * @����: 
 * @˵��: 
 * @����: zjc (2014/8/14)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void Netp_ReadCureData(uint8 index)
{   
    uint8 curbitmap[5];  //40bit
    uint8 YMDhms[6];   //ʱ��
    uint8 ucmin;
    uint8_t ret = 0;
    ST_Netp_FW * fw = &gs_netp_fw_tmp[index];
    
    ClearBuffer(curbitmap, sizeof(curbitmap));
//    STR_SetBits(curbitmap, 1, NETP_READ_TX13_F, 1);  //����1.3
//	    STR_SetBits(curbitmap, 1, NETP_READ_DLMS_WN, 1);  //�������޹�����
//	    STR_SetBits(curbitmap, 1, NETP_READ_DLMS_U, 1);  //����ѹ
//	    STR_SetBits(curbitmap, 1, NETP_READ_DLMS_I, 1);  //������
//	    STR_SetBits(curbitmap, 1, NETP_READ_DLMS_P, 1);  //������
    
//	    if(gs_InpExtracb[0].gbbreak)
//	    {
//	        return;
//	    }
    UpdateSysTimeStruct();  //����ϵͳʱ��
    
    MoveBuffer(gs_SysTime.YMDhms, YMDhms, 6);  //��ǰʱ��  
    ucmin = YMDhms[4] % gs_PstPara.cp_interval;
    YMDhms[4] = YMDhms[4] - ucmin;   //ȡ�ϴ� ���߱�����ӵ�
    YMDhms[5] = 0;  //��
                              //�ⲿ������ѭ����ȡ����
    for(uint8 uc_i = 0; uc_i < CORE_TST_NO; uc_i++)
    {       
        if(!((gss_CDATATst[uc_i].info.type == 0x01 || gss_CDATATst[uc_i].info.type == 0x03)
            && gss_CDATATst[uc_i].pt == index))   //Ҫ��:1������ 3IC��������
        {
            continue;
        }
        ret = Netp_Get_Framework(gss_CDATATst[uc_i].pf, fw);
//	            uint8_t pnum = GetP
        if(SYS_ERR_OK == ret && fw->pf < CON_MAX_DEVICE_TYPES)
        {
            for(uint8_t uc_j = 0; uc_j < fw->datanum; uc_j++)
            {
                
                STR_SetBits(curbitmap, 1, uc_j, 1);
            }
            Netp_ReadCure(YMDhms, curbitmap, uc_i, fw);   //��ʵʱ����
        }
    }
    memcpy(gs_NetpReadStt[index].extime.YMDhms, YMDhms, 6);
//	    for(uint8 uc_i = 0; uc_i < 2; uc_i++)   //��ѯ����
//	    {
//	        Netp_ReReadCureData(0, 4);         //ÿ�β�ѯǰ��4����
//	    }
}


/************************************************************************
 * @function: Netp_ReReadCureData
 * @����: ÿ��һ��ʱ��β�ѯ��ʱ����������Ƿ���©������©���߽��в���
 * 
 * @����: 
 * @param: index ����˿�����
 * @param: readnum ������
 * @����: 
 * @˵��: 
 * @����: zjc (2014/8/14)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	void Netp_ReReadCureData(uint8 index, uint8 readnum)
//	{
//	    uint8 curbitmap[5];  //40bit
//	    S_CURDATA_SEG0 curedata;             //����ն�����ʷ�ն�������
//	    uint8 YMDhms[6],YMDhmsbf[6];   //
//	    uint8 ucmin;
//	    
//	//	    if(gs_InpExtracb[0].gbbreak)
//	//	    {
//	//	        return;
//	//	    }
//	    UpdateSysTimeStruct();  //����ϵͳʱ��
//	    
//	    MoveBuffer(gs_SysTime.YMDhms, YMDhmsbf, 6);  //��ǰʱ��
//	    ucmin = YMDhmsbf[4] % gs_PstPara.cp_interval;
//	    YMDhmsbf[4] = YMDhmsbf[4] - ucmin;   //ȡ�ϴ� ���߱�����ӵ�
//	    YMDhmsbf[5] = 0;  //��
//	    StepMins(YMDhmsbf, -((readnum - 1) * 15));  
//	    
//	    for(uint8 uc_i = 0; uc_i < CORE_TST_NO; uc_i++)
//	    {   
//	//	        if(gss_CDATATst[uc_i].ef != 0x01)   //Ҫ��:1.��������Ч,2.485��
//	//	        {
//	//	            continue;
//	//	        }
//	        MoveBuffer(YMDhmsbf, YMDhms, 6);
//	        
//	        for(uint8 uc_j = 0; uc_j< readnum; uc_j++)   // ��ʱ��������еĵ���
//	        {
//	            ClearBuffer(curbitmap, sizeof(curbitmap));
//	            if(gdw_ReadCureData(YMDhms, uc_i, (uint8 *)&curedata) != SYS_ERR_OK) //��ȡ�������ݴ�����ȫ��������
//	            {
//	                STR_SetBits(curbitmap, 1, NETP_READ_DLMS_WP, 1);  //�������й�����
//	                STR_SetBits(curbitmap, 1, NETP_READ_DLMS_WN, 1);  //�������޹�����
//	                STR_SetBits(curbitmap, 1, NETP_READ_DLMS_U, 1);  //����ѹ
//	                STR_SetBits(curbitmap, 1, NETP_READ_DLMS_I, 1);  //������
//	                STR_SetBits(curbitmap, 1, NETP_READ_DLMS_P, 1);  //������
//	            }
//	            else
//	            {
//	                if(STR_IsBitClr(curedata.seg.eft, EFT_MIN_WP))  //�����й�����  �Ƿ���Ч
//	                {
//	                    STR_SetBits(curbitmap, 1, NETP_READ_DLMS_WP, 1);  
//	                }
//	                if(STR_IsBitClr(curedata.seg.eft, EFT_MIN_WN))  //�����޹�����    �Ƿ���Ч
//	                {
//	                    STR_SetBits(curbitmap, 1, NETP_READ_DLMS_WN, 1);  
//	                }
//	                if(STR_IsBitClr(curedata.seg.eft, EFT_MIN_U))  //��ѹ�Ƿ���Ч
//	                {
//	                    STR_SetBits(curbitmap, 1, NETP_READ_DLMS_U, 1);  
//	                }
//	                if(STR_IsBitClr(curedata.seg.eft, EFT_MIN_I))  //�����Ƿ���Ч
//	                {
//	                    STR_SetBits(curbitmap, 1, NETP_READ_DLMS_I, 1);
//	                }
//	                if(STR_IsBitClr(curedata.seg.eft, EFT_MIN_P))  //����  �Ƿ���Ч
//	                {
//	                    STR_SetBits(curbitmap, 1, NETP_READ_DLMS_P, 1);  
//	                }
//	            }
//	            if( STR_CalcBitSetNum(curbitmap ,sizeof(curbitmap)) )
//	            {
//	                if(Netp_ReadCure(YMDhms ,curbitmap, uc_i, false) != NETP_ERR_OK)   //��ʵʱ����
//	                {
//	                    //�����һ���㲹�������κη��أ����ٲ������ĵ㣬��ֹ������ʱ�����Ӱ������ĳ���
//	                    break;
//	                }
//	            }
//	            StepMins(YMDhms, 15);  //����15���ӣ���ѯ��һ�����ߵ�
//	        }
//	    }
//	}



unsigned char GetProtocolNum(unsigned char *tMan, unsigned char *tPro)
{
    unsigned char tDeviceMan[4], i, re;
    unsigned char DEVICE_MAN[5];
    tDeviceMan[0] = *(tMan + 0);
    tDeviceMan[1] = *(tMan + 1);
    tDeviceMan[2] = *(tPro + 0);
    tDeviceMan[3] = *(tPro + 1);
    re = 0xff;
    
    for(i= 0; i < CON_MAX_DEVICE_TYPES; i++)
    {
        
        Netp_Framework_RW(NETP_FW_PADDR+i*NETP_FW_PLEN, DEVICE_MAN, 5, false);
        if(memcmp(&DEVICE_MAN[1], tDeviceMan, 4) == 0)
        {
            re = DEVICE_MAN[0];
            break;
        }
    }
    return re;
}

