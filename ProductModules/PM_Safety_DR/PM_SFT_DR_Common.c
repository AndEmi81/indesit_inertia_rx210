/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		Safety Common
\n
\n
\file		PM_SFT_DR_Common.c
\ingroup	PM_Safety_DR
\date		05/06/2013
\author		Alessandro Sabbatini
*/

/* ******** */
/* Includes */
/* ******** */
#include "Config.h"
#include "OutputManager.h"
#include "PM_ConsistencyExp.h"
#include "Analog.h"
#include "LIB_Generic.h"
#include "LIB_ClassBVar.h"
#include "PM_SFT_DR_Common.h"
#include "PM_SFT_DR_Heaters.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/* Info buffer bitfields: It maps only those info invloved with security */

/* ********* */
/* Variables */
/* ********* */
/* Global */

/* Static */

/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Local Function */

/* *************** */
/* Function Bodies */
/* *************** */


/* TODO PM_SFT_DR_Init */
void PM_SFT_DR_Init(void)
{
	PM_SFT_DR_Heater_Common_Init();
}

/* ------------------------------------------------------------*/


/**
 *\n Decodes FaultOpcode+Value used in setting file to a value in 100ms unit time.
 *\n
 *\return uint16_t	Time value in 100ms
 *\n
 *
 *\note: Only 1 sec. and 1 min. units are used!
 *
 */
uint16_t PM_SFT_DR_GetTimeByCntrValue(uint16_t fltCounterVal)
{
	uint16_t retVal = (fltCounterVal & FLT_TIME_MASK);

	switch(fltCounterVal & FLT_OPCODE_MASK)
	{
	case FLT_OPCODE_1SEC:
		retVal = BASETIME_1S_TO_100MS(retVal);
		break;

	case FLT_OPCODE_10SEC:
		retVal = BASETIME_10S_TO_100MS(retVal);
		break;

	case FLT_OPCODE_1MIN:
		retVal = BASETIME_1MIN_TO_100MS(retVal);
		break;

	default:
		break;
	}

	return retVal;
}

/* Local Functions */





