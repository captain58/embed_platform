/****************************************Copyright (c)*************************
**                               	________科技有限公司
**                                     		开发部
**
**
**--------------文件信息-------------------------------------------------------
**文   件   名: cdef.h
**创   建   人: yzy
**最后修改日期: 2010年3月3日
**描        述: Uart的一些公共定义公开
**注        意:
**--------------历史版本信息---------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2010年3月3日
** 描　述: 原始版本
******************************************************************************/
#ifndef _CDEF_H_
#define _CDEF_H_




/*******************************************************************************
**串口收发的数据位的枚举
*******************************************************************************/
typedef enum 
{
    DataBits_7bits = 0,
    DataBits_8bits
}SerialDataBits;

/*******************************************************************************
**串口收发的校验位的枚举
*******************************************************************************/
typedef enum 
{
    Parit_N = 0,
    Parit_O,
    Parit_E,
}SerialParit;

/*******************************************************************************
**串口收发的停止位的枚举
*******************************************************************************/
typedef enum 
{
    StopBits_1 = 0,
    StopBits_2,
}SerialStopBits;

/*******************************************************************************
**串口配置数据结构体
*******************************************************************************/
typedef struct 
{
    uint32  baudrate;
    SerialDataBits  databits;
    SerialParit     parit;
    SerialStopBits  stopbits;
}SerialSets;

/*******************************************************************************
**串口配置数据结构体
*******************************************************************************/
typedef struct 
{
    uint16  TDR;            //周期
    union{
        struct {
            uint16  mode:4;//模式 0:间隔定时器；1:pwm输出，master;9:pwm输出，slave
            uint16  r00:2;//00
            uint16  edge:2;//边沿 0:下降沿 1:上升沿 2:双边沿(低电平)3:双边沿(高电平)
            uint16  trigger:3;//触发
            uint16  master:1;//2,4,6通道0:slave 1:master   or  (1,3通道)0:16bit  1:8bit 
            uint16  ccs:1;//通道n 计数时钟（fTCLK）的选择
            uint16  r0:1;//保留
            uint16  cks:2;//0:时钟源0；1时钟源2；2:时钟源1；3:时钟源3
        }TMR_bit;
        uint16 TMR;
        
    };
    uint8 level;//输出电平   0正逻辑 ,1负逻辑 
    uint8 stt;  //输出寄存器   0输出‘0’1输出‘1’
    uint8 en;   //允许输出寄存器 0不允许 1允许 
}TimerSets;


/******************************************************************************
**串口缓存定义
******************************************************************************/
typedef struct
{
    uint8 * tbuff;                           //串口发送缓存指针
    uint16 tlen;                            //串口发送缓存长度
    uint16 tcnt;                            //串口发送数据长度
    uint16 tp;                              //串口发送数据指针
    //
    uint8 * rbuff;                           //串口接收缓存指针
    uint16 rlen;                            //串口接收缓存长度
    uint16 rcnt;                            //串口接收数据长度
    uint16 rp;                              //串口接收数据指针
    uint16 pr;                              //环形接收缓存数据处理指针
    //
#if (SYS_UART_ACCT > 0)
    uint8* acc_tbuff;                       //串口发送加速缓存指针
    uint16 acc_tlen;                        //串口发送加速缓存长度
    uint16 acc_tcnt;                        //串口发送加速缓存中数据长度
    uint16 acc_tp;                          //串口发送加速缓存中数据指针
    uint16 acc_pt;                          //环形发送加速缓存数据处理指针
#endif
}SerialBuffer;


/************************************************************************
**rs485额外操作

*************************************************************************/
typedef struct
{
    TCallback setModeRecv;        //切换为接收状态的函数宏
    TCallback setModeSend;        //切换为发送状态的函数宏
    TCallback setModeInit;        //配置收发模式切换的IO口函数宏
}SerialTRC;
/************************************************************************
**串口分离的信息体
*************************************************************************/
typedef struct
{
    uint8_t uart_no;                      //物理串口号(软串口固定为0xff)
    uint8_t irqPri;                       //串口中断优先级
    uint8_t irqn;
    uint8_t * inited;
    
    uint32_t pUART;//USART_TypeDef * pUART;
    uint32_t clk;
    TCallback   pinConfig;               //串口管脚配置函数宏
//    uint8_t rs485;//
    TCallback   msp;

    SerialTRC* rs485;                   //RS485的收发切换操作(无效赋值__NULL)
    SerialBuffer*  buffer;              //串口缓存
    uint8* recvFlag;                    //数据接收完毕标志
    uint16 recvBufLen;                  //串口接收缓存长度
//    UART_HandleTypeDef * phandle;
#if (SYS_UART_ACCT > 0)                  //(accelerate buffer用于提升短数据连续发送时的系统性能)
    uint16 accBufLen;                   //串口发送加速缓存长度(1.大于硬件FIFO长度,2.大于单个systick传送的字节数)
#endif
#if (SYS_UARTSEM_EN > 0)
//	    TESRes * rx_sem;
//	    TESRes * tx_sem;
    TESRes * uart_mutex;

    TESRes * uart_tx_mutex;
    TESRes * uart_rx_mutex;
    TESRes * uart_rx_sem;
    TESRes * uart_tx_sem;    
#endif

//    uint32_t       overSampling;
//    uint32_t       OneBitSampling;
//    uint32_t       AdvFeatureInit;    

}SerialID;
/******************************************************************************
**数据块操作属性
******************************************************************************/
typedef enum
{
    TDB_MODE_R = 1,       //读权限打开
    TDB_MODE_W = 2,       //写权限打开
    TDB_MODE_RW = 3,      //读写打开
    TDB_MODE_BK = 4,      //支持备份的数据块,具有该属性则块必须是偶数页,否则会出错
    TDB_MODE_R_O = 0x10,     //只读
    TDB_MODE_W_O = 0x20,     //只写
}TDB_MODE;
typedef enum
{
    TDB_PERMS_SYS = 1,       //系统权限
    TDB_PERMS_SUSER = 2,       //超级用户
    TDB_PERMS_USER = 3,      //普通用户
}TDB_PERMS;
/******************************************************************************
**定时器操作属性
******************************************************************************/
typedef enum
{
    TIMER_OUTPUT_ON  = 1,       //波形输出打开
    TIMER_TRIGGER_ON = 2,       //计数器打开
    TIMER_ON         = 3,       //输出计数同时打开
    TIMER_INT_ON     = 4,       //中断打开 
}TIMER_MODE;


