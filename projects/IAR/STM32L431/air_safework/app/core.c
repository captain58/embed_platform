/****************************************Copyright (c)**************************************************
**                                  ____科技有限公司
**                                          开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: core.c
**创   建   人: yzy
**最后修改日期: 2019年6月18日
**描        述: 配变程序核心数据处理公共程序模块
**注        意:
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2019年6月18日
** 描　述: 原始版本
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
**核心数据表
*************************************************************************/
const CDATA gss_CDATAExt[] =
{
    /************************************************************************/
    //645-2007.AFN0C实时抄读标识 
    /************************************************************************/
    {
        0x9EA0,         //A相+W-W+Q-Q(9EA0-9EA3)
        0x8003,
        CORE_TATR_TM | CORE_TATR_MT,
        4,
        CDATA_SelfSelectR,
    },
    {
        0x9EA4,         //B相+W-W+Q-Q(9EA4-9EA7)
        0x8003,
        CORE_TATR_TM | CORE_TATR_MT,
        4,
        CDATA_SelfSelectR,
    },
    {
        0x9EA8,         //C相+W-W+Q-Q(9EA8-9EAB)
        0x8003,
        CORE_TATR_TM | CORE_TATR_MT,
        4,
        CDATA_SelfSelectR,
    },
    {
        0x9EAC,         //上一结算日A相+W-W+Q-Q(9EAC-9EAF)
        0x8003,
        CORE_TATR_TM | CORE_TATR_MT,
        4,
        CDATA_SelfSelectR,
    },
    {
        0x9EB0,         //上一结算日B相+W-W+Q-Q(9EB0-9EB3)
        0x8003,
        CORE_TATR_TM | CORE_TATR_MT,
        4,
        CDATA_SelfSelectR,
    },
    {
        0x9EB4,         //上一结算日C相+W-W+Q-Q(9EB4-9EB7)
        0x8003,
        CORE_TATR_TM | CORE_TATR_MT,
        4,
        CDATA_SelfSelectR,
    },
    //---------------
    {
        0x9EC0,         //上一结算日+W-W+Q-QQ1Q2Q3Q4(9EC0-9EC7)
        0x8007,
        CORE_TATR_TM | CORE_TATR_MT,
        20,
        CDATA_SelfSelectR,
    },
    //---------------
    {
        0x9ECC,         //+W-W+Q-Q需量及时间(9ECC-9ECF)
        0x8003,
        CORE_TATR_TM | CORE_TATR_MT,
        40,
        CDATA_SelfSelectR,
    },
    {
        0x9ED0,         //上一结算日+W-W+Q-Q需量及时间(9ED0-9ED3)
        0x8003,
        CORE_TATR_TM | CORE_TATR_MT,
        40,
        CDATA_SelfSelectR,
    },

    /************************************************************************/
    //645-1997.AFN0C实时抄读标识 
    /************************************************************************/
    {
        0x9F00,         //上月+W
        0x0000,
        CORE_TATR_TM | CORE_TATR_MT,
        20,
        CDATA_SelfSelectR,
    },
    {
        0x9F01,         //上月+Q
        0x0000,
        CORE_TATR_TM | CORE_TATR_MT,
        20,
        CDATA_SelfSelectR,
    },
    {
        0x9F02,         //上月-W
        0x0000,
        CORE_TATR_TM | CORE_TATR_MT,
        20,
        CDATA_SelfSelectR,
    },
    {
        0x9F03,         //上月-Q
        0x0000,
        CORE_TATR_TM | CORE_TATR_MT,
        20,
        CDATA_SelfSelectR,
    },
    {
        0x9F04,         //上月Q1
        0x0000,
        CORE_TATR_TM | CORE_TATR_MT,
        20,
        CDATA_SelfSelectR,
    },
    {
        0x9F05,         //上月Q2
        0x0000,
        CORE_TATR_TM | CORE_TATR_MT,
        20,
        CDATA_SelfSelectR,
    },
    {
        0x9F06,         //上月Q3
        0x0000,
        CORE_TATR_TM | CORE_TATR_MT,
        20,
        CDATA_SelfSelectR,
    },
    {
        0x9F07,         //上月Q4
        0x0000,
        CORE_TATR_TM | CORE_TATR_MT,
        20,
        CDATA_SelfSelectR,
    },
    {
        0x9F08,         //正向有功需量 
        0x0000,
        CORE_TATR_TM | CORE_TATR_MT,
        15,
        CDATA_SelfSelectR,
    },
    {
        0x9F09,         //正向有功需量时间
        0x0000,
        CORE_TATR_TM | CORE_TATR_MT,
        20,
        CDATA_SelfSelectR,
    },
    {
        0x9F0A,         //正向无功需量
        0x0000,
        CORE_TATR_TM | CORE_TATR_MT,
        15,
        CDATA_SelfSelectR,
    },
    {
        0x9F0B,         //正向无功需量时间
        0x0000,
        CORE_TATR_TM | CORE_TATR_MT,
        20,
        CDATA_SelfSelectR,
    },
    {
        0x9F0C,         //反向有功需量
        0x0000,
        CORE_TATR_TM | CORE_TATR_MT,
        15,
        CDATA_SelfSelectR,
    },
    {
        0x9F0D,         //反向有功需量时间
        0x0000,
        CORE_TATR_TM | CORE_TATR_MT,
        20,
        CDATA_SelfSelectR,
    },
    {
        0x9F0E,         //反向无功需量
        0x0000,
        CORE_TATR_TM | CORE_TATR_MT,
        15,
        CDATA_SelfSelectR,
    },
    {
        0x9F0F,         //反向无功需量时间
        0x0000,
        CORE_TATR_TM | CORE_TATR_MT,
        20,
        CDATA_SelfSelectR,
    },
    {
        0x9F10,         //上月正向有功需量
        0x0000,
        CORE_TATR_TM | CORE_TATR_MT,
        15,
        CDATA_SelfSelectR,
    },
    {
        0x9F11,         //上月正向有功需量时间
        0x0000,
        CORE_TATR_TM | CORE_TATR_MT,
        20,
        CDATA_SelfSelectR,
    },
    {
        0x9F12,         //上月正向无功需量
        0x0000,
        CORE_TATR_TM | CORE_TATR_MT,
        15,
        CDATA_SelfSelectR,
    },
    {
        0x9F13,         //上月正向无功需量时间
        0x0000,
        CORE_TATR_TM | CORE_TATR_MT,
        20,
        CDATA_SelfSelectR,
    },
    {
        0x9F14,         //上月反向有功需量
        0x0000,
        CORE_TATR_TM | CORE_TATR_MT,
        15,
        CDATA_SelfSelectR,
    },
    {
        0x9F15,         //上月反向有功需量时间
        0x0000,
        CORE_TATR_TM | CORE_TATR_MT,
        20,
        CDATA_SelfSelectR,
    },
    {
        0x9F16,         //上月反向无功需量
        0x0000,
        CORE_TATR_TM | CORE_TATR_MT,
        15,
        CDATA_SelfSelectR,
    },
    {
        0x9F17,         //上月反向无功需量时间
        0x0000,
        CORE_TATR_TM | CORE_TATR_MT,
        20,
        CDATA_SelfSelectR,
    },

};


