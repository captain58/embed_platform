#ifndef __DRV_DISP_H__
#define __DRV_DISP_H__

#ifndef EXT_DEV_LCD
#define EXT_DEV_LCD extern
#endif


#define DefaultLine 0
#define ExtLine     1

#define LCD_DISP_BIT_0      0x01
#define LCD_DISP_BIT_1      0x02
#define LCD_DISP_BIT_2      0x04
#define LCD_DISP_BIT_3      0x08
#define LCD_DISP_BIT_4      0x10
#define LCD_DISP_BIT_5      0x20
#define LCD_DISP_BIT_6      0x40
#define LCD_DISP_BIT_7      0x80

#define UNKNOW              24
#define LCD_CHAR_BLANK      24
#define LCD_CHAR_SEPARATE   16
#define LCD_CHAR_A          10
#define LCD_CHAR_b          11
#define LCD_CHAR_C          12
#define LCD_CHAR_c          31
#define LCD_CHAR_d          13
#define LCD_CHAR_E          14
#define LCD_CHAR_F          15
#define LCD_CHAR_G          26
#define LCD_CHAR_H          17
#define LCD_CHAR_h          UNKNOW
#define LCD_CHAR_I          1
#define LCD_CHAR_i          30
#define LCD_CHAR_J          UNKNOW
#define LCD_CHAR_K          UNKNOW
#define LCD_CHAR_L          18
#define LCD_CHAR_M          UNKNOW
#define LCD_CHAR_N          27
#define LCD_CHAR_n          19
#define LCD_CHAR_O          0
#define LCD_CHAR_o          23
#define LCD_CHAR_P          21
#define LCD_CHAR_Q          UNKNOW
#define LCD_CHAR_R          29
#define LCD_CHAR_r          22
#define LCD_CHAR_S          5
#define LCD_CHAR_t          25
#define LCD_CHAR_U          28
#define LCD_CHAR_u          20
#define LCD_CHAR_V          UNKNOW
#define LCD_CHAR_W          UNKNOW
#define LCD_CHAR_X          UNKNOW
#define LCD_CHAR_Y          UNKNOW
#define LCD_CHAR_Z          UNKNOW

typedef enum
{
    MARK_AMOUNT,    //�ۼ�
    MARK_PRICE,     //����
    MARK_REMAIN,    //ʣ��
    MARK_STEP,      //����
    MARK_SIGN1,     //�ź�1
    MARK_SIGN2,     //�ź�2
    MARK_SIGN3,		//�ź�3
    MARK_ERR,       //�澯
    MARK_GAS,       //����
    //MARK_CURRENT,   //��ǰ
    MARK_MONEY,     //���
    //MARK_PAST,      //��
    MARK_MONTH,     //��
    MARK_OPEN,      //���ſ�
    MARK_CLOSE,     //���Ź�
    MARK_BATT1,     //����1
    MARK_BATT2,     //����2
    MARK_BATT3,     //����3
    MARK_BATT_FREAM,//��ؿ�
    //MARK_CHECK,     //���
    MARK_CONSUMED,   //����
    MARK_FREEZE,     //����
    MARK_BACKSLASH,      //��б��
    //MARK_PRESS,     //ѹ��
    MARK_CHARGE,      //��ֵ
    MARK_YAN,       //Ԫ
    MARK_M3,        //������
    MARK_1,         //�·�ʮλ
   MARK_2POINT,    //����
}_EnumMark;

//bsp�ӿ�
extern void OV_LcdOn(void);
extern void OV_LcdOff(void);
extern void OV_LcdInit(void);


//Ӧ�ýӿ�
void OV_LcdDispAll(void);                                     //ȫ��
void OV_LcdClear(void);                                       //����
void OV_LcdRefresh(void);               //ˢ��
void OV_LcdClearNum();                                        //�����ֵ
void OV_LcdDispChar(unsigned char index, unsigned char ch);   //��ʾ�ַ�
void OV_LcdDispMark(_EnumMark MarkIndex,char t);              //��ʾ��ʶ
void OV_LcdDispNumWithDot(long num,char dots);                //��ʾ��С�������ֵ
void OV_DispRemainGas(long num,char dots,unsigned int b);     //��ʾʣ������  num-��ֵ  dots-С����λ��  b-�·� 
															                  //��b����12�����0ʱ����ʾ�·�
void OV_DispRemainMoney(long num,char dots,unsigned int b);   //��ʾʣ����
void OV_DispConsumedGas(long num,char dots,unsigned int b);   //��ʾ��������
void OV_DispConsumedMoney(long num,char dots,unsigned int b); //��ʾ���ѽ��
void OV_DispChargeGas(long num,char dots,unsigned int b);     //��ʾ��ֵ����
void OV_DispChargeMoney(long num,char dots,unsigned int b);   //��ʾ��ֵ���
void OV_DispAmountGas(long num,char dots,unsigned int b);     //��ʾ�ۼ�����
void OV_DispAmountMoney(long num,char dots,unsigned int b);   //��ʾ�ۼƽ��
void OV_DispFreezeGas(long num,char dots,unsigned int b);     //��ʾ��������
void OV_DispFreezeMoney(long num,char dots,unsigned int b);   //��ʾ������
void OV_DispPrice(long num,char dots);                        //��ʾ����
void OV_DispErr(void);                                        //��ʾ����
void OV_DispValve(char c);                                    //��ʾ��/�ط�״̬
void OV_DispBatt(unsigned int i);                             //��ʾ���� 
void OV_DispSign(unsigned int i);                             //��ʾ�ź�
void OV_DispMonth(unsigned int b);                             //��ʾ�·�
void OV_DispTime(unsigned int a,unsigned int b,unsigned int c);//��ʾʱ��
void OV_DispDate(unsigned int n,unsigned int m,unsigned int k);//��ʾ����
void OV_DispHello(void);                                       //��ʾ"HELLO"
void OV_DispEnd(void);                                         //��ʾ"End"
void OV_DispStepMoney(long num,char dots,unsigned int b);      //��ʾ���ݽ��
void OV_DispStepGas(long num,char dots,unsigned int b);        //��ʾ��������
void OV_DispStepRemainGas(long num,char dots,unsigned int b);  //��ʾ����ʣ������
void OV_DispStepRemainMoney(long num,char dots,unsigned int b);//��ʾ����ʣ����

void OV_DispSucc(void);
void OV_DispFail(void);
void OV_LcdDispPara1(unsigned char ch1,unsigned long num);
void OV_LcdDispPara2(unsigned char ch1,unsigned char ch2,unsigned int num);
void OV_LcdDispIP(unsigned char c,unsigned char ip0,unsigned char ip1);

//add 2014-12-23
void OV_DispStepNVol(long num,char dots,unsigned char b);
void OV_DispStepNMoney(long num,char dots,unsigned char b);
void OV_DispStepNRemainVol(long num,char dots,unsigned char b);
void OV_DispStepNRemainMoney(long num,char dots,unsigned char b);
void OV_DispStepNPrice(long num,char dots,char b);

void OV_LcdClearValueMark(void);
void LcdDispChar(unsigned char index, unsigned char ch);
void OV_DispErrNum(unsigned char ErrNum);


void LCDTest(void);
void OV_DispLoading(unsigned char step);



#if SYS_LCD_EN >0
typedef struct
{

    /************************************************************************
     * @function: on
     * @����: lcd��
     * 
     * @����: 
     * @param: 
     * 
     * @����: 
     * @return:   
     * @˵��: 
     * @����: yzy (2018/5/2)
     *-----------------------------------------------------------------------
     * @�޸���: 
     ************************************************************************/
    void (*on)(void);
    
    /************************************************************************
     * @function: off
     * @����: lcd��
     * 
     * @����: 
     * @param: 
     * 
     * @����: 
     * @return:   
     * @˵��: 
     * @����: yzy (2018/5/2)
     *-----------------------------------------------------------------------
     * @�޸���: 
     ************************************************************************/
    void (*off)(void);    
    /************************************************************************
     * @function: init
     * @����: lcd��ʼ��
     * 
     * @����: 
     * @param: 
     * 
     * @����: 
     * @return:   
     * @˵��: 
     * @����: yzy (2018/5/2)
     *-----------------------------------------------------------------------
     * @�޸���: 
     ************************************************************************/
    void (*init)(void);
    /************************************************************************
     * @function: clearNum
     * @����: �����Ļ������
     * 
     * @����: 
     * @param: 
     * 
     * @����: 
     * @return:   
     * @˵��: 
     * @����: yzy (2018/5/2)
     *-----------------------------------------------------------------------
     * @�޸���: 
     ************************************************************************/
    void (*clearNum)(void);
    
    void (*dispLoading)(unsigned char step);
    void (*dispEnd)(void);
    void (*dispErrNum)(unsigned char ErrNum);

}_F_LCD;
EXT_DEV_LCD const _F_LCD gs_Lcd;
#endif


#endif
