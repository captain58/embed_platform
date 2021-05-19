/****************************************Copyright (c)**************************************************
**                                  ____�Ƽ����޹�˾
**                                          ������
**
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: core.c
**��   ��   ��: yzy
**����޸�����: 2019��6��18��
**��        ��: ������������ݴ���������ģ��
**ע        ��:
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: yzy
** ��  ��: v1.0
** �ա���: 2019��6��18��
** �衡��: ԭʼ�汾
********************************************************************************************************/
#define EXT_CORE
#include "public.h"

#include "cdata_func.c"
#include "cdata_addr.c"
#include "cdata_tobg.c"

#include "ext.h"
#include "core.h"
#include "lib_com/calclib_gw.h"

#include "netp.h"
#include "paradef.h"
#include "math.h"
/************************************************************************
**�������ݱ�
*************************************************************************/
const CDATA gss_CDATAExt[] =
{
    /************************************************************************/
    //645-2007.AFN0Cʵʱ������ʶ 
    /************************************************************************/
    {
        0x9EA0,         //A��+W-W+Q-Q(9EA0-9EA3)
        0x8003,
        CORE_TATR_TM | CORE_TATR_MT,
        4,
        CDATA_SelfSelectR,
    },
    {
        0x9EA4,         //B��+W-W+Q-Q(9EA4-9EA7)
        0x8003,
        CORE_TATR_TM | CORE_TATR_MT,
        4,
        CDATA_SelfSelectR,
    },
    {
        0x9EA8,         //C��+W-W+Q-Q(9EA8-9EAB)
        0x8003,
        CORE_TATR_TM | CORE_TATR_MT,
        4,
        CDATA_SelfSelectR,
    },
    {
        0x9EAC,         //��һ������A��+W-W+Q-Q(9EAC-9EAF)
        0x8003,
        CORE_TATR_TM | CORE_TATR_MT,
        4,
        CDATA_SelfSelectR,
    },
    {
        0x9EB0,         //��һ������B��+W-W+Q-Q(9EB0-9EB3)
        0x8003,
        CORE_TATR_TM | CORE_TATR_MT,
        4,
        CDATA_SelfSelectR,
    },
    {
        0x9EB4,         //��һ������C��+W-W+Q-Q(9EB4-9EB7)
        0x8003,
        CORE_TATR_TM | CORE_TATR_MT,
        4,
        CDATA_SelfSelectR,
    },
    //---------------
    {
        0x9EC0,         //��һ������+W-W+Q-QQ1Q2Q3Q4(9EC0-9EC7)
        0x8007,
        CORE_TATR_TM | CORE_TATR_MT,
        20,
        CDATA_SelfSelectR,
    },
    //---------------
    {
        0x9ECC,         //+W-W+Q-Q������ʱ��(9ECC-9ECF)
        0x8003,
        CORE_TATR_TM | CORE_TATR_MT,
        40,
        CDATA_SelfSelectR,
    },
    {
        0x9ED0,         //��һ������+W-W+Q-Q������ʱ��(9ED0-9ED3)
        0x8003,
        CORE_TATR_TM | CORE_TATR_MT,
        40,
        CDATA_SelfSelectR,
    },

    /************************************************************************/
    //645-1997.AFN0Cʵʱ������ʶ 
    /************************************************************************/
    {
        0x9F00,         //����+W
        0x0000,
        CORE_TATR_TM | CORE_TATR_MT,
        20,
        CDATA_SelfSelectR,
    },
    {
        0x9F01,         //����+Q
        0x0000,
        CORE_TATR_TM | CORE_TATR_MT,
        20,
        CDATA_SelfSelectR,
    },
    {
        0x9F02,         //����-W
        0x0000,
        CORE_TATR_TM | CORE_TATR_MT,
        20,
        CDATA_SelfSelectR,
    },
    {
        0x9F03,         //����-Q
        0x0000,
        CORE_TATR_TM | CORE_TATR_MT,
        20,
        CDATA_SelfSelectR,
    },
    {
        0x9F04,         //����Q1
        0x0000,
        CORE_TATR_TM | CORE_TATR_MT,
        20,
        CDATA_SelfSelectR,
    },
    {
        0x9F05,         //����Q2
        0x0000,
        CORE_TATR_TM | CORE_TATR_MT,
        20,
        CDATA_SelfSelectR,
    },
    {
        0x9F06,         //����Q3
        0x0000,
        CORE_TATR_TM | CORE_TATR_MT,
        20,
        CDATA_SelfSelectR,
    },
    {
        0x9F07,         //����Q4
        0x0000,
        CORE_TATR_TM | CORE_TATR_MT,
        20,
        CDATA_SelfSelectR,
    },
    {
        0x9F08,         //�����й����� 
        0x0000,
        CORE_TATR_TM | CORE_TATR_MT,
        15,
        CDATA_SelfSelectR,
    },
    {
        0x9F09,         //�����й�����ʱ��
        0x0000,
        CORE_TATR_TM | CORE_TATR_MT,
        20,
        CDATA_SelfSelectR,
    },
    {
        0x9F0A,         //�����޹�����
        0x0000,
        CORE_TATR_TM | CORE_TATR_MT,
        15,
        CDATA_SelfSelectR,
    },
    {
        0x9F0B,         //�����޹�����ʱ��
        0x0000,
        CORE_TATR_TM | CORE_TATR_MT,
        20,
        CDATA_SelfSelectR,
    },
    {
        0x9F0C,         //�����й�����
        0x0000,
        CORE_TATR_TM | CORE_TATR_MT,
        15,
        CDATA_SelfSelectR,
    },
    {
        0x9F0D,         //�����й�����ʱ��
        0x0000,
        CORE_TATR_TM | CORE_TATR_MT,
        20,
        CDATA_SelfSelectR,
    },
    {
        0x9F0E,         //�����޹�����
        0x0000,
        CORE_TATR_TM | CORE_TATR_MT,
        15,
        CDATA_SelfSelectR,
    },
    {
        0x9F0F,         //�����޹�����ʱ��
        0x0000,
        CORE_TATR_TM | CORE_TATR_MT,
        20,
        CDATA_SelfSelectR,
    },
    {
        0x9F10,         //���������й�����
        0x0000,
        CORE_TATR_TM | CORE_TATR_MT,
        15,
        CDATA_SelfSelectR,
    },
    {
        0x9F11,         //���������й�����ʱ��
        0x0000,
        CORE_TATR_TM | CORE_TATR_MT,
        20,
        CDATA_SelfSelectR,
    },
    {
        0x9F12,         //���������޹�����
        0x0000,
        CORE_TATR_TM | CORE_TATR_MT,
        15,
        CDATA_SelfSelectR,
    },
    {
        0x9F13,         //���������޹�����ʱ��
        0x0000,
        CORE_TATR_TM | CORE_TATR_MT,
        20,
        CDATA_SelfSelectR,
    },
    {
        0x9F14,         //���·����й�����
        0x0000,
        CORE_TATR_TM | CORE_TATR_MT,
        15,
        CDATA_SelfSelectR,
    },
    {
        0x9F15,         //���·����й�����ʱ��
        0x0000,
        CORE_TATR_TM | CORE_TATR_MT,
        20,
        CDATA_SelfSelectR,
    },
    {
        0x9F16,         //���·����޹�����
        0x0000,
        CORE_TATR_TM | CORE_TATR_MT,
        15,
        CDATA_SelfSelectR,
    },
    {
        0x9F17,         //���·����޹�����ʱ��
        0x0000,
        CORE_TATR_TM | CORE_TATR_MT,
        20,
        CDATA_SelfSelectR,
    },

};


