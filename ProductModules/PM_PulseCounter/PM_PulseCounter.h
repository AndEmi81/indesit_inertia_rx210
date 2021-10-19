/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		PM_PulseCounter
\n
\file		PM_PulseCounter.h
\ingroup	PM_PulseCounter
\date		24/10/2013
\author		Salvatore Fimiani
\n
*/

#if defined (CONFIG_PM_PULSECOUNTER)


#ifndef __PM_PULSECOUNTER_H
#define __PM_PULSECOUNTER_H

/* ******** */
/* Includes */
/* ******** */

#include "PM_PulseCounterExp.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */

/** Valore ammissibile per il tipo PulseCounterError_t */
#define PULSECOUNTER_E_OK   0
/** Valore ammissibile per il tipo PulseCounterError_t */
#define PULSECOUNTER_E_KO   1

/** Times number during which the frequency has to be too high before issuing a warning */
#define PULSECOUNTER_FREQ_TOO_HIGH_FILTER   (uint8_t)100 
/** Times number during which the frequency has to be too low before issuing a warning */
#define PULSECOUNTER_FREQ_TOO_LOW_FILTER    (uint8_t)10


/** Permitted value for PulseCounterState_t */
#define PULSECOUNTER_ENABLE     1
/** Permitted value for PulseCounterState_t */
#define PULSECOUNTER_DISABLE    0



/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
typedef uint8_t PulseCounterError_t;
typedef uint8_t PulseCounterPin_t;
typedef const void *const pPulseCounterData_t;
typedef uint8_t PulseCounterState_t;
 
/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */

PulseCounterError_t PM_PulseCounter_Init(PulseCounterPin_t, pPulseCounterData_t );
void PM_PulseCounter_SetChannelStatus ( uint8_t status);






#endif /* __PM_PULSECOUNTER_H */

#endif  /*defined (CONFIG_PM_PULSECOUNTER)*/

/* *********** */
/* End of file */
/* *********** */
