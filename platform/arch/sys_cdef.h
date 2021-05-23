/******************************Copyright(c)******************************
**                         
**
** File Name: 
** Author: 
** Date Last Update: 2019-11-02
** Description: 平台常用服务数据类型定义
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: 文件创建
*************************************************************************/
#ifndef _SYS_CDEF_H_
#define _SYS_CDEF_H_


//系统时钟默认使用RCHF
#define RCHFCLKCFG	8	//8, 16, 24, 36MHz

//define_all.h中RCHFCLKCFG控制系统时钟
#if( RCHFCLKCFG == 8 )//8.0MHz
#define clkmode   1
#define SYSCLKdef RCC_RCHFCON_FSEL_8MHZ//RCHF中心频率8MHz
#elif( RCHFCLKCFG == 16 )//16.0MHz
#define clkmode   2
#define SYSCLKdef RCC_RCHFCON_FSEL_16MHZ//RCHF中心频率16MHz
#elif( RCHFCLKCFG == 24 )//24.0MHz
#define clkmode   3
#define SYSCLKdef RCC_RCHFCON_FSEL_24MHZ//RCHF中心频率24MHz
#elif( RCHFCLKCFG == 36 )//36.0MHz
#define clkmode   4
#define SYSCLKdef RCC_RCHFCON_FSEL_36MHZ//RCHF中心频率36MHz
#endif





/************************************************************************
**串口缓存定义
*************************************************************************/
typedef struct
{
    uint8* tbuff;                           //串口发送缓存指针
    uint16 tlen;                            //串口发送缓存长度
    uint16 tcnt;                            //串口发送数据长度
    uint16 tp;                              //串口发送数据指针
    //
    uint8* rbuff;                           //串口接收缓存指针
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
    uint32_t   RxBuf_is_full;
    uint32_t  uart_error_count;
    uint32_t  uart_dma_stop;  //this bit is used only for error count now
    uint32_t  previous_dma_leftbyte;

}SerialBuffer;


/************************************************************************
**串口配置数据结构体
*************************************************************************/

/************************************************************************
**串口收发的数据位的枚举
*************************************************************************/
typedef enum 
{
    DataBits_5bits = 0,
    DataBits_6bits,
    DataBits_7bits,
    DataBits_8bits,
    DataBits_9bits
}SerialDataBits;

/************************************************************************
**串口收发的校验位的枚举
*************************************************************************/
typedef enum 
{
    Parit_N = 0,     // 无
    Parit_O,        //奇
    Parit_E,        //偶
}SerialParit;

/************************************************************************
**串口收发的停止位的枚举
*************************************************************************/
typedef enum 
{
    StopBits_1 = 0,
    StopBits_2,
}SerialStopBits;
/*
 * UART flow control
 */
typedef enum {
    FLOW_CONTROL_DISABLED,
    FLOW_CONTROL_CTS,
    FLOW_CONTROL_RTS,
    FLOW_CONTROL_CTS_RTS
} SerialFlowControl;
/*
 * UART mode
 */
typedef enum {
    MODE_TX,
    MODE_RX,
    MODE_TX_RX
} SerialMode;

typedef struct 
{
    uint32  baudrate;
    SerialDataBits  databits;
    SerialParit     parit;
    SerialStopBits  stopbits;
    
    SerialFlowControl  flowctrl;
    SerialMode mode;
}SerialSets;


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
    
    uint8_t uart;//USART_TypeDef * pUART;
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

    uint32_t       overSampling;
    uint32_t       OneBitSampling;
    uint32_t       AdvFeatureInit;    

}SerialID;









/************************************************************************
**数据块操作属性
*************************************************************************/
typedef enum
{
    TDB_MODE_R = 1,       //读权限打开
    TDB_MODE_W = 2,       //写权限打开
    TDB_MODE_RW = 3,      //读写打开
    TDB_MODE_BK = 4,      //支持备份的数据块,具有该属性则块必须是偶数页,否则会出错
}TDB_MODE;
typedef enum
{
    TDB_PERMS_SYS = 1,       //系统权限
    TDB_PERMS_SUSER = 2,       //超级用户
    TDB_PERMS_USER = 3,      //普通用户
}TDB_PERMS;