/******************************************************************************
**数据块信息结构体定义
******************************************************************************/
typedef struct
{
    uint32 start;               //起始地址
    uint32 length;              //长度
    uint8  RW;                  //属性,创建时指定的
    uint8 _b;
    uint16 ID;                  //打开之后是编号,存储的地址中是CRC校验
}TDataBlock;



/******************************************************************************
**数据块信息列表结构体定义
******************************************************************************/
typedef struct
{
    uint16 dbno;                         //文件块号
    uint16 sml;                          //同类项
    uint16 blocks;                       //最小数据块数量
    uint8 atr;                           //属性
}S_FILEBLOCKS;


//flash循环存储区结构体定义
typedef struct
{
    uint32 fls_addr;            //flash区域头部
    uint32 fls_addr_bk;         //flash备份区域头部*
    uint32 fls_addr_end;        //flash区域尾部*
    uint16 keyno;               //条目总数
    uint16 itm_baby_byte;       //单条目内单成员的数据长度
    uint8 itm_baby_num;         //单条目内单成员的循环数量(<128)
    uint8 itm_sectors;          //单条目占用的flash扇区数
}TFlsCycleConst;


/************************************************************************
**时间标签
*************************************************************************/
typedef struct
{
    uint32 mins;
    uint8  YMDhms[6];
    uint16 crc;
    
}S_TimeLabel;//12字节

/******************************************************************************
**系统时间结构体定义
******************************************************************************/
typedef struct 
{
	uint8   sec;       //秒
	uint8   min;       //分
	uint8   hour;      //时
	uint8   day;       //日
	uint8   month;     //月
	uint8   year;      //低字节年
	uint8   yearh;     //高字节年
	uint8   week;      //BIT7:夏令时标识，bit0~bit3：周
    uint16 dyear;       //写入内部RTC时人工计算(0-365)s
}TIME;


/******************************************************************************
**通用输入输出IO口定义
**成员举例:P1.07
**0x1, 7,  (IOCON_FUNC1 | IOCON_MODE_INACT | IOCON_DIGMODE_EN), LPC_GPIO, 1
******************************************************************************/
typedef struct
{
    //
	uint32 pingrp:4;                    //Pin group             //来自PINMUX_GRP_T成员
	uint32 pinnum:8;                    //Pin number            //来自PINMUX_GRP_T成员
	uint32 pinseg:2;                    //pin pfseg 1:通用io, 0:段寄存器
	uint32 modefunc:8;                 //Function and mode     //来自PINMUX_GRP_T成员
    uint32_t speed:10;
    //
    uint8 dir;                          //1:输出,0:输入

}COMPORT;








/*****************************************************************************
**定义IIC端口结构体
******************************************************************************/
typedef struct
{
    COMPORT* portSDA;               //IIC器件数据线
    COMPORT* portSCK;               //IIC器件时钟线
    COMPORT* portPWR;               //电源
    COMPORT* portADDR1;               //地址线1
    COMPORT* portADDR2;               //地址线2
    COMPORT* portADDR3;               //地址线3

}IIC_PORTS;


/*****************************************************************************
**定义GPO端口结构体
******************************************************************************/
typedef struct
{
    COMPORT* port;                      //端口配置
    uint8  set;                      //初始电平
//    uint8  rvs;                         //高低电平翻转:0正序,1逆序
    uint8_t rvs;                         //高低电平翻转:0正序,1逆序
    uint8_t  ival;   //逻辑初始值:0(低),1(高)
    
}GPO_PORTS;

/*****************************************************************************
**定义GPO端口结构体
******************************************************************************/
typedef struct
{
    COMPORT* port;                      //端口配置
    uint8_t     filter:1; //是否需要去抖
    uint8  pullup;                         //是否存在漏电保护
    
	uint16 pingrp:4;                    //Pin group             //来自PINMUX_GRP_T成员
	uint16 pinnum:8;                    //Pin number            //来自PINMUX_GRP_T成员
    uint16 reserver:4;
}GPI_PORTS;

	
#ifndef BOOT_ONLY                       //BOOT不包含定义

