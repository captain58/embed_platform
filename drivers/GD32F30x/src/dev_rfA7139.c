

#define EXT_RF
#ifndef _HAL_RF_C_
#define _HAL_RF_C_


#include "sys.h"
#include "board.h"
#include "hal.h"
#include "bsp.h"
#include "A7139reg.h"

#include "A7139config.h"
#include "App_Public.h"
#include "user_func.h"


#define CON_FIFO_LEN 64

//	void  INIT_MCU_RF_MAP(void)
//	{
//		Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE);//使能GPIO模块时钟
//		stc_gpio_cfg_t stcGpioCfg;//IO 配置结构体指针
//		DDL_ZERO_STRUCT(stcGpioCfg);//将里面的数据清空
//		stcGpioCfg.enDir=GpioDirOut;//输出
//		Gpio_Init(GpioPortB, GpioPin1, &stcGpioCfg);//SCK
//		Gpio_Init(GpioPortB, GpioPin10, &stcGpioCfg);//SCS
//		Gpio_Init(GpioPortC, GpioPin5, &stcGpioCfg);//SDIO
//		
//		stcGpioCfg.enDir=GpioDirIn;//输入
//		Gpio_Init(GpioPortA, GpioPin7, &stcGpioCfg);//GIO1
//		Gpio_Init(GpioPortB, GpioPin6, &stcGpioCfg);//GIO2
//		
//	}

//	#define GIO1								Gpio_GetInputIO(GpioPortA, GpioPin7)
//	#define GIO2								Gpio_GetInputIO(GpioPortB, GpioPin6)
//	//#define SCK_0								Gpio_ClrPort(GpioPortB,GpioPin3)
//	//#define SCK_1								Gpio_SetIO(GpioPortB,GpioPin3)
//	//#define SCS_0								Gpio_ClrPort(GpioPortD,GpioPin2)
//	//#define SCS_1								Gpio_SetIO(GpioPortD,GpioPin2)
//	
//	//#define SDIO_0							Gpio_ClrPort(GpioPortB,GpioPin4)
//	//#define SDIO_1							Gpio_SetIO(GpioPortB,GpioPin4)
//												
//	//#define SDIO_GetOutIO				Gpio_ReadOutputIO(GpioPortB,GpioPin4)
//	#define SDIO_GetInIO 				Gpio_GetInputIO(GpioPortC,GpioPin5)
//	
//	
//	#define SCK_0								Gpio_WriteOutputIO(GpioPortB,GpioPin1,FALSE)
//	#define SCK_1								Gpio_WriteOutputIO(GpioPortB,GpioPin1,TRUE)
//	#define SCS_0								Gpio_WriteOutputIO(GpioPortB,GpioPin10,FALSE)
//	#define SCS_1								Gpio_WriteOutputIO(GpioPortB,GpioPin10,TRUE)
//	#define SDIO_0							Gpio_WriteOutputIO(GpioPortC,GpioPin5,FALSE)
//	#define SDIO_1							Gpio_WriteOutputIO(GpioPortC,GpioPin5,TRUE)

unsigned int g_startTickTmp;
unsigned int * g_pSysTick = NULL;
#define SX1276_TICK_COUNT( )                        ( *g_pSysTick )
#define TICK_RATE_MS( ms )                          ( ms )
#define DELAYMS(m)  g_startTickTmp = SX1276_TICK_COUNT( ); \
                    while( ( SX1276_TICK_COUNT( ) - g_startTickTmp ) < TICK_RATE_MS( m ) ); 

static TRFModem       gs_stRfModem;
void SX7139SysTick(unsigned int * tick)
{
    g_pSysTick = tick;
}
/***********************************************************
 * @function_name: gs_WifiMODMDrvIntf
 * @function_file: dev_modem.c
 * @描述:定义各个模块的各种处理函数
 * 
 * @参数: 
 * @返回: 
 * @作者:
 *---------------------------------------------------------
 * @修改人: yzy (2010/11/1)
 **********************************************************/
    //	const TRfodemDrive gs_WifiMODMDrvIntf[] =
    //	{
    //	#if Modem_LSDGTW14_EN > 0
    //	    {
    //	        lsdgtw14_modemcheck,
    //	    
    //	        lsdgtw14_modemon,
    //	        lsdgtw14_modemoff,
    //	
    //	        lsdgtw14_setname,
    //	        lsdgtw14_sta_config,
    //	        lsdgtw14_sta_config_chk,
    //	        lsdgtw14_sta_check_link,
    //	
    //	        lsdgtw14_sta_link_close,
    //	        lsdgtw14_sta_scan,
    //	        lsdgtw14_sta_link_csq,
    //	        lsdgtw14_mdl_reset,
    //	        lsdgtw14_tcpa_connect,
    //	        lsdgtw14_tcpa_close,
    //	        lsdgtw14_tcpa_check_lnk,
    //	        
    //	        lsdgtw14_sta_link_ipcfg,
    //	    },
    //	#endif 
    //	    
    //	    
    //	
    //	};
TRFModemState SYS_RF_Status(void)
{
    return DevRfModem->stt;
}



/*********************************************************************
**  Constant Declaration
*********************************************************************/
#define TIMEOUT     100         //100ms
#define t0hrel      1000        //1ms

/*********************************************************************
**  Global Variable Declaration
*********************************************************************/
uint8_t    timer;
uint8_t    TimeoutFlag;
uint16_t   RxCnt;
uint32_t   Err_ByteCnt;
uint32_t   Err_BitCnt;
uint16_t   TimerCnt0;
uint8_t    *Uartptr;
uint8_t    UartSendCnt;
uint8_t    CmdBuf[11];
uint8_t   tmpbuf[64];
uint8_t	fb;
uint8_t	fb_ok;
static unsigned short TxPacketSize = 0;

const uint8_t BitCount_Tab[16]={0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4};
const uint8_t ID_Tab[8]={0x34,0x75,0xC5,0x8C,0xC7,0x33,0x45,0xE7};   //ID code
const uint8_t PN9_Tab[]=
{   0xFF,0x83,0xDF,0x17,0x32,0x09,0x4E,0xD1,
    0xE7,0xCD,0x8A,0x91,0xC6,0xD5,0xC4,0xC4,
    0x40,0x21,0x18,0x4E,0x55,0x86,0xF4,0xDC,
    0x8A,0x15,0xA7,0xEC,0x92,0xDF,0x93,0x53,
    0x30,0x18,0xCA,0x34,0xBF,0xA2,0xC7,0x59,
    0x67,0x8F,0xBA,0x0D,0x6D,0xD8,0x2D,0x7D,
    0x54,0x0A,0x57,0x97,0x70,0x39,0xD2,0x7A,
    0xEA,0x24,0x33,0x85,0xED,0x9A,0x1D,0xE0,
    0xFF,0x83,0xDF,0x17,0x32,0x09,0x4E,0xD1,
    0xE7,0xCD,0x8A,0x91,0xC6,0xD5,0xC4,0xC4,
    0x40,0x21,0x18,0x4E,0x55,0x86,0xF4,0xDC,
    0x8A,0x15,0xA7,0xEC,0x92,0xDF,0x93,0x53,
    0x30,0x18,0xCA,0x34,0xBF,0xA2,0xC7,0x59,
    0x67,0x8F,0xBA,0x0D,0x6D,0xD8,0x2D,0x7D,
    0x54,0x0A,0x57,0x97,0x70,0x39,0xD2,0x7A,
    0xEA,0x24,0x33,0x85,0xED,0x9A,0x1D,0xE0,
    0xFF,0x83,0xDF,0x17,0x32,0x09,0x4E,0xD1,
    0xE7,0xCD,0x8A,0x91,0xC6,0xD5,0xC4,0xC4,
    0x40,0x21,0x18,0x4E,0x55,0x86,0xF4,0xDC,
    0x8A,0x15,0xA7,0xEC,0x92,0xDF,0x93,0x53,
    0x30,0x18,0xCA,0x34,0xBF,0xA2,0xC7,0x59,
    0x67,0x8F,0xBA,0x0D,0x6D,0xD8,0x2D,0x7D,
    0x54,0x0A,0x57,0x97,0x70,0x39,0xD2,0x7A,
    0xEA,0x24,0x33,0x85,0xED,0x9A,0x1D,0xE0,
    0xFF,0x83,0xDF,0x17,0x32,0x09,0x4E,0xD1,
    0xE7,0xCD,0x8A,0x91,0xC6,0xD5,0xC4,0xC4,
    0x40,0x21,0x18,0x4E,0x55,0x86,0xF4,0xDC,
    0x8A,0x15,0xA7,0xEC,0x92,0xDF,0x93,0x53,
    0x30,0x18,0xCA,0x34,0xBF,0xA2,0xC7,0x59,
    0x67,0x8F,0xBA,0x0D,0x6D,0xD8,0x2D,0x7D,
    0x54,0x0A,0x57,0x97,0x70,0x39,0xD2,0x7A,
    0xEA,0x24,0x33,0x85,0xED,0x9A,0x1D,0xE0
    
};  // This table are 64bytes PN9 pseudo random code.

/*********************************************************************
**  function Declaration
*********************************************************************/
//void Timer0ISR(void);
//void UART0Isr(void);
//void InitTimer0(void);
//void InitUART0(void);
void A7139_POR(void);
//	uint8_t InitRF(void);
uint8_t A7139_Config(void);
uint8_t A7139_WriteID(void);
uint8_t A7139_Cal(void);
//	void StrobeCMD(uint8_t);
void ByteSend(uint8_t);
uint8_t ByteRead(void);
void A7139_WriteReg(uint8_t, uint16_t);
uint16_t A7139_ReadReg(uint8_t);
void A7139_WritePageA(uint8_t, uint16_t);
uint16_t A7139_ReadPageA(uint8_t);
void A7139_WritePageB(uint8_t, uint16_t);
uint16_t A7139_ReadPageB(uint8_t);
void A7139_WriteFIFO(void);
void RxPacket(void);
void Err_State(void);
void entry_deep_sleep_mode(void);
void wake_up_from_deep_sleep_mode(void);
void RCOSC_Cal(void);
void WOR_enable_by_preamble(void);
void WOR_enable_by_sync(void);
void WOR_enable_by_carrier(void);
void WOT_enable(void);
void TWOR_enable(void);

void FIFO_extension_TX(void);
void FIFO_extension_RX(void);
void FIFO_extension_Infinite_TX(void);
void FIFO_extension_Infinite_RX(void);
void Auto_Resend(void);
void Auto_ACK(void);
/*!
 * Local RF buffer for communication support
 */
static unsigned char RFBuffer[RF_BUFFER_SIZE];

static unsigned int PacketTimeout;
/*!
 * Rx management support variables
 */
static unsigned short RxPacketSize = 0;
static char RxPacketSnrEstimate;
static double RxPacketRssiValue;
static unsigned char RxGain = 1;
static unsigned int RxTimeoutTimer = 0;
static unsigned int TxTimeoutTimer = 0;

