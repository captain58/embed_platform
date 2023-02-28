#include "sys.h"
#include "App_Public.h"
#include "user_func.h"

uint8 CurStaSeq = 0; //��ǰ��ͳ������
uint32 CurStaMap[4]; // ��ǰ���Ѿ����յ�ͳ�ƽڵ�

/**********************************************************************
 *��λ����
 *
 **********************************************************************/
void Reset(void)
{
//	        void(*preset)(void);
//	        preset = (void(*)())0x0;
//	        preset();
//    WDTC = 0x4F00 ; // ��ų�ʱʱ��Ϊ����
//    WDMOD = 0x03;                               // ���ÿ��Ź�ģʽ���ж��Ҹ�λ
//    WDFEED = 0xAA;                              // ��һ��ι������WDT
//    WDFEED = 0x55;
//    while(1);
}

/*************************************************************************
 * Function Name: CheckID
 * Parameters:  ID
 * Return: 1 ��������0���Ƿ���
 * Description: ���ID�ĺϷ���
 *
 *************************************************************************/
uint8 Check_ID(uint16 id)
{
        if ((id > MAX_SUP_SS_NUM) || (id == 0))
                return 0;
        else
                return 1;
}

/*************************************************************************
 * Function Name: CleanPkt
 * Parameters:  ID
 * Return: 1 ��������0���Ƿ���
 * Description: ������ݰ��ڵ��������
 *
 *************************************************************************/
void Clean_PKT(PKT *pkt)
{
        if (pkt == NULL)
                return ;
        pkt->len = 0;
        pkt->sentoffset = 0;
        memset(pkt->data, 0, 256);
      
}

/*************************************************************************
 * Function Name: SetCrastyle
 * Parameters:  value
 * Return: 1 ��������0���Ƿ���
 * Description: ���þ���Ĳ�������ֵ
 *
 *************************************************************************/
void Set_Crastyle(uint8 value)
{
        SpiWriteRegister(0x09, value);

}

//*******************************************
uint8 Check_Sta_PKT(uint8 seq, uint8 id)
{
        #if 1
                uint8 index;
                uint8 location;
                if (seq == 0)
                        return 1;
                if (seq != CurStaSeq)
                        return 0;

                index = id / 32;
                location = id % 32;

                if ((CurStaMap[index] &(1 << location)) == (1 << location))
                // �ظ������
                        return 0;

                CurStaMap[index] |= (1 << location);
        #endif 
        return 1;


}

//====================================================
// ���x >y ,�򷵻�0xff
// ���򷵻�0
uint8 After(uint32 x, uint32 y)
{
        if (x > y)
                return 0xff;
        else
                return 0x00;
}

//====================================================
uint8 Before(uint32 x, uint32 y) //�ж�X�Ƿ��YС
{
        if (x < y)
              return 1;//���XС��Y������1
        else
              return 0;
}