#endif

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/
/*
    Port Mode Register (PMm)
*/
/* Pmn pin I/O mode selection (PMm7 - PMm0) */
#define _01_PMn0_NOT_USE        (0x01U) /* not use Pn0 as digital I/O */
#define _01_PMn0_MODE_INPUT     (0x01U) /* use Pn0 as input mode */
#define _00_PMn0_MODE_OUTPUT    (0x00U) /* use Pn0 as output mode */
#define _02_PMn1_NOT_USE        (0x02U) /* not use Pn1 as digital I/O */
#define _02_PMn1_MODE_INPUT     (0x02U) /* use Pn1 as input mode */
#define _00_PMn1_MODE_OUTPUT    (0x00U) /* use Pn1 as output mode */
#define _04_PMn2_NOT_USE        (0x04U) /* not use Pn2 as digital I/O */
#define _04_PMn2_MODE_INPUT     (0x04U) /* use Pn2 as input mode */
#define _00_PMn2_MODE_OUTPUT    (0x00U) /* use Pn2 as output mode */
#define _08_PMn3_NOT_USE        (0x08U) /* not use Pn3 as digital I/O */
#define _08_PMn3_MODE_INPUT     (0x08U) /* use Pn3 as input mode */
#define _00_PMn3_MODE_OUTPUT    (0x00U) /* use Pn3 as output mode */
#define _10_PMn4_NOT_USE        (0x10U) /* not use Pn4 as digital I/O */
#define _10_PMn4_MODE_INPUT     (0x10U) /* use Pn4 as input mode */
#define _00_PMn4_MODE_OUTPUT    (0x00U) /* use Pn4 as output mode */
#define _20_PMn5_NOT_USE        (0x20U) /* not use Pn5 as digital I/O */
#define _20_PMn5_MODE_INPUT     (0x20U) /* use Pn5 as input mode */
#define _00_PMn5_MODE_OUTPUT    (0x00U) /* use Pn5 as output mode */
#define _40_PMn6_NOT_USE        (0x40U) /* not use Pn6 as digital I/O */
#define _40_PMn6_MODE_INPUT     (0x40U) /* use Pn6 as input mode */
#define _00_PMn6_MODE_OUTPUT    (0x00U) /* use Pn6 as output mode */
#define _80_PMn7_NOT_USE        (0x80U) /* not use Pn7 as digital I/O */
#define _80_PMn7_MODE_INPUT     (0x80U) /* use Pn7 as input mode */
#define _00_PMn7_MODE_OUTPUT    (0x00U) /* use Pn7 as output mode */

/*
    Port Register (Pm)
*/
/* Pmn pin data (Pm0 to Pm7) */
#define _00_Pn0_OUTPUT_0        (0x00U) /* Pn0 output 0 */
#define _01_Pn0_OUTPUT_1        (0x01U) /* Pn0 output 1 */
#define _00_Pn1_OUTPUT_0        (0x00U) /* Pn1 output 0 */
#define _02_Pn1_OUTPUT_1        (0x02U) /* Pn1 output 1 */
#define _00_Pn2_OUTPUT_0        (0x00U) /* Pn2 output 0 */
#define _04_Pn2_OUTPUT_1        (0x04U) /* Pn2 output 1 */
#define _00_Pn3_OUTPUT_0        (0x00U) /* Pn3 output 0 */
#define _08_Pn3_OUTPUT_1        (0x08U) /* Pn3 output 1 */
#define _00_Pn4_OUTPUT_0        (0x00U) /* Pn4 output 0 */
#define _10_Pn4_OUTPUT_1        (0x10U) /* Pn4 output 1 */
#define _00_Pn5_OUTPUT_0        (0x00U) /* Pn5 output 0 */
#define _20_Pn5_OUTPUT_1        (0x20U) /* Pn5 output 1 */
#define _00_Pn6_OUTPUT_0        (0x00U) /* Pn6 output 0 */
#define _40_Pn6_OUTPUT_1        (0x40U) /* Pn6 output 1 */
#define _00_Pn7_OUTPUT_0        (0x00U) /* Pn7 output 0 */
#define _80_Pn7_OUTPUT_1        (0x80U) /* Pn7 output 1 */

