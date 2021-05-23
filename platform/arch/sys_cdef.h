/******************************Copyright(c)******************************
**                         
**
** File Name: 
** Author: 
** Date Last Update: 2019-11-02
** Description: ƽ̨���÷����������Ͷ���
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: �ļ�����
*************************************************************************/
#ifndef _SYS_CDEF_H_
#define _SYS_CDEF_H_


//ϵͳʱ��Ĭ��ʹ��RCHF
#define RCHFCLKCFG	8	//8, 16, 24, 36MHz

//define_all.h��RCHFCLKCFG����ϵͳʱ��
#if( RCHFCLKCFG == 8 )//8.0MHz
#define clkmode   1
#define SYSCLKdef RCC_RCHFCON_FSEL_8MHZ//RCHF����Ƶ��8MHz
#elif( RCHFCLKCFG == 16 )//16.0MHz
#define clkmode   2
#define SYSCLKdef RCC_RCHFCON_FSEL_16MHZ//RCHF����Ƶ��16MHz
#elif( RCHFCLKCFG == 24 )//24.0MHz
#define clkmode   3
#define SYSCLKdef RCC_RCHFCON_FSEL_24MHZ//RCHF����Ƶ��24MHz
#elif( RCHFCLKCFG == 36 )//36.0MHz
#define clkmode   4
#define SYSCLKdef RCC_RCHFCON_FSEL_36MHZ//RCHF����Ƶ��36MHz
#endif





/************************************************************************
**���ڻ��涨��
*************************************************************************/
typedef struct
{
    uint8* tbuff;                           //���ڷ��ͻ���ָ��
    uint16 tlen;                            //���ڷ��ͻ��泤��
    uint16 tcnt;                            //���ڷ������ݳ���
    uint16 tp;                              //���ڷ�������ָ��
    //
    uint8* rbuff;                           //���ڽ��ջ���ָ��
    uint16 rlen;                            //���ڽ��ջ��泤��
    uint16 rcnt;                            //���ڽ������ݳ���
    uint16 rp;                              //���ڽ�������ָ��
    uint16 pr;                              //���ν��ջ������ݴ���ָ��
    //
#if (SYS_UART_ACCT > 0)
    uint8* acc_tbuff;                       //���ڷ��ͼ��ٻ���ָ��
    uint16 acc_tlen;                        //���ڷ��ͼ��ٻ��泤��
    uint16 acc_tcnt;                        //���ڷ��ͼ��ٻ��������ݳ���
    uint16 acc_tp;                          //���ڷ��ͼ��ٻ���������ָ��
    uint16 acc_pt;                          //���η��ͼ��ٻ������ݴ���ָ��
#endif
    uint32_t   RxBuf_is_full;
    uint32_t  uart_error_count;
    uint32_t  uart_dma_stop;  //this bit is used only for error count now
    uint32_t  previous_dma_leftbyte;

}SerialBuffer;


/************************************************************************
**�����������ݽṹ��
*************************************************************************/

/************************************************************************
**�����շ�������λ��ö��
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
**�����շ���У��λ��ö��
*************************************************************************/
typedef enum 
{
    Parit_N = 0,     // ��
    Parit_O,        //��
    Parit_E,        //ż
}SerialParit;

/************************************************************************
**�����շ���ֹͣλ��ö��
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
	uint8   sec;       //��
	uint8   min;       //��
	uint8   hour;      //ʱ
	uint8   day;       //��
	uint8   month;     //��
	uint8   year;      //���ֽ���
	uint8   yearh;     //���ֽ���
	uint8   week;      //BIT7:����ʱ��ʶ��bit0~bit3����
    uint16 dyear;       //д���ڲ�RTCʱ�˹�����(0-365)s
}TIME;
/************************************************************************
**rs485�������

*************************************************************************/
typedef struct
{
    TCallback setModeRecv;        //�л�Ϊ����״̬�ĺ�����
    TCallback setModeSend;        //�л�Ϊ����״̬�ĺ�����
    TCallback setModeInit;        //�����շ�ģʽ�л���IO�ں�����
}SerialTRC;






