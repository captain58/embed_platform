/****************************************Copyright (c)**************************************************
**                                  ________科技有限公司
**                                          开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**�?  �?  �? gpio.c
**�?  �?  �? yzy
**最后修改日�? 2018�?0�?�?
**�?       �? gpio(HAL)
**�?       �?
**--------------历史版本信息----------------------------------------------------------------------------
** 创建�? yzy
** �? �? v1.0
** 日　�? 2018�?0�?�?
** 描　�? 原始版本
********************************************************************************************************/
#define EXT_GPIO

#include "sys.h"
#include "hal.h"

ST_EXIT_INT gs_stExtIntList[MCU_GPIO_INT_NUM];

//IO模拟功能配置:LCD/ADC
void AnalogIO( GPIOx_Type* GPIOx, uint32_t PinNum )
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructureRun;
	
	GPIO_Get_InitPara(GPIOx, PinNum, &GPIO_InitStructureRun);
	
	if( (GPIO_InitStructureRun.Pin		!= PinNum) ||
		(GPIO_InitStructureRun.PxINEN	!= GPIO_IN_Dis) ||
		(GPIO_InitStructureRun.PxODEN	!= GPIO_OD_En) ||
		(GPIO_InitStructureRun.PxPUEN	!= GPIO_PU_Dis) ||
		(GPIO_InitStructureRun.PxFCR	!= GPIO_FCR_ANA) )
	{
		GPIO_InitStructure.Pin = PinNum;
		GPIO_InitStructure.PxINEN = GPIO_IN_Dis;
		GPIO_InitStructure.PxODEN = GPIO_OD_En;
		GPIO_InitStructure.PxPUEN = GPIO_PU_Dis;
		GPIO_InitStructure.PxFCR = GPIO_FCR_ANA;
		
		GPIO_Init(GPIOx, &GPIO_InitStructure);		
	}	
}

//IO输入口配�?
//type 0 = 普�?
//type 1 = 上拉
//#define IN_NORMAL	0
//#define IN_PULLUP	1
void InputtIO( GPIOx_Type* GPIOx, uint32_t PinNum, uint8_t Type )
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructureRun;
	
	GPIO_Get_InitPara(GPIOx, PinNum, &GPIO_InitStructureRun);
		
	if( (GPIO_InitStructureRun.Pin		!= PinNum) ||
		(GPIO_InitStructureRun.PxINEN	!= GPIO_IN_En) ||
		(GPIO_InitStructureRun.PxODEN	!= GPIO_OD_En) ||
		((Type == IN_NORMAL)&&(GPIO_InitStructureRun.PxPUEN != GPIO_PU_Dis)) ||
		((Type == IN_PULLUP)&&(GPIO_InitStructureRun.PxPUEN != GPIO_PU_En)) ||
		(GPIO_InitStructureRun.PxFCR	!= GPIO_FCR_IN) )
	{
		GPIO_InitStructure.Pin = PinNum;	
		GPIO_InitStructure.PxINEN = GPIO_IN_En;
		GPIO_InitStructure.PxODEN = GPIO_OD_En;
		if(Type == IN_NORMAL)		GPIO_InitStructure.PxPUEN = GPIO_PU_Dis;
		else						GPIO_InitStructure.PxPUEN = GPIO_PU_En;	
		GPIO_InitStructure.PxFCR = GPIO_FCR_IN;
		
		GPIO_Init(GPIOx, &GPIO_InitStructure);	
	}
}