/*
    Pull-up Resistor Option Register (PUm)
*/
/* Pmn pin on-chip pull-up resistor selection (PUmn) */
#define _00_PUn0_PULLUP_OFF     (0x00U) /* Pn0 pull-up resistor not connected */
#define _01_PUn0_PULLUP_ON      (0x01U) /* Pn0 pull-up resistor connected */
#define _00_PUn1_PULLUP_OFF     (0x00U) /* Pn1 pull-up resistor not connected */
#define _02_PUn1_PULLUP_ON      (0x02U) /* Pn1 pull-up resistor connected */
#define _00_PUn2_PULLUP_OFF     (0x00U) /* Pn2 Pull-up resistor not connected */
#define _04_PUn2_PULLUP_ON      (0x04U) /* Pn2 pull-up resistor connected */
#define _00_PUn3_PULLUP_OFF     (0x00U) /* Pn3 pull-up resistor not connected */
#define _08_PUn3_PULLUP_ON      (0x08U) /* Pn3 pull-up resistor connected */
#define _00_PUn4_PULLUP_OFF     (0x00U) /* Pn4 pull-up resistor not connected */
#define _10_PUn4_PULLUP_ON      (0x10U) /* Pn4 pull-up resistor connected */
#define _00_PUn5_PULLUP_OFF     (0x00U) /* Pn5 pull-up resistor not connected */
#define _20_PUn5_PULLUP_ON      (0x20U) /* Pn5 pull-up resistor connected */
#define _00_PUn6_PULLUP_OFF     (0x00U) /* Pn6 pull-up resistor not connected */
#define _40_PUn6_PULLUP_ON      (0x40U) /* Pn6 pull-up resistor connected */
#define _00_PUn7_PULLUP_OFF     (0x00U) /* Pn7 pull-up resistor not connected */
#define _80_PUn7_PULLUP_ON      (0x80U) /* Pn7 pull-up resistor connected */

/*
    Port Input Mode Register (PIMm)
*/
/* Pmn pin input buffer selection (PIMmn) */
#define _00_PIMn3_TTL_OFF       (0x00U) /* set Pn3 normal input buffer */
#define _08_PIMn3_TTL_ON        (0x08U) /* set Pn3 TTL input buffer */
#define _00_PIMn4_TTL_OFF       (0x00U) /* set Pn4 normal input buffer */
#define _10_PIMn4_TTL_ON        (0x10U) /* set Pn4 TTL input buffer */
#define _00_PIMn5_TTL_OFF       (0x00U) /* set Pn5 normal input buffer */
#define _20_PIMn5_TTL_ON        (0x20U) /* set Pn5 TTL input buffer */
#define _00_PIMn6_TTL_OFF       (0x00U) /* set Pn6 normal input buffer */
#define _40_PIMn6_TTL_ON        (0x40U) /* set Pn6 TTL input buffer */
#define _00_PIMn7_TTL_OFF       (0x00U) /* set Pn7 normal input buffer */
#define _80_PIMn7_TTL_ON        (0x80U) /* set Pn7 TTL input buffer */

/*
    Port Output Mode Register (POMm)
*/
/* Pmn pin output mode selection (POMmn) */
#define _00_POMn0_NCH_OFF       (0x00U) /* set Pn0 output normal mode */
#define _01_POMn0_NCH_ON        (0x01U) /* set Pn0 output N-ch open-drain mode */
#define _00_POMn2_NCH_OFF       (0x00U) /* set Pn2 output normal mode */
#define _04_POMn2_NCH_ON        (0x04U) /* set Pn2 output N-ch open-drain mode */
#define _00_POMn3_NCH_OFF       (0x00U) /* set Pn3 output normal mode */
#define _08_POMn3_NCH_ON        (0x08U) /* set Pn3 output N-ch open-drain mode */
#define _00_POMn4_NCH_OFF       (0x00U) /* set Pn4 output normal mode */
#define _10_POMn4_NCH_ON        (0x10U) /* set Pn4 output N-ch open-drain mode */
#define _00_POMn5_NCH_OFF       (0x00U) /* set Pn5 output normal mode */
#define _20_POMn5_NCH_ON        (0x20U) /* set Pn5 output N-ch open-drain mode */
#define _00_POMn6_NCH_OFF       (0x00U) /* set Pn6 output normal mode */
#define _40_POMn6_NCH_ON        (0x40U) /* set Pn6 output N-ch open-drain mode */
#define _00_POMn7_NCH_OFF       (0x00U) /* set Pn7 output normal mode */
#define _80_POMn7_NCH_ON        (0x80U) /* set Pn7 output N-ch open-drain mode */

/*
    Port Operation Mode Register (PMCm)
*/
/* Pmn pin digital input buffer selection (PMCmn) */
#define _01_PMCn0_NOT_USE       (0x01U) /* not use Pn0 digital input */
#define _00_PMCn0_DI_ON         (0x00U) /* enable Pn0 digital input */
#define _02_PMCn1_NOT_USE       (0x02U) /* not use Pn1 digital input */
#define _00_PMCn1_DI_ON         (0x00U) /* enable Pn1 digital input */
#define _04_PMCn2_NOT_USE       (0x04U) /* not use Pn2 digital input */
#define _00_PMCn2_DI_ON         (0x00U) /* enable Pn2 digital input */
#define _08_PMCn3_NOT_USE       (0x08U) /* not use Pn3 digital input */
#define _00_PMCn3_DI_ON         (0x00U) /* enable Pn3 digital input */
#define _10_PMCn4_NOT_USE       (0x10U) /* not use Pn4 digital input */
#define _00_PMCn4_DI_ON         (0x00U) /* enable Pn4 digital input */
#define _20_PMCn5_NOT_USE       (0x20U) /* not use Pn5 digital input */
#define _00_PMCn5_DI_ON         (0x00U) /* enable Pn5 digital input */
#define _40_PMCn6_NOT_USE       (0x40U) /* not use Pn6 digital input */
#define _00_PMCn6_DI_ON         (0x00U) /* enable Pn6 digital input */
#define _80_PMCn7_NOT_USE       (0x80U) /* not use Pn7 digital input */
#define _00_PMCn7_DI_ON         (0x00U) /* enable Pn7 digital input */

