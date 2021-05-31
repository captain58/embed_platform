/****************************************Copyright (c)**************************************************
**                                  ________科技有限公司
**                                          开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: pst_zy.h
**创   建   人: yzy
**最后修改日期: 2019年6月18日
**描        述: 通讯模块____规约处理文件包括主动通讯和被动通讯
**注        意:
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2019年6月18日
** 描　述: 原始版本
********************************************************************************************************/
#ifndef _PST_ZY_H_
#define _PST_ZY_H_


#ifndef EXT_ZY
#define EXT_ZY  extern
#else
#define EXT_ZY_SELF
#endif


#define ERR_OK                      0x00    //正常
#define ERR_HARDFAULT_WRITE         0x01    //写硬件错误
#define ERR_HARDFAULT_READ          0x02    //读硬件错误
#define ERR_NO_RECORDE              0x03    //无相关记录
#define ERR_NOT_SUPPORT             0x04    //无此功能

#define ERR_INVALID_PARA            0x06    //参数无效
#define ERR_CHARGE_INDEX            0x07    //充值次数错误
#define ERR_MAXREMAIN               0x08    //到达表存上限
#define ERR_MAXCHARGE               0x09    //达到充值上限

#define ERR_DATATYPE                0x0d    //冻结数据类型错误
#define ERR_DATA                    0x0e    //数据错误

#define ERR_OUT_OF_LIMIT            0x12    //数据量超出限制
#define ERR_RTC                     0x13    //RTC设备错误
#define ERR_CANNOT_CTL_DOOR         0x14    //远程禁止阀门控制
#define ERR_INVALID_ADDR            0x15    //地址超出有效范围
#define ERR_CHARGEINDEX_LO          0x16    //充值次数小
#define ERR_CHARGEINDEX_HI          0x17    //充值次数大
#define ERR_WRITE_ID                0x18    //写表号错误
#define ERR_CLOCK                   0x19    //写表具时钟错误
#define ERR_CANNOT_WRITE_VER        0x1a    //不支持写表具版本号
#define ERR_DEVICE_NUMS             0x1b    //增加设备数目错误
#define ERR_DONT_HAD_DEL_ADDR       0x1c    //要删除的设备地址不存在
#define ERR_DONT_HAD_ADDR           0x1d    //阀门地址不存在
#define ERR_HAD_ADDR                0x1e    //增加安装设备地址已存在
#define ERR_ADDR                    0x1f    //设备地址错误
#define ERR_NO_RESPOND              0x20    //阀门控制器无回应
#define	ERR_FILEINVALID             0x21	//升级文件无效	                       
#define	ERR_ALREADYUPGRADE          0x22	//已为最新程序	                       
#define	ERR_MPRICEINVALID           0x23	//调价信息无效	                       
#define	ERR_MPRICEINDEX             0x24	//调价序号错误	                       
#define	ERR_ALREADYMPRICE           0x25	//已调价（与下发价格表相同）	         
#define	ERR_OPERATE                 0x26	//操作失败	                           


#define LEN_SMS_FRM   110
#define LEN_GPRS_FRM  (LEN_OF_IPSEND - 30)     //
#define LEN_485_FRM   240


/************************************************************************ 
**被动进程发送缓存 
**若置于栈中,可导致栈溢出,如AFN0C_F137
*************************************************************************/
EXT_ZY uint8 gucs_PstSbuf[LEN_GPRS_FRM];


/************************************************************************
 * 看门狗测试重启上报信息结构体
 ************************************************************************/
typedef struct _TWdtTstUp
{
    uint16 key;                //上报复位钥匙(0xBBBB表示上电执行请求上报)
    uint16 crc;                //上报数据crc
    uint8  chn;                //上报数据通道
    uint8  len;                //上报数据长度

}TWdtTstUp;

typedef struct
{
    uint8_t type;
    uint8_t addr;//设备地址
    uint8_t main_addr;//关联的主设备地址
    uint8_t baud;//波特率
    uint8_t factory_model[2];
    uint8_t protocol[2];
}STMeterInfo, *pSTMeterInfo;
#pragma pack(1)
typedef struct
{
    uint8_t addr;//设备地址
    uint16_t factory_model;//厂家ID
    uint16_t factory_type;//型号
    uint8_t remain_max[6];//最大存量
    uint8_t overdraft[6];//透支量
    uint8_t alarm_volume[6];//报警量
    uint8_t reserve[2];//预留
}STPrepayDevInfo;

typedef struct
{
    uint8_t addr;//设备地址
    uint16_t factory_model;//厂家ID
    uint16_t factory_type;//型号
    uint8_t recharge[6];//充值量
    uint8_t index[4];//充值序号
    uint8_t reserve[12];//预留
}STChargeInfo;
typedef struct
{
    uint8_t addr;//设备地址
    uint16_t factory_model;//厂家ID
    uint16_t factory_type;//型号
    uint16_t errno;//错误代码
    
    uint8_t recharge[6];//充值量
    uint8_t index[4];//充值序号
    uint32_t meterstt;//表具状态
    uint8_t reserve[8];//预留
}STChargeret;

typedef struct
{
    uint32_t price;
    uint32_t gas;
}STStep;

typedef struct
{
    uint8_t addr;//设备地址
    uint16_t factory_model;//厂家ID
    uint16_t factory_type;//型号
    uint8_t step_cycle;//阶梯周期
    uint8_t exe_time[4];
    STStep step[5];
    uint16_t index;//调价序号
}STPriceAdjust;

