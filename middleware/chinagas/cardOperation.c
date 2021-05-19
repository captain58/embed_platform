#include "cardOperation.h"
#include "cardapp.h"
#include "hwCtrl.h"
#include "count.h"
static unsigned char CardState=0;
void Card_DeInit(void)
{
    CardState=0;
}

unsigned char Card_Init(void)
{
    unsigned char tBuf[20],tLen;
    if(CardState==0)
    {
        CardState=1;
        tLen=hwSmciReset(eCpuChan_CARD,tBuf);
        Card_SelectFile(0xdf01);
        if(tLen>0)
            return 0;
    }
    return 1;
}

unsigned int Card_SendApdu(pCpuApdu Cmd)
{
    unsigned char tLen;
    switch(Cmd->Cmd.INS)
    {
    case 0x5c://�����
    case 0xb0://������
    case 0x84://ȡ�����
    case 0xc0:
        tLen=hwSmciApdu(eCpuChan_CARD,(unsigned char *)Cmd,5,Cmd->Cmd.Data);
        if(tLen>2)
        {
            return 0x100*Cmd->Cmd.Data[Cmd->Cmd.Le]+Cmd->Cmd.Data[Cmd->Cmd.Le+1];
        }
        break;
        
    default:
        tLen=hwSmciApdu(eCpuChan_CARD,(unsigned char *)Cmd,5+Cmd->Cmd.Le,Cmd->Cmd.Data);        
        
        break;
    }
    if(tLen==2)
    {
        return 0x100*Cmd->Cmd.Data[0]+Cmd->Cmd.Data[1];
    }
    else
        return 0;
}

static unsigned char EsamState=0;
void Esam_DeInit(void)
{
    EsamState=0;
}

unsigned char Esam_Init(void)
{
    unsigned char tBuf[20],tLen;
    if(EsamState == 0)
    {
        EsamState = 1;
        tLen=hwSmciReset(eCpuChan_ESAM,tBuf);
        Esam_SelectFile(0xdf01);
        if(tLen>0)
            return 0;
    }
    return 1;
}

unsigned int Esam_SendApdu(pCpuApdu Cmd)
{
    unsigned char tLen;
    switch(Cmd->Cmd.INS)
    {
    case 0x5c://�����
    case 0xb0://������
    case 0xb2://����¼
    case 0x84://ȡ�����
    case 0x24://����
    case 0xc0://��ʣ������
        tLen=hwSmciApdu(eCpuChan_ESAM,(unsigned char *)Cmd,5,Cmd->Cmd.Data);
        if(tLen>2)
        {
            return 0x100*Cmd->Cmd.Data[Cmd->Cmd.Le]+Cmd->Cmd.Data[Cmd->Cmd.Le+1];
        }
        break;
        
    default:
        tLen=hwSmciApdu(eCpuChan_ESAM,(unsigned char *)Cmd,5+Cmd->Cmd.Le,Cmd->Cmd.Data);        
        
        break;
    }
    if(tLen==2)
    {
        return 0x100*Cmd->Cmd.Data[0]+Cmd->Cmd.Data[1];
    }
    else
        return 0;
}



void cardOpSetValue(eValueType tType,unsigned long tVal)
{
    switch(tType)
    {
    case BASEVAL_Remain:
        countRemainSet(tVal);
        break;
    }
}

void cardDelayMs(unsigned int ms)
{
    
}

unsigned long cardOpGetValue(eValueType tType)
{
    return 0;
}

unsigned char cardOpMeterIsInuse(void)
{
    return 0;
}

void cardOpWriteMeterID(unsigned char *tID,unsigned char tLen)
{
    
}

unsigned char cardOpCheckMeterID(unsigned char *tID,unsigned char tLen)
{
    return 0;
}

unsigned char cardOpRtcIsValid(void)
{
    return 0;
}



#include <k_api.h>
#include "drv_lcd.h"
#include "faultMan.h"
#include "powMan.h"
#include "meterTask.h"
ksem_t      sem_ReadCard;
/**
 * ��������
 * 
 * @author Zzzzz (2018/3/28)
 * 
 * @param arg 
 */
void readCard_task(void *arg)
{
    uint8_t cardRe;
    while (1)
    {
        krhino_sem_take(&sem_ReadCard,RHINO_WAIT_FOREVER);
        communication_report();
        //������ʾ��Ҫ�л�
        dispLockTimeSet(10);
        //����忨�ܹ�����Ĵ���״̬
        faultClear(eFAULT_WaitInv);
        faultClear(eFAULT_WarningClose);
        //�жϵ�ѹ״̬����ѹ�����粻����
        if (powStatusGet()==ePowStatus_Normal || powStatusGet()==ePowStatus_Under)
        {
            OV_LcdClearNum();
            cardRe=UserReadCard();
            if(cardRe!=0)
            {
                OV_DispErrNum(cardRe);
                hwBeep(1,5,0);
            }
            else
            {
                OV_DispEnd();
                hwBeep(1,2,0);
            }
            dispLockTimeSet(2);
        }
    }
}