//IO输出口配�?
//type 0 = 普�?
//type 1 = OD
//#define OUT_PUSHPULL	0
//#define OUT_OPENDRAIN	1
void OutputIO( GPIOx_Type* GPIOx, uint32_t PinNum, uint8_t Type )
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructureRun;
	
	GPIO_Get_InitPara(GPIOx, PinNum, &GPIO_InitStructureRun);
	
	if( (GPIO_InitStructureRun.Pin		!= PinNum) ||
		(GPIO_InitStructureRun.PxINEN	!= GPIO_IN_Dis) ||
//			((Type == OUT_PUSHPULL)&&(GPIO_InitStructureRun.PxODEN	!= GPIO_OD_Dis)) ||
//			((Type == OUT_OPENDRAIN)&&(GPIO_InitStructureRun.PxODEN	!= GPIO_OD_En)) ||
//			(GPIO_InitStructureRun.PxPUEN	!= GPIO_PU_Dis) ||
//			(GPIO_InitStructureRun.PxFCR	!= GPIO_FCR_OUT) )
//		{
//			GPIO_InitStructure.Pin = PinNum;
//			GPIO_InitStructure.PxINEN = GPIO_IN_Dis;
//			if(Type == OUT_PUSHPULL)	GPIO_InitStructure.PxODEN = GPIO_OD_Dis;
//			else						GPIO_InitStructure.PxODEN = GPIO_OD_En;
//			if( (Type & 0x02) == 0 )	GPIO_InitStructure.PxPUEN = GPIO_PU_Dis;
//			else						GPIO_InitStructure.PxPUEN = GPIO_PU_En;	           
//			GPIO_InitStructure.PxPUEN = GPIO_PU_Dis;
        (((Type & 0x01) == OUT_PUSHPULL)&&(GPIO_InitStructureRun.PxODEN != GPIO_OD_Dis)) ||
        (((Type & 0x01) == OUT_OPENDRAIN)&&(GPIO_InitStructureRun.PxODEN    != GPIO_OD_En)) ||
//          (GPIO_InitStructureRun.PxPUEN   != GPIO_PU_Dis) ||
        (((Type & 0x02) == 0)&&(GPIO_InitStructureRun.PxPUEN    != GPIO_PU_Dis)) ||
        (((Type & 0x02) == 0x02)&&(GPIO_InitStructureRun.PxPUEN != GPIO_PU_En)) ||

        (GPIO_InitStructureRun.PxFCR    != GPIO_FCR_OUT) )
    {
        GPIO_InitStructure.Pin = PinNum;
        GPIO_InitStructure.PxINEN = GPIO_IN_Dis;
        if((Type & 0x01) == OUT_PUSHPULL)   GPIO_InitStructure.PxODEN = GPIO_OD_Dis;
        else                        GPIO_InitStructure.PxODEN = GPIO_OD_En;
        if((Type & 0x02) == 0 ) 
            GPIO_InitStructure.PxPUEN = GPIO_PU_Dis;
        else                        
            GPIO_InitStructure.PxPUEN = GPIO_PU_En;            
        
//      GPIO_InitStructure.PxPUEN = GPIO_PU_Dis;

		GPIO_InitStructure.PxFCR = GPIO_FCR_OUT;
		 
		GPIO_Init(GPIOx, &GPIO_InitStructure);		
	}
}
//IO数字特殊功能�?
//type 0 = 普�?
//type 1 = OD (OD功能仅部分特殊功能支�?
//type 2 = 普�?上拉 
//type 3 = OD+上拉
//#define ALTFUN_NORMAL				0
//#define ALTFUN_OPENDRAIN			1
//#define ALTFUN_PULLUP				2
//#define ALTFUN_OPENDRAIN_PULLUP	3
void AltFunIO( GPIOx_Type* GPIOx, uint32_t PinNum, uint8_t Type  )
{																
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructureRun;
	
	GPIO_Get_InitPara(GPIOx, PinNum, &GPIO_InitStructureRun);
	
	if( (GPIO_InitStructureRun.Pin		!= PinNum) ||
		(GPIO_InitStructureRun.PxINEN	!= GPIO_IN_Dis) ||
		(((Type & 0x01) == 0)&&(GPIO_InitStructureRun.PxODEN	!= GPIO_OD_Dis)) ||
		(((Type & 0x01) != 0)&&(GPIO_InitStructureRun.PxODEN	!= GPIO_OD_En)) ||
		(((Type & 0x02) == 0)&&(GPIO_InitStructureRun.PxPUEN	!= GPIO_PU_Dis)) ||
		(((Type & 0x02) != 0)&&(GPIO_InitStructureRun.PxPUEN	!= GPIO_PU_En)) ||
		(GPIO_InitStructureRun.PxFCR	!= GPIO_FCR_DIG) )
	{
		GPIO_InitStructure.Pin = PinNum;
		GPIO_InitStructure.PxINEN = GPIO_IN_Dis;
		if( (Type & 0x01) == 0 )	GPIO_InitStructure.PxODEN = GPIO_OD_Dis;
		else						GPIO_InitStructure.PxODEN = GPIO_OD_En;
		if( (Type & 0x02) == 0 )	GPIO_InitStructure.PxPUEN = GPIO_PU_Dis;
		else						GPIO_InitStructure.PxPUEN = GPIO_PU_En;	
		GPIO_InitStructure.PxFCR = GPIO_FCR_DIG;
		 
		GPIO_Init(GPIOx, &GPIO_InitStructure);		
	}
}

