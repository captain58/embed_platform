/****************************************Copyright (c)*************************
**                               	________�Ƽ����޹�˾
**                                     		������
**
**
**--------------�ļ���Ϣ-------------------------------------------------------
**��   ��   ��: cdef.h
**��   ��   ��: yzy
**����޸�����: 2010��3��3��
**��        ��: Uart��һЩ�������幫��
**ע        ��:
**--------------��ʷ�汾��Ϣ---------------------------------------------------
** ������: yzy
** ��  ��: v1.0
** �ա���: 2010��3��3��
** �衡��: ԭʼ�汾
******************************************************************************/
#ifndef _CDEF_H_
#define _CDEF_H_




/*******************************************************************************
**�����շ�������λ��ö��
*******************************************************************************/
typedef enum 
{
    DataBits_7bits = 0,
    DataBits_8bits
}SerialDataBits;

/*******************************************************************************
**�����շ���У��λ��ö��
*******************************************************************************/
typedef enum 
{
    Parit_N = 0,
    Parit_O,
    Parit_E,
}SerialParit;

/*******************************************************************************
**�����շ���ֹͣλ��ö��
*******************************************************************************/
typedef enum 
{
    StopBits_1 = 0,
    StopBits_2,
}SerialStopBits;

/*******************************************************************************
**�����������ݽṹ��
*******************************************************************************/
typedef struct 
{
    uint32  baudrate;
    SerialDataBits  databits;
    SerialParit     parit;
    SerialStopBits  stopbits;
}SerialSets;

/*******************************************************************************
**�����������ݽṹ��
*******************************************************************************/
typedef struct 
{
    uint16  TDR;            //����
    union{
        struct {
            uint16  mode:4;//ģʽ 0:�����ʱ����1:pwm�����master;9:pwm�����slave
            uint16  r00:2;//00
            uint16  edge:2;//���� 0:�½��� 1:������ 2:˫����(�͵�ƽ)3:˫����(�ߵ�ƽ)
            uint16  trigger:3;//����
            uint16  master:1;//2,4,6ͨ��0:slave 1:master   or  (1,3ͨ��)0:16bit  1:8bit 
            uint16  ccs:1;//ͨ��n ����ʱ�ӣ�fTCLK����ѡ��
            uint16  r0:1;//����
            uint16  cks:2;//0:ʱ��Դ0��1ʱ��Դ2��2:ʱ��Դ1��3:ʱ��Դ3
        }TMR_bit;
        uint16 TMR;
        
    };
    uint8 level;//�����ƽ   0���߼� ,1���߼� 
    uint8 stt;  //����Ĵ���   0�����0��1�����1��
    uint8 en;   //��������Ĵ��� 0������ 1���� 
}TimerSets;


/******************************************************************************
**���ڻ��涨��
******************************************************************************/
typedef struct
{
    uint8 * tbuff;                           //���ڷ��ͻ���ָ��
    uint16 tlen;                            //���ڷ��ͻ��泤��
    uint16 tcnt;                            //���ڷ������ݳ���
    uint16 tp;                              //���ڷ�������ָ��
    //
    uint8 * rbuff;                           //���ڽ��ջ���ָ��
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
}SerialBuffer;


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
    
    uint32_t pUART;//USART_TypeDef * pUART;
    uint32_t clk;
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

