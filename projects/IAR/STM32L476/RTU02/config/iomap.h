/****************************************Copyright (c)*************************
**                               	________科技有限公司
**                                     		开发部
**
**
**--------------文件信息-------------------------------------------------------
**文   件   名: iomap.h
**创   建   人: yzy
**最后修改日期: 2010年12月1日
**描        述: 硬件IO端口分配图
**注        意:
**--------------历史版本信息---------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2010年12月1日
** 描　述: 原始版本
******************************************************************************/

#ifndef _IOMAP_H_
#define _IOMAP_H_


#define GPIO_PIOR_BITS     0x05//port0和port2重定向
#define GPIO_ADPC_BITS     0x01//模拟输入或者端口的数字输入/ 输出的寄存器


/******************************************************************************
**LED指示灯输出端口结构体信息
******************************************************************************/


#ifdef EXT_DEV_LED                          //LED端口定义

    const COMPORT gs_LedRun       = {0x1, 2,  1, (IOCON_FUNC0 ),    1};

    
    
    /******************************************************************************
    **LED指示灯输出端口信息
    ******************************************************************************/
    const GPO_PORTS gs_LedPort[] = 
    {
        {(COMPORT*)&gs_LedRun,     false, 0},     //true:低电平点亮
    };

    #define LED_NUM (sizeof(gs_LedPort) / sizeof(GPO_PORTS))

#endif                                      //#ifdef EXT_DEV_LED

/******************************************************************************
**LED/Beep的名称枚举
******************************************************************************/
typedef enum
{
    BEEP_WARN = 0,                        //告警蜂鸣器
}LedNo;


/******************************************************************************
**GPO输出端口枚举定义
******************************************************************************/
typedef enum
{
    GPO_ADC_ONOFF,
    GPO_MODEM_PWR,
    GPO_MODEM_ONOFF,
    GPO_VALVE_SLEEP,
    GPO_VALVE_OPEN,
    GPO_VALVE_CLOSE,    
    GPO_ESAM_PWR,    
    GPO_CARD_PWR,
    GPO_CARD_RST,
    GPO_ESAM_RST,     
}GPOENUM;

/******************************************************************************
**GPO端口输出结构体信息
******************************************************************************/

#ifdef EXT_DEV_GPO                      //GPO的端口定义

    const COMPORT gs_GpoAdcCtl      = {0x2, 1,  1, (IOCON_FUNC0 ),    1};
    const COMPORT gs_GpoModemPwr    = {12,  7,  1, (IOCON_FUNC0 ),    1};
    const COMPORT gs_GpoModemOnOff  = {12,  5,  1, (IOCON_FUNC0 ),    1};
    const COMPORT gs_GpoValveSleep  = {0x0, 7,  0, (IOCON_FUNC0 ),    1};
    const COMPORT gs_GpoValveOpen   = {0x0, 6,  1, (IOCON_FUNC0 ),    1};
    const COMPORT gs_GpoValveClose  = {0x0, 5,  1, (IOCON_FUNC0 ),    1};    
    const COMPORT gs_GpoEsamPwr     = {0x1, 5,  1, (IOCON_FUNC0 ),    1};
    const COMPORT gs_GpoCardPwr     = {0x6, 1,  1, (IOCON_FUNC0 ),    1};
    const COMPORT gs_GpoCardRst     = {12,  6,  1, (IOCON_FUNC0 ),    1};
    const COMPORT gs_GpoEsamRst     = {0x1, 4,  1, (IOCON_FUNC0 ),    1};
    
    /******************************************************************************
    **GPO输出端口定义
    ******************************************************************************/
    const GPO_PORTS gs_GpoPort[] = 
    {
        {(COMPORT*)&gs_GpoAdcCtl,       false, false},      //adc采样开关
        {(COMPORT*)&gs_GpoModemPwr,     false, false},      //gprs电源脚
        {(COMPORT*)&gs_GpoModemOnOff,   false, false},      //gprs Power Key脚
        {(COMPORT*)&gs_GpoValveSleep,   false, false},      //阀门芯片休眠 低电平休眠
        {(COMPORT*)&gs_GpoValveOpen,    false, false},      //阀门芯片IN1
        {(COMPORT*)&gs_GpoValveClose,   false, false},      //阀门芯片IN2
        {(COMPORT*)&gs_GpoEsamPwr,      false, false},      //esam电源开关
        {(COMPORT*)&gs_GpoCardPwr,      false, false},      //卡电源开关
        {(COMPORT*)&gs_GpoCardRst,      false, false},      //卡复位开关
        {(COMPORT*)&gs_GpoEsamRst,      false, false},      //Esam复位开关
        
    };
    
    #define GPO_NUM (sizeof(gs_GpoPort) / sizeof(GPO_PORTS))