//Ӧ�ò���ں����ݱ��С
const uint16 gui_CoreTblExtSize = sizeof(gss_CDATAExt)/sizeof(CDATA);













//-------------------------------------------------------------------






/***************************************************************************************************
** ��������: uint8 CDATA_GetTime(uint16 id, uint16 tstp, uint8* buffer, const CDATA* cd)
** ��������: ��ȡ��ǰʱ��
** �䡡��:  id: �ں˱�ʶ
**          tstp: �������
**          buffer: ����
**          cd:  �ں˽ṹ��
** �䡡��:
**
** ȫ�ֱ���: ��
** ����ģ��: ��
**
** ������: yzy
** �ա���: 2007��11��16��
**--------------------------------------------------------------------------------------------------
** �޸���:
** �ա���:
**--------------------------------------------------------------------------------------------------
***************************************************************************************************/
uint8 CDATA_GetTime(uint16 id, uint16 tstp, uint8* buffer, const CDATA* cd)
{
    gs_RealTime.ReadBCDDateTime((TIME*)buffer);
    SYS_OK();
}


/******************************************************************************
**common���ĺ��������
******************************************************************************/
const CDATAFunc gss_CDATAFuncIn[] =
{
    {
        0x0128,
        0x0000,
        CDATA_GetTime,
    },
};