/************************************************************************
**数据块信息结构体定义
*************************************************************************/
typedef struct
{
    uint32 start;               //起始地址
    uint32 length;              //长度
    uint8  RW;                  //属性,创建时指定的
    uint8 _b;
    uint16 ID;                  //打开之后是编号,存储的地址中是CRC校验
}TDataBlock;



/************************************************************************
**数据块信息列表结构体定义
*************************************************************************/
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
**通用输入输出IO口定义
*************************************************************************/
//	typedef struct
//	{
//	    GPIOx_Type* GPIOx;
//	
//	    uint8 port;                         //pin group
//	    uint8 pin;                          //pin number
//	    uint32 mode;                        //function and mode
//	}COMPORT;



/************************************************************************
 * COMPORT模式配置宏
 *************************************************************************/
//b24-b31(位定义)
//#define GPIO_MODE_INPUT      0x01000000
//#define GPIO_MODE_OUTPUT     0x02000000
//#define GPIO_MODE_IRQ        0x04000000
//#define GPIO_MODE_ANALOG     0x08000000
//	/* AF 0 */
//	#define GPIO_AF0_EVENTOUT     ((uint8_t)0x00U)  /*!< AF0: EVENTOUT Alternate Function mapping  */
//	#define GPIO_AF0_MCO          ((uint8_t)0x00U)  /*!< AF0: MCO Alternate Function mapping       */
//	#define GPIO_AF0_SPI1         ((uint8_t)0x00U)  /*!< AF0: SPI1 Alternate Function mapping      */
//	#define GPIO_AF0_SPI2         ((uint8_t)0x00U)  /*!< AF0: SPI2 Alternate Function mapping      */
//	#define GPIO_AF0_TIM15        ((uint8_t)0x00U)  /*!< AF0: TIM15 Alternate Function mapping     */
//	#define GPIO_AF0_TIM17        ((uint8_t)0x00U)  /*!< AF0: TIM17 Alternate Function mapping     */
//	#define GPIO_AF0_SWDIO        ((uint8_t)0x00U)  /*!< AF0: SWDIO Alternate Function mapping     */
//	#define GPIO_AF0_SWCLK        ((uint8_t)0x00U)  /*!< AF0: SWCLK Alternate Function mapping     */
//	#define GPIO_AF0_TIM14        ((uint8_t)0x00U)  /*!< AF0: TIM14 Alternate Function mapping     */
//	#define GPIO_AF0_USART1       ((uint8_t)0x00U)  /*!< AF0: USART1 Alternate Function mapping    */
//	#define GPIO_AF0_IR           ((uint8_t)0x00U)  /*!< AF0: IR Alternate Function mapping        */
//	#define GPIO_AF0_TIM3         ((uint8_t)0x00U)  /*!< AF0: TIM3 Alternate Function mapping      */
//	
//	/* AF 1 */
//	#define GPIO_AF1_TIM3         ((uint8_t)0x01U)  /*!< AF1: TIM3 Alternate Function mapping      */
//	#define GPIO_AF1_TIM15        ((uint8_t)0x01U)  /*!< AF1: TIM15 Alternate Function mapping     */
//	#define GPIO_AF1_USART1       ((uint8_t)0x01U)  /*!< AF1: USART1 Alternate Function mapping    */
//	#define GPIO_AF1_USART2       ((uint8_t)0x01U)  /*!< AF1: USART2 Alternate Function mapping    */
//	#define GPIO_AF1_EVENTOUT     ((uint8_t)0x01U)  /*!< AF1: EVENTOUT Alternate Function mapping  */
//	#define GPIO_AF1_I2C1         ((uint8_t)0x01U)  /*!< AF1: I2C1 Alternate Function mapping      */
//	#define GPIO_AF1_I2C2         ((uint8_t)0x01U)  /*!< AF1: I2C2 Alternate Function mapping      */
//	#define GPIO_AF1_IR           ((uint8_t)0x01U)  /*!< AF1: IR Alternate Function mapping        */
//	
//	/* AF 2 */
//	#define GPIO_AF2_TIM1         ((uint8_t)0x02U)  /*!< AF2: TIM1 Alternate Function mapping      */
//	#define GPIO_AF2_TIM16        ((uint8_t)0x02U)  /*!< AF2: TIM16 Alternate Function mapping     */
//	#define GPIO_AF2_TIM17        ((uint8_t)0x02U)  /*!< AF2: TIM17 Alternate Function mapping     */
//	#define GPIO_AF2_EVENTOUT     ((uint8_t)0x02U)  /*!< AF2: EVENTOUT Alternate Function mapping  */
//	
//	/* AF 3 */
//	#define GPIO_AF3_EVENTOUT     ((uint8_t)0x03U)  /*!< AF3: EVENTOUT Alternate Function mapping  */
//	#define GPIO_AF3_I2C1         ((uint8_t)0x03U)  /*!< AF3: I2C1 Alternate Function mapping      */
//	#define GPIO_AF3_TIM15        ((uint8_t)0x03U)  /*!< AF3: TIM15 Alternate Function mapping     */
//	
//	/* AF 4 */
//	#define GPIO_AF4_TIM14        ((uint8_t)0x04U)  /*!< AF4: TIM14 Alternate Function mapping     */
//	
//	/* AF 5 */
//	#define GPIO_AF5_TIM16        ((uint8_t)0x05U)  /*!< AF5: TIM16 Alternate Function mapping     */
//	#define GPIO_AF5_TIM17        ((uint8_t)0x05U)  /*!< AF5: TIM17 Alternate Function mapping     */
//	
//	/* AF 6 */
//	#define GPIO_AF6_EVENTOUT     ((uint8_t)0x06U)  /*!< AF6: EVENTOUT Alternate Function mapping  */

