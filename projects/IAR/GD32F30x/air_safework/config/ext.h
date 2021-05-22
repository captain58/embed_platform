/****************************************Copyright (c)*************************
**                               	________�Ƽ����޹�˾
**                                     		������
**
**
**--------------�ļ���Ϣ-------------------------------------------------------
**��   ��   ��: ext.h
**��   ��   ��: yzy
**����޸�����: 2018��2��6��
**��        ��: ��ϵͳӦ�õ�ͷ�ļ�����
**ע        ��:
**--------------��ʷ�汾��Ϣ---------------------------------------------------
** ������: yzy
** ��  ��: v1.0
** �ա���: 2010��12��1��
** �衡��: ԭʼ�汾
******************************************************************************/


#ifndef _EXT_H_
#define _EXT_H_



#define SYS_TK_NUM 10
#define MSG_BUFF_LEN        128

#define BUFQUEUE_MSG_NUM       16
#define BUFQUEUE_MSG_MAX       ((MSG_BUFF_LEN/BUFQUEUE_MSG_NUM)-sizeof(size_t))

//Clock����
#define CGC_CGC_CMC                         (_00_CGC_HISYS_PORT | _10_CGC_SUB_OSC | _00_CGC_SUBMODE_LOW)
#define CGC_CGC_CSC                         (_80_CGC_HISYS_STOP | _00_CGC_SUB_OPER)
#define CGC_CSC_HIO_OPER                    _00_CGC_HIO_OPER

#define CGC_CKC_MIANCLOCK_SEL               _00_CGC_MAINCLK_SELHIO
#define CGC_CKC_CPUCLK_SELMAIN              _00_CGC_CPUCLK_SELMAIN

//	#define CGC_CGC_CKC
#define CGC_CGC_OSMC                        (_00_CGC_SUBINHALT_ON | _00_CGC_RTC_IT_LCD_CLK_FSUB)

#define RTC_CFG_RTCC0                       (_00_RTC_RTC1HZ_DISABLE | _08_RTC_24HOUR_SYSTEM | _02_RTC_INTRTC_CLOCK_1)



/******************************************************************************
**GPIO
******************************************************************************/
#define SYS_GPO_EN   1                   //ͨ�ö˿����ģ��ʹ��
#define SYS_GPI_EN   1                   //ͨ�ö˿�����ģ��ʹ��

#if SYS_GPI_EN > 0
    #define SYS_LGPI_EN  1                   //����ͨ�ö˿�����ģ��ʹ��
    #define SYS_FGPI_EN  0                   //����ͨ�ö˿�����ģ��ʹ��
    #define SYS_AD_EN    1                   //ADʹ��
    #define SYS_HALL_EN 0
#endif

/*******************************************************************************
**�����ʾϵͳ�Ĵ�������Ҫ����Դ�Ķ���
********************************************************************************/ 
#define SYS_SER_EN       1               //�����Ƿ���Ч


/******************************************************************************
**�����������ڽ��ջ����С
******************************************************************************/

#define LEN_OF_RECV0    0x90
#define LEN_OF_SND0     1

#define LEN_OF_RECV1    256
#define LEN_OF_SND1     1

#define LEN_OF_RECV2    256
#define LEN_OF_SND2     1

#define LEN_OF_RECV3    0x90
#define LEN_OF_SND3     1


#define LEN_UARTR_GPRS 1024



#if SYS_SER_EN > 0
#define SYS_UART0_EN     0               //UART0����ʹ��
#define SYS_UART1_EN     1               //UART1����ʹ��
#define SYS_UART2_EN     1               //UART2����ʹ��
#define SYS_UART3_EN     1               //UART3����ʹ��
#define SYS_UART4_EN     1               //UART4����ʹ��
#define SYS_UART5_EN     1               //UART5����ʹ��
#define SYS_UART9_EN     0               //UART9���⴮��ʹ��
#define SYS_UART10_EN    0               //UART10�մ���ʹ��



