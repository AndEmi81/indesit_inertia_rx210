/*
Copyright (c) 2004-2009, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\Par		    Header file di Alternato.c
\n			    Contiene l'implementazione del driver Digital IO
\n
\file		    Alternato.h
\ingroup	    PM_Alternato
\date		    24/06/2009
\version	    00.00.01
\author		    Pierluigi Ianni
\n
\n
\Par\b	History:
\n
\n              00.00.01
\n              24/06/2009
\n              Pierluigi Ianni
\n              - Modificate le funzioni che avevano accesso diretto all'HW dei microcontrollori \n
con le chiamate alle funzioni del driver
\n              - Aggiornata la documentazione
\n
\n
\n 		        1.0.0
\n          	28/01/2008
\n          	Stefano Ughi
\n          	Terza versione
\n
\n
\n				1.4.0
\n				10/06/2004
\n				Progettista_1
\n				Seconda versione
\n
\n
\n				1.3.0
\n				10/04/2004
\n				Progettista_1
\n				Prima versione
\n
*/



#ifndef __ALTERNATO_H
#define __ALTERNATO_H

/* ******** */
/* Includes */
/* ******** */

#include "StandardTypes.h"



/* ****************** */
/* Defines and Macros */
/* ****************** */
/** Valore ammissibile per AlternatoError_t */
#define ALTERNATO_E_OK  0
/** Valore ammissibile per AlternatoError_t */
#define ALTERNATO_E_KO  1
/** Numero posizioni del servomeccanismo alternato, le posizioni reali saranno 0,1,2 */
#define NUM_ALT_POS		(uint8_t)3  









/** Posizione servomeccanismo alternato NON definita */
#define POS_ALT_NOT_DEF NUM_ALT_POS			   /* please don't touch */



/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
typedef uint8_t AlternatoError_t;
typedef const struct Loader*  pAlternatoData_t;


/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */
AlternatoError_t ALT_Init(pAlternatoData_t,uint8_t);
void ALT_SetPosition(L_PARAM);
void ALT_EngineDrv( void );


/*=============================================================================*/
/* QUESTE SONO LE FUNZIONI DISPONIBILI PER L'APPLICAZIONE OLTRE ALLA SetLoad() */
/*=============================================================================*/
/* Se TRUE posizionamento eseguito */
bool_t ALT_posReached( void );
/* Ritorna la posizione corrente del servomeccanismo valori ammissibili da 0 a (NUM_ALT_POS-1) 
altrimenti posizione non definita */
uint8_t ALT_getPos( void );



#endif /* __ALTERNATO_H */

/* *********** */
/* End of file */
/* *********** */