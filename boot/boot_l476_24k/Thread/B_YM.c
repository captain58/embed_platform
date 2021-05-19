#include "B_YM.h"
#include "Parameter.h"
#include "string.h"
#include "Thread.h"
#include "stm32l4xx_hal.h"



uint32_t FlashDestination;
uint8_t file_name[FILE_NAME_LENGTH];
uint16_t PageSize = PAGE_SIZE;
uint32_t RamSource;

uint32_t FLASH_PagesMask(volatile uint32_t Size);
int32_t Ymodem_Receive (uint8_t *buf); 
void Int2Str(uint8_t* str, int32_t intnum);
uint32_t Str2Int(uint8_t *inputstr, int32_t *intnum);
static int32_t Receive_Packet (uint8_t *data, int32_t *length, uint32_t timeout);
static uint32_t Send_Byte (uint8_t c);
int32_t Receive_Byte (uint8_t *c, uint32_t timeout);
uint16_t  Crc_HX(uint8_t * buffer, uint16_t len,uint16_t startposition);

extern UART_HandleTypeDef hlpuart1;

static uint8_t packet_data[PACKET_1K_SIZE + PACKET_OVERHEAD], file_size[FILE_SIZE_LENGTH];

const uint8_t FW_KEY[16] ={'C','o','p','y','r','i','g','h','t',' ','H','R','C','o','m','m'};

static void ChangeYmodemData(uint8_t *guc_ftpbuf,uint16_t len)
{
    uint8_t * pxor = guc_ftpbuf;
    for(int loop=0;loop<len;loop++)
    {
        *pxor = (*pxor)^FW_KEY[loop&0xf];
        pxor++;
    }
}

int32_t Ymodem_Receive (uint8_t *buf)
{
    uint8_t *file_ptr;//, *buf_ptr;
    int32_t i, packet_length, session_done, file_done, packets_received, errors, session_begin, size = 0;  
    uint8_t kk=0;
    FlashDestination = 0x1000;

    for (session_done = 0, errors = 0, session_begin = 0; ;)
    {
        for (packets_received = 0, file_done = 0; ;)
        {
            switch (Receive_Packet(packet_data, &packet_length, NAK_TIMEOUT))
            {
	            case 0:	
	                errors = 0;
	                switch (packet_length)
	                {
		                case - 1:
		                    Send_Byte(ACK);
		                    return 0;
		                case 0:
                            Send_Byte(ACK);
		                    file_done = 1;
		                    break;
		                default:
		                    if ((packet_data[PACKET_SEQNO_INDEX] & 0xff) != (packets_received & 0xff))
                                Send_Byte(NAK);
		                    else
		                    {
		                        if (packets_received == 0)
		                        {
		                            if (packet_data[PACKET_HEADER] != 0)
		                            {
		                                for (i = 0, file_ptr = packet_data + PACKET_HEADER; (*file_ptr != 0) && (i < FILE_NAME_LENGTH);)
		                                {
		                                    file_name[i++] = *file_ptr++;
		                                }
		                                file_name[i++] = '\0';
		                                for (i = 0, file_ptr ++; (*file_ptr != ' ') && (i < FILE_SIZE_LENGTH);)
		                                {
		                                    file_size[i++] = *file_ptr++;
		                                }
		                                file_size[i++] = '\0';
		                                Str2Int(file_size, &size);
		                                if (size > (FLASH_MAX_FILE_SIZE - 1))
		                                {
		                                    Send_Byte(CA);
		                                    Send_Byte(CA);
		                                    return -1;
		                                }
		                                Send_Byte(ACK);
		                                Send_Byte(CRC16);
		                            }
		                            else
		                            {
                                        Send_Byte(ACK);
		                                file_done = 1;
		                                session_done = 1;
		                                break;
		                            }
		                        }
		                        else
		                        {
//                                    memcpy(buf_ptr, packet_data + PACKET_HEADER, (uint16_t)packet_length);
//                                    RamSource = (uint32_t)buf;
//		                            for (j = 0; (j < packet_length) && (FlashDestination <  ApplicationAddress + size); j += 8)
//		                            {
//                                        WriteInFlash(FlashDestination,RamSource);
//		                                FlashDestination += 8;
//		                                RamSource += 8;
//		                            }    
                                    memcpy(flashwritebuf+1024*kk, packet_data + PACKET_HEADER, (uint16_t)packet_length);
                                    ChangeYmodemData(flashwritebuf+1024*kk,(uint16_t)packet_length);
                                    kk++;
                                    if(size <= FlashDestination)
                                    {
                                        WriteFlashData(FlashDestination,4096,flashwritebuf);
                                        ReadFlashData(FlashDestination,4096,flashreadbuf);
                                    }
                                    else if(kk>=4)
                                    {
                                        WriteFlashData(FlashDestination,4096,flashwritebuf);
                                        FlashDestination+=4096;
                                        kk = 0;
                                    }
                                    
		                            Send_Byte(ACK);
		                        }
		                        packets_received ++;
		                        session_begin = 1;
		                    }
		                }
	                break;
	            case 1:
	                Send_Byte(CA);
	                Send_Byte(CA);
	                return -3;
	            default:
	                if (session_begin > 0)	errors ++;
	                if (errors > MAX_ERRORS)
	                {
                        Send_Byte(CA);
	                    Send_Byte(CA);
	                    return 0;
	                }
	                Send_Byte(CRC16);
	                break;
            }
            if (file_done != 0)	break;
        }
        if (session_done != 0)	break;
    }
    return (int32_t)size;
}

