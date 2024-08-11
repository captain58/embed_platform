//#include "ht1621.h"
//#include "stm32f10x_rcc.h"
//#include "stm32f10x_gpio.h"
#include "sys.h"
#include "hal.h"
#include "bsp.h"

#define BIAS 0x52 //0b1000 0101 0010 1/3duty 4com
#define SYSDIS 0X00 //0b1000 0000 0000 关振系统荡器和LCD偏压发生�?
#define SYSEN 0X02 //0b1000 0000 0010 打开系统振荡�?
#define LCDOFF 0X04 //0b1000 0000 0100 关LCD偏压
#define LCDON 0X06 //0b1000 0000 0110 打开LCD偏压
#define XTAL 0x28 //0b1000 0010 1000 外部接时�?
#define RC256 0X30 //0b1000 0011 0000 内部时钟
#define TONEON 0X12 //0b1000 0001 0010 打开声音输出
#define TONEOFF 0X10 //0b1000 0001 0000 关闭声音输出
#define WDTDIS 0X0A //0b1000 0000 1010 禁止看门�?


extern void Delay_us(uint16 us);

#define HT1621_DATADELAY Delay_us(1)
#define HT1621_DAT_SET SYS_GPO_Out(GPO_LCD_DATA,TRUE)//GPIOA->BSRR = GPIO_Pin_0
#define HT1621_DAT_CLR SYS_GPO_Out(GPO_LCD_DATA,FALSE)//GPIOA->BRR = GPIO_Pin_0
#define HT1621_CS_SET SYS_GPO_Out(GPO_LCD_CS,TRUE)//GPIOA->BSRR = GPIO_Pin_1
#define HT1621_CS_CLR SYS_GPO_Out(GPO_LCD_CS,FALSE)//GPIOA->BRR = GPIO_Pin_1
#define HT1621_WR_SET SYS_GPO_Out(GPO_LCD_WR,TRUE)//GPIOA->BSRR = GPIO_Pin_2
#define HT1621_WR_CLR SYS_GPO_Out(GPO_LCD_WR,FALSE)//GPIOA->BRR = GPIO_Pin_2
#define HT1621_RD_SET SYS_GPO_Out(GPO_LCD_RD,TRUE)//GPIOA->BSRR = GPIO_Pin_3
#define HT1621_RD_CLR SYS_GPO_Out(GPO_LCD_RD,FALSE)//GPIOA->BRR = GPIO_Pin_3

void HT1621_Init() {
//	    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
//	    GPIO_InitTypeDef GPIO_InitStructure;
//	    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
//	    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	    GPIO_Init(GPIOA, &GPIO_InitStructure);

    HT1621_CS_SET;
    HT1621_WR_SET;
    HT1621_RD_SET;
    SYS_GPO_Out(GPO_LCD_PWR, FALSE);
    SYS_GPO_Out(GPO_LCD_PWR, TRUE);
    
    HT1621_DATADELAY;

    HT1621_Cmd(0x52);//4个公共口,1/3偏压
    HT1621_Cmd(0x30);//系统时钟片内RC
    HT1621_Cmd(0x00);//关闭系统振荡器和LCD偏压发生�?
    HT1621_Cmd(0x0A);//WDT 溢出标志输出失效
    HT1621_Cmd(0x02);//打开系统振荡�?
    HT1621_Cmd(0x06);//打开LCD输出
    
//    HT1621_Set_All();
//    msleep(2000);
//    HT1621_Cls();
}

void HT1621_Cls() {
    uint8_t tmp[32], i;
    for (i = 0; i < 32; i++) {
        tmp[i] = 0;
    }
    HT1621_Write(0, tmp, 32);
}
void HT1621_Set_All() {
    uint8_t tmp[32], i;
    for (i = 0; i < 32; i++) {
        tmp[i] = 0xff;
    }
    HT1621_Write(0, tmp, 32);
}
void HT1621_SetBit(uint8_t dat) {
    if (dat) {
        HT1621_DAT_SET;
    } else {
        HT1621_DAT_CLR;
    }
    HT1621_DATADELAY;
}

void HT1621_SetWrite() {
    HT1621_WR_CLR;
    HT1621_DATADELAY;
    HT1621_WR_SET;
    HT1621_DATADELAY;
}

void HT1621_Cmd(uint8_t cmd) {
    uint8_t tmp, i;
    tmp = 0x80;
    HT1621_CS_CLR;
    HT1621_DATADELAY;
    for (i = 0; i < 4; i++) {
        HT1621_SetBit(tmp & 0x80);
        HT1621_SetWrite();
        tmp <<= 1;
    }
    tmp = cmd;
    for (i = 0; i < 8; i++) {
        HT1621_SetBit(tmp & 0x80);
        HT1621_SetWrite();
        tmp <<= 1;
    }
    HT1621_CS_SET;
    HT1621_DATADELAY;
}

void HT1621_WriteOne(uint8_t addr, uint8_t dat) {
    uint8_t i, j, tmp = 0xA0;
    HT1621_CS_CLR;
    HT1621_DATADELAY;
    for (i = 0; i < 3; i++) {
        HT1621_SetBit(tmp & 0x80);
        HT1621_SetWrite();
        tmp <<= 1;
    }
    tmp = addr << 2;
    for (i = 0; i < 6; i++) {
        HT1621_SetBit(tmp & 0x80);
        HT1621_SetWrite();
        tmp <<= 1;
    }
    for (i = 0; i < 4; i++) {
        HT1621_SetBit(dat & 0x80);
        HT1621_SetWrite();
        dat <<= 1;
    }
    HT1621_CS_SET;
    HT1621_DATADELAY;
}

void HT1621_Write(uint8_t addr, uint8_t* dat, uint8_t len) {
    uint8_t i, j, tmp = 0xA0;
    HT1621_CS_CLR;
    HT1621_DATADELAY;
    for (i = 0; i < 3; i++) {
        HT1621_SetBit(tmp & 0x80);
        HT1621_SetWrite();
        tmp <<= 1;
    }
    tmp = addr << 2;
    for (i = 0; i < 6; i++) {
        HT1621_SetBit(tmp & 0x80);
        HT1621_SetWrite();
        tmp <<= 1;
    }
    for (j = 0; j < len; j++) {
        tmp = dat[j];
        for (i = 0; i < 4; i++) {
            HT1621_SetBit(tmp & 0x80);
            HT1621_SetWrite();
            tmp <<= 1;
        }
    }
    HT1621_CS_SET;
    HT1621_DATADELAY;
}

void HT1621_Set(uint8_t pos, uint8_t dat) {
    if (pos < 3) {
        HT1621_WriteOne(pos * 2, dat);
    } else if (pos == 3) {
    } else {
        HT1621_WriteOne(pos * 2 + 3, dat);
    }
}