/*
Copyright (c) 2004-2009, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		        Header file di PM_LowPowerManager.c
\n
\n
\file           PM_LowPowerManager.h
\ingroup        PM_LowPowerManager
\version        1.0.0
\date           05/07/2012
\author         Leonardo Sparti
\n

*/


/**
\addtogroup PM_LowPowerManager
@{*/

#ifndef __LOW_POWER_MNG_H
#define __LOW_POWER_MNG_H

/* ******** */
/* Includes */
/* ******** */
#include "PM_LowPowerManagerExp.h"

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
void PM_LPM_Sleep_Manager(void);
void PM_LPM_Sleep_Reset(void);


/*@}*/

#endif /* __LOW_POWER_MNG_H */

/* *********** */
/* End of file */
/* *********** */
