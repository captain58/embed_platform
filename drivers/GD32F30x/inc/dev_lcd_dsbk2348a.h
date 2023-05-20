#ifndef _DEV_LCD_DSBK2348A_H_
#define _DEV_LCD_DSBK2348A_H_

void SYS_LCD_Init(void);
uint8_t SYS_LCD_Set_Stt(uint8_t flag);
uint8_t SYS_LCD_Set_Time(TIME tm, uint8_t bcd);
uint8_t SYS_LCD_Set_Head(uint8_t flag);
#endif