/*********************************************************************************************************
** ��������: uint8 CDATA_FunctionR(uint16 id, uint16 tstp, uint8* buffer, const CDATA* cd)
** ��������: ͨ����������������
** �䡡��:  id ���ݱ�ʶ
            tstp bit0 - bit5 ������� bit6-bit15 ͬ������
            buffer ��ȡ���ݵĻ���
            cd �ں˱��е�һ������
** �䡡��: ��
**
** ȫ�ֱ���: ��
** ����ģ��: ��
**
** ������: yzy
** �ա���: 2007��9��15��
**-------------------------------------------------------------------------------------------------------
** �޸���:
** �ա���:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
uint8 CDATA_FunctionR(uint16 id, uint16 tstp, uint8* buffer, const CDATA* cd, bool oc)
{
    uint16 index;                       //��ַ����
    const CDATAFunc* cf;                //�����Ϣ

    index = MidWaySearch(               //�������ݱ�ʶ
        (uint8*)gss_CDATAFuncIn,
        sizeof(gss_CDATAFuncIn) / sizeof(CDATAFunc),
        sizeof(CDATAFunc),
        cd->id
        );

    if(index == 0xFFFF)                 //�����������
    {
        _IF_TRUE_RETURN_Z(gui_CoreFuncExtTblSize ==0, CDATA_ERR_DI);
        index = MidWaySearch(           //�������ݱ�ʶ
            (uint8*)gss_CDATAFuncExt,
            gui_CoreFuncExtTblSize,
            sizeof(CDATAFunc),
            cd->id
            );
        _IF_TRUE_RETURN_Z(index == 0xFFFF, CDATA_ERR_DI);
        cf = gss_CDATAFuncExt + index;
    }
    else
    {
        cf = gss_CDATAFuncIn + index;   //��ȡ���ҵ�����������
    }
                                        //ͨ���������÷�������
    return cf->func(id, tstp, buffer, cd);
}


uint8_t ADATA_GeneralStr(uint16 id, uint16 tstp, uint8_t* buffer, uint8_t* data)
{
    if(buffer == NULL)
    {
        SYS_ERR();
    }

    if(id == NETP_D_Ty_A_N)
    {
        memset(buffer, 0, (tstp & 0xFF));
    }
    else if(id == NETP_D_Ty_WXIC_PREPAY)
    {
//	        memset(buffer, 0, (tstp & 0x7F));
        GD_Para_RW(PREPAYPARA_PADDR + (tstp >> 8) * PREPAYPARA_PER_LEN + 5 , buffer, (tstp & 0xFF), false);
    }
    else if(id == NETP_D_Ty_STR)
    {
        if(data == NULL)
        {
            SYS_ERR();
        }
        memcpy(data, buffer, (tstp & 0xFF));
    }
    else if(id == NETP_D_STR_DATA_TO_BUF)
    {
        if(data == NULL || buffer == NULL)
        {
            SYS_ERR();
        }
        memcpy(buffer, data, (tstp & 0xFF));
    }

    SYS_OK();
}
//����13�ۼ���
typedef struct
{
    uint8_t declen:2;
    uint8_t integer:3;
    uint8_t ranklen:2;
    uint8_t rank:1;
}ST_TXFLOAT;

float Tancy_Calculate_Float(unsigned char *buffer)
{
    uint8_t power = buffer[0] & 0x7f;
    
    uint32_t tmp = RByteToInt24(buffer+1);
    double yard = (double)pow(2,power) * ((double)tmp / 0x800000);
    if((buffer[1] & 0x80) == 0x80)
    {
        return (float)(-yard);
    }
    else
    {
        return (float)yard;
    }

}

uint8_t ADATA_TXATFload(uint16 id, uint16 tstp, uint8_t* buffer, uint8_t* data)
{
    double sum = 0;
    if(id == NETP_D_Ty_DOUBLE)
    {
        
        uint32_t hvalue = LongBcdToHex(RByteToInt(buffer));
        
        double sum = (double)hvalue * 1000000;
//	        uint8_t power = buffer[2] & 0x7f;
//	
//	        uint32_t tmp = RByteToInt24(buffer+3);
//	        double yard = (double)pow(2,power) * ((double)tmp / 0x800000);
//	        if((buffer[3] & 0x80) == 0x80)
//	        {
//	            sum -= yard;
//	        }
//	        else
//	        {
//	            sum += yard;
//	        }
        sum += Tancy_Calculate_Float(buffer + 2);

        uint64_t integer = (uint64_t)sum;
        uint64_t decimal =  (uint64_t)(sum * 10000);
        decimal %= 10000;
        memcpy(data, &integer,4);
        
        memcpy(data+4, &decimal,2);
        
        SYS_OK();
    }
    else if(id == NETP_D_Ty_FLOAT)
    {

        sum = Tancy_Calculate_Float(buffer);
        
        uint64_t integer = (uint64_t)sum;
        uint64_t decimal =  (uint64_t)(sum * 10000);
        decimal %= 10000;
        memcpy(data, &integer,4);
        
        memcpy(data+4, &decimal,2);
        
        SYS_OK();

    }
    SYS_ERR();

}
/******************************************************************************
**common���ĺ��������
******************************************************************************/
const ADATAFunc gss_ADATAFuncIn[] =
{
    {
        NETP_D_Ty_A_N,
        0x0000,
        ADATA_GeneralStr,
    },
    
    {
        NETP_D_Ty_WXIC_PREPAY,
        0x0000,
        ADATA_GeneralStr,
    },   
    {
        NETP_D_Ty_STR,
        0x0000,
        ADATA_GeneralStr,
    },    
    {
        NETP_D_STR_DATA_TO_BUF,
        0x0000,
        ADATA_GeneralStr,
    },    
    {
        NETP_D_Ty_FLOAT,
        0x0000,
        ADATA_TXATFload,
    },      
    
    //120
    {
        NETP_D_Ty_DOUBLE,
        0x0000,
        ADATA_TXATFload,
    },      
};

