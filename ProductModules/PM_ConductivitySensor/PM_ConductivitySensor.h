/*
Copyright (c) 2004-2011, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\Par        PM_ConductivitySensor header file
\n    This file contains all defines,macro,typedef and functions's prototypes 
\n    used in this module and other Platform modules.
\n
\file		        PM_ConductivitySensor.h
\ingroup	      PM_ConductivitySensor
\date		      07/10/2011
\version	      00.00.01
\author		      Alessandro Sabbatini
\n
\n
\Par\b	History:
\n
\n    00.00.01
\n    07/10/2011
\n    Alessandro Sabbatini
\n    - Creation
\n
*/


#ifndef __CONDUCTIVITY_SENSOR_2_H_
#define __CONDUCTIVITY_SENSOR_2_H_

/* ******** */
/* Includes */
/* ******** */
#include "StandardTypes.h"
#include "PM_ConductivitySensorExp.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
typedef struct PM_CSN_ParamsStruct PM_CSN_ParamsType;
struct PM_CSN_ParamsStruct
{
    uint16_t    empty1;
    uint8_t     timeout;
    uint8_t     empty2;
    uint16_t    max;
    uint16_t    min;
    uint8_t     period;
    uint8_t     numOfSamples;
    uint8_t     empty3;
    uint8_t     um;
};

/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */

/** Module initialization - called on input initialization */
void PM_CSN_Init(uint8_t pin,const uint8_t *pCondSensParams);

/** Gives status of channel - enabled or not */
void PM_CSN_SetChannelStatus(uint8_t status );

/** Reset channel - starts average algorithm from beginning */
void PM_CSN_ResetChannel(void);

/** Set / Reset Fault status flag */
void PM_CSN_SetFaultFlag(bool_t newVal);

/** Linear pressure sensor manager - called on main task */
void PM_CSN_Manager(void);



#endif /* __CONDUCTIVITY_SENSOR_2_H_ */

/* *********** */
/* End of file */
/* *********** */