//IO关闭（od输出高）
//当输入使能有效时，如果外部信号浮空，也可能导致FM385管脚漏电�?
//可以将FCR配置�?1（GPIO输出），ODEN配置�?，即伪开漏输出，关闭上拉使能，并将输出数据设�?
//注意SWD接口的PG8,9如果程序改变它们的配置将无法仿真
void CloseeIO( GPIOx_Type* GPIOx, uint32_t PinNum )
{
	GPIO_InitTypeDef  GPIO_InitStructureRun;
	
	GPIO_Get_InitPara(GPIOx, PinNum, &GPIO_InitStructureRun);
		
	if((GPIO_InitStructureRun.PxFCR	!= GPIO_FCR_OUT))
	{
		GPIO_SetBits(GPIOx, PinNum);
		OutputIO( GPIOx, PinNum, OUT_OPENDRAIN );
	}
	else
	{
		OutputIO( GPIOx, PinNum, OUT_OPENDRAIN );
		GPIO_SetBits(GPIOx, PinNum);		
	}
}

//IO口初始输出状态配�?
void Init_Pad_Io(void)
{	
	GPIOx_DO_Write(GPIOA, 0x0000);	//
	GPIOx_DO_Write(GPIOB, 0x0000);	//
	GPIOx_DO_Write(GPIOC, 0x0000);	//
	GPIOx_DO_Write(GPIOD, 0x0000);	//
	GPIOx_DO_Write(GPIOE, 0x0000);	//
	GPIOx_DO_Write(GPIOF, 0x0000);	//
	GPIOx_DO_Write(GPIOG, 0x0000);	//
}

