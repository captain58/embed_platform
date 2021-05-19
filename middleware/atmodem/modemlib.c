/****************************************Copyright (c)**************************************************
**                               	____�Ƽ����޹�˾
**                                     		������
**
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: modemlib.c
**��   ��   ��: yzy
**����޸�����: 2007��3��3��
**��        ��: ����ģ�������������
**ע        ��:
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: yzy
** ��  ��: v1.0
** �ա���: 2007��3��3��
** �衡��: ԭʼ�汾
********************************************************************************************************/
#define EXT_MODEMLIB

#include "public.h"
#include "k_types.h"
#include "k_api.h"
//	#include "lib_app.h"
#include "hal.h"
#include <sal_import.h>
#include "atmodem.h"

#include "modemlib.h"



//	/************************************************************************
//	 * @function: LineLen
//	 * @����: �ӻ����л�ȡ��һ�����ݵĳ���(����һ����β��)
//	 * 
//	 * @����: 
//	 * @param: buffer ����
//	 * @param: len �������ݳ���
//	 * @param: ebytes ��β������
//	 * 
//	 * @����: 
//	 * @return: uint8  �г���(����һ����β��)
//	 *              0��ʾ�����ݻ�δ�ҵ��н�����,1��ʾ��������β��
//	 * @˵��: 
//	 * @����: yzy (2014/11/13)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	uint16 LineLen(uint8* buffer, uint16 len, uint16* ebytes)
//	{
//	    uint8 i;
//	    uint8 getline = false;          //�Ƿ��Ѿ��ҵ���
//	    uint16 ret = 0;
//	    
//	    for(i = 0; i < len; i++)        //ѭ���ж�
//	    {
//	        if((buffer[i] == '\r') || (buffer[i] == '\n'))
//	        {
//	            if(!getline)            //�ҵ���һ���н�����
//	            {
//	                getline = true;
//	                ret = i;
//	            }
//	        }
//	        else
//	        {
//	            if(getline)
//	            {
//	                break;
//	            }
//	        }
//	    }
//	
//	    if(!getline)                    //δ�ҵ���
//	    {
//	        return 0;
//	    }
//	    
//	    if(ebytes != __NULL)            //�ҵ���,��������뷶Χ���ҵ��Ľ�β������
//	    {
//	        *ebytes = i - ret;
//	    }
//	    
//	    return ret + 1;
//	}
//	
//	
//	
//	/************************************************************************
//	 * @function: GetOneChar
//	 * @����: �Ӵ��ڻ����еĵ�ǰ���ж�ȡһ��ASCII�ַ���ʽ������
//	 * 
//	 * @����: 
//	 * @param: rbyte �������ȡ���Ǹ��ֽ�(256��ʾ�����������ݿɶ�)
//	 * 
//	 * @����: 
//	 * @return: uint16  0-0xFFFE ת���õ�����
//	 *                  0xFFFF û���ҵ���ת�����ַ�
//	 * @˵��: 
//	 * @����: yzy (2014/11/13)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	uint16 GetOneChar(uint16* rbyte)
//	{
//		uint16 byte;							//....
//		uint16 value = 0xFFFF;				    //Ĭ�Ϸ��طǷ�ֵ
//		bool isbcd = false;						//Ĭ��û���ҵ�����	
//		
//		while(1)
//		{
//			byte = ReadNextByte();				//�Ӵ����ж�ȡ��һ���ֽ�
//	        
//			if(IsBcdChar(byte))					//�ж��Ƿ�������
//			{
//				if(!isbcd)						//�ж��Ƿ��ǵ�һ����
//				{
//					value = byte - '0';			//��ֵ	
//					isbcd = true;
//				}
//				else
//				{
//					value *= 10;				//��λ��ֵ
//					value += byte - '0';
//				}			
//			}
//			else
//			{
//	//	            if(isbcd && ((byte == 0x0D) || (byte == 0x0A)))//������β,������û��ɨ�赽����,���˳�
//	//	            {
//	//	                break;
//	//	            }
//	            
//				if(isbcd)						//�Ѿ�ɨ�赽��������
//				{
//					break;						//�˳�
//				}
//												//�����Ѿ�û�������˳�								
//				if(byte > 255)
//				{
//					byte = 256;
//					break;
//				}
//			}
//		}
//		
//		*rbyte = byte;							//��ȡ��ǰ��ȡ���ֽڣ������жϵ�ǰ���Ƿ����	
//		return value;							//�������ս��
//	}
//	
//	
//	
//	
//	
//	
//	/************************************************************************
//	 * @function: DecodeRecvDataNone
//	 * @����: ȥ����һ����ʣ�µ�����
//	 * 
//	 * @����: 
//	 * @param: mrfb ��������Ϣ�ṹ(�ú����ò���)
//	 * @����: 
//	 * @˵��: 
//	 * @����: yzy (2014/11/13)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	void DecodeRecvDataNone(ModemRecvFeedBack* mrfb)
//	{
//		uint16 byte;						//����Ӵ��ڶ������ֽ�
//		
//		while(1)
//		{
//			byte = ReadNextByte();			//��ȡ��һ���ֽ�
//			if(byte == 0x0D || byte == 0x0A)//��һ�ж������˳�
//			{
//				break;
//			}
//			
//			if(byte > 255)					//���е����ݶ�����Ҳ�˳�
//			{
//				break;
//			}	
//		}
//	}
//	
//	
//	
//	/************************************************************************
//	 * @function: DecodeRecvDataOneChar
//	 * @����: �Ӵ��ڻ���ĵ�һ���н���һ������,Ȼ��ȥ����һ����ʣ�µ�����
//	 * 
//	 * @����: 
//	 * @param: mrfb ��������Ϣ�ṹ
//	 * @����: 
//	 * @˵��: 
//	 * @����: yzy (2014/11/13)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	void DecodeRecvDataOneChar(ModemRecvFeedBack* mrfb)
//	{	
//		uint16 byte;                 
//											//��ȡһ���ֽڵ���Ϣ
//		mrfb->proced = (void*)GetOneChar(&byte);			
//		
//		if((byte != 0x0D) && (byte != 0x0A))//�����û�е���β
//		{
//			DecodeRecvDataNone(mrfb);		//ȥ������ʣ�����Ч��Ϣ
//		}
//	}
//	
//	
//	
//	
//	/************************************************************************
//	 * @function: DecodeRecvDataTwoChar
//	 * @����: �Ӵ��ڻ���ĵ�һ���н�����������,Ȼ��ȥ����һ����ʣ�µ�����
//	 * 
//	 * @����: 
//	 * @param: mrfb ��������Ϣ�ṹ
//	 * @����: 
//	 * @˵��: 
//	 * @����: yzy (2014/11/13)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	void DecodeRecvDataTwoChar(ModemRecvFeedBack* mrfb)
//	{
//		uint16 byte;
//		Word32 wd;
//		                 
//		wd.byte[0] = GetOneChar(&byte);		//��ȡ��һ������
//		wd.byte[1] = GetOneChar(&byte);		//��ȡ�ڶ�������
//		
//		mrfb->proced = (void*)(int)wd.lword;		//��ȡ����
//		
//		if((byte != 0x0D) && (byte != 0x0A))//ȥ������ʣ�����Ч��Ϣ
//		{
//			DecodeRecvDataNone(mrfb);		//....
//		}
//	}
//	
//	
//	
//	/************************************************************************
//	 * @function: DecodeAndEnqueuePduMsg
//	 * @����: ����PDU����
//	 * 
//	 * @����: 
//	 * @param: rbyte ��ǰ�еĵ�һ��ASCII������
//	 * 
//	 * @����: 
//	 * @return: bool  true �õ���һ�������Ķ���֡
//	 *                false ����ʧ��
//	 * @˵��: 
//	 * @����: yzy (2014/11/13)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	bool DecodeAndEnqueuePduMsg(uint8 rbyte)
//	{
//		uint8 byte;
//		uint8 result;
//		uint8 length = 0;
//		int16 uc_i;
//											
//		rbyte = (HexCharToValue(rbyte)<< 4);//ת����һ���ֽ�
//											//ת���ڶ����ֽڣ�Ȼ��ȥ����������
//		if((byte = HexCharToValue(ReadNextByte())) == 0xFF)
//		{
//			return false;
//		}
//		
//		rbyte+= byte;						//�������ĵĳ���
//		rbyte++;
	
