/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par		PM_LowPowerManagerExp.h
\n			This file contains all defines,macro,typedef and functions's prototypes that Application
\n          can use. 
\n          Access to the Platform's Variables without the functions decribed below is forbidden.
\n
\file		PM_LowPowerManagerExp.h
\ingroup	PM_LowPowerManager
\date		Jul 05, 2012
\version	01.00.00 
\author		Leonardo Sparti 
\n
\n
\par\b	History:
\n
\n 		01.00.00 
\n		Jul 05, 2012 
\n		Leonardo Sparti 
\n		changes 
\n 			Creation
\n
*/


#ifndef __LOW_POWER_MNG_EXP_H
#define __LOW_POWER_MNG_EXP_H

/* ******** */
/* Includes */
/* ******** */
#include "StandardTypes.h"
#include "OCLPM_drv.h"
/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


/* ******************* */
/* Function prototypes */
/* ******************* */
void PM_LPM_Sleep_Request(uint16_t WakeUpSelector);


#endif /* __LOW_POWER_MNG_EXP_H */

/* *********** */
/* End of file */
/* *********** */