static int32_t Receive_Packet (uint8_t *data, int32_t *length, uint32_t timeout)
{
    uint16_t i, packet_size;
    uint16_t crc =0;
    uint8_t c;
    uint8_t crc_h,crc_l;
    *length = 0;
    if (Receive_Byte(&c, timeout) != 0)	return -1;
    switch (c)
    {
	    case SOH: 
	        packet_size = PACKET_SIZE;
	        break;
	    case STX:  
	        packet_size = PACKET_1K_SIZE;
	        break;
	    case EOT:
	        return 0;
	    case CA:
	        if ((Receive_Byte(&c, timeout) == 0) && (c == CA))
	        {
	            *length = -1;
	            return 0;
	        }
	        else	return -1;
	    case ABORT1:
	    case ABORT2:return 1;
	    default:	return -1;
    }
    *data = c;
    for (i = 1; i < (packet_size + PACKET_OVERHEAD); i ++)
    {
        if (Receive_Byte(data + i, timeout) != 0)	
        {
            return -1;
        }
    }
    crc = Crc_HX(data,packet_size,3);
    crc_h = (uint8_t)(crc>>8);
    crc_l = (uint8_t)(crc&0xff);
    if((crc_l != data[packet_size+4])||(crc_h != data[packet_size+3]))	return -1;
    if (data[PACKET_SEQNO_INDEX] != ((data[PACKET_SEQNO_COMP_INDEX] ^ 0xff) & 0xff))	return -1;
    *length = packet_size;
    return 0;
}

static uint32_t Send_Byte (uint8_t c)
{
//    B_Uart3_SendByte(c);
    HAL_UART_Transmit(&hlpuart1,&c, 1, 2);
    return 0;
}

void Int2Str(uint8_t* str, int32_t intnum)
{
    uint32_t i, Div = 1000000000, j = 0, Status = 0;
    for (i = 0; i < 10; i++)
    {
        str[j++] = (uint8_t)((intnum / Div) + 48);
        intnum = intnum % Div;
        Div /= 10;
        if ((str[j-1] == '0') & (Status == 0))	j = 0;
        else	Status++;
    }
}