/*
    AD port configuration register (ADPC)
*/
#define _00_ADPC_DI_OFF         (0x00U) /* use P21, P20 as analog input */
#define _01_ADPC_DI_ON          (0x01U) /* use P21, P20 as digital input */
#define _02_ADPC_DI_ON          (0x02U) /* use P20 as digital input */

/*
    LCD port function registers 00 (PFSEG00)
*/
/* Port (other than segment output)/segment outputs specification of Pmn pins (PFSEGxx) */
#define _00_PFSEG04_PORT        (0x00U) /* used the P50 pin as port (other than segment output) */
#define _10_PFSEG04_SEG         (0x10U) /* used the P50 pin as segment output */
#define _00_PFSEG05_PORT        (0x00U) /* used the P51 pin as port (other than segment output) */
#define _20_PFSEG05_SEG         (0x20U) /* used the P51 pin as segment output */
#define _00_PFSEG06_PORT        (0x00U) /* used the P52 pin as port (other than segment output) */
#define _40_PFSEG06_SEG         (0x40U) /* used the P52 pin as segment output */
#define _00_PFSEG07_PORT        (0x00U) /* used the P53 pin as port (other than segment output) */
#define _80_PFSEG07_SEG         (0x80U) /* used the P53 pin as segment output */

/*
    LCD port function registers 01 (PFSEG01)
*/
/* Port (other than segment output)/segment outputs specification of Pmn pins (PFSEGxx) */
#define _00_PFSEG08_PORT        (0x00U) /* used the P54 pin as port (other than segment output) */
#define _01_PFSEG08_SEG         (0x01U) /* used the P54 pin as segment output */
#define _00_PFSEG09_PORT        (0x00U) /* used the P55 pin as port (other than segment output) */
#define _02_PFSEG09_SEG         (0x02U) /* used the P55 pin as segment output */
#define _00_PFSEG10_PORT        (0x00U) /* used the P56 pin as port (other than segment output) */
#define _04_PFSEG10_SEG         (0x04U) /* used the P56 pin as segment output */
#define _00_PFSEG11_PORT        (0x00U) /* used the P57 pin as port (other than segment output) */
#define _08_PFSEG11_SEG         (0x08U) /* used the P57 pin as segment output */
#define _00_PFSEG12_PORT        (0x00U) /* used the P70 pin as port (other than segment output) */
#define _10_PFSEG12_SEG         (0x10U) /* used the P70 pin as segment output */
#define _00_PFSEG13_PORT        (0x00U) /* used the P71 pin as port (other than segment output) */
#define _20_PFSEG13_SEG         (0x20U) /* used the P71 pin as segment output */
#define _00_PFSEG14_PORT        (0x00U) /* used the P72 pin as port (other than segment output) */
#define _40_PFSEG14_SEG         (0x40U) /* used the P72 pin as segment output */
#define _00_PFSEG15_PORT        (0x00U) /* used the P73 pin as port (other than segment output) */
#define _80_PFSEG15_SEG         (0x80U) /* used the P73 pin as segment output */

/*
    LCD port function registers 02 (PFSEG02)
*/
/* Port (other than segment output)/segment outputs specification of Pmn pins (PFSEGxx) */
#define _00_PFSEG16_PORT        (0x00U) /* used the P74 pin as port (other than segment output) */
#define _01_PFSEG16_SEG         (0x01U) /* used the P74 pin as segment output */
#define _00_PFSEG17_PORT        (0x00U) /* used the P75 pin as port (other than segment output) */
#define _02_PFSEG17_SEG         (0x02U) /* used the P75 pin as segment output */
#define _00_PFSEG18_PORT        (0x00U) /* used the P76 pin as port (other than segment output) */
#define _04_PFSEG18_SEG         (0x04U) /* used the P76 pin as segment output */
#define _00_PFSEG19_PORT        (0x00U) /* used the P77 pin as port (other than segment output) */
#define _08_PFSEG19_SEG         (0x08U) /* used the P77 pin as segment output */
#define _00_PFSEG20_PORT        (0x00U) /* used the P30 pin as port (other than segment output) */
#define _10_PFSEG20_SEG         (0x10U) /* used the P30 pin as segment output */
#define _00_PFSEG21_PORT        (0x00U) /* used the P31 pin as port (other than segment output) */
#define _20_PFSEG21_SEG         (0x20U) /* used the P31 pin as segment output */
#define _00_PFSEG22_PORT        (0x00U) /* used the P32 pin as port (other than segment output) */
#define _40_PFSEG22_SEG         (0x40U) /* used the P32 pin as segment output */
#define _00_PFSEG23_PORT        (0x00U) /* used the P33 pin as port (other than segment output) */
#define _80_PFSEG23_SEG         (0x80U) /* used the P33 pin as segment output */

