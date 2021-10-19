/*
Copyright (c) 2004-2011, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n    This file contains all defines,macro,typedef and functions's prototypes 
\n    used in this module and other Platform modules.
\n
\file		        PM_LinearPressureSensor.h
\ingroup	      PM_LinearPressureSensor
\date		        23/08/2011
\version	      00.00.01
\author		      Alessandro Sabbatini
\n
\n
\Par\b	History:
\n
\n    00.00.01
\n    23/08/2011
\n    Alessandro Sabbatini
\n    - Creation
\n
*/


#ifndef __PM_LINEARPRESSURESENSOR_H
#define __PM_LINEARPRESSURESENSOR_H

/* ******** */
/* Includes */
/* ******** */
#include "StandardTypes.h"
#include "PM_LinearPressureSensorExp.h"

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

/* Module initialization - called on input initialization */
void PM_LPS_Init(uint8_t pin,const uint8_t *pLinearPressureParams);

/** Gives status of channel - enabled or not */
void PM_LPS_SetChannelStatus(uint8_t status );

/** Reset channel - starts average algorithm from beginning */
void PM_LPS_ResetChannel(void);

/** Set / Reset Fault status flag */
void PM_LPS_SetFaultFlag(bool_t newVal);

/** Linear pressure sensor manager - called on main task */
void PM_LPS_Manager(void);



#endif /* __PM_LINEARPRESSURESENSOR_H */

/* *********** */
/* End of file */
/* *********** */