//关闭80脚芯片不可使用的IO
void Close_None_GPIO_80pin(void)
{
	CloseeIO( GPIOC, GPIO_Pin_0 );
	CloseeIO( GPIOC, GPIO_Pin_1 );

	CloseeIO( GPIOD, GPIO_Pin_8);
	CloseeIO( GPIOD, GPIO_Pin_9 );
	CloseeIO( GPIOD, GPIO_Pin_10 );	
	CloseeIO( GPIOD, GPIO_Pin_11 );	
	CloseeIO( GPIOD, GPIO_Pin_12 );	
	CloseeIO( GPIOD, GPIO_Pin_13 );	
	CloseeIO( GPIOD, GPIO_Pin_14 );	
	CloseeIO( GPIOD, GPIO_Pin_15 );	

	CloseeIO( GPIOE, GPIO_Pin_0 );
	CloseeIO( GPIOE, GPIO_Pin_1 );
	CloseeIO( GPIOE, GPIO_Pin_5 );
	CloseeIO( GPIOE, GPIO_Pin_6 );
	CloseeIO( GPIOE, GPIO_Pin_7 );
	CloseeIO( GPIOE, GPIO_Pin_8 );
	CloseeIO( GPIOE, GPIO_Pin_9 );
	CloseeIO( GPIOE, GPIO_Pin_10 );
	CloseeIO( GPIOE, GPIO_Pin_11 );
	CloseeIO( GPIOE, GPIO_Pin_12 );
	CloseeIO( GPIOE, GPIO_Pin_13 );
	CloseeIO( GPIOE, GPIO_Pin_14 );
	CloseeIO( GPIOE, GPIO_Pin_15 );

	CloseeIO( GPIOF, GPIO_Pin_0 );
	CloseeIO( GPIOF, GPIO_Pin_1 );
	CloseeIO( GPIOF, GPIO_Pin_2 );
	CloseeIO( GPIOF, GPIO_Pin_7 );
	CloseeIO( GPIOF, GPIO_Pin_8 );
	CloseeIO( GPIOF, GPIO_Pin_9 );
	CloseeIO( GPIOF, GPIO_Pin_10 );

	CloseeIO( GPIOG, GPIO_Pin_0 );
	CloseeIO( GPIOG, GPIO_Pin_1 );
	CloseeIO( GPIOG, GPIO_Pin_4 );
	CloseeIO( GPIOG, GPIO_Pin_5 );
	CloseeIO( GPIOG, GPIO_Pin_10 );
	CloseeIO( GPIOG, GPIO_Pin_11 );
	CloseeIO( GPIOG, GPIO_Pin_12 );
	CloseeIO( GPIOG, GPIO_Pin_13 );
	CloseeIO( GPIOG, GPIO_Pin_14 );
	CloseeIO( GPIOG, GPIO_Pin_15 );
}
//校验寄存�?
unsigned char CheckSysReg( __IO uint32_t *RegAddr, uint32_t Value )
{
	if( *RegAddr != Value ) 
	{
		*RegAddr = Value;
		return 1;
	}
	else
	{
		return 0;
	}
}
void Close_AllIOEXTI(void)
{
	CheckSysReg( &GPIO->EXTI0_SEL	, 0xFFFF0000 );
	CheckSysReg( &GPIO->EXTI1_SEL	, 0xFFFF0000 );
	CheckSysReg( &GPIO->EXTI2_SEL	, 0xFFFF0000 );
}

void IO_AnalogFunSet(void)
{
	/* PE4模拟功能选择 */
	GPIO_ANASEL_PE4ANS_Set(GPIO_ANASEL_PE4ANS_ACMP2_INP1);

	/* PE3模拟功能选择 */
	GPIO_ANASEL_PE3ANS_Set(GPIO_ANASEL_PE3ANS_ACMP2_INN1);

	/* PC15模拟功能选择 */
	GPIO_ANASEL_PC15ANS_Set(GPIO_ANASEL_PC15ANS_ACMP2_INP0_ADC_IN6);

	/* PC14模拟功能选择 */
	GPIO_ANASEL_PC14ANS_Set(GPIO_ANASEL_PC14ANS_ACMP2_INN0);

	/* PC13模拟功能选择 */
	GPIO_ANASEL_PC13ANS_Set(GPIO_ANASEL_PC13ANS_ADC_IN2);

	/* PC12模拟功能选择 */
	GPIO_ANASEL_PC12ANS_Set(GPIO_ANASEL_PC12ANS_ADC_IN1);
}

