/****************************************Copyright (c)*************************
**                               	________科技有限公司
**                                     		开发部
**
**
**--------------文件信息-------------------------------------------------------
**文   件   名: dev_frm.c
**创   建   人: yzy
**最后修改日期: 2010年02月26日
**描        述: 规约处理配置头文件
**注        意:
**--------------历史版本信息---------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2010年02月24日
** 描　述: 原始版本
******************************************************************************/
#ifndef _DEV_FRM_H_
#define _DEV_FRM_H_


#define PST_ERR_OK      (uint8)0            //没有问题
#define PST_ERR_VAR     (uint8)1            //参数错误
#define PST_ERR_FT      (uint8)2            //致命错误
#define PST_ERR_NT      (uint8)4            //未知错误
#define PST_ERR_LV      (uint8)5            //权限不足
#define PST_ERR_RE      (uint8)9            //无须回复
#define PST_ERR_NE      (uint8)10           //不是有效帧
#define PST_ERR_TS      (uint8)11           //长度太短
#define PST_ERR_DI      (uint8)12           //错误的数据标识


#define PST_FRM_ANY     (uint8)0xFF         //定义匹配符号

#define FRM_HEADCHAR    5                 //有效帧扫描前导字符数


//
typedef enum
{
    NETP_COMMON = 0,
    NETP_READ_TIME = 1,                 //时间
    NETP_READ_CUR_FLOW,             //标况瞬时流量
    NETP_READ_WORKING_CUR_FLOW,      //工况况瞬时流量
    
    NETP_READ_ACCUMULATE_TOTAL,     //标况累计总量
    NETP_READ_WORKING_ACCUMULATE_TOTAL,     //工况累计总量

    NETP_READ_CUR_FLOW_PREPAY,             //预付费标况瞬时流量
    NETP_READ_WORKING_CUR_FLOW_PREPAY,      //预付费工况况瞬时流量
    
    NETP_READ_ACCUMULATE_TOTAL_PREPAY,     //预付费标况累计总量
    NETP_READ_WORKING_ACCUMULATE_TOTAL_PREPAY,     //预付费工况累计总量

    
    NETP_READ_TEMP,                 //温度
    NETP_READ_PRESSURE,             //压力
    NETP_READ_WARN_FLAG,            //报警字位控说明
    NETP_READ_STT,                  //状态字
    NETP_READ_REMAIND_GAS,          //剩余量
    
    NETP_READ_CUR_PRICE,          //当前单价
    NETP_READ_ACCUMULATE_GAS,       //累积气量
    NETP_READ_RECHARGE_GAS,         //当前充值量
    NETP_READ_RECHARGE_TOTAL_GAS,   //总充值量
    NETP_READ_VOL_HADFLOW,                  //电压状态,百分比，是否有后续
    NETP_READ_VALVE,                //阀门状态
    NETP_READ_WXIC_METER_STT,       //威星IC卡控制器表计状态
    NETP_WRITE_WXIC_PREPAY,                  //IC控制器预付费
    NETP_WRITE_WXIC_PREPAY_RET,                  //IC控制器预付费
    NETP_WRITE_WXIC_RECHARGE,                  //IC控制器充值
    NETP_WRITE_WXIC_RECHARGE_RET,                  //IC控制器充值
    NETP_WRITE_WXIC_PRICE,                  //IC控制器调价
    NETP_WRITE_WXIC_PRICE_RET,                  //IC控制器调价
    
}EM_RD_ID;

//D--Data  Ty-Type  
//A-array  T-时间  TM-温度
//uint8_t array 全0

#define NETP_D_Ty_A_N           1
//uint8_t array 全0xEE

#define NETP_D_Ty_A_E                   2
//uint8_t array 全0xFF

#define NETP_D_Ty_A_F                   3
//YYYY MM DD HH mm ss
//透传字符
#define NETP_D_Ty_WXIC_PREPAY           4

