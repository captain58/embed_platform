/****************************************Copyright (c)**************************************************
**                                  ____科技有限公司
**                                          开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: netp.h
**创   建   人: yzy
**最后修改日期: 2007年9月5日
**描        述: 配变程序组网通讯端口的通用函数的头文件
**注        意:
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2007年9月5日
** 描　述: 原始版本
********************************************************************************************************/

#ifndef _NETP_H_
#define _NETP_H_


#ifndef EXT_NETP
#define EXT_NETP extern
#endif


#define NETP_ERR_OK         (uint8)0
#define NETP_ERR_NT         (uint8)1
#define NETP_ERR_FT         (uint8)2
#define NETP_ERR_UV         (uint8)3        //有回复,但非有效值(搜表时使用)


EXT_NETP uint8 ucDlmsReceCnt;   //DLMS接收帧计数
EXT_NETP uint8 ucDlmsSendCnt;   //DLMS发送帧计数

typedef struct
{   
    uint8 hour;
    uint8 day;
    uint8 month;
}STDBFLAG;
EXT_NETP STDBFLAG dbflag;
/************************************************************************ 
**下挂表通讯间隔计数器
*************************************************************************/
EXT_NETP  uint32 gul_NetpReadMins[MCB_PORT_NUM];




/************************************************************************ 
**组网数据抄读信息结构体
*************************************************************************/
typedef struct
{
    uint32  di;                              //部规标识
    uint16 patr;                            //处理属性
    uint16 datr;                            //数据属性
    uint32 add;                             //相对地址
    uint8 zhtp;                             //组合项类型(0:非组合项,1:多费率组合项,others:预留)
    uint8 zhid;                             //组合项编号([zhtp != 0] 0xFF:块抄,others:单抄)
    uint8 zhnum;                            //组合项项数([zhtp != 0 && zhid == 0],否则固定为1)
    uint8 sbit;                             //对应有效状态字中起始位数
    uint8 dgt;                              //增减的小数位数\统一格式掩码
    uint8 len;                              //每项的数据长度 (<=4)
    uint16 _r;                              //数据编码(每种数据编码唯一,用于大小类号识别)
    void* func;                             //特殊处理函数

}NETPRD;

#define  NTRD_ZHTP_NULL  0                  //非组合项
#define  NTRD_ZHTP_MUFL  1                  //多费率数据


#define  NTRD_PATR_SKIP  0x0001             //数据项跳过标记
#define  NTRD_PATR_MUST  0x0002             //必抄项


#define  NTRD_DATR_THEX  0x0001             //转换成HEX格式
#define  NTRD_DATR_BEXT  0x0002             //字节扩展为4字节
#define  NTRD_DATR_PDIV  0x0004             //缩小小数位数
#define  NTRD_DATR_PMUL  0x0008             //扩展小数位数
#define  NTRD_DATR_INSG  0x0010             //带符号数输入(BCD)
#define  NTRD_DATR_OUSG  0x0020             //带符号数输出(HEX)
#define  NTRD_DATR_SPEC  0x0080             //特殊处理







/************************************************************************ 
**组网数据各端口抄读信息结构体
*************************************************************************/
#define SIZE_NETPBMP   8

typedef struct
{
    uint8*  data;                           //内存结构体地址*
    uint8*  eft;                            //数据有效标志位*
    NETPRD* tbl;                            //标识列表*
    uint8   tbllen;                         //标识列表长度*
    uint8   tstno;                          //测量点号*
    uint8   idx;                            //当前标识位图索引    
    uint8   prio;                           //标识优先级(0必抄项)
    uint8   bmp_m[SIZE_NETPBMP];            //高优先级标识位图_must
    uint8   bmp_n[SIZE_NETPBMP];            //低优先级标识位图_normal

}NETPSTR;


EXT_NETP NETPSTR gss_NetpPortStt[MCB_PORT_NUM];




/************************************************************************
 *国网规约大类号小类号与数据项FN的对应
 ************************************************************************/
#define MAX_MAP_DITYPE  32

typedef struct
{
    uint8 dlh;
    uint8 xlh;
    uint8 _r;
    uint8 afncmap[32];                      //需要抄读的一类数据项
    uint8 afndmap[32];                      //需要抄读的二类数据项
    uint8 dimap[MAX_MAP_DITYPE];            //需要抄读的实时数据标识位图
    uint16 crc;
}DXLFnMap;


//EXT_PSTGW DXLFnMap gs_afnDICfg;




/************************************************************************ 
**抄表状态结构体
*************************************************************************/
#define NUM_TSTFAILREC     10

typedef struct
{
    S_TimeLabel start;          //最近抄表起始时间
    S_TimeLabel end;            //最近抄表结束时间
    S_TimeLabel extime;         //数据执行时间
    uint8 BSTOP;                //是否停止状态
    uint8 num_mt;               //总数量
    uint8 num_fl;               //抄表失败数量
    uint8 fl_list[NUM_TSTFAILREC];          //部分失败列表
}S_NetpStt;


