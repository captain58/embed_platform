#include "From_Flash.h"
#include "thread.h"
#include "airthmetic.h"
#include "parameter.h"
#include "task.h"
#include "string.h"

void From_Flash_Task(void);
void Start_Menu1(void);
uint8_t CheckInFlashCrc(void);
void CheckFlashDisplay(uint8_t flag);
void WriteInFlashWork(void);

void From_Flash_Task(void)
{
    Start_Menu1();
    if(CheckInFlashCrc() == 1)  
    {
        CheckFlashDisplay(1);
        WriteInFlashWork();
        WriteExFlashOK();
    }
    else    CheckFlashDisplay(0);
    JumpToApplication();
}

void Start_Menu1(void)
{
    SerialPutString("\r\n\n\n\n\n======================================================================");
    SerialPutString("\r\n=                                                                    =");
    SerialPutString("\r\n=  In-Application Programming Application From Flash                 =");
    SerialPutString("\r\n=                                                                    =");
    SerialPutString("\r\n======================================================================");
    SerialPutString("\r\n\n");
    DelaySuiyi(20000000);
}

uint8_t CheckInFlashCrc(void)
{
    uint32_t i,time;   
    uint16_t crc1;
    uint32_t length123=0,lengthtemp=0;  
    uint32_t k=0;
    uint8_t Number[10] = "";
    uint8_t flag;
    SerialPutString("\r\n==================================================");
    SerialPutString("\r\n=    Prepare to check out crc of flash           =");
//    length123=((uint32_t)ReadFlash28data[18]<<24)+((uint32_t)ReadFlash28data[19]<<16)+((uint32_t)ReadFlash28data[20] <<8)+(uint32_t)ReadFlash28data[21];
    memcpy(&length123,ReadFlash28data+18,4);
    Int2Str(Number, length123);
    SerialPutString("\n\r Size: ");
    SerialPutString(Number);
    SerialPutString(" Bytes\r\n");
    if(length123+256 < 4096)   time = 1;
    else 
    {
        k=(length123+256)>>12;
        if((length123+256)%4096 == 0)  time = k;
        else time=k+1;  
    }
    for(i=0;i<time;i++)
    {
        if(time == 1)
        {
            ReadFlashData(EX_FLASH_SP,4096,flashreadbuf);
            crc1 = Crc_Change(flashreadbuf,(uint16_t)(length123-4),256,0xffff);
        }
        else
        {            
            if(i == 0)
            {
                ReadFlashData(EX_FLASH_SP,4096,flashreadbuf);
                crc1 = Crc_Change(flashreadbuf,3840,256,0xffff);
                lengthtemp = 3840;
            }
            
            //142844  139264 3576
            else if(i == time - 1)
            {
                ReadFlashData(EX_FLASH_SP+i*4096,4096,flashreadbuf);
                crc1 = Crc_Change(flashreadbuf,length123-lengthtemp-4,0,crc1); //length-lengthtemp-4
            }
            else
            {
                ReadFlashData(EX_FLASH_SP+i*4096,4096,flashreadbuf);
                crc1 = Crc_Change(flashreadbuf,4096,0,crc1);
                lengthtemp += 4096;
            }
        }
    }    
    if((flashreadbuf[length123-lengthtemp-4]==flashreadbuf[length123-lengthtemp-2]) &&
       (flashreadbuf[length123-lengthtemp-3]==flashreadbuf[length123-lengthtemp-1]) &&
       (flashreadbuf[length123-lengthtemp-3] == ((uint8_t)(crc1>>8)))&&
       (flashreadbuf[length123-lengthtemp-4] == ((uint8_t)crc1)))  
    {
        flag = 1;
    }
    else flag = 0;
    SerialPutString("\r\n=    Check Complete~~~~~~~~                      =");
    return flag;
}

