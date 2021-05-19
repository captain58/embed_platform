#include "From_Uart.h"
#include "Thread.h"
#include "Parameter.h"
#include "Task.h"
#include "main.h"
#include "airthmetic.h"

void From_Uart_Task(void);
void Start_Menu(void);
void UartIapStartTask(void);
void Enter_App_Call(uint8_t flag);

uint8_t prv_ucUartxRxBuf[8];
uint16_t prv_ucUartxRxLen=0;

void Start_Menu(void)
{
    SerialPutString("\r\n\n\n\n\n======================================================================");
    SerialPutString("\r\n=                                                                    =");
    SerialPutString("\r\n=  OwenMa In-Application Programming Application From Uart           =");
    SerialPutString("\r\n=                                                                    =");
    SerialPutString("\r\n======================================================================");
    SerialPutString("\r\n\n");
    DelaySuiyi(20000000);
}

UART_APPLICATION_FLAG Choose_Application(void)
{
    SerialPutString("\r\n======================================================================");
    SerialPutString("\r\n= START MENU: input the '1''2''3'.other input make reset             =");
    SerialPutString("\r\n=                                                                    =");
    SerialPutString("\r\n=    1,Start the IAP task!!!                                         =");
    SerialPutString("\r\n=        Warning: '1' will earase all the application flash!!!       =");
    SerialPutString("\r\n=                                                                    =");
    SerialPutString("\r\n=    2,Jump to the application task!!!                               =");
    SerialPutString("\r\n=                                                                    =");
    SerialPutString("\r\n=    3,Reset the IAPTask!!!                                          =");
    SerialPutString("\r\n=                                                                    =");
    SerialPutString("\r\n======================================================================");
    SerialPutString("\r\n\n\n");
    prv_ucUartxRxLen = 0;
    vUartxRecData();
    if(prv_ucUartxRxBuf[0] == '1')   return UART_APPLICATION_FLAG_UPDATA;
    else if(prv_ucUartxRxBuf[0] == '2')  return UART_APPLICATION_FLAG_RUN;
    else    return UART_APPLICATION_FLAG_RESET;
}

void From_Uart_Task(void)
{
    UART_APPLICATION_FLAG temp = UART_APPLICATION_FLAG_NONE;
    Start_Menu();
    temp = Choose_Application();
    switch(temp)
    {
        case UART_APPLICATION_FLAG_UPDATA:
                Enter_App_Call(2);
                UartIapStartTask();
                From_Flash_Task();
                break;
        case UART_APPLICATION_FLAG_RUN:
                Enter_App_Call(1);
                JumpToApplication();
                break;
        default:
                Enter_App_Call(0);
                DelaySuiyi(20000000);
                HAL_NVIC_SystemReset();
                break;
    }
}

void UartIapStartTask(void)
{
    uint8_t Number[10] = "";
    int32_t Size = 0;
    SerialPutString("\r\n==================================================");
    SerialPutString("\r\n=    Prepare to earse the application flash      =");
    //EarseAllFlash();
    SerialPutString("\r\n=    Earse complete~~~~~~~~                      =");
    SerialPutString("\r\n==================================================\r\n\n");
    SerialPutString("\r\n==================================================");
    SerialPutString("\r\n=    Start Y Modem protcol~~~~~~~~                ");    
    SerialPutString("\r\n=    Please send the updata bin file~~~          =");
    SerialPutString("\r\n=    Press 'a' to reset the bootload~~~          =");
    SerialPutString("\r\n==================================================\r\n\n");
    prv_ucUartxRxLen = 0;
    Size = Ymodem_Receive(prv_ucUartxRxBuf);
    if (Size > 0)
    {
        SerialPutString("\n\n\n\n\r Programming Completed Successfully!\n\r--------------------------------\r\n Name: ");
        SerialPutString(file_name);
        Int2Str(Number, Size);
        SerialPutString("\n\r Size: ");
        SerialPutString(Number);
        SerialPutString(" Bytes\r\n");
        SerialPutString("-------------------\n");
        ReadFlash28data[18] = (uint8_t)(Size-256);
        ReadFlash28data[19] = (uint8_t)((Size-256)>>8);
        ReadFlash28data[20] = (uint8_t)((Size-256)>>16);
        ReadFlash28data[21] = (uint8_t)((Size-256)>>24);
    }
    else if (Size == -1)
    {
        SerialPutString("\n\n\rThe image size is higher than the allowed space memory!\n\r");
    }
    else if (Size == -2)
    {
        SerialPutString("\n\n\rVerification failed!\n\r");
    }
    else if (Size == -3)
    {
        SerialPutString("\r\n\nAborted by user.\n\r");
    }
    else
    {
        SerialPutString("\n\n\rFailed to receive the file!\n\r");
    }
}

void Enter_App_Call(uint8_t flag)
{
    if(flag == 1)
    {
        SerialPutString("\r\n======================================================================");
        SerialPutString("\r\n=    Jump to application!!!!!!                                       =");
        SerialPutString("\r\n=    See you~~~~    =.=                                              =");
        SerialPutString("\r\n======================================================================");
        SerialPutString("\r\n\r\n");
    }
    else if(flag == 2)
    {
        SerialPutString("\r\n======================================================================");
        SerialPutString("\r\n=    Jump to the Y-Modem mode  !!!                                   =");
        SerialPutString("\r\n======================================================================");
        SerialPutString("\r\n\r\n");
    }
    else
    {
        SerialPutString("\r\n======================================================================");
        SerialPutString("\r\n=    Prepare reset the IAP~~~~                                       =");
        SerialPutString("\r\n=    see you again soon~~~   =.=                                     =");
        SerialPutString("\r\n======================================================================");
        SerialPutString("\r\n\r\n");
    }
}
