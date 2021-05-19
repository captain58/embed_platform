#ifndef _IAP_TASK_H     
#define _IAP_TASK_H

#include "Parameter.h"

extern void IAPTask(void);
extern void JumpToApplication(void);
extern IAP_FROM_FLAG Judge_Iap_From(void);
extern void WriteExFlashOK(void);

#endif