#define SYS_UART0_IDX    (0)
#define SYS_UART1_IDX    (SYS_UART0_EN)
#define SYS_UART2_IDX    (SYS_UART0_EN + SYS_UART1_EN)
#define SYS_UART3_IDX    (SYS_UART0_EN + SYS_UART1_EN + SYS_UART2_EN)
#define SYS_UART4_IDX    (SYS_UART0_EN + SYS_UART1_EN + SYS_UART2_EN + SYS_UART3_EN)
#define SYS_UART5_IDX    (SYS_UART0_EN + SYS_UART1_EN + SYS_UART2_EN + SYS_UART3_EN + SYS_UART4_EN)
#define SYS_UART9_IDX    (SYS_UART0_EN + SYS_UART1_EN + SYS_UART2_EN + SYS_UART3_EN + SYS_UART4_EN + SYS_UART5_EN)
#define SYS_UART10_IDX   (SYS_UART0_EN + SYS_UART1_EN + SYS_UART2_EN + SYS_UART3_EN + SYS_UART4_EN + SYS_UART5_EN + SYS_UART9_EN)
                                        //������Ч��������
#define NO_OF_SERIAL    (SYS_UART0_EN + SYS_UART1_EN + SYS_UART2_EN + \
                            SYS_UART3_EN + SYS_UART4_EN + SYS_UART5_EN)
#endif

/***********************************************************
����485оƬΪ����/����״̬. �߷�����
***********************************************************/
#define UART0_485_EN  0
#define UART1_485_EN  1
#define UART2_485_EN  1
#define UART3_485_EN  0
#define UART4_485_EN  0

//ͨ������
//	#define  UART_CHANNEL_PARA		0//SYS_UART0_IDX    //�������ô���
//	#define  UART_CHANNEL_IRDA		1//SYS_UART1_IDX    //���⴮��
//	#define  UART_CHANNEL_GPRS		2//SYS_UART2_IDX    //gprs����
//	#define  UART_CHANNEL_DEBUG		3//SYS_UART3_IDX    //debug����
#define  UART_CHANNEL_GPRS		SYS_UART1_IDX//SYS_UART0_IDX    //gprs����
#define  UART_CHANNEL_DEBUG		PORT_UART_STD//SYS_UART2_IDX    //debug����
#define  UART_CHANNEL_ESAM      SYS_UART0_IDX
#define  UART_CHANNEL_CARD      SYS_UART3_IDX
//	#define  UART_CHANNEL_NETP		PORT_UART_STD//
//	#define  UART_CHANNEL_DEBUG		PORT_UART_STD//
//	#define  UART_CHANNEL_DEBUG		PORT_UART_STD//
//	#define  UART_CHANNEL_DEBUG		PORT_UART_STD//


#define  SYS_GPRS_PORT UART_CHANNEL_GPRS

/******************************************************************************
**����˿����(�����谴ʵ�ʴ��ڱ�Ŵ�С��������)
******************************************************************************/
//	typedef enum
//	{
//	    SETP_PORT_NO = 0,                    //ʵ�ʴ��ڱ��0 //�� MSG_PSTP ��Ӧ
//	    FARP_PORT_NO,                        //ʵ�ʴ��ڱ��1
//	    NETP_PORT_NO,                        //ʵ�ʴ��ڱ��3
//	    NETP2_PORT_NO,                        //ʵ�ʴ��ڱ��
//	    NETP3_PORT_NO,                        //ʵ�ʴ��ڱ��
//	    NETP4_PORT_NO,                        //ʵ�ʴ��ڱ��
//	    
//	}UART_ENUM;



/******************************************************************************
**LCD
******************************************************************************/
#define SYS_LCD_EN   1                   //LCDģ��ʹ��

/******************************************************************************
**LED Beep
******************************************************************************/
#define SYS_LED_BEEP_EN   1                   //LEDģ��ʹ��


/*******************************************************************************
**RTC��ض���
*********************************************************************************/
#define SYS_RTC_EN       1               //ARM�Դ�RTCʹ��
#define SYS_HRTC_EN      0               //�ⲿӲ��RTCʹ��



/*******************************************************************************
**�����ʾϵͳ���ⲿ�ж�����Ҫ����Դ�Ķ���
********************************************************************************/ 
#define SYS_INT_EN       0               //�ⲿ�жϿ��Ƿ���Ч