#define NETP_D_Ty_MODBUS_REG_ADDR       5
#define NETP_D_Ty_STR                   6//字符串
#define NETP_D_STR_DATA_TO_BUF          7//字符串


#define NETP_D_Ty_T_BCD1        30
#define NETP_D_BIGEND_TO_LITTLE_6B        31//高低位转换，4字节整数，2字节小数
#define NETP_D_BIG_TO_LIT_4B_DECI2        32//高低位转换,4字节，2位小数
#define NETP_D_BIG_TO_LIT_2B_DECI1        33//高低位转换,2字节，1位小数
#define NETP_D_BIG_TO_LIT_4B_DECI1        34//高低位转换,4字节，1位小数

#define NETP_D_BIG_TO_LIT_8B_INT6B_DECI2B_DIV   35//高低位转换，总长8字节，6字节hex整数，2字节小数（算法除余0x10000）
#define NETP_D_BIG_TO_LIT_4B_INT3B_DECI1B_DIV   36//高低位转换,4字节，3字节hex整数，2位小数（算法除余0x100）

#define NETP_D_BIG_TO_LIT_4B_LONG_REV2       37//高低位转换,4字节hex整数，低两位在前，高两位在后
#define NETP_D_BIG_TO_LIT_4B_FLOAT_REV2       38//高低位转换,4字节浮点数，低两位在前，高两位在后

#define NETP_D_BIG_TO_LIT_6B_INT4B_REV2_DECI2B_INVLID1B       39//高低位转换,4字节hex整数，低两位在前，高两位在后,2字节小数位，只保留2位小数

#define NETP_D_ASCII10B_TO_HEX5B        40//ascii转hex 整数
#define NETP_D_ASCII8B_TO_HEX4B_DECI2        41//ascii转4字节hex 整数，小数位2位
#define NETP_D_ASCII6B_TO_HEX3B_DECI1        42//ascii转3字节hex 整数，小数位1位
#define NETP_D_ASCII6B_TO_HEX3B_DECI2        43//ascii转3字节hex 整数，小数位2位
#define NETP_D_ASCII6B_TO_HEX3B              44//ascii转3字节hex 整数

#define NETP_D_ASCII5B_TO_HEX2B_DECI1_SYMBOL        45//ascii转2字节hex 整数，小数位1位,符号位1位
#define NETP_D_BIG_TO_LIT_4B_LONG4B        46//高低位转换,4字节整数
#define NETP_D_BIG_TO_LIT_8B_LONG4B_DECI4B        47//高低位转换,4字节hex整形，4字节hex整形小数

#define NETP_D_BIG_TO_LIT_8B_DOUBLE       48//高低位转换,8字节双精度浮点数
#define NETP_D_BIG_TO_LIT_4B_FLOAT       49//高低位转换,4字节浮点数
#define NETP_D_BIG_TO_LIT_4B_DECI3        50//高低位转换,4字节，3位小数
#define NETP_D_BIG_TO_LIT_2B_DECI2        51//高低位转换,2字节，2位小数

//高低位转换,2个浮点数，第一个浮点数为高5位F1，第二个浮点数为低三位和小数F2，结果是F1*100000 + F2
#define NETP_D_BIG_TO_LIT_8B_FLOAT2_HIGH5_LOW3_DECI       52

#define NETP_D_MSB_BCD6B_DECI3        53//高位在前（MSB First）,6字节，3位小数
#define NETP_D_LSB_4B_FLOAT       54//低位在前（MSB First）,4字节浮点数
//低位在前（MSB First）,第一个4字节浮点数F1，第二个4字节浮点数F2 结果是F1*0x10000000+F2
#define NETP_D_LSB_8B_FLOAT4B_H7_FLOAT4B_LOW       55
#define NETP_D_MSB_8B_LONG4B_H7_LONG4B_LOW       56


#define NETP_D_Ty_TM_FLOAT      60

#define NETP_D_Ty_P_FLOAT      NETP_D_Ty_TM_FLOAT


