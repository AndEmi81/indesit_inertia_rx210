/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par		PM_TaskMonitoring.h
\n			header file of PM_TaskMonitoring.c
\n
\file		PM_TaskMonitoring.h
\ingroup	PM_TaskMonitoring
\date		Mar 28, 2011
\version	01.00.00
\author		stefano.ughi
\n
\n
\par	History:
\n
\n 		01.00.00
\n		Mar 28, 2011
\n		stefano.ughi
\n		changes
\n 			Creation
\n
*/

#ifndef __TASKS_MONITOR_H
#define __TASKS_MONITOR_H

/* ******** */
/* Includes */
/* ******** */
#include "StandardTypes.h"
#include "TaskMonitor_Cfg.h"
#include "PM_TaskMonitoringExp.h"
/* ****************** */
/* Defines and Macros */
/* ****************** */


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */
bool_t PM_TM_TaskMonitor( void );
void PM_TM_HelloTaskMonitor( uint8_t taskID );
void PM_TM_SetTask( uint8_t taskID, bool_t state );

#endif /* __TASKS_MONITOR_H */

/* *********** */
/* End of file */
/* *********** */