#if SYS_INT_EN > 0
#define SYS_INT0_EN     1               //INT0�ⲿ�жϿ�ʹ��
#define SYS_INT1_EN     0               //INT1�ⲿ�жϿ�ʹ��
#define SYS_INT2_EN     0               //INT2�ⲿ�жϿ�ʹ��
#define SYS_INT3_EN     1               //INT3�ⲿ�жϿ�ʹ��
#define SYS_INT4_EN     1 
#define SYS_INT5_EN     0 
#define SYS_INT6_EN     1 
#define SYS_INT7_EN     1 

//	#define NO_OF_INTERRUPT    6

#define SYS_INT0_IDX    (0)
#define SYS_INT1_IDX    (SYS_INT0_EN)
#define SYS_INT2_IDX    (SYS_INT0_EN + SYS_INT1_EN)
#define SYS_INT3_IDX    (SYS_INT0_EN + SYS_INT1_EN + SYS_INT2_EN)
#define SYS_INT4_IDX    (SYS_INT0_EN + SYS_INT1_EN + SYS_INT2_EN + SYS_INT3_EN)
#define SYS_INT5_IDX    (SYS_INT0_EN + SYS_INT1_EN + SYS_INT2_EN + SYS_INT3_EN + SYS_INT4_EN)
#define SYS_INT6_IDX    (SYS_INT0_EN + SYS_INT1_EN + SYS_INT2_EN + SYS_INT3_EN + SYS_INT4_EN + SYS_INT5_EN)
#define SYS_INT7_IDX    (SYS_INT0_EN + SYS_INT1_EN + SYS_INT2_EN + SYS_INT3_EN + SYS_INT4_EN + SYS_INT5_EN + SYS_INT6_EN)
                                        //������Ч�жϿ�����
#define NO_OF_INTERRUPT    (SYS_INT0_EN + SYS_INT1_EN + SYS_INT2_EN + SYS_INT3_EN + \
                            SYS_INT4_EN + SYS_INT5_EN + SYS_INT6_EN + SYS_INT7_EN)

#endif

/******************************************************************************
**����è����
******************************************************************************/
#define SYS_MODEM_EN                 1


#if SYS_MODEM_EN > 0
    //��������
    #define LEN_OF_IPRECV           LEN_UARTR_GPRS
    #define LEN_OF_SERRECV          10      //������ģʽ���ջ���
    #define LEN_OF_IPSEND           2000    //ESAM�������Ҫ1850,��˲���С�ڸ�ֵ
    #define LEN_OF_SMSRECV 		    142     //���Ž��յĻ��泤��
    #define LEN_OF_SMSNUM 			17      //���Ž��յĺ��볤��,ĩβ����\0

    //֧�ֵ�ģ��
    #define Modem_M72X_EN   0
    #define Modem_EC20_EN   1
    //���ػ���(ע��:�ð汾Ӳ���ػ�ʱ��Ҫ�ж�txd�Ĺܽ�,����G610�ĵ�Դ�޷����׹ض�)
//	    #define _ATMODEM_VIN()  
//	    #define _ATMODEM_PWRON()        {SYS_GPO_Out(GPO_MODEM_PWR, true);}
//	    #define _ATMODEM_PWROFF()       {SYS_GPO_Out(GPO_MODEM_PWR, false);}
//	
//	    
//	    #define _ATMODEM_PWRWAIT()      {sleep_ms(2000);}//�ð汾Ӳ����ȫ�ŵ���Ҫ10s.
//	    #define _ATMODEM_ONOFF_0()      {SYS_GPO_Out(GPO_MODEM_ONOFF, false);}
//	    #define _ATMODEM_ONOFF_1()      {SYS_GPO_Out(GPO_MODEM_ONOFF, true);}
//	
//	    #define _ATMODEM_PORTCLOSE()    {SER_Close(UART_CHANNEL_GPRS);}
//		
//	    #define _ATMODEM_PORTOPEN()     {SER_Open(UART_CHANNEL_GPRS, TDB_MODE_R | TDB_MODE_W);}
#endif

//�������ڵ�Ĭ�ϲ�����
#define SETP_PORT_BAUD	    9600
#define FARP_PORT_BAUD	    57600
#define NETP_PORT_BAUD	    4800
#define ISO7816_PORT_BAUD	10753

