/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		LIB_LowPowerMode
\n			Implementation of all functions necessary to achieve the aim.
\n
\file		LIB_LowPowerMode.c
\ingroup	LIB_LowPowerMode
\date		May 12, 2014
\version	01.00.00
\author		roberto.fioravanti
\n
\n
*/

#define MODULE_BODY_LIB_LOWPOWERMODE
/* ******** */
/* Includes */
/* ******** */
#include "config.h"
#if defined(CONFIG_OCLPM_DRV_ENABLE)
#include "LIB_LowPowerMode.h"
#include "OCLPM_drv.h"
#include "OCRST_drv.h"
#include "LIB_Reset.h"
/* ****************** */
/* Defines and Macros */
/* ****************** */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* **************************** */
/* Prototype of local functions */
/* **************************** */

/* ****************** */
/* Exported variables */
/* ****************** */

/* **************** */
/* Global variables */
/* **************** */
static uint16_t Wake_Up_Event;
/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Exported function */

/* Local Function */

/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */
/*
Management of Low Power state.                                                        \n
This function shall be called after Warm Reset if  NewSystemMode = SYSTEM_SLEEP_MODE.

\param  [in]


\return

\retval


\author 	Leonardo Sparti
\date		05/07/2012
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Leonardo Sparti
*/
void LIB_LPM_Sleep_Manager(void)
{
	(void)OCLPM_GoLowPowerMode( Wake_Up_Event );
}

/**
Management of Standby request for Low Power operation.
\n Events allowing the subsequent Wake Up are identified and stored.

\param  [in] WakeUpSelector, identifier of the events which shall imply the future Wake Up


\return

\retval


\author 	Leonardo Sparti
\date		05/07/2012
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Leonardo Sparti

*/
void LIB_LPM_Sleep_Request(uint16_t WakeUpSelector)
{
	Wake_Up_Event = WakeUpSelector;

	LIB_Reset_SetNewSystemMode( (SysMode_Type)SYSTEM_SLEEP_MODE );

	/* Command Warm Reset */
	OCRST_GenerateUserResetSync();
}

/* Local Function */


#endif /*defined(CONFIG_OCLPM_DRV_ENABLE)*/
/* *********** */
/* End of file */
/* *********** */