/************************************************************************
**���ڷ������Ϣ��
*************************************************************************/
typedef struct
{
    uint8_t uart_no;                      //�����ں�(���ڹ̶�Ϊ0xff)
    uint8_t irqPri;                       //�����ж����ȼ�
    uint8_t irqn;
    uint8_t * inited;
    
    uint8_t uart;//USART_TypeDef * pUART;
    TCallback   pinConfig;               //���ڹܽ����ú�����
//    uint8_t rs485;//
    TCallback   msp;

    SerialTRC* rs485;                   //RS485���շ��л�����(��Ч��ֵ__NULL)
    SerialBuffer*  buffer;              //���ڻ���
    uint8* recvFlag;                    //���ݽ�����ϱ�־
    uint16 recvBufLen;                  //���ڽ��ջ��泤��
//    UART_HandleTypeDef * phandle;
#if (SYS_UART_ACCT > 0)                  //(accelerate buffer����������������������ʱ��ϵͳ����)
    uint16 accBufLen;                   //���ڷ��ͼ��ٻ��泤��(1.����Ӳ��FIFO����,2.���ڵ���systick���͵��ֽ���)
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
**���ݿ��������
*************************************************************************/
typedef enum
{
    TDB_MODE_R = 1,       //��Ȩ�޴�
    TDB_MODE_W = 2,       //дȨ�޴�
    TDB_MODE_RW = 3,      //��д��
    TDB_MODE_BK = 4,      //֧�ֱ��ݵ����ݿ�,���и�������������ż��ҳ,��������
}TDB_MODE;
typedef enum
{
    TDB_PERMS_SYS = 1,       //ϵͳȨ��
    TDB_PERMS_SUSER = 2,       //�����û�
    TDB_PERMS_USER = 3,      //��ͨ�û�
}TDB_PERMS;

/************************************************************************
**���ݿ���Ϣ�ṹ�嶨��
*************************************************************************/
typedef struct
{
    uint32 start;               //��ʼ��ַ
    uint32 length;              //����
    uint8  RW;                  //����,����ʱָ����
    uint8 _b;
    uint16 ID;                  //��֮���Ǳ��,�洢�ĵ�ַ����CRCУ��
}TDataBlock;



/************************************************************************
**���ݿ���Ϣ�б�ṹ�嶨��
*************************************************************************/
typedef struct
{
    uint16 dbno;                         //�ļ����
    uint16 sml;                          //ͬ����
    uint16 blocks;                       //��С���ݿ�����
    uint8 atr;                           //����
}S_FILEBLOCKS;


//flashѭ���洢���ṹ�嶨��
typedef struct
{
    uint32 fls_addr;            //flash����ͷ��
    uint32 fls_addr_bk;         //flash��������ͷ��*
    uint32 fls_addr_end;        //flash����β��*
    uint16 keyno;               //��Ŀ����
    uint16 itm_baby_byte;       //����Ŀ�ڵ���Ա�����ݳ���
    uint8 itm_baby_num;         //����Ŀ�ڵ���Ա��ѭ������(<128)
    uint8 itm_sectors;          //����Ŀռ�õ�flash������
}TFlsCycleConst;









/************************************************************************
**ͨ���������IO�ڶ���
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
 * COMPORTģʽ���ú�
 *************************************************************************/
//b24-b31(λ����)
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

//b16-b23(λ����)
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


//b12-b15(λ����)
#define GPIO_SPEED_LOW       0x00000000
#define GPIO_SPEED_MEDIUM    0x00001000
#define GPIO_SPEED_HIGH      0x00002000

//b8-b11(λ����)
#define GPIO_MODE_NOPULL     0x00000000
#define GPIO_MODE_PULLUP     0x00000100
#define GPIO_MODE_PULLDOWN   0x00000200
#define GPIO_MODE_OPENDRAIN  0x00000400

//b0-b7(������ֵ)
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
**ͨ���������IO�ڶ���
**��Ա����:P1.07
**0x1, 7,  (IOCON_FUNC1 | IOCON_MODE_INACT | IOCON_DIGMODE_EN), LPC_GPIO, 1
******************************************************************************/
typedef struct
{
    //
	uint32_t pingrp:3;                    //Pin group             //����PINMUX_GRP_T��Ա
	uint32_t pinnum:5;                    //Pin number            //����PINMUX_GRP_T��Ա
//	uint32_t type:8;                      //Function and mode     //����PINMUX_GRP_T��Ա
	uint32_t modefunc:24;                 //Function and mode     //����PINMUX_GRP_T��Ա
    //
    GPIOx_Type * gpio;                   //GPIO�Ĵ���
    uint8_t analog;                      //0���֣�1ģ���������
    uint8_t dir;                          //2:���,1������io��0:����
    uint8_t rvs;                         //�ߵ͵�ƽ��ת:0����,1����
    uint8_t  ival;   //�߼���ʼֵ:0(��),1(��)

}COMPORT;

/*****************************************************************************
**����SPI�˿ڽṹ��
******************************************************************************/
typedef struct
{
    COMPORT* portMISO;                  //SPI��������ӳ�,����
    COMPORT* portMOSI;                  //SPI������������
    COMPORT* portSCK;                   //SPI����ʱ����
    COMPORT* portPWR;                    //SPI������Դ��
    COMPORT* portCS;                    //SPI����Ƭѡ��
    COMPORT* portWP;                    //SPI����������
    COMPORT* portPWR2;                    //SPI������Դ��
    COMPORT* portCS2;                   //SPI����2Ƭѡ��
    COMPORT* portWP2;                   //SPI����2������
    COMPORT* portPWR3;                    //SPI������Դ��
    COMPORT* portCS3;                   //SPI����3Ƭѡ��
    COMPORT* portWP3;                   //SPI����3������
    
}SPIIO_PORTS;



/************************************************************************
**����IIC�˿ڽṹ��
*************************************************************************/
typedef struct
{
    COMPORT* portSDAOut;               //IIC����������
    COMPORT* portSCK;               //IIC����ʱ����
    COMPORT* portSDAIn;               //IIC����������
    
}IIC_PORTS;


/************************************************************************
**����GPI�˿ڽṹ��
*************************************************************************/
#ifndef __NO_SYS__
typedef struct
{
    COMPORT* cp;     //�˿�����
    uint8_t     filter:1; //�Ƿ���Ҫȥ��
}GPI_PORTS;

#else
typedef struct
{
//	    COMPORT* cp;     //�˿�����
	uint32_t pingrp:3;                    //Pin group             //����PINMUX_GRP_T��Ա
	uint32_t pinnum:5;                    //Pin number            //����PINMUX_GRP_T��Ա
	uint32_t type:8;                      //Function and mode     //����PINMUX_GRP_T��Ա
	uint32_t modefunc:16;                 //Function and mode     //����PINMUX_GRP_T��Ա
    //
    GPIOx_Type * gpio;                   //GPIO�Ĵ���
    uint8_t analog;                      //0���֣�1ģ���������
    uint8_t dir;                          //2:���,1������io��0:����
//	    uint8_t rvs;                         //�ߵ͵�ƽ��ת:0����,1����
//	    uint8  ival:1;   //�߼���ʼֵ:0(��),1(��)

    uint8_t     filter:1; //�Ƿ���Ҫȥ��
    //uint8  en:1;
    uint8_t     handleen:1;
    uint8_t     handleno:6;//io�жϺ�
    uint8_t     edge;
}GPI_PORTS;

#endif
/************************************************************************
**����GPO�˿ڽṹ��
*************************************************************************/
typedef struct
{
    COMPORT* cp;     //�˿�����
    uint8  rvs:1;    //�ߵ͵�ƽ��ת(�����߼���ƽ�������ƽ�Ķ�Ӧ):0����(true��false��),1����(true��false��)
    uint8  ival:1;   //�߼���ʼֵ:0(true),1(false)
    //uint8  en:1;
    
}GPO_PORTS;
/******************************************************************************
**AD�ڶ���
**��Ա����:P1.31
**0x1, 31,  (IOCON_FUNC3 | IOCON_MODE_INACT ), LPC_ADC, 
******************************************************************************/
typedef struct
{
    //
	uint32 pingrp:3;                    //Pin group             //����PINMUX_GRP_T��Ա
	uint32 pinnum:5;                    //Pin number            //����PINMUX_GRP_T��Ա
//		uint32 modefunc:24;                 //Function and mode     //����PINMUX_GRP_T��Ա
	//uint32_t type:8;                      //Function and mode     //����PINMUX_GRP_T��Ա
	uint32_t modefunc:24;                 //Function and mode     //����PINMUX_GRP_T��Ա

    //
    //LPC_ADC_T* adc;                     //ADC�Ĵ���
    
    GPIOx_Type * gpio;                   //GPIO�Ĵ���
    uint32 vref;                        //�ο���ѹ
    uint16_t  vnum;                        //��Ƶ��
    uint8  chn;                         //ͨ����
    uint8_t ctrlen;
    
    uint8_t ctrlno;
}ADPORT;



/* GPIO���ú��������궨�� */
//IO�������� 
// 0 = ���� 
// 1 = ���
#define GPIO_DIR_IN	0
#define GPIO_DIR_OUT	1



//IO��������� 
//type 0 = ��ͨ 
//type 1 = ����
#define IN_NORMAL	0
#define IN_PULLUP	1

//IO��������� 
//type 0 = ��ͨ 
//type 1 = OD
#define OUT_PUSHPULL	0
#define OUT_OPENDRAIN	1
#define OUT_PULLUP			2
#define OUT_OPENDRAIN_PULLUP	3

//IO�������⹦�ܿ� 
//type 0 = ��ͨ 
//type 1 = OD (OD���ܽ��������⹦��֧��)
//type 2 = ��ͨ+���� 
//type 3 = OD+����
#define ALTFUN_NORMAL			0
#define ALTFUN_OPENDRAIN		1
#define ALTFUN_PULLUP			2
#define ALTFUN_OPENDRAIN_PULLUP	3


#define ANALOG_DISENABLE   0
#define ANALOG_ENENABLE   1


#endif

