/**
\defgroup LoadRestore	Modulo microprogramma Load Restore

Gestisce il microprogramma Load Restore per il rabbocco dell'acqua.

\par \b PCLint: x Errors, x Warnings, x Info 18/03/2008 Valeria Catani


\date    18/03/2008
\author  Valeria Catani
\version 1.0.0
\ingroup AppluProgr
*/

/**
Header file del file LoadRestore.c

\file    LoadRestore.h
\ingroup LoadRestore
\date    18/03/2008
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
\addtogroup LoadRestore
@{*/

#ifndef __UP_LOADRESTORE_H
#define __UP_LOADRESTORE_H

/* ******** */
/* Includes */
/* ******** */

/* ****************** */
/* Defines and Macros */
/* ****************** */
#ifdef _CHECKERRORS_
	#define _LOR_CHECKERRORS_
#endif


#define LOAD_RESTORE_INIT		    0
#define LOAD_RESTORE_EXE		    1
#define LOAD_RESTORE_EXE_TIME		2
#define LOAD_RESTORE_TRUE		    3
#define LOAD_1LIMIT_POWER_SV		4
#define LOAD_2LIMIT_POWER_SV		5
#define LOAD_RESTORE_SV				6
#define LOAD_RESTORE_SV_WAIT        7
#define LOAD_RESTORE_SV_WAIT_1		8






/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */

uint8_t LOR_uP_LoadRestore_Exe (uint8_t HighStatement,uint8_t LowStatement);

#endif /* __UP_LOADRESTORE_H */

/*@}*/

/* *********** */
/* End of file */
/* *********** */