/*
    LCD port function registers 03 (PFSEG03)
*/
/* Port (other than segment output)/segment outputs specification of Pmn pins (PFSEGxx) */
#define _00_PFSEG24_PORT        (0x00U) /* used the P34 pin as port (other than segment output) */
#define _01_PFSEG24_SEG         (0x01U) /* used the P34 pin as segment output */
#define _00_PFSEG25_PORT        (0x00U) /* used the P35 pin as port (other than segment output) */
#define _02_PFSEG25_SEG         (0x02U) /* used the P35 pin as segment output */
#define _00_PFSEG26_PORT        (0x00U) /* used the P46 pin as port (other than segment output) */
#define _08_PFSEG26_SEG         (0x08U) /* used the P46 pin as segment output */
#define _00_PFSEG27_PORT        (0x00U) /* used the P47 pin as port (other than segment output) */
#define _10_PFSEG27_SEG         (0x10U) /* used the P47 pin as segment output */
#define _00_PFSEG28_PORT        (0x00U) /* used the P130 pin as port (other than segment output) */
#define _20_PFSEG28_SEG         (0x20U) /* used the P130 pin as segment output */
#define _00_PFSEG29_PORT        (0x00U) /* used the P22 pin as port (other than segment output) */
#define _40_PFSEG29_SEG         (0x40U) /* used the P22 pin as segment output */
#define _00_PFSEG30_PORT        (0x00U) /* used the P23 pin as port (other than segment output) */
#define _80_PFSEG30_SEG         (0x80U) /* used the P23 pin as segment output */
#define _00_PFDEG_PORT          (0x00U) /* used the P45 pin as port */
#define _04_PFDEG_IVREF0        (0x04U) /* used the P45 pin as analog input (IVREF0) */

/*
    LCD port function registers 04 (PFSEG04)
*/
/* Port (other than segment output)/segment outputs specification of Pmn pins (PFSEGxx) */
#define _00_PFSEG31_PORT        (0x00U) /* used the P24 pin as port (other than segment output) */
#define _01_PFSEG31_SEG         (0x01U) /* used the P24 pin as segment output */
#define _00_PFSEG32_PORT        (0x00U) /* used the P25 pin as port (other than segment output) */
#define _02_PFSEG32_SEG         (0x02U) /* used the P25 pin as segment output */
#define _00_PFSEG33_PORT        (0x00U) /* used the P26 pin as port (other than segment output) */
#define _04_PFSEG33_SEG         (0x04U) /* used the P26 pin as segment output */
#define _00_PFSEG34_PORT        (0x00U) /* used the P27 pin as port (other than segment output) */
#define _08_PFSEG34_SEG         (0x08U) /* used the P27 pin as segment output */
#define _00_PFSEG35_PORT        (0x00U) /* used the P10 pin as port (other than segment output) */
#define _10_PFSEG35_SEG         (0x10U) /* used the P10 pin as segment output */
#define _00_PFSEG36_PORT        (0x00U) /* used the P11 pin as port (other than segment output) */
#define _20_PFSEG36_SEG         (0x20U) /* used the P11 pin as segment output */
#define _00_PFSEG37_PORT        (0x00U) /* used the P12 pin as port (other than segment output) */
#define _40_PFSEG37_SEG         (0x40U) /* used the P12 pin as segment output */
#define _00_PFSEG38_PORT        (0x00U) /* used the P13 pin as port (other than segment output) */
#define _80_PFSEG38_SEG         (0x80U) /* used the P13 pin as segment output */

/*
    LCD port function registers 05 (PFSEG05)
*/
/* Port (other than segment output)/segment outputs specification of Pmn pins (PFSEGxx) */
#define _00_PFSEG39_PORT        (0x00U) /* used the P14 pin as port (other than segment output) */
#define _01_PFSEG39_SEG         (0x01U) /* used the P14 pin as segment output */
#define _00_PFSEG40_PORT        (0x00U) /* used the P15 pin as port (other than segment output) */
#define _02_PFSEG40_SEG         (0x02U) /* used the P15 pin as segment output */
#define _00_PFSEG41_PORT        (0x00U) /* used the P16 pin as port (other than segment output) */
#define _04_PFSEG41_SEG         (0x04U) /* used the P16 pin as segment output */
#define _00_PFSEG42_PORT        (0x00U) /* used the P17 pin as port (other than segment output) */
#define _08_PFSEG42_SEG         (0x08U) /* used the P17 pin as segment output */
#define _00_PFSEG43_PORT        (0x00U) /* used the P00 pin as port (other than segment output) */
#define _10_PFSEG43_SEG         (0x10U) /* used the P00 pin as segment output */
#define _00_PFSEG44_PORT        (0x00U) /* used the P01 pin as port (other than segment output) */
#define _20_PFSEG44_SEG         (0x20U) /* used the P01 pin as segment output */
#define _00_PFSEG45_PORT        (0x00U) /* used the P02 pin as port (other than segment output) */
#define _40_PFSEG45_SEG         (0x40U) /* used the P02 pin as segment output */
#define _00_PFSEG46_PORT        (0x00U) /* used the P03 pin as port (other than segment output) */
#define _80_PFSEG46_SEG         (0x80U) /* used the P03 pin as segment output */

