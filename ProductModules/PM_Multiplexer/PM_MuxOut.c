/**
Il modulo si occupa di gestire più multiplexer per gli output. I multiplexer gestiti sono 2:4, con segnale di Enable.
\file    PM_MuxOut.c
\ingroup Multiplexer_3_8
\version 1.0.0
\date    25/10/2011
\author  Stefano Suraci
*/
/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
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
#include "PM_MuxOut.h"
#include "PM_MuxOutExp.h"

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
    uint8_t chipSelectOutPins[NUM_CS_OUT_PINS];
    /** contatore utilizzato per tener nota dei pin registrati in fase
    d'inizializzazione del modulo. */
    uint8_t numChipSelectOutPins;
    /** LoadID associato al singolo multiplexer, necessario per la configurazione
    del dispositivo ed il pilotaggio degli output. */
    uint8_t currentLoadId;
}muxOut_t;

/* **************************** */
/* Prototype of local functions */
/* **************************** */
static void MulSelectOut (uint8_t idx, uint8_t chipSelectConfig);

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
static uint8_t oldChipSelectOutConfig[NUM_CS_OUT];
static muxOut_t MuxOut_Vars[NUM_CS_OUT];
static uint8_t registeredMuxOutNumb = 0;

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
void PM_MulOutInit (void)
{
    (void)memset(&MuxOut_Vars[0],0,sizeof(MuxOut_Vars));
}

/**
La funzione ha il compito di registare il loadID di ogni chip select dichiarato nel file di setting,
in modo da implementare un collegamento 1:1 tra loadID e singolo mux gestito dal fw.

\param  [in] loadID loadID del carico chip_select presente nel setting, che dovrà essere associato al singolo mux
gestito dal firmware.

\return 	muxOut_Error_t

\retval		MPX_OUT_E_OK nessun problema
\retval		MPX_OUT_E_KO sono stati definiti nel setting troppi mux, rispetto a quelli gestibili dal fw.

\author 	Stefano Suraci
\date		25/10/2011
\version	1.0.0


\note		\par \b Nota 1:
			documentation by Stefano Suraci

*/
muxOut_Error_t PM_InsMulChOut(uint8_t loadID){
	if(registeredMuxOutNumb < NUM_CS_OUT){
		MuxOut_Vars[registeredMuxOutNumb].currentLoadId = loadID;
		registeredMuxOutNumb++;
		return MPX_OUT_E_OK;
	}
	else{
		return MPX_OUT_E_KO;
	}
}	/* END: PM_InsMulChOut(uint8_t loadID) */

/**
La funzione viene chiamata in LIB_IOM_OutputInitialization(...) se il reset è freddo.
Ha il compito di memorizzare i pin del micro utilizzati per pilotare i chip select\n
del singolo multiplexer (recuperato tramite il suo loadID).

\param  [in] loadID loadID del carico chip_select presente nel setting.
\param  [in] chipSelectOutPin pin del micro utilizzato per gestire i chip select del singolo multiplexer.

\return 	muxOut_Error_t

\retval		MPX_OUT_E_OK nessun problema
\retval		MPX_OUT_E_KO sono stati definiti nel setting troppi pin per il mux o il loadID non è stato registrato


\author 	Stefano Suraci
\date		25/10/2011
\version	1.0.0


\note		\par \b Nota 1:
			documentation by Stefano Suraci
*/
muxOut_Error_t PM_InsMulChSelOutPin(uint8_t loadID, uint8_t chipSelectOutPin)
{
	uint8_t currMuxOutIndex;
	for(currMuxOutIndex = 0; currMuxOutIndex < NUM_CS_OUT; currMuxOutIndex++)
	{
		if(MuxOut_Vars[currMuxOutIndex].currentLoadId == loadID)
		{
			if ( MuxOut_Vars[currMuxOutIndex].numChipSelectOutPins < NUM_CS_OUT_PINS )
			{
				MuxOut_Vars[currMuxOutIndex].chipSelectOutPins[MuxOut_Vars[currMuxOutIndex].numChipSelectOutPins] = chipSelectOutPin;
				MuxOut_Vars[currMuxOutIndex].numChipSelectOutPins++;
				return MPX_OUT_E_OK;
			}
			else
			{
				return MPX_OUT_E_KO;
			}
		}
	}
	return MPX_OUT_E_KO;
}	/* END: PM_InsMulChSelOutPin(uint8_t loadID, uint8_t chipSelectOutPin) */

/**
Seleziona la configurazione da applicare al multiplexer (per pilotare uscite digitali multiplexate).

\param  [in] loadID loadID del multiplexer da pilotare.
\param  [in] chipSelectConf configurazione da applicare al multiplexer.

\author 	Stefano Suraci
\date		25/10/2011
\version	1.0.0


\note		\par \b Nota 1:
			documentation by Stefano Suraci
*/
void PM_setOutMux(uint8_t loadID, uint8_t chipSelectOutConfig){
	uint8_t currMuxOutIndex;
	for(currMuxOutIndex = 0; currMuxOutIndex < NUM_CS_OUT; currMuxOutIndex++)
	{
		if(MuxOut_Vars[currMuxOutIndex].currentLoadId == loadID)
		{
			/* SURB if there's a variation disable MUX 1 ENABLE */
			if((((chipSelectOutConfig>>1) & MASK_MUX_1)^((oldChipSelectOutConfig[currMuxOutIndex]>>1) & MASK_MUX_1)) != 0)
			{
			  chipSelectOutConfig &= MASK_DISABLE_CS_1; /* enable 1 off*/
			}
			else
			{
			  /* do nothing */
			}
			MulSelectOut(currMuxOutIndex, chipSelectOutConfig);
			oldChipSelectOutConfig[currMuxOutIndex] = chipSelectOutConfig;
			break;
		}
	}
}	/* END: PM_setOutMux(uint8_t loadID, uint8_t chipSelectOutConfig) */

/* ************** */
/* Local Function */
/* ************** */

/**
Seleziona la configurazione da applicare al multiplexer.

\param  [in] idx indice (0, 1,... nel FW) del multiplexer da pilotare.
\param  [in] chipSelectOutConfig configurazione da applicare al multiplexer.

\author 	Stefano Suraci
\date		25/10/2011
\version	1.0.0


\note		\par \b Nota 1:
			documentation by Stefano Suraci
*/
static void MulSelectOut(uint8_t idx, uint8_t chipSelectOutConfig)
{
uint8_t i;
uint8_t pinState;
	for ( i = 0; i < (NUM_CS_OUT_PINS); i++ )
	{
		pinState = ((chipSelectOutConfig & (1<<i))?ON:OFF);
		LIB_DIO_SET_LOAD(pinState, MuxOut_Vars[idx].chipSelectOutPins[i]);
	}
}	/* END: static void MulSelectOut(uint8_t idx, uint8_t chipSelectOutConfig) */

/* *********** */
/* End of file */
/* *********** */
