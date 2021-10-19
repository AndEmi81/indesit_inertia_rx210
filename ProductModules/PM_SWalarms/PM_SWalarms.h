/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par		PM_SWalarms.h
\n			header file of PM_SWalarms.c
\n
\file		PM_SWalarms.h
\ingroup	PM_SwAlarms
\date		Feb 15, 2011
\version	01.00.00
\author		stefano.ughi
\n
\n
\par	History:
\n
\n 		01.00.00
\n		Feb 15, 2011
\n		stefano.ughi
\n		changes
\n 			Creation
\n
*/

#ifndef __PM_SW_ALARMS_H
#define __PM_SW_ALARMS_H

/* ********** */
/* Includes */
/* ********* */
#include "StandardTypes.h"
#include "OCTPM_drv.h"


/* ****************** */
/* Defines and Macros */
/* ****************** */
/*<belonging to the module, visible to the whole Platform project>*/

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/*<belonging to the module, visible to the Platform project>*/
typedef uint8_t SW_AlarmType;

/* ****************** */
/* Exported variables */
/* ****************** */
/*<belonging to the module, visible to the Platform project, keyword extern used>*/

/* ******************* */
/* Function prototypes */
/* ******************* */
/*<belonging to the module, visible to the Platform project>*/
IO_HwTimerType PM_SWalarmsConvNsecInTicks(IO_TimingValueType timeNsec);
void PM_SWalarmsSet( SW_AlarmType swAlarmId, IO_HwTimerType alarm);
void PM_SWalarmsCheck(SW_AlarmType swAlarmId);
bool_t PM_SWalarms_IsAlarmExpired(SW_AlarmType swAlarmId);

#endif /* __PM_SW_ALARMS_H */

/* *********** */
/* End of file */
/* *********** */