static uint8_t guc_GPIO1Falling = 0;
static uint8_t guc_GPIO2Falling = 0;
static uint8_t guc_GPIO1Right = 0;
static uint8_t guc_GPIO2Right = 0;

extern volatile uint32 MSR;       
// Default settings
tLoRaSettings LoRaSettings =
{
    474600000,        // RFFrequency
    15,               // Power
    7,                // SignalBw [0: 7.8kHz, 1: 10.4 kHz, 2: 15.6 kHz, 3: 20.8 kHz, 4: 31.2 kHz,
                      // 5: 41.6 kHz, 6: 62.5 kHz, 7: 125 kHz, 8: 250 kHz, 9: 500 kHz, other: Reserved]
    11,                // SpreadingFactor [6: 64, 7: 128, 8: 256, 9: 512, 10: 1024, 11: 2048, 12: 4096  chips]
    1,                // ErrorCoding [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8]
    true,             // CrcOn [0: OFF, 1: ON]
    false,            // ImplicitHeaderOn [0: OFF, 1: ON]
    0,                // RxSingleOn [0: Continuous, 1 Single]
    0,                // FreqHopOn [0: OFF, 1: ON]
    4,                // HopPeriod Hops every frequency hopping period symbols
    300,              // TxPacketTimeout
    800,              // RxPacketTimeout
    128,              // PayloadLength (used for implicit header mode)
    1,
    30,             //preamble
};

/************************************************************************
**
*************************************************************************/
SPIIO gst_RFCommon = 
{
    {CMD_RF_RST,},
    0,
    __NULL,
    0,
    1,
    SPI_CS_1,
    SPI_CS_0,
};

const SPIIO gs_RFRst = 
{
    {CMD_RF_RST,},
    0,
    __NULL,
    0,
    1,
    SPI_CS_1,
    SPI_CS_0,
};
const SPIIO gs_RFSTBY = 
{
    {CMD_STBY,},
    0,
    __NULL,
    0,
    1,
    SPI_CS_1,
    SPI_CS_0,
};
const SPIIO gs_RFSleep = 
{
    {CMD_SLEEP,},
    0,
    __NULL,
    0,
    1,
    SPI_CS_1,
    SPI_CS_0,
};
const SPIIO gs_RFDeepSleep = 
{
    {CMD_DEEP_SLEEP,},
    0,
    __NULL,
    0,
    1,
    SPI_CS_1,
    SPI_CS_0,
};
const SPIIO gs_RFCmdTx = 
{
    {CMD_TX,},
    0,
    __NULL,
    0,
    1,
    SPI_CS_1,
    SPI_CS_0,
};
    
const SPIIO gs_RFCmdRx = 
{
    {CMD_RX,},
    0,
    __NULL,
    0,
    1,
    SPI_CS_1,
    SPI_CS_0,
};
const SPIIO gs_RFCmdTfr = 
{
    {CMD_TFR,},
    0,
    __NULL,
    0,
    1,
    SPI_CS_1,
    SPI_CS_0,
};
const SPIIO gs_RFCmdRfr = 
{
    {CMD_RFR,},
    0,
    __NULL,
    0,
    1,
    SPI_CS_1,
    SPI_CS_0,
};
const SPIIO gs_RFCmdSleep = 
{
    {CMD_SLEEP,},
    0,
    __NULL,
    0,
    1,
    SPI_CS_1,
    SPI_CS_0,
};

/*********************************************************************
* 
//超时切换模式



*********************************************************************/
uint16_t SYS_A7139_Recv(uint8_t * data)
{

    SPI_Write((SPIIO*)&gs_RFCmdRfr, &gs_RFSpiPort);


    SPIIO * rfSPI = &gst_RFCommon;

    rfSPI->command[0] = CMD_FIFO_R | CMD_Reg_R;  
    rfSPI->dev = 0;
    rfSPI->data = data;
    rfSPI->cmdnum = 1;
    rfSPI->length = 2;
    SPI_Read((SPIIO*)rfSPI, &gs_RFSpiPort);

    uint16_t len = data[1];
    
    rfSPI->cmdnum = 1;
    rfSPI->length = len - 2;
    rfSPI->data = &data[2];
    if(rfSPI->length > CON_FIFO_LEN)
        return 0;
    SPI_Read((SPIIO*)rfSPI, &gs_RFSpiPort);

    return len;
}
uint8_t SYS_A7139SetTxPacket( const void *buffer, unsigned short size )
{
    TxPacketSize = size;
    memcpy( ( void * )RFBuffer, buffer, ( size_t )TxPacketSize ); 

    //RFLRState = RFLR_STATE_TX_INIT;
    
    MSR = TX_STATE_BIT | RFLR_STATE_TX_INIT;//设状态机   
    
    return 0;
}

uint8_t SYS_A7139_Send(uint8_t * data, uint16_t len)
{
//	    uint8_t i;

    //TX FIFO address pointer reset
    SPI_Write((SPIIO*)&gs_RFCmdTfr, &gs_RFSpiPort);//TX FIFO address pointer reset


    
    SPIIO * rfSPI = &gst_RFCommon;
    rfSPI->command[0] = CMD_FIFO_W;  
    
    rfSPI->dev = 0;
    rfSPI->data = data;
    rfSPI->cmdnum = 1;
    rfSPI->length = len;
    SPI_Write((SPIIO*)rfSPI, &gs_RFSpiPort);//TX FIFO write command

    SPI_Write((SPIIO*)&gs_RFCmdTx, &gs_RFSpiPort);

//	    while(SYS_GPI_GetLPort(GPI_DIO2));

    SYS_OK();
}

//uint8_t master_slave=0;
uint8_t ledfleg=0;
uint16_t timefleg=0,timefleg2=0;
uint8_t gio2111=0;
void SYS_A7139_Proc(uint8_t mod)
{
//	    SYS_RF_Init();
    Init_SPI(&gs_RFSpiPort);

    	//power on only
    SYS_RF_Init(0,0,0);

//    master_slave=0;
    if(mod == 1)   // master
    {
//	        SYS_RF_Init();      //init RF
        while(1)
        {

			A7139_WriteFIFO();  //write data to TX FIFO
//						StrobeCMD(CMD_TX);
            
            SPI_Write((SPIIO*)&gs_RFCmdTx, &gs_RFSpiPort);
//					WOR_enable_by_sync();

//				while(!GIO2);
//				while(GIO2);        //wait transmit completed
            while(!SYS_GPI_GetLPort(GPI_DIO2)) msleep(10);
            while(SYS_GPI_GetLPort(GPI_DIO2)) msleep(10);

//				StrobeCMD(CMD_RX);
            
            SPI_Write((SPIIO*)&gs_RFCmdRx, &gs_RFSpiPort);
			timefleg=0;
			while(!SYS_GPI_GetLPort(GPI_DIO2)) msleep(10);
			while(SYS_GPI_GetLPort(GPI_DIO2))
			{
				timefleg++;
//					delay1ms(10);
                msleep(10);
				if(timefleg>100)
				{
					timefleg=0;
					timefleg2=1;
					
					break;
				}
//					Gpio_WriteOutputIO(GpioPortD, GpioPin5,1);
				
			}
            if(timefleg2==1)
            {
//					StrobeCMD(CMD_STBY);
                SPI_Write((SPIIO*)&gs_RFSTBY, &gs_RFSpiPort);

                timefleg=0;
                timefleg2=0;
            }		
            else 
			{
				timefleg=0;
				RxPacket();
				ledfleg=!ledfleg;
//					Gpio_WriteOutputIO(GpioPortD, GpioPin5,ledfleg);
//					delay1ms(100);
                msleep(10);
//                SPI_Write((SPIIO*)&gs_RFSTBY, &gs_RFSpiPort);
			}

			msleep(10);



        }
    }
    else    // slave
    {
//	        SYS_RF_Init();      //init RF
        RxCnt = 0;
        Err_ByteCnt = 0;
        Err_BitCnt = 0;

        while(1)
        {
//	            StrobeCMD(CMD_RX);
            
            SPI_Write((SPIIO*)&gs_RFCmdRx, &gs_RFSpiPort);
//				while(!GIO2);
//	            while(GIO2);        //wait receive completed
			while(!SYS_GPI_GetLPort(GPI_DIO2)) msleep(10);
			while(SYS_GPI_GetLPort(GPI_DIO2)) msleep(10);
            
            RxPacket();
			msleep (1);
		
			A7139_WriteFIFO();  //write data to TX FIFO
//							StrobeCMD(CMD_TX);
            SPI_Write((SPIIO*)&gs_RFCmdTx, &gs_RFSpiPort);

//					WOR_enable_by_sync();
//							while(!GIO2);
//							while(GIO2);        //wait transmit completed
            while(!SYS_GPI_GetLPort(GPI_DIO2));
            while(SYS_GPI_GetLPort(GPI_DIO2));
					
			ledfleg=!ledfleg;
//							Gpio_WriteOutputIO(GpioPortD, GpioPin5,ledfleg);
        }
    }

}






/*********************************************************************
** Strobe Command
*********************************************************************/
//	void StrobeCMD(uint8_t cmd)
//	{
//	    uint8_t i;
//	
//	    SCS_0;
//	    for(i=0; i<8; i++)
//	    {
//	        if(cmd & 0x80)
//	            SDIO_1;
//	        else
//	            SDIO_0;
//	
//	        __ASM("NOP");
//	        SCK_1;
//	        __ASM("NOP");
//	        SCK_0;
//	        cmd<<=1;
//	    }
//	    SCS_1;
//	}

/************************************************************************
**  ByteSend
************************************************************************/
//	void ByteSend(uint8_t src)
//	{
//	    uint8_t i;
//	
//	    for(i=0; i<8; i++)
//	    {
//	        if(src & 0x80)
//	            SDIO_1;
//	        else
//	            SDIO_0;
//	
//	        __ASM("NOP");
//	        SCK_1;
//	        __ASM("NOP");
//	        SCK_0;
//	        src<<=1;
//	    }
//	}
//	
//	/************************************************************************
//	**  ByteRead
//	************************************************************************/
//	uint8_t ByteRead(void)
//	{
//	    uint8_t i,tmp;
//		stc_gpio_cfg_t ByteRead_stcGpioCfg;
//	//SDIO设置为输入----------------------------------------------------------------------------------------
//		ByteRead_stcGpioCfg.enDir=GpioDirIn;Gpio_Init(GpioPortC, GpioPin5, &ByteRead_stcGpioCfg);
//	    for(i=0; i<8; i++)
//	    {
//	        if(SDIO_GetInIO)
//	            tmp = (tmp << 1) | 0x01;
//	        else
//	            tmp = tmp << 1;
//	
//	        SCK_1;
//	        __ASM("NOP"); 
//	        SCK_0;
//	        __ASM("NOP");
//	    }
//			//SDIO设置为输出----------------------------------------------------------------------------------------
//			ByteRead_stcGpioCfg.enDir=GpioDirOut;Gpio_Init(GpioPortC, GpioPin5, &ByteRead_stcGpioCfg);
//	    return tmp;
//	}

