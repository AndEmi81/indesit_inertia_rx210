/*
\defgroup Multi Multiplexer 3:8
Il modulo si occupa di gestire più multiplexer per gli output. I multiplexer gestiti sono 2:4, con segnale di Enable.
\par\b PCLint: x Errors, x Warnings, x Info Stefano Ughi 09/08/2008
\date    25/10/2011
\author  Stefano Suraci
\version 1.0.0
\ingroup ProductM
*/
/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/
/**
Header file di PM_MuxOut.c.
\file    PM_MuxOut.h
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



#ifndef __MULTI_OUT_H
#define __MULTI_OUT_H


/* ******** */
/* Includes */
/* ******** */

#include "StandardTypes.h"
#include "PM_MuxOutExp.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */
/** numero di pins del micro utilizzati per selezionare la configurazione del multiplexer */
#define NUM_CS_OUT_PINS     3

/** Numero di multiplexer per gli output disponibili al FW */
#define NUM_CS_OUT			2
/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/** Typedef per gli errori dei metodi */
typedef uint8_t muxOut_Error_t;
/** define su strutture di tipo muxOut_Error_t tutto O.K. */
#define MPX_OUT_E_OK    0
/** define su strutture di tipo muxOut_Error_t incontrato un problema */
#define MPX_OUT_E_KO    1

/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */

void PM_MulOutInit (void);
muxOut_Error_t PM_InsMulChOut(uint8_t loadID);
muxOut_Error_t PM_InsMulChSelOutPin (uint8_t loadID, uint8_t chipSelectPin );
void PM_setOutMux(uint8_t loadID, uint8_t chipSelectConfig);




/* *********** */
/* End of file */
/* *********** */
#endif  /* __MULTI_OUT_H */