uint32_t Str2Int(uint8_t *inputstr, int32_t *intnum)
{
    uint32_t i = 0, res = 0;
    uint32_t val = 0;

    if (inputstr[0] == '0' && (inputstr[1] == 'x' || inputstr[1] == 'X'))
    {
        if (inputstr[2] == '\0')	return 0;
        for (i = 2; i < 11; i++)
        {
            if (inputstr[i] == '\0')
            {
                *intnum = val;
                res = 1;
                break;
            }
            if (ISVALIDHEX(inputstr[i]))
            {
                val = (val << 4) + CONVERTHEX(inputstr[i]);
            }
            else
            {
                res = 0;
                break;
            }
        }
        if (i >= 11)	res = 0;
    }
    else
    {
        for (i = 0; i < 11; i++)
        {
            if (inputstr[i] == '\0')
            {
                *intnum = val;
                res = 1;
                break;
            }
            else if ((inputstr[i] == 'k' || inputstr[i] == 'K') && (i > 0))
            {
                val = val << 10;
                *intnum = val;
                res = 1;
                break;
            }
            else if ((inputstr[i] == 'm' || inputstr[i] == 'M') && (i > 0))
            {
                val = val << 20;
                *intnum = val;
                res = 1;
                break;
            }
            else if (ISVALIDDEC(inputstr[i]))
            {
                val = val * 10 + CONVERTDEC(inputstr[i]);
            }
            else
            {
                res = 0;
                break;
            }
        }
        if (i >= 11)	res = 0;
    }
    return res;
}

uint32_t FLASH_PagesMask(volatile uint32_t Size)
{
    uint32_t pagenumber = 0x0;
    uint32_t size = Size;

    if ((size % PAGE_SIZE) != 0)
    {
        pagenumber = (size / PAGE_SIZE) + 1;
    }
    else
    {
        pagenumber = size / PAGE_SIZE;
    }
    return pagenumber;

}

int32_t Receive_Byte (uint8_t *c, uint32_t timeout)
{
    while(timeout-- > 0)
    {
        if(HAL_UART_Receive(&hlpuart1,c, 1, 1)==HAL_OK) return 0;
    }
    return -1;
}

uint16_t  Crc_HX(uint8_t * buffer, uint16_t len,uint16_t startposition)
{
	uint16_t CRCConst= 0x1021;
	uint16_t crc = 0;
	uint16_t i = 0;
	uint8_t j=0;
	for(i = startposition; i < len + startposition; i++)
	{
	   for(j=8;j>=1;j--)
	   {
	      if((crc & 0x8000) != 0)    crc = (crc << 1) ^ CRCConst; 
	      else    crc = crc << 1;
	      if((buffer[i] &(1<<(j-1)))!=0)  crc= crc^CRCConst;
	   }
	}
	return crc;
}


///********************************************************************************
//* Function Name : WriteFlash
//* Description	: This function initializes the TAU0 Channel 0 module.		
//* Parameters	: addr - 
//*					start address to write
//*				  data[] -
//*				  	data buffer to write
//*				  len -
//*				  	size of data buffer
//* Return value  : fsl_u08, status_code                                          
//*                   = 0x00(FSL_OK), normal                                      
//*                   = 0x05(FSL_ERR_PARAMETER), parameter error                   
//*                   = 0x10(FSL_ERR_PROTECTION), protection error                 
//*                   = 0x1C(FSL_ERR_WRITE), write error                           
//*                   = 0x1F(FSL_ERR_FLOW), last operation has not finished, yet.  
//*                                      or violates the precondition.             
//*                                      or FSL is suspending.                     
//*                   = 0xFF(FSL_BUSY), normal and means "process was started"
//********************************************************************************/
//static fsl_u08 WriteFlash(uint16_t addr, int8_t data[], uint8_t size)
//{
//	__near fsl_write_t my_fsl_write_str;	
//	
//	my_fsl_write_str.fsl_data_buffer_p_u08 = (__near fsl_u08 *) data;
//	my_fsl_write_str.fsl_word_count_u08 = size/4;
//	my_fsl_write_str.fsl_destination_address_u32 = addr;
//		
//	return FSL_Write((__near fsl_write_t*) &my_fsl_write_str);
//}

