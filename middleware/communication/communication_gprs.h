#ifndef __COMMUNICATION_GPRS_H__
#define __COMMUNICATION_GPRS_H__

#define eCommEvent_Report   0x0001



void communication_report(void);
void commApp_task(void *arg);
void communication_close(void);


#endif


