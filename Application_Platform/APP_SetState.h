/**
Header file del modulo microprogramma SetState

\file		APP_SetState.h
\ingroup	APP_SetState
\date		22/09/2009
\version	01.00.00
\author		Marco Quintini, Simone Costantini, Francesco Corvo

\par		History

*/

/*
Copyright (c) 2004-2007, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

#ifndef __APP_SETSTATE_H
#define __APP_SETSTATE_H

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
#if (MULTI_COMPARTMENT_APPL == 1)
uint8_t SST_SetState_Exe(uint8_t highStatement, uint8_t lowStatement, uint8_t compartment);
#else
uint8_t SST_SetState_Exe(uint8_t highStatement, uint8_t lowStatement);
#endif /* !MULTI_COMPARTMENT_APPL */

#endif /* __APP_SETSTATE_H */

/* *********** */
/* End of file */
/* *********** */
