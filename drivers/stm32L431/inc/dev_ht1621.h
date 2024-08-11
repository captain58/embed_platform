//#include "stm32f10x.h"
//#include "common.h"

void HT1621_Init();
void HT1621_Cls();
void HT1621_WriteOne(uint8_t addr, uint8_t dat);
void HT1621_Write(uint8_t addr, uint8_t* dat, uint8_t len);
void HT1621_Set(uint8_t pos, uint8_t dat);
void HT1621_Cmd(uint8_t cmd);
void HT1621_Set_All();
