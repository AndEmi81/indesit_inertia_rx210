/*
Copyright (c) 2004-2009, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\Par		        Header file di PulseCounter
\n			        Contiene le funzioni per la gestione dispositivo flowmeter.
\n
\file		        PulseCounter.h
\ingroup	      PM_PulseCounter
\date		        21/07/2009
\version	      00.00.01
\author		      Pierluigi Ianni
\n
\n
\Par\b	History:
\n 
\n              00.00.01
\n              21/07/2009
\n              Pierluigi Ianni
\n              - Modificate le funzioni che avevano accesso diretto all'HW dei microcontrollori \n
con le chiamate alle funzioni del driver
\n              - Aggiornata la documentazione
\n
\n
\n				      1.0.0
\n				      17/01/2008
\n				      Stefano Ughi
\n				      Terza versione
\n
\n
\n				      1.4.0
\n				      10/06/2004
\n				      Progettista_1
\n				      Seconda versione
\n
\n
\n				      1.3.0
\n				      10/04/2004
\n				      Progettista_1
\n				      Prima versione
\n
*/


#ifndef __PULSECOUNTER_H
#define __PULSECOUNTER_H

/* ******** */
/* Includes */
/* ******** */
#include "StandardTypes.h"
#include "OCTPM_drv.h"


/* ****************** */
/* Defines and Macros */
/* ****************** */

/** Valore ammissibile per il tipo PulseCounterError_t */
#define PULSECOUNTER_E_OK   0
/** Valore ammissibile per il tipo PulseCounterError_t */
#define PULSECOUNTER_E_KO   1
/** 1 sec massimo periodo discriminabile -> 1 Hz */
#define PULSECOUNTER_MAX_PERIOD_MSEC    1000U    
/** filtro locale per validazione segnale ( corrisponde a c.a. 112 uSec ) */
#define PULSECOUNTER_LOCALFILTER        PULSECOUNTER_LOCALFILTER_DRIVER

/** numero di volte che debbo trovare frequenza troppo alta per dare warning */
#define PULSECOUNTER_FREQ_TOO_HIGH_FILTER   (uint8_t)100 
/** numero di volte che debbo trovare frequenza troppo bassa per dare warning */
#define PULSECOUNTER_FREQ_TOO_LOW_FILTER    (uint8_t)10
/** Numero tick per un giro completo del timer ( 16 bits counter ) */
#define ONE_TIMER_TURN                      0x10000UL

/** questo è il valore del tick del timer cui è agganciato il modulo pulse counter */
#define PULSECOUNTER_TICK_DURATION      PULSECOUNTER_TICK_DURATION_DRIVER

/** valore input funzione PulseCounter_Reset_Warning ()
resetta warning low frequency warning  e azzera relativo filtro 
*/
#define PULSECOUNTER_RESET_LOW_WARNING  0
/** valore input funzione PulseCounter_Reset_Warning ()
resetta warning high frequency warning  e azzera relativo filtro 
*/
#define PULSECOUNTER_RESET_HIGH_WARNING 1
/** valore input funzione PulseCounter_Reset_Warning ()
resetta warning low & high frequency e azzera relativ1 filtri 
*/
#define PULSECOUNTER_RESET_BOTH_WARNING 2

/** Valore ammissibile per il tipo PulseCounterState_t */
#define PULSECOUNTER_ENABLE     1
/** Valore ammissibile per il tipo PulseCounterState_t */
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
/* chiamata in InputInitialization() modulo IO_Manager */
PulseCounterError_t PulseCounter_Init(PulseCounterPin_t, pPulseCounterData_t );

/*========================================================*/
/* QUESTE SONO LE FUNZIONI DISPONIBILI PER L'APPLICAZIONE */
/*========================================================*/
/* abilita/disabilita il modulo (default abilitato) */
PulseCounterError_t PulseCounter_SetState ( PulseCounterState_t );
/* reset di eventuali warnings */
void PulseCounter_Reset_Warning( uint8_t );
/* ritorna il o gli warnings presenti, valori ammissibili:
0 -> No warnings 
1 -> High Frequency warning 
2 -> Low Frequency warning
3 -> High and Low Frequency warning */
uint8_t PulseCounter_GetWarning( void );
/* setta il valore della variabile globale PulseCount indicante il numero degli impulsi
   conteggiati */
void PulseCounter_SetCount( uint16_t );
/* ritorna il valore della variabile globale PulseCount indicante il numero degli impulsi
   conteggiati */
uint16_t PulseCounter_GetCount( void );



#endif /* __PULSECOUNTER_H */

/* *********** */
/* End of file */
/* *********** */