//标况瞬时流量 TX13     IST -instant Standard conditions flow
//标况累计总量     TX13 AT accumulative total
#define NETP_D_Ty_FLOAT     100
#define NETP_D_WXIC_LONG     101

#define NETP_D_Ty_DOUBLE      120
//WS-warn status
#define NETP_D_Ty_HEX        140

#define NETP_D_Ty_WS_HEX        140
//S  ter status
#define NETP_D_Ty_S_HEX        NETP_D_Ty_WS_HEX
#define NETP_D_Ty_WXIC_PREPAY_HEX        NETP_D_Ty_WS_HEX

#define NETP_D_B_DEV_ADDR       1



#define CON_MAX_DEVICE_TYPES  64   // 支持的设备类型的数量 

#define MAX_NETP_CP_FRAME_NUM  8

#define MAX_NETP_CP_ITEMS   4  //抄表数据项个数
#define MAX_NETP_CP_RET_ITEMS   16//抄表返回数据项个数

#define MAX_NETP_EFT_BITS 2

#define NETP_FRM_CMDLP_1     0x01 //功能码相对于帧头的位置为1

#define NETP_FRM_DECOMP_DIATR_NULL    (0)//低位是DI跟帧头位置,解帧无DI域

#define NETP_FRM_ADLEN_1     0x01       //地址域长度为1
#define NETP_FRM_ADLP_0      0x00//地址域相对于帧头的位置,其中高位：1表示 地址域长度要累加 0表示不需要累加



#define NETP_FRM_COMP_LP_0       0x00        //定义长度起始的位置为0

#define NETP_FRM_COMP_LATR_NOLEN     (PST_LATR_NOLEN)//

/***********************************************************************
**定义帧结构的结构体
**********************************************************************/
typedef struct
{   
    uint8* head;                        //帧头
    uint8  hlen;                        //帧头的长度
    uint8  lp;                          //数据长度相对于帧头的位置
    uint8  latr;                        //数据长度的属性
    uint8  llft : 4;                    //数据长度解析时先左移,打包时后左移
    uint8  lrft : 4;                    //数据长度解析时后右移,打包时先右移
    uint8  end;                         //结尾标志符号
    uint8  cssp;                        //待校验数据相对于帧头的位置
    uint8  extlen;                      //数据长度为0时的帧长度
    uint8  atr;                         //属性
    uint8  cssl;                        //待校验数据长度,相对于(数据域长度+extlen)而言(bit7:0正1负)
    uint8  csco;                        //校验和补偿,相对于(数据域长度+extlen)而言(bit7:0正1负)
}PSTFrameStr;                           //帧结构类型
#pragma pack(1)


typedef struct
{
    uint8_t id;
    uint8_t len;
    uint8_t atr;
    uint8_t mult;//透传的时候mult当作是表序号
}NetpDataItem;
//	typedef union{
//	    uint8_t str[4];
//	    struct{
//	        uint8_t id;
//	        uint8_t len;
//	        uint8_t atr;
//	        uint8_t b;
//	    }bit;
//	}NetpDataItem;

typedef struct
{
    uint8_t cmd;
    uint8_t rlen;
    uint8_t scmd[4];//second cmd
    uint8_t dilen;
    uint8_t inum;
    uint8_t rinum;   
    NetpDataItem item[MAX_NETP_CP_ITEMS];
    NetpDataItem retItem[MAX_NETP_CP_RET_ITEMS];
}NetpFrameDataStr;