//    uint32_t       overSampling;
//    uint32_t       OneBitSampling;
//    uint32_t       AdvFeatureInit;    

}SerialID;
/******************************************************************************
**���ݿ��������
******************************************************************************/
typedef enum
{
    TDB_MODE_R = 1,       //��Ȩ�޴�
    TDB_MODE_W = 2,       //дȨ�޴�
    TDB_MODE_RW = 3,      //��д��
    TDB_MODE_BK = 4,      //֧�ֱ��ݵ����ݿ�,���и�������������ż��ҳ,��������
    TDB_MODE_R_O = 0x10,     //ֻ��
    TDB_MODE_W_O = 0x20,     //ֻд
}TDB_MODE;
typedef enum
{
    TDB_PERMS_SYS = 1,       //ϵͳȨ��
    TDB_PERMS_SUSER = 2,       //�����û�
    TDB_PERMS_USER = 3,      //��ͨ�û�
}TDB_PERMS;
/******************************************************************************
**��ʱ����������
******************************************************************************/
typedef enum
{
    TIMER_OUTPUT_ON  = 1,       //���������
    TIMER_TRIGGER_ON = 2,       //��������
    TIMER_ON         = 3,       //�������ͬʱ��
    TIMER_INT_ON     = 4,       //�жϴ� 
}TIMER_MODE;


/******************************************************************************
**���ݿ���Ϣ�ṹ�嶨��
******************************************************************************/
typedef struct
{
    uint32 start;               //��ʼ��ַ
    uint32 length;              //����
    uint8  RW;                  //����,����ʱָ����
    uint8 _b;
    uint16 ID;                  //��֮���Ǳ��,�洢�ĵ�ַ����CRCУ��
}TDataBlock;



/******************************************************************************
**���ݿ���Ϣ�б�ṹ�嶨��
******************************************************************************/
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
**ʱ���ǩ
*************************************************************************/
typedef struct
{
    uint32 mins;
    uint8  YMDhms[6];
    uint16 crc;
    
}S_TimeLabel;//12�ֽ�

/******************************************************************************
**ϵͳʱ��ṹ�嶨��
******************************************************************************/
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


/******************************************************************************
**ͨ���������IO�ڶ���
**��Ա����:P1.07
**0x1, 7,  (IOCON_FUNC1 | IOCON_MODE_INACT | IOCON_DIGMODE_EN), LPC_GPIO, 1
******************************************************************************/
typedef struct
{
    //
	uint32 pingrp:4;                    //Pin group             //����PINMUX_GRP_T��Ա
	uint32 pinnum:8;                    //Pin number            //����PINMUX_GRP_T��Ա
	uint32 pinseg:2;                    //pin pfseg 1:ͨ��io, 0:�μĴ���
	uint32 modefunc:8;                 //Function and mode     //����PINMUX_GRP_T��Ա
    uint32_t speed:10;
    //
    uint8 dir;                          //1:���,0:����

}COMPORT;








/*****************************************************************************
**����IIC�˿ڽṹ��
******************************************************************************/
typedef struct
{
    COMPORT* portSDA;               //IIC����������
    COMPORT* portSCK;               //IIC����ʱ����
    COMPORT* portPWR;               //��Դ
    COMPORT* portADDR1;               //��ַ��1
    COMPORT* portADDR2;               //��ַ��2
    COMPORT* portADDR3;               //��ַ��3

}IIC_PORTS;


/*****************************************************************************
**����GPO�˿ڽṹ��
******************************************************************************/
typedef struct
{
    COMPORT* port;                      //�˿�����
    uint8  set;                      //��ʼ��ƽ
//    uint8  rvs;                         //�ߵ͵�ƽ��ת:0����,1����
    uint8_t rvs;                         //�ߵ͵�ƽ��ת:0����,1����
    uint8_t  ival;   //�߼���ʼֵ:0(��),1(��)
    
}GPO_PORTS;

/*****************************************************************************
**����GPO�˿ڽṹ��
******************************************************************************/
typedef struct
{
    COMPORT* port;                      //�˿�����
    uint8_t     filter:1; //�Ƿ���Ҫȥ��
    uint8  pullup;                         //�Ƿ����©�籣��
    
	uint16 pingrp:4;                    //Pin group             //����PINMUX_GRP_T��Ա
	uint16 pinnum:8;                    //Pin number            //����PINMUX_GRP_T��Ա
    uint16 reserver:4;
}GPI_PORTS;

	
#ifndef BOOT_ONLY                       //BOOT����������

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

