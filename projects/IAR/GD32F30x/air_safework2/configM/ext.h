/****************************************Copyright (c)*************************
**                               	________科技有限公司
**                                     		开发部
**
**
**--------------文件信息-------------------------------------------------------
**文   件   名: ext.h
**创   建   人: yzy
**最后修改日期: 2018年2月6日
**描        述: 对系统应用的头文件定义
**注        意:
**--------------历史版本信息---------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2010年12月1日
** 描　述: 原始版本
******************************************************************************/


#ifndef _EXT_H_
#define _EXT_H_


/*******************************************************************************
**定义每秒钟的tick数
********************************************************************************/
#define  SYS_TICK_PER_SEC   100

#define SYS_TK_NUM 11
#define MSG_BUFF_LEN        128

#define BUFQUEUE_MSG_NUM       16
#define BUFQUEUE_MSG_MAX       ((MSG_BUFF_LEN/BUFQUEUE_MSG_NUM)-sizeof(size_t))

//Clock配置
#define CGC_CGC_CMC                         (_00_CGC_HISYS_PORT | _10_CGC_SUB_OSC | _00_CGC_SUBMODE_LOW)
#define CGC_CGC_CSC                         (_80_CGC_HISYS_STOP | _00_CGC_SUB_OPER)
#define CGC_CSC_HIO_OPER                    _00_CGC_HIO_OPER

#define CGC_CKC_MIANCLOCK_SEL               _00_CGC_MAINCLK_SELHIO
#define CGC_CKC_CPUCLK_SELMAIN              _00_CGC_CPUCLK_SELMAIN

//	#define CGC_CGC_CKC
#define CGC_CGC_OSMC                        (_00_CGC_SUBINHALT_ON | _00_CGC_RTC_IT_LCD_CLK_FSUB)

#define RTC_CFG_RTCC0                       (_00_RTC_RTC1HZ_DISABLE | _08_RTC_24HOUR_SYSTEM | _02_RTC_INTRTC_CLOCK_1)



/******************************************************************************
**文件系统相关
*******************************************************************************/
#define SYS_FILE_EN      1
#define CBB_FILE_EN SYS_FILE_EN
#define SYS_IFLASH_EN   1

/******************************************************************************
**GPIO
******************************************************************************/
#define SYS_GPO_EN   1                   //通用端口输出模块使能
#define SYS_GPI_EN   1                   //通用端口输入模块使能

#define SYS_LGPI_EN  1                   //慢速通用端口输入模块使能
#define SYS_FGPI_EN  0                   //快速通用端口输入模块使能

#define SYS_GPIEVT_EN  1                 //GPI消息分发使能

#if (SYS_GPO_EN > 0)
    #define GPO_PORT_NUM    1
#endif

#if (SYS_LGPI_EN > 0)
    #define LGPI_PORT_NUM   4
    #define LGPI_GATE       1               //按键扫描门限
    #define LGPI_LAST       20              //按键长按门限

#endif

#if (SYS_FGPI_EN > 0)
    #define FGPI_PORT_NUM   1
    #define FGPI_GATE       3               //快速输入口扫描门限
#endif



/*******************************************************************************
**下面表示系统的串口所需要的资源的定义
********************************************************************************/ 
#define SYS_SER_EN       1               //串口是否有效


/******************************************************************************
**定义物理串口接收缓存大小
******************************************************************************/

#define LEN_OF_RECV0    256
#define LEN_OF_SND0     1

#define LEN_OF_RECV1    256
#define LEN_OF_SND1     1

#define LEN_OF_RECV2    256
#define LEN_OF_SND2     1

#define LEN_OF_RECV3    0x90
#define LEN_OF_SND3     1


#define LEN_UARTR_GPRS 1024



#if SYS_SER_EN > 0
#define SYS_UART0_EN     1               //UART0串口使能
#define SYS_UART1_EN     0               //UART1串口使能
#define SYS_UART2_EN     0               //UART2串口使能
#define SYS_UART3_EN     0               //UART3串口使能
#define SYS_UART4_EN     0               //UART4串口使能
#define SYS_UART5_EN     0               //UART5串口使能
#define SYS_UART9_EN     0               //UART9虚拟串口使能
#define SYS_UART10_EN    0               //UART10空串口使能



#define SYS_UART0_IDX    (0)
#define SYS_UART1_IDX    (SYS_UART0_EN)
#define SYS_UART2_IDX    (SYS_UART0_EN + SYS_UART1_EN)
#define SYS_UART3_IDX    (SYS_UART0_EN + SYS_UART1_EN + SYS_UART2_EN)
#define SYS_UART4_IDX    (SYS_UART0_EN + SYS_UART1_EN + SYS_UART2_EN + SYS_UART3_EN)
#define SYS_UART5_IDX    (SYS_UART0_EN + SYS_UART1_EN + SYS_UART2_EN + SYS_UART3_EN + SYS_UART4_EN)
#define SYS_UART9_IDX    (SYS_UART0_EN + SYS_UART1_EN + SYS_UART2_EN + SYS_UART3_EN + SYS_UART4_EN + SYS_UART5_EN)
#define SYS_UART10_IDX   (SYS_UART0_EN + SYS_UART1_EN + SYS_UART2_EN + SYS_UART3_EN + SYS_UART4_EN + SYS_UART5_EN + SYS_UART9_EN)
                                        //定义有效串口数量