/*********************************************************************************************************
** ��������: uint8 ADATA_FunctionW(uint16 id, uint16 tstp, uint8* buffer, const CDATA* cd)
** ��������: ͨ����������������
** �䡡��:  id ���ݱ�ʶ
            tstp bit0 - bit7���� 
            buffer ��ȡ���ݵĻ���
            cd �ں˱��е�һ������
** �䡡��: ��
**
** ȫ�ֱ���: ��
** ����ģ��: ��
**
** ������: yzy
** �ա���: 2007��9��15��
**-------------------------------------------------------------------------------------------------------
** �޸���:
** �ա���:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
uint8_t ADATA_FunctionW(uint16_t id, uint16_t tstp, uint8_t* buffer, uint8_t* data)
{
    uint16 index;                       //��ַ����
    const ADATAFunc* cf;                //�����Ϣ

    index = MidWaySearch(               //�������ݱ�ʶ
        (uint8*)gss_ADATAFuncIn,
        sizeof(gss_ADATAFuncIn) / sizeof(ADATAFunc),
        sizeof(ADATAFunc),
        id
        );

    if(index == 0xFFFF)                 //�����������
    {
//	        _IF_TRUE_RETURN_Z(gui_CoreFuncExtTblSize ==0, CDATA_ERR_DI);
//	        index = MidWaySearch(           //�������ݱ�ʶ
//	            (uint8*)gss_CDATAFuncExt,
//	            gui_CoreFuncExtTblSize,
//	            sizeof(CDATAFunc),
//	            cd->id
//	            );
//	        _IF_TRUE_RETURN_Z(index == 0xFFFF, CDATA_ERR_DI);
//	        cf = gss_CDATAFuncExt + index;
        SYS_ERR();
    }
    else
    {
        cf = gss_ADATAFuncIn + index;   //��ȡ���ҵ�����������
    }
                                        //ͨ���������÷�������
    return cf->func(id, tstp, buffer, data);
}

/******************************************************************************
**common���ĵ�ַ��
******************************************************************************/
const CDATAAddr gss_CDATAAddrIn[] =
{
    {
        0x0000,                         //����汾��
        0xF3,                           //�����ڴ�ƫ��
        0x00,                           //����
        (uint32)&gul_UsrFuncVer,        //�ڴ�ƫ�Ƶ�ַ
    },
    {
        0x0004,                         //�ͻ����ֿ�汾(��ʼ������4505һ��)
        DB_PARA,                        //FLASH
        0,                              //����
        ZKVADDR,                        //�ڴ�ƫ�Ƶ�ַ
    },
    {
        0x0017,                         //�������ڡ��ز����������Դ������ز���������Լ����˯����ʱ
        0x00,                           //���ݿ�0
        0x00,                           //����
        0x27F,                          //�ڴ�ƫ�Ƶ�ַ
    },
    {
        0x0040,                         //�Զ����¼�����
        0x00,                           //���ݿ�0
        0x00,                           //����
        0x41D,                          //�ڴ�ƫ�Ƶ�ַ
    },

    
};

