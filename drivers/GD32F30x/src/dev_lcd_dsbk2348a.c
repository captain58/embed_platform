#ifndef _DEV_LCD_DSBK2348A_C_
#define _DEV_LCD_DSBK2348A_C_

#include "sys.h"
#include "hal.h"
#include "bsp.h"
#include "paradef.h"


#define CON_LCD_MODAL_LEN 32
Word32 gul_lcd_modal_bit;
uint8_t guc_led_modal[CON_LCD_MODAL_LEN];
ktimer_t     gst_lcd_timer;
/*LED 字模结构*/
typedef struct
{
    char mChar;
    uint8_t mModal;
}LED_MODAL_DEFINE;
/*常用的LED字模定义,需要扩充的自己增加数组*/
//段排布  DCBA EGF

const LED_MODAL_DEFINE LED_MODAL[]={
    {'0',0xfa}  ,
    {'1',0x60}  ,
    {'2',0xd6}  ,
    {'3',0xf4}  ,
    {'4',0x6c}  ,
    {'5',0xbc}  ,
    {'6',0xbe}  ,
    {'7',0xe0}  ,
    {'8',0xfe}  ,
    {'9',0xfc}  ,
    {'A',0xee}  ,
    {'b',0x3e}  ,
    {'C',0x9a}  ,
    {'E',0x9e}  ,
    {'F',0x8e}  ,
    {'H',0x6e}  ,
    {'L',0x1a}  ,
    {'n',0xea}  ,
    {'U',0x7a}  ,
    {'O',0x36}  ,
    {'-',0x04}  ,
    {'I',0x94}  ,
    {'q',0xec}  ,
    {'d',0x76}  ,
    {'S',0xbc}  ,
    {'y',0x7c}  ,
    {'t',0x1e}  ,
    {'r',0x06}  ,
    {'j',0xf0}  ,
    {'P',0xce}  ,
};
//段排布  ABCD FGE
const LED_MODAL_DEFINE LED_MODAL2[]={
    {'0',0xf5}  ,
    {'1',0x60}  ,
    {'2',0xb6}  ,
    {'3',0xf2}  ,
    {'4',0x63}  ,
    {'5',0xd3}  ,
    {'6',0xd7}  ,
    {'7',0x70}  ,
    {'8',0xf7}  ,
    {'9',0xf3}  ,
};


const uint16 LED_MODAL_DEFINE_NUM = (sizeof(LED_MODAL) / sizeof(LED_MODAL_DEFINE));

void Handle_Lcd()
{
    uint8_t bit = 0;
    int i = 0;
    if(gul_lcd_modal_bit.lword != 0)
    {
        for(i=0; i<4;)
        {
            if(gul_lcd_modal_bit.byte[i] != 0)
            {
                bit = Bit_Maps[gul_lcd_modal_bit.byte[i]];
                HT1621_WriteOne(i*8 + bit, guc_led_modal[i*8 + bit]);
                gul_lcd_modal_bit.byte[i] &= ~((uint8_t)1 << bit);
            }
            else
            {
                i++;
            }
        }
    }
}

void SYS_LCD_Init(void)
{
    gul_lcd_modal_bit.lword = 0;
    HT1621_Init();
    
    memset(guc_led_modal,0,32);
    

    krhino_timer_create(&gst_lcd_timer, "lcd_timer", Handle_Lcd,
                        krhino_ms_to_ticks(2000), krhino_ms_to_ticks(100), 0, 1);    
}
#define CON_LCD_MODAL_0     0
#define CON_LCD_MODAL_1     1
#define CON_LCD_MODAL_2     2
#define CON_LCD_MODAL_3     3
#define CON_LCD_MODAL_4     4
#define CON_LCD_MODAL_5     5
#define CON_LCD_MODAL_6     6
#define CON_LCD_MODAL_7     7
#define CON_LCD_MODAL_8     8
#define CON_LCD_MODAL_9     9
#define CON_LCD_MODAL_10     10
#define CON_LCD_MODAL_11     11
#define CON_LCD_MODAL_12     12
#define CON_LCD_MODAL_13     13
#define CON_LCD_MODAL_14     14
#define CON_LCD_MODAL_15     15
#define CON_LCD_MODAL_16   16
#define CON_LCD_MODAL_17   17
#define CON_LCD_MODAL_18   18
#define CON_LCD_MODAL_19   19
#define CON_LCD_MODAL_20   20
#define CON_LCD_MODAL_21   21
#define CON_LCD_MODAL_22   22
#define CON_LCD_MODAL_23   23
#define CON_LCD_MODAL_24  24
#define CON_LCD_MODAL_25   25
#define CON_LCD_MODAL_26   26
#define CON_LCD_MODAL_27   27
#define CON_LCD_MODAL_28   28
#define CON_LCD_MODAL_29   29
#define CON_LCD_MODAL_30   30
#define CON_LCD_MODAL_31   31