#define NO_OF_SERIAL    (SYS_UART0_EN + SYS_UART1_EN + SYS_UART2_EN + \
                            SYS_UART3_EN + SYS_UART4_EN + SYS_UART5_EN)
#endif

/***********************************************************
设置485芯片为发送/接收状态. 高发低收
***********************************************************/
#define UART0_485_EN  0
#define UART1_485_EN  1
#define UART2_485_EN  1
#define UART3_485_EN  0
#define UART4_485_EN  0

//通道定义
//	#define  UART_CHANNEL_PARA		0//SYS_UART0_IDX    //参数设置串口
//	#define  UART_CHANNEL_IRDA		1//SYS_UART1_IDX    //红外串口
//	#define  UART_CHANNEL_GPRS		2//SYS_UART2_IDX    //gprs串口
//	#define  UART_CHANNEL_DEBUG		3//SYS_UART3_IDX    //debug串口
#define  UART_CHANNEL_GPRS		SYS_UART1_IDX//SYS_UART0_IDX    //gprs串口
#define  UART_CHANNEL_DEBUG		PORT_UART_STD//SYS_UART2_IDX    //debug串口
#define  UART_CHANNEL_ESAM      SYS_UART0_IDX
#define  UART_CHANNEL_CARD      SYS_UART3_IDX
//	#define  UART_CHANNEL_NETP		PORT_UART_STD//
//	#define  UART_CHANNEL_DEBUG		PORT_UART_STD//
//	#define  UART_CHANNEL_DEBUG		PORT_UART_STD//
//	#define  UART_CHANNEL_DEBUG		PORT_UART_STD//


#define  SYS_GPRS_PORT UART_CHANNEL_GPRS

/******************************************************************************
**定义端口序号(排列需按实际串口编号从小到大排列)
******************************************************************************/
//	typedef enum
//	{
//	    SETP_PORT_NO = 0,                    //实际串口编号0 //与 MSG_PSTP 对应
//	    FARP_PORT_NO,                        //实际串口编号1
//	    NETP_PORT_NO,                        //实际串口编号3
//	    NETP2_PORT_NO,                        //实际串口编号
//	    NETP3_PORT_NO,                        //实际串口编号
//	    NETP4_PORT_NO,                        //实际串口编号
//	    
//	}UART_ENUM;



/******************************************************************************
**LCD
******************************************************************************/
#define SYS_LCD_EN   0                   //LCD模块使能

/******************************************************************************
**RF
******************************************************************************/
#define SYS_RF_EN   1                   //rf模块使能

/******************************************************************************
**LED Beep
******************************************************************************/
#define SYS_LED_BEEP_EN   1                   //LED模块使能


/*******************************************************************************
**RTC相关定义
*********************************************************************************/
#define SYS_RTC_EN       1               //ARM自带RTC使能
#define SYS_HRTC_EN      0               //外部硬件RTC使能



/*******************************************************************************
**下面表示系统的外部中断所需要的资源的定义
********************************************************************************/ 
#define SYS_INT_EN       0               //外部中断口是否有效

#if SYS_INT_EN > 0
#define SYS_INT0_EN     1               //INT0外部中断口使能
#define SYS_INT1_EN     0               //INT1外部中断口使能
#define SYS_INT2_EN     0               //INT2外部中断口使能
#define SYS_INT3_EN     1               //INT3外部中断口使能
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
                                        //定义有效中断口数量
#define NO_OF_INTERRUPT    (SYS_INT0_EN + SYS_INT1_EN + SYS_INT2_EN + SYS_INT3_EN + \
                            SYS_INT4_EN + SYS_INT5_EN + SYS_INT6_EN + SYS_INT7_EN)

#endif

#define PRI_UART0       2                  //UART0中断 优先级
#define PRI_UART1       (PRI_UART0)         //UART1中断 优先级
#define PRI_UART2       (PRI_UART1)         //UART2中断 优先级
#define PRI_UART3       (PRI_UART2)         //UART3中断 优先级
#define PRI_UART4       (PRI_UART3)         //UART4中断 优先级
#define PRI_UART5       (PRI_UART4)         //UART4中断 优先级

#define PRI_HWTIMER      (PRI_UART5)         //定时器3中断 优先级
#define PRI_EINT0       (PRI_HWTIMER)        //外部中断0中断 优先级
#define PRI_EINT1       (PRI_EINT0)         //外部中断1中断 优先级
#define PRI_EINT3       (PRI_EINT1)         //外部中断3中断 优先级
/******************************************************************************
**GPIO
******************************************************************************/
//#define SYS_GPO_EN   1                   //通用端口输出模块使能
//#define SYS_GPI_EN   1                   //通用端口输入模块使能
//
//#define SYS_LGPI_EN  1                   //慢速通用端口输入模块使能
//#define SYS_FGPI_EN  0                   //快速通用端口输入模块使能
//
//#define SYS_GPIEVT_EN  1                 //GPI消息分发使能
//
//#if (SYS_GPO_EN > 0)
//    #define GPO_PORT_NUM    1
//#endif
//
//#if (SYS_LGPI_EN > 0)
//    #define LGPI_PORT_NUM   1
//    #define LGPI_GATE       1               //按键扫描门限
//    #define LGPI_LAST       20              //按键长按门限
//
//#endif
//
//#if (SYS_FGPI_EN > 0)
//    #define FGPI_PORT_NUM   1
//    #define FGPI_GATE       3               //快速输入口扫描门限
//#endif