/*********************************************************************************************************
** ��������: uint8 CDATA_DirecAddrProc(uint16 id, uint16 tstp, uint8* buffer, const CDATA* cd, 
uint8 type)** ��������: Flash��ֱַ�Ӳ���
** �䡡��:  id ���ݱ�ʶ
**          tstp bit0 - bit5 ������� bit6-bit15 ͬ������
**          buffer ��ȡ���ݵĻ���
**          cd �ں˱��е�һ������
**          oc ��д���� true �� false д
** �䡡��:  0 �����ɹ�, ����������ʧ��
**
** ȫ�ֱ���: ��
** ����ģ��: ��
**
** ������: yzy
** �ա���: 2007��11��7��
**-------------------------------------------------------------------------------------------------------
** �޸���:
** �ա���:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
uint8 CDATA_DirecAddrProc(uint16 id, uint16 tstp, uint8* buffer, const CDATA* cd, bool oc)
{
    uint16 index;                       //��ַ����
    const CDATAAddr* ca;                //�����Ϣ
    TDataBlock db;                       //���ݿ�
    uint32 addr;                        //ƫ�Ƶ�ַ
    uint16 len;                         //����

    index = MidWaySearch(               //�������ݱ�ʶ
        (uint8*)gss_CDATAAddrIn,
        sizeof(gss_CDATAAddrIn) / sizeof(CDATAAddr),
        sizeof(CDATAAddr),
        cd->id
        );
    if(index == 0xFFFF)
    {
        _IF_TRUE_RETURN_Z(gui_CoreAddrExtTblSize == 0, CDATA_ERR_DI);

        index = MidWaySearch(           //�������ݱ�ʶ
            (uint8*)gss_CDATAAddrExt,
            gui_CoreAddrExtTblSize,
            sizeof(CDATAAddr),
            cd->id
            );
        _IF_TRUE_RETURN_Z(index == 0xFFFF, CDATA_ERR_DI);
        ca = gss_CDATAAddrExt + index;
    }
    else
    {
        ca = gss_CDATAAddrIn + index;
    }

    addr = ca->addr;                    //���ƫ�Ƶ�ַ
    if(ca->atr & CORE_ADR_ATRT)         //������Ͳ��������
    {
                                        //���ƫ�Ƴ��Բ������
        addr += (tstp & 0x3F) * (ca->atr & 0x7F) * 64;
    }
    addr += (id - cd->id) * cd->len;    //��ǰ���ݱ�ʶ��λ��

    len = cd->len * ((tstp >> 6) + 1);  //��ȡ�����ݵĳ���

    if(oc)                              //��
    {
        if(ca->type <= 0xF0)
        {
            if(SYS_FILE_DB_Open(ca->type, &db, TDB_MODE_RW) != SYS_ERR_OK)
            {
                return CDATA_ERR_BT;    //���ط����ײ����
            }
            if(SYS_ERR_OK != SYS_FILE_DB_ReadFrom(&db, buffer, len, addr))
            {
                SYS_FILE_DB_Close(&db);
                return CDATA_ERR_BT;
            }
            SYS_FILE_DB_Close(&db);
        }
        else if(ca->type == 0xF1)
        {
//	            ES_FRAM_Open();
//	            if(ES_FRAM_Read(buffer, len, addr) != SYS_ERR_OK)
//	            {
//	                ES_FRAM_Close();
//	                return CDATA_ERR_BT;        //���ط����ײ����
//	            }
//	            ES_FRAM_Close();
        }

        else if(ca->type == 0xF2)
        {
            MoveBuffer(                     //ת����������
                (uint8*)addr + 0x7FE0000,   //��ַ
                buffer,                     //����
                len                         //����
                );
        }
        else if(ca->type == 0xF3)
        {
            MoveBuffer(                     //ת����������
                (uint8*)addr,               //��ַ
                buffer,                     //����
                len                         //����
                );
        }

    }
    else                                    //д
    {
        if(ca->type <= 0xF0)
        {
                                            //�����ݿ�
            if(SYS_FILE_DB_Open(ca->type, &db, TDB_MODE_RW) != SYS_ERR_OK)
            {
                return CDATA_ERR_BT;        //���ط����ײ����
            }
    
            if(SYS_ERR_OK != SYS_FILE_DB_WriteFrom(&db, buffer, len, addr))
            {
                SYS_FILE_DB_Close(&db);
                return CDATA_ERR_BT;        //���ط����ײ����
            }
            SYS_FILE_DB_Close(&db);
        }
        else if(ca->type == 0xF1)           //ֱ�Ӷ�ȡ�����е�����
        {
//	            ES_FRAM_Open();
//	            if(ES_FRAM_Write(buffer, len, addr) != SYS_ERR_OK)
//	            {
//	                ES_FRAM_Close();
//	                return CDATA_ERR_BT;        //���ط����ײ����
//	            }
//	            ES_FRAM_Close();
        }
        else if(ca->type == 0xF2)
        {
            MoveBuffer(          //ת����������
                buffer,                     //����
                (uint8*)addr + 0x7FE0000,   //��ַ
                len                         //����
                );
        }
        else if(ca->type == 0xF3)
        {
            MoveBuffer(          //ת����������
                buffer,                     //����
                (uint8*)addr,               //��ַ
                len                         //����
                );
        }
    }
    SYS_OK();
}


/******************************************************************************
**common�������ݱ�
******************************************************************************/
const CDATA gss_CDATAIn[] =
{
    {
        0x0000,                         //����汾��
        0x0000,                         //ͬ����
        CORE_TATR_TM,                   //ֻ���ն˲���
        4,                              //4���ֽ�
        CDATA_DirecAddrProc,            //ֱ�ӵ�ַ����ʽ
    },
    {
        0x0004,                         //�ͻ�������汾��
        0x0000,                         //ͬ����
        CORE_TATR_TM,                   //ֻ���ն˲���
        4,                              //4���ֽ�
        CDATA_DirecAddrProc,            //��������
    },
    {
        0x0017,                         //�Զ������Ӳ���
        0x0000,                         //û��ͬ����
        CORE_TATR_TM,                   //ֻ���ն˲���
        10,                             //�������ڡ��ز����������Դ������ز���������Լ����˯����ʱ
        CDATA_DirecAddrProc,            //ֱ�ӵ�ַ����ʽ
    },
    {
        0x0040,                         //�Զ����¼�����
        0x0000,                         //û��ͬ����
        CORE_TATR_TM,                   //ֻ���ն˲���
        4,                              //��ʽ�֡��ź�������ѹ������
        CDATA_DirecAddrProc,            //ֱ�ӵ�ַ����ʽ
    },
    {
        0x0128,                         //ʱ��
        0x0000,                         //0ͬ����
        CORE_TATR_TM,                   //�ն�����
        6,                              //6�ֽ�����
        CDATA_FunctionR,                //��������
    },
    //------------------------------------------------------------

    
    
    
    
    
    
    
    
    

};