/*
    LCD port function registers 06 (PFSEG06)
*/
/* Port (other than segment output)/segment outputs specification of Pmn pins (PFSEGxx) */
#define _00_PFSEG47_PORT        (0x00U) /* used the P04 pin as port (other than segment output) */
#define _01_PFSEG47_SEG         (0x01U) /* used the P04 pin as segment output */
#define _00_PFSEG48_PORT        (0x00U) /* used the P05 pin as port (other than segment output) */
#define _02_PFSEG48_SEG         (0x02U) /* used the P05 pin as segment output */
#define _00_PFSEG49_PORT        (0x00U) /* used the P06 pin as port (other than segment output) */
#define _04_PFSEG49_SEG         (0x04U) /* used the P06 pin as segment output */

/*
    LCD port function registers 06 (PFSEG06)
*/
/* Port (other than segment output)/segment outputs specification of Pmn pins (PFSEGxx) */
#define _00_PFSEG50_PORT        (0x00U) /* used the P07 pin as port (other than segment output) */
#define _08_PFSEG50_SEG         (0x08U) /* used the P07 pin as segment output */

/*
    LCD input switch control register (ISCLCD)
*/
/* Control of schmitt trigger buffer of CAPL/P126 and CAPH/P127 pins (ISCCAP) */
#define _00_ISCCAP_INVALID      (0x00U) /* makes digital input invalid */
#define _01_ISCCAP_VALID        (0x01U) /* makes digital input valid */
/* Control of schmitt trigger buffer of VL3/P125 pin (ISCVL3) */
#define _00_ISCVL3_INVALID      (0x00U) /* makes digital input invalid */
#define _02_ISCVL3_VALID        (0x02U) /* makes digital input valid */

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define _C0_PM3_DEFAULT         (0xC0U) /* PM3 default value */
#define _FC_PM6_DEFAULT         (0xFCU) /* PM6 default value */
#define _1F_PM12_DEFAULT        (0x1FU) /* PM12 default value */
#define _FE_PM13_DEFAULT        (0xFEU) /* PM13 default value */
#define _F0_PMC1_DEFAULT        (0xF0U) /* PMC1 default value */
#define _03_PMC2_DEFAULT        (0x03U) /* PMC2 default value */

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void R_PORT_Create(void);

/* Start user code for function. Do not edit comment generated here */

/*Pxx,PUxx,PIMxx,POMxx, Reset to 0*/
/*PMxx,PMCxx,PFSEGxx,   Reset to 1*/
#define PORT0OutputConfig(Output)    { PM0 &= ~Output;}/*Configures Port0 as output*/
#define PORT0InputConfig(Input)      { PM0 |= Input;} /*Configures Port0 as input*/
#define PORT0SetBit(Bit)             { P0 |= Bit; }/*Set Port0 bit*/
#define PORT0ClearBit(Bit)           { P0 &= ~Bit;}/*Clears Port0 bit*/
#define PORT0ReadBit(Bit)             (P0 & Bit)/*Reads given Port0 bit value*/
#define PORT0PullUpBit(Bit)          { PU0 |= Bit; }
#define PIM0SetBit(Bit)              { PIM0 |= Bit; }
#define POM0SetBit(Bit)              { POM0 |= Bit; }
#define POM0ClearBit(Bit)            { POM0 &= ~Bit; }

#define PORT1OutputConfig(Output)    { PM1 &= ~Output;}/*Configures Port1 as output*/
#define PORT1InputConfig(Input)      { PM1 |= Input;} /*Configures Port1 as input*/
#define PORT1SetBit(Bit)             { P1 |= Bit; }/*Set Port1 bit*/
#define PORT1ClearBit(Bit)           { P1 &= ~Bit;}/*Clears Port1 bit*/
#define PORT1ReadBit(Bit)             (P1 & Bit)/*Reads given Port1 bit value*/
#define PORT1PullUpBit(Bit)          { PU1 |= Bit; }
#define PIM1SetBit(Bit)              { PIM1 |= Bit; }
#define POM1SetBit(Bit)              { POM1 |= Bit; }
#define POM1ClearBit(Bit)            { POM1 &= ~Bit; }
#define PMC1ClearBit(Bit)            { PMC1 &= ~Bit;}

#define PORT2OutputConfig(Output)    { PM2 &= ~Output;}/*Configures Port2 as output*/
#define PORT2InputConfig(Input)      { PM2 |= Input;} /*Configures Port2 as input*/
#define PORT2SetBit(Bit)             { P2 |= Bit; }/*Set Port2 bit*/
#define PORT2ClearBit(Bit)           { P2 &= ~Bit;}/*Clears Port2 bit*/
#define PORT2ReadBit(Bit)             (P2 & Bit)/*Reads given Port2 bit value*/
#define PORT2PullUpBit(Bit)          { PU2 |= Bit; }
#define PMC2ClearBit(Bit)            { PMC2 &= ~Bit;}

#define PORT3OutputConfig(Output)    { PM3 &= ~Output;}/*Configures Port3 as output*/
#define PORT3InputConfig(Input)      { PM3 |= Input;} /*Configures Port3 as input*/
#define PORT3SetBit(Bit)             { P3 |= Bit; }/*Set Port3 bit*/
#define PORT3ClearBit(Bit)           { P3 &= ~Bit;}/*Clears Port3 bit*/
#define PORT3ReadBit(Bit)             (P3 & Bit)/*Reads given Port3 bit value*/
#define PORT3PullUpBit(Bit)          { PU3 |= Bit; }
#define PIM3SetBit(Bit)              { PIM3 |= Bit; }
#define POM3SetBit(Bit)              { POM3 |= Bit; }
#define POM3ClearBit(Bit)            { POM3 &= ~Bit; }