/******************************************************************************
**无线猫功能
******************************************************************************/
#define SYS_MODEM_EN                 1


#if SYS_MODEM_EN > 0
    //性能配置
    #define LEN_OF_IPRECV           LEN_UARTR_GPRS
    #define LEN_OF_SERRECV          10      //服务器模式接收缓存
    #define LEN_OF_IPSEND           2000    //ESAM最长报文需要1850,因此不能小于该值
    #define LEN_OF_SMSRECV 		    142     //短信接收的缓存长度
    #define LEN_OF_SMSNUM 			17      //短信接收的号码长度,末尾留给\0

    //支持的模块
    #define Modem_M72X_EN   0
    #define Modem_EC20_EN   1
    //开关机宏(注意:该版本硬件关机时需要切断txd的管脚,否则G610的电源无法彻底关断)
//	    #define _ATMODEM_VIN()  
//	    #define _ATMODEM_PWRON()        {SYS_GPO_Out(GPO_MODEM_PWR, true);}
//	    #define _ATMODEM_PWROFF()       {SYS_GPO_Out(GPO_MODEM_PWR, false);}
//	
//	    
//	    #define _ATMODEM_PWRWAIT()      {sleep_ms(2000);}//该版本硬件完全放电需要10s.
//	    #define _ATMODEM_ONOFF_0()      {SYS_GPO_Out(GPO_MODEM_ONOFF, false);}
//	    #define _ATMODEM_ONOFF_1()      {SYS_GPO_Out(GPO_MODEM_ONOFF, true);}
//	
//	    #define _ATMODEM_PORTCLOSE()    {SER_Close(UART_CHANNEL_GPRS);}
//		
//	    #define _ATMODEM_PORTOPEN()     {SER_Open(UART_CHANNEL_GPRS, TDB_MODE_R | TDB_MODE_W);}
#endif

//各个串口的默认波特率
#define SETP_PORT_BAUD	    9600
#define FARP_PORT_BAUD	    57600
#define NETP_PORT_BAUD	    4800
#define ISO7816_PORT_BAUD	10753

#define CONFIG_BLE_HCI_H4_UART_PORT 2

/******************************************************************************
**监视模块相关
*******************************************************************************/
#if SYS_SER_EN > 0
    #define SYS_MNT_EN      1
#endif



#if SYS_MNT_EN > 0
    #define MONITOR_EN  0x01                    //状态位,表示当前监视通道是否打开
    #define MONITOR_MD  (0x02 << UART_CHANNEL_DEBUG)   //状态位,表示当前通道监视是否打开

    #define SYS_MNTOUT_HOOK_EN   0               //使能监控输出钩子函数
    
    //定义监控对应的物理通道参数
    #define MONITOR_BUFLEN  1024                //监视通道缓存长度
    #define WATCHPORT_NO    UART_CHANNEL_DEBUG         //定义监视通道使用串口
    
    #define MODEM_SER_TIMEOUT 1000
    
    #define MODEM_STRING_BUFF_LEN     128
#endif


/******************************************************************************
**阀门模块
*******************************************************************************/
#define SYS_VALVE_EN        1
#define SYS_VALVE_CLOSETIME  1500//单位ms
#define SYS_VALVE_OPENTIME   1500

/*******************************************************************************
**下面表示系统的定时器阵列单元所需要的资源的定义
********************************************************************************/ 
#define SYS_TIMER_EN       1               //定时器单元是否有效

#if SYS_TIMER_EN > 0
#define SYS_TIMER0_EN     0               //定时器单元使能
#define SYS_TIMER1_EN     0               //定时器单元使能
#define SYS_TIMER2_EN     0 
#define SYS_TIMER3_EN     0               //定时器单元使能
#define SYS_TIMER4_EN     1               //定时器单元使能
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
                                        //定义有效中断口数量
#define NO_OF_TIMERERRUPT    (SYS_TIMER0_EN + SYS_TIMER1_EN + SYS_TIMER2_EN + SYS_TIMER3_EN + \
                            SYS_TIMER4_EN + SYS_TIMER5_EN + SYS_TIMER6_EN + SYS_TIMER7_EN)

#define TIMER_CHANNEL_ESAM SYS_TIMER5_IDX

#define TIMER_CHANNEL_CARD SYS_TIMER7_IDX

#endif
/*******************************************************************************
**蓝牙
********************************************************************************/ 
#define SYS_BLEMODEM_EN                 1


#if SYS_BLEMODEM_EN > 0
    //支持的模块
    #define Modem_LSB4BT_EN   1
#endif



#endif