void IO_DFFunSet(void)
{
	GPIO_IODF_SetableEx(IODF_PF3	, DISABLE);
	GPIO_IODF_SetableEx(IODF_PE5	, DISABLE);
	GPIO_IODF_SetableEx(IODF_PE2	, DISABLE);
	GPIO_IODF_SetableEx(IODF_PD3	, DISABLE);
	GPIO_IODF_SetableEx(IODF_PD2	, DISABLE);
	GPIO_IODF_SetableEx(IODF_PD1	, DISABLE);
	GPIO_IODF_SetableEx(IODF_PD0	, DISABLE);
	GPIO_IODF_SetableEx(IODF_PC15	, DISABLE);
	GPIO_IODF_SetableEx(IODF_PC14	, DISABLE);
	GPIO_IODF_SetableEx(IODF_PC13	, DISABLE);
	GPIO_IODF_SetableEx(IODF_PC12	, DISABLE);
	GPIO_IODF_SetableEx(IODF_PB7	, DISABLE);
	GPIO_IODF_SetableEx(IODF_PB6	, DISABLE);
	GPIO_IODF_SetableEx(IODF_PB5	, DISABLE);
	GPIO_IODF_SetableEx(IODF_PB4	, DISABLE);
	GPIO_IODF_SetableEx(IODF_PA11	, DISABLE);
	GPIO_IODF_SetableEx(IODF_PA10	, DISABLE);
	GPIO_IODF_SetableEx(IODF_PA9	, DISABLE);
	GPIO_IODF_SetableEx(IODF_PA8	, DISABLE);
}

void IO_WKENFunSet(void)
{
	GPIO_PINWKEN_SetableEx(PINWKEN_PD6	, DISABLE);
	GPIO_PINWKEN_SetableEx(PINWKEN_PE9	, DISABLE);
	GPIO_PINWKEN_SetableEx(PINWKEN_PE2	, DISABLE);
	GPIO_PINWKEN_SetableEx(PINWKEN_PA13	, DISABLE);
	GPIO_PINWKEN_SetableEx(PINWKEN_PG7	, DISABLE);
	GPIO_PINWKEN_SetableEx(PINWKEN_PC13	, DISABLE);
	GPIO_PINWKEN_SetableEx(PINWKEN_PB0	, DISABLE);
	GPIO_PINWKEN_SetableEx(PINWKEN_PF5	, DISABLE);
}