//应用层的内核数据表大小
const uint16 gui_CoreTblExtSize = sizeof(gss_CDATAExt)/sizeof(CDATA);













//-------------------------------------------------------------------






/***************************************************************************************************
** 函数名称: uint8 CDATA_GetTime(uint16 id, uint16 tstp, uint8* buffer, const CDATA* cd)
** 功能描述: 获取当前时间
** 输　入:  id: 内核标识
**          tstp: 测量点号
**          buffer: 缓存
**          cd:  内核结构体
** 输　出:
**
** 全局变量: 无
** 调用模块: 无
**
** 作　者: yzy
** 日　期: 2007年11月16日
**--------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
**--------------------------------------------------------------------------------------------------
***************************************************************************************************/
uint8 CDATA_GetTime(uint16 id, uint16 tstp, uint8* buffer, const CDATA* cd)
{
    gs_RealTime.ReadBCDDateTime((TIME*)buffer);
    SYS_OK();
}


/******************************************************************************
**common核心函数处理表
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
** 函数名称: uint8 CDATA_FunctionR(uint16 id, uint16 tstp, uint8* buffer, const CDATA* cd)
** 功能描述: 通过函数来操作数据
** 输　入:  id 数据标识
            tstp bit0 - bit5 测量点号 bit6-bit15 同类项数
            buffer 读取数据的缓存
            cd 内核表中的一项内容
** 输　出: 无
**
** 全局变量: 无
** 调用模块: 无
**
** 作　者: yzy
** 日　期: 2007年9月15日
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
uint8 CDATA_FunctionR(uint16 id, uint16 tstp, uint8* buffer, const CDATA* cd, bool oc)
{
    uint16 index;                       //地址索引
    const CDATAFunc* cf;                //表格信息

    index = MidWaySearch(               //搜索数据标识
        (uint8*)gss_CDATAFuncIn,
        sizeof(gss_CDATAFuncIn) / sizeof(CDATAFunc),
        sizeof(CDATAFunc),
        cd->id
        );

    if(index == 0xFFFF)                 //看看搜索结果
    {
        _IF_TRUE_RETURN_Z(gui_CoreFuncExtTblSize ==0, CDATA_ERR_DI);
        index = MidWaySearch(           //搜索数据标识
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
        cf = gss_CDATAFuncIn + index;   //获取查找到的数据内容
    }
                                        //通过函数调用返回数据
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
//天信13累计量
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

uint8_t ADATA_HEX_LSB_Fload(uint16 id, uint16 tstp, uint8_t* buffer, uint8_t* data)
{
    
    double sum = 0;
    if(id == NETP_D_LSB_4B_FLOAT)
    {
        Float32 fvalue;
        fvalue.byte[0] = buffer[0];
        fvalue.byte[1] = buffer[1];
        fvalue.byte[2] = buffer[2];
        fvalue.byte[3] = buffer[3];

        int32_t integer = (int32_t)fvalue.fword;
        int64_t decimal = 0;
        if(fvalue.fword > 0)
        {
            decimal = (int64_t)(fvalue.fword * 10000);
        }
        else
        {
            decimal = -(int64_t)(fvalue.fword * 10000);
        }
        decimal %= 10000;
        memcpy(data, &integer,4);
        
        memcpy(data+4, &decimal,2);

        
        SYS_OK();

    }
    else if(id == NETP_D_LSB_8B_FLOAT4B_H7_FLOAT4B_LOW)
    {
        Float32 fvalue, fvalue2;
        fvalue.byte[0] = buffer[0];
        fvalue.byte[1] = buffer[1];
        fvalue.byte[2] = buffer[2];
        fvalue.byte[3] = buffer[3];
        fvalue2.byte[0] = buffer[4];
        fvalue2.byte[1] = buffer[5];
        fvalue2.byte[2] = buffer[6];
        fvalue2.byte[3] = buffer[7];
        double value = fvalue.fword;
        value = value * 10000000 + fvalue2.fword;
        
        int32_t integer = (int32_t)value;
        int64_t decimal = 0;
        if(value > 0)
        {
            decimal = (int64_t)(value * 10000);
        }
        else
        {
            decimal = -(int64_t)(value * 10000);
        }
        decimal %= 10000;
        memcpy(data, &integer,4);
        
        memcpy(data+4, &decimal,2);

        
        SYS_OK();

    }
    SYS_ERR();
}

uint8_t ADATA_HEX_MSB_Fload(uint16 id, uint16 tstp, uint8_t* buffer, uint8_t* data)
{
    if(id == NETP_D_MSB_8B_LONG4B_H7_LONG4B_LOW)
    {
        Word32 fvalue, fvalue2;
        fvalue.byte[0] = buffer[3];
        fvalue.byte[1] = buffer[2];
        fvalue.byte[2] = buffer[1];
        fvalue.byte[3] = buffer[0];
        fvalue2.byte[0] = buffer[7];
        fvalue2.byte[1] = buffer[6];
        fvalue2.byte[2] = buffer[5];
        fvalue2.byte[3] = buffer[4];
        int64_t value = fvalue.lword;
        value = value * 10000000 + fvalue2.lword;
        
        int32_t integer = (int32_t)value;
//	        int64_t decimal = 0;
//	        if(value > 0)
//	        {
//	            decimal = (int64_t)(value * 10000);
//	        }
//	        else
//	        {
//	            decimal = -(int64_t)(value * 10000);
//	        }
//	        decimal %= 10000;
        memcpy(data, &integer,4);
        
//	        memcpy(data+4, &decimal,2);
        data[4] = 0;
        data[5] = 0;
        

        
        SYS_OK();

    }
    SYS_ERR();

}

uint8_t ADATA_BCD_MSB_Fload(uint16 id, uint16 tstp, uint8_t* buffer, uint8_t* data)
{
    
    double sum = 0;
    if(id == NETP_D_MSB_BCD6B_DECI3)
    {
        Word64 wvalue;
        uint64_t value = 0;
        wvalue.byte[0] = buffer[5];
        wvalue.byte[1] = buffer[4];
        wvalue.byte[2] = buffer[3];
        wvalue.byte[3] = buffer[2];
        wvalue.byte[4] = buffer[1];
        wvalue.byte[5] = buffer[0];
        wvalue.byte[6] = 0;
        wvalue.byte[7] = 0;
        
        value = LongLongBcdToHex(wvalue.qword);

        uint32_t integer = value/1000;
        uint64_t decimal =  (value * 10)%10000;

        
        memcpy(data, &integer,4);
        
        memcpy(data+4, &decimal,2);
        
        SYS_OK();
    }
    SYS_ERR();
}
uint8_t ADATA_Fload(uint16 id, uint16 tstp, uint8_t* buffer, uint8_t* data)
{
    
    double sum = 0;
    if(id == NETP_D_BIGEND_TO_LITTLE_6B)
    {
        memcpy(data, buffer,4);
        InvBuffer(data, 4);
        memcpy(data+4, buffer+4,2);
        InvBuffer(data+4, 2);
        
        SYS_OK();
    }
    else if(id == NETP_D_BIG_TO_LIT_4B_DECI2)
    {
        int64_t hvalue = RByteToLong(buffer);
//	        memcpy(data, buffer,4);
//	        InvBuffer(data, 4);
//	        memcpy(data+4, buffer+4,2);
//	        InvBuffer(data+2, 2);
        
        int32_t integer = hvalue/100;
        int32_t decimal =  abs((hvalue * 100)%10000);
        
        memcpy(data, &integer,4);
        
        memcpy(data+4, &decimal,2);
        
        SYS_OK();
    }
    else if(id == NETP_D_BIG_TO_LIT_4B_DECI3)
    {
        int64_t hvalue = RByteToLong(buffer);
        
        int32_t integer = hvalue/100;
        int64_t decimal =  abs((hvalue * 10)%10000);
        
        memcpy(data, &integer,4);
        
        memcpy(data+4, &decimal,2);
        
        SYS_OK();
    }
    
    else if(id == NETP_D_BIG_TO_LIT_4B_LONG4B)
    {
        uint32_t hvalue = RByteToLong(buffer);
        memcpy(data, &hvalue,4);
        
        
        data[4] = 0;
        data[5] = 0;
        SYS_OK();
    }
    else if(id == NETP_D_BIG_TO_LIT_8B_LONG4B_DECI4B)
    {
        uint32_t hvalue = RByteToLong(buffer);
        memcpy(data, &hvalue,4);
        
        hvalue = RByteToLong(buffer+4);
        hvalue = (hvalue & 0xffff0000)>>16;
        memcpy(data+4, &hvalue,2);
        SYS_OK();
    }
    else if(id == NETP_D_BIG_TO_LIT_2B_DECI1)
    {
        int64_t hvalue = RByteToInt(buffer);

        int32_t integer = hvalue/10;
        int64_t decimal =  abs((hvalue * 1000)%10000);

        
//	        int32_t integer;
//	        if((hvalue & 0x8000) == 0)
//	            integer = hvalue/10;
//	        else
//	        {
//	            hvalue &= 0x7fff;
//	            integer = -(hvalue/10);
//	        }
//	        int32_t decimal =  (hvalue * 1000)%10000;
        memcpy(data, &integer,4);
        
        memcpy(data+4, &decimal,2);
        
        SYS_OK();
    }
    else if(id == NETP_D_BIG_TO_LIT_2B_DECI2)
    {
        uint64_t hvalue = RByteToInt(buffer);
        int32_t integer = hvalue/100;
        int64_t decimal =  abs((hvalue * 100)%10000);
//	        int32_t integer;
//	        if((hvalue & 0x8000) == 0)
//	            integer = hvalue/10;
//	        else
//	        {
//	            hvalue &= 0x7fff;
//	            integer = -(hvalue/10);
//	        }
//	        int32_t decimal =  (hvalue * 1000)%10000;
        memcpy(data, &integer,4);
        
        memcpy(data+4, &decimal,2);
        
        SYS_OK();
    }    
    else if(id == NETP_D_BIG_TO_LIT_4B_DECI1)
    {
        int64_t hvalue = RByteToLong(buffer);
        
        int32_t integer = hvalue/10;
        int32_t decimal =  abs((hvalue * 1000)%10000);
        memcpy(data, &integer,4);
        
        memcpy(data+4, &decimal,2);
        
        SYS_OK();
    }
    else if(id == NETP_D_BIG_TO_LIT_8B_INT6B_DECI2B_DIV)
    {
    
        uint64_t hvalue = buffer[0] * 0x10000000000 + buffer[1] * 0x100000000 + buffer[2] * 0x1000000 +
                          buffer[3] * 0x10000 + buffer[4] * 0x100 + buffer[5];
        memcpy(data, &hvalue, 4);

        int64_t dvalue = ((buffer[6] * 0x100 + buffer[7]) * 10000) / 0x10000;

        memcpy(data+4, &dvalue,2);
        
        SYS_OK();
    }
    else if(id == NETP_D_BIG_TO_LIT_4B_INT3B_DECI1B_DIV)
    {
    
        uint32_t hvalue = buffer[0] * 0x10000 + buffer[1] * 0x100 + buffer[2];
        memcpy(data, &hvalue, 4);

        uint64_t dvalue = (buffer[3] * 100) / 0x100;

        memcpy(data+4, &dvalue,2);
        
        SYS_OK();
    }
    else if(id == NETP_D_BIG_TO_LIT_4B_LONG_REV2)
    {
        Word32 wvalue;
        wvalue.byte[0] = buffer[1];
        wvalue.byte[1] = buffer[0];
        wvalue.byte[2] = buffer[3];
        wvalue.byte[3] = buffer[2];
        
        memcpy(data, &wvalue.lword, 4);
//	
//	        uint32_t dvalue = (buffer[3] * 100) / 0x100;
//	
//	        memcpy(data+4, &dvalue,2);
        data[4] = 0;
        data[5] = 0;
        SYS_OK();
    }
    else if(id == NETP_D_BIG_TO_LIT_4B_FLOAT_REV2)
    {
        Float32 fvalue;
        fvalue.byte[0] = buffer[1];
        fvalue.byte[1] = buffer[0];
        fvalue.byte[2] = buffer[3];
        fvalue.byte[3] = buffer[2];

        int32_t integer = (int32_t)fvalue.fword;
        int64_t decimal = 0;
        if(fvalue.fword > 0)
        {
            decimal = (int64_t)(fvalue.fword * 10000);
        }
        else
        {
            decimal = -(int64_t)(fvalue.fword * 10000);
        }
        decimal %= 10000;
        memcpy(data, &integer,4);
        
        memcpy(data+4, &decimal,2);

        
        SYS_OK();
    }
    else if(id == NETP_D_BIG_TO_LIT_4B_FLOAT)
    {
        Float32 fvalue;
        fvalue.byte[0] = buffer[3];
        fvalue.byte[1] = buffer[2];
        fvalue.byte[2] = buffer[1];
        fvalue.byte[3] = buffer[0];

        int32_t integer = (int32_t)fvalue.fword;
        int64_t decimal = 0;
        if(fvalue.fword > 0)
        {
            decimal = (int64_t)(fvalue.fword * 10000);
        }
        else
        {
            decimal = -(int64_t)(fvalue.fword * 10000);
        }
        decimal %= 10000;
        memcpy(data, &integer,4);
        
        memcpy(data+4, &decimal,2);

        
        SYS_OK();
    }    
    else if(id == NETP_D_BIG_TO_LIT_8B_FLOAT2_HIGH5_LOW3_DECI)
    {
        Float32 fvalueh,fvaluel;
        fvalueh.byte[0] = buffer[3];
        fvalueh.byte[1] = buffer[2];
        fvalueh.byte[2] = buffer[1];
        fvalueh.byte[3] = buffer[0];
        
        fvaluel.byte[0] = buffer[7];
        fvaluel.byte[1] = buffer[6];
        fvaluel.byte[2] = buffer[5];
        fvaluel.byte[3] = buffer[4];

        double dvalue = fvalueh.fword * 10000 + fvaluel.fword;

        int32_t integer = (int32_t)dvalue;
        int64_t decimal = 0;
        if(dvalue > 0)
        {
            decimal = (int64_t)(dvalue * 10000);
        }
        else
        {
            decimal = -(int64_t)(dvalue * 10000);
        }
        decimal %= 10000;
        memcpy(data, &integer,4);
        
        memcpy(data+4, &decimal,2);

        
        SYS_OK();
    }      
    else if(id == NETP_D_BIG_TO_LIT_8B_DOUBLE)
    {
        Double64 dvalue;
        dvalue.byte[0] = buffer[7];
        dvalue.byte[1] = buffer[6];
        dvalue.byte[2] = buffer[5];
        dvalue.byte[3] = buffer[4];
        dvalue.byte[4] = buffer[3];
        dvalue.byte[5] = buffer[2];
        dvalue.byte[6] = buffer[1];
        dvalue.byte[7] = buffer[0];

        int32_t integer = (int32_t)dvalue.dword;
        int64_t decimal = 0;
        if(dvalue.dword > 0)
        {
            decimal = (int64_t)(dvalue.dword * 10000);
        }
        else
        {
            decimal = -(int64_t)(dvalue.dword * 10000);
        }
        decimal %= 10000;
        memcpy(data, &integer,4);
        
        memcpy(data+4, &decimal,2);

        
        SYS_OK();
    }    
    else if(id == NETP_D_BIG_TO_LIT_6B_INT4B_REV2_DECI2B_INVLID1B)
    {
        Word32 wvalue;
        wvalue.byte[0] = buffer[1];
        wvalue.byte[1] = buffer[0];
        wvalue.byte[2] = buffer[3];
        wvalue.byte[3] = buffer[2];
        
        memcpy(data, &wvalue.lword, 4);
        
        uint64_t hvalue = (RByteToInt(buffer+4) * 100) % 10000;
//          uint32_t dvalue = (buffer[3] * 100) / 0x100;
//  
        memcpy(data+4, &hvalue,2);

        SYS_OK();
    }
    else if(id == NETP_D_ASCII10B_TO_HEX5B)
    {
        uint8_t tmp[5];
        memset(tmp,0,5);
        HexStringToNum(buffer,tmp,10);
        
        Word32 wvalue;
        wvalue.byte[0] = tmp[4];
        wvalue.byte[1] = tmp[3];
        wvalue.byte[2] = tmp[2];
        wvalue.byte[3] = tmp[1];

        wvalue.lword = LongBcdToHex(wvalue.lword);
        
        memcpy(data, &wvalue.lword, 4);
        
        data[4] = 0;
        data[5] = 0;
        
        SYS_OK();
    }
    else if(id == NETP_D_ASCII8B_TO_HEX4B_DECI2)
    {
        uint8_t tmp[5];
        memset(tmp,0,5);
        HexStringToNum(buffer,tmp,8);
        
        Word32 wvalue;
        wvalue.byte[0] = tmp[3];
        wvalue.byte[1] = tmp[2];
        wvalue.byte[2] = tmp[1];
        wvalue.byte[3] = tmp[0];
        wvalue.lword = LongBcdToHex(wvalue.lword);
//	        memcpy(data, &wvalue.lword, 4);
        
        uint32_t integer = wvalue.lword/100;
        uint64_t decimal =  (int64_t)(wvalue.lword * 100)%10000;
        memcpy(data, &integer,4);
        
        memcpy(data+4, &decimal,2);
        
        SYS_OK();
    }     
    
    else if(id == NETP_D_ASCII6B_TO_HEX3B_DECI1)
    {
        uint8_t tmp[5];
        memset(tmp,0,5);
        HexStringToNum(buffer,tmp,6);
        
        Word32 wvalue;
        wvalue.lword = 0;
        wvalue.byte[0] = tmp[2];
        wvalue.byte[1] = tmp[1];
        wvalue.byte[2] = tmp[0];
        wvalue.lword = LongBcdToHex(wvalue.lword);
//          memcpy(data, &wvalue.lword, 4);
        
        uint32_t integer = wvalue.lword/10;
        uint64_t decimal =  (int64_t)(wvalue.lword * 1000)%10000;
        memcpy(data, &integer,4);
        
        memcpy(data+4, &decimal,2);
        
        SYS_OK();
    }     
    else if(id == NETP_D_ASCII6B_TO_HEX3B_DECI2)
    {
        uint8_t tmp[5];
        memset(tmp,0,5);
        HexStringToNum(buffer,tmp,6);
        
        Word32 wvalue;
        wvalue.lword = 0;
        wvalue.byte[0] = tmp[2];
        wvalue.byte[1] = tmp[1];
        wvalue.byte[2] = tmp[0];
        wvalue.lword = LongBcdToHex(wvalue.lword);
//          memcpy(data, &wvalue.lword, 4);
        
        uint32_t integer = wvalue.lword/100;
        int64_t decimal =  (int64_t)(wvalue.lword * 100)%10000;
        memcpy(data, &integer,4);
        
        memcpy(data+4, &decimal,2);
        
        SYS_OK();
    }         
    else if(id == NETP_D_ASCII6B_TO_HEX3B)
    {
        uint8_t tmp[5];
        memset(tmp,0,5);
        HexStringToNum(buffer,tmp,6);
        
        Word32 wvalue;
        wvalue.lword = 0;
        wvalue.byte[0] = tmp[2];
        wvalue.byte[1] = tmp[1];
        wvalue.byte[2] = tmp[0];
        wvalue.lword = LongBcdToHex(wvalue.lword);
        memcpy(data, &wvalue.lword, 4);
        
        data[4] = 0;
        data[5] = 0;
        
        SYS_OK();
    }            
    else if(id == NETP_D_ASCII5B_TO_HEX2B_DECI1_SYMBOL)
    {
        uint8_t tmp[5];
        memset(tmp,0,5);
        HexStringToNum(buffer+1,tmp,4);
        
        Word32 wvalue;
        wvalue.lword = 0;
        wvalue.byte[0] = tmp[1];
        wvalue.byte[1] = tmp[0];
        wvalue.lword = LongBcdToHex(wvalue.lword);
        
        int32_t integer = wvalue.lword/10;
        int64_t decimal =  (int64_t)(wvalue.lword * 1000)%10000;
        if(buffer[0] == 0x2D)
        {
            integer = -integer;
        }
        memcpy(data, &integer,4);
        
        memcpy(data+4, &decimal,2);
        
        SYS_OK();
    }            
    
    SYS_ERR();
    
}

uint8_t ADATA_TXATFload(uint16 id, uint16 tstp, uint8_t* buffer, uint8_t* data)
{
    double sum = 0;
    int64_t integer=0;
    int64_t decimal=0;
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

        int64_t integer = (int64_t)sum;
        int64_t decimal =  (int64_t)(sum * 10000);
        decimal %= 10000;
        memcpy(data, &integer,4);
        
        memcpy(data+4, &decimal,2);
        
        SYS_OK();
    }
    else if(id == NETP_D_Ty_FLOAT)
    {

        sum = Tancy_Calculate_Float(buffer);
        
        int64_t integer = (int64_t)sum;
        int64_t decimal =  (int64_t)(sum * 10000);
        decimal %= 10000;
        memcpy(data, &integer,4);
        
        memcpy(data+4, &decimal,2);
        
        SYS_OK();

    }
    SYS_ERR();

}
/******************************************************************************
**common核心函数处理表
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
        NETP_D_BIGEND_TO_LITTLE_6B,
        0x0000,
        ADATA_Fload,
    },
    {
        NETP_D_BIG_TO_LIT_4B_DECI2,
        0x0000,
        ADATA_Fload,
    },    
    {
        NETP_D_BIG_TO_LIT_2B_DECI1,
        0x0000,
        ADATA_Fload,
    },    
    {
        NETP_D_BIG_TO_LIT_4B_DECI1,
        0x0000,
        ADATA_Fload,
    },    
    {
        NETP_D_BIG_TO_LIT_8B_INT6B_DECI2B_DIV,
        0x0000,
        ADATA_Fload,
    },    
    {
        NETP_D_BIG_TO_LIT_4B_INT3B_DECI1B_DIV,
        0x0000,
        ADATA_Fload,
    },    
    {
        NETP_D_BIG_TO_LIT_4B_LONG_REV2,
        0x0000,
        ADATA_Fload,
    },   
    {
        NETP_D_BIG_TO_LIT_4B_FLOAT_REV2,
        0x0000,
        ADATA_Fload,
    },   
    {
        NETP_D_BIG_TO_LIT_6B_INT4B_REV2_DECI2B_INVLID1B,
        0x0000,
        ADATA_Fload,
    },   
    {
        NETP_D_ASCII10B_TO_HEX5B,
        0x0000,
        ADATA_Fload,
    },  
    {
        NETP_D_ASCII8B_TO_HEX4B_DECI2,
        0x0000,
        ADATA_Fload,
    },   
    
    {
        NETP_D_ASCII6B_TO_HEX3B_DECI1,
        0x0000,
        ADATA_Fload,
    },  
    {
        NETP_D_ASCII6B_TO_HEX3B_DECI2,
        0x0000,
        ADATA_Fload,
    },   
    {
        NETP_D_ASCII6B_TO_HEX3B,
        0x0000,
        ADATA_Fload,
    },   
//	    {
//	        NETP_D_ASCII6_TO_HEX3,
//	        0x0000,
//	        ADATA_Fload,
//	    },     
    {
        NETP_D_ASCII5B_TO_HEX2B_DECI1_SYMBOL,
        0x0000,
        ADATA_Fload,
    },     
    {
        NETP_D_BIG_TO_LIT_4B_LONG4B,
        0x0000,
        ADATA_Fload,
    },     
    {
        NETP_D_BIG_TO_LIT_8B_LONG4B_DECI4B,
        0x0000,
        ADATA_Fload,
    },     
    {
        NETP_D_BIG_TO_LIT_8B_DOUBLE,
        0x0000,
        ADATA_Fload,
    },     
    {
        NETP_D_BIG_TO_LIT_4B_FLOAT,
        0x0000,
        ADATA_Fload,
    },     
    {
        NETP_D_BIG_TO_LIT_4B_DECI3,
        0x0000,
        ADATA_Fload,
    },    
    {
        NETP_D_BIG_TO_LIT_2B_DECI2,
        0x0000,
        ADATA_Fload,
    },     
    {
        NETP_D_BIG_TO_LIT_8B_FLOAT2_HIGH5_LOW3_DECI,
        0x0000,
        ADATA_Fload,
    },       
    {
        NETP_D_MSB_BCD6B_DECI3,
        0x0000,
        ADATA_BCD_MSB_Fload,
    },       
    {
        NETP_D_LSB_4B_FLOAT,
        0x0000,
        ADATA_HEX_LSB_Fload,
    },   
    {
        NETP_D_LSB_8B_FLOAT4B_H7_FLOAT4B_LOW,
        0x0000,
        ADATA_HEX_LSB_Fload,
    },       
    {
        NETP_D_MSB_8B_LONG4B_H7_LONG4B_LOW,
        0x0000,
        ADATA_HEX_MSB_Fload,
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
** 函数名称: uint8 ADATA_FunctionW(uint16 id, uint16 tstp, uint8* buffer, const CDATA* cd)
** 功能描述: 通过函数来操作数据
** 输　入:  id 数据标识
            tstp bit0 - bit7长度 
            buffer 读取数据的缓存
            cd 内核表中的一项内容
** 输　出: 无
**
** 全局变量: 无
** 调用模块: 无
**
** 作　者: yzy
** 日　期: 2007年9月15日
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
uint8_t ADATA_FunctionW(uint16_t id, uint16_t tstp, uint8_t* buffer, uint8_t* data)
{
    uint16 index;                       //地址索引
    const ADATAFunc* cf;                //表格信息

    index = MidWaySearch(               //搜索数据标识
        (uint8*)gss_ADATAFuncIn,
        sizeof(gss_ADATAFuncIn) / sizeof(ADATAFunc),
        sizeof(ADATAFunc),
        id
        );

    if(index == 0xFFFF)                 //看看搜索结果
    {
//	        _IF_TRUE_RETURN_Z(gui_CoreFuncExtTblSize ==0, CDATA_ERR_DI);
//	        index = MidWaySearch(           //搜索数据标识
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
        cf = gss_ADATAFuncIn + index;   //获取查找到的数据内容
    }
                                        //通过函数调用返回数据
    return cf->func(id, tstp, buffer, data);
}

/******************************************************************************
**common核心地址表
******************************************************************************/
const CDATAAddr gss_CDATAAddrIn[] =
{
    {
        0x0000,                         //程序版本号
        0xF3,                           //绝对内存偏移
        0x00,                           //属性
        (uint32)&gul_UsrFuncVer,        //内存偏移地址
    },
    {
        0x0004,                         //客户化字库版本(初始化后于4505一致)
        DB_PARA,                        //FLASH
        0,                              //属性
        ZKVADDR,                        //内存偏移地址
    },
    {
        0x0017,                         //心跳周期、重拨次数、重试次数、重拨间隔、重试间隔、睡眠延时
        0x00,                           //数据块0
        0x00,                           //属性
        0x27F,                          //内存偏移地址
    },
    {
        0x0040,                         //自定义事件参数
        0x00,                           //数据块0
        0x00,                           //属性
        0x41D,                          //内存偏移地址
    },

    
};

