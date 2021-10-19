/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par		LIB_TaskMonitoringExp.h
\n			Public interface header file of LIB_TaskMonitoring.c
\n
\file		LIB_TaskMonitoringExp.h
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

#ifndef _LIB_TASKSMONITORING_EXP_H
#define _LIB_TASKSMONITORING_EXP_H

/* ******** */
/* Includes */
/* ******** */

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

void LIB_TaskMonitoringConfig( uint8_t taskID, uint16_t timeout, bool_t state );
void LIB_SetTask( uint8_t taskID, bool_t state );

void LIB_TaskStartActivity(  uint8_t taskID );
void LIB_TaskEndActivity(  uint8_t taskID );

void LIB_HelloTaskMonitor( uint8_t taskID );

#endif	/* _LIB_TASKSMONITORING_EXP_H */

/* *********** */
/* End of file */
/* *********** */