#endif


/******************************************************************************
**慢速输入口配置
******************************************************************************/
//#define LGPI_KEY_NUM    5               //数值小于 LGPI_PORT_NUM
#define LGPI_KEY_NUM    3


#define SYS_LGPI_GATE    2               //按键扫描门限
#define SYS_LGPI_LAST    15              //按键长按门限




/******************************************************************************
**快速输入口配置
******************************************************************************/
#define SYS_FGPI_GATE    3               //快速输入口扫描门限
typedef enum
{
    GPI_KEY1,
    GPI_KEY2,
    GPI_CARD,
    GPI_HALL1,
    GPI_HALL2,
    GPI_CARD_SDA,
    GPI_ESAM_SDA,    
}GPIENUM;


/******************************************************************************
**GPI端口输入结构体信息
******************************************************************************/
#ifdef EXT_DEV_GPI                        //GPI的端口定义

    /******************************************************************************
    **LGPI端口信息
    ******************************************************************************/
    const COMPORT gs_GpiKey1      = {13, 7,  1, (IOCON_FUNC0 ),    0};//key1
    const COMPORT gs_GpiKey2      = {0x3, 1,  1, (IOCON_FUNC0 ),    0};//key2
    const COMPORT gs_GpiCardInt   = {0x3, 3,  1, (IOCON_FUNC0 ),    0};//key3
    const COMPORT gs_GpiHall1      = {5, 7,  1, (IOCON_FUNC0 ),    0};//Hall1
    const COMPORT gs_GpiHall2      = {0, 2,  1, (IOCON_FUNC0 ),    0};//Hall2
    const COMPORT gs_GpiCardSda    = {3, 4,  1, (IOCON_FUNC0 ),    0};//Hall2
    const COMPORT gs_GpiEsamSda    = {1, 7,  1, (IOCON_FUNC0 ),    0};//Hall2
    
    const GPI_PORTS gs_LGPIPort[] = 
    {
        {(COMPORT*)&gs_GpiKey1,       false, 0, 0},      //key1
        {(COMPORT*)&gs_GpiKey2,       false, 0, 0},      //key2
        {(COMPORT*)&gs_GpiCardInt,    false, 0, 0},      //key3     
        {(COMPORT*)&gs_GpiHall1,      false, 0, 0},      //key1
        {(COMPORT*)&gs_GpiHall2,      false, 0, 0},      //key2
        {(COMPORT*)&gs_GpiCardSda,    false, 0, 0},      //cpu卡sda脚
        {(COMPORT*)&gs_GpiEsamSda,    false, 0, 0},      //Esam sda脚
//	        {0x3, 1,  1, (IOCON_FUNC0 ),    0},      //
    };

    const uint8 LGPI_PORT_NUM = (sizeof(gs_LGPIPort) / sizeof(GPI_PORTS));
    
     /******************************************************************************
    **FGPI端口信息
    ******************************************************************************/
    
//	    const GPI_PORTS gs_FGPIPort[] = 
//	    {
//	        {(COMPORT*)&gs_GpiHall1,       false, 0, 0},      //key1
//	    
//	    };
//	    
//	    const uint8 FGPI_PORT_NUM = (sizeof(gs_FGPIPort) / sizeof(COMPORT));
    