/*********************************************************************************************************
** 函数名称: uint8 CDATA_DirecAddrProc(uint16 id, uint16 tstp, uint8* buffer, const CDATA* cd, 
uint8 type)** 功能描述: Flash地址直接操作
** 输　入:  id 数据标识
**          tstp bit0 - bit5 测量点号 bit6-bit15 同类项数
**          buffer 读取数据的缓存
**          cd 内核表中的一项内容
**          oc 读写属性 true 读 false 写
** 输　出:  0 操作成功, 其它，操作失败
**
** 全局变量: 无
** 调用模块: 无
**
** 作　者: yzy
** 日　期: 2007年11月7日
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
uint8 CDATA_DirecAddrProc(uint16 id, uint16 tstp, uint8* buffer, const CDATA* cd, bool oc)
{
    uint16 index;                       //地址索引
    const CDATAAddr* ca;                //表格信息
    TDataBlock db;                       //数据块
    uint32 addr;                        //偏移地址
    uint16 len;                         //长度

    index = MidWaySearch(               //搜索数据标识
        (uint8*)gss_CDATAAddrIn,
        sizeof(gss_CDATAAddrIn) / sizeof(CDATAAddr),
        sizeof(CDATAAddr),
        cd->id
        );
    if(index == 0xFFFF)
    {
        _IF_TRUE_RETURN_Z(gui_CoreAddrExtTblSize == 0, CDATA_ERR_DI);

        index = MidWaySearch(           //搜索数据标识
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

    addr = ca->addr;                    //相对偏移地址
    if(ca->atr & CORE_ADR_ATRT)         //如果还和测量点相关
    {
                                        //相对偏移乘以测量点号
        addr += (tstp & 0x3F) * (ca->atr & 0x7F) * 64;
    }
    addr += (id - cd->id) * cd->len;    //当前数据标识的位移

    len = cd->len * ((tstp >> 6) + 1);  //读取的数据的长度

    if(oc)                              //读
    {
        if(ca->type <= 0xF0)
        {
            if(SYS_FILE_DB_Open(ca->type, &db, TDB_MODE_RW) != SYS_ERR_OK)
            {
                return CDATA_ERR_BT;    //返回发生底层错误
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
//	                return CDATA_ERR_BT;        //返回发生底层错误
//	            }
//	            ES_FRAM_Close();
        }

        else if(ca->type == 0xF2)
        {
            MoveBuffer(                     //转移数据内容
                (uint8*)addr + 0x7FE0000,   //地址
                buffer,                     //缓存
                len                         //长度
                );
        }
        else if(ca->type == 0xF3)
        {
            MoveBuffer(                     //转移数据内容
                (uint8*)addr,               //地址
                buffer,                     //缓存
                len                         //长度
                );
        }

    }
    else                                    //写
    {
        if(ca->type <= 0xF0)
        {
                                            //打开数据块
            if(SYS_FILE_DB_Open(ca->type, &db, TDB_MODE_RW) != SYS_ERR_OK)
            {
                return CDATA_ERR_BT;        //返回发生底层错误
            }
    
            if(SYS_ERR_OK != SYS_FILE_DB_WriteFrom(&db, buffer, len, addr))
            {
                SYS_FILE_DB_Close(&db);
                return CDATA_ERR_BT;        //返回发生底层错误
            }
            SYS_FILE_DB_Close(&db);
        }
        else if(ca->type == 0xF1)           //直接读取铁电中的数据
        {
//	            ES_FRAM_Open();
//	            if(ES_FRAM_Write(buffer, len, addr) != SYS_ERR_OK)
//	            {
//	                ES_FRAM_Close();
//	                return CDATA_ERR_BT;        //返回发生底层错误
//	            }
//	            ES_FRAM_Close();
        }
        else if(ca->type == 0xF2)
        {
            MoveBuffer(          //转移数据内容
                buffer,                     //缓存
                (uint8*)addr + 0x7FE0000,   //地址
                len                         //长度
                );
        }
        else if(ca->type == 0xF3)
        {
            MoveBuffer(          //转移数据内容
                buffer,                     //缓存
                (uint8*)addr,               //地址
                len                         //长度
                );
        }
    }
    SYS_OK();
}


/******************************************************************************
**common核心数据表
******************************************************************************/
const CDATA gss_CDATAIn[] =
{
    {
        0x0000,                         //程序版本号
        0x0000,                         //同类项
        CORE_TATR_TM,                   //只有终端才有
        4,                              //4个字节
        CDATA_DirecAddrProc,            //直接地址处理方式
    },
    {
        0x0004,                         //客户化程序版本号
        0x0000,                         //同类项
        CORE_TATR_TM,                   //只有终端才有
        4,                              //4个字节
        CDATA_DirecAddrProc,            //函数处理
    },
    {
        0x0017,                         //自定义连接参数
        0x0000,                         //没有同类项
        CORE_TATR_TM,                   //只有终端才有
        10,                             //心跳周期、重拨次数、重试次数、重拨间隔、重试间隔、睡眠延时
        CDATA_DirecAddrProc,            //直接地址处理方式
    },
    {
        0x0040,                         //自定义事件参数
        0x0000,                         //没有同类项
        CORE_TATR_TM,                   //只有终端才有
        4,                              //方式字、信号量、电压、电流
        CDATA_DirecAddrProc,            //直接地址处理方式
    },
    {
        0x0128,                         //时间
        0x0000,                         //0同类项
        CORE_TATR_TM,                   //终端数据
        6,                              //6字节数据
        CDATA_FunctionR,                //函数处理
    },
    //------------------------------------------------------------

    
    
    
    
    
    
    
    
    

};

