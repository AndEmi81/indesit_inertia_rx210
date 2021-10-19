/**
\defgroup DW_Hound	Modulo Inizializzazione dati da setting file DW

Il modulo contiene le funzioni di inizializzazione di variabili e strutture 
utilizzate dall'applicazione in base ai dati del setting file

\par \b PCLint: x Errors, 3 Warnings(indentation), x Info 12/06/2008 Valeria Catani


\date    31/01/2008
\author  Valeria Catani
\version 1.0.0
\ingroup ApplControl
*/

/**
Header file del file DW_Hound.c

\file    DW_Hound.h
\ingroup DW_Hound
\date    31/01/2008
\author  Valeria Catani
\version 1.0.0
*/

/*
Copyright (c) 2004-2008, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/* Old Versions
\version 
\date   
\author  
*/

/**
\addtogroup DW_Hound
@{*/

#ifndef __DW_HOUND_H
#define __DW_HOUND_H

/* ******** */
/* Includes */
/* ******** */

/* ****************** */
/* Defines and Macros */
/* ****************** */

/** Restoretype */
#define DEFAULT_RESTORE				0	  /* Checksum dati di backup K.O. 
                                      - restore con valori di default */
#define CHECK_RESTORE				  1 	/* Checksum dati di backup O.K. 
                                      - controllare le condizioni del restore */
									 
#define SPEED_MASK				0x03	
#define TIME_STARTUP_MASK		0x7F	  				 

#ifdef _CHECKERRORS_
#define _HND_CHECK_ERRORS_
#endif
/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */
uint8_t HND_dwConfigInit(void);
uint8_t HND_backupDataAnalysis(void);

#endif /* __DW_HOUND_H */

/*@}*/

/* *********** */
/* End of file */
/* *********** */