#define CON_WHEEL_FLAG_BIT          0x20
#define CON_WHEEL_FLAG_MASK          ~0x20

#define CON_TANK_FLAG_BIT          0x40
#define CON_TANK_FLAG_MASK          ~0x40


#define CON_HEAD_FLAG_BIT          0x80
#define CON_HEAD_FLAG_MASK          ~0x80



#define CON_DATA_YEAR_HIGH_BIT      0x10
#define CON_DATA_YEAR_HIGH_MASK     ~0x10



#define CON_BYTE_HIGH_MASK ~0xf0
#define CON_BYTE_LOW_MASK ~0xe0
#define CON_BYTE2_LOW_MASK ~0x70

#define CON_LCD_MODAL_TYPE_BIT 0
#define CON_LCD_MODAL_TYPE_BYTE 1

typedef struct _ST_LCD_MODAL_
{
    uint16_t id;
    uint8_t bit;
    uint8_t mask;
    uint8_t type;
    uint8_t modal1;
    uint8_t modal2;
}ST_LCD_MODAL;

//#define CON_LCD_WHEEL_WATER_BOX 0


ST_LCD_MODAL gst_lcd_modal_map[] = 
{
    {CON_LCD_WHEEL_WATER_BOX,               0x10, ~0x10, CON_LCD_MODAL_TYPE_BIT, 2,2   },
    {CON_LCD_WHEEL_WATER_LEVEL_LOW_MORE,    0x20, ~0x20, CON_LCD_MODAL_TYPE_BIT, 0,0   },
    {CON_LCD_WHEEL_WATER_LEVEL_LOW,         0x40, ~0x40, CON_LCD_MODAL_TYPE_BIT, 0,0   },
    {CON_LCD_WHEEL_WATER_LEVEL_MIDDLE,      0x80, ~0x80, CON_LCD_MODAL_TYPE_BIT, 0,0   },
    {CON_LCD_WHEEL_WATER_LEVEL_HIGH,        0x80, ~0x80, CON_LCD_MODAL_TYPE_BIT, 1,1   },
    {CON_LCD_WHEEL_WATER_LEVEL_HIGH_MORE,   0x40, ~0x40, CON_LCD_MODAL_TYPE_BIT, 1,1   },
    {CON_LCD_TANK_WATER_LEVEL_LOW_MORE,     0x10, ~0x10, CON_LCD_MODAL_TYPE_BIT, 10, 10   },
    {CON_LCD_TANK_WATER_LEVEL_LOW,          0x20, ~0x20, CON_LCD_MODAL_TYPE_BIT, 10, 10   },
    {CON_LCD_TANK_WATER_LEVEL_MIDDLE,       0x40, ~0x40, CON_LCD_MODAL_TYPE_BIT, 10, 10   },
    {CON_LCD_TANK_WATER_LEVEL_HIGH,         0x10, ~0x10, CON_LCD_MODAL_TYPE_BIT, 11, 11   },
    {CON_LCD_TANK_WATER_LEVEL_HIGH_MORE,    0x20, ~0x20, CON_LCD_MODAL_TYPE_BIT, 11, 11   },    
    {CON_LCD_CONNECT_STT,               0x80, ~0x80, CON_LCD_MODAL_TYPE_BIT, 20,20   },    //11
    {CON_LCD_RF_STT,                    0x10, ~0x10, CON_LCD_MODAL_TYPE_BIT, 8,8   },    
    {CON_LCD_PUMP_WATER_STT,            0x20, ~0x20, CON_LCD_MODAL_TYPE_BIT, 10,10   },    
    {CON_LCD_PUMP_MOTOR_STT,            0x10, ~0x10, CON_LCD_MODAL_TYPE_BIT, 21,21   },    
    {CON_LCD_PUMP_MOTOR_FAN1_STT,       0x20, ~0x20, CON_LCD_MODAL_TYPE_BIT, 21,21   },    
    {CON_LCD_PUMP_MOTOR_FAN2_STT,       0x40, ~0x40, CON_LCD_MODAL_TYPE_BIT, 21,21   },    //16
    {CON_LCD_PUMP_WATER_ARROWS1_STT,    0x10, ~0x10, CON_LCD_MODAL_TYPE_BIT, 25,25   },    
    {CON_LCD_PUMP_WATER_ARROWS2_STT,    0x10, ~0x10, CON_LCD_MODAL_TYPE_BIT, 23,23   },    
    {CON_LCD_PUMP_WATER_ARROWS3_STT,    0x20, ~0x20, CON_LCD_MODAL_TYPE_BIT, 20,20   },    
    {CON_LCD_PUMP_WATER_ARROWS4_STT,    0x40, ~0x40, CON_LCD_MODAL_TYPE_BIT, 20,20   },    
    {CON_LCD_PUMP_WATER_ARROWS5_STT,    0x10, ~0x10, CON_LCD_MODAL_TYPE_BIT, 19,19   },    
    {CON_LCD_PUMP_WATER_ARROWS6_STT,    0x10, ~0x10, CON_LCD_MODAL_TYPE_BIT, 17,17   }, //22   
    {CON_LCD_DEPUMP_MOTOR_STT,          0x10, ~0x10, CON_LCD_MODAL_TYPE_BIT, 4,4   },    
    {CON_LCD_DEPUMP_MOTOR_FAN1_STT,     0x40, ~0x40, CON_LCD_MODAL_TYPE_BIT, 4,4   },    
    {CON_LCD_DEPUMP_MOTOR_FAN2_STT,     0x20, ~0x20, CON_LCD_MODAL_TYPE_BIT, 4,4   },    
    {CON_LCD_DEPUMP_WATER_ARROWS1_STT,  0x10, ~0x10, CON_LCD_MODAL_TYPE_BIT, 12,12   },    
    {CON_LCD_DEPUMP_WATER_ARROWS2_STT,  0x20, ~0x20, CON_LCD_MODAL_TYPE_BIT, 12,12   },    
    {CON_LCD_DEPUMP_WATER_ARROWS3_STT,  0x20, ~0x20, CON_LCD_MODAL_TYPE_BIT, 3,3   }, //28   
    {CON_LCD_DEPUMP_WATER_ARROWS4_STT,  0x40, ~0x40, CON_LCD_MODAL_TYPE_BIT, 3,3   },    
    {CON_LCD_DEPUMP_WATER_ARROWS5_STT,  0x80, ~0x80, CON_LCD_MODAL_TYPE_BIT, 3,3   },    
    {CON_LCD_DEPUMP_WATER_ARROWS6_STT,  0x10, ~0x10, CON_LCD_MODAL_TYPE_BIT, 3,3   },    
    {CON_LCD_AUTO_STT,                  0x80, ~0x80, CON_LCD_MODAL_TYPE_BIT, 4,4   },    
    {CON_LCD_MANUAL_STT,                0x40, ~0x40, CON_LCD_MODAL_TYPE_BIT, 12,12   },    
    {CON_LCD_WHEEL_WATER_LOW_STT,       0x10, ~0x10, CON_LCD_MODAL_TYPE_BIT, 1,1   },    
    {CON_LCD_WHEEL_WATER_HIGH_MORE_STT, 0x80, ~0x80, CON_LCD_MODAL_TYPE_BIT, 12,12   },  //35  
    {CON_LCD_CHANGE_WATER_STT,          0x80, ~0x80, CON_LCD_MODAL_TYPE_BIT, 5,5   },    
    {CON_LCD_TANK_WATER_LOW_STT,        0x80, ~0x80, CON_LCD_MODAL_TYPE_BIT, 10,10   },    
    {CON_LCD_TANK_WATER_HIGH_MORE_STT,  0x80, ~0x80, CON_LCD_MODAL_TYPE_BIT, 11,11   },  //38  

    
    {CON_LCD_TEST1,  0x10, ~0x10, CON_LCD_MODAL_TYPE_BIT, 5,8   },    
    {CON_LCD_TEST2,  0x20, ~0x20, CON_LCD_MODAL_TYPE_BIT, 5,8   },    
    {CON_LCD_TEST3,  0x40, ~0x40, CON_LCD_MODAL_TYPE_BIT, 5,8   },    
    {CON_LCD_TEST4,  0x80, ~0x80, CON_LCD_MODAL_TYPE_BIT, 5,8   },    
    {CON_LCD_TEST5,  0x10, ~0x10, CON_LCD_MODAL_TYPE_BIT, 6,9   },   
    {CON_LCD_TEST6,  0x20, ~0x20, CON_LCD_MODAL_TYPE_BIT, 6,9   },    
    {CON_LCD_TEST7,  0x40, ~0x40, CON_LCD_MODAL_TYPE_BIT, 6,9   },    
    {CON_LCD_TEST8,  0x80, ~0x80, CON_LCD_MODAL_TYPE_BIT, 6,9   },  
    
 
    
};