/***********************************************************************
**定义帧结构的结构体
**********************************************************************/
typedef struct
{   
    uint8_t  head[12];                        //帧头
    uint8_t  hlen;                        //帧头的长度
    uint8_t  lp;                          //数据长度相对于帧头的位置
    uint8_t  dlp;                         //数据域相对于帧头的位置
    uint8_t  latr;                        //数据长度的属性
    uint8_t  llft : 4;                    //数据长度解析时先左移,打包时后左移
    uint8_t  lrft : 4;                    //数据长度解析时后右移,打包时先右移
    uint8_t  end;                         //结尾标志符号
    uint8_t  cssp;                        //待校验数据相对于帧头的位置
    uint8_t  extlen;                      //数据长度为0时的帧长度
    uint8_t  atr;                         //属性
    uint8_t  cssl;                        //待校验数据长度,相对于(数据域长度+extlen)而言(bit7:0正1负)
    uint8_t  csco;                        //校验和补偿,相对于(数据域长度+extlen)而言(bit7:0正1负)
    uint8_t  cmdlen;                      //功能码长度
    uint8_t  cmdlp;                       //功能码相对于帧头的位置
    uint8_t  DIatr;                       //DI属性
    uint8_t  alen;                        //地址域长度
    uint8_t  adlp;                        //地址域相对于帧头的位置
}NetpFrameStr;                           //帧结构类型

typedef struct
{   
    uint8_t  head[12];                    //帧头
    uint8_t  hlen;                        //帧头的长度
    uint8_t  lp;                          //数据长度相对于帧头的位置
    uint8_t  latr;                        //数据长度的属性
    uint8_t  llft : 4;                    //数据长度解析时先左移,打包时后左移
    uint8_t  lrft : 4;                    //数据长度解析时后右移,打包时先右移
    uint8_t  end;                         //结尾标志符号
    uint8_t  cssp;                        //待校验数据相对于帧头的位置
    uint8_t  atr;                         //属性
    uint8_t  cssl;                        //待校验数据长度,相对于(数据域长度+extlen)而言(bit7:0正1负)
    uint8_t  alen;                        //地址域长度
    uint8_t  adlp;                        //地址域相对于帧头的位置
    uint8_t  cmdlen;                      //功能码长度
    uint8_t  cmdlp;                       //功能码相对于帧头的位置
    uint8_t  DIatr;                       //DI属性(数据域命令码)
    uint16_t frm_bit;                     //帧结构内容
    uint16_t len_bit;                     //帧结构内容
}NetpComposeFrameStr;                           //帧结构类型


typedef struct _ST_Netp_REG
{
    uint8_t pf;                           //测量点规约
    uint8_t device_name[4];


}ST_Netp_Reg,*PST_Netp_Reg;


typedef struct _ST_Netp_FW
{
    uint8_t pf;                           //测量点规约
    uint8_t device_name[4];
    uint8_t bs;                           //通信控制字
    uint8_t datanum;
    NetpFrameStr frameStr;
    NetpComposeFrameStr compFrameStr;
    NetpFrameDataStr frameDtaStr[MAX_NETP_CP_FRAME_NUM];
//	    uint8_t plen[16];//要上告的数据项，各项长度
//	    uint16_t eft;//上告时要包含的项
    
//	    uint16 crc;
}ST_Netp_FW,*PST_Netp_FW;


typedef struct _ST_NETP_FW_BASE
{
    uint8_t pf;                           //测量点规约
    uint8_t device_name[4];
    uint8_t bs;                           //通信控制字
    uint8_t datanum;
    NetpFrameStr frameStr;
    NetpComposeFrameStr compFrameStr;
    NetpFrameDataStr frameDtaStr[MAX_NETP_CP_FRAME_NUM];
//	    uint8_t plen[16];//要上告的数据项，各项长度
//	    uint16_t eft;//上告时要包含的项
    NetpFrameDataStr PrepayWParaStr;//预付费参数写
    NetpFrameDataStr PrepayRParaStr;//预付费参数读
    NetpFrameDataStr PriceWStr;     //调价
    NetpFrameDataStr PriceRStr;     //读调价
    NetpFrameDataStr RechargeStr;   //充值
    NetpFrameDataStr ValveStr;      //阀控

    uint16 crc;
}ST_NETP_FW_BASE,*PST_NETP_FW_BASE;


#pragma pack(4)

#define PST_LATR_NOLEN      (uint8)0x80 //置位表示没有帧长度
#define PST_LATR_DBLEN      (uint8)0x40 //置位表示是双字节长度
#define PST_LATR_RELEN      (uint8)0x20 //置位表示长度区重复一次
#define PST_LATR_BFLEN      (uint8)0x04 //高字节在前

