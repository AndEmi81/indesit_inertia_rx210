/**
\defgroup Load	Modulo microprogramma Load

Gestisce il microprogramma Load per il caricamento dell'acqua.

\par \b PCLint: x Errors, x Warnings, x Info 11/03/2008 Valeria Catani


\date    11/03/2008
\author  Valeria Catani
\version 1.0.0
\ingroup AppluProgr
*/

/**
Header file del file Load.c

\file    Load.h
\ingroup Load
\date    11/03/2008
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
\addtogroup Load
@{*/

#ifndef __UP_LOAD_H
#define __UP_LOAD_H

/* ******** */
/* Includes */
/* ******** */

/* ****************** */
/* Defines and Macros */
/* ****************** */
#ifdef _CHECKERRORS_
	#define _LOA_CHECKERRORS_
#endif


#define LOAD_INIT		    0
#define LOAD_EXE		    1
#define LOAD_EXE_TIME		2
#define LOAD_EXE_PP   		3
#define LOAD_TRUE		    4
#define LOAD_PS_DINAMIC		5
#define LOAD_WAIT_PUMP		6


#define LOA_PP_CHECK_MODE		F0
#define LOA_PS_CHECK_MODE		F1

#define NO_REF_PTR			    0xFF

#define NO_PULSE_COUNTER_WARNING  0

#define TOUT_MAX_COUNTER	  0x7F


#define TOUT_FULL_STABILITY  40 /* base tempi 100 ms = 4 sec max valore 127*/


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */

uint8_t LOA_uP_Load_Exe (uint8_t HighStatement,uint8_t LowStatement);
uint16_t LOA_levTOtime_Conv(uint8_t level);
uint8_t LOA_timeTOlev_Conv(uint16_t time);

#endif /* __UP_LOAD_H */

/*@}*/

/* *********** */
/* End of file */
/* *********** */