const uint16 CON_LCD_MAP_NUM = (sizeof(gst_lcd_modal_map) / sizeof(ST_LCD_MODAL));

uint8_t SYS_LCD_Set(uint8_t index, uint8_t data)
{
    if(index >= CON_LCD_MAP_NUM) return 1;

    ST_LCD_MODAL * pModal = &gst_lcd_modal_map[index];

    if(pModal->type == CON_LCD_MODAL_TYPE_BYTE)
    {

    }
    else
    {
        guc_led_modal[pModal->modal1] &= pModal->mask;
        
        if(data)
        {
            guc_led_modal[pModal->modal1] |= pModal->bit;
        }
        else
        {
            guc_led_modal[pModal->modal1] &= ~pModal->bit;
        }
        

        gul_lcd_modal_bit.lword |= 0x01 << pModal->modal1;
    }
    return 0;
}

uint8_t SYS_LCD_Set_Wheel_Water_Level(uint8_t flag)
{
    
    switch(flag)
    {
    case CON_WATER_TANK_STT_LOW:
        SYS_LCD_Set(CON_LCD_WHEEL_WATER_LEVEL_LOW_MORE, 1);
        SYS_LCD_Set(CON_LCD_WHEEL_WATER_LEVEL_LOW, 0);
        SYS_LCD_Set(CON_LCD_WHEEL_WATER_LEVEL_MIDDLE, 0);
        SYS_LCD_Set(CON_LCD_WHEEL_WATER_LEVEL_HIGH, 0);
        SYS_LCD_Set(CON_LCD_WHEEL_WATER_LEVEL_HIGH_MORE, 0);
        
        SYS_LCD_Set(CON_LCD_WHEEL_WATER_LOW_STT, 1);
        SYS_LCD_Set(CON_LCD_WHEEL_WATER_HIGH_MORE_STT, 0);
       
        break;    
    case CON_WATER_TANK_STT_LOW_MID:
        SYS_LCD_Set(CON_LCD_WHEEL_WATER_LEVEL_LOW_MORE, 1);
        SYS_LCD_Set(CON_LCD_WHEEL_WATER_LEVEL_LOW, 1);
        SYS_LCD_Set(CON_LCD_WHEEL_WATER_LEVEL_MIDDLE, 0);
        SYS_LCD_Set(CON_LCD_WHEEL_WATER_LEVEL_HIGH, 0);
        SYS_LCD_Set(CON_LCD_WHEEL_WATER_LEVEL_HIGH_MORE, 0);
        
        SYS_LCD_Set(CON_LCD_WHEEL_WATER_LOW_STT, 0);
        SYS_LCD_Set(CON_LCD_WHEEL_WATER_HIGH_MORE_STT, 0);
        break;
    case CON_WATER_TANK_STT_HIGH_MID:
        SYS_LCD_Set(CON_LCD_WHEEL_WATER_LEVEL_LOW_MORE, 1);
        SYS_LCD_Set(CON_LCD_WHEEL_WATER_LEVEL_LOW, 1);
        SYS_LCD_Set(CON_LCD_WHEEL_WATER_LEVEL_MIDDLE, 1);
        SYS_LCD_Set(CON_LCD_WHEEL_WATER_LEVEL_HIGH, 0);
        SYS_LCD_Set(CON_LCD_WHEEL_WATER_LEVEL_HIGH_MORE, 0);

        SYS_LCD_Set(CON_LCD_WHEEL_WATER_LOW_STT, 0);
        SYS_LCD_Set(CON_LCD_WHEEL_WATER_HIGH_MORE_STT, 0);
        
        break;
    case CON_WATER_TANK_STT_HIGH:
        SYS_LCD_Set(CON_LCD_WHEEL_WATER_LEVEL_LOW_MORE, 1);
        SYS_LCD_Set(CON_LCD_WHEEL_WATER_LEVEL_LOW, 1);
        SYS_LCD_Set(CON_LCD_WHEEL_WATER_LEVEL_MIDDLE, 1);
        SYS_LCD_Set(CON_LCD_WHEEL_WATER_LEVEL_HIGH, 1);
        SYS_LCD_Set(CON_LCD_WHEEL_WATER_LEVEL_HIGH_MORE, 0);

        SYS_LCD_Set(CON_LCD_WHEEL_WATER_LOW_STT, 0);
        SYS_LCD_Set(CON_LCD_WHEEL_WATER_HIGH_MORE_STT, 0);
        
        break;
    case CON_WATER_TANK_STT_HIGH_MORE:
        SYS_LCD_Set(CON_LCD_WHEEL_WATER_LEVEL_LOW_MORE, 1);
        SYS_LCD_Set(CON_LCD_WHEEL_WATER_LEVEL_LOW, 1);
        SYS_LCD_Set(CON_LCD_WHEEL_WATER_LEVEL_MIDDLE, 1);
        SYS_LCD_Set(CON_LCD_WHEEL_WATER_LEVEL_HIGH, 1);
        SYS_LCD_Set(CON_LCD_WHEEL_WATER_LEVEL_HIGH_MORE, 1);

        SYS_LCD_Set(CON_LCD_WHEEL_WATER_LOW_STT, 0);
        SYS_LCD_Set(CON_LCD_WHEEL_WATER_HIGH_MORE_STT, 1);
        
        break;
    default:
        SYS_LCD_Set(CON_LCD_WHEEL_WATER_LEVEL_LOW_MORE, 0);
        SYS_LCD_Set(CON_LCD_WHEEL_WATER_LEVEL_LOW, 0);
        SYS_LCD_Set(CON_LCD_WHEEL_WATER_LEVEL_MIDDLE, 0);
        SYS_LCD_Set(CON_LCD_WHEEL_WATER_LEVEL_HIGH, 0);
        SYS_LCD_Set(CON_LCD_WHEEL_WATER_LEVEL_HIGH_MORE, 0);
        
        SYS_LCD_Set(CON_LCD_WHEEL_WATER_LOW_STT, 1);
        SYS_LCD_Set(CON_LCD_WHEEL_WATER_HIGH_MORE_STT, 1);
        
        break;
    }
    return 0;

}

