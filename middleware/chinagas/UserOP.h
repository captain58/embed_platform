#ifndef __USEROP_H__
#define __USEROP_H__
#include "cardapp.h"

#define ESAM_UPDATE_ALL         0xff
#define ESAM_UPDATE_REMAIN      0x01
#define ESAM_UPDATE_APP_INFO    0x02
#define ESAM_UPDATE_OP_INFO     0x04
#define ESAM_UPDATE_CHARGE_INFO 0x08

#define ESAM_TRAN_MARK_CAN_TRAN     0x11223344
#define ESAM_TRAN_MARK_HAD_TRAN     0x33445522
#define ESAM_TRAN_MARK_LOCK_TRAN    0x99665754

unsigned char SaveToEsam(void);
unsigned char UpdateFromEsam(unsigned char updateCode);
unsigned char SetMeterTime(unsigned char *tTime);

unsigned char CheckCardDate(unsigned char *tEffectiveDate, unsigned char *tInvaildDate);

void EsamClearRemain(void);
unsigned char EsamWriteRemain(unsigned char type, long tRemain);
unsigned char EsamReadRemain(long *tReturn);

void EsamSetTranMark(unsigned long tlong);
void EsamGetTranMark(unsigned long *tlong);

unsigned char MeterCharge(unsigned char tChargeType, unsigned long T_charge);
void EsamClearBalance(void);
#endif