__no_init EXT_NETP S_NetpStt gs_NetpReadStt[MCB_PORT_NUM];


EXT_NETP uint32 gul_noPlcReadSecs;

//曲线数据查询事件参数
#define NETP_READCURE_MIN           2     //延后读取表计时间，防止表计没有完成曲线。
#define NETP_REREADCURE_HOUR        1    //每隔N个小时查询曲线数据是否有遗漏

//日冻结数据查询事件参数
#define NETP_READDAY_MIN   5        //延后读取表计时间，防止表计没有完成日冻结
#define NETP_REREADDAY_HOUR     8    //每隔N个小时查询数据是否有遗漏

//月冻结数据查询事件参数
#define NETP_READMONTH_MIN   8       //延后读取表计时间，防止表计没有完成月冻结

/************************************************************************ 
**抄事件的标志位
*************************************************************************/
typedef struct
{
	uint8 time[5];  //事件起始时间
//    uint8 curstart[6];  //曲线起始时间
//    uint8 curend[6];    //曲线结束时间
}S_NetpStEVT;


__no_init EXT_NETP S_NetpStEVT gs_NetpReadStEVT[CORE_TST_NO];



/************************************************************************ 
**对表校时参数
*************************************************************************/
typedef struct
{
    uint16 period;  //周期 小时
    uint8 datatime[5];  //起始时间 ：年月日时分
    uint32 hours;   //总小时数
    uint8  stt;     //校时状态
}S_NetpMtTime;


EXT_NETP S_NetpMtTime gs_NetpMtTime;

  
    
/************************************************************************
 * DLMS 数据输入格式
************************************************************************/

typedef struct
{
    uint8 port;
    uint8 tstno;
    uint8 cmdtype;
    uint8_t addr;//关联的设备地址
    uint8_t addr_len;
    uint8_t baud_cfg;
    uint8 di;
    uint8 nfrm;
    uint8_t ss;
//    ST_Netp_FW * fw;
}ST_RDMT_CMD;    //命令ID集合


/************************************************************************
 * 数据输入格式
************************************************************************/
#define     ACT_MAX_INBUF_H_LEN       512
#define     ACT_MAX_INBUF_L_LEN       256
#define     ACT_MAX_INBUF_S_LEN       256

typedef struct
{
    uint8 buf[ACT_MAX_INBUF_H_LEN];
    uint16 len;
}ST_ACT_INPBUF_H;    //数据输入格式

typedef struct
{
    uint8 buf[ACT_MAX_INBUF_L_LEN];
    uint16 len;
}ST_ACT_INPBUF_L;    //数据输入格式

typedef struct
{
    uint8 buf[ACT_MAX_INBUF_S_LEN];
    uint8 len;
}ST_ACT_INPBUF_S;    //数据输入格式

typedef struct
{
    uint8 * buf;
    uint8 len;
}ST_ACT_INPBUF_N;    //数据输入格式



/************************************************************************
 * @function: Netp_PreInit
 * @描述: 抄表端口初始化
 * 
 * @参数: 
 * @param: index 端口编号
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/2/7)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void Netp_PreInit(uint8 index);

/************************************************************************
 * @function: Netp_PentTran
 * @描述: 组网端口数据透明传输
 * 
 * @参数: 
 * @param: buffer 收发缓存
 * @param: length 收发数据内容长度
 * @param: ss 端口配置
 * @param: port 端口号(0-!)
 * @param: overtime 等待时间,单位100ms
 * @param: nfrm 通讯的规约编号,0xff表示为中继
 * 
 * @返回: 
 * @return: uint8  SYS_ERR_OK, SYS_ERR_FT
 * @说明: 
 * @作者: yzy (2011-9-8)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 Netp_PentTran(uint8* buffer, uint16* length, void* ss, uint16 overtime, uint8 port);

/************************************************************************
 * @function: Netp_Trans
 * @描述: 组网端口数据透明传输(按测量点)
 * 
 * @参数: 
 * @param: buffer 
 * @param: length 
 * @param: overtime 
 * @param: nfrm 
 * @param: tstno 
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2011-9-8)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 Netp_Trans(uint8* buffer, uint16* len, uint8 overtime, uint8 nfrm);

/************************************************************************
 * @function: Netp_BGRead
 * @描述: 组网端口部规数据读取处理
 * 
 * @参数: 
 * @param: buffer 接收数据缓存
 * @param: di 部规标识
 * @param: length 接收数据的预计长度
 * @param: tstno 测量点号
 * 
 * @返回: 
 * @return: uint8  NETP_ERR_FT/NETP_ERR_OK
 * @说明: 
 * @作者: yzy (2014/2/7)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	uint8 Netp_BGRead(uint8* buffer, uint32 di, uint16 length, uint8 tstno);

/************************************************************************
 * @function: Netp_XBGRead
 * @描述: 组网端口部规数据读取处理
 * 
 * @参数: 
 * @param: buffer 接收数据缓存
 * @param: di 数据标识
 * @param: length 接收数据的预计长度
 * @param: tstno 测量点号
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2014/2/7)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	uint8 Netp_XBGRead(uint8* buffer, uint32 di, uint16 length, uint8 tstno);
//	
//	/************************************************************************
//	 * @function: Netp_Read
//	 * @描述: 组网端口读数据处理
//	 * 
//	 * @参数: 
//	 * @param: buffer 接收数据缓存
//	 * @param: di 抄读的标识
//	 * @param: length 接收数据的长度
//	 * @param: tstno 测量点号
//	 * 
//	 * @返回: 
//	 * @return: uint8  NETP_ERR_FT/NETP_ERR_OK
//	 * @说明: 
//	 * @作者: yzy (2014/2/7)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
//	 ************************************************************************/
//	uint8 Netp_Read(uint8* buffer, uint32 di, uint16 length, uint8 tstno);