#else
    
    /******************************************************************************
    **输入口状态位宏定义
    ******************************************************************************/
    #define LGPI_STT_YX1        0x00000001
    
    //
    #define FGPI_STT_ENG        0x00000001         //置位表示供电正常
    
    extern const COMPORT gs_LGPIPort[];
    extern const uint8 LGPI_PORT_NUM;
    
#endif       







/******************************************************************************
**APP LEVEL FLASH IO PORT DEFINATION
******************************************************************************/
#ifdef EXT_MXFLASH
    
    const COMPORT gs_FlashSpiMISO = {0x2, 3,  1, (IOCON_FUNC0 ),    0};
    const COMPORT gs_FlashSpiMOSI = {0x1, 1,  1, (IOCON_FUNC0 ),    1};
    const COMPORT gs_FlashSpiSCK  = {0x1, 0,  1, (IOCON_FUNC0 ),    1};
    const COMPORT gs_FlashSpiCS   = {0x2, 2,  1, (IOCON_FUNC0 ),    1};
    const COMPORT gs_FlashSpiPWR  = {13,  0,  1, (IOCON_FUNC0 ),    1};
    const COMPORT gs_FlashSpiWP   = {0x2, 4,  1, (IOCON_FUNC0 ),    1};
    
    const SPIIO_PORTS gs_FlashSpiPort = 
    {
        (COMPORT*)&gs_FlashSpiMISO,       //MISO,SPI主入从出
        (COMPORT*)&gs_FlashSpiMOSI,       //MOSI,SPI主出从入
        (COMPORT*)&gs_FlashSpiSCK,        //SCK,SPI时钟线
        (COMPORT*)&gs_FlashSpiPWR,        //电源脚        
        (COMPORT*)&gs_FlashSpiCS,         //FLA,FLASH片选
        (COMPORT*)&gs_FlashSpiWP,         //写保护
    };
    
#endif                                  //#ifdef EXT_MXFLASH





/******************************************************************************
//	**硬时钟的I2C口操作宏
//	******************************************************************************/
//	#ifdef EXT_IIC_HRTC
//	
//	    const COMPORT gs_HrtcSDA = {0x0, 27,  (IOCON_FUNC0 | IOCON_MODE_INACT), 1};//分时输入和输出.默认输出
//	    const COMPORT gs_HrtcSCK = {0x0, 28,  (IOCON_FUNC0 | IOCON_MODE_INACT), 1};
//	
//	    const IIC_PORTS gs_HrtcPort = 
//	    {
//	        (COMPORT*)&gs_HrtcSDA,
//	        (COMPORT*)&gs_HrtcSCK,
//	    };
//	#else
//	    
//	    extern const IIC_PORTS gs_HrtcPort;
//	    
//	#endif                                  //#ifdef EXT_IIC_HRTC
    
/******************************************************************************
**eeprom的I2C口操作宏
******************************************************************************/
#ifdef EXT_IIC_EEPROM

    const COMPORT gs_EepromSDA = {0x1, 1,  1, (IOCON_FUNC0), 1};//分时输入和输出.默认输出
    const COMPORT gs_EepromSCK = {0x1, 0,  1, (IOCON_FUNC0), 1};
    const COMPORT gs_EepromPWR = {13, 0,  1, (IOCON_FUNC0), 1};
    const COMPORT gs_EepromADDR1 = {0x2, 2,  1, (IOCON_FUNC0), 1};
    const COMPORT gs_EepromADDR2 = {0x2, 3,  1, (IOCON_FUNC0), 1};
    const COMPORT gs_EepromADDR3 = {0x2, 4,  1, (IOCON_FUNC0), 1};

    const IIC_PORTS gs_EepromPort = 
    {
        (COMPORT*)&gs_EepromSDA,
        (COMPORT*)&gs_EepromSCK,
        (COMPORT*)&gs_EepromPWR,
        (COMPORT*)&gs_EepromADDR1,
        (COMPORT*)&gs_EepromADDR2,
        (COMPORT*)&gs_EepromADDR3,
        
    };