uint8_t SYS_LCD_Set_Tank_Water_Level(uint8_t flag)
{
    
    switch(flag)
    {
    case CON_WATER_TANK_STT_LOW:
        SYS_LCD_Set(CON_LCD_TANK_WATER_LEVEL_LOW_MORE, 1);
        SYS_LCD_Set(CON_LCD_TANK_WATER_LEVEL_LOW, 0);
        SYS_LCD_Set(CON_LCD_TANK_WATER_LEVEL_MIDDLE, 0);
        SYS_LCD_Set(CON_LCD_TANK_WATER_LEVEL_HIGH, 0);
        SYS_LCD_Set(CON_LCD_TANK_WATER_LEVEL_HIGH_MORE, 0);
        
        SYS_LCD_Set(CON_LCD_TANK_WATER_LOW_STT, 1);
        SYS_LCD_Set(CON_LCD_TANK_WATER_HIGH_MORE_STT, 0);
        
        break;    
    case CON_WATER_TANK_STT_LOW_MID:
        SYS_LCD_Set(CON_LCD_TANK_WATER_LEVEL_LOW_MORE, 1);
        SYS_LCD_Set(CON_LCD_TANK_WATER_LEVEL_LOW, 1);
        SYS_LCD_Set(CON_LCD_TANK_WATER_LEVEL_MIDDLE, 0);
        SYS_LCD_Set(CON_LCD_TANK_WATER_LEVEL_HIGH, 0);
        SYS_LCD_Set(CON_LCD_TANK_WATER_LEVEL_HIGH_MORE, 0);

        SYS_LCD_Set(CON_LCD_TANK_WATER_LOW_STT, 0);
        SYS_LCD_Set(CON_LCD_TANK_WATER_HIGH_MORE_STT, 0);
        
        break;
    case CON_WATER_TANK_STT_HIGH_MID:
        SYS_LCD_Set(CON_LCD_TANK_WATER_LEVEL_LOW_MORE, 1);
        SYS_LCD_Set(CON_LCD_TANK_WATER_LEVEL_LOW, 1);
        SYS_LCD_Set(CON_LCD_TANK_WATER_LEVEL_MIDDLE, 1);
        SYS_LCD_Set(CON_LCD_TANK_WATER_LEVEL_HIGH, 0);
        SYS_LCD_Set(CON_LCD_TANK_WATER_LEVEL_HIGH_MORE, 0);
        
        SYS_LCD_Set(CON_LCD_TANK_WATER_LOW_STT, 0);
        SYS_LCD_Set(CON_LCD_TANK_WATER_HIGH_MORE_STT, 0);
        
        break;
    case CON_WATER_TANK_STT_HIGH:
        SYS_LCD_Set(CON_LCD_TANK_WATER_LEVEL_LOW_MORE, 1);
        SYS_LCD_Set(CON_LCD_TANK_WATER_LEVEL_LOW, 1);
        SYS_LCD_Set(CON_LCD_TANK_WATER_LEVEL_MIDDLE, 1);
        SYS_LCD_Set(CON_LCD_TANK_WATER_LEVEL_HIGH, 1);
        SYS_LCD_Set(CON_LCD_TANK_WATER_LEVEL_HIGH_MORE, 0);
        
        SYS_LCD_Set(CON_LCD_TANK_WATER_LOW_STT, 0);
        SYS_LCD_Set(CON_LCD_TANK_WATER_HIGH_MORE_STT, 0);
        
        break;
    case CON_WATER_TANK_STT_HIGH_MORE:
        SYS_LCD_Set(CON_LCD_TANK_WATER_LEVEL_LOW_MORE, 1);
        SYS_LCD_Set(CON_LCD_TANK_WATER_LEVEL_LOW, 1);
        SYS_LCD_Set(CON_LCD_TANK_WATER_LEVEL_MIDDLE, 1);
        SYS_LCD_Set(CON_LCD_TANK_WATER_LEVEL_HIGH, 1);
        SYS_LCD_Set(CON_LCD_TANK_WATER_LEVEL_HIGH_MORE, 1);

        SYS_LCD_Set(CON_LCD_TANK_WATER_LOW_STT, 0);
        SYS_LCD_Set(CON_LCD_TANK_WATER_HIGH_MORE_STT, 1);
        
        break;
    default:
        SYS_LCD_Set(CON_LCD_TANK_WATER_LEVEL_LOW_MORE, 0);
        SYS_LCD_Set(CON_LCD_TANK_WATER_LEVEL_LOW, 0);
        SYS_LCD_Set(CON_LCD_TANK_WATER_LEVEL_MIDDLE, 0);
        SYS_LCD_Set(CON_LCD_TANK_WATER_LEVEL_HIGH, 0);
        SYS_LCD_Set(CON_LCD_TANK_WATER_LEVEL_HIGH_MORE, 0);
        
        SYS_LCD_Set(CON_LCD_TANK_WATER_LOW_STT, 1);
        SYS_LCD_Set(CON_LCD_TANK_WATER_HIGH_MORE_STT, 1);
        
        
        break;
    }
    return 0;
    
}