//b16-b23(位定义)
#define GPIO_MODE_AFMASK     0x00FF0000
#define GPIO_MODE_AF0        0x00000000
#define GPIO_MODE_AF1        0x00010000
#define GPIO_MODE_AF2        0x00020000 //GPIO_AF2_TIM1
#define GPIO_MODE_AF3        0x00030000
#define GPIO_MODE_AF4        0x00040000
#define GPIO_MODE_AF5        0x00050000
#define GPIO_MODE_AF6        0x00060000

#define GPIO_AF0        0
#define GPIO_AF1        1
#define GPIO_AF2        2
#define GPIO_AF3        3
#define GPIO_AF4        4
#define GPIO_AF5        5
#define GPIO_AF6        6


//b12-b15(位定义)
#define GPIO_SPEED_LOW       0x00000000
#define GPIO_SPEED_MEDIUM    0x00001000
#define GPIO_SPEED_HIGH      0x00002000

//b8-b11(位定义)
#define GPIO_MODE_NOPULL     0x00000000
#define GPIO_MODE_PULLUP     0x00000100
#define GPIO_MODE_PULLDOWN   0x00000200
#define GPIO_MODE_OPENDRAIN  0x00000400

//b0-b7(连续数值)
#define GPIO_MODE_FUNCMASK   0x000000FF
#define GPIO_MODE_FUNC_INPUT                   0x00000000   //GPIO_MODE_INPUT   
#define GPIO_MODE_FUNC0 GPIO_MODE_FUNC_INPUT
#define GPIO_MODE_FUNC_OUTPUT_PP               0x00000001   //GPIO_MODE_OUTPUT_PP
#define GPIO_MODE_FUNC1 GPIO_MODE_FUNC_OUTPUT_PP
#define GPIO_MODE_FUNC_OUTPUT_OD               0x00000002   //GPIO_MODE_OUTPUT_OD
#define GPIO_MODE_FUNC2 GPIO_MODE_FUNC_OUTPUT_OD
#define GPIO_MODE_FUNC_AF_PP                   0x00000003   //GPIO_MODE_AF_PP
#define GPIO_MODE_FUNC3 GPIO_MODE_FUNC_AF_PP
#define GPIO_MODE_FUNC_AF_OD                   0x00000004   //GPIO_MODE_AF_OD
#define GPIO_MODE_FUNC4 GPIO_MODE_FUNC_AF_OD
#define GPIO_MODE_FUNC_ANALOG                  0x00000005   //GPIO_MODE_ANALOG
#define GPIO_MODE_FUNC5 GPIO_MODE_FUNC_ANALOG
#define GPIO_MODE_FUNC_IT_RISING               0x00000006   //GPIO_MODE_IT_RISING
#define GPIO_MODE_FUNC6 GPIO_MODE_FUNC_IT_RISING
#define GPIO_MODE_FUNC_IT_FALLING              0x00000007   //GPIO_MODE_IT_FALLING
#define GPIO_MODE_FUNC7 GPIO_MODE_FUNC_IT_FALLING
#define GPIO_MODE_FUNC_IT_RISING_FALLING       0x00000008   //GPIO_MODE_IT_RISING_FALLING 
#define GPIO_MODE_FUNC8 GPIO_MODE_FUNC_IT_RISING_FALLING
#define GPIO_MODE_FUNC_EVT_RISING              0x00000009   //GPIO_MODE_EVT_RISING
#define GPIO_MODE_FUNC9 GPIO_MODE_FUNC_EVT_RISING
#define GPIO_MODE_FUNC_EVT_FALLING             0x0000000A   //GPIO_MODE_EVT_FALLING
#define GPIO_MODE_FUNCA GPIO_MODE_FUNC_EVT_FALLING
#define GPIO_MODE_FUNC_EVT_RISING_FALLING      0x0000000B   //GPIO_MODE_EVT_RISING_FALLING 
#define GPIO_MODE_FUNCB GPIO_MODE_FUNC_EVT_RISING_FALLING