/************************************************************************
**  A7139_WriteReg
************************************************************************/
void A7139_WriteReg(uint8_t address, uint16_t dataWord)
{


    SPIIO * rfSPI = &gst_RFCommon;
    uint8_t data[10];
    rfSPI->command[0] = address;  
    rfSPI->command[1] = dataWord >> 8;
    rfSPI->command[2] = dataWord;
//      rfSPI->command[3] = ID_Tab[2];
//      rfSPI->command[4] = ID_Tab[3];
    
    rfSPI->dev = 0;
    rfSPI->data = __NULL;
    rfSPI->cmdnum = 3;
    rfSPI->length = 0;
    SPI_Write((SPIIO*)rfSPI, &gs_RFSpiPort);


}

/************************************************************************
**  A7139_ReadReg
************************************************************************/
uint16_t A7139_ReadReg(uint8_t address)
{
    uint8_t i;
    uint16_t tmp;

    SPIIO * rfSPI = &gst_RFCommon;
    uint8_t data[10];

    rfSPI->command[0] = address | CMD_Reg_R;  
//              rfSPI->command[1] = tmp >> 8;
//              rfSPI->command[2] = tmp;
//      rfSPI->command[3] = ID_Tab[2];
//      rfSPI->command[4] = ID_Tab[3];
    
    rfSPI->dev = 0;
    rfSPI->data = data;
    rfSPI->cmdnum = 1;
    rfSPI->length = 2;
    SPI_Read((SPIIO*)rfSPI, &gs_RFSpiPort);
    tmp = data[0] * 0x100 + data[1];


    return tmp;
}

/************************************************************************
**  A7139_WritePageA
************************************************************************/
void A7139_WritePageA(uint8_t address, uint16_t dataWord)
{
    uint16_t tmp;

    tmp = address;
    tmp = ((tmp << 12) | A7139Config[CRYSTAL_REG]);
    A7139_WriteReg(CRYSTAL_REG, tmp);
    A7139_WriteReg(PAGEA_REG, dataWord);
}

/************************************************************************
**  A7139_ReadPageA
************************************************************************/
uint16_t A7139_ReadPageA(uint8_t address)
{
    uint16_t tmp;

    tmp = address;
    tmp = ((tmp << 12) | A7139Config[CRYSTAL_REG]);
    A7139_WriteReg(CRYSTAL_REG, tmp);
//	    SPIIO rfSPI;
//	    uint8_t data[10];
//	    rfSPI->command[0] = CRYSTAL_REG;  
//	    rfSPI->command[1] = tmp>>8;
//	    rfSPI->command[2] = tmp;
//	//      rfSPI->command[3] = ID_Tab[2];
//	//      rfSPI->command[4] = ID_Tab[3];
//	    
//	    rfSPI->dev = 0;
//	    rfSPI->data = __NULL;
//	    rfSPI->cmdnum = 3;
//	    rfSPI->length = 0;
//	    SPI_Write((SPIIO*)rfSPI, &gs_RFSpiPort);

    tmp = A7139_ReadReg(PAGEA_REG);

//	    rfSPI->command[0] = PAGEA_REG | CMD_Reg_R;  
//	//              rfSPI->command[1] = tmp >> 8;
//	//              rfSPI->command[2] = tmp;
//	//      rfSPI->command[3] = ID_Tab[2];
//	//      rfSPI->command[4] = ID_Tab[3];
//	    
//	    rfSPI->dev = 0;
//	    rfSPI->data = data;
//	    rfSPI->cmdnum = 1;
//	    rfSPI->length = 2;
//	    SPI_Read((SPIIO*)rfSPI, &gs_RFSpiPort);
//	    tmp = data[0] * 0x100 + data[1];

    
    return tmp;
}

/************************************************************************
**  A7139_WritePageB
************************************************************************/
void A7139_WritePageB(uint8_t address, uint16_t dataWord)
{
    uint16_t tmp;

    tmp = address;
    tmp = ((tmp << 7) | A7139Config[CRYSTAL_REG]);
    A7139_WriteReg(CRYSTAL_REG, tmp);
    A7139_WriteReg(PAGEB_REG, dataWord);
}

/************************************************************************
**  A7139_ReadPageB
************************************************************************/
uint16_t A7139_ReadPageB(uint8_t address)
{
    uint16_t tmp;

    tmp = address;
    tmp = ((tmp << 7) | A7139Config[CRYSTAL_REG]);
    A7139_WriteReg(CRYSTAL_REG, tmp);
    tmp = A7139_ReadReg(PAGEB_REG);
    return tmp;
}

/*********************************************************************
** A7139_POR
*********************************************************************/
void A7139_POR(void)
{
	//power on only
    msleep(1);   			//for regulator settling time (power on only)
    
//	    StrobeCMD(CMD_RF_RST);  	//reset A7139 chip
    SPI_Write((SPIIO*)&gs_RFRst, &gs_RFSpiPort);

    while(A7139_WriteID())		//check SPI
    {
//			StrobeCMD(CMD_RF_RST);  //reset A7139 chip    
		
        SPI_Write((SPIIO*)&gs_RFRst, &gs_RFSpiPort);
    }
    A7139_WritePageA(PM_PAGEA, A7139Config_PageA[PM_PAGEA] | 0x1000);   //STS=1
    msleep(1);
     
    entry_deep_sleep_mode();		//deep sleep
    msleep(1);
    wake_up_from_deep_sleep_mode();	//wake up
    
//	    StrobeCMD(CMD_RF_RST);  	//reset A7139 chip
    SPI_Write((SPIIO*)&gs_RFRst, &gs_RFSpiPort);
    while(A7139_WriteID())		//check SPI
    {
//			StrobeCMD(CMD_RF_RST);  //reset A7139 chip
		SPI_Write((SPIIO*)&gs_RFRst, &gs_RFSpiPort);
    }
    A7139_WritePageA(PM_PAGEA, A7139Config_PageA[PM_PAGEA] | 0x1000);   //STS=1
    msleep(1);
}




/*********************************************************************
** InitRF
*********************************************************************/
uint8_t SYS_RF_Init(int freqCode, unsigned char ch, unsigned char pwr )
{
//    //initial pin
//    SCS_1;
//    SCK_0;
//    SDIO_1;

//    GIO1=1;
//    GIO2=1;
    Init_SPI(&gs_RFSpiPort);
    
    A7139_POR();
    
    msleep(1);            //delay 1ms for regulator stabilized
//	    StrobeCMD(CMD_RF_RST);  //reset A7139 chip
    SPI_Write((SPIIO*)&gs_RFRst, &gs_RFSpiPort);
    msleep(1);
    
    if(A7139_Config())      //config A7139 chip
        return 1;

    msleep(1);          //delay 800us for crystal stabilized

    if(A7139_WriteID())     //write ID code
        return 1;

    if(A7139_Cal())         //IF and VCO Calibration
        return 1;

    return 0;
}

uint8_t SYS_RF_Reset(void)
{
    if(A7139_Config())      //config A7139 chip
        return 1;

    msleep(1);          //delay 800us for crystal stabilized

    if(A7139_WriteID())     //write ID code
        return 1;

//	    if(A7139_Cal())         //IF and VCO Calibration
//	        return 1;

    return 0;

}

/*********************************************************************
** A7139_Config
*********************************************************************/
uint8_t A7139_Config(void)
{
    uint8_t i;
    uint16_t tmp;
    SPIIO * rfSPI = &gst_RFCommon;



    for(i=0; i<8; i++)
    {
//	        
//	        rfSPI->command[0] = i;  
//	        rfSPI->command[1] = A7139Config[i] >> 8;
//	        rfSPI->command[2] = A7139Config[i] & 0xff;
//	
//	        rfSPI->dev = 0;
//	        rfSPI->data = __NULL;
//	        rfSPI->cmdnum = 3;
//	        rfSPI->length = 0;
//	        SPI_Write((SPIIO*)rfSPI, &gs_RFSpiPort);
        A7139_WriteReg(i, A7139Config[i]);
    }

    for(i=10; i<16; i++)
	{	
//		    uint16_t dd = 0;
//			if((i == 14) && (fb_ok == 1))
//				dd = A7139Config[i] | (1<<4);//A7139_WriteReg(i, A7139Config[i] | (1<<4));			//MIFS=1(Manual)
//			else
//				dd = A7139Config[i];//A7139_WriteReg(i, A7139Config[i]);
//	
//	        
//	        rfSPI->command[0] = i;  
//	        rfSPI->command[1] = dd >> 8;
//	        rfSPI->command[2] = dd & 0xff;
//	        
//	        rfSPI->dev = 0;
//	        rfSPI->data = __NULL;
//	        rfSPI->cmdnum = 3;
//	        rfSPI->length = 0;
//	        SPI_Write((SPIIO*)rfSPI, &gs_RFSpiPort);
        if((i == 14) && (fb_ok == 1))
            A7139_WriteReg(i, A7139Config[i] | (1<<4));         //MIFS=1(Manual)
        else
            A7139_WriteReg(i, A7139Config[i]);

	}	

    for(i=0; i<16; i++)
        A7139_WritePageA(i, A7139Config_PageA[i]);

    for(i=0; i<5; i++)
        A7139_WritePageB(i, A7139Config_PageB[i]);

    //for check        
    tmp = A7139_ReadReg(SYSTEMCLOCK_REG);
    if(tmp != A7139Config[SYSTEMCLOCK_REG])
    {
        return 1;   
    }
    
    return 0;

//	    //for check    
//	    uint8_t data[2]={0,0};
//	    rfSPI->command[0] = SYSTEMCLOCK_REG | CMD_Reg_R;  
//	//	    rfSPI->command[1] = tmp >> 8;
//	//	    rfSPI->command[2] = tmp & 0xff;
//	
//	    rfSPI->dev = 0;
//	    rfSPI->data = data;
//	    rfSPI->cmdnum = 1;
//	    rfSPI->length = 2;
//	    SPI_Read((SPIIO*)rfSPI, &gs_RFSpiPort);
//	
//	    tmp = data[0] * 0x100 + data[1];
//	//	    tmp = A7139_ReadReg(SYSTEMCLOCK_REG);
//	    if(tmp != A7139Config[SYSTEMCLOCK_REG])
//	    {
//	        return 1;   
//	    }
//	    
//	    return 0;
}

