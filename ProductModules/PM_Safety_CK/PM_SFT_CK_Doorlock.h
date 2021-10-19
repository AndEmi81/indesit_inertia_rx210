/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		PM_Safety_CK_DL
\n
\n
\file		PM_SFT_CK_Doorlock.c
\ingroup	PM_Safety_CK
\date		13/02/2013
\author		Marco Sperandini
\n
*/

#ifndef PM_Safety_DOORLOCK_H_
#define PM_Safety_DOORLOCK_H_

#include "Config.h"
#include "LIB_ProductSafety.h"
#include "LoadsRequest.h"
/**
\addtogroup PM_Safety_CK
@{*/
#define FAULTSRC_DOORLOCK_CANNOT_CLOSE	1
#define FAULTSRC_DOORLOCK_CANNOT_OPEN	2
#define FAULTSRC_DOORLOCK_FEEDBACK_KO	3
#define FAULTSRC_DOORLOCK_BAD_SETTING	4
#define FAULTSRC_DOORLOCK_OVER_TEMP		5
#define FAULTSRC_DOORLOCK_TEMP_MON		6

uint16_t PM_SFT_CK_Doorlock_GetTime (sft_module_status_t *status);
void PM_SFT_CK_Doorlock_Monitor (iom_loads_struct_t *newLoads, sft_module_status_t *status, uint8_t applReqFlags);
void PM_SFT_CK_Doorlock_Init( sft_module_status_t *safetyData );
/*@}*/
#endif /* PM_Safety_DOORLOCK_H_ */