//关闭80脚芯片可使用的全部IO(SWD口除外，关闭后不能仿�?
void Close_AllIO_GPIO_80pin( void )
{	
	//关闭所有IO的中断功�?
	Close_AllIOEXTI();
	
	//强驱动关�?
	GPIO_HDSEL_PE2HDEN_Setable(DISABLE);
	GPIO_HDSEL_PG6HDEN_Setable(DISABLE);
	
	//FOUT输出信号选择
	GPIO_FOUTSEL_FOUTSEL_Set(GPIO_FOUTSEL_FOUTSEL_LSCLK);
	
	//模拟口液晶、AD功能选择	
	IO_AnalogFunSet();
	
	//部分IO输入口数字滤波功�?
	IO_DFFunSet();
	
	//关闭NWKUP
	IO_WKENFunSet();
	
	CloseeIO( GPIOA, GPIO_Pin_0 );	//PA0; //
	CloseeIO( GPIOA, GPIO_Pin_1 );	//PA1; //
	CloseeIO( GPIOA, GPIO_Pin_2 );	//PA2; //
	CloseeIO( GPIOA, GPIO_Pin_3 );	//PA3; //
	CloseeIO( GPIOA, GPIO_Pin_4 );	//PA4; //
	CloseeIO( GPIOA, GPIO_Pin_5 );	//PA5; //
	CloseeIO( GPIOA, GPIO_Pin_6 );	//PA6; //
	CloseeIO( GPIOA, GPIO_Pin_7 );	//PA7; //
	CloseeIO( GPIOA, GPIO_Pin_8 );	//PA8; //
	CloseeIO( GPIOA, GPIO_Pin_9 );	//PA9; //
	CloseeIO( GPIOA, GPIO_Pin_10 );	//PA10;//
	CloseeIO( GPIOA, GPIO_Pin_11 );	//PA11;//
	CloseeIO( GPIOA, GPIO_Pin_12 );	//PA12;//
	CloseeIO( GPIOA, GPIO_Pin_13 );	//PA13;//
	CloseeIO( GPIOA, GPIO_Pin_14 );	//PA14;//
	CloseeIO( GPIOA, GPIO_Pin_15 );	//PA15;//

	CloseeIO( GPIOB, GPIO_Pin_0 );	//PB0; //
	CloseeIO( GPIOB, GPIO_Pin_1 );	//PB1; //
	CloseeIO( GPIOB, GPIO_Pin_2 );	//PB2; //
	CloseeIO( GPIOB, GPIO_Pin_3 );	//PB3; //
	CloseeIO( GPIOB, GPIO_Pin_4 );	//PB4; //
	CloseeIO( GPIOB, GPIO_Pin_5 );	//PB5; //
	CloseeIO( GPIOB, GPIO_Pin_6 );	//PB6; //
	CloseeIO( GPIOB, GPIO_Pin_7 );	//PB7; //
	CloseeIO( GPIOB, GPIO_Pin_8 );	//PB8; //
	CloseeIO( GPIOB, GPIO_Pin_9 );	//PB9; //
	CloseeIO( GPIOB, GPIO_Pin_10 );	//PB10;//
	//CloseeIO( GPIOB, GPIO_Pin_11 );	//PB11;//
	CloseeIO( GPIOB, GPIO_Pin_12 );	//PB12;//
	CloseeIO( GPIOB, GPIO_Pin_13 );	//PB13;//
	CloseeIO( GPIOB, GPIO_Pin_14 );	//PB14;//
	CloseeIO( GPIOB, GPIO_Pin_15 );	//PB15;//
	
	CloseeIO( GPIOC, GPIO_Pin_2 );	//PC2; //
	CloseeIO( GPIOC, GPIO_Pin_3 );	//PC3; //
	CloseeIO( GPIOC, GPIO_Pin_4 );	//PC4; //
	CloseeIO( GPIOC, GPIO_Pin_5 );	//PC5; //
	CloseeIO( GPIOC, GPIO_Pin_6 );	//PC6; //
	CloseeIO( GPIOC, GPIO_Pin_7 );	//PC7; //
	CloseeIO( GPIOC, GPIO_Pin_8 );	//PC8; //
	CloseeIO( GPIOC, GPIO_Pin_9 );	//PC9; //
	CloseeIO( GPIOC, GPIO_Pin_10 );	//PC10;//
	CloseeIO( GPIOC, GPIO_Pin_11 );	//PC11;//
	CloseeIO( GPIOC, GPIO_Pin_12 );	//PC12;//
	CloseeIO( GPIOC, GPIO_Pin_13 );	//PC13;//
	CloseeIO( GPIOC, GPIO_Pin_14 );	//PC14;//
	CloseeIO( GPIOC, GPIO_Pin_15 );	//PC15;//
	
	CloseeIO( GPIOD, GPIO_Pin_0 );	//PD0;//
	CloseeIO( GPIOD, GPIO_Pin_1 );	//PD1;//
	CloseeIO( GPIOD, GPIO_Pin_2 );	//PD2;//
	CloseeIO( GPIOD, GPIO_Pin_3 );	//PD3;//
	CloseeIO( GPIOD, GPIO_Pin_4 );	//PD4;//
	CloseeIO( GPIOD, GPIO_Pin_5 );	//PD5;//
	CloseeIO( GPIOD, GPIO_Pin_6 );	//PD6;//
	CloseeIO( GPIOD, GPIO_Pin_7 );	//PD7;//
	
	CloseeIO( GPIOE, GPIO_Pin_2 );	//PE2;//
	CloseeIO( GPIOE, GPIO_Pin_3 );	//PE3;//
	CloseeIO( GPIOE, GPIO_Pin_4 );	//PE4;//

	CloseeIO( GPIOF, GPIO_Pin_3 );	//PF3; //
	CloseeIO( GPIOF, GPIO_Pin_4 );	//PF4; //
	CloseeIO( GPIOF, GPIO_Pin_5 );	//PF5; //
	CloseeIO( GPIOF, GPIO_Pin_6 );	//PF6; //
	CloseeIO( GPIOF, GPIO_Pin_11 );	//PF11;//
	CloseeIO( GPIOF, GPIO_Pin_12 );	//PF12;//
	CloseeIO( GPIOF, GPIO_Pin_13 );	//PF13;//
	CloseeIO( GPIOF, GPIO_Pin_14 );	//PF14;//
	CloseeIO( GPIOF, GPIO_Pin_15 );	//PF15;//
	
	CloseeIO( GPIOG, GPIO_Pin_2 );	//PG2;//
	CloseeIO( GPIOG, GPIO_Pin_3 );	//PG3;//
	CloseeIO( GPIOG, GPIO_Pin_6 );	//PG6;//
	CloseeIO( GPIOG, GPIO_Pin_7 );	//PG7;//
	//注意SWD接口的PG8,9如果程序改变它们的配置将无法仿真
	AltFunIO( GPIOG, GPIO_Pin_8, ALTFUN_NORMAL );	//PG8;//SWDTCK
	AltFunIO( GPIOG, GPIO_Pin_9, ALTFUN_NORMAL );	//PG9;//SWDTDO	
//		AltFunIO( GPIOB, GPIO_Pin_11, ALTFUN_NORMAL );
//	    GPIO_ResetBits(GPIOB, GPIO_Pin_11);
}