#else
    
    extern const IIC_PORTS gs_EepromPorts;
    
#endif                                  //#ifdef EXT_IIC_HRTC


#ifdef EXT_DEV_LCD                      //LCD的端口定义
    /******************************************************************************
    **LCD端口信息
    ******************************************************************************/
    const COMPORT gs_LCDPort[] = 
    {
        {0x5, 0x0,  0, (IOCON_MODE_OUT_GEN),  0},      //seg4
        {0x5, 0x1,  0, (IOCON_MODE_OUT_GEN),  0},      //
        {0x5, 0x2,  0, (IOCON_MODE_OUT_GEN),  0},      //
        {0x5, 0x3,  0, (IOCON_MODE_OUT_GEN),  0},      //
        {0x5, 0x4,  0, (IOCON_MODE_OUT_GEN),  0},      //
        {0x5, 0x5,  0, (IOCON_MODE_OUT_GEN),  0},      //
        {0x5, 0x6,  0, (IOCON_MODE_OUT_GEN),  0},      //
        
        {0x7, 0x0,  0, (IOCON_MODE_OUT_GEN),  0},      //
        {0x7, 0x1,  0, (IOCON_MODE_OUT_GEN),  0},      //
        {0x7, 0x2,  0, (IOCON_MODE_OUT_GEN),  0},      //
        {0x7, 0x3,  0, (IOCON_MODE_OUT_GEN),  0},      //
        {0x7, 0x4,  0, (IOCON_MODE_OUT_GEN),  0},      //
        {0x7, 0x5,  0, (IOCON_MODE_OUT_GEN),  0},      //
        {0x7, 0x6,  0, (IOCON_MODE_OUT_GEN),  0},      //
        {0x7, 0x7,  0, (IOCON_MODE_OUT_GEN),  0},      //
        
        {0x3, 0x0,  0, (IOCON_MODE_OUT_GEN),  0},      //
        {0x3, 0x2,  0, (IOCON_MODE_OUT_GEN),  0},      //

    };

    const unsigned char guc_SEG_Tbl[] = 
    {
        0,1,2,3,            //S0-3
        4,5,6,7,            //S4-7
        8,9,10,12,          //S8-11
        13,14,15,16,        //S12-15
        17,18,19,20,        //S16-19
        22,                 //S20
    };

    
    #define LCD_PORT_NUM  (sizeof(gs_LCDPort) / sizeof(COMPORT))
#endif