/************************************************************************
**  WriteID
************************************************************************/
uint8_t d1, d2, d3, d4;
uint8_t A7139_WriteID(void)
{
    uint8_t i;
    uint8_t di[4];
    SPIIO * rfSPI = &gst_RFCommon;
//    uint8_t d1, d2, d3, d4;

//	    SCS_0;
//	    ByteSend(CMD_ID_W);
//	    for(i=0; i<4; i++)
//	        ByteSend(ID_Tab[i]);
//	    SCS_1;
    rfSPI->command[0] = CMD_ID_W;  
    rfSPI->command[1] = ID_Tab[0];
    rfSPI->command[2] = ID_Tab[1];
    rfSPI->command[3] = ID_Tab[2];
    rfSPI->command[4] = ID_Tab[3];
    
    rfSPI->dev = 0;
    rfSPI->data = __NULL;
    rfSPI->cmdnum = 5;
    rfSPI->length = 0;
    SPI_Write((SPIIO*)rfSPI, &gs_RFSpiPort);

//	    SCS_0;
//	    ByteSend(CMD_ID_R);
//	    d1=ByteRead();
//	    d2=ByteRead();
//	    d3=ByteRead();
//	    d4=ByteRead();
//	    SCS_1;

    rfSPI->command[0] = CMD_ID_R | CMD_Reg_R;  
//	    rfSPI->command[1] = ID_Tab[0];
//	    rfSPI->command[2] = ID_Tab[1];
//	    rfSPI->command[3] = ID_Tab[2];
//	    rfSPI->command[4] = ID_Tab[3];
    
    rfSPI->dev = 0;
    rfSPI->data = di;
    rfSPI->cmdnum = 1;
    rfSPI->length = 4;
    SPI_Read((SPIIO*)rfSPI, &gs_RFSpiPort);


    if((di[0]!=ID_Tab[0]) || (di[1]!=ID_Tab[1]) || (di[2]!=ID_Tab[2]) || (di[3]!=ID_Tab[3]))
    {
        return 1;
    }
    
    return 0;
}

/*********************************************************************
** A7139_Cal
*********************************************************************/
 uint8_t vb,vbcf;
 uint8_t fb_old, fcd, fbcf;  	//IF Filter
uint8_t A7139_Cal(void)
{
    uint8_t i;
//    uint8_t fb_old, fcd, fbcf;  	//IF Filter
//    uint8_t vb,vbcf;          	//VCO Current
    uint8_t vcb, vccf;        	//VCO Band
    uint16_t tmp;
    uint8_t fb_fail;
	
//	    StrobeCMD(CMD_STBY);
//	    SPIIO rfSPI;
//	    uint8_t data[10];
//	    rfSPI->command[0] = CMD_STBY;  
//	//	    rfSPI->command[1] = ID_Tab[0];
//	//	    rfSPI->command[2] = ID_Tab[1];
//	//	    rfSPI->command[3] = ID_Tab[2];
//	//	    rfSPI->command[4] = ID_Tab[3];
//	    
//	    rfSPI->dev = 0;
//	    rfSPI->data = __NULL;
//	    rfSPI->cmdnum = 1;
//	    rfSPI->length = 0;
//	    SPI_Write((SPIIO*)rfSPI, &gs_RFSpiPort);
    SPI_Write((SPIIO*)&gs_RFSTBY, &gs_RFSpiPort);

    //IF calibration procedure @STB state
	if(fb_ok == 1)
	{
		A7139_WriteReg(MODE_REG, A7139Config[MODE_REG] | 0x0800);       //VCO Current Calibration
	    do{
            tmp = A7139_ReadReg(MODE_REG);
	    }while(tmp & 0x0800);
		tmp = (A7139Config[CALIBRATION_REG] & 0xFFE0);
		tmp = tmp | fb | (1<<4);
		A7139_WriteReg(CALIBRATION_REG, tmp);
	}	
	else
	{	
        fb_fail=0;

        for(i=0;i<3;i++)
        {
            A7139_WriteReg(MODE_REG, A7139Config[MODE_REG] | 0x0802);       //IF Filter & VCO Current Calibration
            do{
                tmp = A7139_ReadReg(MODE_REG);
            }while(tmp & 0x0802);

            //for check(IF Filter)
            tmp = A7139_ReadReg(CALIBRATION_REG);
            fb = tmp & 0x0F;
            fcd = (tmp>>11) & 0x1F;
            fbcf = (tmp>>4) & 0x01;

            if((fb<4) || (fb>8))
                fb_fail = 1;
            else
            {		  
                if(i==0)
                    fb_old = fb;
                else
                {	
                    if(fb != fb_old)
                    fb_fail = 1;
                }
            }

            if((fbcf) || (fb_fail))
            {
                return 1;
            }
        }
	}
	 
    //for check(VCO Current)
    tmp = A7139_ReadPageA(VCB_PAGEA);
    vcb = tmp & 0x0F;
    vccf = (tmp>>4) & 0x01;
    if(vccf)
    {
        return 1;
    }

	 
    //RSSI Calibration procedure @STB state
    A7139_WriteReg(ADC_REG, 0x4C00);                                //set ADC average=64
    A7139_WriteReg(MODE_REG, A7139Config[MODE_REG] | 0x1000);       //RSSI Calibration
    do{
        tmp = A7139_ReadReg(MODE_REG);
    }while(tmp & 0x1000);
    A7139_WriteReg(ADC_REG, A7139Config[ADC_REG]);


    //VCO calibration procedure @STB state
//	    for(i=0; i<3; i++)
//	    {
//	        A7139_WriteReg(PLL1_REG, Freq_Cal_Tab[i*2]);
//	        A7139_WriteReg(PLL2_REG, Freq_Cal_Tab[i*2+1]);
        A7139_WriteReg(PLL1_REG, A7139Config[PLL1_REG]);
        A7139_WriteReg(PLL2_REG, A7139Config[PLL2_REG]);

        A7139_WriteReg(MODE_REG, A7139Config[MODE_REG] | 0x0004);   //VCO Band Calibration
        do{
            tmp = A7139_ReadReg(MODE_REG);
        }while(tmp & 0x0004);
    
        //for check(VCO Band)
        tmp = A7139_ReadReg(CALIBRATION_REG);
        vb = (tmp >>5) & 0x07;
        vbcf = (tmp >>8) & 0x01;
        if(vbcf)
        {
            return 1;
        }
//	    }

    
	fb_ok = 1;
    return 0;
}

/*********************************************************************
** A7139_WriteFIFO
*********************************************************************/
void A7139_WriteFIFO(void)
{
    uint8_t i;

//	    StrobeCMD(CMD_TFR);     //TX FIFO address pointer reset
    SPI_Write((SPIIO*)&gs_RFCmdTfr, &gs_RFSpiPort);//TX FIFO address pointer reset
//	    SCS_0;
//	    ByteSend(CMD_FIFO_W);   //TX FIFO write command
//	    for(i=0; i <64; i++)
//	        ByteSend(PN9_Tab[i]);
//	    SCS_1;
    
    SPIIO * rfSPI = &gst_RFCommon;
//	    uint8_t data[10];
    rfSPI->command[0] = CMD_FIFO_W;  
//	    rfSPI->command[1] = dataWord >> 8;
//	    rfSPI->command[2] = dataWord;
//      rfSPI->command[3] = ID_Tab[2];
//      rfSPI->command[4] = ID_Tab[3];
    
    rfSPI->dev = 0;
    rfSPI->data = PN9_Tab;
    rfSPI->cmdnum = 1;
    rfSPI->length = 64;
    SPI_Write((SPIIO*)rfSPI, &gs_RFSpiPort);

    
}

/*********************************************************************
** RxPacket
*********************************************************************/
void RxPacket(void)
{
    uint8_t i;
    uint8_t recv;
    uint8_t tmp;

    RxCnt++;

//	    StrobeCMD(CMD_RFR);     //RX FIFO address pointer reset
    SPI_Write((SPIIO*)&gs_RFCmdRfr, &gs_RFSpiPort);

//	    SCS_0;
//	    ByteSend(CMD_FIFO_R);   //RX FIFO read command
//	    for(i=0; i <64; i++)
//	    {
//	        tmpbuf[i] = ByteRead();
//	    }
//	    SCS_1;

    SPIIO * rfSPI = &gst_RFCommon;
//	    uint8_t data[10];

    rfSPI->command[0] = CMD_FIFO_R | CMD_Reg_R;  
//              rfSPI->command[1] = tmp >> 8;
//              rfSPI->command[2] = tmp;
//      rfSPI->command[3] = ID_Tab[2];
//      rfSPI->command[4] = ID_Tab[3];
    memset(tmpbuf,0,64);
    rfSPI->dev = 0;
    rfSPI->data = tmpbuf;
    rfSPI->cmdnum = 1;
    rfSPI->length = 64;
    SPI_Read((SPIIO*)rfSPI, &gs_RFSpiPort);


    for(i=0; i<64; i++)
    {
        recv = tmpbuf[i];
        tmp = recv ^ PN9_Tab[i];
        if(tmp!=0)
        {
            Err_ByteCnt++;
            Err_BitCnt += (BitCount_Tab[tmp>>4] + BitCount_Tab[tmp & 0x0F]);
        }
    }
}

/*********************************************************************
** Err_State
*********************************************************************/
void Err_State(void)
{
    //ERR display
    //Error Proc...
    //...
    while(1);
}

/*********************************************************************
** entry_deep_sleep_mode进入深度睡眠
*********************************************************************/
void entry_deep_sleep_mode(void)
{
//	    StrobeCMD(CMD_RF_RST);              //RF reset
    SPI_Write((SPIIO*)&gs_RFRst, &gs_RFSpiPort);

    A7139_WriteReg(PIN_REG, A7139Config[PIN_REG] | 0x0800);             //SCMDS=1
    A7139_WritePageA(PM_PAGEA, A7139Config_PageA[PM_PAGEA] | 0x1010);   //STS=1, QDS=1
//	    StrobeCMD(CMD_SLEEP);               //entry sleep mode
//	    delay100us(6);                      //delay 600us for VDD_A shutdown, C load=0.1uF
    msleep(1);
    SPI_Write((SPIIO*)&gs_RFSleep, &gs_RFSpiPort);
    
//			StrobeCMD(CMD_DEEP_SLEEP);          //entry deep sleep mode
		
            
    SPI_Write((SPIIO*)&gs_RFDeepSleep, &gs_RFSpiPort);//entry deep sleep mode
//	    delay100us(2);                      //delay 200us for VDD_D shutdown, C load=0.1uF
    msleep(1);
}

/*********************************************************************
** wake_up_from_deep_sleep_mode从睡眠模式中唤醒
*********************************************************************/
void wake_up_from_deep_sleep_mode(void)
{
//	    StrobeCMD(CMD_STBY);    //wake up
    
    SPI_Write((SPIIO*)&gs_RFSTBY, &gs_RFSpiPort);
//	    delay1ms(2);            //delay 2ms for VDD_D stabilized
    //InitRF();
    msleep(2);
}

/*********************************************************************
** RC Oscillator Calibration     RC振荡器校准
*********************************************************************/
void RCOSC_Cal(void)
{
    uint16_t tmp;

    A7139_WritePageA(WOR2_PAGEA, A7139Config_PageA[WOR2_PAGEA] | 0x0010);       //enable RC OSC

    while(1)
    {
        A7139_WritePageA(WCAL_PAGEA, A7139Config_PageA[WCAL_PAGEA] | 0x0001);   //set ENCAL=1 to start RC OSC CAL
        do{
            tmp = A7139_ReadPageA(WCAL_PAGEA);
        }while(tmp & 0x0001);
            
        tmp = (A7139_ReadPageA(WCAL_PAGEA) & 0x03FF);       //read NUMLH[8:0]
        tmp >>= 1;  
        
        if((tmp > 183) && (tmp < 205))      //NUMLH[8:0]=194+-10 (PF8M=6.4M)
        //if((tmp > 232) && (tmp < 254))    //NUMLH[8:0]=243+-10 (PF8M=8M)
        {
            break;
        }
    }
}

