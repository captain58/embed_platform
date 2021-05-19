#ifndef __DES_H__
#define __DES_H__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char bit0:  1;
        unsigned char bit1:  1;
        unsigned char bit2:  1;
        unsigned char bit3:  1;
        unsigned char bit4:  1;
        unsigned char bit5:  1;
        unsigned char bit6:  1;
        unsigned char bit7:  1;
    };
} Stu_bits;
#endif