#define PORT4OutputConfig(Output)    { PM4 &= ~Output;}/*Configures Port4 as output*/
#define PORT4InputConfig(Input)      { PM4 |= Input;} /*Configures Port4 as input*/
#define PORT4SetBit(Bit)             { P4 |= Bit; }/*Set Port4 bit*/
#define PORT4ClearBit(Bit)           { P4 &= ~Bit;}/*Clears Port4 bit*/
#define PORT4ReadBit(Bit)             (P4 & Bit)/*Reads given Port4 bit value*/
#define PORT4PullUpBit(Bit)          { PU4 |= Bit; }
#define PIM4SetBit(Bit)              { PIM4 |= Bit; }
#define PIM4ClearBit(Bit)            { PIM4&= ~Bit; }
#define POM4SetBit(Bit)              { POM4 |= Bit; }
#define POM4ClearBit(Bit)            { POM4 &= ~Bit; }
#define PMC4ClearBit(Bit)            { PMC4 &= ~Bit;}

#define PORT5OutputConfig(Output)    { PM5 &= ~Output;}/*Configures Port5 as output*/
#define PORT5InputConfig(Input)      { PM5 |= Input;} /*Configures Port5 as input*/
#define PORT5SetBit(Bit)             { P5 |= Bit; }/*Set Port5 bit*/
#define PORT5ClearBit(Bit)           { P5 &= ~Bit;}/*Clears Port5 bit*/
#define PORT5ReadBit(Bit)             (P5 & Bit)/*Reads given Port5 bit value*/
#define PORT5PullUpBit(Bit)          { PU5 |= Bit; }
#define PIM5SetBit(Bit)              { PIM5 |= Bit; }
#define POM5SetBit(Bit)              { POM5 |= Bit; }

#define PORT6OutputConfig(Output)    { PM6 &= ~Output;}/*Configures Port6 as output*/
#define PORT6InputConfig(Input)      { PM6 |= Input;} /*Configures Port6 as input*/
#define PORT6SetBit(Bit)             { P6 |= Bit; }/*Set Port6 bit*/
#define PORT6ClearBit(Bit)           { P6 &= ~Bit;}/*Clears Port6 bit*/
#define PORT6ReadBit(Bit)             (P6 & Bit)/*Reads given Port6 bit value*/


#define PORT7OutputConfig(Output)    { PM7 &= ~Output;}/*Configures Port7 as output*/
#define PORT7InputConfig(Input)      { PM7 |= Input;} /*Configures Port7 as input*/
#define PORT7SetBit(Bit)             { P7 |= Bit; }/*Set Port7 bit*/
#define PORT7ClearBit(Bit)           { P7 &= ~Bit;}/*Clears Port7 bit*/
#define PORT7ReadBit(Bit)             (P7 & Bit)/*Reads given Port7 bit value*/
#define PORT7PullUpBit(Bit)          { PU7 |= Bit; }

#define PORT12OutputConfig(Output)    { PM12 &= ~Output;}/*Configures Port12 as output*/
#define PORT12InputConfig(Input)      { PM12 |= Input;} /*Configures Port12 as input*/
#define PORT12SetBit(Bit)             { P12 |= Bit; }/*Set Port12 bit*/
#define PORT12ClearBit(Bit)           { P12 &= ~Bit;}/*Clears Port12 bit*/
#define PORT12ReadBit(Bit)             (P12 & Bit)/*Reads given Port12 bit value*/
#define PORT12PullUpBit(Bit)          { PU12 |= Bit; }

#define PORT13OutputConfig(Output)    { PM13 &= ~Output;}/*Configures Port13 as output*/
#define PORT13InputConfig(Input)      { PM13 |= Input;} /*Configures Port13 as input*/
#define PORT13SetBit(Bit)             { P13 |= Bit; }/*Set Port13 bit*/
#define PORT13ClearBit(Bit)           { P13 &= ~Bit;}/*Clears Port13 bit*/
#define PORT13ReadBit(Bit)             (P13 & Bit)/*Reads given Port13 bit value*/
#define PORT13PullUpBit(Bit)          { PU13 |= Bit; }
#define POM13SetBit(Bit)              { POM13 |= Bit; }
#define POM13ClearBit(Bit)            { POM13 &= ~Bit; }
//LCD port function registers 0 to 6 (PFSEG0 to PFSEG6)
#define PFSEG0ClearBit(Bit)           { PFSEG0 &= ~Bit;}/*Clears bit, Used the Pmn pin as port*/
#define PFSEG1ClearBit(Bit)           { PFSEG1 &= ~Bit;}
#define PFSEG2ClearBit(Bit)           { PFSEG2 &= ~Bit;}
#define PFSEG3ClearBit(Bit)           { PFSEG3 &= ~Bit;}
#define PFSEG4ClearBit(Bit)           { PFSEG4 &= ~Bit;}
#define PFSEG5ClearBit(Bit)           { PFSEG5 &= ~Bit;}
#define PFSEG6ClearBit(Bit)           { PFSEG6 &= ~Bit;}

#endif

