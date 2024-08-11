#ifndef _DEV_HT1621_C_
#define _DEV_HT1621_C_

#include "aos/include.h"
#include "hal.h"
/*LED 字模结构*/
typedef struct
{
    char mChar;
    uint8_t mModal;
}LED_MODAL_DEFINE;
 
typedef struct
{
    uint8_t Addr;
    uint8_t Ddat;
} LED_ICON_DEFINE;
#define ht1621_BIAS     0x52
#define ht1621_SYSEN    0x02
#define ht1621_LCDOFF   0x04
#define ht1621_LCDON    0x06
#define ht1621_RC256    0X30
#define ht1621_SYSDIS   0X00
#define ht1621_WDTDIS   0X0A
#define ht1621_BLON     0x12
#define ht1621_BLOFF    0x10
 
#undef  LED_WIDTH
#define LED_WIDTH   6
 
//	#define PIN_HT1621_CS       ht_cs
//	#define PIN_HT1621_WR       ht_wr
//	#define PIN_HT1621_DAT  ht_data
 
#define HT1621_SET_CS(value)    SYS_GPO_Out(GPO_LCD_CS,value)
#define HT1621_SET_WR(value)    SYS_GPO_Out(GPO_LCD_WR,value)
#define HT1621_SET_DAT(value) SYS_GPO_Out(GPO_LCD_DATA,value)
 
/*常用的LED字模定义,需要扩充的自己增加数组*/
const LED_MODAL_DEFINE LED_MODAL[]={
    {' ',0x00}  ,
    {'0',0xf5}  ,
    {'1',0x05}  ,
    {'2',0xb6}  ,
    {'3',0x97}  ,
    {'4',0x47}  ,
    {'5',0xd3}  ,
    {'6',0xf3}  ,
    {'7',0x85}  ,
    {'8',0xf7}  ,
    {'9',0xd7}  ,
    {'A',0xe7}  ,
    {'b',0x73}  ,
    {'C',0xf0}  ,
    {'E',0xf2}  ,
    {'F',0xe2}  ,
    {'H',0x67}  ,
    {'L',0x70}  ,
    {'n',0xe5}  ,
    {'U',0x75}  ,
    {'O',0xf5}  ,
    {'-',0x02}  ,
    {'I',0x60}  ,
    {'q',0xc7}  ,
    {'d',0x37}  ,
    {'S',0Xd3}  ,
    {'y',0x57}  ,
    {'t',0x72}  ,
    {'r',0x22}  ,
        {'j',0x15}  ,
        {'P',0xe6}  ,
};
 
const LED_ICON_DEFINE ICON_DEFINE[] =
{
    {0,0x0},
    {1,0x8},
    {2,0x8},
    {3,0x8},
    {4,0x8},
    {5,0x8},
    {6,0x8},
    {7,0x40},
    {8,0x20},
    {9,0x4},
    {0xa,0x2},
    {0xb,0x40},
    {0xc,0x2},
    {0xd,0x4},
    {0xe,0x80},
    {0xf,0x8},
    {0x10,0x20},
    {0x11,0x80},
    {0x12,0x8},
};
 
static char led_content[LED_WIDTH*2];         //显示内容 考虑小数点的问题,内容长度为LED宽度×2
static uint8_t led_len;
uint8_t led_disp_mem[];
uint8_t led_prev_disp_mem[];
//static _eCM led_prev_icon[];
static char _l_prev_blink[];
extern char _l_blink_state[];

void DelayUS(uint16 time)
{
    while(time--);
}

/*取字符对应的字模参数：iValue 需要取模的字符 区分大小写*/
static uint8_t LED_GetModal(char iValue)
{
    uint8_t i;
    for (i=0;i<sizeof(LED_MODAL)/sizeof(LED_MODAL_DEFINE);i++)
        {
        if (iValue == LED_MODAL[i].mChar)
                {
            return LED_MODAL[i].mModal;
        }
    }
    return i;
}
 