/*********************************************************************
** WOR_enable_by_preamble       通过前导码启用WOR
*********************************************************************/
void WOR_enable_by_preamble(void)
{
//    StrobeCMD(CMD_STBY);
    SPI_Write((SPIIO*)&gs_RFSTBY, &gs_RFSpiPort);

    RCOSC_Cal();        //RC Oscillator Calibration
    
    A7139_WritePageA(GIO_PAGEA, (A7139Config_PageA[GIO_PAGEA] & 0xF000) | 0x004D);  //GIO1=PMDO, GIO2=WTR

    //Real WOR Active Period = (WOR_AC[5:0]+1) x 244us  X'TAL and Regulator Settling Time
    //Note : Be aware that Xˇtal settling time requirement includes initial tolerance, 
    //       temperature drift, aging and crystal loading.
    A7139_WritePageA(WOR1_PAGEA, 0x8005);   //setup WOR Sleep time and Rx time
    
    A7139_WritePageA(WOR2_PAGEA, A7139Config_PageA[WOR2_PAGEA] | 0x0030);   //enable RC OSC & WOR by preamble
    A7139_WriteReg(MODE_REG, A7139Config[MODE_REG] | 0x0200);               //WORE=1 to enable WOR function
    
    while(!SYS_GPI_GetLPort(GPI_DIO1));
//while(GIO1==0);     //Stay in WOR mode until receiving preamble code(preamble detect ok)
}

/*********************************************************************
** WOR_enable_by_sync
*********************************************************************/
void WOR_enable_by_sync(void)
{
//    StrobeCMD(CMD_STBY);
    SPI_Write((SPIIO*)&gs_RFSTBY, &gs_RFSpiPort);

    RCOSC_Cal();        //RC Oscillator Calibration
    
    A7139_WritePageA(GIO_PAGEA, (A7139Config_PageA[GIO_PAGEA] & 0xF000) | 0x0045);  //GIO1=FSYNC, GIO2=WTR

    //Real WOR Active Period = (WOR_AC[5:0]+1) x 244us  X'TAL and Regulator Settling Time
    //Note : Be aware that Xˇtal settling time requirement includes initial tolerance, 
    //       temperature drift, aging and crystal loading.
    A7139_WritePageA(WOR1_PAGEA, 0x8005);   //setup WOR Sleep time and Rx time
    
    A7139_WritePageA(WOR2_PAGEA, A7139Config_PageA[WOR2_PAGEA] | 0x0010);   //enable RC OSC & WOR by sync
    A7139_WriteReg(MODE_REG, A7139Config[MODE_REG] | 0x0200);               //WORE=1 to enable WOR function
    
    while(!SYS_GPI_GetLPort(GPI_DIO1));//while(GIO1==0);     //Stay in WOR mode until receiving ID code(ID detect ok)
    
    /*
                        ____    ____    ____    ____    ____
    TX WTR       ______|    |__|    |__|    |__|    |__|    |__
                            ______   _______
    WOR WTR      __________|      |_|       |__________________
                                 _       ___
    WOR FSYNC    _______________| |_____|   |__________________
                                □ □ □       □
                                □ □ □       receive completed
                                □ □ entry RX mode
                                □ disable WOR
                                sync ok 
    */
    
    A7139_WriteReg(MODE_REG, A7139Config[MODE_REG] & ~0x0200);              //WORE=0 to disable WOR function
//    StrobeCMD(CMD_STBY);
    SPI_Write((SPIIO*)&gs_RFSTBY, &gs_RFSpiPort);

//    StrobeCMD(CMD_RX);
    SPI_Write((SPIIO*)&gs_RFCmdRx, &gs_RFSpiPort);

//	    while(GIO2==0);		//wait RX ready
//	    while(GIO2==1);     //wait receive completed
    while(!SYS_GPI_GetLPort(GPI_DIO2));
    while(SYS_GPI_GetLPort(GPI_DIO2));
    
}

/*********************************************************************
** WOR_enable_by_carrier 从carrier启用WOR
*********************************************************************/
void WOR_enable_by_carrier(void)
{
//    StrobeCMD(CMD_STBY);
    SPI_Write((SPIIO*)&gs_RFSTBY, &gs_RFSpiPort);
    
    RCOSC_Cal();        //RC Oscillator Calibration
    
    A7139_WritePageA(GIO_PAGEA, (A7139Config_PageA[GIO_PAGEA] & 0xF000) | 0x0049);  //GIO1=CD, GIO2=WTR

    //Real WOR Active Period = (WOR_AC[5:0]+1) x 244us  X'TAL and Regulator Settling Time
    //Note : Be aware that Xˇtal settling time requirement includes initial tolerance, 
    //       temperature drift, aging and crystal loading.
    A7139_WritePageA(WOR1_PAGEA, 0x8005);   //setup WOR Sleep time and Rx time
    
    A7139_WritePageA(WOR2_PAGEA, A7139Config_PageA[WOR2_PAGEA] | 0x0410);   //enable RC OSC & WOR by carrier
    A7139_WritePageA(RFI_PAGEA, A7139Config_PageA[RFI_PAGEA] & ~0x6000);    //select RSSI Carrier Detect
    A7139_WriteReg(ADC_REG, A7139Config[ADC_REG] | 0x8069);                 //ARSSI=1, RTH=105(-100dBm)
    A7139_WriteReg(MODE_REG, A7139Config[MODE_REG] | 0x0200);               //WORE=1 to enable WOR function
    
    while(!SYS_GPI_GetLPort(GPI_DIO1));//while(GIO1==0);     //Stay in WOR mode until carrier signal strength is greater than the value set by RTH[7:0](carrier detect ok)
}

/*********************************************************************
** WOT_enable--使能WOT
*********************************************************************/
void WOT_enable(void)
{
//    StrobeCMD(CMD_STBY);
    SPI_Write((SPIIO*)&gs_RFSTBY, &gs_RFSpiPort);
    
    RCOSC_Cal();            //RC Oscillator Calibration
    
    A7139_WritePageA(GIO_PAGEA, (A7139Config_PageA[GIO_PAGEA] & 0xF000) | 0x0045);  //GIO1=FSYNC, GIO2=WTR

    A7139_WritePageA(WOR1_PAGEA, 0x0005);   //setup WOT Sleep time
    
    A7139_WriteFIFO();      //write data to TX FIFO

    A7139_WriteReg(PIN_REG, A7139Config[PIN_REG] | 0x0400);     //WMODE=1 select WOT function
    A7139_WriteReg(MODE_REG, A7139Config[MODE_REG] | 0x0200);   //WORE=1 to enable WOT function
    
    while(1);
}

/*********************************************************************
** TWOR_enable------使能TWOR
*********************************************************************/
void TWOR_enable(void)
{
//    StrobeCMD(CMD_STBY); 
    SPI_Write((SPIIO*)&gs_RFSTBY, &gs_RFSpiPort);
    
    RCOSC_Cal();            //RC Oscillator Calibration
    
    A7139_WritePageA(GIO_PAGEA, (A7139Config_PageA[GIO_PAGEA] & 0xF000) | 0x0051);  //GIO1=TWOR, GIO2=WTR

    A7139_WritePageA(WOR1_PAGEA, 0x8005);   //setup TWOR SL time and AC time
    
    A7139_WritePageA(WOR2_PAGEA, A7139Config_PageA[WOR2_PAGEA] | 0x0014);       //start TWOR by WOR_AC
    //A7139_WritePageA(WOR2_PAGEA, A7139Config_PageA[WOR2_PAGEA] | 0x001C);     //start TWOR by WOR_SL
    
//	    StrobeCMD(CMD_SLEEP);   //entry sleep mode

    SPI_Write((SPIIO*)&gs_RFCmdSleep, &gs_RFSpiPort);

    while(!SYS_GPI_GetLPort(GPI_DIO1));//while(GIO1==0);         //User can use this square wave to wake up MCU or other purposes
}

/*********************************************************************
** SYS_RF_Rssi_Get-----RSSI测量
*********************************************************************/
uint16_t SYS_RF_Rssi_Get(void)
{
    uint16_t tmp;

//    StrobeCMD(CMD_STBY);
    SPI_Write((SPIIO*)&gs_RFSTBY, &gs_RFSpiPort);

    A7139_WritePageA(GIO_PAGEA, (A7139Config_PageA[GIO_PAGEA] & 0xF000) | 0x0045);  //GIO1=FSYNC, GIO2=WTR
    
    A7139_WriteReg(ADC_REG, A7139Config[ADC_REG] | 0x8000);     //ARSSI=1
    
//    StrobeCMD(CMD_RX);  //entry RX mode
    SPI_Write((SPIIO*)&gs_RFCmdRx, &gs_RFSpiPort);

//	    delay10us(1);
    msleep(1);
//    while(!SYS_GPI_GetLPort(GPI_DIO1))//while(GIO1==0)      //Stay in RX mode until receiving ID code(ID detect ok)
//    {
//        tmp = (A7139_ReadReg(ADC_REG) & 0x00FF);    //read RSSI value(environment RSSI)
//    }
    tmp = (A7139_ReadReg(ADC_REG) & 0x00FF);        //read RSSI value(wanted signal RSSI)
    return tmp;
}

/*********************************************************************
** FIFO_extension_TX----FIFO扩展TX
*********************************************************************/
void FIFO_extension_TX(void)
{
    uint8_t i, n, j;
    SPIIO * rfSPI = &gst_RFCommon;
    uint8_t data[256];

    //Set FEP=255
    A7139_WritePageA(VCB_PAGEA, A7139Config_PageA[VCB_PAGEA] & 0xC0FF);             //FEP[13:8]=0
    A7139_WritePageA(FIFO_PAGEA, (A7139Config_PageA[FIFO_PAGEA] & 0x3F00) | 0xC0FF);//FPM=[11], FEP[7:0]=255

    A7139_WritePageA(GIO_PAGEA, (A7139Config_PageA[GIO_PAGEA] & 0xF000) | 0x0075);  //GIO1=FPF, GIO2=WTR
    A7139_WritePageA(CKO_PAGEA, (A7139Config_PageA[CKO_PAGEA] & 0xFF81) | 0x0002);  //CKO=DCK

    n=0;

//	    StrobeCMD(CMD_TFR);         //TX FIFO address pointer reset
    
    SPI_Write((SPIIO*)&gs_RFCmdTfr, &gs_RFSpiPort);
//	    SCS_0;
//	    ByteSend(CMD_FIFO_W);       //write 1st TX FIFO 64bytes
    for(i=0; i<64; i++)
    {
//	        ByteSend(n);
        data[i] = n;
        n++;
    }
//	    SCS_1;
    rfSPI->command[0] = CMD_FIFO_W;  
    
    rfSPI->dev = 0;
    rfSPI->data = data;
    rfSPI->cmdnum = 1;
    rfSPI->length = 64;
    SPI_Write((SPIIO*)rfSPI, &gs_RFSpiPort);

//	    StrobeCMD(CMD_TX);          //entry TX mode
    SPI_Write((SPIIO*)&gs_RFCmdTx, &gs_RFSpiPort);

    for(j=0; j<4; j++)			//total TX length = 64 + 48*4 = 256bytes
    {
//	        while(GIO1==0);         //wait FPF go high
        while(!SYS_GPI_GetLPort(GPI_DIO1));
//	        SCS_0;
//	        ByteSend(CMD_FIFO_W);   //write TX FIFO 48bytes
        for(i=0; i<48; i++)
        {
//	            ByteSend(n);
            
            data[i] = n;
            n++;
        }
//	        SCS_1;
        
        rfSPI->command[0] = CMD_FIFO_W;  
        
        rfSPI->dev = 0;
        rfSPI->data = data;
        rfSPI->cmdnum = 1;
        rfSPI->length = 48;
        SPI_Write((SPIIO*)rfSPI, &gs_RFSpiPort);
    }
    
//	    while(GIO2==1); //wait transmit completed
    
//	    while(!SYS_GPI_GetLPort(GPI_DIO2));
    while(SYS_GPI_GetLPort(GPI_DIO2));
}

