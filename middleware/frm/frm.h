/******************************Copyright(c)******************************
**                         
**
** File Name: dev_frm.h
** Author: 
** Date Last Update: 2019-11-02
** Description: 
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: 文件创建
*************************************************************************/
#ifndef _DEV_FRM_H_
#define _DEV_FRM_H_

#include "public.h"

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
    uint8* end;                         //结尾标志符号
    uint8  elen;                        //结尾的长度
    uint8  cssp;                        //待校验数据相对于帧头的位置
    uint8  extlen;                      //数据长度为0时的帧长度
    uint8  atr;                         //属性
    uint8  cssl;                        //待校验数据长度,相对于(数据域长度+extlen)而言(bit7:0正1负)
    uint8  csco;                        //校验和补偿,相对于(数据域长度+extlen)而言(bit7:0正1负)
}PSTFrameStr;                           //帧结构类型



#define PST_LATR_NOLEN      (uint8)0x80 //置位表示没有帧长度
#define PST_LATR_DBLEN      (uint8)0x40 //置位表示是双字节长度
#define PST_LATR_RELEN      (uint8)0x20 //置位表示长度区重复一次
#define PST_LATR_BFLEN      (uint8)0x04 //高字节在前

#define PST_ATR_PARIT       (uint8)0x80 //置位表示没有校验位
#define PST_ATR_END         (uint8)0x40 //置位表示没有结尾符
#define PST_ATR_CSCAL       (uint8)0x07 //校验和的算法

#define PST_ATR_CSCAL_SC    (uint8)0    //累加和
#define PST_ATR_CSCAL_BS    (uint8)1    //位校验
#define PST_ATR_CSCAL_SD    (uint8)2    //双字节累加和
#define PST_ATR_CSCAL_CRC1  (uint8)3    //DLMS_CRC16
#define PST_ATR_CSCAL_CRC2  (uint8)4    //EDMI_CRC16



/*******************************************************************************
通道序号定义
*******************************************************************************/
typedef enum
{
    PST_CHN_SET = 0,             //本地485
    PST_CHN_WL,                 //无线
    PST_CHN_DUMB,                //空通道
}CHN_ENUM;


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
    uint8 cmd;                          //命令字(0x1E为discover_reply)
    uint8* recv;                        //接收帧数据区
    uint8* send;                        //发送帧数据区
    uint16 len;                         //长度
    uint16 adr;                         //逻辑地址
}DLMS_Frame;

//zwj  南京DM帧结构体
typedef struct
{
	uint8 cmd;                          //命令字(0x1E为discover_reply)
	uint8* recv;                        //接收帧数据区
	uint8* send;                        //发送帧数据区
	uint8 len;                         //长度
	uint16 adr;                         //逻辑地址
}DM_Frame;


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



typedef struct _S_TerAddr{
    uint8 ucMeterAddLen;
    uint8 ucMeterAdd[16];
}S_TerAddr;

typedef struct
{
    uint8 ret;
    uint8 version;                        //XML协议公司内部版本号
    S_TerAddr saddr;                          //发送端的地址
    uint8 type;                     //数据区域数据传输协议
    uint8 cmd;                        //功能码
    uint8 seq;                        //XML帧序号
    uint8 encrypt;                         //数据加密算法
    uint8 check;                          //校验算法
    uint8 random[12];                          //随机数
    uint32 verifyData[8];
    uint16 recvLen;                     //数据区长度
    uint8* recv;                        //接收帧数据区
    uint8* send;                        //发送帧数据区,至少1024个字节
}XML_Frame;
typedef struct
{
    
	STAPDU apdu;
    double rssi;
    uint16 id;
    uint8 bNeedReAllocate;
    uint8 bDebugFlg;
    uint8* recv;                        //接收帧数据区
    uint8* send;                        //发送帧数据区
    uint16 len;                         //长度
}SRF_Frame;
typedef struct
{
    uint16 len;                         //应用数据长度
    uint8  FrameType;                   //控制域类型
    uint8  FrameID;                     //控制域
    uint16 cmd;                         //命令字
    uint8* recv;                        //应用数据 
    uint8* send;                        //发送缓存
}RF_Frame;

/***********************************************************************
**定义通用帧解包结构体
**********************************************************************/
typedef struct
{
    uint8 chninfo;
    uint8 protocol;
    union
    {
#if (_DLT_FRM_INC > 0 || _VS_FRM_INC > 0)
        ZY_Frame _zy;
#endif      
#if _DLT_FRM_INC > 0 
         DLT_Frame _dlt;
#endif
#if _PLC_FRM_INC > 0 
         PLC_Frame _plc;
#endif
#if _DLMS_FRM_INC > 0 
         DLMS_Frame _dlms;
#endif
#if _DM_FRM_INC > 0                    //zwj
         DM_Frame _dm;
#endif
#if PST_FRM_XML_EN > 0 
         XML_Frame _xml;
#endif
#if (_WL1_FRM_INC > 0)
        SRF_Frame _srf;
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
//typedef struct
//{
//    uint16 tid;                         //规约数据标识
//    uint16 idatr;                       //标识属性 同类项数量
//    uint16 cid;                         //内核数据标识 0xFFFF表示特殊处理
//    uint16 ftatr;                       //格式属性
//    void* read;                         //读处理
//    void* write;                        //写处理
//}PST_TableStr;
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


/********************************************************** 
//函数表原型
**********************************************************/
typedef struct _RF_CMD_TABLE_ {
	uint16          cmd; 		//!< 命令名
	void*			func;	//!< 函数功能
} CMD_TABLE_t;
	
typedef uint8 (*FW_COMMAD_t)(const CMD_TABLE_t* tbl, SRF_Frame* frm);
typedef uint8 (*FW_COMMAD_CONF_t)(const CMD_TABLE_t* tbl, RF_Frame* frm);

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


/************************************************************************
 * @function: PST_GetFrmLen
 * @描述: 获得buffer中一个有效帧的长度
 * 
 * @参数: 
 * @param: nfrm 通讯的规约编号.如果nfrm为0xff表示为中继
 * @param: buffer 帧缓存
 * @param: len 帧长度
 * 
 * @返回: 
 * @return: uint16 单帧的长度(0表示没有合格帧)
 * @说明: 
 * @作者: 
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint16 PST_GetFrmLen(uint8 nfrm, uint8* buffer);


#endif
