#ifndef __STM32L476RG_BOARD_H
#define __STM32L476RG_BOARD_H


#define HARDWARE_REVISION   "V1.0"
#define MODEL               "STM32L4"

#ifdef BOOTLOADER
#define STDIO_UART 0
#define STDIO_UART_BUADRATE 115200
#else
#define STDIO_UART 0
#define STDIO_UART_BUADRATE 115200
#endif

typedef enum{
    PORT_UART_STD,
    PORT_UART_AT,
//	    PORT_UART_PST,
    NETP_PORT_NO,                        //实际串口编号3
    NETP2_PORT_NO,                        //实际串口编号
    NETP3_PORT_NO,                        //实际串口编号
    NETP4_PORT_NO,                        //实际串口编号    
    PORT_UART_SIZE,
    PORT_UART_INVALID = 255,
}PORT_UART_TYPE;

#define PORT_UART_BLE   PORT_UART_STD
#define PST_CHN_TCP     PORT_UART_AT
#define PST_CHN_SET     PORT_UART_STD
#define PST_CHN_NETP    NETP_PORT_NO
#define PORT_UART_PST   PORT_UART_STD
#define SETP_PORT_NO    NETP_PORT_NO

#define MODEM_ONOFF     HAL_GPIO_6
#define MODEM_PWR       HAL_GPIOE_08
#define RS485_PWR       HAL_GPIOE_12
#define BLE_PWR         HAL_GPIOD_07
#define BLE_MODE        HAL_GPIO_20
#define BLE_UART_CTL        HAL_GPIOE_02

#define GPI_Key1        HAL_GPIO_4
#define GPI_Key2        HAL_GPIO_5

#define GPI_BLE_LINK    HAL_GPIO_22
#define GPI_EXT_PWR    HAL_GPIO_37

#define SPI_FLASH_MISO  HAL_GPIO_30
#define SPI_FLASH_MOSI  HAL_GPIO_31
#define SPI_FLASH_SCK   HAL_GPIO_29
#define SPI_FLASH_CS    HAL_GPIO_28
#define SPI_FLASH_WP    HAL_GPIOD_08
#define SPI_FLASH_PWR   HAL_GPIO_43


//	#define LCD_NSS HAL_GPIO_4
//	#define LCD_SCK HAL_GPIO_5
//	#define LCD_TX HAL_GPIO_7
//	#define SW_WIFI HAL_GPIO_16
//	#define LCD_DCX HAL_GPIO_17
//	#define LED_GS HAL_GPIO_18
//	#define AUDIO_EN HAL_GPIO_8
//	#define SW_FUNC_A HAL_GPIO_11
#define LED_RUN HAL_GPIO_8
#define LED_MBUS HAL_GPIO_39
#define LED_GPRS HAL_GPIO_41
#define LED_ERR HAL_GPIO_38

//	#define WIFI_RST HAL_GPIO_20
//	#define LED_ALS HAL_GPIO_21
//	#define WIFI_TX HAL_GPIO_22
//	#define WIFI_RX HAL_GPIO_23
//	#define ALS_INT HAL_GPIO_24
//	#define WIFI_WU HAL_GPIO_25


typedef enum {
	GPIO_MODEM_ONOFF,
	GPIO_MODEM_PWR,
	GPO_485_PWR,
	GPO_BLE_PWR,
	GPO_BLE_MODE,
	GPO_BLE_UART_CTL,
//		GPIO_LCD_DCX,
//		GPIO_LCD_PWR,
//		GPIO_LCD_RST,
//		GPIO_LED_ALS,
//		GPIO_LED_GS,
//		GPIO_LED_HTS,
//		GPIO_LED_PS,
//		GPIO_SW_FUNC_A,
//		GPIO_SW_FUNC_B,
//		GPIO_SW_WIFI,
//		GPIO_WIFI_RST,
//		GPIO_WIFI_WU,
//		MAX_GPIO_NUM
    GPO_NUM,
} BOARD_GPIO, GPOENUM;
    
typedef struct _led_dev_priv
{
    uint8_t * set;
    uint8_t * rvs;
}led_dev_priv;

typedef enum {
	GPIO_LED_RUN,
	GPIO_LED_485,    
	GPIO_LED_GPRS,   	
	GPIO_LED_ERR,   	
	
    LED_NUM,
} LedNo;

#define FGPI_STT_ENG        0x00000008         //置位表示供电正常

/******************************************************************************
**快速输入口配置
******************************************************************************/
#define SYS_FGPI_GATE    3               //快速输入口扫描门限
#define SYS_LGPI_GATE    2               //按键扫描门限
#define SYS_LGPI_LAST    15              //按键长按门限

typedef enum
{
    KEY1,
//	    GPI_CARD,
//	    GPI_HALL1,
//	    GPI_HALL2,
//	    GPI_CARD_SDA,
//	    GPI_ESAM_SDA, 
    BLE_LINK,
    KEY2,
    EXT_PWR,

    GPI_NUM,
//	    LGPI_KEY_NUM,
}GPIENUM;

#define LGPI_KEY_NUM 4


/******************************************************************************
**AD口定义
**成员举例:P1.31
**0x1, 31,  (IOCON_FUNC3 | IOCON_MODE_INACT ), LPC_ADC, 
******************************************************************************/
typedef struct
{
    //
//		uint32 pingrp:4;                    //Pin group             //来自PINMUX_GRP_T成员
//		uint32 pinnum:8;                    //Pin number            //来自PINMUX_GRP_T成员
//		uint32 pinseg:2;                    //pin pfseg 1:通用io, 0:段寄存器
//		uint32 modefunc:18;                 //Function and mode     //来自PINMUX_GRP_T成员
    //
    uint16_t port;
    GPIO_TypeDef * gpio;
    ADC_TypeDef* adc;                     //ADC寄存器
   // uint8_t adcChan;
    uint32_t channel;
    uint16_t vref;                        //参考电压
    uint8_t  vnum;                        //分频数
//	    uint8_t  chn;                         //通道号

//	    uint8_t opt : 2;//0:一次采样//1:持续采样
//	    uint8_t resolution : 2;//0:8bit//1:10bit
//	    uint8_t VREFU : 2;//0:VDD//1:
//	    uint8_t VREFD : 2;//0:VSS//1: 
    
}ADItem;

typedef struct
{
    ADC_HandleTypeDef * adhandle;
    uint8_t chnum;
    uint8_t choffset;
    ADItem * item;
}ADPORT;


#define AD_NUM      3      //AD个数
#define AD_CONVERSION   30
#define AD_ID_CHECKVOL      0       //      检测电池的电压
#define AD_ID_AI0      1       //      检测电池的电压
#define AD_ID_AI1      2       //      检测电池的电压

void SYS_FeedDOG();

#endif
