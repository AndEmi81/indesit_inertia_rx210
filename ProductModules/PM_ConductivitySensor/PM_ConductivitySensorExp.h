/*
Copyright (c) 2004-2011, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\Par        PM_ConductivitySensor export header file
\n    This file contains all defines,macro,typedef and functions's prototypes 
\n    that Application can use.
\n    Access to the Platform's Variables without the functions decribed below is forbidden.
\n
\file		        PM_ConductivitySensorExp.h
\ingroup	      PM_ConductivitySensor
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

#ifndef __CONDUCTIVITY_SENSOR_2_EXP_H_
#define __CONDUCTIVITY_SENSOR_2_EXP_H_



/* ******** */
/* Includes */
/* ******** */
#include "StandardTypes.h"
#include "CSN_cfg.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */

typedef enum {
  CSN_UPD_OK = 0,
  CSN_UPD_NOTREADY,
  CSN_ERR_OVERFLOW,
  CSN_ERR_NOT_INIT,
  CSN_ERR_FAULT

} CSN_Update_Result;

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/** Standard type used for Liner Pressure Sensor value */
#if defined(CONFIG_CSN_VALUE_32BIT)
typedef uint32_t CSN_ValueType;
#elif defined(CONFIG_CSN_VALUE_16BIT)
typedef uint16_t CSN_ValueType;
#endif

/* ****************** */
/* Exported variables */
/* ****************** */

/** Conductivity Sensor value - average value converted to required unit measure */
extern CSN_ValueType CSNValue;

/** Conductivity Sensor Counter value */
extern uint8_t CSNSamplesCnt;

/* ******************* */
/* Function prototypes */
/* ******************* */

/** Function that returns if Conductivity sensor module is in a fault condition
(timeout of valid frequency value detected or error inizialization) */
bool_t PM_CSN_GetFaultFlag(void);

/** Function called on Application to get CSN Status */
CSN_Update_Result PM_CSN_GetStatus(void);

/** Function called on Application to notify Ready to run state */
bool_t PM_CSN_IsRunning(void);

/** Function called on Application to get Conductivity Value */
CSN_ValueType PM_CSN_GetValue(void);

/** Function called on Application to get Maximum Conductivity Value Sample read */
CSN_ValueType PM_CSN_GetMaxReadValue(void);

/** Reset maximum sample read */
void PM_CSN_ResetMaxReadValue(void);

/** Function called on Application to get Unit Measure set from setting file */
CSN_ValueType PM_CSN_GetUnitMeasure_us(void);

/** Reset value updated flag to allow trigger next sample ready status */
void PM_CSN_WaitNextUpdate(void);

/** Reset sample counter and accumulator to reset reading status */
void PM_CSN_RestartReadings(void);



#endif /* __CONDUCTIVITY_SENSOR_2_EXP_H_ */

/* *********** */
/* End of file */
/* *********** */