void HAL_GPIO_PinConfig(GPIOx_Type* GPIOx, uint8_t Pin, uint8_t Type, uint8_t analogen, uint8_t dir)
{
    uint32_t pinbit = 1 << Pin;
    if(analogen)
    {
        if(GPIOx == GPIOE && pinbit == GPIO_Pin_4)
        {
            GPIO_ANASEL_PE4ANS_Set(GPIO_ANASEL_PE4ANS_SEG19);
        }
        else if(GPIOx == GPIOE && pinbit == GPIO_Pin_3)
        {
            GPIO_ANASEL_PE3ANS_Set(GPIO_ANASEL_PE3ANS_SEG18);
        }
        else if(GPIOx == GPIOC && pinbit == GPIO_Pin_12)
        {
            GPIO_ANASEL_PC12ANS_Set(GPIO_ANASEL_PC12ANS_SEG36);
        }
        else if(GPIOx == GPIOC && pinbit == GPIO_Pin_13)
        {
            GPIO_ANASEL_PC13ANS_Set(GPIO_ANASEL_PC13ANS_SEG37);
        }
        else if(GPIOx == GPIOC && pinbit == GPIO_Pin_14)
        {
            GPIO_ANASEL_PC14ANS_Set(GPIO_ANASEL_PC14ANS_SEG38);
        }
        else if(GPIOx == GPIOC && pinbit == GPIO_Pin_15)
        {
            GPIO_ANASEL_PC15ANS_Set(GPIO_ANASEL_PC15ANS_SEG39);
        }    
        AnalogIO(GPIOx, pinbit);
    }
    else
    {
        if(dir == 1)
        {
            AltFunIO(GPIOx, pinbit, Type);
//	            HAL_GPIO_SetPinState(GPIOx, pinbit, 0);
        }
        else if(dir == 2)
        {
            OutputIO(GPIOx, pinbit, Type);
            HAL_GPIO_SetPinState(GPIOx, pinbit, 0);
        }
        else
        {
            InputtIO(GPIOx,pinbit,Type);
        }
    }

}
//	typedef enum
//	{ 
//	    EXTI_RISING,        //上升�?
//	    EXTI_FALLING,           //下降�?
//	    EXTI_BOTH,              //上升和下降沿
//	    EXTI_DISABLE,
//		
//	}GPIOExtiEdge;