#define CONFIG_BLE_HCI_H4_UART_PORT 2

/******************************************************************************
**����ģ�����
*******************************************************************************/
#if SYS_SER_EN > 0
    #define SYS_MNT_EN      1
#endif



#if SYS_MNT_EN > 0
    #define MONITOR_EN  0x01                    //״̬λ,��ʾ��ǰ����ͨ���Ƿ��
    #define MONITOR_MD  (0x02 << UART_CHANNEL_DEBUG)   //״̬λ,��ʾ��ǰͨ�������Ƿ��

    #define SYS_MNTOUT_HOOK_EN   0               //ʹ�ܼ��������Ӻ���
    
    //�����ض�Ӧ������ͨ������
    #define MONITOR_BUFLEN  1024                //����ͨ�����泤��
    #define WATCHPORT_NO    UART_CHANNEL_DEBUG         //�������ͨ��ʹ�ô���
    
    #define MODEM_SER_TIMEOUT 1000
    
    #define MODEM_STRING_BUFF_LEN     128
#endif


/******************************************************************************
**����ģ��
*******************************************************************************/
#define SYS_VALVE_EN        1
#define SYS_VALVE_CLOSETIME  1500//��λms
#define SYS_VALVE_OPENTIME   1500

/*******************************************************************************
**�����ʾϵͳ�Ķ�ʱ�����е�Ԫ����Ҫ����Դ�Ķ���
********************************************************************************/ 
#define SYS_TIMER_EN       1               //��ʱ����Ԫ�Ƿ���Ч

#if SYS_TIMER_EN > 0
#define SYS_TIMER0_EN     0               //��ʱ����Ԫʹ��
#define SYS_TIMER1_EN     0               //��ʱ����Ԫʹ��
#define SYS_TIMER2_EN     0 
#define SYS_TIMER3_EN     0               //��ʱ����Ԫʹ��
#define SYS_TIMER4_EN     1               //��ʱ����Ԫʹ��
#define SYS_TIMER5_EN     1 
#define SYS_TIMER6_EN     1 
#define SYS_TIMER7_EN     1 

#define SYS_TIMER0_IDX    (0)
#define SYS_TIMER1_IDX    (SYS_TIMER0_EN)
#define SYS_TIMER2_IDX    (SYS_TIMER0_EN + SYS_TIMER1_EN)
#define SYS_TIMER3_IDX    (SYS_TIMER0_EN + SYS_TIMER1_EN + SYS_TIMER2_EN)
#define SYS_TIMER4_IDX    (SYS_TIMER0_EN + SYS_TIMER1_EN + SYS_TIMER2_EN + SYS_TIMER3_EN)
#define SYS_TIMER5_IDX    (SYS_TIMER0_EN + SYS_TIMER1_EN + SYS_TIMER2_EN + SYS_TIMER3_EN + SYS_TIMER4_EN)
#define SYS_TIMER6_IDX    (SYS_TIMER0_EN + SYS_TIMER1_EN + SYS_TIMER2_EN + SYS_TIMER3_EN + SYS_TIMER4_EN + SYS_TIMER5_EN)
#define SYS_TIMER7_IDX    (SYS_TIMER0_EN + SYS_TIMER1_EN + SYS_TIMER2_EN + SYS_TIMER3_EN + SYS_TIMER4_EN + SYS_TIMER5_EN + SYS_TIMER6_EN)
                                        //������Ч�жϿ�����
#define NO_OF_TIMERERRUPT    (SYS_TIMER0_EN + SYS_TIMER1_EN + SYS_TIMER2_EN + SYS_TIMER3_EN + \
                            SYS_TIMER4_EN + SYS_TIMER5_EN + SYS_TIMER6_EN + SYS_TIMER7_EN)

#define TIMER_CHANNEL_ESAM SYS_TIMER5_IDX

#define TIMER_CHANNEL_CARD SYS_TIMER7_IDX

#endif
/*******************************************************************************
**����
********************************************************************************/ 
#define SYS_BLEMODEM_EN                 1


#if SYS_BLEMODEM_EN > 0
    //֧�ֵ�ģ��
    #define Modem_LSB4BT_EN   1
#endif
#endif
