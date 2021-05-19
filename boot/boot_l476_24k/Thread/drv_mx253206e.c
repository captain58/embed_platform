#include "drv_mx253206e.h"

uint8_t ReadFlashData(uint32_t addr, uint16_t Len, uint8_t *value);
uint8_t WriteFlashData(uint32_t addr, uint16_t Len, uint8_t *value);
void InitialExFlashIO(void);

static uint8_t FLASH_MISO_READ(void)
{
    GPIO_PinState temp;
    temp = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_14);
    if(temp == GPIO_PIN_RESET)  return 0;
    else if(temp == GPIO_PIN_SET)   return 1;
    else return 2;
}

static uint8_t SPI_Flash_SendByte(uint8_t byte)
{
    uint8_t i, RxData = 0, num = 0x80;
    for(i = 0;i < 0x08;i++)
    {
        FLASH_CLK(0);
        if(byte & num)      FLASH_MOSI(1);
        else                FLASH_MOSI(0);
        if(num > 0x01)      num = num >> 1;
        FLASH_CLK(1);
        if(FLASH_MISO_READ())   RxData |= 0x01;
        if(i < 7)           RxData = RxData << 1;
    }
    return RxData;
}

static void WriteEnable(void)
{
    Flash_WriteEnable(1);
    Flash_Select(1);
    SPI_Flash_SendByte(0x06);
    Flash_Select(0);
}

static uint8_t SPI_Flash_ReadByte(void)
{
    return (SPI_Flash_SendByte(0));
}

static void ReadStatus(void)
{
    uint8_t busy = 1;
    while((busy&0x01) == 1)
    {
        Flash_Select(1);
        SPI_Flash_SendByte(0x05);
        busy = SPI_Flash_ReadByte();
        Flash_Select(0);
    }
}

static void WriteDisable(void)
{
    Flash_Select(1);
    SPI_Flash_SendByte(0x04);
    Flash_Select(0);
    ReadStatus();
    Flash_WriteEnable(0);
}

static void FLS_Disable4B(void)
{
    Flash_Select(1);
    SPI_Flash_SendByte(0xE9);
    Flash_Select(0);
    //ReadStatus();
    //Flash_WriteEnable(0);
}

static void EnableWriteStatusRegister(void)
{
    Flash_Select(1);
    SPI_Flash_SendByte(0x50);
    Flash_Select(0);
    Flash_Select(1);
    SPI_Flash_SendByte(0x01);
    SPI_Flash_SendByte(0x00);
    Flash_Select(0);
    ReadStatus();
}

static void FLASH_Delay(uint32_t time)
{
    while(time != 0)
    {
        time--;
    }
}

static uint8_t User_GetI2C_Data(uint32_t addr,uint16_t Len,uint8_t *value )
{
    Flash_Select(0);
    FLASH_Delay(DLYTIME);
    Flash_Select(1);
    SPI_Flash_SendByte(0x03); //read memory
    SPI_Flash_SendByte((uint8_t)((addr & 0xffffff)>>16));
    SPI_Flash_SendByte((uint8_t)((addr & 0xffff)>>8));
    SPI_Flash_SendByte((uint8_t)(addr & 0xff));
    while(Len>0)
    {
        *value = SPI_Flash_ReadByte();
        value++;
        Len--;
    }
    Flash_Select(0);
    return 1;
}

static void SectorErase4K(uint32_t addr)
{
    Flash_Select(0);
    FLASH_Delay(DLYTIME);
    EnableWriteStatusRegister();
    WriteEnable();
    Flash_Select(1);
    SPI_Flash_SendByte(0x20);
    SPI_Flash_SendByte((uint8_t)((addr & 0xffffff) >> 16)); 
    SPI_Flash_SendByte((uint8_t)((addr & 0xffff) >> 8));
    SPI_Flash_SendByte((uint8_t)(addr & 0xff));
    Flash_Select(0);
    ReadStatus();
    WriteDisable();
}

static uint8_t User_SetI2C_Data(uint32_t addr, uint16_t Len, uint8_t *value)
{
    uint32_t i = 0;
    uint32_t addr_next, len_write, len_remain;
    Flash_Select(0);
    FLASH_Delay(DLYTIME);
    addr_next = addr;
    len_remain = Len;
    while(len_remain)
    {
        if(((addr_next + len_remain) / 256) <= (addr_next / 256))
        {
            len_write   = len_remain;
            len_remain  = 0;
        }
        else
        {
            len_write   = ((addr_next / 256) + 1) * 256 - addr_next;
            len_remain -= len_write;
        }

        if((addr_next % 4096)==0)
        {
            SectorErase4K ( addr_next );
            Flash_Select(0);
            FLASH_Delay ( DLYTIME );
        }
        ReadStatus();
        EnableWriteStatusRegister();
        WriteEnable();
        Flash_Select(1);
        SPI_Flash_SendByte ( 0x02 ); //byte program
        SPI_Flash_SendByte ( ( uint8_t ) ( ( addr_next & 0xffffff ) >> 16 ) );
        SPI_Flash_SendByte ( ( uint8_t ) ( ( addr_next & 0xffff ) >> 8 ) );
        SPI_Flash_SendByte ( ( uint8_t ) ( addr_next & 0xff ) );
        for ( i = 0; i < len_write; i++ )
        {
            SPI_Flash_SendByte (*value++);
        }
        addr_next += len_write;
        Flash_Select(0);
    }
    ReadStatus();
    WriteDisable();
    return 1;
}

uint8_t ReadFlashData(uint32_t addr, uint16_t Len, uint8_t *value)
{
    User_GetI2C_Data(addr,Len,value);
    return 1;
}

uint8_t WriteFlashData(uint32_t addr, uint16_t Len, uint8_t *value)
{
    User_SetI2C_Data(addr,Len,value);
    return 1;
}

void InitialExFlashIO(void)
{
    Flash_WriteEnable(1);
    Flash_Select(1);
    FLASH_CLK(0);
    FLASH_MOSI(0);
    FLS_Disable4B();
}