#pragma pack(4)

/************************************************************************
**分支处理函数额外数据信息
*************************************************************************/
typedef struct 
{
    uint16  fn;                         //fn
    uint16  pn;                         //pn
    uint8*  rbuf;                       //接收数据指针
    uint8*  sbuf;                       //发送数据指针
    uint8   info[8];                    //附加信息
    uint16  len;                        //长度

}DATA_EXTRA;

typedef uint8 (*ftype_paraload)(void);


typedef uint8 (*ftype_pararst)(PST_TableStr*);

#define PARA_TYPE_ALL  0x00FF           //参数进程类掩码
#define PARA_TYPE_FARP 0x0001
#define PARA_TYPE_INP  0x0002
#define PARA_TYPE_PST  0x0004
#define PARA_TYPE_TASK 0x0008
#define RUNPARA_TYPE_FARP 0x0010


/************************************************************************
**铁电0_重要历史数据备份结构体
*************************************************************************/
typedef struct
{
    uint32 ver;                     //历史软件版本ascii
    uint8  tbk[6];                  //上分钟时间,年月日时分秒(hex)
    uint8  tdt[6];                  //上次停电时间,年月日时分秒(hex)
    uint8  e1cnt;                   //一类(重要)事件计数器
//	    uint8  e2cnt;                   //二类(一般)事件计数器
    uint8  e1pt;                    //一类(重要)事件发送指针
//	    uint8  e2pt;                    //二类(一般)事件发送指针
    uint8  ckten;                   //电能表对时功能开关(afn05f30控制)
    uint8  rcnt;                    //须确认上报帧计数器(0-63)
    uint8  rpt;                     //须确认上报帧发送指针(0-63)
    uint8  enable;                  //使能记录(B0:主动上报,B1:剔除未实现)
    uint8  esamst;                  //esam 模块状态
    uint8  pwrflag;                 //掉电状态
    uint16 crc;

}S_FramMisData;

EXT_ZY S_FramMisData gs_FramMisData;

EXT_ZY    S_PSTPARA gs_PstPara;
/************************************************************************
 * @function: UserVarInit
 * @描述: 恢复出厂设置
 * 
 * @参数: 
 * @param: tbl 
 * @param: frame 
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2014/2/13)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 UserVarInit(void);


/************************************************************************
 * @function: PST_ZY_Decode
 * @描述: 判断当前缓存里面是否为合法帧,如果是的话则解析成为被动通讯结构体
 * 
 * @参数: 
 * @param: receive 接收到数据的缓存
 * @param: send 需要发送的数据的缓存
 * @param: sbuflen 发送缓存长度
 * @param: frame 数据解析成的结构体
 * 
 * @返回: 
 * @return: uint8  PST_ERR_OK/PST_ERR_FT
 * @说明: 
 * @作者: yzy (2013/7/16)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 PST_ZY_Decode(uint8* receive, uint8* send, uint16 sbuflen, PST_Frame* frame);


/************************************************************************
 * @function: PST_ZY_Proc
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
uint8 PST_ZY_Proc(PST_Frame* frm);


/************************************************************************
 * @function: PST_ZY_Pack
 * @描述: 对智源帧进行打包,被动通讯
 * 
 * @参数: 
 * @param: frame 打包的结构体
 * @param: err 
 * @param: length 输入为打包缓存的最大长度,输出为打包好之后的长度
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2014/5/27)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 PST_ZY_Pack(PST_Frame* frame, uint8 err, uint16* length);



/************************************************************************
 * @function: PST_ZY_Do0F
 * @描述: 对智源帧进行打包,被动通讯
 * 
 * @参数: 
 * @param: data: 0F下一个字节的缓冲区指针,格式:C,DI1,DI0,数据区
 * @param: len: 出入是数据区总长度,传出为生成数据区长度
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 处理终端规约0F控制字
 *      传出数据区从传入指针开始保存,依次包括C,DI1,DI0,数据区
 *      控制码C:01为读,04/05为写
 * @作者: yzy (2014/5/27)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 PST_ZY_Do0F(uint8* data, uint16* len);
uint8 VS_BuildLinkFrm(uint8 type, uint16_t id, uint8* buffer);
uint8_t VS_BuildReportFrm(uint8_t type, uint16_t id, uint16_t err, uint8* buffer);

/************************************************************************
 * @function: GD_Para_RW
 * @描述: 读或写flash参数
 * 
 * @参数: 
 * @param: addr flash偏移地址
 * @param: buff 读写缓存
 * @param: len 需要读写的长度
 * @param: rw false读; true写
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2014/2/12)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 GD_Para_RW(uint32 addr, uint8* buff, uint16 len, bool rw);
/************************************************************************
 * @function: Netp_Framework_RW
 * @描述: 抄表框架配置flash读取
 * 
 * @参数: 
 * @param: addr flash偏移地址
 * @param: buff 读写缓存
 * @param: len 需要读写的长度
 * @param: rw false读; true写
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2014/2/12)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 Netp_Framework_RW(uint32 addr, uint8* buff, uint16 len, bool rw);

#define SET_FLAG(X)     {X = 1;}
#define CLEAR_FLAG(X)   {X = 0;}

/*********************************************************************************
* Function: 计算表数量
* Description:    
* Calls:          
* Called By:      
* Data Accessed: 
* Data Updated:  
* Input:      
* Output:         
* Return:         
* Others:         
*******************************************************************************/
unsigned char GetMeterNum(void);

#endif


