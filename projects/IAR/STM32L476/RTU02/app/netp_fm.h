/****************************************Copyright (c)**************************************************
**                                  ____科技有限公司
**                                          开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: Hexing_Dlms.h
**创   建   人: ZJC
**最后修改日期: 2014年11月3日
**描        述: 海兴DLMS协议解析
**注        意:
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: ZJC
** 版  本: v1.0
** 日　期: 2014年11月3日
** 描　述: 原始版本
********************************************************************************************************/

#ifndef _HEXING_DLMS_H_
#define _HEXING_DLMS_H_


#ifndef EXT_HEXING_DLMS
#define EXT_HEXING_DLMS extern
#endif


/************************************************************************
 *   DLMS 数据类型
************************************************************************/

#define		COMM_DATATYPE_NULL		0//空
#define		COMM_DATATYPE_ARRAY		1//数组
#define		COMM_DATATYPE_STRUCT	2//结构体
#define		COMM_DATATYPE_BOOLEAN	3//布尔代数
#define		COMM_DATATYPE_BITSTRING 4//BIT串
#define		COMM_DATATYPE_INT32    	5//有符号32位
#define		COMM_DATATYPE_INT32U    6//无符号32位
#define		COMM_DATATYPE_OCTS		9//字符串类型
#define		COMM_DATATYPE_ASCII		10//ASCII码
#define		COMM_DATATYPE_BCD		13//BCD码类型
#define		COMM_DATATYPE_INT8      15//有符号8位
#define		COMM_DATATYPE_INT16     16//有符号16位
#define		COMM_DATATYPE_INT8U		17//无符号8位
#define		COMM_DATATYPE_INT16U	18//无符号16位
#define		COMM_DATATYPE_ENUM		22//枚举
#define		COMM_DATATYPE_FLOAT32	23//浮点32位数

typedef enum
{
    NETP_DLMS_TYPE_LINK = 1,		//链路层握手
    NETP_DLMS_TYPE_APP,     		//应用层握手

    NETP_DLMS_TYPE_GET = 0x11,		//查询报文
    NETP_DLMS_TYPE_SET,		        //设置报文
    NETP_DLMS_TYPE_ACTION,          //执行报文
    NETP_DLMS_TYPE_GET_AF,          //后续帧补招
    NETP_DLMS_TYPE_REALY,           //中继方式
    NETP_DLMS_TYPE_LINK_AF,       //link后续帧补招
}EM_DLMS_TYPE;

//曲线数据查询事件参数
#define NETP_READCURE_MIN           2     //延后读取表计时间，防止表计没有完成曲线。
#define NETP_REREADCURE_HOUR        1    //每隔N个小时查询曲线数据是否有遗漏

//日冻结数据查询事件参数
#define NETP_READDAY_MIN   5        //延后读取表计时间，防止表计没有完成日冻结
#define NETP_REREADDAY_HOUR     8    //每隔N个小时查询数据是否有遗漏

//月冻结数据查询事件参数
#define NETP_READMONTH_MIN   8       //延后读取表计时间，防止表计没有完成月冻结


typedef enum
{
    NETP_READ_TX13_F = 0,       //天信1.3
//	    NETP_READ_DLMS_ES = 0,          //事件状态
    NETP_READ_DLMS_E1,       //标准事件
    NETP_READ_DLMS_E2,       //窃电事件
    NETP_READ_DLMS_E3,       //继电器事件
    NETP_READ_DLMS_E4,       //电网事件

    NETP_READ_DLMS_WP,       //正向有功电能
    NETP_READ_DLMS_WN,       //正向无功电能
    NETP_READ_DLMS_U,       //电压
    NETP_READ_DLMS_I,       //电流
    NETP_READ_DLMS_P,       //功率

    NETP_READ_DLMS_DE,       //日冻结电能
    NETP_READ_DLMS_DM,       //月冻结需量
    NETP_READ_DLMS_TM,       //校时

    NETP_UPDATA_ENABLE,       //升级使能
    NETP_UPDATA_MODE,       //升级模式
    NETP_UPDATA_INIT,       //升级信息初始化
    NETP_UPDATA_TRAN,       //文件传输
    NETP_UPDATA_MAP,       //读位图
    NETP_UPDATA_CHECK,       //验证
    NETP_UPDATA_DATETIME,       //设置升级时间
    NETP_UPDATA_ACTION_NOW,       //立即执行
    NETP_UPDATA_READ_STAT,       //读状态
    NETP_READ_VERSION,       //读程序版本，防止重复升级

    NETP_READ_HX_OBIS_MAX,        //最大通道数
}EM_RD_DLMS_ID;

    
    
