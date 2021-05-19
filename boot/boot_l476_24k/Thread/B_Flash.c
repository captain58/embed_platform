#include "B_Flash.h"
#include "stm32l4xx_hal.h"
#include "Parameter.h"
#include "airthmetic.h"

uint32_t GetInFlashData(uint32_t addr);
void EarseAllFlash(void);
uint8_t WriteInFlash(uint32_t Address, uint32_t DataAddr);

uint32_t GetInFlashData(uint32_t addr)
{
    return *(__IO uint32_t *)addr;
}

void EarseAllFlash(void)
{
    uint32_t PageError = 0;
    FLASH_EraseInitTypeDef f;
    f.TypeErase = FLASH_TYPEERASE_PAGES;
    f.Banks = FLASH_BANK_1;
    f.Page = 8;
    f.NbPages = 247;
    HAL_FLASH_Unlock();  
    HAL_FLASHEx_Erase(&f, &PageError);
    HAL_FLASH_Lock();
    
    f.TypeErase = FLASH_TYPEERASE_PAGES;
    f.Banks = FLASH_BANK_2;
    f.Page = 256;
    f.NbPages = 255;
    HAL_FLASH_Unlock();
    HAL_FLASHEx_Erase(&f, &PageError);
    HAL_FLASH_Lock();
}

uint8_t WriteInFlash(uint32_t Address, uint32_t DataAddr)
{
    uint64_t data123=0;
    //HAL_FLASH_Unlock();
    data123 = *(__IO uint64_t *)DataAddr;
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,Address,data123);
    //HAL_FLASH_Lock();
    return 1;
}