void CheckFlashDisplay(uint8_t flag)
{
    if(flag == 1)
    {
        if( ReadFlash28data[16]==0)
        {
            SerialPutString("\r\n=    Check OK~~~~~~~~                            =");
            SerialPutString("\r\n==================================================");
            SerialPutString("\r\n=    Prepare to earse the application flash      =");
            //EarseAllFlash();
            SerialPutString("\r\n=    Earse complete~~~~~~~~                      =");
            SerialPutString("\r\n==================================================\r\n\n");
            SerialPutString("\r\n=    Prepare to write the application flash      =");
        }
    }
    else if(flag == 0)
    {
        SerialPutString("\r\n=    Check ERROR~~~~~                            =");
        SerialPutString("\r\n==================================================");
        SerialPutString("\r\n=    Jump to application!!!!!!                   =");
    }
}
#define  CON_INFLASH_WRITE_PER_BYTE  2048

/*******************************************************************************
** @function_name:  BFLS_Read
** @function_file:  mxflash.c
** @描述: 从Flash指定地址读取指定长度的字节数据
** 
** 
** @参数: 
** @param: buffer(uint8*): 数据缓存
** @param: address(uint32): 地址
** @param: length(uint16): 读取的数据长度
** 
** @返回: 
** @return:  uint8   
** @作者: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @修改人: 
** @修改说明: 
*******************************************************************************/
uint8_t BFLS_Read(uint32_t address, uint16_t length, uint8_t* buffer)
{
    uint16_t len;

    while(1)
    {
        if((address & 0xFFF) + length > 4096)
        {
            len = 4096 - (address & 0xFFF);
        }
        else
        {
            len = length;
        }


        ReadFlashData(address, len, buffer);
        
        length -= len;
        if(length == 0)                     //数据读取完毕
        {
            return 0;
        }
        address += len;
        buffer += len;
    }
}
#include "stdio.h"
void WriteInFlashWork(void)
{
    uint32_t k,i,time;
    uint32_t j,l,ramsource,SuperLength;
    uint32_t flashdestination;
            FLASH_EraseInitTypeDef f;
    flashdestination = ApplicationAddress;
    memcpy((uint8_t *)&SuperLength,ReadFlash28data+18,4);
    //SuperLength=((uint32_t)ReadFlash28data[18]<<24);
    //SuperLength+=((uint32_t)ReadFlash28data[19]<<16);
    //SuperLength+=((uint32_t)ReadFlash28data[20] <<8);
    //SuperLength+=(uint32_t)ReadFlash28data[21];
    if(SuperLength < CON_INFLASH_WRITE_PER_BYTE)   
    {
        time = 1;
    }
    else 
    {
        k = (uint8_t)((SuperLength)>>11);
        if((SuperLength)%CON_INFLASH_WRITE_PER_BYTE == 0)  
        {
            time = k;
        }
        else 
        {
            time=k+1; 
        }            
    }
    //HAL_FLASH_Unlock(); 
    for(i=0;i<time;i++)
    {
        BFLS_Read(EX_FLASH_SP+256+CON_INFLASH_WRITE_PER_BYTE*i,CON_INFLASH_WRITE_PER_BYTE,flashreadbuf);
        ramsource =(uint32_t)(flashreadbuf);
        //if(i== 0)
        //{
        //    j = 3840;
        //    ramsource =(uint32_t)(flashreadbuf+256);
        //}
        if(i == time - 1)
            j = SuperLength - (time-2)*CON_INFLASH_WRITE_PER_BYTE;
        else
            j = CON_INFLASH_WRITE_PER_BYTE;
        
        uint32_t PageError = 0;
        
        f.TypeErase = FLASH_TYPEERASE_PAGES;
        f.Banks = 1;//((i * CON_INFLASH_WRITE_PER_BYTE) / 0x80000) + 1;//FLASH_BANK_1;
        f.Page = 12 + i;
        f.NbPages = 1;
        //uint8_t tmp[100];
        //memset(tmp,0,100);
        //sprintf((char *)tmp,"bank = %d page = %d\n", f.Banks, f.Page);
        //SerialPutString(tmp);        
        HAL_FLASH_Unlock();  
        HAL_FLASHEx_Erase(&f, &PageError);
        //HAL_FLASH_Lock();        
        
        for(l=0;l<j;l=l+8)
        {
            WriteInFlash(flashdestination,ramsource);
            flashdestination += 8;
            ramsource += 8;
        }    
        HAL_FLASH_Lock();  
    }    
    
    //SerialPutString("\r\n=    Updata Complete from ex flash~~~~~~~~       =");
}