//		while(rbyte--)						//ȥ����������
//		{
//			(void)ReadNextByte();			
//			(void)ReadNextByte();
//		}
//		
//		rbyte = HexCharToValue(ReadNextByte()) << 4;
//		rbyte += HexCharToValue(ReadNextByte());
//		
//		if(rbyte > 16)						//�Է����벻��̫��
//		{
//			return false;					//���ش���
//		}
//		
//		(void)ReadNextByte();				//���������ֽ�	
//		(void)ReadNextByte();				
//		for(;length < rbyte; length++)		//��ȡ�Է����ź���
//		{
//			if(length & 0x01)				//�ߵ��ֽڻ���
//			{
//				DevModem->smsRev->number[length-1]=ReadNextByte();    //�����ַ���
//			}
//			else
//			{
//				DevModem->smsRev->number[length+1]=ReadNextByte();    //�����ַ���
//			}
//		}
//		if(rbyte & 0x01)
//		{
//			DevModem->smsRev->number[length-1]=ReadNextByte();    //�����ַ���
//		}
//		
//		DevModem->smsRev->numlength = rbyte;	//Ŀ�����ĳ���
//		
//		
//		(void)ReadNextByte();				//���������ֽ�	
//		(void)ReadNextByte();				
//		
//		rbyte = HexCharToValue(ReadNextByte()) << 4;
//		rbyte += HexCharToValue(ReadNextByte());
//		
//		DevModem->smsRev->fmt = rbyte;     //���Ÿ�ʽ  
//		
//		rbyte = 14;
//		
//		while(rbyte--)						//����14���ֽڵ�ʱ��
//		{
//			ReadNextByte();
//		}
//											//��ȡ���ŵĳ���
//		rbyte = HexCharToValue(ReadNextByte()) << 4;
//		rbyte += HexCharToValue(ReadNextByte());	
//		
//		if(rbyte > 140)						//���ų��Ȳ��ܳ���140�ֽ�
//		{
//			return false;					//���ش���	
//		}
//		DevModem->smsRev->smslength=rbyte; //���ݳ���
//		                                    //7bit��8bit����
//		if((DevModem->smsRev->fmt & 0x0F) == 0)
//		{
//		    rbyte = ((rbyte * 7) >> 3) + ((rbyte * 7 & 0x07) ? 1 : 0);
//		}
//		
//		length = 0;	
//		 						
//		while(rbyte--)
//		{
//			byte = ReadNextByte();			//���ֶ�ȡ		
//			if(!IsHexChar(byte))			//ֱ����HexCharΪֹ
//			{
//				break;
//			}
//			result = HexCharToValue(byte) << 4;		
//					
//			byte = ReadNextByte();			//��ȡ����λ����ʽ
//			if(!IsHexChar(byte))			//��ȫ���ж�
//			{
//				break;
//			}
//			result += HexCharToValue(byte);	//���ϵ���λ
//											//����һ���ֽ�
//	
//			DevModem->smsRev->sms[length++]=result;      //ת�ƶ�������  
//		}
//		
//		if(rbyte == 0xFF)					//�ж��Ƿ���һ�������Ķ���
//		{	    
//		                                    //7bit��ʽ��ת��
//		    if((DevModem->smsRev->fmt & 0x0F) == 0)
//		    {
//		                                    //��ȡ7bit�����ݸ�����Ϊ1���ֽ�	                                        	        	        
//		        for(uc_i = DevModem->smsRev->smslength - 1; uc_i >= 0; uc_i--)
//		        {
//		                                    //1���ֽ�1���ֽڵĻ�ȡ
//		            DevModem->smsRev->sms[uc_i] = 
//		                STR_GetBits(DevModem->smsRev->sms, 7, 7 * uc_i);
//		        }
//		    }
//			return true;					//....
//		}
//		return false;						//���ش���
//		
//	}


