/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\Par		PM_EnergyMeterSwExp.h
\n
\n
\n
\file		PM_EnergyMeterSwExp.h
\ingroup	PM_EnergyMeterSw
\date		Oct 24, 2012
\version	01.00.00
\author		salvatore fimiani
\n
\n
\Par\b	History:
\n
\n 		01.04.00
\n		dd/mm/yyyy
\n		developer
\n		changes
\n
\n
\n
\n etc.
*/

#ifndef PM_ENERGYMETERSWEXP_H_
#define PM_ENERGYMETERSWEXP_H_

#include "autoconf.h"
/* ****************** */
/* Defines and Macros */
/* ****************** */
/* <belonging to the module, visible to the Application project> */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/* <belonging to the module, visible to the Application project> */
#if defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT) && defined(CONFIG_EMS_MLOAD_ITEMS_MAX)
typedef struct {
	uint16_t ui16PVal; /* Power value from DSP of the motor board */
	uint32_t ui32EVal; /* Energy value from DSP of the motor board */
}  sEMS_ExtInfo_t;
#endif

typedef enum
{
  EMS_E_OK,                   /* Success */
  EMS_E_UNKNOWN_MODE,         /* The requested command is not supported */
  EMS_E_PARAM_IGNORED,        /* Il parametro passato alla funzione e' stato ignorato */
  EMS_E_INVALID_VALUE         /* Il valore del parametro passato non e' valido */
} eEMS_Err;



typedef struct
{
	uint8_t resLoadID;
	uint8_t resCalOffset;
} sEMS_ResCalWBoxOff_t;



/* ****************** */
/* Exported variables */
/* ****************** */
/* <belonging to the module, visible to the Application project,keyword extern used> */

/* Energy counter from start of life in Watt/h*/
extern  uint32_t b_ui32EnergyCounterWh;          /* SI Backup */

/* Accumulator for energy in a second (Watt) */
extern  uint16_t b_ui16EnergyCounterWhAcc;       /* SI Backup */


/* ******************* */
/* Function prototypes */
/* ******************* */
/* <belonging to the module, visible to the Application project> */
void EM_LoadConfigRead(void);


/**
Perform module initialization for a cold reset

\param  [in]      void
\param  [out]     void
\param  [in,out]  none

\attention \b

\return   eEMS_Err - error code

\retval   [none]

\warning  [none]

\author   Intecs
\date     13-06-2011
\version  1.0.0

\bug    [none]

\note   \par \b Nota 1:
      documentation by ${Intecs}

*/
eEMS_Err PowerMeter_HWReset (void);


/**
 This function must be called in scheduler at a frequency EMS_PW_UPDATE_PERIOD.
 Calculate the average of Vrms2 and calculate the instant power on active.
 Every second elapsed the energy meter is update with the power average in
 the last second

\param  [in]      none
\param  [out]     none
\param  [in,out]  none

\attention \b  It accumulates energy in a global variable if the accumulation flasg (global) is enabled

\return   eEMS_Err - error code

\retval   [eEMS_Err]

\warning  [none]

\author   Intecs
\date     13-06-2011
\version  1.0.0

\bug    [none]

\note   \par \b Nota 1:
      documentation by ${Intecs}

*/
eEMS_Err PowerMeter_Accum (void);

/**
 Set the value of V peak

\param  [in]      uint16_t ui16VADabs [volt peak value in V]
\param  [out]     none
\param  [in,out]  none

\attention \b

\return   eEMS_Err - error code

\retval   [eEMS_Err]

\warning  [none]

\author   Alessandro Sabbatini
\date     03-09-2014
\version  1.0.0

\bug    [none]

\note   \par \b Nota 1:
      documentation by Alessandro Sabbatini

*/
eEMS_Err PowerMeter_SetAbsInstant_V (uint16_t ui16VADabs);

/**
 Set the value of V peak measured in ADC value

\param  [in]      uint32_t ui32VADcalib [the normalized volt peek value (at most 11 bits)]
\param  [out]     none
\param  [in,out]  none

\attention \b

\return   eEMS_Err - error code

\retval   [eEMS_Err]

\warning  [none]

\author   Intecs
\date     13-06-2011
\version  1.0.0

\bug    [none]

\note   \par \b Nota 1:
      documentation by ${Intecs}

*/
eEMS_Err PowerMeter_SetInstant_V (uint32_t ui32VADcalib);

/**
 Set the value of the current normalized V peak measured with native normalization format

\param  [in]      uint32_t ui32VADcalib [the normalized volt peek value]
\param  [out]     none
\param  [in,out]  none

\attention \b

\return   eEMS_Err - error code

\retval   [eEMS_Err]

\warning  [none]

\author   Alessandro Sabbatini
\date     03-09-2014
\version  1.0.0

\bug    [none]

\note   \par \b Nota 1:
      documentation by Alessandro Sabbatini

*/
eEMS_Err PowerMeter_SetNormInst_V (uint32_t ui32VADcalib);

#if defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT) && defined(CONFIG_EMS_MLOAD_ITEMS_MAX)
sEMS_ExtInfo_t* EMS_GetPtrToDSPInfoBuffer(uint8_t posMotor);
#endif



#endif /* PM_ENERGYMETERSWEXP_H_ */
