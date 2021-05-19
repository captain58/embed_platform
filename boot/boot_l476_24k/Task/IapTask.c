#include "IapTask.h"
#include "Parameter.h"
#include "Task.h"
#include "stm32l4xx_hal.h"
#include "Thread.h"
#include "airthmetic.h"

IAP_FROM_FLAG Judge_Iap_From(void);
void IAPTask(void);
void JumpToApplication(void);
void WriteExFlashOK(void);

IAP_FROM_FLAG Judge_Iap_From(void)
{
    //return FROM_UART;
    //if(ReadFlash28data[16] == 0)
    {
        if(ReadFlash28data[16] == 0)    return FROM_FLASH;
        else if(ReadFlash28data[16] == 1)   return FROM_UART;
        else return FROM_NONE;
    }
    //else return FROM_NONE;
}

void IAPTask(void)
{
    IAP_FROM_FLAG temp;
    temp = Judge_Iap_From();
    if(temp == FROM_UART)   From_Uart_Task();
    else if(temp == FROM_FLASH) From_Flash_Task();
    else    JumpToApplication();
    DelaySuiyi(20000000);
    HAL_NVIC_SystemReset();
}

void JumpToApplication(void)
{
    uint32_t JumpAddress;
    pFunction Jump_To_Application;
    if(((*(__IO uint32_t*)ApplicationAddress) & 0x2FFE0000 ) == 0x20000000)
    {
        SerialPutString("\r\nExecute user Program\r\n");
        SerialPutString("========================\r\n\n");
        JumpAddress = *(__IO uint32_t*) (ApplicationAddress + 4);
        Jump_To_Application = (pFunction) JumpAddress;
        __set_MSP(*(__IO uint32_t*) ApplicationAddress);
        Jump_To_Application();
    }
    else    
    {
        SerialPutString("\r\nNo user Program\r\n\n");
        SerialPutString("========================\r\n\n");
        SerialPutString("\r\n======================================================================");
        SerialPutString("\r\n=    Prepare reset the IAP~~~~                                       =");
        SerialPutString("\r\n=    see you again soon~~~   =.=                                     =");
        SerialPutString("\r\n======================================================================");
        SerialPutString("\r\n\r\n");
        DelaySuiyi(20000000);
        HAL_NVIC_SystemReset();
    }
}

void WriteExFlashOK(void)
{
    uint8_t writetemp[28],i;
    uint16_t crc;
    for(i=0;i<16;i++)
    {
        writetemp[i] = 0;
    }
    for(i=16;i<26;i++)
    {
        writetemp[i] = ReadFlash28data[i];
    }
    crc = Crc_Change(writetemp,26,0,0xffff);
    writetemp[26] = (uint8_t)crc;
    writetemp[27] = (uint8_t)(crc>>8);
    WriteFlashData(0,28,writetemp);
}