/*将内容写入显示设备参数:bGlint  需要闪烁的部分是否显示*/
void LED_DisplayToDevice()
{
    uint8_t i = 0;
    uint8_t pos = 0;
    uint8_t value = 0;
    uint8_t mode[LED_WIDTH];
    uint8_t dispMemAddr[] = {0,0,0,0,0,0};
    memset(mode,0,sizeof(mode));
    for (i=0;i<led_len;)
    {
        //取字模
        value=LED_GetModal(led_content[i++]);
        //小数点和前面的字符结合成一个字符
        if (led_content[i]=='.')
        {
            i++;
        }
//      if (_l_blink_state[i] == 0)
    //      value = LED_GetModal(' ');
 
        mode[pos++]=value;
        if (pos>LED_WIDTH-0)
            break;
    }
 
    //
    for (i=0; i<pos; i++)
    {
        if (_l_blink_state[i] == )
            led_disp_mem[dispMemAddr[i]] = ;
        else
            led_disp_mem[dispMemAddr[i]] = mode[i];
    }
    //
}
void WrCmd(uint8_t Cmd)
{
    uint8_t data, i, CmdByte = 0x80;
 
    HT1621_SET_CS();
    for (i=; i<; i++)
    {
        if (CmdByte&0x80)
            HT1621_SET_DAT();
        else
            HT1621_SET_DAT();
        HT1621_SET_WR();
        DelayUS(100);
        HT1621_SET_WR();
        CmdByte <<= ;
    }
 
    for (i=; i<; i++)
    {
        if (Cmd&0x80)
            HT1621_SET_DAT();
        else
            HT1621_SET_DAT();
        HT1621_SET_WR();
        DelayUS(100);
        HT1621_SET_WR();
        DelayUS(100);
        Cmd <<= ;
    }
 
    HT1621_SET_CS();
}
 
void WrAddData(uint8_t Add, uint8_t Ddat)
{
    uint8_t i, CmdByte = 0xA0;
 
    //
    led_disp_mem[Add] |= Ddat;
    //
 
    HT1621_SET_CS();
    for (i=; i<; i++)
    {
        if (CmdByte&0x80)
            HT1621_SET_DAT();
        else
            HT1621_SET_DAT();
        HT1621_SET_WR();
        DelayUS(100);
        HT1621_SET_WR();
        CmdByte <<= ;
    }
 
    Add <<= ;
    for (i=; i<; i++)
    {
        if (Add&0x80)
            HT1621_SET_DAT();
        else
            HT1621_SET_DAT();
        HT1621_SET_WR();
        DelayUS(100);
        HT1621_SET_WR();
        Add <<= ;
    }
 
    for (i=; i<; i++)
    {
        if (Ddat&0x80)
            HT1621_SET_DAT();
        else
            HT1621_SET_DAT();
        HT1621_SET_WR();
        DelayUS(100);
        HT1621_SET_WR();
        Ddat <<= ;
    }
 
    HT1621_SET_CS();
}
 
void HT1621Cls(void)
{
    uint8_t i;
    for (i=; i<; i+=)
    {
        WrAddData(i, 0x00);
    }
 
    memset(led_disp_mem, , sizeof (led_disp_mem));
}
 
void HT1621AllOn(void)
{
    uint8_t i;
 
    for (i=; i<; i+=)
    {
        WrAddData(i, 0xff);
    }
}
 
void HT1621_Init()
{
//	    _eCM cm[] = {CM_bph,CM_gw};
//	    GPIOEX_InitIOPin(PIN_HT1621_CS, GPIO_MODE_OUT_PP_LOW_FAST);
//	  GPIOEX_InitIOPin(PIN_HT1621_WR, GPIO_MODE_OUT_PP_LOW_FAST);
//	  GPIOEX_InitIOPin(PIN_HT1621_DAT, GPIO_MODE_OUT_PP_LOW_FAST);
//	 
//	    GPIOEX_InitIOPin(PIN_RUNLED, GPIO_MODE_OUT_PP_LOW_FAST);
//	    GPIOEX_BitWrite(PIN_RUNLED,);
    SYS_GPO_Out(GPO_LCD_PWR, TRUE);
    WrCmd(ht1621_BIAS);
    WrCmd(ht1621_RC256);
    WrCmd(ht1621_SYSEN);
    WrCmd(ht1621_LCDON);
 
    HT1621Cls();
    //
//	    HC164_Init();
    //
}
 
void HT1621_ReInit()
{
//	        GPIOEX_InitIOPin(PIN_HT1621_CS, GPIO_MODE_OUT_PP_LOW_FAST);
//	        GPIOEX_InitIOPin(PIN_HT1621_WR, GPIO_MODE_OUT_PP_LOW_FAST);
//	        GPIOEX_InitIOPin(PIN_HT1621_DAT, GPIO_MODE_OUT_PP_LOW_FAST);
        WrCmd(ht1621_BIAS);
        WrCmd(ht1621_RC256);
        WrCmd(ht1621_SYSEN);
        WrCmd(ht1621_LCDON);
}
 
void HT1621_BackLight(uint8_t nStat)
{
    if (nStat)
    {
        WrCmd(ht1621_BLON);
    }
    else
    {
        WrCmd(ht1621_BLOFF);
    }
}

#endif