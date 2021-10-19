/**
Il modulo si occupa di gestire un multiplexer 3:8, destinato agli input digitali multiplexati.
\file    PM_MuxIn.c
\ingroup Multiplexer_3_8
\version 1.0.0
\date    25/10/2011
\author  Stefano Suraci
*/
/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\defgroup Multiplexer_3_8 Multiplexer 3:8
\n Il modulo fornisce tutti i medoti per la gestione di un multiplexer 3:8.
\n
\par Configuration:
\todo
\par Exported Functions:
\todo
\n
\ingroup PM_Multiplexer
*/

/* Old Versions
\version 1.0.0
\date    09/08/2008
\author  Stefano Ughi
\note	 Codice originale presente nel modulo Multi.c in Piattaforma 1
*/

/* ******** */
/* Includes */
/* ******** */
#include <string.h>
#include "StandardTypes.h"
#include "Config.h"
#include "DigitalOutputs.h"
#include "PM_MuxIn.h"
#include "PM_MuxInExp.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/** struttura variabili utilizzate dal modulo */
typedef struct
{
    /** vettore contenente i pin del micro che agiscono sui Chip Select
    del componente. */
    uint8_t chipSelectInPins[NUM_CS_IN_PINS];
    /** contatore utilizzato per tener nota dei pin registrati in fase
    d'inizializzazione del modulo. */
    uint8_t numChipSelectInPins;
}muxIn_t;

/* **************************** */
/* Prototype of local functions */
/* **************************** */

/* **************************** */
/* External functions           */
/* **************************** */

/* ****************** */
/* Exported variables */
/* ****************** */

/* **************** */
/* Global variables */
/* **************** */
/** struttura per la gestione del modulo */
static muxIn_t MuxIn_Vars;

/* ***************** */
/* Exported function */
/* ***************** */

/**
La funzione viene chiamata in CALLBACK_Main_InitTask(...).
Ha il compito di azzerare le variabili utilizzate dal modulo.

\author 	Stefano Ughi
\date		08/08/2008
\version	1.0.0


\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/

void PM_MulInInit (void)
{
    (void)memset(&MuxIn_Vars,0,sizeof(MuxIn_Vars));
}

/**
La funzione viene chiamata in LIB_IOM_OutputInitialization(...) se il reset è freddo.
Ha il compito di memorizzare i pin del micro utilizzati per pilotare i chip select\n
del multiplexer.

\param  [in] chipSelectInPin pin del micro utilizzato per gestire i chip select del multiplexer.

\return 	muxIn_Error_t

\retval		MPX_IN_E_OK nessun problema
\retval		MPX_IN_E_KO sono stati definiti nel setting troppi pin per il mux

\author 	Stefano Suraci
\date		25/10/2011
\version	1.0.0


\note		\par \b Nota 1:
			documentation by Stefano Suraci

*/

muxIn_Error_t PM_InsMulChSelInPin(uint8_t chipSelectInPin)
{
    if ( MuxIn_Vars.numChipSelectInPins < NUM_CS_IN_PINS )
    {
    	MuxIn_Vars.chipSelectInPins[MuxIn_Vars.numChipSelectInPins] = chipSelectInPin;
    	MuxIn_Vars.numChipSelectInPins++;
        return MPX_IN_E_OK;
    }
    else
    {
        return MPX_IN_E_KO;
    }
}	/* END: PM_InsMulChSelInPin(uint8_t chipSelectInPin) */

/**
Seleziona la configurazione da applicare al multiplexer (per pilotare ingressi digitali multiplexati).

\param  [in] chipSelectInConfig configurazione da applicare al multiplexer.

\author 	Stefano Suraci
\date		12/10/2011
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Stefano Suraci
*/
void PM_setInMux(uint8_t chipSelectInConfig){
	uint8_t pinState, i;
	for ( i = 0; i < (NUM_CS_IN_PINS); i++ )
	{
		pinState = ((chipSelectInConfig & (1<<i))?ON:OFF);
		LIB_DIO_SET_LOAD(pinState, MuxIn_Vars.chipSelectInPins[i]);
	}
}	/* END: PM_setInMux(uint8_t chipSelectInConfig) */

/* *********** */
/* End of file */
/* *********** */