#define PST_DIATR_NONE      (uint8)0x00 //没有DI
#define PST_DIATR_DB_MB        (uint8)0x80 //modbus寄存器地址

#define PST_DIATR_DBLP      (uint8)0x02 //DI便宜

#define PST_ADLP_ADDLEN      (uint8)0x80 //置位表示地址域长度要累加
#define PST_ADLP_ADD_TO_ASCII      (uint8)0x40 //置位表示地址域转换成ASCII
#define PST_ADLP_ADDLEN_MASK      (uint8)0x3F //

#define PST_ATR_PARIT       (uint8)0x80 //置位表示没有校验位
#define PST_ATR_END         (uint8)0x40 //置位表示没有结尾符
#define PST_ATR_CSCAL       (uint8)0x0F //校验和的算法

#define PST_ATR_CSCAL_SC    (uint8)0    //累加和
#define PST_ATR_CSCAL_BS    (uint8)1    //位校验
#define PST_ATR_CSCAL_SD    (uint8)2    //双字节累加和
#define PST_ATR_CSCAL_CRC1  (uint8)3    //DLMS_CRC16
#define PST_ATR_CSCAL_CRC2  (uint8)4    //EDMI_CRC16
#define PST_ATR_CSCAL_SD1   (uint8)5    //双字节累加和,高位为0
#define PST_ATR_CSCAL_CRC3  (uint8)6    //RtuModbus
#define PST_ATR_CSCAL_CRC4  (uint8)7    //RtuModbus 高位在前
#define PST_ATR_CSCAL_BS_ASCII    (uint8)8    //位校验,ASCII转bin

#define PST_FRM_BIT_HEAD      (uint16_t)0x0001 //置位表示有帧头
#define PST_FRM_BIT_ADDR      (uint16_t)0x0002 //置位表示有地址域
#define PST_FRM_BIT_LEN       (uint16_t)0x0004 //置位表示有长度域
#define PST_FRM_BIT_CMD       (uint16_t)0x0008 //置位表示有功能码域
#define PST_FRM_BIT_DI        (uint16_t)0x0010 //置位表示有DI域

#define PST_FRM_BIT_DATA      (uint16_t)0x0020 //置位表示有数据域

#define PST_FRM_BIT_CS        (uint16_t)0x0040 //置位表示有校验和域
#define PST_FRM_BIT_TAIL      (uint16_t)0x0080 //置位表示有结束符域

#define NETP_FRM_CSSL_SUB_2     0x82        //定义累加和计算的长度偏移
#define NETP_FRM_CSSL_SUB_3     0x83        //定义累加和计算的长度偏移
#define NETP_FRM_CSSL_SUB_4     0x84        //定义累加和计算的长度偏移
#define NETP_FRM_CSSL_SUB_5     0x85        //定义累加和计算的长度偏移

#define NETP_FRM_CSOS_SUB_3     0x83        //定义累加和的位置偏移
#define NETP_FRM_CSOS_SUB_2     0x82        //定义累加和的位置偏移
#define NETP_FRM_CSOS_SUB_5     0x85        //定义累加和的位置偏移


#define NETP_FRM_LP_02       0x02        //定义长度起始的位置
#define NETP_FRM_DLP_02       0x02        //定义数据域起始的位置
#define NETP_FRM_DLP_03       0x03        //定义数据域起始的位置

#define NETP_FRM_WITHOUT_END      00        //定义结尾符

//报文域包含帧头，地址域，命令码域，数据域，DI域，校验域
#define NETP_FRM_COMP_BIT_H_A_C_D_I_C (uint16_t)(PST_FRM_BIT_HEAD|PST_FRM_BIT_ADDR \
                                     | PST_FRM_BIT_CMD | PST_FRM_BIT_DATA \
                                     | PST_FRM_BIT_DI | PST_FRM_BIT_CS)