/*********************************************************************
** FIFO_extension_RX-------FIFO扩展RX
*********************************************************************/
void FIFO_extension_RX(void)
{
    uint8_t i, n, j;
    uint8_t recv;
    uint8_t tmp;
    SPIIO * rfSPI = &gst_RFCommon;

    //Set FEP=255
    A7139_WritePageA(VCB_PAGEA, A7139Config_PageA[VCB_PAGEA] & 0xC0FF);             //FEP[13:8]=0
    A7139_WritePageA(FIFO_PAGEA, (A7139Config_PageA[FIFO_PAGEA] & 0x3F00) | 0xC0FF);//FPM=[11], FEP[7:0]=255

    A7139_WritePageA(GIO_PAGEA, (A7139Config_PageA[GIO_PAGEA] & 0xF000) | 0x0075);  //GIO1=FPF, GIO2=WTR
    A7139_WritePageA(CKO_PAGEA, (A7139Config_PageA[CKO_PAGEA] & 0xFF81) | 0x000A);  //CKO=RCK

    Err_ByteCnt=0;
    Err_BitCnt=0;
    n=0;
     
//	    StrobeCMD(CMD_RX);          //entry RX mode
    SPI_Write((SPIIO*)&gs_RFCmdRx, &gs_RFSpiPort);

    for(j=0; j<5 ; j++)			//total RX length = 48*5 + 16 = 256bytes
    {
//	        while(GIO1==0);         //wait FPF go high
        while(!SYS_GPI_GetLPort(GPI_DIO1));
//	        SCS_0;
//	        ByteSend(CMD_FIFO_R);   //read RX FIFO 48bytes
//	        for(i=0; i<48; i++)     
//	            tmpbuf[i]=ByteRead();
//	        SCS_1;
        rfSPI->command[0] = CMD_FIFO_R | CMD_Reg_R;  
        rfSPI->dev = 0;
        rfSPI->data = tmpbuf;
        rfSPI->cmdnum = 1;
        rfSPI->length = 48;
        SPI_Read((SPIIO*)rfSPI, &gs_RFSpiPort);

        //for check
        for(i=0; i<48; i++)
        {
            recv = tmpbuf[i];
            tmp = recv ^ n;
            if(tmp!=0)
            {
                Err_ByteCnt++;
                Err_BitCnt += (BitCount_Tab[tmp>>4] + BitCount_Tab[tmp & 0x0F]);
            }
            n++;
        }
    }

//	    while(GIO2==1);    			//wait receive completed
//	    while(!SYS_GPI_GetLPort(GPI_DIO2));
    while(SYS_GPI_GetLPort(GPI_DIO2));
//	
//	    SCS_0;
//	    ByteSend(CMD_FIFO_R);       //read RX FIFO 16bytes
//	    for(i=0; i<16; i++)     
//	        tmpbuf[i]=ByteRead();
//	    SCS_1;
    rfSPI->command[0] = CMD_FIFO_R | CMD_Reg_R;  
    rfSPI->dev = 0;
    rfSPI->data = &tmpbuf[0];
    rfSPI->cmdnum = 1;
    rfSPI->length = 16;
    SPI_Read((SPIIO*)rfSPI, &gs_RFSpiPort);

    //for check
    for(i=0; i<16; i++)
    {
        recv = tmpbuf[i];
        tmp = recv ^ n;
        if(tmp!=0)
        {
            Err_ByteCnt++;
            Err_BitCnt += (BitCount_Tab[tmp>>4] + BitCount_Tab[tmp & 0x0F]);
        }
        n++;
    }
}

/*********************************************************************
** FIFO_extension_Infinite_TX  FIFO无限拓展TX
*********************************************************************/
void FIFO_extension_Infinite_TX(void)
{
    uint8_t i, n;
    uint16_t j;   
    SPIIO * rfSPI = &gst_RFCommon;
    uint8_t data[256];
    //for Infinite : FEP[13:0]=remainder, FEP[13:0]<64
    A7139_WritePageA(VCB_PAGEA, A7139Config_PageA[VCB_PAGEA] & 0xC0FF);             //FEP[13:8]=0
    A7139_WritePageA(FIFO_PAGEA, (A7139Config_PageA[FIFO_PAGEA] & 0x3F00) | 0xC013);//FPM=[11], FEP[7:0]=19

    A7139_WritePageA(GIO_PAGEA, (A7139Config_PageA[GIO_PAGEA] & 0xF000) | 0x0075);  //GIO1=FPF, GIO2=WTR
    A7139_WritePageA(CKO_PAGEA, (A7139Config_PageA[CKO_PAGEA] & 0xFF81) | 0x0002);  //CKO=DCK
    
    A7139_WriteReg(PIN_REG, A7139Config[PIN_REG] | 0x0200);     //set INFS=1, infinite length

    n=0;

//	    StrobeCMD(CMD_TFR);         //TX FIFO address pointer reset
    SPI_Write((SPIIO*)&gs_RFCmdTfr, &gs_RFSpiPort);
    
//	    SCS_0;
//	    ByteSend(CMD_FIFO_W);       //write 1st TX FIFO 64bytes
    for(i=0; i<64; i++)
    {
//	        ByteSend(n);
        data[i]=n;
        if(n==255)
            n=0;
        else
            n++;
    }
//	    SCS_1;
    rfSPI->command[0] = CMD_FIFO_W;  
//              rfSPI->command[1] = tmp >> 8;
//              rfSPI->command[2] = tmp;
//      rfSPI->command[3] = ID_Tab[2];
//      rfSPI->command[4] = ID_Tab[3];
    
    rfSPI->dev = 0;
    rfSPI->data = data;
    rfSPI->cmdnum = 1;
    rfSPI->length = 64;
    SPI_Write((SPIIO*)rfSPI, &gs_RFSpiPort);

//	    StrobeCMD(CMD_TX);          //entry TX mode
    SPI_Write((SPIIO*)&gs_RFCmdTx, &gs_RFSpiPort);

    for(j=0; j<500; j++)
    {
//	        while(GIO1==0);         //wait FPF go high
        
        while(!SYS_GPI_GetLPort(GPI_DIO1));
//	        SCS_0;
//	        ByteSend(CMD_FIFO_W);   //write TX FIFO 48bytes
        for(i=0; i<48; i++)
        {
//	            ByteSend(n);
            
            data[i]=n;
            if(n==255)
                n=0;
            else
                n++;
            
        }
//	        SCS_1;

        rfSPI->command[0] = CMD_FIFO_W;  
    //              rfSPI->command[1] = tmp >> 8;
    //              rfSPI->command[2] = tmp;
    //      rfSPI->command[3] = ID_Tab[2];
    //      rfSPI->command[4] = ID_Tab[3];
        
        rfSPI->dev = 0;
        rfSPI->data = data;
        rfSPI->cmdnum = 1;
        rfSPI->length = 48;
        SPI_Write((SPIIO*)rfSPI, &gs_RFSpiPort);

        
//	        while(GIO1==1);         //wait FPF go low
        
        while(SYS_GPI_GetLPort(GPI_DIO1));
//	        SCS_0;
//	        ByteSend(CMD_FIFO_W);   //write TX FIFO 16bytes
        for(i=0; i<16; i++)
        {
//	            ByteSend(n);
            
            data[i]=n;
            if(n==255)
                n=0;
            else
                n++;
        }
//	        SCS_1;
        rfSPI->command[0] = CMD_FIFO_W;  
        rfSPI->dev = 0;
        rfSPI->data = data;
        rfSPI->cmdnum = 1;
        rfSPI->length = 16;
        SPI_Write((SPIIO*)rfSPI, &gs_RFSpiPort);

    }

    //remainder : FEP[13:0]=19, FIFO Length=19+1=20
//	    while(GIO1==0);             //wait last FPF go high
    
    while(!SYS_GPI_GetLPort(GPI_DIO1));
//	    SCS_0;
//	    ByteSend(CMD_FIFO_W);       //write TX FIFO 20bytes
    for(i=0; i<20; i++)
    {
        
        data[i]=n;
        ByteSend(n);
        if(n==255)
            n=0;
        else
            n++;
    }
//	    SCS_1;
    
    rfSPI->command[0] = CMD_FIFO_W;  
    rfSPI->dev = 0;
    rfSPI->data = data;
    rfSPI->cmdnum = 1;
    rfSPI->length = 20;
    SPI_Write((SPIIO*)rfSPI, &gs_RFSpiPort);
//	    while(GIO1==1);             //wait last FPF go low
    
    while(SYS_GPI_GetLPort(GPI_DIO1));
    A7139_WriteReg(PIN_REG, A7139Config[PIN_REG] & ~0x0200);    //disable INFS
    
//	    while(GIO2==1); //wait transmit completed
    
//	    while(!SYS_GPI_GetLPort(GPI_DIO2));
    while(SYS_GPI_GetLPort(GPI_DIO2));
}