uint8_t SYS_LCD_Set_Stt(uint8_t flag)
{
    
    //guc_led_modal[CON_LCD_MODAL_30] = 0;
    guc_led_modal[CON_LCD_MODAL_1] &= CON_WHEEL_FLAG_MASK;
    //guc_led_modal[CON_LCD_MODAL_31]&= CON_DATA_YEAR_HIGH_MASK;
    
    if(flag)
    {
        guc_led_modal[CON_LCD_MODAL_1] |= CON_WHEEL_FLAG_BIT;
        //guc_led_modal[CON_LCD_MODAL_31] |= CON_DATA_YEAR_HIGH_BIT;
    }
    else
    {
        guc_led_modal[CON_LCD_MODAL_1] &= ~CON_WHEEL_FLAG_BIT;
        //guc_led_modal[CON_LCD_MODAL_31] &= ~CON_DATA_YEAR_HIGH_BIT;

    }
    

    gul_lcd_modal_bit.lword |= 0x01 << CON_LCD_MODAL_1;
    //gul_lcd_modal_bit.lword |= 0x01 << CON_LCD_MODAL_31;    

    SYS_LCD_Set_Head(flag);
    SYS_LCD_Set_Tank_Stt(flag);


    return 0;

}



uint8_t SYS_LCD_Set_Head(uint8_t flag)
{
    
    guc_led_modal[CON_LCD_MODAL_21] &= CON_HEAD_FLAG_MASK;
    
    if(flag)
    {
        guc_led_modal[CON_LCD_MODAL_21] |= CON_HEAD_FLAG_BIT;
    }
    else
    {
        guc_led_modal[CON_LCD_MODAL_21] &= ~CON_HEAD_FLAG_BIT;

    }
    

    gul_lcd_modal_bit.lword |= 0x01 << CON_LCD_MODAL_21;


    return 0;

}