#define GPIO_MODE_FUNCC      0x0000000C
#define GPIO_MODE_FUNCD      0x0000000D
#define GPIO_MODE_FUNCE      0x0000000E
#define GPIO_MODE_FUNCF      0x0000000F
#define GPIO_MODE_FUNC10     0x00000010
#define GPIO_MODE_FUNC11     0x00000011
#define GPIO_MODE_FUNC12     0x00000012
#define GPIO_MODE_FUNC13     0x00000013
#define GPIO_MODE_FUNC14     0x00000014
#define GPIO_MODE_FUNC15     0x00000015
#define GPIO_MODE_FUNC16     0x00000016
#define GPIO_MODE_FUNC17     0x00000017
#define GPIO_MODE_FUNC18     0x00000018
#define GPIO_MODE_FUNC1A     0x0000001A
#define GPIO_MODE_FUNC1B     0x0000001B
#define GPIO_MODE_FUNC1C     0x0000001C
#define GPIO_MODE_FUNC1D     0x0000001D
#define GPIO_MODE_FUNC1E     0x0000001E
#define GPIO_MODE_FUNC1F     0x0000001F

#define GPIOx_Type GPIO_TypeDef
#define UARTx_Type USART_TypeDef


/******************************************************************************
**通用输入输出IO口定义
**成员举例:P1.07
**0x1, 7,  (IOCON_FUNC1 | IOCON_MODE_INACT | IOCON_DIGMODE_EN), LPC_GPIO, 1
******************************************************************************/
typedef struct
{
    //
	uint32_t pingrp:3;                    //Pin group             //来自PINMUX_GRP_T成员
	uint32_t pinnum:5;                    //Pin number            //来自PINMUX_GRP_T成员
//	uint32_t type:8;                      //Function and mode     //来自PINMUX_GRP_T成员
	uint32_t modefunc:24;                 //Function and mode     //来自PINMUX_GRP_T成员
    //
    GPIOx_Type * gpio;                   //GPIO寄存器
    uint8_t analog;                      //0数字；1模拟输入输出
    uint8_t dir;                          //2:输出,1：功能io；0:输入
    uint8_t rvs;                         //高低电平翻转:0正序,1逆序
    uint8_t  ival;   //逻辑初始值:0(低),1(高)

}COMPORT;

/*****************************************************************************
**定义SPI端口结构体
******************************************************************************/
typedef struct
{
    COMPORT* portMISO;                  //SPI器件主入从出,输入
    COMPORT* portMOSI;                  //SPI器件主出从入
    COMPORT* portSCK;                   //SPI器件时钟线
    COMPORT* portPWR;                    //SPI器件电源脚
    COMPORT* portCS;                    //SPI器件片选线
    COMPORT* portWP;                    //SPI器件保护线
    COMPORT* portPWR2;                    //SPI器件电源脚
    COMPORT* portCS2;                   //SPI器件2片选线
    COMPORT* portWP2;                   //SPI器件2保护线
    COMPORT* portPWR3;                    //SPI器件电源脚
    COMPORT* portCS3;                   //SPI器件3片选线
    COMPORT* portWP3;                   //SPI器件3保护线
    
}SPIIO_PORTS;