/*********************************************************************************************************
** ��������: uint16 TSTP_GetType(uint8 tstno)
** ��������: ��ȡ����������
** �䡡��:  tstno: �������
** �䡡��:  ��������������
**
** ȫ�ֱ���: ��
** ����ģ��: ��
**
** ������: yzy
** �ա���: 2007��11��7��
**-------------------------------------------------------------------------------------------------------
** �޸���:
** �ա���:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
uint16 TSTP_GetType(uint8 tstno)
{
    if(tstno == 0)                          //������0Ϊ�ն�����
    {
        return CORE_TATR_TM;
    }
//	    switch(gss_CDATATst[tstno].tf)
//	    {
//	        case 0x01:                          //485��
//	            return CORE_TATR_MT;
//	        case 0x02:                          //ģ����
//	            return CORE_TATR_SM;
//	        case 0x03:                          //������
//	            return CORE_TATR_PS;
//	        case 0x04:                          //����ֵ
//	            return CORE_TATR_CA;
//	        case 0x05:                          //��������
//	            return CORE_TATR_AC;
//	        case 0x06:                          //©�籣����
//	            return CORE_TATR_LD;
//	        default:                            //����
//	            return 0x0000;
//	    }
    return 0;
}

/*********************************************************************************************************
** ��������: const CDATAToBG* CDATA_SearchBgDI(uint16 DI)
** ��������: �����ں����ݱ�ʶ��Ӧ�Ĳ����ʶ
** �䡡��: DI: �ں����ݱ�ʶ
** �䡡��: ��Ӧ���ݱ�ʶ�Ľṹ����Ϣָ��
**
** ȫ�ֱ���: ��
** ����ģ��: ��
**
** ������: yzy
** �ա���: 2007��10��15��
**-------------------------------------------------------------------------------------------------------
** �޸���:
** �ա���:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
const CDATAToBG* CDATA_SearchBgDI(uint16 DI)
{
    uint16 index;
    const CDATA* cd;

    index = ADVMidWaySearch(            //�����ں˱���Ӧ�����ݱ�ʶ
        (uint8*)gss_CDATAIn,
        sizeof(gss_CDATAIn) / sizeof(CDATA),
        sizeof(CDATA),
        DI);
    if(index == 0xFFFF)
    {
        _IF_TRUE_RETURN_Z(gui_CoreTblExtSize == 0, __NULL)
        index = ADVMidWaySearch(            //�����ں˱���Ӧ�����ݱ�ʶ
            (uint8*)gss_CDATAExt,
            gui_CoreTblExtSize,
            sizeof(CDATA),
            DI);
                                           //�����������ؿ�ָ��
        _IF_TRUE_RETURN_Z(index == 0xFFFF, __NULL);
        cd = gss_CDATAExt + index;
    }
    else
    {
        cd = gss_CDATAIn + index;
    }
     
    index = MidWaySearch(               //�������ݱ�ʶ
        (uint8*)gs_CDATAToBG,
        gui_CoreBgTblSize,
        sizeof(CDATAToBG),
        cd->id
        );
                                        //�Ҳ����ñ�ʶ���ؿ�ָ��
    _IF_TRUE_RETURN_Z(index == 0xFFFF, __NULL);

    return gs_CDATAToBG + index;        //���ض�Ӧ��ָ��

}

/*********************************************************************************************************
** ��������: uint8 CDATA_SelfSelectR(uint16 id, uint16 tstp, uint8* buffer, const CDATA* cd)
** ��������: ͨ��ͨѶ��ȡ����
** �䡡��:  id ���ݱ�ʶ
            tstp bit0 - bit5 ������� bit6-bit15 ͬ������
            buffer ��ȡ���ݵĻ���
            cd �ں˱��е�һ������
** �䡡��: ��
**
** ȫ�ֱ���: ��
** ����ģ��: ��
**
** ������: yzy
** �ա���: 2007��9��6��
**-------------------------------------------------------------------------------------------------------
** �޸���:
** �ա���:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
uint8 CDATA_SelfSelectR(uint16 id, uint16 tstp, uint8* buffer, const CDATA* cd, bool oc)
{
//	    uint16 index;                       //��ַ����
//	    uint32 di;                          //�����ʶ
//	    const CDATAToBG* ca;                //�����Ϣ
//	
//	    index = MidWaySearch(               //�������ݱ�ʶ
//	        (uint8*)gs_CDATAToBG,
//	        gui_CoreBgTblSize,
//	        sizeof(CDATAToBG),
//	        cd->id
//	        );
//	
//	    if(index == 0xFFFF)                 //�����������
//	    {
//	        return CDATA_ERR_DI;            //���ش������ݱ�ʶ
//	    }
//	
//	    ca = gs_CDATAToBG + index;          //��ȡ���ҵ�����������
//	    di = ca->bgid + (id - ca->cid);
//	
//	    if(ca->proc == __NULL)              //ֱ�Ӷ�ȡ���ݵĴ���
//	    {
//	        if((tstp & 0x3F) == 0)          //������0,�ն�����
//	        {
//	            return CDATA_ERR_BT;
//	        }
//	                                        //����������,ͨ�������˿ڶ�ȡ
//	        if(Netp_Read(buffer, di, cd->len, tstp) != NETP_ERR_OK)
//	        {
//	            return CDATA_ERR_BT;;
//	        }
//	        
//	    }
    SYS_OK();
}

/*********************************************************************************************************
** ��������: uint8 CDATA_Read(uint16 id, uint16 tstp, uint8* buffer, uint16* len)
** ��������: ͨ���ں˱��ȡ����
** �䡡��:  id ���ݱ�ʶ
            tstp bit0 - bit5 ������� bit6-bit15 ͬ������
            buffer ��ȡ���ݵĻ���
            len ����ʱ��ʾ�������󳤶ȣ����ʱ��ʾ��ȡ���ݵĳ���
** �䡡��: ��
**
** ȫ�ֱ���: ��
** ����ģ��: ��
**
** ������: yzy
** �ա���: 2007��9��6��
**-------------------------------------------------------------------------------------------------------
** �޸���:
** �ա���:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
uint8 CDATA_Read(uint16 id, uint16 tstp, uint8* buffer, uint16* len)
{
    uint16 tatr;                        //����������
    uint16 index;                       //��������
    const CDATA* cd;

    tatr = TSTP_GetType(tstp & 0x3F);   //��ȡ�����������
    if(tatr == 0)                       //���Ϊ0��֤��û�����������
    {
        return CDATA_ERR_TT;            //���ز����㲻��ȷ
    }

    index = ADVMidWaySearch(            //�������ݱ�ʶ�����ü�ǿ����ֵ����������
        (uint8*)gss_CDATAIn,
        sizeof(gss_CDATAIn) / sizeof(CDATA),
        sizeof(CDATA),
        id
        );
    if(index == 0xFFFF)
    {
        _IF_TRUE_RETURN_Z(gui_CoreTblExtSize == 0, CDATA_ERR_DI);
        index = ADVMidWaySearch(            //�������ݱ�ʶ�����ü�ǿ����ֵ����������
            (uint8*)gss_CDATAExt,
            gui_CoreTblExtSize,
            sizeof(CDATA),
            id
            );
        _IF_TRUE_RETURN_Z(index == 0xFFFF, CDATA_ERR_DI);

        cd = gss_CDATAExt + index;
    }
    else
    {
        cd = gss_CDATAIn + index;
    }
    
    if(!(cd->tatr & tatr)) //��ǰ�����㲻֧��������ݱ�ʶ
    {
        return CDATA_ERR_TT;            //���ز����㲻��ȷ
    }
                                        //�жϳ����Ƿ���ȷ
    if(cd->len  * ((tstp >> 6) + 1) > *len)
    {
        return CDATA_ERR_LEN;           //���ػ��泤�Ȳ���
    }

    *len = cd->len  * ((tstp >> 6) + 1);
                                        //�ж����ݱ�ʶ��ͬ�������Ƿ���ȷ
    if((cd->nslm & 0xFFF) + cd->id < id + (tstp >> 6))
    {
        return CDATA_ERR_DI;            //���ش������ݱ�ʶ
    }
    return cd->proc(id, tstp, buffer, cd, true);
}


/*********************************************************************************************************
** ��������: uint8 CDATA_Write(uint16 id, uint16 tstp, uint8* buffer, uint16 len)
** ��������: ͨ���ں˱�д������
** �䡡��:  id ���ݱ�ʶ
            tstp bit0 - bit5 ������� bit6-bit15 ͬ������
            buffer д�����ݵĻ���
            len ��ʾҪд������ݵĳ���
** �䡡��: ��
**
** ȫ�ֱ���: ��
** ����ģ��: ��
**
** ������: yzy
** �ա���: 2007��9��6��
**-------------------------------------------------------------------------------------------------------
** �޸���:
** �ա���:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
uint8 CDATA_Write(uint16 id, uint16 tstp, uint8* buffer, uint16 len)
{
    uint16 tatr;                        //����������
    uint16 index;                       //��������
    const CDATA* cd;

    tatr = TSTP_GetType(tstp & 0x3F);   //��ȡ�����������
    tatr = tatr;                        //��ֹ�������澯
//    if(tatr == 0)                       //���Ϊ0��֤��û�����������
//    {
//        return CDATA_ERR_TT;            //���ز����㲻��ȷ
//    }

    index = ADVMidWaySearch(            //�������ݱ�ʶ�����ü�ǿ����ֵ����������
        (uint8*)gss_CDATAIn,
        sizeof(gss_CDATAIn) / sizeof(CDATA),
        sizeof(CDATA),
        id
        );
    if(index == 0xFFFF)
    {
        _IF_TRUE_RETURN_Z(gui_CoreTblExtSize == 0, CDATA_ERR_DI);
        index = ADVMidWaySearch(            //�������ݱ�ʶ�����ü�ǿ����ֵ����������
            (uint8*)gss_CDATAExt,
            gui_CoreTblExtSize,
            sizeof(CDATA),
            id
            );
        _IF_TRUE_RETURN_Z(index == 0xFFFF, CDATA_ERR_DI);

        cd = gss_CDATAExt + index;
    }
    else
    {
        cd = gss_CDATAIn + index;
    }

//    if(!(gss_CDATA[index].tatr & tatr)) //��ǰ�����㲻֧��������ݱ�ʶ
//    {
//        return CDATA_ERR_TT;            //���ز����㲻��ȷ
//    }
                                          //�жϳ����Ƿ���ȷ
    if(cd->len  * ((tstp >> 6) + 1) != len)
    {
        return CDATA_ERR_LEN;           //���ػ��泤�Ȳ���ȷ
    }
                                        //�ж����ݱ�ʶ��ͬ�������Ƿ���ȷ
    if((cd->nslm & 0xFFF) + cd->id < id + (tstp >> 6))
    {
        return CDATA_ERR_DI;            //���ش������ݱ�ʶ
    }
    return cd->proc(id, tstp, buffer, cd, false);
    
}


/*********************************************************************************************************
** ��������: const CDATA* CDATA_GetInfo(uint16 id)
** ��������: �����ں����ݱ�ʶ����ȡ�ں˱��һ������
** �䡡��:  id ���ݱ�ʶ
** �䡡��: ��
**
** ȫ�ֱ���: ��
** ����ģ��: ��
**
** ������: yzy
** �ա���: 2007��9��14��
**-------------------------------------------------------------------------------------------------------
** �޸���:
** �ա���:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
const CDATA* CDATA_GetInfo(uint16 id)
{
    uint16 index;

    index = ADVMidWaySearch(            //�������ݱ�ʶ�����ü�ǿ����ֵ����������
        (uint8*)gss_CDATAIn,
        sizeof(gss_CDATAIn) / sizeof(CDATA),
        sizeof(CDATA),
        id
        );
    if(index == 0xFFFF)
    {
        _IF_TRUE_RETURN_Z(gui_CoreTblExtSize == 0, __NULL);
        index = ADVMidWaySearch(            //�������ݱ�ʶ�����ü�ǿ����ֵ����������
            (uint8*)gss_CDATAExt,
            gui_CoreTblExtSize,
            sizeof(CDATA),
            id
            );
        _IF_TRUE_RETURN_Z(index == 0xFFFF, __NULL);

        return gss_CDATAExt + index;
    }
    return gss_CDATAIn + index;
}





