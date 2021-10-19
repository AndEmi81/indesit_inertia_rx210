/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par		LIB_TaskMonitoring.h
\n			header file of LIB_TaskMonitoring.c
\n
\file		LIB_TaskMonitoring.h
\ingroup	LIB_TaskMonitor
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

#ifndef _LIB_TASKSMONITORING_H
#define _LIB_TASKSMONITORING_H

/* ******** */
/* Includes */
/* ******** */
/*lint -e537*/
#include "Config.h"
#include "StandardTypes.h"
#include "TaskMonitor_Cfg.h"
#include "LIB_TaskMonitoringExp.h"
/*lint +e537*/
/* ****************** */
/* Defines and Macros */
/* ****************** */

#if !defined(IRQ_MONITOR_ENTER)
#define		IRQ_MONITOR_ENTER()		{}
#endif

#if !defined(IRQ_MONITOR_EXIT)
#define		IRQ_MONITOR_EXIT()		{}
#endif



/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */
void LIB_TaskMonitor( void );
void LIB_TaskMonitorInit( void );

void LIB_IRQMonitor_Init(void);
void LIB_IRQMonitor_Enter(void);
void LIB_IRQMonitor_Exit(void);
void LIB_IRQMonitor(void);

#endif /* _LIB_TASKSMONITORING_H */

/* *********** */
/* End of file */
/* *********** */