/*********************************************************************************************************
** 函数名称: uint16 TSTP_GetType(uint8 tstno)
** 功能描述: 获取测量点性质
** 输　入:  tstno: 测量点号
** 输　出:  测量点类型性质
**
** 全局变量: 无
** 调用模块: 无
**
** 作　者: yzy
** 日　期: 2007年11月7日
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
uint16 TSTP_GetType(uint8 tstno)
{
    if(tstno == 0)                          //测量点0为终端数据
    {
        return CORE_TATR_TM;
    }
//	    switch(gss_CDATATst[tstno].tf)
//	    {
//	        case 0x01:                          //485表
//	            return CORE_TATR_MT;
//	        case 0x02:                          //模拟量
//	            return CORE_TATR_SM;
//	        case 0x03:                          //脉冲量
//	            return CORE_TATR_PS;
//	        case 0x04:                          //计算值
//	            return CORE_TATR_CA;
//	        case 0x05:                          //交流采样
//	            return CORE_TATR_AC;
//	        case 0x06:                          //漏电保护器
//	            return CORE_TATR_LD;
//	        default:                            //其他
//	            return 0x0000;
//	    }
    return 0;
}

/*********************************************************************************************************
** 函数名称: const CDATAToBG* CDATA_SearchBgDI(uint16 DI)
** 功能描述: 查找内核数据标识对应的部规标识
** 输　入: DI: 内核数据标识
** 输　出: 对应数据标识的结构体信息指针
**
** 全局变量: 无
** 调用模块: 无
**
** 作　者: yzy
** 日　期: 2007年10月15日
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
const CDATAToBG* CDATA_SearchBgDI(uint16 DI)
{
    uint16 index;
    const CDATA* cd;

    index = ADVMidWaySearch(            //搜索内核表表对应的数据标识
        (uint8*)gss_CDATAIn,
        sizeof(gss_CDATAIn) / sizeof(CDATA),
        sizeof(CDATA),
        DI);
    if(index == 0xFFFF)
    {
        _IF_TRUE_RETURN_Z(gui_CoreTblExtSize == 0, __NULL)
        index = ADVMidWaySearch(            //搜索内核表表对应的数据标识
            (uint8*)gss_CDATAExt,
            gui_CoreTblExtSize,
            sizeof(CDATA),
            DI);
                                           //搜索不到返回空指针
        _IF_TRUE_RETURN_Z(index == 0xFFFF, __NULL);
        cd = gss_CDATAExt + index;
    }
    else
    {
        cd = gss_CDATAIn + index;
    }
     
    index = MidWaySearch(               //搜索数据标识
        (uint8*)gs_CDATAToBG,
        gui_CoreBgTblSize,
        sizeof(CDATAToBG),
        cd->id
        );
                                        //找不到该标识返回空指针
    _IF_TRUE_RETURN_Z(index == 0xFFFF, __NULL);

    return gs_CDATAToBG + index;        //返回对应的指针

}

/*********************************************************************************************************
** 函数名称: uint8 CDATA_SelfSelectR(uint16 id, uint16 tstp, uint8* buffer, const CDATA* cd)
** 功能描述: 通过通讯读取数据
** 输　入:  id 数据标识
            tstp bit0 - bit5 测量点号 bit6-bit15 同类项数
            buffer 读取数据的缓存
            cd 内核表中的一项内容
** 输　出: 无
**
** 全局变量: 无
** 调用模块: 无
**
** 作　者: yzy
** 日　期: 2007年9月6日
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
uint8 CDATA_SelfSelectR(uint16 id, uint16 tstp, uint8* buffer, const CDATA* cd, bool oc)
{
//	    uint16 index;                       //地址索引
//	    uint32 di;                          //部规标识
//	    const CDATAToBG* ca;                //表格信息
//	
//	    index = MidWaySearch(               //搜索数据标识
//	        (uint8*)gs_CDATAToBG,
//	        gui_CoreBgTblSize,
//	        sizeof(CDATAToBG),
//	        cd->id
//	        );
//	
//	    if(index == 0xFFFF)                 //看看搜索结果
//	    {
//	        return CDATA_ERR_DI;            //返回错误数据标识
//	    }
//	
//	    ca = gs_CDATAToBG + index;          //获取查找到的数据内容
//	    di = ca->bgid + (id - ca->cid);
//	
//	    if(ca->proc == __NULL)              //直接读取数据的处理
//	    {
//	        if((tstp & 0x3F) == 0)          //测量点0,终端数据
//	        {
//	            return CDATA_ERR_BT;
//	        }
//	                                        //测量点数据,通过组网端口读取
//	        if(Netp_Read(buffer, di, cd->len, tstp) != NETP_ERR_OK)
//	        {
//	            return CDATA_ERR_BT;;
//	        }
//	        
//	    }
    SYS_OK();
}

/*********************************************************************************************************
** 函数名称: uint8 CDATA_Read(uint16 id, uint16 tstp, uint8* buffer, uint16* len)
** 功能描述: 通过内核表读取数据
** 输　入:  id 数据标识
            tstp bit0 - bit5 测量点号 bit6-bit15 同类项数
            buffer 读取数据的缓存
            len 输入时表示缓存的最大长度，输出时表示读取数据的长度
** 输　出: 无
**
** 全局变量: 无
** 调用模块: 无
**
** 作　者: yzy
** 日　期: 2007年9月6日
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
uint8 CDATA_Read(uint16 id, uint16 tstp, uint8* buffer, uint16* len)
{
    uint16 tatr;                        //测量点属性
    uint16 index;                       //数据索引
    const CDATA* cd;

    tatr = TSTP_GetType(tstp & 0x3F);   //获取测量点的性质
    if(tatr == 0)                       //如果为0则证明没有这个测量点
    {
        return CDATA_ERR_TT;            //返回测量点不正确
    }

    index = ADVMidWaySearch(            //搜索数据标识，采用加强的中值法进行搜索
        (uint8*)gss_CDATAIn,
        sizeof(gss_CDATAIn) / sizeof(CDATA),
        sizeof(CDATA),
        id
        );
    if(index == 0xFFFF)
    {
        _IF_TRUE_RETURN_Z(gui_CoreTblExtSize == 0, CDATA_ERR_DI);
        index = ADVMidWaySearch(            //搜索数据标识，采用加强的中值法进行搜索
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
    
    if(!(cd->tatr & tatr)) //当前测量点不支持这个数据标识
    {
        return CDATA_ERR_TT;            //返回测量点不正确
    }
                                        //判断长度是否正确
    if(cd->len  * ((tstp >> 6) + 1) > *len)
    {
        return CDATA_ERR_LEN;           //返回缓存长度不够
    }

    *len = cd->len  * ((tstp >> 6) + 1);
                                        //判断数据标识和同类项数是否正确
    if((cd->nslm & 0xFFF) + cd->id < id + (tstp >> 6))
    {
        return CDATA_ERR_DI;            //返回错误数据标识
    }
    return cd->proc(id, tstp, buffer, cd, true);
}


/*********************************************************************************************************
** 函数名称: uint8 CDATA_Write(uint16 id, uint16 tstp, uint8* buffer, uint16 len)
** 功能描述: 通过内核表写入数据
** 输　入:  id 数据标识
            tstp bit0 - bit5 测量点号 bit6-bit15 同类项数
            buffer 写入数据的缓存
            len 表示要写入的数据的长度
** 输　出: 无
**
** 全局变量: 无
** 调用模块: 无
**
** 作　者: yzy
** 日　期: 2007年9月6日
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
uint8 CDATA_Write(uint16 id, uint16 tstp, uint8* buffer, uint16 len)
{
    uint16 tatr;                        //测量点属性
    uint16 index;                       //数据索引
    const CDATA* cd;

    tatr = TSTP_GetType(tstp & 0x3F);   //获取测量点的性质
    tatr = tatr;                        //防止编译器告警
//    if(tatr == 0)                       //如果为0则证明没有这个测量点
//    {
//        return CDATA_ERR_TT;            //返回测量点不正确
//    }

    index = ADVMidWaySearch(            //搜索数据标识，采用加强的中值法进行搜索
        (uint8*)gss_CDATAIn,
        sizeof(gss_CDATAIn) / sizeof(CDATA),
        sizeof(CDATA),
        id
        );
    if(index == 0xFFFF)
    {
        _IF_TRUE_RETURN_Z(gui_CoreTblExtSize == 0, CDATA_ERR_DI);
        index = ADVMidWaySearch(            //搜索数据标识，采用加强的中值法进行搜索
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

//    if(!(gss_CDATA[index].tatr & tatr)) //当前测量点不支持这个数据标识
//    {
//        return CDATA_ERR_TT;            //返回测量点不正确
//    }
                                          //判断长度是否正确
    if(cd->len  * ((tstp >> 6) + 1) != len)
    {
        return CDATA_ERR_LEN;           //返回缓存长度不正确
    }
                                        //判断数据标识和同类项数是否正确
    if((cd->nslm & 0xFFF) + cd->id < id + (tstp >> 6))
    {
        return CDATA_ERR_DI;            //返回错误数据标识
    }
    return cd->proc(id, tstp, buffer, cd, false);
    
}


/*********************************************************************************************************
** 函数名称: const CDATA* CDATA_GetInfo(uint16 id)
** 功能描述: 根据内核数据标识，获取内核表的一项内容
** 输　入:  id 数据标识
** 输　出: 无
**
** 全局变量: 无
** 调用模块: 无
**
** 作　者: yzy
** 日　期: 2007年9月14日
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
const CDATA* CDATA_GetInfo(uint16 id)
{
    uint16 index;

    index = ADVMidWaySearch(            //搜索数据标识，采用加强的中值法进行搜索
        (uint8*)gss_CDATAIn,
        sizeof(gss_CDATAIn) / sizeof(CDATA),
        sizeof(CDATA),
        id
        );
    if(index == 0xFFFF)
    {
        _IF_TRUE_RETURN_Z(gui_CoreTblExtSize == 0, __NULL);
        index = ADVMidWaySearch(            //搜索数据标识，采用加强的中值法进行搜索
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





