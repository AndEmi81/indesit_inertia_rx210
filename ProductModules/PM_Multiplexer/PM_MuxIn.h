
/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/
/**
Header file di PM_MuxIn.c.
\file    PM_MuxIn.h
\ingroup Multiplexer_3_8
\date    25/10/2011
\version 1.0.0
\author  Stefano Suraci
*/

/* Old Versions
\version 1.0.0
\date    09/08/2008
\author  Stefano Ughi
\note	 Codice originale presente nel modulo Multi.c in Piattaforma 1
*/



#ifndef __MULTI_H
#define __MULTI_H

/* ******** */
/* Includes */
/* ******** */

#include "StandardTypes.h"
#include "PM_MuxInExp.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */
/** numero di pins del micro utilizzati per selezionare la configurazione del multiplexer */
#define NUM_CS_IN_PINS     3

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/** Typedef per gli errori dei metodi */
typedef uint8_t muxIn_Error_t;
/** define su strutture di tipo muxIn_Error_t tutto O.K. */
#define MPX_IN_E_OK    0
/** define su strutture di tipo muxIn_Error_t incontrato un problema */
#define MPX_IN_E_KO    1

/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */

void PM_MulInInit (void);
muxIn_Error_t PM_InsMulChSelInPin (uint8_t chipSelectPin);
void PM_setInMux(uint8_t chipSelectInConfig);




/* *********** */
/* End of file */
/* *********** */
#endif  /* __MULTI_H */