uint8_t SYS_LCD_Set_Tank_Stt(uint8_t flag)
{
    
    guc_led_modal[CON_LCD_MODAL_11] &= CON_TANK_FLAG_MASK;
    
    if(flag)
    {
        guc_led_modal[CON_LCD_MODAL_11] |= CON_TANK_FLAG_BIT;
    }
    else
    {
        guc_led_modal[CON_LCD_MODAL_11] &= ~CON_TANK_FLAG_BIT;

    }
    

    gul_lcd_modal_bit.lword |= 0x01 << CON_LCD_MODAL_11;


    return 0;

}



uint8_t SYS_LCD_Set_Byte_Test(uint8_t flag)
{
    uint8_t index = flag % LED_MODAL_DEFINE_NUM;

    guc_led_modal[CON_LCD_MODAL_30] &= CON_BYTE_HIGH_MASK;
    guc_led_modal[CON_LCD_MODAL_31]&= CON_BYTE_LOW_MASK;

    uint8_t byte = LED_MODAL[index].mModal;
    guc_led_modal[CON_LCD_MODAL_30] |= byte & 0xf0;
    guc_led_modal[CON_LCD_MODAL_31] |= (byte & 0x0f) << 4;
    
    gul_lcd_modal_bit.lword |= (uint32_t)0x01 << CON_LCD_MODAL_30;
    gul_lcd_modal_bit.lword |= (uint32_t)0x01 << CON_LCD_MODAL_31;    
    return 0;

}
uint8_t SYS_LCD_Set_Byte(uint8_t flag, uint8_t high, uint8_t low)
{
    guc_led_modal[high] &= CON_BYTE_HIGH_MASK;
    guc_led_modal[low]&= CON_BYTE_LOW_MASK;

    uint8_t byte = LED_MODAL[flag].mModal;
    guc_led_modal[high] |= byte & 0xf0;
    guc_led_modal[low] |= (byte & 0x0f) << 4;
    
    gul_lcd_modal_bit.lword |= 0x01 << high;
    gul_lcd_modal_bit.lword |= 0x01 << low;  
    return 0;
}

