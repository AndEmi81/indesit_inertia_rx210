/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par		LIB_LowPowerMode.h
\n			header file of Feedbacks.c
\n
\file		LIB_LowPowerMode.h
\ingroup	LIB_LowPowerMode
\date		May 12, 2014
\version	01.00.00
\author		roberto.fioravanti
\n
\n
\par\b	History:
\n
*/

#ifndef __LOWPOWERMODE_H
#define __LOWPOWERMODE_H

/* ********** */
/* Includes */
/* ********* */
/*lint -e537*/
#include "StandardTypes.h"

/*lint +e537*/
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
void LIB_LPM_Sleep_Manager(void);
void LIB_LPM_Sleep_Request(uint16_t WakeUpSelector);

#endif /* __LOWPOWERMODE_H */

/* *********** */
/* End of file */
/* *********** */


