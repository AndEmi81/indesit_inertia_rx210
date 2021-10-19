/**
Gestione dello standby via SW, che si basa sulle modalit� a basso consumo   \n
dei microcontrollori e che prescinde dall'utilizzo della cella di standby
\file    PM_LowPowerManager.c
\ingroup PM_LowPowerManager
\version 1.0.0
\date    05/07/2012
\PCLint  0 Errors, 0 Warnings, 0 Info
\author  Leonardo Sparti
*/
/*
Copyright (c) 2004-2009, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/
/* Old Versions
\version 1.4.0
\date    gg/mm/aaaa
\author  Progettista_1

\version 1.3.0
\date    10/04/2004
\author  Progettista_1
*/

/* ******** */
/* Includes */
/* ******** */
#include "PM_LowPowerManager.h"
#if defined(CONFIG_PM_LOW_POWER_MANAGER_SUPPORT)
#include "LIB_LowPowerMode.h"
#include "OCLPM_drv.h"
#endif
/* ****************** */
/* Defines and Macros */
/* ****************** */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* **************************** */
/* Prototype of local functions */
/* **************************** */

/* **************************** */
/* External functions           */
/* **************************** */

/* ****************** */
/* Exported variables */
/* ****************** */

/* **************** */
/* Global variables */
/* **************** */

/* ***************** */
/* Exported function */
/* ***************** */

/**
Management of Standby request for Low Power operation.            \n
Events allowing the subsequent Wake Up are identified and stored. 

\param  [in] WakeUpSelector, identifier of the events which shall imply the future Wake Up


\return

\retval


\author 	Leonardo Sparti
\date		  05/07/2012
\version	1.0.0
   
\note		\par \b Nota 1:
			documentation by Leonardo Sparti

*/
void PM_LPM_Sleep_Request(uint16_t WakeUpSelector)
{
#if defined(CONFIG_PM_LOW_POWER_MANAGER_SUPPORT)
	LIB_LPM_Sleep_Request(WakeUpSelector);
#endif
}


/* ************** */
/* Local Function */
/* ************** */

/* *********** */
/* End of file */
/* *********** */