//长度域，空
#define NETP_FRM_COMP_LEN_BIT_NULL (uint16_t)(0)

#define NETP_FRM_CSSP_FROM_0     0x00        //定义累加和计算起始位置


#define NETP_FRM_MIN_LEN_4     4        //帧最小长度为4

#define NETP_FRM_ATR_MODBUSCRC_NO_END      (PST_ATR_CSCAL_CRC3|PST_ATR_END)

#define NETP_FRM_COMP_DIATR_HAS_02    (PST_DIATR_DB_MB | 0x02)//有DI域，DI跟帧头位置为2

#define EFT_MIN_ST_BIT  0x0001//标况总量
#define EFT_MIN_WT_BIT 0x0002//工况总量
#define EFT_MIN_SI_BIT  0x0004//标况瞬时
#define EFT_MIN_WI_BIT 0x0008//工况瞬时
#define EFT_MIN_TMP_BIT   0x0010//温度
#define EFT_MIN_P_BIT  0x0020//压力



/***********************************************************************
**定义每个通道所支持的帧的类型数量以及允许的前导符
**********************************************************************/
typedef struct
{
    uint8* frameno;                     //帧的类型
    uint8  count;                       //帧的数量
    uint8  headchars;                   //帧的前导符的数量
}PSTChnInfo;

#ifdef PST_FRM_SIMPLE

/***********************************************************************
**定义通用帧解包结构体
**********************************************************************/
typedef struct
{
    uint8 chninfo;
    uint8 tstno;                        //测量点号
    uint8 cmd;                          //命令字
    uint8 power;                        //权限等级
    uint8* recv;                        //接收帧数据区
    uint8* send;                        //发送帧数据区
    uint16 len;                         //长度
    uint8 atr;                          //属性
    uint8 tbd;                          //待扩展
    Word32 wd;                          //数据标志,控制字
    
}PST_Frame;

#else

/************************************************************************ 
**帧信息结构体
*************************************************************************/
typedef struct
{
    uint16 len;                         //应用数据长度
    uint8  atr;                         //属性
    uint8  cmd;                         //控制域
    uint8* R;                           //R域
    uint8* src_addr;                    //源地址
    uint8* dest_addr;                   //目标地址
    uint8  afn;                         //AFN       (应用数据域1)
    uint8  fn;                          //FN        (应用数据域2)
    uint8* app;                         //应用数据  (应用数据域3)
    uint8* send;                        //发送缓存
    Word32 wd;                          //0:请求相别,1:实测相别
}PLC_Frame;


typedef struct
{
    uint8 tstno;                        //测量点号
    uint8 cmd;                          //命令字
    uint16 sbuflen;                     //发送缓存长度
    uint8* send;                        //发送缓存
    uint8* recv;                        //接收帧数据区
    uint16 len;                         //长度
    uint8 atr;                          //属性
    uint8 tbd;                          //待扩展
    Word32 wd;                          //数据标志,控制字
}DLT_Frame;

typedef struct
{
//	    uint8 tstno;                        //测量点号
    uint8 cmd;                          //命令字
    uint16 sbuflen;                     //发送缓存长度
    uint8* send;                        //发送缓存
    uint8* recv;                        //接收帧数据区
    uint16 len;                         //长度
    uint8 atr;                          //属性
    uint8 tbd;                          //待扩展
    uint8 * time;
    uint8 seq;
    Word32 wd;                          //数据标志,控制字
}ZY_Frame;

typedef struct
{
    uint8 tstno;                        //测量点号
    uint8 cmd;                          //命令字
    uint16 sbuflen;                     //发送缓存长度
    uint8* send;                        //发送缓存
    uint8* recv;                        //接收帧数据区
    uint16 len;                         //长度
    uint8 atr;                          //属性
    uint8 tbd;                          //待扩展
    Word32 wd;                          //数据标志,控制字
}NETP_Frame;


