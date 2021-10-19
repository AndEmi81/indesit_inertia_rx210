/*
Copyright (c) 2004-2009, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\Par		    Header file di Multiplexer.c
\n			    Contiene le funzioni per la gestione di un generico multiplexer n:2^n
\n
\attention \b   Il modulo è in grado di gestire un mumero di multiplexer \n
pari al valore della costante MAXNUMMUX definita in multiplexer.h e \n
un numero massimo di ingressi di selezione pari a 8
\n
\file		    Multiplexer.h
\ingroup	    Multiplexer_n2n
\date		    29/06/2009
\version	    00.00.01
\author		    Pierluigi Ianni
\n
\n
\Par\b	History:
\n 
\n              00.00.01
\n              29/06/2009
\n              Pierluigi Ianni
\n          	Prima versione
\n              - Sostituisce le vecchie versioni di gestione separata dei multiplexer 2:4 (multi2) \n
e 3:8 (multi)
\n
*/


#ifndef __MULTIPLEXER_H
#define __MULTIPLEXER_H


/* ******** */
/* Includes */
/* ******** */

#ifndef DIO_DEBUG
#include "debug.h"
#endif /* DIO_DEBUG */

#include "StandardTypes.h"

#ifndef DIO_DEBUG
/* PI begin: rimosso solo per permettere la compilazione */
#include "ProductConfig.h"
/* PI end: rimosso solo per permettere la compilazione */
#endif /* DIO_DEBUG */

#if !defined (OLD_MULTI)

/* ****************** */
/* Defines and Macros */
/* ****************** */

#define NUMMUX_2_4	2  		/* Numero di multiplexer con 2 ingressi di selezione */
#define NUMMUX_3_8	2  		/* Numero di multiplexer con 3 ingressi di selezione */
//#define NUMMUX_4_16	0  		/* Numero di multiplexer con 4 ingressi di selezione */

#define NUMMUXTOT	NUMMUX_2_4 + NUMMUX_3_8 /* + NUMMUX_4_16 */ /* ATTENZIONE!!! Numero di multiplexer gestiti */

#define MAXSELINPUT		3  /* ATTENZIONE!!! Numero massimo degli ingressi di selezione */

/** Questi sono le costanti da passare alle funzioni del multiplexer
per selezionare il tipo di multiplexer desiderato */
#define NOMUX     0
#define MUX_2_4   2	 /* Mux 2:4  2 ingressi di selezione */
#define MUX_3_8   3	 /* Mux 3:8  3 ingressi di selezione */
//#define MUX_4_16  4	 /* Mux 4:16 4 ingressi di selezione */


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/** Typedef per gli errori dei metodi */
typedef uint8_t mpx_n_Error_t;
/** define su strutture di tipo mpx_3_8_Error_t tutto O.K. */
#define MPX_N_E_OK    0
/** define su strutture di tipo mpx_3_8_Error_t incontrato un problema */
#define MPX_N_E_KO    1

/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */

void MulInit (void);
//mpx_n_Error_t SingleMulSet (uint8_t muxId, uint8_t muxType);
void MulSet (void);
mpx_n_Error_t InsMulChSelPin ( uint8_t chipSelectPin, uint8_t muxId );
mpx_n_Error_t MulSelect ( uint8_t chipSelectConfig, uint8_t muxId );



/* *********** */
/* End of file */
/* *********** */

#else /* OLD_MULTI is defined */

void MulInit ( void);
void InsMulChSelPin ( unsigned char MulPin);
void MulSelect ( unsigned char MulChSelVal);

#endif /* OLD_MULTI */

#endif  /* __MULTIPLEXER_H */