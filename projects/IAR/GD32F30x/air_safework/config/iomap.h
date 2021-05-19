/****************************************Copyright (c)*************************
**                               	________�Ƽ����޹�˾
**                                     		������
**
**
**--------------�ļ���Ϣ-------------------------------------------------------
**��   ��   ��: iomap.h
**��   ��   ��: yzy
**����޸�����: 2010��12��1��
**��        ��: Ӳ��IO�˿ڷ���ͼ
**ע        ��:
**--------------��ʷ�汾��Ϣ---------------------------------------------------
** ������: yzy
** ��  ��: v1.0
** �ա���: 2010��12��1��
** �衡��: ԭʼ�汾
******************************************************************************/

#ifndef _IOMAP_H_
#define _IOMAP_H_


#define GPIO_PIOR_BITS     0x05//port0��port2�ض���
#define GPIO_ADPC_BITS     0x01//ģ��������߶˿ڵ���������/ ����ļĴ���


/******************************************************************************
**LEDָʾ������˿ڽṹ����Ϣ
******************************************************************************/


#ifdef EXT_DEV_LED                          //LED�˿ڶ���

    const COMPORT gs_LedRun       = {0x1, 2,  1, (IOCON_FUNC0 ),    1};

    
    
    /******************************************************************************
    **LEDָʾ������˿���Ϣ
    ******************************************************************************/
    const GPO_PORTS gs_LedPort[] = 
    {
        {(COMPORT*)&gs_LedRun,     false, 0},     //true:�͵�ƽ����
    };

    #define LED_NUM (sizeof(gs_LedPort) / sizeof(GPO_PORTS))

#endif                                      //#ifdef EXT_DEV_LED

/******************************************************************************
**LED/Beep������ö��
******************************************************************************/
typedef enum
{
    BEEP_WARN = 0,                        //�澯������
}LedNo;


/******************************************************************************
**GPO����˿�ö�ٶ���
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
**GPO�˿�����ṹ����Ϣ
******************************************************************************/

#ifdef EXT_DEV_GPO                      //GPO�Ķ˿ڶ���

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
    **GPO����˿ڶ���
    ******************************************************************************/
    const GPO_PORTS gs_GpoPort[] = 
    {
        {(COMPORT*)&gs_GpoAdcCtl,       false, false},      //adc��������
        {(COMPORT*)&gs_GpoModemPwr,     false, false},      //gprs��Դ��
        {(COMPORT*)&gs_GpoModemOnOff,   false, false},      //gprs Power Key��
        {(COMPORT*)&gs_GpoValveSleep,   false, false},      //����оƬ���� �͵�ƽ����
        {(COMPORT*)&gs_GpoValveOpen,    false, false},      //����оƬIN1
        {(COMPORT*)&gs_GpoValveClose,   false, false},      //����оƬIN2
        {(COMPORT*)&gs_GpoEsamPwr,      false, false},      //esam��Դ����
        {(COMPORT*)&gs_GpoCardPwr,      false, false},      //����Դ����
        {(COMPORT*)&gs_GpoCardRst,      false, false},      //����λ����
        {(COMPORT*)&gs_GpoEsamRst,      false, false},      //Esam��λ����
        
    };
    
    #define GPO_NUM (sizeof(gs_GpoPort) / sizeof(GPO_PORTS))

#endif


/******************************************************************************
**�������������
******************************************************************************/
//#define LGPI_KEY_NUM    5               //��ֵС�� LGPI_PORT_NUM
#define LGPI_KEY_NUM    3


#define SYS_LGPI_GATE    2               //����ɨ������
#define SYS_LGPI_LAST    15              //������������




/******************************************************************************
**�������������
******************************************************************************/
#define SYS_FGPI_GATE    3               //���������ɨ������
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
**GPI�˿�����ṹ����Ϣ
******************************************************************************/
#ifdef EXT_DEV_GPI                        //GPI�Ķ˿ڶ���

    /******************************************************************************
    **LGPI�˿���Ϣ
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
        {(COMPORT*)&gs_GpiCardSda,    false, 0, 0},      //cpu��sda��
        {(COMPORT*)&gs_GpiEsamSda,    false, 0, 0},      //Esam sda��
//	        {0x3, 1,  1, (IOCON_FUNC0 ),    0},      //
    };

    const uint8 LGPI_PORT_NUM = (sizeof(gs_LGPIPort) / sizeof(GPI_PORTS));
    
     /******************************************************************************
    **FGPI�˿���Ϣ
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
    **�����״̬λ�궨��
    ******************************************************************************/
    #define LGPI_STT_YX1        0x00000001
    
    //
    #define FGPI_STT_ENG        0x00000001         //��λ��ʾ��������
    
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
        (COMPORT*)&gs_FlashSpiMISO,       //MISO,SPI����ӳ�
        (COMPORT*)&gs_FlashSpiMOSI,       //MOSI,SPI��������
        (COMPORT*)&gs_FlashSpiSCK,        //SCK,SPIʱ����
        (COMPORT*)&gs_FlashSpiPWR,        //��Դ��        
        (COMPORT*)&gs_FlashSpiCS,         //FLA,FLASHƬѡ
        (COMPORT*)&gs_FlashSpiWP,         //д����
    };
    
#endif                                  //#ifdef EXT_MXFLASH