typedef struct
{
    uint8 cmd;                          //命令字(0x1E为discover_reply)
    uint8* recv;                        //接收帧数据区
    uint8* send;                        //发送帧数据区
    uint16 len;                         //长度
    uint16 adr;                         //逻辑地址
}DLMS_Frame;



/***********************************************************************
**定义通用帧解包结构体
**********************************************************************/
typedef struct
{
    uint8 chninfo;
    union
    {
#if (_DLT_FRM_INC > 0 || _VS_FRM_INC > 0)
        DLT_Frame _dlt;
#endif
#if (_DLT_FRM_INC > 0 || _VS_FRM_INC > 0)
        ZY_Frame _zy;
#endif
#if _PLC_FRM_INC > 0 
        PLC_Frame _plc;
#endif
#if _DLMS_FRM_INC > 0 
        DLMS_Frame _dlms;
#endif
#if _NETP_FRM_INC > 0
        NETP_Frame _netp;
#endif
    };
}PST_Frame;

#endif

#define PST_FRM_CHNNO   0x3F            //通道编号掩码
                                        // 
#define PST_FRM_POWLV   0x0F            //权限等级掩码
                                        // 
#define PST_FRM_ATRWR   0x01            //读写属性
#define PST_FRM_ATRADR  0x06            //地址属性
#define PST_FRM_ATRADE  0x08            //地址是否正确

#define PST_FRM_ADROK   0x00            //普通地址
#define PST_FRM_ADRBR   0x02            //广播地址
#define PST_FRM_ADRFL   0x04            //全能地址




/***********************************************************************
**被动通讯表格定义
**********************************************************************/
typedef struct
{
    uint16 tid;                         //规约数据标识
    uint16 idatr;                       //标识属性 同类项数量
    uint16 cid;                         //内核数据标识 0xFFFF表示特殊处理
    uint16 ftatr;                       //格式属性
    void* read;                         //读处理
    void* write;                        //写处理
    
    uint16  len;                        //长度属性(FLASH分配的总长度)
    uint32  addr;                       //终端参数地址(flash)
    
    uint16_t type;                      //
    void*   load;                       //载入函数(类型:uint8 (*func)(), __NULL无需载入)
    const uint8* a_def;                 //默认参数地址(rom, 0表示默认全为零)
    void*   f_def;                      //默认参数特殊处理

}PST_TableStr;


typedef uint8 (*PSTTblPrc)(const PST_TableStr*, PST_Frame*);

#define PST_TBL_SMLR1   0x8000          //第一类同类项
#define PST_TBL_SMLR2   0x4000          //第二类同类项
#define PST_TBL_SMLRN   0x0FFF          //同类项的数量