uint8_t SYS_LCD_Set_Byte2(uint8_t flag, uint8_t high, uint8_t low)
{
    guc_led_modal[high] &= CON_BYTE_HIGH_MASK;
    guc_led_modal[low]&= CON_BYTE2_LOW_MASK;

    uint8_t byte = LED_MODAL2[flag].mModal;
    guc_led_modal[high] |= byte & 0xf0;
    guc_led_modal[low] |= (byte & 0x0f) << 4;
    
    gul_lcd_modal_bit.lword |= 0x01 << high;
    gul_lcd_modal_bit.lword |= 0x01 << low;  
    return 0;
}


uint8_t SYS_LCD_Set_Mon_High(uint8_t flag)
{
    
    guc_led_modal[CON_LCD_MODAL_29] &= CON_DATA_YEAR_HIGH_MASK;
    guc_led_modal[CON_LCD_MODAL_27] &= CON_DATA_YEAR_HIGH_MASK;
    
    if(flag)
    {
        guc_led_modal[CON_LCD_MODAL_29] |= CON_DATA_YEAR_HIGH_BIT;
//	        guc_led_modal[CON_LCD_MODAL_27] |= CON_DATA_YEAR_HIGH_BIT;
    }
    else
    {
        guc_led_modal[CON_LCD_MODAL_29] |= CON_DATA_YEAR_HIGH_BIT;
        guc_led_modal[CON_LCD_MODAL_27] |= CON_DATA_YEAR_HIGH_BIT;

    }
    

    gul_lcd_modal_bit.lword |= 0x01 << CON_LCD_MODAL_29;
    gul_lcd_modal_bit.lword |= 0x01 << CON_LCD_MODAL_27;    


    return 0;
}

uint8_t SYS_LCD_Set_Year_High()
{
    
    guc_led_modal[CON_LCD_MODAL_31]&= CON_DATA_YEAR_HIGH_MASK;
    guc_led_modal[CON_LCD_MODAL_31] |= CON_DATA_YEAR_HIGH_BIT;

    gul_lcd_modal_bit.lword |= (uint32_t)0x01 << CON_LCD_MODAL_31;    


    return 0;

}