/*********************************************************************
** FIFO_extension_Infinite_RX    无限拓展RX
*********************************************************************/
void FIFO_extension_Infinite_RX(void)
{
    uint8_t i, n;
    uint8_t recv;
    uint8_t tmp;
    uint16_t j;
    SPIIO * rfSPI = &gst_RFCommon;

    //for Infinite : FEP[13:0]=remainder, FEP[13:0]<64
    A7139_WritePageA(VCB_PAGEA, A7139Config_PageA[VCB_PAGEA] & 0xC0FF);             //FEP[13:8]=0
    A7139_WritePageA(FIFO_PAGEA, (A7139Config_PageA[FIFO_PAGEA] & 0x3F00) | 0xC013);//FPM=[11], FEP[7:0]=19

    A7139_WritePageA(GIO_PAGEA, (A7139Config_PageA[GIO_PAGEA] & 0xF000) | 0x0075);  //GIO1=FPF, GIO2=WTR
    A7139_WritePageA(CKO_PAGEA, (A7139Config_PageA[CKO_PAGEA] & 0xFF81) | 0x000A);  //CKO=RCK

    A7139_WriteReg(PIN_REG, A7139Config[PIN_REG] | 0x0200);     //set INFS=1, infinite length

    Err_ByteCnt=0;
    Err_BitCnt=0;
    n=0;
     
//	    StrobeCMD(CMD_RX);          //entry RX mode
    SPI_Write((SPIIO*)&gs_RFCmdRx, &gs_RFSpiPort);

    for(j=0; j<501 ; j++)
    {
//	        while(GIO1==0);         //wait FPF go high
        
        while(SYS_GPI_GetLPort(GPI_DIO1));
//	        SCS_0;
//	        ByteSend(CMD_FIFO_R);   //read RX FIFO 48bytes
//	        for(i=0; i<48; i++)     
//	            tmpbuf[i]=ByteRead();
//	        SCS_1;

    //      uint8_t data[10];
    
        rfSPI->command[0] = CMD_FIFO_R | CMD_Reg_R;  
    //              rfSPI->command[1] = tmp >> 8;
    //              rfSPI->command[2] = tmp;
    //      rfSPI->command[3] = ID_Tab[2];
    //      rfSPI->command[4] = ID_Tab[3];
        
        rfSPI->dev = 0;
        rfSPI->data = tmpbuf;
        rfSPI->cmdnum = 1;
        rfSPI->length = 48;
        SPI_Read((SPIIO*)rfSPI, &gs_RFSpiPort);

        
//	        while(GIO1==1);         //wait FPF go low
        while(SYS_GPI_GetLPort(GPI_DIO1));

        if(j==500)      //last FPF go low
        {
            //set INFS=0 to terminate infinite transmission in the period of 8*(data rate clock).
            A7139_WriteReg(PIN_REG, A7139Config[PIN_REG] & ~0x0200);    //disable INFS
        }
            
//	        SCS_0;
//	        ByteSend(CMD_FIFO_R);   //read RX FIFO 16bytes
//	        for(i=48; i<64; i++)        
//	            tmpbuf[i]=ByteRead();
//	        SCS_1;
        
        rfSPI->command[0] = CMD_FIFO_R | CMD_Reg_R;  
    //              rfSPI->command[1] = tmp >> 8;
    //              rfSPI->command[2] = tmp;
    //      rfSPI->command[3] = ID_Tab[2];
    //      rfSPI->command[4] = ID_Tab[3];
        
        rfSPI->dev = 0;
        rfSPI->data = &tmpbuf[48];
        rfSPI->cmdnum = 1;
        rfSPI->length = 16;
        SPI_Read((SPIIO*)rfSPI, &gs_RFSpiPort);

        //for check
        for(i=0; i<64; i++)
        {
            recv = tmpbuf[i];
            tmp = recv ^ n;
            if(tmp!=0)
            {
                Err_ByteCnt++;
                Err_BitCnt += (BitCount_Tab[tmp>>4] + BitCount_Tab[tmp & 0x0F]);
            }

            if(n==255)
                n=0;
            else
                n++;
        }
    }

//	    while(GIO2);    //wait receive completed
//	    while(!SYS_GPI_GetLPort(GPI_DIO2));
    while(SYS_GPI_GetLPort(GPI_DIO2));

    //remainder : FEP[13:0]=19, FIFO Length=19+1=20
//	    SCS_0;
//	    ByteSend(CMD_FIFO_R);       //read RX FIFO 20bytes
//	    for(i=0; i<20; i++)     
//	        tmpbuf[i]=ByteRead();
//	    SCS_1;
    rfSPI->command[0] = CMD_FIFO_R | CMD_Reg_R;  
//              rfSPI->command[1] = tmp >> 8;
//              rfSPI->command[2] = tmp;
//      rfSPI->command[3] = ID_Tab[2];
//      rfSPI->command[4] = ID_Tab[3];
    
    rfSPI->dev = 0;
    rfSPI->data = &tmpbuf[0];
    rfSPI->cmdnum = 1;
    rfSPI->length = 20;
    SPI_Read((SPIIO*)rfSPI, &gs_RFSpiPort);

    //for check
    for(i=0; i<20; i++)
    {
        recv = tmpbuf[i];
        tmp = recv ^ n;
        if(tmp!=0)
        {
            Err_ByteCnt++;
            Err_BitCnt += (BitCount_Tab[tmp>>4] + BitCount_Tab[tmp & 0x0F]);
        }

        if(n==255)
            n=0;
        else
            n++;
    }
}

/*********************************************************************
** Auto_Resend       自动重发
*********************************************************************/
void Auto_Resend(void)
{
//	    StrobeCMD(CMD_STBY);
    SPI_Write((SPIIO*)&gs_RFSTBY, &gs_RFSpiPort);

    A7139_WritePageA(GIO_PAGEA, (A7139Config_PageA[GIO_PAGEA] & 0xF000) | 0x0071);  //GIO1=VPOAK, GIO2=WTR
    A7139_WritePageA(CKO_PAGEA, (A7139Config_PageA[CKO_PAGEA] & 0xFF81) | 0x0002);  //CKO=DCK

    A7139_WritePageB(ACK_PAGEB, 0x003F);    //enable Auto Resend, by event, fixed, ARC=15
    A7139_WritePageB(ART_PAGEB, 0x001E);    //RND=0, ARD=30
    //ACK format = Preamble + ID = 8bytes
    //data rate=10kbps, Transmission Time=(1/10k)*8*8=6.4ms
    //so set ARD=30, RX time=30*250us=7.75ms

    while(1)
    {
        A7139_WriteFIFO();
//	        StrobeCMD(CMD_TX);      //entry TX mode
        SPI_Write((SPIIO*)&gs_RFCmdTx, &gs_RFSpiPort);
        
//	        delay10us(1);
        msleep(1);
//	        while(GIO2);            //wait transmit completed
//	        while(!SYS_GPI_GetLPort(GPI_DIO2));
        while(SYS_GPI_GetLPort(GPI_DIO2));

        //check VPOAK
//	        if(GIO1)
        if(    SYS_GPI_GetLPort(GPI_DIO1))
        {
            //ACK ok
        }
        else
        {
            //NO ACK
        }

//	        StrobeCMD(CMD_STBY);    //clear VPOAK(GIO1 signal)
        SPI_Write((SPIIO*)&gs_RFSTBY, &gs_RFSpiPort);

        msleep(1);
    }
}

/*********************************************************************
** Auto_ACK   自动确认
*********************************************************************/
void Auto_ACK(void)
{
//	    StrobeCMD(CMD_STBY);
    SPI_Write((SPIIO*)&gs_RFSTBY, &gs_RFSpiPort);
    A7139_WritePageA(GIO_PAGEA, (A7139Config_PageA[GIO_PAGEA] & 0xF000) | 0x0045);  //GIO1=FSYNC, GIO2=WTR
    A7139_WritePageA(CKO_PAGEA, (A7139Config_PageA[CKO_PAGEA] & 0xFF81) | 0x000A);  //CKO=RCK

    A7139_WritePageB(ACK_PAGEB, 0x0001);    //enable Auto ACK

    while(1)
    {
//	        StrobeCMD(CMD_RX);      //entry RX mode
        
        SPI_Write((SPIIO*)&gs_RFCmdRx, &gs_RFSpiPort);
//	        delay10us(1);   
        msleep(1);
//	        while(GIO2);            //wait receive completed
//	        while(!SYS_GPI_GetLPort(GPI_DIO2));
        while(SYS_GPI_GetLPort(GPI_DIO2));

        RxPacket();
    }
}
double SX1276LoRaGetPacketRssi( void )
{
    return -130;
}

void SYS_RF_StartRX(uint8_t flag)
{
//	    SPI_Write((SPIIO*)&gs_RFCmdRx, &gs_RFSpiPort);
    unsigned char RFLRState = MSR & 0x0F;
    if(RFLRState != RFLR_STATE_RX_RUNNING )
    {
        MSR = EZMAC_PRO_IDLE | RFLR_STATE_RX_INIT;//设状态机  
    }

}

void SYS_RF_Set_FallingEdge(uint8_t gpio)
{

    switch(gpio)
    {
        case GPI_DIO1:
            guc_GPIO1Falling = 1;
            break;
        case GPI_DIO2:
            guc_GPIO2Falling = 1;
            break;

    }

}
void SYS_RF_Set_RightEdge(uint8_t gpio)
{

    switch(gpio)
    {
        case GPI_DIO1:
            guc_GPIO1Right = 1;
            break;
        case GPI_DIO2:
            guc_GPIO2Right = 1;
            break;

    }

}

#include "log.h"
/*!
 * \brief Process the LoRa modem Rx and Tx state machines depending on the
 *        SX1276 operating mode.
 *
 * \retval rfState Current RF state [RF_IDLE, RF_BUSY, 
 *                                   RF_RX_DONE, RF_RX_TIMEOUT,
 *                                   RF_TX_DONE, RF_TX_TIMEOUT]
 */
