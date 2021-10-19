/*
Copyright (c) 2004-2011, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n    This file contains all defines,macro,typedef and functions's prototypes 
\n    that Application can use.
\n    Access to the Platform's Variables without the functions decribed below is forbidden.
\par		Properties
\n\li		CONFIG_LPS_VALUE_32BIT - Output value type
\n
\file		        PM_LinearPressureSensorExp.h
\ingroup	      	PM_LinearPressureSensor
\date		        23/08/2011
\version	      	00.00.01
\author		      	Alessandro Sabbatini
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

#ifndef __PM_LINEARPRESSURESENSOR_EXP_H
#define __PM_LINEARPRESSURESENSOR_EXP_H



/* ******** */
/* Includes */
/* ******** */
#include "Config.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */

typedef enum {
  LPS_UPD_OK = 0,
  LPS_UPD_NOTREADY,
  LPS_ERR_OUTOFRANGE,
  LPS_ERR_NOT_INIT,
  LPS_ERR_FAULT

} LPS_Update_Result;

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/** Standard type used for Liner Pressure Sensor value */

#if defined(CONFIG_LPS_VALUE_32BIT)
typedef uint32_t LPS_ValueType;
#elif defined(CONFIG_LPS_VALUE_16BIT)
typedef uint16_t LPS_ValueType;
#endif

/* ****************** */
/* Exported variables */
/* ****************** */
#if !defined(__PM_LINEARPRESSURESENSOR)
/** Linear Pressure Sensor value - average value converted to required unit measure */
extern const LPS_ValueType PSValue;
/** Flag used to disable algorithm update (even PSValue) */
extern const uint8_t PSDisable;
#endif

/* ******************* */
/* Function prototypes */
/* ******************* */

/** Function that returns if Linear pressure sensor module is in a fault condition
(timeout of valid frequency value detected) */
bool_t PM_LPS_GetFaultFlag(void);

/** Function called on Application to get LPS Status  */
LPS_Update_Result PM_LPS_GetStatus(void);

/** Reset MeanUpdate flag to allow trigger next PSValue update */
void PM_LPS_WaitNextUpdate(void);

/** Function called on Application to get PSValue */
LPS_ValueType PM_LPS_GetPSValue(void);

/** Function called on Application to notify Ready to run state */
bool_t PM_LPS_IsRunning(void);



#endif /* __PM_LINEARPRESSURESENSOR_EXP_H */

/* *********** */
/* End of file */
/* *********** */
