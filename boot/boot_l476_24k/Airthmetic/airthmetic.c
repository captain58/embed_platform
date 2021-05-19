#include "airthmetic.h"
#include "parameter.h"

uint16_t Crc_Change(uint8_t *buf,uint16_t trans_length,uint16_t start,uint16_t crc);
void DelaySuiyi(uint32_t time);

uint16_t Crc_Change(uint8_t *buf,uint16_t trans_length,uint16_t start,uint16_t crc) //bb
{ 
    uint16_t i,j,TT;
    for (i=start;i<trans_length+start;i++) 
    {
        if(i==trans_length+start)
        {
            ReadFlash28data[0] = 1;
        }
        crc = crc ^ buf[i];
        for(j = 0; j < 8; j ++)
        {
            TT = crc & 1;
            crc = crc  >> 1;
            crc = crc  & 0x7fff;
            if(TT == 1)   crc = crc ^ 0xa001;
            crc = crc & 0xffff;
        }
    }
    return crc;
}

void DelaySuiyi(uint32_t time)
{
    uint32_t temp;
    for(temp = 0;temp<time;temp++)
    {
        ;
    }
}