//	
//	
//	
//	/******************************************************************************
//	**LED指示灯输出端口结构体信息
//	******************************************************************************/
//	
//	
//	#ifdef EXT_DEV_LED                          //LED端口定义
//	
//	    const COMPORT gs_LedRun       = {0x0, 26,  (IOCON_FUNC0 | IOCON_MODE_INACT),  LPC_GPIO,  1};
//	    const COMPORT gs_LedTxd       = {0x2, 6,   (IOCON_FUNC0 | IOCON_MODE_INACT),  LPC_GPIO,  1};
//	    const COMPORT gs_LedRxd       = {0x2, 7,   (IOCON_FUNC0 | IOCON_MODE_INACT),  LPC_GPIO,  1};
//	
//	    
//	    
//	    /******************************************************************************
//	    **LED指示灯输出端口信息
//	    ******************************************************************************/
//	    const GPO_PORTS gs_LedPort[] = 
//	    {
//	        {(COMPORT*)&gs_LedRun,     false},     //true:低电平点亮
//	        {(COMPORT*)&gs_LedTxd,     false},
//	        {(COMPORT*)&gs_LedRxd,     false},
//	
//	    };
//	
//	    #define LED_NUM (sizeof(gs_LedPort) / sizeof(GPO_PORTS))
//	
//	#endif                                      //#ifdef EXT_DEV_LED
//	
//	
//	
//	
//	
//	
//	
//	
//	
//	
//	
//	
//	
//	
//	
//	
//	/******************************************************************************
//	**慢速输入口配置
//	******************************************************************************/
//	#define LGPI_KEY_NUM    5               //数值小于 LGPI_PORT_NUM
//	
//	
//	#define SYS_LGPI_GATE    1               //按键扫描门限
//	#define SYS_LGPI_LAST    20              //按键长按门限
//	
//	
//	
//	
//	/******************************************************************************
//	**快速输入口配置
//	******************************************************************************/
//	#define SYS_FGPI_GATE    3               //快速输入口扫描门限
//	
//	
//	
//	
//	
//	
//	
//	
//	
//	
//	
//	
//	
//	
//	
//	
//	/******************************************************************************
//	**GPO端口输出结构体信息
//	******************************************************************************/
//	
//	#ifdef EXT_DEV_GPO                      //GPO的端口定义
//	
//	    const COMPORT gs_GpoEthRst      = {0x0, 4,  (IOCON_FUNC0 | IOCON_MODE_INACT),  LPC_GPIO,  1};
//	    
//	    /******************************************************************************
//	    **GPO输出端口定义
//	    ******************************************************************************/
//	    const GPO_PORTS gs_GpoPort[] = 
//	    {
//	        {(COMPORT*)&gs_GpoEthRst,       false},      //以太网芯片复位
//	    };
//	    
//	    #define GPO_NUM (sizeof(gs_GpoPort) / sizeof(GPO_PORTS))
//	
//	#endif                                  //#ifdef EXT_DEV_GPO
//	
//	
//	
//	/******************************************************************************
//	**GPO输出端口枚举定义
//	******************************************************************************/
//	typedef enum
//	{
//	    GPO_ENET_RST,
//	}GPOENUM;
//	
//	
//	
//	/******************************************************************************
//	**GPI端口输入结构体信息
//	******************************************************************************/
//	#ifdef EXT_DEV_GPI                        //GPI的端口定义
//	
//	    /******************************************************************************
//	    **LGPI端口信息
//	    ******************************************************************************/
//	    const COMPORT gs_LGPIPort[] = 
//	    {
//	        {0x1, 1,  (IOCON_FUNC0 | IOCON_MODE_INACT),  LPC_GPIO, 0},      //G610模块电压检测，高电平表示正常启动
//	    };
//	    
//	    const uint8 LGPI_PORT_NUM = (sizeof(gs_LGPIPort) / sizeof(COMPORT));
//	    
//	     /******************************************************************************
//	    **FGPI端口信息
//	    ******************************************************************************/
//	    const COMPORT gs_FGPIPort[] = 
//	    {
//	        {0x1, 4,  (IOCON_FUNC0 | IOCON_MODE_INACT), LPC_GPIO, 0},      //停电检测，高电平表示正常，低表示停电
//	    };
//	    
//	    const uint8 FGPI_PORT_NUM = (sizeof(gs_FGPIPort) / sizeof(COMPORT));
//	    
//	#else
//	    
//	    /******************************************************************************
//	    **输入口状态位宏定义
//	    ******************************************************************************/
//	    #define LGPI_STT_YX1        0x00000001
//	    
//	    //
//	    #define FGPI_STT_ENG        0x00000001         //置位表示供电正常
//	    
//	    extern const COMPORT gs_LGPIPort[];
//	    extern const uint8 LGPI_PORT_NUM;
//	    
//	#endif       
//	
//	
//	
//	
//	
//	
//	/******************************************************************************
//	**外部看门狗管脚定义
//	******************************************************************************/
//	#if SYS_EXTWDT_EN > 0
//	#ifdef _USR_MAIN                        //WDT的端口定义
//	    const COMPORT gs_ExtWdtPort = {0x0, 22,  (IOCON_FUNC0 | IOCON_MODE_PULLUP),  LPC_GPIO,  1};
//	#else
//	    extern const COMPORT gs_ExtWdtPort;
//	#endif
//	#endif





#endif





