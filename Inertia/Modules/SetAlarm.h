/**
\defgroup SetAlarm	Modulo microprogramma SetAlarm

Il modulo gestisce il microprogramma SetAlarm

\par \b PCLint: x Errors, x Warnings, x Info 16/06/2008 Valeria Catani


\date    16/06/2008
\author  Valeria Catani
\version 1.0.0
\ingroup AppluProgr
*/

/**
Header file del file SetAlarm.c

\file    SetAlarm.h
\ingroup SetAlarm
\date    16/06/2008
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
\addtogroup SetAlarm
@{*/

#ifndef __UP_SETALARM_H
#define __UP_SETALARM_H

/* ******** */
/* Includes */
/* ******** */

/* ****************** */
/* Defines and Macros */
/* ****************** */



/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */

uint8_t SAL_uP_SetAlarm_Exe (uint8_t HighStatement,uint8_t LowStatement);

#endif /* __UP_SETALARM_H */

/*@}*/

/* *********** */
/* End of file */
/* *********** */