/************************************************************************
 * @function: Netp_ReadTstDemandData
 * @描述: 更新FLASH需量数据(为了用到时不临时去抄读,比如日月冻结)
 * 
 * @参数: 
 * @param: 
 * @返回: SYS_ERR_FT, SYS_ERR_OK
 * @说明: 
 * @作者: yzy (2012-4-11)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	void Netp_ReadTstDemandData(uint8 port);





/************************************************************************
 * @function: BG_ExtractData
 * @描述: 提取有效数据内容
 * 
 * @参数: 
 * @param: buffer 接收数据缓存,传出有效数据内容
 * @param: length 传入为接收缓存长度,传出为有效数据内容长度
 * @param: di 标识(空表示不判标识)
 * 
 * @返回: 
 * @return: uint8 NETP_ERR_OK,NETP_ERR_NT,NETP_ERR_FT
 * @说明: 
 * @作者: yzy (2011-9-7)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	uint8 BG_ExtractData(uint8* buffer, uint16* length, uint32 di);

/************************************************************************
 * @function: XBG_ExtractData
 * @描述: 提取有效数据内容
 * 
 * @参数: 
 * @param: buffer 接收数据缓存,传出有效数据内容
 * @param: length 传入为接收缓存长度,传出为有效数据内容长度
 * @param: di 标识(空表示不判标识)
 * 
 * @返回: 
 * @return: uint8 NETP_ERR_OK,NETP_ERR_NT,NETP_ERR_FT
 * @说明: 
 * @作者: yzy (2011-9-7)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	uint8 XBG_ExtractData(uint8* buffer, uint16* length, uint32 di);

/************************************************************************
**函数名称: void Netp_StDayProc(uint8 cbport)
**描 述: 冻结日数据
**
**输 入:  cbport 抄表端口号
**输 出: 
**
**备 注: 
**--------------------------------------------------------
**作 者: yzy
**日 期: 2009-08-01
*************************************************************************/
void Netp_StDayProc(uint8 cbport);


/************************************************************************
 * @function: PST_DLMS_HDLC_Decode
 * @描述: 判断当前缓存里面是否为合法帧,如果是的话则解析成为被动通讯结构体
 * 
 * @参数: 
 * @param: receive 接收到数据的缓存
 * @param: send 需要发送的数据的缓存
 * @param: sbuflen 发送缓存长度
 * @param: frame 数据解析成的结构体
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2014/5/27)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	uint8 PST_DLMS_HDLC_Decode(uint8* recv, uint8* send, uint16 sbuflen, PST_Frame* frame);

/************************************************************************
 * @function: PST_DLMS_HDLC_Proc
 * @描述: 
 * 
 * @参数: 
 * @param: frm 数据解析成的结构体
 * 
 * @返回: 
 * @return: uint8  PST_ERR_OK/PST_ERR_FT
 * @说明: 
 * @作者: yzy (2014/5/27)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	uint8 PST_DLMS_HDLC_Proc(PST_Frame* frame);

/************************************************************************
 * @function: Netp_chktimeisok
 * @描述: 判断表计对时时间是否到
 * 
 * @参数: 
 * @param:  
 * @返回: 
 * @说明: 
 * @作者: zjc (2015/5/11)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void Netp_chktimeisok(void);  //

/************************************************************************
 * @function: Netp_Send
 * @描述: 组网端口透明传输(按测量点)
 * 
 * @参数: 
 * @param: phyport 端口
 * @param: buffer 收发缓存
 * @param: len 发送长度
 * @param: overtime 超时时间, 单位100ms
 * @param: ss 串口控制字
 * @param: fw 帧配置
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2011-11-2)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//uint8 Netp_Send(uint8_t phyport, uint8_t * buffer, uint16* len, 
//                    uint8_t overtime, uart_config_t *ss, ST_Netp_FW * fw);
//uint8 Netp_Send(uint8_t phyport, uint8* buffer, uint16* len, 
//                uint8 overtime, void *ss, NetpFrameStr * frameStr);

uint8 Netp_Check(uint8_t port);

int Netp_PowerMng(uint8_t onoff, uint8_t flag);
void SYS_Netp_Init(void);

#endif