void GPIO_IRQHandler(void)
{
    SYS_LockMMTK();
    for(int i = 0; i < MCU_GPIO_INT_NUM; i++)
    {
    	if(gs_stExtIntList[i].gpio != NULL && 
            SET == GPIO_EXTI_EXTIxIF_ChkEx(gs_stExtIntList[i].gpio, 1 << gs_stExtIntList[i].pin))
    	{
    		GPIO_EXTI_EXTIxIF_ClrEx(gs_stExtIntList[i].gpio, 1 << gs_stExtIntList[i].pin);
            if(gs_stExtIntList[i].func != NULL)
            {
                ((void(*)())gs_stExtIntList[i].func)();
            }
    	}
    }
    SYS_UnLockMMTK();
}
uint8_t HAL_GPIO_Ext_Int(uint8_t channel, GPIOx_Type* GPIOx, uint8_t Pin, uint8_t Type, IRQServerFT func)
{
    uint32_t pinbit = 1 << Pin;
    //uint8_t edge = Type & 0x0F;
    uint8_t NWKUP = Type >> 4;
    if(channel >= MCU_GPIO_INT_NUM) return 1; //越界
    RCC_PERCLK_SetableEx(EXTI2CLK, ENABLE);		//EXTI外部引脚中断采样时钟，IO数字滤波时钟使能
    RCC_PERCLK_SetableEx(EXTI1CLK, ENABLE);		//EXTI外部引脚中断采样时钟，IO数字滤波时钟使能
    RCC_PERCLK_SetableEx(EXTI0CLK, ENABLE);		//EXTI外部引脚中断采样时钟，IO数字滤波时钟使能
    RCC_PERCLK_SetableEx(PDCCLK, ENABLE);		//IO控制时钟寄存器使�?

    gs_stExtIntList[channel].gpio = GPIOx;
    gs_stExtIntList[channel].pin = Pin;
    gs_stExtIntList[channel].type = Type;
    gs_stExtIntList[channel].func = func;

    if(NWKUP)
    {
        if(GPIOx == GPIOD && pinbit == GPIO_Pin_6)
        {
            GPIO_PINWKEN_SetableEx(PINWKEN_PD6  , ENABLE);
        }
        else if(GPIOx == GPIOE && pinbit == GPIO_Pin_9)
        {
            GPIO_PINWKEN_SetableEx(PINWKEN_PE9  , ENABLE);
        }
        else if(GPIOx == GPIOE && pinbit == GPIO_Pin_2)
        {
            GPIO_PINWKEN_SetableEx(PINWKEN_PE2  , ENABLE);
        }        
        else if(GPIOx == GPIOA && pinbit == GPIO_Pin_13)
        {
            GPIO_PINWKEN_SetableEx(PINWKEN_PA13  , ENABLE);
        }
        else if(GPIOx == GPIOG && pinbit == GPIO_Pin_7)
        {
            GPIO_PINWKEN_SetableEx(PINWKEN_PG7  , ENABLE);
        }        
        else if(GPIOx == GPIOC && pinbit == GPIO_Pin_13)
        {
            GPIO_PINWKEN_SetableEx(PINWKEN_PC13  , ENABLE);
        }
        else if(GPIOx == GPIOB && pinbit == GPIO_Pin_0)
        {
            GPIO_PINWKEN_SetableEx(PINWKEN_PB0  , ENABLE);
        }         
        else if(GPIOx == GPIOF && pinbit == GPIO_Pin_5)
        {
            GPIO_PINWKEN_SetableEx(PINWKEN_PF5  , ENABLE);
        }            
    }
    
	GPIO_EXTI_Init( GPIOx, pinbit, (GPIOExtiEdge)Type);	//PB0输入中断打开
    /*NVIC中断配置*/
	NVIC_DisableIRQ(GPIO_IRQn);
	NVIC_SetPriority(GPIO_IRQn,2);//中断优先级配�?
	NVIC_EnableIRQ(GPIO_IRQn);
    return 0;
}

void HAL_InitGPIO()
{
	/*外设初始化配�?/
	Init_Pad_Io();				//IO口输出寄存器初始状态配�?
	Close_None_GPIO_80pin();    //关闭80脚芯片不支持的IO
	Close_AllIO_GPIO_80pin();   //关闭全部IO
	
	/*在此添加用户初始化代�?/
//		Init_IO();
    memset((uint8_t *)gs_stExtIntList, 0, sizeof(ST_EXIT_INT) * MCU_GPIO_INT_NUM);

}

