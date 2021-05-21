#ifndef __DEV_METERING_H
#define __DEV_METERING_H

#define METER_SENSOR_CH_A	0
#define METER_SENSOR_CH_B	1

extern void DEV_MeteringGpioInit(void);
extern void DEV_Metering_CHA_IRQ(void * para);
extern void DEV_Metering_CHB_IRQ(void * para);

#endif
