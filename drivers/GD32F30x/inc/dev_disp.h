#ifndef __DEV_DISP_H
#define __DEV_DISP_H

// LCD��ʾ�ڴ��С
#define LCD_CHIP_RAMSIZE	6

// LCD��ʾ�ڴ���������
#define LCD_DATARAM_CLEAR	0x00
#define LCD_DATARAM_TEST	0xFFFFFFFF

// LCD��ʾ���Ŷζ���
// BIT0~4 : RAM��Ӧ��λ0~31
// BIT8~15: RAMƫ����
#define LCD_SEGMENT_LOGO		0x00E	// CHINT�̱�
#define LCD_SEGMENT_DOTS_BIT1	0x40A	// ��һλС��
#define LCD_SEGMENT_DOTS_BIT2	0x210	// �ڶ�λС��
#define LCD_SEGMENT_DOTS_BIT3	0x00F	// ����λС��
#define LCD_SEGMENT_UNIT_M3		0x010	// ��λ��������
#define LCD_SEGMENT_UNIT_RMB	0x416	// ��λ��Ԫ
#define LCD_SEGMENT_UNIT_PRICE	0x110	// ��λ��/
#define LCD_SEGMENT_UNIT_TEMP	0x50E	// ��λ�����϶�
#define LCD_SEGMENT_UNIT_KPA	0x50F	// ��λ��kPa
#define LCD_SEGMENT_UNIT_RATE	0x502	// ��λ��%
#define LCD_SEGMENT_YEAR		0x10E	// ��
#define LCD_SEGMENT_MONTH		0x20E	// ��
#define LCD_SEGMENT_TOTAL		0x30E	// �ۼ�
#define LCD_SEGMENT_REMAIN		0x20F	// ʣ��
#define LCD_SEGMENT_PURCHASE	0x30F	// ����
#define LCD_SEGMENT_ST_USED		0x310	// ����
#define LCD_SEGMENT_ST_TEST		0x10F	// ���
#define LCD_SEGMENT_ST_ERROR	0x50D	// �쳣
#define LCD_SEGMENT_ST_PROTECT	0x301	// ǿ��
#define LCD_SEGMENT_ST_CLOSE	0x303	// ����
#define LCD_SEGMENT_ST_OPEN		0x305	// ����
#define LCD_SEGMENT_BATT_LOW	0x307	// �����
#define LCD_SEGMENT_ALARM		0x309	// ���ֵ
#define LCD_SEGMENT_BATT_CHARGE	0x30B	// �����
#define LCD_SEGMENT_RSSI_S1		0x00D	// �ź�ǿ��S1
#define LCD_SEGMENT_RSSI_S2		0x10D	// �ź�ǿ��S2
#define LCD_SEGMENT_RSSI_S3		0x20D	// �ź�ǿ��S3
#define LCD_SEGMENT_RSSI_S4		0x30D	// �ź�ǿ��S4

// Define Display Status Information
#define DISP_ST_BIT_LOGO		0x0001
#define DISP_ST_BIT_YEAR		0x0002
#define DISP_ST_BIT_MONTH		0x0004
#define DISP_ST_BIT_TOTAL		0x0008
#define DISP_ST_BIT_REMAIN		0x0010
#define DISP_ST_BIT_PURCHASE	0x0020
#define DISP_ST_BIT_USED		0x0040
#define DISP_ST_BIT_TEST		0x0080
#define DISP_ST_BIT_ERROR		0x0100
#define DISP_ST_BIT_PROTECT		0x0200
#define DISP_ST_BIT_CLOSE		0x0400
#define DISP_ST_BIT_OPEN		0x0800
#define DISP_ST_BIT_BATT_LOW	0x1000
#define DISP_ST_BIT_ALARM		0x2000
#define DISP_ST_BIT_BATT_CHARGE	0x4000
#define DISP_ST_BIT_RSSI_S1		0x8000
#define DISP_ST_BIT_RSSI_S2		0x010000
#define DISP_ST_BIT_RSSI_S3		0x020000
#define DISP_ST_BIT_RSSI_S4		0x040000

// Define Units
typedef enum
{
	DISP_UNITS_NULL,		// �޵�λ
	DISP_UNITS_VOLUME,		// ������
	DISP_UNITS_AMOUNT,		// ���
	DISP_UNITS_PRICE,		// ����
	DISP_UNITS_TEMPERATURE,	// �¶�
	DISP_UNITS_PRESSURE,	// ѹ��
	DISP_UNITS_PERCENTAGE,	// �ٷֱ�
} EN_DISP_UNITS;

// Define Display Data Mode
typedef enum
{
	DISP_MODE_HEX,
	DISP_MODE_DEC,
	DISP_MODE_STR,
} EN_DISP_MODE;

typedef struct StApiDispWriteHexPara
{
	uint32_t buf;
	uint8_t  num;
	EN_DISP_UNITS unit;
	uint32_t stBits;
} STAPI_DISPWRITEHEX_PARA;

typedef struct StApiDispWriteDecPara
{
	uint32_t buf;
	uint8_t  dots;
	EN_DISP_UNITS unit;
	uint32_t stBits;
} STAPI_DISPWRITEDEC_PARA;

typedef struct StApiDispWriteStrPara
{
	char *str;
	EN_DISP_UNITS unit;
	uint32_t stBits;
} STAPI_DISPWRITESTR_PARA;

//	// Define Units
//	typedef enum
//	{
//		DISP_UNITS_NULL,		// �޵�λ
//		DISP_UNITS_VOLUME,		// ������
//		DISP_UNITS_AMOUNT,		// ���
//		DISP_UNITS_PRICE,		// ����
//		DISP_UNITS_TEMPERATURE,	// �¶�
//		DISP_UNITS_PRESSURE,	// ѹ��
//		DISP_UNITS_PERCENTAGE,	// �ٷֱ�
//	} EN_DISP_UNITS;


extern void HAL_LcdInit(void);
extern void HAL_LcdDeInit(void);
extern void HAL_LcdSetValue(uint32_t val);
extern void HAL_LcdOn(void);
extern void HAL_LcdOff(void);
extern void HAL_LcdWrite(uint8_t index, uint8_t val);
extern void HAL_LcdSetSegment(uint16_t mark);
extern void HAL_LcdSetDots(uint8_t dots);
extern void HAL_LcdSetUnitStBits(EN_DISP_UNITS unit, uint32_t stBits);
extern void HAL_LcdWriteHex(uint32_t val, uint8_t num, EN_DISP_UNITS unit, uint32_t stBits);
extern void HAL_LcdWriteDec(int32_t val, uint8_t dots, EN_DISP_UNITS unit, uint32_t stBits);
extern void HAL_LcdWriteStr(char *str, EN_DISP_UNITS unit, uint32_t stBits);

extern void ApiDispInit(void);
extern void ApiDispDeInit(void);
extern void ApiDispOn(void);
extern void ApiDispOff(void);
extern void ApiDispClear(void);
extern void ApiDispTest(void);
extern void ApiDispWriteHex(uint32_t para);
extern void ApiDispWriteDec(uint32_t para);
extern void ApiDispWriteStr(uint32_t para);

#endif