/************************************************************************
**定义IIC端口结构体
*************************************************************************/
typedef struct
{
    COMPORT* portSDAOut;               //IIC器件数据线
    COMPORT* portSCK;               //IIC器件时钟线
    COMPORT* portSDAIn;               //IIC器件数据线
    
}IIC_PORTS;


/************************************************************************
**定义GPI端口结构体
*************************************************************************/
#ifndef __NO_SYS__
typedef struct
{
    COMPORT* cp;     //端口配置
    uint8_t     filter:1; //是否需要去抖
}GPI_PORTS;

#else
typedef struct
{
//	    COMPORT* cp;     //端口配置
	uint32_t pingrp:3;                    //Pin group             //来自PINMUX_GRP_T成员
	uint32_t pinnum:5;                    //Pin number            //来自PINMUX_GRP_T成员
	uint32_t type:8;                      //Function and mode     //来自PINMUX_GRP_T成员
	uint32_t modefunc:16;                 //Function and mode     //来自PINMUX_GRP_T成员
    //
    GPIOx_Type * gpio;                   //GPIO寄存器
    uint8_t analog;                      //0数字；1模拟输入输出
    uint8_t dir;                          //2:输出,1：功能io；0:输入
//	    uint8_t rvs;                         //高低电平翻转:0正序,1逆序
//	    uint8  ival:1;   //逻辑初始值:0(低),1(高)

    uint8_t     filter:1; //是否需要去抖
    //uint8  en:1;
    uint8_t     handleen:1;
    uint8_t     handleno:6;//io中断号
    uint8_t     edge;
}GPI_PORTS;

#endif
/************************************************************************
**定义GPO端口结构体
*************************************************************************/
typedef struct
{
    COMPORT* cp;     //端口配置
    uint8  rvs:1;    //高低电平翻转(用于逻辑电平与物理电平的对应):0正序(true高false低),1逆序(true低false高)
    uint8  ival:1;   //逻辑初始值:0(true),1(false)
    //uint8  en:1;
    
}GPO_PORTS;
/******************************************************************************
**AD口定义
**成员举例:P1.31
**0x1, 31,  (IOCON_FUNC3 | IOCON_MODE_INACT ), LPC_ADC, 
******************************************************************************/
typedef struct
{
    //
	uint32 pingrp:3;                    //Pin group             //来自PINMUX_GRP_T成员
	uint32 pinnum:5;                    //Pin number            //来自PINMUX_GRP_T成员
//		uint32 modefunc:24;                 //Function and mode     //来自PINMUX_GRP_T成员
	//uint32_t type:8;                      //Function and mode     //来自PINMUX_GRP_T成员
	uint32_t modefunc:24;                 //Function and mode     //来自PINMUX_GRP_T成员

    //
    //LPC_ADC_T* adc;                     //ADC寄存器
    
    GPIOx_Type * gpio;                   //GPIO寄存器
    uint32 vref;                        //参考电压
    uint16_t  vnum;                        //分频数
    uint8  chn;                         //通道号
    uint8_t ctrlen;
    
    uint8_t ctrlno;
}ADPORT;



/* GPIO配置函数参数宏定义 */
//IO方向配置 
// 0 = 输入 
// 1 = 输出
#define GPIO_DIR_IN	0
#define GPIO_DIR_OUT	1



//IO输入口配置 
//type 0 = 普通 
//type 1 = 上拉
#define IN_NORMAL	0
#define IN_PULLUP	1

//IO输出口配置 
//type 0 = 普通 
//type 1 = OD
#define OUT_PUSHPULL	0
#define OUT_OPENDRAIN	1
#define OUT_PULLUP			2
#define OUT_OPENDRAIN_PULLUP	3

//IO数字特殊功能口 
//type 0 = 普通 
//type 1 = OD (OD功能仅部分特殊功能支持)
//type 2 = 普通+上拉 
//type 3 = OD+上拉
#define ALTFUN_NORMAL			0
#define ALTFUN_OPENDRAIN		1
#define ALTFUN_PULLUP			2
#define ALTFUN_OPENDRAIN_PULLUP	3


#define ANALOG_DISENABLE   0
#define ANALOG_ENENABLE   1


#endif