/************************************************************************
 * DLMS 表计升级
************************************************************************/
#define UPDATABLOCKSIZE     192         //表计升级包长度


#define		F_UpdataFailed		7	//(7)Image activation failed 
#define		F_UpdataOk			6	//(6)Image activation successful,		
#define		F_UpdataFirm		5	//(5)Image activation initiated,			
#define		F_VerifyFailed		4	//(4)Image verification failed,
#define		F_VerifyOk			3	//(3)Image verification successful,
#define		F_WaitVerify		2	//(2)Image verification initiated, 
#define		F_InitOk			1	//(1)Image transfer initiated,
#define		F_NO				0	//(0)Image transfer not initiated,

typedef enum
{
    METER_UPGRADE_NONE = 0,     //无
    METER_UPGRADE_ENABLED,      //（1）允许升级 1
    METER_UPGRADE_READY,       //（2）升级包已传完，2
    METER_UPGRADE_START,       //（3）开始升级
    METER_UPGRADE_INIT,        //（4）开始初始化
    METER_UPGRADE_DATAING,      //（5）升级传输
    METER_UPGRADE_CHECK,        //（6）校验
    METER_UPGRADE_ACTION,       //（7）执行
    METER_UPGRADE_VERSION,      //（8）读取版本号
    METER_UPGRADE_OK,           //（9）升级成功
    METER_UPGRADE_BAD,          //（10）升级失败
}EM_MT_UPGRADE_ID;

    
typedef	struct								//远程升级用的结构体
{
	uint8   mtVer[20];          //表计升级包版本号
    uint8   mtMD5[16];          //表计升级包检验和
    uint8   mtUpdtm[6];            //表计升级执行时间
    uint32  TotalSize;		    //程序总字节数
	uint16  BlockSum;           //总分包数
	uint8   LastSize;           //最后一包的字节数 
	uint8   FlagState[CORE_TST_NO];          //升级状态标志位
    uint16  tstno;              //当前正在升级的测量点号
    uint8   chktype;            //加密类型（00：不加密；01：GCM； 02 ：ECB）
}EXT_UPDATASTRUCT;
EXT_HEXING_DLMS  EXT_UPDATASTRUCT  g_Updata;

EXT_HEXING_DLMS  uint8  guc_mtUpdata_sendfailcnt;       //给表计升级，连续发送失败次数