/************************************************************************
 * @function: GetOneChar
 * @����: �Ӵ��ڻ����еĵ�ǰ���ж�ȡһ��ASCII�ַ���ʽ������
 * 
 * @����: 
 * @param: rbyte �������ȡ���Ǹ��ֽ�(256��ʾ�����������ݿɶ�)
 * 
 * @����: 
 * @return: uint16  0-0xFFFE ת���õ�����
 *                  0xFFFF û���ҵ���ת�����ַ�
 * @˵��: 
 * @����: yzy (2014/11/13)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8_t * GetOneCharAtStr(uint8_t * rbyte, uint8_t *val)
{
	uint8_t byte = 0;							//....
	uint16 value = 0xFFFF;				    //Ĭ�Ϸ��طǷ�ֵ
	bool isbcd = false;						//Ĭ��û���ҵ�����	
	uint8_t * p = rbyte;
	while(p !=  NULL)
	{
		byte = *p++;//ReadNextByte();				//�Ӵ����ж�ȡ��һ���ֽ�
		if(IsBcdChar(byte))					//�ж��Ƿ�������
		{
			if(!isbcd)						//�ж��Ƿ��ǵ�һ����
			{
				value = byte - '0';			//��ֵ	
				isbcd = true;
			}
			else
			{
				value *= 10;				//��λ��ֵ
				value += byte - '0';
			}			
		}
		else
		{
            
			if(isbcd)						//�Ѿ�ɨ�赽��������
			{
				break;						//�˳�
			}
											//�����Ѿ�û�������˳�								
//				if(byte == '\r' || byte == '\n')
//				{
//					byte = 256;
//					break;
//				}
		}
	}
	*val = (uint8_t)value;
	return p;							//�������ս��
}
uint8_t * GetOneShortAtStr(uint8_t * rbyte, uint16_t *val)
{
	uint8_t byte = 0;							//....
	uint16 value = 0xFFFF;				    //Ĭ�Ϸ��طǷ�ֵ
	bool isbcd = false;						//Ĭ��û���ҵ�����	
	uint8_t * p = rbyte;
	while(p !=  NULL)
	{
		byte = *p++;//ReadNextByte();				//�Ӵ����ж�ȡ��һ���ֽ�
		if(IsBcdChar(byte))					//�ж��Ƿ�������
		{
			if(!isbcd)						//�ж��Ƿ��ǵ�һ����
			{
				value = byte - '0';			//��ֵ	
				isbcd = true;
			}
			else
			{
				value *= 10;				//��λ��ֵ
				value += byte - '0';
			}			
		}
		else
		{
            
			if(isbcd)						//�Ѿ�ɨ�赽��������
			{
				break;						//�˳�
			}
											//�����Ѿ�û�������˳�								
//				if(byte == '\r' || byte == '\n')
//				{
//					byte = 256;
//					break;
//				}
		}
	}
	*val = (uint16_t)value;
	return p;							//�������ս��
}
uint8_t * GetOneLongAtStr(uint8_t * rbyte, uint32_t *val)
{
	uint8_t byte = 0;							//....
	uint32_t value = 0xFFFF;				    //Ĭ�Ϸ��طǷ�ֵ
	bool isbcd = false;						//Ĭ��û���ҵ�����	
	uint8_t * p = rbyte;
	while(p !=  NULL)
	{
		byte = *p++;//ReadNextByte();				//�Ӵ����ж�ȡ��һ���ֽ�
		if(IsBcdChar(byte))					//�ж��Ƿ�������
		{
			if(!isbcd)						//�ж��Ƿ��ǵ�һ����
			{
				value = byte - '0';			//��ֵ	
				isbcd = true;
			}
			else
			{
				value *= 10;				//��λ��ֵ
				value += byte - '0';
			}			
		}
		else
		{
            
			if(isbcd)						//�Ѿ�ɨ�赽��������
			{
				break;						//�˳�
			}
											//�����Ѿ�û�������˳�								
//				if(byte == '\r' || byte == '\n')
//				{
//					byte = 256;
//					break;
//				}
		}
	}
	*val = (uint32_t)value;
	return p;							//�������ս��
}

int DecodeStrTwoChar(uint8_t *rcv, uint8_t * out)
{
    uint8_t * p = rcv;
	if(p != NULL)
    {   
	    p = GetOneCharAtStr(p, &(out[0]));		//��ȡ��һ������
    }
    else
    {
        return -1;
    }
    
	if(p != NULL)
    {   
	    p = GetOneCharAtStr(p, &(out[1]));		//��ȡ��һ������
    }
    else
    {
        return -1;
    }
    
    return 0;
}
int DecodeStrFourChar(uint8_t *rcv, uint8_t * out)
{
    uint8_t * p = rcv;
    uint8_t val = 0;
	if(p != NULL)
    {   
	    p = GetOneCharAtStr(p, &val);		//��ȡ��һ������
	    out[0] = val;
    }
    else
    {
        return -1;
    }
    
	if(p != NULL)
    {   
	    p = GetOneCharAtStr(p, &val);		//��ȡ��һ������
	    out[1] = val;
    }
    else
    {
        return -1;
    }

	if(p != NULL)
    {   
	    p = GetOneCharAtStr(p, &val);		//��ȡ��һ������
	    out[2] = val;
    }
    else
    {
        return -1;
    }

	if(p != NULL)
    {   
	    p = GetOneCharAtStr(p, &val);		//��ȡ��һ������
	    out[3] = val;
    }
    else
    {
        return -1;
    }

    return 0;
}

/************************************************************************
 * @function: DecodeRecvDataOneChar
 * @����: �Ӵ��ڻ���ĵ�һ���н���һ������,Ȼ��ȥ����һ����ʣ�µ�����
 * 
 * @����: 
 * @param: mrfb ��������Ϣ�ṹ
 * @����: 
 * @˵��: 
 * @����: yzy (2014/11/13)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
int DecodeStrOneChar(uint8_t *rcv, uint8_t * out)
{	
	if(rcv != NULL)
    {   
	    GetOneCharAtStr(rcv, out);		//��ȡ��һ������
    }
    else
    {
        return -1;
    }
    
    return 0;
}

/************************************************************************
 * @function: DecodeRecvDataOneChar
 * @����: �Ӵ��ڻ���ĵ�һ���н���һ������,Ȼ��ȥ����һ����ʣ�µ�����
 * 
 * @����: 
 * @param: mrfb ��������Ϣ�ṹ
 * @����: 
 * @˵��: 
 * @����: yzy (2014/11/13)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
int DecodeStrOneShort(uint8_t *rcv, uint16_t * out)
{	
	if(rcv != NULL)
    {   
	    GetOneShortAtStr(rcv, out);		//��ȡ��һ������
    }
    else
    {
        return -1;
    }
    
    return 0;
}

/************************************************************************
 * @function: DecodeRecvDataOneChar
 * @����: �Ӵ��ڻ���ĵ�һ���н���һ������,Ȼ��ȥ����һ����ʣ�µ�����
 * 
 * @����: 
 * @param: mrfb ��������Ϣ�ṹ
 * @����: 
 * @˵��: 
 * @����: yzy (2014/11/13)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
int DecodeStrOneLong(uint8_t *rcv, uint32_t * out)
{	
	if(rcv != NULL)
    {   
	    GetOneLongAtStr(rcv, out);		//��ȡ��һ������
    }
    else
    {
        return -1;
    }
    
    return 0;
}