unsigned int SX7319Process( void )
{
    unsigned int result = RF_BUSY;
    
    unsigned char RFLRState = MSR & 0x0F;
    
    switch( RFLRState )
    {
    case RFLR_STATE_IDLE:
        result = RF_IDLE;
        break;
    case RFLR_STATE_RX_INIT:
        SPI_Write((SPIIO*)&gs_RFSTBY, &gs_RFSpiPort);
        SPI_Write((SPIIO*)&gs_RFCmdRx, &gs_RFSpiPort);

        
        memset( RFBuffer, 0, ( size_t )RF_BUFFER_SIZE );

        PacketTimeout = LoRaSettings.RxPacketTimeout;
        RxTimeoutTimer = SX1276_TICK_COUNT( );
        //RFLRState = ;
        guc_GPIO2Falling = 0;
        MSR = EZMAC_PRO_IDLE | RFLR_STATE_RX_RUNNING;//设状态机   
        LOG_DEBUG( "======STATE_RX_INIT=====\n"); 
        break;
    case RFLR_STATE_RX_RUNNING:
        
        if( guc_GPIO2Falling )// !SYS_GPI_GetLPort(GPI_DIO2) ) // RxDone
        {
            guc_GPIO2Falling = 0;
            RxTimeoutTimer = SX1276_TICK_COUNT( );

//	            RxPacketSize = SYS_A7139_Recv(RFBuffer);
//	            if(RxPacketSize > RF_BUFFER_SIZE)
//	            {
//	                RxPacketSize = RF_BUFFER_SIZE;
//	            }

            


            

            //RFLRState = RFLR_STATE_RX_DONE;
            RFLRState = RFLR_STATE_RX_INIT;

            MSR = EZMAC_PRO_IDLE | RFLRState;//设状态机  
            //SPI_Write((SPIIO*)&gs_RFSTBY, &gs_RFSpiPort);
            //SPI_Write((SPIIO*)&gs_RFCmdRx, &gs_RFSpiPort);
            result = RF_RX_DONE;
        }
//	        if(LoRaSettings.FreqHopOn == true && DIO2 == 1 ) // FHSS Changed Channel
//	        {
//	            RxTimeoutTimer = SX1276_TICK_COUNT( );
//	            if( LoRaSettings.FreqHopOn == true )
//	            {
//	                SX1276Read( REG_LR_HOPCHANNEL, &SX1276LR->RegHopChannel );
//	                SX1276LoRaSetRFFrequency( HoppingFrequencies[SX1276LR->RegHopChannel & RFLR_HOPCHANNEL_CHANNEL_MASK] );
//	            }
//	            // Clear Irq
//	            SX1276Write( REG_LR_IRQFLAGS, 0xFF);//RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL );
//	            // Debug
//	            RxGain = SX1276LoRaReadRxGain( );
//	
//	            
//	            result = RF_IDLE;
//	            //MSR = EZMAC_PRO_IDLE;
//	            MSR = EZMAC_PRO_IDLE | RFLR_STATE_RX_INIT;//设状态机              
//	        }

//	        if( LoRaSettings.RxSingleOn == true ) // Rx single mode
        {
            if( ( SX1276_TICK_COUNT( ) - RxTimeoutTimer ) > PacketTimeout )
            {
                RFLRState = RFLR_STATE_RX_TIMEOUT;

                SPI_Write((SPIIO*)&gs_RFSTBY, &gs_RFSpiPort);
                
                MSR = EZMAC_PRO_IDLE | RFLR_STATE_RX_TIMEOUT;//设状态机   
                
                result = RF_IDLE;
            }
        }
        break;

    case RFLR_STATE_RX_TIMEOUT:
        RFLRState = RFLR_STATE_RX_INIT;
        
        MSR = EZMAC_PRO_IDLE | RFLRState;//设状态机   
        result = RF_RX_TIMEOUT;
        break;
    case RFLR_STATE_TX_INIT:
//	        SX1276LoRaSetOpMode( RFLR_OPMODE_STANDBY );
//		
//	        if( LoRaSettings.FreqHopOn == true )
//	        {
//	            SX1276LR->RegIrqFlagsMask = RFLR_IRQFLAGS_RXTIMEOUT |
//	                                        RFLR_IRQFLAGS_RXDONE |
//	                                        RFLR_IRQFLAGS_PAYLOADCRCERROR |
//	                                        RFLR_IRQFLAGS_VALIDHEADER |
//	                                        //RFLR_IRQFLAGS_TXDONE |
//	                                        RFLR_IRQFLAGS_CADDONE |
//	                                        //RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL |
//	                                        RFLR_IRQFLAGS_CADDETECTED;
//	            SX1276LR->RegHopPeriod = LoRaSettings.HopPeriod;
//		
//	            SX1276Read( REG_LR_HOPCHANNEL, &SX1276LR->RegHopChannel );
//	            SX1276LoRaSetRFFrequency( HoppingFrequencies[SX1276LR->RegHopChannel & RFLR_HOPCHANNEL_CHANNEL_MASK] );
//	        }
//	        else
//	        {
//	            SX1276LR->RegIrqFlagsMask = RFLR_IRQFLAGS_RXTIMEOUT |
//	                                        RFLR_IRQFLAGS_RXDONE |
//	                                        RFLR_IRQFLAGS_PAYLOADCRCERROR |
//	                                        RFLR_IRQFLAGS_VALIDHEADER |
//	                                        //RFLR_IRQFLAGS_TXDONE |
//	                                        RFLR_IRQFLAGS_CADDONE |
//	                                        RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL |
//	                                        RFLR_IRQFLAGS_CADDETECTED;
//	            SX1276LR->RegHopPeriod = 0;
//	        }
//	        SX1276Write( REG_LR_HOPPERIOD, SX1276LR->RegHopPeriod );
//	        SX1276Write( REG_LR_IRQFLAGSMASK, SX1276LR->RegIrqFlagsMask );
//	        
//	//	        SpiWriteRegister( REG_LR_PREAMBLEMSB,0);
//	//	        SpiWriteRegister( REG_LR_PREAMBLELSB,10);
//		    SX1276LoRaSetPreambleLength(LoRaSettings.preamble);
//	        // Initializes the payload size
//	        SX1276LR->RegPayloadLength = TxPacketSize;
//	        SX1276Write( REG_LR_PAYLOADLENGTH, SX1276LR->RegPayloadLength );
//	        
//	        SX1276LR->RegFifoTxBaseAddr = 0x00; // Full buffer used for Tx
//	        SX1276Write( REG_LR_FIFOTXBASEADDR, SX1276LR->RegFifoTxBaseAddr );
//		
//	        SX1276LR->RegFifoAddrPtr = SX1276LR->RegFifoTxBaseAddr;
//	        SX1276Write( REG_LR_FIFOADDRPTR, SX1276LR->RegFifoAddrPtr );
//	        
//	        // Write payload buffer to LORA modem
//	        SX1276WriteFifo( RFBuffer, SX1276LR->RegPayloadLength );
//	        
//	        SX1276Write(REG_LR_IRQFLAGS,0xff);     
//	                                        // TxDone               RxTimeout                   FhssChangeChannel          ValidHeader         
//	        SX1276LR->RegDioMapping1 = RFLR_DIOMAPPING1_DIO0_01 | RFLR_DIOMAPPING1_DIO1_00 | RFLR_DIOMAPPING1_DIO2_00 | RFLR_DIOMAPPING1_DIO3_01;
//	                                        // PllLock              Mode Ready
//	        SX1276LR->RegDioMapping2 = RFLR_DIOMAPPING2_DIO4_01 | RFLR_DIOMAPPING2_DIO5_00;
//	        SX1276WriteBuffer( REG_LR_DIOMAPPING1, &SX1276LR->RegDioMapping1, 2 );
//		
//	        SX1276LoRaSetOpMode( RFLR_OPMODE_TRANSMITTER );
//        if(SYS_GPI_GetLPort(GPI_DIO2))
        {
			SPI_Write((SPIIO*)&gs_RFSTBY, &gs_RFSpiPort);
    	    SYS_A7139_Send(RFBuffer,TxPacketSize);
    //	    WOR_enable_by_sync();
            PacketTimeout = LoRaSettings.TxPacketTimeout;
            TxTimeoutTimer = SX1276_TICK_COUNT( );
            RFLRState = RFLR_STATE_TX_RUNNING;
            
            guc_GPIO2Falling = 0;
            MSR = TX_STATE_BIT | RFLRState;//设状态机   
        }
//        else
//        {
//            PacketTimeout = 50;
//            if( ( SX1276_TICK_COUNT( ) - TxTimeoutTimer ) > PacketTimeout )
//            {
//                TxTimeoutTimer = SX1276_TICK_COUNT( );
//                SPI_Write((SPIIO*)&gs_RFSTBY, &gs_RFSpiPort);
//                SPI_Write((SPIIO*)&gs_RFCmdRx, &gs_RFSpiPort);
//            }
//        }
        break;
    case RFLR_STATE_TX_RUNNING:
        if( guc_GPIO2Falling ) // TxDone
        {
            // Clear Irq
//	            SX1276Write( REG_LR_IRQFLAGS, RFLR_IRQFLAGS_TXDONE  );
            RFLRState = RFLR_STATE_TX_DONE; 
            //LED_TX2_ON();
            SPI_Write((SPIIO*)&gs_RFCmdTx, &gs_RFSpiPort);
            TxTimeoutTimer = SX1276_TICK_COUNT( );
            MSR = TX_STATE_BIT | RFLRState;//设状态机   
        }
//	        if(LoRaSettings.FreqHopOn == true && DIO2 == 1 ) // FHSS Changed Channel
//	        {
//	            if( LoRaSettings.FreqHopOn == true )
//	            {
//	                SX1276Read( REG_LR_HOPCHANNEL, &SX1276LR->RegHopChannel );
//	                SX1276LoRaSetRFFrequency( HoppingFrequencies[SX1276LR->RegHopChannel & RFLR_HOPCHANNEL_CHANNEL_MASK] );
//	            }
//	            // Clear Irq
//	            
//	            TxTimeoutTimer = SX1276_TICK_COUNT( );
//	            SX1276Write( REG_LR_IRQFLAGS, RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL );
//	            MSR = EZMAC_PRO_IDLE;
//	        }

        if( ( SX1276_TICK_COUNT( ) - TxTimeoutTimer ) > PacketTimeout )
        {
            RFLRState = RFLR_STATE_TX_TIMEOUT;
        
            
            MSR = EZMAC_PRO_IDLE;//设状态机   
            
            result = RF_TX_TIMEOUT;
        }
        
        break;
    case RFLR_STATE_TX_DONE:
        // optimize the power consumption by switching off the transmitter as soon as the packet has been sent
//	        SX1276LoRaSetOpMode( RFLR_OPMODE_STANDBY );
        //LED_TX2_ON();

        RFLRState = RFLR_STATE_RX_INIT;//RFLR_STATE_IDLE;
        
        MSR = EZMAC_PRO_IDLE | RFLRState;
        result = RF_TX_DONE;
        break;
    
    default:
        break;
    } 
    return result;
}

/*********************************************************************
** entry_deep_sleep_mode进入深度睡眠
*********************************************************************/
//void entry_deep_sleep_mode(void)
//{
//    StrobeCMD(CMD_RF_RST);              //RF reset
//    A7139_WriteReg(PIN_REG, A7139Config[PIN_REG] | 0x0800);             //SCMDS=1
//    A7139_WritePageA(PM_PAGEA, A7139Config_PageA[PM_PAGEA] | 0x1010);   //STS=1, QDS=1
//    StrobeCMD(CMD_SLEEP);               //entry sleep mode
//    msleep(1);                      //delay 600us for VDD_A shutdown, C load=0.1uF
//    
//	StrobeCMD(CMD_DEEP_SLEEP);          //entry deep sleep mode
//    msleep(1);                      //delay 200us for VDD_D shutdown, C load=0.1uF
//}
//
///*********************************************************************
//** wake_up_from_deep_sleep_mode从睡眠模式中唤醒
//*********************************************************************/
//void wake_up_from_deep_sleep_mode(void)
//{
//    StrobeCMD(CMD_STBY);    //wake up
//    msleep(2);            //delay 2ms for VDD_D stabilized
//    //InitRF();
//}


tRadioDriver RadioDriver;

tRadioDriver* RadioDriverInit( void )
{

    RadioDriver.Init = SYS_RF_Init;
    RadioDriver.Reset = SYS_RF_Reset;
    RadioDriver.StartRx = SYS_RF_StartRX;
    RadioDriver.GetRxPacket = SYS_A7139_Recv;
    RadioDriver.SetTxPacket = SYS_A7139SetTxPacket;//SYS_A7139_Send;
    RadioDriver.Process = SX7319Process;
    RadioDriver.Tick = SX7139SysTick;
    RadioDriver.enter_sleep = entry_deep_sleep_mode;
    RadioDriver.wake_up = wake_up_from_deep_sleep_mode;
  

    return &RadioDriver;
}


#endif