uint8_t SYS_LCD_Set_Time_dot()
{
    
    //guc_led_modal[CON_LCD_MODAL_30] = 0;
    guc_led_modal[CON_LCD_MODAL_15] &= CON_DATA_YEAR_HIGH_MASK;
    //guc_led_modal[CON_LCD_MODAL_31]&= CON_DATA_YEAR_HIGH_MASK;
    guc_led_modal[CON_LCD_MODAL_15] |= CON_DATA_YEAR_HIGH_BIT;
//	    if(flag)
//	    {
//	        guc_led_modal[CON_LCD_MODAL_15] |= CON_WHEEL_FLAG_BIT;
//	        //guc_led_modal[CON_LCD_MODAL_31] |= CON_DATA_YEAR_HIGH_BIT;
//	    }
//	    else
//	    {
//	        guc_led_modal[CON_LCD_MODAL_15] &= ~CON_WHEEL_FLAG_BIT;
//	        //guc_led_modal[CON_LCD_MODAL_31] &= ~CON_DATA_YEAR_HIGH_BIT;
//	
//	    }
    

    gul_lcd_modal_bit.lword |= 0x01 << CON_LCD_MODAL_15;
    //gul_lcd_modal_bit.lword |= 0x01 << CON_LCD_MODAL_31;    

    return 0;


}

uint8_t SYS_LCD_Set_Time(TIME tm, uint8_t bcd)
{
    uint8_t index = 0;

    SYS_LCD_Set_Year_High();

    
    if(bcd)
    {
        index = tm.year >> 4;
    }
    else
    {
        index = tm.year / 10;
    }
    SYS_LCD_Set_Byte(index,CON_LCD_MODAL_30,CON_LCD_MODAL_31);
    if(bcd)
    {
        index = tm.year & 0xf;
    }
    else
    {
        index = tm.year % 10;
    }  
    
    SYS_LCD_Set_Byte(index,CON_LCD_MODAL_28,CON_LCD_MODAL_29);

    if(bcd)
    {
        index = tm.month >> 4;
    }
    else
    {
        index = tm.month / 10;
    }

    if(index == 1)
    {

    }
    SYS_LCD_Set_Mon_High(index);


    if(bcd)
    {
        index = tm.month & 0xf;
    }
    else
    {
        index = tm.month % 10;
    }  
    
    SYS_LCD_Set_Byte(index,CON_LCD_MODAL_26,CON_LCD_MODAL_27);

    if(bcd)
    {
        index = tm.day >> 4;
    }
    else
    {
        index = tm.day / 10;
    }
    SYS_LCD_Set_Byte(index,CON_LCD_MODAL_24,CON_LCD_MODAL_25);
    if(bcd)
    {
        index = tm.day & 0xf;
    }
    else
    {
        index = tm.day % 10;
    }  

    SYS_LCD_Set_Byte(index,CON_LCD_MODAL_22,CON_LCD_MODAL_23);

    if(bcd)
    {
        index = tm.hour >> 4;
    }
    else
    {
        index = tm.hour / 10;
    }
    SYS_LCD_Set_Byte(index,CON_LCD_MODAL_18,CON_LCD_MODAL_19);
    if(bcd)
    {
        index = tm.hour & 0xf;
    }
    else
    {
        index = tm.hour % 10;
    }  

    SYS_LCD_Set_Byte(index,CON_LCD_MODAL_16,CON_LCD_MODAL_17);

    if(bcd)
    {
        index = tm.min >> 4;
    }
    else
    {
        index = tm.min / 10;
    }
    SYS_LCD_Set_Byte(index,CON_LCD_MODAL_14,CON_LCD_MODAL_15);
    if(bcd)
    {
        index = tm.min & 0xf;
    }
    else
    {
        index = tm.min % 10;
    }  

    SYS_LCD_Set_Byte(index,CON_LCD_MODAL_9,CON_LCD_MODAL_8);
    SYS_LCD_Set_Time_dot();

//	    guc_led_modal[CON_LCD_MODAL_30] &= CON_BYTE_HIGH_MASK;
//	    guc_led_modal[CON_LCD_MODAL_31]&= CON_BYTE_LOW_MASK;
//	
//	    uint8_t byte = LED_MODAL[index].mModal;
//	    guc_led_modal[CON_LCD_MODAL_30] |= byte & 0xf0;
//	    guc_led_modal[CON_LCD_MODAL_31] |= (byte & 0x0f) << 4;
//	    
//	    gul_lcd_modal_bit.lword |= 0x01 << CON_LCD_MODAL_30;
//	    gul_lcd_modal_bit.lword |= 0x01 << CON_LCD_MODAL_31;   

    

    return 0;
}

uint8_t SYS_LCD_Set_Change_Water_Date(uint8_t remaind_day)
{
    uint8_t index = 0;
    index = (remaind_day % 100) / 10;
    SYS_LCD_Set_Byte2(index,CON_LCD_MODAL_6,CON_LCD_MODAL_5);

    index = remaind_day % 10;

    SYS_LCD_Set_Byte2(index,CON_LCD_MODAL_13,CON_LCD_MODAL_7);

}

#endif