/************************************************************************
 * @function: DLMS_ByteToInt24
 * @描述: 三个字节转换成24位数
 * 
 * @参数: 
 * @param: byte 存放要转换的三个字节的缓存
* @param: isbcd 输入格式(假hex,真bcd)
 * 
 * @返回: 
 * @return: uint32  三个字节转换成24位数之后的结果
 * @说明: 比如byte[0] = 0x03, byte[1] = 0x02, byte[2] = 0x04，
 *        那么转换后的结果为0x030204
 * @作者: zjc (2014/10/9)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint32 DLMS_ByteToInt24(uint8* byte, bool isbcd);
   
/************************************************************************
 * @function: DLMS_UpEvtERC40414243
 * @描述: 上送表计的事件ID：ERC40414243
 * 
 * @参数: 
 * @param:
 * @返回: 
 * @说明: 
 * @作者: zjc (2014/8/14)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	void DLMS_UpEvtERC40414243(uint8 uc_i, uint8 di, uint8 *buffer);

/************************************************************************
 * @function: Netp_mtGetEvt
 * @描述: 获取事件
 * 
 * @参数: 
 * @param:
 * @返回: 
 * @说明: 
 * @作者: zjc (2014/8/14)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	void Netp_mtGetEvt(ST_ACT_INPBUF_L *inpLbuf, ST_ACT_INPBUF_S *inpSbuf, ST_RDMT_CMD *stpDlmsCmd);

/************************************************************************
 * @function: Netp_ReadEvent
 * @描述: 读取DLMS事件
 * 
 * @参数: 
 * @param: index 抄表端口索引
 * @返回: 
 * @说明: 
 * @作者: zjc (2014/8/14)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	void Netp_ReadEvent(uint8 index);

/************************************************************************
 * @function: ReadDlmsLastDayData
 * @描述: DLMS表读上一日的日冻结数据
 * 
 * @参数: 
 * @param: rdenable  1：必须读一次数据；0：根据情况决定是否读数据 
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: zjc (2014-8-16)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	void ReadDlmsLastDayData(uint8 rdenable);

/************************************************************************
 * @function: ReadDlmsLastMonEnergy
 * @描述: DLMS表读上一日的月冻结电能
 * 
 * @参数: 
 * @param: rdenable  1：必须读一次数据；0：根据情况决定是否读数据 
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: zjc (2014-8-16)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	void ReadDlmsLastMonEnergy(uint8 rdenable);

/************************************************************************
 * @function: ReadDlmsLastMonthDMData
 * @描述: DLMS表读上一月的月冻结数据
 * 
 * @参数: 
 * @param: rdenable  1：必须读一次数据；0：根据情况决定是否读数据 
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: zjc (2014-8-18)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	void ReadDlmsLastMonthDMData(uint8 rdenable);

/************************************************************************
 * @function: Netp_ReadCure
 * @描述: 读取DLMS负荷通道数据，
 * 
 * @参数: 
 * @param: time  曲线点时间
 * @param: din   召测的数据项
 * @param: uc_i 测量点
 * @param: fw  协议配置文件
 * @返回: 
 * @说明: 
 * @作者: zjc (2014/8/14)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 Netp_ReadCure(uint8 *time , uint8 *din, uint8 uc_i, PST_Netp_FW fw);

/************************************************************************
 * @function: Netp_ReadCureData
 * @描述: 读取DLMS负荷通道数据，包括15分钟曲线，
 * 
 * @参数: 
 * @param: index 抄表端口索引
 * @返回: 
 * @说明: 
 * @作者: zjc (2014/8/14)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void Netp_ReadCureData(uint8 index);


/************************************************************************
 * @function: Netp_ReReadCureData
 * @描述: 每隔一个时间段查询，时间段内曲线是否遗漏，对遗漏曲线进行补招
 * 
 * @参数: 
 * @param: index 抄表端口索引
 * @param: readnum 补点数
 * @返回: 
 * @说明: 
 * @作者: zjc (2014/8/14)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	void Netp_ReReadCureData(uint8 index, uint8 readnum);

/************************************************************************
 * @function: Netp_CheckTstTimeDlms
 * @描述: 对下挂dlms电表进行校时
 * 
 * @参数: 
 * @param: index 抄表端口索引
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/2/11)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	void Netp_CheckTstTimeDlms(uint8 index);
   
/************************************************************************
 * @function: Netp_LoadFeeDlms
 * @描述: 对下挂dlms电表进行费率下装
 * 
 * @参数: 
 * @param: index 抄表端口索引
 * @返回: 
 * @说明: 
 * @作者: zjc (2015/01/21)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	void Netp_LoadFeeDlms(uint8 index);

/************************************************************************
 * @function: Netp_UpdataMTInit
 * @描述: 表升级开始
 * 
 * @参数: 
 * @param: 
 * @返回: 
 * @说明: 
 * @作者: zjc (2014/9/3)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	void Netp_UpdataMTInit();
//	
//	/************************************************************************
//	 * @function: Netp_UpdataMTDataing
//	 * @描述: 表升级开始
//	 * 
//	 * @参数: 
//	 * @param: 
//	 * @返回: 
//	 * @说明: 
//	 * @作者: zjc (2014/9/3)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
//	 ************************************************************************/
//	void Netp_UpdataMTDataing();
//	
//	/************************************************************************
//	 * @function: Netp_UpdataMTAction
//	 * @描述: 表升级开始
//	 * 
//	 * @参数: 
//	 * @param: 
//	 * @返回: 
//	 * @说明: 
//	 * @作者: zjc (2014/9/3)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
//	 ************************************************************************/
//	void Netp_UpdataMTAction();
//	
//	/************************************************************************
//	 * @function: Netp_GetVersion
//	 * @描述: 获取版本信息
//	 * 
//	 * @参数: 
//	 * @param: 
//	 * @返回: 
//	 * @说明: 
//	 * @作者: yzy (2014/11/14)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
//	 ************************************************************************/
//	void Netp_GetVersion();
//	
//	
//	/************************************************************************
//	 * @function: Netp_UpdataMTStart
//	 * @描述: 表升级开始
//	 * 
//	 * @参数: 
//	 * @param: 
//	 * @返回: 
//	 * @说明: 
//	 * @作者: zjc (2014/9/3)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
//	 ************************************************************************/
//	void Netp_UpdataMTStart();
//	
//	
//	/************************************************************************
//	 * @function: Netp_MtUpdataPro
//	 * @描述: 表计升级管理
//	 * 
//	 * @参数: 
//	 * @param: 
//	 * @返回: 
//	 * @说明: 
//	 * @作者: zjc (2015/04/03)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
//	 ************************************************************************/
//	void Netp_MtUpdataPro();
//	   
//	/************************************************************************
//	 * @function: Netp_MtUpdataParaInit
//	 * @描述: 表计升级参数初始化
//	 * 
//	 * @参数: 
//	 * @param: 
//	 * @返回: 
//	 * @说明: 
//	 * @作者: zjc (2015/04/03)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
//	 ************************************************************************/
//	void Netp_MtUpdataParaInit();
/************************************************************************
 * @function: Netp_ReadCureData
 * @描述: 读取DLMS负荷通道数据，包括15分钟曲线，
 * 
 * @参数: 
 * @param: index 抄表端口索引
 * @返回: 
 * @说明: 
 * @作者: zjc (2014/8/14)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void Netp_ReadCureData(uint8 index);

int Netp_Get_Framework(uint8_t pf, PST_Netp_FW fw);

/************************************************************************
 * @function: Netp_ReadDLMS
 * @描述: 组网端口DLMS数据读取处理
 * 
 * @参数: 
 * @param: buffer 接收数据缓存
 * @param: di 数据标识
 * @param: length 接收数据的预计长度
 * @param: tstno 测量点号
 * @param: cmdtype 命令类型
 * @返回: 
 * @return: uint8  NETP_ERR_OK,NETP_ERR_NT(有回复,但不支持),NETP_ERR_FT
 * @说明: 
 * @作者: ZJC (2014/8/12)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 Netp_CommonRead(ST_ACT_INPBUF_L *inpLbuf, uint8_t * data, 
                                ST_RDMT_CMD *stpActCmd);
unsigned char GetProtocolNum(unsigned char *tMan, unsigned char *tPro);
int Netp_Register_Set(ST_NETP_FW_BASE * fw);
int Netp_Register_Init(void);
uint16_t Netp_Send_With_Retry2(ST_ACT_INPBUF_L *inpLbuf, uint8_t * data, ST_RDMT_CMD *stpActCmd);

#endif
