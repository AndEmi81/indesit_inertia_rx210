/**
\defgroup Heat	Modulo microprogramma Heat

Gestisce il microprogramma Heat per il riscaldamento acqua.

\par \b PCLint: x Errors, x Warnings, x Info 07/02/2008 Valeria Catani


\date    07/02/2008
\author  Valeria Catani
\version 1.0.0
\ingroup AppluProgr
*/

/**
Header file del file Heat.c

\file    Heat.h
\ingroup Heat
\date    07/02/2008
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
\addtogroup Heat
@{*/

#ifndef __UP_HEAT_H
#define __UP_HEAT_H

/* ******** */
/* Includes */
/* ******** */

/* ****************** */
/* Defines and Macros */
/* ****************** */
#ifdef _CHECKERRORS_
	#define _HEA_CHECKERRORS_
#endif


#define HEAT_INIT		0
#define HEAT_EXE		1
#define HEAT_TRUE		2

#define NO_REF_PTR			0xFF


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */

uint8_t HEA_uP_Heat_Exe (uint8_t HighStatement,uint8_t LowStatement);

#endif /* __UP_HEAT_H */

/*@}*/

/* *********** */
/* End of file */
/* *********** */