/******************************************************************************
//	**Ӳʱ�ӵ�I2C�ڲ�����
//	******************************************************************************/
//	#ifdef EXT_IIC_HRTC
//	
//	    const COMPORT gs_HrtcSDA = {0x0, 27,  (IOCON_FUNC0 | IOCON_MODE_INACT), 1};//��ʱ��������.Ĭ�����
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
**eeprom��I2C�ڲ�����
******************************************************************************/
#ifdef EXT_IIC_EEPROM

    const COMPORT gs_EepromSDA = {0x1, 1,  1, (IOCON_FUNC0), 1};//��ʱ��������.Ĭ�����
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


#ifdef EXT_DEV_LCD                      //LCD�Ķ˿ڶ���
    /******************************************************************************
    **LCD�˿���Ϣ
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
//	**LEDָʾ������˿ڽṹ����Ϣ
//	******************************************************************************/
//	
//	
//	#ifdef EXT_DEV_LED                          //LED�˿ڶ���
//	
//	    const COMPORT gs_LedRun       = {0x0, 26,  (IOCON_FUNC0 | IOCON_MODE_INACT),  LPC_GPIO,  1};
//	    const COMPORT gs_LedTxd       = {0x2, 6,   (IOCON_FUNC0 | IOCON_MODE_INACT),  LPC_GPIO,  1};
//	    const COMPORT gs_LedRxd       = {0x2, 7,   (IOCON_FUNC0 | IOCON_MODE_INACT),  LPC_GPIO,  1};
//	
//	    
//	    
//	    /******************************************************************************
//	    **LEDָʾ������˿���Ϣ
//	    ******************************************************************************/
//	    const GPO_PORTS gs_LedPort[] = 
//	    {
//	        {(COMPORT*)&gs_LedRun,     false},     //true:�͵�ƽ����
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
//	**�������������
//	******************************************************************************/
//	#define LGPI_KEY_NUM    5               //��ֵС�� LGPI_PORT_NUM
//	
//	
//	#define SYS_LGPI_GATE    1               //����ɨ������
//	#define SYS_LGPI_LAST    20              //������������
//	
//	
//	
//	
//	/******************************************************************************
//	**�������������
//	******************************************************************************/
//	#define SYS_FGPI_GATE    3               //���������ɨ������
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
//	**GPO�˿�����ṹ����Ϣ
//	******************************************************************************/
//	
//	#ifdef EXT_DEV_GPO                      //GPO�Ķ˿ڶ���
//	
//	    const COMPORT gs_GpoEthRst      = {0x0, 4,  (IOCON_FUNC0 | IOCON_MODE_INACT),  LPC_GPIO,  1};
//	    
//	    /******************************************************************************
//	    **GPO����˿ڶ���
//	    ******************************************************************************/
//	    const GPO_PORTS gs_GpoPort[] = 
//	    {
//	        {(COMPORT*)&gs_GpoEthRst,       false},      //��̫��оƬ��λ
//	    };
//	    
//	    #define GPO_NUM (sizeof(gs_GpoPort) / sizeof(GPO_PORTS))
//	
//	#endif                                  //#ifdef EXT_DEV_GPO
//	
//	
//	
//	/******************************************************************************
//	**GPO����˿�ö�ٶ���
//	******************************************************************************/
//	typedef enum
//	{
//	    GPO_ENET_RST,
//	}GPOENUM;
//	
//	
//	
//	/******************************************************************************
//	**GPI�˿�����ṹ����Ϣ
//	******************************************************************************/
//	#ifdef EXT_DEV_GPI                        //GPI�Ķ˿ڶ���
//	
//	    /******************************************************************************
//	    **LGPI�˿���Ϣ
//	    ******************************************************************************/
//	    const COMPORT gs_LGPIPort[] = 
//	    {
//	        {0x1, 1,  (IOCON_FUNC0 | IOCON_MODE_INACT),  LPC_GPIO, 0},      //G610ģ���ѹ��⣬�ߵ�ƽ��ʾ��������
//	    };
//	    
//	    const uint8 LGPI_PORT_NUM = (sizeof(gs_LGPIPort) / sizeof(COMPORT));
//	    
//	     /******************************************************************************
//	    **FGPI�˿���Ϣ
//	    ******************************************************************************/
//	    const COMPORT gs_FGPIPort[] = 
//	    {
//	        {0x1, 4,  (IOCON_FUNC0 | IOCON_MODE_INACT), LPC_GPIO, 0},      //ͣ���⣬�ߵ�ƽ��ʾ�������ͱ�ʾͣ��
//	    };
//	    
//	    const uint8 FGPI_PORT_NUM = (sizeof(gs_FGPIPort) / sizeof(COMPORT));
//	    
//	#else
//	    
//	    /******************************************************************************
//	    **�����״̬λ�궨��
//	    ******************************************************************************/
//	    #define LGPI_STT_YX1        0x00000001
//	    
//	    //
//	    #define FGPI_STT_ENG        0x00000001         //��λ��ʾ��������
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
//	**�ⲿ���Ź��ܽŶ���
//	******************************************************************************/
//	#if SYS_EXTWDT_EN > 0
//	#ifdef _USR_MAIN                        //WDT�Ķ˿ڶ���
//	    const COMPORT gs_ExtWdtPort = {0x0, 22,  (IOCON_FUNC0 | IOCON_MODE_PULLUP),  LPC_GPIO,  1};
//	#else
//	    extern const COMPORT gs_ExtWdtPort;
//	#endif
//	#endif





#endif