/************************************************************************
 * @function: PST_CheckFrame
 * @描述: 
 * 
 * @参数: 
 * @param: num 需要判断的帧的类型编号
 * @param: buffer 需要判断有效帧的缓存
 * @param: length 需要判断的帧的长度
 * 
 * @返回: 
 * @return: uint8  PST_ERR_OK
 *      PST_ERR_VAR
 *      PST_ERR_TS
 *      PST_ERR_NE
 * @说明: 
 * @作者: yzy (2013/7/15)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 PST_CheckFrame(uint8 num, uint8* buffer, uint16 length);

/************************************************************************
 * @function: PST_Channel
 * @描述: 带前导符扫描接收报文,判断出为何种规约
 * 
 * @参数: 
 * @param: channel 需要判断有效帧的通道编号
 * @param: buffer 需要判断有效帧的缓存
 * @param: length 需要判断的帧的长度
 * @param: protocol 输出,实际规约编号,如果是0xFF就表示不是有效规约(例:PST_FRM_ZY_NO)
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2013/7/15)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 PST_Channel(uint8_t channel, uint8* buffer, uint16 length, uint8* protocol);


/************************************************************************
 * @function: PST_DoneFrame
 * @描述: 进行帧头\帧尾\累加和的填写,对于没有数据区长度的帧这里无法完成其打包
 * 
 * @参数: 
 * @param: num 需要打包的帧的类型编号
 * @param: buffer 需要打包成有效帧的缓存
 * 
 * @返回: 
 * @return: uint16  0 打包失败, 其他 打包后数据的长度
 * @说明: 
 * @作者: yzy (2013/7/15)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint16 PST_DoneFrame(uint8 num, uint8* buffer);

/*******************************************************************************
 * @function_name:  FRM_Decode
 * @function_file:  dev_frm.c
 * @描述: 规约解析函数
 * 
 * 
 * @参数: 
 * @param: protocal  规约序号
 * @param: rbuffer  接收缓存
 * @param: sbuffer  发送缓存
 * @param: sbuflen  发送缓存长度
 * @param: frm  规约帧结构体信息指针
 * 
 * @返回: 
 * @return:  uint8   
 * @作者: yzy (2010-02-27)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
uint8 FRM_Decode(uint8 protocal, uint8* rbuffer, uint8* sbuffer, uint16 sbuflen, PST_Frame* frm);



/*******************************************************************************
 * @function_name:  FRM_Proc
 * @function_file:  dev_frm.c
 * @描述: 规约处理函数
 * 
 * 
 * @参数: 
 * @param: protocal  规约序号
 * @param: frm  规约帧结构体信息指针
 * 
 * @返回: 
 * @return:  uint8   
 * @作者: yzy (2010-03-03)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
uint8 FRM_Proc(uint8 protocal, PST_Frame* frm);


/************************************************************************
 * @function: PST_CheckFrame2
 * @描述: 指定类型帧有效性的检查(若存在前导符,则去掉前导符./不去除帧后的无效字符)
 * 
 * @参数: 
 * @param: nfrm 通讯的规约编号.如果nfrm为0xff表示为中继
 * @param: buffer 帧缓存
 * @param: len 帧长度
 * 
 * @返回: 
 * @return: uint16 合格帧的长度(0表示没有合格帧)
 * @说明: 
 * @作者: yzy (2012-5-15)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint16 PST_CheckFrame2(uint8 nfrm, uint8* buffer, uint16 len);


uint8 PST_CheckFrame3(uint8 nfrm, uint8* buffer, uint16* len);

/************************************************************************
 * @function: NETP_CheckFrame2
 * @描述: 指定类型帧有效性的检查(若存在前导符,则去掉前导符./不去除帧后的无效字符)
 * 
 * @参数: 
 * @param: str 规约解析结构体
 * @param: buffer 帧缓存
 * @param: len 帧长度
 * 
 * @返回: 
 * @return:0成功  其他失败
 * @说明: 
 * @作者: yzy (2012-5-15)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/

uint8 NETP_CheckFrame2(const NetpFrameStr* str, uint8* buffer, uint16* len);
/*********************************************************************************************************
**函数名称  int NETP_DoneFrame(const NetpComposeFrameStr * frmstr, const NetpFrameDataStr * frameDtaStr, uint8_t * buf, uint8* addr, uint16* length)
**输入
        frmstr 打包的结构体
        frameDtaStr 数据域打包配置
        buf 数据缓存
        addr 打包的地址            
        length 输入为打包缓存的最大长度，输出为打包好之后的长度
**输出  PST_ERR_OK
        PST_ERR_VAR
**      PST_ERR_FT
**说明  对抄表帧帧进行打包，主动通讯
** 全局变量:
** 调用模块:
**
**------------------------------------------------------------------------------------------------------
** 作  者: yzy
** 日　期: 2019年6月18日
** 内  容: 
********************************************************************************************************/

//int NETP_DoneFrame(const NetpComposeFrameStr * frmstr, const NetpFrameDataStr * frameDtaStr, uint8_t * buf, uint8* addr, uint16* length);
int NETP_DoneFrame( const NetpComposeFrameStr * frmstr, 
                    const NetpFrameDataStr * frameDtaStr, 
                            uint8_t * buf, uint8_t* addr, uint8_t *data, uint16_t* length);

uint16_t Netp_Get_FW_num(void);


#endif
